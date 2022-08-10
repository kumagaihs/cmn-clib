/** @file *********************************************************************
 * @brief 拡張ログ出力 共通関数
 *
 *  ログを指定ファイルに出力する。ログの出力形式はログメッセージ定義ファイルで定義できる。<BR>
 *  ログメッセージ定義ファイルの構文は標準ログ出力共通関数を参照のこと。<BR>
 * <BR>
 *  ＜使用例＞<BR>
 * --- message.conf（メッセージログファイル） ---------------------<BR>
 * TEST01, テストログ%s<BR>
 * TEST02, テスト%d回目%s<BR>
 * ----------------------------------------------------------------<BR>
 * <BR>
 * ・以下、ログ出力処理例<BR>
 * CmnLog_LogEx *log = CmnLog_InitEx("message.conf", CMN_LOG_LEVEL_DETAIL, "test.log");<BR>
 * if (log == NULL) return ;<BR>
 * CmnLog_PutEx(log, CMN_LOG_LEVEL_DETAIL, "TEST01", "です");<BR>
 * CmnLog_PutEx(log, CMN_LOG_LEVEL_STANDARD, "TEST02", 1, "です");<BR>
 * CmnLog_PutEx(log, CMN_LOG_LEVEL_DEBUG, "TEST01", "です");<BR>
 * CmnLog_EndEx(log);<BR>
 * <BR>
 * ・以下、test.logファイルへの出力例（CMN_LOG_LEVEL_DEGUBで指定したメッセージは、この場合出力されない）<BR>
 * YYYY/MM/DD CODE=TEST01 : テストログです<BR>
 * YYYY/MM/DD CODE=TEST01 : テスト1回目です<BR>
 *
 * @sa 標準ログ共通関数 CmnLog.c
 * @author H.Kumagai
 * @date   2004-06-07
 * $Revision: 1.2 $
 *****************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>
#include<time.h>

#include"cmnclib/Common.h"
#include"cmnclib/CmnLog.h"
#include"cmnclib/CmnTime.h"

static const char* CMN_LOG_LEVEL_NAMES[] = {
	"",
	"ERROR",
	"WARN",
	"INFO",
	"DEBUG",
	"TRACE"
};

static char* timeFormat(int type, char* buf);
static void mutexLock(CmnThreadMutex* mutex);
static void mutexUnLock(CmnThreadMutex* mutex);

/**
 * @brief 拡張ログ出力関数初期化処理
 *
 *  ログメッセージ定義ファイルを読み込み、拡張ログ出力関数の使用準備をする。<BR>
 *  拡張ログ出力関数を使用する前にこの関数をコールしなければならない。<BR>
 *  また、拡張ログ出力関数の使用後は、CmnLogEx_Free()関数をコールすること。<BR>
 *  標準ログ出力関数と異なり、複数のログ出力設定をすることが出来る。
 *
 * @param logFile   (I)   ログを出力するファイルパスを指定する。<BR>
 *                        NULLを指定した場合は標準出力にログを出力する。<BR>
 *                        ファイルへの書き込みモードは、「追加書き込み」である。
 * @param level     (I)   ログ出力レベル。<BR>
 *                        指定可能な出力レベルについては、CmnLog_Init()関数を参照のこと。
 * @param msgFile   (I)   ログメッセージ定義ファイルへのパス。<BR>
 *                        定義ファイルを使用しない場合はNULLを指定する。<BR>
 *                        定義ファイルの構文は、CmnLog_Init()関数を参照のこと。
 * @return 初期化に成功した場合は、LogEx構造体へのポインタを返し、<BR>
 *         初期化に失敗した場合はNULLを返す。（以下のような場合に失敗する）<BR>
 *         msgFileのパスが不正な場合<BR>
 *         メッセージ定義ファイルの文法が不正な場合<BR>
 *         levelに不正な値が設定された場合<BR>
 *         fileにNULLが指定された場合
 * @sa 標準ログ出力関数初期化処理 CmnLog_Init()
 * @author H.Kumagai
 */
CmnLogEx* CmnLogEx_Create(const char* logFile, CMN_LOG_LEVEL level, const char* msgFile)
{
	CmnLogEx *log;

	/* 拡張ログ情報を取得、設定 */
	if ((log = malloc(sizeof(CmnLogEx))) == NULL) {
		return NULL;
	}
	memset(log, 0, sizeof(CmnLogEx));

	if (logFile != NULL) {
		if ((log->file = malloc(strlen(logFile) + 1)) == NULL) {
			CmnLogEx_Free(log);
			return NULL;
		}
		strcpy(log->file, logFile);
	}

	if (msgFile != NULL) {
		log->list = CmnLogMessage_Create(msgFile);
		if (log->list == NULL) {
			CmnLogEx_Free(log);
			return NULL;
		}
	}

	log->mutex = CmnThreadMutex_Create();
	if (log->mutex == NULL) {
		CmnLogEx_Free(log);
		return NULL;
	}

	/* 設定途中にログ出力しないよう、レベルの設定はログ設定が完了してから行う。 */
	log->level = level;

	return log;
}


/**
 * @brief 拡張ログ出力共通関数終了処理
 *
 *  標準ログ共通関数の終了処理を行う。（メモリ領域解放処理）<BR>
 *  標準ログ共通関数の使用を終えた時は、必ずこの関数を実行すること。
 *
 * @param log    (I/O) 拡張ログ情報へのポインタ
 * @author H.Kumagai
 */
void CmnLogEx_Free(CmnLogEx *log)
{
	if (log == NULL) return;

	/* 中途半端なログ出力をしないよう、出力レベルをNothingに設定 */
	log->level = CMN_LOG_LEVEL_NOTHING;

	if (log->list != NULL) {
		CmnLogMessage_Free(log->list);
	}
	if (log->file != NULL) {
		free(log->file);
	}
	if (log->mutex != NULL) {
		CmnThreadMutex_Free(log->mutex);
	}
	free(log);
}


/**
 * @brief 拡張ログ出力
 *
 *  ログを出力する。出力先はCmnLog_InitEx()関数の引数fileに指定したファイル。<BR>
 *  <ログ出力例><BR>
 *    yyyy/mm/dd[hh:mm:ss] [level] メッセージ本文
 *
 * @param log              (I) 拡張ログ情報構造体へのポインタ（CmnLog_InitEx()関数の戻り値）
 * @param level            (I) ログレベル。LoggerInitで指定されたログレベルと比較し、出力可否を決める。
 * @param msg              (I) メッセージ文言
 * @param ...              (I) メッセージ内に含まれる%sや%dの部分に対応する変数を指定する
 * @author H.Kumagai
 * @note メッセージコードが存在しない場合は何も出力されない
 */
void CmnLogEx_Put(CmnLogEx* log, CMN_LOG_LEVEL level, const char* msg, ...)
{
	va_list args;

	va_start(args, msg);
	cmnLogEx_PutLog(log, level, msg, args);
	va_end(args);
}

/**
 * @brief 拡張ログ出力
 *
 *  ログを出力する。出力先はCmnLog_InitEx()関数の引数fileに指定したファイル。<BR>
 *  <ログ出力例><BR>
 *    yyyy/mm/dd[hh:mm:ss] [level] msgCodeに対応するメッセージ文言
 *
 * @param log              (I) 拡張ログ情報構造体へのポインタ（CmnLog_InitEx()関数の戻り値）
 * @param level            (I) ログレベル。LoggerInitで指定されたログレベルと比較し、出力可否を決める。
 * @param msgCode          (I) メッセージコード
 * @param ...              (I) メッセージ内に含まれる%sや%dの部分に対応する変数を指定する
 * @author H.Kumagai
 * @note メッセージコードが存在しない場合は何も出力されない
 */
void CmnLogEx_PutByCode(CmnLogEx *log, CMN_LOG_LEVEL level, const char *msgCode, ...)
{
	va_list args;
	CmnLogMessage msg;

	/* 出力メッセージを取得 */
	if ( ! CmnLogMessage_Get(log->list, msgCode, &msg)) {
		return ;
	}

	/* メッセージ本文を出力 */
	va_start(args, msgCode);
	cmnLogEx_PutLog(log, level, msg.msg, args);
	va_end(args);
}


/**
 * @brief 拡張ログ出力（内部用関数）
 *
 *  ログを出力する。<BR>
 *  <ログ出力例><BR>
 *    yyyy/mm/dd[hh:mm:ss] [level] メッセージ本文
 *
 * @param log              (I) 拡張ログ情報構造体へのポインタ（CmnLog_InitEx()関数の戻り値）
 * @param level            (I) ログレベル。LoggerInitで指定されたログレベルと比較し、出力可否を決める。
 * @param msg              (I) メッセージ文言
 * @param args             (I) メッセージ内に含まれる%sや%dの部分に対応する変数を指定する
 * @author H.Kumagai
 * @note メッセージコードが存在しない場合は何も出力されない
 */
void cmnLogEx_PutLog(CmnLogEx* log, CMN_LOG_LEVEL level, const char* msg, va_list args)
{
	char str_date[CMN_TIME_FORMAT_SIZE_ALL];
	char format[512];
	FILE* file;

	if (log == NULL) return;

	/* ログレベルチェック */
	if (level > log->level) return;

	/* 書式フォーマットされた現在時刻文字列を取得 */
	timeFormat(CMN_TIME_FORMAT_ALL, str_date);

	/* START Synchronized */
	mutexLock(log->mutex);

	/* ログファイルオープン */
	if (log->file != NULL) {
		if ((file = fopen(log->file, "a")) == NULL) {
			return;
		}
	}
	else {
		file = stdout;
	}

	/* 時刻、ログレベルを出力 */
	sprintf(format, "%s [%5s] ", str_date, CMN_LOG_LEVEL_NAMES[level]);
	fprintf(file, format);

	/* メッセージ本文を出力 */
	vfprintf(file, msg, args);
	fprintf(file, "\n");

	if (log->file != NULL) {
		fclose(file);
	}

	/* END Synchronized */
	mutexUnLock(log->mutex);
}

static char* timeFormat(int type, char* buf)
{
	struct tm* ptime;
	time_t now;

	time(&now);
	ptime = localtime(&now);

	/* 年、月情報の修正 */
	ptime->tm_year += 1900;
	ptime->tm_mon += 1;

	sprintf(buf, "%04d/%02d/%02d %02d:%02d:%02d",
		ptime->tm_year, ptime->tm_mon, ptime->tm_mday,
		ptime->tm_hour, ptime->tm_min, ptime->tm_sec);

	return buf;
}

static void mutexLock(CmnThreadMutex* mutex)
{
#if IS_PRATFORM_WINDOWS()
	WaitForSingleObject(mutex->mutexId, INFINITE);
#else
	pthread_mutex_lock(&(mutex->mutexId));
#endif
}

static void mutexUnLock(CmnThreadMutex* mutex)
{
#if IS_PRATFORM_WINDOWS()
	ReleaseMutex(mutex->mutexId);
#else
	pthread_mutex_unlock(&(mutex->mutexId));
#endif
}
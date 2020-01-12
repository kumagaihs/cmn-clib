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
 * @sa 標準ログ共通関数 CommonLog.c
 * @author H.Kumagai
 * @date   2004-06-07
 * $Revision: 1.2 $
 *****************************************************************************/
#include"stdafx.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>

#include"Common.h"
#include"CommonLog.h"
#include"CommonTime.h"

/**
 * @brief 拡張ログ出力関数初期化処理
 *
 *  ログメッセージ定義ファイルを読み込み、拡張ログ出力関数の使用準備をする。<BR>
 *  拡張ログ出力関数を使用する前にこの関数をコールしなければならない。<BR>
 *  また、拡張ログ出力関数の使用後は、CmnLog_EndEx()関数をコールすること。<BR>
 *  標準ログ出力関数と異なり、複数のログ出力設定をすることが出来る。
 *
 * @param msgFile   (I)   ログメッセージ定義ファイルへのパス<BR>
 *                        定義ファイルの構文は、CmnLog_Init()関数を参照のこと。
 * @param level     (I)   ログ出力レベル。<BR>
 *                        指定可能な出力レベルについては、CmnLog_Init()関数を参照のこと。
 * @param file      (I)   ログを出力するファイル名を指定する。<BR>
 *                        ファイルへの書き込みモードは、「追加書き込み」である。
 * @return 初期化に成功した場合は、LogEx構造体へのポインタを返し、<BR>
 *         初期化に失敗した場合はNULLを返す。（以下のような場合に失敗する）<BR>
 *         msgFileのパスが不正な場合<BR>
 *         メッセージ定義ファイルの文法が不正な場合<BR>
 *         levelに不正な値が設定された場合<BR>
 *         fileにNULLが指定された場合
 * @sa 標準ログ出力関数初期化処理 CmnLog_Init()
 * @author H.Kumagai
 */
CmnLog_LogEx *CmnLog_InitEx(const char *msgFile, int level, const char *file)
{
	CmnLog_LogEx *log;

	/* 引数不正チェック */
	if (msgFile == NULL)          return NULL;
	if (level > CMN_LOG_LEVEL_MAX) return NULL;
	if (file == NULL)             return NULL;

	/* 拡張ログ情報を取得、設定 */
	if ((log = malloc(sizeof(CmnLog_LogEx))) == NULL) {
		return NULL;
	}
	memset(log, 0, sizeof(CmnLog_LogEx));
	log->level = level;

	if ((log->file = malloc(strlen(file) + 1)) == NULL) {
		CmnLog_EndEx(log);
		return NULL;
	}
	strcpy(log->file, file);

	log->list = cmnLog_CreateLogMessageList(msgFile);
	if (log->list == NULL) {
		CmnLog_EndEx(log);
		return NULL;
	}

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
void CmnLog_EndEx(CmnLog_LogEx *log)
{
	if (log == NULL) return;
	if (log->list != NULL) {
		cmnLog_ReleaseLogMessageList(log->list);
	}
	if (log->file != NULL) {
		free(log->file);
	}
	free(log);
}


/**
 * @brief 拡張ログ出力
 *
 *  ログを出力する。出力先はCmnLog_InitEx()関数の引数fileに指定したファイル。<BR>
 *  <ログ出力例><BR>
 *    yyyy/mm/dd[hh:mm:ss] CODE=メッセージコード : メッセージ本文
 *
 * @param log              (I) 拡張ログ情報構造体へのポインタ（CmnLog_InitEx()関数の戻り値）
 * @param level            (I) ログレベル。LoggerInitで指定されたログレベルと比較し、出力可否を決める。
 * @param msgCode          (I) メッセージコード
 * @param ...              (I) メッセージ内に含まれる%sや%dの部分に対応する変数を指定する
 * @author H.Kumagai
 * @note メッセージコードが存在しない場合は何も出力されない
 */
void CmnLog_PutEx(CmnLog_LogEx *log, int level, const char *msgCode, ...)
{
	va_list args;
	char str_date[CMN_TIME_FORMAT_SIZE_ALL];
	char format[512];
	CmnLog_LogMessage msg;
	FILE *file;

	if (log == NULL) return;

	/* ログレベルチェック */
	if (level > log->level) return;

	/* 書式フォーマットされた現在時刻文字列を取得 */
	CmnTime_GetFormatTime(CMN_TIME_FORMAT_ALL, str_date);

	/* 出力メッセージを取得 */
	if ( ! cmnLog_GetMessage(log->list, msgCode, &msg)) {
		return ;
	}

	if ((file = fopen(log->file, "a")) == NULL) {
		return ;
	}

	/* 時刻、メッセージコード部分を出力 */
	sprintf(format, "%s CODE=%s : ", str_date, msg.code);
	fprintf(file, format);

	/* メッセージ本文を出力 */
	va_start(args, msgCode);
	vfprintf(file, strcat(msg.msg, "\n"), args);
	va_end(args);

	fclose(file);
}


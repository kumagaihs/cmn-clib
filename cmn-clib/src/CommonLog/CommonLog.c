/** @file *********************************************************************
 * @brief 標準ログ出力 共通関数
 *
 *  プロセスの標準ログを出力する。ログの出力先や出力レベルはCmnLog_Initで指定する。
 *  ログを別ファイルに出力したい場合は、拡張ログ出力共通関数を使用することで複数出力先にログを出し分けることが可能。<BR>
 *  ログの出力形式はフリーフォーマットの他、ログメッセージ定義ファイルで定義することが可能。<BR>
 *  ログ出力メッセージの書式にはprintf書式を使用可能。<BR>
 * <BR>
 *  ＜使用例＞<BR>
 * --- message.conf（メッセージログファイル） ---------------------<BR>
 * C001, テストログ%s<BR>
 * C002, テスト%d回目%s<BR>
 * ----------------------------------------------------------------<BR>
 * <BR>
 * ・以下、ログ出力処理例<BR>
 * if ( ! CmnLog_Init("message.conf", CMN_LOG_LEVEL_INFO)) return ;<BR>
 * CmnLog_Put(CMN_LOG_LEVEL_INFO, "処理結果=%d, detail=%s", 100, 正常終了);
 * CmnLog_PutByCode(CMN_LOG_LEVEL_INFO, "C001", "です");<BR>
 * CmnLog_PutByCode(CMN_LOG_LEVEL_INFO, "C002", 1, "です");<BR>
 * CmnLog_PutByCode(CMN_LOG_LEVEL_DEBUG, "C001", "です");<BR>
 * CmnLog_End();<BR>
 * <BR>
 * ・以下、出力例（CMN_LOG_LEVEL_DEBUGで指定したメッセージは出力されない）<BR>
 * YYYY/MM/DD HH:MM:SS [INFO] 処理結果=100, detail=正常終了
 * YYYY/MM/DD HH:MM:SS [INFO] テストログです<BR>
 * YYYY/MM/DD HH:MM:SS [INFO] テスト1回目です<BR>
 *
 * @author H.Kumagai
 * @date   2004-06-06
 * $Revision: 1.2 $
 *****************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>

#include"cmnclib/Common.h"
#include"cmnclib/CommonLog.h"
#include"cmnclib/CommonTime.h"
#include"cmnclib/CommonString.h"

#define MSG_BUFSIZ   2048
#define PARSE_CHAR   ','
#define COMMENT_CHAR '#'

static CmnLogEx *gLogEx = NULL;				/* 標準ログ */
static CmnLogEx *gLogExCmnClib = NULL;		/* cmn-clib内部ログ */
static CMN_LOG_LEVEL gLogLevelCmnClib = CMN_LOG_LEVEL_NOTHING;	/* cmn-clib内部ログ出力レベル */

/**
 * @brief 標準ログ出力関数初期化処理
 *
 *  標準ログ出力の初期化を行う<BR>
 *  標準ログ出力関数を使用する前にこの関数をコールしなければならない。<BR>
 *  また、標準ログ出力関数の使用後（アプリケーション終了時）は、CmnLog_End関数をコールすること<BR>
 *  １つのアプリケーションでログ形式を複数使用する場合は、拡張ログ出力関数（CmnLogEx_Create）を使用すること。
 *
 * @param logFile   (I)   ログを出力するファイルパスを指定する。<BR>
 *                        NULLを指定した場合は標準出力にログを出力する。<BR>
 *                        ファイルへの書き込みモードは、「追加書き込み」である。
 *
 * @param level     (I)   ログ出力レベル。<BR>
 *
 * @param msgFile   (I)   ログメッセージ定義ファイルへのパス<BR>
 *                        定義ファイルは、以下の構文に従わなければならない。
 *                    <UL>
 *                      <LI># 以降をコメントとする</LI>
 *                      <LI>空行は無視する</LI>
 *                      <LI>スペースは無視する（ただし、メッセージ内のスペースはそのまま扱われる）</LI>
 *                      <LI>メッセージ定義は「メッセージコード」と「メッセージ」を,で区切り定義する</LI>
 *                      <LI>メッセージはprintfの書式に従うものとする</LI>
 *                    </UL>
 *                    例）<BR>
 *                        # コメント<BR>
 *                        ERROR-01, エラー発生。XXの値は[%s]です。%d回目の処理でした。#ここもコメント<BR>
 *                        ↑メッセージコード,　　↑メッセージ
 *
 * @retval True  初期化処理成功
 * @retval False 初期化処理失敗（以下のような場合に失敗する）<BR>
 *                   msgFileのパスが不正な場合<BR>
 *                   メッセージ定義ファイルの文法が不正な場合<BR>
 *                   levelに不正な値が設定された場合
 * @author H.Kumagai
 */
int CmnLog_Init(const char* logFile, CMN_LOG_LEVEL level, const char* msgFile)
{
	/* 2回の起動は認めない。正常に起動出来たとしてreturn */
	if (gLogEx) {
		return True;
	}

	/* LogExの初期化 */
	gLogEx = CmnLogEx_Create(logFile, level, msgFile);
	if (gLogEx == NULL) {
		return False;
	}

	return True;
}


/**
 * @brief 標準ログ出力共通関数終了処理
 *
 *  標準ログ共通関数の終了処理を行う。（メモリ領域解放処理）<BR>
 *  標準ログ共通関数の使用を終えた時は、必ずこの関数を実行すること。
 *
 * @author H.Kumagai
 */
void CmnLog_End()
{
	CmnLogEx_Free(gLogEx);
	gLogEx = NULL;
}


/**
 * @brief 標準ログ出力
 *
 *  ログを出力する。<BR>
 *  CmnLog_Init関数をコールする前に本関数をコールした場合は何も行わない。<BR>
 *  <ログ出力例><BR>
 *    yyyy/mm/dd[hh:mm:ss] [level] メッセージ本文
 *
 * @param level            (I) ログレベル。LoggerInitで指定されたログレベルと比較し、出力可否を決める。
 * @param msg              (I) メッセージ文言
 * @param ...              (I) メッセージ内に含まれる%sや%dの部分に対応する変数を指定する
 * @author H.Kumagai
 */
void CmnLog_Put(int level, const char *msg, ...)
{
	va_list args;

	if (gLogEx == NULL) {
		return;
	}

	va_start(args, msg);
	cmnLogEx_PutLog(gLogEx, level, msg, args);
	va_end(args);
}


/**
 * @brief 標準ログ出力
 *
 *  メッセージコードをもとにログを出力する。<BR>
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
void CmnLog_PutByCode(CmnLogEx* log, CMN_LOG_LEVEL level, const char* msgCode, ...)
{
	va_list args;
	CmnLogMessage msg;

	if (gLogEx == NULL) {
		return;
	}

	/* 出力メッセージを取得 */
	if (!CmnLogMessage_Get(log->list, msgCode, &msg)) {
		return;
	}

	/* メッセージ本文を出力 */
	va_start(args, msgCode);
	cmnLogEx_PutLog(log, level, msg.msg, args);
	va_end(args);
}

/**
 * cmn-clib内部ログ出力設定。<br>
 * この関数を実行するとcmn-clibの内部ログが指定したlogに出力される。
 * @param log cmn-clib内部ログ出力先。NULLを指定した場合は標準ログとなる。
 * @param level cmn-clib内部ログの出力レベル（log->levelより低いレベルのログを指定しても無効）
 */
void CmnLog_InitCmnClibLog(CmnLogEx *log, CMN_LOG_LEVEL level) {
	gLogExCmnClib = log;
	gLogLevelCmnClib = level;
}

/**
 * cmn-clib内部ログ出力終了
 */
void CmnLog_EndCmnClibLog()
{
	gLogLevelCmnClib = CMN_LOG_LEVEL_NOTHING;
}

/**
 * @brief cmn-clib内部ログ出力
 *
 *  cmn-clib内部ログを出力する。<BR>
 *  CmnLog_InitCmnClibLog関数をコールする前に本関数をコールした場合は何も行わない。<BR>
 *  この関数のラッパーマクロとして「CMNLOG_DEBUG」～「CMNLOG_ERROR」がある。<BR>
 *  <ログ出力例><BR>
 *    yyyy/mm/dd hh:mm:ss [level] メッセージ本文
 *
 * @param level            (I) ログレベル。CmnLog_InitCmnClibLogで指定されたログレベルと比較し、出力可否を決める。
 * @param msg              (I) メッセージ文言
 * @param ...              (I) メッセージ内に含まれる%sや%dの部分に対応する変数を指定する
 * @author H.Kumagai
 */
void CmnLog_PutCmnClibLog(CMN_LOG_LEVEL level, const char* msg, ...)
{
	va_list args;

	/* ログレベルチェック */
	if ((gLogExCmnClib == NULL && gLogEx != NULL && level > gLogEx->level)
			|| (gLogExCmnClib != NULL && level > gLogExCmnClib->level)) {
		return;
	}

	/* ログ出力 */
	va_start(args, msg);
	if (gLogExCmnClib != NULL) {
		cmnLogEx_PutLog(gLogExCmnClib, level, msg, args);
	}
	else {
		cmnLogEx_PutLog(gLogEx, level, msg, args);
	}
	va_end(args);
}

/** @file *********************************************************************
 * @brief 標準ログ出力 共通関数
 *
 *  ログを標準出力に出力する。ログの出力形式はログメッセージ定義ファイルで定義できる。<BR>
 *  ログをファイルに出力する場合は、拡張ログ出力共通関数を使用すること。<BR>
 * <BR>
 *  ＜使用例＞<BR>
 * --- message.conf（メッセージログファイル） ---------------------<BR>
 * TEST01, テストログ%s<BR>
 * TEST02, テスト%d回目%s<BR>
 * ----------------------------------------------------------------<BR>
 * <BR>
 * ・以下、ログ出力処理例<BR>
 * if ( ! CmnLog_Init("message.conf", CMN_LOG_LEVEL_DETAIL)) return ;<BR>
 * CmnLog_Put(CMN_LOG_LEVEL_DETAIL, "TEST01", "です");<BR>
 * CmnLog_Put(CMN_LOG_LEVEL_STANDARD, "TEST02", 1, "です");<BR>
 * CmnLog_Put(CMN_LOG_LEVEL_DEBUG, "TEST01", "です");<BR>
 * CmnLog_End();<BR>
 * <BR>
 * ・以下、出力例（CMN_LOG_LEVEL_DEGUBで指定したメッセージは出力されない）<BR>
 * YYYY/MM/DD CODE=TEST01 : テストログです<BR>
 * YYYY/MM/DD CODE=TEST01 : テスト1回目です<BR>
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

static CmnLogMessage *gList = NULL;    /* ログメッセージリスト先頭へのポインタ */
static int gLevel;                     /* ログ出力レベル                       */

/**
 * @brief 標準ログ出力関数初期化処理
 *
 *  ログメッセージ定義ファイルを読み込む。<BR>
 *  標準ログ出力関数を使用する前にこの関数をコールしなければならない。<BR>
 *  また、標準ログ出力関数の使用後（アプリケーション終了時）は、CmnLog_End関数をコールすること<BR>
 *  １つのアプリケーションでログ形式を複数使用する場合や、
 *  ログをファイルに出力する場合は、拡張ログ出力関数（CmnLog_InitEx）を使用すること。
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
 * @param level     (I)   ログ出力レベル。<BR>
 *                        以下のものから指定する。
 *                    <UL>
 *                      <LI>CMN_LOG_LEVEL_NOTHING  &nbsp;&nbsp; --沈黙レベル（全てのログを出力しない）</LI>
 *                      <LI>CMN_LOG_LEVEL_STANDARD &nbsp;&nbsp; --標準レベル（STANDARDで指定されたログを出力）</LI>
 *                      <LI>CMN_LOG_LEVEL_DETAIL   &nbsp;&nbsp; --詳細レベル（DETAILとSTANDARDで指定されたログを出力）</LI>
 *                      <LI>CMN_LOG_LEVEL_DEBUG    &nbsp;&nbsp; --デバッグレベル（全てのログを出力）</LI>
 *                    </UL>
 * @retval True  初期化処理成功
 * @retval False 初期化処理失敗（以下のような場合に失敗する）<BR>
 *                   msgFileのパスが不正な場合<BR>
 *                   メッセージ定義ファイルの文法が不正な場合<BR>
 *                   levelに不正な値が設定された場合
 * @author H.Kumagai
 */
int CmnLog_Init(const char *msgFile, int level)
{
	/* TODO:msgFile	にNULLが渡された場合はエラーにしないこと。（メッセージファイルなしでも手軽に使えるように） */

	/* ログレベル設定 */
	if (level > CMN_LOG_LEVEL_MAX) return False;
	gLevel = level;

	/* 2回の起動は認めない。正常に起動出来たとしてreturn */
	if (gList) {
		return True;
	}

	/* メッセージリストを取得 */
	gList = _CmnLogMessage_Create(msgFile);
	return gList ? True : False ;
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
	_CmnLogMessage_Free(gList);
	gList = NULL;
}


/**
 * @brief 標準ログ出力
 *
 *  ログを出力する。出力先はstdout。<BR>
 *  CmnLog_Init関数をコールする前に本関数をコールした場合は何も行わない。<BR>
 *  <ログ出力例><BR>
 *    yyyy/mm/dd[hh:mm:ss] CODE=メッセージコード : メッセージ本文
 *
 * @param level            (I) ログレベル。LoggerInitで指定されたログレベルと比較し、出力可否を決める。
 * @param msgCode          (I) メッセージコード
 * @param ...              (I) メッセージ内に含まれる%sや%dの部分に対応する変数を指定する
 * @author H.Kumagai
 */
void CmnLog_Put(int level, const char *msgCode, ...)
{
	/* TODO:msgCode	にNULLが渡された場合はエラーにしないこと。（メッセージファイルなしでも手軽に使えるように） */
	va_list args;
	char str_date[CMN_TIME_FORMAT_SIZE_ALL];
	char format[512];
	CmnLogMessage msg;

	if (gList == NULL) return ;

	/* ログレベルチェック */
	if (level > gLevel) return;

	/* 書式フォーマットされた現在時刻文字列を取得 */
	CmnTime_Format(CMN_TIME_FORMAT_ALL, str_date);

	/* 出力メッセージを取得 */
	if ( ! _CmnLogMessage_Get(gList, msgCode, &msg)) {
		return ;
	}

	/* 時刻、メッセージコード部分を出力 */
	sprintf(format, "%s CODE=%s : ", str_date, msg.code);
	fprintf(stdout, format);

	/* メッセージ本文を出力 */
	va_start(args, msgCode);
	vfprintf(stdout, strcat(msg.msg, "\n"), args);
	va_end(args);

}


/**
 * @brief ログメッセージ定義ファイル読み込み
 *
 *  ログメッセージファイルの読み込みを行い、
 *  LogMessage構造体にデータを格納する。
 *
 * @param msgFile    (I)   ログメッセージ定義ファイルへのパス<BR>
 * @retval LogMessageへのポインタ 読み込み完了
 * @retval NULL False 読み込み失敗（以下のような場合に失敗する）<BR>
 *              fileが無効なポインタの場合<BR>
 *              メッセージ定義ファイルの文法が不正な場合<BR>
 * @author H.Kumagai
 * @note この関数は、ログ出力共通関数から使用される。外部からの使用は禁止。
 */
CmnLogMessage *_CmnLogMessage_Create(const char *msgFile)
{
	FILE *fp;
	CmnLogMessage *list = NULL;
	CmnLogMessage *tmp;
	char  buf[MSG_BUFSIZ];
	char *code_pos;
	char *code_buf;
	char *msg_pos;
	char *msg_buf;

	fp = fopen(msgFile, "r");
	if (fp == NULL) {
		return NULL;
	}

	while (fgets(buf, MSG_BUFSIZ, fp)) {
		/* コメントもしくは空行であれば無視 */
		if(CmnString_Trim(buf)[0] == '\n' || CmnString_Trim(buf)[0] == COMMENT_CHAR) {
			continue ;
		}

		/* コードとメッセージへのポインタを取得（同時に区切り文字を'\0'に変換） */
		msg_pos = strchr(buf, PARSE_CHAR);
		if (msg_pos == NULL) {
			fclose(fp);
			return NULL;
		}
		*msg_pos = '\0';
		msg_pos++ ;
		code_pos = buf;

		/* 改行コードを除去 */
		if (msg_pos[strlen(msg_pos) - 1] == '\n') {
			msg_pos[strlen(msg_pos) - 1] = '\0';
		}

		/* コードとメッセージの領域を確保 */
		code_buf = malloc(strlen(code_pos) + 1);
		strcpy(code_buf, CmnString_Trim(code_pos));
		msg_buf = malloc(strlen(msg_pos) + 1);
		strcpy(msg_buf, CmnString_Trim(msg_pos));

		/* 最初のリストを作成 */
		if (list == NULL) {
			list = (CmnLogMessage *)calloc(1, sizeof(CmnLogMessage));
			list->code = code_buf;
			list->msg  = msg_buf;
			list->next = NULL;
			tmp = list;
		}
		/* 2番目以降のリストを作成 */
		else {
			tmp->next = (CmnLogMessage *)calloc(1, sizeof(CmnLogMessage));
			tmp = tmp->next;
			tmp->code = code_buf;
			tmp->msg  = msg_buf;
			tmp->next = NULL;
		}
	}
	fclose(fp);
	return list;
}


/**
 * @brief ログメッセージリスト解放処理
 *
 *  ログメッセージリストのメモリ領域を解放する
 *
 * @param list    (I)   解放するログメッセージリストへのポインタ
 * @author H.Kumagai
 * @note この関数は、ログ出力共通関数から使用される。外部からの使用は禁止。
 */
void _CmnLogMessage_Free(CmnLogMessage *list)
{
	CmnLogMessage *p;
	CmnLogMessage *tmp;

	if (list == NULL) {
		return;
	}

	for (p = list; p; p = tmp) {
		tmp = p->next;

		free(p->code);
		free(p->msg);
		free(p);
	}
}


/**
 * @brief ログメッセージ取得
 *
 *  メッセージコードに対応したメッセージコード文字列とメッセージ文字列を取得する
 *
 * @param list         (I) ログメッセージリスト
 * @param msg_code     (I) メッセージコード
 * @param msg          (O) LogMessage構造体へのポインタを指定する。<BR>
 *                         メッセージ取得成功時には、msg->codeとmsg->msgに値が格納される
 * @retval True 取得成功時
 * @retval False 取得失敗時（該当するメッセージコードが存在しない場合）
 * @author H.Kumagai
 * @note この関数は、ログ出力共通関数から使用される。外部からの使用は禁止。
 */
int _CmnLogMessage_Get(CmnLogMessage *list, const char *msg_code, CmnLogMessage *msg)
{
	CmnLogMessage *p;

	if (list == NULL) return False;

	for (p = list; p; p = p->next){
		if (strcmp(p->code, msg_code) == EQUAL) {
			msg->code = p->code;
			msg->msg  = p->msg;
			return True;
		}
	}
	return False;
}


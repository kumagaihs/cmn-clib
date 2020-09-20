/** @file *********************************************************************
 * @brief ログメッセージ定義 共通関数
 *
 *  ログメッセージ定義ファイルを扱う共通関数。<BR>
 *  ログ出力メッセージの書式は以下。printf書式が使用可能。<BR>
 * <BR>
 *  ＜ログメッセージ定義ファイル例＞<BR>
 * --- message.conf（メッセージログファイル） ---------------------<BR>
 * C001, テストログ%s<BR>
 * C002, テスト%d回目%s<BR>
 * ----------------------------------------------------------------<BR>
 *
 * @author H.Kumagai
 * @date   2004-06-06
 * $Revision: 1.2 $
 *****************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"cmnclib/Common.h"
#include"cmnclib/CommonLog.h"
#include"cmnclib/CommonString.h"

#define MSG_BUFSIZ   2048
#define PARSE_CHAR   ','
#define COMMENT_CHAR '#'

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
CmnLogMessage *CmnLogMessage_Create(const char *msgFile)
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
			if (list == NULL) {
				return NULL;
			}
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
void CmnLogMessage_Free(CmnLogMessage *list)
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
int CmnLogMessage_Get(CmnLogMessage *list, const char *msg_code, CmnLogMessage *msg)
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


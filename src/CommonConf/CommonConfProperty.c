/** @file *********************************************************************
 * @brief プロパティファイル操作 共通関数
 *
 *  プロパティファイルからのプロパティ取得と、
 *  プロパティファイルへの書き込みを行う。<BR>
 *  <BR>
 *  <プロパティファイルの書式><BR>
 *      ・コメントは'#'形式。'#'から行末までがコメントとして扱われる。<BR>
 *      ・プロパティの書式は、Name = Valueの形式。<BR>
 *      　(Nameはプロパティ名、Valueはプロパティ値。)<BR>
 *      ・空白は無視される。以下の表現は全て有効である。<BR>
 *      　　"Name=Value"<BR>
 *      　　"Name = Value"<BR>
 *      　　"   Name         = Value        "<BR>
 *      　ただし、以下の表現の場合は、空白は空白として認識される。<BR>
 *      　　"Na me=V a l u e"<BR>
 *      ・タブは使用不可<BR>
 *      ・プロパティ定義は、必ず1行で完結すること。<BR>
 *      　複数行にまたがる定義はみとめない。<BR>
 *      ・無効な書式での定義は無視される。（プロパティとして読み込まない）<BR>
 *      ・プロパティに重複があった場合は、より前に定義されているものが優先される<BR>
 *      ・一行に記述できる最大文字列長は4096Byteまで。<BR>
 * 
 * @author H.Kumagai
 * @date   2004-06-10
 * $Revision: 1.2 $
 *****************************************************************************/
#include"stdafx.h"

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include"Common.h"
#include"CommonConf.h"
#include"CommonString.h"

#define PROP_BUF_SIZE  4096		/* プロパティファイル１行の最大文字数  */
#define COMMENT_CHAR   '#'		/* コメント識別文字                    */
#define PAUSE_CHAR     '='		/* プロパティ名と値の区切り文字        */
/* 改行文字（Win32の場合は"\r\n"となるので、'\r'を改行文字として扱う） */
#ifdef WIN32
	#define RETURN_CHAR  '\r'
#else
	#define RETURN_CHAR  '\n'
#endif

/**
 * @brief プロパティリスト取得
 *
 *  プロパティファイルの読み込みを行い、
 *  CmnConf_PropertyList構造体にプロパティリストを格納する。<BR>
 *  プロパティリストの使用が終了したらCmnConf_FreePropertyList関数をコールし、
 *  メモリ領域の解放を行うこと。
 *
 * @param file      (I)プロパティ定義ファイル（フルパスで指定すること）
 * @return 取得したプロパティリスト(CmnConf_PropertyList)へのポインタ。<BR>
 *         プロパティの取得に失敗した場合はNULLを返す。
 * @author H.Kumagai
 */
CmnConf_PropertyList *CmnConf_GetPropertyList(const char *file)
{
	char  buf[PROP_BUF_SIZE + 1];
	char *comment_pos;
	char *name_pos;
	char *name_buf;
	char *value_pos;
	char *value_buf;
	FILE *prop_fp;
	CmnConf_PropertyList *list = NULL;
	CmnConf_PropertyList *tmp;
	
	prop_fp = fopen(file, "r");
	if (prop_fp == NULL) {
		return NULL;
	}

	while (fgets(buf, PROP_BUF_SIZE, prop_fp)) {
		/* 行頭、行末の空白を除去 */
		memmove(buf, CmnString_Trim(buf), strlen(CmnString_Trim(buf)) + 1);

		/* 行頭からのコメントもしくは空行であれば無視 */
		if (buf[0] == RETURN_CHAR || buf[0] == COMMENT_CHAR) {
			continue ;
		}
		/* 改行コードを除去 */
		if (buf[strlen(buf) - 1] == RETURN_CHAR) {
			buf[strlen(buf) - 1] = '\0';
		}
		/* コメントを除去 */
		comment_pos = strchr(buf, COMMENT_CHAR);
		if (comment_pos != NULL) {
			*comment_pos = '\0';
		}

		/* NameとValueへのポインタを取得（同時に'='を'\0'に変換） */
		value_pos = strchr( buf, PAUSE_CHAR );
		if (value_pos == NULL) {
			continue ;
		}
		*value_pos = '\0';
		value_pos++ ;
		name_pos = buf;
		
		/* コードとメッセージの領域を確保 */
		name_buf = malloc(strlen(CmnString_Trim(name_pos)) + 1);
		strcpy(name_buf, CmnString_Trim(name_pos));
		value_buf = malloc(strlen(CmnString_Trim(value_pos)) + 1);
		strcpy(value_buf, CmnString_Trim(value_pos));
		
		/* 最初のリストを作成 */
		if (list == NULL) {
			list = (CmnConf_PropertyList *)calloc(1, sizeof(CmnConf_PropertyList));
			list->name  = name_buf;
			list->value = value_buf;
			list->next  = NULL;
		}
		/* 2番目以降のリストを作成 */
		else {
			tmp = list;
			while (tmp->next != NULL) {
				tmp = tmp->next;
			}
			tmp->next = (CmnConf_PropertyList *)calloc(1, sizeof(CmnConf_PropertyList));
			tmp->next->name  = name_buf;
			tmp->next->value = value_buf;
			tmp->next->next  = NULL;
		}
	}
	fclose(prop_fp);
	return list;
}


/**
 * @brief プロパティ値取得
 *
 *  プロパティリストのメモリ領域を解放する。
 *
 * @param list      (I)   解放するプロパティリスト
 * @author H.Kumagai
 */
void CmnConf_FreePropertyList(CmnConf_PropertyList *list)
{
	CmnConf_PropertyList *p;
	CmnConf_PropertyList *tmp;
	
	if (list == NULL) {
		return;
	}

	for (p = list; p; p = tmp) {
		tmp = p->next;

		free(p->name);
		free(p->value);
		free(p);
	}
}


/**
 * @brief プロパティ値取得
 *
 *   nameに指定されたプロパティ値をlistから取得する
 *
 * @param list      (I)   プロパティリスト（CmnConf_GetPropertyList関数の戻り値）
 * @param name      (I)   取得するプロパティ名
 * @return プロパティ値文字列へのポインタ。nameに該当が無い場合はNULLを返す。
 * @author H.Kumagai
 */
char *CmnConf_GetProperty(const CmnConf_PropertyList *list, const char *name )
{
	for (; list; list = list->next) {
		if (strcmp(list->name, name) == 0) {
			return list->value;
		}
	}
	return NULL;
}


/** @file *********************************************************************
 * @brief 設定値操作 共通関数 I/Fヘッダファイル
 *
 *  設定値操作系共通関数を使用するためのI/Fヘッダファイル。<br>
 *  設定値操作系の共通関数を使用する場合は、このヘッダファイルを読み込むこと
 *
 * @author H.Kumagai
 * @date   2004-06-04
 * $Revision: 1.1 $
 *****************************************************************************/

#ifndef _COMMON_CONF_H
#define _COMMON_CONF_H

#include "Common.h"

/** プロパティリスト。プロパティファイルの情報を全て格納する。 */
typedef struct tag_CmnConf_PropertyList {
	char *name;					/* プロパティ名      */
	char *value;				/* プロパティ値      */
	struct tag_CmnConf_PropertyList *next;			/* リストのNextポインタ */
} CmnConf_PropertyList;

/* --- CommonConf.c --- */
/* 環境変数取得 */
D_EXTERN int CmnConf_GetEnv(char *buf, const char *envName);

/* --- CommonConfProperty.c --- */
D_EXTERN CmnConf_PropertyList *CmnConf_GetPropertyList(const char *name);
D_EXTERN void CmnConf_FreePropertyList(CmnConf_PropertyList *list);
D_EXTERN char *CmnConf_GetProperty(const CmnConf_PropertyList *list, const char *name );

#endif /* _COMMON_CONF_H */


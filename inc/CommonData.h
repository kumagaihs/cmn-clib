/** @file *********************************************************************
 * @brief 構造化データ操作 共通関数 I/Fヘッダファイル
 *
 *  構造化データ操作系共通関数を使用するためのI/Fヘッダファイル。<br>
 *  構造化データ操作系の共通関数を使用する場合は、このヘッダファイルを読み込むこと
 * 
 * @author H.Kumagai
 * @date   2004-06-12
 * $Revision: 1.1 $
 *****************************************************************************/

#ifndef _COMMON_DATA_H
#define _COMMON_DATA_H

#include "Common.h"

/** 単方向リストの要素 */
typedef struct tag_CmnData_ListItem {
	void *data;							/**< この要素が保持するデータへのポインタ */
	struct tag_CmnData_ListItem *next;	/**< 次の要素へのポインタ */
} CmnData_ListItem;

/** 単方向リスト（線状リスト） */
typedef struct tag_CmnData_List {
	int size;					/**< リストのサイズ(要素数) */
	CmnData_ListItem *first;	/**< リスト内の最初の要素へのポインタ */
} CmnData_List;


/* DLL使用かLIB使用かによるプロトタイプ切り替え */
#ifdef _USRDLL
  /* DLL作成 */
  #define D_EXTERN extern __declspec(dllexport)
#else
  #ifdef COMMON_DLL_IMPORT
    /* DLL使用 */
    #define D_EXTERN extern __declspec(dllimport)
  #else
    /* LIB使用 */
    #define D_EXTERN extern
  #endif
#endif

/* --- CommonDataList.c --- */
/* 環境変数取得 */
D_EXTERN CmnData_List *CmnData_CreateList();
D_EXTERN void CmnData_FreeList(CmnData_List *list, void *method);
D_EXTERN void CmnData_ListAddItem(CmnData_List *list, void *data);
D_EXTERN void *CmnData_ListGetItem(CmnData_List *list, int index);

#endif /* _COMMON_DATA_H */


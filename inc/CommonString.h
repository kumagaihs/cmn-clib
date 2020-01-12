/** @file *********************************************************************
 * @brief 文字列系 共通関数 I/Fヘッダファイル
 *
 *  文字列系共通関数を使用するためのI/Fヘッダファイル。<br>
 *  文字列系の共通関数を使用する場合は、このヘッダファイルを読み込むこと
 * 
 * @author H.Kumagai
 * @date   2004-06-05
 * $Revision: 1.3 $
 *****************************************************************************/

#ifndef _COMMON_STRING_H
#define _COMMON_STRING_H

#include "Common.h"
#include "CommonData.h"

/** 文字列リスト（構造は単方向リストと同じ）
 * @sa 単方向リスト CmnData_List
 */
typedef CmnData_List CmnString_List;

/** 文字列一致(strcmp関数用) */
#define EQUAL 0

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

/* --- CommonString.c --- */
/* 右側トリム*/
D_EXTERN char *CmnString_RTrim(char *str);
/* 左側トリム*/
D_EXTERN char *CmnString_LTrim(char *str);
/* 両側トリム*/
D_EXTERN char *CmnString_Trim(char *str);

/* --- CommonStringList.c --- */
/* 文字列リスト作成 */
D_EXTERN CmnString_List *CmnString_CreateList();
/* 文字列リスト解放 */
D_EXTERN void CmnString_FreeList(CmnString_List *list);
/* 文字列リスト要素追加 */
D_EXTERN void CmnString_ListAddItem(CmnString_List *list, const char *str);
/* 文字列リスト要素取得 */
D_EXTERN char *CmnString_ListGetItem(CmnString_List *list, int index);


#endif /* _COMMON_STRING_H */


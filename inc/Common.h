/** @file *********************************************************************
 * @brief 共通関数 メインヘッダファイル
 *
 *  共通関数のメインヘッダファイル。<br>
 *  真偽値など、基本的な定義が記載されている。
 *
 * @author H.Kumagai
 * @date   2004-06-04
 * $Revision: 1.2 $
 *****************************************************************************/

#ifndef _COMMON_H
#define _COMMON_H

#include "Common.h"

/* 真偽値 */
#ifndef True
  #define True      1  /**< 真 */
#endif
#ifndef False
  #define False     0  /**< 偽 */
#endif

/** 配列サイズを取得 */
#define GET_LENGTH(x)  (sizeof(x) / sizeof(*x))

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

#endif /* _COMMON_H */


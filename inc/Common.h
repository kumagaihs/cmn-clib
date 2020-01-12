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

#endif /* _COMMON_H */


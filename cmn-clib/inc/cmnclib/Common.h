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

#include <stddef.h>

/* 真偽値 */
#ifndef True
  #define True      1  /**< 真 */
#endif
#ifndef False
  #define False     0  /**< 偽 */
#endif

/** 文字コード */
typedef enum enmCHARSET {
    CHARSET_ASCII,
    CHARSET_SHIFT_JIS,
    CHARSET_UTF8,
} CHARSET;

/** 配列サイズを取得 */
#define ARRAY_LENGTH(x)  (sizeof(x) / sizeof(*x))

/* OS識別 */
#define PRATFORM_WINDOWS 1
#define PRATFORM_LINUX   2
#ifdef _WIN32
  #define PRATFORM   PRATFORM_WINDOWS
#else
  #define PRATFORM   PRATFORM_LINUX
#endif

/** OSがWindowsかを判定（Windowsの場合：True */
#define IS_PRATFORM_WINDOWS()  (PRATFORM == PRATFORM_WINDOWS)
/** OSがLinuxかを判定（Linuxの場合：True */
#define IS_PRATFORM_LINUX()    (PRATFORM == PRATFORM_LINUX)

/* Winsock2のための設定。CommonNetでwinsock2.hを使用するため、windows.hから旧バージョンのwinsock.hが読み込まれないようにする。 */
#if IS_PRATFORM_WINDOWS()
  #define _WINSOCKAPI_ /* Prevent includsion of winsock.h in windows.h */
#endif

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


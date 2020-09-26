/** @file *********************************************************************
 * @brief Win32共通関数 ヘッダファイル
 *
 *  Win32共通関数のヘッダファイル<br>
 *
 * @author H.Kumagai
 * @date   2020-07-30
 *****************************************************************************/

#ifndef _COMMON_WIN32_H_
#define _COMMON_WIN32_H_

#include "cmnclib/Common.h"

#if IS_PRATFORM_WINDOWS()

#include "cmnclib/CommonString.h"

D_EXTERN int CmnWin32Clipboard_GetString(CmnStringBuffer *buf);
D_EXTERN int CmnWin32Clipboard_SetString(CmnStringBuffer *buf);

#endif /* IS_PRATFORM_WINDOWS */
#endif /* _COMMON_WIN32_H_ */

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

/** 自動領域拡張文字列バッファ */
typedef struct _tag_CmnStringBuffer {
	CmnDataBuffer *_buf;	/**< 自動領域拡張バッファへのポインタ。内部的な処理で使うため使用不可。 */
	char *string;			/**< バッファへのポインタ。Append/Setによる領域拡張時にアドレスが変わる可能性があるため、利用側で保存せず、常に最新のポインタを参照すること。 */
	size_t length;			/**< 文字数 */
} CmnStringBuffer;

/* --- CommonString.c --- */
D_EXTERN char *CmnString_RTrim(char *str);
D_EXTERN char *CmnString_LTrim(char *str);
D_EXTERN char *CmnString_Trim(char *str);
D_EXTERN char* CmnString_Replace(const char *src, const char *old, const char *new, char *dest);
D_EXTERN char* CmnString_ReplaceNew(const char *src, const char *old, const char *new);
D_EXTERN char* CmnString_StrcatNew(const char *left, const char *right);
D_EXTERN int CmnString_Split(char *buf, size_t rowlen, size_t collen, const char *str, const char *delim);
D_EXTERN char* CmnString_Lpad(char *buf, const char *str, const char padch, size_t digit);

/* --- CommonStringList.c --- */
D_EXTERN CmnString_List *CmnString_CreateList();
D_EXTERN void CmnString_FreeList(CmnString_List *list);
D_EXTERN void CmnString_ListAddItem(CmnString_List *list, const char *str);
D_EXTERN char *CmnString_ListGetItem(CmnString_List *list, int index);

/* --- CommonStringBuffer.c --- */
D_EXTERN CmnStringBuffer* CmnStringBuffer_Create(const char *str);
D_EXTERN int CmnStringBuffer_Append(CmnStringBuffer *buf, const char *str);
D_EXTERN int CmnStringBuffer_Set(CmnStringBuffer *buf, const char *str);
D_EXTERN void CmnStringBuffer_Free(CmnStringBuffer *buf);


#endif /* _COMMON_STRING_H */


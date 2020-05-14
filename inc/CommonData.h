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

/** スタックの要素 */
typedef struct _tag_CmnData_StackItem {
	struct _tag_CmnData_StackItem *prev;	/**< 前の要素へのポインタ */
	struct _tag_CmnData_StackItem *next;	/**< 次の要素へのポインタ */
	void *data;								/**< この要素が保持するデータへのポインタ */
} CmnData_StackItem;

/** スタック */
typedef struct _tag_CmnData_Stack {
	CmnData_StackItem *first;		/**< スタックの最初の要素へのポインタ */
	CmnData_StackItem *last;		/**< スタックの最後の要素へのポインタ */
	unsigned long size;			/**< スタックのサイズ(要素数) */
} CmnData_Stack;

/** 自動領域拡張バッファ */
typedef struct _tag_CmnDataBuffer {
	void *data;			/**< バッファへのポインタ。Append/Setによる領域拡張時にアドレスが変わる可能性があるため、利用側で保存せず、常に最新のポインタを参照すること。 */
	size_t bufSize;		/**< バッファ領域のサイズ */
	size_t size;		/**< 有効なデータのサイズ */
} CmnDataBuffer;

/* --- CommonDataList.c --- */
D_EXTERN CmnData_List *CmnData_CreateList();
D_EXTERN void CmnData_FreeList(CmnData_List *list, void *method);
D_EXTERN void CmnData_ListAddItem(CmnData_List *list, void *data);
D_EXTERN void *CmnData_ListGetItem(CmnData_List *list, int index);

/* --- CommonDataStack.c --- */
D_EXTERN CmnData_Stack* CmnData_CreateStack();
D_EXTERN void CmnData_FreeStack(CmnData_Stack *stack, void *method);
D_EXTERN void CmnData_PushStack(CmnData_Stack *stack, void *data);
D_EXTERN void* CmnData_PopStack(CmnData_Stack *stack);

/* --- CommonDataBuffer.c --- */
D_EXTERN CmnDataBuffer* CmnDataBuffer_Create(size_t bufSize);
D_EXTERN int CmnDataBuffer_Append(CmnDataBuffer *buf, const void *data, size_t len);
D_EXTERN int CmnDataBuffer_Set(CmnDataBuffer *buf, const void *data, size_t len);
D_EXTERN void CmnDataBuffer_Delete(CmnDataBuffer *buf, size_t len);
D_EXTERN void CmnDataBuffer_Free(CmnDataBuffer *buf);

#endif /* _COMMON_DATA_H */


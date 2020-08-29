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

#include "cmnclib/Common.h"

/** 単方向リストの要素 */
typedef struct tag_CmnDataListItem {
	void *data;							/**< この要素が保持するデータへのポインタ */
	struct tag_CmnDataListItem *next;	/**< 次の要素へのポインタ */
} CmnDataListItem;

/** 単方向リスト（線状リスト） */
typedef struct tag_CmnDataList {
	int size;					/**< リストのサイズ(要素数) */
	CmnDataListItem *first;		/**< リスト内の最初の要素へのポインタ */
} CmnDataList;

/** スタックの要素 */
typedef struct _tag_CmnDataStackItem {
	struct _tag_CmnDataStackItem *prev;	/**< 前の要素へのポインタ */
	struct _tag_CmnDataStackItem *next;	/**< 次の要素へのポインタ */
	void *data;								/**< この要素が保持するデータへのポインタ */
} CmnDataStackItem;

/** スタック */
typedef struct _tag_CmnDataStack {
	CmnDataStackItem *first;		/**< スタックの最初の要素へのポインタ */
	CmnDataStackItem *last;		/**< スタックの最後の要素へのポインタ */
	unsigned long size;			/**< スタックのサイズ(要素数) */
} CmnDataStack;

/** 自動領域拡張バッファ */
typedef struct _tag_CmnDataBuffer {
	void *data;			/**< バッファへのポインタ。Append/Setによる領域拡張時にアドレスが変わる可能性があるため、利用側で保存せず、常に最新のポインタを参照すること。 */
	size_t bufSize;		/**< バッファ領域のサイズ */
	size_t size;		/**< 有効なデータのサイズ */
} CmnDataBuffer;

/* --- CommonDataList.c --- */
D_EXTERN CmnDataList *CmnDataList_Create();
D_EXTERN void CmnDataList_Free(CmnDataList *list, void *method);
D_EXTERN void CmnDataList_Add(CmnDataList *list, void *data);
D_EXTERN void *CmnDataList_Get(CmnDataList *list, int index);

/* --- CommonDataStack.c --- */
D_EXTERN CmnDataStack* CmnDataStack_Create();
D_EXTERN void CmnDataStack_Free(CmnDataStack *stack, void *method);
D_EXTERN void CmnDataStack_Push(CmnDataStack *stack, void *data);
D_EXTERN void* CmnDataStack_Pop(CmnDataStack *stack);

/* --- CommonDataBuffer.c --- */
D_EXTERN CmnDataBuffer* CmnDataBuffer_Create(size_t bufSize);
D_EXTERN int CmnDataBuffer_Append(CmnDataBuffer *buf, const void *data, size_t len);
D_EXTERN int CmnDataBuffer_Set(CmnDataBuffer *buf, const void *data, size_t len);
D_EXTERN void CmnDataBuffer_Delete(CmnDataBuffer *buf, size_t len);
D_EXTERN void CmnDataBuffer_Free(CmnDataBuffer *buf);

#endif /* _COMMON_DATA_H */


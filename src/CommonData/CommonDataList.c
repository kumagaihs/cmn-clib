/** @file *********************************************************************
 * @brief 単方向リスト操作 共通関数
 *
 *  単方向リスト（線状リスト）を作成、操作するための共通関数。<BR>
 *  構造も単純であるため、可変配列としての使用に向いている。
 *
 * @author H.Kumagai
 * @date   2004-06-12
 * $Revision: 1.1 $
 *****************************************************************************/
#include<stdlib.h>

#include"Common.h"
#include"CommonData.h"


/**
 * @brief 単方向リスト作成
 *
 *  単方向リストを新規に作成する。
 *
 * @return 作成した単方向リストへのポインタ。作成に失敗した場合はNULLを返す。
 * @author H.Kumagai
 */
CmnDataList *CmnDataList_Create()
{
	CmnDataList *list;
	list = malloc(sizeof(CmnDataList));
	if (list != NULL) {
		list->first = NULL;
		list->size  = 0;
	}
	return list;
}


/**
 * @brief 単方向リスト解放
 *
 *  単方向リストを破棄し、メモリ領域を解放する
 *
 * @param list     (I/O) 解放する単方向リストへのポインタ
 * @param method   (I)   リスト内で保持しているデータを解放する関数へのポインタ。<BR>
 *                       （単にmalloc関数を使用して確保したメモリなら、freeを指定すれば良い）<BR>
 *                       指定する関数のI/Fは、以下の条件を満たしていること。<BR>
 *                       <UL>
 *                         <LI>戻り値はなし(void)</LI>
 *                         <LI>引数は、ポインタ変数が１つのみ
 *                             （ポインタ変数であれば、型は問わない）</LI>
 *                       </UL>
 *                       NULLが指定された場合は、データの解放処理は行わず、リストの解放のみを行う。<BR>
 *                       （グローバルメモリや関数ポインタをリストとして格納する場合などにNULLを指定する）
 * @author H.Kumagai
 */
void CmnDataList_Free(CmnDataList *list, void *method)
{
	CmnDataListItem *item, *tmp;
	void (*freeMethod)() = method;

	if (list == NULL) return;

	for (item = list->first; item; item = tmp) {
		tmp = item->next;
		if (method != NULL) {
			freeMethod(item->data);
		}
		free(item);
	}
	free(list);
}


/**
 * @brief 単方向リスト要素追加
 *
 *  単方向リストの末尾に要素を追加する。
 *  引数が不正な場合は何もしない。
 *
 * @param list    (I/O) 要素を追加するリストへのポインタ
 * @param data    (I)   追加する要素（データ）
 * @author H.Kumagai
 */
void CmnDataList_Add(CmnDataList *list, void *data)
{
	CmnDataListItem *item, *p, *tmp;

	if (list == NULL) return;
	if ((item = malloc(sizeof(CmnDataListItem))) == NULL) return;
	item->data = data;
	item->next = NULL;

	if (list->first == NULL) {
		list->first = item;
	}
	else {
		tmp = list->first;
		for (p = list->first; p->next; p = p->next) {
			tmp = p->next;
		}
		tmp->next = item;
	}
	list->size++ ;
}


/**
 * @brief 単方向リスト要素取得
 *
 *  単方向リストから要素（データ）を取得する
 *
 * @param list    (I)   要素を取得するリストへのポインタ
 * @param index   (I)   取得する要素のインデックス（最初の要素を0とする）
 * @return 取得したデータへのポインタ（listやindexが不正な場合はNULLを返す）
 * @author H.Kumagai
 */
void *CmnDataList_Get(CmnDataList *list, int index)
{
	CmnDataListItem *item;
	int i;

	if (list == NULL) return NULL;

	item = list->first;
	for (i = 0; i < index; i++) {
		if (item == NULL) {
			return NULL;
		}
		else {
			item = item->next;
		}
	}

	return (item) ? item->data : NULL ;
}


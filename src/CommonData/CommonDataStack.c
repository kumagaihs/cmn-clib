/** @file *********************************************************************
 * @brief スタック操作 共通関数
 *
 *  スタックを作成、操作するための共通関数。
 *
 * @author H.Kumagai
 * @date   2020-01-16
 *****************************************************************************/
#include<stdlib.h>

#include "CommonData.h"

/**
 * @brief スタック作成
 *
 *  スタックを新規に作成する。
 *
 * @return 作成したスタックへのポインタ。作成に失敗した場合はNULLを返す。
 * @author H.Kumagai
 */
CmnData_Stack* CmnData_CreateStack()
{
	CmnData_Stack *ret;
	ret = malloc(sizeof(CmnData_Stack));
	ret->first = NULL;
	ret->last = NULL;
	ret->size = 0L;
}

/**
 * @brief スタック解放
 *
 *  スタックを破棄し、メモリ領域を解放する
 *
 * @param list     (I/O) 解放するスタックへのポインタ
 * @param method   (I)   スタック内で保持しているデータを解放する関数へのポインタ。<BR>
 *                       （単にmalloc関数を使用して確保したメモリなら、freeを指定すれば良い）<BR>
 *                       指定する関数のI/Fは、以下の条件を満たしていること。<BR>
 *                       <UL>
 *                         <LI>戻り値はなし(void)</LI>
 *                         <LI>引数は、ポインタ変数が１つのみ
 *                             （ポインタ変数であれば、型は問わない）</LI>
 *                       </UL>
 *                       NULLが指定された場合は、データの解放処理は行わず、スタックの解放のみを行う。<BR>
 *                       （グローバルメモリや関数ポインタをスタックに格納する場合などにNULLを指定する）
 * @author H.Kumagai
 */
void CmnData_FreeStack(CmnData_Stack *stack, void *method)
{
	CmnData_StackItem *current;
	void (*freeMethod)() = method;
	if (stack == NULL) return;

	current = stack->first;
	while (current != NULL) {
		CmnData_StackItem *tmp = current;
		current = current->next;
		if (method != NULL) {
			freeMethod(tmp->data);
		}
		free(tmp);
	}

	free(stack);
}

/**
 * @brief スタックに要素追加
 *
 *  スタックに要素を追加する。
 *  引数が不正な場合は何もしない。
 *
 * @param list    (I/O) 要素を追加するスタックへのポインタ
 * @param data    (I)   追加する要素（データ）
 * @author H.Kumagai
 */
void CmnData_PushStack(CmnData_Stack *stack, void *data)
{
	CmnData_StackItem *item;
	if (stack == NULL) return;

	item = malloc(sizeof(CmnData_StackItem));
	item->prev = stack->last;
	item->next = NULL;
	item->data = data;

	if (stack->size == 0) {
		stack->first = item;
		stack->last = item;
	}
	else {
		stack->last->next = item;
		stack->last = item;
	}
	stack->size++;
}

/**
 * @brief スタックから要素取得
 *
 *  スタックから要素（データ）を取り出す。
 *
 * @param stack    (I)   要素を取得するスタックへのポインタ
 * @return スタックから取り出したデータ（stackが空の場合はNULLを返す）
 * @author H.Kumagai
 */
void* CmnData_PopStack(CmnData_Stack *stack)
{
	CmnData_StackItem *item = NULL;
	if (stack == NULL || stack->size == 0) {
		return NULL;
	}
	item = stack->last;
	stack->last = stack->last->prev;
	stack->last->next = NULL;		//XXX 最後の要素をPopしたときにNullpo発生。
	stack->size--;

	char *ret = item->data;
	free(item);
	return ret;
}


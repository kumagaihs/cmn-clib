/** @file *********************************************************************
 * @brief 文字列リスト操作 共通関数
 *
 *  単方向リスト（CmnDataList.c）のラッパで、<BR>
 *  文字列をリストで保持するのに適している。
 *
 * @author H.Kumagai
 * @date   2004-06-16
 * @sa 単方向リスト CmnDataList.c
 * $Revision: 1.1 $
 *****************************************************************************/
#include<stdlib.h>
#include<string.h>

#include"cmnclib/Common.h"
#include"cmnclib/CmnString.h"
#include"cmnclib/CmnData.h"
#include"cmnclib/CmnLog.h"

/**
 * @brief 文字列リスト作成
 *
 *  文字列リストを新規に作成する
 *
 * @return 作成した文字列リストへのポインタ。作成に失敗した場合はNULLを返す。
 * @author H.Kumagai
 */
CmnStringList *CmnStringList_Create()
{
	CmnStringList *ret;
	CMNLOG_TRACE_START();

	ret = (CmnStringList *)CmnDataList_Create();

	CMNLOG_TRACE_END();
	return ret;
}


/**
 * @brief 文字列リスト解放
 *
 *  文字列リストを破棄し、メモリ領域を解放する
 *
 * @param list     (I/O) 解放する文字列リストへのポインタ
 * @author H.Kumagai
 */
void CmnStringList_Free(CmnStringList *list)
{
	CMNLOG_TRACE_START();
	CmnDataList_Free((CmnDataList *)list, free);
	CMNLOG_TRACE_END();
}


/**
 * @brief 文字列リスト要素追加
 *
 *  文字列リストの末尾に要素を追加する。<BR>
 *  引数が不正な場合は何もしない。<BR>
 *  また、メモリ確保に失敗した場合は、データがNULLの要素を追加する。
 *
 * @param list    (I/O) 要素を追加するリストへのポインタ
 * @param str     (I)   追加する要素（文字列）
 * @author H.Kumagai
 */
void CmnStringList_Add(CmnStringList *list, const char *str)
{
	char *data;
	CMNLOG_TRACE_START();

	data = malloc(strlen(str) + 1);
	if (data) {
		strcpy(data, str);
	}
	CmnDataList_Add((CmnDataList *)list, data);

	CMNLOG_TRACE_END();
}


/**
 * @brief 文字列リスト要素取得
 *
 *  文字列リストから要素（文字列）を取得する
 *
 * @param list    (I)   要素を取得するリストへのポインタ
 * @param index   (I)   取得する要素のインデックス（最初の要素を0とする）
 * @return 取得した文字列へのポインタ（listやindexが不正な場合はNULLを返す）
 * @author H.Kumagai
 */
char *CmnStringList_Get(CmnStringList *list, int index)
{
	char *ret;
	CMNLOG_TRACE_START();

	ret = (char *)CmnDataList_Get((CmnDataList *)list, index);

	CMNLOG_TRACE_END();
	return ret;
}


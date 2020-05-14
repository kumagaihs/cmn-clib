/** @file *********************************************************************
 * @brief 自動領域拡張文字列バッファ 共通関数
 *
 *  自動領域拡張を行う文字列バッファの共通関数。
 *
 * @author H.Kumagai
 * @date   2020-05-09
 *****************************************************************************/
#include<stdlib.h>
#include<string.h>

#include "CommonString.h"

/**
 * @brief 自動領域拡張バッファ作成
 *
 *  自動領域拡張バッファを新規に作成する。
 *
 * @param str バッファに格納する文字列。NULLを指定した場合は空文字列を設定する。
 * @return 作成したバッファへのポインタ。作成に失敗した場合はNULLを返す。
 */
CmnStringBuffer* CmnStringBuffer_Create(const char *str)
{
	size_t strLen = 0;
	CmnStringBuffer *ret = malloc(sizeof(CmnStringBuffer));
	if (ret == NULL) {
		return NULL;
	}

	if (str == NULL) {
		str = "";
	}
	strLen = strlen(str);

	ret->_buf = CmnDataBuffer_Create(strLen + 1);
	if (ret->_buf == NULL) {
		return NULL;
	}
	CmnDataBuffer_Set(ret->_buf, str, strLen + 1);
	ret->string = ret->_buf->data;
	ret->length = strLen;

	return ret;
}

/**
 * @brief 自動領域拡張バッファへのデータ追加
 *
 *  自動領域拡張バッファの末尾にデータを追加する。
 *
 * @param buf 自動拡張バッファ
 * @param str 追加する文字列
 * @return 正常:0, エラー:-1
 */
int CmnStringBuffer_Append(CmnStringBuffer *buf, const char *str)
{
	size_t strLen = strlen(str);

	/* '\0'を削除 */
	CmnDataBuffer_Delete(buf->_buf, 1);
	/* 文字列を追加 */
	if (CmnDataBuffer_Append(buf->_buf, str, strLen + 1) != 0) {
		return -1;
	}

	buf->string = buf->_buf->data;
	buf->length += strLen;

	return 0;
}

/**
 * @brief 自動領域拡張バッファへのデータ設定
 *
 *  自動領域拡張バッファにデータを設定する。もとのデータは上書かれる。
 *
 * @param buf 自動拡張バッファ
 * @param str 設定するデータ
 * @return 正常:0, エラー:-1
 */
int CmnStringBuffer_Set(CmnStringBuffer *buf, const char *str)
{
	size_t strLen = strlen(str);

	/* 文字列を設定 */
	if (CmnDataBuffer_Set(buf->_buf, str, strLen + 1) != 0) {
		return -1;
	}

	buf->string = buf->_buf->data;
	buf->length = strLen;

	return 0;
}

/**
 * @brief 自動領域拡張バッファの解放
 *
 *  自動領域拡張バッファが不要になった場合、メモリ解放のために必ず本関数を呼び出すこと。
 *
 * @param buf 自動拡張バッファ
 */
void CmnStringBuffer_Free(CmnStringBuffer *buf)
{
	free(buf->_buf);
	free(buf);
}


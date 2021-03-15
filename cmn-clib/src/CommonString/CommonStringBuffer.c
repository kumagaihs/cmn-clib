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

#include "cmnclib/CommonString.h"
#include "cmnclib/CommonLog.h"

/**
 * @brief 文字列バッファ作成
 *
 *  自動領域拡張をする文字列バッファを新規に作成する。
 *
 * @param str 文字列バッファに格納する文字列。NULLを指定した場合は空文字列を設定する。
 * @return 作成したバッファへのポインタ。作成に失敗した場合はNULLを返す。
 */
CmnStringBuffer* CmnStringBuffer_Create(const char *str)
{
	size_t strLen = 0;
	CmnStringBuffer *ret;
	CMNLOG_TRACE_START();

	ret = malloc(sizeof(CmnStringBuffer));
	if (ret == NULL) {
		CMNLOG_TRACE_END();
		return NULL;
	}

	if (str == NULL) {
		CMNLOG_TRACE_END();
		str = "";
	}
	strLen = strlen(str);

	ret->_buf = CmnDataBuffer_Create(strLen + 1);
	if (ret->_buf == NULL) {
		CMNLOG_TRACE_END();
		return NULL;
	}
	CmnDataBuffer_Set(ret->_buf, str, strLen + 1);
	ret->string = ret->_buf->data;
	ret->length = strLen;

	CMNLOG_TRACE_END();
	return ret;
}

/**
 * @brief 文字列バッファへのデータ追加
 *
 *  文字列バッファの末尾にデータを追加する。
 *
 * @param buf 文字列バッファ
 * @param str 追加する文字列
 * @return 正常:0, エラー:-1
 */
int CmnStringBuffer_Append(CmnStringBuffer *buf, const char *str)
{
	size_t strLen;
	CMNLOG_TRACE_START();

	strLen = strlen(str);

	/* '\0'を削除 */
	CmnDataBuffer_Delete(buf->_buf, 1);
	/* 文字列を追加 */
	if (CmnDataBuffer_Append(buf->_buf, str, strLen + 1) != 0) {
		CMNLOG_TRACE_END();
		return -1;
	}

	buf->string = buf->_buf->data;
	buf->length += strLen;

	CMNLOG_TRACE_END();
	return 0;
}

/**
 * @brief 文字列バッファへのデータ設定
 *
 *  文字列バッファにデータを設定する。もとのデータは上書かれる。
 *
 * @param buf 文字列バッファ
 * @param str 設定する文字列
 * @return 正常:0, エラー:-1
 */
int CmnStringBuffer_Set(CmnStringBuffer *buf, const char *str)
{
	size_t strLen;
	CMNLOG_TRACE_START();
	
	strLen = strlen(str);

	/* 文字列を設定 */
	if (CmnDataBuffer_Set(buf->_buf, str, strLen + 1) != 0) {
		CMNLOG_TRACE_END();
		return -1;
	}

	buf->string = buf->_buf->data;
	buf->length = strLen;

	CMNLOG_TRACE_END();
	return 0;
}

/**
 * @brief 文字列バッファへのデータ設定（by CmnDataBuffer）
 *
 *  CmnDataBufferのデータを文字列として設定する。もとのbufのデータは上書かれる。
 *
 * @param buf 自動拡張バッファ
 * @param dat 設定するデータ
 * @return 正常:0, エラー:-1
 */
int CmnStringBuffer_SetByCmnDataBuffer(CmnStringBuffer *buf, const CmnDataBuffer *dat)
{
	CMNLOG_TRACE_START();

	/* 文字列を設定 */
	if (CmnDataBuffer_Set(buf->_buf, dat->data, dat->size) != 0) {
		CMNLOG_TRACE_END();
		return -1;
	}
	/* 終端文字を設定 */
	CmnDataBuffer_Append(buf->_buf, "", 1);

	buf->string = buf->_buf->data;
	buf->length = dat->size;

	CMNLOG_TRACE_END();
	return 0;
}

/**
 * @brief 文字列バッファの解放
 *
 *  文字列バッファが不要になった場合、メモリ解放のために必ず本関数を呼び出すこと。
 *
 * @param buf 文字列バッファ
 */
void CmnStringBuffer_Free(CmnStringBuffer *buf)
{
	CMNLOG_TRACE_START();
	free(buf->_buf);
	free(buf);
	CMNLOG_TRACE_END();
}


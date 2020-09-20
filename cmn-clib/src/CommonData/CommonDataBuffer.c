/** @file *********************************************************************
 * @brief 自動領域拡張バッファ 共通関数
 *
 *  自動領域拡張を行うバッファの共通関数。
 *
 * @author H.Kumagai
 * @date   2020-05-09
 *****************************************************************************/
#include<stdlib.h>
#include<string.h>

#include "cmnclib/CommonData.h"
#include "cmnclib/CommonLog.h"

static const size_t DEFAULT_BUFFER_SIZE = 4096;

/**
 * @brief 自動領域拡張バッファ作成
 *
 *  自動領域拡張バッファを新規に作成する。
 *
 * @param bufSize 初期バッファサイズ。0を指定した場合はデフォルトのバッファサイズが適用される。
 * @return 作成したバッファへのポインタ。作成に失敗した場合はNULLを返す。
 */
CmnDataBuffer* CmnDataBuffer_Create(size_t bufSize)
{
	CmnDataBuffer *ret;
	CMNLOG_TRACE_START();

	ret = malloc(sizeof(CmnDataBuffer));
	if (ret == NULL) {
		CMNLOG_TRACE_END();
		return NULL;
	}

	if (bufSize == 0) {
		bufSize = DEFAULT_BUFFER_SIZE;
	}
	if ((ret->data = malloc(bufSize)) == NULL) {
		CMNLOG_TRACE_END();
		return NULL;
	}
	ret->bufSize = bufSize;
	ret->size = 0;

	CMNLOG_TRACE_END();
	return ret;
}

/**
 * @brief 自動領域拡張バッファへのデータ追加
 *
 *  自動領域拡張バッファの末尾にデータを追加する。
 *
 * @param buf 自動拡張バッファ
 * @param data 追加するデータ
 * @param len バッファに書き込むデータの長さ
 * @return 正常:0, エラー:-1
 */
int CmnDataBuffer_Append(CmnDataBuffer *buf, const void *data, size_t len)
{
	void *buftmp;
	size_t oldBufSize = buf->bufSize;
	size_t newBufSize = buf->size + len;
	CMNLOG_TRACE_START();

	/* 領域が不足する場合は拡張 */
	if (oldBufSize < newBufSize) {

		/* すでにある程度大きい領域の場合は少し多めに拡張する */
		if (DEFAULT_BUFFER_SIZE < oldBufSize) {
			newBufSize += (newBufSize / 10);
		}

		buftmp = realloc(buf->data, newBufSize);
		if (buftmp == NULL) {
			CMNLOG_TRACE_END();
			return -1;
		}
		buf->data = buftmp;
		buf->bufSize = newBufSize;
	}

	/* データ追加 */
	memcpy(((char*)buf->data) + buf->size, data, len);
	buf->size += len;

	CMNLOG_TRACE_END();
	return 0;
}

/**
 * @brief 自動領域拡張バッファへのデータ設定
 *
 *  自動領域拡張バッファにデータを設定する。もとのデータは上書かれる。
 *
 * @param buf 自動拡張バッファ
 * @param data 設定するデータ
 * @param len バッファに書き込むデータの長さ
 * @return 正常:0, エラー:-1
 */
int CmnDataBuffer_Set(CmnDataBuffer *buf, const void *data, size_t len)
{
	void *buftmp;
	size_t oldBufSize = buf->bufSize;
	size_t newBufSize = len;
	int resize = False;
	CMNLOG_TRACE_START();

	/* 領域が不足する場合は拡張 */
	if (oldBufSize < newBufSize) {

		/* すでにある程度大きい領域を拡張する場合は少し多めに拡張する */
		if (DEFAULT_BUFFER_SIZE < oldBufSize) {
			newBufSize += (newBufSize / 10);
		}
		resize = True;
	}
	/* 領域が余る場合、3分の1以上の空きができるならバッファを3分の1縮小する */
	else if (newBufSize < oldBufSize && (oldBufSize / 3) < (oldBufSize - newBufSize)) {
		newBufSize = oldBufSize - (oldBufSize / 3);
		resize = True;
	}

	if (resize) {
		buftmp = realloc(buf->data, newBufSize);
		if (buftmp == NULL) {
			CMNLOG_TRACE_END();
			return -1;
		}
		buf->data = buftmp;
		buf->bufSize = newBufSize;
	}

	/* データ追加 */
	memcpy(buf->data, data, len);
	buf->size = len;

	CMNLOG_TRACE_END();
	return 0;
}

/**
 * @brief 自動領域拡張バッファのデータ削除
 *
 *  自動領域拡張バッファのデータを一部もしくは全部削除する。
 *
 * @param buf 自動拡張バッファ
 * @param len 削除する長さ
 */
void CmnDataBuffer_Delete(CmnDataBuffer *buf, size_t len)
{
	CMNLOG_TRACE_START();
	if (buf->size < len) {
		len = buf->size;
	}
	buf->size -=len;
	CMNLOG_TRACE_END();
}

/**
 * @brief 自動領域拡張バッファの解放
 *
 *  自動領域拡張バッファが不要になった場合、メモリ解放のために必ず本関数を呼び出すこと。
 *
 * @param buf 自動拡張バッファ
 */
void CmnDataBuffer_Free(CmnDataBuffer *buf)
{
	CMNLOG_TRACE_START();
	free(buf->data);
	free(buf);
	CMNLOG_TRACE_END();
}


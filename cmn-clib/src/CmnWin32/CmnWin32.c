/** @file *********************************************************************
 * @brief Win32関連機能
 * @author H.Kumagai
 *****************************************************************************/

#include "cmnclib/Common.h"
#if IS_PRATFORM_WINDOWS()

#include "cmnclib/CmnWin32.h"
#include "cmnclib/CmnString.h"
#include "cmnclib/CmnLog.h"

/**
 * @brief クリップボードのデータを文字列として取得する。
 * @param buf クリップボードから取得した文字列を格納するバッファ
 * @return 0:正常、-1:クリップボードのオープンに失敗、-2:クリップボードに文字列変換可能なデータなし
 */
int CmnWin32Clipboard_GetString(CmnStringBuffer *buf)
{
	HGLOBAL hglobal;
	LPSTR lpstr;
	CMNLOG_TRACE_START();

	/* クリップボードデータ形式チェック */
	if ( ! IsClipboardFormatAvailable(CF_TEXT)) {
		CMNLOG_TRACE_END();
		return -2;
	}
	/* クリップボードオープン */
	if ( ! OpenClipboard(NULL)) {
		CMNLOG_TRACE_END();
		return -1;
	}

	/* クリップボードデータ取得 */
	hglobal = GetClipboardData(CF_TEXT);
	if (hglobal == NULL) {
		CloseClipboard();
		CMNLOG_TRACE_END();
		return -1;
	}

	/* グローバルデータ領域確保(Lock)、先頭アドレスコピー */
	lpstr = (LPSTR)GlobalLock(hglobal);
	if (lpstr == NULL) {
		CloseClipboard();
		CMNLOG_TRACE_END();
		return -1;
	}

	/* 文字列コピー */
	CmnStringBuffer_Set(buf, lpstr);

	/* グローバルメモリ、クリップボード解放 */
	GlobalUnlock(hglobal);
	CloseClipboard();

	CMNLOG_TRACE_END();
	return 0;
}

/**
 * @brief クリップボードに文字列を設定する。
 * @param buf クリップボードに設定する文字列
 * @return 0:正常、-1:クリップボードの設定に失敗
 */
int CmnWin32Clipboard_SetString(CmnStringBuffer *buf)
{
	HGLOBAL hglobal;
	LPSTR lpstr;
	CMNLOG_TRACE_START();

	/* クリップボードオープン */
	if ( ! OpenClipboard(NULL)) {
		CMNLOG_TRACE_END();
		return -1;
	}

	/* クリップボード初期化 */
	if ( ! EmptyClipboard()) {
		CMNLOG_TRACE_END();
		return -1;
	}

	/* グローバルメモリ確保＆文字列コピー */
	hglobal = GlobalAlloc(GMEM_FIXED, buf->length + 1);
	strcpy_s((char*)hglobal, buf->length + 1, buf->string);

	/* クリップボードに設定 */
	if (SetClipboardData(CF_TEXT, hglobal) == NULL)
	{
		CloseClipboard();
		GlobalFree(hglobal);
		CMNLOG_TRACE_END();
		return -1;
	}

	/* グローバルデータ領域確保(Lock)、先頭アドレスコピー */
	lpstr = (LPSTR)GlobalLock(hglobal);
	if (lpstr == NULL) {
		CloseClipboard();
		CMNLOG_TRACE_END();
		return -1;
	}

	CloseClipboard();

	CMNLOG_TRACE_END();
	return 0;
}

#endif

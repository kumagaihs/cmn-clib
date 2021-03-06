/** @file *********************************************************************
 * @brief 文字列操作 共通関数
 *
 *  文字列操作系共通関数のうち、特に汎用的な関数郡が実装されている。
 *
 * @author H.Kumagai
 * @date   2004-06-05
 * $Revision: 1.3 $
 *****************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"cmnclib/Common.h"
#include"cmnclib/CommonString.h"
#include"cmnclib/CommonLog.h"

/**
 * @brief 右側トリム
 *
 *  文字列の右側のスペースを削除する
 *
 * @param str       (I/O) トリム処理対象の文字列
 * @return strへのポインタ
 * @author H.Kumagai
 */
char *CmnString_RTrim(char *str)
{
	size_t len;
	CMNLOG_TRACE_START();

	if ( ! str) {
		CMNLOG_TRACE_END();
		return str;
	}

	len = strlen(str) - 1;
	if (len <= 0) {
		CMNLOG_TRACE_END();
		return str;
	}

	while (len) {
		if (*(str + len) != ' ') {
			break;
		}
		len--;
	}
	len++;
	*(str + len) = '\0';

	CMNLOG_TRACE_END();
	return str;
}


/**
 * @brief 左側トリム
 *
 *  文字列の左側のスペースを削除する
 *
 * @param str       (I)   トリム処理対象の文字列
 * @return strへのポインタ
 * @author H.Kumagai
 * @note この関数は、実際にスペース部分を削除するのではなく、
 *       スペース部分を飛ばしたポインタを生成する。
 */
char *CmnString_LTrim(char *str)
{
	CMNLOG_TRACE_START();
	for (; *str && (*str == ' '); str++) ;

	CMNLOG_TRACE_END();
	return str;
}


/**
 * @brief トリム
 *
 *  文字列の両側のスペースを削除する
 *
 * @param str       (I/O) トリム処理対象の文字列
 * @return strへのポインタ
 * @author H.Kumagai
 */
char *CmnString_Trim(char *str)
{
	char *ret;
	CMNLOG_TRACE_START();

	ret = CmnString_RTrim(CmnString_LTrim(str));

	CMNLOG_TRACE_END();
	return ret;
}

/**
 * @brief 文字列置換
 *
 *  srcを読み込み、oldをnewに置換した文字列をdestに格納する。
 *
 * @param src  (I) 元文字列
 * @param befor  (I) 置換対象文字列
 * @param after  (I) 置換後文字列
 * @param dest (O) 置換処理後の文字列を格納するバッファ
 * @return destを返却する。
 */
char* CmnString_Replace(const char *src, const char *befor, const char *after, char *dest)
{
	size_t oldlen;
	CMNLOG_TRACE_START();

	oldlen = strlen(befor);

	*dest = '\0';

	while (src != NULL) {
		char *oldpos = strstr(src, befor);
		if (oldpos != NULL) {
			strncat(dest, src, oldpos - src);
			strcat(dest, after);
			src = oldpos + oldlen;
		}
		else {
			strcat(dest, src);
			src = NULL;
		}
	}

	CMNLOG_TRACE_END();
	return dest;
}

/**
 * @brief 文字列置換（動的メモリ確保）
 *
 *  srcを読み込み、oldをnewに置換した文字列を生成する。
 *
 * @param src  (I) 元文字列
 * @param befor  (I) 置換対象文字列
 * @param after  (I) 置換後文字列
 * @return 置換後文字列へのポインタを返却する。呼び出し元でfreeすること。
 */
char* CmnString_ReplaceNew(const char *src, const char *befor, const char *after)
{
	size_t srclen, oldlen, newlen;
	size_t bufsize;
	char *dest;
	char *ret;
	CMNLOG_TRACE_START();

	/* 元文字列のサイズから必要十分なバッファサイズを算出 */
	srclen = strlen(src);
	oldlen = strlen(befor);
	newlen = strlen(after);
	bufsize = srclen + (srclen * (newlen / oldlen)) + 1;

	/* バッファ確保 */
	dest = malloc(bufsize);
	memset(dest, '\0', bufsize);

	ret = CmnString_Replace(src, befor, after, dest);

	CMNLOG_TRACE_END();
	return ret;
}

/**
 * @brief 文字列連結（動的メモリ確保）
 *
 *  leftとrightを連結した文字列を生成する。
 *
 * @param left  (I) 左側文字列
 * @param right (I) 右側文字列
 * @return 連結後文字列へのポインタを返却する。呼び出し元でfreeすること。
 */
char* CmnString_StrCatNew(const char *left, const char *right)
{
	char *buf;
	size_t bufsize;
	char *ret;
	CMNLOG_TRACE_START();

	bufsize = strlen(left) + strlen(right) + 1;
	buf = malloc(bufsize);
	memset(buf, '\0', bufsize);

	ret = strcat(strcat(buf, left), right);

	CMNLOG_TRACE_END();
	return ret;
}

/**
 * @brief 文字列コピー（動的メモリ確保）
 *
 *  strをコピーした文字列を生成する。
 *
 * @param str  (I) 文字列
 * @return コピーした文字列へのポインタを返却する。呼び出し元でfreeすること。
 */
char* CmnString_StrCopyNew(const char* str)
{
	char *buf;
	CMNLOG_TRACE_START();

	buf = calloc(1, strlen(str) + 1);
	if (buf != NULL) {
		strcpy(buf, str);
	}

	CMNLOG_TRACE_END();
	return buf;
}

/**
 * @brief 文字列分割（into配列）
 *
 *  strをdelimで分割してbufに格納する。
 *  bufには十分なサイズを確保しておくこと。
 *
 * @param buf 分割後の文字列を格納する2次元配列。例：最大100文字の文字列を最大10個格納可能とする場合 -> cahr[10][100]
 * @param rowlen bufの配列要素数。例の場合は10
 * @param collen buf1要素あたりのの最大文字数。例の場合は100
 * @param str 分割対象の文字列
 * @param delim 区切り文字(列)
 * @return 分割した文字列数
 */
int CmnString_Split(char *buf, size_t rowlen, size_t collen, const char *str, const char *delim)
{
	const char *pos = str;
	size_t delimlen;
	size_t row = 0;
	CMNLOG_TRACE_START();

	delimlen = strlen(delim);

	while (*str != '\0') {
		char *tmp = buf + (row * collen);

		pos = strstr(str, delim);
		if (pos == NULL) {
			strcpy(tmp, str);
			row++;
			break;
		}

		strncpy(tmp, str, pos - str);
		tmp[pos - str] = '\0';

		str = pos + delimlen;
		row++;

		/* 最後がdelimで終わっている場合は末尾に空文字列の要素を補充 */
		if (*str == '\0') {
			tmp = buf + (row * collen);
			tmp[0] = '\0';
			row++;
		}
	}

	CMNLOG_TRACE_END();
	return row;
}

/**
 * @brief 文字列の左側パディング
 * @param buf パディング後の文字列を格納するバッファ
 * @param str 元文字列
 * @param padch パディングする文字
 * @param digit パディング後何文字にするか
 * @return bufを返す
 */
char* CmnString_Lpad(char *buf, const char *str, char padch, size_t digit)
{
	size_t stlen = 0;
	size_t padlen = 0;
	CMNLOG_TRACE_START();

	stlen = strlen(str);

	/* padding */
	if (stlen < digit) {
		padlen = digit - stlen;
		memset(buf, padch, padlen);
	}

	/* add str */
	strcpy(buf + padlen, str);

	CMNLOG_TRACE_END();
	return buf;
}

/**
 * @brief 文字列の右側パディング
 * @param buf パディング後の文字列を格納するバッファ
 * @param str 元文字列
 * @param padch パディングする文字
 * @param digit パディング後何文字にするか
 * @return bufを返す
 */
char* CmnString_Rpad(char *buf, const char *str, char padch, size_t digit)
{
	size_t stlen = 0;
	size_t padlen = 0;
	CMNLOG_TRACE_START();

	strcpy(buf, str);
	stlen = strlen(str);

	if (stlen < digit) {
		/* padding */
		padlen = digit - stlen;
		memset(buf + stlen, padch, padlen);
	}

	CMNLOG_TRACE_END();
	return buf;
}

/**
 * @brief 対象文字列が特定文字列で始まっているかチェックする
 * @param str 対象文字列
 * @param mark チェックする文字列
 * @return strがmarkで始まっている場合は1を、そうでない場合は0を返す。
 */
int CmnString_StartWith(const char *str, const char *mark)
{
	int ret = 0;
	CMNLOG_TRACE_START();

	if (strstr(str, mark) == EQUAL) {
		return 1;
	}
	CMNLOG_TRACE_END();
	return ret;
}

/**
 * @brief 対象文字列が特定文字列で終わっているかチェックする
 * @param str 対象文字列
 * @param mark チェックする文字列
 * @return strがmarkで終わっている場合は1を、そうでない場合は0を返す。
 */
int CmnString_EndWith(const char *str, const char *mark)
{
	size_t strLen;
	size_t markLen;
	int ret = 0;
	CMNLOG_TRACE_START();

	strLen = strlen(str);
	markLen = strlen(mark);

	if (strcmp(str + strLen - markLen, mark) == EQUAL) {
		ret = 1;
	}

	CMNLOG_TRACE_END();
	return ret;
}


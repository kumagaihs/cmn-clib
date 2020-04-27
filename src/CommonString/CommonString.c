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

#include"Common.h"
#include"CommonString.h"


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
	int len;

	if ( ! str) {
		return str;
	}

	len = strlen(str) - 1;
	if (len <= 0) {
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
	for (; *str && (*str == ' '); str++) ;
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
	return CmnString_RTrim(CmnString_LTrim(str));
}

/**
 * @brief 文字列置換
 *
 *  srcを読み込み、oldをnewに置換した文字列をdestに格納する。
 *
 * @param src  (I) 元文字列
 * @param old  (I) 置換対象文字列
 * @param new  (I) 置換後文字列
 * @param dest (O) 置換処理後の文字列を格納するバッファ
 * @return destを返却する。
 */
char* CmnString_Replace(const char *src, const char *old, const char *new, char *dest)
{
	size_t oldlen;
	oldlen = strlen(old);

	*dest = '\0';

	while (src != NULL) {
		char *oldpos = strstr(src, old);
		if (oldpos != NULL) {
			strncat(dest, src, oldpos - src);
			strcat(dest, new);
			src = oldpos + oldlen;
		}
		else {
			strcat(dest, src);
			src = NULL;
		}
	}

	return dest;
}

/**
 * @brief 文字列置換（動的メモリ確保）
 *
 *  srcを読み込み、oldをnewに置換した文字列を生成する。
 *
 * @param src  (I) 元文字列
 * @param old  (I) 置換対象文字列
 * @param new  (I) 置換後文字列
 * @return 置換後文字列へのポインタを返却する。呼び出し元でfreeすること。
 */
char* CmnString_ReplaceNew(const char *src, const char *old, const char *new)
{
	size_t srclen, oldlen, newlen;
	size_t bufsize;
	char *dest;

	/* 元文字列のサイズから必要十分なバッファサイズを算出 */
	srclen = strlen(src);
	oldlen = strlen(old);
	newlen = strlen(new);
	bufsize = srclen + (srclen * (newlen / oldlen)) + 1;

	/* バッファ確保 */
	dest = malloc(bufsize);
	memset(dest, '\0', bufsize);

	return CmnString_Replace(src, old, new, dest);
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
char* CmnString_StrcatNew(const char *left, const char *right)
{
	char *buf;
	size_t bufsize = strlen(left) + strlen(right) + 1;
	buf = malloc(bufsize);
	memset(buf, '\0', bufsize);

	return strcat(strcat(buf, left), right);
}


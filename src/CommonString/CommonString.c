/** @file *********************************************************************
 * @brief 文字列操作 共通関数
 *
 *  文字列操作系共通関数のうち、特に汎用的な関数郡が実装されている。
 * 
 * @author H.Kumagai
 * @date   2004-06-05
 * $Revision: 1.3 $
 *****************************************************************************/
#include"stdafx.h"

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


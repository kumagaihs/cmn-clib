/** @file *********************************************************************
 * @brief 設定値操作 共通関数
 *
 *  設定値操作系共通関数のうち、特に汎用的な関数郡が実装されている。
 *
 * @author H.Kumagai
 * @date   2004-06-10
 * $Revision: 1.1 $
 *****************************************************************************/
#include<stdlib.h>
#include<string.h>

#include"Common.h"
#include"CommonConf.h"

/**
 * @brief 環境変数取得
 *
 *  環境変数envNameに設定されている値を取得する。
 *
 * @param buf       (O)   取得した値を設定する。値が取得できなかった場合は何もしない。<BR>
 *                        関数コール元で十分なバッファ領域を確保すること。
 * @param envName   (I)   値を取得する環境変数名
 * @retval True  環境変数が取得できた場合
 * @retval False 環境変数が取得できなかった場合
 * @author H.Kumagai
 */
int CmnConf_GetEnv(char *buf, const char *envName)
{
	char *env;		/* 取得した環境変数の値 */

	env = getenv(envName);
	if(env == NULL) {
		return False;
	}

	strcpy(buf, env);
	return True;
}


/** @file *********************************************************************
 * @brief ファイル操作共通関数群
 *
 *  ファイル操作共通関数群<br>
 *
 * @author H.Kumagai
 *****************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "Common.h"
#include "CommonFile.h"
#include "CommonData.h"

static const size_t BUF_SIZE = 4096;

/**
 * @brief ファイルをテキストデータとして全て読み込む
 *
 *  ファイルサイズ+1のメモリをmallocし、ファイルから読み込んだデータをコピーして返却する。また末尾には'\0'を格納する。<br>
 *  返却したポインタは呼び出し側でfreeすること。<br>
 *  なお、ファイルはバイナリモードで読み込む。改行コードの変換が必要であれば呼び出し側で行うこと。
 *
 * @param filePath ファイルパス
 * @return 読み込んだデータ（呼び出し元でfreeすること）。読み込みに失敗した場合はNULLを返却する。
 */
char* CmnFile_ReadAllText(char *filePath)
{
	char *ret;
	CmnDataBuffer *buf = CmnFile_ReadAll(filePath);
	if (buf == NULL) {
		return NULL;
	}

	if ((ret = malloc(buf->size + 1)) == NULL) {
		return NULL;
	}
	strncpy(ret, buf->data, buf->size);
	ret[buf->size] = '\0';
	CmnDataBuffer_Free(buf);

	return ret;
}

/**
 * @brief ファイルを全て読み込む
 *
 *  ファイルから読み込んだデータをコピーして返却する。<br>
 *  返却したポインタは呼び出し側でCmnDataBuffer_Freeで解放すること。<br>
 *
 * @param filePath ファイルパス
 * @return 読み込んだデータ。読み込みに失敗した場合はNULLを返却する。
 */
CmnDataBuffer* CmnFile_ReadAll(char *filePath)
{
	FILE *fp;
	CmnDataBuffer *buf;
	char tmp[BUF_SIZE];
	int readLen;

	if ((fp = fopen(filePath, "rb")) == NULL) {
		return NULL;
	}

	if ((buf = CmnDataBuffer_Create(0)) == NULL) {
		fclose(fp);
		return NULL;
	}

	while ((readLen = fread(tmp, sizeof(tmp[0]), BUF_SIZE, fp)) > 0) {
		if (CmnDataBuffer_Append(buf, tmp, readLen) != 0) {
			CmnDataBuffer_Free(buf);
			return NULL;
		}

	}

	fclose(fp);
	return buf;
}

/**
 * @brief データをファイルに書き込む。ファイルがなければ新規作成。ファイルがあれば上書き。
 * @param filePath ファイルパス
 * @param data 書き込むデータ
 * @return 0:正常終了、-1:書き込み失敗
 */
int CmnFile_WriteNew(char *filePath, void *data, size_t len)
{
	//TODO
	return 0;
}

/**
 * @brief データをファイルの先頭に追加する。ファイルがなければ新規作成。
 * @param filePath ファイルパス
 * @param data 書き込むデータ
 * @return 0:正常終了、-1:書き込み失敗
 */
int CmnFile_WriteHead(char *filePath, void *data, size_t len)
{
	//TODO
	return 0;
}

/**
 * @brief データをファイルの末尾に追加する。ファイルがなければ新規作成。
 * @param filePath ファイルパス
 * @param data 書き込むデータ
 * @return 0:正常終了、-1:書き込み失敗
 */
int CmnFile_WriteTail(char *filePath, void *data, size_t len)
{
	//TODO
	return 0;
}


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

#include "cmnclib/Common.h"
#include "cmnclib/CommonFile.h"
#include "cmnclib/CommonData.h"

#if IS_PRATFORM_WINDOWS()
#include<windows.h>
#endif

static const size_t BUF_SIZE = 4096;

static CmnDataList* ListForWindows(const char *path, CmnDataList *list);
static CmnDataList* ListForLinux(const char *path, CmnDataList *list);

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
char* CmnFile_ReadAllText(const char *filePath)
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
CmnDataBuffer* CmnFile_ReadAll(const char *filePath)
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
int CmnFile_WriteNew(const char *filePath, void *data, size_t len)
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
int CmnFile_WriteHead(const char *filePath, void *data, size_t len)
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
int CmnFile_WriteTail(const char *filePath, void *data, size_t len)
{
	//TODO
	return 0;
}

/**
 * @brief path直下のファイル/ディレクトリ一覧を取得する。
 * @param path ファイル/ディレクトリ一覧を取得するパス
 * @param list ファイル/ディレクトリ一覧を格納するリストオブジェクト。リストの要素にはCmnFileInfoをセットする。
 * @return listを返す。pathが無効な場合など一覧の取得に失敗した場合はNULLを返す。
 */
CmnDataList* CmnFile_List(const char *path, CmnDataList *list)
{
	if (IS_PRATFORM_WINDOWS()) {
		return ListForWindows(path, list);
	}
	else {
		return ListForLinux(path, list);
	}
}

char* CmnFileInfo_ToString(const CmnFileInfo *info, char *buf)
{
	char timeBuf[160];

	*buf = '\0';
	sprintf(buf, "dir=%s, name=%s, size=%I64d, lastUpdateTime=[%s], isDirectory=%d, isFile=%d, isHiddenFile=%d, isSystemFile=%d, isSymbolicLink=%d",
			info->dir,
			info->name,
			info->size,
			CmnTimeDateTime_ToString(&info->lastUpdateTime, timeBuf),	/*TODO:時刻を見やすい形式に変更する。*/
			info->isDirectory,
			info->isFile,
			info->isHiddenFile,
			info->isSystemFile,
			info->isSymbolicLink);
	return buf;
}


static CmnDataList* ListForWindows(const char *path, CmnDataList *list)
{
	char *searchPath;
	HANDLE hFind;
	WIN32_FIND_DATA win32fd;
	SYSTEMTIME win32time;

	CmnFileInfo *info;
	CmnTimeDateTime cmnTime;

	/* TODO:最後の文字がパス区切りなら除去 */

	/* TODO:pathの存在確認して、存在しなければNULLリターン */

	searchPath = CmnString_StrCatNew(path, CMN_FILE_PATH_DELIMITER "*.*");
	hFind = FindFirstFile(searchPath, &win32fd);
	if (hFind == INVALID_HANDLE_VALUE) {
		return list;
	}

	do {
		/* 「.」と「..」はスキップ */
		if (strcmp(win32fd.cFileName, ".") == 0
				|| strcmp(win32fd.cFileName, "..") == 0) {
			continue;
		}

		info = calloc(1, sizeof(CmnFileInfo));

		/* パス、ファイル名 */
		strcpy(info->dir, path);
		strcpy(info->name, win32fd.cFileName);

		/* ファイルサイズ */
		info->size = (win32fd.nFileSizeHigh * (MAXDWORD + 1)) + win32fd.nFileSizeLow;

		/* 最終更新日時  XXX:タイムゾーンがゼロ（グリニッジ標準時）の時刻が取れるため、日本なら＋9時間してやる必要がある。 */
		FileTimeToSystemTime(&(win32fd.ftLastWriteTime), &win32time);
		CmnTimeDateTime_Set(&cmnTime, win32time.wYear, win32time.wMonth, win32time.wDay, win32time.wHour, win32time.wMinute, win32time.wSecond, -1);
		info->lastUpdateTime = cmnTime;

		/* ファイル属性 */
		if (win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			info->isDirectory = True;
		} else {
			info->isFile = True;
		}
		if (win32fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) {
			info->isHiddenFile = True;
		}
		if (win32fd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) {
			info->isSystemFile = True;
		}

		CmnDataList_Add(list, info);
	} while (FindNextFile(hFind, &win32fd));

	FindClose(hFind);

	return list;
}

static CmnDataList* ListForLinux(const char *path, CmnDataList *list)
{
	/* TODO */
	return NULL;
}


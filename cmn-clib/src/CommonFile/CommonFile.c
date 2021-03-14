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
#include"cmnclib/CommonLog.h"

#if IS_PRATFORM_WINDOWS()
#include<windows.h>
#else
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#define BUF_SIZE 4096
#define MAX_PATH_SIZE 2048

#if IS_PRATFORM_WINDOWS()
static CmnDataList* ListForWindows(const char *path, CmnDataList *list, CHARSET pathCharset);
#else
static CmnDataList* ListForLinux(const char *path, CmnDataList *list);
#endif

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
	CmnDataBuffer *buf;
	CMNLOG_TRACE_START();

	buf = CmnFile_ReadAll(filePath);
	if (buf == NULL) {
		CMNLOG_TRACE_END();
		return NULL;
	}

	if ((ret = malloc(buf->size + 1)) == NULL) {
		CMNLOG_TRACE_END();
		return NULL;
	}
	strncpy(ret, buf->data, buf->size);
	ret[buf->size] = '\0';
	CmnDataBuffer_Free(buf);

	CMNLOG_TRACE_END();
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
	CMNLOG_TRACE_START();

	if ((fp = fopen(filePath, "rb")) == NULL) {
		CMNLOG_TRACE_END();
		return NULL;
	}

	if ((buf = CmnDataBuffer_Create(0)) == NULL) {
		fclose(fp);
		CMNLOG_TRACE_END();
		return NULL;
	}

	while ((readLen = fread(tmp, sizeof(tmp[0]), BUF_SIZE, fp)) > 0) {
		if (CmnDataBuffer_Append(buf, tmp, readLen) != 0) {
			CmnDataBuffer_Free(buf);
			CMNLOG_TRACE_END();
			return NULL;
		}

	}

	fclose(fp);
	CMNLOG_TRACE_END();
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
	CMNLOG_TRACE_START();

	//TODO
	CMNLOG_TRACE_END();
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
	CMNLOG_TRACE_START();

	//TODO
	CMNLOG_TRACE_END();
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
	CMNLOG_TRACE_START();

	//TODO
	CMNLOG_TRACE_END();
	return 0;
}

/**
 * @brief path直下のファイル/ディレクトリ一覧を取得する。
 * @param path ファイル/ディレクトリ一覧を取得するパス
 * @param list ファイル/ディレクトリ一覧を格納するリストオブジェクト。リストの要素にはCmnFileInfoをセットする。
 * @return listを返す。pathが無効な場合など一覧の取得に失敗した場合はNULLを返す。
 */
CmnDataList* CmnFile_List(const char *path, CmnDataList *list, CHARSET pathCharset)
{
	CMNLOG_TRACE_START();
	CmnDataList *ret;

#if IS_PRATFORM_WINDOWS()
	ret = ListForWindows(path, list, pathCharset);
#else
	ret = ListForLinux(path, list);
#endif

	CMNLOG_TRACE_END();
	return ret;
}

char* CmnFileInfo_ToString(const CmnFileInfo *info, char *buf)
{
	char timeBuf[160];
	CMNLOG_TRACE_START();

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

	CMNLOG_TRACE_END();
	return buf;
}


#if IS_PRATFORM_WINDOWS()
/* ============================================================================
 *    Only Windows code
 * ========================================================================= */

static CmnDataList* ListForWindows(const char *path, CmnDataList *list, CHARSET pathCharset)
{
	char *searchPath;
	wchar_t searchPathWide[MAX_PATH_SIZE * 2] = {'\0'};
	HANDLE hFind;
	WIN32_FIND_DATA win32fd;
	SYSTEMTIME win32time;
	UINT codepage;

	CmnFileInfo *info;
	CmnTimeDateTime cmnTime;
	char newpath[CMN_FILE_MAX_PATH + CMN_FILE_MAX_FILE_NAME] = "";

	CMNLOG_TRACE_START();

	/* 最後の文字がパス区切りなら除去 */
	if (CmnString_EndWith(path, CMN_FILE_PATH_DELIMITER)) {
		strcpy(newpath, path);
		newpath[strlen(path) - 1] = '\0';
		path = newpath;
	}

	/* TODO:pathの存在確認して、存在しなければNULLリターン */

	searchPath = CmnString_StrCatNew(path, CMN_FILE_PATH_DELIMITER "*");

	/* FindFirstFileWを呼び出すためにファイルパスをUNICODEに変換（UTF-8以外は環境文字コード（SJIS）と判定） */
	codepage = (pathCharset == CHARSET_UTF8) ? CP_UTF8 : CP_THREAD_ACP;
	MultiByteToWideChar(codepage, MB_PRECOMPOSED, searchPath, -1, searchPathWide, ARRAY_LENGTH(searchPathWide));

	/* ファイル一覧を取得 */
	hFind = FindFirstFileW(searchPathWide, &win32fd);
	if (hFind == INVALID_HANDLE_VALUE) {
		CMNLOG_TRACE_END();
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
		WideCharToMultiByte(codepage, 0, win32fd.cFileName, -1, info->name, ARRAY_LENGTH(info->name), NULL, NULL);

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

	CMNLOG_TRACE_END();
	return list;
}

#else
/* ============================================================================
 *    Only Linux code
 * ========================================================================= */

static CmnDataList* ListForLinux(const char *path, CmnDataList *list)
{
	char newpath[CMN_FILE_MAX_PATH + CMN_FILE_MAX_FILE_NAME] = "";
	DIR *dir;
	struct dirent *dp;

	CMNLOG_TRACE_START();

	/* 最後の文字がパス区切りなら除去 */
	if (CmnString_EndWith(path, CMN_FILE_PATH_DELIMITER)) {
		strcpy(newpath, path);
		newpath[strlen(path) -1] = '\0';
		path = newpath;
	}

	/* TODO:readdirはスレッドセーフではない。readdir_rがスレッドセーフだが最新のgccでは非推奨となっているためmutexによる排他制御を行うこと。 */

	dir = opendir(path);
	if (dir == NULL) {
		CMNLOG_DEBUG("opendir return NULL, path is not exists or not permited. path=%s", path);
		CMNLOG_TRACE_END();
		return NULL;
	}

	dp = readdir(dir);
	while (dp != NULL) {
		struct stat childStat;
		char childPath[CMN_FILE_MAX_PATH + CMN_FILE_MAX_FILE_NAME] = "";
		CmnFileInfo *info;

		/* 「.」と「..」はスキップ */
		if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) {
			dp = readdir(dir);
			continue;
		}

		/* ファイル属性を取得 */
		strcat(childPath, path);
		strcat(childPath, CMN_FILE_PATH_DELIMITER);
		strcat(childPath, dp->d_name);
		if (stat(childPath, &childStat) < 0) {
			CMNLOG_DEBUG("get stat failed, path=%s", childPath);
			dp = readdir(dir);
			continue;
		}

		info = calloc(1, sizeof(CmnFileInfo));

		/* パス、ファイル名 */
		strcpy(info->dir, path);
		strcpy(info->name, dp->d_name);

		/* ファイルサイズ */
		info->size = childStat.st_size;

		/* 最終更新日時 */
		CmnTimeDateTime_SetBySerial(&(info->lastUpdateTime), childStat.st_mtime);

		/* ファイル属性 */
		if (S_ISDIR(childStat.st_mode)) {
			info->isDirectory = True;
		} else {
			info->isFile = True;
		}
		if (S_ISLNK(childStat.st_mode)) {
			info->isSymbolicLink = True;
		}
		if (CmnString_StartWith(info->name, ".")) {
			info->isHiddenFile = True;
		}
		if (!S_ISREG(childStat.st_mode) && !S_ISDIR(childStat.st_mode) && !S_ISLNK(childStat.st_mode)) {
			info->isSystemFile = True;
		}

		CmnDataList_Add(list, info);
		dp = readdir(dir);
	}

	if (dir != NULL) {
		closedir(dir);
	}

	CMNLOG_TRACE_END();
	return list;
}

#endif


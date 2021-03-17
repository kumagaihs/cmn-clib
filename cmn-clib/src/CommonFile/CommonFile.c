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
#include "cmnclib/CommonLog.h"

#if IS_PRATFORM_WINDOWS()
#include <windows.h>
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
static void Win32FileAttributeToCmnFileInfo(CmnFileInfo *info, DWORD sizeHigh, DWORD sizeLow, FILETIME *lastUpdateTime, DWORD attributes);
#else
static CmnDataList* ListForLinux(const char *path, CmnDataList *list);
static void FileStatToCmnFileInfo(CmnFileInfo *info, struct stat *stat);
#endif

/**
 * @brief ファイルをテキストデータとして全て読み込む
 *
 *  ファイルから読み込んだデータをbufにコピーして返却する。<br>
 *  ファイルはバイナリモードで読み込む。改行コードの変換が必要であれば呼び出し側で行うこと。
 *
 * @param filePath ファイルパス
 * @param buf 読み込んだデータを格納する文字列バッファ
 * @return 読み込みに成功した場合はbufを返す。失敗した場合はNULLを返却する。
 */
CmnStringBuffer* CmnFile_ReadAllText(const char *filePath, CmnStringBuffer *buf)
{
	CmnStringBuffer *ret;
	CmnDataBuffer *dat;
	CMNLOG_TRACE_START();

	dat = CmnDataBuffer_Create(0);
	if (dat == NULL) {
		CMNLOG_TRACE_END();
		return NULL;
	}

	if (CmnFile_ReadAll(filePath, dat) == NULL) {
		CmnDataBuffer_Free(dat);
		CMNLOG_TRACE_END();
		return NULL;
	}

	ret = NULL;
	if (CmnStringBuffer_SetByCmnDataBuffer(buf, dat) == 0) {
		/* 処理正常 */
		ret = buf;
	}
	CmnDataBuffer_Free(dat);

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
 * @param buf 読み込んだデータを格納するバッファ
 * @return 読み込んだデータ。読み込みに失敗した場合はNULLを返却する。
 */
CmnDataBuffer* CmnFile_ReadAll(const char *filePath, CmnDataBuffer *buf)
{
	FILE *fp;
	char tmp[BUF_SIZE];
	size_t readLen;
	CMNLOG_TRACE_START();

	if ((fp = fopen(filePath, "rb")) == NULL) {
		CMNLOG_TRACE_END();
		return NULL;
	}

	while ((readLen = fread(tmp, sizeof(tmp[0]), BUF_SIZE, fp)) > 0) {
		if (CmnDataBuffer_Append(buf, tmp, readLen) != 0) {
			CmnDataBuffer_Free(buf);
			fclose(fp);
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

	/* ファイル存在確認 */
	if (!CmnFile_Exists(path)) {
		CMNLOG_DEBUG("Not found, path=%s", path);
		CMNLOG_TRACE_END();
		return NULL;
	}

#if IS_PRATFORM_WINDOWS()
	ret = ListForWindows(path, list, pathCharset);
#else
	ret = ListForLinux(path, list);
#endif

	CMNLOG_TRACE_END();
	return ret;
}

/**
 * @brief pathが実在するかチェックする
 * @param path 実在するかチェックするパス。ディレクトリでもファイルでもOK
 * @return pathが存在する場合はTrue、存在しない場合はFalseを返す
*/
int CmnFile_Exists(const char *path)
{
	int ret = False;
	CMNLOG_TRACE_START();

#if IS_PRATFORM_WINDOWS()
	if (_access(path, 0) != -1) {
		ret = True;
	}
#else
	{
		struct stat st;
		if (stat(path, &st) == 0) {
			ret = True;
		}
	}
#endif

	CMNLOG_TRACE_END();
	return ret;
}

/**
 * @brief ファイル情報を取得する
 * @param path ファイルパス
 * @param info 取得したファイル情報を格納する
 * @return infoを返す。ファイル情報の取得に失敗した場合はNULLを返す。
 */
CmnFileInfo* CmnFile_GetFileInfo(const char *path, CmnFileInfo *info)
{
	CmnFileInfo *ret = NULL;
	char newpath[CMN_FILE_MAX_PATH + CMN_FILE_MAX_FILE_NAME] = "";
	CMNLOG_TRACE_START();

	/* 最後の文字がパス区切りなら除去 */
	if (CmnString_EndWith(path, CMN_FILE_PATH_DELIMITER)) {
		strcpy(newpath, path);
		newpath[strlen(path) - 1] = '\0';
		path = newpath;
	}

	/* ファイル存在確認 */
	if ( ! CmnFile_Exists(path)) {
		CMNLOG_DEBUG("Not found, path=%s", path);
		CMNLOG_TRACE_END();
		return ret;
	}

	memset(info, 0, sizeof(CmnFileInfo));

#if IS_PRATFORM_WINDOWS()
	{
		WIN32_FILE_ATTRIBUTE_DATA fileInfo;
		wchar_t pathWide[MAX_PATH_SIZE * 2] = { '\0' };

		MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, path, -1, pathWide, ARRAY_LENGTH(pathWide));
		if (GetFileAttributesExW(pathWide, GetFileExInfoStandard, &fileInfo) == 0) {
			CMNLOG_DEBUG("Failed to get file info, path=%s", path);	
		}
		else {
			Win32FileAttributeToCmnFileInfo(info, fileInfo.nFileSizeHigh, fileInfo.nFileSizeLow, &(fileInfo.ftLastWriteTime), fileInfo.dwFileAttributes);
			ret = info;
		}
	}
#else
	{
		struct stat stat;
		if (stat(path, &stat) < 0) {
			CMNLOG_DEBUG("get stat failed, path=%s", path);
		}
		else {
			FileStatToCmnFileInfo(info, &stat);
			ret = info;
		}
	}
#endif

	/* dir, nameを設定 */
	if (ret != NULL) {
		int pathDelimPos;

		/* 最後のパス区切り文字を検索 */
		if ((pathDelimPos = CmnString_LastIndexOf(path, CMN_FILE_PATH_DELIMITER)) == -1) {
			/* パス区切り文字が環境定義と異なりスラッシュを指定しているかもしれないのでスラッシュでも検索 */
			pathDelimPos = CmnString_LastIndexOf(path, "/");
		}

		if (pathDelimPos == -1) {
			strcpy(info->parentDir, ".");
			strcpy(info->name, path);
		}
		else {
			strncpy(info->parentDir, path, pathDelimPos);
			info->parentDir[pathDelimPos] = '\0';
			strcpy(info->name, path + pathDelimPos + 1);
		}
	}

	CMNLOG_TRACE_END();
	return ret;
}

char* CmnFileInfo_ToString(const CmnFileInfo *info, char *buf)
{
	char timeBuf[160];
	CMNLOG_TRACE_START();

	*buf = '\0';
	sprintf(buf, "parentDir=%s, name=%s, size=%I64d, lastUpdateTime=[%s], isDirectory=%d, isFile=%d, isHiddenFile=%d, isSystemFile=%d, isSymbolicLink=%d",
			info->parentDir,
			info->name,
			info->size,
			CmnTime_Format(&info->lastUpdateTime, CMN_TIME_FORMAT_ALL, timeBuf),
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
	UINT codepage;

	CmnFileInfo *info;
	char newpath[CMN_FILE_MAX_PATH + CMN_FILE_MAX_FILE_NAME] = "";

	CMNLOG_TRACE_START();

	/* 最後の文字がパス区切りなら除去 */
	if (CmnString_EndWith(path, CMN_FILE_PATH_DELIMITER)) {
		strcpy(newpath, path);
		newpath[strlen(path) - 1] = '\0';
		path = newpath;
	}

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
		strcpy(info->parentDir, path);
		WideCharToMultiByte(codepage, 0, win32fd.cFileName, -1, info->name, ARRAY_LENGTH(info->name), NULL, NULL);

		/* ファイル情報を設定 */
		Win32FileAttributeToCmnFileInfo(info, win32fd.nFileSizeHigh, win32fd.nFileSizeLow, &(win32fd.ftLastWriteTime), win32fd.dwFileAttributes);

		CmnDataList_Add(list, info);
	} while (FindNextFile(hFind, &win32fd));

	FindClose(hFind);

	CMNLOG_TRACE_END();
	return list;
}

/**
 * @brief Win32から取得したファイル情報をCmnFileInfoに設定する
 * @param info CmnFileInfo
 * @param sizeHigh ファイルサイズ（上位ビット）
 * @param sizeLow ファイルサイズ（下位ビット）
 * @param lastUpdateTime 最終更新日時
 * @param attributes ファイル属性
 */
static void Win32FileAttributeToCmnFileInfo(CmnFileInfo *info, DWORD sizeHigh, DWORD sizeLow, FILETIME *lastUpdateTime, DWORD attributes)
{
	SYSTEMTIME win32time;
	CmnTimeDateTime cmnTime;

	/* ファイルサイズ */
	info->size = (sizeHigh * (MAXDWORD + 1)) + sizeLow;

	/* 最終更新日時  XXX:タイムゾーンがゼロ（グリニッジ標準時）の時刻が取れるため、日本なら＋9時間してやる必要がある。 */
	FileTimeToSystemTime(lastUpdateTime, &win32time);
	CmnTimeDateTime_Set(&cmnTime, win32time.wYear, win32time.wMonth, win32time.wDay, win32time.wHour, win32time.wMinute, win32time.wSecond, -1);
	info->lastUpdateTime = cmnTime;

	/* ファイル属性 */
	if (attributes & FILE_ATTRIBUTE_DIRECTORY) {
		info->isDirectory = True;
	}
	else {
		info->isFile = True;
	}
	if (attributes & FILE_ATTRIBUTE_HIDDEN) {
		info->isHiddenFile = True;
	}
	if (attributes & FILE_ATTRIBUTE_SYSTEM) {
		info->isSystemFile = True;
	}
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
		strcpy(info->parentDir, path);
		strcpy(info->name, dp->d_name);

		/* ファイル情報設定 */
		FileStatToCmnFileInfo(info, &childStat);

		CmnDataList_Add(list, info);
		dp = readdir(dir);
	}

	if (dir != NULL) {
		closedir(dir);
	}

	CMNLOG_TRACE_END();
	return list;
}

/**
 * @brief statから取得したファイル情報をCmnFileInfoに設定する
 * @param info CmnFileInfo
 * @param stat ファイル情報
 */
static void FileStatToCmnFileInfo(CmnFileInfo *info, struct stat *stat)
{
	/* ファイルサイズ */
	info->size = stat.st_size;

	/* 最終更新日時 */
	CmnTimeDateTime_SetBySerial(&(info->lastUpdateTime), stat.st_mtime);

	/* ファイル属性 */
	if (S_ISDIR(stat.st_mode)) {
		info->isDirectory = True;
	}
	else {
		info->isFile = True;
	}
	if (S_ISLNK(stat.st_mode)) {
		info->isSymbolicLink = True;
	}
	if (CmnString_StartWith(info->name, ".")) {
		info->isHiddenFile = True;
	}
	if (!S_ISREG(stat.st_mode) && !S_ISDIR(stat.st_mode) && !S_ISLNK(stat.st_mode)) {
		info->isSystemFile = True;
	}
}

#endif


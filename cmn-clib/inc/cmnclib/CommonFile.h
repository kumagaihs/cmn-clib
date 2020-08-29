/** @file *********************************************************************
 * @brief ファイル操作共通関数群 ヘッダファイル
 *
 *  ファイル操作共通関数群のヘッダファイル。
 *
 * @author H.Kumagai
 * @date   2020-05-08
 *****************************************************************************/

#ifndef _COMMON_FILE_H
#define _COMMON_FILE_H

#include "cmnclib/Common.h"
#include "cmnclib/CommonData.h"
#include "cmnclib/CommonString.h"
#include "cmnclib/CommonTime.h"

/* 不要
typedef enum {
	CMN_FILE_WRITE_MODE_NEW,
	CMN_FILE_WRITE_MODE_ADD_HEAD,
	CMN_FILE_WRITE_MODE_ADD_TAIL
} CMN_FILE_WRITE_MODE;
*/

#if IS_PRATFORM_WINDOWS()
	/** パス区切り文字 */
	#define CMN_FILE_PATH_DELIMITER "\\"
	/** 最大パス長 */
	#define CMN_FILE_MAX_PATH (4096)
	/** 最大ファイル名長 */
	#define CMN_FILE_MAX_FILE_NAME (1024)
#else
	/** パス区切り文字 */
	#define CMN_FILE_PATH_DELIMITER "/"
	/** 最大パス長 */
	#define CMN_FILE_MAX_PATH (4096)
	/** 最大ファイル名長 */
	#define CMN_FILE_MAX_FILE_NAME (1024)
#endif

/** ファイル情報構造体 */
typedef struct _tag_CmnFileInfo {
	char dir[CMN_FILE_MAX_PATH];			/**< ファイル格納先ディレクトリ */
	char name[CMN_FILE_MAX_FILE_NAME];		/**< ファイル名/ディレクトリ名 */
	size_t size;							/**< ファイルサイズ。ディレクトリの場合は常にゼロ */
	CmnTimeDateTime lastUpdateTime;			/**< 最終更新日時 */
	unsigned int isDirectory: 1;			/**< 属性フラグ：ディレクトリの場合に1 */
	unsigned int isFile: 1;				/**< 属性フラグ：ファイルの場合に1 */
	unsigned int isHiddenFile: 1;			/**< 属性フラグ：隠しファイルの場合に1 */
	unsigned int isSystemFile: 1;			/**< 属性フラグ：システムファイルの場合に1 */
	unsigned int isSymbolicLink: 1;		/**< 属性フラグ：シンボリックリンクの場合に1 */
} CmnFileInfo;

/* --- CommonFile.c --- */
D_EXTERN char* CmnFile_ReadAllText(const char *filePath);
D_EXTERN CmnDataBuffer* CmnFile_ReadAll(const char *filePath);
D_EXTERN int CmnFile_WriteNew(const char *filePath, void *data, size_t len);
D_EXTERN int CmnFile_WriteHead(const char *filePath, void *data, size_t len);
D_EXTERN int CmnFile_WriteTail(const char *filePath, void *data, size_t len);
D_EXTERN CmnDataList* CmnFile_List(const char *path, CmnDataList *list);
/* TODO:絶対パス変換 */
D_EXTERN char* CmnFile_ToAbsolutePath(const char *path, char *buf);
/* TODO:ファイルの情報取得 */
D_EXTERN CmnFileInfo* CmnFile_GetFileInfo(const char *path, CmnFileInfo *info);
D_EXTERN char* CmnFileInfo_ToString(const CmnFileInfo *info, char *buf);



#endif /* _COMMON_FILE_H */

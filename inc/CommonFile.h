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

#include "Common.h"
#include "CommonData.h"

/* 不要
typedef enum {
	CMN_FILE_WRITE_MODE_NEW,
	CMN_FILE_WRITE_MODE_ADD_HEAD,
	CMN_FILE_WRITE_MODE_ADD_TAIL
} CMN_FILE_WRITE_MODE;
*/

/* --- CommonFile.c --- */
D_EXTERN char* CmnFile_ReadAllText(char *filePath);
D_EXTERN CmnDataBuffer* CmnFile_ReadAll(char *filePath);
D_EXTERN int CmnFile_WriteNew(char *filePath, void *data, size_t len);
D_EXTERN int CmnFile_WriteHead(char *filePath, void *data, size_t len);
D_EXTERN int CmnFile_WriteTail(char *filePath, void *data, size_t len);
/** TODO:ファイルの存在確認 */
/** TODO:ディレクトリ内のファイル一覧を取得 */


#endif /* _COMMON_FILE_H */

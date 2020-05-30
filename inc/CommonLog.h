/** @file *********************************************************************
 * @brief ログ出力系 共通関数 I/Fヘッダファイル
 *
 *  ログ出力系共通関数を使用するためのI/Fヘッダファイル。<br>
 *  ログ出力系の共通関数を使用する場合は、このヘッダファイルを読み込むこと
 *
 * @author H.Kumagai
 * @date   2004-06-07
 * $Revision: 1.2 $
 *****************************************************************************/

#ifndef _COMMON_LOG_H
#define _COMMON_LOG_H

#include "Common.h"


/** ログメッセージ格納構造体 */
typedef struct tag_CmnLogMessage {
	char *code;							/**< メッセージコード */
	char *msg;							/**< メッセージ文     */
	struct tag_CmnLogMessage *next;	/**< Nextポインタ     */
} CmnLogMessage;

/** 拡張ログ情報 */
typedef struct tag_CmnLogEx {
	int level;				/**< ログ出力レベル       */
	char *file;				/**< ログ出力ファイル     */
	CmnLogMessage *list;	/**< ログメッセージリスト */
} CmnLogEx;

/** ログ出力レベル */
enum {
	CMN_LOG_LEVEL_NOTHING,     /**< 沈黙レベル（全てのログを出力しない） */
	CMN_LOG_LEVEL_ERROR,       /**< エラーレベル（ERRORで指定されたログを出力） */
	CMN_LOG_LEVEL_WARN,        /**< 警告レベル（ERROR/WARNで指定されたログを出力） */
	CMN_LOG_LEVEL_INFO,        /**< 通知レベル（ERROR/WARN/INFOで指定されたログを出力） */
	CMN_LOG_LEVEL_DEBUG,       /**< デバッグレベル（全てのログを出力） */
	CMN_LOG_LEVEL_MAX = CMN_LOG_LEVEL_DEBUG  /**< ログレベルに指定できる最大値 */
};

/* --- CommonLog.c --- */
/* 標準ログ出力関数初期化処理 */
D_EXTERN int CmnLog_Init(const char *log_file, int level);
/* 標準ログ出力共通関数終了処理 */
D_EXTERN void CmnLog_End();
/* 標準ログ出力 */
D_EXTERN void CmnLog_Put(int level, const char *msgCode, ...);
/* ログメッセージ定義ファイル読み込み */
D_EXTERN CmnLogMessage* _CmnLogMessage_Create(const char *msgFile);
/* ログメッセージリスト解放処理 */
D_EXTERN void _CmnLogMessage_Free(CmnLogMessage *list);
/* ログメッセージ取得関数 */
D_EXTERN int _CmnLogMessage_Get(CmnLogMessage *list, const char *msg_code, CmnLogMessage *msg);

/* --- CommonLogEx.c --- */
/* 拡張ログ出力関数初期化処理 */
D_EXTERN CmnLogEx *CmnLogEx_Init(const char *msgFile, int level, const char *file);
/* 拡張ログ出力共通関数終了処理 */
D_EXTERN void CmnLogEx_End(CmnLogEx *log);
/* 拡張ログ出力 */
D_EXTERN void CmnLogEx_Put(CmnLogEx *log, int level, const char *msgCode, ...);

/* ラッパーマクロ */
#define CMNLOG_DEBUG(code, ...) CmnLog_Put(CMN_LOG_LEVEL_DEBUG, (code), __VA_ARGS__)
#define CMNLOG_INFO(code, ...) CmnLog_Put(CMN_LOG_LEVEL_INFO, (code), __VA_ARGS__)
#define CMNLOG_WARN(code, ...) CmnLog_Put(CMN_LOG_LEVEL_WARN, (code), __VA_ARGS__)
#define CMNLOG_ERROR(code, ...) CmnLog_Put(CMN_LOG_LEVEL_ERROR, (code), __VA_ARGS__)

#endif /* _COMMON_LOG_H */


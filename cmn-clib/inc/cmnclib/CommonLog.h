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

#include <time.h>

#include "cmnclib/Common.h"
#include "cmnclib/CommonThread.h"


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
	CmnThreadMutex* mutex;	/**< ログ出力処理をスレッドセーフにするためのミューテックス */
} CmnLogEx;

/** ログ出力レベル */
typedef enum {
	CMN_LOG_LEVEL_NOTHING,     /**< 沈黙レベル（全てのログを出力しない） */
	CMN_LOG_LEVEL_ERROR,       /**< エラーレベル（ERRORで指定されたログを出力） */
	CMN_LOG_LEVEL_WARN,        /**< 警告レベル（ERROR/WARNで指定されたログを出力） */
	CMN_LOG_LEVEL_INFO,        /**< 通知レベル（ERROR/WARN/INFOで指定されたログを出力） */
	CMN_LOG_LEVEL_DEBUG,       /**< デバッグレベル（ERROR/WARN/INFO/DEBUGで指定されたログを出力） */
	CMN_LOG_LEVEL_TRACE,       /**< トレースレベル（全てのログを出力） */
	CMN_LOG_LEVEL_MAX = CMN_LOG_LEVEL_TRACE  /**< ログレベルに指定できる最大値 */
} CMN_LOG_LEVEL;

/* --- CommonLog.c --- */
/* 標準ログ出力関数初期化処理 */
D_EXTERN int CmnLog_Init(const char* logFile, CMN_LOG_LEVEL level, const char* msgFile);
/* 標準ログ出力共通関数終了処理 */
D_EXTERN void CmnLog_End();
/* 標準ログ出力（メッセージ指定） */
D_EXTERN void CmnLog_Put(int level, const char *msgCode, ...);
/* 標準ログ出力（メッセージコード指定） */
D_EXTERN void CmnLog_PutByCode(CmnLogEx* log, CMN_LOG_LEVEL level, const char* msgCode, ...);
/* cmn-clibの内部ログ出力設定 */
D_EXTERN void CmnLog_InitCmnClibLog(CmnLogEx *log, CMN_LOG_LEVEL level);
/* cmn-clibの内部ログ出力終了 */
D_EXTERN void CmnLog_EndCmnClibLog();
/* cmn-clibの内部ログ出力 */
D_EXTERN void CmnLog_PutCmnClibLog(CMN_LOG_LEVEL level, const char* msgCode, ...);

/* --- CommonLogEx.c --- */
/* 拡張ログ出力関数初期化処理 */
D_EXTERN CmnLogEx* CmnLogEx_Create(const char* logFile, CMN_LOG_LEVEL level, const char* msgFile);
/* 拡張ログ出力共通関数終了処理 */
D_EXTERN void CmnLogEx_Free(CmnLogEx *log);
/* 拡張ログ出力（メッセージ指定） */
D_EXTERN void CmnLogEx_Put(CmnLogEx* log, CMN_LOG_LEVEL level, const char* msg, ...);
/* 拡張ログ出力（メッセージコード指定） */
D_EXTERN void CmnLogEx_PutByCode(CmnLogEx* log, CMN_LOG_LEVEL level, const char* msgCode, ...);
/* 拡張ログ出力（ログ出力共通） */
D_EXTERN void cmnLogEx_PutLog(CmnLogEx* log, CMN_LOG_LEVEL level, const char* msg, va_list args);

/* --- CommonLogMessage.c --- */
/* ログメッセージ定義ファイル読み込み */
D_EXTERN CmnLogMessage* CmnLogMessage_Create(const char* msgFile);
/* ログメッセージリスト解放処理 */
D_EXTERN void CmnLogMessage_Free(CmnLogMessage* list);
/* ログメッセージ取得関数 */
D_EXTERN int CmnLogMessage_Get(CmnLogMessage* list, const char* msg_code, CmnLogMessage* msg);

/* cmn-clib内部ログ出力ラッパーマクロ */
#ifdef CMN_CLIB_HI_PERFORMANCE
	#define CMNLOG_TRACE_START()
	#define CMNLOG_TRACE_END()
#else
	#define CMNLOG_TRACE_START() clock_t cmnlogtrace_stclock=clock();CmnLog_PutCmnClibLog(CMN_LOG_LEVEL_TRACE, "START %s", __func__)
	#define CMNLOG_TRACE_END() CmnLog_PutCmnClibLog(CMN_LOG_LEVEL_TRACE, "END %s(clocks=%d)", __func__, clock() - cmnlogtrace_stclock)
#endif
#define CMNLOG_TRACE(msg, ...) CmnLog_PutCmnClibLog(CMN_LOG_LEVEL_TRACE, (msg), __VA_ARGS__)
#define CMNLOG_DEBUG(msg, ...) CmnLog_PutCmnClibLog(CMN_LOG_LEVEL_DEBUG, (msg), __VA_ARGS__)
#define CMNLOG_INFO(msg, ...) CmnLog_PutCmnClibLog(CMN_LOG_LEVEL_INFO, (msg), __VA_ARGS__)
#define CMNLOG_WARN(msg, ...) CmnLog_PutCmnClibLog(CMN_LOG_LEVEL_WARN, (msg), __VA_ARGS__)
#define CMNLOG_ERROR(msg, ...) CmnLog_PutCmnClibLog(CMN_LOG_LEVEL_ERROR, (msg), __VA_ARGS__)

#endif /* _COMMON_LOG_H */


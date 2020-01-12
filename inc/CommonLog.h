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
typedef struct tag_CmnLog_LogMessage {
	char *code;							/**< メッセージコード */
	char *msg;							/**< メッセージ文     */
	struct tag_CmnLog_LogMessage *next;	/**< Nextポインタ     */
} CmnLog_LogMessage;

/** 拡張ログ情報 */
typedef struct tag_CmnLog_LogEx {
	int level;				/**< ログ出力レベル       */
	char *file;				/**< ログ出力ファイル     */
	CmnLog_LogMessage *list;	/**< ログメッセージリスト */
} CmnLog_LogEx;

/** ログ出力レベル */
enum {
	CMN_LOG_LEVEL_NOTHING,     /**< 沈黙レベル（全てのログを出力しない） */
	CMN_LOG_LEVEL_STANDARD,    /**< 標準レベル（STANDARDで指定されたログを出力） */
	CMN_LOG_LEVEL_DETAIL,      /**< 詳細レベル（DETAILとSTANDARDで指定されたログを出力） */
	CMN_LOG_LEVEL_DEBUG,       /**< デバッグレベル（全てのログを出力） */
	CMN_LOG_LEVEL_MAX = CMN_LOG_LEVEL_DEBUG  /**< ログレベルに指定できる最大値 */
};


/* DLL使用かLIB使用かによるプロトタイプ切り替え */
#ifdef _USRDLL
  /* DLL作成 */
  #define D_EXTERN extern __declspec(dllexport)
#else
  #ifdef COMMON_DLL_IMPORT
    /* DLL使用 */
    #define D_EXTERN extern __declspec(dllimport)
  #else
    /* LIB使用 */
    #define D_EXTERN extern
  #endif
#endif

/* --- CommonLog.c --- */
/* 標準ログ出力関数初期化処理 */
D_EXTERN int CmnLog_Init(const char *log_file, int level);
/* ログメッセージ定義ファイル読み込み */
D_EXTERN CmnLog_LogMessage *cmnLog_CreateLogMessageList(const char *msgFile);
/* 標準ログ出力共通関数終了処理 */
D_EXTERN void CmnLog_End();
/* ログメッセージリスト解放処理 */
D_EXTERN void cmnLog_ReleaseLogMessageList(CmnLog_LogMessage *list);
/* 標準ログ出力 */
D_EXTERN void CmnLog_Put(int level, const char *msgCode, ...);
/* ログメッセージ取得関数 */
D_EXTERN int cmnLog_GetMessage(CmnLog_LogMessage *list, const char *msg_code, CmnLog_LogMessage *msg);

/* --- CommonLogEx.c --- */
/* 拡張ログ出力関数初期化処理 */
D_EXTERN CmnLog_LogEx *CmnLog_InitEx(const char *msgFile, int level, const char *file);
/* 拡張ログ出力共通関数終了処理 */
D_EXTERN void CmnLog_EndEx(CmnLog_LogEx *log);
/* 拡張ログ出力 */
D_EXTERN void CmnLog_PutEx(CmnLog_LogEx *log, int level, const char *msgCode, ...);


#endif /* _COMMON_LOG_H */


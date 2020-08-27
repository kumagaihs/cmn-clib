/** @file *********************************************************************
 * @brief ネットワークライブラリ ヘッダファイル
 *
 *  ネットワークライブラリのヘッダファイル。<br>
 *
 * @author H.Kumagai
 * @date   2020-07-26
 *****************************************************************************/

#ifndef _COMMON_NET_H_
#define _COMMON_NET_H_

#include "cmnclib/Common.h"
#include "cmnclib/CommonData.h"
#include "cmnclib/CommonThread.h"

#if IS_PRATFORM_WINDOWS()
	#include <winsock2.h>
#else
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
#endif

/** ソケットオプション：リクエスト送信後にFIN送信（送信側クローズ）を行う */
#define CNS_OPT_GRACEFUL_CLOSE			0x01

/** ソケットステータス */
typedef enum {
	CNS_SUCCESS = 0,					/**< 正常 */
	CNS_INPUT_ERROR,					/**< I/Oエラー：INPUT */
	CNS_OUTPUT_ERROR,					/**< I/Oエラー：OUTPUT */
	CNS_ADDRESS_ERROR,					/**< IPアドレス変換エラー */
	CNS_SERVER_INITIAL_ERROR = 101,		/**< サーバーエラー：初期化 */
	CNS_BIND_ERROR,						/**< サーバーエラー：BIND */
	CNS_LISTEN_ERROR,					/**< サーバーエラー：LISTEN */
	CNS_ACCEPT_ERROR,					/**< サーバーエラー：ACCEPT */
	CNS_MEMORY_OVER_ERROR,				/**< サーバーエラー：メモリ確保失敗 */
	CNS_CLIENT_INITIAL_ERROR = 201,		/**< クライアントエラー：初期化 */
	CNS_CONNECT_ERROR,					/**< クライアントエラー：接続 */
} CmnNetSocketStatus;

/** ソケットオブジェクト（ネイティブソケットのラッパー） */
typedef struct tag_CmnNetSocket {
#if IS_PRATFORM_WINDOWS()
	SOCKET socketId;		/**< ソケットハンドラ */
#else
	int socketId;			/**< ソケットハンドラ */
#endif
} CmnNetSocket;

/** ソケット待ち受けサーバーオブジェクト */
typedef struct tag_CmnNetSocketServer {
	CmnNetSocket *serverSocket;		/**< bind/listenしているサーバーソケット */
	CmnThread *acceptThread;		/**< サーバー待ち受けループ(accept)を行うスレッド */
	void (*serverMainProc)(CmnNetSocket*);		/**< サーバー主処理 */
	CmnNetSocketStatus errorStatus;				/**< サーバー処理ステータス。異常終了した場合に設定。 */
	volatile int _isRunnable;					/**< 処理継続フラグ。1:継続、0:処理終了 */
} CmnNetSocketServer;

typedef struct tag_CmnNetHttpResponse {
/* TODO CmnDataMap(JavaのHashMapみたいなの)が実装されたら有効にすること
	CmnDataMap *header;
 */
	CmnDataBuffer *body;
} CmnNetHttpResponse;

D_EXTERN CmnNetSocketStatus CmnNetSocket_StartServer(unsigned short port, void (*serverMainProc)(CmnNetSocket*), CmnNetSocketServer *server);
D_EXTERN CmnNetSocketStatus CmnNetSocket_EndServer(CmnNetSocketServer *server);
D_EXTERN CmnNetSocketStatus CmnNetSocket_NoSessionRequest(const char *host, unsigned short port, CmnDataBuffer *request, CmnDataBuffer *response, const char *responseEndMark, const int responseEndMarkLen, int opt);
D_EXTERN CmnNetSocketStatus CmnNetSocket_ReceiveAll(CmnNetSocket *socket, CmnDataBuffer *buf, const char *endMark, const int endMarkLen);
D_EXTERN CmnNetSocketStatus CmnNetSocket_SendAll(CmnNetSocket *socket, const void *data, int len);
D_EXTERN CmnNetSocketStatus CmnNetSocket_ToSocketAddress(const char *host, unsigned short port, struct sockaddr_in *addr);

D_EXTERN CmnNetHttpResponse* CmnNetHttp_GetRequest(const char *ip, unsigned short port, const char *path);
D_EXTERN CmnNetHttpResponse* CmnNetHttp_PostRequest(const char *ip, unsigned short port, const char *path, CmnDataBuffer *requestBody);

#endif /* _COMMON_NET_H_ */

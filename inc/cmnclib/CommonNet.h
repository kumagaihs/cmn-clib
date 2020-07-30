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

#if IS_PRATFORM_WINDOWS()
	#include <winsock2.h>
#else
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
#endif

/** ソケットステータス */
typedef enum {
	CNS_SUCCESS = 0,					/**< 正常 */
	CNS_INPUT_ERROR,					/**< I/Oエラー：INPUT */
	CNS_OUTPUT_ERROR,					/**< I/Oエラー：OUTPUT */
	CNS_SERVER_INITIAL_ERROR = 101,		/**< サーバーエラー：初期化 */
	CNS_BIND_ERROR,						/**< サーバーエラー：BIND */
	CNS_LISTEN_ERROR,					/**< サーバーエラー：LISTEN */
	CNS_ACCEPT_ERROR,					/**< サーバーエラー：ACCEPT */
	CNS_CLIENT_INITIAL_ERROR = 201,		/**< クライアントエラー：初期化 */
	CNS_CONNECT_ERROR,					/**< クライアントエラー：接続 */
} CmnNetSocketStatus;

/** ソケットオブジェクト（ネイティブソケットのラッパー） */
typedef struct tag_CmnNetSocket {
#if IS_PRATFORM_WINDOWS()
	SOCKET socket;		/**< ソケットハンドラ */
#else
	int socket;			/**< ソケットハンドラ */
#endif
} CmnNetSocket;

typedef struct tag_CmnNetHttpResponse {
/* TODO CmnDataMap(JavaのHashMap)が実装されたら有効にすること
	CmnDataMap *header;
 */
	CmnDataBuffer *body;
} CmnNetHttpResponse;

D_EXTERN CmnNetSocketStatus CmnNetSocket_StartSimpleServer(unsigned short port, void (*method)(CmnNetSocket*));
D_EXTERN CmnNetSocketStatus CmnNetSocket_NoSessionRequest(const char *ip, unsigned short port, CmnDataBuffer *request, CmnDataBuffer *response);

D_EXTERN CmnNetHttpResponse* CmnNetHttp_GetRequest(const char *ip, unsigned short port, const char *path);
D_EXTERN CmnNetHttpResponse* CmnNetHttp_PostRequest(const char *ip, unsigned short port, const char *path, CmnDataBuffer *requestBody);

#endif /* _COMMON_NET_H_ */

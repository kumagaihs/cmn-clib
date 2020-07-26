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

#if IS_PRATFORM_WINDOWS()
	#include <winsock2.h>
#else
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
#endif

typedef struct tag_CmnNetSocket {
#if IS_PRATFORM_WINDOWS()
	SOCKET socket;
#else
	int socket;
#endif
} CmnNetSocket;

typedef struct tag_CmnNetHttpResponse {
/* TODO CmnDataMap(JavaのHashMap)が実装されたら有効にすること
	CmnDataMap *header;
 */
	CmnDataBuffer *body;
} CmnNetHttpResponse;

D_EXTERN void CmnNetSocket_StartSimpleServer(unsigned short port, void (*method)(CmnNetSocket*));
D_EXTERN CmnDataBuffer* CmnNetSocket_NoSessionRequest(const char *ip, unsigned short port, CmnDataBuffer *requestData);

D_EXTERN CmnNetHttpResponse* CmnNetHttp_GetRequest(const char *ip, unsigned short port, const char *path);
D_EXTERN CmnNetHttpResponse* CmnNetHttp_PostRequest(const char *ip, unsigned short port, const char *path, CmnDataBuffer *requestBody);

#endif /* _COMMON_NET_H_ */

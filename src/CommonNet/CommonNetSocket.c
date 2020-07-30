/** @file *********************************************************************
 * @brief ネットワークライブラリ TCP/IPソケット関連機能
 * @author H.Kumagai
 *****************************************************************************/

#include "cmnclib/CommonNet.h"
#include "cmnclib/CommonData.h"

#if IS_PRATFORM_WINDOWS()
	#include <winsock2.h>
#else
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
#endif

#if IS_PRATFORM_WINDOWS()
	#define CmnNetSocket_isSocketError(x) (x == INVALID_SOCKET)
#else
	#define CmnNetSocket_isSocketError(x) (x < 0)
#endif

/**
 * @brief TCP/IP待ち受けサーバー起動
 *
 *  特定ポートにLISTENするサーバー処理を開始する。<br>
 *  portで待ち受けを開始し、アクセスがあればmethodを新規スレッドでコールする。
 *  待ち受けを終了する場合は。。。 TODO:検討すること。
 *
 * @param port 通信を待ち受けるポート
 * @param method portにアクセスがあった場合に呼び出す処理
 * @return 処理結果ステータス
 */
CmnNetSocketStatus CmnNetSocket_StartSimpleServer(unsigned short port, void (*method)(CmnNetSocket*))
{
#if IS_PRATFORM_WINDOWS()
	WSADATA wsaData;
	BOOL flagOn = 1;
#else
	int flagOn = 1;
#endif
	CmnNetSocket serverSocket;

	struct sockaddr_in addr;
	struct sockaddr_in client;
	int len;

	char buf[2048];
	char inbuf[2048];

#if IS_PRATFORM_WINDOWS()
	WSAStartup(MAKEWORD(2, 0), &wsaData);
#endif

	serverSocket.socket = socket(AF_INET, SOCK_STREAM, 0);
	if (CmnNetSocket_isSocketError(serverSocket.socket)) {
		return CNS_SERVER_INITIAL_ERROR;
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
#if IS_PRATFORM_WINDOWS()
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
#else
	addr.sin_addr.s_addr = INADDR_ANY;
#endif

	setsockopt(serverSocket.socket,
			SOL_SOCKET, SO_REUSEADDR, (const char*) &flagOn, sizeof(flagOn));

	if (bind(serverSocket.socket, (struct sockaddr*) &addr, sizeof(addr)) != 0) {
		return CNS_BIND_ERROR;
	}

	if (listen(serverSocket.socket, 5) != 0) {
		return CNS_LISTEN_ERROR;
	}

	while (1) {
		CmnNetSocket sock;

		/* TODO:method呼び出し */

		len = sizeof(client);
		sock.socket = accept(serverSocket.socket, (struct sockaddr*) &client, &len);
		if (CmnNetSocket_isSocketError(sock.socket)) {
			return CNS_ACCEPT_ERROR;
		}

		memset(inbuf, 0, sizeof(inbuf));
		recv(sock.socket, inbuf, sizeof(inbuf), 0);
		// 本来ならばクライアントからの要求内容をパースすべきです
		printf("%s", inbuf);

		// 相手が何を言おうとダミーHTTPメッセージ送信
		send(sock.socket, buf, (int) strlen(buf), 0);

#if IS_PRATFORM_WINDOWS()
		closesocket(sock.socket);
#else
		close(sock.socket);
#endif
	}

#if IS_PRATFORM_WINDOWS()
	WSACleanup();
#endif

	return 0;
}

/**
 * @brief セッションレスTCP/IPリクエスト送信
 *
 *  セッションがなく、１回のRequest→Responseで完結するTCP/IPリクエストを送信する。<br>
 *
 * @param ip リモートホストのIPアドレス
 * @param port リモートホストのポート
 * @param request リクエストデータ
 * @param response レスポンスデータ
 * @return ステータス
 */
CmnNetSocketStatus CmnNetSocket_NoSessionRequest(const char *ip, unsigned short port, CmnDataBuffer *request, CmnDataBuffer *response)
{
	/* TODO:実装 */
	return CNS_SUCCESS;
}




/** @file *********************************************************************
 * @brief ネットワークライブラリ TCP/IPソケット関連機能
 * @author H.Kumagai
 *****************************************************************************/

#include "cmnclib/CommonNet.h"
#include "cmnclib/CommonData.h"
#include "cmnclib/CommonThread.h"
#include "cmnclib/CommonLog.h"

#if IS_PRATFORM_WINDOWS()
	#include <winsock2.h>
#else
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netdb.h>
#endif

#if IS_PRATFORM_WINDOWS()
	#define CmnNetSocket_isSocketError(x) (x == INVALID_SOCKET)
#else
	#define CmnNetSocket_isSocketError(x) (x < 0)
#endif

#define CMNNETSOCKET_RECEIVE_BUFFER_SIZE 4096
#define CMNNETSOCKET_ACCEPT_TIMEOUT_SEC 1

typedef struct tag_RunServerMainProcParam {
	CmnNetSocket *socket;
	void (*serverMainProc)(CmnNetSocket*);
} RunServerMainProcParam;

/**
 * @brief ソケットを利用するためのグローバルリソースの確保を行う。
 */
static void allocateGlobalSocketResource()
{
	CMNLOG_TRACE_START();
	#if IS_PRATFORM_WINDOWS()
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 0), &wsaData);
	#endif
	CMNLOG_TRACE_END();
}

/**
 * @brief ソケットを利用するためのグローバルリソースの解放を行う。
 */
static void releaseGlobalSocketResource()
{
	CMNLOG_TRACE_START();
	#if IS_PRATFORM_WINDOWS()
		WSACleanup();
	#endif
	CMNLOG_TRACE_END();
}

/**
 * @brief ソケットをクローズする
 * @param sock ソケット
 */
static void closeSocket(CmnNetSocket *sock)
{
	CMNLOG_TRACE_START();
	#if IS_PRATFORM_WINDOWS()
		closesocket(sock->socketId);
	#else
		close(sock->socketId);
	#endif
	CMNLOG_TRACE_END();
}

/**
 * @brief サーバー主処理を実行するスレッド処理
 * @param thread スレッド
 */
static void runServerMainProc(CmnThread *thread)
{
	CMNLOG_TRACE_START();

	/* サーバー主処理の実行 */
	RunServerMainProcParam *param = (RunServerMainProcParam*)thread->data;
	param->serverMainProc(param->socket);

	/* ソケットのクローズ */
	closeSocket(param->socket);
	free(param->socket);
	free(param);

	CMNLOG_TRACE_END();
}

/**
 * @brief サーバーの待ち受け処理（Accept）を行う。
 * @param server サーバーオブジェクト]
 */
static void runAccept(CmnThread *thread)
{
	struct sockaddr_in clientAddr;
	int clientAddrLen;
	fd_set fdRead;
	struct timeval timeout;
	CmnNetSocketServer *server;

	CMNLOG_TRACE_START();

	server = (CmnNetSocketServer*)thread->data;

	clientAddrLen = sizeof(clientAddr);
	timeout.tv_sec = CMNNETSOCKET_ACCEPT_TIMEOUT_SEC;
	timeout.tv_usec = 0;

	while (1) {
		int selectRet;

		FD_ZERO(&fdRead);
		FD_SET(server->serverSocket->socketId, &fdRead);
		selectRet = select(server->serverSocket->socketId + 1, &fdRead, NULL, NULL, &timeout);

		/* タイムアウト */
		if (selectRet == 0) {
			/* 待ち受け終了をチェック */
			if ( ! server->_isRunnable) {
				break;
			}
		}
		/* 受信あり */
		else if (selectRet > 0) {
			CmnNetSocket *sock;
			CmnThread procThread;
			RunServerMainProcParam *param;

			sock = malloc(sizeof(CmnNetSocket));
			if (sock == NULL) {
				server->errorStatus = CNS_MEMORY_OVER_ERROR;
				break;
			}

			param = malloc(sizeof(RunServerMainProcParam));
			if (param == NULL) {
				free(sock);
				server->errorStatus = CNS_MEMORY_OVER_ERROR;
				break;
			}

			sock->socketId = accept(server->serverSocket->socketId, (struct sockaddr*) &clientAddr, &clientAddrLen);
			if (CmnNetSocket_isSocketError(sock->socketId)) {
				free(sock);
				free(param);
				server->errorStatus = CNS_ACCEPT_ERROR;
				break;
			}

			/* サーバー側主処理を別スレッドで呼び出し */
			param->serverMainProc = server->serverMainProc;
			param->socket = sock;
			CmnThread_Init(&procThread, runServerMainProc, param, NULL);
			CmnThread_Start(&procThread);
		}
		/* エラー */
		else {
			server->errorStatus = CNS_ACCEPT_ERROR;
			break;
		}
	}

	/* サーバーソケットのクローズ */
	closeSocket(server->serverSocket);
	releaseGlobalSocketResource();
	free(server->serverSocket);
	free(server->acceptThread);

	CMNLOG_TRACE_END();
}

/**
 * @brief TCP/IP待ち受けサーバー起動
 *
 *  portにLISTENするサーバー処理を別スレッドで開始する。<br>
 *  待ち受け開始後、portにアクセスがあればmethodを新規スレッドでコールする。
 *  待ち受けを終了する場合はCmnNetSocket_EndServerをコールすること。
 *  待ち受けスレッドはserver->acceptThreadに格納される。
 *
 * @param port 通信を待ち受けるポート
 * @param serverMainProc portにアクセスがあった場合に呼び出す処理
 * @param server サーバー起動に成功した場合にサーバー情報を設定する。呼び出し側での初期化は不要。
 * @return 処理結果ステータス
 */
CmnNetSocketStatus CmnNetSocket_StartServer(unsigned short port, void (*serverMainProc)(CmnNetSocket*), CmnNetSocketServer *server)
{
	#if IS_PRATFORM_WINDOWS()
		BOOL flagOn = 1;
	#else
		int flagOn = 1;
	#endif
	CmnNetSocket *serverSocket;
	CmnThread *acceptThread;
	struct sockaddr_in addr;

	CMNLOG_TRACE_START();

	serverSocket = calloc(1, sizeof(CmnNetSocket));
	if (serverSocket == NULL) {
		return CNS_MEMORY_OVER_ERROR;
	}
	allocateGlobalSocketResource();

	/* サーバーソケット生成 */
	serverSocket->socketId = socket(AF_INET, SOCK_STREAM, 0);
	if (CmnNetSocket_isSocketError(serverSocket->socketId)) {
		free(serverSocket);
		releaseGlobalSocketResource();
		CMNLOG_TRACE_END();
		return CNS_SERVER_INITIAL_ERROR;
	}

	/* アドレス設定 */
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	#if IS_PRATFORM_WINDOWS()
		addr.sin_addr.S_un.S_addr = INADDR_ANY;
	#else
		addr.sin_addr.s_addr = INADDR_ANY;
	#endif

	/* ソケットオプション設定 */
	setsockopt(serverSocket->socketId,
			SOL_SOCKET, SO_REUSEADDR, (const char*) &flagOn, sizeof(flagOn));

	/* ポートバインディング */
	if (bind(serverSocket->socketId, (struct sockaddr*) &addr, sizeof(addr)) != 0) {
		free(serverSocket);
		closeSocket(serverSocket);
		releaseGlobalSocketResource();
		CMNLOG_TRACE_END();
		return CNS_BIND_ERROR;
	}

	/* ポートListen */
	if (listen(serverSocket->socketId, 5) != 0) {
		free(serverSocket);
		closeSocket(serverSocket);
		releaseGlobalSocketResource();
		CMNLOG_TRACE_END();
		return CNS_LISTEN_ERROR;
	}

	/* 別スレッドで待ち受け（Accept）ループ開始 */
	acceptThread = malloc(sizeof(CmnThread));
	if (acceptThread == NULL) {
		free(serverSocket);
		closeSocket(serverSocket);
		releaseGlobalSocketResource();
		CMNLOG_TRACE_END();
		return CNS_MEMORY_OVER_ERROR;
	}
	server->serverSocket = serverSocket;
	server->serverMainProc = serverMainProc;
	server->acceptThread = acceptThread;
	server->_isRunnable = True;
	CmnThread_Init(acceptThread, runAccept, server, NULL);
	CmnThread_Start(acceptThread);

	CMNLOG_TRACE_END();
	return 0;
}

/**
 * @brief サーバーを終了する
 * @param server サーバーオブジェクト
 */
CmnNetSocketStatus CmnNetSocket_EndServer(CmnNetSocketServer *server)
{
	CMNLOG_TRACE_START();
	server->_isRunnable = False;
	return CNS_SUCCESS;
	CMNLOG_TRACE_END();
}

/**
 * @brief セッションレスTCP/IPリクエスト送信
 *
 *  セッションがなく、１回のRequest→Responseで完結するTCP/IPリクエストを送信する。<br>
 *
 * @param ip リモートホストのアドレス（IPアドレスまたはホスト名）
 * @param port リモートホストのポート
 * @param request リクエストデータ
 * @param response レスポンスデータを格納するバッファ
 * @param responseEndMark 受信データの終わりを示すバイト列。相手の送信終了（graceful close）まで無制限に読み込み続ける場合はNULLを指定する。
 * @param responseEndMarkLen endMarkのバイト数
 * @param opt ソケットオプション（CNS_OPT_XXX を論理和で指定する）
 * @return ステータス
 */
CmnNetSocketStatus CmnNetSocket_NoSessionRequest(
		const char *host,
		unsigned short port,
		CmnDataBuffer *request,
		CmnDataBuffer *response,
		const char *responseEndMark,
		const int responseEndMarkLen,
		int opt)
{
	struct sockaddr_in addr;
	CmnNetSocket cmnSocket;
	CmnNetSocketStatus status;

	CMNLOG_TRACE_START();
	allocateGlobalSocketResource();

	/* アドレス情報生成 */
	status = CmnNetSocket_ToSocketAddress(host, port, &addr);
	if (status != CNS_SUCCESS) {
		releaseGlobalSocketResource();
		CMNLOG_TRACE_END();
		return status;
	}

	/* ソケット生成 */
	cmnSocket.socketId = socket(AF_INET, SOCK_STREAM, 0);
	if (CmnNetSocket_isSocketError(cmnSocket.socketId)) {
		releaseGlobalSocketResource();
		CMNLOG_TRACE_END();
		return CNS_CLIENT_INITIAL_ERROR;
	}

	/* 接続 */
	if (connect(cmnSocket.socketId, (struct sockaddr *)&addr, sizeof(addr)) != 0) {
		closeSocket(&cmnSocket);
		releaseGlobalSocketResource();
		CMNLOG_TRACE_END();
		return CNS_CONNECT_ERROR;
	}

	/* リクエスト送信 */
	status = CmnNetSocket_SendAll(&cmnSocket, request->data, request->size);
	if (status != CNS_SUCCESS) {
		closeSocket(&cmnSocket);
		releaseGlobalSocketResource();
		CMNLOG_TRACE_END();
		return status;
	}

	/* graceful close */
	if (opt & CNS_OPT_GRACEFUL_CLOSE) {
		#if IS_PRATFORM_WINDOWS()
			shutdown(cmnSocket.socketId, SD_SEND);
		#else
			shutdown(cmnSocket.socketId, SHUT_WR);
		#endif
	}

	/* レスポンス受信 */
	status = CmnNetSocket_ReceiveAll(&cmnSocket, response, responseEndMark, responseEndMarkLen);
	if (status != CNS_SUCCESS) {
		closeSocket(&cmnSocket);
		releaseGlobalSocketResource();
		CMNLOG_TRACE_END();
		return status;
	}

	CMNLOG_TRACE_END();
	return CNS_SUCCESS;
}

/**
 * @brief ソケットからすべてのデータを読み込む
 *
 *  受信データにendMarkが現れる、もしくは相手が送信終了（FIN送信、graceful close）するまでブロックして受信データを全て読み込む。
 *
 * @param socket Socket
 * @param buf 受信したデータを格納するバッファ。endMarkも含めて全ての受信データを格納する。
 * @param endMark 受信データの終わりを示すバイト列。相手の送信終了（graceful close）まで無制限に読み込み続ける場合はNULLを指定する。
 * @param endMarkLen endMarkのバイト数
 * @return ステータス
 */
CmnNetSocketStatus CmnNetSocket_ReceiveAll(CmnNetSocket *socket, CmnDataBuffer *buf, const char *endMark, const int endMarkLen)
{
	char tmpBuf[CMNNETSOCKET_RECEIVE_BUFFER_SIZE];
	int recvRet;

	CMNLOG_TRACE_START();
	while (1) {
		recvRet = recv(socket->socketId, tmpBuf, CMNNETSOCKET_RECEIVE_BUFFER_SIZE, 0);
		/* 受信データあり */
		if (recvRet > 0) {
			unsigned char *p;
			CmnDataBuffer_Append(buf, tmpBuf, recvRet);
			p = buf->data;

			/* 終了マークの確認 */
			if (endMark != NULL && endMarkLen > 0
					&& memcmp(p + recvRet - endMarkLen, endMark, endMarkLen) == 0) {
				CMNLOG_TRACE_END();
				return CNS_SUCCESS;
			}
		}
		/* 相手の送信終了（graceful close） */
		else if (recvRet == 0) {
			CMNLOG_TRACE_END();
			return CNS_SUCCESS;
		}
		/* エラー発生 */
		else {
			CMNLOG_TRACE_END();
			return CNS_INPUT_ERROR;
		}
	}
	CMNLOG_TRACE_END();
}

/**
 * @brief ソケットへすべてのデータを書き込む
 * @param socket Socket
 * @param data 送信するデータ
 * @param len dataのバイト数
 * @return ステータス
 */
CmnNetSocketStatus CmnNetSocket_SendAll(CmnNetSocket *socket, const void *data, int len)
{
	CmnNetSocketStatus ret = CNS_SUCCESS;
	CMNLOG_TRACE_START();

	if (send(socket->socketId, data, len, 0) < 0) {
		ret = CNS_OUTPUT_ERROR;
	}

	CMNLOG_TRACE_END();
	return ret;
}

/**
 * @brief IPアドレスやホスト名文字列とポート番号からsockaddr_inを設定する
 * @param host IPアドレスまたはホスト名の文字列
 * @param port ポート番号
 * @param addr 変換したIPアドレス情報を格納する
 * @return ステータス（正常時：CNS_SUCCESS、変換エラー：CNS_ADDRESS_ERROR）
 */
CmnNetSocketStatus CmnNetSocket_ToSocketAddress(const char *host, unsigned short port, struct sockaddr_in *addr)
{
	unsigned long sAddr;
	CMNLOG_TRACE_START();

	addr->sin_family = AF_INET;
	addr->sin_port = htons(port);

	/* IPアドレスに変換 */
	sAddr = inet_addr(host);
	#if IS_PRATFORM_WINDOWS()
		addr->sin_addr.S_un.S_addr = sAddr;
	#else
		addr->sin_addr.s_addr = sAddr;
	#endif

	/* 単純変換に失敗した場合は名前解決によりIPアドレスに変換 */
	if (sAddr == 0xffffffff) {
		struct hostent *hostInfo;

		hostInfo = gethostbyname(host);
		if (hostInfo == NULL) {
			CMNLOG_TRACE_END();
			return CNS_ADDRESS_ERROR;
		}

		#if IS_PRATFORM_WINDOWS()
			addr->sin_addr.S_un.S_addr = *(unsigned int *)hostInfo->h_addr_list[0];
		#else
			addr->sin_addr.s_addr = *(unsigned int *)hostInfo->h_addr_list[0];
		#endif
	}

	CMNLOG_TRACE_END();
	return CNS_SUCCESS;
}


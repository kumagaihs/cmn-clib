/** @file *********************************************************************
 * @brief ネットワークライブラリ TCP/IPソケット関連機能
 * @author H.Kumagai
 *****************************************************************************/

#include "cmnclib/CommonNet.h"
#include "cmnclib/CommonData.h"

/**
 * @brief TCP/IP待ち受けサーバー起動
 *
 *  特定ポートにLISTENするサーバー処理を開始する。<br>
 *  portで待ち受けを開始し、アクセスがあればmethodを新規スレッドでコールする。
 *  待ち受けを終了する場合は。。。 TODO:検討すること。
 *
 * @param port 通信を待ち受けるポート
 * @param method portにアクセスがあった場合に呼び出す処理
 */
void CmnNetSocket_StartSimpleServer(unsigned short port, void (*method)(CmnNetSocket*))
{
	/* TODO:Sample_WinLin.cppを参考にすること。 */
}

/**
 * @brief セッションレスTCP/IPリクエスト送信
 *
 *  セッションがなく、１回のRequest→Responseで完結するTCP/IPリクエストを送信する。<br>
 *
 * @param ip リモートホストのIPアドレス
 * @param port リモートホストのポート
 * @param requestData リクエストデータ
 * @return レスポンスデータ
 */
CmnDataBuffer* CmnNetSocket_NoSessionRequest(const char *ip, unsigned short port, CmnDataBuffer *requestData)
{
	/* TODO:実装 */
	return NULL;
}




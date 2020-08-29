/** @file *********************************************************************
 * @brief ネットワークライブラリ HTTP関連機能
 * @author H.Kumagai
 *****************************************************************************/

#include "cmnclib/CommonNet.h"
#include "cmnclib/CommonData.h"

/**
 * @brief HTTPのGETリクエストを送信する
 * @param ip リモートホストのIPアドレス
 * @param port リモートホストのポート
 * @param path リクエストパス
 * @return レスポンスデータ
 */
CmnNetHttpResponse* CmnNetHttp_GetRequest(const char *ip, unsigned short port, const char *path)
{
	/* TODO:ヘッダー情報も指定可能な様に機能追加 */
	/* TODO:実装 */
	return NULL;
}

/**
 * @brief HTTPのPOSTリクエストを送信する
 * @param ip リモートホストのIPアドレス
 * @param port リモートホストのポート
 * @param path リクエストパス
 * @param requestBody リクエストボディに設定するデータ
 * @return レスポンスデータ
 */
CmnNetHttpResponse* CmnNetHttp_PostRequest(const char *ip, unsigned short port, const char *path, CmnDataBuffer *requestBody)
{
	/* TODO:ヘッダー情報も指定可能な様に機能追加 */
	/* TODO:実装 */
	return NULL;
}




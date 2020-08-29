/** @file *********************************************************************
 * @brief スレッドライブラリ TCP/IPソケット関連機能
 * @author H.Kumagai
 *****************************************************************************/

#include "cmnclib/Common.h"
#include "cmnclib/CommonThread.h"

#if IS_PRATFORM_WINDOWS()
	#include <windows.h>
	#include <process.h>
#else
	#include <pthread.h>
#endif

#if IS_PRATFORM_WINDOWS()
/* 呼出規約「__stdcall」を吸収するラッパ */
static unsigned int __stdcall callMethodForWin(void *arg) {
	CmnThread *thread = (CmnThread *)arg;
	thread->method(thread);
	return 0;
}
#endif

/**
 * @brief スレッドオブジェクト初期化
 *
 *  スレッドオブジェクトを初期化する。
 *  スレッド処理の最初に行うこと。
 *
 * @param thread スレッドオブジェクト
 * @param method 別スレッドで実行する処理
 * @param data CmnThread.dataに設定するオブジェクト。methodへのデータ受け渡しに使用する。使用しない場合はNULLを設定。
 * @param mutex mutexによる排他制御を行う場合に設定。mutexを使用しない場合はNULLを設定。
 */
void CmnThread_Init(CmnThread *thread, void (*method)(CmnThread*), void *data, CmnThreadMutex *mutex)
{
	thread->method = method;
	thread->data = data;
	thread->mutex = mutex;
}

/**
 * @brief スレッド処理開始
 *
 *  スレッド処理を開始する。
 *
 * @param thread スレッドオブジェクト
 * @return 0:正常、0以上:スレッドの開始に失敗した場合
 */
int CmnThread_Start(CmnThread *thread)
{
#if IS_PRATFORM_WINDOWS()
	unsigned int dummy;
	HANDLE tmpThreadId;

	/*
	 * Memo : CreateThreadと_beginthreadexの違い
	 * 　_beginthreadexはCreateThreadのラッパーで、
	 * 　スレッドセーフではないC標準ライブラリを安全に動かすために、スレッド起動時に専用のグローバルメモリを確保するもの。
	 * 　C標準ライブラリはどこで使用しているかわからないので、安全のために_beginthreadexを使用する。
	 */
	/* スレッド作成＆起動 */
	tmpThreadId = (HANDLE)_beginthreadex(
			NULL,				/* SECURITY_ATTRIBUTES構造体 */
			0,					/* スタックサイズ( 0は呼び出し側と同じサイズ ) */
			callMethodForWin,	/* スレッド関数 */
			thread,				/* スレッド関数への引数 */
			0,					/* 作成オプション( 0 / CREATE_SUSPENDED ) */
			&dummy				/* スレッドId */
	);

	if (tmpThreadId == 0) {
		/* 起動エラー */
		return -1;
	}

	thread->threadId = tmpThreadId;
	return 0;
#else
	pthread_t tmpThreadId;
	if (pthread_create(
					&(thread->threadId),		/* スレッドIDを格納するポインタ */
					NULL,						/* オプション指定 */
					thread->method,				/* スレッド関数 */
					thread)						/* スレッド関数への引数 */
			!= 0) {

		/* 起動エラー */
		return -1;
	}
	return 0;
#endif
}

/**
 * @brief スレッド処理待ち合わせ
 *
 *  対象のスレッド処理が完了するまでwaitする
 *
 * @param thread スレッドオブジェクト
 */
void CmnThread_Join(CmnThread *thread)
{
#if IS_PRATFORM_WINDOWS()
	WaitForSingleObject(thread->threadId, INFINITE);
#else
	pthread_join(thread->threadId, NULL);
#endif
}

/**
 * @brief スレッド強制終了
 *
 *  対象のスレッドを強制終了する
 *
 * @param thread スレッドオブジェクト
 */
void CmnThread_Kill(CmnThread *thread)
{
#if IS_PRATFORM_WINDOWS()
	TerminateThread(thread->threadId, 0);
#else
	pthread_cancel(thread->threadId);
#endif
}

/**
 * @brief Mutex作成
 *
 *  スレッド間排他制御用のMutexを作成する
 *
 * @return Mutexオブジェクト
 */
CmnThreadMutex* CmnThreadMutex_Create()
{
	CmnThreadMutex tmp;
	CmnThreadMutex *ret;

#if IS_PRATFORM_WINDOWS()
	tmp.mutexId = CreateMutex(NULL, FALSE, "CmnThreadMutex");
	if (tmp.mutexId == 0) {
		return NULL;
	}
#else
	if (pthread_mutex_init(&(tmp.mutexId), NULL) != 0) {
		return NULL;
	}
#endif

	if ((ret = calloc(1, sizeof(CmnThreadMutex))) == NULL) {
		return NULL;
	}

	ret->mutexId = tmp.mutexId;
	return ret;
}

/**
 * @brief Mutexロック取得
 *
 *  Mutexロックを取得する
 *
 * @return Mutexオブジェクト
 */
void CmnThreadMutex_Lock(CmnThreadMutex *mutex)
{
#if IS_PRATFORM_WINDOWS()
	WaitForSingleObject(mutex->mutexId, INFINITE);
#else
	pthread_mutex_lock(&(mutex->mutexId));
#endif
}

/**
 * @brief Mutexロック解除
 *
 *  Mutexロックを解除する
 *
 * @return Mutexオブジェクト
 */
void CmnThreadMutex_UnLock(CmnThreadMutex *mutex)
{
#if IS_PRATFORM_WINDOWS()
	ReleaseMutex(mutex->mutexId);
#else
	pthread_mutex_unlock(&(mutex->mutexId));
#endif
}

/**
 * @brief Mutex破棄
 *
 *  Mutexを破棄する
 *
 * @return Mutexオブジェクト
 */
void CmnThreadMutex_Free(CmnThreadMutex *mutex)
{
#if IS_PRATFORM_WINDOWS()
	CloseHandle(mutex->mutexId);
#else
	pthread_mutex_destroy(&(mutex->mutexId));
#endif

	free(mutex);
}


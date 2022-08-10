/** @file *********************************************************************
 * @brief スレッドライブラリ ヘッダファイル
 *
 *  スレッドライブラリのヘッダファイル。<br>
 *
 * @author H.Kumagai
 * @date   2020-07-30
 *****************************************************************************/

#ifndef CMNCLIB_CMN_THREAD_H_
#define CMNCLIB_CMN_THREAD_H_

#include "cmnclib/Common.h"

#if IS_PRATFORM_WINDOWS()
	#include <windows.h>
	#include <process.h>
#else
	#include <pthread.h>
#endif

/** Mutexオブジェクト */
typedef struct tag_CmnThreadMutex {
#if IS_PRATFORM_WINDOWS()
	HANDLE mutexId;				/**< mutex id */
#else
	pthread_mutex_t mutexId;	/**< mutex id */
#endif

} CmnThreadMutex;

/** スレッドオブジェクト */
typedef struct tag_CmnThread {
#if IS_PRATFORM_WINDOWS()
	HANDLE threadId;		/**< スレッドID */
#else
	pthread_t threadId;		/**< スレッドID */
#endif
	void (*method)(struct tag_CmnThread*);		/**< スレッド処理 */
	void *data;				/**< スレッド処理用の任意のデータ */
	CmnThreadMutex *mutex;	/**< Mutexオブジェクト */
} CmnThread;

D_EXTERN void CmnThread_Init(CmnThread *thread, void (*method)(CmnThread*), void *data, CmnThreadMutex *mutex);
D_EXTERN int CmnThread_Start(CmnThread *thread);
D_EXTERN void CmnThread_Join(CmnThread *thread);
D_EXTERN void CmnThread_Kill(CmnThread *thread);

D_EXTERN CmnThreadMutex* CmnThreadMutex_Create();
D_EXTERN void CmnThreadMutex_Lock(CmnThreadMutex *mutex);
D_EXTERN void CmnThreadMutex_UnLock(CmnThreadMutex *mutex);
D_EXTERN void CmnThreadMutex_Free(CmnThreadMutex *mutex);

#endif /* CMNCLIB_CMN_THREAD_H_ */

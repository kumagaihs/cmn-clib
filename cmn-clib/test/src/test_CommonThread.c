/** @file
 * @brief CommonThreadライブラリの動作を確認するためのテストプログラム
 * @author H.Kumagai
 * @date 2020-07-30
 */
#include <stdio.h>
#include <stdlib.h>

#include "cmnclib/CommonTest.h"
#include "cmnclib/CommonThread.h"
#include "cmnclib/CommonString.h"
#include "cmnclib/CommonTime.h"

static void methodNormal(CmnThread *thread)
{
	int i = 0;
	int *dat = thread->data;
	for (; i < 1000000; i++) {
		(*dat)++;
	}
}

static void test_CmnThread_Normal(CmnTestCase *t)
{
	CmnThread thread;
	int data = 100;

	CmnThread_Init(&thread, methodNormal, &data, NULL);
	CmnThread_Start(&thread);
	CmnThread_Join(&thread);

	CmnTest_AssertNumber(t, __LINE__, data, 1000100);
}

static void methodMutex1(CmnThread *thread)
{
	int i = 0;
	CmnThreadMutex_Lock(thread->mutex);
	for (; i < 20; i++) {
		CmnTime_Sleep(10);
		CmnStringBuffer_Append(thread->data, "A");
	}
	CmnThreadMutex_UnLock(thread->mutex);
}

static void methodMutex2(CmnThread *thread)
{
	int i = 0;
	CmnThreadMutex_Lock(thread->mutex);
	for (; i < 20; i++) {
		CmnTime_Sleep(10);
		CmnStringBuffer_Append(thread->data, "B");
	}
	CmnThreadMutex_UnLock(thread->mutex);
}

static void test_CmnThread_Mutex(CmnTestCase *t)
{
	CmnThread thread1;
	CmnThread thread2;
	CmnThreadMutex *mutex;
	CmnStringBuffer *data;
	CmnStringBuffer *expected;

	data = CmnStringBuffer_Create("");
	expected = CmnStringBuffer_Create("AAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBB");

	mutex = CmnThreadMutex_Create();

	CmnThread_Init(&thread1, methodMutex1, data, mutex);
	CmnThread_Init(&thread2, methodMutex2, data, mutex);

	CmnThread_Start(&thread1);
	CmnThread_Start(&thread2);

	CmnThread_Join(&thread1);
	CmnThread_Join(&thread2);

	CmnThreadMutex_Free(mutex);

	CmnTest_AssertString(t, __LINE__, data->string, expected->string);
}

void test_CommonThread_AddCase(CmnTestPlan *plan)
{
	CmnTest_AddTestCaseEasy(plan, test_CmnThread_Normal);
	CmnTest_AddTestCaseEasy(plan, test_CmnThread_Mutex);
}

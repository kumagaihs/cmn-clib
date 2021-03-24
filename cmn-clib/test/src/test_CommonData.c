/** @file
 * @brief CommonDataライブラリの動作を確認するためのテストプログラム
 * @author H.Kumagai
 * @date 2020-05-07
 */
#include <stdio.h>
#include <stdlib.h>

#include "cmnclib/CommonTest.h"
#include "cmnclib/CommonData.h"

static void test_CmnDataBuffer_small(CmnTestCase *t)
{
	CmnDataBuffer *buf = CmnDataBuffer_Create(8);
	CmnTest_AssertNumber(t, __LINE__, buf->bufSize, 8);
	CmnTest_AssertNumber(t, __LINE__, buf->size, 0);

	CmnDataBuffer_Set(buf, "123456789A", 10);
	CmnTest_AssertData(t, __LINE__, buf->data, "123456789A", 10);
	CmnTest_AssertNumber(t, __LINE__, buf->bufSize, 10);
	CmnTest_AssertNumber(t, __LINE__, buf->size, 10);

	CmnDataBuffer_Append(buf, "12345", 5);
	CmnTest_AssertData(t, __LINE__, buf->data, "123456789A12345", 15);
	CmnTest_AssertNumber(t, __LINE__, buf->bufSize, 15);
	CmnTest_AssertNumber(t, __LINE__, buf->size, 15);

	CmnDataBuffer_Set(buf, "123456789B", 10);
	CmnTest_AssertData(t, __LINE__, buf->data, "123456789B", 10);
	CmnTest_AssertNumber(t, __LINE__, buf->bufSize, 15);
	CmnTest_AssertNumber(t, __LINE__, buf->size, 10);

	CmnDataBuffer_Append(buf, "123", 3);
	CmnTest_AssertData(t, __LINE__, buf->data, "123456789B123", 13);
	CmnTest_AssertNumber(t, __LINE__, buf->bufSize, 15);
	CmnTest_AssertNumber(t, __LINE__, buf->size, 13);

	CmnDataBuffer_Set(buf, "12345678", 8);
	CmnTest_AssertData(t, __LINE__, buf->data, "12345678", 8);
	CmnTest_AssertNumber(t, __LINE__, buf->bufSize, 10);
	CmnTest_AssertNumber(t, __LINE__, buf->size, 8);
}

static void test_CmnDataBuffer_large(CmnTestCase *t)
{
	int i = 0;
	char testData[10240];
	for (i = 0; i < 2046; i++) testData[i] = "A";

	CmnDataBuffer *buf = CmnDataBuffer_Create(2046);
	CmnTest_AssertNumber(t, __LINE__, buf->bufSize, 2046);
	CmnTest_AssertNumber(t, __LINE__, buf->size, 0);

	CmnDataBuffer_Set(buf, testData, 2046);
	CmnTest_AssertData(t, __LINE__, buf->data, testData, 2046);
	CmnTest_AssertNumber(t, __LINE__, buf->bufSize, 2046);
	CmnTest_AssertNumber(t, __LINE__, buf->size, 2046);

	testData[i++] = 'B';
	CmnDataBuffer_Append(buf, "B", 1);
	CmnTest_AssertData(t, __LINE__, buf->data, testData, 2047);
	CmnTest_AssertNumber(t, __LINE__, buf->bufSize, 2047);
	CmnTest_AssertNumber(t, __LINE__, buf->size, 2047);

	testData[i++] = 'C';
	CmnDataBuffer_Append(buf, "C", 1);
	CmnTest_AssertData(t, __LINE__, buf->data, testData, 2048);
	CmnTest_AssertNumber(t, __LINE__, buf->bufSize, 2048);
	CmnTest_AssertNumber(t, __LINE__, buf->size, 2048);

	testData[i++] = 'D';
	CmnDataBuffer_Append(buf, "D", 1);
	CmnTest_AssertData(t, __LINE__, buf->data, testData, 2049);
	CmnTest_AssertNumber(t, __LINE__, buf->bufSize, 4096);
	CmnTest_AssertNumber(t, __LINE__, buf->size, 2049);

	CmnDataBuffer_Append(buf, testData, 2049);
	CmnTest_AssertNumber(t, __LINE__, buf->bufSize, 8192);
	CmnTest_AssertNumber(t, __LINE__, buf->size, 4098);
}

void test_CommonData_AddCase(CmnTestPlan *plan)
{
	CmnTest_AddTestCaseEasy(plan, test_CmnDataBuffer_small);
	CmnTest_AddTestCaseEasy(plan, test_CmnDataBuffer_large);
}

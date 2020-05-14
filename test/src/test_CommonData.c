/** @file
 * @brief CommonDataライブラリの動作を確認するためのテストプログラム
 * @author H.Kumagai
 * @date 2020-05-07
 */
#include <stdio.h>
#include <stdlib.h>

#include "CommonTest.h"
#include "CommonData.h"

static void test_CmnDataBuffer_small(CmnTest_Case *t)
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

void test_CommonData_AddCase(CmnTest_Plan *plan)
{
	CmnTest_AddTestCaseEasy(plan, test_CmnDataBuffer_small);
}

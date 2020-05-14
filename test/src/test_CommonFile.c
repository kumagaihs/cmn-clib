/** @file
 * @brief CommonFileライブラリの動作を確認するためのテストプログラム
 * @author H.Kumagai
 * @date 2020-05-14
 */
#include <stdio.h>
#include <stdlib.h>

#include "CommonTest.h"
#include "CommonFile.h"
#include "CommonData.h"

static void test_CmnFile_ReadAll(CmnTest_Case *t)
{
	CmnDataBuffer *buf = CmnFile_ReadAll("test/resources/CmnFile/ReadAll.txt");

	CmnTest_AssertNumber(t, __LINE__, buf->size, 5010);
	CmnTest_AssertData(t, __LINE__, buf->data, "START", 5);
	CmnTest_AssertData(t, __LINE__, ((char*)buf->data) + buf->size - 3, "END", 3);

	CmnDataBuffer_Free(buf);
}

static void test_CmnFile_ReadAllText(CmnTest_Case *t)
{
	CmnDataBuffer *buf = CmnFile_ReadAll("test/resources/CmnFile/ReadAll.txt");
	char *txt = CmnFile_ReadAllText("test/resources/CmnFile/ReadAll.txt");

	/* 末尾に'\0'追加 */
	CmnDataBuffer_Append(buf, "", 1);

	CmnTest_AssertString(t, __LINE__, buf->data, txt);

	CmnDataBuffer_Free(buf);
	free(txt);
}

void test_CommonFile_AddCase(CmnTest_Plan *plan)
{
	CmnTest_AddTestCaseEasy(plan, test_CmnFile_ReadAll);
	CmnTest_AddTestCaseEasy(plan, test_CmnFile_ReadAllText);
}

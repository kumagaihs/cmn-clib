/** @file
 * @brief CommonFileライブラリの動作を確認するためのテストプログラム
 * @author H.Kumagai
 * @date 2020-05-14
 */
#include <stdio.h>
#include <stdlib.h>

#include "cmnclib/CommonTest.h"
#include "cmnclib/CommonFile.h"
#include "cmnclib/CommonData.h"

static void test_CmnFile_ReadAll(CmnTestCase *t)
{
	CmnDataBuffer *buf = CmnFile_ReadAll("test/resources/CmnFile/ReadAll.txt");

	CmnTest_AssertNumber(t, __LINE__, buf->size, 5010);
	CmnTest_AssertData(t, __LINE__, buf->data, "START", 5);
	CmnTest_AssertData(t, __LINE__, ((char*)buf->data) + buf->size - 3, "END", 3);

	CmnDataBuffer_Free(buf);
}

static void test_CmnFile_ReadAllText(CmnTestCase *t)
{
	CmnDataBuffer *buf = CmnFile_ReadAll("test/resources/CmnFile/ReadAll.txt");
	char *txt = CmnFile_ReadAllText("test/resources/CmnFile/ReadAll.txt");

	/* 末尾に'\0'追加 */
	CmnDataBuffer_Append(buf, "", 1);

	CmnTest_AssertString(t, __LINE__, buf->data, txt);

	CmnDataBuffer_Free(buf);
	free(txt);
}

static void test_CmnFile_List(CmnTestCase *t)
{
	int i;
	char buf[4096];
	CmnFileInfo *info;
	CmnDataList *list = CmnDataList_Create();
	CmnFile_List("test/resources/CmnFile/list", list);

	CmnTest_AssertNumber(t, __LINE__, list->size, 3);
	for (i = 0; i < list->size; i++) {
		info = CmnDataList_Get(list, i);
		printf("%s\n", CmnFileInfo_ToString(info, buf));
	}

	CmnDataList_Free(list, free);
}

void test_CommonFile_AddCase(CmnTestPlan *plan)
{
	CmnTest_AddTestCaseEasy(plan, test_CmnFile_ReadAll);
	CmnTest_AddTestCaseEasy(plan, test_CmnFile_ReadAllText);
	CmnTest_AddTestCaseEasy(plan, test_CmnFile_List);
}

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
	CmnDataBuffer *buf = CmnDataBuffer_Create(0);
	if (CmnFile_ReadAll("test/resources/CmnFile/ReadAll.txt", buf) == NULL) {
		CmnTest_AssertNG(t, __LINE__);
	}

	CmnTest_AssertNumber(t, __LINE__, buf->size, 5010);
	CmnTest_AssertData(t, __LINE__, buf->data, "START", 5);
	CmnTest_AssertData(t, __LINE__, ((char*)buf->data) + buf->size - 3, "END", 3);

	CmnDataBuffer_Free(buf);
}

static void test_CmnFile_ReadAllText(CmnTestCase *t)
{
	CmnDataBuffer *buf = CmnDataBuffer_Create(0);
	if (CmnFile_ReadAll("test/resources/CmnFile/ReadAll.txt", buf) == NULL) {
		CmnTest_AssertNG(t, __LINE__);
	}
	CmnStringBuffer *txt = CmnStringBuffer_Create("");
	if (CmnFile_ReadAllText("test/resources/CmnFile/ReadAll.txt", txt) == NULL) {
		CmnTest_AssertNG(t, __LINE__);
	}

	/* 末尾に'\0'追加 */
	CmnDataBuffer_Append(buf, "", 1);

	CmnTest_AssertString(t, __LINE__, buf->data, txt->string);

	CmnDataBuffer_Free(buf);
	free(txt);
}

static void test_CmnFile_List(CmnTestCase *t)
{
	int i;
	char buf[4096];
	CmnFileInfo *info;
	CmnDataList *list = CmnDataList_Create();

	/* 正常系 */
	CmnFile_List("test/resources/CmnFile/list", list, CHARSET_UTF8);
	CmnTest_AssertNumber(t, __LINE__, list->size, 3);
	for (i = 0; i < list->size; i++) {
		info = CmnDataList_Get(list, i);
		printf("%s\n", CmnFileInfo_ToString(info, buf));
	}

	/* 存在しないディレクトリ */
	if (CmnFile_List("aaa/bbb/ccc", list, CHARSET_UTF8) != NULL) {
		CmnTest_AssertNG(t, __LINE__);
	}

	CmnDataList_Free(list, free);
}

static void test_CmnFile_Exists(CmnTestCase *t)
{
	CmnTest_AssertNumber(t, __LINE__, CmnFile_Exists("test/resources/CmnFile/ReadAll.txt"), True);
	CmnTest_AssertNumber(t, __LINE__, CmnFile_Exists("test/resources/CmnFile/ReadAll"), False);
	CmnTest_AssertNumber(t, __LINE__, CmnFile_Exists("test/resources/CmnFile"), True);
}

static void test_CmnFile_GetFileInfo(CmnTestCase *t)
{
	CmnFileInfo info;
	char buf[4096];

	/* 存在しない */
	if (CmnFile_GetFileInfo("xxx/xxx/xyz.txt", &info) != NULL) {
		CmnTest_AssertNG(t, __LINE__);
	}

	/* ファイル */
	if (CmnFile_GetFileInfo("test/resources/CmnFile/ReadAll.txt", &info) == NULL) {
		CmnTest_AssertNG(t, __LINE__);
	}
	else {
		CmnTest_AssertString(t, __LINE__, info.parentDir, "test/resources/CmnFile");
		CmnTest_AssertString(t, __LINE__, info.name, "ReadAll.txt");
		CmnTest_AssertNumber(t, __LINE__, info.isDirectory, 0);
		CmnTest_AssertNumber(t, __LINE__, info.isFile, 1);
		CmnTest_AssertNumber(t, __LINE__, info.size, 5010);
		printf("%s\n", CmnFileInfo_ToString(&info, buf));
	}

	/* ディレクトリ */
	if (CmnFile_GetFileInfo("test", &info) == NULL) {
		CmnTest_AssertNG(t, __LINE__);
	}
	else {
		CmnTest_AssertString(t, __LINE__, info.parentDir, ".");
		CmnTest_AssertString(t, __LINE__, info.name, "test");
		CmnTest_AssertNumber(t, __LINE__, info.isDirectory, 1);
		CmnTest_AssertNumber(t, __LINE__, info.isFile, 0);
		CmnTest_AssertNumber(t, __LINE__, info.size, 0);
		printf("%s\n", CmnFileInfo_ToString(&info, buf));
	}
}

void test_CommonFile_AddCase(CmnTestPlan *plan)
{
	CmnTest_AddTestCaseEasy(plan, test_CmnFile_ReadAll);
	CmnTest_AddTestCaseEasy(plan, test_CmnFile_ReadAllText);
	/*TODO CmnFile_WriteNew*/
	/*TODO CmnFile_WriteHead*/
	/*TODO CmnFile_WriteTail*/
	CmnTest_AddTestCaseEasy(plan, test_CmnFile_List);
	CmnTest_AddTestCaseEasy(plan, test_CmnFile_Exists);
	CmnTest_AddTestCaseEasy(plan, test_CmnFile_GetFileInfo);
	/*TODO CmnFile_ToAbsolutePath*/
}

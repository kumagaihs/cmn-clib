/** @file
 * @brief CmnFileライブラリの動作を確認するためのテストプログラム
 * @author H.Kumagai
 * @date 2020-05-14
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmnclib/CmnTest.h"
#include "cmnclib/CmnFile.h"
#include "cmnclib/CmnData.h"

static void test_CmnFile_ReadAll(CmnTestCase *t)
{
	CmnDataBuffer *buf = CmnDataBuffer_Create(0);
	if (CmnFile_ReadAll("test/resources/CmnFile/ReadAll.txt", buf) == NULL) {
		CmnTest_AssertNG(t, __LINE__);
	}
#if IS_PRATFORM_WINDOWS()
	CmnTest_AssertNumber(t, __LINE__, buf->size, 5010);
#else
	CmnTest_AssertNumber(t, __LINE__, buf->size, 4959);
#endif
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

static void test_CmnFile_Write_AndRemove(CmnTestCase *t)
{
	char *file = "test/resources/CmnFile/WriteTest.txt";
	CmnDataBuffer *buf = CmnDataBuffer_Create(0);
	int ret = 0;

	/* ファイル書き込みテスト */
	ret += CmnFile_WriteNew(file, "ZZZ", 3);
	ret += CmnFile_WriteNew(file, " 123 ", 5);
	ret += CmnFile_WriteTail(file, "XYZ", 3);
	ret += CmnFile_WriteHead(file, "abc", 3);
	if (ret != 0) {
		CmnTest_AssertNG(t, __LINE__);
	}

	/* 書き込んだデータを読み出し */
	if (CmnFile_ReadAll(file, buf) == NULL) {
		CmnTest_AssertNG(t, __LINE__);
	}

	/* 検証 */
	CmnTest_AssertData(t, __LINE__, buf->data, "abc 123 XYZ", 11);

	/* バッファ開放とファイル削除 */
	CmnDataBuffer_Free(buf);
	CmnFile_Remove(file);
	if (CmnFile_Exists(file)) {
		CmnTest_AssertNG(t, __LINE__);
	}
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

static void test_CmnFile_ToAbsolutePath(CmnTestCase *t)
{
	char target[] = "test/resources/CmnFile/ReadAll.txt";
	char cnvDelimTarget[CMN_FILE_MAX_PATH];
	char absPath[CMN_FILE_MAX_PATH];
	char pwdAndTarget[CMN_FILE_MAX_PATH];

	/* フルパスを取得 */
	if (CmnFile_ToAbsolutePath(target, absPath, ARRAY_LENGTH(absPath), CHARSET_UTF8) == NULL) {
		CmnTest_AssertNG(t, __LINE__);
	}

	/* カレントディレクトリを取得 */
	if (CmnFile_GetCurrentDirectory(pwdAndTarget, ARRAY_LENGTH(pwdAndTarget)) == NULL) {
		CmnTest_AssertNG(t, __LINE__);
	}

	/* ファイル名の/は環境の区切り文字に統一する */
	CmnString_Replace(target, "/", CMN_FILE_PATH_DELIMITER, cnvDelimTarget);

	/* 取得したフルパスをカレントディレクトリ＋相対パスで比較 */
	strcat(pwdAndTarget, CMN_FILE_PATH_DELIMITER);
	strcat(pwdAndTarget, cnvDelimTarget);
	CmnTest_AssertString(t, __LINE__, absPath, pwdAndTarget);
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
#if IS_PRATFORM_WINDOWS()
		CmnTest_AssertNumber(t, __LINE__, info.size, 5010);
#else
		CmnTest_AssertNumber(t, __LINE__, info.size, 4959);
#endif
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
		printf("%s\n", CmnFileInfo_ToString(&info, buf));
	}
}

void test_CmnFile_AddCase(CmnTestPlan *plan)
{
	CmnTest_AddTestCaseEasy(plan, test_CmnFile_ReadAll);
	CmnTest_AddTestCaseEasy(plan, test_CmnFile_ReadAllText);
	CmnTest_AddTestCaseEasy(plan, test_CmnFile_Write_AndRemove);
	CmnTest_AddTestCaseEasy(plan, test_CmnFile_List);
	CmnTest_AddTestCaseEasy(plan, test_CmnFile_ToAbsolutePath);
	CmnTest_AddTestCaseEasy(plan, test_CmnFile_Exists);
	CmnTest_AddTestCaseEasy(plan, test_CmnFile_GetFileInfo);
}

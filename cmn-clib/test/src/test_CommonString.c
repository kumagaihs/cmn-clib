/** @file
 * @brief CommonStringライブラリの動作を確認するためのテストプログラム
 * @author H.Kumagai
 * @date 2020-05-05
 */
#include <stdio.h>
#include <stdlib.h>

#include "cmnclib/CommonTest.h"
#include "cmnclib/CommonString.h"

static void test_CmnString_RTrim(CmnTestCase *t)
{
	char nonSpace[]    = "Hello world";
	char singleSpace[] = " Hello world ";
	char multiSpace[]  = "  Hello world  ";

	CmnTest_AssertString(t, __LINE__, CmnString_RTrim(nonSpace),    "Hello world");
	CmnTest_AssertString(t, __LINE__, CmnString_RTrim(singleSpace), " Hello world");
	CmnTest_AssertString(t, __LINE__, CmnString_RTrim(multiSpace),  "  Hello world");
}

static void test_CmnString_LTrim(CmnTestCase *t)
{
	char nonSpace[]    = "Hello world";
	char singleSpace[] = " Hello world ";
	char multiSpace[]  = "  Hello world  ";

	CmnTest_AssertString(t, __LINE__, CmnString_LTrim(nonSpace),    "Hello world");
	CmnTest_AssertString(t, __LINE__, CmnString_LTrim(singleSpace), "Hello world ");
	CmnTest_AssertString(t, __LINE__, CmnString_LTrim(multiSpace),  "Hello world  ");
}

static void test_CmnString_Trim(CmnTestCase *t)
{
	char nonSpace[]    = "Hello world";
	char singleSpace[] = " Hello world ";
	char multiSpace[]  = "  Hello world  ";

	/* Trim */
	CmnTest_AssertString(t, __LINE__, CmnString_Trim(nonSpace),    "Hello world");
	CmnTest_AssertString(t, __LINE__, CmnString_Trim(singleSpace), "Hello world");
	CmnTest_AssertString(t, __LINE__, CmnString_Trim(multiSpace),  "Hello world");
}

static void test_CmnString_Replace(CmnTestCase *t)
{
	char org[] = "hoge fuga fuga foo";
	char dest[64];

	/* 変換対象なし */
	CmnTest_AssertString(t, __LINE__, CmnString_Replace(org, "kuma", "tama", dest), org);
	/* 変換１つ */
	CmnTest_AssertString(t, __LINE__, CmnString_Replace(org, "hoge", "a", dest), "a fuga fuga foo");
	/* 変換２つ */
	CmnTest_AssertString(t, __LINE__, CmnString_Replace(org, "fuga", "AAAAAA", dest), "hoge AAAAAA AAAAAA foo");
	/* 変換連続２つ */
	CmnTest_AssertString(t, __LINE__, CmnString_Replace(org, "o", "EEE", dest), "hEEEge fuga fuga fEEEEEE");
}

static void test_CmnString_ReplaceNew(CmnTestCase *t)
{
	char org[] = "hoge fuga fuga foo";
	char *dest;

	/* 変換対象なし */
	CmnTest_AssertString(t, __LINE__, dest = CmnString_ReplaceNew(org, "kuma", "tama"), org);
	free(dest);
	/* 変換１つ */
	CmnTest_AssertString(t, __LINE__, dest = CmnString_ReplaceNew(org, "hoge", "a"), "a fuga fuga foo");
	free(dest);
	/* 変換２つ */
	CmnTest_AssertString(t, __LINE__, dest = CmnString_ReplaceNew(org, "fuga", "AAAAAA"), "hoge AAAAAA AAAAAA foo");
	free(dest);
	/* 変換連続２つ */
	CmnTest_AssertString(t, __LINE__, dest = CmnString_ReplaceNew(org, "o", "EEE"), "hEEEge fuga fuga fEEEEEE");
	free(dest);
}

static void test_CmnString_StrcatNew(CmnTestCase *t)
{
	char *dest;
	CmnTest_AssertString(t, __LINE__, dest = CmnString_StrCatNew("kuma", "tama"), "kumatama");
	free(dest);
}

static void test_CmnString_Split(CmnTestCase *t)
{

	/* 区切り文字なし */
	{
		char buf[10][100];
		int ret = CmnString_Split(&buf[0][0], 10, 100, " 123 456 789 ", ",");
		if (ret != 1) {
			CmnTest_AssertNG(t, __LINE__);
		}
		CmnTest_AssertString(t, __LINE__, buf[0], " 123 456 789 ");
	}

	/* 区切り１文字 */
	{
		char buf[10][100];
		int ret = CmnString_Split(&buf[0][0], 10, 100, " 123 456  789 ", " ");
		if (ret != 6) {
			CmnTest_AssertNG(t, __LINE__);
		}
		CmnTest_AssertString(t, __LINE__, buf[0], "");
		CmnTest_AssertString(t, __LINE__, buf[1], "123");
		CmnTest_AssertString(t, __LINE__, buf[2], "456");
		CmnTest_AssertString(t, __LINE__, buf[3], "");
		CmnTest_AssertString(t, __LINE__, buf[4], "789");
		CmnTest_AssertString(t, __LINE__, buf[5], "");
	}

	/* 区切り複数文字 */
	{
		char buf[10][100];
		int ret = CmnString_Split(&buf[0][0], 10, 100, "123<->456<-><->789<->", "<->");
		if (ret != 5) {
			CmnTest_AssertNG(t, __LINE__);
		}
		CmnTest_AssertString(t, __LINE__, buf[0], "123");
		CmnTest_AssertString(t, __LINE__, buf[1], "456");
		CmnTest_AssertString(t, __LINE__, buf[2], "");
		CmnTest_AssertString(t, __LINE__, buf[3], "789");
		CmnTest_AssertString(t, __LINE__, buf[4], "");
	}

}

static void test_CmnString_SplitAsList(CmnTestCase *t)
{

	/* 区切り文字なし */
	{
		CmnStringList *list = CmnStringList_Create();
		if (CmnString_SplitAsList(list, " 123 456 789 ", ",") == NULL) {
			CmnTest_AssertNG(t, __LINE__);
		}
		else if (list->size != 1) {
			CmnTest_AssertNG(t, __LINE__);
		}
		else {
			CmnTest_AssertString(t, __LINE__, CmnStringList_Get(list, 0), " 123 456 789 ");
		}
		CmnStringList_Free(list);
	}

	/* 区切り１文字 */
	{
		CmnStringList *list = CmnStringList_Create();
		if (CmnString_SplitAsList(list, " 123 456  789 ", " ") == NULL) {
			CmnTest_AssertNG(t, __LINE__);
		}
		else if (list->size != 6) {
			CmnTest_AssertNG(t, __LINE__);
		}
		else {
			CmnTest_AssertString(t, __LINE__, CmnStringList_Get(list, 0), "");
			CmnTest_AssertString(t, __LINE__, CmnStringList_Get(list, 1), "123");
			CmnTest_AssertString(t, __LINE__, CmnStringList_Get(list, 2), "456");
			CmnTest_AssertString(t, __LINE__, CmnStringList_Get(list, 3), "");
			CmnTest_AssertString(t, __LINE__, CmnStringList_Get(list, 4), "789");
			CmnTest_AssertString(t, __LINE__, CmnStringList_Get(list, 5), "");
		}
		CmnStringList_Free(list);
	}

	/* 区切り複数文字 */
	{
		CmnStringList *list = CmnStringList_Create();
		if (CmnString_SplitAsList(list, "123<->456<-><->789<->", "<->") == NULL) {
			CmnTest_AssertNG(t, __LINE__);
		}
		else if (list->size != 5) {
			CmnTest_AssertNG(t, __LINE__);
		}
		else {
			CmnTest_AssertString(t, __LINE__, CmnStringList_Get(list, 0), "123");
			CmnTest_AssertString(t, __LINE__, CmnStringList_Get(list, 1), "456");
			CmnTest_AssertString(t, __LINE__, CmnStringList_Get(list, 2), "");
			CmnTest_AssertString(t, __LINE__, CmnStringList_Get(list, 3), "789");
			CmnTest_AssertString(t, __LINE__, CmnStringList_Get(list, 4), "");
		}
		CmnStringList_Free(list);
	}

}

static void test_CmnString_Lpad(CmnTestCase *t)
{
	char buf[64];
	CmnTest_AssertString(t, __LINE__, CmnString_Lpad(buf, "1", '0', 0), "1");
	CmnTest_AssertString(t, __LINE__, CmnString_Lpad(buf, "1", '0', 1), "1");
	CmnTest_AssertString(t, __LINE__, CmnString_Lpad(buf, "1", '0', 2), "01");
	CmnTest_AssertString(t, __LINE__, CmnString_Lpad(buf, "1", '0', 3), "001");
	CmnTest_AssertString(t, __LINE__, CmnString_Lpad(buf, "001", '0', 4), "0001");
	CmnTest_AssertString(t, __LINE__, CmnString_Lpad(buf, "001", '0', 3), "001");
	CmnTest_AssertString(t, __LINE__, CmnString_Lpad(buf, "001", '0', 2), "001");
	CmnTest_AssertString(t, __LINE__, CmnString_Lpad(buf, "001", '0', 1), "001");
}

static void test_CmnString_Rpad(CmnTestCase *t)
{
	/* TODO テスト実施すること */
}

static void test_CmnString_StartWith(CmnTestCase *t)
{
	CmnTest_AssertNumber(t, __LINE__, CmnString_StartWith(".abc.txt", "."), 1);
	CmnTest_AssertNumber(t, __LINE__, CmnString_StartWith(".abc.txt", ".abc"), 1);
	CmnTest_AssertNumber(t, __LINE__, CmnString_StartWith(".abc.txt", ".abc.txt"), 1);
	CmnTest_AssertNumber(t, __LINE__, CmnString_StartWith(".abc.txt", "a"), 0);
	CmnTest_AssertNumber(t, __LINE__, CmnString_StartWith(".abc.txt", ".aa"), 0);
	CmnTest_AssertNumber(t, __LINE__, CmnString_StartWith(".abc.txt", "txt"), 0);
}

static void test_CmnString_EndWith(CmnTestCase *t)
{
	CmnTest_AssertNumber(t, __LINE__, CmnString_EndWith(".abc.txt", "t"), 1);
	CmnTest_AssertNumber(t, __LINE__, CmnString_EndWith(".abc.txt", ".txt"), 1);
	CmnTest_AssertNumber(t, __LINE__, CmnString_EndWith(".abc.txt", ".abc.txt"), 1);
	CmnTest_AssertNumber(t, __LINE__, CmnString_EndWith(".abc.txt", "x"), 0);
	CmnTest_AssertNumber(t, __LINE__, CmnString_EndWith(".abc.txt", ".tx"), 0);
	CmnTest_AssertNumber(t, __LINE__, CmnString_EndWith(".abc.txt", "txx"), 0);
}

static void test_CmnString_IndexOf(CmnTestCase *t)
{
	CmnTest_AssertNumber(t, __LINE__, CmnString_IndexOf("/dir/sub/file.txt", "/"), 0);
	CmnTest_AssertNumber(t, __LINE__, CmnString_IndexOf("/dir/sub/file.txt", "sub"), 5);
	CmnTest_AssertNumber(t, __LINE__, CmnString_IndexOf("/dir/sub/file.txz", "z"), 16);
	CmnTest_AssertNumber(t, __LINE__, CmnString_IndexOf("/dir/sub/file.txt", "_"), -1);
	CmnTest_AssertNumber(t, __LINE__, CmnString_IndexOf("/dir/sub/file.txt", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"), -1);
	CmnTest_AssertNumber(t, __LINE__, CmnString_IndexOf("/dir/sub/file.txt", "txta"), -1);
}

static void test_CmnString_LastIndexOf(CmnTestCase *t)
{
	CmnTest_AssertNumber(t, __LINE__, CmnString_LastIndexOf("/dir/sub/file.txt", "/"), 8);
	CmnTest_AssertNumber(t, __LINE__, CmnString_LastIndexOf("/dir/sub/file.txt", "sub"), 5);
	CmnTest_AssertNumber(t, __LINE__, CmnString_LastIndexOf("/dir/sub/file.txz", "z"), 16);
	CmnTest_AssertNumber(t, __LINE__, CmnString_LastIndexOf("Adir/sub/file.txz", "A"), 0);
	CmnTest_AssertNumber(t, __LINE__, CmnString_LastIndexOf("/dir/sub/file.txt", "_"), -1);
	CmnTest_AssertNumber(t, __LINE__, CmnString_LastIndexOf("/dir/sub/file.txt", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"), -1);
	CmnTest_AssertNumber(t, __LINE__, CmnString_LastIndexOf("/dir/sub/file.txt", "txta"), -1);
}

static void test_CmnString_List(CmnTestCase *t)
{
	CmnStringList *list = CmnStringList_Create();
	CmnTest_AssertNumber(t, __LINE__, list->size, 0);

	CmnStringList_Add(list, "One");
	CmnTest_AssertNumber(t, __LINE__, list->size, 1);
	CmnStringList_Add(list, "Two");
	CmnTest_AssertNumber(t, __LINE__, list->size, 2);
	CmnStringList_Add(list, "Three");
	CmnTest_AssertNumber(t, __LINE__, list->size, 3);

	CmnTest_AssertString(t, __LINE__, CmnStringList_Get(list, 0), "One");
	CmnTest_AssertString(t, __LINE__, CmnStringList_Get(list, 1), "Two");
	CmnTest_AssertString(t, __LINE__, CmnStringList_Get(list, 2), "Three");

	CmnStringList_Free(list);
}

static void test_CmnStringBuffer(CmnTestCase *t)
{
	CmnStringBuffer *buf = CmnStringBuffer_Create("");
	CmnTest_AssertString(t, __LINE__, buf->string, "");
	CmnTest_AssertNumber(t, __LINE__, buf->length, 0);

	CmnStringBuffer_Set(buf, "123456789A");
	CmnTest_AssertString(t, __LINE__, buf->string, "123456789A");
	CmnTest_AssertNumber(t, __LINE__, buf->length, 10);

	CmnStringBuffer_Append(buf, "12345");
	CmnTest_AssertString(t, __LINE__, buf->string, "123456789A12345");
	CmnTest_AssertNumber(t, __LINE__, buf->length, 15);

	CmnStringBuffer_Set(buf, "123456789B");
	CmnTest_AssertString(t, __LINE__, buf->string, "123456789B");
	CmnTest_AssertNumber(t, __LINE__, buf->length, 10);

	CmnStringBuffer_Append(buf, "123");
	CmnTest_AssertString(t, __LINE__, buf->string, "123456789B123");
	CmnTest_AssertNumber(t, __LINE__, buf->length, 13);

	CmnStringBuffer_Set(buf, "12345678");
	CmnTest_AssertString(t, __LINE__, buf->string, "12345678");
	CmnTest_AssertNumber(t, __LINE__, buf->length, 8);
}
void test_CommonString_AddCase(CmnTestPlan *plan)
{
	CmnTest_AddTestCaseEasy(plan, test_CmnString_RTrim);
	CmnTest_AddTestCaseEasy(plan, test_CmnString_LTrim);
	CmnTest_AddTestCaseEasy(plan, test_CmnString_Trim);
	CmnTest_AddTestCaseEasy(plan, test_CmnString_Replace);
	CmnTest_AddTestCaseEasy(plan, test_CmnString_ReplaceNew);
	CmnTest_AddTestCaseEasy(plan, test_CmnString_StrcatNew);
	CmnTest_AddTestCaseEasy(plan, test_CmnString_Split);
	CmnTest_AddTestCaseEasy(plan, test_CmnString_SplitAsList);
	CmnTest_AddTestCaseEasy(plan, test_CmnString_Lpad);
	CmnTest_AddTestCaseEasy(plan, test_CmnString_Rpad);
	CmnTest_AddTestCaseEasy(plan, test_CmnString_StartWith);
	CmnTest_AddTestCaseEasy(plan, test_CmnString_EndWith);
	CmnTest_AddTestCaseEasy(plan, test_CmnString_IndexOf);
	CmnTest_AddTestCaseEasy(plan, test_CmnString_LastIndexOf);
	CmnTest_AddTestCaseEasy(plan, test_CmnString_List);
	CmnTest_AddTestCaseEasy(plan, test_CmnStringBuffer);
}

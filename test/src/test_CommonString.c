/** @file
 * @brief CommonStringライブラリの動作を確認するためのテストプログラム
 * @author H.Kumagai
 * @date 2020-05-05
 */
#include <stdio.h>
#include <stdlib.h>

#include "CommonTest.h"
#include "CommonString.h"

static void test_CmnString_RTrim(CmnTest_Case *t)
{
	char nonSpace[]    = "Hello world";
	char singleSpace[] = " Hello world ";
	char multiSpace[]  = "  Hello world  ";

	CmnTest_AssertString(t, __LINE__, CmnString_RTrim(nonSpace),    "Hello world");
	CmnTest_AssertString(t, __LINE__, CmnString_RTrim(singleSpace), " Hello world");
	CmnTest_AssertString(t, __LINE__, CmnString_RTrim(multiSpace),  "  Hello world");
}

static void test_CmnString_LTrim(CmnTest_Case *t)
{
	char nonSpace[]    = "Hello world";
	char singleSpace[] = " Hello world ";
	char multiSpace[]  = "  Hello world  ";

	CmnTest_AssertString(t, __LINE__, CmnString_LTrim(nonSpace),    "Hello world");
	CmnTest_AssertString(t, __LINE__, CmnString_LTrim(singleSpace), "Hello world ");
	CmnTest_AssertString(t, __LINE__, CmnString_LTrim(multiSpace),  "Hello world  ");
}

static void test_CmnString_Trim(CmnTest_Case *t)
{
	char nonSpace[]    = "Hello world";
	char singleSpace[] = " Hello world ";
	char multiSpace[]  = "  Hello world  ";

	/* Trim */
	CmnTest_AssertString(t, __LINE__, CmnString_Trim(nonSpace),    "Hello world");
	CmnTest_AssertString(t, __LINE__, CmnString_Trim(singleSpace), "Hello world");
	CmnTest_AssertString(t, __LINE__, CmnString_Trim(multiSpace),  "Hello world");
}

static void test_CmnString_Replace(CmnTest_Case *t)
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

static void test_CmnString_ReplaceNew(CmnTest_Case *t)
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

static void test_CmnString_StrcatNew(CmnTest_Case *t)
{
	char *dest;
	CmnTest_AssertString(t, __LINE__, dest = CmnString_StrcatNew("kuma", "tama"), "kumatama");
	free(dest);
}

static void test_CmnString_Split(CmnTest_Case *t)
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

static void test_CmnString_Lpad(CmnTest_Case *t)
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

static void test_CmnString_List(CmnTest_Case *t)
{
	CmnString_List *list = CmnString_CreateList();
	CmnTest_AssertNumber(t, __LINE__, list->size, 0);

	CmnString_ListAddItem(list, "One");
	CmnTest_AssertNumber(t, __LINE__, list->size, 1);
	CmnString_ListAddItem(list, "Two");
	CmnTest_AssertNumber(t, __LINE__, list->size, 2);
	CmnString_ListAddItem(list, "Three");
	CmnTest_AssertNumber(t, __LINE__, list->size, 3);

	CmnTest_AssertString(t, __LINE__, CmnString_ListGetItem(list, 0), "One");
	CmnTest_AssertString(t, __LINE__, CmnString_ListGetItem(list, 1), "Two");
	CmnTest_AssertString(t, __LINE__, CmnString_ListGetItem(list, 2), "Three");

	CmnString_FreeList(list);
}

static void test_CmnStringBuffer(CmnTest_Case *t)
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
void test_CommonString_AddCase(CmnTest_Plan *plan)
{
	CmnTest_AddTestCaseEasy(plan, test_CmnString_RTrim);
	CmnTest_AddTestCaseEasy(plan, test_CmnString_LTrim);
	CmnTest_AddTestCaseEasy(plan, test_CmnString_Trim);
	CmnTest_AddTestCaseEasy(plan, test_CmnString_Replace);
	CmnTest_AddTestCaseEasy(plan, test_CmnString_ReplaceNew);
	CmnTest_AddTestCaseEasy(plan, test_CmnString_StrcatNew);
	CmnTest_AddTestCaseEasy(plan, test_CmnString_Split);
	CmnTest_AddTestCaseEasy(plan, test_CmnString_Lpad);
	CmnTest_AddTestCaseEasy(plan, test_CmnString_List);
	CmnTest_AddTestCaseEasy(plan, test_CmnStringBuffer);
}

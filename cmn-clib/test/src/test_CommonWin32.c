/** @file
 * @brief CommonWin32ライブラリの動作を確認するためのテストプログラム
 * @author H.Kumagai
 * @date 2020-09-27
 */

#include "cmnclib/CommonTest.h"
#include "cmnclib/CommonWin32.h"
#include "cmnclib/CommonString.h"

#if IS_PRATFORM_WINDOWS()

static void test_CmnWin32_Clipboard(CmnTestCase *t)
{
	CmnStringBuffer *setBuf;
	CmnStringBuffer *getBuf;
	int ret;

	setBuf = CmnStringBuffer_Create("TEST INPUT\n2ND LINE\tTAB\n日本語");
	getBuf = CmnStringBuffer_Create("");

	ret = CmnWin32Clipboard_SetString(setBuf);
	CmnTest_AssertNumber(t, __LINE__, ret, 0);

	ret = CmnWin32Clipboard_GetString(getBuf);
	CmnTest_AssertNumber(t, __LINE__, ret, 0);

	CmnTest_AssertString(t, __LINE__, getBuf->string, setBuf->string);
}

void test_CommonWin32_AddCase(CmnTestPlan *plan)
{
	CmnTest_AddTestCaseEasy(plan, test_CmnWin32_Clipboard);
}

#endif

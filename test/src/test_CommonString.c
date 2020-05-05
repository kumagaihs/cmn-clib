/** @file
 * @brief 共通ライブラリの動作を確認するためのテストプログラム
 * @author H.Kumagai
 * @date 2004-06-04
 * $Revision: 1.5 $
 */
#include <stdio.h>

#include "CommonTest.h"
#include "CommonString.h"

static void test_CmnString_RTrim(CmnTest_Case *testCase)
{
	char buf[] = "Hello   ";
	char *ret;
	ret = CmnString_RTrim(buf);

	CmnTest_AssertString(testCase, __LINE__, ret, "Helloa");
}

void test_CommonString_AddCase(CmnTest_Plan *plan)
{
	CmnTest_AddTestCaseEasy(plan, test_CmnString_RTrim);
}

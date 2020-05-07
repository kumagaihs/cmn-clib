/** @file
 * @brief CommonLogライブラリの動作を確認するためのテストプログラム
 * @author H.Kumagai
 * @date 2020-05-07
 */
#include <stdio.h>
#include <stdlib.h>

#include "CommonTest.h"
#include "CommonLog.h"

static void test_Xxx(CmnTest_Case *testCase)
{
	/* TODO */
}

void test_CommonLog_AddCase(CmnTest_Plan *plan)
{
	CmnTest_AddTestCaseEasy(plan, test_Xxx);
}

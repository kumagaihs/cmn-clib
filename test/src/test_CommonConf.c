/** @file
 * @brief CommonConfライブラリの動作を確認するためのテストプログラム
 * @author H.Kumagai
 * @date 2020-05-07
 */
#include <stdio.h>
#include <stdlib.h>

#include "CommonTest.h"
#include "CommonConf.h"

static void test_Xxx(CmnTest_Case *testCase)
{
	/* TODO */
}

void test_CommonConf_AddCase(CmnTest_Plan *plan)
{
	CmnTest_AddTestCaseEasy(plan, test_Xxx);
}

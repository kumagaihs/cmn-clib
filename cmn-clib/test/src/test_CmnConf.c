/** @file
 * @brief CmnConfライブラリの動作を確認するためのテストプログラム
 * @author H.Kumagai
 * @date 2020-05-07
 */
#include <stdio.h>
#include <stdlib.h>

#include "cmnclib/CmnTest.h"
#include "cmnclib/CmnConf.h"

static void test_Xxx(CmnTestCase *testCase)
{
	/* TODO */
}

void test_CmnConf_AddCase(CmnTestPlan *plan)
{
	CmnTest_AddTestCaseEasy(plan, test_Xxx);
}

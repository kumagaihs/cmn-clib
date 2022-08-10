/** @file
 * @brief CmnLogライブラリの動作を確認するためのテストプログラム
 * @author H.Kumagai
 * @date 2020-05-07
 */
#include <stdio.h>
#include <stdlib.h>

#include "cmnclib/CmnTest.h"
#include "cmnclib/CmnLog.h"

static void test_CmnLogEx(CmnTestCase *testCase)
{
	CmnLogEx* logex = CmnLogEx_Create("test.log", CMN_LOG_LEVEL_DEBUG, NULL);
	CmnLogEx_Put(logex, CMN_LOG_LEVEL_DEBUG, "test[%s] val[%s]", "AAA", "BBB");
}

void test_CmnLog_AddCase(CmnTestPlan *plan)
{
	CmnTest_AddTestCaseEasy(plan, test_CmnLogEx);
}

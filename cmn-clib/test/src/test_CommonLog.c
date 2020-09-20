/** @file
 * @brief CommonLogライブラリの動作を確認するためのテストプログラム
 * @author H.Kumagai
 * @date 2020-05-07
 */
#include <stdio.h>
#include <stdlib.h>

#include "cmnclib/CommonTest.h"
#include "cmnclib/CommonLog.h"

static void test_CmnLogEx(CmnTestCase *testCase)
{
	CmnLogEx* logex = CmnLogEx_Create("test.log", CMN_LOG_LEVEL_DEBUG, NULL);
	CmnLogEx_Put(logex, CMN_LOG_LEVEL_DEBUG, "test[%s] val[%s]", "AAA", "BBB");
}

void test_CommonLog_AddCase(CmnTestPlan *plan)
{
	CmnTest_AddTestCaseEasy(plan, test_CmnLogEx);
}

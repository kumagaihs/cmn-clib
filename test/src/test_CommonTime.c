/** @file
 * @brief CommonTimeライブラリの動作を確認するためのテストプログラム
 * @author H.Kumagai
 * @date 2020-05-07
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "CommonTest.h"
#include "CommonTime.h"

static void test_CmnTime_DateTimeSetNow(CmnTestCase *testCase)
{
	CmnTimeDateTime datetime;
	CmnTimeDateTime_SetNow(&datetime);

	/* 目視確認 */
	printf("time=%I64d, year=%d, month=%d, day(m)=%d, day(w)=%d, day(y)=%d, hour=%d, min=%d, sec=%d, isdst=%d, timezone=%ld\n",
			datetime.time,
			datetime.year,
			datetime.month,
			datetime.dayOfMonth,
			datetime.dayOfWeek,
			datetime.dayOfYear,
			datetime.hour,
			datetime.minute,
			datetime.second,
			datetime.isdst,
			datetime.timezone);
}

void test_CommonTime_AddCase(CmnTestPlan *plan)
{
	CmnTest_AddTestCaseEasy(plan, test_CmnTime_DateTimeSetNow);
}

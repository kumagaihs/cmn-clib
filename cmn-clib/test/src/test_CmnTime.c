/** @file
 * @brief CmnTimeライブラリの動作を確認するためのテストプログラム
 * @author H.Kumagai
 * @date 2020-05-07
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "cmnclib/CmnTest.h"
#include "cmnclib/CmnTime.h"

static void test_CmnTime_DateTimeSetNow(CmnTestCase *t)
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

static void test_CmnTime_DateTimeSetAndAdd(CmnTestCase *t)
{
	CmnTimeDateTime datetime;
	CmnTimeDateTime_Set(&datetime, 2020, 3, 1, 12, 20, 30, 0);
	CmnTest_AssertNumber(t, __LINE__, datetime.year, 2020);
	CmnTest_AssertNumber(t, __LINE__, datetime.month, 3);
	CmnTest_AssertNumber(t, __LINE__, datetime.dayOfMonth, 1);
	CmnTest_AssertNumber(t, __LINE__, datetime.dayOfWeek, 0);
	CmnTest_AssertNumber(t, __LINE__, datetime.dayOfYear, 60);
	CmnTest_AssertNumber(t, __LINE__, datetime.hour, 12);
	CmnTest_AssertNumber(t, __LINE__, datetime.minute, 20);
	CmnTest_AssertNumber(t, __LINE__, datetime.second, 30);

	CmnTimeDateTime_Add(&datetime, 1, 2, 3, 4, 5, 6);
	CmnTest_AssertNumber(t, __LINE__, datetime.year, 2021);
	CmnTest_AssertNumber(t, __LINE__, datetime.month, 5);
	CmnTest_AssertNumber(t, __LINE__, datetime.dayOfMonth, 4);
	CmnTest_AssertNumber(t, __LINE__, datetime.dayOfWeek, 2);
	CmnTest_AssertNumber(t, __LINE__, datetime.dayOfYear, 123);
	CmnTest_AssertNumber(t, __LINE__, datetime.hour, 16);
	CmnTest_AssertNumber(t, __LINE__, datetime.minute, 25);
	CmnTest_AssertNumber(t, __LINE__, datetime.second, 36);

	CmnTimeDateTime_Add(&datetime, 0, 25, 0, 0, 0, 0);
	CmnTest_AssertNumber(t, __LINE__, datetime.year, 2023);
	CmnTest_AssertNumber(t, __LINE__, datetime.month, 6);
	CmnTest_AssertNumber(t, __LINE__, datetime.dayOfMonth, 4);
	CmnTest_AssertNumber(t, __LINE__, datetime.dayOfWeek, 0);
	CmnTest_AssertNumber(t, __LINE__, datetime.dayOfYear, 154);
	CmnTest_AssertNumber(t, __LINE__, datetime.hour, 16);
	CmnTest_AssertNumber(t, __LINE__, datetime.minute, 25);
	CmnTest_AssertNumber(t, __LINE__, datetime.second, 36);
}

void test_CmnTime_AddCase(CmnTestPlan *plan)
{
	CmnTest_AddTestCaseEasy(plan, test_CmnTime_DateTimeSetNow);
	CmnTest_AddTestCaseEasy(plan, test_CmnTime_DateTimeSetAndAdd);
}

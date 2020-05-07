/** @file *********************************************************************
 * @brief ユニットテストライブラリ
 *
 *  ユニットテストライブラリ。<br>
 *  <br>
 *  ＜使用例＞<br>
 *  <pre>
 *  # テストプランの作成
 *  CmnTest_Plan plan;
 *  CmnTest_InitializeTestPlan(&plan);
 *
 *  # テストケースの追加
 *  CmnTest_AddTestCaseEasy(&plan, test_1);
 *  CmnTest_AddTestCaseEasy(&plan, test_2);
 *
 *  # テスト実行
 *  CmnTest_Run(&plan, False);
 *
 *  # テスト結果表示
 *  puts(plan.report);
 *
 *  # テスト破棄
 *  CmnTest_DestroyTest(&plan);
 *
 *  # テストメソッド（test_1）
 *  void test_1(CmnTest_Case *testCase) {
 *    # テスト対象処理の実行
 *    char buf[32];
 *    sprintf(buf, "%s %s", "Hello", "world");
 *
 *    # 検証
 *    CmnTest_AssertString(case, __LINE__, buf, "Hello world");
 *  }
 *  # テストメソッド（test_2）
 *  void test_2(CmnTest_Case *testCase) {
 *    ・・・省略・・・
 *  }
 *  </pre>
 *
 * @author H.Kumagai
 *****************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "CommonTest.h"
#include "Common.h"
#include "CommonData.h"
#include "CommonTime.h"
#include "CommonString.h"

static const size_t REPORT_BUF_SIZE_OF_ONE_CALSE = 1024;

/**
 * @brief テストプランを作成する
 */
void CmnTest_InitializeTestPlan(CmnTest_Plan *plan)
{
	plan->caseList = CmnData_CreateList();
}

/**
 * @brief テストプランにテストケースを追加する
 *
 *  本関数にはラッパーマクロ「CmnTest_AddTestCaseEasy」が用意されている。
 *  ラッパーマクロを使用すると、fileName（__FILE__）とcaseName（testFunction関数名）が補完される。
 *
 * @param testPlan テストプラン
 * @param fileName テストケースファイル名
 * @param caseName テストケース名
 * @param testFunction テスト実施関数
 */
void CmnTest_AddTestCase(CmnTest_Plan *plan, char *fileName, char *caseName, void (*testFunction)())
{
	CmnTest_Case *testCase = calloc(1, sizeof(CmnTest_Case));
	testCase->testFileName = fileName;
	testCase->testCaseName = caseName;
	testCase->testFunction = testFunction;
	testCase->result = True;
	testCase->actual = NULL;
	testCase->expected = NULL;
	CmnData_ListAddItem(plan->caseList, testCase);
}

/**
 * @brief テストを実行する
 * @param plan テストプラン
 * @param realtimeReport テスト実行時にテスト結果を順次標準出力に出力するか（True:出力する、False:出力しない）
 */
void CmnTest_Run(CmnTest_Plan *plan, _Bool realtimeReport)
{
	CmnTest_Case *testCase;
	char reportTmp[REPORT_BUF_SIZE_OF_ONE_CALSE];
	int i = 0;

	/* レポートの初期化 */
	plan->report = calloc(plan->caseList->size * REPORT_BUF_SIZE_OF_ONE_CALSE, sizeof(char));

	/* テスト開始時間記録 */
	CmnTime_DateTimeSetNow(&plan->startTime);

	for (i = 0; i < plan->caseList->size; i++) {
		/* テスト実行 */
		testCase = CmnData_ListGetItem(plan->caseList, i);
		testCase->testFunction(testCase);

		/* テスト実行結果作成 */
		if (testCase->result) {
			/* Test OK */
			sprintf(reportTmp,
					"File[%-30s] Case[%-30s] OK\n",
					testCase->testFileName, testCase->testCaseName);
		}
		else {
			/* Test NG */
			sprintf(reportTmp,
					"File[%-30s] Case[%-30s] NG\n"
					"    +- Line    %ld\n"
					"    +- Expected[%s]\n"
					"    +- Actual  [%s]\n",
					testCase->testFileName, testCase->testCaseName,
					testCase->lineOfNg,
					testCase->expected,
					testCase->actual);
		}
		strcat(plan->report, reportTmp);

		/* テスト実行結果出力 */
		if (realtimeReport) {
			printf("%s", reportTmp);
		}
	}

	/* テスト終了時間記録 */
	CmnTime_DateTimeSetNow(&plan->endTime);
}

/**
 * @brief テストプランを破棄する
 * @param plan テストプラン
 */
void CmnTest_DestroyTest(CmnTest_Plan *plan)
{
	int i;
	CmnTest_Case *testCase;

	/* 実測値、期待値を解放 */
	for (i = 0; i < plan->caseList->size; i++) {
		testCase = CmnData_ListGetItem(plan->caseList, i);
		free(testCase->actual);
		free(testCase->expected);
	}

	/* リストの解放 */
	CmnData_FreeList(plan->caseList, free);

	/* レポートの解放 */
	free(plan->report);
}

/*
 * @brief 整数値を検証する
 * @param testCase テストケース
 * @param line 検証処理を記述している行番号（__LINE__を指定する）
 * @param actual 実測値
 * @param expected 期待値
 * @return 検証OK：True、検証NG：False
 */
_Bool CmnTest_AssertNumber(CmnTest_Case *testCase, long line, long actual, long expected)
{
	/* すでに検証NGの場合は検証しない */
	if ( ! testCase->result) {
		return False;
	}

	if (actual != expected) {
		char buf[64];
		testCase->result = False;
		testCase->lineOfNg = line;
		sprintf(buf, "%ld", actual);
		testCase->actual = CmnString_StrcatNew(buf, "");
		sprintf(buf, "%ld", expected);
		testCase->expected = CmnString_StrcatNew(buf, "");
		return False;
	}

	return True;
}

/*
 * @brief 文字列を検証する
 * @param testCase テストケース
 * @param line 検証処理を記述している行番号（__LINE__を指定する）
 * @param actual 実測値
 * @param expected 期待値
 * @return 検証OK：True、検証NG：False
 */
_Bool CmnTest_AssertString(CmnTest_Case *testCase, long line, char *actual, char *expected)
{
	/* すでに検証NGの場合は検証しない */
	if ( ! testCase->result) {
		return False;
	}

	if (strcmp(actual, expected) != 0) {
		testCase->result = False;
		testCase->lineOfNg = line;
		testCase->actual = CmnString_StrcatNew(actual, "");
		testCase->expected = CmnString_StrcatNew(expected, "");
		return False;
	}

	return True;
}

/*===========================================
 * TODO : 共通関数化すること　ここから
 *==========================================*/
/** HEX変換に使用する文字群 */
static const char hex_chars[] = "0123456789ABCDEF";
/**
 * @brief 8bitデータをHEX形式の文字列に変換する.
 * @param buf    (0)   変換後のHEX文字列を格納するバッファ。<br>
 *   最低でも2byteの領域を有すること。
 * @param dat    (I)   変換対象のデータ
 * @author N.Ogasawara
 */
void tohex8(char *buf, unsigned char dat)
{
	int mask = 0x0000000f;
	int i;
	for (i = 0; i < 2; i++) {
		buf[1 - i] = hex_chars[(dat >> (i * 4)) & mask];
	}
}
char* toHexString(void *data, size_t len)
{
	int i;
	char *ret;
	char *chData = data;
	ret = malloc((len * 3) + 1);
	for (i = 0; i < len; i++) {
		tohex8(&ret[i * 3], chData[i]);
		ret[(i * 3) + 2] = ' ';
	}
	ret[len * 2] = '\0';
	return ret;
}
/*===========================================
 * TODO : 共通関数化すること　ここまで
 *==========================================*/

/*
 * @brief 任意の型のデータを検証する
 * @param testCase テストケース
 * @param line 検証処理を記述している行番号（__LINE__を指定する）
 * @param actual 実測値
 * @param expected 期待値
 * @return 検証OK：True、検証NG：False
 */
_Bool CmnTest_AssertData(CmnTest_Case *testCase, long line, void *actual, void *expected, size_t dataLen)
{
	/* すでに検証NGの場合は検証しない */
	if ( ! testCase->result) {
		return False;
	}

	if (memcmp(actual, expected, dataLen) != 0) {
		testCase->result = False;
		testCase->lineOfNg = line;
		testCase->actual = toHexString(actual, dataLen);
		testCase->expected = toHexString(expected, dataLen);
		return False;
	}
	testCase->result = False;
	return False;
}

/*
 * @brief 検証NGを記録する
 * @param testCase テストケース
 * @param line 検証処理を記述している行番号（__LINE__を指定する）
 * @return 常にFalse
 */
_Bool CmnTest_AssertNG(CmnTest_Case *testCase, long line)
{
	/* すでに検証NGの場合は検証しない */
	if ( ! testCase->result) {
		return False;
	}

	testCase->result = False;
	testCase->lineOfNg = line;
	testCase->actual = CmnString_StrcatNew("no data", "");
	testCase->expected = CmnString_StrcatNew("no data", "");
	return False;
}


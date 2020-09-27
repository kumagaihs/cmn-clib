/** @file *********************************************************************
 * @brief ユニットテストライブラリ ヘッダファイル
 *
 *  ユニットテストライブラリのヘッダファイル。<br>
 *  ユニットテストのソースコードからのみこのヘッダファイルを読み込むこと
 *
 * @author H.Kumagai
 * @date   2020-05-05
 *****************************************************************************/

#ifndef _COMMON_TEST_H
#define _COMMON_TEST_H

#include "cmnclib/Common.h"
#include "cmnclib/CommonData.h"
#include "cmnclib/CommonTime.h"

/** テストプラン */
typedef struct tag_CmnTestPlan {
	CmnDataList *caseList;			/**< テストケースのリスト */
	CmnTimeDateTime startTime;		/**< テスト開始日時 */
	CmnTimeDateTime endTime;		/**< テスト終了日時 */
	char *report;					/**< テスト実施結果 */
} CmnTestPlan;

/** テストケース */
typedef struct tag_CmnTestCase {
	char *testFileName;			/**< テストケースの書かれているソースファイル名 */
	char *testCaseName;			/**< テストケース名 */
	void (*testFunction)(struct tag_CmnTestCase*);		/**< テスト実行関数 */
	int result;				/**< テスト結果（True:OK、False:NG） */
	long lineOfNg;				/**< AssertNGとなった行番号 */
	char *expected;				/**< NG時の期待値 */
	char *actual;				/**< NG時の実測値 */
} CmnTestCase;

/** テストプランを作成する */
D_EXTERN void CmnTest_InitializeTestPlan(CmnTestPlan *plan);
/** テストプランにテストケースを追加する */
D_EXTERN void CmnTest_AddTestCase(CmnTestPlan *plan, char *fileName, char *caseName, void (*testFunction)(CmnTestCase*));
/** テストを実行する */
D_EXTERN void CmnTest_Run(CmnTestPlan *plan, int realtimeReport);
/** テストプランを破棄する */
D_EXTERN void CmnTest_DestroyTest(CmnTestPlan *plan);
/** 整数値検証 */
D_EXTERN int CmnTest_AssertNumber(CmnTestCase *testCase, long line, long long actual, long long expected);
/** 文字列検証 */
D_EXTERN int CmnTest_AssertString(CmnTestCase *testCase, long line, char *actual, char *expected);
/** データ検証 */
D_EXTERN int CmnTest_AssertData(CmnTestCase *testCase, long line, void *actual, void *expected, size_t dataLen);
/** 検証NGを記録する */
D_EXTERN int CmnTest_AssertNG(CmnTestCase *testCase, long line);

/** テストプランにテストケースを追加する（CmnTest_AddTestCaseのラッパーマクロ） */
#define CmnTest_AddTestCaseEasy(plan, testFunction) CmnTest_AddTestCase(plan, __FILE__, #testFunction, testFunction)

#endif /* _COMMON_TEST_H */

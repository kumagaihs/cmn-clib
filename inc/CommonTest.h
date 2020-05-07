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

#include "Common.h"
#include "CommonData.h"
#include "CommonTime.h"

/** テストプラン */
typedef struct tag_CmnTest_Plan {
	CmnData_List *caseList;			/**< テストケースのリスト */
	CmnTime_DateTime startTime;		/**< テスト開始日時 */
	CmnTime_DateTime endTime;		/**< テスト終了日時 */
	char *report;					/**< テスト実施結果 */
} CmnTest_Plan;

/** テストケース */
typedef struct tag_CmnTest_Case {
	char *testFileName;			/**< テストケースの書かれているソースファイル名 */
	char *testCaseName;			/**< テストケース名 */
	void (*testFunction)(struct tag_CmnTest_Case*);		/**< テスト実行関数 */
	_Bool result;				/**< テスト結果（True:OK、False:NG） */
	long lineOfNg;				/**< AssertNGとなった行番号 */
	char *expected;				/**< NG時の期待値 */
	char *actual;				/**< NG時の実測値 */
} CmnTest_Case;

/** テストプランを作成する */
D_EXTERN void CmnTest_InitializeTestPlan(CmnTest_Plan *plan);
/** テストプランにテストケースを追加する */
D_EXTERN void CmnTest_AddTestCase(CmnTest_Plan *plan, char *fileName, char *caseName, void (*testFunction)(CmnTest_Case*));
/** テストを実行する */
D_EXTERN void CmnTest_Run(CmnTest_Plan *plan, _Bool realtimeReport);
/** テストプランを破棄する */
D_EXTERN void CmnTest_DestroyTest(CmnTest_Plan *plan);
/** 整数値検証 */
D_EXTERN _Bool CmnTest_AssertNumber(CmnTest_Case *testCase, long line, long actual, long expected);
/** 文字列検証 */
D_EXTERN _Bool CmnTest_AssertString(CmnTest_Case *testCase, long line, char *actual, char *expected);
/** データ検証 */
D_EXTERN _Bool CmnTest_AssertData(CmnTest_Case *testCase, long line, void *actual, void *expected, size_t dataLen);
/** 検証NGを記録する */
D_EXTERN _Bool CmnTest_AssertNG(CmnTest_Case *testCase, long line);

/** テストプランにテストケースを追加する（CmnTest_AddTestCaseのラッパーマクロ） */
#define CmnTest_AddTestCaseEasy(plan, testFunction) CmnTest_AddTestCase(plan, __FILE__, #testFunction, testFunction)

#endif /* _COMMON_TEST_H */

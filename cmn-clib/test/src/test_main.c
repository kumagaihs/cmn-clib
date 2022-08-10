/** @file
 * @brief 共通ライブラリの動作を確認するためのテストプログラム
 * @author H.Kumagai
 * @date 2004-06-04
 * $Revision: 1.5 $
 */
#include<stdio.h>
#include<string.h>

#include"cmnclib/CmnTest.h"
#include"cmnclib/CmnLog.h"
#include"cmnclib/CmnTime.h"

extern void test_CmnConf_AddCase(CmnTestPlan *plan);
extern void test_CmnData_AddCase(CmnTestPlan *plan);
extern void test_CmnFile_AddCase(CmnTestPlan *plan);
extern void test_CmnLog_AddCase(CmnTestPlan *plan);
extern void test_CmnString_AddCase(CmnTestPlan *plan);
extern void test_CmnTime_AddCase(CmnTestPlan *plan);
extern void test_CmnThread_AddCase(CmnTestPlan *plan);
extern void test_CmnNet_AddCase(CmnTestPlan *plan);
#if IS_PRATFORM_WINDOWS()
  extern void test_CmnWin32_AddCase(CmnTestPlan *plan);
#endif

int main(int argc, char **argv)
{
	CmnTestPlan plan;
	CmnTimeDateTime now;
	char timeBuf[64];
	char logFileName[128] = "cmn-clib_test_";

	printf("### Start test ###\n");

	CmnTimeDateTime_SetNow(&now);
	CmnTime_Format(&now, CMN_TIME_FORMAT_ALL_SHORT, timeBuf);
	CmnLog_Init(strcat(strcat(logFileName, timeBuf), ".log"), CMN_LOG_LEVEL_TRACE, NULL);
	CmnLog_InitCmnClibLog(NULL, CMN_LOG_LEVEL_TRACE);

	CmnTest_InitializeTestPlan(&plan);

	/* CmnConf */
	test_CmnConf_AddCase(&plan);
	/* CmnData */
	test_CmnData_AddCase(&plan);
	/* CmnFile */
	test_CmnFile_AddCase(&plan);
	/* CmnLog */
	test_CmnLog_AddCase(&plan);
	/* CmnString */
	test_CmnString_AddCase(&plan);
	/* CmnTime */
	test_CmnTime_AddCase(&plan);
	/* CmnThread */
	test_CmnThread_AddCase(&plan);
	/* CmnNet */
	test_CmnNet_AddCase(&plan);
	/* CmnWin32 */
	#if IS_PRATFORM_WINDOWS()
	test_CmnWin32_AddCase(&plan);
	#endif

	CmnTest_Run(&plan, True);

	CmnTest_DestroyTest(&plan);

	CmnLog_End();
	CmnLog_EndCmnClibLog();

	printf("### End test ###\n");

	return 0;
}

//int test()
//{
//	char trimstr[] = "  a b c  ";
//	char str[256] = "";
//	CmnString_List *strList;
//	CmnLog_LogEx *logex;
//	CmnConf_PropertyList *prop;
//	CmnData_List *list;
//	char *p;
//	char buf[CMN_TIME_FORMAT_SIZE_ALL];
//
//	/* Common test */
//	if (IS_PRATFORM_WINDOWS()) {
//		printf("Pratform is windows\n");
//	}
//	else if (IS_PRATFORM_LINUX()) {
//		printf("Pratform is linux\n");
//	}
//
//	/* CmnTime test */
//	printf("%s\n", CmnTime_GetFormatTime(CMN_TIME_FORMAT_ALL, buf));
//
//	/* CmnString test */
//	printf("[%s]\n", CmnString_RTrim(trimstr));
//	printf("[%s]\n", CmnString_LTrim(trimstr));
//	printf("[%s]\n", CmnString_Trim(trimstr));
//
//	strList = CmnString_CreateList();
//	CmnString_ListAddItem(strList, "strだ！");
//	CmnString_ListAddItem(strList, strcat(str, "wahaha"));
//	printf("size=[%d], str=[%s]\n", strList->size, CmnString_ListGetItem(strList, 0));
//	printf("size=[%d], str=[%s]\n", strList->size, CmnString_ListGetItem(strList, 1));
//	CmnString_FreeList(strList);
//
//	/* CmnLog test */
//	if ( ! CmnLog_Init("test/conf/message.conf", CMN_LOG_LEVEL_DETAIL)) {
//		printf("標準ログ初期化失敗\n");
//		return 0;
//	}
//	CmnLog_Put(CMN_LOG_LEVEL_DETAIL, "TEST01", "です");
//	CmnLog_Put(CMN_LOG_LEVEL_STANDARD, "TEST02", 1, "です");
//	CmnLog_Put(CMN_LOG_LEVEL_DEBUG, "TEST01", "です");
//	CmnLog_End();
//
//	logex = CmnLog_InitEx("test/conf/message.conf", CMN_LOG_LEVEL_DETAIL, "test.log");
//	if (logex == NULL) {
//		printf("拡張ログ初期化失敗\n");
//		return 0;
//	}
//	CmnLog_PutEx(logex, CMN_LOG_LEVEL_DETAIL, "TEST01", "です");
//	CmnLog_PutEx(logex, CMN_LOG_LEVEL_STANDARD, "TEST02", 1, "です");
//	CmnLog_PutEx(logex, CMN_LOG_LEVEL_DEBUG, "TEST01", "です");
//	CmnLog_EndEx(logex);
//
//	/* CmnConf test */
//	prop = CmnConf_GetPropertyList("test/conf/property.conf");
//	if (prop == NULL) {
//		printf("プロパティファイル読み込み失敗\n");
//		return 0;
//	}
//	printf("TEST1 = %s\n", CmnConf_GetProperty(prop, "TEST1"));
//	printf("TEST2 = %s\n", CmnConf_GetProperty(prop, "TEST2"));
//	CmnConf_FreePropertyList(prop);
//
//	/* CmnData test */
//	list = CmnData_CreateList();
//	printf("size=[%d], data=[%s]\n", list->size, (char*)CmnData_ListGetItem(list, 0));
//
//	p = malloc(sizeof("data1"));
//	strcpy(p, "data1");
//	CmnData_ListAddItem(list, p);
//	printf("size=[%d], data=[%s]\n", list->size, (char*)CmnData_ListGetItem(list, 0));
//
//	p = malloc(sizeof("data2"));
//	strcpy(p, "data2");
//	CmnData_ListAddItem(list, p);
//	printf("size=[%d], data=[%s]\n", list->size, (char*)CmnData_ListGetItem(list, 1));
//
//	p = malloc(sizeof("data3"));
//	strcpy(p, "data3");
//	CmnData_ListAddItem(list, p);
//	printf("size=[%d], data=[%s]\n", list->size, (char*)CmnData_ListGetItem(list, 2));
//
//	p = malloc(sizeof("data4"));
//	strcpy(p, "data4");
//	CmnData_ListAddItem(list, p);
//	printf("size=[%d], data=[%s]\n", list->size, (char*)CmnData_ListGetItem(list, 3));
//
//	CmnData_FreeList(list, free);
//
//
//	list = CmnData_CreateList();
//	printf("size=[%d], data=[%s]\n", list->size, (char*)CmnData_ListGetItem(list, 0));
//	CmnData_ListAddItem(list, "string1");
//	printf("size=[%d], data=[%s]\n", list->size, (char*)CmnData_ListGetItem(list, 0));
//	CmnData_ListAddItem(list, "string2");
//	printf("size=[%d], data=[%s]\n", list->size, (char*)CmnData_ListGetItem(list, 1));
//	CmnData_ListAddItem(list, "string3");
//	printf("size=[%d], data=[%s]\n", list->size, (char*)CmnData_ListGetItem(list, 2));
//	CmnData_ListAddItem(list, "string4");
//	printf("size=[%d], data=[%s]\n", list->size, (char*)CmnData_ListGetItem(list, 3));
//
//	CmnData_FreeList(list, NULL);
//
//	printf("\n\\(^o^)/ SUCCESS!!\n\n");
//
//	return 0;
//}

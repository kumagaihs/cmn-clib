/** @file
 * @brief 共通ライブラリの動作を確認するためのテストプログラム
 * @author H.Kumagai
 * @date 2004-06-04
 * $Revision: 1.5 $
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"Common.h"
#include"CommonTime.h"
#include"CommonString.h"
#include"CommonLog.h"
#include"CommonConf.h"
#include"CommonData.h"

static int test();

int main(int argc, char **argv)
{
	printf("### TestLib テスト開始 ###\n");
	test();
	printf("### テスト終了 ###\n");
	getchar();
}

int test()
{
	char trimstr[] = "  a b c  ";
	char str[256] = "";
	CmnString_List *strList;
	CmnLog_LogEx *logex;
	CmnConf_PropertyList *prop;
	CmnData_List *list;
	char *p;

	/* CommonTime test */
	char buf[CMN_TIME_FORMAT_SIZE_ALL];
	printf("%s\n", CmnTime_GetFormatTime(CMN_TIME_FORMAT_ALL, buf));

	/* CommonString test */
	printf("[%s]\n", CmnString_RTrim(trimstr));
	printf("[%s]\n", CmnString_LTrim(trimstr));
	printf("[%s]\n", CmnString_Trim(trimstr));

	strList = CmnString_CreateList();
	CmnString_ListAddItem(strList, "strだ！");
	CmnString_ListAddItem(strList, strcat(str, "wahaha"));
	printf("size=[%d], str=[%s]\n", strList->size, CmnString_ListGetItem(strList, 0));
	printf("size=[%d], str=[%s]\n", strList->size, CmnString_ListGetItem(strList, 1));
	CmnString_FreeList(strList);

	/* CommonLog test */
	if ( ! CmnLog_Init("TestConf\\message.conf", CMN_LOG_LEVEL_DETAIL)) {
		printf("標準ログ初期化失敗\n");
		return 0;
	}
	CmnLog_Put(CMN_LOG_LEVEL_DETAIL, "TEST01", "です");
	CmnLog_Put(CMN_LOG_LEVEL_STANDARD, "TEST02", 1, "です");
	CmnLog_Put(CMN_LOG_LEVEL_DEBUG, "TEST01", "です");
	CmnLog_End();

	logex = CmnLog_InitEx("TestConf\\message.conf", CMN_LOG_LEVEL_DETAIL, "test.log");
	if (logex == NULL) {
		printf("拡張ログ初期化失敗\n");
		return 0;
	}
	CmnLog_PutEx(logex, CMN_LOG_LEVEL_DETAIL, "TEST01", "です");
	CmnLog_PutEx(logex, CMN_LOG_LEVEL_STANDARD, "TEST02", 1, "です");
	CmnLog_PutEx(logex, CMN_LOG_LEVEL_DEBUG, "TEST01", "です");
	CmnLog_EndEx(logex);

	/* CommonConf test */
	prop = CmnConf_GetPropertyList("TestConf\\property.conf");
	if (prop == NULL) {
		printf("プロパティファイル読み込み失敗\n");
		return 0;
	}
	printf("TEST1 = %s\n", CmnConf_GetProperty(prop, "TEST1"));
	printf("TEST2 = %s\n", CmnConf_GetProperty(prop, "TEST2"));
	CmnConf_FreePropertyList(prop);

	/* CommonData test */
	list = CmnData_CreateList();
	printf("size=[%d], data=[%s]\n", list->size, CmnData_ListGetItem(list, 0));

	p = malloc(sizeof("data1"));
	strcpy(p, "data1");
	CmnData_ListAddItem(list, p);
	printf("size=[%d], data=[%s]\n", list->size, CmnData_ListGetItem(list, 0));

	p = malloc(sizeof("data2"));
	strcpy(p, "data2");
	CmnData_ListAddItem(list, p);
	printf("size=[%d], data=[%s]\n", list->size, CmnData_ListGetItem(list, 1));

	p = malloc(sizeof("data3"));
	strcpy(p, "data3");
	CmnData_ListAddItem(list, p);
	printf("size=[%d], data=[%s]\n", list->size, CmnData_ListGetItem(list, 2));

	p = malloc(sizeof("data4"));
	strcpy(p, "data4");
	CmnData_ListAddItem(list, p);
	printf("size=[%d], data=[%s]\n", list->size, CmnData_ListGetItem(list, 3));

	CmnData_FreeList(list, free);


	list = CmnData_CreateList();
	printf("size=[%d], data=[%s]\n", list->size, CmnData_ListGetItem(list, 0));
	CmnData_ListAddItem(list, "string1");
	printf("size=[%d], data=[%s]\n", list->size, CmnData_ListGetItem(list, 0));
	CmnData_ListAddItem(list, "string2");
	printf("size=[%d], data=[%s]\n", list->size, CmnData_ListGetItem(list, 1));
	CmnData_ListAddItem(list, "string3");
	printf("size=[%d], data=[%s]\n", list->size, CmnData_ListGetItem(list, 2));
	CmnData_ListAddItem(list, "string4");
	printf("size=[%d], data=[%s]\n", list->size, CmnData_ListGetItem(list, 3));

	CmnData_FreeList(list, NULL);

	printf("\n\\(^o^)/ SUCCESS!!\n\n");

	return 0;
}

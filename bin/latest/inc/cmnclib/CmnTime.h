/** @file *********************************************************************
 * @brief 日付/時刻系 共通関数 I/Fヘッダファイル
 *
 *  日付/時刻系共通関数を使用するためのI/Fヘッダファイル。<br>
 *  日付/時刻系の共通関数を使用する場合は、このヘッダファイルを読み込むこと
 *
 * @author H.Kumagai
 * @date   2004-06-04
 * $Revision: 1.2 $
 *****************************************************************************/

#ifndef CMNCLIB_CMN_TIME_H
#define CMNCLIB_CMN_TIME_H

#include <time.h>

#include "cmnclib/Common.h"

/** 日付時刻情報 */
typedef struct tag_CmnTimeDateTime {
	time_t time;			/**< 1970/01/01 00:00:00 をゼロとした経過秒 */
	int year;				/**< 西暦年 */
	int month;				/**< 月(1 - 12) */
	int dayOfMonth;			/**< 日(1 - 31) */
	int dayOfWeek;			/**< 曜日(0 - 6) */
	int dayOfYear;			/**< 年通算日数(0 - 365) */
	int hour;				/**< 時(0 - 23) */
	int minute;				/**< 分(0 - 59) */
	int second;				/**< 秒(0 - 59) */
	int isdst;				/**< 夏時間（夏時間の場合は1、そうでなければ0、不明の場合は-1） */
	long timezone;			/**< タイムゾーン（UTCと現地時刻の差(秒単位)） */
} CmnTimeDateTime;

/** 現在時刻文字列のフォーマットタイプ */
typedef enum {
	CMN_TIME_FORMAT_ALL,			/**< 形式：yyyy/mm/dd hh:mm:ss */
	CMN_TIME_FORMAT_ALL_SHORT,		/**< 形式：yyyymmddhhmmss */
	CMN_TIME_FORMAT_DATE,			/**< 形式：yyyy/mm/dd */
	CMN_TIME_FORMAT_DATE_SHORT,		/**< 形式：yyyymmdd */
	CMN_TIME_FORMAT_TIME,			/**< 形式：hh:mm:ss */
	CMN_TIME_FORMAT_TIME_SHORT		/**< 形式：hhmmss */
} CmnTimeFormatType;

/** フォーマット後の日付/時刻文字列長 */
#define CMN_TIME_FORMAT_SIZE_ALL			(19 + 1)	/**< 形式：yyyy/mm/dd hh:mm:ss */
#define CMN_TIME_FORMAT_SIZE_ALL_SHORT 		(14 + 1)	/**< 形式：yyyymmddhhmmss */
#define CMN_TIME_FORMAT_SIZE_DATE			(10 + 1)	/**< 形式：yyyy/mm/dd */
#define CMN_TIME_FORMAT_SIZE_DATE_SHORT		( 8 + 1)	/**< 形式：yyyymmdd */
#define CMN_TIME_FORMAT_SIZE_TIME			( 8 + 1)	/**< 形式：hh:mm:ss */
#define CMN_TIME_FORMAT_SIZE_TIME_SHORT		( 6 + 1)	/**< 形式：hhmmss */

/** 時刻文字列のバッファ長 */
#define CMN_TIME_DATETIME_STRING_BUFFER_SIZE (160)

/* --- CmnTime.c --- */
/* 現在日時を取得する */
D_EXTERN CmnTimeDateTime* CmnTimeDateTime_SetNow(CmnTimeDateTime *datetime);
/* 日時情報を作成する */
D_EXTERN CmnTimeDateTime* CmnTimeDateTime_Set(CmnTimeDateTime *datetime, int year, int month, int day, int hour, int minute, int second, int isdst);
/* 日時情報を作成する */
D_EXTERN CmnTimeDateTime* CmnTimeDateTime_SetBySerial(CmnTimeDateTime *datetime, time_t time);
/* 日時を加算する */
D_EXTERN CmnTimeDateTime* CmnTimeDateTime_Add(CmnTimeDateTime *datetime, int year, int month, int day, int hour, int minute, int second);
/* 日時を加算する */
D_EXTERN CmnTimeDateTime* CmnTimeDateTime_AddBySerial(CmnTimeDateTime *datetime, time_t time);
/* CmnTimeDateTimeを文字列に変換する */
D_EXTERN char* CmnTimeDateTime_ToString(const CmnTimeDateTime *datetime, char *buf);
/* 現在時刻文字列生成 */
D_EXTERN char* CmnTime_Format(const CmnTimeDateTime *datetime, const CmnTimeFormatType type, char *buf);
/* 指定ミリ秒スリープする */
D_EXTERN void CmnTime_Sleep(unsigned long long msec);

#endif /* CMNCLIB_CMN_TIME_H */


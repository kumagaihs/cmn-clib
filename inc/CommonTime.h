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

#ifndef _COMMON_TIME_H
#define _COMMON_TIME_H

#include <time.h>
#include "Common.h"

/** 日付時刻情報 */
typedef struct tag_CmnTime_DateTime {
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
} CmnTime_DateTime;

/** 現在時刻文字列のフォーマットタイプ */
enum {
	CMN_TIME_FORMAT_ALL,			/**< 形式：yyyy/mm/dd[hh:mm:ss] */
	CMN_TIME_FORMAT_ALL_SHORT,		/**< 形式：yyyymmddhhmmss */
	CMN_TIME_FORMAT_DATE,			/**< 形式：yyyy/mm/dd */
	CMN_TIME_FORMAT_DATE_SHORT,		/**< 形式：yyyymmdd */
	CMN_TIME_FORMAT_TIME,			/**< 形式：hh:mm:ss */
	CMN_TIME_FORMAT_TIME_SHORT		/**< 形式：hhmmss */
};

/** フォーマット後の日付/時刻文字列長 */
#define CMN_TIME_FORMAT_SIZE_ALL			(20 + 1)	/**< 形式：yyyy/mm/dd[hh:mm:ss] */
#define CMN_TIME_FORMAT_SIZE_ALL_SHORT 		(14 + 1)	/**< 形式：yyyymmddhhmmss */
#define CMN_TIME_FORMAT_SIZE_DATE			(10 + 1)	/**< 形式：yyyy/mm/dd */
#define CMN_TIME_FORMAT_SIZE_DATE_SHORT		( 8 + 1)	/**< 形式：yyyymmdd */
#define CMN_TIME_FORMAT_SIZE_TIME			( 8 + 1)	/**< 形式：hh:mm:ss */
#define CMN_TIME_FORMAT_SIZE_TIME_SHORT		( 6 + 1)	/**< 形式：hhmmss */

/* --- CommonTime.c --- */
/* 現在日時を取得する */
D_EXTERN CmnTime_DateTime* CmnTime_DateTimeSetNow(CmnTime_DateTime *datetime);
/* 日時情報を作成する */
D_EXTERN CmnTime_DateTime* CmnTime_DateTimeSet(CmnTime_DateTime *datetime, int year, int month, int day, int hour, int minute, int second, int isdst);
/* 日時情報を作成する */
D_EXTERN CmnTime_DateTime* CmnTime_DateTimeSetByTime(CmnTime_DateTime *datetime, time_t time);
/* 日時を加算する */
D_EXTERN CmnTime_DateTime* CmnTime_DateTimeAdd(CmnTime_DateTime *datetime, int year, int month, int day, int hour, int minute, int second);
/* 日時を加算する */
D_EXTERN CmnTime_DateTime* CmnTime_DateTimeAddByTime(CmnTime_DateTime *datetime, time_t time);
/* 現在時刻文字列生成 */
D_EXTERN char *CmnTime_GetFormatTime(int type, char *buf);

#endif /* _COMMON_TIME_H */


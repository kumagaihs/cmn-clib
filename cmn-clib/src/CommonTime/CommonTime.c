/** @file *********************************************************************
 * @brief 日付/時刻系 共通関数
 *
 *  日付/時刻系共通関数のうち、特に汎用的な関数郡が実装されている。
 *
 * @author H.Kumagai
 * @date   2004-06-04
 * $Revision: 1.3 $
 *****************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#include"cmnclib/Common.h"
#include"cmnclib/CommonTime.h"
#include"cmnclib/CommonLog.h"

#if IS_PRATFORM_WINDOWS()
	#include <windows.h>
#else
	#include <unistd.h>
#endif


/* 年月日情報の調整値 */
static const int BASE_YEAR = 1900;	/* 年数調整用の値。localtime()では、                  */
										/* tm構造体のtm_yearには(現在年-1900)がセットされる為 */
static const int BASE_MON = 1;		/* 月調整用の値。tm_monは0～11月で表される為 */

/**
 * @brief tm構造体の内容をCmnTime_DateTimeにコピーする
 * @param time コピー元
 * @param datetime コピー先
 */
static void tmToDateTime(struct tm *time, CmnTimeDateTime *datetime)
{
	CMNLOG_TRACE_START();
	datetime->year = time->tm_year + BASE_YEAR;
	datetime->month = time->tm_mon + BASE_MON;
	datetime->dayOfMonth = time->tm_mday;
	datetime->dayOfWeek = time->tm_wday;
	datetime->dayOfYear = time->tm_yday;
	datetime->hour = time->tm_hour;
	datetime->minute = time->tm_min;
	datetime->second = time->tm_sec;
	datetime->isdst = time->tm_isdst;
	CMNLOG_TRACE_END();
}

/**
 * @brief CmnTime_DateTimeの内容をtm構造体にコピーする
 * @param datetime コピー元
 * @param time コピー先
 */
static void dateTimeToTm(CmnTimeDateTime *datetime, struct tm *time)
{
	CMNLOG_TRACE_START();
	time->tm_year = datetime->year - BASE_YEAR;
	time->tm_mon = datetime->month - BASE_MON;
	time->tm_mday = datetime->dayOfMonth;
	time->tm_wday = datetime->dayOfWeek;
	time->tm_yday = datetime->dayOfYear;
	time->tm_hour = datetime->hour;
	time->tm_min = datetime->minute;
	time->tm_sec = datetime->second;
	time->tm_isdst = datetime->isdst;
	CMNLOG_TRACE_END();
}

/**
 * @brief 現在日時を取得する
 *
 *  現在日時をdatetimeに設定する。
 *
 * @param datetime 日時を設定する先
 * @return datetimeを返却する
 */
CmnTimeDateTime* CmnTimeDateTime_SetNow(CmnTimeDateTime *datetime)
{
	CMNLOG_TRACE_START();
	CmnTimeDateTime *ret;
	ret = CmnTimeDateTime_SetBySerial(datetime, time(NULL));
	CMNLOG_TRACE_END();
	return ret;
}

/**
 * @brief 日時情報を設定する
 *
 *  引数で指定された日時をdatetimeに設定する
 *
 * @param datetime 日時を設定する先
 * @param year 西暦年
 * @param month 月(1 - 12)
 * @param day 日(1 - 31)
 * @param hour 時(0 - 23)
 * @param minute 分(0 - 59)
 * @param second 秒(0 - 59)
 * @param isdst 夏時間（夏時間の場合は1、そうでなければ0、不明の場合は-1）
 * @return datetimeを返却する
 */
CmnTimeDateTime* CmnTimeDateTime_Set(CmnTimeDateTime *datetime, int year, int month, int day, int hour, int minute, int second, int isdst)
{
	struct tm tmptm;
	time_t time;
	CmnTimeDateTime *ret;
	CMNLOG_TRACE_START();

	/* time_tを取得 */
	tmptm.tm_year = year - BASE_YEAR;
	tmptm.tm_mon = month - BASE_MON;
	tmptm.tm_mday = day;
	tmptm.tm_hour = hour;
	tmptm.tm_min = minute;
	tmptm.tm_sec = second;
	tmptm.tm_isdst = isdst;
	time = mktime(&tmptm);

	/* DateTimeを作成 */
	ret = CmnTimeDateTime_SetBySerial(datetime, time);

	CMNLOG_TRACE_END();
	return ret;
}

/**
 * @brief 日時情報を設定する
 *
 *  引数で指定された日時をdatetimeに設定する
 *
 * @param datetime 日時を設定する先
 * @param time 1970/01/01 00:00:00 をゼロとした経過秒
 * @return datetimeを返却する
 */
CmnTimeDateTime* CmnTimeDateTime_SetBySerial(CmnTimeDateTime *datetime, time_t time)
{
	struct tm tmptm;
	CMNLOG_TRACE_START();

	/* tm取得 */
#if IS_PRATFORM_WINDOWS()
		localtime_s(&tmptm, &time);
#else
		localtime_r(&time, &tmptm);
#endif

	/* 戻り値に移し替え */
	tmToDateTime(&tmptm, datetime);
	datetime->time = time;
	datetime->timezone = 0;		/* タイムゾーン設定処理を実装すること */

	CMNLOG_TRACE_END();
	return datetime;
}

/**
 * @brief 日時情報を設定する
 *
 *  引数で指定された日時をdatetimeに設定する。<br>
 *  加減算する値（year～second）には通常の時刻表記の上限を超えて設定可能。<br>
 *  <br>
 *  例：monthに12を超える数字、dayに31を超える数字を指定可能。<br>
 *  　　2020/11/1 にday=40を加算した場合、datetimeは2020/12/10となる。
 *
 * @param datetime 計算元となる日時
 * @param year 加減算する西暦年
 * @param month 加減算する月
 * @param day 加減算する日
 * @param hour 加減算する時
 * @param minute 加減算する分
 * @param second 加減算する秒
 * @return datetimeを返却する
 */
CmnTimeDateTime* CmnTimeDateTime_Add(CmnTimeDateTime *datetime, int year, int month, int day, int hour, int minute, int second)
{
	struct tm tmptm;
	time_t time;
	CmnTimeDateTime *ret;
	CMNLOG_TRACE_START();

	/* struct tmに変換 */
	dateTimeToTm(datetime, &tmptm);

	/* 日付の加減算 */
	tmptm.tm_year += year;
	tmptm.tm_mon += month;
	tmptm.tm_mday += day;
	tmptm.tm_hour += hour;
	tmptm.tm_min += minute;
	tmptm.tm_sec += second;

	/* 変換 */
	time = mktime(&tmptm);
	ret = CmnTimeDateTime_SetBySerial(datetime, time);

	CMNLOG_TRACE_END();
	return ret;
}

/**
 * @brief 日時情報を設定する
 *
 *  引数で指定されたtimeをdatetimeに設定する。<br>
 *
 * @param datetime 計算元となる日時
 * @param time 加減算する時間（1970/01/01 00:00:00 をゼロとした経過秒）
 * @return datetimeを返却する
 */
CmnTimeDateTime* CmnTimeDateTime_AddBySerial(CmnTimeDateTime *datetime, time_t time)
{
	CmnTimeDateTime *ret;
	CMNLOG_TRACE_START();

	ret = CmnTimeDateTime_SetBySerial(datetime, datetime->time + time);

	CMNLOG_TRACE_END();
	return ret;
}

/**
 * @brief CmnTimeDateTimeを文字列に変換する
 * @param datetime 変換するCmnTimeDateTime
 * @param buf 変換後文字列を格納するバッファ。バッファサイズはCMN_TIME_DATETIME_STRING_BUFFER_SIZE以上であること。
 */
char* CmnTimeDateTime_ToString(const CmnTimeDateTime *datetime, char *buf)
{
	CMNLOG_TRACE_START();
	*buf = '\0';
	sprintf(buf, "time=%I64d, year=%d, month=%d, day(m)=%d, day(w)=%d, day(y)=%d, hour=%d, min=%d, sec=%d, isdst=%d, timezone=%ld",
			datetime->time,
			datetime->year,
			datetime->month,
			datetime->dayOfMonth,
			datetime->dayOfWeek,
			datetime->dayOfYear,
			datetime->hour,
			datetime->minute,
			datetime->second,
			datetime->isdst,
			datetime->timezone);
	CMNLOG_TRACE_END();
	return buf;
}


/* TODO : CmnTime_DateTimeを渡して任意の時間の文字列を生成できるよう修正 */
/**
 * @brief 現在時刻文字列生成
 *
 *  現在時刻を指定されたフォーマットに従ってバッファに出力する。<BR>
 *  指定可能なフォーマットについては、引数typeの項を参照のこと。
 *
 * @param type      (I)   フォーマットタイプ。以下のものを指定すること
 *                    <UL>
 *                      <LI>CMN_TIME_FORMAT_ALL        &nbsp;&nbsp; --形式：yyyy/mm/dd hh:mm:ss</LI>
 *                      <LI>CMN_TIME_FORMAT_ALL_SHORT  &nbsp;&nbsp; --形式：yyyymmddhhmmss</LI>
 *                      <LI>CMN_TIME_FORMAT_DATE       &nbsp;&nbsp; --形式：yyyy/mm/dd</LI>
 *                      <LI>CMN_TIME_FORMAT_DATE_SHORT &nbsp;&nbsp; --形式：yyyymmdd</LI>
 *                      <LI>CMN_TIME_FORMAT_TIME       &nbsp;&nbsp; --形式：hh:mm:ss</LI>
 *                      <LI>CMN_TIME_FORMAT_TIME_SHORT &nbsp;&nbsp; --形式：hhmmss</LI>
 *                    </UL>
 *
 * @param buf       (O)   フォーマット後の文字列が格納されるバッファ。<BR>
 *                    必要なバッファサイズは、以下のマクロから得られる。
 *                    <UL>
 *                      <LI>CMN_TIME_FORMAT_SIZE_ALL        &nbsp;&nbsp; --形式：yyyy/mm/dd hh:mm:ss</LI>
 *                      <LI>CMN_TIME_FORMAT_SIZE_ALL_SHORT  &nbsp;&nbsp; --形式：yyyymmddhhmmss</LI>
 *                      <LI>CMN_TIME_FORMAT_SIZE_DATE       &nbsp;&nbsp; --形式：yyyy/mm/dd</LI>
 *                      <LI>CMN_TIME_FORMAT_SIZE_DATE_SHORT &nbsp;&nbsp; --形式：yyyymmdd</LI>
 *                      <LI>CMN_TIME_FORMAT_SIZE_TIME       &nbsp;&nbsp; --形式：hh:mm:ss</LI>
 *                      <LI>CMN_TIME_FORMAT_SIZE_TIME_SHORT &nbsp;&nbsp; --形式：hhmmss</LI>
 *                    </UL>
 *                    尚、typeに不正な値が指定された場合はバッファをNUL文字でクリアする
 * @return bufへのポインタ
 * @author H.Kumagai
 * @note この関数は<time.h>のlocaltime関数を使用している。<BR>
 *       そのため、2038年問題の影響を受ける。
 */
char *CmnTime_Format(int type, char *buf)
{
	struct tm *ptime;
	time_t now;
	CMNLOG_TRACE_START();

	time(&now);
	ptime = localtime(&now);

	/* 年、月情報の修正 */
	ptime->tm_year += BASE_YEAR;
	ptime->tm_mon  += BASE_MON;

	/* TODO 暇があったら、この部分を外出しする */
	switch (type) {
		/* 形式：yyyy/mm/dd hh:mm:ss */
		case CMN_TIME_FORMAT_ALL:
			sprintf(buf, "%04d/%02d/%02d %02d:%02d:%02d",
			        ptime->tm_year, ptime->tm_mon, ptime->tm_mday,
			        ptime->tm_hour, ptime->tm_min, ptime->tm_sec);
			break;

		/* 形式：yyyymmddhhmmss */
		case CMN_TIME_FORMAT_ALL_SHORT:
			sprintf(buf, "%04d%02d%02d%02d%02d%02d",
			        ptime->tm_year, ptime->tm_mon, ptime->tm_mday,
			        ptime->tm_hour, ptime->tm_min, ptime->tm_sec);
			break;

		/* 形式：yyyy/mm/dd */
		case CMN_TIME_FORMAT_DATE:
			sprintf(buf, "%04d/%02d/%02d",
			        ptime->tm_year, ptime->tm_mon, ptime->tm_mday);
			break;

		/* 形式：yyyymmdd */
		case CMN_TIME_FORMAT_DATE_SHORT:
			sprintf(buf, "%04d%02d%02d",
			        ptime->tm_year, ptime->tm_mon, ptime->tm_mday);
			break;

		/* 形式：hh:mm:ss */
		case CMN_TIME_FORMAT_TIME:
			sprintf(buf, "%02d:%02d:%02d",
			        ptime->tm_hour, ptime->tm_min, ptime->tm_sec);
			break;

		/* 形式：hhmmss */
		case CMN_TIME_FORMAT_TIME_SHORT:
			sprintf(buf, "%02d%02d%02d",
			        ptime->tm_hour, ptime->tm_min, ptime->tm_sec);
			break;

		default:
			*buf = '\0';
	}

	CMNLOG_TRACE_END();
	return buf;
}

/**
 * @brief 指定ミリ秒スリープする
 * @param msec スリープする時間（ミリ秒）
 */
void CmnTime_Sleep(unsigned long long msec)
{
	CMNLOG_TRACE_START();
#if IS_PRATFORM_WINDOWS()
	Sleep(msec);
#else
	usleep(msec * 1000);
#endif
	CMNLOG_TRACE_END();
}

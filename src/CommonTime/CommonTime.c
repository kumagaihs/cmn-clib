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

#include"Common.h"
#include"CommonTime.h"


/* 年月日情報の調整値 */
#define PLUS_YEAR  1900		/* 年数調整用の値。localtime()では、                  */
							/* tm構造体のtm_yearには(現在年-1900)がセットされる為 */
#define PLUS_MON      1		/* 月調整用の値。tm_monは0～11月で表される */


/**
 * @brief 現在時刻文字列生成
 *
 *  現在時刻を指定されたフォーマットに従ってバッファに出力する。<BR>
 *  指定可能なフォーマットについては、引数typeの項を参照のこと。
 *
 * @param type      (I)   フォーマットタイプ。以下のものを指定すること
 *                    <UL>
 *                      <LI>CMN_TIME_FORMAT_ALL        &nbsp;&nbsp; --形式：yyyy/mm/dd[hh:mm:ss]</LI>
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
 *                      <LI>CMN_TIME_FORMAT_SIZE_ALL        &nbsp;&nbsp; --形式：yyyy/mm/dd[hh:mm:ss]</LI>
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
char *CmnTime_GetFormatTime(int type, char *buf)
{
	struct tm *ptime;
	time_t now;

	time(&now);
	ptime = localtime(&now);

	/* 年、月情報の修正 */
	ptime->tm_year += PLUS_YEAR;
	ptime->tm_mon  += PLUS_MON;

	/* TODO 暇があったら、この部分を外出しする */
	switch (type) {
		/* 形式：yyyy/mm/dd[hh:mm:ss] */
		case CMN_TIME_FORMAT_ALL:
			sprintf(buf, "%04d/%02d/%02d[%02d:%02d:%02d]",
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
			memset(buf, '\0', sizeof(buf));
	}

	return buf;
}


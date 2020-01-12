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

#include "Common.h"

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
/* 現在時刻文字列生成 */
D_EXTERN char *CmnTime_GetFormatTime(int type, char *buf);

#endif /* _COMMON_TIME_H */


/** @file *********************************************************************
 * @brief マッピングリスト操作 共通関数
 *
 *  マッピングリストを作成、操作する共通関数。<BR>
 *  マッピングリストとは、キーと値の組をマッピングしたリストのこと。<BR>
 *  以下のテーブルのような構成になっている。<BR>
 *  <TABLE BADER=1>
 *    <TR><TD>キー</TD><TD>値</TD></TR>
 *    <TR><TD>101</TD><TD>値へ１のポインタ</TD></TR>
 *    <TR><TD>102</TD><TD>値へ２のポインタ</TD></TR>
 *    <TR><TD>103</TD><TD>値へ３のポインタ</TD></TR>
 *  </TABLE>
 *  例えば、キー[101]を指定してCmnData_GetArg()関数をコールすると、
 *  値１へのポインタが得られる。
 * 
 * @author H.Kumagai
 * @date   2004-06-XX
 * $Revision: 1.1 $
 *****************************************************************************/
#include"stdafx.h"



//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MsTime.cpp
//
//		時刻処理
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"
#include "MsLib.h"

namespace MC
{

////////////////////////////////////////////////////////////////////////////
//【機能】現在時刻を取得する
//【返値】MTIME型　現在時刻

MTIME MsGetCurrentTime()
{
	return CTime::GetCurrentTime();
}

	//struct	tm newTime;
	//time_t	longTime;
	//static char	cTimeBf[26];
	//errno_t err;

	//time(&longTime);
	//err = localtime_s( &newTime, &longTime );
	//ASSERT( err);

	//long yy = newTime.tm_year+1900;				// 年 
	//long mm = newTime.tm_mon+1;					// 月
	//long dd = newTime.tm_mday;					// 日
	//long wk = newTime.tm_wday;					// 曜日
	//long hh = newTime.tm_hour;					// 時間 
	//long mn = newTime.tm_min;					// 分
	//long ss = newTime.tm_sec;					// 秒


////////////////////////////////////////////////////////////////////////////
//【機能】時刻を比較する
//【返値】比較結果 -1 : cTime1が早い
//					0 : 同一
//					1 : cTime2が早い

MINT MsCmpTime(
						MTIME		i_cTime1,	// 比較対象時間1
						MTIME		i_cTime2	// 比較対象時間2
				)
{

	MINT		ist;

	if ( i_cTime1 < i_cTime2)
		ist = -1;
	else if ( i_cTime1 == i_cTime2)
		ist = 0;
	else
		ist = 1;

	return ist;
}

////////////////////////////////////////////////////////////////////////////
//【機能】整数型時刻に変換する
//【返値】> 0 : MTIME型　現在時刻
//		  < 0 : エラー

MTIME MsMTime(
						MINT		i_iYear,	// 年
						MINT		i_iMonth,	// 月
						MINT		i_iDay,		// 日
						MINT		i_iHour,	// 時
						MINT		i_iMin,		// 分
						MINT		i_iSec		// 秒
				)
{
	return CTime( i_iYear, i_iMonth, i_iDay, i_iHour, i_iMin, i_iSec, 0);
}

///////////////////////////////////////////////////////////////////////////////
//	CTimeの日時をCStringに変換する
CString MsCnvSCTime(							// 
						MINT		i_iMode,	// 変換形式
												//	 1: YYMMDD
												//	 2: YYMMDDhh
												//	 3: YYMMDDhhmm
												//	 4: YYMMDDhhmmss
												//	11: YYYYMMDD
												//	12: YYYYMMDDhh
												//	13: YYYYMMDDhhmm
												//	14: YYYYMMDDhhmmss
												//	21: YY/MM/DD
												//	23: YY/MM/DD hh:mm
												//	24: YY/MM/DD hh:mm:ss
												//	31: YYYY/MM/DD
												//	33: YYYY/MM/DD hh:mm
												//	34: YYYY/MM/DD hh:mm:ss
						CTime&		i_theTime	// CTime 日時
				)
{
	static MCHAR cTimeBf[20];

	MINT iYear	= i_theTime.GetYear();
	MINT iMonth	= i_theTime.GetMonth();
	MINT iDay	= i_theTime.GetDay();
	MINT iHour	= i_theTime.GetHour();
	MINT iMin	= i_theTime.GetMinute();
	MINT iSec	= i_theTime.GetSecond();

	switch ( i_iMode)
	{
	case 1:
		Msprintf( cTimeBf, Mstr( "%02d%02d%02d"), iYear-2000, iMonth, iDay);
		break;
	case 2:
		Msprintf( cTimeBf, Mstr( "%02d%02d%02d%02d"), iYear-2000, iMonth, iDay, iHour);
		break;
	case 3:
		Msprintf( cTimeBf, Mstr( "%02d%02d%02d%02d%02d"), iYear-2000, iMonth, iDay, iHour, iMin);
		break;
	case 4:
		Msprintf( cTimeBf, Mstr( "%02d%02d%02d%02d%02d%02d"), iYear-2000, iMonth, iDay, iHour, iMin, iSec);
		break;
	case 11:
		Msprintf( cTimeBf, Mstr( "%4d%02d%02d"), iYear, iMonth, iDay);
		break;
	case 12:
		Msprintf( cTimeBf, Mstr( "%4d%02d%02d %02d"), iYear, iMonth, iDay, iHour);
		break;
	case 13:
		Msprintf( cTimeBf, Mstr( "%4d%02d%02d %02d:%02d"), iYear, iMonth, iDay, iHour, iMin);
		break;
	case 14:
		Msprintf( cTimeBf, Mstr( "%4d%02d%02d %02d:%02d:%02d"), iYear, iMonth, iDay, iHour, iMin, iSec);
		break;
	case 21:
		Msprintf( cTimeBf, Mstr( "%02d/%02d/%02d"), iYear-2000, iMonth, iDay);
		break;
	case 22:
		Msprintf( cTimeBf, Mstr( "%02d/%02d/%02d %02d"), iYear-2000, iMonth, iDay, iHour);
		break;
	case 23:
		Msprintf( cTimeBf, Mstr( "%02d/%02d/%02d %02d:%02d"), iYear-2000, iMonth, iDay, iHour, iMin);
		break;
	case 24:
		Msprintf( cTimeBf, Mstr( "%02d/%02d/%02d %02d:%02d:%02d"), iYear-2000, iMonth, iDay, iHour, iMin, iSec);
		break;
	case 31:
		Msprintf( cTimeBf, Mstr( "%4d/%02d/%02d"), iYear, iMonth, iDay);
		break;
	case 32:
		Msprintf( cTimeBf, Mstr( "%4d/%02d/%02d %02d"), iYear, iMonth, iDay, iHour);
		break;
	case 33:
		Msprintf( cTimeBf, Mstr( "%4d/%02d/%02d %02d:%02d"), iYear, iMonth, iDay, iHour, iMin);
		break;
	case 34:
		Msprintf( cTimeBf, Mstr( "%4d/%02d/%02d %02d:%02d:%02d"), iYear, iMonth, iDay, iHour, iMin, iSec);
		break;
	}
	return cTimeBf;
}

///////////////////////////////////////////////////////////////////////////////
//	ツェラーの公式
//	(年、月、日　から　曜日を求める: 0が日曜、1が月曜、…6が土曜)
int zeller(int y, int m, int d)
{
	int value;
	
	if( m == 1 || m == 2) {
		--y; /* 1,2月は前の年の13,14月として扱うため１年戻すために-1をする */
		m += 12; /* 1,2月は前の年の13,14月として扱うため12を足す */
	} 

	value = (y + y / 4 - y / 100 + y / 400 + (13 * m + 8) / 5 + d) % 7;
		/* （年 + 年 / 4 - 年 / 100 + 年 / 400 + (13 * 月 + 8) / 5 + 日)
			を７で割った余りというツェラーの公式そのもの */
	
	return value; /* 公式の結果を戻り値とする */
}

} // namespace MC
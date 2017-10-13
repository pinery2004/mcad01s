//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MsLib.cpp
//
//		
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"
#include "MsLib.h"


// SORT SAmple
namespace MC
{

//int CDatas::Compare( const void *parg1, const void *parg2) {
int CARRAY<CData>::Compare( const void *parg1, const void *parg2) {
	int int1 = ((CData *)parg1)->lgKey;
	int int2 = ((CData *)parg2)->lgKey;
	if ( int1 < int2)
		return -1;
	else
		if ( int1 > int2)
			return +1;
	return 0;
}

void SortSample()
{
	CString ss = L"Ç±ÇÒÇ…ÇøÇÕÅAê¢äE\n";
	TRACE( ss);

	TRACE( L"Ç±ÇÒÇ…ÇøÇÕÅAê¢äE\n");

	CData Data, SearchData;
//	CDatas Datas;
	CARRAY<CData> Datas;

	Data.lgKey = 10;
	Data.sNm = L"ÇƒÇ∑Ç∆TEST10";
	Data.lgN = 100;
	Datas.Add(Data);

	Data.lgKey = 8;
	Data.sNm = "TEST80";
	Data.lgN = 80;
	Datas.Add(Data);

	Data.lgKey = 4;
	Data.sNm = "TEST40";
	Data.lgN = 40;
	Datas.Add(Data);

	Data.lgKey = 5;
	Data.sNm = "TEST50";
	Data.lgN = 50;
	Datas.Add(Data);

	Datas.Sort();

	int iC;
	for( iC=0; iC<Datas.GetCount(); iC++) {
		CData* pPI = &Datas[iC];
		TRACE( L"SORT lgKey(%d), sNm(%s), lgN(%d)\n", pPI->lgKey, (LPCTSTR)pPI->sNm, pPI->lgN);
		TRACE( L"SORT lgKey(%d), sNm(", pPI->lgKey);
		TRACE( pPI->sNm);
		TRACE( L"), lgN(%d)\n", pPI->lgN);
	}

	Data.lgKey = 0;
	Data.sNm = "";
	Data.lgN = 0;

	SearchData = Datas[2];
	SearchData.sNm = "";
	SearchData.lgN = 0;
	Data = *Datas.Search( SearchData);

	TRACE( "SEARCH lgKey(%d), sNm(%s), lgN(%d)\n", Data.lgKey, Data.sNm, Data.lgN);
}

}
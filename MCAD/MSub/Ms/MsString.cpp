//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MsString.cpp
//
//		
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"
#include "MsLib.h"

namespace MC
{

///////////////////////////////////////////////////////////////////////////////
//	CStringの文字列を実数に変換する
MREAL MsGetMREAL(									// 実数
						CString&	in_str			// CString文字列
				)
{
//	MCHAR	sNum[20];
//	Mstrncpy( sNum, in_str, 20);
//	MREAL	r = Matof( sNum);
	MREAL	r = Matof( in_str);
	return r;
}

///////////////////////////////////////////////////////////////////////////////
//	CStringの文字列を整数に変換する
MINT MsGetMINT(										// 整数
						CString&	in_str			// CString文字列
				)
{
//	MINT	i = atoi( in_str);
//	MCHAR	sNum[20];
//	Mstrncpy( sNum, str, 20);
//	MINT	i = Matoi( sNum);
	MINT	i = Matoi( in_str);
	return i;
}

///////////////////////////////////////////////////////////////////////////////
//	MINTの整数をCStringに変換する
CString MsCnvSMINT(									// 整数文字列
						MINT	i_iMode,			// モード　0: "n"形式に変換
						MINT	i_iNum				// 整数
				)
{
	CString sKoumoku;
	sKoumoku.Format( Mstr( "%d"), i_iNum);
	return sKoumoku;
}

///////////////////////////////////////////////////////////////////////////////
//	MINTの実数をCStringに変換する
CString MsCnvSMREAL(								// 整数文字列
						MINT	i_iMode,			// モード　1: "0.0"形式に変換
						MREAL	i_iReal				// 実数
				)
{
	CString sKoumoku;
	sKoumoku.Format( Mstr( "%7.1f"), i_iReal);
	return sKoumoku;
}

///////////////////////////////////////////////////////////////////////////////
// ファイル名の拡張子より形式を得る
// 返値: ファイル形式 拡張子バッファの配列番号    -1: ｴﾗｰ

MINT MsGetFileExtNo(							// 拡張子バッファの配列番号 -1:ｴﾗｰ
						MCHAR*	in_cFileName,	// ﾌｧｲﾙ名
						MCHAR**	in_ppExt,		// 拡張子バッファ(例 "bmp", "jpg" : 0, 1)
						MINT	in_NExt)		// 拡張子数
{
	MINT	iC;
//	MINT	iTp;								// ﾌｧｲﾙの種類
	MCHAR	cPrd = '.';							// ﾋﾟﾘｵﾄﾞ記号
	MCHAR	cExt[11];							// ファイル名の拡張子
	MCHAR*	pcExt;								// 拡張子の位置を示すﾎﾟｲﾝﾀ

	pcExt = Mstrrchr( in_cFileName, cPrd);						// ﾌｧｲﾙ名の末尾から'.'を探す
	if ( pcExt != NULL) {
		pcExt++;												// 拡張子
		Code::Mstrtolower( pcExt, cExt, 11);							// 小文字に変換
		for ( iC=in_NExt; iC<in_NExt; iC--) {
			if ( Mstrcmp( pcExt, in_ppExt[iC]) == 0)			// ﾋﾞｯﾄﾏｯﾌﾟ
				break;
		}
	} else {
		iC = -1;
	}
	return iC;
}

//U
///******************************************************************************************************************
//[ 機能 ] 文字列を";" をデリミタとし分解する
//[ 返値 ] 行数      0 : なし
//******************************************************************************************************************/
//MINT HsSplitString             (														// (  O) 行数         0:なし
//								MCHAR				String[],							// (I  ) 文字列
//								MCHAR				SLine[][128])						// (  O) 分解後文字列 [行][列]
//{
//	MINT			NumLne;
//	
//	MINT			Num;
//	MINT			QFlg;
//	MINT			ic;
//
//
//	NumLne = 0;																			// 行数
//	Num    = 0;																			// 文字数
//	QFlg   = 0;																			// " なし
//	ic     = 0;
//	while  (String[ic] != NULL && String[ic] != Mchar('\n')) {						// 終わり・改行以外
//		if (String[ic] == Mchar('"')) {
//			QFlg = 1 - QFlg;															// 反転
//		} else if (QFlg == 1) {															// ﾌﾗｸﾞON
//			SLine[NumLne][Num] = String[ic];
//			if (SLine[NumLne][Num] == Mchar('\t'))  SLine[NumLne][Num] = Mchar(' ');
//			Num++;
//		} else {																		// ﾌﾗｸﾞOFF
//			if (String[ic] == Mchar(';')) {											// ﾃﾞﾘﾐﾀ
//				SLine[NumLne][Num] = NULL;
//				NumLne++ ; Num = 0;
//			} else {
//				SLine[NumLne][Num] = String[ic];
//				if (SLine[NumLne][Num] == Mchar('\t'))  SLine[NumLne][Num] = Mchar(' ');
//				Num++;
//			}
//		}
//		ic++;
//	}
//	SLine[NumLne][Num] = NULL;
//	NumLne++;
//
//	return(NumLne);
//}
//
// *******************************************************************************
//										Created on 10/01/00 by K.Matsubayashi
//										Updated on xx/xx/xx/ by x.xxxxxx
//										Copyright
//【機能】MBCS文字列をUNICODE文字列に変換する
//【返値】 0 : 正常     -1 : 異常
//【注意】なし
// ********************************************************************************

MINT Mmbstowcs( 
				char*	StrC,					// ( I  ) MBCS文字列
				MCHAR*	StrU,					// (  O) UNICODE文字列
				MINT	iSize					// ( I  ) 最大文字数
				)
{

#ifdef _UNICODE
	MINT iLen = ( MINT)strlen( StrC);

	if ( iLen == 0) {
		StrU[0] = NULL;
		return( 0);
	} else {
		MINT iLen1 = MultiByteToWideChar( CP_ACP, 0, ( LPCSTR)StrC,  iLen,  ( LPWSTR)StrU, iSize);
		if ( iLen1 == 0) {
			StrU[0] = NULL;
			return( -1);
		} else {
			StrU[min( iLen1, iSize)] = NULL;
			return( 0);
		}
	}
#else
	Mstrncpy( StrU, StrC, iSize+1);
	return( 0);
#endif
}

// *******************************************************************************
//										Created on 10/01/00 by K.Matsubayashi
//										Updated on xx/xx/xx/ by x.xxxxxx
//										Copyright
//【機能】UNICODE文字列をMBCS文字列に変換する
//【返値】 0 : 正常     -1 : 異常
//【注意】なし
// ********************************************************************************

MINT Mwcstombs( 
				MCHAR*	StrU,					// ( I  ) UNICODE文字列
				char*	StrC,					// (  O) MBCS文字列
				MINT	iSize					// ( I  ) 最大文字数
				)
{
#ifdef _UNICODE
	MINT iLen = ( MINT)Mstrlen( StrU);
	if ( iLen == 0) {
		StrC[0] = NULL;
		return( 0);
	} else {
		MINT iLen1 = WideCharToMultiByte( CP_ACP, 0, ( LPCWSTR)StrU, iLen, ( LPSTR)StrC, iSize, NULL, NULL);
		if ( iLen1 == 0) {
			StrC[0] = NULL;
			return( -1);
		} else {
			StrC[min( iLen1, iSize)] = NULL;
			return( 0);
		}
	}

#else
	strcpy( StrC, StrU);
	return( 0);
#endif
}

} // namespace MC
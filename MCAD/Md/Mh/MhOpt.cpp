//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MhOpt.cpp
//
//		ＯＰＴ群制御サポート
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"

#include "MbDebug.h"
#include "MgDefine.h"
#include "MgTol.h"

#include "MgAngle.h"
#include "MgPoint.h"
#include "MgLine.h"
#include "MgPlane.h"
#include "MgPolygon.h"

#include "MmDefine.h"
#include "MsBitSet.h"
#define	DLL_EXPORT_OPTION_DO
#include "MmOpt.h"
#include "MmHist.h"

#include "MbList.h"
#include "MhLib.h"

#include "MmLib.h"
#include "MhInp.h"

#include "MgGrp.h"

#include "MhRoof.h"
#include "MdLib.h"

#define	MH_ALL_BITS	0xFFFFFFFF

#define		MHSZ_OPTION		sizeof(MUINT)*3

namespace MC
{

/////////////////////////////////////////////////////////////////////////////
//【機能】ＯＰＴ群コードの領域を開放する
void Option::MhFreOpt(
						MsBitSet		*pOptv		// (I O) 開放ＯＰＴ群 NULL: 開放不要
				)
{
	if ( pOptv != NULL)
		delete (char*)pOptv;
}

/////////////////////////////////////////////////////////////////////////////
//【機能】新規レコード追加時に設定するＯＰＴ群コードを取得する
//		　（ＯＰＴ群コードが入るエリアの確保は本関数で行う）
void Option::MhAddOpt(
						MsBitSet*		*ppOptv		// (  O) 追加レコードに設定するＯＰＴ群コードのポインタ NULL: 全ＯＰＴ群
				 )
{
	MhOpt	bOptC;
	MsBitSet	*pOptvW;

	if ( MhOptV::MmOptGetC( &bOptC)) {							// カレント選択ＯＰＴ群を取得する
		*ppOptv = NULL;											// 標準
	} else {													// 標準以外のオプション設定あり
		pOptvW = (MsBitSet*)new char[MHSZ_OPTION];

#ifdef _DEBUG
						Msprintf( MbLog::m_Str, Mstr( "MALLOC OPTION MhAddOpt TRC %x %x %x\n"), pOptvW->GetSOB()[0], pOptvW->GetSOB()[1], pOptvW->GetSOB()[2]);
						MBTRCPRBF;
#endif
		if ( MhOptV::MmOptSrch( bOptC) == 0) {				// 新しいオプション群なら追加し
			MhOptV::MmOptAddDB();									// 参照対象全DBレコードにＯＰＴ群追加
			MhOptV::MmOptIncSrch( bOptC);							// 読込み書込みオプション群セットを求める
		}
		*pOptvW = MhOptV::MhOptBScWGet();
		*ppOptv = pOptvW;
	}
}

/////////////////////////////////////////////////////////////////////////////
//【機能】レコード修正時に設定するＯＰＴ群コードを取得する
//【返値】修正後のレコード構成コード
//			1 : 修正元ＯＰＴ群は書込みＯＰＴ群と同一の場合で、
//				修正元レコードは、ＯＰＴ群変更なしで
//				修正内容を上書きする。	追加レコードはなし。
//
//			2 :	修正元ＯＰＴ群は書込みＯＰＴ群に全て含まれかつ追加ＯＰＴ群がある場合で、
//				修正元レコードは、書込みＯＰＴ群に変更し、→ ＯＰＴ群変更なしで　（1と同じ）
//				修正内容を上書きする。追加レコードはなし。
//
//			3 : 修正元ＯＰＴ群に書込みＯＰＴ群以外のＯＰＴ群を含む場合で、
//				書込みＯＰＴ群以外のＯＰＴ群部分は、修正元レコードをそのまま残しＯＰＴ群コードのみ修正する。
//				さらに、書込みＯＰＴ群を持つ修正後のレコードを新たに追加する。
//				（ＯＰＴ群コードが入るエリアの開放と確保は本関数で行う）
MINT Option::MhModOpt(
						MhPlcInfo	 *pPlcEnR,		// (I O) 修正元部材
						MhPlcInfo* *ppPlcEnM		// (  O) 修正先部材
				)
{
	MhOpt	OptC;
	MsBitSet	OptvM;

	MsBitSet	*pOptvRN;
	MsBitSet	*pOptvDN;
	MsBitSet	*pOptvWN;
	MINT		iMod;
	MhOptV	*pOptv;

	if ( !MhOptV::MmOptGetC( &OptC)) {								// カレント選択ＯＰＴ群を取得する
																// 標準以外のオプション設定あり
		if ( MhOptV::MmOptSrch( OptC) == 0) {
			MhOptV::MmOptAddDB();									// 参照対象全DBレコードにＯＰＴ群追加
			MhOptV::MmOptIncSrch( OptC);
		}
	}

	pOptvRN = pPlcEnR->GetPIOpt1();
	iMod = MhOptV::MmOptModChk( &pOptvRN, &pOptvWN, &pOptv);

	if ( iMod < 3) {
																// 修正元ＯＰＴ群は書込みＯＰＴ群と同一の場合
																// 修正元ＯＰＴ群は書込みＯＰＴ群に全て含まれかつ追加ＯＰＴ群がある場合
			*ppPlcEnM = pPlcEnR;							// 修正元レコードは、ＯＰＴ群変更なし
	} else {
																// 修正元ＯＰＴ群に書込みＯＰＴ群以外のＯＰＴ群を含む場合
			MhPlcInfo	PlcEnM;
			PlcEnM.Copy( *pPlcEnR);							// 

			pOptvDN = pPlcEnR->GetPIOpt2();
			if ( pOptvDN == NULL) {
				pOptvDN = (MsBitSet*)new char[sizeof( pOptv->bmCurWGOptSet)];	// 修正用に削除後のレコードに表示ＯＦＦOPT群セットを追加
				*pOptvDN = pOptv->bmCurWGOptSet;								// 、設定
			} else {
				*pOptvDN |= pOptv->bmCurWGOptSet;								// 修正用に削除後のレコードに表示ＯＦＦOPT群セットを設定
			} 
			pPlcEnR->SetPIOpt2( pOptvDN);

			PlcEnM.FreePIOpt1();
			PlcEnM.SetPIOpt1( pOptvWN);

			*ppPlcEnM = HaitiDb::MdInjectPts( &PlcEnM);
	}
	return iMod;
}

/////////////////////////////////////////////////////////////////////////////
//【機能】レコード削除時に設定するＯＰＴ群コードを取得する
//【返値】削除後のレコード構成コード
//			1 : 削除元ＯＰＴ群は書込みＯＰＴ群と同一の場合で、
//				削除元レコードは削除する。
//			2 :	削除元ＯＰＴ群は書込みＯＰＴ群に全て含まれかつ追加ＯＰＴ群がある場合で、
//				削除元レコードは削除する。
//			3 : 削除元ＯＰＴ群に書込みＯＰＴ群以外のＯＰＴ群を含む場合で、
//				書込みＯＰＴ群以外のＯＰＴ群部分は、削除元レコードをそのまま残しＯＰＴ群コードのみ修正する。
//				（ＯＰＴ群コードが入るエリアの開放と確保は本関数で行う）

MINT Option::MhDelOpt(
						MsBitSet*		*ppOptvRN,		// (I O) 削除前後のレコードの表示ＯＮ  OPT群セット
						MsBitSet*		*ppOptvDN		// (I O) 削除前後のレコードの表示ＯＦＦOPT群セット
				)
{
	MINT		ist;

	MhOpt	OptC;
	MsBitSet	OptvM;

	if ( !MhOptV::MmOptGetC( &OptC)) {						// カレント選択ＯＰＴ群を取得する
														// 標準以外のオプション設定あり
		if ( MhOptV::MmOptSrch( OptC) == 0) {
			MhOptV::MmOptAddDB();							// 参照対象全DBレコードにＯＰＴ群追加
			MhOptV::MmOptIncSrch( OptC);
		}
	}
	ist = MhOptV::MmOptDelChk( ppOptvRN, ppOptvDN);
	return ist;
}

} // namespace MC
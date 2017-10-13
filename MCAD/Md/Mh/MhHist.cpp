//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MhHist.cpp
//
//		履歴管理制御サポート
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

#include "MhPlcInfo.h"

#include "MdLib.h"


#define	MH_ALL_BITS	0xFFFFFFFF

#define		MHSZ_Hist		sizeof(MUINT)*3


namespace MC
{

/////////////////////////////////////////////////////////////////////////////
//【機能】履歴管理コードの領域を開放する
	void MhOpt::MhHistFre(
//						MhHistV		*pHstv		// (I O) 開放履歴管理 NULL: 開放不要
						MsBitSet*	*pHstv		// (I O) 開放履歴管理 NULL: 開放不要
				)
{
	if ( pHstv != NULL)
		delete (char*)pHstv;
}

/////////////////////////////////////////////////////////////////////////////
//【機能】新規レコード追加時に設定する履歴管理コードを取得する
//		　（履歴管理コードが入るエリアの確保は本関数で行う）
void MhOpt::MhHistAdd(
						MsBitSet*		*ppHstv		// (  O) 追加レコードに設定する履歴管理コードのポインタ NULL: 全履歴管理
				 )
{
	MsBitSet	bHstvC;
	MsBitSet	*pHstvW;

	if ( MhHist::MmVerGetC( &bHstvC)) {							// カレント選択履歴管理取得
		*ppHstv = NULL;											// 初期
	} else {													// 初期以外のバージョンを設定
		pHstvW = (MhHistV*)new char[MHSZ_Hist];

#ifdef _DEBUG
						Msprintf( MbLog::m_Str, Mstr( "MALLOC Hist MhHistAdd TRC %x %x %x\n"), pHstvW->SOB(0), pHstvW->SOB(1), pHstvW->SOB(2));
						MBTRCPRBF;
#endif

		*pHstvW = bHstvC;
		*ppHstv = pHstvW;
	}
	return;
}

/////////////////////////////////////////////////////////////////////////////
//【機能】レコード修正時に設定する履歴管理コードを取得する
//【返値】修正後のレコード構成コード
//			1 : 修正元履歴管理はカレントバージョンのみ設定されている場合で、
//				修正元レコードは履歴管理変更なしで修正内容を上書きする。
//				追加レコードはなし。
//			3 : 修正元履歴管理にカレントバージョン以外のバージョンを含む場合で、
//				カレントバージョン以外の履歴部分は、修正元レコードをそのまま残し履歴管理コードのみ修正する。
//				さらに、カレントバージョンのみの履歴管理を持つ修正後のレコードを新たに追加する。
//				（履歴管理コードが入るエリアの開放と確保は本関数で行う）
MINT MhOpt::MhHistMod(
						MhPlcInfo	*pPlcEnR,		// (I O) 修正元部材
						MhPlcInfo*	*ppPlcEnM		// (  O) 修正先部材
				)
{
	MsBitSet	*pHstvR;
	MsBitSet	HstvMod;
	MsBitSet	*pHstvMod;
	MsBitSet	*pHstvAdd;
	MINT		iMod;

	pHstvR = pPlcEnR->GetPIVer();

	// 修正元部材の履歴情報の取得
	if ( pHstvR == NULL) {										// 修正対象部品配置の履歴管理をコピー
		pHstvMod = NULL;
	} else {
		HstvMod = *pHstvR;
		pHstvMod = &HstvMod;
	}
	
	iMod = MhHist::MmHistModChk( &pHstvMod, &pHstvAdd);

	switch ( iMod) {
		case 1:													// 修正元履歴管理はカレントバージョンのみ設定している場合
			*ppPlcEnM = pPlcEnR;							// 		修正元のレコードを修正対象とする
			break;
		case 3:													// 修正元履歴管理にカレントバージョン以外のバージョンを含む場合
																// 		修正元の履歴管理を変更する（カレントバージョンをＯＦＦする）
			if ( pHstvR == NULL) {
				pPlcEnR->SetPIVer( pHstvMod);				//			新規追加カレントバージョン以外ＯＮの履歴管理のポインタをセット
			} else {
				*pHstvR = *pHstvMod;							//			履歴管理をカレントバージョンＯＦＦに修正
			}

			MhPlcInfo	PlcEnM;								// 		新規に履歴管理がカレントのコピーレコードを作成し修正対象とする
			PlcEnM.Copy( *pPlcEnR);

			PlcEnM.FreePIVer();
			PlcEnM.SetPIVer( pHstvAdd);
			*ppPlcEnM = HaitiDb::MdInjectPts( &PlcEnM);
	}
	return iMod;
}

/////////////////////////////////////////////////////////////////////////////
//【機能】レコード削除時に設定する履歴管理コードを取得する
//【返値】削除後のレコード構成コード
//			1 : 削除元履歴管理はカレントバージョンと同一の場合で、
//				削除元レコードは削除する。
//			3 : 削除元履歴管理にカレントバージョン以外の履歴管理を含む場合で、
//				カレントバージョン以外の履歴管理部分は、削除元レコードをそのまま残し履歴管理コードのみ修正する。
//				（履歴管理コードが入るエリアの開放と確保は本関数で行う）


MINT MhOpt::MhHistDel(
						MsBitSet*		*ppHstvMod		// (I O) 削除後のレコードの表示ＯＮ履歴管理群
				)
{
	return MhHist::MmHistDelChk( ppHstvMod);
}

} // namespace MC
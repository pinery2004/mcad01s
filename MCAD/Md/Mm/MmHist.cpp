//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MmHist.cpp
//
//		履歴管理処理
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"
#include "MgLib.h"
#include "MgMat.h"
#include "MbDefine.h"
#include "MbCod.h"
#include "MmGridNum.h"
#include "MmDrag.h"
#include "MmWnd.h"
#include "MmDefine.h"
#include "MmLib.h"
#include "MmSystemProperty.h"

#include "MmCmd.h"

#include "MsBitSet.h"
#define DLL_EXPORT_MHHISTV_DO
#include "MmHist.h"

#include "MbList.h"
#include "MhLib.h"

#include "MhPlcInfo.h"
#include "MdLib.h"

namespace MC
{

static	MhHistV		z_Verv;						// バージョン　履歴管理

/////////////////////////////////////////////////////////////////////////////
//	バージョン管理情報
//  スタティック属性値

//bool		MhHistV::fcCng_NU;					// カレントバージョンの変更有無
												//		(TRUE: 変更有り、FALSE: 変更無し)
bool		MhHistV::fcStd;						// カレントバージョン==初期　フラグ
												//		(TRUE:初期、FALSE:初期以外のバージョン)
MsBitSet	MhHistV::vVerC;						// カレントバージョン
MsBitSet	MhHistV::vVerB;						// コピー元バージョン
MINT		MhHistV::nVer;						// 登録バージョン数
CString		MhHistV::sVerC[MSZ_HISTB];			// 登録バージョン
CString		MhHistV::sVerB[MSZ_HISTB];			// コピー元バージョン
CString		MhHistV::sVerD[MSZ_HISTB];			// 修正日時

///////////////////////////////////////////////////////////////////////////////
//	CStringの文字列を整数に変換する
CString MsCnvSCTime(							// "YYYY/MM/DD HH:SS"
						MINT	i_iMode,		// モード　1: "YYYY/MM/DD HH:SS"形式に変換
						CTime&	i_theTime		// CTime 日時
				);

/////////////////////////////////////////////////////////////////////////////
//【機能】修正履歴管理情報を初期化する
void MhHist::MmHistInit()
{
//	z_Verv.fcCng_NU = false;
	z_Verv.fcStd = true;
	z_Verv.vVerC = z_Verv.vVerB = MsBitSet( 0, 0, 0);  
	z_Verv.nVer = 1;
	z_Verv.sVerC[0] = "0.00"; 
	z_Verv.sVerB[0] = ""; 
	z_Verv.sVerD[0] = "";
	CTime theTime = CTime::GetCurrentTime();
	z_Verv.sVerD[0] = MsCnvSCTime( 33, theTime); 
}

/////////////////////////////////////////////////////////////////////////////
//【機能】修正履歴管理情報を取得する
void MhHist::MmHistGet(
						MINT*		o_pnVer,
						CString*	*o_ppsVerC,
						CString*	*o_ppsVerB,
						CString*	*o_ppsVerD
				)
{
	*o_pnVer = z_Verv.nVer;
	*o_ppsVerC = z_Verv.sVerC;
	*o_ppsVerB = z_Verv.sVerB;
	*o_ppsVerD = z_Verv.sVerD;
}

/////////////////////////////////////////////////////////////////////////////
//【機能】バージョンＵＰ
//【返値】ステイタス	0: 正常	-1: 旧バージョン無し	-2: 新バージョンが既に有り
//						-3: 新バージョンが空白
MINT MhHist::MmVerUp(
						CString		i_sVerB,
						CString		i_sVerC,
						MINT*		o_pnVer,
						CString*	*o_ppsVerC,
						CString*	*o_ppsVerB,
						CString*	*o_ppsVerD
				)
{
	MINT		ist = 0;
	MINT		ic, icf, ict;
	MhHistV		VerB;
	MhHistV		VerC;
	CTime		theTime;

	if ( i_sVerB == "") {
		icf = 0;
	} else {
		for ( ic=0; ic<z_Verv.nVer; ic++)
			if ( i_sVerB == z_Verv.sVerC[ic])
				break;
		if ( ic == z_Verv.nVer) {
			ist = -1;								// 旧バージョンなし
			MQUIT;
		}
		icf = ic;
	}
	if ( i_sVerC == "") {
		ist = -3;									// 新バージョンが空白
		MQUIT;
	}
	for ( ic=0; ic<z_Verv.nVer; ic++)
		if ( i_sVerC == z_Verv.sVerC[ic])
			break;
	if ( ic < z_Verv.nVer) {
		ist = -2;									// 新バージョンが既に有り
		MQUIT;
	}
	ict = z_Verv.nVer++;
	z_Verv.sVerC[ict] = i_sVerC;
	z_Verv.sVerB[ict] = i_sVerB;
	theTime = CTime::GetCurrentTime();
	z_Verv.sVerD[ict] = MsCnvSCTime( 33, theTime); 

	z_Verv.vVerC.SetMask( ict);
	z_Verv.vVerB.SetMask( icf);

	if ( ict == 0) 
		z_Verv.fcStd = true;
	else
		z_Verv.fcStd = false;

	MPOSITION	pos1;
	MhPlcInfo	*PlcEn;
	MsBitSet*	pVer;

	for ( PlcEn = HaitiDb::MdGetHeadPts( &pos1); PlcEn!=0; PlcEn = HaitiDb::MdGetNextPts( &pos1)) {
		pVer = PlcEn->GetPIVer();
		if ( pVer) {
			if ( *pVer && z_Verv.vVerB) {
				*pVer |= z_Verv.vVerC;
				PlcEn->SetPIVer( pVer);
			}
		}
	}
	WindowCtrl::MmWndKReDraw();
exit:
	*o_pnVer = z_Verv.nVer;
	*o_ppsVerC = z_Verv.sVerC;
	*o_ppsVerB = z_Verv.sVerB;
	*o_ppsVerD = z_Verv.sVerD;
	return ist;
}

/////////////////////////////////////////////////////////////////////////////
//【機能】バージョン名変更
//【返値】ステイタス	0: 正常	-1: 旧バージョン無し	-2: 新バージョンが既に有り
MINT MhHist::MmVerCng(
						CString		i_sVerB,
						CString		i_sVerC,
						MINT*		o_pnVer,
						CString*	*o_ppsVerC,
						CString*	*o_ppsVerB,
						CString*	*o_ppsVerD
				)
{
	MINT		ist = 0;
	MINT		ic, icf;
	MhHistV		VerB;
	MhHistV		VerC;
	for ( ic=0; ic<z_Verv.nVer; ic++)
		if ( i_sVerB == z_Verv.sVerC[ic] || i_sVerC == z_Verv.sVerB[ic])
			break;
	if ( ic == z_Verv.nVer) {
		ist = -1;									// 旧バージョンなし
		MQUIT;
	}
	icf = ic;
	for ( ic=0; ic<z_Verv.nVer; ic++)
		if ( i_sVerC == z_Verv.sVerC[ic] || i_sVerC == z_Verv.sVerB[ic])
			break;
	if ( ic < z_Verv.nVer) {
		ist = -2;									// 新バージョンが既に有り
		MQUIT;
	}
	for ( ic=0; ic<z_Verv.nVer; ic++) {
		if ( i_sVerB == z_Verv.sVerC[ic])
			z_Verv.sVerC[ic] = i_sVerC;
		if ( i_sVerB == z_Verv.sVerB[ic])
			z_Verv.sVerB[ic] = i_sVerC;
	}
	*o_pnVer = z_Verv.nVer;
	*o_ppsVerC = z_Verv.sVerC;
	*o_ppsVerB = z_Verv.sVerB;
	*o_ppsVerD = z_Verv.sVerD;

	z_Verv.vVerC.SetMask( icf);
	WindowCtrl::MmWndKReDraw();

exit:
	return ist;
}

/////////////////////////////////////////////////////////////////////////////
//【機能】バージョン選択
//【返値】ステイタス	0: 正常	-1: 選択バージョン無し
MINT MhHist::MmSetVer(
						CString		sVerB,		//
						CString		sVerC,		//
						MINT*		m_pnVer,	//
						CString*	*m_ppsVerC,	//
						CString*	*m_ppsVerB,	//
						CString*	*m_ppsVerD	//
				)
{
	MINT		ist = 0;
	MINT		ic, icf;
	MhHistV		VerB;
	MhHistV		VerC;
	for ( ic=0; ic<z_Verv.nVer; ic++)
		if ( sVerC == z_Verv.sVerC[ic])
			break;
	if ( ic == z_Verv.nVer) {
		ist = -1;									// 選択バージョンなし
		MQUIT;
	}
	icf = ic;
	*m_pnVer = z_Verv.nVer;
	*m_ppsVerC = z_Verv.sVerC;
	*m_ppsVerB = z_Verv.sVerB;
	*m_ppsVerD = z_Verv.sVerD;

	z_Verv.vVerC.SetMask( icf);
	WindowCtrl::MmWndKReDraw();
exit:
	return ist;
}

/////////////////////////////////////////////////////////////////////////////
//【機能】カレントバージョンを取得する
//【返値】バージョン有無
//			TRUE : 初期
//			FALSE: 初期以外のバージョン設定あり
bool MhHist::MmVerGetC(
						MsBitSet	*o_pHst		// カレントバージョン
				)
{
//	MsBitSet	z_Verv;

	if ( z_Verv.fcStd) 
		*o_pHst = MsBitSet( 1, 0, 0);							// 初期
	else
		*o_pHst = z_Verv.vVerC;

//	TRACE1( "MmHistGet	z_Verv.vVerC %x \n", z_Verv.vVerC.GetVer());

	return z_Verv.fcStd;
}

/////////////////////////////////////////////////////////////////////////////
//【機能】ＤＢレコードの履歴管理がカレントバージョンを含むか調べる
//【返値】カレントバージョン含みフラグ
//			TRUE : 含む
//			FALSE: 含まない
bool MhHist::MmVerChkC(
						MsBitSet	*i_pHstv	// ＤＢレコードのOPT群セット
				)
{
	bool bst;

	if ( i_pHstv == 0){											// DB ＯＰＴ群　標準
		bst = TRUE;

	} else {
		if ( (*i_pHstv) && z_Verv.vVerC)
			bst = TRUE;
		else
			bst = FALSE;

//		MUINT	*iOptv = pOptv->GetSOB();
//		TRACE3( "MhOptV::MmOptChkC	DB Opt %x, %x, %x		", iOptv[0], iOptv[1], iOptv[2]);
//		iOptv = z_Optv.bmCurRGOptSet.GetSOB();
//		TRACE3( "z_Optv.bmCurRGOptSet %x, %x, %x		", iOptv[0], iOptv[1], iOptv[2]);
//		if ( bst) TRACE0 ( "bst TRUE\n"); else  TRACE0 ( "bst FALSE\n"); 
	}
	return bst;
}

/////////////////////////////////////////////////////////////////////////////
//【機能】ＤＢレコード修正時のレコード処理タイプを取得する
//		　（修正元のバージョンの履歴管理とカレントバージョン該当の履歴管理を
//		　	比較してどのような修正処理のレコードであるか調べる）
//【返値】修正後のレコード構成コード
//			1: 修正元レコードの履歴管理 ＝ 書込み対象の履歴管理
//				バージョンの変更は不要で、属性修正のみ変更する
//			3: その他 ( 修正元レコードの履歴管理 ⊃ カレントバージョン該当の履歴管理 )
//				修正元レコードはバージョンのみ変更し、新しく書込み対象の履歴管理のバージョンを持つ
//				コピーレコードを作製しその属性を変更する
//

MINT MhHist::MmHistModChk(							// 修正後のレコード構成コード
						MsBitSet*	*io_ppHstvMod,	// 修正元レコードに設定する履歴管理　(修正後のレコード構成コード >= 2 の時有効)
						MsBitSet*	*o_ppHstvAdd	// 修正用に追加レコードの履歴管理　(修正後のレコード構成コード == 3 の時有効)
				)
{
	MINT		ist;
	MsBitSet	*pHstvAdd;

	if ( (*io_ppHstvMod == NULL && z_Verv.fcStd) ||					// カレントバージョン == 初期で修正元レコードの履歴管理も初期
		(*io_ppHstvMod != NULL && **io_ppHstvMod == z_Verv.vVerC)) {	// または、修正元レコードの履歴管理 == 書込み履歴管理
		ist = 1;
	} else {
		if ( *io_ppHstvMod == NULL) {
			*io_ppHstvMod = (MsBitSet*)new char[sizeof(MsBitSet)];	// 修正元のレコードに履歴管理を追加
			**io_ppHstvMod = ~z_Verv.vVerC;							// 全バージョン － 書込み対象バージョン
		} else {
//			*ppHstvMod = pHstvR;									// 修正元のレコードの履歴管理を修正
			**io_ppHstvMod = **io_ppHstvMod & ~z_Verv.vVerC;		// 修正元バージョン － 書込み対象バージョン
		}

		if ( (**io_ppHstvMod).IsNull()) {							// 修正元レコードの履歴管理 ⊂ 書込み対象バージョン
			ist = 2;
//			**ppHstvMod = z_Verv.vVerC;								// 修正対象の履歴管理設定
			ASSERT( false);											// 履歴管理　カレントバージョンエラー　<ERROR>
		} else {													// その他 修正元レコードの履歴管理 ⊃ （書込み履歴管理 ∩ 修正元レコードの履歴管理)
			pHstvAdd = (MsBitSet*)new char[sizeof(z_Verv.vVerC)];
			*pHstvAdd = z_Verv.vVerC;								// 修正対象の履歴管理設定
			*o_ppHstvAdd = pHstvAdd;
			ist = 3;
		}
	}
	return ist;
}

/////////////////////////////////////////////////////////////////////////////
//【機能】レコード削除時に設定するバージョンコードを取得する
//【返値】削除後のレコード構成コード
//			1 : 削除元バージョンは書込みバージョンと同一の場合で、
//				削除元レコードは削除する。
//			3 : 削除元バージョンに書込みバージョン以外のバージョンを含む場合で、
//				カレントバージョンのみ削除する。
//				カレントバージョン以外のバージョン部分は、削除元レコードをそのまま残す。（バージョンコードのみ修正）
//				（バージョンコードが入るエリアの開放と確保は本関数で行う）
//
MINT MhHist::MmHistDelChk(							// 削除後のレコード構成コード
//						MsBitSet	*i_pHstvR,		// 削除元レコードの履歴管理
//													// 削除後レコードの履歴管理に変更する
						MsBitSet*	*io_ppHstvMod	// 削除後のレコードの履歴管理
				)
{
	MINT		ist;
//	MsBitSet	HstvMod;

	if ( z_Verv.fcStd && *io_ppHstvMod == NULL ||				// カレントバージョン == 初期で、(削除元レコードの履歴管理も初期
		(*io_ppHstvMod != NULL &&								// または、削除元レコードの履歴管理 == 書込み履歴管理)
		 (**io_ppHstvMod) == z_Verv.vVerC)) {
		ist = 1;
	} else {
		if ( *io_ppHstvMod == NULL) {
			*io_ppHstvMod =  (MsBitSet*)new char[sizeof(z_Verv.vVerC)];	// 削除後のレコードに表示ＯＮ履歴管理を追加
			**io_ppHstvMod = ~z_Verv.vVerC;						// 、設定
		} else {
			**io_ppHstvMod = **io_ppHstvMod & ~z_Verv.vVerC;	// 削除元のレコードの履歴管理 － 書込み履歴管理
		}

		if ( (**io_ppHstvMod).IsNull()) {						// 削除元レコードの履歴管理 ⊂ 書込み履歴管理
			ist = 2;
			ASSERT( false);										// 履歴管理　カレントバージョンエラー　<ERROR>
		} else {
			ist = 3;
		}
	}
	return ist;
}

} // namespace MC
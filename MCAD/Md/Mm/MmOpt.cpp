//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MmDrag.cpp
//
//		ドラッギング操作
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
#include "MbList.h"
#include "MhLib.h"

#include "MhInp.h"

#define DLL_EXPORT_MHOPTV_DO
#include "MmOpt.h"
#include "MdLib.h"

#define	MH_ALL_BITS		0xFFFFFFFF

namespace MC
{

static	MhOptV	z_Optv;
/////////////////////////////////////////////////////////////////////////////
//	ＯＰＴ群管理情報
//  スタティック属性値
bool		MhOptV::fcDB;						// カレント選択ＯＰＴ群をDBへ反映フラグ
												//		(TRUE:反映済、FALSE: 未反映)
bool		MhOptV::fcStd;						// カレント選択ＯＰＴ群==標準　フラグ
												//		(TRUE:標準、FALSE:標準以外のＯＰＴ群有り)
bool		MhOptV::fDispStd;					// カレント選択標準表示ＯＮフラグ
												//		(TRUE:標準表示ＯＮ、FALSE:標準表示ＯＦＦ)
MhOpt		MhOptV::bmCurGOpt;					// カレント選択ＯＰＴ群
MsBitSet	MhOptV::bmCurRGOptSet;				// カレント選択ＯＰＴ群で読込み対象のOPT群セット
MsBitSet	MhOptV::bmCurWGOptSet;				// カレント選択ＯＰＴ群で書込み対象のOPT群セット
MsBitSet	MhOptV::bmCurGOptSet;				// カレント選択ＯＰＴ群相当のOPT群セット
MINT		MhOptV::nbmGOpt;					// 登録OPT群数
MhOpt		MhOptV::bmGOpt[MSZ_OPTVB];			// 登録OPT群

/////////////////////////////////////////////////////////////////////////////
//【機能】ＯＰＴ群管理情報を初期化する
void MhOptV::MmOptInit()
{
	z_Optv.fcDB = FALSE;										// 
	z_Optv.fcStd = TRUE;

	z_Optv.fDispStd = TRUE;
	z_Optv.bmCurGOpt = MhOpt( 0);
	z_Optv.bmCurGOptSet = MsBitSet( 0, 0, 0);
	z_Optv.bmCurRGOptSet = z_Optv.bmCurWGOptSet = MsBitSet( 0, 0, 0);
	z_Optv.nbmGOpt = 1;											// 標準のみ登録済
	z_Optv.bmGOpt[0] = MhOpt( 1);								// 標準のみ登録済
}

/////////////////////////////////////////////////////////////////////////////
//【機能】カレント選択ＯＰＴ群で読込み対象のOPT群セットを取得する
MsBitSet	MhOptV::MhOptBScRGet()
{
	return z_Optv.bmCurRGOptSet;
}
						
/////////////////////////////////////////////////////////////////////////////
//【機能】カレント選択ＯＰＴ群で書込み対象のOPT群セットを取得する
MsBitSet	MhOptV::MhOptBScWGet()
{
	return z_Optv.bmCurWGOptSet;
}
						
/////////////////////////////////////////////////////////////////////////////
//【機能】カレント選択ＯＰＴ群相当のOPT群セットを取得する
MsBitSet	MhOptV::MhOptBScGet()
{
	return z_Optv.bmCurGOptSet;
}

/////////////////////////////////////////////////////////////////////////////
//【機能】ＯＰＴ群選択チェックボックスの選択状況をカレント選択ＯＰＴ群に反映する
void MhOptV::MmOptSet(
						MINT		i_nOpt,		// チェックボックス番号
												//		0 : 標準表示オフ
												//	   >0 : ＯＰＴ群番号
						MINT		i_iChk		// チェックボックス選択状況
				)
{
	MsBitSet	OptvWDumy;
	MINT		iOptN;
	MhOpt		bOpt;

	if ( i_nOpt == 0) {
		if ( i_iChk == BST_CHECKED) {
			z_Optv.fDispStd = FALSE;
		} else {
			z_Optv.fDispStd = TRUE;
		}
	} else {
		iOptN = i_nOpt;
		bOpt = 1 << iOptN;
		if ( i_iChk == BST_CHECKED) {
			z_Optv.bmCurGOpt |= MhOpt( bOpt);					// カレント選択ＯＰＴ群に該当オプションを設定
		} else {
			z_Optv.bmCurGOpt &= ~bOpt;							// カレント選択ＯＰＴ群の該当オプションを消去
		}
	}
	z_Optv.fcStd = MhOptV::MmOptIncSrch( z_Optv.bmCurGOpt);
	z_Optv.fcDB = FALSE;										// 変更

//D	TRACE1( "MhOptV::MmOptSet ********	z_Optv.cb %x		", z_Optv.cb);
//D	MUINT	*iOptv = z_Optv.bmCurRGOptSet.GetSOB();
//D	TRACE3( "z_Optv.bmCurRGOptSet %x, %x, %x\n", iOptv[0], iOptv[1], iOptv[2]);

	WindowCtrl::MmWndKReDraw();
/*
//-------------------   ３次元表示  -----------------------------------

	//　３次元表示は一旦消す
	MmWndInfo* pWndInfo = MmWndKFind( 2, MTHEIMENZU, 1, 1);
	if ( pWndInfo) 
		pWndInfo->m_pChildFrm->SendMessage(WM_CLOSE);

	//	モデルを読み込み表示する
//	ist1 = MhMdlLoad( PathO);
//	MmWndKReDrawAll();

	//　３次元表示がされていた場合は新しいデータで再度表示する
	if ( pWndInfo) 
		g_theApp.CreateWnd( 2, MTHEIMENZU, 1, 1);
//		pWndInfo->m_pChildFrm->SendMessage(WM_CREATE);
//		g_theApp.OnView2On();

//------------------------------------------------------
*/
}

/////////////////////////////////////////////////////////////////////////////
//【機能】カレント選択ＯＰＴ群を取得する
//【返値】ＯＰＴ群有無
//			TRUE : 標準
//			FALSE: 標準以外のオプション設定あり
bool MhOptV::MmOptGetC(
						MhOpt*		o_pOpt		// カレント選択ＯＰＴ群
				)
{
	if ( z_Optv.fcStd) 
		*o_pOpt = MhOpt( 1);									// 標準
	else
		*o_pOpt = z_Optv.bmCurGOpt;

//D	TRACE1( "MmOptGet	z_Optv.cb %x \n", z_Optv.cb.GetOpt());

	return z_Optv.fcStd;
}

/////////////////////////////////////////////////////////////////////////////
//【機能】ＤＢレコードのOPT群セットがカレント選択ＯＰＴ群を含むか調べる
//【返値】選択ＯＰＴ群含みフラグ
//			TRUE : 含む
//			FALSE: 含まない
bool MhOptV::MmOptChkC(
						MsBitSet*	i_pOptv		// ＤＢレコードのOPT群セット
				)
{
	bool bst;
	MsBitSet	Optv;

	if ( i_pOptv == 0){											// DB ＯＰＴ群　標準
		if ( z_Optv.fDispStd)									// カレント選択標準表示ＯＮフラグ
			bst = TRUE;
		else
			bst = FALSE;

	} else {
		if ( ( *i_pOptv) && z_Optv.bmCurRGOptSet)
			bst = TRUE;
		else
			bst = FALSE;

//D		MUINT	*iOptv = pOptv->GetSOB();
//D		TRACE3( "MhOptV::MmOptChkC	DB Opt %x, %x, %x		", iOptv[0], iOptv[1], iOptv[2]);
//D		iOptv = z_Optv.bmCurRGOptSet.GetSOB();
//D		TRACE3( "z_Optv.bmCurRGOptSet %x, %x, %x		", iOptv[0], iOptv[1], iOptv[2]);
//D		if ( bst) TRACE0 ( "bst TRUE\n"); else  TRACE0 ( "bst FALSE\n"); 
	}
	return bst;
}

/////////////////////////////////////////////////////////////////////////////
//【機能】ＤＢレコードの表示ＯＦＦOPT群セットがカレント選択ＯＰＴ群を含むか調べる
//【返値】選択ＯＰＴ群含みフラグ
//			TRUE : 含む		表示ＯＦＦ
//			FALSE: 含まない	表示ＯＮ
bool MhOptV::MmOptOffChkC(
						MsBitSet*	i_pOptv		// ＤＢレコードのOPT群セット
				)
{
	bool	bst;

	if ( i_pOptv == 0){											// DB ＯＰＴ群　標準
			bst = FALSE;

	} else {
		if ( ( *i_pOptv) && z_Optv.bmCurRGOptSet)
			bst = TRUE;
		else
			bst = FALSE;

//D		MUINT	*iOptv = pOptv->GetSOB();
//D		TRACE3( "MhOptV::MmOptOffChkC	DB Opt %x, %x, %x		", iOptv[0], iOptv[1], iOptv[2]);
//D		iOptv = z_Optv.bmCurRGOptSet.GetSOB();
//D		TRACE3( "z_Optv.bmCurRGOptSet %x, %x, %x \n", iOptv[0], iOptv[1], iOptv[2]);
	}

	return bst;
}

/////////////////////////////////////////////////////////////////////////////
//【機能】ＤＢレコード修正後のレコード構成コードを取得する
//		　（修正元のＯＰＴ群のOPT群セットとカレント選択ＯＰＴ群の書込み対象OPT群セットを
//		　	比較してどのような修正処理のレコードであるか調べる）
//【返値】修正後のレコード構成コード
//			1: 修正元レコードのOPT群セット ＝＝ 書込み対象のOPT群セット
//				ＯＰＴ群の変更は不要で、属性修正のみ変更する
//			2: 修正元レコードのOPT群セット ⊂ 書込み対象のOPT群セット
//				ＯＰＴ群の変更と属性変更する→ＯＰＴ群の変更は不要で、属性修正のみ変更する（1と同じ）
//			3: その他 ( 修正元レコードのOPT群セット ⊃ 書込み対象のOPT群セット || 部分的に重なっている )
//				（修正元レコードのOPT群セット ⊃ （書込み対象のOPT群セット ∩ 修正元レコードのOPT群セット)）
//				修正元レコードはＯＰＴ群を変更し、新しく書込み対象のOPT群セットのＯＰＴ群を持つ
//				コピーレコードを作製しその属性を変更する
//
MINT MhOptV::MmOptModChk(									// 修正後のレコード構成コード
						MsBitSet*	*io_ppOptvRN,	// 修正前後レコードのOPT群セット　	(修正後のレコード構成コード >= 2 の時有効)
						MsBitSet*	*o_ppOptvWN,	// 修正用に追加レコードのOPT群セット　(修正後のレコード構成コード == 3 の時有効)
						MhOptV*		*o_ppOptv		// ＯＰＴ群管理情報
				)
{
	MINT		ist;
	MsBitSet	*pOptvR;
	MsBitSet	OptvRN;

	pOptvR = *io_ppOptvRN;
	if ( ( pOptvR == NULL && z_Optv.fcStd) ||					// カレント選択ＯＰＴ群 == 標準 (この場合は標準のみ表示されており削除元レコードも標準)
		(pOptvR != NULL &&										// カレント選択ＯＰＴ群 == 修正元レコードＯＰＴ群
		 *pOptvR == z_Optv.bmCurWGOptSet)) {
		ist = 1;
	} else {
		if ( pOptvR == NULL) {
			*io_ppOptvRN = ( MsBitSet*)new char[sizeof( z_Optv.bmCurWGOptSet)];	// 修正元のレコードにOPT群セットを追加
			**io_ppOptvRN = ~z_Optv.bmCurWGOptSet;
			ist = 3;
		} else {
			OptvRN = *pOptvR & ~z_Optv.bmCurWGOptSet;			// 修正元ＯＰＴ群 － 書込み対象のOPT群セット

			if ( OptvRN.IsNull()) {								// 修正元レコードのOPT群セット ⊂ 書込み対象のOPT群セット
				ist = 2;
				goto exit;
			} else {											// その他 修正元レコードのOPT群セット ⊃ （書込み対象のOPT群セット ∩ 修正元レコードのOPT群セット)
				**io_ppOptvRN = OptvRN;
				ist = 3;
			}
		}
		*o_ppOptvWN = ( MsBitSet*)new char[sizeof( z_Optv.bmCurWGOptSet)];
		**o_ppOptvWN = z_Optv.bmCurWGOptSet;					// 修正対象のOPT群セット設定
		*o_ppOptv = &z_Optv;
	}
exit:
	return ist;
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
//				（本関数で**ppOptvRN,**ppOptvDNを修正)
//				（ＯＰＴ群コードが入るエリアの開放と確保は本関数で行う）
//
MINT MhOptV::MmOptDelChk(							// 削除後のレコード構成コード
						MsBitSet*	*io_ppOptvRN,	// 削除前後のレコードの表示ＯＮOPT群セット
						MsBitSet*	*o_ppOptvDN		// 削除前後のレコードの表示ＯＦＦOPT群セット
				)
				
				
{
	MINT		ist;
	MsBitSet	*pOptvR;
	MsBitSet	OptvRN;

	pOptvR = *io_ppOptvRN;
	if ( z_Optv.fcStd ||											// カレント選択ＯＰＴ群 == 標準 (この場合は標準のみ表示されており削除元レコードも標準)
		( pOptvR != NULL &&											// カレント選択ＯＰＴ群 ＝ 削除元レコードのOPT群セット
		  *pOptvR == z_Optv.bmCurWGOptSet)) {
		ist = 1;
	} else {
		if ( pOptvR == NULL) {
			 *io_ppOptvRN = ( MsBitSet*)new char[sizeof( z_Optv.bmCurWGOptSet)];	// 削除後のレコードに表示ＯＮOPT群セットを追加
			 **io_ppOptvRN = ~z_Optv.bmCurWGOptSet;					// 、設定
			ist = 3;
		} else {
//D			if ( *pOptvR == z_Optv.bmCurWGOptSet) {					// 書込み対象のOPT群セット == 削除元レコードのOPT群セット
//D				ist = 1;
//D				goto exit;
//D			} else { 
				OptvRN = *pOptvR & ~z_Optv.bmCurWGOptSet;			// ( 削除元のレコードのOPT群セット － 書込み対象のOPT群セット)

				if ( OptvRN.IsNull()) {								// 書込み対象のOPT群セット ⊃ 削除元レコードのOPT群セット
					ist = 2;
					goto exit;
				} else {											// その他
					**io_ppOptvRN = OptvRN;							// 削除元のレコードのOPT群セットを修正
					ist = 3;
				}
//D			}
		}

		if ( *o_ppOptvDN == NULL) {
			 *o_ppOptvDN = (MsBitSet*)new char[sizeof( z_Optv.bmCurWGOptSet)];	// 削除後のレコードに表示ＯＦＦOPT群セットを追加
			 **o_ppOptvDN = z_Optv.bmCurWGOptSet;					// 、設定
		} else {
			**o_ppOptvDN |= z_Optv.bmCurWGOptSet;					// 削除後のレコードに表示ＯＦＦOPT群セットを設定
		} 
	}
exit:
	return ist;
}

/////////////////////////////////////////////////////////////////////////////
//【機能】登録OPT群より選択ＯＰＴ群を検索し無ければ追加登録する
//		　選択ＯＰＴ群を全て含む登録OPT群の集まりに相当する
//		  OPT群セットを追加する
//【返値】検索状況
//			0:無しであり追加
//			1:有り
//
MINT MhOptV::MmOptSrch(
						MhOpt		i_Opt		// 検索ＯＰＴ群
				)
{
	MINT	ist;
	MINT	id;

	ist = 1;
	if ( !z_Optv.fcDB) {										// カレント選択ＯＰＴ群に変更あり
		for ( id=0; id<z_Optv.nbmGOpt; id++) {					// カレント選択ＯＰＴ群を{登録OPT群}より検索
			if ( i_Opt == z_Optv.bmGOpt[id])
				break;
		}

		if ( id >= z_Optv.nbmGOpt) {							// 未登録であり登録する
			ASSERT( id < 96);									// 登録OPT群オーバーフローエラー　<ERROR>
			z_Optv.bmGOpt[id] = i_Opt;
			z_Optv.nbmGOpt++;
			ist = 0;
		}

		z_Optv.bmCurGOptSet.SetMask( id);
	}
	return ist;
}

/////////////////////////////////////////////////////////////////////////////
//【機能】選択ＯＰＴ群で読込み対象のOPT群セットと
//		  書込み対象のOPT群セットを求める
//【返値】
//			TRUE  : 選択ＯＰＴ群は標準
//			FALSE : 選択ＯＰＴ群に標準以外のオプション設定あり
//
bool MhOptV::MmOptIncSrch(
						MhOpt		i_Opt		// 選択ＯＰＴ群
				)
{
	bool		bst;
	MINT		ic;
	MINT		id;
	MsBitSet	OptvF;
	
	MINT		nOptIdB = 0;					// 読込み対象ＯＰＴ群数
	MINT		iOptIdB[96];					// 読込み対象ＯＰＴ群の集合

	if ( i_Opt.IsStd()) {										// 選択ＯＰＴ群==標準(0|1)
		bst = TRUE;
		if ( z_Optv.fDispStd) 									// 標準表示ＯＮ
			z_Optv.bmCurRGOptSet = MsBitSet( 1, 0, 0);			//		読込み対象のOPT群セット = ( 1, 0, 0)
		else													// 標準表示ＯＦＦ
			z_Optv.bmCurRGOptSet = MsBitSet( 0, 0, 0);			//		読込み対象のOPT群セット = ( 0, 0, 0)
		z_Optv.bmCurWGOptSet = MsBitSet( 0, 0, 0);				// 書込み対象のOPT群セット = ( 0, 0, 0)
	} else {
	// 読込み対象のOPT群セットを求める
		for ( id=0; id<z_Optv.nbmGOpt; id++) {					// 登録OPT群よりカレント選択ＯＰＴ群に含まれるものを全て求める
			if ( z_Optv.bmGOpt[id] == i_Opt) {					// 等しいものがある場合は、そのオプション群のみを読み込み対象とする
				iOptIdB[0] = id;
				nOptIdB = 1;
				break;
			} else {
				if ( z_Optv.bmGOpt[id] == ( i_Opt & z_Optv.bmGOpt[id]))	// 登録OPT群[id]がカレント選択ＯＰＴ群に含まれる
					iOptIdB[nOptIdB++] = id;
			}
		}
		z_Optv.bmCurRGOptSet = MsBitSet( 1, 0, 0);
		for ( ic=0; ic<nOptIdB; ic++) {
			if ( iOptIdB[ic] >= 0) {							// 選択ＯＰＴ群に含まれる全登録OPT群相当のOPT群セット取得
				OptvF.SetMask( iOptIdB[ic]);
				z_Optv.bmCurRGOptSet |= OptvF;
			}
		}
		// 書込み対象のOPT群セットを求める
		z_Optv.bmCurWGOptSet = MsBitSet( 0, 0, 0);
		for ( id=0; id<z_Optv.nbmGOpt; id++) {
			if ( i_Opt == (z_Optv.bmGOpt[id] & i_Opt)) {		// 選択ＯＰＴ群を含む全登録OPT群相当のOPT群セット取得
				OptvF.SetMask( id);
				z_Optv.bmCurWGOptSet |= OptvF;
			}
		}
		bst = FALSE;
	}

	return bst;
}

/////////////////////////////////////////////////////////////////////////////
// カレント選択ＯＰＴ群に相当のOPT群セットをDBの参照対象レコードへ反映
void MhOptV::MmOptAddDB()
{
	MPOSITION	pos1;
	MhPlcInfo*	pPlcEn;
	MsBitSet	*pOptv;

	MINT	iDB = 0;
	for ( pPlcEn = HaitiDb::MdGetHeadPts( &pos1); pPlcEn!=0; pPlcEn = HaitiDb::MdGetNextPts( &pos1)) {
		pOptv = pPlcEn->GetPIOpt1();
		if ( pOptv == NULL) {		// オプション設定無しの参照対象レコード
		} else if ( MhOptV::MmOptChkC( pOptv)) {						// ＯＰＴ群有りの参照対象レコード
			*pOptv |= z_Optv.bmCurGOptSet;
		} else {
		}


		pOptv = pPlcEn->GetPIOpt2();
		if ( pOptv == NULL) {		// オプション設定無しの参照対象レコード
		} else if ( MhOptV::MmOptOffChkC( pOptv)) {						// ＯＰＴ群有りの参照対象レコード
			*pOptv |= z_Optv.bmCurGOptSet;
		} else {
		}

	}
	z_Optv.fcDB = TRUE;											// ＤＢオプション反映済み
}

} // namespace MC
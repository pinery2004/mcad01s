//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MmsDialogBarPts.cpp
//
//		
//	1.4	部材選択
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MCAD.h"
#include "MainFrm.h"

#define DLL_EXPORT_MC_INPATTR_DO
#include "MmLib.h"
#include "MmDefine.h"
#include "MhInp.h"
#include "MhLib.h"
#include "MmPrompt.h"

namespace MC
{

// 部材選択用コンボボックス
static MINT z_nComboIdTpPts;									// 表示項目数
static MINT	z_iComboIdTpPts[MX_CMB_PARTSNM];					// 表示項目対応部材ID
static MINT z_iComboKmIdTpPts;								// カレント選択項目番号

// 寸法型式選択用コンボボックス
static MINT z_nComboMbrId;									// 表示項目数
static MINT	z_iComboMbrId[MX_CMB_CDMEMBER];					// 表示項目対応寸法型式ID
static MINT z_iComboKmIdMbr;									// カレント選択項目番号

///////////////////////////////////////////////////////////////////////////////
//	部材選択用コンポボックスに部材名の項目(一覧)を設定し
//	対応する部材ＩＤテーブルを作成する
//
//	カレントの構成(組)コード( 屋根、小屋、天井、耐力壁、壁、床、基礎) と
//	分類コード("部材", "金物", "パネル", "屋根", "壁", "線分", "区画", "開口", "注記", "その他") とが
//	一致する部材のみ設定する

void InpAttr::InitComboTpPtsAttr()
{
	MhTpPts	*pTpPts;
	MINT		ic1;
	MINT		ip = 0;

	CComboBox *pCmb = MmpComboCdBuzai();
	pCmb->ResetContent();

	MINT mxPtsNm = BuzaiCode::MhGetNoOfTpPts();
	for ( ic1=0; ic1<mxPtsNm; ic1++) {
		pTpPts = BuzaiCode::MhGetpTpPts( ic1);
		if ( InpMode::GetKGp() != pTpPts->GetPTCdGp())	// 構成(組)コードがカレントと不一致は無視
			continue;
		if ( InpMode::GetKBr() == MP_BR_OTHER) {			// 選択した分類がその他の場合
			if ( pTpPts->GetPTBr() == MP_BR_BUZAI ||	// "部材"、
				pTpPts->GetPTBr() == MP_BR_KANAGU ||	// "金物"、
				pTpPts->GetPTBr() == MP_BR_PANEL)		// "パネル"は無視
				continue;
		} else {												// 選択した分類が"部材", "金物", "パネル"の場合は
			if ( InpMode::GetKBr() != pTpPts->GetPTBr())	//		分類コードコードがカレントと不一致は無視
				 continue;
		}
		ASSERT( ip < MX_CMB_PARTSNM - 1);						// 部材コンボボックス項目　オーバフロー　<ERROR>
		z_iComboIdTpPts[ip++] = ic1;							// 部材ＩＤをテーブルに設定
		pCmb->InsertString( -1, pTpPts->GetPTNmPts1());	// 部材名を項目に追加
	}
	z_nComboIdTpPts = ip;
	if ( ip == 0)
		pCmb->InsertString( -1, Mstr( "none"));
	z_iComboKmIdTpPts = -1;
	InpMode::SetKBrB( false, false, false, false);			//	部材、金物、パネル、その他のラジオボタンを未選択に設定

//	MmSetComboMbr();
}

///////////////////////////////////////////////////////////////////////////////
//	部材名より部材選択項目番号を取得する
//	コンボボックスの項目より指定された部材の部材IDを検索し、
//	該当の部材選択項目番号を返す
//	返値
//							≧ 0: 部材ID
//							＝-1: 該当なし
static MINT MmGetKmIdTpPts( MCHAR* i_pcNmPts1)
{
	MINT		iKmIdTpPts;
	MINT		ic1;
	MINT		iIdTpPts;
	MhTpPts	*pTpPts;
	
	for ( ic1=0; ic1<z_nComboIdTpPts; ic1++) {
		iIdTpPts = z_iComboIdTpPts[ic1];
		pTpPts = BuzaiCode::MhGetpTpPts( iIdTpPts);
		if ( Mstrcmp( pTpPts->GetPTNmPts1(), i_pcNmPts1) == 0)
			break;
	}
	if ( ic1 < z_nComboIdTpPts)
		iKmIdTpPts = ic1;
	else
		iKmIdTpPts = -1;
	return iKmIdTpPts;
}

///////////////////////////////////////////////////////////////////////////////
//	コンボボックスに指定した部材選択項目番号の部材名を表示し
//	カレントの部材選択項目番号を設定する

void InpAttr::SetComboKmIdTpPts(
						MINT	iKmIdTpPts		// (I  ) 部材選択項目番号
				)
{
	System::GetpMainFrame()->SetCombo1( iKmIdTpPts);
	z_iComboKmIdTpPts = iKmIdTpPts;

	// 部材入力属性の設定
	MINT iIdTpPts = z_iComboIdTpPts[z_iComboKmIdTpPts];
	MhPlcInp::SetIdTpPts( iIdTpPts);
	MhTpPts *pTpPts = BuzaiCode::MhGetpTpPts( iIdTpPts);
	MhPlcInp::SetpTpPts( pTpPts);

	MINT iCdPlc = pTpPts->GetPTCdPlc();
	HaitiIn::SetComboCdPlc( iCdPlc);
}

///////////////////////////////////////////////////////////////////////////////
//	コンボボックスに指定した部材名を表示しその部材IDを取得する
//	カレントの部材選択項目番号を設定する
//	返値
//							≧ 0: 部材ID
//							＝-1: 該当なし
MINT InpAttr::SetComboCdTpPts( MCHAR* i_pcNmPts1)
{
	MINT 	iIdTpPts = -1;
	MINT	iKmIdTpPts;

	iKmIdTpPts = MmGetKmIdTpPts( i_pcNmPts1);
	if ( iKmIdTpPts < 0) {
		Msg::ErrorMsg( i_pcNmPts1, MC_ERR_NOTSET_PARTS_TABLE);	// "が部品テーブルに未設定です．"
		MQUIT;
	}
	InpAttr::SetComboKmIdTpPts( iKmIdTpPts);

	iIdTpPts = z_iComboIdTpPts[iKmIdTpPts];
exit:
	return iIdTpPts;
}

///////////////////////////////////////////////////////////////////////////////
//	部材名より部材IDを取得する
//							＝-1: 未選択
//							≧ 0: 部材ID
MINT InpAttr::GetIdTpPts( MCHAR* i_pcNmPts1)
{
	MINT		ist;
	MINT		ic1;
	MhTpPts	*pTpPts;

	MINT	mxPtsNm = BuzaiCode::MhGetNoOfTpPts();
	for ( ic1=0; ic1<mxPtsNm; ic1++) {
		pTpPts = BuzaiCode::MhGetpTpPts( ic1);
		if ( Mstrcmp( pTpPts->GetPTNmPts1(), i_pcNmPts1) == 0)
			break;
	}
	if ( ic1 < mxPtsNm)
		ist = ic1;
	else
		ist = -1;
	return ist;
}

///////////////////////////////////////////////////////////////////////////////
//	カレントの部材IDを取得する
//							＝-1: 該当なし
//							≧ 0: 部材ID
MINT InpAttr::GetCurIdTpPts()
{
	MINT iIdTpPts;

	if ( z_iComboKmIdTpPts >= 0 && z_iComboKmIdTpPts < z_nComboIdTpPts) {
		iIdTpPts = z_iComboIdTpPts[z_iComboKmIdTpPts];
	} else {
		iIdTpPts = -1;
	}
	return iIdTpPts;
}

///////////////////////////////////////////////////////////////////////////////
//	寸法型式選択用コンポボックスに寸法型式の項目(一覧)を設定し
//	対応する寸法型式ＩＤテーブルを作成する
//
//	カレントの部材の寸法型式使用部位コードを持つ寸法型式のみ設定

void InpAttr::InitComboPtsMbr()
{
	MhMbr	*pMbr;
	MINT		ic1;
	MINT		ic;
	MINT		ip = 0;

	CComboBox *pCmb = MmpComboMbr();
	pCmb->ResetContent();

	MINT		iIdTpPts = InpAttr::GetCurIdTpPts();
	if ( iIdTpPts == -1) {
//		pCmb->>EnableToolTips(0);			NG
		goto exit;
	}

	MCHAR		cMTp1 = BuzaiCode::MhGetpTpPts( iIdTpPts)->GetPTTpMbr()[0];
	MCHAR		cMTp2;

	MINT mxMbr = BuzaiCode::MhGetNoOfMbr();
	for ( ic1=0; ic1<mxMbr; ic1++) {
		pMbr = BuzaiCode::MhGetpMbr( ic1);

		for ( ic=0; cMTp2=pMbr->GetMbType()[ic]; ic++) {		// 一致する寸法型式使用部位コードがあるかnullまで調べる
			if ( cMTp1 == cMTp2) break;
		}
		if ( cMTp2 == 0) continue;								// 一致しない場合次の寸法型式に移る

		ASSERT( ip < MX_CMB_CDMEMBER - 1);						// 寸法型式選択コンボボックス項目　オーバフロー　<ERROR>
		z_iComboMbrId[ip++] = ic1;
		pCmb->InsertString( -1, pMbr->GetMbCode());
	}
exit:
	z_nComboMbrId = ip;
	if ( ip == 0)
		pCmb->InsertString( -1, Mstr( "none"));

	z_iComboKmIdMbr = -1;
}

///////////////////////////////////////////////////////////////////////////////
//	コンボボックスに寸法型式選択項目番号の寸法型式を表示し
//	カレントの寸法型式選択項目番号を設定する

void InpAttr::SetComboKmIdMbr(
						MINT	iKmIdMbr			// (I  ) 寸法型式選択項目番号
				)
{
	System::GetpMainFrame()->SetCombo2( iKmIdMbr);
	z_iComboKmIdMbr = iKmIdMbr;

	// 部材入力属性の設定
	MINT iMbrId = z_iComboMbrId[iKmIdMbr];
	MhPlcInp::SetIdMbr( iMbrId); 
	MhMbr *pMbr = BuzaiCode::MhGetpMbr( iMbrId);
	MhPlcInp::SetpMbr( pMbr);
}

///////////////////////////////////////////////////////////////////////////////
//	寸法型式選択項目番号を取得する
//							＝-1: 該当なし
//							≧ 0: 部材ID
MINT InpAttr::GetComboKmIdMbr( MCHAR* sCdMbr)
{
	MINT		ist;
	MINT		ic1;
	MINT		iMbrId;
	MhMbr	*pMbr;
	
	for ( ic1=0; ic1<z_nComboMbrId; ic1++) {
		iMbrId = z_iComboMbrId[ic1];
		pMbr = BuzaiCode::MhGetpMbr( iMbrId);
		if ( Mstrcmp( pMbr->GetMbCode(), sCdMbr) == 0)
			break;
	}
	if ( ic1 < z_nComboMbrId)
		ist = ic1;
	else
		ist = -1;
	return ist;
}

///////////////////////////////////////////////////////////////////////////////
//	寸法型式を設定する
//							＝-1: 該当なし
//							≧ 0: 寸法型式ID
MINT InpAttr::SetComboCdMbr( MCHAR* sCdMbr)
{
	MINT		iMbrId = -1;
	MINT		iKmIdMbr;

	iKmIdMbr = InpAttr::GetComboKmIdMbr( sCdMbr);
	if ( iKmIdMbr < 0) {
		Msg::ErrorMsg( sCdMbr, MC_ERR_NOTSET_MEMBER_TABLE);	// "が寸法型式テーブルに未設定です．"
		MQUIT;
	}
	InpAttr::SetComboKmIdMbr( iKmIdMbr);

	iMbrId = z_iComboMbrId[iKmIdMbr];
exit:
	return iMbrId;
}

///////////////////////////////////////////////////////////////////////////////
//	寸法型式IDを取得する
//							＝-1: 該当なし
//							≧ 0: 寸法型式ID
static MINT GetComboIdMbr( MCHAR* sCdMbr)
{
	MINT		ist;
	MINT		ic1;
	MhMbr	*pMbr;

	MINT mxMbr = BuzaiCode::MhGetNoOfMbr();
	for ( ic1=0; ic1<mxMbr; ic1++) {
		pMbr = BuzaiCode::MhGetpMbr( ic1);
		if ( Mstrcmp( pMbr->GetMbCode(), sCdMbr) == 0)
			break;
	}
	if ( ic1 < mxMbr)
		ist = ic1;
	else
		ist = -1;
	return ist;
}

///////////////////////////////////////////////////////////////////////////////
//	カレントの寸法型式IDを取得する
//							＝-1: 未選択
//							≧ 0: 寸法型式ID
MINT InpAttr::GetComboIdMbr()
{
	MINT		iMbrId;

	if ( z_iComboKmIdMbr >= 0 && z_iComboKmIdMbr < z_nComboMbrId) {
		iMbrId = z_iComboMbrId[z_iComboKmIdMbr];
	} else {
		iMbrId = -1;
	}
	return iMbrId;
}

} // namespace MC
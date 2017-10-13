//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MmDialogBarPanel.cpp
//
//		
//	パネル番号選択ダイアログバー
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MCAD.h"
#include "MainFrm.h"

#define DLL_EXPORT_MC_INPATTR_DO
#include "MmLib.h"
#include "MmDefine.h"
#include "resource.h"

#define		MC_PANEL_ALL	Mstr( "　　全")

namespace MC
{

// パネル番号選択用コンボボックス

static MINT z_nComboPanelNo;									// 表示項目数
static MINT	z_iComboPanelNo[MX_CMB_PANELNO];					// 表示項目対応パネル番号

///////////////////////////////////////////////////////////////////////////////
//	パネル番号選択用コンポボックスにパネル番号の項目(一覧)を設定し
//	パネル番号テーブルを作成する

void InpAttr::InitComboPanelNo()
{
	MINT		ic1;
	MINT		ic;
	MINT		ip = 0;
	CString		sKomoku;

	CComboBox *pCmb = MmpComboPanelNo();
	pCmb->ResetContent();

//	全パネル番号設定
	for ( ic=0; ic<10; ic++) {
		ASSERT( ip < MX_CMB_PANELNO - 1);								// パネル番号選択コンボボックス項目　オーバフロー　<ERROR>
		z_iComboPanelNo[ip++] = ic;
	}
	z_nComboPanelNo = ip;												// パネル数 + 1("全")　
//
	if ( z_nComboPanelNo == 0)
		goto exit;

	pCmb->InsertString( -1, MC_PANEL_ALL);

	for ( ic1=1; ic1<z_nComboPanelNo; ic1++) {
		sKomoku.Format(Mstr( "  %4d"), z_iComboPanelNo[ic1]);

		pCmb->InsertString( -1, sKomoku);
	}
exit:
	if ( z_nComboPanelNo == 0)
		pCmb->InsertString( -1, Mstr( "none"));
}

///////////////////////////////////////////////////////////////////////////////
//	コンボボックスにパネル番号を表示し
//	カレントのパネル番号選択項目番号を設定する

void InpAttr::SetComboPanelNo(
						MINT	iPanelNo			// (I  ) パネル番号
				)
{
	CComboBox *pCmb = MmpComboPanelNo();
	CString strPanelNo;
	strPanelNo.Format( Mstr( "%5d"), iPanelNo);
	pCmb->SetWindowText(strPanelNo);
}

///////////////////////////////////////////////////////////////////////////////
//	カレントのパネル番号を取得する
//							＝-1: 未選択
//							＝ 0: 全
//							≧ 1: パネル番号
MINT InpAttr::GetComboPanelNo()
{
	MINT iPanelNo;
	CComboBox *pCmb = MmpComboPanelNo();
	CString strPanelNo;
	pCmb->GetWindowText(strPanelNo);
	if ( strPanelNo == MC_PANEL_ALL)
		iPanelNo = 0;
	else if ( strPanelNo == Mstr( ""))
		iPanelNo = -1;
	else
		iPanelNo = MsGetMINT( strPanelNo);
	return iPanelNo;
}

} // namespace MC
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: Msg::SetStatusBar.cpp
//
//		ウィンドウ表示関連
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

#define DLL_EXPORT_MC_MSG_DO
#include "MmLib.h"

#include "MmSystemProperty.h"

#include "MmCmd.h"
#include "resource.h"

#include "Mainfrm.h"

#include "MmCmdMsg.h"
#include "MmPrompt.h"

#define	MAXNCSTBGUIDANCE	61
#define	MAXNCSTBERROR		61
#define	MAXNCSTBOPERATION	11

namespace MC
{

static	MCHAR		z_cStbGuidance[MAXNCSTBGUIDANCE];				// ステイタスバーのガイダンスメッセージ表示内容
static	MCHAR		z_cStbError[MAXNCSTBERROR];						// ステイタスバーのエラーメッセージ表示内容
static	MCHAR		z_cStbOperation[MAXNCSTBOPERATION];				// ステイタスバーのオペレーションメッセージ表示内容

/////////////////////////////////////////////////////////////////////////////
//  ステイタスバー表示処理　

void Msg::SetSelStatusBar(
						MINT		i_iStb,		// ステイタスバー項目番号
				const	MCHAR*		i_pcStb		// ステイタスバー項目表示内容
				)
{
	CWnd*		pWnd;
	MmWndInfo*	pWndInfo;

	if ( i_iStb == MC_STSBAR_GUIDANCE) {
		Mstrncpy( z_cStbGuidance, i_pcStb, MAXNCSTBGUIDANCE);
	} else if ( i_iStb == MC_STSBAR_ERROR) {
		Mstrncpy( z_cStbError, i_pcStb, MAXNCSTBERROR);
		z_cStbGuidance[0] = NULL;
	} else if ( i_iStb == MC_STSBAR_OPERATION) {
		Mstrncpy( z_cStbOperation, i_pcStb, MAXNCSTBOPERATION);
	} else {
		return;
	}
	pWndInfo = WindowCtrl::MmWndKGetCurWnd();								// カレントウィンドウ取得
	if ( pWndInfo) {
		pWnd = pWndInfo->GetWnd();
		pWnd->PostMessage(WM_MYMESSAGESTATUSBAR);				// ステイタスバー表示用メッセージハンドラを起動する
	}
}

/////////////////////////////////////////////////////////////////////////////
//  ステイタスバー表示処理　(メッセージハンドラより呼ばれる)
void Msg::DispStatusBar()
{
	Msg::DispSelStatusBar( MC_STSBAR_GUIDANCE, z_cStbGuidance);		// ステイタスバーのガイダンスメッセージを表示する
	Msg::DispSelStatusBar( MC_STSBAR_OPERATION, z_cStbOperation);		// ステイタスバーのオペレーションメッセージを表示する
	Msg::DispSelStatusBar( MC_STSBAR_ERROR, z_cStbError);				// ステイタスバーのエラーメッセージを表示する
}

/////////////////////////////////////////////////////////////////////////////
//	ステータスバーに文字列を表示する　(メッセージハンドラより使用可)

void Msg::DispSelStatusBar(
						MINT		i_iStb,		// ステイタスバー項目番号
				const	MCHAR*		i_pcStb		// ステイタスバー項目表示内容
				)
{
	CMDIFrameWnd*	MainFrm;					// メインフレーム
	CStatusBar*		StBar;						// ステイタスバー

	MainFrm = System::GetpMainFrame();										// メインフレームを取得
	if ( MainFrm != NULL) {
		StBar = (CStatusBar*)(MainFrm->GetDescendantWindow( AFX_IDW_STATUS_BAR));
		StBar->SetPaneText( i_iStb, i_pcStb);
	}
}

} // namespace MC
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
#include "MCAD.h"
#include "MmCmd.h"
#include "resource.h"

#include "McCmd.h"
#include "MmWnd.h"
#include "MmCmdMsg.h"

#include "MmDialogGrid.h"

namespace MC
{

static CMmDialogGrid*	z_pDlgGrid = NULL;						// グリッドダイアログ

/////////////////////////////////////////////////////////////////////////////
//	グリッド数変更入力

void MCmdGridNum()
{
	MmWndInfo*	pWndInfo = WindowCtrl::MmWndKGetCurWnd();				// カレントウィンドウを取得する
	CWnd*		pWnd = pWndInfo->GetWnd();
	
	pWnd->PostMessage(WM_MYMESSAGEGRIDNUM);

//	MCmdOptInp( NULL);
}

/////////////////////////////////////////////////////////////////////////////
//	グリッド数変更入力
void MCmdGridNumInp		(
						CWnd*		pWnd						// (I  ) ウィンドウのインスタンス
				)
{
	if (z_pDlgGrid == NULL) {
		z_pDlgGrid = new CMmDialogGrid( pWnd);
		z_pDlgGrid->Create( pWnd);
	} else {
		z_pDlgGrid->BringWindowToTop();
	}


}

/////////////////////////////////////////////////////////////////////////////
//	グリッド数変更終了
void MCmdGridNumEnd()
{
	z_pDlgGrid = NULL;
}

/////////////////////////////////////////////////////////////////////////////
//	グリッド数変更キャンセル
void MmGridNumCancel()
{
	if (z_pDlgGrid)
//		z_pDlgGrid->SendMessage(WM_CLOSE);
		z_pDlgGrid->PostMessage(WM_CLOSE);
}

} // namespace MC
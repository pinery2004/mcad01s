//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MmTenkai.cpp
//
//		住棟展開
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

#include "MmTenkai.h"
#include "MmDialogTenkai.h"

namespace MC
{

static	CMmDialogTenkai* z_pDlgTenkai = NULL;	// モードレスバージョンダイアログの表示用

/////////////////////////////////////////////////////////////////////////////
//	住棟展開

void MCmdTenkai()
{
	MmWndInfo*	pWndInfo = WindowCtrl::MmWndKGetCurWnd();					// カレントウィンドウを取得する
	CWnd*		pWnd = pWndInfo->GetWnd();
	
	pWnd->PostMessage(WM_MYMESSAGETENKAI);
}
/////////////////////////////////////////////////////////////////////////////
//	住棟展開設定入力
void MCmdTenkaiInp(
						CWnd*		pWnd			// (I  ) ウィンドウのインスタンス
				)
{
	if (z_pDlgTenkai == NULL) {
		z_pDlgTenkai = new CMmDialogTenkai( pWnd);
		z_pDlgTenkai->Create( pWnd);
	} else {
		z_pDlgTenkai->BringWindowToTop();
	}
}

/////////////////////////////////////////////////////////////////////////////
//	住棟展開設定終了
void MCmdTenkaiEnd()
{
	z_pDlgTenkai = NULL;
}

} // namespace MC
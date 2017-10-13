//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MmPtsEdit.cpp
//
//		部品展開
//
//
//  K.Matsu           07/14/07    Created.
//==========================================================================================
#include "stdafx.h"
#include "MCAD.h"
#include "resource.h"

#include "../MSub/gridctrl/gridctrl.h"
#include "MmWnd.h"
#include "MmCmdMsg.h"

#include "MmDialogPtsEdit.h"

namespace MC
{

static	CMmDialogPtsEdit* z_pDlgPtsEdit = NULL;	// モードレスバージョンダイアログの表示用

/////////////////////////////////////////////////////////////////////////////
//	部品展開

void MCmdPtsEdit()
{
	MmWndInfo*	pWndInfo = WindowCtrl::MmWndKGetCurWnd();					// カレントウィンドウを取得する
	CWnd*		pWnd = pWndInfo->GetWnd();
	
	pWnd->PostMessage(WM_MYMESSAGEPARTSEDIT);
}

/////////////////////////////////////////////////////////////////////////////
//	部品展開設定入力
void MCmdPtsEditInp(
						CWnd*		pWnd			// (I  ) ウィンドウのインスタンス
				)
{
	if (z_pDlgPtsEdit == NULL) {
		z_pDlgPtsEdit = new CMmDialogPtsEdit( pWnd);
		z_pDlgPtsEdit->Create( pWnd);
	} else {
		z_pDlgPtsEdit->BringWindowToTop();
	}
}

/////////////////////////////////////////////////////////////////////////////
//	部品展開設定終了
void MCmdPtsEditEnd()
{
	z_pDlgPtsEdit = NULL;
}

} // namespace MC
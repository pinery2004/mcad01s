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
//
//	[機能] 
//
#include "stdafx.h"
#include "MCAD.h"
#include "MmCmd.h"
#include "resource.h"

#include "McCmd.h"
#include "MmWnd.h"
#include "MmCmdMsg.h"

//#include "MmPtsEdit.h"
#include "MmDialogPtsEdit.h"

static	CMmDialogPtsEdit* z_pDlgPtsEdit = NULL;	// モードレスバージョンダイアログの表示用

/////////////////////////////////////////////////////////////////////////////
//	部品展開

void MCmdPtsEdit()
{
	MmWndInfo*	pWndInfo = MmWndKGetCurWnd();					// カレントウィンドウを取得する
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

 

//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MCmdOpt.cpp
//
//		オプション設定操作
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MCAD.h"
#include "resource.h"
#include "MmDialogOpt.h"
#include "McCmd.h"
#include "MmWnd.h"
#include "MmCmdMsg.h"

namespace MC
{

static	CMmDialogOpt* z_pDialogOpt = NULL;		// モードレスＯＰＴ群ダイアログの表示用

/////////////////////////////////////////////////////////////////////////////
//	ＯＰＴ群入力

void MCmdOpt()
{
	MmWndInfo*	pWndInfo = WindowCtrl::MmWndKGetCurWnd();					// カレントウィンドウを取得する
	CWnd*		pWnd = pWndInfo->GetWnd();
	
	pWnd->PostMessage(WM_MYMESSAGEOPTION);

//	MCmdOptInp( NULL);
}

/////////////////////////////////////////////////////////////////////////////
//【機能】オプション設定ダイアログ表示　入力
void MCmdOptInp(
						CWnd*		pWnd			// (I  ) ウィンドウのインスタンス
				)
{
	if (z_pDialogOpt == NULL)
	{
		z_pDialogOpt = new CMmDialogOpt;
		z_pDialogOpt->Create( pWnd);
	}
	else
	{
		z_pDialogOpt->BringWindowToTop();
	}
}

/////////////////////////////////////////////////////////////////////////////
//【機能】オプション設定ダイアログ表示　終了
void MCmdOptEnd()
{
	z_pDialogOpt = NULL;
}

} // namespace MC
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MmCmdHist.cpp
//
//		修正履歴管理設定操作
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MCAD.h"
#include "resource.h"
#include "MmDialogHist.h"
#include "McCmd.h"
#include "MmWnd.h"
#include "MmCmdMsg.h"

namespace MC
{

static	CMmDialogHist* z_pDlgHist = NULL;	// モードレスバージョンダイアログの表示用

/////////////////////////////////////////////////////////////////////////////
//	修正履歴管理

void MCmdHist()
{
	MmWndInfo*	pWndInfo = WindowCtrl::MmWndKGetCurWnd();					// カレントウィンドウを取得する
	CWnd*		pWnd = pWndInfo->GetWnd();
	
	pWnd->PostMessage(WM_MYMESSAGEHIST);							// OK: 1st
//	pWnd->SendMessage(WM_MYMESSAGEHIST);							// OK:
//	MCmdHistInp( NULL);												// NG: 後ろに隠れてしまう
}

/////////////////////////////////////////////////////////////////////////////
//	修正履歴管理設定入力
void MCmdHistInp(
						CWnd*		pWnd			// (I  ) ウィンドウのインスタンス
				)
{
	if (z_pDlgHist == NULL)
	{
		// CDialog::Create( CMmDialogHist::IDD, NULL);で次のアボートされるのを防ぐため
		// AFX_MANAGE_STATE(AfxGetStaticModuleState());を追加
		// ただし、このせいか不明であるが、View1を触ると後ろに隠れてしまう
		// (dlgcore.cpp)
		//	if (!_AfxCheckDialogTemplate(lpszTemplateName, FALSE))
		//	{
		//		ASSERT(FALSE);          // invalid dialog template name

//		AFX_MANAGE_STATE(AfxGetStaticModuleState());

		// その後MCmd2.cppを含める事によりOkとなった
		//

		z_pDlgHist = new CMmDialogHist;							// OK 
		z_pDlgHist->Create( pWnd);
//		z_pDlgHist = new CMmDialogHist( NULL);					// OK
//				MATRACE(CMmDialogHist);
	}
	else
	{
		z_pDlgHist->BringWindowToTop();
	}
}

/////////////////////////////////////////////////////////////////////////////
//	修正履歴管理設定終了
void MCmdHistEnd()
{
	z_pDlgHist = NULL;
}

} // namespace MC
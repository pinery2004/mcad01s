//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MCmdSuudoku.cpp
//
//		数独
//
//
//  K.Matsu           07/15/07    Created.
//==========================================================================================
#include "stdafx.h"
#include "MCAD.h"
#include "MmCmd.h"
#include "resource.h"

#include "McCmd.h"
#include "MmWnd.h"
#include "MmCmdMsg.h"

#include "SuudokuTbl.h"

static	CSuudokuTbl* z_pSuudokuTbl = NULL;	// モードレスバージョンダイアログの表示用
/*
/////////////////////////////////////////////////////////////////////////////
//	数独

void MCmdSuudoku()
{
	MmWndInfo*	pWndInfo = MmWndKGetCurWnd();					// カレントウィンドウを取得する
	CWnd*		pWnd = pWndInfo->GetWnd();
	
	pWnd->PostMessage(WM_MYMESSAGESUUDOKU);
}
*/
/////////////////////////////////////////////////////////////////////////////
//	数独開始
void MCmdSuudokuiInp(
						CWnd*		pWnd			// (I  ) ウィンドウのインスタンス
				)
{
	if (z_pSuudokuTbl == NULL) {
		z_pSuudokuTbl = new CSuudokuTbl( pWnd);
		z_pSuudokuTbl->Create( pWnd);
	} else {
		z_pSuudokuTbl->BringWindowToTop();
	}
}

/////////////////////////////////////////////////////////////////////////////
//	数独終了
void MCmdSuudokuEnd()
{
	z_pSuudokuTbl = NULL;
}

 

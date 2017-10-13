//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MCmdSuudoku.cpp
//
//		����
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

static	CSuudokuTbl* z_pSuudokuTbl = NULL;	// ���[�h���X�o�[�W�����_�C�A���O�̕\���p
/*
/////////////////////////////////////////////////////////////////////////////
//	����

void MCmdSuudoku()
{
	MmWndInfo*	pWndInfo = MmWndKGetCurWnd();					// �J�����g�E�B���h�E���擾����
	CWnd*		pWnd = pWndInfo->GetWnd();
	
	pWnd->PostMessage(WM_MYMESSAGESUUDOKU);
}
*/
/////////////////////////////////////////////////////////////////////////////
//	���ƊJ�n
void MCmdSuudokuiInp(
						CWnd*		pWnd			// (I  ) �E�B���h�E�̃C���X�^���X
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
//	���ƏI��
void MCmdSuudokuEnd()
{
	z_pSuudokuTbl = NULL;
}

 

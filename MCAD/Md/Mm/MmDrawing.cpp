//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MmDrag.cpp
//
//		�h���b�M���O����
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
#define DLL_EXPORT_MC_WINDOWCTRL_DO
#include "MmWnd.h"
#include "MmDefine.h"
#include "MmLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace MC
{

/////////////////////////////////////////////////////////////////////////////
//  �c�b�ɍ\���}��`��

void WindowCtrl::MmDrawStructure(
				MmWndInfo*	pWndInfo,							// (I  ) �E�B���h�E�Ǘ����
				MbCod*		pCod
				)
{
	CDC* pDC = pCod->GetDC();

	// �}�b�v���[�h��ݒ肷��
	MINT iOldMM = pDC->SetMapMode(MM_HIMETRIC);

	// �}�ʃT�C�Y�Ɣ{�����擾����
	CSize cszB = pDC->GetWindowExt();

	// �}�ʘg��`��
	pDC->MoveTo( 0,			-cszB.cy);
	pDC->LineTo( cszB.cx,	-cszB.cy);
	pDC->LineTo( cszB.cx,	0);
	pDC->LineTo( 0,			0);
	pDC->LineTo( 0,			-cszB.cy);

	// �`��̈��ݒ肷��
	pCod->SetMinMaxRS( pWndInfo->GetMinMaxRS());

	MINT iLleft = 0;
	MINT iLtop = 0;
	MINT iLwidth = cszB.cx;
	MINT iLhight = -cszB.cy;
	pCod->SetWinL( iLleft, iLtop, iLwidth, iLhight);
	pCod->SetMat();

	// �c�b�ɕ\��
	WindowCtrl::MmWndKDrawGrid( pWndInfo, pCod);
	MINT iKaiC = InpMode::GetKai();									// ���݂̊K
	MINT iGpC = InpMode::GetKGp();							// ���݂̍\��
	WindowCtrl::MmWndKDrawKabe( pCod, iKaiC, iGpC);
	WindowCtrl::MmWndKDrawKiso( pCod, iKaiC, iGpC);
	WindowCtrl::MmWndKDrawPts( pCod, iKaiC, iGpC);
	WindowCtrl::MmWndKDrawRoof( pCod, iKaiC, iGpC);
//	WindowCtrl::MmWndKDrawTemp( pWndInfo, pCod);

	//	�}�b�v���[�h��߂�
	pWndInfo->m_pMemDC->SetMapMode( iOldMM);
}

} // namespace MC
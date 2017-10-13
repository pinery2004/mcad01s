//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MmDrag.cpp
//
//		ƒhƒ‰ƒbƒMƒ“ƒO‘€ì
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
//  ‚c‚b‚É\‘¢}‚ð•`‚­

void WindowCtrl::MmDrawStructure(
				MmWndInfo*	pWndInfo,							// (I  ) ƒEƒBƒ“ƒhƒEŠÇ—î•ñ
				MbCod*		pCod
				)
{
	CDC* pDC = pCod->GetDC();

	// ƒ}ƒbƒvƒ‚[ƒh‚ðÝ’è‚·‚é
	MINT iOldMM = pDC->SetMapMode(MM_HIMETRIC);

	// }–ÊƒTƒCƒY‚Æ”{—¦‚ðŽæ“¾‚·‚é
	CSize cszB = pDC->GetWindowExt();

	// }–Ê˜g‚ð•`‚­
	pDC->MoveTo( 0,			-cszB.cy);
	pDC->LineTo( cszB.cx,	-cszB.cy);
	pDC->LineTo( cszB.cx,	0);
	pDC->LineTo( 0,			0);
	pDC->LineTo( 0,			-cszB.cy);

	// •`‰æ—Ìˆæ‚ðÝ’è‚·‚é
	pCod->SetMinMaxRS( pWndInfo->GetMinMaxRS());

	MINT iLleft = 0;
	MINT iLtop = 0;
	MINT iLwidth = cszB.cx;
	MINT iLhight = -cszB.cy;
	pCod->SetWinL( iLleft, iLtop, iLwidth, iLhight);
	pCod->SetMat();

	// ‚c‚b‚É•\Ž¦
	WindowCtrl::MmWndKDrawGrid( pWndInfo, pCod);
	MINT iKaiC = InpMode::GetKai();									// Œ»Ý‚ÌŠK
	MINT iGpC = InpMode::GetKGp();							// Œ»Ý‚Ì\¬
	WindowCtrl::MmWndKDrawKabe( pCod, iKaiC, iGpC);
	WindowCtrl::MmWndKDrawKiso( pCod, iKaiC, iGpC);
	WindowCtrl::MmWndKDrawPts( pCod, iKaiC, iGpC);
	WindowCtrl::MmWndKDrawRoof( pCod, iKaiC, iGpC);
//	WindowCtrl::MmWndKDrawTemp( pWndInfo, pCod);

	//	ƒ}ƒbƒvƒ‚[ƒh‚ð–ß‚·
	pWndInfo->m_pMemDC->SetMapMode( iOldMM);
}

} // namespace MC
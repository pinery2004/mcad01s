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
#include "MbBasic.h"
#include "MgLib.h"
#include "MmDefine.h"
#include "MgMat.h"
#include "MbDefine.h"
#include "MbCod.h"
#include "MmGridNum.h"
#include "MmDrag.h"
#include "MmWnd.h"
#include "MmLib.h"
#include "MmSystemProperty.h"

#include "MmCmd.h"
#include "resource.h"
#include "..\\MCAD\\resource.h"

namespace MC
{

// ***********************************
// CLASSES IMPLEMENTATION
//

//
// MmDrag
//

MmDrag::MmDrag( MmWndInfo* pWndInfo, MgPoint2 ptStart)
{
	m_pWndInfo = pWndInfo;

	m_ptStart = ptStart;
	m_ptLast = ptStart;
	
// 070907	m_bContinuousOrbitStarted = false;
}

MmDrag::~MmDrag(void)
{
	m_pWndInfo->SetDrag( this);	// (see MCadEntRenderer::drawImageObject).
	m_pWndInfo->SetCursor( m_iPrevCursorMode);
}

bool
MmDrag::Continue( MmWndInfo* pWndInfo, MgPoint2 ptLatest)
{
	return true;
}	

void
MmDrag::RedrawAfter( MmWndInfo* pWndInfo)
{

	WindowCtrl::MmWndKDrawMDC( pWndInfo);
	pWndInfo->GetWnd()->PostMessage(WM_PAINT);
}

//
// MmDragZoom
//
MmDragZoom::MmDragZoom( MmWndInfo* pWndInfo, MgPoint2 ptStart) : MmDrag( pWndInfo, ptStart)
{
	m_iPrevCursorMode = pWndInfo->SetCursor( IDC_ZOOM);
	m_Motion = ZOOM;
}

bool
MmDragZoom::Continue( MmWndInfo* pWndInfo, MgPoint2 ptLatest)
{
	if ( m_Motion != ZOOM) return true;

	if( MmDrag::Continue( pWndInfo, ptLatest))
	{
		MgMinMaxR2	rMinMaxR = pWndInfo->GetMinMaxRS();
		MgMinMaxR2	rRelMinMaxR = rMinMaxR - m_ptStart;
		MgMinMaxI2	iMinMaxL = pWndInfo->GetMinMaxL();
		MINT		iHWndLy = iMinMaxL.GetHgt();		

		CPoint		ptLatestL = pWndInfo->RPtoLP( ptLatest);					// 論理座標へ変換
		CPoint		ptLastL = pWndInfo->RPtoLP( m_ptLast);						// 論理座標へ変換

		MREAL		DeltaY = ( ptLatestL.y - ptLastL.y) / (MREAL)iHWndLy;
//		if ( MMAPMODE == MM_TEXT) DeltaY = - DeltaY;

		if( DeltaY >= 0)
			rRelMinMaxR /= (1 + (5.0f * DeltaY));
		else
			rRelMinMaxR *= (1 + (5.0f * (- DeltaY)));
		
		rMinMaxR = rRelMinMaxR + m_ptStart;
		pWndInfo->SetMinMaxRS( rMinMaxR);
		pWndInfo->SetMat();

		m_ptLast = pWndInfo->LPtoRP( ptLatestL);
		
		RedrawAfter( pWndInfo);
	
		return true;
	}
	else
		return false;
}


//
// MmDragPan
//
MmDragPan::MmDragPan( MmWndInfo* pWndInfo, MgPoint2 ptStart) : MmDrag( pWndInfo, ptStart)
{
	m_iPrevCursorMode = pWndInfo->SetCursor(IDC_GRAB);
	m_Motion = PAN;
}

bool
MmDragPan::Continue( MmWndInfo* pWndInfo, MgPoint2 ptLatest)
{
	if( MmDrag::Continue( pWndInfo, ptLatest))
	{
		MgPoint2	PtPan;

		PtPan = m_ptLast - ptLatest;

		MgMinMaxR2	rMinMaxR = pWndInfo->GetMinMaxRS();
		rMinMaxR += PtPan;
		pWndInfo->SetMinMaxRS( rMinMaxR);
		pWndInfo->SetMat();

		RedrawAfter( pWndInfo);
	
//NG		m_ptLast = ptLatest;									//070907

		return true;
	}
	else
		return false;
}	

} // namespace MC
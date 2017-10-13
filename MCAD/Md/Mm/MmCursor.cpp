//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MmCursor.cpp
//
//		ウィンドウ表示関連
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
#include "MmWnd.h"
#include "MmDefine.h"

#define	DLL_EXPORT_MC_WINDOW_DO
#include "MmLib.h"

#include "MmSystemProperty.h"

namespace MC
{

static	bool		z_fDrawCrossCursor = FALSE;

static	CPoint			z_ptCurCrossHairL[2][2];
static	CPoint			z_ptLCurMouth;

static	MmWndInfo* 		z_pCurWndInfo = NULL;					// クロスヘアーカーソル表示先カレントウィンドウ

/////////////////////////////////////////////////////////////////////////////
//	新規ウィンドウ作成時の初期化 (ビューの初期表示時に使用)
//	クロスヘアーカーソルが表示されていない状態である事を設定する

void Window::InitCrossHair()
{
	z_fDrawCrossCursor = FALSE;
}

////////////////////////////////////////////////////////////////////////////
//  カーソルを設定する

MINT Window::SetCursor(
						CWnd*		pWnd			// (I  ) ウィンドウのインスタンス
				) 
{
	MINT	ist = 1;
	MINT	iCursor;											// ウィンドウのカレントカーソル番号

	MmWndInfo* pWndInfo = WindowCtrl::MmWndKFindWnd( pWnd);

	if ( pWndInfo != 0) {

		iCursor = pWndInfo->m_iCursor;
		::SetCursor( AfxGetApp()->LoadCursor( iCursor));
		ist = 0;
	}
	return ist;
}

/////////////////////////////////////////////////////////////////////////////
//	クロスヘアーカーソルを消去する

void Window::EraseCrossHair()
{
	if ( z_fDrawCrossCursor) {
		Window::DispCrossHair( -1, NULL, NULL);
		z_fDrawCrossCursor = FALSE;
	}
}

/////////////////////////////////////////////////////////////////////////////
//	クロスヘアーカーソルを表示する

void Window::DrawCrossHair(
						MmWndInfo*	pWndInfo,		// (I  ) ウィンドウ管理情報
						CPoint		ptMouthL		// (I  ) 論理座標
				)
{
	if ( !z_fDrawCrossCursor) {
		Window::DispCrossHair( 1, pWndInfo, &ptMouthL);
		z_fDrawCrossCursor = TRUE;
	}
}

/////////////////////////////////////////////////////////////////////////////
//	クロスヘアーカーソルを再表示する

void Window::ReDrawCrossHair()
{
	if ( !z_fDrawCrossCursor) {
		Window::DispCrossHair( 0, NULL, NULL);
		z_fDrawCrossCursor = TRUE;
	}
}

/////////////////////////////////////////////////////////////////////////////
//	クロスヘアーカーソルを表示/再表示/消去する
MINT Window::DispCrossHair(
						MINT		ictl,			// (I  ) 表示制御	 1 : 表示、0:再表示、-1 : 消去
						MmWndInfo*	pWndInfo,		// (I  ) ウィンドウ管理情報(表示制御 == 1) または NULL(表示制御 <= 0)　
						CPoint*		pptLMouth		// (I  ) マウス位置論理座標(表示制御 == 1) または NULL(表示制御 <= 0)
				)
{
	if ( ictl == 1) {
		z_pCurWndInfo = pWndInfo;
		z_ptLCurMouth = *pptLMouth;
	}
	if ( z_pCurWndInfo == NULL)
		return 0;

	CClientDC	dc( z_pCurWndInfo->GetWnd());
	DWORD		rgbCursor = MmSystemProperty::GetColor( MM_COLOR_CURSOR);
	CPen		CrossHairPen( PS_SOLID, 1, rgbCursor);
	CPen*		OldPen = dc.SelectObject( &CrossHairPen);

	MINT		iMapMode = z_pCurWndInfo->GetMapMode();
	MINT		iOldMM = dc.SetMapMode( iMapMode);

	dc.SetROP2( R2_XORPEN);

	if ( ictl >= 0) {
		MgMinMaxI2	iMinMaxL = z_pCurWndInfo->GetMinMaxL();
		MINT		iUpL = z_pCurWndInfo->GetUpL();								// ＸＹ表示バランス
//S		z_ptCurCrossHairL[0][0] = CPoint( z_ptLCurMouth.x, iMinMaxL.min.y * iUpL);
//		z_ptCurCrossHairL[0][1] = CPoint( z_ptLCurMouth.x, ( iMinMaxL.max.y+100) * iUpL);
//		z_ptCurCrossHairL[0][1] = CPoint( z_ptLCurMouth.x, iMinMaxL.max.y * iUpL);
//		z_ptCurCrossHairL[1][0] = CPoint( iMinMaxL.min.x, z_ptLCurMouth.y * iUpL);
//		z_ptCurCrossHairL[1][1] = CPoint( iMinMaxL.max.x, z_ptLCurMouth.y * iUpL);
		z_ptCurCrossHairL[0][0] = CPoint( z_ptLCurMouth.x, iMinMaxL.min.y * iUpL);
		z_ptCurCrossHairL[0][1] = CPoint( z_ptLCurMouth.x, iMinMaxL.max.y * iUpL);
		z_ptCurCrossHairL[1][0] = CPoint( iMinMaxL.min.x, z_ptLCurMouth.y * iUpL);
		z_ptCurCrossHairL[1][1] = CPoint( iMinMaxL.max.x, z_ptLCurMouth.y * iUpL);
	}
	dc.Polyline( z_ptCurCrossHairL[0], 2);										// 		縦線
	dc.Polyline( z_ptCurCrossHairL[1], 2);										// 		横線

	dc.SetMapMode( iOldMM);
	dc.SelectObject( OldPen);
	return 1;
}

} // namespace MC
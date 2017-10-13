//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MmDrag.cpp
//
//		ドラッギング操作（ラバーバンド図形表示）
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"
#include "MgLib.h"
#include "MmDefine.h"
#include "MbDefine.h"
#include "MmWnd.h"

#define DLL_EXPORT_MC_WINDOW_DO
#include "MmLib.h"
#include "MmSystemProperty.h"
#include "MtAskForPoint.h"

#define		MAXDRAGGING		40
#define		MCREVOVRP		22

namespace MC
{

static	bool			z_fDragMode = false;					// トラッギングモード
																// true : ON、false : OFF

static	bool			z_fDrawDragging = FALSE;				// ラバーバンド図形表示中フラグ

static	MmWndInfo* 		z_pLDragWndInfo = NULL;
static	MINT			z_modeL = NULL;							// 現在表示中の図形種類
static	MINT			z_nLDragging;							// 現在表示中のポリライン頂点数(線分数+1)
static	CPoint			z_ptLDragging[MAXDRAGGING];				// 現在表示中のポリライン頂点の論理座標(線分数+1)

static	MmWndInfo* 		z_pRDragWndInfo = NULL;
static	MINT			z_modeR = NULL;							// 入力図形種類
static	MINT			z_nRDragging;							// 入力済ポリライン頂点数
static	MgPoint2		z_ptRDragging[MAXDRAGGING];				// 入力済ポリライン頂点の座標

static	CPoint			z_ptLDragMouth;

/////////////////////////////////////////////////////////////////////////////
//	新規ウィンドウ作成時の初期化 (ビューの初期表示時に使用)
//	ビューの初期表示時にラバーバンド図形が表示されていない事を設定する

void Window::InitDragging()
{
	z_fDrawDragging = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//	ドラッギングモード終了

void Window::DragModeEnd()
{
	Window::EraseDragging();
	z_fDragMode = false;										// トラッギングモード終了
}

/////////////////////////////////////////////////////////////////////////////
//	ドラッギングモード確認

bool Window::GetDragMode()
{
	return z_fDragMode;
}

/////////////////////////////////////////////////////////////////////////////
//	ラバーバンド図形を消去する

void Window::EraseDragging()
{
	if ( Window::GetDragMode() && z_fDrawDragging) {
		Window::DispDragging( -1, NULL, NULL);
		z_fDrawDragging = FALSE;
	}
}

/////////////////////////////////////////////////////////////////////////////
//	ラバーバンド図形を表示する

void Window::DrawDragging(
						MmWndInfo*	pWndInfo,		// (I  ) ウィンドウ管理情報
						MgPoint2	ptMouthR		// (I  ) マウス位置実座標
				)
{
	if ( Window::GetDragMode() && !z_fDrawDragging) {
		Window::DispDragging( 1, pWndInfo, &ptMouthR);
		z_fDrawDragging = TRUE;
	}
}


/////////////////////////////////////////////////////////////////////////////
//	ラバーバンド図形を再表示する
//	途中ポイントの座標はカレントの座標変換マトリックスで座標変換した論理座標で
//	最終ポイントはカレントのマウス論理座標で描く

void Window::ReDrawDragging()
{
	if ( Window::GetDragMode()) {
		Window::DispDragging( 0, NULL, NULL);
		z_fDrawDragging = TRUE;
	}
}

/////////////////////////////////////////////////////////////////////////////
//	ラバーバンド図形を表示してドラッギングモードを開始し、
//	マウスにて座標入力する
//	既に表示されていた場合はその表示を消した後、新たに表示する

MINT Window::DragObject(
						MINT		mode,			// (I  ) ドラッギング入力モード
				const	void*		pFg,			// (I  ) ドラッギング入力済み座標
						MgPoint2*	ptGet			// (  O) マウス位置実座標
				)
{
	MINT		irt;
	MgPoint2*	pPtSt;
	MgPolyg2*	pPg;
	MINT		ic;

	// 入力済座標と点数を設定する
	z_modeR = mode;

	switch( z_modeR) {
		case MC_RBND_LINE:										// Line
			pPtSt = (MgPoint2*)pFg;
			z_ptRDragging[0] = pPtSt[0];
			z_nRDragging = 1;
			break;
		case MC_RBND_RECT:										// Rectangle
			pPtSt = (MgPoint2*)pFg;
			z_ptRDragging[0] = pPtSt[0];
			z_nRDragging = 1;
			break;
		case MC_RBND_POLYGON:									// Polygon
			pPg = (MgPolyg2*)pFg;
			for ( ic=0; ic<pPg->m_n; ic++) {
				z_ptRDragging[ic] = pPg->m_p[ic];
			}
			z_nRDragging = pPg->m_n;
			break;
		case MC_RBND_PARALOG:									// Parallelogram
			pPtSt = (MgPoint2*)pFg;
			z_ptRDragging[0] = pPtSt[0];
			z_ptRDragging[1] = pPtSt[1];
			z_nRDragging = 1;
			break;
	}
	z_pRDragWndInfo = WindowCtrl::MmWndKGetCurWnd();						// ラバーバンド図形表示先カレントウィンドウを設定する
	z_fDragMode = true;											// ラバーバンド図形表示モード開始

	Window::EraseDragging();
	Window::DrawDragging( z_pRDragWndInfo, z_pRDragWndInfo->GetCurMousePtR());
//S	irt = MtAskForPoint( NULL, NULL, ptGet);
	irt = MtAskForPoint( ptGet);

	return irt;
}

/////////////////////////////////////////////////////////////////////////////
//	ラバーバンド図形を表示/再表示/消去する
//		表示: 

MINT Window::DispDragging(
						MINT		ictl,			// (I  ) 表示制御 1 : 表示、0 : 再表示、-1 : 消去
						MmWndInfo*	pWndInfo,		// (I  ) ウィンドウ管理情報 または NULL
						MgPoint2*	pptRMouth		// (I  ) マウス位置実座標 または NULL
				)
{
	MINT		ic;
	MINT		iUpL;											// ＸＹ表示バランス
	CPoint		rev( MCREVOVRP, MCREVOVRP);						// ドラッグ表示線分とグリッド線との重なり補正
	MgVect2 	v1, v2;

	if ( ictl == 1) {
		z_pLDragWndInfo = pWndInfo;
		z_modeL = z_modeR;
		z_nLDragging = z_nRDragging + 1;						// 表示点数 = 入力済み点数 + 1(ドラッグ表示最終点)
		z_ptLDragMouth = z_pLDragWndInfo->RPtoLP( *pptRMouth);
	}

	iUpL = z_pLDragWndInfo->GetUpL();							// 縦横表示補正値

	CClientDC	dc( z_pLDragWndInfo->GetWnd());
	DWORD		rgbCursor = MmSystemProperty::GetColor( MM_COLOR_DRAGGING);
	CPen		CrossHairPen( PS_SOLID, 1, rgbCursor);
	CPen*		OldPen = dc.SelectObject( &CrossHairPen);

	MINT		iMapMode = z_pLDragWndInfo->GetMapMode();
	MINT		iOldMM = dc.SetMapMode( iMapMode);

	dc.SetROP2( R2_XORPEN);

	if ( ictl >= 0) {
		switch( z_modeL)  {
			case 1:																	// 線分
				z_ptLDragging[0] = z_pLDragWndInfo->RPtoLP( z_ptRDragging[0]) + rev;
				z_ptLDragging[1] = CPoint( z_ptLDragMouth.x, z_ptLDragMouth.y * iUpL) + rev;
				break;
			case 2:																	// 長方形
				z_ptLDragging[0] = z_pLDragWndInfo->RPtoLP( z_ptRDragging[0]) + rev;
				z_ptLDragging[2] = CPoint( z_ptLDragMouth.x, z_ptLDragMouth.y * iUpL) + rev;
				z_ptLDragging[1] = CPoint( z_ptLDragging[0].x, z_ptLDragging[2].y);
				z_ptLDragging[3] = CPoint( z_ptLDragging[2].x, z_ptLDragging[0].y);
				z_ptLDragging[4] = z_ptLDragging[0];
				z_nLDragging = 5;
				break;
			case 3:																	// 区画
				for ( ic=0; ic<z_nRDragging; ic++)
					z_ptLDragging[ic] = z_pLDragWndInfo->RPtoLP( z_ptRDragging[ic]) + rev;
				if ( z_nLDragging == z_nRDragging + 1)
					z_ptLDragging[z_nRDragging] = CPoint( z_ptLDragMouth.x, z_ptLDragMouth.y * iUpL) + rev;

				if ( z_nLDragging > 2) {											// 表示構成点が３点以上の場合は区画表示にする
					z_ptLDragging[z_nLDragging] = z_ptLDragging[0];
					z_nLDragging++;
				}
				break;
			case 4:																	// 平行四辺形
				z_ptLDragging[0] = z_pLDragWndInfo->RPtoLP( z_ptRDragging[0]) + rev;
				z_ptLDragging[1] = z_pLDragWndInfo->RPtoLP( z_ptRDragging[1]) + rev;
				z_ptLDragging[3] = CPoint( z_ptLDragMouth.x, z_ptLDragMouth.y * iUpL) + rev;
				z_ptLDragging[2] = z_ptLDragging[1] + z_ptLDragging[3] - z_ptLDragging[0];
				z_ptLDragging[4] = z_ptLDragging[0];
				z_nLDragging = 5;

				// 形状が一直線に近い場合は線分２本に間引く
				v1 = MgVect2C( z_ptLDragging[1] - z_ptLDragging[0]); 
				v1.SetUnitize();
				v2 = MgVect2C( z_ptLDragging[2] - z_ptLDragging[1]);
				v2.SetUnitize();
				if ( MGeo::Abs(v1 ^ v2) < 0.05) {
					if ( (v1 * v2) < 0) {
						z_ptLDragging[0] = z_ptLDragging[1];
						z_ptLDragging[1] = z_ptLDragging[2];
						z_ptLDragging[2] = z_ptLDragging[3];
					}
					z_nLDragging = 3;
				}
				break;
			default:
				;
		}
	}
	if ( z_nLDragging != 0)
		dc.Polyline( z_ptLDragging, z_nLDragging);					// 図形ありの場合はラバーバンド図形を表示する
	if ( ictl == -1)
		z_nLDragging = 0;											// 削除後は現在表示中の図形なしとする

	dc.SetMapMode( iOldMM);
	dc.SelectObject( OldPen);
	return 1;
}

} // namespace MC
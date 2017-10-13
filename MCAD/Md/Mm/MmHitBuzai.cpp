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
#include "MgMat.h"
#include "MbDefine.h"
#include "MbCod.h"
#include "MmGridNum.h"
#define DLL_EXPORT_MC_HAITIIN_DO
#define DLL_EXPORT_MC_WINDOW_DO
#include "MmLib.h"

#include "MmDrag.h"
#include "MmWnd.h"

#include "MmDefine.h"
#include "MsBitSet.h"
#include "MmOpt.h"
#include "MmHist.h"

#include "MbList.h"
#include "MhLib.h"

#include "MhPlcInfo.h"
#include "MgGrp.h"
#include "MhRoof.h"
#include "MdLib.h"

#include "MmSystemProperty.h"

namespace MC
{

static	bool			z_fDrawHitBzi = FALSE;

/////////////////////////////////////////////////////////////////////////////
//  部材の形状を求める

static void PtsShape(
						MhPlcInfo	*pPlcEn,
						MgPolyg2*	pgPtsShape
				);

static	MgPoint2 z_ptBziFig[20];
static	MgPolyg2 z_pgBziFig( 20, z_ptBziFig);

////////////////////////////////////////////////////////////////////////////
//	カレント選択の部材形状を設定する

void Window::SetCurBziFig(
						 MgPolyg2*	ppgBziFig		// (I  ) カレント選択の部材形状
				)
{
	z_pgBziFig = *ppgBziFig;
}

////////////////////////////////////////////////////////////////////////////
//	カレント選択の部材形状をクリアする

void Window::ClrCurBziFig()
{
	z_pgBziFig.m_n = 0;
}

////////////////////////////////////////////////////////////////////////////
//	カレント選択の部材形状を返す
//	返値　カレント選択の部材形状

MgPolyg2* Window::GetCurBziFig()
{
	return &z_pgBziFig;
}

static	MhRfm* 	z_pRfm = NULL;

////////////////////////////////////////////////////////////////////////////
//	カレント選択の屋根面を設定する

void HaitiIn::SetCurRfm(
						MhRfm* 	pRfm				// (I  ) カレント選択の屋根面
				)
{
	z_pRfm = pRfm;
}

////////////////////////////////////////////////////////////////////////////
//	カレント選択の屋根面を返す

MhRfm* HaitiIn::GetCurRfm()
{
	return z_pRfm;
}

/////////////////////////////////////////////////////////////////////////////
//	テンポラリ表示

void WindowCtrl::MmWndKDrawTemp(
				MmWndInfo*		pWndInfo,
				MbCod*			pCod
				)
{
	// 屋根面の選択状況を表示する
	MgPolyg2	pgRfm(20);

	MhRfm *pRfm = HaitiIn::GetCurRfm();
	if ( pRfm) {

		CClientDC	dc( pWndInfo->GetWnd());
		DWORD		rgbTempRfm = MmSystemProperty::GetColor( MM_COLOR_TEMP);

		pCod->SetLineAttr( MPS_SOLID, 1, rgbTempRfm);

		pgRfm = pRfm->m_Pg;
		pCod->Polygon( pgRfm.m_p, pgRfm.m_n);
	}

	MgPolyg2 *ppgBziFig = Window::GetCurBziFig();
	if ( ppgBziFig->m_n != 0) {

		CClientDC	dc( pWndInfo->GetWnd());
		DWORD		rgbTempRfm = MmSystemProperty::GetColor( MM_COLOR_TEMP);

		pCod->SetLineAttr( MPS_SOLID, 1, rgbTempRfm);

		pCod->Polygon( ppgBziFig->m_p, ppgBziFig->m_n);
	}
}

/////////////////////////////////////////////////////////////////////////////
//	新規ウィンドウ作成時の初期化 (ビューの初期表示時に使用)
//	ヒット部材が表示されていない状態であることを設定する

void Window::InitHitBuzai()
{
	z_fDrawHitBzi = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//	ヒット部材の表示を消去する

void Window::EraseHitBzi()
{
	if ( z_fDrawHitBzi) {
		Window::DispHitBzi( -1, NULL, NULL);
		z_fDrawHitBzi = FALSE;
	}
	Window::ResetDispHitBzi();
}

/////////////////////////////////////////////////////////////////////////////
//	ヒット部材を表示する
//	（既表示ヒット部材を消去後に、ヒット部材を表示する）

void Window::DrawHitBzi(
						MmWndInfo*	pWndInfo,		// (I  ) ウィンドウ管理情報
						MgPolyg2*	pgHitBzi		// (I  ) ヒット部材の形状
				)
{
	// 既表示ヒット部材を消去
	if ( z_fDrawHitBzi)
		Window::DispHitBzi( -1, NULL, NULL);

	// ヒット部材を表示
	Window::DispHitBzi( 1, pWndInfo, pgHitBzi);
	z_fDrawHitBzi = TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	ヒット部材を再表示する

void Window::ReDispHitBzi()
{
	if ( !z_fDrawHitBzi) {
		Window::DispHitBzi( 0, NULL, NULL);
		z_fDrawHitBzi = TRUE;
	}
}

static	MmWndInfo* 		z_pHitBziWndInfo = NULL;					// ラバーバンド入力表示用カレントウィンドウ
static	MgPolyg2		z_pgHitBzi(10);

/////////////////////////////////////////////////////////////////////////////
//	ヒット部材の形状を表示/再表示/消去する

void Window::ResetDispHitBzi()
{
	z_pHitBziWndInfo = NULL;
}

/////////////////////////////////////////////////////////////////////////////
//	ヒット部材の形状を表示/再表示/消去する

MINT Window::DispHitBzi(
						MINT		ictl,			// (I  ) 表示制御 1 : 表示、0 : 再表示、-1 : 消去
						MmWndInfo*	pWndInfo,		// (I  ) ウィンドウ管理情報(表示制御 == 1) または NULL(表示制御 <= 0)
						MgPolyg2*	pgHitBzi		// (I  ) ヒット部材の形状(表示制御 == 1) または NULL(表示制御 <= 0)
				)
{
	MINT		ic1;
	CPoint		ptLHitBzi[10];

	MbCod*		pCod;

	if ( ictl == 1) {
		z_pHitBziWndInfo = pWndInfo;
		z_pgHitBzi = *pgHitBzi;													// ヒット部材の形状
	}
	if ( z_pHitBziWndInfo == NULL)
		return 0;

	pCod = z_pHitBziWndInfo;

	CClientDC	dc( z_pHitBziWndInfo->GetWnd());

	DWORD		rgbHitBzi = MmSystemProperty::GetColor( MM_COLOR_HITBZI);
	CPen		HitBziPen( PS_SOLID, 1, rgbHitBzi);
	CPen*		OldPen = dc.SelectObject( &HitBziPen);

	MINT		iMapMode = z_pHitBziWndInfo->GetMapMode();
	MINT		iOldMM = dc.SetMapMode( iMapMode);

	dc.SetROP2( R2_XORPEN);														// ドラッグ用色反転ペン

	for ( ic1=0; ic1<z_pgHitBzi.m_n; ic1++)
		ptLHitBzi[ic1] = z_pHitBziWndInfo->RPtoLP(z_pgHitBzi.m_p[ic1]);
	ptLHitBzi[ic1] = ptLHitBzi[0];
	dc.Polyline( ptLHitBzi, z_pgHitBzi.m_n + 1);

	dc.SetMapMode( iOldMM);
	dc.SelectObject( OldPen);
	return 1;
}

} // namespace MC
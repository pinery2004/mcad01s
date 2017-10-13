//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MmWnd.cpp
//
//		ウィンドウ管理
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"
#include "MmSystem.h"
#include "MgLib.h"
#include "MgMat.h"
#include "MbDefine.h"
#include "MbCod.h"
#include "MmGridNum.h"
#include "MmDrag.h"
#define DLL_EXPORT_WNDINFO_DO
#define DLL_EXPORT_MC_WINDOWCTRL_DO
#include "MmWnd.h"
#include "MmSystemProperty.h"

#include "resource.h"
#include "..\\MCAD\\resource.h"

//S #include "MgMapMode.h"
#include "MbList.h"
#include "MainFrm.h"

#define		AKIG		600.f

namespace MC
{

static	MbListE		z_lWnd( 10, sizeof( MmWndInfo)+10);			// ウィンドウ管理情報
static	MmWndInfo	z_wWndC;									// カレントウィンドウ管理情報
static	MPOSITION	z_WndPos;

/////////////////////////////////////////////////////////////////////////////
//	先頭のウィンドウを取得する
//	返値 =NULL: レコードなし !=NULL: 読み込み[部品配置]位置
MmWndInfo* WindowCtrl::MmGetHeadWnd( MPOSITION *pWndPos)
{
	if ( pWndPos)
		return (MmWndInfo*)z_lWnd.GetHead( pWndPos);
	else
		return (MmWndInfo*)z_lWnd.GetHead( &z_WndPos);
}

/////////////////////////////////////////////////////////////////////////////
//	次のウィンドウを取得する
//	返値 =NULL: レコードなし !=NULL: 読み込み[部品配置]位置
MmWndInfo* WindowCtrl::MmGetNextWnd( MPOSITION *pWndPos)
{
	if ( pWndPos)
		return (MmWndInfo*)z_lWnd.GetNext( pWndPos);
	else
		return (MmWndInfo*)z_lWnd.GetNext( &z_WndPos);
}


/////////////////////////////////////////////////////////////////////////////
//	カレントのウィンドウ種別を設定
//						(1st ウィンドウ作成開始時に設定)

void	WindowCtrl::MmWndSetSyubetsuC(
						MINT		iWndSyu,		// (I  ) ウィンドウ種類
						MINT		iZuSyu,			// (I  ) 図種類
						MINT		iKai,			// (I  ) 階
						MINT		iZuNum			// (I  ) 図番号
				) 
{
	z_wWndC.SetWndSyu( iWndSyu);
	z_wWndC.SetZuSyu( iZuSyu);
	z_wWndC.SetKai( iKai);
	z_wWndC.SetZuNum( iZuNum);
}

/////////////////////////////////////////////////////////////////////////////
//	カレントのフレームのインスタンスを設定
//						(2nd 子フレーム作成開始時に設定)

void	WindowCtrl::MmWndSetFrameC(
						MINT		iWndSyu,		// (I  ) ウィンドウ種類
						CMDIChildWnd* pChildFrm		// (I  ) フレームのインスタンス
				)
{
	_ASSERTE( iWndSyu == z_wWndC.m_iWndSyu);
	z_wWndC.m_pChildFrm = pChildFrm;
}

/////////////////////////////////////////////////////////////////////////////
//	カレントのウィンドウのインスタンスを設定
//						(3rd 子ビュー作成開始時に設定)

void	WindowCtrl::MmWndSetWndC(
						MINT		iWndSyu,		// (I  ) ウィンドウ種類
						CWnd*		pWnd			// (I  ) ウィンドウのインスタンス
				)
{
	_ASSERTE( iWndSyu == z_wWndC.m_iWndSyu);
	z_wWndC.m_pWnd = pWnd;
}

/////////////////////////////////////////////////////////////////////////////
//	ウィンドウ管理情報の作成

MmWndInfo::MmWndInfo(
						MINT		iWndSyu,		// (I  ) ウィンドウ種類
						MINT		iZuSyu,			// (I  ) 図種類
						MINT		iKai,			// (I  ) 階
						MINT		iZuNum			// (I  ) 図番号
				)
{
	m_iWndSyu		= iWndSyu;									// ウィンドウ種類
	m_iZuSyu		= iZuSyu;									// 図種類
	m_iKai			= iKai,										// 階
	m_iZuNum		= iZuNum;									// 図番号
	m_iHoko			= 0;										// 方向
	m_pWnd			= NULL;										// ウィンドウのインスタンス
	m_pDrag			= NULL;										// ドラッグ操作情報
	m_pMemBmp		= NULL;										// カレントウィンドウイメージビットマップ
	m_pOldMemBmp	= NULL;										// 保存用ウィンドウイメージビットマップ
	m_pMemDC		= NULL;										// ウィンドウイメージメモリＤＣ

	SetMinMaxD( MgMinMaxI2( 0, 0, 100, 100));					//	表示座標(Min/Max)

	SetMinMaxRS( MgMinMaxR2( 0, 0, 100., 100.));				//	実座標(Min/Max)スタック
	SetMat();
}

MmWndInfo::~MmWndInfo      ()
{
//	DeleteWnd();
//	MBDELETE( m_pMemBmp);
//	MBDELETE( m_pMemDC);
}

/////////////////////////////////////////////////////////////////////////////
//	ドラッグ操作情報の削除

void MmWndInfo::DeleteDrag()
{ 
	if ( m_pDrag) {
//	    KillTimer(RealTimeTimer_id);
	    delete m_pDrag;
		m_pDrag = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
//	ウィンドウイメージメモリＤＣの作成
//
//	次の手順で使用
//
//		CreateMDC		(AsignMDCを含む)
//			FreeMDC		(繰り返し)
//			AsignMDC	(繰り返し)
//		DeleteMDC		(FreeMDCを含む)
//

void MmWndInfo::CreateMDC()
{
	m_pMemBmp		= new CBitmap;
	m_pMemDC		= new CDC;
	SetDC( m_pMemDC);
}

/////////////////////////////////////////////////////////////////////////////
//	ウィンドウイメージメモリＤＣの削除

void MmWndInfo::DeleteMDC()
{
	FreeMDC();
	MBDELETE( m_pMemBmp);
	MBDELETE( m_pMemDC);
}

/////////////////////////////////////////////////////////////////////////////
//	ウィンドウイメージメモリＤＣの割り当て

void MmWndInfo::AsignMDC()
{
	if ( m_pMemDC->m_hDC == NULL) {
		CClientDC		dc( m_pWnd);
	
		m_pMemDC->CreateCompatibleDC( &dc);													// ウィンドウＤＣの属性設定
		m_pMemBmp->CreateCompatibleBitmap( &dc, m_iMinMaxD.max.x, -m_iMinMaxD.min.y);		// ウィンドウイメージビットマップの作成
		m_pOldMemBmp = m_pMemDC->SelectObject( m_pMemBmp);									// ウィンドウイメージビットマップを割り付け
	}
}

/////////////////////////////////////////////////////////////////////////////
//	ウィンドウイメージメモリＤＣの開放

void MmWndInfo::FreeMDC()
{
	if ( m_pOldMemBmp)
		m_pMemDC->SelectObject( m_pOldMemBmp);
	if ( m_pMemDC)
		m_pMemDC->DeleteDC();
	if ( m_pMemBmp)
		m_pMemBmp->DeleteObject();
}

/////////////////////////////////////////////////////////////////////////////
//	メモリＤＣのウィンドウをクリアする

void MmWndInfo::ClearMDC()
{
	CBrush	brushBG = MmSystemProperty::GetColor( MM_COLOR_BACKGROUND);
	CBrush* pOldBrush = m_pMemDC->SelectObject( &brushBG);

	MINT iOldMM = SetMapMode( MMAPMODE);						// マップモードを初期設定する

	m_pMemDC->PatBlt( 0, 0, m_iMinMaxL.max.x, m_iMinMaxL.min.y * GetUpL(), PATCOPY);	// 背景色設定

	m_pMemDC->SetMapMode( iOldMM);
	m_pMemDC->SelectObject( pOldBrush);
}
/////////////////////////////////////////////////////////////////////////////
//	ウィンドウ管理
/*
/////////////////////////////////////////////////////////////////////////////
//	初期化

MINT WindowCtrl::MmWndKInit        ()				// (  O) 0 : 正常  1 : ｴﾗｰ
{
	MmWndKDeleteAll();								// ﾘｽﾄを破棄する

	return(0);
}

/////////////////////////////////////////////////////////////////////////////
//	終了

MINT WindowCtrl::MmWndKTerm        ()				// (  O) 0 : 正常  1 : ｴﾗｰ
{
	MmWndKDeleteAll();								// ﾘｽﾄを破棄する

	return(0);
}
*/
/////////////////////////////////////////////////////////////////////////////
//  カレントウィンドウを追加する
//						(4th 子ビュー作成開始時にMmWndSetWndC実行後に設定)

MINT WindowCtrl::MmWndKAddC()						// (  O) 0 : 正常  1 : ｴﾗｰ
{
	MINT		ist = 1;
	MmWndInfo	WndInfo(z_wWndC.m_iWndSyu, z_wWndC.m_iZuSyu, z_wWndC.m_iKai, z_wWndC.m_iZuNum);

	WndInfo.SetCursor( IDC_CROSS1);

	WndInfo.SetWnd( z_wWndC.m_pWnd);
	WndInfo.SetFrame( z_wWndC.m_pChildFrm);

	if ( WindowCtrl::MmWndKFindSyu( z_wWndC.m_iWndSyu, z_wWndC.m_iZuSyu, z_wWndC.m_iKai, z_wWndC.m_iZuNum) == NULL) {
		WndInfo.CreateMDC();
		z_lWnd.Inject( &WndInfo);
		ist = 0;
	}
	return( ist);
}

/////////////////////////////////////////////////////////////////////////////
//  ウィンドウを追加する

MINT WindowCtrl::MmWndKAdd(							// (  O) 0 : 正常  1 : ｴﾗｰ
						MINT		iWndSyu,		// (I  ) ウィンドウ種類(1,2,3,4,5)
						MINT		iZuSyu,			// (I  ) 図種類
						MINT		iKai,			// (I  ) 階
						MINT		iZuNum			// (I  ) ウィンドウ情報図番号
				)
{
	MINT		ist = 1;
	MmWndInfo	*pWndInfo = new MmWndInfo( iWndSyu, iZuSyu, iKai, iZuNum);

	if ( WindowCtrl::MmWndKFindSyu( iWndSyu, iZuSyu, iKai, iZuNum) == NULL) {
		z_lWnd.Inject( pWndInfo);
		ist = 0;
	}
	return( ist);
}

/////////////////////////////////////////////////////////////////////////////
//  ウィンドウを削除する

MINT WindowCtrl::MmWndKDelete(						// (  O) ｽﾃｰﾀｽ  0 : 正常  1 : ｴﾗｰ
						MmWndInfo*	pWndInfo		// (I  ) ウィンドウ管理情報
				)
{
	MINT		ist = 1;
	MPOSITION	nPos;

	if ( pWndInfo) {
		pWndInfo->DeleteDrag();
		pWndInfo->DeleteMDC();

		nPos = z_lWnd.Find( pWndInfo, 0);
		if ( nPos) {
			z_lWnd.RemoveAt( &nPos);
			ist = 0;
		}
	}
	return( ist);
}

/////////////////////////////////////////////////////////////////////////////
//  ウィンドウを検索する

MmWndInfo* WindowCtrl::MmWndKFindSyu(				// (  O) ウィンドウ情報へのﾎﾟｲﾝﾀ
						MINT		iWndSyu,		// (I  ) ウィンドウ種類(1,2,3,4,5)
						MINT		iZuSyu,			// (I  ) 図種類
						MINT		iKai,			// (I  ) 階
						MINT		iZuNum			// (I  ) ウィンドウ情報図番号
				)
{
	MPOSITION			Pos;
	MmWndInfo*			pWndInfo;

	for (pWndInfo = (MmWndInfo*)z_lWnd.GetHead( &Pos);
			pWndInfo != 0; 
				pWndInfo = (MmWndInfo*)z_lWnd.GetNext( &Pos)) {
		if ( (pWndInfo->m_iWndSyu == 0 || iWndSyu == 0 || pWndInfo->m_iWndSyu == iWndSyu) &&
			 (pWndInfo->m_iZuSyu  == 0 || iZuSyu  == 0 || pWndInfo->m_iZuSyu  == iZuSyu) &&
			 (pWndInfo->m_iKai    == 0 || iKai    == 0 || pWndInfo->m_iKai    == iKai) &&
			 (pWndInfo->m_iZuNum  == 0 || iZuNum  == 0 || pWndInfo->m_iZuNum  == iZuNum))
			return pWndInfo;
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
//  ウィンドウを検索する

MmWndInfo* WindowCtrl::MmWndKFindWnd(							// (  O) ウィンドウ情報へのﾎﾟｲﾝﾀ
						CWnd*		pWnd			// (I  ) ウィンドウのインスタンス
				)
{
	MPOSITION			Pos;
	MmWndInfo*			pWndInfo2;

	for (pWndInfo2=(MmWndInfo*)z_lWnd.GetHead( &Pos); pWndInfo2!=0; pWndInfo2=(MmWndInfo*)z_lWnd.GetNext( &Pos)) {
		if ( pWndInfo2->GetWnd() == pWnd)
			return pWndInfo2;
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
//  ウィンドウを検索する

MmWndInfo* WindowCtrl::MmWndKFindFrm(							// (  O) ウィンドウ情報へのﾎﾟｲﾝﾀ
						CMDIChildWnd* pChildFrm		// (I  ) フレームのインスタンス
				)
{
	MPOSITION			Pos;
	MmWndInfo*			pWndInfo2;

	for (pWndInfo2=(MmWndInfo*)z_lWnd.GetHead( &Pos); pWndInfo2!=0; pWndInfo2=(MmWndInfo*)z_lWnd.GetNext( &Pos)) {
		if ( pWndInfo2->GetFrame() == pChildFrm)
			return pWndInfo2;
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
//	全ウィンドウを削除する

MINT WindowCtrl::MmWndKDeleteAll()								// (  O) ステイタス 0 : 正常  1 : エラー
{
	MINT				ist;
	MPOSITION			Pos;
	MmWndInfo*			pWndInfo2;

	for (pWndInfo2=(MmWndInfo*)z_lWnd.GetHead( &Pos); pWndInfo2!=0; pWndInfo2=(MmWndInfo*)z_lWnd.GetHead( &Pos)) {
		ist = MmWndKDelete( pWndInfo2);
		if ( ist) return ist;
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
//	カレントのウィンドウを取得する

MmWndInfo* WindowCtrl::MmWndKGetCurWnd()						// (  O) ステイタス  NULL : エラー 
{
	CMainFrame*			pMainFrm;					// メインフレーム
	CMDIChildWnd*		pChildFrm;					// 子フレーム
	MmWndInfo*			pWndInfo = NULL;

//	pMainFrm = (CMDIFrameWnd *)AfxGetMainWnd();					// メインフレーム取得
//	if ( pMainFrm == NULL) MQUIT;
	pMainFrm = System::GetpMainFrame();

//	pChildFrm = pMainFrm->MDIGetActive();						// アクティブな子フレームを取る
//	if ( pChildFrm == NULL) MQUIT;
	pChildFrm = pMainFrm->m_pCurFrame;

	pWndInfo = MmWndKFindFrm( pChildFrm);

//exit:

	return( pWndInfo);
}

} // namespace MC
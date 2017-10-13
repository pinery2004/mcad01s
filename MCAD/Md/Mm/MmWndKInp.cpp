//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MmWndKInp.cpp
//
//		ウィンドウ入力関連
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

#define DLL_EXPORT_MC_VIEWINP_DO
#define DLL_EXPORT_MC_WINDOW_DO
#include "MmLib.h"
#include "MmPrompt.h"

#include "MtAskForPoint.h"
#include "MtDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace MC
{

/////////////////////////////////////////////////////////////////////////////
//	コマンドメニュー処理
void WindowCtrl::MmWndKCmdXqt(
						MINT		i_idCmd			// コマンドID
				) 
{
	MtSendMessage( WM_CMDMENUP, i_idCmd, NULL);
}

/////////////////////////////////////////////////////////////////////////////
//  マウス左ボタンダウン処理
MINT ViewInp::LButtonDown(
						CWnd*		pWnd,			// (I  ) ウィンドウのインスタンス
						UINT 		nFlags,			// (I  ) 
						CPoint		ptMouthD		// (I  ) 表示座標
				) 
{
	MINT		ist = 0;

	MmWndInfo*	pWndInfo = WindowCtrl::MmWndKFindWnd( pWnd);

	MgPoint2	ptMouthR = pWndInfo->DPtoRP( ptMouthD);			// 実座標へ変換

	// ドラッギングズーム操作
	if ( CONTROL_FLAGS(nFlags)) {								// コントロールキー
		if ( pWndInfo->GetDrag()) {
			if ( pWndInfo->GetDrag()->GetMotion() == ZOOM_WAIT) {
				pWndInfo->GetDrag()->SetMotion( ZOOM);
//				pWndInfo->GetDrag()->SetStartPt( ptMouthR);		// ズームの中心を左ボタンダウン位置にする
																// コメントにするとコントロールキーを押下時の
																// 最初の左ボタンダウン位置にする
				pWndInfo->GetDrag()->SetLastPt( ptMouthR);		// 左ボタンアップでのマウスの移動量を無しにする
			} else {
				pWndInfo->DeleteDrag();
			}
		}
		if ( pWndInfo->GetDrag() == NULL) {
			pWndInfo->SetDrag( new MmDragZoom( pWndInfo, ptMouthR));
		}
// 070907		return ist;
	}

	// コマンドキュー
	MtSendMessage( WM_LBUTTONDOWN, nFlags, &ptMouthR);
	return ist;
}

/////////////////////////////////////////////////////////////////////////////
//  マウス左ボタンアップ処理
MINT ViewInp::LButtonUp(
						CWnd*		pWnd,			// (I  ) ウィンドウのインスタンス
						UINT 		nFlags,			// (I  ) 
						CPoint		ptMouthD		// (I  ) 表示座標
				) 
{
	MINT		ist = 0;

	MmWndInfo* pWndInfo = WindowCtrl::MmWndKFindWnd( pWnd);

	// ドラッギングズーム操作
	if ( pWndInfo->GetDrag()) {
		if ( pWndInfo->GetDrag()->GetMotion() == ZOOM)
			pWndInfo->GetDrag()->SetMotion( ZOOM_WAIT);
		else
			pWndInfo->DeleteDrag();
	}

	// コマンドキュー		add 070907
	MgPoint2	ptMouthR = pWndInfo->DPtoRP( ptMouthD);			// 実座標へ変換
	MtSendMessage( WM_LBUTTONUP, nFlags, &ptMouthR);
	return ist;
}

/////////////////////////////////////////////////////////////////////////////
//  マウス右ボタンダウン処理

MINT ViewInp::RButtonDown(
						CWnd*		pWnd,			// (I  ) ウィンドウのインスタンス
						UINT 		nFlags,			// (I  ) 
						CPoint		ptMouthD		// (I  ) 表示座標
				) 
{
	MINT				ist = 0;

	MmWndInfo*	pWndInfo = WindowCtrl::MmWndKFindWnd( pWnd);

	MgPoint2	ptMouthR = pWndInfo->DPtoRP( ptMouthD);			// 実座標へ変換

	// ドラッギング移動操作開始
	pWndInfo->DeleteDrag();

	if ( CONTROL_FLAGS(nFlags)) {
		pWndInfo->SetDrag( new MmDragPan( pWndInfo, ptMouthR));
// 070907		return ist;
	}

	// コマンドキュー
	MtSendMessage( WM_RBUTTONDOWN, nFlags, &ptMouthR);

	return ist;
}

/////////////////////////////////////////////////////////////////////////////
//  マウス右ボタンアップ処理
MINT ViewInp::RButtonUp(
						CWnd*		pWnd,			// (I  ) ウィンドウのインスタンス
						UINT 		nFlags,			// (I  ) 
						CPoint		ptMouthD		// (I  ) 表示座標
				) 
{
	MINT		ist = 0;
	MmWndInfo* pWndInfo = WindowCtrl::MmWndKFindWnd( pWnd);

	// ドラッギング移動操作終了
	pWndInfo->DeleteDrag();

	// コマンドキュー		add 070907
	MgPoint2	ptMouthR = pWndInfo->DPtoRP( ptMouthD);			// 実座標へ変換
	MtSendMessage( WM_RBUTTONUP, nFlags, &ptMouthR);
	return ist;
}

/////////////////////////////////////////////////////////////////////////////
//  マウス左ボタンダブルクリック処理
MINT ViewInp::LButtonDblClk(
						CWnd*		pWnd,			// (I  ) ウィンドウのインスタンス
						UINT 		nFlags,			// (I  ) 
						CPoint		ptMouthD		// (I  ) 表示座標
				) 
{
	MINT		ist = 0;

	MmWndInfo*	pWndInfo = WindowCtrl::MmWndKFindWnd( pWnd);

	MgPoint2	ptMouthR = pWndInfo->DPtoRP( ptMouthD);			// 実座標へ変換

	// コマンドキュー
	MtSendMessage( WM_LBUTTONDBLCLK, nFlags, &ptMouthR);
	return ist;
}

/////////////////////////////////////////////////////////////////////////////
//  マウス右ボタンダブルクリック処理
MINT ViewInp::RButtonDblClk(
						CWnd*		pWnd,			// (I  ) ウィンドウのインスタンス
						UINT 		nFlags,			// (I  ) 
						CPoint		ptMouthD		// (I  ) 表示座標
				) 
{
	MINT		ist = 0;

	MmWndInfo*	pWndInfo = WindowCtrl::MmWndKFindWnd( pWnd);

	MgPoint2	ptMouthR = pWndInfo->DPtoRP( ptMouthD);			// 実座標へ変換

	// コマンドキュー
	MtSendMessage( WM_RBUTTONDBLCLK, nFlags, &ptMouthR);
	return ist;
}

/////////////////////////////////////////////////////////////////////////////
//  マウス中ボタンダブルクリック処理
MINT ViewInp::MButtonDblClk(
						CWnd*		pWnd,			// (I  ) ウィンドウのインスタンス
						UINT 		nFlags,			// (I  ) 
						CPoint		ptMouthD		// (I  ) 表示座標
				) 
{
	MINT		ist = 0;

	MmWndInfo*	pWndInfo = WindowCtrl::MmWndKFindWnd( pWnd);

	MgPoint2	ptMouthR = pWndInfo->DPtoRP( ptMouthD);			// 実座標へ変換

	// コマンドキュー
	MtSendMessage( WM_MBUTTONDBLCLK, nFlags, &ptMouthR);
	return ist;
}

static	bool	z_fDispSelPts;					// マウスで選択部材の表示フラグ
static	MINT	z_iCdBuzai;							// マウスで選択対象の部材コード

/////////////////////////////////////////////////////////////////////////////
//  マウス選択部材の表示制御
void Window::SetDispSelPts(
						MINT 		iCdBuzai		// (I  ) マウスで選択対象の部材コード、NULL(全)、または -1(非表示)
				) 
{
	if ( iCdBuzai >= 0) {
		z_fDispSelPts = true;
		z_iCdBuzai = iCdBuzai;
	} else {
		z_fDispSelPts = false;
		z_iCdBuzai = NULL;
		Window::EraseHitBzi();
		Window::ResetDispHitBzi();
	}
}

/////////////////////////////////////////////////////////////////////////////
//	マウス選択部材を再表示する
void Window::ReDrawHitBzi()
{
	if ( z_fDispSelPts) {
		Window::ReDispHitBzi();
	}
}

/////////////////////////////////////////////////////////////////////////////
//  マウス移動処理
MINT ViewInp::MouseMove(
						CWnd*		pWnd,			// (I  ) ウィンドウのインスタンス
						UINT 		nFlags,			// (I  ) 
						CPoint		ptMouthD			// (I  ) 表示座標
				) 
{
// 070907	MINT		ist = 1;
	MINT		ist = 0;
	MCHAR		strPtMouthR[20];
	CPoint		ptMouthL;
	MgPoint2	ptMouthR;
	MgPolyg2	pgHitBzi;

	MmWndInfo*	pWndInfo = WindowCtrl::MmWndKFindWnd( pWnd);

	MINT iOldMM = pWndInfo->SetMapMode( NULL);

	ptMouthL = pWndInfo->DPtoLP( ptMouthD);						// 論理座標へ変換
	ptMouthR = pWndInfo->LPtoRP( ptMouthL);						// 実座標へ変換

	pWndInfo->SetCurMousePtL( ptMouthL);						// ウィンドウ管理情報の現在のマウス位置を設定

	// ドラッギング操作
// 070907	if ( pWndInfo->GetDrag() && !pWndInfo->GetDrag()->m_bContinuousOrbitStarted) {
	if ( pWndInfo->GetDrag()) {
		pWndInfo->GetDrag()->Continue( pWndInfo, ptMouthR);		// ドラッギング操作
	}
	// クロスヘアーカーソル表示
	Window::EraseCrossHair();
//UTy	Window::DrawCrossHair( pWndInfo, ptMouthL);		//UT01この行を追加するとカーソル移動のレスポンスが悪くなる
//	Window::EraseCrossHair();
//	Window::DrawCrossHair( pWndInfo, ptMouthL);		//UT01この行を追加するとカーソル移動のレスポンスが悪くなる
	// ラバーバンド図形表示
	Window::EraseDragging();
	Window::DrawDragging( pWndInfo, ptMouthR);
	// ヒット部材形状表示
	if ( z_fDispSelPts) {
		if ( HaitiIn::SrchBuzai( pWndInfo, ptMouthR, z_iCdBuzai, &pgHitBzi)) {
			Window::DrawHitBzi( pWndInfo, &pgHitBzi);
		} else {
			Window::EraseHitBzi();
		}
	}

	// ステイタスバーにマウス位置座標表示
	Msprintf(strPtMouthR, Mstr( " %6.0f: %6.0f"),
						  ptMouthR.x, ptMouthR.y);
	Msg::DispSelStatusBar( MC_STSBAR_COORDINATE, strPtMouthR);						// ステイタスバーへマウス座標を表示

	pWndInfo->GetMemDC()->SetMapMode( iOldMM);
//070907	ist = 0;

	// コマンドキュー		add 070907
//G	MtSendMessage( WM_MOUSEMOVE, nFlags, &ptMouthR);
	return ist;
}

/////////////////////////////////////////////////////////////////////////////
//  マウス中ボタンダウン処理
MINT ViewInp::MButtonDown(
						CWnd*		pWnd,			// (I  ) ウィンドウのインスタンス
						UINT 		nFlags,			// (I  ) 
						CPoint		ptMouthD		// (I  ) 表示座標
				) 
{
	MINT		ist = 0;

	MmWndInfo*	pWndInfo = WindowCtrl::MmWndKFindWnd( pWnd);

// 070907	MINT iOldMM = pWndInfo->SetMapMode( NULL);
	
	MgPoint2	ptMouthR = pWndInfo->DPtoRP( ptMouthD);			// 実座標へ変換

	// ドラッギング移動操作
	pWndInfo->DeleteDrag();
	pWndInfo->SetDrag( new MmDragPan( pWndInfo, ptMouthR));

// 070907	pWndInfo->GetMemDC()->SetMapMode( iOldMM);

	// コマンドキュー		add 070907
	MtSendMessage( WM_MBUTTONDOWN, nFlags, &ptMouthR);
	return ist;
}

/////////////////////////////////////////////////////////////////////////////
//  マウス中ボタンアップ処理
MINT ViewInp::MButtonUp(
						CWnd*		pWnd,			// (I  ) ウィンドウのインスタンス
						UINT 		nFlags,			// (I  ) 
						CPoint		ptMouthD		// (I  ) 表示座標
				) 
{
	MINT				ist = 0;

	MmWndInfo*	pWndInfo = WindowCtrl::MmWndKFindWnd( pWnd);

	// ドラッギング移動操作終了
	pWndInfo->DeleteDrag();

	// コマンドキュー		add 070907
	MgPoint2	ptMouthR = pWndInfo->DPtoRP( ptMouthD);			// 実座標へ変換
	MtSendMessage( WM_MBUTTONUP, nFlags, &ptMouthR);
	return ist;
}

/////////////////////////////////////////////////////////////////////////////
//  マウスホイール回転処理
MINT ViewInp::MouseWheel(
						CWnd*		pWnd,			// (I  ) ウィンドウのインスタンス
						UINT 		nFlags,			// (I  ) 
						short		zDelta,			// (I  ) 
						CPoint		ptMouthD		// (I  ) 表示座標
				)
{
	MINT		ist = 0;

	MmWndInfo*	pWndInfo = WindowCtrl::MmWndKFindWnd( pWnd);

	MgPoint2	ptMouthR = pWndInfo->DPtoRP( ptMouthD);			// 実座標へ変換

	// ドラッギングズーム操作開始
	pWndInfo->DeleteDrag();
	pWndInfo->SetDrag( new MmDragZoom( pWndInfo, ptMouthR));

	// ドラッギングズーム操作
	MgMinMaxR2 rMinMaxR = pWndInfo->GetMinMaxRS();
	MREAL dHeight = rMinMaxR.GetHgt() / 10;
	if ( zDelta < 0)
		dHeight = - dHeight;

//	ptMouthR += MgVect2( 0., dHeight);
//	pWndInfo->GetDrag()->Continue( pWndInfo, ptMouthR);
	// change 070907
	MgPoint2	ptMouthRW = ptMouthR + MgVect2( 0., dHeight);
	pWndInfo->GetDrag()->Continue( pWndInfo, ptMouthRW);

	// ドラッギング移動操作終了
	pWndInfo->DeleteDrag();

	// コマンドキュー		add 070907
	MtSendMessage( WM_MOUSEWHEEL, nFlags, &ptMouthR);
	return ist;
}

/////////////////////////////////////////////////////////////////////////////
//	ウィンドウのサイズ設定
void ViewInp::Size(									// (  O) ステイタス　0 : 正常  1 : エラー
						CWnd*		pWnd,			// (I  ) ウィンドウのインスタンス
						CPoint		ptMouthD,		// (I  ) 画面サイズ(表示座標)
						CPoint		ptMouthL		// (I  ) 画面サイズ(論理座標)
				)
{
	MmWndInfo* pWndInfo = WindowCtrl::MmWndKFindWnd( pWnd);

	pWndInfo->SetWinD( 0, 0, ptMouthD.x, ptMouthD.y);				// 表示画面窓枠を設定
	pWndInfo->SetWinL( 0, 0, ptMouthL.x, ptMouthL.y);				// 論理表示画面窓枠を設定
	pWndInfo->SetMinMaxRS( pWndInfo->GetMinMaxGA( pWndInfo));		// 実座標(Min/Max)
	pWndInfo->SetMat();												// 座標変換用マトリックスの設定

	pWndInfo->FreeMDC();

//	Window::ResetDispHitBzi();
//	Window::InitHitBuzai();

	WindowCtrl::MmWndKDrawMDC( pWndInfo);
}

/////////////////////////////////////////////////////////////////////////////
//  キーボードキャラクタ入力処理
MINT ViewInp::Char(
						CWnd*		pWnd,			// (I  ) ウィンドウのインスタンス
						UINT		nChar,			// (I  )
						UINT		nRepCnt,		// (I  )
						UINT 		nFlags			// (I  ) 
				)
{
	MINT	ist = 0;
	MINT	iChar[2];

	// コマンドキュー
	iChar[0] = nChar;											// Virtual-Key Cords 
	iChar[1] = nRepCnt;
	MtSendMessage( WM_CHAR, nFlags, (MgPoint2 *)iChar);
	return ist;
}

/////////////////////////////////////////////////////////////////////////////
//  キーボードキーアップ処理
MINT ViewInp::KeyUp(
						CWnd*		pWnd,			// (I  ) ウィンドウのインスタンス
						UINT		nChar,			// (I  )
						UINT		nRepCnt,		// (I  )
						UINT 		nFlags			// (I  ) 
				)
{
	MINT		ist = 0;
	MmWndInfo*	pWndInfo;
	MINT	iChar[2];

	// コントロールキーのアップならドラッギング操作終了
	if ( nChar == 17) {
		pWndInfo = WindowCtrl::MmWndKFindWnd( pWnd);
		pWndInfo->DeleteDrag();
	}

	// コマンドキュー		add 070907
	iChar[0] = nChar;											// Virtual-Key Cords 
	iChar[1] = nRepCnt;
	MtSendMessage( WM_KEYUP, nFlags, (MgPoint2 *)iChar);
	return ist;
}

/////////////////////////////////////////////////////////////////////////////
//  キーボードキーダウン処理
MINT ViewInp::KeyDown(
						CWnd*		pWnd,			// (I  ) ウィンドウのインスタンス
						UINT		nChar,			// (I  )
						UINT		nRepCnt,		// (I  )
						UINT 		nFlags			// (I  ) 
				)
{
	MINT		ist = 0;
	MINT		iChar[2];

	// コマンドキュー		add 070907
	iChar[0] = nChar;											// Virtual-Key Cords 
	iChar[1] = nRepCnt;
	MtSendMessage( WM_KEYDOWN, nFlags, (MgPoint2 *)iChar);
	return ist;
}

} // namespace MC
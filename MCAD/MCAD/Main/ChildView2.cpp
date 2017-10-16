////////////////////////////////////////////////////////////////////////////
// ChildView2.cpp : 実装ファイル
//

#include "stdafx.h"
#include "MCAD.h"

#include "MgMat.h"
#include "MbCod.h"
#include "MmGridNum.h"
#include "MmDrag.h"
#include "MmWnd.h"

#include	< GL/gl.h >
#include	< GL/glu.h >
#include	"GlLib.h"

#include "ChildView2.h"

#define DEBUG1 0

namespace MC
{

//S void MhGetBCtr(	MREAL *rB, MC::MgPoint3 *PtCtr);

void ModelDraw( void);

void MsSetLight_MaterialParameter1();

void SetLightModel1();
void SetLight();
void DrawDisplayList( int iWire);

void ShowError( int e);

} // namespace MC

// CChildView2

static const MCHAR* pErrorStrings[] = {
    {Mstr( "No Error")},                   // 0
    {Mstr( "Unable to get a CD")},         // 1
    {Mstr( "ChoosePixelFormat failed")},   // 2
    {Mstr( "SelectPixelFormat failed")},   // 3
    {Mstr( "wglCreateContext failed")},    // 4
    {Mstr( "wglCreateCurrect failed")},    // 5
    {Mstr( "wglDeleteCotext failed")},     // 6
    {Mstr( "SwapBuffer failed")},          // 7
};

IMPLEMENT_DYNAMIC( CChildView2, CWnd)

CChildView2::CChildView2() : m_pDC( 0)
{
}

CChildView2::~CChildView2()
{
}

BEGIN_MESSAGE_MAP( CChildView2, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
//	ON_WM_CLOSE()
END_MESSAGE_MAP()

// CChildView2 メッセージ ハンドラ

BOOL CChildView2::PreCreateWindow( CREATESTRUCT& cs)
{
	MINT	ist;
	if ( !CWnd::PreCreateWindow( cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor( NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);


	MC::WindowCtrl::MmWndSetWndC( 2, this);										// ウィンドウ管理
	ist = MC::WindowCtrl::MmWndKAddC();											// ウィンドウ管理
	_ASSERTE( ist == 0);														// ウィンドウ管理

	return TRUE;
}

void CChildView2::OnPaint()
{
	CPaintDC dc(this); // 描画のデバイス コンテキスト
	
	SetWindowPos(&wndTopMost , 0, 0, 0, 0,
		SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE | SWP_NOZORDER);

//	MC::MmWndInfo* pWndInfo = MC::WindowCtrl::MmWndKFind( this);					// ウィンドウ管理
	MC::MmWndInfo* pWndInfo = MC::WindowCtrl::MmWndKFindSyu( 2, MTHEIMENZU, 1, 1);	// ウィンドウ管理
	(pWndInfo->GetFrame())->SetWindowText(Mstr( "WINDOW_02"));	// ウィンドウ管理

	// 描画メッセージで CWnd::OnPaint() を呼び出さないでください。

#if( DEBUG1)
	GlLib::DrawGLobject();
	SwapBuffers( m_pDC->m_hDC);    // Double buffer
#else

	m_M3View.OnPaint();
	if( FALSE == SwapBuffers( m_pDC->GetSafeHdc())) MC::ShowError(7);	// Orig
//	SwapBuffers( m_pDC->m_hDC);    // Double buffer
#endif
}

BOOL CChildView2::Create( LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
						  DWORD dwStyle, const RECT& rect,
						  CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// Setup the shared menu

	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

////////////////////////////////////////////////////////////////////////////

int CChildView2::OnCreate( LPCREATESTRUCT lpCreateStruct)
{
//	MREAL		rB;								// 表示倍率
	MC::MgPoint3	PtCtr;						// 中心座標

	if (CWnd::OnCreate( lpCreateStruct) == -1)
		return -1;

	//	DCの生成
	m_pDC = new CClientDC( this);
	if( NULL == m_pDC ){
		MC::ShowError( 1);
		return FALSE;
	}
	m_M3View.OnCreate( m_pDC->m_hDC);

#if( DEBUG1)
	GlLib::SetDCPixelFormat( m_pDC->m_hDC);						// OpenGL用にPixel Formatを指定
	m_GLRC = wglCreateContext( m_pDC->m_hDC);					// Rendering contextの生成
	wglMakeCurrent( m_pDC->m_hDC, m_GLRC);						// 現在のcontext設定

	GlLib::InitializeOpenGL();									//3Dシーンを初期化する関数を用意する
	GlLib::MakeGLObject();										//3Dオブジェクトを生成する
#endif

	return 0;
}

//BOOL CChildView2::SetDCPixelFormat( HDC hdc)

// OnCreate から最初に実行される　イニシャライズ処理
//BOOL CChildView2::InitializeOpenGL( void)

////////////////////////////////////////////////////////////////////////////

void CChildView2::OnDestroy()
{
	CWnd::OnDestroy();

	// レンダリング コンテキストとデバイス コンテキストの解放
	m_M3View.OnDestroy();

	if( m_pDC ) delete m_pDC;
}

BOOL CChildView2::OnEraseBkgnd( CDC* pDC)
{
//	return CWnd::OnEraseBkgnd(pDC);
	return TRUE;
}

//void MsOnSize( UINT nType, int cx, int cy);

void CChildView2::OnSize( UINT nType, int cx, int cy)
{
	CWnd::OnSize( nType, cx, cy);

																				//	MsOnSize( nType, cx, cy);//
#if( DEBUG1)
	GlLib::ViewSetting(cx, cy);
#endif

	m_M3View.OnSize( nType, cx, cy);


}

void CChildView2::OnLButtonDown( UINT nFlags, CPoint point)
{
	m_iPtLast = point;
	SetCapture();

	CWnd::OnLButtonDown( nFlags, point);
}

void CChildView2::OnLButtonUp( UINT nFlags, CPoint point)
{
	ReleaseCapture();

	CWnd::OnLButtonUp(nFlags, point);
}

void CChildView2::OnMouseMove( UINT nFlags, CPoint point)
{
	CPoint	iPtD;												// マウス移動量	
	CRect	iRcWin;												// ウィンドウ領域 (描画領域)
	CPoint	iSzWin;												// ウィンドウの幅と高さ
	MC::MgVect3	PtD( 0., 0., 0.);
	MC::MgVect3	RtD( 0., 0., 0.);
	MREAL	SclD;

	if (!(nFlags & MK_LBUTTON) && !( nFlags & MK_RBUTTON))
		MQUIT;													// マウスの左または右ボタンが押されていない場合は無視

	GetClientRect( iRcWin);
	
	iPtD.x = ( point.x - m_iPtLast.x) * 2;						// マウス移動量	
//S	iPtD.y = ( m_iPtLast.y - point.y) * 2;						// Yは上が負方向である(例: 0:上 ～　1024:下)
	iPtD.y = MMDIRY * (LONG)( m_iPtLast.y - point.y) * 2;				// Yは上が正方向である(例: 1024:上 ～　0:下)

	if ( ( MGABS( iPtD.x) < 5) && ( MGABS( iPtD.y) < 5)) 
		MQUIT;													// 判定移動量未満の場合は無視

	iSzWin = CPoint( iRcWin.right - iRcWin.left,
					 iRcWin.bottom - iRcWin.top);  

	//	Ctrl + Left Button:   左右移動、上下移動
	if ( nFlags & MK_CONTROL && nFlags & MK_LBUTTON) {
		PtD.x = (MREAL)iPtD.x / (MREAL)iSzWin.x;
		PtD.y = (MREAL)iPtD.y / (MREAL)iSzWin.y;

	//	Ctrl + Right Button:   ____、前後移動
	} else if ( nFlags & MK_CONTROL && nFlags & MK_RBUTTON) {
		PtD.z = (MREAL)iPtD.y / (MREAL)iSzWin.y;

	//	Left Button:   Ｙ軸回転、Ｘ軸回転
	} else 	if ( nFlags & MK_LBUTTON) {
		RtD.x = (MREAL)iPtD.x / (MREAL)iSzWin.x;
		RtD.y = - (MREAL)iPtD.y / (MREAL)iSzWin.y;

		RtD.z = - ((MREAL)iPtD.y * ((MREAL)point.x / (MREAL)iSzWin.x * 2 - 1) +		// 外側の方のＸ軸回転とＹ軸回転はＺ軸回転
				(MREAL)iPtD.x * ((MREAL)point.y / (MREAL)iSzWin.y * 2 -1)) /		// にも影響を及ぼす様にする
				(MREAL)iSzWin.x;;													// 下は (正方向:Ｚ軸に対して右回転)

	//	Right Button:   Ｚ回転、拡大縮小
	} else if ( nFlags & MK_RBUTTON) {
		RtD.z = (MREAL)iPtD.x / (MREAL)iSzWin.x;
		SclD  = (MREAL)iPtD.y / (MREAL)iSzWin.y;

		if (point.y < (iRcWin.top + iRcWin.bottom)/2)								// Ｚ軸回転角　画面中央より
			RtD.z = - RtD.z;														// 上は (正方向:Ｚ軸に対して左回転) で、
																					//			   (Ｚ軸から見て右回転)
																					// 下は (正方向:Ｚ軸に対して右回転)
																					//			   (Ｚ軸から見て左回転)
	}
	m_M3View.OnMouseMove( nFlags, PtD, RtD, SclD);

	m_iPtLast = point;

	PostMessage(WM_PAINT);										// 再表示のメッセージを送り処理の終了を待たずに次へ移る
exit:
	CWnd::OnMouseMove(nFlags, point);
}

void CChildView2::OnRButtonDown(UINT nFlags, CPoint point)
{
	m_iPtLast = point;
	SetCapture();

	CWnd::OnRButtonDown(nFlags, point);
}

void CChildView2::OnRButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();

	CWnd::OnRButtonUp(nFlags, point);
}

void CChildView2::OnMButtonDown(UINT nFlags, CPoint point)
{
	m_M3View.m_iFWire++;
	if (m_M3View.m_iFWire >= 5 || (nFlags != 24 && m_M3View.m_iFWire >= 3))
		m_M3View.m_iFWire = 0;
	PostMessage(WM_PAINT);										// 再表示のメッセージを送り処理の終了を待たずに次へ移る

	CWnd::OnMButtonDown(nFlags, point);
}

void CChildView2::OnMButtonUp(UINT nFlags, CPoint point)
{
	CWnd::OnMButtonUp(nFlags, point);
}
/*
void CChildView2::OnClose()
{
	MC::MmWndInfo*	pWndInfo = MC::WindowCtrl::MmWndKFind( this);					// 管理情報の開放
	MC::WindowCtrl::MmWndKDelete( pWndInfo);

	CWnd::OnClose();
}
*/
// ChildView3.cpp : 実装ファイル
//

#include "stdafx.h"
#include "MCAD.h"
#include "ChildView3.h"

#include "MgMat.h"
#include "MbCod.h"
#include "MmGridNum.h"
#include "MmDrag.h"
#include "MmWnd.h"

// CChildView3

IMPLEMENT_DYNAMIC(CChildView3, CWnd)
CChildView3::CChildView3()
{
}

CChildView3::~CChildView3()
{
}

BEGIN_MESSAGE_MAP(CChildView3, CWnd)
	ON_WM_PAINT()
//	ON_WM_CLOSE()
END_MESSAGE_MAP()

// CChildView3 メッセージ ハンドラ

BOOL CChildView3::PreCreateWindow(CREATESTRUCT& cs)
{
	MINT	ist;
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	MC::WindowCtrl::MmWndSetWndC( 3, this);										// ウィンドウ管理
	ist = MC::WindowCtrl::MmWndKAddC();											// ウィンドウ管理
	_ASSERTE(ist == 0);											// ウィンドウ管理

	return TRUE;
}

void CChildView3::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO : ここにメッセージ ハンドラ コードを追加します。

	MC::MmWndInfo* pWndInfo = MC::WindowCtrl::MmWndKFindWnd( this);
	(pWndInfo->GetFrame())->SetWindowText(Mstr( "WINDOW_03"));

	// 描画メッセージで CWnd::OnPaint() を呼び出さないでください。
}
/*
BOOL CChildView3::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO : ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CChildView3::OnClose()
{
	// TODO : ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	MmWndInfo*	pWndInfo = MmWndKFind( this);					// 管理情報の開放
	MmWndKDelete( pWndInfo);

	CWnd::OnClose();
}
*/
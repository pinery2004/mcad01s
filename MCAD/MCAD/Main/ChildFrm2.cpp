// ChildFrm2.cpp : CChildFrame2 クラスの実装
//
#include "stdafx.h"
#include "MCAD.h"

#include "MgMat.h"
#include "MbCod.h"
#include "MmGridNum.h"
#include "MmDrag.h"
#include "MmWnd.h"

#include "resource.h"

#include "ChildFrm2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame2

IMPLEMENT_DYNCREATE(CChildFrame2, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame2, CMDIChildWnd)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	ON_WM_SETFOCUS()
	ON_WM_CREATE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CChildFrame2 コンストラクション/デストラクション

CChildFrame2::CChildFrame2()
{
	// TODO: メンバ初期化コードをここに追加してください。
}

CChildFrame2::~CChildFrame2()
{
}


BOOL CChildFrame2::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs を変更して、Window クラスまたはスタイルを変更します。

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	// ディスプレイ画面サイズの取得
	CRect rect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);

	HDC hdc = ::GetDC( NULL);
	int Width = GetDeviceCaps(hdc, HORZRES);
	int Height = GetDeviceCaps(hdc, VERTRES);

	::ReleaseDC(NULL, hdc);

	// 表示ウィンドウサイズの設定
	if ( Width >= 0x500) {
		cs.x = 500; cs.cx = 700;					// 実行直後のウィンドウのサイズ 1280x??? 
	} else {
		cs.x = 200; cs.cx = 580; 					// 実行直後のウィンドウのサイズ 1024x???
	}
	if ( Height >= 0x400) {
		cs.y = 0;	cs.cy = 700;					// 実行直後のウィンドウのサイズ ????x1024
	} else {
		cs.y = 0;	cs.cy = 580;					// 実行直後のウィンドウのサイズ ????x768
	}

	return TRUE;
}

// CChildFrame2 診断

#ifdef _DEBUG
void CChildFrame2::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame2::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

// CChildFrame2 メッセージ ハンドラ
void CChildFrame2::OnFileClose() 
{
	// このフレームを閉じるには、WM_CLOSE を送ります。このメッセージは、
	// システム メニューの [閉じる] を選択した場合と同じです。
	SendMessage(WM_CLOSE);
}

int CChildFrame2::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// フレームのクライアント領域全体を占めるビューを作成します。
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, 
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("ビュー ウィンドウを作成できませんでした。\n");
		return -1;
	}

	return 0;
}

void CChildFrame2::OnSetFocus(CWnd* pOldWnd) 
{
	CMDIChildWnd::OnSetFocus(pOldWnd);

	m_wndView.SetFocus();
}

BOOL CChildFrame2::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// ビューに最初にコマンドを処理する機会を与えます。
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;
	
	// それ以外の場合は、既定の処理を行います。
	return CMDIChildWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

CMenu CChildFrame2::menu;        // menu

BOOL CChildFrame2::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle , const RECT& rect , CMDIFrameWnd* pParentWnd , CCreateContext* pContext)
{
	// TODO : ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if (menu.m_hMenu == NULL)
		menu.LoadMenu(IDR_MCADTYPE);
	m_hMenuShared = menu.m_hMenu;

	return CMDIChildWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, pContext);
}

void CChildFrame2::OnClose()
{
	// TODO : ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	MC::MmWndInfo* pWndInfo = MC::WindowCtrl::MmWndKFindFrm( this);
	MC::WindowCtrl::MmWndKDelete( pWndInfo);

	CMDIChildWnd::OnClose();
}

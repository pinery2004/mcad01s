// ChildFrm3.cpp : CChildFrame3 クラスの実装
//
#include "stdafx.h"
#include "MCAD.h"

#include "MgMat.h"
#include "MbCod.h"
#include "MmGridNum.h"
#include "MmDrag.h"
#include "MmWnd.h"

#include "resource.h"

#include "ChildFrm3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame3

IMPLEMENT_DYNCREATE(CChildFrame3, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame3, CMDIChildWnd)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	ON_WM_SETFOCUS()
	ON_WM_CREATE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CChildFrame3 コンストラクション/デストラクション

CChildFrame3::CChildFrame3()
{
	// TODO: メンバ初期化コードをここに追加してください。
}

CChildFrame3::~CChildFrame3()
{
}

BOOL CChildFrame3::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs を変更して、Window クラスまたはスタイルを変更します。
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CChildFrame3 診断

#ifdef _DEBUG
void CChildFrame3::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame3::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

// CChildFrame3 メッセージ ハンドラ
void CChildFrame3::OnFileClose() 
{
	// このフレームを閉じるには、WM_CLOSE を送ります。このメッセージは、
	// システム メニューの [閉じる] を選択した場合と同じです。
	SendMessage(WM_CLOSE);
}

int CChildFrame3::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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

void CChildFrame3::OnSetFocus(CWnd* pOldWnd) 
{
	CMDIChildWnd::OnSetFocus(pOldWnd);

	m_wndView.SetFocus();
}

BOOL CChildFrame3::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// ビューに最初にコマンドを処理する機会を与えます。
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;
	
	// それ以外の場合は、既定の処理を行います。
	return CMDIChildWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

CMenu CChildFrame3::menu;        // menu

BOOL CChildFrame3::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle , const RECT& rect , CMDIFrameWnd* pParentWnd , CCreateContext* pContext)
{
	// TODO : ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if (menu.m_hMenu == NULL)
		menu.LoadMenu(IDR_MCADTYPE);
	m_hMenuShared = menu.m_hMenu;

	return CMDIChildWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, pContext);
}

void CChildFrame3::OnClose()
{
	// TODO : ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	MC::MmWndInfo* pWndInfo = MC::WindowCtrl::MmWndKFindFrm( this);
	MC::WindowCtrl::MmWndKDelete( pWndInfo);

	CMDIChildWnd::OnClose();
}

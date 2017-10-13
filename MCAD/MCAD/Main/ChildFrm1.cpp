// ChildFrm1.cpp : CChildFrame1 クラスの実装
//
#include "stdafx.h"
#include "MCAD.h"
#include "MainFrm.h"

#include "MgMat.h"
#include "MbCod.h"
#include "MmGridNum.h"
#include "MmDrag.h"
#include "MmWnd.h"

#include "resource.h"

#include "ChildFrm1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame1

IMPLEMENT_DYNCREATE(CChildFrame1, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame1, CMDIChildWnd)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	ON_WM_SETFOCUS()
	ON_WM_CREATE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// CChildFrame1 コンストラクション/デストラクション

CChildFrame1::CChildFrame1()
{
	// TODO: メンバ初期化コードをここに追加してください。
	TRACE( "CChildFrame1::CChildFrame1(%x)\n", this);
}

CChildFrame1::~CChildFrame1()
{
	TRACE( "CChildFrame1::~CChildFrame1(%x)\n", this);
}

BOOL CChildFrame1::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs を変更して、Window クラスまたはスタイルを変更します。
	TRACE( "CChildFrame1::PreCreateWindow(%x)\n", this);

	cs.x  = 0;									//- GetSystemMetrics(SM_CXSIZEFRAME);
	cs.y  = 0;									//- GetSystemMetrics(SM_CXSIZEFRAME);
	cs.cx = min( GetSystemMetrics(SM_CXMAXIMIZED), 1280) - 
				 GetSystemMetrics(SM_CYMENU)/* 20 */ -
				 GetSystemMetrics(SM_CXSIZEFRAME)/* 4 */ * 4;
	cs.cy = min( GetSystemMetrics(SM_CYMAXIMIZED), 1060) -
				 (GetSystemMetrics(SM_CYMENU)/* 20 */ + GetSystemMetrics(SM_CXEDGE)/* 2 */) * 5 -
				 GetSystemMetrics(SM_CYCAPTION)/* 26 */ * 1 -
				 GetSystemMetrics(SM_CYSIZEFRAME)/* 4 */ * 4 + GetSystemMetrics(SM_CXEDGE)/* 2 */ * 3;


	if( !CMDIChildWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}


// CChildFrame1 診断

#ifdef _DEBUG
void CChildFrame1::AssertValid() const
{
	TRACE( "CChildFrame1::AssertValid(%x)\n", this);
	CMDIChildWnd::AssertValid();
}

void CChildFrame1::Dump(CDumpContext& dc) const
{
	TRACE( "CChildFrame1::Dump(%x)\n", this);
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG


// CChildFrame1 メッセージ ハンドラ
void CChildFrame1::OnFileClose() 
{
	TRACE( "CChildFrame1::OnFileClose(%x)\n", this);
	// このフレームを閉じるには、WM_CLOSE を送ります。このメッセージは、
	// システム メニューの [閉じる] を選択した場合と同じです。
	SendMessage(WM_CLOSE);
}

int CChildFrame1::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	TRACE( "CChildFrame1::OnCreate(%x)\n", this);
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

void CChildFrame1::OnSetFocus(CWnd* pOldWnd) 
{
	TRACE( "CChildFrame1::OnSetFocus(%x)\n", this);
	CMDIChildWnd::OnSetFocus(pOldWnd);

	ms_pMainFrame->m_pCurFrame = this;

//	CMainFrame*	pMainFrame = (CMainFrame*)AfxGetMainWnd();

	m_wndView.SetFocus();
}

BOOL CChildFrame1::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	TRACE( "CChildFrame1::OnCmdMsg(%x)\n", this);
	// ビューに最初にコマンドを処理する機会を与えます。
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;
	
	// それ以外の場合は、既定の処理を行います。
	return CMDIChildWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

CMenu CChildFrame1::menu;        // menu

BOOL CChildFrame1::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect,
						  CMDIFrameWnd* pParentWnd, CCreateContext* pContext)
{
	TRACE( "CChildFrame1::Create(%x)\n", this);
	// TODO : ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if (menu.m_hMenu == NULL)
		menu.LoadMenu(IDR_MCADTYPE);
	m_hMenuShared = menu.m_hMenu;

	return CMDIChildWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, pContext);
}

void CChildFrame1::OnClose()
{
	TRACE( "CChildFrame1::OnClose(%x)\n", this);
	// TODO : ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	MC::MmWndInfo* pWndInfo = MC::WindowCtrl::MmWndKFindFrm( this);
	MC::WindowCtrl::MmWndKDelete( pWndInfo);

	CMDIChildWnd::OnClose();
}

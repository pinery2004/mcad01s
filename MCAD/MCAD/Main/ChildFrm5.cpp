// ChildFrm5.cpp : CChildFrame5 クラスの実装
//
#include "stdafx.h"
#include "MCAD.h"

#include "MgMat.h"
#include "MbCod.h"
#include "MmGridNum.h"
#include "MmDrag.h"
#include "MmWnd.h"

#include "resource.h"

#include "ChildFrm5.h"
#include ".\childfrm5.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame5

IMPLEMENT_DYNCREATE(CChildFrame5, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame5, CMDIChildWnd)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	ON_WM_SETFOCUS()
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, OnUpdateFilePrintPreview)
END_MESSAGE_MAP()

// CChildFrame5 コンストラクション/デストラクション

CChildFrame5::CChildFrame5()
{
	// TODO: メンバ初期化コードをここに追加してください。
	m_fPrintPreview = true;
}

CChildFrame5::~CChildFrame5()
{
}

BOOL CChildFrame5::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs を変更して、Window クラスまたはスタイルを変更します。
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CChildFrame5 診断

#ifdef _DEBUG
void CChildFrame5::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame5::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

CMenu CChildFrame5::menu;        // menu

// CChildFrame5 メッセージ ハンドラ
void CChildFrame5::OnFileClose() 
{
	// このフレームを閉じるには、WM_CLOSE を送ります。このメッセージは、
	// システム メニューの [閉じる] を選択した場合と同じです。
	SendMessage(WM_CLOSE);
}

int CChildFrame5::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// フレームのクライアント領域全体を占めるビューを作成します。
/*
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, 
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("ビュー ウィンドウを作成できませんでした。\n");
		return -1;
	}
*/
	CCreateContext context;										// PrintPreview
	context.m_pNewViewClass = RUNTIME_CLASS(CChildView5);		// PrintPreview
	context.m_pCurrentFrame = this;								// PrintPreview
	context.m_pCurrentDoc = NULL;									// PrintPreview
	context.m_pLastView = NULL;									// PrintPreview
																// PrintPreview
	m_pView = STATIC_DOWNCAST(CChildView5, CreateView(&context)); //CreateView(&context);	 PrintPreview
	if( m_pView != NULL)											// PrintPreview
	{															// PrintPreview
		m_pView->ShowWindow(SW_SHOW);							// PrintPreview
		SetActiveView(m_pView);									// PrintPreview
	}															// PrintPreview

	return 0;
}

void CChildFrame5::OnSetFocus(CWnd* pOldWnd) 
{
	CMDIChildWnd::OnSetFocus(pOldWnd);

//	m_wndView.SetFocus();	
	m_pView->SetFocus();										// PrintPreview
}

BOOL CChildFrame5::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// ビューに最初にコマンドを処理する機会を与えます。
	if (m_pView->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))	// PrintPreview
		return TRUE;
	if ( nCode == 0 && pExtra == 0 &&
		(nID == AFX_ID_PREVIEW_CLOSE || nID == AFX_ID_PREVIEW_PRINT))	// PrintPreview
			m_fPrintPreview = true;									// PrintPreview
	
	// それ以外の場合は、既定の処理を行います。
	return CMDIChildWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
///*
BOOL CChildFrame5::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle , const RECT& rect , CMDIFrameWnd* pParentWnd , CCreateContext* pContext)
{
	// TODO : ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if (menu.m_hMenu == NULL)
		menu.LoadMenu(IDR_MCADTYPE);
	m_hMenuShared = menu.m_hMenu;

	return CMDIChildWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, pContext);
}
//*/
void CChildFrame5::OnClose()
{
	// TODO : ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
//	SendMessage(AFX_ID_PREVIEW_CLOSE, 0, 0);					// 先にPrintPreview画面をcloseしたいが出来なかった

	MC::MmWndInfo* pWndInfo = MC::WindowCtrl::MmWndKFindFrm( this);
	MC::WindowCtrl::MmWndKDelete( pWndInfo);

	CMDIChildWnd::OnClose();
}

void CChildFrame5::OnFilePrint()								// PrintPreview
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	m_pView->OnFilePrint();										// PrintPreview
}

void CChildFrame5::OnFilePrintPreview()							// PrintPreview
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	m_pView->OnFilePrintPreview(this);							// PrintPreview
	m_fPrintPreview = false;									// PrintPreview
}

void CChildFrame5::OnUpdateFilePrintPreview(CCmdUI *pCmdUI)		// PrintPreview
{
	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
	pCmdUI->Enable( m_fPrintPreview);							// PrintPreview
}

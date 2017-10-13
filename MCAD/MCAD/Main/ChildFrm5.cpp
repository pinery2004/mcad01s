// ChildFrm5.cpp : CChildFrame5 �N���X�̎���
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

// CChildFrame5 �R���X�g���N�V����/�f�X�g���N�V����

CChildFrame5::CChildFrame5()
{
	// TODO: �����o�������R�[�h�������ɒǉ����Ă��������B
	m_fPrintPreview = true;
}

CChildFrame5::~CChildFrame5()
{
}

BOOL CChildFrame5::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs ��ύX���āAWindow �N���X�܂��̓X�^�C����ύX���܂��B
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CChildFrame5 �f�f

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

// CChildFrame5 ���b�Z�[�W �n���h��
void CChildFrame5::OnFileClose() 
{
	// ���̃t���[�������ɂ́AWM_CLOSE �𑗂�܂��B���̃��b�Z�[�W�́A
	// �V�X�e�� ���j���[�� [����] ��I�������ꍇ�Ɠ����ł��B
	SendMessage(WM_CLOSE);
}

int CChildFrame5::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// �t���[���̃N���C�A���g�̈�S�̂��߂�r���[���쐬���܂��B
/*
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, 
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("�r���[ �E�B���h�E���쐬�ł��܂���ł����B\n");
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
	// �r���[�ɍŏ��ɃR�}���h����������@���^���܂��B
	if (m_pView->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))	// PrintPreview
		return TRUE;
	if ( nCode == 0 && pExtra == 0 &&
		(nID == AFX_ID_PREVIEW_CLOSE || nID == AFX_ID_PREVIEW_PRINT))	// PrintPreview
			m_fPrintPreview = true;									// PrintPreview
	
	// ����ȊO�̏ꍇ�́A����̏������s���܂��B
	return CMDIChildWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
///*
BOOL CChildFrame5::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle , const RECT& rect , CMDIFrameWnd* pParentWnd , CCreateContext* pContext)
{
	// TODO : �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B
	if (menu.m_hMenu == NULL)
		menu.LoadMenu(IDR_MCADTYPE);
	m_hMenuShared = menu.m_hMenu;

	return CMDIChildWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, pContext);
}
//*/
void CChildFrame5::OnClose()
{
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
//	SendMessage(AFX_ID_PREVIEW_CLOSE, 0, 0);					// ���PrintPreview��ʂ�close���������o���Ȃ�����

	MC::MmWndInfo* pWndInfo = MC::WindowCtrl::MmWndKFindFrm( this);
	MC::WindowCtrl::MmWndKDelete( pWndInfo);

	CMDIChildWnd::OnClose();
}

void CChildFrame5::OnFilePrint()								// PrintPreview
{
	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
	m_pView->OnFilePrint();										// PrintPreview
}

void CChildFrame5::OnFilePrintPreview()							// PrintPreview
{
	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
	m_pView->OnFilePrintPreview(this);							// PrintPreview
	m_fPrintPreview = false;									// PrintPreview
}

void CChildFrame5::OnUpdateFilePrintPreview(CCmdUI *pCmdUI)		// PrintPreview
{
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
	pCmdUI->Enable( m_fPrintPreview);							// PrintPreview
}

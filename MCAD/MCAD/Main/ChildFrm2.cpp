// ChildFrm2.cpp : CChildFrame2 �N���X�̎���
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


// CChildFrame2 �R���X�g���N�V����/�f�X�g���N�V����

CChildFrame2::CChildFrame2()
{
	// TODO: �����o�������R�[�h�������ɒǉ����Ă��������B
}

CChildFrame2::~CChildFrame2()
{
}


BOOL CChildFrame2::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs ��ύX���āAWindow �N���X�܂��̓X�^�C����ύX���܂��B

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	// �f�B�X�v���C��ʃT�C�Y�̎擾
	CRect rect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);

	HDC hdc = ::GetDC( NULL);
	int Width = GetDeviceCaps(hdc, HORZRES);
	int Height = GetDeviceCaps(hdc, VERTRES);

	::ReleaseDC(NULL, hdc);

	// �\���E�B���h�E�T�C�Y�̐ݒ�
	if ( Width >= 0x500) {
		cs.x = 500; cs.cx = 700;					// ���s����̃E�B���h�E�̃T�C�Y 1280x??? 
	} else {
		cs.x = 200; cs.cx = 580; 					// ���s����̃E�B���h�E�̃T�C�Y 1024x???
	}
	if ( Height >= 0x400) {
		cs.y = 0;	cs.cy = 700;					// ���s����̃E�B���h�E�̃T�C�Y ????x1024
	} else {
		cs.y = 0;	cs.cy = 580;					// ���s����̃E�B���h�E�̃T�C�Y ????x768
	}

	return TRUE;
}

// CChildFrame2 �f�f

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

// CChildFrame2 ���b�Z�[�W �n���h��
void CChildFrame2::OnFileClose() 
{
	// ���̃t���[�������ɂ́AWM_CLOSE �𑗂�܂��B���̃��b�Z�[�W�́A
	// �V�X�e�� ���j���[�� [����] ��I�������ꍇ�Ɠ����ł��B
	SendMessage(WM_CLOSE);
}

int CChildFrame2::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// �t���[���̃N���C�A���g�̈�S�̂��߂�r���[���쐬���܂��B
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, 
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("�r���[ �E�B���h�E���쐬�ł��܂���ł����B\n");
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
	// �r���[�ɍŏ��ɃR�}���h����������@���^���܂��B
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;
	
	// ����ȊO�̏ꍇ�́A����̏������s���܂��B
	return CMDIChildWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

CMenu CChildFrame2::menu;        // menu

BOOL CChildFrame2::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle , const RECT& rect , CMDIFrameWnd* pParentWnd , CCreateContext* pContext)
{
	// TODO : �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B
	if (menu.m_hMenu == NULL)
		menu.LoadMenu(IDR_MCADTYPE);
	m_hMenuShared = menu.m_hMenu;

	return CMDIChildWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, pContext);
}

void CChildFrame2::OnClose()
{
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	MC::MmWndInfo* pWndInfo = MC::WindowCtrl::MmWndKFindFrm( this);
	MC::WindowCtrl::MmWndKDelete( pWndInfo);

	CMDIChildWnd::OnClose();
}

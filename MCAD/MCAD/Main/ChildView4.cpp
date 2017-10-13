// ChildView4.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "MCAD.h"
#include "ChildView4.h"

#include "MgMat.h"
#include "MbCod.h"
#include "MmGridNum.h"
#include "MmDrag.h"
#include "MmWnd.h"

// CChildView4

IMPLEMENT_DYNAMIC(CChildView4, CWnd)
CChildView4::CChildView4()
{
}

CChildView4::~CChildView4()
{
}

BEGIN_MESSAGE_MAP(CChildView4, CWnd)
	ON_WM_PAINT()
//	ON_WM_CLOSE()
END_MESSAGE_MAP()

// CChildView4 ���b�Z�[�W �n���h��

BOOL CChildView4::PreCreateWindow(CREATESTRUCT& cs)
{
	MINT	ist;
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	MC::WindowCtrl::MmWndSetWndC( 4, this);										// �E�B���h�E�Ǘ�
	ist = MC::WindowCtrl::MmWndKAddC();											// �E�B���h�E�Ǘ�
	_ASSERTE(ist == 0);											// �E�B���h�E�Ǘ�

	return TRUE;
}

void CChildView4::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����܂��B

	MC::MmWndInfo* pWndInfo = MC::WindowCtrl::MmWndKFindWnd( this);
	(pWndInfo->GetFrame())->SetWindowText(Mstr( "WINDOW_04"));

	// �`�惁�b�Z�[�W�� CWnd::OnPaint() ���Ăяo���Ȃ��ł��������B
}
/*
void CChildView4::OnClose()
{
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	MmWndInfo*	pWndInfo = MmWndKFind( this);					// �Ǘ����̊J��
	MmWndKDelete( pWndInfo);

	CWnd::OnClose();
}
*/
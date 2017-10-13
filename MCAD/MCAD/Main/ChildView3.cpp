// ChildView3.cpp : �����t�@�C��
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

// CChildView3 ���b�Z�[�W �n���h��

BOOL CChildView3::PreCreateWindow(CREATESTRUCT& cs)
{
	MINT	ist;
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	MC::WindowCtrl::MmWndSetWndC( 3, this);										// �E�B���h�E�Ǘ�
	ist = MC::WindowCtrl::MmWndKAddC();											// �E�B���h�E�Ǘ�
	_ASSERTE(ist == 0);											// �E�B���h�E�Ǘ�

	return TRUE;
}

void CChildView3::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����܂��B

	MC::MmWndInfo* pWndInfo = MC::WindowCtrl::MmWndKFindWnd( this);
	(pWndInfo->GetFrame())->SetWindowText(Mstr( "WINDOW_03"));

	// �`�惁�b�Z�[�W�� CWnd::OnPaint() ���Ăяo���Ȃ��ł��������B
}
/*
BOOL CChildView3::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO : �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B

	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CChildView3::OnClose()
{
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	MmWndInfo*	pWndInfo = MmWndKFind( this);					// �Ǘ����̊J��
	MmWndKDelete( pWndInfo);

	CWnd::OnClose();
}
*/
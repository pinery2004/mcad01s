// ChildView5.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "MCAD.h"
#include "ChildView5.h"

#include "MgMat.h"
#include "MbCod.h"
#include "MmGridNum.h"
#include "MmDrag.h"
#include "MmWnd.h"

#include "MmDefine.h"
#include "MmLib.h"

// CChildView5

IMPLEMENT_DYNCREATE(CChildView5, CWrapperView)					// PrintPreview Modify

CChildView5::CChildView5()
{
}

CChildView5::~CChildView5()
{
}

BEGIN_MESSAGE_MAP(CChildView5, CWnd)
	ON_WM_PAINT()
//	ON_WM_CLOSE()
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CChildView5 ���b�Z�[�W �n���h��

BOOL CChildView5::PreCreateWindow(CREATESTRUCT& cs)
{
	MINT	ist;
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS,
		NULL,reinterpret_cast<HBRUSH>(COLOR_WINDOW+1),NULL); 

	MC::WindowCtrl::MmWndSetWndC( 5, this);										// �E�B���h�E�Ǘ�
	ist = MC::WindowCtrl::MmWndKAddC();											// �E�B���h�E�Ǘ�
	_ASSERTE(ist == 0);											// �E�B���h�E�Ǘ�

	return TRUE;
}

void CChildView5::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����܂��B
	MC::MmWndInfo* pWndInfo = MC::WindowCtrl::MmWndKFindWnd( this);

	if (pWndInfo == NULL)										// PrintPreview��ʂ𒼐�close�������AView Close���
																// Preview Close���A�����ɓ����Ă��邽�ߔ����ǉ�
		return;													// Debug�p
	else														// Debug�p
		(pWndInfo->GetFrame())->SetWindowText(Mstr( "WINDOW_05"));

//C	OnDraw( &dc);												// PrintPreview

	// �`�惁�b�Z�[�W�� CWnd::OnPaint() ���Ăяo���Ȃ��ł��������B

//C	-------------------------------------------------------------------------------------------------

//	MmWndKInitMouse();

	//	�}�b�v���[�h��ݒ�
	MINT iOldMM = pWndInfo->SetMapMode( NULL);

	// �������[�c�b�̓��e����ʂɕ\��
	MC::WindowCtrl::MmWndKDraw( pWndInfo);

	// �N���X�w�A�[�J�[�\���\��
//C	Window::ReDrawCrossHair();

	//	���o�[�o���h�}�`���ĕ\������
//C	Window::ReDrawDragging();
	
	//	�}�b�v���[�h��߂�
	pWndInfo->m_pMemDC->SetMapMode( iOldMM);

	// ���b�Z�[�W�̕`��̂��߂� CWnd::OnPaint() ���Ăяo���Ȃ��ł��������B
}
/*
void CChildView5::OnClose()
{
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	MmWndInfo*	pWndInfo = MmWndKFind( this);					// �Ǘ����̊J��
	MmWndKDelete( pWndInfo);

	CWnd::OnClose();
}
*/
void CChildView5::OnDraw(CDC* pDC)								// PrintPreview
{																// PrintPreview
	// PreviusPrint �\���p�@�c�b�C�E�C���h�E�G���A�ݒ�
	CRect client1;												// PrintPreview
	MC::MbCod cod1;

	cod1.SetDC( pDC);
	GetClientRect(&client1);									// PrintPreview

	MINT iDleft = client1.left;
	MINT iDtop  = client1.top;
	MINT iDwidth = client1.right - client1.left;
	MINT iDhight = client1.bottom - client1.top;

	cod1.SetWinD( iDleft, iDtop, iDwidth, iDhight);

	MC::MmWndInfo* pWndInfo = MC::WindowCtrl::MmWndKFindWnd( this);

	MC::WindowCtrl::MmDrawStructure( pWndInfo, &cod1);
}

void CChildView5::OnFilePrint()									// PrintPreview
{																// PrintPreview
	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B		// PrintPreview
	CView::OnFilePrint();										// PrintPreview
}																// PrintPreview

int CChildView5::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWrapperView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO :  �����ɓ���ȍ쐬�R�[�h��ǉ����Ă��������B

	return 0;
}

void CChildView5::OnSize(UINT nType, int cx, int cy)
{
	CWrapperView::OnSize(nType, cx, cy);

	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����܂��B
	MINT	iOldMM;

	if (cx > 0 && cy > 0) {
		CPoint	ptMouthD( cx, cy);								// ��ʃT�C�Y(�\�����W)
		CPoint	ptMouthL = ptMouthD;

		CClientDC dc( this);

		iOldMM = dc.SetMapMode( MMAPMODE);
		dc.DPtoLP( &ptMouthL);									// ��ʃT�C�Y(�_�����W)
		dc.SetMapMode( iOldMM);

																// �O���b�h�\������荞��
		MC::MmWndInfo* pWndInfo1 = MC::WindowCtrl::MmWndKFindSyu( 1, MTHEIMENZU, 1, 1);
		MC::MmWndInfo* pWndInfo5 = MC::WindowCtrl::MmWndKFindWnd( this);
		if (pWndInfo5 == NULL)
			return;
		MINT nGrid[4];
		pWndInfo1->GetGridNum( nGrid);
		pWndInfo5->SetGridNum( nGrid);
		pWndInfo5->SetMinMaxRS( pWndInfo5->GetMinMaxGA( pWndInfo5));	//	�����W(Min/Max)
//		pWndInfo5->SetMat();							// ���W�ϊ��p�}�g���b�N�X�̐ݒ�				//ZZ? �Ȃ������� 070901
		
		MC::ViewInp::Size( this, ptMouthD, ptMouthL);					// �E�B���h�E�T�C�Y�ݒ�
	}
}


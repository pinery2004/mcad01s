// ChildView1.cpp : CChildView1 �N���X�̎���
//
#include "stdafx.h"
#include "MCAD.h"
#include "MainFrm.h"
#include "ChildView1.h"

#include "MmWnd.h"

#include "MmDefine.h"
#include "MmLib.h"

#include "MmCmd.h"
#include "resource.h"

#include "MsBitSet.h"

#include "MmCmdMsg.h"
#include "McCmd.h"

#include "MmInp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static CPoint			z_ptCurCursorD;							// ���݃J�[�\���ړ��ʒu(�\�����W)

CChildView1::CChildView1()
{
}

CChildView1::~CChildView1()
{
}

BEGIN_MESSAGE_MAP(CChildView1, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()												// 1
	ON_WM_RBUTTONDOWN()												// 2
	ON_WM_MBUTTONDOWN()												// 3
	ON_WM_LBUTTONUP()												// 4
	ON_WM_RBUTTONUP()												// 5
	ON_WM_MBUTTONUP()												// 6
	ON_WM_LBUTTONDBLCLK()											// 7
	ON_WM_RBUTTONDBLCLK()											// 8
	ON_WM_MBUTTONDBLCLK()											// 9
	ON_WM_MOUSEMOVE()												// 10
	ON_WM_SIZE()													// 11
	ON_WM_HSCROLL()													// 1
	ON_WM_VSCROLL()													// 1
	ON_WM_MOUSEWHEEL()												// 21
    ON_MESSAGE(WM_MYMESSAGEPARTSEDIT, OnMyMessagePtsEdit)			// 1
    ON_MESSAGE(WM_MYMESSAGEGRIDNUM, OnMyMessageGridNum)				// 1
    ON_MESSAGE(WM_MYMESSAGESTATUSBAR, OnMyMessageStatusBar)			// 1
    ON_MESSAGE(WM_MYMESSAGEOPTION, OnMyMessageOpt)					// 1
    ON_MESSAGE(WM_MYMESSAGEHIST, OnMyMessageHist)					// 1
	ON_MESSAGE(WM_MYMESSAGETENKAI, OnMyMessageTenkai)				// 1
	ON_WM_SETCURSOR()												// 1
	ON_WM_KILLFOCUS()												// 1
	ON_WM_CANCELMODE()												// 1
	ON_WM_SETFOCUS()												// 1
	ON_WM_CREATE()													// 1
	ON_WM_CLOSE()													// 1
	ON_WM_CHAR()													// 1
	ON_WM_KEYDOWN()													// 1
	ON_WM_KEYUP()													// 1
	ON_WM_MOUSEACTIVATE()											// 1
	ON_WM_SHOWWINDOW()												// 1
//1c	ON_COMMAND(IDC_CANCEL1PT, OnCancel1Pt)
//1c	ON_UPDATE_COMMAND_UI(IDC_CANCEL1PT, OnUpdateCancel1Pt)
//1c	ON_COMMAND(IDC_CANCELALL, OnCancelAll)
//1c	ON_UPDATE_COMMAND_UI(IDC_CANCELALL, OnUpdateCancelAll)
//1c	ON_COMMAND(IDC_InpKai, OnInpKai)
//1c	ON_UPDATE_COMMAND_UI(IDC_InpKai, OnUpdateInpKai)
//1c	ON_COMMAND(IDC_InpALL, OnInpAll)
//1c	ON_UPDATE_COMMAND_UI(IDC_InpALL, OnUpdateInpAll)
//1c	ON_COMMAND(IDC_InpEND, OnInpEnd)
//1c	ON_UPDATE_COMMAND_UI(IDC_InpEND, OnUpdateInpEnd)
//1c	ON_COMMAND(IDC_MODECREATE, OnModeCreate)
//1c	ON_UPDATE_COMMAND_UI(IDC_MODECREATE, OnUpdateModeCreate)
//1c	ON_COMMAND(IDC_MODEDELETE, OnModeDelete)
//1c	ON_UPDATE_COMMAND_UI(IDC_MODEDELETE, OnUpdateModeDelete)
//1c	ON_COMMAND(IDC_MODEMODIFY, OnModeModify)
//1c	ON_UPDATE_COMMAND_UI(IDC_MODEMODIFY, OnUpdateModeModify)
//1c	ON_COMMAND(IDC_MODEMOVE, OnModeMove)
//1c	ON_UPDATE_COMMAND_UI(IDC_MODEMOVE, OnUpdateModeMove)
//1c	ON_COMMAND(IDC_MODECOPY, OnModeCopy)
//1c	ON_UPDATE_COMMAND_UI(IDC_MODECOPY, OnUpdateModeCopy)
//1c	ON_COMMAND(IDC_MODEPROPERTY, OnModeProperty)
//1c	ON_UPDATE_COMMAND_UI(IDC_MODEPROPERTY, OnUpdateModeProperty)
//1c	ON_COMMAND(IDC_ZOOMINIT, OnZoomInit)
//1c	ON_UPDATE_COMMAND_UI(IDC_ZOOMINIT, OnUpdateZoomInit)
//1c	ON_COMMAND(IDC_ZOOMPAN, OnZoomPan)
//1c	ON_UPDATE_COMMAND_UI(IDC_ZOOMPAN, OnUpdateZoomPan)
//1c	ON_COMMAND(IDC_ZOOMSIZE, OnZoomSz)
//1c	ON_UPDATE_COMMAND_UI(IDC_ZOOMSIZE, OnUpdateZoomSz)
//1c	ON_COMMAND(IDC_UNDO, OnUndo)
//1c	ON_UPDATE_COMMAND_UI(IDC_UNDO, OnUpdateUndo)
//1c	ON_COMMAND(IDC_REDO, OnRedo)
//1c	ON_UPDATE_COMMAND_UI(IDC_REDO, OnUpdateRedo)
//1c	ON_COMMAND(IDC_PARTSEDIT, OnPtsEdit)
//1c	ON_UPDATE_COMMAND_UI(IDC_PARTSEDIT, OnUpdatePtsEdit)
//
//1c	ON_COMMAND(IDC_GRIDNUM, OnGridNum)
//1c	ON_COMMAND(IDC_OPTION, OnOpt)
//1c	ON_COMMAND(IDC_HIST, OnHist)
//1c	ON_COMMAND(IDC_TENKAI, OnTenkai)
//1c	ON_UPDATE_COMMAND_UI(IDC_GRIDNUM, OnUpdateGridNum)
//1c	ON_COMMAND(IDC_CHUKIMOVE, OnChukimove)
//1c	ON_UPDATE_COMMAND_UI(IDC_CHUKIMOVE, OnUpdateChukimove)
//1c	ON_COMMAND(IDC_CHUKIDEL, OnChukidel)
//1c	ON_UPDATE_COMMAND_UI(IDC_CHUKIDEL, OnUpdateChukidel)
//1c	ON_COMMAND(IDC_CHUKIDLGON, OnChukidlgon)
//1c	ON_UPDATE_COMMAND_UI(IDC_CHUKIDLGON, OnUpdateChukidlgon)
//1c	ON_COMMAND(IDC_PANELBEFORE, OnPanelBefore)
//1c	ON_UPDATE_COMMAND_UI(IDC_PANELBEFORE, OnUpdatePanelBefore)
//1c	ON_COMMAND(IDC_PANELNEXT, OnPanelNext)
//1c	ON_UPDATE_COMMAND_UI(IDC_PANELNEXT, OnUpdatePanelNext)
//1c	ON_COMMAND(IDC_PANELALL, OnPanelAll)
//1c	ON_UPDATE_COMMAND_UI(IDC_PANELALL, OnUpdatePanelAll)
//	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
//	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, OnUpdateFilePrintPreview)
END_MESSAGE_MAP()

// CChildView1 ���b�Z�[�W �n���h��

BOOL CChildView1::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER | WS_MAXIMIZE;
//	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
//		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS,
		NULL,reinterpret_cast<HBRUSH>(COLOR_WINDOW+1),NULL); 

	MC::WindowCtrl::MmWndSetWndC( 1, this);
	MINT ist = MC::WindowCtrl::MmWndKAddC();
	_ASSERTE(ist == 0);

	return TRUE;
}

void CChildView1::OnPaint() 
{
	CPaintDC dc(this); // �`��̃f�o�C�X �R���e�L�X�g ���̍s��������OnPaint�������ɌJ��Ԃ��ƂȂ�

	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����܂��B
	MC::MmWndInfo* pWndInfo = MC::WindowCtrl::MmWndKFindWnd( this);

	pWndInfo->GetFrame()->SetWindowText( Mstr( "WINDOW_01"));			// �E�B���h�E�^�C�g���\��

	// �N���X�w�A�[�J�[�\���̏�����
	MC::Window::InitCrossHair();
	// �h���b�M���O�̏�����
	MC::Window::InitDragging();
	// ���ރq�b�g�\���̏�����
	MC::Window::InitHitBuzai();

	//	�}�b�v���[�h��ݒ�
	MINT iOldMM = pWndInfo->SetMapMode( NULL);

	// �������[�c�b�̓��e����ʂɕ\��
	MC::WindowCtrl::MmWndKDraw( pWndInfo);

	// �N���X�w�A�[�J�[�\���\��
	MC::Window::ReDrawCrossHair();

	//	���o�[�o���h�}�`���ĕ\������
	MC::Window::ReDrawDragging();
	
	//	�q�b�g���ނ��ĕ\������
	MC::Window::ReDrawHitBzi();
	
	//	�}�b�v���[�h��߂�
	pWndInfo->m_pMemDC->SetMapMode( iOldMM);

	// ���b�Z�[�W�̕`��̂��߂� CWnd::OnPaint() ���Ăяo���Ȃ��ł��������B
}

void CChildView1::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	MC::ViewInp::LButtonDown( this, nFlags, point);

	CWnd::OnLButtonDown(nFlags, point);
}

void CChildView1::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	MC::ViewInp::LButtonUp( this, nFlags, point);

	CWnd::OnLButtonUp(nFlags, point);
}

void CChildView1::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	MC::ViewInp::RButtonDown( this, nFlags, point);

	CWnd::OnRButtonDown(nFlags, point);
}

void CChildView1::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	MC::ViewInp::RButtonUp( this, nFlags, point);

	CWnd::OnRButtonUp(nFlags, point);
}

void CChildView1::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	z_ptCurCursorD = point;
	
	MC::ViewInp::MouseMove( this, nFlags,  point);

	CWnd::OnMouseMove(nFlags, point);
}

void CChildView1::OnMButtonDown(UINT nFlags, CPoint point)
{
//			g_MbDbg.Trace( " MButtonDown ", point.x, point.y);

	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	MC::ViewInp::MButtonDown( this, nFlags, point);

	CWnd::OnMButtonDown(nFlags, point);
}

void CChildView1::OnMButtonUp(UINT nFlags, CPoint point)
{
//			g_MbDbg.Trace( " MButtonUp ", point.x, point.y);

	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	MC::ViewInp::MButtonUp( this, nFlags, point);

	CWnd::OnMButtonUp(nFlags, point);
}

BOOL CChildView1::OnMouseWheel(UINT nFlags, short zDelta, CPoint point)
{
//			g_MbDbg.Trace( " MouseWheel ", point.x, point.y);

	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	// point�� �E������ւ̂��ꂪ(+31,+26)����s���m�Ȃ̂ŕs�g�p

	MC::ViewInp::MouseWheel( this, nFlags, zDelta, z_ptCurCursorD);

	return CWnd::OnMouseWheel(nFlags, zDelta, point);
}

void CChildView1::OnSize(UINT nType, int cx, int cy)
{
	MINT	iMM_sv;
	CWnd::OnSize(nType, cx, cy);

	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����܂��B

	if (cx > 0 && cy > 0) {
		CPoint	ptMouthD( cx, cy);							// ��ʃT�C�Y(�\�����W)
		CPoint	ptMouthL = ptMouthD;

		CClientDC dc( this);

		iMM_sv = dc.SetMapMode( MMAPMODE);
		dc.DPtoLP( &ptMouthL);								// ��ʃT�C�Y(�_�����W)
		dc.SetMapMode( iMM_sv);

		MC::ViewInp::Size( this, ptMouthD, ptMouthL);				// �E�B���h�E�T�C�Y�ݒ�
	}
}

void CChildView1::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CChildView1::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

//1cvoid CChildView1::OnCancel1Pt()
//1c{
//1c	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
//1c}

//1cvoid CChildView1::OnUpdateCancel1Pt(CCmdUI *pCmdUI)
//1c{
//1c	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
//1c}

//1cvoid CChildView1::OnCancelAll()
//1c{
//1c	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
//1c}

//1cvoid CChildView1::OnUpdateCancelAll(CCmdUI *pCmdUI)
//1c{
//1c	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
//1c}

//1cvoid CChildView1::OnInpKai()
//1c{
//1c	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
//1c}

//1cvoid CChildView1::OnUpdateInpKai(CCmdUI *pCmdUI)
//1c{
//1c	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
//1c}

//1cvoid CChildView1::OnInpAll()
//1c{
//1c	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
//1c}

//1cvoid CChildView1::OnUpdateInpAll(CCmdUI *pCmdUI)
//1c{
//1c	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
//1c}

//1cvoid CChildView1::OnInpEnd()
//1c{
//1c	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
//1c}

//1cvoid CChildView1::OnUpdateInpEnd(CCmdUI *pCmdUI)
//1c{
//1c	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
//1c}

//1cvoid CChildView1::OnModeCreate()
//1c{
//1c	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
//1c}

//1cvoid CChildView1::OnUpdateModeCreate(CCmdUI *pCmdUI)
//1c{
//1c	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
//1c}

//1cvoid CChildView1::OnModeDelete()
//1c{
//1c	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
//1c}

//1cvoid CChildView1::OnUpdateModeDelete(CCmdUI *pCmdUI)
//1c{
//1c	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
//1c}

//1cvoid CChildView1::OnModeModify()
//1c{
//1c	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
//1c}

//1cvoid CChildView1::OnUpdateModeModify(CCmdUI *pCmdUI)
//1c{
//1c	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
//1c}

//1cvoid CChildView1::OnModeMove()
//1c{
//1c	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
//1c}

//1cvoid CChildView1::OnUpdateModeMove(CCmdUI *pCmdUI)
//1c{
//1c	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
//1c}

//1cvoid CChildView1::OnModeCopy()
//1c{
//1c	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
//1c}

//1cvoid CChildView1::OnUpdateModeCopy(CCmdUI *pCmdUI)
//1c{
//1c	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
//1c}

//1cvoid CChildView1::OnModeProperty()
//1c{
//1c	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
//1c}

//1cvoid CChildView1::OnUpdateModeProperty(CCmdUI *pCmdUI)
//1c{
//1c	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
//1c}

void CChildView1::OnZoomInit()
{
	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
	MINT	nGrid[4] = { 16, -3, -3, 17}; 
	MC::WindowCtrl::MmGridNumSet( nGrid);
	MC::WindowCtrl::MmGridNumXqt();
}

void CChildView1::OnUpdateZoomInit(CCmdUI *pCmdUI)
{
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
}

//1cvoid CChildView1::OnZoomPan()
//1c{
//1c	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
//1c}

//1cvoid CChildView1::OnUpdateZoomPan(CCmdUI *pCmdUI)
//1c{
//1c	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
//1c}

//1cvoid CChildView1::OnZoomSz()
//1c{
//1c	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
//1c}

//1cvoid CChildView1::OnUpdateZoomSz(CCmdUI *pCmdUI)
//1c{
//1c	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
//1c}

//1cvoid CChildView1::OnUndo()
//1c{
//1c	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
//1c	MtSendMessage( WM_CMDMENUP, IDC_UNDO, NULL);
//1c}

//1cvoid CChildView1::OnUpdateUndo(CCmdUI *pCmdUI)
//1c{
//1c	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
//1c	pCmdUI->Enable( MC::MmUndoIsNotEmpty());
//1c}

//1cvoid CChildView1::OnRedo()
//1c{
//1c	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
//1c	MtSendMessage( WM_CMDMENUP, IDC_REDO, NULL);
//1c}

//1cvoid CChildView1::OnUpdateRedo(CCmdUI *pCmdUI)
//1c{
//1c	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
//1c	pCmdUI->Enable( MC::MmRedoIsNotEmpty());
//1c}

//1cvoid CChildView1::OnShowWindow(BOOL bShow, UINT nStatus)
//1c{
//1c	CWnd::OnShowWindow(bShow, nStatus);

//1c	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����܂��B
//1c}

LRESULT CChildView1::OnMyMessagePtsEdit(WPARAM wParam, LPARAM lParam)
{
	MC::MCmdPtsEditInp( this);
	return 0;
}	

namespace MC
{

void MCmdSuudokuiInp(
						CWnd*		pWnd			// (I  ) �E�B���h�E�̃C���X�^���X
				);

} // namespace MC

//1cvoid CChildView1::OnPtsEdit()
//1c{
//1c	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B

//1c//	MC::MmPtsEditInp( this);
//1c	MCmdPtsEditInp( this);
//1c//	MCmdSuudokuiInp( this);
//1c}

//1c
//1cvoid CChildView1::OnUpdatePtsEdit(CCmdUI *pCmdUI)
//1c{
//1c	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
//1c}

LRESULT CChildView1::OnMyMessageGridNum(WPARAM wParam, LPARAM lParam)
{
	MC::MCmdGridNumInp( this);
	return 0;
}	

//1cvoid CChildView1::OnGridNum()
//1c{
//1c	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
//1c
//1c//	MC::MmGridNumInp( this);

//1c	MtSendMessage( WM_CMDMENUP, IDC_GRIDNUM, NULL);
//1c}

//1cvoid CChildView1::OnUpdateGridNum(CCmdUI *pCmdUI)
//1c{
//1c	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
//1c}

LRESULT CChildView1::OnMyMessageOpt(WPARAM wParam, LPARAM lParam)
{
	MC::MCmdOptInp( this);
	return 0;
}	

//1cvoid CChildView1::OnOpt()
//1c{
//1c//	CMmDialogOpt myDL;
//1c//	myDL.DoModal();
//1c//	MC::MhOptV::MmOptInp( this);

//1c	MtSendMessage( WM_CMDMENUP, IDC_OPTION, NULL);
//1c}

//1c//void MC::MmHistInp(
//1c//						CWnd*		pWnd			// (I  ) �E�B���h�E�̃C���X�^���X
//1c//				);
LRESULT CChildView1::OnMyMessageHist(WPARAM wParam, LPARAM lParam)
{
	MC::MCmdHistInp( this);
	return 0;
}	

//1cvoid CChildView1::OnHist()
//1c{
//1c	MtSendMessage( WM_CMDMENUP, IDC_HIST, NULL);
//1c}

LRESULT CChildView1::OnMyMessageTenkai(WPARAM wParam, LPARAM lParam)
{
	MC::MCmdTenkaiInp( this);
	return 0;
}	

//1cvoid CChildView1::OnTenkai()
//1c{
//1c	MtSendMessage( WM_CMDMENUP, IDC_TENKAI, NULL);	
//1c}

//1cvoid CChildView1::OnChukimove()
//1c{
//1c	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
//1c}

//1cvoid CChildView1::OnUpdateChukimove(CCmdUI *pCmdUI)
//1c{
//1c	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
//1c}

//1cvoid CChildView1::OnChukidel()
//1c{
//1c	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
//1c}

//1cvoid CChildView1::OnUpdateChukidel(CCmdUI *pCmdUI)
//1c{
//1c	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
//1c}

//1cvoid CChildView1::OnChukidlgon()
//1c{
//1c	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
//1c}

//1cvoid CChildView1::OnUpdateChukidlgon(CCmdUI *pCmdUI)
//1c{
//1c	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
//1c}

//1cvoid CChildView1::OnPanelBefore()
//1c{
//1c	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
//1c}

//1cvoid CChildView1::OnUpdatePanelBefore(CCmdUI *pCmdUI)
//1c{
//1c	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
//1c}

//1cvoid CChildView1::OnPanelNext()
//1c{
//1c	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
//1c}

//1cvoid CChildView1::OnUpdatePanelNext(CCmdUI *pCmdUI)
//1c{
//1c	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
//1c}

//1cvoid CChildView1::OnPanelAll()
//1c{
//1c	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
//1c}

//1cvoid CChildView1::OnUpdatePanelAll(CCmdUI *pCmdUI)
//1c{
//1c	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
//1c}

BOOL CChildView1::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	MC::Window::SetCursor( this);

//	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
//	SetCursor(AfxGetApp()->LoadCursor(IDC_CROSS1));

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CChildView1::OnKillFocus(CWnd* pNewWnd)
{
	CWnd::OnKillFocus(pNewWnd);

	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����܂��B
}

void CChildView1::OnCancelMode()
{
	CWnd::OnCancelMode();

	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����܂��B
}

void CChildView1::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);

	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����܂��B
	ms_pMainFrame->m_pCurView = this;
}

int CChildView1::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO :  �����ɓ���ȍ쐬�R�[�h��ǉ����Ă��������B
//C	MC::MmWndSetWndC( 1, this);
//C	MINT ist = MC::WindowCtrl::MmWndKAddC();
//C	_ASSERTE(ist == 0);

//    SetBkColor( RGB(0,0,250) );
//Q?	lpCreateStruct->lpszName = Mstr( "TEST");

	return 0;
}
/*
void CChildView1::OnClose()
{
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	MC::MmWndInfo*	pWndInfo = MC::WindowCtrl::MmWndKFind( this);					// �Ǘ����̊J��
	MC::WindowCtrl::MmWndKDelete( pWndInfo);

	CWnd::OnClose();
}
*/
void CChildView1::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	MC::ViewInp::Char( this, nChar, nRepCnt, nFlags);

	CWnd::OnChar(nChar, nRepCnt, nFlags);
}

void CChildView1::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	MC::ViewInp::KeyDown( this, nChar, nRepCnt, nFlags);

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CChildView1::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	MC::ViewInp::KeyUp( this, nChar, nRepCnt, nFlags);

	CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}

int CChildView1::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	SetFocus();

	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

LRESULT CChildView1::OnMyMessageStatusBar(WPARAM wParam, LPARAM lParam)
{
	MC::Msg::DispStatusBar();
	return 0;
}	

//1cvoid CChildView1::OnFilePrintPreview()
//1c{
//1c	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
//1c	Printing printwnd;
//1c	printwnd.Initialize(this);
//1c	printwnd.ShowWindow(SW_NORMAL);
//1c	MessageBox(Mstr( "Wait Print Preview"), Mstr( ""), MB_OK);
//1c}

//1cvoid CChildView1::OnUpdateFilePrintPreview(CCmdUI *pCmdUI)
//1c{
//1c	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
//1c}


void CChildView1::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	MC::ViewInp::LButtonDblClk( this, nFlags, point);

	CWnd::OnLButtonDblClk(nFlags, point);
}

void CChildView1::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	MC::ViewInp::RButtonDblClk( this, nFlags, point);

	CWnd::OnRButtonDblClk(nFlags, point);
}

void CChildView1::OnMButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	MC::ViewInp::MButtonDblClk( this, nFlags, point);

	CWnd::OnMButtonDblClk(nFlags, point);
}

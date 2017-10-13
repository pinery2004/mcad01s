// MainFrm.cpp : CMainFrame �N���X�̎���
//
#include "stdafx.h"
#include "resource.h"
#include "MCAD.h"
#include "MmCmdMsg.h"
#include "MainFrm.h"
#include "MmLib.h"
#include "MhPlcInfo.h"

#include "ChildFrm1.h"
#include "ChildFrm2.h"
#include "ChildFrm3.h"
#include "ChildFrm4.h"
#include "ChildFrm5.h"
#include "MmWnd.h"
#include ".\mainfrm.h"
#include "MhLib.h"

#include "MtAskForPoint.h"								// MtInit,MtClose,MtSendMessage
#include "MCAdApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void	MmMainFrame( CMainFrame *pMainFrame);

CMainFrame *ms_pMainFrame;										// ���C���t���[���|�C���^(Global) 

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
//	ON_COMMAND(ID_VIEW1_ON, OnView1On)
//	ON_COMMAND(ID_VIEW2_ON, OnView2On)
//	ON_COMMAND(ID_VIEW3_ON, OnView3On)
//	ON_COMMAND(ID_VIEW4_ON, OnView4On)
//	ON_COMMAND(ID_VIEW5_ON, OnView5On)
//	ON_COMMAND(ID_VIEW1_OFF, OnView1Off)
//	ON_COMMAND(ID_VIEW2_OFF, OnView2Off)
//	ON_COMMAND(ID_VIEW3_OFF, OnView3Off)
//	ON_COMMAND(ID_VIEW4_OFF, OnView4Off)
//	ON_COMMAND(ID_VIEW5_OFF, OnView5Off)
ON_WM_CLOSE()
ON_CBN_SELCHANGE(IDC_CMB_BZI1, OnCbnSelchangeCombo1)
ON_CBN_SELCHANGE(IDC_CMB_BZI2, OnCbnSelchangeCombo2)
ON_CBN_SELCHANGE(IDC_CMB_Inp1, OnCbnSelchangeCombo11)
ON_CBN_SELCHANGE(IDC_CMB_Inp2, OnCbnSelchangeCombo12)
ON_CBN_SELCHANGE(IDC_CMB_Inp3, OnCbnSelchangeCombo13)
ON_CBN_SELCHANGE(IDC_COMBOPANELNO, OnCbnSelchangeComboPanelNo)
ON_CBN_SELCHANGE(IDC_CMB_ATTR1, OnCbnSelchangeComboAttr1)
ON_CBN_SELCHANGE(IDC_CMB_ATTR2, OnCbnSelchangeComboAttr2)
ON_CBN_SELCHANGE(IDC_CMB_ATTR3, OnCbnSelchangeComboAttr3)
ON_CBN_SELCHANGE(IDC_CMB_ATTR4, OnCbnSelchangeComboAttr4)
ON_CBN_SELCHANGE(IDC_CMB_ATTR5, OnCbnSelchangeComboAttr5)
ON_CBN_SELCHANGE(IDC_CMB_ATTR6, OnCbnSelchangeComboAttr6)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // �X�e�[�^�X ���C�� �C���W�P�[�^
	ID_INDICATOR_EROR,
	ID_INDICATOR_MUTH,
	ID_INDICATOR_CMND,
	ID_INDICATOR_KANA,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame �R���X�g���N�V����/�f�X�g���N�V����

CMainFrame::CMainFrame()
{
	// TODO: �����o�������R�[�h�������ɒǉ����Ă��������B
	ms_pMainFrame = this;
	MC::System::SetpMainFrame( this);

//	MtInit( GetSafeHwnd(), 1);
	MC::MtCmdOpen();
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
// ------------------------- �㕔��i�c�[���o�[�@IDR_MAINTOOL

	if (!m_wndToolBar1.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | 
						CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar1.LoadToolBar( IDR_MAINTOOL))
	{
		TRACE0( "�c�[�� �o�[�̍쐬�Ɏ��s���܂����B\n");
		return -1;      // �쐬�ł��܂���ł����B
	}

// ------------------------- �������i�X�e�C�^�X�o�[

	if (!m_wndStatusBar.Create( this) ||
		!m_wndStatusBar.SetIndicators( indicators, sizeof( indicators)/sizeof(UINT)))
	{
		TRACE0( "�X�e�[�^�X �o�[�̍쐬�Ɏ��s���܂����B\n");
		return -1;      // �쐬�ł��܂���ł����B
	}

	UINT			nStBarId;															// �h�c
	UINT			nStBarStyle;														// �X�^�C��
	int				cxStBarSbWidth;														// ��

	m_wndStatusBar.GetPaneInfo( 1, nStBarId, nStBarStyle, cxStBarSbWidth);				// �G���[
	m_wndStatusBar.SetPaneInfo( 1, nStBarId, nStBarStyle, 480);							// ��
	m_wndStatusBar.GetPaneInfo( 2, nStBarId, nStBarStyle, cxStBarSbWidth);				// ���W
	m_wndStatusBar.SetPaneInfo( 2, nStBarId, nStBarStyle, 80);							// ��
	m_wndStatusBar.GetPaneInfo( 3, nStBarId, nStBarStyle, cxStBarSbWidth);				// ����
	m_wndStatusBar.SetPaneInfo( 3, nStBarId, nStBarStyle, 70);							// ��

	// TODO: �c�[�� �o�[���h�b�L���O�\�ɂ��Ȃ��ꍇ�́A������ 3 �s���폜���Ă��������B
	m_wndToolBar1.EnableDocking( CBRS_ALIGN_ANY);
	EnableDocking( CBRS_ALIGN_ANY);
	DockControlBar( &m_wndToolBar1);

// ------------------------- �㕔��i�E�_�C�A���O�o�[ IDD_DIALOGBARPANEL

	if (!m_wndDlgBar4.Create( this, IDD_DIALOGBARPANEL,
						CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,
						IDD_DIALOGBARPANEL))
	{
		TRACE0( "Failed to create DlgBar\n");
	}

	m_wndDlgBar4.EnableDocking( CBRS_ALIGN_ANY);
	EnableDocking( CBRS_ALIGN_ANY);
	DockControlBar( &m_wndDlgBar4);
	m_wndDlgBar4.SetWindowText(Mstr( "�o�`�m�d�k�̕\���I��"));

	CRect RC;

	RecalcLayout();
	m_wndToolBar1.GetWindowRect(&RC);
	RC.OffsetRect(1,0);
//	CBRS_ALIGN_TOP		�� AFX_IDW_DOCKBAR_TOP
//	CBRS_ALIGN_BOTTOM	�� AFX_IDW_DOCKBAR_BOTTOM
//	CBRS_ALIGN_LEFT		�� AFX_IDW_DOCKBAR_LEFT
//	CBRS_ALIGN_RIGHT	�� AFX_IDW_DOCKBAR_RIGHT

	DockControlBar( &m_wndDlgBar4, AFX_IDW_DOCKBAR_TOP, &RC);

// ------------------------- �㕔���i���_�C�A���O�o�[ IDD_DIALOGBARINPUT

	if (!m_wndDlgBar1.Create( this, IDD_DIALOGBARINPUT,
						CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,
						IDD_DIALOGBARINPUT))
	{
		TRACE0( "Failed to create DlgBar\n");
	}

	m_wndDlgBar1.EnableDocking( CBRS_ALIGN_ANY);
	EnableDocking( CBRS_ALIGN_ANY);
	DockControlBar( &m_wndDlgBar1);
	m_wndDlgBar1.SetWindowText(Mstr( "�\�����ނ̓���"));

// ------------------------- �㕔���i�E�_�C�A���O�o�[ 1 IDD_DIALOGBARSEKKEI
// �Q�l�@http://www.alpha-net.ne.jp/users2/uk413/vc/VCT_DlgBar.html

	if (!m_wndDlgBar6.Create( this, IDD_DIALOGBARSEKKEI,
						CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,
						IDD_DIALOGBARSEKKEI))
	{
		TRACE0( "Failed to create DlgBar\n");
	}

	m_wndDlgBar6.EnableDocking( CBRS_ALIGN_ANY);
	EnableDocking( CBRS_ALIGN_ANY);
	DockControlBar( &m_wndDlgBar6);
	m_wndDlgBar6.SetWindowText(Mstr( "�ӏ����͂̑I��"));

	RecalcLayout();
	m_wndDlgBar1.GetWindowRect(&RC);
	RC.OffsetRect(1,0);

	DockControlBar( &m_wndDlgBar6, AFX_IDW_DOCKBAR_TOP, &RC);

	ShowControlBar( &m_wndDlgBar6, TRUE, FALSE);

// ------------------------- �㕔���i�E�_�C�A���O�o�[ 1 IDD_DIALOGBARPARTS

	if (!m_wndDlgBar3.Create( this, IDD_DIALOGBARPARTS,
						CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,
						IDD_DIALOGBARPARTS))
	{
		TRACE0( "Failed to create DlgBar\n");
	}

	m_wndDlgBar3.EnableDocking( CBRS_ALIGN_ANY);
	EnableDocking( CBRS_ALIGN_ANY);
	DockControlBar( &m_wndDlgBar3);
	m_wndDlgBar3.SetWindowText(Mstr( "�\�����ނ̑I��"));

//	CRect RC;

	RecalcLayout();
	m_wndDlgBar1.GetWindowRect(&RC);
	RC.OffsetRect(1,0);
//	CBRS_ALIGN_TOP		�� AFX_IDW_DOCKBAR_TOP
//	CBRS_ALIGN_BOTTOM	�� AFX_IDW_DOCKBAR_BOTTOM
//	CBRS_ALIGN_LEFT		�� AFX_IDW_DOCKBAR_LEFT
//	CBRS_ALIGN_RIGHT	�� AFX_IDW_DOCKBAR_RIGHT

	DockControlBar( &m_wndDlgBar3, AFX_IDW_DOCKBAR_TOP, &RC);

	ShowControlBar( &m_wndDlgBar3, FALSE, FALSE);

// ------------------------- ������i���_�C�A���O�o�[ IDD_DIALOGBARDISP

	if (!m_wndDlgBar2.Create( this, IDD_DIALOGBARDISP,
						CBRS_BOTTOM | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,
						IDD_DIALOGBARDISP))
	{
		TRACE0( "Failed to create DlgBar\n");
	}

	m_wndDlgBar2.EnableDocking( CBRS_ALIGN_ANY);
	EnableDocking( CBRS_ALIGN_ANY);
	DockControlBar( &m_wndDlgBar2);

// ------------------------- ������i�E�_�C�A���O�o�[ IDD_DIALOGBARATTR

	if (!m_wndDlgBar5.Create( this, IDD_DIALOGBARATTR,
						CBRS_BOTTOM | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,
						IDD_DIALOGBARATTR))
	{
		TRACE0( "Failed to create DlgBar\n");
	}

	m_wndDlgBar5.EnableDocking( CBRS_ALIGN_ANY);
	EnableDocking( CBRS_ALIGN_ANY);
	DockControlBar( &m_wndDlgBar5);
	m_wndDlgBar5.SetWindowText(Mstr( "�\�����ނ̑���"));

	RecalcLayout();
	m_wndDlgBar2.GetWindowRect(&RC);
	RC.OffsetRect(1,0);

	DockControlBar( &m_wndDlgBar5, AFX_IDW_DOCKBAR_BOTTOM, &RC);

// ------------------------- �E���c�[���o�[ IDR_SUB1TOOL

	if (!m_wndToolBar2.CreateEx( this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE |
						CBRS_RIGHT | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar2.LoadToolBar( IDR_SUB1TOOL))
	{
		TRACE0( "�c�[�� �o�[�̍쐬�Ɏ��s���܂����B\n");
		return -1;      // �쐬�ł��܂���ł����B
	}

	// TODO: �c�[�� �o�[���h�b�L���O�\�ɂ��Ȃ��ꍇ�́A������ 3 �s���폜���Ă��������B
	m_wndToolBar2.EnableDocking( CBRS_ALIGN_ANY);
	EnableDocking( CBRS_ALIGN_ANY);
	DockControlBar( &m_wndToolBar2);

// -------------------------

	//*** Load the aliases.
//	LoadAliases();

	return 0;
}

BOOL CMainFrame::PreCreateWindow( CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C������ Window �N���X�܂��̓X�^�C����
	//  �C�����Ă��������B
	//cs.x  = -GetSystemMetrics( SM_CXSIZEFRAME);
	//cs.y  = -GetSystemMetrics( SM_CYSIZEFRAME);
	//cs.cx = GetSystemMetrics( SM_CXMAXIMIZED);
	//cs.cy = GetSystemMetrics( SM_CYMAXIMIZED);
	cs.x  = 0;
	cs.y  = 0;
	cs.cx = min( GetSystemMetrics( SM_CXMAXIMIZED), 1280);
	cs.cy = min( GetSystemMetrics( SM_CYMAXIMIZED), 1060);

	int i11 = ::GetSystemMetrics( SM_CXBORDER);				// 1
	int i12 = ::GetSystemMetrics( SM_CYBORDER);				// 1
	int i13 = ::GetSystemMetrics( SM_CXEDGE);				// 2
	int i14 = ::GetSystemMetrics( SM_CYEDGE);				// 2
	int i15 = ::GetSystemMetrics( SM_CXFIXEDFRAME);			// 3
	int i16 = ::GetSystemMetrics( SM_CYFIXEDFRAME);			// 3
	int i17 = ::GetSystemMetrics( SM_CXFULLSCREEN);			// 1280
	int i18 = ::GetSystemMetrics( SM_CYFULLSCREEN);			// 968
	int i19 = ::GetSystemMetrics( SM_CXMAXIMIZED);			// 1288
	int i20 = ::GetSystemMetrics( SM_CYMAXIMIZED);			// 1002
	int i21 = ::GetSystemMetrics( SM_CXMAXTRACK);			// 1292
	int i22 = ::GetSystemMetrics( SM_CYMAXTRACK);			// 1036
	int i23 = ::GetSystemMetrics( SM_CXSCREEN);				// 1280
	int i24 = ::GetSystemMetrics( SM_CYSCREEN);				// 1024
	int i25 = ::GetSystemMetrics( SM_CXSIZEFRAME);			// 4
	int i26 = ::GetSystemMetrics( SM_CYSIZEFRAME);			// 4
	int i27 = ::GetSystemMetrics( SM_CYMENU);				// 20
	int i28 = ::GetSystemMetrics( SM_CYCAPTION);			// 26
	int i29 = ::GetSystemMetrics( SM_CYSMCAPTION);			// 18

	return TRUE;
}

// CMainFrame �f�f

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump( CDumpContext& dc) const
{
	CMDIFrameWnd::Dump( dc);
}

#endif //_DEBUG


// CMainFrame ���b�Z�[�W �n���h��


void CMainFrame::OnView1On()
{
	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
	CChildFrame1 *pChildFrm1 = new CChildFrame1;
	TRACE( "new CChildFrame1(%x)\n", pChildFrm1);
	MC::WindowCtrl::MmWndSetFrameC( 1, pChildFrm1);
	if (!pChildFrm1->Create( NULL, _T("Window1"),
		WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, rectDefault, this))
		return;
}

void CMainFrame::OnView2On()
{
	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
	CChildFrame2 *pChildFrm2 = new CChildFrame2;
	MC::WindowCtrl::MmWndSetFrameC( 2, pChildFrm2);
	if (!pChildFrm2->Create( NULL, _T("Window2"),
		WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, rectDefault, this))
		return;
}

void CMainFrame::OnView3On()
{
	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
	CChildFrame3 *pChildFrm3 = new CChildFrame3;
	MC::WindowCtrl::MmWndSetFrameC( 3, pChildFrm3);
	if (!pChildFrm3->Create( NULL, _T("Window3"),
		WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, rectDefault, this))
		return;
}

void CMainFrame::OnView4On()
{
	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
	CChildFrame4 *pChildFrm4 = new CChildFrame4;
	MC::WindowCtrl::MmWndSetFrameC( 4, pChildFrm4);
	if (!pChildFrm4->Create( NULL, _T("Window4"),
		WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, rectDefault, this))
		return;
}

void CMainFrame::OnView5On()
{
	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B

	CChildFrame5 *pChildFrm5 = new CChildFrame5;
	MC::WindowCtrl::MmWndSetFrameC( 5, pChildFrm5);
	if (!pChildFrm5->Create( NULL, _T("Window5"),
		WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, rectDefault, this))
		return;
}

//void CMainFrame::OnView1Off()
//{
//	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
//}

//void CMainFrame::OnView2Off()
//{
//	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
//}

//void CMainFrame::OnView3Off()
//{
//	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
//}

//void CMainFrame::OnView4Off()
//{
//	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
//}

//void CMainFrame::OnView5Off()
//{
//	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
//}

void CMainFrame::OnClose()
{
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	MINT	ist1;
	if (MC::IeModel::MhGetModFIeMdl()) {
		ist1 = MessageBox( Mstr( "�I�����܂���"), Mstr( "�I���m�F"), MB_OKCANCEL);
		if ( ist1 == IDCANCEL)
			return;
	}
	MC::MtCmdClose();
	MC::WindowCtrl::MmWndKDeleteAll();

	CMDIFrameWnd::OnClose();
}

// PARTS �_�C�A���O�o�[�@���ޖ�
void CMainFrame::OnCbnSelchangeCombo1()
{
	m_iCombo1 = ((CComboBox*)(m_wndDlgBar3.GetDlgItem(IDC_CMB_BZI1)))->GetCurSel();	// ����ID
	MC::InpAttr::SetComboKmIdTpPts( m_iCombo1);
	MC::InpAttr::InitComboPtsMbr();
	MC::Window::CurWndFocus();
	MC::MhTpPts* pTpPts	= MC::BuzaiCode::MhGetpTpPts( MC::InpAttr::GetCurIdTpPts());
	if (pTpPts->GetPTBr() >= MP_BR_SENBUN || MC::InpMode::GetMode() == MP_MD_DELETE)
		MC::WindowCtrl::MmWndKCmdXqt( IDC_PARTSCREATE);							//	���ޓ��̓R�}���h
	else 
		MC::WindowCtrl::MmWndKCmdXqt( IDC_CANCELCMD);							//	�R�}���h�L�����Z��

}

// PARTS �_�C�A���O�o�[�@���@�^��
void CMainFrame::OnCbnSelchangeCombo2()
{
	m_iCombo2 = ((CComboBox*)(m_wndDlgBar3.GetDlgItem(IDC_CMB_BZI2)))->GetCurSel();	// ���@�^��ID
	MC::InpAttr::SetComboKmIdMbr( m_iCombo2);
	MC::Window::CurWndFocus();

	MC::WindowCtrl::MmWndKCmdXqt( IDC_PARTSCREATE);							//	���i����
}

// INPUT �_�C�A���O�o�[�@���͓_�敪
void CMainFrame::OnCbnSelchangeCombo11()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	m_iComboInp1 = ((CComboBox*)(m_wndDlgBar1.GetDlgItem(IDC_CMB_Inp1)))->GetCurSel();
	MC::InpMode::SetComboInpKb( m_iComboInp1);
	MC::Window::CurWndFocus();
}

// INPUT �_�C�A���O�o�[�@�ۂ߃R�[�h
void CMainFrame::OnCbnSelchangeCombo12()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	m_iComboInp2 = ((CComboBox*)(m_wndDlgBar1.GetDlgItem(IDC_CMB_Inp2)))->GetCurSel();
	MC::InpMode::SetComboCdMarume( m_iComboInp2);
	MC::Window::CurWndFocus();
}

// INPUT �_�C�A���O�o�[�@�z�u�R�[�h
void CMainFrame::OnCbnSelchangeCombo13()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	m_iComboInp3 = ((CComboBox*)(m_wndDlgBar1.GetDlgItem(IDC_CMB_Inp3)))->GetCurSel();
	MC::HaitiIn::SetComboCdPlc( m_iComboInp3);
	MC::Window::CurWndFocus();
}

// PANEL �_�C�A���O�o�[�@�p�l���ԍ�
void CMainFrame::OnCbnSelchangeComboPanelNo()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
}

//void CMainFrame::SetComboPanelNo( MINT iComboPanelNo)

// ATTR �_�C�A���O�o�[�@�����␳�l1
void CMainFrame::OnCbnSelchangeComboAttr1()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
}

// ATTR �_�C�A���O�o�[�@�����␳�l2
void CMainFrame::OnCbnSelchangeComboAttr2()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
}

// ATTR �_�C�A���O�o�[�@�ގ��c�����
void CMainFrame::OnCbnSelchangeComboAttr3()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
}

// ATTR �_�C�A���O�o�[�@�z�u�_�����
void CMainFrame::OnCbnSelchangeComboAttr4()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
}

// ATTR �_�C�A���O�o�[�@���t������
void CMainFrame::OnCbnSelchangeComboAttr5()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
}

// ATTR �_�C�A���O�o�[�@�J��������(ROH)
void CMainFrame::OnCbnSelchangeComboAttr6()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
}


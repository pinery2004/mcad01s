// MCAD.cpp : �A�v���P�[�V�����̃N���X������`���܂��B
//
#include "stdafx.h"
#include "resource.h"
#include "MCAD.h"
#include "MCADApp.h"
#include "MainFrm.h"
#include "MmSystem.h"
#include "MmDefine.h"
#include "MmCmdMsg.h"
#include <Winspool.h>

#include "ChildFrm1.h"
#include "ChildFrm2.h"
#include "ChildFrm3.h"
#include "ChildFrm4.h"
#include "ChildFrm5.h"

#include "MmWnd.h"
#include "Mmlib.h"
#include "MhPlcInfo.h"
#include "MhInp.h"
#include "Mhlib.h"

#include "MmCmd.h"
//#include "MmDialogBarDisp.h"
#include "MtAskForPoint.h"								// MtSendMessage
#include "MdCmds.h"

#define		DEBUG001	0

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void	MmMCADApp( CMCADApp* ptheApp);
// CMCADApp

BEGIN_MESSAGE_MAP(CMCADApp, CWinApp)
ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
ON_COMMAND(ID_FILE_NEW, OnFileNew)
ON_COMMAND(ID_VIEW1_ON, OnView1On)
ON_COMMAND(ID_VIEW2_ON, OnView2On)
ON_COMMAND(ID_VIEW3_ON, OnView3On)
ON_COMMAND(ID_VIEW4_ON, OnView4On)
ON_COMMAND(ID_VIEW5_ON, OnView5On)
ON_COMMAND(ID_VIEW1_OFF, OnView1Off)
ON_COMMAND(ID_VIEW2_OFF, OnView2Off)
ON_COMMAND(ID_VIEW3_OFF, OnView3Off)
ON_COMMAND(ID_VIEW4_OFF, OnView4Off)
ON_COMMAND(ID_VIEW5_OFF, OnView5Off)

ON_COMMAND_RANGE(IDC_P0010, IDC_P0360, OnMainMenuP)
ON_UPDATE_COMMAND_UI_RANGE(IDC_P0010, IDC_P0360, OnUpdateMainMenuP)
ON_COMMAND_RANGE(IDP_P0010, IDP_P0370, OnSubMenuP)
ON_UPDATE_COMMAND_UI_RANGE(IDP_P0010, IDP_P0370, OnUpdateSubMenuP)

ON_BN_CLICKED(IDC_RADIOYANE, OnBnClickedRadioYane)
ON_UPDATE_COMMAND_UI(IDC_RADIOYANE, OnUpdateRadioYane)
ON_BN_CLICKED(IDC_RADIOTENJO, OnBnClickedRadioTenjo)
ON_UPDATE_COMMAND_UI(IDC_RADIOTENJO, OnUpdateRadioTenjo)
ON_BN_CLICKED(IDC_RADIOTAIRYOKU, OnBnClickedRadioTairyoku)
ON_UPDATE_COMMAND_UI(IDC_RADIOTAIRYOKU, OnUpdateRadioTairyoku)
ON_BN_CLICKED(IDC_RADIOKABE, OnBnClickedRadioKabe)
ON_UPDATE_COMMAND_UI(IDC_RADIOKABE, OnUpdateRadioKabe)
ON_BN_CLICKED(IDC_RADIOYUKA, OnBnClickedRadioYuka)
ON_UPDATE_COMMAND_UI(IDC_RADIOYUKA, OnUpdateRadioYuka)
ON_BN_CLICKED(IDC_RADIOKOYA, OnBnClickedRadioKoya)
ON_UPDATE_COMMAND_UI(IDC_RADIOKOYA, OnUpdateRadioKoya)
ON_BN_CLICKED(IDC_RADIODODAI, OnBnClickedRadioDodai)
ON_UPDATE_COMMAND_UI(IDC_RADIODODAI, OnUpdateRadioDodai)
ON_BN_CLICKED(IDC_RADIOKISO, OnBnClickedRadioKiso)
ON_UPDATE_COMMAND_UI(IDC_RADIOKISO, OnUpdateRadioKiso)
ON_BN_CLICKED(IDC_RADIOBUZAI, OnBnClickedRadioPts)
ON_UPDATE_COMMAND_UI(IDC_RADIOBUZAI, OnUpdateRadioPts)
ON_BN_CLICKED(IDC_RADIOKANAGU, OnBnClickedRadioKanagu)
ON_UPDATE_COMMAND_UI(IDC_RADIOKANAGU, OnUpdateRadioKanagu)
ON_BN_CLICKED(IDC_RADIOPANEL, OnBnClickedRadioPanel)
ON_UPDATE_COMMAND_UI(IDC_RADIOPANEL, OnUpdateRadioPanel)
ON_BN_CLICKED(IDC_RADIOOTHER, OnBnClickedRadioOther)
ON_UPDATE_COMMAND_UI(IDC_RADIOOTHER, OnUpdateRadioOther)

ON_BN_CLICKED(IDC_RADIOSEL1, OnBnClickedRadioSel1)
ON_UPDATE_COMMAND_UI(IDC_RADIOSEL1, OnUpdateRadioSel1)
ON_BN_CLICKED(IDC_RADIOSEL2, OnBnClickedRadioSel2)
ON_UPDATE_COMMAND_UI(IDC_RADIOSEL2, OnUpdateRadioSel2)
ON_BN_CLICKED(IDC_RADIOSEL3, OnBnClickedRadioSel3)
ON_UPDATE_COMMAND_UI(IDC_RADIOSEL3, OnUpdateRadioSel3)
ON_BN_CLICKED(IDC_CHUKIDELETE, OnBnClickedChukiDelete)
ON_UPDATE_COMMAND_UI(IDC_CHUKIDELETE, OnUpdateChukiDelete)
ON_BN_CLICKED(IDC_CHUKIMOVE, OnBnClickedChukiMove)
ON_UPDATE_COMMAND_UI(IDC_CHUKIMOVE, OnUpdateChukiMove)
ON_BN_CLICKED(IDC_CHUKICHANGE, OnBnClickedChukiChange)
ON_UPDATE_COMMAND_UI(IDC_CHUKICHANGE, OnUpdateChukiChange)
ON_BN_CLICKED(IDC_CHECKBUZAI, OnBnClickedCheckBuzai)
ON_UPDATE_COMMAND_UI(IDC_CHECKBUZAI, OnUpdateCheckBuzai)
ON_BN_CLICKED(IDC_CHECKKANAGU, OnBnClickedCheckKanagu)
ON_UPDATE_COMMAND_UI(IDC_CHECKKANAGU, OnUpdateCheckKanagu)
ON_BN_CLICKED(IDC_CHECKGOHAN, OnBnClickedCheckGohan)
ON_UPDATE_COMMAND_UI(IDC_CHECKATTR1, OnUpdateCheckGohan)
ON_BN_CLICKED(IDC_CHECKATTR1, OnBnClickedCheckAttr1)
ON_UPDATE_COMMAND_UI(IDC_CHECKATTR2, OnUpdateCheckAttr1)
ON_BN_CLICKED(IDC_CHECKATTR2, OnBnClickedCheckAttr2)
ON_UPDATE_COMMAND_UI(IDC_CHECKATTR3, OnUpdateCheckAttr2)
ON_BN_CLICKED(IDC_CHECKATTR3, OnBnClickedCheckAttr3)
ON_UPDATE_COMMAND_UI(IDC_CHECKATTR4, OnUpdateCheckAttr3)
ON_BN_CLICKED(IDC_CHECKATTR4, OnBnClickedCheckAttr4)
ON_UPDATE_COMMAND_UI(IDC_CHECKBUZAI, OnUpdateCheckAttr4)
ON_BN_CLICKED(IDC_RADIO1F, OnBnClickedRadio1f)
ON_UPDATE_COMMAND_UI(IDC_RADIO1F, OnUpdateRadio1f)
ON_BN_CLICKED(IDC_RADIO2F, OnBnClickedRadio2f)
ON_UPDATE_COMMAND_UI(IDC_RADIO2F, OnUpdateRadio2f)
ON_BN_CLICKED(IDC_RADIO3F, OnBnClickedRadio3f)
ON_UPDATE_COMMAND_UI(IDC_RADIO3F, OnUpdateRadio3f)
ON_BN_CLICKED(IDC_PANELBEFORE, OnBnClickedPanelBefore)
ON_UPDATE_COMMAND_UI(IDC_PANELBEFORE, OnUpdatePanelBefore)
ON_BN_CLICKED(IDC_PANELNEXT, OnBnClickedPanelNext)
ON_UPDATE_COMMAND_UI(IDC_PANELNEXT, OnUpdatePanelNext)
ON_BN_CLICKED(IDC_PANELALL, OnBnClickedPanelAll)
ON_UPDATE_COMMAND_UI(IDC_PANELALL, OnUpdatePanelAll)
ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, OnUpdateFileOpen)
ON_COMMAND(ID_FILE_SAVE, OnFileSave)
ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
//ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdateFilePrint)
//ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
//ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, OnUpdateFilePrintPreview)
//ON_COMMAND(ID_FILE_PRINT_SETUP, OnFilePrintSetup)
ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_SETUP, OnUpdateFilePrintSetup)
ON_COMMAND(ID_APP_EXIT, OnAppExit)

ON_COMMAND(IDM_ISYOU, &CMCADApp::OnIsyou)
ON_UPDATE_COMMAND_UI(IDM_ISYOU, &CMCADApp::OnUpdateIsyou)
ON_COMMAND(IDM_KOUZOU, &CMCADApp::OnKouzou)
ON_UPDATE_COMMAND_UI(IDM_KOUZOU, &CMCADApp::OnUpdateKouzou)

ON_BN_CLICKED(IDC_RADIOJYUUKO, &CMCADApp::OnBnClickedRadioJyuuko)
ON_UPDATE_COMMAND_UI(IDC_RADIOJYUUKO, OnUpdateRadioJyuuko)
ON_BN_CLICKED(IDC_RADIOJYUUTO, &CMCADApp::OnBnClickedRadioJyuuto)
ON_UPDATE_COMMAND_UI(IDC_RADIOJYUUTO, OnUpdateRadioJyuuto)
ON_BN_CLICKED(IDC_RADIOBUKKEN, &CMCADApp::OnBnClickedRadioBukken)
ON_UPDATE_COMMAND_UI(IDC_RADIOBUKKEN, OnUpdateRadioBukken)
ON_BN_CLICKED(IDC_RADIOSYANE, &CMCADApp::OnBnClickedRadioSYane)
ON_UPDATE_COMMAND_UI(IDC_RADIOSYANE, OnUpdateRadioSYane)
ON_BN_CLICKED(IDC_RADIOSIKITI, &CMCADApp::OnBnClickedRadioSikiti)
ON_UPDATE_COMMAND_UI(IDC_RADIOSIKITI, OnUpdateRadioSikiti)
ON_BN_CLICKED(IDC_RADIOHEKIRYOU, &CMCADApp::OnBnClickedRadioHekiryou)
ON_UPDATE_COMMAND_UI(IDC_RADIOHEKIRYOU, OnUpdateRadioHekiryou)
END_MESSAGE_MAP()

// CMCADApp �R���X�g���N�V����

CMCADApp::CMCADApp()
{
	// TODO: ���̈ʒu�ɍ\�z�p�R�[�h��ǉ����Ă��������B
	// ������ InitInstance ���̏d�v�ȏ��������������ׂċL�q���Ă��������B
	MC::System::SetpMCADApp( this);
}


// �B��� CMCADApp �I�u�W�F�N�g�ł��B

CMCADApp g_theApp;

// CMCADApp ������

BOOL CMCADApp::InitInstance()
{
	//	save copies of instance handles accessible to DLLs
	MC::MCadSharedGlobals::SetMCadAppInstance( AfxGetInstanceHandle());
	MC::MCadSharedGlobals::SetMCadResourceInstance( AfxGetInstanceHandle());	// no resource DLL yet

//	MC::MCadSharedGlobals::SetCmdMessage(&sds_printf);

	// �A�v���P�[�V�����@�}�j�t�F�X�g���@visual �X�^�C����L���ɂ��邽�߂ɁA
	// ComCtl32.dll �o�[�W���� 6�@�ȍ~�̎g�p���w�肷��ꍇ�́A
	// Windows XP �Ɂ@InitCommonControls() ���K�v�ł��B�����Ȃ���΁A�E�B���h�E�쐬�͂��ׂĎ��s���܂��B
	InitCommonControls();

	CWinApp::InitInstance();

	// OLE ���C�u���������������܂��B
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	
	// �W��������
	// �����̋@�\���g�킸�ɁA�ŏI�I�Ȏ��s�\�t�@�C���̃T�C�Y���k���������ꍇ�́A
	// �ȉ�����A�s�v�ȏ��������[�`����
	// �폜���Ă��������B
	// �ݒ肪�i�[����Ă��郌�W�X�g�� �L�[��ύX���܂��B
	// TODO: ���̕�������A��Ж��܂��͑g�D���Ȃǂ́A
	// �K�؂ȕ�����ɕύX���Ă��������B
	SetRegistryKey(_T("�A�v���P�[�V���� �E�B�U�[�h�Ő������ꂽ���[�J�� �A�v���P�[�V����"));
	
	// ���C�� �E�C���h�E���쐬����Ƃ��A���̃R�[�h�͐V�����t���[�� �E�C���h�E �I�u�W�F�N�g���쐬���A
	// ������A�v���P�[�V�����̃��C�� �E�C���h�E�ɃZ�b�g���܂�
	CMDIFrameWnd* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	
	// ���C�� MDI �t���[�� �E�B���h�E���쐬���܂��B
	if (!pFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	
	// ���ʂ� MDI ���j���[�ƃA�N�Z�����[�^ �e�[�u����ǂݍ��݂܂��B
	//TODO: �ǉ��̃����o�ϐ��������āA�A�v���P�[�V�������K�v�Ƃ���
	//	�ǉ��̃��j���[�^�C�v�̂��߂ɌĂяo����ǂݍ���ł��������B
	HINSTANCE hInst = AfxGetResourceHandle();
	m_hMDIMenu  = ::LoadMenu(hInst, MAKEINTRESOURCE(IDR_MCADTYPE));
	m_hMDIAccel = ::LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_MCADTYPE));
	
//-----------------------------------------------------------------------------

	MC::System::MmInitialize();

	MC::InpMode::SetKai( MP_KAI1);
	MC::MhPlcInp::SetKai( MC::InpMode::GetKai());
	MC::InpMode::SetMode( MP_MD_CREATE);

	MC::InpMode::SetSGp( MP_GP_JYUKO);

	MC::InpMode::SetKGp( MP_GP_KABE);
	MC::InpMode::SetKBr( MP_BR_BUZAI);
	MC::InpMode::SetKSel( MP_SEL1);
	m_bPtsListBuzai	= false;
	m_bPtsListKanagu = false;
	m_bPtsListGohan	= false;

//-----------------------------------------------------------------------------

	// ���C�� �E�B���h�E�����������ꂽ�̂ŁA�\���ƍX�V���s���܂��B
	pFrame->ShowWindow(m_nCmdShow);
	pFrame->UpdateWindow();

//	((CMainFrame*)pFrame)->OnView1On();
	CreateWnd( 1, MTHEIMENZU, 1, 1);

	MC::InpAttr::InitComboAttr( MP_AT_HRZ_PARTS);							// �������͗p�R���{�{�b�N�X������������
	MC::InpAttr::InitComboTpPtsAttr();										// �g�A���ނɑΉ����镔�i��I���\���ڂƂ��Đݒ肷��

	return TRUE;
}

// CMCADApp ���b�Z�[�W �n���h��

int CMCADApp::ExitInstance() 
{
	//TODO: �ǉ��������\�[�X������ꍇ�ɂ͂������������Ă�������
	MC::System::MmTerminate();

	if (m_hMDIMenu != NULL)
		FreeResource(m_hMDIMenu);
	if (m_hMDIAccel != NULL)
		FreeResource(m_hMDIAccel);

	return CWinApp::ExitInstance();
}

void CMCADApp::OnFileNew() 
{
//A ------------------------------------------------------------------------
//A	CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);

	// �V���� MDI �q�E�B���h�E���쐬���܂�
//A	pFrame->CreateNewChild(
//A		RUNTIME_CLASS(CChildFrame5), IDR_MCADTYPE, m_hMDIMenu, m_hMDIAccel);
//A ------------------------------------------------------------------------

	MINT				ist1;

	if ( MC::IeModel::MhGetModFIeMdl()) {
		ist1 = MessageBox( NULL, Mstr( "���������܂���"), Mstr( "�������m�F"), MB_OKCANCEL);
//		ist1 = MessageBox( NULL, Mstr( "�I�����܂���"), Mstr( "�I���m�F"), MB_OKCANCEL);
		if ( ist1 == IDCANCEL)
			return;
	}

	//�@�R�����\���͈�U����
	MC::MmWndInfo* pWndInfo = MC::WindowCtrl::MmWndKFindSyu( 2, MTHEIMENZU, 1, 1);
	if (pWndInfo) 
		pWndInfo->m_pChildFrm->SendMessage(WM_CLOSE);

	//	���f�������������\������
	ist1 = MC::IeModel::MhMdlLoad( NULL);
	MC::WindowCtrl::MmWndKReDrawAll();
}

void CMCADApp::CreateWnd(
				const	MINT		iWndSyu,		// (I  ) �E�B���h�E���
				const	MINT		iZuSyu,			// (I  ) �}���
				const	MINT		iKai,			// (I  ) �K
				const	MINT		iZuNum			// (I  ) �}�ԍ�
				) 
{
	CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);

	MC::WindowCtrl::MmWndSetSyubetsuC( iWndSyu, iZuSyu, iKai, iZuNum);

	// �V���� MDI �q�E�B���h�E���쐬���܂�
	switch (iWndSyu)
	{
	default:
		ASSERT(FALSE);
		break;

	case 1:
		pFrame->OnView1On();
		break;

	case 2:
		pFrame->OnView2On();
		break;

	case 3:
		pFrame->OnView3On();
		break;

	case 4:
		pFrame->OnView4On();
		break;

	case 5:
		pFrame->OnView5On();
//		OnFileNew();
		break;
	}
}

// �A�v���P�[�V�����̃o�[�W�������Ɏg���� CAboutDlg �_�C�A���O

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

// ����
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// �_�C�A���O�����s���邽�߂̃A�v���P�[�V���� �R�}���h
void CMCADApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CMCADApp ���b�Z�[�W �n���h��

void CMCADApp::OnView1On()
{
	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
	MC::MmWndInfo* pWndInfo = MC::WindowCtrl::MmWndKFindSyu( 1, MTHEIMENZU, 1, 1);		// �E�B���h�E�Ǘ�
	if (pWndInfo)
		pWndInfo->m_pChildFrm->SendMessage(WM_CLOSE);
	CreateWnd( 1, MTHEIMENZU, 1, 1);
}

void CMCADApp::OnView2On()
{
	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
	MC::MmWndInfo* pWndInfo = MC::WindowCtrl::MmWndKFindSyu( 2, MTHEIMENZU, 1, 1);		// �E�B���h�E�Ǘ�
	if (pWndInfo) {
//		pWndInfo->GetWnd()->SetWindowPos(&CWnd::wndTopMost , 0, 0, 0, 0,		// �O�ʂɕ\�����������Ƃ��������s��
//		SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE | SWP_NOZORDER);				// ���Ɉ�U�폜��ɍĕ\������
		pWndInfo->m_pChildFrm->SendMessage(WM_CLOSE);

	}
//	else {
		CreateWnd( 2, MTHEIMENZU, 1, 1);
//	}
#if( DEBUG001)
//=============================	from OnView1On()
//S	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
	MC::MmWndInfo* pWndInfo1 = MC::WindowCtrl::MmWndKFindSyu( 1, MTHEIMENZU, 1, 1);		// �E�B���h�E�Ǘ�
//	if (pWndInfo1)
//		pWndInfo1->m_pChildFrm->SendMessage(WM_CLOSE);
	CreateWnd( 1, MTHEIMENZU, 1, 1);
//-----------------------------
#endif
}

void CMCADApp::OnView3On()
{
	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
#if( 1 - DEBUG001)
	MC::MmWndInfo* pWndInfo = MC::WindowCtrl::MmWndKFindSyu( 3, MTHEIMENZU, 1, 1);		// �E�B���h�E�Ǘ�
	if (pWndInfo)
		pWndInfo->m_pChildFrm->SendMessage(WM_CLOSE);
#endif
	CreateWnd( 3, MTHEIMENZU, 1, 1);
}

void CMCADApp::OnView4On()
{
	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
#if( 1 - DEBUG001)
	MC::MmWndInfo* pWndInfo = MC::WindowCtrl::MmWndKFindSyu( 4, MTHEIMENZU, 1, 1);		// �E�B���h�E�Ǘ�
	if (pWndInfo)
		pWndInfo->m_pChildFrm->SendMessage(WM_CLOSE);
	CreateWnd( 4, MTHEIMENZU, 1, 1);
#endif
}

void CMCADApp::OnView5On()
{
	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
#if( 1 - DEBUG001)
	MC::MmWndInfo* pWndInfo = MC::WindowCtrl::MmWndKFindSyu( 5, MTHEIMENZU, 1, 1);		// �E�B���h�E�Ǘ�
	if (pWndInfo)
		pWndInfo->m_pChildFrm->SendMessage(WM_CLOSE);
#endif
	CreateWnd( 5, MTHEIMENZU, 1, 1);
}

void CMCADApp::OnView1Off()
{
	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
	MC::MmWndInfo* pWndInfo = MC::WindowCtrl::MmWndKFindSyu( 1, MTHEIMENZU, 1, 1);
	if (pWndInfo)
//		pWndInfo->m_pChildFrm->SendMessage(WM_CLOSE);
		pWndInfo->m_pChildFrm->PostMessage(WM_CLOSE);
}

void CMCADApp::OnView2Off()
{
	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
	MC::MmWndInfo* pWndInfo = MC::WindowCtrl::MmWndKFindSyu( 2, MTHEIMENZU, 1, 1);
	if (pWndInfo)
//		pWndInfo->m_pChildFrm->SendMessage(WM_CLOSE);
		pWndInfo->m_pChildFrm->PostMessage(WM_CLOSE);
}

void CMCADApp::OnView3Off()
{
	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
	MC::MmWndInfo* pWndInfo = MC::WindowCtrl::MmWndKFindSyu( 3, MTHEIMENZU, 1, 1);
	if (pWndInfo)
//		pWndInfo->m_pChildFrm->SendMessage(WM_CLOSE);
		pWndInfo->m_pChildFrm->PostMessage(WM_CLOSE);
}

void CMCADApp::OnView4Off()
{
	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
	MC::MmWndInfo* pWndInfo = MC::WindowCtrl::MmWndKFindSyu( 4, MTHEIMENZU, 1, 1);
	if (pWndInfo)
//		pWndInfo->m_pChildFrm->SendMessage(WM_CLOSE);
		pWndInfo->m_pChildFrm->PostMessage(WM_CLOSE);
}

void CMCADApp::OnView5Off()
{
	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
	MC::MmWndInfo* pWndInfo = MC::WindowCtrl::MmWndKFindSyu( 5, MTHEIMENZU, 1, 1);
	if (pWndInfo)
//		pWndInfo->m_pChildFrm->SendMessage(WM_CLOSE);
		pWndInfo->m_pChildFrm->PostMessage(WM_CLOSE);
}

void CMCADApp::OnMainMenuP( UINT nID)
{
	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
	// �R�}���h�L���[
	if ( nID == IDC_DISP_3D) {
		OnView2On();
	} else {
		if ( nID == IDC_ZOOMINIT) {
			MC::WindowCtrl::MmGridNumSet( NULL);								// �����ݒ�
			MC::WindowCtrl::MmGridNumXqt();
		}
		MC::WindowCtrl::MmWndKCmdXqt( nID);										//	���j���[�R�}���h
	}
}

void CMCADApp::OnUpdateMainMenuP( CCmdUI *pCmdUI)
{
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
	if (pCmdUI->m_nID == IDC_UNDO) {
		pCmdUI->Enable( MC::HaitiCmd::MmUndoIsNotEmpty());
	} else if (pCmdUI->m_nID == IDC_REDO) {
		pCmdUI->Enable( MC::HaitiCmd::MmRedoIsNotEmpty());
	}
}

void CMCADApp::OnSubMenuP( UINT nID)
{
	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
	// �R�}���h�L���[
	MC::WindowCtrl::MmWndKCmdXqt( nID);										//	���j���[�R�}���h

}

void CMCADApp::OnUpdateSubMenuP( CCmdUI *pCmdUI)
{
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
}

/*
// The code fragment below shows how to use ON_COMMAND_RANGE macro
// to map a contiguous range of command IDs to a single message 
// handler function (i.e. OnFileMenuItems() is the sample below). In 
// addition, it also shows how to use CheckMenuRadioItem() to check a 
// selected menu item and makes it a radio item.
BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
   // ... 
   ON_COMMAND_RANGE(ID_FILE_MENUITEM1, ID_FILE_MENUITEM3, OnFileMenuItems)
END_MESSAGE_MAP()

void CMainFrame::OnFileMenuItems(UINT nID)
{
   CMenu* mmenu = GetMenu();
   CMenu* submenu = mmenu->GetSubMenu(0);
   submenu->CheckMenuRadioItem(ID_FILE_MENUITEM1, ID_FILE_MENUITEM3, 
      nID, MF_BYCOMMAND);
}
*/

void CMCADApp::OnBnClickedRadioYane()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	MC::InpMode::SetKGp( MP_GP_YANE);
	MC::InpAttr::SetComboPts();
//	�R�}���h�L�����Z��
	MC::WindowCtrl::MmWndKCmdXqt( IDC_CANCELCMD);							//	�R�}���h�L�����Z��
}

void CMCADApp::OnUpdateRadioYane(CCmdUI *pCmdUI)
{
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
	pCmdUI->SetCheck( MC::InpMode::GetKGp() == MP_GP_YANE);
}

void CMCADApp::OnBnClickedRadioTenjo()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	MC::InpMode::SetKGp( MP_GP_TENJO);
	MC::InpAttr::SetComboPts();
//	�R�}���h�L�����Z��
	MC::WindowCtrl::MmWndKCmdXqt( IDC_CANCELCMD);							//	�R�}���h�L�����Z��
}

void CMCADApp::OnUpdateRadioTenjo(CCmdUI *pCmdUI)
{
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
	pCmdUI->SetCheck( MC::InpMode::GetKGp() == MP_GP_TENJO);
}

void CMCADApp::OnBnClickedRadioTairyoku()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	MC::InpMode::SetKGp( MP_GP_TAIRYOKU);
	MC::InpAttr::SetComboPts();
//	�R�}���h�L�����Z��
	MC::WindowCtrl::MmWndKCmdXqt( IDC_CANCELCMD);							//	�R�}���h�L�����Z��
}

void CMCADApp::OnUpdateRadioTairyoku(CCmdUI *pCmdUI)
{
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
	pCmdUI->SetCheck( MC::InpMode::GetKGp() == MP_GP_TAIRYOKU);
}

void CMCADApp::OnBnClickedRadioKabe()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	MC::InpMode::SetKGp( MP_GP_KABE);
	MC::InpAttr::SetComboPts();
//	�R�}���h�L�����Z��
	MC::WindowCtrl::MmWndKCmdXqt( IDC_CANCELCMD);							//	�R�}���h�L�����Z��
}

void CMCADApp::OnUpdateRadioKabe(CCmdUI *pCmdUI)
{
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
	pCmdUI->SetCheck( MC::InpMode::GetKGp() == MP_GP_KABE);
}

void CMCADApp::OnBnClickedRadioYuka()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	MC::InpMode::SetKGp( MP_GP_YUKA);
	MC::InpAttr::SetComboPts();
//	�R�}���h�L�����Z��
	MC::WindowCtrl::MmWndKCmdXqt( IDC_CANCELCMD);							//	�R�}���h�L�����Z��
}

void CMCADApp::OnUpdateRadioYuka(CCmdUI *pCmdUI)
{
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
	pCmdUI->SetCheck( MC::InpMode::GetKGp() == MP_GP_YUKA);
}

void CMCADApp::OnBnClickedRadioKoya()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	MC::InpMode::SetKGp( MP_GP_KOYA);
	MC::InpAttr::SetComboPts();
//	�R�}���h�L�����Z��
	MC::WindowCtrl::MmWndKCmdXqt( IDC_CANCELCMD);							//	�R�}���h�L�����Z��
}

void CMCADApp::OnUpdateRadioKoya(CCmdUI *pCmdUI)
{
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
	pCmdUI->SetCheck( MC::InpMode::GetKGp() == MP_GP_KOYA);
}

void CMCADApp::OnBnClickedRadioKiso()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	MC::InpMode::SetKGp( MP_GP_KISO);
	MC::InpAttr::SetComboPts();
//	�R�}���h�L�����Z��
	MC::WindowCtrl::MmWndKCmdXqt( IDC_CANCELCMD);							//	�R�}���h�L�����Z��
}

void CMCADApp::OnUpdateRadioDodai(CCmdUI *pCmdUI)
{
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
	pCmdUI->SetCheck( MC::InpMode::GetKGp() == MP_GP_DODAI);
}

void CMCADApp::OnBnClickedRadioDodai()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	MC::InpMode::SetKGp( MP_GP_DODAI);
	MC::InpAttr::SetComboPts();
//	�R�}���h�L�����Z��
	MC::WindowCtrl::MmWndKCmdXqt( IDC_CANCELCMD);							//	�R�}���h�L�����Z��
}

void CMCADApp::OnUpdateRadioKiso(CCmdUI *pCmdUI)
{
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
	pCmdUI->SetCheck( MC::InpMode::GetKGp() == MP_GP_KISO);
}

void CMCADApp::OnBnClickedRadioPts()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	MC::InpMode::SetKBr( MP_BR_BUZAI);
	MC::InpAttr::SetComboPts();
//	�R�}���h�L�����Z��
	MC::WindowCtrl::MmWndKCmdXqt( IDC_CANCELCMD);							//	�R�}���h�L�����Z��
}

void CMCADApp::OnUpdateRadioPts(CCmdUI *pCmdUI)
{
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
	pCmdUI->SetCheck( MC::InpMode::GetKBr() == MP_BR_BUZAI);
//	pCmdUI->Enable( m_bKInpBuzai);
}

void CMCADApp::OnBnClickedRadioKanagu()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	MC::InpMode::SetKBr( MP_BR_KANAGU);
	MC::InpAttr::SetComboPts();
//	�R�}���h�L�����Z��
	MC::WindowCtrl::MmWndKCmdXqt( IDC_CANCELCMD);							//	�R�}���h�L�����Z��
}

void CMCADApp::OnUpdateRadioKanagu(CCmdUI *pCmdUI)
{
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
	pCmdUI->SetCheck( MC::InpMode::GetKBr() == MP_BR_KANAGU);
//	pCmdUI->Enable( m_bKInpKanagu);
}

void CMCADApp::OnBnClickedRadioPanel()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	MC::InpMode::SetKBr( MP_BR_PANEL);
	MC::InpAttr::SetComboPts();
//	�R�}���h�L�����Z��
	MC::WindowCtrl::MmWndKCmdXqt( IDC_CANCELCMD);							//	�R�}���h�L�����Z��
}

void CMCADApp::OnUpdateRadioPanel(CCmdUI *pCmdUI)
{
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
	pCmdUI->SetCheck( MC::InpMode::GetKBr() == MP_BR_PANEL);
//	pCmdUI->Enable( m_bKInpPanel);
}

void CMCADApp::OnBnClickedRadioOther()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	MC::InpMode::SetKBr( MP_BR_OTHER);
	MC::InpAttr::SetComboPts();
//	�R�}���h�L�����Z��
	MC::WindowCtrl::MmWndKCmdXqt( IDC_CANCELCMD);							//	�R�}���h�L�����Z��
}

void CMCADApp::OnUpdateRadioOther(CCmdUI *pCmdUI)
{
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
	pCmdUI->SetCheck( MC::InpMode::GetKBr() == MP_BR_OTHER);
//	pCmdUI->Enable( m_bKInpOther);
}

void CMCADApp::OnBnClickedRadioSel1()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	m_iKInpSel = MP_SEL1;
	MC::Window::CurWndFocus();
}

void CMCADApp::OnUpdateRadioSel1(CCmdUI *pCmdUI)
{
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
	pCmdUI->SetCheck( m_iKInpSel == MP_SEL1);
}

void CMCADApp::OnBnClickedRadioSel2()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	m_iKInpSel = MP_SEL2;
	MC::Window::CurWndFocus();
}

void CMCADApp::OnUpdateRadioSel2(CCmdUI *pCmdUI)
{
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
	pCmdUI->SetCheck( m_iKInpSel == MP_SEL2);
}

void CMCADApp::OnBnClickedRadioSel3()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	m_iKInpSel = MP_SEL3;
	MC::Window::CurWndFocus();
}

void CMCADApp::OnUpdateRadioSel3(CCmdUI *pCmdUI)
{
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
	pCmdUI->SetCheck( m_iKInpSel == MP_SEL3);
}

void CMCADApp::OnBnClickedChukiDelete()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	MC::Window::CurWndFocus();
}

void CMCADApp::OnUpdateChukiDelete(CCmdUI *pCmdUI)
{
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
}

void CMCADApp::OnBnClickedChukiMove()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	MC::Window::CurWndFocus();
}

void CMCADApp::OnUpdateChukiMove(CCmdUI *pCmdUI)
{
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
}

void CMCADApp::OnBnClickedChukiChange()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	MC::Window::CurWndFocus();
}

void CMCADApp::OnUpdateChukiChange(CCmdUI *pCmdUI)
{
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
}

void CMCADApp::OnBnClickedCheckBuzai()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	MC::Window::CurWndFocus();

	MCHAR sTitle[] =Mstr( "�e�X�g");
	CButton* pCheckBuzai = (CButton*)(MC::System::GetpMainFrame()->m_wndDlgBar2.GetDlgItem(IDC_CHECKBUZAI));
	pCheckBuzai->SetWindowText( sTitle);
	MINT i1 = pCheckBuzai->GetCheck();
	pCheckBuzai->ShowWindow( 0);
}

void CMCADApp::OnUpdateCheckBuzai(CCmdUI *pCmdUI)
{
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
}

void CMCADApp::OnBnClickedCheckKanagu()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	MC::Window::CurWndFocus();
}

void CMCADApp::OnUpdateCheckKanagu(CCmdUI *pCmdUI)
{
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
}

void CMCADApp::OnBnClickedCheckGohan()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	MC::Window::CurWndFocus();
}

void CMCADApp::OnUpdateCheckGohan(CCmdUI *pCmdUI)
{
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
}

void CMCADApp::OnBnClickedCheckAttr1()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	MC::Window::CurWndFocus();
}

void CMCADApp::OnUpdateCheckAttr1(CCmdUI *pCmdUI)
{
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
}

void CMCADApp::OnBnClickedCheckAttr2()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	MC::Window::CurWndFocus();
}

void CMCADApp::OnUpdateCheckAttr2(CCmdUI *pCmdUI)
{
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
}

void CMCADApp::OnBnClickedCheckAttr3()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	MC::Window::CurWndFocus();
}

void CMCADApp::OnUpdateCheckAttr3(CCmdUI *pCmdUI)
{
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
}

void CMCADApp::OnBnClickedCheckAttr4()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	MC::Window::CurWndFocus();
}

void CMCADApp::OnUpdateCheckAttr4(CCmdUI *pCmdUI)
{
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
}

void CMCADApp::OnBnClickedRadio1f()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	MC::InpMode::SetKai( MP_KAI1);										// �P�K
	MC::MhPlcInp::SetKai( MC::InpMode::GetKai());
	MC::Window::CurWndFocus();
	MC::WindowCtrl::MmWndKReDraw();
}

void CMCADApp::OnUpdateRadio1f(CCmdUI *pCmdUI)
{
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
	pCmdUI->SetCheck( MC::InpMode::GetKai() == MP_KAI1);
}

void CMCADApp::OnBnClickedRadio2f()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	MC::InpMode::SetKai( MP_KAI2);										// �Q�K
	MC::MhPlcInp::SetKai( MC::InpMode::GetKai());
	MC::Window::CurWndFocus();
	MC::WindowCtrl::MmWndKReDraw();
}

void CMCADApp::OnUpdateRadio2f(CCmdUI *pCmdUI)
{
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
	pCmdUI->SetCheck( MC::InpMode::GetKai() == MP_KAI2);
}

void CMCADApp::OnBnClickedRadio3f()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	MC::InpMode::SetKai( MP_KAI3);										// �R�K
	MC::MhPlcInp::SetKai( MC::InpMode::GetKai());
	MC::Window::CurWndFocus();
	MC::WindowCtrl::MmWndKReDraw();
}

void CMCADApp::OnUpdateRadio3f(CCmdUI *pCmdUI)
{
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
	pCmdUI->SetCheck( MC::InpMode::GetKai() == MP_KAI3);
}

void CMCADApp::OnBnClickedPanelBefore()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	MC::Window::CurWndFocus();
}

void CMCADApp::OnUpdatePanelBefore(CCmdUI *pCmdUI)
{
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
}

void CMCADApp::OnBnClickedPanelNext()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	MC::Window::CurWndFocus();
}

void CMCADApp::OnUpdatePanelNext(CCmdUI *pCmdUI)
{
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
}

void CMCADApp::OnBnClickedPanelAll()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	MC::Window::CurWndFocus();
}

void CMCADApp::OnUpdatePanelAll(CCmdUI *pCmdUI)
{
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
}

void CMCADApp::OnFileOpen()
{
	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
//	MINT		Ist1;
	MCHAR		PathI[256];
	MCHAR		PathO[256];
//	CWnd*		ViewId;															// �ޭ��̲ݽ�ݽ
	MINT		ist1;

//		Mstrcpy( TablePath, MC::MmSystemProperty::GetStr( MM_STR_MODEL_DIR));
//		Mstrcat( TablePath, _T("�\��.csv"));
//
//	Ist1 = MsGetRegistryPath( PathI);													// ڼ޽�؂���߽���擾
	Mstrcpy_s( PathI, Mstr("C:\\MCAD\\�}�X�^\\�f�[�^\\"));
	ist1 = MC::IeModel::MhFileDialog( true, PathI, PathO);								// ̧���޲�۸ނ̕\��

	if ( ist1 == 1) {																	// OK�̏ꍇ�ǂݍ���
		//�@�R�����\���͈�U����
		MC::MmWndInfo* pWndInfo = MC::WindowCtrl::MmWndKFindSyu( 2, MTHEIMENZU, 1, 1);
		if (pWndInfo) 
			pWndInfo->m_pChildFrm->SendMessage(WM_CLOSE);

		//	���f����ǂݍ��ݕ\������
		ist1 = MC::IeModel::MhMdlLoad( PathO);
		MC::WindowCtrl::MmWndKReDrawAll();

		//�@�R�����\��������Ă����ꍇ�͐V�����f�[�^�ōēx�\������
		if (pWndInfo) 
			OnView2On();
	}
}

void CMCADApp::OnUpdateFileOpen(CCmdUI *pCmdUI)
{
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
}

void CMCADApp::OnFileSave()
{
	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
//	MCHAR				PathI[256];
	MCHAR				PathO[256];
	MINT				ist1;

//	Ist1 = HeGetRegistryPath( PathI);													// ڼ޽�؂���߽���擾
	ist1 = MC::IeModel::MhFileDialog( false, NULL, PathO);								// ̧���޲�۸ނ̕\��

	if ( ist1 == 1)																		// OK�̏ꍇ�ۑ�
		ist1 = MC::IeModel::MhMdlSave( PathO);
}

void CMCADApp::OnUpdateFileSave(CCmdUI *pCmdUI)
{
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
}

void CMCADApp::OnFileSaveAs()
{
	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
	OnFileSave();
}

void CMCADApp::OnUpdateFileSaveAs(CCmdUI *pCmdUI)
{
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
}

void CMCADApp::OnFilePrint()
{
	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
    int i;
    HPEN hPen;
    PRINTDLG pd;
    DOCINFO di;
    TEXTMETRIC tm;
    MCHAR* szStrOrg = Mstr( "This is a test print--Line(%02d)");
    MCHAR szStr[128];
    MCHAR* szLine = Mstr( "******************************");

    memset(&pd, 0, sizeof(PRINTDLG));
    pd.lStructSize = sizeof(PRINTDLG);
//    pd.hwndOwner = hWnd;
    pd.hwndOwner = NULL;
    pd.hDevMode = NULL;
    pd.hDevNames = NULL;
    pd.Flags = PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC |
        PD_ALLPAGES | PD_NOSELECTION | PD_HIDEPRINTTOFILE;
    pd.nCopies = 1;
    pd.nFromPage = 1;
    pd.nToPage = 1;
    pd.nMinPage = 1;
    pd.nMaxPage = 1;
    memset(&di, 0, sizeof(DOCINFO));
    di.cbSize = sizeof(DOCINFO);
    di.lpszDocName = Mstr( "TEST PRINT");
                    
    if (PrintDlg(&pd)==TRUE) {
        StartDoc(pd.hDC, &di);
        StartPage(pd.hDC);
        GetTextMetrics(pd.hDC, &tm);
        for (i = 1; i <= 20; i += 2) {
            wsprintf(szStr, szStrOrg, i);
            TextOut(pd.hDC, 100, tm.tmHeight * i, szStr, (int)Mstrlen(szStr));
            TextOut(pd.hDC, 100, tm.tmHeight * (i + 1), szLine, (int)Mstrlen(szLine));
        }
        hPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
        SelectObject(pd.hDC, hPen);
        MoveToEx(pd.hDC, 100, tm.tmHeight, NULL);
        LineTo(pd.hDC, tm.tmAveCharWidth * (int)Mstrlen(szLine) + 100, tm.tmHeight);
        LineTo(pd.hDC, tm.tmAveCharWidth * (int)Mstrlen(szLine) + 100, tm.tmHeight * 21);
        LineTo(pd.hDC, 100, tm.tmHeight * 21);
        LineTo(pd.hDC, 100, tm.tmHeight);
        EndPage(pd.hDC);
        EndDoc(pd.hDC);
        DeleteObject(hPen);
        DeleteDC(pd.hDC);
    }
//    return TRUE;
}

void CMCADApp::OnUpdateFilePrint(CCmdUI *pCmdUI)
{
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
}

void CMCADApp::OnFilePrintPreview()
{
	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
}

void CMCADApp::OnUpdateFilePrintPreview(CCmdUI *pCmdUI)
{
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
}

void CMCADApp::OnFilePrintSetup()
{
	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
	HWND hWnd = NULL;
    PAGESETUPDLG psd;
    MCHAR szPrnName[256];
    HGLOBAL hMem;
    DEVMODE *pdev = NULL;
    PRINTER_DEFAULTS pdef;
    HANDLE hPrn;
    DWORD dwNeeded, dwSize;
    PRINTER_INFO_2 *pprn_info;

    memset(&psd, 0, sizeof(PAGESETUPDLG));
    psd.lStructSize = sizeof(PAGESETUPDLG);
    psd.hwndOwner = hWnd;
    psd.Flags = PSD_DISABLEMARGINS;

    if (PageSetupDlg(&psd)) {
        //�f�o�C�X���̎擾
        pdev = (DEVMODE *)GlobalLock(psd.hDevMode);
        if (pdev == NULL) {
            MessageBox(hWnd, Mstr( "GlobalLock Error"), Mstr( "Error"), MB_OK);
            return;
        }
        Mstrcpy(szPrnName, (MCHAR *)(pdev->dmDeviceName));

        //PRINTER_ALL_ACCESS���Ńv�����^�I�[�v��
        memset(&pdef, 0, sizeof(PRINTER_DEFAULTS));
        pdef.DesiredAccess = PRINTER_ALL_ACCESS;
        if (OpenPrinter(szPrnName, &hPrn, &pdef) == 0)
            MessageBox(hWnd, Mstr( "OpenPrinter Error"), Mstr( "Error"), MB_OK);

        //�P��ڂ͂킴�ƂɎ��s���ĕK�v�o�C�g�����擾
        GetPrinter(hPrn, 2, 0, 0, &dwNeeded);
        dwSize = dwNeeded;
        hMem = GlobalAlloc(GHND, dwSize);
        if (hMem == NULL) {
            MessageBox(hWnd, Mstr( "GlobalAlloc Error"), Mstr( "Error"), MB_OK);
            ClosePrinter(hPrn);
            return;
        }
        pprn_info = (PRINTER_INFO_2 *)GlobalLock(hMem);
        if (pprn_info == NULL) {
            MessageBox(hWnd, Mstr( "GlobalLock Error"), Mstr( "Error"), MB_OK);
            GlobalFree(hMem);
            ClosePrinter(hPrn);
            return;
        }

        //PRINTER_INFO_2�\���̂Ƀv�����^�����擾
        if (GetPrinter(hPrn, 2, (LPBYTE)pprn_info, dwSize, &dwNeeded) == 0) {
            MessageBox(hWnd, Mstr( "GetPrinter Error"), Mstr( "Error"), MB_OK);
            GlobalUnlock(hMem);
            GlobalFree(hMem);
            ClosePrinter(hPrn);
            return;
        }
        //���݂̃v�����^���̂������[�U�[���͂��ꂽ���̂ɕύX
        pprn_info->pDevMode->dmFields = DM_ORIENTATION | DM_PAPERSIZE |
            DM_PAPERLENGTH | DM_PAPERWIDTH | DM_DEFAULTSOURCE |
            DM_PRINTQUALITY | DM_COLOR | DM_DUPLEX;
        pprn_info->pDevMode->dmOrientation = pdev->dmOrientation;
        pprn_info->pDevMode->dmPaperSize = pdev->dmPaperSize;
        pprn_info->pDevMode->dmPaperLength = pdev->dmPaperLength;
        pprn_info->pDevMode->dmPaperWidth = pdev->dmPaperWidth;
        pprn_info->pDevMode->dmDefaultSource = pdev->dmDefaultSource;
        pprn_info->pDevMode->dmPrintQuality = pdev->dmPrintQuality;
        pprn_info->pDevMode->dmColor = pdev->dmColor;

        //�f�o�C�X�ˑ������̃A�b�v�f�[�g
        if (DocumentProperties(hWnd,
                hPrn,
                szPrnName,
                (PDEVMODE)pprn_info->pDevMode,
                (PDEVMODE)pprn_info->pDevMode,
                DM_IN_BUFFER | DM_OUT_BUFFER) < 0) {
                MessageBox(hWnd, Mstr( "DocumentProperties Error"), Mstr( "Error"), MB_OK);
                GlobalUnlock(hMem);
                GlobalFree(hMem);
                ClosePrinter(hPrn);
                return;
        }
        //�v�����^���̃Z�b�g
        if (SetPrinter(hPrn, 2, (LPBYTE)pprn_info, 0) == 0) {
            MessageBox(hWnd, Mstr( "SetPrinter Error"), Mstr( "Error"), MB_OK);
            return;
        }
        if (GlobalUnlock(hMem) == 0) {
            if (GetLastError() != NO_ERROR) {
                MessageBox(hWnd, Mstr( "GlobalUnlock Error"), Mstr( "Error"), MB_OK);
                return;
            }
        }
        
        if (GlobalFree(hMem) != NULL)
            MessageBox(hWnd, Mstr( "GlobalFree Error"), Mstr( "Error"), MB_OK);

        GlobalUnlock(psd.hDevMode);
        if (GlobalFree(psd.hDevMode) != NULL)
            MessageBox(hWnd, Mstr( "Error"), Mstr( "Error"), MB_OK);
        ClosePrinter(hPrn);
        return;
    }
    return;
}

void CMCADApp::OnUpdateFilePrintSetup(CCmdUI *pCmdUI)
{
	// TODO : �����ɃR�}���h�X�V UI �n���h�� �R�[�h��ǉ����܂��B
}

void CMCADApp::OnAppExit()
{
	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
	MC::System::GetpMainFrame()->SendMessage( WM_CLOSE);
}

void CMCADApp::OnIsyou()
{
	MC::System::GetpMainFrame()->m_iSentaku = MP_SENTAKU_ISYOU;
	MC::System::GetpMainFrame()->ShowControlBar( &(MC::System::GetpMainFrame()->m_wndDlgBar3), FALSE, FALSE);
	MC::System::GetpMainFrame()->ShowControlBar( &(MC::System::GetpMainFrame()->m_wndDlgBar6), TRUE, FALSE);
}

void CMCADApp::OnUpdateIsyou(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck( MC::System::GetpMainFrame()->m_wndDlgBar6.IsWindowVisible());
}

void CMCADApp::OnKouzou()
{
	MC::System::GetpMainFrame()->m_iSentaku = MP_SENTAKU_KOUZOU;
	MC::System::GetpMainFrame()->ShowControlBar( &(MC::System::GetpMainFrame()->m_wndDlgBar3), TRUE, FALSE);
	MC::System::GetpMainFrame()->ShowControlBar( &(MC::System::GetpMainFrame()->m_wndDlgBar6), FALSE, FALSE);
}

void CMCADApp::OnUpdateKouzou(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck( MC::System::GetpMainFrame()->m_wndDlgBar3.IsWindowVisible());
}

void CMCADApp::OnBnClickedRadioJyuuko()
{
	MC::InpMode::SetSGp( MP_GP_JYUKO);
}

void CMCADApp::OnUpdateRadioJyuuko(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck( MC::InpMode::GetSGp() == MP_GP_JYUKO);
}

void CMCADApp::OnBnClickedRadioJyuuto()
{
	MC::InpMode::SetSGp( MP_GP_JYUTO);
}

void CMCADApp::OnUpdateRadioJyuuto(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck( MC::InpMode::GetSGp() == MP_GP_JYUTO);
}

void CMCADApp::OnBnClickedRadioBukken()
{
	MC::InpMode::SetSGp( MP_GP_BUKKEN);
}

void CMCADApp::OnUpdateRadioBukken(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck( MC::InpMode::GetSGp() == MP_GP_BUKKEN);
}

void CMCADApp::OnBnClickedRadioSYane()
{
	MC::InpMode::SetSGp( MP_GP_S_YANE);
}

void CMCADApp::OnUpdateRadioSYane(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck( MC::InpMode::GetSGp() == MP_GP_S_YANE);
}

void CMCADApp::OnBnClickedRadioSikiti()
{
	MC::InpMode::SetSGp( MP_GP_SIKITI);
}

void CMCADApp::OnUpdateRadioSikiti(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck( MC::InpMode::GetSGp() == MP_GP_SIKITI);
}

void CMCADApp::OnBnClickedRadioHekiryou()
{
	MC::InpMode::SetSGp( MP_GP_HEKIRYOU);
}

void CMCADApp::OnUpdateRadioHekiryou(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck( MC::InpMode::GetSGp() == MP_GP_HEKIRYOU);
}


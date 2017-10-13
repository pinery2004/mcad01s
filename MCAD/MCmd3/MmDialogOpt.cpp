// MmDialogOpt.cpp : 実装ファイル
//

#include "stdafx.h"
#include "MCAD.h"
#include "MmOpt.h"
#include "MmWnd.h"
#include "resource.h"

#include "MmDialogOpt.h"

namespace MC
{

void MCmdOptEnd();

} // namespace MC

// CMmDialogOpt ダイアログ

IMPLEMENT_DYNAMIC(CMmDialogOpt, CDialog)

CMmDialogOpt::CMmDialogOpt(CWnd* pParent /*=NULL*/)
	: CDialog(CMmDialogOpt::IDD, pParent)
{
	m_pParent = NULL;
}

CMmDialogOpt::~CMmDialogOpt()
{
}

BOOL CMmDialogOpt::Create(CWnd * pWnd)
{
	m_pParent = pWnd;
	BOOL bret = CDialog::Create( CMmDialogOpt::IDD, m_pParent);

	if (bret == TRUE)
	{
		this->ShowWindow( SW_SHOW);
	}
	return bret;
}

BOOL CMmDialogOpt::OnInitDialog()
{
	MC::MmWndInfo*			pWndInfo;

	CDialog::OnInitDialog();

	CFont	myfont1;
	myfont1.CreateFont( 18, 0, 0, 0, 900,  0, 0, 0, SHIFTJIS_CHARSET,
						OUT_DEVICE_PRECIS, CLIP_DEFAULT_PRECIS,
						DEFAULT_QUALITY, DEFAULT_PITCH, Mstr( "ＭＳ Ｐゴシック"));

	pWndInfo = MC::WindowCtrl::MmWndKGetCurWnd();							// カレントウィンドウ取得
	if (pWndInfo == NULL) MQUIT;

exit:
	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CMmDialogOpt::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECKOPTION0, m_chkBox0);
	DDX_Control(pDX, IDC_CHECKOPTION1, m_chkBox1);
	DDX_Control(pDX, IDC_CHECKOPTION2, m_chkBox2);
	DDX_Control(pDX, IDC_CHECKOPTION3, m_chkBox3);
	DDX_Control(pDX, IDC_CHECKOPTION4, m_chkBox4);
	DDX_Control(pDX, IDC_CHECKOPTION5, m_chkBox5);
	DDX_Control(pDX, IDC_CHECKOPTION6, m_chkBox6);
	DDX_Control(pDX, IDC_CHECKOPTION7, m_chkBox7);
	DDX_Control(pDX, IDC_CHECKOPTION8, m_chkBox8);
	DDX_Control(pDX, IDC_CHECKOPTION9, m_chkBox9);
	DDX_Control(pDX, IDC_CHECKOPTION10, m_chkBox10);
}

BEGIN_MESSAGE_MAP(CMmDialogOpt, CDialog)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDCANCEL, &CMmDialogOpt::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECKOPTION0, &CMmDialogOpt::OnBnClickedCheckoption0)
	ON_BN_CLICKED(IDC_CHECKOPTION1, &CMmDialogOpt::OnBnClickedCheckoption1)
	ON_BN_CLICKED(IDC_CHECKOPTION2, &CMmDialogOpt::OnBnClickedCheckoption2)
	ON_BN_CLICKED(IDC_CHECKOPTION3, &CMmDialogOpt::OnBnClickedCheckoption3)
	ON_BN_CLICKED(IDC_CHECKOPTION4, &CMmDialogOpt::OnBnClickedCheckoption4)
	ON_BN_CLICKED(IDC_CHECKOPTION5, &CMmDialogOpt::OnBnClickedCheckoption5)
	ON_BN_CLICKED(IDC_CHECKOPTION6, &CMmDialogOpt::OnBnClickedCheckoption6)
	ON_BN_CLICKED(IDC_CHECKOPTION7, &CMmDialogOpt::OnBnClickedCheckoption7)
	ON_BN_CLICKED(IDC_CHECKOPTION8, &CMmDialogOpt::OnBnClickedCheckoption8)
	ON_BN_CLICKED(IDC_CHECKOPTION9, &CMmDialogOpt::OnBnClickedCheckoption9)
	ON_BN_CLICKED(IDC_CHECKOPTION10, &CMmDialogOpt::OnBnClickedCheckoption10)
END_MESSAGE_MAP()

// CMmDialogOpt メッセージ ハンドラ

void CMmDialogOpt::PostNcDestroy()
{
	if (m_pParent != NULL) {
		MC::MCmdOptEnd();
		delete this;
	}

	CDialog::PostNcDestroy();
}

void CMmDialogOpt::OnClose()
{
	if (m_pParent != NULL) {
		DestroyWindow();
	} else {
		CDialog::OnClose();
	}
}

void CMmDialogOpt::OnBnClickedCancel()
{
	if (m_pParent != NULL) {
		DestroyWindow();
	} else {
		OnCancel();
	}
}

void CMmDialogOpt::OnBnClickedCheckoption0()
{
	MC::MhOptV::MmOptSet( 0, m_chkBox0.GetCheck());
}

void CMmDialogOpt::OnBnClickedCheckoption1()
{
	MC::MhOptV::MmOptSet( 1, m_chkBox1.GetCheck());
}

void CMmDialogOpt::OnBnClickedCheckoption2()
{
	MC::MhOptV::MmOptSet( 2, m_chkBox2.GetCheck());
}

void CMmDialogOpt::OnBnClickedCheckoption3()
{
	MC::MhOptV::MmOptSet( 3, m_chkBox3.GetCheck());
}

void CMmDialogOpt::OnBnClickedCheckoption4()
{
	MC::MhOptV::MmOptSet( 4, m_chkBox4.GetCheck());
}

void CMmDialogOpt::OnBnClickedCheckoption5()
{
	MC::MhOptV::MmOptSet( 5, m_chkBox5.GetCheck());
}

void CMmDialogOpt::OnBnClickedCheckoption6()
{
	MC::MhOptV::MmOptSet( 6, m_chkBox6.GetCheck());
}

void CMmDialogOpt::OnBnClickedCheckoption7()
{
	MC::MhOptV::MmOptSet( 7, m_chkBox7.GetCheck());
}

void CMmDialogOpt::OnBnClickedCheckoption8()
{
	MC::MhOptV::MmOptSet( 8, m_chkBox8.GetCheck());
}

void CMmDialogOpt::OnBnClickedCheckoption9()
{
	MC::MhOptV::MmOptSet( 9, m_chkBox9.GetCheck());
}

void CMmDialogOpt::OnBnClickedCheckoption10()
{
	MC::MhOptV::MmOptSet( 10, m_chkBox10.GetCheck());
}

BOOL CMmDialogOpt::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_RETURN) {
			return TRUE;
		}
//		if (pMsg->wParam == VK_ESCAPE) {
//			SetControl();
//			return TRUE;
//		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

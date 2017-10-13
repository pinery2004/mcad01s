// MmDialogTekai.cpp : 実装ファイル
//

#include "stdafx.h"
#include "MCAD.h"
#include "resource.h"
#include "MmTenkai.h"
#include "MmDialogTenkai.h"


// CMmDialogTenkai ダイアログ

IMPLEMENT_DYNAMIC(CMmDialogTenkai, CDialog)

CMmDialogTenkai::CMmDialogTenkai(CWnd* pParent /*=NULL*/)
	: CDialog(CMmDialogTenkai::IDD, pParent)
	, m_pParent(NULL)
	, m_iSelJuko(0)
{
}

CMmDialogTenkai::~CMmDialogTenkai()
{
}

BOOL CMmDialogTenkai::Create(CWnd* pWnd)
{
	m_pParent = pWnd;
	BOOL bret = CDialog::Create( CMmDialogTenkai::IDD, m_pParent);
	if (bret == TRUE)
		this->ShowWindow( SW_SHOW);
	return bret;
}

void CMmDialogTenkai::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBONUM, m_comboJuko);
	DDX_Control(pDX, IDC_EDT_JK1, m_NmJuko1);
	DDX_Control(pDX, IDC_EDT_JK2, m_NmJuko2);
	DDX_Control(pDX, IDC_EDT_JK3, m_NmJuko3);
	DDX_Control(pDX, IDC_EDT_JK4, m_NmJuko4);
	DDX_Control(pDX, IDC_EDT_JK5, m_NmJuko5);
	DDX_Control(pDX, IDC_EDT_JK6, m_NmJuko6);
	DDX_Control(pDX, IDC_EDT_JK7, m_NmJuko7);
	DDX_Control(pDX, IDC_EDT_JK8, m_NmJuko8);
	DDX_Control(pDX, IDC_CHECKH1, m_H1);
	DDX_Control(pDX, IDC_CHECKH2, m_H2);
	DDX_Control(pDX, IDC_CHECKH3, m_H3);
	DDX_Control(pDX, IDC_CHECKH4, m_H4);
	DDX_Control(pDX, IDC_CHECKH5, m_H5);
	DDX_Control(pDX, IDC_CHECKH6, m_H6);
	DDX_Control(pDX, IDC_CHECKH7, m_H7);
	DDX_Control(pDX, IDC_CHECKH8, m_H8);
	DDX_Control(pDX, IDC_EDT_MD1, m_MDJuko1);
	DDX_Control(pDX, IDC_EDT_MD2, m_MDJuko2);
	DDX_Control(pDX, IDC_EDT_MD3, m_MDJuko3);
	DDX_Control(pDX, IDC_EDT_MD4, m_MDJuko4);
	DDX_Control(pDX, IDC_EDT_MD5, m_MDJuko5);
	DDX_Control(pDX, IDC_EDT_MD6, m_MDJuko6);
	DDX_Control(pDX, IDC_EDT_MD7, m_MDJuko7);
	DDX_Control(pDX, IDC_EDT_MD8, m_MDJuko8);
	DDX_Control(pDX, IDC_EDT_W1, m_WJuko1);
	DDX_Control(pDX, IDC_EDT_W2, m_WJuko2);
	DDX_Control(pDX, IDC_EDT_W3, m_WJuko3);
	DDX_Control(pDX, IDC_EDT_W4, m_WJuko4);
	DDX_Control(pDX, IDC_EDT_W5, m_WJuko5);
	DDX_Control(pDX, IDC_EDT_W6, m_WJuko6);
	DDX_Control(pDX, IDC_EDT_W7, m_WJuko7);
	DDX_Control(pDX, IDC_EDT_W8, m_WJuko8);
	DDX_Control(pDX, IDC_EDT_D1, m_DJuko1);
	DDX_Control(pDX, IDC_EDT_D2, m_DJuko2);
	DDX_Control(pDX, IDC_EDT_D3, m_DJuko3);
	DDX_Control(pDX, IDC_EDT_D4, m_DJuko4);
	DDX_Control(pDX, IDC_EDT_D5, m_DJuko5);
	DDX_Control(pDX, IDC_EDT_D6, m_DJuko6);
	DDX_Control(pDX, IDC_EDT_D7, m_DJuko7);
	DDX_Control(pDX, IDC_EDT_D8, m_DJuko8);
	DDX_Control(pDX, IDC_BUTTONF1, m_ButtonJuko1);
	DDX_Control(pDX, IDC_BUTTONF2, m_ButtonJuko2);
	DDX_Control(pDX, IDC_BUTTONF3, m_ButtonJuko3);
	DDX_Control(pDX, IDC_BUTTONF4, m_ButtonJuko4);
	DDX_Control(pDX, IDC_BUTTONF5, m_ButtonJuko5);
	DDX_Control(pDX, IDC_BUTTONF6, m_ButtonJuko6);
	DDX_Control(pDX, IDC_BUTTONF7, m_ButtonJuko7);
	DDX_Control(pDX, IDC_BUTTONF8, m_ButtonJuko8);
	DDX_Control(pDX, IDC_STATICJUKO1, m_grpJuko1);
	DDX_Control(pDX, IDC_STATICJUKO2, m_grpJuko2);
	DDX_Control(pDX, IDC_STATICJUKO3, m_grpJuko3);
	DDX_Control(pDX, IDC_STATICJUKO4, m_grpJuko4);
	DDX_Control(pDX, IDC_STATICJUKO5, m_grpJuko5);
	DDX_Control(pDX, IDC_STATICJUKO6, m_grpJuko6);
	DDX_Control(pDX, IDC_STATICJUKO7, m_grpJuko7);
	DDX_Control(pDX, IDC_STATICJUKO8, m_grpJuko8);
	DDX_Control(pDX, IDC_STATICMXJUKO, m_staticMxJuko);
	DDX_Control(pDX, IDC_BUTTONKY1, m_buttonKy1);
	DDX_Control(pDX, IDCANCEL, m_ButtonCancel1);
}

BEGIN_MESSAGE_MAP(CMmDialogTenkai, CDialog)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_COMBONUM, &CMmDialogTenkai::OnCbnSelchangeCombonum)
	ON_BN_CLICKED(IDC_BUTTONF1, &CMmDialogTenkai::OnBnClickedButtonF1)
	ON_BN_CLICKED(IDC_BUTTONF2, &CMmDialogTenkai::OnBnClickedButtonF2)
	ON_BN_CLICKED(IDC_BUTTONF3, &CMmDialogTenkai::OnBnClickedButtonF3)
	ON_BN_CLICKED(IDC_BUTTONF4, &CMmDialogTenkai::OnBnClickedButtonF4)
	ON_BN_CLICKED(IDC_BUTTONF5, &CMmDialogTenkai::OnBnClickedButtonF5)
	ON_BN_CLICKED(IDC_BUTTONF6, &CMmDialogTenkai::OnBnClickedButtonF6)
	ON_BN_CLICKED(IDC_BUTTONF7, &CMmDialogTenkai::OnBnClickedButtonF7)
	ON_BN_CLICKED(IDC_BUTTONF8, &CMmDialogTenkai::OnBnClickedButtonF8)

	ON_WM_CLOSE()
	ON_BN_CLICKED(IDCANCEL, &CMmDialogTenkai::OnBnClickedCancel)
//	ON_BN_CLICKED(IDOK, &CMmDialogTenkai::OnBnClickedOk)

	ON_EN_CHANGE(IDC_EDT_W1, &CMmDialogTenkai::OnEnChangeEditW1)
	ON_EN_CHANGE(IDC_EDT_W2, &CMmDialogTenkai::OnEnChangeEditW2)
	ON_EN_CHANGE(IDC_EDT_W3, &CMmDialogTenkai::OnEnChangeEditW3)
	ON_EN_CHANGE(IDC_EDT_W4, &CMmDialogTenkai::OnEnChangeEditW4)
	ON_EN_CHANGE(IDC_EDT_W5, &CMmDialogTenkai::OnEnChangeEditW5)
	ON_EN_CHANGE(IDC_EDT_W6, &CMmDialogTenkai::OnEnChangeEditW6)
	ON_EN_CHANGE(IDC_EDT_W7, &CMmDialogTenkai::OnEnChangeEditW7)
	ON_EN_CHANGE(IDC_EDT_W8, &CMmDialogTenkai::OnEnChangeEditW8)

	ON_BN_CLICKED(IDC_BUTTONKY1, &CMmDialogTenkai::OnBnClickedButtonky1)
	ON_BN_CLICKED(IDC_KANKEIJK, &CMmDialogTenkai::OnBnClickedKankeijk)
	ON_BN_CLICKED(IDC_SELECTJK, &CMmDialogTenkai::OnBnClickedSelectjk)

	ON_EN_SETFOCUS(IDC_EDT_JK1, &CMmDialogTenkai::OnEnSetfocusEditJK1)
	ON_EN_SETFOCUS(IDC_EDT_JK2, &CMmDialogTenkai::OnEnSetfocusEditJK2)
	ON_EN_SETFOCUS(IDC_EDT_JK3, &CMmDialogTenkai::OnEnSetfocusEditJK3)
	ON_EN_SETFOCUS(IDC_EDT_JK4, &CMmDialogTenkai::OnEnSetfocusEditJK4)
	ON_EN_SETFOCUS(IDC_EDT_JK5, &CMmDialogTenkai::OnEnSetfocusEditJK5)
	ON_EN_SETFOCUS(IDC_EDT_JK6, &CMmDialogTenkai::OnEnSetfocusEditJK6)
	ON_EN_SETFOCUS(IDC_EDT_JK7, &CMmDialogTenkai::OnEnSetfocusEditJK7)
	ON_EN_SETFOCUS(IDC_EDT_JK8, &CMmDialogTenkai::OnEnSetfocusEditJK8)

	ON_EN_CHANGE(IDC_EDT_D1, &CMmDialogTenkai::OnEnChangeEditD1)
	ON_EN_CHANGE(IDC_EDT_D2, &CMmDialogTenkai::OnEnChangeEditD2)
	ON_EN_CHANGE(IDC_EDT_D3, &CMmDialogTenkai::OnEnChangeEditD3)
	ON_EN_CHANGE(IDC_EDT_D4, &CMmDialogTenkai::OnEnChangeEditD4)
	ON_EN_CHANGE(IDC_EDT_D5, &CMmDialogTenkai::OnEnChangeEditD5)
	ON_EN_CHANGE(IDC_EDT_D6, &CMmDialogTenkai::OnEnChangeEditD6)
	ON_EN_CHANGE(IDC_EDT_D7, &CMmDialogTenkai::OnEnChangeEditD7)
	ON_EN_CHANGE(IDC_EDT_D8, &CMmDialogTenkai::OnEnChangeEditD8)

	ON_EN_CHANGE(IDC_EDT_MD1, &CMmDialogTenkai::OnEnChangeEditMd1)
	ON_EN_CHANGE(IDC_EDT_MD2, &CMmDialogTenkai::OnEnChangeEditMd2)
	ON_EN_CHANGE(IDC_EDT_MD3, &CMmDialogTenkai::OnEnChangeEditMd3)
	ON_EN_CHANGE(IDC_EDT_MD4, &CMmDialogTenkai::OnEnChangeEditMd4)
	ON_EN_CHANGE(IDC_EDT_MD5, &CMmDialogTenkai::OnEnChangeEditMd5)
	ON_EN_CHANGE(IDC_EDT_MD6, &CMmDialogTenkai::OnEnChangeEditMd6)
	ON_EN_CHANGE(IDC_EDT_MD7, &CMmDialogTenkai::OnEnChangeEditMd7)
	ON_EN_CHANGE(IDC_EDT_MD8, &CMmDialogTenkai::OnEnChangeEditMd8)

	ON_CBN_SELCHANGE(IDC_COMBONUM, &CMmDialogTenkai::OnCbnSelchangeCombonum)
END_MESSAGE_MAP()

// CMmDialogTenkai メッセージ ハンドラ
namespace MC
{

void MCmdTenkaiEnd();

} // namespace MC

BOOL CMmDialogTenkai::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	CFont	myfont1;
	myfont1.CreateFont( 18, 0, 0, 0, 900,  0, 0, 0, SHIFTJIS_CHARSET,
						OUT_DEVICE_PRECIS, CLIP_DEFAULT_PRECIS,
						DEFAULT_QUALITY, DEFAULT_PITCH, Mstr( "ＭＳ Ｐゴシック"));

//	pWndInfo = MmWndKGetCurWnd();							// カレントウィンドウ取得

///////////////////////////////////////////////////////////////////////////////
//	住戸数入力ダイアログを初期化する

//	住戸数入力用コンポボックスに住戸数の項目(一覧)を設定する

	m_comboJuko.ResetContent();

	for (MINT ic=1; ic<=MX_NJUKO; ic++) {
		CString sKoumoku;
		sKoumoku.Format(Mstr( "  %4d"), ic);
		m_comboJuko.InsertString( -1, sKoumoku);
	}

//	住棟展開ダイアログのエディットコントロールに初期値を設定する

	m_modeSel = MC_TENKAI_SET;		
	m_iSelJuko = m_iRefJuko = 1;
	m_TenkaiPara = *MC::JTTenkai::GetJTTPara();
	SetControl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void CMmDialogTenkai::OnCbnSelchangeCombonum()
{
	MINT iMxJuko = m_comboJuko.GetCurSel();
	m_TenkaiPara.nJuko = iMxJuko+1;
	SetControl();
}

//	展開元住戸を設定し表示

void CMmDialogTenkai::SetnJuko(
						MINT	iSel,			// 展開先住戸番号、または、NULL
						MINT	iRef			// 展開元住戸番号、または、NULL
				)
{
	if ( iSel)
		m_iSelJuko = iSel;
	if ( iRef)
		m_iRefJuko = iRef;
	if (m_modeSel == MC_TENKAI_SET) {			// 展開住戸設定
		m_TenkaiPara.nOJuko[m_iSelJuko-1] = m_iRefJuko;
		SetControl();
	} else {									// 入力住戸選択
		MC::JTTenkai::SetJTTJuuko( iRef);
	}
}

//	住棟展開ダイアログにカレントの値を設定する

void CMmDialogTenkai::SetControl()
{
	CString snJuko;
	snJuko.Format( Mstr( "%d"), m_TenkaiPara.nJuko);
	m_comboJuko.SetWindowTextW( snJuko);

	MINT iDx1 = (238 - 98) * (MX_NJUKO - m_TenkaiPara.nJuko) / MX_NJUKO;
	MINT iDx2 = (295 - 35) * (MX_NJUKO - m_TenkaiPara.nJuko) / MX_NJUKO;
	MINT iDx3 = (590 - 30) * (MX_NJUKO - m_TenkaiPara.nJuko) / MX_NJUKO;

	this->SetWindowPos( &CWnd::wndTop, 290 + iDx1, 100, 668 - iDx3, 283, SWP_SHOWWINDOW);

	m_staticMxJuko.ShowWindow( 0);
	m_staticMxJuko.SetWindowPos( &CWnd::wndTop, 238 - iDx1, 17, 50, 15, SWP_SHOWWINDOW);
	m_comboJuko.ShowWindow( 0);
	m_comboJuko.SetWindowPos( &CWnd::wndTop, 282 - iDx1, 15, 45, 15, SWP_SHOWWINDOW);
	m_ButtonCancel1.ShowWindow( 0);
	m_ButtonCancel1.SetWindowPos( &CWnd::wndTop, 590 - iDx3, 217, 60, 25, SWP_SHOWWINDOW);
	m_buttonKy1.ShowWindow( 0);
	m_buttonKy1.SetWindowPos( &CWnd::wndTop, 295 - iDx2, 217, 60, 26, SWP_SHOWWINDOW);

	for (MINT ic=0; ic<m_TenkaiPara.nJuko; ic++) {
		MINT ic1 = m_TenkaiPara.nOJuko[ic] - 1;
		m_TenkaiPara.rDJuko[ic] = m_TenkaiPara.rDJuko[ic1];
		m_TenkaiPara.rWJuko[ic] = m_TenkaiPara.rWJuko[ic1];
	}

	for (MINT ic=0; ic<MX_NJUKO; ic++) {
		if ( ic < m_TenkaiPara.nJuko) {
			m_pH( ic)->SetCheck( m_TenkaiPara.ifH[ic]);
			m_pNmJuko( ic)->SetWindowTextW( MC::MsCnvSMINT( 0, m_TenkaiPara.nOJuko[ic]));
			m_pMDJuko( ic)->SetWindowTextW( MC::MsCnvSMREAL( 1, m_TenkaiPara.rMDJuko[ic]));
			m_pDJuko( ic)->SetWindowTextW( MC::MsCnvSMREAL( 1, m_TenkaiPara.rDJuko[ic]));
			m_pWJuko( ic)->SetWindowTextW( MC::MsCnvSMREAL( 1, m_TenkaiPara.rWJuko[ic]));

			m_pgrpJuko( ic)->ShowWindow( 1);
			m_pH( ic)->ShowWindow( 1);
			m_pButtonJuko( ic)->ShowWindow( 1);
			m_pNmJuko( ic)->ShowWindow( 1);
			m_pMDJuko( ic)->ShowWindow( 1);
			m_pDJuko( ic)->ShowWindow( 1);
			m_pWJuko( ic)->ShowWindow( 1);
		} else {
			m_pgrpJuko( ic)->ShowWindow( 0);
			m_pH( ic)->ShowWindow( 0);
			m_pButtonJuko( ic)->ShowWindow( 0);
			m_pNmJuko( ic)->ShowWindow( 0);
			m_pMDJuko( ic)->ShowWindow( 0);
			m_pDJuko( ic)->ShowWindow( 0);
			m_pWJuko( ic)->ShowWindow( 0);
		}
	}

	MC::JTTenkai::SetJTTPara( &m_TenkaiPara);
	MC::JTTenkai::CreJTTArea();
}

void CMmDialogTenkai::PostNcDestroy()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if (m_pParent != NULL) {
		MC::MCmdTenkaiEnd();
		delete this;
	}

	CDialog::PostNcDestroy();
}

void CMmDialogTenkai::OnClose()
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	if (m_pParent != NULL) {
		DestroyWindow();
	} else {
		CDialog::OnClose();
	}
}

CStatic* CMmDialogTenkai::m_pgrpJuko( MINT iSel)
{
	CStatic* pgrpJuko[] =	  { &m_grpJuko1, &m_grpJuko2, &m_grpJuko3, &m_grpJuko4, 
								&m_grpJuko5, &m_grpJuko6, &m_grpJuko7, &m_grpJuko8};
	return pgrpJuko[iSel];
}

//	反転フラグ　チェックボックス
CButton* CMmDialogTenkai::m_pH( MINT iSel)
{
	CButton* pH[] =			  { &m_H1, &m_H2, &m_H3, &m_H4,
								&m_H5, &m_H6, &m_H7, &m_H8};
	return pH[iSel];
}

// 展開元住戸選択　ボタン
CButton* CMmDialogTenkai::m_pButtonJuko( MINT iSel)
{
	CButton* pButtonJuko[] =  { &m_ButtonJuko1, &m_ButtonJuko2, &m_ButtonJuko3,
								&m_ButtonJuko4, &m_ButtonJuko5, &m_ButtonJuko6,
								&m_ButtonJuko7, &m_ButtonJuko8};
	return pButtonJuko[iSel];
}

// 展開元住戸設定　エディットボックス
CEdit* CMmDialogTenkai::m_pNmJuko( MINT iSel)
{
	CEdit* pnmJuko[] =		  { &m_NmJuko1, &m_NmJuko2, &m_NmJuko3, &m_NmJuko4,
								&m_NmJuko5, &m_NmJuko6, &m_NmJuko7, &m_NmJuko8};
	return pnmJuko[iSel];
}

// 縦方向オフセット　エディットボックス
CEdit* CMmDialogTenkai::m_pMDJuko( MINT iSel)
{
	CEdit* pmdJuko[] =		  { &m_MDJuko1, &m_MDJuko2, &m_MDJuko3, &m_MDJuko4,
								&m_MDJuko5, &m_MDJuko6, &m_MDJuko7, &m_MDJuko8};
	return pmdJuko[iSel];
}

// 住戸奥行　エディットボックス
CEdit* CMmDialogTenkai::m_pDJuko( MINT iSel)
{
	CEdit* pdJuko[] =		  { &m_DJuko1, &m_DJuko2, &m_DJuko3, &m_DJuko4,
								&m_DJuko5, &m_DJuko6, &m_DJuko7, &m_DJuko8};
	return pdJuko[iSel];
}

// 住戸巾　エディットボックス
CEdit* CMmDialogTenkai::m_pWJuko( MINT iSel)
{
	CEdit* pwJuko[] =		  { &m_WJuko1, &m_WJuko2, &m_WJuko3, &m_WJuko4,
								&m_WJuko5, &m_WJuko6, &m_WJuko7, &m_WJuko8};
	return pwJuko[iSel];
}

// キャンセル処理
void CMmDialogTenkai::OnBnClickedCancel()
{

	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if (m_pParent != NULL) {
		DestroyWindow();
	} else {
//		CDialog::OnClose();
		OnCancel();
	}
}
/*
//	ＯＫボタン
void CMmDialogTenkai::OnBnClickedOk()
{
*/
/*
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if (m_pParent != NULL) {
		DestroyWindow();
	} else {
//		CDialog::OnClose();
		OnOK();
	}
*/
//}

void CMmDialogTenkai::OnBnClickedButtonF1()	{ SetnJuko( NULL, 1);}
void CMmDialogTenkai::OnBnClickedButtonF2()	{ SetnJuko( NULL, 2);}
void CMmDialogTenkai::OnBnClickedButtonF3()	{ SetnJuko( NULL, 3);}
void CMmDialogTenkai::OnBnClickedButtonF4()	{ SetnJuko( NULL, 4);}
void CMmDialogTenkai::OnBnClickedButtonF5()	{ SetnJuko( NULL, 5);}
void CMmDialogTenkai::OnBnClickedButtonF6()	{ SetnJuko( NULL, 6);}
void CMmDialogTenkai::OnBnClickedButtonF7()	{ SetnJuko( NULL, 7);}
void CMmDialogTenkai::OnBnClickedButtonF8()	{ SetnJuko( NULL, 8);}

//	共通部の選択
void CMmDialogTenkai::OnBnClickedButtonky1(){
	SetnJuko( NULL, 0);};

// 各住戸の幅を設定
void CMmDialogTenkai::ChangeEditW( MINT iJ)
{
	CString sD;
	m_pWJuko( iJ)->GetWindowTextW( sD);
	m_TenkaiPara.rWJuko[iJ] = MC::MsGetMREAL( sD);
}
void CMmDialogTenkai::OnEnChangeEditW1()	{	ChangeEditW( 0);}
	// TODO:  これが RICHEDIT コントロールの場合、
	// まず、CDialog::OnInitDialog() 関数をオーバーライドして、OR 状態の ENM_CHANGE
	// フラグをマスクに入れて、CRichEditCtrl().SetEventMask() を呼び出さない限り、
	// コントロールは、この通知を送信しません。

	// TODO:  ここにコントロール通知ハンドラ コードを追加してください。
//	CString sW;
//	m_WJuko1.GetWindowTextW( sW);
//	m_TenkaiPara.rWJuko[0] = MsGetMREAL( sW);

void CMmDialogTenkai::OnEnChangeEditW2()	{ ChangeEditW( 1);}
void CMmDialogTenkai::OnEnChangeEditW3()	{ ChangeEditW( 2);}
void CMmDialogTenkai::OnEnChangeEditW4()	{ ChangeEditW( 3);}
void CMmDialogTenkai::OnEnChangeEditW5()	{ ChangeEditW( 4);}
void CMmDialogTenkai::OnEnChangeEditW6()	{ ChangeEditW( 5);}
void CMmDialogTenkai::OnEnChangeEditW7()	{ ChangeEditW( 6);}
void CMmDialogTenkai::OnEnChangeEditW8()	{ ChangeEditW( 7);}

//	住戸関係付け
void CMmDialogTenkai::OnBnClickedKankeijk()
{
	m_modeSel = MC_TENKAI_SET;
}
//	住戸選択
void CMmDialogTenkai::OnBnClickedSelectjk()
{
	m_modeSel = MC_TENKAI_SEL;
}
/*
void CMmDialogTenkai::OnLButtonDown(UINT nFlags, CPoint point)
{
	CDialog::OnLButtonDown(nFlags, point);
}
*/
void CMmDialogTenkai::OnEnSetfocusEditJK1()	{ SetnJuko( 1, NULL);}
void CMmDialogTenkai::OnEnSetfocusEditJK2()	{ SetnJuko( 2, NULL);}
void CMmDialogTenkai::OnEnSetfocusEditJK3()	{ SetnJuko( 3, NULL);}
void CMmDialogTenkai::OnEnSetfocusEditJK4()	{ SetnJuko( 4, NULL);}
void CMmDialogTenkai::OnEnSetfocusEditJK5()	{ SetnJuko( 5, NULL);}
void CMmDialogTenkai::OnEnSetfocusEditJK6()	{ SetnJuko( 6, NULL);}
void CMmDialogTenkai::OnEnSetfocusEditJK7()	{ SetnJuko( 7, NULL);}
void CMmDialogTenkai::OnEnSetfocusEditJK8()	{ SetnJuko( 8, NULL);}

BOOL CMmDialogTenkai::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_RETURN) {
			SetControl();
			return TRUE;
		}
//		if (pMsg->wParam == VK_ESCAPE) {
//			SetControl();
//			return TRUE;
//		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

//	住戸奥行き設定
void CMmDialogTenkai::ChangeEditD( MINT iJ)
{
	CString sD;
	m_pDJuko( iJ)->GetWindowTextW( sD);
	m_TenkaiPara.rDJuko[iJ] = MC::MsGetMREAL( sD);
}
void CMmDialogTenkai::OnEnChangeEditD1()	{ ChangeEditD( 0);}
void CMmDialogTenkai::OnEnChangeEditD2()	{ ChangeEditD( 1);}
void CMmDialogTenkai::OnEnChangeEditD3()	{ ChangeEditD( 2);}
void CMmDialogTenkai::OnEnChangeEditD4()	{ ChangeEditD( 3);}
//	CString sD;
//	m_DJuko4.GetWindowTextW( sD);
//	m_TenkaiPara.rDJuko[3] = MsGetMREAL( sD);

void CMmDialogTenkai::OnEnChangeEditD5()	{ ChangeEditD( 4);}
void CMmDialogTenkai::OnEnChangeEditD6()	{ ChangeEditD( 5);}
void CMmDialogTenkai::OnEnChangeEditD7()	{ ChangeEditD( 6);}
void CMmDialogTenkai::OnEnChangeEditD8()	{ ChangeEditD( 7);}

//	住戸奥行き設定
void CMmDialogTenkai::ChangeEditMd( MINT iJ)
{
	CString sD;
	m_pMDJuko( iJ)->GetWindowTextW( sD);
	m_TenkaiPara.rMDJuko[iJ] = MC::MsGetMREAL( sD);
}
void CMmDialogTenkai::OnEnChangeEditMd1()	{ ChangeEditMd( 0);}
//	CString sD;
//	m_MDJuko1.GetWindowTextW( sD);
//	m_TenkaiPara.rMDJuko[0] = MsGetMREAL( sD);

void CMmDialogTenkai::OnEnChangeEditMd2()	{ ChangeEditMd( 1);}
void CMmDialogTenkai::OnEnChangeEditMd3()	{ ChangeEditMd( 2);}
void CMmDialogTenkai::OnEnChangeEditMd4()	{ ChangeEditMd( 3);}
void CMmDialogTenkai::OnEnChangeEditMd5()	{ ChangeEditMd( 4);}
void CMmDialogTenkai::OnEnChangeEditMd6()	{ ChangeEditMd( 5);}
void CMmDialogTenkai::OnEnChangeEditMd7()	{ ChangeEditMd( 6);}
void CMmDialogTenkai::OnEnChangeEditMd8()	{ ChangeEditMd( 7);}


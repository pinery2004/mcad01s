// MmDialogGrid.cpp : 実装ファイル
//

#include "stdafx.h"
#include "MCAD.h"
#include "MbBasic.h"
#include "MmCmd.h"
#include "MmWnd.h"
#include "resource.h"
#include "MmDialogGrid.h"

#define		MINGRIDNUM	-100
#define		MAXGRIDNUM	 200


static	MINT		s_nGrid[5][4] = {
									16, -3, -3, 17, 
									17, -3, -3, 27, 
									27, -3, -3, 37, 
									27, -3, -3, 17, 
									37, -3, -3, 27
									};

// CMmDialogGrid ダイアログ

IMPLEMENT_DYNAMIC(CMmDialogGrid, CDialog)

CMmDialogGrid::CMmDialogGrid(CWnd* pParent /*=NULL*/)
	: CDialog(CMmDialogGrid::IDD, pParent)
{
	m_pParent = NULL;
}

CMmDialogGrid::~CMmDialogGrid()
{
}

BOOL CMmDialogGrid::Create(CWnd * pWnd)
{
	m_pParent = pWnd;
	BOOL bret = CDialog::Create( CMmDialogGrid::IDD, m_pParent);

	if (bret == TRUE)
	{
		this->ShowWindow( SW_SHOW);
	}
	return bret;
}

void CMmDialogGrid::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDT_GRIDNUM1, m_EditGridNum1);
	DDX_Control(pDX, IDC_EDT_GRIDNUM2, m_EditGridNum2);
	DDX_Control(pDX, IDC_EDT_GRIDNUM3, m_EditGridNum3);
	DDX_Control(pDX, IDC_EDT_GRIDNUM4, m_EditGridNum4);
	DDX_Control(pDX, IDC_SPINGRIDNUM1, m_SpinGridNum1);
	DDX_Control(pDX, IDC_SPINGRIDNUM2, m_SpinGridNum2);
	DDX_Control(pDX, IDC_SPINGRIDNUM3, m_SpinGridNum3);
	DDX_Control(pDX, IDC_SPINGRIDNUM4, m_SpinGridNum4);
}


BEGIN_MESSAGE_MAP(CMmDialogGrid, CDialog)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPINGRIDNUM1, &CMmDialogGrid::OnDeltaposSpingridnum1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPINGRIDNUM2, &CMmDialogGrid::OnDeltaposSpingridnum2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPINGRIDNUM3, &CMmDialogGrid::OnDeltaposSpingridnum3)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPINGRIDNUM4, &CMmDialogGrid::OnDeltaposSpingridnum4)
	ON_BN_CLICKED(IDC_BUTTONGRIDNUM1, &CMmDialogGrid::OnBnClickedButtongridnum1)
	ON_BN_CLICKED(IDC_BUTTONGRIDNUM2, &CMmDialogGrid::OnBnClickedButtongridnum2)
	ON_BN_CLICKED(IDC_BUTTONGRIDNUM4, &CMmDialogGrid::OnBnClickedButtongridnum4)
	ON_BN_CLICKED(IDC_BUTTONGRIDNUM3, &CMmDialogGrid::OnBnClickedButtongridnum3)
	ON_BN_CLICKED(IDC_BUTTONGRIDNUM5, &CMmDialogGrid::OnBnClickedButtongridnum5)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// CMmDialogGrid メッセージ ハンドラ

void CMmDialogGrid::OnDeltaposSpingridnum1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	MINT	nGrid[4];										// 北方向, 西方向, 南方向, 東方向
	MINT	nGrid1, nGrid2;

	nGrid1 = (MSHORT)m_SpinGridNum1.GetPos();
	nGrid2 = nGrid1 + pNMUpDown->iDelta;
	m_SpinGridNum1.SetPos( nGrid2);

	GetSpinGridNum( nGrid);
	SetGridNum( nGrid);

	m_SpinGridNum1.SetPos( nGrid1);
	*pResult = 0;
}

void CMmDialogGrid::OnDeltaposSpingridnum2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	MINT	nGrid[4];										// 北方向, 西方向, 南方向, 東方向
	MINT	nGrid1, nGrid2;

	nGrid1 = (MSHORT)m_SpinGridNum2.GetPos();
	nGrid2 = nGrid1 + pNMUpDown->iDelta;
	m_SpinGridNum2.SetPos( nGrid2);

	GetSpinGridNum( nGrid);
	SetGridNum( nGrid);

	m_SpinGridNum2.SetPos( nGrid1);
	*pResult = 0;
}

void CMmDialogGrid::OnDeltaposSpingridnum3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	MINT	nGrid[4];										// 北方向, 西方向, 南方向, 東方向
	MINT	nGrid1, nGrid2;

	nGrid1 = (MSHORT)m_SpinGridNum3.GetPos();
	nGrid2 = nGrid1 + pNMUpDown->iDelta;
	m_SpinGridNum3.SetPos( nGrid2);

	GetSpinGridNum( nGrid);
	SetGridNum( nGrid);

	m_SpinGridNum3.SetPos( nGrid1);
	*pResult = 0;
}

void CMmDialogGrid::OnDeltaposSpingridnum4(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	MINT	nGrid[4];										// 北方向, 西方向, 南方向, 東方向
	MINT	nGrid1, nGrid2;

	nGrid1 = (MSHORT)m_SpinGridNum4.GetPos();
	nGrid2 = nGrid1 + pNMUpDown->iDelta;
	m_SpinGridNum4.SetPos( nGrid2);

	GetSpinGridNum( nGrid);
	SetGridNum( nGrid);

	m_SpinGridNum4.SetPos( nGrid1);
	*pResult = 0;
}

void CMmDialogGrid::OnBnClickedButtongridnum1()
{
	SetGridNum( s_nGrid[0]);
}

void CMmDialogGrid::OnBnClickedButtongridnum2()
{
	SetGridNum( s_nGrid[1]);
}

void CMmDialogGrid::OnBnClickedButtongridnum3()
{
	SetGridNum( s_nGrid[2]);
}

void CMmDialogGrid::OnBnClickedButtongridnum4()
{
	SetGridNum( s_nGrid[3]);
}

void CMmDialogGrid::OnBnClickedButtongridnum5()
{
	SetGridNum( s_nGrid[4]);
}

void CMmDialogGrid::GetSpinGridNum(
					MINT		*nGrid							// 北方向, 西方向, 南方向, 東方向
					)
{
	nGrid[0] = (MSHORT)m_SpinGridNum1.GetPos();
	nGrid[1] = (MSHORT)m_SpinGridNum2.GetPos();
	nGrid[2] = (MSHORT)m_SpinGridNum3.GetPos();
	nGrid[3] = (MSHORT)m_SpinGridNum4.GetPos();
}

void CMmDialogGrid::SetSpinGridNum(
					MINT		*nGrid							// 北方向, 西方向, 南方向, 東方向
					)
{
	m_SpinGridNum1.SetPos( nGrid[0]);
	m_SpinGridNum2.SetPos( nGrid[1]);
	m_SpinGridNum3.SetPos( nGrid[2]);
	m_SpinGridNum4.SetPos( nGrid[3]);
}

namespace MC
{

void MCmdGridNumEnd();

} // namespace MC

void CMmDialogGrid::PostNcDestroy()
{
	if (m_pParent != NULL) {
		MC::MCmdGridNumEnd();
		delete this;
	}

	CDialog::PostNcDestroy();
}

void CMmDialogGrid::OnClose()
{
	if (m_pParent != NULL) {
		DestroyWindow();
	} else {
		CDialog::OnClose();
	}
}


BOOL CMmDialogGrid::OnInitDialog()
{
	MINT				nGrid[4];								// 北方向, 西方向, 南方向, 東方向
	MC::MmWndInfo*			pWndInfo;

	CDialog::OnInitDialog();

	CFont	myfont1;
	myfont1.CreateFont( 18, 0, 0, 0, 900,  0, 0, 0, SHIFTJIS_CHARSET,
						OUT_DEVICE_PRECIS, CLIP_DEFAULT_PRECIS,
						DEFAULT_QUALITY, DEFAULT_PITCH, Mstr( "ＭＳ Ｐゴシック"));
	m_EditGridNum1.SetFont( &myfont1);
	m_EditGridNum2.SetFont( &myfont1);
	m_EditGridNum3.SetFont( &myfont1);
	m_EditGridNum4.SetFont( &myfont1);

	m_SpinGridNum1.SetRange( MINGRIDNUM, MAXGRIDNUM);
	m_SpinGridNum2.SetRange( MINGRIDNUM, MAXGRIDNUM);
	m_SpinGridNum3.SetRange( MINGRIDNUM, MAXGRIDNUM);
	m_SpinGridNum4.SetRange( MINGRIDNUM, MAXGRIDNUM);

	pWndInfo = MC::WindowCtrl::MmWndKGetCurWnd();							// カレントウィンドウ取得
	if (pWndInfo == NULL) MQUIT;

	pWndInfo->GetGridNum( nGrid);
	SetSpinGridNum( nGrid);
	
//	SetGridNum( s_nGrid[0]);
exit:
	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

/////////////////////////////////////////////////////////////////////////////
//	グリッド数設定	（シングルグリッド用）
void CMmDialogGrid::SetGridNum(
						MINT*		nGrid						// (I  ) グリッド数（北、西、南、東）または NULL(初期設定)
				)
{
	MC::WindowCtrl::MmGridNumSet( nGrid);
	SetSpinGridNum( nGrid);
	MC::WindowCtrl::MmGridNumXqt();
}

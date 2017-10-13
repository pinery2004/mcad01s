// MmDialogPtsEdit.cpp : 実装ファイル
//

#include "stdafx.h"
#include "MCAD.h"
#include "resource.h"

#define DLL_EXPORT_DO
#include "../msub/gridctrl/gridctrl.h"

#include "MmDialogPtsEdit.h"

// CMmDialogPtsEdit ダイアログ

IMPLEMENT_DYNAMIC(CMmDialogPtsEdit, CDialog)

CMmDialogPtsEdit::CMmDialogPtsEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CMmDialogPtsEdit::IDD, pParent)
	, m_pParent(NULL)
{
	m_nFixCols = 1;
	m_nFixRows = 1;
	m_nCols = 8;
	m_nRows = 20;
	m_bHeaderSort = FALSE;
//	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CMmDialogPtsEdit::~CMmDialogPtsEdit()
{
}

void CMmDialogPtsEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PARTSLIST1, m_GridPtLst1);
}

BEGIN_MESSAGE_MAP(CMmDialogPtsEdit, CDialog)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDCANCEL, &CMmDialogPtsEdit::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CMmDialogPtsEdit::OnBnClickedOk)
END_MESSAGE_MAP()

// CMmDialogPtsEdit メッセージ ハンドラ

BOOL CMmDialogPtsEdit::Create(CWnd* pWnd)
{
	m_pParent = pWnd;
	BOOL bret = CDialog::Create( CMmDialogPtsEdit::IDD, m_pParent);
	if (bret == TRUE)
		this->ShowWindow( SW_SHOW);
	return bret;
}

BOOL CMmDialogPtsEdit::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください
//a	m_ImageList.Create(MAKEINTRESOURCE(IDB_IMAGES), 16, 1, RGB(255,255,255));
//a	m_GridPtLst1.SetImageList(&m_ImageList);

//	ドラッグ　アンド　ドロップ　有効　設定
	m_GridPtLst1.EnableDragAndDrop(TRUE);
//	背景色の設定
	m_GridPtLst1.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));

	UpdateData();

//	固定カラム数、固定行数（スクロールしないカラム、行）
	m_nFixCols = 2;
	m_nFixRows = 1;
//	最大カラム数、最大行数
	m_nCols = 6;
	m_nRows = 16;

	TRY {
		m_GridPtLst1.SetRowCount(m_nRows);
		m_GridPtLst1.SetColumnCount(m_nCols);
		m_GridPtLst1.SetFixedRowCount(m_nFixRows);
		m_GridPtLst1.SetFixedColumnCount(m_nFixCols);
	}
	CATCH (CMemoryException, e)
	{
	   	e->ReportError();
		MQUIT;
	}
	END_CATCH

	// fill rows/cols with text
	for (int row = 0; row < m_GridPtLst1.GetRowCount(); row++)
	{
		for (int col = 0; col < m_GridPtLst1.GetColumnCount(); col++)
		{ 
			CString str;

			GV_ITEM Item;

			Item.mask = GVIF_TEXT;
			Item.row = row;
			Item.col = col;

			if (row < m_nFixRows)
				str.Format(_T("Column %d"),col);
//			else if (col < m_nFixCols) 
			else if (col < 1) 
				str.Format(_T("Row %d"), row);
			else 
				str.Format(_T("%d"),row*col);
			Item.strText = str;

			if (rand() % 10 == 1)
			{
				COLORREF clr = RGB(rand()%128 + 128, rand()%128 + 128, rand()%128 + 128);
				Item.crBkClr = clr;             // or - m_GridPtLst1.SetItemBkColour(row, col, clr);
				Item.crFgClr = RGB(255,0,0);    // or - m_GridPtLst1.SetItemFgColour(row, col, RGB(255,0,0));				    
				Item.mask    = (GVIF_BKCLR|GVIF_FGCLR);
			}

//			if (col < m_GridPtLst1.GetFixedColumnCount())
			if (col < 1)
			{
//a				Item.iImage = rand()%m_ImageList.GetImageCount();
				Item.mask  |= (GVIF_IMAGE);
			}

			m_GridPtLst1.SetItem(&Item);
		}
	}
	m_GridPtLst1.AutoSize();
	UpdateData(FALSE);

exit:
	m_bHeaderSort = !m_bHeaderSort;
	m_GridPtLst1.SetHeaderSort(m_bHeaderSort);

//	ソート条件設定　数値・文字列
	m_GridPtLst1.SetCompareFunction(CGridCtrl::pfnCellNumericCompare);

//	m_pParent = NULL;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

namespace MC
{

void MCmdPtsEditEnd();

} // namespace MC

void CMmDialogPtsEdit::PostNcDestroy()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if (m_pParent != NULL) {
		MC::MCmdPtsEditEnd();
		delete this;
	}

	CDialog::PostNcDestroy();
}

void CMmDialogPtsEdit::OnClose()
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	if (m_pParent != NULL) {
		DestroyWindow();
	} else {
		CDialog::OnClose();
	}
}

void CMmDialogPtsEdit::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if (m_pParent != NULL) {
		DestroyWindow();
	} else {
		OnCancel();
	}
}

void CMmDialogPtsEdit::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if (m_pParent != NULL) {
		DestroyWindow();
	} else {
		OnOK();
	}
}

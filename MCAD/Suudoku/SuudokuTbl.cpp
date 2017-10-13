// SuudokuTbl.cpp : 実装ファイル
//

#include "stdafx.h"
#include "resource.h"
#include "SuudokuTbl.h"


// CSuudokuTbl ダイアログ

IMPLEMENT_DYNAMIC(CSuudokuTbl, CDialog)

CSuudokuTbl::CSuudokuTbl(CWnd* pParent /*=NULL*/)
	: CDialog(CSuudokuTbl::IDD, pParent)
	, m_pParent(NULL)
	, m_nFixCols(0)
	, m_nFixRows(0)
	, m_nCols(0)
	, m_nRows(0)
	, m_bHeaderSort(FALSE)
{

}

CSuudokuTbl::~CSuudokuTbl()
{
}

void CSuudokuTbl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSuudokuTbl, CDialog)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDCANCEL, &CSuudokuTbl::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CSuudokuTbl::OnBnClickedOk)
END_MESSAGE_MAP()


// CSuudokuTbl メッセージ ハンドラ

BOOL CSuudokuTbl::Create(CWnd* pWnd)
{
	m_pParent = pWnd;
	BOOL bret = CDialog::Create( CSuudokuTbl::IDD, m_pParent);
	if (bret == TRUE)
	{
		this->ShowWindow( SW_SHOW);
	}
	return bret;
//	return 0;
}

BOOL CSuudokuTbl::OnInitDialog()
{
//	CDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	static int RowH[] = { 30, 50, 50, 50, 50, 
						  50, 50, 50, 50, 50,
						  50, 50, 50, 50, 50,
						  50, 50, 50, 50, 50 };
	static int ColW[] = { 30, 50, 50, 50, 50, 
						  50, 50, 50, 50, 50,
						  50, 50, 50, 50, 50,
						  50, 50, 50, 50, 50 };

	CDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください
//	ドラッグ　アンド　ドロップ　有効　設定
	m_SudokuGrid.EnableDragAndDrop(TRUE);
//	背景色の設定
	m_SudokuGrid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));

	UpdateData();

//	固定カラム数、固定行数（スクロールしないカラム、行）
	m_nFixCols = 1;
	m_nFixRows = 1;
//	最大カラム数、最大行数
	m_nCols = 10;
	m_nRows = 10;

	TRY {
		m_SudokuGrid.SetRowCount(m_nRows);
		m_SudokuGrid.SetColumnCount(m_nCols);
		m_SudokuGrid.SetFixedRowCount(m_nFixRows);
		m_SudokuGrid.SetFixedColumnCount(m_nFixCols);
	}
	CATCH (CMemoryException, e)
	{
	   	e->ReportError();
		goto exit;
	}
	END_CATCH

	// fill rows/cols with text
	for (int row = 0; row < m_SudokuGrid.GetRowCount(); row++)
	{
		for (int col = 0; col < m_SudokuGrid.GetColumnCount(); col++)
		{ 
			CString str;

			GV_ITEM Item;

			Item.mask = GVIF_TEXT;
			Item.row = row;
			Item.col = col;

			if (row < m_nFixRows)
				str.Format(_T("%d"),col);
			else if (col < m_nFixCols) 
				str.Format(_T("%d"), row);
			else  {
				str.Format(_T("%d"),row*col);

				if (rand() % 10 == 1)
				{
					COLORREF clr = RGB(rand()%128 + 128, rand()%128 + 128, rand()%128 + 128);
					Item.crBkClr = clr;             // or - m_GridPtLst1.SetItemBkColour(row, col, clr);
					Item.crFgClr = RGB(255,0,0);    // or - m_GridPtLst1.SetItemFgColour(row, col, RGB(255,0,0));				    
					Item.mask    = (GVIF_BKCLR|GVIF_FGCLR);
				}
			}

			Item.strText = str;
			if (col < m_SudokuGrid.GetFixedColumnCount())
//			if (col < 1)
			{
//a				Item.iImage = rand()%m_SudokuGrid.GetImageCount();
				Item.mask  |= (GVIF_IMAGE);
			}

			if (row == 0) {
				int iw = m_SudokuGrid.GetColumnWidth( col);
				m_SudokuGrid.SetColumnWidth( col, ColW[col]);
			}
			m_SudokuGrid.SetItem(&Item);
		}
		BOOL bh = m_SudokuGrid.SetRowHeight( row, RowH[row]);
	}
//	m_SudokuGrid.AutoSize();
	UpdateData(FALSE);

exit:
	m_bHeaderSort = !m_bHeaderSort;
	m_SudokuGrid.SetHeaderSort(m_bHeaderSort);

//	ソート条件設定　数値・文字列
	m_SudokuGrid.SetCompareFunction(CGridCtrl::pfnCellNumericCompare);

//	m_pParent = NULL;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。

//	return TRUE;  // return TRUE unless you set the focus to a control
//	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void MCmdSuudokuEnd();

void CSuudokuTbl::PostNcDestroy()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if( m_pParent != NULL) {
		MCmdSuudokuEnd();
		delete this;
	}

	CDialog::PostNcDestroy();
}

void CSuudokuTbl::OnClose()
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	if( m_pParent != NULL) {
		DestroyWindow();
	} else {
		CDialog::OnClose();
	}
}

void CSuudokuTbl::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if (m_pParent != NULL) {
		DestroyWindow();
	} else {
		OnCancel();
	}
}

void CSuudokuTbl::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if (m_pParent != NULL) {
		DestroyWindow();
	} else {
		OnOK();
	}
}


BOOL CSuudokuTbl::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

//	return CDialog::Create(lpszTemplateName, pParentWnd);
	return TRUE;
}

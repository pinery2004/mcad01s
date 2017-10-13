// SuudokuTbl.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "resource.h"
#include "SuudokuTbl.h"


// CSuudokuTbl �_�C�A���O

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


// CSuudokuTbl ���b�Z�[�W �n���h��

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

	// TODO:  �����ɏ�������ǉ����Ă�������
	static int RowH[] = { 30, 50, 50, 50, 50, 
						  50, 50, 50, 50, 50,
						  50, 50, 50, 50, 50,
						  50, 50, 50, 50, 50 };
	static int ColW[] = { 30, 50, 50, 50, 50, 
						  50, 50, 50, 50, 50,
						  50, 50, 50, 50, 50,
						  50, 50, 50, 50, 50 };

	CDialog::OnInitDialog();

	// TODO:  �����ɏ�������ǉ����Ă�������
//	�h���b�O�@�A���h�@�h���b�v�@�L���@�ݒ�
	m_SudokuGrid.EnableDragAndDrop(TRUE);
//	�w�i�F�̐ݒ�
	m_SudokuGrid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));

	UpdateData();

//	�Œ�J�������A�Œ�s���i�X�N���[�����Ȃ��J�����A�s�j
	m_nFixCols = 1;
	m_nFixRows = 1;
//	�ő�J�������A�ő�s��
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

//	�\�[�g�����ݒ�@���l�E������
	m_SudokuGrid.SetCompareFunction(CGridCtrl::pfnCellNumericCompare);

//	m_pParent = NULL;

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B

//	return TRUE;  // return TRUE unless you set the focus to a control
//	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

void MCmdSuudokuEnd();

void CSuudokuTbl::PostNcDestroy()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B
	if( m_pParent != NULL) {
		MCmdSuudokuEnd();
		delete this;
	}

	CDialog::PostNcDestroy();
}

void CSuudokuTbl::OnClose()
{
	// TODO: �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	if( m_pParent != NULL) {
		DestroyWindow();
	} else {
		CDialog::OnClose();
	}
}

void CSuudokuTbl::OnBnClickedCancel()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	if (m_pParent != NULL) {
		DestroyWindow();
	} else {
		OnCancel();
	}
}

void CSuudokuTbl::OnBnClickedOk()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	if (m_pParent != NULL) {
		DestroyWindow();
	} else {
		OnOK();
	}
}


BOOL CSuudokuTbl::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B

//	return CDialog::Create(lpszTemplateName, pParentWnd);
	return TRUE;
}

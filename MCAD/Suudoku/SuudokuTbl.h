#pragma once
#include "MCAD.h"
#include "d:\mdev\mb_05_2005\mcad\msub\gridctrl\gridctrl.h"


// CSuudokuTbl ダイアログ

class CSuudokuTbl : public CDialog
{
	DECLARE_DYNAMIC(CSuudokuTbl)

public:
	CSuudokuTbl(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CSuudokuTbl();

// ダイアログ データ
	enum { IDD = IDD_SUDOKU };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CWnd* m_pParent;
public:
	BOOL Create(CWnd* pWnd);
	virtual BOOL OnInitDialog();
protected:
	virtual void PostNcDestroy();
	afx_msg void OnClose();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
public:
	CGridCtrl m_SudokuGrid;
protected:
	int m_nFixCols;
	int m_nFixRows;
	int m_nCols;
	int m_nRows;
public:
	BOOL m_bHeaderSort;
public:
	virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
};

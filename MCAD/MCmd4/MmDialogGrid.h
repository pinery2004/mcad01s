#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CMmDialogGrid ダイアログ

class CMmDialogGrid : public CDialog
{
	DECLARE_DYNAMIC(CMmDialogGrid)

public:
	CMmDialogGrid(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CMmDialogGrid();

// ダイアログ データ
	enum { IDD = IDD_DIALOGGRIDNUM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_EditGridNum1;
	CEdit m_EditGridNum2;
	CEdit m_EditGridNum3;
	CEdit m_EditGridNum4;
	CSpinButtonCtrl m_SpinGridNum1;
	CSpinButtonCtrl m_SpinGridNum2;
	CSpinButtonCtrl m_SpinGridNum3;
	CSpinButtonCtrl m_SpinGridNum4;
	afx_msg void OnDeltaposSpingridnum1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpingridnum2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpingridnum3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpingridnum4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtongridnum1();
	afx_msg void OnBnClickedButtongridnum2();
	afx_msg void OnBnClickedButtongridnum4();
	afx_msg void OnBnClickedButtongridnum3();
	afx_msg void OnBnClickedButtongridnum5();
	void GetSpinGridNum(MINT * nGrid);
	void SetSpinGridNum(MINT * nGrid);
	void SetGridNum( MINT * nGrid);
protected:
	virtual void PostNcDestroy();
public:
	afx_msg void OnClose();
public:
	CWnd *m_pParent;
public:
	BOOL Create(CWnd * pWnd);
public:
	virtual BOOL OnInitDialog();
};

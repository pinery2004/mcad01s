#pragma once
#include "afxwin.h"


// CMmDialogHist ダイアログ

class CMmDialogHist : public CDialog
{
	DECLARE_DYNAMIC(CMmDialogHist)

public:
	CMmDialogHist(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CMmDialogHist();

// ダイアログ データ
	enum { IDD = IDD_DIALOGHIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CWnd *m_pParent;
	MINT m_nVer;
	CListBox m_listHistC;
	CListBox m_listHistB;
	CListBox m_listHistD;
	CEdit m_edt_Ver_C;
	CEdit m_edt_Ver_B;
	CString *m_psVerC;
	CString *m_psVerB;
	CString *m_psVerD;
public:
	afx_msg void OnClose();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonv1();
	afx_msg void OnBnClickedButtonv2();
	afx_msg void OnBnClickedButtonv3();
protected:
	virtual void PostNcDestroy();
public:
	BOOL Create(CWnd * pWnd);
public:
//	virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

#pragma once
#include "afxwin.h"


// CMmDialogOpt ダイアログ

class CMmDialogOpt : public CDialog
{
	DECLARE_DYNAMIC(CMmDialogOpt)

public:
	CMmDialogOpt(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CMmDialogOpt();

// ダイアログ データ
	enum { IDD = IDD_DIALOGOPTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
public:
	CWnd *m_pParent;

	DECLARE_MESSAGE_MAP()
protected:
	virtual void PostNcDestroy();
public:
	afx_msg void OnClose();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCheckoption0();						// 標準費用時オフ
	afx_msg void OnBnClickedCheckoption1();						// オプション　１
	afx_msg void OnBnClickedCheckoption2();						// オプション　２
	afx_msg void OnBnClickedCheckoption3();						// オプション　３
	afx_msg void OnBnClickedCheckoption4();						// オプション　４
	afx_msg void OnBnClickedCheckoption5();						// オプション　５
	afx_msg void OnBnClickedCheckoption6();						// オプション　６
	afx_msg void OnBnClickedCheckoption7();						// オプション　７
	afx_msg void OnBnClickedCheckoption8();						// オプション　８
	afx_msg void OnBnClickedCheckoption9();						// オプション　９
	afx_msg void OnBnClickedCheckoption10();					// オプション１０
	virtual BOOL OnInitDialog();
	CButton m_chkBox0;											// 標準費用時オフ
	CButton m_chkBox1;											// オプション　１
	CButton m_chkBox2;											// オプション　２
	CButton m_chkBox3;											// オプション　３
	CButton m_chkBox4;											// オプション　４
	CButton m_chkBox5;											// オプション　５
	CButton m_chkBox6;											// オプション　６
	CButton m_chkBox7;											// オプション　７
	CButton m_chkBox8;											// オプション　８
	CButton m_chkBox9;											// オプション　９
	CButton m_chkBox10;											// オプション１０
public:
	BOOL Create(CWnd* pWnd);
public:
//	virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

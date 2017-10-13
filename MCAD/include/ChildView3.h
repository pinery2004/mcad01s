#pragma once


// CChildView3

class CChildView3 : public CWnd
{
	DECLARE_DYNAMIC(CChildView3)

public:
	CChildView3();
	virtual ~CChildView3();

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg void OnPaint();
//	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
//	afx_msg void OnClose();
};

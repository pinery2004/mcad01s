#pragma once


// CChildView4

class CChildView4 : public CWnd
{
	DECLARE_DYNAMIC(CChildView4)

public:
	CChildView4();
	virtual ~CChildView4();

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg void OnPaint();
//	afx_msg void OnClose();
};

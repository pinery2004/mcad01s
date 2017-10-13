#pragma once

#include "WrapperView.h"

// CChildView5

class CChildView5 : public CWrapperView								// PrintPreview
{
//	DECLARE_DYNAMIC(CChildView5)
	DECLARE_DYNCREATE(CChildView5)									// PrintPreview

public:
	CChildView5();
	virtual ~CChildView5();

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnDraw(CDC* pDC);									// PrintPreview

public:
	afx_msg void OnPaint();
//	afx_msg void OnSize(UINT nType, int cx, int cy);				//C
//	afx_msg void OnClose();
	afx_msg void OnFilePrint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

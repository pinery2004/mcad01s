#pragma once

#include "M3View.h"

// CChildView2

class CChildView2 : public CWnd
{
	DECLARE_DYNAMIC(CChildView2)

public:
	CChildView2();
	virtual ~CChildView2();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
						DWORD dwStyle, const RECT& rect, 
						CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);

public:
//    BOOL RenderScene( void );
private:
//    BOOL SetDCPixelFormat( HDC hdc);
//    BOOL InitializeOpenGL( void);
//    void ShowError(int e);

    CDC*	m_pDC;
	HGLRC   m_GLRC  ;   // OpenGL Rendering Context

	CPoint	m_iPtLast;											// ç≈èIéwé¶ç¿ïW

	MC::M3View	m_M3View;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
//	afx_msg void OnClose();
};


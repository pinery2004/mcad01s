#pragma once

#include "ChildView5.h"

// CChildFrame5 フレーム 

class CChildFrame5 : public CMDIChildWnd
{
public:
	CChildFrame5();           // 動的生成で使用される protected コンストラクタ
protected:
	DECLARE_DYNCREATE(CChildFrame5)

// 属性
public:

protected:
	bool		m_fPrintPreview;								// PrintPreview
	CChildView5 *m_pView;										// PrintPreview

// 操作
public:

// オーバーライド
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// 実装
public:
	// フレームのクライアント領域用のビュー
	virtual ~CChildFrame5();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// 生成された、メッセージ割り当て関数
protected:
	static CMenu NEAR menu;     // menu

	afx_msg void OnFileClose();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFilePrint();
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, const RECT& rect = rectDefault, CMDIFrameWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	afx_msg void OnClose();
	afx_msg void OnUpdateFilePrintPreview(CCmdUI *pCmdUI);
};

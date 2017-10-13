#pragma once

#include "ChildView5.h"

// CChildFrame5 �t���[�� 

class CChildFrame5 : public CMDIChildWnd
{
public:
	CChildFrame5();           // ���I�����Ŏg�p����� protected �R���X�g���N�^
protected:
	DECLARE_DYNCREATE(CChildFrame5)

// ����
public:

protected:
	bool		m_fPrintPreview;								// PrintPreview
	CChildView5 *m_pView;										// PrintPreview

// ����
public:

// �I�[�o�[���C�h
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// ����
public:
	// �t���[���̃N���C�A���g�̈�p�̃r���[
	virtual ~CChildFrame5();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
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

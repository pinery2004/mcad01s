#pragma once

#include "ChildView4.h"

// CChildFrame4 �t���[�� 

class CChildFrame4 : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame4)
public:
	CChildFrame4();           // ���I�����Ŏg�p����� protected �R���X�g���N�^

// ����
public:

// ����
public:

// �I�[�o�[���C�h
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// ����
public:
	// �t���[���̃N���C�A���g�̈�p�̃r���[
	CChildView4 m_wndView;
	virtual ~CChildFrame4();
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
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, const RECT& rect = rectDefault, CMDIFrameWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	afx_msg void OnClose();
};

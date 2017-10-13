// MainFrm.h : CMainFrame �N���X�̃C���^�[�t�F�C�X
//


#pragma once
#include "afxext.h"
class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// ����
public:

// ����
public:

// �I�[�o�[���C�h
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ����
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

//protected:  // �R���g���[�� �o�[�p�����o
public:		  // �R���g���[�� �o�[�p�����o
	CStatusBar  m_wndStatusBar;									// �X�e�C�^�X�o�[		�������i
	CToolBar    m_wndToolBar1;									// ���C�� �c�[���o�[	�㕔��i��
	CToolBar    m_wndToolBar2;									// �T�u �c�[���o�[		�E

	CDialogBar  m_wndDlgBar1;									// INPUT �_�C�A���O�o�[ �㕔���i��
	CDialogBar  m_wndDlgBar2;									// DISP  �_�C�A���O�o�[ ������i��
	CDialogBar	m_wndDlgBar3;									// PARTS �_�C�A���O�o�[ �㕔���i�E
	CDialogBar	m_wndDlgBar4;									// PANEL �_�C�A���O�o�[ �㕔��i�E
	CDialogBar	m_wndDlgBar5;									// ATTR  �_�C�A���O�o�[ ������i�E
	CDialogBar	m_wndDlgBar6;									// �݌v  �_�C�A���O�o�[ �㕔���i�E
	
	MINT		m_iCombo1;										// ���iID
	MINT		m_iCombo2;										// ���@�^��
	MINT		m_iComboInp1;									// ���͓_�敪
	MINT		m_iComboInp2;									// �ۂ߃R�[�h
	MINT		m_iComboInp3;									// �z�u�R�[�h
	MINT		m_iComboPanelNo;								// �p�l���ԍ�

	MINT		m_iSentaku;

// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
public:
	void OnView1On();
	void OnView2On();
	void OnView3On();
	void OnView4On();
	void OnView5On();
	afx_msg void OnClose();
	class CWnd*				m_pCurView;
	class CMDIChildWnd*		m_pCurFrame;
//G	MINT					m_nOpenViews;
public:
	//*** Read and save the alias registry info.
//	void LoadAliases();
//	void SaveAliasesToReg();

	afx_msg void OnCbnSelchangeCombo1();						// ���ޖ�
	afx_msg void OnCbnSelchangeCombo2();						// ���@�^��
	afx_msg void OnCbnSelchangeCombo11();						// ���͓_�敪
	afx_msg void OnCbnSelchangeCombo12();						// �ۂ߃R�[�h
	afx_msg void OnCbnSelchangeCombo13();						// �z�u�R�[�h
	afx_msg void OnCbnSelchangeComboPanelNo();					// �p�l���ԍ�
	afx_msg void OnCbnSelchangeComboAttr1();					// �����␳1
	afx_msg void OnCbnSelchangeComboAttr2();					// �����␳2
	afx_msg void OnCbnSelchangeComboAttr3();					// �ގ��c�����
	afx_msg void OnCbnSelchangeComboAttr4();					// �z�u�X�����
	afx_msg void OnCbnSelchangeComboAttr5();					// ���t������(TORIZ)
	afx_msg void OnCbnSelchangeComboAttr6();					// �J��������(ROH)

	void SetCombo1( MINT iCombo1);
	void SetCombo2( MINT iCombo2);
	void SetComboInp1( MINT iCombo11);
	void SetComboInp2( MINT iCombo12);
	void SetComboInp3( MINT iCombo13);
//	void SetComboPanelNo( MINT iComboPanelNo);

	friend CComboBox* MmpComboCdBuzai();
	friend CComboBox* MmpComboMbr();
	friend CComboBox* MmpComboInpTp();
	friend CComboBox* MmpComboMarumeCd();
	friend CComboBox* MmpComboPlcCd();
	friend CComboBox* MmpComboPanelNo();
};

extern CMainFrame *ms_pMainFrame;								// ���C���t���[�� �|�C���^ (�O���[�o���ϐ�)


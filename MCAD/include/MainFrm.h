// MainFrm.h : CMainFrame クラスのインターフェイス
//


#pragma once
#include "afxext.h"
class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// 属性
public:

// 操作
public:

// オーバーライド
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 実装
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

//protected:  // コントロール バー用メンバ
public:		  // コントロール バー用メンバ
	CStatusBar  m_wndStatusBar;									// ステイタスバー		下部下段
	CToolBar    m_wndToolBar1;									// メイン ツールバー	上部上段左
	CToolBar    m_wndToolBar2;									// サブ ツールバー		右

	CDialogBar  m_wndDlgBar1;									// INPUT ダイアログバー 上部下段左
	CDialogBar  m_wndDlgBar2;									// DISP  ダイアログバー 下部上段左
	CDialogBar	m_wndDlgBar3;									// PARTS ダイアログバー 上部下段右
	CDialogBar	m_wndDlgBar4;									// PANEL ダイアログバー 上部上段右
	CDialogBar	m_wndDlgBar5;									// ATTR  ダイアログバー 下部上段右
	CDialogBar	m_wndDlgBar6;									// 設計  ダイアログバー 上部下段右
	
	MINT		m_iCombo1;										// 部品ID
	MINT		m_iCombo2;										// 寸法型式
	MINT		m_iComboInp1;									// 入力点区分
	MINT		m_iComboInp2;									// 丸めコード
	MINT		m_iComboInp3;									// 配置コード
	MINT		m_iComboPanelNo;								// パネル番号

	MINT		m_iSentaku;

// 生成された、メッセージ割り当て関数
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

	afx_msg void OnCbnSelchangeCombo1();						// 部材名
	afx_msg void OnCbnSelchangeCombo2();						// 寸法型式
	afx_msg void OnCbnSelchangeCombo11();						// 入力点区分
	afx_msg void OnCbnSelchangeCombo12();						// 丸めコード
	afx_msg void OnCbnSelchangeCombo13();						// 配置コード
	afx_msg void OnCbnSelchangeComboPanelNo();					// パネル番号
	afx_msg void OnCbnSelchangeComboAttr1();					// 長さ補正1
	afx_msg void OnCbnSelchangeComboAttr2();					// 長さ補正2
	afx_msg void OnCbnSelchangeComboAttr3();					// 材軸芯ずれ量
	afx_msg void OnCbnSelchangeComboAttr4();					// 配置店ずれ量
	afx_msg void OnCbnSelchangeComboAttr5();					// 取り付け高さ(TORIZ)
	afx_msg void OnCbnSelchangeComboAttr6();					// 開口部高さ(ROH)

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

extern CMainFrame *ms_pMainFrame;								// メインフレーム ポインタ (グローバル変数)


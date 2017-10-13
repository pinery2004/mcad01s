// MCADApp.h : MCAD �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//
#pragma once

#include "MgDefine.h"
#include "MmDefine.h"

#ifndef __AFXWIN_H__
	#error ���̃t�@�C���� PCH �Ɋ܂߂�O�ɁA'stdafx.h' ���܂߂Ă��������B
#endif

// CMCADApp:
// ���̃N���X�̎����ɂ��ẮAMCAD.cpp ���Q�Ƃ��Ă��������B
//

class CMCADApp : public CWinApp
{
public:
	CMCADApp();


// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ����
protected:
	HMENU m_hMDIMenu;
	HACCEL m_hMDIAccel;
	CMDIChildWnd *m_pChildFrame1;
	CMDIChildWnd *m_pChildFrame2;
	CMDIChildWnd *m_pChildFrame3;
	CMDIChildWnd *m_pChildFrame4;
	CMDIChildWnd *m_pChildFrame5;

// �\�����͗p
	MINT m_iKInpSel;										// �\�����͑I��
	bool m_bPtsListBuzai;										// ���i�\�p	���ވꗗ�@�`�F�b�N�{�b�N�X
	bool m_bPtsListKanagu;									// ���i�\�p	�����ꗗ�@�`�F�b�N�{�b�N�X
	bool m_bPtsListGohan;										// ���i�\�p	���ꗗ�@�`�F�b�N�{�b�N�X

public:
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	DECLARE_MESSAGE_MAP();
	afx_msg void OnView1On();
	afx_msg void OnView2On();
	afx_msg void OnView3On();
	afx_msg void OnView4On();
	afx_msg void OnView5On();
	afx_msg void OnView1Off();
	afx_msg void OnView2Off();
	afx_msg void OnView3Off();
	afx_msg void OnView4Off();
	afx_msg void OnView5Off();

	void CreateWnd(
				MINT		iWndSyu,							// (I  ) �E�B���h�E���
				MINT		iZuSyu,								// (I  ) �}���
				MINT		iKai,								// (I  ) �K
				MINT		iZuNum								// (I  ) �}�ԍ�
				);
	afx_msg void OnMainMenuP( UINT nID);
	afx_msg void OnUpdateMainMenuP(CCmdUI *pCmdUI);
	afx_msg void OnSubMenuP( UINT nID);
	afx_msg void OnUpdateSubMenuP(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedRadioYane();
	afx_msg void OnUpdateRadioYane(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedRadioTenjo();
	afx_msg void OnUpdateRadioTenjo(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedRadioTairyoku();
	afx_msg void OnUpdateRadioTairyoku(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedRadioKabe();
	afx_msg void OnUpdateRadioKabe(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedRadioYuka();
	afx_msg void OnUpdateRadioYuka(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedRadioKoya();
	afx_msg void OnUpdateRadioKoya(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedRadioDodai();
	afx_msg void OnUpdateRadioDodai(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedRadioKiso();
	afx_msg void OnUpdateRadioKiso(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedRadioPts();
	afx_msg void OnUpdateRadioPts(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedRadioKanagu();
	afx_msg void OnUpdateRadioKanagu(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedRadioPanel();
	afx_msg void OnUpdateRadioPanel(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedRadioOther();
	afx_msg void OnUpdateRadioOther(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedRadioSel1();
	afx_msg void OnUpdateRadioSel1(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedRadioSel2();
	afx_msg void OnUpdateRadioSel2(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedRadioSel3();
	afx_msg void OnUpdateRadioSel3(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedChukiDelete();
	afx_msg void OnUpdateChukiDelete(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedChukiMove();
	afx_msg void OnUpdateChukiMove(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedChukiChange();
	afx_msg void OnUpdateChukiChange(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedCheckBuzai();
	afx_msg void OnUpdateCheckBuzai(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedCheckKanagu();
	afx_msg void OnUpdateCheckKanagu(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedCheckGohan();
	afx_msg void OnUpdateCheckGohan(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedCheckAttr1();
	afx_msg void OnUpdateCheckAttr1(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedCheckAttr2();
	afx_msg void OnUpdateCheckAttr2(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedCheckAttr3();
	afx_msg void OnUpdateCheckAttr3(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedCheckAttr4();
	afx_msg void OnUpdateCheckAttr4(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedRadio1f();
	afx_msg void OnUpdateRadio1f(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedRadio2f();
	afx_msg void OnUpdateRadio2f(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedRadio3f();
	afx_msg void OnUpdateRadio3f(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedPanelBefore();
	afx_msg void OnUpdatePanelBefore(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedPanelNext();
	afx_msg void OnUpdatePanelNext(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedPanelAll();
	afx_msg void OnUpdatePanelAll(CCmdUI *pCmdUI);
	afx_msg void OnFileOpen();
	afx_msg void OnUpdateFileOpen(CCmdUI *pCmdUI);
	afx_msg void OnFileSave();
	afx_msg void OnUpdateFileSave(CCmdUI *pCmdUI);
	afx_msg void OnFileSaveAs();
	afx_msg void OnUpdateFileSaveAs(CCmdUI *pCmdUI);
	afx_msg void OnFilePrint();
	afx_msg void OnUpdateFilePrint(CCmdUI *pCmdUI);
	afx_msg void OnFilePrintPreview();
	afx_msg void OnUpdateFilePrintPreview(CCmdUI *pCmdUI);
	afx_msg void OnFilePrintSetup();
	afx_msg void OnUpdateFilePrintSetup(CCmdUI *pCmdUI);
	afx_msg void OnAppExit();
public:
	afx_msg void OnIsyou();
	afx_msg void OnUpdateIsyou(CCmdUI *pCmdUI);
	afx_msg void OnKouzou();
	afx_msg void OnUpdateKouzou(CCmdUI *pCmdUI);
public:
	afx_msg void OnBnClickedRadioJyuuko();
	afx_msg void OnUpdateRadioJyuuko(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedRadioJyuuto();
	afx_msg void OnUpdateRadioJyuuto(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedRadioBukken();
	afx_msg void OnUpdateRadioBukken(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedRadioSYane();
	afx_msg void OnUpdateRadioSYane(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedRadioSikiti();
	afx_msg void OnUpdateRadioSikiti(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedRadioHekiryou();
	afx_msg void OnUpdateRadioHekiryou(CCmdUI *pCmdUI);
};

//extern CMCADApp g_MCADApp;
//extern CMCADApp* ms_pMCADApp;


//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MmMainframe.cpp
//
//		メインフレーム管理
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"
#include "MgLib.h"
#include "MmDefine.h"
#define DLL_EXPORT_MC_SYSTEM_DO
#include "MmLib.h"
#include "MmSystemProperty.h"

#include "MmCmd.h"
#include "resource.h"
#include "..\\MCAD\\resource.h"

#include "MCADApp.h"
#include "MainFrm.h"


class CMCADApp*		_System::ms_pMCADApp;		// アプリケーションポインタ 
class CMainFrame*	_System::ms_pMainFrame;		// メインフレームポインタ 

////namespace MC
////{
////// MainFrame Pointer 設定
////void	MC::System::MmMainFrame( CMainFrame* pMainFrame)
////{
////	System::GetpMainFrame = pMainFrame;
////}
////
////// MainFrame Pointer 取得
////CMainFrame* MC::System::MmGetpMainFrame()
////{
////	return System::GetpMainFrame;
////}
////
////// MCADApp Pointer 設定
////void	MC::System::MmMCADApp( CMCADApp* pMCADApp)
////{
////	ms_pMCADApp = pMCADApp;
////}
////
////// MCADApp Pointer 取得
////CMCADApp* MC::System::MmGetpMCADApp()
////{
////	return ms_pMCADApp;
////}
////
////}

// MainFrm.cpp : CMainFrame クラスの実装
//

// CMainFrame 診断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump( CDumpContext& dc) const
{
	CMDIFrameWnd::Dump( dc);
}

#endif //_DEBUG


// CMainFrame メッセージ ハンドラ


// PARTS ダイアログバー　部材名

void CMainFrame::SetCombo1( MINT iCombo1)
{
	m_iCombo1 = iCombo1;
	((CComboBox*)(m_wndDlgBar3.GetDlgItem(IDC_CMB_BZI1)))->SetCurSel( m_iCombo1);
}

void CMainFrame::SetCombo2( MINT iCombo2)
{
	m_iCombo2 = iCombo2;
	((CComboBox*)(m_wndDlgBar3.GetDlgItem(IDC_CMB_BZI2)))->SetCurSel( m_iCombo2);
}


void CMainFrame::SetComboInp1( MINT iCombo11)
{
	m_iComboInp1 = iCombo11;
	((CComboBox*)(m_wndDlgBar1.GetDlgItem(IDC_CMB_Inp1)))->SetCurSel( m_iComboInp1);
}

void CMainFrame::SetComboInp2( MINT iCombo12)
{
	m_iComboInp2 = iCombo12;
	((CComboBox*)(m_wndDlgBar1.GetDlgItem(IDC_CMB_Inp2)))->SetCurSel( m_iComboInp2);
}

void CMainFrame::SetComboInp3( MINT iCombo13)
{
	m_iComboInp3 = iCombo13;
	((CComboBox*)(m_wndDlgBar1.GetDlgItem(IDC_CMB_Inp3)))->SetCurSel( m_iComboInp3);
}


CComboBox* MmpComboCdBuzai()
{
	return (CComboBox*)( MC::System::GetpMainFrame()->m_wndDlgBar3.GetDlgItem(IDC_CMB_BZI1));
}

CComboBox* MmpComboMbr()
{
	return (CComboBox*)( MC::System::GetpMainFrame()->m_wndDlgBar3.GetDlgItem(IDC_CMB_BZI2));
}

CComboBox* MmpComboInpTp()
{
	return (CComboBox*)( MC::System::GetpMainFrame()->m_wndDlgBar1.GetDlgItem(IDC_CMB_Inp1));
}

CComboBox* MmpComboCdMarume()
{
	return (CComboBox*)( MC::System::GetpMainFrame()->m_wndDlgBar1.GetDlgItem(IDC_CMB_Inp2));
}

CComboBox* MmpComboCdPlc()
{
	return (CComboBox*)( MC::System::GetpMainFrame()->m_wndDlgBar1.GetDlgItem(IDC_CMB_Inp3));
}

CComboBox* MmpComboPanelNo()
{
	return (CComboBox*)( MC::System::GetpMainFrame()->m_wndDlgBar4.GetDlgItem(IDC_COMBOPANELNO));
}

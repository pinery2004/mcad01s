#pragma once

#include "MmDefine.h"

#ifdef DLL_EXPORT
	#undef DLL_EXPORT
#endif

#ifdef DLL_EXPORT_MC_SYSTEM_DO
	#pragma message( "<<< MC::System・dll_EXport >>>")
	#define DLL_EXPORT							__declspec( dllexport)
#else
	#ifdef DLL_NO_IMPORT_MD_DO
		#define DLL_EXPORT
	#else
//		#pragma message( "=== MC::System・dll_IMport ===")
		#define DLL_EXPORT						__declspec( dllimport)
	#endif
#endif

class _System
{
private:
	static class CMCADApp	*ms_pMCADApp;		// アプリケーションポインタ(Global) 
	static class CMainFrame	*ms_pMainFrame;		// メインフレームポインタ(Global) 

public:
	/////////////////////////////////////////////////////////////////////////////
	//	システム管理                                                           //
	/////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////
	// MainFrame Pointer 設定
	// class CMainFrame;
	static void	SetpMainFrame(
					class	CMainFrame*	pMainFrame
					)
	{
		ms_pMainFrame = pMainFrame;
	}

	/////////////////////////////////////////////////////////////////////////////
	// MainFrame Pointer 取得
	static CMainFrame* GetpMainFrame()
	{
		return ms_pMainFrame;
	}

	/////////////////////////////////////////////////////////////////////////////
	// MCADApp Pointer 設定
	// class CMCADApp;
	static void	SetpMCADApp(
					class	CMCADApp*	pMCADApp
					)
	{
		ms_pMCADApp = pMCADApp;
	}

	/////////////////////////////////////////////////////////////////////////////
	// MCADApp Pointer 取得
	static CMCADApp* GetpMCADApp()
	{
		return ms_pMCADApp;
	}

};

namespace MC
{

class DLL_EXPORT System
{
private:

public:
	/////////////////////////////////////////////////////////////////////////////
	//	システム管理                                                           //
	/////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////
	//	システム初期化
	static MINT MmInitialize();

	/////////////////////////////////////////////////////////////////////////////
	//	システム終了処理
	static void MmTerminate();

	/////////////////////////////////////////////////////////////////////////////
	//	MCADINIファイルの読み込み
	static void LoadMCADIniFile( MCHAR i_cIniFilePath[]);

	/////////////////////////////////////////////////////////////////////////////
	// MainFrame Pointer 設定
	// class CMainFrame;
	static void	SetpMainFrame(
					class	CMainFrame*	pMainFrame
					)
	{
		::_System::SetpMainFrame( pMainFrame);
	}

	/////////////////////////////////////////////////////////////////////////////
	// MainFrame Pointer 取得
	static CMainFrame* GetpMainFrame()
	{
		return ::_System::GetpMainFrame();
	}

	/////////////////////////////////////////////////////////////////////////////
	// MCADApp Pointer 設定
	//class CMCADApp;
	static void	SetpMCADApp(
					class	CMCADApp*	pMCADApp
					)
	{
		::_System::SetpMCADApp( pMCADApp);
	}

	/////////////////////////////////////////////////////////////////////////////
	// MCADApp Pointer 取得
	static CMCADApp* GetpMCADApp()
	{
		return ::_System::GetpMCADApp();
	}

};


}// namespace MC
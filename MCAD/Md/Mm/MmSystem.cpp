//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MmDrag.cpp
//
//		ドラッギング操作
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"
#include "MgLib.h"
#include "MgMat.h"
#include "MbDefine.h"
#include "MbCod.h"
#include "MmGridNum.h"
#include "MmDrag.h"
#include "MmWnd.h"
#include "MmDefine.h"

#define	DLL_EXPORT_MC_SYSTEM_DO
#include "MmLib.h"
#include "MdModel.h"
#include "Mdm.h"

#include "MmSystemProperty.h"

#include "MmCmd.h"
#include "resource.h"

#include "MsBitSet.h"
#include "MmHist.h"

#include "MmOpt.h"
#include "MmTenkai.h"
#include "MmSystemProperty.h"
#include "MhLib.h"

#include "MbDebug.h"

#define MC_SZ_CMBATTR	6
#define MC_SZ_CHKATTR	4

namespace MC
{
//===========================================================================
//				システム初期化　(固有部分)
//
//
//===========================================================================

MINT System::MmInitialize()
{
	MINT	ist;

	// 日本語モードの設定
	//							setlocale(LC_ALL, "jpn");
	Base::MbSetJapanese();

	// ルートディレクトリの設定
	// システムのプロパティをイニシャル設定する
	MmSystemProperty::Init();

	//
	Msg::ClearErrorMsg();

	// 全カーソルのＩＤを登録する 
	//	MINT	nIDCURSOR = 5;
	//	MINT	IDCURSOR[] = {IDC_CROSS1, IDC_CROSS2, IDC_CROSS3, IDC_ZOOM, IDC_GRAB};

	// MCAD.ini ファイルの読み込み
	MCHAR	cIniFilePath[MAX_PATH];
	MmSystemProperty::GetEnvPath( MP_PATH_ROOT, MmSystemProperty::GetStr( MM_STR_FILE_INI), cIniFilePath);
	LoadMCADIniFile( cIniFilePath);

	// Logファイルの削除とオープン
	MCHAR	cLogFilePath[MAX_PATH];
	MmSystemProperty::GetEnvPath( MP_PATH_LOG, MmSystemProperty::GetStr( MM_STR_FILE_LOG), cLogFilePath);
	Trace::OpenLogFile( cLogFilePath);
	
	// DBを初期化する
	Mdm::MdmInitialize();

	// 部材マスタを読み込む
	MhHist::MmHistInit();
	MhOptV::MmOptInit();
	JTTenkai::InitJTT();
	IeModel::MhInitPts();

	// 入力属性値を初期化する
	IeModel::MhInitInpAt();

	// トレース処理のファイルオープン
	//MBTRCOPEN( Mstr( "C:/temp/MCadDebugTrace.txt"));
	MCHAR	cTraceFilePath[MAX_PATH];
	MmSystemProperty::GetEnvPath( MP_PATH_LOG, MmSystemProperty::GetStr(MM_STR_FILE_TRACE), cTraceFilePath);
	MBTRCOPEN( cTraceFilePath);

	// メッセージの出力先設定
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);

//	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);


	ist = 0;
	return ist;
}

void System::MmTerminate()
{
	// DBの解放
	Mdm::MdmTerminate();

	// トレース処理のファイルクローズ
	MBTRCCLOSE;
}

/////////////////////////////////////////////////////////////////////////////
//	MCADINIファイルの読み込み
void System::LoadMCADIniFile( MCHAR i_cIniFilePath[])
{
}

} // namespace MC
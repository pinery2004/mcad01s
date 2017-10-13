//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MmDrag.cpp
//
//		�h���b�M���O����
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
//				�V�X�e���������@(�ŗL����)
//
//
//===========================================================================

MINT System::MmInitialize()
{
	MINT	ist;

	// ���{�ꃂ�[�h�̐ݒ�
	//							setlocale(LC_ALL, "jpn");
	Base::MbSetJapanese();

	// ���[�g�f�B���N�g���̐ݒ�
	// �V�X�e���̃v���p�e�B���C�j�V�����ݒ肷��
	MmSystemProperty::Init();

	//
	Msg::ClearErrorMsg();

	// �S�J�[�\���̂h�c��o�^���� 
	//	MINT	nIDCURSOR = 5;
	//	MINT	IDCURSOR[] = {IDC_CROSS1, IDC_CROSS2, IDC_CROSS3, IDC_ZOOM, IDC_GRAB};

	// MCAD.ini �t�@�C���̓ǂݍ���
	MCHAR	cIniFilePath[MAX_PATH];
	MmSystemProperty::GetEnvPath( MP_PATH_ROOT, MmSystemProperty::GetStr( MM_STR_FILE_INI), cIniFilePath);
	LoadMCADIniFile( cIniFilePath);

	// Log�t�@�C���̍폜�ƃI�[�v��
	MCHAR	cLogFilePath[MAX_PATH];
	MmSystemProperty::GetEnvPath( MP_PATH_LOG, MmSystemProperty::GetStr( MM_STR_FILE_LOG), cLogFilePath);
	Trace::OpenLogFile( cLogFilePath);
	
	// DB������������
	Mdm::MdmInitialize();

	// ���ރ}�X�^��ǂݍ���
	MhHist::MmHistInit();
	MhOptV::MmOptInit();
	JTTenkai::InitJTT();
	IeModel::MhInitPts();

	// ���͑����l������������
	IeModel::MhInitInpAt();

	// �g���[�X�����̃t�@�C���I�[�v��
	//MBTRCOPEN( Mstr( "C:/temp/MCadDebugTrace.txt"));
	MCHAR	cTraceFilePath[MAX_PATH];
	MmSystemProperty::GetEnvPath( MP_PATH_LOG, MmSystemProperty::GetStr(MM_STR_FILE_TRACE), cTraceFilePath);
	MBTRCOPEN( cTraceFilePath);

	// ���b�Z�[�W�̏o�͐�ݒ�
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
	// DB�̉��
	Mdm::MdmTerminate();

	// �g���[�X�����̃t�@�C���N���[�Y
	MBTRCCLOSE;
}

/////////////////////////////////////////////////////////////////////////////
//	MCADINI�t�@�C���̓ǂݍ���
void System::LoadMCADIniFile( MCHAR i_cIniFilePath[])
{
}

} // namespace MC
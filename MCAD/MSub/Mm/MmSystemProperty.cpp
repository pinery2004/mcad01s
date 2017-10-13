//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MmSystemProperty.cpp
//
//		�V�X�e���v���p�e�B
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"
#include "MmDefine.h"

#define	DLL_EXPORT_SYSTEMPROPERTY_DO
#include "MmSystemProperty.h"

namespace MC
{

//S MmSystemProperty	g_SysProp;
//	�V�X�e���萔
DWORD	MmSystemProperty::m_Color[MM_N_COLOR];					// �F
MREAL	MmSystemProperty::m_Real[MM_N_REAL];					// ����
MINT	MmSystemProperty::m_Int[MM_N_INT];						// ����
MCHAR*	MmSystemProperty::m_Str[MM_N_STR];						// ������
MCHAR*	MmSystemProperty::m_Path[MM_N_PATH];					// PATH
MREAL	MmSystemProperty::m_Stnd[MMAX_KAI][MM_N_STNDH];			// ������A�����@

/////////////////////////////////////////////////////////////////////////////
//	�V�X�e���萔�̏������@(�ŗL����)
void	MmSystemProperty::Init()
{
	m_Color[MM_COLOR_GRID_LINE]			= RGB( 110, 180, 150);	// �O���b�h���\���F
	m_Color[MM_COLOR_GRID_TEXT]			= RGB(  50, 100,  60);	// �O���b�h�ԍ��\���F
	m_Color[MM_COLOR_MARUME]			= RGB( 255,   0,   0);	// �ۂߐ��\���F
	m_Color[MM_COLOR_CURSOR]			= RGB( 245, 150, 100);	// �N���X�w�A�[�J�[�\���F
	m_Color[MM_COLOR_DRAGGING]			= RGB(  20, 220, 245);	// �h���b�M���O�F
	m_Color[MM_COLOR_HITBZI]			= RGB( 255, 255, 255);	// �q�b�g���ސF
	m_Color[MM_COLOR_TEMP]				= RGB( 120,  20, 125);	// �e���|�����I��\���F
	m_Color[MM_COLOR_BACKGROUND]		= RGB( 252, 252, 252);	// �w�i�F
//
	m_Color[MM_COLOR_DOUKAIKABESIN]  	= RGB( 25, 25, 205);	// ���K�ǐc�F
	m_Color[MM_COLOR_KAKAIKABESIN]	 	= RGB( 185, 75, 15);	// ���K�ǐc��F
	m_Color[MM_COLOR_JYOUKAIKABESIN] 	= RGB( 205, 15, 15);	// ��K�ǐc���F
	m_Color[MM_COLOR_DOUKAITAIKABESIN]  = RGB( 25, 25, 205);	// ���K�ϗ͕ǐc�F
	m_Color[MM_COLOR_KAKAITAIKABESIN]	= RGB( 185, 75, 15);	// ���K�ϗ͕ǐc��F
	m_Color[MM_COLOR_JYOUKAITAIKABESIN] = RGB( 205, 15, 15);	// ��K�Ǒϗ͐c���F
	m_Color[MM_COLOR_DOUKAISIJIKABESIN] = RGB( 25, 25, 205);	// ���K�x���ǐc�F
	m_Color[MM_COLOR_KAKAISIJIKABESIN]	= RGB( 185, 75, 15);	// ���K�x���ǐc��F
	m_Color[MM_COLOR_JYOUKAISIJIKABESIN]= RGB( 205, 15, 15);	// ��K�x���ǐc���F
	m_Color[MM_COLOR_KISOSIN]			= RGB( 195, 25, 25);	// ��b�c���F
	m_Color[MM_COLOR_KABESEN]			= RGB( 185, 205, 145);	// �Ǖ\�����̕ǐ��F
	
	m_Real[MM_REAL_PITCH]			= 910.f;					// �O���b�h�E�s�b�`
	m_Real[MM_REAL_GRID_SPC]		= 7.f;						// �O���b�h�ԍ��\����(�|�C���g)
//	m_Real[MM_REAL_GRID_SPC]		= 14.f;						// �O���b�h�ԍ��\����(�|�C���g)
	m_Real[MM_REAL_GRID_HEI]		= 9.f;						// �O���b�h��������(�|�C���g)
	m_Real[MM_REAL_GRID_AKI]		= 300.f;					// �O���b�h�g�\����

	m_Int[MM_INT_GRID_N]			= 16;						// �����k���O���b�h��
	m_Int[MM_INT_GRID_W]			= -3;						// ���������O���b�h��
	m_Int[MM_INT_GRID_S]			= -3;						// �����쑤�O���b�h��
	m_Int[MM_INT_GRID_E]			= 17;						// ���������O���b�h��
	// �t�H���_
	m_Str[MM_STR_DIR_TABLE]			= Mstr( "�e�[�u��");		// �e�[�u��
	m_Str[MM_STR_DIR_PARTS]			= Mstr( "�e�[�u��\\����");	// �e�[�u��\���i
	m_Str[MM_STR_DIR_ATTR]			= Mstr( "����");			// ����
	m_Str[MM_STR_DIR_TEXTURE]		= Mstr( "�e�N�X�`��");		// �e�N�X�`��
	m_Str[MM_STR_DIR_FIG]			= Mstr( "�}�`");			// �}�` 
	// �t�@�C��
	m_Str[MM_STR_FILE_INI]			= Mstr( "MCAD.ini");		// MCAD.ini 
	m_Str[MM_STR_FILE_LOG]			= Mstr( "MCADLOG.txt");		// LOG 
	m_Str[MM_STR_FILE_TRACE]		= Mstr( "MCADTRACE.txt");	// TRACE 

	m_Str[MM_STR_CSV_MEMBER]		= Mstr( "���@�^��.csv");	// ���@�^��.csv 
	m_Str[MM_STR_CSV_PARTS]			= Mstr( "���i.csv");		// ���i.csv 
	m_Str[MM_STR_CSV_ATTR]			= Mstr( "����.csv");		// ����.csv 
	m_Str[MM_STR_CSV_DRAFTSZ]		= Mstr( "�}�ʃT�C�Y.csv");	// �}�ʃT�C�Y.csv 
	m_Str[MM_STR_CSV_KOUSEI]		= Mstr( "�\��.csv");		// �\��.csv 
	m_Str[MM_STR_CSV_BUNRUI]		= Mstr( "����.csv");		// ����.csv			 
/*
	// ���s���W���[���̃f�B���N�g���擾
	static MCHAR s_cRootDir[MAX_PATH];
	GetModuleFileName( NULL, s_cRootDir, sizeof( s_cRootDir));
	for ( int i1=(int)Mstrlen( s_cRootDir)-1; i1>=0; i1--) {
		if ( s_cRootDir[i1] == '\\') {
			s_cRootDir[i1] = '\0';
			break;
		}
	}
*/
#ifdef _WIN64
	//N	m_Path[MM_PATH_ROOT_FLDR]		= s_cRootDir;								// ���s���W���[����(���[�g)�f�B���N�g��
	//N	m_Path[MM_PATH_PARTS_FLDR]		= Mstr( "D:\\MCAD_64\\�e�[�u��\\����\\");	// �\�����i�e�[�u��
	m_Path[MM_PATH_DIR_SYS_ENV]	= Mstr( "D:\\MCAD_64\\�}�X�^");					// �V�X�e�����f�B���N�g��
	m_Path[MM_PATH_DIR_LOG]		= Mstr( "D:\\MCAD_64\\Log");					// ���O �f�B���N�g��	
#else
	//N	m_Path[MM_PATH_ROOT_FLDR]		= s_cRootDir;								// ���s���W���[����(���[�g)�f�B���N�g��
	//N	m_Path[MM_PATH_PARTS_FLDR]		= Mstr( "D:\\MCAD_32\\�e�[�u��\\����\\");	// �\�����i�e�[�u��
	m_Path[MM_PATH_DIR_SYS_ENV]	= Mstr( "D:\\MCAD_32\\�}�X�^");					// �V�X�e�����f�B���N�g��
	m_Path[MM_PATH_DIR_LOG]		= Mstr( "D:\\MCAD_32\\Log");					// ���O �f�B���N�g��	
#endif

	m_Stnd[0][MM_STNDH_KISO]		= 400.f;			// Kiso
	m_Stnd[0][MM_STNDH_LOWER]		= 645.f;			// Kiso + 245.f (90 + 140 + 15)
	m_Stnd[0][MM_STNDH_UPPER]		= 2740.f;			// rLower + 2095.f
	m_Stnd[0][MM_STNDH_CEILING]		= 3095.f;			// rLower + 2450.f
//	m_Stnd[0][MM_STNDH_ROOF]		= 3184.f;			// rUpper + 89.f
	m_Stnd[0][MM_STNDH_ROOF]		= 3125.f;			// rUpper + 89.f
	m_Stnd[0][MM_STNDA_NOKI]		= 89.f;				// 89.f | 140.f
//	m_Stnd[1][MM_STNDH_KISO]		= 0.f;
	m_Stnd[1][MM_STNDH_LOWER]		= 3354.f;			// rCeiling + 259.f (235 + 24)
	m_Stnd[1][MM_STNDH_UPPER]		= 5449.f;			// rLower + 2095.f
	m_Stnd[1][MM_STNDH_CEILING]		= 5804.f;			// rLower + 2450.f
//	m_Stnd[1][MM_STNDH_ROOF]		= 5893.f;			// rUpper + 89.f
	m_Stnd[1][MM_STNDA_NOKI]		= 89.f;				// 89.f | 140.f
//	m_Stnd[2][MM_STNDH_KISO]		= 0.f;
	m_Stnd[1][MM_STNDH_ROOF]		= 5834.f;			// rUpper + 89.f
	m_Stnd[2][MM_STNDH_LOWER]		= 6063.f;			// rCeiling + 259.f (235 + 24)
	m_Stnd[2][MM_STNDH_UPPER]		= 8158.f;			// rLower + 2095.f
	m_Stnd[2][MM_STNDH_CEILING]		= 8513.f;			// rLower + 2450.f
//	m_Stnd[2][MM_STNDH_ROOF]		= 8602.f;			// rUpper + 89.f
	m_Stnd[2][MM_STNDH_ROOF]		= 8543.f;			// rUpper + 89.f
	m_Stnd[2][MM_STNDA_NOKI]		= 89.f;				// 89.f | 140.f
}

/////////////////////////////////////////////////////////////////////////////
//	�v���p�e�B�l���擾����@(���L����)

/////////////////////////////////////////////////////////////////////////////
//	�V�X�e���F���擾����
DWORD	MmSystemProperty::GetColor( MSYSTEMCOLOR iCol)
{
	return m_Color[iCol];
}

/////////////////////////////////////////////////////////////////////////////
//	�V�X�e�������萔���擾����
MREAL	MmSystemProperty::GetReal( MSYSTEMREAL iReal)
{
	return m_Real[iReal];
}

/////////////////////////////////////////////////////////////////////////////
//	�V�X�e�������萔���擾����
MINT	MmSystemProperty::GetInt( MSYSTEMINT iInt)
{
	return m_Int[iInt];
}

/////////////////////////////////////////////////////////////////////////////
//	�V�X�e��������萔���擾����
MCHAR*	MmSystemProperty::GetStr( MSYSTEMSTR iStr)
{
	return m_Str[iStr];
}

/////////////////////////////////////////////////////////////////////////////
//	PATH�萔���擾����
MCHAR*	MmSystemProperty::GetPath( MSYSTEMPATH iPath)
{
	return m_Path[iPath];
}

/////////////////////////////////////////////////////////////////////////////
//	�V�X�e�����@�t�@�C���p�X�𓾂�
MINT MmSystemProperty::GetEnvPath(				// (  O) �X�e�C�^�X	0:����	!=0:�G���[
						MPPATHENV	i_iType,	// (I  ) ���p�X���
												//		 MP_PATH_ROOT:	���[�g
												//		 MP_PATH_TABLE:	�e�[�u��
												//		 MP_PATH_PARTSTABLE:	���i�e�[�u��
												//		 MP_PATH_ATTR:	����
												//		 MP_PATH_TEX:	�e�N�X�`��(TEXTURE)
												//		 MP_PATH_FIG:	�}�`
												//		 MP_PATH_LOG:	LOG
	MCHAR*		i_cFile,	// (I  ) �t�@�C����
						MCHAR*		o_cPath		// (  O) �t�@�C���p�X��
				)
{
	MINT	ist;

//	Mstrcpy( o_cPath, MmSystemProperty::GetPath( MM_PATH_ROOT_FLDR));
//	Mstrcpy( o_cPath, MmSystemProperty::GetPath( MM_PATH_PARTS_FLDR));
	if (i_iType == MP_PATH_LOG) {
		Mstrcpy(o_cPath, MmSystemProperty::GetPath(MM_PATH_DIR_LOG));		// LOG �f�B���N�g��
	}
	else {
		Mstrcpy(o_cPath, MmSystemProperty::GetPath(MM_PATH_DIR_SYS_ENV));	// �V�X�e�����f�B���N�g�� (�}�X�^)

		MINT nSize = (MINT)Mstrlen(o_cPath);
		if ( o_cPath[nSize - 1] != Mchar('\\'))
			Mstrcat(o_cPath, Mstr("\\"));

		switch (i_iType)
		{
		case MP_PATH_ROOT:
			break;
		case MP_PATH_TABLE:
			Mstrcat(o_cPath, MmSystemProperty::GetStr(MM_STR_DIR_TABLE));	break;
		case MP_PATH_PARTSTABLE:
			Mstrcat(o_cPath, MmSystemProperty::GetStr(MM_STR_DIR_PARTS));	break;
		case MP_PATH_ATTR:
			Mstrcat(o_cPath, MmSystemProperty::GetStr(MM_STR_DIR_ATTR));	break;
		case MP_PATH_TEX:
			Mstrcat(o_cPath, MmSystemProperty::GetStr(MM_STR_DIR_TEXTURE));	break;
		case MP_PATH_FIG:
			Mstrcat(o_cPath, MmSystemProperty::GetStr(MM_STR_DIR_FIG));	break;
		default:
			ist = -1;
			goto exit;
		}
	}
	if ( i_iType != MP_PATH_ROOT) {
		Mstrcat( o_cPath, Mstr( "\\"));
	}
	Mstrcat( o_cPath, i_cFile);

	ist = 0;
exit:
	return ist;
}

/////////////////////////////////////////////////////////////////////////////
//	�������ݒ肷��
void	MmSystemProperty::SetStnd(
						MINT		iKai,				// (I  ) �K�@(1,2,3)
						MSTNDH		iKTCode,			// (I  ) ������R�[�h
						MREAL		rHA					// (I  ) �����(����)

				)
{
	m_Stnd[iKai - 1][iKTCode] = rHA;
}

/////////////////////////////////////////////////////////////////////////////
//	��������擾����
MREAL	MmSystemProperty::GetStnd(						// (  O) �����(����)
						MINT		iKai,				// (I  ) �K�@(1,2,3)
						MSTNDH		iKTCode				// (I  ) ������R�[�h
				)
{
	return m_Stnd[iKai - 1][iKTCode];
}

} // namespace MC
#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MmSystemProperty.
//
//		�V�X�e���v���p�e�B
//
//  K.Matsu			11/01/09    Created.
//==========================================================================================

#ifdef DLL_EXPORT_SYSTEMPROPERTY
	#undef DLL_EXPORT_YSTEMPROPERTY
#endif

#ifdef DLL_EXPORT_SYSTEMPROPERTY_DO
	#pragma message( "<<< MmSystemProperty�Edll_EXport >>>")
	#define DLL_EXPORT_SYSTEMPROPERTY			__declspec( dllexport)
#else
	#ifdef _DEBUG								// Release Mode �� inline�֐���undef�ɂȂ�s��Ή�
		#ifdef DLL_NO_IMPORT_SUB_DO
			#define DLL_EXPORT_SYSTEMPROPERTY
		#else
//			#pragma message( "=== MmSystemProperty�Edll_IMport ===")
			#define DLL_EXPORT_SYSTEMPROPERTY	__declspec( dllimport)
		#endif
	#else
		#define DLL_EXPORT_SYSTEMPROPERTY
	#endif
#endif

namespace MC
{

#define		MMAX_KAI	3

enum	MSYSTEMCOLOR {
	MM_COLOR_GRID_LINE = 0,							// �O���b�h���\���F
	MM_COLOR_GRID_TEXT,								// �O���b�h�ԍ��\���F
	MM_COLOR_MARUME,								// �ۂߐ��\���F
	MM_COLOR_CURSOR,	   							// �N���X�w�A�[�J�[�\���F
	MM_COLOR_DRAGGING,	   							// �h���b�M���O�F
	MM_COLOR_HITBZI,	   							// �q�b�g���ސF
	MM_COLOR_TEMP,		   							// �e���|�����I��\���F
	MM_COLOR_BACKGROUND,   							// �w�i�F
	MM_COLOR_DOUKAIKABESIN,							// ���K�ǐc�F
	MM_COLOR_KAKAIKABESIN,  						// ���K�ǐc��F
	MM_COLOR_JYOUKAIKABESIN,						// ��K�ǐc���F,
	MM_COLOR_DOUKAITAIKABESIN,						// ���K�ϗ͕ǐc�FIN,
	MM_COLOR_KAKAITAIKABESIN,						// ���K�ϗ͕ǐc��FN,
	MM_COLOR_JYOUKAITAIKABESIN,						// ��K�Ǒϗ͐c���FSIN,
	MM_COLOR_DOUKAISIJIKABESIN,						// ���K�x���ǐc�FSIN,
	MM_COLOR_KAKAISIJIKABESIN,						// ���K�x���ǐc��FIN,
	MM_COLOR_JYOUKAISIJIKABESIN,					// ��K�x���ǐc���FESIN,
	MM_COLOR_KISOSIN,	   							// ��b�c���F
	MM_COLOR_KABESEN,	   							// �Ǖ\�����̕ǐ��F			
	MM_N_COLOR
};
enum	MSYSTEMREAL {
	MM_REAL_PITCH = 0,								// �O���b�h�E�s�b�`
	MM_REAL_GRID_SPC,  								// �O���b�h�ԍ��\����(�|�C���g)
	MM_REAL_GRID_HEI,  								// �O���b�h��������(�|�C���g)
	MM_REAL_GRID_AKI,  								// �O���b�h�g�\����
	MM_N_REAL		 
};
enum	MSYSTEMINT {
	MM_INT_GRID_N = 0,								// �����k���O���b�h��
	MM_INT_GRID_W,	  								// ���������O���b�h��
	MM_INT_GRID_S,	  								// �����쑤�O���b�h��
	MM_INT_GRID_E,	  								// ���������O���b�h��
	MM_N_INT
};
enum	MSYSTEMSTR {
	// �t�H���_
	MM_STR_DIR_TABLE = 0,							// �e�[�u��
	MM_STR_DIR_PARTS,								// �e�[�u��\���i
	MM_STR_DIR_ATTR,								// ����
	MM_STR_DIR_TEXTURE,								// �e�N�X�`��
	MM_STR_DIR_FIG,									// �}�` 
	// �t�@�C��
	MM_STR_FILE_INI,								// MCAD.ini 
	MM_STR_FILE_LOG,								// LOG.txt 
	MM_STR_FILE_TRACE,								// TRACE.txt 
	MM_STR_CSV_MEMBER,								// ���@�^��.csv 
	MM_STR_CSV_PARTS,								// ���i.csv 
	MM_STR_CSV_ATTR,								// ����.csv 
	MM_STR_CSV_DRAFTSZ,								// �}�ʃT�C�Y.csv 
	MM_STR_CSV_KOUSEI,								// �\��.csv 
	MM_STR_CSV_BUNRUI,								// ����.csv 
	MM_N_STR
};

enum	MSYSTEMPATH {
//N	MM_PATH_ROOT_FLDR = 0,							// ���s�t�@�C��(���[�g)�f�B���N�g��
//N	MM_PATH_PARTS_FLDR,								// �\�����i�e�[�u��
	MM_PATH_DIR_SYS_ENV = 0,						// �V�X�e�����f�B���N�g��
	MM_PATH_DIR_LOG,								// ���O�f�B���N�g��
	MM_N_PATH
};

//===========================================================================
//		���ϐ�
//
enum MPPATHENV {
	MP_PATH_ROOT = 1,								// ���[�g
	MP_PATH_TABLE,									// �e�[�u��
	MP_PATH_PARTSTABLE,								// �e�[�u��\���i
	MP_PATH_ATTR,									// ����
	MP_PATH_TEX,									// �e�N�X�`��(TEXTURE)
	MP_PATH_FIG,									// �}�`
	MP_PATH_LOG										// LOG, TRACE
};

enum	MSTNDH {
	MM_STNDH_SGL = 0,								//ZZZZZ
	MM_STNDH_KISO,
	MM_STNDH_LOWER,
	MM_STNDH_UPPER,
	MM_STNDH_CEILING,
	MM_STNDH_ROOF,
	MM_STNDA_NOKI,
	MM_N_STNDH
};

/*
class	MmStndH
{
public:
	MREAL	rKiso;
	MREAL	rLower;
	MREAL	rUpper;
	MREAL	rCeiling;
	MREAL	rRoof;
};
*/
class DLL_EXPORT_SYSTEMPROPERTY MmSystemProperty
{
private:
//	�V�X�e���萔
static DWORD	m_Color[MM_N_COLOR];			// �F
static MREAL	m_Real[MM_N_REAL];				// ����
static MINT		m_Int[MM_N_INT];				// ����
static MCHAR*	m_Str[MM_N_STR];				// ������
static MCHAR*	m_Path[MM_N_PATH];				// PATH
static MREAL	m_Stnd[MMAX_KAI][MM_N_STNDH];	// ������A�����@

public:
/////////////////////////////////////////////////////////////////////////////
//	�V�X�e���v���p�e�B�̏�����
static void		MmSystemProperty::Init();

/////////////////////////////////////////////////////////////////////////////
//	�V�X�e���F���擾����
static DWORD	MmSystemProperty::GetColor( MSYSTEMCOLOR iCol);

/////////////////////////////////////////////////////////////////////////////
//	�V�X�e�������萔���擾����
static MREAL	MmSystemProperty::GetReal( MSYSTEMREAL iReal);

/////////////////////////////////////////////////////////////////////////////
//	�V�X�e�������萔���擾����
static MINT		MmSystemProperty::GetInt( MSYSTEMINT iInt);

/////////////////////////////////////////////////////////////////////////////
//	�V�X�e��������萔���擾����
static _TCHAR*	MmSystemProperty::GetStr( MSYSTEMSTR iStr);

/////////////////////////////////////////////////////////////////////////////
//	PATH�萔���擾����
static _TCHAR*	MmSystemProperty::GetPath( MSYSTEMPATH iPath);

//S /////////////////////////////////////////////////////////////////////////////
//	//	�V�X�e���v���p�e�B�f�[�^
//	//static extern	MmSystemProperty	g_SysProp;

/////////////////////////////////////////////////////////////////////////////
//	�V�X�e�����@�t�@�C���p�X�𓾂�
static MINT		MmSystemProperty::GetEnvPath(	// �X�e�C�^�X	0:����	!=0:�G���[
						MPPATHENV	i_iType,	// ���p�X���
												//	MM_STR_DIR_TABLE:	�e�[�u��
												//	MM_STR_DIR_ATTR:	����
												//	MM_STR_DIR_TEXTURE:	�e�N�X�`��(TEXTURE)
												//	MM_STR_DIR_FIG:	�}�`
						MCHAR*		i_pcFile,	// �t�@�C����
						MCHAR*		o_pcPath	// �p�X��
				);

/////////////////////////////////////////////////////////////////////////////
//	�������ݒ肷��
static void		MmSystemProperty::SetStnd(
						MINT		i_iKai,		// �K�@(1,2,3)
						MSTNDH		i_iKTCode,	// ������R�[�h
						MREAL		i_rHA		// �����(����)

				);

/////////////////////////////////////////////////////////////////////////////
//	��������擾����
static MREAL	MmSystemProperty::GetStnd(		// �����(����)
						MINT		i_iKai,		// �K�@(1,2,3)
						MSTNDH		i_iKTCode	// ������R�[�h
				);
};

} // namespace MC
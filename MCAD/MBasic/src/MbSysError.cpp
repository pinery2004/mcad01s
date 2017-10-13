//==========================================================================================
//  Copyright ( C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MbSysErrorMsg.cpp
//
//		�V�X�e���G���[�\��
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#pragma warning( disable : 4996 )
#include "stdafx.h"
#define DLL_EXPORT_BASE_DO
#include "MbBasic.h"
#include "MgDefine.h"
#include "MbFile.h"

namespace MC
{

//===========================================================================
//	MCAD.ini�t�@�C����ǂݍ���
//
//LoadMCADIniFile

//===========================================================================
//	�G���[�\����ɏ������I������
//
void 	Base::MbSysError( 
						const char*	i_cFunction,	// �G���[�֐���
						MINT		i_iErrorCode	// �G���[�R�[�h
				  )
{
	MINT	ist;
	MCHAR	Msg[256];							// ү����

	ist = Code::Mmbstowcs( i_cFunction, Msg, 256);
	Mstrcat( Msg, Mstr( "�@\n\n"));

	switch( i_iErrorCode)
	{
	case MC_ERR_POINTER:
		Mstrcat( Msg, Mstr( "�������擾�G���["));
		break;
	case MC_ERR_OVERFLOW:
		Mstrcat( Msg, Mstr( "�̈�I�[�o�t���[�G���["));
		break;
	case MC_ERR_ID:
		Mstrcat( Msg, Mstr( "�h�c���G���["));
		break;
	case MC_ERR_INPUT:
		Mstrcat( Msg, Mstr( "���̓G���["));
		break;
	case MC_ERR_NUM_OVER:
		Mstrcat( Msg, Mstr( "�����\�������G���["));
		break;
	case MC_ERR_VECT_ZERO:
		Mstrcat( Msg, Mstr( "�[���x�N�g���ŏ����s�G���["));
		break;
	case MC_ERR_INPUT_ZERO:
		Mstrcat( Msg, Mstr( "���l�[���ŏ����s�G���["));
		break;
	case MC_ERR_OFFSET:
		Mstrcat( Msg, Mstr( "�I�t�Z�b�g���G���["));
		break;
	default:
		Mstrcat( Msg, Mstr( "���̑��G���["));
	}		
	Mstrcat( Msg, Mstr( "�@\n\n�V�X�e���I�����܂��@"));

	ist = AfxMessageBox( Msg, MB_ICONSTOP);
	__debugbreak();
	AfxGetMainWnd( )->PostMessage( WM_CLOSE, 0, 0);				// ���C���E�B���h�E�擾�E�I��
}

//
//===========================================================================
//	�R�}���h�G���[�\��
//
void 	Base::MbCmdError( 
						const char*	i_cFunction,	// �G���[�֐���
						MINT		i_iErrorCode	// �G���[�R�[�h
				  )
{
	MINT	ist;
	MCHAR	Msg[256];							// ү����

	ist = Code::Mmbstowcs( i_cFunction, Msg, 256);
	Mstrcat( Msg, Mstr( "�@\n\n"));

	switch( i_iErrorCode)
	{
	case MC_ERR_COMMAND:
		Mstrcat( Msg, Mstr( "�R�}���h�����G���["));
		break;
	default:
		Mstrcat( Msg, Mstr( "�R�}���h�����G���["));
	}		
	ist = AfxMessageBox( Msg, MB_ICONSTOP);
}

//
//===========================================================================
//	�G���[���^�[���u���[�N�|�C���g�p
//
void	Base::MbErrorReturn( void)
{
	__debugbreak();
}

} // namespace MC

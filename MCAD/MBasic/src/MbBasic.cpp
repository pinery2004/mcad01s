//==========================================================================================
//  Copyright ( C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MbBasic.cpp
//
//		��{�I�Ȋ֐��̏W�܂�
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"

#define	DLL_EXPORT_BASE_DO
#include "MbBasic.h"

#include <memory.h>
#include <locale.h>

//===========================================================================
//	���{�ꃂ�[�h�ɐݒ肷��
//
void 	MC::Base::MbSetJapanese( )
{
//	setlocale( LC_ALL, "ja");			// �G���[�ɂȂ��Ă���悤�����G���[�\������

//	setlocale( LC_ALL, ".ACP" );		// �ݒ�ɂ��킹��@( ���{���fwprintf���\�ɂȂ�)

	setlocale( LC_ALL, "jpn" );			// ���{���fwprintf���\�ɂ���
}


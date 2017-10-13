#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MhZukei.h
//
//		�}�`���
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#define		MHZK_DISP	2											// �}�`�\���p
#define		MHZK_AREA	1											// �}�`�̈攻��p

#define		MHZK_GLINE	2											// �����Q
#define		MHZK_GPOINT	1											// �_�Q

namespace MC
{

//	�}�`�f�[�^
class	MhZukei
{
	friend	class	MhPlcInfo;
public:
	MUBYTE				m_obj;									// �g�p�ړI
																//  MHZK_DISP	: �}�`�\���p
																//  MHZK_AREA	: �}�`�̈攻��p
	MUBYTE				m_iZTp;									// �}�`�f�[�^�^�C�v
																//	MHZK_GLINE	: �����Q
																//	MHZK_GPOINT	: �|���S��(�_�Q)
	MSHORT				m_nZukei;								// �}�`������
	MhZukei*			m_pNext;								// �ǉ��}�`����
	MgLine3				m_lnZukei[1];

	MINT	Load(												// �c�a���P���R�[�h�ǂݍ���
						MhMdlIO*	i_phMdl						// �ǂݍ��݃t�@�C���@�n���h��
				);
	MINT	Save(												// �c�a�ւP���R�[�h��������
						MhMdlIO*	i_phMdl						// �������݃t�@�C���@�n���h��
				);
};

} // namespace MC
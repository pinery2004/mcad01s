//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MdSystem.cpp
//
//		�V�X�e��
//
//
//  K.Matsu           01/06/08    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"
#include "MgDefine.h"
#include "MgPoint.h"
#include "MgRect.h"
#include "MgPolygon.h"
#include "MgMat.h"

#include "MdModel.h"

namespace MC
{

//===========================================================================
//		�f�[�^�x�[�X�I�[�v��
//
//		�w�肳�ꂽ���[�h�ɂ��AAUZ�t�@�C�������I�[�v������B
//		�����t�@�C���̏ꍇ�́A�ۑ�����Ă�����ŉ�ʂ��\�������B
//		�I�[�v����CALL����O�ɁA�J�����gDB���N���[�Y���Ă������ƁB
//
//		�I�[�v�����[�h�́A�w��f�[�^�x�[�X�t�@�C���ւ̃A�N�Z�X���[�h�B
//		�V�F�A���[�h�́A�w��f�[�^�x�[�X�t�@�C���ւ̋��L���[�h�ŁA�{�֐��ŃI�[�v����A
//		CloseAuzModel�ŃN���[�Y����܂ł̊Ԃ̃A�N�Z�X�𐧌�����B
//
//		���⑫��
//		�E���[�h�I�����[�ŃV�F�A����ꍇ
//			1�ڂ̃I�[�v������
//			�I�[�v�����[�h���hREAD�h�A�V�F�A���[�h���hREAD�h���w�肵�A
//
//			2�ڂ̃I�[�v�����ɂ����l��
//			�I�[�v�����[�h���hREAD�h�A�V�F�A���[�h���hREAD�h���w�肷��ƃV�F�A�G���[�ɂȂ�Ȃ��B
//
//		�E���[�h���C�g�ŃV�F�A����ꍇ
//			1�ڂ̃I�[�v������
//			�I�[�v�����[�h���hOLD�h�A�V�F�A���[�h���hWRITE�h���w�肵�A
//
//			2�ڂ̃I�[�v�����ɂ����l��
//			�I�[�v�����[�h���hOLD�h�A�V�F�A���[�h���hWRITE�h���w�肷��ƃV�F�A�G���[�ɂȂ�Ȃ��B
//
MINT	MdsOpenAuzModel (							// �X�e�[�^�X
												//	0: ����
												//	2: �����s��
												//	3: �f�[�^�x�[�X���N���[�Y����Ă��Ȃ�
												//	4: NEWTO�Ńt�@�C�������݂���
												//	5: OLD�AREAD�Ńt�@�C�������݂��Ȃ�
												//	6: �V�F�A�G���[
												//	7: ���̑��I�[�v���G���[

						MCHAR*		i_sFName,	// �f�[�^�x�[�X�t�@�C����
						MCHAR*		i_smdOpen,	// �I�[�v�����[�h
												//	NEWTO     �V�K�i�t�@�C�������݂���΃G���[�j
												//	NEWOVER  �V�K�i�t�@�C�������݂���΍폜�j
												//	OLD�@      �����iREAD/WRITE�ŃA�N�Z�X�j
												//	READ       �����iREAD-ONLY�ŃA�N�Z�X�j
						MCHAR*		i_smdhare	// �V�F�A���[�h
												//	NON�@�@�@�@�s��
												//	READ�@�@�@READ�̂݉�
												//	WRITE  �@�@READ/WRITE��
				)
{
	return 0;
}

//
//===========================================================================
//		�f�[�^�x�[�X�㏑���ۑ�
//
//		�I�[�v�����̃f�[�^�x�[�X�t�@�C�����㏑���ۑ�����B
//
MINT	MdsSaveAuzModel ()							// �X�e�[�^�X
												//	0:����
												//	7:�t�@�C��������i�e���|�����I�[�v�����j
												//	8:�������ݕs�iREAD-ONLY�j
												//	���̑�:�������݃G���[
{
	return 0;
}

//
//===========================================================================
//		�f�[�^�x�[�X���O��t���ĕۑ�
//
//		�I�[�v�����̃f�[�^�x�[�X�t�@�C���ɖ��O��t���ĕۑ�����B
//
MINT	MdsSaveasAuzModel  (						// �X�e�[�^�X
												//	0: ����
												//	2: ���[�h�s��
												//	4: TO�Ńt�@�C�������݂���
												//	8: �������݃G���[
												//	���̑�:�����G���[
						MCHAR*		i_sFName,	// �f�[�^�x�[�X�t�@�C����
						MCHAR*		i_smdSave	// ���[�h
												//	TO		�t�@�C�������݂���΃G���[
												//	OVER	�t�@�C�������݂���΍폜
				)
{
	return 0;
}

//
//===========================================================================
//		�f�[�^�x�[�X�N���[�Y
//
//		�I�[�v�����̃f�[�^�x�[�X�t�@�C�����N���[�Y����B
//
MINT	MdsCloseAuzModel (							// �X�e�[�^�X
												//�@0:����
												//�@2:���[�h�s��
												// ���̑�:�����G���[
						MCHAR*		i_smdClose	// �N���[�Y���[�h
												//	CANCEL   �L�����Z��
												//	OVER�@      �ۑ�
				)
{
	return 0;
}

//
//===========================================================================
//		���ݒ�̎��ʕ����񂩂�p�X������𓾂�
//
//		���ݒ�̎��ʕ����񂩂�p�X������𓾂�B
//		�����l�́A�V�X�e���f�B���N�g���[�ɂ���auz.sys�t�@�C���ɋL�q���Ă���B
//
MINT	MdsGetEnvPath  (						// �X�e�[�^�X
												//	0: ����
												//	2: ���[�h�s��
												//	4: TO�Ńt�@�C�������݂���
												//	8: �������݃G���[
												//	���̑�:�����G���[
						MCHAR*		i_sKey[],	// ���ʕ�����
						MCHAR*		o_sPath[],	// �p�X������
						MINT		i_szPath	// �p�X������o�b�t�@�T�C�Y
				)
{
	return 0;
}

//
//===========================================================================
//		���ݒ�Ɏ��ʕ�����ƃp�X�������ݒ肷��
//
//		���ݒ�̎��ʕ�����ɑ΂���p�X�������ݒ肷��B
//		���ʕ�����́A�P�U�o�C�g�܂łƂ���B
//		�p�X������͂Q�U�O�o�C�g�܂łƂ���B
//
MINT	MdsSetEnvPath  (						// �X�e�[�^�X
												//	0: ����
												//	2: ���[�h�s��
												//	4: TO�Ńt�@�C�������݂���
												//	8: �������݃G���[
												//	���̑�:�����G���[
						MCHAR*		i_sKey[],	// ���ʕ�����
						MCHAR*		i_sPath[]	// �p�X������
				)
{
	return 0;
}

//
//===========================================================================
//		���ݒ�̎��ʕ����񂩂�p�X������𓾂�
//
//		���ݒ�̎��ʕ����񂩂�p�X������𓾂�B
//		�����l�́A�V�X�e���f�B���N�g���[�ɂ���auz.sys�t�@�C���ɋL�q���Ă���B
//
MINT	MdsGetTitle  (						// �X�e�[�^�X
												//	0: ����
												//	2: ���[�h�s��
												//	4: TO�Ńt�@�C�������݂���
												//	8: �������݃G���[
												//	���̑�:�����G���[
						MCHAR*		i_sKey[],	// ���ʕ�����
						MCHAR*		o_sPath[],	// �p�X������
						MINT		i_szPath	// �p�X������o�b�t�@�T�C�Y
				)
{
	return 0;
}

//
//===========================================================================
//		�^�C�g��������̐ݒ�
//
//		�E�B���h�E�^�C�g���ɕ\�����镶�����ݒ肷��B
//		���݂̃C���f�b�N�X��
//		�@�@�@�@0:���W���[����
//		�@�@�@�@1:�f�[�^�x�[�X�t�@�C����
//		�@�@�@�@2:�J�����g�O���[�v��
//		�@�@�@�@3:�J�����g���C���[��
//		�ŗ��p���Ă���B
//
MINT	MdsSetTitle  (								// �X�e�[�^�X
												//	0:����
												//�@2:tidx�s��
												//	���̑�:�����G���[
						MINT		i_szPath,	// �^�C�g���C���f�b�N�X
						MCHAR*		o_sPath[]	// �^�C�g��������
				)
{
	return 0;
}

//
//===========================================================================
//		�^�C�g��������̕\��
//
//		�w�肳��Ă���^�C�g���������������ĕ\������B
//
MINT	MdsDispTitle  ()
{
	return 0;
}

//
//===========================================================================
//		�f�[�^�x�[�X�t�@�C�����𓾂�
//
//		���݃I�[�v������Ă���f�[�^�x�[�X�t�@�C�����𓾂�B
//
MINT	MdsGetAuzModelName  (								// �X�e�[�^�X
												//	0:����
												//	1:�o�b�t�@�[�I�[�o�t���[-
						MCHAR*		o_sPath[]	// �f�[�^�x�[�X�t�@�C����
				)
{
	return 0;
}

//
//===========================================================================
//		�V�X�e���f�B���N�g���̖₢���킹
//
//		�V�X�e���f�B���N�g����₢���킹��B
//
MINT	MdsGetExeDir  (							// �X�e�[�^�X
												//	0:����
												//	1:�o�b�t�@�[�I�[�o�t���[-
						MCHAR*		o_sPath[]	// �V�X�e���f�B���N�g���p�X
				)
{
	return 0;
}

//
//===========================================================================
//		�ڍ׃G���[�R�[�h�𓾂�
//
MINT	MdsGetErr  (								// �ڍ׃G���[�R�[�h
												//	0:����
												//	1:�o�b�t�@�[�I�[�o�t���[-
						MCHAR*		o_sPath[]	// �V�X�e���f�B���N�g���p�X
				)
{
	return 0;
}

} // namespace MC
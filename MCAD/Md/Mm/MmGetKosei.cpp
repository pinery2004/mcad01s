//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MmGetGp.cpp
//
//		�J�����g���̓��[�h
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"
#include "MbDefine.h"
#include "MbCod.h"
#include "MmWnd.h"

#define DLL_EXPORT_MC_INPATTR_DO
#define DLL_EXPORT_MC_INPMODE_DO
#include "MmLib.h"

#include "MsBitSet.h"
#include "MCADApp.h"
#include "MhLib.h"

#include "MhInp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace MC
{

static MINT z_iInpMd;							// �n�����[�h		�폜�A�C���A�ړ��A���ʁA�����\��
static MINT z_InpKai;							// �K				�P�K�A�Q�K�A�R�K

static MINT z_iSInpGp;							// �݌v���͍\��		�Z�ˁA�Z���A�����A�����A�~�n�A�Ǘ�
static MINT z_iSInpBr;							// �݌v���͎��
static MINT z_iSInpSel;							// �݌v���͑I��

static MINT z_iKInpGp;							// �\�����͍\��		�����A�����A�V��A�ϗ͕ǁA�ǁA���A��b
static MINT z_iKInpBr;							// �\�����͎��		���ށA�����A�p�l���A���̑�
static MINT z_iKInpSel;							// �\�����͑I��		�I��1�A�I��2�A�I��3						���g�p

static MINT z_bKInpBuzai;						// ���ޑI����͋��t���O
static MINT z_bKInpKanagu;						// �����I����͋��t���O
static MINT z_bKInpPanel;						// �p�l���I����͋��t���O
static MINT z_bKInpOther;						// ���ޑI����͋��t���O

//	static MINT z_nPtsID;						// �z�u���i�^�C�v(���ށA�����A�p�l���A���̑��@�̕���ID)

////////////////////////////////////////////////////////////////////////////
//	���ޑ������͗p�R���{�{�b�N�X�̐ݒ�

void InpAttr::SetComboPts()
{
	//	���ޑI��p�R���|�{�b�N�X�̍��ڂ�ݒ肷��
	InpAttr::InitComboTpPtsAttr();

	//	���@�^���I��p�R���|�{�b�N�X�̍��ڂ�ݒ肷��
	InpAttr::InitComboPtsMbr();

	//	�p�l���ԍ��I��p�R���|�{�b�N�X�̍��ڂ�ݒ肷��
	InpAttr::InitComboPanelNo();

	//	���i�I��p�R���|�{�b�N�X�̍��ڂ�ݒ肷��
	InpAttr::InitComboAttr();

	Window::CurWndFocus();
	WindowCtrl::MmWndKReDraw();
}

//	�n�����[�h		�폜�A�C���A�ړ��A���ʁA�����\��

void InpMode::SetMode( MINT iMode)
{
	z_iInpMd = iMode;
}

MINT InpMode::GetMode()
{
	return z_iInpMd;
}

//	�K				�P�K�A�Q�K�A�R�K

void InpMode::SetKai( MINT iKai)
{
	z_InpKai = iKai;
}

MINT InpMode::GetKai()
{
	return z_InpKai;
}

// �݌v���͍\��		�Z�ˁA�Z���A�����A�����A�~�n�A�Ǘ�

void InpMode::SetSGp( MINT iGp)
{
	z_iSInpGp = iGp;
}

MINT InpMode::GetSGp()
{
	return z_iSInpGp;
}

// �݌v���͎��

void InpMode::SetSBr( MINT iBr)
{
	z_iSInpBr = iBr;
}

MINT InpMode::GetSBr()
{
	return z_iSInpBr;
}

// �݌v���͑I��

void InpMode::SetSSel( MINT iSel)
{
	z_iSInpSel = iSel;
}

MINT InpMode::GetSSel()
{
	return z_iSInpSel;
}

// �\�����͍\��		�����A�����A�V��A�ϗ͕ǁA�ǁA���A��b

void InpMode::SetKGp( MINT iGp)
{
	z_iKInpGp = iGp;
}

MINT InpMode::GetKGp()
{
	return z_iKInpGp;
}

// �\�����͎��		���ށA�����A�p�l���A���̑�

void InpMode::SetKBr( MINT iBr)
{
	z_iKInpBr = iBr;
}

MINT InpMode::GetKBr()
{
	return z_iKInpBr;
}

// �\�����͑I��		�I��1�A�I��2�A�I��3						���g�p

void InpMode::SetKSel( MINT iSel)
{
	z_iKInpSel = iSel;
}

MINT InpMode::GetKSel()
{
	return z_iKInpSel;
}

//	���ށA�����A�p�l���A���̑��̃��W�I�{�^���̑I����͋���ݒ�

void InpMode::SetKBrB( bool bBuzai, bool bKanagu, bool bPanel, bool bOther)
{
	z_bKInpBuzai  = bBuzai;
	z_bKInpKanagu = bKanagu;
	z_bKInpPanel  = bPanel;
	z_bKInpOther  = bOther;
}

} // namespace MC
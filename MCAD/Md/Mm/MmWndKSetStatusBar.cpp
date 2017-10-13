//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: Msg::SetStatusBar.cpp
//
//		�E�B���h�E�\���֘A
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

#define DLL_EXPORT_MC_MSG_DO
#include "MmLib.h"

#include "MmSystemProperty.h"

#include "MmCmd.h"
#include "resource.h"

#include "Mainfrm.h"

#include "MmCmdMsg.h"
#include "MmPrompt.h"

#define	MAXNCSTBGUIDANCE	61
#define	MAXNCSTBERROR		61
#define	MAXNCSTBOPERATION	11

namespace MC
{

static	MCHAR		z_cStbGuidance[MAXNCSTBGUIDANCE];				// �X�e�C�^�X�o�[�̃K�C�_���X���b�Z�[�W�\�����e
static	MCHAR		z_cStbError[MAXNCSTBERROR];						// �X�e�C�^�X�o�[�̃G���[���b�Z�[�W�\�����e
static	MCHAR		z_cStbOperation[MAXNCSTBOPERATION];				// �X�e�C�^�X�o�[�̃I�y���[�V�������b�Z�[�W�\�����e

/////////////////////////////////////////////////////////////////////////////
//  �X�e�C�^�X�o�[�\�������@

void Msg::SetSelStatusBar(
						MINT		i_iStb,		// �X�e�C�^�X�o�[���ڔԍ�
				const	MCHAR*		i_pcStb		// �X�e�C�^�X�o�[���ڕ\�����e
				)
{
	CWnd*		pWnd;
	MmWndInfo*	pWndInfo;

	if ( i_iStb == MC_STSBAR_GUIDANCE) {
		Mstrncpy( z_cStbGuidance, i_pcStb, MAXNCSTBGUIDANCE);
	} else if ( i_iStb == MC_STSBAR_ERROR) {
		Mstrncpy( z_cStbError, i_pcStb, MAXNCSTBERROR);
		z_cStbGuidance[0] = NULL;
	} else if ( i_iStb == MC_STSBAR_OPERATION) {
		Mstrncpy( z_cStbOperation, i_pcStb, MAXNCSTBOPERATION);
	} else {
		return;
	}
	pWndInfo = WindowCtrl::MmWndKGetCurWnd();								// �J�����g�E�B���h�E�擾
	if ( pWndInfo) {
		pWnd = pWndInfo->GetWnd();
		pWnd->PostMessage(WM_MYMESSAGESTATUSBAR);				// �X�e�C�^�X�o�[�\���p���b�Z�[�W�n���h�����N������
	}
}

/////////////////////////////////////////////////////////////////////////////
//  �X�e�C�^�X�o�[�\�������@(���b�Z�[�W�n���h�����Ă΂��)
void Msg::DispStatusBar()
{
	Msg::DispSelStatusBar( MC_STSBAR_GUIDANCE, z_cStbGuidance);		// �X�e�C�^�X�o�[�̃K�C�_���X���b�Z�[�W��\������
	Msg::DispSelStatusBar( MC_STSBAR_OPERATION, z_cStbOperation);		// �X�e�C�^�X�o�[�̃I�y���[�V�������b�Z�[�W��\������
	Msg::DispSelStatusBar( MC_STSBAR_ERROR, z_cStbError);				// �X�e�C�^�X�o�[�̃G���[���b�Z�[�W��\������
}

/////////////////////////////////////////////////////////////////////////////
//	�X�e�[�^�X�o�[�ɕ������\������@(���b�Z�[�W�n���h�����g�p��)

void Msg::DispSelStatusBar(
						MINT		i_iStb,		// �X�e�C�^�X�o�[���ڔԍ�
				const	MCHAR*		i_pcStb		// �X�e�C�^�X�o�[���ڕ\�����e
				)
{
	CMDIFrameWnd*	MainFrm;					// ���C���t���[��
	CStatusBar*		StBar;						// �X�e�C�^�X�o�[

	MainFrm = System::GetpMainFrame();										// ���C���t���[�����擾
	if ( MainFrm != NULL) {
		StBar = (CStatusBar*)(MainFrm->GetDescendantWindow( AFX_IDW_STATUS_BAR));
		StBar->SetPaneText( i_iStb, i_pcStb);
	}
}

} // namespace MC
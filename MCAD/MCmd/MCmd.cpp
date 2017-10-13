// MCmd.cpp : DLL �̏��������[�`���ł��B
//

#include "stdafx.h"
#include <afxdllx.h>
#ifdef _MANAGED
#error /clr �Ƌ��ɃR���p�C������ɂ� MCmd.cpp ���̑���菇���Q�Ƃ��Ă��������B
// /clr ���v���W�F�N�g�ɒǉ��������ꍇ�́A���̑�����s���Ă�������:
//	1. ��L�� afxdllx.h �̃C���N���[�h���폜����
//	2. .cpp �t�@�C�����A�v���R���p�C���ς݃w�b�_�[�������ɂ���Ă��Ď��̃e�L�X�g���܂݁A
//	   /clr ���X���[����Ă��Ȃ��v���W�F�N�g�ɒǉ����܂�:
//			#include <afxwin.h>
//			#include <afxdllx.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


static AFX_EXTENSION_MODULE MCmdDLL = { NULL, NULL };

#ifdef _MANAGED
#pragma managed(push, off)
#endif

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// lpReserved ���g���ꍇ�͂������폜���Ă�������
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("MCmd.DLL Initializing!\n");
		
		// �g�� DLL �� 1 �񂾂����������܂��B
		if (!AfxInitExtensionModule(MCmdDLL, hInstance))
			return 0;

		// ���� DLL �����\�[�X �`�F�[���֑}�����܂��B
		// ����: �g�� DLL �� MFC �A�v���P�[�V�����ł͂Ȃ�
		//  MFC �W�� DLL (ActiveX �R���g���[���̂悤��)
		//  �ɈÖٓI�Ƀ����N�����ꍇ�A���̍s�� DllMain
		//  ����폜���āA���̊g�� DLL ����G�N�X�|�[�g
		//  ���ꂽ�ʂ̊֐����֒ǉ����Ă��������B
		//  ���̊g�� DLL ���g�p����W�� DLL �͂��̊g�� DLL
		//  �����������邽�߂ɖ����I�ɂ��̊֐����Ăяo���܂��B
		//  ����ȊO�̏ꍇ�́ACDynLinkLibrary �I�u�W�F�N�g��
		//  �W�� DLL �̃��\�[�X �`�F�[���փA�^�b�`���ꂸ�A
		//  ���̌��ʏd��Ȗ��ƂȂ�܂��B

		new CDynLinkLibrary(MCmdDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("MCmd.DLL Terminating!\n");

		// �f�X�g���N�^���Ăяo�����O�Ƀ��C�u�������I�����܂�
		AfxTermExtensionModule(MCmdDLL);
	}
	return 1;   // OK
}

#ifdef _MANAGED
#pragma managed(pop)
#endif


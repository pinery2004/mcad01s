//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MsDispModel.cpp
//
//		�ƃ��f���̃f�B�X�v���C���X�g�쐬
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MCAD.h"

#include "MmWnd.h"

#include "MgLib.h"
#include "MmDefine.h"
#include "MmSystemProperty.h"

#include "MsBitSet.h"
#include "MbList.h"
#include "MhLib.h"

#include "MgGrp.h"
#include "MhRoof.h"
#include "MdLib.h"

#include "MhInp.h"

#include "MmOpt.h"
#include "MmHist.h"

#include "MdFig.h"
#include "MdmDisp.h"

#include "MdDispList.h"
#include "MmValid.h"

#include "MnLib.h"

#define		MP_PANEL_LINE			1
#define		MP_PANEL_RECTANGULAR	2

namespace MC
{

////////////////////////////////////////////////////////////////////////////
//	�f�B�X�v���C���X�g���쐬����
MINT DispList::Open( 
						MINT		inmDispList		// DisplayList�ԍ�
				)
{
	// make display list number
	if ( ( z_DispListFig[inmDispList] = glGenLists( 1)) == 0)
		return 1;

	// �f�B�X�v���C���X�g���쐬����
	glNewList( z_DispListFig[inmDispList], GL_COMPILE);

//	glPushMatrix();
	return 0;
}

////////////////////////////////////////////////////////////////////////////
//	�f�B�X�v���C���X�g�̍쐬���I������
void DispList::Close()
{
//	glPopMatrix();
	glEndList();
}

////////////////////////////////////////////////////////////////////////////
//	�\���ƃ��f���̂R�����f�B�X�v���C���X�g���쐬����	���ށA��b	����
//	
int DispList::MakeIeMdl(
						MREAL		rB,				// �\���{��
				const	MgPoint3	&PtCtr			// ���S���W
				)
{
	DispList::MakeIeMdl1( rB, PtCtr);
	DispList::MakeIeMdl2( rB, PtCtr);
	DispList::MakeIeMdl3( rB, PtCtr);
	MsMakeDispListMat0( rB, PtCtr);
	MsMakeDispListMat1( rB, PtCtr);
	MsMakeDispListMat2( rB, PtCtr);
	return 0;
}

////////////////////////////////////////////////////////////////////////////
//	�\���ƃ��f���̂R�����f�B�X�v���C���X�g���쐬����	���ށA��b	����
//	
MINT DispList::MakeIeMdl1(
						MREAL		rB,				// �\���{��
				const	MgPoint3	&PtCtr			// ���S���W
				)
{
	MhPlcInfo*	pPlcEn;
	MPOSITION	posH;

	if ( DispList::Open( 1))
		return 1;
	
	for (pPlcEn=MdGetHeadPts( &posH); pPlcEn!=0; pPlcEn=MdGetNextPts( &posH)) {

		if ( !MmChkValidParts( pPlcEn))							// �I�v�V�����Ɨ����̃`�F�b�N
			continue;

		//	���ނ̃f�B�X�v���C���X�g���쐬����
		if (Mstrcmp( pPlcEn->GetPTNmGeneral(), Mstr( "��")) == 0 ||
			Mstrcmp( (&pPlcEn->GetPTNmGeneral()[2]), Mstr( "����")) == 0)			// �ǂƌ���͓ǂݔ�΂�
			continue;

		//	���Ęg�̃f�B�X�v���C���X�g���쐬����
		if (pPlcEn->IsFrame()) {
			MsOGL::SetMaterial( 0, 3);
			DrwTatewaku( pPlcEn, PtCtr, rB);

		//	�p�l���̃f�B�X�v���C���X�g���쐬����
		} else if (pPlcEn->IsPanel()) {
			MsOGL::SetMaterial( 0, 3);
			DrwPanel( pPlcEn, PtCtr, rB, 3);

		//	���A�V��A�����̊J���̃f�B�X�v���C���X�g���쐬����
		} else if (pPlcEn->IsKaiko()) {
			MsOGL::SetMaterial( 0, 3);
			DrwKaiko( pPlcEn, PtCtr, rB);

		//	��b�̃f�B�X�v���C���X�g���쐬����
		} else if (pPlcEn->IsKiso()) {
			MsOGL::SetMaterial( 0, 4);
			DrwKiso( pPlcEn, PtCtr, rB);

		//	���u���ނ̃f�B�X�v���C���X�g���쐬����
		} else if (pPlcEn->IsOukaZai()) {
			MsOGL::SetMaterial( 0, 3);
			DrwOukaZai( pPlcEn, PtCtr, rB);
		}
	}
	// �}�`���̃f�B�X�v���C���X�g���쐬����
	MdDispList( PtCtr, rB);

	DispList::Close();
	return 0;
}

////////////////////////////////////////////////////////////////////////////
//	�\���ƃ��f���̂R�����f�B�X�v���C���X�g���쐬����	�ǁA��؁@�}�e���A���Ȃ�
//
int DispList::MakeIeMdl2(
						MREAL		rB,				// �\���{��
				const	MgPoint3	&PtCtr			// ���S���W
				)
{
	MhPlcInfo*	pPlcEn;
	MPOSITION	posH;
	MhRoofInfo*	pRoofEn;
	MPOSITION	posR;

	if ( DispList::Open( 2))
		return 1;

	//	�ǂ̃f�B�X�v���C���X�g���쐬����
	for (pPlcEn=MdGetHeadPts( &posH); pPlcEn!=0; pPlcEn=MdGetNextPts( &posH)) {

		if ( !MmChkValidParts( pPlcEn))							// �I�v�V�����Ɨ����̃`�F�b�N
			continue;

		//	�ǂ̃f�B�X�v���C���X�g���쐬����
		if (Mstrcmp( pPlcEn->GetPTNmGeneral(), Mstr( "��")) == 0) {
			Kabe( pPlcEn, PtCtr, rB);

		//	����̊J���̃f�B�X�v���C���X�g���쐬����
		} else if (Mstrcmp( (&pPlcEn->GetPTNmGeneral()[2]), Mstr( "����")) == 0) {
			DrwTategu( NULL, pPlcEn, PtCtr, rB);

		}
	}

	//	��؂̃f�B�X�v���C���X�g���쐬����
	for (pRoofEn=MdGetHeadRoof( &posR); pRoofEn!=0; pRoofEn=MdGetNextRoof( &posR)) {

		if ( !MmChkValidRoof( pRoofEn))							// �I�v�V�����Ɨ����̃`�F�b�N
			continue;

		Yagiri( pRoofEn, PtCtr, rB);
	}

	DispList::Close();
	return 0;
}

////////////////////////////////////////////////////////////////////////////
//	�\���ƃ��f���̂R�����f�B�X�v���C���X�g���쐬����	�����@�}�e���A���Ȃ�
//
int DispList::MakeIeMdl3(
						MREAL		rB,				// �\���{��
				const	MgPoint3	&PtCtr			// ���S���W
				)
{

	MhRoofInfo*		pRoofEn;
	MPOSITION		posR;

	if ( DispList::Open( 3))
		return 1;

	//	�����̃f�B�X�v���C���X�g���쐬����
	for (pRoofEn=MdGetHeadRoof( &posR); pRoofEn!=0; pRoofEn=MdGetNextRoof( &posR)) {
		if ( !MmChkValidRoof( pRoofEn))							// �I�v�V�����Ɨ����̃`�F�b�N
			continue;
		Yane1( pRoofEn, PtCtr, rB);
	}

	DispList::Close();
	return 0;
}

} // namespace MC
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MhMdlIO.cpp
//
//		�ƃ��f���ǂݏ���
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"
#include "MgLib.h"

#include "MmDefine.h"
#define DLL_EXPORT_IEMODEL_DO
#include "MhLib.h"
#include "MmLib.h"
#include "MdLib.h"
#include "MbList.h"

#include "MhInp.h"

#include "MsBitSet.h"
#include "MmOpt.h"
#include "MmHist.h"

#include "MmCmd.h"
#include "MgGrp.h"
#include "MhPlcInfo.h"
#include "MhRoof.h"

#include "MbFile.h"
#include "MmTenkai.h"

#include "MhDefine.h"

#include "MdModel.h"

#include "MdmDB.h"

#include "MhModel.h"

#include "MmWnd.h"
namespace MC
{

////////////////////////////////////////////////////////////////////////////
//	�\���ƃ��f���̓ǂݍ���
//	[�Ԓl] �X�e�C�^�X     0 : ����  -1 : �ُ�

MINT IeModel::MhMdlLoad(										//
						MCHAR*		i_pcPathI		// �ǂݍ��݃t�@�C���@�t���p�X
				)
{
	MINT		ist = -1;
	MINT		ist1;
	MINT		ic;
	MhMdlIO		hMdl;
	MINT		iType;
	MINT		iSize;

	void*			pEnt;
	MhPlcInfo*		pPlcEn = 0;
	MhRoofInfo*		pRoofEn = 0;
	MINT*			pnGrid;

	bool			fPlcSzChk = true;
	bool			fRoofSzChk = true;

	MgVect3			vLnPlc;

	MDID	idEnt[3000];
	MINT	isz = 1;
	static MINT	iCe = 0;

	if ( i_pcPathI == 0) {
		InpMode::SetKai( 1);
		MhPlcInp::SetKai( 1);
		WindowCtrl::MmGridNumSet( NULL);													// �����ݒ�
		WindowCtrl::MmGridNumXqt();
		IeModel::MhSetModFIeMdl( false);
		ist = 0;
		MQUIT;
	}

	// MDB�I�[�v��
	MhModel::Open();

	// DB�t�@�C���I�[�v��
	ist1 = hMdl.Open( MBREAD, i_pcPathI);
	if ( ist1 != 0)
		MQUIT;

	// DB�t�@�C���ǂݍ���
	for (;;) {
		iSize = hMdl.Read( &iType, (void**)&pEnt);
//===
		static int iCrf = 0;
		TRACE( "Roof iCrf = %d\n", ++iCrf);
		//if( iCrf == 315) {
		//	__debugbreak();										// �u���[�N�|�C���g
		//}
//---

		if ( iSize <= 0)
			break;

		switch( iType) {
			case MDR_ATRCD_GRID:													// �O���b�h
				// �O���b�h�̓Ǎ�
				pnGrid = (MINT*)pEnt;
				WindowCtrl::MmGridNumSet( pnGrid);

				ist = DEnt::CreateEnt( MhModel::m_db.pidLyr[MD_ID_LYR_GRID], &idEnt[iCe], isz);
				ist = DEnt::SetAttr( idEnt[iCe++],	1, pnGrid);
				break;

			case MDR_ATRCD_HAITIEN:												// �z�u
				// �z�u���i�̓Ǎ�
				MBFREE( pEnt);

				pPlcEn = (MhPlcInfo*)malloc( sizeof( MhPlcInfo));
				pPlcEn->Load( &hMdl);
				HaitiDb::MdPtsAdd( pPlcEn, NULL);										// �n�o�s�Q�ύX�����Œǉ�

				ist = DEnt::CreateEnt( MhModel::m_db.pidLyr[MD_ID_LYR_HAITI], &idEnt[iCe], isz);
				ist = DEnt::SetAttr( idEnt[iCe++], 2, pPlcEn);
				break;

			case MDR_ATRCD_ROOFEN:												// ����
				// �����̓Ǎ�
				ASSERT( iSize == sizeof(MhRoofInfo));

				pRoofEn = (MhRoofInfo*)pEnt;
				pRoofEn->Load( &hMdl);
				HaitiDb::MdRoofAdd( pRoofEn);

				ist = DEnt::CreateEnt( MhModel::m_db.pidLyr[MD_ID_LYR_YANE], &idEnt[iCe], isz);
				ist = DEnt::SetAttr( idEnt[iCe++],	3, pRoofEn);
				break;

			default:
				MBFREE( pEnt);
		}
	}
	hMdl.Close();

	for ( ic=1; ic<=MMAX_KAI; ic++) {
		InpMode::SetKai( ic);
		MhPlcInp::SetKai( ic);

		IeModel::MhKabeSetUpFlg();
		IeModel::MhNormKabe( 0);
		IeModel::MhKisoSetUpFlg();
		IeModel::MhNormKiso( 0);
	}

	InpMode::SetKai( 1);
	MhPlcInp::SetKai( 1);

	WindowCtrl::MmGridNumSet( NULL);														// �����ݒ�
	WindowCtrl::MmGridNumXqt();

	IeModel::MhSetModFIeMdl( false);
	ist = 0;
exit:
    return ist;
}

////////////////////////////////////////////////////////////////////////////
//	�n�o�s�Q�Ǘ���񃌃R�[�h��������
//	[�Ԓl] �X�e�C�^�X     0 : ����  -1 : �ُ�

MINT MhOptionSave(								//
				MhMdlIO*		i_phMdl			// �������݃t�@�C���@�n���h��
				)
{
	// �n�o�s�Q�Ǘ����̕ۑ�
	i_phMdl->Write( MDW_ATRCD_OPTION_KANRI, (MINT*)MhOptV::bmGOpt, MhOptV::nbmGOpt);

	i_phMdl->Write( MDW_ATRCD_VERSION_KANRI0, &MhHistV::nVer, sizeof(MINT));		// �o�[�W����
	for (MINT ic=0; ic<MhHistV::nVer; ic++) {
		i_phMdl->Write( MDW_ATRCD_VERSION_KANRI1, MhHistV::sVerC[ic], MSZ_HISTB);
		i_phMdl->Write( MDW_ATRCD_VERSION_KANRI2, MhHistV::sVerB[ic], MSZ_HISTB);
		i_phMdl->Write( MDW_ATRCD_VERSION_KANRI3, MhHistV::sVerD[ic], MSZ_HISTB);
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////
//	�\���ƃ��f���̏�������
//	[�Ԓl] �X�e�C�^�X     0 : ����  -1 : �ُ�

MINT IeModel::MhMdlSave(									//
				const	MCHAR*		i_pcPathO	// �������݃t�@�C���@�t���p�X
				)
{
	MhPlcInfo*		pPlcEn1;
	MPOSITION		posH;
	MhRoofInfo*		pRoofEn1;
	MPOSITION		posR;
	MhMdlIO			hMdl;
	MINT			iDumy = 0;
	MINT			nGrid[4];

	hMdl.Open( MBWRITE, i_pcPathO);

	// �O���b�h�̕ۑ�
	WindowCtrl::MmGridNumGet( nGrid);
	hMdl.Write( MDW_ATRCD_GRID, nGrid, sizeof(MINT) * 4);			// �O���b�h
	
	// �z�u���i�̕ۑ�
	for ( pPlcEn1 = HaitiDb::MdGetHeadPts( &posH); pPlcEn1!=0;
		  pPlcEn1 = HaitiDb::MdGetNextPts( &posH)) {
		pPlcEn1->Save( &hMdl);
	}

	// �����̕ۑ�
	for ( pRoofEn1 = HaitiDb::MdGetHeadRoof( &posR); pRoofEn1!=0;
		  pRoofEn1 = HaitiDb::MdGetNextRoof( &posR)) {
		pRoofEn1->Save( &hMdl);
	}

	// �n�o�s�Q�Ǘ����̕ۑ�
	MhOptionSave( &hMdl);

	hMdl.Close();

	return 0;
}

} // namespace MC
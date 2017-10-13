//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MhPlcInfo.cpp
//
//		���ޔz�u�ǂݏ���
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"
#include "MgLib.h"

#include "MsBitSet.h"
#include "MmOpt.h"
#include "MmHist.h"

#include "MhLib.h"
#include "MhPlcInfo.h"

#include "MbFile.h"
#include "MmTenkai.h"

#include "MhDefine.h"

//void MdPtsDbInit();
//void MdRedoPtsDbInit();
//void MdRoofDbInit();

namespace MC
{

////////////////////////////////////////////////////////////////////////////
//	�z�u���ނ̓ǂݍ���
//	[�Ԓl] �X�e�C�^�X     0 : ����  -1 : �ُ�

MINT MhPlcInfo::Load(							//
						MhMdlIO*	i_phMdl		// �ǂݍ��݃t�@�C���@�n���h��
				)
{
	MINT		ist = 0;
	MINT		iType;
	MINT		iSize;
	MINT		nVer;
	
	void*		pEnt;
	MhPlcInfo*	pPlcEn = 0;
	MhTateguInfo*	pAuxTategu;
	JTTenkaiPr*	pAuxTenkai;
	MhHistV*	pVer;
	MsBitSet*	pOpt;
	MhZukei*	pZukei;
	MINT		szZukei;
	MCHAR*		pcMbr;
	MINT		iMbr;
	MINT		iCdBuzai;

	bool		fPlcSzChk = true;
	bool		fRoofSzChk = true;

	MgVect3		vLnPlc;
	
	bool		bEor = false;

	// �z�u���R�[�h�J�n

	//pPlcEn = (MhPlcInfo*)malloc( sizeof( MhPlcInfo));
	//
	//m_pVer = 0;
	//m_pOpt1 = 0;
	//m_pOpt2 = 0;
	//m_pAuxTategu = 0;
	//m_pZukei = 0;
	//m_iDisplayFlg = 0;
	//m_ptCmnt1 = MgPoint2( 0., 0.);

	memset( this, 0, sizeof( MhPlcInfo));

	do {
		iSize = i_phMdl->Read( &iType, (void**)&pEnt);

		if ( iSize <= 0)
			break;

		switch( iType) {
			case MDR_ATRCD_HAITIEN_EOR:											// �z�u ���R�[�h�I��r
				ASSERT( iSize == sizeof(MINT));
				MBFREE( pEnt);													// TYPE_HAITIEN_EOR���R�[�h�p�G���A�̍폜
				
				m_pTpPts	= BuzaiCode::MhGetpTpPts( m_iIdTpPts);
				m_pMbr		= BuzaiCode::MhGetpMbr( m_iMbrId);
//				m_pZukei	= NULL;
				
				bEor = true;
				break;
			case MDR_ATRCD_HAITIEN_IKAI:										// �K
				ASSERT( iSize == sizeof(MINT));
				m_iKai = *(MINT*)pEnt;
				MBFREE( pEnt);
				break;
			case MDR_ATRCD_HAITIEN_ICDHGT:										// �z�u�_
				ASSERT( iSize == sizeof(MINT));
				m_iCdHgt = *(MINT*)pEnt;
				MBFREE( pEnt);
				break;
			case MDR_ATRCD_HAITIEN_IPANELNO:									// �z�u�_
				ASSERT( iSize == sizeof(MINT));
				m_iPanelNo = *(MINT*)pEnt;
				MBFREE( pEnt);
				break;
			case MDR_ATRCD_HAITIEN_IKAIKONO:									// �J���ԍ�
				ASSERT( iSize == sizeof(MINT));
				m_iKaikoNo = *(MINT*)pEnt;
				MBFREE( pEnt);
				break;
//			case MDR_ATRCD_HAITIEN_IPARTSTPID:
			case MDR_ATRCD_HAITIEN_IPARTSTPCD:									// ���i�z�u�^�C�v�R�[�h 
				ASSERT( iSize == sizeof(MINT));
				iCdBuzai = *(MINT*)pEnt;										// (���ރR�[�h)
				m_iIdTpPts = BuzaiCode::MhGetPIIdTpPts( iCdBuzai);
				MBFREE( pEnt);
				break;
//			case MDR_ATRCD_HAITIEN_IMEMBERID:									// �����o�[
//				ASSERT( iSize == sizeof(MINT));
//				m_iMbrId = *(MINT*)pEnt;
//				MBFREE( pEnt);
//				break;
			case MDR_ATRCD_HAITIEN_MEMBER:										// �����o�[
				ASSERT( iSize == sizeof( MhTateguInfo));
				pcMbr = (MCHAR*)pEnt;
				iMbr = BuzaiCode::MhGetIdMbr( pcMbr);
				if ( iMbr < 0) {
					iMbr = 0;
				}
				m_iMbrId	= iMbr;
				m_pMbr		= BuzaiCode::MhGetpMbr( m_iMbrId);
				MBFREE( pcMbr);
				break;
			case MDR_ATRCD_HAITIEN_LNHAITI:										// �z�u�_�A�z�u�����_
				ASSERT( iSize == sizeof(MgLine3));
				m_lnPlc = *(MgLine3*)pEnt;
				vLnPlc = m_lnPlc.p[1] - m_lnPlc.p[0];
				vLnPlc.SetUnitize();
				m_vUpPlc = vLnPlc ^ MgVect3( 0., 0., 1.);
				MBFREE( pEnt);
				break;
			case MDR_ATRCD_HAITIEN_PTUPHAITI:									// �����
				ASSERT( iSize == sizeof(MgPoint3));
				m_vUpPlc = *(MgPoint3*)pEnt;
				MBFREE( pEnt);
				break;
			case MDR_ATRCD_HAITIEN_RSINZURE:									// �ގ��c�����
				ASSERT( iSize == sizeof(MREAL));
				m_rSinZure = *(MREAL*)pEnt;
				MBFREE( pEnt);
				break;
			case MDR_ATRCD_HAITIEN_RHAITIZURE:									// �z�u�_����� 
				ASSERT( iSize == sizeof(MREAL));
				m_rPlcZure = *(MREAL*)pEnt;
				MBFREE( pEnt);
				break;
			case MDR_ATRCD_HAITIEN_RNAGASAHOSEI:								// �����␳�l1�A�����␳�l2
				ASSERT( iSize == sizeof(MREAL) * 2);
				m_rLngHosei[0] = *(MREAL*)pEnt;
				m_rLngHosei[1] = *(((MREAL*)pEnt)+1);
				MBFREE( pEnt);
				break;
			case MDR_ATRCD_HAITIEN_RTAKASA:										// ����
				ASSERT( iSize == sizeof(MREAL) * 2);
				m_rHgt[0] = *(MREAL*)pEnt;
				m_rHgt[1] = *(((MREAL*)pEnt)+1);
				MBFREE( pEnt);
				break;
			case MDR_ATRCD_HAITIEN_TATEGU:										// �����
				ASSERT( iSize == sizeof( MhTateguInfo));
				pAuxTategu = (MhTateguInfo*)pEnt;
				m_pAuxTategu = pAuxTategu;
				break;
			case MDR_ATRCD_HAITIEN_TENKAI:										// �Z���W�J
				ASSERT( iSize == sizeof(JTTenkaiPr));
				pAuxTenkai = (JTTenkaiPr*)pEnt;
				m_pAuxTenkai = pAuxTenkai;
				JTTenkai::SetJTTPara( pAuxTenkai);
				break;
			case MDR_ATRCD_HAITIEN_VERSION:										// �o�[�W����
				ASSERT( iSize == sizeof(MhHistV));
				pVer = (MhHistV*)pEnt;
				m_pVer = pVer;
				break;
			case MDR_ATRCD_HAITIEN_OPTION1:										// �n�o�s�Q
				ASSERT( iSize == sizeof(MsBitSet));
				pOpt = (MsBitSet*)pEnt;
				m_pOpt1 = pOpt;
				break;
			case MDR_ATRCD_HAITIEN_OPTION2:										// �n�o�s�Q
				ASSERT( iSize == sizeof(MsBitSet));
				pOpt = (MsBitSet*)pEnt;
				m_pOpt2 = pOpt;
				break;
			case MDR_ATRCD_OPTION_KANRI:										// �n�o�s�Q�Ǘ�
				MhOptV::nbmGOpt = iSize / sizeof( MhOpt);
				memcpy( MhOptV::bmGOpt, pEnt, iSize);
				MBFREE( pEnt);
				break;
			case MDR_ATRCD_VERSION_KANRI0:										// �����Ǘ�
				nVer = 0;
				memcpy( &MhHistV::nVer, pEnt, iSize);
				MBFREE( pEnt);
				break;
			case MDR_ATRCD_VERSION_KANRI1:										// �����Ǘ��@�o�[�W����
				MhHistV::sVerC[nVer] = (MCHAR*)pEnt;
				MBFREE( pEnt);
				break;
			case MDR_ATRCD_VERSION_KANRI2:										// �����Ǘ��@���O�o�[�W����
				MhHistV::sVerB[nVer] = (MCHAR*)pEnt;
				MBFREE( pEnt);
				break;
			case MDR_ATRCD_VERSION_KANRI3:										// �����Ǘ��@���t
				MhHistV::sVerD[nVer++] = (MCHAR*)pEnt;
				MBFREE( pEnt);
				break;
			case MDR_ATRCD_HAITIEN_ZUKEI:
				pZukei = (MhZukei*)pEnt;
				szZukei = sizeof( MhZukei) + sizeof( MgLine3) * (pZukei->m_nZukei - 1);
				ASSERT( iSize == szZukei);
				SetPIZukei( pZukei);
				GetPIZukei()->m_pNext = 0;
				break;
			case MDR_ATRCD_HAITIEN_ZUKEIN:
				pZukei->m_pNext = (MhZukei*)pEnt;								// ���O�̐}�`�f�[�^�Ƀ����N������
				pZukei = (MhZukei*)pEnt;
				szZukei = sizeof( MhZukei) + sizeof( MgLine3) * (pZukei->m_nZukei - 1);
				ASSERT( iSize == szZukei);
				pZukei->m_pNext = 0;
				break;
			default:
				MBFREE( pEnt);
		}
	} while ( !bEor);
    return ist;
}

////////////////////////////////////////////////////////////////////////////
//	���ޔz�u���R�[�h��������
//	[�Ԓl] �X�e�C�^�X     0 : ����  -1 : �ُ�

MINT MhPlcInfo::Save(							//
				MhMdlIO*		i_phMdl			// �������݃t�@�C���@�n���h��
				)
{
	MINT		iDumy = 0;
//	MhZukei*	pZukei;
//	MINT		szZukei;

	i_phMdl->Write( MDW_ATRCD_HAITIEN, &iDumy);									// �z�u���R�[�h
		
//	MINT iKai = GetPIKai();														// �K
	i_phMdl->Write( MDW_ATRCD_HAITIEN_IKAI, &m_iKai);
//	MINT iCdHgt = GetPICdHgt();													// ���t�������R�[�h
	i_phMdl->Write( MDW_ATRCD_HAITIEN_ICDHGT, &m_iCdHgt);
//	MINT iDisplayFlag = GetPIDisplayFlg();										// �\���t���O(�}�`,���L1,���L2)
	i_phMdl->Write( MDW_ATRCD_HAITIEN_DISPLAYFLAG, &m_iDisplayFlg);
//	MINT iPanelNo = GetPIPanelNo();												// �p�l���ԍ�
	i_phMdl->Write( MDW_ATRCD_HAITIEN_IPANELNO, &m_iPanelNo);
//	MINT iKaikoNo = GetPIKaikoNo();												// �J���ԍ�
	i_phMdl->Write( MDW_ATRCD_HAITIEN_IKAIKONO, &m_iKaikoNo);
	MINT iCdBuzaiW = GetPTCdBuzaiW();											// ���ރR�[�h
	i_phMdl->Write( MDW_ATRCD_HAITIEN_IPARTSTPCD, &iCdBuzaiW);
//	MgLine3 lnPlc = GetPIPlcIti();												// p[0]: �n�_ | �z�u�_
																				// p[1]: �I�_ | �z�u�����_
	i_phMdl->Write( MDW_ATRCD_HAITIEN_LNHAITI, (MREAL*)&m_lnPlc, sizeof(MgLine3));
//	MgPoint3 ptUpPlc = GetPIUpPlc();											// �z�u������_
	i_phMdl->Write( MDW_ATRCD_HAITIEN_PTUPHAITI, (MREAL*)&m_vUpPlc, sizeof(MgPoint3));
//	MREAL rSinZure = GetPISinZure();											// �ގ��c����ʁ@	>0:�E���A<0:����
	i_phMdl->Write( MDW_ATRCD_HAITIEN_RSINZURE, &m_rSinZure);
//	MREAL rPlcZure = GetPIPlcZure();											// �z�u�_�����		>0:�O�A�@<0:��
	i_phMdl->Write( MDW_ATRCD_HAITIEN_RHAITIZURE, &m_rPlcZure);
//	MREAL* prLngHosei = GetPILenHosei();										// �����␳�l�i�n�_���A�I�_���j
	i_phMdl->Write( MDW_ATRCD_HAITIEN_RNAGASAHOSEI, m_rLngHosei, 2);
//	MREAL* prHgt = GetPIHgt();													// �����i���t�������A�����j
	i_phMdl->Write( MDW_ATRCD_HAITIEN_RTAKASA, m_rHgt, 2);
//	MgPoint2 ptCmnt1 = GetPICmntPt();											// ���L�\���ʒu
	i_phMdl->Write( MDW_ATRCD_HAITIEN_PTCMNT1, (MREAL*)&m_ptCmnt1, sizeof(MgPoint2));
	MCHAR	cMemBerCode[16];
	Mstrcpy( cMemBerCode, GetMbCdMbrW());										// ���@�^�� (�������ݗp�@�ʏ�m_cCode�Ɠ��� VerUp���ɕύX�������e������)
	i_phMdl->Write( MDW_ATRCD_HAITIEN_MEMBER, cMemBerCode, sizeof(cMemBerCode));
	if ( GetPIAuxAttr() != 0) {
		if ( IsTategu())														// ������
//			i_phMdl->Write( MDW_ATRCD_HAITIEN_TATEGU, GetPIAuxTategu(), sizeof(MhTateguInfo));
			GetPIAuxTategu()->Save( i_phMdl);
		else if ( IsTenkai())														// �Z���W�J
//			i_phMdl->Write( MDW_ATRCD_HAITIEN_TENKAI, GetPIAuxTenkai(), sizeof(JTTenkaiPr));
			GetPIAuxTenkai()->Save( i_phMdl);
	}
//	if ( GetPIVer() != 0)														// �o�[�W����
	if ( m_pVer != 0)															// �o�[�W����
		i_phMdl->Write( MDW_ATRCD_HAITIEN_VERSION, (MUINT*)m_pVer, sizeof(MhHistV));
//	if ( GetPIOpt1() != 0)														// �n�o�s�Q
	if ( m_pOpt1 != 0)															// �n�o�s�Q
		i_phMdl->Write( MDW_ATRCD_HAITIEN_OPTION1, (MUINT*)m_pOpt1, sizeof(MsBitSet));
//	if ( GetPIOpt2() != 0)														// �n�o�s�Q
	if ( m_pOpt2 != 0)															// �n�o�s�Q
		i_phMdl->Write( MDW_ATRCD_HAITIEN_OPTION2, (MUINT*)m_pOpt2, sizeof(MsBitSet));

//	if ( (pZukei = GetPIZukei()) != 0) {										// �}�`
	if ( m_pZukei != 0) {										// �}�`
//S		szZukei = sizeof( MhZukei) + sizeof( MgLine3) * (pZukei->m_nZukei - 1);
//		i_phMdl->Write( MDW_ATRCD_HAITIEN_ZUKEI, pZukei, szZukei);
//		for ( pZukei=pZukei->m_pNext; pZukei != 0; pZukei=pZukei->m_pNext) { 
//			szZukei = sizeof( MhZukei) + sizeof( MgLine3) * (pZukei->m_nZukei - 1);
//			i_phMdl->Write( MDW_ATRCD_HAITIEN_ZUKEIN, pZukei, szZukei);
//		}
		m_pZukei->Save( i_phMdl);
	}
	i_phMdl->Write( MDW_ATRCD_HAITIEN_EOR, &iDumy);								// �z�u���R�[�h�I��

	return 0;
}

////////////////////////////////////////////////////////////////////////////
//	���ޔz�u���R�[�h ������@��������
//	[�Ԓl] �X�e�C�^�X     0 : ����  -1 : �ُ�

MINT MhTateguInfo::Save(							//
				MhMdlIO*		i_phMdl			// �������݃t�@�C���@�n���h��
				)
{
	MINT			iDumy = 0;
	//MhZukei*		pZukei;
	//MINT			 szZukei;

	i_phMdl->Write( MDW_ATRCD_HAITIEN_TATEGU, &iDumy);							// �z�u���R�[�h
		
	MCHAR	cKCode[MHTTSZ_CODE+1];
	Mstrcpy( cKCode, GetKCode());												// �J���R�[�h
	i_phMdl->Write( MDW_ATRCD_HAITIEN_CCODE, cKCode, sizeof(cKCode));
	MREAL rROW = GetROW();														// �J����
	i_phMdl->Write( MDW_ATRCD_HAITIEN_RROW, &rROW);
	MREAL rROH = GetROH();														// �J������
	i_phMdl->Write( MDW_ATRCD_HAITIEN_RROH, &rROH);
	MREAL rHeight= GetHeight();													// ���t������(�܂������[����)
	i_phMdl->Write( MDW_ATRCD_HAITIEN_RHEIGHT, &rHeight);
	MREAL* prMUSz =	GetMUSz();													// ���E�܂����󒷂�
	i_phMdl->Write( MDW_ATRCD_HAITIEN_RMUSZ, prMUSz, 2);

	i_phMdl->Write( MDW_ATRCD_HAITIEN_TATEGU_EOR, &iDumy);						// �z�u���R�[�h�I��

	return 0;
}

////////////////////////////////////////////////////////////////////////////
//	���ޔz�u���R�[�h �}�`��� ��������
//	[�Ԓl] �X�e�C�^�X     0 : ����  -1 : �ُ�

MINT MhZukei::Save(							//
				MhMdlIO*		i_phMdl			// �������݃t�@�C���@�n���h��
				)
{
	MINT			iDumy = 0;

	i_phMdl->Write( MDW_ATRCD_HAITIEN_TATEGU, &iDumy);							// �}�`���R�[�h
		

	i_phMdl->Write( MDW_ATRCD_HAITIEN_TATEGU_EOR, &iDumy);						// �}�`���R�[�h�I��

	return 0;
}

////////////////////////////////////////////////////////////////////////////
//	�Z���W�J���R�[�h ��������
//	[�Ԓl] �X�e�C�^�X     0 : ����  -1 : �ُ�

MINT JTTenkaiPr::Save(							//
				MhMdlIO*		i_phMdl			// �������݃t�@�C���@�n���h��
				)
{
	MINT			iDumy = 0;

	i_phMdl->Write( MDW_ATRCD_HAITIEN_TATEGU, &iDumy);							// �}�`���R�[�h
		

	i_phMdl->Write( MDW_ATRCD_HAITIEN_TATEGU_EOR, &iDumy);						// �}�`���R�[�h�I��

	return 0;
}

} // namespace MC
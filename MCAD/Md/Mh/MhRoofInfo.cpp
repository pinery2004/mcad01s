//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MhRoofInfo.cpp
//
//		�����ǂݏ���
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"
#include "MgLib.h"

#include "MdLib.h"

#include "MsBitSet.h"
#include "MmOpt.h"
#include "MmHist.h"

#include "MhLib.h"
#include "MhRoof.h"

#include "MbFile.h"
#include "MmTenkai.h"

#include "MhDefine.h"

//#include "Heap.h"
class Heap
{
    private:
	unsigned long getCombinedSizesOfProcessHeaps(void);
	
    public:
	Heap();
	virtual ~Heap();
	
	bool ValidateAllHeaps(void);
	bool ValidateHeap(HANDLE heap);
	
	unsigned long GetCombinedSizesOfProcessHeaps(void);
	
	unsigned long GetHeapSize(HANDLE heap);
};

PZBUF*	____pzbuf;

//void MdPtsDbInit();
//void MdRedoPtsDbInit();
//void MdRoofDbInit();

namespace MC
{

MINT MhRoofInfo::Load(								//
				MhMdlIO*		i_phMdl		// �������݃t�@�C���@�n���h��
				)
{
	MINT		ist = 0;
	MhMdlIO		hMdl;
	MINT		iType;
	MINT		iSize;
	void*			pEnt;
	MhPlcInfo*		pPlcEn = 0;
//	MhRoofInfo*		pRoofEn;
	MhJim*			pJim1;
	MhRfm*			pRfm;
	MgPoint3*		pPt;
	MINT*			piCd;

	bool			fPlcSzChk = true;
	bool			fRoofSzChk = true;

	bool			bEor = false;

	MgVect3			vLnPlc;

//	�������R�[�h
//	ASSERT( iSize == sizeof(MhRoofInfo));
	//if ( i_iSize == sizeof(MhRoofInfo)) {
	//	pRoofEn = *(MhRoofInfo**)io_ppEnt;
	//}else {
	//	if ( fRoofSzChk) {
	//		MessageBox( NULL, Mstr( "�����f�[�^���R�[�h�T�C�Y�G���["), Mstr( "���R�[�h�T�C�Y�`�F�b�N"), MB_OK);
	//		fRoofSzChk = false;
	//	}
	//	pRoofEn = (MhRoofInfo*)realloc( *io_ppEnt, sizeof( MhRoofInfo));
	//}
//	pRoofEn = (MhRoofInfo*)malloc( sizeof( MhRoofInfo));
	this->InitAllAtr( 0);										// �q�̃T�C�Y�ƃ��R�[�h�����N���A

	do {
		iSize = i_phMdl->Read( &iType, (void**)&pEnt);
//===
		static	int iCyane = 0;
		TRACE( "%d; iType=%d\n", ++iCyane, iType);

//    HANDLE hHeaps[10000];
//	
//    DWORD numHeaps = GetProcessHeaps( 10000, hHeaps);
//	__debugbreak();												// �u���[�N�|�C���g
////S	for( unsigned int i=0; i < numHeaps; i++) {
////	    if ( !ValidateHeap(hHeaps[i])) {
////			__debugbreak();
////		}
////	}
////---

		if ( iSize <= 0)
			break;

		switch( iType) {
			case MDR_ATRCD_ROOFEN_EOR:											// ���� eor
				ASSERT( iSize == sizeof(MINT));
				MBFREE( pEnt);													// MDR_ATRCD_ROOFEN_EOR���R�[�h�p�G���A�̍폜

				CreateRfmZukei();
				bEor = true;
				break;
			case MDR_ATRCD_JIM:													// �n���
				ASSERT( iSize == sizeof(MhJim));
				pJim1 = (MhJim*)pEnt;

				*(this->GetpGJim1()) += (*pJim1);				// ���Ή��@MDR_ATRCD_JIM_EOR�Ȃ��̈�	ZZZZZZZZZZZZZ 091018
				MBFREE( pJim1);									// ���Ή�

				break;
			case MDR_ATRCD_JIM_EOR:												// �n���
				ASSERT( iSize == sizeof(MINT));
				MBFREE( pEnt);													// MDR_ATRCD_JIM_EOR���R�[�h�p�G���A�̍폜

				*(this->GetpGJim1()) += (*pJim1);
				MBFREE( pJim1);	
				break;
			case MDR_ATRCD_RFM:													// ������
				ASSERT( iSize == sizeof(MhRfm));
				pRfm = (MhRfm*)pEnt;
				break;
			case MDR_ATRCD_RFM_EOR:												// ������ eor
				ASSERT( iSize == sizeof(MINT));
				MBFREE( pEnt);													// MDR_ATRCD_RFM_EOR���R�[�h�p�G���A�̍폜

				*(this->GetpGRfm()) += (*pRfm);
				pRfm->Free();
				MBFREE( pRfm);
				break;
			case MDR_ATRCD_RFM_PG_PT:												// �����ʍ\���_ 
				ASSERT( iSize == sizeof(MgPoint3) * pRfm->m_Pg.m_n);
				pPt = (MgPoint3*)pEnt;
				pRfm->m_Pg.m_P = pPt;
				break;
			case MDR_ATRCD_RFM_ICD_I:												// �����ʃR�[�h
				ASSERT( iSize == sizeof(MINT) * pRfm->m_cdPg.m_n);
				piCd = (MINT*)pEnt;
				pRfm->m_cdPg.m_i = piCd;
				break;
			default:
				MBFREE( pEnt);
		}
	} while( !bEor);
    return ist;
}

////////////////////////////////////////////////////////////////////////////
//	�����z�u���R�[�h��������
//	[�Ԓl] �X�e�C�^�X     0 : ����  -1 : �ُ�

MINT MhRoofInfo::Save(							//
				MhMdlIO*		i_phMdl			// �������݃t�@�C���@�n���h��
				)
{
	MINT		ic1;
	MINT		iDumy = 0;

//	i_phMdl->Write( MDW_ATRCD_ROOFEN, sizeof(MINT), &iDumy);
	i_phMdl->Write( MDW_ATRCD_ROOFEN, &iDumy, sizeof(MINT));

//	i_phMdl->Write( MDW_ATRCD_ROOFEN, this, sizeof(MhRoofInfo));

	i_phMdl->Write( MDW_ATRCD_ROOFEN_KAI, &m_iKai, sizeof(MINT));

//class MhRoofInfo: public MOBJECT
//{
//
//	MINT				m_iKai;						// �K( 1, 2, 3)
//	MgGrpS<MhJim>		m_GJim1;					// �n�����i���́j
//													// �i�i���_�E�n�����R�[�h�E���̓t���O)n�j
//	MgPoint2			m_ptH;						// �����������_�i���́j
//	MgGrpS<MgKs1>		m_GKs1;						// �\�����i���́j
//													// �i�i�\�������E�\���R�[�h�E�\�������)n�j
//	MgGrpF<MhRfm>		m_GRfm;						// ������
//													// �i�i(���_�E�\���R�[�h)n)n�j
//	MgGPolyg3			m_GNUm;						// ������
//													// �i�i(���_)n)n�j
//	MgGPolyg3			m_GHm;						// �ւ��
//													// �i�i(���_)n)n�j
//	
////	MhRoofInfo*			m_Oya;						// �e����
//	MINT				m_itpRf;					// �����^�C�v
//	MREAL				m_rKb;						// ���z
//	MREAL				m_rRkH;						// ����������ύX��
//	MREAL				m_rRfmA;					// �����ʌ���(89,140,182)
//	MREAL				m_rNUmA;					// �����ʌ���(89,140)
//	MREAL				m_rMnZ[2];					// ������ʂP�E�Q
//	MREAL				m_rNkD[2];					// ���̏o
//	MREAL				m_rKrD[2];					// ����΂̏o
//	MINT				m_itpNU;					// �����^�C�v�P�E�Q
//	MINT				m_itpNS;					// ����^�C�v�P�E�Q
//	MREAL				m_rHKmA;					// �@�B���ʌ���
//	MREAL				m_rNUk;						// �����Ԃ���
//	MREAL				m_rNkH;						// ���̍����i��`�h�[�}�p�j
//	MINT				m_icdURS;					// �������R�[�h
////	MINT				m_icdRR;					// �剮���R�[�h
//	MhZukei*			m_pZukei;					// �}�`
//
//	MINT				m_imdDsp;					// �\�����[�h (0: ������  1:�\����)
//	COLORREF			m_colDispD;					// �\���F�i�f�[�^�w��j
//	COLORREF			m_colDispC;					// �\���F�i���݁j
////	MnDisp*				m_iDispM;					// �\�����i�ʁj
////	MnDisp*				m_iDispL;					// �\�����i�����j
//
//	MINT				m_nRtm;						// ���ʐ�
//	MINT*				m_pifRitm;					// ���ʕ\��(�ʃt���O)  0:��  1:��
//	MINT*				m_pnPtRitm;					// ���ʕ\��(���_��)
//	MgPoint3*			m_pPtRitm;					// ���ʕ\��(���_)
////	MnDisp*				m_pLnRitm;					// ���ʕ\�����i�����j
//
////	MhTexture*			m_KsInfo[NKSINFO];			// �\�����
//
//	MsBitSet*			m_pVer;					// �o�[�W�������� (0: �S�o�[�W�����ŕ\��)
//	MsBitSet*			m_pOpt1;					// �\���I��OPT�Q�Z�b�g (NULL: �I�v�V�����ݒ薳���ł��\��)
//	MsBitSet*			m_pOpt2;					// �\���I�tOPT�Q�Z�b�g (NULL: ����)
//


	for ( ic1=0; ic1 < GetpGJim1()->m_n; ic1++) {
//		i_phMdl->Write( MDW_ATRCD_JIM, &GetpGJim1()->m_pst[ic1], sizeof(MhJim));
//		i_phMdl->Write( MDW_ATRCD_JIM_EOR, &iDumy, sizeof(MINT));

		GetpGJim1()->m_pst[ic1].Save( i_phMdl);
	}

	for ( ic1=0; ic1 < GetpGRfm()->m_n; ic1++) {
		if ( GetpGRfm()->m_pst[ic1].m_Pg.m_n != 0) {
//			i_phMdl->Write( MDW_ATRCD_RFM, &GetpGRfm()->m_pst[ic1], sizeof(MhRfm));
//			i_phMdl->Write( MDW_ATRCD_RFM_PG_PT, (MREAL*)GetpGRfm()->m_pst[ic1].m_Pg.m_P, sizeof(MgPoint3) * GetpGRfm()->m_pst[ic1].m_Pg.m_n);
//			i_phMdl->Write( MDW_ATRCD_RFM_ICD_I, GetpGRfm()->m_pst[ic1].m_cdPg.m_i, sizeof(MINT) * GetpGRfm()->m_pst[ic1].m_cdPg.m_n);
//			i_phMdl->Write( MDW_ATRCD_RFM_EOR, &iDumy, sizeof(MINT));

			GetpGRfm()->m_pst[ic1].Save( i_phMdl);
		}
	}
	i_phMdl->Write( MDW_ATRCD_ROOFEN_EOR, &iDumy, sizeof(MINT));

    return 0;
}

////////////////////////////////////////////////////////////////////////////
//	�����z�u���R�[�h��������
//	[�Ԓl] �X�e�C�^�X     0 : ����  -1 : �ُ�

MINT MhJim::Save(							//
				MhMdlIO*		i_phMdl			// �������݃t�@�C���@�n���h��
				)
{
//	MINT		ic1;
	MINT		iDumy = 0;

	i_phMdl->Write( MDW_ATRCD_JIM, &iDumy, sizeof(MINT));
	i_phMdl->Write( MDW_ATRCD_JIM_P, &iDumy, sizeof(MINT));
	i_phMdl->Write( MDW_ATRCD_JIM_ICD, &iDumy, sizeof(MINT));
	i_phMdl->Write( MDW_ATRCD_JIM_IFINP, &iDumy, sizeof(MINT));
	i_phMdl->Write( MDW_ATRCD_JIM_RKB, &iDumy, sizeof(MINT));
	i_phMdl->Write( MDW_ATRCD_JIM_IRFM, &iDumy, sizeof(MINT));
	i_phMdl->Write( MDW_ATRCD_JIM_LNR, &iDumy, sizeof(MINT));
	i_phMdl->Write( MDW_ATRCD_JIM_ILNRCD, &iDumy, sizeof(MINT));
	i_phMdl->Write( MDW_ATRCD_JIM_EOR, &iDumy, sizeof(MINT));

	return 0;
}

MINT MhRfm::Save(
				MhMdlIO*		i_phMdl			// �������݃t�@�C���@�n���h��
				)
{
	MINT			iDumy = 0;

	i_phMdl->Write( MDW_ATRCD_HAITIEN_TATEGU, &iDumy);							// �}�`���R�[�h
		

	i_phMdl->Write( MDW_ATRCD_HAITIEN_TATEGU_EOR, &iDumy);						// �}�`���R�[�h�I��

	return 0;
}

} // namespace MC
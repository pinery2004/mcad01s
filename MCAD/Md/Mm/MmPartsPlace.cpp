//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MmPtsPlc.cpp
//
//		���i�z�u����
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

#define DLL_EXPORT_MC_HAITIIN_DO
#include "MmLib.h"

#include "MmSystemProperty.h"

#include "resource.h"
#include "..\\MCAD\\resource.h"

#include "MsBitSet.h"
#include "MbList.h"
#include "MhLib.h"

#include "MhInp.h"

#define	DLL_EXPORT_MC_INPATTR_DO 
#include "MmInpAttr.h"

#include "MmOpt.h"
#include "MdLib.h"

#include "MmHist.h"
#include "MmValid.h"

#define	 DLL_EXPORT_HAITICMD_DO
#include "MmCmd.h"

#include "MhPts1.h"

namespace MC
{

/////////////////////////////////////////////////////////////////////////////
//	���i���P�{�z�u����
void HaitiCmd::MmPtsPlc(
				const	MgPoint3	*Pt,		// (I  ) �z�u�_�A�z�u�����_
				const	MgVect3		vUpPlc,		// (I  ) �z�u�����
				const	MgPolyg2	*ppg		// (I  ) �}�`�p�̋��@�܂��� NULL
				)
{
	MhPlcInfo	PlcEn;
	MhZukei*	pZukei;
	MINT		szZukei;
	MgVect3		vBz = Pt[1] - Pt[0];
	MREAL		rBzLng_2 = vBz * vBz;
	if ( rBzLng_2 < MMIN_BZILEN_2) {
		//	���ޒ����s���G���[�\�� **************************************
		MQUIT;
	}
/*D
	MsBitSet	*pOptv;
	MsBitSet	*pHstv;

	MhAddOpt( &pOptv);							// Opt�Ή��@�V�K�ǉ����R�[�h
	PlcEn.SetPIOpt1( pOptv);					// Opt�Ή�
	MhHistAdd( &pHstv);							// �����Ǘ��Ή��@�V�K�ǉ����R�[�h
	PlcEn.SetPIVer( pHstv);						// �����Ǘ��Ή�
*/
//D	MdPtsAdd( &PlcEn, 1);
	
	PlcEn.SetPIKai( MhPlcInp::GetKai());
	PlcEn.SetPIIdTpPts( MhPlcInp::GetIdTpPts());
	PlcEn.SetPITpPts( MhPlcInp::GetTpPts());
	PlcEn.SetPIIdMbr( MhPlcInp::GetIdMbr());
	PlcEn.SetPIMbr( MhPlcInp::GetMbr());
	PlcEn.SetPILenHosei( 0, MhPlcInp::GetLenHosei( 0));
	PlcEn.SetPILenHosei( 1, MhPlcInp::GetLenHosei( 1));
	if ( PlcEn.IsPanel() || PlcEn.IsKaiko()) {
		PlcEn.SetPIMaeHosei( MhPlcInp::GetMaeHosei());
		PlcEn.SetPIOku( MhPlcInp::GetOku());
		PlcEn.SetPIOkuHosei(MhPlcInp::GetOkuHosei());
		PlcEn.SetPIHgt( 0, MhPlcInp::GetHgt( 0));
	} else {
		PlcEn.SetPISinZure( MhPlcInp::GetSinZure());
		PlcEn.SetPIPlcZure( MhPlcInp::GetPlcZure());
		PlcEn.SetPIHgt( 0, MhPlcInp::GetHgt( 0));
		PlcEn.SetPIHgt( 1, MhPlcInp::GetHgt( 1));
	}

	if ( PlcEn.IsFrame()) {										// ���Ęg | ���@�̏�[������ݒ�
		if ( MGeo::GT( PlcEn.m_rHgt[1], 0.f)) {
			PlcEn.SetPIHgt( 1, PlcEn.m_rHgt[1]);
		} else {
			PlcEn.SetPIHgt( 1, MmSystemProperty::GetStnd( PlcEn.m_iKai, MM_STNDH_CEILING) -	// �V��
							   MmSystemProperty::GetStnd( PlcEn.m_iKai, MM_STNDH_LOWER) +		// ���
							   PlcEn.m_rHgt[1]);
		}
	}
	PlcEn.SetPIPanelNo( MhPlcInp::GetPanelNo());
	PlcEn.SetPIKaikoNo( MhPlcInp::GetKaikoNo());

	PlcEn.SetPIPlcIti( 0, Pt[0]);
	PlcEn.SetPIPlcIti( 1, Pt[1]);
	PlcEn.SetPIUpPlc( vUpPlc);

	PlcEn.SetPIZukei( NULL);
	PlcEn.SetPIAuxAttr( MhPlcInp::GetpAuxAttr());
	MhPlcInp::SetpAuxAttr( NULL);

	if ( ppg != NULL && ppg->m_n>=2) {							// �}�`�p�̋��f�[�^����
		MINT nLine = ppg->m_n;
		if ( ppg->m_n == 2) nLine--;								// 2�_�̏ꍇ�͒���1�{

		szZukei = sizeof(MhZukei) + sizeof(MgLine3) * (nLine - 1);			// ����[nLine]�{���̐}�`�f�[�^�T�C�Y
		pZukei = (MhZukei*)new char[szZukei];

		pZukei->m_obj = MHZK_DISP;								// �}�`�\���p
		pZukei->m_iZTp = MHZK_GLINE;								// �����Q

		pZukei->m_pNext = NULL;
		pZukei->m_nZukei = nLine;
		for (MINT ic1=0; ic1<nLine; ic1++) {
			MINT ic2 = ic1 + 1;
			if ( ic2 == nLine) ic2 = 0;
			pZukei->m_lnZukei[ic1] = MgLine3( MgVect3C( ppg->m_p[ic1]), MgVect3C( ppg->m_p[ic2]));
		}
		PlcEn.m_pZukei = pZukei;
		//
		szZukei = sizeof(MhZukei) - sizeof(MgLine3) + sizeof(MgPoint3) * (ppg->m_n);	// �_[ppg->n]���̐}�`�f�[�^�T�C�Y
		pZukei = (MhZukei*)new char[szZukei];

		pZukei->m_obj = MHZK_AREA;								// �}�`�̈攻��p
		pZukei->m_iZTp = MHZK_GPOINT;								// �|���S��(�_�Q)

		pZukei->m_pNext = NULL;
		pZukei->m_nZukei = ppg->m_n;
		MgPoint2* ppt = (MgPoint2*)&pZukei->m_lnZukei[0];
		for (MINT ic1=0; ic1<ppg->m_n; ic1++) {
			ppt[ic1] = ppg->m_p[ic1];
		}
		PlcEn.m_pZukei->m_pNext = pZukei;
	}
//D	MdInjectPts( &PlcEn);
	HaitiDb::MdPtsAdd( &PlcEn, 1);
exit:;
}

/////////////////////////////////////////////////////////////////////////////
//	���i�z�u���擾����
//	�Ԓl =-1: �I�[�o�[�t���[, ��0: �I�𕔕i�z�u��
MINT HaitiIn::GetPts(
						MINT		iKai,			// (I  ) �K
						MINT		iIdTpPts,		// (I  ) ���iID
				const	MCHAR*		cGeneralName,	// (I  ) ���� �܂��� NULL
				const	MCHAR*		cNmPts1,		// (I  ) ����p���ޖ� �܂��� NULL
						MINT		szPlcEn,		// (I  ) ���i�z�u�ő吔
						MhPlcInfo*	*pPlcEno,		// (  O) �I�𕔕i�z�u
						MPOSITION	*pPlcpos		// (  O) �I��[���i�z�u]�ʒu �܂��� NULL
				)
{
	MPOSITION	pos1;
	MhPlcInfo* pPlcEn;
//	MsBitSet	*pOptv;
//	MsBitSet*	pHstv;

//	MINT szHE = szPlcEn - 1;
	MINT szHE = szPlcEn;

	MINT iHE = 0;
	for (pPlcEn = HaitiDb::MdGetHeadPts( &pos1); pPlcEn!=0;
		 pPlcEn = HaitiDb::MdGetNextPts( &pos1)) {
		if ( !HaitiIn::ChkPts( iKai, iIdTpPts, cGeneralName, cNmPts1, pPlcEn))
			continue;											// �ΏۊO���i�z�u
		if ( !MmChkValidParts( pPlcEn))							// �I�v�V�����Ɨ����̃`�F�b�N
			continue;

		if ( iHE >= szHE) {										// �I�𕔕i�z�u�@�I�[�o�t���[�@<ERROR>
			iHE = -1;
			break;
		}
		pPlcEno[iHE] = pPlcEn;
		if ( pPlcpos)
			pPlcpos[iHE] = pos1;
		iHE++;
	}
	return iHE;
}

/////////////////////////////////////////////////////////////////////////////
//	���i�z�u�𒲂ׂ�
//	�Ԓl true : �Ώە��i�z�u, false : �ΏۊO���i�z�u
bool HaitiIn::ChkPts(
						MINT		iKai,			// (I  ) �K �܂��� NULL
						MINT		iIdTpPts,		// (I  ) ���iID �܂��� NULL
				const	MCHAR*		cGeneralName,	// (I  ) ���� �܂��� NULL
				const	MCHAR*		cNmPts1,	// (I  ) ����p���ޖ� �܂��� NULL
						MhPlcInfo	*pPlcEn1		// (I  ) �������i�z�u
				)
{
	bool	bSt = false;
	if ( iKai != NULL && pPlcEn1->GetPIKai() != iKai)
		MQUIT;													// �قȂ�K�̕��ނ͑ΏۊO
	if ( iIdTpPts != NULL && pPlcEn1->GetPTCdGp() != iIdTpPts)
		MQUIT;													// �قȂ�\���̕��ނ͑ΏۊO
	if ( cGeneralName != NULL &&
		Mstrcmp( pPlcEn1->GetPTNmGeneral(), cGeneralName) != 0)
		MQUIT;													// �قȂ鑍�̂̕��ނ͑ΏۊO
	if ( cNmPts1 != NULL &&
		Mstrcmp( pPlcEn1->GetPTNmPts1(), cNmPts1) != 0)
		MQUIT;													// �قȂ鑀��p���ޖ��̕��ނ͑ΏۊO
	bSt = true;
exit:
	return bSt;
}

#define		MC_HITBZI_SZ	20
/////////////////////////////////////////////////////////////////////////////
//  ���ޔz�u����������
//	�Ԓl �������ʁ@�܂��́@null:�q�b�g�Ȃ�
MhPlcInfo* HaitiIn::SrchBuzai(
						MmWndInfo*	pWndInfo,	// (I  ) �E�B���h�E�Ǘ����
						MgPoint2&	ptMouthR,	// (I  ) �����w�����W
						MINT		iCdBuzai,	// (I  ) ���ރR�[�h�@�܂��́@NULL(�S)
												//       MP_BZICD_PANEL(�S�p�l��)
						MgPolyg2*	ppgPtsShape	// (  O) ���o���p�`�@�܂��́@NULL
				)
{
	MINT		ist1;
	MINT		ic1;
	MPOSITION	pos1;
	MhPlcInfo	*pPlcEn;
//	MsBitSet	*pOptv;
//	MsBitSet*	pHstv;														// �����Ǘ��Ή�
	MhPlcInfo	*pHitBziEn[MC_HITBZI_SZ];
	MREAL		rAHitBziEn[MC_HITBZI_SZ];
	MgPolyg2	pgHitBzi;
	MREAL		rArea;

	MINT		nHitBziEn = 0;
	MINT		iHitEnMin = 0;
   
	MINT		iKaiC   = InpMode::GetKai();						// �K  	(1,2,3)
	MINT		iGpC = InpMode::GetKGp();							// �\��

	MREAL		rAMinHitBziEn = MREALMAX;

	MINT		iBuzaiType = iCdBuzai / MP_BZICD_TYPE;
	if ( iCdBuzai - iBuzaiType * MP_BZICD_TYPE) iBuzaiType = -1;

	for ( pPlcEn = HaitiDb::MdGetHeadPts( &pos1); pPlcEn!=0;
		  pPlcEn = HaitiDb::MdGetNextPts( &pos1)) {

		if ( pPlcEn->GetPIKai() != iKaiC)
			continue;											// �قȂ�K�̕��ނ͌������Ȃ�
		if ( pPlcEn->GetPTCdGp() != iGpC)
			continue;											// �قȂ�\���̕��ނ͌������Ȃ�
			
		if ( !MmChkValidParts( pPlcEn))							// �I�v�V�����Ɨ����̃`�F�b�N
			continue;

		if ( iCdBuzai) {											// ���ރR�[�h���w�肳��Ă���ꍇ�̓`�F�b�N����
			if ( (pPlcEn->GetPTCdBuzai() / MP_BZICD_TYPE == iBuzaiType) ||
				(pPlcEn->GetPTCdBuzai() == iCdBuzai))
				;
			else
				continue;										// �قȂ镔�ރR�[�h�̕��ނ͌������Ȃ�
		} 

		HaitiIn::PtsShape( pPlcEn,	&pgHitBzi);							// ���ތ`������߂�
		// ���ތ`���MIN/MAX�����߂�
		MgMinMaxR2 rMinMax;
		rMinMax.SetInit();
		for ( ic1=0; ic1<pgHitBzi.m_n; ic1++) 
			rMinMax.Ins2( pgHitBzi.m_p[ic1]);
		// �w�����ނ�����
		if ( MGeo::ChkPt2InMMR2( ptMouthR, rMinMax)) {				// ���ތ`���MIN/MAX�Ō�����
			if ( MGeo::ChkPt2OnPg2WS( ptMouthR, pgHitBzi, &ist1)) {	// ����ɕ��ތ`��Ō�������
				pHitBziEn[nHitBziEn] = pPlcEn;
				// �����Y���̏ꍇ�͖ʐύŏ���I��
				rArea = pgHitBzi.Area();
				rAHitBziEn[nHitBziEn] = rArea;
				if ( rArea < rAMinHitBziEn) {
					rAMinHitBziEn = rArea;
					if ( ppgPtsShape)
						*ppgPtsShape = pgHitBzi;
					iHitEnMin = nHitBziEn;
				}
				nHitBziEn++;
//				return pPlcEn;									// �w������
			}
		}
	}
	if ( nHitBziEn)
		return pHitBziEn[iHitEnMin];
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
//	�����������ނɐڐG���镔�ނƌ������镔�ނ����߂�
void HaitiIn::MmSrchCrossBuzai(
						MmWndInfo	*pWndInfo,		// (I  ) �E�B���h�E�Ǘ����
						MhPlcInfo	*pBziInfo1,		// (I  ) ������������
						MINT		iCdBuzai,		// (I  ) �����Ώۂ̕��ރR�[�h
						MhPlcInfo	**pbTchBzi,		// (  O) ����������̕��ނɐڐG���镔��
						MhPlcInfo	**pbCrsBzi		// (  O) ����������̕��ނƌ������镔��
				)
{
	MINT		ist1;
	MINT		ic1;
	MINT		iTch = 0;
	MINT		iCrs = 0;
	MgLine2		lnBziSin1;
	MgPolyg2	pgBzi1;
	MPOSITION	pos1;
	MhPlcInfo	*pPlcEn;
	MgLine2		lnBziSin2;
	MgPolyg2	pgBzi2;
	MINT		nT;
	MINT		iIntr;
	MgPoint2	po;

	MINT		nBzi2En = 0;
	MINT		iHitEnMin = 0;
   
	MINT		iKaiC   = InpMode::GetKai();						// �K  	(1,2,3)
	MINT		iGpC = InpMode::GetKGp();							// �\��

	MREAL		rAMinBzi2En = MREALMAX;

	MINT		iBuzaiType = iCdBuzai / MP_BZICD_TYPE;
	if ( iCdBuzai - iBuzaiType * MP_BZICD_TYPE) iBuzaiType = -1;

	HaitiIn::PtsShape( pBziInfo1, &pgBzi1);							// �����������ނ̕��ތ`������߂�
	// �����������ނ̕��ތ`���MIN/MAX�����߂�
	MgMinMaxR2 rMinMax1;
	rMinMax1.SetInit();											// ���ތ`���MIN/MAX�����߂�
	for ( ic1=0; ic1<pgBzi1.m_n; ic1++) 
		rMinMax1.Ins2( pgBzi1.m_p[ic1]);
	BuzaiCode::MhBziSin( pBziInfo1, &lnBziSin1);							// �c�������߂�

	for ( pPlcEn = HaitiDb::MdGetHeadPts( &pos1); pPlcEn!=0;
		  pPlcEn = HaitiDb::MdGetNextPts( &pos1)) {

		if ( pPlcEn == pBziInfo1)
			continue;											// �����������ނ͖���

		if ( pPlcEn->GetPIKai() != iKaiC)
			continue;											// �قȂ�K�̕��ނ͌������Ȃ�
		if ( pPlcEn->GetPTCdGp() != iGpC)
			continue;											// �قȂ�\���̕��ނ͌������Ȃ�

		if ( !MmChkValidParts( pPlcEn))							// �I�v�V�����Ɨ����̃`�F�b�N
			continue;

		if ( iCdBuzai) {											// �����Ώۂ̕��ރR�[�h���w�肳��Ă���ꍇ�̓`�F�b�N����
			if ( (pPlcEn->GetPTCdBuzai() / MP_BZICD_TYPE == iBuzaiType) ||
				(pPlcEn->GetPTCdBuzai() == iCdBuzai))
				;
			else
				continue;										// �قȂ镔�ރR�[�h�̕��ނ͌������Ȃ�
		} 

		HaitiIn::PtsShape( pPlcEn,	&pgBzi2);							// �������ނ̕��ތ`������߂�
		// �������ނ̕��ތ`���MIN/MAX�����߂�
		MgMinMaxR2 rMinMax2;
		rMinMax2.SetInit();										// ���ތ`���MIN/MAX�����߂�
		for ( ic1=0; ic1<pgBzi2.m_n; ic1++)
			rMinMax2.Ins2( pgBzi2.m_p[ic1]);
		BuzaiCode::MhBziSin( pPlcEn, &lnBziSin2);							// �c�������߂�

		// �����܂��͒[���ڑ����ނ�����
		if ( MGeo::ChkMMR2OnMMR2( rMinMax1, rMinMax2)) {				// ���ތ`���MIN/MAX�ŏd�Ȃ肠�肩����������
			iIntr = MGeo::Intr2Ln2( lnBziSin2, lnBziSin1, &po);
			if ( iIntr != MC_PARALLEL) {							// �����܂��͐ڐG
				nT = 0;
				for ( ic1=0; ic1<pgBzi2.m_n; ic1++) {			// ���������ʂ̍\���_�������������ޓ��ɂ���ꍇ�͐ڐG�Ƃ݂Ȃ�
					if ( MGeo::ChkPt2OnPg2WS( pgBzi2.m_p[ic1], pgBzi1, &ist1))
						nT++;
				}
				if ( nT >= 1) {
//					MgVect2 vt0 = lnBziSin2.p[1] - lnBziSin2.p[0];
//					MgVect2 vt1 = lnBziSin1.p[0] - lnBziSin2.p[0];
//					MgVect2 vt2 = lnBziSin1.p[1] - lnBziSin2.p[0];
//					if ( MgLE( (vt1 ^ vt0) * (vt2 ^ vt0), 0)) 
						pbTchBzi[iTch++] = pPlcEn;
				} else if ( MF_CHECK_OR( iIntr, (MC_INT | MC_CONNECTION))) {
					pbCrsBzi[iCrs++] = pPlcEn;
				}
			}
		}
	}
	pbTchBzi[iTch] = NULL;
	pbCrsBzi[iCrs] = NULL;
}

/////////////////////////////////////////////////////////////////////////////
//  ���ނ̌`������߂�
void HaitiIn::PtsShape(
						MhPlcInfo	*pPlcEn,	// ���ޔz�u���R�[�h
						MgPolyg2*	pgPtsShape	// ���ތ`��
				)
{
	MINT		iKaiC   = InpMode::GetKai();						// �K  	(1,2,3)
	MINT		iGpC = InpMode::GetKGp();							// �\��

	MgLine2		LnPts;
	MgPoint2	ptPtsN;
	MINT		iKeijoF;
	MREAL		rWidthR, rWidth;
	MgVect2		VtWidthR, VtWidth;
	MgPoint2	ptW[2];
	MgVect2		VtW, VtUtW;
	MgPoint2	ptH;
	MgPoint2	ptK[4];
	MgVect2		vptH;
	MgVect2		vOffset = MgVect2( 0., 20.);
	
	MINT		ic1;

	ptW[0] = *(MgPoint2*)&(pPlcEn->GetPIPlcIti( 0));
	ptW[1] = *(MgPoint2*)&(pPlcEn->GetPIPlcIti( 1));

	// ���ނ̌`�����ߌ�������
	VtW = ptW[1] - ptW[0];										// �c��
	VtUtW = MGeo::UnitizeV2( VtW);									// ���ނ̌`�����߂�
	if ( pPlcEn->IsFrame()) {									// ���Ęg�p�̌`��쐬
		VtWidthR = (pPlcEn->GetMbTWidthR() + pPlcEn->GetPISinZure()) * VtUtW.RotR90(); 
		VtWidth = pPlcEn->GetMbTWidth() * VtUtW.RotL90(); 
		ptH = ptW[0] + pPlcEn->GetPIPlcZure() * VtUtW;
		ptK[0] = ptH + pPlcEn->GetMbTHeightF() * VtUtW + VtWidthR;
		ptK[1] = ptH - pPlcEn->GetMbTHeightB() * VtUtW + VtWidthR;
		ptK[2] = ptK[1] + VtWidth;
		ptK[3] = ptK[0] + VtWidth;
		iKeijoF = 0;											// �`��Z�o��
	} else if ( pPlcEn->IsPanel() || pPlcEn->IsKaiko()) {		// �p�l���`�� | �J���`��
//		if ( pPlcEn->IsYukaPanel() || pPlcEn->IsTenjoPanel()) {
		if ( pPlcEn->IsYuka() || pPlcEn->IsTenjo()) {
			rWidthR = pPlcEn->GetPIMaeHosei();					// ��O���␳
			rWidth = pPlcEn->GetPIOkuHosei() + pPlcEn->GetPIOku() +
							   pPlcEn->GetPIMaeHosei();			// ��O���␳ + ���s�� + ���s���␳
			iKeijoF = 2;
//		} else if ( pPlcEn->IsYanePanel()) {
		} else if ( pPlcEn->IsYane()) {
			rWidthR = pPlcEn->GetPIMaeHosei();					// ��O���␳
			rWidth = pPlcEn->GetPIOkuHosei() + pPlcEn->GetPIOku() +
							   pPlcEn->GetPIMaeHosei();			// ��O���␳ + ���s�� + ���s���␳
			iKeijoF = 2;
		} else {
			rWidthR = pPlcEn->GetMbTWidthR() + pPlcEn->GetPISinZure(); 
			rWidth = pPlcEn->GetMbTWidth(); 
			iKeijoF = 3;
		}
	} else {													// ���u���ނ̌`��쐬
		rWidthR = pPlcEn->GetMbWidthR() + pPlcEn->GetPISinZure(); 
		rWidth = pPlcEn->GetMbWidth(); 
		iKeijoF = 1;
	}

	if ( iKeijoF != 0) {											// �`��Z�o
		VtWidthR = rWidthR * VtUtW.RotR90(); 
		VtWidth = rWidth * VtUtW.RotL90(); 
		ptK[0] = ptW[0] - pPlcEn->GetPILenHosei( 0) * VtUtW + VtWidthR;
		ptK[1] = ptW[1] + pPlcEn->GetPILenHosei( 1) * VtUtW + VtWidthR;
		ptK[2] = ptK[1] + VtWidth;
		ptK[3] = ptK[0] + VtWidth;
	}

//L		pCod->SetLineAttr( MPS_SOLID, 1, PtsLineColor);
//		pZukei = pPlcEn->m_pZukei;
//		if ( pZukei) {											// �}�`�f�[�^�L��̏ꍇ�́A�����茟��
//			for ( ic=0; ic<pZukei->m_nZukei; ic++) {
//				pCod->Line( MgLine2C( pZukei->m_lnZukei[ic]));
//			}
//		} else {												// �}�`�f�[�^�Ȃ��̏ꍇ�͋��߂��`���茟��
	pgPtsShape->m_n = 0;

	for ( ic1=0; ic1<4; ic1++) 
		(*pgPtsShape) += ptK[ic1];
//			pCod->Polygon( ptK, 4);
//		}

//		if ( pPlcEn->GetPITpPts()->m_iBr == 2)
//			DrawPart( pCod, iGpC, pPlcEn);
}

/////////////////////////////////////////////////////////////////////////////
//	���ނ̔z�u�_�ƒ����␳�l���C��
void HaitiIn::MhModBzPH(
						MINT		iInpKbCd,		// (I  ) ���͓_�敪�R�[�h
						MINT		iMov,			// (I  ) �C�����@(0:�n�_�A1:�I�_)
				const	MgPoint3	&PtInt,			// (I  ) �z�u�_
						MREAL		rLH,			// (I  ) �����␮�l
						MhPlcInfo	*pPlcEnR		// (I O) ������������
				)
{
	MhPlcInfo	*pPlcEnM;							// ������������2

	if ( ( pPlcEnR->GetPIPlcIti( iMov) != PtInt) ||
		( !MGeo::Equal(pPlcEnR->GetPILenHosei( iMov), rLH)) ||
		 pPlcEnR->GetPIInpKbCd() == iInpKbCd) {

		HaitiDb::MdPtsModify( pPlcEnR, &pPlcEnM);
		iInpKbCd = pPlcEnM->GetPIInpKbCd();
		if ( iInpKbCd == MP_INPKB_1PT) {
			if ( iMov == 0) {
				pPlcEnM->SetPIPlcIti( 0, pPlcEnM->GetPIPlcIti(1));
			} else {
				pPlcEnM->SetPIPlcIti( iMov, PtInt);
				pPlcEnM->SetPILenHosei( iMov, rLH);
			}

		} else {
			pPlcEnM->SetPIPlcIti( iMov, PtInt);
			pPlcEnM->SetPILenHosei( iMov, rLH);
		}

	}
}

/////////////////////////////////////////////////////////////////////////////
//	�����悪���ނŎ����ꂽ���ނ̒�������
MINT HaitiIn::MhAdjBzL(										// (  O) �X�e�C�^�X�@
													//	MC_PARALLEL (-1) �����Ȃ��i���s�j
													//	MC_TWIST    (-2) �����Ȃ��i�˂���j
													//	MC_NINT	    (0)	�����Ȃ�
													//	MC_INT      (1)	�������蒷������
						MINT		iKati,			// (I  ) ���������t���O(1:�����A0:����)
						MhPlcInfo *pPlcEn1,			// (I O) ������������1
				const	MgPoint3	&Pt1,			// (I  ) ���������w���_�i����1�̎c�����������Ŋ�̓_�j
						MhPlcInfo *pPlcEn2			// (I  ) �������������������2
				)
{
	MINT		ist;
	MINT		ist1, ist2;
	
	MhSenBuzai	Bz1, Bz2;
	MgPoint3	PtInt;
	MREAL		s0, s1, s2;							// ���������w���_�ƕ���1�̕���2�ɑ΂��鍶�E�ʒu
	MINT		iMov;								// ����1�̃J�b�g�[ 0:�z�u�_�� 1:�z�u�����X��
	MgPoint3	PtC;
	MgULine3	ULnBz1;								// 
	MgULine3	ULnBz2;
	MgPoint3	PtI[2];
	MREAL		rLH;
	MREAL		rLnWH;

//	if ( pPlcEn1 == pPlcEn2)									// �����������ނƒ�����̕��ނ������ꍇ��
//		return;													// ���������ΏۊO

	MgVect3	VtUtZ( 0., 0., 1.);

	Bz1 = *pPlcEn1;
	Bz2 = *pPlcEn2;
	
	ist = MhSenBuzai::MhOn(  Bz1, Bz2, &PtInt, &ist1, &ist2);
	if ( ist != MC_INT && ist != MC_NINT) goto exit;			// �����Ȃ��̂��ߖ�����
	
	iMov = ist2 - 1;
	
	s0 = (Bz2.vtUt ^ (Pt1 - Bz2.LnWH.p[0])) * VtUtZ;			// s0>0: ���������w���_�͒���������̕��ނ̍���
	s1 = (Bz2.vtUt ^ (Bz1.LnWH.p[0] - Bz2.LnWH.p[0])) * VtUtZ;	// s1>0: �����������ނ̔z�u�_�͒���������̕��ނ̍���
	s2 = (Bz2.vtUt ^ (Bz1.LnWH.p[1] - Bz2.LnWH.p[0])) * VtUtZ;	// s2>0: �����������ނ̔z�u�����_�͒���������̕��ނ̍���

	if ( ist1 == MC_CLOSS) {
		if (s0 * s1 > 0) {
			iMov = 1;											// �z�u�����_���̒�����␳
		} else {
			iMov = 0;											// �z�u�_���̒�����␳
		}
	}

	// ����1�̎c�����̈ʒu�����ނQ�̍��E�ǂ���ɂ��邩���肵�A�J�b�g�����i������̏����܂��͕������̒����j�����߂�
	bool bLeft = (iMov == 1 && MGLEFT( s1) ||
				  iMov == 0 && MGLEFT( s2));					// true: ����2�̍����ɂ���
	if ( iKati == 1)
		bLeft = ! bLeft;										// �����̏ꍇ�́A���E�t�ɂ���
	if ( bLeft) {
		ULnBz2 = Bz2.ULnW[1];
	} else {
		ULnBz2 = Bz2.ULnW[0];
	}

	ULnBz1 = Bz1.ULnW[0];										// ����1��������
	ist = MGeo::Intr2ULn3( ULnBz1, ULnBz2, &PtI[0]);			// �J�b�g�����Ƃ̌�_
	if ( ist != MC_INT)
		goto exit;

	ULnBz1 = Bz1.ULnW[1];										// ����1�E������
	ist = MGeo::Intr2ULn3( ULnBz1, ULnBz2, &PtI[1]);			// �J�b�g�����Ƃ̌�_
	if ( ist != MC_INT)
		goto exit;

	if ( iMov == 0) {											// �ő�̒����␳�l�����߂�
		rLH = MGMAX( (PtI[0] - PtInt) * (- Bz1.vtUt), (PtI[1] - PtInt) * (- Bz1.vtUt));
	} else {
		rLH = MGMAX( (PtI[0] - PtInt) * (Bz1.vtUt), (PtI[1] - PtInt) * (Bz1.vtUt));
	}

	if ( MGABS( rLH) > MMAX_HOSEI_DBG)
		rLH = MMAX_HOSEI_DBG;									// �f�o�b�O�p�@�G���[�`�F�b�N���\�ɂ��邽��999���Z�b�g

	if ( Bz1.Ln.p[1 - iMov] == PtInt) {
		rLnWH = Bz1.rLH[1 - iMov] + rLH;
		if ( !MGeo::Zero( rLnWH))
			MhModBzPH( MP_INPKB_1PT, iMov, Bz1.vtUt, rLH, pPlcEn1);	// ��������
	} else {
		MhModBzPH( MP_INPKB_LEN2PT, iMov, PtInt, rLH, pPlcEn1);		// ��������
	}
exit:
	return ist;
}

/////////////////////////////////////////////////////////////////////////////
//	�����悪���s���ނŎ����ꂽ���ނ̒�������
void HaitiIn::MhAdjBzL(										// (  O) �X�e�C�^�X�@
													//	MC_PARALLEL (-1) �����Ȃ��i���s�j
													//	MC_TWIST    (-2) �����Ȃ��i�˂���j
													//	MC_NINT	    (0)	�����Ȃ�
													//	MC_INT      (1)	��������
						MhPlcInfo *pPlcEn1,			// (I O) ������������1
				const	MgPoint3	&Pt1,			// (I  ) ����1�̒����������鑤�������Ŋ�̓_
						MhPlcInfo *pPlcEn2,			// (I  ) �������������������2
				const	MgPoint3	&Pt2			// (I  ) ����2�̒���������������Ŋ�̓_
				)
{
	MhSenBuzai	Bz1, Bz2;
	MINT		iRef;

	Bz1 = *pPlcEn1;
	Bz2 = *pPlcEn2;

	if ( MGeo::Dist2Pt3( Bz2.LnWH.p[0], Pt2) < MGeo::Dist2Pt3( Bz2.LnWH.p[1], Pt2)) {
		iRef = 0;												// ������𕔍�2�̔z�u�_���Ƃ���
	} else {
		iRef = 1;												// ������𕔍�2�̔z�u�����_���Ƃ���
	}


	MREAL		rLH;
	MREAL		rLnWH;
	MINT		iMov;

	MgPoint3	PtIntWH;
	MgPoint3	PtInt;
	MREAL		s1, s2;

	// ����1�̎c�����̈ʒu�����ނQ�w��[���̑O���ǂ���ɂ��邩���肵�A�J�b�g���������߂�
	MGeo::PerpLnPt3( Bz1.Ln, Bz2.LnWH.p[iRef], &PtIntWH);
	
	s1 = (Bz1.LnWH.p[0] - PtIntWH) * Bz1.vtUt;
	s2 = (Bz1.LnWH.p[1] - PtIntWH) * Bz1.vtUt;

	if ( s1 > 0 && s2 > 0) {									// �����������ނ͒���������̕��ނ̑O���ɂ���
		if ( s1 < s2) {
			iMov = 0;											// �z�u�_���̒�����␳
		} else {
			iMov = 1;											// �z�u�����_���̒�����␳
		}
	} else if(s1 < 0 && s2 < 0) {								// �����������ނ͒���������̕��ނ̌㑤�ɂ���
		if ( s1 > s2) {
			iMov = 0;											// �z�u�_���̒�����␳
		} else {
			iMov = 1;											// �z�u�����_���̒�����␳
		}
	} else {													// �����������ނ͒���������̕��ނ̑O��ɂ܂������Ă���
		if ( MGeo::Dist2Pt3( Bz1.LnWH.p[0], Pt1) < MGeo::Dist2Pt3( Bz1.LnWH.p[1], Pt1)) {
			iMov = 0;											// �z�u�_���̒�����␳
		} else {
			iMov = 1;											// �z�u�����_���̒�����␳
		}
	}

	MGeo::PerpLnPt3( Bz1.Ln, Bz2.Ln.p[iRef], &PtInt);
	rLH = Bz2.rLH[iRef];
	if (Bz1.vtUt * Bz2.vtUt > 0) {
		if ( iMov != iRef)
			rLH = -rLH;
	} else {
		if ( iMov == iRef)
			rLH = -rLH;
	}

	if (Bz1.Ln.p[1 - iMov] == PtInt) {
		rLnWH = Bz1.rLH[1 - iMov] + rLH;
		if ( !MGeo::Zero( rLnWH))
			MhModBzPH( MP_INPKB_1PT, iMov, Bz1.vtUt, rLH, pPlcEn1);	// ��������
	} else {
		MhModBzPH( MP_INPKB_LEN2PT, iMov, PtInt, rLH, pPlcEn1);		// ��������
	}
}

/////////////////////////////////////////////////////////////////////////////
//	�����悪�_���W�Ŏ����ꂽ���ނ̒����������s��
void HaitiIn::MhAdjBzL(
						MhPlcInfo *pPlcEn1,			// ������������1
				const	MgPoint3	&Pt1,			// ����1�̒����������鑤�������Ŋ�̓_1
				const	MgPoint3	&Pt2			// ����������������_2
				)
{
	MINT		ist;
	MhSenBuzai	Bz1;
	MREAL		rLH;
	MREAL		rLnWH;
	MINT		iMov;
	MgPoint3	PtInt;
	MREAL		s1, s2;


	Bz1 = *pPlcEn1;

	MGeo::PerpLnPt3( Bz1.Ln, Pt2, &PtInt);
	
	s1 = (Bz1.Ln.p[0] - PtInt) * Bz1.vtUt;
	s2 = (Bz1.Ln.p[1] - PtInt) * Bz1.vtUt;

	if ( s1 > 0 && s2 > 0) {									// �n�_�I�_���ɔz�u�������Ȃ̂�
		iMov = 0;												// �z�u�_�̕����߂��z�u�_���̒����␳���C��
	} else if(s1 < 0 && s2 < 0) {								// �n�_�I�_���ɔz�u�_���Ȃ̂�
		iMov = 1;												// �z�u�����_�̕����߂��z�u�����_���̒����␳���C��
	} else {
		if ( MGeo::Dist2Pt3( Bz1.Ln.p[0], Pt1) < MGeo::Dist2Pt3( Bz1.Ln.p[1], Pt1)) {
			iMov = 0;											// �z�u�_���̒����␳
		} else {
			iMov = 1;											// �z�u�����_���̒����␳
		}
	}

	ist = InpAttr::GetComboAttrR( MC_CMB_LHS1, &rLH);				// �����␳�l���擾����
	if ( Bz1.Ln.p[1 - iMov] == PtInt) {
		rLnWH = Bz1.rLH[1 - iMov] + rLH;
		if ( !MGeo::Zero( rLnWH))
			MhModBzPH( MP_INPKB_1PT, iMov, Bz1.vtUt, rLH, pPlcEn1);	// ��������
	} else {
		MhModBzPH( MP_INPKB_LEN2PT, iMov, PtInt, rLH, pPlcEn1);		// ��������
	}
}

/////////////////////////////////////////////////////////////////////////////
//	���������O��ŐڐG��Ԃƌ�����Ԃ��ω����镔�ނ����߂�
void HaitiIn::MhChngCrossBuzai(
						MhPlcInfo	**pbTchBziI,	// (I  ) ���������O�ɒ����������ނɐڐG���镔��
						MhPlcInfo	**pbCrsBziI,	// (I  ) ���������O�ɒ����������ނƌ������镔��
						MhPlcInfo	**pbTchBziO,	// (I  ) ����������ɒ����������ނɐڐG���镔��
						MhPlcInfo	**pbCrsBziO,	// (I  ) ����������ɒ����������ނƌ������镔��
						MhPlcInfo	**pbFRtoTCBzi,	// (  O) ���������O�ɒ����������ނƗ���Ă�����
													//		 ����������ɐڐG�܂��͌�����������
						MhPlcInfo	**pbTCtoFRBzi,	// (  O) ���������O�ɒ����������ނƐڐG�܂��͌������Ă�����
													//		 ����������ɗ��ꂽ����
						MhPlcInfo	**pbTCtoTCBzi	// (  O) ���������O�ɒ����������ނɐڐG�܂��͌������Ă�����
													//		 �A����������ɂ��ڐG�܂��͌������Ă��镔��
				)
{
	MINT	ic0, ic1, ic2;
	MhPlcInfo* TCBziI[MSZ_CROSS_BZI];
	MhPlcInfo* TCBziO[MSZ_CROSS_BZI];
	MINT	if0;
//
	ic0 = 0;
	for ( ic1=0; pbTchBziI[ic1]!=NULL; ic1++)
		TCBziI[ic0++] = pbTchBziI[ic1];
	for ( ic1=0; pbCrsBziI[ic1]!=NULL; ic1++)
		TCBziI[ic0++] = pbCrsBziI[ic1];
	TCBziI[ic0] = NULL;
//
	ic0 = 0;
	for ( ic1=0; pbTchBziO[ic1]!=NULL; ic1++)
		TCBziO[ic0++] = pbTchBziO[ic1];
	for ( ic1=0; pbCrsBziO[ic1]!=NULL; ic1++)
		TCBziO[ic0++] = pbCrsBziO[ic1];
	TCBziO[ic0] = NULL;
//
	ic0 = 0;
	for ( ic1=0; TCBziO[ic1]!=NULL; ic1++) {
		if0 = 0;
		for ( ic2=0; TCBziI[ic2]!=NULL; ic2++) {
			if (TCBziI[ic2] == TCBziO[ic1]) {
				if0 = 1;
				break;
			}
		}
		if ( if0 == 1)
			continue;
		pbFRtoTCBzi[ic0++] = TCBziO[ic1];
	}
	pbFRtoTCBzi[ic0] = NULL;
//
	ic0 = 0;
	for ( ic1=0; TCBziI[ic1]!=NULL; ic1++) {
		if0 = 0;
		for ( ic2=0; TCBziO[ic2]!=NULL; ic2++) {
			if (TCBziO[ic2] == TCBziI[ic1]) {
				if0 = 1;
				break;
			}
		}
		if ( if0 == 1)
			continue;
		pbTCtoFRBzi[ic0++] = TCBziI[ic1];
	}
	pbTCtoFRBzi[ic0] = NULL;
//
	ic0 = 0;
	for ( ic1=0; TCBziO[ic1]!=NULL; ic1++) {
		for ( ic2=0; TCBziI[ic2]!=NULL; ic2++) {
			if (TCBziI[ic2] == TCBziO[ic1]) {
				pbTCtoTCBzi[ic0++] = TCBziO[ic1];
				break;
			}
		}
		if (TCBziO[ic1] == TCBziI[ic2])
			continue;
	}
	pbTCtoTCBzi[ic0] = NULL;
}

} // namespace MC
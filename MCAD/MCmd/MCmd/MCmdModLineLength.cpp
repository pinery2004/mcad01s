//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: .cpp
//
//		
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "McAPI.h"
#include "MhPts1.h"

namespace MC
{

/////////////////////////////////////////////////////////////////////////////
//	���u���ނ̒�������
	void MCmdMdLineLength()
{
	MINT		irt;
	MINT		ist1;
	MINT		ic1;
	MgPoint2	pt1, pt2, pt2m;
	MgPoint3	Pt1, Pt2, Pt2m;
	MgLine2		ln1;
	MgLine3		Ln1;
	MgPolyg2	Pg1(20);
//	MINT		iIdTpPts;
//	MhTpPts*	pTpPts;
	bool		bFirst = TRUE;
	MINT		iKati;
//	MINT		iMulti;
	MINT		iCrsC;

	MINT		iBuzaiCd;
	MgPolyg2	pgHitBzi;
	MhPlcInfo* pBziInfo1;
	MhPlcInfo* pBziInfo2;
	MhPlcInfo* pbTchBziI[MSZ_CROSS_BZI];
	MhPlcInfo* pbCrsBziI[MSZ_CROSS_BZI];
	MhPlcInfo* pbTchBziO[MSZ_CROSS_BZI];
	MhPlcInfo* pbCrsBziO[MSZ_CROSS_BZI];

	MhPlcInfo* pbFRtoTCBzi[MSZ_CROSS_BZI];
	MhPlcInfo* pbTCtoFRBzi[MSZ_CROSS_BZI];
	MhPlcInfo* pbTCtoTCBzi[MSZ_CROSS_BZI];

	Msg::ClearErrorMsg();
	Msg::OperationMsg( MC_OPRT_MOD_BUZAI);							// �X�e�C�^�X�o�[�̑���\������"���ޏC��"��\��

	InpAttr::InitComboAttr( MP_AT_ADJLNG);							// �������͗p�R���{�{�b�N�X�𕔍ޒ��������p�ɂ���

	MmWndInfo* pWndInfo = WindowCtrl::MmWndKGetCurWnd();			// �J�����g�E�B���h�E���擾����
	Window::CurWndFocus();											// �t�H�[�J�X���J�����g�E�B���h�E�ɐݒ肷��

	MFOREVER {
		Window::SetDispSelPts( NULL);

		Msg::GuidanceMsg( MC_GUID_MOD_BUZAILENGTH1);				// "���ނ̒����������̒[�����w�����ĉ�����"
		irt = MtAskForPoint( &pt1);

		if ( irt == MTRT_SYSTEMSTOP || irt == MTRT_CAN)
			break;

		iBuzaiCd = NULL;
		if ( pBziInfo1 = HaitiIn::SrchBuzai( pWndInfo, pt1, iBuzaiCd, &pgHitBzi)) {
			Window::SetCurBziFig( &pgHitBzi);						// �J�����g�I��(����������)�̕���(����������)�`���ݒ肷��
			WindowCtrl::MmWndKReDraw();
		} else {
			Window::ClrCurBziFig();									// �J�����g�I���̕��ތ`����N���A����
			continue;
		}

		Window::SetDispSelPts( NULL);

		Msg::GuidanceMsg( MC_GUID_MOD_BUZAILENGTH2);				// "���ނ̒�����������w�����ĉ�����"
		irt = MtAskForPoint( &pt2);

		if ( irt == MTRT_SYSTEMSTOP || irt == MTRT_CAN)
			break;

		iBuzaiCd = NULL;
		pBziInfo2 = HaitiIn::SrchBuzai( pWndInfo, pt2, iBuzaiCd, &pgHitBzi);
		if ( pBziInfo1 == pBziInfo2)								// �������������ނƒ��������敔�ނƓ��ꕔ�ނ�I���̏ꍇ�͍��W�w���Ƃ݂Ȃ�
			pBziInfo2 = NULL;

		Pt1 = MgPoint3C( pt1);

		Window::DrawHitBzi( pWndInfo, &pgHitBzi);					//	�q�b�g���ށi���������敔�ށj��\������
																	//	�i���\���q�b�g���ނ�������ɁA�q�b�g���ނ�\������j
		ist1 = InpAttr::GetCheckAttr( MC_CHK_KATI, &iKati);			// ���������`�F�b�N�{�b�N�X�̏����}�[�N���擾����(1:����,0:����)
		ist1 = InpAttr::GetCheckAttr( MC_CHK_INTC, &iCrsC);			// �������ޒ����`�F�b�N�{�b�N�X�̒����}�[�N���擾����(1:����,0:�Ȃ�)

		HaitiIn::MmSrchCrossBuzai( pWndInfo, pBziInfo1, iBuzaiCd, pbTchBziI, pbCrsBziI);
																	// �������������ނƒ����O�ɐG��Ă��镔�ތQ�ƌ������Ă��镔�ތQ�����߂�
		if ( pBziInfo2) {
			ist1 = HaitiIn::MhAdjBzL( iKati, pBziInfo1, Pt1, pBziInfo2);	// ��������
		} else {
//S			pWndInfo->Marume( pt2, &pt2m);
			Input::Marume( pt2, &pt2m);
			Pt2m = MgPoint3C( pt2m);
			HaitiIn::MhAdjBzL( pBziInfo1, Pt1, Pt2m);
		}

		if ( ist1 == MC_PARALLEL) {
			Pt2 = MgPoint3C( pt2);
			HaitiIn::MhAdjBzL( pBziInfo1, Pt1, pBziInfo2, Pt2);		// ���������ނƒ����敔�ނ����s���̒���
		}
			
		HaitiIn::MmSrchCrossBuzai( pWndInfo, pBziInfo1, iBuzaiCd, pbTchBziO, pbCrsBziO);
																	// �������������ނƒ�����ɐG��Ă��镔�ތQ�ƌ������Ă��镔�ތQ�����߂�
			
		HaitiIn::MhChngCrossBuzai( pbTchBziI, pbCrsBziI, pbTchBziO, pbCrsBziO, 
								   pbFRtoTCBzi, pbTCtoFRBzi, pbTCtoTCBzi);
		if ( iCrsC == 1) {
			MgLine3 	LnBzi;
			MgPoint3	Pt0;
			// ����Ă��� �� �ڐG�@�܂��́@����
				// ���������@�܂��́@����
			for ( ic1=0; pbFRtoTCBzi[ic1]; ic1++) {
				LnBzi = pbFRtoTCBzi[ic1]->GetPIPlcIti();
				Pt0 = (LnBzi.p[0] + LnBzi.p[1]) / 2.;
				HaitiIn::MhAdjBzL( 0, pbFRtoTCBzi[ic1], Pt0, pBziInfo1);
			}
			// �ڐG�@�܂��́@���� �� ����Ă���
/*
			for ( ic1=0; pbTCtoFRBzi[ic1]; ic1++) {
				// �A���@�܂��́@����
				LnBzi = pbTCtoFRBzi[ic1]->GetPIPlcIti();
				Pt0 = (LnBzi.p[0] + LnBzi.p[1]) / 2.;
				MhAdjBzL( 0, pbTCtoFRBzi[ic1], Pt0, pBziInfo1);
			}
*/
			// �ڐG�@�܂��́@���� �� �ڐG�@�܂��́@����
				// ���������@�܂��́@����
			for ( ic1=0; pbTCtoTCBzi[ic1]; ic1++) {
				LnBzi = pbTCtoTCBzi[ic1]->GetPIPlcIti();
				Pt0 = (LnBzi.p[0] + LnBzi.p[1]) / 2.;
				HaitiIn::MhAdjBzL( 0, pbTCtoTCBzi[ic1], Pt0, pBziInfo1);
			}
		}
		Window::ClrCurBziFig();
		Window::EraseHitBzi();
		WindowCtrl::MmWndKReDraw();
		Msg::ClearErrorMsg();
	}
	Window::SetDispSelPts( -1);
	Msg::OperationMsg( MC_OPRT_BASE);								// �X�e�C�^�X�o�[�̑���\������""��\��
}

} // namespace MC
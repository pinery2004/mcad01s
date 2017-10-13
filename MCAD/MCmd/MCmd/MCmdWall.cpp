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
#include "MhInp.h"

#define		MAXHAIKABE		500									// �z�񐧌��l

namespace MC
{

void MCmdLine();
void MCmdLineW();
void SetInpAt();

/////////////////////////////////////////////////////////////////////////////
//	�O�Ǔ���
void MCmdWallOuter()
{
	MINT	ist1;
//	MhInitInpAt();
	ist1 = InpAttr::SetDialogBar( MP_GP_TAIRYOKU, MP_BR_OTHER, Mstr( "�O��"), Mstr( "204C"));
	if ( ist1 == 0)
		MCmdLine();
}

/////////////////////////////////////////////////////////////////////////////
//	���Ǔ���
void MCmdWallInner()
{
	MINT	ist1;
//	MhInitInpAt();
	ist1 = InpAttr::SetDialogBar( MP_GP_TAIRYOKU, MP_BR_OTHER, Mstr( "����"), Mstr( "204C"));
	if ( ist1 == 0)
		MCmdLine();
}

/////////////////////////////////////////////////////////////////////////////
//	�O������i�T�b�V�j����
void MCmdGaiTateSash()
{
	MINT	ist1;
//	MhInitInpAt();
//	SetInpAt();
	ist1 = InpAttr::SetDialogBar( MP_GP_KABE, MP_BR_OTHER, Mstr( "�T�b�V"), Mstr( "204C"));
	if ( ist1 == 0)
		MCmdLineW();
}

/////////////////////////////////////////////////////////////////////////////
//	�O������i�h�A�j����
void MCmdGaiTateDoor()
{
	MINT	ist1;
//	MhInitInpAt();
//	SetInpAt();
	ist1 = InpAttr::SetDialogBar( MP_GP_KABE, MP_BR_OTHER, Mstr( "�h�A"), Mstr( "204C"));
	if ( ist1 == 0)
		MCmdLineW();
}

/////////////////////////////////////////////////////////////////////////////
//	�����������
void MCmdNaiTate()
{
	MINT	ist1;
//	MhInitInpAt();
//	SetInpAt();
	ist1 = InpAttr::SetDialogBar( MP_GP_KABE, MP_BR_OTHER, Mstr( "��������"), Mstr( "204C"));
	if ( ist1 == 0)
		MCmdLineW();
}

/////////////////////////////////////////////////////////////////////////////
//	�������
void MCmdLineW()
{
	MINT		irt;
	MgPoint2	pt1, pt2;
	MgPoint2	ptln1[3], ptln1_org[3];
	MgLine3		Ln1;
	MgPolyg2	pg1(20);
	MINT		iIdTpPts;
	MhTpPts*	pTpPts;

	MhPlcInfo* pHaiKabe[MAXHAIKABE];				// �ǃf�[�^
	MINT		nHaiKabe;							// �ǐ�
	MINT		ic2;
	MINT		iMode;

	Window::CurWndFocus();

	Msg::ClearErrorMsg();
	Msg::OperationMsg( MC_OPRT_PARTS);								// �X�e�C�^�X�o�[�̑���\������"���ޒǉ�"��\��

	InpAttr::InitComboAttr( MP_AT_AUTO);								// ������͗p�̑������̓R���{�{�b�N�X��\��

	IeModel::MhInitInpAt();				// 070901
	SetInpAt();					// 070901

	iIdTpPts = InpAttr::GetCurIdTpPts();
	pTpPts = BuzaiCode::MhGetpTpPts( iIdTpPts);
	InpMode::SetComboInpKb( pTpPts->GetPTInpKb());
	InpMode::SetComboCdMarume( pTpPts->GetPTCdMarume());

	MFOREVER {
		iMode = 0;
		irt = Input::GetLen2Pt( iMode, ptln1, ptln1_org);
		Ln1 = MgLine3C( MgLine2( ptln1));

		if ( irt == MTRT_SYSTEMSTOP || irt == MTRT_CAN)
			break;

		MINT iKai = InpMode::GetKai();
		nHaiKabe = HaitiIn::GetPts( iKai, MP_GP_TAIRYOKU, Mstr( "��"),
							   NULL, MAXHAIKABE, pHaiKabe);
																					// �ƃ��f�����J�����g�K�̑S�Ă̕ǂ��擾����
		for ( ic2=0; ic2<nHaiKabe; ic2++) {											// ��
			if ( MGeo::ChkLn3OnLn3( Ln1, pHaiKabe[ic2]->GetPIPlcIti()))
				break;
		}
		if ( ic2 == nHaiKabe) {
			Msg::ErrorMsg( Mstr( "�Ǐ�ɓ��͂��ĉ�����"), MC_ERR_BASE);				// ""
			continue;
		}

		MINT IdMbr = pHaiKabe[ic2]->GetPIIdMbr();
		MhPlcInp::SetIdMbr( IdMbr);
		MhPlcInp::SetpMbr( BuzaiCode::MhGetpMbr( IdMbr));
		HaitiCmd::MmPresetCmd();
		InpAttr::GetComboAttrA();

//DDD		MhPlcInp::SetpAux_xxxxxx();

		MhTateguInfo* pAuxAtr = (MhTateguInfo*)new char[sizeof( MhTateguInfo)];
		*pAuxAtr = *MhTateguInp::GetpTateguInp(); 
		MhPlcInp::SetpAuxTategu( pAuxAtr);

//		g_hInpPIAttr.GetPIAuxAttr();

		HaitiCmd::MmPtsPlc( Ln1.p);

		WindowCtrl::MmWndKReDraw();
		Msg::ClearErrorMsg();
	}
	Msg::OperationMsg( MC_OPRT_BASE);								// �X�e�C�^�X�o�[�̑���\������""��\��
}


/////////////////////////////////////////////////////////////////////////////
//	������͗p���������l�ݒ�
void SetInpAt()
{
	MhTateguInp::SetKCode( Mstr( ""));
	MhTateguInp::SetROW( 910.f);
	MhTateguInp::SetROH( 1000.f);
	MhTateguInp::SetHeight( 2100.f);
	MhTateguInp::SetMU( 38.f, 0);
	MhTateguInp::SetMU( 38.f, 1);
//
	static MINT ifirst = 0;
	MhPlcInp::SetLenHosei( 0, -82.5f);
	MhPlcInp::SetLenHosei( 1, -82.5f);
//
	InpAttr::SetComboAttrR( MC_CMB_LHS1, -82.5f);
	InpAttr::SetComboAttrR( MC_CMB_LHS2, -82.5f);
	if ( ifirst == 1) {
		InpAttr::SetComboAttrR( MC_CMB_KROH, 1000.0f);
	} else {
		InpAttr::SetComboAttrR( MC_CMB_KROH, 2095.0f);
		ifirst = 1;
	}
	InpAttr::SetComboAttrR( MC_CMB_ZJSZ, 0.0f);
	InpAttr::SetComboAttrR( MC_CMB_HAIZ, 0.0f);
	InpAttr::SetComboAttrR( MC_CMB_TRTH, 2095.0f);
}

void MCmdDebug()
{
	MhPlcInfo*	pPlcEn1;
	MPOSITION		posH;
	MhRoofInfo*		pRoofEn1;
	MPOSITION		posR;


	for ( pPlcEn1 = HaitiDb::MdGetHeadPts( &posH); pPlcEn1!=0;
		  pPlcEn1 = HaitiDb::MdGetNextPts( &posH)) {
		pPlcEn1->FreeAllAtr();
	}

	for ( pRoofEn1 = HaitiDb::MdGetHeadRoof( &posR); pRoofEn1!=0;
		  pRoofEn1 = HaitiDb::MdGetNextRoof( &posR)) {
	}
}

} // namespace MC
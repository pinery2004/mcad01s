//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MmDialogBarAttr.cpp
//
//		
//	�������̓_�C�A���O�o�[
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
//#define	 WINVER	0x0400
#include <afxwin.h>         // MFC �̃R�A����ѕW���R���|�[�l���g

#include "MCAD.h"
#include "MainFrm.h"
#include "MmDefine.h"

#define	DLL_EXPORT_MC_INPATTR_DO
#include "MmLib.h"
#include "MhInp.h"
#include "resource.h"
#include "..\\MCAD\\resource.h"
#include "MhPlcInfo.h"
#include "MhLib.h"
#include "MgGeo.h"

#define MC_SZ_CMBATTR	6
#define MC_SZ_CHKATTR	4

namespace MC
{

static MINT z_IdcComboAttr[6] =		{ IDC_CMB_ATTR1, IDC_CMB_ATTR2, IDC_CMB_ATTR3,
									  IDC_CMB_ATTR4, IDC_CMB_ATTR5, IDC_CMB_ATTR6};
static MINT z_IdcCheckAttr[4] =		{ IDC_CHECKATTR1, IDC_CHECKATTR2, IDC_CHECKATTR3, IDC_CHECKATTR4};
static MINT z_IdcStaticAttr[6] =	{ IDC_STATICATTR1, IDC_STATICATTR2, IDC_STATICATTR3,
									  IDC_STATICATTR4, IDC_STATICATTR5, IDC_STATICATTR6};

static MINT z_Combo_Attr[MC_SZ_CMBATTR];
static MINT z_Check_Attr[MC_SZ_CHKATTR];

//===========================================================================
//				�������p�X�^�e�B�b�N�f�[�^
//
//===========================================================================

static void MmsInitComboAttr( MCCMBATTR, MCCMBATTR, MCCMBATTR, MCCMBATTR, MCCMBATTR,
							  MCCMBATTR, MCCHKATTR, MCCHKATTR, MCCHKATTR, MCCHKATTR);

// ======== ���ޗp ========

// �����␳�l
static MREAL z_rLngH[] = {  95.5,  82.5,  70.0,  57.5,   44.5,  38.0, 
							 32.0,   6.5,   0.0, -18.5,  -19.0, -31.5,
							-38.0, -44.0, -44.5, -57.0,  -69.5, -70.0,
							-82.5, -95.0, -95.5, -114.0};
#define	ISZLNGH		sizeof(z_rLngH)/sizeof(MREAL)
#define	INITLNGTH	0.0f

// �ގ��c�����
static MREAL z_rSinZ[] = {  76.5,   76.0,  63.5,  57.0,  53.5,  51.0,
							 38.5,  38.0,  25.5,  19.0,  13.0,  12.5,
							  0.5,   0.0, -12.5,  -19.0, -25.0, -25.5,
							-38.0, -50.5, -53.5, -57.0, -63.5, -95.0};
#define	ISZSINZ		sizeof(z_rSinZ)/sizeof(MREAL)
#define	INITSINZ	0.0f

// �z�u�_�����
static MREAL z_rHaiZ[] = { 152.5, 127.0,  114.5,  101.5,  95.5,  89.0,
							 76.5,  70.0,   63.5,   57.0,  51.0,  44.5,
							 38.5,  25.5,   19.0,   13.0,   0.5,   0.0,
							-12.5, -19.0,  -25.0,  -50.5, -57.0, -95.0};
#define	ISZHAIZ	sizeof(z_rHaiZ)/sizeof(MREAL)
#define	INITHAIZ	0.0f

// ���t������
static MREAL z_rZ1[] = { 2250.0, 2150.0, 2095.0, 2067.0, 2057.0, 2055.0,
						  2050.0, 2010.0, 1950.0, 1850.0, 1810.0, 1750.0,
						  1650.0, 1550.0, 1450.0, 1350.0, 1250.0, 1150.0,
						  1050.0, 950.0, 850.0, 600.0, 0.0};
#define	ISZZ1		sizeof(z_rZ1)/sizeof(MREAL)
#define	INITZ1		0.0f

// �q�n�g
static MREAL z_rZ2[] = { 2250.0, 2150.0, 2095.0, 2067.0, 2055.0, 2050.0,
						  2010.0, 1950.0, 1850.0, 1550.0, 1545.0, 1450.0,
						  1400.0, 1350.0, 1245.0, 1000.0,  900.0,  750.0,
						   600.0, 0.0};
#define	ISZZ2		sizeof(z_rZ2)/sizeof(MREAL)
#define	INITZ2		0.0f

// �Ԋu
static MREAL z_rIntv[] = { 1820.0, 1368.0, 910.0, 455.0, 303.0, 227.5};
#define	ISZINTV		sizeof(z_rIntv)/sizeof(MREAL)
#define	INITINTV	455.0f

// �{��
static MINT	  z_iNum[] = { 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 
						   10,  9,  8,  7,  6,  5,  4,  3,  2,  1, 9998};
#define	ISZNUM		sizeof(z_iNum)/sizeof(MINT)
#define	INITNUM		1

// ======== �p�l���p ========

// �p�l�����␳�l
static MREAL z_rLngH2[] = { 9999.0,  95.5,  70.0,  53.0,  44.5,  0.0,
							  -44.5, -53.0, -70.0, -95.5};
#define	ISZLNGH2	sizeof(z_rLngH2)/sizeof(MREAL)
#define	INITLNGTH2	0.0f

// ��O���␳�l�A���s���␳�l
static MREAL z_rOkuH[] = { 9999.0,	 95.5,	70.0,  53.0,  44.5,  0.0,
								 -44.5, -53.0, -70.0, -95.5};
#define	ISZOKUYUKIH	sizeof(z_rOkuH)/sizeof(MREAL)
#define	INITOKUYUKIH	0.0f

// ���s��
static MREAL z_rOku[] = {	7280.0, 6825.0, 6370.0, 5915.0, 5460.0, 5005.0, 
								4550.0, 4095.0, 3640.0, 3185.0, 2730.0, 2275.0,
								1820.0, 1365.0,  910.0,  455.0, 9999.0};
#define	ISZOKUYUKI	sizeof(z_rOku)/sizeof(MREAL)
#define	INITOKUYUKI		3640.0f

// �������z
static MINT z_iKoubai[] = {	12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
#define	ISZKOUBAI	sizeof(z_iKoubai)/sizeof(MINT)
#define	INITKOUBAI		4

// ���̏o
static MREAL z_rNokinoDe[] = {	650.0, 455.0, 330.0, 44.5, 0.};
#define	ISZNOKINODE	sizeof(z_rNokinoDe)/sizeof(MREAL)
#define	INITNOKINODE		650.0f

// �P���o�̏o
static MREAL z_rKerabanoDe[] = { 650.0, 455.0, 330.0, 44.5, 0.};
#define	ISZKERABANODE	sizeof(z_rKerabanoDe)/sizeof(MREAL)
#define	INITKERABANODE	650.0f

//////////////////////////////////////////////////////////////////////////////
//	�����l���͗p�R���{�{�b�N�X�ԍ����擾����

static MINT GetComboAttrNo(
						MCCMBATTR	iAttr			// (I  ) ����ID
				)
{
	MINT	ic;
	MINT	iCmbNo = 0;

	for ( ic=0; ic<MC_SZ_CMBATTR; ic++)
		if ( iAttr == z_Combo_Attr[ic])
			break;
	if ( ic < MC_SZ_CMBATTR)
		iCmbNo = ic + 1;
	return iCmbNo;
}

///////////////////////////////////////////////////////////////////////////////
//	�����l���͗p�R���{�{�b�N�X�̃^�C�g����ݒ肷��
static void SetComboAttrText(
						MINT	iCmbNo,				// (I  ) �R���{�{�b�N�X�ԍ�
						MCHAR*	sTitle				// (I  ) �^�C�g��
				)
{
	((CComboBox*)( System::GetpMainFrame()->m_wndDlgBar5.GetDlgItem(z_IdcStaticAttr[iCmbNo - 1])))->SetWindowText( sTitle);
}

///////////////////////////////////////////////////////////////////////////////
//	�����l���͗p�R���{�{�b�N�X�Ɏ����l��\������

static void SetComboAttrRCbn(
						MINT	iAttr,				// (I  ) �R���{�{�b�N�X�ԍ�
						MREAL	rValue				// (I  ) �\����������l
				)
{
	CString	strValue;
	strValue.Format( Mstr( "%7.1f"), rValue);

	((CComboBox*)(System::GetpMainFrame()->m_wndDlgBar5.GetDlgItem(z_IdcComboAttr[iAttr - 1])))->SetWindowText(strValue);
}

//////////////////////////////////////////////////////////////////////////////
//	�����l���͗p�R���{�{�b�N�X�Ɏ����l��\������

void InpAttr::SetComboAttrR(
						MCCMBATTR	iAttr,			// (I  ) ����ID
						MREAL		rValue			// (I  ) �\����������l
				)
{
	MINT iCmbNo = GetComboAttrNo( iAttr);
	if ( iCmbNo != 0)
		SetComboAttrRCbn( iCmbNo, rValue);
}

///////////////////////////////////////////////////////////////////////////////
//	�����l���͗p�R���|�{�b�N�X�Ɏ����l�̍���(�ꗗ)��ݒ肷��

static void InitComboAttrR(
						MINT	iAttr,				// (I  ) �R���{�{�b�N�X�ԍ�
						MINT	nComboAttr,			// (I  ) �I�𑮐��l�̐�
						MREAL	*rComboAttr,		// (I  ) �I�𑮐��l
						MREAL	rInitValue			// (I  ) �����\�����鑮���l
				)
{
	MINT		ic1;
	MINT		ip = 0;
	CString		sKomoku;

	CComboBox *pCmb;

	pCmb = (CComboBox*)(System::GetpMainFrame()->m_wndDlgBar5.GetDlgItem(z_IdcComboAttr[iAttr - 1]));

	pCmb->ResetContent();

	// �I�𑮐��l�ݒ�
	for ( ic1=0; ic1<nComboAttr; ic1++) {
		ASSERT( ip < MX_CMB_ATTR - 1);						// �����l�I���R���{�{�b�N�X���ځ@�I�[�o�t���[�@<ERROR>
//		if ( MgEqual( rComboAttr[ic1], 9999.0))
//			sKomoku = "����";
		if ( MGeo::Equal( rComboAttr[ic1], MREAL( MC_INT_JIDO)))
			sKomoku = MC_STR_JIDO;
		else
			sKomoku.Format(Mstr( "  %6.1f"), rComboAttr[ic1]);
		pCmb->InsertString( -1, sKomoku);
	}

	if ( nComboAttr == 0) {
		// pCmb->InsertString( -1, Mstr( "none"));
		pCmb->ShowWindow( 0);
	} else {
		pCmb->ShowWindow( 1);
		SetComboAttrRCbn( iAttr, rInitValue);
	}
}

///////////////////////////////////////////////////////////////////////////////
//	�����l���͗p�R���{�{�b�N�X�̎����l���擾����
//
static MREAL GetComboAttrRCbn(						// (  O) �����l�@�܂��́@0(���ݒ�)
						MINT	iCmbNo				// (I  ) �R���{�{�b�N�X�ԍ�
				)
{
	CString strValue;

	((CComboBox*)(System::GetpMainFrame()->m_wndDlgBar5.GetDlgItem(z_IdcComboAttr[iCmbNo - 1])))->GetWindowText(strValue);

	return MsGetMREAL( strValue);
}

///////////////////////////////////////////////////////////////////////////////
//	�����l���͗p�R���{�{�b�N�X�̎����l���擾����

MINT InpAttr::GetComboAttrR(								// (  O) �X�e�C�^�X0(���ݒ�) 1(�ݒ�)
						MCCMBATTR	iAttr,			// (I  ) ����ID
						MREAL		*rValue			// (  O) �����l
				)
{
	MINT	ist;
	MINT	iCmbNo;

	iCmbNo = GetComboAttrNo( iAttr);

	if ( iCmbNo != 0) {
		*rValue = GetComboAttrRCbn( iCmbNo);
		ist = 1;
	} else {
		*rValue = 0.;
		ist = 0;
	}
	return ist;
}

///////////////////////////////////////////////////////////////////////////////
//	�����l���͗p�R���{�{�b�N�X�ɐ����l��\������

static void SetComboAttrICbn(
						MINT	iCmbNo,				// (I  ) �R���{�{�b�N�X�ԍ�
						MINT	iValue 				// (I  ) �\�����鐮���l
				)
{
	CString	strValue;
	strValue.Format( Mstr( "%7d"), iValue);

	((CComboBox*)(System::GetpMainFrame()->m_wndDlgBar5.GetDlgItem(z_IdcComboAttr[iCmbNo - 1])))->SetWindowText(strValue);
}

///////////////////////////////////////////////////////////////////////////////
//	�����l���͗p�R���{�{�b�N�X�ɐ����l��\������

void InpAttr::SetComboAttrI(
						MCCMBATTR	iAttr,			// (I  ) ����ID
						MINT		iValue 			// (I  ) �\�����鐮���l
				)
{
	MINT iCmbNo = GetComboAttrNo( iAttr);
	if ( iCmbNo != 0)
		SetComboAttrICbn( iCmbNo, iValue);
}

///////////////////////////////////////////////////////////////////////////////
//	�����l���͗p�w��R���|�{�b�N�X�ɐ����l�̍���(�ꗗ)��ݒ肷��

static void InitComboAttrI(
						MINT	iCmbNo,				// (I  ) �R���{�{�b�N�X�ԍ�
						MINT	nComboAttr,			// (I  ) �I�𑮐��l�̐�
						MINT	*iCmbAttr,			// (I  ) �I�𑮐��l
						MINT	iInitValue			// (I  ) �����\�����鑮���l
				)
{
	MINT		ic1;
	MINT		ip = 0;
	CString		sKomoku;

	CComboBox *pCmb;

	pCmb = (CComboBox*)(System::GetpMainFrame()->m_wndDlgBar5.GetDlgItem(z_IdcComboAttr[iCmbNo - 1]));

	pCmb->ResetContent();

	for ( ic1=0; ic1<nComboAttr; ic1++) {
		ASSERT( ip < MX_CMB_ATTR - 1);							// �J���������I���R���{�{�b�N�X���ځ@�I�[�o�t���[�@<ERROR>
		if ( iCmbAttr[ic1] == MC_INT_JIDO)
			sKomoku = MC_STR_JIDO;								// ����
		if ( iCmbAttr[ic1] == MC_INT_AREA)
			sKomoku = MC_STR_AREA;								// �̈�
		else
			sKomoku.Format(Mstr( "  %6d"), iCmbAttr[ic1]);
		pCmb->InsertString( -1, sKomoku);
	}

	if ( nComboAttr == 0) {
		// pCmb->InsertString( -1, Mstr( "none"));
		pCmb->ShowWindow( 0);
	} else {
		pCmb->ShowWindow( 1);
//		MhSetG_InpTateguROH( iZ2);
		SetComboAttrICbn( iCmbNo, iInitValue);
	}
}

///////////////////////////////////////////////////////////////////////////////
//	�����l���͗p�R���{�{�b�N�X�̐����l���擾����
//
static MINT GetComboAttrICbn(						// (  O) �����l�@�܂��́@0(���ݒ�)
						MINT	iCmbNo				// (I  ) �R���{�{�b�N�X�ԍ�
					)
{
	CString strValue;
	MINT	iNum;

	((CComboBox*)(System::GetpMainFrame()->m_wndDlgBar5.GetDlgItem(z_IdcComboAttr[iCmbNo - 1])))->GetWindowText(strValue);

	if ( Mstrcmp( strValue, MC_STR_JIDO) == 0)					// ����
		iNum = MC_INT_JIDO;										// 9999
	else if ( Mstrcmp( strValue, MC_STR_AREA) == 0)				// �̈�
		iNum = MC_INT_AREA;										// 9998
	else
		iNum =  MsGetMINT( strValue);
		
	return iNum;
}

///////////////////////////////////////////////////////////////////////////////
//	�����l���͗p�R���{�{�b�N�X�̐����l���擾����

MINT InpAttr::GetComboAttrI(								// (  O) �X�e�C�^�X0(���ݒ�) 1(�ݒ�)
						MCCMBATTR	iAttr,			// (I  ) ����ID
						MINT		*iValue 		// (  O) �\�����鐮���l
					)
{
	MINT	ist;
	MINT	iCmbNo;

	ist = 0;
	iCmbNo = GetComboAttrNo( iAttr);

	if ( iCmbNo != 0) {
		*iValue = GetComboAttrICbn( iCmbNo);
		ist = 1;
	} else {
		*iValue = 0;
		ist = 0;
	}
	return ist;
}

///////////////////////////////////////////////////////////////////////////////
//	�����l���͗p�R���{�{�b�N�X�̒l����荞��

void InpAttr::GetComboAttrA( void)
{
	MINT	ist;

	MREAL	rComboAttr1;
	MREAL	rComboAttr2;
	MREAL	rComboAttr3;
	MREAL	rComboAttr4;
	MREAL	rComboAttr5;
	MREAL	rComboAttr6;

	MINT	iMode = InpMode::GetAtMd();						// �����l���̓��[�h

	switch ( iMode)
	{
	case MP_AT_YTPANEL:
	case MP_AT_YANEPANEL:

		if ( iMode == MP_AT_YTPANEL) {							// ***** ���E�V��p�l�� *****
			InpAttr::GetComboAttrR( MC_CMB_LFH1, &rComboAttr1);		// �@���Е␳�l�@���@�����␳�l1
			InpAttr::GetComboAttrR( MC_CMB_RTH1, &rComboAttr2);		// �@�E�Е␳�l�@���@�����␳�l2
			InpAttr::GetComboAttrR( MC_CMB_FHS1, &rComboAttr3);		// �@��O���␳�l�@���@��O���␳�l

		} else if ( iMode == MP_AT_YANEPANEL) {					// ***** �����p�l�� *****
			InpAttr::GetComboAttrR( MC_CMB_LFH2, &rComboAttr1);		// �@���Е␳�l�@���@�����␳�l1
			InpAttr::GetComboAttrR( MC_CMB_RTH2, &rComboAttr2);		// �@�E�Е␳�l�@���@�����␳�l2
			InpAttr::GetComboAttrR( MC_CMB_FHS2, &rComboAttr3);		// �@���̏o�@���@��O���␳�l
		}

		MhPlcInp::SetLenHosei( 0, rComboAttr1);					// ���@�����␳�l1
		MhPlcInp::SetLenHosei( 1, rComboAttr2);					// ���@�����␳�l2
		MhPlcInp::SetMaeHosei( rComboAttr3);						// ���@��O���␳�l

		InpAttr::GetComboAttrR( MC_CMB_OKYK, &rComboAttr4);
		MhPlcInp::SetOku( rComboAttr4);							// ���s���@���@���s��

		InpAttr::GetComboAttrR( MC_CMB_OHS2, &rComboAttr5);
		MhPlcInp::SetOkuHosei( rComboAttr5);						// ���s���␳�l�@���@���s���␳�l�@

		InpAttr::GetComboAttrR( MC_CMB_TRTH, &rComboAttr5);
		MhPlcInp::SetHgt( 0, rComboAttr5);						// ���t�������@���@���t������ 
		break;

	case MP_AT_HRZ_PARTS:
	case MP_AT_VRT_PARTS:
	case MP_AT_TATEGU:
																// ***** ���u�����ށE�c�u�����ށE���� *****
		InpAttr::GetComboAttrR( MC_CMB_LHS1, &rComboAttr1);
		MhPlcInp::SetLenHosei( 0, rComboAttr1);					// �����␳�l1 �@���@�����␳�l1

		InpAttr::GetComboAttrR( MC_CMB_LHS2, &rComboAttr2);
		MhPlcInp::SetLenHosei( 1, rComboAttr2);					// �����␳�l2 �@���@�����␳�l2

		InpAttr::GetComboAttrR( MC_CMB_ZJSZ, &rComboAttr3);
		MhPlcInp::SetSinZure( rComboAttr3);						// �ގ��c����ʁ@���@�ގ��c�����

		InpAttr::GetComboAttrR( MC_CMB_HAIZ, &rComboAttr4);
		MhPlcInp::SetPlcZure( rComboAttr4);						// �z�u�_����ʁ@���@�z�u�_�����

		ist = InpAttr::GetComboAttrR( MC_CMB_TRTH, &rComboAttr5);		// ���t�������@���@����1
		if ( ist == 0)
			ist = InpAttr::GetComboAttrR( MC_CMB_LWRH, &rComboAttr5);	// ���[�����@���@����1

		MhPlcInp::SetHgt( 0, rComboAttr5);						// ���@����1
		MhTateguInp::SetHeight( rComboAttr5);					// ���@�����

		ist = InpAttr::GetComboAttrR( MC_CMB_KROH, &rComboAttr6);		// ����ROH�@���@����2
		if ( ist ==0)
			ist = InpAttr::GetComboAttrR( MC_CMB_UPRH, &rComboAttr6);	// ��[�����@���@����2

		MhPlcInp::SetHgt( 1, rComboAttr6);						// ���@����2
		MhTateguInp::SetROH( rComboAttr6);						// ���@����ROH

//		iIdTpPts = InpAttr::GetCurIdTpPts();							// �J�����g�̕���ID
//		pTpPts = BuzaiCode::MhGetpTpPts( iIdTpPts);						// ���i��ރ��R�[�h
//		if ( pTpPts->IsTategu()) {								// ���� (���ރR�[�h = ����)
//		}
		break;

	case MP_AT_ADJLNG:										// ***** ���ޒ����������� *****
	case MP_AT_YANE:											// ***** �������� *****

		ASSERT(false);											// �G���[
		break;
	}

}

//////////////////////////////////////////////////////////////////////////////
//	�����l���͗p�`�F�b�N�{�b�N�X�ԍ����擾����

static MINT GetCheckAttrNo(
						MCCHKATTR	iAttr			// (I  ) ����ID
				)
{
	MINT ic;
	MINT iChkNo = 0;

	for ( ic=0; ic<MC_SZ_CHKATTR; ic++) {
		if ( iAttr == z_Check_Attr[ic])
			break;
	}
	if ( ic < MC_SZ_CHKATTR)
		iChkNo = ic + 1;
	return iChkNo;
}


///////////////////////////////////////////////////////////////////////////////
//	�����l���͗p�`�F�b�N�{�b�N�X�̃^�C�g����ݒ肷��
static void SetCheckAttrText(
						MINT	iAttr,				// (I  ) �`�F�b�N�{�b�N�X�ԍ�
						MCHAR*	sTitle				// (I  ) �^�C�g���@�܂��́@NULL:�`�F�b�N�{�b�N�X��\�����Ȃ�
				)
{
	CButton* pCheckAttr;
	
	pCheckAttr = (CButton*)(System::GetpMainFrame()->m_wndDlgBar5.GetDlgItem(z_IdcCheckAttr[iAttr - 1]));

	if ( sTitle) {
		pCheckAttr->ShowWindow( 1);
		pCheckAttr->SetWindowText( sTitle);
	} else {
		pCheckAttr->ShowWindow( 0);
	}
}

///////////////////////////////////////////////////////////////////////////////
//	�����l���͗p�`�F�b�N�{�b�N�X�̃`�F�b�N�}�[�N��ݒ肷��

static void SetCheckAttrCkb(
						MINT	iChkNo,				// (I  ) �`�F�b�N�{�b�N�X�ԍ�
						MINT	iCheckAttr			// (I  ) �`�F�b�N�}�[�N�@0:OFF, 1:ON
				)
{
	CButton* pCheckAttr;
	
	pCheckAttr = (CButton*)(System::GetpMainFrame()->m_wndDlgBar5.GetDlgItem(z_IdcCheckAttr[iChkNo - 1]));

	pCheckAttr->SetCheck( iCheckAttr);
}

///////////////////////////////////////////////////////////////////////////////
//	�`�F�b�N�{�b�N�X�̃`�F�b�N�}�[�N��ݒ肷��

void InpAttr::SetCheckAttr(
						MCCHKATTR	iAttr,			// (I  ) ����ID
						MINT		iCheckAttr		// (I  ) �`�F�b�N�}�[�N�@0:OFF, 1:ON
				)
{
	MINT iChkNo = GetCheckAttrNo( iAttr);
	if ( iChkNo != 0)
		SetCheckAttrCkb( iChkNo, iCheckAttr);
}

///////////////////////////////////////////////////////////////////////////////
//	�����l���͗p�`�F�b�N�{�b�N�X�̃`�F�b�N�L�����擾����
//
static MINT GetCheckAttrCkb(						// (  O) �`�F�b�N�}�[�N�@0:OFF, 1:ON
						MINT	iChkNo				// (I  ) �`�F�b�N�{�b�N�X�ԍ�
				)
{
	CButton* pCheckAttr;

	pCheckAttr = (CButton*)(System::GetpMainFrame()->m_wndDlgBar5.GetDlgItem(z_IdcCheckAttr[iChkNo - 1]));

	return pCheckAttr->GetCheck();
}


///////////////////////////////////////////////////////////////////////////////
//	�`�F�b�N�{�b�N�X�̃`�F�b�N�L�����擾����
//

MINT InpAttr::GetCheckAttr(								// (  O) �X�e�C�^�X0(���ݒ�) 1(�ݒ�)
						MCCHKATTR	iAttr,			// (I  ) ����ID
						MINT		*iCheckAttr		// (  O) �`�F�b�N�}�[�N�@0:OFF, 1:ON
				)
{
	MINT	ist;
	MINT	iChkNo;

	iChkNo = GetCheckAttrNo( iAttr);

	if ( iChkNo != 0) {
		*iCheckAttr = GetCheckAttrCkb( iChkNo);
		ist = 1;
	} else {
		*iCheckAttr = 0;
		ist = 0;
	}
	return ist;
}

/////////////////////////////////////////////////////////////////////////////
//	�����l���̓��[�h	(���ޑ������͗p�_�C�A���O�o�[�̍��ڐݒ蒆�̑����l���̓��[�h)

static	MINT	z_iMode = 0;						// �����l���̓��[�h
													//	MP_AT_NONE(0)			:�����l���͂Ȃ�
													//	MP_AT_HRZ_PARTS(1)	:�������ޓ���
													//	MP_AT_VRT_PARTS(2)	:�������ޓ���
													//	MP_AT_YTPANEL(3)		:���V��p�l������
													//	MP_AT_YANEPANEL(4)	:�����p�l������
													//	MP_AT_ADJLNG(5)		:���ޒ�����������
													//	MP_AT_YANE(6)			:��������
													//	MP_AT_TATEGU(7)		:�������

/////////////////////////////////////////////////////////////////////////////
//	�\���R�[�h�A���ށA���ގ��ID���A�����l���̓��[�h�����߂�

MINT MmCalcInpAtMode()
{
	MINT		iMode;							// �����l���̓��[�h
	MINT		iGp;
	MINT		iBr;

	MINT		iIdTpPts;
	MhTpPts*	pTpPts;

	// ���ޑ����l���̓��[�h( iMode)(�\������)�̌���
	iGp = InpMode::GetKGp();
	iBr = InpMode::GetKBr();

	iIdTpPts = InpAttr::GetCurIdTpPts();
	pTpPts = BuzaiCode::MhGetpTpPts( iIdTpPts);

	if ( pTpPts->IsTategu()) {									// (���ގ��ID == ����)
		iMode = MP_AT_TATEGU;									//			�������

	} else if ( pTpPts->IsFrame())	{							// (���ގ��ID == ���Ęg | ��) (�ˑ��R�[�h == ����)
		iMode = MP_AT_VRT_PARTS;								//			�������ޓ���

	} else if ( pTpPts->IsPanel() || pTpPts->IsKaiko()) {		// (���ގ��ID == �p�l�� | �J��)
		if ( iGp == MP_GP_YUKA || iGp == MP_GP_TENJO)			//		(�\���R�[�h == �� | �V��)
			iMode = MP_AT_YTPANEL;								//			���E�V��p�l������

		else if ( iGp == MP_GP_YANE)							//		(�\�� == ����)
			iMode = MP_AT_YANEPANEL;							//			�����p�l������

		else													//		(���̑�)
			iMode = MP_AT_HRZ_PARTS;							//			���u�����ޓ���

	} else if ( iBr == MP_BR_YANE) {							// (���� == ����)
		if ( iGp == MP_GP_YANE) 								//		(�\�� == ����)
			iMode = MP_AT_YANE;									//			��������
		else													//		(���̑�)
			iMode = MP_AT_HRZ_PARTS;							//			���u�����ޓ���

	} else {													// (���̑�)
		iMode = MP_AT_HRZ_PARTS;								//			���u������
	}
	return iMode;
}

/////////////////////////////////////////////////////////////////////////////
//	�J�����g�̑����l���̓��[�h���擾����
//	(���ޑ������͗p�_�C�A���O�o�[�̍��ڐݒ蒆�̑����l���̓��[�h)
//
MINT InpMode::GetAtMd()
{
	return z_iMode;												// �����l���̓��[�h
}

/////////////////////////////////////////////////////////////////////////////
//	�����l���̓��[�h�ŕ��ޑ������͗p�_�C�A���O�o�[�̍��ڂ�ݒ肷��
//	 (�R���{�{�b�N�X�ƃ`�F�b�N�{�b�N�X�̍��ڂ�ݒ肷��)
//	
void InpAttr::InitComboAttr(
						MINT	iMode				// �����l���̓��[�h
													//  MP_AT_AUTO(-1)		:�����ݒ�
													//	MP_AT_NONE(0)			:�����l���͂Ȃ�
													//	MP_AT_HRZ_PARTS(1)	:�������ޓ���
													//	MP_AT_VRT_PARTS(2)	:�������ޓ���
													//	MP_AT_YTPANEL(3)		:���V��p�l������
													//	MP_AT_YANEPANEL(4)	:�����p�l������
													//	MP_AT_ADJLNG(5)		:���ޒ�����������
													//	MP_AT_YANE(6)			:��������
													//	MP_AT_TATEGU(7)		:�������
				)
{
	if ( iMode >= 0)
		z_iMode = iMode;
	else
		z_iMode = MmCalcInpAtMode();

	//	���ޑ������͗p�R���{�{�b�N�X�ƃ`�F�b�N�{�b�N�X�̍��ڂ�ݒ肷��
	switch (z_iMode)
	{
	case	MP_AT_NONE:
		MmsInitComboAttr( MC_CMB_NULL, MC_CMB_NULL, MC_CMB_NULL,	// null, null, null
						MC_CMB_NULL, MC_CMB_NULL, MC_CMB_NULL,	// null, null, null
						MC_CHK_NULL, MC_CHK_NULL,				// null, null
						MC_CHK_NULL, MC_CHK_NULL);				// null, null
		break;
	case	MP_AT_HRZ_PARTS:									// ���u����
		MmsInitComboAttr( MC_CMB_LHS1, MC_CMB_LHS2, MC_CMB_ZJSZ,	// �����␳1, �����␳2, �c����
						MC_CMB_TRTH, MC_CMB_INTR, MC_CMB_HONS,	// ��t����, �Ԋu, �{��
						MC_CHK_NULL, MC_CHK_NULL,				// null, null
						MC_CHK_NULL, MC_CHK_NULL);				// null, null
		break;
	case	MP_AT_VRT_PARTS:									// ���Ęg
		MmsInitComboAttr( MC_CMB_UPRH, MC_CMB_LWRH, MC_CMB_HAIZ,	// ��[����, ���[����, �z�u����
						MC_CMB_NULL, MC_CMB_INTR, MC_CMB_HONS,	// null, �Ԋu, �{��
						MC_CHK_NULL, MC_CHK_NULL,				// null, null
						MC_CHK_NULL, MC_CHK_NULL);				// null, null
		break;
	case	MP_AT_TATEGU:										// ����
		MmsInitComboAttr( MC_CMB_LHS1, MC_CMB_LHS2, MC_CMB_TRTH,	// �����␳1, �����␳2, ��t����
						MC_CMB_KROH, MC_CMB_HAIZ, MC_CMB_NULL,	// �q�n�g, �z�u���� ,null
						MC_CHK_NULL, MC_CHK_NULL,				// null, null 
						MC_CHK_NULL, MC_CHK_NULL);				// null, null
		break;
	case	MP_AT_YTPANEL:										// ���E�V��p�l��
		MmsInitComboAttr( MC_CMB_LFH1, MC_CMB_RTH1, MC_CMB_TRTH,	// ���Е␳, �E�Е␳, ��t����
						MC_CMB_FHS1, MC_CMB_OKYK, MC_CMB_OHS2,	// ��O�␳, ���s, ���s�␳
						MC_CHK_NULL, MC_CHK_NULL,				// null, null
						MC_CHK_NULL, MC_CHK_NULL);				// null, null
		break;
	case	MP_AT_YANEPANEL:									// �����p�l��
		MmsInitComboAttr( MC_CMB_LFH2, MC_CMB_RTH2, MC_CMB_TRTH,	// ���Е␳, �E�Е␳, ��t����
						MC_CMB_FHS2, MC_CMB_OKYK, MC_CMB_OHS2,	// ���̏o, ���s, ���s�␳
						MC_CHK_NULL, MC_CHK_NULL,				// null, null
						MC_CHK_NULL, MC_CHK_NULL);				// null, null
		break;
	case	MP_AT_ADJLNG:										// ���ޒ�������
		MmsInitComboAttr( MC_CMB_LHS1, MC_CMB_NULL, MC_CMB_NULL,	// �����␳, null, null
						MC_CMB_NULL, MC_CMB_NULL, MC_CMB_NULL,	// null, null, null
						MC_CHK_NULL, MC_CHK_KATI,				// null, ����
						MC_CHK_MULT, MC_CHK_INTC);				// ����, �������ޒ���
		break;
	case	MP_AT_YANE:											// ����
		MmsInitComboAttr( MC_CMB_KOBY, MC_CMB_NKDE, MC_CMB_KRDE,	// �������z, ���̏o, �P���o�̏o
						MC_CMB_NULL, MC_CMB_NULL, MC_CMB_NULL,	// null, null, null
						MC_CHK_NULL, MC_CHK_NULL,				// null, null
						MC_CHK_NULL, MC_CHK_NKTP);				// null, ���^�C�v
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
//	���ޑ������͗p�R���{�{�b�N�X�ƃ`�F�b�N�{�b�N�X�̍��ڂ�ݒ肷��

static void MmsInitComboAttr(
					MCCMBATTR	icombo1,			// �������͗p�R���{�{�b�N�X1�̑���ID
					MCCMBATTR	icombo2,			// �������͗p�R���{�{�b�N�X2�̑���ID
					MCCMBATTR	icombo3,			// �������͗p�R���{�{�b�N�X3�̑���ID
					MCCMBATTR	icombo4,			// �������͗p�R���{�{�b�N�X4�̑���ID
					MCCMBATTR	icombo5,			// �������͗p�R���{�{�b�N�X5�̑���ID
					MCCMBATTR	icombo6,			// �������͗p�R���{�{�b�N�X6�̑���ID
					MCCHKATTR	icheck1,			// �������͗p�`�F�b�N�{�b�N�X1�̑���ID
					MCCHKATTR	icheck2,			// �������͗p�`�F�b�N�{�b�N�X2�̑���ID
					MCCHKATTR	icheck3,			// �������͗p�`�F�b�N�{�b�N�X3�̑���ID
					MCCHKATTR	icheck4				// �������͗p�`�F�b�N�{�b�N�X4�̑���ID
				)
{
	z_Combo_Attr[0] = icombo1; z_Combo_Attr[1] = icombo2; z_Combo_Attr[2] = icombo3;
	z_Combo_Attr[3] = icombo4; z_Combo_Attr[4] = icombo5; z_Combo_Attr[5] = icombo6;
	z_Check_Attr[0] = icheck1; z_Check_Attr[1] = icheck2;
	z_Check_Attr[2] = icheck3; z_Check_Attr[3] = icheck4;

	MINT	ic1;
	
	for ( ic1=1; ic1<=MC_SZ_CMBATTR; ic1++) {
		switch (z_Combo_Attr[ic1-1]) {
			case MC_CMB_NULL:
				// �R���{�{�b�N�X�����������ɂ���
				SetComboAttrText( ic1, Mstr( ""));
				InitComboAttrR( ic1, 0, NULL, 0);
				break;
			case MC_CMB_LHS1:
				// �����␳�l1�I��p�R���|�{�b�N�X�̍��ڂ�ݒ肷��
				SetComboAttrText( ic1, Mstr( "�����␳1"));
				InitComboAttrR( ic1, ISZLNGH, z_rLngH, INITLNGTH);
				break;
			case MC_CMB_LHS2:
				// �����␳�l2�I��p�R���|�{�b�N�X�̍��ڂ�ݒ肷��
				SetComboAttrText( ic1, Mstr( "�����␳2"));
				InitComboAttrR( ic1, ISZLNGH, z_rLngH, INITLNGTH);
				break;
			case MC_CMB_ZJSZ:
				// �ގ��c����ʑI��p�R���|�{�b�N�X�̍��ڂ�ݒ肷��
				SetComboAttrText( ic1, Mstr( "�c����"));
				InitComboAttrR( ic1, ISZSINZ, z_rSinZ, INITSINZ);
				break;
			case MC_CMB_HAIZ:
				// �z�u�_����ʑI��p�R���|�{�b�N�X�̍��ڂ�ݒ肷��
				SetComboAttrText( ic1, Mstr( "�z�u����"));
				InitComboAttrR( ic1, ISZHAIZ, z_rHaiZ, INITHAIZ);
				break;
			case MC_CMB_TRTH:
				// ���t�������I��p�R���|�{�b�N�X�̍��ڂ�ݒ肷��
				SetComboAttrText( ic1, Mstr( "��t����"));
				InitComboAttrR( ic1, ISZZ1, z_rZ1, INITZ1);
				break;
			case MC_CMB_KROH:
				// �J��������(ROH)�I��p�R���|�{�b�N�X�̍��ڂ�ݒ肷��
				SetComboAttrText( ic1, Mstr( "�q�n�g"));
				InitComboAttrR( ic1, ISZZ2, z_rZ2, INITZ2);
				break;
			case MC_CMB_INTR:
				// �Ԋu�I��p�R���|�{�b�N�X�̍��ڂ�ݒ肷��
				SetComboAttrText( ic1, Mstr( "�Ԋu"));
				InitComboAttrR( ic1, ISZINTV, z_rIntv, INITINTV);
				break;
			case MC_CMB_HONS:
				// �{���I��p�R���|�{�b�N�X�̍��ڂ�ݒ肷��
				SetComboAttrText( ic1, Mstr( "�{��"));
				InitComboAttrI( ic1, ISZNUM, z_iNum, INITNUM);
				break;

//			case MC_CMB_HHS1:
//				// ���␳�l1�I��p�R���|�{�b�N�X�̍��ڂ�ݒ肷��
//				SetComboAttrText( ic1, Mstr( "���␳1"));
//				InitComboAttrR( ic1, ISZLNGH2, z_rLngH2, INITLNGTH2);
//				break;
//			case MC_CMB_HHS2:
//				// ���␳�l2�I��p�R���|�{�b�N�X�̍��ڂ�ݒ肷��
//				SetComboAttrText( ic1, Mstr( "���␳2"));
//				InitComboAttrR( ic1, ISZLNGH2, z_rLngH2, INITLNGTH2);
//				break;

			case MC_CMB_FHS1:
				// ����V��p�l���p�@��O�␳�I��p�R���|�{�b�N�X�̍��ڂ�ݒ肷��
				SetComboAttrText( ic1, Mstr( "��O�␳"));
				InitComboAttrR( ic1, ISZOKUYUKIH, z_rOkuH, INITOKUYUKIH);
				break;
			case MC_CMB_FHS2:
				// �����p�l���p�@��O�␳�I��p�R���|�{�b�N�X�̍��ڂ�ݒ肷��
				SetComboAttrText( ic1, Mstr( "���̏o"));
				InitComboAttrR( ic1, ISZKOUBAI, z_rNokinoDe, INITNOKINODE);
				break;
			case MC_CMB_OKYK:
				// ���s���I��p�R���|�{�b�N�X�̍��ڂ�ݒ肷��
				SetComboAttrText( ic1, Mstr( "���s"));
				InitComboAttrR( ic1, ISZOKUYUKI, z_rOku, INITOKUYUKI);
				break;
			case MC_CMB_OHS2:
				// ���s���␳�I��p�R���|�{�b�N�X�̍��ڂ�ݒ肷��
				SetComboAttrText( ic1, Mstr( "���s�␳"));
				InitComboAttrR( ic1, ISZOKUYUKIH, z_rOkuH, INITOKUYUKIH);
				break;
			case MC_CMB_KOBY:
				// �������z�I��p�R���|�{�b�N�X�̍��ڂ�ݒ肷��
				SetComboAttrText( ic1, Mstr( "�������z"));
				InitComboAttrI( ic1, ISZKOUBAI, z_iKoubai, INITKOUBAI);
				break;
			case MC_CMB_NKDE:
				// ���̏o�I��p�R���|�{�b�N�X�̍��ڂ�ݒ肷��
				SetComboAttrText( ic1, Mstr( "���̏o"));
				InitComboAttrR( ic1, ISZNOKINODE, z_rNokinoDe, INITNOKINODE);
				break;
			case MC_CMB_KRDE:
				// �P���o�̏o�I��p�R���|�{�b�N�X�̍��ڂ�ݒ肷��
				SetComboAttrText( ic1, Mstr( "�P���o�̏o"));
				InitComboAttrR( ic1, ISZKERABANODE, z_rKerabanoDe, INITKERABANODE);
				break;
			case MC_CMB_UPRH:
				// ��[�����I��p�R���|�{�b�N�X�̍��ڂ�ݒ肷��
				SetComboAttrText( ic1, Mstr( "��[����"));
				InitComboAttrR( ic1, ISZZ2, z_rZ2, INITZ2);
				break;
			case MC_CMB_LWRH:
				// ���[�����I��p�R���|�{�b�N�X�̍��ڂ�ݒ肷��
				SetComboAttrText( ic1, Mstr( "���[����"));
				InitComboAttrR( ic1, ISZZ1, z_rZ1, INITZ1);
				break;
			case MC_CMB_LFH1:
				// ����V��p�l���p�@���Е␳(�����␳�l1)�I��p�R���|�{�b�N�X�̍��ڂ�ݒ肷��
				SetComboAttrText( ic1, Mstr( "���Е␳"));
				InitComboAttrR( ic1, ISZLNGH, z_rLngH, INITLNGTH);
				break;
			case MC_CMB_RTH1:
				// ����V��p�l���p�@�E�Е␳(�����␳�l2)�I��p�R���|�{�b�N�X�̍��ڂ�ݒ肷��
				SetComboAttrText( ic1, Mstr( "�E�Е␳"));
				InitComboAttrR( ic1, ISZLNGH, z_rLngH, INITLNGTH);
				break;
			case MC_CMB_LFH2:
				// �����p�l���p�@���Е␳(�����␳�l1)�I��p�R���|�{�b�N�X�̍��ڂ�ݒ肷��
				SetComboAttrText( ic1, Mstr( "���Е␳"));
				InitComboAttrR( ic1, ISZKERABANODE, z_rKerabanoDe, INITLNGTH);
				break;
			case MC_CMB_RTH2:
				// �����p�l���p�@�E�Е␳(�����␳�l2)�I��p�R���|�{�b�N�X�̍��ڂ�ݒ肷��
				SetComboAttrText( ic1, Mstr( "�E�Е␳"));
				InitComboAttrR( ic1, ISZKERABANODE, z_rKerabanoDe, INITLNGTH);
				break;
		}
	}
	for ( ic1=1; ic1<=MC_SZ_CHKATTR; ic1++) {
		switch (z_Check_Attr[ic1-1]) {
			case MC_CHK_NULL:
				// �`�F�b�N�{�b�N�X�����������ɂ���
				SetCheckAttrText( ic1, NULL);		
				SetCheckAttrCkb( ic1, 0);		
				break;
			case MC_CHK_KATI:
				// �����������͗p�`�F�b�N�{�b�N�X��ݒ肷��
				SetCheckAttrText( ic1, Mstr( "����"));
				SetCheckAttrCkb( ic1, 0);
				break;
			case MC_CHK_MULT:
				// �������͗p�`�F�b�N�{�b�N�X��ݒ肷��
				SetCheckAttrText( ic1, Mstr( "����"));
				SetCheckAttrCkb( ic1, 1);
				break;
			case MC_CHK_INTC:
				// �������ޒ����p�`�F�b�N�{�b�N�X��ݒ肷��
				SetCheckAttrText( ic1, Mstr( "�������ޒ���"));
				SetCheckAttrCkb( ic1, 1);
				break;
			case MC_CHK_NKTP:
				// ����^�C�v(0:���z���܂�A1:����)
				SetCheckAttrText( ic1, Mstr( "����^�C�v����"));
				SetCheckAttrCkb( ic1, 1);
				break;
		}
	}
}

} // namespace MC
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MmDrag.cpp
//
//		�h���b�M���O����
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
#define DLL_EXPORT_MC_HAITIIN_DO
#define DLL_EXPORT_MC_WINDOW_DO
#include "MmLib.h"

#include "MmDrag.h"
#include "MmWnd.h"

#include "MmDefine.h"
#include "MsBitSet.h"
#include "MmOpt.h"
#include "MmHist.h"

#include "MbList.h"
#include "MhLib.h"

#include "MhPlcInfo.h"
#include "MgGrp.h"
#include "MhRoof.h"
#include "MdLib.h"

#include "MmSystemProperty.h"

namespace MC
{

static	bool			z_fDrawHitBzi = FALSE;

/////////////////////////////////////////////////////////////////////////////
//  ���ނ̌`������߂�

static void PtsShape(
						MhPlcInfo	*pPlcEn,
						MgPolyg2*	pgPtsShape
				);

static	MgPoint2 z_ptBziFig[20];
static	MgPolyg2 z_pgBziFig( 20, z_ptBziFig);

////////////////////////////////////////////////////////////////////////////
//	�J�����g�I���̕��ތ`���ݒ肷��

void Window::SetCurBziFig(
						 MgPolyg2*	ppgBziFig		// (I  ) �J�����g�I���̕��ތ`��
				)
{
	z_pgBziFig = *ppgBziFig;
}

////////////////////////////////////////////////////////////////////////////
//	�J�����g�I���̕��ތ`����N���A����

void Window::ClrCurBziFig()
{
	z_pgBziFig.m_n = 0;
}

////////////////////////////////////////////////////////////////////////////
//	�J�����g�I���̕��ތ`���Ԃ�
//	�Ԓl�@�J�����g�I���̕��ތ`��

MgPolyg2* Window::GetCurBziFig()
{
	return &z_pgBziFig;
}

static	MhRfm* 	z_pRfm = NULL;

////////////////////////////////////////////////////////////////////////////
//	�J�����g�I���̉����ʂ�ݒ肷��

void HaitiIn::SetCurRfm(
						MhRfm* 	pRfm				// (I  ) �J�����g�I���̉�����
				)
{
	z_pRfm = pRfm;
}

////////////////////////////////////////////////////////////////////////////
//	�J�����g�I���̉����ʂ�Ԃ�

MhRfm* HaitiIn::GetCurRfm()
{
	return z_pRfm;
}

/////////////////////////////////////////////////////////////////////////////
//	�e���|�����\��

void WindowCtrl::MmWndKDrawTemp(
				MmWndInfo*		pWndInfo,
				MbCod*			pCod
				)
{
	// �����ʂ̑I���󋵂�\������
	MgPolyg2	pgRfm(20);

	MhRfm *pRfm = HaitiIn::GetCurRfm();
	if ( pRfm) {

		CClientDC	dc( pWndInfo->GetWnd());
		DWORD		rgbTempRfm = MmSystemProperty::GetColor( MM_COLOR_TEMP);

		pCod->SetLineAttr( MPS_SOLID, 1, rgbTempRfm);

		pgRfm = pRfm->m_Pg;
		pCod->Polygon( pgRfm.m_p, pgRfm.m_n);
	}

	MgPolyg2 *ppgBziFig = Window::GetCurBziFig();
	if ( ppgBziFig->m_n != 0) {

		CClientDC	dc( pWndInfo->GetWnd());
		DWORD		rgbTempRfm = MmSystemProperty::GetColor( MM_COLOR_TEMP);

		pCod->SetLineAttr( MPS_SOLID, 1, rgbTempRfm);

		pCod->Polygon( ppgBziFig->m_p, ppgBziFig->m_n);
	}
}

/////////////////////////////////////////////////////////////////////////////
//	�V�K�E�B���h�E�쐬���̏����� (�r���[�̏����\�����Ɏg�p)
//	�q�b�g���ނ��\������Ă��Ȃ���Ԃł��邱�Ƃ�ݒ肷��

void Window::InitHitBuzai()
{
	z_fDrawHitBzi = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//	�q�b�g���ނ̕\������������

void Window::EraseHitBzi()
{
	if ( z_fDrawHitBzi) {
		Window::DispHitBzi( -1, NULL, NULL);
		z_fDrawHitBzi = FALSE;
	}
	Window::ResetDispHitBzi();
}

/////////////////////////////////////////////////////////////////////////////
//	�q�b�g���ނ�\������
//	�i���\���q�b�g���ނ�������ɁA�q�b�g���ނ�\������j

void Window::DrawHitBzi(
						MmWndInfo*	pWndInfo,		// (I  ) �E�B���h�E�Ǘ����
						MgPolyg2*	pgHitBzi		// (I  ) �q�b�g���ނ̌`��
				)
{
	// ���\���q�b�g���ނ�����
	if ( z_fDrawHitBzi)
		Window::DispHitBzi( -1, NULL, NULL);

	// �q�b�g���ނ�\��
	Window::DispHitBzi( 1, pWndInfo, pgHitBzi);
	z_fDrawHitBzi = TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	�q�b�g���ނ��ĕ\������

void Window::ReDispHitBzi()
{
	if ( !z_fDrawHitBzi) {
		Window::DispHitBzi( 0, NULL, NULL);
		z_fDrawHitBzi = TRUE;
	}
}

static	MmWndInfo* 		z_pHitBziWndInfo = NULL;					// ���o�[�o���h���͕\���p�J�����g�E�B���h�E
static	MgPolyg2		z_pgHitBzi(10);

/////////////////////////////////////////////////////////////////////////////
//	�q�b�g���ނ̌`���\��/�ĕ\��/��������

void Window::ResetDispHitBzi()
{
	z_pHitBziWndInfo = NULL;
}

/////////////////////////////////////////////////////////////////////////////
//	�q�b�g���ނ̌`���\��/�ĕ\��/��������

MINT Window::DispHitBzi(
						MINT		ictl,			// (I  ) �\������ 1 : �\���A0 : �ĕ\���A-1 : ����
						MmWndInfo*	pWndInfo,		// (I  ) �E�B���h�E�Ǘ����(�\������ == 1) �܂��� NULL(�\������ <= 0)
						MgPolyg2*	pgHitBzi		// (I  ) �q�b�g���ނ̌`��(�\������ == 1) �܂��� NULL(�\������ <= 0)
				)
{
	MINT		ic1;
	CPoint		ptLHitBzi[10];

	MbCod*		pCod;

	if ( ictl == 1) {
		z_pHitBziWndInfo = pWndInfo;
		z_pgHitBzi = *pgHitBzi;													// �q�b�g���ނ̌`��
	}
	if ( z_pHitBziWndInfo == NULL)
		return 0;

	pCod = z_pHitBziWndInfo;

	CClientDC	dc( z_pHitBziWndInfo->GetWnd());

	DWORD		rgbHitBzi = MmSystemProperty::GetColor( MM_COLOR_HITBZI);
	CPen		HitBziPen( PS_SOLID, 1, rgbHitBzi);
	CPen*		OldPen = dc.SelectObject( &HitBziPen);

	MINT		iMapMode = z_pHitBziWndInfo->GetMapMode();
	MINT		iOldMM = dc.SetMapMode( iMapMode);

	dc.SetROP2( R2_XORPEN);														// �h���b�O�p�F���]�y��

	for ( ic1=0; ic1<z_pgHitBzi.m_n; ic1++)
		ptLHitBzi[ic1] = z_pHitBziWndInfo->RPtoLP(z_pgHitBzi.m_p[ic1]);
	ptLHitBzi[ic1] = ptLHitBzi[0];
	dc.Polyline( ptLHitBzi, z_pgHitBzi.m_n + 1);

	dc.SetMapMode( iOldMM);
	dc.SelectObject( OldPen);
	return 1;
}

} // namespace MC
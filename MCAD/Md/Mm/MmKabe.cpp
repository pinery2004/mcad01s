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
#include "MmDrag.h"
#include "MmWnd.h"
#include "MmDefine.h"
#include "MmLib.h"
#include "MmSystemProperty.h"

#include "MmCmd.h"
#include "resource.h"

#include "MsBitSet.h"
#include "MmHist.h"

#include "MbList.h"
#include "MhLib.h"

#include "MhPlcInfo.h"

#include "MmPrompt.h"

#define		MAXHAIKABE		500									// �z�񐧌��l
#define		MC_KABE_TOL		44.5								// �ǔ���g�������X

namespace MC
{

/////////////////////////////////////////////////////////////////////////////
//	�ǐc�ۂߐ����擾����
//
void Input::GetMarumeKabeLine(
						MINT		iKai,			// �����K
				const	MgPoint2	&Pi,			// ���͍��W
						MgGLine2	*pGLn			// �Ώەǐc
				)
{
	MINT		ist1;
	MINT		iCdMarume;
	MREAL		rTol;
	MhPlcInfo*	pHaiKabe[MAXHAIKABE];				// �ǃf�[�^
	MINT		nHaiKabe;							// �ǐ�
	MINT		ic2;
	MgLine2		ln1;

	iCdMarume = InpMode::GetComboCdMarume();
	if ( iCdMarume == 0)
		rTol = MC_KABE_TOL;
	else
//		rTol = g_SysProp.Real[MM_REAL_PITCH] / iCdMarume;
		rTol = MmSystemProperty::GetReal( MM_REAL_PITCH) / iCdMarume;

	nHaiKabe = HaitiIn::GetPts( iKai, MP_GP_TAIRYOKU, Mstr( "��"), NULL, MAXHAIKABE, pHaiKabe);
																				// �ƃ��f�����J�����g�K�̑S�Ă̕ǂ��擾
	pGLn->m_n = 0;
	for ( ic2=0; ic2<nHaiKabe; ic2++) {											// ��
		ln1 = MgLine2C( pHaiKabe[ic2]->GetPIPlcIti());
		if ( MGeo::ChkPt2OnLn2WS( Pi, ln1, &ist1, MC_KABE_TOL))
			(*pGLn) += MgLine2C( pHaiKabe[ic2]->GetPIPlcIti());
	}
	if ( pGLn->m_n == 0) {
		Msg::ErrorMsg( Mstr( "�Ǐ�ɓ��͂��ĉ�����"), MC_ERR_BASE);				// ""
	}
}

} // namespace MC
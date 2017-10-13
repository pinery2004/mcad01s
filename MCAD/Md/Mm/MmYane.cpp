//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MmDrag.cpp
//
//		ドラッギング操作
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

#include "MdLib.h"
#include "MhLib.h"


#include "MbList.h"
#include "MhRoof.h"
#include "MhLib.h"

#include "MhPlcInfo.h"

#include "MmOpt.h"
#include "MmValid.h"

#include "MmPrompt.h"


namespace MC
{

/////////////////////////////////////////////////////////////////////////////
//	屋根構成線丸め線を取得する
//
void Input::MmGetMarumeYaneLine(
						MINT		iKai,			// 検索階
				const	MgPoint2	&Pi,			// 入力座標
						MgGLine2	*pGLn			// 対象屋根構成線
				)
{
	MINT		ist1;
	MINT		iCdMarume;
	MREAL		rTol;
	MINT		ic1, ic2, ic2b;
	MhRoofInfo*	pRoofEn;
	MgPoint3	Py1;
	MgLine2		Ln1;

	iCdMarume = InpMode::GetComboCdMarume();
	if ( iCdMarume == 0)
		rTol = MC_MARUME_TOL;
	else
		rTol = MmSystemProperty::GetReal( MM_REAL_PITCH) / ( iCdMarume * 2);

	pGLn->m_n = 0;
	for ( pRoofEn = HaitiDb::MdGetHeadRoof(); pRoofEn!=0;
		  pRoofEn = HaitiDb::MdGetNextRoof()) {
		if ( pRoofEn->GetKai() != iKai)
			continue;
		if ( !MmChkValidRoof( pRoofEn))							// オプションと履歴のチェック
			continue;
		for ( ic1=0; ic1<pRoofEn->GetpGRfm()->m_n; ic1++) {
			if ( MGeo::Zero( pRoofEn->GetpGRfm()->m_pst[ic1].m_Pln.v.z))				// 矢切は無視
				continue;
			ic2b = pRoofEn->GetpGRfm()->m_pst[ic1].m_Pg.m_n - 1;
			for ( ic2=0; ic2<pRoofEn->GetpGRfm()->m_pst[ic1].m_Pg.m_n; ic2b=ic2,ic2++) {
				Ln1 = MgLine2( MgPoint2C( pRoofEn->GetpGRfm()->m_pst[ic1].m_Pg.m_P[ic2b]),
							   MgPoint2C( pRoofEn->GetpGRfm()->m_pst[ic1].m_Pg.m_P[ic2]));
				if ( MGeo::ChkPt2OnLn2WS( Pi, Ln1, &ist1, rTol))
					(*pGLn) += Ln1;
			}
		}
	}
}

} // namespace MC
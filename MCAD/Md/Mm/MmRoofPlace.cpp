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

#define DLL_EXPORT_MC_HAITIIN_DO
#include "MmLib.h"

#include "MmSystemProperty.h"

#include "MmCmd.h"
#include "resource.h"

#include "MsBitSet.h"
#include "MbList.h"
#include "MhLib.h"

#include "MgGrp.h"
#include "MhRoof.h"
#include "MdLib.h"

#include "MhInp.h"

#include "MmOpt.h"

#include "MmHist.h"
#include "MmValid.h"


#include	"MmSystemProperty.h"

namespace MC
{

/////////////////////////////////////////////////////////////////////////////
//	屋根を配置する
MINT HaitiIn::RoofPlc(									// (  O) ステイタス	0: 正常、-1: 屋根配置エラー
				const	MgPolyg2	&pgJim,			// (I  ) 地廻り区画
				const	MgGInt		&GifInp,		// (I  ) 地廻り線種類(仮想キー(nflag)  MK_SHIFT(004): シフトキー)
				const	MgPoint2	&pth			// (I  ) 方向点
				)
{
	MINT	ist = -1;
	MINT	ist1;
	MhRoofInfo	RoofEn;

	RoofEn.InitAllAtr();

	RoofEn.m_iKai = InpMode::GetKai();
	RoofEn.SetAttr();
	RoofEn.SetJimCd( pgJim, GifInp, pth);
	RoofEn.CreateRfmForJim();
	ist1 = RoofEn.AdjustRfm1();
	if ( ist1 < 0)
		MQUIT;
//								RoofEn.Print( Mstr( "1112 HaitiIn::RoofPlc"));
	RoofEn.ConnectRfm();
//								RoofEn.Print( Mstr( "1113 HaitiIn::RoofPlc"));
	RoofEn.OffsetRfm();
	ist1 = RoofEn.AdjustRfm2();
	if ( ist1 < 0)
		MQUIT;
//								RoofEn.Print( Mstr( "1114 HaitiIn::RoofPlc"));
	RoofEn.CreateRfmZukei();

	HaitiDb::MdRoofAdd( &RoofEn, 1);
	ist = 0;
exit:
	return ist;
}

/////////////////////////////////////////////////////////////////////////////
//	屋根を表示する
void WindowCtrl::MmWndKDrawRoof(
						MbCod*		pCod,			// 座標系
						MINT		iKaiC,			// 階  	(1,2,3)
						MINT		iGpC			// 構成
				)
{
	MPOSITION	pos1;
	MhRoofInfo	*pRoofEn;
//	MUINT		*pOpt;

	MgLine2		LnRoof;
	MgPoint2	PtRoofN;
	MgPoint2	PtW[2];
	MgPoint2	PtK[4];
	MgVect2		vPtH;
	MgVect2		vOffset = MgVect2( 0., 20.);

	DWORD		RoofLineColor;
	DWORD		RoofTextColor;

	MINT		ign;
	MINT		ic;

	if ( iGpC < MP_GP_TAIRYOKU)
		return;													// 耐力壁、天井、小屋、屋根　のみ表示する

	RoofTextColor = MmSystemProperty::GetColor( MM_COLOR_GRID_TEXT);
	pCod->SetTextAttr( Mstr( "ＭＳ ゴシック"), 100.f, MT_LOWER, MT_CENTER, RoofTextColor,
					   MgVect2( 1., 0.), vOffset, MT_FREESIZE);

	ign = 0;
	for ( pRoofEn = HaitiDb::MdGetHeadRoof( &pos1); pRoofEn!=0;
		  pRoofEn = HaitiDb::MdGetNextRoof( &pos1)) {

		if ( pRoofEn->GetKai() != iKaiC)
			continue;											// 異なる階の部材は表示しない
		if ( !MmChkValidRoof( pRoofEn))							// オプションと履歴のチェック
			continue;

		RoofLineColor = RGB( 10, 30, 30);
		pCod->SetLineAttr( MPS_SOLID, 1, RoofLineColor);
		if ( pRoofEn->GetpZukei()) {
			for ( ic=0; ic<pRoofEn->GetpZukei()->m_nZukei; ic++) {
				pCod->Line( MgLine2C( pRoofEn->GetpZukei()->m_lnZukei[ic]));
			}
		}

	}
}

static MPOSITION	z_RoofPos;

/////////////////////////////////////////////////////////////////////////////
//	屋根面を検索する
//	返値 =NULL: 該当屋根面なし !=NULL: 該当屋根面数
MhRfm*	HaitiIn::SrchRfm(									// (  O) 屋根面　または　NULL
						MINT		imode,			// 検索モード　指示点に該当する屋根面が複数ある場合の優先選択条件
													//				MC_PRI_MIN_AREA(0):	 面積が小を優先
													//				MC_PRI_MIN_HIGHT(1): 高さが小を優先
													//				MC_PRI_MAX_HIGHT(2) | その他: 高さが大を優先	
				const	MgPoint2	&pt1			// 指示点
				)
{
	MINT		ist1;

	MPOSITION	pos1;
	MhRoofInfo	*pRoofEn;

	MINT		ic1;

	MREAL		rArea;

	MgPolyg2	pgRfm;
	MhRfm		*pRfm = 0;
	MREAL		rHight;

	MINT	iKaiC = InpMode::GetKai();								// 現在の階

	MREAL	rMinArea = MREALMAX;
	MREAL	rMinHight = MREALMAX;
	MREAL	rMaxHight = MREALMIN;

	for ( pRoofEn = HaitiDb::MdGetHeadRoof( &pos1); pRoofEn!=0;
		  pRoofEn = HaitiDb::MdGetNextRoof( &pos1)) {

		if ( pRoofEn->GetKai() != iKaiC)
			continue;											// 異なる階の部材は対象外
		if ( !MmChkValidRoof( pRoofEn))							// オプションと履歴のチェック
			continue;

		for ( ic1=0; ic1<pRoofEn->GetpGRfm()->m_n; ic1++) {
			if ( MGeo::Zero( pRoofEn->GetpGRfm()->m_pst[ic1].m_Pln.v.z))// 矢切は無視
				continue;
			pgRfm = pRoofEn->GetpGRfm()->m_pst[ic1].m_Pg;		// ポリゴン　３Ｄ→２Ｄ
			
			if ( MGeo::ChkPt2OnPg2WS( pt1, pgRfm, &ist1)) {
				if ( imode == MC_PRI_MIN_AREA) {
					rArea = pgRfm.Area();
					if ( rArea < rMinArea) {
						rMinArea = rArea;
						pRfm = &(pRoofEn->GetpGRfm()->m_pst[ic1]);
					}
				} else {
					rHight = pRoofEn->GetpGRfm()->m_pst[ic1].m_Pln.GetZ( pt1);
					if ( imode == MC_PRI_MIN_HIGHT) {
						if ( rHight < rMinHight) {
							rMinHight = rHight;
							pRfm = &(pRoofEn->GetpGRfm()->m_pst[ic1]);
						}
					} else {
						if ( rHight > rMinHight) {
							rMinHight = rHight;
							pRfm = &(pRoofEn->GetpGRfm()->m_pst[ic1]);
						}
					}
				}
			}
		}
	}
	return pRfm;
}

} // namespace MC
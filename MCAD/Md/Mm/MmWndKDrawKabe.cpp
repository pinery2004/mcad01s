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
#include "MbList.h"
#include "MhLib.h"

#include "MhInp.h"

#include "MmOpt.h"
#include "MdLib.h"

#include "MmHist.h"
#include "MmValid.h"

namespace MC
{

/////////////////////////////////////////////////////////////////////////////
//	線分を表示する
void MmWndKDrawLine(
						MbCod*			pCod,		// 座標系
						MhPlcInfo		*pPlcEn,	// 配置レコード
						MPENSTYLE		LineType,	// 線種
						MSYSTEMCOLOR	iLineColor,	// 線色コード
						MgVect2			vSinZ		// 芯ずれ
				)
{
	MgPoint2	ptW[2];
	DWORD		PtsLineColor;

	ptW[0] = (*(MgPoint2*)&(pPlcEn->GetPIPlcIti( 0))) + vSinZ;
	ptW[1] = (*(MgPoint2*)&(pPlcEn->GetPIPlcIti( 1))) + vSinZ;

	PtsLineColor = MmSystemProperty::GetColor( iLineColor);
	pCod->SetLineAttr( LineType, 1, PtsLineColor);
	pCod->Line( MgLine2( ptW[0], ptW[1]));

}

/////////////////////////////////////////////////////////////////////////////
//	線分を表示する
void MmWndKDrawLine(
						MINT			iMode,		// モード
													//		0:配置線分を表示、
													//		1:配置線分または図形を表示
						MbCod*			pCod,		// 座標系
						MhPlcInfo		*pPlcEn,	// 配置レコード
						MPENSTYLE		LineType,	// 線種
						MSYSTEMCOLOR	iLineColor,	// 線色コード
						MgVect2			vSinZ		// 芯ずれ
				)
{
	MgPoint2	ptW[2];
	DWORD		PtsLineColor;
	MhZukei		*pZukei;
	MINT		ic;

	ptW[0] = (*(MgPoint2*)&(pPlcEn->GetPIPlcIti( 0))) + vSinZ;
	ptW[1] = (*(MgPoint2*)&(pPlcEn->GetPIPlcIti( 1))) + vSinZ;

	PtsLineColor = MmSystemProperty::GetColor( iLineColor);
	pCod->SetLineAttr( LineType, 1, PtsLineColor);

	pZukei = pPlcEn->GetPIZukei();
	if ( iMode == 0 || pZukei == NULL) {
		pCod->Line( MgLine2( ptW[0], ptW[1]));
	} else {
		for ( ic=0; ic<pZukei->m_nZukei; ic++) 
			pCod->Line( MgLine2C( pZukei->m_lnZukei[ic]));
	}
}

/////////////////////////////////////////////////////////////////////////////
//	壁芯線を表示する
void WindowCtrl::MmWndKDrawKabe(
						MbCod*		pCod,			// 座標系
						MINT		iKaiC,			// 階  	(1,2,3)
						MINT		iGpC			// 構成
				)
{
	MPOSITION	pos1;
	MhPlcInfo	*pPlcEn;
	MINT		iEnKai;
//	MsBitSet	*pOptv;
//	MsBitSet*	pHstv;
	MgPoint2	ptW[2];

	MREAL		rKabeSinZ = pCod->LPtoRP( MINT( pCod->DPtoLP(1)));
	MgVect2		vKabeSinZ = MgVect2( rKabeSinZ, rKabeSinZ) / 2.;

	// 壁線を表示する

	for ( pPlcEn = HaitiDb::MdGetHeadPts( &pos1); pPlcEn!=0;
		  pPlcEn = HaitiDb::MdGetNextPts( &pos1)) {

		iEnKai = pPlcEn->GetPIKai();

		// 壁(耐力壁)
		if ( pPlcEn->IsKabe()) {

			if ( !MmChkValidParts( pPlcEn))						// オプションと履歴のチェック
			continue;

			// 床組、壁枠組、耐力壁には、下階の壁芯線を表示する
			if ( iEnKai == iKaiC - 1) {							// 下階の壁
				if ( iGpC <= MP_GP_TAIRYOKU && iGpC > MP_GP_DODAI) 
					MmWndKDrawLine( pCod, pPlcEn, MPS_DASHDOT, MM_COLOR_KAKAIKABESIN, vKabeSinZ);

			// 耐力壁|壁枠組以外には、同階の壁芯線を表示する
			} else if ( iEnKai == iKaiC) {						// 同階の壁
				if ( iGpC != MP_GP_TAIRYOKU && iGpC != MP_GP_KABE)			// 耐力壁|壁枠組み以外は壁芯線を表示
					MmWndKDrawLine( pCod, pPlcEn, MPS_DASHDOT, MM_COLOR_DOUKAIKABESIN, vKabeSinZ);

			// 天井、小屋、屋根には、上階の壁芯線を表示する
			} else if ( iEnKai == iKaiC + 1) {					// 上階の壁
				if ( iGpC >= MP_GP_TENJO)						// 壁(耐力壁)部材の表示
					MmWndKDrawLine( pCod, pPlcEn, MPS_DASHDOT, MM_COLOR_JYOUKAIKABESIN, - vKabeSinZ);
			}

		// 耐力壁区画
		} else if ( pPlcEn->IsTaiKabeArea()) {

			if ( !MmChkValidParts( pPlcEn))						// オプションと履歴のチェック
			continue;

			// 床組、壁枠組、耐力壁には、下階の耐力壁区画を表示する
			if ( iEnKai == iKaiC - 1) {							// 下階の壁
				if ( iGpC <= MP_GP_TAIRYOKU && iGpC > MP_GP_DODAI) 
					MmWndKDrawLine( 1, pCod, pPlcEn, MPS_DASHDOT, MM_COLOR_KAKAITAIKABESIN, vKabeSinZ);

			// 耐力壁|壁枠組以外には、同階の耐力壁区画を表示する
			} else if ( iEnKai == iKaiC) {						// 同階の壁
				if ( iGpC != MP_GP_TAIRYOKU && iGpC != MP_GP_KABE)			// 耐力壁|壁枠組み以外は壁芯線を表示
					MmWndKDrawLine( 1, pCod, pPlcEn, MPS_DASHDOT, MM_COLOR_DOUKAITAIKABESIN, vKabeSinZ);

			// 天井、小屋、屋根には、上階の耐力壁区画を表示する
//			} else if ( iEnKai == iKaiC + 1) {					// 上階の壁
//				if ( iGpC >= MP_GP_TENJO)						// 壁(耐力壁)部材の表示
//					MmWndKDrawLine( pCod, pPlcEn, MPS_DASHDOT, MM_COLOR_JYOUKAITAIKABESIN, - vKabeSinZ);
			}

		// 耐力壁線
		} else if ( pPlcEn->IsTaiKabeLine()) {

			if ( !MmChkValidParts( pPlcEn))						// オプションと履歴のチェック
			continue;

			// 床組、壁枠組、耐力壁には、下階の耐力壁線を表示する
			if ( iEnKai == iKaiC - 1) {							// 下階の壁
				if ( iGpC <= MP_GP_TAIRYOKU && iGpC > MP_GP_DODAI) 
					MmWndKDrawLine( pCod, pPlcEn, MPS_DASHDOT, MM_COLOR_KAKAITAIKABESIN, vKabeSinZ);

			// 耐力壁|壁枠組以外には、同階の耐力壁線を表示する
			} else if ( iEnKai == iKaiC) {						// 同階の壁
				if ( iGpC != MP_GP_TAIRYOKU && iGpC != MP_GP_KABE)			// 耐力壁|壁枠組み以外は壁芯線を表示
					MmWndKDrawLine( pCod, pPlcEn, MPS_DASHDOT, MM_COLOR_DOUKAITAIKABESIN, vKabeSinZ);

			// 天井、小屋、屋根には、上階の耐力壁線を表示する
//			} else if ( iEnKai == iKaiC + 1) {					// 上階の壁
//				if ( iGpC >= MP_GP_TENJO)						// 壁(耐力壁)部材の表示
//					MmWndKDrawLine( pCod, pPlcEn, MPS_DASHDOT, MM_COLOR_JYOUKAITAIKABESIN, - vKabeSinZ);
			}

		// 支持壁線
		} else if ( pPlcEn->IsSijiKabeLine()) {

			if ( !MmChkValidParts( pPlcEn))						// オプションと履歴のチェック
			continue;

			// 床組、壁枠組、耐力壁には、下階の支持壁線を表示する
			if ( iEnKai == iKaiC - 1) {							// 下階の壁
				if ( iGpC <= MP_GP_TAIRYOKU && iGpC > MP_GP_DODAI) 
					MmWndKDrawLine( pCod, pPlcEn, MPS_DASHDOT, MM_COLOR_KAKAISIJIKABESIN, vKabeSinZ);

			// 耐力壁|壁枠組以外には、同階の支持壁線を表示する
			} else if ( iEnKai == iKaiC) {						// 同階の壁
				if ( iGpC != MP_GP_TAIRYOKU && iGpC != MP_GP_KABE)			// 耐力壁|壁枠組み以外は壁芯線を表示
					MmWndKDrawLine( pCod, pPlcEn, MPS_DASHDOT, MM_COLOR_DOUKAISIJIKABESIN, vKabeSinZ);

			// 天井、小屋、屋根には、上階の支持壁線を表示する
//			} else if ( iEnKai == iKaiC + 1) {					// 上階の壁
//				if ( iGpC >= MP_GP_TENJO)						// 壁(耐力壁)部材の表示
//					MmWndKDrawLine( pCod, pPlcEn, MPS_DASHDOT, MM_COLOR_JYOUKAISIJIKABESIN, - vKabeSinZ);
			}
		}
	}
}

} // namespace MC
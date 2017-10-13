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
#include "MmValid.h"

namespace MC
{

void MCmdLineDelete()
{
	MINT			ist1;
	MINT			irt;
	MgPoint2		pt1, pt2;
	MgPoint2		ptK[2];
	MgPoint2		ptW[2];
	MgLine2			Ln1;
	MgPolyg2		pg1(20);
	MINT			iIdTpPts, iIdTpPtsB;
	MINT			iKaiC;
	MINT			iGpC, iGpB;
	MhTpPts*		pTpPts;
	MhPlcInfo*		pPlcEn;
	MPOSITION		pos1;
	Window::CurWndFocus();

	Msg::ClearErrorMsg();
	Msg::OperationMsg( MC_OPRT_DEL_PARTS);							// ステイタスバーの操作表示部へ"部材追加"を表示

	iKaiC = InpMode::GetKai();
	iGpC = InpMode::GetKGp();
	iIdTpPts = InpAttr::GetCurIdTpPts();
	pTpPts = BuzaiCode::MhGetpTpPts( iIdTpPts);

	MFOREVER {
		Msg::GuidanceMsg( MC_GUID_DEL_PARTS);
		irt = MtAskForPoint( &pt1);								// １点目を入力
		if ( irt == MTRT_SYSTEMSTOP || irt == MTRT_CAN)			// システムストップまたはキャンセル
			break;
		if ( irt == MTRT_RBTNDWN || irt == MTRT_KWORD)			// マウス右ボタン || メニュー入力
			continue;

		HaitiCmd::MmPresetCmd();
		// 指示部材を検索する
		for ( pPlcEn = HaitiDb::MdGetHeadPts( &pos1); pPlcEn!=0;
			  pPlcEn = HaitiDb::MdGetNextPts( &pos1)) {

			if (pPlcEn->GetPIKai() != iKaiC)
				continue;										// 異なる階の部材は表示しない
			iGpB = pPlcEn->GetPITpPts()->GetPTCdGp();
			iIdTpPtsB = pPlcEn->GetPIIdTpPts();
			if ( iGpB != iGpC || iIdTpPtsB != iIdTpPts)
				continue;										// 異なる構成の部材は表示しない

			if ( !MmChkValidParts( pPlcEn))						// オプションと履歴のチェック
				continue;

			ptW[0] = *(MgPoint2*)&(pPlcEn->GetPIPlcIti( 0));
			ptW[1] = *(MgPoint2*)&(pPlcEn->GetPIPlcIti( 1));

			MgVect2	VtW = ptW[1] - ptW[0];
			MgVect2	VtUtW = MGeo::UnitizeV2( VtW);
			MgVect2	VtWidthR = (pPlcEn->GetMbWidthR() + pPlcEn->GetPISinZure()) * VtUtW.RotR90(); 
			MgVect2	VtWidth = pPlcEn->GetMbWidth() * VtUtW.RotL90(); 
			pg1.m_n = 0;
			pg1 += (ptW[0] - pPlcEn->GetPILenHosei( 0) * VtUtW + VtWidthR);
			pg1 += (ptW[1] + pPlcEn->GetPILenHosei( 1) * VtUtW + VtWidthR);
			pg1 += (pg1.m_p[1] + VtWidth);
			pg1 += (pg1.m_p[0] + VtWidth);

			MGeo::ChkPt2OnPg2WS( pt1, pg1, &ist1);
			if ( ist1 == MC_IN_BORDER) {
				if (HaitiIn::ChkPts( NULL, MP_GP_TAIRYOKU, Mstr( "壁"), NULL, pPlcEn))
					IeModel::MhKabeSetUpFlg();
				break;
			}
		}
		// 削除する
		if (pPlcEn != 0)
			HaitiDb::MdPtsDelete( pos1);

		IeModel::MhNormKabe( 0);

		WindowCtrl::MmWndKReDraw();
		Msg::ClearErrorMsg();
	}
	Msg::OperationMsg( MC_OPRT_BASE);								// ステイタスバーの操作表示部へ""を表示
}

void MCmdRoofDelete()
{
	MINT			irt;
	MgPoint2		pt1, pt2;
	MgPoint2		ptK[2];
	MgPoint2		ptW[2];
	MgPoint3		Py1;
	MgLine2			Ln1;
	MgPolyg2		pg1(20);
	MINT			ic1, ic2;
	MhRoofInfo*		pRoofEn;
	MPOSITION		pos1;
	Window::CurWndFocus();
	MgMinMaxR3		rMmMdl;
	MgMinMaxR2		rmmMdl;
	Msg::ClearErrorMsg();
	Msg::OperationMsg( MC_OPRT_DEL_ROOF);							// ステイタスバーの操作表示部へ"屋根削除"を表示

	MFOREVER {
		Msg::GuidanceMsg( MC_GUID_DEL_ROOF);
		irt = MtAskForPoint( &pt1);								// １点目を入力
		if ( irt == MTRT_SYSTEMSTOP || irt == MTRT_CAN)			// システムストップまたはキャンセル
			break;
		if ( irt == MTRT_RBTNDWN || irt == MTRT_KWORD)			// マウス右ボタン || メニュー入力
			continue;

		HaitiCmd::MmPresetCmd();
		// 屋根を検索する
		for ( pRoofEn = HaitiDb::MdGetHeadRoof( &pos1); pRoofEn!=0;
			  pRoofEn = HaitiDb::MdGetNextRoof( &pos1)) {
			if ( !MmChkValidRoof( pRoofEn))						// オプションと履歴のチェック
				continue;
			// 屋根のMINMAXを求める
			rMmMdl.SetInit();
			for ( ic1=0; ic1<pRoofEn->GetpGRfm()->m_n; ic1++) {
				for ( ic2=0; ic2<pRoofEn->GetpGRfm()->m_pst[ic1].m_Pg.m_n; ic2++) {
					Py1 = pRoofEn->GetpGRfm()->m_pst[ic1].m_Pg.m_P[ic2];
					rMmMdl.Ins3( Py1);
				}
			}
			rmmMdl = MgMinMaxR2( MgPoint2C( rMmMdl.min), MgPoint2C( rMmMdl.max));
			if ( MGeo::ChkPt2InMMR2( pt1, rmmMdl))
				break;
		}

		// 削除する
		if (pRoofEn != 0)
			HaitiDb::MdRoofDelete( pos1);

		WindowCtrl::MmWndKReDraw();
		Msg::ClearErrorMsg();
	}
	Msg::OperationMsg( MC_OPRT_BASE);								// ステイタスバーの操作表示部へ""を表示
}

} // namespace MC
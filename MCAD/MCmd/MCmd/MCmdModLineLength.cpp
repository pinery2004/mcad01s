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
//	横置部材の長さ調整
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
	Msg::OperationMsg( MC_OPRT_MOD_BUZAI);							// ステイタスバーの操作表示部へ"部材修正"を表示

	InpAttr::InitComboAttr( MP_AT_ADJLNG);							// 属性入力用コンボボックスを部材長さ調整用にする

	MmWndInfo* pWndInfo = WindowCtrl::MmWndKGetCurWnd();			// カレントウィンドウを取得する
	Window::CurWndFocus();											// フォーカスをカレントウィンドウに設定する

	MFOREVER {
		Window::SetDispSelPts( NULL);

		Msg::GuidanceMsg( MC_GUID_MOD_BUZAILENGTH1);				// "部材の長さ調整側の端部を指示して下さい"
		irt = MtAskForPoint( &pt1);

		if ( irt == MTRT_SYSTEMSTOP || irt == MTRT_CAN)
			break;

		iBuzaiCd = NULL;
		if ( pBziInfo1 = HaitiIn::SrchBuzai( pWndInfo, pt1, iBuzaiCd, &pgHitBzi)) {
			Window::SetCurBziFig( &pgHitBzi);						// カレント選択(長さ調整側)の部材(長さ調整側)形状を設定する
			WindowCtrl::MmWndKReDraw();
		} else {
			Window::ClrCurBziFig();									// カレント選択の部材形状をクリアする
			continue;
		}

		Window::SetDispSelPts( NULL);

		Msg::GuidanceMsg( MC_GUID_MOD_BUZAILENGTH2);				// "部材の長さ調整先を指示して下さい"
		irt = MtAskForPoint( &pt2);

		if ( irt == MTRT_SYSTEMSTOP || irt == MTRT_CAN)
			break;

		iBuzaiCd = NULL;
		pBziInfo2 = HaitiIn::SrchBuzai( pWndInfo, pt2, iBuzaiCd, &pgHitBzi);
		if ( pBziInfo1 == pBziInfo2)								// 長さ調整側部材と長さ調整先部材と同一部材を選択の場合は座標指示とみなす
			pBziInfo2 = NULL;

		Pt1 = MgPoint3C( pt1);

		Window::DrawHitBzi( pWndInfo, &pgHitBzi);					//	ヒット部材（長さ調整先部材）を表示する
																	//	（既表示ヒット部材を消去後に、ヒット部材を表示する）
		ist1 = InpAttr::GetCheckAttr( MC_CHK_KATI, &iKati);			// 勝ち負けチェックボックスの勝ちマークを取得する(1:勝ち,0:負け)
		ist1 = InpAttr::GetCheckAttr( MC_CHK_INTC, &iCrsC);			// 交差部材調整チェックボックスの調整マークを取得する(1:あり,0:なし)

		HaitiIn::MmSrchCrossBuzai( pWndInfo, pBziInfo1, iBuzaiCd, pbTchBziI, pbCrsBziI);
																	// 長さ調整側部材と調整前に触れている部材群と交差している部材群を求める
		if ( pBziInfo2) {
			ist1 = HaitiIn::MhAdjBzL( iKati, pBziInfo1, Pt1, pBziInfo2);	// 長さ調整
		} else {
//S			pWndInfo->Marume( pt2, &pt2m);
			Input::Marume( pt2, &pt2m);
			Pt2m = MgPoint3C( pt2m);
			HaitiIn::MhAdjBzL( pBziInfo1, Pt1, Pt2m);
		}

		if ( ist1 == MC_PARALLEL) {
			Pt2 = MgPoint3C( pt2);
			HaitiIn::MhAdjBzL( pBziInfo1, Pt1, pBziInfo2, Pt2);		// 調整側部材と調整先部材が平行時の調整
		}
			
		HaitiIn::MmSrchCrossBuzai( pWndInfo, pBziInfo1, iBuzaiCd, pbTchBziO, pbCrsBziO);
																	// 長さ調整側部材と調整後に触れている部材群と交差している部材群を求める
			
		HaitiIn::MhChngCrossBuzai( pbTchBziI, pbCrsBziI, pbTchBziO, pbCrsBziO, 
								   pbFRtoTCBzi, pbTCtoFRBzi, pbTCtoTCBzi);
		if ( iCrsC == 1) {
			MgLine3 	LnBzi;
			MgPoint3	Pt0;
			// 離れている → 接触　または　交差
				// 長さ調整　または　分割
			for ( ic1=0; pbFRtoTCBzi[ic1]; ic1++) {
				LnBzi = pbFRtoTCBzi[ic1]->GetPIPlcIti();
				Pt0 = (LnBzi.p[0] + LnBzi.p[1]) / 2.;
				HaitiIn::MhAdjBzL( 0, pbFRtoTCBzi[ic1], Pt0, pBziInfo1);
			}
			// 接触　または　交差 → 離れている
/*
			for ( ic1=0; pbTCtoFRBzi[ic1]; ic1++) {
				// 連結　または　無視
				LnBzi = pbTCtoFRBzi[ic1]->GetPIPlcIti();
				Pt0 = (LnBzi.p[0] + LnBzi.p[1]) / 2.;
				MhAdjBzL( 0, pbTCtoFRBzi[ic1], Pt0, pBziInfo1);
			}
*/
			// 接触　または　交差 → 接触　または　交差
				// 長さ調整　または　分割
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
	Msg::OperationMsg( MC_OPRT_BASE);								// ステイタスバーの操作表示部へ""を表示
}

} // namespace MC
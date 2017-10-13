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
#include "MmDrag.h"
#define DLL_EXPORT_GRIDNUM_DO
#include "MmGridNum.h"
#define DLL_EXPORT_MC_WINDOWCTRL_DO
#include "MmWnd.h"
#include "MmDefine.h"
#include "MmLib.h"
#include "MmSystemProperty.h"

namespace MC
{

//static CMmDialogGrid*	z_pDlgGrid = NULL;						// グリッドダイアログ
static MINT				z_nGridI[4] = { 16, -3, -3, 17};

/*
/////////////////////////////////////////////////////////////////////////////
//	グリッド数変更入力
void WindowCtrl::MmGridNumInp		(
						CWnd*		pWnd						// (I  ) ウィンドウのインスタンス
				)
{
	if ( z_pDlgGrid) {
		z_pDlgGrid->BringWindowToTop();
	} else {
		z_pDlgGrid = new CMmDialogGrid( pWnd);
	}
}
*/
/////////////////////////////////////////////////////////////////////////////
//	グリッド数設定	（シングルグリッド用）
void WindowCtrl::MmGridNumSet(
						MINT*		nGrid						// (I  ) グリッド数（北、西、南、東）または NULL(初期設定)
				)
{
	MmWndInfo*	pWndInfo;
	MINT*		pnGrid;
//	MINT		nGridI[4] = { 16, -3, -3, 17};
	MINT		ic1;

	pWndInfo = WindowCtrl::MmWndKGetCurWnd();							// カレントウィンドウ取得
	if ( pWndInfo == NULL)
		MQUIT;

	if ( nGrid == NULL)
		pnGrid = z_nGridI;										// グリッド数初期化
	else {
		pnGrid = nGrid;											// 指定のグリッド数に設定
		for ( ic1=0; ic1<4; ic1++)
			z_nGridI[ic1] = nGrid[ic1];
	}

//	if ( z_pDlgGrid != NULL) z_pDlgGrid->SetSpinGridNum( pnGrid);
	pWndInfo->SetGridNum( pnGrid);
exit:;
}

/////////////////////////////////////////////////////////////////////////////
//	グリッド数読み込み	（シングルグリッド用）
void WindowCtrl::MmGridNumGet(
						MINT*		nGrid						// (I  ) グリッド数（北、西、南、東）
				)
{
	MmWndInfo*	pWndInfo;

	pWndInfo = WindowCtrl::MmWndKGetCurWnd();							// カレントウィンドウ取得
	if ( pWndInfo == NULL) MQUIT;

	pWndInfo->GetGridNum( nGrid);
exit:;
}

/////////////////////////////////////////////////////////////////////////////
//	グリッド数変更表示実行
void WindowCtrl::MmGridNumXqt()
{
	MmWndInfo*	pWndInfo;

	pWndInfo = WindowCtrl::MmWndKGetCurWnd();							// カレントウィンドウ取得
	if ( pWndInfo == NULL)
		MQUIT;

	pWndInfo->SetMinMaxRS( pWndInfo->GetMinMaxGA( pWndInfo));
	pWndInfo->SetMat();
	
	WindowCtrl::MmWndKDrawMDC( pWndInfo);

	pWndInfo->GetWnd()->PostMessage(WM_PAINT);

exit:;
//	Window::CurWndFocus();										// 住棟展開で住戸選択で2度の選択毎に1度変更となりコメント化
}
/*
/////////////////////////////////////////////////////////////////////////////
//	グリッド数変更キャンセル
void WindowCtrl::MmGridNumCancel()
{
	if ( z_pDlgGrid)
//		z_pDlgGrid->SendMessage(WM_CLOSE);
		z_pDlgGrid->PostMessage(WM_CLOSE);
}

/////////////////////////////////////////////////////////////////////////////
//	グリッド数変更終了
void WindowCtrl::MmGridNumEnd()
{
	z_pDlgGrid = NULL;
}
*/
/////////////////////////////////////////////////////////////////////////////
// コンストラクタ
MmGridNum::MmGridNum()
{
	static MINT nGrid[4];
	MgMinMaxR2	rMinMaxG;

	nGrid[0] = MmSystemProperty::GetInt( MM_INT_GRID_N);
	nGrid[1] = MmSystemProperty::GetInt( MM_INT_GRID_W);
	nGrid[2] = MmSystemProperty::GetInt( MM_INT_GRID_S);
	nGrid[3] = MmSystemProperty::GetInt( MM_INT_GRID_E);

	m_bFlgDrawG = TRUE;
	m_PtOriginG = MgPoint2( 0., 0.);
	m_PichG[0][0] = m_PichG[1][0] = MmSystemProperty::GetReal( MM_REAL_PITCH);
	
	SetGridNum( nGrid);
}
/*
// マルチグリッド用

/////////////////////////////////////////////////////////////////////////////
//	縦横のグリッド数の設定
void MmGridNum::SetNumG( MINT nGrid[2][MGRID_GRP_MAX])
{
	MINT	ic1, ic2;
	
	for ( ic1=0; ic1<2; ic1++)
		for ( ic2=0; ic2<m_nGrpG[0]; ic2++)
			m_nGrid[ic1][ic2] = nGrid[ic1][ic2];
}

/////////////////////////////////////////////////////////////////////////////
//	縦横のグリッド数を取得する
void MmGridNum::GetNumG( MINT nGrid[2][MGRID_GRP_MAX])
{
	MINT	ic1, ic2;
	
	for ( ic1=0; ic1<2; ic1++)
		for ( ic2=0; ic2<m_nGrpG[0]; ic2++)
			nGrid[ic1][ic2] = m_nGrid[ic1][ic2];
}

/////////////////////////////////////////////////////////////////////////////
//	縦横のグリッド間隔の設定
void MmGridNum::SetPichG( MREAL PichG[2][MGRID_GRP_MAX])
{
	MINT	ic1, ic2;
	
	for ( ic1=0; ic1<2; ic1++)
		for ( ic2=0; ic2<m_nGrpG[0]; ic2++)
			m_PichG[ic1][ic2] = PichG[ic1][ic2];
}

/////////////////////////////////////////////////////////////////////////////
//	縦横のグリッド間隔を取得する
void MmGridNum::GetPichG( MREAL PichG[2][MGRID_GRP_MAX])
{
	MINT	ic1, ic2;
	
	for ( ic1=0; ic1<2; ic1++)
		for ( ic2=0; ic2<m_nGrpG[0]; ic2++)
			PichG[ic1][ic2] = m_PichG[ic1][ic2];
}
*/
/////////////////////////////////////////////////////////////////////////////
//	グリッド数設定	（シングルグリッド用）
void MmGridNum::SetGridNum(
						MINT*		i_nGrid		// グリッド数（北、西、南、東）
				)
{
	m_nGrpG[0] = m_nGrpG[1] = 1;

	m_iMinG[0] = i_nGrid[1];
	m_nGrid[0][0] = i_nGrid[3] - i_nGrid[1];

	m_rMinMaxG.min.x = m_PichG[0][0] * m_iMinG[0] + m_PtOriginG.x; 
	m_rMinMaxG.max.x = m_rMinMaxG.min.x + m_PichG[0][0] * m_nGrid[0][0]; 

	m_iMinG[1] = i_nGrid[2];
	m_nGrid[1][0] = i_nGrid[0] - i_nGrid[2];

	m_rMinMaxG.min.y = m_PichG[1][0] * m_iMinG[1] + m_PtOriginG.y; 
	m_rMinMaxG.max.y = m_rMinMaxG.min.y + m_PichG[1][0] * m_nGrid[1][0];
}

/////////////////////////////////////////////////////////////////////////////
//	グリッド数読み込み	（シングルグリッド用）
void MmGridNum::GetGridNum(
						MINT*	o_nGrid		// グリッド数（北、西、南、東）
				)
{

	o_nGrid[1] = m_iMinG[0];
	o_nGrid[3] = m_iMinG[0] + m_nGrid[0][0];

	o_nGrid[2] = m_iMinG[1];
	o_nGrid[0] = m_iMinG[1] + m_nGrid[1][0];
}

/////////////////////////////////////////////////////////////////////////////
//	グリッド表示画面MINMAX算出	（シングルグリッド用）
//	グリッドMinMaxの領域に周囲の空きを加えた領域を取得する
MgMinMaxR2 MmGridNum::GetMinMaxGA(
						MbCod*		pCod
				)
{
	MgMinMaxR2	rMinMaxG = MgMinMaxR2(0., 0., 1., 1.);

	MREAL		GridHeight;
	MREAL		GridAki;
	MREAL		rHosei;

	MINT		iStX;												// ウィンドウの始点Ｘ座標
	MINT		iStY;												// ウィンドウの始点Ｙ座標
	MINT		iWidth;												// ウィンドウの幅
	MINT		iHeight;											// ウィンドウの高さ

	pCod->GetWinL( &iStX, &iStY, &iWidth, &iHeight);

	MgVect2		Haba;
	MgMinMaxR2	Waku;

	GridHeight	= MmSystemProperty::GetReal( MM_REAL_GRID_HEI) * 20.f;	// グリッド文字高さ
	GridAki		= MmSystemProperty::GetReal( MM_REAL_GRID_AKI);			// グリッド枠表示空き
	rHosei		= MGMAX( m_nGrid[0][0] / (MGeo::Abs( iWidth) / 1300.f),
						 m_nGrid[1][0] / (MGeo::Abs( iHeight) / 1170.f));
	GridHeight	*= rHosei;											// グリッド文字高さ
	GridAki		*= rHosei;											// グリッド枠表示空き

	rMinMaxG = GetMinMaxG();

	Haba = rMinMaxG.max - rMinMaxG.min;
	Waku.min.x = - ((GridAki + GridHeight));		// グリッド空きと文字高さ分をグリッド数で補正
	Waku.min.y = - ((GridAki + GridHeight));		// グリッド空きと文字高さ分をグリッド数で補正
	Waku.max.x = GridAki;
	Waku.max.y = GridAki;

	rMinMaxG.min += Waku.min;
	rMinMaxG.max += Waku.max;

	return rMinMaxG;
}

/////////////////////////////////////////////////////////////////////////////
//	グリッドを表示する
void MmGridNum::DrawGrid(
						MmWndInfo*	pWndInfo,
						MbCod*		pCod
				)
{
	MREAL		GridSpace;
	MREAL		GridHeight;
	MREAL		GridAki;

	MgVect2		Haba;
	MgVect2		vOffset;

	MgLine2		LnGrid;
	MgPoint2	PtGridN;
	MINT		ic1, ic2;
	MINT		ign;
	DWORD		GridLineColor;
	DWORD		GridTextColor;
	MCHAR		SGN[5];

	MREAL		rOnePt = pCod->LPtoRP( MINT( pCod->DPtoLP( 1))); 

	MgMinMaxR2	rMinMaxG = pWndInfo->GetMinMaxG();					// グリッド表示領域取得
	
	MgMinMaxI2	rMinMaxGA = pCod->GetMinMaxL();						// 論理座標MinMax取得
	MgPoint2	ptGAMin = pCod->LPtoRP( rMinMaxGA.min);				// 表示領域の最小実座標を求める

	GridSpace	= MmSystemProperty::GetReal( MM_REAL_GRID_SPC);			// グリッド番号表示空き
	GridHeight	= MmSystemProperty::GetReal( MM_REAL_GRID_HEI) * 20.f;	// グリッド文字高さ
	GridAki		= MmSystemProperty::GetReal( MM_REAL_GRID_AKI);			// グリッド枠表示空き

	Haba = rMinMaxG.max - rMinMaxG.min;
	vOffset.x = pCod->LPtoRP( MINT( GridAki + GridHeight)) * 1.53f;	// グリッド空きと文字高さ分を補正
	vOffset.y = pCod->LPtoRP( MINT( GridAki + GridHeight)) * 1.28f;	// グリッド空きと文字高さ分を補正

	GridLineColor = MmSystemProperty::GetColor( MM_COLOR_GRID_LINE);
	GridTextColor = MmSystemProperty::GetColor( MM_COLOR_GRID_TEXT);

	// 縦のグリッド線を表示	

	LnGrid.p[0].y = m_rMinMaxG.min.y;
	LnGrid.p[1].y = m_rMinMaxG.max.y;

	if ( MGeo::GE( LnGrid.p[0].y, ptGAMin.y + vOffset.y)) {
		// グリッド番号表示空きありの場合は、最小グリッド値より下（-GridSpace）にグリッド番号を表示
		PtGridN.y = LnGrid.p[0].y;
		pCod->SetTextAttr( Mstr( "ＭＳ ゴシック"), 10.f, MT_UPPER, MT_CENTER, GridTextColor,
							   MgVect2( 1., 0.), MgVect2( 0., - GridSpace), MT_FIXEDSIZE);
	} else {
		// グリッド番号表示空きなしの場合は、表示エリアの下端より上（GridSpace）にグリッド番号を表示
		PtGridN.y = ptGAMin.y;
		pCod->SetTextAttr( Mstr( "ＭＳ ゴシック"), 10.f, MT_LOWER, MT_CENTER, GridTextColor,
							   MgVect2( 1., 0.), MgVect2( 0., GridSpace), MT_FIXEDSIZE);
	}

	ign = m_iMinG[0];
	LnGrid.p[0].x = LnGrid.p[1].x = m_rMinMaxG.min.x;
	for ( ic1=0; ic1<m_nGrpG[0]; ic1++) {
		for ( ic2=0; ic2<m_nGrid[0][ic1]; ic2++) {
			if ( ic2==0 || (( ic2 + m_iMinG[0]) % 5) == 0) {
				pCod->SetLineAttr( MPS_SOLID, 1, GridLineColor, MT_FIXEDSIZE);
			} else {
				pCod->SetLineAttr( MPS_DOT, 1, GridLineColor, MT_FIXEDSIZE);
			}
			pCod->Line( LnGrid);
			PtGridN.x = LnGrid.p[0].x;
			Msprintf( SGN, Mstr( "%d"), ign++);
			pCod->Text( PtGridN, SGN);

			LnGrid.p[0].x = LnGrid.p[1].x = LnGrid.p[0].x + m_PichG[0][ic1];
		}
	}
	pCod->SetLineAttr( MPS_SOLID, 1, GridLineColor);
	pCod->Line( LnGrid);

	PtGridN.x = LnGrid.p[0].x;
	Msprintf( SGN, Mstr( "%d"), ign++);
	pCod->Text( PtGridN, SGN);

	// 横のグリッド線を表示	

	LnGrid.p[0].x = m_rMinMaxG.min.x;
	LnGrid.p[1].x = m_rMinMaxG.max.x;

	if ( MGeo::GE( LnGrid.p[0].x, ptGAMin.x + vOffset.x)) {
		// グリッド番号表示空きありの場合は、最小グリッド値より左（-GridSpace）にグリッド番号を表示
		PtGridN.x = LnGrid.p[0].x;
		pCod->SetTextAttr( Mstr( "ＭＳ ゴシック"), 10.f, MT_CENTER, MT_RIGHT, GridTextColor,
							   MgVect2( 1., 0.), MgVect2( - GridSpace, 0.), MT_FIXEDSIZE);
	} else {
		// グリッド番号表示空きなしの場合は、表示エリアの左端より右（GridSpace）にグリッド番号を表示
		PtGridN.x = ptGAMin.x;
		pCod->SetTextAttr( Mstr( "ＭＳ ゴシック"), 10.f, MT_CENTER, MT_LEFT, GridTextColor,
							   MgVect2( 1., 0.), MgVect2( GridSpace, 0.), MT_FIXEDSIZE);
	}

	ign = m_iMinG[1];
	LnGrid.p[0].y = LnGrid.p[1].y = m_rMinMaxG.min.y;
	for ( ic1=0; ic1<m_nGrpG[1]; ic1++) {
		for ( ic2=0; ic2<m_nGrid[1][ic1]; ic2++) {
			if ( ic2==0 || (( ic2 + m_iMinG[1]) % 5) == 0) {
				pCod->SetLineAttr( MPS_SOLID, 1, GridLineColor, MT_FIXEDSIZE);
			} else {
				pCod->SetLineAttr( MPS_DOT, 1, GridLineColor, MT_FIXEDSIZE);
			}
			pCod->Line( LnGrid);

			PtGridN.y = LnGrid.p[0].y;
			Msprintf( SGN, Mstr( "%2d"), ign++);
			pCod->Text( PtGridN, SGN);

			LnGrid.p[0].y = LnGrid.p[1].y = LnGrid.p[0].y + m_PichG[1][ic1];
		}
	}
	pCod->SetLineAttr( MPS_SOLID, 1, GridLineColor);
	pCod->Line( LnGrid);

	PtGridN.y = LnGrid.p[0].y;
	Msprintf( SGN, Mstr( "%2d"), ign++);
	pCod->Text( PtGridN, SGN);
}

/////////////////////////////////////////////////////////////////////////////////
//////	丸め
////void MmGridNum::Marume(
////				const	MgPoint2	&Pi,
////						MgPoint2*	pPo
////				)
////{
////	MINT		ist;
////	MgVect2		vtDis;
////	MREAL		rDis_2;
////	MINT		ic1, ic2;
////
////	MREAL		rDisMin_2;
////	MgPoint2	Po, P0;
////
////	MINT		iCdPlc;										// 配置コード
////	MINT		iCdMarume;										// グリッド丸め分割値
////	MgPoint2	PtGrid;
////	MgLine2		Lnm[2];
////	MGGLINE2( GLnk, 10);
////
////	MINT iKai = InpMode::GetKai();
////
////	iCdMarume = InpMode::GetComboCdMarume();
////	iCdPlc = HaitiIn::GetComboCdPlc();
////
////	if ( iCdPlc == MP_HAICD_KABESINZUKE ||
////		iCdPlc == MP_HAICD_YANEKOUSEISENZUKE) {									// 壁芯付け丸め　または　屋根構成線付け丸め
////		// 付近の丸め対象線分を求める
////		if ( iCdPlc == MP_HAICD_KABESINZUKE) {									// 壁芯付け丸めの場合は、最寄の壁芯を求める
////			Input::GetMarumeKabeLine( iKai, Pi, &GLnk);
////		} else if ( iCdPlc == MP_HAICD_YANEKOUSEISENZUKE) {						// 屋根構成線付け丸めの場合は、最寄の屋根構成線を求める
////			Input::MmGetMarumeYaneLine( iKai, Pi, &GLnk);
////		}
////		// 複数本の丸め対象線分があった場合は交点が近くにあるならその交点を返す
////		rDisMin_2 = MREALMAX;
////		if ( GLnk.m_n >= 2) {
////			for ( ic1=0; ic1<GLnk.m_n - 1; ic1++) {
////				for ( ic2=ic1+1; ic2<GLnk.m_n; ic2++) {
////					ist = MGeo::Intr2Ln2( GLnk.m_ln[ic1], GLnk.m_ln[ic2], &P0);
////					if ( MF_CHECK_OR( ist, (MC_INT | MC_CONNECTION))) {
////						vtDis = P0 - Pi;
////						rDis_2 =MGeo::Abs_2( vtDis);
////						if ( rDis_2 < rDisMin_2) {
////							rDisMin_2 = rDis_2;
////							Po = P0;
////						}
////					}
////				}
////			}
////		}
////		if ( rDisMin_2 < MC_MARUME_TOL * MC_MARUME_TOL)
////			MQUIT;
////	} else {
////		GLnk.m_n = 0;
////	}
////				
//////	iCdMarume = InpMode::GetComboCdMarume();
////
////	// グリッド丸めなしの場合
////	if ( iCdMarume == 0) {
////		if ( GLnk.m_n == 0) {
////			// 壁芯での丸めなし(または候補の壁がなし)の場合は、入力位置をそのまま返す
////			*pPo = Pi;
////			return;
////		} else {
////			// 壁芯丸めで壁との重なりありの場合は、最も近い壁芯上の位置を返す
////			rDisMin_2 = MREALMAX;
////			for ( ic1=0; ic1<GLnk.m_n; ic1++) {
////				MGeo::PerpPtLn2( Pi, GLnk.m_ln[ic1], &P0);
////				vtDis = P0 - Pi;
////				rDis_2 =MGeo::Abs_2( vtDis);
////				if ( rDis_2 < rDisMin_2) {
////					rDisMin_2 = rDis_2;
////					Po = P0;
////				}
////			}
////	        MQUIT;
////		}
////	}
////
////	// グリッド丸めありの処理
////	GetMarumeGridLine( iCdMarume, Pi, Lnm);
////	if ( GLnk.m_n == 0) {
////		// 壁芯での丸めなし(または候補の壁なし)の場合は、グリッド丸め位置をそのまま返す
////		ist = MGeo::Intr2Ln2( Lnm[0], Lnm[1], pPo);
////		if ( !MF_CHECK_OR( ist, (MC_INT | MC_CONNECTION))) {
////			*pPo = Pi;
////		}
////		return;
////	} else {
////		// 壁芯丸めで壁との重なりありの場合は、壁芯とグリッド線との交点で最も近い位置を返す
////		rDisMin_2 = MREALMAX;
////		for ( ic1=0; ic1<GLnk.m_n; ic1++) {
////			for ( ic2=0; ic2<2; ic2++) {
////				ist = MGeo::Intr2Ln2( GLnk.m_ln[ic1], Lnm[ic2], &P0);
////				if ( MF_CHECK_OR( ist, (MC_INT | MC_CONNECTION))) {
////					vtDis = P0 - Pi;
////					rDis_2 = MGeo::Abs_2( vtDis);
////					if ( rDis_2 < rDisMin_2) {
////						rDisMin_2 = rDis_2;
////						Po = P0;
////					}
////				}
////			}
////		}
////		MQUIT;
////	}
////exit:
////	*pPo = Po;
////}

/////////////////////////////////////////////////////////////////////////////
//	グリッド丸め線を取得する
void MmGridNum::GetMarumeGridLine(
						MINT		iCdMarume,
				const	MgPoint2	&Pi,
						MgLine2		*pLn
				)
{
	MgPoint2	PtGrid;
	MREAL		rMarumeGridP;
	MREAL		rDistMrmGrd, rDist;
	MINT		ic1, ic2, ic3;
	MgPoint2	Po;

	// 丸め有りの場合は、入力位置を丸めて返す
	// 縦のグリッド丸め	
	rDistMrmGrd = MREALMAX;
	PtGrid.x = m_rMinMaxG.min.x;
	for ( ic1=0; ic1<m_nGrpG[0]; ic1++) {
		rMarumeGridP = m_PichG[0][ic1] / iCdMarume;
		for ( ic2=0; ic2<m_nGrid[0][ic1]; ic2++) {
			for ( ic3=0; ic3<iCdMarume; ic3++) {
				rDist = MGABS( Pi.x - PtGrid.x);
				if ( rDist < rDistMrmGrd) {
					rDistMrmGrd = rDist;
					Po.x = PtGrid.x;
				}
				PtGrid.x += rMarumeGridP;
			}
		}
	}
	rDist = MGABS( Pi.x - PtGrid.x);
	if ( rDist < rDistMrmGrd) {
		rDistMrmGrd = rDist;
		Po.x = PtGrid.x;
	}
	pLn[0] = MgLine2( MgPoint2( Po.x, m_rMinMaxG.min.y), MgPoint2( Po.x, m_rMinMaxG.max.y));

	// 横のグリッド丸め	
	rDistMrmGrd = MREALMAX;
	PtGrid.y = m_rMinMaxG.min.y;
	for ( ic1=0; ic1<m_nGrpG[1]; ic1++) {
		rMarumeGridP = m_PichG[1][ic1] / iCdMarume;
		for ( ic2=0; ic2<m_nGrid[1][ic1]; ic2++) {
			for ( ic3=0; ic3<iCdMarume; ic3++) {
				rDist = MGABS( Pi.y - PtGrid.y);
				if ( rDist < rDistMrmGrd) {
					rDistMrmGrd = rDist;
					Po.y = PtGrid.y;
				}
				PtGrid.y += rMarumeGridP;
			}
		}
	}
	rDist = MGABS( Pi.y - PtGrid.y);
	if ( rDist < rDistMrmGrd) {
		rDistMrmGrd = rDist;
		Po.y = PtGrid.y;
	}
	pLn[1] = MgLine2( MgPoint2( m_rMinMaxG.min.x, Po.y), MgPoint2( m_rMinMaxG.max.x, Po.y));
}

} // namespace MC
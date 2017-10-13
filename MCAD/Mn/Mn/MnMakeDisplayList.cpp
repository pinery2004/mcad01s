//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MsDispModel.cpp
//
//		家モデルのディスプレイリスト作成
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"

#include "MgLib.h"
#include "MmDefine.h"
#include "MmSystemProperty.h"

#include "MsBitSet.h"
#include "MbList.h"
#include "MhLib.h"

#include "MgGrp.h"
#include "MhRoof.h"
#include "MdLib.h"

#include "MhInp.h"

#include "MmOpt.h"
#include "MmHist.h"

#include "MdFig.h"

#include "MdDispList.h"
#include "MmValid.h"

#include "MnLib.h"

#define DLL_EXPORT_MDLDISPLIST_DO
#include "MnMdlDispList.h"

#define		MP_PANEL_LINE			1
#define		MP_PANEL_RECTANGULAR	2

namespace MC
{

//////////////////////////////////////////////////////////////////////////////
////	【機能】家モデルのMINMAXを取得する
////

////////////////////////////////////////////////////////////////////////////
//	【機能】建具の穴の多角形を求める
//			
	void MdlDispList::TateguHole(
						MhPlcInfo	*i_pPlcEn,		// 壁またはNULL
						MhPlcInfo	*pPlcTEn,		// 建具
						MgPolyg3	*PgR,			// 壁右面開口
						MgPolyg3	*PgL			// 壁左面開口
				)
{
	MGPOLYG3( PgW, 4);

	MhTateguInfo*	pAuxTategu;

	MREAL		rZU, rZL;

	MgVect2		ptW[2];
	MgLine2		lnK[2];
	MgLine3		LnK[2];
	MgLine3		LnKL[2];
	MgLine3		LnKR[2];
	MgVect2		vtW;
	MgVect2		vuW;
	MgVect2		vtWidthR;
	MgVect2		vtWidth;
	MINT		iRev;
	MgVect3		V1, V2;
	MREAL		cc;

	// 開口の上端高さと下端高さを求める
	pAuxTategu = (MhTateguInfo *)pPlcTEn->GetPIAuxTategu();
	rZU = MmSystemProperty::GetStnd( pPlcTEn->GetPIKai(), MM_STNDH_LOWER) + pAuxTategu->GetHeight();
	rZL = rZU - pAuxTategu->GetROH();

	ptW[0] = *(MgPoint2*)&(pPlcTEn->GetPIPlcIti( 0));
	ptW[1] = *(MgPoint2*)&(pPlcTEn->GetPIPlcIti( 1));
	vtW = ptW[1] - ptW[0];
	vuW = MGeo::UnitizeV2( vtW);
	vtWidthR = pPlcTEn->GetMbWidthR() * vuW.RotR90(); 
	vtWidth = pPlcTEn->GetMbWidth() * vuW.RotL90(); 

	iRev = 0;
	if ( i_pPlcEn != NULL) {
		V1 = i_pPlcEn->GetPIPlcIti( 1) - i_pPlcEn->GetPIPlcIti( 0);
		V2 = pPlcTEn->GetPIPlcIti( 1) - pPlcTEn->GetPIPlcIti( 0);
		cc = V1 * V2;												// cc > 0: 壁と建具が同一方向
		if (cc < 0)
			iRev = 1;
	}
	if ( iRev == 0) {
		lnK[0].p[0] = ptW[0] - pPlcTEn->GetPILenHosei( 0) * vuW + vtWidthR;		// 右側２次元線分始点
		lnK[0].p[1] = ptW[1] + pPlcTEn->GetPILenHosei( 1) * vuW + vtWidthR;		// 右側２次元線分終点
		lnK[1] = lnK[0] + vtWidth;														// 左側２次元線分
	} else {
		vtWidthR = -vtWidthR;
		vtWidth = - vtWidth;

		lnK[0].p[0] = ptW[1] + pPlcTEn->GetPILenHosei( 1) * vuW + vtWidthR;		// 右側２次元線分始点
		lnK[0].p[1] = ptW[0] - pPlcTEn->GetPILenHosei( 0) * vuW + vtWidthR;		// 右側２次元線分終点
		lnK[1] = lnK[0] + vtWidth;														// 左側２次元線分
	}

	// 右側開口多角形を求める
	PgR->m_n = 4;
	PgR->m_P[3] = MgPoint3C( lnK[0].p[1], rZL);
	PgR->m_P[2] = MgPoint3C( lnK[0].p[1], rZU);
	PgR->m_P[1] = MgPoint3C( lnK[0].p[0], rZU);
	PgR->m_P[0] = MgPoint3C( lnK[0].p[0], rZL);

	// 左側開口多角形を求める
	PgL->m_n = 4;
	PgL->m_P[0] = MgPoint3C( lnK[1].p[1], rZL);
	PgL->m_P[1] = MgPoint3C( lnK[1].p[1], rZU);
	PgL->m_P[2] = MgPoint3C( lnK[1].p[0], rZU);
	PgL->m_P[3] = MgPoint3C( lnK[1].p[0], rZL);
}

//////////////////////////////////////////////////////////////////////////////
////	【機能】壁のディスプレイリストを作成する 実態、半透明
////			
//void Kabe(

////////////////////////////////////////////////////////////////////////////
//	【機能】建具の開口枠のディスプレイリストを作成する
//			
void MdlDispList::DrawTategu(
						MhPlcInfo	*i_pPlcEn,		// 壁またはNULL
						MhPlcInfo	*pPlcTEn,		// 建具
				const	MgPoint3	&PtCtr,			// 構造家モデルの中心
						MREAL		rB				// ３次元表示倍率
				)
{
	MGPOLYG3( PgR, 4);
	MGPOLYG3( PgL, 4);
	MINT	ic1b, ic1;
	MgLine3	LnKR, LnKL;

	TateguHole( i_pPlcEn, pPlcTEn, &PgR, &PgL);

	for ( ic1b=0,ic1=PgR.m_n - 1; ic1>=0; ic1b=ic1,ic1--) {
		LnKR = MgLine3( PgR[ic1b], PgR[ic1]);
		LnKL = MgLine3( PgL[3 - ic1b], PgL[3 - ic1]);
		DispList::DspQuads( DPtoDSP( LnKR, 0., PtCtr, rB), DPtoDSP( LnKL, 0., PtCtr, rB));
	}
}

////////////////////////////////////////////////////////////////////////////
//	【機能】基礎のディスプレイリストを作成する
//			
void MdlDispList::DrawKiso(
						MhPlcInfo	*i_pPlcEn,		// 基礎
				const	MgPoint3	&PtCtr,			// 構造家モデルの中心
						MREAL		rB				// ３次元表示倍率
				)
{
	MREAL		rZU, rZL;
	MINT		ic1;

	MgLine3		LnKiso0;
	MgVect3		vuLnKiso0;
	MgLine3		LnKiso1, LnKiso2;
	MgLine3		LnKiso3;
	MREAL		dl;

	MGPOLYG3( PgW, 4);
	MGPOLYG3( PgR, 4);
	MGPOLYG3( PgL, 4);
	MgGPolyg3	GPgR;
	MgGPolyg3	GPgL;

	rZU = MmSystemProperty::GetStnd( i_pPlcEn->GetPIKai(), MM_STNDH_KISO);			// 基礎基準
	rZL = 0.;																	// グランド
	LnKiso0 = i_pPlcEn->GetPIPlcIti();
	vuLnKiso0 = LnKiso0.Vu();
	LnKiso1 = i_pPlcEn->GetPIZukei()->m_lnZukei[0];				// 右側基礎面線分
	LnKiso2 = i_pPlcEn->GetPIZukei()->m_lnZukei[1];				// 左側基礎面線分

	// 右側基礎面
	PgW.m_n = 4;
	PgW[0] = MgPoint3C( MgPoint2C( LnKiso1.p[0]), rZL);
	PgW[1] = MgPoint3C( MgPoint2C( LnKiso1.p[1]), rZL);
	PgW[2] = MgPoint3C( MgPoint2C( LnKiso1.p[1]), rZU);
	PgW[3] = MgPoint3C( MgPoint2C( LnKiso1.p[0]), rZU);
	GPgR += PgW;

	// 左側基礎面
	PgW.m_n = 4;
	PgW[0] = MgPoint3C( MgPoint2C( LnKiso2.p[0]), rZL);
	PgW[1] = MgPoint3C( MgPoint2C( LnKiso2.p[1]), rZL);
	PgW[2] = MgPoint3C( MgPoint2C( LnKiso2.p[1]), rZU);
	PgW[3] = MgPoint3C( MgPoint2C( LnKiso2.p[0]), rZU);
	GPgL += PgW;

	DispList::DrawPolygon( GPgR, PtCtr, rB);
	DispList::DrawPolygon( GPgL, PtCtr, rB);

	// 基礎上下面　
	dl = (LnKiso1.p[0] - LnKiso0.p[0]) * vuLnKiso0;				// 交差部の隙間を埋める
	if (dl > MGPTOL->D)
		LnKiso1.p[0] -= (dl * vuLnKiso0);
	dl = (LnKiso1.p[1] - LnKiso0.p[1]) * vuLnKiso0;
	if (dl < - MGPTOL->D)
		LnKiso1.p[1] -= (dl * vuLnKiso0);
	dl = (LnKiso2.p[1] - LnKiso0.p[0]) * vuLnKiso0;
	if (dl > MGPTOL->D)
		LnKiso2.p[1] -= (dl * vuLnKiso0);
	dl = (LnKiso2.p[0] - LnKiso0.p[1]) * vuLnKiso0;
	if (dl < - MGPTOL->D)
		LnKiso2.p[0] -= (dl * vuLnKiso0);

	LnKiso2.SetRev();
	DispList::DspQuads( DPtoDSP( LnKiso2, rZL, PtCtr, rB), DPtoDSP( LnKiso1, rZL, PtCtr, rB));		// 基礎下面
	DispList::DspQuads( DPtoDSP( LnKiso1, rZU, PtCtr, rB), DPtoDSP( LnKiso2, rZU, PtCtr, rB));		// 基礎上面

	// 基礎端部面
	for ( ic1=2; ic1<i_pPlcEn->GetPIZukei()->m_nZukei; ic1++) {
		LnKiso3 = i_pPlcEn->GetPIZukei()->m_lnZukei[ic1];									// 基礎端部面線分
		DispList::DspQuads( DPtoDSP( LnKiso3, rZL, PtCtr, rB), DPtoDSP( LnKiso3, rZU, PtCtr, rB));	// 基礎端部面
	}
}

//////////////////////////////////////////////////////////////////////////////
////	【機能】屋根のディスプレイリストを作成する　実体用
////			
//void MdlDispList::DrawYane1(

//////////////////////////////////////////////////////////////////////////////
////	【機能】矢切のディスプレイリストを作成する　実体用
////			
//void MdlDispList::DrawYagiri(

//////////////////////////////////////////////////////////////////////////////
////	【機能】屋根のディスプレイリストを作成する　半透明
////			
//void MdlDispList::DrawYane2(

////////////////////////////////////////////////////////////////////////////
//	【機能】たて枠、束のディスプレイリストを作成する
//			
void MdlDispList::DrawTatewaku( 
						MhPlcInfo	*i_pPlcEn,
				const	MgPoint3	&PtCtr,
						MREAL		rB
				)
{
	MREAL		rZ1, rZ2;
	MgLine3		LnPlc;
	MgLine3		LnBz;
	MREAL		rW1, rH1;

	MgVect3		VtHoko, VtRt, VtUp;					// 部材の配置方向ベクトル、右方向ベクトル、上方向ベクトル
	MgVect3		VuHoko, VuRt, VuUp;					// 部材の配置方向単位ベクトル、右方向単位ベクトル、上方向単位ベクトル
	MgVect3		VtHzSz;								// 配置点ずれ + 材軸芯ずれ
	MREAL		rHgt1, rHgt2;

	MgPoint3	PT[2][2][2];						// 頂点座標

	rZ1 = MmSystemProperty::GetStnd( i_pPlcEn->GetPIKai(), MM_STNDH_LOWER);
	rZ2 = MmSystemProperty::GetStnd( i_pPlcEn->GetPIKai(), MM_STNDH_CEILING);
//	LnPlc.Set( i_pPlcEn->m_lnPlc);
	LnPlc = i_pPlcEn->GetPIPlcIti();

	VuHoko = LnPlc.Vu();
	VuRt = VuHoko.RotR90();
	VuUp = VuRt ^ VuHoko;

	VtHzSz = (i_pPlcEn->GetPIPlcZure() - i_pPlcEn->GetMbWidth() * 0.5f) * VuHoko +
			 i_pPlcEn->GetPISinZure() * VuHoko.RotR90();
	
	rHgt1 = i_pPlcEn->GetPIHgt( 0);												// 下端高さ
	LnPlc.p[0] = MgPoint3C( MgPoint2C( LnPlc.p[0] + VtHzSz), rHgt1);
	rHgt2 = i_pPlcEn->GetPIHgt( 1);												// 上端高さ
	if ( MGeo::LE( rHgt2, 0.)) {
		rHgt2 = rZ2 - rZ1 + rHgt2;
	} else {
		rHgt2 = rHgt2;
	}
	LnPlc.p[1] = MgPoint3C( MgPoint2C( LnPlc.p[0]), rHgt2);

	LnBz = DPtoDSP( LnPlc, rZ1, PtCtr, rB);
	rW1 = (i_pPlcEn->GetMbTWidth() - 0.2f) * rB;
	rH1 = i_pPlcEn->GetMbTHeight() * rB;

	VtHoko = VuHoko * rH1;
	VtRt = VuRt * rW1;
	VtUp = LnBz.p[1] - LnBz.p[0];

	PT[0][0][0].Set( LnBz.p[0] + VtRt * 0.5);					// 下付け材
	PT[0][0][1].Set( PT[0][0][0] + VtUp);						//
	PT[0][1][0].Set( PT[0][0][0] - VtRt);						//
	PT[0][1][1].Set( PT[0][0][1] - VtRt);						//
	PT[1][0][0].Set( PT[0][0][0] + VtHoko);						//
	PT[1][0][1].Set( PT[0][0][1] + VtHoko);						//
	PT[1][1][0].Set( PT[0][1][0] + VtHoko);						//
	PT[1][1][1].Set( PT[0][1][1] + VtHoko);						//

	DispList::DspRectangular( PT, VuHoko, VuRt, VuUp);
}

////////////////////////////////////////////////////////////////////////////
//	【機能】横置部材のディスプレイリストを作成する
//			
void MdlDispList::DrawOukaZai( 
						MhPlcInfo	*i_pPlcEn,
				const	MgPoint3	&PtCtr,
						MREAL		rB
				)
{
	MSTNDH		iCdHgt;
	MINT		iULCd;								// 上下付けコード (0:下付け, 1:上付け)
	MREAL		rZ;
	MgLine3		LnPlc;
	MgLine3		LnBz;
	MREAL		rW1, rH1;

	MgVect3		VtLng, VtRt, VtUp;					// 部材の長さ方向ベクトル、右方向ベクトル、上方向ベクトル
	MgVect3		VuLng, VuRt, VuUp;					// 部材の長さ方向単位ベクトル、右方向単位ベクトル、上方向単位ベクトル
	MgVect3		VtTkSz;								// 取り付け高さ + 材軸芯ずれ

	MgPoint3	PT[2][2][2];						// 頂点座標

	iCdHgt = i_pPlcEn->GetPTCdHgt();
	iULCd = i_pPlcEn->GetPTCdToritk();

	rZ = MmSystemProperty::GetStnd( i_pPlcEn->GetPIKai(), iCdHgt);
	LnPlc = i_pPlcEn->GetPIPlcIti();

	VuLng = LnPlc.Vu();
	VuUp = i_pPlcEn->GetPIUpPlc();
	VuRt = VuLng ^ VuUp;
	
	VtTkSz = i_pPlcEn->GetPIHgt( 0) * VuUp + i_pPlcEn->GetPISinZure() * VuRt;		// 取り付け高さ+材軸芯ずれ
	LnPlc.p[0] = LnPlc.p[0] - i_pPlcEn->GetPILenHosei( 0) * VuLng + VtTkSz;
	LnPlc.p[1] = LnPlc.p[1] + i_pPlcEn->GetPILenHosei( 1) * VuLng + VtTkSz;

	// 表示座標系
	LnBz = DPtoDSP( LnPlc, rZ, PtCtr, rB);
	rW1 = i_pPlcEn->GetMbWidth() * rB;
	rH1 = i_pPlcEn->GetMbHeight() * rB;

	VtLng = LnBz.p[1] - LnBz.p[0];
	VtRt = VuRt * rW1;
	VtUp = VuUp * rH1;

	if ( iULCd == 0) 
		PT[0][0][0].Set( LnBz.p[0] + VtRt * 0.5);				// 下付け材
	else
		PT[0][0][0].Set( LnBz.p[0] + VtRt * 0.5 - VtUp);		// 上付け材

	PT[0][0][1].Set( PT[0][0][0] + VtUp);						//
	PT[0][1][0].Set( PT[0][0][0] - VtRt);						//
	PT[0][1][1].Set( PT[0][0][1] - VtRt);						//
	PT[1][0][0].Set( PT[0][0][0] + VtLng);						//
	PT[1][0][1].Set( PT[0][0][1] + VtLng);						//
	PT[1][1][0].Set( PT[0][1][0] + VtLng);						//
	PT[1][1][1].Set( PT[0][1][1] + VtLng);						//

	DispList::DspRectangular( PT, VuLng, VuRt, VuUp);
}

////////////////////////////////////////////////////////////////////////////
//	【機能】床、天井、屋根の開口のディスプレイリストを作成する
//			
void MdlDispList::DrawKaiko( 
						MhPlcInfo *i_pPlcEn,
				const	MgPoint3	&PtCtr,
						MREAL		rB
				)
{
	MSTNDH		iCdHgt;
	MINT		iULCd;								// 上下付けコード (0:下付け, 1:上付け)
	MREAL		rZ;
	MgLine3		LnPlc;
	MgLine3		LnBz;
	MREAL		rH1, rTH, rOY;

	MgVect3		VtLng, VtRt, VtUp;					// 部材の長さ方向ベクトル、右方向ベクトル、上方向ベクトル
	MgVect3		VuLng, VuRt, VuUp;					// 部材の長さ方向単位ベクトル、右方向単位ベクトル、上方向単位ベクトル
	MgVect3		VtTkH;								// 取り付け高さ
	MgVect3		VtTH, VtOY;							// 手前側補正値、奥行き
	MgPoint3	PT[2][2][2];						// 頂点座標
	MgVect2		vuRt;								// 屋根パネル用奥行き方向２次元ベクトル
	MREAL		rlXY;								// 屋根パネル用奥行き方向２次元ベクトルのXY平面上長さ
	
	iCdHgt = (MSTNDH)i_pPlcEn->GetPICdHgt();
	iULCd = i_pPlcEn->GetPTCdToritk();

	rZ = MmSystemProperty::GetStnd( i_pPlcEn->GetPIKai(), iCdHgt);
	LnPlc = i_pPlcEn->GetPIPlcIti();

	VuLng = LnPlc.Vu();
	if ( MGeo::Zero( VuLng.z))
		VuRt = VuLng.RotR90();
	else {
		MREAL r1_D = 1.f / (MREAL)sqrt( VuLng.x * VuLng.x + VuLng.y * VuLng.y);
		VuRt.Set( VuLng.y * r1_D, - VuLng.x * r1_D, 0.f);
	}
	VuUp = i_pPlcEn->GetPIUpPlc();
	VuRt = VuLng ^ VuUp;
	
	VtTkH = i_pPlcEn->GetPIHgt( 0)  * VuUp;						// 取り付け高さ
	LnPlc.p[0] = LnPlc.p[0] - i_pPlcEn->GetPILenHosei( 0) * VuLng + VtTkH;
	LnPlc.p[1] = LnPlc.p[1] + i_pPlcEn->GetPILenHosei( 1) * VuLng + VtTkH;

	// 表示座標系
	LnBz = DPtoDSP( LnPlc, rZ, PtCtr, rB);
	rH1 = i_pPlcEn->GetMbHeight() * rB;
	rTH = i_pPlcEn->GetPIMaeHosei() * rB;						// パネル手前側補正値
	rOY = (i_pPlcEn->GetPIMaeHosei() + i_pPlcEn->GetPIOku() +
		   i_pPlcEn->GetPIOkuHosei()) * rB;						// パネル手前側補正値 + 奥行長 + 奥側補正値
	if ( i_pPlcEn->IsYanePanel()) {
		vuRt = MgVect2C( VuRt);
		rlXY = MGeo::Abs( vuRt);
		if ( !MGeo::Zero( rlXY)) {
			rTH /= rlXY;
			rOY /= rlXY;
		}
	}

	VtLng = LnBz.p[1] - LnBz.p[0];
	VtTH = VuRt * rTH;
	VtOY = VuRt * rOY;
	VtUp = VuUp * rH1;

	if ( iULCd == 0) 
		PT[0][0][0].Set( LnBz.p[0] + VtTH);						// 下付け材
	else
		PT[0][0][0].Set( LnBz.p[0] + VtTH - VtUp);				// 上付け材

	PT[0][0][1].Set( PT[0][0][0] + VtUp);						//
	PT[0][1][0].Set( PT[0][0][0] - VtOY);						//
	PT[0][1][1].Set( PT[0][0][1] - VtOY);						//
	PT[1][0][0].Set( PT[0][0][0] + VtLng);						//
	PT[1][0][1].Set( PT[0][0][1] + VtLng);						//
	PT[1][1][0].Set( PT[0][1][0] + VtLng);						//
	PT[1][1][1].Set( PT[0][1][1] + VtLng);						//

	DispList::DspRectangular( PT, VuLng, VuRt, VuUp);
}

} // namespace MC
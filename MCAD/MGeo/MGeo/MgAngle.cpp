//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgAngle.cpp
//
//		
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"
#include "MbDebug.h"

#define	DLL_EXPORT_GEO_DO

#include "MgLib.h"

namespace MC
{

/////////////////////////////////////////////////////////////////////////////
//		直線　形状演算
//
// ---------------------( ２次元 )------------------------------
//
//	ベクトルと水平線の角度を求める
//
MREAL	MGeo::GetAngleVH2(					// (  O) 左回転角度　（度）
												//		ベクトルの長さがg_gTol.D未満の場合は0度を返す
				const	MgVect2&	v1			// (I  ) ベクトル
		)
{
	if ((v1 * v1) < g_gTol.D_2) return 0.0f;
	
	return MGDEGREE( MREAL( atan2( v1.y, v1.x)));
}

/////////////////////////////////////////////////////////////////////////////
//	２点と水平線の角度を求める
//
MREAL	MGeo::GetAngle2PtH2(					// (  O) 左回転角度　（度）
												//		始点、終点間の距離がg_gTol.D未満の場合は0度を返す
				const	MgPoint2&	p1,			// (I  ) 始点
				const	MgPoint2&	p2			// (I  ) 終点
		)
{
	MgVect2 v1;
	
	v1 = p2 - p1;
	if ((v1 * v1) < g_gTol.D_2) return 0.0f;
	
	return MGDEGREE( MREAL( atan2( v1.y, v1.x)));
}

/////////////////////////////////////////////////////////////////////////////
//	２ベクトルの角度を求める
//
MREAL	MGeo::GetAngle2V2(					// (  O) 左回転角度　（度）
												//		ベクトルの長さがg_gTol.D未満の場合は0度を返す
				const	MgVect2&	v1,			// (I  ) ベクトル1
				const	MgVect2&	v2			// (I  ) ベクトル2
		)
{
	MREAL l1, l2;
//	MREAL ss2, s12, c12, ang;
	MREAL s12, c12;
//
	if ((l1 = (v1 * v1)) < g_gTol.D_2 ||
	    (l2 = (v2 * v2)) < g_gTol.D_2) return 0.0f;

	s12 = (v1 ^ v2);
	c12 = (v1 * v2);
	
	return MGDEGREE( MREAL( atan2( s12, c12)));
/*
	ss2 = MREAL( (MREAL)sqrt( l1 * l2));
	s12 = (v1 ^ v2) / ss2;
	c12 = (v1 * v2) / ss2;
	if (s12*s12 < 0.5) {
		ang = MREAL( asin( s12)) * MC_RTD;
		if (c12 < 0.) ang = 180.f - ang;
	} else {
		ang = MREAL( acos( c12)) * MC_RTD;
		if (s12 <0.) ang = - ang;
	}
	return ang;
*/
}

/////////////////////////////////////////////////////////////////////////////
//	３点で作られる２直線の角度を求める
//
MREAL	MGeo::GetAngle3Pt2(					// (  O) 左回転角度　（度）
												//		2点間の距離がg_gTol.D未満の場合は0度を返す
				const	MgPoint2&	p1,			// (I  ) 点1
				const	MgPoint2&	p2,			// (I  ) 点2
				const	MgPoint2&	p3			// (I  ) 点3
		)
{
	MgVect2	v21, v23;
//
	v21 = p1 - p2;
	v23 = p3 - p2;
	return GetAngle2V2(v21, v23);
}

/////////////////////////////////////////////////////////////////////////////
//	２直線の角度を求める
//
MREAL	MGeo::GetAngle2ULn2(					// (  O) 左回転角度　（度）
												//		2点間の距離がg_gTol.D未満の場合は0度を返す
				const	MgULine2&	ULn1,		// (I  ) 線分1
				const	MgULine2&	ULn2		// (I  ) 線分2
		)
{
	return GetAngle2V2(ULn1.v, ULn2.v);
}

/////////////////////////////////////////////////////////////////////////////
//	直線と線分の角度を求める
//
MREAL	MGeo::GetAngleULnLn2(					// (  O) 左回転角度　（度）
												//		2点間の距離がg_gTol.D未満の場合は0度を返す
				const	MgULine2&	ULn1,		// (I  ) 直線1
				const	MgLine2&	Ln2			// (I  ) 線分2
		)
{
	MgVect2	v2;
//
	v2 = Ln2.p[1] - Ln2.p[0];
	return GetAngle2V2( ULn1.v, v2);
}

//	-------------------------------------------------------
MREAL	MGeo::GetAngleLnULn2(					// (  O) 左回転角度　（度）
												//		2点間の距離がg_gTol.D未満の場合は0度を返す
				const	MgLine2&	Ln1,		// (I  ) 線分1
				const	MgULine2&	ULn2		// (I  ) 直線2
		)
{
	MgVect2	v1;
//
	v1 = Ln1.p[1] - Ln1.p[0];
	return GetAngle2V2(v1, ULn2.v);
}

/////////////////////////////////////////////////////////////////////////////
//	２線分の角度を求める
//
MREAL	MGeo::GetAngle2Ln2(					// (  O) 左回転角度　（度）
												//		2点間の距離がg_gTol.D未満の場合は0度を返す
				const	MgLine2&	Ln1,		// (I  ) 線分1
				const	MgLine2&	Ln2			// (I  ) 線分2
		)
{
	MgVect2	v1, v2;
//
	v1 = Ln1.p[1] - Ln1.p[0];
	v2 = Ln2.p[1] - Ln2.p[0];
	return GetAngle2V2( v1, v2);
}

/////////////////////////////////////////////////////////////////////////////
//	２ベクトルの角度を求める
//
MREAL	MGeo::GetAngle2V3(					// (  O) 左回転角度　（度）
												//		ベクトルの長さがg_gTol.D未満の場合は0度を返す
				const	MgVect3&	v1,			// (I  ) ベクトル1
				const	MgVect3&	v2,			// (I  ) ベクトル2
				const	MgVect3&	vn			// (I  ) ２ベクトルに直交する単位ベクトル
		)
{
//	MREAL l1, l2;
//	MREAL ss2, s12, c12, ang;
	MREAL s12, c12;
//
	s12 = (v1 ^ v2) * vn;
	c12 = v1 * v2;

	return MGDEGREE( MREAL( atan2( s12, c12)));
/*
	if ((l1 = (v1 * v1)) < g_gTol.D_2 ||
	    (l2 = (v2 * v2)) < g_gTol.D_2) return 0.0f;
	ss2 = MREAL(sqrt(l1 * l2));
	s12 = ((v1 ^ v2) * vn) / ss2;
	c12 = (v1 * v2) / ss2;
	if (s12*s12 < 0.5) {
		ang = MREAL(asin(s12)) * MC_RTD;
		if (c12 < 0.) ang = 180.f - ang;
	} else {
		ang = MREAL(acos(c12)) * MC_RTD;
		if (s12 <0.) ang = - ang;
	}
	return ang;
*/
}

/////////////////////////////////////////////////////////////////////////////
//	３点で作られる２直線の角度を求める
//
MREAL	MGeo::GetAngle3Pt3(					// (  O) 左回転角度　（度）
												//		2点間の距離がg_gTol.D未満の場合は0度を返す
				const	MgPoint3&	p1,			// (I  ) 点1
				const	MgPoint3&	p2,			// (I  ) 点2
				const	MgPoint3&	p3,			// (I  ) 点3
				const	MgVect3&	vn			// (I  ) 点1,点,2点3を通る平面に直交する単位ベクトル
		)
{
	MgVect3	v21, v23;
//
	v21 = p1 - p2;
	v23 = p3 - p2;
	return GetAngle2V3( v21, v23, vn);
}

/////////////////////////////////////////////////////////////////////////////
//	２直線の角度を求める
//
MREAL	MGeo::GetAngle2ULn3(					// (  O) 左回転角度　（度）
												//		2点間の距離がg_gTol.D未満の場合は0度を返す
				const	MgULine3&	ULn1,		// (I  ) 直線1
				const	MgULine3&	ULn2,		// (I  ) 直線2
				const	MgVect3&	vn			// (I  ) ２直線に直交する単位ベクトル
		)
{
	return GetAngle2V3( ULn1.v, ULn2.v, vn);
}

/////////////////////////////////////////////////////////////////////////////
//	直線と線分の角度を求める
//
MREAL	MGeo::GetAngleULnLn3(					// (  O) 左回転角度　（度）
												//		2点間の距離がg_gTol.D未満の場合は0度を返す
				const	MgULine3&	ULn1,		// (I  ) 直線1
				const	MgLine3&	Ln2,		// (I  ) 線分2
				const	MgVect3&	vn			// (I  ) 直線と線分に直交する単位ベクトル
		)
{
	MgVect3	v2;
//
	v2 = Ln2.p[1] - Ln2.p[0];
	return GetAngle2V3( ULn1.v, v2, vn);
}

//	-------------------------------------------------------
MREAL	MGeo::GetAngleLnULn3(					// (  O) 左回転角度　（度）
												//		2点間の距離がg_gTol.D未満の場合は0度を返す
				const	MgLine3&	Ln1,		// (I  ) 線分1
				const	MgULine3&	ULn2,		// (I  ) 直線2
				const	MgVect3&	vn			// (I  ) 線分と直線に直交する単位ベクトル
		)
{
	MgVect3	v1;
//
	v1 = Ln1.p[1] - Ln1.p[0];
	return GetAngle2V3( v1, ULn2.v, vn);
}

/////////////////////////////////////////////////////////////////////////////
//	２線分の角度を求める
//
MREAL	MGeo::GetAngle2Ln3(					// (  O) 左回転角度　（度）
												//		2点間の距離がg_gTol.D未満の場合は0度を返す
				const	MgLine3&	Ln1,		// (I  ) 線分1
				const	MgLine3&	Ln2,		// (I  ) 線分2
				const	MgVect3&	vn			// (I  ) ２線分に直行する単位ベクトル
		)
{
	MgVect3	v1, v2;
//
	v1 = Ln1.p[1] - Ln1.p[0];
	v2 = Ln2.p[1] - Ln2.p[0];
	return GetAngle2V3( v1, v2, vn);
}

/////////////////////////////////////////////////////////////////////////////
//		直線　平面　形状演算
//
// ---------------------( ３次元 )------------------------------
//
//	ベクトルと平面の角度を求める
//
MREAL	MGeo::GetAngleVPln3(					// (  O) 左回転角度　（度）
												//		2点間の距離がg_gTol.D未満の場合は0度を返す
				const	MgVect3&	v1,			// (I  ) ベクトル1
				const	MgPlane3&	Pln2		// (I  ) 平面2
		)
{
//	MREAL l1;
//	MREAL s12, c12, ang;
	MREAL s12, c12;
//
	if ((v1 * v1) < g_gTol.D_2) return 0.0f;
	
	c12 = Abs(v1 ^ Pln2.v);
	s12 = Abs(v1 * Pln2.v);
	
	return MGDEGREE( MREAL( atan2( s12, c12)));
/*
	l1 = MgAbs(v1);
	if (l1 < g_gTol.D) return 0.0f;
	c12 = MgAbs(v1 ^ Pln2.v) / l1;
	s12 = MgAbs(v1 * Pln2.v) / l1;
	if (s12 < 0.5) {
		ang = MREAL( asin( s12)) * MC_RTD;
	} else {
		ang = MREAL( acos( c12)) * MC_RTD;
	}
	return ang;
*/
}

/////////////////////////////////////////////////////////////////////////////
//	直線と平面の角度を求める
//
MREAL	MGeo::GetAngleULnPln3(				// (  O) 左回転角度　（度）
												//		2点間の距離がg_gTol.D未満の場合は0度を返す
				const	MgULine3&	ULn1,		// (I  ) 直線1
				const	MgPlane3&	Pln2		// (I  ) 平面2
		)
{
	return GetAngleVPln3( ULn1.v, Pln2);
}

/////////////////////////////////////////////////////////////////////////////
//	線分と平面の角度を求める
//
MREAL	MGeo::GetAngleLnPln3(					// (  O) 左回転角度　（度）
												//		2点間の距離がg_gTol.D未満の場合は0度を返す
				const	MgLine3&	Ln1,		// (I  ) 線分1
				const	MgPlane3&	Pln2		// (I  ) 平面2
		)
{
	MgVect3	v1;
	v1 = Ln1.p[1] - Ln1.p[0];
	return GetAngleVPln3( v1, Pln2);
}

/////////////////////////////////////////////////////////////////////////////
//		直線　形状演算
//
// ---------------------( ２次元 )------------------------------
//
//	２直線間の角の2等分線を求める
//
void	MGeo::Bisector2ULn2(					// (  O) ステータス
				const	MgULine2&	ULn1,		// (I  ) 直線1
				const	MgULine2&	ULn2,		// (I  ) 直線2
						MgULine2*	ULn3		// (  O) 直線3
		)
{
	MREAL	c12;
	MgVect2 vd;
//
	if ( Intr2ULn2(ULn1, ULn2, &(*ULn3).p) == MC_PARALLEL) (*ULn3).p = ULn1.p;
	c12 = ULn1.v * ULn2.v;
	if ( c12 > -0.7) {
		vd = (ULn1.v + ULn2.v);
	} else {
		vd = RotL90( ULn1.v - ULn2.v);
	}
	(*ULn3).v = MGeo::UnitizeV2(vd);
}

/////////////////////////////////////////////////////////////////////////////
//	直線と線分間の角の2等分線を求める
//
void MGeo::BisectorULnLn2(					// (  O) ステータス
				const	MgULine2&	ULn1,		// (I  ) 直線1
				const	MgLine2&	Ln2,		// (I  ) 線分2
						MgULine2*	ULn3		// (  O) 直線3
		)
{
	MgULine2	ULn2;
//
	ULn2.p = Ln2.p[0];
	ULn2.v = MGeo::UnitizeV2(Ln2.p[1] - Ln2.p[0]);
	Bisector2ULn2( ULn1, ULn2, ULn3);
}

/////////////////////////////////////////////////////////////////////////////
//	線分と直線間の角の2等分線を求める
//
void MGeo::BisectorLnULn2(					// (  O) ステータス
				const	MgLine2&	Ln1,		// (I  ) 線分1
				const	MgULine2&	ULn2,		// (I  ) 直線2
						MgULine2*	ULn3		// (  O) 直線3
		)
{
	MgULine2	ULn1;
//
	ULn1.p = Ln1.p[0];
	ULn1.v = MGeo::UnitizeV2(Ln1.p[1] - Ln1.p[0]);
	Bisector2ULn2( ULn1, ULn2, ULn3);
}

/////////////////////////////////////////////////////////////////////////////
//	線分と線分間の角の2等分線を求める
//
void MGeo::Bisector2Ln2(						// (  O) ステータス
				const	MgLine2&	Ln1,		// (I  ) 線分1
				const	MgLine2&	Ln2,		// (I  ) 線分2
						MgULine2*	ULn3		// (  O) 直線3
		)
{
	MgULine2	ULn1;
	MgULine2	ULn2;
//
	ULn1.p = Ln1.p[0];
	ULn1.v = MGeo::UnitizeV2( Ln1.p[1] - Ln1.p[0]);
	ULn2.p = Ln2.p[0];
	ULn2.v = MGeo::UnitizeV2( Ln2.p[1] - Ln2.p[0]);
	Bisector2ULn2( ULn1, ULn2, ULn3);
}

/////////////////////////////////////////////////////////////////////////////
//
// ---------------------( ３次元 )------------------------------
//
//	２直線間の角の2等分線を求める
//
void MGeo::Bisector2ULn3(						// (  O) ステータス
				const	MgULine3&	ULn1,		// (I  ) 直線1
				const	MgULine3&	ULn2,		// (I  ) 直線2
						MgULine3*	ULn3		// (  O) 直線3
		)
{
	MREAL	c12;
	MgVect3 vt;
	MgVect3 vp;
	MgVect3 vd;
//
	if ( Intr2ULn3( ULn1, ULn2, &(*ULn3).p) == MC_PARALLEL) (*ULn3).p = ULn1.p;
	c12 = ULn1.v * ULn2.v;
	if ( c12 > -0.7) {
		vd = (ULn1.v + ULn2.v);
	} else {
		vt = (ULn1.v - ULn2.v);
		vp = (ULn1.v ^ ULn2.v);
		vd = vt ^ vp;
	}
	ULn3->v = UnitizeV3( vd);
}

/////////////////////////////////////////////////////////////////////////////
//	直線と線分間の角の2等分線を求める
//
void MGeo::BisectorULnLn3(					// (  O) ステータス
				const	MgULine3&	ULn1,		// (I  ) 直線1
				const	MgLine3&	Ln2,		// (I  ) 線分2
						MgULine3*	ULn3		// (  O) 直線3
		)
{
	MgULine3 ULn2;
//
	ULn2.p = Ln2.p[0];
	ULn2.v = UnitizeV3(Ln2.p[1] - Ln2.p[0]);
	Bisector2ULn3( ULn1, ULn2, ULn3);
}

/////////////////////////////////////////////////////////////////////////////
//	線分と直線間の角の2等分線を求める
//
void MGeo::BisectorLnULn3(					//
				const	MgLine3&	Ln1,		// (I  ) 線分1
				const	MgULine3&	ULn2,		// (I  ) 直線2
						MgULine3*	ULn3		// (  O) 直線3
		)
{
	MgULine3 ULn1;
//
	ULn1.p = Ln1.p[0];
	ULn1.v = UnitizeV3(Ln1.p[1] - Ln1.p[0]);
	Bisector2ULn3( ULn1, ULn2, ULn3);
}

/////////////////////////////////////////////////////////////////////////////
//	線分と線分間の角の2等分線を求める
//
void MGeo::Bisector2Ln3(						//
				const	MgLine3&	Ln1,		// (I  ) 線分1
				const	MgLine3&	Ln2,		// (I  ) 線分2
						MgULine3*	ULn3		// (  O) 直線3
		)
{
	MgULine3 ULn1;
	MgULine3 ULn2;
//
	ULn1.p = Ln1.p[0];
	ULn1.v = UnitizeV3(Ln1.p[1] - Ln1.p[0]);
	ULn2.p = Ln2.p[0];
	ULn2.v = UnitizeV3(Ln2.p[1] - Ln2.p[0]);
	Bisector2ULn3( ULn1, ULn2, ULn3);
}

} // namespace MC
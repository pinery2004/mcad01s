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
#include "MbBasic.h"
#include "MbDebug.h"

#define DLL_EXPORT_GEO_DO
#define DLL_EXPORT_PLANE_DO

#include "MgLib.h"

namespace MC
{

//===========================================================================
//		平面 形状演算
//

//===========================================================================
//		３点より平面の方程式の係数と定数を求める（正規化）
//
//	ax+by+cz+d=0
//
MgPlane3 MGeo::Plane3CPPt(							// (  O) 平面式
				const	MgPoint3*	pt			// (I  ) ３点座標
				)
{
	MgPlane3	Plno;
	MgVect3		v1, v2;

	v1 = pt[1] - pt[0];
	v2 = pt[2] - pt[0];

	Plno.v = MGeo::UnitizeV3( v1 ^ v2);
	Plno.d = -(Plno.v * pt[0]);
	return Plno;
}

//===========================================================================
//		３点より平面の方程式の係数と定数を求める（正規化）
//
MgPlane3 MGeo::Plane3C3Pt(							// (  O) 平面式
				const	MgPoint3&	p0,			// (I  ) 点１
				const	MgPoint3&	p1,			// (I  ) 点２
				const	MgPoint3&	p2			// (I  ) 点３
				)
{
	MgPlane3	Plno;
	MgVect3		v1, v2;

	v1 = p1 - p0;
	v2 = p2 - p0;

	Plno.v = MGeo::UnitizeV3( v1 ^ v2);
	Plno.d = -(Plno.v * p0);
	return Plno;
}

//===========================================================================
//		多角形より平面の方程式の係数と定数を求める（正規化）
//													pkm	面積の絶対値の合計で求め方向は実面積で求める方が良いか pkm
MgPlane3 MGeo::Plane3CPg(						// (  O) 平面式
				const	MgPolyg3&	i_Pg		// (I  ) 多角形
				)
{
	MgPlane3	Plno;
	MINT		ic0, ic1;
	MgPlane3	pl;

	MgVect3 vNf = MgVect3(0.f, 0.f, 0.f);
	ic1 = i_Pg.m_n - 1;
	for ( ic0=0; ic0<i_Pg.m_n; ic0++) {
		vNf += (i_Pg.m_P[ic1] ^ i_Pg.m_P[ic0]);
		ic1 = ic0;
	}
	Plno.v = MGeo::UnitizeV3( vNf);
	Plno.d = -(Plno.v * i_Pg.m_P[0]);
	return Plno;
}

//
//===========================================================================
//		２次元始点と２次元終点の２点、勾配とＺ座標より平面の方程式の係数と定数を求める（正規化）
//
MgPlane3 MGeo::Plane3C2Pt2F(
				const	MgPoint2&	p1,			// (I  ) ２次元始点
				const	MgPoint2&	p2,			// (I  ) ２次元終点
						MREAL		rkb,		// (I  ) 勾配	(始点から終点に向かい左方向=0 tan)
						MREAL		z			// (I  ) Ｚ座標
				)
{
	MgPlane3	Plno;
	MgPoint3	Pt[3];
	MgVect3		v1, v2;
	MREAL		h1;

	Pt[0] = MgPoint3C( p1, z);
	Pt[1] = MgPoint3C( p2, z);
	v1 = Pt[1] - Pt[0];

	Pt[2] = Pt[0] + v1.RotL90();
	h1 = MGeo::Abs( v1) * rkb;
	Pt[2].z += h1;
	v2 = Pt[2] - Pt[0];

	Plno.v = MGeo::UnitizeV3( v1 ^ v2);
	Plno.d = -(Plno.v * Pt[0]);
	return Plno;
}

//
//===========================================================================
//		２次元線分、勾配とＺ座標より平面の方程式の係数と定数を求める（正規化）
//
MgPlane3 MGeo::Plane3CLn2F(
				const	MgLine2&	ln1,		// (I  ) ２次元線分
						MREAL		rkb,		// (I  ) 勾配	(線分の始点から終点に向かい左方向=0 tan)
						MREAL		z			// (I  ) Ｚ座標
				)
{
	MgPlane3	Plno;
	MgPoint3	Pt[3];
	MgVect3		v1, v2;
	MREAL		h1;

	Pt[0] = MgPoint3C( ln1.p[0], z);
	Pt[1] = MgPoint3C( ln1.p[1], z);
	v1 = Pt[1] - Pt[0];

	Pt[2] = Pt[0] + v1.RotL90();
	h1 = MGeo::Abs( v1) * rkb;
	Pt[2].z += h1;
	v2 = Pt[2] - Pt[0];

	Plno.v = MGeo::UnitizeV3( v1 ^ v2);
	Plno.d = -(Plno.v * Pt[0]);
	return Plno;
}

////===========================================================================
////		平面の方程式の係数と定数を正規化する(係数a,b,cを単位ベクトルに変換する)
////
//MgPlane3 MgPlane3::SetNorm( MREAL i_Tol)
//{
//	MgPlane3	Plno;
//	MREAL	d1 = MgAbs( v);
//		MBCHECK_ZEROVECT( d1, MBCstr("MgPlane3 Norm"). i_Tol);
//	MREAL	d2 = 1.f / d1;
////	v.x *= d2;
////	v.y *= d2;
////	v.z *= d2;
//	v *= d2;
//	d *= d2;
//	return *this;
//}

//===========================================================================
//		平面の方程式の係数と定数を正規化した(係数a,b,cを単位ベクトルに変換した)平面を求める
//
MgPlane3 MgPlane3::Norm( MREAL i_Tol)
{
	MREAL	d1 = MGeo::Abs( v);
		MBCHECK_ZEROVECT( d1, MBCstr("MgPlane3 Norm"), i_Tol);
	MREAL	d2 = 1.f / d1;
	return MgPlane3(v.x * d2, v.y * d2, v.z * d2, d * d2);
}
//
//===========================================================================
//		平面が等しい事を判定する（平面の方程式の係数と定数を正規化し比較する）
//
bool MgPlane3::operator == ( const MgPlane3& pl) const							// b = (Pl1 == Pl2) 判定
{
//	MgPlane3 pl1 = MgNorm( *this);
//	MgPlane3 pl2 = MgNorm( pl); 
//	return (pl1.v == pl2.v) && MFEQUAL( pl1.d, pl2.d, g_gTol.D);
//	return ( v == pl.v) && MFEQUAL( d, pl.d, g_gTol.D);
	return ( v == pl.v) && MGeo::Equal( d, pl.d, g_gTol.D);
}
//
//======================( ３次元 )==============================
//		平面をトレースする
//
void MgPlane3::Print(
						MCHAR*		s
				)								// print
{
#ifdef _DEBUG
	Msprintf( MbLog::m_Str, Mstr( "%s	MgPlane3	: (%7.1f,%7.1f,%7.1f),%7.1f\n"),
								s, v.x, v.y, v.z, d);
	MBTRCPRBF;
#endif
};
//
//===========================================================================
//		平面の方程式の係数と定数を正規化した(係数a,b,cを単位ベクトルに変換した)平面を求める
//
MgPlane3 MGeo::NormPln(
				const	MgPlane3&	i_pl,
						MREAL		i_Tol
				)
{
	MREAL	d1 = MGeo::Abs( i_pl.v);
		MBCHECK_ZEROVECT( d1, MBCstr( "MgPlane3 MgNorm"), i_Tol);
	MREAL	d2 = 1.f / d1;
	return MgPlane3( i_pl.v.x * d2, i_pl.v.y * d2, i_pl.v.z * d2, i_pl.d * d2);
}

} // namespace MC
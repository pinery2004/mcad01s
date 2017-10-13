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

#include "MgLib.h"
#include "MgMat.h"
#include "MgKeijyou.h"

namespace MC
{

//===========================================================================
//		形状演算
//
// ---------------------( ２次元 )------------------------------
//
//	同次座標による３点よりＳ形式を求める
//
MREAL	MgSMatD(const MgPoint2D3& p0, const MgPoint2D3& p1,
			   const MgPoint2D3& p2)
{
	return 
	(	  p0.x * (p1.y*p2.w-p1.w*p2.y)
		- p0.y * (p1.x*p2.w-p1.w*p2.x)
		+ p0.w * (p1.x*p2.y-p1.y*p2.x));
}
//
// ---------------------( ３次元 )------------------------------
//
//	同次座標による４点よりＳ形式を求める
//
MREAL	MgSMatD3(const MgPoint2D3& p0, const MgPoint2D3& p1,
				 const MgPoint2D3& p2, const MgPoint2D3& p3)
{
	return 
	(	  (p0.x*p1.y-p0.y*p1.x) * (p2.z*p3.w-p2.w*p3.z)
		- (p0.x*p1.z-p0.z*p1.x) * (p2.y*p3.w-p2.w*p3.y)
		+ (p0.x*p1.w-p0.w*p1.x) * (p2.y*p3.z-p2.z*p3.y)
		+ (p0.y*p1.z-p0.z*p1.y) * (p2.x*p3.w-p2.w*p3.x)
		- (p0.y*p1.z-p0.z*p1.y) * (p2.x*p3.z-p2.z*p3.x)
		+ (p0.z*p1.w-p0.w*p1.z) * (p2.x*p3.y-p2.y*p3.x));
}
//
//	同次座標による３点より平面式を求める
//
//  平面式 : nx*X + ny*Y + nz*Z + d = 0
//
MgPlane3D3::MgPlane3D3(const MgPoint2D3& p0, const MgPoint2D3& p1,
						  const MgPoint2D3& p2)
{
	MREAL xy = p1.x*p2.y - p1.y*p2.x;
	MREAL yz = p1.y*p2.z - p1.z*p2.y;
	MREAL zx = p1.z*p2.x - p1.x*p2.z;
	MREAL xw = p1.x*p2.w - p1.w*p2.x;
	MREAL yw = p1.y*p2.w - p1.w*p2.y;
	MREAL zw = p1.z*p2.w - p1.w*p2.z;
	NX = p0.y*zw - p0.z*yw + p0.w*yz;
	NY = p0.z*xw - p0.x*zw + p0.w*zx;
	NZ = p0.x*yw - p0.y*xw + p0.w*xy;
	d  = - (p0.x*yz + p0.y*zx + p0.z*xy);
}
//
//	同次座標による４Ｘ４行列式より平面式を求める
//
//  平面式 : nx*X + ny*Y + nz*Z + d = 0
//
MgPlane3D3::MgPlane3D3(const MgMat3 &m1)
{
	MREAL xy = m1.m[0][2] * m1.m[1][3] - m1.m[1][2] * m1.m[0][3];
	MREAL yz = m1.m[1][2] * m1.m[2][3] - m1.m[2][2] * m1.m[1][3];
	MREAL zx = m1.m[2][2] * m1.m[0][3] - m1.m[0][2] * m1.m[2][3];
	MREAL xw = m1.m[0][2] * m1.m[3][3] - m1.m[3][2] * m1.m[0][3];
	MREAL yw = m1.m[1][2] * m1.m[3][3] - m1.m[3][2] * m1.m[1][3];
	MREAL zw = m1.m[2][2] * m1.m[3][3] - m1.m[3][2] * m1.m[2][3];
	NX = m1.m[1][1] * zw - m1.m[2][1] * yw + m1.m[3][1] * yz;
	NY = m1.m[2][1] * xw - m1.m[0][1] * zw + m1.m[3][1] * zx;
	NZ = m1.m[0][1] * yw - m1.m[1][1] * xw + m1.m[3][1] * xy;
	d  = - (m1.m[0][1] * yz + m1.m[1][1] * zx + m1.m[2][1] * xy);
}
//
//	平面に対する点のサイドテスト
//
//  return -1:平面の裏側　0:平面上　1:面の表側
//
int	MgSide(const MgPoint2D3& p1, const MgPlane3D3 &Pln1)
{
	MREAL	hb1;
	MREAL	pltol;				// (平面の法線ベクトルの絶対値*誤差範囲)**2

	hb1 = Pln1.NX * p1.x + Pln1.NY * p1.y +
		  Pln1.NZ * p1.z + Pln1.d * p1.w;

    pltol = g_gTol.A_2 * 
		    (Pln1.NX * Pln1.NX + Pln1.NY * Pln1.NY + Pln1.NZ * Pln1.NZ);
	if (hb1 * hb1 < pltol) return 0;
	if (hb1 > 0) return 1;
	return -1;
}
//
//	２線分の重なり、交差テスト(0,1)
MINT MgIntersect(const MgLine2D3 &Ln1, const MgLine2D3 &Ln2, MINT *ist)
{
/*
	MREAL s1, s2, s3, s4;
	s1 = MgSMatD3(n1, Ln1.p[0], Ln1.p[1], Ln2.p[0]);
	s2 = MgSMatD3(n1, Ln1.p[0], Ln1.p[1], Ln2.p[1]);
	s3 = MgSMatD3(n1, Ln2.p[0], Ln2.p[1], Ln1.p[0]);
	s4 = MgSMatD3(n1, Ln2.p[0], Ln2.p[1], Ln1.p[1]);
*/
	return 1;
}
MREAL	MgSMatD3(const MgPoint2D3& p0, const MgPoint2D3& p1,
				 const MgPoint2D3& p2, const MgPoint2D3& p3);
MINT MgIntersect(const MgPoint2D3& p1, const MgTriangleD3 &t1, MINT *ist);
//	点と３角形の半空間テスト(2)
MINT MgInside(const MgTriangleD3 &t1, const MgPoint2D3& p1, MINT *ist);
//	３角形と点（平面）の内外テスト(3)
MINT MgIntersect(const MgLine2D3 &Ln1, const MgTriangleD3 &t1);
//	線分と３角形の交差テスト(4,5,6)
void  MgMkCVT(const MgPolyg2onD3 &pg1, MgCVTD3 *cvt);
//	面分の特性を求める(10,11)
void  MgPartition(const MgPolyg2onD3 &pg1, MgCVTD3 *cvt);
//	面分の３角形分割(12)
MINT MgVolume(const MgPoint2& p1, const MgPolyg2onD3 &pg1, MgCVTD3 *cvt);
//	点と面分の錐体体積の算出、距離(20)
MINT MgInside(const MgPolyg2onD3 &pg1, const MgPoint2D3& p1, MINT *ist);
//	面分に対する点（平面）の内外テスト(21)
MINT MgShade(const MgPolyg2onD3 &pg1, const MgPoint2D3& p1, MINT *ist);
//	面分に対する点（平面）の遮蔽(22)
MINT MgIntersect(const MgLine2D3 &Ln1, const MgPolyg2onD3 &Ln2, MINT *ist);
//	線分と面分の交差テスト(30,31,32)
MINT MgNearestEdge(const MgLine2D3 &Ln1, const MgPolyg2onD3 &Ln2, MINT *ist);
//	最近傍交差稜線(33)
MINT MgInside(const MgPolyg2onD3 &pg1, const MgLine2D3 &Ln1, MINT *ist);
//	面分に対する線分（平面）の内外テスト(34)
MINT MgShade(const MgPolyg2onD3 &pg1, const MgLine2D3 &Ln1, MINT *ist);
//	面分に対する線分（平面）の遮蔽(35)
MgPoint2D3 MgRatio(const MgPoint2& p1, const MgPoint2& p2, MREAL r1);
//	線分ＶａＶｂと内分比r1より、線分ＶａＶｂをr1:(1-r1)に内分する点の
//	同時座標値の算出(192)
void MgSValue(const MgPoint2D3& p1, MINT a, MINT b, MgCVTD3 *cvt);
//	点p1(Vx)とa,bより、Sxijk(i=a..b)をSARY[i]に書き込む(200)
//	(j=i+1, k=i+2)
void MgSValue(const MgPoint2D3& p1, const MgPoint2D3& p2, MINT a, MINT b, MgCVTD3 *cvt);
//	点p1(Vx)、点p2(Vy)とa,bより、Sxyij(i=a..b)をSARY[i]に書き込む(220)
//	(j=i+1)
void MgSValue(const MgPoint2D3& p1, const MgPoint2D3& p2, const MgPoint2& p3, 
			  MINT a, MINT b, MgCVTD3 *cvt);
//	点p1(Vx)、点p2(Vy)、点p3(Vz)とa,bより、Sxyzi(i=a..b)をSARY[i]に書き込む(220)
	
} // namespace MC
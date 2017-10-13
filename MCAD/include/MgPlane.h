#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgPlane.h
//
//	[機能] 
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================

#include "MgLine.h"

class MgPolyg3;

										// 多角形の平面と点の位置関係
//#define	MC_UPPER	010				// 点が平面の上側	
//#define	MC_ON_PLANE	020				// 点が平面上
//#define	MC_LOWER	040				// 点が平面の下側

//#define	MgcUndef	-999			// 未設定状態

#include "MbPlex.h"

#ifdef DLL_EXPORT_PLANE
	#undef DLL_EXPORT_PLANE
#endif

#ifdef DLL_EXPORT_PLANE_DO
	#pragma message( "<<< MgPlane・dll_EXport >>>")
	#define DLL_EXPORT_PLANE							__declspec( dllexport)
#else
	#ifdef _DEBUG								// Release Mode で inline関数がundefになる不具合対応
		#ifdef DLL_NO_IMPORT_GEO_DO
			#define DLL_EXPORT_PLANE
		#else
//			#pragma message( "=== MgPlane・dll_IMport ===")
			#define DLL_EXPORT_PLANE						__declspec( dllimport)
		#endif
	#else
		#define DLL_EXPORT_PLANE
	#endif
#endif

namespace MC
{

//===========================================================================
//		３次元平面
//		( ax + by + cz + d = 0)
//
class DLL_EXPORT_PLANE MgPlane3
{
public:
	MgVect3	v;													// 平面の方程式の係数 a, b, c
	MREAL	d;													// 定数 d
//
public:
	MgPlane3() 			{}

	MgPlane3( MREAL pla, MREAL plb, MREAL plc, MREAL pld)
						{ v.x = pla;	v.y = plb;
						  v.z = plc;	d = pld;}
	MgPlane3( const MgVect3& v1, MREAL d1)
						{ v.x = v1.x;	v.y = v1.y;
						  v.z = v1.z;	d = d1;}

	MgPlane3 SetNorm( MREAL i_Tol = g_gTol.D);					// 係数を正規化する	(a,b,cを単位ベクトルに変換する)

	MgPlane3 Norm( MREAL i_Tol = g_gTol.D);					// 係数を正規化した平面を得る
																//		(a,b,cを単位ベクトルに変換した値を求める)
	void SetHorizontal()
						{ v.x = 0.;		v.y = 0.;
						  v.z = 1.;		d = 0.;}
	MREAL GetZ( const MgPoint2 p1, MREAL i_Tol = g_gTol.D) const	// ２次元座標の位置のＺ座標を求める
						{ if ( MgTol::MgZero( v.z, i_Tol)) return 0.f;
						  else return - (v.x * p1.x + v.y * p1.y + d) / v.z;}
	bool operator == ( const MgPlane3& pl) const;				// b = (Pl1 == Pl2) 判定

	bool operator != ( const MgPlane3& pl) const				// b = (Pl1 != Pl2) 判定
						{ return !(*this == pl);}

	void Print(MCHAR* s);										// トレース
};

} // namespace MC
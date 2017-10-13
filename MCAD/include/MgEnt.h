#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgEnt.h
//
//		図形要素の定義
//
//		半直線(lay)は未対応
//
//  K.Matsu           09/12/07    Created.
//==========================================================================================
//
#define		MPRM_SZPT	10					// デバッグ時の表示用点サイズ
//
// 図形要素
//
typedef struct	_mgeprm2	{
	MSHORT		Type;						// タイプ
	MSHORT		Size;						// サイズ	( byte )
	MgPoint2	p[MPRM_SZPT];				// 座標値	( x, y )
}	MgePrm2;

typedef struct	_mgeprm3	{
	MSHORT		Type;						// タイプ
	MSHORT		Size;						// サイズ	( byte )
	MgPoint3	P[MPRM_SZPT];				// 座標値	( x, y )
}	MgePrm3;

//
// 点	(２次元)
//
typedef struct	_mgepnt2	{
	MSHORT		Type;						// タイプ	( = MPT_DOT )
	MSHORT		Size;						// サイズ	( byte )
	MgPoint2	p;							// 座標値	( x, y )
}	MgePnt2;

//
// 点	(３次元)
//
typedef struct	_mgepnt3	{
	MSHORT		Type;						// タイプ	( = MPT_DOT )
	MSHORT		Size;						// サイズ	( byte )
	MgPoint3	P;							// 座標値	( x, y, z )
}	MgePnt3;

//
// 直線	(２次元)
//
typedef struct	_mgeuline2	{
	MSHORT		Type;						// タイプ	( = MPT_ULINE )
	MSHORT		Size;						// サイズ	( byte )
	MgPoint2	p;							// 点				( x, y )
	MgVect2		v;							// 方向ベクトル	( x, y )
}	MgeULine2;

//
// 直線	(３次元)
//
typedef struct	_mgeuline3	{
	MSHORT		Type;						// タイプ	( = MPT_ULINE )
	MSHORT		Size;						// サイズ	( byte )
	MgPoint3	P;							// 点				( x, y, z )
	MgVect3		V;							// 方向ベクトル	( x, y, z )
}	MgeULine3;

//
// 線分	(２次元)
//
typedef struct	_mgeline2	{
	MSHORT		Type;						// タイプ	( = MPT_LINE )
	MSHORT		Size;						// サイズ	( byte )
	MgPoint2	p[2];						// 始点、終点座標値	( x, y )
}	MgeLine2;

//
// 線分	(３次元)
//
typedef struct	_mgeline3	{
	MSHORT		Type;						// タイプ	( = MPT_LINE )
	MSHORT		Size;						// サイズ	( byte )
	MgPoint3	P[2];						// 始点、終点座標値	( x, y, z )
}	MgeLine3;

//
// ポリライン	(２次元)
//
typedef struct	_mgepolyline2	{
	MSHORT		Type;						// タイプ	( = MPT_PLYL )
	MSHORT		Size;						// サイズ	( byte )
	MgPoint2	p[MPRM_SZPT];				// 構成点座標値	( x, y )
}	MgePlyLine2;

//
// ポリライン	(３次元)
//
typedef struct	_mgepolyline3	{
	MSHORT		Type;						// タイプ	( = MPT_PLYL )
	MSHORT		Size;						// サイズ	( byte )
	MgPoint3	P[MPRM_SZPT];				// 構成点座標値	( x, y, z )
}	MgePlyLine3;

//
// ポリゴン	(２次元)
//
typedef struct	_mgepolygon2	{
	MSHORT		Type;						// タイプ	( = MPT_PLYG )
	MSHORT		Size;						// サイズ	( byte )
	MgPoint2	p[MPRM_SZPT];				// 構成点座標値	( x, y )
}	MgePlygon2;

//
// ポリゴン	(３次元)
//
typedef struct	_mgepolygon3	{
	MSHORT		Type;						// タイプ	( = MPT_PLYG )
	MSHORT		Size;						// サイズ	( byte )
	MgPoint3	P[MPRM_SZPT];				// 構成点座標値	( x, y, z )
}	MgePlygon3;

//
// 穴付きポリゴン	(２次元)
//
typedef struct	_mgegpolygon2	{
	MSHORT		Type;						// タイプ	( = MPT_GPLYG )
	MSHORT		Size;						// サイズ	( byte )
	MgePolygon2	*pg[MPRM_SZPT];				// ポリゴンへのポインタ	( x, y )
}	MgeGPlygon2;

//
// 穴付きポリゴン	(３次元)
//
typedef struct	_mgegpolygon3	{
	MSHORT		Type;						// タイプ	( = MPT_GPLYG )
	MSHORT		Size;						// サイズ	( byte )
	MgePolygon3	*Pg[MPRM_SZPT];				// ポリゴンへのポインタ	( x, y, z )
}	MgeGPlygon3;

//
// 円	(２次元)
//
typedef struct	_mgecircle2	{
	MSHORT		Type;						// タイプ	( = MPT_CIR )
	MSHORT		Size;						// サイズ	( byte )
	MgPoint2	pc;							// 中心点座標値	( x, y )
	MREAL		r;							// 半径
}	MgeCircle2;

//
// 円	(３次元)
//
typedef struct	_mgecircle2	{
	MSHORT		Type;						// タイプ	( = MPT_CIR )
	MSHORT		Size;						// サイズ	( byte )
	MgPoint3	Pc;							// 中心点座標値	( x, y, z )
	MgVect3		V;							// 法線単位ベクトル	( x, y, z )
	MREAL		R;							// 半径
}	MgeCircle2;

//
// 円弧	(２次元)
//
typedef struct	_mgearc2	{
	MSHORT		Type;						// タイプ	( = MPT_ARC )
	MSHORT		Size;						// サイズ	( byte )
	MgPoint2	pt[2];						// 始点、終点値	( x, y )
	MgPoint2	pc;							// 中心点座標値	( x, y )
	MREAL		r;							// 半径
}	MgeArc2;

//
// 円弧	(３次元)
//
typedef struct	_mgearc3	{
	MSHORT		Type;						// タイプ	( = MPT_ARC )
	MSHORT		Size;						// サイズ	( byte )
	MgPoint3	Pt[2];						// 始点、終点値	( x, y, z )
	MgPoint3	Pc;							// 中心点座標値	( x, y, z )
	MgVect3		V;							// 法線単位ベクトル	( x, y, z )
	MREAL		r;							// 半径
}	MgeArc3;

//
// 楕円	(２次元)
//
typedef struct	_mgeeps2	{
	MSHORT		Type;						// タイプ	( = MPT_EPS )
	MSHORT		Size;						// サイズ	( byte )
	MgPoint2	pc;							// 中心点座標値	( x, y )
	MgPoint2	pf;							// 焦点座標値		( x, y )
	MREAL		rl;							// 長径
}	MgeEps2;

//
// 楕円	(３次元)
//
typedef struct	_mgeeps3	{
	MSHORT		Type;						// タイプ	( = MPT_EPS )
	MSHORT		Size;						// サイズ	( byte )
	MgPoint3	Pc;							// 中心点座標値	( x, y, z )
	MgPoint3	Pf;							// 焦点座標値		( x, y, z )
	MgVect3		V;							// 法線単位ベクトル	( x, y, z )
	MREAL		rl;							// 長径
}	MgeEps3;

//
// 楕円弧	(２次元)
//
typedef struct	_mgeeps2	{
	MSHORT		Type;						// タイプ	( = MPT_EPS )
	MSHORT		Size;						// サイズ	( byte )
	MgPoint2	pt[2];						// 始点、終点値	( x, y )
	MgPoint2	pc;							// 中心点座標値	( x, y )
	MgPoint2	pf;							// 焦点座標値		( x, y )
	MREAL		rl;							// 長径
}	MgeEps2;

//
// 楕円弧	(３次元)
//
typedef struct	_mgeeps3	{
	MSHORT		Type;						// タイプ	( = MPT_EPS )
	MSHORT		Size;						// サイズ	( byte )
	MgPoint3	Pt[2];						// 始点、終点値	( x, y, z )
	MgPoint3	Pc;							// 中心点座標値	( x, y, z )
	MgPoint3	Pf;							// 焦点座標値		( x, y, z )
	MgVect3		V;							// 法線単位ベクトル	( x, y, z )
	MREAL		rl;							// 長径
}	MgeEps3;

//
// 平面	( ax + by + cz + d = 0 )
//
typedef struct	_mgepln3	{
	MSHORT		Type;						// タイプ	( = MPT_PLN )
	MSHORT		Size;						// サイズ	( byte )
	MgVect3		V;							// 法線ベクトル	( a, b, c )
	MREAL		d;							// d = - (中心から平面までの平面表方向の距離)
}	MgePln3;

//
// 平面	( ax + by + cz + d = 0 )
//
typedef struct	_mgeplna3	{
	MSHORT		Type;						// タイプ	( = MPT_PLNA )
	MSHORT		Size;						// サイズ	( byte )
	MgPoint3	P;							// 中心点座標値	( x, y, z )
	MREAL		d;							// - (中心から平面までの平面表方向の距離)
	MgePrm3		Prm;						// 図形要素
}	MgePlna3;

//
// 複合曲線 (開)	(２次元)
//
typedef struct	_mgecmpo2	{
	MSHORT		Type;						// タイプ	( = MPT_PLN )
	MSHORT		Size;						// サイズ	( byte )
	MgePrm2		prm;						// 図形要素
}	MgeCmpo2

//
// 複合曲線 (開)	 (３次元)
//
typedef struct	_mgecmpo3	{
	MSHORT		Type;						// タイプ	( = MPT_PLN )
	MSHORT		Size;						// サイズ	( byte )
	MgePrm3		Prm;						// 図形要素
}	MgeCmpo3

//
// 複合曲線 (閉)	(２次元)
//
typedef struct	_mgecmpc2	{
	MSHORT		Type;						// タイプ	( = MPT_PLN )
	MSHORT		Size;						// サイズ	( byte )
	MgePrm2		prm;						// 図形要素
}	MgeCmpc2

//
// 複合曲線 (閉)	 (３次元)
//
typedef struct	_mgecmpc3	{
	MSHORT		Type;						// タイプ	( = MPT_PLN )
	MSHORT		Size;						// サイズ	( byte )
	MgePrm3		Prm;						// 図形要素
}	MgeCmpc3

//
// 多面	(２次元)
//
	typedef struct	_mgemanys2	{			// (many sides)
	MSHORT		Type;						// タイプ	( = MPT_PLN )
	MSHORT		Size;						// サイズ	( byte )
	MgePrm3		prm;						// 図形要素
}	MgeManys2

//
// 多面	 (３次元)
//
typedef struct	_mgemanys3	{
	MSHORT		Type;						// タイプ	( = MPT_PLN )
	MSHORT		Size;						// サイズ	( byte )
	MgePrm3		Prm;						// 図形要素
}	MgeManys3

//
// 多面体	 (３次元)
//
typedef struct	_mgepolyh	{				// (polyhedron)
	MSHORT		Type;						// タイプ	( = MPT_PLN )
	MSHORT		Size;						// サイズ	( byte )
	MgePrm3		Prm;						// 図形要素
}	MgePolyh3

//
// 集合	(２次元)
//
typedef struct	_mgeset2	{
	MSHORT		Type;						// タイプ	( = MPT_PLN )
	MSHORT		Size;						// サイズ	( byte )
	MgePrm3		prm;						// 図形要素
}	MgeSet2

//
// 集合	 (３次元)
//
typedef struct	_mgeset3	{
	MSHORT		Type;						// タイプ	( = MPT_PLN )
	MSHORT		Size;						// サイズ	( byte )
	MgePrm3		Prm;						// 図形要素
}	MgeSet3


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

#define	DLL_EXPORT_LINE_DO

#include "MgLib.h"
#include "MgLine.h"

namespace MC
{

//
//======================( ２次元 )==============================
//		２次元直線の方向を単位ベクトルに正規化する
//
/*
MgULine2 MgULine2::SetNorm()
{
	MREAL	dv = MgAbs( v);
		MGCHECK_ZEROVECTOR( dv, Mstr( "MgULine2 SetNorm"));
	MREAL	dd = 1.f / dv;
	v.x *= dd;
	v.y *= dd;
	return *this;
}
*/
//
//======================( ２次元 )==============================
//		２次元直線の方向を単位ベクトルに正規化する
//
/*
MgULine2 MgULine2::Norm()
{
	MgULine2 uln;
	MREAL	dv = MgAbs( v);
		MGCHECK_ZEROVECTOR( dv, Mstr( "MgULine2 Norm"));
	MREAL	dd = 1.f / dv;
	uln.p = p;
	uln.v.x = v.x * dd;
	uln.v.x = v.y * dd;
	return uln;
}
*/

//======================( ２次元 )==============================
//		２次元直線同士が等しいか調べる
//
bool MgULine2::operator == ( const MgULine2& ULn1) const							// b = (uln == ULn1) 判定
{
	MINT	ist;
	return ( MGeo::ChkPt2OnULn2WS( p, ULn1, &ist) &&
			( MGeo::EqualV( v,  ULn1.v) || MGeo::EqualV( v, -ULn1.v)));
}

//======================( ２次元 )==============================
//		２次元直線をトレースする
//
void MgULine2::Print( MCHAR* s)
{
#ifdef _DEBUG
	Msprintf( MbLog::m_Str, Mstr( "%s	MgULine2	: (%7.1f,%7.1f), (%7.1f,%7.1f)\n"),
								 s, p.x, p.y, v.x, v.y);
	MBTRCPRBF;
#endif
}

//======================( ２次元 )==============================
//		２次元半直線の方向を単位ベクトルに正規化する
//
/*
MgHLine2 MgHLine2::SetNorm()
{
	MREAL	dv = MgAbs( v);
		MGCHECK_ZEROVECTOR( dv, Mstr( "MgHLine2 SetNorm"));
	MREAL	dd = 1.f / dv;
	v.x *= dd;
	v.y *= dd;
	return *this;
}
*/
//
//======================( ２次元 )==============================
//		２次元半直線の方向を単位ベクトルに正規化する
//
/*
MgHLine2 MgHLine2::Norm()
{
	MgHLine2 uln;
	MREAL	dv = MgAbs( v);
		MGCHECK_ZEROVECTOR( dv, Mstr( "MgHLine2 Norm"));
	MREAL	dd = 1.f / dv;
	uln.p = p;
	uln.v.x = v.x * dd;
	uln.v.x = v.y * dd;
	return uln;
}
*/
//
//======================( ２次元 )==============================
//		２次元半直線同士が等しいか調べる
//
bool MgHLine2::operator == ( const MgHLine2 &HLn1) const							// b = (uln == ULn1) 判定
{
	MINT	ist;
	return ( MGeo::ChkPt2OnHLn2WS( p, HLn1, &ist) &&
			( MGeo::EqualV( v,  HLn1.v)));
}
//
//======================( ２次元 )==============================
//		２次元半直線をトレースする
//
void MgHLine2::Print( MCHAR* s)
{
#ifdef _DEBUG
	Msprintf( MbLog::m_Str, Mstr( "%s	MgHLine2	: (%7.1f,%7.1f), (%7.1f,%7.1f)\n"),
									s, p.x, p.y, v.x, v.y);
	MBTRCPRBF;
#endif
}
//
//======================( ２次元 )==============================
//		２次元線分同士が等しいか調べる (逆方向も含む)
//
bool MgLine2::operator == ( const MgLine2& Ln1) const								// b = (ln == Ln1) 判定
{ 
	return (((p[0] == Ln1.p[0]) && (p[1] == Ln1.p[1])) ||
			((p[0] == Ln1.p[1]) && (p[1] == Ln1.p[0])));
}
//
//======================( ２次元 )==============================
//		２次元線分をトレースする
//
void MgLine2::Print( MCHAR* s)
{
#ifdef _DEBUG
	Msprintf( MbLog::m_Str, Mstr( "%s	MgLine2		: (%7.1f,%7.1f), (%7.1f,%7.1f)\n"),
									 s, p[0].x, p[0].y, p[1].x, p[1].y);
	MBTRCPRBF;
#endif
}
//
//======================( ３次元 )==============================
//		直線の方向を単位ベクトルに正規化する
//
/*
MgULine3 MgULine3::Norm()
{
	MREAL	d1 = MgAbs( v);
		MGCHECK_ZEROVECTOR( d1, Mstr( "MgULine3 Norm"));
	MREAL	d2 = 1.f / d1;
	v.x *= d2;
	v.y *= d2;
	v.z *= d2;
	return *this;
}
*/
//
//======================( ３次元 )==============================
//		３次元直線同士が等しいか調べる
//
bool MgULine3::operator == ( const MgULine3& ULn1) const							// b = (uln == ULn1) 判定
{
	return ( MGeo::ChkPt3OnULn3( p, ULn1) &&
			( MGeo::EqualV( v,  ULn1.v) || MGeo::EqualV( v, -ULn1.v)));
}
//
//======================( ３次元 )==============================
//		３次元直線をトレースする
//
void MgULine3::Print( MCHAR* s)
{
#ifdef _DEBUG
	Msprintf( MbLog::m_Str, Mstr( "%s 	MgULine3	: (%7.1f,%7.1f,%7.1f), (%7.1f,%7.1f,%7.1f)\n"),
									s, p.x, p.y, p.z, v.x, v.y, v.z);
	MBTRCPRBF;
#endif
}
//
//======================( ３次元 )==============================
//		半直線の方向を単位ベクトルに正規化する
//
/*
MgHLine3 MgHLine3::Norm()
{
	MREAL	d1 = MgAbs( v);
		MGCHECK_ZEROVECTOR( d1, Mstr( "MgHLine3 Norm"));
	MREAL	d2 = 1.f / d1;
	v.x *= d2;
	v.y *= d2;
	v.z *= d2;
	return *this;
}
*/
//
//======================( ３次元 )==============================
//		３次元半直線同士が等しいか調べる
//
bool MgHLine3::operator == ( const MgHLine3 &HLn1) const							// b = (uln == ULn1) 判定
{
	return ( MGeo::ChkPt3OnHLn3( p, HLn1) &&
			( MGeo::EqualV( v,  HLn1.v)));
}
//
//======================( ３次元 )==============================
//		３次元半直線をトレースする
//
void MgHLine3::Print( MCHAR* s)
{
#ifdef _DEBUG
	Msprintf( MbLog::m_Str, Mstr( "%s 	MgHLine3	: (%7.1f,%7.1f,%7.1f), (%7.1f,%7.1f,%7.1f)\n"),
									s, p.x, p.y, p.z, v.x, v.y, v.z);
	MBTRCPRBF;
#endif
}
//
//======================( ３次元 )==============================
//		３次元線分同士が等しいか調べる
//
bool MgLine3::operator == ( const MgLine3& Ln1) const							// b = (ln == Ln1) 判定
{
	return (((p[0] == Ln1.p[0]) && (p[1] == Ln1.p[1])) ||
			((p[0] == Ln1.p[1]) && (p[1] == Ln1.p[0])));
}
//
//======================( ３次元 )==============================
//		３次元線分をトレースする
//
void MgLine3::Print( MCHAR* s)
{
#ifdef _DEBUG
	Msprintf( MbLog::m_Str, Mstr( "%s 	MgLine3 	: (%7.1f,%7.1f,%7.1f), (%7.1f,%7.1f,%7.1f)\n"),
									 s, p[0].x, p[0].y, p[0].z, p[1].x, p[1].y, p[1].z);
	MBTRCPRBF;
#endif
}

} // namespace MC
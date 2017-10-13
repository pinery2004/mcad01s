#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgMat.h
//
//		マトリックス
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
//

//
#include "MgRect.h"

class MgMat3;
class MgGPoint2;
class MgGPoint3;
class MgPolyg2;
class MgGPolyg2;
class MgGPolyg3;
class MgGLine2;
class MgGLine3;

#include "MbPlex.h"

#ifdef DLL_EXPORT_MAT
	#undef DLL_EXPORT_MAT
#endif

#ifdef DLL_EXPORT_MAT_DO
	#pragma message( "<<< MgMat・dll_EXport >>>")
	#define DLL_EXPORT_MAT							__declspec( dllexport)
#else
	#ifdef _DEBUG								// Release Mode で inline関数がundefになる不具合対応
		#ifdef DLL_NO_IMPORT_GEO_DO
			#define DLL_EXPORT_MAT
		#else
//			#pragma message( "=== MgMat・dll_IMport ===")
			#define DLL_EXPORT_MAT						__declspec( dllimport)
		#endif
	#else
		#define DLL_EXPORT_MAT
	#endif
#endif

namespace MC
{

//
//======================( ２次元 )==============================
//	２次元座標計算用３次元マトリックス
class DLL_EXPORT_MAT MgMat2
{
public:
	MREAL m[3][3];
//
public:
	MgMat2() {}
	MgMat2( MREAL i_m11, MREAL i_m12, MREAL i_m13,
		    MREAL i_m21, MREAL i_m22, MREAL i_m23,
		    MREAL i_m31, MREAL i_m32, MREAL i_m33) {
		m[0][0] = i_m11; m[0][1] = i_m12; m[0][2] = i_m13;
		m[1][0] = i_m21; m[1][1] = i_m22; m[1][2] = i_m23;
		m[2][0] = i_m31; m[2][1] = i_m32; m[2][2] = i_m33;}
	MgMat2( MREAL i_m[3][3]) {
		memcpy( m, i_m[0], SZMREAL( 9));}
	MgMat2( MREAL i_m[9]) {
		memcpy( m, i_m, SZMREAL( 9));}
	void SetUnit();																// 単位行列を設定する
	friend MgMat2 operator + ( const MgMat2& m1, const MgMat2& m2);				// +
	friend MgMat2 operator += ( MgMat2&, const MgMat2&);						// +=
	friend MgMat2 operator - ( const MgMat2&);									// -
	friend MgMat2 operator - ( const MgMat2&, const MgMat2&);					// -
	friend MgMat2 operator -= ( MgMat2&, const MgMat2&);						// -=
	friend MgMat2 operator * ( const MgMat2&, const MgMat2&);					// *
	friend MgMat2 operator *= ( MgMat2&, const MgMat2&);						// *=
//
	friend MgMat2 operator + ( const MgMat2 &Mat, const MgPoint2& Pt);			// +	平行移動
	friend MgMat2 operator += ( MgMat2 &Mat, const MgPoint2& Pt);				// +=	平行移動
//
	friend MgMat2 operator - ( const MgMat2 &Mat, const MgPoint2& Pt);			// -	平行移動
	friend MgMat2 operator -= ( MgMat2 &Mat, const MgPoint2& Pt);				// -=	平行移動
//	２Ｄ
	friend MgPoint2 operator * ( const MgPoint2&, const MgMat2&);				// *	座標変換
	friend MgPoint2 operator *= ( MgPoint2&, const MgMat2&);					// *=	座標変換
//
	friend MgULine2 operator * ( const MgULine2&, const MgMat2&);				// *	座標変換
	friend MgULine2 operator *= ( MgULine2&, const MgMat2&);					// *=	座標変換
//
	friend MgLine2 operator * ( const MgLine2& Ln1, const MgMat2 &Mat);			// *	座標変換
	friend MgLine2 operator *= ( MgLine2& Ln, const MgMat2 &Mat);				// *=	座標変換
//	３Ｄ
	friend MgPoint3 operator * ( const MgPoint3&, const MgMat2&);				// *	座標変換
	friend MgPoint3 operator *= ( MgPoint3&, const MgMat2&);					// *=	座標変換
//
	friend MgULine3 operator * ( const MgULine3&, const MgMat2&);				// *	座標変換
	friend MgULine3 operator *= ( MgULine3&, const MgMat2&);					// *=	座標変換
//
	friend MgLine3 operator * ( const MgLine3& Ln1, const MgMat2 &Mat);			// *	座標変換
	friend MgLine3 operator *= ( MgLine3& Ln, const MgMat2 &Mat);				// *=	座標変換
//
	void Print( MCHAR* s);														// print
	void MgMatPrint2(MCHAR* s);
};

inline	MgMat2 operator += ( MgMat2 &m1, const MgPoint2& Pt)				// +=	平行移動
						{ m1.m[2][0] += Pt.x; m1.m[2][1] += Pt.y;
						  return m1;}
inline	MgMat2 operator + ( const MgMat2 &m1, const MgPoint2& Pt)			// +	平行移動
						{ MgMat2 mw = m1; mw += Pt; return mw;}
inline	MgMat2 operator -= ( MgMat2 &m1, const MgPoint2& Pt)				// -=	平行移動
						{ m1.m[2][0] -= Pt.x; m1.m[2][1] -= Pt.y;}
inline	MgMat2 operator - ( const MgMat2 &m1, const MgPoint2& Pt)			// -	平行移動
						{ MgMat2 mw = m1; mw -= Pt; return mw;}
inline	MgLine2 operator * ( const MgLine2& Ln1, const MgMat2& m2)			// *	座標変換
						{ MgLine2 Ln;
						  Ln.p[0] = Ln1.p[0] * m2; Ln.p[1] = Ln1.p[1] * m2;
						  return Ln;}
inline	MgLine2 operator *= ( MgLine2& Ln, const MgMat2 &m2)				// *=	座標変換
						{ Ln.p[0] *= m2; Ln.p[1] *= m2;}
inline	MgLine3 operator * ( const MgLine3& Ln1, const MgMat2 &Mat)			// *	座標変換
						{ MgLine3 Ln;
						  Ln.p[0] = Ln1.p[0] * Mat; Ln.p[1] = Ln1.p[1] * Mat;
						  return Ln;}
inline	MgLine3 operator *= ( MgLine3& Ln, const MgMat2 &Mat)				// *=	座標変換
						{ Ln.p[0] *= Mat; Ln.p[1] *= Mat;}

inline	void MgMat2::Print( MCHAR* s)												// print
						{ MgMatPrint2( s);}
//
//======================( ３次元 )==============================
//	３次元座標計算用４次元マトリックス
//
class DLL_EXPORT_MAT MgMat3
{
public:
	MREAL m[4][4];

public:
	MgMat3() {}
	MgMat3( MREAL i_m11, MREAL i_m12, MREAL i_m13, MREAL i_m14,
		    MREAL i_m21, MREAL i_m22, MREAL i_m23, MREAL i_m24,
		    MREAL i_m31, MREAL i_m32, MREAL i_m33, MREAL i_m34,
		    MREAL i_m41, MREAL i_m42, MREAL i_m43, MREAL i_m44) {
		m[0][0] = i_m11; m[0][1] = i_m12; m[0][2] = i_m13; m[0][3] = i_m14;
		m[1][0] = i_m21; m[1][1] = i_m22; m[1][2] = i_m23; m[1][3] = i_m24;
		m[2][0] = i_m31; m[2][1] = i_m32; m[2][2] = i_m33; m[2][3] = i_m34;
		m[3][0] = i_m41; m[3][1] = i_m42; m[3][2] = i_m43; m[3][3] = i_m44;}
	MgMat3( MREAL i_m[4][4]) {
		memcpy( m, i_m[0], SZMREAL( 16));}
	MgMat3( MREAL i_m[16]) {
		memcpy( m, i_m, SZMREAL( 16));}
	MgMat3( MgVect3 i_V1, MgVect3 i_V2, MgVect3 i_V3) {
		m[0][0] = i_V1.x; m[0][1] = i_V2.x; m[0][2] = i_V3.x; m[0][3] = 0.0;
		m[1][0] = i_V1.y; m[1][1] = i_V2.y; m[1][2] = i_V3.y; m[1][3] = 0.0;
		m[2][0] = i_V1.z; m[2][1] = i_V2.z; m[2][2] = i_V3.z; m[2][3] = 0.0;
		m[3][0] = 0.0;	  m[3][1] = 0.0;	m[3][2] = 0.0;	  m[3][3] = 1.0;}
	void SetUnit();																// 単位行列を設定する
	friend MgMat3 operator + ( const MgMat3&, const MgMat3&);
	friend MgMat3 operator += ( MgMat3&, const MgMat3&);
	friend MgMat3 operator - ( const MgMat3&);
	friend MgMat3 operator - ( const MgMat3&, const MgMat3&);
	friend MgMat3 operator -= ( MgMat3&, const MgMat3&);
	friend MgMat3 operator * ( const MgMat3&, const MgMat3&);
	friend MgMat3 operator *= ( MgMat3&, const MgMat3&);
//
	friend MgMat3 operator + ( const MgMat3 &Mat, const MgPoint3& Pt);			// +	平行移動
	friend MgMat3 operator += ( MgMat3 &Mat, const MgPoint3& Pt);				// +=	平行移動
//
	friend MgMat3 operator - ( const MgMat3 &Mat, const MgPoint3& Pt);			// -	平行移動
	friend MgMat3 operator -= ( MgMat3 &Mat, const MgPoint3& Pt);				// -=	平行移動
//
	friend MgPoint2 operator * ( const MgPoint2&, const MgMat3&);				// *	座標変換
	friend MgPoint2 operator *= ( MgPoint2&, const MgMat3&);					// *=	座標変換
//
	friend MgPoint3 operator * ( const MgPoint3&, const MgMat3&);				// *	座標変換
	friend MgPoint3 operator *= ( MgPoint3&, const MgMat3&);					// *=	座標変換
//
	friend MgULine3 operator * ( const MgULine3&, const MgMat3&);				// *	座標変換
	friend MgULine3 operator *= ( MgULine3&, const MgMat3&);					// *=	座標変換
//
	friend MgLine3 operator * ( const MgLine3& Ln1, const MgMat3 &Mat);			// *	座標変換
	friend MgLine3 operator *= ( MgLine3& Ln, const MgMat3 &Mat1);				// *=	座標変換
//
	void Print( MCHAR* s);														// print
	void MgMatPrint3(MCHAR* s);
};

inline MgMat3 operator += ( MgMat3 &m1, const MgPoint3& Pt)						// +=	平行移動
						{ m1.m[3][0] += Pt.x; m1.m[3][1] += Pt.y;
						  m1.m[3][2] += Pt.z; return m1;} 
inline MgMat3 operator + ( const MgMat3 &m1, const MgPoint3& Pt)				// +	平行移動
						{ MgMat3 mw = m1; mw += Pt; return mw;}
inline MgMat3 operator -= ( MgMat3 &m1, const MgPoint3& Pt)						// -=	平行移動
						{ m1.m[3][0] -= Pt.x; m1.m[3][1] -= Pt.y;
						  m1.m[3][2] -= Pt.z; return m1;} 
inline MgMat3 operator - ( const MgMat3 &m1, const MgPoint3& Pt)				// -	平行移動
						{ MgMat3 mw = m1; mw -= Pt; return mw;}
inline MgLine3 operator * ( const MgLine3& Ln1, const MgMat3 &m2)				// 座標変換
						{ MgLine3 Ln; Ln.p[0] = Ln1.p[0] * m2;
						  Ln.p[1] = Ln1.p[1] * m2; return Ln;}
inline MgLine3 operator *= ( MgLine3& Ln1, const MgMat3 &m2)					// 座標変換
						{ Ln1.p[0] *= m2; Ln1.p[1] *= m2;}
inline	void MgMat3::Print( MCHAR* s)												// print
						{ MgMatPrint3( s);}

} // namespace MC
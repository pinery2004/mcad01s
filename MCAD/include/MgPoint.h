#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgPoint.h
//
//		点またはベクトルの定義、編集
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
//
#include "MgAngle.h"

#define		MgPoint2C		MgVect2C
#define		MgPoint3C		MgVect3C

namespace MC
{

class MgVect2;
class MgVect3;
class MgPlane3;

//typedef		MgVect2			MgPoint2;			// 2次元点
//typedef		MgVect3			MgPoint3;			// 3次元点

#define		MgPoint2		MgVect2
#define		MgPoint3		MgVect3

#include "MbPlex.h"

#ifdef DLL_EXPORT_POINT
	#undef DLL_EXPORT_POINT
#endif

#ifdef DLL_EXPORT_POINT_DO
	#pragma message( "<<< MgPoint・dll_EXport >>>")
	#define DLL_EXPORT_POINT							__declspec( dllexport)
#else
	#ifdef _DEBUG								// Release Mode で inline関数がundefになる不具合対応
		#ifdef DLL_NO_IMPORT_GEO_DO
//			#pragma message( "=== MgPoint・dll ===")
			#define DLL_EXPORT_POINT
		#else
//			#pragma message( "=== MgPoint・dll_IMport ===")
			#define DLL_EXPORT_POINT						__declspec( dllimport)
		#endif
	#else
		#define DLL_EXPORT_POINT
	#endif
#endif

//
//======================( ２次元 )==============================
//		２次元の点
//
class DLL_EXPORT_POINT MgVect2
{
public:
	union{
		MREAL v[1];
		MREAL x;
	};
	MREAL y;

public:
// コンストラクタ
	MgVect2()			{}

	MgVect2( MREAL x1, MREAL y2)
						{ x = x1; y = y2;}
	MgVect2( MREAL r, MgAngleR ra)  
						{ x = MREAL( r*cos( ra.a)); y = MREAL( r*sin( ra.a));}
	MgVect2( CPoint pt)
						{ x = MREAL(pt.x); y = MREAL(pt.y);}
	MgVect2 operator + ( const MgVect2& v) const;								// P1 = P2 + p3

	MgVect2 operator - ( const MgVect2& v) const;								// P1 = P2 - P3

	friend	MgVect2 operator - ( const MgVect2& p)								// P1 = - P2
				 		{ MgVect2 vto; vto.x = -p.x;
						  vto.y = -p.y; return vto;}
	MgVect2 operator += ( const MgVect2& v);									// P1 += P2

	MgVect2 operator -= ( const MgVect2& v);									// P1 -= P2

	friend	MgVect2 operator * ( MREAL r, const MgVect2& v)					// P1 = r * P2
				 		{ MgVect2 vto;vto.x = r * v.x;
						  vto.y = r * v.y; return vto;}
	MgVect2 operator * ( MREAL r) const;										// P1 = P2 * r

	MgVect2 operator / ( MREAL r) const;										// P1 = P2 / r

	MgVect2 operator *= ( MREAL r);											// P1 *= r

	MgVect2 operator /= ( MREAL r);											// P1 /= r

	MREAL operator * ( const MgVect2& v) const;								// r = P1 * P2 (内積 cos)

	MREAL operator ^ ( const MgVect2& v) const;								// P1 = P2 ^ P3 (外積 sin)

	MgVect2 operator & ( const MgVect2& v) const;								// P1 = P2 & P3 (rotation)

	bool operator == (const MgVect2& v) const;									// b = (P1 == P2) 判定

	bool operator != (const MgVect2& v) const;									// b = (P1 != P2) 判定

	MgVect2 Set( const MgVect2& v1)												// ２次元→２次元
						{ x = v1.x; y = v1.y; return *this;} 
	MgVect2 Set( MREAL x1, MREAL y2)
						{ x = x1; y = y2; return *this;}
	MgVect2 Set( const MgVect3& v);												// ３次元→２次元

	MgVect2 Set( MREAL r, MgAngleR ra)											// 半径、角度(ラジアン)
						{ x = MREAL( r*cos( ra.a)); y = MREAL( r*sin( ra.a)); return *this;}
	MgVect2 Set( MgAngleR ra)
						{ x = MREAL( cos( ra.a)); y = MREAL( sin( ra.a)); return *this;}

    MgVect2 Set( MREAL r, const MgAngleV &a)  
						{ x = r * a.x; y = r * a.y; return *this;}
	MgVect2 Set( const MgAngleV &a)
						{ x = a.x; y = a.y; return *this;}
	MgVect2 SetUnit()															// (1., 0.)に変更する
						{ x = 1; y = 0; return *this;}
	MgVect2 SetUnitize( MREAL i_tol = MgTol_Dt::D);							// 単位ベクトルに変換する
	
	MgVect2 SetRotR90()															// 右に９０度回転したベクトルに変換する
						{ MREAL xs = x; x = y; y = -xs; return *this;}
	MgVect2 SetRotL90()															// 左に９０度回転したベクトルに変換する
						{ MREAL xs = x; x = -y; y = xs; return *this;}
	MgVect2 SetRot180()															// １８０度回転したベクトルに変換する
						{ x = -x; y = -y; return *this;}
	MgVect2 SetRot( const MgVect2& v)											// 単位ベクトル方向に回転したベクトルに変換する
						{ MgVect2 vt = *this; 
						  x = vt.x * v.x -  vt.y * v.y;
						  y = vt.x * v.y +  vt.y * v.x;
						  return *this;}
	MgVect2 Unitize( MREAL i_tol = MgTol_Dt::D) const;							// 単位ベクトルを求める

	MgVect2 RotR90() const;														// 右に９０度回転したベクトルを求める

	MgVect2 RotL90() const;														// 左に９０度回転したベクトルを求める

	MgVect2 Rot180() const;														// １８０度回転したベクトルを求める

	MgVect2 Rot( const MgVect2& v) const;										// 単位ベクトル方向に回転したベクトルを求める

	MgVect2& operator = ( const CPoint &pt)
						{ x = MREAL(pt.x); y = MREAL(pt.y); return *this;}
	MgVect2& operator = ( const MREAL* pt)
						{ x = pt[0]; y = pt[1]; return *this;}
	MgVect2& operator = ( const MgAngleV &a)
						{ x = a.x; y = a.y; return *this;}
	MgVect2& operator = ( const MgAngleR &ra)
						{ x = MREAL( cos( ra.a)); y = MREAL( sin( ra.a)); return *this;}
	void Set( MREAL* rP)
						{ x = rP[0]; y = rP[1];} 
	void Get( MREAL* rP) const
						{ rP[0] = x; rP[1] = y;} 
	void Print(MCHAR* s) const;														// トレース
};

//
//======================( ３次元 )==============================
//		３次元の点（ベクトル）
//
class DLL_EXPORT_POINT MgVect3
{
public:
	union{
		MREAL v[1];
		MREAL x;
	};
	MREAL y;
	MREAL z;

public:
// コンストラクタ
	MgVect3()			{}

	MgVect3( MREAL x1, MREAL y2, MREAL z3)
						{ x = x1; y = y2; z = z3;}
	MgVect3( MgVect2 v1, MREAL z3)
						{ x = v1.x; y = v1.y; z = z3;}
	MgVect3( MREAL r, MgAngleR ra1, MgAngleR ra2)								// 半径、緯度(ラジアン)、経度(ラジアン)
						{ x = MREAL(r*cos(ra1.a) * cos(ra2.a)); 
						  y = MREAL(r*sin(ra1.a) * cos(ra2.a)); 
						  z = MREAL(r*sin(ra2.a));}
	MgVect3( MREAL r, const MgAngleV &a1, const MgAngleV &a2)					// 半径、緯度(単位ベクトル)、経度(単位ベクトル)  
						{ x = r * a1.x * a2.x; 
						  y = r * a1.y * a2.x; 
						  z = r * a2.y;}

	MgVect3 operator + ( const MgVect3& v) const;								// P1 = P2 + p3

	MgVect3 operator - ( const MgVect3& p) const;								// P1 = P2 - P3

	friend	MgVect3 operator - ( const MgVect3& v)								// P1 = - P2
				 		{ MgVect3 Vto; Vto.x =-v.x; Vto.y = -v.y;
						  Vto.z = -v.z; return Vto;}
	MgVect3 operator += ( const MgVect3& v);									// P1 += P2

	MgVect3 operator -= ( const MgVect3& v);									// P1 -= P2

	friend	MgVect3 operator * ( MREAL r, const MgVect3& v)					// P1 = r * P2
				 		{ MgVect3 Vto; Vto.x =r * v.x; Vto.y = r * v.y;
						  Vto.z = r * v.z; return Vto;}
	MgVect3 operator * (  MREAL r)	const;										// P1 = P2 * r

	MgVect3 operator / (  MREAL r) const;										// P1 = P2 / r

	MgVect3 operator *= ( MREAL r);											// P1 *= r

	MgVect3 operator /= ( MREAL r);											// P1 /= r

	MREAL operator * ( const MgVect3& v) const;								// r = P1 * P2 (内積 cos)

	MgVect3 operator ^ ( const MgVect3& v) const;								// P1 = P2 ^ P3 (外積 sin)

	bool operator == ( const MgVect3& v) const;									// b = (P1 == P2) 判定

	bool operator != ( const MgVect3& v) const;									// b = (P1 != P2) 判定

	MgVect3 Set( const MgVect3& v1)												// ３次元→３次元
						{ x = v1.x; y = v1.y; z = v1.z; return *this;} 
	MgVect3 Set( const MREAL x1, const MREAL y2, const MREAL z3)				// 直交座標の３次元ベクトルを設定する
						{ x = x1; y = y2; z = z3; return *this;}
	MgVect3 Set( const MREAL r[3])												// 実数配列のベクトルに相当する３次元ベクトルを設定する
						{ x = r[0]; y = r[1]; z = r[2]; return *this;}
	MgVect3 Set( const MgVect2& v1, MREAL z2 = 0.)								// ２次元→３次元　(Z省略0.)
						{ x = v1.x; y = v1.y; z = z2; return *this;} 
	MgVect3 Set( const MgVect2& v1, const MgPlane3& Pln3);						// ２次元→３次元

    MgVect3 Set( MREAL r, MgAngleR ra1, MgAngleR ra2)							// 半径、緯度(ラジアン)、経度(ラジアン)
						{ x = MREAL(r*cos(ra1.a) * cos(ra2.a)); 
						  y = MREAL(r*sin(ra1.a) * cos(ra2.a)); 
						  z = MREAL(r*sin(ra2.a)); return *this;}
    MgVect3 Set( MREAL r, const MgAngleV &a1, const MgAngleV &a2)				// 半径、緯度(単位ベクトル)、経度(単位ベクトル)  
						{ x = r * a1.x * a2.x; 
						  y = r * a1.y * a2.x; 
						  z = r * a2.y; return *this;}
	MgVect3 SetUnit()
						{ x = 1; y = 0; z = 0; return *this;}					// (1., 0., 0.)に変更する
	MgVect3 SetUnitize( MREAL i_tol = MgTol_Dt::D);							// 単位ベクトルに変換する

	MgVect3 SetRotR90()															// 右に９０度回転したベクトルに変換する
						{ MREAL xs = x; x = y; y = -xs; return *this;}
	MgVect3 SetRotL90()															// 左に９０度回転したベクトルに変換する
						{ MREAL xs = x; x = -y; y = xs; return *this;}
	MgVect3 SetRot180()															// １８０度回転したベクトルに変換する
						{ x = -x; y = -y; return *this;}
	MgVect3 SetRot( const MgVect2& v)											// 単位ベクトル方向に回転したベクトルに変換する
						{ MgVect3 vt = *this; 
						  x = vt.x * v.x -  vt.y * v.y;
						  y = vt.x * v.y +  vt.y * v.x;
						  return *this;}
	MgVect3 Unitize( MREAL i_tol = MgTol_Dt::D) const;							// 単位ベクトルを求める

	MgVect3 RotR90() const;														// 右に９０度回転したベクトルを求める

	MgVect3 RotL90() const;														// 左に９０度回転したベクトルを求める

	MgVect3 Rot180() const;														// １８０度回転したベクトルを求める

	MgVect3 Rot( const MgVect2& v) const;										// 単位ベクトル方向に回転したベクトルを求める

	MgVect3& operator = ( const MREAL* pt)
						{ x = pt[0]; y = pt[1]; z = pt[2]; return *this;}
	void	Set( MREAL* rP)
						{ x = rP[0]; y = rP[1]; z = rP[2];} 
	void	Get( MREAL* rP) const
						{ rP[0] = x; rP[1] = y; rP[2] = z;} 
	void	Print(MCHAR* s) const;												// トレース
};
//
//======================( ２次元 )==============================
//		２次元ベクトル演算
//
inline MgVect2 MgVect2::operator + ( const MgVect2& v) const					// P1 = P2 + p3
				 		{ return MgVect2( x + v.x, y + v.y);}
inline MgVect2 MgVect2::operator - ( const MgVect2& v) const					// P1 = P2 - P3
				 		{ return MgVect2( x - v.x, y - v.y);}
inline MgVect2 MgVect2::operator += ( const MgVect2& v)							// P1 += P2
				 		{ return MgVect2( x += v.x, y += v.y);}
inline MgVect2 MgVect2::operator -= ( const MgVect2& v)							// P1 -= P2
				 		{ return MgVect2( x -= v.x, y -= v.y);}
inline MgVect2 MgVect2::operator * ( MREAL r) const							// P1 = P2 * r
				 		{ return MgVect2( x * r, y * r);}
inline MgVect2 MgVect2::operator / ( MREAL r) const							// P1 = P2 / r
				 		{ return MgVect2( x / r, y / r);}
inline MgVect2 MgVect2::operator *= ( MREAL r)									// P1 *= r
				 		{ return MgVect2( x *= r, y *= r);}
inline MgVect2 MgVect2::operator /= ( MREAL r)									// P1 /= r
				 		{ return MgVect2( x /= r, y /= r);}
inline MREAL MgVect2::operator * ( const MgVect2& v) const						// r = P1 * P2 (内積 cos)
				 		{ return ( x * v.x + y * v.y);}
inline MREAL MgVect2::operator ^ ( const MgVect2& v) const						// r = P2 ^ P3 (外積 sin)
				 		{ return ( x * v.y -  y * v.x);}
inline MgVect2 MgVect2::operator & ( const MgVect2& v) const					// P1 = P2 & P3 (rotation)
				 		{ return MgVect2( x * v.x -  y * v.y,
		 			               		  x * v.y +  y * v.x);}
inline bool MgVect2::operator == (const MgVect2& v) const						// b = (P1 == P2) 判定
				 		{ MgVect2 v1 = v - *this; return (v1 * v1) <= g_gTol.D_2;}
inline bool MgVect2::operator != (const MgVect2& v) const						// b = (P1 != P2) 判定
				 		{ return !((*this) == v);}

inline MgVect2 MgVect2::RotR90() const											// 右に９０度回転したベクトルを求める
						{ return MgVect2( y, -x);}
inline MgVect2 MgVect2::RotL90() const											// 左に９０度回転したベクトルを求める
						{ return MgVect2( -y, x);}
inline MgVect2 MgVect2::Rot180() const											// １８０度回転したベクトルを求める
						{ return MgVect2( -x, -y);}
inline MgVect2 MgVect2::Rot( const MgVect2& v) const							// 単位ベクトル方向に回転したベクトルを求める
						{ return MgVect2( x * v.x -  y * v.y,
		 			               		  x * v.y +  y * v.x);}

inline MgVect2 MgVect2C( const MgVect3& v)										// ３次元(x,y)→２次元
						{ MgVect2 vto; vto.x=v.x; vto.y=v.y; return vto;}

inline MgVect2 MgVect2C( const CPoint &cp)										// CPoint→２次元
						{ MgVect2 vto; vto.x = MREAL( cp.x);
						  vto.y = MREAL( cp.y); return vto;}

inline MgVect2 MgVect2C( MgAngleR ra)
						{ MgVect2 vto; vto.x = MREAL( cos( ra.a));
						  vto.y = MREAL( sin( ra.a)); return vto;}

inline MgVect2 MgVect2C( MREAL r, MgAngleR ra)									// 半径、角度(ラジアン)
						{ MgVect2 vto; vto.x = MREAL( r*cos( ra.a));
						  vto.y = MREAL( r*sin( ra.a)); return vto;}

inline MgVect2 MgVect2C( const MgAngleV &a)
						{ MgVect2 vto; vto.x = a.x; vto.y = a.y; return vto;}

inline MgVect2 MgVect2C( MREAL r, const MgAngleV &a)  
						{ MgVect2 vto; vto.x = r * a.x; vto.y = r * a.y; return vto;}

//skip
inline CPoint MgCPointC(MgVect2& v)
						{ CPoint cp; cp.x = MINT( v.x + 0.5);
						  cp.y = MINT( v.y + 0.5); return cp;}

inline MgVect2 MgVect2AngR( MREAL rad)
						{ return MgVect2( (MREAL)cos( rad), (MREAL)sin( rad));}

inline MgVect2 MgVect2AngD( MREAL deg)
						{ MREAL rad = deg * MC_DTR; return MgVect2( (MREAL)cos( rad), (MREAL)sin( rad));}

inline MgVect2 MgVect2::Set( const MgVect3& v)									// ３次元(x,y)→２次元
						{ x=v.x, y=v.y; return *this;}

//======================( ３次元 )==============================
//		３次元ベクトル演算
//
//inline MgVect3 MgVect3( MREAL x1, MREAL y2, MREAL z3)
//						{ MgVect3 vto; vto.x = x1; vto.y = y2;
//						  vto.z = z3; return vto;}
inline MgVect3 MgVect3C( const MgVect2& v1, MREAL z2 = 0.)						// ２次元→３次元　(Z省略0.)
						{ MgVect3 vto; vto.x = v1.x; vto.y = v1.y;
						  vto.z = z2; return vto;} 
inline MgVect3 MgVect3C( const MgVect2& v1, const MgPlane3& Pln3);				// ２次元→３次元

inline MgVect3 MgVect3C( MREAL r, MgAngleR ra1, MgAngleR ra2)					// 半径、緯度(ラジアン)、経度(ラジアン)
						{ MgVect3 vto; vto.x = MREAL(r*cos(ra1.a) * cos(ra2.a)); 
								  vto.y = MREAL(r*sin(ra1.a) * cos(ra2.a)); 
								  vto.z = MREAL(r*sin(ra2.a)); return vto;}
inline MgVect3 MgVect3C( MREAL r, const MgAngleV &a1, const MgAngleV &a2)		// 半径、緯度(単位ベクトル)、経度(単位ベクトル)  
						{ MgVect3 vto; vto.x = r * a1.x * a2.x; 
									   vto.y = r * a1.y * a2.x; 
									   vto.z = r * a2.y; return vto;}
//inline MgVect3 MgVect3::operator = ( const MgVect2& v)
//						{ return MgVect3(x = v.x, y = v.y, z = 0.);}			// ２次元 → ３次元　(Z=0.)
inline MgVect3 MgVect3::operator + ( const MgVect3& v) const					// P1 = P2 + p3
				 		{ return MgVect3( x + v.x, y + v.y, z + v.z);}
inline MgVect3 MgVect3::operator - ( const MgVect3& p) const					// P1 = P2 - P3
				 		{ return MgVect3(x - p.x, y - p.y, z - p.z);}
inline MgVect3 MgVect3::operator += ( const MgVect3& v)							// P1 += P2
				 		{ return MgVect3( x += v.x, y += v.y, z += v.z);}
inline MgVect3 MgVect3::operator -= ( const MgVect3& v)							// P1 -= P2
				 		{ return MgVect3( x -= v.x, y -= v.y, z -= v.z);}
inline MgVect3 MgVect3::operator * (  MREAL r)	const							// P1 = P2 * r
				 		{ return MgVect3( x * r, y * r, z * r);}
inline MgVect3 MgVect3::operator / (  MREAL r) const							// P1 = P2 / r
				 		{ return MgVect3( x / r, y / r, z / r);}
inline MgVect3 MgVect3::operator *= ( MREAL r)									// P1 *= r
				 		{ return MgVect3( x *= r, y *= r, z *= r);}
inline MgVect3 MgVect3::operator /= ( MREAL r)									// P1 /= r
				 		{ return MgVect3( x /= r, y /= r, z /= r);}
inline MREAL MgVect3::operator * ( const MgVect3& v) const						// r = P1 * P2 (内積 cos)
				 		{ return ( x * v.x + y * v.y + z * v.z);}
inline MgVect3 MgVect3::operator ^ ( const MgVect3& v) const					// P1 = P2 ^ P3 (外積 sin)
				 		{ return MgVect3( y * v.z - z * v.y,
						                  z * v.x - x * v.z,
								          x * v.y - y * v.x);}
inline bool MgVect3::operator == ( const MgVect3& v) const						// b = (P1 == P2) 判定
				 		{ MgVect3 v1 = v - *this; return ((v1 * v1) <= g_gTol.D_2);}
inline bool MgVect3::operator != ( const MgVect3& v) const						// b = (P1 != P2) 判定
				 		{ return !((*this) == v);}

inline MgVect3 MgVect3::RotR90() const											// 右に９０度回転したベクトルを求める
						{ return MgVect3( y, -x, z);}
inline MgVect3 MgVect3::RotL90() const											// 左に９０度回転したベクトルを求める
						{ return MgVect3( -y, x, z);}
inline MgVect3 MgVect3::Rot180() const											// １８０度回転したベクトルを求める
						{ return MgVect3( -x, -y, z);}
inline MgVect3 MgVect3::Rot( const MgVect2& v) const							// 単位ベクトル方向に回転したベクトルを求める
						{ return MgVect3( x * v.x -  y * v.y,
		 			               		  x * v.y +  y * v.x, z);}

/*
//
//#ifdef _MGVectA										
//
//======================( ２次元 )==============================
//		２次元の点（単位ベクトル，絶対値）
//
class DLL_EXPORT_POINT MgVect2a
{
	union{
		MREAL v[1];
		MREAL x;
	};
	MREAL y;
	MREAL a;
public:
	MgVect2a( MREAL x1, MREAL y2, MREAL a3)									// =
						{ x=x; y=y2; a=a3;}
	MgVect2a()
						{ x=1.; y=0.; a=1.;}

	void Print(MCHAR* s)														// トレース
				 		{ 
#ifdef _DEBUG
						Msprintf( MbLog::m_Str, Mstr( "MgVect2a  %s =	%f, %f, %f\n"), s, x, y, a);
						  MBTRCPRBF;}
#endif
};
MgVect2a MgNormaV2a( const MgVect2&);
//#endif
//
//#ifdef _MgVect3A
//
//======================( ３次元 )==============================
//		３次元の点（単位ベクトル，絶対値）
//
class DLL_EXPORT_POINT MgVect3a
{
public:
	union{
		MREAL v[1];
		MREAL x;
	};
	MREAL y;
	MREAL z;
	MREAL a;
//
	MgVect3a( MREAL x1, MREAL y2, MREAL z3, MREAL a4)						// =
						{ x=x1; y=y2; z=z3; a=a4;}
	MgVect3a()
						{ x=1.; y=0.; z = 0.; a=1.;}
	void Print( MCHAR* s)														// トレース
				 		{ 
#ifdef _DEBUG
						Msprintf( MbLog::m_Str, Mstr( "MgPoint3a %s	= %f, %f, %f, %f\n"), s, x, y, z, a);
						  MBTRCPRBF;};
#endif
};
//
MgVect3a MgNormaV3a( const MgVect3&);
//#endif
*/

// サイズ
inline MINT SZMgPoint2( MINT i_sz = 1)	{ return  ( i_sz * (MINT)sizeof( MgPoint2));}
inline MINT SZMgPoint3( MINT i_sz = 1)	{ return  ( i_sz * (MINT)sizeof( MgPoint3));}
inline MINT SZMgVect2( MINT i_sz = 1)	{ return  ( i_sz * (MINT)sizeof( MgVect2));}
inline MINT SZMgVect3( MINT i_sz = 1)	{ return  ( i_sz * (MINT)sizeof( MgVect3));}
inline MINT WSZMgPoint2( MINT i_sz = 1)	{ return  ( i_sz * (MINT)sizeof( MgPoint2) / SZMINT());}
inline MINT WSZMgPoint3( MINT i_sz = 1)	{ return  ( i_sz * (MINT)sizeof( MgPoint3) / SZMINT());}
inline MINT WSZMgVect2( MINT i_sz = 1)	{ return  ( i_sz * (MINT)sizeof( MgVect2) / SZMINT());}
inline MINT WSZMgVect3( MINT i_sz = 1)	{ return  ( i_sz * (MINT)sizeof( MgVect3) / SZMINT());}

} // namespace MC
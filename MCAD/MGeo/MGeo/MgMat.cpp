//==========================================================================================
//	Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//	MODULE: MgMat.cpp
//
//	マトリックスの計算
//
//　本関数を使用するプログラムは、マトリックス変換の前処理は左側に後処理は右側に記述する。
//
//	K.Matsu           08/01/04    Created.
//==========================================================================================
//
#include "stdafx.h"
#include "MbBasic.h"
#include "MbDebug.h"

#define	DLL_EXPORT_MAT_DO

#include "MgMat.h"

#define	DLL_EXPORT_GEO_DO

#include "MgLib.h"

namespace MC
{

static MgMat2 McMatN  = MgMat2 ( 1.0f, 0.0f, 0.0f,
						 		 0.0f, 1.0f, 0.0f,
								 0.0f, 0.0f, 1.0f);
static MgMat3 McMat3N = MgMat3( 1.0f, 0.0f, 0.0f, 0.0f,
								0.0f, 1.0f, 0.0f, 0.0f,
								0.0f, 0.0f, 1.0f, 0.0f,
								0.0f, 0.0f, 0.0f, 1.0f);

//
void MgMat2::SetUnit() {*this = McMatN;}
void MgMat3::SetUnit() {*this = McMat3N;}

//// 指定軸を中心に回転する
//MgRot( MgULine3& i_UL, MgVect3& i_VR)
//{
//	MREAL	Vx2 = i_UL.v.x * i_UL.v.x;
//	MREAL	Vy2 = i_UL.v.y * i_UL.v.y;
//	MREAL	Vz2 = i_UL.v.z * i_UL.v.z;
//	MREAL	Vxy = i_UL.v.x * i_UL.v.y;
//	MREAL	Vyz = i_UL.v.y * i_UL.v.z;
//	MREAL	Vzx = i_UL.v.z * i_UL.v.x;
//	MREAL	VRmc = 1 - i_VR.x;
//	MgMat3	M1;
//	M1.m[0][0]
//}

//===========================================================================
//
//	２Ｄ座標変換マトリックスの計算
//
//	| m11 m12 m13 |
//	| m21 m22 m23 |
//	| m31 m32 m33 |
//
//	| ↓　↓　↓ |     | →　→　→ |
//	| ↓　↓　↓ |  X  | →　→　→ |
//	| dx　dy　↓ |     | dx  dy  → |
//
//
//	|  x　 y　↓ |     | →　→　→ |
//				　  X  | →　→　→ |
//					   | dx  dy　→ |
//
//===========================================================================
//	処理内容
//	    座標変換マトリックスの加算
//
//	使用法
//		MgMat3 = MgMat21 + MgMat22;
//
//	| m11 m12 m13 |     | n11 n12 n13 |
//	| m21 m22 m23 |  +  | n21 n22 n23 |
//	| m31 m32 m33 |     | n31 n32 n33 |
//
//MgMat2 operator + (const MgMat2 &tm, const MgMat2 &tn)
MgMat2 MGeo::MatPlus2( const MgMat2& i_tm, const MgMat2& i_tn)
{
	MgMat2	to;
	int		ic;
	for ( ic=0; ic<3; ic++) {
		to.m[ic][0] = i_tm.m[ic][0] + i_tn.m[ic][0];
		to.m[ic][1] = i_tm.m[ic][1] + i_tn.m[ic][1];
		to.m[ic][2] = i_tm.m[ic][2] + i_tn.m[ic][2];
	}
	return to;
}
//
//===========================================================================
//	処理内容
//		座標変換マトリックスの加算
//
//	使用法
//		MgMat21 += MgMat22;
//
//	| m11 m12 m13 |     | m11 m12 m13 |     | n11 n12 n13 |
//	| m21 m22 m23 |  =  | m21 m22 m23 |  +  | n21 n22 n23 |
//	| m31 m32 m33 |     | m31 m32 m33 |     | n31 n32 n33 |
//
//MgMat2 operator += (MgMat2 &tm, const MgMat2 &tn)
MgMat2 MGeo::MatPlusEqual2( MgMat2& io_tm, const MgMat2& i_tn)
{
	int		ic;
	for ( ic=0; ic<3; ic++) {
		io_tm.m[ic][0] += i_tn.m[ic][0];
		io_tm.m[ic][1] += i_tn.m[ic][1];
		io_tm.m[ic][2] += i_tn.m[ic][2];
	}
	return io_tm;
}
//
//===========================================================================
//	処理内容
//		負の座標変換マトリックスを作成
//
//	使用法
//		MgMat22 = - MgMat21;
//
//	  | m11 m12 m13 |
//	- | m21 m22 m23 |
//	  | m31 m32 m33 |
//
//MgMat2 operator - (const MgMat2 &tm)
MgMat2 MGeo::MatMinusSingle2( const MgMat2& i_tm)
{
	return MgMat2( -i_tm.m[0][0], -i_tm.m[0][1], -i_tm.m[0][2],
		           -i_tm.m[1][0], -i_tm.m[1][1], -i_tm.m[1][2],
				   -i_tm.m[2][0], -i_tm.m[2][1], -i_tm.m[2][2]);
}
//
//===========================================================================
//	処理内容
//		座標変換マトリックスの引算
//
//	使用法
//		MgMat3 = MgMat21 - MgMat22;
//
//	  | m11 m12 m13 |     | n11 n12 n13 |
//	  | m21 m22 m23 |  -  | n21 n22 n23 |
//	  | m31 m32 m33 |     | n31 n32 n33 |
//
//MgMat2 operator - (const MgMat2 &M1, const MgMat2 &M2)
MgMat2 MGeo::MatMinus2( const MgMat2& i_tm, const MgMat2& i_tn)
{
	MgMat2	to;
	int		ic;
	for ( ic=0; ic<3; ic++) {
		to.m[ic][0] = i_tm.m[ic][0] - i_tn.m[ic][0];
		to.m[ic][1] = i_tm.m[ic][1] - i_tn.m[ic][1];
		to.m[ic][2] = i_tm.m[ic][2] - i_tn.m[ic][2];
	}
	return to;
}
//
//===========================================================================
//	処理内容
//	    座標変換マトリックスの引算
//
//	使用法
//		MgMat21 -= MgMat22;
//
//	| m11 m12 m13 |    | m11 m12 m13 |     | n11 n12 n13 |
//	| m21 m22 m23 |  = | m21 m22 m23 |  -  | n21 n22 n23 |
//	| m31 m32 m33 |    | m31 m32 m33 |     | n31 n32 n33 |
//
//MgMat2 operator -= (MgMat2 &tm, const MgMat2 &tn)
MgMat2 MGeo::MatMinusEqual2( MgMat2& io_tm, const MgMat2& i_tn)
{
	int		ic;
	for ( ic=0; ic<3; ic++) {
		io_tm.m[ic][0] -= i_tn.m[ic][0];
		io_tm.m[ic][1] -= i_tn.m[ic][1];
		io_tm.m[ic][2] -= i_tn.m[ic][2];
	}
	return io_tm;
}
//
//===========================================================================
//	処理内容
//		座標変換マトリックスのかけ算
//	    座標変換マトリックスによる変換後に、さらに座標変換マトリックスによる変換するマトリックスを作成
//
//	使用法
//		MgMat3 = MgMat21 * MgMat22;
//
//	| m11 m12 m13 |     | n11 n12 n13 |
//	| m21 m22 m23 |  X  | n21 n22 n23 |
//	| m31 m32 m33 |     | n31 n32 n33 |
//
//MgMat2 operator * (const MgMat2 &tm, const MgMat2 &tn)
MgMat2 MGeo::MatMult2( const MgMat2& i_tm, const MgMat2& i_tn)
{
	MgMat2	to;
	int		i1, i2;
	for (i1=0; i1<3; i1++) 
		for (i2=0; i2<3; i2++) 
			to.m[i1][i2] = i_tm.m[i1][0] * i_tn.m[0][i2] +
						   i_tm.m[i1][1] * i_tn.m[1][i2] +
						   i_tm.m[i1][2] * i_tn.m[2][i2]	;
	return to;
}
//
//===========================================================================
//	処理内容
//	    座標変換マトリックスのかけ算
//	    座標変換マトリックスによる変換後に、さらに座標変換マトリックスによる変換するマトリックスを作成
//
//	使用法
//		MgMat22 *= MgMat21;
//
//	| m11 m12 m13 |     | m11 m12 m13 |     | n11 n12 n13 |
//	| m21 m22 m23 |  =  | m21 m22 m23 |  X  | n21 n22 n23 |
//	| m31 m32 m33 |     | m31 m32 m33 |     | n31 n32 n33 |
//
//MgMat2 operator *= (MgMat2 &tm, const MgMat2 &tn)
MgMat2 MGeo::MatMultEqual2( MgMat2& io_tm, const MgMat2& i_tn)
{
	MgMat2	tw;
	int		i1, i2;
	for (i1=0; i1<3; i2++) 
		for (i2=0; i2<3; i1++) 
			tw.m[i1][i2] = io_tm.m[i1][0] * i_tn.m[0][i2] +
						   io_tm.m[i1][1] * i_tn.m[1][i2] +
						   io_tm.m[i1][2] * i_tn.m[2][i2]	;
	io_tm = tw;
	return io_tm;
}
//
//===========================================================================
//	処理内容
//	    座標変換マトリックスにより座標変換する
//
//	使用法
//		MgPoint2po = MgPoint2p * MgMat2tm;
//
//					   | m11 m12 m13 |
//	| px  py  1  |  X  | m21 m22 m23 |
//					   | m31 m32 m33 |     
//
//MgPoint2 operator * (const MgPoint2& pi, const MgMat2 &tm)
MgPoint2 MGeo::MatMultPt22( const MgPoint2& i_pt, const MgMat2& i_tm)
{
	MgPoint2	po;
	po.x = i_tm.m[0][0] * i_pt.x + i_tm.m[1][0] * i_pt.y + i_tm.m[2][0];
	po.y = i_tm.m[0][1] * i_pt.x + i_tm.m[1][1] * i_pt.y + i_tm.m[2][1];
	return po;
}
//
//===========================================================================
//	処理内容
//		座標変換マトリックスにより座標変換する
//
//	使用法
//		MgPoint2p *= MgMat2m;
//
//										  | m11 m12 m13 |
//	| px  py  1  |  =  | px  py  1  |  X  | m21 m22 m23 |
//				   						  | m31 m32 m33 |
//
//MgPoint2 operator *= (MgPoint2& pi, const MgMat2& tm)
MgPoint2 MGeo::MatMultEqualPt22( MgPoint2& io_pt, const MgMat2& i_tm)
{
	MREAL	rwx;
	rwx	    = i_tm.m[0][0] * io_pt.x + i_tm.m[1][0] * io_pt.y + i_tm.m[2][0];
	io_pt.y = i_tm.m[0][1] * io_pt.x + i_tm.m[1][1] * io_pt.y + i_tm.m[2][1];
	io_pt.x = rwx;
	return io_pt;
}
//
//===========================================================================
//	処理内容
//		座標変換マトリックスにより座標変換する
//
//	使用法
//		MgULine2 = MgULine2*  MgMat2;
//
//	| px  py  1  |     | m11 m12 m13 |
//	| vx  vy  0  |  X  | m21 m22 m23 |
//					   | m31 m32 m33 |
//
//MgULine2 operator * (const MgULine2& uln1, const MgMat2 &m2)
MgULine2 MGeo::MatMultULn22( const MgULine2& i_uln, const MgMat2& i_tm)
{
	MgULine2	ulno;
	ulno.p.x = i_tm.m[0][0] * i_uln.p.x + i_tm.m[1][0] * i_uln.p.y + i_tm.m[2][0];
	ulno.p.y = i_tm.m[0][1] * i_uln.p.x + i_tm.m[1][1] * i_uln.p.y + i_tm.m[2][1];

	ulno.v.x = i_tm.m[0][0] * i_uln.v.x + i_tm.m[1][0] * i_uln.v.y;
	ulno.v.y = i_tm.m[0][1] * i_uln.v.x + i_tm.m[1][1] * i_uln.v.y;
	return ulno;
}
//
//===========================================================================
//	処理内容
//		座標変換マトリックスにより座標変換する
//
//	使用法
//		MgULine2* = MgMat2;
//
//	| px  py  1  |     | px  py  1  |     | m11 m12 m13 |
//	| vx  vy  0  |  =  | vx  vy  1  |  X  | m21 m22 m23 |
//			             				  | m31 m32 m33 |
//
//MgULine2 operator *= (MgULine2& ili, const MgMat2 &m2)
MgULine2 MGeo::MatMultEqualULn22( MgULine2& io_uln, const MgMat2& i_tm)
{
	MREAL	rwx;
	rwx		   = i_tm.m[0][0] * io_uln.p.x + i_tm.m[1][0] * io_uln.p.y + i_tm.m[2][0];
	io_uln.p.y = i_tm.m[0][1] * io_uln.p.x + i_tm.m[1][1] * io_uln.p.y + i_tm.m[2][1];
	io_uln.p.x = rwx;

	rwx		   = i_tm.m[0][0] * io_uln.v.x + i_tm.m[1][0] * io_uln.v.y;
	io_uln.v.y = i_tm.m[0][1] * io_uln.v.x + i_tm.m[1][1] * io_uln.v.y;
	io_uln.v.x = rwx;

	return io_uln;
}
//===============( ２次元座標変換マトリックスで３次元座標処理 )==============
//
//===========================================================================
//	処理内容
//	    座標変換マトリックスにより座標変換する
//
//	使用法
//		MgPoint32 = MgPoint31 * MgMat21;
//
//						  | m11 m12 0 m13 |     
//	| px  py  pz  1 |  X  | m21 m22 0 m23 |
//						  |  0   0  1  0  |
//						  | m31 m32 0 m33 |
//
//MgPoint3 operator * (const MgPoint3& Pt1, const MgMat2 &m2)
MgPoint3 MGeo::MatMultPt32( const MgPoint3& i_Pt, const MgMat2& i_tm)
{
	MgPoint3	Po;
	Po.x = i_tm.m[0][0] * i_Pt.x + i_tm.m[1][0] * i_Pt.y + i_tm.m[2][0];
	Po.y = i_tm.m[0][1] * i_Pt.x + i_tm.m[1][1] * i_Pt.y + i_tm.m[2][1];
	Po.z = i_Pt.z;
	return Po;
}
//
//===========================================================================
//	処理内容
//		座標変換マトリックスにより座標変換する
//
//	使用法
//		MgPoint3 *= MgMat2;
//
//						 						| m11 m12 0 m13 |
//	| px  py  pz  1 |  =  | px  py  pz  1 |  X  | m21 m22 0 m23 |
//			   									|  0   0  1  0  |
//			   									| m31 m32 0 m33 |


//MgPoint3 operator *= (MgPoint3& Pt1, const MgMat2 &m2)
MgPoint3 MGeo::MatMultEqualPt32( MgPoint3& io_Pt, const MgMat2& i_tm)
{
	MREAL	rwx;
	rwx		= i_tm.m[0][0] * io_Pt.x + i_tm.m[1][0] * io_Pt.y + i_tm.m[2][0];
	io_Pt.y = i_tm.m[0][1] * io_Pt.x + i_tm.m[1][1] * io_Pt.y + i_tm.m[2][1];
	io_Pt.x = rwx;
	return io_Pt;
}
//
//===========================================================================
//	処理内容
//		座標変換マトリックスにより座標変換する
//
//	使用法
//		MgULine2 = MgULine2*  MgMat2;
//
//	| px  py  pz  1 |	  | m11 m12 0 m13 |
//	| vx  vy  vz  1 |  X  | m21 m22 0 m23 |
//						  |  0   0  1  0  |
//			   			  | m31 m32 0 m33 |
//
//MgULine3 operator * (const MgULine3& uln1, const MgMat2 &m2)
MgULine3 MGeo::MatMultULn32( const MgULine3& i_ULn, const MgMat2& i_tm)
{
	MgULine3 ULno;
	ULno.p.x = i_tm.m[0][0] * i_ULn.p.x + i_tm.m[1][0] * i_ULn.p.y + i_tm.m[2][0];
	ULno.p.y = i_tm.m[0][1] * i_ULn.p.x + i_tm.m[1][1] * i_ULn.p.y + i_tm.m[2][1];
	ULno.p.z = i_ULn.p.z;

	ULno.v.x = i_tm.m[0][0] * i_ULn.v.x + i_tm.m[1][0] * i_ULn.v.y;
	ULno.v.y = i_tm.m[0][1] * i_ULn.v.x + i_tm.m[1][1] * i_ULn.v.y;
	ULno.v.z = i_ULn.v.z;
	return ULno;
}
//
//===========================================================================
//	処理内容
//		座標変換マトリックスにより座標変換する
//
//	使用法
//		MgULine2* = MgMat2;
//
//	| px  py  pz  1 |     | px  py  pz  1 |		| m11 m12 0 m13 |
//	| vx  vy  vz  1 |  =  | vx  vy  vz  1 |  X  | m21 m22 0 m23 |
//												|  0   0  1  0  |
//				 				   				| m31 m32 0 m33 |
//
//MgULine3 operator *= (MgULine3& ULn1, const MgMat2 &m2)
MgULine3 MGeo::MatMultEqualULn32( MgULine3& io_ULn, const MgMat2& i_tm)
{
	MREAL	rwx;
	rwx		   = i_tm.m[0][0] * io_ULn.p.x + i_tm.m[1][0] * io_ULn.p.y + i_tm.m[2][0];
	io_ULn.p.y = i_tm.m[0][1] * io_ULn.p.x + i_tm.m[1][1] * io_ULn.p.y + i_tm.m[2][1];
	io_ULn.p.x = rwx;

	rwx		   = i_tm.m[0][0] * io_ULn.v.x + i_tm.m[1][0] * io_ULn.v.y;
	io_ULn.v.y = i_tm.m[0][1] * io_ULn.v.x + i_tm.m[1][1] * io_ULn.v.y;
	io_ULn.v.x = rwx;
	return io_ULn;
}
//
//======================( ２次元 )==============================
//		マトリックスをトレースする
//
void MgMat2::MgMatPrint2(MCHAR* s)														// print
{
#ifdef _DEBUG
	Msprintf( MbLog::m_Str, Mstr( "%s	MgMat2	=	%f, %f, %f\n")
			        Mstr( "					%f, %f, %f\n")
					Mstr( "					%f, %f, %f\n"),
							s, m[0][0], m[0][1], m[0][2],
							   m[1][0], m[1][1], m[1][2],
							   m[2][0], m[2][1], m[2][2]);
	MBTRCPRBF;
#endif
};
//
//===========================================================================
//
//	３Ｄ座標変換マトリックスの計算
//
//===========================================================================
//	処理内容
//		座標変換マトリックスの加算
//
//	使用法
//		MgMat3_3 = MgMat3_1 + MgMat3_2;
//
//	| m11 m12 m13 m14 |     | n11 n12 n13 n14 |
//	| m21 m22 m23 m24 |  +  | n21 n22 n23 n24 |
//	| m31 m32 m33 m34 |     | n31 n32 n33 n34 |
//	| m41 m42 m43 m44 |     | n41 n42 n43 n44 |    
//
//MgMat3 operator + (const MgMat3 &M1, const MgMat3 &M2)
MgMat3 MGeo::MatPlus3(const MgMat3 &i_Tm, const MgMat3 &i_Tn)
{
	MgMat3	To;
	int		ic;
	for ( ic=0; ic<4; ic++) {
		To.m[ic][0] = i_Tm.m[ic][0] + i_Tn.m[ic][0];
		To.m[ic][1] = i_Tm.m[ic][1] + i_Tn.m[ic][1];
		To.m[ic][2] = i_Tm.m[ic][2] + i_Tn.m[ic][2];
		To.m[ic][3] = i_Tm.m[ic][3] + i_Tn.m[ic][3];
	}
	return To;
}
//
//===========================================================================
//	処理内容
//		座標変換マトリックスの加算
//
//	使用法
//		MgMat3 += MgMat3;
//
//	| m11 m12 m13 m14 |		| m11 m12 m13 m14 |     | n11 n12 n13 n14 |
//	| m21 m22 m23 m24 |  =  | m21 m22 m23 m24 |  +  | n21 n22 n23 n24 |
//	| m31 m32 m33 m34 |     | m31 m32 m33 m34 |     | n31 n32 n33 n34 |
//	| m41 m42 m43 m44 |     | m41 m42 m43 m44 |     | n41 n42 n43 n44 |     
//
//MgMat3 operator += (MgMat3 &M1, const MgMat3 &M2)
MgMat3 MGeo::MatPlusEqual3( MgMat3 &io_Tm, const MgMat3 &i_Tn)
{
	int		ic;
	for ( ic=0; ic<4; ic++) {
		io_Tm.m[ic][0] += i_Tn.m[ic][0];
		io_Tm.m[ic][1] += i_Tn.m[ic][1];
		io_Tm.m[ic][2] += i_Tn.m[ic][2];
		io_Tm.m[ic][3] += i_Tn.m[ic][3];
	}
	return io_Tm;
}
//
//===========================================================================
//	処理内容
//		負の座標変換マトリックスを作成
//
//	使用法
//		MgMat3_2 = - MgMat3_1;
//
//	  | m11 m12 m13 m14 |
//	- | m21 m22 m23 m24 |
//	  | m31 m32 m33 m34 |
//	  | m41 m42 m43 m44 |     
//
//MgMat3 operator - (const MgMat3 &M1)
MgMat3 MGeo::MatSingleMinus3( const MgMat3& i_Tm)
{
	MgMat3	To;
	int		ic;
	for ( ic=0; ic<4; ic++) {
		To.m[ic][0] = - i_Tm.m[ic][0];
		To.m[ic][1] = - i_Tm.m[ic][1];
		To.m[ic][2] = - i_Tm.m[ic][2];
		To.m[ic][3] = - i_Tm.m[ic][3];
	}
	return To;
}
//
//===========================================================================
//	処理内容
//		座標変換マトリックスの引算
//
//	使用法
//		MgMat3_3 = MgMat3_1 - MgMat3_2;
//
//	  | m11 m12 m13 m14 |     | n11 n12 n13 n14 |
//	  | m21 m22 m23 m24 |  -  | n21 n22 n23 n24 |
//	  | m31 m32 m33 m34 |     | n31 n32 n33 n34 |
//	  | m41 m42 m43 m44 |     | n41 n42 n43 n44 |    
//
//MgMat3 operator - (const MgMat3 &M1, const MgMat3 &M2)
MgMat3 MGeo::MatMinus3( const MgMat3& i_Tm, const MgMat3& i_Tn)
{
	MgMat3	To;
	int		ic;
	for ( ic=0; ic<4; ic++) {
		To.m[ic][0] = i_Tm.m[ic][0] - i_Tn.m[ic][0];
		To.m[ic][1] = i_Tm.m[ic][1] - i_Tn.m[ic][1];
		To.m[ic][2] = i_Tm.m[ic][2] - i_Tn.m[ic][2];
		To.m[ic][3] = i_Tm.m[ic][3] - i_Tn.m[ic][3];
	}
	return To;
}
//
//===========================================================================
//	処理内容
//		座標変換マトリックスの引算
//
//	使用法
//		MgMat3 -= MgMat3;
//
//	| m11 m12 m13 m14 |		| m11 m12 m13 m14 |     | n11 n12 n13 n14 |
//	| m21 m22 m23 m24 |  =  | m21 m22 m23 m24 |  -  | n21 n22 n23 n24 |
//	| m31 m32 m33 m34 |     | m31 m32 m33 m34 |     | n31 n32 n33 n34 |
//	| m41 m42 m43 m44 |     | m41 m42 m43 m44 |     | n41 n42 n43 n44 |     
//
//MgMat3 operator -= (MgMat3 &M1, const MgMat3 &M2)
MgMat3 MGeo::MatMinusEqual3( MgMat3& io_Tm, const MgMat3& i_Tn)
{
	int		ic;
	for ( ic=0; ic<4; ic++) {
		io_Tm.m[ic][0] -= i_Tn.m[ic][0];
		io_Tm.m[ic][1] -= i_Tn.m[ic][1];
		io_Tm.m[ic][2] -= i_Tn.m[ic][2];
		io_Tm.m[ic][3] -= i_Tn.m[ic][3];
	}
	return io_Tm;
}
//
//===========================================================================
//	処理内容
//		座標変換マトリックスのかけ算
//
//	使用法
//		MgMat3_3 = MgMat3_1 * MgMat3_2;
//
//	| m11 m12 m13 m14 |     | n11 n12 n13 n14 |
//	| m21 m22 m23 m24 |  X  | n21 n22 n23 n24 |
//	| m31 m32 m33 m34 |     | n31 n32 n33 n34 |
//	| m41 m42 m43 m44 |     | n41 n42 n43 n44 |    
//
//MgMat3 operator * (const MgMat3 &M1, const MgMat3 &M2)
MgMat3 MGeo::MatMult3( const MgMat3& i_Tm, const MgMat3& i_Tn)
{
	MgMat3	To;
	int		i1, i2;
	for (i1=0; i1<4; i1++) 
		for (i2=0; i2<4; i2++) 
			To.m[i1][i2] = i_Tm.m[i1][0] * i_Tn.m[0][i2] +
						   i_Tm.m[i1][1] * i_Tn.m[1][i2] +
						   i_Tm.m[i1][2] * i_Tn.m[2][i2] +
						   i_Tm.m[i1][3] * i_Tn.m[3][i2];
	return To;
}
//
//===========================================================================
//	処理内容
//		座標変換マトリックスのかけ算
//
//	使用法
//		MgMat3 *= MgMat3;
//
//	| m11 m12 m13 m14 |     | m11 m12 m13 m14 |     | n11 n12 n13 n14 |
//	| m21 m22 m23 m24 |  =  | m21 m22 m23 m24 |  X  | n21 n22 n23 n24 |
//	| m31 m32 m33 m34 |     | m31 m32 m33 m34 |     | n31 n32 n33 n34 |
//	| m41 m42 m43 m44 |     | m41 m42 m43 m44 |     | n41 n42 n43 n44 |     
//
//MgMat3 operator *= (MgMat3 &M1, const MgMat3 &M2)
MgMat3 MGeo::MatMultEqual3( MgMat3& io_Tm, const MgMat3& i_Tn)
{
	MgMat3	To;
	int		i1, i2;
	for (i1=0; i1<4; i1++) 
		for (i2=0; i2<4; i2++) 
			To.m[i1][i2] = io_Tm.m[i1][0] * i_Tn.m[0][i2] +
						   io_Tm.m[i1][1] * i_Tn.m[1][i2] +
						   io_Tm.m[i1][2] * i_Tn.m[2][i2] +
						   io_Tm.m[i1][3] * i_Tn.m[3][i2];
	io_Tm = To;
	return io_Tm;
}
//
//===========================================================================
//	処理内容
//		３次元座標変換マトリックスにより２次元データを座標変換する
//
//	使用法
//		MgMat3_2 = MgPoint2 * MgMat3_1;
//
//						    | m11 m12 m13 m14 |
//	| px  py  0   1   |  X  | m21 m22 m23 m24 |
//						    | m31 m32 m33 m34 |
//						    | m41 m42 m43 m44 |
//
//MgPoint2 operator * (const MgPoint2& Pt1, const MgMat3 &M1)
MgPoint2 MGeo::MatMultPt23( const MgPoint2& i_Pt, const MgMat3& i_Tm)
{
	MgPoint2	Po;
	Po.x = i_Tm.m[0][0] * i_Pt.x + i_Tm.m[1][0] * i_Pt.y + i_Tm.m[3][0];
	Po.y = i_Tm.m[0][1] * i_Pt.x + i_Tm.m[1][1] * i_Pt.y + i_Tm.m[3][1];
	return Po;
}
//
//===========================================================================
//	処理内容
//		３次元座標変換マトリックスにより２次元データを座標変換する
//
//	使用法
//		MgPoint2 *= MgMat3;
//
//						     					    | m11 m12 m13 m14 |
//	| px  py  0   1   |  =  | px  py  0   1   |  X  | m21 m22 m23 m24 |
//	           									    | m31 m32 m33 m34 |
//	           									    | m41 m42 m43 m44 |
//
//MgPoint2 operator *= (MgPoint2& Pt1o, const MgMat3 &M1)
MgPoint2 MGeo::MatMultEqualPt23( MgPoint2& io_Pt, const MgMat3& i_Tm)
{
	MREAL	rwx;
	rwx		= i_Tm.m[0][0] * io_Pt.x + i_Tm.m[1][0] * io_Pt.y + i_Tm.m[3][0];
	io_Pt.y = i_Tm.m[0][1] * io_Pt.x + i_Tm.m[1][1] * io_Pt.y + i_Tm.m[3][1];
	io_Pt.x = rwx;
	return io_Pt;
}
//
//===========================================================================
//	処理内容
//		３次元座標変換マトリックスにより３次元データを座標変換する
//
//	使用法
//		MgPoint3_2 = MgPoint3_1 * MgMat3;
//
//	 					    | m11 m12 m13 m14 |
//	| px  py  pz  1   |  X  | m21 m22 m23 m24 |
//						    | m31 m32 m33 m34 |
//						    | m41 m42 m43 m44 |
//
//MgPoint3 operator * (const MgPoint3& Pt1, const MgMat3 &M1)
MgPoint3 MGeo::MatMultPt33( const MgPoint3& i_Pt, const MgMat3& i_Tm)
{
	MgPoint3	Pto;
	Pto.x = i_Tm.m[0][0] * i_Pt.x + i_Tm.m[1][0] * i_Pt.y +	i_Tm.m[2][0] * i_Pt.z + i_Tm.m[3][0];
	Pto.y = i_Tm.m[0][1] * i_Pt.x + i_Tm.m[1][1] * i_Pt.y + i_Tm.m[2][1] * i_Pt.z + i_Tm.m[3][1];
	Pto.z = i_Tm.m[0][2] * i_Pt.x + i_Tm.m[1][2] * i_Pt.y +	i_Tm.m[2][2] * i_Pt.z + i_Tm.m[3][2];
#ifdef	MGDOUJIZAHYOU
	MREAL		dd;
	dd 	  = i_Tm.m[0][3] * i_Pt.x + i_Tm.m[1][3] * i_Pt.y + i_Tm.m[2][3] * i_Pt.z + i_Tm.m[3][3];
//	_ASSERT(dd>MCTOLDN);
	if (dd > g_gTol.DN) {
		MREAL	dd1 = 1. / dd;
		po.x *= dd1;
		po.y *= dd1;
		po.z *= dd1;
	}
#endif
	return Pto;
}
//
//===========================================================================
//	処理内容
//		３次元座標変換マトリックスにより３次元データを座標変換する
//
//	使用法
//		MgPoint3 *= MgMat3;
//
//							 					    | m11 m12 m13 m14 |
//	| px  py  pz  1   |  =  | px  py  pz  1   |  X  | m21 m22 m23 m24 |
//												    | m31 m32 m33 m34 |
//												    | m41 m42 m43 m44 |
//
//MgPoint3 operator *= (MgPoint3& Pt1o, const MgMat3 &M1)
MgPoint3 MGeo::MatMultEqualPt33( MgPoint3& io_Pt, const MgMat3& i_Tm)
{
	MgPoint3	Pto;
	Pto.x = i_Tm.m[0][0] * io_Pt.x + i_Tm.m[1][0] * io_Pt.y + i_Tm.m[2][0] * io_Pt.z + i_Tm.m[3][0];
	Pto.y = i_Tm.m[0][1] * io_Pt.x + i_Tm.m[1][1] * io_Pt.y + i_Tm.m[2][1] * io_Pt.z + i_Tm.m[3][1];
	Pto.z = i_Tm.m[0][2] * io_Pt.x + i_Tm.m[1][2] * io_Pt.y + i_Tm.m[2][2] * io_Pt.z + i_Tm.m[3][2];
#ifdef	MGDOUJIZAHYOU
	MREAL		dd;
	dd 	  = i_Tm.m[0][3] * io_Pt.x + i_Tm.m[1][3] * io_Pt.y + i_Tm.m[2][3] * io_Pt.z + i_Tm.m[3][3];
//	_ASSERT(dd>MCTOLDN);
	if (dd > g_gTol.DN) {
		MREAL	dd1 = 1. / dd;
		po.x *= dd1;
		po.y *= dd1;
		po.z *= dd1;
	}
#endif
	io_Pt = Pto;
	return io_Pt;
}
//
//===========================================================================
//	処理内容
//		３次元座標変換マトリックスにより３次元データを座標変換する
//
//	使用法
//		MgULine3_2 = MgULine3_1 * MgMat3;
//
//	| px  py  pz  1   |     | m11 m12 m13 m14 |
//	| vx  vy  vz  0   |  X  | m21 m22 m23 m24 |
//							| m31 m32 m33 m34 |
//							| m41 m42 m43 m44 |
//
//MgULine3 operator * (const MgULine3& ULn1, const MgMat3 &M1)
MgULine3 MGeo::MatMultULn33( const MgULine3& i_ULn, const MgMat3& i_Tm)
{
	MgULine3	ULno;
	ULno.p.x = i_Tm.m[0][0] * i_ULn.p.x + i_Tm.m[1][0] * i_ULn.p.y + i_Tm.m[2][0] * i_ULn.p.z + i_Tm.m[3][0];
	ULno.p.y = i_Tm.m[0][1] * i_ULn.p.x + i_Tm.m[1][1] * i_ULn.p.y + i_Tm.m[2][1] * i_ULn.p.z + i_Tm.m[3][1];
	ULno.p.z = i_Tm.m[0][2] * i_ULn.p.x + i_Tm.m[1][2] * i_ULn.p.y + i_Tm.m[2][2] * i_ULn.p.z + i_Tm.m[3][2];
																		   
	ULno.v.x = i_Tm.m[0][0] * i_ULn.v.x + i_Tm.m[1][0] * i_ULn.v.y + i_Tm.m[2][0] * i_ULn.v.z;
	ULno.v.y = i_Tm.m[0][1] * i_ULn.v.x + i_Tm.m[1][1] * i_ULn.v.y + i_Tm.m[2][1] * i_ULn.v.z;
	ULno.v.z = i_Tm.m[0][2] * i_ULn.v.x + i_Tm.m[1][2] * i_ULn.v.y + i_Tm.m[2][2] * i_ULn.v.z;

	return ULno;
}
//
//===========================================================================
//	処理内容
//		３次元座標変換マトリックスにより３次元データを座標変換する
//
//	使用法
//		MgPoint3 *= MgMat3;
//
//	| px  py  pz  1   |     | px  py  pz  1   |     | m11 m12 m13 m14 |
//	| vx  vy  vz  0   |  =  | vx  vy  vz  0   |  X  | m21 m22 m23 m24 |
//													| m31 m32 m33 m34 |
//													| m41 m42 m43 m44 |
//
//MgULine3 operator *= (MgULine3& ULn1, const MgMat3 &M1)
MgULine3 MGeo::MatMultEqualULn33( MgULine3& io_ULn, const MgMat3& i_Tm)
{
	MgULine3	ULno;
	ULno.p.x = i_Tm.m[0][0] * io_ULn.p.x + i_Tm.m[1][0] * io_ULn.p.y + i_Tm.m[2][0] * io_ULn.p.z + i_Tm.m[3][0];
	ULno.p.y = i_Tm.m[0][1] * io_ULn.p.x + i_Tm.m[1][1] * io_ULn.p.y + i_Tm.m[2][1] * io_ULn.p.z + i_Tm.m[3][1];
	ULno.p.z = i_Tm.m[0][2] * io_ULn.p.x + i_Tm.m[1][2] * io_ULn.p.y + i_Tm.m[2][2] * io_ULn.p.z + i_Tm.m[3][2];
																		   
	ULno.v.x = i_Tm.m[0][0] * io_ULn.v.x + i_Tm.m[1][0] * io_ULn.v.y + i_Tm.m[2][0] * io_ULn.v.z;
	ULno.v.y = i_Tm.m[0][1] * io_ULn.v.x + i_Tm.m[1][1] * io_ULn.v.y + i_Tm.m[2][1] * io_ULn.v.z;
	ULno.v.z = i_Tm.m[0][2] * io_ULn.v.x + i_Tm.m[1][2] * io_ULn.v.y + i_Tm.m[2][2] * io_ULn.v.z;
	io_ULn = ULno;
	return io_ULn;
}
//
//======================( ３次元 )==============================
//		マトリックスをトレースする
//
void MgMat3::MgMatPrint3(MCHAR* s)													// print
{
#ifdef _DEBUG
	Msprintf( MbLog::m_Str, Mstr( "%s	MgMat2	=	%f, %f, %f, %f\n")
			        Mstr( "					%f, %f, %f, %f\n")
			        Mstr( "					%f, %f, %f, %f\n")
					Mstr( "					%f, %f, %f, %f\n"),
							s, m[0][0], m[0][1], m[0][2],m[0][3],
							   m[1][0], m[1][1], m[1][2],m[1][3],
							   m[2][0], m[2][1], m[2][2],m[2][3],
							   m[3][0], m[3][1], m[3][2],m[3][3]);
	MBTRCPRBF;
#endif
};
//
//======================( ２次元 )==============================
//	処理内容
//		原点を中心に回転の座標変換マトリックスを作成
//
//	使用法
//		MgMat2 = MgMat2RotV(MgVect2);
//
//		( MgVect2: 単位ベクトル)
//
//	|  cos  sin  0. |
//	| -sin  cos  0. |
//	|   0.   0.  1. |
//
MgMat2 MGeo::Mat2RotV(const MgVect2& v)
{
	return MgMat2( v.x,  v.y,  0.0f, 
				  -v.y,  v.x,  0.0f, 
				   0.0f, 0.0f, 1.0f);
}
//
//======================( ２次元 )==============================
//	処理内容
//		原点を中心に逆回転の座標変換マトリックスを作成
//
//	使用法
//		MgMat2 = MgMat2RotRevV(MgVect2);
//
//		( MgVect2: 単位ベクトル)
//
//	| cos -sin  0. |
//	| sin  cos  0. |
//	|  0.   0.  1. |
//
MgMat2 MGeo::Mat2RotRevV(const MgVect2& v)
{
	return MgMat2( v.x, -v.y,  0.0f, 
				   v.y,  v.x,  0.0f,
				   0.0f, 0.0f, 1.0f);
}
//
//======================( ３次元 )==============================
//	処理内容
//		ＸＹ平面で原点を中心に回転の座標変換マトリックスを作成
//
//	使用法
//		MgMat3 = MgMat3RotZV(MgVect2);
//
//		( MgVect2: 単位ベクトル)
//
//	|  cos  sin  0.  0. |
//	| -sin  cos  0.  0. |
//	|   0.   0.  1.  0. |
//	|   0.   0.  0.  1. |
//
MgMat3 MGeo::Mat3RotZV(const MgVect2& v)
{
	return MgMat3( v.x,  v.y,  0.0f, 0.0f,
				  -v.y,  v.x,  0.0f, 0.0f,
				   0.0f, 0.0f, 1.0f, 0.0f,
				   0.0f, 0.0f, 0.0f, 1.0f);
}
//
//======================( ３次元 )==============================
//	処理内容
//		ＸＹ平面で原点を中心に逆回転の座標変換マトリックスを作成
//
//	使用法
//		MgMat3 = MgMat3RotRevZV(MgVect2);
//
//		( MgVect2: 単位ベクトル)
//
//	|  cos -sin  0.  0. |
//	|  sin  cos  0.  0. |
//	|   0.   0.  1.  0. |
//	|   0.   0.  0.  1. |
//
MgMat3 MGeo::Mat3RotRevZV(const MgVect2& v)
{
	return MgMat3( v.x, -v.y,  0.0f, 0.0f,
				   v.y,  v.x,  0.0f, 0.0f,
				   0.0f, 0.0f, 1.0f, 0.0f,
				   0.0f, 0.0f, 0.0f, 1.0f);
}
//
//======================( ３次元 )==============================
//	処理内容
//		ＹＺ平面で原点を中心に回転の座標変換マトリックスを作成
//
//	使用法
//		MgMat3 = MgMat3RotTXV(MgVect2);
//
//		( MgVect2: 単位ベクトル)
//
//	|  1.   0.   0.  0. |
//	|  0.  cos  sin  0. |
//	|  0. -sin  cos  0. |
//	|  0.   0.   0.  1. |
//
MgMat3 MGeo::Mat3RotTXV(const MgVect2& v)
{
	return MgMat3( 1.0f,  0.0f, 0.0f, 0.0f,
				   0.0f,  v.x,  v.y,  0.0f,
				   0.0f, -v.y,  v.x,  0.0f,
				   0.0f,  0.0f, 0.0f, 1.0f);
}
//
//======================( ３次元 )==============================
//	処理内容
//		ＹＺ平面で原点を中心に逆回転の座標変換マトリックスを作成
//
//	使用法
//		MgMat3 = MgMat3RotRevXV(MgVect2);
//
//		( MgVect2: 単位ベクトル)
//
//	|  1.   0.   0.  0. |
//	|  0.  cos -sin  0. |
//	|  0.  sin  cos  0. |
//	|  0.   0.   0.  1. |
//
MgMat3 MGeo::Mat3RotRevXV(const MgVect2& v)
{
	return MgMat3( 1.0f,  0.0f, 0.0f, 0.0f,
				   0.0f,  v.x, -v.y,  0.0f,
				   0.0f,  v.y,  v.x,  0.0f,
				   0.0f,  0.0f, 0.0f, 1.0f);
}
//
//======================( ３次元 )==============================
//	処理内容
//		ＺＸ平面で原点を中心に回転の座標変換マトリックスを作成
//
//	使用法
//		MgMat3 = MgMat3RotYV(MgVect2);
//
//		( MgVect2: 単位ベクトル)
//
//	|  cos   0. -sin   0. |
//	|   0.   1.   0.   0. |
//	|  sin   0.  cos   0. |
//	|   0.   0.   0.   1. |
//
MgMat3 MGeo::Mat3RotYV(const MgVect2& v)
{
	return MgMat3( v.x,   0.0f, -v.y,  0.0f,
				   0.0f,  1.0f,  0.0f, 0.0f,
				   v.y,   0.0f,  v.x,  0.0f,
				   0.0f,  0.0f,  0.0f, 1.0f);
}
//
//======================( ３次元 )==============================
//	処理内容
//		ＺＸ平面で原点を中心に逆回転の座標変換マトリックスを作成
//
//	使用法
//		MgMat3 = MgMat3RotRevYV(MgVect2);
//
//		( MgVect2: 単位ベクトル)
//
//	|  cos   0.  sin   0. |
//	|   0.   1.   0.   0. |
//	| -sin   0.  cos   0. |
//	|   0.   0.   0.   1. |
//
MgMat3 MGeo::Mat3RotRevYV(const MgVect2& v)
{
	return MgMat3( v.x,   0.0f,  v.y,  0.0f,
				   0.0f,  1.0f,  0.0f, 0.0f,
				  -v.y,   0.0f,  v.x,  0.0f,
				   0.0f,  0.0f,  0.0f, 1.0f);
}
//
//======================( ２次元 )==============================
//	処理内容
//	    Ｘ，Ｙ方向に拡大／縮小するマトリックスを作成
//
//	使用法
//		Mgmat_2 = MgMat2Scl(MgVect2_1);
//
//	| sx  0   0  |
//	| 0   xy  0  |
//	| 0   0   1  |
//
MgMat2 MGeo::Mat2Scl(const MgVect2& vs)
{
	return MgMat2(vs.x, 0.0f, 0.0f, 
			      0.0f, vs.y, 0.0f,
				  0.0f, 0.0f, 1.0f);
}
//
//======================( ３次元 )==============================
//	処理内容
//	    Ｘ，Ｙ，Ｚ方向に拡大／縮小するマトリックスを作成する
//
//	使用法
//		Mgmat3_2 = MgMat3Scl(MgMat3_1);
//
//	| sx  0   0   0  |
//	| 0   xy  0   0  |
//	| 0   0   sz  0  |
//	| 0   0   0   1  |
//
MgMat3 MGeo::Mat3Scl(const MgVect3& vs)
{
	return MgMat3(vs.x, 0.0f, 0.0f, 0.0f, 
				  0.0f, vs.y, 0.0f, 0.0f,
				  0.0f, 0.0f, vs.z, 0.0f, 
				  0.0f, 0.0f, 0.0f, 1.0f);
}
//
//======================( ２次元 )==============================
//	処理内容
//		平行移動の変換マトリックスを作成する
//
//	使用法
//		MgMat2 = MgMat2Mov(MgVect2)
//
//	|  1.  0.  0. |
//	|  0.  1.  0. |
//	|  v.x v.y 1. |
//
MgMat2 MGeo::Mat2Mov(const MgVect2& v)
{
	return MgMat2(1.0f, 0.0f, 0.0f,
				  0.0f, 1.0f, 0.0f,
				  v.x,  v.y,  1.0f);
}
//
//======================( ３次元 )==============================
//	処理内容
//		平行移動の変換マトリックスを作成する
//
//	使用法
//		Mgmat3 = MgMat3Mov(MgVect3)
//
//	| 1.  0.  0.  0. |
//	| 0.  1.  0.  0. |
//	| 0.  0.  1.  0. |
//	| v.x v.y v.z 1. |
//
MgMat3 MGeo::Mat3Mov(const MgVect3& v)
{
	return MgMat3(1.0f, 0.0f, 0.0f, 0.0f,
				  0.0f, 1.0f, 0.0f, 0.0f,
				  0.0f, 0.0f, 1.0f, 0.0f,
				  v.x,  v.y,  v.z,  1.0f);
}
//
//======================( ２次元 )==============================
//	処理内容
//		変換マトリックスの逆変換マトリックスを作成
//
//	使用法
//		MgMat2 = MgMat2Inv(MgMat2);
//
//	| m11 m21 m31 |
//	| m12 m22 m32 |			（入力行列は単位行列である事 ???）
//	| m13 m23 m33 |
//
MgMat2 MGeo::Mat2Inv( const MgMat2& i_tm)
{
	MgMat2	tmlu = i_tm;
	MgMat2	tmo;
	int		i, j, k;
	
    // LU分解	
    for (i=0; i<3; i++){
        for (j=i+1; j<3; j++){
            tmlu.m[j][i] /= tmlu.m[i][i];
            for (k=i+1; k<3; k++)
                tmlu.m[j][k] -= tmlu.m[i][k] * tmlu.m[j][i];
        }
    }

    // 逆行列を求める
    for (k=0; k<3; k++){
        // 初期化
        for (i=0; i<3; i++){
            if( i == k){ tmo.m[i][k] = 1; }
            else{ tmo.m[i][k] = 0; }
        }
        // 解を求める
        for (i=0; i<3; i++){
            for (j=i+1; j<3; j++)
                tmo.m[j][k] -= tmo.m[i][k] * tmlu.m[j][i];
        }
        for (i=3-1; i>=0; i--){
            for (j=i+1; j<3; j++)
                tmo.m[i][k] -= tmlu.m[i][j] * tmo.m[j][k];
            tmo.m[i][k] /= tmlu.m[i][i];
        }
    }
    return tmo;
}
//
//======================( ３次元 )==============================
//	処理内容
//	    変換マトリックスの逆変換マトリックスを作成する
//
//	使用法
//		Mgmat3_2 = MgMat3Inv(MgMat3_1);
//
//	| m11 m21 m31 m41 |
//	| m12 m22 m32 m42 |     （入力行列は単位行列である事 ???）
//	| m13 m23 m33 m43 |
//	| M14 m24 m34 m44 |
//
MgMat3 MGeo::Mat3Inv( const MgMat3& i_Tm)
{
	MgMat3	Tmlu = i_Tm;
	MgMat3	Tmo;
	int		i, j, k;
	
    // LU分解 
    for (i=0; i<4; i++){
        for (j=i+1; j<4; j++){
            Tmlu.m[j][i] /= Tmlu.m[i][i];
            for (k=i+1; k<4; k++)
                Tmlu.m[j][k] -= Tmlu.m[i][k] * Tmlu.m[j][i];
        }
    }

    // 逆行列を求める
    for (k=0; k<4; k++){
        // 初期化
        for (i=0; i<4; i++){
            if( i == k)
				Tmo.m[i][k] = 1;
            else{ Tmo.m[i][k] = 0; }
        }
        // 解を求める
        for (i=0; i<4; i++){
            for (j=i+1; j<4; j++)
                Tmo.m[j][k] -= Tmo.m[i][k] * Tmlu.m[j][i];
        }
        for (i=4-1; i>=0; i--){
            for (j=i+1; j<4; j++)
                Tmo.m[i][k] -= Tmlu.m[i][j] * Tmo.m[j][k];
            Tmo.m[i][k] /= Tmlu.m[i][i];
        }
    }
    return Tmo;
}
//
//===========================================================================
//		直線に対する鏡像変換行列を作成
//
// ---------------------( ２次元 )------------------------------
//
MgMat2 MGeo::Mat2Mirror(								// (  O) 鏡像変換行列
				const MgMat2&	i_tm, 				// (I  ) 変換行列
				const MgULine2& i_uln				// (I  ) 鏡映の軸とする直線
		)
{
	MgMat2	tmo;
//
	tmo = i_tm;
	tmo *= Mat2Mov( - i_uln.p);
	tmo *= Mat2RotRevV( i_uln.v);
//
	tmo *= Mat2Scl( MgVect2( 1.0f, -1.0f));
//
	tmo *= Mat2RotV( i_uln.v);
    tmo *= Mat2Mov( i_uln.p);
	return tmo;
}
//===========================================================================
//		平面に対する鏡像変換行列を作成
//
// ---------------------( ３次元 )------------------------------
//
MgMat3 MGeo::Mat3Mirror(								// (  O) 鏡像変換行列
				const	MgMat3&	  i_Tm,	 			// (I  ) 変換行列
				const	MgPlane3& i_Pln				// (I  ) 鏡映の面とする平面
		)
{
	MgVect3 V0, V1, V2;
	MgMat3	Tmo;
//
	Tmo = i_Tm;
//
	V0 = i_Pln.d * i_Pln.v;
	V1 = TaniV3( i_Pln.v);
	V2 = i_Pln.v ^ V1;
	MgMat3 rm1( V2, i_Pln.v, V1);
//
	Tmo *= Mat3Mov( -V0);
	Tmo *= rm1;
//
	Tmo *= Mat3Scl( MgVect3( 1.0f, -1.0f, 1.0f));
//
	Tmo *= Mat3Inv( rm1);
	Tmo *= Mat3Mov( V0);
	return Tmo;
}
//
//===========================================================================
//		中心点と回転角より変換行列を作成
//
// ---------------------( ２次元 )------------------------------
//
MgMat2 MGeo::Mat2Rot(									// (  O) 変換行列
				const	MgPoint2&	i_ptc, 			// (I  ) 回転の中心となる点
				const	MREAL&		i_ar			// (I  ) 回転角 Degree
		)
{

	MgMat2    tmw, tmo;
	MgAngleR  ra;
	MgVect2   v1;
//
	ra.a  = MGRADIAN( i_ar);
	v1 = MgVect2C( ra);
//
	tmw = Mat2RotV( v1);
//
    tmo = Mat2Mov( -i_ptc) * tmw;
//
	tmo *= Mat2Mov( i_ptc);
	return tmo;
}
//===========================================================================
//		回転軸となる直線と回転角より変換行列を作成
//
// ---------------------( ３次元 )------------------------------
//
MgMat3 MGeo::Mat3Rot(									// (  O) 変換行列
				const	MgULine3&	i_ULn, 		// (I  ) 回転軸となる直線
				const	MREAL		&i_ar			// (I  ) 回転角(Degree)
		)
{
	MgMat3		Tmw, Tmo;
	MgAngleR	ra;
	MgVect2		va;
	MREAL		rmcos;
//
	ra.a  = MGRADIAN( i_ar);
	va = MgVect2C( ra);
	rmcos = 1 - va.x;

//
	Tmw.m[0][0] = i_ULn.v.x * i_ULn.v.x * rmcos + va.x;
	Tmw.m[1][0] = i_ULn.v.x * i_ULn.v.y * rmcos - i_ULn.v.z * va.y;
	Tmw.m[2][0] = i_ULn.v.x * i_ULn.v.z * rmcos - i_ULn.v.y * va.y;
		 
	Tmw.m[0][1] = i_ULn.v.x * i_ULn.v.y * rmcos + i_ULn.v.z * va.y;
	Tmw.m[1][1] = i_ULn.v.y * i_ULn.v.y * rmcos + va.x;
	Tmw.m[2][1] = i_ULn.v.y * i_ULn.v.z * rmcos - i_ULn.v.x * va.y;
		 
	Tmw.m[0][2] = i_ULn.v.x * i_ULn.v.z * rmcos - i_ULn.v.y * va.y;
	Tmw.m[1][2] = i_ULn.v.y * i_ULn.v.z * rmcos + i_ULn.v.x * va.y;
	Tmw.m[2][2] = i_ULn.v.z * i_ULn.v.z * rmcos + va.x;

	Tmw.m[3][0] = Tmw.m[3][1] = Tmw.m[3][2] = 
	Tmw.m[0][3] = Tmw.m[1][3] = Tmw.m[2][3] = 0.0f;
	Tmw.m[3][3] = 1.0f;
//
    Tmo = Mat3Mov( -i_ULn.p) * Tmw;
//
	//MgPoint3 a;
	//a = i_ULn.p;
	Tmo *= Mat3Mov( i_ULn.p);
	return Tmo;
}
//===========================================================================
//		正投象変換行列を計算する
//
// ---------------------( ３次元 )------------------------------
//
MgMat3 MGeo::Mat3NProj(									// (  O) 正投象変換行列
				const	MgMat3		&i_Tm,			// (I  ) 入力座標変換行列
				const	MgPlane3	&i_Pln			// (I  ) 投象面
		)
{
	MgVect3 V1, V2;
	MgMat3	Tmw, Tmo;
	int		ic, jc;
//
	V1 = TaniV3( i_Pln.v);
	V2 = i_Pln.v ^ V1;
	MgMat3 Tm1(V1, V2, i_Pln.v);
//
	Tmw = Tm1 * i_Tm;
//
	Tmw.m[0][2] = Tmw.m[0][3] * i_Pln.d;	
	Tmw.m[1][2] = Tmw.m[1][3] * i_Pln.d;	
	Tmw.m[2][2] = Tmw.m[2][3] * i_Pln.d;	
	Tmw.m[3][2] = Tmw.m[3][3] * i_Pln.d;
//
	for ( ic=0; ic<4; ic++) {
		for (jc=0; jc<4; jc++) {
			Tmo.m[jc][ic] = Tmw.m[ic][0] * Tm1.m[jc][0] + Tmw.m[ic][1] * Tm1.m[jc][1]
						  + Tmw.m[ic][2] * Tm1.m[jc][2] + Tmw.m[ic][3] * Tm1.m[jc][3];
		}
	}
	return	Tmo;
}
//===========================================================================
//		斜投象変換行列を計算する
//
// ---------------------( ３次元 )------------------------------
//
MgMat3 MGeo::Mat3SProj(									// (  O) 斜投象変座標変換行列
				const	MgMat3		&i_Tm,			// (I  ) 入力座標変換行列
				const	MgPlane3	&i_Pln,			// (I  ) 投象面
				const	MgVect3&		i_V			// (I  ) 投象方向を示す単位ベクトル
		)
{
	MgVect3 V1, V2, V3;
	MgMat3	Tm2, Tm3, Tmo;
	int		ic, jc;
//
	V1 = TaniV3( i_Pln.v);
	V2 = i_Pln.v ^ V1;
	MgMat3 Tm1(V1, V2, i_Pln.v);
//
	Tm2 = Tm1 * i_Tm;
	V3	= i_V * Tm2;
//
//	MULTIPLY TMO BY (OBLIQUE PROJECTION MATRIX).

	for ( ic=0; ic<4; ic++) {
		Tm3.m[ic][0] = Tmo.m[ic][0] - ( Tmo.m[ic][2] - Tmo.m[ic][3] * i_Pln.d) * V1.x / V1.z;
		Tm3.m[ic][1] = Tmo.m[ic][1] - ( Tmo.m[ic][2] - Tmo.m[ic][3] * i_Pln.d) * V1.y / V1.z;
		Tm3.m[ic][2] = Tmo.m[ic][3] * i_Pln.d;
		Tm3.m[ic][3] = 0.0f;
	}
//
	for ( ic=0; ic<4; ic++) {
		for (jc=0; jc<4; jc++) {
			Tmo.m[ic][jc] = Tm3.m[ic][0] * Tm1.m[jc][0] + Tm3.m[ic][1] * Tm1.m[jc][1]
						  + Tm3.m[ic][2] * Tm1.m[jc][2] + Tm3.m[ic][3] * Tm1.m[jc][3];
		}
	}
	return	Tmo;
}
/*
//===========================================================================
//		３Ｄ平面上の点から２Ｄ座標への変換行列を計算する
//
// ---------------------( ３次元 )------------------------------
//
MgMat3 MGeo::Mat3Dto2D(									// (  O) 座標変換行列
				const	MgPlane3	&pln,			// (I  ) 平面の法線を示す単位ベクトル
				const	MgVect3&		vi				// (I  ) 平面の法線を示す単位ベクトル
				)
{
	MgMat3	M1o;
	M1o.SetUnit();
//
	MREAL ra = MgAbs( *(MgVect2*)&pln.v);
	if (ra > g_gTol.S)
		MgVect2 Vh = *(MgVect2*)&pln.v;
	
	MgVect2	vt;
	ra = MgAbs(vi);
	
	M1o.m[0][0] = vi.x;
	M1o.m[0][1] = vi.y;
	M1o.m[0][2] = vi.z;
	
	if (ra < MCTOLD) {
					// Ｙ軸回転
		M1o.m[1][0] = vi.z;
		M1o.m[1][1] = 0.f;
		M1o.m[1][2] = - ra;
		M1o.m[2][0] = 0.f;
		M1o.m[2][1] = 1.f;
		M1o.m[2][2] = 0.f;

	} else {
		 			// Ｙ軸回転 × Ｚ軸回転
		vt.x = vi.x / ra;
		vt.y = vi.y / ra;
		
		M1o.m[1][0] = vi.z * vt.x;
		M1o.m[1][1] = vi.z * vt.y;
		M1o.m[1][2] = - ra;
		M1o.m[2][0] = - vt.y;
		M1o.m[2][1] = vt.x;
		M1o.m[2][2] = 0.f;
	}
	M1o.m[0][3] = M1o.m[1][3] = M1o.m[2][3] =
	M1o.m[3][0] = M1o.m[3][1] = M1o.m[3][2] = 0.f;
	M1o.m[3][3] = 1.f;

	return M1o;
}
*/
//===========================================================================
//		３Ｄ平面上の点から２Ｄ座標への変換行列を計算する
//
// ---------------------( ３次元 )------------------------------
//		底辺がX軸に平行になるようXY平面に投影するマトリックスを求める
//
//
//	平面式	ax + by + cz + d = 0
//
//
//	屋根
//
//	(|Pln.v.z|<0.6)
//
//		Z方向のベクトルを　Ｘ,　Zの順で回転すると次の式となり、
//		入力平面に相当する												（２Ｄ→３Ｄ）
//		逆に入力平面を　Ｚ， Ｘの順で回転するとZ方向のベクトルとなる	（３Ｄ→２Ｄ）、
//
//		sx: sin(x), cx: cos(x), sz: sin(z), cz: cos(z), (ab): (MREAL)sqrt(a*a+b*b)
//
//		cz = -b/(ab)	sz = a/(ab)
//		cx = c			sx = (ab)
//
//	================================================
//	３Ｄ→２Ｄ
//
//					|	1			0			0		|
//	X軸逆回転	A	|	0			cx			-sx		|
//					|	0			sx			cx		|
//
//					|	cz			-sz			0		|
//	Z軸逆回転	B	|	sz			cz			0		|
//					|	0			0			1		|
//
//	Z軸逆回転後にX軸逆回転する
//					|	cz			-cxsz		sxsz	|
//		A x B	=	|	sz			cxcz		-sxcz	|
//					|	0			sx			cx		|
//
//					|	-b/(ab)		-ac/(ab)	a		|
//					|	a/(ab)		-bc/(ab)	b		|
//					|	0			(ab)		c		|
//
//	================================================
//	２Ｄ→３Ｄ
//
//	Z軸逆回転後(2)に逆移動(1)し次の回転を行う
//
//					|	cz			sz			0		|
//	Z軸回転		A	|	-sz			cz			0		|
//					|	0			0			1		|
//
//					|	1			0			0		|
//	X軸回転		B	|	0			cx			sx		|
//					|	0			-sx			cx		|
//
//	X軸回転後にＺ軸回転する
//					|	cz			sz			0		|
//		A x B		|	-szcx		czcx		sx		|
//					|	szsx		-czsx		cx		|
//
//					|	-b/(ab)		a/(ab)		0		|
//					|	-ac/(ab)	-bc/(ab)	(ab)	|
//					|	a			b			c		|
//
//	================================================
//
//	|pln.v.y|<0.6	
//
//	X軸回転				-sx		cx
//	Y軸回転		sy･cx	-sx		cy･cx
//				a		b		c
//
//	================================================
//	３Ｄ→２Ｄ
//
//					|	1			0			0			|
//	Ｘ軸逆回転	B	|	0			cx			-sx			|
//					|	0			sx			cx			|
//
//					|	cy			0			sy			|
//	Ｙ軸逆回転	A	|	0			1			0			|
//					|	-sy			0			cy			|
//
//	Ｙ軸逆回転後にX軸逆回転する
//					|	cy			sysx		sycx		|
//		A x B		|	0			cx			-sx			|
//					|	-sy			cysx		cycx		|
//
//					|	c/(ca)		-ab/(ca)	a			|
//		-			|	0			(ca)		b			|
//					|	-a/(ca)		-bc/(ca)	c			|
//
//	================================================
//
//	２Ｄ→３Ｄ
//					
//	Z軸逆回転後に逆移動する
//
//					|	cy			0			-sy			|
//	Ｙ軸回転	A	|	0			1			0			|
//					|	sy			0			cy			|
//
//					|	1			0			0			|
//	X軸回転		B	|	0			cx			sx			|
//					|	0			-sx			cx			|
//
//	X軸回転後にY軸回転する
//					|	cy			0			-sy			|
//			A x B	|	sxsy		cx			sxcy		|
//					|	cxsy		-sx			cxcy		|
//
//					|	c/(ca)		0			-a/(ca)		|
//					|	-ab/(ca)	(ca)		-bc/(ca)	|
//					|	a			b			c			|

void MGeo::Mat3PlntoXY(
				const	MgPlane3	&i_Pln,		// (I  ) 変換対象平面
				const	MgPoint3&	i_PtOrg,	// (I  ) 原点座標			
						MgMat3*		o_MatXY,	// (  O) Pln to XY平面 ３Ｄマトリックス
						MgMat3*		o_MatPln	// (  O) XY平面 to Pln ３Ｄマトリックス
				)
{
	MREAL		A = i_Pln.v.x, B = i_Pln.v.y, C = i_Pln.v.z;
	MgPoint3	PtOrgD;

	// 投影マトリックス
//	MgMat3(MREAL m11, MREAL m12, MREAL m13, MREAL m14,
//		   MREAL m21, MREAL m22, MREAL m23, MREAL m24,
//		   MREAL m31, MREAL m32, MREAL m33, MREAL m34,
//		   MREAL m41, MREAL m42, MREAL m43, MREAL m44)

//	３Ｄ→２Ｄのマトリックス作成
//	Z軸逆回転後にX軸逆回転する

	if ( Abs(C) < 0.98) {
		MREAL	Lab = (MREAL)sqrt( A * A + B * B);
		
		*o_MatXY  = MgMat3(		-B / Lab,		-A * C / Lab,	A,				0.,
								A / Lab,		-B * C / Lab,	B,				0.,
								0.,				Lab,			C,				0.,
								0.,				0.,				0.,				1. );
		PtOrgD = i_PtOrg * *o_MatXY;
		*o_MatXY -= PtOrgD;

		// 逆変換マトリックス
		*o_MatPln = MgMat3(		-B / Lab,		A / Lab,		0.,				0.,
								-A * C / Lab,	-B * C / Lab,	Lab,			0.,
								A,				B,				C,				0.,
								0.,				0.,				0.,				1. );
		*o_MatPln += i_PtOrg;
	} else {
		MREAL	Lca = (MREAL)sqrt( C * C + A * A);

		*o_MatXY  = MgMat3(		C / Lca,		-A * B / Lca,	A,				0.,
								0.,				Lca,			B,				0.,
								-A / Lca,		-B * C / Lca,	C,				0.,
								0.,				0.,				0.,				1. );
		PtOrgD = i_PtOrg * *o_MatXY;
		*o_MatXY -= PtOrgD;
							
		// 逆変換マトリックス
		*o_MatPln	= MgMat3( 	C / Lca,		0.,				-A / Lca,		0.,
								-A * B / Lca,	Lca,			-B * C / Lca,	0.,
								A,				B,				C,				0.,
								0.,				0.,				0.,				1. );
		*o_MatPln += i_PtOrg;
	}
}

//
//===================(３次元 → ２次元)=========================
//		３Ｄ座標を２Ｄ座標へ変換する
//
void MGeo::Point3Dto2D(
				const	MgPoint3&	i_P,			// (I  ) ３Ｄ座標
				const	MgMat3		&i_Tm,			// (I  ) 座標変換マトリックス
						MgPoint2*	o_P			// (  O) ２Ｄ座標
				)
{
	o_P->v[0] = i_Tm.m[0][0] * i_P.x + i_Tm.m[1][0] * i_P.y + i_Tm.m[2][0] * i_P.z + i_Tm.m[3][0];
	o_P->v[1] = i_Tm.m[0][1] * i_P.x + i_Tm.m[1][1] * i_P.y + i_Tm.m[2][1] * i_P.z + i_Tm.m[3][1];
}
/*
//
//===================(３次元 → ２次元)=========================
//		３Ｄ線分を２Ｄ線分へ座標変換する
//
inline void MGeo::Line3Dto2D(
				const MgLine3		&i_Ln,			// (I  ) ３Ｄ線分
				const MgMat3		&i_Tm,			// (I  ) 座標変換マトリックス
					  MgLine2		*o_ln			// (  O) ２Ｄ線分
				)
						{ Mg3Dto2D( i_Ln.p[0], Mat, &o_ln->p[0]); 
						  Mg3Dto2D( i_Ln.p[1], Mat, &o_ln->p[1]);}  
*/
//
//===================(３次元 → ２次元)=========================
//		３Ｄ直線を２Ｄ直線へ座標変換する
//
void MGeo::ULine3Dto2D(
				const MgULine3		&i_ULn,			// (I  ) ３Ｄ直線
				const MgMat3		&i_Mt,			// (I  ) 座標変換マトリックス
					  MgULine2		*o_uln			// (  O) ２Ｄ直線
				)
{
	Point3Dto2D( i_ULn.p, i_Mt, &o_uln->p); 

	o_uln->v.x = i_Mt.m[0][0] * i_ULn.v.x + i_Mt.m[1][0] * i_ULn.v.y + i_Mt.m[2][0] * i_ULn.v.z;		 
	o_uln->v.y = i_Mt.m[0][1] * i_ULn.v.x + i_Mt.m[1][1] * i_ULn.v.y + i_Mt.m[2][1] * i_ULn.v.z;
	o_uln->v.SetUnitize();
}
//
//===================(３次元 → ２次元)=========================
//		３Ｄ座標を２Ｄ座標へ変換する
//
MgPoint2 MGeo::Point3Dto2D2(							// (  O) ２Ｄ座標
				const	MgPoint3&	i_P,			// (I  ) ３Ｄ座標
				const	MgMat3		&i_Tm			// (I  ) 座標変換マトリックス
				)
{
	MgPoint2	Po;

	Po.x = i_Tm.m[0][0] * i_P.x + i_Tm.m[1][0] * i_P.y + i_Tm.m[2][0] * i_P.z + i_Tm.m[3][0];
	Po.y = i_Tm.m[0][1] * i_P.x + i_Tm.m[1][1] * i_P.y + i_Tm.m[2][1] * i_P.z + i_Tm.m[3][1];
	return Po;
}
/*
//
//===================(３次元 → ２次元)=========================
//		３Ｄ線分を２Ｄ線分へ座標変換する
//
inline MgLine2 MgLine3Dto2D2(							// (  O) ２Ｄ線分
				const MgLine3		&i_Ln,			// (I  ) ３Ｄ線分
				const MgMat3		&i_Tm			// (I  ) 座標変換マトリックス
				)
						{ return MgLine2( 
						  Point3Dto2D2( i_Ln.p[0], i_Tm), 
						  Point3Dto2D2( i_Ln.p[1], i_Tm));}  
*/
//
//===================(３次元 → ２次元)=========================
//		３Ｄ直線を２Ｄ直線へ座標変換する
//
MgULine2 MGeo::ULine3Dto2D2(								// (  O) ２Ｄ直線
				const MgULine3		&i_ULn,			// (I  ) ３Ｄ直線
				const MgMat3		&i_Tm			// (I  ) 座標変換マトリックス
				)
{
	MgULine2	ulno;
	Point3Dto2D( i_ULn.p, i_Tm, &ulno.p); 

	ulno.v.x = i_Tm.m[0][0] * i_ULn.v.x + i_Tm.m[1][0] * i_ULn.v.y + i_Tm.m[2][0] * i_ULn.v.z;		
	ulno.v.y = i_Tm.m[0][1] * i_ULn.v.x + i_Tm.m[1][1] * i_ULn.v.y + i_Tm.m[2][1] * i_ULn.v.z;
	ulno.v.SetUnitize();
	return ulno;
}
//
//===================(３次元 → ２次元)=========================
//		３Ｄ座標群を２Ｄ座標群へ変換する
//
void MGeo::GPoint3Dto2D(
				const	MgGPoint3	&i_GP,			// (I  ) ３Ｄ座標群
				const	MgMat3		&i_Tm,			// (I  ) 座標変換マトリックス
						MgGPoint2	*o_GP			// (  O) ２Ｄ座標群
				)
{
	int	ic;
	o_GP->m_n = 0;
	for ( ic=0; ic<i_GP.m_n; ic++)
		*o_GP += Point3Dto2D2( i_GP.m_p[ic], i_Tm);
}

//
//===================(３次元 → ２次元)=========================
//		３Ｄ座標群（ポリライン）を２Ｄ座標群へ変換する
//
void MGeo::Polyg3Dto2D(
				const	MgPolyg3&	i_Pg,			// (I  ) ３Ｄ座標群
				const	MgMat3		&iTm,			// (I  ) 座標変換マトリックス
						MgPolyg2*	o_Pg			// (  O) ２Ｄ座標群
				)
{
	int		ic;
	o_Pg->m_n = 0;
	for ( ic=0; ic<i_Pg.m_n; ic++)
		*o_Pg += Point3Dto2D2( i_Pg.m_P[ic], iTm);
}

//
//===================(３次元 → ２次元)=========================
//		３Ｄ線分群を２Ｄ線分群へ変換する
//
void MGeo::GLine3Dto2D(
				const	MgGLine3	&i_GLn,			// (I  ) ３Ｄ線分群
				const	MgMat3		&i_Tm,			// (I  ) 座標変換マトリックス
						MgGLine2*	o_gln			// (  O) ２Ｄ線分群
				)
{
	int		ic;
	o_gln->m_n = 0;
	for ( ic=0; ic<i_GLn.m_n; ic++)
		*o_gln += Line3Dto2D2( i_GLn.m_ln[ic], i_Tm);
}

//
//===================(３次元 → ２次元)=========================
//		３Ｄ多角形群（ポリライン群）を２Ｄ多角形群へ変換する
//
void MGeo::GPolyg3Dto2D(
				const MgGPolyg3		&i_gPg,			// (I  ) ３Ｄ多角形群
				const MgMat3		&i_Tm,			// (I  ) 座標変換マトリックス
					  MgGPolyg2		*o_gpg			// (  O) ２Ｄ多角形群
				)
{
	int	ic;
	MGPOLYG2( pgw, MX_PNT0);

	o_gpg->SetCount0();
	for ( ic=0; ic<i_gPg.m_n; ic++) {
		Polyg3Dto2D( i_gPg.m_Pg[ic], i_Tm, &pgw);
		(*o_gpg) += pgw;
	}
}
//
//===================(２次元 → ３次元)=========================
//		２Ｄ座標を３Ｄ座標へ変換する
//
void MGeo::Point2Dto3D(
				const	MgPoint2&	i_pt,			// (I  ) ２Ｄ座標
				const	MgMat3		&i_Tm,			// (I  ) 座標変換マトリックス
						MgPoint3	*o_Pt			// (  O) ３Ｄ座標
				)
{
	o_Pt->v[0] = i_Tm.m[0][0] * i_pt.x + i_Tm.m[1][0] * i_pt.y + i_Tm.m[3][0];
	o_Pt->v[1] = i_Tm.m[0][1] * i_pt.x + i_Tm.m[1][1] * i_pt.y + i_Tm.m[3][1];
	o_Pt->v[2] = i_Tm.m[0][2] * i_pt.x + i_Tm.m[1][2] * i_pt.y + i_Tm.m[3][2];
}
/*
//
//===================(２次元 → ３次元)=========================
//		２Ｄ線分を３Ｄ線分へ座標変換する
//
inline void MgLine2Dto3D(
				const MgLine2&		i_ln,			// (I  ) ２Ｄ線分
				const MgMat3		&i_Tm,			// (I  ) 座標変換マトリックス
					  MgLine3		*o_Ln			// (  O) ３Ｄ線分
				)
						{ Mg2Dto3D( i_ln.p[0], i_Tm, &o_Ln->p[0]);
						  Mg2Dto3D( i_ln.p[1], i_Tm, &o_Ln->p[1]);}  
*/
//
//===================(２次元 → ３次元)=========================
//		２Ｄ直線を３Ｄ直線へ座標変換する
//
void MGeo::ULine2Dto3D(
				const MgULine2		&i_uln,			// (I  ) ２Ｄ直線
				const MgMat3		&i_Tm,			// (I  ) 座標変換マトリックス
					  MgULine3		*o_ULn			// (  O) ３Ｄ直線
				)
{
	Point2Dto3D( i_uln.p, i_Tm, &o_ULn->p); 

	o_ULn->v.x = i_Tm.m[0][0] * i_uln.v.x + i_Tm.m[1][0] * i_uln.v.y + i_Tm.m[3][0];
	o_ULn->v.y = i_Tm.m[0][1] * i_uln.v.x + i_Tm.m[1][1] * i_uln.v.y + i_Tm.m[3][1];
	o_ULn->v.z = i_Tm.m[0][2] * i_uln.v.x + i_Tm.m[1][2] * i_uln.v.y + i_Tm.m[3][2];
	o_ULn->v.SetUnitize();
}
//
//===================(２次元 → ３次元)=========================
//		２Ｄ座標を３Ｄ座標へ変換する
//
MgPoint3 MGeo::Point2Dto3D3(							// (  O) ３Ｄ座標
				const	MgPoint2&	i_Pt,			// (I  ) ２Ｄ座標
				const	MgMat3		&i_Tm			// (I  ) 座標変換マトリックス
				)
{
	MgPoint3	Pto;
	Pto.x = i_Tm.m[0][0] * i_Pt.x + i_Tm.m[1][0] * i_Pt.y + i_Tm.m[3][0];
	Pto.y = i_Tm.m[0][1] * i_Pt.x + i_Tm.m[1][1] * i_Pt.y + i_Tm.m[3][1];
	Pto.z = i_Tm.m[0][2] * i_Pt.x + i_Tm.m[1][2] * i_Pt.y + i_Tm.m[3][2];
	return Pto;
}
/*
//===================(２次元 → ３次元)=========================
//		２Ｄ線分を３Ｄ線分へ座標変換する
//
inline MgLine3 MgLine2Dto3D3(							// (  O) ３Ｄ線分
				const MgLine2&		i_ln,			// (I  ) ２Ｄ線分
				const MgMat3		&i_Tm			// (I  ) 座標変換マトリックス
				)
						{ return MgLine3(
						  Mg2Dto3D( i_ln.p[0], i_Tm), 
						  Mg2Dto3D( i_ln.p[1], i_Tm))}  
*/
//
//===================(２次元 → ３次元)=========================
//		２Ｄ直線を３Ｄ直線へ座標変換する
//
MgULine3 MGeo::ULine2Dto3D3(								// (  O) ３Ｄ直線
				const MgULine2		&i_uln,			// (I  ) ２Ｄ直線
				const MgMat3		&i_Tm			// (I  ) 座標変換マトリックス
				)
{
	MgULine3	ULno;
	Point2Dto3D( i_uln.p, i_Tm, &ULno.p); 

	ULno.v.x = i_Tm.m[0][0] * i_uln.v.x + i_Tm.m[1][0] * i_uln.v.y;
	ULno.v.y = i_Tm.m[0][1] * i_uln.v.x + i_Tm.m[1][1] * i_uln.v.y;
	ULno.v.z = i_Tm.m[0][2] * i_uln.v.x + i_Tm.m[1][2] * i_uln.v.y;
	ULno.v.SetUnitize();
	return ULno;
}
//
//===================(２次元 → ３次元)=========================
//		２Ｄ座標群を３Ｄ座標群へ変換する
//
void MGeo::GPoint2Dto3D(
				const	MgGPoint2	&i_GPt,			// (I  ) ２Ｄ座標群
				const	MgMat3		&i_Tm,			// (I  ) 座標変換マトリックス
						MgGPoint3	*o_GPt			// (  O) ３Ｄ座標群
				)
{
	int	ic;
	o_GPt->m_n = 0;
	for ( ic=0; ic<i_GPt.m_n; ic++)
		*o_GPt += Point2Dto3D3( i_GPt.m_p[ic], i_Tm);
}

//
//===================(２次元 → ３次元)=========================
//		２Ｄ座標群（ポリライン）を３Ｄ座標群へ変換する
//
void MGeo::Polyg2Dto3D(
				const	MgPolyg2&	i_Pg,			// (I  ) ２Ｄ座標群
				const	MgMat3		&i_Tm,			// (I  ) 座標変換マトリックス
						MgPolyg3*	o_Pg			// (  O) ３Ｄ座標群
				)
{
	int ic;
	o_Pg->m_n = 0;
	for ( ic=0; ic<i_Pg.m_n; ic++)
		*o_Pg += Point2Dto3D3( i_Pg.m_p[ic], i_Tm);
}

//
//===================(２次元 → ３次元)=========================
//		２Ｄ直線群を３Ｄ直線群へ変換する
//
void MGeo::GLine2Dto3D(
				const	MgGLine2&	i_gln,			// (I  ) ２Ｄ直線群
				const	MgMat3		&i_Tm,			// (I  ) 座標変換マトリックス
						MgGLine3	*o_GLn			// (  O) ３Ｄ直線群
				)
{
	int ic;
	o_GLn->m_n = 0;
	for ( ic=0; ic<i_gln.m_n; ic++)
		*o_GLn += Line2Dto3D3( i_gln.m_ln[ic], i_Tm);
}
//
//===================(２次元 → ３次元)=========================
//		２Ｄ多角形群（ポリライン群）を３Ｄ多角形群へ変換する
//
void MGeo::GPolyg2Dto3D(
				const MgGPolyg2		&i_GPg,			// (I  ) ２Ｄ多角形群
				const MgMat3		&i_Tm,			// (I  ) 座標変換マトリックス
					  MgGPolyg3		*o_GPg			// (  O) ３Ｄ多角形群
				)
{
	int	ic;
	MGPOLYG3( Pgw, MX_PNT0);
	
	o_GPg->SetCount0();
	for ( ic=0; ic<i_GPg.m_n; ic++) {
		Polyg2Dto3D( i_GPg.m_pg[ic], i_Tm, &Pgw);
		(*o_GPg) += Pgw;
	}
}
/*
//
//===========================================================================
//		行列式関数
//
void madd(double a[], double b[], double c[], int la, int lb, int lc, int m, int n)
{
	int i, j, ka, kb, kc;
	double *p, *q, *r;

	if(m <= 0 || n <= 0 || la < n || lb < n || lc < n)
	{
		fprintf(stderr, "Error : Illegal parameter  in madd()\n");
		return;
	}
	for (i=0, ka=kb=kc=0; i<m; i++, ka+=la, kb+=lb, kc+=lc)
		for (j=0, p=a+ka, q=b+kb, r=c+kc; j<n; j++)
			*r++ = *p++ + *q++;
	return;
}

void msub(double a[], double b[], double c[], int la, int lb, int lc, int m, int n)
{
	int i, j, ka, kb, kc;
	double *p, *q, *r;

	if(m <= 0 || n <= 0 || la < n || lb < n || lc < n)
	{
		fprintf(stderr, "Error : Illegal parameter  in msub()\n");
		return;
	}
	for (i=0, ka=kb=kc=0; i<m; i++, ka+=la, kb+=lb, kc+=lc)
		for (j=0, p=a+ka, q=b+kb, r=c+kc; j<n; j++)
			*r++ = *p++ - *q++;
	return;
}

void mmuLn1(double a[], double b[], int la, int lb, int m)
{
	int i, j, ka, l;
	double *p, *q, *r, w, *work;

	if(m <= 0 || la < m || lb < m)
	{
		fprintf(stderr, "Error : Illegal parameter  in mmuLn1()\n");
		return;
	}
	work = (double *)new char[m * m * sizeof(double)];
	if(work == NULL)
	{
		fprintf(stderr, "Error : Out of memory  in mmuLn1()\n");
		return;
	}
	for (i=0, ka=0, r=work; i<m; i++, ka+=la)
	{
		for (l=0; l<m; l++)
		{
			w = 0.;
			for (j=0, p=a+ka, q=b+l; j<m; j++)
			{
				w += *p++ * *q;
				q += lb;
			}
			*r++ = w;
		}
	}
	mmove(work, a, m, la, m, m);
	free((MCHAR* )work);
	return;
}

void mmuLn2(double a[], double b[], double c[], int la, int lb, int lc, int m, int n, int k)
{
	int i, j, ka, kc, l;
	double *p, *q, *r, w;

	if(m <= 0 || n <= 0 || k <= 0 || la < n || lb < k || lc < k)
	{
		fprintf(stderr, "Error : Illegal parameter  in mmuLn2()\n");
		return;
	}
	for (i=0, ka=kc=0; i<m; i++, ka+=la, kc+=lc)
	{
		for (l=0, r=c+kc; l<k; l++)
		{
			w = 0.;
			for (j=0, p=a+ka, q=b+l; j<n; j++)
			{
				w += *p++ * *q;
				q += lb;
			}
			*r++ = w;
		}
	}
}

void mtra1(double a[], int l, int m, int n)
{
	int i, j, ka, mn;
	double *p, *q;

	if(m <= 0 || n <= 0 || l < m || l < n || m < n)
	{
		fprintf(stderr, "Error : Illegal parameter  in mtra1()\n");
		return;
	}
	mn = m;
	if(mn < n)	mn = n;
	for (i=0, ka=0; i<mn-1; i++, ka+=l)
		for (j=i+1, p=a+ka+j, q=p+l-1; j<mn; j++, q+=l)
			swapd(p++, q);
}

void mtra2(double a[], double b[], int la, int lb, int m, int n)
{
	int i, j, ka;
	double *p, *q;

	if(m <= 0 || n <= 0 || la < n || lb < m)
	{
		fprintf(stderr, "Error : Illegal parameter  in mtra2()\n");
		return;
	}
	for (i=0, ka=0; i<m; i++, ka+=la)
		for (j=0, p=a+ka, q=b+i; j<n; j++, q+=lb) *q = *p++;
}

double minver(double a[], int l, int m, double eps)
{
	int i, j, k, *p, r, s, u, v, *work;
	double api, pivot, *q, *q1, w, w1, wmax;

	if(m < 2 || l < m || eps <= 0.)
	{
		fprintf(stderr, "Error : Illegal parameter  in minver()\n");
		return 0.;
	}
	work = (int *)new char[m * sizeof(int)];
	if(work == NULL)
	{
		fprintf(stderr, "Error : Out of memory  in minver()\n");
		return 0.;
	}
	w1 = 1.;
	for (i=0, p=work; i<m; i++)	*p++ = i;
	for (k=0, u=0; k<m; k++, u+=l)
	{
		wmax = 0.;
		for (i=k; i<m; i++)
		{
			w = fabs(*(a + i * l + k));
			if(w > wmax)
			{
				wmax = w;
				r = i;
			}
		}
		api = fabs(pivot = *(a + r * l + k));
		if(api < eps)
		{
			fprintf(stderr, "Error : api < eps  in minver()\n");
			free((MCHAR* )work);
			return w1;
		}
		w1 *= pivot;
		v = r * l;
		if(r != k)
		{
			w1 = -w1;
			swapi(work + k, work + r);
			for (j=0, q=a+u, q1=a+v; j<m; j++)	swapd(q++, q1++);
		}
		for (i=0, q=a+u; i<m; i++)	*q++ /= pivot;
		for (i=0, v=0; i<m; i++, v+=l)
		{
			if(i != k)
			{
				s = v + k;
				w = *(a + s);
				if(w != 0.)
				{
					for (j=0, q=a+u, q1=a+v; j<m; j++, q++, q1++)
						if (j != k)	*q1 -= w * *q;
					*(a + s) = - w / pivot;
				}
			}
		}
		*(a + u + k) = 1. / pivot;
	}
	for (i=0; i<m; i++)
	{
		for (;;)
		{
			k = *(work + i);
			if(k == i)	break;
			swapi(work + k, work + i);
			for (j=0, u=0; j<m; j++, u+=l)	swapd(a + u + i, a + u + k);
		}
	}
	free((MCHAR* )work);
	return w1;
}

void mmove(double a[], double b[], int la, int lb, int m, int n)
{
	int i, j, ka, kb;
	double *p, *q;

	if(m <= 0 || n <= 0 || la < n || lb < n)
	{
		fprintf(stderr, "Error : Illegal parameter  in mmove()\n");
		return;
	}
	for (i=ka=kb=0; i<m; i++, ka+=la, kb+=lb)
		for (j=0, p=a+ka, q=b+kb; j<n; j++)	*q++ = *p++;
	return;
}

void mswap(double a[], double b[], int la, int lb, int m, int n)
{
	int i, j, ka, kb;
	double *p, *q;

	if(m <= 0 || n <= 0 || la < n || lb < n)
	{
		fprintf(stderr, "Error : Illegal parameter  in mswap()\n");
		return;
	}
	for (i=ka=kb=0; i<m; i++, ka+=la, kb+=lb)
		for (j=0, p=a+ka, q=b+kb; j<n; j++)	swapd(p++, q++);
	return;
}

int jacobi(double a[], double v[], int l, int m, int *nr, double eps)
{
	int n, i, j, r, c, k1, k2, s1, s2, s3, s5, s6;
	double wmax, w, a1, a2, a3, t1, ta, si, co;

	if(m < 2 || *nr < 1 || eps <= 0.0)	return 999;
	n = 0;
	for (i=0; i<m - 1; i++)
	{
		k1 = i * l;
		for (j=i+1; j<m; j++)	if(a[k1 + j] != a[j * l + i])	return 999;
	}
	for (i=0; i<m; i++)
	{
		k1 = i * l;
		for (j=0; j<m; j++)	if(i != j)	v[k1 + j] = 0.0;
		v[k1 + i] = 1.0;
	}
	while(1)
	{
		wmax = 0.0;
		for (i=0; i<m; i++)
		{
			k1 = i * l;
			for (j=i+1; j<m; j++)
			{
				w = fabs(a[k1 + j]);
				if(w > wmax)
				{
					wmax = w;
					r = i;
					c = j;
				}
			}
		}
		if(wmax <= eps)
		{
			*nr = n;
			return 0;
		}
		if(n >= *nr)
		{
			*nr = n;
			return 1;
		}
		k1 = r * l;
		k2 = c * l;
		s1 = k1 + r;
		s2 = k2 + c;
		s3 = k1 + c;
		n++;
		a1 = a[s1];
		a2 = a[s2];
		a3 = a[s3];
		t1 = fabs(a1 - a2);
		ta = 2.0 * a3 / (t1 + (MREAL)sqrt(t1 * t1 + 4.0 * a3 * a3));
		if(a1 < a2)	ta = - ta;
		co = (MREAL)sqrt(1.0 / (ta * ta + 1.0));
		si = ta * co;
		for (i=0; i<m; i++)
		{
			s5 = i * l + r;
			s6 = i * l + c;
			w = v[s5];
			v[s5] = w * co + v[s6] * si;
			v[s6] = - w * si + v[s6] * co;
			if(i != r && i != c)
			{
				w = a[s5];
				a[s5] = w * co + a[s6] *si;
				a[s6] = - w * si + a[s6] * co;
				a[k1 + i] = a[s5];
				a[k2 + i] = a[s6];
			}
		}
		a[s1] = a1 * co * co + a2 * si * si + 2.0 * a3 * co * si;
		a[s2] = a1 + a2 - a[s1];
		a[s3] = 0.0;
		a[k2 + r] = 0.0;
	}
}

void swapi(int *a, int *b)
{
	int w;

	w = *a;
	*a = *b;
	*b = w;
	return;
}

void swapd(double *a, double *b)
{
	double w;

	w = *a;
	*a = *b;
	*b = w;
	return;
}
*/

} // namespace MC
#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgGeo.h
//
//		図形処理
//
//
//  K.Matsu           09/01/18    Created.
//==========================================================================================
//
#include "MgPoint.h"
#include "MgAngle.h"
#include "MgPolygon.h"
#include "MgArc.h"
#include "MgCircle.h"
#include "MgTol.h"

#include "MgMat.h"

#ifdef DLL_EXPORT_GEO
	#undef DLL_EXPORT_GEO
#endif

#ifdef DLL_EXPORT_GEO_DO
	#pragma message( "<<< MgGeo・dll_EXport >>>")
	#define DLL_EXPORT_GEO							__declspec( dllexport)
#else
	#ifdef _DEBUG								// Release Mode で inline関数がundefになる不具合対応
		#ifdef DLL_NO_IMPORT_GEO_DO
			#define DLL_EXPORT_GEO
		#else
//			#pragma message( "=== MgGeo・dll_IMport ===")
			#define DLL_EXPORT_GEO						__declspec( dllimport)
		#endif
	#else
		#define DLL_EXPORT_GEO
	#endif
#endif

namespace MC
{

class DLL_EXPORT_GEO MGeo
{
public:
	//======================( １次元 )==============================
	//
	static MREAL Abs(MREAL x)
						{return ((x > 0) ? x : -x);}
	static MINT Abs(MINT x)
						{return ((x > 0) ? x : -x);}
	static bool Zero(MREAL x, MREAL tol = MGPTOL->D)
						{return (x < tol && x > -tol);}
	static bool Equal(MREAL x, MREAL y, MREAL tol = MGPTOL->D)
						{return (x - y < tol && y - x < tol);}
	static bool EQ(MREAL x, MREAL y, MREAL tol = MGPTOL->D)					// MgEqualと同じ
						{return (x - y < tol && y - x < tol);}
	static bool LT(MREAL x, MREAL y, MREAL tol = MGPTOL->D)
						{return (x < y - tol);}
	static bool LE(MREAL x, MREAL y, MREAL tol = MGPTOL->D)
						{return (x <= y + tol);}
	static bool GT(MREAL x, MREAL y, MREAL tol = MGPTOL->D)
						{return (x > y + tol);}
	static bool GE(MREAL x, MREAL y, MREAL tol = MGPTOL->D)
						{return (x >= y - tol);}

	//======================( ２次元 )==============================
	// MgVect2, MgPoint2

	static MREAL Abs_2( const MgVect2& v)											// ベクトルの長さの２乗	Abs**2
				 		{ return ( v.x*v.x+v.y*v.y);}
	static MREAL Abs( const MgVect2& v)											// ベクトルの長さ		ABS
				 		{ return (MREAL)sqrt( v.x*v.x+v.y*v.y);}
	static MREAL Dist_2( const MgPoint2& P1, const MgPoint2& P2)					// ２点間の距離の２乗 MgDist**2
						{ return ( (P2.x-P1.x)*(P2.x-P1.x)+
								   (P2.y-P1.y)*(P2.y-P1.y));}
	static MREAL Dist( const MgPoint2& P1, const MgPoint2& P2)						// ２点間の距離
						{ return (MREAL)sqrt( (P2.x-P1.x)*(P2.x-P1.x)+
											   (P2.y-P1.y)*(P2.y-P1.y));}
	static bool Zero( MgVect2 v1)													// P1 == (0.,0.)	判定
						{ return Abs_2( v1) <= g_gTol.D_2;}
	static bool Zero( MgVect2 v1, MREAL tol)										// P1 == (0.,0.)	誤差指定の判定
						{ return Abs_2( v1) <= tol * tol;}
	static bool Equal( MgVect2 v1, MgVect2 v2)										// P1 == P2 		判定
				 		{ MgVect2 vt = v2 - v1; return Abs_2( vt) <= g_gTol.D_2;}
	static bool EqualV( MgVect2 v1, MgVect2 v2)										// V1 == V2 		ベクトル型式の角度の等号判定
				 		{ MgVect2 vt = v2 - v1; return Abs_2( vt) <= g_gTol.A_2;}
	static bool Equal( MgVect2 v1, MgVect2 v2, MREAL tol)							// P1 == P2			誤差指定の判定
				 		{ MgVect2 vt = v2 - v1; return Abs_2( vt) <= tol * tol;}
	static bool Parallel( MgVect2 v1, MgVect2 v2)									// V1とV2が平行であるかを確認
				 		{ MREAL ro = v1 ^ v2; MREAL ri = v1 * v2;
						  if( ro < 0) ro = - ro; if(ri < 0) ri = -ri;
						  return (ro <= ri * g_gTol.A);}
	static bool Parallel( MgVect2 v1, MgVect2 v2, MREAL* so, MREAL* si)			// V1とV2が平行であるかを確認し、外積と内積を出力
				 		{ MREAL ro = v1 ^ v2; MREAL ri = v1 * v2;
				 		  *so = ro; *si = ri;
						  if (ro < 0) ro = - ro; if (ri < 0) ri = -ri;
						  return (ro <= ri * g_gTol.A);}
	static void SVal( MgVect2 ps, MgVect2 pe, MgVect2 p1, MgVect2 v1,
						MREAL* ss, MREAL* se)
																					// 直線(p1,v1)と線分(ps, pe)の交点を求めるのに使用可能
																					// ss = 直線(p1,v1)から点psへの垂直距離
																					// se = 直線(p1,v1)から点peへの垂直距離
						{ *ss = (v1 ^ (ps - p1));									// 線分2の始点側S
						  *se = (v1 ^ (pe - p1));}									// 線分2の終点側S
	static MgVect2 UnitizeV2( const MgVect2& v1, MREAL i_Tol = g_gTol.D	);		// 長さ１に正規化する

	static MgVect2 RotL90( const MgVect2& v1)										// 左90゜回転
						{ return	MgVect2( -v1.y, v1.x);}
	static MgVect2 Rot180( const MgVect2& v1)										// 180゜回転
						{ return	MgVect2( -v1.x, -v1.y);}
	static MgVect2 RotR90( const MgVect2& v1)										// 右90゜回転
						{ return	MgVect2( v1.y, -v1.x);}

	//======================( ３次元 )==============================
	// MgVect3, MgPoint3

	static MREAL Abs_2( MgVect3 v)													// ベクトルの長さの２乗	MgAbs**2
				 		{ return v.x*v.x+v.y*v.y+v.z*v.z;}
	static MREAL Abs( MgVect3 v)													// ベクトルの長さ		MGABS
				 		{ return (MREAL)sqrt(v.x*v.x+v.y*v.y+v.z*v.z);}
	static MREAL Dist_2( const MgPoint3& P1, const MgPoint3& P2)					// ２点間の距離の２乗 MgDist**2
						{ return ( (P2.x-P1.x)*(P2.x-P1.x)+
								   (P2.y-P1.y)*(P2.y-P1.y)+
								   (P2.z-P1.z)*(P2.z-P1.z));}
	static MREAL Dist( const MgPoint3& P1, const MgPoint3& P2)						// ２点間の距離
						{ return (MREAL)sqrt( (P2.x-P1.x)*(P2.x-P1.x)+
											   (P2.y-P1.y)*(P2.y-P1.y)+
											   (P2.z-P1.z)*(P2.z-P1.z));}
	static bool Zero( MgVect3 v1)													// P1 == (0.,0.,0.)	判定
						{ return Abs_2(v1) <= g_gTol.D_2;}
	static bool Zero( MgVect3 v1, MREAL tol)										// P1 == (0.,0.,0.)	誤差指定の判定
						{ return Abs_2(v1) <= tol * tol;}
	static bool Equal( MgVect3 v1, MgVect3 v2)										// P1 == P2 		判定== 判定
				 		{ MgVect3 vt = v2 - v1; return Abs_2(vt) <= g_gTol.D_2;}  
	static bool EqualV( MgVect3 v1, MgVect3 v2)										// V1 == V2 		ベクトル型式の角度の等号判定
				 		{ MgVect3 vt = v2 - v1; return Abs_2(vt) <= g_gTol.A_2;}  
	static bool Equal( MgVect3 v1, MgVect3 v2, MREAL tol)							// P1 == P2			誤差指定の判定== 判定
				 		{ MgVect3 vt = v2 - v1; return Abs_2(vt) <= tol * tol;}  
	static bool Parallel( MgVect3 v1, MgVect3 v2)									// V1とV2が平行であるかを確認
				 		{ MgVect3 vo = v1 ^ v2; MREAL fi = v1 * v2;
						  return ( Abs_2(vo) <= fi * fi * g_gTol.A_2);}
	static bool Parallel( MgVect3 v1, MgVect3 v2, MgVect3 *pso, MREAL* si)			// V1とV2が平行であるかを確認し、外積と内積を出力
				 		{ MgVect3 vo = v1 ^ v2; MREAL fi = v1 * v2;
				 		  *pso = vo; *si = fi;
						  return ( Abs_2(vo) <= fi * fi * g_gTol.A_2);}
	static MgVect3 UnitizeV3( const MgVect3& V1, MREAL i_Tol = g_gTol.D);			// 単位ベクトルを求める
	static MgVect3 TaniV3( const MgVect3& V1, MREAL i_Tol = g_gTol.D);

	static MgVect3 RotL90( const MgVect3& v1)										// 左90゜回転
						{ return	MgVect3( -v1.y, v1.x, v1.z);}
	static MgVect3 Rot180( const MgVect3& v1)										// 180゜回転
						{ return	MgVect3( -v1.x, -v1.y, v1.z);}
	static MgVect3 RotR90( const MgVect3& v1)										// 右90゜回転
						{ return	MgVect3( v1.y, -v1.x, v1.z);}

#ifdef _MgPoint3A
	//
	//======================( ２次元 )==============================
	static MgPoint2a Norma( const MgPoint2& p);
#endif
	
	////
	////======================( ３次元 )==============================
	////		ベクトルの単位ベクトルを求める
	////
	//static MgVect3 MgUnitizeV3( const MgVect3& v, MREAL i_Tol = g_gTol.D);		//	(  O) 単位ベクトル

#ifdef _MgPoint3A

	static MgPoint3a Norma( const MgPoint3& p);
#endif
	
	// ---------------------( ２次元 )------------------------------
	//
	//	２次元直線
	//
	static MgULine2 Rev( const MgULine2& ULn1)										// 直線反転
						{ return MgULine2C( ULn1.p, - ULn1.v);}
	//	２次元線分
	//
	static MgLine2 Rev( const MgLine2& Ln1)											// 線分 反転
						{ return MgLine2( Ln1.p[1], Ln1.p[0]);}

	static bool Parallel( const MgLine2& Ln1, const MgLine2& Ln2)							// Ln1とLn2が平行であるかを確認
				 		{ MgVect2 v1 = MgVect2C(Ln1); MgVect2 v2 = MgVect2C(Ln2);
						  return Parallel( v1, v2);}
	static bool Parallel( const MgLine2& Ln1, const MgLine2& Ln2, MREAL* so, MREAL* si)	// Ln1とLn2が平行であるかを確認し、外積と内積を出力
				 		{ MgVect2 v1 = MgVect2C(Ln1); MgVect2 v2 = MgVect2C(Ln2);
						  return Parallel( v1, v2, so, si);}

	// ---------------------( ３次元 )------------------------------
	//
	//	３次元直線
	//
	static MgULine3 Rev( MgULine3& ULn1)													// 直線反転
						{ return MgULine3(ULn1.p, - ULn1.v);}

	static MgLine3 Rev( const MgLine3& Ln1)													// 線分 反転
						{ return MgLine3( Ln1.p[1], Ln1.p[0]);}

	static bool Parallel( const MgLine3& Ln1, const MgLine3& Ln2)							// Ln1とLn2が平行であるかを確認
				 		{ MgVect3 v1 = MgVect3C(Ln1); MgVect3 v2 = MgVect3C(Ln2);
						  return Parallel( v1, v2);}
	static bool Parallel( const MgLine3& Ln1, const MgLine3& Ln2, MgVect3 *pso, MREAL* si)	// Ln1とLn2が平行であるかを確認し、外積と内積を出力
				 		{ MgVect3 v1 = MgVect3C(Ln1); MgVect3 v2 = MgVect3C(Ln2);
						  return Parallel( v1, v2, pso, si);}


	// ---------------------( ２次元 )------------------------------
	//
	//	２次元円弧
	//
	static bool Parallel( const MgArc2 &arc1, const MgArc2 &arc2)						// arc1とarc2が平行であるかを確認
				 		{ return ( arc1.pc == arc2.pc);}

	static MINT Arc2Div( const MgArc2 &arc1, MINT n, const MgGPoint2 *gpto);				// 円弧をn等分する

	// ---------------------( ３次元 )------------------------------
	//
	//	３次元円弧
	//
	static bool Parallel( const MgArc3 &Arc1, const MgArc3 &Arc2)						// Arc1とArc2が平行であるかを確認
				 		{ return ( Arc1.Pc == Arc2.Pc) &&
								   Parallel( Arc1.V, Arc2.V);}

	static MINT Arc3Div( const MgArc3 &arc1, MINT n, const MgGPoint3 *gPto);				// 円弧をn等分する

	// ---------------------( ２次元 )------------------------------
	//
	//	２次元円
	//
	static bool Parallel( const MgCir2 &cir1, const MgCir2 &cir2)						// cir1とcir2が平行であるかを確認
				 		{ return ( cir1.pc == cir2.pc);}

	// ---------------------( ３次元 )------------------------------
	//
	//	３次元円
	//
	static bool Parallel( const MgCir3 &Cir1, const MgCir3 &Cir2)						// Cir1とCir2が平行であるかを確認
				 		{ return ( Cir1.Pc == Cir2.Pc) &&
								   Parallel( Cir1.V, Cir2.V);}

	/////////////////////////////////////////////////////////////////////////////
	//		直線　形状演算
	//
	// ---------------------( ２次元 )------------------------------
	//
	//	ベクトルと水平線の角度を求める
	//
	static MREAL GetAngleVH2(						// (  O) 左回転角度　（度）
													//		ベクトルの長さがg_gTol.D未満の場合は0度を返す
					const	MgVect2&	v1			// (I  ) ベクトル
			);

	/////////////////////////////////////////////////////////////////////////////
	//	２点と水平線の角度を求める
	//
	static MREAL GetAngle2PtH2(					// (  O) 左回転角度　（度）
													//		始点、終点間の距離がg_gTol.D未満の場合は0度を返す
					const	MgPoint2&	p1,			// (I  ) 始点
					const	MgPoint2&	p2			// (I  ) 終点
			);

	/////////////////////////////////////////////////////////////////////////////
	//	２ベクトルの角度を求める
	//
	static MREAL GetAngle2V2(						// (  O) 左回転角度　（度）
													//		ベクトルの長さがg_gTol.D未満の場合は0度を返す
					const	MgVect2&	v1,			// (I  ) ベクトル1
					const	MgVect2&	v2			// (I  ) ベクトル2
			);

	/////////////////////////////////////////////////////////////////////////////
	//	３点で作られる２直線の角度を求める
	//
	static MREAL GetAngle3Pt2(						// (  O) 左回転角度　（度）
													//		2点間の距離がg_gTol.D未満の場合は0度を返す
					const	MgPoint2&	p1,			// (I  ) 点1
					const	MgPoint2&	p2,			// (I  ) 点2
					const	MgPoint2&	p3			// (I  ) 点3
			);

	/////////////////////////////////////////////////////////////////////////////
	//	２直線の角度を求める
	//
	static MREAL GetAngle2ULn2(					// (  O) 左回転角度　（度）
													//		2点間の距離がg_gTol.D未満の場合は0度を返す
					const	MgULine2&	ULn1,		// (I  ) 線分1
					const	MgULine2&	ULn2		// (I  ) 線分2
			);

	/////////////////////////////////////////////////////////////////////////////
	//	直線と線分の角度を求める
	//
	static MREAL GetAngleULnLn2(					// (  O) 左回転角度　（度）
													//		2点間の距離がg_gTol.D未満の場合は0度を返す
					const	MgULine2&	ULn1,		// (I  ) 直線1
					const	MgLine2&	Ln2			// (I  ) 線分2
			);

	//	-------------------------------------------------------
	static MREAL GetAngleLnULn2(					// (  O) 左回転角度　（度）
													//		2点間の距離がg_gTol.D未満の場合は0度を返す
					const	MgLine2&	Ln1,		// (I  ) 線分1
					const	MgULine2&	ULn2		// (I  ) 直線2
			);

	/////////////////////////////////////////////////////////////////////////////
	//	２線分の角度を求める
	//
	static MREAL GetAngle2Ln2(						// (  O) 左回転角度　（度）
													//		2点間の距離がg_gTol.D未満の場合は0度を返す
					const	MgLine2&	Ln1,		// (I  ) 線分1
					const	MgLine2&	Ln2			// (I  ) 線分2
			);

	/////////////////////////////////////////////////////////////////////////////
	//	２ベクトルの角度を求める
	//
	static MREAL GetAngle2V3(						// (  O) 左回転角度　（度）
													//		ベクトルの長さがg_gTol.D未満の場合は0度を返す
					const	MgVect3&	v1,			// (I  ) ベクトル1
					const	MgVect3&	v2,			// (I  ) ベクトル2
					const	MgVect3&	vn			// (I  ) ２ベクトルに直交する単位ベクトル
			);

	/////////////////////////////////////////////////////////////////////////////
	//	３点で作られる２直線の角度を求める
	//
	static MREAL GetAngle3Pt3(						// (  O) 左回転角度　（度）
													//		2点間の距離がg_gTol.D未満の場合は0度を返す
					const	MgPoint3&	p1,			// (I  ) 点1
					const	MgPoint3&	p2,			// (I  ) 点2
					const	MgPoint3&	p3,			// (I  ) 点3
					const	MgVect3&	vn			// (I  ) 点1,点,2点3を通る平面に直交する単位ベクトル
			);

	/////////////////////////////////////////////////////////////////////////////
	//	２直線の角度を求める
	//
	static MREAL GetAngle2ULn3(					// (  O) 左回転角度　（度）
													//		2点間の距離がg_gTol.D未満の場合は0度を返す
					const	MgULine3&	ULn1,		// (I  ) 直線1
					const	MgULine3&	ULn2,		// (I  ) 直線2
					const	MgVect3&	vn			// (I  ) ２直線に直交する単位ベクトル
			);

	/////////////////////////////////////////////////////////////////////////////
	//	直線と線分の角度を求める
	//
	static MREAL GetAngleULnLn3(					// (  O) 左回転角度　（度）
													//		2点間の距離がg_gTol.D未満の場合は0度を返す
					const	MgULine3&	ULn1,		// (I  ) 直線1
					const	MgLine3&	Ln2,		// (I  ) 線分2
					const	MgVect3&	vn			// (I  ) 直線と線分に直交する単位ベクトル
			);

	//	-------------------------------------------------------
	static MREAL GetAngleLnULn3(					// (  O) 左回転角度　（度）
													//		2点間の距離がg_gTol.D未満の場合は0度を返す
					const	MgLine3&	Ln1,		// (I  ) 線分1
					const	MgULine3&	ULn2,		// (I  ) 直線2
					const	MgVect3&	vn			// (I  ) 線分と直線に直交する単位ベクトル
			);

	/////////////////////////////////////////////////////////////////////////////
	//	２線分の角度を求める
	//
	static MREAL GetAngle2Ln3(						// (  O) 左回転角度　（度）
													//		2点間の距離がg_gTol.D未満の場合は0度を返す
					const	MgLine3&	Ln1,		// (I  ) 線分1
					const	MgLine3&	Ln2,		// (I  ) 線分2
					const	MgVect3&	vn			// (I  ) ２線分に直行する単位ベクトル
			);

	/////////////////////////////////////////////////////////////////////////////
	//		直線　平面　形状演算
	//
	// ---------------------( ３次元 )------------------------------
	//
	//	ベクトルと平面の角度を求める
	//
	static MREAL GetAngleVPln3(					// (  O) 左回転角度　（度）
													//		2点間の距離がg_gTol.D未満の場合は0度を返す
					const	MgVect3&	v1,			// (I  ) ベクトル1
					const	MgPlane3&	Pln2		// (I  ) 平面2
			);

	/////////////////////////////////////////////////////////////////////////////
	//	直線と平面の角度を求める
	//
	static MREAL GetAngleULnPln3(					// (  O) 左回転角度　（度）
													//		2点間の距離がg_gTol.D未満の場合は0度を返す
					const	MgULine3&	ULn1,		// (I  ) 直線1
					const	MgPlane3&	Pln2		// (I  ) 平面2
			);

	/////////////////////////////////////////////////////////////////////////////
	//	線分と平面の角度を求める
	//
	static MREAL GetAngleLnPln3(					// (  O) 左回転角度　（度）
													//		2点間の距離がg_gTol.D未満の場合は0度を返す
					const	MgLine3&	Ln1,		// (I  ) 線分1
					const	MgPlane3&	Pln2		// (I  ) 平面2
			);

	/////////////////////////////////////////////////////////////////////////////
	//		直線　形状演算
	//
	// ---------------------( ２次元 )------------------------------
	//
	//	２直線間の角の2等分線を求める
	//
	static void Bisector2ULn2(						// (  O) ステータス
					const	MgULine2&	ULn1,		// (I  ) 直線1
					const	MgULine2&	ULn2,		// (I  ) 直線2
							MgULine2*	ULn3		// (  O) 直線3
			);

	/////////////////////////////////////////////////////////////////////////////
	//	直線と線分間の角の2等分線を求める
	//
	static void BisectorULnLn2(						// (  O) ステータス
					const	MgULine2&	ULn1,		// (I  ) 直線1
					const	MgLine2&	Ln2,		// (I  ) 線分2
							MgULine2*	ULn3		// (  O) 直線3
			);

	/////////////////////////////////////////////////////////////////////////////
	//	線分と直線間の角の2等分線を求める
	//
	static void BisectorLnULn2(						// (  O) ステータス
					const	MgLine2&	Ln1,		// (I  ) 線分1
					const	MgULine2&	ULn2,		// (I  ) 直線2
							MgULine2*	ULn3		// (  O) 直線3
			);

	/////////////////////////////////////////////////////////////////////////////
	//	線分と線分間の角の2等分線を求める
	//
	static void Bisector2Ln2(						// (  O) ステータス
					const	MgLine2&	Ln1,		// (I  ) 線分1
					const	MgLine2&	Ln2,		// (I  ) 線分2
							MgULine2*	ULn3		// (  O) 直線3
			);

	/////////////////////////////////////////////////////////////////////////////
	//
	// ---------------------( ３次元 )------------------------------
	//
	//	２直線間の角の2等分線を求める
	//
	static void Bisector2ULn3(						// (  O) ステータス
					const	MgULine3&	ULn1,		// (I  ) 直線1
					const	MgULine3&	ULn2,		// (I  ) 直線2
							MgULine3*	ULn3		// (  O) 直線3
			);

	/////////////////////////////////////////////////////////////////////////////
	//	直線と線分間の角の2等分線を求める
	//
	static void BisectorULnLn3(						// (  O) ステータス
					const	MgULine3&	ULn1,		// (I  ) 直線1
					const	MgLine3&	Ln2,		// (I  ) 線分2
							MgULine3*	ULn3		// (  O) 直線3
			);

	/////////////////////////////////////////////////////////////////////////////
	//	線分と直線間の角の2等分線を求める
	//
	static void BisectorLnULn3(						//
					const	MgLine3&	Ln1,		// (I  ) 線分1
					const	MgULine3&	ULn2,		// (I  ) 直線2
							MgULine3*	ULn3		// (  O) 直線3
			);

	/////////////////////////////////////////////////////////////////////////////
	//	線分と線分間の角の2等分線を求める
	//
	static void Bisector2Ln3(						//
					const	MgLine3&	Ln1,		// (I  ) 線分1
					const	MgLine3&	Ln2,		// (I  ) 線分2
							MgULine3*	ULn3		// (  O) 直線3
			);


	//======================( ２次元 )==============================
	//		原点を中心に回転の座標変換マトリックスを作成
	//
	//S static MgMat2 Mat2RotV( const MgVect2& v);
	static MgMat2 Mat2Rot( MREAL dg)
	{
		MREAL	rd = MGRADIAN( dg);
		return Mat2RotV( MgVect2( MREAL( cos( rd)), MREAL( sin( rd))));
	}

	//======================( ２次元 )==============================
	//		原点を中心に逆回転の座標変換マトリックスを作成
	//
	//S static MgMat2 Mat2RotRevV( const MgVect2& v);
	static MgMat2 Mat2RotRev( MREAL dg)
	{
		MREAL	rd = MGRADIAN( dg);
		return Mat2RotRevV( MgVect2( MREAL( cos( rd)), MREAL( sin( rd))));
	}

	//======================( ３次元 )==============================
	//		ＸＹ平面で原点を中心に回転の座標変換マトリックスを作成
	//
	//S static MgMat3 Mat3RotZV( const MgVect2& v);
	static MgMat3 Mat3RotZ( MREAL dg)
	{
		MREAL	rd = MGRADIAN( dg);
		return Mat3RotZV( MgVect2( MREAL( cos( rd)), MREAL( sin( rd))));
	}

	//======================( ３次元 )==============================
	//		ＸＹ平面で原点を中心に逆回転の座標変換マトリックスを作成
	//
	//S static MgMat3 Mat3RotRevZV( const MgVect2& v);
	static MgMat3 Mat3RotRevZ( MREAL dg)
	{
		MREAL	rd = MGRADIAN( dg);
		return Mat3RotRevZV( MgVect2( MREAL( cos( rd)), MREAL( sin( rd))));
	}

	//======================( ３次元 )==============================
	//		ＹＺ平面で原点を中心に回転の座標変換マトリックスを作成
	//
	//S static MgMat3 Mat3RotTXV( const MgVect2& v);
	static MgMat3 Mat3RotTX( MREAL dg)
	{
		MREAL	rd = MGRADIAN( dg);
		return  Mat3RotTXV( MgVect2( MREAL( cos( rd)), MREAL( sin( rd))));
	}

	//======================( ３次元 )==============================
	//		ＹＺ平面で原点を中心に逆回転の座標変換マトリックスを作成
	//
	//S static MgMat3 Mat3RotRevXV( const MgVect2& v);
	static MgMat3 Mat3RotRevX( MREAL dg)
	{
		MREAL	rd = MGRADIAN( dg);
		return Mat3RotRevXV( MgVect2( MREAL( cos( rd)), MREAL( sin( rd))));
	}

	//======================( ３次元 )==============================
	//		ＺＸ平面で原点を中心に回転の座標変換マトリックスを作成
	//
	//S static MgMat3 Mat3RotYV( const MgVect2& v);
	static MgMat3 Mat3RotY( MREAL dg)
	{
		MREAL	rd = MGRADIAN( dg);
		return Mat3RotYV( MgVect2( MREAL( cos( rd)), MREAL( sin( rd))));
	}

	//======================( ３次元 )==============================
	//		ＺＸ平面で原点を中心に回転の座標変換マトリックスを作成
	//
	//S static MgMat3 Mat3RotRevYV( const MgVect2& v);
	static MgMat3 Mat3RotRevY( MREAL dg)
	{
		MREAL	rd = MGRADIAN( dg);
		return Mat3RotRevYV( MgVect2( MREAL( cos( rd)), MREAL( sin( rd))));
	}

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
	static MgMat2 MatPlus2( const MgMat2& i_tm, const MgMat2& i_tn);

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
	static MgMat2 MatPlusEqual2( MgMat2& io_tm, const MgMat2& i_tn);

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
	static MgMat2 MatSingleMinus2( const MgMat2& m1);

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
	static MgMat2 MatMinusSingle2( const MgMat2& i_tm);

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
	static MgMat2 MatMinus2( const MgMat2& i_tm, const MgMat2& i_tn);

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
	static MgMat2 MatMinusEqual2( MgMat2& io_tm, const MgMat2& i_tn);

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
	static MgMat2 MatMult2( const MgMat2& i_tm, const MgMat2& i_tn);

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
	static MgMat2 MatMultEqual2( MgMat2& io_tm, const MgMat2& i_tn);

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
	static MgPoint2 MatMultPt22( const MgPoint2& i_pt, const MgMat2& i_tm);

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
	static MgPoint2 MatMultEqualPt22( MgPoint2& io_pt, const MgMat2& i_tm);

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
	static MgULine2 MatMultULn22( const MgULine2& i_uln, const MgMat2& i_tm);

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
	static MgULine2 MatMultEqualULn22( MgULine2& io_uln, const MgMat2& i_tm);

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
	static MgPoint3 MatMultPt32( const MgPoint3& i_Pt, const MgMat2& i_tm);

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
	
	
	static MgPoint3 MatMultEqualPt32( MgPoint3& io_Pt, const MgMat2& i_tm);

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
	static MgULine3 MultULn32( const MgULine3& ULn1, const MgMat2& m2);

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
	//						  | m31 m32 0 m33 |
	//
	static MgULine3 MatMultULn32( const MgULine3& i_ULn, const MgMat2& i_tm);

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
	static MgULine3 MatMultEqualULn32( MgULine3& io_ULn, const MgMat2& i_tm);

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
	static MgMat3 MatPlus3(const MgMat3 &i_Tm, const MgMat3 &i_Tn);

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
	static MgMat3 MatPlusEqual3( MgMat3 &io_Tm, const MgMat3 &i_Tn);

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
	static MgMat3 MatSingleMinus3( const MgMat3& i_Tm);

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
	static MgMat3 MatMinus3( const MgMat3& i_Tm, const MgMat3& i_Tn);

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
	static MgMat3 MatMinusEqual3( MgMat3& io_Tm, const MgMat3& i_Tn);

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
	static MgMat3 MatMult3( const MgMat3& i_Tm, const MgMat3& i_Tn);

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
	static MgMat3 MatMultEqual3( MgMat3& io_Tm, const MgMat3& i_Tn);

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
	static MgPoint2 MatMultPt23( const MgPoint2& i_Pt, const MgMat3& i_Tm);

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
	static MgPoint2 MatMultEqualPt23( MgPoint2& io_Pt, const MgMat3& i_Tm);

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
	static MgPoint3 MatMultPt33( const MgPoint3& i_Pt, const MgMat3& i_Tm);

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
	static MgPoint3 MatMultEqualPt33( MgPoint3& io_Pt, const MgMat3& i_Tm);

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
	static MgULine3 MatMultULn33( const MgULine3& i_ULn, const MgMat3& i_Tm);

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
	static MgULine3 MatMultEqualULn33( MgULine3& io_ULn, const MgMat3& i_Tm);

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
	static MgMat2 Mat2RotV(const MgVect2& v);

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
	static MgMat2 Mat2RotRevV(const MgVect2& v);

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
	static MgMat3 Mat3RotZV(const MgVect2& v);

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
	static MgMat3 Mat3RotRevZV(const MgVect2& v);

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
	static MgMat3 Mat3RotTXV(const MgVect2& v);

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
	static MgMat3 Mat3RotRevXV(const MgVect2& v);

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
	static MgMat3 Mat3RotYV(const MgVect2& v);

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
	static MgMat3 Mat3RotRevYV(const MgVect2& v);

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
	static MgMat2 Mat2Scl(const MgVect2& vs);

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
	static MgMat3 Mat3Scl(const MgVect3& vs);

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
	static MgMat2 Mat2Mov(const MgVect2& v);

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
	static MgMat3 Mat3Mov(const MgVect3& v);

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
	static MgMat2 Mat2Inv( const MgMat2& i_tm);

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
	static MgMat3 Mat3Inv( const MgMat3& i_Tm);

	//======================( ２次元 )==============================
	//		直線に対する鏡像座標変換マトリックスを作成
	//
	static MgMat2 Mat2Mirror(						// (  O) 鏡像変換行列
					const	MgMat2&	i_tm, 			// (I  ) 変換行列
					const	MgULine2& i_uln			// (I  ) 鏡映の軸とする直線
			);

	//======================( ３次元 )==============================
	//		平面に対する鏡像座標変換マトリックスを作成
	//
	static MgMat3 Mat3Mirror(						// (  O) 鏡像変換行列
					const	MgMat3&	  i_Tm,	 		// (I  ) 変換行列
					const	MgPlane3& i_Pln			// (I  ) 鏡映の面とする平面
			);

	//======================( ２次元 )==============================
	//		中心点と回転角より座標変換マトリックスを作成
	//
	static MgMat2 Mat2Rot(							// (  O) 変換行列
					const	MgPoint2&	i_ptc, 		// (I  ) 回転の中心となる点
					const	MREAL&		i_ar		// (I  ) 回転角 Degree
			);

	//======================( ３次元 )==============================
	//		回転軸となる直線と回転角より座標変換マトリックスを作成
	//
	static MgMat3 Mat3Rot(							// (  O) 変換行列
					const	MgULine3&	i_ULn, 		// (I  ) 回転軸となる直線
					const	MREAL		&i_ar		// (I  ) 回転角(Degree)
			);

	//======================( ３次元 )==============================
	//		正投象座標変換マトリックスを計算する
	//
	static MgMat3 Mat3NProj(						// (  O) 正投象変換行列
					const	MgMat3		&i_Tm,		// (I  ) 入力座標変換行列
					const	MgPlane3	&i_Pln		// (I  ) 投象面
			);

	//======================( ３次元 )==============================
	//		斜投象座標変換マトリックスを計算する
	//
	static MgMat3 Mat3SProj(						// (  O) 斜投象変座標変換行列
					const	MgMat3		&i_Tm,		// (I  ) 入力座標変換行列
					const	MgPlane3	&i_Pln,		// (I  ) 投象面
					const	MgVect3&		i_V		// (I  ) 投象方向を示す単位ベクトル
			);

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
	
	static void Mat3PlntoXY(
					const	MgPlane3	&i_Pln,		// (I  ) 変換対象平面
					const	MgPoint3&	i_PtOrg,	// (I  ) 原点座標			
							MgMat3*		o_MatXY,	// (  O) Pln to XY平面 ３Ｄマトリックス
							MgMat3*		o_MatPln	// (  O) XY平面 to Pln ３Ｄマトリックス
					);
	
	//===================(３次元 → ２次元)=========================
	//		３Ｄ座標を２Ｄ座標へ変換する
	//
	static void Point3Dto2D(
					const	MgPoint3&	i_P,		// (I  ) ３Ｄ座標
					const	MgMat3		&i_Tm,		// (I  ) 座標変換マトリックス
							MgPoint2*	o_P			// (  O) ２Ｄ座標
					);

	//===================(３次元 → ２次元)=========================
	//		３Ｄ線分を２Ｄ線分へ座標変換する
	//
	static inline void Line3Dto2D(
					const MgLine3		&Lni,		// (I  ) ３Ｄ線分
					const MgMat3		&Mat,		// (I  ) 座標変換マトリックス
						  MgLine2		*lno		// (  O) ２Ｄ線分
					)
	{
		Point3Dto2D( Lni.p[0], Mat, &lno->p[0]); 
		Point3Dto2D( Lni.p[1], Mat, &lno->p[1]);
	}  

	//===================(３次元 → ２次元)=========================
	//		３Ｄ直線を２Ｄ直線へ座標変換する
	//
	static void ULine3Dto2D(
					const	MgULine3	&i_ULn,		// (I  ) ３Ｄ直線
					const	MgMat3		&i_Mt,		// (I  ) 座標変換マトリックス
						 	MgULine2	*o_uln		// (  O) ２Ｄ直線
					);

	//===================(３次元 → ２次元)=========================
	//		３Ｄ座標を２Ｄ座標へ変換する
	//
	static MgPoint2 Point3Dto2D2(						// (  O) ２Ｄ座標
					const	MgPoint3&	i_P,		// (I  ) ３Ｄ座標
					const	MgMat3		&i_Tm		// (I  ) 座標変換マトリックス
					);

	//===================(３次元 → ２次元)=========================
	//		３Ｄ線分を２Ｄ線分へ座標変換する
	//
	static inline MgLine2 Line3Dto2D2(				// (  O) ２Ｄ線分
					const MgLine3		&Lni,		// (I  ) ３Ｄ線分
					const MgMat3		&Mat		// (I  ) 座標変換マトリックス
					)
	{
		return MgLine2( 
					  Point3Dto2D2( Lni.p[0], Mat), 
					  Point3Dto2D2( Lni.p[1], Mat));
	}  

	//===================(３次元 → ２次元)=========================
	//		３Ｄ直線を２Ｄ直線へ座標変換する
	//
	static MgULine2 ULine3Dto2D2(						// (  O) ２Ｄ直線
					const	MgULine3	&i_ULn,		// (I  ) ３Ｄ直線
					const	MgMat3		&i_Tm		// (I  ) 座標変換マトリックス
					);

	//===================(３次元 → ２次元)=========================
	//		３Ｄ座標群を２Ｄ座標群へ変換する
	//
	static void	GPoint3Dto2D(
					const	MgGPoint3	&i_GP,		// (I  ) ３Ｄ座標群
					const	MgMat3		&i_Tm,		// (I  ) 座標変換マトリックス
							MgGPoint2	*o_GP		// (  O) ２Ｄ座標群
					);
	
	//===================(３次元 → ２次元)=========================
	//		３Ｄ座標群（ポリライン）を２Ｄ座標群へ変換する
	//
	static void	Polyg3Dto2D(
					const	MgPolyg3&	i_Pg,		// (I  ) ３Ｄ座標群
					const	MgMat3		&iTm,		// (I  ) 座標変換マトリックス
							MgPolyg2*	o_Pg		// (  O) ２Ｄ座標群
					);
	
	//===================(３次元 → ２次元)=========================
	//		３Ｄ線分群を２Ｄ線分群へ変換する
	//
	static void	GLine3Dto2D(
					const	MgGLine3	&i_GLn,		// (I  ) ３Ｄ線分群
					const	MgMat3		&i_Tm,		// (I  ) 座標変換マトリックス
							MgGLine2*	o_gln		// (  O) ２Ｄ線分群
					);
	
	//===================(３次元 → ２次元)=========================
	//		３Ｄ多角形群（ポリライン群）を２Ｄ多角形群へ変換する
	//
	static void GPolyg3Dto2D(
					const	MgGPolyg3	&i_GPg,		// (I  ) ３Ｄ多角形群
					const	MgMat3		&i_Tm,		// (I  ) 座標変換マトリックス
						 	MgGPolyg2	*o_gpg		// (  O) ２Ｄ多角形群
					);

	//===================(２次元 → ３次元)=========================
	//		２Ｄ座標を３Ｄ座標へ変換する
	//
	static void	Point2Dto3D(
					const	MgPoint2&	i_pt,		// (I  ) ２Ｄ座標
					const	MgMat3		&i_Tm,		// (I  ) 座標変換マトリックス
							MgPoint3	*o_Pt		// (  O) ３Ｄ座標
					);

	//===================(２次元 → ３次元)=========================
	//		２Ｄ線分を３Ｄ線分へ座標変換する
	//
	static inline void Line2Dto3D(
					const MgLine2&		lni,		// (I  ) ２Ｄ線分
					const MgMat3		&Mat,		// (I  ) 座標変換マトリックス
						  MgLine3		*Lno		// (  O) ３Ｄ線分
					)
	{
		Point2Dto3D( lni.p[0], Mat, &Lno->p[0]); 
		Point2Dto3D( lni.p[1], Mat, &Lno->p[1]);
	}  


	//===================(２次元 → ３次元)=========================
	//		２Ｄ直線を３Ｄ直線へ座標変換する
	//
	static void ULine2Dto3D(
					const	MgULine2	&i_uln,		// (I  ) ２Ｄ直線
					const	MgMat3		&i_Tm,		// (I  ) 座標変換マトリックス
						 	MgULine3	*o_ULn		// (  O) ３Ｄ直線
					);

	//===================(２次元 → ３次元)=========================
	//		２Ｄ座標を３Ｄ座標へ変換する
	//
	static MgPoint3 Point2Dto3D3(					// (  O) ３Ｄ座標
					const	MgPoint2&	i_Pt,		// (I  ) ２Ｄ座標
					const	MgMat3		&i_Tm		// (I  ) 座標変換マトリックス
					);

	//===================(２次元 → ３次元)=========================
	//		２Ｄ線分を３Ｄ線分へ座標変換する
	//
	static MgLine3 Line2Dto3D3(						// (  O) ３Ｄ線分
					const MgLine2&		lni,		// (I  ) ２Ｄ線分
					const MgMat3		&Mat		// (I  ) 座標変換マトリックス
					)
	{
		return MgLine3(
					  Point2Dto3D3( lni.p[0], Mat), 
					  Point2Dto3D3( lni.p[1], Mat));
	}  

	//===================(２次元 → ３次元)=========================
	//		２Ｄ直線を３Ｄ直線へ座標変換する
	//
	static MgULine3 ULine2Dto3D3(					// (  O) ３Ｄ直線
					const	MgULine2	&i_uln,		// (I  ) ２Ｄ直線
					const	MgMat3		&i_Tm		// (I  ) 座標変換マトリックス
					);

	//===================(２次元 → ３次元)=========================
	//		２Ｄ座標群を３Ｄ座標群へ変換する
	//
	static void	GPoint2Dto3D(
					const	MgGPoint2	&i_GPt,		// (I  ) ２Ｄ座標群
					const	MgMat3		&i_Tm,		// (I  ) 座標変換マトリックス
							MgGPoint3	*o_GPt		// (  O) ３Ｄ座標群
					);
	
	//===================(２次元 → ３次元)=========================
	//		２Ｄ座標群（ポリライン）を３Ｄ座標群へ変換する
	//
	static void	Polyg2Dto3D(
					const	MgPolyg2&	i_Pg,		// (I  ) ２Ｄ座標群
					const	MgMat3		&i_Tm,		// (I  ) 座標変換マトリックス
							MgPolyg3*	o_Pg		// (  O) ３Ｄ座標群
					);
	
	//===================(２次元 → ３次元)=========================
	//		２Ｄ直線群を３Ｄ直線群へ変換する
	//
	static void	GLine2Dto3D(
					const	MgGLine2&	i_gln,		// (I  ) ２Ｄ直線群
					const	MgMat3		&i_Tm,		// (I  ) 座標変換マトリックス
							MgGLine3	*o_GLn		// (  O) ３Ｄ直線群
					);

	//===================(２次元 → ３次元)=========================
	//		２Ｄ多角形群（ポリライン群）を３Ｄ多角形群へ変換する
	//
	static void GPolyg2Dto3D(
					const	MgGPolyg2	&i_GPg,		// (I  ) ２Ｄ多角形群
					const	MgMat3		&i_Tm,		// (I  ) 座標変換マトリックス
						 	MgGPolyg3	*o_GPg		// (  O) ３Ｄ多角形群
					);

	// ---------------------( ２次元 )------------------------------
	//		指定線分より始まり線分群の線分を結合して最小の多角形を作成する
	//
	static bool	MakeGPg2FromLn2InGLn2(			// (  O) ステイタス　true: 正常作成　false: 作成不可
					const	MgLine2&	Ln1,		// (I  ) 指定線分
							MgGLine2*	GLn2,		// (I  ) 結合する線分群
							MINT*		nHk,		// (I O) 結合する線分の中の正しい方向を持つ線分数（先頭より幾つかを表す）
							MgPolyg2*	Pg3			// (  O) 領域
					);
	
	// ---------------------( ２次元 )------------------------------
	//		多角形を直線で切り、直線の指定側の領域を得る			<多角形が穴の場合の考慮不足?>
	//
	static MINT DividePgULn2(						// (  O) ステイタス
													//			MC_NINT			(0): 交差なし
													//			MC_INT			(1): 交差あり
							MINT		Sel,		// (I  ) 選択条件
													//			MC_LEFT			(010): 左側の領域
													//			MC_RIGHT		(040): 右側の領域
					const	MgPolyg2&	Pg1,		// (I  ) 多角形
					const	MgULine2&	ULn2,		// (I  ) 直線
							MgGPolyg2*	GPg3		// (  O) 穴付き多角形群
					);
	
	// ---------------------( ２次元 )------------------------------
	//		穴付き多角形を直線で切り、直線の指定側の領域を得る
	//
	static MINT DivideGPgULn2(						// (  O) ステイタス
													//			MC_NINT			(0): 交差なし
													//			MC_INT			(1): 交差あり
							MINT		Sel,		// (I  ) 選択条件
													//			MC_LEFT			(010): 左側の領域
													//			MC_RIGHT		(040): 右側の領域
					const	MgGPolyg2&	GPg1,		// (I  ) 穴付き多角形
					const	MgULine2&	ULn2,		// (I  ) 直線
							MgGPolyg2*	GPg3		// (  O) 穴付き多角形群
					);
	
	// ---------------------( ２次元 )------------------------------
	//		多角形1を多角形2で分割し、多角形2の指定側（内側または外側）の領域を得る
	//
	static MINT Divide2Pg2(							// (  O) ステイタス
													//			MC_NINT			(0): 交差なし
													//			MC_INT			(1): 交差あり
													//			MC_MATCH		(4): 選択条件に合う部分あり
													//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
													//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
													//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
							MINT		Sel,		// (I  ) 選択条件
													//			MC_IN_BORDER	(001): 内側
													//			MC_OUT_BORDER	(004): 外側
													//									
					const	MgPolyg2&	Pg1,		// (I  ) 多角形1
					const	MgPolyg2&	Pg2,		// (I  ) 多角形2
							MgGPolyg2*	pGPgo		// (  O) 穴付き多角形3
					);
	
	// ---------------------( ２次元 )------------------------------
	//		穴付き多角形を穴付き多角形で分割し、選択条件に合う領域を得る
	//
	static MINT Divide2GPg2(						// (  O) ステイタス
													//			MC_NINT			(0): 交差なし
													//			MC_INT			(1): 交差あり
													//			MC_MATCH		(4): 選択条件に合う部分あり
													//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
													//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
													//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
							MINT		Sel,		// (I  ) 選択条件
													//			MC_IN_BORDER	(001): 内側
													//			MC_OUT_BORDER	(004): 外側
					const	MgGPolyg2&	GPg1,		// (I  ) 穴付き多角形1
					const	MgGPolyg2&	GPg2,		// (I  ) 穴付き多角形2
							MgGPolyg2*	pGPgo		// (  O) 穴付き多角形3
					);
	
	// ---------------------( ３次元 )------------------------------
	//		多角形1を多角形2で分割し、多角形2の指定側（内側または外側）の領域を得る
	//
	static MINT Divide2Pg3(							// (  O) ステイタス
													//			MC_NINT			(0): 交差なし
													//			MC_INT			(1): 交差あり
													//			MC_MATCH		(4): 選択条件に合う部分あり
													//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
													//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
													//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
							MINT		Sel,		// (I  ) 選択条件
													//			MC_IN_BORDER	(001): 内側
													//			MC_OUT_BORDER	(004): 外側
													//									
					const	MgPolyg3&	Pg1,		// (I  ) 多角形1
					const	MgPolyg3&	Pg2,		// (I  ) 多角形2
							MgGPolyg3*	pGPgo		// (  O) 穴付き多角形3
					);
	
	// ---------------------( ３次元 )------------------------------
	//		穴付き多角形を穴付き多角形で分割し、選択条件に合う領域を得る
	//
	static MINT Divide2GPg3(						// (  O) ステイタス
													//			MC_NINT			(0): 交差なし
													//			MC_INT			(1): 交差あり
													//			MC_MATCH		(4): 選択条件に合う部分あり
													//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
													//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
													//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
							MINT		Sel,		// (I  ) 選択条件
													//			MC_IN_BORDER	(001): 内側
													//			MC_OUT_BORDER	(004): 外側
					const	MgGPolyg3&	GPg1,		// (I  ) 穴付き多角形1
					const	MgGPolyg3&	GPg2,		// (I  ) 穴付き多角形2
							MgGPolyg3*	pGPgo		// (  O) 穴付き多角形3
					);
	
	// ---------------------( ２次元 )------------------------------
	//		多角形と多角形の和、差、積の穴付き多角形を求める
	//
	static MINT Merge2PgtoGPg2(						// (  O) ステイタス
													//			MC_NINT			(0): 交差なし
													//			MC_INT			(1): 交差あり
													//			MC_MATCH		(4): 選択条件に合う部分あり
													//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
													//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
													//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
							MINT		Sel,		// (I  ) 選択条件
													//			MC_OR			(1): 和
													//			MC_DIFF			(2): 差
													//			MC_AND			(4): 積
					const	MgPolyg2&	Pg1,		// (I  ) 多角形1
					const	MgPolyg2&	Pg2,		// (I  ) 多角形2
							MgGPolyg2*	pGPgo		// (  O) 穴付き多角形3
					);
	
	// ---------------------( ２次元 )------------------------------
	//		多角形と穴付き多角形の和、差、積の穴付き多角形を求める
	//
	static MINT MergePgGPgtoGPg2(					// (  O) ステイタス
													//			MC_NINT			(0): 交差なし
													//			MC_INT			(1): 交差あり
													//			MC_MATCH		(4): 選択条件に合う部分あり
													//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
													//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
													//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
							MINT		Sel,		// (I  ) 選択条件
													//			MC_OR			(1): 和
													//			MC_DIFF			(2): 差
													//			MC_AND			(4): 積
					const	MgPolyg2&	Pg1,		// (I  ) 多角形1
					const	MgGPolyg2&	GPg2,		// (I  ) 穴付き多角形2
							MgGPolyg2*	pGPgo		// (  O) 穴付き多角形3
					);
	
	// ---------------------( ２次元 )------------------------------
	//		穴付き多角形と多角形の和、差、積の穴付き多角形を求める
	//
	static MINT MergeGPgPgtoGPg2(					// (  O) ステイタス
													//			MC_NINT			(0): 交差なし
													//			MC_INT			(1): 交差あり
													//			MC_MATCH		(4): 選択条件に合う部分あり
													//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
													//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
													//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
							MINT		Sel,		// (I  ) 選択条件
													//			MC_OR			(1): 和
													//			MC_DIFF			(2): 差
													//			MC_AND			(4): 積
					const	MgGPolyg2&	GPg1,		// (I  ) 穴付き多角形1
					const	MgPolyg2&	Pg2,		// (I  ) 多角形2
							MgGPolyg2*	pGPgo		// (  O) 穴付き多角形3
					);
	
	// ---------------------( ２次元 )------------------------------
	//		穴付き多角形と穴付き多角形の和、差、積の穴付き多角形を求める
	//
	static MINT Merge2GPgtoGPg2(					// (  O) ステイタス
													//			MC_NINT			(x01): 交差なし
													//			MC_INT			(x02): 交差あり
													//			MC_MATCH		(x10): 選択条件に合う部分あり
													//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
													//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
													//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
							MINT		Sel,		// (I  ) 選択条件
													//			MC_OR			(1): 和
													//			MC_DIFF			(2): 差
													//			MC_AND			(4): 積
					const	MgGPolyg2&	GPg1,		// (I  ) 穴付き多角形1
					const	MgGPolyg2&	GPg2,		// (I  ) 穴付き多角形2
							MgGPolyg2*	pGPgo		// (  O) 穴付き多角形3
					);
	
	// ---------------------( ３次元 )------------------------------
	//		多角形と多角形の和、差、積の穴付き多角形を求める
	//
	static MINT Merge2PgtoGPg3(						// (  O) ステイタス
													//			MC_NINT			(0): 交差なし
													//			MC_INT			(1): 交差あり
													//			MC_MATCH		(4): 選択条件に合う部分あり
													//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
													//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
													//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
							MINT		Sel,		// (I  ) 選択条件
													//			MC_OR			(1): 和
													//			MC_DIFF			(2): 差
													//			MC_AND			(4): 積
					const	MgPolyg3&	Pg1,		// (I  ) 多角形1
					const	MgPolyg3&	Pg2,		// (I  ) 多角形2
							MgGPolyg3*	pGPgo		// (  O) 穴付き多角形3
					);
	
	// ---------------------( ３次元 )------------------------------
	//		多角形と穴付き多角形の和、差、積の穴付き多角形を求める
	//
	static MINT MergePgGPgtoGPg3(					// (  O) ステイタス
													//			MC_NINT			(0): 交差なし
													//			MC_INT			(1): 交差あり
													//			MC_MATCH		(4): 選択条件に合う部分あり
													//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
													//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
													//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
							MINT		Sel,		// (I  ) 選択条件
													//			MC_OR			(1): 和
													//			MC_DIFF			(2): 差
													//			MC_AND			(4): 積
					const	MgPolyg3&	Pg1,		// (I  ) 多角形1
					const	MgGPolyg3&	GPg2,		// (I  ) 穴付き多角形2
							MgGPolyg3*	pGPgo		// (  O) 穴付き多角形3
					);
	
	// ---------------------( ３次元 )------------------------------
	//		穴付き多角形と多角形の和、差、積の穴付き多角形を求める
	//
	static MINT MergeGPgPgtoGPg3(					// (  O) ステイタス
													//			MC_NINT			(0): 交差なし
													//			MC_INT			(1): 交差あり
													//			MC_MATCH		(4): 選択条件に合う部分あり
													//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
													//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
													//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
							MINT		Sel,		// (I  ) 選択条件
													//			MC_OR			(1): 和
													//			MC_DIFF			(2): 差
													//			MC_AND			(4): 積
					const	MgGPolyg3&	GPg1,		// (I  ) 穴付き多角形1
					const	MgPolyg3&	Pg2,		// (I  ) 多角形2
							MgGPolyg3*	pGPgo		// (  O) 穴付き多角形3
					);
	
	// ---------------------( ３次元 )------------------------------
	//		穴付き多角形と穴付き多角形の和、差、積の穴付き多角形を求める
	//
	static MINT Merge2GPgtoGPg3(					// (  O) ステイタス
													//			MC_NINT			(0): 交差なし
													//			MC_INT			(1): 交差あり
													//			MC_MATCH		(4): 選択条件に合う部分あり
													//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
													//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
													//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
							MINT		Sel,		// (I  ) 選択条件
													//			MC_OR			(1): 和
													//			MC_DIFF			(2): 差
													//			MC_AND			(4): 積
					const	MgGPolyg3&	GPg1,		// (I  ) 穴付き多角形1
					const	MgGPolyg3&	GPg2,		// (I  ) 穴付き多角形2
							MgGPolyg3*	pGPgo		// (  O) 穴付き多角形3
					);
	
	// ---------------------( ２次元 )------------------------------
	//		多角形の辺（線分）を他の多角形で分割し、指定した側の線分を残す
	//								線分が多角形の辺上にある場合は、
	//				(1) 選択条件が辺上同一方向の場合は、同一方向の時残し
	//				(2) 選択条件が辺上逆方向の場合は、逆方向の時残す
	//				(3) その他の場合は捨てる
	//
	static MINT DivideAdd2Pg2(						// (  O) ステイタス
													//			MC_NINT			(0): 交差なし
													//			MC_INT			(1): 交差あり
													//			MC_MATCH		(4): 選択条件に合う部分あり
													//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
													//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
													//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
							MINT		Sel1,		// (I  ) 選択条件1　多角形1の線分を残す条件
													//			MC_IN_BORDER	(001):	多角形2の領域の内側
													//			MC_ON_BORDER	(002):  多角形2の辺上
													//			MC_OUT_BORDER	(004):	多角形2の領域の外側
													//			MC_SAME_DIR		(0100):	辺上同一方向
													//			MC_REV_DIR		(0200):	辺上逆方向
					const	bool		fRev,		// (I  ) 反転フラグ	
													//		 true: 線分の方向を反転する
					const	MgPolyg2&	Pg1,		// (I  ) 多角形1
					const	MgPolyg2&	Pg2,		// (I  ) 多角形2
							MgGLine2*	pGLno,		// (I O) 直線群
							MgGPolyg2*	pGPgo		// (I O) 穴付き多角形
					);
	
	// ---------------------( ２次元 )------------------------------
	//		多角形の辺（線分）を穴付き多角形で分割し、指定した側の線分を残す
	//								線分が多角形の辺上にある場合は、
	//				(1) 選択条件が辺上同一方向の場合は、同一方向の時残し
	//				(2) 選択条件が辺上逆方向の場合は、逆方向の時残す
	//				(3) その他の場合は捨てる
	//
	
	static MINT DivideAddPgGPg2(					// (  O) ステイタス
													//			MC_NINT			(0): 交差なし
													//			MC_INT			(1): 交差あり
													//			MC_MATCH		(4): 選択条件に合う部分あり
													//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
													//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
													//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
							MINT		Sel1,		// (I  ) 選択条件1　多角形1の線分を残す条件
													//			MC_IN_BORDER	(001): 多角形2の領域の内側
													//			MC_ON_BORDER	(002): 多角形2の辺上
													//			MC_OUT_BORDER	(004): 多角形2の領域の外側
													//			MC_SAME_DIR		(0100):	辺上同一方向
													//			MC_REV_DIR		(0200):	辺上逆方向
					const	bool		fRev,		// (I  ) 反転フラグ	
													//		 true: 線分の方向を反転する
					const	MgPolyg2&	Pg1,		// (I  ) 多角形1
					const	MgGPolyg2&	GPg2,		// (I  ) 穴付き多角形2
							MgGLine2*	pGLno,		// (I O) 直線群
							MgGPolyg2*	pGPgo		// (I O) 穴付き多角形
					);
	
	// ---------------------( ２次元 )------------------------------
	//		GLn1の方向を考慮して接続し領域群を作成する
	//
	
	#define	MX_AREA_BY_PT	20						// １点の周りの最大領域数
	
	static void MakeGPg2InGLn2(
					const	MgGLine2&	GLn1,		// (I  ) 正しい方向を持つ線分群
							MgGPolyg2*	GPg3		// (  O) 領域群
					);
	
	// ---------------------( ２次元 )------------------------------
	//		GLn1の方向を考慮して直線分割線分GLn2を追加し領域群を作成する
	//
	static void MakeGPg2In2GLn2(
					const	MgGLine2&	GLn1,		// (I  ) 正しい方向を持つ線分群
					const	MgGLine2&	GLn2,		// (I  ) 不正確な方向の線分群
							MgGPolyg2*	GPg3		// (  O) 領域群
					);
	
	// ---------------------( ２次元 )------------------------------
	//		指定線分より始まり線分群の線分を交点で分割後に結合して最小の多角形を作成する
	//
	static bool MakeGPg2FromLn2InGLn2(				// (  O) ステイタス　true: 正常作成　false: 作成不可
					const	MgLine2&	Ln1,		// (I  ) 指定線分
					const	MgGLine2&	GLn2,		// (I  ) 結合する線分群
							MgPolyg2*	Pg3			// (  O) 領域
					);
	
//	// ---------------------( ２次元 )------------------------------
//	//		指定線分より始まり線分群の線分を結合して最小の多角形を作成する
//	//
//	static bool	MgMakeGPg2FromLn2InGLn2(			// (  O) ステイタス　true: 正常作成　false: 作成不可
//					const	MgLine2&	Ln1,		// (I  ) 指定線分
//							MgGLine2*	GLn2,		// (I O) 結合する線分群
//							MINT*		nHk,		// (I O) 結合する線分の中の正しい方向を持つ線分数（先頭より幾つかを表す）
//							MgPolyg2*	Pg3			// (  O) 領域
//					);
	
	// ---------------------( ２次元 )------------------------------
	//		穴付き多角形を多角形に結合する
	//
	static void ConnectGPgtoPg2(					//
					const	MgGPolyg2&	GpgI,		// (I  ) 穴付き多角形
							MgPolyg2*	pgO			// (  O) 結合後の多角形
					);
	
	// ---------------------( ３次元 )------------------------------
	//		穴付き多角形を多角形に結合する
	//
	static void ConnectGPgtoPg3(					//
					const	MgGPolyg3&	GPgI,		// (I  ) 穴付き多角形
					const	MgVect3&	VuH,		// (I  ) 法線方向
							MgPolyg3*	PgO			// (  O) 結合後の多角形
					);

	
	//===========================================================================
	//		点　形状演算
	//
	// ---------------------( ２次元 )------------------------------
	//
	//		２点間の距離を求める。
	//
	static MREAL Dist2Pt2(							// (  O) 直線から点までの距離 >0:左側 <0:右側
					const	MgPoint2&	p1,			// (I  ) 点1
					const	MgPoint2&	p2			// (I  ) 点2
					);

	//===========================================================================
	//		点　形状演算
	//
	// ---------------------( ３次元 )------------------------------
	//
	//		２点間の距離を求める。
	//
	static MREAL Dist2Pt3(							// (  O) ２点間の距離 >0:左側 <0:右側
					const	MgPoint3&	p1,			// (I  ) 点1
					const	MgPoint3&	p2			// (I  ) 点2
					);
	
	//===========================================================================
	//		直線　形状演算
	//
	// ---------------------( ２次元 )------------------------------
	//
	//		直線から点までの正負方向付き距離を求める。
	//
	static MREAL DistPtULn2(						// (  O) 直線から点までの距離 >0:左側 <0:右側
					const	MgPoint2&	p1,			// (I  ) 点1
					const	MgULine2&	ULn2		// (I  ) 直線2
					);
	
	//	------------------------
	//		線分から点までの正負方向付き距離を求める。	(直線と同様)
	
	//
	static MREAL DistPtLn2(						// (  O) 線分から点までの距離 >0:左側 <0:右側
													//	線分の長さが0の場合は点から線分の始点までの距離
					const	MgPoint2&	p1,			// (I  ) 点1
					const	MgLine2&	Ln2			// (I  ) 線分2
					);
	
	// ---------------------( ３次元 )------------------------------
	//		直線から点までの距離を求める。
	//
	static MREAL DistPtULn3(						// (  O) 直線から点までの距離
					const	MgPoint3&	p1,			// (I  ) 点1
					const	MgULine3&	ULn2		// (I  ) 直線2
					);

	//
	//		線分から点までの距離を求める。(直線と同様)
	//
	static MREAL DistPtLn3(						// (  O) 線分から点までの距離
													//	線分の長さが0の場合は点から線分の始点までの距離
					const	MgPoint3&	p1,			// (I  ) 点1
					const	MgLine3	&Ln2			// (I  ) 線分2
					);
	
	// ---------------------( ３次元 )------------------------------
	//		直線から点までの正負方向付き距離を求める。
	//
	static MREAL DistPtULnV3(						// (  O) 直線から点までの距離 >0:左側 <0:右側
					const	MgPoint3&	p1,			// (I  ) 点1
					const	MgULine3&	ULn2,		// (I  ) 直線2
					const	MgVect3&	vn			// (I  ) 点と直線に直交する単位ベクトル
					);

	//
	//		線分から点までの正負方向付き距離を求める。(直線と同様)
	//
	static MREAL DistPtLnV3(						// (  O) 線分から点までの距離 >0:左側 <0:右側
													//		 線分の長さが0の場合は点から線分の始点までの距離
													//		 線分の長さが0の場合は点から線分の始点までの距離
				const	MgPoint3&	p1,				// (I  ) 点1
				const	MgLine3&	Ln2,			// (I  ) 線分2
				const	MgVect3&	vn				// (I  ) 点と直線に直交する単位ベクトル
				);
	
	// ---------------------( ３次元 )------------------------------
	//		平面から点までの正負方向付き距離を求める。
	//
	static MREAL DistPtPln3(						// (  O) 点から平面までの距離 >0:表側（平面の単位ベクトル方向側）<0:裏側
					const	MgPoint3&	p1,			// (I  ) 点1
					const	MgPlane3&	Pln2		// (I  ) 平面2
					);

	// ---------------------( ２次元 )------------------------------
	//		線分を線分で分割する
	//		(線分１の端部での交差は交差無しとし分割無し)
	//
	static MINT Divide2Ln2(							// (  O) ステイタス
													//			MC_NINT			(0): 分割無し
													//			MC_INT			(1): 分割あり
					const	MgLine2&	Ln1, 		// (I  ) 分割対象線分
					const	MgLine2&	Ln2, 		// (I  ) 分割線分
							MgGLine2*	GLn3		// (  O) 分割後の線分群
					);

	// ---------------------( ２次元 )------------------------------
	//		線分を直線で分割する
	//		(線分１の端部での交差は交差無しとし分割無し)
	//
	static MINT DivideLnULn2(						// (  O) ステイタス
													//			MC_NINT			(0): 交差なし
													//			MC_INT			(1): 交差あり
					const	MgLine2&	Ln1, 		// (I  ) 分割対象線分
					const	MgULine2&	ULn2, 		// (I  ) 分割直線
							MgGLine2*	GLn3		// (  O) 分割後の線分群
					);

	// ---------------------( ２次元 )------------------------------
	//		線分を直線で分割し直線の選択側の線分を分割後の線分群に追加する
	//		(線分の端部での交差は交差なしとする)
	//			選択条件に「辺上同一方向または辺上逆方向」が設定されており、分割後の線分が穴付き多角形の辺上にある場合には、
	//			次のように選択する
	//				(1) 選択条件が辺上同一方向の場合は、同一方向の時残し
	//				(2) 選択条件が辺上逆方向の場合は、逆方向の時残す
	//				(3) その他の場合は捨てる
	//		//			(1) 選択条件が直線の左側|直線上の場合は、同一方向の時残し
	//		//			(2) 選択条件が直線の右側|直線上の場合は、逆方向の時残す
	//		//			(3) その他の場合は捨てる
	//
	// 		sels = MC_LEFT | MC_ON_BORDER | MC_SAME_DIR;
	
	//
	static MINT DivideAddLnULn2(					// (  O) ステイタス
													//			MC_NINT			(0): 交差なし
													//			MC_INT			(1): 交差あり
													//			MC_MATCH		(4): 選択条件に合う部分あり
													//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
													//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
													//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
							MINT		Sel,		// (I  ) 選択条件
													//			MC_LEFT			(010): 直線の左側
													//			MC_ON_LINE		(020): 直線上
													//			MC_RIGHT		(040): 直線の右側
													//			MC_SAME_DIR		(0100):	辺上同一方向
													//			MC_REV_DIR		(0200):	辺上逆方向
					const	MgLine2&	Ln1, 		// (I  ) 分割対象線分
					const	MgULine2&	ULn2, 		// (I  ) 分割直線
							MgGLine2*	GLn3		// (I O) 分割後の線分群
					);

	// ---------------------( ２次元 )------------------------------
	//		線分を線分群との交点で分割する
	//		(線分の端部での交差は交差なしとする)
	//
	static MINT DivideLnGLn2(						// (  O) ステイタス
													//			MC_NINT			(0): 交差なし
													//			MC_INT			(1): 交差あり
					const	MgLine2&	Ln1, 		// (I  ) 分割対象線分
					const	MgGLine2&	GLn2, 		// (I  ) 分割線分群
							MgGLine2*	GLn3		// (  O) 分割後の線分群
					);

	// ---------------------( ２次元 )------------------------------
	//		線分群を交点で分割し同一線分は除き一本にする（逆方向も除く）
	//		交差判定で２線分が重なる場合は端点で分割する
	//
	static MINT DivideGLn2(							// (  O) ステイタス
													//			MC_NINT			(0): 交差なし
													//			MC_INT			(1): 交差あり
					const	MgGLine2&	GLn1, 		// (I  ) 線分群
							MgGLine2*	GLn2		// (  O) 分割後の線分群
					);

	// ---------------------( ２次元 )------------------------------
	//		直線を多角形で分割し、選択条件に合う線分群を取り出す
	//			選択条件に「辺上同一方向」が設定されており、分割後の線分が穴付き多角形の辺上にある場合には、
	//			次のように選択する
	//				(1) 選択条件が辺上同一方向の場合は、同一方向の時残し
	//				(2) その他の場合は捨てる
	//
	static MINT DivideULnPg2(						// (  O) ステイタス
													//			MC_NINT			(0): 交差なし
													//			MC_INT			(1): 交差あり
													//			MC_MATCH		(4): 選択条件に合う部分あり
													//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
													//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
													//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
							MINT		Sel,		// (I  ) 選択条件
													//			MC_IN_BORDER	(001): 内側
													//			MC_ON_BORDER	(002): 辺上
													//			MC_SAME_DIR		(0100):	辺上同一方向
					const	MgULine2&	ULn1,		// (I  ) 直線
					const	MgPolyg2&	Pg2,		// (I  ) 多角形
							MgGLine2*	GLn3		// (  O) 分割後の線分群
					);
	
	// ---------------------( ２次元 )------------------------------
	//		直線を多角形で分割し、選択条件に合う線分を分割後の線分群に追加する
	//			選択条件に「辺上同一方向」が設定されており、分割後の線分が穴付き多角形の辺上にある場合には、
	//			次のように選択する
	//				(1) 選択条件が辺上同一方向の場合は、同一方向の時残し
	//				(2) その他の場合は捨てる
	//
	static MINT DivideAddULnPg2(					// (  O) ステイタス
													//			MC_NINT			(0): 交差なし
													//			MC_INT			(1): 交差あり
													//			MC_MATCH		(4): 選択条件に合う部分あり
													//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
													//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
													//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
							MINT		Sel,		// (I  ) 選択条件
													//			MC_IN_BORDER	(001): 内側
													//			MC_ON_BORDER	(002): 辺上
													//			MC_SAME_DIR		(0100):	辺上同一方向
					const	MgULine2&	ULn1,		// (I  ) 直線
					const	MgPolyg2&	Pg2,		// (I  ) 多角形
							MgGLine2*	GLn3		// (I O) 分割後の線分群
					);

	// ---------------------( ２次元 )------------------------------
	//		線分を多角形で分割し、選択条件に合う線分群を得る
	//			選択条件に「辺上同一方向または辺上逆方向」が設定されており、分割後の線分が穴付き多角形の辺上にある場合には、
	//			次のように選択する
	//				(1) 選択条件が辺上同一方向の場合は、同一方向の時残し
	//				(2) 選択条件が辺上逆方向の場合は、逆方向の時残す
	//				(3) その他の場合は捨てる
	//
	static MINT DivideLnPg2(						// (  O) ステイタス
													//			MC_NINT			(0): 交差なし
													//			MC_INT			(1): 交差あり
													//			MC_MATCH		(4): 選択条件に合う部分あり
													//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
													//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
													//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
							MINT		Sel,		// (I  ) 選択条件
													//			MC_IN_BORDER	(001): 内側
													//			MC_ON_BORDER	(002): 辺上
													//			MC_OUT_BORDER	(004): 外側
													//			MC_FOR_POLYGON	(0100): 多角形作成用
					const	MgLine2&	Ln1,		// (I  ) 線分
					const	MgPolyg2&	Pg2,		// (I  ) 多角形
							MgGLine2*	GLn3		// (  O) 分割後の線分群
					);
	
	// ---------------------( ２次元 )------------------------------
	//		線分を多角形で分割し、選択条件に合う線分を分割後の線分群に追加する
	//			選択条件に「辺上同一方向または辺上逆方向」が設定されており、分割後の線分が穴付き多角形の辺上にある場合には、
	//			次のように選択する
	//				(1) 選択条件が辺上同一方向の場合は、同一方向の時残し
	//				(2) 選択条件が辺上逆方向の場合は、逆方向の時残す
	//				(3) その他の場合は捨てる
	//
	static MINT DivideAddLnPg2(						// (  O) ステイタス
													//			MC_NINT			(0): 交差なし
													//			MC_INT			(1): 交差あり
													//			MC_MATCH		(4): 選択条件に合う部分あり
													//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
													//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
													//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
							MINT		Sel,		// (I  ) 選択条件
													//			MC_IN_BORDER	(001): 内側
													//			MC_ON_BORDER	(002): 辺上
													//			MC_OUT_BORDER	(004): 外側
													//			MC_SAME_DIR		(0100):	辺上同一方向
													//			MC_REV_DIR		(0200):	辺上逆方向
					const	MgLine2&	Ln1,		// (I  ) 線分
					const	MgPolyg2&	Pg2,		// (I  ) 多角形
							MgGLine2*	GLn3		// (I O) 分割後の線分群
					);

	// ---------------------( ２次元 )------------------------------
	//		直線を穴付き多角形で分割し、選択条件に合う線分群を得る
	//			選択条件に「辺上同一方向」が設定されており、分割後の線分が穴付き多角形の辺上にある場合には、
	//			次のように選択する
	//				(1) 選択条件が辺上同一方向の場合は、同一方向の時残し
	//				(2) その他の場合は捨てる
	//
	static MINT DivideULnGPg2(						// (  O) ステイタス
													//			MC_NINT			(0): 選択条件に合う線分なし
													//			MC_INT			(1): 選択条件に合う線分あり
							MINT		Sel,		// (I  ) 選択条件
													//			MC_IN_BORDER	(001): 内側
													//			MC_ON_BORDER	(002): 辺上
													//			MC_SAME_DIR		(0100):	辺上同一方向
					const	MgULine2&	ULn1,		// (I  ) 直線
					const	MgGPolyg2&	GPg2,		// (I  ) 穴付き多角形
							MgGLine2*	GLn3		// (  O) 分割後の線分群
					);
	
	// ---------------------( ２次元 )------------------------------
	//		直線を穴付き多角形で分割し、選択条件に合う線分を分割後の線分群に追加する
	//			選択条件に「辺上同一方向」が設定されており、分割後の線分が穴付き多角形の辺上にある場合には、次のように選択する
	//				(1) 選択条件が辺上同一方向の場合は、同一方向の時残し
	//				(2) その他の場合は捨てる
	//
	static MINT DivideAddULnGPg2(					// (  O) ステイタス
													//			MC_NINT			(0): 選択条件に合う線分なし
													//			MC_INT			(1): 選択条件に合う線分あり
													//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
													//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
													//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
							MINT		Sel,		// (I  ) 選択条件
													//			MC_IN_BORDER	(001): 内側
													//			MC_ON_BORDER	(002): 辺上
													//			MC_SAME_DIR		(0100):	辺上同一方向
					const	MgULine2&	ULn1,		// (I  ) 直線
					const	MgGPolyg2&	GPg2,		// (I  ) 穴付き多角形
							MgGLine2*	GLn3		// (I O) 分割後の線分群
					);

	// ---------------------( ２次元 )------------------------------
	//		線分を穴付き多角形で分割し、選択条件に合う線分群を得る
	//			選択条件に「辺上同一方向または辺上逆方向」が設定されており、分割後の線分が穴付き多角形の辺上にある場合には、
	//			次のように選択する
	//				(1) 選択条件が辺上同一方向の場合は、同一方向の時残し
	//				(2) 選択条件が辺上逆方向の場合は、逆方向の時残す
	//				(3) その他の場合は捨てる
	//
	static MINT DivideLnGPg2(						// (  O) ステイタス
													//			MC_NINT			(0): 交差なし
													//			MC_INT			(1): 交差あり
													//			MC_MATCH		(4): 選択条件に合う部分あり
													//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
													//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
													//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
							MINT		Sel,		// (I  ) 選択条件
													//			MC_IN_BORDER	(001): 内側
													//			MC_ON_BORDER	(002): 辺上
													//			MC_OUT_BORDER	(004): 外側
													//			MC_SAME_DIR		(0100):	辺上同一方向
													//			MC_REV_DIR		(0200):	辺上逆方向
					const	MgLine2&	Ln1,		// (I  ) 線分
					const	MgGPolyg2&	GPg2,		// (I  ) 穴付き多角形
							MgGLine2*	GLn3		// (  O) 分割後の線分群
					);
	
	// ---------------------( ２次元 )------------------------------
	//		線分を穴付き多角形で分割し、選択条件に合う線分を分割後の線分群に追加する
	//			選択条件に「辺上同一方向または辺上逆方向」が設定されており、分割後の線分が穴付き多角形の辺上にある場合には、
	//			次のように選択する
	//				(1) 選択条件が辺上同一方向の場合は、同一方向の時残し
	//				(2) 選択条件が辺上逆方向の場合は、逆方向の時残す
	//				(3) その他の場合は捨てる
	//
	static MINT DivideAddLnGPg2(					// (  O) ステイタス
													//			MC_NINT			(0): 交差なし
													//			MC_INT			(1): 交差あり
													//			MC_MATCH		(4): 選択条件に合う部分あり
													//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
													//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
													//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
							MINT		Sel,		// (I  ) 選択条件
													//			MC_IN_BORDER	(001): 内側
													//			MC_ON_BORDER	(002): 辺上
													//			MC_OUT_BORDER	(004): 外側
													//			MC_SAME_DIR		(0100):	辺上同一方向
													//			MC_REV_DIR		(0200):	辺上逆方向
					const	MgLine2&	Ln1,		// (I  ) 線分
					const	MgGPolyg2&	GPg2,		// (I  ) 穴付き多角形
							MgGLine2*	GLn3		// (I O) 分割後の線分群
					);

	// ---------------------( ２次元 )------------------------------
	//		重なっている線分を削除し一本にする (逆方向も含む)				<要効率UP?>
	//
	static void MergeGLn2(
							MgGLine2*	Gln			// (I O) 線分群
					);
	
	//===========================================================================
	// ---------------------( ３次元 )------------------------------
	//		直線を多角形で分割し、選択条件に合う線分群を取り出す
	//
	static MINT DivideULnPg3(						// (  O) ステイタス
													//		 MC_NINT		(0): 交差なし
													//		 MC_INT			(1): 交差あり
							MINT		Sel,		// (I  ) 選択条件
													//		 MC_IN_BORDER	(001): 内側
													//		 MC_ON_BORDER	(002): 辺上
					const	MgULine3&	ULn1,		// (I  ) 直線
					const	MgPolyg3&	Pg2,		// (I  ) 多角形
							MgGLine3	*GLn3		// (  O) 分割後の線分群
					);

	//---------------------( ３次元 )------------------------------
	//		直線を多角形で分割し、分割条件に合う線分を分割後の線分群に追加する
	//
	static MINT DivideAddULnPg3(					// (  O) ステイタス
													//			MC_NINT			(0): 交差なし
													//			MC_INT			(1): 交差あり
													//			MC_MATCH		(4): 選択条件に合う部分あり
													//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
													//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
													//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
							MINT		Sel,		// (I  ) 選択条件
													//			MC_IN_BORDER	(001): 内側
													//			MC_ON_BORDER	(002): 辺上
					const	MgULine3&	ULn1,		// (I  ) 直線
					const	MgPolyg3&	Pg2,		// (I  ) 多角形
							MgGLine3	*GLn3		// (I O) 分割後の線分群
					);

	//// ---------------------( ２次元 )------------------------------
	////	２つの線分間の角度の大小に従う数値CCを求める
	////	[p0からp1に向かう方向]から[p1からp2に向かう方向]への角度により
	////		0.: -0゜, 0.5: 90゜, 1.: 180゜, 1.5: -90゜, <2.: <360(-)゜
	////
	//static MREAL AngSSa3Pt2( 
	//				const	MgPoint2&	p0,
	//				const	MgPoint2&	p1,
	//				const	MgPoint2&	p2
	//				)
	////
	//// ---------------------( ３次元 )------------------------------
	////	２つの線分間の角度の大小に従う数値CCを求める
	////	平面pln3上で[p0からp1に向かう方向]から[p1からp2に向かう方向]への角度により
	////		0.: -0゜, 0.5: 90゜, 1.: 180゜, 1.5: -90゜, <2.: <360(-)゜
	////
	//static MREAL AngSSa3Pt3( 
	//				const	MgPoint3&	P0,
	//				const	MgPoint3&	P1,
	//				const	MgPoint3&	P2,
	//				const	MgPlane3	&Pln3
	//				)
	////
	//// ---------------------( ２次元 )------------------------------
	////	２つの線分間の角度の大小に従う数値CCを求める
	////	[p0からp1に向かう方向]から[p1からp2に向かう方向]への角度により
	////		>-1.: >-180゜, -0.5: -90゜, 0.: -0゜, 0.5: 90゜, 1.: 180゜
	////
	//static MREAL AngSSb3Pt2( 
	//				const	MgPoint2&	p0,
	//				const	MgPoint2&	p1,
	//				const	MgPoint2&	p2
	//				)
	////
	//// ---------------------( ３次元 )------------------------------
	////	２つの線分間の角度の大小に従う数値CCを求める
	////	平面pln3上で[p0からp1に向かう方向]から[p1からp2に向かう方向]への角度により
	////		>-1.: >-180゜, -0.5: -90゜, 0.: -0゜, 0.5: 90゜, 1.: 180゜
	////
	//static MREAL AngSSb3Pt3( 
	//				const	MgPoint3&	P0,
	//				const	MgPoint3&	P1,
	//				const	MgPoint3&	P2,
	//				const	MgPlane3	&Pln3
	//				)
	//
	// ---------------------( ２次元 )------------------------------
	//	選択３角形が分割可能か調べる
	//			 0: 面積が0
	//			 1: 分割可能
	//			-1: 三角形内に他の入隅点があり分割不可(３角形の辺上は対象外)
	//
	static MINT ChkDivTriPtPg2( 
							MgPoint2*	p3,			// 選択３角形
					const	MgPolyg2&	Pgw,		// 分割多角形
					const	MgGInt		&iGIrisumi	// 入隅点リスト
					);
	
	// ---------------------( ３次元 )------------------------------
	//	選択３角形が分割可能か調べる
	//			 0: 面積が0
	//			 1: 分割可能
	//			-1: 三角形上または内に他の入隅点があり分割不可(３角形の辺上は対象外)
	//
	static MINT ChkDivTriPtPg3( 
							MgPoint3	*p3,		// 選択３角形
					const	MgPolyg3&	Pgw,		// 分割多角形
					const	MgGInt		&iGIrisumi,	// 入隅点リスト
					const	MgVect3&	VuPg1		// 多角形の法線方向
					);
	
	// ---------------------( ２次元 )------------------------------
	//		多角形を３角形に分割する
	//
	static MINT DivideTriPg2(						// (  O) ステイタス
													//			MC_NINT			(0): 分割なし
													//			MC_INT			(1): 分割あり
					const	MgPolyg2&	Pg1,		// (I  ) 多角形
							MgGPolyg2*	GPg2		// (  O) 分割後の３角形群
					);
	
	//
	// ---------------------( ３次元 )------------------------------
	//		多角形を３角形に分割する
	//
	static MINT DivideTriPg3(						// (  O) ステイタス
													//			MC_NINT			(0): 分割なし
													//			MC_INT			(1): 分割あり
					const	MgPolyg3&	Pg1,		// (I  ) 多角形
					const	MgVect3&	VuPg1,		// (I  ) 多角形の法線方向
							MgGPolyg3*	GPg2		// (  O) 分割後の３角形群
					);
	
	//===========================================================================
	//		直線と直線の交点　形状演算
	//
	// ---------------------( ２次元 )------------------------------
	//　２本の直線の交点を求める。
	//
	static MINT Intr2ULn2(							// (  O) ステイタス
													//	MC_PARALLEL (-1) 交差なし（平行）
													//	MC_NINT	    (0)	交差なし
													//	MC_INT      (1)	交差あり
					const	MgULine2&	ULn1,		// (I  ) 直線1
					const	MgULine2&	ULn2,		// (I  ) 直線2
							MgPoint2*	po			// (  O) ２次元交点
					);
	
	// ---------------------( ２次元 )------------------------------
	//　直線と線分の交点を求める。
	//　重なっている場合は、交差なし（平行）とみなす。
	//
	static MINT IntrULnLn2(							// (  O) ステイタス
													//	MC_PARALLEL (-1) 交差なし（平行）
													//	MC_NINT	    (0)	交差なし
													//	MC_INT      (1)	交差あり
					const	MgULine2&	ULn1,		// (I  ) 直線1
					const	MgLine2&	Ln2,		// (I  ) 線分2
							MgPoint2*	po			// (  O) ２次元交点
					);
	
	// ---------------------( ２次元 )------------------------------
	//　線分と直線の交点を求める。
	//　重なっている場合は、交差なし（平行）とみなす。
	//
	static inline MINT IntrLnULn2(					// (  O) ステイタス
													//	MC_PARALLEL (-1) 交差なし（平行）
													//	MC_NINT	    (0)	交差なし
													//	MC_INT      (1)	交差あり
					const	MgLine2&	Ln1,		// (I  ) 線分1
					const	MgULine2&	ULn2,		// (I  ) 直線2
							MgPoint2*	po			// (  O) ２次元交点
					)
	{
		return IntrULnLn2( ULn2, Ln1, po);
	}
	
	// ---------------------( ２次元 )------------------------------
	//　線分と線分の交点を求める。
	//　２線分が重なっている場合は、交差なし（平行）とみなす。
	//  ２線分が重なりなく直線的に端部で接合している場合はその接合点を交点とみなす。
	static MINT Intr2Ln2(							// (  O) ステイタス
													//	MC_PARALLEL (-1) 交差なし（平行）
													//  MC_CONNECTION(4) 接続	 （平行）
													//	MC_NINT	    (0)	交差なし
													//	MC_INT      (1)	交差あり
				const	MgLine2&	Ln1,			// (I  ) 線分1
				const	MgLine2&	Ln2,			// (I  ) 線分2
				MgPoint2			*po				// (  O) ２次元交点
				);

	// ---------------------( ２次元 )------------------------------
	//	線分１と直線２との交点を求め、点群に追加する。
	//　重なっている場合は、交差なし（平行）とみなす。
	//
	static MINT IntrAddLnULn2(						// (  O) ステイタス
													//	MC_PARALLEL (-1) 交差なし（平行）
													//	MC_NINT	    (0)	交差なし
													//	MC_INT      (1)	交差あり
					const	MgLine2&	Ln1,		// (I  ) 線分1
					const	MgULine2&	ULn2,		// (I  ) 直線2
							MgGPoint2	*GPt3		// (I O) 交点を追加する点群
													//		 追加交点数(0:交差なし、線分の端部で交差、または重なる
													//					1:交差あり（線分の端部での交差は含まず))
					);
	
	// ---------------------( ２次元 )------------------------------
	//	線分１と線分２との交点と重なり部分の端点を求め、点群に追加する。
	//  交点または重なり部分の端点が線分１の端点である場合は、その点を対象外とする。
	//
	static MINT IntrAdd2Ln2(						// (  O) ステイタス
													//	MC_PARALLEL (-1) 交差なし（平行）
													//	MC_NINT	    (0)	交差なし
													//	MC_INT      (1)	交差あり
													//  MC_REP	    (2)	交差あり（重なり）
					const	MgLine2&	Ln1,		// (I  ) 線分1
					const	MgLine2&	Ln2,		// (I  ) 線分2
							MgGPoint2	*GPt3		// (I O) 交点を追加する線分群
													// 		 追加交点数(0:交差無しまたは線分1が線分2の内側にある重なり(端部同士の重なり含む) |
													//					1:線分2と交差（線分2の端部での交差も含む）または一部重なり(片端重なり含む) |
													//					2:線分2が線分1の内側にある重なり(端部同士の重なりなし)) 
					);

	// ---------------------( ２次元 )------------------------------
	//	直線１と線分２との交点または重なり部分の端点を求め、点群に追加する。
	//
	static MINT IntrAddULnLn2(						// (  O) ステイタス
													//	MC_PARALLEL (-1) 交差なし（平行）
													//	MC_NINT	    (0)	交差なし
													//	MC_INT      (1)	交差あり
													//  MC_REP	    (2)	交差あり（重なり）
					const	MgULine2&	ULn1,		// (I  ) 直線1
					const	MgLine2&	Ln2,		// (I  ) 線分2
							MgGPoint2	*GPt3		// (I O) 交点を追加する線分群
													// 		 追加交点数(0:交差無し |
													//					1:直線1と線分2が交差（線分2の端部での交差も含む）|
													//					2:直線1と線分2が重なる 
					);

	// ---------------------( ３次元 )------------------------------
	//	２本の直線の交点を求める。
	//
	static MINT Intr2ULn3(							// (  O) ステイタス
													//	MC_PARALLEL (-1) 交差なし（平行）
													//	MC_TWIST	(-2) 交差なし（ねじれ）
													//	MC_INT      (1)	交差あり
					const	MgULine3&	ULn1,		// (I  ) 直線1
					const	MgULine3&	ULn2,		// (I  ) 直線2
							MgPoint3	*po			// (  O) ３次元交点
					);

	// ---------------------( ３次元 )------------------------------
	//　直線と線分の交点を求める。
	//
	static MINT IntrULnLn3(							// (  O) ステイタス
													//	MC_PARALLEL (-1) 交差なし（平行）
													//	MC_TWIST    (-2) 交差なし（ねじれ）
													//	MC_NINT	    (0)	交差なし
													//	MC_INT      (1)	交差あり
					const	MgULine3&	ULn1,		// (I  ) 直線1
					const	MgLine3		&Ln2,		// (I  ) 線分2
							MgPoint3	*po			// (  O) ３次元交点
					);
	
	// ---------------------( ３次元 )------------------------------
	//　線分と直線の交点を求める。
	//
	static inline MINT IntrLnULn3(					// (  O) ステイタス
													//	MC_PARALLEL (-1) 交差なし（平行）
													//	MC_TWIST    (-2) 交差なし（ねじれ）
													//	MC_NINT	    (0)	交差なし
													//	MC_INT      (1)	交差あり
					const	MgLine3		&Ln1,		// (I  ) 線分1
					const	MgULine3&	ULn2,		// (I  ) 直線2
							MgPoint3	*po			// (  O) ３次元交点
					)
	{
		return IntrULnLn3(ULn2, Ln1, po);
	}
	
	// ---------------------( ３次元 )------------------------------
	//　線分と線分の交点を求める。
	//　２線分が重なっている場合は、交差なし（平行）とみなす。
	//  ２線分が重なりなく直線的に端部で接合している場合はその接合点を交点とみなす。
	//
	static MINT Intr2Ln3(							// (  O) ステイタス
													//	MC_PARALLEL (-1) 交差なし（平行）
													//  MC_CONNECTION(4) 接続	 （平行）
													//	MC_TWIST    (-2) 交差なし（ねじれ）
													//	MC_NINT	    (0)	交差なし
													//	MC_INT      (1)	交差あり
					const	MgLine3		&Ln1,		// (I  ) 線分1
					const	MgLine3		&Ln2,		// (I  ) 線分2
							MgPoint3	*po			// (  O) ３次元交点
					);
	
	//===========================================================================
	//		平面と直線の交点　形状演算
	//
	// ---------------------( ３次元 )------------------------------
	//
	//	平面と直線の交点を求める。
	//
	static MINT IntrPlnULn3(						// (  O) ステイタス
													//	MC_PARALLEL (-1) 交差なし（平行）
													//	MC_INT      (1)	交差あり
					const	MgPlane3	&Pln1,		// (I  ) 平面1
					const	MgULine3&	ULn2,		// (I  ) 直線2
							MgPoint3	*po			// (  O) ３次元交点(点の標準形式)
					);

	// ---------------------( ３次元 )------------------------------
	//	平面と線分の交点を求める。
	//
	static MINT IntrPlnLn3(							// (  O) ステイタス
													//	MC_PARALLEL (-1) 交差なし（平行）
													//	MC_NINT	    (0)	交差なし
													//	MC_INT      (1)	交差あり
					const	MgPlane3	&Pln1,		// (I  ) 平面1
					const	MgLine3		&Ln2,		// (I  ) 線分2
							MgPoint3	*po			// (  O) ３次元交点(点の標準形式)
					);
	
	// ---------------------( ３次元 )------------------------------
	//	直線と平面の交点を求める。
	//
	static inline MINT IntrULnPln3(					// (  O) ステイタス
													//	MC_PARALLEL (-1) 交差なし（平行）
													//	MC_INT      (1)	交差あり
					const	MgULine3&	ULn1,		// (I  ) 直線1
					const	MgPlane3	&Pln2,		// (I  ) 平面2
							MgPoint3	*po			// (  O) ３次元交点(点の標準形式)
					)
	{
		return IntrPlnULn3(Pln2, ULn1, po);
	}
	
	// ---------------------( ３次元 )------------------------------
	//	線分と平面の交点を求める。
	//
	static inline MINT IntrLnPln3(					// (  O) ステイタス
													//	MC_PARALLEL (-1) 交差なし（平行）
													//	MC_NINT	    (0)	交差なし
													//	MC_INT      (1)	交差あり
					const	MgLine3		&Ln1,		// (I  ) 線分1
					const	MgPlane3	&Pln2,		// (I  ) 平面2
							MgPoint3	*po			// (  O) ３次元交点(点の標準形式)
					)
	{
		return IntrPlnLn3(Pln2, Ln1, po);
	}
	
	//===========================================================================
	//		平面と平面の交線　形状演算
	//
	// ---------------------( ３次元 )------------------------------
	//
	//	平面と平面の交線を求める。
	//
	static MINT Intr2Pln3(							// (  O) ステイタス
													//	MC_PARALLEL (-1)	交差なし（平行）
													//	MC_INT		(1)		交差あり
					const	MgPlane3	&Pln1,		// (I  ) 平面1
					const	MgPlane3	&Pln2,		// (I  ) 平面2
							MgULine3	*ULn3		// (  O) 直線3
					);


	//===========================================================================
	//		形状演算
	//
	//	-------------------------２ＤＬＩＮＥ・ＰＯＩＮＴ----------------------
	//
	// ---------------------( ２次元 )------------------------------
	//		点から直線への垂線の足位置を求める
	static inline void PerpULnPt2(
					const	MgULine2&	ULn1,		// (I  ) 直線1
					const	MgPoint2&	p2,			// (I  ) 点2
							MgPoint2*	po			// (  O) 点から直線への垂線の足
					)
	{
		MREAL		pa1;							//	直線1の始点から垂線の足までの距離
	//
		pa1 = (p2 - ULn1.p) * ULn1.v;				//	cosΘv12v2
		*po = ULn1.p + ULn1.v * pa1;
	}
	
	// ---------------------( ２次元 )------------------------------
	//		点から線分への垂線の足位置を求める
	static inline MINT PerpLnPt2(					// (  O) ステイタス(=MC_ON_LINE	:線分上,
													//					=0			:線分範囲外)
					const	MgLine2&	Ln1,		// (I  ) 線分1
					const	MgPoint2&	p2,			// (I  ) 点2
							MgPoint2*	po			// (  O) 点から直線への垂線の足
					)
	{
		MINT		ist;
		MgPoint2		vd1, vd2;
		MREAL		ss, se;
	//
		vd1 = Ln1.p[1] - Ln1.p[0];
		vd2 = RotL90(vd1);
	//
		SVal(Ln1.p[0], Ln1.p[1], p2, vd2, &ss, &se);
	//
		*po = Ln1.p[0] + (vd1 * (ss / (ss - se)));					//	交点
	//
		if (ss * se < 0 ||											//	線分上
			*po == Ln1.p[0] || *po == Ln1.p[1]) ist = MC_ON_LINE;
		else ist =  0;												//	線分の外
		return ist;
	}
	
	// ---------------------( ２次元 )------------------------------
	//		点から直線への垂線の足位置を求める
	static inline void	MgPerpPtULn2(				// (  O) ステイタス
					const	MgPoint2&	p1,			// (I  ) 点1
					const	MgULine2&	ULn2,		// (I  ) 直線2
							MgPoint2*	po			// (  O) ２次元交点(点の標準形式)
			)
	{
		PerpULnPt2(ULn2, p1, po);
	}
	
	// ---------------------( ２次元 )------------------------------
	//		点から線分への垂線の足位置を求める
	static inline MINT PerpPtLn2(					// (  O) ステイタス(=MC_ON_LINE	:線分上,
													//					=0			:線分範囲外)
					const	MgPoint2&	p1,			// (I  ) 点1
					const	MgLine2		&Ln2,		// (I  ) 線分2
							MgPoint2*	po			// (  O) ２次元交点(点の標準形式)
			)
	{
		return PerpLnPt2(Ln2, p1, po);
	}
	
	// ---------------------( ３次元 )------------------------------
	//		点から直線への垂線の足位置を求める
	static inline void PerpULnPt3(
					const	MgULine3&	ULn1,		// (I  ) 直線1
					const	MgPoint3&	p2,			// (I  ) 点2
							MgPoint3	*po			// (  O) 点から直線への垂線の足
					)
	{
		MREAL	pa1;								//	直線1の始点から垂線の足までの距離
	//
		pa1 = (p2 - ULn1.p) * ULn1.v;				//	cosΘv12v2
		*po = ULn1.p + ULn1.v * pa1;
	}
	
	// ---------------------( ３次元 )------------------------------
	//		点から線分への垂線の足位置を求める
	static inline MINT	PerpLnPt3(					// (  O) ステイタス(=MC_ON_LINE	:線分上,
													//					=0			:線分範囲外)
					const	MgLine3		&Ln1,		// (I  ) 線分1
					const	MgPoint3&	p2,			// (I  ) 点2
							MgPoint3	*po			// (  O) ３次元交点(点の標準形式)
					)
	{
		MINT		ist;
		MgPoint3	vd1, vd2;
		MREAL		ss, se;
		MREAL		d;
	//
		vd1 = Ln1.p[1] - Ln1.p[0];
	//
		d  = p2 * vd1;
		ss = (Ln1.p[0] * vd1) - d;
		se = (Ln1.p[1] * vd1) - d;
	//
		*po = Ln1.p[0] + (vd1 * (ss / (ss - se)));					//	交点
	//
		if (ss * se < 0 ||											//	線分上
			*po == Ln1.p[0] || *po == Ln1.p[1]) ist = MC_ON_LINE;
		else ist = 0;												//	線分の外
	//
		return ist;
	}

	// ---------------------( ３次元 )------------------------------
	//		点から線分への垂線の足位置を求める
	static inline MINT	MgPerpPtLn3(				// (  O) ステイタス(=MC_ON_LINE	:線分上,
					const	MgPoint3&	p1,			// (I  ) 点1
					const	MgLine3		&Ln2,		// (I  ) 直線2
							MgPoint3	*po			// (  O) ３次元交点(点の標準形式)
					)
	{
		return PerpLnPt3( Ln2, p1, po);
	}
	
	// ---------------------( ３次元 )------------------------------
	//		点から直線への垂線の足位置を求める
	static inline void	MgPerpPtULn3(
					const	MgPoint3&	p1,			// (I  ) 点1
					const	MgULine3&	ULn2,		// (I  ) 線分2
							MgPoint3	*po			// (  O) ３次元交点(点の標準形式)
					)
	{
		PerpULnPt3( ULn2, p1, po);
	}
	
	//	-------------------------３Ｄ面・ＰＯＩＮＴ----------------------
	//
	// ---------------------( ３次元 )------------------------------
	//		点から面への垂線の足位置を求める
	//
	static inline void PerpPlnPt3(
					const	MgPlane3	&Pln1,		// (I  ) 面1
					const	MgPoint3&	p2,			// (I  ) 点2
							MgPoint3	*po			// (  O) 点から直線への垂線の足
					)
	{
		MREAL	pa1;								//	点2から垂線の足までの距離
	//
		pa1 = (p2 * Pln1.v) + Pln1.d;
		*po = p2 - Pln1.v * pa1;
	}
	
	// ---------------------( ３次元 )------------------------------
	//		点から面への垂線の足位置を求める
	//
	static inline void PerpPtPln3(
					const	MgPoint3&	p1,			// (I  ) 点1
					const	MgPlane3	&Pln2,		// (I  ) 面2
							MgPoint3	*po			// (  O) ３次元交点(点の標準形式)
					)
	{
		PerpPlnPt3(Pln2, p1, po);
	}

	//===========================================================================
	//		点の移動　形状演算
	//
	// ---------------------( ２次元 )------------------------------
	//
	static inline void MovePt2InDirOfVt2(			//
					const	MgPoint2&	p1,			//  (I  ) 移動点1
					const	MgVect2&	v2,			//	(I  ) 移動方向を示すベクトル
							MREAL		dl,			//	(I  ) 移動距離
							MgPoint2	*p3,		//	(  O) 移動された点3
							MREAL i_Tol = g_gTol.D	//	(I  ) 許容誤差
					)
	{
		*p3 = p1 + dl * UnitizeV2( v2, i_Tol);
	}

	//	------------------------
	static inline void MovePt2AlongULn2(			//
					const	MgPoint2&	p1,			//  (I  ) 移動点1
					const	MgULine2&	ULn2,		//	(I  ) 移動方向を示す直線2
							MREAL		dl,			//	(I  ) 移動距離
							MgPoint2	*p3			//	(  O) 移動された点3
					)
	{
		*p3 = p1 + dl * ULn2.v;
	}

	//	------------------------
	static inline void MovePt2AlongLn2(				//
					const	MgPoint2&	p1,			//  (I  ) 移動点1
					const	MgLine2&	Ln2,		//	(I  ) 移動方向を示す線分2
							MREAL		dl,			//	(I  ) 移動距離
							MgPoint2	*p3,		//	(  O) 移動された点3
							MREAL i_Tol = g_gTol.D	//	(I  ) 許容誤差
					)
	{
		MgVect2	vd, vn;
	
		vd = Ln2.p[1] - Ln2.p[0];
		vn = UnitizeV2( vd, i_Tol);
		*p3 = p1 + dl * vn;
	}

	//
	// ---------------------( ３次元 )------------------------------
	//
	static inline void MovePt3InDirOfVt3(			//
					const	MgPoint3&	p1,			//  (I  ) 移動点1
					const	MgVect3&	v2,			//	(I  ) 移動方向を示すベクトル
							MREAL		dl,			//	(I  ) 移動距離
							MgPoint3	*p3,		//	(  O) 移動された点3
							MREAL i_Tol = g_gTol.D	//	(I  ) 許容誤差
					)
	{
		*p3 = p1 + dl * UnitizeV3( v2, i_Tol);
	}

	//	------------------------
	static inline void MovePt3AlongULn3(			//	(  O) ステイタス
					const	MgPoint3&	p1,			//  (I  ) 移動点1
					const	MgULine3&	ULn2,		//	(I  ) 移動方向を示す直線2
							MREAL		dl,			//	(I  ) 移動距離
							MgPoint3	*p3			//	(  O) 移動された点3
					)
	{
		*p3 = p1 + dl * ULn2.v;
	}

	//	------------------------
	static inline void MovePt3AlongLn3(				//	(  O) ステイタス
					const	MgPoint3&	p1,			//  (I  ) 移動点1
					const	MgLine3		&Ln2,		//	(I  ) 移動方向を示す線分2
							MREAL		dl,			//	(I  ) 移動距離
							MgPoint3	*p3,		//	(  O) 移動された点3
							MREAL i_Tol = g_gTol.D	//	(I  ) 許容誤差
					)
	{
		MgVect3	vd, vn;
	
		vd = Ln2.p[1] - Ln2.p[0];
		vn = UnitizeV3(vd, i_Tol);
		*p3 = p1 + dl * vn;
	}

	//===========================================================================
	//		直線のオフセット　形状演算
	//
	// ---------------------( ２次元 )------------------------------
	//
	//	直線をオフセットする。
	//
	static inline void OffsetULn2(					//	(  O) ステイタス (=1)
					const	MgULine2&	ULn1,		//	(I  ) 直線1
							MREAL		dl,			//	(I  ) オフセット量
							MgULine2 	*ULn2		//	(  O) オフセットされた直線2
					)
	{
	
		ULn2->p = ULn1.p + dl * RotL90(ULn1.v);
		ULn2->v = ULn1.v;
	}

	//	------------------------
	static inline void OffsetLn2(					//	(  O) ステイタス
					const	MgLine2&	Ln1,		//	(I  ) 線分1
							MREAL		dl,			//	(I  ) オフセット量
							MgLine2		*Ln2,		//	(  O) オフセットされた線分2
							MREAL i_Tol = g_gTol.D	//	(I  ) 許容誤差
					)
	{
		MgVect2	vd, vn;
	//
		vd = Ln1.p[1] - Ln1.p[0];
		vn = UnitizeV2( vd, i_Tol);
		Ln2->p[0] = Ln1.p[0] + dl * RotL90(vn);
		Ln2->p[1] = Ln2->p[0] + vd;
	}

	//
	// ---------------------( ３次元 )------------------------------
	//
	static inline void OffsetULn3(					//	(  O) ステイタス
					const	MgULine3&	ULn1,		//	(I  ) 直線1
					const	MgPlane3	&Pln2,		//	(I  ) 直線1が乗る平面2
							MREAL		dl,			//	(I  ) オフセット量
							MgULine3	*ULn3		//	(  O) オフセットされた直線3
					)
	{
		ULn3->p = ULn1.p + dl * (Pln2.v ^ ULn1.v);
		ULn3->v = ULn1.v;
	}

	//	------------------------
	static inline void OffsetLn3(					//	(  O) ステイタス
					const	MgLine3		&Ln1,		//	(I  ) 線分1
					const	MgPlane3	&Pln2,		//	(I  ) 線分1が乗る平面2
							MREAL		dl,			//	(I  ) オフセット量
							MgLine3		*Ln3,		//	(  O) オフセットされた線分3
							MREAL i_Tol = g_gTol.D	//	(I  ) 許容誤差
					)
	{
		MgVect3	vd, vn;
		//
		vd = Ln1.p[1] - Ln1.p[0];
		vn = UnitizeV3( vd, i_Tol);
		MgVect3 vv = vn ^ Pln2.v;
		Ln3->p[0] = Ln1.p[0] + dl * (Pln2.v ^ vn);
		Ln3->p[1] = Ln3->p[0] + vd;
	}
	
	//===========================================================================
	//		点　形状演算
	//
	// ---------------------( ２次元 )------------------------------
	//
	//	点が点上にあるかを調べる。
	//
	static inline bool On(							// (  O) ステイタス
													//			true:	点上にある
													//			false:	点上にない
					const	MgPoint2&	p1,			// (I  ) 点1
					const	MgPoint2&	p2,			// (I  ) 点2
							MREAL rTol= MGPTOL->D	// (I  ) トレランス
					)
	{
	//	return (p1 == p2);
		MgVect2	V1 = p2 - p1;
		return ( Abs_2( V1) <= rTol * rTol);
	}
	
	//===========================================================================
	//		点　形状演算
	//
	// ---------------------( ２次元 )------------------------------
	//		点が点上にあるかを調べる。
	//
	static bool	MgChkPt2OnPt2(						// (  O) ステイタス
													//		 true:  点上にある
													//		 false: 点上にない
					const	MgPoint2&	p1,			// (I  ) 点1
					const	MgPoint2&	p2,			// (I  ) 点2
							MREAL rTol= MGPTOL->D	// (I  ) トレランス
					);

	// ---------------------( ３次元 )------------------------------
	//
	//	点が点上にあるかを調べる。
	//
	static inline bool ChkPt3OnPt3(					// (  O) ステイタス
													//			true:	点上にある
													//			false:	点上にない
					const	MgPoint3&	p1,			// (I  ) 点1
					const	MgPoint3&	p2,			// (I  ) 点2
							MREAL rTol= MGPTOL->D	// (I  ) トレランス
					)
	{
	//	return (p1 == p2);
		MgVect3	V1 = p2 - p1;
		return ( Abs_2( V1) <= rTol * rTol);
	}
	
	//===========================================================================
	//		直線　形状演算
	//
	// ---------------------( ２次元 )------------------------------
	//
	//	点が直線上にあるかを調べる。
	//
	static inline bool ChkPt2OnULn2WS(				// (  O) ステイタス
													//		 	true:	直線上にある
													//		 	false:	直線上にない
					const	MgPoint2&	p1,			// (I  ) 点1
					const	MgULine2&	ULn2,		// (I  ) 直線2
							MINT*		ist,		// (  O) 補助ステイタス
													//			MC_RIGHT			(010): 点が直線の右側
													//			MC_ON_LINE			(020): 点が直線上
													//			MC_LEFT				(040): 点が直線の左側
							MREAL rTol= MGPTOL->D	// (I  ) トレランス
					)
	{
		bool		bst;							// ステイタス
		MgVect2		v2s1;							// 直線2の始点から点1までのベクトル
		MREAL		pa1;							// 直線2から点1までの鉛直距離
	//
	// 直線2の方向（単位ベクトル）と直線2の始点から点1までのベクトルの外積より鉛直距離を求める
		v2s1 = p1 - ULn2.p;					
		pa1 = ULn2.v ^ v2s1;						// 直線2の方向（単位ベクトル）と直線2の始点から点1までのベクトルの外積
	//
	// 鉛直距離が誤差内なら直線上とみなす
		if ( Zero(pa1, rTol)) {
			*ist = MC_ON_LINE;
			bst = true;
	//
	// 鉛直距離が正なら直線の左とみなす
		} else if (pa1 > 0) {
			*ist = MC_LEFT;
			bst = false;
	//
	// 鉛直距離が負なら直線の右とみなす
		} else {
			*ist = MC_RIGHT;
			bst = false;
		}
		return bst;
	}
	
	// ---------------------( ２次元 )------------------------------
	//
	//	点が半直線上にあるかを調べる。
	//
	static inline bool ChkPt2OnHLn2WS(				// (  O) ステイタス
													//		 	true:	半直線上にある
													//		 	false:	半直線上にない
					const	MgPoint2&	p1,			// (I  ) 点1
					const	MgHLine2	&HLn2,		// (I  ) 半直線2
							MINT*		ist,		// (  O) 補助ステイタス
													//			MC_RIGHT			(010): 点が半直線の右側
													//			MC_ON_LINE			(020): 点が半直線上
													//			MC_LEFT				(040): 点が半直線の左側
							MREAL rTol= MGPTOL->D	// (I  ) トレランス
					)
	{
		bool		bst;							// ステイタス
		MgVect2		v2s1;							// 半直線2の始点から点1までのベクトル
		MREAL		pa1;							// 半直線2から点1までの鉛直距離
	//
	// 半直線2の方向（単位ベクトル）と半直線2の始点から点1までのベクトルの外積より鉛直距離を求める
		v2s1 = p1 - HLn2.p;					
		pa1 = HLn2.v ^ v2s1;						// 半直線2の方向（単位ベクトル）と半直線2の始点から点1までのベクトルの外積
	//
	// 鉛直距離が誤差内なら直線上とみなす
		if ( Zero( pa1, rTol)) {
			*ist = MC_ON_LINE;
			bst = true;
	//
	// 鉛直距離が正なら直線の左とみなす
		} else if ( pa1 > 0) {
			*ist = MC_LEFT;
			bst = false;
	//
	// 鉛直距離が負なら直線の右とみなす
		} else {
			*ist = MC_RIGHT;
			bst = false;
		}
		return bst;
	}

	// ---------------------( ２次元 )------------------------------
	//
	//	点が線分上にあるかを調べる。
	//
	static bool ChkPt2OnLn2WS(						// (  O) ステイタス
													//			true:	線分上にある
													//			false:	線分上にない
					const	MgPoint2&	p1,			// (I  ) 点1
					const	MgLine2&	Ln2,		// (I  ) 線分2
							MINT*		ist,		// (  O) 補助ステイタス	(ステイタス1 | ステイタス2)
													//		 ステイタス1
													//			MC_RIGHT			(010): 点が線分の右側
													//			MC_ON_LINE			(020): 点が線分延長直線上
													//			MC_LEFT				(040): 点が線分の左側
													//		 ステイタス2
													//			MC_ON_PS			(001): 点が線分の始点上
													//			MC_INSIDE			(002): 点が線分の内側上
													//			MC_ON_PE			(004): 点が線分の終点上
							MREAL rTol= MGPTOL->D	// (I  ) トレランス
					);
	
	// ---------------------( ２次元 )------------------------------
	//
	//	線分の中心点が直線上にあるかを調べる。
	//
	
	static inline bool ChkLn2OnULn2WS(				// (  O) ステイタス
													//			true:	直線上にある
													//			false:	直線上にない
					const	MgLine2&	Ln1,		// (I  ) 線分1
					const	MgULine2&	ULn2,		// (I  ) 直線2
							MINT*		ist,		// (  O) ステイタス
													//			MC_RIGHT			(010): 点が線分の右側
													//			MC_ON_LINE			(020): 点が直線上
													//			MC_LEFT				(040): 点が線分の左側
							MREAL rTol= MGPTOL->D	// (I  ) トレランス
					)
	{
		MgPoint2 Pt1;
		Pt1 = ( Ln1.p[0] + Ln1.p[1]) * 0.5;
		return ChkPt2OnULn2WS( Pt1, ULn2, ist);
	}
	
	// ---------------------( ２次元 )------------------------------
	//
	//	線分と線分が重なっているかを調べる。
	//
	static bool	ChkLn2OnLn2(						// (  O) ステイタス
													//			true:	重なっている
													//			false:	重なっていない
					const	MgLine2&	Ln1,		// (I  ) 線分1
					const	MgLine2&	Ln2,		// (I  ) 線分2
							MREAL rTol= MGPTOL->D	// (I  ) トレランス
					);
	
	// ---------------------( ３次元 )------------------------------
	//
	//	点が直線上にあるかを調べる。
	//
	static inline bool ChkPt3OnULn3(				// (  O) ステイタス
													//			true:	線分上にある
													//			false:	線分上にない
					const	MgPoint3&	p1,			// (I  ) 点1
					const	MgULine3&	ULn2,		// (I  ) 直線2
							MREAL rTol= MGPTOL->D	// (I  ) トレランス
					)
	{
		MgVect3		v21, va1;
	//	MREAL		pa_2;							//	直線2から点1までの鉛直距離**2
	//
		v21 = p1 - ULn2.p;
		va1 = v21 ^ ULn2.v;
	//	pa_2 = Abs_2(va1);
	//	return ( pa_2 <= rTol * rTol);
		return ( Abs_2(va1) <= rTol * rTol);
	}
	
	// ---------------------( ３次元 )------------------------------
	//
	//	点が半直線上にあるかを調べる。
	//
	static inline bool ChkPt3OnHLn3(				// (  O) ステイタス
													//			true:	線分上にある
													//			false:	線分上にない
					const	MgPoint3&	p1,			// (I  ) 点1
					const	MgHLine3	&HLn2,		// (I  ) 半直線2
							MREAL rTol= MGPTOL->D	// (I  ) トレランス
					)
	{
		MgVect3		v21, va1;
		MREAL		c1;
	//	MREAL		pa_2;							//	直線2から点1までの鉛直距離**2
	//
		v21 = p1 - HLn2.p;
		va1 = v21 ^ HLn2.v;
		c1 = v21 * HLn2.v;
	//	pa_2 = Abs_2(va1);
	//	return ( pa_2 <= rTol * rTol);
		return ( Abs_2(va1) <= rTol * rTol && c1 > -rTol);
	}
	
	// ---------------------( ３次元 )------------------------------
	//
	//	点が線分上にあるかを調べる。
	//
	static bool	ChkPt3OnLn3WS(						// (  O) ステイタス
													//			true:	線分上にある
													//			false:	線分上にない
					const	MgPoint3&	p1,			// (I  ) 点1
					const	MgLine3		&Ln2,		// (I  ) 線分2
							MINT*		ist,		// (  O) 補助ステイタス	(ステイタス1 | ステイタス2)
													//		 ステイタス1
													//			MC_ON_LINE			(020): 点が線分延長直線上
													//		 ステイタス2
													//			MC_ON_PS			(001): 点が線分の始点上
													//			MC_INSIDE			(002): 点が線分の内側上
													//			MC_ON_PE			(004): 点が線分の終点上
							MREAL rTol= MGPTOL->D	// (I  ) トレランス
					);
	
	// ---------------------( ３次元 )------------------------------
	//
	//	線分と線分が重なっているかを調べる。
	//
	static bool	ChkLn3OnLn3(						// (  O) ステイタス
													//			true:	重なっている
													//			false:	重なっていない
					const	MgLine3		&Ln1,		// (I  ) 線分1
					const	MgLine3		&Ln2,		// (I  ) 線分2
							MREAL rTol= MGPTOL->D	// (I  ) トレランス
					);
	
	// ---------------------( ３次元 )------------------------------
	//
	//	線分と線分が重なっているかを調べる。
	//
	static bool	ChkLn3OnLn3WS(						// (  O) ステイタス
													//			true:	重なっている
													//			false:	重なっていない
					const	MgLine3		&Ln1,		// (I  ) 線分1
					const	MgLine3		&Ln2,		// (I  ) 線分2
							MINT*		ist,		// (  O) 補助ステイタス　重なりありの場合は次の通り
													//        -4    -3   -2   -1    0     1    2    3    4 		   	
													//		|--   |--  |--- | -- | --- |----| ---|  --|   --|           	  	
													//		|   --|  --| ---|----| --- | -- |--- |--  |--   |                　	
							MREAL rTol= MGPTOL->D	// (I  ) トレランス
					);

	// ---------------------( ２次元 )------------------------------
	//		ベクトルとベクトルが垂直であるかを確認する
	static inline bool ChkPerp2V2(					// (  O) ステイタス true: 垂直 
					const	MgVect2		&v1,		// (I  ) ベクトル1
					const	MgVect2		&v2			// (I  ) ベクトル2
					)
	{
		MREAL fo = v1 ^ v2; MREAL fi = v1 * v2;
		return ( MGABS(fi/fo)  <= MGPTOL->A);
	}

	// ---------------------( ２次元 )------------------------------
	//		ベクトルとベクトルが垂直であるかを確認し、外積と内積を出力する
	static inline bool ChkPerp2VS2(					// (  O) ステイタス true: 垂直
				const	MgVect2		&v1,			// (I  ) ベクトル1
				const	MgVect2		&v2,			// (I  ) ベクトル2
						MREAL		*so,			// (  O) 外積
						MREAL		*si				// (  O) 内積
				)
	{
		*so = v1 ^ v2; *si = v1 * v2;
		return ( MGABS( *si / *so) <= MGPTOL->A);
	}

	// ---------------------( ３次元 )------------------------------
	//		ベクトルとベクトルが垂直であるかを確認する
	static inline bool ChkPerp2V3(					// (  O) ステイタス true: 垂直
					const	MgVect3&	v1,			// (I  ) ベクトル1
					const	MgVect3&	v2			// (I  ) ベクトル2
					)
	{
		MgVect3 vo = v1 ^ v2; MREAL fi = v1 * v2;
		return (fi * fi <= Abs_2(vo) * MGPTOL->A_2);
	}

	// ---------------------( ３次元 )------------------------------
	//		ベクトルとベクトルが垂直であるかを確認し、外積と内積を出力する
	static inline bool ChkPerp2VS3(					// (  O) ステイタス true: 垂直
					const	MgVect3&	v1,			// (I  ) ベクトル1
					const	MgVect3&	v2,			// (I  ) ベクトル2
							MgVect3		*pso,		// (  O) 外積
							MREAL		*si			// (  O) 内積
					)
	{
		MgVect3 vo = v1 ^ v2; MREAL fi = v1 * v2;
		*pso = vo; *si = fi;
		return (fi * fi <= Abs_2(vo) * MGPTOL->A_2);
	}
	//
	//===========================================================================
	//		点 多角形 演算
	// ---------------------( ２次元 )------------------------------
	//	点と多角形の位置関係を求める
	//
	
	static inline bool ChkPt2OnPg2WS(				// (  O) ステイタス
													//			true:	多角形の内側または辺上
													//			false:	多角形の外側
					const	MgPoint2&	Pt,			// (I  ) 点
					const	MgPolyg2&	Pg,			// (I  ) 多角形
							MINT*		ist,		// (  O) ステイタス
													//			MC_IN_BORDER		(001): 点が多角形の内側
													//			MC_ON_BORDER		(002): 点が多角形の辺上(または頂点上)
													//			MC_OUT_BORDER		(004): 点が多角形の外側
							MREAL rTol= MGPTOL->D	// (I  ) トレランス
					)
	{
		bool	bst;
		MINT	ist1;
		MINT	ict;
		
		ist1 = CountPg2AroundPt2( Pt, NULL, Pg, &ict, rTol);
		if ( ist1) {
			*ist = MC_ON_BORDER;
			bst = true;												// 多角形の辺上(または頂点上)
		} else {
			if ( ict) {
				*ist = MC_IN_BORDER;
				bst = true;											// 多角形の内側
			}
			else {
				*ist = MC_OUT_BORDER;
				bst = false;										// 多角形の外側
			}
		}
		return bst;
	}
	
	// ---------------------( ２次元 )------------------------------
	//	線分の中心点と多角形の位置関係を求める ２Ｄ
	//
	static inline bool ChkLn2OnPg2WS(				// (  O) ステイタス
													//			true:	多角形の内側または辺上
													//			false:	多角形の外側
					const	MgLine2&	Ln1,		// (I  ) 線分
					const	MgPolyg2&	Pg2,		// (I  ) 多角形
							MINT*		isth,		// (  O) 補助ステイタス	(ステイタス1 | ステイタス2)
													// 		 テイタス1
													//			MC_IN_BORDER		(001):	点が多角形の内側
													//			MC_ON_BORDER		(002):	点が多角形の辺上(または頂点上)
													//			MC_OUT_BORDER		(004):	点が多角形の外側
													//		ステイタス2
													//			MC_ON_SIDE_SAME 	(010):	辺上(同一方向)
													//			MC_ON_SIDE_REV		(020):	辺上(逆方向)
													//			MC_ON_TOP			(040): 頂点上
							MREAL rTol= MGPTOL->D	// (I  ) トレランス
					)
	{
		bool		bst;
		MgPoint2	pt1;
		MgVect2		vt1;
		MINT		ist1;
		MINT		ict;
		
		pt1 = ( Ln1.p[0] + Ln1.p[1]) * 0.5;
		vt1 = Ln1.p[1] - Ln1.p[0];
		
		ist1 = CountPg2AroundPt2( pt1, &vt1, Pg2, &ict, rTol);
		if ( ist1) {
			*isth = MC_ON_BORDER | ist1;
			bst = true;												// 多角形の辺上(または頂点上)
		} else {
			if ( ict) {
				*isth = MC_IN_BORDER;
				bst = true;											// 多角形の内側
			}
			else {
				*isth = MC_OUT_BORDER;
				bst = false;										// 多角形の外側
			}
		}
		return bst;
	}
	
	// ---------------------( ２次元 )------------------------------
	//	点と穴付き多角形の位置関係を求める
	//
	static bool ChkPt2OnGPg2WS(						// (  O) ステイタス
													//			true:	穴付き多角形の内側または辺上
													//			false:	穴付き多角形の外側
					const	MgPoint2&	Pt,			// (I  ) 点
					const	MgGPolyg2&	GPg,		// (I  ) 穴付き多角形
							MINT*		ist,		// (  O) ステイタス
													//			MC_IN_BORDER		(001): 点が穴付き多角形の内側
													//			MC_ON_BORDER		(002): 点が穴付き多角形の辺上(または頂点上)
													//			MC_OUT_BORDER		(004): 点が穴付き多角形の外側
							MREAL rTol= MGPTOL->D	// (I  ) トレランス
					);

	// ---------------------( ２次元 )------------------------------
	//	線分の中心点と穴付き多角形の位置関係を求める
	//
	static bool ChkLn2OnGPg2WS(						// (  O) ステイタス
													//			true:	穴付き多角形の内側または辺上
													//			false:	穴付き多角形の外側
					const	MgLine2&	Ln1,		// (I  ) 線分
					const	MgGPolyg2&	GPg2,		// (I  ) 穴付き多角形
							MINT*		isth,		// (  O) 補助ステイタス	(ステイタス1 | ステイタス2)
													// 		 テイタス1
													//			MC_IN_BORDER		(001): 点が穴付き多角形の内側
													//			MC_ON_BORDER		(002): 点が穴付き多角形の辺上(または頂点上)
													//			MC_OUT_BORDER		(004): 点が穴付き多角形の外側
													// 		 テイタス2
													//			MC_ON_SIDE_SAME		(010): 辺上(同一方向)	(MC_ON_BORDERと共に設定)
													//			MC_ON_SIDE_REV		(020): 辺上(逆方向)		(MC_ON_BORDERと共に設定)
													//			MC_ON_TOP			(040): 頂点上			(MC_ON_BORDERと共に設定)
							MREAL rTol= MGPTOL->D	// (I  ) トレランス
					);
	
	// ---------------------( ２次元 )------------------------------
	// 点と多角形の位置関係を求める
	// 多角形の辺が点の周りを回る周回数を求める（実体一周:1　穴一周:-1）
	static MINT	CountPg2AroundPt2(				// (  O) ステイタス
													//								(000): 多角形の中または外	
													//			MC_ON_SIDE_SAME		(010): 辺上(同一方向)
													//			MC_ON_SIDE_REV		(020): 辺上(逆方向)
													//			MC_ON_SIDE			(030): 辺上(線分方向=NULL: 線分方向指定無し)
													//			MC_ON_TOP			(040): 頂点上
					const	MgPoint2&	Pt,			// (I  ) 点
					const	MgVect2		*vst,		// (I  ) 線分方向 または NULL
													//		 引数がNULLで無ければ点が辺上の場合、
													//		 辺の線分方向と比較する
					const	MgPolyg2&	Pg,			// (I  ) 多角形
							MINT*		ict,		// (  O) 周回カウント
													//			>0: 多角形が実体の場合の周回数	(1:1周)
													//			=0: 頂点、辺上、多角形の外
													//			<0: 多角形が穴の場合の周回数	(-1:1周)	
							MREAL rTol= MGPTOL->D	// (I  ) トレランス
					);
	
	// ---------------------( ３次元 )------------------------------
	//	点と多角形の位置関係を求める ３Ｄ
	//
	/*
	static bool ChkPt3OnPg3WS(						// (  O) ステイタス
													//			true:	多角形の内側または辺上
													//			false:	多角形の外側
					const	MgPoint3&	Pt,			// (I  ) 点
					const	MgPolyg3&	Pg,			// (I  ) 多角形
							MINT*		ist,		// (  O) ステイタス
								  					//		 多角形の平面と点の位置関係
													//			MC_UPPER			(010): 点が平面の上側	
													//			MC_ON_PLANE			(020): 点が平面上
													//			MC_LOWER			(040): 点が平面の下側
													//		 多角形と点の位置関係
													//			MC_IN_BORDER		(001): 点が多角形の内側
													//			MC_ON_BORDER		(002): 点が多角形の辺上
													//			MC_OUT_BORDER		(004): 点が多角形の外側
							MREAL rTol= MGPTOL->D	// (I  ) トレランス
					);
	
	*/
	//
	// ---------------------( ３次元 )------------------------------
	//	線分の中心点と多角形の位置関係を求める
	//
	/*
	static inline bool ChkLn3OnPg3WS(				// (  O) ステイタス
													//			true:	多角形上
													//			false:	多角形外
					const	MgLine3		&Ln1,		// (I  ) 線分
					const	MgPolyg3&	Pg2,		// (I  ) 多角形
							MINT*		ist,		// (  O) 補助ステイタス
								  					//		 多角形の平面と点の位置関係
													//			MC_UPPER			(010): 点が平面の上側	
													//			MC_ON_PLANE			(020): 点が平面上
													//			MC_LOWER			(040): 点が平面の下側
													//       多角形と点の位置関係
													//			MC_IN_BORDER		(001): 点が多角形の内側
													//			MC_ON_BORDER		(002): 点が多角形の辺上
													//			MC_OUT_BORDER		(004): 点が多角形の外側
							MREAL rTol= MGPTOL->D	// (I  ) トレランス
					)
	{
		MgPoint3	Pt1;
		Pt1 = ( Ln1.p[0] + Ln1.p[1]) * 0.5;
		return OnPtPg3( Pt1, Pg2, ist);
	}

	*/
	//		平面　形状演算
	//
	// ---------------------( ３次元 )------------------------------
	//
	//	点が平面上にあるかを調べる。
	//
	static inline bool ChkPt3OnPln3WS(				// (  O) ステイタス
													//			true:  平面上
													//			false: 平面外
					const	MgPoint3&	p1,			// (I  ) 点1
					const	MgPlane3	&Pln2,		// (I  ) 平面2
							MINT*		ist,		// (  O) 補足ステイタス
													//			MC_UPPER			(010): 点が平面の上側	
													//			MC_ON_PLANE			(020): 点が平面上
													//			MC_LOWER			(040): 点が平面の下側
							MREAL rTol= MGPTOL->D	// (I  ) トレランス
					)
	{
		bool	bst;
		MREAL	pa1;								//	点から平面までの距離
	//
		pa1 = (Pln2.v * p1) + Pln2.d;
		if (pa1 > rTol) {
			*ist = MC_UPPER;
			bst = false;
		} else if (pa1 < -rTol) {
			*ist = MC_LOWER;
			bst = false;
		} else {
			*ist = MC_ON_PLANE;
			bst = true;
		}
		return bst;
	}

	// ---------------------( ３次元 )------------------------------
	//	直線が平面上にあるかを調べる。
	//
	static inline bool ChkULn3OnPln3(				// (  O) ステイタス
													//			true:  平面上
													//			false: 平面外
					const	MgULine3&	ULn1,		// (I  ) 直線1
					const	MgPlane3	&Pln2,		// (I  ) 平面2
							MREAL rTol= MGPTOL->D	// (I  ) トレランス
					)
	{
		int		ist;
		if ( ChkPt3OnPln3WS(ULn1.p, Pln2, &ist)) {
	//
	//	平行のチェック
	//
			if ( ChkPerp2V3(ULn1.v, Pln2.v))
				return true;									//	平面上
		}
		return false;
	}
	
	// ---------------------( ３次元 )------------------------------
	//	線分が平面上にあるかを調べる。
	//
	static inline bool ChkLn3OnPln3WS(				// (  O) ステイタス
													//			true:  平面上
													//			false: 平面外
					const	MgLine3		&Ln1,		// (I  ) 線分1
					const	MgPlane3	&Pln2,		// (I  ) 平面2
							MINT*		ist,		// (  O) 補足ステイタス
								  					//		 多角形の平面と点の位置関係
													//			MC_PS_UPPER			(001): 始点が平面の上側	
													//			MC_PS_ON_PLANE		(002): 始点が平面上
													//			MC_PS_LOWER			(004): 始点が平面の下側
													//       多角形と点の位置関係
													//			MC_PE_UPPER			(010): 終点が平面の上側	
													//			MC_PE_ON_PLANE		(020): 終点が平面上
													//			MC_PE_LOWER			(040): 終点が平面の下側
							MREAL rTol= MGPTOL->D	// (I  ) トレランス
					)
	{
		bool	bs, be;
		MINT	ists, iste;
	
		bs = ChkPt3OnPln3WS(Ln1.p[0], Pln2, &ists);
		be = ChkPt3OnPln3WS(Ln1.p[1], Pln2, &iste);
		*ist = (ists * MC_PS_UPPER + iste * MC_PE_UPPER)/ MC_UPPER;
		return (bs && be);
	}
	
	// ---------------------( ２次元 )------------------------------
	//
	//		２つの４角形が重なるか否かをチェックする。
	// 
	static inline bool	MgChkRct2OnRct2(			// (  O) ステイタス
													//		 true   交差あり
													//		 false  交差なし
					const	MgRect2		&rct10,		// (I  ) ４角形1 (min,max)
					const	MgRect2		&rct20,		// (I  ) ４角形2 (min,max)
							MREAL rTol= MGPTOL->D	// (I  ) トレランス
					)
	{
		return rct10.p[0].x < rct20.p[1].x + MGPTOL->D &&
			   rct20.p[0].x < rct10.p[1].x + MGPTOL->D &&
			   rct10.p[0].y < rct20.p[1].y + MGPTOL->D &&
			   rct20.p[0].y < rct10.p[1].y + MGPTOL->D;
	}

	// ---------------------( ３次元 )------------------------------
	//
	//		２つの４角形が重なるか否かをチェックする。
	//
	static inline bool	MgChkRct3OnRct3(			// (  O) ステイタス
													//		 true   交差あり
													//		 false  交差なし
					const	MgRect3		&rct10,		// (I  ) ４角形1 (min,max)
					const	MgRect3		&rct20,		// (I  ) ４角形2 (min,max)
							MREAL rTol= MGPTOL->D	// (I  ) トレランス
					)
	{
		return rct10.p[0].x < rct20.p[1].x + MGPTOL->D &&
			   rct20.p[0].x < rct10.p[1].x + MGPTOL->D &&
			   rct10.p[0].y < rct20.p[1].y + MGPTOL->D &&
			   rct20.p[0].y < rct10.p[1].y + MGPTOL->D &&
			   rct10.p[0].z < rct20.p[1].z + MGPTOL->D &&
			   rct20.p[0].z < rct10.p[1].z + MGPTOL->D;
	}
	
	// ---------------------( ２次元 )------------------------------
	//
	//		点がＭＩＮ／ＭＡＸ領域が内にあるか否かをチェックする。
	//
	static inline	bool ChkPt2InMMR2(
					const	MgPoint2&	pt1,		// Point
					const	MgMinMaxR2	&mx2,		// MIN/MAX
							MREAL rTol= MGPTOL->D	// (I  ) トレランス
					)
	{
		return pt1.x <= mx2.max.x + MGPTOL->D && mx2.min.x < pt1.x + MGPTOL->D &&
			   pt1.y <= mx2.max.y + MGPTOL->D && mx2.min.y < pt1.y + MGPTOL->D;
	}
	
	// ---------------------( ３次元 )------------------------------
	//
	//		点がＭＩＮ／ＭＡＸ領域が内にあるか否かをチェックする。
	//
	static inline	bool ChkPt3InMMR3(
					const	MgPoint3&	pt1,		// Point
					const	MgMinMaxR3	&mx2,		// MIN/MAX
							MREAL rTol= MGPTOL->D	// (I  ) トレランス
					)
	{
		return pt1.x < mx2.max.x + MGPTOL->D && mx2.min.x < pt1.x + MGPTOL->D &&
			   pt1.y < mx2.max.y + MGPTOL->D && mx2.min.y < pt1.y + MGPTOL->D &&
			   pt1.z < mx2.max.z + MGPTOL->D && mx2.min.z < pt1.z + MGPTOL->D;
	}

	// ---------------------( ２次元 )------------------------------
	//
	//		２つのＭＩＮ／ＭＡＸ領域が重なるか否かをチェックする。
	//
	static inline	bool ChkMMI2OnMMI2(
					const	MgMinMaxI2	&mx1,		// MIN/MAX
					const	MgMinMaxI2	&mx2,		// MIN/MAX
							MREAL rTol= MGPTOL->D	// (I  ) トレランス
					)
	{
		return mx1.min.x <= mx2.max.x && mx2.min.x <= mx1.max.x &&
			   mx1.min.y <= mx2.max.y && mx2.min.y <= mx1.max.y;
	}

	// ---------------------( ２次元 )------------------------------
	//
	//		２つのＭＩＮ／ＭＡＸ領域が重なるか否かをチェックする。
	//
	static inline	bool ChkMMR2OnMMR2(
					const	MgMinMaxR2	&mx1,		// MIN/MAX
					const	MgMinMaxR2	&mx2,		// MIN/MAX
							MREAL rTol= MGPTOL->D	// (I  ) トレランス
					)
	{
		return mx1.min.x < mx2.max.x + MGPTOL->D && mx2.min.x < mx1.max.x + MGPTOL->D &&
			   mx1.min.y < mx2.max.y + MGPTOL->D && mx2.min.y < mx1.max.y + MGPTOL->D;
	}

	// ---------------------( ３次元 )------------------------------
	//
	//		２つのＭＩＮ／ＭＡＸ領域が重なるか否かをチェックする。
	//
	static inline	bool ChkMMR3OnMMR3(
					const	MgMinMaxR3	&mx1,		// MIN/MAX
					const	MgMinMaxR3	&mx2,		// MIN/MAX
							MREAL rTol= MGPTOL->D	// (I  ) トレランス
					)
	{
		return mx1.min.x < mx2.max.x + MGPTOL->D && mx2.min.x < mx1.max.x + MGPTOL->D &&
			   mx1.min.y < mx2.max.y + MGPTOL->D && mx2.min.y < mx1.max.y + MGPTOL->D &&
			   mx1.min.z < mx2.max.z + MGPTOL->D && mx2.min.z < mx1.max.z + MGPTOL->D;
	}

	// ---------------------( ２次元 )------------------------------
	// P1を原点としたP2の位置の象限を求める
	//
	static inline MINT Quadrant	(					// (  O) 1～4: 象限
					const	MgPoint2&	p1,			// (I  ) P1
					const	MgPoint2&	p2			// (I  ) P2
					)
	{
		return ((p2.x > p1.x) ? ((p2.y > p1.y) ? 1 : 4) : ((p2.y > p1.y) ? 2 : 3));
	}
		
	// ---------------------( ２次元 )------------------------------
	//		点列を並べ方向に従った座標順に並べる（同一点は間引く）
	//
	static void	SortVGPt2(
					const	MgVect2&	v1,			// (I  ) 点列の並べ方向
							MgGPoint2*	GPt2		// (I O) 並べる点列
					);

	// ---------------------( ２次元 )------------------------------
	//		点列を線分の始点から終点に向かう方向に従った座標順に並べる（同一点は間引く）
	//
	static inline void	SortLnGPt2(
					const	MgLine2&	Ln1,		// (I  ) 点列の並べ方向を示す線分
							MgGPoint2*	GPt2		// (I O) 並べる点列
					)
	{
		MgVect2		v1;
		v1 = Ln1.p[1] - Ln1.p[0];
		SortVGPt2( v1, GPt2);
	}

	//// ---------------------( ３次元 )------------------------------
	////		点列を並べ方向に従った座標順に並べる（同一点は間引く）
	////
	//static void	MgSortVGPt3(
	//				const	MgVect3&	V1,			// (I  ) 点列の並べ方向
	//						MgGPoint3	*GPt2		// (I O) 並べる点列
	//				);

	//// ---------------------( ３次元 )------------------------------
	////		点列を線分の始点から終点に向かう方向に従った座標順に並べる（同一点は間引く）
	////
	//static inline void	MgSortLnGPt3(
	//				const	MgLine3		&Ln1,		// (I  ) 点列の並べ方向を示す線分
	//						MgGPoint3	*GPt2		// (I O) 並べる点列
	//				)
	//	{
	//		MgVect3		V1;
	//		V1 = Ln1.p[1] - Ln1.p[0];
	//		MgSortVGPt3( V1, GPt2);
	//	}

	//===========================================================================
	//	線分群を正規化する。（線分の逆方向の重なりを除去する）
	//
	static inline void Norm(						//
					const	MgGLine2&	GLn1,		// (I  ) 線分
					const	MgGLine2*	GLn2		// (  O) 重なりを除去後の線分
					)
	{
		MgLine2		work[MX_LIN1+1];				// 線分の数用のエリアを追加した作業エリアを確保する
		MgGLine2	*GLn = (MgGLine2*)work;
		DivideGLn2( GLn1, GLn);						// 線分を交点で分割する
	//	ThinOut( *GLn, GLn2);						// 往復線分と開放端を持つ線分を間引く
	}
	
	//===========================================================================
	//		共通　形状演算
	//
	//	------------- Ｓ値の算出　------------------
	//
	// ---------------------( ３次元 )------------------------------
	//		直線と点の位置関係を表すＳ値
	//
	static inline MREAL SVal2ULn3(					// (  O) Ｓ値(２直線の方向ベクトルからなる平行６面体の体積）
				    const	MgULine3&	ULn1,		// (I  ) 直線1
					const	MgULine3&	ULn2		// (I  ) 直線2
					)
	{
	//		( (ULn1.p.x*ULn1.v.y-ULn1.p.y*ULn1.v.x) * (ULn2.p.z*ULn2.v.w-ULn2.p.w*ULn2.v.z)
	//		- (ULn1.p.x*ULn1.v.z-ULn1.p.z*ULn1.v.x) * (ULn2.p.y*ULn2.v.w-ULn2.p.w*ULn2.v.y)
	//		+ (ULn1.p.x*ULn1.v.w-ULn1.p.w*ULn1.v.x) * (ULn2.p.y*ULn2.v.z-ULn2.p.z*ULn2.v.y)
	//		+ (ULn1.p.y*ULn1.v.z-ULn1.p.z*ULn1.v.y) * (ULn2.p.x*ULn2.v.w-ULn2.p.w*ULn2.v.x)
	//		- (ULn1.p.y*ULn1.v.w-ULn1.p.w*ULn1.v.y) * (ULn2.p.x*ULn2.v.z-ULn2.p.z*ULn2.v.x)
	//		+ (ULn1.p.z*ULn1.v.w-ULn1.p.w*ULn1.v.z) * (ULn2.p.x*ULn2.v.y-ULn2.p.y*ULn2.v.x))
	//
		return	( ( ULn1.p.x*ULn1.v.y-ULn1.p.y*ULn1.v.x) * ( -ULn2.v.z)
				- ( ULn1.p.x*ULn1.v.z-ULn1.p.z*ULn1.v.x) * ( -ULn2.v.y)
				+ ( -ULn1.v.x) * ( ULn2.p.y*ULn2.v.z-ULn2.p.z*ULn2.v.y)
				+ ( ULn1.p.y*ULn1.v.z-ULn1.p.z*ULn1.v.y) * ( -ULn2.v.x)
				- ( -ULn1.v.y) * ( ULn2.p.x*ULn2.v.z-ULn2.p.z*ULn2.v.x)
				+ ( -ULn1.v.z) * ( ULn2.p.x*ULn2.v.y-ULn2.p.y*ULn2.v.x));
	}

	// ---------------------( ３次元 )------------------------------
	//		直線と点の位置関係を表すＳ値
	//
	static inline MREAL SValULnLn3(				// (  O) Ｓ値(直線の方向ベクトルと線分からなる平行６面体の体積）
				    const	MgULine3&	ULn1,		// (I  ) 直線1
					const	MgLine3&	Ln2			// (I  ) 線分2
					)
	{
	//		( (ULn1.p.x*ULn1.v.y-ULn1.p.y*ULn1.v.x) * (Ln2.p[0].z*Ln2.p[1].w-Ln2.p[0].w*Ln2.p[1].z)
	//		- (ULn1.p.x*ULn1.v.z-ULn1.p.z*ULn1.v.x) * (Ln2.p[0].y*Ln2.p[1].w-Ln2.p[0].w*Ln2.p[1].y)
	//		+ (ULn1.p.x*ULn1.v.w-ULn1.p.w*ULn1.v.x) * (Ln2.p[0].y*Ln2.p[1].z-Ln2.p[0].z*Ln2.p[1].y)
	//		+ (ULn1.p.y*ULn1.v.z-ULn1.p.z*ULn1.v.y) * (Ln2.p[0].x*Ln2.p[1].w-Ln2.p[0].w*Ln2.p[1].x)
	//		- (ULn1.p.y*ULn1.v.w-ULn1.p.w*ULn1.v.y) * (Ln2.p[0].x*Ln2.p[1].z-Ln2.p[0].z*Ln2.p[1].x)
	//		+ (ULn1.p.z*ULn1.v.w-ULn1.p.w*ULn1.v.z) * (Ln2.p[0].x*Ln2.p[1].y-Ln2.p[0].y*Ln2.p[1].x))
	//
		return
		(	  (ULn1.p.x*ULn1.v.y-ULn1.p.y*ULn1.v.x) * (Ln2.p[0].z-Ln2.p[1].z)
			- (ULn1.p.x*ULn1.v.z-ULn1.p.z*ULn1.v.x) * (Ln2.p[0].y-Ln2.p[1].y)
			+ (-ULn1.v.x) * (Ln2.p[0].y*Ln2.p[1].z-Ln2.p[0].z*Ln2.p[1].y)
			+ (ULn1.p.y*ULn1.v.z-ULn1.p.z*ULn1.v.y) * (Ln2.p[0].x-Ln2.p[1].x)
			- (-ULn1.v.y) * (Ln2.p[0].x*Ln2.p[1].z-Ln2.p[0].z*Ln2.p[1].x)
			+ (-ULn1.v.z) * (Ln2.p[0].x*Ln2.p[1].y-Ln2.p[0].y*Ln2.p[1].x));
	}

	// ---------------------( ３次元 )------------------------------
	//		直線と点の位置関係を表すＳ値
	//
	static inline MREAL SVal2Ln3(					// (  O) Ｓ値(2線分からなる平行６面体の体積）
				    const	MgLine3&	Ln1,		// (I  ) 線分1
					const	MgLine3&	Ln2			// (I  ) 線分2
					)
	{
	//		( (Ln1.p[0].x*Ln1.v.y-Ln1.p[0].y*Ln1.v.x) * (Ln2.p[0].z*Ln2.p[1].w-Ln2.p[0].w*Ln2.p[1].z)
	//		- (Ln1.p[0].x*Ln1.v.z-Ln1.p[0].z*Ln1.v.x) * (Ln2.p[0].y*Ln2.p[1].w-Ln2.p[0].w*Ln2.p[1].y)
	//		+ (Ln1.p[0].x*Ln1.v.w-Ln1.p[0].w*Ln1.v.x) * (Ln2.p[0].y*Ln2.p[1].z-Ln2.p[0].z*Ln2.p[1].y)
	//		+ (Ln1.p[0].y*Ln1.v.z-Ln1.p[0].z*Ln1.v.y) * (Ln2.p[0].x*Ln2.p[1].w-Ln2.p[0].w*Ln2.p[1].x)
	//		- (Ln1.p[0].y*Ln1.v.w-Ln1.p[0].w*Ln1.v.y) * (Ln2.p[0].x*Ln2.p[1].z-Ln2.p[0].z*Ln2.p[1].x)
	//		+ (Ln1.p[0].z*Ln1.v.w-Ln1.p[0].w*Ln1.v.z) * (Ln2.p[0].x*Ln2.p[1].y-Ln2.p[0].y*Ln2.p[1].x))
	//
		return
		(	  (Ln1.p[0].x*Ln1.p[1].y-Ln1.p[0].y*Ln1.p[1].x) * (Ln2.p[0].z-Ln2.p[1].z)
			- (Ln1.p[0].x*Ln1.p[1].z-Ln1.p[0].z*Ln1.p[1].x) * (Ln2.p[0].y-Ln2.p[1].y)
			+ (Ln1.p[0].x-Ln1.p[1].x) * (Ln2.p[0].y*Ln2.p[1].z-Ln2.p[0].z*Ln2.p[1].y)
			+ (Ln1.p[0].y*Ln1.p[1].z-Ln1.p[0].z*Ln1.p[1].y) * (Ln2.p[0].x-Ln2.p[1].x)
			- (Ln1.p[0].y-Ln1.p[1].y) * (Ln2.p[0].x*Ln2.p[1].z-Ln2.p[0].z*Ln2.p[1].x)
			+ (Ln1.p[0].z-Ln1.p[1].z) * (Ln2.p[0].x*Ln2.p[1].y-Ln2.p[0].y*Ln2.p[1].x));
	}

	// ---------------------( ３次元 )------------------------------
	//		直線と点の位置関係を表すＳ値
	//
	static inline void SValLnULnS3(					//
					const	MgLine3&	Ln1,		// (I  ) 線分1
				    const	MgULine3&	ULn2,		// (I  ) 直線2
					const	MgVect3&	vso,		// (I  ) 線分1と直線2がある平面の法線ベクトル
							MREAL*		ss,			// (  O) 線分１の始点側Ｓ値
													//		(直線2の方向ベクトルと平面の法線ベクトルと
													//		 線分1の始点で出来る平行６面体の体積)
							MREAL*		se			// (  O) 線分１の終点側Ｓ値
													//		(直線2の方向ベクトルと平面の法線ベクトルと
													//		 線分1の終点で出来る平行６面体の体積)
					)
	{
		MgVect3		N;								//	直線２と垂直ベクトルからなる平面の係数
		MREAL		d;								//	基準点から直線1の始点までの同平面に垂直距離
	//
		N = ULn2.v ^ vso;
		d =  ULn2.p * N;
		*ss = (Ln1.p[0] * N) - d;
		*se = (Ln1.p[1] * N) - d;
	}

	// ---------------------( ３次元 )------------------------------
	//		直線と点の位置関係を表すＳ値
	//
	static inline void SVal2LnS3(					// (  O) ステイタス
				    const	MgLine3&	Ln1,		// (I  ) 線分1
					const	MgLine3&	Ln2,		// (I  ) 線分2
					const	MgVect3&	vso,		// (I  ) 線分1と線分2がある平面の法線ベクトル
							MREAL*		ss,			// (I  ) 線分１の始点側Ｓ値
													//		(線分2と平面の法線ベクトルと線分1の始点で
													//		 出来る平行６面体の体積)
							MREAL*		se			// (I  ) 線分１の終点側Ｓ値
													//		(直線2と平面の法線ベクトルと線分1の終点で
													//		 出来る平行６面体の体積)
			)
	{
		MgVect3		N;								//	直線２と垂直ベクトルからなる平面の係数
		MREAL		d;								//	基準点から直線1の始点までの同平面に垂直距離
		MgVect3		vd;								//	線分２のベクトル
	//
		vd = Ln2.p[1] - Ln2.p[0];
		N = vd ^ vso;
		d =  Ln2.p[0] * N;
		*ss = (Ln1.p[0] * N) - d;
		*se = (Ln1.p[1] * N) - d;
	}

		
		
	//===========================================================================
	//		平面 形状演算
	//

	static MgPlane3 Plane3CPPt( const MgPoint3 *pt);							// ３点より平面の方程式の係数と定数を求める（正規化）

	static MgPlane3 Plane3C3Pt( const MgPoint3& p0, const MgPoint3& p1,			// ３点より平面の方程式の係数と定数を求める（正規化）
					   const MgPoint3& p2);

	static MgPlane3 Plane3CPg( const MgPolyg3 &pg);								// 多角形より平面の方程式の係数と定数を求める（正規化）

	static MgPlane3 Plane3C2Pt2F( const MgPoint2& p1, const MgPoint2& p2,		// 始点と終点の２点と勾配より平面の方程式の係数と定数を求める（正規化）
						 MREAL rkb, MREAL z);

	static MgPlane3 Plane3CLn2F( const MgLine2& ln1,							// 線分と勾配より平面の方程式の係数と定数を求める（正規化）
						MREAL rkb, MREAL z = 0.);

	//===========================================================================
	//		平面の方程式の係数と定数を正規化した平面を求める
	//		ax+by+cz+d=0	(a*a+b*b+c*c=1.0)
	//
	static MgPlane3 NormPln(						// (  O) 正規化後の３次元平面
				const	MgPlane3	&pl,			// (I  ) ３次元平面
						MREAL i_Tol = g_gTol.D);	// (I  ) 許容誤差





	////===========================================================================
	////		３点より平面の方程式の係数と定数を求める（正規化）
	////
	////	ax+by+cz+d=0
	////
	//static inline MgPlane3 Plane3CPPt(			// (  O) 平面式
	//				const	MgPoint3*	pt,			// (I  ) ３点座標
	//						MREAL i_Tol = g_gTol.D	// (I  ) 許容誤差
	//				)
	//{
	//	MgPlane3	Plno;
	//	MgVect3		v1, v2;
	//
	//	v1 = pt[1] - pt[0];
	//	v2 = pt[2] - pt[0];
	//
	//	Plno.v = UnitizeV3( v1 ^ v2, i_Tol);
	//	Plno.d = -(Plno.v * pt[0]);
	//	return Plno;
	//}

	////===========================================================================
	////		３点より平面の方程式の係数と定数を求める（正規化）
	////
	//static inline MgPlane3 Plane3C3Pt(			// (  O) 平面式
	//				const	MgPoint3&	p0,			// (I  ) 点１
	//				const	MgPoint3&	p1,			// (I  ) 点２
	//				const	MgPoint3&	p2,			// (I  ) 点３
	//						MREAL i_Tol = g_gTol.D	// (I  ) 許容誤差
	//				)
	//{
	//	MgPlane3	Plno;
	//	MgVect3		v1, v2;
	//
	//	v1 = p1 - p0;
	//	v2 = p2 - p0;
	//
	//	Plno.v = UnitizeV3( v1 ^ v2, i_Tol);
	//	Plno.d = -(Plno.v * p0);
	//	return Plno;
	//}
	
	////===========================================================================
	////		多角形より平面の方程式の係数と定数を求める（正規化）
	////													pkm	面積の絶対値の合計で求め方向は実面積で求める方が良いか pkm
	//static MgPlane3 Plane3CPg(					// (  O) 平面式
	//				const	MgPolyg3&	pg			// (I  ) 多角形
	//				);
	
	////===========================================================================
	////		２次元始点と２次元終点の２点、勾配とＺ座標より平面の方程式の係数と定数を求める（正規化）
	////
	//static MgPlane3 Plane3C2Pt2F(
	//				const	MgPoint2&	p1,			// (I  ) ２次元始点
	//				const	MgPoint2&	p2,			// (I  ) ２次元終点
	//						MREAL		rkb,		// (I  ) 勾配	(始点から終点に向かい左方向=0 tan)
	//						MREAL		z			// (I  ) Ｚ座標
	//				);
	
	////===========================================================================
	////		２次元線分、勾配とＺ座標より平面の方程式の係数と定数を求める（正規化）
	////
	//static MgPlane3 Plane3CLn2F(
	//				const	MgLine2&	ln1,		// (I  ) ２次元線分
	//						MREAL		rkb,		// (I  ) 勾配	(線分の始点から終点に向かい左方向=0 tan)
	//						MREAL		z			// (I  ) Ｚ座標
	//				);
	
	////===========================================================================
	////		平面の方程式の係数と定数を正規化した(係数a,b,cを単位ベクトルに変換した)平面を求める
	////
	//static MgPlane3 NormPln(
	//				const	MgPlane3&	i_pl,
	//						MREAL		i_Tol
	//				)
	//{
	//	MREAL	d1 = Abs( i_pl.v);
	//		MBCHECK_ZEROVECT( d1, MBCstr( "MgPlane3 Norm"), i_Tol);
	//	MREAL	d2 = 1.f / d1;
	//	return Plane3( i_pl.v.x * d2, i_pl.v.y * d2, i_pl.v.z * d2, i_pl.d * d2);
	//}

};

//	;必要であるが無し　MgChkLn2OnLn2WS

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
inline MgMat2 operator + ( const MgMat2& i_tm, const MgMat2& i_tn)			// +
{
	return MGeo::MatPlus2( i_tm, i_tn);
}

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
inline	MgMat2 operator += ( MgMat2& io_tm, const MgMat2& i_tn)				// +=
{
	return MGeo::MatPlusEqual2( io_tm, i_tn);
}

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
inline	MgMat2 operator - ( const MgMat2& i_tm)								// -
{
	return MGeo::MatSingleMinus2( i_tm);
}

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
inline	MgMat2 operator - ( const MgMat2& i_tm, const MgMat2& i_tn)			// -
{
	return MGeo::MatMinus2( i_tm, i_tn);
}

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
inline	MgMat2 operator -= ( MgMat2& io_tm, const MgMat2& i_tn)				// -=
{
	return MGeo::MatMinusEqual2( io_tm, i_tn);
}

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
inline	MgMat2 operator * ( const MgMat2& i_tm, const MgMat2& i_tn)			// *
{
	return MGeo::MatMult2( i_tm, i_tn);
}

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
inline	MgMat2 operator *= ( MgMat2& io_tm, const MgMat2& i_tn)				// *=
{
	return MGeo::MatMultEqual2( io_tm, i_tn);
}

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
inline	MgPoint2 operator * ( const MgPoint2& i_pt, const MgMat2& i_tm)		// *	座標変換
{
	return MGeo::MatMultPt22( i_pt, i_tm);
}

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
inline	MgPoint2 operator *= ( MgPoint2& io_pt, const MgMat2& i_tm)			// *=	座標変換
{
	return MGeo::MatMultEqualPt22( io_pt, i_tm);
}

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
inline	MgULine2 operator * ( const MgULine2& i_uln, const MgMat2& i_tm)	// *	座標変換
{
	return MGeo::MatMultULn22( i_uln, i_tm);
}

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
inline	MgULine2 operator *= ( MgULine2& io_uln, const MgMat2& i_tm)		// *=	座標変換
{
	return MGeo::MatMultEqualULn22( io_uln, i_tm);
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
inline	MgPoint3 operator * ( const MgPoint3& i_Pt, const MgMat2& i_tm)		// *	座標変換
{
	return MGeo::MatMultPt32( i_Pt, i_tm);
}

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
//
inline	MgPoint3 operator *= ( MgPoint3& io_Pt, const MgMat2& i_tm)			// *=	座標変換

{
	return MGeo::MatMultEqualPt32( io_Pt, i_tm);
}

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
inline MgULine3 operator * ( const MgULine3& i_ULn, const MgMat2& i_tm)		// *	座標変換
{
	return MGeo::MultULn32( i_ULn, i_tm);
}

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
inline MgULine3 operator *= ( MgULine3& io_ULn, const MgMat2& i_tm)			// *=	座標変換
{
	return MGeo::MatMultEqualULn32( io_ULn, i_tm);
}

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
inline	MgMat3 operator + ( const MgMat3& i_Tm, const MgMat3& i_Tn)			// +
{
	return MGeo::MatPlus3( i_Tm, i_Tn);
}

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
inline	MgMat3 operator += ( MgMat3& io_Tm, const MgMat3& i_Tn)				// +=
{
	return MGeo::MatPlusEqual3( io_Tm, i_Tn);
}	

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
inline	MgMat3 operator - ( const MgMat3& i_Tm)								// -
{
	return MGeo::MatSingleMinus3( i_Tm);
}

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
inline	MgMat3 operator - ( const MgMat3& i_Tm, const MgMat3& i_Tn)			// -
{
	return MGeo::MatMinus3( i_Tm, i_Tn);
}

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
inline MgMat3 operator -= ( MgMat3& io_Tm, const MgMat3& i_Tn)				// -=
{
	return MGeo::MatMinusEqual3( io_Tm, i_Tn);
}

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
inline MgMat3 operator * ( const MgMat3& i_Tm, const MgMat3& i_Tn)			// *
{
	return MGeo::MatMult3( i_Tm, i_Tn);
}

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
inline MgMat3 operator *= ( MgMat3& io_Tm, const MgMat3& i_Tn)
{
	return MGeo::MatMultEqual3( io_Tm, i_Tn);
}

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
inline MgPoint2 operator * ( const MgPoint2& i_Pt, const MgMat3& i_Tm)		// 座標変換
{
	return MGeo::MatMultPt23( i_Pt, i_Tm);
}

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
inline MgPoint2 operator *= ( MgPoint2& io_Pt, const MgMat3& i_Tm)			// 座標変換
{
	return MGeo::MatMultEqualPt23( io_Pt, i_Tm);
}

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
inline MgPoint3 operator * ( const MgPoint3& i_Pt, const MgMat3& i_Tm)		// 座標変換
{
	return MGeo::MatMultPt33( i_Pt, i_Tm);
}

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
inline MgPoint3 operator *= ( MgPoint3& io_Pt, const MgMat3& i_Tm)			// 座標変換
{
	return MGeo::MatMultEqualPt33( io_Pt, i_Tm);
}

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
inline MgULine3 operator * ( const MgULine3& i_ULn, const MgMat3& i_Tm)		// 座標変換
{
	return MGeo::MatMultULn33( i_ULn, i_Tm);
}

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
inline MgULine3 operator *= ( MgULine3& io_ULn, const MgMat3& i_Tm)			// 座標変換
{
	return MGeo::MatMultEqualULn33( io_ULn, i_Tm);
}


//===========================================================================
//		平面 形状演算
//

//S inline MgPlane3 MgPlane3C( MREAL pla, MREAL plb, MREAL plc, MREAL pld)
//						{ MgPlane3 Vto; Vto.v.x = pla; Vto.v.y = plb;
//										Vto.v.z = plc; Vto.d = pld; return Vto;}
//inline MgPlane3 MgPlane3C( const MgVect3& v1, MREAL d1)
//						{ MgPlane3 Vto; Vto.v.x = v1.x;	Vto.v.y = v1.y;
//										Vto.v.z = v1.z;	Vto.d = d1; return Vto;}]

inline MgPlane3 MgPlane3C( const MgPoint3 *pt)						// ３点より平面の方程式の係数と定数を求める（正規化）
						{ return MGeo::Plane3CPPt( pt);}

inline MgPlane3 MgPlane3C( const MgPoint3& p0, const MgPoint3& p1,	// ３点より平面の方程式の係数と定数を求める（正規化）
						   const MgPoint3& p2)
						{ return MGeo::Plane3C3Pt( p0, p1, p2);}

inline MgPlane3 MgPlane3C( const MgPolyg3 &pg)						// 多角形より平面の方程式の係数と定数を求める（正規化）
						{ return MGeo::Plane3CPg( pg);}

inline MgPlane3 MgPlane3C( const MgPoint2& p1, const MgPoint2& p2,	// 始点と終点の２点と勾配より平面の方程式の係数と定数を求める（正規化）
						 MREAL rkb, MREAL z = 0.)
						{ return MGeo::Plane3C2Pt2F( p1, p2,	rkb, z);}

inline MgPlane3 MgPlane3C( const MgLine2& ln1,						// 線分と勾配より平面の方程式の係数と定数を求める（正規化）
						MREAL rkb, MREAL z = 0.)
						{ return MGeo::Plane3CLn2F( ln1, rkb, z);}
//
//===========================================================================
//		平面の方程式の係数と定数を正規化した平面を求める
//		ax+by+cz+d=0	(a*a+b*b+c*c=1.0)
//
inline MgPlane3	MgNorm(							// (  O) 正規化後の３次元平面
				const	MgPlane3	&pl)		// (I  ) ３次元平面
{ return MGeo::NormPln( pl);}
//
//===========================================================================
//		MgPlane3を使用のインライン関数
//	
//======================( ３次元 )==============================
//		３次元ベクトル演算
inline MgVect3 MgVect3C( const MgVect2& v1, const MgPlane3& Pln)							// ２次元→３次元
						{ MgVect3 Vto; Vto.x = v1.x; Vto.y = v1.y;
									   Vto.z = Pln.GetZ( v1); return Vto;} 
inline MgVect3 MgVect3::Set( const MgVect2& v1, const MgPlane3& Pln)						// ２次元→３次元
						{ x = v1.x; y = v1.y; z = Pln.GetZ( v1); return *this;} 

inline MgULine3 MgULine3C( const MgULine2& uln, const MgPlane3& Pln, MREAL i_Tol = g_gTol.D)				// ２次元→３次元
						{ MgULine3 ULno; ULno.p.x = uln.p.x; ULno.p.y = uln.p.y;
										 ULno.p.z = Pln.GetZ( uln.p);
						  ULno.v = MgVect3C( uln.v, - (Pln.v.x * uln.v.x + Pln.v.y * uln.v.y) / Pln.v.z);
						  ULno.v.SetUnitize( i_Tol); return ULno;} 
inline MgULine3 MgULine3::Set( const MgULine2& uln, const MgPlane3& Pln, MREAL i_Tol)		// ２次元→３次元
						{ p.x = uln.p.x; p.y = uln.p.y; p.z = Pln.GetZ( uln.p);
						  v = MgVect3C( uln.v, - (Pln.v.x * uln.v.x + Pln.v.y * uln.v.y) / Pln.v.z);
						  v.SetUnitize( i_Tol); return *this;} 

inline MgLine3 MgLine3C( const MgLine2& ln, const MgPlane3& Pln)							// ２次元→３次元
						{ MgLine3 Lno; Lno.p[0].x = ln.p[0].x; Lno.p[0].y = ln.p[0].y;
									   Lno.p[0].z = Pln.GetZ( ln.p[0]);
									   Lno.p[1].x = ln.p[1].x; Lno.p[1].y = ln.p[1].y;
									   Lno.p[0].z = Pln.GetZ( ln.p[1]); return Lno;}
inline MgLine3 MgLine3::Set( const MgLine2& ln, const MgPlane3& Pln)						// ２次元→３次元
						{ p[0].x = ln.p[0].x; p[0].y = ln.p[0].y; p[0].z = Pln.GetZ( ln.p[0]);
						  p[1].x = ln.p[1].x; p[1].y = ln.p[1].y; p[0].z = Pln.GetZ( ln.p[1]);
						  return *this;}

inline MINT SZMgPlane3( MINT i_sz = 1)	{ return  ( i_sz * (MINT)sizeof( MgPlane3));}

inline MINT WSZMgPlane3( MINT i_sz = 1)	{ return  ( i_sz * (MINT)sizeof( MgPlane3) / SZMINT());}

} // namespace MC
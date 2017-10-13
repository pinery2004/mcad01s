#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: .h
//
//		
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "MgDefine.h"

//===========================================================================
//		図形演算 許容誤差

#include "MbPlex.h"

#ifdef DLL_EXPORT_TOL_DT
	#undef DLL_EXPORT_TOL_DT
#endif

#ifdef DLL_EXPORT_TOLERANCE_DO
	#pragma message( "<<< MgTol_Dt・dll_EXport >>>")
	#define DLL_EXPORT_TOL_DT							__declspec( dllexport)		// 他のDLLからの参照を可能にする
#else
	#ifdef DLL_NO_IMPORT_GEO_DO
//		#pragma message( "=== MgTol_Dt・dll ===")
		#define DLL_EXPORT_TOL_DT
	#else
//		#pragma message( "=== MgTol_Dt・dll_IMport ===")
		#define DLL_EXPORT_TOL_DT					__declspec( dllimport)
	#endif
#endif

#ifdef DLL_EXPORT_TOL
	#undef DLL_EXPORT_TOL
#endif

#ifdef DLL_EXPORT_TOLERANCE_DO
	#pragma message( "<<< MgTol・dll_EXport >>>")
	#define DLL_EXPORT_TOL							__declspec( dllexport)		// 他のDLLからの参照を可能にする
#else
	#ifdef _DEBUG								// Release Mode で inline関数がundefになる不具合対応
		#ifdef DLL_NO_IMPORT_GEO_DO
//			#pragma message( "=== MgTol・dll ===")
			#define DLL_EXPORT_TOL
		#else
//			#pragma message( "=== MgTol・dll_IMport ===")
			#define DLL_EXPORT_TOL						__declspec( dllimport)
		#endif
	#else
//		#pragma message( "=== MgTol_Dt・dll ===")
		#define DLL_EXPORT_TOL
	#endif
#endif

class DLL_EXPORT_TOL_DT MgTol_Dt
{
public:
//===========================================================================
//		許容誤差
//
//	static	MREAL	DN;					//	ゼロ判定用	Normarize用線分最小長さ
//	static	MREAL	ZN;					//	ゼロ判定用	Normarize用軸方向最小長さ（現在不使用）
	static	MREAL	D;					//	同一座標判定用	線分長さ		許容誤差
	static	MREAL	L;					//	同一論理座標判定用	線分長さ	許容誤差
	static	MREAL	A;					//	同一角度判定用	角度(ラジアン)	許容誤差
	static	MREAL	S;					//	同一平面判定用	平面			許容誤差
	static	MREAL	D_2;				//	同一座標判定用	線分長さ		許容誤差**2
	static	MREAL	L_2;				//	同一論理座標判定用	線分長さ	許容誤差**2
	static	MREAL	A_2;				//	同一角度判定用	角度(ラジアン)	許容誤差**2
	static	MREAL	S_2;				//	同一平面判定用	平面			許容誤差**2
};

class DLL_EXPORT_TOL MgTol: public MgTol_Dt
{
public:
	MgTol(void){}
	~MgTol(void){}

public:
	//	実数
	static MREAL MgAbs(MREAL x)
						{return ((x > 0) ? x : -x);}
	static MINT MgAbs(MINT x)
						{return ((x > 0) ? x : -x);}
	static bool MgZero(MREAL x, MREAL tol = D)
						{return (x < tol && x > -tol);}
	static bool MgEqual(MREAL x, MREAL y, MREAL tol = D)
						{return (x - y < tol && y - x < tol);}
	static bool MgEQ(MREAL x, MREAL y, MREAL tol = D)					// MgEqualと同じ
						{return (x - y < tol && y - x < tol);}
	static bool MgLT(MREAL x, MREAL y, MREAL tol = D)
						{return (x < y - tol);}
	static bool MgLE(MREAL x, MREAL y, MREAL tol = D)
						{return (x <= y + tol);}
	static bool MgGT(MREAL x, MREAL y, MREAL tol = D)
						{return (x > y + tol);}
	static bool MgGE(MREAL x, MREAL y, MREAL tol = D)
						{return (x >= y - tol);}
};

extern	MgTol_Dt	g_gTol;


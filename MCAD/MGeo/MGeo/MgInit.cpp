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

#define DLL_EXPORT_TOLERANCE_DO
#include "MgTol.h"


//
//===========================================================================
//		許容誤差
//
MgTol_Dt	g_gTol;

//	MREAL	MgTol_Dt::DN=0.01f;					//	ゼロ判定用	Normarize用線分最小長さ
//	MREAL	MgTol_Dt::ZN=0.01f;					//	ゼロ判定用	Normarize用軸方向最小長さ（現在不使用）
	MREAL	MgTol_Dt::D=0.01f;					//	同一座標判定用	線分長さ		許容誤差
	MREAL	MgTol_Dt::L=0.00001f;				//	同一論理座標判定用	線分長さ	許容誤差
	MREAL	MgTol_Dt::A=0.0001f;				//	同一角度判定用	角度(ラジアン)	許容誤差
	MREAL	MgTol_Dt::S=0.01f;					//	同一平面判定用	平面			許容誤差
	MREAL	MgTol_Dt::D_2=0.01f;				//	同一座標判定用	線分長さ		許容誤差**2
	MREAL	MgTol_Dt::A_2=0.0000001f;			//	同一角度判定用	角度(ラジアン)	許容誤差**2
	MREAL	MgTol_Dt::S_2=0.0001f;				//	同一平面判定用	平面			許容誤差**2
	MREAL	MgTol_Dt::L_2=0.000000001f;			//	同一論理座標判定用	線分長さ	許容誤差**2

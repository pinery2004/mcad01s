//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgRect.cpp
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"
#include "MbDebug.h"

#define	DLL_EXPORT_RECT_DO

#include "MgDefine.h"
#include "MgTol.h"
#include "MgAngle.h"
#include "MgPoint.h"
#include "MgLine.h"
#include "MgRect.h"

namespace MC
{

//
//static MgMinMaxI2	McMinMaxIN	= MgMinMaxI2( 0, 0, 1, 1);
//static MgMinMaxR2	McMinMaxRN	= MgMinMaxR2( 0.0f, 0.0f, 1.0f, 1.0f);
//static MgMinMaxR3 McMinMaxR3N = MgMinMaxR3( 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
//static MgRect2	McRectN		= MgRect2( 0.0f, 0.0f, 1.0f, 1.0f);
//static MgRect3	McRect3N	= MgRect3( 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
//
//======================( ２次元 )==============================
//		ＭＩＮ／ＭＡＸに単位長方形を設定する
//
void MgMinMaxI2::SetUnit()	{*this = MgMinMaxI2( 0, 0, 1, 1);}
void MgMinMaxI2::SetInit()	{*this = MgMinMaxI2( INT_MAX, INT_MAX, INT_MIN, INT_MIN);}
//
void MgMinMaxR2::SetUnit()	{*this = MgMinMaxR2( 0.0f, 0.0f, 1.0f, 1.0f);}
void MgMinMaxR2::SetInit()	{*this = MgMinMaxR2( MREALMAX, MREALMAX, MREALMIN, MREALMIN);}
//
//======================( ３次元 )==============================
//		ＭＩＮ／ＭＡＸに単位直方体を設定する
//
void MgMinMaxR3::SetUnit()	{*this = MgMinMaxR3( 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);}
void MgMinMaxR3::SetInit()	{*this = MgMinMaxR3( MREALMAX, MREALMAX, MREALMAX, MREALMIN, MREALMIN, MREALMIN);}
//
//======================( ２次元 )==============================
//		単位長方形を設定する
//
void MgRect2::SetUnit()		{*this = MgRect2( 0.0f, 0.0f, 1.0f, 1.0f);}
//
//======================( ２次元 )==============================
//		長方形をトレースする
//
void MgRect2::Print(MCHAR* s)														// print
{
//#ifdef _DEBUG
//	Msprintf( MbLog::m_Str, Mstr( "%s	MgRect2	= %f, %f, %f, %f\n"),
//							s, p[0].x, p[0].y, p[1].x, p[1].y);
//	MBTRCPRBF;
//	MbLog::Trace( MbLog::m_Str);
//#endif
}
//
//======================( ３次元 )==============================
//		単位直方体を設定する
//
void MgRect3::SetUnit()		{*this = MgRect3( 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);}
//
//======================( ３次元 )==============================
//		直方体をトレースする
//
void MgRect3::Print(MCHAR* s)														// print
{
//#ifdef _DEBUG
//	Msprintf( MbLog::m_Str, Mstr( "%s	MgRect2	= %f, %f, %f, %f, %f, %f\n"),
//							s, p[0].x, p[0].y, p[0].z, 
//							   p[1].x, p[1].y, p[1].z);
//	MBTRCPRBF;
//	MbLog::Trace( MbLog::m_Str);
//#endif
}
//

} // namespace MC
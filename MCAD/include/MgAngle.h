#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgAngle.h
//
//		角度
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
//
#include "MgDefine.h"
#include "MgTol.h"
#include "MbDebug.h"

namespace MC
{

/////////////////////////////////////////////////////////////////////////////
//  角度　（ラジアンで表現）
struct MgAngleR{
	MREAL a;
public:
	MgAngleR() 			{}
//	MgAngleR() {a=0.;}
	MgAngleR(MREAL a)
						{
							MgAngleR::a = a;
						}
	void Print(MCHAR* s)														// Print
						{ 
#ifdef _DEBUG
							Msprintf( MbLog::m_Str, Mstr( "MgAngleR   %s	= %f\n"), s, a);
							MBTRCPRBF;
#endif
						}
};

/////////////////////////////////////////////////////////////////////////////
//	角度　（単位ベクトルで表現）
class MgAngleV
{
public:
	MREAL x;
	MREAL y;
//
	MgAngleV() 			{}
	MgAngleV(MREAL a)
						{
							x = (MREAL)cos(a);
							y = (MREAL)sin(a);
						}
	void Print(MCHAR* s)														// Print
				 		{
#ifdef _DEBUG
							Msprintf( MbLog::m_Str, Mstr( "MgAngleV   %s = (%f, %f)\n"), s, x, y);
							MBTRCPRBF;
#endif
						}
};

} // namespace MC
#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgAngle.h
//
//		�p�x
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
//  �p�x�@�i���W�A���ŕ\���j
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
//	�p�x�@�i�P�ʃx�N�g���ŕ\���j
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
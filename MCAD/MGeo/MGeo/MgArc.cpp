//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgArc.cpp
//
//		
//
//
//  K.Matsu           09/20/07    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"
#include "MbDebug.h"

#define	DLL_EXPORT_ARC_DO

#include "MgLib.h"
#include "MgArc.h"

namespace MC
{

//
//======================( ÇQéüå≥ )==============================
//		ÇQéüå≥â~å ÇÉgÉåÅ[ÉXÇ∑ÇÈ
//
void MgArc2::Print(const MCHAR* s) const
{
#ifdef _DEBUG
	Msprintf( MbLog::m_Str, Mstr( "%s	MgArc2	: (%7.1f,%7.1f), (%7.1f,%7.1f), (%7.1f,%7.1f), %7.1f\n"),
								 s, pt[0].x, pt[0].y, pt[1].x, pt[1].y, pc.x, pc.y, r);
	MBTRCPRBF;
#endif
}

//
//======================( ÇRéüå≥ )==============================
//		ÇRéüå≥â~å ÇÉgÉåÅ[ÉXÇ∑ÇÈ
//
void MgArc3::Print(const MCHAR* s) const
{
#ifdef _DEBUG
	Msprintf( MbLog::m_Str, Mstr( "%s 	MgArc3 	: (%7.1f,%7.1f,%7.1f), (%7.1f,%7.1f,%7.1f), (%7.1f,%7.1f,%7.1f), (%7.1f,%7.1f,%7.1f), %7.1f\n"),
								 s,  Pt[0].x, Pt[0].y, Pt[0].z, Pt[1].x, Pt[1].y, Pt[1].z, Pc.x, Pc.y, Pc.z, V.x, V.y, V.z, r);
	MBTRCPRBF;
#endif
}

} // namespace MC

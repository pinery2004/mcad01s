//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: Mgs.cpp
//
//		èàóù
//
//
//  K.Matsu			10/07/07	Created.
//==========================================================================================
//
/**
 *\file		point.cpp
 *
 *\brief	
 *
 *\author	Xianming Chen\n
 *		Computer Science Department\n
 *		University of Utah
 *
 *\date		20 Aug 2006\n
 *		Copyright (c) 2006, University of Utah
 */

#include "stdafx.h"
#include "MgsPoint.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace euler
{

ostream& operator << (ostream& os, MgsPoint const& pt)
{
	float pto[4];
	float pti[4];
	for ( int iC=0; iC<4; iC++) {
		pti[iC] = pt[iC];
		pto[iC] = ( pti[iC] < 0.00001f ? 0.f: pti[iC]);
	}
	return os << "(" << setw(6) << pto[0] << ", " <<  setprecision( 2) << pto[1] << ", " <<	setw( 5) << pto[2] << ", " << pto[3] << ")";
}



}//end namespace euler


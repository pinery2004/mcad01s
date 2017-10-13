#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgsFace.h
//
//		ñ ï™ÇÃíËã`ÅAï“èW
//
//
//  K.Matsu			10/07/07	Created.
//==========================================================================================
//
/**
 *\file		MgsPrint.h
 *
 *\brief
 *
 *\author	Xianming Chen\n
 *		Computer Science Department\n
 *		University of Utah
 *
 *\date		14 Aug 2006\n
 *		Copyright (c) 2006, University of Utah
 */

#include "MgsEuler.h"
#include <iostream>
using namespace std;


namespace euler
{


ostream& operator << (ostream& os, MgSolid const&);
ostream& operator << (ostream& os, MgsFace const&);
ostream& operator << (ostream& os, MgsLoop const&);
ostream& operator << (ostream& os, MgsEdge const&);
ostream& operator << (ostream& os, MgsVertex const&);

inline ostream& operator << (ostream& os, MgSolid const* sol)
{
	return os << *sol;
}

inline ostream& operator << (ostream& os, MgsFace const* fce)
{
	return os << *fce;
}

inline ostream& operator << (ostream& os, MgsLoop const* lop)
{
	return os << *lop;
}

inline ostream& operator << (ostream& os, MgsEdge const* edg)
{
	return os << *edg;
}

inline ostream& operator << (ostream& os, MgsVertex const* vtx)
{
	return os << *vtx;
}


}//end namespace euler


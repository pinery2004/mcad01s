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
 *\file		kfmrh.cpp
 *
 *\brief	connected sum
 *
 *              \em kfmrh(f1, f2) kill the second face and make its boundary as a ring (inner loop) of the first face, 
 *              to either make a hole if both faces are in the same shell, or to kill a shell otherwise.
 *
 *\image        html kfmrh.jpg
 *\image        html kfsmr.jpg 
 *
 *\author	Xianming Chen\n
 *		Computer Science Department\n
 *		University of Utah
 *
 *\date		16 Aug 2006\n
 *		Copyright (c) 2006, University of Utah
 */
#include "stdafx.h"

#include <cassert>
#include "MgsEuler.h"

namespace euler
{



void MgSolid::kfmrh(MgsFace* f1, MgsFace* f2)
{
	assert(! f2->m_lop->m_nxt);

	add_MgsFaceo(f2, f1);

	MgSolid::assert_consistent_B_rep();
}




}//end namespace euler


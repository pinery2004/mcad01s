//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: disk.cpp
//
//		ˆ—	‰~”Õ‚ğì¬‚·‚é
//
//
//  K.Matsu			10/07/07	Created.
//==========================================================================================
//
#include "stdafx.h"

#include "MgsEuler.h"
#include "MgsPrint.h"
#include <cmath>

// ‰~”Õ‚ğì¬‚·‚é
// w’è‚³‚ê‚½@’†SA”¼ŒaA•ªŠ„”‚Ì‘½ŠpŒ`‚Ì—Åü‚ğ‚Â‰~‚ğ•\‚·‚Q‚Â‚Ì–Ê•ª‚ğì¬‚·‚é
//
namespace euler
{

MgSolid* MgSolid::disk( MgsPoint const& cntr, float r, int segs)
{
	MgsEdge*		edg1st;
	MgSolid*	solAdd;
	MgsEdge*		edgAdd;
	MgsPoint	pt( cntr[0] + r, cntr[1], cntr[2]);

//	solAdd = MgSolid::mvfs( MgsPoint( cntr[0] + r, cntr[1], cntr[2]) );

	solAdd = MgSolid::mvfs( pt);

	edg1st = solAdd->m_fce->m_lop->m_edg;

	edgAdd = solAdd->m_fce->arc( edg1st, cntr, r, 0, ( 2 * pi * (segs - 1)) / segs, segs - 1);

	solAdd->mef( edgAdd, solAdd->m_fce->m_lop->m_edg);

	return solAdd;
}


}//end namespace euler


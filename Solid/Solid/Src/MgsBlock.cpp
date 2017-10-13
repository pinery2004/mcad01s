//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgsBlock.cpp
//
//		ˆ—	’¼•û‘Ì‚ğì¬‚·‚é
//
//
//  K.Matsu			10/07/07	Created.
//==========================================================================================
//
#include "stdafx.h"
#include "MgsEuler.h"
#include "MgsPrint.h"
#include <cmath>


namespace euler
{

// ’¼•û‘Ì‚ğì¬‚·‚é	( 0., 0., 0.) - ( pt[0], pt[1], pt[2])
//
MgSolid* MgSolid::block( MgsPoint const& pt)
{
	MgSolid* solAdd;
	MgsLoop* lopAdd;
	MgsEdge* edgAdd;
	MgsFace* fceAdd;

	solAdd = MgSolid::mvfs( MgsPoint());
	lopAdd = solAdd->m_fce->m_lop;	// to 1

	edgAdd = lopAdd->m_edg;	   // MgsPoint( 0.0,   0.0,	0.0)
	edgAdd = lopAdd->mev( edgAdd, MgsPoint( pt[0], 0.0,	0.0) );
	lopAdd->del_single_vertex_edge_pair();	// to 1

	edgAdd = lopAdd->mev( edgAdd, MgsPoint( pt[0], pt[1], 0.0) );
	edgAdd = lopAdd->mev( edgAdd, MgsPoint( 0.0, pt[1],	0.0) );

	// from 1 lopAdd->del_single_vertex_edge_pair(); 
	fceAdd = solAdd->mef( edgAdd, lopAdd->m_edg)->m_fce;

	solAdd->tsweep_face( fceAdd, MgsVect(0., 0., pt[2]) );

	return solAdd;
}



}//end namespace euler


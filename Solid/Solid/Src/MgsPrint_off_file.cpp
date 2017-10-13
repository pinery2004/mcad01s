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
 *\file		print_off_file.cpp
 *
 *\brief	
 *
 *\author	Xianming Chen\n
 *		Computer Science Department\n
 *		University of Utah
 *
 *\date		18 Aug 2006\n
 *		Copyright (c) 2006, University of Utah
 */
#include "stdafx.h"
#include "MgsPrint.h"
#include "MgsEuler.h"
#include <iostream>
using namespace std;


namespace euler
{


void MgSolid::print_off_file(std::ostream& os) const

{
	os << "<Solid>\n";
	int nvertices = 0, nfaces = 0;

	for(MgsVertex* vtx = m_vtxfirst; vtx; vtx=vtx->m_nxt)
		nvertices++;
	for(MgsFace* fac = m_fce; fac; fac=fac->m_nxt)
		nfaces++;
    os << " Vertexêî = " << nvertices << " Faceêî = " << nfaces << ' ' << 0 << endl;

	for(MgsVertex* vtx = m_vtxfirst; vtx; vtx=vtx->m_nxt)
		os << "VertexÇÃç¿ïW = (" << vtx->m_pt[0] << ",\t" << vtx->m_pt[1] << ",\t"
		   << vtx->m_pt[2] << ",\t" << vtx->m_pt[3] << ")\t ID = #" << vtx->m_id << endl;
	os << endl << endl;

	for(MgsFace* fac = m_fce; fac; fac=fac->m_nxt) {
		int nvertices = fac->m_lop->No_of_Vertex();
		os << " FaseÇÃVertexêî = " << nvertices << '\t';
		MgsEdge *edg = fac->m_lop->m_edg;
		MgsEdge *edg1st = edg;
		do {
			os << " edgeÇÃVertexID = " << edg->m_vtx->m_id << ' ';
		} while( (edg = edg->m_nxt) != edg1st );

		os << endl;
	}

	os << endl;
}


}//end namespace euler


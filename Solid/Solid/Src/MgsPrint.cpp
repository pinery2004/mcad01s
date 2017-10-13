//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgsPrint.cpp
//
//		トレース出力
//
//
//  K.Matsu			10/07/07	Created.
//==========================================================================================
//
/**
 *\file		print.cpp
 *
 *\brief	
 *
 *\author	Xianming Chen\n
 *		Computer Science Department\n
 *		University of Utah
 *
 *\date		 16 Aug 2006\n
 *		Copyright (c) 2006, University of Utah
 */

#include "stdafx.h"
#include "MgsEuler.h"
#include "MgsPrint.h"

namespace euler
{

//	ソリッドのトレース出力
//
//	・ソリッドの全面分をトレース出力する
//
ostream& operator << (
				ostream&	os,
		const	MgSolid& 	sol
		)
{
	MgsFace *fce;
	
	os << "1. solid = " << hex << (int*)&sol << " (" << sol.m_id << ")";
	for( fce = sol.m_fce; fce; fce = fce->m_nxt) {
		os << "\n\t    " << fce;
	}
	return os;
}

//	面分のトレース出力
//
//	・面分のIDと全ループをトレース出力する
//
ostream& operator << (
				ostream&	os,
		const	MgsFace&	fce
		)
{
	const MgsLoop* lop;
	
	os << "2. face = " << hex << (int*)&fce << " (" << fce.m_id << ")";
	for( lop = fce.m_lop; lop; lop = lop->m_nxt) {
		os << "\n\t      " << lop;
	}
	return os;
}

//	ループのトレース出力
//
//	・ループの全半稜線をトレース出力する
//
ostream& operator << (
				ostream&	os,
		const	MgsLoop&	lop
		)
{
	const MgsEdge* edg;
	
	os << "3. loop = " << hex << (int*)&lop;
	edg = lop.m_edg; 
	do {
		os << "\n\t\t" << *edg;
	} while( (edg = edg->m_nxt) != lop.m_edg );
	os << endl;
	return os;
}

//	半稜線のトレース出力
//
//	・半稜線の頂点IDとペアの半稜線を持つならその頂点IDをトレース出力する
//	・半稜線の頂点とペアの半稜線の頂点が等しいなら、半稜線のループの面分IDをトレース出力する
//	・半稜線の頂点座標をトレース出力する
//	・ペアの半稜線を持つならその頂点座標をトレース出力する
//
ostream& operator << (
				ostream&	os,
		const	MgsEdge&		edg
		)
{
	os << "4. half edge( vtxID, vtxOtrID) = " << hex << (int*)&edg << " (" << edg.m_vtx->m_id;
	if( edg.m_otr)
		os  << ", " << edg.m_otr->m_vtx->m_id;

	os << ")";

	if( edg.m_otr && edg.m_vtx == edg.m_otr->m_vtx)
	os << " [face = (" << edg.m_lop->m_fce->m_id << ") ]";

	os << "\t edgVtx = " << edg.m_vtx->m_pt;

	if(edg.m_otr)
		os << "\t edgOtrVtx = "<< edg.m_otr->m_vtx->m_pt;

	return os;
}

//	頂点のトレース出力
//
//	・頂点IDと座標をトレース出力する
//
ostream& operator << (
				ostream&	os,
		const	MgsVertex&	vtx
		)
{
	return os << "5. vtx = (" << vtx.m_id << ')\t' << vtx.m_pt;
}


}//end namespace euler


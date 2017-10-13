//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: translation_sweep.cpp
//
//		処理
//
//
//  K.Matsu			10/07/07	Created.
//==========================================================================================
//
/**
 *\file		translation_sweep.cpp
 *
 *\brief	translational sweep
 *
 *\author	Xianming Chen\n
 *		Computer Science Department\n
 *		University of Utah
 *
 *\date		17 Aug 2006\n
 *		Copyright (c) 2006, University of Utah
 */
#include "stdafx.h"

#include "MgsEuler.h"
#include "MgsPrint.h"
#include <cmath>


namespace euler
{


  
void MgSolid::tsweep_face( MgsFace* fce, MgsVect const& vSwp)
{
	MgsLoop*	lopC;
	MgsEdge*	edgC;
	MgsEdge*	edg1st;
	MgsEdge*	edgNxt;
	MgsEdge* edgEnd;
	MgsEdge*	edgSwp;
	MgsPoint	ptSwp;	

	for( lopC = fce->m_lop; lopC; lopC = lopC->m_nxt) {

		// スイープ側面の稜線を作成する
		edg1st = lopC->m_edg;
		edgC = edg1st;
		do {
			ptSwp = edgC->m_vtx->m_pt + vSwp;
			edgSwp = lopC->mev( edgC, ptSwp);
			edgC = edgSwp->m_pre->m_pre;
		} while( edgC != edg1st);

		// スイープ側面の面分を作成する
		edgC = edg1st->m_pre;
		edgNxt = edgC->m_nxt->m_nxt->m_nxt;
		edgEnd = mef( edgC, edgNxt)->m_edg->m_otr;
		while( ( edgC = edgNxt) != edgEnd ) {
			edgNxt = edgC->m_nxt->m_nxt->m_nxt;
			mef( edgC, edgNxt);
		}
	}
}



}//end namespace euler


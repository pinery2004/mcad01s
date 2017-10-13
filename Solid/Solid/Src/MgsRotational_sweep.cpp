//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgsRotational_sweep.cpp
//
//		処理
//
//
//  K.Matsu			10/07/07	Created.
//==========================================================================================
//
/**
 *\file		rotational_sweep.cpp
 *
 *\brief	wire and face rotational sweeps.
 *
 *              the first has its topological transformation demonstrated by the following picture.
 *
 *\image        html rsweep.jpg
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
#include <vector>


namespace euler
{


/**
  * Rotational sweep of a *wire*
  *
  * assume given loop is a wire on some face, and half edge 
  * of the first vertex is on the half edge list head.
  * the wire, on the half plane y>0. is to be rotated around x-axis.
  */
//
//
void MgSolid::rsweep_wire( MgsLoop* lop, int resolution)

{
	int	nVtx;
	MgsEdge*	edg;
	MgsEdge*	e0;
	MgsEdge*	e00;
	float theta;
	float y;
	float z;
	MgsEdge*	e1;
	MgsEdge*	e2;
	MgsEdge*	nxt_e;
	MgsEdge*	nxt_e1;
	int iSeg;
	int iC;

	nVtx = lop->No_of_Vertex() / 2 + 1;
	edg = lop->m_edg;
	e0 = edg;
	e00 = e0; 

	std::vector<float> r;
	theta = 0.0;

	for( iSeg=0; iSeg < resolution - 1; iSeg++) {
		theta += 2 * pi / resolution;
		edg = e0;
		for( iC=0; iC < nVtx; iC++) {
			if( r.size() <= (unsigned)iC)
				r.push_back( edg->m_vtx->m_pt[1]);
			y = r[iC] * cos( theta);
			z = r[iC] * sin( theta);
			edg = lop->mev( edg, MgsPoint( edg->m_vtx->m_pt[0], y, z))->m_nxt->m_nxt;
		}
		edg = e0->m_pre;

		for( iC=0; iC < nVtx-1; iC++) {
			nxt_e = edg->m_nxt->m_nxt->m_nxt;
			mef( edg, nxt_e);
			edg = nxt_e;
		}
		assert_consistent_B_rep();
		e0 = e0->m_pre->m_pre->m_otr;
	}
	e1 = e00->m_otr->m_nxt;
	e2 = e0;

	for( iC=0; iC < nVtx-1; iC++) {
		nxt_e1 = e1->m_pre;
		mef(e1, e2);
		e1 = nxt_e1;
		e2 = e2->m_nxt;
	}
	mef(e2, e1);
}

//	XY平面上の面分図形をＸ軸を中心に回転した形状を求める
//	始角から終角まで指定分割した回転体を作成する
//	360゜の回転を行う場合は、後処理でkffmhを使用して2つの端の面分を削除して結合する必要がある
//
void MgSolid::rsweep_face(
						MgsFace*	fce,		//	回転元の面分
						float		theta1,		//	始角
						float		theta2,		//	終角
						int			resolution	//	始角から終角までの分割数
				)
{

	float step;
	float theta;

	std::vector<float> r;

	float y, z;

	int iC;
	MgsEdge* edgC;
	MgsEdge* edg1st;
	MgsEdge* edgNxt;
	MgsEdge* edgEnd;

	MgsLoop *lop;
	int		nVtx;

	// 指定面分のループが外周のみであり穴あきでないことを確認する
	assert( !fce->m_lop->m_nxt);

	step = ( theta2 - theta1) / resolution;
	theta = theta1;

	lop = fce->m_lop; // same operation applied to l, which is changed each iteration.

	nVtx = lop->No_of_Vertex();

	for( int seg = 0; seg < resolution; seg++) {
		theta += step;
		edgC = lop->m_edg;
		edg1st = edgC;
		iC = 0;
		cout << "[sol_0]" << this << endl;
		do {
			if( r.size() < (unsigned)nVtx)	{
				r.push_back( edgC->m_vtx->m_pt[1]);
			}
			y = r[iC] * cos( theta),  
			z = r[iC] * sin( theta);
			edgC = lop->mev( edgC, MgsPoint( edgC->m_vtx->m_pt[0], y, z))->m_pre->m_pre;
			iC++;
			cout << "[edgC_1]" << edgC << endl;
		} while( edgC != edg1st);

		cout << "[sol_1]" << this << endl;

		edg1st = edgC = edgC->m_pre;
		edgNxt = edgC->m_nxt->m_nxt->m_nxt,
		edgEnd = mef( edgC, edgNxt)->m_edg->m_otr;

//		cout <<  "[sol_2]" << this << endl;

		while( ( edgC = edgNxt) != edgEnd) {
			edgNxt = edgC->m_nxt->m_nxt->m_nxt;
			mef( edgC, edgNxt);
		}

//		cout <<  "[sol_3]" << this << endl;

		//lop->set_loop_head( lop->m_edg->m_nxt);
		lop->m_edg = lop->m_edg->m_nxt;
//		cout <<  "[sol_4]" << this << endl;
	}
}



}//end namespace euler




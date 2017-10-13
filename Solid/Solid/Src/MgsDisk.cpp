//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: disk.cpp
//
//		処理	円盤を作成する
//
//
//  K.Matsu			10/07/07	Created.
//==========================================================================================
//
#include "stdafx.h"

#include "MgsEuler.h"
#include "MgsPrint.h"
#include <cmath>

// 円盤を作成する
// 指定された　中心、半径、分割数の多角形の稜線を持つ円を表す２つの面分を作成する
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


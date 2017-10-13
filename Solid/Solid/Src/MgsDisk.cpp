//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: disk.cpp
//
//		����	�~�Ղ��쐬����
//
//
//  K.Matsu			10/07/07	Created.
//==========================================================================================
//
#include "stdafx.h"

#include "MgsEuler.h"
#include "MgsPrint.h"
#include <cmath>

// �~�Ղ��쐬����
// �w�肳�ꂽ�@���S�A���a�A�������̑��p�`�̗Ő������~��\���Q�̖ʕ����쐬����
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


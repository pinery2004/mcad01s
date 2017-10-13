//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgsArc.cpp
//
//		����	XY���ʂɕ��s�ȉ~�ʂ�����킷�|���S�����쐬����
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


//	XY���ʂɕ��s�ȉ~�ʂ�����킷�|���S�����쐬����
//	�Ō�̒��_�����Ő���Ԃ�
//	( �~���쐬����ɂ́@�I���p�x�͊J�n�p�x��1������O�̊p�x��ݒ肵�A�������͉~�̕������|�P��ݒ肷��)
//
MgsEdge* MgsFace::arc(
				MgsEdge*		edg,			// �~�ʂ̎n�_��\���Ő�(���[�v�A�ʕ�)
		const	MgsPoint&	ptCnt,			// ���S�_
				float		r,				// ���a
				float		angStt,			// �J�n�p�x
				float		angEnd,			// �I���p�x�@�@�~�̏ꍇ�͂P��O
				int			nDiv			// �������@�@�@�~�̏ꍇ��n-1�@�@
				)
{
	MgsLoop*	lop;
	float	angDiv;
	float	angNSeg;

	// debug
	//cout << " �EArc edg = " << edg << "\n\t ptCnt = " << ptCnt << " r = " << r <<
	//		" angStt = " << angStt << " angEnd = " << angEnd << " nDiv = " << nDiv << endl;

	lop = edg->m_lop;
    angDiv = ( angEnd - angStt) / nDiv;

	angNSeg = angDiv;
    for(int i=0; i<nDiv; i++, angNSeg += angDiv) {
		edg = lop->mev( edg, MgsPoint( ptCnt[0] + r * cos( angNSeg),
									   ptCnt[1] + r * sin( angNSeg),
									   ptCnt[2]));
		// debug
		//cout << " �EArc edg No = " << i << this << endl;
    }
    lop->del_single_vertex_edge_pair(); 
    return edg;
}



}//end namespace euler


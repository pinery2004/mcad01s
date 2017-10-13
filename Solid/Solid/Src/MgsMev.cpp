//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgsMev.cpp
//
//		����
//			(make edge and vertex)
//
//  K.Matsu			10/07/07	Created.
//==========================================================================================
//
#include "stdafx.h"

#include "MgsEuler.h"
#include <cassert>
#include <utility>
using namespace std;


namespace euler
{


// edg1�̒��_(=edg2�̒��_)�ɏo���肷�锼�Ő����Q�̃O���[�v�ɕ������V�����Ő��Ō���
// �Eedg2����edg1.m_pre�܂ł̃O���[�v��edg1����edg2.m_pre�܂ł̃O���[�v�ɕ�����
// �Eedg1����edg2.m_pre�܂ł̃O���[�v�̊O�ɏo�锼�Ő��̒��_�͐V�����w�肵�����Wp�̒��_�ɂ���
//
MgsEdge* MgSolid::mev( MgsEdge* edg1, MgsEdge* edg2, MgsPoint const& p)
{
	MgsVertex* vtxNew;
	assert( edg1->m_vtx == edg2->m_vtx);
	vtxNew = edg1->m_lop->m_fce->m_sol->new_vertex(p);
	return mev(edg1, edg2, vtxNew); 
}

// edg1�̒��_(=edg2�̒��_)�ɏo���肷�锼�Ő����Q�̃O���[�v�ɕ������V�����Ő��Ō���
// �Eedg2����edg1.m_pre�܂ł̃O���[�v��edg1����edg2.m_pre�܂ł̃O���[�v�ɕ�����
// �Eedg1����edg2.m_pre�܂ł̃O���[�v�̊O�ɏo�锼�Ő��̒��_�͐V�����w�肵��vtx�ɂ���
// edg1�̎�O��edg2�̎�O�ŃJ�b�g���A�Ő���}������
// edg1�̗Ő�����ǉ����Ő��܂ł̒��_��vtx�ɂ���
// vtx ����edg2->m_vtx�ւ̔��Ő���Ԃ�
//
MgsEdge* MgSolid::mev( MgsEdge* edg1, MgsEdge* edg2, MgsVertex* vtx)
{
	pair<MgsEdge*, MgsEdge*> pedg;
	MgsEdge*	edgC;

	//
	//	 edg2						 edg2	   edgA2 
	//	<-----	 <-----				<-----�@  <-----   <-----
	//		   �� edg1     �@�@���@		 �@�� -----> �� edg1
	//	----->	 ----->				----->	   edgA1   ----->
	//	 edg1.m_vtx				      edg1.m_vtx     vtx
	//
	pedg = add_edge_pair( edg1->m_vtx, edg1, vtx, edg2);

	// pr.first���pr.second�܂�vtx�𒆐S�ɏ��ɉ�]���Avtx����o��S�Ă̔��Ő��̒��_��vtx�ɐݒ肷��
	for( edgC = pedg.first->m_nxt; edgC != pedg.second; edgC = edgC->m_otr->m_nxt) {
		edgC->m_vtx = vtx;
	}

	MgSolid::assert_consistent_B_rep();
	return pedg.second;
}

//
//
//
//	  �---------�			�-----------�						�-----------�
//	  | �-----�E|			| �-------�E|						| �-------�E|
//	  | |	  �b| 			| |		  �b|						| |		  �b|
//	  |	|�@e1 ��| 			| |	�@e1  ��|   eA2  vtx			| |	�@e1  ��|   eA2         eA4  vtx
//	  |	�-----				| �<-----    <-------				| �<-----    <-------	 <-------   
//	  |	e0	  ��	 ��	  �@| e0	�@�� -------> ��	 ��		| e0	�@�� -------> �� -------> ��
//	  �------>				�------->	    eA1					�------->	   eA1		   eA3	  
//		  e1.m_vtx			      e1.m_vtx						      e1.m_vtx   eA2.m_vtx
//
MgsEdge* MgsLoop::mev(MgsEdge* e1, MgsPoint const& p)
{ 
	return m_fce->m_sol->mev( e1, e1, p);
}  

MgsEdge* MgsLoop::mev(MgsEdge* e1, MgsVertex* v)              
{ 
	return m_fce->m_sol->mev( e1, e1, v);
}  




}


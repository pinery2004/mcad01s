//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: Mgs.cpp
//
//		����	�Ő��ƒ��_���폜����
//			(make edge and vertex)
//
//  K.Matsu			10/07/07	Created.
//==========================================================================================
//
#include "stdafx.h"

#include "MgsPrint.h"
#include <cassert>

namespace euler
{


//	����:	�Ő��ƒ��_���폜����
//
//	����:	�w�蔼�Ő��ƃy�A�Ő����قȂ�ʕ��ł��邱��	<Case.1>
//
void MgSolid::kev(
						MgsEdge*	edgDel		// (I  ) �폜���Ő�
				)
{
	// �����̊m�F:	�w�蔼�Ő��ƃy�A�Ő����قȂ�ʕ��ł��邱��
	assert( edgDel->m_lop->m_fce != edgDel->m_otr->m_lop->m_fce);

	return _kev( edgDel); 
} 

//	
//	����:	�Ő��ƒ��_���폜����
//
//	<Case.1>
//
//	 edgEnd	 vtx1				   edgEnd		  
//	<--------	 <---------		 <--------�@    edgDel	    <---------
//				  ----->				  vtx1 <------- vtx2 ----->
//			  ��			 ��	�@		    �� ------->  �� 
//				  <-----					   m_otr        <-----
//	-------->	 --------->		 -------->				    --------->
//		  		  edgFront		            			     edgFront
//
//
//	<Case.2>
//
//	  �---------�			�-----------�						�-----------�
//	  | �-----�E|			| �-------�E|						| �-------�E|
//	  | |	  �b| 			| |		  �b|						| |		  �b|			  edgFront
//	  |	|�@e1 ��| 			| |	�@e1  ��|   eA2  vtx			| |	�@e1  ��| edgEnd vtx1 edgDel vtx2
//	  |	�-----				| �<-----    <-------				| �<-----    <-------	 <-------   
//	  |	e0	  ��	 ��	  �@| e0	�@�� -------> ��	 ��		| e0	�@�� -------> �� -------> ��
//	  �------>				�------->	    eA1					�------->	    		  m_otr	  
//		  e1.m_vtx			      e1.m_vtx						      e1.m_vtx   eA2.m_vtx
//

void MgSolid::_kev(
						MgsEdge*	edgDel		// (I  ) �폜���Ő�
				)
{
	// �����̊m�F:	�w�蔼�Ő��ƃy�A�Ő������݂��邱��
	assert( edgDel->m_otr);

	MgsEdge* edgFront;
	MgsEdge* edgEnd;
	MgsVertex* vtx1;
	MgsVertex* vtx2;

	edgFront = edgDel->m_otr->m_nxt;
	edgEnd = edgDel->m_nxt;

	vtx1 = edgEnd->m_vtx;
	vtx2 = edgDel->m_vtx;

	del_edge_pair( edgDel);

	if( edgDel->m_otr->m_nxt != edgDel) {
		// <Case.1>
		if( edgFront->m_otr)
			// ��vtx2�𒸓_�Ƃ��锼�Ő��̒��_��vtx1�ɕύX����
			for( ; edgFront != edgEnd; edgFront = edgFront->m_otr->m_nxt)
				edgFront->m_vtx = vtx1;
		else
			// single_vertex_edge_pair�̏ꍇ�͎��̔��Ő������_��vtx1�ɕύX����
			edgFront->m_vtx = edgFront->m_nxt->m_vtx = vtx1;
	} else {
		// <case.2>
		// �폜���Ő��̒��_�������̔��Ő��Ȃ��̂��ߒ��_�ύX�̏����͂Ȃ�
	}
	// �폜���Ő��̒��_���폜����
	del_vertex(vtx2);

	MgSolid::assert_consistent_B_rep();
}

//	����:	�Ő��ƒ��_���폜����
//
//	����:	�w�蔼�Ő��ƃy�A�Ő������ꃋ�[�v�ł���	<Case.2>
//
void MgsLoop::kev(
						MgsEdge*	edgDel		// (I  ) �폜���Ő�
				)              
{ 
	//	�����̊m�F:	�w�蔼�Ő��ƃy�A�Ő������ꃋ�[�v�ł���
	assert( edgDel->m_lop == edgDel->m_otr->m_lop);

	//	�Ő��ƒ��_���폜����
	m_fce->m_sol->_kev( edgDel);
}  



}
 


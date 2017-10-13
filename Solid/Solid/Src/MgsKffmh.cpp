//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: Mgskffmh.cpp
//
//		����	
//
//
//  K.Matsu			10/07/07	Created.
//==========================================================================================
//
/**
 *\file		kffmh.cpp
 *
 *\brief	connected sum
 *
 *              \em kffmh(MgsFace* f1, MgsFace* f2) identifies and kills two faces f1 and f2,  to either make a hole if
 *              two faces are in the same shell,  or to kill a shell otherwise.
 *
 *\author	Xianming Chen\n
 *		Computer Science Department\n
 *		University of Utah
 *
 *\date		20 Aug 2006\n
 *		Copyright (c) 2006, University of Utah
 */
#include "stdafx.h"

#include "MgsEuler.h"
#include "MgsPrint.h"


namespace euler
{

	
//�@2�̕��̖ʕ�fce1�Ɩʕ�fce2���d�ˍ��킹�āA2�̕����������A�ʕ�fce1�Ɩʕ�fce2���폜����
//
void MgSolid::kffmh(
						MgsFace*	fce1,
						MgsFace*	fce2
				)
{
	//�@�����̊m�F:	�ʕ�fce1�Ɩʕ�fce2�̃��[�v�͊O���݂̂ł��邱��(�����J���Ă��Ȃ�)
	//				�ʕ�fce1�̒��_�̐��Ɩʕ�fce2�̒��_�̐�������������
	//
	assert( ! fce1->m_lop->m_nxt &&
			! fce2->m_lop->m_nxt &&
			fce1->m_lop->No_of_Vertex() == fce2->m_lop->No_of_Vertex() );
	//
	//	�ʕ�fce2_m_otr�̒��_��ʕ�1�̒��_�ɕϊ�����
	//
	//			<------------						
	//			-----------> �_	 1st(edg1->m_vtx)		
	//   edgC->m_pre->m_otr �_					2nd(edg1->m_vtx)	
	//			<------------	 <------------					1st(edg1->m_vtx)	2nd(edg1->m_vtx)
	//			------------> �� ------------> ��						 �� <------------ ��
	//				edgC	�^�b  edg2->m_nxt�@�b		   edg1->m_otr   �b	 edg2->m_pre  �`
	//			<----------- �^				�@ �b		   ----------->	�^				  �b
	//		 �� ------------>				   �b		�� <------------				�@�b
	//		 �`		edg2		 			   �b				edg1		�@			�@�b
	//		 �b				fce2->m_lop		�@ �b		�b							�@	�@�b
	//		 �b edg2->m_pre					�@ �b		�b			fce1->m_lop		�@	�@�b
	//		 �b								   �u		�u	  							  �b
	//		 �� <------------ �� <-------------��		�� ------------> �� ------------> ��
	//
	MgsEdge* edg1;
	MgsEdge* edg2;
	MgsEdge* edg20;
	MgsEdge* edgC;

	edg1 = fce1->m_lop->m_edg;
	edg2 = fce2->m_lop->m_edg;
	edg20 = edg2;

	do {
		for( edgC = edg2->m_otr; edgC != edg2->m_nxt; edgC = edgC->m_pre->m_otr) {
			edgC->m_vtx = edg1->m_vtx;
		}
	} while(
		(
		edg1 = edg1->m_nxt,
		edg2 = edg2->m_pre
		) != edg20 );

	// fce1��m_otr��fce2��m_otr���y�A�ɂ�fce1��fce2���폜����
	edg1 = fce1->m_lop->m_edg;
	edg2 = fce2->m_lop->m_edg;
	edg20 = edg2;

	do {
		edg1->m_otr->m_otr = edg2->m_otr;
		edg2->m_otr->m_otr = edg1->m_otr;
	} while(
		(
		edg1 = edg1->m_nxt,
		edg2 = edg2->m_pre
		) != edg20 );

	del_face(fce1); 
	del_face(fce2);
}



}//end namespace euler


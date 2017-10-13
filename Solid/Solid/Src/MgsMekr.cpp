//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgsMekr.cpp
//
//			�@ make edge and kill ring (inner loop)
//
//		�����@�Q�̃��[�v��Ő��Őڍ����P�̃��[�v�ɂ���
//
//  K.Matsu			10/07/07	Created.
//==========================================================================================
//

#include "stdafx.h"
#include <cassert>
#include "MgsPrint.h"


namespace euler
{

//
//	�Q�̃��[�v��Ő��Őڍ����P�̃��[�v�ɂ���
//	�Q�̗Ő��̃��[�v��Ő��̒��_�����Ԃ��Ƃɂ��P�̃��[�v�ɂ���
//
//	����: �w��Ő��̃��[�v�͈قȂ邱��
//		  �Q�ڂ̗Ő��̃��[�v�͖ʕ��̊O�����[�v�łȂ�����
//				
//		  ��----------->��		  ��----------->��			��----------->��		  ��----------->��
//		  �`			�b		�@�`			�b			�`			  �b		�@�`			�b
//		  �b			�b		�@�bedg1		�b			�b	lop1	  �b  edgAdd2 �bedg1		�b
//		  �b			�u		�@�b			�b			�b			  �u--------->�b			�b
//		  �b	lop2	��		�@��	lop1	�b	  =>	�b edg2->m_vtx��		�@��	lop1	�b
//		  �b			�b		�@�`			�b			�b			  �b<---------�`			�b
//		  �b		edg2�b		�@�b			�b			�b		  edg2�b  edgAdd1 �b			�b
//		  �b			�u		�@�b			�u			�b			  �u		�@�b			�u
//		  ��<-----------��		  ��<-----------��			��<-----------��		  ��<-----------��
//

MgsEdge* MgsFace::mekr(MgsEdge * edg1, MgsEdge * edg2)
{
	//	�����̊m�F:	�w��Ő��̃��[�v�͈قȂ邱��
	//				�Q�ڂ̗Ő��̃��[�v�͖ʕ��̊O�����[�v�łȂ�����
	assert(edg1->m_lop != edg2->m_lop && edg2->m_lop != m_lop);
	 
	MgsLoop* lop1;
	MgsLoop* lop2;

	MgsEdge* edgAdd2;
	MgsEdge* edgAdd1;
	MgsEdge* edgC;

	lop1 = edg1->m_lop;
	lop2 = edg2->m_lop;

	edgAdd2 = lop1->mev(edg1, edg2->m_vtx), 
	edgAdd1 = edgAdd2->m_otr;

	if(edg2->m_nxt != edg2) {
		// 2�̃��[�v��ڑ�����
		edgAdd1->m_nxt = edg2;
		edgAdd2->m_pre = edg2->m_pre;

		edg2->m_pre->m_nxt = edgAdd2;
		edg2->m_pre = edgAdd1;
		// edg2�̃��[�v��lop1�ɕύX����
		for( edgC = edg2; edgC!= edgAdd2; edgC=edgC->m_nxt)
			edgC->m_lop = lop1;
	}
	// lop2���폜����
	lop2->m_edg = 0;
	del_loop(lop2);

	MgSolid::assert_consistent_B_rep();

	return edgAdd1;
}



}//end namespace euler


//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgsMef.cpp
//
//		���� �Ő���ǉ����邱�Ƃɂ��ʕ��ƃ��[�v���Q�ɕ�����
//				( make edge and face)		
//
//
//  K.Matsu			10/07/07	Created.
//==========================================================================================
//
#include "stdafx.h"

#include <cassert>
#include "MgsPrint.h"


namespace euler
{

//	�Q�̗Ő��̒��_��V�����Ő��Ō��т��ʕ��ƃ��[�v��ǉ�����
//
//	<Case.1>	mef( edg1, edg2);
//
//				edg2   edg2->m_vtx									edg2  edg2->m_vtx
//		  �� <----------- �� <----------- ��				 �� <----------- �� <----------- �� 
//		  �b							  �`				 �b			    ���b	  fceNew �`
//		  �b							  �b				 �b			    �b�b         ��	 �b
//		  �b							  �b	   =>		 �b m_lop��edgA2�b�bedgA1��lopNew�b
//		  �b							  �b				 �b			    �b�b			 �b
//		  �u					edg1	  �b				 �u			    �b��	edg1	 �b
//		  �� -----------> �� -----------> ��				 �� -----------> �� -----------> �� 
//					   edg1->m_vtx										  edg1->m_vtx	
//
//
//	<Case.2>	mef( edg1);
//				
//		  �� <----------- �� <----------- ��				 �� <----------- �� <----------- �� 
//		  �b							  �`				 �b				   				 �`
//		  �b							  �b				 �b		   �---(edgA2)--�        �b  
//		  �b							  �b				 �b		   |�--(edgA1)-�|        �b  
//		  �b				  			  �b	   =>		 �b	fce	   || fceNew   ||		 �b
//		  �b				�@			  �b				 �b		   |�-->   	---�|		 �b
//		  �u					edg1	  �b				 �u		   �----   	<---� edg1	 �b
//		  �� -----------> �� -----------> ��				 �� -----------> �� -----------> �� 
//					   edg1->m_vtx										  edg1->m_vtx	
//
//
MgsLoop* MgSolid::mef(							// (  O) �V�K�ǉ����[�v
						MgsEdge*	edg1,		// (I  ) �V�K�ǉ����Ő��̒��_(�������[�v����锼�Ő�) 
						MgsEdge*	edg2		// (I  ) �V�K�ǉ����Ő��̒��_(�ǉ����[�v����锼�Ő�) 
				)
{
	pair<MgsEdge*, MgsEdge*> edgPar;

	edgPar = add_edge_pair( edg2->m_vtx, edg1, edg1->m_vtx, edg2);
	MgsEdge *&edgAdd1 = edgPar.first;
	MgsEdge *&edgAdd2 = edgPar.second;

    // �ǉ����Ő��̘A��������
	MgsEdge *edgAdd2Pre;
	edgAdd2Pre = edgAdd2->m_pre;		// case1�̏ꍇ�͔��Ő�2�̎�O�Acase2�̏ꍇ�͒ǉ����Ő�1

	// �ǉ����Ő�2�𔼗Ő�1�̎�O�ɘA��
	edgAdd2->m_pre = edgAdd1->m_pre;
	edgAdd1->m_pre->m_nxt = edgAdd2;

	// �ǉ����Ő�1�𔼗Ő�2�̎�O�ɘA��(case2�̏ꍇ�͒ǉ����Ő�1��ǉ����Ő�1�ɘA��)
	edgAdd1->m_pre = edgAdd2Pre;
	edgAdd2Pre->m_nxt = edgAdd1;
    
    // ���[�v�Ɩʕ���ǉ����֌W�t����
	MgsFace* fceNew;
	MgsLoop* lopNew;

	fceNew = new_face();
	lopNew = fceNew->new_loop();

	MgsEdge* edgC;
	edgC = edgAdd1;
	do {
		edgC->m_lop = lopNew;
	} while( (edgC = edgC->m_nxt) != edgAdd1);

    // �ǉ����Ő������ꂼ��̃��[�v�̐擪�Ɉʒu�Â���
	edgAdd1->m_lop->m_edg = edgAdd1;
	edgAdd2->m_lop->m_edg = edgAdd2;

	MgSolid::assert_consistent_B_rep();
	return lopNew;
}



}//end namespace euler


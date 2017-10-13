//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgsKemr.cpp
//
//		����:	�Ő�����菜�����Ƃɂ�胋�[�v��ǉ�����@�����@mekr
//					( kill edge and make ring)		
//
//		����:	�폜���Ő��̃��[�v�ƃy�A���Ő��̃��[�v������������
//				�폜���Ő����y�A���Ő��̌�̔��Ő��łȂ�����
//
//  K.Matsu			10/07/07	Created.
//==========================================================================================
//
#include "stdafx.h"

#include <cassert>
#include "MgsPrint.h"


namespace euler
{


MgsLoop* MgsFace::kemr(							// (  O) �ǉ����[�v (�������[�v)
						MgsEdge*	i_edgDel	// (I  ) �폜���Ő�
				)
{
	MgsEdge* edgDelS;
	MgsEdge* edgDelO;
	MgsEdge* edg1st;
	MgsEdge* edgC;
	MgsLoop* lopAdd;

	edgDelS = i_edgDel;
	edgDelO = i_edgDel->m_otr;

	// �����̊m�F:	�폜���Ő��̃��[�v�ƃy�A���Ő��̃��[�v������������
	//				�폜���Ő����y�A���Ő��̌�̔��Ő��łȂ�����
	assert( edgDelS->m_lop == edgDelO->m_lop && edgDelO->m_nxt != edgDelS);
  
    //
	// �폜���Ő��Ƃ̃y�A���Ő����폜���Ő��̒��_�ʒu�̃����N����O��
	// �폜���Ő��̃��[�v�̐擪���Ő����폜���Ő��̏ꍇ�܂��̓y�A���Ő��̏ꍇ��
	// ��O�̔��Ő��Ɠ���ւ���
	//		
	//		<--------------------^        <--------------------^			<--------------------^        <--------------------^
	//		|                    |		  |                    |			|                    |		  |                    |
	//		|                    |		  |                    |			|                    |		  |                    |
	//		|  (A1) (A)          |		  |       (A)          |			|  (A1) (A)          |		  |       (A)          |
	//		V=======>�@^--------->		  V--------> ---------->			V=======>�@^--------->		  V--------> ---------->
	//				|�@|															|�@|				
	//				|�@|															|�@|				
	//			DelS|�@|DelO		��											DelS|�@|DelO		��	
	//				|�@|															|�@|									
	//				|�@|															|�@|						  (C)
	//		<-------V�@<---------^		  <-------- <----------^					V  |						�-^ <-�
	//		|       (B)          |		  |       (B)          |				    (C)          				| | | |        
	//		|                    |		  |                    |												�-� �-�
	//		|                    |		  |      lopAdd        |						
	//		V-------------------->		  V-------------------->
	//

	edgDelS->m_pre->m_nxt = edgDelO->m_nxt;						// (A)
	edgDelO->m_nxt->m_pre = edgDelS->m_pre;

	// �폜���Ő��܂��̓y�A���Ő������[�v�̐擪�ł���ꍇ�́A���[�v�̐擪�͎�O�̔��Ő��ɂ���
	if( edgDelS->m_lop->m_edg == edgDelS ||
		edgDelS->m_lop->m_edg == edgDelO)
		edgDelS->m_lop->m_edg = edgDelS->m_pre;					// (A1)
    
    // �V�K�������[�v��ǉ�����
	lopAdd = 0;
	if( edgDelS->m_nxt == edgDelO) {
		lopAdd = new MgsLoop(this, edgDelO->m_vtx);				// (C)
	} else {
		lopAdd = new MgsLoop(this);

		edgDelO->m_pre->m_nxt = edgDelS->m_nxt;					// (B)
		edgDelS->m_nxt->m_pre = edgDelO->m_pre;
		lopAdd->m_edg = edgDelS->m_nxt;

		edg1st = edgDelS->m_nxt;
		edgC = edg1st;

		do {
			edgC->m_lop = lopAdd;
			edgC = edgC->m_nxt;
		} while( edgC != edg1st);
	}

	delete edgDelS; 
	delete edgDelO;

	MgSolid::assert_consistent_B_rep();

	return lopAdd;
}
  



}//end namespace euler


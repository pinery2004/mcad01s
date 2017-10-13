//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgsSolid.cpp
//
//		����
//
//
//  K.Matsu			10/07/07	Created.
//==========================================================================================
//
#include "stdafx.h"
#include "MgsEuler.h"
#include "MgsPrint.h"

namespace euler
{

//	�\���b�h�ɖʕ����쐬����
//
//	�E�ʕ���V�K�쐬����
//	�E�\���b�h����̖ʕ������N�̐擪�ɕt����
//

MgsFace* MgSolid::new_face()
{
	MgsFace* fceNew;
	
	//	�ʕ���V�K�쐬����
	fceNew = new MgsFace( this);

	//	�\���b�h����̖ʕ������N�̐擪�ɒǉ�����
	if( m_fce)
		m_fce->m_pre = fceNew;

	fceNew->m_pre = NULL;
	fceNew->m_nxt = m_fce;
	
	m_fce = fceNew;

	return fceNew;
}

//	�\���b�h�ɒ��_���쐬����
//
//	�E�_���W���w�肵�Ē��_��V�K�쐬����
//	�E�\���b�h����̒��_�o���������N�̌���ɕt����
//	1st
//						m_vtxfirst	m_vtxlast	�@
//								��	��
//					  m_pre(0)	vtxNew  m_nxt(0)
//
//	2nd �ȍ~
//								   	
//		m_vtxfirst		m_vtxlast  -------�E �@
//			�b				 �b			  �b
//			��			from(��)  m_nxt to��
//		  (vtx1)�@�E�E�E�E	vtxn   -->  vtxNew   m_nxt(0)
//								  m_pre
//								   <--
//

MgsVertex* MgSolid::new_vertex( MgsPoint const& pt) 
{
	MgsVertex* vtxNew;
	
	//	�_���W���w�肵�Ē��_��V�K�쐬����
	vtxNew = new MgsVertex( pt);

	//	�\���b�h����̒��_�o���������N�̌���ɕt����
	//	���_�Ԃ̃����N��t����
	if( m_vtxlast) {
		m_vtxlast->m_nxt = vtxNew;
		vtxNew->m_pre = m_vtxlast;
	}
	// �\���b�h����̃����N��t����
	if( ! m_vtxfirst)
		m_vtxfirst = vtxNew;
	m_vtxlast = vtxNew;

	return vtxNew;
}

//	�Ő���ǉ�����
//
//	�E���Ő�(edgNxt1)�̎�O�ɔ��Ő���ǉ�����
//	�E���Ő�(edgNxt2)�̎�O�ɔ��Ő���ǉ�����
//	�E�ǉ������Q�{�̔��Ő����y�A�ɂ���
//
//
// < case.1 >
//
//	pedg = add_edge_pair( edg1->m_vtx, edg1, vtx, edg1);
//
//
//		 �@edg1	   vtx1					[	   edg1	   vtx1	edgAdd1	 ]		  edg1	   vtx1	edgAdd2	 vtx
//		<--------- (edg1->n_vtx)	=>	[	<--------- 	   <----|	 ]	=>	<---------	   <---------		
//		--------->  ��			 		[	--------->  �� -----|	 ]		--------->  �� ---------> ��	
//		 edg1_nxt						[	 edg1_nxt				 ]		 edg1_nxt		edgAdd1
//
// Nxt1 ----> Nxt2
// Nxt1 --> edgAdd1 --> Nxt2
//
//
//
// < case.2 >
//
//	pedg = add_edge_pair( edg1->m_vtx, edg1, vtx, edg2);
//
//		   edgNxt2 vtx1(edg1.m_vtx)		  edgNxt2  			  
//		<----------	  <----------		<-----------�@   edgAdd2	<----------
//		   (----->)	   (----->)			  (----->) vtx1<--------- vtx(----->)
//				    ��				=>			 �@ ��			  �� 
//		   (<-----)	   (<-----)			  (<-----)	   --------->	 (<-----)
//		---------->	  ---------->		----------->	 edgAdd1	---------->
//						edgNxt1										  edgNxt1
//

std::pair<MgsEdge*, MgsEdge*> MgSolid::add_edge_pair(
						MgsVertex*	vtx1, MgsEdge*	edgNxt1,
						MgsVertex*	vtx2, MgsEdge*	edgNxt2
				)
{
//1	MgsEdge*	edgNxt2_nxt;
//1	MgsEdge*	edg_Nxt2;
	MgsEdge*	edgAdd1;
	MgsEdge*	edgAdd2;
	
//1	edgNxt2_nxt = edgNxt2->m_nxt;
 
	// ���Ő�(edgNxt1)�̎�O�ɔ��Ő���ǉ�����
	//edgAdd1 = new MgsEdge(vtx1, edgNxt1);

	edgAdd1 = new MgsEdge( vtx1);

	//	�ǉ����Ő��Ɏw��Ő��Ɗ֌W�t�����Ă��郋�[�v���֌W�t����
	edgAdd1->m_lop = edgNxt1->m_lop;
	//	�ǉ����Ő��̓y�A�̊֌W�̗Ő��͎����Ȃ�
//a	edgAdd1->m_otr = NULL;
	//	�w��Ő��̎�O�Ɋ֌W�t����
	edgNxt1->m_pre->m_nxt = edgAdd1;
	edgAdd1->m_pre = edgNxt1->m_pre;
	edgAdd1->m_nxt = edgNxt1;
	edgNxt1->m_pre = edgAdd1;

	// ���Ő�(edgNxt2)�̎�O�ɔ��Ő���ǉ�����
	//edgAdd2 = new MgsEdge(vtx2, edgNxt2_nxt->m_pre);

	edgAdd2 = new MgsEdge( vtx2);

//1	edg_Nxt2 = edgNxt2_nxt->m_pre;

	#define edg_Nxt2 edgNxt2		//1

	//	�ǉ����Ő��ɗŐ��Ɏw��Ő��Ɗ֌W�t�����Ă��郋�[�v���֌W�t����
	edgAdd2->m_lop = edg_Nxt2->m_lop;
	//	�ǉ����Ő��̓y�A�̊֌W�̗Ő��͎����Ȃ�
//a	edgAdd2->m_otr = NULL;
	//	�w��Ő��̎�O�Ɋ֌W�t����
	edg_Nxt2->m_pre->m_nxt = edgAdd2;
	edgAdd2->m_pre = edg_Nxt2->m_pre;
	edgAdd2->m_nxt = edg_Nxt2;
	edg_Nxt2->m_pre = edgAdd2;

	// �ǉ������Q�{�̔��Ő����y�A�ɂ���
	edgAdd1->m_otr = edgAdd2;		//a
	edgAdd2->m_otr = edgAdd1;		//a
	return std::make_pair(edgAdd1, edgAdd2);
}

//	�\���b�h�̃f�X�g���N�^
//
//	�{�\���b�h���S�\���b�h�̃����N�̐擪�ɂ���ꍇ�́A���̃\���b�h��擪�����N�ɐݒ肷��
//	�\���b�h�̃����N���l�߂�
//	�\���ʕ���S�č폜����
//	�\�����_��S�č폜����
//	�ʕ���Id���J�����gId-1�̏ꍇ(�ŏIId�̏ꍇ)�̓J�����gId--;
// 
MgSolid::~MgSolid() 
{ 
	MgsFace* fceDel;
	MgsVertex* vtxDel;
	
	// �{�\���b�h���S�\���b�h�̃����N�̐擪�ɂ���ꍇ�́A���̃\���b�h��擪�����N�ɐݒ肷��
	if( m_sol == this)
		m_sol = m_nxt;

	// �\���b�h�̃����N���l�߂�
	if( m_pre)
		m_pre->m_nxt = m_nxt;  
	if( m_nxt)
		m_nxt->m_pre = m_pre; 

	// �\���ʕ���S�č폜����
	while( m_fce) {
		fceDel = m_fce;
		m_fce = m_fce->m_nxt;
		delete fceDel;
	}

	// �\�����_��S�č폜����
	while( m_vtxfirst) {
		vtxDel = m_vtxfirst;
		m_vtxfirst = vtxDel->m_nxt;
		delete vtxDel;
	}

	// �ʕ���Id���J�����gId-1�̏ꍇ(�ŏIId�̏ꍇ)�̓J�����gId--;
	if(m_id == m_cur_id - 1)
		m_cur_id--; 
}

//
//	�w��ʕ�(fceDel)���폜����
//
//	�w��ʕ�(fceDel)�O��̖ʕ��̃����N���l�߂�
//	�w��ʕ�(fceDel)���폜����
//
void MgSolid::del_face(MgsFace* fceDel)
{
	if( m_fce == fceDel)					// �\���b�h����̃����N�̐擪�̖ʕ��̏ꍇ��
		m_fce = fceDel->m_nxt; 				// �\���b�h�̃����N��t������

	if( fceDel->m_pre)
		fceDel->m_pre->m_nxt = fceDel->m_nxt;  

	if( fceDel->m_nxt)
		fceDel->m_nxt->m_pre = fceDel->m_pre; 

	delete fceDel;							// �ʕ��̃f�X�g���N�^�����s���ʕ���
											// �ʕ�����̃����N�̃��[�v���폜����
}

//
//	�w�蒸�_(vtxDel)���폜����
//
//	�E�w�蒸�_(vtxDel)�O��̒��_�̃����N���l�߂�
//	�E�\���b�h����̃����N���{���_���w���Ă���ꍇ�͒��O�̎w�蒸�_(vtxDel)���폜����
//
void MgSolid::del_vertex(MgsVertex* vtxDel)
{
	if( vtxDel == NULL)
		return;

	//	�w�蒸�_(vtxDel)�O��̒��_�̃����N���l�߂�
	if( vtxDel->m_pre)
		vtxDel->m_pre->m_nxt = vtxDel->m_nxt;

	if( vtxDel->m_nxt)
		vtxDel->m_nxt->m_pre = vtxDel->m_pre;

	//if(m_vtxfirst == vtxDel) {								// �I���W�i���͍��̒ʂ�ł��邪��q�̒ʂ�ł́@???
	//	if( (m_vtxfirst = vtx->m_pre) == NULL)
	//		m_vtxfirst = vtx->m_nxt;
	//}

	//	�\���b�h����̐擪�֌W(�擪�����N)���{���_���w���Ă���ꍇ�͒���̒��_���֌W�t����
	if(m_vtxfirst == vtxDel)
		m_vtxfirst = vtxDel->m_nxt;

	//	�\���b�h����̍ŏI�֌W(�ŏI�����N)���{���_���w���Ă���ꍇ�͒��O�̒��_���֌W�t����
	if(m_vtxlast == vtxDel)
		m_vtxlast = vtxDel->m_pre;
	delete vtxDel;
}

//	�w��Ő�(vtxDel)���폜����
//
//	�E�w��Ő�(vtxDel)�O��̗Ő��̃����N���l�߂�
//	�E�\���b�h����̃����N���{�Ő����w���Ă���ꍇ�͒��O�̎w��Ő�(vtxDel)���폜����
//
void MgSolid::del_edge_pair(MgsEdge* edgDel)
{
	MgsEdge* edgOtr;
	MgsEdge* *pedgC;

	edgOtr = edgDel->m_otr;

	// ���[�v�̐擪���폜���Ő��ł��y�A�Ő��ł��Ȃ��Ő��ƂȂ�悤�A�폜���Ő��ł��y�A�Ő��Ȃ玟�̗Ő���擪�ɂ���
	for( pedgC = &(edgDel->m_lop->m_edg); *pedgC == edgDel || *pedgC == edgOtr; *pedgC = (*pedgC)->m_nxt);
	for( pedgC = &(edgOtr->m_lop->m_edg); *pedgC == edgOtr || *pedgC == edgOtr; *pedgC = (*pedgC)->m_nxt);

	//	�w�蔼�Ő��̃����N���l�߂�
	edgDel->m_pre->m_nxt = edgDel->m_nxt;
	edgDel->m_nxt->m_pre = edgDel->m_pre;

	//	�y�A���Ő��̃����N���l�߂�
	edgOtr->m_pre->m_nxt = edgOtr->m_nxt;
	edgOtr->m_nxt->m_pre = edgOtr->m_pre;

	// �w�蔼�Ő��ƃy�A���Ő����폜����
	delete edgDel;
	delete edgOtr;
}

//	�ʕ�(fce1)�̃��[�v��ʕ�(fce2)�̊O�����[�v(�擪���[�v)�̎��ɒǉ���
//	�ʕ�(fce1)�̃��[�v�Ɩʕ�(fce1)���폜����
//
//	�E�ʕ�(fce1)�̃��[�v��ʕ�(fce2)�̃��[�v�ɕύX����
//	�E�ʕ�(fce2)�̃��[�v�̃����N�̐擪�̎��ɖʕ�(fce1)�̃��[�v�Q��t����
//	�E�t�����������[�v�Q�̌�ɖʕ�(fce2)�̃��[�v�̂Q�Ԗڈȍ~��t����
//	�E�ʕ�(fce1)�̃��[�v��؂�		�K�v???
//	�E�ʕ�(fce1)���폜����
//
void MgSolid::add_MgsFaceo(MgsFace* fce1, MgsFace* fce2) 
{ 
	MgsLoop* lop;
	MgsLoop* lopNxt;
	MgsLoop* last;

	assert( fce1 != fce2);
	// �ʕ�(fce1)�̃��[�v��ʕ�(fce2)�̃��[�v�ɕύX����
	for( lop = fce1->m_lop; lop; lop = lop->m_nxt)
		lop->m_fce = fce2;

	// �ʕ�(fce2)�̃��[�v�̃����N�̐擪�̎��ɖʕ�(fce1)�̃��[�v�Q��t����
	lopNxt = fce2->m_lop->m_nxt;
	fce2->m_lop->m_nxt = fce1->m_lop;
	fce1->m_lop->m_pre = fce2->m_lop;

	// �t�����������[�v�Q�̌�ɖʕ�(fce2)�̃��[�v�̂Q�Ԗڈȍ~��t����
	last = fce1->m_lop;
	while( last->m_nxt)
		last = last->m_nxt;

	last->m_nxt = lopNxt;
	if( lopNxt)
		lopNxt->m_pre = last;

	// �ʕ�(fce1)�̃��[�v��؂�		�K�v???
	fce1->m_lop = NULL;

	// �ʕ�(fce1)���폜����
	del_face( fce1);
}

//	�\���b�h�̑S���_�̍��W���x�N�g��(vct)�����Ɉړ�����
//
MgSolid& MgSolid::operator += (MgsVect const& vct)
{
	MgsVertex* vtxSrc;

	for( vtxSrc = m_vtxfirst; vtxSrc; vtxSrc = vtxSrc->m_nxt) {
		vtxSrc->m_pt += vct;
	}
	return *this;
}


}//end namespace euler


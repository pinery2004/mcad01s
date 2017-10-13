//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgsLoop.cpp
//
//		���[�v�̒�`�ƕҏW
//
//
//  K.Matsu			10/07/07	Created.
//==========================================================================================
//
#include "stdafx.h"
#include "MgsEuler.h"

namespace euler
{

//	�R���X�g���N�^
//
MgsLoop::MgsLoop( MgsFace* f)
{
	m_fce = f;
	m_edg = NULL;
	m_pre = NULL;
	m_nxt = NULL;

    append_to_loop_list();
}

//	�ʕ��ɒP�꒸�_�̂Q�̘A�����Ő����쐬����	(�o���������N�݂̂ŁA�y�A�ł͂Ȃ��Ɨ����Ő�)
//
MgsLoop::MgsLoop( MgsFace* fce, MgsVertex* vtx)
{
	MgsEdge*	edgO;

	m_fce = fce;
	m_edg = NULL;
	m_pre = NULL;
	m_nxt = NULL; 

	append_to_loop_list();

	//new MgsEdge( vtx, this);
	//new MgsEdge( vtx, this->m_edg);

	// ���[�v�ɗŐ������������m�F����(�G���[�`�F�b�N)
	assert( ! m_edg);

	// ���[�v�ɐV�K�Ő���݂���
	m_edg = (MgsEdge*) new MgsEdge( vtx);
	// �Ő��Ƀ��[�v���֌W�t����
	m_edg->m_lop = this;

	// �V�K�Ő���ǉ�����
	edgO = new MgsEdge( vtx);
	// �Ő��Ƀ��[�v���֌W�t����
	edgO->m_lop = this;
	// �w��Ő��̎�O�Ɋ֌W�t����

	// �Ő��Ԃ̊֌W������
	m_edg->m_nxt = edgO;
	m_edg->m_pre = edgO;
	edgO->m_nxt = m_edg;
	edgO->m_pre = m_edg;
}

//	�f�X�g���N�^
//
MgsLoop::~MgsLoop()
{
	MgsEdge *edgFst;
	MgsEdge* edgDel;

	// ���[�v���\������S�G�b�W���폜����
	//
	if( m_edg == NULL)
		return;

	edgFst = m_edg;
	do {
		edgDel = m_edg;
		m_edg = m_edg->m_nxt;
		delete edgDel;
	} while( m_edg != edgFst);
}

//	�P�꒸�_�̂Q�̘A�����Ő����폜����
//
void MgsLoop::del_single_vertex_edge_pair()
{
	MgsEdge *edg1, *edg2, *edgC;

	edg1 = NULL;
	edg2 = NULL;

	// �\���Ő������ǂ�A�Q�A�����ăy�A�Ő��������Ȃ��Ő��Q�����߂�
	edgC = m_edg;
	do {
		if( ! edgC->m_otr && ! edgC->m_nxt->m_otr) {
			edg1 = edgC;
			edg2 = edgC->m_nxt;
			break;
		}
		edgC = edgC->m_nxt;

	} while( edgC != m_edg);

	if( !edg1 || !edg2)											// �Q�A�����ăy�A�Ő��������Ȃ��Ő����Ȃ��ꍇ��
		return;													// �������Ȃ�
    
	assert( edg1->m_vtx == edg2->m_vtx);
    // �Ő��̃����N��3�p�`�łȂ��ꍇ�ɁA���������Q�̗Ő����폜����
	if( m_edg->m_nxt->m_nxt->m_nxt != m_edg) {					// �Ő��̃����N��3�p�`�̏ꍇ�͉������Ȃ�  ?????
//	if( m_edg->m_nxt->m_nxt != m_edg) {							// single_vertex_edge_pair�݂̂̏ꍇ�͉������Ȃ��@�����ł͂Ȃ���????? 2007/11/04
    	if( m_edg == edg1 || m_edg == edg2)
			m_edg = edg2->m_nxt;								// ���[�v�̑�\�Ő����c��Ő��ɐݒ肷��
		edg2->m_nxt->m_pre = edg1->m_pre;						// �Ő��̃����N���폜�ɍ��킹�Č��ђ���
		edg1->m_pre->m_nxt = edg2->m_nxt;
		delete edg1;											// �Ő����폜����
		delete edg2;
	}
}

//	���[�v�̃G�b�W�������߂�
//
int MgsLoop::No_of_Vertex() const
{
	int icEdg;
	MgsEdge* edgC;

	icEdg = 0;
	edgC = m_edg;
	do {
		icEdg++;
		edgC = edgC->m_nxt;
	} while( edgC != m_edg);
	return icEdg;
}
  


}//end namespace euler


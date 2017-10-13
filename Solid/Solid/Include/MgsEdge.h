#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgsEdge.h
//
//		�Ő��̒�`�A�ҏW
//
//
//  K.Matsu			10/07/07	Created.
//==========================================================================================
//
/**
 *\file		MgsEdge.h
 *
 *		
 *\brief        Define HALF edge (no EDGE in euler!)
 *
 *              1. From a *pure perspective* of topological plane model of a closed	
 *                 2-manifold, there are actually no such things as edges, instead,	
 *                 there are only half edges, with the condition each is identified to	
 *		   exactly another one. Therefore, there is no edge nodes in my		
 *                 design. The identification is archived by storing the opposite half	
 *                 edge in the half edge node (member o).				
 *											
 *              2. All half edges, including circular edge (end points coincident), have
 *                 their oppositive halves, except singular half edge, i.e., of zero length,
 *                 which has nil as its opposite half edge link.
 * 
 *              3. Because there are only half edges, we simple use MgsEdge to denote the 
 *                 actually half edge type.
 *
 *\author	Xianming Chen\n
 *		Computer Science Department\n
 *		University of Utah
 *
 *\date		15 Aug 2006\n
 *		Copyright (c) 2006, University of Utah
 */

#include "MgsClass.h"
#include <utility>
#include <cassert>


namespace euler
{

class MgsEdge
{
public:
	MgsLoop *m_lop;
	MgsEdge *m_otr;
	MgsEdge *m_pre, *m_nxt;
	MgsVertex *m_vtx;
  
private:
	//	�V�K�̎w�蒸�_�����Ő����쐬����
	MgsEdge( MgsVertex* vtx)
	{
		// �Ő��Ɏw�蒸�_���֌W�t����
		m_vtx = vtx;
		// �Ő��͑��̗Ő��Ƃ̊֌W(�����N�A�y�A)�͎����Ȃ�
		m_otr = NULL;

		//m_pre = this;
		//m_nxt = this;
		//// �Ő��Ɏw�胋�[�v���֌W�t����
		//m_lop = lop;
		//// �w�胋�[�v�ɖ{�Ő����֌W�t����
		//m_lop->m_edg = this;
	}


	//	�V�K�̎w�胋�[�v�ɁA�w�蒸�_��[�_�Ɏ����Ő���V�K�쐬����
	//�@====
	//	�E�V�K�Ő����쐬����(�R���X�g���N�^)
	//	�E�Ő��Ɏw�胋�[�v���֌W�t����
	//	�E�Ő��͑��̗Ő��Ƃ̊֌W(�����N�A�y�A)�͎����Ȃ�
	//	�E�Ő��Ɏw�蒸�_���֌W�t����
	//	�E�w�胋�[�v�ɗŐ��Ƃ̊֌W�t�����܂����������m�F����(�G���[�`�F�b�N)
	//	�E�w�胋�[�v�ɖ{�Ő����֌W�t����
	// 
	MgsEdge( MgsVertex* vtx, MgsLoop* lop)
	{
		// �Ő��Ɏw�胋�[�v���֌W�t����
		m_lop = lop;
		// �Ő��͑��̗Ő��Ƃ̊֌W(�����N�A�y�A)�͎����Ȃ�
		m_otr = NULL;
		m_pre = this;
		m_nxt = this;
		// �Ő��Ɏw�蒸�_���֌W�t����
		m_vtx = vtx;
		// �w�胋�[�v�ɗŐ��Ƃ̊֌W�t�����܂����������m�F����(�G���[�`�F�b�N)
		assert( ! m_lop->m_edg);
		// �w�胋�[�v�ɖ{�Ő����֌W�t����
		m_lop->m_edg = this;
	}

    //	�w��Ő�(edgNxt)�̎�O�ɑo���������N��t�������Ő���V�K�쐬����
	//
	//	�E�V�K�Ő����쐬����(�R���X�g���N�^)	
	//	�E�Ő��Ɏw��Ő��Ɗ֌W�t�����Ă��郋�[�v���֌W�t����
	//	�E�Ő��̓y�A�̊֌W�̗Ő��͎����Ȃ�
	//	�E�w��Ő��̎�O�Ɋ֌W�t����
	//	�E�Ő��Ɏw�蒸�_���֌W�t����
	// 
	MgsEdge( MgsVertex* vtx, MgsEdge* edgNxt)
	{
		// �Ő��Ɏw��Ő��Ɗ֌W�t�����Ă��郋�[�v���֌W�t����
		m_lop = edgNxt->m_lop;
		// �Ő��̓y�A�̊֌W�̗Ő��͎����Ȃ�
		m_otr = NULL;
		// �w��Ő��̎�O�Ɋ֌W�t����
		m_nxt = edgNxt;
		m_pre = edgNxt->m_pre;
		m_pre->m_nxt = this;
		edgNxt->m_pre = this;
		// �Ő��Ɏw�蒸�_���֌W�t����
		m_vtx = vtx;
	}

	friend class MgSolid;
	friend class MgsLoop;
};


}


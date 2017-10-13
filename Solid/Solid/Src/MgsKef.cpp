//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgsKef.cpp
//
//		����	�Ő����폜��2�̖ʕ�����������@�����@mef
//					( kill edge and face)		
//		�ڍ�	��������Q�̖ʕ��̋��E�Ő��̍폜�ʕ�����(�n�[�t)�G�b�W���w�肷�鎖�ɂ��
//				�w�肳�ꂽ�Ő��Ɩʕ��A���[�v���폜���A�y�A�G�b�W���̖ʕ��ƃ��[�v�ɂ܂Ƃ߂�
//
//  K.Matsu			10/07/07	Created.
//==========================================================================================
//
#include "stdafx.h"

#include "MgsPrint.h"
#include <cassert>


namespace euler
{

	

void MgSolid::kef(
						MgsEdge*	i_edgDel	// (I  ) �폜�G�b�W
				)
{
	MgsEdge* edgC;

	MgsLoop* lopDelS;						// �폜�G�b�W�̃��[�v
	MgsLoop* lopDelO;						// �폜�y�A�G�b�W�̃��[�v

	lopDelS = i_edgDel->m_lop;
	lopDelO = i_edgDel->m_otr->m_lop;

	//	�폜�G�b�W�̃��[�v���O�����[�v�ł��鎖���m�F����
	assert( lopDelS->m_fce->m_lop == lopDelS);

	//	�폜�G�b�W�̖ʕ�(fce2)�̃��[�v(lopDelS)���y�A�G�b�W�̖ʕ�(fce1)��
	//	�O�����[�v(�擪���[�v)(lop1)�̎��ɒǉ����ʕ�(fce2)�̃��[�v(lopDelS)
	//	�Ɩʕ�(fce2)���폜����
	add_MgsFaceo( lopDelS->m_fce, lopDelO->m_fce);

	edgC = i_edgDel;
	do {
		edgC->m_lop = lopDelO;
		edgC = edgC->m_nxt;
	} while ( edgC != i_edgDel );

	//	���[�vS�ƃ��[�vO����������
    //	�폜�G�b�W�ƃy�A�Ő�����菜��2�̃��[�v���Ȃ�
	i_edgDel->m_otr->m_pre->m_nxt = i_edgDel->m_nxt;
	i_edgDel->m_nxt->m_pre = i_edgDel->m_otr->m_pre;

	i_edgDel->m_pre->m_nxt = i_edgDel->m_otr->m_nxt;
	i_edgDel->m_otr->m_nxt->m_pre = i_edgDel->m_pre;

	//	���[�vO�̑�\�Ő����폜�y�A�Ő��̏ꍇ�͎��̗Ő��ɓ���ւ���
	if(lopDelO->m_edg == i_edgDel->m_otr)
		lopDelO->m_edg = i_edgDel->m_otr->m_nxt;

	//	���[�vS�̑�\�Ő���NULL�ɂ��A
	//	���[�vO�̖ʕ��̑�\���[�v�����[�vS�̏ꍇ�́A���̃��[�v�ɓ���ւ���
	//	���[�vS���폜����
	lopDelS->m_edg = 0;
	lopDelO->m_fce->del_loop( lopDelS);

	//	�Ő����폜����
	delete i_edgDel;
	delete i_edgDel->m_otr;

	MgSolid::assert_consistent_B_rep();
}
  




}//end namespace euler


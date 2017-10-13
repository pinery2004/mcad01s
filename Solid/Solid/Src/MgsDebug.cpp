//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgSolid.cpp
//
//		����	�\���b�h�̐��������m�F����
//				�S�\���b�h�̑S�ʕ��̑S���[�v�̑S�Ő��̃��[�v�ԍ��������������m�F����
//
//
//  K.Matsu			10/07/07	Created.
//==========================================================================================
//
#include "stdafx.h"
#include "MgsEuler.h"


namespace euler
{

//	�S�\���b�h�̑S�ʕ��̑S���[�v�̑S�Ő��̃��[�v�ԍ��������������m�F����
//
bool MgSolid::assert_consistent_B_rep()
{
	MgSolid* sol;				// �Е��������N�Ń��X�g��NULL
	MgsFace* fce;				// �Е��������N�Ń��X�g��NULL
	MgsLoop* lop;				// �Е��������N�Ń��X�g��NULL
	MgsEdge* edg;				// �o���������N
	MgsEdge* edgStt;			// ���[�v���̗Ő��̃����N�J�n

	for( sol = m_sol; sol; sol = sol->m_nxt) {					//	�S�\���b�h
		for( fce = sol->m_fce; fce; fce = fce->m_nxt) {			//	�S�ʕ�
			for( lop = fce->m_lop; lop; lop = lop->m_nxt) {		//	�S���[�v
				edgStt = lop->m_edg,

				edg = edgStt;
				do {											//	�S�Ő�
					if( edg->m_lop != lop) {					//		���[�v�ԍ�������Ă���ꍇ��
						*(int*)( 0) = 0;						//			error �I��������
						return false;
					}
					edg = edg->m_nxt;
				} while( edg != edgStt);
			}
		}
	}
	return true;
}
  


}//end namespace euler


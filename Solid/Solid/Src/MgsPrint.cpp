//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgsPrint.cpp
//
//		�g���[�X�o��
//
//
//  K.Matsu			10/07/07	Created.
//==========================================================================================
//
/**
 *\file		print.cpp
 *
 *\brief	
 *
 *\author	Xianming Chen\n
 *		Computer Science Department\n
 *		University of Utah
 *
 *\date		 16 Aug 2006\n
 *		Copyright (c) 2006, University of Utah
 */

#include "stdafx.h"
#include "MgsEuler.h"
#include "MgsPrint.h"

namespace euler
{

//	�\���b�h�̃g���[�X�o��
//
//	�E�\���b�h�̑S�ʕ����g���[�X�o�͂���
//
ostream& operator << (
				ostream&	os,
		const	MgSolid& 	sol
		)
{
	MgsFace *fce;
	
	os << "1. solid = " << hex << (int*)&sol << " (" << sol.m_id << ")";
	for( fce = sol.m_fce; fce; fce = fce->m_nxt) {
		os << "\n\t    " << fce;
	}
	return os;
}

//	�ʕ��̃g���[�X�o��
//
//	�E�ʕ���ID�ƑS���[�v���g���[�X�o�͂���
//
ostream& operator << (
				ostream&	os,
		const	MgsFace&	fce
		)
{
	const MgsLoop* lop;
	
	os << "2. face = " << hex << (int*)&fce << " (" << fce.m_id << ")";
	for( lop = fce.m_lop; lop; lop = lop->m_nxt) {
		os << "\n\t      " << lop;
	}
	return os;
}

//	���[�v�̃g���[�X�o��
//
//	�E���[�v�̑S���Ő����g���[�X�o�͂���
//
ostream& operator << (
				ostream&	os,
		const	MgsLoop&	lop
		)
{
	const MgsEdge* edg;
	
	os << "3. loop = " << hex << (int*)&lop;
	edg = lop.m_edg; 
	do {
		os << "\n\t\t" << *edg;
	} while( (edg = edg->m_nxt) != lop.m_edg );
	os << endl;
	return os;
}

//	���Ő��̃g���[�X�o��
//
//	�E���Ő��̒��_ID�ƃy�A�̔��Ő������Ȃ炻�̒��_ID���g���[�X�o�͂���
//	�E���Ő��̒��_�ƃy�A�̔��Ő��̒��_���������Ȃ�A���Ő��̃��[�v�̖ʕ�ID���g���[�X�o�͂���
//	�E���Ő��̒��_���W���g���[�X�o�͂���
//	�E�y�A�̔��Ő������Ȃ炻�̒��_���W���g���[�X�o�͂���
//
ostream& operator << (
				ostream&	os,
		const	MgsEdge&		edg
		)
{
	os << "4. half edge( vtxID, vtxOtrID) = " << hex << (int*)&edg << " (" << edg.m_vtx->m_id;
	if( edg.m_otr)
		os  << ", " << edg.m_otr->m_vtx->m_id;

	os << ")";

	if( edg.m_otr && edg.m_vtx == edg.m_otr->m_vtx)
	os << " [face = (" << edg.m_lop->m_fce->m_id << ") ]";

	os << "\t edgVtx = " << edg.m_vtx->m_pt;

	if(edg.m_otr)
		os << "\t edgOtrVtx = "<< edg.m_otr->m_vtx->m_pt;

	return os;
}

//	���_�̃g���[�X�o��
//
//	�E���_ID�ƍ��W���g���[�X�o�͂���
//
ostream& operator << (
				ostream&	os,
		const	MgsVertex&	vtx
		)
{
	return os << "5. vtx = (" << vtx.m_id << ')\t' << vtx.m_pt;
}


}//end namespace euler


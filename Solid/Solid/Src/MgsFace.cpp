//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgsFase.cpp
//
//		����	�ʕ��̏����A���[�v�̍쐬�A����
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

//	�ʕ��̃f�X�g���N�^
//
//	�\�����[�v��S�č폜����
//	�ʕ���Id���J�����gId-1�̏ꍇ(�ŏIId�̏ꍇ)�̓J�����gId--;
//
MgsFace::~MgsFace()
{
	while( m_lop) {
		MgsLoop* lopDel = m_lop;
		m_lop = m_lop->m_nxt;
		delete lopDel;
	}
	if( m_id == m_cur_id - 1)
		m_cur_id--; 
}

MgsLoop* MgsFace::new_loop()
{
	return new MgsLoop( this);
}

MgsLoop* MgsFace::new_loop( MgsVertex *v)
{
	return new MgsLoop( this, v);
}
	
//	�w�胋�[�v(lopDel)���폜����
//
//	�ʕ�����̃����N�̐擪�̃��[�v�ł���ꍇ�͖ʕ��̃����N�����̃��[�v�ɐݒ肷��
//	�w�胋�[�v(lopDel)�O��̃��[�v�̃����N���l�߁A�w�胋�[�v(lopDel)���폜����
//
void MgsFace::del_loop( MgsLoop* lopDel)
{
	if( m_lop == lopDel)					// �ʕ�����̃����N�̐擪���[�v�̏ꍇ��
		m_lop = lopDel->m_nxt;  			// �ʕ��̃����N�����̃��[�v�ɐݒ肷��
	if( lopDel->m_pre)
		lopDel->m_pre->m_nxt = lopDel->m_nxt;  
	if( lopDel->m_nxt)
		lopDel->m_nxt->m_pre = lopDel->m_pre; 
	delete lopDel;
}
 


}//end namespace euler


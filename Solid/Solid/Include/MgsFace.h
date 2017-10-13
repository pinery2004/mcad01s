#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgsFace.h
//
//		�ʕ��̒�`�A�ҏW
//
//
//  K.Matsu			10/07/07	Created.
//==========================================================================================
//

#include "MgsClass.h"

namespace euler
{


class MgsFace
{
public:
	MgIdS m_id;
	MgSolid *m_sol;
	MgsLoop *m_lop;
	MgsFace *m_pre, *m_nxt;
	MgsVect m_eq;

private:
	static MgIdS m_cur_id;

private:
	friend class MgSolid;

	//	�V�K�̎w��\���b�h�ɁA�ʕ���V�K�쐬����
	//
	//	�E�V�K�ʕ����쐬����(�R���X�g���N�^)
	//	�E�ʕ��ɃJ�����gID(m_cur_id++)��t����
	//	�E�ʕ��Ɏw��\���b�h���֌W�t����
	MgsFace(MgSolid* sol)
	{
		m_id = m_cur_id++;
		m_sol = sol;
		m_lop = NULL;
		m_pre = NULL;
		m_nxt = NULL;
	}
	~MgsFace();

public:
	/**
	*  construct an arc at xy-plane, assume the start point of arc (e->m_vtx) is positioned at angle theta1.
	*  return the edge tailed at the last vertex.
	*/

	MgsEdge* arc(MgsEdge* edg, MgsPoint const& cnt, float r, float stt, float end, int segs=8);
	MgsLoop* kemr(MgsEdge* edg);								//!< return created inner loop.
	MgsEdge* mekr(MgsEdge* edg1, MgsEdge* edg2);					//!< return new half edge v1v2.
	MgsLoop* new_loop();
	MgsLoop* new_loop(MgsVertex *vtx);
	void del_loop(MgsLoop* lop);
};


}



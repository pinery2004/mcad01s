#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgsLoop.h
//
//		ループの定義、編集
//
//
//  K.Matsu			10/07/07	Created.
//==========================================================================================
//
/**
 *\file		MgsLoop.h
 *
 *		
 *\brief
 *
 *
 *\author	Xianming Chen\n
 *		Computer Science Department\n
 *		University of Utah
 *
 *\date		15 Aug 2006\n
 *		Copyright (c) 2006, University of Utah
 */
#include "MgsClass.h"

namespace euler
{

class MgsLoop
{
public:
	MgsFace *m_fce;
	MgsEdge *m_edg;
	MgsLoop *m_pre, *m_nxt;

private:
     MgsLoop(MgsFace* fce);
	 MgsLoop(MgsFace* fce, MgsVertex *vtx);
	 ~MgsLoop();

	 friend class MgsFace;

public:
	MgsEdge* mev(MgsEdge* edg, MgsVertex* vtx);					//!< same as euler::MgSolid::mev(e, e, p).
	MgsEdge* mev(MgsEdge* edg, MgsPoint const& pt1);			//!< same as euler::MgSolid::mev(e, e, p).
	void kev(MgsEdge* edg);										//!< same as euler::MgSolid::kev(e),
																//	except e and its opposite have to be on the same loop.
	int No_of_Vertex() const;									//!< multiple counting vertices with multiple occurence in the loop.

	void append_to_loop_list();
	void del_single_vertex_edge_pair();
	//void set_loop_head(MgsEdge* head)
	//	{ m_edg = head; }
};
  
//	属する面分のループの最後に本ループを付ける
//
inline void MgsLoop::append_to_loop_list()
{
	MgsLoop* lopC;

	if( m_fce->m_lop) { 
		// 属する面分に既にループがある場合は、面分の最後のループの後に付ける
		for( lopC = m_fce->m_lop; lopC->m_nxt; lopC = lopC->m_nxt);
		lopC->m_nxt = this;
		m_pre = lopC;

	} else
		// 属する面分にループがない場合は、面分の先頭につける
		m_fce->m_lop = this;
}


}


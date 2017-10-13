#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgsVertex.h
//
//		点の定義、編集
//
//
//  K.Matsu			10/07/07	Created.
//==========================================================================================
//
 /**
 *\file		MgsVertex.h
 *
 *		
 *\brief
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


class MgsVertex
{
public:
	MgIdS	 m_id;
	MgsPoint m_pt;
	MgsEdge	 *m_edg;
	MgsVertex *m_pre, *m_nxt;

private:
	static MgIdS m_cur_id;

private:
	friend class MgSolid;
	MgsVertex(MgsPoint const& pt)
	{
		m_id = m_cur_id++;
		m_pt = pt;
		m_edg = NULL;
		m_pre = NULL;
		m_nxt = NULL;
	}
	~MgsVertex()
	{
		if ( m_id == m_cur_id-1)
			m_cur_id--;
	}
};


}


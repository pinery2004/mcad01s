#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgsSolid.h
//
//		ソリッドの定義、編集
//
//
//  K.Matsu			10/07/07	Created.
//==========================================================================================
//
#include "MgsClass.h"
#include <utility>
#include <iosfwd>

namespace euler
{

class MgSolid
{
	MgsVertex	*m_vtxfirst, *m_vtxlast;		// head and last vertices.

public:
	MgIdS		m_id;
	MgsFace*	m_fce;
	MgSolid		*m_pre, *m_nxt;

	static MgSolid*	m_sol;

private:
	static MgIdS	m_cur_id;

private:
	friend class MgsLoop;

	MgsVertex* new_vertex( MgsPoint const&p);				// append to vertex list.
	MgsFace* new_face();

	std::pair<MgsEdge*, MgsEdge*> 
		add_edge_pair( MgsVertex* v1, MgsEdge* nxt1, MgsVertex* v2, MgsEdge* nxt2);

	void del_vertex( MgsVertex* v);
	void del_face( MgsFace*);
	void del_edge_pair( MgsEdge* e);
	void add_MgsFaceo( MgsFace* f1, MgsFace *f2);				// add loops of f1 after the outer (first) loop of f2, delete f1. 
	void _kev( MgsEdge* e);
	MgsEdge* mev( MgsEdge* e1, MgsEdge* e2, MgsVertex* v);

public:
	//	コンストラクタ
	//
	//	新しいID(カレントID++)を設定し
	//	本ソリッドを全ソリッドリンクの先頭にリンク付ける
	//
	MgSolid()
	{
		m_vtxfirst = NULL;
		m_vtxlast = NULL;
		m_id = m_cur_id++;
		m_fce = NULL;
		m_pre = NULL;
		m_nxt = NULL;

		//	本ソリッドを全ソリッドリンクの先頭にリンク付ける
		if ( m_sol)
			m_sol->m_pre = this;
		m_sol = this;
	}
	~MgSolid();

	static MgSolid* disk( MgsPoint const& cntr=MgsPoint(), float r=1.0f, int segs=8); //!<actually a lamina (of two faces touch tightly to each other)
	static MgSolid* block( MgsPoint const& ur=MgsPoint(1.0f, 1.0f, 1.0f));            //!<make a block with lower left corner at origin, and upper right as given
	static bool assert_consistent_B_rep();
	void rsweep_wire( MgsLoop* l, int resolution = 8);                               //!<rotational sweep a wire (translational sweep only affects one surface, and is defined in euler::MgsFace).
	void tsweep_face( MgsFace* f, MgsVect const& disp = MgsVect(0.f, 0.f, 1.f) );
	void rsweep_face( MgsFace* f, float theta1=0, float theta2= 2*pi, int resolution = 10);
	static MgSolid* mvfs( MgsPoint const& p);

	MgsEdge* mev( MgsEdge* e1, MgsEdge* e2, MgsPoint const& p);	//!<subdivide edge cycle (at p0=p1==p2) into p0p+[e2, e1) at p0 (change in place), and pp0+[e1, e2) at p (as cycle at new vertex).
	void kev( MgsEdge* e);										//!<inverse of mev: edge cycle (at tail of e) is merged to that at head of e.
	MgsLoop *mef( MgsEdge* e1, MgsEdge* e2);						//!<subdivide edge loop (of l1==l2) into v1v2+[e2, e1) (change in place), and v2v1+[e1,e2) (as new edge loop of new face)
	MgsLoop *mef( MgsEdge* e)										//!< same as mef(e, e).
		{ return mef(e, e); }
	void kef( MgsEdge* e);										//!<inverse of mef: face is merged to that of opposite half edge.
	void kfmrh( MgsFace* f1, MgsFace* f2);
	void kfsmr( MgsFace* f1, MgsFace* f2)
		{ kfmrh( f1, f2); }
	void kffmh( MgsFace* f1, MgsFace* f2);
	void print_off_file( std::ostream& os) const;
	MgSolid& operator += ( MgsVect const& v);

};


}//end namespace euler



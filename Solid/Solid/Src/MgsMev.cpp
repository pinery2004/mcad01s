//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgsMev.cpp
//
//		処理
//			(make edge and vertex)
//
//  K.Matsu			10/07/07	Created.
//==========================================================================================
//
#include "stdafx.h"

#include "MgsEuler.h"
#include <cassert>
#include <utility>
using namespace std;


namespace euler
{


// edg1の頂点(=edg2の頂点)に出入りする半稜線を２つのグループに分割し新しい稜線で結ぶ
// ・edg2からedg1.m_preまでのグループとedg1からedg2.m_preまでのグループに分ける
// ・edg1からedg2.m_preまでのグループの外に出る半稜線の頂点は新しく指定した座標pの頂点にする
//
MgsEdge* MgSolid::mev( MgsEdge* edg1, MgsEdge* edg2, MgsPoint const& p)
{
	MgsVertex* vtxNew;
	assert( edg1->m_vtx == edg2->m_vtx);
	vtxNew = edg1->m_lop->m_fce->m_sol->new_vertex(p);
	return mev(edg1, edg2, vtxNew); 
}

// edg1の頂点(=edg2の頂点)に出入りする半稜線を２つのグループに分割し新しい稜線で結ぶ
// ・edg2からedg1.m_preまでのグループとedg1からedg2.m_preまでのグループに分ける
// ・edg1からedg2.m_preまでのグループの外に出る半稜線の頂点は新しく指定したvtxにする
// edg1の手前とedg2の手前でカットし、稜線を挿入する
// edg1の稜線から追加半稜線までの頂点をvtxにする
// vtx からedg2->m_vtxへの半稜線を返す
//
MgsEdge* MgSolid::mev( MgsEdge* edg1, MgsEdge* edg2, MgsVertex* vtx)
{
	pair<MgsEdge*, MgsEdge*> pedg;
	MgsEdge*	edgC;

	//
	//	 edg2						 edg2	   edgA2 
	//	<-----	 <-----				<-----　  <-----   <-----
	//		   ● edg1     　　→　		 　● -----> ● edg1
	//	----->	 ----->				----->	   edgA1   ----->
	//	 edg1.m_vtx				      edg1.m_vtx     vtx
	//
	pedg = add_edge_pair( edg1->m_vtx, edg1, vtx, edg2);

	// pr.firstよりpr.secondまでvtxを中心に順に回転し、vtxから出る全ての半稜線の頂点をvtxに設定する
	for( edgC = pedg.first->m_nxt; edgC != pedg.second; edgC = edgC->m_otr->m_nxt) {
		edgC->m_vtx = vtx;
	}

	MgSolid::assert_consistent_B_rep();
	return pedg.second;
}

//
//
//
//	  ･---------･			･-----------･						･-----------･
//	  | ･-----・|			| ･-------・|						| ･-------・|
//	  | |	  ｜| 			| |		  ｜|						| |		  ｜|
//	  |	|　e1 ↓| 			| |	　e1  ↓|   eA2  vtx			| |	　e1  ↓|   eA2         eA4  vtx
//	  |	･-----				| ･<-----    <-------				| ･<-----    <-------	 <-------   
//	  |	e0	  ●	 →	  　| e0	　● -------> ●	 →		| e0	　● -------> ● -------> ●
//	  ･------>				･------->	    eA1					･------->	   eA1		   eA3	  
//		  e1.m_vtx			      e1.m_vtx						      e1.m_vtx   eA2.m_vtx
//
MgsEdge* MgsLoop::mev(MgsEdge* e1, MgsPoint const& p)
{ 
	return m_fce->m_sol->mev( e1, e1, p);
}  

MgsEdge* MgsLoop::mev(MgsEdge* e1, MgsVertex* v)              
{ 
	return m_fce->m_sol->mev( e1, e1, v);
}  




}


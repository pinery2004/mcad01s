//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: Mgs.cpp
//
//		処理	稜線と頂点を削除する
//			(make edge and vertex)
//
//  K.Matsu			10/07/07	Created.
//==========================================================================================
//
#include "stdafx.h"

#include "MgsPrint.h"
#include <cassert>

namespace euler
{


//	処理:	稜線と頂点を削除する
//
//	条件:	指定半稜線とペア稜線が異なる面分であること	<Case.1>
//
void MgSolid::kev(
						MgsEdge*	edgDel		// (I  ) 削除半稜線
				)
{
	// 条件の確認:	指定半稜線とペア稜線が異なる面分であること
	assert( edgDel->m_lop->m_fce != edgDel->m_otr->m_lop->m_fce);

	return _kev( edgDel); 
} 

//	
//	処理:	稜線と頂点を削除する
//
//	<Case.1>
//
//	 edgEnd	 vtx1				   edgEnd		  
//	<--------	 <---------		 <--------　    edgDel	    <---------
//				  ----->				  vtx1 <------- vtx2 ----->
//			  ●			 ←	　		    ● ------->  ● 
//				  <-----					   m_otr        <-----
//	-------->	 --------->		 -------->				    --------->
//		  		  edgFront		            			     edgFront
//
//
//	<Case.2>
//
//	  ･---------･			･-----------･						･-----------･
//	  | ･-----・|			| ･-------・|						| ･-------・|
//	  | |	  ｜| 			| |		  ｜|						| |		  ｜|			  edgFront
//	  |	|　e1 ↓| 			| |	　e1  ↓|   eA2  vtx			| |	　e1  ↓| edgEnd vtx1 edgDel vtx2
//	  |	･-----				| ･<-----    <-------				| ･<-----    <-------	 <-------   
//	  |	e0	  ●	 ←	  　| e0	　● -------> ●	 ←		| e0	　● -------> ● -------> ●
//	  ･------>				･------->	    eA1					･------->	    		  m_otr	  
//		  e1.m_vtx			      e1.m_vtx						      e1.m_vtx   eA2.m_vtx
//

void MgSolid::_kev(
						MgsEdge*	edgDel		// (I  ) 削除半稜線
				)
{
	// 条件の確認:	指定半稜線とペア稜線が存在すること
	assert( edgDel->m_otr);

	MgsEdge* edgFront;
	MgsEdge* edgEnd;
	MgsVertex* vtx1;
	MgsVertex* vtx2;

	edgFront = edgDel->m_otr->m_nxt;
	edgEnd = edgDel->m_nxt;

	vtx1 = edgEnd->m_vtx;
	vtx2 = edgDel->m_vtx;

	del_edge_pair( edgDel);

	if( edgDel->m_otr->m_nxt != edgDel) {
		// <Case.1>
		if( edgFront->m_otr)
			// 元vtx2を頂点とする半稜線の頂点をvtx1に変更する
			for( ; edgFront != edgEnd; edgFront = edgFront->m_otr->m_nxt)
				edgFront->m_vtx = vtx1;
		else
			// single_vertex_edge_pairの場合は次の半稜線も頂点をvtx1に変更する
			edgFront->m_vtx = edgFront->m_nxt->m_vtx = vtx1;
	} else {
		// <case.2>
		// 削除半稜線の頂点を持つ他の半稜線なしのため頂点変更の処理はない
	}
	// 削除半稜線の頂点を削除する
	del_vertex(vtx2);

	MgSolid::assert_consistent_B_rep();
}

//	処理:	稜線と頂点を削除する
//
//	条件:	指定半稜線とペア稜線が同一ループである	<Case.2>
//
void MgsLoop::kev(
						MgsEdge*	edgDel		// (I  ) 削除半稜線
				)              
{ 
	//	条件の確認:	指定半稜線とペア稜線が同一ループである
	assert( edgDel->m_lop == edgDel->m_otr->m_lop);

	//	稜線と頂点を削除する
	m_fce->m_sol->_kev( edgDel);
}  



}
 


//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgsMekr.cpp
//
//			　 make edge and kill ring (inner loop)
//
//		処理　２つのループを稜線で接合し１つのループにする
//
//  K.Matsu			10/07/07	Created.
//==========================================================================================
//

#include "stdafx.h"
#include <cassert>
#include "MgsPrint.h"


namespace euler
{

//
//	２つのループを稜線で接合し１つのループにする
//	２つの稜線のループを稜線の頂点を結ぶことにより１つのループにする
//
//	条件: 指定稜線のループは異なること
//		  ２つ目の稜線のループは面分の外周ループでないこと
//				
//		  ●----------->●		  ●----------->●			●----------->●		  ●----------->●
//		  Ａ			｜		　Ａ			｜			Ａ			  ｜		　Ａ			｜
//		  ｜			｜		　｜edg1		｜			｜	lop1	  ｜  edgAdd2 ｜edg1		｜
//		  ｜			Ｖ		　｜			｜			｜			  Ｖ--------->｜			｜
//		  ｜	lop2	●		　●	lop1	｜	  =>	｜ edg2->m_vtx●		　●	lop1	｜
//		  ｜			｜		　Ａ			｜			｜			  ｜<---------Ａ			｜
//		  ｜		edg2｜		　｜			｜			｜		  edg2｜  edgAdd1 ｜			｜
//		  ｜			Ｖ		　｜			Ｖ			｜			  Ｖ		　｜			Ｖ
//		  ●<-----------●		  ●<-----------●			●<-----------●		  ●<-----------●
//

MgsEdge* MgsFace::mekr(MgsEdge * edg1, MgsEdge * edg2)
{
	//	条件の確認:	指定稜線のループは異なること
	//				２つ目の稜線のループは面分の外周ループでないこと
	assert(edg1->m_lop != edg2->m_lop && edg2->m_lop != m_lop);
	 
	MgsLoop* lop1;
	MgsLoop* lop2;

	MgsEdge* edgAdd2;
	MgsEdge* edgAdd1;
	MgsEdge* edgC;

	lop1 = edg1->m_lop;
	lop2 = edg2->m_lop;

	edgAdd2 = lop1->mev(edg1, edg2->m_vtx), 
	edgAdd1 = edgAdd2->m_otr;

	if(edg2->m_nxt != edg2) {
		// 2つのループを接続する
		edgAdd1->m_nxt = edg2;
		edgAdd2->m_pre = edg2->m_pre;

		edg2->m_pre->m_nxt = edgAdd2;
		edg2->m_pre = edgAdd1;
		// edg2のループをlop1に変更する
		for( edgC = edg2; edgC!= edgAdd2; edgC=edgC->m_nxt)
			edgC->m_lop = lop1;
	}
	// lop2を削除する
	lop2->m_edg = 0;
	del_loop(lop2);

	MgSolid::assert_consistent_B_rep();

	return edgAdd1;
}



}//end namespace euler


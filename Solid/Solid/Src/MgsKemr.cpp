//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgsKemr.cpp
//
//		処理:	稜線を取り除くことによりループを追加する　←→　mekr
//					( kill edge and make ring)		
//
//		条件:	削除半稜線のループとペア半稜線のループが等しいこと
//				削除半稜線がペア半稜線の後の半稜線でないこと
//
//  K.Matsu			10/07/07	Created.
//==========================================================================================
//
#include "stdafx.h"

#include <cassert>
#include "MgsPrint.h"


namespace euler
{


MgsLoop* MgsFace::kemr(							// (  O) 追加ループ (内部ループ)
						MgsEdge*	i_edgDel	// (I  ) 削除半稜線
				)
{
	MgsEdge* edgDelS;
	MgsEdge* edgDelO;
	MgsEdge* edg1st;
	MgsEdge* edgC;
	MgsLoop* lopAdd;

	edgDelS = i_edgDel;
	edgDelO = i_edgDel->m_otr;

	// 条件の確認:	削除半稜線のループとペア半稜線のループが等しいこと
	//				削除半稜線がペア半稜線の後の半稜線でないこと
	assert( edgDelS->m_lop == edgDelO->m_lop && edgDelO->m_nxt != edgDelS);
  
    //
	// 削除半稜線とのペア半稜線を削除半稜線の頂点位置のリンクから外す
	// 削除半稜線のループの先頭半稜線が削除半稜線の場合またはペア半稜線の場合は
	// 手前の半稜線と入れ替える
	//		
	//		<--------------------^        <--------------------^			<--------------------^        <--------------------^
	//		|                    |		  |                    |			|                    |		  |                    |
	//		|                    |		  |                    |			|                    |		  |                    |
	//		|  (A1) (A)          |		  |       (A)          |			|  (A1) (A)          |		  |       (A)          |
	//		V=======>　^--------->		  V--------> ---------->			V=======>　^--------->		  V--------> ---------->
	//				|　|															|　|				
	//				|　|															|　|				
	//			DelS|　|DelO		→											DelS|　|DelO		→	
	//				|　|															|　|									
	//				|　|															|　|						  (C)
	//		<-------V　<---------^		  <-------- <----------^					V  |						･-^ <-･
	//		|       (B)          |		  |       (B)          |				    (C)          				| | | |        
	//		|                    |		  |                    |												･-･ ･-･
	//		|                    |		  |      lopAdd        |						
	//		V-------------------->		  V-------------------->
	//

	edgDelS->m_pre->m_nxt = edgDelO->m_nxt;						// (A)
	edgDelO->m_nxt->m_pre = edgDelS->m_pre;

	// 削除半稜線またはペア半稜線がループの先頭である場合は、ループの先頭は手前の半稜線にする
	if( edgDelS->m_lop->m_edg == edgDelS ||
		edgDelS->m_lop->m_edg == edgDelO)
		edgDelS->m_lop->m_edg = edgDelS->m_pre;					// (A1)
    
    // 新規内部ループを追加する
	lopAdd = 0;
	if( edgDelS->m_nxt == edgDelO) {
		lopAdd = new MgsLoop(this, edgDelO->m_vtx);				// (C)
	} else {
		lopAdd = new MgsLoop(this);

		edgDelO->m_pre->m_nxt = edgDelS->m_nxt;					// (B)
		edgDelS->m_nxt->m_pre = edgDelO->m_pre;
		lopAdd->m_edg = edgDelS->m_nxt;

		edg1st = edgDelS->m_nxt;
		edgC = edg1st;

		do {
			edgC->m_lop = lopAdd;
			edgC = edgC->m_nxt;
		} while( edgC != edg1st);
	}

	delete edgDelS; 
	delete edgDelO;

	MgSolid::assert_consistent_B_rep();

	return lopAdd;
}
  



}//end namespace euler


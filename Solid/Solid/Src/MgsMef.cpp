//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgsMef.cpp
//
//		処理 稜線を追加することにより面分とループを２つに分ける
//				( make edge and face)		
//
//
//  K.Matsu			10/07/07	Created.
//==========================================================================================
//
#include "stdafx.h"

#include <cassert>
#include "MgsPrint.h"


namespace euler
{

//	２つの稜線の頂点を新しい稜線で結びつけ面分とループを追加する
//
//	<Case.1>	mef( edg1, edg2);
//
//				edg2   edg2->m_vtx									edg2  edg2->m_vtx
//		  ● <----------- ● <----------- ●				 ● <----------- ● <----------- ● 
//		  ｜							  Ａ				 ｜			    ↑｜	  fceNew Ａ
//		  ｜							  ｜				 ｜			    ｜｜         ↓	 ｜
//		  ｜							  ｜	   =>		 ｜ m_lop→edgA2｜｜edgA1←lopNew｜
//		  ｜							  ｜				 ｜			    ｜｜			 ｜
//		  Ｖ					edg1	  ｜				 Ｖ			    ｜↓	edg1	 ｜
//		  ● -----------> ● -----------> ●				 ● -----------> ● -----------> ● 
//					   edg1->m_vtx										  edg1->m_vtx	
//
//
//	<Case.2>	mef( edg1);
//				
//		  ● <----------- ● <----------- ●				 ● <----------- ● <----------- ● 
//		  ｜							  Ａ				 ｜				   				 Ａ
//		  ｜							  ｜				 ｜		   ･---(edgA2)--･        ｜  
//		  ｜							  ｜				 ｜		   |･--(edgA1)-･|        ｜  
//		  ｜				  			  ｜	   =>		 ｜	fce	   || fceNew   ||		 ｜
//		  ｜				　			  ｜				 ｜		   |･-->   	---･|		 ｜
//		  Ｖ					edg1	  ｜				 Ｖ		   ･----   	<---･ edg1	 ｜
//		  ● -----------> ● -----------> ●				 ● -----------> ● -----------> ● 
//					   edg1->m_vtx										  edg1->m_vtx	
//
//
MgsLoop* MgSolid::mef(							// (  O) 新規追加ループ
						MgsEdge*	edg1,		// (I  ) 新規追加半稜線の頂点(既存ループを閉じる半稜線) 
						MgsEdge*	edg2		// (I  ) 新規追加半稜線の頂点(追加ループを閉じる半稜線) 
				)
{
	pair<MgsEdge*, MgsEdge*> edgPar;

	edgPar = add_edge_pair( edg2->m_vtx, edg1, edg1->m_vtx, edg2);
	MgsEdge *&edgAdd1 = edgPar.first;
	MgsEdge *&edgAdd2 = edgPar.second;

    // 追加半稜線の連結し直し
	MgsEdge *edgAdd2Pre;
	edgAdd2Pre = edgAdd2->m_pre;		// case1の場合は半稜線2の手前、case2の場合は追加半稜線1

	// 追加半稜線2を半稜線1の手前に連結
	edgAdd2->m_pre = edgAdd1->m_pre;
	edgAdd1->m_pre->m_nxt = edgAdd2;

	// 追加半稜線1を半稜線2の手前に連結(case2の場合は追加半稜線1を追加半稜線1に連結)
	edgAdd1->m_pre = edgAdd2Pre;
	edgAdd2Pre->m_nxt = edgAdd1;
    
    // ループと面分を追加し関係付ける
	MgsFace* fceNew;
	MgsLoop* lopNew;

	fceNew = new_face();
	lopNew = fceNew->new_loop();

	MgsEdge* edgC;
	edgC = edgAdd1;
	do {
		edgC->m_lop = lopNew;
	} while( (edgC = edgC->m_nxt) != edgAdd1);

    // 追加半稜線をそれぞれのループの先頭に位置づける
	edgAdd1->m_lop->m_edg = edgAdd1;
	edgAdd2->m_lop->m_edg = edgAdd2;

	MgSolid::assert_consistent_B_rep();
	return lopNew;
}



}//end namespace euler


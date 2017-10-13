//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgsMef.cpp
//
//		ˆ— —Åü‚ğ’Ç‰Á‚·‚é‚±‚Æ‚É‚æ‚è–Ê•ª‚Æƒ‹[ƒv‚ğ‚Q‚Â‚É•ª‚¯‚é
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

//	‚Q‚Â‚Ì—Åü‚Ì’¸“_‚ğV‚µ‚¢—Åü‚ÅŒ‹‚Ñ‚Â‚¯–Ê•ª‚Æƒ‹[ƒv‚ğ’Ç‰Á‚·‚é
//
//	<Case.1>	mef( edg1, edg2);
//
//				edg2   edg2->m_vtx									edg2  edg2->m_vtx
//		  œ <----------- œ <----------- œ				 œ <----------- œ <----------- œ 
//		  b							  ‚`				 b			    ªb	  fceNew ‚`
//		  b							  b				 b			    bb         «	 b
//		  b							  b	   =>		 b m_lop¨edgA2bbedgA1©lopNewb
//		  b							  b				 b			    bb			 b
//		  ‚u					edg1	  b				 ‚u			    b«	edg1	 b
//		  œ -----------> œ -----------> œ				 œ -----------> œ -----------> œ 
//					   edg1->m_vtx										  edg1->m_vtx	
//
//
//	<Case.2>	mef( edg1);
//				
//		  œ <----------- œ <----------- œ				 œ <----------- œ <----------- œ 
//		  b							  ‚`				 b				   				 ‚`
//		  b							  b				 b		   ¥---(edgA2)--¥        b  
//		  b							  b				 b		   |¥--(edgA1)-¥|        b  
//		  b				  			  b	   =>		 b	fce	   || fceNew   ||		 b
//		  b				@			  b				 b		   |¥-->   	---¥|		 b
//		  ‚u					edg1	  b				 ‚u		   ¥----   	<---¥ edg1	 b
//		  œ -----------> œ -----------> œ				 œ -----------> œ -----------> œ 
//					   edg1->m_vtx										  edg1->m_vtx	
//
//
MgsLoop* MgSolid::mef(							// (  O) V‹K’Ç‰Áƒ‹[ƒv
						MgsEdge*	edg1,		// (I  ) V‹K’Ç‰Á”¼—Åü‚Ì’¸“_(Šù‘¶ƒ‹[ƒv‚ğ•Â‚¶‚é”¼—Åü) 
						MgsEdge*	edg2		// (I  ) V‹K’Ç‰Á”¼—Åü‚Ì’¸“_(’Ç‰Áƒ‹[ƒv‚ğ•Â‚¶‚é”¼—Åü) 
				)
{
	pair<MgsEdge*, MgsEdge*> edgPar;

	edgPar = add_edge_pair( edg2->m_vtx, edg1, edg1->m_vtx, edg2);
	MgsEdge *&edgAdd1 = edgPar.first;
	MgsEdge *&edgAdd2 = edgPar.second;

    // ’Ç‰Á”¼—Åü‚Ì˜AŒ‹‚µ’¼‚µ
	MgsEdge *edgAdd2Pre;
	edgAdd2Pre = edgAdd2->m_pre;		// case1‚Ìê‡‚Í”¼—Åü2‚Ìè‘OAcase2‚Ìê‡‚Í’Ç‰Á”¼—Åü1

	// ’Ç‰Á”¼—Åü2‚ğ”¼—Åü1‚Ìè‘O‚É˜AŒ‹
	edgAdd2->m_pre = edgAdd1->m_pre;
	edgAdd1->m_pre->m_nxt = edgAdd2;

	// ’Ç‰Á”¼—Åü1‚ğ”¼—Åü2‚Ìè‘O‚É˜AŒ‹(case2‚Ìê‡‚Í’Ç‰Á”¼—Åü1‚ğ’Ç‰Á”¼—Åü1‚É˜AŒ‹)
	edgAdd1->m_pre = edgAdd2Pre;
	edgAdd2Pre->m_nxt = edgAdd1;
    
    // ƒ‹[ƒv‚Æ–Ê•ª‚ğ’Ç‰Á‚µŠÖŒW•t‚¯‚é
	MgsFace* fceNew;
	MgsLoop* lopNew;

	fceNew = new_face();
	lopNew = fceNew->new_loop();

	MgsEdge* edgC;
	edgC = edgAdd1;
	do {
		edgC->m_lop = lopNew;
	} while( (edgC = edgC->m_nxt) != edgAdd1);

    // ’Ç‰Á”¼—Åü‚ğ‚»‚ê‚¼‚ê‚Ìƒ‹[ƒv‚Ìæ“ª‚ÉˆÊ’u‚Ã‚¯‚é
	edgAdd1->m_lop->m_edg = edgAdd1;
	edgAdd2->m_lop->m_edg = edgAdd2;

	MgSolid::assert_consistent_B_rep();
	return lopNew;
}



}//end namespace euler


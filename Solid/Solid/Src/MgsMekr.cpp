//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgsMekr.cpp
//
//			@ make edge and kill ring (inner loop)
//
//		ˆ—@‚Q‚Â‚Ìƒ‹[ƒv‚ğ—Åü‚ÅÚ‡‚µ‚P‚Â‚Ìƒ‹[ƒv‚É‚·‚é
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
//	‚Q‚Â‚Ìƒ‹[ƒv‚ğ—Åü‚ÅÚ‡‚µ‚P‚Â‚Ìƒ‹[ƒv‚É‚·‚é
//	‚Q‚Â‚Ì—Åü‚Ìƒ‹[ƒv‚ğ—Åü‚Ì’¸“_‚ğŒ‹‚Ô‚±‚Æ‚É‚æ‚è‚P‚Â‚Ìƒ‹[ƒv‚É‚·‚é
//
//	ğŒ: w’è—Åü‚Ìƒ‹[ƒv‚ÍˆÙ‚È‚é‚±‚Æ
//		  ‚Q‚Â–Ú‚Ì—Åü‚Ìƒ‹[ƒv‚Í–Ê•ª‚ÌŠOüƒ‹[ƒv‚Å‚È‚¢‚±‚Æ
//				
//		  œ----------->œ		  œ----------->œ			œ----------->œ		  œ----------->œ
//		  ‚`			b		@‚`			b			‚`			  b		@‚`			b
//		  b			b		@bedg1		b			b	lop1	  b  edgAdd2 bedg1		b
//		  b			‚u		@b			b			b			  ‚u--------->b			b
//		  b	lop2	œ		@œ	lop1	b	  =>	b edg2->m_vtxœ		@œ	lop1	b
//		  b			b		@‚`			b			b			  b<---------‚`			b
//		  b		edg2b		@b			b			b		  edg2b  edgAdd1 b			b
//		  b			‚u		@b			‚u			b			  ‚u		@b			‚u
//		  œ<-----------œ		  œ<-----------œ			œ<-----------œ		  œ<-----------œ
//

MgsEdge* MgsFace::mekr(MgsEdge * edg1, MgsEdge * edg2)
{
	//	ğŒ‚ÌŠm”F:	w’è—Åü‚Ìƒ‹[ƒv‚ÍˆÙ‚È‚é‚±‚Æ
	//				‚Q‚Â–Ú‚Ì—Åü‚Ìƒ‹[ƒv‚Í–Ê•ª‚ÌŠOüƒ‹[ƒv‚Å‚È‚¢‚±‚Æ
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
		// 2‚Â‚Ìƒ‹[ƒv‚ğÚ‘±‚·‚é
		edgAdd1->m_nxt = edg2;
		edgAdd2->m_pre = edg2->m_pre;

		edg2->m_pre->m_nxt = edgAdd2;
		edg2->m_pre = edgAdd1;
		// edg2‚Ìƒ‹[ƒv‚ğlop1‚É•ÏX‚·‚é
		for( edgC = edg2; edgC!= edgAdd2; edgC=edgC->m_nxt)
			edgC->m_lop = lop1;
	}
	// lop2‚ğíœ‚·‚é
	lop2->m_edg = 0;
	del_loop(lop2);

	MgSolid::assert_consistent_B_rep();

	return edgAdd1;
}



}//end namespace euler


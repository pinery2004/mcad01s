//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: Mgskffmh.cpp
//
//		処理	
//
//
//  K.Matsu			10/07/07	Created.
//==========================================================================================
//
/**
 *\file		kffmh.cpp
 *
 *\brief	connected sum
 *
 *              \em kffmh(MgsFace* f1, MgsFace* f2) identifies and kills two faces f1 and f2,  to either make a hole if
 *              two faces are in the same shell,  or to kill a shell otherwise.
 *
 *\author	Xianming Chen\n
 *		Computer Science Department\n
 *		University of Utah
 *
 *\date		20 Aug 2006\n
 *		Copyright (c) 2006, University of Utah
 */
#include "stdafx.h"

#include "MgsEuler.h"
#include "MgsPrint.h"


namespace euler
{

	
//　2つの物の面分fce1と面分fce2を重ね合わせて、2つの物を結合し、面分fce1と面分fce2を削除する
//
void MgSolid::kffmh(
						MgsFace*	fce1,
						MgsFace*	fce2
				)
{
	//　条件の確認:	面分fce1と面分fce2のループは外周のみであること(穴が開いていない)
	//				面分fce1の頂点の数と面分fce2の頂点の数が等しいこと
	//
	assert( ! fce1->m_lop->m_nxt &&
			! fce2->m_lop->m_nxt &&
			fce1->m_lop->No_of_Vertex() == fce2->m_lop->No_of_Vertex() );
	//
	//	面分fce2_m_otrの頂点を面分1の頂点に変換する
	//
	//			<------------						
	//			-----------> ＼	 1st(edg1->m_vtx)		
	//   edgC->m_pre->m_otr ＼					2nd(edg1->m_vtx)	
	//			<------------	 <------------					1st(edg1->m_vtx)	2nd(edg1->m_vtx)
	//			------------> ● ------------> ●						 ● <------------ ●
	//				edgC	／｜  edg2->m_nxt　｜		   edg1->m_otr   ｜	 edg2->m_pre  Ａ
	//			<----------- ／				　 ｜		   ----------->	／				  ｜
	//		 ● ------------>				   ｜		● <------------				　｜
	//		 Ａ		edg2		 			   ｜				edg1		　			　｜
	//		 ｜				fce2->m_lop		　 ｜		｜							　	　｜
	//		 ｜ edg2->m_pre					　 ｜		｜			fce1->m_lop		　	　｜
	//		 ｜								   Ｖ		Ｖ	  							  ｜
	//		 ● <------------ ● <-------------●		● ------------> ● ------------> ●
	//
	MgsEdge* edg1;
	MgsEdge* edg2;
	MgsEdge* edg20;
	MgsEdge* edgC;

	edg1 = fce1->m_lop->m_edg;
	edg2 = fce2->m_lop->m_edg;
	edg20 = edg2;

	do {
		for( edgC = edg2->m_otr; edgC != edg2->m_nxt; edgC = edgC->m_pre->m_otr) {
			edgC->m_vtx = edg1->m_vtx;
		}
	} while(
		(
		edg1 = edg1->m_nxt,
		edg2 = edg2->m_pre
		) != edg20 );

	// fce1のm_otrとfce2のm_otrをペアにしfce1とfce2を削除する
	edg1 = fce1->m_lop->m_edg;
	edg2 = fce2->m_lop->m_edg;
	edg20 = edg2;

	do {
		edg1->m_otr->m_otr = edg2->m_otr;
		edg2->m_otr->m_otr = edg1->m_otr;
	} while(
		(
		edg1 = edg1->m_nxt,
		edg2 = edg2->m_pre
		) != edg20 );

	del_face(fce1); 
	del_face(fce2);
}



}//end namespace euler


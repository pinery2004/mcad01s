//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgsKef.cpp
//
//		処理	稜線を削除し2つの面分を結合する　←→　mef
//					( kill edge and face)		
//		詳細	結合する２つの面分の境界稜線の削除面分側の(ハーフ)エッジを指定する事により
//				指定された稜線と面分、ループを削除し、ペアエッジ側の面分とループにまとめる
//
//  K.Matsu			10/07/07	Created.
//==========================================================================================
//
#include "stdafx.h"

#include "MgsPrint.h"
#include <cassert>


namespace euler
{

	

void MgSolid::kef(
						MgsEdge*	i_edgDel	// (I  ) 削除エッジ
				)
{
	MgsEdge* edgC;

	MgsLoop* lopDelS;						// 削除エッジのループ
	MgsLoop* lopDelO;						// 削除ペアエッジのループ

	lopDelS = i_edgDel->m_lop;
	lopDelO = i_edgDel->m_otr->m_lop;

	//	削除エッジのループが外周ループである事を確認する
	assert( lopDelS->m_fce->m_lop == lopDelS);

	//	削除エッジの面分(fce2)のループ(lopDelS)をペアエッジの面分(fce1)の
	//	外周ループ(先頭ループ)(lop1)の次に追加し面分(fce2)のループ(lopDelS)
	//	と面分(fce2)を削除する
	add_MgsFaceo( lopDelS->m_fce, lopDelO->m_fce);

	edgC = i_edgDel;
	do {
		edgC->m_lop = lopDelO;
		edgC = edgC->m_nxt;
	} while ( edgC != i_edgDel );

	//	ループSとループOを結合する
    //	削除エッジとペア稜線を取り除き2つのループをつなぐ
	i_edgDel->m_otr->m_pre->m_nxt = i_edgDel->m_nxt;
	i_edgDel->m_nxt->m_pre = i_edgDel->m_otr->m_pre;

	i_edgDel->m_pre->m_nxt = i_edgDel->m_otr->m_nxt;
	i_edgDel->m_otr->m_nxt->m_pre = i_edgDel->m_pre;

	//	ループOの代表稜線が削除ペア稜線の場合は次の稜線に入れ替える
	if(lopDelO->m_edg == i_edgDel->m_otr)
		lopDelO->m_edg = i_edgDel->m_otr->m_nxt;

	//	ループSの代表稜線をNULLにし、
	//	ループOの面分の代表ループがループSの場合は、次のループに入れ替える
	//	ループSを削除する
	lopDelS->m_edg = 0;
	lopDelO->m_fce->del_loop( lopDelS);

	//	稜線を削除する
	delete i_edgDel;
	delete i_edgDel->m_otr;

	MgSolid::assert_consistent_B_rep();
}
  




}//end namespace euler


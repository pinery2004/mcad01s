//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgSolid.cpp
//
//		処理	ソリッドの整合性を確認する
//				全ソリッドの全面分の全ループの全稜線のループ番号が正しい事を確認する
//
//
//  K.Matsu			10/07/07	Created.
//==========================================================================================
//
#include "stdafx.h"
#include "MgsEuler.h"


namespace euler
{

//	全ソリッドの全面分の全ループの全稜線のループ番号が正しい事を確認する
//
bool MgSolid::assert_consistent_B_rep()
{
	MgSolid* sol;				// 片方向リンクでラストはNULL
	MgsFace* fce;				// 片方向リンクでラストはNULL
	MgsLoop* lop;				// 片方向リンクでラストはNULL
	MgsEdge* edg;				// 双方向リンク
	MgsEdge* edgStt;			// ループ内の稜線のリンク開始

	for( sol = m_sol; sol; sol = sol->m_nxt) {					//	全ソリッド
		for( fce = sol->m_fce; fce; fce = fce->m_nxt) {			//	全面分
			for( lop = fce->m_lop; lop; lop = lop->m_nxt) {		//	全ループ
				edgStt = lop->m_edg,

				edg = edgStt;
				do {											//	全稜線
					if( edg->m_lop != lop) {					//		ループ番号が誤っている場合は
						*(int*)( 0) = 0;						//			error 終了させる
						return false;
					}
					edg = edg->m_nxt;
				} while( edg != edgStt);
			}
		}
	}
	return true;
}
  


}//end namespace euler


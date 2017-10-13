//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgsArc.cpp
//
//		処理	XY平面に平行な円弧をあらわすポリゴンを作成する
//
//
//  K.Matsu			10/07/07	Created.
//==========================================================================================
//
#include "stdafx.h"

#include "MgsEuler.h"
#include "MgsPrint.h"
#include <cmath>


namespace euler
{


//	XY平面に平行な円弧をあらわすポリゴンを作成する
//	最後の頂点を持つ稜線を返す
//	( 円を作成するには　終了角度は開始角度の1分割手前の角度を設定し、分割数は円の分割数－１を設定する)
//
MgsEdge* MgsFace::arc(
				MgsEdge*		edg,			// 円弧の始点を表す稜線(ループ、面分)
		const	MgsPoint&	ptCnt,			// 中心点
				float		r,				// 半径
				float		angStt,			// 開始角度
				float		angEnd,			// 終了角度　　円の場合は１つ手前
				int			nDiv			// 分割数　　　円の場合はn-1　　
				)
{
	MgsLoop*	lop;
	float	angDiv;
	float	angNSeg;

	// debug
	//cout << " ・Arc edg = " << edg << "\n\t ptCnt = " << ptCnt << " r = " << r <<
	//		" angStt = " << angStt << " angEnd = " << angEnd << " nDiv = " << nDiv << endl;

	lop = edg->m_lop;
    angDiv = ( angEnd - angStt) / nDiv;

	angNSeg = angDiv;
    for(int i=0; i<nDiv; i++, angNSeg += angDiv) {
		edg = lop->mev( edg, MgsPoint( ptCnt[0] + r * cos( angNSeg),
									   ptCnt[1] + r * sin( angNSeg),
									   ptCnt[2]));
		// debug
		//cout << " ・Arc edg No = " << i << this << endl;
    }
    lop->del_single_vertex_edge_pair(); 
    return edg;
}



}//end namespace euler


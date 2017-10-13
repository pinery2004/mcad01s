#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgsEdge.h
//
//		稜線の定義、編集
//
//
//  K.Matsu			10/07/07	Created.
//==========================================================================================
//
/**
 *\file		MgsEdge.h
 *
 *		
 *\brief        Define HALF edge (no EDGE in euler!)
 *
 *              1. From a *pure perspective* of topological plane model of a closed	
 *                 2-manifold, there are actually no such things as edges, instead,	
 *                 there are only half edges, with the condition each is identified to	
 *		   exactly another one. Therefore, there is no edge nodes in my		
 *                 design. The identification is archived by storing the opposite half	
 *                 edge in the half edge node (member o).				
 *											
 *              2. All half edges, including circular edge (end points coincident), have
 *                 their oppositive halves, except singular half edge, i.e., of zero length,
 *                 which has nil as its opposite half edge link.
 * 
 *              3. Because there are only half edges, we simple use MgsEdge to denote the 
 *                 actually half edge type.
 *
 *\author	Xianming Chen\n
 *		Computer Science Department\n
 *		University of Utah
 *
 *\date		15 Aug 2006\n
 *		Copyright (c) 2006, University of Utah
 */

#include "MgsClass.h"
#include <utility>
#include <cassert>


namespace euler
{

class MgsEdge
{
public:
	MgsLoop *m_lop;
	MgsEdge *m_otr;
	MgsEdge *m_pre, *m_nxt;
	MgsVertex *m_vtx;
  
private:
	//	新規の指定頂点を持つ稜線を作成する
	MgsEdge( MgsVertex* vtx)
	{
		// 稜線に指定頂点を関係付ける
		m_vtx = vtx;
		// 稜線は他の稜線との関係(リンク、ペア)は持たない
		m_otr = NULL;

		//m_pre = this;
		//m_nxt = this;
		//// 稜線に指定ループを関係付ける
		//m_lop = lop;
		//// 指定ループに本稜線を関係付ける
		//m_lop->m_edg = this;
	}


	//	新規の指定ループに、指定頂点を端点に持つ半稜線を新規作成する
	//　====
	//	・新規稜線を作成する(コンストラクタ)
	//	・稜線に指定ループを関係付ける
	//	・稜線は他の稜線との関係(リンク、ペア)は持たない
	//	・稜線に指定頂点を関係付ける
	//	・指定ループに稜線との関係付けがまだ無い事を確認する(エラーチェック)
	//	・指定ループに本稜線を関係付ける
	// 
	MgsEdge( MgsVertex* vtx, MgsLoop* lop)
	{
		// 稜線に指定ループを関係付ける
		m_lop = lop;
		// 稜線は他の稜線との関係(リンク、ペア)は持たない
		m_otr = NULL;
		m_pre = this;
		m_nxt = this;
		// 稜線に指定頂点を関係付ける
		m_vtx = vtx;
		// 指定ループに稜線との関係付けがまだ無い事を確認する(エラーチェック)
		assert( ! m_lop->m_edg);
		// 指定ループに本稜線を関係付ける
		m_lop->m_edg = this;
	}

    //	指定稜線(edgNxt)の手前に双方向リンクを付けた半稜線を新規作成する
	//
	//	・新規稜線を作成する(コンストラクタ)	
	//	・稜線に指定稜線と関係付けられているループを関係付ける
	//	・稜線はペアの関係の稜線は持たない
	//	・指定稜線の手前に関係付ける
	//	・稜線に指定頂点を関係付ける
	// 
	MgsEdge( MgsVertex* vtx, MgsEdge* edgNxt)
	{
		// 稜線に指定稜線と関係付けられているループを関係付ける
		m_lop = edgNxt->m_lop;
		// 稜線はペアの関係の稜線は持たない
		m_otr = NULL;
		// 指定稜線の手前に関係付ける
		m_nxt = edgNxt;
		m_pre = edgNxt->m_pre;
		m_pre->m_nxt = this;
		edgNxt->m_pre = this;
		// 稜線に指定頂点を関係付ける
		m_vtx = vtx;
	}

	friend class MgSolid;
	friend class MgsLoop;
};


}


//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgsLoop.cpp
//
//		ループの定義と編集
//
//
//  K.Matsu			10/07/07	Created.
//==========================================================================================
//
#include "stdafx.h"
#include "MgsEuler.h"

namespace euler
{

//	コンストラクタ
//
MgsLoop::MgsLoop( MgsFace* f)
{
	m_fce = f;
	m_edg = NULL;
	m_pre = NULL;
	m_nxt = NULL;

    append_to_loop_list();
}

//	面分に単一頂点の２つの連結半稜線を作成する	(双方向リンクのみで、ペアではない独立半稜線)
//
MgsLoop::MgsLoop( MgsFace* fce, MgsVertex* vtx)
{
	MgsEdge*	edgO;

	m_fce = fce;
	m_edg = NULL;
	m_pre = NULL;
	m_nxt = NULL; 

	append_to_loop_list();

	//new MgsEdge( vtx, this);
	//new MgsEdge( vtx, this->m_edg);

	// ループに稜線が無い事を確認する(エラーチェック)
	assert( ! m_edg);

	// ループに新規稜線を設ける
	m_edg = (MgsEdge*) new MgsEdge( vtx);
	// 稜線にループを関係付ける
	m_edg->m_lop = this;

	// 新規稜線を追加する
	edgO = new MgsEdge( vtx);
	// 稜線にループを関係付ける
	edgO->m_lop = this;
	// 指定稜線の手前に関係付ける

	// 稜線間の関係をつける
	m_edg->m_nxt = edgO;
	m_edg->m_pre = edgO;
	edgO->m_nxt = m_edg;
	edgO->m_pre = m_edg;
}

//	デストラクタ
//
MgsLoop::~MgsLoop()
{
	MgsEdge *edgFst;
	MgsEdge* edgDel;

	// ループを構成する全エッジを削除する
	//
	if( m_edg == NULL)
		return;

	edgFst = m_edg;
	do {
		edgDel = m_edg;
		m_edg = m_edg->m_nxt;
		delete edgDel;
	} while( m_edg != edgFst);
}

//	単一頂点の２つの連結半稜線を削除する
//
void MgsLoop::del_single_vertex_edge_pair()
{
	MgsEdge *edg1, *edg2, *edgC;

	edg1 = NULL;
	edg2 = NULL;

	// 構成稜線をたどり、２つ連続してペア稜線を持たない稜線２つを求める
	edgC = m_edg;
	do {
		if( ! edgC->m_otr && ! edgC->m_nxt->m_otr) {
			edg1 = edgC;
			edg2 = edgC->m_nxt;
			break;
		}
		edgC = edgC->m_nxt;

	} while( edgC != m_edg);

	if( !edg1 || !edg2)											// ２つ連続してペア稜線を持たない稜線がない場合は
		return;													// 何もしない
    
	assert( edg1->m_vtx == edg2->m_vtx);
    // 稜線のリンクが3角形でない場合に、検索した２つの稜線を削除する
	if( m_edg->m_nxt->m_nxt->m_nxt != m_edg) {					// 稜線のリンクが3角形の場合は何もしない  ?????
//	if( m_edg->m_nxt->m_nxt != m_edg) {							// single_vertex_edge_pairのみの場合は何もしない　こうではないか????? 2007/11/04
    	if( m_edg == edg1 || m_edg == edg2)
			m_edg = edg2->m_nxt;								// ループの代表稜線を残る稜線に設定する
		edg2->m_nxt->m_pre = edg1->m_pre;						// 稜線のリンクを削除に合わせて結び直す
		edg1->m_pre->m_nxt = edg2->m_nxt;
		delete edg1;											// 稜線を削除する
		delete edg2;
	}
}

//	ループのエッジ数を求める
//
int MgsLoop::No_of_Vertex() const
{
	int icEdg;
	MgsEdge* edgC;

	icEdg = 0;
	edgC = m_edg;
	do {
		icEdg++;
		edgC = edgC->m_nxt;
	} while( edgC != m_edg);
	return icEdg;
}
  


}//end namespace euler


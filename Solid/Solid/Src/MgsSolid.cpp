//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgsSolid.cpp
//
//		処理
//
//
//  K.Matsu			10/07/07	Created.
//==========================================================================================
//
#include "stdafx.h"
#include "MgsEuler.h"
#include "MgsPrint.h"

namespace euler
{

//	ソリッドに面分を作成する
//
//	・面分を新規作成する
//	・ソリッドからの面分リンクの先頭に付ける
//

MgsFace* MgSolid::new_face()
{
	MgsFace* fceNew;
	
	//	面分を新規作成する
	fceNew = new MgsFace( this);

	//	ソリッドからの面分リンクの先頭に追加する
	if( m_fce)
		m_fce->m_pre = fceNew;

	fceNew->m_pre = NULL;
	fceNew->m_nxt = m_fce;
	
	m_fce = fceNew;

	return fceNew;
}

//	ソリッドに頂点を作成する
//
//	・点座標を指定して頂点を新規作成する
//	・ソリッドからの頂点双方向リンクの後尾に付ける
//	1st
//						m_vtxfirst	m_vtxlast	　
//								↓	↓
//					  m_pre(0)	vtxNew  m_nxt(0)
//
//	2nd 以降
//								   	
//		m_vtxfirst		m_vtxlast  -------・ 　
//			｜				 ｜			  ｜
//			↓			from(↓)  m_nxt to↓
//		  (vtx1)　・・・・	vtxn   -->  vtxNew   m_nxt(0)
//								  m_pre
//								   <--
//

MgsVertex* MgSolid::new_vertex( MgsPoint const& pt) 
{
	MgsVertex* vtxNew;
	
	//	点座標を指定して頂点を新規作成する
	vtxNew = new MgsVertex( pt);

	//	ソリッドからの頂点双方向リンクの後尾に付ける
	//	頂点間のリンクを付ける
	if( m_vtxlast) {
		m_vtxlast->m_nxt = vtxNew;
		vtxNew->m_pre = m_vtxlast;
	}
	// ソリッドからのリンクを付ける
	if( ! m_vtxfirst)
		m_vtxfirst = vtxNew;
	m_vtxlast = vtxNew;

	return vtxNew;
}

//	稜線を追加する
//
//	・半稜線(edgNxt1)の手前に半稜線を追加する
//	・半稜線(edgNxt2)の手前に半稜線を追加する
//	・追加した２本の半稜線をペアにする
//
//
// < case.1 >
//
//	pedg = add_edge_pair( edg1->m_vtx, edg1, vtx, edg1);
//
//
//		 　edg1	   vtx1					[	   edg1	   vtx1	edgAdd1	 ]		  edg1	   vtx1	edgAdd2	 vtx
//		<--------- (edg1->n_vtx)	=>	[	<--------- 	   <----|	 ]	=>	<---------	   <---------		
//		--------->  ●			 		[	--------->  ● -----|	 ]		--------->  ● ---------> ●	
//		 edg1_nxt						[	 edg1_nxt				 ]		 edg1_nxt		edgAdd1
//
// Nxt1 ----> Nxt2
// Nxt1 --> edgAdd1 --> Nxt2
//
//
//
// < case.2 >
//
//	pedg = add_edge_pair( edg1->m_vtx, edg1, vtx, edg2);
//
//		   edgNxt2 vtx1(edg1.m_vtx)		  edgNxt2  			  
//		<----------	  <----------		<-----------　   edgAdd2	<----------
//		   (----->)	   (----->)			  (----->) vtx1<--------- vtx(----->)
//				    ●				=>			 　 ●			  ● 
//		   (<-----)	   (<-----)			  (<-----)	   --------->	 (<-----)
//		---------->	  ---------->		----------->	 edgAdd1	---------->
//						edgNxt1										  edgNxt1
//

std::pair<MgsEdge*, MgsEdge*> MgSolid::add_edge_pair(
						MgsVertex*	vtx1, MgsEdge*	edgNxt1,
						MgsVertex*	vtx2, MgsEdge*	edgNxt2
				)
{
//1	MgsEdge*	edgNxt2_nxt;
//1	MgsEdge*	edg_Nxt2;
	MgsEdge*	edgAdd1;
	MgsEdge*	edgAdd2;
	
//1	edgNxt2_nxt = edgNxt2->m_nxt;
 
	// 半稜線(edgNxt1)の手前に半稜線を追加する
	//edgAdd1 = new MgsEdge(vtx1, edgNxt1);

	edgAdd1 = new MgsEdge( vtx1);

	//	追加半稜線に指定稜線と関係付けられているループを関係付ける
	edgAdd1->m_lop = edgNxt1->m_lop;
	//	追加半稜線はペアの関係の稜線は持たない
//a	edgAdd1->m_otr = NULL;
	//	指定稜線の手前に関係付ける
	edgNxt1->m_pre->m_nxt = edgAdd1;
	edgAdd1->m_pre = edgNxt1->m_pre;
	edgAdd1->m_nxt = edgNxt1;
	edgNxt1->m_pre = edgAdd1;

	// 半稜線(edgNxt2)の手前に半稜線を追加する
	//edgAdd2 = new MgsEdge(vtx2, edgNxt2_nxt->m_pre);

	edgAdd2 = new MgsEdge( vtx2);

//1	edg_Nxt2 = edgNxt2_nxt->m_pre;

	#define edg_Nxt2 edgNxt2		//1

	//	追加半稜線に稜線に指定稜線と関係付けられているループを関係付ける
	edgAdd2->m_lop = edg_Nxt2->m_lop;
	//	追加半稜線はペアの関係の稜線は持たない
//a	edgAdd2->m_otr = NULL;
	//	指定稜線の手前に関係付ける
	edg_Nxt2->m_pre->m_nxt = edgAdd2;
	edgAdd2->m_pre = edg_Nxt2->m_pre;
	edgAdd2->m_nxt = edg_Nxt2;
	edg_Nxt2->m_pre = edgAdd2;

	// 追加した２本の半稜線をペアにする
	edgAdd1->m_otr = edgAdd2;		//a
	edgAdd2->m_otr = edgAdd1;		//a
	return std::make_pair(edgAdd1, edgAdd2);
}

//	ソリッドのデストラクタ
//
//	本ソリッドが全ソリッドのリンクの先頭にある場合は、次のソリッドを先頭リンクに設定する
//	ソリッドのリンクを詰める
//	構成面分を全て削除する
//	構成頂点を全て削除する
//	面分のIdがカレントId-1の場合(最終Idの場合)はカレントId--;
// 
MgSolid::~MgSolid() 
{ 
	MgsFace* fceDel;
	MgsVertex* vtxDel;
	
	// 本ソリッドが全ソリッドのリンクの先頭にある場合は、次のソリッドを先頭リンクに設定する
	if( m_sol == this)
		m_sol = m_nxt;

	// ソリッドのリンクを詰める
	if( m_pre)
		m_pre->m_nxt = m_nxt;  
	if( m_nxt)
		m_nxt->m_pre = m_pre; 

	// 構成面分を全て削除する
	while( m_fce) {
		fceDel = m_fce;
		m_fce = m_fce->m_nxt;
		delete fceDel;
	}

	// 構成頂点を全て削除する
	while( m_vtxfirst) {
		vtxDel = m_vtxfirst;
		m_vtxfirst = vtxDel->m_nxt;
		delete vtxDel;
	}

	// 面分のIdがカレントId-1の場合(最終Idの場合)はカレントId--;
	if(m_id == m_cur_id - 1)
		m_cur_id--; 
}

//
//	指定面分(fceDel)を削除する
//
//	指定面分(fceDel)前後の面分のリンクを詰める
//	指定面分(fceDel)を削除する
//
void MgSolid::del_face(MgsFace* fceDel)
{
	if( m_fce == fceDel)					// ソリッドからのリンクの先頭の面分の場合は
		m_fce = fceDel->m_nxt; 				// ソリッドのリンクを付け直す

	if( fceDel->m_pre)
		fceDel->m_pre->m_nxt = fceDel->m_nxt;  

	if( fceDel->m_nxt)
		fceDel->m_nxt->m_pre = fceDel->m_pre; 

	delete fceDel;							// 面分のデストラクタを実行し面分と
											// 面分からのリンクのループを削除する
}

//
//	指定頂点(vtxDel)を削除する
//
//	・指定頂点(vtxDel)前後の頂点のリンクを詰める
//	・ソリッドからのリンクが本頂点を指している場合は直前の指定頂点(vtxDel)を削除する
//
void MgSolid::del_vertex(MgsVertex* vtxDel)
{
	if( vtxDel == NULL)
		return;

	//	指定頂点(vtxDel)前後の頂点のリンクを詰める
	if( vtxDel->m_pre)
		vtxDel->m_pre->m_nxt = vtxDel->m_nxt;

	if( vtxDel->m_nxt)
		vtxDel->m_nxt->m_pre = vtxDel->m_pre;

	//if(m_vtxfirst == vtxDel) {								// オリジナルは左の通りであるが後述の通りでは　???
	//	if( (m_vtxfirst = vtx->m_pre) == NULL)
	//		m_vtxfirst = vtx->m_nxt;
	//}

	//	ソリッドからの先頭関係(先頭リンク)が本頂点を指している場合は直後の頂点を関係付ける
	if(m_vtxfirst == vtxDel)
		m_vtxfirst = vtxDel->m_nxt;

	//	ソリッドからの最終関係(最終リンク)が本頂点を指している場合は直前の頂点を関係付ける
	if(m_vtxlast == vtxDel)
		m_vtxlast = vtxDel->m_pre;
	delete vtxDel;
}

//	指定稜線(vtxDel)を削除する
//
//	・指定稜線(vtxDel)前後の稜線のリンクを詰める
//	・ソリッドからのリンクが本稜線を指している場合は直前の指定稜線(vtxDel)を削除する
//
void MgSolid::del_edge_pair(MgsEdge* edgDel)
{
	MgsEdge* edgOtr;
	MgsEdge* *pedgC;

	edgOtr = edgDel->m_otr;

	// ループの先頭が削除半稜線でもペア稜線でもない稜線となるよう、削除半稜線でもペア稜線なら次の稜線を先頭にする
	for( pedgC = &(edgDel->m_lop->m_edg); *pedgC == edgDel || *pedgC == edgOtr; *pedgC = (*pedgC)->m_nxt);
	for( pedgC = &(edgOtr->m_lop->m_edg); *pedgC == edgOtr || *pedgC == edgOtr; *pedgC = (*pedgC)->m_nxt);

	//	指定半稜線のリンクを詰める
	edgDel->m_pre->m_nxt = edgDel->m_nxt;
	edgDel->m_nxt->m_pre = edgDel->m_pre;

	//	ペア半稜線のリンクを詰める
	edgOtr->m_pre->m_nxt = edgOtr->m_nxt;
	edgOtr->m_nxt->m_pre = edgOtr->m_pre;

	// 指定半稜線とペア半稜線を削除する
	delete edgDel;
	delete edgOtr;
}

//	面分(fce1)のループを面分(fce2)の外周ループ(先頭ループ)の次に追加し
//	面分(fce1)のループと面分(fce1)を削除する
//
//	・面分(fce1)のループを面分(fce2)のループに変更する
//	・面分(fce2)のループのリンクの先頭の次に面分(fce1)のループ群を付ける
//	・付け加えたループ群の後に面分(fce2)のループの２番目以降を付ける
//	・面分(fce1)のループを切る		必要???
//	・面分(fce1)を削除する
//
void MgSolid::add_MgsFaceo(MgsFace* fce1, MgsFace* fce2) 
{ 
	MgsLoop* lop;
	MgsLoop* lopNxt;
	MgsLoop* last;

	assert( fce1 != fce2);
	// 面分(fce1)のループを面分(fce2)のループに変更する
	for( lop = fce1->m_lop; lop; lop = lop->m_nxt)
		lop->m_fce = fce2;

	// 面分(fce2)のループのリンクの先頭の次に面分(fce1)のループ群を付ける
	lopNxt = fce2->m_lop->m_nxt;
	fce2->m_lop->m_nxt = fce1->m_lop;
	fce1->m_lop->m_pre = fce2->m_lop;

	// 付け加えたループ群の後に面分(fce2)のループの２番目以降を付ける
	last = fce1->m_lop;
	while( last->m_nxt)
		last = last->m_nxt;

	last->m_nxt = lopNxt;
	if( lopNxt)
		lopNxt->m_pre = last;

	// 面分(fce1)のループを切る		必要???
	fce1->m_lop = NULL;

	// 面分(fce1)を削除する
	del_face( fce1);
}

//	ソリッドの全頂点の座標をベクトル(vct)方向に移動する
//
MgSolid& MgSolid::operator += (MgsVect const& vct)
{
	MgsVertex* vtxSrc;

	for( vtxSrc = m_vtxfirst; vtxSrc; vtxSrc = vtxSrc->m_nxt) {
		vtxSrc->m_pt += vct;
	}
	return *this;
}


}//end namespace euler


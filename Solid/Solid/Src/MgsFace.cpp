//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgsFase.cpp
//
//		処理	面分の消去、ループの作成、消去
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

//	面分のデストラクタ
//
//	構成ループを全て削除する
//	面分のIdがカレントId-1の場合(最終Idの場合)はカレントId--;
//
MgsFace::~MgsFace()
{
	while( m_lop) {
		MgsLoop* lopDel = m_lop;
		m_lop = m_lop->m_nxt;
		delete lopDel;
	}
	if( m_id == m_cur_id - 1)
		m_cur_id--; 
}

MgsLoop* MgsFace::new_loop()
{
	return new MgsLoop( this);
}

MgsLoop* MgsFace::new_loop( MgsVertex *v)
{
	return new MgsLoop( this, v);
}
	
//	指定ループ(lopDel)を削除する
//
//	面分からのリンクの先頭のループである場合は面分のリンクを次のループに設定する
//	指定ループ(lopDel)前後のループのリンクを詰め、指定ループ(lopDel)を削除する
//
void MgsFace::del_loop( MgsLoop* lopDel)
{
	if( m_lop == lopDel)					// 面分からのリンクの先頭ループの場合は
		m_lop = lopDel->m_nxt;  			// 面分のリンクを次のループに設定する
	if( lopDel->m_pre)
		lopDel->m_pre->m_nxt = lopDel->m_nxt;  
	if( lopDel->m_nxt)
		lopDel->m_nxt->m_pre = lopDel->m_pre; 
	delete lopDel;
}
 


}//end namespace euler


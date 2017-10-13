//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
// MtPrcEventQueue.cpp
// 
//	機能	コマンドキュー管理
// 
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MCad.h" /*DNT*/
#include "MCadApi.h"
#include "MtPrcEvent.h"
#include "MbList.h"
#include "MtPrcEventQueue.h" /*DNT*/

namespace MC
{

//	列の全てのコマンドを消去する（属性領域も含む）
void MtPrcEventQueue::Flush( void)
{
	SYNCHRONIZEMETHOD();

	MtPrcEvent*	pRb;
	MPOSITION	pos1;
	
	// 全レコードの項目の属性領域を開放する
	for (pRb=(MtPrcEvent*)GetHead( &pos1); pRb!=0; pRb=(MtPrcEvent*)GetNext( &pos1)) {
		if ( pRb)
			pRb->Free();
	}
	RemoveAll();												// キューの全レコードを消す
}

//	列の先頭のコマンドを消去する（属性領域も含む）
void MtPrcEventQueue::FreeNext(void) 
{
	SYNCHRONIZEMETHOD();

	MtPrcEvent	Rb;

	Pop( &Rb);													// 先頭のコマンドを取り出しキューから開放する
	Rb.Free();
}

//	列の先頭のコマンドを消去し（属性領域も含む）次のコマンドを取得する
MtPrcEvent* MtPrcEventQueue::AdvanceNext( void)
{
	SYNCHRONIZEMETHOD();

	FreeNext();

	return this->PeekItem();
}

//	列の末尾にコマンドを追加する(属性項目はポインタのみコピーしメモリ領域をそのまま引き継ぐ)
void MtPrcEventQueue::AddItem( MtPrcEvent *pItem)
{
	SYNCHRONIZEMETHOD();

	Inject( pItem);
}

//	列の先頭にコマンドを追加する(属性項目はポインタのみコピーしメモリ領域をそのまま引き継ぐ)
void MtPrcEventQueue::AddItemToFront( MtPrcEvent *pItem)
{
	SYNCHRONIZEMETHOD();

	Push( pItem);
}

//	列の末尾に他の列を追加する(注：属性項目はポインタのみコピーしメモリ領域をそのまま引き継ぐ)
void MtPrcEventQueue::AddQueue( MtPrcEventQueue *pOther)
{
	SYNCHRONIZEMETHOD();

	ASSERT( pOther != this);  // maybe we should allow to concat to itself, but....

	CNode* pNode = pOther->GetFirst();							// start at Head
	for (; pNode != &(pOther->m_sStart); pNode = pNode->pNext)
		Push( (MtPrcEvent*)&(pNode->pEnt));
}

//	列の先頭のコマンドを取り出し、列から消去する(注：属性項目はポインタのみコピーしメモリ領域をそのまま引き継ぐ)
void MtPrcEventQueue::PopItem( MtPrcEvent* pRb)
{
	SYNCHRONIZEMETHOD();

	Pop( pRb);
}

bool MtPrcEventQueue::IsCommandPause( void)
{
	SYNCHRONIZEMETHOD();

	MPOSITION	Pos;
	MtPrcEvent*	pRb;

	bool bRv = false;
	if ( (pRb = (MtPrcEvent*)GetHead( &Pos)) != 0)
		if ( (pRb->m_restype == MTRT_MENUCOMMAND) && (pRb->m_resval.rstring[0] == '\\'))
			bRv = true;
	return bRv;
}

} // namespace MC
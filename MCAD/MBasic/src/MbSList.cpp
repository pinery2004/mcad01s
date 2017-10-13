//==========================================================================================
//  Copyright ( C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MbSList.h
//
//		双方向リスト　実体リンク型	同期制御付き
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"

#include "MbSynchroObj.h"

#define	 DLL_EXPORT_SLIST_DO
#include "MbSList.h"

/******************************************************************************************************************
  MbSListO のメソッド 実体リンク型
******************************************************************************************************************/

//////////////////////////////////////////////////////////////
//	リストの新規作成　コンストラクタ
MbSListO::MbSListO(
					MINT	i_nBlockSize		// 1ブロックあたりのノード数
				)
{
	m_sStart.pEnt = NULL;
	m_sStart.pNext = &m_sStart;
	m_sStart.pPrev = &m_sStart;
	m_nCount = 0;

	m_nRecSize = sizeof( CNode);				// ノードサイズ

	m_pNodeFree = NULL;
	m_pBlocks = NULL;
	m_nBlockSize = i_nBlockSize;
}

MbSListO::~MbSListO( )
{
	RemoveAll( );
}
//////////////////////////////////////////////////////////////
//	リストの先頭に実体を追加する
MPOSITION MbSListO::Push(
					MOBJECT*	i_pEnt			// エンティティへのポインタ 
				)
{
	SYNCHRONIZEMETHOD( );
	return ( MPOSITION) NewNode( i_pEnt, &m_sStart, GetFirst( ));
}

//////////////////////////////////////////////////////////////
//	リストの先頭から実体を取り出す（リストの先頭のノードは削除、
//									レコードタイプ＝０の時は実体へのポインタを返し、実体は残す
//									レコードタイプ≠０の時は実体を返す）
MOBJECT* MbSListO::Pop( )
{
	SYNCHRONIZEMETHOD( );
	CNode*		pNode = GetFirst();
	MOBJECT*	ret = pNode->pEnt;
	if ( pNode != &m_sStart)
		FreeNode( pNode);
	return	ret;
}

//////////////////////////////////////////////////////////////
//	リストの末尾に実体を追加する
MPOSITION MbSListO::Inject(
					MOBJECT*	i_pEnt			// エンティティへのポインタ
				)
{
	SYNCHRONIZEMETHOD( );
 	return ( MPOSITION) NewNode( i_pEnt, GetLast( ), &m_sStart);
}

//////////////////////////////////////////////////////////////
//	リストの末尾から実体を取り出す（リストの末尾のノードは削除、
//									レコードタイプ＝０の時は実体へのポインタを返し、実体は残す
//									レコードタイプ≠０の時は実体を返す）
MOBJECT* MbSListO::Eject( )
{
	SYNCHRONIZEMETHOD( );
	CNode*	pNode = GetLast( );
	MOBJECT* ret = pNode->pEnt;
	if ( pNode != &m_sStart)
		FreeNode( pNode);
	return	ret;
}

//////////////////////////////////////////////////////////////
//	指定ノード位置の直前に実体を追加する
void MbSListO::InsertBefore(
					MOBJECT*	i_pEnt,			// エンティティへのポインタ
					MPOSITION*	i_pPosition		// ノード位置
				)
{
	SYNCHRONIZEMETHOD( );
	NewNode( i_pEnt, ( ( CNode*)*i_pPosition)->pPrev, ( ( CNode*)*i_pPosition));
}

//////////////////////////////////////////////////////////////
//	指定ノード位置の直後に実体を追加する
void MbSListO::InsertAfter(
					MOBJECT*	i_pEnt,			// エンティティへのポインタ
					MPOSITION*	i_pPosition		// ノード位置
				)
{
	SYNCHRONIZEMETHOD( );
	NewNode( i_pEnt, ( CNode*)*i_pPosition, ( ( CNode*)*i_pPosition)->pNext);
}

//////////////////////////////////////////////////////////////
//	指定ノード位置のエンティティを削除し、直前のノード位置を返す
void MbSListO::RemoveAt(
					MPOSITION*	io_pPosition	// (in)削除ノード位置、(out)直前のノード位置
				)
{
	SYNCHRONIZEMETHOD( );
	CNode* pNode = ( CNode*) *io_pPosition;
	*io_pPosition = ( MPOSITION) pNode->pPrev;
	MBDELETE( pNode->pEnt);
	FreeNode( pNode);
}

//////////////////////////////////////////////////////////////
//	全ノード位置のエンティティを削除する
void MbSListO::RemoveAll( )
{
	SYNCHRONIZEMETHOD( );
	CNode*	pNode;
	CNode*	pNext;
	for ( pNode = GetFirst( ); pNode != &m_sStart; pNode = pNext) {
		pNext = pNode->pNext;
		MBDELETE( pNode->pEnt);
	}
	m_nCount = 0;
	m_sStart.pNext = &m_sStart;
	m_sStart.pPrev = &m_sStart;

	m_pBlocks->FreeDataChain( );
	m_pBlocks = NULL;
	m_pNodeFree = NULL;
}

/////////////////////////////////////////////////////////////////////////////
//		先頭または指定ノードの次のノードよりエンティティを検索する
MPOSITION MbSListO::FindIndex(
					MINT		i_nIndex			// 
				)
{
	SYNCHRONIZEMETHOD( );
	CNode*	pNode;
	_ASSERTE( this);

	if ( i_nIndex >= m_nCount || i_nIndex < 0)
		return NULL;											// went too far

	pNode = &m_sStart;
	while ( i_nIndex--)
	{
		pNode = pNode->pNext;
	}
	return ( MPOSITION) pNode;
}

/////////////////////////////////////////////////////////////////////////////
//		先頭または指定ノードの次のノードよりエンティティを検索する
MPOSITION MbSListO::Find(
					MOBJECT*	i_pSrchEnt,		// 検索エンティティ
					MPOSITION	i_startAfter	// 検索直前のノード位置　=0:先頭ノード、!=0:指定ノード
				)
{
	SYNCHRONIZEMETHOD( );
	_ASSERTE( this);

	CNode* pNode = ( CNode*) i_startAfter;
	if ( pNode == NULL)
	{
		pNode = GetFirst( );									// 先頭ノード
	}
	else
	{
		_ASSERTE( pNode);
		pNode = pNode->pNext;									// 指定ノードの次のノード
	}

	for ( ; pNode != &m_sStart; pNode = pNode->pNext)
		if ( pNode->pEnt == i_pSrchEnt)
			return ( MPOSITION) pNode;
	return NULL;
}

//////////////////////////////////////////////////////////////
//	ノードの挿入
MbSListO::CNode*
MbSListO::NewNode(
					MOBJECT*	i_pEnt,			// 
					CNode*		i_pPrev,		// 
					CNode*		i_pNext			// 
				)
{
	if ( m_pNodeFree == NULL) {
		// 空きノードがない場合は、ノードブロックを追加
		MbPlex* pNewBlock = MbPlex::Create( m_pBlocks, m_nBlockSize, m_nRecSize);
		// chain them into free list
		CNode* pNode = ( CNode*) pNewBlock->data( );
		// free in reverse order to make it easier to debug
		pNode = ( CNode*)( ( char*)pNode + m_nRecSize * ( m_nBlockSize - 1));
		for ( MINT i = m_nBlockSize-1; i >= 0; i--)
		{
			pNode->pNext = m_pNodeFree;
			m_pNodeFree = pNode;
			pNode = ( CNode*)( ( char*)pNode - m_nRecSize);
		}
	}
	_ASSERTE( m_pNodeFree);  // we must have something

	CNode*	pNode = m_pNodeFree;
	m_pNodeFree = m_pNodeFree->pNext;
	
	pNode->pEnt = i_pEnt;

	pNode->pPrev = i_pPrev;
	pNode->pNext = i_pNext;
	i_pPrev->pNext = pNode;
	i_pNext->pPrev = pNode;
	
	m_nCount++;
	return pNode;
}

//////////////////////////////////////////////////////////////
//	ノードの開放
void	MbSListO::FreeNode(
					CNode*		i_pNode
				)
{
//	_ASSERTE( pNode != 0);
	i_pNode->pPrev->pNext = i_pNode->pNext;
	i_pNode->pNext->pPrev = i_pNode->pPrev;

	i_pNode->pNext = m_pNodeFree;
	m_pNodeFree = i_pNode;
	
	m_nCount--;
	_ASSERTE( m_nCount >= 0);  // make sure we don't underflow

	// if no more elements, cleanup completely
	if ( m_nCount == 0)
		RemoveAll( );
}
//==========================================================================================
//  Copyright ( C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: Mblist.h
//
//		双方向リスト	実体組込型
//
//  Modification History
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================

/******************************************************************************************************************
  MbSListE のメソッド
******************************************************************************************************************/

MbSListE::MbSListE(
					MINT	i_nBlockSize,		// 1ブロックあたりのノード数
					MINT	i_nRecSize			// レコードサイズ
				)
{
	m_sStart.pEnt = NULL;
	m_sStart.pNext = &m_sStart;
	m_sStart.pPrev = &m_sStart;
	m_nCount = 0;

	m_nEntType = i_nRecSize;
	m_nRecSize = sizeof( CNode) + ( i_nRecSize - sizeof( char*));	// ノードサイズ

	m_pNodeFree = NULL;
	m_pBlocks = NULL;
	m_nBlockSize = i_nBlockSize;
}

MbSListE::~MbSListE( )
{
	RemoveAll( );
}
//////////////////////////////////////////////////////////////
//	リストの先頭に実体を追加する
MPOSITION MbSListE::Push(
					MOBJECT*	i_pEnt			// エンティティへのポインタ
				)
{
	SYNCHRONIZEMETHOD( );
	return ( MPOSITION) NewNode( i_pEnt, &m_sStart, GetFirst( ));
}

//////////////////////////////////////////////////////////////
//	リストの先頭から実体を取り出す（リストの先頭のノードは削除、
//									レコードタイプ＝０の時は実体へのポインタを返し、実体は残す
//									レコードタイプ≠０の時は実体を返す）
MINT MbSListE::Pop(
					MOBJECT*	o_pEnt			// エンティティへのポインタ
				)
{
	SYNCHRONIZEMETHOD( );
	CNode*	pNode = GetFirst( );
	if ( pNode == &m_sStart)
		return 0;
	memcpy( o_pEnt, &( pNode->pEnt), m_nEntType);

	if ( pNode != &m_sStart)
		FreeNode( pNode);
	return 1;
}

//////////////////////////////////////////////////////////////
//	リストの末尾に実体を追加する
MPOSITION MbSListE::Inject(
					MOBJECT*	i_pEnt			// エンティティへのポインタ
				)
{
	SYNCHRONIZEMETHOD( );
 	return ( MPOSITION) NewNode( i_pEnt, GetLast( ), &m_sStart);
}

//////////////////////////////////////////////////////////////
//	リストの末尾から実体を取り出す（リストの末尾のノードは削除、
//									レコードタイプ＝０の時は実体へのポインタを返し、実体は残す
//									レコードタイプ≠０の時は実体を返す）
MINT MbSListE::Eject(
					MOBJECT*	o_pEnt			// エンティティへのポインタ
				)
{
	SYNCHRONIZEMETHOD( );
	CNode*	pNode = GetLast( );
	if ( pNode == &m_sStart)
		return 0;
	memcpy( o_pEnt, &( pNode->pEnt), m_nEntType);

	if ( pNode != &m_sStart)
		FreeNode( pNode);
	return 1;
}

//////////////////////////////////////////////////////////////
//	指定ノードの直前に実体を追加する
void MbSListE::InsertBefore(
					MOBJECT*	i_pEnt,			// エンティティへのポインタ
					MPOSITION*	i_pPosition		// ノード位置
				)
{
	SYNCHRONIZEMETHOD( );
	NewNode( i_pEnt, ( ( CNode*)*i_pPosition)->pPrev, ( ( CNode*)*i_pPosition));
}

//////////////////////////////////////////////////////////////
//	指定ノードの直後に実体を追加する
void MbSListE::InsertAfter(
					MOBJECT*	i_pEnt,			// エンティティへのポインタ
					MPOSITION*	i_pPosition		// ノード位置
				)
{
	SYNCHRONIZEMETHOD( );
	NewNode( i_pEnt, ( CNode*)*i_pPosition, ( ( CNode*)*i_pPosition)->pNext);
}

//////////////////////////////////////////////////////////////
//	指定ノード位置のエンティティを削除し、直前のノード位置を返す
void MbSListE::RemoveAt(
					MPOSITION*	i_pPosition		// (in)削除ノード位置、(out)直前のノード位置
				)
{
	SYNCHRONIZEMETHOD( );
	_ASSERTE( *i_pPosition);
	CNode* pNode = ( CNode*) *i_pPosition;
	*i_pPosition = ( MPOSITION) pNode->pPrev;
	FreeNode( pNode);
}

//////////////////////////////////////////////////////////////
//	全ノード位置のエンティティを削除する
void MbSListE::RemoveAll( )
{
	SYNCHRONIZEMETHOD( );
	m_nCount = 0;
	m_sStart.pNext = &m_sStart;
	m_sStart.pPrev = &m_sStart;

	m_pBlocks->FreeDataChain( );
	m_pBlocks = NULL;
	m_pNodeFree = NULL;
}

/////////////////////////////////////////////////////////////////////////////
//		指定ＩＤのエンティティを得る
MPOSITION MbSListE::FindIndex(
					MINT		i_nIndex
				)
{
	SYNCHRONIZEMETHOD( );
	CNode*	pNode;
	_ASSERTE( this);

	if ( i_nIndex >= m_nCount || i_nIndex < 0)
		return NULL;											// went too far

	pNode = &m_sStart;
	while ( i_nIndex--)
	{
		pNode = pNode->pNext;
	}
	return ( MPOSITION) pNode;
}

/////////////////////////////////////////////////////////////////////////////
//		先頭または指定ノードの次のノードよりエンティティを検索する
MPOSITION MbSListE::Find(
					MOBJECT*	i_pSrchEnt,		// 検索エンティティ
					MPOSITION	i_startAfter	// 検索直前のノード位置　=0:先頭ノード、!=0:指定ノード
				)
{
	SYNCHRONIZEMETHOD( );
	_ASSERTE( this);

	CNode* pNode = ( CNode*) i_startAfter;
	if ( pNode == NULL)
	{
		pNode = GetFirst( );									// 先頭ノード
	}
	else
	{
		_ASSERTE( pNode);
		pNode = pNode->pNext;									// 指定ノードの次のノード
	}

	for ( ; pNode != &m_sStart; pNode = pNode->pNext)
		if ( ( MOBJECT*)&( pNode->pEnt) == i_pSrchEnt)
			return ( MPOSITION) pNode;
	return NULL;
}

//////////////////////////////////////////////////////////////
//	エンティティノードの挿入
MbSListE::CNode*
MbSListE::NewNode(
					MOBJECT*	i_pEnt,
					CNode*		i_pPrev,
					CNode*		i_pNext
				)
{
	SYNCHRONIZEMETHOD( );
	if ( m_pNodeFree == NULL) {
		// 空きノードがない場合は、ノードブロックを追加
		MbPlex* pNewBlock = MbPlex::Create( m_pBlocks, m_nBlockSize, m_nRecSize);
		// chain them into free list
		CNode* pNode = ( CNode*) pNewBlock->data( );
		// free in reverse order to make it easier to debug
		pNode = ( CNode*)( ( char*)pNode + m_nRecSize * ( m_nBlockSize - 1));
		for ( MINT i = m_nBlockSize-1; i >= 0; i--)
		{
			pNode->pNext = m_pNodeFree;
			m_pNodeFree = pNode;
			pNode = ( CNode*)( ( char*)pNode - m_nRecSize);
		}
	}
	_ASSERTE( m_pNodeFree);  // we must have something

	CNode*	pNode = m_pNodeFree;
	m_pNodeFree = m_pNodeFree->pNext;
	
	memcpy( &( pNode->pEnt), i_pEnt, m_nEntType);

	pNode->pPrev = i_pPrev;
	pNode->pNext = i_pNext;
	i_pPrev->pNext = pNode;
	i_pNext->pPrev = pNode;
	
	m_nCount++;
	return pNode;
}

//////////////////////////////////////////////////////////////
//	ノードの開放
void	MbSListE::FreeNode(
					CNode*		i_pNode
				)
{
	SYNCHRONIZEMETHOD( );
//	_ASSERTE( pNode != 0);
	i_pNode->pPrev->pNext = i_pNode->pNext;
	i_pNode->pNext->pPrev = i_pNode->pPrev;

	i_pNode->pNext = m_pNodeFree;
	m_pNodeFree = i_pNode;
	
	m_nCount--;
	_ASSERTE( m_nCount >= 0);  // make sure we don't underflow

	// if no more elements, cleanup completely
	if ( m_nCount == 0)
		RemoveAll( );
}

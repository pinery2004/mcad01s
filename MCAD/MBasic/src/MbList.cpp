//==========================================================================================
//  Copyright ( C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: Mblist.h
//
//		双方向リスト　実体リンク型
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"

#define	 DLL_EXPORT_LIST_DO
#include "MbList.h"

/******************************************************************************************************************
  MbListO のメソッド 実体リンク型
******************************************************************************************************************/

//////////////////////////////////////////////////////////////
//	リストの新規作成　コンストラクタ
MbListO::MbListO(
					MINT	i_nBlockSize		// 1ブロックあたりのノード数
				)
{
	m_sStart.pEnt = NULL;
	m_sStart.pNext = &m_sStart;
	m_sStart.pPrev = &m_sStart;
	m_nCount = 0;

	m_nRecSize = sizeof( CNode);

	m_pNodeFree = NULL;
	m_pBlocks = NULL;
	m_nBlockSize = i_nBlockSize;
}

MbListO::~MbListO( )
{
	RemoveAll( );
}
//////////////////////////////////////////////////////////////
//	リストの先頭に実体を追加する
MPOSITION MbListO::Push(
					MOBJECT*	i_pEnt			// エンティティへのポインタ 
				)
{
	return ( MPOSITION) NewNode( i_pEnt, &m_sStart, GetFirst( ));
}

//////////////////////////////////////////////////////////////
//	リストの先頭から実体を取り出す（リストの先頭のノードは削除、
//									レコードタイプ＝０の時は実体へのポインタを返し、実体は残す
//									レコードタイプ≠０の時は実体を返す）
MOBJECT* MbListO::Pop( )
{
	CNode*		pNode = m_sStart.pNext;
	MOBJECT*	ret = pNode->pEnt;
	if ( pNode != &m_sStart)
		FreeNode( pNode);
	return	ret;
}

//////////////////////////////////////////////////////////////
//	リストの末尾に実体を追加する
MPOSITION MbListO::Inject(
					MOBJECT*	i_pEnt			// エンティティへのポインタ
				)
{
 	return ( MPOSITION) NewNode( i_pEnt, GetLast( ), &m_sStart);
}

//////////////////////////////////////////////////////////////
//	リストの末尾から実体を取り出す（リストの先頭のノードは削除、
//									レコードタイプ＝０の時は実体へのポインタを返し、実体は残す
//									レコードタイプ≠０の時は実体を返す）
MOBJECT* MbListO::Eject( )
{
	CNode*	pNode = GetLast( );
	MOBJECT* ret = pNode->pEnt;
	if ( pNode != &m_sStart)
		FreeNode( pNode);
	return	ret;
}

//////////////////////////////////////////////////////////////
//	指定ノードの直前に実体を追加する
void MbListO::InsertBefore(
					MOBJECT*	i_pEnt,			// エンティティへのポインタ
					MPOSITION*	io_pPosition	// ノード位置
				)
{
	NewNode( i_pEnt, ( ( CNode*)*io_pPosition)->pPrev, ( ( CNode*)*io_pPosition));
}

//////////////////////////////////////////////////////////////
//	指定ノードの直後に実体を追加する
void MbListO::InsertAfter(
					MOBJECT*	i_pEnt,			// エンティティへのポインタ
					MPOSITION*	io_pPosition	// ノード位置
				)
{
	NewNode( i_pEnt, ( CNode*)*io_pPosition, ( ( CNode*)*io_pPosition)->pNext);
}

//////////////////////////////////////////////////////////////
//	指定ノード位置のエンティティを削除し、直前のノード位置を返す
void MbListO::RemoveAt(
					MPOSITION*	io_pPosition	// (in)削除ノード位置、(out)直前のノード位置
				)
{
	CNode* pNode = ( CNode*) *io_pPosition;
	*io_pPosition = ( MPOSITION) pNode->pPrev;
	MBDELETE( pNode->pEnt);
	FreeNode( pNode);
}

//////////////////////////////////////////////////////////////
//	全ノード位置のエンティティを削除する
void MbListO::RemoveAll( )
{
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
MPOSITION MbListO::FindIndex(
					MINT		i_nIndex			// 
				)
{
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
MPOSITION MbListO::Find(
					MOBJECT*	i_pSrchEnt,		// 検索エンティティ
					MPOSITION	i_startAfter	// 検索直前のノード位置　=0:先頭ノード、!=0:指定ノード
				)
{
	_ASSERTE( this);

	CNode* pNode = ( CNode*) i_startAfter;
	if ( pNode == NULL)
	{
		pNode = GetFirst( );										// start at head
	}
	else
	{
		_ASSERTE( pNode);
		pNode = pNode->pNext;									// start after the one specified
	}

	for ( ; pNode != &m_sStart; pNode = pNode->pNext)
		if ( pNode->pEnt == i_pSrchEnt)
			return ( MPOSITION) pNode;
	return NULL;
}

//////////////////////////////////////////////////////////////
//	ノードの挿入
MbListO::CNode*
MbListO::NewNode(
					MOBJECT*	i_pEnt,			// 
					CNode*		io_pPrev,		// 
					CNode*		io_pNext		// 
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

	pNode->pPrev = io_pPrev;
	pNode->pNext = io_pNext;
	io_pPrev->pNext = pNode;
	io_pNext->pPrev = pNode;
	
	m_nCount++;
	return pNode;
}

//////////////////////////////////////////////////////////////
//	ノードの開放
void	MbListO::FreeNode(
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
  MbListE のメソッド
******************************************************************************************************************/

MbListE::MbListE(
					MINT	i_nBlockSize,		// 1ブロックあたりのノード数
					MINT	i_nRecSize			// レコードサイズ
				)
{
	m_sStart.pEnt = NULL;
	m_sStart.pNext = &m_sStart;
	m_sStart.pPrev = &m_sStart;
	m_nCount = 0;

	m_nEntType = i_nRecSize;
	m_nRecSize = sizeof( CNode) + ( i_nRecSize - sizeof( char*));

	m_pNodeFree = NULL;
	m_pBlocks = NULL;
	m_nBlockSize = i_nBlockSize;
}

MbListE::~MbListE( )
{
	RemoveAll( );
}
//////////////////////////////////////////////////////////////
//	リストの先頭に実体を追加する
MPOSITION MbListE::Push(
					MOBJECT*	i_pEnt			// エンティティへのポインタ
				)
{
	return ( MPOSITION) NewNode( i_pEnt, &m_sStart, GetFirst( ));
}

//////////////////////////////////////////////////////////////
//	リストの先頭から実体を取り出す（リストの先頭のノードは削除、
//									レコードタイプ＝０の時は実体へのポインタを返し、実体は残す
//									レコードタイプ≠０の時は実体を返す）
MINT MbListE::Pop(
					MOBJECT*	o_pEnt			// エンティティへのポインタ
				)
{
	CNode*	pNode = GetFirst( );
	if ( pNode == &m_sStart)
		return 0;
	memcpy( o_pEnt, &pNode->pEnt, m_nEntType);

	if ( pNode != &m_sStart)
		FreeNode( pNode);
	return 1;
}

//////////////////////////////////////////////////////////////
//	リストの末尾に実体を追加する
MPOSITION MbListE::Inject(
					MOBJECT*	i_pEnt			// エンティティへのポインタ
				)
{
 	return ( MPOSITION) NewNode( i_pEnt, GetLast( ), &m_sStart);
}

//////////////////////////////////////////////////////////////
//	リストの末尾から実体を取り出す（リストの先頭のノードは削除、
//									レコードタイプ＝０の時は実体へのポインタを返し、実体は残す
//									レコードタイプ≠０の時は実体を返す）
MINT MbListE::Eject(
					MOBJECT*	o_pEnt			// エンティティへのポインタ
				)
{
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
void MbListE::InsertBefore(
					MOBJECT*	i_pEnt,			// エンティティへのポインタ
					MPOSITION*	io_pPosition	// ノード位置
				)
{
	NewNode( i_pEnt, ( ( CNode*)*io_pPosition)->pPrev, ( ( CNode*)*io_pPosition));
}

//////////////////////////////////////////////////////////////
//	指定ノードの直後に実体を追加する
void MbListE::InsertAfter(
					MOBJECT*	i_pEnt,			// エンティティへのポインタ
					MPOSITION*	io_pPosition	// ノード位置
				)
{
	NewNode( i_pEnt, ( CNode*)*io_pPosition, ( ( CNode*)*io_pPosition)->pNext);
}

//////////////////////////////////////////////////////////////
//	指定ノード位置のエンティティを削除し、直前のノード位置を返す
void MbListE::RemoveAt(
					MPOSITION*	io_pPosition	// (in)削除ノード位置、(out)直前のノード位置
				)
{
	_ASSERTE( *io_pPosition);
	CNode* pNode = ( CNode*) *io_pPosition;
	*io_pPosition = ( MPOSITION) pNode->pPrev;
	FreeNode( pNode);
}

//////////////////////////////////////////////////////////////
//	全ノード位置のエンティティを削除する
void MbListE::RemoveAll( )
{
	m_nCount = 0;
	m_sStart.pNext = &m_sStart;
	m_sStart.pPrev = &m_sStart;

	m_pBlocks->FreeDataChain( );
	m_pBlocks = NULL;
	m_pNodeFree = NULL;
}

/////////////////////////////////////////////////////////////////////////////
//		指定ＩＤのエンティティを得る
MPOSITION MbListE::FindIndex(
					MINT		i_nIndex
				)
{
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
MPOSITION MbListE::Find(
					MOBJECT*	i_pSrchEnt,		// 検索エンティティ
					MPOSITION	i_startAfter	// 検索直前のノード位置　=0:先頭ノード、!=0:指定ノード
				)
{
	_ASSERTE( this);

	CNode* pNode = ( CNode*) i_startAfter;
	if ( pNode == NULL)
	{
		pNode = GetFirst( );										// start at head
	}
	else
	{
		_ASSERTE( pNode);
		pNode = pNode->pNext;									// start after the one specified
	}

	for ( ; pNode != &m_sStart; pNode = pNode->pNext)
		if ( ( MOBJECT*)&( pNode->pEnt) == i_pSrchEnt)
			return ( MPOSITION) pNode;
	return NULL;
}

//////////////////////////////////////////////////////////////
//	エンティティノードの挿入
MbListE::CNode*
MbListE::NewNode(
					MOBJECT*	i_pEnt,
					CNode*		io_pPrev,
					CNode*		io_pNext
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
	
	memcpy( &( pNode->pEnt), i_pEnt, m_nEntType);

	pNode->pPrev = io_pPrev;
	pNode->pNext = io_pNext;
	io_pPrev->pNext = pNode;
	io_pNext->pPrev = pNode;
	
	m_nCount++;
	return pNode;
}

//////////////////////////////////////////////////////////////
//	ノードの開放
void	MbListE::FreeNode(
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


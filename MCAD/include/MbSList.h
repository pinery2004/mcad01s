#pragma once
//==========================================================================================
//  Copyright ( C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MbSlist.h
//
//		同期型の双方向リスト
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "MbPlex.h"

#ifdef DLL_EXPORT
	#undef DLL_EXPORT
#endif

#ifdef DLL_EXPORT_SLIST_DO
	#define DLL_EXPORT							__declspec( dllexport)
#else
//	#define DLL_EXPORT							__declspec( dllimport)
	#define DLL_EXPORT
#endif

/////////////////////////////////////////////////////////////////////////////
//
// 双方向リスト 実体リンク型
//

class DLL_EXPORT MbSListO : public MbSynchroObj
{
protected:
	struct CNode
	{
		CNode*		pNext;							// 次の実体へのリンク
		CNode*		pPrev;							// 前の実体へのリンク
		MOBJECT*	pEnt;						// 実体へのポインタ
	};
public:
	CNode		m_sStart;							// 基点
	MINT		m_nCount;							// 実体数

	CNode*		m_pNodeFree;						// 開放ノードの基点
	MINT		m_nRecSize;							// レコードサイズ( byte)

	MbPlex*		m_pBlocks;							// ノード確保領域
	MINT		m_nBlockSize;						// ノード確保単位

	// 初めのリンクの取り出し	 return			  : 最初のリンク
	CNode*		GetFirst ( )
						{ return m_sStart.pNext;}

	// 最後のリンクの取り出し	 return			  : 最後のリンク
	CNode*		GetLast  ( )
						{ return m_sStart.pPrev;}

public:

	// コンストラクタ
	MbSListO( MINT nBlockSize);

	// コピーコンストラクタ
//	MbSListO( MbSListO& List);
	
	// デストラクタ
	~MbSListO( );

// Attributes ( head and tail)
	// count of elements
	// リストが空かどうか							return: True: 空, False: 実体がある
	bool 		IsEmpty( )
						{ return m_nCount == 0;}
	bool 		IsNotEmpty( )
						{ return m_nCount != 0;}
	// リストの実体数を得る							return: 実体の数
	MINT		GetCount( )
						{ return m_nCount;}
	MINT		GetSize( )
						{ return m_nCount;}
	
	// peek at head or tail
	// 先頭の実体を得る								return  : 先頭実体のポインタ、エラーなら０
	MOBJECT*		GetHead( MPOSITION* pPosition)
						{
							SYNCHRONIZEMETHOD( );
							CNode* pNode = m_sStart.pNext;
							*pPosition = ( MPOSITION) pNode;
							if ( pNode == &m_sStart)
								return 0;
							return pNode->pEnt;
						}
	// 末尾の実体を得る								return  : 末尾実体のポインタ、エラーなら０
	MOBJECT*	GetTail( MPOSITION* pPosition)
						{
							SYNCHRONIZEMETHOD( );
							CNode* pNode = m_sStart.pPrev;
							*pPosition = ( MPOSITION) pNode;
							if ( pNode == &m_sStart)
								return 0;
							return pNode->pEnt;
						}

// Operations
	// リストの先頭に実体を入れる						pEnt	: 挿入する実体
	MPOSITION	Push( MOBJECT*  pEnt);
	MPOSITION	Push( class MbListO*  pListO);
	MPOSITION 	AddHead( MOBJECT*  pEnt)
						{ return Push( pEnt);}
	// リストの先頭の実体を取り出しノードを消去する		return	: 0: 実体無し　≠0:実体のポインタ
	MOBJECT* Pop( );
	MOBJECT* RemoveHead( )
						{ return Pop( );}

	// リストの末尾に実体を入れる						pEnt	: 挿入する実体
	MPOSITION	Inject( MOBJECT*  pEnt);
	MPOSITION	Inject( class MbListO*  pListO);
	MPOSITION 	AddTail( MOBJECT*  pEnt)
						{ return Inject( pEnt);}
	// リストの末尾の実体を取り出しノードを消去する		return	: 0: 実体無し　≠0:実体のポインタ
	MOBJECT*	Eject( );
	MOBJECT*	RemoveTail( )
						{ return Eject( );}

	// remove all elements
	// すべての実体を削除する
	void		RemoveAll( );

	// iteration
	// 先頭のノード位置を得る
	MPOSITION 	GetHeadPosition( )
						{ return ( MPOSITION) m_sStart.pNext;}

	// 末尾のノード位置を得る
	MPOSITION 	GetTailPosition( )
						{ return ( MPOSITION) m_sStart.pPrev;}

	// ノード位置を進め、次の実体を得る					return: 実体のポインタ、最終より後なら０
	MOBJECT*	GetNext( MPOSITION* pPosition)			// ( i O) pPosition: ノード位置
						{
							SYNCHRONIZEMETHOD( );
							*pPosition = ( MPOSITION) ( ( CNode*) *pPosition)->pNext;
							if ( *pPosition == ( MPOSITION) &m_sStart) 
								return NULL;
							return ( ( CNode*) *pPosition)->pEnt;
						}

	// ノード位置を戻し、手前の実体を得る				return: 実体のポインタ、先頭より前なら０
	MOBJECT*	GetPrev( MPOSITION* pPosition)			// ( i O) pPosition: 次のノード位置
						{
							SYNCHRONIZEMETHOD( );
							*pPosition = ( MPOSITION) ( ( CNode*) *pPosition)->pPrev;
							if ( *pPosition == ( MPOSITION) &m_sStart) 
								return NULL;
							return ( ( CNode*) *pPosition)->pEnt;
						}

	// 指定位置の実体を得る								return	: 0: 実体無し　≠0:実体のポインタ
	MOBJECT*	GetAt( MPOSITION* pPosition)
						{ return ( ( CNode*) *pPosition)->pEnt;}

	// 指定位置に実体を書き込む
	void 		SetAt( MPOSITION* *pPosition, MOBJECT* pEnt)
						{ ( ( CNode*) *pPosition)->pEnt = pEnt;}

	// 指定位置のノードと実体を削除し、手前のノード位置を返す	pPosition   : 削除する実体のノード位置
	void		RemoveAt( MPOSITION* pPosition);

	// inserting before or after a given position
	// 指定位置の前に挿入							 pEnt	: 挿入する実体
	//											     pPosition	: この実体の前に挿入される
	void		InsertBefore( MOBJECT* pEnt, MPOSITION* pPosition);
	// 指定位置の後に挿入							 pEnt    : 挿入する実体
	//											     pPosition	: この実体の後に挿入される
	void		InsertAfter( MOBJECT* pEnt, MPOSITION* pPosition);

	// helper functions ( note: O( n) speed)
	MPOSITION Find( MOBJECT* searchValue, MPOSITION startAfter = NULL);
		// defaults to starting at the HEAD, return NULL if not found
	MPOSITION FindIndex( MINT nIndex);
		// get the 'nIndex'th element ( may return NULL)

//	Implementation
protected:
	//	ノードの追加
	CNode*		NewNode( MOBJECT* pEnt, CNode* pPrev, CNode* pNext);
	
	//	ノードの開放
	void		FreeNode( CNode* pNode);
};

/////////////////////////////////////////////////////////////////////////////
//
// 双方向リスト 実体組込型
//

class DLL_EXPORT MbSListE : public MbSynchroObj
{

protected:
	struct CNode
	{
		CNode*		pNext;							// 次の実体へのリンク
		CNode*		pPrev;							// 前の実体へのリンク
		MOBJECT*	pEnt;						// 実体へのポインタ
	};
public:
	CNode		m_sStart;							// 基点
	MINT		m_nCount;							// 実体数

	CNode*		m_pNodeFree;						// 開放ノードの基点
	MINT		m_nEntType;						// 実体サイズ( byte)
	MINT		m_nRecSize;							// レコードサイズ( byte)

	MbPlex*		m_pBlocks;							// ノード確保領域
	MINT		m_nBlockSize;						// ノード確保単位

	// 初めのリンクの取り出し	 return			  : 最初のリンク
	CNode*		GetFirst ( )
						{ return m_sStart.pNext;}

	// 最後のリンクの取り出し	 return			  : 最後のリンク
	CNode*		GetLast  ( )
						{ return m_sStart.pPrev;}

public:

	// コンストラクタ
	MbSListE( MINT nBlockSize, MINT nRecSize);

	// コピーコンストラクタ
//	MbSListE( MbSListE& List);
	
	// デストラクタ
	~MbSListE( );

// Attributes ( head and tail)
	// count of elements
	// リストが空かどうか							return: True: 空, False: 実体がある
	bool 		IsEmpty( )
						{ return m_nCount == 0;}
	bool 		IsNotEmpty( )
						{ return m_nCount != 0;}
	// リストの実体数を得る							return: 実体の数
	MINT		GetCount( )
						{ return m_nCount;}
	MINT		GetSize( )
						{ return m_nCount;}
	
	// peek at head or tail
	// 先頭の実体のポインタを得る					return  : 先頭実体のポインタ、エラーなら０
	MOBJECT*	GetHead( MPOSITION* pPosition)
						{
							SYNCHRONIZEMETHOD( );
							CNode* pNode = m_sStart.pNext;
							*pPosition = ( MPOSITION) pNode;
							if ( pNode == &m_sStart)
								return 0;
							return ( MOBJECT*) &( pNode->pEnt);
						}
	// 末尾の実体のポインタを得る					return  : 末尾実体のポインタ、エラーなら０
	MOBJECT*	GetTail( MPOSITION* pPosition)
						{
							SYNCHRONIZEMETHOD( );
							CNode* pNode = m_sStart.pPrev;
							*pPosition = ( MPOSITION) pNode;
							if ( pNode == &m_sStart)
								return 0;
							return ( MOBJECT*) &( pNode->pEnt);
						}

// Operations
	// リストの先頭に実体を入れる						pEnt	: 挿入する実体
	MPOSITION	Push( MOBJECT*  pEnt);
	MPOSITION	Push( class MbListE*  pListE);
	MPOSITION 	AddHead( MOBJECT*  pEnt)
						{ return Push( pEnt);}
	// リストの先頭の実体を取り出し消去する				return	: 0: 実体無し　1:実体有り
	MINT		Pop( MOBJECT* pEnt);
	MINT		RemoveHead( MOBJECT* pEnt)
						{ return Pop( pEnt);}

	// リストの末尾に実体を入れる						pEnt	: 挿入する実体
	MPOSITION	Inject( MOBJECT*  pEnt);
	MPOSITION	Inject( class MbListE*  pListE);
	MPOSITION 	AddTail( MOBJECT*  pEnt)
						{ return Inject( pEnt);}
	// リストの末尾の実体を取り出し消去する				return	: 0: 実体無し　1:実体有り
	MINT		Eject( MOBJECT* pEnt);
	MINT		RemoveTail( MOBJECT* &pEnt)
						{ return Eject( pEnt);}

	// remove all elements
	// すべての実体を削除する
	void		RemoveAll( );

	// iteration
	// 先頭のノード位置を得る
	MPOSITION 	GetHeadPosition( )
						{ return ( MPOSITION) m_sStart.pNext;}

	// 末尾のノード位置を得る
	MPOSITION 	GetTailPosition( )
						{ return ( MPOSITION) m_sStart.pPrev;}

	// ノード位置を進め、次の実体のポインタを得る	return: 実体のポインタ、最終より後なら０
	MOBJECT*	GetNext( MPOSITION* pPosition)			// ( i O) pPosition: ノード位置
						{
							SYNCHRONIZEMETHOD( );
							*pPosition = ( MPOSITION) ( ( CNode*) *pPosition)->pNext;
							if ( *pPosition == ( MPOSITION) &m_sStart) 
								return NULL;
							return ( MOBJECT*) &( ( CNode*) *pPosition)->pEnt;
						}

	// ノード位置を戻し、手前の実体のポインタを得る	return: 実体のポインタ、先頭より前なら０
	MOBJECT*	GetPrev( MPOSITION* pPosition)			// ( i O) pPosition: ノード位置
						{
							SYNCHRONIZEMETHOD( );
							*pPosition = ( MPOSITION) ( ( CNode*) *pPosition)->pPrev;
							if ( *pPosition == ( MPOSITION) &m_sStart) 
								return NULL;
							return ( MOBJECT*) &( ( CNode*) *pPosition)->pEnt;
						}

	// 指定位置の実体のポインタを得る
	MOBJECT*	GetAt( MPOSITION* pPosition)
						{ return ( MOBJECT*) &( ( CNode*) *pPosition)->pEnt;}

	// 指定位置に実体を書き込む
	void 		SetAt( MPOSITION* pPosition, MOBJECT* pEnt)
						{ memcpy( &( ( CNode*) *pPosition)->pEnt, pEnt, m_nEntType);}

	// 指定位置のノードと実体を削除し、手前のノード位置を返す	pPosition   : 削除する実体のノード位置
	void		RemoveAt( MPOSITION* pPosition);

	// inserting before or after a given position
	// 指定位置の前に実体を挿入する					 pEnt	: 挿入する実体
	//											     pPosition	: この実体の前に挿入される
	void		InsertBefore( MOBJECT* pEnt, MPOSITION* pPosition);
	// 指定位置の後に実体を挿入する					 pEnt    : 挿入する実体
	//											     pPosition	: この実体の後に挿入される
	void		InsertAfter( MOBJECT* pEnt, MPOSITION* pPosition);

	// helper functions ( note: O( n) speed)
	MPOSITION Find( MOBJECT* searchValue, MPOSITION startAfter = NULL);
		// defaults to starting at the HEAD, return NULL if not found
	MPOSITION FindIndex( MINT nIndex);
		// get the 'nIndex'th element ( may return NULL)

//	Implementation
protected:
	//	ノードの追加
	CNode*		NewNode( MOBJECT* pEnt, CNode* pPrev, CNode* pNext);
	
	//	ノードの開放
	void		FreeNode( CNode* pNode);
};


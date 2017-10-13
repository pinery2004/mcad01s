#pragma once
//==========================================================================================
//  Copyright ( C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MbArray.h
//
//		要素の集合を管理する基本クラス
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================

#ifndef ASSERT
	#define IC_DYNARRAY_H_ASSERT	
	#define ASSERT( a )			assert( ( a) )
#endif

template <class elementType>
class MbArray
	{	
	
public:
	MbArray( unsigned int nGran = 10 )
	{
		ASSERT( nGran != 0 );

	    m_nGran = nGran;
	    m_nEntries = 0;
	    m_nSize = nGran;

		m_pElements = ( elementType *)::new char[m_nGran * sizeof( elementType )];
		ASSERT( m_pElements != NULL );
	}

	~MbArray( void)
	{					 	
		delete ( char*)m_pElements;
		m_pElements = NULL;
		m_nEntries = -1;
		m_nSize = -1;
		m_nGran = -1;
	}

public:
	// 要素テーブルの設定済み要素数を取得する
	//
	unsigned int Number( void)
	{ 
	 	return m_nEntries; 
	}

	// 要素テーブル( の後ろ)に要素を追加する
	//
	bool Add( elementType pItem )
	{
	    if ( m_nEntries == m_nSize)
			if ( !expand( ))
		    	return( FALSE );

		m_pElements[ m_nEntries ] = pItem;
		m_nEntries = m_nEntries+1;
	    return( TRUE );
	}

	// 要素テーブルの( 配列の)指定要素を削除し詰める
	// テーブル拡張時の追加サイズ単位以上の空きが出来た場合は要素テーブルのサイズを縮小する
	//
	void Remove( unsigned int nItem )
	{ 
		compress( nItem ); 
	}

	// 要素テーブルの( 配列の)指定要素を取得する
	//
	bool Get( unsigned int nItem, elementType *theelement )
	{		
		if ( nItem < m_nEntries )
		{
			*theelement=m_pElements[nItem];
			return true;
		}		
		ASSERT( FALSE );
		return false;
	}


	// 要素テーブルのポインタを取得する
	//
	elementType *GetArray( ) const
	{		
	    return( m_pElements );
	}

	// 要素テーブルの( 配列の)指定要素を修正する( 書込む)
	//
	void Set( unsigned int nItem, elementType lpItem )
	{
		if ( nItem < m_nEntries )
			m_pElements[nItem] = lpItem;
	}

private:

	// 要素テーブルのサイズを変更する
	//
	bool changeSize( unsigned int nNewSize )
	{
		ASSERT( nNewSize >= m_nEntries );
		unsigned int wMemSize = nNewSize * sizeof( elementType );

		elementType* lpNew = ( elementType*)::realloc( m_pElements, wMemSize );
		ASSERT( lpNew != NULL );
		if ( lpNew != NULL )
		{
			m_pElements = lpNew;
			m_nSize = nNewSize;
		}
	
	    return( lpNew != NULL );
	}

	// 要素テーブルを拡張する
	// 拡張サイズは( m_nEntries:テーブル拡張時の追加サイズ単位)
	//
	bool expand( )
	{
		unsigned int wNewSize = m_nSize + m_nGran;
	
		return ( changeSize( wNewSize ) );
	}

	// 要素テーブルの( 配列の)指定要素を削除し詰める
	// テーブル拡張時の追加サイズ単位以上の空きが出来た場合は要素テーブルのサイズを縮小する
	//
	void compress( unsigned int nIndex )
	{
		if ( nIndex == -1)
		{
			return;
		}

		elementType* lpElement = m_pElements + nIndex;

		unsigned int wEntriesAfterIndex = ( m_nEntries - ( nIndex + 1 ) );
	    if ( wEntriesAfterIndex > 0)
	   {
			::memmove( lpElement, lpElement + 1, wEntriesAfterIndex * sizeof( elementType ) );
		}
	    m_nEntries--;

		// テーブル拡張時の追加サイズ単位以上の空きが出来た場合は要素テーブルのサイズを縮小する
		//
		if ( ( m_nEntries > 0 ) &&
			 ( m_nEntries <= ( m_nSize - m_nGran) ) )
		{
			// テーブル拡張時の追加サイズ単位の整数倍のサイズである事を確認
			//
			ASSERT( ( m_nEntries % m_nGran) == 0 );
	
			changeSize( m_nEntries );
		}
	}

	// データ
private:
   	unsigned int	m_nGran;			// テーブル拡張時の追加サイズ単位
	unsigned int	m_nSize;			// 全テーブルサイズ
	unsigned int	m_nEntries;			// 設定済み要素数
	elementType *	m_pElements;		// 要素テーブル
};


#ifdef IC_DYNARRAY_H_ASSERT
	#undef ASSERT
#endif


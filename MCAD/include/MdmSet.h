#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MdmSet.h
//
//		関係設定
//
//
#include	"MdmDefine.h"
#include	"MdAry.h"
#include	"MdAryH.h"
#define MDC_DELETE 0x7DCDCDC

// ＭＤＢのＩＤを配列番号に変換する
//inline MINT MIDtoHN( MINT i_id) { return ( i_id - 1); }
#define MIDtoHN( id)  ( id - 1)
// ＭＤＢの配列番号をＩＤに変換する
//inline MINT MHNtoID( MINT i_idS) { return ( i_idS + 1); }
#define MHNtoID( idS) ( idS + 1)

namespace MC
{

enum MDCONCTYPE {
	
};

//==========================================================================================
//class MdRelation
//{
//	MINT		type;
//	MdEnt*	pEntP;
//	MdEnt*	pEntC;
//};

//==========================================================================================
//	親子関係のセット
//	・セットとセットの関係
//	・セットとエンティティの関係
//	・エンティティとセットの関係
//	親IDと子ID組み合わせ
//	(２親を持つ関係のセットの定義に使用)
//==========================================================================================
class MdeCombPC
{
	MDID		m_idP;							// 親ID
	MDID		m_idC;							// 子ID

public:
	// デバッグ用トレース出力
	void Print( MCHAR* s, MINT i_i);
};

//==========================================================================================
//	関係のセット
//	・エンティティとエンティティの関係
//	関係コードと属性1、属性2、子ID組み合わせ
//==========================================================================================
class MdeRfC
{
	MUCHAR		m_iC;							// 関係コード
	MUCHAR		m_iA;							// 属性コード
	MSHORT		m_iLp;							// ループコード
	MDID		m_idC;							// 子ID

public:
	// デバッグ用トレース出力
	void Print( MCHAR* s, MINT i_i);
};

//	トレース
inline void MdeRfC::Print( MCHAR* s, MINT i_i)
{
#ifdef _DEBUG
	Msprintf( MbLog::m_Str,
			  Mstr( "・%s < MdeRfC > ID = %d, 関係コード = %d, 属性コード = %d, ループコード = %d, 子ID = %d\n"),
			  s, i_i, m_iC, m_iA, m_iLp, m_idC);
	MBTRCPRBF;
#endif
}

class MDIDSET : public MdAryH_S_I<MDID> 		// ID(MDID)セット(郡)
{
public:
	MDIDSET() : MdAryH_S_I() {};
	MDIDSET( MUSHORT i_sz) : MdAryH_S_I( i_sz) {};
};

class MDRFCSET : public MdAryH_S_C<MdeRfC>		// 関係コード、属性コード、ループコード,子ID}セット
{
public:
	MDRFCSET() : MdAryH_S_C<MdeRfC>() {};
	MDRFCSET( MUSHORT i_sz) : MdAryH_S_C<MdeRfC>( i_sz) {};
};

class MUBYTESET : public MdAryH_S_B<MUBYTE> 	// ウィンドウ表示検地モードセット
{
public:
	MUBYTESET() : MdAryH_S_B<MUBYTE>() {};
	MUBYTESET( MUSHORT i_sz) : MdAryH_S_B<MUBYTE>( i_sz) {};
};

//class MDFIG : public MdAryH_S_I<MINT> 		// 図形情報(可変長ワード(仮に整数で定義))
//{
//public:
//	MUSHORT	m_isz;
//	MUSHORT	m_n;
//	MINT	m_st[100];
//
//	MUSHORT	icdPrim( MINT iP)
//				{ return ((MUSHORT*)&m_st[iP])[0];}
//	MUSHORT	Next( MINT iP)
//				{ return (iP + ((MUSHORT*)&m_st[iP])[1]);}
//	MINT*	pPrim( MINT iP)
//				{ return &m_st[iP + 1];}
//};

//==========================================================================================
//	削除データリンク付きセット
template <typename T>
class MdAryH_F_D : public MdAryH_F<typename T>
{
public:
	MDID		m_idSpaceLast;					// (削除による)最終空きレコードID
	MINT		m_nidSpace;						// (削除による)空きレコードID数

public:
	MdAryH_F_D() {
		m_idSpaceLast = 0;
		m_nidSpace = 0;
	}
	// デバッグ用トレース出力
	void Print( MCHAR* s, MINT i_i);
};

//	トレース
template <typename T>
inline void MdAryH_F_D<T>::Print( MCHAR* i_s, MINT i_i)
{
#ifdef _DEBUG
	if ( i_i == MDC_NONE_ID) {
		if ( m_idSpaceLast ==0 && m_nidSpace == 0) {
			Msprintf( MbLog::m_Str, Mstr( "%s < MdAryH_F_D >\n"), i_s);
		} else {
			Msprintf( MbLog::m_Str, Mstr( "%s < MdAryH_F_D > 最終空きレコードID = %d, 空きレコードID数 = %d\n"),
											i_s, m_idSpaceLast, m_nidSpace);
		}
	} else {
		if ( m_idSpaceLast ==0 && m_nidSpace == 0) {
			Msprintf( MbLog::m_Str, Mstr( "%s < MdAryH_F_D > ID = %d\n"), i_s, i_i);
		} else {
			Msprintf( MbLog::m_Str, Mstr( "%s < MdAryH_F_D > ID = %d，最終空きレコードID = %d, 空きレコードID数 = %d\n"),
											i_s, i_i, m_idSpaceLast, m_nidSpace);
		}
	}
	MBTRCPRBF;
	// グループ(セット)有効データ出力
	int iG;
	for ( iG=1; iG<=m_n; iG++) {
		if ( MDSPACEID( &m_st[MIDtoHN( iG)]) != MDID_DELETE)				// Validグループデータ
			m_st[MIDtoHN( iG)].Print( Mstr( "レコード"), iG);
	}

	// グループ(セット)削除データのリンクを出力
	MDID	idDelTrc;
	MDIDSET	GidSpNext( 100);
	MDIDSET GidSpBefore( 100);

	idDelTrc =  m_idSpaceLast;

	if ( idDelTrc)
		MBTRCPRINTI( Mstr( "\tLAST削除ID"), idDelTrc);

	for ( ; GidSpNext.m_n < m_nidSpace && idDelTrc; ) {
		GidSpNext += MDSPACENEXT( &m_st[MIDtoHN( idDelTrc)]);
		GidSpBefore += MDSPACEBEFORE( &m_st[MIDtoHN( idDelTrc)]);
		idDelTrc = MDSPACEBEFORE( &m_st[MIDtoHN( idDelTrc)]);
	}

	if ( GidSpNext.m_n) {
		MBTRCPRINTIN( Mstr( "\t削除NextIDセット"), GidSpNext.m_st, GidSpNext.m_n);
		MBTRCPRINTIN( Mstr( "\t削除BeforeIDセット"), GidSpBefore.m_st, GidSpBefore.m_n);
	}
#endif
}

} // namespace MC

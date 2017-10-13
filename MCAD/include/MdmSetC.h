#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MdModel.h
//
//		ＭＣＡＤモデル
//
//
//==========================================================================================
namespace MC
{

//	子IDセット
//
//					親ID - {子ID}n
//
#undef new

class MdmSetC
{
public:
	MDID		m_idP;							// 親ID
	MDIDSET		m_GidC;							// 子IDセット

public:
	void Init()		{ m_GidC.Init();}
	void Free()		{ m_GidC.Free();}
	// 子IDセット生成
	//	最大指定数の(子の)IDが入る空領域を確保する
	void* operator new( size_t i_sz);			// 初期の最大子ID数
	// 領域を確保する
	void Alloc( MINT i_szC)	
						{ m_GidC.Alloc( i_szC);}
	////	領域を開放する
	//void* operator delete();
	// 子IDセットに子IDを追加する
	void operator += ( MDID i_idMd);
	// 子IDセットの子IDを削除する
	void operator -= ( MDID i_idMd);
	// 子IDセットを初期設定する
	void initialize(
						MDID	i_idP			// 親ID
					);
	// デバッグ用トレース出力
	void Print( MCHAR* s, MINT i_i);
};

// 子IDセット生成
//	最大指定数の(子の)IDが入る空領域を確保
inline void* MdmSetC::operator new( size_t i_sz)
{
	MdmSetC* pSetC = (MdmSetC*)DEBUG_NEW char[sizeof( MdmSetC)];
	pSetC->m_idP = 0;
	pSetC->m_GidC.Alloc( (MINT) i_sz);
	return pSetC;
}

// 子IDセットに子IDを追加する
inline void MdmSetC::operator += (MDID i_idMd)
{
	m_GidC += i_idMd;
}

// 子IDセットの子IDを削除する
inline void MdmSetC::operator -= (MDID i_idMd)
{
	MINT iC;

	ASSERT(i_idMd >0 && i_idMd <= m_GidC.m_n);
	for ( iC=0; iC<m_GidC.m_n; iC++) {
		if ( m_GidC.m_st[iC] == i_idMd) {
			for ( iC++; iC<m_GidC.m_n; iC++) {
				m_GidC.m_st[iC-1] = m_GidC.m_st[iC];
			}
			m_GidC.m_n --;
			break;
		}
	}
}

// 子IDセットを初期設定する
inline void MdmSetC::initialize(
						MDID	i_idP			// 親ID
				)
{
	m_idP = i_idP;
}

//	トレース
inline void MdmSetC::Print( MCHAR* s, MINT i_i)
{
#ifdef _DEBUG
	Msprintf( MbLog::m_Str, Mstr( "\t\t%s < MdmSetC > ID = %d、親ID = %d\n"), s, i_i, m_idP);
	MBTRCPRBF;
	m_GidC.Print( Mstr( "子IDセット"), MDC_NONE_ID);
#endif
}

//次の追加が必要;
//==========================================================================================
//	子関係セット
//
//					親ID - {関係コード、属性コード、ループコード,子ID}n
//
//MDCPCSET;

} // namespace MC
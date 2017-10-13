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

//	親IDセット
class MdmSetP
{
	MDID		m_idC;							// 子ID
	MDIDSET		m_GidP;							// 親ID群
public:
	// 親IDセット生成
	//	最大指定数の(親の)IDが入る空領域を確保する
	void* operator new( size_t i_sz);			// 初期の最大親ID数
	// 親IDを追加する
	void operator += ( MDID i_idMd);
	// 親IDを削除する
	void operator -= ( MDID i_idMd);
	// 親IDセットを初期設定する
	void initialize(
						MDID	i_idC			// 子ID
				);
	// デバッグ用トレース出力
	void Print( MCHAR* s);
};

// 親IDセット生成
//	最大指定数の(親の)IDが入る空領域を確保
inline void* MdmSetP::operator new( size_t i_sz)
{
	MdmSetP* pSetP = (MdmSetP*)DEBUG_NEW char[sizeof( MdmSetP)];
	pSetP->m_idC = 0;
	pSetP->m_GidP.Alloc( (int)i_sz);
	return pSetP;
}

// 親IDセットに親IDを追加する
inline void MdmSetP::operator += (MDID i_idMd)
{
	m_GidP += i_idMd;
}

// 親IDセットの親IDを削除する
inline void MdmSetP::operator -= (MDID i_idMd)
{
	ASSERT(i_idMd >0 && i_idMd <= m_GidP.n);
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

// 親IDセットを初期設定する
inline void MdmSetP::initialize(
						MDID	i_idC			// 子ID
				)
{
	m_idC = i_idC;
}

//	トレース
inline void MdmSetP::Print( MCHAR* s, MINT i_i)
{
#ifdef _DEBUG
	Msprintf( MbLog::m_Str, Mstr( "%s < MdmSetP > ID = %d, 子ID = %d\n"), s, i_i, m_idC);
	MBTRCPRBF;
	m_GidP.Print( Mstr( "親ID群"));
#endif
}

} // namespace MC
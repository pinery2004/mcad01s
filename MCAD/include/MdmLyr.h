#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MdmLyr.h
//
//		ＭＣＡＤモデル
//						レイヤー
//
//==========================================================================================
//	２親を持つ子IDセット
#include	"MdFig.h"

namespace MC
{

class MdmSetLYR
{
public:
	MDID		m_idP1;							// 親ID1	グループId ( >=0)	( -1: 削除し無効レイヤー)
	MDID		m_idP2;							// 親ID2	スキーマId
	MDIDSET		m_GidC;							// 子ID群
	MUBYTESET	m_GaWD;							// ウィンドウ表示検地モード(ウィンドウ毎設定)
												//		0: 表示 OFF	検地 OFF
												//		1: 表示 ON	検地 OFF
												//		2: 表示 ON	検地 ON
												//		3: 表示 OFF	検地 OFF	Lock
												//		4: 表示 ON	検地 OFF	Lock
												//		5: 表示 ON	検地 ON		Lock
	MDFIG		m_DfltFig;						// デフォルト表示属性
	MDFIG		m_CurFig;						// カレント表示属性

public:
	MdmSetLYR()		{ Init();}					// 2008/05/05 add
	~MdmSetLYR()		{ Free();}					// 2008/05/05 add
	void Init()
	{
		m_GidC.Init();
		m_GaWD.Init();
		//m_DfltFig.Alloc();
		//m_CurFig.Alloc();
	}
	void Free()
	{
		m_GidC.Free();
		m_GaWD.Free();
		m_DfltFig.Free();
		m_CurFig.Free();
	}
	// 子IDセット生成
	//	最大指定数の(子の)IDが入る空領域を確保する
	void* operator new( size_t i_sz);			// 初期の最大子ID数
	////	領域を開放する
	//void* operator delete();
	// 子IDを追加する
	void operator += ( MDID i_idMd);
	// 子IDを削除する
	void operator -= ( MDID i_idMd);
	// 子IDセットに初期値を設定する
	void SetInit(
						MDID	i_idP1,			// 親ID1
						MDID	i_idP2,			// 親ID2
						MINT	i_szCID,		// 子IDエリアサイズ
						MINT	i_szMdD		// 表示検知モード設定ウィンドウ数
				);
	// デバッグ用トレース出力
	void Print( MCHAR* s, MINT i_i);
};

// ２親を持つ子IDセット生成
//	最大指定数の(子の)IDが入る空領域を確保
inline void* MdmSetLYR::operator new( size_t i_sz)
{
	MdmSetLYR* pSetLC = (MdmSetLYR*)DEBUG_NEW char[sizeof( MdmSetLYR)];
	pSetLC->m_idP1 = 0;
	pSetLC->m_idP2 = 0;
	pSetLC->m_GidC.Alloc( (int)i_sz);
	return pSetLC;
}

// ２親を持つ子IDセットに子IDを追加する
inline void MdmSetLYR::operator += (MDID i_idMd)
{
	m_GidC += i_idMd;
}

// ２親を持つ子IDセットの子IDを削除する
inline void MdmSetLYR::operator -= (MDID i_idMd)
{
	MINT iC;
	bool bFnd = false;
	for ( iC=0; iC<m_GidC.m_n; iC++) {
		if ( m_GidC.m_st[iC] == i_idMd) {
			for ( iC++; iC<m_GidC.m_n; iC++) {
				m_GidC.m_st[iC-1] = m_GidC.m_st[iC];
			}
			m_GidC.m_n --;
			bFnd = true;
			break;
		}
	}
	ASSERT( bFnd);
}

// ２親を持つ子IDセットに初期値を設定する
inline void MdmSetLYR::SetInit(
						MDID	i_idP1,			// 親ID1
						MDID	i_idP2,			// 親ID2
						MINT	i_szCID,		// 子IDエリアサイズ
						MINT	i_szMdD			// 表示検知モード設定ウィンドウ数
				)
{
	m_idP1 = i_idP1;
	m_idP2 = i_idP2;
	m_GidC.Alloc( i_szCID);					// 子IDエリア
	m_GaWD.Alloc( i_szMdD);					// ウィンドウ毎の表示・検知モード(初期は4ウィンドウ)
	m_DfltFig.SetpPmtl( NULL);
	m_CurFig.SetpPmtl( NULL);
}

//	トレース
inline void MdmSetLYR::Print( MCHAR* s, MINT i_i)
{
#ifdef _DEBUG
	Msprintf( MbLog::m_Str, Mstr( "\t%3d: %s < MdmSetLYR > 親ID1 = %d, 親ID2 = %d\n"), i_i, s, m_idP1, m_idP2);
	MBTRCPRBF;
	m_GidC.Print( Mstr( "子ID群"), MDC_NONE_ID);
	m_GaWD.PrintB( Mstr( "ウィンドウ表示検地モード"), MDC_NONE_ID);
#endif
}

} // namespace MC
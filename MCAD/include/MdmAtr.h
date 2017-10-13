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

	class MdmAtr
{
	MUBYTESET	m_at;

public:
	void Init()	{}
	void Free()	{}

	void Alloc( MINT i_szAt)
						{ m_at.Alloc( i_szAt);}
	// デバッグ用トレース出力
	void Print( MCHAR* s, MINT i_i);
};
//	トレース
inline void MdmAtr::Print( MCHAR* s, MINT i_i)
{
#ifdef _DEBUG
	Msprintf( MbLog::m_Str, Mstr( "		%s < MdmAtr > ID = %d\n"), s, i_i);
	MBTRCPRBF;
#endif
}

} // namespace MC

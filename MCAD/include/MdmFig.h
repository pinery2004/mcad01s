#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MdmFig.h
//
//		�l�b�`�c���f��
//
//
//==========================================================================================
namespace MC
{

class MdmFig
{
public:
	MDFIG		m_GiFig;
public:
	void Init()	{}
	void Free()	{}

	void Alloc( int i_szFig)
						{ m_GiFig.ReSize( i_szFig);}
	// �f�o�b�O�p�g���[�X�o��
	void Print( MCHAR* s, MINT i_i);
};
//	�g���[�X
inline void MdmFig::Print( MCHAR* s, MINT i_i)
{
#ifdef _DEBUG
	Msprintf( MbLog::m_Str, Mstr( "		%s < MdmFig > ID = %d\n"), s, i_i);
	MBTRCPRBF;
#endif
}

//#pragma once

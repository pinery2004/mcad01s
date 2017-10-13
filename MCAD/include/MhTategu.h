#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MhTategu.h
//
//		建具属性レコード
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================

//	建具(壁開口)
#define	MHTTSZ_CODE		4										// 開口コード最大文字数

namespace MC
{

class MhMdlIO;

class	MhTateguInfo
{
	friend	class	MhPlcInfo;

	MCHAR	m_cKCode[MHTTSZ_CODE + 1];			// 開口コード
	MREAL	m_rROW;								// 開口幅
	MREAL	m_rROH;								// 開口高さ
	MREAL	m_rHeight;							// 取り付け高さ(まぐさ下端高さ)
	MREAL	m_rMUSz[2];							// 左右まぐさ受長さ

public:
	MINT	Load(												// ＤＢより１レコード読み込み
						MhMdlIO*	i_phMdl						// 読み込みファイル　ハンドル
				);
	MINT	Save(												// ＤＢへ１レコード書き込み
						MhMdlIO*	i_phMdl						// 書き込みファイル　ハンドル
				);

	void	SetKCode( MCHAR* cKCode)	{ Mstrncpy( m_cKCode, cKCode, MHTTSZ_CODE);}
	void	SetROW( MREAL rROW)			{ m_rROW = rROW;}
	void	SetROH( MREAL rROH)			{ m_rROH = rROH;}
	void	SetHgt( MREAL rHeight)
										{ m_rHeight = rHeight;}
	void	SetMUSz( MREAL rMUSz, MINT iM)
										{ m_rMUSz[iM] = rMUSz;}

//S	void	GetKCode( MCHAR* cKCode)	{ Mstrncpy( cCode, m_cCode, MHTTSZ_CODE);}
	MCHAR*	GetKCode()					{ return m_cKCode;}
	MREAL 	GetROW()					{ return m_rROW;}
	MREAL	GetROH()					{ return m_rROH;}
	MREAL	GetHeight()					{ return m_rHeight;}
	MREAL*	GetMUSz()					{ return m_rMUSz;}
	MREAL	GetMUSz( MINT iM)			{ return m_rMUSz[iM];}
};

} // namespace MC
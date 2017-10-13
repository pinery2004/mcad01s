#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MhMbr.h
//
//		寸法型式レコード
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================

//	寸法型式
#define	MHMBSZ_CODE		15										// 寸法型式最大文字数
#define	MHMBSZ_TYPE		15										// 寸法型式選択用種類コード最大文字数
#define	MHMBSZ_CODEW	15										// 寸法型式 (書き込み用)最大文字数

namespace MC
{

class	MhMbr													// 寸法型式レコード
{
	friend	class	MhPlcInfo;

	friend	void	MhLoadPtsMst();

//public:
	MINT				m_iMbId;								// 寸法型式ID
	MCHAR				m_cMbCode[16];							// 寸法型式
	MREAL				m_rMbWidth;								// 部材幅
	MREAL				m_rMbSZure;								// 幅補正用芯ずれ量		右幅 = m_rWidth/2 + m_rSZure
																//						左幅 = m_rWidth/2 - m_rSZure
	MREAL				m_rMbHeight;							// 部材高さ
	MCHAR				m_cMbType[16];							// 寸法型式選択用種類コード
	MCHAR				m_cMbCodeW[16];							// 寸法型式 (書き込み用　通常m_cCodeと同じ VerUp時に変更した内容が入る)

public:
	void	SetMbId( MINT iMbId)	
									{ m_iMbId = iMbId;}
	void	SetMbCode( MCHAR* cMbCode)
									{ Mstrncpy( m_cMbCode, cMbCode, MHMBSZ_CODE);}
	void	SetMbWidth( MREAL rMbWidth)
									{ m_rMbWidth = rMbWidth;}
	void	SetMbSZure( MREAL rMbSZure)
									{ m_rMbSZure = rMbSZure;}
	void	SetMbHeight( MREAL rMbHeight)
									{ m_rMbHeight = rMbHeight;}
	void	SetMbType( MCHAR* cMbType)
									{ Mstrncpy( m_cMbType, cMbType, MHMBSZ_TYPE);}
	void	SetMbCodeW( MCHAR* cMbCodeW)
									{ Mstrncpy( m_cMbCodeW, cMbCodeW, MHMBSZ_CODEW);}
	MINT 	GetMbId()				{ return m_iMbId;}
	void	GetMbCode( MCHAR* cMbCode)	{ Mstrncpy( cMbCode, m_cMbCode, MHMBSZ_CODE);}
	MCHAR*	GetMbCode()				{ return m_cMbCode;}
	MREAL 	GetMbWidth()			{ return m_rMbWidth;}
	MREAL	GetMbSZure()			{ return m_rMbSZure;}
	MREAL	GetMbHeight()			{ return m_rMbHeight;}
	void	GetMbType( MCHAR* cMbType)
									{ Mstrncpy( cMbType, m_cMbType, MHMBSZ_TYPE);}
	MCHAR*	GetMbType()				{ return m_cMbType;}
	void	GetMbCodeW( MCHAR* cMbCodeW)
									{ Mstrncpy( cMbCodeW, m_cMbCodeW, MHMBSZ_CODEW);}
	MCHAR*	GetMbCodeW()			{ return m_cMbCodeW;}
};

} // namespace MC
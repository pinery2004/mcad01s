#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MhMbr.h
//
//		¡@^®R[h
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================

//	¡@^®
#define	MHMBSZ_CODE		15										// ¡@^®Åå¶
#define	MHMBSZ_TYPE		15										// ¡@^®IðpíÞR[hÅå¶
#define	MHMBSZ_CODEW	15										// ¡@^® («Ýp)Åå¶

namespace MC
{

class	MhMbr													// ¡@^®R[h
{
	friend	class	MhPlcInfo;

	friend	void	MhLoadPtsMst();

//public:
	MINT				m_iMbId;								// ¡@^®ID
	MCHAR				m_cMbCode[16];							// ¡@^®
	MREAL				m_rMbWidth;								// Þ
	MREAL				m_rMbSZure;								// â³pc¸êÊ		E = m_rWidth/2 + m_rSZure
																//						¶ = m_rWidth/2 - m_rSZure
	MREAL				m_rMbHeight;							// Þ³
	MCHAR				m_cMbType[16];							// ¡@^®IðpíÞR[h
	MCHAR				m_cMbCodeW[16];							// ¡@^® («Ýp@Êím_cCodeÆ¯¶ VerUpÉÏXµ½àeªüé)

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
#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MhGp.h
//
//		構成レコード
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================

// 構成( 屋根、小屋、天井、耐力壁、壁、床、基礎)
#define	MHKSSZ_NMKOSEI	15									// 構成(組)名最大文字数
#define	MHKSSZ_NMFUSEZU	15									// 伏図名最大文字数

namespace MC
{

class	MhGp									// 構成レコード
{
	friend	class	MhPlcInfo;
	friend	class	MhTpPts;

	MINT	m_iCdGp;							// 構成(組)コード
	MCHAR	m_sNmGp[MHKSSZ_NMKOSEI + 1];		// 構成(組)名
	MCHAR	m_sNmFusezu[MHKSSZ_NMFUSEZU + 1];	// 伏図名
public:
	void	SetCdGp( MINT iCdGp)	{ m_iCdGp = iCdGp;}
	void	SetNmGp( MCHAR* cNmGp)	{ Mstrncpy( m_sNmGp, cNmGp, MHKSSZ_NMKOSEI + 1);}
	void	SetNmFusezu( MCHAR* cNmFusezu)
									{ Mstrncpy( m_sNmFusezu, cNmFusezu, MHKSSZ_NMKOSEI + 1);}

	MINT	GetCdGp()		{ return m_iCdGp;}
	MCHAR*	GetNmGp()		{ return m_sNmGp;}
	MCHAR*	GetNmFusezu()	{ return m_sNmFusezu;}

	bool	IsYane()		{ return m_iCdGp == MP_GP_YANE;}		// 屋根
	bool	IsTenjo()		{ return m_iCdGp == MP_GP_TENJO;}		// 天井
	bool	IsYuka()		{ return m_iCdGp == MP_GP_YUKA;}		// 床
	bool	IsKiso()		{ return m_iCdGp == MP_GP_KISO;}		// 基礎
};

} // namespace MC
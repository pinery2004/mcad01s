#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MdAry.h
//
//		�l�b�`�c���f��
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#pragma message( "<< MdAry.h >>")

#include "MgDefine.h"
#include "MgPoint.h"
#include "MgPolygon.h"

#define	MDTP_ENTITY			100
#define	MDTP_GROUP			11
#define	MDTP_SCHEMA			12
#define	MDTP_LAYER			13
#define	MDTP_CLATABLE		14
#define	MDTP_MONO			1
#define	MDTP_PARTS			2
#define	MDTP_COORDINATE		3

#define	MDID_DELETE			-1

namespace MC
{
//S typedef	MUBYTE			MUBYTE;					// �E�B���h�E�\�����n���[�h
//S typedef	MINT			MDID;					// �E�B���h�EId	
//S typedef	MINT			MINT;					// �E�B���h�E�^�C�v ( 0:�R�c�A1�F�}��)
typedef	MgGInt			MDGID;

//==========================================================================================
//	�폜�f�[�^�����N
class MdDelLink
{
public:
	MDID		m_idP;							// >0:�eID�A-1:�폜
	MDID		m_idNext;						// m_idP == -1�̂Ƃ��A���̍폜�f�[�^��ID�܂���0
	MDID		m_idBefore;						// m_idP == -1�̂Ƃ��A�O�̍폜�f�[�^��ID�܂���0
	// �f�o�b�O�p�g���[�X�o��
	void Print( MCHAR* s, MINT i_i);
};

//	�g���[�X
inline void MdDelLink::Print( MCHAR* s, MINT i_i)
{
#ifdef _DEBUG
	Msprintf( MbLog::m_Str, Mstr( "%s	< MdDelLink > ID = %d�C�eID = %d, ��ID = %d, �OID = %d\n"),
									s, i_i, m_idP, m_idNext, m_idBefore);
	MBTRCPRBF;
#endif
}

#define	MDSPACEID( pD)		( ( MdDelLink*)pD)->m_idP
#define	MDSPACENEXT( pD)	( ( MdDelLink*)pD)->m_idNext
#define	MDSPACEBEFORE( pD)	( ( MdDelLink*)pD)->m_idBefore

#define MDISGRP( i_idGrp)	( i_idGrp >= 1 && i_idGrp <= Mdm::GetCurModel()->m_GrpBf.m_n && \
							  MDSPACEID( &Mdm::GetCurModel()->m_GrpBf.m_st[MIDtoHN( i_idGrp)]) >= 0)
#define MDISSCM( i_idScm)	( i_idScm >= 1 && i_idScm <= Mdm::GetCurModel()->m_ScmBf.m_n && \
							  MDSPACEID( &Mdm::GetCurModel()->m_ScmBf.m_st[MIDtoHN( i_idScm)]) >= 0)
#define MDISLYR( i_idLyr)	( i_idLyr >= 1 && i_idLyr <= Mdm::GetCurModel()->m_LyrBf.m_n && \
							  MDSPACEID( &Mdm::GetCurModel()->m_LyrBf.m_st[MIDtoHN( i_idLyr)]) >= 0)
#define MDISENT( i_idEnt)	( i_idEnt >= 1 && i_idEnt <= Mdm::GetCurModel()->m_EntBf.m_n && \
							  MDSPACEID( &Mdm::GetCurModel()->m_EntBf.m_st[MIDtoHN( i_idEnt)]) >= 0)

} // namespace MC
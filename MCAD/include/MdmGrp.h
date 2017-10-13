#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MdmGrp.h
//
//		�l�b�`�c���f��
//						�O���[�v�̎qID�Z�b�g
//
//==========================================================================================

#include "MgMat.h"

#undef new

namespace MC
{

//	�O���[�v�̎qID�Z�b�g
class MdmSetGRP
{
public:
	MDID		m_idP;							// �eID(0)	( -1: �폜�������O���[�v)
	MCHAR*		m_sName;						// �O���[�v��
	MREAL		m_rKt;							// �K����
	MREAL		m_rFl;							// ������
	MgMat3		m_MTrans;						// ���W�ϊ��}�g���b�N�X
	MDIDSET		m_GidC;							// �qID�Z�b�g
	
public:
	MdmSetGRP()		{ Init();}
	~MdmSetGRP()		{ Free();}
	void Init()
	{
		m_sName = NULL;
		m_GidC.Init();
	}
	void Free()	
	{
		MBFREE( m_sName);
		m_GidC.Free();
	}

	// �O���[�v�̎qID�Z�b�g����
	//	�ő�w�萔��(�q�O���[�v��)ID�������̈���m�ۂ���
	void* operator new( size_t i_sz);			// �����̍ő�O���[�v�̎qID��
	// �O���[�v�̎qID��ǉ�����
	void operator += ( MDID i_idMdg);
	// �O���[�v�̎qID���폜����
	void operator -= ( MDID i_idMdg);
	// �O���[�v�ɏ����l��ݒ肷��
	void SetInit(
						MDID	i_idP,			// �eID(0)
						MCHAR*	i_psName,		// �O���[�v��
						MREAL	i_rKt,			// �K����
						MREAL	i_rFl			// ������
				);
	// �f�o�b�O�p�g���[�X�o��
	void Print( MCHAR* s, MINT i_i);
};

// �O���[�v�̎qID�Z�b�g����
//	�ő�w�萔��(�q�O���[�v��)ID�������̈���m�ۂ���
inline void* MdmSetGRP::operator new( size_t i_sz)
{
	MdmSetGRP* pSetGC = (MdmSetGRP*)DEBUG_NEW char[sizeof( MdmSetGRP)];
	pSetGC->m_idP = 0;
	pSetGC->m_sName = NULL;
	pSetGC->m_rKt = 0.;
	pSetGC->m_rFl = 0.;
	pSetGC->m_GidC.Alloc( (int)i_sz);
	return pSetGC;
}

// �O���[�v�̎qID�Z�b�g�ɃO���[�v�̎qID��ǉ�����
inline void MdmSetGRP::operator += (MDID i_idMdg)
{
	m_GidC += i_idMdg;
}

// �O���[�v�̎qID�Z�b�g�̃O���[�v�̎qID���폜����
inline void MdmSetGRP::operator -= (MDID i_idMd)
{
	//MINT iC;
	//bool bFnd = false;
	//for ( iC=0; iC<m_GidC.m_n; iC++) {
	//	if ( m_GidC.m_st[iC] == i_idMd) {
	//		for ( iC++; iC<m_GidC.m_n; iC++) {
	//			m_GidC.m_st[iC-1] = m_GidC.m_st[iC];
	//		}
	//		m_GidC.m_n --;
	//		bFnd = true;
	//		break;
	//	}
	//}
	//ASSERT( bFnd);

	m_GidC -= i_idMd;
}

	// �O���[�v�ɏ����l��ݒ肷��
inline void MdmSetGRP::SetInit(
						MDID	i_idP,			// �eID(0)
						MCHAR*	i_psName,		// �O���[�v�̎qID�Z�b�g��
						MREAL	i_rKt,			// �K����
						MREAL	i_rFl			// ������
				)
{
	m_idP = i_idP;
	m_sName = (MCHAR*)DEBUG_NEW char[( Mstrlen( i_psName) + 1) * sizeof( MCHAR)];
	Mstrcpy( m_sName, i_psName);
	m_rKt = i_rKt;
	m_rFl = i_rFl;
}

//	�g���[�X
inline void MdmSetGRP::Print( MCHAR* s, MINT i_i)
{
#ifdef _DEBUG
	Msprintf( MbLog::m_Str, Mstr( "	%3d: %s < MdmSetGRP > �eID = %d�C"), i_i, m_sName, m_idP);
	MBTRCPRBF;
	Msprintf( MbLog::m_Str, Mstr( "�K���� = %.1f, ������ = %.1f\n"),
									m_rKt, m_rFl);
	MBTRCPRBF;
	m_GidC.Print( Mstr( "�q���C���[ID�Q"), MDC_NONE_ID);
#endif
}

} // namespace MC
#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MdModel.h
//
//		�l�b�`�c���f��
//
//

//==========================================================================================
//	�E�B���h�E���
namespace MC
{

class MdmWin
{
public:
	MINT		m_itpWin;						// �E�B���h�E�^�C�v ( 0:�R�c�A1�F�}��)�@�@// ����
	MINT		m_iProj;						// �������@			�������e�ƕ��s���e
	MDID		m_idFn;							// �@�\�ԍ�
	MgPoint3	m_ViewPt;						// ���_�^�����_�i�܂ޗ����j
	MREAL		m_rZClip[2];					// Z�N���b�s���O�l
	MgPoint2	m_Size[2];						// �T�C�Y
	MREAL		m_Scale;						// �k��etc
	// ���̕\���^��\���@����

	// �E�B���h�E�ɕ���\�����鎞�ɂ́A
	//   �E�B���h�E�̋@�\�ԍ��ɑΉ�����֌W���Ő}�`�����o���B
	//   �����ꍇ�ɂ́A�R�����̐}�`�����o���ĕ\�����s���B
public:
	void	Init()		{}
	void	Free()		{}
	// �f�o�b�O�p�g���[�X�o��
	void Print( MCHAR* i_s, MINT i_i);
};

//	�g���[�X
inline void MdmWin::Print( MCHAR* i_s, MINT i_i)
{
#ifdef _DEBUG
	//Msprintf( MbLog::m_Str, Mstr( "%s < MdmSetP > ID = %d\n"), i_s, i_i);
	//MBTRCPRBF;
	//MBTRCPRINTI( Mstr( "		�E�B���h�E�^�C�v ( 0:�R�c�A1�F�}��)", m_itpWin);
	//MBTRCPRINTI( Mstr( "		�������@			�������e�ƕ��s���e"), m_iProj);
	//MBTRCPRINTI( Mstr( "		�@�\�ԍ�"), m_idFn);
	//MBTRCPRINTIN( Mstr( "		���_�^�����_�i�܂ޗ����j", m_ViewPt, 3);
	//MBTRCPRINTFN( Mstr( "		Z�N���b�s���O�l"), m_rZClip, 2);
	//MBTRCPRINTFN( Mstr( "		�T�C�Y"), m_Size, 2);
	//MBTRCPRINTFN( Mstr( "		�k��etc", m_Scale);
#endif
}

//==========================================================================================
class MdmCordinate
{
public:	
	// �f�o�b�O�p�g���[�X�o��
	void Print( MCHAR* i_s, MINT i_i);
};
//	�g���[�X
inline void MdmCordinate::Print( MCHAR* i_s, MINT i_i)
{
#ifdef _DEBUG
	Msprintf( MbLog::m_Str, Mstr( "%s	< MdmCordinate > ID = %d\n"), i_s, i_i);
	MBTRCPRBF;
#endif
}

} // namespace MC
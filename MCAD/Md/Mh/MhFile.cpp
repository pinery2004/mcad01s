//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MhFile.cpp
//
//		�\���ƃ��f���ǂݏ����h�n����
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"
#include "afxdlgs.h"

#include "MbDebug.h"
#include "MgDefine.h"
#include "MgTol.h"

#include "MgAngle.h"
#include "MgPoint.h"
#include "MgLine.h"
#include "MgPlane.h"

#include "MmDefine.h"
#include "MsBitSet.h"
#include "MmOpt.h"

#define DLL_EXPORT_IEMODEL_DO
#include "MhLib.h"

#include "MbFile.h"
#include "MsLib.h"

namespace MC
{

static MCHAR*	 z_cTitle = Mstr( "MCAD");
static MCHAR*	 z_cVer = Mstr( "V0.01");

////////////////////////////////////////////////////////////////////////////
//�y�@�\�z�\���ƃ��f��

////////////////////////////////////////////////////////////////////////////
//�y�@�\�z�R���X�g���N�^

MhMdlIO::MhMdlIO()
{
	memset( &m_Header, 0, sizeof(MhMdlHeader));
	Mstrcpy( m_Header.cTitle, z_cTitle);
	Mstrcpy( m_Header.cVer, z_cVer);
	m_Header.cTime = MsGetCurrentTime();								// �쐬���� 
}

////////////////////////////////////////////////////////////////////////////
//�y�@�\�z�f�X�g���N�^

MhMdlIO::~MhMdlIO          ()
{
}

////////////////////////////////////////////////////////////////////////////
//�y�@�\�z�t�@�C���ǂݏ��������J�n�錾
//�y�Ԓl�z==  0 : ����I��
//		  == -2 : ���ɃI�[�v������Ă���
//		  �� -3 : ���̑��G���[

MINT MhMdlIO::Open(
				const	DWORD		iMode,			// (I ) �g�p���[�h
													//		== MBREAD:	�ǂݍ��ݐ�p
													//		== MBWRITE:	�������ݐ�p
				const	MCHAR*		cFilePath
				)
{
	MINT		ist = -1;
	MINT		OutSiz;
	MUINT		nOpenFlag;										// �A�N�Z�X���[�h

	Mstrcpy( m_MdlPath, cFilePath);

	if ( iMode == MBREAD) {
		nOpenFlag = CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary;
	} else if ( iMode == MBWRITE) {
		nOpenFlag = CFile::modeWrite | CFile::modeCreate | CFile::shareExclusive;
	}
	
	if ( !m_File.Open( cFilePath, nOpenFlag)) {
		ist = -3;
		MQUIT;
	}

	if ( iMode == MBREAD) {
		OutSiz = m_File.Read( &m_Header, sizeof( MhMdlHeader));
		if ( OutSiz != sizeof( MhMdlHeader))
			MQUIT;
		if ( Mstrcmp( m_Header.cTitle, z_cTitle) != 0)
			MQUIT;
		MINT	itr;
		OutSiz = m_File.Read( &itr, sizeof( MINT));
		OutSiz = m_File.Read( &itr, sizeof( MINT));
	} else if ( iMode == MBWRITE) {
		m_File.Write( &m_Header, sizeof(MhMdlHeader));
		MINT	it = 333;
		m_File.Write( &it, sizeof(MINT));
		it = 777;
		m_File.Write( &it, sizeof(MINT));
	}
	ist = 0;
exit:
	return( ist);
}


////////////////////////////////////////////////////////////////////////////
//�y�@�\�z�t�@�C���ǂݏ��������I���錾
//�y�Ԓl�z==  0 : ����I��
//		  ��  0 : ���̑��G���[

MINT MhMdlIO::Close()
{
	MINT		ist;
//	MINT		ist1;
	MINT		iSize = 0;

	ist = -1;

	if ( m_iMode == MBWRITE) {
		m_File.Write( &iSize, sizeof(MINT));
		m_File.Write( &iSize, sizeof(MINT));
	}

	m_File.Close();

	return ( ist);
}

////////////////////////////////////////////////////////////////////////////
//�y�@�\�z�t�@�C���ǂݍ���
//�y�Ԓl�z > 0 : �ǂݍ��݃o�C�g��
//		  == 0 : �I��
//		   < 0 : ���̑��G���[

MINT MhMdlIO::Read(
						MINT*		piType,			// (  O) ����
						void**		Buf			// (  O) �Ǎ��ݗ̈�
				)
{
	MINT		iSizeR;
	MINT		iSizeW;

	iSizeW = m_File.Read( piType, sizeof(MINT));

	if ( iSizeW != sizeof(MINT)) {
		iSizeR = -1;
		MQUIT;
	}

	iSizeW = m_File.Read( &iSizeR, sizeof(MINT));

	if ( iSizeW != sizeof(MINT)) {
		iSizeR = -1;
		MQUIT;
	}
	if ( iSizeR == 0)
		MQUIT;													// �I��

	//int* pTest = new int[2];
//===
	static int iCtrc = 0;
	TRACE( "%d: new char[%d]\n", ++iCtrc, iSizeR);
	if( iCtrc == 5157) {
		int iii = 1;
	}
//---
	*Buf = (byte *)new char[iSizeR];
	ASSERT( *Buf);												// �������@��A���b�N�G���[�@<ERROR>
	iSizeW = m_File.Read( *Buf, iSizeR);

	if ( iSizeW != iSizeR) {
		iSizeR = -1;
		MQUIT;
	}
exit:
	return ( iSizeR);
}

//U ////////////////////////////////////////////////////////////////////////////
////�y�@�\�z�C�ӌ^�f�[�^�̃t�@�C����������
////�y�Ԓl�z==  0 : ����I��
////		  ��  0 : ���̑��G���[
//
//MINT MhMdlIO::Write(
//				const	MINT		i_tp,			// ����
//				const	void*		i_bf,			// �����ݗ̈�
//				const	MINT		i_sz			// �������޲Đ�
//				)
//{
//	MINT		ist;
//	ist = -1;
//	m_File.Write( &i_tp, sizeof(MINT));							// ����
//	m_File.Write( &i_sz, sizeof(MINT));							// ����
//	m_File.Write( i_bf, i_sz);									// �C�ӌ^�f�[�^
//	ist = 0;
//	return( ist);
//}

////////////////////////////////////////////////////////////////////////////
//�y�@�\�zSHORT�^�f�[�^�̃t�@�C����������
//�y�Ԓl�z==  0 : ����I��
//		  ��  0 : ���̑��G���[

MINT MhMdlIO::Write(
				const	MINT		i_tp,			// ����
				const	MSHORT*		i_ibf,			// ������SHORT�z��
				const	MINT		i_iwd			// ������SHORT�^���[�h��
				)
{
	MINT		ist;
	ist = -1;
	m_File.Write( &i_tp, sizeof(MINT));							// ����
	m_File.Write( &i_iwd, sizeof(MINT));						// ����
	m_File.Write( i_ibf, i_iwd * sizeof(MSHORT));				// �����z��
	ist = 0;
	return( ist);
}

////////////////////////////////////////////////////////////////////////////
//�y�@�\�z�����^�f�[�^�̃t�@�C����������
//�y�Ԓl�z==  0 : ����I��
//		  ��  0 : ���̑��G���[

MINT MhMdlIO::Write(
				const	MINT		i_tp,			// ����
				const	MINT*		i_ibf,			// �����ݐ����z��
				const	MINT		i_iwd			// �����݃��[�h��
				)
{
	MINT		ist;
	ist = -1;
	m_File.Write( &i_tp, sizeof(MINT));							// ����
	m_File.Write( &i_iwd, sizeof(MINT));						// ����
	m_File.Write( i_ibf, i_iwd * sizeof(MINT));					// �����z��
	ist = 0;
	return( ist);
}

////////////////////////////////////////////////////////////////////////////
//�y�@�\�z�����^�f�[�^�̃t�@�C����������
//�y�Ԓl�z==  0 : ����I��
//		  ��  0 : ���̑��G���[

MINT MhMdlIO::Write(
				const	MINT		i_tp,			// ����
				const	MUINT*		i_ibf,			// �����ݐ����z��
				const	MINT		i_iwd			// �����݃��[�h��
				)
{
	MINT		ist;
	ist = -1;
	m_File.Write( &i_tp, sizeof(MINT));							// ����
	m_File.Write( &i_iwd, sizeof(MINT));						// ����
	m_File.Write( i_ibf, i_iwd * sizeof(MINT));					// �����z��
	ist = 0;
	return( ist);
}

////////////////////////////////////////////////////////////////////////////
//�y�@�\�zFLOAT�^�f�[�^�̃t�@�C����������
//�y�Ԓl�z==  0 : ����I��
//		  ��  0 : ���̑��G���[

MINT MhMdlIO::Write(
				const	MINT		i_tp,			// ����
				const	float*		i_rbf,			// �����ݎ����z��
				const	MINT		i_iwd			// �����݃��[�h��
				)
{
	MINT		ist;
	ist = -1;
	m_File.Write( &i_tp, sizeof(MINT));							// ����
	m_File.Write( &i_iwd, sizeof(MINT));						// ����
	m_File.Write( i_rbf, i_iwd * sizeof(float));				// �����z��
	ist = 0;
	return( ist);
}

////////////////////////////////////////////////////////////////////////////
//�y�@�\�zDOUBLE�^�f�[�^�̃t�@�C����������
//�y�Ԓl�z==  0 : ����I��
//		  ��  0 : ���̑��G���[

MINT MhMdlIO::Write(
				const	MINT		i_tp,			// ����
				const	double*		i_rbf,			// �����ݎ����z��
				const	MINT		i_iwd			// �����݃��[�h��
				)
{
	MINT		ist;
	ist = -1;
	m_File.Write( &i_tp, sizeof(MINT));							// ����
	m_File.Write( &i_iwd, sizeof(MINT));						// ����
	m_File.Write( i_rbf, i_iwd * sizeof(double));				// �����z��
	ist = 0;
	return( ist);
}

//////////////////////////////////////////////////////////////////////////////
////�y�@�\�zBYTE�^�f�[�^�̃t�@�C����������
////�y�Ԓl�z==  0 : ����I��
////		  ��  0 : ���̑��G���[
//
//MINT MhMdlIO::Write(
//				const	MINT		i_tp,			// ����
//				const	MBYTE*		i_ibf,			// ������BYTE�z��
//				const	MINT		i_ibt			// ������BYTE�^�o�C�g��
//				)
//{
//	MINT		ist;
//	ist = -1;
//	m_File.Write( &i_tp, sizeof(MINT));							// ����
//	m_File.Write( &i_ibt, sizeof(MBYTE));						// ����
//	m_File.Write( i_ibf, i_ibt * sizeof(MBYTE));				// �����z��
//	ist = 0;
//	return( ist);
//}

////////////////////////////////////////////////////////////////////////////
//�y�@�\�z����(char)�^�f�[�^�̃t�@�C����������
//�y�Ԓl�z==  0 : ����I��
//		  ��  0 : ���̑��G���[

MINT MhMdlIO::Write(
				const	MINT		i_tp,			// ����
				const	char*		i_cDat,			// �����ݕ�����
				const	MINT		i_sz			// �����ݕ�����
				)
{
	MINT		ist;
	ist = -1;
	m_File.Write( &i_tp, sizeof(MINT));							// ����
	m_File.Write( &i_sz, sizeof(MINT));							// ����
	m_File.Write( i_cDat, i_sz);								// ������
	ist = 0;
	return( ist);
}

////////////////////////////////////////////////////////////////////////////
//�y�@�\�z����(_TCHAR)�^�f�[�^�̃t�@�C����������
//�y�Ԓl�z==  0 : ����I��
//		  ��  0 : ���̑��G���[

MINT MhMdlIO::Write(
				const	MINT		i_tp,			// ����
				const	_TCHAR*		i_cDat,			// �����ݕ�����
				const	MINT		i_sz			// �����ݕ�����
				)
{
	MINT		ist;
	ist = -1;
	m_File.Write( &i_tp, sizeof(MINT));							// ����
	m_File.Write( &i_sz, sizeof(MINT));							// ����
	m_File.Write( i_cDat, i_sz * sizeof(_TCHAR));				// ������
	ist = 0;
	return( ist);
}

////////////////////////////////////////////////////////////////////////////
//�y�@�\�z����(_TCHAR)�^�f�[�^�̃t�@�C����������
//�y�Ԓl�z==  0 : ����I��
//		  ��  0 : ���̑��G���[

MINT MhMdlIO::Write(
				const	MINT		i_tp,			// ����
				const	CString*	i_sDat,			// �����ݕ�����
				const	MINT		i_sz			// �����ݕ�����
				)
{
	MINT		ist;
	ist = -1;
	m_File.Write( &i_tp, sizeof(MINT));							// ����
	m_File.Write( &i_sz, sizeof(MINT));							// ����
	m_File.Write( i_sDat, i_sz * sizeof(_TCHAR));				// ������
	ist = 0;
	return( ist);
}

////////////////////////////////////////////////////////////////////////////
//	�t�@�C���_�C�A���O��\�����t���p�X���擾����
//	[�Ԓl] �X�e�C�^�X     0 : �L�����Z��  1 : �n�j

MINT IeModel::MhFileDialog(									//
				const	MINT		fRead,			// (I  ) �ǂݎ��t���O		true:�J��	false:�ۑ�
				const	MCHAR*		cPathI,			// (I  ) �w��t�@�C�����i�t���p�X�j
						MCHAR*		cPathO)			// (  O) �I���t�@�C�����i�t���p�X�j
{
	MINT			ist;
	CFileDialog*	DlgFile = NULL;
	MCHAR			cFldPath[MX_PATH];
	MCHAR			cFileName[MX_PATH];
	MCHAR*			pcFileName = NULL;


	ist = 0;

	Mstrcpy( cPathO, Mstr( ""));

	DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ENABLESIZING;

	if ( cPathI) {																		// �w��t�@�C��������
		IeModel::MhSeparateFname( cPathI, cFldPath, cFileName, (MCHAR*)NULL, (MCHAR*)NULL);
		pcFileName = cFileName;
	}

	CFileDialog FDlg( fRead,  Mstr( "mdk"), pcFileName, dwFlags,
						 	    Mstr( "�\���ƃ��f���t�@�C��(*.mdk)|*.mdk|�S�t�@�C��(*.*)|*.*||"), NULL);
	if ( cPathI)
		FDlg.m_ofn.lpstrInitialDir = cFldPath;

	if ( FDlg.DoModal() == IDOK) {													// �_�C�A���O��\������
		Mstrcpy( cPathO, FDlg.GetPathName());
		ist = 1;
	}
	return( ist);
}

////////////////////////////////////////////////////////////////////////////
//	�t���p�X������A�t�H���_�[�p�X���A�t�@�C�����A�t�@�C���^�C�g���A
//					�t�@�C���g���q���擾����
void IeModel::MhSeparateFname(
				const	MCHAR*		cFullPath,		// (I  ) �t�@�C���t���p�X��
						MCHAR*		cFolderPath,	// (  O) �t�H���_�[�p�X��	�܂��́@NULL
						MCHAR*		cFileName,		// (  O) �t�@�C����			�܂��́@NULL
						MCHAR*		cFileTitle,		// (  O) �t�@�C���^�C�g��	�܂��́@NULL
						MCHAR*		cFileExt		// (  O) �t�@�C���g���q		�܂��́@NULL
				)
{
	MINT		ist = 0;
	MINT		ic;
	MINT		szFullPath;
	MINT		szPath;
	MINT		szName;
	MINT		szTitle;

	szFullPath = (MINT)Mstrlen( cFullPath);
	for ( ic=szFullPath-1; ic>=0; ic--) {
		if ( cFullPath[ic] == Mchar('\\'))
			break;
	}
	szPath = ic;
	if ( cFolderPath) {
		if ( szPath > 1) {
			Mstrncpy( cFolderPath, cFullPath, szPath);
			cFolderPath[szPath] = NULL;
		} else {
			Mstrcpy( cFolderPath, Mstr( ".\\"));
		}
	}
	if ( cFileName)
		Mstrcpy( cFileName, &cFullPath[szPath+1]);
	
	szName = szFullPath - szPath - 1;
	if ( cFileTitle || cFileExt) {
		for ( ic=szName-1; ic>=0; ic--) {
			if ( cFileName[ic] == Mchar('.'))
				break;
		}
		szTitle = ic;
		if ( cFileTitle) {
			if ( szTitle < 0) {
				Mstrcpy( cFileTitle, cFileName);
			} else {
				if ( szTitle)
					Mstrncpy( cFileTitle, cFileName, szTitle);
				cFileTitle[szTitle] = NULL;
			}
		}
		if ( cFileExt) {
			if ( szTitle < 0) {
				cFileExt[0] = NULL;
			} else {
                Mstrcpy( cFileExt, &cFileName[szTitle+1]);
			}
		}
	}
}

} // namespace MC
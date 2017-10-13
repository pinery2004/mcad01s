#pragma once
//==========================================================================================
//  Copyright ( C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MbDebug.h
//
//	[�@�\] �g���[�X
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#define MBTRCMAXDEPTH			10							// �g���[�X�C���f���g�ő�[��
#define MBTRCINDENT				Mstr( "  ")					// �g���[�X�C���f���g
//S #define	MBTRCLOGFILENAME		Mstr( "MCADLOG.txt")		// �g���[�X�t�@�C����
#define MBTRCFUNCNAME			40							// �֐����ő啶����

#define MBTRCOPEN( fn)			MbLog::OpenTrace( fn)		// �g���[�X�J�n�錾�@�t�@�C����
#define MBTRCCLOSE				MbLog::CloseTrace( )		// �g���[�X�I���錾
#define MBTRCON					MbLog::Ctrl( 1)				// �g���[�X�@�n�m
#define MBTRCOFF				MbLog::Ctrl( 0)				// �g���[�X�@�n�e�e
#define MBTRCPRBF				MbLog::Trace( )				// �g���[�X�o�b�t�@���̕�����
#define MBTRCPRS( S)			MbLog::Trace( S)			// ������
#define MBTRCFLUSH				MbLog::Flush( )				// �t���b�V���i�g���[�X���̍�ƃG���A���������|�������j

#define MBTRCPRINTCR			MbLog::PrintCR( )			// ���s
#define MBTRCPRINTS( S)			MbLog::Print( S)			// ������@���s
#define MBTRCPRINTSS( S, T)		MbLog::Print( S, T)			// ������ = ������@���s
#define MBTRCPRINTI( S, I)		MbLog::Print( S, I)			// ������ = �����@���s
#define MBTRCPRINTIN( S, I, N)	MbLog::Print( S, I, N)		// ������ = N�̐����@���s
#define MBTRCPRINTB( S, B)		MbLog::Print( S, B)			// ������ = �o�C�g( MUBYTE)�����@���s
#define MBTRCPRINTBN( S, B, N)	MbLog::Print( S, B, N)		// ������ = N�̃o�C�g( MUBYTE)�����@���s
#define MBTRCPRINTF( S, F)		MbLog::Print( S, F)			// ������ = �����@���s
#define MBTRCPRINTFN( S, F, N)	MbLog::Print( S, F, N)			// ������ = N�̎����@���s

#include "MbBasic.h"

#ifdef DLL_EXPORT
	#undef DLL_EXPORT
#endif

#ifdef DLL_EXPORT_MBLOG_DO
	#pragma message( "<<< MbLog�Edll_EXport >>>")
	#define DLL_EXPORT							__declspec( dllexport)
#else
	#ifdef _DEBUG								// Release Mode �� inline�֐���undef�ɂȂ�s��Ή�
		#ifdef DLL_NO_IMPORT_MBASIC_DO
			#define DLL_EXPORT
		#else
//			#pragma message( "=== MbLog�Edll_IMport ===")
			#define DLL_EXPORT						__declspec( dllimport)
		#endif
	#else
		#define DLL_EXPORT
	#endif
#endif

namespace MC
{

//===========================================================================
//�y�@�\�z	�g���[�X
//
class DLL_EXPORT MbLog
{
public:
	static MCHAR	m_Str[1000];
	static FILE*	m_File;
	static MINT		m_iCtl;

public:
	static void	OpenTrace( MCHAR* i_pcFileName);
	static void	CloseTrace( );
	static void	Ctrl( int ictrl);
	static int	Wait( MCHAR* str);
	static void	Flush( );
	static void	PrintCR( );
	static void	Print( MCHAR* str);
	static void	Print( MCHAR* str, const MCHAR* s1);
	static void	Print( MCHAR* str, MINT i1);
	static void	Print( MCHAR* str, MINT *i1, int ni1);
	static void	Print( MCHAR* str, MUBYTE i1);
	static void	Print( MCHAR* str, MUBYTE *i1, int ni1);
	static void	Print( MCHAR* str, MREAL f1);
	static void	Print( MCHAR* str, MREAL *f1, int nf1);
	static void	Trace( MCHAR* str);
	static void	Trace( );
};

#ifdef DLL_EXPORT
	#undef DLL_EXPORT
#endif

#ifdef DLL_EXPORT_TRACE_DO
	#pragma message( "<<< Trace�Edll_EXport >>>")
	#define DLL_EXPORT							__declspec( dllexport)
#else
	#ifdef _DEBUG								// Release Mode �� inline�֐���undef�ɂȂ�s��Ή�
		#ifdef DLL_NO_IMPORT_MBASIC_DO
			#define DLL_EXPORT
		#else
//			#pragma message( "=== Trace�Edll_IMport ===")
			#define DLL_EXPORT						__declspec( dllimport)
		#endif
	#else
		#define DLL_EXPORT
	#endif
#endif

//===========================================================================
//�y�@�\�z	�g���[�X
//
class  DLL_EXPORT Trace
{
private:
	static	MCHAR	ms_cLogFilePath[MAX_PATH];					// �g���[�X�t�@�C����
	static	FILE*	ms_pfp;										// �g���[�X�t�@�C���|�C���^
	static	MINT	ms_iDepth;									// �g���[�X�C���f���g�[��
	static	MCHAR	ms_cDepth[Msizeof( MBTRCINDENT) * (MBTRCMAXDEPTH + 1)];
																// �g���[�X�C���f���g
			MCHAR	m_cFuncName[MBTRCFUNCNAME];					// �֐���
	
public:
	Trace( void);
	Trace( MCHAR* i_cFuncName);
	~Trace( void);
	// ���O�t�@�C���̍폜�ƃI�[�v��
	static void OpenLogFile(
						MCHAR i_cLogFilePath[]	// ���O�t�@�C���p�X
						);
	// ���O�t�@�C���̃N���[�Y
	static void CloseLogFile( void);
	// ���O�f�[�^��������
	void Write( MCHAR i_cFormat[], ...);
	// ���O�f�[�^�s��������
	void WriteLine( MCHAR i_cOut[]);

};
//extern	CMbDebug g_MbDbg;

} // namespace MC
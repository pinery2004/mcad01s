#pragma once
//==========================================================================================
//  Copyright ( C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MbBasic.h
//
//		��{�@�萔�E�֐�
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#ifndef WINVER				// Windows XP �ȍ~�̃o�[�W�����ɌŗL�̋@�\�̎g�p�������܂��B
#define WINVER 0x0501		// ����� Windows �̑��̃o�[�W���������ɓK�؂Ȓl�ɕύX���Ă��������B
#endif

#pragma warning( disable : 4996 )

#include <afxwin.h> 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//===========================================================================
//	�V�X�e���ݒ�

//===========================================================================
// �����̐��x	�{���x�����Ŏ�舵���ꍇ�́AMG_DOUBLE_PRECISION_REAL���`
//
// #define		MG_DOUBLE_PRECISION_REAL

//===========================================================================
//		�}�b�v ���[�h
//
//#define		MMAPMODE			MM_HIMETRIC	// 0.01mm
#define		MMAPMODE			MM_TWIPS		// 1/20�|�C���g
//#define		MMAPMODE			MM_TEXT

#if( MMAPMODE == MM_TEXT)
	#define		MMDIRY			(-1)
#else
	#define		MMDIRY			1
#endif

//===========================================================================
//		�i�v���[�v
//
#define		MFOREVER			for ( ;;)

//===========================================================================
//		�^�̒�`
//
typedef		_TCHAR				MCHAR;			// �����^
typedef		char				MBYTE;			// 1/4�ꐮ���^
typedef		short				MSHORT;			// 1/2�ꐮ���^
typedef		int					MINT;			// 32�r�b�g�����^
typedef		long				MLONG;			// 32�r�b�g�����^
typedef		__int64				MLONGLONG;		// 64�r�b�g�����^
typedef		float				MFLOAT;			// �P���x�����^
typedef		double				MDOUBLE;		// �{���x�����^

#ifdef MG_DOUBLE_PRECISION_REAL
	typedef		double				MREAL;			// �P���x�����^
#else
	typedef		float				MREAL;			// �{���x�����^
#endif

typedef		_TUCHAR				MUCHAR;			// �����Ȃ������^
typedef		unsigned char		MUBYTE;			// �����Ȃ�1/4�ꐮ���^
typedef		unsigned short		MUSHORT;		// �����Ȃ�1/2�ꐮ���^
typedef		unsigned int		MUINT;			// �����Ȃ�32�r�b�g�����^
typedef		unsigned long		MULONG;			// �����Ȃ�32�r�b�g�����^
typedef		unsigned __int64	MULONGLONG;		// �����Ȃ�64�r�b�g�����^

typedef		void				MVOID;			// VOID�^
typedef		void*				MHANDLE;		// HANDLE�^

typedef	 enum _mbool {							// �_���^
			MFALSE = 0,							// �U
			MTRUE  = 1							// �^
		} 						MBOOL;

typedef		MINT				MDID;			// ID	

typedef 	long 				MRC;			// ���^�[���R�[�h

//typedef	time_t				MTIME;			// ���Ԋl���p�ϐ��^
#define		MTIME				CTime			// ���Ԋl���p�ϐ��^

#define		MCONST				const			// �萔�^

//===========================================================================
//		�e�L�X�g ��Ɨ̈�T�C�Y
//
#define		MX_PATH				256				// �p�X������@�ő啶����

//===========================================================================
//		�f�[�^�^�萔�́A�����f�[�^�^�Ɏg�p�ł���l�͈̔�
//
#define		MCHARMAX			CHAR_MAX		// 127
#define		MCHARMIN			CHAR_MIN		// -128
#define		MUCHARMAX			UCHAR_MAX		// 255		( 0xff)
#define		MUSHORTMAX			USHRT_MAX		// 65535	( 0xffff)
#define		MSHORTMAX			SHRT_MAX		// 32767
#define		MSHORTMIN			SHRT_MIN		// -32768
#define		MUINTMAX			UINT_MAX		// 4294967295	( 0xffffffff)
#define		MINTMAX				INT_MAX			// 2147483647
#define		MINTMIN				INT_MIN			// -2147483647-1
#define		MLONGLONGMAX		I64_MAX			// 9223372036854775807
#define		MLONGLONGMIN		I64_MIN			// -9223372036854775807-1
#define		MULONGLONGMAX		_UI64_MAX		// ( 0xffffffffffffffff)
//		 ( from FLOAT.H)
#define		MDOUBLEMAX			DBL_MAX			// 1.7976931348623158e+308
#define		MDOUBLEMIN			DBL_MIN			// 2.2250738585072014e-308
#define		MFLOATMAX			FLT_MAX			// 3.402823466e+38F
#define		MFLOATMIN			FLT_MIN			// 1.175494351e-38F

#ifdef MG_DOUBLE_PRECISION_REAL
	#define		MREALMAX			DBL_MAX			// 1.7976931348623158e+308
	#define		MREALMIN			DBL_MIN			// 2.2250738585072014e-308
#else
	#define		MREALMAX			FLT_MAX			// 3.402823466e+38F
	#define		MREALMIN			FLT_MIN			// 1.175494351e-38F
#endif

// �T�C�Y
inline MINT SZMINT( MINT i_sz = 1)		{ return  ( i_sz * (MINT)sizeof( MINT));}
inline MINT SZMREAL( MINT i_sz = 1)	{ return  ( i_sz * (MINT)sizeof( MREAL));}

//===========================================================================
//		�S�I�u�W�F�N�g�̃x�[�X
//
class 		MOBJECT				{};				// �S�I�u�W�F�N�g�̃x�[�X

//===========================================================================
//		abstract iteration position
//
class 		__MPOSITION 		{};
//typedef 	__MPOSITION* 		MPOSITION;
#define		MPOSITION		 	__MPOSITION*

//===========================================================================
//		���l���Z�萔
//
#define		MC_E				2.71828182845904523536		// e
#define		MC_LOG2E			1.44269504088896340736		// log2( e)
#define		MC_LOG10E			0.434294481903251827651		// log10( e)
#define		MC_LN2				0.693147180559945309417		// ln( 2)
#define		MC_LN10				2.30258509299404568402		// ln( 10)

#define		MC_2PIE				6.28318530717958647692		// 2*�~����
#define		MC_PIE				3.14159265358979323846		// �~����
#define		MC_PIE_2			1.57079632679489661923		// �~����/2
#define		MC_PIE_4			0.785398163397448309616		// �~����/4
#define		MC_1_PIE			0.318309886183790671538		// 1/�~����
#define		MC_2_PIE			0.636619772367581343076		// 2/�~����

#define		MC_DTR				0.0174532925199433f			// �~����/180
#define		MC_RTD				57.2957795130824f			// 180/�~����
#define		MC_INCHES_TO_MM		25.4f;

#define		MC_SQRTPIE			1.77245385090551602729f		// (MREAL)sqrt( �~����)
#define		MC_2_SQRTPIE		1.12837916709551257390f		// 2/sqrt( �~����)
#define		MC_SQRT2			1.41421356237309504880f		// (MREAL)sqrt( 2)
#define		MC_SQRT1_2			0.707106781186547524401f	// 1/sqrt( 2)

//===========================================================================
//		�����񑀍�
//
#define		Mchar( cha)			_TEXT( cha)		// �L�����N�^UNICODE�ϊ�
#define		Mstring( str)		_TEXT( str)		// ������UNICODE�ϊ�
#define		Mstr( str)			_TEXT( str)		// ������UNICODE�ϊ�
#define		MBCstr( str)		( str)			// ������MBC	���ϊ�
#define		Mgetchar( str, n)	string[number]	// �����񒆂̃L�����N�^�̎擾
#define		Msizeof( str)		( sizeof( str)/sizeof( MCHAR)-1)
												// ������T�C�Y
#define		Mstrcpy				_tcscpy			// ������̕���
#define		Mstrcpy_s			_tcscpy_s		// ������̕���
#define		Mstrncpy			_tcsncpy		// ������̕��� (��������NULL���܂܂Ȃ�)
#define		Mstrncpy_s			_tcsncpy_s		// ������̕��� (��������NULL���܂܂Ȃ�)�@( ����NULL��t���ăR�s�[)
#define		Mstrcat				_tcscat			// ������̘A��
#define		Mstrcat_s			_tcscat_s		// ������̘A��
#define		Mstrncat			_tcsncat		// ������̘A�� (��������NULL���܂܂Ȃ�)
#define		Mstrncat_s			_tcsncat_s		// ������̘A�� (��������NULL���܂܂Ȃ�)
#define		Mstrcmp				_tcscmp			// ������̔�r
#define		Mstrcmp_s			_tcscmp_s		// ������̔�r (��������NULL���܂܂Ȃ�)
#define		Mstricmp			_tcsicmp		// ������������������Ĕ�r
#define		Mstricmp_s			_tcsicmp_s		// ������������������Ĕ�r (��������NULL���܂܂Ȃ�)
#define		Mstrncmp			_tcsncmp		// ������̔�r (��������NULL���܂܂Ȃ�)
#define		Mstrncmp_s			_tcsncmp_s		// ������̔�r (��������NULL���܂܂Ȃ�)
#define		Mstrnicmp			_tcsnicmp		// ������������������Ĕ�r (��������NULL���܂܂Ȃ�)
#define		Mstrnicmp_s			_tcsnicmp_s		// ������������������Ĕ�r (��������NULL���܂܂Ȃ�)
#define		Mstrchr				_tcschr			// �����񒆂̕����̌��� (str ���� c ���ŏ��ɏo������ʒu���w���|�C���^��Ԃ��Bc ��������Ȃ��ꍇ�́ANULL ��Ԃ��B)
#define		Mstrchr_s			_tcschr_s		// �����񒆂̕����̌��� (str ���� c ���ŏ��ɏo������ʒu���w���|�C���^��Ԃ��Bc ��������Ȃ��ꍇ�́ANULL ��Ԃ��B)
#define		Mstrrchr			_tcsrchr		// �����񒆂̕����̌��� (str ���� c ���Ō�ɏo������ʒu���w���|�C���^��Ԃ��Bc ��������Ȃ��ꍇ�́ANULL ��Ԃ��B)
#define		Mstrrchr_s			_tcsrchr_s		// �����񒆂̕����̌��� (str ���� c ���Ō�ɏo������ʒu���w���|�C���^��Ԃ��Bc ��������Ȃ��ꍇ�́ANULL ��Ԃ��B)
#define		Mstrstr				_tcsstr			// �����񒆂̕�����̌���
#define		Mstrstr_s			_tcsstr_s		// �����񒆂̕�����̌���
//#define	Mstrrstr			_tcsrstr		// �����񒆂̕�����̌���
#define		Mstrtok				_tcstok			// �����񒆂̍ŏ��̃g�[�N�������o��
#define		Mstrtok_s			_tcstok_s		// �����񒆂̍ŏ��̃g�[�N�������o��
#define		Mstrlen				_tcslen			// ������̕��������擾
#define		Mstrlen_s			_tcslen_s		// ������̕��������擾
//	���o��
#define		Msprintf			_stprintf		// �������̈�ւ̏����t���o��
#define		Msprintf_s			_stprintf_s		// �������̈�ւ̏����t���o��
#define		M_snprintf			_sntprintf		// �������̈�ւ̏����t���o��
#define		Mscanf				_tscanf			// �����t������
#define		Msscanf				_stscanf		// �������̈悩��̏����t������

#define		Mprintf				_tprintf		// �����t���o��
#define		Mfprintf			_ftprintf		// �t�@�C���ւ̏����t���o��
#define		Mfscanf				_ftscanf		// �t�@�C������̏����t������

//	�]��
#define		Misalnum			_istalnum		// �p�����ł���
#define		Misalpha			_istalpha		// �p�����ł���
#define		Miscntrl			_istcntrl		// �폜�����܂��͐��䕶���ł���
#define		Misdigit			_istdigit		// 0-9�̐����ł���
#define		Misgraph			_istgraph		// ���O���t�B�b�N�����ł���
#define		Mislower			_istlower		// �p�������ł���
#define		Misprint			_istprint		// ����p�����ł���
#define		Mispunct			_istpunct		// ��Ǔ_�ł���
#define		Misspace			_istspace		// SPACE,TAB,RETURN,NEWLINE,FORMFEED,�����^�u
#define		Misupper			_istupper		// �p�啶���ł���
#define		Misxdigit			_istxdigit		// 0-9,A-F,a-f��16�i�����ł���

//	�ϊ�
#define		Mtolower			_totlower		// �������������ɕϊ�
#define		Mtoupper			_totupper		// ������啶���ɕϊ�
#define		Mstrlwr				_tcslwr			// ������̏�������
#define		Mstrlwr_s			_tcslwr_s		// ������̏�������
#define		Mstrupr				_tcsupr			// ������̑啶����
#define		Mstrupr_s			_tcsupr_s		// ������̑啶����
#define		Matoi				_tstoi			// ������𐮐��ɕϊ�
#define		Mwtoi				_tstoi			// ������𐮐��ɕϊ�
#define		Matol				_tstol			// ������𐮐��ɕϊ�
#define		Mwtol				_tstol			// ������𐮐��ɕϊ�
#define		Matof				( MREAL)_tstof	// ������������ɕϊ�
#define		Mwtof				( MREAL)_tstof	// ������������ɕϊ�
#define		Matodbl				_tstodbl		// ������������ɕϊ�
#define		Mwtodbl				_tstodbl		// ������������ɕϊ�

//===========================================================================
//		�t�@�C������
//
#define		Mfopen				_tfopen			// �t�@�C���I�[�v��
#define		Mfclose				fclose			// �t�@�C���N���[�Y
#define		Mfflush				fflush			// �X�g���[�����t���b�V��

//===========================================================================
//		�o�b�t�@���� fill memory, copy memory, copy memcopys
//
//===========================================================================
//	�����𕶎���S�̂ɃR�s�[����( fill)
//
inline void MbFill( void *sdest, char cdt, int nsz)
{
	memset( sdest, cdt, nsz);
	//	for ( int n=0; n<nsz; sdest[n++]=cdt);
}

//===========================================================================
//	�����𐮐��z��S�̂ɃR�s�[����( fill)
//
inline void MbFill( int* idest, int idt, int nsz)
{
	for ( int n=0; n<nsz; idest[n++]=idt);
}

//===========================================================================
//	�����������z��S�̂ɃR�s�[����( fill)
//
inline void MbFill( MREAL *rdest, MREAL rdt, int nsz)
{
	for ( int n=0; n<nsz; rdest[n++]=rdt);
}

//===========================================================================
//	�����z�񂩂當���z��փR�s�[����( memcpy)
//
inline void MbCopy( char *sdest, char *ssource, int nsz)
{
	memcpy( sdest, ssource, nsz);
//	for ( int n=0; n<nsz; n++) sdest[n] = ssource[n];
}

//===========================================================================
//	�����z�񂩂琮���z��փR�s�[����( memcpy)
//
inline void MbCopy( int* idest, int* isource, int nsz)
{
	memcpy( idest, isource, nsz*4);
//	for ( int n=0; n<nsz; n++)
//		idest[n] = isource[n];
}

//===========================================================================
//	�����z�񂩂�����z��փR�s�[����( memcpy)
//
inline void MbCopy( MREAL *rdest, MREAL *rsource, int nsz)
{
	memcpy( rdest, rsource, nsz*4);
//	for ( int n=0; n<nsz; n++)
//		idest[n] = isource[n];
}

//===========================================================================
//	�����z�񂩂當���z��փR�s�[����( memcpy)
//	�̈�̏d�Ȃ������
//
inline void MbCopyS( MCHAR* sdest, MCHAR* ssource, int nsz)
{
	if( sdest < ssource) {
		for ( int n=0; n<nsz; n++)
			sdest[n] = ssource[n];
	} else {
		for ( int n=nsz-1; n>=0; n--)
			sdest[n] = ssource[n];
	}
}

//===========================================================================
//	�����z�񂩂琮���z��փR�s�[����( memcpy)
//	�̈�̏d�Ȃ������
//
inline void MbCopyS( int* idest, int* isource, int nsz)
{
	if( idest < isource) {
		for ( int n=0; n<nsz; n++)
			idest[n] = isource[n];
	} else {
		for ( int n=nsz-1; n>=0; n--)
			idest[n] = isource[n];
	}
}

//===========================================================================
//	�����z�񂩂�����z��փR�s�[����( memcpy)
//	�̈�̏d�Ȃ������
//
inline void MbCopyS( MREAL *rdest, MREAL *rsource, int nsz)
{
	if( rdest < rsource) {
		for ( int n=0; n<nsz; n++)
			rdest[n] = rsource[n];
	} else {
		for ( int n=nsz-1; n>=0; n--)
			rdest[n] = rsource[n];
	}
}

//===========================================================================
//		�o�b�t�@�Ǘ�
//

//===========================================================================
//	MbMAlcBuf
//
//		�T�C�Y�t���o�b�t�@
//
class	MbMAlcBuf
{
public:
	MINT	sz;
	void*	m_Buf;
public:
	MbMAlcBuf( )				{ m_Buf = NULL;}

	~MbMAlcBuf( )				{ if ( m_Buf) delete [] ( char*)m_Buf;}
};

//===========================================================================
//	MBMALLOC( �A���P�[�V�����������A�h���X�ݒ�|�C���^, �ϐ��^, �m�ۗv���T�C�Y);
//
//		�f�[�^�^���w�肵���������A���P�[�V����
//
//	��	MBMALLOC( dd, MREAL, MX_PNT1);
//			��
//		MREAL	*dd = ( MREAL*)new char[MX_PNT1 * sizeof( MREAL)];
//				MBCHECK_POINTER( dd);
//
#define		MBMALLOC( P, T, S)	{ P = ( T*)new char[( S) * sizeof( T)]; MBCHECK_POINTER( P);}
#define		MBMALLOCF( F, P, T, S)	{ P = ( F*)new char[( S) * sizeof( T)]; MBCHECK_POINTER( P);}

//===========================================================================
//	MBFREE( �A���P�[�V�����������A�h���X�ݒ�|�C���^);
//
//		�������A���P�[�V�����̈�̊J��
//
//	��	MBFREE( dd);
//			��
//		if ( dd) {
//			delete [] ( char*)dd;
//			dd = NULL;
//		}
//
#define		MBFREE( P)			{ if ( P) { delete [] ( char*)P; P = NULL;}}

//===========================================================================
//	MBFREEST( �A���P�[�V�����������A�h���X�ݒ�|�C���^);
//
//		�\���̂̊J���ƃ������A���P�[�V�����̈�̊J��
//
//	��	MBFREE( dd);
//			��
//		if ( dd) {
//			dd->Freee( );
//			delete [] ( char*)dd;
//			dd = NULL;
//		}
//
#define		MBFREEST( P)		{ if ( P) { P->Free( ); delete [] ( char*)P; P = NULL;}}

////===========================================================================
////	MBDELETE( new�ō쐬�z��̈�̐擪�A�h���X�ݒ�|�C���^);
////
////	��	MBDELETEH( dd);
////			��
////		if ( dd) {
////			delete [] dd;
////			dd = NULL;
////		}
////
//#define		MBDELETEH( P)	{ if ( P) {delete [] ( char*)P ; P = NULL;}}

//===========================================================================
//	MBDELETE( new�ō쐬�̈�̐擪�A�h���X�ݒ�|�C���^);
//
//	��	MBDELETE( dd);
//			��
//		if ( dd) {
//			delete dd;
//			dd = NULL;
//		}
//
#define		MBDELETE( P)		{ if ( P) { delete P ; P = NULL;}}

//===========================================================================
//	�萔�Ŏw�肵���T�C�Y�̔z����m�ۂ���
//
//	MBBUFF( �쐬�̈�̐擪�A�h���X�ݒ�|�C���^, �ϐ��^, �����z��T�C�Y( �萔)); 
//
//	��	MBBUFF( dd, MREAL, MX_PNT1);
//			��
//		MREAL		*dd;
//		MbMAlcBuf	MAB_dd;
//		MREAL		MHB_dd[MX_PNT1];
//		MAB_dd.sz = MX_PNT1;
//		dd = MHB_dd;
//
//	MBBUFF( 	iPtW1,	MINT,	20); 
//		MINT *iPtW1; MbMAlcBuf MAB_iPtW1; MINT MHB_iPtW1[20]; MAB_iPtW1.sz = 20; iPtW1 = MHB_iPtW1;

#define		MBBUFF( P, T, M)\
								 T *P; MbMAlcBuf MAB_ ## P; T MHB_ ## P[M]; MAB_ ## P.sz = ( M); P = MHB_ ## P;

//===========================================================================
//	�萔�Ŏw�肵���T�C�Y�̔z����m�ۂ���
//		�����ɒ萔�Ŏw�肵���T�C�Y�̔z����`���Ă��邪�ϐ��Ŏw�肵���T�C�Y�̕����傫���ꍇ��malloc�ō쐬����B
//		malloc�Ŋm�ۂ��ꂽ�̈��Prog�̃��^�[�����Ɏ����I�ɍ폜�����
//
//	MBMALCBUFF( �쐬�̈�̐擪�A�h���X�ݒ�|�C���^, �ϐ��^, �m�ۗv���T�C�Y, �����z��T�C�Y( �萔)); 
//
//	��	MBMALCBUFF( dd, MREAL, GPt2->n, MX_PNT1);
//			��
//		MREAL		*dd;
//		MbMAlcBuf	MAB_dd;
//		MREAL		MHB_dd[MX_PNT1];
//		if ( GPt2->n > MX_PNT1) {
//			MAB_dd.sz = GPt2->n;
//			MAB_dd.m_Buf = dd = ( MREAL *)new char[GPt2->n * sizeof( MREAL)];
//					MBCHECK_POINTER( dd);
//		} else {
//			MAB_dd.sz = MX_PNT1;
//			dd = MHB_dd;
//		}
//
#define		MBMALCBUFF( P, T, S, M)\
								 T *P; MbMAlcBuf MAB_ ## P; T MHB_ ## P[M];\
								 if ( ( S) > ( M)) {MAB_dd.sz = ( S); MAB_ ## P.m_Buf = P = ( T *)new char[( S) * sizeof( T)]; MBCHECK_POINTER( P);}\
								 else {MAB_dd.sz = ( M); P = MHB_ ## P;}

////===========================================================================
////	MBBUFF�܂���MBMALCBUFF�Ŋm�ۂ����̈�̃T�C�Y��ύX����
////
////	MBREALCBUFF( ���A���P�[�V�����������A�h���X�ݒ�|�C���^, �ϐ��^, �m�ۗv���T�C�Y);
////
////	��	MBREALCBUFF( dd, MREAL, GPt2->n);
////			��
////		if ( GPt2->n > MAB_dd.sz) {
////			if ( MAB_dd.m_Buf) {
////				MAB_dd.m_Buf = dd = ( MREAL*)realloc( dd, sizeof( MREAL) * GPt2->n);
////			} else {
////				MAB_dd.m_Buf = dd = new char[GPt2->n * sizeof( MREAL)];
////				memcpy( dd, MHB_dd, sizeof( MREAL) * MAB_dd.sz);
////			}
////			MAB_dd.sz = GPt2->n;
////		}
////				MBCHECK_POINTER( dd);
////
////#define		MBREALCBUFF( P, T, S)\
////								if ( S > MAB_ ## P.sz) {\
////									if ( MAB_ ## P.m_Buf) {MAB_ ## P.m_Buf = P = ( T*)realloc( P, sizeof( T) * S);}\
////									else {MAB_ ## P.m_Buf = P = ( T*)new char[S * sizeof( T)]; memcpy( P, MHB_ ## P, sizeof( T) * MAB_ ## P.sz);}\
////									MAB_ ## P.sz = S;\
////								} MBCHECK_POINTER( P);}
//#define		MBREALCBUFF( P, T, S)\
//								if ( S > MAB_ ## P.sz) {\
//									if ( MAB_ ## P.m_Buf) {MAB_ ## P.m_Buf = P = ( T*)realloc( P, sizeof( T) * S);}\
//									else {MAB_ ## P.m_Buf = P = new T[ S]; memcpy( P, MHB_ ## P, sizeof( T) * MAB_ ## P.sz);}\
//									MAB_ ## P.sz = S;\
//								} MBCHECK_POINTER( P);}

//===========================================================================
//		��������
//
#define		MBRED( Col)			( Col & 0xFF)
#define		MBGREEN( Col)		( ( Col >> 8) & 0xFF)
#define		MBBLUE( Col)		( ( Col >> 16)& 0xFF)

//===========================================================================
//		�t���O�`�F�b�N
//

inline bool MF_CHECK_ON( MINT a, MINT mask)						// mask�̏������n�m�ł��邩�`�F�b�N
				{ return ( ( a & mask) != 0);}

inline bool MF_CHECK_OR( MINT a, MINT mask)						// mask�̂����ꂩ�̏������n�m�ł��邩�`�F�b�N
				{ return ( ( a & mask) != 0);}

inline bool	MF_CHECK_AND( MINT a, MINT mask)					// mask�̑S�Ă̏������n�m�ł��邩�`�F�b�N
				{ return ( ( a & mask) == mask);}

inline bool	MF_CHECK_EQ( MINT a, MINT mask)						// mask�̑S�Ă̏������Z�b�g����Ă���
				{ return ( a == mask);}							// ���̏����̃Z�b�g���������Ƃ��`�F�b�N

inline bool	MF_CHECK_OFF( MINT a, MINT mask)					// mask�̏������n�e�e�ł��邩�`�F�b�N
				{ return ( ( ~( a) & mask) != 0);}

inline bool	MF_CHECK_OFF_OR( MINT a, MINT mask)					// mask�̂����ꂩ�̏������n�e�e�ł��邩�`�F�b�N
				{ return ( ( ~( a) & mask) != 0);}
				
inline bool	MF_CHECK_OFF_AND( MINT a, MINT mask)				// mask�̑S�Ă̏������n�e�e�ł��邩�`�F�b�N
				{ return ( ( ~( a) & mask) == mask);}

inline void MF_SET_ON( MINT &a, MINT mask)						// mask�̏������Z�b�g����
				{ a |= mask;}

inline void MF_SET_OFF( MINT &a, MINT mask)						// mask�̏������N���A����
				{ a &= ~( mask);}

inline void MF_RESET_ON( MINT &a, MINT mask)					// mask�̏������N���A����
				{ a &= ~( mask);}

inline void MF_RESET( MINT &a)									// �O�������N���A����
				{ a = 0;}

inline bool MI_CHECK_ODD( MINT n)								// ��`�F�b�N
				{ return ( n & 1);}

inline bool MI_CHECK_EVEN( MINT n)								// �����`�F�b�N
				{ return ( !MI_CHECK_ODD( n));}

#define 	MQUIT				{ goto exit;}
 				
#define 	MQUIT_IF_FAILED( p)	{ if ( !p) { rc = DM_MEM; goto exit;}}

#define		MQUIT_STS( s)		{ iSt = s; goto exit;}

//===========================================================================
//		�|�C���^�`�F�b�N
//
//		�A�N�Z�X�\�ȃ|�C���^�ł��鎖���`�F�b�N����
//
//		CHECKDATA( p, n)	:	( p != 0) && ( p ���� n �o�C�g�̃A�N�Z�X�\)
//		CHECKPTR( p)		:	( p != 0) && ( p ���� 1 �o�C�g�̃A�N�Z�X�\)
//		CHECKFUNCPTR( p) 	:	( p != 0) && ( p �ɃA�N�Z�X�\)
//		CHECKSTR( s)		:	( p != 0) && ( p ���� 30000�����̃A�N�Z�X�\)
//		CHECKSTRUCTPTR( p)	:	( p != 0) && ( p ���� �\���̂̃T�C�Y���̃A�N�Z�X�\)
//  
#define CHECKDATA( p, n)		( ( p) && !IsBadReadPtr( ( LPVOID)( p), ( n)))
#define CHECKPTR( p)			CHECKDATA( p, 1)
#define CHECKFUNCPTR( p) 		( ( p) && !IsBadCodePtr( ( FARPROC) ( p)))
#define CHECKSTR( s)			( ( s) && !IsBadStringPtr( ( LPCSTR)( s), 30000))
#define CHECKSTRUCTPTR( p)		CHECKDATA( p, sizeof( *p)) 

//===========================================================================
//		�A���P�[�V�������`�F�b�N
//			( out of memory)
//
//	�g�p���@:
//		  rc = MALLOC_FAIL( p = new CMyClass( a, b, c));
//
//	����: rc = DM_MEM: �A���P�[�V�������
//		  rc = SUCCESS: �A���P�[�V��������
//
#define MALLOC_FAIL( object) ( object) ? SUCCESS: DM_MEM

//===========================================================================
//		�^�C�v���w�肵���Q�̃I�u�W�F�N�g������
//
#define MSWAP( x,y,type)			{type z = x; x = y; y = z;}

//===========================================================================
//		0�`1. �Ԃ� �����_�������l�𔭐�
//
#define FRAND	( ( double)rand( )/( double)RAND_MAX)


namespace MC
{

//===========================================================================
//		�ėp�֐�
//

#ifdef DLL_EXPORT
	#undef DLL_EXPORT
#endif

#ifdef DLL_EXPORT_BASE_DO
	#pragma message( "<<< Base�Edll_EXport >>>")
	#define DLL_EXPORT							__declspec( dllexport)
#else
	#ifdef _DEBUG								// Release Mode �� inline�֐���undef�ɂȂ�s��Ή�
		#ifdef DLL_NO_IMPORT_MBASIC_DO
			#define DLL_EXPORT
		#else
//			#pragma message( "=== Base�Edll_IMport ===")
			#define DLL_EXPORT					__declspec( dllimport)
		#endif
	#else
		#define DLL_EXPORT
	#endif
#endif

class DLL_EXPORT Base
{
public:
//===========================================================================
//		�g���[�X

/////////////////////////////////////////////////////////////////////////////
//		���{�ꃂ�[�h

static void 	MbSetJapanese( );

/////////////////////////////////////////////////////////////////////////////
//		�V�X�e���G���[�X�e�C�^�X

#define	 	MC_ERR_COMMAND		1200			// �R�}���h�G���[
#define	 	MC_ERR_ID			1201			// �h�c���G���[
#define		MC_ERR_POINTER		1202			// �������擾�G���[
#define		MC_ERR_OVERFLOW		1203			// �̈�I�[�o�t���[�G���[
#define		MC_ERR_OTHER	   -999				// ���̑��G���[

//		�������擾�G���[�\��
#define		MB_DM_MEM			{ MbSysError( X, MC_ERR_POINTER);}

/////////////////////////////////////////////////////////////////////////////
//		�X�e�C�^�X
#define		MC_SUCCESS			  0
#define		MC_FAILURE			 ~0

#define		MC_E_CANCEL			 13
#define		MC_E_NOMSG			 14

//S void	MbSysErrorId( MINT id_err);

/////////////////////////////////////////////////////////////////////////////
//		���^�[��
#define		RETURN_ID_ERR( CPROG)\
								Base::MbSysError( CPROG, MC_ERR_ID)
#define		RETURN_POINTER_ERR( CPROG)\
								Base::MbSysError( CPROG, MC_ERR_POINTER)
#define		RETURN_OVERFLOW_ERR( CPROG)\
								Base::MbSysError( CPROG, MC_ERR_OVERFLOW)
#define		RETURN_OTHER_ERR( CPROG)\
								MbSysError( CPROG, MC_ERR_OTHER)
#define		GOTO_ERROR			{ Base::MbErrorReturn(); goto ERR_EXIT;}
#define		GOTO_ERROR_1		{ Base::MbErrorReturn(); goto ERR_EXIT_1;}
#define		GOTO_ERROR1			{ Base::MbErrorReturn(); goto ERR_EXIT1;}
#define		GOTO_ERROR2			{ Base::MbErrorReturn(); goto ERR_EXIT2;}
#define		GOTO_ERROR3			{ Base::MbErrorReturn(); goto ERR_EXIT3;}
#define		GOTO_ERROR4			{ Base::MbErrorReturn(); goto ERR_EXIT4;}
#define		GOTO_ERROR999		{ Base::MbErrorReturn(); goto ERR_EXIT999;}

static void 	MbSysError( 
						const char*		i_Program,	// �G���[�v���O����
						MINT			i_ErrorCode	// �G���[�R�[�h
					);
static void 	MbCmdError( 
						const char*		i_Program,	// �G���[�v���O����
						MINT			i_ErrorCode	// �G���[�R�[�h
					);

//===========================================================================
//	�G���[���^�[���u���[�N�|�C���g�p
//
void	Base::MbErrorReturn( void);

/////////////////////////////////////////////////////////////////////////////////
//////		���^�[���R�[�h
////
////static short shortCast( int i)
////{
////	ASSERT( -MINSHORT <= i && i <= MAXSHORT);
////	return (short)i;
////}

////struct MbRT
////{
////	int	m_iRt;
////
////	MbRT()				: m_iRt( 0)		{}
////	MbRT( int i_iRt)	: m_iRt( i_iRt)	{}
////	
////	operator short()			{ return shortCast( m_iRt);}
////
////	bool operator &&( bool b)	{ return (*this != 0) && b;}
////	bool operator ||( bool b)	{ return (*this != 0) || b;}
////	bool operator ==( int i)	{ return m_iRt == i;}
////	bool operator !=( int i)	{ return m_iRt != i;}
////	bool operator < ( int i)	{ return m_iRt <  i;}
////	bool operator < ( long lg)	{ return m_iRt <  lg;}
////	bool operator <=( int i)	{ return m_iRt <= i;}
////	bool operator > ( int i)	{ return m_iRt >  i;}
////	bool operator >=( int i)	{ return m_iRt >= i;}
////
////};

};

//
//	MGCHECK_ZEROVECT( �[���`�F�b�N�̃x�N�g����, "�v���O������");
#define		MBCHECK_ZEROVECT(X,Z,T)\
								{ if ( ( X) < T)  Base::MbSysError( Z, MC_ERR_VECT_ZERO);}

#define		MBCHECK_POINTER(P)	{ if ( P == NULL) Base::MbSysError( __FILE__ + __LINE__, MC_ERR_POINTER);}


#ifdef DLL_EXPORT
	#undef DLL_EXPORT
#endif

#ifdef DLL_EXPORT_CODE_DO
	#pragma message( "<<< Code�Edll_EXport >>>")
	#define DLL_EXPORT							__declspec( dllexport)
#else
	#ifdef _DEBUG								// Release Mode �� inline�֐���undef�ɂȂ�s��Ή�
		#ifdef DLL_NO_IMPORT_MBASIC_DO
			#define DLL_EXPORT
		#else
//			#pragma message( "=== Code�Edll_IMport ===")
			#define DLL_EXPORT					__declspec( dllimport)
		#endif
	#else
		#define DLL_EXPORT
	#endif
#endif

class DLL_EXPORT Code
{
public:
// *******************************************************************************
//										Created on 10/01/00 by K.Matsubayashi
//										Updated on xx/xx/xx/ by x.xxxxxx
//										Copyright
//�y�@�\�zMBCS�������UNICODE������ɕϊ�����
//�y�Ԓl�z 0 : ����     -1 : �ُ�
//�y���Ӂz�Ȃ�
// ********************************************************************************

	static MINT Mmbstowcs( 
						const char*		i_StrC,			// MBCS������
						MCHAR*			o_StrU,			// UNICODE������
						MINT			i_iSize			// �ő啶����
						);

// *******************************************************************************
//										Created on 10/01/00 by K.Matsubayashi
//										Updated on xx/xx/xx/ by x.xxxxxx
//										Copyright
//�y�@�\�zUNICODE�������MBCS������ɕϊ�����
//�y�Ԓl�z 0 : ����     -1 : �ُ�
//�y���Ӂz�Ȃ�
// ********************************************************************************

	static MINT Mwcstombs( 
						const MCHAR*	i_StrU,			// UNICODE������
						char*			o_StrC,			// MBCS������
						MINT			i_iSize			// �ő啶����
						);
//};

// ��������������ɕϊ�
static void Mstrtolower(
						const MCHAR*	i_pstr,			// �ϊ��O�̕�����
						MCHAR			o_pstrL[],		// �ϊ���̕�����o�b�t�@
						size_t			i_SzstrL)		// ������o�b�t�@�T�C�Y
{
	const MCHAR*	pstr;
	MCHAR*	pstrL;
	size_t	available;

	pstr = i_pstr;
	pstrL = o_pstrL;
	available = i_SzstrL;
	while( *pstr != 0 && --available) {
		*pstrL++ = Mtolower( *pstr++);
	}
	if ( available == 0) {
		RETURN_OVERFLOW_ERR( __FUNCTION__);
	}
	*pstrL = 0;
}

// �������啶���ɕϊ�
static void Mstrtoupper(
						MCHAR	in_pstr[],		// �ϊ��O�̕�����
						MCHAR	out_pstrU[],	// �ϊ���̕�����o�b�t�@
						MINT	in_SzstrU)		// �ő啶����(�o�b�t�@�T�C�Y - 1)
{
	MCHAR*	pstr;
	MCHAR*	pstrU;
	size_t	available;

	pstr = in_pstr;
	pstrU = out_pstrU;
	available = in_SzstrU;
	while( *pstr != 0 && --available) {
		*pstrU++ = Mtoupper( *pstr++);
	}
	if ( available == 0) {
		RETURN_OVERFLOW_ERR( __FUNCTION__);
	}
	*pstrU = 0;

}

};

} // namespace MC

union PZBUF {
	long	lgDt[400];
	int		iDt[200];
	float	fDt[200];
	double	dbDt[200];
};
extern PZBUF* ____pzbuf;

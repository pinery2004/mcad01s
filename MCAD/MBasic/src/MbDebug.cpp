//==========================================================================================
//  Copyright ( C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MbDebug.cpp
//
//		
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#pragma warning( disable : 4996 )
#include "stdafx.h"
#include "MbBasic.h"

#define	DLL_EXPORT_MBLOG_DO
#define	DLL_EXPORT_TRACE_DO
#include "MbDebug.h"

//MCHAR	MbLog::m_Str[1000] = { MCHAR( "INIT")};
//FILE*	MbLog::m_File;
//MINT	MbLog::m_iCtl;

#define	PRINT_CNT_P_L		10					// １行あたりの印刷数

namespace MC
{

FILE*	Trace::ms_pfp = 0;						// トレースファイルポインタ
MCHAR	Trace::ms_cLogFilePath[MAX_PATH];		// トレースファイル名
MINT	Trace::ms_iDepth = 0;					// トレースインデント深さ
MCHAR	Trace::ms_cDepth[Msizeof( MBTRCINDENT) * ( MBTRCMAXDEPTH + 1)] = Mstr( "");
												// トレースインデント

//===========================================================================
//		デバッグ
MCHAR	MbLog::m_Str[1000];
FILE*	MbLog::m_File;
MINT	MbLog::m_iCtl;

//MbLog* MbDbg( )
//{
//	return &g_MbDbg;
//}

void MbLog::OpenTrace( MCHAR* i_pcFileName)
{
//#ifdef _DEBUG
	if ( Mstrcmp( i_pcFileName, Mstr( "stdout")) == 0) 
		m_File = stdout;
	else
		m_File = Mfopen( i_pcFileName, Mstr( "w"));
	m_iCtl = 1;
//#endif
}

void MbLog::CloseTrace( )
{
//#ifdef _DEBUG
	if ( m_File != stdout)
		fclose( m_File);
//#endif
}

void MbLog::Ctrl( int ictrl)
{
#ifdef _DEBUG
	m_iCtl = ictrl;
#endif
}

int MbLog::Wait( MCHAR* str)
{
#ifdef _DEBUG
	int ii;
	if( m_iCtl != 0) {
		Mprintf( Mstr( "%s --- Keyin dumy number >"), str);
		Mscanf( Mstr( "%d"), &ii);
	}
	else ii = 0;
	return ii;
#else
	return 0;
#endif
}
void MbLog::Flush( )
{
#ifdef _DEBUG
	if( m_iCtl != 0) {
		fflush( m_File);
	}
#endif
}
void MbLog::PrintCR( )
{
#ifdef _DEBUG
	if( m_iCtl != 0) {
		Mfprintf( m_File, Mstr( "\n"));
	}
#endif
}

void MbLog::Print( MCHAR* str)
{
#ifdef _DEBUG
	if( m_iCtl != 0) {
		Mfprintf( m_File, Mstr( "%s\n"), str);
	}
#endif
}

void MbLog::Print( MCHAR* str, const MCHAR* s1)
{
#ifdef _DEBUG
	if( m_iCtl != 0) {
		Mfprintf( m_File, Mstr( "%s	= %s\n"), str, s1);
	}
#endif
}

void MbLog::Print( MCHAR* str, MINT i1)
{
#ifdef _DEBUG
	if( m_iCtl != 0) {
		Mfprintf( m_File, Mstr( "%s	= %d\n"), str, i1);
	}
#endif
}

void MbLog::Print( MCHAR* str, MINT *i1, int ni1)
{
#ifdef _DEBUG
	if( m_iCtl != 0) {
		if ( ni1 != 0) {
			Mfprintf( m_File, Mstr( "%s	="), str);
			int	iCr = PRINT_CNT_P_L;
			for ( int i=0; i<ni1; i++) {
				Mfprintf( m_File, Mstr( " %d"), i1[i]);
				if ( --iCr == 0) {
					Mfprintf( m_File, Mstr( "\n\t\t\t"));
					iCr = PRINT_CNT_P_L;
				}
			}
			if ( iCr != 0)
				Mfprintf( m_File, Mstr( "\n"));
		}
	}
#endif
}

void MbLog::Print( MCHAR* str, MUBYTE i1)
{
#ifdef _DEBUG
	if( m_iCtl != 0) {
		Mfprintf( m_File, Mstr( "%s	= %d\n"), str, i1);
	}
#endif
}

void MbLog::Print( MCHAR* str, MUBYTE *i1, int ni1)
{
#ifdef _DEBUG
	if( m_iCtl != 0) {
		Mfprintf( m_File, Mstr( "%s	="), str);
		int	iCr = PRINT_CNT_P_L;
		for ( int i=0; i<ni1; i++) {
			if ( --iCr == 0) {
				Mfprintf( m_File, Mstr( "\n\t\t\t"));
				iCr = PRINT_CNT_P_L;
			}
		}
		if ( iCr != 0)
			Mfprintf( m_File, Mstr( "\n"));
	}
#endif
}

void MbLog::Print( MCHAR* str, MREAL f1)
{
#ifdef _DEBUG
	if( m_iCtl != 0) {
		Mfprintf( m_File, Mstr( "%s	= %f\n"), str, f1);
	}
#endif
}

void MbLog::Print( MCHAR* str, MREAL *f1, int nf1)
{
#ifdef _DEBUG
	if( m_iCtl != 0) {
		Mfprintf( m_File, Mstr( "%s	="), str);
		int	iCr = PRINT_CNT_P_L;
		for ( int i=0; i<nf1; i++) {
			Mfprintf( m_File, Mstr( " %f"), f1[i]);
			if ( --iCr == 0) {
				Mfprintf( m_File, Mstr( "\n\t\t\t"));
				iCr = PRINT_CNT_P_L;
			}
		}
		if ( iCr != 0)
			Mfprintf( m_File, Mstr( "\n"));
	}
#endif
}

void MbLog::Trace( MCHAR* str)
{
#ifdef _DEBUG
	if( m_iCtl != 0) {
		Mfprintf( m_File, Mstr( "%s"), str);
	}
#endif
}

void MbLog::Trace( )
{
#ifdef _DEBUG
	if( m_iCtl != 0) {
		Mfprintf( m_File, Mstr( "%s"), m_Str);
	}
#endif
}

//------------------------------------------------------------------------
//【機能】		ログファイルのオープン
//【返値】		なし
//------------------------------------------------------------------------
void Trace::OpenLogFile(
						MCHAR i_cLogFilePath[]	// ログファイルパス
						)
{
	if ( ms_pfp == NULL) {
		ms_pfp = Mfopen( i_cLogFilePath, Mstr( "at"));
	}
}

//------------------------------------------------------------------------
//【機能】		ログファイルのクローズ
//【返値】		なし
//------------------------------------------------------------------------
void Trace::CloseLogFile( void)
{
	if ( ms_pfp!=NULL) {
		Mfclose( ms_pfp);
		ms_pfp = NULL;
	}
}


//===========================================================================
//【機能】		ログファイル出力コンストラクタ
//【返値】		なし
//===========================================================================
Trace::Trace( void)
{
	ASSERT( ms_pfp);
}

Trace::Trace( MCHAR* i_cFuncName)
{
	ASSERT( ms_pfp >= 0);
	ms_iDepth++;
	if ( ms_iDepth < MBTRCMAXDEPTH) {
		Mstrcpy_s( &ms_cDepth[ms_iDepth * 2], MBTRCMAXDEPTH * 2 + 1, MBTRCINDENT);
	}
	Mstrcpy_s( m_cFuncName, Msizeof( m_cFuncName), i_cFuncName);
	Mfprintf( ms_pfp, Mstr("%s===> Start( %s)"), ms_cDepth, m_cFuncName);
}

//===========================================================================
//【機能】		ログファイル出力デストラクタ
//【返値】		なし
//===========================================================================
Trace::~Trace( void)
{
	ASSERT( ms_pfp > 0);
	Mfprintf( ms_pfp, Mstr("%s<=== End( %s)"), ms_cDepth, m_cFuncName);

	ms_iDepth--;
	if ( ms_iDepth < MBTRCMAXDEPTH) {
		ms_cDepth[ms_iDepth * 2] = 0;
	}
}

//------------------------------------------------------------------------
//【機能】		ログファイルへインデントされたフォーマット付きで出力する
//【引数】		なし
//【返値】		なし
//------------------------------------------------------------------------
void Trace::Write(
						MCHAR* i_cFormat, ...	// ログ書き込みデータ
						)
{
	MCHAR cOut[256];
	va_list	cList;
	va_start( cList, i_cFormat);
	vswprintf_s( cOut, 256, i_cFormat, cList);
	va_end( cList);

	Mfprintf( ms_pfp, Mstr("%s"), cOut);
}

//------------------------------------------------------------------------
//【機能】		ログファイルへ一行出力する
//【引数】		なし
//【返値】		なし
//------------------------------------------------------------------------
void Trace::WriteLine( 
						MCHAR i_cOut[]			// ログ書き込み行データ
						)
{
	Mfprintf( ms_pfp, Mstr("%s\n"), i_cOut);
}

} // namespace MC

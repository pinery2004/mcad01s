#pragma once
//==========================================================================================
//  Copyright ( C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MbDebug.h
//
//	[機能] トレース
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#define MBTRCMAXDEPTH			10							// トレースインデント最大深さ
#define MBTRCINDENT				Mstr( "  ")					// トレースインデント
//S #define	MBTRCLOGFILENAME		Mstr( "MCADLOG.txt")		// トレースファイル名
#define MBTRCFUNCNAME			40							// 関数名最大文字数

#define MBTRCOPEN( fn)			MbLog::OpenTrace( fn)		// トレース開始宣言　ファイル名
#define MBTRCCLOSE				MbLog::CloseTrace( )		// トレース終了宣言
#define MBTRCON					MbLog::Ctrl( 1)				// トレース　ＯＮ
#define MBTRCOFF				MbLog::Ctrl( 0)				// トレース　ＯＦＦ
#define MBTRCPRBF				MbLog::Trace( )				// トレースバッファ内の文字列
#define MBTRCPRS( S)			MbLog::Trace( S)			// 文字列
#define MBTRCFLUSH				MbLog::Flush( )				// フラッシュ（トレース中の作業エリア内文字列を掃きだす）

#define MBTRCPRINTCR			MbLog::PrintCR( )			// 改行
#define MBTRCPRINTS( S)			MbLog::Print( S)			// 文字列　改行
#define MBTRCPRINTSS( S, T)		MbLog::Print( S, T)			// 文字列 = 文字列　改行
#define MBTRCPRINTI( S, I)		MbLog::Print( S, I)			// 文字列 = 整数　改行
#define MBTRCPRINTIN( S, I, N)	MbLog::Print( S, I, N)		// 文字列 = N個の整数　改行
#define MBTRCPRINTB( S, B)		MbLog::Print( S, B)			// 文字列 = バイト( MUBYTE)整数　改行
#define MBTRCPRINTBN( S, B, N)	MbLog::Print( S, B, N)		// 文字列 = N個のバイト( MUBYTE)整数　改行
#define MBTRCPRINTF( S, F)		MbLog::Print( S, F)			// 文字列 = 実数　改行
#define MBTRCPRINTFN( S, F, N)	MbLog::Print( S, F, N)			// 文字列 = N個の実数　改行

#include "MbBasic.h"

#ifdef DLL_EXPORT
	#undef DLL_EXPORT
#endif

#ifdef DLL_EXPORT_MBLOG_DO
	#pragma message( "<<< MbLog・dll_EXport >>>")
	#define DLL_EXPORT							__declspec( dllexport)
#else
	#ifdef _DEBUG								// Release Mode で inline関数がundefになる不具合対応
		#ifdef DLL_NO_IMPORT_MBASIC_DO
			#define DLL_EXPORT
		#else
//			#pragma message( "=== MbLog・dll_IMport ===")
			#define DLL_EXPORT						__declspec( dllimport)
		#endif
	#else
		#define DLL_EXPORT
	#endif
#endif

namespace MC
{

//===========================================================================
//【機能】	トレース
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
	#pragma message( "<<< Trace・dll_EXport >>>")
	#define DLL_EXPORT							__declspec( dllexport)
#else
	#ifdef _DEBUG								// Release Mode で inline関数がundefになる不具合対応
		#ifdef DLL_NO_IMPORT_MBASIC_DO
			#define DLL_EXPORT
		#else
//			#pragma message( "=== Trace・dll_IMport ===")
			#define DLL_EXPORT						__declspec( dllimport)
		#endif
	#else
		#define DLL_EXPORT
	#endif
#endif

//===========================================================================
//【機能】	トレース
//
class  DLL_EXPORT Trace
{
private:
	static	MCHAR	ms_cLogFilePath[MAX_PATH];					// トレースファイル名
	static	FILE*	ms_pfp;										// トレースファイルポインタ
	static	MINT	ms_iDepth;									// トレースインデント深さ
	static	MCHAR	ms_cDepth[Msizeof( MBTRCINDENT) * (MBTRCMAXDEPTH + 1)];
																// トレースインデント
			MCHAR	m_cFuncName[MBTRCFUNCNAME];					// 関数名
	
public:
	Trace( void);
	Trace( MCHAR* i_cFuncName);
	~Trace( void);
	// ログファイルの削除とオープン
	static void OpenLogFile(
						MCHAR i_cLogFilePath[]	// ログファイルパス
						);
	// ログファイルのクローズ
	static void CloseLogFile( void);
	// ログデータ書き込み
	void Write( MCHAR i_cFormat[], ...);
	// ログデータ行書き込み
	void WriteLine( MCHAR i_cOut[]);

};
//extern	CMbDebug g_MbDbg;

} // namespace MC
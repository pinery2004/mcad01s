//==========================================================================================
//  K.Matsu. Copyright ( C) 2004-2005.
//
//  MODULE: MbTrace.cpp
//
//		
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"
#include "MgDefine.h"

/////////////////////////////////////////////////////////////////////////////
//	トレース
#include	"MbTrace.h"
#include	<stdarg.h>
#if	defined(_DEBUG)
#include	<stdio.h>
#if	! defined(BUFSIZ)
#define	BUFSIZ	1024
#endif	/* ! defined(BUFSIZ) */
#endif	/* defined(_DEBUG) */

/*.....................................................................
*/
#if     defined(_DEBUG)
CStdioFile *    DebugFp = NULL;
static  void    CloseDebugLog(void)
{
        delete DebugFp;
        DebugFp = NULL;
}
static  void    OpenDebugLog(void)
{
        UINT    mode = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
        DebugFp = new CStdioFile( Mstr( "Trace.txt"), mode);
}
int     WriteDebugLog( MCHAR* text)
{
        if (DebugFp == NULL) {
                OpenDebugLog();
        }
        DebugFp->WriteString( text);
        DebugFp->WriteString( Mstr( "\n"));
        return 0;
}
#endif  /* defined(_DEBUG) */




// 次の場所に設定する	http://www.amris.co.jp/cpp/c12.html
///* InitInstance() */
//#if     defined(_DEBUG)
//        DebugSetLogFunc(WriteDebugLog);
//#endif  /* defined(_DEBUG) */
///* ExitInstance() */
//#if     defined(_DEBUG)
//        CloseDebugLog();
//#endif  /* defined(_DEBUG) */



/*.....................................................................
*/
//U static UDF_TRACELOG *	UdfLogOutFunc = NULL;
static long				TraceLevel = 0;
/*=====================================================================
*/
//U void DebugSetLogFunc(
//	UDF_TRACELOG *  udfLogOutFunc)
//{
//	UdfLogOutFunc = udfLogOutFunc;
//}
/*---------------------------------------------------------------------
*/
//void
//DebugTraceIn(char * name)
//{
//	DebugTraceLog(">>> %s", name);
//	TraceLevel ++;
//}
/*---------------------------------------------------------------------
*/
//void
//DebugTraceOut(char * name)
//{
//	TraceLevel --;
//	DebugTraceLog("<<< %s", name);
//}
/*---------------------------------------------------------------------
*/












#define _TRACE_MALLOC 1

#ifdef	TRACE
	#undef	TRACE
#endif
#define TRACE( X)	{ if ( stream == NULL) MbTraceOpen( NULL); Mfprintf( stream, Mstr( "%s\n"), X);}
#define TRACEC( X)	{ if ( stream == NULL) MbTraceOpen( NULL); fprintf( stream, "%s\n", X);}

/////////////////////////////////////////////////////////////////////////////
//	汎用関数
//
/////////////////////////////////////////////////////////////////////////////
//	トレース

//MINT g_DbgTrace;
static	FILE *stream = NULL;

void MbTraceOpen( MCHAR* cFileName) { 
	if ( cFileName == NULL)
		stream = Mfopen( Mstr( "c:/temp/MCadTrace.txt"), Mstr( "w") );
	else 
		stream = Mfopen( cFileName, Mstr( "w") );
	TRACE( Mstr( "START TRACE\n"));
	fflush( stream);
}

void MbTraceClose( ) { 
   fclose( stream );
}

void MbTraceCVd( MCHAR* cHeader, void *pi1)
{
#pragma warning ( disable: 4311) 
	MINT i1 = MINT( pi1);
	MCHAR	cTraceLine[256];
	Msprintf( cTraceLine, Mstr( " %s %x\n"), cHeader, i1);
	TRACE( cTraceLine);
}

void MbTraceCNII( MCHAR* cHeader, MINT* ib, MINT nib)
{
	MINT	ic;
	MCHAR	cTraceLine[256], cTraceItem[32];

	Msprintf( cTraceLine, Mstr( " %s"), cHeader);
	for ( ic=0; ic<nib; ic++) {
		Msprintf( cTraceItem, Mstr( " %d"), ib[ic]);
		Mstrcat( cTraceLine, cTraceItem);
	}
	Msprintf( cTraceItem, Mstr( "\n"));
	Mstrcat( cTraceLine, cTraceItem);
	TRACE( cTraceLine);
}

void MbTraceCI( MCHAR* cHeader, MINT i1)
{
	MCHAR	cTraceLine[256];
	Msprintf( cTraceLine, Mstr( " %s %d\n"), cHeader, i1);
	TRACE( cTraceLine);
}

void MbTraceC2I( MCHAR* cHeader, MINT i1, MINT i2)
{
	MCHAR	cTraceLine[256];
	Msprintf( cTraceLine, Mstr( " %s %d %d\n"), cHeader, i1, i2);
	TRACE( cTraceLine);
}

void MbTraceC3I( MCHAR* cHeader, MINT i1, MINT i2, MINT i3)
{
	MCHAR	cTraceLine[256];
	Msprintf( cTraceLine, Mstr( " %s %d %d %d\n"), cHeader, i1, i2, i3);
	TRACE( cTraceLine);
}

void MbTraceC4I( MCHAR* cHeader, MINT i1, MINT i2, MINT i3, MINT i4)
{
	MCHAR	cTraceLine[256];
	Msprintf( cTraceLine, Mstr( " %s %d %d %d %d\n"), cHeader, i1, i2, i3, i4);
	TRACE( cTraceLine);
}

void MbTraceCNFI( MCHAR* cHeader, MREAL *fb, MINT nfb)
{
	MINT	ic;
	MCHAR	cTraceLine[256], cTraceItem[32];

	Msprintf( cTraceLine, Mstr( " %s"), cHeader);
	for ( ic=0; ic<nfb; ic++) {
		Msprintf( cTraceItem, Mstr( " %f"), fb[ic]);
		Mstrcat( cTraceLine, cTraceItem);
	}
	Msprintf( cTraceItem, Mstr( "\n"));
	Mstrcat( cTraceLine, cTraceItem);
	TRACE( cTraceLine);
}

void MbTraceCF( MCHAR* cHeader, MREAL f1)
{
	MCHAR	cTraceLine[256];
	Msprintf( cTraceLine, Mstr( " %s %f\n"), cHeader, f1);
	TRACE( cTraceLine);
}

void MbTraceC2F( MCHAR* cHeader, MREAL f1, MREAL f2)
{
	MCHAR	cTraceLine[256];
	Msprintf( cTraceLine, Mstr( " %s %f %f\n"), cHeader, f1, f2);
	TRACE( cTraceLine);
}

void MbTraceC3F( MCHAR* cHeader, MREAL f1, MREAL f2, MREAL f3)
{
	MCHAR	cTraceLine[256];
	Msprintf( cTraceLine, Mstr( " %s %f %f %f\n"), cHeader, f1, f2, f3);
	TRACE( cTraceLine);
}

void MbTraceC4F( MCHAR* cHeader, MREAL f1, MREAL f2, MREAL f3, MREAL f4)
{
	MCHAR	cTraceLine[256];
	Msprintf( cTraceLine, Mstr( " %s %f %f %f %f\n"), cHeader, f1, f2, f3, f4);
	TRACE( cTraceLine);
}

void MbTraceLog(char *fmt, ...)
{
	int	n = 0;

//#if	defined( _DEBUG)

    va_list	argptr;

	char	cTraceLine[BUFSIZ];
	int		i;

	int	indent = 2;
	if ( 0 < TraceLevel) {
		indent = TraceLevel * 4 + 2;
	}
	for ( i = 0; i < indent; i++) {
		cTraceLine[i] = ' ';
	}               /* 念のため memset() は避ける */
	cTraceLine[0] = ':';

	va_start( argptr, fmt);
	n = _vsnprintf( &cTraceLine[indent], BUFSIZ - indent, fmt, argptr);
	va_end( argptr);

	//U if ( UdfLogOutFunc != NULL) {
	//	if ( n < 0) {
	//		( *UdfLogOutFunc)( "[FATAL] (DebugTraceLog) Too long message.");
	//		cTraceLine[BUFSIZ - 1] = '\0';
	//	}
	//	( *UdfLogOutFunc)( cTraceLine);
	//}

	TRACE( cTraceLine);

//#endif  /* defined(_DEBUG) */

//	return n;
}

////////////////////////////////////////////////////////////////////////////////
//	トレース用ストリームバッファの出力
void MbFlush( )
{
	fflush( stream);
}

//==========================================================================================
//  Copyright ( C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MbFile.cpp
//
//		
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#pragma warning( disable : 4996 )
#include "stdafx.h"
#define	DLL_EXPORT_CODE_DO
#include "MbBasic.h"

#define	DLL_EXPORT_FILE_DO
#include "MbFile.h"

namespace MC
{

// *******************************************************************************
//	CSVファイルの読み込み 使用方法
//
//	MBFILE	csv
//	ist = csv.Open( )
//	ist = csv.NextLine( )
//  ist = csv.Head( Str, 10);
//  ist = csv.GetStr( Str, 10);
//  ist = csv.GetReal( Flt, 3);
//  ist = csv.GetInt( Int, 10);
//	ist = csv.Close( )
//
// *******************************************************************************

// *******************************************************************************
//【機能】ファイル読み書き処理開始宣言
//【返値】==  0 : 正常終了
//	　　　== -2 : 既にオープンされている
//		　≦ -3 : その他エラー
//【注意】なし
// ********************************************************************************

int		MBFILE::Open( 
				MCHAR*	i_pcFileName,			// ファイル名
				DWORD	i_Mode					// 処理モード
												//		== MBREAD:		読み込み専用
												//		== MBWRITE:		書き込み専用
												//		== MBREADWRITE: 読み書き可能
				)
{
	int		ist0;								// ステイタス
	ist0 = 0;
		if ( m_stat != 0) {						// 既にオープンされている
			ist0 = STS_OPENED;	goto END;
		};
	if ( i_Mode == MBREAD) {
		m_fileHandle = CreateFileW( i_pcFileName, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
		m_stat = MBREAD;
	} else if ( i_Mode == MBWRITE) {
		m_fileHandle = CreateFileW( i_pcFileName, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
		m_stat = MBWRITE;
	} else {
		ist0 = STS_ACSMDERR;	goto END;
	}
	if ( m_fileHandle == INVALID_HANDLE_VALUE) {
		ist0 = STS_FLOPNERR;	goto END;
	}
END:
	return	ist0;
}

// *******************************************************************************
//										Created on 10/01/00 by K.Matsubayashi
//										Updated on xx/xx/xx/ by x.xxxxxx
//										Copyright
//【機能】ファイル読み書き処理終了宣言
//【返値】==  0 : 正常終了
//		  ≠  0 : その他エラー
//【注意】なし
// ********************************************************************************

int	MBFILE::Close( void)
{
	CloseHandle( m_fileHandle);
	m_stat = 0;
	return 0;									//	要修正
}

//===========================================================================
//【機能】		:ファイルの更新日を取得する
//【返値】==  0 : 正常終了
//		  ≠  0 : その他エラー
//===========================================================================
MINT MBFILE::GetFileDate(
						MCHAR*	i_cFilePath,	// ファイル名
						MCHAR	o_cDate[9]		// ファイルの作成日
						)
{
	MINT		ist;
	HANDLE		hFile;
	FILETIME	FileTime;
	FILETIME	LocalFileTime;
	SYSTEMTIME	SystemTime;

	hFile = CreateFile( i_cFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if ( hFile == INVALID_HANDLE_VALUE) {
		o_cDate[0] = 0;
		ist = -1;

	} else {
		GetFileTime( hFile, NULL, NULL, &FileTime);
		FileTimeToLocalFileTime( &FileTime, &LocalFileTime);
		FileTimeToSystemTime( &LocalFileTime, &SystemTime);
		Msprintf_s( o_cDate, sizeof( o_cDate), Mstr( "%04d%02d%02d"), SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay);
		ist = 0;
	}
	CloseHandle(hFile);

	return ist;
}

// *******************************************************************************
//										Created on 10/01/00 by K.Matsubayashi
//										Updated on xx/xx/xx/ by x.xxxxxx
//										Copyright
//【機能】ファイル項目読み込み処理開始宣言
//【返値】==  0 : 正常終了
//		　== -2 : 既にオープンされている
//		　≦ -3 : その他エラー
//【注意】なし
// ********************************************************************************

int		MBHZDT::Open( 
				MCHAR*	i_pcFileName			// ( I ) ファイル名
				)
{
	m_icsLine = 0;
	m_icsDbuf = 0;
	m_fhead = 1;
	return 	MBFILE::Open( i_pcFileName, MBREAD);
}

// *******************************************************************************
//										Created on 10/01/00 by K.Matsubayashi
//										Updated on xx/xx/xx/ by x.xxxxxx
//										Copyright
//【機能】ファイル項目読み込み処理終了宣言
//【返値】==  0 : 正常終了
//		　≠  0 : その他エラー
//【注意】なし
// ********************************************************************************

int	MBHZDT::Close( void)
{
	m_fopen=0;
	m_fback=0;
	m_icsDbuf = -1;								// 文字読み込み処理バッファが空( -1)
	m_icsLine = 0;								// 文字読み込み処理バッファの先頭

	return	MBFILE::Close( );
}

// *******************************************************************************
//										Created on 10/01/00 by K.Matsubayashi
//										Updated on xx/xx/xx/ by x.xxxxxx
//										Copyright
//【機能】次行読み込み
//【返値】 == 0 : 正常終了
//		== -1 : ファイルの終わり	( MBEOF)
//		== -2 : 行の終わり			( MBEOL)
//		≦ -3 : その他エラー
//【注意】なし
// ********************************************************************************

int		MBHZDT::NextLine( void)
{
	if ( m_fopen == MBEOL) {
		m_fopen = 0;
	}
	if ( m_fopen != MBEOF) {
		Skip( 0 );
	}
	m_fhead = 1;
	return m_fopen;
}

// *******************************************************************************
//										Created on 10/01/00 by K.Matsubayashi
//										Updated on xx/xx/xx/ by x.xxxxxx
//										Copyright
//【機能】次行の先頭文字列項目を読み込む
//【返値】 >  0 : 読み込み文字数
//		== -1 : ファイルの終わり	( MBEOF)
//		== -2 : 行の終わり			( MBEOL)
//		≦ -3 : その他エラー
//【注意】なし
// ********************************************************************************

int		MBHZDT::Head( 
				MCHAR*	str,					// ( O) 読み込み文字列
				int		cbstr					// ( I ) 読み込み最大文字数
			)

{
	if ( m_fhead == 0) 
		if ( NextLine( ) == MBEOF)
			return MBEOF;
	m_fhead = 0;
	return GetStr( str, cbstr );
}

// *******************************************************************************
//										Created on 10/01/00 by K.Matsubayashi
//										Updated on xx/xx/xx/ by x.xxxxxx
//										Copyright
//【機能】次の項目群を飛び越す
//【返値】 >  0 : 正常
//		== -1 : ファイルの終わり	( MBEOF)
//		== -2 : 行の終わり			( MBEOL)
//		≦ -3 : その他エラー
//【注意】なし
// ********************************************************************************

int 	MBHZDT::Skip( 
				int		ns						// ( I ) ns ＞ 0	スキップする項目数
												//         ＝ 0 １行スキップ
				)
{
	char	Str[1];
	int		isf;
	int		ik;
//
	if ( m_fopen < 0) {
		isf = m_fopen;							// MBEOL または MBEOF
		goto END;
	}
	if ( ns == 0) {
		do {
			isf = GetCh( );
		} while ( ( isf != MBEOL) && ( isf != MBEOF));
		if ( isf == MBEOL) isf = 0;
		m_fhead = 1;
	} else {
		for ( ik=0; ik<ns; ik++) {
			isf = GetStrC( Str, 1 );
			if( ( isf == MBEOL) || ( isf == MBEOF)) break;
		}
		m_fhead = 0;
	}
	m_fopen = isf;
END:
	return isf;
}

// *******************************************************************************
//										Created on 10/01/00 by K.Matsubayashi
//										Updated on xx/xx/xx/ by x.xxxxxx
//										Copyright
//【機能】ファイルより指定数の実数項目を読み込む
//【返値】 >  0 : 読み込み項目数
//		== -1 : ファイルの終わり	( MBEOF)
//		== -2 : 行の終わり			( MBEOL)
//		≦ -3 : その他エラー
//【注意】なし
// ********************************************************************************

int		MBHZDT::GetReal( 
				MREAL	*rb,					// ( O) 読み込み先実数バッファ
				int		icrb					// ( I ) 読み込み項目数
			)
{
	char Str[MBCBSLINE];						//	項目データ
	int ik;										//	読み込み項目数
	int	ic;

	if ( ( m_fopen == MBEOL) || ( m_fopen == MBEOF)) {
		ik = m_fopen;
		goto END;
	}

	for( ik=0; ik<icrb; ) {
		UINT ichs = GetStrC( Str, MBCBSLINE );
		if( ( ichs == 0) || ( ichs == MBEOL)) {
			rb[ik] = 0;
		} else {
			#ifdef MG_DOUBLE_PRECISION_REAL
				sscanf( Str, "%lf", &rb[ik]);
			#else
				sscanf( Str, "%f", &rb[ik]);
			#endif
		}
		ik++;
	}
	for( ic=ik; ic<icrb; ic++) rb[ic] = 0.;
END:
	m_fhead = 0;
	return ik;
}

// *******************************************************************************
//										Created on 10/01/00 by K.Matsubayashi
//										Updated on xx/xx/xx/ by x.xxxxxx
//										Copyright
//【機能】ファイルより指定数の整数項目を読み込む
//【返値】 >  0 : 読み込み項目数
//		== -1 : ファイルの終わり	( MBEOF)
//		== -2 : 行の終わり			( MBEOL)
//		≦ -3 : その他エラー
//【注意】なし
// ********************************************************************************

int		MBHZDT::GetInt( 
				int		*ib,					//	読み込み先整数バッファ
				int		icib					//	読み込み項目数
			)
{
	char	Str[MBCBSLINE];						//	項目文字列バッファ
	int		ik;									//	項目カウンタ
	int		ic;									//	残項目カウンタ

	if( ( m_fopen == MBEOL) || ( m_fopen == MBEOF)) {
		ik = m_fopen;
		goto END;
	}

	for( ik=0; ik<icib; ) {
		UINT ichs = GetStrC( Str, MBCBSLINE );
		if( ( ichs == 0) || ( ichs == MBEOL)) ib[ik] = 0;
		else sscanf( Str, "%d", &ib[ik] );
		ik++;
	}
	for( ic=ik; ic<icib; ic++) ib[ic] = 0;
END:
	m_fhead = 0;
	return ik;
}

// *******************************************************************************
//										Created on 10/01/00 by K.Matsubayashi
//										Updated on xx/xx/xx/ by x.xxxxxx
//										Copyright
//【機能】ファイルより文字列項目を１項目読み込む
//【返値】 >  0 : 読み込み文字数
//		== -1 : ファイルの終わり	( MBEOF)
//		== -2 : 行の終わり			( MBEOL)
//		≦ -3 : その他エラー
//【注意】なし
// ********************************************************************************

int		MBHZDT::GetStr( 
				MCHAR*	Str,					// ( O) 読み込み文字列
				int		cbStr					// ( I ) 読み込み最大文字数( ASCII文字相当エリアで換算し最終NULLを含まない文字数)
												//		EOL または EOF　以後は 0
				)
{
	MINT	ist;
	char	StrC[1000];
	ist = GetStrC( StrC, cbStr * 2);
	Code::Mmbstowcs( StrC, Str, cbStr);   
	return ist;
}

// *******************************************************************************
//										Created on 10/01/00 by K.Matsubayashi
//										Updated on xx/xx/xx/ by x.xxxxxx
//										Copyright
//【機能】ファイルより文字列項目を１項目読み込む
//【返値】 >  0 : 読み込み文字数
//		== -1 : ファイルの終わり	( MBEOF)
//		== -2 : 行の終わり			( MBEOL)
//		≦ -3 : その他エラー
//【注意】なし
// ********************************************************************************

int		MBHZDT::GetStrC( 
				char	*Str,					// ( O) 読み込み文字列
				int		cbStr					// ( I ) 読み込み最大文字数( NULLを含まず)
												//		EOL または EOF　以後は 0
				)
{
	int		ist0;								//	ステイタス
	int		iclp;								//	読み込み文字数
	int		fdbq;								//  '"'で挟まれた文字列
												//	0:先頭文字!='"'、1,2:先頭文字=='"'の場合の'"'の数
	char	chs;

	ist0 = 0;

	Str[0] = 0;
	if( ( m_fopen == MBEOL) || ( m_fopen == MBEOF)) {
		iclp = m_fopen;
		goto END;
	}
//
//	区切り文字が現れるまで行バッファより読み込む
	fdbq = 0;
	iclp = 0;
	for( ;;) {
		chs = GetCh( );
		if( ( chs == MBEOL) || ( chs == MBEOF)) break;		// MBEOL または MBEOF
		if( ( chs == ';')) {
			Skip( 0 );									// next line
			chs = GetCh( );
			if( ( chs == MBEOL) || ( chs == MBEOF)) break;	// MBEOL または MBEOF
		}
//		if( ( chs == ' ') || ( chs == '\x09')) break;
		if( ( chs == ',') && ( fdbq != 1)) break;
		if ( ( chs != 0x0A) && ( chs != 0x0D) && ( iclp < cbStr)) {
//
			if ( iclp == 0 && chs == '"') {				// 0:先頭文字 == '"'
				fdbq = 1;
			} else if ( fdbq == 1 && chs == '"') {		// 先頭文字 == '"'の場合の'"'のカウント( MAX 2)
				fdbq++;
			} else {
				Str[iclp++] = chs;
			}
		}
	}
//	if( iclp < cbStr) Str[iclp] = 0;
	if( chs == ';') {
		Skip( 0 );										// next line
	}
//
//	スペースおよびタブ文字以外の文字が現れるまで行バッファの文字位置を進める
	while( ( chs != MBEOL) && ( chs != MBEOF)) {
		chs = GetCh( );
		if( chs == ';') {
			Skip( 0 );									// next line
		} else if ( chs == ' ') {
		} else if ( chs == '\x09') {
		} else {
			break;
		}
	}
	BackCh( );
//
//	バッファの後ろのスペースを削る
	iclp--;
	for ( ; iclp>=0; iclp--) {
		if ( ( Str[iclp] != ' ') && ( Str[iclp] != '\x09'))  break;
	}
	iclp++;
	Str[iclp] = 0;
END:
	m_fhead = 0;
	return iclp;
}

// *******************************************************************************
//										Created on 10/01/00 by K.Matsubayashi
//										Updated on xx/xx/xx/ by x.xxxxxx
//										Copyright
//【機能】ファイルより１文字読み込む
//【返値】>=  0 : 文字コード
//		== -1 : ファイルの終わり	( MBEOF)
//		== -2 : 行の終わり			( MBEOL)
// 【注意】なし
// ********************************************************************************

int		MBHZDT::GetCh( 
				void
				)
{
	static int	chs;							//	読み込み文字
	BOOL		successR;						//	ファイル読み込みステイタス
//
	if( m_fopen < 0) {
		//	テキストファイルの読み込み処理状況フラグ( m_fopen )を調べ、
		//	行の終わりまたはファイルの終わりなら MBEOL または MBEOF を返す。
		chs = m_fopen;							// MBEOL または MBEOF
		goto END;
	}
	if( m_fback == 1) {
		//	一文字戻しフラグ( m_fback )が立っている( 1)ならば最後に読み込んだ文字を返し
		//  同フラグを消す
		m_fback = 0;
	}
	else {
		//	一文字戻しフラグ( m_fback )が立っていない( 0)なら
		if ( m_icsLine >= m_icsDbuf) {
			//	( 文字読み込み)処理バッファ内の文字列がすべて処理済みである場合は、
			//	ファイルから同処理バッファに、続く文字列を読み込む。
			successR = ReadFile( m_fileHandle, m_sDbuf, MBCBSDBUF, ( DWORD*)&m_icsDbuf, 0);
			m_icsLine = 0;						// 処理済み文字数 = 0
			if( ( !successR) || ( m_icsDbuf <= 0)) {
				// ファイル内に続く文字が無い場合は、ファイルの終わり( MBEOF )を返して終了
				m_fopen = MBEOF;
				chs = MBEOF;
				goto END;
			}
		}
		//	( 文字読み込み)処理バッファ内より一文字取り出す。
		chs = m_sDbuf[m_icsLine++];
		if( chs == 0x0A) {
		    //	取り出した文字が改行文字ならば、行の終わり( MBEOL )を返して終了
			m_fopen = MBEOL;
			chs = MBEOL;
		}
	}
END:
	return chs;
}

// *******************************************************************************
//										Created on 10/01/00 by K.Matsubayashi
//										Updated on xx/xx/xx/ by x.xxxxxx
//										Copyright
//【機能】文字処理ポインタを一文字戻す
//        ( 一文字戻すことを表わすフラグを立てる)
//【返値】なし
//【注意】なし
// ********************************************************************************

void	MBHZDT::BackCh( 
		void
		)
{
	m_fback = 1;
}

// *******************************************************************************
//										Created on 10/01/00 by K.Matsubayashi
//										Updated on xx/xx/xx/ by x.xxxxxx
//										Copyright
//【機能】MBCS文字列をUNICODE文字列に変換する
//【返値】 0 : 正常     -1 : 異常
//【注意】なし
// ********************************************************************************

MINT Code::Mmbstowcs( 
					const char*		i_StrC,				// MBCS文字列
					MCHAR*			o_StrU,				// UNICODE文字列
					MINT			i_iSize				// 最大文字数
				)
{

#ifdef _UNICODE
	MINT iLen = ( MINT)strlen( i_StrC);

	if ( iLen == 0) {
		o_StrU[0] = NULL;
		return( 0);
	} else {
		MINT iLen1 = MultiByteToWideChar( CP_ACP, 0, ( LPCSTR)i_StrC,  iLen,  ( LPWSTR)o_StrU, i_iSize);
		if ( iLen1 == 0) {
			o_StrU[0] = NULL;
			return( -1);
		} else {
			o_StrU[min( iLen1, i_iSize)] = NULL;
			return( 0);
		}
	}
#else
	Mstrncpy( StrU, StrC, iSize+1);
	return( 0);
#endif
}

// *******************************************************************************
//										Created on 10/01/00 by K.Matsubayashi
//										Updated on xx/xx/xx/ by x.xxxxxx
//										Copyright
//【機能】UNICODE文字列をMBCS文字列に変換する
//【返値】 0 : 正常     -1 : 異常
//【注意】なし
// ********************************************************************************

MINT Code::Mwcstombs( 
				const MCHAR*	i_StrU,			// UNICODE文字列
				char*			o_StrC,			// MBCS文字列
				const MINT		i_iSize			// 最大文字数
				)
{
#ifdef _UNICODE
	MINT iLen = ( MINT)Mstrlen( i_StrU);
	if ( iLen == 0) {
		o_StrC[0] = NULL;
		return( 0);
	} else {
		MINT iLen1 = WideCharToMultiByte( CP_ACP, 0, ( LPCWSTR)i_StrU, iLen, ( LPSTR)o_StrC, i_iSize, NULL, NULL);
		if ( iLen1 == 0) {
			o_StrC[0] = NULL;
			return( -1);
		} else {
			o_StrC[min( iLen1, i_iSize)] = NULL;
			return( 0);
		}
	}

#else
	strcpy( StrC, StrU);
	return( 0);
#endif
}

} // namespace MC

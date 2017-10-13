#pragma once
//==========================================================================================
//  Copyright ( C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MbBasic.h
//
//		基本　定数・関数
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#ifndef WINVER				// Windows XP 以降のバージョンに固有の機能の使用を許可します。
#define WINVER 0x0501		// これを Windows の他のバージョン向けに適切な値に変更してください。
#endif

#pragma warning( disable : 4996 )

#include <afxwin.h> 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//===========================================================================
//	システム設定

//===========================================================================
// 実数の精度	倍精度実数で取り扱う場合は、MG_DOUBLE_PRECISION_REALを定義
//
// #define		MG_DOUBLE_PRECISION_REAL

//===========================================================================
//		マップ モード
//
//#define		MMAPMODE			MM_HIMETRIC	// 0.01mm
#define		MMAPMODE			MM_TWIPS		// 1/20ポイント
//#define		MMAPMODE			MM_TEXT

#if( MMAPMODE == MM_TEXT)
	#define		MMDIRY			(-1)
#else
	#define		MMDIRY			1
#endif

//===========================================================================
//		永久ループ
//
#define		MFOREVER			for ( ;;)

//===========================================================================
//		型の定義
//
typedef		_TCHAR				MCHAR;			// 文字型
typedef		char				MBYTE;			// 1/4語整数型
typedef		short				MSHORT;			// 1/2語整数型
typedef		int					MINT;			// 32ビット整数型
typedef		long				MLONG;			// 32ビット整数型
typedef		__int64				MLONGLONG;		// 64ビット整数型
typedef		float				MFLOAT;			// 単精度実数型
typedef		double				MDOUBLE;		// 倍精度実数型

#ifdef MG_DOUBLE_PRECISION_REAL
	typedef		double				MREAL;			// 単精度実数型
#else
	typedef		float				MREAL;			// 倍精度実数型
#endif

typedef		_TUCHAR				MUCHAR;			// 符号なし文字型
typedef		unsigned char		MUBYTE;			// 符号なし1/4語整数型
typedef		unsigned short		MUSHORT;		// 符号なし1/2語整数型
typedef		unsigned int		MUINT;			// 符号なし32ビット整数型
typedef		unsigned long		MULONG;			// 符号なし32ビット整数型
typedef		unsigned __int64	MULONGLONG;		// 符号なし64ビット整数型

typedef		void				MVOID;			// VOID型
typedef		void*				MHANDLE;		// HANDLE型

typedef	 enum _mbool {							// 論理型
			MFALSE = 0,							// 偽
			MTRUE  = 1							// 真
		} 						MBOOL;

typedef		MINT				MDID;			// ID	

typedef 	long 				MRC;			// リターンコード

//typedef	time_t				MTIME;			// 時間獲得用変数型
#define		MTIME				CTime			// 時間獲得用変数型

#define		MCONST				const			// 定数型

//===========================================================================
//		テキスト 作業領域サイズ
//
#define		MX_PATH				256				// パス文字列　最大文字数

//===========================================================================
//		データ型定数は、整数データ型に使用できる値の範囲
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

// サイズ
inline MINT SZMINT( MINT i_sz = 1)		{ return  ( i_sz * (MINT)sizeof( MINT));}
inline MINT SZMREAL( MINT i_sz = 1)	{ return  ( i_sz * (MINT)sizeof( MREAL));}

//===========================================================================
//		全オブジェクトのベース
//
class 		MOBJECT				{};				// 全オブジェクトのベース

//===========================================================================
//		abstract iteration position
//
class 		__MPOSITION 		{};
//typedef 	__MPOSITION* 		MPOSITION;
#define		MPOSITION		 	__MPOSITION*

//===========================================================================
//		数値演算定数
//
#define		MC_E				2.71828182845904523536		// e
#define		MC_LOG2E			1.44269504088896340736		// log2( e)
#define		MC_LOG10E			0.434294481903251827651		// log10( e)
#define		MC_LN2				0.693147180559945309417		// ln( 2)
#define		MC_LN10				2.30258509299404568402		// ln( 10)

#define		MC_2PIE				6.28318530717958647692		// 2*円周率
#define		MC_PIE				3.14159265358979323846		// 円周率
#define		MC_PIE_2			1.57079632679489661923		// 円周率/2
#define		MC_PIE_4			0.785398163397448309616		// 円周率/4
#define		MC_1_PIE			0.318309886183790671538		// 1/円周率
#define		MC_2_PIE			0.636619772367581343076		// 2/円周率

#define		MC_DTR				0.0174532925199433f			// 円周率/180
#define		MC_RTD				57.2957795130824f			// 180/円周率
#define		MC_INCHES_TO_MM		25.4f;

#define		MC_SQRTPIE			1.77245385090551602729f		// (MREAL)sqrt( 円周率)
#define		MC_2_SQRTPIE		1.12837916709551257390f		// 2/sqrt( 円周率)
#define		MC_SQRT2			1.41421356237309504880f		// (MREAL)sqrt( 2)
#define		MC_SQRT1_2			0.707106781186547524401f	// 1/sqrt( 2)

//===========================================================================
//		文字列操作
//
#define		Mchar( cha)			_TEXT( cha)		// キャラクタUNICODE変換
#define		Mstring( str)		_TEXT( str)		// 文字列UNICODE変換
#define		Mstr( str)			_TEXT( str)		// 文字列UNICODE変換
#define		MBCstr( str)		( str)			// 文字列MBC	無変換
#define		Mgetchar( str, n)	string[number]	// 文字列中のキャラクタの取得
#define		Msizeof( str)		( sizeof( str)/sizeof( MCHAR)-1)
												// 文字列サイズ
#define		Mstrcpy				_tcscpy			// 文字列の複写
#define		Mstrcpy_s			_tcscpy_s		// 文字列の複写
#define		Mstrncpy			_tcsncpy		// 文字列の複写 (文字数はNULLを含まない)
#define		Mstrncpy_s			_tcsncpy_s		// 文字列の複写 (文字数はNULLを含まない)　( 後ろにNULLを付けてコピー)
#define		Mstrcat				_tcscat			// 文字列の連結
#define		Mstrcat_s			_tcscat_s		// 文字列の連結
#define		Mstrncat			_tcsncat		// 文字列の連結 (文字数はNULLを含まない)
#define		Mstrncat_s			_tcsncat_s		// 文字列の連結 (文字数はNULLを含まない)
#define		Mstrcmp				_tcscmp			// 文字列の比較
#define		Mstrcmp_s			_tcscmp_s		// 文字列の比較 (文字数はNULLを含まない)
#define		Mstricmp			_tcsicmp		// 文字列を小文字化して比較
#define		Mstricmp_s			_tcsicmp_s		// 文字列を小文字化して比較 (文字数はNULLを含まない)
#define		Mstrncmp			_tcsncmp		// 文字列の比較 (文字数はNULLを含まない)
#define		Mstrncmp_s			_tcsncmp_s		// 文字列の比較 (文字数はNULLを含まない)
#define		Mstrnicmp			_tcsnicmp		// 文字列を小文字化して比較 (文字数はNULLを含まない)
#define		Mstrnicmp_s			_tcsnicmp_s		// 文字列を小文字化して比較 (文字数はNULLを含まない)
#define		Mstrchr				_tcschr			// 文字列中の文字の検索 (str 内で c が最初に出現する位置を指すポインタを返す。c が見つからない場合は、NULL を返す。)
#define		Mstrchr_s			_tcschr_s		// 文字列中の文字の検索 (str 内で c が最初に出現する位置を指すポインタを返す。c が見つからない場合は、NULL を返す。)
#define		Mstrrchr			_tcsrchr		// 文字列中の文字の検索 (str 内で c が最後に出現する位置を指すポインタを返す。c が見つからない場合は、NULL を返す。)
#define		Mstrrchr_s			_tcsrchr_s		// 文字列中の文字の検索 (str 内で c が最後に出現する位置を指すポインタを返す。c が見つからない場合は、NULL を返す。)
#define		Mstrstr				_tcsstr			// 文字列中の文字列の検索
#define		Mstrstr_s			_tcsstr_s		// 文字列中の文字列の検索
//#define	Mstrrstr			_tcsrstr		// 文字列中の文字列の検索
#define		Mstrtok				_tcstok			// 文字列中の最初のトークンを取り出す
#define		Mstrtok_s			_tcstok_s		// 文字列中の最初のトークンを取り出す
#define		Mstrlen				_tcslen			// 文字列の文字数を取得
#define		Mstrlen_s			_tcslen_s		// 文字列の文字数を取得
//	入出力
#define		Msprintf			_stprintf		// メモリ領域への書式付き出力
#define		Msprintf_s			_stprintf_s		// メモリ領域への書式付き出力
#define		M_snprintf			_sntprintf		// メモリ領域への書式付き出力
#define		Mscanf				_tscanf			// 書式付き入力
#define		Msscanf				_stscanf		// メモリ領域からの書式付き入力

#define		Mprintf				_tprintf		// 書式付き出力
#define		Mfprintf			_ftprintf		// ファイルへの書式付き出力
#define		Mfscanf				_ftscanf		// ファイルからの書式付き入力

//	評価
#define		Misalnum			_istalnum		// 英数字である
#define		Misalpha			_istalpha		// 英文字である
#define		Miscntrl			_istcntrl		// 削除文字または制御文字である
#define		Misdigit			_istdigit		// 0-9の数字である
#define		Misgraph			_istgraph		// 可視グラフィック文字である
#define		Mislower			_istlower		// 英小文字である
#define		Misprint			_istprint		// 印刷用文字である
#define		Mispunct			_istpunct		// 句読点である
#define		Misspace			_istspace		// SPACE,TAB,RETURN,NEWLINE,FORMFEED,垂直タブ
#define		Misupper			_istupper		// 英大文字である
#define		Misxdigit			_istxdigit		// 0-9,A-F,a-fの16進数字である

//	変換
#define		Mtolower			_totlower		// 文字を小文字に変換
#define		Mtoupper			_totupper		// 文字を大文字に変換
#define		Mstrlwr				_tcslwr			// 文字列の小文字化
#define		Mstrlwr_s			_tcslwr_s		// 文字列の小文字化
#define		Mstrupr				_tcsupr			// 文字列の大文字化
#define		Mstrupr_s			_tcsupr_s		// 文字列の大文字化
#define		Matoi				_tstoi			// 文字列を整数に変換
#define		Mwtoi				_tstoi			// 文字列を整数に変換
#define		Matol				_tstol			// 文字列を整数に変換
#define		Mwtol				_tstol			// 文字列を整数に変換
#define		Matof				( MREAL)_tstof	// 文字列を実数に変換
#define		Mwtof				( MREAL)_tstof	// 文字列を実数に変換
#define		Matodbl				_tstodbl		// 文字列を実数に変換
#define		Mwtodbl				_tstodbl		// 文字列を実数に変換

//===========================================================================
//		ファイル処理
//
#define		Mfopen				_tfopen			// ファイルオープン
#define		Mfclose				fclose			// ファイルクローズ
#define		Mfflush				fflush			// ストリームをフラッシュ

//===========================================================================
//		バッファ処理 fill memory, copy memory, copy memcopys
//
//===========================================================================
//	文字を文字列全体にコピーする( fill)
//
inline void MbFill( void *sdest, char cdt, int nsz)
{
	memset( sdest, cdt, nsz);
	//	for ( int n=0; n<nsz; sdest[n++]=cdt);
}

//===========================================================================
//	整数を整数配列全体にコピーする( fill)
//
inline void MbFill( int* idest, int idt, int nsz)
{
	for ( int n=0; n<nsz; idest[n++]=idt);
}

//===========================================================================
//	実数を実数配列全体にコピーする( fill)
//
inline void MbFill( MREAL *rdest, MREAL rdt, int nsz)
{
	for ( int n=0; n<nsz; rdest[n++]=rdt);
}

//===========================================================================
//	文字配列から文字配列へコピーする( memcpy)
//
inline void MbCopy( char *sdest, char *ssource, int nsz)
{
	memcpy( sdest, ssource, nsz);
//	for ( int n=0; n<nsz; n++) sdest[n] = ssource[n];
}

//===========================================================================
//	整数配列から整数配列へコピーする( memcpy)
//
inline void MbCopy( int* idest, int* isource, int nsz)
{
	memcpy( idest, isource, nsz*4);
//	for ( int n=0; n<nsz; n++)
//		idest[n] = isource[n];
}

//===========================================================================
//	実数配列から実数配列へコピーする( memcpy)
//
inline void MbCopy( MREAL *rdest, MREAL *rsource, int nsz)
{
	memcpy( rdest, rsource, nsz*4);
//	for ( int n=0; n<nsz; n++)
//		idest[n] = isource[n];
}

//===========================================================================
//	文字配列から文字配列へコピーする( memcpy)
//	領域の重なりも許可
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
//	整数配列から整数配列へコピーする( memcpy)
//	領域の重なりも許可
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
//	実数配列から実数配列へコピーする( memcpy)
//	領域の重なりも許可
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
//		バッファ管理
//

//===========================================================================
//	MbMAlcBuf
//
//		サイズ付きバッファ
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
//	MBMALLOC( アロケーションメモリアドレス設定ポインタ, 変数型, 確保要求サイズ);
//
//		データ型を指定したメモリアロケーション
//
//	例	MBMALLOC( dd, MREAL, MX_PNT1);
//			↓
//		MREAL	*dd = ( MREAL*)new char[MX_PNT1 * sizeof( MREAL)];
//				MBCHECK_POINTER( dd);
//
#define		MBMALLOC( P, T, S)	{ P = ( T*)new char[( S) * sizeof( T)]; MBCHECK_POINTER( P);}
#define		MBMALLOCF( F, P, T, S)	{ P = ( F*)new char[( S) * sizeof( T)]; MBCHECK_POINTER( P);}

//===========================================================================
//	MBFREE( アロケーションメモリアドレス設定ポインタ);
//
//		メモリアロケーション領域の開放
//
//	例	MBFREE( dd);
//			↓
//		if ( dd) {
//			delete [] ( char*)dd;
//			dd = NULL;
//		}
//
#define		MBFREE( P)			{ if ( P) { delete [] ( char*)P; P = NULL;}}

//===========================================================================
//	MBFREEST( アロケーションメモリアドレス設定ポインタ);
//
//		構造体の開放とメモリアロケーション領域の開放
//
//	例	MBFREE( dd);
//			↓
//		if ( dd) {
//			dd->Freee( );
//			delete [] ( char*)dd;
//			dd = NULL;
//		}
//
#define		MBFREEST( P)		{ if ( P) { P->Free( ); delete [] ( char*)P; P = NULL;}}

////===========================================================================
////	MBDELETE( newで作成配列領域の先頭アドレス設定ポインタ);
////
////	例	MBDELETEH( dd);
////			↓
////		if ( dd) {
////			delete [] dd;
////			dd = NULL;
////		}
////
//#define		MBDELETEH( P)	{ if ( P) {delete [] ( char*)P ; P = NULL;}}

//===========================================================================
//	MBDELETE( newで作成領域の先頭アドレス設定ポインタ);
//
//	例	MBDELETE( dd);
//			↓
//		if ( dd) {
//			delete dd;
//			dd = NULL;
//		}
//
#define		MBDELETE( P)		{ if ( P) { delete P ; P = NULL;}}

//===========================================================================
//	定数で指定したサイズの配列を確保する
//
//	MBBUFF( 作成領域の先頭アドレス設定ポインタ, 変数型, 初期配列サイズ( 定数)); 
//
//	例	MBBUFF( dd, MREAL, MX_PNT1);
//			↓
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
//	定数で指定したサイズの配列を確保する
//		初期に定数で指定したサイズの配列を定義しているが変数で指定したサイズの方が大きい場合にmallocで作成する。
//		mallocで確保された領域はProgのリターン時に自動的に削除される
//
//	MBMALCBUFF( 作成領域の先頭アドレス設定ポインタ, 変数型, 確保要求サイズ, 初期配列サイズ( 定数)); 
//
//	例	MBMALCBUFF( dd, MREAL, GPt2->n, MX_PNT1);
//			↓
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
////	MBBUFFまたはMBMALCBUFFで確保した領域のサイズを変更する
////
////	MBREALCBUFF( リアロケーションメモリアドレス設定ポインタ, 変数型, 確保要求サイズ);
////
////	例	MBREALCBUFF( dd, MREAL, GPt2->n);
////			↓
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
//		部分処理
//
#define		MBRED( Col)			( Col & 0xFF)
#define		MBGREEN( Col)		( ( Col >> 8) & 0xFF)
#define		MBBLUE( Col)		( ( Col >> 16)& 0xFF)

//===========================================================================
//		フラグチェック
//

inline bool MF_CHECK_ON( MINT a, MINT mask)						// maskの条件がＯＮであるかチェック
				{ return ( ( a & mask) != 0);}

inline bool MF_CHECK_OR( MINT a, MINT mask)						// maskのいずれかの条件がＯＮであるかチェック
				{ return ( ( a & mask) != 0);}

inline bool	MF_CHECK_AND( MINT a, MINT mask)					// maskの全ての条件がＯＮであるかチェック
				{ return ( ( a & mask) == mask);}

inline bool	MF_CHECK_EQ( MINT a, MINT mask)						// maskの全ての条件がセットされており
				{ return ( a == mask);}							// 他の条件のセットが無いことをチェック

inline bool	MF_CHECK_OFF( MINT a, MINT mask)					// maskの条件がＯＦＦであるかチェック
				{ return ( ( ~( a) & mask) != 0);}

inline bool	MF_CHECK_OFF_OR( MINT a, MINT mask)					// maskのいずれかの条件がＯＦＦであるかチェック
				{ return ( ( ~( a) & mask) != 0);}
				
inline bool	MF_CHECK_OFF_AND( MINT a, MINT mask)				// maskの全ての条件がＯＦＦであるかチェック
				{ return ( ( ~( a) & mask) == mask);}

inline void MF_SET_ON( MINT &a, MINT mask)						// maskの条件をセットする
				{ a |= mask;}

inline void MF_SET_OFF( MINT &a, MINT mask)						// maskの条件をクリアする
				{ a &= ~( mask);}

inline void MF_RESET_ON( MINT &a, MINT mask)					// maskの条件をクリアする
				{ a &= ~( mask);}

inline void MF_RESET( MINT &a)									// 前条件をクリアする
				{ a = 0;}

inline bool MI_CHECK_ODD( MINT n)								// 奇数チェック
				{ return ( n & 1);}

inline bool MI_CHECK_EVEN( MINT n)								// 偶数チェック
				{ return ( !MI_CHECK_ODD( n));}

#define 	MQUIT				{ goto exit;}
 				
#define 	MQUIT_IF_FAILED( p)	{ if ( !p) { rc = DM_MEM; goto exit;}}

#define		MQUIT_STS( s)		{ iSt = s; goto exit;}

//===========================================================================
//		ポインタチェック
//
//		アクセス可能なポインタである事をチェックする
//
//		CHECKDATA( p, n)	:	( p != 0) && ( p から n バイトのアクセス可能)
//		CHECKPTR( p)		:	( p != 0) && ( p から 1 バイトのアクセス可能)
//		CHECKFUNCPTR( p) 	:	( p != 0) && ( p にアクセス可能)
//		CHECKSTR( s)		:	( p != 0) && ( p から 30000文字のアクセス可能)
//		CHECKSTRUCTPTR( p)	:	( p != 0) && ( p から 構造体のサイズ分のアクセス可能)
//  
#define CHECKDATA( p, n)		( ( p) && !IsBadReadPtr( ( LPVOID)( p), ( n)))
#define CHECKPTR( p)			CHECKDATA( p, 1)
#define CHECKFUNCPTR( p) 		( ( p) && !IsBadCodePtr( ( FARPROC) ( p)))
#define CHECKSTR( s)			( ( s) && !IsBadStringPtr( ( LPCSTR)( s), 30000))
#define CHECKSTRUCTPTR( p)		CHECKDATA( p, sizeof( *p)) 

//===========================================================================
//		アロケーション誤りチェック
//			( out of memory)
//
//	使用方法:
//		  rc = MALLOC_FAIL( p = new CMyClass( a, b, c));
//
//	結果: rc = DM_MEM: アロケーション誤り
//		  rc = SUCCESS: アロケーション成功
//
#define MALLOC_FAIL( object) ( object) ? SUCCESS: DM_MEM

//===========================================================================
//		タイプを指定した２つのオブジェクトを交換
//
#define MSWAP( x,y,type)			{type z = x; x = y; y = z;}

//===========================================================================
//		0～1. 間の ランダム実数値を発生
//
#define FRAND	( ( double)rand( )/( double)RAND_MAX)


namespace MC
{

//===========================================================================
//		汎用関数
//

#ifdef DLL_EXPORT
	#undef DLL_EXPORT
#endif

#ifdef DLL_EXPORT_BASE_DO
	#pragma message( "<<< Base・dll_EXport >>>")
	#define DLL_EXPORT							__declspec( dllexport)
#else
	#ifdef _DEBUG								// Release Mode で inline関数がundefになる不具合対応
		#ifdef DLL_NO_IMPORT_MBASIC_DO
			#define DLL_EXPORT
		#else
//			#pragma message( "=== Base・dll_IMport ===")
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
//		トレース

/////////////////////////////////////////////////////////////////////////////
//		日本語モード

static void 	MbSetJapanese( );

/////////////////////////////////////////////////////////////////////////////
//		システムエラーステイタス

#define	 	MC_ERR_COMMAND		1200			// コマンドエラー
#define	 	MC_ERR_ID			1201			// ＩＤ誤りエラー
#define		MC_ERR_POINTER		1202			// メモリ取得エラー
#define		MC_ERR_OVERFLOW		1203			// 領域オーバフローエラー
#define		MC_ERR_OTHER	   -999				// その他エラー

//		メモリ取得エラー表示
#define		MB_DM_MEM			{ MbSysError( X, MC_ERR_POINTER);}

/////////////////////////////////////////////////////////////////////////////
//		ステイタス
#define		MC_SUCCESS			  0
#define		MC_FAILURE			 ~0

#define		MC_E_CANCEL			 13
#define		MC_E_NOMSG			 14

//S void	MbSysErrorId( MINT id_err);

/////////////////////////////////////////////////////////////////////////////
//		リターン
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
						const char*		i_Program,	// エラープログラム
						MINT			i_ErrorCode	// エラーコード
					);
static void 	MbCmdError( 
						const char*		i_Program,	// エラープログラム
						MINT			i_ErrorCode	// エラーコード
					);

//===========================================================================
//	エラーリターンブレークポイント用
//
void	Base::MbErrorReturn( void);

/////////////////////////////////////////////////////////////////////////////////
//////		リターンコード
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
//	MGCHECK_ZEROVECT( ゼロチェックのベクトル長, "プログラム名");
#define		MBCHECK_ZEROVECT(X,Z,T)\
								{ if ( ( X) < T)  Base::MbSysError( Z, MC_ERR_VECT_ZERO);}

#define		MBCHECK_POINTER(P)	{ if ( P == NULL) Base::MbSysError( __FILE__ + __LINE__, MC_ERR_POINTER);}


#ifdef DLL_EXPORT
	#undef DLL_EXPORT
#endif

#ifdef DLL_EXPORT_CODE_DO
	#pragma message( "<<< Code・dll_EXport >>>")
	#define DLL_EXPORT							__declspec( dllexport)
#else
	#ifdef _DEBUG								// Release Mode で inline関数がundefになる不具合対応
		#ifdef DLL_NO_IMPORT_MBASIC_DO
			#define DLL_EXPORT
		#else
//			#pragma message( "=== Code・dll_IMport ===")
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
//【機能】MBCS文字列をUNICODE文字列に変換する
//【返値】 0 : 正常     -1 : 異常
//【注意】なし
// ********************************************************************************

	static MINT Mmbstowcs( 
						const char*		i_StrC,			// MBCS文字列
						MCHAR*			o_StrU,			// UNICODE文字列
						MINT			i_iSize			// 最大文字数
						);

// *******************************************************************************
//										Created on 10/01/00 by K.Matsubayashi
//										Updated on xx/xx/xx/ by x.xxxxxx
//										Copyright
//【機能】UNICODE文字列をMBCS文字列に変換する
//【返値】 0 : 正常     -1 : 異常
//【注意】なし
// ********************************************************************************

	static MINT Mwcstombs( 
						const MCHAR*	i_StrU,			// UNICODE文字列
						char*			o_StrC,			// MBCS文字列
						MINT			i_iSize			// 最大文字数
						);
//};

// 文字列を小文字に変換
static void Mstrtolower(
						const MCHAR*	i_pstr,			// 変換前の文字列
						MCHAR			o_pstrL[],		// 変換後の文字列バッファ
						size_t			i_SzstrL)		// 文字列バッファサイズ
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

// 文字列を大文字に変換
static void Mstrtoupper(
						MCHAR	in_pstr[],		// 変換前の文字列
						MCHAR	out_pstrU[],	// 変換後の文字列バッファ
						MINT	in_SzstrU)		// 最大文字数(バッファサイズ - 1)
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

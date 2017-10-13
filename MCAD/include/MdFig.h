#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MdFig.h
//
//		図形情報
//
//

#include	"MdmDefine.h"
#include	"MdAry.h"
#include	"MdAryH.h"
#include	"MdZukei.h"

#ifdef DLL_EXPORT_FIG
	#undef DLL_EXPORT_FIG
#endif

#ifdef DLL_EXPORT_MDFIG_DO
	#pragma message( "<<< MdFig・dll_EXport >>>")
	#define DLL_EXPORT_FIG							__declspec( dllexport)
#else
	#ifdef _DEBUG								// Release Mode で inline関数がundefになる不具合対応
		#ifdef DLL_NO_IMPORT_MD_DO
			#define DLL_EXPORT_FIG
		#else
//			#pragma message( "=== MdFig・dll_IMport ===")
			#define DLL_EXPORT_FIG						__declspec( dllimport)
		#endif
	#else
		#define DLL_EXPORT_FIG
	#endif
#endif

//==========================================================================================
//	図形情報
//	・
//
//==========================================================================================

#define MP_SZ_MDFIG		10
#define MP_MXSZ_MDFIG	0xffff
#define MP_SZ_MDHDR		1
#define MP_EOF_MDFIG	-1

#define MC_INTRGB		0x80			// 4バイト型式	A,R,G,B

#define MC_AMBIENT		1				// 環境光
#define MC_DIFFUSE		2				// 拡散光
#define MC_SPECULAR		3				// 鏡面光
#define MC_EMISSION		4				// 放射光
#define MC_AMBIENT_AND_DIFFUSE	5		// 環境光と拡散光

typedef	MREAL		MDPMTDT;

namespace MC
{

// 基本要素
class MINT1 {
public:
	MINT i[1];

//	MINT1() {}
	MINT1( MINT i_i)
		{ i[0] = i_i;}
	MINT1& operator = ( const MINT i_i)
						{ i[0] = i_i; return *this;}
	MINT1& operator = ( const MINT i_i[1])
						{ i[0] = i_i[0]; return *this;}
};

class MINT2
{
public:
	MINT i[2];

//	MINT2() {}
	MINT2( MINT i_i1, MINT i_i2)
		{ i[0] = i_i1; i[1] = i_i2;}
	MINT2& operator = ( const MINT i_i[2])
						{ i[0] = i_i[0]; i[1] = i_i[1]; return *this;}
};

class MINT3
{
public:
	MINT i[3];

//	MINT3() {}
	MINT3( MINT i_i1, MINT i_i2, MINT i_i3)
		{ i[0] = i_i1; i[1] = i_i2; i[2] = i_i3;}
	MINT3& operator = ( const MINT i_i[3])
						{ i[0] = i_i[0]; i[1] = i_i[1]; i[2] = i_i[2]; return *this;}
};

class MINT4 { MINT i[4];};
class MINT5 { MINT i[5];};
class MINT6 { MINT i[6];};
class MINT7 { MINT i[7];};
class MINT8 { MINT i[8];};
class MINT9 { MINT i[9];};
class MINT10 { MINT i[10];};

class MREAL1
{
public:
	MREAL r[1];

//	MREAL1() {}
	MREAL1( MREAL i_r)
		{ r[0] = i_r;}
	MREAL1& operator = ( const MREAL i_r)
						{ r[0] = i_r; return *this;}
	MREAL1& operator = ( const MREAL i_r[1])
						{ r[0] = i_r[0]; return *this;}
};

class MREAL2
{
public:
	MREAL r[2];

//	MREAL2() {}
	MREAL2( MREAL i_r1, MREAL i_r2)
		{ r[0] = i_r1; r[1] = i_r2;}
	MREAL2& operator = ( const MREAL i_r[2])
						{ r[0] = i_r[0]; r[1] = i_r[1]; return *this;}
};

class MREAL3 
{
public:
	MREAL r[3];

//	MREAL3() {}
	MREAL3( MREAL i_r1, MREAL i_r2, MREAL i_r3)
		{ r[0] = i_r1; r[1] = i_r2; r[2] = i_r3;}
	MREAL3& operator = ( const MREAL i_r[3])
						{ r[0] = i_r[0]; r[1] = i_r[1]; r[2] = i_r[2]; return *this;}
};
class MREAL4 { MREAL r[4];};
class MREAL5 { MREAL r[5];};
class MREAL6 { MREAL r[6];};
class MREAL7 { MREAL r[7];};
class MREAL8 { MREAL r[8];};
class MREAL9 { MREAL r[9];};
class MREAL10 { MREAL r[10];};

class FLOAT1
{
public:
	float f[1];

//	FLOAT1() {}
	FLOAT1( float i_f)
		{ f[0] = i_f;}
	FLOAT1& operator = ( const float i_f)
						{ f[0] = i_f; return *this;}
	FLOAT1& operator = ( const float i_f[1])
						{ f[0] = i_f[0]; return *this;}
};
class FLOAT2
{
public:
	float f[2];

//	FLOAT2() {}
	FLOAT2( float i_f1, float i_f2)
		{ f[0] = i_f1; f[1] = i_f2;}
	FLOAT2& operator = ( const float i_f[2])
						{ f[0] = i_f[0]; f[1] = i_f[1]; return *this;}
};
class FLOAT3
{
public:
	float f[3];

//	FLOAT3() {}
	FLOAT3( float i_f1, float i_f2, float i_f3)
		{ f[0] = i_f1; f[1] = i_f2; f[2] = i_f3;}
	FLOAT3& operator = ( const float i_f[3])
						{ f[0] = i_f[0]; f[1] = i_f[1]; f[2] = i_f[2]; return *this;}
};
class FLOAT4 { float f[4];};
class FLOAT5 { float f[5];};
class FLOAT6 { float f[6];};
class FLOAT7 { float f[7];};
class FLOAT8 { float f[8];};
class FLOAT9 { float f[9];};
class FLOAT10 { float i[10];};

class DOUBLE1
{
public:
	double d[1];

//	DOUBLE1() {}
	DOUBLE1( double i_d)
						{ d[0] = i_d;}
	DOUBLE1& operator = ( const double i_d)
						{ d[0] = i_d; return *this;}
	DOUBLE1& operator = ( const double i_d[1])
						{ d[0] = i_d[0]; return *this;}
};
class DOUBLE2
{
public:
	double d[2];

//	DOUBLE2() {}
	DOUBLE2( double i_d1, double i_d2)
						{ d[0] = i_d1; d[1] = i_d2;}
	DOUBLE2& operator = ( const double i_d[2])
						{ d[0] = i_d[0]; d[1] = i_d[1]; return *this;}
};
class DOUBLE3 {
public:
	double d[3];

//	DOUBLE3() {}
	DOUBLE3( double i_d1, double i_d2, double i_d3)
						{ d[0] = i_d1; d[1] = i_d2; d[2] = i_d3;}
	DOUBLE3& operator = ( const double i_d[3])
						{ d[0] = i_d[0]; d[1] = i_d[1]; d[2] = i_d[2]; return *this;}
};
class DOUBLE4 { double d[4];};
class DOUBLE5 { double d[5];};
class DOUBLE6 { double d[6];};
class DOUBLE7 { double d[7];};
class DOUBLE8 { double d[8];};
class DOUBLE9 { double d[9];};
class DOUBLE10 { double d[10];};

class MHALF {
public:
	MSHORT	m_iH1;
	MSHORT	m_iH2;

	MHALF( MSHORT i_iH1, MSHORT i_iH2) { m_iH1 = i_iH1; m_iH2 = i_iH2;}
	MHALF& operator = ( const MUINT i_i)
						{ *(UINT*)this = i_i; return *this;}
	MUINT& operator = ( const MHALF i_h)
						{ return *(UINT*)this;}
};

class MUHALF {
public:
	MUSHORT	m_iH1;
	MUSHORT	m_iH2;

	MUHALF( MUSHORT i_iH1, MUSHORT i_iH2) { m_iH1 = i_iH1; m_iH2 = i_iH2;}
	MUHALF& operator = ( const MUINT i_i)
						{ *(UINT*)this = i_i; return *this;}
	MUINT& operator = ( const MUHALF i_h)
						{ return *(UINT*)this;}
};

class MQUARTER {
public:
	MBYTE	m_iQ4;
	MBYTE	m_iQ3;
	MBYTE	m_iQ2;
	MBYTE	m_iQ1;

	MQUARTER( MBYTE i_iQ1, MBYTE i_iQ2, MBYTE i_iQ3, MBYTE i_iQ4)
					{ m_iQ1 = i_iQ1; m_iQ2 = i_iQ2; m_iQ3 = i_iQ3; m_iQ4 = i_iQ4;}
	MQUARTER& operator = ( const MUINT i_i)
						{ *(UINT*)this = i_i; return *this;}
	MUINT& operator = ( const MQUARTER i_h)
						{ return *(UINT*)this;}
};

class MUQUARTER {
public:
	MUBYTE	m_iQ4;
	MUBYTE	m_iQ3;
	MUBYTE	m_iQ2;
	MUBYTE	m_iQ1;

	MUQUARTER( MUBYTE i_iQ1, MUBYTE i_iQ2, MUBYTE i_iQ3, MUBYTE i_iQ4)
					{ m_iQ1 = i_iQ1; m_iQ2 = i_iQ2; m_iQ3 = i_iQ3; m_iQ4 = i_iQ4;}
	MUQUARTER& operator = ( const MUINT i_i)
						{ *(UINT*)this = i_i; return *this;}
	MUINT& operator = ( const MUQUARTER i_h)
						{ return *(UINT*)this;}
};

inline MINT MNWORD( MCHAR* i_pcStr)
					{ return ( (int)Mstrlen( i_pcStr) * sizeof( MCHAR) / sizeof( MINT) + 1);}
inline MINT MNWORD( char* i_pcStr)
					{ return ( (int)strlen( i_pcStr) * sizeof( char) / sizeof( MINT) + 1);}

// カラーマップ部
typedef struct _pgrgbquad {
	MBYTE		Blue;																	// ｶﾗｰﾏｯﾌﾟｴﾝﾄﾘの青の値
	MBYTE		Green;																	// ｶﾗｰﾏｯﾌﾟｴﾝﾄﾘの緑の値
	MBYTE		Red;																	// ｶﾗｰﾏｯﾌﾟｴﾝﾄﾘの赤の値
	MBYTE		Reserved;																// 常に0
} MGRGBQUAD;

//// short データ
//class MSHORT2
//{
//	MUSHORT		S2;
//	MUSHORT		S1;
//public:
//	MSHORT2( MUSHORT s1, MUSHORT s2) { S1 = s1; S2 = s2;}
//};

//==========================================================================================

// 図形要素ヘッダ
class DLL_EXPORT_FIG MDPHD
{
public:
	MUBYTE		m_icd;							// 図形コード(icd)
	MUBYTE		m_itp;							// 図形タイプ			BIT(0,1,2,3,4,5,6,7)単位で定義
												// 図形モード			BIT(1,2,3,4,5,6,7)
												// 図形次元(idim)		BIT(0):	0:3Dデータ、1:2Dデータ
	MUSHORT		m_n;								// 図形データサイズ(ヘッダ部を除く)
};

///////////////////////////////////////////////////////////////////////////////
// 図形要素(可変長ワード(仮に整数で定義))
class DLL_EXPORT_FIG MDPMT							// サイズ指定が最大2**16-1までの形式
{
	friend	class	MDFIG;

	MDPHD		m_hd;							// 図形要素ヘッダ
	union {
		MDPMTDT	m_dt[MP_SZ_MDFIG - 1];				// 図形情報(図形要素郡)
		MINT	m_i[MP_SZ_MDFIG - 1];					// デバッグ参照用
		MDPHD	m_h[MP_SZ_MDFIG - 1];
	};

public:
	// Head
	MINT	GetCd() const	{ return m_hd.m_icd;}			// プリミティブコードを取得
	MINT	GetDm() const	{ return (m_hd.m_itp & 0x80);}	// プリミティブの次元を取得 ( 0;３次元, 1:２次元)
	MINT	GetMd() const	{ return (m_hd.m_itp & 0x7F);}	// プリミティブのモード
	MINT	GetTp() const	{ return m_hd.m_itp;}			// プリミティブのタイプコード
	MINT	GetN()	const	{ return m_hd.m_n;}				// プリミティブデータ数を取得

	void SetHd( MINT i_icd, MINT i_idm, MINT i_n)
							{ m_hd.m_icd = i_icd;
							  m_hd.m_itp = i_idm;
							  m_hd.m_n = i_n;}
	void SetCd( MINT i_icd)	{ m_hd.m_icd = i_icd;}			// プリミティブコードを設定
	void SetDm( MINT i_idm)	{ m_hd.m_itp = ( m_hd.m_itp & 0x7F) | i_idm;}
															// プリミティブの次元を設定 ( 0;３次元, 1:２次元)
	void SetMd( MINT i_imd)	{ m_hd.m_itp = ( m_hd.m_itp & 0x80) | i_imd;}
															// プリミティブのモードを設定 ( 0 ～ 0x3F)
	void SetTp( MINT i_itp)	{ m_hd.m_itp = i_itp;}
															// プリミティブのタイプコードを設定
	void SetN( MINT i_n)	{ m_hd.m_n = i_n;}				// プリミティブデータ数を設定

	// Data
	MDPMTDT	GetDt( MINT i_ip)
							{ return m_dt[i_ip];}			// プリミティブデータ要素
	MDPMTDT	*GetpDt( MINT i_ip)
							{ return &m_dt[i_ip];}			// プリミティブデータ要素へのポインタ
	void SetDt( MINT i_ip, MDPMTDT i_dt)
							{ m_dt[i_ip] = i_dt;}			// プリミティブデータ要素

	// MDPMT
	void zPush( void);
	void zPull( void);
	void zMarker( MINT i_iTp, MREAL i_rSz);					// マーカ
	void zPaint( MINT i_iTp, MINT i_iId, MINT i_iEdg);		// 塗りつぶし
	void zLineTp( MINT i_iTp);								// 線種
	void zColorRGB( MUQUARTER i_iRgb);						// 色( RGB) ( byte * 3)
	void zColorRGB( FLOAT3& i_fRgb);						// 色( RGB) ( float * 3)
	void zColorPlt( MINT i_iPltN);							// 色(パレットNo)
	void zTransmissivity( MFLOAT i_fTransm);				// 透過率 ( 0. ～ 1.)
	void zLineWd( MFLOAT i_rLWd);							// 線幅
	void zMaterialRGBA( MINT i_iMtN, MUQUARTER i_iRgb);		// 材質( RGB) ( byte * 3)
	void zMaterialRGBA( MINT i_iMtN, FLOAT3& i_f3Rgb,
						MFLOAT i_fAlpha = 1);				// 材質( RGB) ( float * 3)
	void zMaterialShininess( MREAL i_rShns);						// 材質鏡面光(光沢)係数
	void zTexture( MINT i_iTxtrId);							// テクスチャ
	void zTextureCoordinate(
						MINT		i_iMdCA,
						MINT		i_iMdCB = 0,
				const	MgVect2*	i_vszimg = 0,
				const	MgMat3*		i_matH = 0);			// テクスチャ座標
	void zTextureParam( MINT i_iTp, MINT i_iPxLap,
					    MINT i_iPyLap, MINT i_iFltEx,
					    MINT i_iFltRd, MREAL3& i_fColBlc1,
					    MREAL3& i_fColBlc2);				// テクスチャパラメータ
	void zShininess( MREAL i_fShns);						// 鏡面反射率 ( 0. ～ 1.)
	void zPovRay( MCHAR* i_pcStr);							// PovRay出力属性

	void Point3( const MgPoint3&	i_Pt);
	void Line3( const MgLine3&		i_Ln);
	void ULine3( const MgULine3&	i_uLn);
	void HLine3( const MgHLine3&	i_hLn);
	void Polyl3( const MgPolyl3&	i_Ply);
	void Polyg3( const MgPolyg3&	i_Pgn);
	void Cir3( const MgCir3&		i_Cir);
	void Arc3( const MgArc3&		i_Arc);
//U	void Elps3( const MgElps3&		i_Elps);
//U	void Ela3( const MgEla3&		i_Ela);
	void Plane3( const MgPlane3&	i_Pln);
	void IPlane3( const MgPlane3&	i_iPln);
//U	void Cyl3( const MgCly3&		i_Cly);
//U	void SLine3( const MgSLine3&	i_sLn);
};

// 図形情報
class MDFIG;

///////////////////////////////////////////////////////////////////////////////
// 図形要素郡(可変長ワード(仮に整数で定義))
class DLL_EXPORT_FIG MDPMTL
{
	friend class MDFIG;
	MUSHORT		m_sz;							// バッファサイズ
	MUSHORT		m_n;							// データサイズ(ヘッダ部を除く)
//	MgMinMaxR3	m_Mmx;

	union {
		MDPMTDT	m_dt[MP_SZ_MDFIG];				// 図形情報(図形要素郡)
		MINT	m_i[MP_SZ_MDFIG];				// デバッグ参照用
		MDPHD	m_h[MP_SZ_MDFIG - 1];
	};

public:
	MDPMTL*	New( MINT i_sz);

	MINT	GetSz() const;						// 図形情報バッファサイズ
	MINT	GetN() const;						// 図形情報サイズ
	MDPMT*	GetpPmt( MINT i_ip);				// 図形情報ポインタ

	void	SetSz( MINT i_sz);					// 図形情報バッファサイズ
	void	SetN( MINT i_n);					// 図形情報サイズ
};

inline MINT	MDPMTL::GetSz()	const				// 図形情報バッファサイズ
{ 
	return m_sz;
}
inline MINT	MDPMTL::GetN() const				// 図形情報サイズ
{
	return m_n;
}
inline MDPMT*	MDPMTL::GetpPmt( MINT i_ip)		// 図形情報ポインタ
{
	return (MDPMT*)&m_dt[i_ip];
}

inline void	MDPMTL::SetSz( MINT i_sz)			// 図形情報バッファサイズ
{
	m_sz = i_sz;
}
inline void	MDPMTL::SetN( MINT i_n)				// 図形情報サイズ
{
	m_n = i_n;
}

///////////////////////////////////////////////////////////////////////////////
// 図形情報
class DLL_EXPORT_FIG MDFIG
{
	MDPMTL*		m_pPmtl;						// 図形情報

public:
	// コンストラクタ			初期はサイズ指定が最大255までの形式
	MDFIG();									
	MDFIG( MINT i_sz);
	~MDFIG()			{ Free();}

	void 	Free()		{ MBFREE( m_pPmtl);}

	MINT	GetSz() const;						// 図形情報バッファサイズ
	MINT	GetN() const;						// 図形情報サイズ
	MDPMTL*	GetpPmtl();							// 図形情報のポインタ

	void	SetSz( MINT i_sz);					// 図形情報バッファサイズ
	void	SetN( MINT i_n);					// 図形情報サイズ
	void	SetpPmtl( MDPMTL* i_pPmts);			// 図形情報のポインタ

	// 図形要素
	MDPMT*	GetpPmt( MDID i_idl);				// 図形要素のポインタを取得
	MINT	GetPmtCd( MDID i_idl);				// 図形要素のコード
	MINT	GetPmtDm( MDID i_idl);				// 図形要素の次元
	MINT	GetPmtTp( MDID i_idl);				// 図形要素のタイプ
	MINT	GetPmtN( MDID i_idl);				// 図形要素のサイズ
	MDID	GetNextPmt( MDID i_idl);			// 次の図形要素のIDL(配列番号)

	void	SetHd( MDID i_idl, MINT i_icd,
				   MINT i_idm, MINT i_n);		// 図形要素のコード、次元、サイズ
	void	SetPmtCd( MDID i_idl, MINT i_icd);	// 図形要素のコード
	void	SetPmtDm( MDID i_idl, MINT i_idm);	// 図形要素の次元
	void	SetPmtTp( MDID i_idl, MINT i_itp);	// 図形要素のタイプ
	void	SetPmtN( MDID i_idl, MINT i_n);		// 図形要素のサイズ

	// 図形情報の最後に要素を追加
	//	バッファサイズが不足する場合は２倍に領域拡張(拡張後の最低領域サイズは４とする)
	void operator += ( const MREAL i_r); 		// 図形情報郡に図形情報要素の追加	図形情報 += 要素
	void operator += ( const MINT i_i)			// 図形情報郡に図形情報要素の追加	図形情報 += 要素
				{ *this += *(MREAL*)&i_i;}

	// 図形情報をコピー
	void operator = ( const MDFIG& i_Fig);		// 複写		構造体群 = 構造体群
	void operator = ( const MDPMT& i_Pmt);		// 複写		構造体群 = 構造体群

	// 図形情報に図形情報を追加
	//	バッファサイズが不足する場合は２倍に領域拡張(拡張後の最低領域サイズは４とする)
	void operator += ( const MDFIG& i_Fig);		// 構造体群の追加	構造体群 += 構造体群
	void operator += ( const MDPMT& i_Pmt);		// 構造体群の追加	構造体群 += 構造体群

	// 図形情報を得る
	MDFIG& operator () ( MINT i_idx);			// 図形情報郡から指定番目の図形要素を読み書きする

	// 領域確保とサイズ変更
	void Alloc( MINT i_sz);						// 図形情報郡の領域サイズの領域確保
	void ReSize( MINT i_sz);					// 図形情報郡の領域サイズの変更と領域確保
	void IncrN( MINT i_n);						// 図形情報郡の領域サイズの拡張と領域確保
	MDPMT* PmtAlloc( MINT i_n);					// プリミティブ領域を確保

	// 図形属性
	void zPush( void);
	void zPull( void);
	void zMarker( MINT i_iTp, MREAL	i_rSz);					// マーカ
	void zPaint( MINT i_iTp, MINT i_iId, MINT i_iEdg);		// 塗りつぶし
	void zLineTp( MINT i_iTp);								// 線種
	void zColorRGB(	MUQUARTER i_iRgb);						// 色(RGB) {byte} ( 0, r, g, b)
	void zColorRGB(	FLOAT3& i_f3Rgb);						// 色(RGB) {float} ( 1., 1., 1.) (1.)
	void zColorPlt( MINT i_iPltN);							// 色(パレット)
	void zTransmissivity( MFLOAT i_fTransm);				// 透過率 ( 0. ～ 1.)
	void zLineWd( MFLOAT i_rLWd);							// 線幅
	void zMaterialRGBA( MINT i_iMtN, MUQUARTER i_iRgb);		// 材質(RGB) {byte} ( A{透過率}, R, G, B)
	void zMaterialRGBA( MINT i_iMtN,						// 材質(RGB) {float}
						FLOAT3& i_f3Rgb, MFLOAT i_fAlpha);	// A 透過率	( 0. ～ 1.)
	void zMaterialShininess( MREAL i_rShns);				// 材質鏡面光(光沢)係数 ( 0. ～ 128.)
	void zTexture( MINT i_iTxtrId);							// テクスチャ
	void zTextureCoordinate(
						MINT		i_iMdCA,
						MINT		i_iMdCB, 
				const	MgVect2*	i_vszimg,
				const	MgMat3*		i_matH);				// テクスチャ座標
	void zTextureParam( MINT i_iTp, MINT i_iPxLap,
						MINT i_iPyLap, MINT i_iFltEx,
						MINT i_iFltRd, MREAL3& i_fColBlc1,
						MREAL3& i_fColBlc2);				// テクスチャパラメータ
	void zShininess( MREAL i_fShns);						// 鏡面反射率 ( 0. ～ 1.)
	void zPovRay( MCHAR* i_pcStr);							// PovRay出力属性 文字列

	// 図形要素
	void Point3( const MgPoint3&	i_Pt);
	void Line3( const MgLine3&		i_Ln);
	void ULine3( const MgULine3&	i_uLn);
	void HLine3( const MgHLine3&	i_hLn);
	void Polyl3( const MgPolyl3&	i_Ply);
	void Polyg3( const MgPolyg3&	i_Pgn);
	void Cir3( const MgCir3&		i_Cir);
	void Arc3( const MgArc3&		i_Arc);
//U	void Elps3( const MgElps3&		i_Elps);
//U	void Ela3( const MgEla3&		i_Ela);
	void Plane3( const MgPlane3&	i_Pln);
	void IPlane3( const MgPlane3&	i_iPln);
//U	void Cyl3( const MgCly3&		i_Cly);
//U	void SLine3( const MgSLine3&	i_sLn);

	// デバッグ用トレース出力
	void Print( MCHAR* i_s);

};

//	コンストラクタ
inline MDFIG::MDFIG()									
{
	m_pPmtl = 0;
}

inline MDFIG::MDFIG(
						MINT	i_sz			// 図形情報サイズ
				)
{
	MBMALLOCF( MDPMTL, m_pPmtl, MINT, i_sz);
	m_pPmtl->SetSz( i_sz);
	m_pPmtl->SetN( 0);
}

} // namespace MC
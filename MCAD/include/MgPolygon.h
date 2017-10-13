#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgPolygon.h
//
//		
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================

#include "MbPlex.h"

//
//	MgPolyg.h 
//	Copyright;
// 
//	説明
//
//	MgPolyg2は２次元の多角形を、MgPolyg3は３次元の多角形を定義する。
//	MgGPolyg2は２次元の多角形群を、MgGPolyg3は３次元の多角形群を定義する。
//	MgGLine2は２次元の線分群を、MgGPolyg3は３次元の線分群を定義する。
//	MgGIntは整数群を、MgGRealは実数群を定義する。
//	MgGPoint2は２次元の点群を、MgGPoint3は３次元の点群を定義する。
//	オーバレイ関数を定義する。
//
#include "MgPlane.h"
#include "MgRect.h"
#include "MgPolyline.h"

#ifdef DLL_EXPORT_POLYGON
	#undef DLL_EXPORT_POLYGON
#endif

#ifdef DLL_EXPORT_POLYGON_DO
	#pragma message( "<<< MgPolyg・dll_EXport >>>")
	#define DLL_EXPORT_POLYGON							__declspec( dllexport)
#else
	#ifdef _DEBUG								// Release Mode で inline関数がundefになる不具合対応
		#ifdef DLL_NO_IMPORT_GEO_DO
			#define DLL_EXPORT_POLYGON
		#else
//			#pragma message( "=== MgPolyg・dll_IMport ===")
			#define DLL_EXPORT_POLYGON						__declspec( dllimport)
		#endif
	#else
		#define DLL_EXPORT_POLYGON
	#endif
#endif

namespace MC
{

class MgGPoint2;
class MgGPoint3;
class MgGPolyg2;
class MgGPolyg3;
class MgPolyg3;
class MgGLine2;
class MgGLine3;
class MgGReal;

//======================( ２次元 )==============================
//		多角形
class DLL_EXPORT_POLYGON MgPolyg2						// ２Ｄポリゴン
{
public:
	MUBYTE		m_iTp;							// データタイプ　MGTP_POLYG
	MUBYTE		m_fa;							// アロケーションフラグ(0:配列, 1:malloc)
												// 		=1の場合は使用終了時にfreeで領域の開放を必要とする
	MSHORT		m_isz;							// 最大頂点数
	MSHORT		m_n;							// 頂点数
	MgPoint2* 	m_p;
public:
	MgPolyg2()			{ m_iTp = MGTP_POLYG; m_isz = m_n = m_fa = 0; m_p = NULL;}
	MgPolyg2( MINT sz);
	MgPolyg2( MINT sz1, MgPoint2 *p2);
	~MgPolyg2()			{ if ( m_fa) MBFREE( m_p);}									// 領域開放
//
	void Init()			{ m_iTp = MGTP_POLYG; m_isz = m_n = m_fa = 0; m_p = NULL;}	// 領域初期化
	void Free()			{ if ( m_fa) MBFREE( m_p); m_isz = m_n = m_fa = 0;}			// 領域開放
//	
	const MgPoint2& operator () ( MINT idx) const;
	MgPoint2& operator () ( MINT idx);
	const MgPoint2& operator [] ( MINT idx) const								// ()のチェック機能と領域拡張機能を取り外し高速版	
						{ return m_p[idx];}
	MgPoint2& operator [] ( MINT idx)											// ()のチェック機能と領域拡張機能を取り外し高速版	
						{ return m_p[idx];}
	void operator = (const MgPolyg2 &pg);
	void operator = (const MC::MgPolyg3 &Pg);										// ３次元→２次元
	void operator += (const MgPoint2& pt);										// ポリゴンに点を追加		Pg1 += Pt2
	void operator += (const MgPolyg2 &pg);										// ポリゴンにポリゴンを追加	Pg1 += Pg2
	void operator -= (const MINT dn)											// ポリゴンの最終ｎ点を削除
						{ m_n = max( m_n -= dn, 0);}
	void operator -- ()															// ポリゴンの最終点を削除
						{ if ( m_n>0) -- m_n;}
	void operator -- (int)														// ポリゴンの最終点を削除
						{ if ( m_n>0) m_n --;}
//
	void SetInit()		{ m_isz = m_n = m_fa = 0; m_p = NULL;}					// この群がさらに群になった場合の最終データより後のデータのクリア用
	void Set( const MgPolyg3 &Pg);												// ３次元→２次元代入用
	MINT Rotation() const;
	void Rev();
	void Resize( MINT szn);
	MREAL Area();
	MgMinMaxR2 MinMax();
	void Print( MCHAR* s) const;												// Print
};

//======================( ３次元 )==============================
//		多角形
//
class DLL_EXPORT_POLYGON MgPolyg3						// ３Ｄポリゴン
{
public:
	MUBYTE		m_iTp;							// データタイプ　MGTP_POLYG3
	MUBYTE		m_fa;							// アロケーションフラグ(0:配列, 1:malloc)
												// 		=1の場合は使用終了時にfreeで領域の開放を必要とする
	MSHORT		m_isz;							// 最大頂点数
	MSHORT		m_n;							// 頂点数
	MgPoint3*	m_P;
public:
	MgPolyg3()			{ m_iTp = MGTP_POLYG3; m_isz = m_n = m_fa = 0; m_P = NULL;}
	MgPolyg3( MINT sz1);
	MgPolyg3( MINT sz1, MgPoint3 *p2);
	~MgPolyg3()			{ if ( m_fa) MBFREE( m_P);}								// 領域開放
//
	void Init()			{ m_iTp = MGTP_POLYG; m_isz = m_n = m_fa = 0; m_P = NULL;}// 領域初期化
	void Free()	
				{
					if ( m_fa)
						MBFREE( m_P);
					m_isz = m_n = m_fa = 0;
				}					// 領域開放
//
	const MgPoint3& operator () ( MINT idx) const;
	MgPoint3& operator () ( MINT idx);
	const MgPoint3& operator [] ( MINT idx) const								// ()のチェック機能と領域拡張機能を取り外し高速版	
						{ return m_P[idx];}
	MgPoint3& operator [] ( MINT idx)											// ()のチェック機能と領域拡張機能を取り外し高速版	
						{ return m_P[idx];}
	void operator = (const MgPolyg3 &Pg);
	void operator = (const MgPolyg2 &pg);										// ２次元→３次元 (z = 0.)
//						{ *this = MgPolyg3( pg);}
	void operator += (const MgPoint3& Pt);										// Pg1 += Pt2
	void operator += (const MgPolyg3 &Pg);										// Pg1 += Pg2
	void operator -= (const MINT dn)
						{ m_n = max( m_n -= dn, 0);}
	void operator -- ()
						{ if ( m_n>0) -- m_n;}
	void operator -- (int)
						{ if ( m_n>0) m_n --;}
//
	void SetInit()		{ m_isz = m_n = m_fa = 0; m_P = NULL;}					//この群がさらに群になった場合の最終データより後のデータのクリア用
	void Set( const MgPolyg2 &pg, MREAL z = 0.);								// ２次元→３次元代入用 (z = 0.)
	void Set( const MgPolyg2 &pg, const MgPlane3& Ply);							// ２次元→３次元代入用
	void Rev();
	void Resize( MINT szn);
	MgMinMaxR3 MinMax();
	void Print( MCHAR* s) const;												// Print
};

//======================( ２次元 )==============================
//		多角形群
//
class DLL_EXPORT_POLYGON MgGPolyg2
{
public:
	MUBYTE		m_iTp;							// データタイプ　MGTP_GPOLYG
	MUBYTE		m_fa;							// アロケーションフラグ(0:配列, 1:malloc)
												// 		=1の場合は使用終了時にfreeで領域の開放を必要とする
	MSHORT		m_isz;							// 最大ポリゴン+穴数数
	MSHORT		m_n;							// ポリゴン+穴数
	MgPolyg2*	m_pg;							// ポリゴンおよび穴
public:
	MgGPolyg2()			{ m_iTp = MGTP_GPOLYG; m_isz = m_n = m_fa = 0; m_pg = NULL;}
	MgGPolyg2( MINT sz);
	MgGPolyg2( MINT sz1, MgPolyg2 *pg2);
	~MgGPolyg2();
//
	void Free();																// 領域開放
//
	const MgPolyg2& operator () ( MINT idx) const;
	MgPolyg2& operator () ( MINT idx);
	const MgPolyg2& operator [] ( MINT idx) const								// ()のチェック機能と領域拡張機能を取り外し高速版	
						{ return m_pg[idx];}
	MgPolyg2& operator [] ( MINT idx)											// ()のチェック機能と領域拡張機能を取り外し高速版	
						{ return m_pg[idx];}
	void operator = (const MgGPolyg2 &gpg);
	void operator = (const MgGPolyg3 &gPg);										// ３次元→２次元
	void operator += (const MgPolyg2 &pg);										// GPg1 += Pg2
	void operator += (const MgGPolyg2 &gpg);									// GPg1 += GPg2
	void operator -= (const MINT dn)
						{ int ict; for ( ict=0; ict<dn && m_n>0; ict--) { m_n--; if (m_pg[m_n].m_fa) MBFREE( m_pg[m_n].m_p);}}
	void operator -- ()
						{ if (m_n>0) { --m_n; if (m_pg[m_n].m_fa) MBFREE( m_pg[m_n].m_p);}}
	void operator -- (int)
						{ if (m_n>0) { m_n--; if (m_pg[m_n].m_fa) MBFREE( m_pg[m_n].m_p);}}
//
	void SetCount0()
						{ Free();}
	void Set( const MgGPolyg3 &GPg);											// ３次元→２次元代入用
	void AddPolyg();
	void Rev();
	void Resize( MINT szn);
	MgMinMaxR2 MinMax();
	void Print( MCHAR* s) const;												// Print
};

//======================( ３次元 )==============================
//		多角形群
//
class DLL_EXPORT_POLYGON MgGPolyg3
{
public:
	MUBYTE		m_iTp;							// データタイプ　MGTP_GPOLYG3
	MUBYTE		m_fa;							// アロケーションフラグ(0:配列, 1:malloc)
												// 		=1の場合は使用終了時にfreeで領域の開放を必要とする
	MSHORT		m_isz;							// 最大ポリゴン+穴数数
	MSHORT		m_n;							// ポリゴン+穴数
	MgPolyg3*	m_Pg;							// ポリゴンおよび穴
public:
	MgGPolyg3()	
						{ m_iTp = MGTP_GPOLYG3; m_isz = m_n = m_fa = 0; m_Pg = NULL;}
	MgGPolyg3( MINT sz1);
	MgGPolyg3( MINT sz1, MgPolyg3 *Pg2);
	~MgGPolyg3();
//
	void Free();																// 領域開放
//
	const MgPolyg3& operator () ( MINT idx) const; 
	MgPolyg3& operator () ( MINT idx);
	const MgPolyg3& operator [] ( MINT idx) const								// ()のチェック機能と領域拡張機能を取り外し高速版	
						{ return m_Pg[idx];}
	MgPolyg3& operator [] ( MINT idx)											// ()のチェック機能と領域拡張機能を取り外し高速版	
						{ return m_Pg[idx];}
	void operator = (const MgGPolyg3 &GPg);
	void operator = (const MgGPolyg2 &Gpg);										// ２次元→３次元 (z = 0.)
	void operator += (const MgPolyg3 &Pg);										// GPg1 += Pg2
	void operator += (const MgGPolyg3 &GPg);									// GPg1 += GPg2
	void operator -= (const MINT dn)
						{ int ict; for ( ict=0; ict<dn && m_n>0; ict--) { m_n--; if ( m_Pg[m_n].m_fa) MBFREE( m_Pg[m_n].m_P);}}
	void operator -- ()
						{ if ( m_n>0) { --m_n; if ( m_Pg[m_n].m_fa) MBFREE( m_Pg[m_n].m_P);}}
	void operator -- (int)
						{ if ( m_n>0) { m_n--; if ( m_Pg[m_n].m_fa) MBFREE( m_Pg[m_n].m_P);}}
//
	void SetCount0()
						{ Free();}
	void Set( const MgGPolyg2 &Gpg, MREAL z = 0.);								// ２次元→３次元代入用 (z = 0.)
	void Set( const MgGPolyg2 &Gpg, const MgPlane3& Pln);						// ２次元→３次元代入用
	void AddPolyg();
	void Rev();
	void Resize( MINT szn);
	MgMinMaxR3 MinMax();
	void Print( MCHAR* s) const;												// Print
};

//======================( ２次元 )==============================
//		線分群
//
class DLL_EXPORT_POLYGON MgGLine2
{
public:
	MUBYTE		m_iTp;							// データタイプ　MGTP_GLINE
	MUBYTE		m_fa;							// アロケーションフラグ(0:配列, 1:malloc)
												// 		=1の場合は使用終了時にfreeで領域の開放を必要とする
	MSHORT		m_isz;							// 最大線分数
	MSHORT		m_n;							// 線分数
	MgLine2*	m_ln;							// 線分
public:
	MgGLine2()			{ m_iTp = MGTP_GLINE; m_isz = m_n = m_fa = 0; m_ln = NULL;}
	MgGLine2( MINT sz);
	MgGLine2( MINT sz1, MgLine2 *ln2);
	~MgGLine2()			{ if ( m_fa) MBFREE( m_ln);}							// 領域開放
//
	void Free()			{ if ( m_fa) MBFREE( m_ln); m_isz = m_n = m_fa = 0;}		// 領域開放
//
	const MgLine2& operator () (MINT idx) const;
	MgLine2& operator () ( MINT idx);
	const MgLine2& operator [] (MINT idx) const									// ()のチェック機能と領域拡張機能を取り外し高速版	
						{ return m_ln[idx];}
	MgLine2& operator [] (MINT idx)												// ()のチェック機能と領域拡張機能を取り外し高速版	
						{ return m_ln[idx];}
	void operator = (const MgGLine2 &Gln);
	void operator = (const MgGLine3 &GLn);										// ３次元→２次元
	void operator += (const MgLine2& ln);										// GLn1 += ln2
	void operator += (const MgGLine2 &Gln);										// GLn1 += GLn2
	void operator -= (const MINT dn)
						{ m_n = max( m_n -= dn, 0);}
	void operator -- ()
						{ if ( m_n>0) -- m_n;}
	void operator -- (int)
						{ if ( m_n>0) m_n --;}
//
	void SetInit()		{ m_isz = m_n = m_fa = 0; m_ln = NULL;}					// この群がさらに群になった場合の最終データより後のデータのクリア用
	void Set( const MgGLine3 &GLn);												// ３次元→２次元代入用
	void Rev();
	void Resize( MINT szn);
	MgMinMaxR2 MinMax();
	void Print( MCHAR* s) const;												// Print
};

MINT DLL_EXPORT_POLYGON MgMergeGLine2( const MgGLine2 &gln1);							// 線分の重なりをまとめ一本にする
MINT DLL_EXPORT_POLYGON MgCutGLine2( const MgGLine2 &gln1);								// 線分を交差部で分割する

//======================( ３次元 )==============================
//		線分群
//
class DLL_EXPORT_POLYGON MgGLine3
{
public:
	MUBYTE		m_iTp;						// データタイプ　MGTP_GLINE3
	MUBYTE		m_fa;						// アロケーションフラグ(0:配列, 1:malloc)
											// 		=1の場合は使用終了時にfreeで領域の開放を必要とする
	MSHORT		m_isz;						// 最大線分数
	MSHORT		m_n;						// 線分数
	MgLine3*	m_ln;							// 線分
public:
	MgGLine3()			{ m_iTp = MGTP_GLINE3; m_isz = m_n = m_fa = 0; m_ln = NULL;}
	MgGLine3( MINT sz);
	MgGLine3( MINT sz1, MgLine3 *Ln3);
	~MgGLine3()			{ if ( m_fa) MBFREE( m_ln);}							// 領域開放
//
	void Free()			{ if ( m_fa) MBFREE( m_ln); m_isz = m_n = m_fa = 0;}		// 領域開放
//
	const MgLine3& operator () ( MINT idx) const;
	MgLine3& operator () ( MINT idx);
	const MgLine3& operator [] ( MINT idx) const								// ()のチェック機能と領域拡張機能を取り外し高速版	
						{ return m_ln[idx];}
	MgLine3& operator [] ( MINT idx)											// ()のチェック機能と領域拡張機能を取り外し高速版	
						{ return m_ln[idx];}
	void operator = (const MgGLine3 &GLn);
	void operator = (const MgGLine2 &Gln);										// ２次元→３次元 (z = 0.)
	void operator += (const MgLine3& Ln);										// GLn1 += Ln2
	void operator += (const MgGLine3 &GLn);										// GLn1 += GLn2
	void operator -= (const MINT dn)
						{ m_n = max( m_n -= dn, 0);}
	void operator -- ()
						{ if ( m_n>0) -- m_n;}
	void operator -- (int)
						{ if ( m_n>0) m_n --;}
//
	void SetInit()		{ m_isz = m_n = m_fa = 0; m_ln = NULL;}					//この群がさらに群になった場合の最終データより後のデータのクリア用
	void Set( const MgGLine2 &Gln2, const MgPlane3& Pln);						// ２次元→３次元代入用
	void Set( const MgGLine2 &Gln2, MREAL z = 0.);								// ２次元→３次元代入用 (z = 0.)
	void Rev();
	void Resize( MINT szn);
	MgMinMaxR3 MinMax();
	void Print( MCHAR* s) const;												// Print
};

//======================( １次元 )==============================
//		整数群
//
class DLL_EXPORT_POLYGON MgGInt
{
public:
	MUBYTE		m_iTp;							// データタイプ　MGTP_GINT
	MUBYTE		m_fa;							// アロケーションフラグ(0:配列, 1:malloc)
												// 		=1の場合は使用終了時にfreeで領域の開放を必要とする
	MSHORT		m_isz;							// 最大整数数
	MSHORT		m_n;							// 整数数
	MINT*		m_i;							// 整数
public:
	MgGInt()			{ m_iTp = MGTP_GINT; m_isz = m_n = m_fa = 0; m_i = NULL;}
	MgGInt( MINT sz);
	MgGInt( MINT sz1, MINT *i2);
	~MgGInt()			{ if ( m_fa) MBFREE( m_i);}								// 領域開放
//
	void Init()			{ m_iTp = MGTP_GINT; m_isz = m_n = m_fa = 0; m_i = NULL;}
	void Free()	
						{
							if ( m_fa) 
								MBFREE( m_i);
							m_isz = m_n = m_fa = 0;
							m_i = NULL;
						}														// 領域開放
//
	const MINT& operator () ( MINT idx) const;
	MINT& operator () ( MINT idx);
	const MINT& operator [] ( MINT idx) const									// ()のチェック機能と領域拡張機能を取り外し高速版	
						{ return m_i[idx];}
	MINT& operator [] ( MINT idx)												// ()のチェック機能と領域拡張機能を取り外し高速版	
						{ return m_i[idx];}
	void operator = (const MgGInt &GI);
	void operator = (const MgGReal &GR);										// 実数群 → 整数群
	void operator += (const MINT &i);											// GPt1 += i2
	void operator += (const MgGInt &GI);										// GPt1 += GI2
	void operator -= (const MINT dn)
						{ m_n = max( m_n -= dn, 0);}
	void operator -- ()
						{ if ( m_n>0) -- m_n;}
	void operator -- (int)
						{ if ( m_n>0) m_n --;}
//
	void SetInit()		{ m_isz = m_n = m_fa = 0; m_i = NULL;}					//この群がさらに群になった場合の最終データより後のデータのクリア用
	void Set( const MgGReal &GR);												// 実数群 → 整数群代入用
	void Rev();
	void Resize( MINT szn);
	void Print( MCHAR* s) const;												// Print
};

//======================( １次元 )==============================
//		実数群
//
class DLL_EXPORT_POLYGON MgGReal
{
public:
	MUBYTE		m_iTp;							// データタイプ　MGTP_GREAL
	MUBYTE		m_fa;							// アロケーションフラグ(0:配列, 1:malloc)
												// 		=1の場合は使用終了時にfreeで領域の開放を必要とする
	MSHORT		m_isz;							// 最大実数数
	MSHORT		m_n;							// 実数数
	MREAL*		m_r;							// 実数
public:
	MgGReal()			{ m_iTp = MGTP_GREAL; m_isz = m_n = m_fa = 0; m_r = NULL;}
	MgGReal( MINT sz);
	MgGReal( MINT sz1, MREAL* r2);
	~MgGReal()			{ if ( m_fa) MBFREE( m_r);}								// 領域開放
//
	void Free()			{ if ( m_fa) MBFREE( m_r);	m_isz = m_n = m_fa = 0;}		// 領域開放
//
	const MREAL& operator () ( MINT idx) const;
	MREAL& operator () ( MINT idx);
	const MREAL& operator [] ( MINT idx) const									// ()のチェック機能と領域拡張機能を取り外し高速版	
						{ return m_r[idx];}
	MREAL& operator [] ( MINT idx)												// ()のチェック機能と領域拡張機能を取り外し高速版	
						{ return m_r[idx];}
	void operator = (const MgGReal &GR);
	void operator = (const MgGInt &GI);											// 整数群 → 実数群
	void operator += (const MREAL &r);											// GPt1 += r2
	void operator += (const MgGReal &GR);										// GPt1 += GR2
	void operator -= (const MINT dn)
						{ m_n = max( m_n -= dn, 0);}
	void operator -- ()
						{ if ( m_n>0) -- m_n;}
	void operator -- (int)
						{ if ( m_n>0) m_n --;}
//
	void SetInit()		{ m_isz = m_n = m_fa = 0; m_r = NULL;}					//この群がさらに群になった場合の最終データより後のデータのクリア用
	void Set( const  MgGInt &GI);												// 整数群 → 実数群代入用
	void Rev();
	void Resize( MINT szn);
	void Print( MCHAR* s) const;												// Print
};

//======================( ２次元 )==============================
//		点群
//
class DLL_EXPORT_POLYGON MgGPoint2
{
public:
	MUBYTE		m_iTp;							// データタイプ　MGTP_GPOINT2
	MUBYTE		m_fa;							// アロケーションフラグ(0:配列, 1:malloc)
												// 		=1の場合は使用終了時にfreeで領域の開放を必要とする
	MSHORT		m_isz;							// 最大点数
	MSHORT		m_n;							// 点数
	MgPoint2*	m_p;							// 点
public:
	MgGPoint2()			{ m_iTp = MGTP_GPOINT2; m_isz = m_n = m_fa = 0; m_p = NULL;}
	MgGPoint2( MINT sz);
	MgGPoint2( MINT sz1, MgPoint2 *pt2);
	~MgGPoint2()		{ if ( m_fa) MBFREE( m_p);}								// 領域開放
//
	void Free()			{ if ( m_fa) MBFREE( m_p);  m_isz = m_n = m_fa = 0;}		// 領域開放
//
	const MgPoint2& operator () ( MINT idx) const;
	MgPoint2& operator () ( MINT idx);
	const MgPoint2& operator [] ( MINT idx) const								// ()のチェック機能と領域拡張機能を取り外し高速版	
						{ return m_p[idx];}
	MgPoint2& operator [] ( MINT idx)											// ()のチェック機能と領域拡張機能を取り外し高速版	
						{ return m_p[idx];}
	void operator = (const MgGPoint2 &Gpt);
	void operator = (const MgGPoint3 &GPt);										// ３次元→２次元
	void operator += (const MgPoint2& pt);										// GPt1 += Pt2
	void operator += (const MgGPoint2 &Gpt);									// GPt1 += GPt2
	void operator -= (const MINT dn)
						{ m_n = max( m_n -= dn, 0);}
	void operator -- ()
						{ if ( m_n>0) -- m_n;}
	void operator -- (int)
						{ if ( m_n>0) m_n --;}
//
	void SetInit()		{ m_isz = m_n = m_fa = 0; m_p = NULL;}							// この群がさらに群になった場合の最終データより後のデータのクリア用
	void Set( const MgGPoint3 &GPt);											// ３次元→２次元代入用
	void Rev();
	void Resize( MINT szn);
	MgMinMaxR2 MinMax();
	void Print( MCHAR* s) const;												// Print
};

//======================( ３次元 )==============================
//		点群
//
class DLL_EXPORT_POLYGON MgGPoint3
{
public:
	MUBYTE		m_iTp;							// データタイプ　MGTP_GPOINT3
	MUBYTE		m_fa;							// アロケーションフラグ(0:配列, 1:malloc)
												// 		=1の場合は使用終了時にfreeで領域の開放を必要とする
	MSHORT		m_isz;							// 最大点数
	MSHORT		m_n;							// 点数
	MgPoint3*	m_p;							// 点
public:
	MgGPoint3()			{ m_iTp = MGTP_GPOINT3; m_isz = m_n = m_fa = 0; m_p = NULL;}
	MgGPoint3( MINT sz);
	MgGPoint3( MINT sz1, MgPoint3 *pt2);
	~MgGPoint3()		{ if ( m_fa) MBFREE( m_p);}								// 領域開放
//
	void Free()			{ if ( m_fa) MBFREE( m_p);  m_isz = m_n = m_fa = 0;}		// 領域開放
//
	const MgPoint3& operator () ( MINT idx) const;
	MgPoint3& operator () ( MINT idx);
	const MgPoint3& operator [] ( MINT idx) const								// ()のチェック機能と領域拡張機能を取り外し高速版	
						{ return m_p[idx];}
	MgPoint3& operator [] ( MINT idx)											// ()のチェック機能と領域拡張機能を取り外し高速版	
						{ return m_p[idx];}
	void operator = (const MgGPoint3 &Pt);
	void operator = (const MgGPoint2 &Gpt);										// ２次元→３次元 (z = 0.)	
	void operator += (const MgPoint3& Pt);										// GPt1 += Pt2
	void operator += (const MgGPoint3 &GPt);									// GPt1 += GPt2
	void operator -= (const MINT dn)
						{ m_n = max( m_n -= dn, 0);}
	void operator -- ()
						{ if ( m_n>0) -- m_n;}
	void operator -- (int)
						{ if ( m_n>0) m_n --;}
//
	void SetInit()		{ m_isz = m_n = m_fa = 0; m_p = NULL;}					//この群がさらに群になった場合の最終データより後のデータのクリア用
	void Set( const MgGPoint2 &Gpt, const MgPlane3& Pln);						// ２次元→３次元代入用
	void Set( const MgGPoint2 &Gpt, MREAL z = 0.);								// ２次元→３次元代入用 (z = 0.)
	void Rev();
	void Resize( MINT szn);
	MgMinMaxR3 MinMax();
	void Print( MCHAR* s) const;												// Print
};

} // namespace MC
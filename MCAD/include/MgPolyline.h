#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgPolyline.h
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
//	MgPolyl2は２次元の折れ線を、MgPolyl3は３次元の折れ線を定義する。
//	MgGPolyl2は２次元の折れ線群を、MgGPolyl3は３次元の折れ線群を定義する。
//	MgGLine2は２次元の線分群を、MgGPolyl3は３次元の線分群を定義する。
//	MgGIntは整数群を、MgGRealは実数群を定義する。
//	MgGPoint2は２次元の点群を、MgGPoint3は３次元の点群を定義する。
//	オーバレイ関数を定義する。
//
#include "MgPlane.h"
#include "MgRect.h"

#ifdef DLL_EXPORT_POLILINE
	#undef DLL_EXPORT_POLILINE
#endif

#ifdef DLL_EXPORT_POLYLINE_DO
	#pragma message( "<<< MgPolyline・dll_EXport >>>")
	#define DLL_EXPORT_POLILINE							__declspec( dllexport)
#else
	#ifdef _DEBUG								// Release Mode で inline関数がundefになる不具合対応
		#ifdef DLL_NO_IMPORT_GEO_DO
			#define DLL_EXPORT_POLILINE
		#else
//			#pragma message( "=== MgPolyline・dll_IMport ===")
			#define DLL_EXPORT_POLILINE						__declspec( dllimport)
		#endif
	#else
		#define DLL_EXPORT_POLILINE
	#endif
#endif

namespace MC
{

//class MgGPoint2;
//class MgGPoint3;
//class MgGPolyl2;
//class MgGPolyl3;
//class MgGLine2;
//class MgGLine3;
//class MgGReal;
class MgPolyl3;
class MgGPolyl3;
//
//======================( ２次元 )==============================
//		折れ線
class DLL_EXPORT_POLILINE MgPolyl2						// ２Ｄポリゴン
{
public:
	MUBYTE		m_tp;							// データタイプ　MGTP_POLYG
	MUBYTE		m_fa;							// アロケーションフラグ(0:配列, 1:malloc)
												// 		=1の場合は使用終了時にfreeで領域の開放を必要とする
	MSHORT		m_isz;							// 最大頂点数
	MSHORT		m_n;							// 頂点数
	MgPoint2* 	m_p;
public:
	MgPolyl2()			{ m_tp = MGTP_POLYG; m_isz = m_n = m_fa = 0; m_p = NULL;}
	MgPolyl2( MINT sz);
	MgPolyl2( MINT sz1, MgPoint2 *p2);
	~MgPolyl2()			{ if ( m_fa) MBFREE( m_p);}									// 領域開放
//
	void Init()			{ m_tp = MGTP_POLYG; m_isz = m_n = m_fa = 0; m_p = NULL;}	// 領域初期化
	void Free()			{ if ( m_fa) MBFREE( m_p); m_isz = m_n = m_fa = 0;}			// 領域開放
//	
	const MgPoint2& operator () ( MINT idx) const;
	MgPoint2& operator () ( MINT idx);
	const MgPoint2& operator [] ( MINT idx) const								// ()のチェック機能と領域拡張機能を取り外し高速版	
						{ return m_p[idx];}
	MgPoint2& operator [] ( MINT idx)											// ()のチェック機能と領域拡張機能を取り外し高速版	
						{ return m_p[idx];}
	void operator = (const MgPolyl2 &pg);
	void operator = (const MgPolyl3 &Pg);										// ３次元→２次元
	void operator += (const MgPoint2& pt);										// ポリゴンに点を追加		Pg1 += Pt2
	void operator += (const MgPolyl2 &pg);										// ポリゴンにポリゴンを追加	Pg1 += Pg2
	void operator -= (const MINT dn)											// ポリゴンの最終ｎ点を削除
						{ m_n = max( m_n -= dn, 0);}
	void operator -- ()															// ポリゴンの最終点を削除
						{ if ( m_n>0) -- m_n;}
	void operator -- (int)														// ポリゴンの最終点を削除
						{ if ( m_n>0) m_n --;}
//
	void SetInit()		{ m_isz = m_n = m_fa = 0; m_p = NULL;}					// この群がさらに群になった場合の最終データより後のデータのクリア用
	void Set( const MgPolyl3 &Pg);												// ３次元→２次元代入用
	MINT Rotation() const;
	void Rev();
	void Resize( MINT szn);
	MREAL Area();
	MgMinMaxR2 MinMax();
	void Print( MCHAR* s) const;												// Print
};
//
//======================( ３次元 )==============================
//		折れ線
//
class DLL_EXPORT_POLILINE MgPolyl3						// ３Ｄポリゴン
{
public:
	MUBYTE		m_tp;							// データタイプ　MGTP_POLYG3
	MUBYTE		m_fa;							// アロケーションフラグ(0:配列, 1:malloc)
												// 		=1の場合は使用終了時にfreeで領域の開放を必要とする
	MSHORT		m_isz;							// 最大頂点数
	MSHORT		m_n;							// 頂点数
	MgPoint3*	m_p;
public:
	MgPolyl3()			{ m_tp = MGTP_POLYG3; m_isz = m_n = m_fa = 0; m_p = NULL;}
	MgPolyl3( MINT sz1);
	MgPolyl3( MINT sz1, MgPoint3 *p2);
	~MgPolyl3()			{ if ( m_fa) MBFREE( m_p);}								// 領域開放
//
	void Init()			{ m_tp = MGTP_POLYG; m_isz = m_n = m_fa = 0; m_p = NULL;}// 領域初期化
	void Free()	
				{
					if ( m_fa)
						MBFREE( m_p);
					m_isz = m_n = m_fa = 0;
				}					// 領域開放
//
	const MgPoint3& operator () ( MINT idx) const;
	MgPoint3& operator () ( MINT idx);
	const MgPoint3& operator [] ( MINT idx) const								// ()のチェック機能と領域拡張機能を取り外し高速版	
						{ return m_p[idx];}
	MgPoint3& operator [] ( MINT idx)											// ()のチェック機能と領域拡張機能を取り外し高速版	
						{ return m_p[idx];}
	void operator = (const MgPolyl3 &Pg);
	void operator = (const MgPolyl2 &pg);										// ２次元→３次元 (z = 0.)
//						{ *this = MgPolyl3( pg);}
	void operator += (const MgPoint3& Pt);										// Pg1 += Pt2
	void operator += (const MgPolyl3 &Pg);										// Pg1 += Pg2
	void operator -= (const MINT dn)
						{ m_n = max( m_n -= dn, 0);}
	void operator -- ()
						{ if ( m_n>0) -- m_n;}
	void operator -- (int)
						{ if ( m_n>0) m_n --;}
//
	void SetInit()		{ m_isz = m_n = m_fa = 0; m_p = NULL;}					//この群がさらに群になった場合の最終データより後のデータのクリア用
	void Set( const MgPolyl2 &pg, MREAL z = 0.);								// ２次元→３次元代入用 (z = 0.)
	void Set( const MgPolyl2 &pg, const MgPlane3& Ply);							// ２次元→３次元代入用
	void Rev();
	void Resize( MINT szn);
	MgMinMaxR3 MinMax();
	void Print( MCHAR* s) const;												// Print
};

//
//======================( ２次元 )==============================
//		折れ線群
//
class DLL_EXPORT_POLILINE MgGPolyl2
{
public:
	MUBYTE		m_tp;							// データタイプ　MGTP_GPOLYG
	MUBYTE		m_fa;							// アロケーションフラグ(0:配列, 1:malloc)
												// 		=1の場合は使用終了時にfreeで領域の開放を必要とする
	MSHORT		m_isz;							// 最大ポリゴン+穴数数
	MSHORT		m_n;							// ポリゴン+穴数
	MgPolyl2*	m_pg;							// ポリゴンおよび穴
public:
	MgGPolyl2()			{ m_tp = MGTP_GPOLYG; m_isz = m_n = m_fa = 0; m_pg = NULL;}
	MgGPolyl2( MINT sz);
	MgGPolyl2( MINT sz1, MgPolyl2 *pg2);
	~MgGPolyl2();
//
	void Free();																// 領域開放
//
	const MgPolyl2& operator () ( MINT idx) const;
	MgPolyl2& operator () ( MINT idx);
	const MgPolyl2& operator [] ( MINT idx) const								// ()のチェック機能と領域拡張機能を取り外し高速版	
						{ return m_pg[idx];}
	MgPolyl2& operator [] ( MINT idx)											// ()のチェック機能と領域拡張機能を取り外し高速版	
						{ return m_pg[idx];}
	void operator = (const MgGPolyl2 &Gpg);
	void operator = (const MgGPolyl3 &GPg);										// ３次元→２次元
	void operator += (const MgPolyl2 &pg);										// GPg1 += Pg2
	void operator += (const MgGPolyl2 &Gpg);									// GPg1 += GPg2
	void operator -= (const MINT dn)
						{ int ict; for ( ict=0; ict<dn && m_n>0; ict--) { m_n--; if (m_pg[m_n].m_fa) MBFREE( m_pg[m_n].m_p);}}
	void operator -- ()
						{ if (m_n>0) { --m_n; if (m_pg[m_n].m_fa) MBFREE( m_pg[m_n].m_p);}}
	void operator -- (int)
						{ if (m_n>0) { m_n--; if (m_pg[m_n].m_fa) MBFREE( m_pg[m_n].m_p);}}
//
	void SetCount0()
						{ Free();}
	void Set( const MgGPolyl3 &GPg);											// ３次元→２次元代入用
	void AddPolyg();
	void Rev();
	void Resize( MINT szn);
	MgMinMaxR2 MinMax();
	void Print( MCHAR* s) const;												// Print
};

//
//======================( ３次元 )==============================
//		折れ線群
//
class DLL_EXPORT_POLILINE MgGPolyl3
{
public:
	MUBYTE		m_tp;							// データタイプ　MGTP_GPOLYG3
	MUBYTE		m_fa;							// アロケーションフラグ(0:配列, 1:malloc)
												// 		=1の場合は使用終了時にfreeで領域の開放を必要とする
	MSHORT		m_isz;							// 最大ポリゴン+穴数数
	MSHORT		m_n;							// ポリゴン+穴数
	MgPolyl3*	m_pg;							// ポリゴンおよび穴
public:
	MgGPolyl3()	
						{ m_tp = MGTP_GPOLYG3; m_isz = m_n = m_fa = 0; m_pg = NULL;}
	MgGPolyl3( MINT sz1);
	MgGPolyl3( MINT sz1, MgPolyl3 *Pg2);
	~MgGPolyl3();
//
	void Free();																// 領域開放
//
	const MgPolyl3& operator () ( MINT idx) const; 
	MgPolyl3& operator () ( MINT idx);
	const MgPolyl3& operator [] ( MINT idx) const								// ()のチェック機能と領域拡張機能を取り外し高速版	
						{ return m_pg[idx];}
	MgPolyl3& operator [] ( MINT idx)											// ()のチェック機能と領域拡張機能を取り外し高速版	
						{ return m_pg[idx];}
	void operator = (const MgGPolyl3 &GPg);
	void operator = (const MgGPolyl2 &Gpg);										// ２次元→３次元 (z = 0.)
	void operator += (const MgPolyl3 &Pg);										// GPg1 += Pg2
	void operator += (const MgGPolyl3 &GPg);									// GPg1 += GPg2
	void operator -= (const MINT dn)
						{ int ict; for ( ict=0; ict<dn && m_n>0; ict--) { m_n--; if ( m_pg[m_n].m_fa) MBFREE( m_pg[m_n].m_p);}}
	void operator -- ()
						{ if ( m_n>0) { --m_n; if ( m_pg[m_n].m_fa) MBFREE( m_pg[m_n].m_p);}}
	void operator -- (int)
						{ if ( m_n>0) { m_n--; if ( m_pg[m_n].m_fa) MBFREE( m_pg[m_n].m_p);}}
//
	void SetCount0()
						{ Free();}
	void Set( const MgGPolyl2 &Gpg, MREAL z = 0.);								// ２次元→３次元代入用 (z = 0.)
	void Set( const MgGPolyl2 &Gpg, const MgPlane3& Pln);						// ２次元→３次元代入用
	void AddPolyg();
	void Rev();
	void Resize( MINT szn);
	MgMinMaxR3 MinMax();
	void Print( MCHAR* s) const;												// Print
};

} // namespace MC
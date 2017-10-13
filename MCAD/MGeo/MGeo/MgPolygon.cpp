//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgPolygon.cpp
//		
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"
#include "MbDebug.h"

#define	DLL_EXPORT_POLYGON_DO

#include "MgDefine.h"
#include "MgTol.h"
#include "MgDefine.h"
#include "MgPlane.h"
#include "MgPolygon.h"

namespace MC
{

//
//======================( ２次元 )==============================
//		多角形（点群）
//
//	構成点の領域をmallocで確保する場合のコンストラクタ
MgPolyg2::MgPolyg2( MINT sz1)
{
	m_iTp = MGTP_POLYG;
	m_isz = sz1;
	m_n = 0;
	m_fa = 1;
	MBMALLOC( m_p, MgPoint2, sz1);
}

//	構成点の領域を配列で与える場合のコンストラクタ
MgPolyg2::MgPolyg2( MINT sz1, MgPoint2 *p2)
{
	m_iTp = MGTP_POLYG;
	m_isz = sz1;
	m_n = 0;
	m_fa = 0;
	m_p = p2;
}

//	構成点を()で指定して読み書きする
const MgPoint2& MgPolyg2::operator () ( MINT idx) const
{
	if ( idx >= m_n)
		RETURN_OVERFLOW_ERR( "MgPolyg2 ()");
	return m_p[idx];
}

MgPoint2& MgPolyg2::operator () ( MINT idx)
{
	if ( idx >= m_isz)
		RETURN_OVERFLOW_ERR( "MgPolyg2 ()");
	return m_p[idx];
}

//	複写
void MgPolyg2::operator = ( const MgPolyg2& Pg)
{
	MINT	nn = Pg.m_n;
	if ( nn > m_isz) {
		if ( m_fa) MBFREE( m_p);								//	構成点の領域をmallocで確保していた場合は一旦free
		m_isz = nn;
		m_fa = 1;
		MBMALLOC( m_p, MgPoint2, nn);							//	構成点の領域をmallocで確保
	}
	m_n = nn;
	memcpy( m_p, Pg.m_p, sizeof( MgPoint2) * nn);
}

void MgPolyg2::operator = ( const MgPolyg3& Pg)
{
	MINT	ic1;
	MINT	nn = Pg.m_n;
	if ( nn > m_isz) {
		if ( m_fa) MBFREE( m_p);								//	構成点の領域をmallocで確保していた場合は一旦free
		m_isz = nn;
		m_fa = 1;
		MBMALLOC( m_p, MgPoint2, nn);							//	構成点の領域をmallocで確保
	}
	m_n = nn;
	for ( ic1 = 0; ic1<nn; ic1++)
		m_p[ic1] = MgPoint2C( Pg.m_P[ic1]);  
}

//	構成点の追加
void MgPolyg2::operator += ( const MgPoint2& p1)				// Pg += Pt1
{
	MgPoint2	p1sv;
	if ( m_n >= m_isz) {
//		p1sv = p1;												// ソースがリサイズする元の領域にあるデータの場合を考慮
		Resize( m_isz + MGMAX( (MINT)m_isz, 4));						// ２倍に領域拡張(拡張後の最低領域サイズは４とする)
//		m_p[n++] = p1sv;
		m_p[m_n++] = p1;
	} else {
		m_p[m_n++] = p1;
	}
}

//	多角形（点群）の追加
void MgPolyg2::operator += ( const MgPolyg2 &pg)				// Pg += Pt1
{
	MINT	ic;
	MINT	nn = m_n + pg.m_n;
	if ( nn >= m_isz)
		Resize( nn);
	for ( ic=0; ic<pg.m_n; ic++)
		m_p[m_n++] = pg.m_p[ic];
}

//	３次元の多角形より２次元の多角形を作成
void MgPolyg2::Set( const MgPolyg3 &Pg)
{
	MINT	ic1;
	MINT	nn = Pg.m_n;
	if ( nn > m_isz) {
		if ( m_fa) MBFREE( m_p);								//	構成点の領域をmallocで確保していた場合は一旦free
		m_isz = nn;
		m_fa = 1;
		MBMALLOC( m_p, MgPoint2, nn);							//	構成点の領域をmallocで確保
	}
	m_n = nn;
	for ( ic1 = 0; ic1<nn; ic1++)
		m_p[ic1] = MgPoint2C( Pg.m_P[ic1]);  
}

//	回転方向を得る(MC_LEFT_ROT:左回転, MC_RIGHT_ROT:右回転)
MINT MgPolyg2::Rotation() const
{
	MINT	ist = 0;
	MINT	ic;
	MgPoint2	Pb;
	MgPoint2	Pc;
//
	MINT	irot = 0;
	MREAL	s = 0.;
	Pb = m_p[m_n-1];
	for ( ic=0; ic<m_n; ic++) {
		Pc = m_p[ic];											// 原点から各頂点へのベクトルの外積の合計を求め
		s += (Pb ^ Pc);											// 回転方向を考慮した多角形の面積*2を求める
		Pb = Pc;
	}
	if (s > g_gTol.D_2) {
		ist = MC_LEFT_ROT;
	} else if (s < -g_gTol.D_2) {
		ist = MC_RIGHT_ROT;
	}							
	return ist;
}

//	多角形の回転方向の反転
void MgPolyg2::Rev()
{
	MINT	ic1, ic2;
	ic2 = m_n - 1;
	for ( ic1=0; ic1<ic2; ic1++) {
		MSWAP( m_p[ic1], m_p[ic2], MgPoint2);
		ic2--;
	}
}

//	多角形領域の拡張
void MgPolyg2::Resize( MINT szn)
{
	//if (szn > sz) {
	//	if (fa && p) {											//	構成点の領域をmallocで確保している場合
	//		MGREALLOC( p, MgPoint2, szn);						//		構成点の領域をremallocで拡張
	//	} else {												//	構成点の領域を未確保または配列の場合
	//		MgPoint2		*pb = p;
	//		MBMALLOC( p, MgPoint2, szn);  						//		構成点の領域をmallocで確保
	//		if (!fa) memcpy( p, pb, sizeof( MgPoint2) * n);		//	構成点の領域が配列であった場合はmalloc領域へコピーする
	//		fa = 1;
	//	}
	//	sz = szn;
	//}

	if (szn > m_isz) {
		MgPoint2*	pb = m_p;
		MBMALLOC( m_p, MgPoint2, szn);		 					//	構成点の新領域をmallocで確保
		if ( m_n)
			 memcpy( m_p, pb, m_n * sizeof( MgPoint2));			//	構成点の領域にデータがあった場合は新領域へコピーする
		if ( m_fa && pb)										//	構成点の領域をmallocで確保していた場合はその領域を開放する
			delete (char*)pb;									//	
		m_fa = 1;
		m_isz = szn;
	}
}

//	多角形領域の面積
MREAL MgPolyg2::Area()
{
	MINT	ic1;
	MgVect2	v1, v2;
	MREAL	rArea = 0;

	for ( ic1=2; ic1<m_n; ic1++) {
		v1 = m_p[ic1-1] - m_p[0];
		v2 = m_p[ic1] - m_p[0];
		rArea += (v1 ^ v2);
	}
	return rArea / 2;
}

//	最大最小を求める
MgMinMaxR2 MgPolyg2::MinMax()
{
	MINT	ic1;
	MgMinMaxR2	mm = MgMinMaxR2( MREALMAX, MREALMAX, MREALMIN, MREALMIN);

	for ( ic1=0; ic1<m_n; ic1++) {
		mm.Ins2( m_p[ic1]);
	}
	return mm;
}

//	トレース
void MgPolyg2::Print( MCHAR* s) const
{
#ifdef _DEBUG
	Msprintf( MbLog::m_Str, Mstr( "%s	MgPolyg2	サイズ: %d, 頂点数: %d\n"), s, m_isz, m_n);
	MBTRCPRBF;
	for (int ic=0; ic<m_n; ic++) {
		Msprintf( MbLog::m_Str, Mstr( "			P[%2d]: (%7.1f,%7.1f)\n"),
						  ic, m_p[ic].x, m_p[ic].y);
		MBTRCPRBF;
	}
#endif
}

//
//======================( ３次元 )==============================
//		多角形（点群）
//
//	構成点の領域をmallocで確保する場合のコンストラクタ
MgPolyg3::MgPolyg3( MINT sz1)
{
	m_iTp = MGTP_POLYG3;
	m_isz = sz1;
	m_n = 0;
	m_fa = 1;
	MBMALLOC( m_P, MgPoint3, sz1);
}

//	構成点の領域を配列で与える場合のコンストラクタ
MgPolyg3::MgPolyg3( MINT i_isz1, MgPoint3 *i_pP2)
{
	m_iTp = MGTP_POLYG3;
	m_isz = i_isz1;
	m_n = 0;
	m_fa = 0;
	m_P = i_pP2;
}

//	構成点を()で指定して読み書きする
const MgPoint3& MgPolyg3::operator () (MINT idx) const
{
	if ( idx >= m_isz)
		RETURN_OVERFLOW_ERR( MBCstr( "MgPolyg3 ()"));
	return m_P[idx];
}

MgPoint3& MgPolyg3::operator () (MINT idx)
{
	if ( idx >= m_isz)
		RETURN_OVERFLOW_ERR( MBCstr( "MgPolyg3 ()"));
	return m_P[idx];
}

//	複写
void MgPolyg3::operator = ( const MgPolyg3& Pg)
{
	MINT	nn = Pg.m_n;
	if ( nn > m_isz) {
		if ( m_fa) MBFREE( m_P);								//	構成点の領域をmallocで確保していた場合は一旦free
		m_isz = nn;
		m_fa = 1;
		MBMALLOC( m_P, MgPoint3, nn);							//	構成点の領域をmallocで確保
	}
	memcpy( m_P, Pg.m_P, sizeof( MgPoint3) * nn);
	m_n = nn;
}

void MgPolyg3::operator = (const MgPolyg2 &pg)					// ２次元→３次元 (z = 0.)
{
	MINT	ic1;
	MINT	nn = pg.m_n;
	if ( nn > m_isz) {
		if (m_fa) MBFREE( m_P);									//	構成点の領域をmallocで確保していた場合は一旦free
		m_isz = nn;
		m_fa = 1;
		MBMALLOC( m_P, MgPoint3, nn);							//	構成点の領域をmallocで確保
	}
	m_n = nn;
	for ( ic1 = 0; ic1<nn; ic1++)
		m_P[ic1] = MgPoint3C( pg.m_p[ic1], 0.);  
}

//	構成点の追加
void MgPolyg3::operator += ( const MgPoint3& i_P1)				// Pg += Pt1
{
	MgPoint3	P1sv;
	if (m_n >= m_isz) {
		P1sv = i_P1;												// ソースがリサイズする元の領域にあるデータの場合を考慮
		Resize( m_isz + MGMAX( m_isz, 4));							// ２倍に領域拡張(拡張後の最低領域サイズは４)
		m_P[m_n++] = P1sv;
	} else {
		m_P[m_n++] = i_P1;
	}
}

//	多角形（点群）の追加
void MgPolyg3::operator += ( const MgPolyg3 &i_Pg)				// Pg += Pt1
{
	MINT	ic;
	MINT	nn = m_n + i_Pg.m_n;
	if ( nn >= m_isz)
		Resize( nn);					//
	for ( ic=0; ic<i_Pg.m_n; ic++)
		m_P[m_n++] = i_Pg.m_P[ic];
}

//	２次元の多角形より３次元の多角形を作成
void MgPolyg3::Set( const MgPolyg2 &i_pg, MREAL z)				// ２次元→３次元代入用 (z = 0.)
{
	MINT	ic1;
	MINT	nn = i_pg.m_n;
	if ( nn > m_isz) {
		if ( m_fa) MBFREE( m_P);								//	構成点の領域をmallocで確保していた場合は一旦free
		m_isz = nn;
		m_fa = 1;
		MBMALLOC( m_P, MgPoint3, nn);							//	構成点の領域をmallocで確保
	}
	m_n = nn;
	for ( ic1 = 0; ic1<nn; ic1++)
		m_P[ic1].Set( i_pg.m_p[ic1], z);  
}

void MgPolyg3::Set( const MgPolyg2 &i_pg, const MgPlane3& o_pPly)// ２次元→３次元代入用
{
	MINT	ic1;
	MINT	nn = i_pg.m_n;
	if ( nn > m_isz) {
		if ( m_fa) MBFREE( m_P);								//	構成点の領域をmallocで確保していた場合は一旦free
		m_isz = nn;
		m_fa = 1;
		MBMALLOC( m_P, MgPoint3, nn);							//	構成点の領域をmallocで確保
	}
	m_n = nn;
	for ( ic1 = 0; ic1<nn; ic1++)
		m_P[ic1].Set( i_pg.m_p[ic1], o_pPly);  
}

//	多角形の回転方向の反転
void MgPolyg3::Rev()
{
	MINT	ic1, ic2;
	ic2 = m_n - 1;
	for ( ic1=0; ic1<ic2; ic1++) {
		MSWAP( m_P[ic1], m_P[ic2], MgPoint3);
		ic2--;
	}
}

//	多角形領域の拡張
void MgPolyg3::Resize( MINT szn)
{
	//if (szn > sz) {
	//	if (fa && p) {											//	構成点の領域をmallocで確保している場合
	//		MGREALLOC( p, MgPoint3, szn);						//		構成点の領域をremallocで拡張
	//	} else {												//	構成点の領域を未確保または配列の場合
	//		MgPoint3	*pb = p;
	//		MBMALLOC( p, MgPoint3, szn);		 				//		構成点の領域をmallocで確保
	//		if (!fa) memcpy( p, pb, sizeof( MgPoint3) * n);		//	構成点の領域が配列であった場合はmalloc領域へコピーする
	//		fa = 1;
	//	}
	//	sz = szn;
	//}

	if (szn > m_isz) {
		MgPoint3	*pb = m_P;
		MBMALLOC( m_P, MgPoint3, szn);		 					//	構成点の新領域をmallocで確保
		if ( m_n)
			memcpy( m_P, pb, sizeof( MgPoint3) * m_n);				//	構成点の領域にデータがあった場合は新領域へコピーする
		if (m_fa && pb)											//	構成点の領域をmallocで確保していた場合はその領域を開放する
			delete (char*)pb;											//
		m_fa = 1;
		m_isz = szn;
	}
}

//	最大最小を求める
MgMinMaxR3 MgPolyg3::MinMax()
{
	MINT	ic1;
	MgMinMaxR3	mm = MgMinMaxR3( MREALMAX, MREALMAX, MREALMAX, MREALMIN, MREALMIN, MREALMIN);

	for ( ic1=0; ic1<m_n; ic1++) {
		mm.Ins3( m_P[ic1]);
	}
	return mm;
}

//	トレース
void MgPolyg3::Print( MCHAR* s) const							// Print
{
#ifdef _DEBUG
	Msprintf( MbLog::m_Str, Mstr( "%s	MgPolyg3 サイズ: %d, 頂点数: %d\n"), s, m_isz, m_n);
	MBTRCPRBF;
	for (int ic=0; ic<m_n; ic++) {
		Msprintf( MbLog::m_Str, Mstr( "			P[%2d]: (%7.1f,%7.1f,%7.1f)\n"),
									ic, m_P[ic].x, m_P[ic].y, m_P[ic].z);
		MBTRCPRBF;
	}
#endif
}

//
//======================( ２次元 )==============================
//		多角形群
//
//	構成多角形の領域をmallocで確保する場合のコンストラクタ
MgGPolyg2::MgGPolyg2( MINT sz1)
{
	m_iTp = MGTP_GPOLYG; 
	m_isz = sz1;
	m_n = 0;
	m_fa = 1;
	MBMALLOC( m_pg, MgPolyg2, sz1);
	memset( m_pg, 0, sizeof( MgPolyg2) * sz1);
}

//	構成多角形の領域を配列で与える場合のコンストラクタ
MgGPolyg2::MgGPolyg2( MINT sz1, MgPolyg2 *pg2)
{
	m_iTp = MGTP_GPOLYG; 
	m_isz = sz1;
	m_n = 0;
	m_fa = 0;
	m_pg = pg2;
	memset( m_pg, 0, sizeof( MgPolyg2) * sz1);
}

//	デストラクタ
MgGPolyg2::~MgGPolyg2()
{
	MINT	ic;
	for ( ic=0; ic<m_n; ic++)
		m_pg[ic].Free();
	if ( m_fa)
		MBFREE( m_pg);
}

//	領域開放
void MgGPolyg2::Free()
{
	MINT	ic;
	for ( ic=0; ic<m_n; ic++)
		m_pg[ic].Free();
	if ( m_fa)
		MBFREE( m_pg);
	m_isz = m_n = m_fa = 0;
}

//	構成多角形を()で指定し読み書きする
const MgPolyg2& MgGPolyg2::operator () ( MINT idx) const
{
	if ( idx >= m_n)
		RETURN_OVERFLOW_ERR( MBCstr( "MgGPolyg2 ()"));
	return m_pg[idx];
}

MgPolyg2& MgGPolyg2::operator () ( MINT idx)
{
	if ( idx >= m_isz)
		RETURN_OVERFLOW_ERR( MBCstr( "MgGPolyg2 ()"));
	for (; m_n<=idx; m_n++)
		m_pg[m_n].SetInit();									// 最終データより後のデータはクリアし、n = idx+1　にする
	return m_pg[idx];
}

//	複写
void MgGPolyg2::operator = ( const MgGPolyg2 &GPg)
{
	MINT	ic;
	MINT	nn = GPg.m_n;
	if ( nn > m_isz) {
		MINT	szb = m_isz;
		MINT	sza = nn - m_isz;
		Resize( nn);
		memset( &m_pg[szb], 0, sizeof( MgPolyg2) * sza);		//	.Init
		m_isz = nn;
	}
	for ( ic=0; ic<nn; ic++)
		m_pg[ic] = GPg.m_pg[ic];
	m_n = nn;
}

void MgGPolyg2::operator = (const MgGPolyg3 &GPg)				// ３次元→２次元
{
	MINT	ic;
	MINT	nn = GPg.m_n;
	if ( nn > m_isz) {
		MINT	szb = m_isz;
		MINT	sza = nn - m_isz;
		Resize( nn);
		memset( &m_pg[szb], 0, sizeof( MgPolyg2) * sza);		//	.Init
		m_isz = nn;
	}
	for ( ic=0; ic<nn; ic++)
		m_pg[ic] = GPg.m_Pg[ic];
	m_n = nn;
}

//	構成多角形の追加
void MgGPolyg2::operator += ( const MgPolyg2 &Pg1)				// GPg += Pg1
{
	if (m_n >= m_isz) {
		MINT	sza = MGMAX( m_isz, 4);							// ２倍に領域拡張(拡張後の最低領域サイズは４)
		MINT	szb = m_isz;
		MINT	szn = m_isz + sza;
		Resize( szn);
		memset( &m_pg[szb], 0, sizeof( MgPolyg2) * sza);		//	.Init
	}
	m_pg[m_n++] = Pg1;
}

//	多角形群の追加
void MgGPolyg2::operator += ( const MgGPolyg2 &GPg)				// GPg += GPg1
{
	MINT	ic;
	MINT	nn = m_n + GPg.m_n;
//
	if ( nn > m_isz) {
		MINT	szb = m_isz;
		MINT	sza = nn - m_isz;
		Resize( nn);
		memset( &m_pg[szb], 0, sizeof( MgPolyg2) * sza);		//	.Init
	}
	for ( ic=0; ic<GPg.m_n; ic++)
		m_pg[m_n++] = GPg.m_pg[ic];
}


//	３次元の多角形より２次元の多角形を作成
void MgGPolyg2::Set( const MgGPolyg3 &GPg)						// ３次元→２次元代入用
{
	MINT	ic;
	MINT	nn = GPg.m_n;
	if ( nn > m_isz) {
		MINT	szb = m_isz;
		MINT	sza = nn - m_isz;
		Resize( nn);
		memset( &m_pg[szb], 0, sizeof( MgPolyg2) * sza);		//	.Init
		m_isz = nn;
	}
	for ( ic=0; ic<nn; ic++)
		m_pg[ic] = GPg.m_Pg[ic];
	m_n = nn;
}

//	空多角形の追加
void MgGPolyg2::AddPolyg()
{
	MINT	nn = m_n++;
	if ( nn > m_isz) {
		MINT	sza = MGMAX( m_isz, 4);							// ２倍に領域拡張(拡張後の最低領域サイズは４)
		MINT	szb = m_isz;
		Resize( m_isz + sza);
		memset( &m_pg[szb], 0, sizeof( MgPolyg2) * sza);		//	.Init
	}
	m_n = nn;
}

//	全ての多角形の回転方向の反転
void MgGPolyg2::Rev()
{
	MINT	ic1, ic2, ic3;
	MgPolyg2*	pg1;
	for ( ic1=0; ic1<m_n; ic1++) {
		pg1 = &m_pg[ic1];
		ic3 = pg1->m_n - 1;
		for ( ic2=0; ic2<ic3; ic2++) {
			MSWAP( pg1->m_p[ic2], pg1->m_p[ic3], MgPoint2);
			ic3--;
		}
	}
}

//	多角形群領域の拡張(拡張部分は未クリア)
void MgGPolyg2::Resize( MINT szn)
{
	//if (szn > sz) {
	//	if (fa && pg) {											//	構成多角形の領域をmallocで確保している場合
	//		MGREALLOC( pg, MgPolyg2, szn);						//		構成多角形の領域をreallocで拡張
	//	} else {												//	構成多角形の領域を未確保または配列の場合
	//		MgPolyg2		*pgb = pg;
	//		MBMALLOC( pg, MgPolyg2, szn);						//		構成多角形の領域をmallocで確保
	//		if (!fa) memcpy( pg, pgb, sizeof( MgPolyg2) * n);	//	構成多角形の領域が配列であった場合はmalloc領域へコピーする
	//		fa = 1;                                             
	//	}
	//	sz = szn;
	//}

	if (szn > m_isz) {
		MgPolyg2*	pgb = m_pg;
		MBMALLOC( m_pg, MgPolyg2, szn);			 				//	構成点の新領域をmallocで確保
		if ( m_n)
			 memcpy( m_pg, pgb, m_n * sizeof( MgPolyg2));		//	構成点の領域にデータがあった場合は新領域へコピーする
		if (m_fa && pgb)										//	構成点の領域をmallocで確保していた場合はその領域を開放する
			delete (char*)pgb;									//	
		m_fa = 1;
		m_isz = szn;
	}
}

//	トレース
void MgGPolyg2::Print( MCHAR* s) const							// Print
{
#ifdef _DEBUG
	Msprintf( MbLog::m_Str, Mstr( "%s	MgGPolyg2	サイズ: %d, ポリゴン数: %d\n"), s, m_isz, m_n);
	MBTRCPRBF;
	for (int ic=0; ic<m_n; ic++)
		m_pg[ic].Print( Mstr( "MgGPolyg2"));
#endif
}

//
//======================( ３次元 )==============================
//		多角形群
//
//	構成多角形の領域をmallocで確保する場合のコンストラクタ
MgGPolyg3::MgGPolyg3( MINT sz1)
{
	m_iTp = MGTP_GPOLYG3; 
	m_isz = sz1;
	m_n = 0;
	m_fa = 1;
	MBMALLOC( m_Pg, MgPolyg3, sz1);
	memset( m_Pg, 0, sizeof( MgPolyg3) * sz1);
}

//	構成多角形の領域を配列で与える場合のコンストラクタ
MgGPolyg3::MgGPolyg3( MINT sz1, MgPolyg3 *pg2)
{
	m_iTp = MGTP_GPOLYG3; 
	m_isz = sz1;
	m_n = 0;
	m_fa = 0;
	m_Pg = pg2;
	memset( m_Pg, 0, sizeof( MgPolyg3) * sz1);
}

//	デストラクタ
MgGPolyg3::~MgGPolyg3()
{
	MINT	ic;
	for ( ic=0; ic<m_n; ic++)
		m_Pg[ic].Free();
	if ( m_fa)
		MBFREE( m_Pg);
}

//	領域開放
void MgGPolyg3::Free()
{
	MINT	ic;
	for ( ic=0; ic<m_n; ic++)
		m_Pg[ic].Free();
	if ( m_fa)
		MBFREE( m_Pg);
	m_isz = m_n = m_fa = 0;

}

//	構成多角形を()で指定し読み書きする
const MgPolyg3& MgGPolyg3::operator () ( MINT idx) const
{
	if ( idx >= m_n)
		RETURN_OVERFLOW_ERR( MBCstr("MgGPolyg3 ()"));
	return m_Pg[idx];
};

MgPolyg3& MgGPolyg3::operator () ( MINT idx)
{
	if ( idx >= m_isz)
		RETURN_OVERFLOW_ERR( MBCstr( "MgGPolyg3 ()"));
	for (; m_n<=idx; m_n++)
		m_Pg[m_n].SetInit();									//  最終データより後のデータはクリアし、n = idx+1　にする
	return m_Pg[idx];
};

//	複写
void MgGPolyg3::operator = ( const MgGPolyg3 &GPg)
{
	MINT	ic;
	MINT	nn = GPg.m_n;
	if ( nn > m_isz) {
		MINT	szb = m_isz;
		MINT	sza = nn - m_isz;
		Resize( nn);
		memset( &m_Pg[szb], 0, sizeof( MgPolyg3) * sza);		//	.Init
	}
	for ( ic=0; ic<nn; ic++)
		m_Pg[ic] = GPg.m_Pg[ic];
	m_n = nn;
}

void MgGPolyg3::operator = (const MgGPolyg2 &Gpg)				// ２次元→３次元 (z = 0.)
{
	MINT	ic;
	MINT	nn = Gpg.m_n;
	if ( nn > m_isz) {
		MINT	szb = m_isz;
		MINT	sza = nn - m_isz;
		Resize( nn);
		memset( &m_Pg[szb], 0, sizeof( MgPolyg2) * sza);		//	.Init
		m_isz = nn;
	}
	for ( ic=0; ic<nn; ic++)
		m_Pg[ic] = Gpg.m_pg[ic];
	m_n = nn;
}

//	構成多角形の追加
void MgGPolyg3::operator += ( const MgPolyg3 &Pg1)				// GPg += Pg1
{
	if (m_n >= m_isz) {
		MINT	sza = MGMAX( m_isz, 4);							// ２倍に領域拡張(拡張後の最低領域サイズは４)
		MINT	szb = m_isz;
		MINT	szn = m_isz + sza;
		Resize( szn);
		memset( &m_Pg[szb], 0, sizeof( MgPolyg3) * sza);		//	.Init
	}
	m_Pg[m_n++] = Pg1;
}

//	多角形群の追加
void MgGPolyg3::operator += ( const MgGPolyg3 &GPg)				// GPg += GPg1
{
	MINT	ic;
	MINT	nn = m_n + GPg.m_n;
//
	if ( nn > m_isz) {
		MINT	szb = m_isz;
		MINT	sza = nn - m_isz;
		Resize( nn);
		memset( &m_Pg[szb], 0, sizeof( MgPolyg3) * sza);		//	.Init
	}
	for ( ic=0; ic<GPg.m_n; ic++)
		m_Pg[m_n++] = GPg.m_Pg[ic];
}

//	２次元の多角形群より３次元の多角形群を作成
void MgGPolyg3::Set( const MgGPolyg2 &Gpg, MREAL z)			// ２次元→３次元代入用 (z = 0.)
{
	MINT	ic;
	MINT	nn = Gpg.m_n;
	if ( nn > m_isz) {
		MINT	szb = m_isz;
		MINT	sza = nn - m_isz;
		Resize( nn);
		memset( &m_Pg[szb], 0, sizeof( MgPolyg2) * sza);		//	.Init
		m_isz = nn;
	}
	for ( ic=0; ic<nn; ic++)
		m_Pg[ic].Set( Gpg.m_pg[ic], z);
	m_n = nn;
}

void MgGPolyg3::Set( const MgGPolyg2 &Gpg, const MgPlane3& Pln)	// ２次元→３次元代入用
{
	MINT	ic;
	MINT	nn = Gpg.m_n;
	if ( nn > m_isz) {
		MINT	szb = m_isz;
		MINT	sza = nn - m_isz;
		Resize( nn);
		memset( &m_Pg[szb], 0, sizeof( MgPolyg2) * sza);		//	.Init
		m_isz = nn;
	}
	for ( ic=0; ic<nn; ic++)
		m_Pg[ic].Set( Gpg.m_pg[ic], Pln);
	m_n = nn;
}

//	空多角形の追加
void MgGPolyg3::AddPolyg()
{
	if (m_n >= m_isz) {
		MINT	sza = MGMAX( m_isz, 4);							// ２倍に領域拡張(拡張後の最低領域サイズは４)
		MINT	szb = m_isz;
		MINT	szn = m_isz + sza;
		Resize( szn);
		memset( &m_Pg[szb], 0, sizeof( MgPolyg3) * sza);		//	.Init
	}
	m_n++;
}

//	全ての多角形の回転方向の反転
void MgGPolyg3::Rev()
{
	MINT		ic1, ic2, ic3;
	MgPolyg3*	Pg1;
	for ( ic1=0; ic1<m_n; ic1++) {
		Pg1 = &m_Pg[ic1];
		ic3 = Pg1->m_n - 1;
		for ( ic2=0; ic2<ic3; ic2++) {
			MSWAP( Pg1->m_P[ic2], Pg1->m_P[ic3], MgPoint3);
			ic3--;
		}
	}
}

//	多角形群領域の拡張
void MgGPolyg3::Resize( MINT szn)
{
	//if (szn > sz) {
	//	if (fa && pg) {											//	構成多角形の領域をmallocで確保している場合
	//		MGREALLOC( pg, MgPolyg3, szn);						//		構成多角形の領域をremallocで拡張
	//	} else {												//	構成多角形の領域を未確保または配列の場合
	//		MgPolyg3*	pgb = pg;
	//		MBMALLOC( pg, MgPolyg3, szn);						//		構成多角形の領域をmallocで確保
	//		if (!fa) memcpy( pg, pgb, sizeof( MgPolyg3) * n);	//	構成多角形の領域が配列であった場合はmalloc領域へコピーする
	//		fa = 1;                                             
	//	}
	//	sz = szn;
	//}

	if (szn > m_isz) {
		MgPolyg3*	pgb = m_Pg;
		MBMALLOC( m_Pg, MgPolyg3, szn);		 					//	構成点の新領域をmallocで確保
		if ( m_n)
			 memcpy( m_Pg, pgb, m_n * sizeof( MgPolyg3));		//	構成点の領域にデータがあった場合は新領域へコピーする
		if (m_fa && pgb)										//	構成点の領域をmallocで確保していた場合はその領域を開放する
			delete (char*)pgb;									//	
		m_fa = 1;
		m_isz = szn;
	}
}

//	トレース
void MgGPolyg3::Print( MCHAR* s) const							// Print
{
#ifdef _DEBUG
	Msprintf( MbLog::m_Str, Mstr( "%s	MgGPolyg3	サイズ: %d, ポリゴン数: %d\n"), s, m_isz, m_n);
	MBTRCPRBF;
	for (int ic=0; ic<m_n; ic++)
		m_Pg[ic].Print( Mstr( "MgGPolyg3"));
#endif
}

//
//======================( ２次元 )==============================
//		線分群
//
//	構成線分の領域をmallocで確保する場合のコンストラクタ
MgGLine2::MgGLine2( MINT sz1)
{
	m_iTp = MGTP_GLINE; 
	m_isz = sz1;
	m_n = 0;
	m_fa = 1;
	MBMALLOC( m_ln, MgLine2, sz1);
}

//	構成線分の領域を配列で与える場合のコンストラクタ
MgGLine2::MgGLine2( MINT sz1, MgLine2 *ln2)
{
	m_iTp = MGTP_GLINE; 
	m_isz = sz1;
	m_n = 0;
	m_fa = 0;
	m_ln = ln2;
}

//	構成線分を()で指定して読み書きする
const MgLine2& MgGLine2::operator () ( MINT idx) const
{
	if ( idx >= m_n)
		Base::MbSysError( MBCstr( "MgGLine2 ()"), MC_ERR_OVERFLOW);
	return m_ln[idx];
}

MgLine2& MgGLine2::operator () ( MINT idx)
{
	if ( idx >= m_isz)
		Base::MbSysError( MBCstr( "MgGLine2 ()"), MC_ERR_OVERFLOW);
	return m_ln[idx];
}

//	複写
void MgGLine2::operator = ( const MgGLine2& GLn)
{
	MINT	nn = GLn.m_n;
	if ( nn > m_isz) {
		if (m_fa) MBFREE( m_ln);								//	構成線分の領域をmallocで確保していた場合は一旦free
		MBMALLOC( m_ln, MgLine2, nn);							//	構成線分の領域をmallocで確保
		m_fa = 1;
		m_isz = nn;
	}
	memcpy( m_ln, GLn.m_ln, sizeof( MgLine2) * nn);
	m_n = nn;
}

void MgGLine2::operator = ( const MgGLine3 &GLn)				// ３次元→２次元
{
	MINT	ic;
	MINT	nn = GLn.m_n;
	if ( nn > m_isz) {
		if ( m_fa) MBFREE( m_ln);								//	構成線分の領域をmallocで確保していた場合は一旦free
		MBMALLOC( m_ln, MgLine2, nn);							//	構成線分の領域をmallocで確保
		m_fa = 1;
		m_isz = nn;
	}
	for ( ic=0; ic<nn; ic++)
		m_ln[ic] = MgLine2C( GLn.m_ln[ic]);
	m_n = nn;
}

//	構成線分の追加
void MgGLine2::operator += ( const MgLine2& Ln1)				// GLn += Ln1
{
	MgLine2	Ln1sv;
	if (m_n >= m_isz) {
		Ln1sv = Ln1;											// ソースがリサイズする元の領域にあるデータの場合を考慮
		Resize( m_isz + MGMAX( m_isz, 4));
																//	２倍に領域拡張(拡張後の最低領域サイズは４とする)
		m_ln[m_n++] = Ln1sv;
	} else {
		m_ln[m_n++] = Ln1;
	}
}

//	構成線分群の追加
void MgGLine2::operator += ( const MgGLine2 &GLn)				// GLn += GLn1
{
	MINT	nn = m_n + GLn.m_n;
	if ( nn > m_isz)
		Resize( nn);
	memcpy( &m_ln[m_n], GLn.m_ln, sizeof( MgLine2) * GLn.m_n);
	m_n = nn;
}

//	３次元の線分群より２次元の線分群を作成
void MgGLine2::Set( const MgGLine3 &GLn)						// ３次元→２次元代入用
{
	MINT	ic;
	MINT	nn = GLn.m_n;
	if ( nn > m_isz) {
		if ( m_fa) MBFREE( m_ln);								//	構成線分の領域をmallocで確保していた場合は一旦free
		MBMALLOC( m_ln, MgLine2, nn);							//	構成線分の領域をmallocで確保
		m_fa = 1;
		m_isz = nn;
	}
	for ( ic=0; ic<nn; ic++)
		m_ln[ic] = MgLine2C( GLn.m_ln[ic]);
	m_n = nn;
}

//	全線分の反転
void MgGLine2::Rev()
{
	MINT	ic1;
	for ( ic1=0; ic1<m_n; ic1++) {
		MSWAP( m_ln[ic1].p[0], m_ln[ic1].p[1], MgPoint2);
	}
}

//	線分群領域の拡張
void MgGLine2::Resize( MINT szn)
{
	//if (szn > sz) {
	//	if (fa && ln) {											//	構成線分の領域をmallocで確保している場合
	//		MGREALLOC( ln, MgLine2, szn);						//		構成線分の領域をremallocで拡張
	//	} else {												//	構成線分の領域を未確保または配列の場合
	//		MgLine2		*lnb = ln;
	//		MBMALLOC( ln, MgLine2, szn);						//		構成点の領域をmallocで確保
	//		if (!fa) memcpy( ln, lnb, sizeof( MgLine2) * n);	//	構成線分の領域が配列であった場合はmalloc領域へコピーする
	//		fa = 1;
	//	}
	//	sz = szn;
	//}

	if (szn > m_isz) {
		MgLine2	*lnb = m_ln;
		MBMALLOC( m_ln, MgLine2, szn);			 				//	構成点の新領域をmallocで確保
		if ( m_n)
			 memcpy( m_ln, lnb, m_n * sizeof( MgLine2));		//	構成点の領域にデータがあった場合は新領域へコピーする
		if (m_fa && lnb)										//	構成点の領域をmallocで確保していた場合はその領域を開放する
			delete (char*)lnb;									//	
		m_fa = 1;
		m_isz = szn;
	}
}

//	トレース
void MgGLine2::Print( MCHAR* s) const
{
#ifdef _DEBUG
	Msprintf( MbLog::m_Str, Mstr( "%s	MgGLine2	サイズ: %d, 線分数: %d\n"), s, m_isz, m_n);
	MBTRCPRBF;
	for (int ic=0; ic<m_n; ic++) {
		Msprintf( MbLog::m_Str, Mstr( "			ln[%2d]: (%7.1f,%7.1f,%7.1f,%7.1f)\n"),
						  ic, m_ln[ic].p[0].x, m_ln[ic].p[0].y,
						      m_ln[ic].p[1].x, m_ln[ic].p[1].y);
		MBTRCPRBF;
	}
#endif
}

//
//======================( ３次元 )==============================
//		線分群
//

//	構成多角形の領域をmallocで確保する場合のコンストラクタ
MgGLine3::MgGLine3( MINT sz1)
{
	m_iTp = MGTP_GLINE3; 
	m_isz = sz1;
	m_n = 0;
	m_fa = 1;
	MBMALLOC( m_ln, MgLine3, sz1);
}

//	構成線分の領域を配列で与える場合のコンストラクタ
MgGLine3::MgGLine3( MINT sz1, MgLine3 *ln2)
{
	m_iTp = MGTP_GLINE3; 
	m_isz = sz1;
	m_n = 0;
	m_fa = 0;
	m_ln = ln2;
}

//	構成線分を()で指定して読み書きする
const MgLine3& MgGLine3::operator () ( MINT idx) const
{
	if ( idx >= m_n)
		Base::MbSysError( MBCstr( "MgGLine3 ()"), MC_ERR_OVERFLOW);
	return m_ln[idx];
};

MgLine3& MgGLine3::operator () ( MINT idx)
{
	if ( idx >= m_isz)
		Base::MbSysError( MBCstr( "MgGLine3 ()"), MC_ERR_OVERFLOW);
	return m_ln[idx];
};

//	複写
void MgGLine3::operator = ( const MgGLine3& GLn)
{
	MINT	nn = GLn.m_n;
	if ( nn > m_isz) {
		if ( m_fa) MBFREE( m_ln);								//	構成線分の領域をmallocで確保していた場合は一旦free
		MBMALLOC( m_ln, MgLine3, nn);							//	構成線分の領域をmallocで確保
		m_fa = 1;
		m_isz = nn;
	}
	memcpy( m_ln, GLn.m_ln, sizeof( MgLine3) * nn);
	m_n = nn;
}

//	２次元の直線群より３次元の直線群を作成
void MgGLine3::operator = (const MgGLine2 &Gln)					// ２次元→３次元 (z = 0.)
{
	MINT	ic;
	MINT	nn = Gln.m_n;
	if ( nn > m_isz) {
		if ( m_fa) MBFREE( m_ln);								//	構成線分の領域をmallocで確保していた場合は一旦free
		MBMALLOC( m_ln, MgLine3, nn);							//	構成線分の領域をmallocで確保
		m_fa = 1;
		m_isz = nn;
	}
	for ( ic=0; ic<nn; ic++)
		m_ln[ic] = MgLine3C( Gln.m_ln[ic]);
	m_n = nn;
}

//	構成線分の追加
void MgGLine3::operator += ( const MgLine3& Ln1)				// GLn += Ln1
{
	MgLine3	Ln1sv;
	if (m_n >= m_isz) {
		Ln1sv = Ln1;											// ソースがリサイズする元の領域にあるデータの場合を考慮
		Resize( m_isz + MGMAX( m_isz, 4));
																//	２倍に領域拡張(拡張後の最低領域サイズは４とする)
		m_ln[m_n++] = Ln1sv;
	} else {
		m_ln[m_n++] = Ln1;
	}
}

//	構成線分群の追加
void MgGLine3::operator += ( const MgGLine3 &GLn)				// GLn += GLn1
{
	MINT	nn = m_n + GLn.m_n;
	if ( nn > m_isz)
		Resize( nn);
	memcpy( &m_ln[m_n], GLn.m_ln, sizeof( MgLine3) * GLn.m_n);
	m_n = nn;
}

//	２次元の線分群より３次元の線分群を作成
void MgGLine3::Set( const MgGLine2 &Gln, const MgPlane3& Pln)	// ２次元→３次元代入用
{
	MINT	ic;
	MINT	nn = Gln.m_n;
	if ( nn > m_isz) {
		if ( m_fa) MBFREE( m_ln);								//	構成線分の領域をmallocで確保していた場合は一旦free
		MBMALLOC( m_ln, MgLine3, nn);							//	構成線分の領域をmallocで確保
		m_fa = 1;
		m_isz = nn;
	}
	for ( ic=0; ic<nn; ic++)
		m_ln[ic].Set( Gln.m_ln[ic], Pln);
	m_n = nn;
}

void MgGLine3::Set( const MgGLine2 &Gln, MREAL z)				// ２次元→３次元代入用 (z = 0.)
{
	MINT	ic;
	MINT	nn = Gln.m_n;
	if ( nn > m_isz) {
		if ( m_fa) MBFREE( m_ln);								//	構成線分の領域をmallocで確保していた場合は一旦free
		MBMALLOC( m_ln, MgLine3, nn);							//	構成線分の領域をmallocで確保
		m_fa = 1;
		m_isz = nn;
	}
	for ( ic=0; ic<nn; ic++)
		m_ln[ic].Set( Gln.m_ln[ic], z);
	m_n = nn;
}

//	全線分の反転
void MgGLine3::Rev()
{
	MINT	ic1;
	for ( ic1=0; ic1<m_n; ic1++) {
		MSWAP( m_ln[ic1].p[0], m_ln[ic1].p[1], MgPoint3);
	}
}

//	線分群領域の拡張
void MgGLine3::Resize( MINT szn)
{
	//if (szn > sz) {
	//	if (fa && ln) {											//	構成線分の領域をmallocで確保している場合
	//		MGREALLOC( ln, MgLine3, szn);						//		構成線分の領域をremallocで拡張
	//	} else {												//	構成線分の領域を未確保または配列の場合
	//		MgLine3	*lnb = ln;
	//		MBMALLOC( ln, MgLine3, szn);						//		構成点の領域をmallocで確保
	//		if (!fa) memcpy( ln, lnb, sizeof( MgLine3) * n);	//	構成線分の領域が配列であった場合はmalloc領域へコピーする
	//		fa = 1;
	//	}
	//	sz = szn;
	//}

	if (szn > m_isz) {
		MgLine3	*lnb = m_ln;
		MBMALLOC( m_ln, MgLine3, szn);		 					//	構成点の新領域をmallocで確保
		if ( m_n)
			 memcpy( m_ln, lnb, m_n * sizeof( MgLine3));			//	構成点の領域にデータがあった場合は新領域へコピーする
		if (m_fa && lnb)										//	構成点の領域をmallocで確保していた場合はその領域を開放する
			delete (char*)lnb;									//	
		m_fa = 1;
		m_isz = szn;
	}
}

//	トレース
void MgGLine3::Print( MCHAR* s) const
{
#ifdef _DEBUG
	Msprintf( MbLog::m_Str, Mstr( "%s	MgGLine3	サイズ: %d, 線分数: %d\n"), s, m_isz, m_n);
	MBTRCPRBF;
	for (int ic=0; ic<m_n; ic++) {
		Msprintf( MbLog::m_Str, Mstr( "			ln[%2d]: (%7.1f,%7.1f,%7.1f),(%7.1f,%7.1f,%7.1f)\n"),
						  ic, m_ln[ic].p[0].x, m_ln[ic].p[0].y, m_ln[ic].p[0].z,
						      m_ln[ic].p[1].x, m_ln[ic].p[1].y, m_ln[ic].p[1].z);
		MBTRCPRBF;
	}
#endif
}

//
//======================( １次元 )==============================
//		整数群
//

//	構成整数の領域をmallocで確保する場合のコンストラクタ
MgGInt::MgGInt( MINT sz1)
{
	m_iTp = MGTP_GINT;
	m_isz = sz1;
	m_n = 0;
	m_fa = 1;
	MBMALLOC( m_i, MINT, sz1);
}

//	構成整数の領域を配列で与える場合のコンストラクタ
MgGInt::MgGInt( MINT sz1, MINT *i2)
{
	m_iTp = MGTP_GINT;
	m_isz = sz1;
	m_n = 0;
	m_fa = 0;
	m_i = i2;
}

//	構成整数を()で指定して読み書きする
const MINT& MgGInt::operator () ( MINT idx) const
{
	if ( idx >= m_n)
		Base::MbSysError( MBCstr( "MgGInt ()"), MC_ERR_OVERFLOW);
	return m_i[idx];
};

MINT& MgGInt::operator () ( MINT idx)
{
	if ( idx >= m_isz)
		Base::MbSysError( MBCstr( "MgGInt ()"), MC_ERR_OVERFLOW);
	return m_i[idx];
};

//	複写
void MgGInt::operator = ( const MgGInt& GI)
{
	MINT	nn = GI.m_n;
	if ( nn > m_isz) {
		if ( m_fa) MBFREE( m_i);								//	構成整数の領域をmallocで確保していた場合は一旦free
		MBMALLOC( m_i, MINT, nn);								//	構成整数の領域をmallocで確保
		m_fa = 1;
		m_isz = nn;
	}
	memcpy( m_i, GI.m_i, sizeof( MINT) * nn);
	m_n = nn;
}

void MgGInt::operator = (const MgGReal &GR)					// 実数群 → 整数群
{
	MINT	ic;
	MINT	nn = GR.m_n;
	if ( nn > m_isz) {
		if ( m_fa) MBFREE( m_i);								//	構成整数の領域をmallocで確保していた場合は一旦free
		MBMALLOC( m_i, MINT, nn);								//	構成整数の領域をmallocで確保
		m_fa = 1;
		m_isz = nn;
	}
	for ( ic=0; ic<nn; ic++)
		m_i[ic] = (MINT)GR.m_r[ic];
	m_n = nn;
}

//	構成整数の追加
void MgGInt::operator += ( const MINT &i1)						// GPt += i1
{
	MINT	i1sv;
	if (m_n >= m_isz) {
		i1sv = i1;												// ソースがリサイズする元の領域にあるデータの場合を考慮
		Resize( m_isz + MGMAX( m_isz, 4));
																//	２倍に領域拡張(拡張後の最低領域サイズは４とする)
		m_i[m_n++] = i1sv;
	} else {
		m_i[m_n++] = i1;
	}
}

//	整数群の追加
void MgGInt::operator += ( const MgGInt &GI)					// GPt += GI
{
	MINT	nn = m_n + GI.m_n;
	if ( nn > m_isz)
		Resize( nn);
	memcpy( &m_i[m_n], GI.m_i, sizeof( MINT) * GI.m_n);
	m_n = nn;
}

//	実数群の代入
void MgGInt::Set( const MgGReal &GR)							// 実数群→整数群 代入用
{
	MINT	ic;
	MINT	nn = GR.m_n;
	if ( nn > m_isz) {
		if ( m_fa) MBFREE( m_i);								//	構成整数の領域をmallocで確保していた場合は一旦free
		MBMALLOC( m_i, MINT, nn);								//	構成整数の領域をmallocで確保
		m_fa = 1;
		m_isz = nn;
	}
	for ( ic=0; ic<nn; ic++)
		m_i[ic] = (MINT)GR.m_r[ic];
	m_n = nn;
}

//	整数群領域の拡張
void MgGInt::Resize( MINT szn)
{
	//if (szn > sz) {
	//	if (fa && i) {											//	構成整数の領域をmallocで確保している場合
	//		MGREALLOC( i, MINT, szn);							//		構成整数の領域をremallocで拡張
	//	} else {												//	構成整数の領域を未確保または配列の場合
	//		MINT*		pb = i;
	//		MBMALLOC( i, MINT, szn);							//		構成整数の領域をmallocで確保
	//		if (!fa) memcpy( i, pb, sizeof( MINT) * n);			//	構成整数の領域が配列であった場合はmalloc領域へコピーする
	//		fa = 1;                                             
	//	}
	//	sz = szn;
	//}

	if (szn > m_isz) {
		MINT	*ib = m_i;
		MBMALLOC( m_i, MINT, szn);			 					//	構成点の新領域をmallocで確保
		if ( m_n)
			 memcpy( m_i, ib, m_n * sizeof( MINT));				//	構成点の領域にデータがあった場合は新領域へコピーする
		if (m_fa && ib)											//	構成点の領域をmallocで確保していた場合はその領域を開放する
			delete (char*)ib;									//	
		m_fa = 1;
		m_isz = szn;
	}
}

//	トレース
void MgGInt::Print( MCHAR* s) const
{
#ifdef _DEBUG
	Msprintf( MbLog::m_Str, Mstr( "%s	MgGInt	サイズ: %d, 整数の数: %d\n"), s, m_isz, m_n);
	MBTRCPRBF;
	for (int ic=0; ic<m_n; ic++) {
		Msprintf( MbLog::m_Str, Mstr( "			P[%2d]: %d\n"),
						  ic, m_i[ic]);
		MBTRCPRBF;
	}
#endif
}

//
//======================( １次元 )==============================
//		実数群
//

//	構成実数の領域をmallocで確保する場合のコンストラクタ
MgGReal::MgGReal( MINT sz1)
{
	m_iTp = MGTP_GREAL;
	m_isz = sz1;
	m_n = 0;
	m_fa = 1;
	MBMALLOC( m_r, MREAL, sz1);
}

//	構成実数の領域を配列で与える場合のコンストラクタ
MgGReal::MgGReal( MINT sz1, MREAL* r2)
{
	m_iTp = MGTP_GINT;
	m_isz = sz1;
	m_n = 0;
	m_fa = 0;
	m_r = r2;
}

//	構成実数を()で指定して読み書きする
const MREAL& MgGReal::operator () ( MINT idx) const
{
	if ( idx >= m_n)
		RETURN_OVERFLOW_ERR( MBCstr( "MgGReal ()"));
	return m_r[idx];
};

MREAL& MgGReal::operator () ( MINT idx)
{
	if ( idx >= m_isz)
		RETURN_OVERFLOW_ERR( MBCstr( "MgGReal ()"));
	return m_r[idx];
};

//	複写
void MgGReal::operator = ( const MgGReal& GR)
{
	MINT	nn = GR.m_n;
	if ( nn > m_isz) {
		if ( m_fa) MBFREE( m_r);								//	構成実数の領域をmallocで確保していた場合は一旦free
		MBMALLOC( m_r, MREAL, nn);								//	構成実数の領域をmallocで確保
		m_fa = 1;
		m_isz = nn;
	}
	memcpy( m_r, GR.m_r, sizeof( MREAL) * nn);
	m_n = nn;
}

void MgGReal::operator = (const MgGInt &GI)					// 整数群 → 実数群
{
	MINT	ic;
	MINT	nn = GI.m_n;
	if ( nn > m_isz) {
		if ( m_fa) MBFREE( m_r);								//	構成実数の領域をmallocで確保していた場合は一旦free
		MBMALLOC( m_r, MREAL, nn);								//	構成実数の領域をmallocで確保
		m_fa = 1;
		m_isz = nn;
	}
	for ( ic=0; ic<nn; ic++)
		m_r[ic] = (MREAL)GI.m_i[ic];
	m_n = nn;
}

//	構成実数の追加
void MgGReal::operator += ( const MREAL &r1)					// GPt += i1
{
	MREAL	r1sv;
	if (m_n >= m_isz) {
		r1sv = r1;												// ソースがリサイズする元の領域にあるデータの場合を考慮
		Resize( m_isz + MGMAX( m_isz, 4));
																//	２倍に領域拡張(拡張後の最低領域サイズは４とする)
		m_r[m_n++] = r1sv;
	} else {
		m_r[m_n++] = r1;
	}
}

//	実数群の追加
void MgGReal::operator += ( const MgGReal &GR)				// GPt += GI
{
	MINT	nn = m_n + GR.m_n;
	if ( nn > m_isz)
		Resize( nn);
	memcpy( &m_r[m_n], GR.m_r, sizeof( MREAL) * GR.m_n);
	m_n = nn;
}

//	整数群の代入
void MgGReal::Set( const MgGInt &GI)							// 整数群→実数群 代入用
{
	MINT	ic;
	MINT	nn = GI.m_n;
	if ( nn > m_isz) {
		if ( m_fa) MBFREE( m_r);								//	構成実数の領域をmallocで確保していた場合は一旦free
		MBMALLOC( m_r, MREAL, nn);								//	構成実数の領域をmallocで確保
		m_fa = 1;
		m_isz = nn;
	}
	for ( ic=0; ic<nn; ic++)
		m_r[ic] = (MREAL)GI.m_i[ic];
	m_n = nn;
}

//	実数群領域の拡張
void MgGReal::Resize( MINT szn)
{
	//if (szn > sz) {
	//	if (fa && r) {											//	構成実数の領域をmallocで確保している場合
	//		MGREALLOC( r, MREAL, szn);							//		構成実数の領域をremallocで拡張
	//	} else {												//	構成実数の領域を未確保または配列の場合
	//		MREAL		*pb = r;
	//		MBMALLOC( r, MREAL, szn);							//		構成実数の領域をmallocで確保
	//		if (!fa) memcpy( r, pb, sizeof( MREAL) * n);		//	構成実数の領域が配列であった場合はmalloc領域へコピーする
	//		fa = 1;                                             
	//	}
	//	sz = szn;
	//}

	if (szn > m_isz) {
		MREAL	*rb = m_r;
		MBMALLOC( m_r, MREAL, szn);				 			//	構成点の新領域をmallocで確保
		if ( m_n)
			 memcpy( m_r, rb, m_n * sizeof( MREAL));			//	構成点の領域にデータがあった場合は新領域へコピーする
		if (m_fa && rb)											//	構成点の領域をmallocで確保していた場合はその領域を開放する
			delete (char*)rb;									//	
		m_fa = 1;
		m_isz = szn;
	}
}

//	トレース
void MgGReal::Print( MCHAR* s) const
{
#ifdef _DEBUG
	Msprintf( MbLog::m_Str, Mstr( "%s	MgGReal	サイズ: %d, 実数の数: %d\n"), s, m_isz, m_n);
	MBTRCPRBF;
	for (int ic=0; ic<m_n; ic++) {
		Msprintf( MbLog::m_Str, Mstr( "			P[%2d] = %f\n"),
						  ic, m_r[ic]);
		MBTRCPRBF;
	}
#endif
}

//
//======================( ２次元 )==============================
//		点群
//

//	構成点の領域をmallocで確保する場合のコンストラクタ
MgGPoint2::MgGPoint2( MINT sz1)
{
	m_iTp = MGTP_GPOINT2;
	m_isz = sz1;
	m_n = 0;
	m_fa = 1;
	MBMALLOC( m_p, MgPoint2, sz1);
}

//	構成点の領域を配列で与える場合のコンストラクタ
MgGPoint2::MgGPoint2( MINT sz1, MgPoint2 *p2)
{
	m_iTp = MGTP_GPOINT2;
	m_isz = sz1;
	m_n = 0;
	m_fa = 0;
	m_p = p2;
}

//	構成点を()で指定して読み書きする
const MgPoint2& MgGPoint2::operator () ( MINT idx) const
{
	if ( idx >= m_n)
		RETURN_OVERFLOW_ERR( MBCstr( "MgGPoint2 ()"));
	return m_p[idx];
};

MgPoint2& MgGPoint2::operator () ( MINT idx)
{
	if ( idx >= m_isz)
		RETURN_OVERFLOW_ERR( MBCstr( "MgGPoint2 ()"));
	return m_p[idx];
};

//	複写
void MgGPoint2::operator = ( const MgGPoint2& Pg)
{
	MINT	nn = Pg.m_n;
	if ( nn > m_isz) {
		if ( m_fa) MBFREE( m_p);								//	構成点の領域をmallocで確保していた場合は一旦free
		MBMALLOC( m_p, MgPoint2, nn);							//	構成点の領域をmallocで確保
		m_fa = 1;
		m_isz = nn;
	}
	memcpy( m_p, Pg.m_p, sizeof( MgPoint2) * nn);
	m_n = nn;
}

void MgGPoint2::operator = (const MgGPoint3 &GPt)				// ３次元→２次元
{
	MINT	ic;
	MINT	nn = GPt.m_n;
	if ( nn > m_isz) {
		if ( m_fa) MBFREE( m_p);								//	構成点の領域をmallocで確保していた場合は一旦free
		MBMALLOC( m_p, MgPoint2, nn);							//	構成点の領域をmallocで確保
		m_fa = 1;
		m_isz = nn;
	}
	for ( ic=0; ic<nn; ic++)
		m_p[ic] = MgPoint2C( GPt.m_p[ic]);
	m_n = nn;
}

//	構成点の追加
void MgGPoint2::operator += ( const MgPoint2& p1)				// GPt += Pt1
{
	MgPoint2	p1sv;
	if (m_n >= m_isz) {
		p1sv = p1;												// ソースがリサイズする元の領域にあるデータの場合を考慮
		Resize( m_isz + MGMAX( m_isz, 4));
																//	２倍に領域拡張(拡張後の最低領域サイズは４とする)
		m_p[m_n++] = p1sv;
	} else {
		m_p[m_n++] = p1;
	}
}

//	点群の追加
void MgGPoint2::operator += ( const MgGPoint2 &GPt)				// GPt += GPt1
{
	MINT	nn = m_n + GPt.m_n;
	if ( nn > m_isz)
		Resize( nn);
	memcpy( &m_p[m_n], GPt.m_p, sizeof( MgPoint2) * GPt.m_n);
	m_n = nn;
}

//	３次元の点群より２次元の点群を作成
void MgGPoint2::Set( const MgGPoint3 &GPt)						// ３次元→２次元代入用
{
	MINT	ic;
	MINT	nn = GPt.m_n;
	if ( nn > m_isz) {
		if ( m_fa) MBFREE( m_p);								//	構成点の領域をmallocで確保していた場合は一旦free
		MBMALLOC( m_p, MgPoint2, nn);							//	構成点の領域をmallocで確保
		m_fa = 1;
		m_isz = nn;
	}
	for ( ic=0; ic<nn; ic++)
		m_p[ic] = MgPoint2C( GPt.m_p[ic]);
	m_n = nn;
}

//	点群領域の拡張
void MgGPoint2::Resize( MINT szn)
{
	//if (szn > sz) {
	//	if (fa && p) {											//	構成点の領域をmallocで確保している場合
	//		MGREALLOC( p, MgPoint2, szn);						//		構成点の領域をremallocで拡張
	//	} else {												//	構成点の領域を未確保または配列の場合
	//		MgPoint2		*pb = p;
	//		MBMALLOC( p, MgPoint2, szn);						//		構成点の領域をmallocで確保
	//		if (!fa) memcpy( p, pb, sizeof( MgPoint2) * n);		//	構成点の領域が配列であった場合はmalloc領域へコピーする
	//		fa = 1;                                             
	//	}
	//	sz = szn;
	//}

	if (szn > m_isz) {
		MgPoint2*	pb = m_p;
		MBMALLOC( m_p, MgPoint2, szn);			 				//	構成点の新領域をmallocで確保
		if ( m_n)
			 memcpy( m_p, pb, m_n * sizeof( MgPoint2));			//	構成点の領域にデータがあった場合は新領域へコピーする
		if ( m_fa && pb)										//	構成点の領域をmallocで確保していた場合はその領域を開放する
			delete (char*)pb;									//	
		m_fa = 1;
		m_isz = szn;
	}
}

//	最大最小を求める
MgMinMaxR2 MgGPoint2::MinMax()
{
	MINT	ic1;
	MgMinMaxR2	mm = MgMinMaxR2( MREALMAX, MREALMAX, MREALMIN, MREALMIN);

	for ( ic1=0; ic1<m_n; ic1++) {
		mm.Ins2( m_p[ic1]);
	}
	return mm;
}

//	トレース
void MgGPoint2::Print( MCHAR* s) const
{
#ifdef _DEBUG
	Msprintf( MbLog::m_Str, Mstr( "%s	MgPolyg2	サイズ: %d, 頂点数: %d\n"), s, m_isz, m_n);
	MBTRCPRBF;
	for (int ic=0; ic<m_n; ic++) {
		Msprintf( MbLog::m_Str, Mstr( "			P[%2d]: (%7.1f,%7.1f)\n"),
						  ic, m_p[ic].x, m_p[ic].y);
		MBTRCPRBF;
	}
#endif
}

//
//======================( ３次元 )==============================
//		点群
//

//	構成点の領域をmallocで確保する場合のコンストラクタ
MgGPoint3::MgGPoint3( MINT sz1)
{
	m_iTp = MGTP_GPOINT3;
	m_isz = sz1;
	m_n = 0;
	m_fa = 1;
	MBMALLOC( m_p, MgPoint3, sz1);
}

//	構成点の領域を配列で与える場合のコンストラクタ
MgGPoint3::MgGPoint3( MINT sz1, MgPoint3 *p2)
{
	m_iTp = MGTP_GPOINT3;
	m_isz = sz1;
	m_n = 0;
	m_fa = 0;
	m_p = p2;
}

//	構成点を()で指定して読み書きする
const MgPoint3& MgGPoint3::operator () ( MINT idx) const
{
	if ( idx >= m_n)
		RETURN_OVERFLOW_ERR( MBCstr( "MgGPoint3 ()"));
	return m_p[idx];
};

MgPoint3& MgGPoint3::operator () ( MINT idx)
{
	if ( idx >= m_isz)
		RETURN_OVERFLOW_ERR( MBCstr( "MgGPoint3 ()"));
	return m_p[idx];
};

//	複写
void MgGPoint3::operator = ( const MgGPoint3& GPt)
{
	MINT	nn = GPt.m_n;
	if ( nn > m_isz) {
		if ( m_fa)
			MBFREE( m_p);										//	構成点の領域をmallocで確保していた場合は一旦free
		MBMALLOC( m_p, MgPoint3, nn);							//	構成点の領域をmallocで確保
		m_fa = 1;
		m_isz = nn;
	}
	memcpy( m_p, GPt.m_p, sizeof( MgPoint3) * nn);
	m_n = nn;
}

void MgGPoint3::operator = ( const MgGPoint2& Gpt)
{
	MINT	ic;
	MINT	nn = Gpt.m_n;
	if ( nn > m_isz) {
		if ( m_fa)
			MBFREE( m_p);										//	構成点の領域をmallocで確保していた場合は一旦free
		MBMALLOC( m_p, MgPoint3, nn);							//	構成点の領域をmallocで確保
		m_fa = 1;
		m_isz = nn;
	}
	for ( ic=0; ic<nn; ic++)
		m_p[ic] = MgPoint3C( Gpt.m_p[ic]);
	m_n = nn;
}

//	構成点の追加
void MgGPoint3::operator += ( const MgPoint3& p1)				// GPt += Pt1
{
	MgPoint3	p1sv;
	if (m_n >= m_isz) {
		p1sv = p1;												// ソースがリサイズする元の領域にあるデータの場合を考慮
		Resize( m_isz + MGMAX( m_isz, 4));
																//	２倍に領域拡張(拡張後の最低領域サイズは４とする)
		m_p[m_n++] = p1sv;
	} else {
		m_p[m_n++] = p1;
	}
}

//	点群の追加
void MgGPoint3::operator += ( const MgGPoint3 &GPt)				// GPt += GPt1
{
	MINT	nn = m_n + GPt.m_n;
	if ( nn > m_isz)
		Resize( nn);
	memcpy( &m_p[m_n], GPt.m_p, sizeof( MgPoint3) * GPt.m_n);
	m_n = nn;
}

//	２次元の点群より３次元の点群を作成
void MgGPoint3::Set( const MgGPoint2 &Gpt, const MgPlane3& Pln)	// ２次元→３次元代入用
{
	MINT	ic;
	MINT	nn = Gpt.m_n;
	if ( nn > m_isz) {
		if ( m_fa)
			MBFREE( m_p);										//	構成点の領域をmallocで確保していた場合は一旦free
		MBMALLOC( m_p, MgPoint3, nn);							//	構成点の領域をmallocで確保
		m_fa = 1;
		m_isz = nn;
	}
	for ( ic=0; ic<nn; ic++)
		m_p[ic].Set( Gpt.m_p[ic], Pln);
	m_n = nn;
}

void MgGPoint3::Set( const MgGPoint2 &Gpt, MREAL z)			// ２次元→３次元代入用 (z = 0.)
{
	MINT	ic;
	MINT	nn = Gpt.m_n;
	if ( nn > m_isz) {
		if ( m_fa)
			MBFREE( m_p);										//	構成点の領域をmallocで確保していた場合は一旦free
		MBMALLOC( m_p, MgPoint3, nn);							//	構成点の領域をmallocで確保
		m_fa = 1;
		m_isz = nn;
	}
	for ( ic=0; ic<nn; ic++)
		m_p[ic].Set( Gpt.m_p[ic], z);
	m_n = nn;
}

//	点群領域の拡張
void MgGPoint3::Resize( MINT szn)
{
	//if (szn > sz) {
	//	if (fa && p) {											//	構成点の領域をmallocで確保している場合
	//		MGREALLOC( p, MgPoint3, szn);						//		構成点の領域をremallocで拡張
	//	} else {												//	構成点の領域を未確保または配列の場合
	//		MgPoint3	*pb = p;
	//		MBMALLOC( p, MgPoint3, szn);						//		構成点の領域をmallocで確保
	//		if (!fa) memcpy( p, pb, sizeof( MgPoint3) * n);		//	構成点の領域が配列であった場合はmalloc領域へコピーする
	//		fa = 1;                                             
	//	}
	//	sz = szn;
	//}

	if (szn > m_isz) {
		MgPoint3	*pb = m_p;
		MBMALLOC( m_p, MgPoint3, szn);							//	構成点の新領域をmallocで確保
		if ( m_n)
			memcpy( m_p, pb, sizeof( MgPoint3) * m_n);			//	構成点の領域にデータがあった場合は新領域へコピーする
		if ( m_fa && pb)										//	構成点の領域をmallocで確保していた場合はその領域を開放する
			delete (char*)pb;									//
		m_fa = 1;
		m_isz = szn;
	}
}

//	最大最小を求める
MgMinMaxR3 MgGPoint3::MinMax()
{
	MINT	ic1;
	MgMinMaxR3	mm = MgMinMaxR3( MREALMAX, MREALMAX, MREALMAX, MREALMIN, MREALMIN, MREALMIN);

	for ( ic1=0; ic1<m_n; ic1++) {
		mm.Ins3( m_p[ic1]);
	}
	return mm;
}

//	トレース
void MgGPoint3::Print( MCHAR* s) const
{
#ifdef _DEBUG
	Msprintf( MbLog::m_Str, Mstr( "%s	MgPolyg2	サイズ: %d, 頂点数: %d\n"), s, m_isz, m_n);
	MBTRCPRBF;
	for (int ic=0; ic<m_n; ic++) {
		Msprintf( MbLog::m_Str, Mstr( "			P[%2d]: (%7.1f,%7.1f)\n"),
						  ic, m_p[ic].x, m_p[ic].y);
		MBTRCPRBF;
	}
#endif
}

} // namespace MC

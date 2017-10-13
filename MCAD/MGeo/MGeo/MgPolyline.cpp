//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgPolyline.cpp
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"
#include "MbDebug.h"

#define	DLL_EXPORT_POLYLINE_DO

#include "MgDefine.h"
#include "MgTol.h"
#include "MgDefine.h"
#include "MgPlane.h"
#include "MgPolyline.h"

namespace MC
{

//
//======================( ２次元 )==============================
//		折れ線（点群）
//
//	構成点の領域をmallocで確保する場合のコンストラクタ
MgPolyl2::MgPolyl2( MINT sz1)
{
	m_tp = MGTP_POLYG;
	m_isz = sz1;
	m_n = 0;
	m_fa = 1;
	MBMALLOC( m_p, MgPoint2, sz1);
}

//	構成点の領域を配列で与える場合のコンストラクタ
MgPolyl2::MgPolyl2( MINT sz1, MgPoint2 *p2)
{
	m_tp = MGTP_POLYG;
	m_isz = sz1;
	m_n = 0;
	m_fa = 0;
	m_p = p2;
}

//	構成点を()で指定して読み書きする
const MgPoint2& MgPolyl2::operator () ( MINT idx) const
{
	if ( idx >= m_n)
		Base::MbSysError( MBCstr( "MgPolyl2 ()"), MC_ERR_OVERFLOW);
	return m_p[idx];
}

MgPoint2& MgPolyl2::operator () ( MINT idx)
{
	if ( idx >= m_isz)
		Base::MbSysError( MBCstr( "MgPolyl2 ()"), MC_ERR_OVERFLOW);
	return m_p[idx];
}

//	複写
void MgPolyl2::operator = ( const MgPolyl2& Pg)
{
	MINT	nn = Pg.m_n;
	if ( nn > m_isz) {
		if ( m_fa) MBFREE( m_p);								//	構成点の領域をmallocで確保していた場合は一旦free
		m_isz = nn;
		m_fa = 1;
		MBMALLOC( m_p, MgPoint2, nn);			//	構成点の領域をmallocで確保
	}
	m_n = nn;
	memcpy( m_p, Pg.m_p, sizeof( MgPoint2) * nn);
}

void MgPolyl2::operator = ( const MgPolyl3& Pg)
{
	MINT	ic1;
	MINT	nn = Pg.m_n;
	if ( nn > m_isz) {
		if ( m_fa) MBFREE( m_p);								//	構成点の領域をmallocで確保していた場合は一旦free
		m_isz = nn;
		m_fa = 1;
		MBMALLOC( m_p, MgPoint2, nn);			//	構成点の領域をmallocで確保
	}
	m_n = nn;
	for ( ic1 = 0; ic1<nn; ic1++)
		m_p[ic1] = MgPoint2C( Pg.m_p[ic1]);  
}

//	構成点の追加
void MgPolyl2::operator += ( const MgPoint2& p1)				// Pg += Pt1
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

//	折れ線（点群）の追加
void MgPolyl2::operator += ( const MgPolyl2 &pg)				// Pg += Pt1
{
	MINT	ic;
	MINT	nn = m_n + pg.m_n;
	if ( nn >= m_isz)
		Resize( nn);
	for ( ic=0; ic<pg.m_n; ic++)
		m_p[m_n++] = pg.m_p[ic];
}

//	３次元の折れ線より２次元の折れ線を作成
void MgPolyl2::Set( const MgPolyl3 &Pg)
{
	MINT	ic1;
	MINT	nn = Pg.m_n;
	if ( nn > m_isz) {
		if ( m_fa) MBFREE( m_p);								//	構成点の領域をmallocで確保していた場合は一旦free
		m_isz = nn;
		m_fa = 1;
		MBMALLOC( m_p, MgPoint2, nn);			//	構成点の領域をmallocで確保
	}
	m_n = nn;
	for ( ic1 = 0; ic1<nn; ic1++)
		m_p[ic1] = MgPoint2C( Pg.m_p[ic1]);  
}

//	回転方向を得る(MC_LEFT_ROT:左回転, MC_RIGHT_ROT:右回転)
MINT MgPolyl2::Rotation() const
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
		Pc = m_p[ic];												// 原点から各頂点へのベクトルの外積の合計を求め
		s += (Pb ^ Pc);											// 回転方向を考慮した折れ線の面積*2を求める
		Pb = Pc;
	}
	if (s > g_gTol.D_2) {
		ist = MC_LEFT_ROT;
	} else if (s < -g_gTol.D_2) {
		ist = MC_RIGHT_ROT;
	}							
	return ist;
}

//	折れ線の回転方向の反転
void MgPolyl2::Rev()
{
	MINT	ic1, ic2;
	ic2 = m_n - 1;
	for ( ic1=0; ic1<ic2; ic1++) {
		MSWAP( m_p[ic1], m_p[ic2], MgPoint2);
		ic2--;
	}
}

//	折れ線領域の拡張
void MgPolyl2::Resize( MINT szn)
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

//	折れ線領域の面積
MREAL MgPolyl2::Area()
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
MgMinMaxR2 MgPolyl2::MinMax()
{
	MINT	ic1;
	MgMinMaxR2	mm = MgMinMaxR2( MREALMAX, MREALMAX, MREALMIN, MREALMIN);

	for ( ic1=0; ic1<m_n; ic1++) {
		mm.Ins2( m_p[ic1]);
	}
	return mm;
}

//	トレース
void MgPolyl2::Print( MCHAR* s) const
{
#ifdef _DEBUG
	Msprintf( MbLog::m_Str, Mstr("%s	MgPolyl2	サイズ: %d, 頂点数: %d\n"), s, m_isz, m_n);
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
//		折れ線（点群）
//
//	構成点の領域をmallocで確保する場合のコンストラクタ
MgPolyl3::MgPolyl3( MINT sz1)
{
	m_tp = MGTP_POLYG3;
	m_isz = sz1;
	m_n = 0;
	m_fa = 1;
	MBMALLOC( m_p, MgPoint3, sz1);
}

//	構成点の領域を配列で与える場合のコンストラクタ
MgPolyl3::MgPolyl3( MINT sz1, MgPoint3 *p2)
{
	m_tp = MGTP_POLYG3;
	m_isz = sz1;
	m_n = 0;
	m_fa = 0;
	m_p = p2;
}

//	構成点を()で指定して読み書きする
const MgPoint3& MgPolyl3::operator () (MINT idx) const
{
	if ( idx >= m_isz)
		Base::MbSysError( MBCstr( "MgPolyl3 ()"), MC_ERR_OVERFLOW);
	return m_p[idx];
}

MgPoint3& MgPolyl3::operator () (MINT idx)
{
	if ( idx >= m_isz)
		Base::MbSysError( MBCstr( "MgPolyl3 ()"), MC_ERR_OVERFLOW);
	return m_p[idx];
}

//	複写
void MgPolyl3::operator = ( const MgPolyl3& Pg)
{
	MINT	nn = Pg.m_n;
	if ( nn > m_isz) {
		if ( m_fa) MBFREE( m_p);								//	構成点の領域をmallocで確保していた場合は一旦free
		m_isz = nn;
		m_fa = 1;
		MBMALLOC( m_p, MgPoint3, nn);							//	構成点の領域をmallocで確保
	}
	memcpy( m_p, Pg.m_p, sizeof( MgPoint3) * nn);
	m_n = nn;
}

void MgPolyl3::operator = (const MgPolyl2 &pg)					// ２次元→３次元 (z = 0.)
{
	MINT	ic1;
	MINT	nn = pg.m_n;
	if ( nn > m_isz) {
		if (m_fa) MBFREE( m_p);									//	構成点の領域をmallocで確保していた場合は一旦free
		m_isz = nn;
		m_fa = 1;
		MBMALLOC( m_p, MgPoint3, nn);							//	構成点の領域をmallocで確保
	}
	m_n = nn;
	for ( ic1 = 0; ic1<nn; ic1++)
		m_p[ic1] = MgPoint3C( pg.m_p[ic1], 0.);  
}

//	構成点の追加
void MgPolyl3::operator += ( const MgPoint3& p1)				// Pg += Pt1
{
	MgPoint3	p1sv;
	if (m_n >= m_isz) {
		p1sv = p1;												// ソースがリサイズする元の領域にあるデータの場合を考慮
		Resize( m_isz + MGMAX( m_isz, 4));							// ２倍に領域拡張(拡張後の最低領域サイズは４)
		m_p[m_n++] = p1sv;
	} else {
		m_p[m_n++] = p1;
	}
}

//	折れ線（点群）の追加
void MgPolyl3::operator += ( const MgPolyl3 &pg)				// Pg += Pt1
{
	MINT	ic;
	MINT	nn = m_n + pg.m_n;
	if ( nn >= m_isz)
		Resize( nn);					//
	for ( ic=0; ic<pg.m_n; ic++)
		m_p[m_n++] = pg.m_p[ic];
}

//	２次元の折れ線より３次元の折れ線を作成
void MgPolyl3::Set( const MgPolyl2 &pg, MREAL z)				// ２次元→３次元代入用 (z = 0.)
{
	MINT	ic1;
	MINT	nn = pg.m_n;
	if ( nn > m_isz) {
		if ( m_fa) MBFREE( m_p);								//	構成点の領域をmallocで確保していた場合は一旦free
		m_isz = nn;
		m_fa = 1;
		MBMALLOC( m_p, MgPoint3, nn);							//	構成点の領域をmallocで確保
	}
	m_n = nn;
	for ( ic1 = 0; ic1<nn; ic1++)
		m_p[ic1].Set( pg.m_p[ic1], z);  
}

void MgPolyl3::Set( const MgPolyl2 &pg, const MgPlane3& Ply)	// ２次元→３次元代入用
{
	MINT	ic1;
	MINT	nn = pg.m_n;
	if ( nn > m_isz) {
		if ( m_fa) MBFREE( m_p);								//	構成点の領域をmallocで確保していた場合は一旦free
		m_isz = nn;
		m_fa = 1;
		MBMALLOC( m_p, MgPoint3, nn);							//	構成点の領域をmallocで確保
	}
	m_n = nn;
	for ( ic1 = 0; ic1<nn; ic1++)
		m_p[ic1].Set( pg.m_p[ic1], Ply);  
}

//	折れ線の回転方向の反転
void MgPolyl3::Rev()
{
	MINT	ic1, ic2;
	ic2 = m_n - 1;
	for ( ic1=0; ic1<ic2; ic1++) {
		MSWAP( m_p[ic1], m_p[ic2], MgPoint3);
		ic2--;
	}
}

//	折れ線領域の拡張
void MgPolyl3::Resize( MINT szn)
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
		MgPoint3	*pb = m_p;
		MBMALLOC( m_p, MgPoint3, szn);		 					//	構成点の新領域をmallocで確保
		if ( m_n)
			memcpy( m_p, pb, sizeof( MgPoint3) * m_n);				//	構成点の領域にデータがあった場合は新領域へコピーする
		if (m_fa && pb)											//	構成点の領域をmallocで確保していた場合はその領域を開放する
			delete (char*)pb;											//
		m_fa = 1;
		m_isz = szn;
	}
}

//	最大最小を求める
MgMinMaxR3 MgPolyl3::MinMax()
{
	MINT	ic1;
	MgMinMaxR3	mm = MgMinMaxR3( MREALMAX, MREALMAX, MREALMAX, MREALMIN, MREALMIN, MREALMIN);

	for ( ic1=0; ic1<m_n; ic1++) {
		mm.Ins3( m_p[ic1]);
	}
	return mm;
}

//	トレース
void MgPolyl3::Print( MCHAR* s) const							// Print
{
#ifdef _DEBUG
	Msprintf( MbLog::m_Str, Mstr( "%s	MgPolyl3 サイズ: %d, 頂点数: %d\n"), s, m_isz, m_n);
	MBTRCPRBF;
	for (int ic=0; ic<m_n; ic++) {
		Msprintf( MbLog::m_Str, Mstr( "			P[%2d]: (%7.1f,%7.1f,%7.1f)\n"),
									ic, m_p[ic].x, m_p[ic].y, m_p[ic].z);
		MBTRCPRBF;
	}
#endif
}

//
//======================( ２次元 )==============================
//		折れ線群
//
//	構成折れ線の領域をmallocで確保する場合のコンストラクタ
MgGPolyl2::MgGPolyl2( MINT sz1)
{
	m_tp = MGTP_GPOLYG; 
	m_isz = sz1;
	m_n = 0;
	m_fa = 1;
	MBMALLOC( m_pg, MgPolyl2, sz1);
	memset( m_pg, 0, sizeof( MgPolyl2) * sz1);
}

//	構成折れ線の領域を配列で与える場合のコンストラクタ
MgGPolyl2::MgGPolyl2( MINT sz1, MgPolyl2 *pg2)
{
	m_tp = MGTP_GPOLYG; 
	m_isz = sz1;
	m_n = 0;
	m_fa = 0;
	m_pg = pg2;
	memset( m_pg, 0, sizeof( MgPolyl2) * sz1);
}

//	デストラクタ
MgGPolyl2::~MgGPolyl2()
{
	MINT	ic;
	for ( ic=0; ic<m_n; ic++)
		m_pg[ic].Free();
	if ( m_fa)
		MBFREE( m_pg);
}

//	領域開放
void MgGPolyl2::Free()
{
	MINT	ic;
	for ( ic=0; ic<m_n; ic++)
		m_pg[ic].Free();
	if ( m_fa)
		MBFREE( m_pg);
	m_isz = m_n = m_fa = 0;
}

//	構成折れ線を()で指定し読み書きする
const MgPolyl2& MgGPolyl2::operator () ( MINT idx) const
{
	if ( idx >= m_n)
		Base::MbSysError( MBCstr( "MgGPolyl2 ()"), MC_ERR_OVERFLOW);
	return m_pg[idx];
}

MgPolyl2& MgGPolyl2::operator () ( MINT idx)
{
	if ( idx >= m_isz)
		Base::MbSysError( MBCstr( "MgGPolyl2 ()"), MC_ERR_OVERFLOW);
	for (; m_n<=idx; m_n++)
		m_pg[m_n].SetInit();									// 最終データより後のデータはクリアし、n = idx+1　にする
	return m_pg[idx];
}

//	複写
void MgGPolyl2::operator = ( const MgGPolyl2 &GPg)
{
	MINT	ic;
	MINT	nn = GPg.m_n;
	if ( nn > m_isz) {
		MINT	szb = m_isz;
		MINT	sza = nn - m_isz;
		Resize( nn);
		memset( &m_pg[szb], 0, sizeof( MgPolyl2) * sza);			//	.Init
		m_isz = nn;
	}
	for ( ic=0; ic<nn; ic++)
		m_pg[ic] = GPg.m_pg[ic];
	m_n = nn;
}

void MgGPolyl2::operator = (const MgGPolyl3 &GPg)			// ３次元→２次元
{
	MINT	ic;
	MINT	nn = GPg.m_n;
	if ( nn > m_isz) {
		MINT	szb = m_isz;
		MINT	sza = nn - m_isz;
		Resize( nn);
		memset( &m_pg[szb], 0, sizeof( MgPolyl2) * sza);			//	.Init
		m_isz = nn;
	}
	for ( ic=0; ic<nn; ic++)
		m_pg[ic] = GPg.m_pg[ic];
	m_n = nn;
}

//	構成折れ線の追加
void MgGPolyl2::operator += ( const MgPolyl2 &Pg1)				// GPg += Pg1
{
	if (m_n >= m_isz) {
		MINT	sza = MGMAX( m_isz, 4);							// ２倍に領域拡張(拡張後の最低領域サイズは４)
		MINT	szb = m_isz;
		MINT	szn = m_isz + sza;
		Resize( szn);
		memset( &m_pg[szb], 0, sizeof( MgPolyl2) * sza);		//	.Init
	}
	m_pg[m_n++] = Pg1;
}

//	折れ線群の追加
void MgGPolyl2::operator += ( const MgGPolyl2 &GPg)				// GPg += GPg1
{
	MINT	ic;
	MINT	nn = m_n + GPg.m_n;
//
	if ( nn > m_isz) {
		MINT	szb = m_isz;
		MINT	sza = nn - m_isz;
		Resize( nn);
		memset( &m_pg[szb], 0, sizeof( MgPolyl2) * sza);		//	.Init
	}
	for ( ic=0; ic<GPg.m_n; ic++)
		m_pg[m_n++] = GPg.m_pg[ic];
}


//	３次元の折れ線より２次元の折れ線を作成
void MgGPolyl2::Set( const MgGPolyl3 &GPg)						// ３次元→２次元代入用
{
	MINT	ic;
	MINT	nn = GPg.m_n;
	if ( nn > m_isz) {
		MINT	szb = m_isz;
		MINT	sza = nn - m_isz;
		Resize( nn);
		memset( &m_pg[szb], 0, sizeof( MgPolyl2) * sza);		//	.Init
		m_isz = nn;
	}
	for ( ic=0; ic<nn; ic++)
		m_pg[ic] = GPg.m_pg[ic];
	m_n = nn;
}

//	空折れ線の追加
void MgGPolyl2::AddPolyg()
{
	MINT	nn = m_n++;
	if ( nn > m_isz) {
		MINT	sza = MGMAX( m_isz, 4);							// ２倍に領域拡張(拡張後の最低領域サイズは４)
		MINT	szb = m_isz;
		Resize( m_isz + sza);
		memset( &m_pg[szb], 0, sizeof( MgPolyl2) * sza);		//	.Init
	}
	m_n = nn;
}

//	全ての折れ線の回転方向の反転
void MgGPolyl2::Rev()
{
	MINT	ic1, ic2, ic3;
	MgPolyl2	*pg1;
	for ( ic1=0; ic1<m_n; ic1++) {
		pg1 = &m_pg[ic1];
		ic3 = pg1->m_n - 1;
		for ( ic2=0; ic2<ic3; ic2++) {
			MSWAP( pg1->m_p[ic2], pg1->m_p[ic3], MgPoint2);
			ic3--;
		}
	}
}

//	折れ線群領域の拡張(拡張部分は未クリア)
void MgGPolyl2::Resize( MINT szn)
{
	//if (szn > sz) {
	//	if (fa && pg) {											//	構成折れ線の領域をmallocで確保している場合
	//		MGREALLOC( pg, MgPolyl2, szn);						//		構成折れ線の領域をreallocで拡張
	//	} else {												//	構成折れ線の領域を未確保または配列の場合
	//		MgPolyl2		*pgb = pg;
	//		MBMALLOC( pg, MgPolyl2, szn);						//		構成折れ線の領域をmallocで確保
	//		if (!fa) memcpy( pg, pgb, sizeof( MgPolyl2) * n);	//	構成折れ線の領域が配列であった場合はmalloc領域へコピーする
	//		fa = 1;                                             
	//	}
	//	sz = szn;
	//}

	if (szn > m_isz) {
		MgPolyl2	*pgb = m_pg;
		MBMALLOC( m_pg, MgPolyl2, szn);			 				//	構成点の新領域をmallocで確保
		if ( m_n)
			 memcpy( m_pg, pgb, m_n * sizeof( MgPolyl2));			//	構成点の領域にデータがあった場合は新領域へコピーする
		if (m_fa && pgb)											//	構成点の領域をmallocで確保していた場合はその領域を開放する
			delete (char*)pgb;											//	
		m_fa = 1;
		m_isz = szn;
	}
}

//	トレース
void MgGPolyl2::Print( MCHAR* s) const							// Print
{
#ifdef _DEBUG
	Msprintf( MbLog::m_Str, Mstr( "%s	MgGPolyl2	サイズ: %d, ポリゴン数: %d\n"), s, m_isz, m_n);
	MBTRCPRBF;
	for (int ic=0; ic<m_n; ic++)
		m_pg[ic].Print( Mstr( "MgGPolyl2"));
#endif
}

//
//======================( ３次元 )==============================
//		折れ線群
//
//	構成折れ線の領域をmallocで確保する場合のコンストラクタ
MgGPolyl3::MgGPolyl3( MINT sz1)
{
	m_tp = MGTP_GPOLYG3; 
	m_isz = sz1;
	m_n = 0;
	m_fa = 1;
	MBMALLOC( m_pg, MgPolyl3, sz1);
	memset( m_pg, 0, sizeof( MgPolyl3) * sz1);
}

//	構成折れ線の領域を配列で与える場合のコンストラクタ
MgGPolyl3::MgGPolyl3( MINT sz1, MgPolyl3 *pg2)
{
	m_tp = MGTP_GPOLYG3; 
	m_isz = sz1;
	m_n = 0;
	m_fa = 0;
	m_pg = pg2;
	memset( m_pg, 0, sizeof( MgPolyl3) * sz1);
}

//	デストラクタ
MgGPolyl3::~MgGPolyl3()
{
	MINT	ic;
	for ( ic=0; ic<m_n; ic++)
		m_pg[ic].Free();
	if ( m_fa)
		MBFREE( m_pg);
}

//	領域開放
void MgGPolyl3::Free()
{
	MINT	ic;
	for ( ic=0; ic<m_n; ic++)
		m_pg[ic].Free();
	if ( m_fa)
		MBFREE( m_pg);
	m_isz = m_n = m_fa = 0;

}

//	構成折れ線を()で指定し読み書きする
const MgPolyl3& MgGPolyl3::operator () ( MINT idx) const
{
	if ( idx >= m_n)
		Base::MbSysError( MBCstr( "MgGPolyl3 ()"), MC_ERR_OVERFLOW);
	return m_pg[idx];
};

MgPolyl3& MgGPolyl3::operator () ( MINT idx)
{
	if ( idx >= m_isz)
		Base::MbSysError( MBCstr( "MgGPolyl3 ()"), MC_ERR_OVERFLOW);
	for (; m_n<=idx; m_n++)
		m_pg[m_n].SetInit();										//  最終データより後のデータはクリアし、n = idx+1　にする
	return m_pg[idx];
};

//	複写
void MgGPolyl3::operator = ( const MgGPolyl3 &GPg)
{
	MINT	ic;
	MINT	nn = GPg.m_n;
	if ( nn > m_isz) {
		MINT	szb = m_isz;
		MINT	sza = nn - m_isz;
		Resize( nn);
		memset( &m_pg[szb], 0, sizeof( MgPolyl3) * sza);			//	.Init
	}
	for ( ic=0; ic<nn; ic++)
		m_pg[ic] = GPg.m_pg[ic];
	m_n = nn;
}

void MgGPolyl3::operator = (const MgGPolyl2 &Gpg)				// ２次元→３次元 (z = 0.)
{
	MINT	ic;
	MINT	nn = Gpg.m_n;
	if ( nn > m_isz) {
		MINT	szb = m_isz;
		MINT	sza = nn - m_isz;
		Resize( nn);
		memset( &m_pg[szb], 0, sizeof( MgPolyl2) * sza);		//	.Init
		m_isz = nn;
	}
	for ( ic=0; ic<nn; ic++)
		m_pg[ic] = Gpg.m_pg[ic];
	m_n = nn;
}

//	構成折れ線の追加
void MgGPolyl3::operator += ( const MgPolyl3 &Pg1)				// GPg += Pg1
{
	if (m_n >= m_isz) {
		MINT	sza = MGMAX( m_isz, 4);							// ２倍に領域拡張(拡張後の最低領域サイズは４)
		MINT	szb = m_isz;
		MINT	szn = m_isz + sza;
		Resize( szn);
		memset( &m_pg[szb], 0, sizeof( MgPolyl3) * sza);		//	.Init
	}
	m_pg[m_n++] = Pg1;
}

//	折れ線群の追加
void MgGPolyl3::operator += ( const MgGPolyl3 &GPg)				// GPg += GPg1
{
	MINT	ic;
	MINT	nn = m_n + GPg.m_n;
//
	if ( nn > m_isz) {
		MINT	szb = m_isz;
		MINT	sza = nn - m_isz;
		Resize( nn);
		memset( &m_pg[szb], 0, sizeof( MgPolyl3) * sza);		//	.Init
	}
	for ( ic=0; ic<GPg.m_n; ic++)
		m_pg[m_n++] = GPg.m_pg[ic];
}

//	２次元の折れ線群より３次元の折れ線群を作成
void MgGPolyl3::Set( const MgGPolyl2 &Gpg, MREAL z)			// ２次元→３次元代入用 (z = 0.)
{
	MINT	ic;
	MINT	nn = Gpg.m_n;
	if ( nn > m_isz) {
		MINT	szb = m_isz;
		MINT	sza = nn - m_isz;
		Resize( nn);
		memset( &m_pg[szb], 0, sizeof( MgPolyl2) * sza);		//	.Init
		m_isz = nn;
	}
	for ( ic=0; ic<nn; ic++)
		m_pg[ic].Set( Gpg.m_pg[ic], z);
	m_n = nn;
}

void MgGPolyl3::Set( const MgGPolyl2 &Gpg, const MgPlane3& Pln)	// ２次元→３次元代入用
{
	MINT	ic;
	MINT	nn = Gpg.m_n;
	if ( nn > m_isz) {
		MINT	szb = m_isz;
		MINT	sza = nn - m_isz;
		Resize( nn);
		memset( &m_pg[szb], 0, sizeof( MgPolyl2) * sza);		//	.Init
		m_isz = nn;
	}
	for ( ic=0; ic<nn; ic++)
		m_pg[ic].Set( Gpg.m_pg[ic], Pln);
	m_n = nn;
}

//	空折れ線の追加
void MgGPolyl3::AddPolyg()
{
	if (m_n >= m_isz) {
		MINT	sza = MGMAX( m_isz, 4);							// ２倍に領域拡張(拡張後の最低領域サイズは４)
		MINT	szb = m_isz;
		MINT	szn = m_isz + sza;
		Resize( szn);
		memset( &m_pg[szb], 0, sizeof( MgPolyl3) * sza);		//	.Init
	}
	m_n++;
}

//	全ての折れ線の回転方向の反転
void MgGPolyl3::Rev()
{
	MINT		ic1, ic2, ic3;
	MgPolyl3	*pg1;
	for ( ic1=0; ic1<m_n; ic1++) {
		pg1 = &m_pg[ic1];
		ic3 = pg1->m_n - 1;
		for ( ic2=0; ic2<ic3; ic2++) {
			MSWAP( pg1->m_p[ic2], pg1->m_p[ic3], MgPoint3);
			ic3--;
		}
	}
}

//	折れ線群領域の拡張
void MgGPolyl3::Resize( MINT szn)
{
	//if (szn > sz) {
	//	if (fa && pg) {											//	構成折れ線の領域をmallocで確保している場合
	//		MGREALLOC( pg, MgPolyl3, szn);						//		構成折れ線の領域をremallocで拡張
	//	} else {												//	構成折れ線の領域を未確保または配列の場合
	//		MgPolyl3	*pgb = pg;
	//		MBMALLOC( pg, MgPolyl3, szn);						//		構成折れ線の領域をmallocで確保
	//		if (!fa) memcpy( pg, pgb, sizeof( MgPolyl3) * n);	//	構成折れ線の領域が配列であった場合はmalloc領域へコピーする
	//		fa = 1;                                             
	//	}
	//	sz = szn;
	//}

	if (szn > m_isz) {
		MgPolyl3	*pgb = m_pg;
		MBMALLOC( m_pg, MgPolyl3, szn);		 					//	構成点の新領域をmallocで確保
		if ( m_n)
			 memcpy( m_pg, pgb, m_n * sizeof( MgPolyl3));			//	構成点の領域にデータがあった場合は新領域へコピーする
		if (m_fa && pgb)											//	構成点の領域をmallocで確保していた場合はその領域を開放する
			delete (char*)pgb;											//	
		m_fa = 1;
		m_isz = szn;
	}
}

//	トレース
void MgGPolyl3::Print( MCHAR* s) const							// Print
{
#ifdef _DEBUG
	Msprintf( MbLog::m_Str, Mstr( "%s	MgGPolyl3	サイズ: %d, ポリゴン数: %d\n"), s, m_isz, m_n);
	MBTRCPRBF;
	for (int ic=0; ic<m_n; ic++)
		m_pg[ic].Print( Mstr( "MgGPolyl3"));
#endif
}

} // namespace MC
#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MdFig.h
//
//		図形情報
//
//
#include "stdafx.h"
#include "MbBasic.h"
#include "MgDefine.h"
#include "MgPoint.h"
#include "MgRect.h"
#include "MgMat.h"

#define	DLL_EXPORT_MDFIG_DO

#include "MdModel.h"

#include "MdmDefine.h"
#include "MdAry.h"
#include "MdAryH.h"

#undef	inline_nu
#define inline_nu

namespace MC
{

/*
// プリミティブヘッダワードを作成する
inline_nu MINT MdPhd(  MUBYTE i_icd, MUBYTE i_itp, MUSHORT i_n)
{
	MDPHD	clsPhd;
	clsPhd.Init( i_icd, i_itp, i_n);
	return *(MINT*)&clsPhd;
} 

// CLASS MDPMTL

inline_nu MINT	MDPMTL::GetSz()	const						// 図形情報バッファサイズ
{ 
	return m_sz;
}
inline_nu MINT	MDPMTL::GetN() const						// 図形情報サイズ
{
	return m_n;
}
inline_nu MREAL*	MDPMTL::GetP( MINT i_ip)				// 図形情報ポインタ
{
	return &m_pmt[i_ip];
}

inline_nu void	MDPMTL::SetSz( MINT i_sz)					// 図形情報バッファサイズ
{
	m_sz = i_sz;
}
inline_nu void	MDPMTL::SetN( MINT i_n)						// 図形情報サイズ
{
	m_n = i_n;
}

// CLASS MDFIG

//	コンストラクタ
inline_nu MDFIG::MDFIG()									
{
	m_pPmtl = 0;
}

inline_nu MDFIG::MDFIG(
				 MINT		i_sz							// 図形情報サイズ
				)
{
	MBMALLOCF( MDPMTL, m_pPmtl, MINT, i_sz);
	m_pPmtl->SetSz( i_sz);
	m_pPmtl->SetN( 0);
}
*/

// 図形情報のバッファサイズを得る
inline_nu MINT	MDFIG::GetSz() const						// 図形情報バッファサイズ
{ 
	return ( m_pPmtl ? m_pPmtl->GetSz(): 0);
}

inline_nu MINT	MDFIG::GetN() const							// 図形情報サイズ
{
	return ( m_pPmtl ? m_pPmtl->GetN(): 0);
}

inline_nu MDPMTL* MDFIG::GetpPmtl()								// 図形情報
{ 
	return m_pPmtl;
}  

inline_nu void	MDFIG::SetSz( MINT i_sz)					// 図形情報バッファサイズ
{
	ASSERT( m_pPmtl);
	m_pPmtl->SetSz( i_sz);
}

inline_nu void	MDFIG::SetN( MINT i_n)						// 図形情報サイズ
{
	ASSERT( m_pPmtl);
	m_pPmtl->SetN( i_n);
}

inline_nu void	MDFIG::SetpPmtl( MDPMTL* i_pPmtl)				// 図形情報
{
	m_pPmtl = i_pPmtl;
}

// 図形要素
inline_nu MDPMT* MDFIG::GetpPmt( MDID i_idl)				// 図形要素
{ 
//	return ( (MDPMT*)m_pPmtl->GetpPmt( i_idl));
	return m_pPmtl->GetpPmt( i_idl);
}  

inline_nu MINT	MDFIG::GetPmtCd( MDID i_idl)				// 図形要素のコード
{
	return ( GetpPmt( i_idl)->GetCd());
}

inline_nu MINT	MDFIG::GetPmtDm( MDID i_idl)				// 図形要素の次元
{
	return ( GetpPmt( i_idl)->GetDm());
}

//inline_nu MINT	MDFIG::GetPmtTp( MDID i_idl)			// 図形要素のタイプ
//{
//	return ( GetpPmt( i_idl)->GetTp());
//}

inline_nu MINT	MDFIG::GetPmtN( MDID i_idl)					// 図形要素のサイズ
{
	return ( GetpPmt( i_idl)->GetN());
}

inline_nu MDID	MDFIG::GetNextPmt( MDID i_idl)				// 次の図形要素のIDL(配列番号)
{
	return ( i_idl + GetpPmt( i_idl)->GetN() + MP_SZ_MDHDR);
}

inline_nu void	MDFIG::SetHd( MDID i_idl, MINT i_icd,
							  MINT i_idm, MINT i_n)			// 図形要素のコード、次元、サイズ
{
	GetpPmt( i_idl)->SetCd( i_icd);
	GetpPmt( i_idl)->SetDm( i_idm);
	GetpPmt( i_idl)->SetN( i_n);
}

inline_nu void	MDFIG::SetPmtCd( MDID i_idl, MINT i_icd)	// 図形要素のコード
{
	return ( GetpPmt( i_idl)->SetCd( i_icd));
}

inline_nu void	MDFIG::SetPmtDm( MDID i_idl, MINT i_idm)	// 図形要素の次元
{
	return ( GetpPmt( i_idl)->SetDm( i_idm));
}

//inline_nu void	MDFIG::SetPmtTp( MDID i_idl, MINT i_itp)	// 図形要素のタイプ
//{
//	return ( GetpPmt( i_idl)->SetTp( i_itp));
//}

inline_nu void	MDFIG::SetPmtN( MDID i_idl, MINT i_n)		// 図形要素のサイズ
{
	return ( GetpPmt( i_idl)->SetN( i_n));
}

// 図形情報郡の最後の図形情報に要素を追加
//	バッファサイズが不足する場合は２倍に領域拡張(拡張後の最低領域サイズは４とする)
inline_nu void MDFIG::operator += ( const MREAL i_r)				// Fig -= i
{
	if ( m_pPmtl->GetN() >= m_pPmtl->GetSz()) {
		ReSize( m_pPmtl->GetSz() + MGMAX( m_pPmtl->GetSz(), 4));
	}															// ２倍に領域拡張(拡張後の最低領域サイズは４とする)
	m_pPmtl->m_dt[m_pPmtl->m_n++] = i_r;
}

//	複写				構造体群 = 構造体群
inline_nu void MDFIG::operator = ( const MDFIG& i_Fig)
{
	MINT	wi_n;
	MINT	w_sz;

	if ( i_Fig.m_pPmtl == 0) {
		if ( m_pPmtl)
			SetN( 0);
	} else {
		wi_n = i_Fig.m_pPmtl->m_n;
		w_sz = m_pPmtl ? m_pPmtl->m_sz: 0;

		if ( wi_n > w_sz) {
			MBFREE( m_pPmtl);									//	構成点の領域をmallocで確保していた場合は一旦free
			MBMALLOCF( MDPMTL, m_pPmtl, MINT, (wi_n + 1));		//	構成点の領域をmallocで確保
			SetSz( wi_n);
		}
		memcpy( m_pPmtl, i_Fig.m_pPmtl, SZMINT( wi_n + 1));
		SetN( wi_n);
	}
}

//	複写				構造体群 = 構造体群
inline_nu void MDFIG::operator = ( const MDPMT& i_Pmt)
{
	MINT	wi_n;
	MINT	w_sz;

	wi_n = i_Pmt.GetN() + 1;
	w_sz = m_pPmtl ? m_pPmtl->m_sz: 0;

	if ( wi_n > w_sz) {
		MBFREE( m_pPmtl);										//	構成点の領域をmallocで確保していた場合は一旦free
		MBMALLOCF( MDPMTL, m_pPmtl, MINT, wi_n);				//	構成点の領域をmallocで確保
		SetSz( i_Pmt.GetN());
	}
	memcpy( &((MREAL*)m_pPmtl)[1], &i_Pmt, SZMINT( wi_n));

	SetN( wi_n);
}

//	構造体群の追加		構造体群 += 構造体群
//	バッファサイズが不足する場合は２倍に領域拡張(拡張後の最低領域サイズは４とする)
inline_nu void MDFIG::operator += ( const MDFIG& i_Fig)			// Fig += Fig
{
	MINT	w_n;
	MINT	w_sz;

	if ( i_Fig.m_pPmtl && i_Fig.m_pPmtl->m_n) { 
		w_n = m_pPmtl->m_n + i_Fig.m_pPmtl->m_n - MP_SZ_MDHDR;


		if ( w_n > m_pPmtl->m_sz) {
			w_sz = MGMAX( ( m_pPmtl->m_sz + m_pPmtl->m_sz), w_n);
			ReSize( w_sz);										//	２倍に領域拡張(拡張後の最低領域サイズは４とする)
		}
		memcpy( &((MINT*)m_pPmtl)[m_pPmtl->m_n], 
				&((MINT*)i_Fig.m_pPmtl)[1],
				SZMINT( i_Fig.m_pPmtl->m_n - MP_SZ_MDHDR));
		SetN( w_n);
	}
}

//	構造体群の追加		構造体群 += 構造体群
//	バッファサイズが不足する場合は２倍に領域拡張(拡張後の最低領域サイズは４とする)
inline_nu void MDFIG::operator += ( const MDPMT& i_Pmt)			// Fig += Fig
{
	MINT	w_n;
	MINT	w_sz;

	if ( i_Pmt.GetN()) { 
		w_n = m_pPmtl->m_n + i_Pmt.GetN() - MP_SZ_MDHDR;

		if ( w_n > m_pPmtl->m_sz) {
			w_sz = MGMAX( ( m_pPmtl->m_sz + m_pPmtl->m_sz), w_n);
			ReSize( w_sz);										//	２倍に領域拡張(拡張後の最低領域サイズは４とする)
		}
		memcpy( &((MINT*)m_pPmtl)[m_pPmtl->m_n], 
				&((MINT*)&i_Pmt)[1],
				SZMINT( i_Pmt.GetN() - MP_SZ_MDHDR));
		SetN( w_n);
	}
}

////	構成構造体を()で指定して読み書きする
////	例　Gst1(n).p, Gst1(n).i1, Gst1(n).i2
//inline_nu T& MDFIG::operator () (MUSHORT i_idx)
//{
//	if ( i_idx == m_sz) {
//		ReSize( m_sz + MGMAX( m_sz, 4));						//	２倍に領域拡張(拡張後の最低領域サイズは４とする)
//	} else if ( idx > m_sz) {
//		MbSysError( MBCstr( "MdAryH()"), MC_ERR_OVERFLOW);
//	}
//	for (; m_n<=i_idx; n++) m_st[m_n].SetInit();				//	最終データより後のデータはクリアし、n = idx+1　にする
//	return m_st[i_idx];
//}

////	構造体群領域の確保
//inline_nu void MdAryH<T>::Alloc( MUSHORT i_sz)
//{
//	MBMALLOC( m_st, T, i_sz);									//	構成構造体の領域をmallocで確保
//	memset( m_st, 0, sizeof( T) * (i_sz));
//	m_sz = i_sz;
//}

//	構造体群領域の確保
inline_nu void MDFIG::Alloc( MINT i_sz)
{
	ASSERT( m_pPmtl == 0);

	MBMALLOCF( MDPMTL, m_pPmtl, MINT, i_sz);					//	構成点の領域をmallocで確保
	SetSz( i_sz);
}

//	構造体群領域のサイズ変更
inline_nu void MDFIG::ReSize( MINT i_sz)
{
	MDPMTL*	pFigs;

	if ( i_sz > m_pPmtl->m_sz) {
		MBMALLOCF( MDPMTL, pFigs, MINT, i_sz);					//	構成点の領域をmallocで確保
		memcpy( pFigs, m_pPmtl, SZMINT( i_sz));
		SetSz( i_sz);

		MBFREE( m_pPmtl);										//	構成点の領域をmallocで確保していた場合は一旦free
		m_pPmtl = pFigs;
	}
}

//	構造体群領域の確保
inline_nu void MDFIG::IncrN( MINT i_n)
{
	MINT	n = m_pPmtl->m_n + i_n;
	ReSize( n);
	m_pPmtl->m_n = n;
}

// 図形情報データ数を増やす
//	バッファサイズが不足する場合は２倍に領域拡張(拡張後の最低領域サイズは４とする)
inline_nu MDPMT* MDFIG::PmtAlloc( MINT i_n)
{
	MINT	sv_n;
	MINT	w_n;
	MINT	w_sz;

	sv_n = m_pPmtl->m_n;
	w_n = sv_n + i_n;

	if ( w_n > m_pPmtl->m_sz) {
		w_sz = m_pPmtl->m_sz + MGMAX( w_n, 4);
		ReSize( w_sz);										//	２倍に領域拡張(拡張後の最低領域サイズは４とする)
	}
	m_pPmtl->m_n = w_n;
	return (MDPMT*)&m_pPmtl->m_dt[sv_n];
}

//	トレース
inline_nu void MDFIG::Print(MCHAR* i_s)
{
#ifdef _DEBUG
	if ( m_pPmtl) {
		Msprintf( MbLog::m_Str, Mstr( "\t\t\t%s < MDFIG > 整数型データセット，サイズ =%d，データ数 = %d\n"),
										i_s, m_pPmtl->m_sz, m_pPmtl->m_n);
		MBTRCPRBF;
		if ( m_pPmtl->m_n > 103) 
			 int iDumy = 1;

	}
//	MBTRCPRINTIN( Mstr( "\t\t\tデータ"), m_pPmtl->m_st, m_n);
#endif
}

} // namespace MC
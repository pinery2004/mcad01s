#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MdFigPmt.h
//
//		図形要素
//
//

#include "stdafx.h"
#include "MbBasic.h"
#include "MgDefine.h"
#include "MgPoint.h"
#include "MgRect.h"
#include "MgMat.h"

#define	DLL_EXPORT_MDFIG_DO

#include "MdZukei.h"
#include	"MdFig.h"
#include "MdModel.h"

#include "MdmDefine.h"
#include "MdAry.h"
#include "MdAryH.h"

#undef	inline_nu
#define inline_nu

namespace MC
{

//==========================================================================================
//	図形要素の追加
//	・
//
//==========================================================================================

// ==================== 図形属性 ==========================
// 属性プッシュ

inline_nu void	MDPMT::zPush( void)
{ 
	SetHd( MZAT_PUSH, 0, 0);
}

inline_nu void	MDFIG::zPush( void)
{ 
	GetpPmt( m_pPmtl->m_n)->SetHd( MZAT_PUSH, 0, MZWD_PUSH);
	IncrN( MZWD_PUSH + 1);
}

// 属性プル

inline_nu void	MDPMT::zPull( void)
{ 
	SetHd( MZAT_PULL, 0, 0);
}

inline_nu void	MDFIG::zPull( void)
{ 
	GetpPmt( m_pPmtl->m_n)->SetHd( MZAT_PULL, 0, MZWD_PULL);
	IncrN( MZWD_PULL + 1);
}

// マーカ

inline_nu void	MDPMT::zMarker(
						MINT	i_iTp,			// マーカタイプ
						MREAL	i_rSz			// マーカサイズ
				)
{ 
	SetHd( MZAT_MRKR, i_iTp, MZWD_MRKR);
	(MREAL&)m_dt[0] = i_rSz;
}

inline_nu void	MDFIG::zMarker(
						MINT	i_iTp,			// マーカタイプ
						MREAL	i_rSz			// マーカサイズ
				)
{ 
	GetpPmt( m_pPmtl->m_n)->zMarker( i_iTp, i_rSz);
	IncrN( MZWD_MRKR + 1);
}

// 塗りつぶし

inline_nu void	MDPMT::zPaint(
						MINT	i_iTp,			// 塗りつぶしタイプ ( 0:ホロー、1:ソリッド、2:ハッチング)
						MINT	i_iId,			// ハッチングパターンID
						MINT	i_iEdg			// エッジフラグ		( 0: OFF, 1: ON)
				)
{ 
	SetHd( MZAT_PAINT, i_iTp, MZWD_PAINT);
	(MUHALF&)m_dt[0] = MUHALF( i_iId, i_iEdg);
}

inline_nu void	MDFIG::zPaint(
						MINT	i_iTp,			// 塗りつぶしタイプ ( 0:ホロー、1:ソリッド、2:ハッチング)
						MINT	i_iId,			// ハッチングパターンID
						MINT	i_iEdg			// エッジフラグ		( 0: OFF, 1: ON)
				)
{ 
	GetpPmt( m_pPmtl->m_n)->zPaint( i_iTp, i_iId, i_iEdg);
	IncrN( MZWD_PAINT + 1);
}

// 線種

inline_nu void	MDPMT::zLineTp(
						MINT	i_iTp			// 線種
				)
{ 
	SetHd( MZAT_LINETP, i_iTp, 0);
}

inline_nu void	MDFIG::zLineTp(
						MINT	i_iTp			// 線種
				)
{ 
	GetpPmt( m_pPmtl->m_n)->zLineTp( i_iTp);
	IncrN( MZWD_LINETP + 1);
}

// 色( RGB) ( byte * 3)

inline_nu void	MDPMT::zColorRGB(
						MUQUARTER i_iRgb			// RGB	( 0, r, g, b)
				)
{ 
	SetHd( MZAT_COLOR, 0, MZWD_COLOR_I);
	(MUQUARTER&)m_dt[0] = i_iRgb;
}

inline_nu void	MDFIG::zColorRGB(
						MUQUARTER i_iRgb			// RGB	( 0, r, g, b)
				)
{ 
	GetpPmt( m_pPmtl->m_n)->zColorRGB( i_iRgb);
	IncrN( MZWD_COLOR_I + 1);
}

// 色( RGB) ( MFLOAT * 3)

inline_nu void	MDPMT::zColorRGB(
						FLOAT3&	i_f3Rgb		// RGB	( 1., 1., 1.)
				)
{ 
	SetHd( MZAT_COLOR, 1, MZWD_COLOR_R);
	(FLOAT3&)m_dt[0] = i_f3Rgb;
}

inline_nu void	MDFIG::zColorRGB(
						FLOAT3&	i_f3Rgb		// RGB	( 1., 1., 1.)
				)
{ 
	GetpPmt( m_pPmtl->m_n)->zColorRGB( i_f3Rgb);
	IncrN( MZWD_COLOR_R + 1);
}

// 色(パレット)

inline_nu void	MDPMT::zColorPlt(
						MINT	i_iPltN			// パレットＮｏ
				)
{ 
	SetHd( MZAT_COLOR, 2, MZWD_COLOR_P);
	(MINT&)m_dt[0] = i_iPltN;
}

inline_nu void	MDFIG::zColorPlt(
						MINT	i_iPltN			// パレットＮｏ
				)
{ 
	GetpPmt( m_pPmtl->m_n)->zColorPlt( i_iPltN);
	IncrN( MZWD_COLOR_P + 1);
}

// 透過率

inline_nu void	MDPMT::zTransmissivity(
						MFLOAT	i_fTransm		// 透過率 ( 0. ～ 1.)
				)
{ 
	SetHd( MZAT_TRANSM, 0, MZWD_TRANSM);
	(MREAL&)m_dt[0] = i_fTransm;
}

inline_nu void	MDFIG::zTransmissivity(
						MFLOAT	i_fTransm		// 透過率 ( 0. ～ 1.)
				)
{ 
	GetpPmt( m_pPmtl->m_n)->zTransmissivity( i_fTransm);
	IncrN( MZWD_TRANSM + 1);
}

// 線幅

inline_nu void	MDPMT::zLineWd(
						MFLOAT	i_rLWd			// 線幅
				)
{ 
	SetHd( MZAT_LINEWD, 0, MZWD_LINEWD);
	(MREAL&)m_dt[0] = i_rLWd;
}

inline_nu void	MDFIG::zLineWd(
						MFLOAT	i_rLWd			// 線幅
				)
{ 
	GetpPmt( m_pPmtl->m_n)->zLineWd( i_rLWd);
	IncrN( MZWD_LINEWD + 1);
}

// 材質( RGB) ( byte * 3)

inline_nu void	MDPMT::zMaterialRGBA(
						MINT	i_iMtN,			// 材質番号	( MC_AMBIENT:1		環境光の反射指定
												//			  MC_DIFFUSE:2		拡散光の反射指定
												//			  MC_SPECULAR:3		鏡面光の反射指定
												//			  MC_EMISSION:4		放射光の放射輝度
												//			  MC_AMBIENT_AND_DIFFUSE:5	環境光と拡散光)
						MUQUARTER i_iRgb		// RGB	( A{透過率}, R, G, B)
				)
{ 
	SetHd( MZAT_MTRL, MC_INTRGB | i_iMtN, MZWD_MTRL_I);
	(MUQUARTER&)m_dt[0] = i_iRgb;
}

inline_nu void	MDFIG::zMaterialRGBA(
						MINT	i_iMtN,			// 材質番号	( MC_AMBIENT:1		環境光の反射指定
												//			  MC_DIFFUSE:2		拡散光の反射指定
												//			  MC_SPECULAR:3		鏡面光の反射指定
												//			  MC_EMISSION:4		放射光の放射輝度
												//			  MC_AMBIENT_AND_DIFFUSE:5	環境光と拡散光)
						MUQUARTER i_iRgb		// RGB	( A{透過率}, R, G, B)
				)
{ 
	GetpPmt( m_pPmtl->m_n)->zMaterialRGBA( i_iMtN, i_iRgb);
	IncrN( MZWD_MTRL_I + 1);
}

// 材質( RGB) ( MFLOAT * 3)

inline_nu void	MDPMT::zMaterialRGBA(
						MINT	i_iMtN,			// 材質番号	( MC_AMBIENT:1		環境光の反射指定
												//			  MC_DIFFUSE:2		拡散光の反射指定
												//			  MC_SPECULAR:3		鏡面光の反射指定
												//			  MC_EMISSION:4		放射光の放射輝度
												//			  MC_AMBIENT_AND_DIFFUSE:5	環境光と拡散光)
						FLOAT3& i_f3Rgb,		// RGB		( ( 0.～1.), ( 0.～1.), ( 0.～1.))
						MFLOAT	 i_fAlpha		// A 透過率	( 0. ～ 1.)
				)
{ 
	SetHd( MZAT_MTRL, i_iMtN, MZWD_MTRL_R);
	(FLOAT3&)m_dt[0] = i_f3Rgb;
	m_dt[3] = i_fAlpha;
}

inline_nu void	MDFIG::zMaterialRGBA(
						MINT	i_iMtN,			// 材質番号	( MC_AMBIENT:1		環境光の反射指定
												//			  MC_DIFFUSE:2		拡散光の反射指定
												//			  MC_SPECULAR:3		鏡面光の反射指定
												//			  MC_EMISSION:4		放射光の放射輝度
												//			  MC_AMBIENT_AND_DIFFUSE:5	環境光と拡散光)
						FLOAT3& i_f3Rgb,		// RGB		( ( 0.～1.), ( 0.～1.), ( 0.～1.))
						MFLOAT	i_fAlpha		// A 透過率	( 0. ～ 1.)
				)
{ 
	GetpPmt( m_pPmtl->m_n)->zMaterialRGBA( i_iMtN, i_f3Rgb, i_fAlpha);
	IncrN( MZWD_MTRL_R + 1);
}

// 材質鏡面光(光沢)係数

inline_nu void	MDPMT::zMaterialShininess(
						MREAL	i_rShns			// 鏡面光係数 ( 0. ～ 128.)
				)
{ 
	SetHd( MZAT_MTRLSHINNS, 0, MZWD_MTRLSHINNS);
	(MREAL&)m_dt[0] = i_rShns;
}

inline_nu void	MDFIG::zMaterialShininess(
						MREAL	i_rShns			// 鏡面光係数 ( 0. ～ 128.)
				)
{ 
	GetpPmt( m_pPmtl->m_n)->zMaterialShininess( i_rShns);
	IncrN( MZWD_MTRLSHINNS + 1);
}

// テクスチャ

inline_nu void	MDPMT::zTexture(
						MINT		i_iTxtrId	// テクスチャＩＤ
				)
{ 
	SetHd( MZAT_TXTR, 0, MZWD_TXTR);
	(MINT&)m_dt[0] = i_iTxtrId;
}

inline_nu void	MDFIG::zTexture(
						MINT		i_iTxtrId	// テクスチャＩＤ
				)
{ 
	GetpPmt( m_pPmtl->m_n)->zTexture( i_iTxtrId);
	IncrN( MZWD_TXTR + 1);
}

// テクスチャ座標

inline_nu void	MDPMT::zTextureCoordinate(
						MINT		i_iMdCre,	// 座標生成モードA	( MC_EYE_PLANE:0	EYE_PLANE、
												//					  MC_OBJECT_PLANE:1	OBJECT_PLANE、
												//					  MC_2:環境マップ用)
						MINT		i_iMdCdn,	// 座標生成モードB	( MC_PLANE:0		面からベクトルを生成、
												//					  MC_1:指定したベクトル)
				const	MgVect2* 	i_pvszimg,	// イメージ寸法	(mm)( 横、縦) 
				const	MgMat3*		i_pmatH		// 配置マトリックス
				)
{ 
	SetHd( MZAT_TXTRC, 0, ((i_iMdCdn == 0) ? MZWD_TXTRC_S:MZWD_TXTRC_L) );
	(MHALF&)m_dt[0] = MHALF( i_iMdCre, i_iMdCdn);
	if ( i_pvszimg)
		(MgVect2&)m_dt[1] = *i_pvszimg;
	if ( i_pmatH)
		(MgMat3&)m_dt[3] = *i_pmatH;
}

inline_nu void	MDFIG::zTextureCoordinate(
						MINT		i_iMdCre,	// 座標生成モードA	( 0:EYE_PLANE、1:OBJECT_PLANE、2:環境マップ用)
						MINT		i_iMdCdn,	// 座標生成モードB	( 0:面からベクトルを生成、1:指定したベクトル)
				const	MgVect2* 	i_pvszimg,	// イメージ寸法	(mm)( 横、縦) 
				const	MgMat3*		i_pmatH		// 配置マトリックス
				)
{ 
	GetpPmt( m_pPmtl->m_n)->zTextureCoordinate( i_iMdCre, i_iMdCdn, i_pvszimg, i_pmatH);
	if ( i_iMdCdn = 0) {
		IncrN( MZWD_TXTRC_S + 1);
	} else {
		IncrN( MZWD_TXTRC_L + 1);
	}
}

// テクスチャパラメータ

inline_nu void	MDPMT::zTextureParam(
						MINT	i_iTp,			// 関数				( 0:ステッカー、1:変調、2:混合)
						MINT	i_iPxLap,		// ラップパラメータ	( 横、縦)	( 0:REPEAT、1:CLANP)
						MINT	i_iPyLap,		//
						MINT	i_iFltEx,		// 拡大／縮小フィルタ
						MINT	i_iFltRd,		//	( 0:くっきり、1:ぼやかす、
												//	  2:くっきり - くっきり
												//	  3:くっきり - ぼやかす
												//	  4:ぼやかす - くっきり
												//	  5:ぼやかす - ぼやかす)
						MREAL3& i_fColBlc1,		// 境界線色1		
						MREAL3& i_fColBlc2		// 境界線色2		
				)
{ 
	SetHd( MZAT_TXTPRM, i_iTp, MZWD_TXTPRM);
	(MQUARTER&)m_dt[0] = MQUARTER( i_iPxLap, i_iPyLap, i_iFltEx, i_iFltRd);
	(MREAL3&)m_dt[1] = i_fColBlc1;
	(MREAL3&)m_dt[4] = i_fColBlc2;
}

inline_nu void	MDFIG::zTextureParam(
						MINT	i_iTp,			// 関数				( 0:ステッカー、1:変調、2:混合)
						MINT	i_iPxLap,		// ラップパラメータ	( 横、縦)	( 0:REPEAT、1:CLANP)
						MINT	i_iPyLap,		//
						MINT	i_iFltEx,		// 拡大／縮小フィルタ
						MINT	i_iFltRd,		//	( 0:くっきり、1:ぼやかす、
												//	  2:くっきり - くっきり
												//	  3:くっきり - ぼやかす
												//	  4:ぼやかす - くっきり
												//	  5:ぼやかす - ぼやかす)
						MREAL3& i_fColBlc1,		// 境界線色1		
						MREAL3& i_fColBlc2		// 境界線色2		
				)
{ 
	GetpPmt( m_pPmtl->m_n)->zTextureParam( i_iTp, i_iPxLap, i_iPyLap, i_iFltEx, i_iFltRd,
										   i_fColBlc1,i_fColBlc2);
	IncrN( MZWD_TXTPRM + 1);
}

// 鏡面反射率
inline_nu void	MDPMT::zShininess(
						MREAL	i_fShns			// 鏡面反射率 ( 0. ～ 1.)
				)
{ 
	SetHd( MZAT_SHINNS, 0, MZWD_SHINNS);
	(MREAL&)m_dt[0] = i_fShns;
}

inline_nu void	MDFIG::zShininess(
						MREAL	i_fShns			// 鏡面反射率 ( 0. ～ 1.)
				)
{ 
	GetpPmt( m_pPmtl->m_n)->zShininess( i_fShns);
	IncrN( MZWD_SHINNS + 1);
}

// PovRay出力属性

inline_nu void	MDPMT::zPovRay(
						MCHAR*	i_pcStr			// 文字列
				)
{ 
	SetHd( MZAT_POVRAY, 0, MNWORD( i_pcStr));
	Mstrcpy( (MCHAR*)&m_dt[0], i_pcStr);
}

inline_nu void	MDFIG::zPovRay(
						MCHAR*	i_pcStr			// 文字列
				)
{ 
	GetpPmt( m_pPmtl->m_n)->zPovRay( i_pcStr);
	IncrN( MZWD_POVRAY + MNWORD( i_pcStr));
}

// ==================== 図形要素 ==========================
// 点
inline_nu void	MDPMT::Point3(
				const	MgPoint3& i_Pt)			// 点データ
{
	SetHd( MZCD_PNT, MZTP_3D, WSZMgPoint3());
	(MgPoint3&)m_dt[0] = i_Pt;
}

inline_nu void MDFIG::Point3(
				const	MgPoint3& i_Pt)			// 点データ
{
	GetpPmt( m_pPmtl->m_n)->Point3( i_Pt);
	IncrN( MZWD_PNT_3D + 1);
}

// 線分（3次元）
inline_nu void MDPMT::Line3(
				const	MgLine3& i_Ln)			// 線分データ
{
	SetHd( MZCD_LINE, MZTP_3D, MZWD_LINE_3D);
	(MgLine3&)m_dt[0] = i_Ln;
}

inline_nu void MDFIG::Line3(
				const	MgLine3& i_Ln)			// 線分データ
{
	GetpPmt( m_pPmtl->m_n)->Line3( i_Ln);
	IncrN( MZWD_LINE_3D + 1);
}

// 直線（3次元）
inline_nu void MDPMT::ULine3(
				const	MgULine3& i_uLn)		// 直線データ
{
	SetHd( MZCD_ULINE, MZTP_3D, MZWD_ULINE_3D);
	(MgULine3&)m_dt[0] = i_uLn;
}

inline_nu void MDFIG::ULine3(
				const	MgULine3& i_uLn)		// 直線データ
{
	GetpPmt( m_pPmtl->m_n)->ULine3( i_uLn);
	IncrN( MZWD_ULINE_3D + 1);
}

// 半直線（3次元）
inline_nu void MDPMT::HLine3(
				const	MgHLine3&	i_hLn)		// 半直線データ（3次元）
{
	SetHd( MZCD_HLINE, MZTP_3D, MZWD_HLINE_3D);
	(MgHLine3&)m_dt[0] = i_hLn;
}

inline_nu void MDFIG::HLine3(
				const	MgHLine3&	i_hLn)		// 半直線データ（3次元）
{
	GetpPmt( m_pPmtl->m_n)->HLine3( i_hLn);
	IncrN( MZWD_HLINE_3D + 1);
}

// 折れ線（3次元）
inline_nu void MDPMT::Polyl3(
				const	MgPolyl3& i_Ply)		// 折れ線（3次元）
{
	SetHd( MZCD_PLY, MZTP_3D, i_Ply.m_n * 3);
	MgPt3Cpy( (MgPoint3*)m_dt, i_Ply.m_n, i_Ply.m_p, i_Ply.m_n);
}

inline_nu void MDFIG::Polyl3(
				const	MgPolyl3& i_Ply)		// 折れ線（3次元）
{
	GetpPmt( m_pPmtl->m_n)->Polyl3( i_Ply);
	IncrN( i_Ply.m_n * 3 + 1);
}

// 多角形（3次元）
inline_nu void MDPMT::Polyg3(
				const	MgPolyg3& i_Pgn)		// 多角形（3次元）
{
	SetHd( MZCD_PGN, MZTP_3D, i_Pgn.m_n * 3);
	MgPt3Cpy( (MgPoint3*)m_dt, i_Pgn.m_n, i_Pgn.m_P, i_Pgn.m_n);
}

inline_nu void MDFIG::Polyg3(
				const	MgPolyg3& i_Pgn)		// 多角形（3次元）
{
	GetpPmt( m_pPmtl->m_n)->Polyg3( i_Pgn);
	IncrN( i_Pgn.m_n * 3 + 1);
}

// 円（3次元）
inline_nu void MDPMT::Cir3(
				const	MgCir3& i_Cir)			// 円（3次元）
{
	SetHd( MZCD_CIR, MZTP_3D, MZWD_CIR_3D);
	(MgCir3&)m_dt = i_Cir;
}

inline_nu void MDFIG::Cir3(
				const	MgCir3& i_Cir)			// 円（3次元）
{
	GetpPmt( m_pPmtl->m_n)->Cir3( i_Cir);
	IncrN( MZWD_CIR_3D + 1);
}

// 円弧（3次元）
//inline_nu MDZArc3::MDZArc3( MgArc3& i_Arc)
//{
//	SetHd( MZCD_ARC, MZTP_3D, WSZMgArc3());
//	m_Arc = i_Arc;
//}
inline_nu void MDPMT::Arc3(
				const	MgArc3& i_Arc)			// 円弧（3次元）
{
	SetHd( MZCD_ARC, MZTP_3D, MZWD_ARC_3D);
	(MgArc3&)m_dt = i_Arc;
}

inline_nu void MDFIG::Arc3(
				const	MgArc3& i_Arc)			// 円弧（3次元）
{
	GetpPmt( m_pPmtl->m_n)->Arc3( i_Arc);
	IncrN( MZWD_ARC_3D + 1);
}


//// 楕円（3次元）
//inline_nu MDZELPS3::MDZELPS3( MgSLine3 i_sLn)
//{
//	SetHd( MZELN3, MZCD_3D, 6);
//	m_sLn = i_sLn;
//}

//// 楕円弧（3次元）
//inline_nu MDZELA3::MDZELA3( MgSLine3 i_sLn)
//{
//	SetHd( MZELA3, MZCD_3D, 6);
//	m_sLn = i_sLn;
//}

// 面分（3次元）
inline_nu void MDPMT::Plane3(
				const	MgPlane3& i_Pln)			// 面分データ
{
	SetHd( MZCD_PLN, MZTP_3D, MZWD_PLN_3D);
	(MgPlane3&)m_dt[0] = i_Pln;
}

inline_nu void MDFIG::Plane3(
				const	MgPlane3& i_Pln)			// 面分データ
{
	GetpPmt(  m_pPmtl->m_n)->Plane3( i_Pln);
	IncrN( MZWD_PLN_3D + 1);
}

// 無限平面（3次元）
inline_nu void MDPMT::IPlane3(
				const	MgPlane3& i_iPln)			// 無限平面データ
{
	SetHd( MZCD_IPLN, MZTP_3D, MZWD_IPLN_3D);
	(MgPlane3&)m_dt[0] = i_iPln;
}

inline_nu void MDFIG::IPlane3(
				const	MgPlane3& i_iPln)			// 無限平面データ
{
	GetpPmt( m_pPmtl->m_n)->IPlane3( i_iPln);
	IncrN( MZWD_IPLN_3D + 1);
}

//// 円筒面（3次元）
//inline_nu MDZCYL3::MDZCYL3( MgSLine3 i_sLn)
//{
//	SetHd( MZSLINE3, MZCD_3D, 6);
//	m_sLn = i_sLn;
//}

//// ３次スプライン曲線の構成点
//inline_nu MDZSPLC3::MDZSPLC3( MgSLine3 i_sLn)
//{
//	SetHd( MZSLINE3, MZCD_3D, 6);
//	m_sLn = i_sLn;
//}
//
//// ３次スプライン曲線（3次元）
//inline_nu MDZSPL3::MDZSPL3( MgSLine3 i_sLn)
//{
//	SetHd( MZSLINE3, MZCD_3D, 6);00.
//	m_sLn = i_sLn;
//}
//
//// 寸法表示要素
//inline_nu MDZMESLC::MDZMESLC( MgSLine3 i_sLn)
//{
//	SetHd( MZSLINE3, MZCD_3D, 6);
//	m_sLn = i_sLn;
//}
//
//// 寸法表示(距離)
//inline_nu MDZMESL3::MDZMESL3( MgSLine3 i_sLn)
//{
//	SetHd( MZSLINE3, MZCD_3D, 6);
//	m_sLn = i_sLn;
//}
//
//
//// 寸法表示(角度)
//inline_nu MDZMESA3::MDZMESA3( MgSLine3 i_sLn)
//{
//	SetHd( MZSLINE3, MZCD_3D, 6);
//	m_sLn = i_sLn;
//}
//
//// 寸法表示(半径 | 直径)
//inline_nu MDZMESR3::MDZMESR3( MgSLine3 i_sLn)
//{
//	SetHd( MZSLINE3, MZCD_3D, 6);
//	m_sLn = i_sLn;
//}

//// ビットマップファイルヘッダ

//// ビットマップ情報ヘッダ

//// ビットマップカラーデータ
////	この構造体（大きさは4バイト）はビットマップのカラーデータを表すものだが、１つの構造体で
////	１つのカラーデータを表している。つまり、16色なら16個、256色なら256個のRGBQUAD構造体が並んでいる。
////	BITMAPFILEHEADER構造体のbfOffBitsの値はBITMAPINFOHEADER構造体のbiBitCountの値によって決定される。
////	例えばbiBitcount = 8の場合、そのビットマップは256色であるからRGB構造体は256個存在する。
////	よってbfOffBits = 14 + 40 + 4 * 256 = 1078 となる。
////	但し、16ビット(65536色)以上の場合はカラーパレットが不要なため、この構造体は使用されない。
////	常にbfOffBits = 54である。
////	参考	http://www.alpha-net.ne.jp/users2/uk413/vc/VCT_DIB.html
////			VC++テクニック : ディスク上のBMPファイルを表示

//// ビットマップ
//
//// 光源要素
//
//// 光源

} // namespace MC
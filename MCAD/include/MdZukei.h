#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MdZukei.h
//
//		ＭＣＡＤモデル
//		ディスプレイ管理
//	
//==========================================================================================
//
#include "MgMat.h"
#include "MgPlane.h"
#include "MgCircle.h"
#include "MgArc.h"

// サイズ
#define		MZWD_PNT_2D		2					// 点(２次元)ワードサイズ
#define		MZWD_PNT_3D		3					// 点(３次元)ワードサイズ
#define		MZWD_LINE_2D	4					// 線分(２次元)ワードサイズ
#define		MZWD_LINE_3D	6					// 線分(３次元)ワードサイズ
#define		MZWD_ULINE_2D	4					// 直線(２次元)ワードサイズ
#define		MZWD_ULINE_3D	6					// 直線(３次元)ワードサイズ
#define		MZWD_HLINE_2D	4					// 半直線(２次元)ワードサイズ
#define		MZWD_HLINE_3D	6					// 半直線(３次元)ワードサイズ
#define		MZWD_CIR_2D		3					// 円(２次元)ワードサイズ
#define		MZWD_CIR_3D		7					// 円(３次元)ワードサイズ
#define		MZWD_ARC_2D		8					// 円弧(２次元)ワードサイズ
#define		MZWD_ARC_3D		13					// 円弧(３次元)ワードサイズ
#define		MZWD_PLN_3D		4					// 面分(３次元)ワードサイズ
#define		MZWD_IPLN_3D	4					// 無限平面(３次元)ワードサイズ
#define		MZWD_PUSH		0					// Pushワードサイズ
#define		MZWD_PULL		0					// Pullワードサイズ
#define		MZWD_MRKR		1					// マーカ
#define		MZWD_PAINT		1					// 塗りつぶし
#define		MZWD_LINETP		0					// 線種
#define		MZWD_COLOR_I	1					// 色( RGB) 
#define		MZWD_COLOR_R	3					// 色( RGB) 
#define		MZWD_COLOR_P	1					// パレットＮｏ 色( RGB) 
#define		MZWD_TRANSM		1					// 透過率 ( 0. ～ 1.)
#define		MZWD_LINEWD		1					// 線幅
#define		MZWD_MTRL_I		1					// 材質( RGB)
#define		MZWD_MTRL_R		4					// 材質( RGB)
#define		MZWD_MTRLSHINNS	1					// 材質鏡面光( 光沢)係数
#define		MZWD_TXTR		1					// テクスチャ
#define		MZWD_TXTRC_S	1					// テクスチャ座標( TpCdn=0:面から座標を生成)
#define		MZWD_TXTRC_L	19					// テクスチャ座標( TpCdn=1:座標を指定)
#define		MZWD_TXTPRM		7					// テクスチャパラメータ
#define		MZWD_SHINNS		1					// 鏡面反射率
#define		MZWD_POVRAY		0					// PovRay出力属性

// 次元
#define		MZTP_3D			0x00				// ３次元
#define		MZTP_2D			0x80				// ２次元

// 図形PMTコード
#define		MZCD_PNT		0x01				// 点
#define		MZCD_PNTV		0x02				// 点
#define		MZCD_ULINE		0x03				// 直線
#define		MZCD_HLINE		0x04				// 半直線
#define		MZCD_LINE		0x05				// 線分
#define		MZCD_PLY		0x06				// 折れ線
#define		MZCD_PLYV		0x07				// 折れ線 VECTOR
#define		MZCD_PAC		0x08				// 線分|円弧 折れ線

#define		MZCD_RCT		0x09				// 長方形
#define		MZCD_PGN		0x0A				// 多角形
#define		MZCD_PGNV		0x0B				// 多角形 VECTOR
#define		MZCD_ARA		0x0C				// 閉領域

#define		MZCD_HRCT		0x0D				// 穴付長方形
#define		MZCD_HPLN		0x0E				// 穴付多角形
#define		MZCD_HARA		0x0F				// 穴付閉領域

#define		MZCD_CIR		0x10				// 円
#define		MZCD_ARC		0x11				// 円弧
#define		MZCD_ELN		0x12				// 楕円
#define		MZCD_ELA		0x13				// 楕円弧

#define		MZCD_SPLN		0x14				// スプライン曲線

#define		MZCD_PLN		0x1A				// 面分
#define		MZCD_IPLN		0x1B				// 無限平面

// 属性PMTコード
#define		MZAT_PUSH		0x20				// Push
#define		MZAT_PULL		0x21				// Pull

#define		MZAT_MRKR		0x22				// マーカ
#define		MZAT_PAINT		0x23				// 塗りつぶし
#define		MZAT_LINETP		0x24				// 線種
#define		MZAT_COLOR		0x25				// 色( RGB) 
#define		MZAT_TRANSM		0x26				// 透過率 ( 0. ～ 1.)
#define		MZAT_LINEWD		0x27				// 線幅
#define		MZAT_MTRL		0x28				// 材質( RGB)
#define		MZAT_MTRLSHINNS	0x29				// 材質鏡面光(光沢)係数
#define		MZAT_TXTR		0x2A				// テクスチャ
#define		MZAT_TXTRC		0x2B				// テクスチャ座標
#define		MZAT_TXTPRM		0x2C				// テクスチャパラメータ
#define		MZAT_SHINNS		0x2D				// 鏡面反射率
#define		MZAT_POVRAY		0x2E				// PovRay出力属性



#define		MAX_BUFFSIZE8		0				// 0xFF
#define		MAX_BUFFSIZE16		1				// 0xFFFF
#define		MAX_BUFFSIZE1M		2				// 1000000
#define		MAX_BUFFSIZE10M		3				// 10000000
#define		MAX_BUFFSIZE100M	4				// 100000000

#define		MC_SZ_FBF_MIN		20

namespace MC
{

inline void	MgByteCpy( void* out_ppt, size_t i_szpt, const void* in_ppt, size_t i_npt)
{
	memcpy_s( out_ppt, i_szpt, in_ppt, i_npt);
}

inline void	MgPt3Cpy( MgPoint3* out_pPt, MINT i_szPt, const MgPoint3* in_pPt, MINT i_nPt)
{
//	MINT	iC;
	ASSERT( i_nPt < i_szPt);
//	for ( iC=0; iC<i_nPt; iC++)
//		out_pPt[iC] = in_pPt[iC];
	memcpy( out_pPt, in_pPt, i_nPt * sizeof( MgPoint3));
}

inline void	MgPt2Cpy( MgPoint2* out_ppt, MINT i_szpt, const MgPoint2* in_ppt, MINT i_npt)
{
//	MINT	iC;
	ASSERT( i_npt < i_szpt);
//	for ( iC=0; iC<i_npt; iC++)
//		out_ppt[iC] = in_ppt[iC];
	memcpy( out_ppt, in_ppt, i_npt * sizeof( MgPoint2));
}

} // namespace MC
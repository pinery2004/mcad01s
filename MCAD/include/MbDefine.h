#pragma once
//==========================================================================================
//  Copyright ( C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MbDefine.h
//
//		MCADで使用する用語の定義
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================

//===========================================================================
// 図形種類
enum MFIGURETYPE {
	MPT_DOT			= 1,										// 点
	MPT_LINE,													// 線
	MPT_RECT,													// 折れ線
	MPT_POLYLINE,												// 長方形
	MPT_POLYGON,												// 多角形
	MPT_CIRCLE,													// 円
	MPT_ELLIPSE,												// 楕円
	MPT_ARC,													// 円弧
	MPT_TEXT													// 文字列
};

// 線種
enum MPENSTYLE {
	MPS_SOLID		= 0,										// 実線
	MPS_DASH		= 1,										// 破線
	MPS_DOT 		= 2,										// 点線
	MPS_DASHDOT 	= 3,										// １点鎖線
	MPS_DASHDOTDOT	= 4,										// ２点鎖線
	MPS_NULL		= 5											// 無色
};

// 領域塗りつぶし形式
enum MAREAFILL {
	MAF_NONE 		= 0,										// なし
	MAF_DIAGONAL,												// 対角線
	MAF_SLANTTORIGHT,											// 右下がりハッチング
	MAF_SLANTTOLEFT,											// 左下がりハッチング
	MAF_HORIZONTAL,												// 水平ハッチング
	MAF_VERTICAL,												// 垂直ハッチング
	MAF_CROSS,													// 水平垂直格子ハッチング
	MAF_SLANTCROSS												// 斜め格子ハッチング
};

// ビュー種類
enum MVIEWTYPE {
	MVIEWT_HEI = 1,												// 平面
	MVIEWT_SYO,													// 正面
	MVIEWT_HAI,													// 背面
	MVIEWT_LSK,													// 左側面
	MVIEWT_RSK,													// 右側面
	MVIEWT_URA,													// 裏面
	MVIEWT_3D													// ３次元
};

#define	MB_ESCAPECHAR	27
#define	MB_ENTERCHAR	13

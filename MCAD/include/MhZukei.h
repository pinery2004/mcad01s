#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MhZukei.h
//
//		図形情報
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#define		MHZK_DISP	2											// 図形表示用
#define		MHZK_AREA	1											// 図形領域判定用

#define		MHZK_GLINE	2											// 線分群
#define		MHZK_GPOINT	1											// 点群

namespace MC
{

//	図形データ
class	MhZukei
{
	friend	class	MhPlcInfo;
public:
	MUBYTE				m_obj;									// 使用目的
																//  MHZK_DISP	: 図形表示用
																//  MHZK_AREA	: 図形領域判定用
	MUBYTE				m_iZTp;									// 図形データタイプ
																//	MHZK_GLINE	: 線分群
																//	MHZK_GPOINT	: ポリゴン(点群)
	MSHORT				m_nZukei;								// 図形線分数
	MhZukei*			m_pNext;								// 追加図形線分
	MgLine3				m_lnZukei[1];

	MINT	Load(												// ＤＢより１レコード読み込み
						MhMdlIO*	i_phMdl						// 読み込みファイル　ハンドル
				);
	MINT	Save(												// ＤＢへ１レコード書き込み
						MhMdlIO*	i_phMdl						// 書き込みファイル　ハンドル
				);
};

} // namespace MC
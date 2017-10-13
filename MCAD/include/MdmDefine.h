#pragma once
//
//==========================================================================================
//  Copyright(C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MdmDefine.h
//
//		家モデル・データベースの定数
//
//
//  K.Matsu           03/01/09    Created.
//==========================================================================================

#define		MDC_ID_NULL				0							// 未定義ID

// 表示検知モード
//					An indication detection mode
//
#define		MDD_MD_INDICATION		2							// 表示モード
#define		MDD_MD_DETECTION		1							// 検出モード

#define		MDC_NONE_ID				-888						// IDなし
#define 	MDC_DELETE				0x7DCDCDC					// 削除ID

// 制限値
#define		MDMSZ_NMMODEL			40							// モデル名最大文字数


namespace MC
{

// ＭＤＢのＩＤを配列番号に変換する
inline MINT MIDtoHN( MINT i_id) { return ( i_id - 1); }
// ＭＤＢの配列番号をＩＤに変換する
inline MINT MHNtoID( MINT i_idS) { return ( i_idS + 1); }

} // namespace MC
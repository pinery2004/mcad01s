#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MmValid.h
//
//		ＭＣＡＤモデル
//						オプションと履歴の有効データのチェック
//
//==========================================================================================

#include "MhRoof.h"

namespace MC
{

// 部品配置のオプションと履歴のチェック
// 
inline bool MmChkValidParts( MhPlcInfo*	pPlcEn) 
{
	MsBitSet* pOptv1;
	MsBitSet* pOptv2;
	MsBitSet* pHstv;

	// オプションチェック
	pOptv1 = pPlcEn->GetPIOpt1();
	if ( MhOptV::MmOptChkC( pOptv1) &&
		 ( ( pOptv2 = pPlcEn->GetPIOpt2()) == NULL || !MhOptV::MmOptOffChkC( pOptv2)) &&

		// 履歴チェック
		MhHist::MmVerChkC( pHstv = pPlcEn->GetPIVer()))
		return true;
	else
		return false;
}

// 屋根配置のオプションと履歴のチェック
//
inline bool MmChkValidRoof( MhRoofInfo* i_pRoofEn) 
{
	MsBitSet* pOptv1;
	MsBitSet* pOptv2;
	MsBitSet* pHstv;

	// オプションチェック
	pOptv1 = i_pRoofEn->GetPIOpt1();
	if ( MhOptV::MmOptChkC( pOptv1) &&
		 ( ( pOptv2 = i_pRoofEn->GetPIOpt2()) == NULL || !MhOptV::MmOptOffChkC( pOptv2)) &&

	// 履歴チェック
		MhHist::MmVerChkC( pHstv = i_pRoofEn->GetPIVer()))
		return true;
	else
		return false;
}

} // namespace MC
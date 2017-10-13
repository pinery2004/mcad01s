//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MdGrp.cpp
//
//		モデルデータベース
//
//
//  K.Matsu           01/06/08    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"
#include "MbDebug.h"
#include "MgDefine.h"

#include "MdModel.h"
#define DLL_EXPORT_DGRP_DO
#include "MdmDB.h"
#include "Mdm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace MC
{

//===========================================================================
//		グループの生成
//
	MINT DGrp::CreateGrp(								//
					MCHAR		i_sName[],		// グループ名
					MREAL		i_rKt,			// 階高さ
					MREAL		i_rFl,			// 床高さ
					MDID*		o_pidGrp,		// グループId
					MINT		i_sz/*=10*/		// 初期グループIdバッファサイズ
				)
{
	MdmSetGRP*	pGrp;
	
	// GrpBfより空きのIDを取得し、グループの子のID設定用領域を設ける
	//
	if ( Mdm::GetCurModel()->m_GrpBf.m_idSpaceLast == 0) {
		// 削除して空きになっているIDがない場合は最終ID+1を使用する
		Mdm::GetCurModel()->m_GrpBf ++;
		*o_pidGrp = Mdm::GetCurModel()->m_GrpBf.m_n;
		pGrp = &Mdm::GetCurModel()->m_GrpBf.m_st[MIDtoHN( *o_pidGrp)];

	} else {
		// 削除して空きになっているIDがある場合はそのIDを使用する
		ASSERT( Mdm::GetCurModel()->m_GrpBf.m_nidSpace >= 1);

		*o_pidGrp = Mdm::GetCurModel()->m_GrpBf.m_idSpaceLast;
		pGrp = &Mdm::GetCurModel()->m_GrpBf.m_st[MIDtoHN( *o_pidGrp)];

		// 空きエリアへのIDを一つ戻す
		Mdm::GetCurModel()->m_GrpBf.m_idSpaceLast = MDSPACEBEFORE( pGrp);
		if ( Mdm::GetCurModel()->m_GrpBf.m_idSpaceLast)
			MDSPACENEXT( &Mdm::GetCurModel()->m_GrpBf.m_st[MIDtoHN( Mdm::GetCurModel()->m_GrpBf.m_idSpaceLast)]) = 0;
		Mdm::GetCurModel()->m_GrpBf.m_nidSpace--;
	}

	//	生成グループに子IDの設定用領域を確保する
	pGrp->m_GidC.Alloc( (int)i_sz);

	// グループ名、階高さ、床高さを設定する
	pGrp->SetInit( 0, i_sName, i_rKt, i_rFl);
	return 0;
}

//===========================================================================
//		グループの削除
//
MINT DGrp::DeleteGrp(								//
						MDID		i_idGrp		// グループId
				)
{
	MdmSetGRP*	pGrpB;							// 直前に削除したグループ
	MdmSetGRP*	pGrpC;							// 削除グループ

	ASSERT( MDISGRP( i_idGrp));

	// 削除し割り当て待ちのグループ数
	Mdm::GetCurModel()->m_GrpBf.m_nidSpace++;

	// グループ削除
	pGrpC = &Mdm::GetCurModel()->m_GrpBf.m_st[MIDtoHN( i_idGrp)];
	MDSPACEID( pGrpC) = MDID_DELETE;
	pGrpC->Free();

	// 空きエリアリンクに追加
	// 直前に削除したグループの次のグループとする
	if ( Mdm::GetCurModel()->m_GrpBf.m_idSpaceLast) {
		pGrpB = &Mdm::GetCurModel()->m_GrpBf.m_st[MIDtoHN( Mdm::GetCurModel()->m_GrpBf.m_idSpaceLast)];
		MDSPACENEXT( pGrpB) = i_idGrp;
	}

	// 空きエリアリンクの先頭にリンク付ける
	MDSPACEBEFORE( pGrpC) = Mdm::GetCurModel()->m_GrpBf.m_idSpaceLast;
	MDSPACENEXT( pGrpC) = 0;
	Mdm::GetCurModel()->m_GrpBf.m_idSpaceLast = i_idGrp;
	return 0;
}

//===========================================================================
//		グループの階高とFLの問い合わせ
//
MINT DGrp::GetGrpParam(
						MDID		i_idGrp,	// グループId
						MREAL*		o_prKt,		// 階高さ
						MREAL*		o_prFl		// 床高さ
				)
{
	MdmSetGRP*	pGrp;

	ASSERT( MDISGRP( i_idGrp));

	pGrp = &Mdm::GetCurModel()->m_GrpBf.m_st[MIDtoHN( i_idGrp)];
	*o_prKt = pGrp->m_rKt;
	*o_prFl = pGrp->m_rFl;
	return 0;
}

//===========================================================================
//		グループの階高とFLを設定
//
MINT DGrp::SetGrpParam(
						MDID		i_idGrp,	// グループId
						MREAL		i_rKt,		// 階高さ
						MREAL		i_rFl		// 床高さ
				)
{
	MdmSetGRP*	pGrp;

	ASSERT( MDISGRP( i_idGrp));

	pGrp = &Mdm::GetCurModel()->m_GrpBf.m_st[MIDtoHN( i_idGrp)];
	pGrp->m_rKt = i_rKt;
	pGrp->m_rFl = i_rFl;
	return 0;
}

//===========================================================================
//		カレントグループIdを得る
//
MDID DGrp::GetCurGrpId()
{
	return	Mdm::GetCurModel()->m_idCurGrp;
}

//===========================================================================
//		カレントグループIdを設定する
//
MDID DGrp::SetCurGrpId(							// 直前のグループId
						MDID		i_idGrp		// グループId
				)
{
	MDID idCurGrp;

	ASSERT( MDISGRP( i_idGrp));

	idCurGrp = Mdm::GetCurModel()->m_idCurGrp;
	Mdm::GetCurModel()->m_idCurGrp = i_idGrp;

	// カレント座標変換マトリックスを設定する
	DLyr::SetCurCoord( Mdm::GetCurModel()->m_GrpBf.m_st[MIDtoHN( i_idGrp)].m_MTrans);
	return	idCurGrp;
}

//===========================================================================
//		グループIdのグループ名を得る
//
MINT DGrp::GetGrpNameFromId(
						MDID		i_idGrp,	// グループId
						MCHAR*		o_psName	// グループ名
				)
{
	ASSERT( MDISGRP( i_idGrp));

	Mstrcpy( o_psName, Mdm::GetCurModel()->m_GrpBf.m_st[MIDtoHN( i_idGrp)].m_sName);
	return 0;
}

//===========================================================================
//	xx	グループIdのグループ名を設定する
//
MINT DGrp::SetGrpNameFromId(
						MDID		i_idGrp,	// グループId
						MCHAR*		i_psName	// グループ名
				)
{
	ASSERT( MDISGRP( i_idGrp));

	Mstrcpy( Mdm::GetCurModel()->m_GrpBf.m_st[MIDtoHN( i_idGrp)].m_sName, i_psName);
	return 0;
}

//===========================================================================
//		グループ名よりグループIdを得る
//
MINT DGrp::GetGrpIdFromName (
						MCHAR*		i_psName,	// グループ名
						MDID*		o_pidGrp	// グループId
				)
{
	MINT	iC;

	for ( iC=0; iC<Mdm::GetCurModel()->m_GrpBf.m_n; iC++) {
		if ( Mstrcmp(  Mdm::GetCurModel()->m_GrpBf.m_st[iC].m_sName, i_psName))
			break;
	}
	if ( iC < Mdm::GetCurModel()->m_GrpBf.m_n)
		*o_pidGrp = MHNtoID( iC);
	else
		*o_pidGrp = 0;
	return 0;
}

//===========================================================================
//		グループ数を得る
//
MINT DGrp::GetGrpCount ()
{
	return ( Mdm::GetCurModel()->m_GrpBf.m_n - Mdm::GetCurModel()->m_GrpBf.m_nidSpace);
}

//===========================================================================
//		全グループIdのセットを得る
//

MINT DGrp::GetGrpIdAll (
						MDIDSET*		o_pGidGrp	// グループIdセット
				)
{
	MINT	iC;

	o_pGidGrp->m_n = 0;
	for ( iC=0; iC<Mdm::GetCurModel()->m_GrpBf.m_n; iC++) {
		if ( MDSPACEID( &Mdm::GetCurModel()->m_GrpBf.m_st[iC]) != MDID_DELETE)
			(*o_pGidGrp) += MHNtoID( iC);
	}
	return 0;
}

//===========================================================================
//		グループの座標変換マトリックスを得る
//
MINT	MdlGetGrpCoord (
						MDID		i_idGrp,
						MgMat3*		o_mTrans	// 座標変換マトリックス
				)
{
	ASSERT( MDISGRP( i_idGrp));

	*o_mTrans = Mdm::GetCurModel()->m_GrpBf.m_st[MIDtoHN( i_idGrp)].m_MTrans;
	return 0;
}

//===========================================================================
//		基準座標変換マトリックスを得る
//
//		データベースをオープンすると、基準座標変換マトリックスは必ず１つ存在する。
//
MgMat3	MdlGetBaseCoord ( )						// 基準座標変換マトリックス
{
	return Mdm::GetCurModel()->m_MBaseTrans;
}


//===========================================================================
//		カレント座標変換マトリックスを得る
//
//		１つのグループには１つの座標系が存在する。
//		SetCurGrpIdでグループがカレントに設定されると、
//		そのグループの座標系がカレント座標系になる。
//		Inp:Point等で得られる座標値はカレント座標系上の座標値となる。
//
MgMat3	MdlGetCurCoord ( )						// カレント座標変換マトリックス
{
	return Mdm::GetCurModel()->m_MCurTrans;
}

//===========================================================================
//		カレント座標変換マトリックスを設定
//
//		１つのグループには１つの座標系が存在する。
//		SetCurGrpIdでグループがカレントに設定されると、
//		そのグループの座標系がカレント座標系になる。
//		Inp:Point等で得られる座標値はカレント座標系上の座標値となる。
//
//		カレント座標系を一時的に変更する場合は、事前にGetCurCoordで
//		元のカレント座標系を問い合わせておき、処理終了時に元に戻すこと。
//
MgMat3 DLyr::SetCurCoord (
						MgMat3&		i_MTransr	// 座標変換マトリックス
				)
{
	MgMat3	MTrans;

	MTrans = Mdm::GetCurModel()->m_MCurTrans;
	Mdm::GetCurModel()->m_MCurTrans = i_MTransr;
	return MTrans;
}

} // namespace MC
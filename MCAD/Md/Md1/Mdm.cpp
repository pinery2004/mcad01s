//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MdModel.cpp
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

#define DLL_EXPORT_MDMODEL_DO
#define DLL_EXPORT_DATTR_DO
#include "MdModel.h"
#include "MdmDB.h"
#define DLL_EXPORT_MDM_DO
#include "Mdm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace MC
{

CArray<MdModel*>	Mdm::ms_pMdl;			// モデルリスト
MDID				Mdm::ms_idCurMdl;		// カレントモデルID
MdModel*			Mdm::ms_pCurMdl;		// カレントモデル

//===========================================================================
//【機能】モデルDBを初期化する
//【返値】なし
MINT Mdm::MdmInitialize( void)
{
	MDID idMdl;
	idMdl = AddModel( Mstr( ""));
	SetCurModel( idMdl);
	return 0;
}

//===========================================================================
//【機能】モデルDBを解放する
//【返値】なし
MINT Mdm::MdmTerminate( void)
{
	MINT iC;

	for ( iC=0; iC<Mdm::ms_pMdl.GetCount(); iC++) {
		delete( Mdm::ms_pMdl[iC]);
	}
	return 0;
}

//===========================================================================
//【機能】モデルを追加し、モデルIDを取得
//【返値】モデルID
MDID Mdm::AddModel(
					MCHAR*	i_cMdlNm		// モデル名
					)
{
	MDID idMdl;
	MdModel* pMdl = new MdModel;
	Mdm::ms_pMdl.Add( pMdl);
	idMdl = (MDID)Mdm::ms_pMdl.GetCount();
	return idMdl; 
}

//===========================================================================
//【機能】モデル名からモデルIDを取得する
//【返値】モデルID
MDID Mdm::GetModelId(
					MCHAR*	i_cMdlNm		// モデル名
					)
{
	MDID idMdl = 0;
	for ( int iC=0; iC<ms_pMdl.GetCount(); iC++) {
		if ( Mstrcmp( ms_pMdl[iC]->m_cMdlNm, i_cMdlNm)) {
			idMdl = MHNtoID( iC);
			break;
		}
	}
	return idMdl; 
}

//===========================================================================
//【機能】モデルIDからモデル名を取得する
//【返値】モデルID
static MDID GetModelName(
					MDID	i_idMdl,		// モデルID
					MCHAR*	o_cMdlNm		// モデル名
					)
{
}

} // namesace MC
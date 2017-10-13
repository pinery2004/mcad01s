//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MhModel.cpp
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

#include "MdModel.h"
#include "MdmDB.h"

#include "MhDefine.h"
#include "MhModel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace MC
{

static	MCHAR*	z_cKai[]	= { Mstr( "Ｂ２"), Mstr( "Ｂ１"), Mstr( "地階"), Mstr( "１Ｆ"), Mstr( "２Ｆ"),
								Mstr( "３Ｆ"), Mstr( "４Ｆ"), Mstr( "５Ｆ")};
static  MCHAR*	z_cTpKz[]	= { MD_NM_SCMK_KUKAKU, MD_NM_SCMK_BUZAI, MD_NM_SCMK_PANEL, MD_NM_SCMK_KANAGU, MD_NM_SCMK_CHUKI};

static	MCHAR*	z_cNmGrp[]	= { MD_NM_GRP_WIN, MD_NM_GRP_SHIKITI, MD_NM_GRP_ISHOU, MD_NM_GRP_KOUZOU};
static	MDID	z_nhGrp[]	= { MD_NH_GRP_WIN, MD_NH_GRP_SHIKITI, MD_NH_GRP_ISHOU, MD_NH_GRP_KOUZOU};

static	MCHAR*	z_cNmScm[]	= { MD_NM_SCM_GRID, MD_NM_SCM_HAITI, MD_NM_SCM_KISO, MD_NM_SCM_DODAI,
								MD_NM_SCM_YUKA, MD_NM_SCM_KABE, MD_NM_SCM_TENJO, MD_NM_SCM_KOYA,
								MD_NM_SCM_YANE};
static	MDID	z_nhScm[]	= { MD_NH_SCM_GRID, MD_NH_SCM_HAITI, MD_NH_SCM_KISO, MD_NH_SCM_DODAI,
								MD_NH_SCM_YUKA, MD_NH_SCM_KABE, MD_NH_SCM_TENJO, MD_NH_SCM_KOYA,
								MD_NH_SCM_YANE};

static	MCHAR*	z_cTpK[]	= { MD_NM_SCMK_BUZAI, MD_NM_SCMK_PANEL, MD_NM_SCMK_KANAGU};
static	MDID	z_nhTpK[]	= { MD_NH_SCMK_BUZAI, MD_NH_SCMK_PANEL, MD_NH_SCMK_KANAGU};

static	MDID	z_nhLyr[MD_N_SCM][MD_N_SCMK];


//inline	MINT nhMhGrpId( MINT iGpm, MINT iGps)	{ return iGpm + iGps;}		// グループＩＤの配列番号を取得
//inline	MINT nhMhScmId( MINT iScmm, MINT iScms) { return iScmm + iScms;}	// スキーマＩＤの配列番号を取得

MhModel	z_DbIE;									// 実体生成
MhDbIE	MhModel::m_db;							// DB

MhModel::MhModel( void)
{
//	m_MBaseTrans.SetUnit();
	
	m_db.pidGrp = new MDID[1000];				// グループID
	m_db.nGrp = 0;								// グループID数
	m_db.pidScm = new MDID[2000];				// スキーマID
	m_db.nScm = 0;								// スキーマID数
	m_db.pidLyr = new MDID[3000];				// レイヤーID
	m_db.nLyr = 0;								// レイヤーID数

	m_db.nFKai	= 5;							// 地上階数
	m_db.nBKai	= 2;							// 地下階数
	m_db.nAKai	= m_db.nFKai + 1 + m_db.nAKai;	// 全階数
}

MhModel::~MhModel( void)
{
	MBFREE( m_db.pidGrp);
	MBFREE( m_db.pidScm);
	MBFREE( m_db.pidLyr);
}


MINT	MhCreateGrp2(							//
						MCHAR		i_cName[],	// 代表グループ名
						MINT		i_iKai,		// 階(代表グループ内の種類No)
						MREAL		i_rKt,		// 階高さ
						MREAL		i_rFl,		// 床高さ
						MDID		i_nhGrp,	// グループ配列番号
						MINT		i_sz		// 初期グループIdバッファサイズ /*=10*/
				)
{
	MCHAR	cGrp[101];
	Mstrcpy( cGrp, i_cName);
	Mstrcat( cGrp, z_cKai[i_iKai]);
	return DGrp::CreateGrp( cGrp, i_rKt, i_rFl, &MhModel::m_db.pidGrp[MDIDGRPK( i_nhGrp , i_iKai)], i_sz);
}

MINT	MhCreateScm2(							//
						MCHAR		i_cName[],	// 代表スキーマ名
						MINT		i_iTp,		// スキーマ種類
						MDID		i_nhScm,	// スキーマ配列番号
						MINT		i_sz		// 初期化スキーマIdバッファサイズ /*=10*/
				)
{
	MCHAR	cScm[101];
	Mstrcpy( cScm, i_cName);
	Mstrcat( cScm, z_cTpKz[i_iTp]);
	return DGrp::CreateScm( cScm, &MhModel::m_db.pidScm[MDIDSCMT( i_nhScm, i_iTp)], i_sz);
}

MINT	MhCreateLyr2(							//
						MDID		i_nhScm,	// スキーマ配列番号
						MINT		i_iTp,		// スキーマ種類
						MDID		i_nhGrp,	// グループ配列番号
						MINT		i_iKai,		// グループ種類
						MDID		i_nhLyr,	// レイヤー配列番号
						MINT		i_sz		// 初期化レイヤーIdバッファサイズ /*=10*/
				)
{
	return DLyr::CreateLyr( MhModel::m_db.pidScm[MDIDSCMT( i_nhScm, i_iTp)],
						 MhModel::m_db.pidGrp[MDIDGRPK( i_nhGrp , i_iKai)],
						 &(MhModel::m_db.pidLyr[i_nhLyr]),
						 i_sz);
}


///////////////////////////////////////////////////////////////////////////////
////  家モデル データベース 使用開始宣言
//
MINT MhModel::Open( void)
{
	MINT	ist;

	MINT	isz = 1;
	static MINT	iCe = 0;

	// DB設定
	// グループ
	MINT	iKai;
	MhCreateGrp1( MD_NM_GRP_WIN, 3000., 250., MD_NH_GRP_WIN, isz);
	MhCreateGrp1( MD_NM_GRP_SHIKITI, 10000., 0., MD_NH_GRP_SHIKITI, isz);
	MhCreateGrp1( MD_NM_GRP_ISHOU, 10000., 0., MD_NH_GRP_ISHOU, isz);
	MhCreateGrp1( MD_NM_GRP_KOUZOU, 3000., 250., MD_NH_GRP_KOUZOU, isz);
	m_db.nGrp = MD_N_GRP;

	for ( iKai=1; iKai<m_db.nAKai; iKai++)
		MhCreateGrp2( MD_NM_GRP_KOUZOU, iKai, 3000., 250., MD_NH_GRP_KOUZOU, isz);

	// スキーマ
	MINT	iTp;
	MINT	iSc;
	MhCreateScm1( MD_NM_SCM_GRID, MD_NH_SCM_GRID, isz);
	MhCreateScm1( MD_NM_SCM_HAITI, MD_NH_SCM_HAITI, isz);
	MhCreateScm1( MD_NM_SCM_KISO, MD_NH_SCM_KISO, isz);

	for ( iSc=2; iSc<9; iSc++) {
		for ( iTp=0; iTp<MD_N_SCMK; iTp++) {
			MhCreateScm2( z_cNmScm[iSc], iTp, z_nhScm[iSc], isz);
		}
	}

	MhCreateScm1( MD_NM_SCM_YANE, MD_NH_SCM_YANE, isz);
	m_db.nScm = MD_N_SCM;

	// レイヤー
	MINT	iK;
	MhCreateLyr1( MD_NH_SCM_GRID, MD_NH_GRP_WIN, MD_ID_LYR_GRID, isz);
	MhCreateLyr1( MD_NH_SCM_HAITI, MD_NH_GRP_WIN, MD_ID_LYR_HAITI, isz);
	for ( iK=1; iK<m_db.nAKai; iK++) {
		for ( iTp=0; iTp<MD_N_SCMK; iTp++) {
//			z_nhLyr[] = 
//			MhCreateLyr2( MD_NH_SCM_HAITI, iTp, MD_NH_GRP_KOUZOU, iK, z_nhLyr, isz);
		}
	}

	MhCreateLyr1( MD_NH_SCM_YANE, MD_NH_GRP_WIN, MD_ID_LYR_YANE, isz);

	ist = 0;
	return ist;
}

} // namespace MC
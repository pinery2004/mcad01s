//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: .cpp
//
//		
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"

#define DLL_EXPORT_HAITIDB_DO
#include "MbDebug.h"
#include "MgDefine.h"
#include "MgTol.h"

#include "MgAngle.h"
#include "MgPoint.h"
#include "MgLine.h"
#include "MgPlane.h"
#include "MgPolygon.h"

#define DLL_EXPORT_HAITIIN_DO
#include "MmInpAttr.h"
#include "MdLib.h"

#include "MmDefine.h"
#include "MsBitSet.h"
#include "MmOpt.h"
#include "MmHist.h"
#include "MbList.h"

#include "MdPtsQueue.h"
#define DLL_EXPORT_IEMODEL_DO
#include "MhPlcInfo.h"

namespace MC
{

static	MdPtsQueue	z_PtsQueue( 100, sizeof( MhPlcInfo));		// 家モデル保存領域
static	bool		z_modIeModel = false;

/////////////////////////////////////////////////////////////////////////////
//	家モデル修正フラグの設定
void IeModel::MhSetModFIeMdl(
						bool	i_bMod			// 修正フラグ
				)
{
	z_modIeModel = i_bMod;
}

/////////////////////////////////////////////////////////////////////////////
//	家モデル修正フラグの検索
bool IeModel::MhGetModFIeMdl()
{
	return z_modIeModel;
}

/////////////////////////////////////////////////////////////////////////////
//	部品配置キュー　コンストラクタ　デストラクタ
MdPtsQueue::MdPtsQueue(
						MINT	i_nBk,			// 
						MINT	i_sz			// 
				)			
	: MbListE( i_nBk, i_sz)
{
}

MdPtsQueue::~MdPtsQueue( void)
{
	RemoveAll();
}

//////////////////////////////////////////////////////////////
//	部品配置キューの先頭に[部品配置]を追加する。
//	レコードの属性領域は追加したレコードに引渡し、引数のレコードはイニシャライズする。
MPOSITION MdPtsQueue::Push(						// 追加レコード位置
						MhPlcInfo*	i_pPlcEn	// 部品配置管理情報
				)
{
	MPOSITION	pos;
	pos = MbListE::Push( i_pPlcEn);
	i_pPlcEn->InitPtrTypeAtr();
	IeModel::MhSetModFIeMdl( true);
	return pos;
}


//////////////////////////////////////////////////////////////
//	部品配置キューの後尾に[部品配置]を追加する
//	レコードの属性領域は追加したレコードに引渡し、引数のレコードはイニシャライズする。
MPOSITION MdPtsQueue::Inject(					// 追加レコード位置
						MhPlcInfo*	i_pPlcEn	// 部品配置管理情報
				)
{
	MPOSITION	pos;
	pos = MbListE::Inject( i_pPlcEn);
	i_pPlcEn->InitPtrTypeAtr();
	IeModel::MhSetModFIeMdl( true);
	return pos;
}

//////////////////////////////////////////////////////////////
//	部品配置キューの[部品配置]を全て削除する
void MdPtsQueue::RemoveAll( void)
{
	MPOSITION	pos1;
	MhPlcInfo	*PlcEn;

	for (PlcEn=(MhPlcInfo*)this->GetHead( &pos1); PlcEn!=0; PlcEn= (MhPlcInfo*)this->GetNext( &pos1)) {
		PlcEn->FreeAllAtr();
	}
		
	MbListE::RemoveAll();
}

/////////////////////////////////////////////////////////////////////////////
//	ＤＢをイニシャライズする
void HaitiDb::MdPtsDbInit( void)
{
	z_PtsQueue.RemoveAll();
}

/////////////////////////////////////////////////////////////////////////////
//	エンティティとPOSITIONのポインタの差を求める
//   pEnt = pPos + MdGetSizeOfHd();
MINT HaitiDb::MdGetSizeOfHd()
{
	return z_PtsQueue.GetSizeOfHd();
}

/////////////////////////////////////////////////////////////////////////////
//	[部品配置]を削除する
void HaitiDb::MdPtsDelete(
						MPOSITION	i_pPosition		// [部品配置]位置
				)
{
	MINT		ist1;
	MhPlcInfo	*pPlcEn;
	MsBitSet	*pOptvRN, *pOptvDN;
	MsBitSet	HstvMod;
	MsBitSet	*pHstvR, *pHstvMod;
	MhPlcInfo	*pPlcEnM;										// カレントバージョンのみ削除対象のＯＰＴ群を削除

	pPlcEn = ( MhPlcInfo*)( i_pPosition + z_PtsQueue.GetSizeOfHd());
	pOptvRN = pPlcEn->GetPIOpt1();
	pOptvDN = pPlcEn->GetPIOpt2();
	if ( Option::MhDelOpt( &pOptvRN, &pOptvDN) == 3) {					// Delete Opt ⊂ Plc Opt	（部品配置のＯＰＴ群が削除ＯＰＴ群より広範囲）
																// (部品配置のオプションの修正)
		ist1 = MhOpt::MhHistMod( pPlcEn, &pPlcEnM);
			pPlcEnM->SetPIOpt1( pOptvRN);						// 		部品配置のオプションを修正
			pPlcEnM->SetPIOpt2( pOptvDN);						// 		削除対象のＯＰＴ群を表示ＯＦＦＯＰＴ群に設定
	} else {													// Delete Opt ⊇ Plc Opt	（部品配置のＯＰＴ群より削除ＯＰＴ群が広範囲または同一）
																// (部品配置を削除)
		pHstvR = pPlcEn->GetPIVer();

		if ( pHstvR == NULL) {									// 削除対象部品配置の履歴管理をコピー
			pHstvMod = NULL;
		} else {
			HstvMod = *pHstvR;
			pHstvMod = &HstvMod;
		}

		if ( MhOpt::MhHistDel( &pOptvRN) == 1) {						// 		カレント Ver == 配置 Ver('s)
			pPlcEn->FreeAllAtr();								// 			部品配置の履歴管理とカレントバージョンが同一であり
			z_PtsQueue.RemoveAt( &i_pPosition);					//			部品配置を削除
		} else {												//		カレント Ver ⊂ 配置 Ver('s)
																// 			部品配置の履歴管理からカレントバージョンのみ削除
			if ( pHstvR == NULL) {								//			(バージョンの修正)
				pPlcEn->SetPIVer( pOptvRN);						//			新規追加カレントバージョン以外ＯＮの履歴管理のポインタをセット
			} else {
				*pHstvR = *pOptvRN;								//			履歴管理をカレントバージョンＯＦＦに修正
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//	部品配置を修正する
void HaitiDb::MdPtsModify(
						MhPlcInfo* io_pPlcEnR,	// 修正元部材
						MhPlcInfo* *o_ppPlcEnM	// 修正先部材
				)
{
	MINT	ist1, ist2;
	MhPlcInfo	*pPlcEnM1;										// 
	MhPlcInfo	*pPlcEnM2;										// 

	ist1 = Option::MhModOpt( io_pPlcEnR, &pPlcEnM1);
	ist2 = MhOpt::MhHistMod( pPlcEnM1, &pPlcEnM2);
	*o_ppPlcEnM = pPlcEnM2;

}

/////////////////////////////////////////////////////////////////////////////
//	部品配置を追加する
//	レコードの属性領域は追加したレコードに引渡し、引数のレコードはイニシャライズする。
MPOSITION HaitiDb::MdPtsAdd(
						MhPlcInfo*	i_pPlcEn,	// 部品配置管理情報
						MINT		i_fdispCtrl	// 表示制御フラグ
												//		 0: ＯＰＴ群ｾｯﾄ・展開ｺｰﾄﾞ・履歴管理ｺｰﾄﾞを追加しない
												//			外部ファイルからＤＢへ取り込み時にそのまま書き込むため使用する
												//		 1: ＯＰＴ群ｾｯﾄ・展開ｺｰﾄﾞ・履歴管理ｺｰﾄﾞを追加する
				)
{
	MPOSITION	iPos;

	MsBitSet	*pOptv;
	MsBitSet	*pHstv;

	if ( i_fdispCtrl == 1) {
		Option::MhAddOpt( &pOptv);										// Opt対応	新規追加レコード
		i_pPlcEn->SetPIOpt1( pOptv);							// Opt対応
//DD		MhAddTenkai( &bTenkai);									// 住棟展開対応	新規追加レコード
//DD		i_pPlcEn->SetPITenkai1( bTenkai);						// 住棟展開対応
		MhOpt::MhHistAdd( &pHstv);										// 履歴管理対応	新規追加レコード
		i_pPlcEn->SetPIVer( pHstv);								// 履歴管理対応
	}

	iPos = z_PtsQueue.Inject( i_pPlcEn);
	
//	MhCreateGrp
	

	return iPos;
}

//////////////////////////////////////////////////////////////
//	指定ノードの部品配置を取得する
MhPlcInfo* HaitiDb::MdPtsGet(
						MPOSITION	i_pPosition		// [部品配置]位置
				)
{
	return ( MhPlcInfo*)z_PtsQueue.GetAt( &i_pPosition);
}

static MPOSITION	z_PtsPos;

/////////////////////////////////////////////////////////////////////////////
//	先頭の部品配置を取得する
//	返値 =NULL: レコードなし !=NULL: 読み込み[部品配置]位置
MhPlcInfo* HaitiDb::MdGetHeadPts(
						MPOSITION*	i_pPtsPos		// [部品配置]位置
				)
{
	if ( i_pPtsPos)
		return (MhPlcInfo*)z_PtsQueue.GetHead( i_pPtsPos);
	else
		return (MhPlcInfo*)z_PtsQueue.GetHead( &z_PtsPos);
}

/////////////////////////////////////////////////////////////////////////////
//	次の部品配置を取得する
//	返値 =NULL: レコードなし !=NULL: 読み込み[部品配置]位置
MhPlcInfo* HaitiDb::MdGetNextPts(
						MPOSITION*	i_pPtsPos		// [部品配置]位置
				)
{
	if ( i_pPtsPos)
		return (MhPlcInfo*)z_PtsQueue.GetNext( i_pPtsPos);
	else
		return (MhPlcInfo*)z_PtsQueue.GetNext( &z_PtsPos);
}

/////////////////////////////////////////////////////////////////////////////
//	家モデルＤ／Ｂの部品配置の有無を調べる

bool HaitiDb::MdIsNotEmptyPts()							// 部品配置の有無 true: 部品配置有り/ false: 部品配置無し
{
	return z_PtsQueue.IsNotEmpty();
}

/////////////////////////////////////////////////////////////////////////////
//	家モデルＤ／Ｂの最後に部品配置を追加する

MhPlcInfo* HaitiDb::MdInjectPts(
						MhPlcInfo*	i_pPlcEn	// 部品配置管理情報
				)
{
	MPOSITION	pPosition;

	pPosition = z_PtsQueue.Inject( i_pPlcEn);
	return (MhPlcInfo*)z_PtsQueue.GetAt( &pPosition);
}

/////////////////////////////////////////////////////////////////////////////
//	家モデルＤ／Ｂの最後の部品配置を取得する

void HaitiDb::MdEjectPts(
						MhPlcInfo*	o_pPlcEn	// 部品配置管理情報
				)
{
	z_PtsQueue.Eject( o_pPlcEn);
}

} // namespace MC
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

#include "MbDebug.h"
#include "MgDefine.h"
#include "MgTol.h"

#include "MgAngle.h"
#include "MgPoint.h"
#include "MgLine.h"
#include "MgPlane.h"
#include "MgPolygon.h"

#define DLL_EXPORT_HAITIDB_DO
#include "MdLib.h"

#include "MmDefine.h"
#include "MsBitSet.h"
#include "MmOpt.h"
#include "MmHist.h"
#include "MbList.h"
#include "MhLib.h"

#include "MgGrp.h"
#include "MhRoof.h"
#include "MdRoofQueue.h"

namespace MC
{

static	MdRoofQueue	z_RoofQueue( 100, sizeof( MhRoofInfo));			// 家モデル保存領域

/////////////////////////////////////////////////////////////////////////////
//	部品配置キュー　コンストラクタ　デストラクタ
MdRoofQueue::MdRoofQueue( MINT nBk, MINT iSz)
	: MbListE( nBk, iSz)
{
}

MdRoofQueue::~MdRoofQueue( void)
{
	RemoveAll();
}

//////////////////////////////////////////////////////////////
//	屋根リストの先頭に屋根レコードを追加する。
//	レコードの属性領域は追加したレコードに引渡し、引数のレコードはイニシャライズする。
MPOSITION MdRoofQueue::Push( MhRoofInfo *pRoofEn)
{
	MPOSITION	pos;
	pos = MbListE::Push( pRoofEn);
	pRoofEn->InitAllAtr();
	IeModel::MhSetModFIeMdl( true);
	return pos;
}

//////////////////////////////////////////////////////////////
//	屋根リストの後尾に屋根レコードを追加する
//	レコードの属性領域は追加したレコードに引渡し、引数のレコードはイニシャライズする。
MPOSITION MdRoofQueue::Inject( MhRoofInfo *pRoofEn)
{
	MPOSITION	pos;
	pos = MbListE::Inject( pRoofEn);
	pRoofEn->InitAllAtr();
	IeModel::MhSetModFIeMdl( true);
	return pos;
}

//////////////////////////////////////////////////////////////
//	屋根リストの屋根レコードを全て削除する
void MdRoofQueue::RemoveAll( void)
{
	MPOSITION	pos1;
	MhRoofInfo	*pRoofEn;

	for (pRoofEn=(MhRoofInfo*)this->GetHead( &pos1); pRoofEn!=0; pRoofEn=(MhRoofInfo*)this->GetNext( &pos1)) {
		pRoofEn->FreeAllAtr();
	}
		
	MbListE::RemoveAll();
}

/////////////////////////////////////////////////////////////////////////////
//	家モデルＤ／Ｂの屋根をイニシャライズする
void HaitiDb::MdRoofDbInit( void)
{
	z_RoofQueue.RemoveAll();
}

/////////////////////////////////////////////////////////////////////////////
//	屋根を削除する
//void MdRoofDelete( MPOSITION pPosition)
//{
//	MhRoofInfo*	pRoofEn = (MhRoofInfo*)(pPosition + z_RoofQueue.GetSizeOfHd());
//	pRoofEn->FreeAllAtr();
//	z_RoofQueue.RemoveAt( &pPosition);
//}

/////////////////////////////////////////////////////////////////////////////
//	部品配置を削除する
void HaitiDb::MdRoofDelete(
						MPOSITION	i_pPosition		// [部品配置]位置
				)
{
	MsBitSet	*pOptvRN, *pOptvDN;
	MhRoofInfo	*pRoofEn;

	pRoofEn = ( MhRoofInfo*)( i_pPosition + z_RoofQueue.GetSizeOfHd());
	pOptvRN = pRoofEn->GetPIOpt1();
	pOptvDN = pRoofEn->GetPIOpt2();

	if ( Option::MhDelOpt( &pOptvRN, &pOptvDN) < 3) {
		pRoofEn->FreeAllAtr();
		z_RoofQueue.RemoveAt( &i_pPosition);					// 部品配置のＯＰＴ群より削除ＯＰＴ群が広範囲であり部品配置を削除
	} else {
		pRoofEn->SetPIOpt1( pOptvRN);
		pRoofEn->SetPIOpt2( pOptvDN);							// 削除対象のＯＰＴ群を表示ＯＦＦＯＰＴ群に設定
	}
}

/////////////////////////////////////////////////////////////////////////////
//	部品配置を追加する
//	レコードの属性領域は追加したレコードに引渡し、引数のレコードはイニシャライズする。
MPOSITION HaitiDb::MdRoofAdd(
						MhRoofInfo* io_pRoofEn,		// 部品配置管理情報
						MINT		i_fOptv			// ＯＰＴ群追加フラグ
													//		 0: ＯＰＴ群追加しない
													//		 1: ＯＰＴ群追加する
				)
{
	MsBitSet	*pOptv;
	MsBitSet	*pHstv;

	if ( i_fOptv == 1) {
		Option::MhAddOpt( &pOptv);								// Opt対応　新規追加レコード
		io_pRoofEn->SetPIOpt1( pOptv);						// Opt対応
		MhOpt::MhHistAdd( &pHstv);									// 履歴管理対応　新規追加レコード
		io_pRoofEn->SetPIVer( pHstv);						// 履歴管理対応
	}
	
	return z_RoofQueue.Inject( io_pRoofEn);
}

/////////////////////////////////////////////////////////////////////////////
//	屋根を追加する
//	レコードの属性領域は追加したレコードに引渡し、引数のレコードはイニシャライズする。
//MPOSITION HaitiDb::MdRoofAdd( MhRoofInfo &RoofEn)
//{
//	return z_RoofQueue.Inject( &RoofEn);
//}

//////////////////////////////////////////////////////////////
//	指定ノードの屋根を取得する
MhRoofInfo* HaitiDb::MdRoofGet( MPOSITION pPosition)
{
	return (MhRoofInfo*)z_RoofQueue.GetAt( &pPosition);
}

static MPOSITION	z_RoofPos;

/////////////////////////////////////////////////////////////////////////////
//	先頭の屋根を取得する
//	返値 =NULL: レコードなし !=NULL: 読み込み[部品配置]位置
MhRoofInfo* HaitiDb::MdGetHeadRoof( MPOSITION *pRoofPos)
{
	if ( pRoofPos)
		return (MhRoofInfo*)z_RoofQueue.GetHead( pRoofPos);
	else
		return (MhRoofInfo*)z_RoofQueue.GetHead( &z_RoofPos);
}

/////////////////////////////////////////////////////////////////////////////
//	次の屋根を取得する
//	返値 =NULL: レコードなし !=NULL: 読み込み[部品配置]位置
MhRoofInfo* HaitiDb::MdGetNextRoof( MPOSITION *pRoofPos)
{
	if ( pRoofPos)
		return (MhRoofInfo*)z_RoofQueue.GetNext( pRoofPos);
	else
		return (MhRoofInfo*)z_RoofQueue.GetNext( &z_RoofPos);
}

/////////////////////////////////////////////////////////////////////////////
//	家モデルＤ／Ｂの屋根の有無を調べる

bool HaitiDb::MdIsNotEmptyRoof()								// true: 屋根有り/ false: 屋根無し
{
	return z_RoofQueue.IsNotEmpty();
}

/////////////////////////////////////////////////////////////////////////////
//	家モデルＤ／Ｂの最後に屋根を追加する

void HaitiDb::MdInjectRoof(
						MhRoofInfo	*pRoofEn		// (I  ) 屋根管理情報
				)
{
	z_RoofQueue.Inject( pRoofEn);
}

/////////////////////////////////////////////////////////////////////////////
//	家モデルＤ／Ｂの最後の屋根を取得する

void HaitiDb::MdEjectRoof(
						MhRoofInfo *pRoofEn			// (  O) 屋根管理情報
				)
{
	z_RoofQueue.Eject( pRoofEn);
}

} // namespace MC
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MsDispModel.cpp
//
//		家モデルのディスプレイリスト作成
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MCAD.h"

#include "MmWnd.h"

#include "MgLib.h"
#include "MmDefine.h"
#include "MmSystemProperty.h"

#include "MsBitSet.h"
#include "MbList.h"
#include "MhLib.h"

#include "MgGrp.h"
#include "MhRoof.h"
#include "MdLib.h"

#include "MhInp.h"

#include "MmOpt.h"
#include "MmHist.h"

#include "MdFig.h"
#include "MdmDisp.h"

#include "MdDispList.h"
#include "MmValid.h"

#include "MnLib.h"

#define		MP_PANEL_LINE			1
#define		MP_PANEL_RECTANGULAR	2

namespace MC
{

////////////////////////////////////////////////////////////////////////////
//	ディスプレイリストを作成する
MINT DispList::Open( 
						MINT		inmDispList		// DisplayList番号
				)
{
	// make display list number
	if ( ( z_DispListFig[inmDispList] = glGenLists( 1)) == 0)
		return 1;

	// ディスプレイリストを作成する
	glNewList( z_DispListFig[inmDispList], GL_COMPILE);

//	glPushMatrix();
	return 0;
}

////////////////////////////////////////////////////////////////////////////
//	ディスプレイリストの作成を終了する
void DispList::Close()
{
//	glPopMatrix();
	glEndList();
}

////////////////////////////////////////////////////////////////////////////
//	構造家モデルの３次元ディスプレイリストを作成する	部材、基礎	実体
//	
int DispList::MakeIeMdl(
						MREAL		rB,				// 表示倍率
				const	MgPoint3	&PtCtr			// 中心座標
				)
{
	DispList::MakeIeMdl1( rB, PtCtr);
	DispList::MakeIeMdl2( rB, PtCtr);
	DispList::MakeIeMdl3( rB, PtCtr);
	MsMakeDispListMat0( rB, PtCtr);
	MsMakeDispListMat1( rB, PtCtr);
	MsMakeDispListMat2( rB, PtCtr);
	return 0;
}

////////////////////////////////////////////////////////////////////////////
//	構造家モデルの３次元ディスプレイリストを作成する	部材、基礎	実体
//	
MINT DispList::MakeIeMdl1(
						MREAL		rB,				// 表示倍率
				const	MgPoint3	&PtCtr			// 中心座標
				)
{
	MhPlcInfo*	pPlcEn;
	MPOSITION	posH;

	if ( DispList::Open( 1))
		return 1;
	
	for (pPlcEn=MdGetHeadPts( &posH); pPlcEn!=0; pPlcEn=MdGetNextPts( &posH)) {

		if ( !MmChkValidParts( pPlcEn))							// オプションと履歴のチェック
			continue;

		//	部材のディスプレイリストを作成する
		if (Mstrcmp( pPlcEn->GetPTNmGeneral(), Mstr( "壁")) == 0 ||
			Mstrcmp( (&pPlcEn->GetPTNmGeneral()[2]), Mstr( "建具")) == 0)			// 壁と建具は読み飛ばし
			continue;

		//	たて枠のディスプレイリストを作成する
		if (pPlcEn->IsFrame()) {
			MsOGL::SetMaterial( 0, 3);
			DrwTatewaku( pPlcEn, PtCtr, rB);

		//	パネルのディスプレイリストを作成する
		} else if (pPlcEn->IsPanel()) {
			MsOGL::SetMaterial( 0, 3);
			DrwPanel( pPlcEn, PtCtr, rB, 3);

		//	床、天井、屋根の開口のディスプレイリストを作成する
		} else if (pPlcEn->IsKaiko()) {
			MsOGL::SetMaterial( 0, 3);
			DrwKaiko( pPlcEn, PtCtr, rB);

		//	基礎のディスプレイリストを作成する
		} else if (pPlcEn->IsKiso()) {
			MsOGL::SetMaterial( 0, 4);
			DrwKiso( pPlcEn, PtCtr, rB);

		//	横置部材のディスプレイリストを作成する
		} else if (pPlcEn->IsOukaZai()) {
			MsOGL::SetMaterial( 0, 3);
			DrwOukaZai( pPlcEn, PtCtr, rB);
		}
	}
	// 図形情報のディスプレイリストを作成する
	MdDispList( PtCtr, rB);

	DispList::Close();
	return 0;
}

////////////////////////////////////////////////////////////////////////////
//	構造家モデルの３次元ディスプレイリストを作成する	壁、矢切　マテリアルなし
//
int DispList::MakeIeMdl2(
						MREAL		rB,				// 表示倍率
				const	MgPoint3	&PtCtr			// 中心座標
				)
{
	MhPlcInfo*	pPlcEn;
	MPOSITION	posH;
	MhRoofInfo*	pRoofEn;
	MPOSITION	posR;

	if ( DispList::Open( 2))
		return 1;

	//	壁のディスプレイリストを作成する
	for (pPlcEn=MdGetHeadPts( &posH); pPlcEn!=0; pPlcEn=MdGetNextPts( &posH)) {

		if ( !MmChkValidParts( pPlcEn))							// オプションと履歴のチェック
			continue;

		//	壁のディスプレイリストを作成する
		if (Mstrcmp( pPlcEn->GetPTNmGeneral(), Mstr( "壁")) == 0) {
			Kabe( pPlcEn, PtCtr, rB);

		//	建具の開口のディスプレイリストを作成する
		} else if (Mstrcmp( (&pPlcEn->GetPTNmGeneral()[2]), Mstr( "建具")) == 0) {
			DrwTategu( NULL, pPlcEn, PtCtr, rB);

		}
	}

	//	矢切のディスプレイリストを作成する
	for (pRoofEn=MdGetHeadRoof( &posR); pRoofEn!=0; pRoofEn=MdGetNextRoof( &posR)) {

		if ( !MmChkValidRoof( pRoofEn))							// オプションと履歴のチェック
			continue;

		Yagiri( pRoofEn, PtCtr, rB);
	}

	DispList::Close();
	return 0;
}

////////////////////////////////////////////////////////////////////////////
//	構造家モデルの３次元ディスプレイリストを作成する	屋根　マテリアルなし
//
int DispList::MakeIeMdl3(
						MREAL		rB,				// 表示倍率
				const	MgPoint3	&PtCtr			// 中心座標
				)
{

	MhRoofInfo*		pRoofEn;
	MPOSITION		posR;

	if ( DispList::Open( 3))
		return 1;

	//	屋根のディスプレイリストを作成する
	for (pRoofEn=MdGetHeadRoof( &posR); pRoofEn!=0; pRoofEn=MdGetNextRoof( &posR)) {
		if ( !MmChkValidRoof( pRoofEn))							// オプションと履歴のチェック
			continue;
		Yane1( pRoofEn, PtCtr, rB);
	}

	DispList::Close();
	return 0;
}

} // namespace MC
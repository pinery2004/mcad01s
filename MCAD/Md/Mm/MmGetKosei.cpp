//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MmGetGp.cpp
//
//		カレント入力モード
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"
#include "MbDefine.h"
#include "MbCod.h"
#include "MmWnd.h"

#define DLL_EXPORT_MC_INPATTR_DO
#define DLL_EXPORT_MC_INPMODE_DO
#include "MmLib.h"

#include "MsBitSet.h"
#include "MCADApp.h"
#include "MhLib.h"

#include "MhInp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace MC
{

static MINT z_iInpMd;							// 創成モード		削除、修正、移動、複写、属性表示
static MINT z_InpKai;							// 階				１階、２階、３階

static MINT z_iSInpGp;							// 設計入力構成		住戸、住棟、物件、屋根、敷地、壁量
static MINT z_iSInpBr;							// 設計入力種類
static MINT z_iSInpSel;							// 設計入力選択

static MINT z_iKInpGp;							// 構造入力構成		屋根、小屋、天井、耐力壁、壁、床、基礎
static MINT z_iKInpBr;							// 構造入力種類		部材、金物、パネル、その他
static MINT z_iKInpSel;							// 構造入力選択		選択1、選択2、選択3						未使用

static MINT z_bKInpBuzai;						// 部材選択入力許可フラグ
static MINT z_bKInpKanagu;						// 金物選択入力許可フラグ
static MINT z_bKInpPanel;						// パネル選択入力許可フラグ
static MINT z_bKInpOther;						// 部材選択入力許可フラグ

//	static MINT z_nPtsID;						// 配置部品タイプ(部材、金物、パネル、その他　の部材ID)

////////////////////////////////////////////////////////////////////////////
//	部材属性入力用コンボボックスの設定

void InpAttr::SetComboPts()
{
	//	部材選択用コンポボックスの項目を設定する
	InpAttr::InitComboTpPtsAttr();

	//	寸法型式選択用コンポボックスの項目を設定する
	InpAttr::InitComboPtsMbr();

	//	パネル番号選択用コンポボックスの項目を設定する
	InpAttr::InitComboPanelNo();

	//	部品選択用コンポボックスの項目を設定する
	InpAttr::InitComboAttr();

	Window::CurWndFocus();
	WindowCtrl::MmWndKReDraw();
}

//	創成モード		削除、修正、移動、複写、属性表示

void InpMode::SetMode( MINT iMode)
{
	z_iInpMd = iMode;
}

MINT InpMode::GetMode()
{
	return z_iInpMd;
}

//	階				１階、２階、３階

void InpMode::SetKai( MINT iKai)
{
	z_InpKai = iKai;
}

MINT InpMode::GetKai()
{
	return z_InpKai;
}

// 設計入力構成		住戸、住棟、物件、屋根、敷地、壁量

void InpMode::SetSGp( MINT iGp)
{
	z_iSInpGp = iGp;
}

MINT InpMode::GetSGp()
{
	return z_iSInpGp;
}

// 設計入力種類

void InpMode::SetSBr( MINT iBr)
{
	z_iSInpBr = iBr;
}

MINT InpMode::GetSBr()
{
	return z_iSInpBr;
}

// 設計入力選択

void InpMode::SetSSel( MINT iSel)
{
	z_iSInpSel = iSel;
}

MINT InpMode::GetSSel()
{
	return z_iSInpSel;
}

// 構造入力構成		屋根、小屋、天井、耐力壁、壁、床、基礎

void InpMode::SetKGp( MINT iGp)
{
	z_iKInpGp = iGp;
}

MINT InpMode::GetKGp()
{
	return z_iKInpGp;
}

// 構造入力種類		部材、金物、パネル、その他

void InpMode::SetKBr( MINT iBr)
{
	z_iKInpBr = iBr;
}

MINT InpMode::GetKBr()
{
	return z_iKInpBr;
}

// 構造入力選択		選択1、選択2、選択3						未使用

void InpMode::SetKSel( MINT iSel)
{
	z_iKInpSel = iSel;
}

MINT InpMode::GetKSel()
{
	return z_iKInpSel;
}

//	部材、金物、パネル、その他のラジオボタンの選択入力許可を設定

void InpMode::SetKBrB( bool bBuzai, bool bKanagu, bool bPanel, bool bOther)
{
	z_bKInpBuzai  = bBuzai;
	z_bKInpKanagu = bKanagu;
	z_bKInpPanel  = bPanel;
	z_bKInpOther  = bOther;
}

} // namespace MC
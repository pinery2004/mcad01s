//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MmDrag.cpp
//
//		ドラッギング操作
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"
#include "MgLib.h"
#include "MgMat.h"
#include "MbDefine.h"
#include "MbCod.h"
#include "MmGridNum.h"
#include "MmDrag.h"
#include "MmWnd.h"
#include "MmDefine.h"
#define DLL_EXPORT_MC_MSG_DO
#include "MmLib.h"
#include "MmSystemProperty.h"

#include "MmPrompt.h"

namespace MC
{

/////////////////////////////////////////////////////////////////////////////
//	ステータスバーにガイダンスメッセージを表示する

void Msg::GuidanceMsg(
						MINT	i_iMsgNo		// ガイダンスメッセージ ID
				)
{
	CString		GuidanceMsg;

	switch ( i_iMsgNo) {
		case MC_GUID_BASE:				GuidanceMsg = Mstr( "");																						break;
		case MC_GUID_SITEN:				GuidanceMsg = Mstr( "始点を指示して下さい");																	break;
		case MC_GUID_HAITITEN:			GuidanceMsg = Mstr( "配置点を指示して下さい");																break;
		case MC_GUID_SYUUTEN:			GuidanceMsg = Mstr( "終点を指示して下さい");																	break;
		case MC_GUID_HOUKOUTEN:			GuidanceMsg = Mstr( "方向点を指示して下さい");																break;
		case MC_GUID_AREA_SITEN:		GuidanceMsg = Mstr( "区画の始点を指示して下さい");															break;
		case MC_GUID_AREA_TUUKATEN:		GuidanceMsg = Mstr( "区画の通過点を指示して下さい. （終了は区画の始点指示）");								break;
		case MC_GUID_AREA_TAIKAKUTEN:	GuidanceMsg = Mstr( "区画の対角点を指示して下さい");															break;
		case MC_GUID_AREA_HOUKOUTEN:	GuidanceMsg = Mstr( "区画の方向点を指示して下さい");															break;
		case MC_GUID_JIM_SITEN:			GuidanceMsg = Mstr( "地廻り区画の始点を指示して下さい");														break;
		case MC_GUID_JIM_TUUKATEN:		GuidanceMsg = Mstr( "地廻り区画の通過点を指示して下さい. （切り妻部はSHIFTキー、終了は区画の始点指示）");	break;
		case MC_GUID_JIM_HOUKOUTEN:		GuidanceMsg = Mstr( "地廻り区画の方向点を指示して下さい");													break;
		case MC_GUID_DEL_PARTS:			GuidanceMsg = Mstr( "削除する部材を指示して下さい");															break;
		case MC_GUID_DEL_ROOF:			GuidanceMsg = Mstr( "削除する屋根を指示して下さい");															break;
		case MC_GUID_MOD_PANELNO:		GuidanceMsg = Mstr( "パネル番号を設定し、パネル番号を修正するパネルを指示して下さい");						break;
		case MC_GUID_MOD_BUZAILENGTH1:	GuidanceMsg = Mstr( "部材の長さ調整側の端部を指示して下さい");												break;
		case MC_GUID_MOD_BUZAILENGTH2:	GuidanceMsg = Mstr( "部材の長さ調整先を指示して下さい");												break;
		case MC_GUID_MULTI_BUZAIAREA:	GuidanceMsg = Mstr( "複数部材の配置領域を示す端点を指示して下さい");												break;
		default:						GuidanceMsg = Mstr( "未定義 Guidance Message");
	}
	Msg::SetSelStatusBar( MC_STSBAR_GUIDANCE, GuidanceMsg);						// ステイタスバーのガイダンスメッセージ表示部へ表示する
}

/////////////////////////////////////////////////////////////////////////////
//	ステータスバーに操作表示メッセージを表示する

void Msg::OperationMsg(
						MINT	iOprtMsgId			// (I  ) 操作表示ID
				)
{
	CString		OperationMsg;

	switch ( iOprtMsgId) {
		case MC_OPRT_BASE:				OperationMsg = Mstr( "");									break;
		case MC_OPRT_WAIT:				OperationMsg = Mstr( " 操作待ち");							break;
		case MC_OPRT_PARTS:				OperationMsg = Mstr( " 部材入力");							break;
		case MC_OPRT_ROOF:				OperationMsg = Mstr( " 屋根入力");							break;
		case MC_OPRT_DEL_PARTS:			OperationMsg = Mstr( " 部材削除");							break;
		case MC_OPRT_MOD_PANEL:			OperationMsg = Mstr( " パネル修正");						break;
		case MC_OPRT_MOD_BUZAI:			OperationMsg = Mstr( " 部材修正");							break;
		default:						OperationMsg = Mstr( " 未定義MSG");
	}
	Msg::SetSelStatusBar( MC_STSBAR_OPERATION, OperationMsg);							// ステイタスバーの操作表示部へ表示する
}

/////////////////////////////////////////////////////////////////////////////
//	ステータスバーにエラーメッセージを表示する
// 　エラーメッセージ1 + エラーメッセージ2

void Msg::ErrorMsg(
						MCHAR*	i_pcErrMsg1,	// エラーメッセージ１　または　NULL
						MINT	i_idErrMsg2,	// エラーメッセージ２表示ID
						MINT	i_imd_Disp		// エラー表示モード
												//		1: メッセージボックスへ表示
												//		0: コマンドラインへ表示 (コマンドラインへの表示が無ければメッセージボックスへ表示)
												//	   -1: コマンドラインへ表示
				)
{
	CString		ErrorMsg;

	switch (i_idErrMsg2){
		case MC_ERR_BASE:					ErrorMsg = Mstr( "");										break;
		case MC_ERR_IMPOS_MAKE_ROOF:		ErrorMsg = Mstr( "屋根を作成不可です");						break;
		case MC_ERR_NOTSET_TABLE:			ErrorMsg = Mstr( "テーブルに未設定です");					break;
		case MC_ERR_NOTSET_PARTS_TABLE:		ErrorMsg = Mstr( "が部品テーブルに未設定です");				break;
		case MC_ERR_NOTSET_MEMBER_TABLE:	ErrorMsg = Mstr( "が寸法型式テーブルに未設定です");			break;
		default:							ErrorMsg = Mstr( "未定義 ERROR MSG");
	}
	if ( i_pcErrMsg1 != NULL && i_pcErrMsg1[0] != 0)
		ErrorMsg = i_pcErrMsg1 + ErrorMsg;
	Msg::SetSelStatusBar( MC_STSBAR_ERROR, ErrorMsg);								// ステイタスバーのエラー表示部へ表示する
}

/////////////////////////////////////////////////////////////////////////////
//	ステータスバーのエラーメッセージを消去する
void Msg::ClearErrorMsg()
{
	Msg::SetSelStatusBar( MC_STSBAR_ERROR, Mstr( " "));							// ステイタスバーのエラー表示部へ表示する
}

} // namespace MC
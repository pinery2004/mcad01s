//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: M_CadApi.cpp
//
//		
//	コマンド処理用バックグラウンドのタスクの起動
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbDefine.h"

#include "MCAD.h"
#include "MtAskForPoint.h"
#include "MbCriticalSection.h" 

#include "MtInpEvent.h"
#include "MtInpEventQueue.h"
#include "MtPrcEventQueue.h"

#include "MgPoint.h"

#include "MmLib.h"

namespace MC
{

void MtCmdLine();

//bool	MtIsMCadExiting( void);
bool	MtPostMtInpEvent( const MtInpEvent &theEvent );

static UINT	z_CurEventMask = 0;

#ifdef	MS_CMDMENU_STR_TYPE
struct MT_InternalCmdList *z_pInternalCmdList;
static MINT	z_nInternalCmdList;

////////////////////////////////////////////////////////////////
//	コマンドリストのポインタ設定
void MtSetCmdList( struct MT_InternalCmdList *pInternalCmdList, MINT nInternalCmdList)
{
	z_pInternalCmdList = pInternalCmdList;
	z_nInternalCmdList = nInternalCmdList;
}

////////////////////////////////////////////////////////////////
//	コマンドID(メニューID)よりコマンド名を取得する
MCHAR* GetGlobalName( MINT nCmdIndex)
{
	MINT	ic;
	for ( ic=0; ic<z_nInternalCmdList; ic++) {
		if ( z_pInternalCmdList[ic].cmdIndex == nCmdIndex) {
			return z_pInternalCmdList[ic].globalName;
		}
	}
	return NULL;
}
#endif

///////////////////////////////////////////////////////////////////////////////
//	(Command_1) 入力操作からの処理対象メッセージをイベントキューに追加する
MINT MtSendMessage(
				MINT		message,							// メッセージ
				MINT		nFlags,								// 種類フラグ
				MgPoint2	*pPtMR								// マウス実座標 または NULL
				)
{
	MgPoint2*	pPtR;
	MgPoint2	PtZ = MgPoint2( 0., 0.);
//	MgVect2	PtZ = MgVect2( 0., 0.);

#ifdef	MS_CMDMENU_STR_TYPE
	MCHAR* pcMenuStr;
#endif

	if ( pPtMR == 0)
		pPtR = &PtZ;
	else 
		pPtR = pPtMR;

	int hadevent = 1;
	MtInpEvent event( 0, nFlags, *pPtR);

	switch ( message)
	{
	case WM_CHAR:
		event.m_type = MT_EVM_KEYCHAR;
		break;
	case WM_LBUTTONDOWN:										// マウス左ボタンダウン
		event.m_type = MT_EVM_LBUTTONDOWN;
		break;
	case WM_MBUTTONDOWN:										// マウス中ボタンダウン
		event.m_type = MT_EVM_MBUTTONDOWN;
		break;
	case WM_RBUTTONDOWN:										// マウス右ボタンダウン
		event.m_type = MT_EVM_RBUTTONDOWN;
		break;

	case WM_LBUTTONUP:											// マウス左ボタンアップ
		event.m_type = MT_EVM_LBUTTONUP;
		break;
	case WM_MBUTTONUP:											// マウス中ボタンアップ
		event.m_type = MT_EVM_MBUTTONUP;
		break;
	case WM_RBUTTONUP:											// マウス右ボタンアップ
		event.m_type = MT_EVM_RBUTTONUP;
		break;

	case WM_LBUTTONDBLCLK:										// マウス左ボタンダブルクリック
		event.m_type = MT_EVM_LBUTTONDBLCLK;
		break;
	case WM_MBUTTONDBLCLK:										// マウス中ボタンダブルクリック
		event.m_type = MT_EVM_MBUTTONDBLCLK;
		break;
	case WM_RBUTTONDBLCLK:										// マウス右ボタンダブルクリック
		event.m_type = MT_EVM_RBUTTONDBLCLK;
		break;

	case WM_MOUSEMOVE:											// マウス移動
		event.m_type = MT_EVM_MOUSEMOVE;
		break;
	case WM_MOUSEWHEEL:											// マウスホイール回転
		event.m_type = MT_EVM_MOUSEWHEEL;
		break;

	//case WM_POPUPCOMMAND:										// ポップアップメニューコマンド選択
	//	event.m_type = MT_EVM_POPUPCOMMAND;
	//	break;

	//case WM_AUXMENU:											// 
	//	event.m_type = MT_EVM_AUXMENU;
	//	break;

	case WM_SIZE:												// ウィンドウサイズ変更
		event.m_type = MT_EVM_SIZE;
		break;
	case WM_MOVE:												// ウィンドウ移動
		event.m_type = MT_EVM_MOVE;
		break;
	case WM_ACTIVATE:											// ウィンドウアクティブ
		event.m_type = MT_EVM_ACTIVATE;
		break;
	case WM_TIMER:												// タイマー
		event.m_type = MT_EVM_TIMER;
		break;
	case WM_DROPFILES:											// ドロップファイル
		event.m_type = MT_EVM_DROPFILES;
		break;

	case WM_APP_SYSTEMSTOP:
		// システム終了
		event.m_type = MT_EVM_SYSTEMSTOP;					// システムストップ(ダミーメッセージ)
		break;

	case WM_CMDMENUP:											// メニューコマンド選択
		event.m_type = MT_EVM_MENUCOMMAND;
#ifdef	MS_CMDMENU_STR_TYPE
		pcMenuStr	= GetGlobalName( nFlags);					// メニューIDよりコマンド名取得
		if ( pcMenuStr) {
			event.m_pcMenuStr = (MCHAR* )new char[(Mstrlen( pcMenuStr) + 1)*sizeof( MCHAR)];
			Mstrcpy( event.m_pcMenuStr, pcMenuStr);
		} else {
			hadevent = 0;
		}
#endif
		break;

	default:
		hadevent = 0;
	}

	if ( hadevent) {												// 入力イベントキューにイベントを追加	
		if ( z_CurEventMask == 0 || ( z_CurEventMask & event.m_type)) {
			MtPostMtInpEvent( event );								// **?? ここで止めるとキューが溜まりすぎるのかループのような状況になる
		}
	}
	return(0);
}

MtInpEventQueueInterface *MtGetMtInpEventQueue();
//
// 入力イベントキューにイベントを追加	
bool MtPostMtInpEvent( const MtInpEvent &theEvent)
{
	MtInpEvent *pevent = new MtInpEvent( theEvent);

	return  MtGetMtInpEventQueue()->PostEvent( pevent );
}

} // namespace MC
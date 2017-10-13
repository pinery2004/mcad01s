//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: M_CadApi.cpp
//		
//	機能	コマンド処理用バックグラウンドのタスクの起動
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

#include "MmLib.h"
#include "MdCmds.h"

#include "..\\MCAD\\res\\MCADrc2.h"

namespace MC
{

void MtCmdLine();

bool	MtIsMCadExiting( void);
bool	MtPostMtInpEvent( const MtInpEvent &theEvent );

MtInpEventQueueInterface *MtGetMtInpEventQueue();

#ifndef	MS_CMDMENU_STR_TYPE

	struct	MT_InternalCmdList *z_pInternalCmdList;
	MINT	z_nInternalCmdList;

	//	コマンドリストのポインタ設定
	void MtSetCmdList( struct MT_InternalCmdList *pInternalCmdList, MINT nInternalCmdList)
	{
		z_pInternalCmdList = pInternalCmdList;
		z_nInternalCmdList = nInternalCmdList;
	}

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
////////////////////////////////////////////////////////////////////////////
//	(Command_2) メニュー入力　または　座標入力イベントを待ち
//　イベントをメニューキューとコマンドキューに分けてResBufを追加する
//	メニューキューに入力がある場合はコマンドキューをクリア後、
//	コマンドキューにキャンセルコマンドとメニューコマンドを追加する
// 
MINT MtWaitLoop( void)
{
	struct MtPrcEvent NextItem;

	if ( MtIsMCadExiting())										// システム終了　0
		return 0;

	MtInpEvent *pEvent = NULL;

	// 複数の起動を防ぐ
	static MbCriticalSection theSection;
	MbSynchroGuard theGuard( &theSection );

	int retcode = 0;

	if( ! MtGetMenuQueue()->IsEmpty()) {						// メニューキューに入力がある場合
//		const MCHAR* szMenuToken = MtGetMenuQueue()->PeekString();

		MtGetCommandQueue()->Flush();						// コマンドキューをクリア後、コマンドキューの末尾に

		NextItem.m_restype = MTRT_CAN;
		MtGetCommandQueue()->AddItem( &NextItem );			// コマンドキューにキャンセルコマンド(MTRT_CAN)を追加し、

		MtGetMenuQueue()->PopItem( &NextItem);				// メニューキューの先頭コマンドを取得後、メニューキーから取り除き
		MtGetCommandQueue()->AddItem( &NextItem );			// コマンドキューにメニューキューの先頭コマンドを追加する

		retcode=1;  
		MQUIT;
	}
	if( !MtGetCommandQueue()->IsEmpty()) {						// コマンドキューに入力がある場合
		retcode=1;  
		MQUIT;
	}

	pEvent = MtGetMtInpEventQueue()->WaitEvent();				// 入力イベントを待つ
	if ( pEvent != NULL ) {
		MtProcessEvent( pEvent);
		delete pEvent;
		retcode = 1;
	}
	retcode = 1;
exit:
	return( retcode);
}

////////////////////////////////////////////////////////////////////////////////////////
//	(Command_2.1) 入力イベントに対応して、コマンドキューまたはメニューキューに追加する
void MtProcessEvent( MtInpEvent *event ) 
{
	MtPrcEvent tRb;
	MtPrcEventQueue	RbQue( 10);
	MINT	iForm;								// 形式　( 1:座標、)	
	MINT	iChar;
	MINT	nChar;
	MINT	nRepCnt;
#ifndef	MS_CMDMENU_STR_TYPE
	MCHAR*	pcMenuStr;
#endif

	iForm = 1;
	switch( event->GetType() ) 									// 入力イベントにより対応する処理を実行
	{

	case MT_EVM_KEYCHAR:
 		tRb.m_restype = MTRT_KEYCHAR;							// マウス左ボタンダウン
		iChar = event->GetKey();

		nChar = *(MINT*)(&event->m_pt.x);
		nRepCnt = *(MINT*)(&event->m_pt.y);

//		iChar = 'a';

		//switch ( event->GetKey() ) 
		//	{
		//	case '\r':
		//		tRb.m_restype = MTRT_MENUCOMMAND;								// コマンドタイプを座標(MTRT_LBTNDWN)とする
		//		tRb.m_resval.rstring = (MCHAR* )new char[sizeof( MCHAR)];
		//		tRb.m_resval.rstring[0] = NULL;
		//		MtGetCommandQueue()->Push( &tRb );
		//		break;
		//	} // end of switch ( event->GetKey() );
		iForm = 0;
        break;

 	case MT_EVM_LBUTTONDOWN:
 		tRb.m_restype = MTRT_LBTNDWN;							// マウス左ボタンダウン
		break;

	case MT_EVM_MBUTTONDOWN:
		tRb.m_restype = MTRT_MBTNDWN;							// マウス中ボタンダウン
		break;

	case MT_EVM_RBUTTONDOWN:
		tRb.m_restype = MTRT_RBTNDWN;							// マウス中ボタンダウン
		break;

	case MT_EVM_LBUTTONUP:
		tRb.m_restype = MTRT_LBTNUP;							// マウス左ボタンアップ
		break;

	case MT_EVM_MBUTTONUP:
		tRb.m_restype = MTRT_MBTNUP;							// マウス左ボタンアップ
		break;

	case MT_EVM_RBUTTONUP:
		tRb.m_restype = MTRT_RBTNUP;							// マウス左ボタンアップ
		break;

	case MT_EVM_LBUTTONDBLCLK:
		tRb.m_restype = MTRT_LBTNDBL;							// マウス左ボタンダブルクリック
		break;

	case MT_EVM_MBUTTONDBLCLK:
		tRb.m_restype = MTRT_MBTNDBL;							// マウス左ボタンダブルクリック
		break;

	case MT_EVM_RBUTTONDBLCLK:
		tRb.m_restype = MTRT_RBTNDBL;							// マウス左ボタンダブルクリック
		break;

	case MT_EVM_MOUSEMOVE:
		tRb.m_restype = MTRT_MOUSEMOVE;							// マウス移動
		break;
	case MT_EVM_MOUSEWHEEL:
		tRb.m_restype = MTRT_MOUSEWHEEL;						// マウスホイール
		break;

    case MT_EVM_MENUCOMMAND:									// 入力イベントがメニュー入力の場合
    case MT_EVM_POPUPCOMMAND:
////E		if( z_IgnoreMenuWQuote && event->m_pcMenuStr[0] == '\'' ) 
////E			break;
//
//		//tRb.m_restype = MTRT_RBTNDWN;						// コマンドタイプを座標(MTRT_RBTNDWN)とする
//		//tRb.m_nflag = event->GetNFlag();					// nflag
//		//event->GetPoint( tRb.m_resval.rpoint);			// コマンド入力座標を入力イベント座標とする
//		//MtGetCommandQueue()->Push( &tRb);					// コマンドキューに追加する

#ifdef	MS_CMDMENU_STR_TYPE
		MtMnuStrToRB( &RbQue, event->m_pcMenuStr);			// メニューキューに追加する
		MBFREE( event->m_pcMenuStr); 
#else
		pcMenuStr = GetGlobalName( event->GetNFlag());	// メニューIDよりコマンド名取得
		MtMnuStrToRB( &RbQue, pcMenuStr);						// メニューキューに追加する
#endif

		if( RbQue.GetCount() != 0) {
			MtGetMenuQueue()->AddQueue( &RbQue);
			RbQue.RemoveAll();
		}
		iForm = 0;
		break;

	case MT_EVM_SIZE:
		tRb.m_restype = MTRT_SIZE;								// ウィンドウサイズ変更
		iForm = 0;
		break;
	case MT_EVM_SIZEMAX:
		tRb.m_restype = MTRT_SIZEMAX;							// ウィンドウ最大化(未)
		iForm = 0;
		break;
	case MT_EVM_SIZEICON:
		tRb.m_restype = MTRT_SIZEICON;							// ウィンドウアイコン化			(未)
		iForm = 0;
		break;
	case MT_EVM_MOVE:
		tRb.m_restype = MTRT_MOVE;								// ウィンドウ移動
		iForm = 0;
		break;
	case MT_EVM_ACTIVATE:
		tRb.m_restype = MTRT_ACTIVE;							// ウィンドウアクティブ
		iForm = 0;
		break;
	case MT_EVM_TIMER:
		tRb.m_restype = MTRT_TIMER;								// タイマーイベント
		iForm = 0;
		break;
	case MT_EVM_DROPFILES:
		tRb.m_restype = MTRT_DROPFILE;							// ドロップファイルイベント
		iForm = 0;
		break;
	default:
		iForm = 0;
		break;	
	}
//
	switch ( iForm)
	{
	case 1:
		tRb.m_nflag = event->GetNFlag();						// nflag
		event->GetPoint( tRb.m_resval.rpoint);					// コマンド入力座標を入力イベント座標とする
		MtGetCommandQueue()->Push( &tRb);						// コマンドキューに追加する
		break;
	}
}

// 指定したコマンド名のメニューコマンドをメニューキューに登録型で返す
void MtMnuStrToRB(
						MtPrcEventQueue* i_pRbQue,	// メニューコマンドキュー
						MCHAR*			 i_pcMnuStr	// コマンド
				)
{
	MINT iSt;
	if ( i_pcMnuStr != NULL && i_pcMnuStr[0] != 0) {

#ifdef MS_CMDMENU_STR_TYPE
		MtInpEvent theMtInpEvent( MT_EVM_KEYCHAR, 0, i_pcMnuStr);
		MtProcessEvent( &theMtInpEvent);
#endif
 
		iSt = MtBuildPrcEvent( i_pRbQue, MTRT_MENUCOMMAND, i_pcMnuStr, 0);
		if ( iSt == NULL) {
			ASSERT( iSt);
			MdCmdErrorPrompt( CMD_ERR_UNABLE, 0);					// The user shouldn't need to care what went wrong internally.  OK?
		}
	}
}

} // namespace MC
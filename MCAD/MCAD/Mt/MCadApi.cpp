//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MCadApi.cpp
//
//	機能	コマンド処理用バックグラウンドのタスクの起動			
//
//  K.Matsu           08/01/04    Created.
//
//==========================================================================================
#include "stdafx.h"
#include "MCAD.h"
#include "..\\MCad\\resource.h"
#include "MtAskForPoint.h"
#include "MmCmdMsg.h"
#include "MtPrcEventQueue.h"
#include "MCadApi.h"

#include "MCADLib.h"

#include "McCmd.h"

namespace MC
{

void MtCmdLine();
void MtSetCmdList( struct MT_InternalCmdList *pInternalCmdList, MINT nInternalCmdList);

MINT			z_userbreak;

struct MT_InternalCmdList z_pInternalCmdList[] = {
	{ Mstr( "_CancelCmd")		,MCmdWait			,0	 ,NULL  ,IDC_CANCELCMD },
	{ Mstr( "_CreateKabeO")		,MCmdWallOuter		,0	 ,NULL  ,IDP_OUTERWALL },		// 外壁入力
	{ Mstr( "_CreateKabeI")		,MCmdWallInner		,0	 ,NULL  ,IDP_INNERWALL },		// 内壁入力
	{ Mstr( "_CreatePts")		,MCmdPts			,0	 ,NULL  ,IDC_PARTSCREATE },		// 部材入力
	{ Mstr( "_GTate1")			,MCmdGaiTateSash	,0	 ,NULL  ,IDP_GAITATESASH },		// 外部建具（サッシ）入力
	{ Mstr( "_GTate2")			,MCmdGaiTateDoor	,0	 ,NULL  ,IDP_GAITATEDOOR },		// 外部建具（ドア）入力
	{ Mstr( "_NTate")			,MCmdNaiTate		,0	 ,NULL  ,IDP_NAITATE },			// 内部建具入力
	{ Mstr( "_Demado")			,MCmdDemado			,0	 ,NULL  ,IDP_BAYWINDOW },		// 出窓入力
	{ Mstr( "_CreateRoof")		,MCmdRoof			,0	 ,NULL  ,IDP_ROOF },			// 屋根入力
	{ Mstr( "_YaneFutai")		,MCmdRoofSupplement	,0	 ,NULL  ,IDP_ROOFSUPPLEMENT },	// 屋根付帯入力
	{ Mstr( "_Kaikou")			,MCmdOpening		,0	 ,NULL  ,IDP_OPENING },			// 開口部入力
	{ Mstr( "_KKaidan")			,MCmdStairs			,0	 ,NULL  ,IDP_STAIRS },			// 階段入力
	{ Mstr( "_KBalcony")		,MCmdBalcony		,0	 ,NULL  ,IDP_BALCONY },			// バルコニー入力
	{ Mstr( "_KDebug")			,MCmdDebug			,0	 ,NULL  ,IDP_P0130 },			// デバッグ用
	{ Mstr( "_KYane")			,MCmdStructRoof		,0	 ,NULL  ,IDP_STRUCTROOF },		// 構造屋根部材入力
	{ Mstr( "_KTenjo")			,MCmdStructCeiling	,0	 ,NULL  ,IDP_STRUCTCEILING },	// 構造天井部材入力
	{ Mstr( "_KKabe")			,MCmdStructWall		,0	 ,NULL  ,IDP_STRUCTWALL },		// 構造壁部材入力
	{ Mstr( "_KYuka")			,MCmdStructFloor	,0	 ,NULL  ,IDP_STRUCTFLOOR },		// 構造床部材入力
	{ Mstr( "_KDodai")			,MCmdStructDodai	,0	 ,NULL  ,IDP_STRUCTDODAI },		// 構造土台部材入力
	{ Mstr( "_KKiso")			,MCmdKiso			,0	 ,NULL  ,IDP_KISO },			// 基礎入力
	{ Mstr( "_Wasitu")			,MCmdWasitu			,0	 ,NULL  ,IDP_WASITU },			// 和室入力
//
	{ Mstr( "_Kitchen")			,MCmdKitchen		,0	 ,NULL  ,IDP_KITCHEN },			// キッチン入力
	{ Mstr( "_Senmen")			,MCmdSenmen			,0	 ,NULL  ,IDP_SENMEN },			// 洗面・浴室入力
	{ Mstr( "_Toile")			,MCmdToile			,0	 ,NULL  ,IDP_TOILE },			// トイレ入力
	{ Mstr( "_Settubi")			,MCmdSettubi		,0	 ,NULL  ,IDP_SETUBI },			// 設備入力
	{ Mstr( "_Denki")			,MCmdDenki			,0	 ,NULL  ,IDP_DENKI },			// 電気入力
	{ Mstr( "_Pipe")			,MCmdPipe			,0	 ,NULL  ,IDP_PIPE },			// 配管入力
//
	{ Mstr( "_SunpouSen")		,MCmdSunpouSen		,0	 ,NULL  ,IDP_SUNPOUSEN },		// 寸法線入力
	{ Mstr( "_HikidasiSen")		,MCmdHikidasiSen	,0	 ,NULL  ,IDP_HIKIDASISEN },		// 引出線型注記入力
//
	{ Mstr( "_Zumen")			,MCmdZumen			,0	 ,NULL  ,IDP_ZUMEN },			// 図面入力
//
	{ Mstr( "_MCreate")			,MCmdMdCreate		,0	 ,NULL  ,IDC_MODECREATE },		// 創成モード
	{ Mstr( "_MDelete")			,MCmdMdDelete		,0	 ,NULL  ,IDC_MODEDELETE },		// 削除モード
	{ Mstr( "_MModify")			,MCmdMdModify		,0	 ,NULL  ,IDC_MODEMODIFY },		// 修正モード
	{ Mstr( "_MMove")			,MCmdMdMove			,0	 ,NULL  ,IDC_MODEMOVE },		// 移動モード
	{ Mstr( "_MCopy")			,MCmdMdCopy			,0	 ,NULL  ,IDC_MODECOPY },		// 複写モード
	{ Mstr( "_MProperty")		,MCmdMdProperty		,0	 ,NULL  ,IDC_MODEPROPERTY },	// 属性表示モード
	{ Mstr( "_Undo")			,MCmdUndo			,0	 ,NULL  ,IDC_UNDO },			// アンドゥ
	{ Mstr( "_Redo")			,MCmdRedo			,0	 ,NULL  ,IDC_REDO },			// リドゥ
//	{ Mstr( "_Cancel1pt")		,MCmdCancel1Pt		,0	 ,NULL  ,IDC_CANCEL1PT },		// １点キャンセル
//	{ Mstr( "_CancelAll")		,MCmdCancelAll		,0	 ,NULL  ,IDC_CANCELALL },		// 全点キャンセル
//	{ Mstr( "_InpKai")			,MCmdInpKai			,0	 ,NULL  ,IDC_INPUTKAI },		// 階入力
//	{ Mstr( "_InpAll")			,MCmdInpAll			,0	 ,NULL  ,IDC_INPUTALL },		// 全階入力
//	{ Mstr( "_InpEnd")			,MCmdInpEnd			,0	 ,NULL  ,IDC_INPUTEND },		// 入力終了
//	{ Mstr( "_BHeight")			,MCmdBaseHeight		,0	 ,NULL  ,IDC_BASEHIGHT },		// 基準高さ入力
	{ Mstr( "_GridNum")			,MCmdGridNum		,0	 ,NULL  ,IDC_GRIDNUM },			// グリッド数設定
	{ Mstr( "_Opt")				,MCmdOpt			,0	 ,NULL  ,IDC_OPTION },			// オプション設定
	{ Mstr( "_Hist")			,MCmdHist			,0	 ,NULL  ,IDC_HIST },			// 履歴管理
	{ Mstr( "_Tenkai")			,MCmdTenkai			,0	 ,NULL  ,IDC_TENKAI },			// 住棟展開
	{ Mstr( "_PtsEdit")			,MCmdPtsEdit		,0	 ,NULL  ,IDC_PARTSEDIT },		// グリッドコントロールテスト
	{ Mstr( "_MLoadDxfFile")	,MCmdLoadDxfFile	,0	 ,NULL  ,IDC_LOAD_DXF_FILE },	// ＤＸＦファイルの読込み
	{ Mstr( "_ModLineLength")	,MCmdMdLineLength	,0	 ,NULL  ,IDC_MOD_LINELENGTH },	// 横置部材の長さ調整
	{ Mstr( "_ModPanelNo")		,MCmdMdPanelNo		,0	 ,NULL  ,IDC_MOD_PANELNO },		// パネル番号の修正
	{ Mstr( "_ModTest001")		,MCmdTest001		,0	 ,NULL  ,IDC_TEST_001 }			// テスト用００１
};
MINT z_nInternalCmdList = sizeof(z_pInternalCmdList)/sizeof(z_pInternalCmdList[0]);

// ===================================================================


static CWinThread		*z_pThread;
static DWORD			z_dwThreadID;
static HANDLE			z_hResumeEvent;

void MtCmdOpen()
{
	MtSetCmdList( z_pInternalCmdList, z_nInternalCmdList);
	MtLaunch();
}

void MtCmdClose()
{
	MtCloseThread();
}

//
//	別スレッドでMtThreadProcedureを起動する
bool MtLaunch( void)
{
	z_hResumeEvent = CreateEvent( NULL,						// security attributes
								  FALSE,					// this means an auto-reset object
								  FALSE,					// initially in non-signaled state
								  NULL);					// unnamed object (we'll just keep the handle)

	ASSERT( z_hResumeEvent != NULL);
	if ( z_hResumeEvent)
	{
		//スレッドを休止中で生成
		z_pThread = AfxBeginThread( (UINT(*)(LPVOID))MtCmdThread,
									NULL,
									THREAD_PRIORITY_NORMAL,
									0,
									CREATE_SUSPENDED);
		ASSERT( z_pThread != NULL);

		//スレッドの自動削除を禁止
		z_pThread->m_bAutoDelete = FALSE;
		z_dwThreadID = z_pThread->m_nThreadID;
	
		//スレッドを実行状態にする
		z_pThread->ResumeThread();
	}
	return ( z_pThread != NULL);
}

void MtCloseThread( void)
{
	z_pThread->ResumeThread();									// 休止中のスレッドを実行状態にする
	HANDLE hThread = z_pThread ->m_hThread;						// スレッドのハンドルを取得する
	MtStopThread();												// スレッドに終了を指示
	MtSendMessage( WM_APP_SYSTEMSTOP, 0, NULL);					// 
	WaitForSingleObject( hThread, INFINITE);					// スレッドが終了するまで待つ	
	delete z_pThread;											// スレッドオブジェクトを削除
}


static MINT				z_IgnoreMenuWQuote;

static volatile MINT	z_iCommand;

//	別タスクの処理であり、入力イベントを待ちながら実行する。
MINT MtCmdThread( void)
{
	MINT	ist = 0;
	MINT	irt;
	MCHAR	fs1[MC_MCADBUF], fs2[MC_MCADBUF];
	MgPoint2	pt1;
	
	MFOREVER {
		irt = MtAskForPoint( &pt1);								// 入力イベントに対応したコマンド名取得
		if ( irt == MTRT_CAN) continue;							// キャンセル
		if ( irt == MTRT_SYSTEMSTOP) break;						// システム終了

		*fs1 = 0;

		switch( irt) {											// 入力イベントの種類により
			//case MT_RTNONE:
			//	Mstrcpy( fs2, Mstr( "?1"));
			//	break;

			case MTRT_ERROR:
				Mstrcpy( fs2, Mstr( "?2"));
				break;

			case MTRT_LBTNDWN:									// マウスにより座標入力
				Mstrcpy( fs2, Mstr( "?3"));
				break;

			case MTRT_MBTNDWN:									// マウスにより座標入力
				Mstrcpy( fs2, Mstr( "?4"));
				break;

			case MTRT_RBTNDWN:									// マウスにより座標入力
				Mstrcpy( fs2, Mstr( "?5"));
				break;

			case MTRT_KWORD:									// メニュー入力
				MtGetInp( fs1);
				break;
		}

		try {
			MtDoOneCommand( fs1,0);								// 対応するコマンドを実行する

		} catch ( ...) {
			Base::MbCmdError( MBCstr( "MCadApi"), MC_ERR_COMMAND);

		}

	}
	return ist;
}

/*----------------------------------------------------------------------------*/
//
//	コマンド名に対応するコマンドを実行する
//	コマンド名が登録されていない場合は無視する
MINT MtDoOneCommand
						(
							MCHAR*	argcmd, 
							MINT	pushlsp
						) 
{
	MINT ic;
	MINT (*fnptr)(void);
	MINT ret = MTRT_ERROR;

	for ( ic=0; ic<z_nInternalCmdList; ic++) {
		if (Mstrcmp( z_pInternalCmdList[ic].globalName, argcmd) == 0) {
			fnptr = (MINT(*)(void))(z_pInternalCmdList[ic].funcPtr);
			ret = MTRT_NORM;
			if (fnptr)
				(*fnptr)();
			break;
		}
	}

    return(ret);
}

} // namespace MC
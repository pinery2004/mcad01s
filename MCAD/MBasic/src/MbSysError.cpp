//==========================================================================================
//  Copyright ( C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MbSysErrorMsg.cpp
//
//		システムエラー表示
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#pragma warning( disable : 4996 )
#include "stdafx.h"
#define DLL_EXPORT_BASE_DO
#include "MbBasic.h"
#include "MgDefine.h"
#include "MbFile.h"

namespace MC
{

//===========================================================================
//	MCAD.iniファイルを読み込む
//
//LoadMCADIniFile

//===========================================================================
//	エラー表示後に処理を終了する
//
void 	Base::MbSysError( 
						const char*	i_cFunction,	// エラー関数名
						MINT		i_iErrorCode	// エラーコード
				  )
{
	MINT	ist;
	MCHAR	Msg[256];							// ﾒｯｾｰｼﾞ

	ist = Code::Mmbstowcs( i_cFunction, Msg, 256);
	Mstrcat( Msg, Mstr( "　\n\n"));

	switch( i_iErrorCode)
	{
	case MC_ERR_POINTER:
		Mstrcat( Msg, Mstr( "メモリ取得エラー"));
		break;
	case MC_ERR_OVERFLOW:
		Mstrcat( Msg, Mstr( "領域オーバフローエラー"));
		break;
	case MC_ERR_ID:
		Mstrcat( Msg, Mstr( "ＩＤ誤りエラー"));
		break;
	case MC_ERR_INPUT:
		Mstrcat( Msg, Mstr( "入力エラー"));
		break;
	case MC_ERR_NUM_OVER:
		Mstrcat( Msg, Mstr( "処理可能個数超えエラー"));
		break;
	case MC_ERR_VECT_ZERO:
		Mstrcat( Msg, Mstr( "ゼロベクトルで処理不可エラー"));
		break;
	case MC_ERR_INPUT_ZERO:
		Mstrcat( Msg, Mstr( "数値ゼロで処理不可エラー"));
		break;
	case MC_ERR_OFFSET:
		Mstrcat( Msg, Mstr( "オフセット誤りエラー"));
		break;
	default:
		Mstrcat( Msg, Mstr( "その他エラー"));
	}		
	Mstrcat( Msg, Mstr( "　\n\nシステム終了します　"));

	ist = AfxMessageBox( Msg, MB_ICONSTOP);
	__debugbreak();
	AfxGetMainWnd( )->PostMessage( WM_CLOSE, 0, 0);				// メインウィンドウ取得・終了
}

//
//===========================================================================
//	コマンドエラー表示
//
void 	Base::MbCmdError( 
						const char*	i_cFunction,	// エラー関数名
						MINT		i_iErrorCode	// エラーコード
				  )
{
	MINT	ist;
	MCHAR	Msg[256];							// ﾒｯｾｰｼﾞ

	ist = Code::Mmbstowcs( i_cFunction, Msg, 256);
	Mstrcat( Msg, Mstr( "　\n\n"));

	switch( i_iErrorCode)
	{
	case MC_ERR_COMMAND:
		Mstrcat( Msg, Mstr( "コマンド処理エラー"));
		break;
	default:
		Mstrcat( Msg, Mstr( "コマンド処理エラー"));
	}		
	ist = AfxMessageBox( Msg, MB_ICONSTOP);
}

//
//===========================================================================
//	エラーリターンブレークポイント用
//
void	Base::MbErrorReturn( void)
{
	__debugbreak();
}

} // namespace MC

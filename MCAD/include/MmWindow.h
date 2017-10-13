#pragma once

#include "MmDefine.h"

namespace MC
{

	/////////////////////////////////////////////////////////////////////////////
	//	コマンド補助処理                                                       //
	/////////////////////////////////////////////////////////////////////////////

#ifdef DLL_EXPORT
	#undef DLL_EXPORT
#endif

#ifdef DLL_EXPORT_MC_WINDOW_DO
	#pragma message( "<<< MC::Window・dll_EXport >>>")
	#define DLL_EXPORT							__declspec( dllexport)
#else
	#ifdef DLL_NO_IMPORT_MD_DO
		#define DLL_EXPORT
	#else
//		#pragma message( "=== MC::Window・dll_IMport ===")
		#define DLL_EXPORT						__declspec( dllimport)
	#endif
#endif

class DLL_EXPORT Window
{
public:
	////////////////////////////////////////////////////////////////////////////
	//  カーソルを設定する

	static MINT SetCursor(
					class	CWnd*		i_pWnd
					);

	/////////////////////////////////////////////////////////////////////////////
	//	新規ウィンドウ作成時のクロスヘアーカーソルの初期化

	static void InitCrossHair();

	/////////////////////////////////////////////////////////////////////////////
	//	クロスヘアーカーソルを消去する

	static void EraseCrossHair();

	/////////////////////////////////////////////////////////////////////////////
	//	クロスヘアーカーソルを表示する

	static void DrawCrossHair(
					class	MmWndInfo*	i_pWndInfo,	// ウィンドウ管理情報
					class	CPoint		i_ptMouthL	// 論理座標
					);

	/////////////////////////////////////////////////////////////////////////////
	//	クロスヘアーカーソルを再表示する
	static void ReDrawCrossHair();

	/////////////////////////////////////////////////////////////////////////////
	//	クロスヘアカーソルを表示/消去する

	static MINT DispCrossHair(
							MINT		i_ictl,		// 表示制御	 1 : 表示、0:再表示、-1 : 消去
					class	MmWndInfo*	i_pWndInfo,	// ウィンドウ管理情報(表示制御 == 1) または NULL(表示制御 <= 0)　
					class	CPoint*		i_pptLMouth	// マウス位置論理座標(表示制御 == 1) または NULL(表示制御 <= 0)
					);

	/////////////////////////////////////////////////////////////////////////////
	//	新規ウィンドウ作成時の初期化 (ビューの初期表示時に使用)
	//	ビューの初期表示時にラバーバンド図形が表示されていない事を設定する

	static void InitDragging();

	/////////////////////////////////////////////////////////////////////////////
	//	ドラッギングモード終了（ラバーバンド図形表示モード終了）

	static void DragModeEnd();

	/////////////////////////////////////////////////////////////////////////////
	//	ドラッギングモード確認（ラバーバンド図形表示モード確認）

	static bool GetDragMode();

	/////////////////////////////////////////////////////////////////////////////
	//	ラバーバンド図形を消去する

	static void EraseDragging();

	/////////////////////////////////////////////////////////////////////////////
	//	ラバーバンド図形を表示する

	static void DrawDragging(
					class	MmWndInfo*	i_pWndInfo,	// ウィンドウ管理情報
					class	MgPoint2	i_ptMouthR	// マウス位置実座標
					);

	/////////////////////////////////////////////////////////////////////////////
	//	ラバーバンド図形を再表示する
	//	途中ポイントの座標はカレントの座標変換マトリックスで座標変換した論理座標で
	//	最終ポイントはカレントのマウス論理座標で描く

	static void ReDrawDragging();

	/////////////////////////////////////////////////////////////////////////////
	//	ラバーバンド図形を表示してドラッギングモードを開始し、
	//	マウスにて座標入力する

	static MINT DragObject(
							MINT		i_iMode,	// ドラッギング入力モード
					const	void*		i_pFg,		// ドラッギング入力済み座標
					class	MgPoint2*	o_ptGet		// マウス位置実座標
					);

	/////////////////////////////////////////////////////////////////////////////
	//	ラバーバンド図形を表示/再表示/消去する

	static MINT DispDragging(
							MINT		i_ictl,		// 表示制御 1 : 表示、0 : 再表示、-1 : 消去
					class	MmWndInfo*	i_pWndInfo,	// ウィンドウ管理情報
					class	MgPoint2*	i_pptMouthR	// マウス位置実座標
					);

	////////////////////////////////////////////////////////////////////////////
	//	カレント選択の部材形状をクリアする

	static void ClrCurBziFig();

	////////////////////////////////////////////////////////////////////////////
	//	カレント選択の部材形状を設定する

	static void SetCurBziFig(
					class	 MgPolyg2*	i_ppgBziFig	// カレント選択の部材形状
					);

	////////////////////////////////////////////////////////////////////////////
	//	カレント選択の部材形状を返す

	static MgPolyg2* GetCurBziFig();

	/////////////////////////////////////////////////////////////////////////////
	//	新規ウィンドウ作成時のヒット部材表示の初期化

	static void InitHitBuzai();

//S	/////////////////////////////////////////////////////////////////////////////
//	//	ヒット部材の表示モード確認
//
//	bool GetHitBziMode();

	/////////////////////////////////////////////////////////////////////////////
	//	ヒット部材の表示を消去する

	static void EraseHitBzi();

	/////////////////////////////////////////////////////////////////////////////
	//	ヒット部材を表示する

	static void DrawHitBzi(
					class	MmWndInfo*	i_pWndInfo,	// ウィンドウ管理情報
					class	MgPolyg2*	i_pgHitBzi	// ヒット部材の形状
					);

	/////////////////////////////////////////////////////////////////////////////
	//	ヒット部材を再表示する

	static void ReDispHitBzi();

	/////////////////////////////////////////////////////////////////////////////
	//	ヒット部材の形状表示を初期化する（不表示にする）

	static void ResetDispHitBzi();

	/////////////////////////////////////////////////////////////////////////////
	//	ヒット部材の形状を表示/再表示/消去する

	static MINT DispHitBzi(
							MINT		i_ictl,		// 表示制御 1 : 表示、0 : 再表示、-1 : 消去
					class	MmWndInfo*	i_pWndInfo,	// ウィンドウ管理情報(表示制御 == 1) または NULL(表示制御 <= 0)
					class	MgPolyg2*	i_pgHitBzi	// ヒット部材の形状(表示制御 == 1) または NULL(表示制御 <= 0)
					);

	/////////////////////////////////////////////////////////////////////////////
	//  マウス選択部材の表示制御

	static void SetDispSelPts(
							MINT 		i_iCdBuzai	// マウスで選択対象の部材コード、NULL(全)、または -1(非表示)
					);

	/////////////////////////////////////////////////////////////////////////////
	//	マウス選択部材を再表示する

	static void ReDrawHitBzi();

	/////////////////////////////////////////////////////////////////////////////
	//  フォーカスをカレントウィンドウに設定する

	static void CurWndFocus();

};

#ifdef DLL_EXPORT
	#undef DLL_EXPORT
#endif

#ifdef DLL_EXPORT_MC_VIEWINP_DO
	#pragma message( "<<< MC::ViewInp・dll_EXport >>>")
	#define DLL_EXPORT							__declspec( dllexport)
#else
	#ifdef DLL_NO_IMPORT_MD_DO
		#define DLL_EXPORT
	#else
//		#pragma message( "=== MC::ViewInp・dll_IMport ===")
		#define DLL_EXPORT						__declspec( dllimport)
	#endif
#endif

class DLL_EXPORT ViewInp
{
public:
	/////////////////////////////////////////////////////////////////////////////
	//  マウス左ボタンダウン処理

	static MINT LButtonDown( 
					class	CWnd*		i_pWnd,		// ウィンドウのインスタンス
							UINT 		i_nFlags,	// 
							CPoint		i_ptMouthD	// 表示座標
					);

	/////////////////////////////////////////////////////////////////////////////
	//  マウス左ボタンアップ処理

	static MINT LButtonUp(
					class	CWnd*		i_pWnd,		// ウィンドウのインスタンス
							UINT 		i_nFlags,	// 
							CPoint		i_ptMouthD	// 表示座標
					);

	/////////////////////////////////////////////////////////////////////////////
	//  マウス右ボタンダウン処理

	static MINT RButtonDown( 
					class	CWnd*		i_pWnd,		// ウィンドウのインスタンス
							UINT 		i_nFlags,	// 
							CPoint		i_ptMouthD	// 表示座標
					);

	/////////////////////////////////////////////////////////////////////////////
	//  マウス右ボタンアップ処理

	static MINT RButtonUp(
					class	CWnd*		i_pWnd,		// ウィンドウのインスタンス
							UINT 		i_nFlags,	// 
							CPoint		i_ptMouthD	// 表示座標
					);

	/////////////////////////////////////////////////////////////////////////////
	//  マウス中ボタンダウン処理

	static MINT MButtonDown( 
					class	CWnd*		i_pWnd,		// ウィンドウのインスタンス
							UINT 		i_nFlags,	// 
							CPoint		i_ptMouthD	// 表示座標
					);

	/////////////////////////////////////////////////////////////////////////////
	//  マウス中ボタンアップ処理

	static MINT MButtonUp(
					class	CWnd*		i_pWnd,		// ウィンドウのインスタンス
							UINT 		i_nFlags,	// 
							CPoint		i_ptMouthD	// 表示座標
					);

	/////////////////////////////////////////////////////////////////////////////
	//  マウス左ボタンダブルクリック処理
	static MINT LButtonDblClk(
					class	CWnd*		i_pWnd,		// ウィンドウのインスタンス
							UINT 		i_nFlags,	// 
							CPoint		i_ptMouthD	// 表示座標
					);

	/////////////////////////////////////////////////////////////////////////////
	//  マウス右ボタンダブルクリック処理
	static MINT RButtonDblClk(
					class	CWnd*		i_pWnd,		// ウィンドウのインスタンス
							UINT 		i_nFlags,	// 
							CPoint		i_ptMouthD	// 表示座標
					);

	/////////////////////////////////////////////////////////////////////////////
	//  マウス中ボタンダブルクリック処理
	static MINT MButtonDblClk(
					class	CWnd*		i_pWnd,		// ウィンドウのインスタンス
							UINT 		i_nFlags,	// 
							CPoint		i_ptMouthD	// 表示座標
					);

	/////////////////////////////////////////////////////////////////////////////
	//  マウスホイール回転処理

	static MINT MouseWheel(
					class	CWnd*		i_pWnd,		// ウィンドウのインスタンス
							UINT 		i_nFlags,	// 
							short		i_zDelta,	// 
							CPoint		i_ptMouthD	// 表示座標
					);

	/////////////////////////////////////////////////////////////////////////////
	//  マウス移動処理

	static MINT MouseMove( 
					class	CWnd*		i_pWnd,		// ウィンドウのインスタンス
							UINT 		i_nFlags,	// 
							CPoint		i_ptMouthD	// 表示座標
					);

	/////////////////////////////////////////////////////////////////////////////
	//  キーボードキャラクタ入力処理

	static MINT Char(
					class	CWnd*		i_pWnd,		// ウィンドウのインスタンス
							UINT		i_nChar,	// 
							UINT		i_nRepCnt,	// 
							UINT 		i_nFlags	// 
					);

	/////////////////////////////////////////////////////////////////////////////
	//  キーボードキーアップ処理

	static MINT KeyUp(
					class	CWnd*		i_pWnd,		// ウィンドウのインスタンス
							UINT		i_nChar,	// 
							UINT		i_nRepCnt,	// 
							UINT 		i_nFlags	// 
					);

	/////////////////////////////////////////////////////////////////////////////
	//  キーボードキーダウン処理

	static MINT KeyDown(
					class	CWnd*		i_pWnd,		// ウィンドウのインスタンス
							UINT		i_nChar,	// 
							UINT		i_nRepCnt,	// 
							UINT 		i_nFlags	// 
					);

	/////////////////////////////////////////////////////////////////////////////
	//	ウィンドウのサイズ設定

	static void Size(								// ステイタス 0 : 正常  1 : エラー
					class	CWnd*		i_pWnd,		// ウィンドウのインスタンス
							CPoint		i_ptMouthD,	// 画面サイズ(表示座標)
							CPoint		i_ptMouthL	// 画面サイズ(論理座標)
					);
};


#ifdef DLL_EXPORT
	#undef DLL_EXPORT
#endif

#ifdef DLL_EXPORT_MC_MSG_DO
	#pragma message( "<<< MC::Msg・dll_EXport >>>")
	#define DLL_EXPORT							__declspec( dllexport)
#else
	#ifdef DLL_NO_IMPORT_MD_DO
		#define DLL_EXPORT
	#else
//		#pragma message( "=== MC::Msg・dll_IMport ===")
		#define DLL_EXPORT						__declspec( dllimport)
	#endif
#endif

class DLL_EXPORT Msg
{
public:
	/////////////////////////////////////////////////////////////////////////////
	//	ステータスバーに文字列を表示する

	static void DispSelStatusBar(
							MINT		i_iStb,		// ステイタスバー項目番号
					const	MCHAR*		i_pcStb		// ステイタスバー項目表示内容
					);

	/////////////////////////////////////////////////////////////////////////////
	//  ステイタスバー表示処理

	static void SetSelStatusBar(
							MINT		i_iStb,		// ステイタスバー表示先番号
					const	MCHAR*		i_pcStb		// ステイタスバー表示内容
					);

	static void DispStatusBar();

	/////////////////////////////////////////////////////////////////////////////
	//	ステータスバーにガイダンスメッセージを表示する

	static void GuidanceMsg(
							MINT		i_iMsgNo	// ガイダンスメッセージ ID
					);

	/////////////////////////////////////////////////////////////////////////////
	//	ステータスバーに操作表示メッセージを表示する

	static void OperationMsg(
							MINT		i_iOprtMsgId // 操作表示ID
					);

	/////////////////////////////////////////////////////////////////////////////
	//	ステータスバーにエラーメッセージを表示する
	// 　エラーメッセージ1 + エラーメッセージ2 + プロンプト

	static void ErrorMsg(
							MCHAR*		i_pcErrMsg1,	// エラーメッセージ１　または　NULL
							MINT		i_idErrMsg2,	// エラーメッセージ２表示ID
							MINT		i_imdDisp = -1	// エラー表示モード
														//		1: メッセージボックスへ表示
														//		0: コマンドラインへ表示 (コマンドラインへの表示が無ければメッセージボックスへ表示)
														//	   -1: コマンドラインへ表示
					);

	/////////////////////////////////////////////////////////////////////////////
	//	ステータスバーのエラーメッセージを消去する

	static void ClearErrorMsg();

};

}// namespace MC
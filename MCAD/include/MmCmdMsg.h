// MmCmdMsg.h : GDIメッセージ ファイル
//
#pragma once

namespace MC
{

enum MTCMDSTR{
	MTCMD_COMMENT = 0,
	MTCMD_YUKANETA,
	MTCMD_YUKAKANAMONO
};

} // namespace MC

#define		IDC_UNDO			IDC_P0010		// アンドゥ
#define		IDC_REDO			IDC_P0020		// リドゥ
#define		IDC_CANCEL1PT		IDC_P0030		// １点キャンセル
#define		IDC_CANCELALL		IDC_P0040		// 全点キャンセル
#define		IDC_ZOOMINIT		IDC_P0050		// 初期画面表示
#define		IDC_ZOOMPAN			IDC_P0060		// 画面移動
#define		IDC_ZOOMSIZE		IDC_P0070		// MIN/MAX指示ズーム
#define		IDC_InpKai		IDC_P0080		// 階入力
#define		IDC_InpALL		IDC_P0090		// 全階入力
#define		IDC_InpEND		IDC_P0100		// 入力終了
#define		IDC_MODECREATE		IDC_P0110		// 創成モード
#define		IDC_MODEDELETE		IDC_P0120		// 削除モード
#define		IDC_MODEMOVE		IDC_P0130		// 移動モード
#define		IDC_MODEMODIFY		IDC_P0140		// 修正モード
#define		IDC_MODECOPY		IDC_P0150		// 複写モード
#define		IDC_MODEPROPERTY	IDC_P0160		// 属性表示モード
#define		IDC_BASEHIGHT		IDC_P0170		// 基準高さ入力
#define		IDC_GRIDNUM			IDC_P0180		// グリッド数設定
#define		IDC_OPTION			IDC_P0190		// オプション設定
#define		IDC_HIST			IDC_P0200		// 履歴管理
#define		IDC_TENKAI			IDC_P0210		// 
#define		IDC_PARTSEDIT		IDC_P0220		// グリッドコントロールテスト
#define		IDC_N11				IDC_P0230		// 
#define		IDC_N15				IDC_P0240		// 
#define		IDC_N16				IDC_P0250		// 
#define		IDC_DISP_3D			IDC_P0260		// ３次元表示
#define		IDC_N18				IDC_P0270		// 
#define		IDC_N19				IDC_P0280		// 
#define		IDC_N1A				IDC_P0290		// 
#define		IDC_LOAD_DXF_FILE	IDC_P0300		// ＤＸＦファイル読込み
#define		IDC_N1C				IDC_P0310		// 
#define		IDC_N1D				IDC_P0320		// 
#define		IDC_N1E				IDC_P0330		// 
#define		IDC_MOD_LINELENGTH	IDC_P0340		// 部材長さ調整
#define		IDC_TEST_001		IDC_P0350		// テスト用００１
#define		IDC_MOD_PANELNO		IDC_P0360		// パネル番号変更

#define		IDP_OUTERWALL		IDP_P0010		// 外壁入力
#define		IDP_INNERWALL		IDP_P0020		// 内壁入力
#define		IDP_ROOM			IDP_P0030		// 部屋入力
#define		IDP_GAITATESASH		IDP_P0040		// 外建（サッシ) 入力
#define		IDP_GAITATEDOOR		IDP_P0050		// 外建（ドア) 入力
#define		IDP_NAITATE			IDP_P0060		// 内部建具入力
#define		IDP_BAYWINDOW		IDP_P0070		// 出窓入力
#define		IDP_ROOF			IDP_P0080		// 屋根入力
#define		IDP_ROOFSUPPLEMENT	IDP_P0090		// 屋根付帯入力
#define		IDP_OPENING			IDP_P0100		// 開口部入力
#define		IDP_STAIRS			IDP_P0110		// 階段入力
#define		IDP_BALCONY			IDP_P0120		// バルコニー入力
#define		IDP_N21				IDP_P0130		// 
#define		IDP_STRUCTROOF		IDP_P0140		// 構造屋根入力
#define		IDP_STRUCTCEILING	IDP_P0150		// 構造天井入力
#define		IDP_STRUCTWALL		IDP_P0160		// 構造壁入力
#define		IDP_STRUCTFLOOR		IDP_P0170		// 構造床入力
#define		IDP_STRUCTDODAI		IDP_P0180		// 土台入力
#define		IDP_KISO			IDP_P0190		// 基礎入力
#define		IDP_WASITU			IDP_P0200		// 和室入力
#define		IDP_N22				IDP_P0210		// 
#define		IDP_KITCHEN			IDP_P0220		// キッチン入力
#define		IDP_SENMEN			IDP_P0230		// 洗面・浴室入力
#define		IDP_TOILE			IDP_P0240		// トイレ入力
#define		IDP_SETUBI			IDP_P0250		// 設備入力
#define		IDP_DENKI			IDP_P0260		// 電気入力
#define		IDP_PIPE			IDP_P0270		// 配管入力
#define		IDP_N24				IDP_P0280		// 
#define		IDP_N25				IDP_P0290		// 
#define		IDP_N26				IDP_P0300		// 
#define		IDP_SUNPOUSEN		IDP_P0310		// 寸法線入力
#define		IDP_HIKIDASISEN		IDP_P0320		// 引出注記入力
#define		IDP_N27				IDP_P0330		// 
#define		IDP_N28				IDP_P0340		// 
#define		IDP_ZUMEN			IDP_P0350		// 図面入力
#define		IDP_N29				IDP_P0360		// 
#define		IDP_N30				IDP_P0370		// 

// USER

#define		IDC_USERDEF			1000000
#define		IDC_PARTSCREATE		IDC_USERDEF
#define		IDC_CANCELCMD		(IDC_USERDEF + 1)

// USER MSG

#define WM_MYMESSAGEPARTSEDIT	(WM_USER + 100)
#define WM_MYMESSAGEGRIDNUM		(WM_USER + 101)
#define WM_MYMESSAGESTATUSBAR	(WM_USER + 102)
#define WM_MYMESSAGEOPTION		(WM_USER + 103)
#define WM_MYMESSAGEHIST		(WM_USER + 104)
#define WM_MYMESSAGETENKAI		(WM_USER + 105)

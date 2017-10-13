#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MmInp.h
//
//		入力処理
//
//
//  K.Matsu           07/25/07    Created.
//==========================================================================================
#define MM_MOUSEFIRST		0x0200
#define MM_MOUSEMOVE		0x0200		// マウス移動
#define MM_LBUTTONDOWN		0x0201		// 左ボタンダウン
#define MM_LBUTTONUP		0x0202		// 左ボタンアップ
#define MM_LBUTTONDBLCLK	0x0203		// 左ボタンダブルクリック
#define MM_RBUTTONDOWN		0x0204		// 右ボタンダウン
#define MM_RBUTTONUP		0x0205		// 右ボタンアップ
#define MM_RBUTTONDBLCLK	0x0206		// 右ボタンダブルクリック
#define MM_MBUTTONDOWN		0x0207		// 中ボタンダウン
#define MM_MBUTTONUP		0x0208		// 中ボタンアップ
#define MM_MBUTTONDBLCLK	0x0209		// 中ボタンダブルクリック
#define MM_MOUSEWHEEL		0x020A		// ホイール
#define MM_XBUTTONDOWN		0x020B
#define MM_XBUTTONUP		0x020C
#define MM_XBUTTONDBLCLK	0x020D
#define MM_MOUSELAST		0x020D

#define MM_WINDOWSIZE		0x0100		// ウィンドウサイズ変更
#define MM_WINDOWMAX		0x0101		// ウィンドウ最大化
#define MM_WINDOWMIN		0x0102		// ウィンドウアイコン化
#define MM_WINDOWMOVE		0x0103		// ウィンドウ移動
#define MM_WINDOWDELETE		0x0104		// ウィンドウ削除
#define MM_MENU				0x0105		// メニューイベント
#define MM_WINDOWACTIVE		0x0106		// ウィンドウアクティブ
#define MM_TIMER			0x0107		// タイマーイベント
#define MM_FLOATMENU		0x0108		// フローティングメニューイベント
#define MM_DROPFILE			0x0109		// ドロップファイルイベント
#define MM_KE				0x010A		// キーボード入力
#define MM_KEY31			0x010B		// キーボード入力
#define MM_P_RET			0x0300		// パネル文字列return入力
#define MM_P_BTN			0x0301		// パネルボタン入力
#define MM_P_MNU			0x0302		// パネルメニュー入力
#define MM_P_CHI			0x0303		// パネルチョイス入力
#define MM_P_CYC			0x0304		// パネルサイクル入力
#define MM_P_TGL			0x0305		// パネルトグル入力
#define MM_P_SLI			0x0306		// パネルスライダー入力
#define MM_P_KEY			0x0307		// パネル文字列入力（return以外）
#define MM_P_LST			0x0308		// パネルリストボックス入力
#define MM_P_CBX			0x0309		// パネルコンボボックス入力
#define MM_P_MCX			0x030A		// カレンダー入力
#define MM_P_DTX			0x030B		// DateTimePicker入力
#define MM_J_COM			0x030C		// 他メニューが押された
#define MM_S_BPR			0x040D		// 横：  １ページ右
#define MM_S_BLR			0x040E		//       １行右
#define MM_S_BPL			0x040F		//       １ページ左
#define MM_S_BLL			0x0410		//       １行左
#define MM_S_BPD			0x0411		// 縦：  １ページ下
#define MM_S_BLD			0x0412		//       １行下
#define MM_S_BPU			0x0413		//       １ページ上
#define MM_S_BLU			0x0414		//       １行上
#define MM_S_BTO			0x0415		// 共通：最上行、最右行
#define MM_S_BBO			0x0416		//       最下行、最左行
#define MM_S_BTP			0x0417		//       つまみのドラッグ後
#define MM_S_BTT			0x0418		//       つまみのドラッグ中
#define MM_KYIN 			0x0419		// キーイン表の入力（INPUTで使用
#define MM_USER				0x041A		// ユーザーイベント最小値
// 
//

namespace MC
{

//
////////////////////////////////////////////////////////////////////////////
//	バックグラウンド処理開始宣言
void MmWndKOpen();

////////////////////////////////////////////////////////////////////////////
//	バックグラウンド処理終了宣言
void MmWndKClose();

/////////////////////////////////////////////////////////////////////////////
//	コマンドメニュー処理
void MmWndKCmdMenu(
						MINT		i_idCmd			// コマンドID
				);

} // namespace MC
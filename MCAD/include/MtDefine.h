#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MtDefine.h
//
//	機能	コマンド処理用定数定義
//
//  K.Matsu           08/01/04    Created.
//

// ウィンドウメッセージコード
#define WM_APP_SYSTEMSTOP		WM_APP + 0x0001	// システムストップ
#define	WM_CMDMENUP				WM_APP + 0x0002	// メニューコマンド

// イベントコード
#define MT_EVM_NONE				0		// イベント無し
#define MT_EVM_SYSTEMSTOP		1		// システム終了
#define MT_EVM_KEYCHAR			2		// 文字入力または文字列入力
#define MT_EVM_LBUTTONDOWN		3		// マウス左ボタンダウン
#define MT_EVM_MBUTTONDOWN		4		// マウス中ボタンダウン
#define MT_EVM_RBUTTONDOWN		5		// マウス右ボタンダウン
#define MT_EVM_LBUTTONUP		6		// マウス左ボタンアップ
#define MT_EVM_MBUTTONUP		7		// マウス中ボタンアップ
#define MT_EVM_RBUTTONUP		8		// マウス右ボタンアップ
#define MT_EVM_LBUTTONDBLCLK	9		// マウス左ボタンダブルクリック
#define MT_EVM_MBUTTONDBLCLK	10		// マウス中ボタンダブルクリック
#define MT_EVM_RBUTTONDBLCLK	11		// マウス右ボタンダブルクリック
#define MT_EVM_MOUSEMOVE		12		// マウス移動
#define MT_EVM_MOUSEWHEEL		13		// マウスホイール
#define MT_EVM_MENUCOMMAND		14		// メニューコマンド
#define MT_EVM_POPUPCOMMAND		15		// ポップアップメニューコマンド
#define MT_EVM_AUXMENU			16		// Aux (right) button click
#define MT_EVM_SIZE				17		// ウィンドウサイズ変更
#define MT_EVM_SIZEMAX			18		// ウィンドウ最大化				(未)
#define MT_EVM_SIZEICON			19		// ウィンドウアイコン化			(未)
#define MT_EVM_MOVE				20		// ウィンドウ移動
#define MT_EVM_ACTIVATE			21		// ウィンドウアクティブ
#define MT_EVM_DELETE			22		// ウィンドウ削除
#define MT_EVM_TIMER			23		// タイマーイベント
#define MT_EVM_DROPFILES		24		// ドロップファイルイベント

// イベント処理定義
#define MTRT_ERROR				-5001
#define MTRT_CAN				-5002
//#define MTRT_REJ				-5003
#define MTRT_FAIL				-5004
#define MTRT_KWORD				-5005
#define MTRT_SYSTEMSTOP			-5006	// システム終了
#define MTRT_NONE				-5007
#define MTRT_NORM				-5008
//
#define MTRT_REAL				5001
#define MTRT_POINT				5002
#define MTRT_SHORT				5003
#define MTRT_STR				5004
#define MTRT_LONG				5005
#define MTRT_VOID				5006
#define MTRT_DXF0				5007
#define MTRT_BINARY				5008
//
#define MTRT_KEYCHAR			2	// 文字入力または文字列入力
#define MTRT_LBTNDWN			3	// マウス左ボタンダウン
#define MTRT_MBTNDWN			4	// マウス中ボタンダウン
#define MTRT_RBTNDWN			5	// マウス右ボタンダウン
#define MTRT_LBTNUP				6	// マウス左ボタンアップ
#define MTRT_MBTNUP				7	// マウス中ボタンアップ
#define MTRT_RBTNUP				8	// マウス右ボタンアップ
#define MTRT_LBTNDBL			9	// マウス左ボタンダブルクリック
#define MTRT_MBTNDBL			10	// マウス中ボタンダブルクリック
#define MTRT_RBTNDBL			11	// マウス右ボタンダブルクリック
#define MTRT_MOUSEMOVE			12	// マウス移動
#define MTRT_MOUSEWHEEL			13	// マウスホイール
#define MTRT_MENUCOMMAND		14	// メニューコマンド
#define MTRT_POPUPCOMMAND		15	// ポップアップメニューコマンド
#define MTRT_AUXMENU			16	// Aux (right) button click
#define MTRT_SIZE				17	// ウィンドウサイズ変更
#define MTRT_SIZEMAX			18	// ウィンドウ最大化				(未)		//	(未)
#define MTRT_SIZEICON			19	// ウィンドウアイコン化			(未)		//	(未)
#define MTRT_MOVE				20	// ウィンドウ移動
#define MTRT_ACTIVE				21	// ウィンドウアクティブ
#define MTRT_DELETE				22	// ウィンドウ削除
#define MTRT_TIMER				23	// タイマーイベント
#define MTRT_DROPFILE			24	// ドロップファイルイベント

									




/////////////////////////////////////////////////////////////////////////////////////////

//#define MT_RTNONE        5000
//#define MT_RTANG         5004
//#define MT_RTENAME       5006
//#define MT_RTPICKS       5007
//#define MT_RTORINT       5008
//#define MT_RTLB          5016
//#define MT_RTLE          5017
//#define MT_RTDOTE        5018
//#define MT_RTNIL         5019

//void MtRelRb( MtResBuf* pRb);

//typedef double			Mtreal;
//typedef Mtreal			Mtpoint[3];
//typedef long			Mtname[2];
//typedef Mtreal			Mtmatrix[4][4];
//typedef Mtreal			*Mtpointp;
//typedef long			*Mtnamep;
//typedef struct _Mthdlg	*Mthdlg;
//typedef struct _Mthtile	*Mthtile;

//typedef struct {
////    Mthdlg  dialog;
////    Mthtile tile;
//    MCHAR*     value;
//    void*     client_data;
//    int       reason;
//    long      x, y;
//} Mtcallback_packet;

//#define MT_X             0
//#define MT_Y             1
//#define MT_Z             2
//#define MT_T             3
//
//#define MT_TRUE          1
//#define MT_FALSE         0
//
//#define MT_PAUSE         "\\"
//
//#define MT_RSG_NONULL    0x01
//#define MT_RSG_NOZERO    0x02
//#define MT_RSG_NONEG     0x04
//#define MT_RSG_NOLIM     0x08
//#define MT_RSG_GETZ      0x10
//#define MT_RSG_DASH      0x20
//#define MT_RSG_2D        0x40
//#define MT_RSG_OTHER     0x80
//#define MT_RSG_NOPMENU   0x100
//#define MT_RSG_NOCANCEL  0x200
//#define MT_RSG_NOVIEWCHG 0x400
//#define MT_RSG_NODOCCHG  0x800
//
//#define MT_RQSAVE        14
//#define MT_RQEND         15
//#define MT_RQQUIT        16
//#define MT_RQCFG         22
//#define MT_RQXLOAD       100
//#define MT_RQXUNLD       101
//#define MT_RQSUBR        102
//#define MT_RQHUP         105
//#define MT_RQXHELP       118
//
//#define MT_RSRSLT        1
//#define MT_RSERR         3

//
//#define COND_OP_CODE		-4
//
//#define MT_RTDRAGPT      5500
//
//#define MT_CBR_SELECT       1
//#define MT_CBR_LOST_FOCUS   2
//#define MT_CBR_DRAG         3
//#define MT_CBR_DOUBLE_CLICK 4
//
//#define MT_DLGALLDONE   -1
//#define MT_DLGCANCEL     0
//#define MT_DLGOK         1
//#define MT_DLGSTATUS     2
//
//#define MT_BGLCOLOR      -2
//#define MT_DBGLCOLOR     -15
//#define MT_DFGLCOLOR     -16
//#define MT_LINELCOLOR    -18
//
//#define MT_LIST_CHANGE   1
//#define MT_LIST_APPEND   2
//#define MT_LIST_NEW      3
//
////================================
////	 MT xref actions
////===============================
//#define MT_XREF_FIND		0
//#define MT_XREF_ATTACH     1	 
//#define MT_XREF_OVERLAY	2	 
//#define MT_XREF_BIND		3	 
//#define MT_XREF_INSERT     4	                                                         
//#define MT_XREF_EDIT_PATH	5
//#define MT_XREF_PATH_DNT_RETAIN	6
//#define MT_XREF_RELOAD		7	 
//#define MT_XREF_UNLOAD		8	 
//#define MT_XREF_DETACH		9	
////=======================================================================
////	Mtxref(action,param) 
////	actions:
////	-----------------
////	MT_XREF_FIND		Inquiry to the user for xref search. 
////						On input :
////							param->resval.rint==0 request for attach
////							param->resval.rint!=0 request for overlay
////						On exit: 
////						In param->resval.rstring will be returned path.
////	---------------------------------------------------------------------
////	MT_XREF_ATTACH		In this case "param" contains those parameters 
////	MT_XREF_OVERLAY	(for example rotate angle, original point etc.), 
////						 which will not be requested from the user.
////							The DXF-codes may be next:
////						 10 - insertion point,	41,42,43 - x,y,z scales 
////						 50 - rotation,			1 - path,	2 - name
////	---------------------------------------------------------------------
////	MT_XREF_BIND	
////	MT_XREF_INSERT		In this case "param" contains xref names list
////	MT_XREF_RELOAD					(DXF code 2 or MT code RTSTR)
////	MT_XREF_UNLOAD			
////	MT_XREF_DETACH
////	---------------------------------------------------------------------
////	MT_XREF_EDIT_PATH	      In this case "param" contains  
////							xref name pattern	(DXF code 2) 
////							and new path		(DXF code 1)
////	MT_XREF_PATH_DNT_RETAIN	Similarly, only name of a file
////							is kept without path.
////========================================================================
//
//
////********************************
//// MT callback flags
////********************************
//
//// Command Start/End.
//#define MT_CBCMDBEGIN        0  // arg1="command name"	 cast as (MCHAR* )
//							     // Callback return  RTERROR will cause command to not be called.
//#define MT_CBCMDEND          1	 // arg1="command name"	 cast as (MCHAR* )
//						         // Return code does not matter.
//
//// Mouse movements and Mouse buttons.
//#define MT_CBMOUSEMOVE       2	 // arg1=X pixel  arg2=Y pixel
//#define MT_CBLBUTTONDN       3	 // arg1=X pixel  arg2=Y pixel
//#define MT_CBLBUTTONUP       4	 // arg1=X pixel  arg2=Y pixel
//#define MT_CBLBUTTONDBLCLK   5	 // arg1=X pixel  arg2=Y pixel
//#define MT_CBRBUTTONDN       6	 // arg1=X pixel  arg2=Y pixel
//#define MT_CBRBUTTONUP       7	 // arg1=X pixel  arg2=Y pixel
//							     // Callback return  RTERROR will cause point to be ignored.
//								 // Cast arg1 and arg2 as (int*)
//
//// Transforming a Selection Set.
//#define MT_CBXFORMSS         8	 // arg1=Selection set (ok to modify) cast as (Mtname)
//								 // arg2=Transformation Matrix (ok to modify) cast as (Mtmatrix)
//								 // Callback return RTERROR will stop the transformation.
//
//// Undo/Redo.
//#define MT_CBENTUNDO         9	 // arg1=Entname (ok to modify)	cast as (Mtname), arg2 below defines on undo type
//								 // Callback return RTERROR will stop UNDO for this ent.
//#define MT_CBENTREDO        10	 // arg1=Entname (ok to modify)	 cast as (Mtname), arg2 below defines on undo type
//								 // Callback return RTERROR will stop REDO for this ent.
//
//// Undo/Redo constants - Passed back as arg2 parameter so the user can tell what action is being undone.
//#define MT_ADD_NOTICE		1
//#define MT_MODIFY_NOTICE	2
//#define MT_DELETE_NOTICE	3
//
//// Palette Change.
//#define MT_CBPALETTECHG     11	 // arg1=HPALETTE cast as (HPALETTE)
//
//// Open/Close/New/Save on documents.
//#define MT_CBOPENDOC        12	 // arg1=file name, cast as (MCHAR* )
//#define MT_CBNEWDOC         13	 // (all args NULL)
//#define MT_CBCLOSEDOC       14	 // arg1=file name, cast as (MCHAR* )
//#define MT_CBSAVEDOC        15	 // arg1= new file name, cast as (MCHAR* )
//								 // arg2= old file name, cast as (MCHAR* )
//
//// !!! See new callback MT_CBDOCCHG below for better MDI control
//// Doc/View change (when the user activates a new MDI window).
//#define MT_CBVIEWDOCCHG     16	 // arg1=file name, cast as (MCHAR* )
//								 // arg2=current window HWND, cast as (HWND)
//
//// Changes to documents.
//#define MT_CBENTDEL         17	 // arg1=Entname to be deleted (or undeleted), cast as (Mtname)
//								 // Callback return RTERROR will stop Mtentdel() for this ent.
//#define MT_CBENTMAKE        18	 // arg1=Entname of new entity, cast as (Mtname)
//#define MT_CBENTMOD         19	 // arg1=Entname of modified entity, cast as (Mtname)
//
//// Grip edits.
//#define MT_CBGRIPEDITBEG    20	 // arg1=Entname being edited, cast as (Mtname)
//								 // arg2=Point being edited, cast as (Mtpoint)
//								 // Callback return RTERROR will stop grip edit.
//#define MT_CBGRIPEDITEND    21	 // arg1=Entname of entity that was edited, cast as (Mtname)
//								 // arg2=Point being edited, cast as (Mtpoint)
//
//#define MT_CBVIEWCHANGE     22	 // arg1=type of paint 
//                                 // 1=redraw, 2=shade, 3=hide, 4=view change (zoom, vpoint, etc.) 5=regen
//				                 // 
//// Mouse movements and Mouse buttons returning current UCS cordinates
//// Callback return  RTERROR will cause point to be ignored.
//#define MT_CBMOUSEMOVEUCS       23  // arg1=X coord  arg2=Y coord arg3=Z coord
//#define MT_CBLBUTTONDNUCS       24	 // arg1=X coord  arg2=Y coord arg3=Z coord
//#define MT_CBLBUTTONUPUCS       25	 // arg1=X coord  arg2=Y coord arg3=Z coord
//#define MT_CBLBUTTONDBLCLKUCS   26	 // arg1=X coord  arg2=Y coord arg3=Z coord
//#define MT_CBRBUTTONDNUCS       27	 // arg1=X coord  arg2=Y coord arg3=Z coord
//#define MT_CBRBUTTONUPUCS       28	 // arg1=X coord  arg2=Y coord arg3=Z coord
//								 // Cast arg1 and arg2 and arg3 as (double)
//
//// Callbacks associated with painting and dragging
//#define MT_CBBEGINPAINT		 29  // no arguments
//#define MT_CBENDPAINT			 30  // no arguments
//#define MT_CBENDMOUSEMOVE		 31  // arg1=X coord  arg2=Y coord arg3 = is Dragging flag 
//								 // Cast arg1 and arg2 as (int*) cast arg3 as bool
//
//// Doc change (when the user activates a new MDI window).
//#define MT_CBDOCCHG			32	 // arg1=new file name, cast as (MCHAR* )
//							 		 // arg2=new window, cast as HWND
//									 // arg3=old window, cast as HWND 
//
//// Callbacks associated with cloning
//#define MT_CBBEGINCLONE		33	// arg1=selection set of the entities to be processed by sds-apps
//
//#define MT_CBENDCLONE			34	// arg1=selection set of the entities the cloning was performed from 
//						// arg2=selection set of created objects or entity name of created block
//						// arg3=mode(0 - arg2=selection set, 1 - agr2=block entity name)
//
//
////*****************************
//// end MT callback flags
////*****************************
//
//#define MT_MODE_ENABLE   0
//#define MT_MODE_DISABLE  1
//#define MT_MODE_SETFOCUS 2
//#define MT_MODE_SETSEL   3
//#define MT_MODE_FLIP     4
//
//#define MT_MAX_TILE_STR   40
//#define MT_TILE_STR_LIMIT 255       
//
//MINT	Mtalert(const MCHAR* szAlertMsg);
//MINT	MtPrintf(const MCHAR* szPrintThis, ...);


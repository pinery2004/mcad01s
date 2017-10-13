//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MdlDispList.h
//
//		家モデルのディスプレイリスト作成
//
//
//  K.Matsu           08/06/01    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"

#include "MgLib.h"
#include "MmDefine.h"
#include "MmSystemProperty.h"

#include "MsBitSet.h"
#include "MbList.h"
#include "MhLib.h"

#include "MgGrp.h"
#include "MhRoof.h"
#include "MdLib.h"

#include "MhInp.h"

#include "MmOpt.h"
#include "MmHist.h"

#include "MdDispList.h"
#include "MmValid.h"

#define		MP_PANEL_LINE			1
#define		MP_PANEL_RECTANGULAR	2

namespace MC
{

#ifdef DLL_EXPORT
	#undef DLL_EXPORT
#endif

#ifdef DLL_EXPORT_MDLDISPLIST_DO
	#pragma message( "<<< MdlDispList・dll_EXport >>>")
	#define DLL_EXPORT							__declspec( dllexport)
#else
	#ifdef _DEBUG								// Release Mode で inline関数がundefになる不具合対応
		#ifdef DLL_NO_IMPORT_MN_DO
			#define DLL_EXPORT
		#else
//			#pragma message( "=== MdlDispList・dll_IMport ===")
			#define DLL_EXPORT					__declspec( dllimport)
		#endif
	#else
		#define DLL_EXPORT
	#endif
#endif

class DLL_EXPORT MdlDispList
{
public:	
	////////////////////////////////////////////////////////////////////////////
	//	構造家モデルの３次元ディスプレイリストを作成する	部材、基礎	実体
	//	
	static int DrawIeMdl(
							MREAL		rB,				// 表示倍率
					const	MgPoint3	&PtCtr			// 中心座標
					);

	////////////////////////////////////////////////////////////////////////////
	//	構造家モデルの３次元ディスプレイリストを作成する	部材、基礎	実体
	//	
	static int DrawIeMdl1(
							MREAL		rB,				// 表示倍率
					const	MgPoint3	&PtCtr			// 中心座標
					);

	////////////////////////////////////////////////////////////////////////////
	//	構造家モデルの３次元ディスプレイリストを作成する	壁、矢切　マテリアルなし
	//
	static int DrawIeMdl2(
							MREAL		rB,				// 表示倍率
					const	MgPoint3	&PtCtr			// 中心座標
					);

	////////////////////////////////////////////////////////////////////////////
	//	構造家モデルの３次元ディスプレイリストを作成する	屋根　マテリアルなし
	//
	static int DrawIeMdl3(
							MREAL		rB,				// 表示倍率
					const	MgPoint3	&PtCtr			// 中心座標
					);

	////////////////////////////////////////////////////////////////////////////
	//	【機能】屋根のディスプレイリストを作成する　実体用
	//			
	static void DrawYane1(
							MhRoofInfo	*pRoofEn,		// 横置部材
					const	MgPoint3	&PtCtr,			// 構造家モデルの中心
							MREAL		rB				// ３次元表示倍率
					);

	////////////////////////////////////////////////////////////////////////////
	//	【機能】屋根のディスプレイリストを作成する　半透明
	//			
	static void DrawYane2(
							MhRoofInfo	*pRoofEn,		// 横置部材
					const	MgPoint3	&PtCtr,			// 構造家モデルの中心
							MREAL		rB				// ３次元表示倍率
					);

	////////////////////////////////////////////////////////////////////////////
	//	【機能】矢切のディスプレイリストを作成する　実体用
	//			
	static void DrawYagiri(
							MhRoofInfo	*pRoofEn,		// 横置部材
					const	MgPoint3	&PtCtr,			// 構造家モデルの中心
							MREAL		rB				// ３次元表示倍率
					);
			
	////////////////////////////////////////////////////////////////////////////
	//	【機能】壁のディスプレイリストを作成する 実態、半透明
	//			
	static void DrawKabe(
							MhPlcInfo	*i_pPlcEn,		// 壁
					const	MgPoint3	&PtCtr,			// 構造家モデルの中心
							MREAL		rB				// ３次元表示倍率
					);

	////////////////////////////////////////////////////////////////////////////
	//	ディスプレイリストを作成する
	static MINT OpenDL( 
							MINT		inmDispList		// DisplayList番号
					);

	////////////////////////////////////////////////////////////////////////////
	//	ディスプレイリストの作成を終了する
	static void CloseDL();

	////////////////////////////////////////////////////////////////////////////
	//	構造家モデルの３次元ディスプレイリストを作成する	屋根、半透明
	//
	static MINT MakeDispListMat0(
							MREAL		rB,				// 表示倍率
					const	MgPoint3	&PtCtr			// 中心座標
					);

	////////////////////////////////////////////////////////////////////////////
	//	構造家モデルの３次元ディスプレイリストを作成する	壁、矢切　
	//
	static MINT MakeDispListMat1(
							MREAL		rB,				// 表示倍率
					const	MgPoint3	&PtCtr			// 中心座標
					);

	////////////////////////////////////////////////////////////////////////////
	//	構造家モデルの３次元ディスプレイリストを作成する	壁、矢切、屋根　半透明
	//
	static MINT MakeDispListMat2(
							MREAL		rB,				// 表示倍率
					const	MgPoint3	&PtCtr			// 中心座標
					);

	////////////////////////////////////////////////////////////////////////////
	//	【機能】建具の開口枠のディスプレイリストを作成する
	//			
	static void DrawTategu(
							MhPlcInfo	*i_pPlcEn,		// 壁またはNULL
							MhPlcInfo	*pPlcTEn,		// 建具
					const	MgPoint3	&PtCtr,			// 構造家モデルの中心
							MREAL		rB				// ３次元表示倍率
					);

	////////////////////////////////////////////////////////////////////////////
	//	【機能】たて枠、束のディスプレイリストを作成する
	//			
	static void DrawTatewaku( 
							MhPlcInfo	*i_pPlcEn,
					const	MgPoint3	&PtCtr,
							MREAL		rB
					);

	////////////////////////////////////////////////////////////////////////////
	//	【機能】床、天井、屋根の開口のディスプレイリストを作成する
	//			
	static void DrawKaiko( 
							MhPlcInfo *i_pPlcEn,
					const	MgPoint3	&PtCtr,
							MREAL		rB
					);

	////////////////////////////////////////////////////////////////////////////
	//	【機能】横置部材のディスプレイリストを作成する
	//			
	static void DrawOukaZai( 
							MhPlcInfo	*i_pPlcEn,
					const	MgPoint3	&PtCtr,
							MREAL		rB
					);

	////////////////////////////////////////////////////////////////////////////
	//	【機能】パネルのディスプレイリストを作成する
	//			
	static void DrawPanel( 
							MhPlcInfo *i_pPlcEn,
					const	MgPoint3	&PtCtr,
							MREAL		rB,
							MINT		iMode			// 表示モード 1:パネル上部線分 2:パネル形状直方体 3:線分+パネル形状
					);

	////////////////////////////////////////////////////////////////////////////
	//	【機能】基礎のディスプレイリストを作成する
	//			
	static void DrawKiso(
							MhPlcInfo	*i_pPlcEn,		// 基礎
					const	MgPoint3	&PtCtr,			// 構造家モデルの中心
							MREAL		rB				// ３次元表示倍率
					);

	////////////////////////////////////////////////////////////////////////////
	//	構造家モデルのデータを全て読み込み３次元ディスプレイリスト作成用の
	//  表示倍率と中心座標を求める
	//
	static void MhGetBCtr(
					MREAL		*rB,					// 表示倍率
					MgPoint3	*PtCtr					// 中心座標
					);

	////////////////////////////////////////////////////////////////////////////
	//	【機能】家モデルのMINMAXを取得する
	//
	static void MhGetMinMaxMdl( MgMinMaxR3	*prMmMdl);

	////////////////////////////////////////////////////////////////////////////
	//	【機能】建具の穴の多角形を求める
	//			
	static void TateguHole(
							MhPlcInfo	*i_pPlcEn,		// 壁またはNULL
							MhPlcInfo	*pPlcTEn,		// 建具
							MgPolyg3	*PgR,			// 壁右面開口
							MgPolyg3	*PgL			// 壁左面開口
					);
};

} // namespace MC
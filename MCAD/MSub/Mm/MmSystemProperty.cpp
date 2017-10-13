//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MmSystemProperty.cpp
//
//		システムプロパティ
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"
#include "MmDefine.h"

#define	DLL_EXPORT_SYSTEMPROPERTY_DO
#include "MmSystemProperty.h"

namespace MC
{

//S MmSystemProperty	g_SysProp;
//	システム定数
DWORD	MmSystemProperty::m_Color[MM_N_COLOR];					// 色
MREAL	MmSystemProperty::m_Real[MM_N_REAL];					// 実数
MINT	MmSystemProperty::m_Int[MM_N_INT];						// 整数
MCHAR*	MmSystemProperty::m_Str[MM_N_STR];						// 文字列
MCHAR*	MmSystemProperty::m_Path[MM_N_PATH];					// PATH
MREAL	MmSystemProperty::m_Stnd[MMAX_KAI][MM_N_STNDH];			// 基準高さ、厚さ　

/////////////////////////////////////////////////////////////////////////////
//	システム定数の初期化　(固有部分)
void	MmSystemProperty::Init()
{
	m_Color[MM_COLOR_GRID_LINE]			= RGB( 110, 180, 150);	// グリッド線表示色
	m_Color[MM_COLOR_GRID_TEXT]			= RGB(  50, 100,  60);	// グリッド番号表示色
	m_Color[MM_COLOR_MARUME]			= RGB( 255,   0,   0);	// 丸め線表示色
	m_Color[MM_COLOR_CURSOR]			= RGB( 245, 150, 100);	// クロスヘアーカーソル色
	m_Color[MM_COLOR_DRAGGING]			= RGB(  20, 220, 245);	// ドラッギング色
	m_Color[MM_COLOR_HITBZI]			= RGB( 255, 255, 255);	// ヒット部材色
	m_Color[MM_COLOR_TEMP]				= RGB( 120,  20, 125);	// テンポラリ選択表示色
	m_Color[MM_COLOR_BACKGROUND]		= RGB( 252, 252, 252);	// 背景色
//
	m_Color[MM_COLOR_DOUKAIKABESIN]  	= RGB( 25, 25, 205);	// 同階壁芯青色
	m_Color[MM_COLOR_KAKAIKABESIN]	 	= RGB( 185, 75, 15);	// 下階壁芯橙色
	m_Color[MM_COLOR_JYOUKAIKABESIN] 	= RGB( 205, 15, 15);	// 上階壁芯線色
	m_Color[MM_COLOR_DOUKAITAIKABESIN]  = RGB( 25, 25, 205);	// 同階耐力壁芯青色
	m_Color[MM_COLOR_KAKAITAIKABESIN]	= RGB( 185, 75, 15);	// 下階耐力壁芯橙色
	m_Color[MM_COLOR_JYOUKAITAIKABESIN] = RGB( 205, 15, 15);	// 上階壁耐力芯線色
	m_Color[MM_COLOR_DOUKAISIJIKABESIN] = RGB( 25, 25, 205);	// 同階支持壁芯青色
	m_Color[MM_COLOR_KAKAISIJIKABESIN]	= RGB( 185, 75, 15);	// 下階支持壁芯橙色
	m_Color[MM_COLOR_JYOUKAISIJIKABESIN]= RGB( 205, 15, 15);	// 上階支持壁芯線色
	m_Color[MM_COLOR_KISOSIN]			= RGB( 195, 25, 25);	// 基礎芯線色
	m_Color[MM_COLOR_KABESEN]			= RGB( 185, 205, 145);	// 壁表示時の壁線色
	
	m_Real[MM_REAL_PITCH]			= 910.f;					// グリッド・ピッチ
	m_Real[MM_REAL_GRID_SPC]		= 7.f;						// グリッド番号表示空き(ポイント)
//	m_Real[MM_REAL_GRID_SPC]		= 14.f;						// グリッド番号表示空き(ポイント)
	m_Real[MM_REAL_GRID_HEI]		= 9.f;						// グリッド文字高さ(ポイント)
	m_Real[MM_REAL_GRID_AKI]		= 300.f;					// グリッド枠表示空き

	m_Int[MM_INT_GRID_N]			= 16;						// 初期北側グリッド数
	m_Int[MM_INT_GRID_W]			= -3;						// 初期西側グリッド数
	m_Int[MM_INT_GRID_S]			= -3;						// 初期南側グリッド数
	m_Int[MM_INT_GRID_E]			= 17;						// 初期東側グリッド数
	// フォルダ
	m_Str[MM_STR_DIR_TABLE]			= Mstr( "テーブル");		// テーブル
	m_Str[MM_STR_DIR_PARTS]			= Mstr( "テーブル\\部材");	// テーブル\部品
	m_Str[MM_STR_DIR_ATTR]			= Mstr( "属性");			// 属性
	m_Str[MM_STR_DIR_TEXTURE]		= Mstr( "テクスチャ");		// テクスチャ
	m_Str[MM_STR_DIR_FIG]			= Mstr( "図形");			// 図形 
	// ファイル
	m_Str[MM_STR_FILE_INI]			= Mstr( "MCAD.ini");		// MCAD.ini 
	m_Str[MM_STR_FILE_LOG]			= Mstr( "MCADLOG.txt");		// LOG 
	m_Str[MM_STR_FILE_TRACE]		= Mstr( "MCADTRACE.txt");	// TRACE 

	m_Str[MM_STR_CSV_MEMBER]		= Mstr( "寸法型式.csv");	// 寸法型式.csv 
	m_Str[MM_STR_CSV_PARTS]			= Mstr( "部品.csv");		// 部品.csv 
	m_Str[MM_STR_CSV_ATTR]			= Mstr( "属性.csv");		// 属性.csv 
	m_Str[MM_STR_CSV_DRAFTSZ]		= Mstr( "図面サイズ.csv");	// 図面サイズ.csv 
	m_Str[MM_STR_CSV_KOUSEI]		= Mstr( "構成.csv");		// 構成.csv 
	m_Str[MM_STR_CSV_BUNRUI]		= Mstr( "分類.csv");		// 分類.csv			 
/*
	// 実行モジュールのディレクトリ取得
	static MCHAR s_cRootDir[MAX_PATH];
	GetModuleFileName( NULL, s_cRootDir, sizeof( s_cRootDir));
	for ( int i1=(int)Mstrlen( s_cRootDir)-1; i1>=0; i1--) {
		if ( s_cRootDir[i1] == '\\') {
			s_cRootDir[i1] = '\0';
			break;
		}
	}
*/
#ifdef _WIN64
	//N	m_Path[MM_PATH_ROOT_FLDR]		= s_cRootDir;								// 実行モジュールの(ルート)ディレクトリ
	//N	m_Path[MM_PATH_PARTS_FLDR]		= Mstr( "D:\\MCAD_64\\テーブル\\部材\\");	// 構造部品テーブル
	m_Path[MM_PATH_DIR_SYS_ENV]	= Mstr( "D:\\MCAD_64\\マスタ");					// システム環境ディレクトリ
	m_Path[MM_PATH_DIR_LOG]		= Mstr( "D:\\MCAD_64\\Log");					// ログ ディレクトリ	
#else
	//N	m_Path[MM_PATH_ROOT_FLDR]		= s_cRootDir;								// 実行モジュールの(ルート)ディレクトリ
	//N	m_Path[MM_PATH_PARTS_FLDR]		= Mstr( "D:\\MCAD_32\\テーブル\\部材\\");	// 構造部品テーブル
	m_Path[MM_PATH_DIR_SYS_ENV]	= Mstr( "D:\\MCAD_32\\マスタ");					// システム環境ディレクトリ
	m_Path[MM_PATH_DIR_LOG]		= Mstr( "D:\\MCAD_32\\Log");					// ログ ディレクトリ	
#endif

	m_Stnd[0][MM_STNDH_KISO]		= 400.f;			// Kiso
	m_Stnd[0][MM_STNDH_LOWER]		= 645.f;			// Kiso + 245.f (90 + 140 + 15)
	m_Stnd[0][MM_STNDH_UPPER]		= 2740.f;			// rLower + 2095.f
	m_Stnd[0][MM_STNDH_CEILING]		= 3095.f;			// rLower + 2450.f
//	m_Stnd[0][MM_STNDH_ROOF]		= 3184.f;			// rUpper + 89.f
	m_Stnd[0][MM_STNDH_ROOF]		= 3125.f;			// rUpper + 89.f
	m_Stnd[0][MM_STNDA_NOKI]		= 89.f;				// 89.f | 140.f
//	m_Stnd[1][MM_STNDH_KISO]		= 0.f;
	m_Stnd[1][MM_STNDH_LOWER]		= 3354.f;			// rCeiling + 259.f (235 + 24)
	m_Stnd[1][MM_STNDH_UPPER]		= 5449.f;			// rLower + 2095.f
	m_Stnd[1][MM_STNDH_CEILING]		= 5804.f;			// rLower + 2450.f
//	m_Stnd[1][MM_STNDH_ROOF]		= 5893.f;			// rUpper + 89.f
	m_Stnd[1][MM_STNDA_NOKI]		= 89.f;				// 89.f | 140.f
//	m_Stnd[2][MM_STNDH_KISO]		= 0.f;
	m_Stnd[1][MM_STNDH_ROOF]		= 5834.f;			// rUpper + 89.f
	m_Stnd[2][MM_STNDH_LOWER]		= 6063.f;			// rCeiling + 259.f (235 + 24)
	m_Stnd[2][MM_STNDH_UPPER]		= 8158.f;			// rLower + 2095.f
	m_Stnd[2][MM_STNDH_CEILING]		= 8513.f;			// rLower + 2450.f
//	m_Stnd[2][MM_STNDH_ROOF]		= 8602.f;			// rUpper + 89.f
	m_Stnd[2][MM_STNDH_ROOF]		= 8543.f;			// rUpper + 89.f
	m_Stnd[2][MM_STNDA_NOKI]		= 89.f;				// 89.f | 140.f
}

/////////////////////////////////////////////////////////////////////////////
//	プロパティ値を取得する　(共有部分)

/////////////////////////////////////////////////////////////////////////////
//	システム色を取得する
DWORD	MmSystemProperty::GetColor( MSYSTEMCOLOR iCol)
{
	return m_Color[iCol];
}

/////////////////////////////////////////////////////////////////////////////
//	システム実数定数を取得する
MREAL	MmSystemProperty::GetReal( MSYSTEMREAL iReal)
{
	return m_Real[iReal];
}

/////////////////////////////////////////////////////////////////////////////
//	システム整数定数を取得する
MINT	MmSystemProperty::GetInt( MSYSTEMINT iInt)
{
	return m_Int[iInt];
}

/////////////////////////////////////////////////////////////////////////////
//	システム文字列定数を取得する
MCHAR*	MmSystemProperty::GetStr( MSYSTEMSTR iStr)
{
	return m_Str[iStr];
}

/////////////////////////////////////////////////////////////////////////////
//	PATH定数を取得する
MCHAR*	MmSystemProperty::GetPath( MSYSTEMPATH iPath)
{
	return m_Path[iPath];
}

/////////////////////////////////////////////////////////////////////////////
//	システム環境　ファイルパスを得る
MINT MmSystemProperty::GetEnvPath(				// (  O) ステイタス	0:正常	!=0:エラー
						MPPATHENV	i_iType,	// (I  ) 環境パス種類
												//		 MP_PATH_ROOT:	ルート
												//		 MP_PATH_TABLE:	テーブル
												//		 MP_PATH_PARTSTABLE:	部品テーブル
												//		 MP_PATH_ATTR:	属性
												//		 MP_PATH_TEX:	テクスチャ(TEXTURE)
												//		 MP_PATH_FIG:	図形
												//		 MP_PATH_LOG:	LOG
	MCHAR*		i_cFile,	// (I  ) ファイル名
						MCHAR*		o_cPath		// (  O) ファイルパス名
				)
{
	MINT	ist;

//	Mstrcpy( o_cPath, MmSystemProperty::GetPath( MM_PATH_ROOT_FLDR));
//	Mstrcpy( o_cPath, MmSystemProperty::GetPath( MM_PATH_PARTS_FLDR));
	if (i_iType == MP_PATH_LOG) {
		Mstrcpy(o_cPath, MmSystemProperty::GetPath(MM_PATH_DIR_LOG));		// LOG ディレクトリ
	}
	else {
		Mstrcpy(o_cPath, MmSystemProperty::GetPath(MM_PATH_DIR_SYS_ENV));	// システム環境ディレクトリ (マスタ)

		MINT nSize = (MINT)Mstrlen(o_cPath);
		if ( o_cPath[nSize - 1] != Mchar('\\'))
			Mstrcat(o_cPath, Mstr("\\"));

		switch (i_iType)
		{
		case MP_PATH_ROOT:
			break;
		case MP_PATH_TABLE:
			Mstrcat(o_cPath, MmSystemProperty::GetStr(MM_STR_DIR_TABLE));	break;
		case MP_PATH_PARTSTABLE:
			Mstrcat(o_cPath, MmSystemProperty::GetStr(MM_STR_DIR_PARTS));	break;
		case MP_PATH_ATTR:
			Mstrcat(o_cPath, MmSystemProperty::GetStr(MM_STR_DIR_ATTR));	break;
		case MP_PATH_TEX:
			Mstrcat(o_cPath, MmSystemProperty::GetStr(MM_STR_DIR_TEXTURE));	break;
		case MP_PATH_FIG:
			Mstrcat(o_cPath, MmSystemProperty::GetStr(MM_STR_DIR_FIG));	break;
		default:
			ist = -1;
			goto exit;
		}
	}
	if ( i_iType != MP_PATH_ROOT) {
		Mstrcat( o_cPath, Mstr( "\\"));
	}
	Mstrcat( o_cPath, i_cFile);

	ist = 0;
exit:
	return ist;
}

/////////////////////////////////////////////////////////////////////////////
//	基準高さを設定する
void	MmSystemProperty::SetStnd(
						MINT		iKai,				// (I  ) 階　(1,2,3)
						MSTNDH		iKTCode,			// (I  ) 基準高さコード
						MREAL		rHA					// (I  ) 基準高さ(厚さ)

				)
{
	m_Stnd[iKai - 1][iKTCode] = rHA;
}

/////////////////////////////////////////////////////////////////////////////
//	基準高さを取得する
MREAL	MmSystemProperty::GetStnd(						// (  O) 基準高さ(厚さ)
						MINT		iKai,				// (I  ) 階　(1,2,3)
						MSTNDH		iKTCode				// (I  ) 基準高さコード
				)
{
	return m_Stnd[iKai - 1][iKTCode];
}

} // namespace MC
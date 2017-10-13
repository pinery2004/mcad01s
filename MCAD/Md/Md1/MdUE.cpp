//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MdUE.cpp
//
//		ユーザーEXIT
//
//
//  K.Matsu           01/19/08    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"
#include "MgDefine.h"
#include "MgPoint.h"
#include "MgRect.h"
#include "MgMat.h"

#include "MdModel.h"

namespace MC
{

//===========================================================================
//		ユーザーEXIT　システム起動時
//
//		指定された光源の図形情報を得る
//		
//		システム起動時のユーザーEXIT関数。
//
//		ユーザーEXITとは、システムのベース部分に割込み処理をいれる場合にカスタマーが
//		作成するDLL関数である。
//		DLLファイル名は「cgsUE.dll」固定。
//		上記DLLに本関数Ｉ／Ｆを作成し、システム実行フォルダに置く事で動作する。
//
//		本関数は、システム起動時に一度だけ実行される。
//
//
void	MduSystem_Init()
{
}

//
//===========================================================================
//		ユーザーEXIT　システム終了時
//
//		システム終了時のユーザーEXIT関数。
//
//		ユーザーEXITとは、システムのベース部分に割込み処理をいれる場合にカスタマーが
//		作成するDLL関数である。
//		DLLファイル名は「cgsUE.dll」固定。
//		上記DLLに本関数Ｉ／Ｆを作成し、システム実行フォルダに置く事で動作する。
//
//		本関数は、システム終了時に一度だけ実行される。
//
void	MduSystem_Term()
{
}

//
//===========================================================================
//		ユーザーEXIT　データベースOPEN後
//
//		データベースOPEN後のユーザーEXIT関数。
//
//		ユーザーEXITとは、システムのベース部分に割込み処理をいれる場合にカスタマーが
//		作成するDLL関数である。
//		DLLファイル名は「cgsUE.dll」固定。
//		上記DLLに本関数Ｉ／Ｆを作成し、システム実行フォルダに置く事で動作する。
//
//		本関数は、システムの図面データベースのOPEN後に実行される。
//
//		注）データベースID（vid）はオープンされている図面データベース毎に振られるユニークな
//		IDである。
//		現バージョンでは必ずvid=1となる。
//
MINT	MduDB_Open_After (
						MDID		i_idDB		// データベースID
				)
{
	return 0;
}

//
//===========================================================================
//		ユーザーEXIT　データベースCLOSE後
//
//		データベースOPEN後のユーザーEXIT関数。
//
//		ユーザーEXITとは、システムのベース部分に割込み処理をいれる場合にカスタマーが
//		作成するDLL関数である。
//		DLLファイル名は「cgsUE.dll」固定。
//		上記DLLに本関数Ｉ／Ｆを作成し、システム実行フォルダに置く事で動作する。
//
//		本関数は、システムの図面データベースのCLOSE後に実行される。
//
//		注）データベースID（vid）はオープンされている図面データベース毎に振られるユニークな
//		IDである。
//		現バージョンでは必ずvid=1となる。
//
MINT	MduDB_Close_After(
						MDID		i_idDB		// データベースID
				)
{
	return 0;
}

//
//===========================================================================
//		ユーザーEXIT　ウィンドウOPEN後
//
//		ウィンドウOPEN後のユーザーEXIT関数。
//
//		ユーザーEXITとは、システムのベース部分に割込み処理をいれる場合にカスタマーが
//		作成するDLL関数である。
//		DLLファイル名は「cgsUE.dll」固定。
//		上記DLLに本関数Ｉ／Ｆを作成し、システム実行フォルダに置く事で動作する。
//
//		本関数は、システムのグラフィックウィンドウのOPEN後に実行される。
//
MINT	MduWindow_Open_After(
						MDID		i_idWin		// ウィンドウID
				)
{
	return 0;
}

//
//===========================================================================
//		ユーザーEXIT　ウィンドウCLOSE後
//
//		ウィンドウOPEN後のユーザーEXIT関数。
//
//		ユーザーEXITとは、システムのベース部分に割込み処理をいれる場合にカスタマーが
//		作成するDLL関数である。
//		DLLファイル名は「cgsUE.dll」固定。
//		上記DLLに本関数Ｉ／Ｆを作成し、システム実行フォルダに置く事で動作する。
//
//		本関数は、システムのグラフィックウィンドウのCLOSE後に実行される。
//
MINT	MduWindow_Close_After(
						MDID		i_idWin		// ウィンドウID
				)
{
	return 0;
}

} // namespace MC
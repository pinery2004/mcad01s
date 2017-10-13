//==========================================================================================
//  Copyright ( C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MbBasic.cpp
//
//		基本的な関数の集まり
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"

#define	DLL_EXPORT_BASE_DO
#include "MbBasic.h"

#include <memory.h>
#include <locale.h>

//===========================================================================
//	日本語モードに設定する
//
void 	MC::Base::MbSetJapanese( )
{
//	setlocale( LC_ALL, "ja");			// エラーになっているようだがエラー表示無し

//	setlocale( LC_ALL, ".ACP" );		// 設定にあわせる　( 日本語のfwprintfが可能になる)

	setlocale( LC_ALL, "jpn" );			// 日本語のfwprintfを可能にする
}


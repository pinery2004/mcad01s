//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MsLib.cpp
//
//		
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"
#include "MgLib.h"

#include "MsLib.h"

namespace MC
{

static MINT			z_CmpWidth;									// デフォルトの配列の要素サイズ
static MAryCmpFunc	AryCompare;									// デフォルトの比較関数

////////////////////////////////////////////////////////////////////////////
//【機能】クイックソート

void MsQSort(													// (  O) ｽﾃｰﾀｽ
			void*			base,								// (I  ) 並び替える配列の先頭
			MINT			num,								// (I  ) 配列の要素数
			MINT			width,								// (I  ) 配列の要素サイズ(バイト数)
			MAryCmpFunc*	AryCmpFunc							// (I  ) 比較関数
			)
{
	z_CmpWidth = width;
	if ( AryCmpFunc == NULL)  AryCmpFunc = AryCompare;			// デフォルトの比較関数を設定
	qsort( base, num, width, AryCmpFunc);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//【機能】デフォルトの比較関数
//【返値】 < 0 : array1  < array2
//		  == 0 : array1 == array2
//		   > 0 : array1  > array2 

static MINT AryCompare(											// (  O) ｽﾃｰﾀｽ
			const void*		array1,								// (I  ) ﾃﾞｰﾀ1
			const void*		array2								// (I  ) ﾃﾞｰﾀ2
			)
{
	return memcmp(array1, array2, z_CmpWidth);
}

} // namespace MC
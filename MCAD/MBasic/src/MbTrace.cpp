//==========================================================================================
//  Copyright ( C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MbDebug.cpp
//
//		
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#pragma warning( disable : 4996 )
#include "stdafx.h"
#include "MbBasic.h"

#define	DLL_EXPORT_MBLOG_DO
#include "MbDebug.h"

//MCHAR	MbLog::m_Str[1000] = { MCHAR( "INIT")};
//FILE*	MbLog::m_File;
//MINT	MbLog::m_iCtl;

#define	PRINT_CNT_P_L		10					// １行あたりの印刷数

namespace MC
{

FILE* Trace::m_pfp;
//===========================================================================
//		デバッグ

//------------------------------------------------------------------------
// 関数名　　　　:write
// 概要　　　　　:ログファイルへフォーマット付きでインデント付きで出力する
// 引数　　　　　:なし
// 戻り値　　　　:なし
// 履歴　　　　　:2007/06/13　　　　　　岡本　晃　　　　　新規作成
//------------------------------------------------------------------------
void Trace::Write(char *in_szFormat, ...)
{
	char szBuf[256];
	va_list	stList;
	va_start(stList,in_szFormat);
	vsprintf(szBuf,in_szFormat,stList);
	va_end(stList);

	fprintf(m_pfp,"%5s%s","",szBuf);
}

} // namespace MC

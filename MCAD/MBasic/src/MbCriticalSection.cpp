//==========================================================================================
//  Copyright ( C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MbCriticalSection.cpp
//
//	同期処理	
//	[機能] 一つのスレッドのみ実行し他のスレッドを待たせる
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"
#include "MbCriticalSection.h" /*DNT*/

// クリティカルセクションオブジェクトへのポインタが正常であるかチェックする
//
bool MbCriticalSection::MbCSIsValid( void)
{

//	if ( this == NULL) return false;       // CHECKSTRUCTPTRにNULチェック機能あり

	if ( !CHECKSTRUCTPTR( this))
	{
		return false;
	}
	return true;
}

// クリティカルセクションオブジェクトの所有権を要求する
// ( 所有権を獲得するまで待機する)
//
void MbCriticalSection::MbCSEnter( void)
{
	::EnterCriticalSection( ( LPCRITICAL_SECTION)&CritSect);
}

// 所有権を放棄する
//
void MbCriticalSection::MbCSLeave( void)
{
	::LeaveCriticalSection( ( LPCRITICAL_SECTION)&CritSect);
}


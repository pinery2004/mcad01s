//==========================================================================================
//  Copyright ( C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MbSynchroObj.cpp
//
//	[機能] 同期を行うための基本クラス
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include <windows.h>
#include <assert.h>
#include "MbCriticalSection.h"
#include "MbSynchroObj.h"

//	単一スレッド管理用オブジェクトの所有権を要求し他のスレッドからの要求を待たせる
//
void MbSynchroObj::MbSOLock( void)
{

	m_TryLockLock.MbCSEnter( );
	m_iLockCount++;
	m_TryLockLock.MbCSLeave( );

	m_ObjectLock.MbCSEnter( );
}

//	単一スレッド管理用オブジェクトの所有権を放棄する
//
void MbSynchroObj::MbSOUnlock( void)
{
	m_ObjectLock.MbCSLeave( );
	m_iLockCount--;
	assert( m_iLockCount >= 0);
}

bool MbSynchroObj::MbSOisValid( void)
{

	if ( !m_ObjectLock.MbCSIsValid( ))	// クリティカルセクションオブジェクトへのポインタ
		return false;							// m_ObjectLock が正常であるかチェックする

	if ( !m_TryLockLock.MbCSIsValid( ))	// クリティカルセクションオブジェクトへのポインタ
		return false;                        	// m_TryLockLock が正常であるかチェックする
                                             
	if ( m_iLockCount < 0)				// <0 デストラクタを抜けている
		return false;

	return true;
}


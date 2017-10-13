/* LOCKABLEOBJECT.H
 * Copyright ( C) 1997-1998 Visio Corporation. All rights reserved.
 * 
 * 
 */
////////////////////////////////////////////////////////////////
// 同期を可能にするためにロック可能なオブジェクトの基本クラス 

#pragma once

#include "MbCriticalSection.h"

class MbCriticalSection;

// ********************************
// CLASS MbSynchroObj
//
// Derive classes from this one that you want
// to use synchronously.  This allows external
// synchronization, or the class can be written to 
// internally synchronize using its own Lock/Unlock
// calls.
//
class MbSynchroObj
{
public:
	MbSynchroObj( void)
				{ m_iLockCount = 0;};
	MbSynchroObj::~MbSynchroObj( void)
				{ m_iLockCount = -1;};

public:

	// Is this a properly formed instance?
	//
	bool MbSOIsValid( )
	{
		bool bRetval = false;
		if ( this != NULL)
		{
			bRetval = MbSOisValid( );
		}
		return bRetval;
	}

	// ********************
	// Lock will block until it succeeds!
	void MbSOLock( void);
	void MbSOUnlock( void);
	// This is the only method that needs much explanation
	// If you call this method, it will try to lock the object.
	// If the object is already locked, it will return immediately
	// with a false value.  
	// If the object is not locked, it will lock it and return 
	// immediately with a true value
	//  THIS METHOD NEVER BLOCKS
	//
	// The implementation is a little tricky, but thread-safe.  
	// By the time TryLock returns it may be possible to lock
	// the object, but it will never say it got the lock when it couldn't
	//
	// This method should be used with care, since the caller could ignore
	// the return value and go into non-safe code.
	//
	bool MbSOTryLock( void);

protected:
	bool MbSOisValid( );						// 自身およびm_ObjectLock、m_TryLockLockが正常であるか調べる

private:
	// Keep track of how many objects have this locked or are waiting
	//
	int					m_iLockCount;	// Lockカウント

	// This is the actual lock
	//
	MbCriticalSection	m_ObjectLock;	// クリティカルセクションオブジェクトへのポインタ

	// This is an extra one need to implement TryLock
	//
	MbCriticalSection	m_TryLockLock;	// クリティカルセクションオブジェクトへのポインタ

};

// ************************************
// Use this class to automatically lock
// an object in a scope.  CTOR and DTOR
// handle all the work so you don't have to worry
// about abnormal exit from the scope
//
class LockBracket
{
	// ******************
	// CTOR AND DTOR
	//
public:
	LockBracket( MbSynchroObj *pObject)
	{
		pObject->MbSOLock( );
		m_pObject = pObject;
	}

	~LockBracket( void)
	{
		m_pObject->MbSOUnlock( );
	}

private:
	MbSynchroObj *m_pObject;
};

// ****************************
// Helper macro for internal synchronization.
// For example
//
// class Foo : public MbSynchroObj
//	{
//		....
//		int SomeMethod( void)
//		{
//			SYNCHRONIZEMETHOD( )
//			....
//		}
//

#define SYNCHRONIZEMETHOD( )			LockBracket theLockBracket( this);


//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MtInpEventQueue.cpp
//
//	機能	イベントキュー			
//		PostEvent
//		PeekEvent		
//		WaitEvent
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MCAD.h"
#include "MbBasic.h"
#include "MbCriticalSection.h"
#include "MtInpEvent.h"
#include "MtInpEventFilter.h"
#include "MtInpEventQueue.h"

namespace MC
{

void ProcessMsgWaitForObject( HANDLE hSynchroObject); 

static volatile MINT	z_continue = 1;							// スレッド続行フラグ

MINT MtStopThread( void)
{
	z_continue = 0;
	return 0;
}

bool MtChkStopThread( void)
{
	return ( z_continue == 0);
}

// コンストラクタ
MtInpEventQueue::MtInpEventQueue( void)
{
	m_ourState = EQS_BAD;
	m_nWaitCount = 0;

	m_hSynchroEvent = ::CreateEvent( NULL,	// security attributes
									 FALSE,	// this means an auto-reset object
									 FALSE,	// initially in non-signaled state
									 NULL	// unnamed object (we'll just keep the handle)
									);

	if ( m_hSynchroEvent != NULL)
		m_ourState = EQS_NORMAL;
}

// デストラクタ
MtInpEventQueue::~MtInpEventQueue( void)
{
	Shutdown();
	m_ourState = EQS_BAD;

	::CloseHandle( m_hSynchroEvent);
	m_hSynchroEvent = NULL;
}

////////////////////////////////////////////
//	イベントキューに入力操作のイベントを追加し、追加したことを知らせる
//
//	注意　本関数は待ちのある処理内での使用は禁止、瞬時に終了すること
//
bool MtInpEventQueue::PostEvent( MtInpEvent *pEvent)
{
	SYNCHRONIZEMETHOD();

	if ( m_ourState != EQS_NORMAL)
		return false;

	m_theQueue.Add( pEvent);
	VERIFY( ::SetEvent( m_hSynchroEvent));						// イベントキューに追加したことを知らせる

	return true;
}

////S ////////////////////////////////////////////
////	注意　本関数は待ちのある処理内での使用は禁止、瞬時に終了すること
////
//MtInpEvent * MtInpEventQueue::PeekEvent( MtInpEventFilter *pFilter)
//{
//	MtInpEvent *pRetval = NULL;
//
//	pRetval = getEvent( pFilter);
//
//	return pRetval;
//}

////////////////////////////////////////////
// メッセージキューへの入力イベント待ち
//
// 注意　本関数は　m_hSynchroEvent　を使用しており、デッドロック
//       にならないよう考慮して使用の事
//	
MtInpEvent * MtInpEventQueue::WaitEvent( MtInpEventFilter *pFilter)
{
	ASSERT( MbSOIsValid());

	MtInpEvent *pRetval = getEvent( pFilter);					// 

	while ( ( m_ourState == EQS_NORMAL) && ( pRetval == NULL)) {
		// 待ち開始
		::InterlockedIncrement( &m_nWaitCount);

		while ( ( m_ourState == EQS_NORMAL) && ( pRetval == NULL))	{
//S			processMessagesWhileWaiting();						// 入力イベント待ち
			ProcessMsgWaitForObject( m_hSynchroEvent);			// 入力イベント待ち

			pRetval = getEvent( pFilter);
		}

		// 待ち終了
   		::InterlockedDecrement( &m_nWaitCount);
	}
	return pRetval;
}

////////////////////////////////////////////
// イベントキューのキャンセル処理
//
void MtInpEventQueue::SignalCancel( void)
{
	SYNCHRONIZEMETHOD();

	// 処理待ちのイベントをキャンセルする
	m_ourState = EQS_CANCEL;
	flushQueue();

	ASSERT( m_nWaitCount < 1000);
	ASSERT( m_nWaitCount >= 0);

	// イベント待ちにキャンセルを返す
	int iCount = m_nWaitCount;
	for( int i = 0; i < iCount; i++)
		::SetEvent( m_hSynchroEvent);

	m_ourState = EQS_NORMAL;
}

////////////////////////////////////////////
// イベントキューの処理終了
//
void MtInpEventQueue::Shutdown( void)
{
	SYNCHRONIZEMETHOD();

	SignalCancel();

	m_ourState = EQS_SHUTDOWN;
}

////////////////////////////////////////////
//	入力イベントキューよりフィルタに沿っイベントを取得し、キューから取り除く。
//	フィルタがNULLの場合は最初のイベント。
//	返値 NULL	  : 待ちイベント無し
//		 ポインタ : 待ちイベント
//
MtInpEvent* MtInpEventQueue::getEvent( MtInpEventFilter *pFilter)
{
	SYNCHRONIZEMETHOD();

	MtInpEvent *pRetval = NULL;
	if ( ( m_theQueue.Number() > 0) && ( m_ourState == EQS_NORMAL)) {

		// フィルタに沿った最初のイベントを探す
		int i;
		for( i = 0; i < ( int)m_theQueue.Number(); i++)	{
			MtInpEvent *pCur;
			if ( m_theQueue.Get( i, &pCur) == false) {
				ASSERT( false);
				return NULL;
			}

			if ( ( pFilter == NULL) ||
				 ( pFilter->Match( pCur)))	{
				pRetval = pCur;
				ASSERT( pRetval->IsValid());
				m_theQueue.Remove( i);
				break;
			} 
			else {												// フィルタ対象外のイベントは取り除く AAAAAAAAAAAAAAAAAAAAAAAAA
				m_theQueue.Remove( i);							// フィルタ対象外のイベントは取り除く
				break;											// フィルタ対象外のイベントは取り除く
			}													// フィルタ対象外のイベントは取り除く
		}
	}

	return pRetval;
}

////////////////////////////////////////////
//	イベントキューをクリアする
void MtInpEventQueue::flushQueue( void)
{
	SYNCHRONIZEMETHOD();

	int iNumber;
	while( ( iNumber = m_theQueue.Number()) > 0) {
		MtInpEvent *pItem;
		if ( m_theQueue.Get( iNumber - 1 , &pItem)==false) {
			ASSERT(false);
			return;
		}
		m_theQueue.Remove( iNumber - 1);
		delete pItem;
	}
}

////////////////////////////////////////////
// メッセージ(イベント)を待つ
//
void ProcessMsgWaitForObject( HANDLE hSynchroObject) 
{
	static const DWORD GOTWINDOWSMESSAGE = (WAIT_OBJECT_0 + 1);

	DWORD dwWaitResult;
	do {
		MSG msg;
		// process any and all windows messages
		//
		while( ::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE)) {
			if ( !AfxGetApp()->PreTranslateMessage(&msg)) {
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}

		// hSynchroObject = HANDLE(0x000007a8);
		//
		dwWaitResult = ::MsgWaitForMultipleObjects( 1, 			// Only one object to wait on
											&hSynchroObject,	// this is the one
											FALSE,				// wait for only one (redundant, since there is only one)
											INFINITE,			// wait until the sun dies
											QS_ALLINPUT			// TODO -- is this right????
										);
	}
	while( dwWaitResult == GOTWINDOWSMESSAGE);
}
 
////S void MtInpEventQueue::processMessagesWhileWaiting( void)
//{
//	::ProcessMsgWaitForObject( m_hSynchroEvent);
//}

MtInpEventQueueInterface * CreateMtInpEventQueue( void)
{
	return new MtInpEventQueue;
}

} // namespace MC

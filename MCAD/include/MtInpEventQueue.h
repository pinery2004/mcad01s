#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MtInpEventQueue.h
//
//		同期型の双方向リスト
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "MbSynchroObj.h"
#include "MbArray.h"

// Interface of MtInpEventQueue uses these classes in name
//

namespace MC
{

class MtInpEvent;
class MtInpEventFilter;

class MtInpEventQueueInterface	: public MbSynchroObj
{
	public:

	// キューにイベントを追加
	//	注意　本関数は待ちのある処理内での使用は禁止、瞬時に終了すること
	virtual bool PostEvent( MtInpEvent *pEvent) = 0;

	// キューからイベントを取得する。イベントが無ければ、イベントの発生を待つ。
	// キューがキャンセルされているまたは廃棄されている場合は、NULLを返す。
	virtual MtInpEvent *WaitEvent( MtInpEventFilter *pEventFilter = NULL) = 0;
};

MtInpEventQueueInterface* CreateMtInpEventQueue( void);

class MtInpEventQueue : public MtInpEventQueueInterface
{
private:
	// キューの状態を得る
	enum EventQueueState {
		EQS_BAD = 0,			// 初期化されていない
		EQS_NORMAL = 1,			// 正常
		EQS_CANCEL = 2,			// キャンセルが呼ばれた
		EQS_SHUTDOWN = 3		// シャットダウン
	};

public:
	MtInpEventQueue( void);
	virtual ~MtInpEventQueue( void);

public:
	virtual bool PostEvent( MtInpEvent *pEvent);

	virtual MtInpEvent *WaitEvent( MtInpEventFilter *pEventFilter);
	virtual void	SignalCancel( void);

	// イベントキューの使用を終了する
	virtual void	Shutdown( void);
	virtual void	DestroySelf( void)
	{
		delete this;
   	}
private:
	// イベントキューを空にする
	void	flushQueue( void);

	MtInpEvent *getEvent( MtInpEventFilter *pEventFilter);

private:
	// イベントキューの状態
	EventQueueState		m_ourState;

	// 待ちになっているスレッドの数
	long				m_nWaitCount;

	// イベントキュー
	MbArray< MtInpEvent * >	m_theQueue;

	// イベント処理同期用	( WIN32 Event Synchronization object)
	HANDLE				m_hSynchroEvent;
};

} // namespace MC
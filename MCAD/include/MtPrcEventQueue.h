#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MtPrcEventQueue.h
//
//	機能	イベントキュー管理
//
//  K.Matsu           08/05/01    Created.
//==========================================================================================

#include "MbSynchroObj.h"
#include "MtPrcEvent.h"

namespace MC
{

class MtPrcEventQueue : public MbListE, public MbSynchroObj
	{
	public:
		MtPrcEventQueue( MINT nBk)
			: MbListE( nBk, sizeof( MtPrcEvent))
		{
			SYNCHRONIZEMETHOD();
		}

		~MtPrcEventQueue( void)
		{
			SYNCHRONIZEMETHOD();
			RemoveAll();
		}

	public:
		// These three are used ALOT, so inline
		//
		//	inline bool IsEmpty( void)
		//	inline bool IsNotEmpty( void)
		inline MtPrcEvent *PeekItem( void)
			{
				SYNCHRONIZEMETHOD();
				MPOSITION	Pos;
				return ( MtPrcEvent*)GetHead( &Pos);
			}
		void AddItem( MtPrcEvent *pItem);
		void AddItemToFront( MtPrcEvent *pItem);
		void AddQueue( MtPrcEventQueue *pOther);
		void PopItem( MtPrcEvent* pRb);
		MtPrcEvent *AdvanceNext( void);
		void Flush( void);
		void FreeNext( void);
		void Copy( MtPrcEventQueue *pOther);

		const MCHAR* PeekString( void);
		bool IsCommandPause( void);
	};

} // namespace MC
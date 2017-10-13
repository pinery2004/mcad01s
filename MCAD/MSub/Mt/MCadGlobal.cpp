//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MCadGlobal.cpp
//
//		
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MCad.h"
#include "MbBasic.h"
#include "MtPrcEventQueue.h"
#include "MtPrcEvent.h"
#include "MtEngine.h"
#include "MtInpEventQueue.h"

namespace MC
{

static MtEngineVariables z_EngineVariables;
static bool	z_MCadExiting = false;

void MtSignalMCadExiting( void)
				{ z_MCadExiting = true;}			// MCAD終了設定

bool MtIsMCadExiting( void)
				{ return z_MCadExiting;}			// MCAD終了確認

//MtEngineVariables *	MtGetEngineVariables( void)
//				{ return &z_EngineVariables;}

MtPrcEventQueue * MtGetCommandQueue( void)
				{ return MtIsMCadExiting() ? NULL : z_EngineVariables.GetCommandQueue();}

MtPrcEventQueue * MtGetMenuQueue( void)
				{ return z_EngineVariables.GetMenuQueue();}

MtInpEventQueueInterface * MtGetMtInpEventQueue( void)
				{ return z_EngineVariables.GetMtInpEventQueue();}

// CLASS IMPLEMENTATION -- class MtEngineVariables
//
MtEngineVariables::MtEngineVariables( void)
{
	m_pMenuQueue = new MtPrcEventQueue( 10);
	m_pActiveCommandQueue = new MtPrcEventQueue( 10);
	m_pMtInpEventQueue = CreateMtInpEventQueue();
}

MtEngineVariables::~MtEngineVariables( void)
{
	MBDELETE( m_pMenuQueue);
	MBDELETE( m_pActiveCommandQueue);
	((MtInpEventQueue*) m_pMtInpEventQueue)->~MtInpEventQueue();
	MBDELETE( m_pMtInpEventQueue);
}

} // namespace MC
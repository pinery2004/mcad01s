#pragma once
//===========================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//				
//	機能	処理キューの管理
//
//
//===========================================================================
//

namespace MC
{

//class MCadThreadController;
class MtPrcEventQueue;
class MtInpEventQueueInterface;

class MtEngineVariables
{
	public:
		MtEngineVariables( void);
		~MtEngineVariables( void);

	// ACCESSORS
	//
	public:
		MtPrcEventQueue * GetMenuQueue( void)
				{ return m_pMenuQueue;}

		MtPrcEventQueue * GetCommandQueue( void)
				{ return m_pActiveCommandQueue;}

		MtInpEventQueueInterface * GetMtInpEventQueue( void)
				{ return m_pMtInpEventQueue;}

	private:
		MtPrcEventQueue *m_pMenuQueue;
		MtPrcEventQueue *m_pActiveCommandQueue;
		MtInpEventQueueInterface *m_pMtInpEventQueue;

};

} // namespace MC
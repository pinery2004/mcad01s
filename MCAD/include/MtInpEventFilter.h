#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
// MtInpEventFilter.h
//
//	機能	コマンド処理用
//								イベントフィルター
// 

namespace MC
{

class MtInpEvent;

class MtInpEventFilter
{
	public:
		MtInpEventFilter( int iFilterFlags);
		virtual ~MtInpEventFilter( void);

	public:
		bool Match( MtInpEvent *pMtInpEvent);

	private:
		int m_iFilterFlags;

};

} // namespace MC
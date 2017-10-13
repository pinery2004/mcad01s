//==========================================================================================
//  Copyright ( C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MbCriticalSection.h
//
//		
//
//
//  K.Matsu           04/25/07    Created.
//

#pragma once

class MbCriticalSection
	{
	public:
		// クリティカルセクションオブジェクトを初期化する
		MbCriticalSection::MbCriticalSection( void)
		{
			memset( &CritSect,0,sizeof( CRITICAL_SECTION));
			::InitializeCriticalSection( &CritSect);
		};
		// クリティカルセクションオブジェクトを解放する
		MbCriticalSection::~MbCriticalSection( void)
		{
			::DeleteCriticalSection( &CritSect);
		};

	public:
		bool MbCSIsValid( );			// クリティカルセクションオブジェクトへのポインタが
										// 正常であるかチェックする
		void MbCSEnter( void);			// クリティカルセクションオブジェクトの所有権を要求する
										// ( 所有権を獲得するまで待機する)
		void MbCSLeave( void);			// 所有権を放棄する
	private:
		CRITICAL_SECTION CritSect;		// CRITICAL_SECTION型変数を宣言する
	};


// 単一スレッドのみ実行を許し、複数のスレッドの起動があった場合は他のスレッドは待機する
//
// 使用方法
//	static MbCriticalSection theSection;
//	MbSynchroGuard theGuard( &theSection );

class MbSynchroGuard
	{
	public:
		MbSynchroGuard( MbCriticalSection *pSection)
		{
			m_pSection = pSection;
			m_pSection->MbCSEnter( );
		}

		~MbSynchroGuard( void)
		{
			m_pSection->MbCSLeave( );
			m_pSection = NULL;
		}

	private:
		MbCriticalSection *m_pSection;
	};


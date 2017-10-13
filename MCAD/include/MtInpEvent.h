#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MtInpEvent.h
//
//	機能	入力イベントキュー
//
//  K.Matsu           08/01/04    Created.
//
//#define MS_CMDMENU_STR_TYPE						// 定義: コマンドメニューキューに文字列形式で登録、未定義: 整数形式で登録

namespace MC
{

class MtInpEvent
{
	friend MINT MtSendMessage( MINT msg, MINT nFlags, MgPoint2* pPtMR);
	friend void MtProcessEvent( MtInpEvent *event);

	private:

#ifdef	MS_CMDMENU_STR_TYPE

		MUINT		m_type;
		MUINT		m_nflag;
		MCHAR		m_key;
		MgPoint2	m_pt;
		MCHAR*		m_pcMenuStr;
#else
		MUINT		m_type;
		MUINT		m_nflag;
		MCHAR		m_key;
		MgPoint2	m_pt;
#endif
	public:
		//MtInpEvent( void)
		//				{ memset( this, 0, sizeof( *this));}
		//MtInpEvent( int type, MCHAR key)
		//				{ memset( this, 0, sizeof( *this)); m_key = key;}
#ifdef	MS_CMDMENU_STR_TYPE
		MtInpEvent( int type, int nflag, MgPoint2 pt)
						{ memset( this, 0, sizeof( *this)); m_type = type; m_nflag = nflag; m_pt= pt;}
		MtInpEvent( int type, int nflag, MCHAR* pcMenuString)
						{ memset( this, 0, sizeof( *this)); m_type = type;  m_nflag = nflag;
						  m_pt = MgPoint2( 0.f, 0.f); m_pcMenuStr = pcMenuString;}
#else
		MtInpEvent( int type, int nflag, MgPoint2 pt)
						{ memset( this, 0, sizeof( *this)); m_type = type; m_nflag = nflag; m_pt= pt;}
#endif
		//MtInpEvent( const MtInpEvent &theOther)
		//				{ m_type = theOther.m_type; m_nflag = theOther.m_nflag; m_key = theOther.m_key;
		//				  m_pcMenuStr = theOther.m_pcMenuStr; m_pt = theOther.m_pt;}
		virtual ~MtInpEvent( void)
						{ m_type = 0;}

	public:
		int		GetType( void)
						{ return m_type;}
		MCHAR	GetKey( void)
						{ return m_key;}
		MgPoint2 GetPoint( void)
						{ return m_pt;}
		void	GetPoint( MgPoint2* pPt)
						{ *pPt = m_pt;}
		void	GetPoint( MREAL* pPt)
						{ pPt[0] = m_pt.x; pPt[1] = m_pt.y;}
#ifdef	MS_CMDMENU_STR_TYPE
		const MCHAR* GetMenuString( void)
						{ return m_pcMenuStr;}
#endif
		bool	IsValid( void)
						{ return CHECKSTRUCTPTR( this);}
		void	SetNFlag( MINT	nflag)
						{ m_nflag = nflag & 0377;}
		MINT	GetNFlag( void)
						{ return m_nflag;}
};

} // namespace MC
#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MtPrcEvent.h
//
//	機能	コマンド処理用コマンド
//
//  K.Matsu           08/01/04    Created.
//

#include "MtDefine.h"

namespace MC
{

struct MtBinary {
    MSHORT	clen;
    MCHAR*	buf;
};

union Mtu_val {
   MREAL	rreal;
   MREAL	rpoint[2];
   MSHORT	rint;
   MCHAR*	rstring;
   long		rlname[2];
   long		rlong;
   struct MtBinary	rbinary;
};

struct MtPrcEvent: public MOBJECT{
	MUINT	m_restype;
	MUINT	m_nflag;
	union Mtu_val m_resval;

	void SetPoint( MREAL* rPt)
						{ m_resval.rpoint[0] = rPt[0]; m_resval.rpoint[1] = rPt[1];}

	void SetPoint( MgPoint2* Pt)
						{ m_resval.rpoint[0] = Pt->x; m_resval.rpoint[1] = Pt->y;}

	void GetPoint( MREAL* rPt)
						{ rPt[0] = m_resval.rpoint[0]; rPt[1] = m_resval.rpoint[1];}

	void GetPoint( MgPoint2* Pt)
						{ Pt->x = m_resval.rpoint[0]; Pt->y = m_resval.rpoint[1];}
	void Free( void);
};

MINT MtBuildPrcEvent( class MtPrcEventQueue* pRbQue, ...);

inline void MtPrcEvent::Free( void)
{
	switch( m_restype)
	{
	case MTRT_MENUCOMMAND:
		MBFREE( m_resval.rstring);
		break;
	case MTRT_VOID:
		MBFREE( m_resval.rbinary.buf);
		break;
	case MTRT_BINARY:
		MBFREE( m_resval.rbinary.buf);
		break;
	}
}

} // namespace MC
//==========================================================================================
//  Copyright ( C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MbPlex.cpp
//
//		MbList, MbSList ‚Ì—ÌˆæŠm•Û
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"

#define	DLL_EXPORT_PLEX_DO
#include "MbPlex.h"

#define		BYTE	char

MbPlex* MbPlex::Create( MbPlex*& pHead, MINT nMax, MINT cbElement)
{
	MINT	iSz = sizeof( MbPlex) + nMax * cbElement;
	MbPlex* p = ( MbPlex*) new BYTE[iSz];
			// may throw exception
	p->pNext = pHead;
	pHead = p;  // change head ( adds in reverse order for simplicity)
	return p;
}

void MbPlex::FreeDataChain( )     // free this one and links
{
	MbPlex* p = this;
	while ( p != NULL)
	{
		BYTE* pBytes = ( BYTE*) p;
		MbPlex* pNext = p->pNext;
		MBDELETE( pBytes);
		p = pNext;
	}
}

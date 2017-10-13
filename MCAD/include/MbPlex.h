#pragma once
//==========================================================================================
//  Copyright ( C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MbPlex.h
//
//		
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#ifdef DLL_EXPORT
	#undef DLL_EXPORT
#endif

#ifdef DLL_EXPORT_PLEX_DO
	#define DLL_EXPORT							__declspec( dllexport)
#else
//	#define DLL_EXPORT							__declspec( dllimport)
	#define DLL_EXPORT
#endif

struct DLL_EXPORT MbPlex	   // warning variable length structure
{
	MbPlex* pNext;

	void* data( ) { return this+1; }

	static MbPlex* Create( MbPlex*& head, MINT nMax, MINT cbElement);
			// like 'calloc' but no zero fill
			// may throw memory exceptions

	void FreeDataChain( );       // free this one and links
};

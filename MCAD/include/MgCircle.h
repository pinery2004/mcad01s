#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgCir.h
//
//		‰~(Cir)‚Ì’è‹`A•ÒW
//
//  K.Matsu           09/20/07    Created.
//==========================================================================================
//
#include "MgPoint.h"

#include "MbPlex.h"

#ifdef DLL_EXPORT_CIR
	#undef DLL_EXPORT_CIR
#endif

#ifdef DLL_EXPORT_CIRCLE_DO
	#pragma message( "<<< MgCirEdll_EXport >>>")
	#define DLL_EXPORT_CIR							__declspec( dllexport)
#else
	#ifdef _DEBUG								// Release Mode ‚Å inlineŠÖ”‚ªundef‚É‚È‚é•s‹ï‡‘Î‰
		#ifdef DLL_NO_IMPORT_GEO_DO
			#define DLL_EXPORT_CIR
		#else
//			#pragma message( "=== MgCirEdll_IMport ===")
			#define DLL_EXPORT_CIR						__declspec( dllimport)
		#endif
	#else
		#define DLL_EXPORT_CIR
	#endif
#endif

namespace MC
{

class	MgCir2;
class	MgCir3;
class	MgPlane3;

//
// ---------------------( ‚QŸŒ³ )------------------------------
//
//  ‚QŸŒ³‰~ 	(Cir)
//
class DLL_EXPORT_CIR MgCir2
{
public:
	MgPoint2	pc;							// ’†S“_
	MREAL		r;							// ”¼Œa
	
	MgCir2()			{}

	MgCir2( const MgPoint2& pc1, MREAL r1)
						{ pc = pc1; r = r1;}

	bool operator == ( const MgCir2 &cir1) const								// b = (cir == cir1) ”»’è
						{ return ( pc == cir1.pc && MgTol::MgEQ( r, cir1.r));}

	bool operator != ( const MgCir2 &cir1) const								// b = (cir != cir1) ”»’è
						{ return !((*this) == cir1);}

	MgCir2 Set( const MgPoint2& pc1, MREAL r1)
						{ pc = pc1; r = r1; return *this;}
	MgCir2 Set( const MgCir3 &Cir1);											// ‚RŸŒ³(x,y) ¨ ‚QŸŒ³
	
	void Print(MCHAR* s);
};

//
// ---------------------( ‚RŸŒ³ )------------------------------
//
//	‚RŸŒ³‰~
//
class DLL_EXPORT_CIR MgCir3
{
public:
	MgPoint3	Pc;							// ’†S“_
	MgVect3		V;							// –@ü’PˆÊƒxƒNƒgƒ‹
	MREAL		r;							// ”¼Œa

	MgCir3()			{}

	MgCir3( const MgPoint3& Pc1, const MgVect3 V1, const MREAL r1)
						{ Pc = Pc1; V = V1; r = r1;}

	MgCir3 Set( const MgPoint3& Pc1, const MgVect3 V1, MREAL r1)
						{ Pc = Pc1; V = V1; r = r1; return *this;}

	MgCir3 Set( const MgCir2 &cir1, MREAL z = 0.)								// ‚QŸŒ³¨‚RŸŒ³	(ZÈ—ª0.)
						{ Pc.Set( cir1.pc, z);
						  V = MgPoint3( 0., 0., 1.);
						  r = cir1.r; return *this;}

	MgCir3 Set( const MgCir2 &cir1, const MgPlane3& Pln)						// ‚QŸŒ³¨‚RŸŒ³	(•½–Êã)
						{ Pc.Set( cir1.pc, Pln);
						  V = Pln.v; r = cir1.r; return *this;}

	void Print(MCHAR* s);
};

// ---------------------( ‚QŸŒ³ )------------------------------
//
//	‚QŸŒ³‰~
//
inline MgCir2 MgCir2C( const MgCir3 &Cir1)										// ‚RŸŒ³(x,y)¨‚QŸŒ³
						{ MgCir2 ciro; ciro.pc.Set( Cir1.Pc);
						  ciro.r = Cir1.r; return ciro;} 

inline MgCir2 MgCir2C( const MgPoint3& Pc, const MREAL r1)						// ‚RŸŒ³(x,y)¨‚QŸŒ³
						{ MgCir2 ciro; ciro.pc.Set( Pc);
						  ciro.r = r1; return ciro;} 

inline MgCir2 MgCir2::Set( const MgCir3 &Cir1)									// ‚RŸŒ³(x,y)¨‚QŸŒ³
						{ MgCir2 ciro;
						  ciro.pc.x = Cir1.Pc.x; ciro.pc.y = Cir1.Pc.y;
						  ciro.r = Cir1.r; return ciro;} 

MINT DLL_EXPORT_CIR MgCir2Div ( const MgCir2 &cir1, MINT n, const MgPoint2& pts,	// ‰~‚ğn“™•ª‚·‚é
							MgGPoint2 *gpto);


// ---------------------( ‚RŸŒ³ )------------------------------
//
//	‚RŸŒ³‰~
//
inline MgCir3 MgCir3C( const MgCir2 &cir1, MREAL z = 0.)						// ‚QŸŒ³¨‚RŸŒ³	(ZÈ—ª0.)
						{ MgCir3 Ciro; Ciro.Pc.Set( cir1.pc, z);
						  Ciro.V = MgPoint3( 0., 0., 1.);
						  Ciro.r = cir1.r; return Ciro;}

inline MgCir3 MgCir3C( const MgCir2 &cir1, const MgPlane3& Pln)					// ‚QŸŒ³¨‚RŸŒ³	(•½–Êã)
						{ MgCir3 Ciro; Ciro.Pc.Set( cir1.pc, Pln);
						  Ciro.V = Pln.v;
						  Ciro.r = cir1.r; return Ciro;}

inline MgCir3 MgCir3C( const MgPoint2& pc, MREAL r, MREAL z = 0.)				// ‚QŸŒ³¨‚RŸŒ³	(ZÈ—ª0.)
						{ MgCir3 Ciro; Ciro.Pc.Set( pc, z);
						  Ciro.V = MgPoint3( 0., 0., 1.);
						  Ciro.r = r; return Ciro;}

inline MgCir3 MgCir3C( const MgPoint2& pc, const MREAL r, const MgPlane3& Pln)	// ‚QŸŒ³¨‚RŸŒ³	(•½–Êã)
						{ MgCir3 Ciro; Ciro.Pc.Set( pc, Pln);
						  Ciro.V = Pln.v;
						  Ciro.r = r; return Ciro;}

MINT DLL_EXPORT_CIR MgCir3Div ( const MgCir3 &Cir1, MINT n, const MgPoint3& Pts,	// ‰~‚ğn“™•ª‚·‚é
							MgGPoint3 *gPto);

inline MINT SZMgCir2( MINT i_sz = 1)	{ return  ( i_sz * (MINT)sizeof( MgCir2));}
inline MINT SZMgCir3( MINT i_sz = 1)	{ return  ( i_sz * (MINT)sizeof( MgCir3));}
inline MINT WSZMgCir2( MINT i_sz = 1)	{ return  ( i_sz * (MINT)sizeof( MgCir2) / SZMINT());}
inline MINT WSZMgCir3( MINT i_sz = 1)	{ return  ( i_sz * (MINT)sizeof( MgCir3) / SZMINT());}

} // namespace MC
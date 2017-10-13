#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgArc.h
//
//		â~å (Arc)ÇÃíËã`ÅAï“èW
//
//  K.Matsu           09/20/07    Created.
//==========================================================================================
//
#include "MgPoint.h"

#include "MbPlex.h"

#ifdef DLL_EXPORT_ARC
	#undef DLL_EXPORT_ARC
#endif

#ifdef DLL_EXPORT_ARC_DO
	#pragma message( "<<< MgArcÅEdll_EXport >>>")
	#define DLL_EXPORT_ARC							 __declspec( dllexport)
#else
	#ifdef _DEBUG								// Release Mode Ç≈ inlineä÷êîÇ™undefÇ…Ç»ÇÈïsãÔçáëŒâû
		#ifdef DLL_NO_IMPORT_GEO_DO
			#define DLL_EXPORT_ARC
		#else
//			#pragma message( "=== MgArcÅEdll_IMport ===")
			#define DLL_EXPORT_ARC						__declspec( dllimport)
		#endif
	#else
		#define DLL_EXPORT_ARC
	#endif
#endif

namespace MC
{

class	MgArc2;
class	MgArc3;
class	MgPlane3;

// ---------------------( ÇQéüå≥ )------------------------------
//
//  ÇQéüå≥â~å  	(Arc)
//
class DLL_EXPORT_ARC MgArc2
{
public:
	MgPoint2	pt[2];						// énì_ÅAèIì_íl	( x, y )
	MgPoint2	pc;							// íÜêSì_
	MREAL		r;							// îºåa
	
	MgArc2()			{}

	MgArc2( const MgPoint2 pt1[2], const MgPoint2& pc1, MREAL r1)
						{ pt[0] = pt1[0]; pt[1] = pt1[1]; pc = pc1; r = r1;}

	MgArc2( const MgPoint2 pt1, const MgPoint2 pt2, const MgPoint2& pc1, MREAL r1)
						{ pt[0] = pt1; pt[1] = pt2; pc = pc1; r = r1;}

	bool operator == ( const MgArc2 &arc1) const								// b = (arc == arc1) îªíË
						{ return ( pc == arc1.pc && 
						  ( ( MgTol::MgEQ( r, arc1.r) && (pt[0] == arc1.pt[0]) && (pt[1] == arc1.pt[1])) ||
						    ( MgTol::MgEQ( r, - arc1.r) && (pt[0] == arc1.pt[1]) && (pt[1] == arc1.pt[0]))));}

	bool operator != ( const MgArc2 &arc1) const								// b = (arc != arc1) îªíË
						{ return !((*this) == arc1);}

	MgArc2 Set( const MgPoint2 pt1[2], const MgPoint2& pc1, MREAL r1)
						{ pt[0] = pt1[0]; pt[1] = pt1[1]; pc = pc1; r = r1; return *this;}
							
	MgArc2 Set( const MgPoint2 pt1, const MgPoint2 pt2, const MgPoint2& pc1, MREAL r1)
						{ pt[0] = pt1; pt[1] = pt2; pc = pc1; r = r1; return *this;}
							
	MgArc2 Set( const MgArc3 &Arc1);											// ÇRéüå≥(x,y) Å® ÇQéüå≥
	
	void Print(const MCHAR* s) const;
};

//
// ---------------------( ÇRéüå≥ )------------------------------
//
//	ÇRéüå≥â~å 
//
class DLL_EXPORT_ARC MgArc3
{
public:
	MgPoint3	Pt[2];						// énì_ÅAèIì_íl	( x, y, z )
	MgPoint3	Pc;							// íÜêSì_ç¿ïWíl	( x, y, z )
	MgVect3		V;							// ñ@ê¸íPà ÉxÉNÉgÉã	( x, y, z )
	MREAL		r;							// îºåa

	MgArc3()			{}

	MgArc3( const MgPoint3 Pt1[2], const MgPoint3& Pc1,
			const MgVect3& V1, const MREAL r1)
						{ Pt[0] = Pt1[0]; Pt[1] = Pt1[1]; Pc = Pc1; V = V1; r = r1;}

	MgArc3( const MgPoint3& Pt1, const MgPoint3& Pt2, const MgPoint3& Pc1,
			const MgVect3& V1, const MREAL r1)
						{ Pt[0] = Pt1; Pt[1] = Pt2; Pc = Pc1; V = V1; r = r1;}

	bool operator == ( const MgArc3 &Arc1) const								// b = (arc == arc1) îªíË
							{ MINT iFR; MINT iFV;
								if ( Pc != Arc1.Pc ) return false;
								if ( MgTol::MgEQ( r, Arc1.r)) iFR = 1;  
								else if ( MgTol::MgEQ( r, -Arc1.r)) iFR = -1;
								else return false;
								if ( V == Arc1.V ) iFV = 1;
								else if ( V == -Arc1.V ) iFV = -1;
								else return false;
								if ( iFR * iFV > 0 ) return ( Pt[0] == Arc1.Pt[0] && Pt[1] == Arc1.Pt[1]);
						  		else return ( Pt[0] == Arc1.Pt[1] && Pt[1] == Arc1.Pt[0]);}

	bool operator != ( const MgArc3 &Arc1) const								// b = (arc != arc1) îªíË
						{ return !((*this) == Arc1);}

	MgArc3 Set( const MgPoint3 Pt1[2], const MgPoint3& Pc1, const MgVect3 V1, MREAL r1)
						{ Pt[0] = Pt1[0]; Pt[1] = Pt1[1]; Pc = Pc1;
						  V = V1; r = r1; return *this;}

	MgArc3 Set( const MgPoint3 Pt1, const MgPoint3 Pt2, const MgPoint3& Pc1,
				const MgVect3 V1, const MREAL r1)
						{ Pt[0] = Pt1; Pt[1] = Pt2; Pc = Pc1;
						  V = V1; r = r1; return *this;}

	MgArc3 Set( const MgArc2 &arc1, MREAL z = 0.)								// ÇQéüå≥Å®ÇRéüå≥	(Zè»ó™0.)
						{ Pt[0].Set( arc1.pt[0], z); Pt[1].Set( arc1.pt[1], z);
						  Pc.Set( arc1.pc, z); V = MgPoint3( 0., 0., 1.);
						  r = arc1.r; return *this;}

	MgArc3 Set( const MgArc2 &arc1, const MgPlane3& Pln)						// ÇQéüå≥Å®ÇRéüå≥	(ïΩñ è„)
						{ Pt[0].Set( arc1.pt[0], Pln); Pt[1].Set( arc1.pt[1], Pln);
						  Pc.Set( arc1.pc, Pln);
						  V = Pln.v; r = arc1.r; return *this;}

	void Print(const MCHAR* s) const;
};

// ---------------------( ÇQéüå≥ )------------------------------
//
//	ÇQéüå≥â~å 
//
inline MgArc2 MgArc2C( const MgArc3 &Arc1)										// ÇRéüå≥(x,y)Å®ÇQéüå≥
						{ MgArc2 arco; arco.pt[0].Set( Arc1.Pt[0]);
						  arco.pt[1].Set( Arc1.Pt[1]); arco.pc.Set( Arc1.Pc);
						  arco.r = ( ( Arc1.V.z > 0) ? Arc1.r: -Arc1.r);
						  return arco;} 

inline MgArc2 MgArc2C( const MgPoint3 Pt[2], const MgPoint3& Pc,
					   const MREAL r1)											// ÇRéüå≥(x,y)Å®ÇQéüå≥
						{ MgArc2 arco; arco.pt[0].Set( Pt[0]);
						  arco.pt[1].Set( Pt[1]); arco.pc.Set( Pc);
						  arco.r = r1; return arco;} 

inline MgArc2 MgArc2C( const MgPoint3 Pt1, const MgPoint3 Pt2,
					   const MgPoint3& Pc, const MREAL r1)						// ÇRéüå≥(x,y)Å®ÇQéüå≥
						{ MgArc2 arco; arco.pt[0].Set( Pt1);
						  arco.pt[1].Set( Pt2); arco.pc.Set( Pc);
						  arco.r = r1; return arco;} 

inline MgArc2 MgArc2::Set( const MgArc3 &Arc1)									// ÇRéüå≥(x,y)Å®ÇQéüå≥
						{ MgArc2 arco; arco.pt[0].Set( Arc1.Pt[0]);
						  arco.pt[1].Set( Arc1.Pt[1]);
						  arco.pc.x = Arc1.Pc.x; arco.pc.y = Arc1.Pc.y;
						  arco.r = Arc1.r; return arco;} 

// ---------------------( ÇRéüå≥ )------------------------------
//
//	ÇRéüå≥â~
//
inline MgArc3 MgArc3C( const MgArc2 &arc1, MREAL z = 0.)						// ÇQéüå≥Å®ÇRéüå≥	(Zè»ó™0.)
						{ MgArc3 Arco; Arco.Pc.Set( arc1.pc, z);
						  Arco.V = MgPoint3( 0., 0., 1.);
						  Arco.r = arc1.r; return Arco;}

inline MgArc3 MgArc3C( const MgArc2 &arc1, const MgPlane3& Pln)					// ÇQéüå≥Å®ÇRéüå≥	(ïΩñ è„)
						{ MgArc3 Arco; Arco.Pc.Set( arc1.pc, Pln);
						  Arco.V = Pln.v;
						  Arco.r = arc1.r; return Arco;}

inline MgArc3 MgArc3C( const MgPoint2 pt[2], const MgPoint2& pc,
					   MREAL r, MREAL z = 0.)									// ÇQéüå≥Å®ÇRéüå≥	(Zè»ó™0.)
						{ MgArc3 Arco; Arco.Pt[0].Set( pt[0], z);
						  Arco.Pt[1].Set( pt[1], z); Arco.Pc.Set( pc, z);
						  Arco.V = MgPoint3( 0., 0., 1.);
						  Arco.r = r; return Arco;}

inline MgArc3 MgArc3C( const MgPoint2 pt1, const MgPoint2 pt2,
					   const MgPoint2& pc, MREAL r, MREAL z = 0.)				// ÇQéüå≥Å®ÇRéüå≥	(Zè»ó™0.)
						{ MgArc3 Arco; Arco.Pt[0].Set( pt1, z);
						  Arco.Pt[1].Set( pt2, z); Arco.Pc.Set( pc, z);
						  Arco.V = MgPoint3( 0., 0., 1.);
						  Arco.r = r; return Arco;}

inline MgArc3 MgArc3C( const MgPoint2 pt[2], const MgPoint2& pc,
					   const MREAL r, const MgPlane3& Pln)						// ÇQéüå≥Å®ÇRéüå≥	(ïΩñ è„)
						{ MgArc3 Arco; Arco.Pt[0].Set( pt[0], Pln);
						  Arco.Pt[1].Set( pt[1], Pln); Arco.Pc.Set( pc, Pln);
						  Arco.V = Pln.v; Arco.r = r; return Arco;}

inline MgArc3 MgArc3C( const MgPoint2 pt1, const MgPoint2 pt2,
					   const MgPoint2& pc,
					   const MREAL r, const MgPlane3& Pln)						// ÇQéüå≥Å®ÇRéüå≥	(ïΩñ è„)
						{ MgArc3 Arco; Arco.Pt[0].Set( pt1, Pln);
						  Arco.Pt[1].Set( pt2, Pln); Arco.Pc.Set( pc, Pln);
						  Arco.V = Pln.v; Arco.r = r; return Arco;}

// ÉTÉCÉY
inline MINT SZMgArc2( MINT i_sz = 1)	{ return  ( i_sz * (MINT)sizeof( MgArc2));}
inline MINT SZMgArc3( MINT i_sz = 1)	{ return  ( i_sz * (MINT)sizeof( MgArc3));}
inline MINT WSZMgArc2( MINT i_sz = 1)	{ return  ( i_sz * (MINT)sizeof( MgArc2) / SZMINT());}
inline MINT WSZMgArc3( MINT i_sz = 1)	{ return  ( i_sz * (MINT)sizeof( MgArc3) / SZMINT());}

} // namespace MC
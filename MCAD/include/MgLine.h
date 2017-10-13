#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgLine.h
//
//		ü(line)‚Æü•ª(UnboundedLine)‚Ì’è‹`A•ÒW
//
//		”¼’¼ü(lay)‚Í–¢‘Î‰
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
//
#include "MgPoint.h"

#include "MbPlex.h"

#ifdef DLL_EXPORT_LINE
	#undef DLL_EXPORT_LINE
#endif

#ifdef DLL_EXPORT_LINE_DO
	#pragma message( "<<< MgLineEdll_EXport >>>")
	#define DLL_EXPORT_LINE							__declspec( dllexport)
#else
	#ifdef _DEBUG								// Release Mode ‚Å inlineŠÖ”‚ªundef‚É‚È‚é•s‹ï‡‘Î‰
		#ifdef DLL_NO_IMPORT_GEO_DO
			#define DLL_EXPORT_LINE
		#else
//			#pragma message( "=== MgLineEdll_IMport ===")
			#define DLL_EXPORT_LINE						__declspec( dllimport)
		#endif
	#else
		#define DLL_EXPORT_LINE
	#endif
#endif

namespace MC
{

class	MgULine2;
class	MgULine3;
class	MgHLine2;
class	MgHLine3;
class	MgLine2;
class	MgLine3;
class	MgPlane3;

//
//==========================================================================================
//
// ---------------------( ‚QŸŒ³ )------------------------------
//
//	‚QŸŒ³’¼ü	(UnboundedLine)
//
class DLL_EXPORT_LINE MgULine2
{
public:
	MgPoint2	p;																// ’Ê‰ß“_
	MgVect2		v;																// ’¼ü•ûŒü

	MgULine2()			{}

	MgULine2( const MgPoint2& p1, const MgVect2& v1)
						{ p = p1; v = v1;}										
	bool operator == ( const MgULine2& ULn1) const;								// b = (uln == ULn1) ”»’è

	bool operator != ( const MgULine2& ULn1) const								// b = (uln != ULn1) ”»’è
						{ return !((*this) == ULn1);}
	MgULine2 Set( const MgPoint2& p1, const MgVect2& v1)
						{ p = p1; v = v1; return *this;}										
	MgULine2 Set( const MgULine3& ULn1);										// ‚RŸŒ³’¼ü ¨ ‚QŸŒ³’¼ü

	void SetRev()																// ’¼ü”½“]
						{ v = - v;}
	void Print(MCHAR* s);
};
//
// ---------------------( ‚QŸŒ³ )------------------------------
//
//	‚QŸŒ³”¼’¼ü	(HalfUnboundedLine)
//
class DLL_EXPORT_LINE MgHLine2
{
public:
	MgPoint2	p;																// n“_
	MgVect2		v;																// ”¼’¼ü•ûŒü

	MgHLine2()			{}

	MgHLine2( const MgPoint2& p1, const MgVect2& v1)
						{ p = p1; v = v1;}										
	bool operator == ( const MgHLine2 &ULn1) const;								// b = (uln == ULn1) ”»’è

	bool operator != ( const MgHLine2 &ULn1) const								// b = (uln != ULn1) ”»’è
						{ return !((*this) == ULn1);}
	MgHLine2 Set( const MgPoint2& p1, const MgVect2& v1)
						{ p = p1; v = v1; return *this;}										
	MgHLine2 Set( const MgHLine3 &ULn1);										// ‚RŸŒ³’¼ü ¨ ‚QŸŒ³’¼ü

	void SetRev()																// ’¼ü”½“]
						{ v = - v;}
	void Print(MCHAR* s);
};
//
//  ‚QŸŒ³ü•ª 	(Line)
//
class DLL_EXPORT_LINE MgLine2
{
public:
	MgPoint2	p[2];															// n“_AI“_

	MgLine2()			{}

	MgLine2( const MgPoint2& ps, const MgPoint2& pe)
						{ p[0] = ps; p[1] = pe;}
	MgLine2( const MgPoint2 *pi)
						{ p[0] = pi[0]; p[1] = pi[1];}

	MgLine2 operator + ( const MgVect2& v) const;								// Ln1 = Ln2 + V3

	MgLine2 operator - ( const MgVect2& v) const;								// Ln1 = Ln2 + V3

	MgLine2 operator += ( const MgVect2& v);									// Ln1 += P2

	MgLine2 operator -= ( const MgVect2& v);									// Ln1 += P2

	bool operator == ( const MgLine2& Ln1) const;								// b = (ln == Ln1) ”»’è

	bool operator != ( const MgLine2& Ln1) const								// b = (ln != Ln1) ”»’è
						{ return !((*this) == Ln1);}
	MgLine2 Set( const MREAL &psx, const MREAL &psy,
				 const MREAL &pex, const MREAL &pey)
						{ p[0].x = psx; p[0].y = psy; 
						  p[1].x = pex; p[1].y = pey; return *this;}
	MgLine2 Set( const MgPoint2& ps, const MgPoint2& pe)
						{ p[0] = ps; p[1] = pe; return *this;}
	MgLine2 Set( const MgPoint2 *pi)
						{ p[0] = pi[0]; p[1] = pi[1]; return *this;}										
	MgLine2 Set( const MgLine3& Ln1);											// ‚RŸŒ³(x,y) ¨ ‚QŸŒ³

	MgLine2 SetRev()															// ü•ª ”½“]
						{ MSWAP( p[0], p[1], MgPoint2); return *this;}
	MgVect2	Vu()
						{ MgVect2 v1 = p[1] - p[0]; return v1.Unitize();}
	void Print(MCHAR* s);
};
//
// ---------------------( ‚RŸŒ³ )------------------------------
//
//	‚RŸŒ³’¼ü
//
class DLL_EXPORT_LINE MgULine3
{
public:
	MgPoint3	p;																// ’Ê‰ß“_
	MgVect3		v;																// ’¼ü•ûŒü

	MgULine3()			{}
	
	MgULine3( const MgPoint3& p1, const MgVect3& v1)
						{ p = p1; v = v1;}
	bool operator == ( const MgULine3& ULn1) const;								// b = (uln == ULn1) ”»’è

	bool operator != ( const MgULine3& ULn1) const								// b = (uln != ULn1) ”»’è
						{ return !((*this) == ULn1);}
	MgULine3 Set( const MgPoint3& p1, const MgVect3& v1)
						{ p = p1; v = v1; return *this;}
	MgULine3 Set( const MgULine2& uln1, const MREAL z1 = 0.f, MREAL i_Tol = g_gTol.D)	// ‚QŸŒ³¨‚RŸŒ³@(ZÈ—ª0.)
						{ p.x = uln1.p.x; p.y = uln1.p.y; p.z = z1;
						  v = MgVect3C( uln1.v).Unitize(i_Tol); return *this;} 
	MgULine3 Set( const MgULine2& ULn1, const MgPlane3& Pln, MREAL i_Tol = g_gTol.D);	// ‚QŸŒ³¨‚RŸŒ³

	MgULine3 SetRev()	{ v = - v; return *this;}								// ’¼ü”½“]
	void Print(MCHAR* s);
};

//
// ---------------------( ‚RŸŒ³ )------------------------------
//
//	‚RŸŒ³”¼’¼ü
//
class DLL_EXPORT_LINE MgHLine3
{
public:
	MgPoint3	p;																// n“_
	MgVect3		v;																// ”¼’¼ü•ûŒü

	MgHLine3()			{}

	MgHLine3( const MgPoint3& p1, const MgVect3& v1)
						{ p = p1; v = v1;}

	bool operator == ( const MgHLine3 &ULn1) const;								// b = (uln == ULn1) ”»’è

	bool operator != ( const MgHLine3 &ULn1) const								// b = (uln != ULn1) ”»’è
						{ return !((*this) == ULn1);}
	MgHLine3 Set( const MgPoint3& p1, const MgVect3& v1)
						{ p = p1; v = v1; return *this;}
	MgHLine3 Set( const MgHLine2 &uln1, const MREAL z1 = 0.f)					// ‚QŸŒ³¨‚RŸŒ³@(ZÈ—ª0.)
						{ p.x = uln1.p.x; p.y = uln1.p.y; p.z = z1;
						  v = MgVect3C( uln1.v).Unitize(); return *this;} 
	MgHLine3 Set( const MgHLine2 &ULn1, const MgPlane3& Pln);					// ‚QŸŒ³¨‚RŸŒ³

	MgHLine3 SetRev()	{ v = - v; return *this;}								// ”¼’¼ü”½“]
	void Print(MCHAR* s);
};

//
//	‚RŸŒ³ü•ª
//
class DLL_EXPORT_LINE MgLine3
{
public:
	MgPoint3	p[2];															// n“_AI“_

	MgLine3()			{}

	MgLine3( const MgPoint3& ps, const MgPoint3& pe)
						{ p[0] = ps; p[1] = pe;}
	MgLine3( const MgPoint3 *pi)
						{ p[0] = pi[0]; p[1] = pi[1];}										
	MgLine3( const MREAL &psx, const MREAL &psy, const MREAL &psz,
			 const MREAL &pex, const MREAL &pey, const MREAL &pez)
						{ p[0].x = psx; p[0].y = psy; p[0].z = psz; 
						  p[1].x = pex; p[1].y = pey; p[1].z = pez;}
	MgLine3 operator + ( const MgVect3& v) const;								// Ln1 = Ln2 + p3

	MgLine3 operator - ( const MgVect3& v) const;								// Ln1 = Ln2 + p3

	MgLine3 operator += ( const MgVect3& v);									// Ln1 += P2

	MgLine3 operator -= ( const MgVect3& v);									// Ln1 += P2

	bool operator == ( const MgLine3& Ln1) const;								// b = (ln == Ln1) ”»’è

	bool operator != ( const MgLine3& Ln1) const								// b = (ln != Ln1) ”»’è
						{ return !((*this) == Ln1);}
	MgLine3 Set( const MgPoint3& ps, const MgPoint3& pe)						// n“_AI“_
						{ p[0] = ps; p[1] = pe; return *this;}
	MgLine3 Set( const MgPoint3 *pi)											// ‚Q“_
						{ p[0] = pi[0]; p[1] = pi[1]; return *this;}										
	MgLine3 Set( const MREAL &psx, const MREAL &psy, const MREAL &psz,		// 2*3À”
				 const MREAL &pex, const MREAL &pey, const MREAL &pez)
						{ p[0].x = psx; p[0].y = psy; p[0].z = psz; 
						  p[1].x = pex; p[1].y = pey; p[1].z = pez;
						  return *this;}
	MgLine3 Set( const MgLine2& ln1, MREAL z = 0.)								// ‚QŸŒ³¨‚RŸŒ³@(ZÈ—ª0.)
						{ p[0].x = ln1.p[0].x; p[0].y = ln1.p[0].y; p[0].z = z;
						  p[1].x = ln1.p[1].x; p[1].y = ln1.p[1].y; p[1].z = z;
						  return *this;}
	MgLine3 Set( const MgLine2& ln1, const MgPlane3& Pln);						// ‚QŸŒ³¨‚RŸŒ³

	MgLine3 SetRev()
						{ MSWAP( p[0], p[1], MgPoint3); return *this;}			// ü•ª ”½“]
	MgVect3	Vu()
						{ MgVect3 v1 = p[1] - p[0]; return v1.Unitize();}		// ü•ª‚Ì•ûŒü(’PˆÊƒxƒNƒgƒ‹)
	void Print(MCHAR* s);
};
//
//==========================================================================================
//
// ---------------------( ‚QŸŒ³ )------------------------------
//
//	‚QŸŒ³’¼ü
//
inline MgULine2 MgULine2C( const MgPoint2& p1, const MgVect2& v1)							// ’Ê‰ß“_‚Æ•ûŒü’PˆÊƒxƒNƒgƒ‹‚æ‚è‚QŸŒ³’¼ü‚ğ‹‚ß‚é
						{ MgULine2 ULno; ULno.p = p1; ULno.v = v1; return ULno;}										
inline MgULine2 MgULine2C( const MgULine3& ULn1)											// ‚RŸŒ³(x,y) ¨ ‚QŸŒ³
						{ MgULine2 ULno; ULno.p.x = ULn1.p.x; ULno.p.y = ULn1.p.y;
						  ULno.v.x = ULn1.v.x, ULno.v.y = ULn1.v.y; return ULno;} 
inline MgULine2 MgULine2::Set( const MgULine3& ULn1)										// ‚RŸŒ³(x,y) ¨ ‚QŸŒ³
						{ MgULine2 ULno; ULno.p.x = ULn1.p.x; ULno.p.y = ULn1.p.y;
						  ULno.v.x = ULn1.v.x; ULno.v.y = ULn1.v.y; return ULno;} 
//
//	‚QŸŒ³ü•ª
//
inline MgLine2 MgLine2C( const MgLine3& Ln1)												// ‚RŸŒ³(x,y)¨‚QŸŒ³
						{ MgLine2 Lno; Lno.p[0].x = Ln1.p[0].x; Lno.p[0].y = Ln1.p[0].y;
						  Lno.p[1].x = Ln1.p[1].x; Lno.p[1].y = Ln1.p[1].y; return Lno;} 
inline MgLine2 MgLine2C( const MgPoint3& Ps, const MgPoint3& Pe)							// ‚RŸŒ³(x,y)¨‚QŸŒ³
						{ MgLine2 Lno; Lno.p[0].x = Ps.x; Lno.p[0].y = Ps.y;
						  Lno.p[1].x = Pe.x; Lno.p[1].y = Pe.y; return Lno;} 
inline MgLine2 MgLine2C( const MgPoint3 *Pi)												// ‚RŸŒ³(x,y)¨‚QŸŒ³
						{ MgLine2 Lno; Lno.p[0].x = Pi[0].x; Lno.p[0].y = Pi[0].y;
						  Lno.p[1].x = Pi[1].x; Lno.p[1].y = Pi[1].y; return Lno;} 

inline MgVect2 MgVect2C( const MgLine2& Ln1)												// Line2 ¨ Vect2
						{ return ( Ln1.p[1] - Ln1.p[0]);}
inline MgLine2 MgLine2::operator + ( const MgVect2& v) const								// Ln1 = Ln2 + p3
				 		{ return MgLine2( MgPoint2( p[0].x + v.x, p[0].y + v.y),
										  MgPoint2( p[1].x + v.x, p[1].y + v.y));}
inline MgLine2 MgLine2::operator - ( const MgVect2& v) const								// Ln1 = Ln2 + p3
				 		{ return MgLine2( MgPoint2( p[0].x - v.x, p[0].y - v.y),
										  MgPoint2( p[1].x - v.x, p[1].y - v.y));}
inline MgLine2 MgLine2::operator += ( const MgVect2& v)										// Ln1 += P2
				 		{ return MgLine2( MgPoint2( p[0].x += v.x, p[0].y += v.y),
										  MgPoint2( p[1].x += v.x, p[1].y += v.y));}
inline MgLine2 MgLine2::operator -= ( const MgVect2& v)										// Ln1 += P2
				 		{ return MgLine2( MgPoint2( p[0].x -= v.x, p[0].y -= v.y),
										  MgPoint2( p[1].x -= v.x, p[1].y -= v.y));}
inline MgLine2 MgLine2::Set( const MgLine3& Ln1)											// ‚RŸŒ³(x,y)¨‚QŸŒ³
						{ MgLine2 Lno; Lno.p[0].x = Ln1.p[0].x; Lno.p[0].y = Ln1.p[0].y;
						  Lno.p[1].x = Ln1.p[1].x; Lno.p[1].y = Ln1.p[1].y;
						  return Lno;} 

// ---------------------( ‚RŸŒ³ )------------------------------
//
//	‚RŸŒ³’¼ü
//
inline MgULine3 MgULine3C( const MgULine2& uln1, const MREAL z1 = 0.f)						// ‚QŸŒ³¨‚RŸŒ³@(ZÈ—ª0.)
						{ MgULine3 ULno; ULno.p.x = uln1.p.x; ULno.p.y = uln1.p.y; ULno.p.z = z1;
						  ULno.v = MgVect3C( uln1.p).Unitize(); return ULno;} 
inline MgULine3 MgULine3C( const MgULine2& ULn1, const MgPlane3& Pln);						// ‚QŸŒ³¨‚RŸŒ³

inline MgLine3 MgLine3C( const MgLine2& ln1, MREAL z = 0.)									// ‚QŸŒ³¨‚RŸŒ³@(ZÈ—ª0.)
						{ MgLine3 Lno; Lno.p[0].x = ln1.p[0].x; Lno.p[0].y = ln1.p[0].y;
									   Lno.p[0].z = z;
									   Lno.p[1].x = ln1.p[1].x; Lno.p[1].y = ln1.p[1].y;
									   Lno.p[1].z = z; return Lno;}
inline MgLine3 MgLine3C( const MgLine2& ln1, const MgPlane3& Pln);							// ‚QŸŒ³¨‚RŸŒ³

inline MgLine3 MgLine3C( const MgPoint2& ps, const MgPoint2& pe, MREAL z = 0.)				// ‚QŸŒ³¨‚RŸŒ³@(ZÈ—ª0.)
						{ MgLine3 Lno; Lno.p[0].x = ps.x; Lno.p[0].y = ps.y; Lno.p[0].z = z;
						  Lno.p[1].x = pe.x; Lno.p[1].y = pe.y; Lno.p[1].z = z; return Lno;} 
inline MgLine3 MgLine3C( const MgPoint2& ps, const MgPoint2& pe, const MgPlane3& Pln)		// ‚QŸŒ³¨‚RŸŒ³
							{ return MgLine3C( MgLine2( ps, pe), Pln);} 
inline MgLine3 MgLine3C( const MgPoint2 *pi, MREAL z = 0.)									// ‚QŸŒ³¨‚RŸŒ³@(ZÈ—ª0.)
						{ MgLine3 Lno; Lno.p[0].x = pi[0].x; Lno.p[0].y = pi[0].y; Lno.p[0].z = z;
						  Lno.p[1].x = pi[1].x; Lno.p[1].y = pi[1].y; Lno.p[1].z = z; return Lno;} 
inline MgLine3 MgLine3C( const MgPoint2 *pi, const MgPlane3& Pln)		// ‚QŸŒ³¨‚RŸŒ³
							{ return MgLine3C( MgLine2( pi), Pln);} 
inline MgVect3 MgVect3C( const MgLine3& Ln1)												// Line3 ¨ Vect3
						{ return ( Ln1.p[1] - Ln1.p[0]);}
inline MgLine3 MgLine3::operator + ( const MgVect3& v) const								// Ln1 = Ln2 + p3
				 		{ return MgLine3( p[0].x + v.x, p[0].y + v.y, p[0].z + v.z,
										  p[1].x + v.x, p[1].y + v.y, p[1].z + v.z);}
inline MgLine3 MgLine3::operator - ( const MgVect3& v) const								// Ln1 = Ln2 + p3
				 		{ return MgLine3( p[0].x - v.x, p[0].y - v.y, p[0].z - v.z,
										  p[1].x - v.x, p[1].y - v.y, p[1].z - v.z);}
inline MgLine3 MgLine3::operator += ( const MgVect3& v)										// Ln1 += P2
				 		{ return MgLine3( p[0].x += v.x, p[0].y += v.y, p[0].z += v.z,
										  p[1].x += v.x, p[1].y += v.y, p[1].z += v.z);}
inline MgLine3 MgLine3::operator -= ( const MgVect3& v)										// Ln1 += P2
				 		{ return MgLine3( p[0].x -= v.x, p[0].y -= v.y, p[0].z -= v.z,
										  p[1].x -= v.x, p[1].y -= v.y, p[1].z -= v.z);}

// ƒTƒCƒY
inline MINT SZMgULine2( MINT i_sz = 1)	{ return  ( i_sz * (MINT)sizeof( MgULine2));}
inline MINT SZMgULine3( MINT i_sz = 1)	{ return  ( i_sz * (MINT)sizeof( MgULine3));}
inline MINT SZMgHLine2( MINT i_sz = 1)	{ return  ( i_sz * (MINT)sizeof( MgHLine2));}
inline MINT SZMgHLine3( MINT i_sz = 1)	{ return  ( i_sz * (MINT)sizeof( MgHLine3));}
inline MINT SZMgLine2( MINT i_sz = 1)	{ return  ( i_sz * (MINT)sizeof( MgLine2));}
inline MINT SZMgLine3( MINT i_sz = 1)	{ return  ( i_sz * (MINT)sizeof( MgLine3));}
inline MINT WSZMgULine2( MINT i_sz = 1)	{ return  ( i_sz * (MINT)sizeof( MgULine2) / SZMINT());}
inline MINT WSZMgULine3( MINT i_sz = 1)	{ return  ( i_sz * (MINT)sizeof( MgULine3) / SZMINT());}
inline MINT WSZMgHLine2( MINT i_sz = 1)	{ return  ( i_sz * (MINT)sizeof( MgHLine2) / SZMINT());}
inline MINT WSZMgHLine3( MINT i_sz = 1)	{ return  ( i_sz * (MINT)sizeof( MgHLine3) / SZMINT());}
inline MINT WSZMgLine2( MINT i_sz = 1)	{ return  ( i_sz * (MINT)sizeof( MgLine2) / SZMINT());}
inline MINT WSZMgLine3( MINT i_sz = 1)	{ return  ( i_sz * (MINT)sizeof( MgLine3) / SZMINT());}

} // namespace MC
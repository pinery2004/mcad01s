#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgRect2.h
//
//		‹éŒ`
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================

#include "MgPoint.h"
#include "MgLine.h"

class MgPolyg2;
class MgPolyg3;

#include "MbPlex.h"

#ifdef DLL_EXPORT_RECT
	#undef DLL_EXPORT_RECT
#endif

#ifdef DLL_EXPORT_RECT_DO
	#pragma message( "<<< MgRectEdll_EXport >>>")
	#define DLL_EXPORT_RECT							__declspec( dllexport)
#else
	#ifdef _DEBUG								// Release Mode ‚Å inlineŠÖ”‚ªundef‚É‚È‚é•s‹ï‡‘Î‰ž
		#ifdef DLL_NO_IMPORT_GEO_DO
			#define DLL_EXPORT_RECT
		#else
//			#pragma message( "=== MgRectEdll_IMport ===")
			#define DLL_EXPORT_RECT						__declspec( dllimport)
		#endif
	#else
		#define DLL_EXPORT_RECT
	#endif
#endif

namespace MC
{

//
//===========================================================================
//		‚l‚h‚Ž^‚l‚‚˜
//
//======================( ‚QŽŸŒ³ )==============================
//	®”Œ^
class DLL_EXPORT_RECT MgMinMaxI2
{
public:
	CPoint			min;											// Å¬À•W‚Ì“_			
	CPoint			max;											// Å‘åÀ•W‚Ì“_
//
public:
	MgMinMaxI2() {};
	MgMinMaxI2(MINT i1x, MINT i1y, MINT i2x, MINT i2y) {
						min.x = i1x; min.y = i1y;
						max.x = i2x; max.y = i2y;}
	MgMinMaxI2(const CPoint &cp1, const CPoint &cp2) {
						min.x = min( cp1.x, cp2.x);
						min.y = min( cp1.y, cp2.y);
						max.x = max( cp1.x, cp2.x);
						max.y = max( cp1.y, cp2.y);}
	void SetUnit();
	void SetInit();
	void Ins2(const CPoint &pt) {
						if (min.x > pt.x) min.x = pt.x;	if (max.x < pt.x) max.x = pt.x;
						if (min.y > pt.y) min.y = pt.y;	if (max.y < pt.y) max.y = pt.y;}				
	void Ins2(const MgMinMaxI2 &imm1) {
						if (min.x > imm1.min.x) min.x = imm1.min.x;
						if (max.x < imm1.max.x) max.x = imm1.max.x;
						if (min.y > imm1.min.y) min.y = imm1.min.y;
						if (max.y < imm1.max.y) max.y = imm1.max.y;}				
	CPoint GetCenter()
						{ return CPoint( (min.x + max.x) / 2, (min.y + max.y) / 2);}
	MINT GetWidth()
						{ return (max.x - min.x);}
	MINT GetHgt()
						{ return (max.y - min.y);}

	MgMinMaxI2 operator | ( const MgMinMaxI2 &imm1) const							// mm1 = mm2 | mm3
						{ return MgMinMaxI2( MGMIN( min.x, imm1.min.x),
											 MGMIN( min.y, imm1.min.y),
											 MGMAX( max.x, imm1.max.x),
											 MGMAX( max.y, imm1.max.y));}
	MgMinMaxI2 operator + ( const CPoint &pt) const									// mm1 = mm2 + pt
						{ return MgMinMaxI2( min + pt,
										   max + pt);}
	MgMinMaxI2 operator - ( const CPoint &pt) const									// mm1 = mm2 - pt
						{ return MgMinMaxI2( min - pt,
										   max - pt);}
	MgMinMaxI2 operator += ( const CPoint &pt)										// mm1 += pt
						{ min += pt; max += pt; return *this;}
	MgMinMaxI2 operator -= ( const CPoint &pt)										// mm1 -= pt
						{ min -= pt; max -= pt; return *this;}
	friend MgMinMaxI2 operator * ( MINT isc, const MgMinMaxI2 &mm)					// mm1 = isc * mm2
						{ return MgMinMaxI2( mm.min.x * isc, mm.min.y * isc, 
										   mm.max.x * isc, mm.max.y * isc);}
	friend MgMinMaxI2 operator * ( MREAL rsc, const MgMinMaxI2 &mm)				// mm1 = rsc * mm2
						{ return MgMinMaxI2( MINT(mm.min.x * rsc),
										   MINT(mm.min.y * rsc), 
										   MINT(mm.max.x * rsc), 
										   MINT(mm.max.y * rsc));}
	MgMinMaxI2 operator * ( MINT isc) const											// mm1 = mm2 * isc
						{ return MgMinMaxI2( min.x * isc, min.y * isc, 
										   max.x * isc, max.y * isc);}
	MgMinMaxI2 operator * ( MREAL rsc) const										// mm1 = mm2 * rsc
						{ return MgMinMaxI2( MINT(min.x * rsc),
										   MINT(min.y * rsc), 
										   MINT(max.x * rsc),
										   MINT(max.y * rsc));}
	MgMinMaxI2 operator / ( MINT isc) const											// mm1 = mm2 / isc
						{ return MgMinMaxI2( min.x / isc, min.y / isc, 
										   max.x / isc, max.y / isc);}
	MgMinMaxI2 operator / ( MREAL rsc) const										// mm1 = mm2 / rsc
						{ return MgMinMaxI2( MINT(min.x / rsc),
										   MINT(min.y / rsc), 
										   MINT(max.x / rsc),
										   MINT(max.y / rsc));}
	MgMinMaxI2 operator *= ( MINT isc)												// mm1 *= isc
						{ return MgMinMaxI2( min.x *= isc, min.y *= isc, 
										   max.x *= isc, max.y *= isc);}
	MgMinMaxI2 operator *= ( MREAL rsc)											// mm1 *= rsc
						{ return MgMinMaxI2( min.x = MINT(min.x * rsc),
										   min.y = MINT(min.y * rsc), 
										   max.x = MINT(max.x * rsc),
										   max.y = MINT(max.y * rsc));}
	MgMinMaxI2 operator /= ( MINT isc)												// mm1 /= isc
						{ return MgMinMaxI2( min.x /= isc, min.y /= isc, 
										   max.x /= isc, max.y /= isc);}
	MgMinMaxI2 operator /= ( MREAL rsc)											// mm1 /= rsc
						{ return MgMinMaxI2( min.x = MINT(min.x / rsc),
										   min.y = MINT(min.y / rsc), 
										   max.x = MINT(max.x / rsc),
										   max.y = MINT(max.y / rsc));}

	void Print(MCHAR* s);														// print
};
//======================( ‚QŽŸŒ³ )==============================
//	ŽÀ”Œ^
class DLL_EXPORT_RECT MgMinMaxR2
{
public:
	MgPoint2			min;											// Å¬À•W‚Ì“_			
	MgPoint2			max;											// Å‘åÀ•W‚Ì“_
//
public:
	MgMinMaxR2() {};
	MgMinMaxR2(MREAL r1x, MREAL r1y, MREAL r2x, MREAL r2y) {
						min.x = r1x; min.y = r1y; 
						max.x = r2x; max.y = r2y;}
	MgMinMaxR2(const MgPoint2& p1, const MgPoint2& p2) {
						min.x = min( p1.x, p2.x);
						min.y = min( p1.y, p2.y);
						max.x = max( p1.x, p2.x);
						max.y = max( p1.y, p2.y);}
	MgMinMaxR2(const MgLine2& Ln1) {
						min.x = min( Ln1.p[0].x, Ln1.p[1].x);
						min.y = min( Ln1.p[0].y, Ln1.p[1].y);
						max.x = max( Ln1.p[0].x, Ln1.p[1].x);
						max.y = max( Ln1.p[0].y, Ln1.p[1].y);}
	void SetUnit();
	void SetInit();
	void Ins2(const MgPoint2& pt) {
						if (min.x > pt.x) min.x = pt.x;	if (max.x < pt.x) max.x = pt.x;
						if (min.y > pt.y) min.y = pt.y;	if (max.y < pt.y) max.y = pt.y;}				
	void Ins2(const MgLine2& ln) {
						if (min.x > ln.p[0].x) min.x = ln.p[0].x; if (max.x < ln.p[0].x) max.x = ln.p[0].x;
						if (min.y > ln.p[0].y) min.y = ln.p[0].y; if (max.y < ln.p[0].y) max.y = ln.p[0].y;
						if (min.x > ln.p[1].x) min.x = ln.p[1].x; if (max.x < ln.p[1].x) max.x = ln.p[1].x;
						if (min.y > ln.p[1].y) min.y = ln.p[1].y; if (max.y < ln.p[1].y) max.y = ln.p[1].y;}				
	void Ins2(const MgPolyg2 &pg);

	void Ins2(const MgMinMaxR2 &mm1) {
						if (min.x > mm1.min.x) min.x = mm1.min.x;
						if (max.x < mm1.max.x) max.x = mm1.max.x;
						if (min.y > mm1.min.y) min.y = mm1.min.y;
						if (max.y < mm1.max.y) max.y = mm1.max.y;}				
	MgPoint2 GetCenter()
						{ return MgPoint2( (min.x + max.x) * 0.5f, (min.y + max.y) * 0.5f);}
	MREAL GetWidth()
						{ return (max.x - min.x);}
	MREAL GetHgt()
						{ return (max.y - min.y);}

	MgMinMaxR2 operator | ( const MgMinMaxR2 &mm1) const							// mm1 = mm2 | mm3
						{ return MgMinMaxR2( MGMIN( min.x, mm1.min.x),
											 MGMIN( min.y, mm1.min.y),
											 MGMAX( max.x, mm1.max.x),
											 MGMAX( max.y, mm1.max.y));}
	MgMinMaxR2 operator + ( const MgPoint2& pt) const								// mm1 = mm2 + pt
						{ return MgMinMaxR2( min + pt,
										   max + pt);}
	MgMinMaxR2 operator - ( const MgPoint2& pt) const								// mm1 = mm2 - pt
						{ return MgMinMaxR2( min - pt,
										   max - pt);}
	MgMinMaxR2 operator += ( const MgPoint2& pt)									// mm1 += pt
						{ min += pt; max += pt; return *this;}
	MgMinMaxR2 operator -= ( const MgPoint2& pt)									// mm1 -= pt
						{ min -= pt; max -= pt; return *this;}
	friend MgMinMaxR2 operator * ( MREAL rsc, const MgMinMaxR2 &mm)				// mm1 = rsc * mm2
						{ return MgMinMaxR2( mm.min.x * rsc, mm.min.y * rsc, 
										   mm.max.x * rsc, mm.max.y * rsc);}
	MgMinMaxR2 operator * ( MREAL rsc) const										// mm1 = mm2 * rsc
						{ return MgMinMaxR2( min.x * rsc, min.y * rsc, 
										   max.x * rsc, max.y * rsc);}
	MgMinMaxR2 operator / ( MREAL rsc) const										// mm1 = mm2 / rsc
						{ return MgMinMaxR2( min.x / rsc, min.y / rsc, 
										   max.x / rsc, max.y / rsc);}
	MgMinMaxR2 operator *= ( MREAL rsc)											// mm1 *= rsc
						{ return MgMinMaxR2( min.x *= rsc, min.y *= rsc, 
										   max.x *= rsc, max.y *= rsc);}
	MgMinMaxR2 operator /= ( MREAL rsc)											// mm1 /= rsc
						{ return MgMinMaxR2( min.x /= rsc, min.y /= rsc, 
										   max.x /= rsc, max.y /= rsc);}

	void Print(MCHAR* s);														// print
};
//======================( ‚RŽŸŒ³ )==============================
//	ŽÀ”Œ^
class DLL_EXPORT_RECT MgMinMaxR3
{
public:
	MgPoint3		min;											// Å¬À•W‚Ì“_
	MgPoint3		max;											// Å‘åÀ•W‚Ì“_
//
public:
	MgMinMaxR3() {};
	MgMinMaxR3(MREAL minx, MREAL miny, MREAL minz,
			   MREAL maxx, MREAL maxy, MREAL maxz) {
						min.x = minx; min.y = miny; min.z = minz;
						max.x = maxx; max.y = maxy; max.z = maxz;}
	MgMinMaxR3(const MgPoint3& min1, const MgPoint3& max1) {
						min.x = min1.x; min.y = min1.y; min.z = min1.z;
						max.x = max1.x; max.y = max1.y; max.z = max1.z;}
	MgMinMaxR3(const MgLine3& Ln1) {
						min.x = min( Ln1.p[0].x, Ln1.p[1].x);
						min.y = min( Ln1.p[0].y, Ln1.p[1].y);
						min.z = min( Ln1.p[0].z, Ln1.p[1].z);
						max.x = max( Ln1.p[0].x, Ln1.p[1].x);
						max.y = max( Ln1.p[0].y, Ln1.p[1].y);
						max.z = max( Ln1.p[0].z, Ln1.p[1].z);}
	void SetUnit();
	void SetInit();
	// “_‚ÌÀ•W‚ð’Ç‰Á‚µ‚½MinMax‚ð
	void Ins3(const MgPoint3& Pt) {
						if (min.x > Pt.x) min.x = Pt.x;	if (max.x < Pt.x) max.x = Pt.x;
						if (min.y > Pt.y) min.y = Pt.y;	if (max.y < Pt.y) max.y = Pt.y;				
						if (min.z > Pt.z) min.z = Pt.z;	if (max.z < Pt.z) max.z = Pt.z;}				
	void Ins3(const MgLine3& Ln) {
						if (min.x > Ln.p[0].x) min.x = Ln.p[0].x; if (max.x < Ln.p[0].x) max.x = Ln.p[0].x;
						if (min.y > Ln.p[0].y) min.y = Ln.p[0].y; if (max.y < Ln.p[0].y) max.y = Ln.p[0].y;
						if (min.z > Ln.p[0].z) min.z = Ln.p[0].z; if (max.z < Ln.p[0].z) max.z = Ln.p[0].z;
						if (min.x > Ln.p[1].x) min.x = Ln.p[1].x; if (max.x < Ln.p[1].x) max.x = Ln.p[1].x;
						if (min.y > Ln.p[1].y) min.y = Ln.p[1].y; if (max.y < Ln.p[1].y) max.y = Ln.p[1].y;
						if (min.z > Ln.p[1].z) min.z = Ln.p[1].z; if (max.z < Ln.p[1].z) max.y = Ln.p[1].z;}				
	void Ins3(const MgPolyg3 &Ln);

	void Ins3(const MgMinMaxR3 &mm1) {
						if (min.x > mm1.min.x) min.x = mm1.min.x;
						if (max.x < mm1.max.x) max.x = mm1.max.x;
						if (min.y > mm1.min.y) min.y = mm1.min.y;
						if (max.y < mm1.max.y) max.y = mm1.max.y;}				

	MgPoint3 GetCenter()
						{ return MgPoint3( (min.x + max.x) * 0.5f, (min.y + max.y) * 0.5f, (min.z + max.z) * 0.5f);}
	MREAL GetWidth()
						{ return (max.x - min.x);}
	MREAL GetHgt()
						{ return (max.y - min.y);}
	MREAL GetDepth()
						{ return (max.z - min.z);}

	MgMinMaxR3 operator | ( const MgMinMaxR3 &mm1) const							// mm1 = mm2 | mm3
						{ return MgMinMaxR3( MGMIN( min.x, mm1.min.x),
											 MGMIN( min.y, mm1.min.y),
											 MGMIN( min.z, mm1.min.z),
											 MGMAX( max.x, mm1.max.x),
											 MGMAX( max.y, mm1.max.y),
											 MGMAX( max.z, mm1.max.z));}
	MgMinMaxR3 operator + ( const MgPoint3& pt) const								// mm1 = mm2 + pt
						{ return MgMinMaxR3( min + pt,
										   max + pt);}
	MgMinMaxR3 operator - ( const MgPoint3& pt) const								// mm1 = mm2 - pt
						{ return MgMinMaxR3( min - pt,
										   max - pt);}
	MgMinMaxR3 operator += ( const MgPoint3& pt)									// mm1 += pt
						{ min += pt; max += pt; return *this;}
	MgMinMaxR3 operator -= ( const MgPoint3& pt)									// mm1 -= pt
						{ min -= pt; max -= pt; return *this;}
	friend MgMinMaxR3 operator * ( MREAL rsc, const MgMinMaxR3 &mm)					// mm1 = rsc * mm2
						{ return MgMinMaxR3( mm.min.x * rsc, mm.min.y * rsc, mm.min.z * rsc, 
											mm.max.x * rsc, mm.max.y * rsc, mm.max.z * rsc);}
	MgMinMaxR3 operator * ( MREAL rsc) const											// mm1 = mm2 * rsc
						{ return MgMinMaxR3( min.x * rsc, min.y * rsc, min.z * rsc,
											max.x * rsc, max.y * rsc, max.z * rsc);}
	MgMinMaxR3 operator / ( MREAL rsc) const											// mm1 = mm2 / rsc
						{ return MgMinMaxR3( min.x / rsc, min.y / rsc, min.z / rsc,
											max.x / rsc, max.y / rsc, max.z / rsc);}
	MgMinMaxR3 operator *= ( MREAL rsc)												// mm1 *= rsc
						{ return MgMinMaxR3( min.x *= rsc, min.y *= rsc, min.z *= rsc,
											max.x *= rsc, max.y *= rsc, max.z *= rsc);}
	MgMinMaxR3 operator /= ( MREAL rsc)												// mm1 /= rsc
						{ return MgMinMaxR3( min.x /= rsc, min.y /= rsc, min.z /= rsc,
											max.x /= rsc, max.y /= rsc, max.z /= rsc);}

	void Print(MCHAR* s);														// print
};
//
//======================( ‚QŽŸŒ³ )==============================
//	’·•ûŒ`
class DLL_EXPORT_RECT MgRect2
{
public:
	MgPoint2 p[2];
//
public:
	MgRect2() {};
	MgRect2(MREAL r1x, MREAL r1y, MREAL r2x, MREAL r2y) {
		p[0].x = r1x; p[0].y = r1y; p[1].x = r2x; p[1].y = r2y;}
	MgRect2(const MgPoint2& min, const MgPoint2& max) {
		p[0].x = min.x; p[0].y = min.y; p[1].x = max.x; p[1].y = max.y;}
	void SetUnit();
	MgPoint2 GetCenter()
						{ return MgPoint2( (p[0].x + p[1].x) * 0.5f, (p[0].y + p[1].y) * 0.5f);}
	MREAL GetWidth()
						{ return (p[1].x - p[0].x);}
	MREAL GetHgt()
						{ return (p[1].y - p[0].y);}
	bool Overlap(const MgRect2&, const MgRect2);

	MgRect2 operator + ( const MgPoint2& pt) const									// rc1 = rc2 + pt
						{ return MgRect2( p[0] + pt,
										 p[1] + pt);}
	MgRect2 operator - ( const MgPoint2& pt) const									// rc1 = rc2 - pt
						{ return MgRect2( p[0] - pt,
										 p[1] - pt);}
	MgRect2 operator += ( const MgPoint2& pt)										// rc1 += pt
						{ p[0] += pt; p[1] += pt; return *this;}
	MgRect2 operator -= ( const MgPoint2& pt)										// rc1 -= pt
						{ p[0] -= pt; p[1] -= pt; return *this;}
	friend MgRect2 operator * ( MREAL rsc, const MgRect2 &rc)						// rc1 = rsc * rc2
						{ return MgRect2( rc.p[0].x * rsc, rc.p[0].y * rsc, 
										 rc.p[1].x * rsc, rc.p[1].y * rsc);}
	MgRect2 operator * ( MREAL rsc) const											// rc1 = rc2 * rsc
						{ return MgRect2( p[0].x * rsc, p[0].y * rsc, 
										 p[1].x * rsc, p[1].y * rsc);}
	MgRect2 operator / ( MREAL rsc) const											// rc1 = rc2 / rsc
						{ return MgRect2( p[0].x / rsc, p[0].y / rsc, 
										 p[1].x / rsc, p[1].y / rsc);}
	MgRect2 operator *= ( MREAL rsc)												// rc1 *= rsc
						{ return MgRect2( p[0].x *= rsc, p[0].y *= rsc, 
										 p[1].x *= rsc, p[1].y *= rsc);}
	MgRect2 operator /= ( MREAL rsc)												// rc1 /= rsc
						{ return MgRect2( p[0].x /= rsc, p[0].y /= rsc, 
										 p[1].x /= rsc, p[1].y /= rsc);}

	void Print(MCHAR* s);															// print
};
//
//======================( ‚RŽŸŒ³ )==============================
//	’¼•û‘Ì
class DLL_EXPORT_RECT MgRect3
{
public:
	MgPoint3 p[2];
//
public:
	MgRect3() {};
	MgRect3(MREAL r1x, MREAL r1y, MREAL r1z, 
		    MREAL r2x, MREAL r2y, MREAL r2z) {
		p[0].x = r1x; p[0].y = r1y; p[0].z = r1z;
		p[1].x = r2x; p[1].y = r2y; p[1].z = r2z;}
	MgRect3(const MgPoint3& min, const MgPoint3& max) {
		p[0].x = min.x; p[0].y = min.y; p[0].z = min.z;
		p[1].x = max.x; p[1].y = max.y; p[1].z = max.z;}
	void SetUnit();
	MgPoint3 GetCenter()
						{ return MgPoint3( (p[0].x + p[1].x) * 0.5f,
										   (p[0].y + p[1].y) * 0.5f,
										   (p[0].z + p[1].z) * 0.5f);}
	MREAL GetWidth()
						{ return (p[1].x - p[0].x);}
	MREAL GetHgt()
						{ return (p[1].y - p[0].y);}
	MREAL GetDepth()
						{ return (p[1].z - p[0].z);}
	bool Overlap(const MgRect3&, const MgRect3);

	MgRect3 operator + ( const MgPoint3& pt) const								// rc1 = rc2 + pt
						{ return MgRect3( p[0] + pt,
										  p[1] + pt);}
	MgRect3 operator - ( const MgPoint3& pt) const								// rc1 = rc2 - pt
						{ return MgRect3( p[0] - pt,
										  p[1] - pt);}
	MgRect3 operator += ( const MgPoint3& pt)									// rc1 += pt
						{ p[0] += pt; p[1] += pt; return *this;}
	MgRect3 operator -= ( const MgPoint3& pt)									// rc1 -= pt
						{ p[0] -= pt; p[1] -= pt; return *this;}

	friend MgRect3 operator * ( MREAL rsc, const MgRect3 &rc)					// rc1 = rsc * rc2
						{ return MgRect3( rc.p[0].x * rsc, rc.p[0].y * rsc, rc.p[0].z * rsc,
										  rc.p[1].x * rsc, rc.p[1].y * rsc, rc.p[1].z * rsc);}
	MgRect3 operator * ( MREAL rsc) const										// rc1 = rc2 * rsc
						{ return MgRect3( p[0].x * rsc, p[0].y * rsc, p[0].z * rsc,
										  p[1].x * rsc, p[1].y * rsc, p[1].z * rsc);}
	MgRect3 operator / ( MREAL rsc) const										// rc1 = rc2 / rsc
						{ return MgRect3( p[0].x / rsc, p[0].y / rsc, p[0].z / rsc,
										  p[1].x / rsc, p[1].y / rsc, p[1].z / rsc);}
	MgRect3 operator *= ( MREAL rsc)											// rc1 *= rsc
						{ return MgRect3( p[0].x *= rsc, p[0].y *= rsc, p[0].z *= rsc,
										  p[1].x *= rsc, p[1].y *= rsc, p[1].z *= rsc);}
	MgRect3 operator /= ( MREAL rsc)											// rc1 /= rsc
						{ return MgRect3( p[0].x /= rsc, p[0].y /= rsc, p[0].z /= rsc,
										  p[1].x /= rsc, p[1].y /= rsc, p[1].z /= rsc);}

	void Print(MCHAR* s);														// print
};

inline MINT SZMgMinMaxI2( MINT i_sz = 1)	{ return  ( i_sz * (MINT)sizeof( MgMinMaxI2));}
inline MINT SZMgMinMaxR2( MINT i_sz = 1)	{ return  ( i_sz * (MINT)sizeof( MgMinMaxR2));}
inline MINT SZMgMinMaxR3( MINT i_sz = 1)	{ return  ( i_sz * (MINT)sizeof( MgMinMaxR3));}
inline MINT SZMgRect2( MINT i_sz = 1)	{ return  ( i_sz * (MINT)sizeof( MgRect2));}
inline MINT SZMgRect3( MINT i_sz = 1)	{ return  ( i_sz * (MINT)sizeof( MgRect3));}

inline MINT WSZMgMinMaxI2( MINT i_sz = 1)	{ return  ( i_sz * (MINT)sizeof( MgMinMaxI2) / SZMINT());}
inline MINT WSZMgMinMaxR2( MINT i_sz = 1)	{ return  ( i_sz * (MINT)sizeof( MgMinMaxR2) / SZMINT());}
inline MINT WSZMgMinMaxR3( MINT i_sz = 1)	{ return  ( i_sz * (MINT)sizeof( MgMinMaxR3) / SZMINT());}
inline MINT WSZMgRect2( MINT i_sz = 1)	{ return  ( i_sz * (MINT)sizeof( MgRect2) / SZMINT());}
inline MINT WSZMgRect3( MINT i_sz = 1)	{ return  ( i_sz * (MINT)sizeof( MgRect3) / SZMINT());}

} // namespace MC
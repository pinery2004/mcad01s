//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: .cpp
//
//		
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"

#define	DLL_EXPORT_GEO_DO

#include "MgLib.h"

namespace MC
{

//===========================================================================
//		点　形状演算
//
// ---------------------( ２次元 )------------------------------
//
//		２点間の距離を求める。
//
MREAL MGeo::Dist2Pt2(						// (  O) 直線から点までの距離 >0:左側 <0:右側
				const	MgPoint2&	p1,			// (I  ) 点1
				const	MgPoint2&	p2			// (I  ) 点2
		)
{
	MgVect2	v1;
	MREAL	pa1;								//	直線から点までの距離
//
	v1 = p1 - p2;
	pa1 = (MREAL)sqrt( v1 * v1);
	return pa1;
}

//===========================================================================
//		点　形状演算
//
// ---------------------( ３次元 )------------------------------
//
//		２点間の距離を求める。
//
MREAL MGeo::Dist2Pt3(						// (  O) ２点間の距離 >0:左側 <0:右側
				const	MgPoint3&	p1,			// (I  ) 点1
				const	MgPoint3&	p2			// (I  ) 点2
		)
{
	MgVect3	v1;
	MREAL	pa1;								//	直線から点までの距離
//
	v1 = p1 - p2;
	pa1 = (MREAL)sqrt( v1 * v1);
	return pa1;
}

//===========================================================================
//		直線　形状演算
//
// ---------------------( ２次元 )------------------------------
//
//		直線から点までの正負方向付き距離を求める。
//
MREAL MGeo::DistPtULn2(						// (  O) 直線から点までの距離 >0:左側 <0:右側
				const	MgPoint2&	p1,			// (I  ) 点1
				const	MgULine2&	ULn2		// (I  ) 直線2
		)
{
	MgVect2	v1s2;
	MREAL	pa1;								//	直線から点までの距離
//
	v1s2 = p1 - ULn2.p;
	pa1 = ULn2.v ^ v1s2;
	return pa1;
}
//	------------------------
//		線分から点までの正負方向付き距離を求める。(直線と同様）
//
MREAL MGeo::DistPtLn2(						// (  O) 線分から点までの距離 >0:左側 <0:右側
												//	線分の長さが0の場合は点から線分の始点までの距離
				const	MgPoint2&	p1,			// (I  ) 点1
				const	MgLine2&	Ln2			// (I  ) 線分2
		)
{
	MgVect2	v2s1;
	MgVect2	v2;
	MREAL	sz2;								//	線分2の長さ
	MREAL	pa1;								//	線分から点までの距離
//
	v2s1 = p1 - Ln2.p[0];
	v2 = Ln2.p[1] - Ln2.p[0];
	sz2 = Abs(v2);
	if (sz2 < g_gTol.D) return Abs(v2s1);
	pa1 = (v2 ^ v2s1) / sz2;
	return pa1;
}
// ---------------------( ３次元 )------------------------------
//		直線から点までの距離を求める。
//
MREAL MGeo::DistPtULn3(						// (  O) 直線から点までの距離
				const	MgPoint3&	p1,			// (I  ) 点1
				const	MgULine3&	ULn2		// (I  ) 直線2
		)
{
	MgVect3	v21, va1;
	MREAL pa1;									//	直線から点までの距離
//
	v21 = p1 - ULn2.p;
	va1 = v21 ^ ULn2.v;
	pa1 = Abs(va1);
	return pa1;
}
//
//		線分から点までの距離を求める。(直線と同様）
//
MREAL MGeo::DistPtLn3(						// (  O) 線分から点までの距離
												//	線分の長さが0の場合は点から線分の始点までの距離
				const	MgPoint3&	p1,			// (I  ) 点1
				const	MgLine3	&Ln2			// (I  ) 線分2
		)
{
	MgVect3	v21;
	MgVect3	v2;
	MREAL	sz2;
	MgVect3 va1;
	MREAL pa1;									//	線分から点までの距離
//
	v21 = p1 - Ln2.p[0];
	v2 = Ln2.p[1] - Ln2.p[0];
	sz2 = Abs(v2);
	if (sz2 < g_gTol.D) return Abs(v21);
	va1 = v21 ^ v2;
	pa1 = Abs(va1) / sz2;
	return pa1;
}
// ---------------------( ３次元 )------------------------------
//		直線から点までの正負方向付き距離を求める。
//
MREAL MGeo::DistPtULnV3(						// (  O) 直線から点までの距離 >0:左側 <0:右側
				const	MgPoint3&	p1,			// (I  ) 点1
				const	MgULine3&	ULn2,		// (I  ) 直線2
				const	MgVect3&	vn			// (I  ) 点と直線に直交する単位ベクトル
		)
{
	MgVect3	v2s1, va1;
	MREAL pa1;									//	直線から点までの距離
//
	v2s1 = p1 - ULn2.p;
	va1 = ULn2.v ^ v2s1;
	pa1 = va1 * vn;
	return pa1;
}
//
//		線分から点までの正負方向付き距離を求める。(直線と同様）
//
MREAL MGeo::DistPtLnV3(						// (  O) 線分から点までの距離 >0:左側 <0:右側
												//		 線分の長さが0の場合は点から線分の始点までの距離
												//		 線分の長さが0の場合は点から線分の始点までの距離
				const	MgPoint3&	p1,			// (I  ) 点1
				const	MgLine3&	Ln2,		// (I  ) 線分2
				const	MgVect3&	vn			// (I  ) 点と直線に直交する単位ベクトル
		)
{
	MgVect3	v2s1;
	MgVect3	v2;
	MREAL	sz2;								//  線分2の長さ
	MgVect3 va1;
	MREAL pa1;									//	点から線分までの距離≧0
//
	v2s1 = p1 - Ln2.p[0];
	v2 = Ln2.p[1] - Ln2.p[0];
	sz2 = Abs(v2);
	if (sz2 < g_gTol.D) return Abs(v2s1);
	va1 = v2 ^ v2s1;
	pa1 = (va1 * vn) / sz2;
	return pa1;
}
// ---------------------( ３次元 )------------------------------
//		平面から点までの正負方向付き距離を求める。
//
MREAL MGeo::DistPtPln3(						// (  O) 点から平面までの距離 >0:表側（平面の単位ベクトル方向側）<0:裏側
				const	MgPoint3&	p1,			// (I  ) 点1
				const	MgPlane3&	Pln2		// (I  ) 平面2
		)
{
	MREAL	pa1;								//	点から平面までの距離
//
	pa1 = (p1 * Pln2.v) - Pln2.d;
	return pa1;
}

} // namespace MC
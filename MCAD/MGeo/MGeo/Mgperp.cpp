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

//
//===========================================================================
//		形状演算
//
////	-------------------------２ＤＬＩＮＥ・ＰＯＩＮＴ----------------------
////
////	点から直線への垂線の足位置を求める
////
//void MGeo::PerpULnPt2(
//				const	MgULine2&	ULn1,		// (I  ) 直線1
//				const	MgPoint2&	p2,			// (I  ) 点2
//						MgPoint2*	po			// (  O) 点から直線への垂線の足
//		)
//{
//	MREAL		pa1;							//	直線1の始点から垂線の足までの距離
//
//	pa1 = (p2 - ULn1.p) * ULn1.v;				//	cosΘv12v2
//	*po = ULn1.p + ULn1.v * pa1;
//}

////	------------------------
//MINT MGeo::PerpLnPt2(						// (  O) ステイタス(=MC_ON_LINE	:線分上,
//												//					=0			:線分範囲外)
//				const	MgLine2&	Ln1,		// (I  ) 線分1
//				const	MgPoint2&	p2,			// (I  ) 点2
//						MgPoint2*	po			// (  O) 点から直線への垂線の足
//		)
//{
//	MINT		ist;
//	MgPoint2		vd1, vd2;
//	MREAL		ss, se;
//
//	vd1 = Ln1.p[1] - Ln1.p[0];
//	vd2 = MgRotL90(vd1);
//
//	MgSVal(Ln1.p[0], Ln1.p[1], p2, vd2, &ss, &se);
//
//	*po = Ln1.p[0] + (vd1 * (ss / (ss - se)));					//	交点
//
//	if (ss * se < 0 ||											//	線分上
//		*po == Ln1.p[0] || *po == Ln1.p[1]) ist = MC_ON_LINE;
//	else ist =  0;												//	線分の外
//	return ist;
//}

//	------------------------
/*
void MGeo::PerpPtULn2(
				const	MgPoint2&	p1,			// (I  ) 点1
				const	MgULine2&	ULn2,		// (I  ) 直線2
						MgPoint2*	po			// (  O) ２次元交点(点の標準形式)
		)
{
	MgPerpULnPt2(ULn2, p1, po);
}
//	------------------------
MINT MGeo::PerpPtLn2(							// (  O) ステイタス(=MC_ON_LINE	:線分上,
												//					=0			:線分範囲外)
				const	MgPoint2&	p1,			// (I  ) 点1
				const	MgLine2		&Ln2,		// (I  ) 線分2
						MgPoint2*	po			// (  O) ２次元交点(点の標準形式)
		)
{
	return MgPerpLnPt2(Ln2, p1, po);
}
*/

////
////	-------------------------３ＤＬＩＮＥ・ＰＯＩＮＴ----------------------
////
////	点から直線への垂線の足位置を求める
////
//void MGeo::PerpULnPt3(
//				const	MgULine3&	ULn1,		// (I  ) 直線1
//				const	MgPoint3&	p2,			// (I  ) 点2
//						MgPoint3	*po			// (  O) 点から直線への垂線の足
//		)
//{
//	MREAL	pa1;								//	直線1の始点から垂線の足までの距離
//
//	pa1 = (p2 - ULn1.p) * ULn1.v;		//	cosΘv12v2
//	*po = ULn1.p + ULn1.v * pa1;
//}

////	------------------------
//MINT MGeo::PerpLnPt3(						// (  O) ステイタス(=MC_ON_LINE	:線分上,
//												//					=0			:線分範囲外)
//				const	MgLine3		&Ln1,		// (I  ) 線分1
//				const	MgPoint3&	p2,			// (I  ) 点2
//				MgPoint3		*po				// (  O) ３次元交点(点の標準形式)
//		)
//{
//	MINT		ist;
//	MgPoint3	vd1, vd2;
//	MREAL		ss, se;
//	MREAL		d;
//
//	vd1 = Ln1.p[1] - Ln1.p[0];
//
//	d  = p2 * vd1;
//	ss = (Ln1.p[0] * vd1) - d;
//	se = (Ln1.p[1] * vd1) - d;
//
//	*po = Ln1.p[0] + (vd1 * (ss / (ss - se)));					//	交点
//
//	if (ss * se < 0 ||											//	線分上
//		*po == Ln1.p[0] || *po == Ln1.p[1]) ist = MC_ON_LINE;
//	else ist = 0;												//	線分の外
//
//	return ist;
//}

//	------------------------
/*
void MGeo::PerpPtLn3(
				const	MgPoint3&	p1,			// (I  ) 点1
				const	MgLine3		&Ln2,		// (I  ) 直線2
						MgPoint3	*po			// (  O) ３次元交点(点の標準形式)
		)
{
	MgPerpLnPt3(Ln2,	p1, po);
}

//	------------------------
MINT MGeo::PerpPtULn3(						// (  O) ステイタス(=MC_ON_LINE	:線分上,
												//					=0			:線分範囲外)
				const MgPoint3& p1,				// (I  ) 点1
				const MgULine3& ULn2,			// (I  ) 線分2
				MgPoint3	*po					// (  O) ３次元交点(点の標準形式)
		)
{
	return MgPerpULnPt3(ULn2, p1, po);
}
*/

////
////	-------------------------３Ｄ面・ＰＯＩＮＴ----------------------
////
////	点から面への垂線の足位置を求める
////
//void MGeo::PerpPlnPt3(
//				const	MgPlane3	&Pln1,		// (I  ) 面1
//				const	MgPoint3&	p2,			// (I  ) 点2
//						MgPoint3	*po			// (  O) 点から直線への垂線の足
//		)
//{
//	MREAL	pa1;								//	点2から垂線の足までの距離
//
//	pa1 = (p2 * Pln1.v) + Pln1.d;
//	*po = p2 - Pln1.v * pa1;
//}

//	------------------------
/*
void MGeo::PerpPtPln3(
				const	MgPoint3&	p1,			// (I  ) 点1
				const	MgPlane3	&Pln2,		// (I  ) 面2
						MgPoint3	*po			// (  O) ３次元交点(点の標準形式)
		)
{
	MgPerpPlnPt3(Pln2, p1, po);
}
*/

} // namespace MC
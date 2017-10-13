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

#include "MgLib.h"

namespace MC
{

/*        inline化
//
//===========================================================================
//		点の移動　形状演算
//
// ---------------------( ２次元 )------------------------------
//
void MGeo::MovePt2InDirOfVt2(					//
				const	MgPoint2&	p1,			//  (I  ) 移動点1
				const	MgVect2&	v2,			//	(I  ) 移動方向を示すベクトル
						MREAL		dl,			//	(I  ) 移動距離
						MgPoint2	*p3			//	(  O) 移動された点3
		)
{
	*p3 = p1 + dl * MGeo::UnitizeV2(v2);
}
//	------------------------
void MGeo::MovePt2AlongULn2(					//
				const	MgPoint2&	p1,			//  (I  ) 移動点1
				const	MgULine2&	ULn2,		//	(I  ) 移動方向を示す直線2
						MREAL		dl,			//	(I  ) 移動距離
						MgPoint2	*p3			//	(  O) 移動された点3
		)
{
	*p3 = p1 + dl * ULn2.v;
}
//	------------------------
void MGeo::MovePt2AlongLn2(					//
				const	MgPoint2&	p1,			//  (I  ) 移動点1
				const	MgLine2&	Ln2,		//	(I  ) 移動方向を示す線分2
						MREAL		dl,			//	(I  ) 移動距離
						MgPoint2	*p3			//	(  O) 移動された点3
		)
{
	MgVect2	vd, vn;
//
	vd = Ln2.p[1] - Ln2.p[0];
	vn = MGeo::UnitizeV2(vd);
	*p3 = p1 + dl * vn;
}
//
// ---------------------( ３次元 )------------------------------
//
void MGeo::MovePt3InDirOfVt3(					//
				const	MgPoint3&	p1,			//  (I  ) 移動点1
				const	MgVect3&	v2,			//	(I  ) 移動方向を示すベクトル
						MREAL		dl,			//	(I  ) 移動距離
						MgPoint3	*p3			//	(  O) 移動された点3
		)
{
	*p3 = p1 + dl * MgUnitizeV3(v2);
}
//	------------------------
void MGeo::MovePt3AlongULn3(					//	(  O) ステイタス
				const	MgPoint3&	p1,			//  (I  ) 移動点1
				const	MgULine3&	ULn2,		//	(I  ) 移動方向を示す直線2
						MREAL		dl,			//	(I  ) 移動距離
						MgPoint3	*p3			//	(  O) 移動された点3
		)
{
	*p3 = p1 + dl * ULn2.v;
}
//	------------------------
void MGeo::MovePt3AlongLn3(					//	(  O) ステイタス
				const	MgPoint3&	p1,			//  (I  ) 移動点1
				const	MgLine3		&Ln2,		//	(I  ) 移動方向を示す線分2
						MREAL		dl,			//	(I  ) 移動距離
						MgPoint3	*p3			//	(  O) 移動された点3
		)
{
	MgVect3	vd, vn;
//
	vd = Ln2.p[1] - Ln2.p[0];
	vn = MgUnitizeV3(vd);
	*p3 = p1 + dl * vn;
}
//===========================================================================
//		直線のオフセット　形状演算
//
// ---------------------( ２次元 )------------------------------
//
//	直線をオフセットする。
//
void MGeo::OffsetULn2(						//	(  O) ステイタス (=1)
				const	MgULine2&	ULn1,		//	(I  ) 直線1
						MREAL		dl,			//	(I  ) オフセット量
						MgULine2 	*ULn2		//	(  O) オフセットされた直線2
		)
{

	ULn2->p = ULn1.p + dl * MgRotL90(ULn1.v);
	ULn2->v = ULn1.v;
}
//	------------------------
void MGeo::OffsetLn2(							//	(  O) ステイタス
				const	MgLine2&	Ln1,		//	(I  ) 線分1
						MREAL		dl,			//	(I  ) オフセット量
						MgLine2		*Ln2		//	(  O) オフセットされた線分2
		)
{
	MgVect2	vd, vn;
//
	vd = Ln1.p[1] - Ln1.p[0];
	vn = MGeo::UnitizeV2(vd);
	Ln2->p[0] = Ln1.p[0] + dl * MgRotL90(vn);
	Ln2->p[1] = Ln2->p[0] + vd;
}
//
// ---------------------( ３次元 )------------------------------
//
void MGeo::OffsetULn3(						//	(  O) ステイタス
				const	MgULine3&	ULn1,		//	(I  ) 直線1
				const	MgPlane3	&Pln2,		//	(I  ) 直線1が乗る平面2
						MREAL		dl,			//	(I  ) オフセット量
						MgULine3	*ULn3		//	(  O) オフセットされた直線3
		)
{
	ULn3->p = ULn1.p + dl * (Pln2.v ^ ULn1.v);
	ULn3->v = ULn1.v;
}
//	------------------------
void MGeo::OffsetLn3(							//	(  O) ステイタス
				const	MgLine3		&Ln1,		//	(I  ) 線分1
				const	MgPlane3	&Pln2,		//	(I  ) 線分1が乗る平面2
						MREAL		dl,			//	(I  ) オフセット量
						MgLine3		*Ln3		//	(  O) オフセットされた線分3
		)
{
	MgVect3	vd, vn;
//
	vd = Ln1.p[1] - Ln1.p[0];
	vn = MgUnitizeV3(vd);
	MgVect3 vv = vn ^ Pln2.v;
	Ln3->p[0] = Ln1.p[0] + dl * (Pln2.v ^ vn);
	Ln3->p[1] = Ln3->p[0] + vd;
}
*/

} // namespace MC
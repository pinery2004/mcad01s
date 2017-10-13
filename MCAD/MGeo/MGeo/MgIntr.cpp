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
//		直線と直線の交点　形状演算
//
// ---------------------( ２次元 )------------------------------
//　２本の直線の交点を求める。
//
MINT MGeo::Intr2ULn2(							// (  O) ステイタス
												//	MC_PARALLEL (-1) 交差なし（平行）
												//	MC_NINT	    (0)	交差なし
												//	MC_INT      (1)	交差あり
				const	MgULine2&	ULn1,		// (I  ) 直線1
				const	MgULine2&	ULn2,		// (I  ) 直線2
						MgPoint2*	po			// (  O) ２次元交点
				)
{
	MINT	ist;
	MREAL	so;									// 単位ベクトルの外積
	MREAL	pa1;								// 直線1の始点から交点までの距離
//
//	平行のチェック
//
	so = ULn1.v ^ ULn2.v;										// sinΘv1v2
	if ( Zero( so, g_gTol.A)) {
		ist = MC_PARALLEL;										// 平行
	} else {
		pa1 = (ULn2.p - ULn1.p) ^ ULn2.v / so;					// sinΘv12v2 / sinΘv1v2
		*po = ULn1.p + pa1 * ULn1.v;
		ist = MC_INT;
	}
	return ist;
}

// ---------------------( ２次元 )------------------------------
//　直線と線分の交点を求める。
//　重なっている場合は、交差なし（平行）とみなす。
//
MINT MGeo::IntrULnLn2(						// (  O) ステイタス
												//	MC_PARALLEL (-1) 交差なし（平行）
												//	MC_NINT	    (0)	交差なし
												//	MC_INT      (1)	交差あり
				const	MgULine2&	ULn1,		// (I  ) 直線1
				const	MgLine2&	Ln2,		// (I  ) 線分2
						MgPoint2*	po			// (  O) ２次元交点
				)
{
	MINT	ist;
	MgVect2	vd2;
	MREAL	ss, se;
//
//　平行のチェック
//
	vd2 = Ln2.p[1] - Ln2.p[0];
	if (MGeo::Parallel(ULn1.v, vd2)) {
		ist = MC_PARALLEL;										// 平行
	} else {
//
		SVal(Ln2.p[0], Ln2.p[1], ULn1.p, ULn1.v, &ss, &se);
//
		*po = Ln2.p[0] + (vd2 * (ss / (ss - se)));				// 交点
//
		if (ss * se < 0 || *po == Ln2.p[0]						// 線分上
						|| *po == Ln2.p[1]) {
			ist = MC_INT;
		} else {
			ist = MC_NINT;										// 線分の外
		}
	}
	return ist;
}

// ---------------------( ２次元 )------------------------------
//　線分と直線の交点を求める。
//　重なっている場合は、交差なし（平行）とみなす。
//
//					<< コーディング実体はMgLib.hにあり >>
//
// MINT	MgIntrLnULn2(							// (  O) ステイタス
//												//	MC_PARALLEL (-1) 交差なし（平行）
//												//	MC_NINT	    (0)	交差なし
//												//	MC_INT      (1)	交差あり
//				const	MgLine2&	Ln1,		// (I  ) 線分1
//				const	MgULine2&	ULn2,		// (I  ) 直線2
//						MgPoint2*	po			// (  O) ２次元交点
//				)
//{
//	return MgIntr(ULn2, Ln1, po);
// }
// ---------------------( ２次元 )------------------------------
//　線分と線分の交点を求める。
//　２線分が重なっている場合は、交差なし（平行）とみなす。
//  ２線分が重なりなく直線的に端部で接合している場合はその接合点を交点とみなす。
MINT MGeo::Intr2Ln2(							// (  O) ステイタス
												//	MC_PARALLEL (-1) 交差なし（平行）
												//  MC_CONNECTION(4) 接続	 （平行）
												//	MC_NINT	    (0)	交差なし
												//	MC_INT      (1)	交差あり
				const	MgLine2&	Ln1,		// (I  ) 線分1
				const	MgLine2&	Ln2,		// (I  ) 線分2
				MgPoint2			*po			// (  O) ２次元交点
				)
{
	MINT		ist;
	MREAL		so, si;
	MgPoint2	vd1, vd2;
	MREAL		ss1, se1;
	MREAL		ss2, se2;
//
//	平行のチェック
//
	vd1 = Ln1.p[1] - Ln1.p[0];
	vd2 = Ln2.p[1] - Ln2.p[0];
	if (MGeo::Parallel(vd1, vd2, &so, &si)) {
		if (Ln1.p[0] == Ln2.p[0] && si < 0 ||
			Ln1.p[0] == Ln2.p[1] && si > 0) {
			*po = Ln1.p[0];
			ist = MC_CONNECTION;								// 平行　線分1の始点と線分2の端点が近点
		} else if (Ln1.p[1] == Ln2.p[0] && si > 0 ||
				   Ln1.p[1] == Ln2.p[1] && si < 0) {
			*po = Ln1.p[1];
			ist = MC_CONNECTION;								// 平行　線分1の終点と線分2の端点が近点
		} else {
			ist = MC_PARALLEL;									// 平行
		}
	} else {
		SVal(Ln1.p[0], Ln1.p[1], Ln2.p[0], vd2, &ss1, &se1);
		SVal(Ln2.p[0], Ln2.p[1], Ln1.p[0], vd1, &ss2, &se2);
//
		*po = Ln1.p[0] + (vd1 * (ss1 / (ss1 - se1)));			// 交点
//
		if ((ss1 * se1 < 0 ||									// 線分上
			 *po == Ln1.p[0] || *po == Ln1.p[1]) &&
			(ss2 * se2 < 0 ||
			 *po == Ln2.p[0] || *po == Ln2.p[1])) {
			ist = MC_INT;
		} else {
			ist = MC_NINT;										// 線分の外
		}
	}
	return ist;
}

// ---------------------( ２次元 )------------------------------
//	線分１と直線２との交点を求め、点群に追加する。
//　重なっている場合は、交差なし（平行）とみなす。
//
MINT MGeo::IntrAddLnULn2(						// (  O) ステイタス
												//	MC_PARALLEL (-1) 交差なし（平行）
												//	MC_NINT	    (0)	交差なし
												//	MC_INT      (1)	交差あり
				const	MgLine2&	Ln1,		// (I  ) 線分1
				const	MgULine2&	ULn2,		// (I  ) 直線2
						MgGPoint2	*GPt3		// (I O) 交点を追加する点群
												//		 追加交点数(0:交差なし、線分の端部で交差、または重なる
												//					1:交差あり（線分の端部での交差は含まず))
				)
{
	MINT		ist;
	MgPoint2	vd1, vd2;
	MgPoint2	po;
	MREAL		ss, se;
//
//	平行のチェック
//
	vd1 = Ln1.p[1] - Ln1.p[0];
	if (MGeo::Parallel(vd1, ULn2.v)) {								// 平行
		ist = MC_PARALLEL;
	} else {
		SVal(Ln1.p[0], Ln1.p[1], ULn2.p, ULn2.v, &ss, &se);
		if (ss * se < 0) {										// 交点は線分１の内側
			po = Ln1.p[0] + (vd1 * (ss / (ss - se)));			// 交点
			if (po != Ln1.p[0] && po != Ln1.p[1]) {				// 線分１の端部は交差点と見なさない
				(*GPt3) += po;									// 交点あり
				ist = MC_INT;
				MQUIT;
			}
		}
		ist = MC_NINT;											// 交点なし
	}
exit:
	return ist;
}

// ---------------------( ２次元 )------------------------------
//	線分１と線分２との交点と重なり部分の端点を求め、点群に追加する。
//  交点または重なり部分の端点が線分１の端点である場合は、その点を対象外とする。
//
MINT MGeo::IntrAdd2Ln2(						// (  O) ステイタス
												//	MC_PARALLEL (-1) 交差なし（平行）
												//	MC_NINT	    (0)	交差なし
												//	MC_INT      (1)	交差あり
												//  MC_REP	    (2)	交差あり（重なり）
				const	MgLine2&	Ln1,		// (I  ) 線分1
				const	MgLine2&	Ln2,		// (I  ) 線分2
						MgGPoint2	*GPt3		// (I O) 交点を追加する線分群
												// 		 追加交点数(0:交差無しまたは線分1が線分2の内側にある重なり(端部同士の重なり含む) |
												//					1:線分2と交差（線分2の端部での交差も含む）または一部重なり(片端重なり含む) |
												//					2:線分2が線分1の内側にある重なり(端部同士の重なりなし)) 
				)
{
	MINT		ist;
	MINT		ist1;
	MgPoint2	vd1, vd2;
	MgPoint2	po;
	MREAL		ss1, se1;
	MREAL		ss2, se2;
//	MINT		ist = 0;
//
//	平行のチェック
//
	vd1 = Ln1.p[1] - Ln1.p[0];
	vd2 = Ln2.p[1] - Ln2.p[0];
	if (MGeo::Parallel(vd1, vd2)) {

		MINT	nb = GPt3->m_n;
		ChkPt2OnLn2WS( Ln2.p[0], Ln1, &ist1);
		if ( MF_CHECK_OR(ist1, MC_INSIDE)) (*GPt3) += Ln2.p[0];		// 線分２の始点が線分１内
		ChkPt2OnLn2WS( Ln2.p[1], Ln1, &ist1);
		if ( MF_CHECK_OR(ist1, MC_INSIDE)) (*GPt3) += Ln2.p[1];		// 線分２の終点が線分１内
		if ( GPt3->m_n == nb) {
			ist = MC_PARALLEL;
		} else {
			ist = MC_REP;
		}
	} else {
//
		SVal(Ln1.p[0], Ln1.p[1], Ln2.p[0], vd2, &ss1, &se1);
		SVal(Ln2.p[0], Ln2.p[1], Ln1.p[0], vd1, &ss2, &se2);
//
		po = Ln1.p[0] + (vd1 * (ss1 / (ss1 - se1)));			// 交点
//
		if ((ss2 * se2 < 0 || po == Ln2.p[0] || po == Ln2.p[1]) &&
			 ss1 * se1 < 0 && po != Ln1.p[0] && po != Ln1.p[1]) {
			(*GPt3) += po;										// 交点が線分１および線分２上で線分1の端点でない
			ist = MC_INT;
		} else {
			ist = MC_NINT;										// 線分１または線分２の外または線分1の端点
		}
	}
	return ist;
}

// ---------------------( ２次元 )------------------------------
//	直線１と線分２との交点または重なり部分の端点を求め、点群に追加する。
//
MINT MGeo::IntrAddULnLn2(						// (  O) ステイタス
												//	MC_PARALLEL (-1) 交差なし（平行）
												//	MC_NINT	    (0)	交差なし
												//	MC_INT      (1)	交差あり
												//  MC_REP	    (2)	交差あり（重なり）
				const	MgULine2&	ULn1,		// (I  ) 直線1
				const	MgLine2&	Ln2,		// (I  ) 線分2
						MgGPoint2	*GPt3		// (I O) 交点を追加する線分群
												// 		 追加交点数(0:交差無し |
												//					1:直線1と線分2が交差（線分2の端部での交差も含む）|
												//					2:直線1と線分2が重なる 
				)
{
	MINT		ist;
	MINT		ist1;
	MgPoint2	vd1, vd2;
	MgPoint2	po;
	MREAL		ss, se;
//	MINT	ist = 0;
//
//	平行のチェック
//
	vd2 = Ln2.p[1] - Ln2.p[0];
	if (MGeo::Parallel(ULn1.v, vd2)) {

		if ( ChkPt2OnULn2WS( Ln2.p[0], ULn1, &ist1)) {
			(*GPt3) += Ln2.p[0];								// 線分２の始点が直線１上
			(*GPt3) += Ln2.p[1];								// (線分２の終点も線分１上)
			ist = MC_REP;
		} else {
			ist = MC_PARALLEL;
		}
	} else {
//
		SVal(Ln2.p[0], Ln2.p[1], ULn1.p, ULn1.v, &ss, &se);
//
		po = Ln2.p[0] + (vd2 * (ss / (ss - se)));				// 交点
//
		if (ss * se < 0 || po == Ln2.p[0]						// 線分２上
						|| po == Ln2.p[1]) {
			(*GPt3) += po;
			ist = MC_INT;
		} else {
			ist = MC_NINT;										// 線分２の外
		}
	}
	return ist;
}

// ---------------------( ３次元 )------------------------------
//	２本の直線の交点を求める。
//
MINT MGeo::Intr2ULn3(							// (  O) ステイタス
												//	MC_PARALLEL (-1) 交差なし（平行）
												//	MC_TWIST	(-2) 交差なし（ねじれ）
												//	MC_INT      (1)	交差あり
				const	MgULine3&	ULn1,		// (I  ) 直線1
				const	MgULine3&	ULn2,		// (I  ) 直線2
						MgPoint3	*po			// (  O) ３次元交点
				)
{
	MINT	ist;
	MgVect3	sp;									//	単位ベクトルの外積
	MgVect3	V12;								//	直線1の始点から直線2の始点へのベクトル
	MREAL	s12_2;								//	単位ベクトルの外積**2
	MREAL	pa1;								//	直線1の始点から交点までの距離
	MREAL	Hab12;								//	単位ベクトル2本からなる平行６面体の体積
//	MREAL	L12_2;								//	両直線の始点間の距離の２乗
//
//	平行のチェック
//
	sp = ULn1.v ^ ULn2.v;										// Vect(sinΘv1v2)
	s12_2 = sp * sp;											// (sinΘv1v2)**2
	if (s12_2 < g_gTol.A_2) {
		ist = MC_PARALLEL;										// 平行
	} else {
		V12 = ULn2.p - ULn1.p;									// 直線1の始点から直線2の始点へのベクトル	
//
		Hab12 = SVal2ULn3(ULn1, ULn2);								// 平行４平面体の体積
		if ( Hab12*Hab12 > g_gTol.S_2*s12_2) {
			ist = MC_TWIST;										// 交差なし（ねじれ状態） 
		} else {
			pa1 = ((V12 ^ ULn2.v) * sp) / s12_2;				// sinΘv12v2 / sinΘv1v2
			*po = ULn1.p + pa1 * ULn1.v;
			ist = MC_INT;
		}
	}
	return ist;
}

// ---------------------( ３次元 )------------------------------
//　直線と線分の交点を求める。
//
MINT MGeo::IntrULnLn3(						// (  O) ステイタス
												//	MC_PARALLEL (-1) 交差なし（平行）
												//	MC_TWIST    (-2) 交差なし（ねじれ）
												//	MC_NINT	    (0)	交差なし
												//	MC_INT      (1)	交差あり
				const	MgULine3&	ULn1,		// (I  ) 直線1
				const	MgLine3		&Ln2,		// (I  ) 線分2
						MgPoint3	*po			// (  O) ３次元交点
				)
{
	MINT	ist;
	MgVect3	vd2;								//	線分２の始点から終点へのベクトル
	MgVect3	vso;								//	直線と線分の方向ベクトルの外積
	MREAL	si;									//	直線と線分の方向ベクトルの内積
//
	MgVect3	V12;								//	直線1の始点から直線2の始点へのベクトル
	MREAL	s12_2;								//	直線と線分の方向ベクトルの外積**2
	MREAL	Hab12;								//	単位ベクトル2本からなる平行６面体の体積
//
//	平行のチェック
//
	vd2 = Ln2.p[1] - Ln2.p[0];
//
	if (MGeo::Parallel(ULn1.v, vd2, &vso, &si)) {
		ist = MC_PARALLEL;										//	平行
	} else {
//
		V12 = Ln2.p[0] - ULn1.p;								// 直線1の始点から線分2の始点へのベクトル	
//
		s12_2 = vso * vso;
		Hab12 = SValULnLn3(ULn1, Ln2);							// 平行４平面体の体積
		if (Hab12*Hab12 > g_gTol.S_2*s12_2) {
			ist = MC_TWIST;		//	交差なし（ねじれ状態） 
		} else {
//
			MREAL		ss;										// 線分２の始点側Ｓバリュー
			MREAL		se;										// 線分２の終点側Ｓバリュー
//
			SValLnULnS3(Ln2, ULn1, vso, &ss, &se);
//
			*po = Ln2.p[0] + (vd2 * (ss / (ss - se)));			// 交点
//
			if (ss * se < 0 || *po == Ln2.p[0]					// 線分上
							|| *po == Ln2.p[1]) {
				ist = MC_INT;
			} else {
				ist = MC_NINT;									// 線分外
			}
		}
	}
	return ist;
}

// ---------------------( ３次元 )------------------------------
//　線分と直線の交点を求める。
//
//
//					<< コーディング実体はMgLib.hにあり >>
//
// MINT	MGeo::IntrLnULn3(							// (  O) ステイタス
//												//	MC_PARALLEL (-1) 交差なし（平行）
//												//	MC_TWIST    (-2) 交差なし（ねじれ）
//												//	MC_NINT	    (0)	交差なし
//												//	MC_INT      (1)	交差あり
//				const	MgLine3		&Ln1,		// (I  ) 線分1
//				const	MgULine3&	ULn2,		// (I  ) 直線2
//						MgPoint3	*po			// (  O) ３次元交点
//				)
// {
//	return MgIntrULnLn3(ULn2, Ln1, po);
// }
// ---------------------( ３次元 )------------------------------
//　線分と線分の交点を求める。
//　２線分が重なっている場合は、交差なし（平行）とみなす。
//  ２線分が重なりなく直線的に端部で接合している場合はその接合点を交点とみなす。
//
MINT MGeo::Intr2Ln3(							// (  O) ステイタス
												//	MC_PARALLEL (-1) 交差なし（平行）
												//  MC_CONNECTION(4) 接続	 （平行）
												//	MC_TWIST    (-2) 交差なし（ねじれ）
												//	MC_NINT	    (0)	交差なし
												//	MC_INT      (1)	交差あり
				const	MgLine3		&Ln1,		// (I  ) 線分1
				const	MgLine3		&Ln2,		// (I  ) 線分2
						MgPoint3	*po			// (  O) ３次元交点
				)
{
	MINT	ist;
	MgVect3	vso;
	MREAL	si;
	MgVect3	vd1,vd2;
	MREAL	s12_2;								//	直線と線分の方向ベクトルの外積**2
	MREAL	Hab12;								//	単位ベクトル2本からなる平行６面体の体積
	MREAL	ss1, se1;
	MREAL	ss2, se2;
//
//	平行のチェック
//
	vd1 = Ln1.p[1] - Ln1.p[0];
	vd2 = Ln2.p[1] - Ln2.p[0];
	if (MGeo::Parallel(vd1, vd2, &vso, &si)) {
		if (Ln1.p[0] == Ln2.p[0] && si < 0 ||
			Ln1.p[0] == Ln2.p[1] && si > 0) {
			*po = Ln1.p[0];
			ist = MC_CONNECTION;								// 平行　線分1の始点と線分2の端点が近点
		} else if (Ln1.p[1] == Ln2.p[0] && si > 0 ||
				   Ln1.p[1] == Ln2.p[1] && si < 0) {
			*po = Ln1.p[1];
			ist = MC_CONNECTION;								// 平行　線分1の終点と線分2の端点が近点
		} else {
			ist = MC_PARALLEL;									// 平行
		}
	} else {
//
		s12_2 = vso * vso;
		Hab12 = SVal2Ln3(Ln1, Ln2);								// 平行４平面体の体積
		if ( Hab12*Hab12 > g_gTol.S_2*s12_2) {
			ist = MC_TWIST;		//	交差なし（ねじれ状態） 
		} else {
//
			SVal2LnS3(Ln1, Ln2, vso, &ss1, &se1);
			SVal2LnS3(Ln2, Ln1, vso, &ss2, &se2);
//
			*po = Ln1.p[0] + (vd1 * (ss1 / (ss1 - se1)));		// 交点
//
			if ((ss1 * se1 < 0 ||			//	線分上
				 *po == Ln1.p[0] || *po == Ln1.p[1]) &&
				(ss2 * se2 < 0 ||
				 *po == Ln2.p[0] || *po == Ln2.p[1])) {
				ist = MC_INT;
			} else {
				ist = MC_NINT;									// 線分外
			}
		}
	}
	return ist;
}

//===========================================================================
//		平面と直線の交点　形状演算
//
// ---------------------( ３次元 )------------------------------
//
//	平面と直線の交点を求める。
//
MINT MGeo::IntrPlnULn3(						// (  O) ステイタス
												//	MC_PARALLEL (-1) 交差なし（平行）
												//	MC_INT      (1)	交差あり
				const	MgPlane3	&Pln1,		// (I  ) 平面1
				const	MgULine3&	ULn2,		// (I  ) 直線2
						MgPoint3	*po			// (  O) ３次元交点(点の標準形式)
				)
{
	MINT	ist;
	MREAL	c12;								//	単位ベクトルの内積
	MREAL	ss;									//	直線2の始点側Ｓバリュー
	MREAL	pa1;								//	直線2の始点から交点までの距離
//
//	平行のチェック
//
	c12 = Pln1.v * ULn2.v;										// (cosΘv1v2)
	if ( Zero( c12, g_gTol.A)) {
		ist = MC_PARALLEL;										// 平行
	} else {
//
		ss = (Pln1.v * ULn2.p) + Pln1.d;
		pa1 = - ss / c12;										// ss / (ss - se) [se = ss + c12]
		*po = ULn2.p + ULn2.v * pa1;
//
		ist = MC_INT;
	}
	return ist;
}

// ---------------------( ３次元 )------------------------------
//	平面と線分の交点を求める。
//
MINT MGeo::IntrPlnLn3(						// (  O) ステイタス
												//	MC_PARALLEL (-1) 交差なし（平行）
												//	MC_NINT	    (0)	交差なし
												//	MC_INT      (1)	交差あり
				const	MgPlane3	&Pln1,		// (I  ) 平面1
				const	MgLine3		&Ln2,		// (I  ) 線分2
						MgPoint3	*po			// (  O) ３次元交点(点の標準形式)
				)
{
	MINT	ist;
	MgVect3	vso;
	MREAL	si;
	MgVect3	vd2;								//	線分2のベクトル
	MREAL	ss;									//	線分２の始点側Ｓバリュー
	MREAL	se;									//	線分２の終点側Ｓバリュー
//
	MREAL	pa1;								//	直線2の始点から交点までの距離
//
//	平行のチェック
//
	vd2 = Ln2.p[1] - Ln2.p[0];
	if (MGeo::Parallel(Pln1.v, vd2, &vso, &si)) {
		ist = MC_PARALLEL;	//	平行
	} else {
//
		ss = (Pln1.v * Ln2.p[0]) + Pln1.d;
		se = (Pln1.v * Ln2.p[1]) + Pln1.d;
		pa1 = ss / (ss - se);
		*po = Ln2.p[0] + vd2 * pa1;
//
		if (ss * se < 0 || *po == Ln2.p[0]			 			// 線分上
						|| *po == Ln2.p[1]) {
			ist = MC_INT;
		} else {
			ist = MC_NINT;										// 線分外
		}
	}
	return ist;
}

// ---------------------( ３次元 )------------------------------
//	直線と平面の交点を求める。
/*
MINT MGeo::IntrULnPln3(						// (  O) ステイタス
												//	MC_PARALLEL (-1) 交差なし（平行）
												//	MC_INT      (1)	交差あり
				const	MgULine3&	ULn1,		// (I  ) 直線1
				const	MgPlane3	&Pln2,		// (I  ) 平面2
						MgPoint3	*po			// (  O) ３次元交点(点の標準形式)
				)
{
	return MgIntrPlnULn3(Pln2, ULn1, po);
}
*/
// ---------------------( ３次元 )------------------------------
//	線分と平面の交点を求める。
/*
MINT MGeo::IntrLnPln3(						// (  O) ステイタス
												//	MC_PARALLEL (-1) 交差なし（平行）
												//	MC_NINT	    (0)	交差なし
												//	MC_INT      (1)	交差あり
				const	MgLine3		&Ln1,		// (I  ) 線分1
				const	MgPlane3	&Pln2,		// (I  ) 平面2
						MgPoint3	*po			// (  O) ３次元交点(点の標準形式)
				)
{
	return MgIntrPlnLn3(Pln2, Ln1, po);
}
*/
//===========================================================================
//		平面と平面の交線　形状演算
//
// ---------------------( ３次元 )------------------------------
//
//	平面と平面の交線を求める。
//
MINT MGeo::Intr2Pln3(							// (  O) ステイタス
												//	MC_PARALLEL (-1) 交差なし（平行）
												//	MC_INT      (1)	交差あり
				const	MgPlane3	&Pln1,		// (I  ) 平面1
				const	MgPlane3	&Pln2,		// (I  ) 平面2
						MgULine3	*ULn3		// (  O) 直線2
				)
{
	MINT	ist;
	MgVect3 vso;
	MREAL	s12;								//	単位ベクトルの内積
//
//	平行のチェック
//
	vso = Pln1.v ^ Pln2.v;										// (sinΘv1v2)
	s12 = Abs_2(vso);
	if ( Zero(s12, g_gTol.A_2)) {
		ist = MC_NINT;											// 平行
	} else {
//
		if ( !Zero( vso.z, 0.5)) {
			ULn3->p.x = ( Pln2.d * Pln1.v.y - Pln1.d * Pln2.v.y) / vso.z;
			ULn3->p.y = ( Pln1.d * Pln2.v.x - Pln2.d * Pln1.v.x) / vso.z;
			ULn3->p.z = 0.0f;
		} else if ( !Zero(vso.y, 0.5)) {
			ULn3->p.x = ( Pln1.d * Pln2.v.z - Pln2.d * Pln1.v.z) / vso.y;
			ULn3->p.y = 0.0f;
			ULn3->p.z = ( Pln2.d * Pln1.v.x - Pln1.d * Pln2.v.x) / vso.y;
		} else {
			ULn3->p.x = 0.0f;
			ULn3->p.y = ( Pln2.d * Pln1.v.z - Pln1.d * Pln2.v.z) / vso.x;
			ULn3->p.z = ( Pln1.d * Pln2.v.y - Pln2.d * Pln1.v.y) / vso.x;
		}
		ULn3->v = vso.Unitize();
		ist = MC_INT;
	}
	return ist;
}

} // namespace MC
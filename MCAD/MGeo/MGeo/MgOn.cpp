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

////
////===========================================================================
////		点　形状演算
////
//// ---------------------( ２次元 )------------------------------
////
////	点が点上にあるかを調べる。
////
//bool MGeo::On(									// (  O) ステイタス
//												//			true:	点上にある
//												//			false:	点上にない
//				const	MgPoint2&	p1,			// (I  ) 点1
//				const	MgPoint2&	p2,			// (I  ) 点2
//						MREAL		rTol		// (I  ) トレランス
//				)
//{
////	return (p1 == p2);
//	MgVect2	V1 = p2 - p1;
//	return (MgAbs_2( V1) <= rTol * rTol);
//}

//// ---------------------( ３次元 )------------------------------
////
////	点が点上にあるかを調べる。
////
//bool MGeo::ChkPt3OnPt3(							// (  O) ステイタス
//												//			true:	点上にある
//												//			false:	点上にない
//				const	MgPoint3&	p1,			// (I  ) 点1
//				const	MgPoint3&	p2,			// (I  ) 点2
//						MREAL		rTol		// (I  ) トレランス
//				)
//{
////	return (p1 == p2);
//	MgVect3	V1 = p2 - p1;
//	return (MgAbs_2( V1) <= rTol * rTol);
//}

////===========================================================================
////		直線　形状演算
////
//// ---------------------( ２次元 )------------------------------
////
////	点が直線上にあるかを調べる。
////
//bool MGeo::ChkPt2OnULn2WS(						// (  O) ステイタス
//												//		 	true:	直線上にある
//												//		 	false:	直線上にない
//				const	MgPoint2&	p1,			// (I  ) 点1
//				const	MgULine2&	ULn2,		// (I  ) 直線2
//						MINT*		ist,		// (  O) 補助ステイタス
//												//			MC_RIGHT			(010): 点が直線の右側
//												//			MC_ON_LINE			(020): 点が直線上
//												//			MC_LEFT				(040): 点が直線の左側
//						MREAL		rTol		// (I  ) トレランス
//		)
//{
//	bool		bst;							// ステイタス
//	MgVect2		v2s1;							// 直線2の始点から点1までのベクトル
//	MREAL		pa1;							// 直線2から点1までの鉛直距離
////
//// 直線2の方向（単位ベクトル）と直線2の始点から点1までのベクトルの外積より鉛直距離を求める
//	v2s1 = p1 - ULn2.p;					
//	pa1 = ULn2.v ^ v2s1;						// 直線2の方向（単位ベクトル）と直線2の始点から点1までのベクトルの外積
////
//// 鉛直距離が誤差内なら直線上とみなす
//	if (MgZero(pa1, rTol)) {
//		*ist = MC_ON_LINE;
//		bst = true;
////
//// 鉛直距離が正なら直線の左とみなす
//	} else if (pa1 > 0) {
//		*ist = MC_LEFT;
//		bst = false;
////
//// 鉛直距離が負なら直線の右とみなす
//	} else {
//		*ist = MC_RIGHT;
//		bst = false;
//	}
//	return bst;
//}

//// ---------------------( ２次元 )------------------------------
////
////	点が半直線上にあるかを調べる。
////
//bool MGeo::ChkPt2OnHLn2WS(						// (  O) ステイタス
//												//		 	true:	半直線上にある
//												//		 	false:	半直線上にない
//				const	MgPoint2&	p1,			// (I  ) 点1
//				const	MgHLine2	&HLn2,		// (I  ) 半直線2
//						MINT*		ist,		// (  O) 補助ステイタス
//												//			MC_RIGHT			(010): 点が半直線の右側
//												//			MC_ON_LINE			(020): 点が半直線上
//												//			MC_LEFT				(040): 点が半直線の左側
//						MREAL		rTol		// (I  ) トレランス
//		)
//{
//	bool		bst;							// ステイタス
//	MgVect2		v2s1;							// 半直線2の始点から点1までのベクトル
//	MREAL		pa1;							// 半直線2から点1までの鉛直距離
////
//// 半直線2の方向（単位ベクトル）と半直線2の始点から点1までのベクトルの外積より鉛直距離を求める
//	v2s1 = p1 - HLn2.p;					
//	pa1 = HLn2.v ^ v2s1;						// 半直線2の方向（単位ベクトル）と半直線2の始点から点1までのベクトルの外積
////
//// 鉛直距離が誤差内なら直線上とみなす
//	if ( MgZero( pa1, rTol)) {
//		*ist = MC_ON_LINE;
//		bst = true;
////
//// 鉛直距離が正なら直線の左とみなす
//	} else if ( pa1 > 0) {
//		*ist = MC_LEFT;
//		bst = false;
////
//// 鉛直距離が負なら直線の右とみなす
//	} else {
//		*ist = MC_RIGHT;
//		bst = false;
//	}
//	return bst;
//}

// ---------------------( ２次元 )------------------------------
//
//	点が線分上にあるかを調べる。
//
bool MGeo::ChkPt2OnLn2WS(						// (  O) ステイタス
												//			true:	線分上にある
												//			false:	線分上にない
				const	MgPoint2&	p1,			// (I  ) 点1
				const	MgLine2&	Ln2,		// (I  ) 線分2
						MINT*		ist,		// (  O) 補助ステイタス	(ステイタス1 | ステイタス2)
												//		 ステイタス1
												//			MC_RIGHT			(010): 点が線分の右側
												//			MC_ON_LINE			(020): 点が線分延長直線上
												//			MC_LEFT				(040): 点が線分の左側
												//		 ステイタス2
												//			MC_ON_PS			(001): 点が線分の始点上
												//			MC_INSIDE			(002): 点が線分の内側上
												//			MC_ON_PE			(004): 点が線分の終点上
						MREAL		rTol		// (I  ) トレランス
		)
{
	bool		bst;
	MgVect2		v2s1;							// 線分2の始点から点1へのベクトル
	MgVect2		v2e1;							// 線分2の始点から点1へのベクトル
	MgVect2		v2;								// 線分2の始点から終点へのベクトル
	MREAL		pa1;							// 線分2と線分2の始点から点1へのベクトルの外積
	MREAL		cs, ce;							// 線分2と線分2の始点，終点から点1へのベクトルの内積
	MREAL		tol_2;							// 判定用誤差
	MREAL		v2tol_2;						// 判定用誤差
	MREAL		v2tol_2_1;						// 判定用誤差 v2tol_2/10000
//
	v2s1 = p1 - Ln2.p[0];
	v2e1 = p1 - Ln2.p[1];
	v2 = Ln2.p[1] - Ln2.p[0];
	pa1 = v2 ^ v2s1;
	//
	// 点から線分までの鉛直距離( abs( pa1)/abs( v2)) <= tol より直線上であるかを調べ
	//						 ( pa1 * pa1 <= tol * tol * (v2 * v2))
	// 直線上なら、さらに線分上であるかを調べる
	tol_2 = rTol * rTol;
	v2tol_2 = tol_2 * Abs_2( v2);
	v2tol_2_1 = tol_2 * Abs_2( v2) * 0.0001f * 0.0001f;
	*ist = 0;
	if (pa1 * pa1 <= v2tol_2) { 								// 点から線分までの鉛直距離がrTol以内
		// 延長直線上
		cs = v2 * v2s1;
		ce = v2 * v2e1;
		if (v2s1 * v2s1 <= tol_2) {
			*ist = MC_ON_PS;									// 点が線分の始点上
		} else if (v2e1 * v2e1 <= tol_2) {
			*ist = MC_ON_PE;									// 点が線分の終点上
		} else if (cs > v2tol_2_1 &&  ce < -v2tol_2_1) {
			*ist = MC_INSIDE;									// 点が線分の内側上
		} 
		bst = (*ist != 0);
		*ist |= MC_ON_LINE;
	} else {
		// 延長直線上でないなら、線分の左右どちらにあるかを調べる
		if (pa1 > 0) {
			*ist |= MC_LEFT;									// 左側
		} else {
			*ist |= MC_RIGHT;									// 右側
		}
		bst = false;
	}
	return bst;
}
//// ---------------------( ２次元 )------------------------------
////
////	線分の中心点が直線上にあるかを調べる。
////
//bool MGeo::ChkLn2OnULn2WS(						// (  O) ステイタス
//												//			true:	直線上にある
//												//			false:	直線上にない
//				const	MgLine2&	Ln1,		// (I  ) 線分1
//				const	MgULine2&	ULn2,		// (I  ) 直線2
//						MINT*		ist,		// (  O) ステイタス
//												//			MC_RIGHT			(010): 点が線分の右側
//												//			MC_ON_LINE			(020): 点が直線上
//												//			MC_LEFT				(040): 点が線分の左側
//						MREAL		rTol		// (I  ) トレランス
//		)
//{
//	MgPoint2 Pt1;
//	Pt1 = ( Ln1.p[0] + Ln1.p[1]) * 0.5;
//	return MgChkPt2OnULn2WS( Pt1, ULn2, ist);
//}

// ---------------------( ２次元 )------------------------------
//
//	線分と線分が重なっているかを調べる。
//
bool MGeo::ChkLn2OnLn2(							// (  O) ステイタス
												//			true:	重なっている
												//			false:	重なっていない
				const	MgLine2&	Ln1,		// (I  ) 線分1
				const	MgLine2&	Ln2,		// (I  ) 線分2
						MREAL		rTol		// (I  ) トレランス
		)
{
	bool		bst = false;
	MgVect2		v2s1;							// 線分2の始点から点1へのベクトル
//	MgVect2		v2e1;							// 線分2の始点から点1へのベクトル
	MgVect2		v2;								// 線分2の始点から終点へのベクトル
	MREAL		pa1;							// 線分2と線分2の始点から点1へのベクトルの外積
	MREAL		v2tol_2;						// 判定用誤差
	MREAL		cs11, cs12, cs21, cs22;
	MREAL		d1, d2, dd;
//
	if (!MGeo::Parallel( Ln1, Ln2))									// 平行でない場合は
		MQUIT;													// 重なっていないとする
	v2s1 = Ln1.p[0] - Ln2.p[0];
//	v2e1 = Ln1.p[0] - Ln2.p[1];
	v2 = Ln2.p[1] - Ln2.p[0];
	pa1 = v2 ^ v2s1;
	//
	// 線分1の始点から線分2までの鉛直距離( abs( pa1)/abs( v2)) <= tol より直線上であるかを調べる
	//						 ( pa1 * pa1 <= tol * tol * (v2 * v2))
	cs22 = v2 * v2;
	v2tol_2 = rTol * rTol * cs22;
	if (pa1 * pa1 > v2tol_2) 									// 点から線分までの鉛直距離がrTol超の場合は
		MQUIT;													// 重なっていないとする
	cs21 = 0;
	cs11 = (Ln1.p[0] - Ln2.p[0]) * v2;
	cs12 = (Ln1.p[1] - Ln2.p[0]) * v2;
	if (cs11 < cs12) {
		d1 = MGMAX( cs11, cs21);
		d2 = MGMIN( cs12, cs22);
	} else {
		d1 = MGMAX( cs12, cs21);
		d2 = MGMIN( cs11, cs22);
	}
	dd = d2 - d1;
	// 重なりの始点から終点までの距離( dd/abs( v2)) <= tol より重なっていないかを調べる
	//						 ( dd * dd <= tol * tol * (v2 * v2))
	if (dd < 0. || (dd * dd) <= v2tol_2)						// 重なりがrTol以下の場合は
		MQUIT;													// 重なっていないとする

	bst = true;
exit:
	return bst;
}

//// ---------------------( ３次元 )------------------------------
////
////	点が直線上にあるかを調べる。
////
//bool MGeo::ChkPt3OnULn3(							// (  O) ステイタス
//												//			true:	線分上にある
//												//			false:	線分上にない
//				const	MgPoint3&	p1,			// (I  ) 点1
//				const	MgULine3&	ULn2,		// (I  ) 直線2
//						MREAL		rTol		// (I  ) トレランス
//		)
//{
//	MgVect3		v21, va1;
////	MREAL		pa_2;							//	直線2から点1までの鉛直距離**2
////
//	v21 = p1 - ULn2.p;
//	va1 = v21 ^ ULn2.v;
////	pa_2 = MgAbs_2(va1);
////	return (pa_2 <= rTol * rTol);
//	return (MgAbs_2(va1) <= rTol * rTol);
//}

//// ---------------------( ３次元 )------------------------------
////
////	点が半直線上にあるかを調べる。
////
//bool MGeo::ChkPt3OnHLn3(							// (  O) ステイタス
//												//			true:	線分上にある
//												//			false:	線分上にない
//				const	MgPoint3&	p1,			// (I  ) 点1
//				const	MgHLine3	&HLn2,		// (I  ) 半直線2
//						MREAL		rTol		// (I  ) トレランス
//		)
//{
//	MgVect3		v21, va1;
//	MREAL		c1;
////	MREAL		pa_2;							//	直線2から点1までの鉛直距離**2
////
//	v21 = p1 - HLn2.p;
//	va1 = v21 ^ HLn2.v;
//	c1 = v21 * HLn2.v;
////	pa_2 = MgAbs_2(va1);
////	return (pa_2 <= rTol * rTol);
//	return (MgAbs_2(va1) <= rTol * rTol && c1 > -rTol);
//}

// ---------------------( ３次元 )------------------------------
//
//	点が線分上にあるかを調べる。
//
bool MGeo::ChkPt3OnLn3WS(						// (  O) ステイタス
												//			true:	線分上にある
												//			false:	線分上にない
				const	MgPoint3&	p1,			// (I  ) 点1
				const	MgLine3		&Ln2,		// (I  ) 線分2
						MINT*		ist,		// (  O) 補助ステイタス	(ステイタス1 | ステイタス2)
												//		 ステイタス1
												//			MC_ON_LINE			(020): 点が線分延長直線上
												//		 ステイタス2
												//			MC_ON_PS			(001): 点が線分の始点上
												//			MC_INSIDE			(002): 点が線分の内側上
												//			MC_ON_PE			(004): 点が線分の終点上
						MREAL		rTol		// (I  ) トレランス
		)
{
	bool		bst;
	MgVect3		v2s1;							// 線分2の始点から点1へのベクトル
	MgVect3		v2e1;							// 線分2の終点から点1へのベクトル
	MgVect3		v2;								// 線分2の始点から終点へのベクトル
	MgVect3		va1;							// 線分2と線分2の始点から点1へのベクトルの外積 
	MREAL		cs, ce;
	MREAL		tol_2;							// 判定用誤差
	MREAL		v2tol_2;						// 判定用誤差
	MREAL		v2tol_2_1;						// 判定用誤差 v2tol_2/10000
//
	v2s1 = p1 - Ln2.p[0];
	v2e1 = p1 - Ln2.p[1];
	v2 = Ln2.p[1] - Ln2.p[0];
	va1 = v2 ^ v2s1;
	//
	// 点から線分までの鉛直距離( abs( va1)/abs( v2)) <= tol より直線上であるかを調べ
	//						 ( (va1 * va1) <= tol * tol * (v2 * v2))
	// 直線上なら、さらに線分上であるかを調べる
	tol_2 = rTol * rTol;
	v2tol_2 = tol_2 * Abs_2( v2);
	v2tol_2_1 = tol_2 * Abs_2( v2) * 0.0001f * 0.0001f;
	*ist = 0;
	if ( Abs_2( va1) <= v2tol_2) { 							// 点から線分までの鉛直距離がrTol以内
		cs = v2 * v2s1;
		ce = v2 * v2e1;
		if (v2s1 * v2s1 <= tol_2) {
			*ist = MC_ON_PS;									// 点が線分の始点上
		} else if ( v2e1 * v2e1 <= v2tol_2) {
			*ist = MC_ON_PE;									// 点が線分の終点上
//		} else if ( cs * ce <= 0) {
		} else if ( cs > v2tol_2_1 &&  ce < -v2tol_2_1) {
			*ist = MC_INSIDE;									// 点が線分の内側上
		} 
		bst = ( *ist != 0);
		*ist |= MC_ON_LINE;
	} else {
		bst = false;
	}
	return bst;
}
// ---------------------( ３次元 )------------------------------
//
//	線分と線分が重なっているかを調べる。
//
bool MGeo::ChkLn3OnLn3(							// (  O) ステイタス
												//			true:	重なっている
												//			false:	重なっていない
				const	MgLine3		&Ln1,		// (I  ) 線分1
				const	MgLine3		&Ln2,		// (I  ) 線分2
						MREAL		rTol		// (I  ) トレランス
		)
{
	bool		bst = false;
	MgVect3		v2s1;							// 線分2の始点から点1へのベクトル
//	MgVect3		v2e1;							// 線分2の始点から点1へのベクトル
	MgVect3		v2;								// 線分2の始点から終点へのベクトル
	MgVect3		pa1;							// 線分2と線分2の始点から点1へのベクトルの外積
	MREAL		v2tol_2;						// 判定用誤差
	MREAL		cs11, cs12, cs21, cs22;
	MREAL		d1, d2, dd;
//
	if (!MGeo::Parallel( Ln1, Ln2))									// 平行でない場合は
		MQUIT;													// 重なっていないとする
	v2s1 = Ln1.p[0] - Ln2.p[0];
//	v2e1 = Ln1.p[0] - Ln2.p[1];
	v2 = Ln2.p[1] - Ln2.p[0];
	pa1 = v2 ^ v2s1;
	//
	// 線分1の始点から線分2までの鉛直距離( abs( pa1)/abs( v2)) <= tol より直線上であるかを調べる
	//						 ( pa1 * pa1 <= tol * tol * (v2 * v2))
	cs22 = v2 * v2;
	v2tol_2 = rTol * rTol * cs22;
	if ((pa1 * pa1) > v2tol_2) 									// 点から線分までの鉛直距離がrTol超の場合は
		MQUIT;													// 重なっていないとする
	cs21 = 0;
	cs11 = v2s1 * v2;
	cs12 = (Ln1.p[1] - Ln2.p[0]) * v2;
	if (cs11 < cs12) {
		d1 = MGMAX( cs11, cs21);
		d2 = MGMIN( cs12, cs22);
	} else {
		d1 = MGMAX( cs12, cs21);
		d2 = MGMIN( cs11, cs22);
	}
	dd = d2 - d1;
	// 重なりの始点から終点までの距離( dd/abs( v2)) <= tol より重なっていないかを調べる
	//						 ( dd * dd <= tol * tol * (v2 * v2))
	if (dd < 0. || (dd * dd) <= v2tol_2)						// 重なりがrTol以下の場合は
		MQUIT;													// 重なっていないとする

	bst = true;
exit:
	return bst;
}
// ---------------------( ３次元 )------------------------------
//
//	線分と線分が重なっているかを調べる。
//
bool MGeo::ChkLn3OnLn3WS(						// (  O) ステイタス
												//			true:	重なっている
												//			false:	重なっていない
				const	MgLine3		&Ln1,		// (I  ) 線分1
				const	MgLine3		&Ln2,		// (I  ) 線分2
						MINT*		ist,		// (  O) 補助ステイタス　重なりありの場合は次の通り
												//        -4    -3   -2   -1    0     1    2    3    4 		   	
												//		|--   |--  |--- | -- | --- |----| ---|  --|   --|           	  	
												//		|   --|  --| ---|----| --- | -- |--- |--  |--   |                　	
						MREAL		rTol		// (I  ) トレランス
				)
{
	bool		bst = false;
	MgVect3		v2s1;							// 線分2の始点から点1へのベクトル
//	MgVect3		v2e1;							// 線分2の始点から点1へのベクトル
	MgVect3		v2, vu2;						// 線分2の始点から終点へのベクトル、同単位ベクトル
	MgVect3		pa1;							// 線分2と線分2の始点から点1へのベクトルの外積
	MREAL		cs11, cs12, cs21, cs22;
	MREAL		d1, d2, dd;
	MREAL		d11, d12, d21, d22;
//
	*ist = 0;
	if (!MGeo::Parallel( Ln1, Ln2))									// 平行でない場合は
		MQUIT;													// 重なっていないとする
	v2s1 = Ln1.p[0] - Ln2.p[0];
//	v2e1 = Ln1.p[0] - Ln2.p[1];
	v2 = Ln2.p[1] - Ln2.p[0];
	vu2 = v2.Unitize();
	pa1 = vu2 ^ v2s1;
	//
	// 線分1の始点から線分2までの鉛直距離 abs( pa1) <= tol より直線上であるかを調べる
	//						 ( pa1 * pa1 <= tol * tol)
	if ( Abs_2( pa1) > rTol * rTol) 							// 点から線分までの鉛直距離がrTol超の場合は
		MQUIT;													// 重なっていないとする
	cs21 = 0;
	cs22 = v2 * vu2;
	cs11 = v2s1 * vu2;
	cs12 = (Ln1.p[1] - Ln2.p[0]) * vu2;
	if (cs11 < cs12) {
		d1 = MGMAX( cs11, cs21);
		d2 = MGMIN( cs12, cs22);

		d11 = cs11;
		d12 = cs12;
		d21 = cs21;
		d22 = cs22;

	} else {
		d1 = MGMAX( cs12, cs21);
		d2 = MGMIN( cs11, cs22);

		d11 = cs12;
		d12 = cs11;
		d21 = cs22;
		d22 = cs21;
	}
	// 補助ステイタスを求める
	if (d12 > d22 + rTol) {
		if (d11 > d22 - rTol) {
			if (d11 > d22 + rTol)
				*ist = 4;
			else
				*ist = 3;
		} else {
			if (d11 > d21 + rTol)
				*ist = 2;
			else
				*ist = 1;
		}
	} else if (d11 < d21 - rTol) {
		if (d12 < d21 + rTol) {
			if (d12 < d21 - rTol)
				*ist = -4;
			else
				*ist = -3;
		} else {
			if (d12 < d22 - rTol)
				*ist = -2;
			else
				*ist = 1;
		}
	} else if (d12 > d22 - rTol && d11 < d21 + rTol)
		*ist = 0;
	else
		*ist = -1;

	dd = d2 - d1;
	// 重なりの始点から終点までの距離( dd/abs( v2)) <= tol より重なっていないかを調べる
	//						 ( dd * dd <= tol * tol * (v2 * v2))
	if (dd < 0. || (dd * dd) <= rTol * rTol)					// 重なりがrTol以下の場合は
		MQUIT;													// 重なっていないとする

	bst = true;
exit:
	return bst;
}

//
// ---------------------( ２次元 )------------------------------
//	点と穴付き多角形の位置関係を求める
//
bool MGeo::ChkPt2OnGPg2WS(						// (  O) ステイタス
												//			true:	穴付き多角形の内側または辺上
												//			false:	穴付き多角形の外側
				const	MgPoint2&	i_pt,		// (I  ) 点
				const	MgGPolyg2&	i_Gpg,		// (I  ) 穴付き多角形
						MINT*		o_piSt,		// (  O) ステイタス
												//			MC_IN_BORDER		(001): 点が穴付き多角形の内側
												//			MC_ON_BORDER		(002): 点が穴付き多角形の辺上(または頂点上)
												//			MC_OUT_BORDER		(004): 点が穴付き多角形の外側
						MREAL		i_rTol		// (I  ) トレランス
		)
{
	bool		bst;
	MINT		ist1;
	MINT		ic1;
	MINT		ict, ictSum = 0;									// 内外判定用カウンタ
	MgPolyg2*	pPg;
	
	for ( ic1=0; ic1<i_Gpg.m_n; ic1++) {
		pPg = &(i_Gpg.m_pg[ic1]);

		
		ist1 = CountPg2AroundPt2( i_pt, NULL, *pPg, &ict, i_rTol);
		if ( ist1) {
			*o_piSt = MC_ON_BORDER;
			bst = true;												// 穴付き多角形の辺上(または頂点上)
			MQUIT;
		}
		ictSum += ict;
	}

	if ( ictSum > 0) {
		*o_piSt = MC_IN_BORDER;										// 穴付き多角形の内側
		bst = true;
	} else {
		*o_piSt = MC_OUT_BORDER;										// 穴付き多角形の外側
		bst = false;
	}
exit:
	return bst;
}

//
// ---------------------( ２次元 )------------------------------
//	線分の中心点と穴付き多角形の位置関係を求める
//
bool MGeo::ChkLn2OnGPg2WS(						// (  O) ステイタス
												//			true:	穴付き多角形の内側または辺上
												//			false:	穴付き多角形の外側
				const	MgLine2&	Ln1,		// (I  ) 線分
				const	MgGPolyg2&	GPg2,		// (I  ) 穴付き多角形
						MINT*		isth,		// (  O) 補助ステイタス	(ステイタス1 | ステイタス2)
												// 		 テイタス1
												//			MC_IN_BORDER		(001): 点が穴付き多角形の内側
												//			MC_ON_BORDER		(002): 点が穴付き多角形の辺上(または頂点上)
												//			MC_OUT_BORDER		(004): 点が穴付き多角形の外側
												// 		 テイタス2
												//			MC_ON_SIDE_SAME		(010): 辺上(同一方向)	(MC_ON_BORDERと共に設定)
												//			MC_ON_SIDE_REV		(020): 辺上(逆方向)		(MC_ON_BORDERと共に設定)
												//			MC_ON_TOP			(040): 頂点上			(MC_ON_BORDERと共に設定)
						MREAL		rTol		// (I  ) トレランス
				)
{
	bool		bst;
	MgPoint2	pt1;
	MgVect2		vt1;
	MINT		ist1;
	MINT		ic1;
	MINT		ict, ictSum = 0;									// 内外判定用カウンタ
	MgPolyg2*	pPg;
	
	pt1 = ( Ln1.p[0] + Ln1.p[1]) * 0.5;
	vt1 = Ln1.p[1] - Ln1.p[0];
	
	for ( ic1=0; ic1<GPg2.m_n; ic1++) {
		pPg = &(GPg2.m_pg[ic1]);
		
		ist1 = CountPg2AroundPt2( pt1, &vt1, *pPg, &ict, rTol);
		if ( ist1) {
			*isth = MC_ON_BORDER | ist1;
			bst = true;												// 穴付き多角形の辺上(または頂点上)
			MQUIT;
		}
		ictSum += ict;
	}

	if ( ictSum > 0) {
		*isth = MC_IN_BORDER;										// 穴付き多角形の内側
		bst = true;
	} else {
		*isth = MC_OUT_BORDER;										// 穴付き多角形の外側
		bst = false;
	}
exit:
	return bst;
}
//
// ---------------------( ２次元 )------------------------------
// 点と多角形の位置関係を求める
// 多角形の辺が点の周りを回る周回数を求める（実体一周:1　穴一周:-1）
MINT MGeo::CountPg2AroundPt2(				// (  O) ステイタス
												//								(000): 多角形の中または外	
												//			MC_ON_SIDE_SAME		(010): 辺上(同一方向)
												//			MC_ON_SIDE_REV		(020): 辺上(逆方向)
												//			MC_ON_SIDE			(030): 辺上(線分方向=NULL: 線分方向指定無し)
												//			MC_ON_TOP			(040): 頂点上
				const	MgPoint2&	Pt,			// (I  ) 点
				const	MgVect2		*vst,		// (I  ) 線分方向 または NULL
												//		 引数がNULLで無ければ点が辺上の場合、
												//		 辺の線分方向と比較する
				const	MgPolyg2&	Pg,			// (I  ) 多角形
						MINT*		ict,		// (  O) 周回カウント
												//			>0: 多角形が実体の場合の周回数	(1:1周)
												//			=0: 頂点、辺上、多角形の外
												//			<0: 多角形が穴の場合の周回数	(-1:1周)	
						MREAL		rTol		// (I  ) トレランス
				)
{
	MINT		ist;
	MINT		ic;
	MgPoint2	Ps, Pe;
	MgVect2		Vs, Ve;
	MgVect2		Vl;
	MINT		iQs, iQe;						// 始点、終点の象限
	MINT 		iQse;							// 始点から終点への象限移動数
	MINT 		iQSum = 0;						// 象限移動数の合計
	
	*ict = 0;
	Ps = Pg.m_p[Pg.m_n-1];
	if (Pt == Ps) {								// 点が始点上
		ist = MC_ON_TOP;
		MQUIT;
	}
	iQs = Quadrant( Pt, Ps);
	Vs = Pt - Ps;
	for ( ic=0; ic<Pg.m_n; ic++) {
		Pe = Pg.m_p[ic];
		Ve = Pt - Pe;
		if ( Abs_2(Ve) <= rTol * rTol) {						// 点が終点上(Pt == Pe)
			ist = MC_ON_TOP;
			MQUIT;
		}
		Vl = Pe - Ps;
		MREAL Ss = Vl ^ Vs;
		if ( ( Ss * Ss) <= (rTol * rTol * Abs_2(Vl))) {			// 点から辺までの距離がrTol以内
			MREAL Cs = Vs * Vl;
			MREAL Ce = Ve * Vl;
			if (Cs >= 0.0f && Ce <= 0.0f) {						// 辺上
				if (vst) {
					if (*vst * Vl > 0.f) 
						ist = MC_ON_SIDE_SAME;					// 点が辺上(同一方向)
					else
						ist = MC_ON_SIDE_REV;					// 点が辺上(逆方向)
				} else {
					ist = MC_ON_SIDE;							// 点が辺上(線分方向=NULL: 線分方向指定無し)
				}
				MQUIT;
			}
		}
		
		iQe = Quadrant( Pt, Pe);
		iQse = iQe - iQs;

		if (Ss > 0.0f) {										// 点が線分の左側
			if ( iQse < 0) iQse += 4;
		} else {												// 点が線分の右側
			if ( iQse > 0) iQse -= 4;
		}
		iQSum += iQse;
		
		Ps = Pe;
		iQs = iQe;
		Vs = Ve;
	}
	*ict = iQSum / 4;
	ist = 0;
exit:
	return ist;
}
//
// ---------------------( ３次元 )------------------------------
//	点と多角形の位置関係を求める ３Ｄ
//
/*
bool MGeo::ChkPt3OnPg3WS(						// (  O) ステイタス
												//			true:	多角形の内側または辺上
												//			false:	多角形の外側
				const	MgPoint3&	Pt,			// (I  ) 点
				const	MgPolyg3&	Pg,			// (I  ) 多角形
						MINT*		ist,		// (  O) ステイタス
							  					//		 多角形の平面と点の位置関係
												//			MC_UPPER			(010): 点が平面の上側	
												//			MC_ON_PLANE			(020): 点が平面上
												//			MC_LOWER			(040): 点が平面の下側
												//		 多角形と点の位置関係
												//			MC_IN_BORDER		(001): 点が多角形の内側
												//			MC_ON_BORDER		(002): 点が多角形の辺上
												//			MC_OUT_BORDER		(004): 点が多角形の外側
						MREAL		rTol		// (I  ) トレランス
				)
{
	MgPoint2 wPt;												// 点の２Ｄ作業用
	MINT work[sizeof(MgPolyg2)+MX_LIN2*(sizeof(MgPoint2)-1)];	// 最大構成線数がMX_LIN2の
	MgPolyg2 *wPg = (MgPolyg2*)work;							// 多角形用２Ｄ作業用エリアを確保

	*ist = 0;
	
	MgPlane3 Pln = MgPgtoPln( Pg);								// 多角形が乗る平面
	MREAL pa1 = (Pln.v * Pt) + Pln.d;							// 点から平面までの距離
	
	if (!(MgAbs_2( pa1) <= rTol * rTol)) {						// 多角形の平面上で無い場合に、多角形の上下を判定する
		if (pa1 > 0) *ist = MC_UPPER;							// 多角形の構成点を右ねじ回転とみなし進む方向を上側とする
		else *ist = MC_LOWER;
		return	false;
	}
	MgMat3 mat = MgMat3Dto2D( Pln);
	Mg3Dto2D( Pt, mat, &wPt);
	Mg3Dto2D( Pg, mat, wPg);
	MgPolyg2 wPg1;
	bool bst = MgOnPtPg3( wPt, wPg1, ist);
	bst = MgOnPtPg3( wPt, *wPg, ist);
	*ist |= MC_ON_PLANE;
	return bst;
}
*/
//
// ---------------------( ３次元 )------------------------------
//	線分の中心点と多角形の位置関係を求める
//
/*
bool MGeo::ChkLn3OnPg3WS(						// (  O) ステイタス
												//			true:	多角形上
												//			false:	多角形外
				const	MgLine3		&Ln1,		// (I  ) 線分
				const	MgPolyg3&	Pg2,		// (I  ) 多角形
						MINT*		ist,		// (  O) 補助ステイタス
							  					//		 多角形の平面と点の位置関係
												//			MC_UPPER			(010): 点が平面の上側	
												//			MC_ON_PLANE			(020): 点が平面上
												//			MC_LOWER			(040): 点が平面の下側
												//       多角形と点の位置関係
												//			MC_IN_BORDER		(001): 点が多角形の内側
												//			MC_ON_BORDER		(002): 点が多角形の辺上
												//			MC_OUT_BORDER		(004): 点が多角形の外側
						MREAL		rTol		// (I  ) トレランス
		)
{
	MgPoint3	Pt1;
	Pt1 = ( Ln1.p[0] + Ln1.p[1]) * 0.5;
	return MgOnPtPg3( Pt1, Pg2, ist);
}
*/

////		平面　形状演算
////
//// ---------------------( ３次元 )------------------------------
////
////	点が平面上にあるかを調べる。
////
//bool MGeo::ChkPt3OnPln3WS(						// (  O) ステイタス
//												//			true:  平面上
//												//			false: 平面外
//				const	MgPoint3&	p1,			// (I  ) 点1
//				const	MgPlane3	&Pln2,		// (I  ) 平面2
//						MINT*		ist,		// (  O) 補足ステイタス
//												//			MC_UPPER			(010): 点が平面の上側	
//												//			MC_ON_PLANE			(020): 点が平面上
//												//			MC_LOWER			(040): 点が平面の下側
//						MREAL		rTol		// (I  ) トレランス
//		)
//{
//	bool	bst;
//	MREAL	pa1;								//	点から平面までの距離
////
//	pa1 = (Pln2.v * p1) + Pln2.d;
//	if (pa1 > rTol) {
//		*ist = MC_UPPER;
//		bst = false;
//	} else if (pa1 < -rTol) {
//		*ist = MC_LOWER;
//		bst = false;
//	} else {
//		*ist = MC_ON_PLANE;
//		bst = true;
//	}
//	return bst;
//}

////
//// ---------------------( ３次元 )------------------------------
////	直線が平面上にあるかを調べる。
////
//bool MGeo::ChkULn3OnPln3(						// (  O) ステイタス
//												//			true:  平面上
//												//			false: 平面外
//				const	MgULine3&	ULn1,		// (I  ) 直線1
//				const	MgPlane3	&Pln2,		// (I  ) 平面2
//						MREAL		rTol		// (I  ) トレランス
//		)
//{
//	int		ist;
//	if (MgChkPt3OnPln3WS(ULn1.p, Pln2, &ist)) {
////
////	平行のチェック
////
//		if (MgChkPerp2V3(ULn1.v, Pln2.v)) return true;	//	平面上
//	}
//	return false;
//}

////
//// ---------------------( ３次元 )------------------------------
////	線分が平面上にあるかを調べる。
////
//bool MGeo::ChkLn3OnPln3WS(						// (  O) ステイタス
//												//			true:  平面上
//												//			false: 平面外
//				const	MgLine3		&Ln1,		// (I  ) 線分1
//				const	MgPlane3	&Pln2,		// (I  ) 平面2
//						MINT*		ist,		// (  O) 補足ステイタス
//							  					//		 多角形の平面と点の位置関係
//												//			MC_PS_UPPER			(001): 始点が平面の上側	
//												//			MC_PS_ON_PLANE		(002): 始点が平面上
//												//			MC_PS_LOWER			(004): 始点が平面の下側
//												//       多角形と点の位置関係
//												//			MC_PE_UPPER			(010): 終点が平面の上側	
//												//			MC_PE_ON_PLANE		(020): 終点が平面上
//												//			MC_PE_LOWER			(040): 終点が平面の下側
//						MREAL		rTol		// (I  ) トレランス
//		)
//{
//	bool	bs, be;
//	MINT	ists, iste;
//
//	bs = MgChkPt3OnPln3WS(Ln1.p[0], Pln2, &ists);
//	be = MgChkPt3OnPln3WS(Ln1.p[1], Pln2, &iste);
//	*ist = (ists * MC_PS_UPPER + iste * MC_PE_UPPER)/ MC_UPPER;
//	return (bs && be);
//}

////
//// ---------------------( ２次元 )------------------------------
//// P1を原点としたP2の位置の象限を求める
////
//static MINT MGeo::Quadrant	(						// (  O) 1～4: 象限
//				const	MgPoint2&	p1,			// (I  ) P1
//				const	MgPoint2&	p2			// (I  ) P2
//		)
//{
//	return ((p2.x > p1.x) ? ((p2.y > p1.y) ? 1 : 4) : ((p2.y > p1.y) ? 2 : 3));
//}

} // namespace MC
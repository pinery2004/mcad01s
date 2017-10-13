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
#include "MbDebug.h"

#define	DLL_EXPORT_GEO_DO

#include "MgLib.h"

namespace MC
{

// ---------------------( ２次元 )------------------------------
//		線分を線分で分割する
//		(線分１の端部での交差は交差無しとし分割無し)
//
MINT MGeo::Divide2Ln2(							// (  O) ステイタス
												//			MC_NINT			(0): 分割無し
												//			MC_INT			(1): 分割あり
				const	MgLine2&	Ln1, 		// (I  ) 分割対象線分
				const	MgLine2&	Ln2, 		// (I  ) 分割線分
						MgGLine2*	GLn3		// (  O) 分割後の線分群
				)
{
	MINT		ist;
	MgPoint2	vd1, vd2;
	MgPoint2 	po;
	MREAL		ss1, se1;
	MREAL		ss2, se2;

	_ASSERTE(Ln1.p[0] != Ln1.p[1]);								// debug --> check same point
//
	GLn3->m_n = 0;
//
//	平行のチェック
//
	vd1 = Ln1.p[1] - Ln1.p[0];
	vd2 = Ln2.p[1] - Ln2.p[0];
	if (!MGeo::Parallel( vd1, vd2)) {								// 交差
		SVal( Ln1.p[0], Ln1.p[1], Ln2.p[0], vd2, &ss1, &se1);
		SVal( Ln2.p[0], Ln2.p[1], Ln1.p[0], vd1, &ss2, &se2);
		if (ss1 * se1 < 0) {									// 交点は線分１の内側
			po = Ln1.p[0] + (vd1 * (ss1 / (ss1 - se1)));		// 交点
			if (ss2 * se2 < 0 || po == Ln2.p[0] || po == Ln2.p[1]) { // 交点は線分２上
				if (po != Ln1.p[0] && po != Ln1.p[1]) {			// 線分１の端部は交点と見なさない
					(*GLn3) += MgLine2(Ln1.p[0], po);			// 分割あり
					(*GLn3) += MgLine2(po, Ln1.p[1]);
					ist = MC_INT;
					MQUIT;
				}
			}
		}
	}
	(*GLn3) += Ln1;												//	分割無し
	ist = MC_NINT;
exit:
	return ist;
}

// ---------------------( ２次元 )------------------------------
//		線分を直線で分割する
//		(線分１の端部での交差は交差無しとし分割無し)
//
MINT MGeo::DivideLnULn2(							// (  O) ステイタス
												//			MC_NINT			(0): 交差なし
												//			MC_INT			(1): 交差あり
				const	MgLine2&	Ln1, 		// (I  ) 分割対象線分
				const	MgULine2&	ULn2, 		// (I  ) 分割直線
						MgGLine2*	GLn3		// (  O) 分割後の線分群
				)
{
	MINT		ist;
	MgVect2		vd1;
	MgPoint2	po;
	MREAL		ss, se;
//
	_ASSERTE(Ln1.p[0] != Ln1.p[1]);								// debug --> check same point

	GLn3->m_n = 0;
//
//　平行のチェック
//
	vd1 = Ln1.p[1] - Ln1.p[0];
	if (!MGeo::Parallel( vd1, ULn2.v)) {
		SVal( Ln1.p[0], Ln1.p[1], ULn2.p, ULn2.v, &ss, &se);
		if (ss * se < 0) {										//	交点は線分１の内側
			po = Ln1.p[0] + (vd1 * (ss / (ss - se)));			//	交点
			if (po != Ln1.p[0] && po != Ln1.p[1]) {				//	線分１の端部は交点と見なさない
				(*GLn3) += MgLine2( Ln1.p[0], po);				// 分割あり
				(*GLn3) += MgLine2( po, Ln1.p[1]);
				ist = MC_INT;
				MQUIT;
			}
		}
	}
	(*GLn3) += Ln1;												//	分割無し
	ist = MC_NINT;
exit:
	return ist;
}

// ---------------------( ２次元 )------------------------------
//		線分を直線で分割し直線の選択側の線分を分割後の線分群に追加する
//		(線分の端部での交差は交差なしとする)
//			選択条件に「辺上同一方向または辺上逆方向」が設定されており、分割後の線分が穴付き多角形の辺上にある場合には、
//			次のように選択する
//				(1) 選択条件が辺上同一方向の場合は、同一方向の時残し
//				(2) 選択条件が辺上逆方向の場合は、逆方向の時残す
//				(3) その他の場合は捨てる
//		//			(1) 選択条件が直線の左側|直線上の場合は、同一方向の時残し
//		//			(2) 選択条件が直線の右側|直線上の場合は、逆方向の時残す
//		//			(3) その他の場合は捨てる
//
// 		sels = MC_LEFT | MC_ON_BORDER | MC_SAME_DIR;

//
MINT MGeo::DivideAddLnULn2(						// (  O) ステイタス
												//			MC_NINT			(0): 交差なし
												//			MC_INT			(1): 交差あり
												//			MC_MATCH		(4): 選択条件に合う部分あり
												//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
												//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
												//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
						MINT		Sel,		// (I  ) 選択条件
												//			MC_LEFT			(010): 直線の左側
												//			MC_ON_LINE		(020): 直線上
												//			MC_RIGHT		(040): 直線の右側
												//			MC_SAME_DIR		(0100):	辺上同一方向
												//			MC_REV_DIR		(0200):	辺上逆方向
				const	MgLine2&	Ln1, 		// (I  ) 分割対象線分
				const	MgULine2&	ULn2, 		// (I  ) 分割直線
						MgGLine2*	GLn3		// (I O) 分割後の線分群
				)
{
	MINT		ist;
	MINT		ist1;
	MgVect2		vd1;
	MgPoint2 	po;
	MREAL		ss, se;
	MREAL		selr;
//
	_ASSERTE(Ln1.p[0] != Ln1.p[1]);								// debug --> check same point
//
	if (MF_CHECK_OR( Sel, MC_LEFT)) selr = 1.;
	else selr = -1.;
//
	vd1 = Ln1.p[1] - Ln1.p[0];
	// 線分と直線が平行で線分の端部でない位置で交差する場合は、
	// 線分を分割し選択条件に合う方を分割後の線分群に追加する
	if (!MGeo::Parallel( vd1, ULn2.v)) {
		SVal( Ln1.p[0], Ln1.p[1], ULn2.p, ULn2.v, &ss, &se);
		if (ss * se < 0) {										// 交点は線分１の内側にあり
			po = Ln1.p[0] + (vd1 * (ss / (ss - se)));			// 交点を求める
			if (po != Ln1.p[0] && po != Ln1.p[1]) {				// 交点が線分１の端部で無いならば
				if (ss * selr > 0) {							// 線分を分割し選択条件に合う方を分割後の線分群に追加する
					(*GLn3) += MgLine2( Ln1.p[0], po);
				} else {
					(*GLn3) += MgLine2( po, Ln1.p[1]);
				}
				ist = MC_INT | MC_MATCH;
				MQUIT;
			}
		}
	}
	// 平行または分割無しの場合選択条件に合う場合は分割後の線分群に追加する
	ist = MC_NINT;
	MGeo::ChkLn2OnULn2WS( Ln1, ULn2, &ist1);								// 線分の中心点が直線と選択条件に合うか調べる
	if (MF_CHECK_OR( Sel, MC_SAME_DIR | MC_REV_DIR) &&			// 選択条件に「辺上同一方向または辺上逆方向」が設定されている場合は、
		MF_CHECK_OR( ist1, MC_ON_LINE)) {						// 分割後の線分が直線上にあると次のように選択する
		MREAL	cvd1 = vd1 * ULn2.v;
		if ((MF_CHECK_OR( Sel, MC_SAME_DIR) && cvd1 > 0. ||		//	(1) 選択条件が辺上同一方向の場合は、同一方向の時を分割後の線分群に追加する
			 MF_CHECK_OR( Sel, MC_REV_DIR) && cvd1 < 0. )) {	//	(2) 選択条件が辺上逆方向の場合は、逆方向の時を分割後の線分群に追加する
			(*GLn3) += Ln1;
			ist |= MC_MATCH;
		}														//	(3) その他の場合は捨てる
	} else if (MF_CHECK_OR( ist1, Sel & (MC_LEFT | MC_ON_LINE | MC_RIGHT))) {
																// その他、線分が選択条件に合う場合は分割後の線分群に追加する
		(*GLn3) += Ln1;
		ist |= MC_MATCH;
	}
exit:
	return ist;
}

// ---------------------( ２次元 )------------------------------
//		線分を線分群との交点で分割する
//		(線分の端部での交差は交差なしとする)
//
MINT MGeo::DivideLnGLn2(							// (  O) ステイタス
												//			MC_NINT			(0): 交差なし
												//			MC_INT			(1): 交差あり
				const	MgLine2&	Ln1, 		// (I  ) 分割対象線分
				const	MgGLine2&	GLn2, 		// (I  ) 分割線分群
						MgGLine2*	GLn3		// (  O) 分割後の線分群
				)
{
	MINT		ist;
	MINT		ic2, ic3;
	MINT		ULn1 = 0;
	MGGPOINT2( GPt, MX_PNT1);						// 点集合の作業エリア

		_ASSERTE(Ln1.p[0] != Ln1.p[1]);									// debug --> check same point
	GLn3->m_n = 0;
	
	GPt += Ln1.p[0];											// 始点と終点を点集合の作業エリアにセットする
	GPt += Ln1.p[1];
	for ( ic2=0; ic2<GLn2.m_n; ic2++) {
																// 線分と線分の交点を求める
		IntrAdd2Ln2( Ln1, GLn2.m_ln[ic2], &GPt);				// 交点（２線分が重なり時には２点もあり）
	}
//
	SortLnGPt2( Ln1, &GPt);									// 求めた交点の同一点を間引いて、Ln1の方向に一列に並べる
//
	if (GLn3->m_isz < GPt.m_n - 1)
		GLn3->Resize( GPt.m_n - 1);
	for ( ic3=1; ic3<GPt.m_n; ic3++) {							// 分割された線分を求める
		(*GLn3) += MgLine2( GPt.m_p[ic3-1], GPt.m_p[ic3]);
	}
//
	if (GPt.m_n > 2)
		ist = MC_INT;
	else
		ist = MC_NINT;
//
	return ist;
}

// ---------------------( ２次元 )------------------------------
//		線分群を交点で分割し同一線分は除き一本にする（逆方向も除く）
//		交差判定で２線分が重なる場合は端点で分割する
//
MINT MGeo::DivideGLn2(							// (  O) ステイタス
												//			MC_NINT			(0): 交差なし
												//			MC_INT			(1): 交差あり
				const	MgGLine2&	GLn1, 		// (I  ) 線分群
						MgGLine2*	GLn2		// (  O) 分割後の線分群
				)
{
	MINT		ist = MC_NINT;
	MINT		ic1, ic2, ic3;
	MINT		ULn1 = 0;
	MGGPOINT2( GPt, MX_PNT1);						// 点群の作業エリア
	
	GLn2->m_n = 0;

	for ( ic1=0; ic1<GLn1.m_n; ic1++) {
		MgLine2 Ln1 = GLn1.m_ln[ic1];							// 一本の線分を取り出す

		GPt.m_n = 0;
		GPt += Ln1.p[0];										// 始点と終点を点集合作業エリアにセットする
		GPt += Ln1.p[1];
		for ( ic2=0; ic2<GLn1.m_n; ic2++) {
			if ( ic1 == ic2) continue;
																// 線分と線分の交点を求める
			IntrAdd2Ln2( Ln1, GLn1.m_ln[ic2], &GPt);			// 交点（２線分が重なり時には２点もあり）
		}
//
		SortLnGPt2( Ln1, &GPt);									// 求めた交点の同一点を間引いて、Ln1の方向に一列に並べる
//
		for ( ic3=1; ic3<GPt.m_n; ic3++) {						// 分割された線分を求める
			(*GLn2) += MgLine2( GPt.m_p[ic3-1], GPt.m_p[ic3]);
		}
		if (GPt.m_n > 2)
			ist = MC_INT;
	}
	MergeGLn2( GLn2);											// 重なっている線分を削除する
	return ist;
}

// ---------------------( ２次元 )------------------------------
//		直線を多角形で分割し、選択条件に合う線分群を取り出す
//			選択条件に「辺上同一方向」が設定されており、分割後の線分が穴付き多角形の辺上にある場合には、
//			次のように選択する
//				(1) 選択条件が辺上同一方向の場合は、同一方向の時残し
//				(2) その他の場合は捨てる
//
MINT MGeo::DivideULnPg2(							// (  O) ステイタス
												//			MC_NINT			(0): 交差なし
												//			MC_INT			(1): 交差あり
												//			MC_MATCH		(4): 選択条件に合う部分あり
												//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
												//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
												//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
						MINT		Sel,		// (I  ) 選択条件
												//			MC_IN_BORDER	(001): 内側
												//			MC_ON_BORDER	(002): 辺上
												//			MC_SAME_DIR		(0100):	辺上同一方向
				const	MgULine2&	ULn1,		// (I  ) 直線
				const	MgPolyg2&	Pg2,		// (I  ) 多角形
						MgGLine2*	GLn3		// (  O) 分割後の線分群
				)
{
	GLn3->m_n = 0;
	return DivideAddULnPg2( Sel, ULn1, Pg2, GLn3);
}

// ---------------------( ２次元 )------------------------------
//		直線を多角形で分割し、選択条件に合う線分を分割後の線分群に追加する
//			選択条件に「辺上同一方向」が設定されており、分割後の線分が穴付き多角形の辺上にある場合には、
//			次のように選択する
//				(1) 選択条件が辺上同一方向の場合は、同一方向の時残し
//				(2) その他の場合は捨てる
//
MINT MGeo::DivideAddULnPg2(						// (  O) ステイタス
												//			MC_NINT			(0): 交差なし
												//			MC_INT			(1): 交差あり
												//			MC_MATCH		(4): 選択条件に合う部分あり
												//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
												//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
												//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
						MINT		Sel,		// (I  ) 選択条件
												//			MC_IN_BORDER	(001): 内側
												//			MC_ON_BORDER	(002): 辺上
												//			MC_SAME_DIR		(0100):	辺上同一方向
				const	MgULine2&	ULn1,		// (I  ) 直線
				const	MgPolyg2&	Pg2,		// (I  ) 多角形
						MgGLine2*	GLn3		// (I O) 分割後の線分群
				)
{
	MINT		ist;
	MINT		ist1;
	MINT		ic1, icb;
	MgLine2		lnw1;
	MGGPOINT2( GPt, MX_PNT1);						// 点群の作業エリア

	// 多角形の全ての辺（線分）との交点を求め直線の方向にあわせて並べる
	for ( icb=Pg2.m_n-1,ic1=0; ic1<Pg2.m_n; icb=ic1,ic1++) {
		lnw1 = MgLine2( Pg2.m_p[icb], Pg2.m_p[ic1]);
		IntrAddULnLn2( ULn1, lnw1, &GPt);							// 線分と線分の交点（２線分が重なり時には２点もあり）
	}

	MGeo::SortVGPt2( ULn1.v, &GPt);										// 求めた交点の同一点を間引いて一列に並べる

	if (GPt.m_n >= 2)
		ist = MC_INT;
	else
		ist = MC_NINT;
	// 分割後の線分が多角形と選択条件に合うもののみ残す
	for ( ic1=1; ic1<GPt.m_n; ic1++) {
		lnw1 = MgLine2( GPt.m_p[ic1-1], GPt.m_p[ic1]);
		ChkLn2OnPg2WS( lnw1, Pg2, &ist1);							// 線分の中心点が多角形と選択条件の関係か調べる
		if (MF_CHECK_OR( Sel, MC_SAME_DIR) &&						// 選択条件に「辺上同一方向」が設定されている場合は、
			MF_CHECK_OR( ist1, MC_ON_SIDE)) {						// 分割後の線分が多角形の辺上にあると次のように選択する
			if (MF_CHECK_OR( ist1, MC_ON_SIDE_SAME)) {				// (1) 同一方向の時残し
				(*GLn3) += lnw1;
				ist |= MC_MATCH;
			}													//  (2) その他の場合は捨てる
		} else if (MF_CHECK_OR( ist1, Sel & (MC_IN_BORDER | MC_ON_BORDER))) {
																// その他、線分が選択条件に合う場合は分割後の線分群に追加する
			(*GLn3) += lnw1;
			ist |= MC_MATCH;
		}
	}
	return ist;
}

// ---------------------( ２次元 )------------------------------
//		線分を多角形で分割し、選択条件に合う線分群を得る
//			選択条件に「辺上同一方向または辺上逆方向」が設定されており、分割後の線分が穴付き多角形の辺上にある場合には、
//			次のように選択する
//				(1) 選択条件が辺上同一方向の場合は、同一方向の時残し
//				(2) 選択条件が辺上逆方向の場合は、逆方向の時残す
//				(3) その他の場合は捨てる
//
MINT MGeo::DivideLnPg2(							// (  O) ステイタス
												//			MC_NINT			(0): 交差なし
												//			MC_INT			(1): 交差あり
												//			MC_MATCH		(4): 選択条件に合う部分あり
												//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
												//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
												//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
						MINT		Sel,		// (I  ) 選択条件
												//			MC_IN_BORDER	(001): 内側
												//			MC_ON_BORDER	(002): 辺上
												//			MC_OUT_BORDER	(004): 外側
												//			MC_FOR_POLYGON	(0100): 多角形作成用
				const	MgLine2&	Ln1,		// (I  ) 線分
				const	MgPolyg2&	Pg2,		// (I  ) 多角形
						MgGLine2*	GLn3		// (  O) 分割後の線分群
				)
{
	GLn3->m_n = 0;
	return DivideAddLnPg2( Sel, Ln1, Pg2, GLn3);
}

// ---------------------( ２次元 )------------------------------
//		線分を多角形で分割し、選択条件に合う線分を分割後の線分群に追加する
//			選択条件に「辺上同一方向または辺上逆方向」が設定されており、分割後の線分が穴付き多角形の辺上にある場合には、
//			次のように選択する
//				(1) 選択条件が辺上同一方向の場合は、同一方向の時残し
//				(2) 選択条件が辺上逆方向の場合は、逆方向の時残す
//				(3) その他の場合は捨てる
//
MINT MGeo::DivideAddLnPg2(						// (  O) ステイタス
												//			MC_NINT			(0): 交差なし
												//			MC_INT			(1): 交差あり
												//			MC_MATCH		(4): 選択条件に合う部分あり
												//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
												//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
												//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
						MINT		Sel,		// (I  ) 選択条件
												//			MC_IN_BORDER	(001): 内側
												//			MC_ON_BORDER	(002): 辺上
												//			MC_OUT_BORDER	(004): 外側
												//			MC_SAME_DIR		(0100):	辺上同一方向
												//			MC_REV_DIR		(0200):	辺上逆方向
				const	MgLine2&	Ln1,		// (I  ) 線分
				const	MgPolyg2&	Pg2,		// (I  ) 多角形
						MgGLine2*	GLn3		// (I O) 分割後の線分群
				)
{
	MINT		ist;
	MINT		ist1;
	MINT		ic1, icb;
	MgLine2		lnw1;
	MGGPOINT2( GPt, MX_PNT1);						// 点群の作業エリア
	
	// 多角形の全ての辺（線分）の交点を求め線分の始点から終点の方向にあわせて並べる
	GPt += Ln1.p[0];
	GPt += Ln1.p[1];
																// 多角形の全ての辺（線分）との交点を求める
	for ( icb=Pg2.m_n-1,ic1=0; ic1<Pg2.m_n; icb=ic1,ic1++) {
		lnw1 = MgLine2( Pg2.m_p[icb], Pg2.m_p[ic1]);
		IntrAdd2Ln2( Ln1, lnw1, &GPt);							// 線分と線分の交点（２線分が重なり時には２点もあり）
	}

	SortLnGPt2( Ln1, &GPt);										// 求めた交点の同一点を間引いて一列に並べる

	if (GPt.m_n > 2)
		ist = MC_INT;
	else
		ist = MC_NINT;
	// 分割後の線分が多角形と選択条件に合うもののみ残す
	for ( ic1=1; ic1<GPt.m_n; ic1++) {
		lnw1 = MgLine2( GPt.m_p[ic1-1], GPt.m_p[ic1]);
		ChkLn2OnPg2WS( lnw1, Pg2, &ist1);						// 線分の中心点が多角形と選択条件の関係か調べる
		if (MF_CHECK_OR( Sel, MC_SAME_DIR | MC_REV_DIR) &&		// 選択条件に「辺上同一方向または辺上逆方向」が設定されている場合は、
			MF_CHECK_OR( ist1, MC_ON_SIDE)) {					// 分割後の線分が多角形の辺上にあると次のように選択する
			if ((MF_CHECK_OR( Sel, MC_SAME_DIR) && MF_CHECK_OR( ist1, MC_ON_SIDE_SAME)) ||
																//	(1) 選択条件が辺上同一方向の場合は、同一方向の時残し
				(MF_CHECK_OR( Sel, MC_REV_DIR) && MF_CHECK_OR( ist1, MC_ON_SIDE_REV))) {
																//	(2) 選択条件が辺上逆方向の場合は、逆方向の時残す
				(*GLn3) += lnw1;
				ist |= MC_MATCH;
			}													//	(3) その他の場合は捨てる
		} else if (MF_CHECK_OR( ist1, Sel & (MC_IN_BORDER | MC_ON_BORDER | MC_OUT_BORDER))) {
																// その他、線分が選択条件に合う場合は分割後の線分群に追加する
			(*GLn3) += lnw1;
			ist |= MC_MATCH;
		}
	}
	return ist;
}

// ---------------------( ２次元 )------------------------------
//		直線を穴付き多角形で分割し、選択条件に合う線分群を得る
//			選択条件に「辺上同一方向」が設定されており、分割後の線分が穴付き多角形の辺上にある場合には、
//			次のように選択する
//				(1) 選択条件が辺上同一方向の場合は、同一方向の時残し
//				(2) その他の場合は捨てる
//
MINT MGeo::DivideULnGPg2(						// (  O) ステイタス
												//			MC_NINT			(0): 選択条件に合う線分なし
												//			MC_INT			(1): 選択条件に合う線分あり
						MINT		Sel,		// (I  ) 選択条件
												//			MC_IN_BORDER	(001): 内側
												//			MC_ON_BORDER	(002): 辺上
												//			MC_SAME_DIR		(0100):	辺上同一方向
				const	MgULine2&	ULn1,		// (I  ) 直線
				const	MgGPolyg2&	gPg2,		// (I  ) 穴付き多角形
						MgGLine2*	GLn3		// (  O) 分割後の線分群
				)
{
	GLn3->m_n = 0;
	return DivideAddULnGPg2( Sel, ULn1, gPg2, GLn3);
}

// ---------------------( ２次元 )------------------------------
//		直線を穴付き多角形で分割し、選択条件に合う線分を分割後の線分群に追加する
//			選択条件に「辺上同一方向」が設定されており、分割後の線分が穴付き多角形の辺上にある場合には、次のように選択する
//				(1) 選択条件が辺上同一方向の場合は、同一方向の時残し
//				(2) その他の場合は捨てる
//
MINT MGeo::DivideAddULnGPg2(							// (  O) ステイタス
													//			MC_NINT			(0): 選択条件に合う線分なし
													//			MC_INT			(1): 選択条件に合う線分あり
													//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
													//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
													//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
						MINT		Sel,			// (I  ) 選択条件
													//			MC_IN_BORDER	(001): 内側
													//			MC_ON_BORDER	(002): 辺上
													//			MC_SAME_DIR		(0100):	辺上同一方向
				const	MgULine2&	ULn1,			// (I  ) 直線
				const	MgGPolyg2&	gpg2,			// (I  ) 穴付き多角形
						MgGLine2*	gLn3			// (I O) 分割後の線分群
				)
{
	MINT		ist;
	MINT		ist1;
	MINT		ic1, ic2, icb;
	MgLine2		lnw1;
	MGGPOINT2( GPt, MX_PNT1);									// 点群の作業エリア
	
	// 穴付き多角形の全ての辺（線分）との交点を求め直線の方向にあわせて並べる
	for ( ic1=0; ic1<gpg2.m_n; ic1++) {
		MgPolyg2*	pg2 = &(gpg2.m_pg[ic1]);
																// ポリゴンの全て辺との交点を求める
		for ( icb=pg2->m_n-1,ic2=0; ic2<pg2->m_n; icb=ic2,ic2++) {
			lnw1 = MgLine2( pg2->m_p[icb], pg2->m_p[ic2]);
			IntrAddULnLn2( ULn1, lnw1, &GPt);						// 線分と線分の交点（２線分が重なり時には２点もあり）
		}
	}

	MGeo::SortVGPt2( ULn1.v, &GPt);										// 求めた交点の同一点を間引いて一列に並べる

	if (GPt.m_n >= 2)
		ist = MC_INT;
	else
		ist = MC_NINT;
	// 分割後の線分が穴付き多角形と選択条件に合うもののみ残す
	for ( ic1=1; ic1<GPt.m_n; ic1++) {
		lnw1 = MgLine2(GPt.m_p[ic1-1], GPt.m_p[ic1]);
		ChkLn2OnGPg2WS( lnw1, gpg2, &ist1);								// 線分の中心点が穴付き多角形と選択条件の関係か調べる
		if (MF_CHECK_OR( Sel, MC_SAME_DIR) &&							// 選択条件に「辺上同一方向」が設定されている場合は、
			MF_CHECK_OR( ist1, MC_ON_SIDE)) {							// 分割後の線分が穴付き多角形の辺上にあると、次のように選択する
			if (MF_CHECK_OR( ist1, MC_ON_SIDE_SAME)) {				//	(1) 同一方向の時残し
				(*gLn3) += lnw1;
				ist |= MC_MATCH;
			}													//	(2) その他の場合は捨てる
		} else if (MF_CHECK_OR( ist1, Sel & (MC_IN_BORDER | MC_ON_BORDER))) {
																// その他、線分が選択条件に合う場合は分割後の線分群に追加する
				(*gLn3) += lnw1;
				ist |= MC_MATCH;
		}
	}
	return ist;
}

// ---------------------( ２次元 )------------------------------
//		線分を穴付き多角形で分割し、選択条件に合う線分群を得る
//			選択条件に「辺上同一方向または辺上逆方向」が設定されており、分割後の線分が穴付き多角形の辺上にある場合には、
//			次のように選択する
//				(1) 選択条件が辺上同一方向の場合は、同一方向の時残し
//				(2) 選択条件が辺上逆方向の場合は、逆方向の時残す
//				(3) その他の場合は捨てる
//
MINT MGeo::DivideLnGPg2(							// (  O) ステイタス
												//			MC_NINT			(0): 交差なし
												//			MC_INT			(1): 交差あり
												//			MC_MATCH		(4): 選択条件に合う部分あり
												//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
												//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
												//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
						MINT		Sel,		// (I  ) 選択条件
												//			MC_IN_BORDER	(001): 内側
												//			MC_ON_BORDER	(002): 辺上
												//			MC_OUT_BORDER	(004): 外側
												//			MC_SAME_DIR		(0100):	辺上同一方向
												//			MC_REV_DIR		(0200):	辺上逆方向
				const	MgLine2&	Ln1,		// (I  ) 線分
				const	MgGPolyg2&	GPg2,		// (I  ) 穴付き多角形
						MgGLine2*	GLn3		// (  O) 分割後の線分群
				)
{
	GLn3->m_n = 0;
	return DivideAddLnGPg2( Sel, Ln1, GPg2, GLn3);
}

// ---------------------( ２次元 )------------------------------
//		線分を穴付き多角形で分割し、選択条件に合う線分を分割後の線分群に追加する
//			選択条件に「辺上同一方向または辺上逆方向」が設定されており、分割後の線分が穴付き多角形の辺上にある場合には、
//			次のように選択する
//				(1) 選択条件が辺上同一方向の場合は、同一方向の時残し
//				(2) 選択条件が辺上逆方向の場合は、逆方向の時残す
//				(3) その他の場合は捨てる
//
MINT MGeo::DivideAddLnGPg2(						// (  O) ステイタス
												//			MC_NINT			(0): 交差なし
												//			MC_INT			(1): 交差あり
												//			MC_MATCH		(4): 選択条件に合う部分あり
												//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
												//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
												//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
						MINT		Sel,		// (I  ) 選択条件
												//			MC_IN_BORDER	(001): 内側
												//			MC_ON_BORDER	(002): 辺上
												//			MC_OUT_BORDER	(004): 外側
												//			MC_SAME_DIR		(0100):	辺上同一方向
												//			MC_REV_DIR		(0200):	辺上逆方向
				const	MgLine2&	Ln1,		// (I  ) 線分
				const	MgGPolyg2&	GPg2,		// (I  ) 穴付き多角形
						MgGLine2*	GLn3		// (I O) 分割後の線分群
				)
{
	MINT		ist;
	MINT		ist1;
	MINT		ic1, ic2, icb;
	MgLine2		lnw1;
	MGGPOINT2(GPt, MX_PNT1);									// 点群の作業エリア
	
	// 穴付き多角形の全ての辺（線分）の交点を求め線分の始点から終点の方向にあわせて並べる
	GPt += Ln1.p[0];
	GPt += Ln1.p[1];

	for ( ic1=0; ic1<GPg2.m_n; ic1++) {
		MgPolyg2*	pg2 = &(GPg2.m_pg[ic1]);
																// 多角形の全て辺との交点を求める
		for ( icb=pg2->m_n-1,ic2=0; ic2<pg2->m_n; icb=ic2,ic2++) {
																// 線分と線分の交点を求める
			lnw1 = MgLine2( pg2->m_p[icb], pg2->m_p[ic2]);
			IntrAdd2Ln2( Ln1, lnw1, &GPt);						// 交点（２線分が重なり時には２点もあり）
		}
	}

	SortLnGPt2( Ln1, &GPt);										// 求めた交点の同一点を間引いて一列に並べる

	if (GPt.m_n > 2)
		ist = MC_INT;
	else
		ist = MC_NINT;
	// 分割後の線分が穴付き多角形と選択条件に合致するもののみ残す
	for ( ic1=1; ic1<GPt.m_n; ic1++) {
		lnw1 = MgLine2( GPt.m_p[ic1-1], GPt.m_p[ic1]);
		ChkLn2OnGPg2WS( lnw1, GPg2, &ist1);						// 線分の中心点が穴付き多角形と選択条件の関係か調べる
		if (MF_CHECK_OR( Sel, MC_SAME_DIR | MC_REV_DIR) &&		// 選択条件に「辺上同一方向または辺上逆方向」が設定されている場合は、
			MF_CHECK_OR( ist1, MC_ON_SIDE)) {					// 分割後の線分が穴付き多角形の辺上にあると次のように選択する
			if (MF_CHECK_OR( Sel, MC_ON_BORDER) &&
				((MF_CHECK_OR( Sel, MC_SAME_DIR) && MF_CHECK_OR( ist1, MC_ON_SIDE_SAME)) ||
																//	(1) 選択条件が辺上同一方向の場合は、同一方向の時残し
				 (MF_CHECK_OR( Sel, MC_REV_DIR) && MF_CHECK_OR( ist1, MC_ON_SIDE_REV)))) {
																//	(2) 選択条件が辺上逆方向の場合は、逆方向の時残す
				(*GLn3) += lnw1;
				ist |= MC_MATCH;
			}													//	(3) その他の場合は捨てる
		} else if (MF_CHECK_OR( ist1, Sel & (MC_IN_BORDER | MC_ON_BORDER | MC_OUT_BORDER))) {
																// その他、線分が選択条件に合う場合は分割後の線分群に追加する
			(*GLn3) += lnw1;
			ist |= MC_MATCH;
		}
	}
	return ist;
}

// ---------------------( ２次元 )------------------------------
//		重なっている線分を削除し一本にする (逆方向も含む)				<要効率UP?>
//
void MGeo::MergeGLn2(
						MgGLine2*	Gln			// (I O) 線分群
				)
{
	MINT	ic0, ic1, ic2;
	bool	fEq;
	MINT	nGln = Gln->m_n;

	for ( ic1=0; ic1<nGln; ic1++) {
		fEq = false;
		ic0 = ic1 + 1;
		for ( ic2=ic0; ic2<nGln; ic2++) {
			if (Gln->m_ln[ic1] == Gln->m_ln[ic2]) {
				fEq = true;										// 重なり線分(等しい線分)があり
			} else {
				if (fEq) Gln->m_ln[ic0] = Gln->m_ln[ic2];		// 途中に重なり線分があれば詰める
				ic0++;
			}
		}
		nGln = ic0;
	}
	Gln->m_n = nGln;
}

//===========================================================================
// ---------------------( ３次元 )------------------------------
//		直線を多角形で分割し、選択条件に合う線分群を取り出す
//
MINT MGeo::DivideULnPg3(							// (  O) ステイタス
												//		 MC_NINT		(0): 交差なし
												//		 MC_INT			(1): 交差あり
						MINT		Sel,		// (I  ) 選択条件
												//		 MC_IN_BORDER	(001): 内側
												//		 MC_ON_BORDER	(002): 辺上
				const	MgULine3&	ULn1,		// (I  ) 直線
				const	MgPolyg3&	Pg2,		// (I  ) 多角形
						MgGLine3	*GLn3		// (  O) 分割後の線分群
				)
{
	GLn3->m_n = 0;
	return DivideAddULnPg3( Sel, ULn1, Pg2, GLn3);
}

/*MINT MGeo::DivideULnPg3(							// (  O) ステイタス
												//			MC_NINT			(0): 交差なし
												//			MC_INT			(1): 交差あり
												//			MC_MATCH		(4): 選択条件に合う部分あり
												//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
												//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
												//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
						MINT		Sel,		// (I  ) 選択条件
												//			MC_IN_BORDER	(001): 内側
												//			MC_ON_BORDER	(002): 辺上
				const	MgULine3&	ULn1,		// (I  ) 直線
				const	MgPolyg3&	Pg2,		// (I  ) 多角形
						MgGLine3	*GLn3		// (I O) 分割後の線分群
				)
{
	GLn3->m_n = 0;
	return DivideAddULnPg3( Sel, ULn1, Pg2, GLn3);
}
*/

// ---------------------( ３次元 )------------------------------
//		直線を多角形で分割し、分割条件に合う線分を分割後の線分群に追加する
//
MINT MGeo::DivideAddULnPg3(						// (  O) ステイタス
												//			MC_NINT			(0): 交差なし
												//			MC_INT			(1): 交差あり
												//			MC_MATCH		(4): 選択条件に合う部分あり
												//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
												//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
												//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
						MINT		Sel,		// (I  ) 選択条件
												//			MC_IN_BORDER	(001): 内側
												//			MC_ON_BORDER	(002): 辺上
				const	MgULine3&	ULn1,		// (I  ) 直線
				const	MgPolyg3&	Pg2,		// (I  ) 多角形
						MgGLine3	*GLn3		// (I O) 分割後の線分群
				)
{
	MINT		ist;
	MINT		ic1;
	MgPlane3	Pln2;
	MgLine3		Ln3;
	MgMat3		MatXY;							// Pln to XY平面 ３Ｄマトリックス
	MgMat3		MatPln;							// XY平面 to Pln ３Ｄマトリックス
	MgULine2	uln1;
	MgPolyg2	pg2;
	MgGLine2	Gln3;

	Pln2 = MgPlane3C( Pg2);

	// 底辺がX軸に平行になるようXY平面に投影マトリックスを求める
	Mat3PlntoXY( Pln2, Pg2.m_P[0], &MatXY, &MatPln);
	ULine3Dto2D( ULn1, MatXY, &uln1);
	Polyg3Dto2D( Pg2, MatXY, &pg2);
	ist = DivideULnPg2( Sel, uln1, pg2, &Gln3);

	if ( ist == (MC_INT | MC_MATCH)) {
		for ( ic1=0; ic1<Gln3.m_n; ic1++) {
			Line2Dto3D( Gln3.m_ln[ic1], MatPln, &Ln3);
			*GLn3 += Ln3;
		}
	}
	return ist;
}

//S //
//// ---------------------( ２次元 )------------------------------
////	２つの線分間の角度の大小に従う数値CCを求める
////	[p0からp1に向かう方向]から[p1からp2に向かう方向]への角度により
////		0.: -0゜, 0.5: 90゜, 1.: 180゜, 1.5: -90゜, <2.: <360(-)゜
////
//MREAL MGeo::AngSSa3Pt2( 
//				const	MgPoint2&	p0,
//				const	MgPoint2&	p1,
//				const	MgPoint2&	p2
//				)
//{
//	MgVect2	v1, v2;
//	MREAL	sp;
//	MREAL	ss, cc;
//	MREAL	sse, cce;
//
//	v1 = p1 - p0;
//	v2 = p2 - p1;
//	sp = MREAL( (MREAL)sqrt( (v1 * v1) * (v2 * v2)));					// MgAbs( v1) * MgAbs( v2)
//	ss = v1 ^ v2;												// sin( v1 v2) * sp
//	cc = v1 * v2 / sp;											// cos( v1 v2)
//
//	sse = ( ( cc > 0) ? ( ( ss > 0) ? sse = ss : sse = ss + 4) : sse = 2 - ss);
//	cce = ( ( ss > 0) ? cce = 1 - cc : cce = cc + 3);
//	return  ( sse + cce) / 4.f;
//}
////
//// ---------------------( ３次元 )------------------------------
////	２つの線分間の角度の大小に従う数値CCを求める
////	平面pln3上で[p0からp1に向かう方向]から[p1からp2に向かう方向]への角度により
////		0.: -0゜, 0.5: 90゜, 1.: 180゜, 1.5: -90゜, <2.: <360(-)゜
////
//MREAL MGeo::AngSSa3Pt3( 
//				const	MgPoint3&	P0,
//				const	MgPoint3&	P1,
//				const	MgPoint3&	P2,
//				const	MgPlane3	&Pln3
//				)
//{
//	MgVect3	V1, V2;
//	MREAL	sp;
//	MREAL	ss, cc;
//	MREAL	sse, cce;
//
//	V1 = P1 - P0;
//	V2 = P2 - P1;
//	sp = MREAL( (MREAL)sqrt( (V1 * V1) * (V2 * V2)));					// MgAbs( v1) * MgAbs( v2)
//	ss = (V1 ^ V2) * Pln3.v;									// sin( v1 v2) * sp
//	cc = V1 * V2 / sp;											// cos( v1 v2)
//
//	sse = ( ( cc > 0) ? ( ( ss > 0) ? sse = ss : sse = ss + 4) : sse = 2 - ss);
//	cce = ( ( ss > 0) ? cce = 1 - cc : cce = cc + 3);
//	return  ( sse + cce) / 4.f;
//}
////
//// ---------------------( ２次元 )------------------------------
////	２つの線分間の角度の大小に従う数値CCを求める
////	[p0からp1に向かう方向]から[p1からp2に向かう方向]への角度により
////		>-1.: >-180゜, -0.5: -90゜, 0.: -0゜, 0.5: 90゜, 1.: 180゜
////
//MREAL MGeo::AngSSb3Pt2( 
//				const	MgPoint2&	p0,
//				const	MgPoint2&	p1,
//				const	MgPoint2&	p2
//				)
//{
//	MgVect2	v1, v2;
//	MREAL	sp;
//	MREAL	ss, cc;
//	MREAL	sse, cce;
//
//	v1 = p1 - p0;
//	v2 = p2 - p1;
//	sp = MREAL( (MREAL)sqrt( (v1 * v1) * (v2 * v2)));					// MgAbs( v1) * MgAbs( v2)
//	ss = v1 ^ v2;												// sin( v1 v2) * sp
//	cc = v1 * v2 / sp;											// cos( v1 v2)
//
//	sse = ( ( cc < 0) ? ( ( ss > 0) ? sse = 2 - ss : sse = -2 - ss) : sse = ss);
//	cce = ( ( ss > 0) ? cc = 1 - cc : cc = -1 + cc);
//	return  ( sse + cce) / 4.f;
//}
////
//// ---------------------( ３次元 )------------------------------
////	２つの線分間の角度の大小に従う数値CCを求める
////	平面pln3上で[p0からp1に向かう方向]から[p1からp2に向かう方向]への角度により
////		>-1.: >-180゜, -0.5: -90゜, 0.: -0゜, 0.5: 90゜, 1.: 180゜
////
//MREAL MGeo::AngSSb3Pt3( 
//				const	MgPoint3&	P0,
//				const	MgPoint3&	P1,
//				const	MgPoint3&	P2,
//				const	MgPlane3	&Pln3
//				)
//{
//	MgVect3	V1, V2;
//	MREAL	sp;
//	MREAL	ss, cc;
//	MREAL	sse, cce;
//
//	V1 = P1 - P0;
//	V2 = P2 - P1;
//	sp = MREAL( (MREAL)sqrt( (V1 * V1) * (V2 * V2)));					// MgAbs( v1) * MgAbs( v2)
//	ss = (V1 ^ V2) * Pln3.v;									// sin( v1 v2) * sp
//	cc = V1 * V2 / sp;											// cos( v1 v2)
//
//	sse = ( ( cc < 0) ? ( ( ss > 0) ? sse = 2 - ss : sse = -2 - ss) : sse = ss);
//	cce = ( ( ss > 0) ? cc = 1 - cc : cc = -1 + cc);
//	return  ( sse + cce) / 4.f;
//}
//
// ---------------------( ２次元 )------------------------------
//	選択３角形が分割可能か調べる
//			 0: 面積が0
//			 1: 分割可能
//			-1: 三角形内に他の入隅点があり分割不可(３角形の辺上は対象外)
//
MINT MGeo::ChkDivTriPtPg2( 
						MgPoint2*	p3,			// 選択３角形
				const	MgPolyg2&	Pgw,		// 分割多角形
				const	MgGInt		&iGIrisumi	// 入隅点リスト
				)
{
	MgVect2		v12, v23, v31;
	MgVect2		v10, v20, v30;
	MREAL		ss10, ss20, ss30;
	MREAL		ss, cc;
	MINT		ic0;
	MgPoint2	p0;

	v12 = *(p3 + 1) - *p3;
	v23 = *(p3 + 2) - *(p3 + 1);
	ss = v12 ^ v23;												// sin( v12 v23) * |v12| * |v23|
	cc = v12 * v23;												// cos( v12 v23) + |v12| * |v23|
	if (ss * ss < g_gTol.A_2 * cc * cc)							// if (|sin( v12 v23)| < g_gTol.A)
		return 0;
	else {
		for ( ic0=0; ic0<iGIrisumi.m_n; ic0++) {
			v31 = *p3 - *(p3 + 2);
			p0 = Pgw.m_p[iGIrisumi[ic0]];
			v10 = p0 - *p3;
			v20 = p0 - *(p3 + 1);
			v30 = p0 - *(p3 + 2);
			if ((ss10 = (v12 ^ v10)) > 0 && (ss20 = (v23 ^ v20)) > 0 && (ss30 = (v31 ^ v30)) > 0)
				if (ss10 * ss10 > g_gTol.D_2 * (v10 * v10) &&
					ss20 * ss20 > g_gTol.D_2 * (v20 * v20) &&
					ss30 * ss30 > g_gTol.D_2 * (v30 * v30))
					return -1;
		}
	}
	return 1;
}

// ---------------------( ３次元 )------------------------------
//	選択３角形が分割可能か調べる
//			 0: 面積が0
//			 1: 分割可能
//			-1: 三角形上または内に他の入隅点があり分割不可(３角形の辺上は対象外)
//
MINT MGeo::ChkDivTriPtPg3( 
						MgPoint3	*p3,		// 選択３角形
				const	MgPolyg3&	Pgw,		// 分割多角形
				const	MgGInt		&iGIrisumi,	// 入隅点リスト
				const	MgVect3&	VuPg1		// 多角形の法線方向
				)
{
	MgVect3		v12, v23, v31;
	MgVect3		v10, v20, v30;
	MREAL		ss10, ss20, ss30;
	MREAL		ss, cc;
	MINT		ic0;
	MgPoint3	p0;

	v12 = *(p3 + 1) - *p3;
	v23 = *(p3 + 2) - *(p3 + 1);
	ss = (v12 ^ v23) * VuPg1;									// sin( v12 v23) * |v12| * |v23|
	cc = v12 * v23;												// cos( v12 v23) + |v12| * |v23|
	if ( ss * ss < g_gTol.D_2 * Abs(cc))
		return 0;
	else if (ss < 0)
		return -1;
	else {
		for ( ic0=0; ic0<iGIrisumi.m_n; ic0++) {
			p0 = Pgw.m_P[iGIrisumi[ic0]];
			v31 = *p3 - *(p3 + 2);
			v10 = p0 - *p3;
			v20 = p0 - *(p3 + 1);
			v30 = p0 - *(p3 + 2);
			if ((ss10 = MREAL((v12 ^ v10) * VuPg1)) > 0 &&
				(ss20 = MREAL((v23 ^ v20) * VuPg1)) > 0 &&
				(ss30 = MREAL((v31 ^ v30) * VuPg1)) > 0)
				if (ss10 * ss10 > g_gTol.D_2 * (v10 * v10) &&
					ss20 * ss20 > g_gTol.D_2 * (v20 * v20) &&
					ss30 * ss30 > g_gTol.D_2 * (v30 * v30))
					return -1;
		}
	}
	return 1;
}

// ---------------------( ２次元 )------------------------------
//		多角形を３角形に分割する
//
MINT MGeo::DivideTriPg2(							// (  O) ステイタス
												//			MC_NINT			(0): 分割なし
												//			MC_INT			(1): 分割あり
				const	MgPolyg2&	Pg1,		// (I  ) 多角形
						MgGPolyg2*	GPg2		// (  O) 分割後の３角形群
				)
{
	MINT		ist = 0;
	MINT		ist1;
	MINT		ic0, ic1;
	MINT		icm;
	MgVect2		v0, v1;
	MREAL		ss;
	MGPOLYG2( Pgw, MX_PNT1);					// 作業用多角形
	MINT		Pgwn;
	MGGINT( iGIrisumi, MX_PNT0);				// 入隅点リスト
	MGPOLYG2( Pg3, 3);							// 三角形
	Pg3.m_n = 0;

	GPg2->SetCount0();

	if (Pg1.m_n < 3)
		MQUIT;
	if (Pg1.m_n == 3) {
		(*GPg2) += Pg1;
		MQUIT;
	}
	// 入隅点を求め入隅点リストに追加する
	Pgw = Pg1;
	Pgw += Pg1[0];
	Pgw += Pg1[1];

	Pgwn = Pg1.m_n;
	for ( ic0=0; ic0<Pgwn; ic0++) {
		v0 = Pgw.m_p[ic0 + 1] - Pgw.m_p[ic0];
		v1 = Pgw.m_p[ic0 + 2] - Pgw.m_p[ic0 + 1];
		ss = v0 ^ v1;
		if (ss < 0)
			iGIrisumi += (ic0 + 1);								// 入隅点リストに追加 (1 ～ n)
	}
//							Pg1.Print( L"Pg1");
//							iGIrisumi.Print( L"iGIrisumi");

	// 入隅点リストの先頭の入隅点を分割対象頂点とする
	for (; iGIrisumi.m_n > 0; ) {
		// 反時計回転向きの連続する３点で３角形を作り検査
		ic0 = iGIrisumi.m_i[0];									// ic0: (1 ～ n)、(0 ～ n-1)
		if ( ic0 >= Pgwn)
			ic0 = 0;											// ic0: (1 ～ n-1 | 0)、(0 ～ n-1)
		ist1 = ChkDivTriPtPg2( &Pgw.m_p[ic0], Pgw, iGIrisumi);
		if ( ist1 == 0)											// 三角形の面積が０
			goto skip1;
		else if ( ist1 == -1) {									// 三角形内に他の入隅点あり
			// 時計回転向きの連続する３点で３角形を作り検査
			ic0 = iGIrisumi.m_i[0] - 2;							// ic0: (-1 ～ n-2)、(-2 ～ n-3)
			if ( ic0 < 0)
				ic0 += Pgwn;									// ic0: (n-1 | 0 ～ n-2)、(n-2 | n-1 | 0 ～ n-2)
			ist1 = ChkDivTriPtPg2( &Pgw.m_p[ic0], Pgw, iGIrisumi);
			if ( ist1 == 0)										// 三角形の面積が０
				goto skip1;
			else if ( ist1 == -1) {								// 三角形内に他の入隅点あり
				// 先頭入隅点を入隅点リストの末尾に格納
				ic0 = iGIrisumi.m_i[0];
				for ( ic1=0; ic1<iGIrisumi.m_n; ic1++)
					iGIrisumi.m_i[ic1] = iGIrisumi.m_i[ic1 + 1];
				iGIrisumi.m_i[iGIrisumi.m_n - 1] = ic0;
				continue;
			}
		}
		// 調査３角形を分割後の３角形群に追加
		Pg3.m_p[0] = Pgw.m_p[ic0];
		Pg3.m_p[1] = Pgw.m_p[ic0 + 1];
		Pg3.m_p[2] = Pgw.m_p[ic0 + 2];
		(*GPg2) += Pg3;
skip1:
		// ３角形分割による頂点を間引き
		ic1 = ic0 + 1;											// ic0: (1 ～ n)
		if ( ic1 >= Pgwn)
			ic1 -= Pgwn;										// ic1: (1 ～  n-1 | 0)
		icm = ic1;
		Pgwn --;
		Pgw.m_n = Pgwn;
		for ( ; ic1<Pgwn; ic1++)
			Pgw.m_p[ic1] = Pgw.m_p[ic1 + 1];
		Pgw += Pgw.m_p[0];
		Pgw += Pgw.m_p[1];
		for ( ic0=0; ic0<iGIrisumi.m_n; ic0++) {
			if ( iGIrisumi.m_i[ic0] > icm)
				iGIrisumi.m_i[ic0] --;
			if ( iGIrisumi.m_i[ic0] == 0)
				iGIrisumi.m_i[ic0] = Pgwn;
		}
		// 分割対象頂点が出隅になったなら入隅点リストから削除
		ic0 = iGIrisumi.m_i[0];
		v0 = Pgw.m_p[ic0] - Pgw.m_p[ic0 - 1];
		v1 = Pgw.m_p[ic0 + 1] - Pgw.m_p[ic0];
		ss = v0 ^ v1;
		if (ss > 0) {
			iGIrisumi.m_n --;
			for ( ic1=0; ic1<iGIrisumi.m_n; ic1++)
				iGIrisumi.m_i[ic1] = iGIrisumi.m_i[ic1 + 1];
		}
	}
	// 入隅なしなので任意の１点(先頭の点)で３角形分割
	Pg3[0] = Pgw.m_p[ic1];
	for ( ic1=1; ic1<Pgw.m_n-1; ic1++) {
		Pg3[1] = Pgw.m_p[ic1];
		Pg3[2] = Pgw.m_p[ic1 + 1];
		ist1 = ChkDivTriPtPg2( Pg3.m_p, Pgw, iGIrisumi);
		if ( ist != 0)											// 三角形の面積が０でない
			(*GPg2) += Pg3;
	}
	ist = 1;
exit:
	return ist;
}

// ---------------------( ３次元 )------------------------------
//		多角形を３角形に分割する
//
MINT MGeo::DivideTriPg3(						// (  O) ステイタス
												//			MC_NINT			(0): 分割なし
												//			MC_INT			(1): 分割あり
				const	MgPolyg3&	Pg1,		// (I  ) 多角形
				const	MgVect3&	VuPg1,		// (I  ) 多角形の法線方向
						MgGPolyg3*	gPg2		// (  O) 分割後の３角形群
				)
{
	MINT		ist = 0;
	MINT		ist1;
	MINT		ic0, ic1;
	MINT		icm;
	MgVect3		v0, v1;
	MREAL		ss;
	MGPOLYG3( Pgw, MX_PNT1);						// 作業用多角形
	MINT		Pgwn;
	MGGINT( iGIrisumi, MX_PNT0);					// 入隅点リスト
	MINT		iIrisumi;
	MGPOLYG3( Pg3, 3);								// 三角形
	Pg3.m_n = 3;
	MINT		iDel;

	gPg2->SetCount0();

	if (Pg1.m_n < 3)
		MQUIT;
	if (Pg1.m_n == 3) {
		(*gPg2) += Pg1;
		MQUIT;
	}
	// 入隅点を求め入隅点リストに追加する
	Pgw = Pg1;
	Pgw += Pg1[0];												
	Pgw += Pg1[1];

	Pgwn = Pg1.m_n;
	for ( ic0=0; ic0<Pgwn; ic0++) {
		v0 = Pgw.m_P[ic0 + 1] - Pgw.m_P[ic0];
		v1 = Pgw.m_P[ic0 + 2] - Pgw.m_P[ic0 + 1];
		ss = (v0 ^ v1) * VuPg1;
		if (ss < 0)
			iGIrisumi += (ic0 + 1);								// 入隅点リストに追加 (n-1 | 0 ～ n-2)
	}
//								Pg1.Print( L"Pg1");
//								iGIrisumi.Print( L"first入隅点_iGIrisumi");

	// 入隅点リストの先頭の入隅点を分割対象頂点とする
	for ( ; iGIrisumi.m_n > 0; ) {
		// 続く３点（反時計回転向きの連続する３点）で３角形を作り検査
		ic0 = iGIrisumi.m_i[0];									// 先頭入隅点
		if ( ic0 >= Pgwn)
			ic0 = 0;											// ic0: 三角形の開始位置（先頭入隅点）
		ist1 = ChkDivTriPtPg3( &Pgw.m_P[ic0], Pgw, iGIrisumi, VuPg1);
		if ( ist1 == 0)											// 三角形の面積が０の場合は間引き
		{
//								MBTRCPRINTI( L"三角形の面積が0でskip_ic0", ic0);
			goto skip1;
			}
		else if ( ist1 == -1) {									// 三角形内に他の入隅点あり
			// 手前の３点（時計回転向きの連続する３点）で３角形を作り検査
			ic0 = iGIrisumi.m_i[0] - 2;							// ic0: 三角形の開始位置（先頭入隅点を３点目とした開始位置）
			if ( ic0 < 0)
				ic0 += Pgwn;									//
			ist1 = ChkDivTriPtPg3( &Pgw.m_P[ic0], Pgw, iGIrisumi, VuPg1);
			if ( ist1 == 0)										// 三角形の面積が０の場合は間引き
			{
//										MBTRCPRINTI( L"三角形の面積が0でskip_ic0", ic0);
				goto skip1;
			}
			else if ( ist1 == -1) {								// 三角形内に他の入隅点あり
				// 先頭入隅点を入隅点リストの末尾に格納
				iIrisumi = iGIrisumi.m_i[0];
				for ( ic1=0; ic1<iGIrisumi.m_n-1; ic1++)
					iGIrisumi.m_i[ic1] = iGIrisumi.m_i[ic1 + 1];
				iGIrisumi.m_i[iGIrisumi.m_n - 1] = iIrisumi;
				continue;
			}
		}
		// 調査３角形を分割後の３角形群に追加
		Pg3.m_P[0] = Pgw.m_P[ic0];
		Pg3.m_P[1] = Pgw.m_P[ic0 + 1];
		Pg3.m_P[2] = Pgw.m_P[ic0 + 2];
		(*gPg2) += Pg3;
//										MBTRCPRINTI( Mstr( "分割開始点_ic0"), ic0);
//										Pg3.Print( Mstr( "分割３角形_Pg3"));
skip1:
		// ３角形分割による頂点を間引き
		ic1 = ic0 + 1;											// ic1: 間引き点　ic0: (1 ～ n)
		if ( ic1 >= Pgwn)
			ic1 -= Pgwn;										// ic1: (1 ～  n-1 | 0)
		icm = ic1;
//										MBTRCPRINTI( Mstr( "間引き点_icm"), icm);
		Pgwn --;
		Pgw.m_n = Pgwn;
		for ( ; ic1<Pgwn; ic1++)
			Pgw.m_P[ic1] = Pgw.m_P[ic1 + 1];
		Pgw += Pgw.m_P[0];
		Pgw += Pgw.m_P[1];

		iDel = 0;
		for ( ic0=0; ic0<iGIrisumi.m_n; ic0++) {
			if ( iGIrisumi.m_i[ic0] == icm) {
				iDel = 1;
			} else if ( iGIrisumi.m_i[ic0] > icm) {
				iGIrisumi.m_i[ic0 - iDel] = iGIrisumi.m_i[ic0] - 1;
			} else {
				iGIrisumi.m_i[ic0 - iDel] = iGIrisumi.m_i[ic0];
			}
//			if ( iGIrisumi.i[ic0] == 0)
//				iGIrisumi.i[ic0] = Pgwn;
		}
		iGIrisumi.m_n -= iDel;

		// 分割対象頂点が出隅になったなら入隅点リストから削除
		ic0 = iGIrisumi.m_i[0];
		v0 = Pgw.m_P[ic0] - Pgw.m_P[ic0 - 1];
		v1 = Pgw.m_P[ic0 + 1] - Pgw.m_P[ic0];
		ss = (v0 ^ v1) * VuPg1;
		if (ss > 0) {
			iGIrisumi.m_n --;
			for ( ic1=0; ic1<iGIrisumi.m_n; ic1++)
				iGIrisumi.m_i[ic1] = iGIrisumi.m_i[ic1 + 1];
		}
//										Pgw.Print( Mstr( "分割多角形_Pgw"));
//										iGIrisumi.Print( Mstr( "入隅点_iGIrisumi"));
//										MBTRCFLUSH;
	}
	// 入隅なしなので任意の１点(先頭の点)で３角形分割
	Pg3[0] = Pgw.m_P[0];
	Pgw.m_n = Pgwn;
	for ( ic1=1; ic1<Pgw.m_n-1; ic1++) {
		Pg3[1] = Pgw.m_P[ic1];
		Pg3[2] = Pgw.m_P[ic1 + 1];
		ist1 = ChkDivTriPtPg3( Pg3.m_P, Pgw, iGIrisumi, VuPg1);
		if ( ist1 != 0)										// 三角形の面積が０でない
			(*gPg2) += Pg3;
	}
	ist = 1;
exit:
	return ist;
}

} // namespace MC

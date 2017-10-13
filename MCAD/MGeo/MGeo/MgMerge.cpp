//==========================================================================================
//	Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//	MODULE: .cpp
//
//		
//
//
//	K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"

#define	DLL_EXPORT_GEO_DO

#include "MgLib.h"
#include "MgMat.h"

namespace MC
{

////
//// ---------------------( ２次元 )------------------------------
////		指定線分より始まり線分群の線分を結合して最小の多角形を作成する
////
//bool MGeo::MakeGPg2FromLn2InGLn2(			// (  O) ステイタス　true: 正常作成　false: 作成不可
//				const	MgLine2&	ln1,		// (I  ) 指定線分
//						MgGLine2*	Gln2,		// (I  ) 結合する線分群
//						MINT*		nHk,		// (I O) 結合する線分の中の正しい方向を持つ線分数（先頭より幾つかを表す）
//						MgPolyg2*	pg3			// (  O) 領域
//				);

//
// ---------------------( ２次元 )------------------------------
//		多角形を直線で切り、直線の指定側の領域を得る			<多角形が穴の場合の考慮不足?>
//
MINT MGeo::DividePgULn2(						// (  O) ステイタス
												//			MC_NINT			(0): 交差なし
												//			MC_INT			(1): 交差あり
						MINT		i_Sel,		// (I  ) 選択条件
												//			MC_LEFT			(010): 左側の領域
												//			MC_RIGHT		(040): 右側の領域
				const	MgPolyg2&	i_pg1,		// (I  ) 多角形
				const	MgULine2&	i_uln2,		// (I  ) 直線
						MgGPolyg2*	o_pGpg3		// (  O) 穴付き多角形群
				)
{
	MINT		ist = MC_NINT;
	MINT		ist1;
	MINT		ic1, icb;
	MgLine2		lnw1;
	MgULine2	uln2;
	MINT		sels;
	MgVect2		vULn2;
	
	MINT		icls;							// 1: 多角形が直線と交差するか、直線の非選択側にありで、多角形の辺に削除部分がある

	MGGPOINT2( GPt, MX_PNT1);					// 直線と線分の分割作業用点エリア
	MGGLINE2( GLn1, MX_PNT1);					// 分割されたポリゴンの構成線分の線分群
	MGGLINE2( GLn2, MX_PNT2);					// 直線の分割作業用線分エリア
	MGGPOLYG2( GPgW1, MX_LIN1, MX_LIN0, MX_LIN0); // 直線の選択側のポリゴン群

	uln2.p  = i_uln2.p;
	if ( i_Sel == MC_LEFT)
		uln2.v = i_uln2.v;
	else
		uln2.v = - i_uln2.v;

	//	多角形の分割
	sels = MC_LEFT | MC_ON_LINE | MC_SAME_DIR;
	icls = 0;													// 多角形と直線との交差有無 (0:無し、1:有り)

	GLn1.m_n = 0;
	for ( icb=i_pg1.m_n-1,ic1=0; ic1<i_pg1.m_n; icb=ic1,icb=ic1,ic1++) {
		lnw1 = MgLine2( i_pg1.m_p[icb], i_pg1.m_p[ic1]);
		ist1 = DivideAddLnULn2( sels, lnw1, uln2, &GLn1);
		if ( ist1 != (MC_NINT | MC_MATCH))						// 多角形の辺に削除部分あり
			 icls = 1;
	}

	if (GLn1.m_n > 0) {											// 直線の指定側に多角形の辺があり
		if ( icls == 0) {
			GPgW1 += i_pg1;										// 交差無しならそのまま残す
		} else {
			GLn2 += GLn1;										// 交差ありなら直線の指定側の線分のみ、新規領域作成用線分群に追加する
			MF_SET_ON( ist, MC_INT);
		}
		MF_SET_ON( ist, MC_MATCH);
	}
//
//	直線の分割

	ist1 = DivideAddULnPg2( MC_IN_BORDER, uln2, i_pg1, &GLn2);
	
	ist |= ist1;
//
	MakeGPg2InGLn2( GLn2, o_pGpg3);								// GLn2(正しい方向を持っている線分群)より領域群を作成する
	
	*o_pGpg3 += GPgW1;												// 領域群を返す

	if ( MF_CHECK_ON( ist, MC_INT))
		MF_SET_OFF( ist, MC_NINT);								// 交差ありなら交差なしのフラグを消去

	return ist;
}
//
// ---------------------( ２次元 )------------------------------
//		穴付き多角形を直線で切り、直線の指定側の領域を得る
//
MINT MGeo::DivideGPgULn2(						// (  O) ステイタス
												//			MC_NINT			(0): 交差なし
												//			MC_INT			(1): 交差あり
						MINT		i_Sel,		// (I  ) 選択条件
												//			MC_LEFT			(010): 左側の領域
												//			MC_RIGHT		(040): 右側の領域
				const	MgGPolyg2&	i_Gpg1,		// (I  ) 穴付き多角形
				const	MgULine2&	i_uln2,		// (I  ) 直線
						MgGPolyg2*	o_pGpg3		// (  O) 穴付き多角形群
				)
{
	MINT		ist = MC_NINT;
	MINT		ist1;
	MINT		ic1, ic2, icb;
	MgLine2		lnw1;
	MINT		sels;
	MgULine2	uln2;
	
	MgPolyg2*	pg1;
	MINT		icls;

	MGGPOINT2( GPt, MX_PNT1);					// 直線と線分の分割作業用点エリア
	MGGLINE2( GLn1, MX_PNT1);					// 分割されたポリゴンの構成線分の線分群
	MGGLINE2( GLn2, MX_PNT2);					// 直線の分割作業用線分エリア
	MGGPOLYG2( GPgW1, MX_LIN1, MX_LIN0, MX_LIN0); // 直線の選択側のポリゴン群

	uln2.p  = i_uln2.p;
	if ( i_Sel == MC_LEFT)
		uln2.v = i_uln2.v;
	else
		uln2.v = - i_uln2.v;

	//	穴付き多角形の分割
																// 直線と穴付き多角形の全てのポリゴンとの交点を求める
	for ( ic1=0; ic1<i_Gpg1.m_n; ic1++) {
		pg1 = &(i_Gpg1.m_pg[ic1]);
		
		sels = MC_LEFT | MC_ON_BORDER | MC_SAME_DIR;
		icls = 0;												// ポリゴンと直線との交差有無 (0:無し、1:有り)

		//	多角形の分割
		GLn1.m_n = 0;
		for ( icb=pg1->m_n-1,ic2=0; ic2<pg1->m_n; icb=ic2,ic2++) {
			lnw1 = MgLine2( pg1->m_p[icb], pg1->m_p[ic2]);
			ist1 = DivideAddLnULn2( sels, lnw1, uln2, &GLn1);
			if ( ist1 != (MC_NINT | MC_MATCH))					// 多角形の辺に削除部分あり
				 icls = 1;
		}

		if (GLn1.m_n > 0) {										// 直線の指定側にあり
			if ( icls == 0) {
				GPgW1 += *pg1;									// 交差無しならそのまま残す
			} else {
				GLn2 += GLn1;									// 交差ありなら直線の指定側の線分のみ、新規領域作成用線分群に追加する
				MF_SET_ON( ist, MC_INT);
			}
			MF_SET_ON( ist, MC_MATCH);
		}
	}
//
//	直線の分割

	ist1 = DivideAddULnGPg2( MC_IN_BORDER, uln2, i_Gpg1, &GLn2);
	
	MF_SET_ON( ist, ist1);
//
	MakeGPg2InGLn2( GLn2, o_pGpg3);								// GLn2(正しい方向を持っている線分群)より領域群を作成する
	
	*o_pGpg3 += GPgW1;											// 領域群を返す

	if ( MF_CHECK_ON( ist, MC_INT))
		MF_SET_OFF( ist, MC_NINT);								// 交差ありなら交差なしのフラグを消去

	return ist;
}
//
// ---------------------( ２次元 )------------------------------
//		多角形1を多角形2で分割し、多角形2の指定側（内側または外側）の領域を得る
//
MINT MGeo::Divide2Pg2(							// (  O) ステイタス
												//			MC_NINT			(0): 交差なし
												//			MC_INT			(1): 交差あり
												//			MC_MATCH		(4): 選択条件に合う部分あり
												//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
												//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
												//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
						MINT		i_Sel,		// (I  ) 選択条件
												//			MC_IN_BORDER	(001): 内側
												//			MC_OUT_BORDER	(004): 外側
												//									
				const	MgPolyg2&	i_pg1,		// (I  ) 多角形1
				const	MgPolyg2&	i_pg2,		// (I  ) 多角形2
						MgGPolyg2*	o_pGpgo		// (  O) 穴付き多角形3
				)
{
	MINT	Selc;

	if ( i_Sel == MC_IN_BORDER)
		Selc = MC_AND;
	else if ( i_Sel == MC_OUT_BORDER)
		Selc = MC_DIFF;

	return Merge2PgtoGPg2( Selc, i_pg1, i_pg2, o_pGpgo);
}

//
// ---------------------( ２次元 )------------------------------
//		穴付き多角形を穴付き多角形で分割し、選択条件に合う領域を得る
//
MINT MGeo::Divide2GPg2(							// (  O) ステイタス
												//			MC_NINT			(0): 交差なし
												//			MC_INT			(1): 交差あり
												//			MC_MATCH		(4): 選択条件に合う部分あり
												//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
												//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
												//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
						MINT		i_Sel,		// (I  ) 選択条件
												//			MC_IN_BORDER	(001): 内側
												//			MC_OUT_BORDER	(004): 外側
				const	MgGPolyg2&	i_Gpg1,		// (I  ) 穴付き多角形1
				const	MgGPolyg2&	i_Gpg2,		// (I  ) 穴付き多角形2
						MgGPolyg2*	o_pGpgo		// (  O) 穴付き多角形3
				)
{
	MINT	Selc;

	if ( i_Sel == MC_IN_BORDER)
		Selc = MC_AND;
	else if ( i_Sel == MC_OUT_BORDER)
		Selc = MC_DIFF;

	return Merge2GPgtoGPg2( Selc, i_Gpg1, i_Gpg2, o_pGpgo);
}

//
// ---------------------( ３次元 )------------------------------
//		多角形1を多角形2で分割し、多角形2の指定側（内側または外側）の領域を得る
//
MINT MGeo::Divide2Pg3(							// (  O) ステイタス
												//			MC_NINT			(0): 交差なし
												//			MC_INT			(1): 交差あり
												//			MC_MATCH		(4): 選択条件に合う部分あり
												//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
												//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
												//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
						MINT		i_Sel,		// (I  ) 選択条件
												//			MC_IN_BORDER	(001): 内側
												//			MC_OUT_BORDER	(004): 外側
												//									
				const	MgPolyg3&	i_Pg1,		// (I  ) 多角形1
				const	MgPolyg3&	i_Pg2,		// (I  ) 多角形2
						MgGPolyg3*	o_pGPgo		// (  O) 穴付き多角形3
				)
{
	MINT	Selc;

	if ( i_Sel == MC_IN_BORDER)
		Selc = MC_AND;
	else if ( i_Sel == MC_OUT_BORDER)
		Selc = MC_DIFF;

	return Merge2PgtoGPg3( Selc, i_Pg1, i_Pg2, o_pGPgo);
}

//
// ---------------------( ３次元 )------------------------------
//		穴付き多角形を穴付き多角形で分割し、選択条件に合う領域を得る
//
MINT MGeo::Divide2GPg3(							// (  O) ステイタス
												//			MC_NINT			(0): 交差なし
												//			MC_INT			(1): 交差あり
												//			MC_MATCH		(4): 選択条件に合う部分あり
												//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
												//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
												//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
						MINT		i_Sel,		// (I  ) 選択条件
												//			MC_IN_BORDER	(001): 内側
												//			MC_OUT_BORDER	(004): 外側
				const	MgGPolyg3&	i_GPg1,		// (I  ) 穴付き多角形1
				const	MgGPolyg3&	i_GPg2,		// (I  ) 穴付き多角形2
						MgGPolyg3*	o_pGPgo		// (  O) 穴付き多角形3
				)
{
	MINT	Selc;

	if ( i_Sel == MC_IN_BORDER)
		Selc = MC_AND;
	else if ( i_Sel == MC_OUT_BORDER)
		Selc = MC_DIFF;

	return Merge2GPgtoGPg3( Selc, i_GPg1, i_GPg2, o_pGPgo);
}

//
// ---------------------( ２次元 )------------------------------
//		多角形と多角形の和、差、積の穴付き多角形を求める
//
MINT MGeo::Merge2PgtoGPg2(						// (  O) ステイタス
												//			MC_NINT			(0): 交差なし
												//			MC_INT			(1): 交差あり
												//			MC_MATCH		(4): 選択条件に合う部分あり
												//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
												//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
												//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
						MINT		i_Sel,		// (I  ) 選択条件
												//			MC_OR			(1): 和
												//			MC_DIFF			(2): 差
												//			MC_AND			(4): 積
				const	MgPolyg2&	i_pg1,		// (I  ) 多角形1
				const	MgPolyg2&	i_pg2,		// (I  ) 多角形2
						MgGPolyg2*	o_pGpgo		// (  O) 穴付き多角形3
				)
{
	MINT		ist, ist2;
	MINT		sel1, sel2;
	bool		fRev;

	MGGLINE2( GLnw, MX_PNT1);						// 分割されたポリゴンの構成線分の線分群
	MGGPOLYG2( GPgw, MX_LIN1, MX_PNT0, MX_PNT0);	// 選択側のポリゴン群
//
	// 線分分割で残す側を指定
	switch ( i_Sel)
	{
	case MC_OR:
		sel1 = MC_OUT_BORDER | MC_ON_BORDER | MC_SAME_DIR;
		sel2 = MC_OUT_BORDER;
		break;
	case MC_DIFF:
		sel1 = MC_OUT_BORDER | MC_ON_BORDER | MC_REV_DIR;
		sel2 = MC_IN_BORDER;
		break;
	case MC_AND:
		sel1 = MC_IN_BORDER | MC_ON_BORDER | MC_SAME_DIR;
		sel2 = MC_IN_BORDER;
		break;
	}
	fRev = ( i_Sel == MC_DIFF);
	//	多角形1の線分を多角形2で分割し指定側を残す
	o_pGpgo->SetCount0();
	ist  = DivideAdd2Pg2( sel1, false, i_pg1, i_pg2, &GLnw, o_pGpgo);
	ist2 = DivideAdd2Pg2( sel2, fRev, i_pg2, i_pg1, &GLnw, o_pGpgo);
	if (GLnw.m_n > 2) {
		MakeGPg2InGLn2( GLnw, &GPgw);							// 正しい方向を持った線分群より領域群を作成する
		*o_pGpgo += GPgw;										// 領域群を追加する
	}
//	
	MF_SET_ON( ist, ist2);
	if ( MF_CHECK_ON( ist, MC_INT))
		MF_SET_OFF( ist, MC_NINT);								// 交差ありなら交差なしのフラグを消去

	return ist;
}

//
// ---------------------( ２次元 )------------------------------
//		多角形と穴付き多角形の和、差、積の穴付き多角形を求める
//
MINT MGeo::MergePgGPgtoGPg2(						// (  O) ステイタス
												//			MC_NINT			(0): 交差なし
												//			MC_INT			(1): 交差あり
												//			MC_MATCH		(4): 選択条件に合う部分あり
												//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
												//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
												//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
						MINT		i_Sel,		// (I  ) 選択条件
												//			MC_OR			(1): 和
												//			MC_DIFF			(2): 差
												//			MC_AND			(4): 積
				const	MgPolyg2&	i_pg1,		// (I  ) 多角形1
				const	MgGPolyg2&	i_Gpg2,		// (I  ) 穴付き多角形2
						MgGPolyg2*	o_pGpgo		// (  O) 穴付き多角形3
				)
{
	MINT		ist;
	MINT		ist2;
	MINT		ic2;
	MINT		sel1, sel2;
	bool		fRev;
	MgPolyg2*	ppg2;

	MGGLINE2( GLnw, MX_PNT1);						// 分割されたポリゴンの構成線分の線分群
	MGGPOLYG2( GPgw, MX_LIN1, MX_PNT0, MX_PNT0);	// 選択側のポリゴン群
//
	// 線分分割で残す側を指定
	switch ( i_Sel)
	{
	case MC_OR:
		sel1 = MC_OUT_BORDER | MC_ON_BORDER | MC_SAME_DIR;
		sel2 = MC_OUT_BORDER;
		break;
	case MC_DIFF:
		sel1 = MC_OUT_BORDER | MC_ON_BORDER | MC_REV_DIR;
		sel2 = MC_IN_BORDER;
		break;
	case MC_AND:
		sel1 = MC_IN_BORDER | MC_ON_BORDER | MC_SAME_DIR;
		sel2 = MC_IN_BORDER;
		break;
	}
	fRev = ( i_Sel == MC_DIFF);

	o_pGpgo->SetCount0();
	//	多角形1の線分を穴付き多角形2で分割し指定側を残す
	ist = DivideAddPgGPg2( sel1, false, i_pg1, i_Gpg2, &GLnw, o_pGpgo);

	//	穴付き多角形2の線分を多角形1で分割し指定側を残す
	for ( ic2=0; ic2<i_Gpg2.m_n; ic2++) {
		ppg2 = &i_Gpg2.m_pg[ic2];
		ist2 = DivideAdd2Pg2( sel2, fRev, *ppg2, i_pg1, &GLnw, o_pGpgo);
		MF_SET_ON( ist, ist2);
	}
	if (GLnw.m_n > 2) {
		MakeGPg2InGLn2( GLnw, &GPgw);									// 正しい方向を持った線分群より領域群を作成する
		*o_pGpgo += GPgw;											// 領域群を追加する
	}

	if ( MF_CHECK_ON( ist, MC_INT))
		MF_SET_OFF( ist, MC_NINT);								// 交差ありなら交差なしのフラグを消去

	return ist;
}

//
// ---------------------( ２次元 )------------------------------
//		穴付き多角形と多角形の和、差、積の穴付き多角形を求める
//
MINT MGeo::MergeGPgPgtoGPg2(						// (  O) ステイタス
												//			MC_NINT			(0): 交差なし
												//			MC_INT			(1): 交差あり
												//			MC_MATCH		(4): 選択条件に合う部分あり
												//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
												//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
												//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
						MINT		i_Sel,		// (I  ) 選択条件
												//			MC_OR			(1): 和
												//			MC_DIFF			(2): 差
												//			MC_AND			(4): 積
				const	MgGPolyg2&	i_Gpg1,		// (I  ) 穴付き多角形1
				const	MgPolyg2&	i_pg2,		// (I  ) 多角形2
						MgGPolyg2*	o_pGpgo		// (  O) 穴付き多角形3
				)
{
	MINT		ist = MC_NINT;
	MINT		ist1, ist2;
	MINT		ic1;
	MINT		sel1, sel2;
	bool		fRev;
	MgPolyg2*	ppg1;

	MGGLINE2( GLnw, MX_PNT1);						// 分割されたポリゴンの構成線分の線分群
	MGGPOLYG2( GPgw, MX_LIN1, MX_PNT0, MX_PNT0);	// 選択側のポリゴン群
//
	// 線分分割で残す側を指定
	switch ( i_Sel)
	{
	case MC_OR:
		sel1 = MC_OUT_BORDER | MC_ON_BORDER | MC_SAME_DIR;
		sel2 = MC_OUT_BORDER;
		break;
	case MC_DIFF:
		sel1 = MC_OUT_BORDER | MC_ON_BORDER | MC_REV_DIR;
		sel2 = MC_IN_BORDER;
		break;
	case MC_AND:
		sel1 = MC_IN_BORDER | MC_ON_BORDER | MC_SAME_DIR;
		sel2 = MC_IN_BORDER;
		break;
	}
	fRev = ( i_Sel == MC_DIFF);

	o_pGpgo->SetCount0();
	//	穴付き多角形1の線分を多角形2で分割し指定側を残す
	for ( ic1=0; ic1<i_Gpg1.m_n; ic1++) {
		ppg1 = &i_Gpg1.m_pg[ic1];
		//	多角形1の線分を穴付き多角形2で分割し指定側を残す
		ist1 = DivideAdd2Pg2( sel1, false, *ppg1, i_pg2, &GLnw, o_pGpgo);
		MF_SET_ON( ist, ist1);
	}
	//	多角形2の線分を穴付き多角形1で分割し指定側を残す
	ist2 = DivideAddPgGPg2( sel2, fRev, i_pg2, i_Gpg1, &GLnw, o_pGpgo);
	MF_SET_ON( ist, ist2);

	if (GLnw.m_n > 2) {
		MakeGPg2InGLn2( GLnw, &GPgw);							// 正しい方向を持った線分群より領域群を作成する
		*o_pGpgo += GPgw;										// 領域群を追加する
	}

	if ( MF_CHECK_ON( ist, MC_INT))
		MF_SET_OFF( ist, MC_NINT);								// 交差ありなら交差なしのフラグを消去

	return ist;
}

//
// ---------------------( ２次元 )------------------------------
//		穴付き多角形と穴付き多角形の和、差、積の穴付き多角形を求める
//
MINT MGeo::Merge2GPgtoGPg2(						// (  O) ステイタス
												//			MC_NINT			(x01): 交差なし
												//			MC_INT			(x02): 交差あり
												//			MC_MATCH		(x10): 選択条件に合う部分あり
												//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
												//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
												//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
						MINT		i_Sel,		// (I  ) 選択条件
												//			MC_OR			(1): 和
												//			MC_DIFF			(2): 差
												//			MC_AND			(4): 積
				const	MgGPolyg2&	i_Gpg1,		// (I  ) 穴付き多角形1
				const	MgGPolyg2&	i_Gpg2,		// (I  ) 穴付き多角形2
						MgGPolyg2*	o_pGpgo		// (  O) 穴付き多角形3
				)
{
	MINT		ist = MC_NINT;
	MINT		ic1, ic2;
	MINT		ist1, ist2;
	MINT		sel1, sel2;
	bool		fRev;
	MgPolyg2*	ppg1;
	MgPolyg2*	ppg2;

	MGGLINE2( GLnw, MX_PNT1);					// 分割されたポリゴンの構成線分の線分群
	MGGPOLYG2( GPgw, MX_LIN1, MX_PNT0, MX_PNT0);// 選択側のポリゴン群
//
	// 線分分割で残す側を指定
	switch ( i_Sel)
	{
	case MC_OR:
		sel1 = MC_OUT_BORDER | MC_ON_BORDER | MC_SAME_DIR;
		sel2 = MC_OUT_BORDER;
		break;
	case MC_DIFF:
		sel1 = MC_OUT_BORDER | MC_ON_BORDER | MC_REV_DIR;
		sel2 = MC_IN_BORDER;
		break;
	case MC_AND:
		sel1 = MC_IN_BORDER | MC_ON_BORDER | MC_SAME_DIR;
		sel2 = MC_IN_BORDER;
		break;
	}
	fRev = ( i_Sel == MC_DIFF);
	o_pGpgo->SetCount0();
	//	穴付き多角形1の線分を穴付き多角形2で分割し指定側を残す
	for ( ic1=0; ic1<i_Gpg1.m_n; ic1++) {
		ppg1 = &i_Gpg1.m_pg[ic1];
		//	多角形1の線分を穴付き多角形2で分割し指定側を残す
		ist1 = DivideAddPgGPg2( sel1, false, *ppg1, i_Gpg2, &GLnw, o_pGpgo);
		MF_SET_ON( ist, ist1);
	}
	//	穴付き多角形2の線分を穴付き多角形1で分割し指定側を残す
	for ( ic2=0; ic2<i_Gpg2.m_n; ic2++) {
		ppg2 = &i_Gpg2.m_pg[ic2];
		//	多角形2の線分を穴付き多角形1で分割し指定側を残す
		ist2 = DivideAddPgGPg2( sel2, fRev, *ppg2, i_Gpg1, &GLnw, o_pGpgo);
		MF_SET_ON( ist, ist2);
	}
	if (GLnw.m_n > 2) {
		MakeGPg2InGLn2( GLnw, &GPgw);							// 正しい方向を持った線分群より領域群を作成する
		*o_pGpgo += GPgw;										// 領域群を返す
	}

	if ( MF_CHECK_ON( ist, MC_INT))
		MF_SET_OFF( ist, MC_NINT);								// 交差ありなら交差なしのフラグを消去

	return ist;
}
//
// ---------------------( ３次元 )------------------------------
//		多角形と多角形の和、差、積の穴付き多角形を求める
//
MINT MGeo::Merge2PgtoGPg3(						// (  O) ステイタス
												//			MC_NINT			(0): 交差なし
												//			MC_INT			(1): 交差あり
												//			MC_MATCH		(4): 選択条件に合う部分あり
												//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
												//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
												//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
						MINT		i_Sel,		// (I  ) 選択条件
												//			MC_OR			(1): 和
												//			MC_DIFF			(2): 差
												//			MC_AND			(4): 積
				const	MgPolyg3&	i_Pg1,		// (I  ) 多角形1
				const	MgPolyg3&	i_Pg2,		// (I  ) 多角形2
						MgGPolyg3*	o_pGPgo		// (  O) 穴付き多角形3
				)
{
	MINT		ist = MC_NINT;
	MgPlane3	Pln1, Pln2;
	MgMat3		MatXY;							// Pln to XY平面 ３Ｄマトリックス
	MgMat3		MatPln;							// XY平面 to Pln ３Ｄマトリックス

	MGPOLYG2( pg1, MX_PNT0);
	MGPOLYG2( pg2, MX_PNT0);
	MGGPOLYG2( gpg3, MX_LIN1, MX_PNT0, MX_PNT0);

	Pln1 = MgPlane3C( i_Pg1);									// 多角形1の平面係数を取り出す
	Pln2 = MgPlane3C( i_Pg2);									// 多角形2の平面係数を取り出す
	if (Pln1 != Pln2)
		MQUIT;													// 同一平面上に無い

	Mat3PlntoXY( Pln1, i_Pg1.m_P[0], &MatXY, &MatPln);			// ３Ｄ→２Ｄ用変換マトリックスと逆変換マトリックスを求める
	Polyg3Dto2D( i_Pg1, MatXY, &pg1);							// 多角形1を２次元に変換する
	Polyg3Dto2D( i_Pg2, MatXY, &pg2);							// 多角形2を２次元に変換する

	ist = Merge2PgtoGPg2( i_Sel, pg1, pg2, &gpg3);
	GPolyg2Dto3D( gpg3, MatPln, o_pGPgo);
exit:
	return ist;
}

//
// ---------------------( ３次元 )------------------------------
//		多角形と穴付き多角形の和、差、積の穴付き多角形を求める
//
MINT MGeo::MergePgGPgtoGPg3(					// (  O) ステイタス
												//			MC_NINT			(0): 交差なし
												//			MC_INT			(1): 交差あり
												//			MC_MATCH		(4): 選択条件に合う部分あり
												//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
												//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
												//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
						MINT		i_Sel,		// (I  ) 選択条件
												//			MC_OR			(1): 和
												//			MC_DIFF			(2): 差
												//			MC_AND			(4): 積
				const	MgPolyg3&	i_Pg1,		// (I  ) 多角形1
				const	MgGPolyg3&	i_GPg2,		// (I  ) 穴付き多角形2
						MgGPolyg3*	o_pGPgo		// (  O) 穴付き多角形3
				)
{
	MINT		ist = MC_NINT;
	MgPlane3	Pln1, Pln2;
	MgMat3		MatXY;							// Pln to XY平面 ３Ｄマトリックス
	MgMat3		MatPln;							// XY平面 to Pln ３Ｄマトリックス

	MGPOLYG2( pg1, MX_PNT0);
	MGGPOLYG2( gpg2, MX_LIN1, MX_PNT0, MX_PNT0);
	MGGPOLYG2( gpg3, MX_LIN1, MX_PNT0, MX_PNT0);
//

	Pln1 = MgPlane3C( i_Pg1);										// 多角形1の平面係数を取り出す
	Pln2 = MgPlane3C( i_GPg2.m_Pg[0]);								// 穴付き多角形2の平面係数を取り出す
	if (Pln1 != Pln2)
		MQUIT;													// 同一平面上に無い

	Mat3PlntoXY( Pln1, i_Pg1.m_P[0], &MatXY, &MatPln);			// ３Ｄ→２Ｄ用変換マトリックスと逆変換マトリックスを求める
	Polyg3Dto2D( i_Pg1, MatXY, &pg1);							// 穴付き多角形1を２次元に変換する
	GPolyg3Dto2D( i_GPg2, MatXY, &gpg2);						// 穴付き多角形2を２次元に変換する

	ist = MergePgGPgtoGPg2( i_Sel, pg1, gpg2, &gpg3);
	GPolyg2Dto3D( gpg3, MatPln, o_pGPgo);
exit:
	return ist;
}

//
// ---------------------( ３次元 )------------------------------
//		穴付き多角形と多角形の和、差、積の穴付き多角形を求める
//
MINT MGeo::MergeGPgPgtoGPg3(						// (  O) ステイタス
												//			MC_NINT			(0): 交差なし
												//			MC_INT			(1): 交差あり
												//			MC_MATCH		(4): 選択条件に合う部分あり
												//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
												//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
												//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
						MINT		i_Sel,		// (I  ) 選択条件
												//			MC_OR			(1): 和
												//			MC_DIFF			(2): 差
												//			MC_AND			(4): 積
				const	MgGPolyg3&	i_GPg1,		// (I  ) 穴付き多角形1
				const	MgPolyg3&	i_Pg2,		// (I  ) 多角形2
						MgGPolyg3*	o_pGPgo		// (  O) 穴付き多角形3
				)
{
	MINT		ist = MC_NINT;
	MgPlane3	Pln1, Pln2;
	MgMat3		MatXY;							// Pln to XY平面 ３Ｄマトリックス
	MgMat3		MatPln;							// XY平面 to Pln ３Ｄマトリックス
	MgMat3		Dbg1;
	MgMat3		Dbg2;
	MGGPOLYG2( gpg1, MX_LIN1, MX_PNT0, MX_PNT0);
	MGPOLYG2( pg2, MX_PNT0);
	MGGPOLYG2( gpg3, MX_LIN1, MX_PNT0, MX_PNT0);
//
	Pln1 = MgPlane3C( i_GPg1.m_Pg[0]);							// 穴付き多角形1の平面係数を取り出す
	Pln2 = MgPlane3C( i_Pg2);									// 多角形2の平面係数を取り出す
	if (Pln1 != Pln2)
		MQUIT;													// 同一平面上に無い

	Mat3PlntoXY( Pln1, i_GPg1.m_Pg[0].m_P[0], &MatXY, &MatPln);	// ３Ｄ→２Ｄ用変換マトリックスと逆変換マトリックスを求める
	Dbg1 = MatXY * MatPln;
	Dbg2 = MatPln * MatXY;
	GPolyg3Dto2D( i_GPg1, MatXY, &gpg1);						// 穴付き多角形1を２次元に変換する
	Polyg3Dto2D( i_Pg2, MatXY, &pg2);							// 穴付き多角形2を２次元に変換する

	ist = MergeGPgPgtoGPg2( i_Sel, gpg1, pg2, &gpg3);
	GPolyg2Dto3D( gpg3, MatPln, o_pGPgo);
exit:
	return ist;
}

//
// ---------------------( ３次元 )------------------------------
//		穴付き多角形と穴付き多角形の和、差、積の穴付き多角形を求める
//
MINT MGeo::Merge2GPgtoGPg3(						// (  O) ステイタス
												//			MC_NINT			(0): 交差なし
												//			MC_INT			(1): 交差あり
												//			MC_MATCH		(4): 選択条件に合う部分あり
												//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
												//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
												//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
						MINT		i_Sel,		// (I  ) 選択条件
												//			MC_OR			(1): 和
												//			MC_DIFF			(2): 差
												//			MC_AND			(4): 積
				const	MgGPolyg3&	i_GPg1,		// (I  ) 穴付き多角形1
				const	MgGPolyg3&	i_GPg2,		// (I  ) 穴付き多角形2
						MgGPolyg3*	o_pGPgo		// (  O) 穴付き多角形3
				)
{
	MINT		ist = MC_NINT;
	MgPlane3	Pln1, Pln2;
	MgMat3		MatXY;								// Pln to XY平面 ３Ｄマトリックス
	MgMat3		MatPln;								// XY平面 to Pln ３Ｄマトリックス

	MGGPOLYG2( gpg1, MX_LIN1, MX_PNT0, MX_PNT0);
	MGGPOLYG2( gpg2, MX_LIN1, MX_PNT0, MX_PNT0);
	MGGPOLYG2( gpg3, MX_LIN1, MX_PNT0, MX_PNT0);
//

	Pln1 = MgPlane3C( i_GPg1.m_Pg[0]);							// 穴付き多角形1の平面係数を取り出す
	Pln2 = MgPlane3C( i_GPg2.m_Pg[0]);							// 穴付き多角形2の平面係数を取り出す
	if (Pln1 != Pln2)
		MQUIT;													// 同一平面上に無い

	Mat3PlntoXY( Pln1, i_GPg1.m_Pg[0].m_P[0], &MatXY, &MatPln);		// ３Ｄ→２Ｄ用変換マトリックスと逆変換マトリックスを求める
	GPolyg3Dto2D( i_GPg1, MatXY, &gpg1);								// 穴付き多角形1を２次元に変換する
	GPolyg3Dto2D( i_GPg2, MatXY, &gpg2);								// 穴付き多角形2を２次元に変換する

	ist = Merge2GPgtoGPg2( i_Sel, gpg1, gpg2, &gpg3);
	GPolyg2Dto3D( gpg3, MatPln, o_pGPgo);
exit:
	return ist;
}

//
// ---------------------( ２次元 )------------------------------
//		多角形の辺（線分）を他の多角形で分割し、指定した側の線分を残す
//								線分が多角形の辺上にある場合は、
//				(1) 選択条件が辺上同一方向の場合は、同一方向の時残し
//				(2) 選択条件が辺上逆方向の場合は、逆方向の時残す
//				(3) その他の場合は捨てる
//
MINT MGeo::DivideAdd2Pg2(							// (  O) ステイタス
													//			MC_NINT			(0): 交差なし
													//			MC_INT			(1): 交差あり
													//			MC_MATCH		(4): 選択条件に合う部分あり
													//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
													//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
													//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
						MINT		i_Sel1,			// (I  ) 選択条件1　多角形1の線分を残す条件
													//			MC_IN_BORDER	(001):	多角形2の領域の内側
													//			MC_ON_BORDER	(002):  多角形2の辺上
													//			MC_OUT_BORDER	(004):	多角形2の領域の外側
													//			MC_SAME_DIR		(0100):	辺上同一方向
													//			MC_REV_DIR		(0200):	辺上逆方向
				const	bool		i_fRev,			// (I  ) 反転フラグ	
													//		 true: 線分の方向を反転する
				const	MgPolyg2&	i_pg1,			// (I  ) 多角形1
				const	MgPolyg2&	i_pg2,			// (I  ) 多角形2
						MgGLine2*	io_pGlno,		// (I O) 直線群
						MgGPolyg2*	io_pGpgo		// (I O) 穴付き多角形
				)
{
	MINT		ist = MC_NINT;
	MINT		ist1;
	MINT		icb, ic1;
	MgLine2		lnw1;

	bool		fcls;								// ポリゴンと直線との交差有無 (0:無し、1:有り)
	
	MGGLINE2( GLnw, MX_PNT1);						// 分割されたポリゴンの構成線分の線分群
//
	//	多角形1の線分を多角形2で分割し指定側を残す
	fcls = false;												// 多角形1の線分と多角形2との交差有無 (0:無し、1:有り)
	for ( icb=i_pg1.m_n-1,ic1=0; ic1<i_pg1.m_n; icb=ic1,ic1++) {
		lnw1 = MgLine2( i_pg1.m_p[icb], i_pg1.m_p[ic1]);		// 線分を多角形で切る
		ist1 = DivideAddLnPg2( i_Sel1, lnw1, i_pg2, &GLnw);
		if (!MF_CHECK_AND( ist1, (MC_NINT | MC_MATCH)))			// 多角形の辺に削除部分あり(交差または選択範囲外側)
			 fcls = true;
	}

	if (GLnw.m_n > 0) {											// 多角形2の指定側に多角形1の辺があり
		if (fcls) {
			if ( i_fRev) GLnw.Rev();
			*io_pGlno += GLnw;									// 交差ありなら線分の指定側の線分のみ、新規領域作成用線分群に追加する
			ist = MC_INT;
		} else {
			*io_pGpgo += i_pg1;									// 交差無しならその多角形のまま残す
			if ( i_fRev) io_pGpgo->m_pg[io_pGpgo->m_n - 1].Rev();
		}
		MF_SET_ON( ist, MC_MATCH);
	}


	return ist;
}

//
// ---------------------( ２次元 )------------------------------
//		多角形の辺（線分）を穴付き多角形で分割し、指定した側の線分を残す
//								線分が多角形の辺上にある場合は、
//				(1) 選択条件が辺上同一方向の場合は、同一方向の時残し
//				(2) 選択条件が辺上逆方向の場合は、逆方向の時残す
//				(3) その他の場合は捨てる
//

MINT MGeo::DivideAddPgGPg2(							// (  O) ステイタス
													//			MC_NINT			(0): 交差なし
													//			MC_INT			(1): 交差あり
													//			MC_MATCH		(4): 選択条件に合う部分あり
													//	例	 (STS == MC_NINT):				交差無しで選択条件に合う部分なし 
													//		 (STS == (MC_NINT | MC_MATCH)):	交差無しであるが選択条件に合う
													//		 (STS == (MC_INT | MC_MATCH)):	交差ありで選択条件に合う部分あり
						MINT		i_Sel1,			// (I  ) 選択条件1　多角形1の線分を残す条件
													//			MC_IN_BORDER	(001): 多角形2の領域の内側
													//			MC_ON_BORDER	(002): 多角形2の辺上
													//			MC_OUT_BORDER	(004): 多角形2の領域の外側
													//			MC_SAME_DIR		(0100):	辺上同一方向
													//			MC_REV_DIR		(0200):	辺上逆方向
				const	bool		i_fRev,			// (I  ) 反転フラグ	
													//		 true: 線分の方向を反転する
				const	MgPolyg2&	i_pg1,			// (I  ) 多角形1
				const	MgGPolyg2&	i_Gpg2,			// (I  ) 穴付き多角形2
						MgGLine2*	io_pGlno,		// (I O) 直線群
						MgGPolyg2*	io_pGpgo		// (I O) 穴付き多角形
				)
{
	MINT		ist = MC_NINT;
	MINT		ist1;
	MINT		icb, ic1;
	MgLine2		lnw1;

	bool		fcls;								// ポリゴンと直線との交差有無 (0:無し、1:有り)
	
	MGGLINE2( GLnw, MX_PNT1);						// 分割されたポリゴンの構成線分の線分群
//
	//	多角形1の線分を穴付き多角形2で分割し指定側を残す
	fcls = false;												// 多角形1の線分と穴付き多角形2との交差有無 (0:無し、1:有り)
	for ( icb=i_pg1.m_n-1,ic1=0; ic1<i_pg1.m_n; icb=ic1,ic1++) {
		lnw1 = MgLine2( i_pg1.m_p[icb], i_pg1.m_p[ic1]);		// 線分を穴付き多角形で切る
		ist1 = DivideAddLnGPg2( i_Sel1, lnw1, i_Gpg2, &GLnw);
		if (!MF_CHECK_AND( ist1, (MC_NINT | MC_MATCH)))			// 多角形1の辺に削除部分あり(交差または選択範囲外側)
			 fcls = true;
	}

	if (GLnw.m_n > 0) {											// 穴付き多角形2の指定側に多角形1の辺があり
		if (fcls) {
			if ( i_fRev) GLnw.Rev();
			*io_pGlno += GLnw;									// 交差ありなら線分の指定側の線分のみ、新規領域作成用線分群に追加する
			ist = MC_INT;
		} else {
			*io_pGpgo += i_pg1;									// 交差無しならその多角形のまま残す
			if ( i_fRev) io_pGpgo->m_pg[io_pGpgo->m_n - 1].Rev();
		}
		MF_SET_ON( ist, MC_MATCH);
	}
	return ist;
}

//
// ---------------------( ２次元 )------------------------------
//		GLn1の方向を考慮して接続し領域群を作成する
//

#define	MX_AREA_BY_PT	20						// １点の周りの最大領域数

void MGeo::MakeGPg2InGLn2(
				const	MgGLine2&	i_Gln1,		// (I  ) 正しい方向を持つ線分群
						MgGPolyg2*	o_pGpg3		// (  O) 領域群
				)
{
	bool		bst1;
	MINT		ic1;
	MgLine2		Lnh1;
	MINT		nHk;								// 正しい方向を持つ線分数

	MBMALCGLINE2(	gLnW1,	i_Gln1.m_n,		MX_PNT2);			// 線分作業領域
	MGPOLYG2(		pgW1,	MX_PNT1);							// 多角形頂点

	o_pGpg3->SetCount0();
	gLnW1 = i_Gln1;

	for (;gLnW1.m_n > 0;)	{
		MgLine2	Lnh1 = gLnW1.m_ln[0];							// 線分作業領域より最初の１本を取り出す
		for ( ic1=1; ic1<gLnW1.m_n; ic1++)						// 線分作業領域からその線分を間引く
			gLnW1.m_ln[ic1-1] = gLnW1.m_ln[ic1];
		gLnW1.m_n--;
		nHk = gLnW1.m_n;
		bst1 = MakeGPg2FromLn2InGLn2( Lnh1, &gLnW1, &nHk, &pgW1);
		if (bst1)
			(*o_pGpg3) += pgW1;
	}
}

//
// ---------------------( ２次元 )------------------------------
//		GLn1の方向を考慮して直線分割線分GLn2を追加し領域群を作成する
//
void MGeo::MakeGPg2In2GLn2(
				const	MgGLine2&	i_Gln1,		// (I  ) 正しい方向を持つ線分群
				const	MgGLine2&	i_Gln2,		// (I  ) 不正確な方向の線分群
						MgGPolyg2*	o_Gpg3		// (  O) 領域群
				)
{
	bool		bst;
	MINT		ic1;
	MgLine2		Lnh1;
	MINT		nHk;								// 正しい方向を持つ線分数

	MBMALCGLINE2( GLnW1, i_Gln1.m_n+i_Gln2.m_n, MX_PNT2);		// 線分作業領域
	MGPOLYG2( PgW1,	MX_PNT1);									// 多角形頂点

	o_Gpg3->SetCount0();
	GLnW1 = i_Gln1;
	GLnW1 += i_Gln2;
	nHk = i_Gln1.m_n;

	MINT	ifstart = 1;
	for (;!ifstart || nHk>0;)	{
		MgLine2 Lnh1 = GLnW1.m_ln[0];							// 線分作業領域より最初の１本を取り出す
		for ( ic1=1; ic1<GLnW1.m_n; ic1++)						// 線分作業領域からその線分を間引く
			GLnW1.m_ln[ic1-1] = GLnW1.m_ln[ic1];
		GLnW1.m_n--;
		nHk--;
		bst = MakeGPg2FromLn2InGLn2( Lnh1, &GLnW1, &nHk, &PgW1);
		if (bst)
			(*o_Gpg3) += PgW1;
	}
}

//
// ---------------------( ２次元 )------------------------------
//		指定線分より始まり線分群の線分を交点で分割後に結合して最小の多角形を作成する
//
bool MGeo::MakeGPg2FromLn2InGLn2(							// (  O) ステイタス　true: 正常作成　false: 作成不可
				const	MgLine2&	i_ln1,		// (I  ) 指定線分
				const	MgGLine2&	i_Gln2,		// (I  ) 結合する線分群
						MgPolyg2*	o_ppg3		// (  O) 領域
				)
{
	bool		bst;
	MINT		nHk;
	MgGLine2	GLn2t;

	MBMALCGLINE2( GLnW1, i_Gln2.m_n * 2, MX_PNT2);				// 線分作業領域
	MGPOLYG2( PgW1,	MX_PNT1);									// 多角形頂点

	o_ppg3->m_n = 0;
	DivideGLn2( i_Gln2, &GLnW1);
	nHk = 0;
	GLn2t = i_Gln2;

	bst = MakeGPg2FromLn2InGLn2( i_ln1, &GLnW1, &nHk, &PgW1);
	if (bst)
		*o_ppg3 = PgW1;
	return bst;
}

//
// ---------------------( ２次元 )------------------------------
//		指定線分より始まり線分群の線分を結合して最小の多角形を作成する
//
bool MGeo::MakeGPg2FromLn2InGLn2(				// (  O) ステイタス　true: 正常作成　false: 作成不可
				const	MgLine2&	i_ln1,		// (I  ) 指定線分
						MgGLine2*	io_Gln2,	// (I O) 結合する線分群
						MINT*		io_nHk,		// (I O) 結合する線分の中の正しい方向を持つ線分数（先頭より幾つかを表す）
						MgPolyg2*	o_pg3		// (  O) 領域
				)
{
	bool		bst;
	MINT		ic1, ic2;
	MINT		nc1;
	MgPoint2	PtF, PtS, PtE;
	MINT		nPtW1;
	MINT		mxic;
	MREAL		mxcc;

	MgVect2		V1;
	MgVect2		V2;
	MREAL		ss;
	MREAL		cc;

	MGPOLYG2( PgW1,		MX_PNT1);

	MBBUFF(			PtW1,	MgPoint2,	MX_PNT0); 
	MBBUFF(			iPtW1,	MINT,		MX_PNT0); 

	o_pg3->m_n = 0;

	PtS = i_ln1.p[0];
	PtF = PtS;
	PtE = i_ln1.p[1];

	PgW1 += PtS;
	PgW1 += PtE;

	for (;;) {
		nPtW1 = 0;
		for ( ic1=0; ic1<io_Gln2->m_n; ic1++) {						// 終点に接続する線分を検索
			if (PtE == io_Gln2->m_ln[ic1].p[0]) {
				PtW1[nPtW1] = io_Gln2->m_ln[ic1].p[1];
				iPtW1[nPtW1++] = ic1;
			}
		}
		for ( ic1=*io_nHk; ic1<io_Gln2->m_n; ic1++) {
			if (PtE == io_Gln2->m_ln[ic1].p[1]) {
				PtW1[nPtW1] = io_Gln2->m_ln[ic1].p[0];
				iPtW1[nPtW1++] = ic1;
			}
		}
		if ( nPtW1 == 0) {										// 線分なし、終点手前の線分を削除
			PgW1.m_n--;
			if (PgW1.m_n < 2) {
				bst = false;
				break;
			} else {
				PtE = PtS;
				PtS = PgW1.m_p[PgW1.m_n - 2];
			}
		} else {
			if ( nPtW1 == 1) {									// 線分が1つ見つかった場合はその線分を接続
				mxic = 0;
			} else {
				V1 = (PtS - PtE).Unitize();						// 線分が複数見つかった場合はもっとも左側の線分を接続
				mxcc = -9.0f;
				for ( ic2=0; ic2<nPtW1; ic2++) {
					V2 = (PtW1[ic2] - PtE).Unitize();
					ss = V1 ^ V2;
					cc = V1 * V2;
					if (ss > 0) cc = -cc - 2.0f;
					if (cc > mxcc) {
						mxcc = cc;
						mxic = ic2;
					}
				}
			}
			if (PtW1[mxic] == PtF) {
				*o_pg3 = PgW1;									// 終点と多角形の開始点が同一位置の場合は、１つの多角形とみなす
				bst = true;
				break;
			} else {
				PtS = PtE;
				PtE = PtW1[mxic];
				PgW1 += PtE;									// 見つかった線分を接続
			}
			nc1 = iPtW1[mxic];									// 線分作業領域から接続済みの線分を間引き
			for ( ic1=1; ic1<io_Gln2->m_n; ic1++) {
				if ( ic1 > nc1)
					io_Gln2->m_ln[ic1-1] = io_Gln2->m_ln[ic1];
			}
			io_Gln2->m_n--;
			if ( nc1 < *io_nHk) (*io_nHk)--;
		}
	}
	return	bst;
}

//
// ---------------------( ２次元 )------------------------------
//		穴付き多角形を多角形に結合する
//
void MGeo::ConnectGPgtoPg2(						//
				const	MgGPolyg2&	i_GpgI,		// (I  ) 穴付き多角形
						MgPolyg2*	o_pgO		// (  O) 結合後の多角形
				)
{
	MINT			ic0, ic1, ic1a, ic2, ic2a;
	MINT			ic1amin, ic2min, ic2amin;
	MgGPolyg2		GpgW;
	MGPOLYG2( pgW, 100);
	MgPoint2		pt1;
	MgVect2			v12;
	MREAL			rL2, rMinL2;

	GpgW = i_GpgI;
	for ( ic1=GpgW.m_n-1; ic1>0; ic1--) {
		rMinL2 = MREALMAX;
		for ( ic1a=0; ic1a<GpgW.m_pg[ic1].m_n; ic1a++) {
			pt1 = GpgW.m_pg[ic1].m_p[ic1a];
			for ( ic2=0; ic2<ic1; ic2++) {
				for ( ic2a=0; ic2a<GpgW.m_pg[ic2].m_n; ic2a++) {
					v12 = GpgW.m_pg[ic2].m_p[ic2a] - pt1;
					rL2 = v12.x * v12.x + v12.y * v12.y;
					if (rL2 < rMinL2) {
						ic1amin = ic1a;
						ic2min = ic2;
						ic2amin = ic2a;
						rMinL2 = rL2;
					}
				}
			}
		}
		pgW.m_n = 0;
		for ( ic0=0; ic0<=ic2a; ic0++)
			pgW += GpgW.m_pg[ic2].m_p[ic0];
		for ( ic0=ic1a; ic0<GpgW.m_pg[ic1].m_n; ic0++)
			pgW += GpgW.m_pg[ic1].m_p[ic0];
		for ( ic0=0; ic0<=ic1a; ic0++)
			pgW += GpgW.m_pg[ic1].m_p[ic0];
		for ( ic0=ic2a; ic0<GpgW.m_pg[ic2].m_n; ic0++)
			pgW += GpgW.m_pg[ic2].m_p[ic0];
		GpgW.m_pg[ic2] = pgW;
	}
	*o_pgO = GpgW.m_pg[0];
}

//
// ---------------------( ３次元 )------------------------------
//		穴付き多角形を多角形に結合する
//
void MGeo::ConnectGPgtoPg3(						//
				const	MgGPolyg3&	i_GPgI,		// (I  ) 穴付き多角形
				const	MgVect3&	i_VuH,		// (I  ) 法線方向
						MgPolyg3*	o_pPgO		// (  O) 結合後の多角形
				)
{
	MINT			ic0, ic1, ic1a, ic2, ic2a;
	MINT			ic2a1, ic2a2;
	MINT			ic1b1, ic1b2;
	MINT			ic1amin, ic2min, ic2amin;
	MgGPolyg3		gPgW;
	MGPOLYG3( PgW, 100);
	MgPoint3		Pt1;
	MgVect3			V12;
	MREAL			rL2, rMinL2;
	MgVect3			V1, V1p, V2, V2p;

//								gPgI.Print( Mstr( "051127 穴付き多角形を多角形に結合 gPgI"));

	gPgW = i_GPgI;
	for ( ic1=gPgW.m_n-1; ic1>0; ic1--) {
		rMinL2 = MREALMAX;

		for ( ic1a=0; ic1a<gPgW.m_Pg[ic1].m_n; ic1a++) {
			Pt1 = gPgW.m_Pg[ic1].m_P[ic1a];
			for ( ic2=0; ic2<ic1; ic2++) {
				ic2a2 = gPgW.m_Pg[ic2].m_n - 2;
				ic2a1 = ic2a2 + 1;

				for ( ic2a=0; ic2a<gPgW.m_Pg[ic2].m_n; ic2a2=ic2a1,ic2a1=ic2a,ic2a++) {
					V12 = gPgW.m_Pg[ic2].m_P[ic2a1] - Pt1;
					rL2 = V12 * V12;											// 距離の二乗

					if ((rL2 - rMinL2) * (rL2 - rMinL2) < g_gTol.D_2 * 4 * rL2) {
						V1 = gPgW.m_Pg[ic2min].m_P[ic2a1] - gPgW.m_Pg[ic2min].m_P[ic2a2];
						V1p = Pt1 - gPgW.m_Pg[ic2min].m_P[ic2a2];
						V2 = gPgW.m_Pg[ic2min].m_P[ic2a] - gPgW.m_Pg[ic2min].m_P[ic2a1];
						V2p = Pt1 - gPgW.m_Pg[ic2min].m_P[ic2a1];
						if ((V1 ^ V1p) * i_VuH < 0 || (V2 ^ V2p) * i_VuH < 0)
							continue;											// 最小と同一距離の点であるが裏側
					} else if (rL2 > rMinL2) {
						continue;												// 最小で無い
					}

					ic1amin	= ic1a;												// 最小
					ic2min = ic2;
					ic2amin = ic2a1;
					rMinL2 = rL2;
				}
			}
		}
		// 最後のポリゴンと距離が一番近いポリゴンを合体し、最後のポリゴンを取り除く（とみなす）
		PgW.m_n = 0;
		for ( ic0=0; ic0<=ic2amin; ic0++)
			PgW += gPgW.m_Pg[ic2min].m_P[ic0];
		for ( ic0=ic1amin; ic0<gPgW.m_Pg[ic1].m_n; ic0++)
			PgW += gPgW.m_Pg[ic1].m_P[ic0];
		for ( ic0=0; ic0<=ic1amin; ic0++)
			PgW += gPgW.m_Pg[ic1].m_P[ic0];
		for ( ic0=ic2amin; ic0<gPgW.m_Pg[ic2min].m_n; ic0++)
			PgW += gPgW.m_Pg[ic2min].m_P[ic0];
		gPgW.m_Pg[ic2min] = PgW;
	}
//								PgW.Print( L"PgW");
//								gPgW.pg[0].Print( Mstr( "gPgW.pg[0]"));
	*o_pPgO = gPgW.m_Pg[0];
//								PgO->Print( L"PgO");
	o_pPgO->m_n = 0;
//								PgO->Print( L"PgO_0");
	ic1b2 = gPgW.m_Pg[0].m_n -2;
	ic1b1 = ic1b2 + 1;
	for ( ic1=0; ic1<gPgW.m_Pg[0].m_n; ic1b2=ic1b1, ic1b1=ic1,ic1++) {
		V1 = gPgW.m_Pg[0].m_P[ic1b1] - gPgW.m_Pg[0].m_P[ic1b2];
		V2 = gPgW.m_Pg[0].m_P[ic1] - gPgW.m_Pg[0].m_P[ic1b2];

//								MBTRCPRINTI( L"Count", ic1);
//								V1.Print( L"V1");
//								V2.Print( L"V2");
//								MREAL V22 = V2 * V2;			MBTRCPRINTF( L"V22", V22);
		// ３点が直線に並んでいる場合は中央の点を間引きする
		MREAL D12 = (V1 ^ V2) * i_VuH;
			MBTRCPRINTF( L"D12", D12);
			MBTRCPRINTF( L"g_gTol.D_2", g_gTol.D_2);
		if (D12 * D12 > V2 * V2 * g_gTol.D_2)
			(*o_pPgO) += gPgW.m_Pg[0].m_P[ic1b1];
	}
//								PgO->Print( Mstr( "051127 穴付き多角形を多角形に結合 PgO"));
//								MBTRCFLUSH;
}

} // namespace MC
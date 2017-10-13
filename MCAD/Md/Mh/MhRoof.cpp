//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MhRoof.cpp
//
//		屋根面を生成する
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"

#include "MgLib.h"
#include "MgGrp.h"

#include "MbList.h"

#include "MhPlcInfo.h"

#include "MmSystemProperty.h"

#include "MmLib.h"
#include "MhRoof.h"

#define	 	MCMAXIESIZE		50000.								// 屋根面最大奥行き50m
#define		MCMINKSSIZE		1.

#define		MAXHAIKABE		500									// 壁配列制限値
#define		MAXGKABE		100									// 外壁最大数(約)
#define		MAXRFMKJ		10									// 屋根面形状最大角数(約)

namespace MC
{

/*
//
// ---------------------( ３次元 )------------------------------
//		直線を多角形で分割し、選択条件に合う線分群を取り出す
//
inline MINT	MgDivide(								// (  O) ステイタス
													//		 MC_NINT		(0): 交差なし
													//		 MC_INT			(1): 交差あり
						MINT		Sel,			// (I  ) 選択条件
													//		 MC_IN_BORDER	(001): 内側
													//		 MC_ON_BORDER	(002): 辺上
				const	MgULine3	&ULn1,			// (I  ) 直線
				const	MgPolyg3	&Pg2,			// (I  ) 多角形
						MgGLine3	*GLn3			// (  O) 分割後の線分群
				)
{
	GLn3->n = 0;
	return MgDivideAdd( Sel, ULn1, Pg2, GLn3);
}
*/

static void MgAndGLineOnULineAdd(
				const	MgGLine2	&Gln1,			// (I  ) 線分群1	(1つの直線上の線分群)
				const	MgGLine2	&Gln2,			// (I  ) 線分群2	(線分群1と同一直線上の線分群)
						MgGLine2	*Glno			// (  O) 線分群1と線分群2との重なった部分の線分群
													//		 ＝0: 重なり線分なし
													//		 ≠0: 重なり線分あり
				);

static bool MgChkRfmCls(									// (  O) true:	交差チェック対象屋根面
													//		 false:	交差チェック対象外屋根面
				const	MgLine3		&LnJim1,		// (I  ) 屋根面1の地廻り線
				const	MgPlane3	&Pln1,			// (I  ) 屋根面1の平面係数
				const	MgLine3		&LnJim2,		// (I  ) 屋根面2の地廻り線
				const	MgPlane3	&Pln2			// (I  ) 屋根面2の平面係数
				);

////////////////////////////////////////////////////////////////////////////////////
//勾配　　　　　= 整数　　　　　<地廻り線上の屋根面の勾配,
//　　　　　　　　　　　　　　　　　　　　ただし妻側は9900, 建物側は99900
//					(10寸当たりの屋根面の上がり量:単位1/10寸)
//屋根線＋ = 構成点２･線種
//
//　　　　構成点２ 　　　= (整数２)２　　　 <両端点の座標:(X･Y)２ (連動座標系)>
//　　　　線種　　　　　= 1|2|3|4|5|6|7 <1:水平､2:妻側､3:山､4:谷､5:棟､6:建物側(水平)､
//　　　　　　　　　　　　　　　　　　　 7:建物側(勾配)>
//	屋根構成線タイプコード
static	MREAL	McRfmOffset[] = {
				0.,			// MHRL_DUMMY,				// ダミー
				455.,		// MHRL_NOKI,				// 軒
				300.,		// MHRL_KERABA,				// けらば
				0.,			// MHRL_MUNE,				// 棟
				0.,			// MHRL_RYOU,				// 稜
				0.,			// MHRL_TANI,				// 谷
				0.,			// NHRL_MIZUKIRI,			// 水切り
				0.,			// MHRL_KATANAGARENOKI,		// 片流れ軒
				0.,			// MHRL_YANEUEMIZUKIRI,		// 親屋根の上に乗る子屋根の構成線
				0.			// MHRL_YAGIRI				// 矢切
				};	

void MhRoofInfo::FreeAllAtr()
{ 
	MBFREE( m_pVer);
	MBFREE( m_pOpt1);
	MBFREE( m_pOpt2);

	m_GJim1.Free();
//	m_GJim2.Free();
//	m_GTn.Free();
	m_GKs1.Free();
//	m_GKs2.Free();
	m_GRfm.Free();
	m_GNUm.Free();
	m_GHm.Free();
	if ( m_pZukei) {
		MBFREE( m_pZukei->m_pNext);
		delete (char*)m_pZukei;
	}
}
void MhRoofInfo::InitAllAtr(
				MINT	ifPtr									// イニシャライズ範囲 1:全ての属性 0:属性領域ポインタのみ
				)
{
	m_pVer = NULL;
	m_pOpt1 = NULL;
	m_pOpt2 = NULL;
	m_GJim1.m_pst = NULL;
	m_GJim1.m_n = m_GJim1.m_isz = 0;
	m_GKs1.m_pst = NULL;
	m_GKs1.m_n = m_GKs1.m_isz = 0;
	m_GRfm.m_pst = NULL;
	m_GRfm.m_n = m_GRfm.m_isz = 0;
	m_GNUm.m_Pg = NULL;
	m_GNUm.m_n = m_GNUm.m_isz = 0;
	m_GHm.m_Pg = NULL;
	m_GHm.m_n = m_GHm.m_isz = 0;
//
	m_pZukei = NULL;

	if ( ifPtr == 1) {
		m_iKai = 0;
//		m_Oya = NULL;											// 親屋根
		m_itpRf = 0;											// 屋根タイプ
		m_rKb = 0;												// 勾配
		m_rRkH = 0.;											// 屋根基準高さ変更量
		m_rRfmA = 89.;											// 屋根面厚さ(89,140,182)
		m_rNUmA = 89.;											// 軒裏面厚さ(89,140)
		m_rMnZ[0] = m_rMnZ[1] = 0.;								// 棟ずれ量１・２
		m_rNkD[0] = m_rNkD[1] = 0.;								// 軒の出
		m_rKrD[0] = m_rKrD[1] = 0.;								// けらばの出
		m_itpNU = 0;											// 軒裏タイプ１・２
		m_itpNS = 0;											// 軒先タイプ１・２
		m_rHKmA = 0.;											// 鼻隠し面厚さ
		m_rNUk = 0.;											// 軒裏返し量
		m_rNkH = 0.;											// 軒の高さ（台形ドーマ用）
		m_icdURS = 0;											// 下屋根コード
//		m_icdRR = 0;											// 大屋根コード

		m_imdDsp = 0;											// 表示モード (0: 屋根面  1:構成線)
		m_colDispD = RGB( 0, 0, 0);								// 表示色（データ指定）
		m_colDispC = RGB( 0, 0, 0);								// 表示色（現在）
//		m_nDispM = NULL;										// 表示情報（面）
//		m_nDispL = NULL;										// 表示情報（線分）

		m_nRtm = 0;												// 立面数
		m_pifRitm = NULL;										// 立面表示(面フラグ)  0:面  1:穴
		m_pnPtRitm = NULL;										// 立面表示(頂点数)
		m_pPtRitm = NULL;										// 立面表示(頂点)
//		m_pLnRitm = NULL;										// 立面表示情報（線分）

//		MhTexture*			m_KsInfo[NKSINFO];					// 構成情報
	}
}

////////////////////////////////////////////////////////////////////////////
//	属性入力値を取り込み設定する 
void MhRoofInfo::SetAttr()
{
	MINT	ist;
	MINT	iKobai;
	MREAL	rNokinoDe;
	MREAL	rKerabeDe;

	ist = InpAttr::GetComboAttrI( MC_CMB_KOBY, &iKobai);
	m_rKb = MREAL( iKobai);

	ist = InpAttr::GetComboAttrR( MC_CMB_NKDE, &rNokinoDe);
	m_rNkD[0] = m_rNkD[1] = rNokinoDe;							// 軒の出
	McRfmOffset[MHRL_NOKI] = rNokinoDe;

	ist = InpAttr::GetComboAttrR( MC_CMB_KRDE, &rKerabeDe);
	m_rKrD[0] = m_rKrD[1] = rKerabeDe;							// けらばの出
	McRfmOffset[MHRL_KERABA] = rKerabeDe;
}

////////////////////////////////////////////////////////////////////////////
//	地廻り線にコードを設定する 
//  現在はm_itpRf==MHRF_YOSEMUNEのみ使用であり、操作入力されたシフトキーにより切り妻を判定している
//	また、方向点は未使用
void MhRoofInfo::SetJimCd(
				const	MgPolyg2	&pgJim,			// (I  ) 地廻り区画
				const	MgGInt		&GifInp,		// (I  ) 地廻り線種類(仮想キー(nflag)  MK_SHIFT(004): シフトキー)
				const	MgPoint2	&pth			// (I  ) 方向点
				)
{
	MINT	ic;
//	MINT	Id;
	MhJim	pgJimw;
	
	for ( ic=0; ic<pgJim.m_n; ic++) {
		pgJimw.m_p = pgJim.m_p[ic];
		int ii = sizeof( MhJim);
		m_GJim1 += pgJimw;
	}

	switch(m_itpRf) {
		case(MHRF_YOSEMUNE):									// 寄棟
			for ( ic=0; ic<m_GJim1.m_n; ic++) {
				if ( MF_CHECK_OR( GifInp.m_i[ic], MK_SHIFT)) {
					m_GJim1[ic].m_icd = MHRL_KERABA;			// 地廻りコード = 軒線
					m_GJim1[ic].m_rKb = 999;					// 勾配(単位:1/10)　MgGetKobai
				} else {
					m_GJim1[ic].m_icd = MHRL_NOKI;				// 地廻りコード = 軒線
					m_GJim1[ic].m_rKb = m_rKb;					// 勾配(単位:1/10)　MgGetKobai
				}
			}
			break;
		case(MHRF_KIRITUMA):									// 切妻
/*
			MINT		ist;
			MINT		ic0, ic1;
			MgPoint2	pt0;
			MgVect2		v1;
			MREAL		rD2, rMinD2;

			Id = -1;
			rMinD2 = MREALMAX;
			ic0 = m_GJim1.n;
			for ( ic1=0; ic1<m_GJim1.n; ic1++) {
				ist = MgPerpPtLn2( m_ptH, MgLine2( m_GJim1[ic0].p, m_GJim1[ic1].p), &pt0);
				if ( ist == MC_ON_LINE) {
					rD2 = MgDist_2( pt0, m_ptH);				// 距離の２乗
					if ( rD2 < rMinD2) {
						Id = ic0;
						rMinD2 = rD2;
					}
				}
				ic0 = ic1;
			}

			Id &= 1;
			for ( ic=0; ic<m_GJim1.n; ic++) {
				if ( Id == 0) {
					m_GJim1[ic].iCd = MHRL_KERABA;				// 地廻りコード = けらば線
					m_GJim1[ic].rKb = 999;						// 勾配(単位:1/10) または 999:垂直 MgGetKobai
				} else {
					m_GJim1[ic].iCd = MHRL_NOKI;				// 地廻りコード = 軒線
					m_GJim1[ic].rKb = m_rKb;					// 勾配(単位:1/10)　MgGetKobai
				}
				Id = 1 - Id;
			}
			*/
			Base::MbSysError( MBCstr( "MhRoof"), MC_ERR_OTHER);
			break;
	}
}

////////////////////////////////////////////////////////////////////////////
//  地廻り線に対応する屋根面を作成する 
//		屋根面形状は上方に延長された形状で作成する
void MhRoofInfo::CreateRfmForJim()
{
	MINT		ist;
	MINT		ic0, ic1;
	MhRfm		RfmI;
	MgPoint3	Pth;
	MINT		iRfm0, iRfm1;
	MgPlane3	Pln0, Pln1;
	MgULine3	ULn1;
	MgLine3		Ln0, Ln1, Ln2, Ln3;
	MgPoint3	P0, P1;
//
//    地廻り線に対応する屋根面を作成する 
	ic0 = m_GJim1.m_n - 1;
	for ( ic1=0; ic1<m_GJim1.m_n; ic0=ic1,ic1++) {
		RfmI.m_icd = 0;
		if ( m_GJim1[ic1].m_icd == MHRL_NOKI) {
			RfmI.m_icd = MHRL_NOKI;
			RfmI.m_Pln = MgPlane3C( m_GJim1[ic0].m_p, m_GJim1[ic1].m_p, m_GJim1[ic1].m_rKb / 10.f);	// 始点・終点･勾配
		} else if ( m_GJim1[ic1].m_icd == MHRL_KERABA) {
			RfmI.m_icd = MHRL_YAGIRI;
			Pth = MgPoint3C( m_GJim1[ic0].m_p);
			Pth.z += 1000.;
			RfmI.m_Pln = MgPlane3C( MgPoint3C( m_GJim1[ic0].m_p), MgPoint3C( m_GJim1[ic1].m_p), Pth);			// ３点
		}
		if ( RfmI.m_icd) {
			m_GRfm += RfmI;										// 屋根面追加
			int ii = sizeof( MhRfm);
			m_GJim1[ic1].m_iRfm = m_GRfm.m_n - 1;				// 地廻りに配列番号を設定
		}
	}
//
//    地廻り線を外から見て右側に接続する屋根構成線（延長線分）を作成する 
	ic0 = m_GJim1.m_n - 1;
	for ( ic1=0; ic1<m_GJim1.m_n; ic0=ic1,ic1++) {
		iRfm0 = m_GJim1[ic0].m_iRfm;
		Pln0 = m_GRfm.m_pst[iRfm0].m_Pln;						// 地廻り線に接続する屋根面
		iRfm1 = m_GJim1[ic1].m_iRfm;
		Pln1 = m_GRfm.m_pst[iRfm1].m_Pln;						// 次の地廻り線に接続する屋根面
		ist = MGeo::Intr2Pln3( Pln0, Pln1, &ULn1);					// 屋根面間の交線を求める
			ASSERT( ist == MC_INT);								// 寄棟部構成線作成エラー　<ERROR>
		if ( ULn1.v.z < 0.0)										// 交線の方向を上向きにする
			ULn1.v = - ULn1.v;
		P0 = MgPoint3C( m_GJim1[ic0].m_p);
		P1 = P0 + MCMAXIESIZE * ULn1.v;
		m_GJim1[ic0].m_LnR = MgLine3( P0, P1);					// 外から見て地廻り線の右側に接続する屋根構成線（最大延長線分）
		if ( m_GJim1[ic0].m_icd == MHRL_KERABA ||
			m_GJim1[ic1].m_icd == MHRL_KERABA)
			m_GJim1[ic0].m_icdLnR = MHRL_KERABA;
		else {
			if ( ( MgVect2C(Pln0.v) ^ MgVect2C( Pln1.v)) > 0.)
				m_GJim1[ic0].m_icdLnR = MHRL_RYOU;
			else
				m_GJim1[ic0].m_icdLnR = MHRL_TANI;
		}
	}
//
//    地廻り線に対応する屋根面に構成線を作成する（上方に延長した領域） 
	ic0 = m_GJim1.m_n - 1;
	for ( ic1=0; ic1<m_GJim1.m_n; ic0=ic1,ic1++) {
		iRfm1 = m_GJim1[ic1].m_iRfm;

		Ln1 = m_GJim1[ic0].m_LnR;
		Ln2 = m_GJim1[ic1].m_LnR;
		Ln0 = MgLine3( Ln1.p[0], Ln2.p[0]); 
		ist = MGeo::Intr2Ln3( Ln1, Ln2, &P0);						// 地廻り線に接続する左右の屋根構成線が交差を確認

		m_GRfm.m_pst[iRfm1].m_Pg   += Ln1.p[0];					// 0: 地廻り線( ic1)の左の点を、地廻り線に接続する屋根面形状の最初点として追加
		m_GRfm.m_pst[iRfm1].m_cdPg += m_GJim1[ic0].m_icdLnR;		// 0: 手前の地廻り線( ic0)の右側の線種を設定
		m_GRfm.m_pst[iRfm1].m_Pg   += Ln2.p[0];					// 1: 地廻り線( ic1)の右の点を、地廻り線に接続する屋根面形状の２番目の点として追加
		m_GRfm.m_pst[iRfm1].m_cdPg += m_GJim1[ic1].m_icd;		// 1: 自身の地廻り線( ic1)の線種を設定
		m_GRfm.m_pst[iRfm1].m_cdPg += m_GJim1[ic1].m_icdLnR;		// 2: 自身の地廻り線( ic1)の右側の線種を設定
		if ( MF_CHECK_OR( ist, (MC_INT | MC_CONNECTION))) {
			m_GRfm.m_pst[iRfm1].m_Pg += P0;						// 2: 交差する場合、交差点を追加
		} else {
			m_GRfm.m_pst[iRfm1].m_Pg += Ln2.p[1];				// 3,4: 交差しない場合は両側延長線分の端点を追加
			m_GRfm.m_pst[iRfm1].m_Pg += Ln1.p[1];
//			m_GRfm.m_pst[iRfm1].m_cdPg += MHRL_DUMMY;			// 4: 上辺の線種をダミーに設定
		}
	}
}

////////////////////////////////////////////////////////////////////////////
//	他の屋根面との交差を調べ交差線分で屋根面形状をカットする
MINT MhRoofInfo::AdjustRfm1()						// (  O) ステイタス　0: 正常　-1: 屋根面形状作成エラー
{
	MINT		ist = -1;
	MINT		ist1;
	MINT		ic1, ic2;
	MINT		sel1;
	MgPlane3	*pPln1, *pPln2;
	MgMat3		MatXY;											// Pln to XY平面 ３Ｄマトリックス
	MgMat3		MatPln;											// XY平面 to Pln ３Ｄマトリックス
	MgLine2		ln0;
	MgULine3	ULn0;
	MgULine2	uln1;
	MgGLine2	Gln0, Gln1, Gln2, Gln3;
	MgGLine3	GLn2;
	MgPolyg2	pg0, pg1, pg3;
	MgPolyg3	Pg0;
	MgGLine3	GLnW;
//#ifdef _DEBUG
//MBTRCON;
//#endif
	for ( ic1=0; ic1<m_GRfm.m_n; ic1++) {
//#ifdef _DEBUG
//							MBTRCON;
//							Msprintf( MbLog::m_Str, Mstr( "******* 屋根 AdjustRfm ic1=%d\n"), ic1); MBTRCPRBF;
//#endif
		pPln1 = &m_GRfm.m_pst[ic1].m_Pln;						// 屋根面1を取り出す

		MGeo::Mat3PlntoXY( *pPln1, m_GRfm.m_pst[ic1].m_Pg.m_P[0], &MatXY, &MatPln);	// ３Ｄ→２Ｄ用変換マトリックスと逆変換マトリックスを求める

		MGeo::Polyg3Dto2D( m_GRfm.m_pst[ic1].m_Pg, MatXY, &pg1);		// 屋根面1の形状を２次元に変換する
//							m_GRfm.m_pst[ic1].pg.Print( Mstr( "051114 m_GRfm.m_pst[ic1].pg"));
//							pg1.Print( Mstr( "051114→ pg1"));
		ln0 = MgLine2( pg1.m_p[0], pg1.m_p[1]);
		Gln0.m_n = 0;
		Gln0 += MgLine2( pg1.m_p[1], pg1.m_p[2]);
		Gln0 += MgLine2( pg1.m_p[pg1.m_n-1], pg1.m_p[0]);

		for ( ic2=0; ic2<m_GRfm.m_n; ic2++) {
//#ifdef _DEBUG
//							Msprintf( MbLog::m_Str, Mstr( "-------- 屋根 AdjustRfm ic2=%d\n"), ic2); MBTRCPRBF;
//#endif
			if ( ic1 == ic2)										// 同一屋根面は無視
				continue;
			if ( m_GRfm.m_pst[ic2].m_Pg.m_n == 0)
				MQUIT;
//			ASSERT(m_GRfm.m_pst[ic2].pg.n != 0);
			pPln2 = &m_GRfm.m_pst[ic2].m_Pln;					// 他の屋根面2を取り出す
			ist1 = MGeo::Intr2Pln3( *pPln1, *pPln2, &ULn0);			// 二つの屋根面の交差直線を求める
																// 屋根面は地廻り線の順に並んでおり交差直線の方向は
																// ２次元にした時にxが大から小の方向に向かっている
			if ( ist1 != MC_INT) 
				continue;

			MGeo::ULine3Dto2D( ULn0,  MatXY, &uln1);					// 交差直線を２次元に変換し
			ist1 = MGeo::DivideULnPg2( MC_IN_BORDER | MC_ON_BORDER , uln1, pg1, &Gln1);
																// 屋根面1の形状内に含まれる線分群1を求める
//							Gln1.Print( Mstr( "051114 屋根面1の形状内に含まれる線分群1 Gln1"));
			if ( ist1 != (MC_INT | MC_MATCH)) 
				continue;
																// ３次元で屋根面2の形状内に含まれる線分群2を求める
			if ( MgChkRfmCls( MgLine3( m_GRfm.m_pst[ic1].m_Pg.m_P), *pPln1,
							 MgLine3( m_GRfm.m_pst[ic2].m_Pg.m_P), *pPln2))
				sel1 = MC_IN_BORDER | MC_ON_BORDER;
			else
				sel1 = MC_IN_BORDER;
//				sel1 = MC_IN_BORDER | MC_ON_BORDER;

			ist1 = MGeo::DivideULnPg3( sel1, ULn0, m_GRfm.m_pst[ic2].m_Pg, &GLn2);
//							GLn2.Print( Mstr( "051114 屋根面2の形状内に含まれる線分群2 GLn2"));
			if ( ist1 != (MC_INT | MC_MATCH)) 
				continue;
			MGeo::GLine3Dto2D( GLn2, MatXY, &Gln2);					// 線分群２を２次元に変換する
//							Gln2.Print( Mstr( "051114 屋根面2の形状内に含まれる線分群2 Gln2"));
			
			MgAndGLineOnULineAdd( Gln1, Gln2, &Gln3);			// (同一直線上の)線分群1と線分群2の重なり部分を取り出す
																// （注: 入力データは始点X < 終点Xで、 Ｘ方向並びのデータ）
//			GLine2Dto3D( Gln3, MatPln, &GLnW);					// デバッグ用
			Gln0 += Gln3;
//							Gln0.Print( Mstr( "051114 重ねた線分群計 Gln0"));
		}
		if ( Gln0.m_n > 1) {
			MGeo::MakeGPg2FromLn2InGLn2( ln0, Gln0, &pg0);			// 地廻り線を基線とし交差線分を結合し最小の多角形を作る
//							pg0.Print( Mstr( "051114 結合し最小の多角形 pg0"));
			MGeo::Polyg2Dto3D( pg0, MatPln, &Pg0);						// ３次元に戻す
//							Pg0.Print( Mstr( "051114 結合し最小の多角形 Pg0"));
			m_GRfm.m_pst[ic1].m_Pg = Pg0;						// 屋根面形状を修正
			for ( ic2=3; ic2<m_GRfm.m_pst[ic1].m_Pg.m_n; ic2++)	// 屋根構成線の線種をダミーに設定する
				m_GRfm.m_pst[ic1].m_cdPg += MHRL_DUMMY;
		}
	}
	ist = 0;
exit:
	return ist;
}

////////////////////////////////////////////////////////////////////////////
//	上階外壁との交差を調べ交差線分で屋根面形状をカットする
MINT MhRoofInfo::AdjustRfm2()						// (  O) ステイタス　0: 正常　-1: 屋根面形状作成エラー
{
	MINT		ist = 0;
	MINT		ist1;

	MhPlcInfo* pHaiKabe[MAXHAIKABE+2];						// 壁データ
	MPOSITION	pPlcPos[MAXHAIKABE+2];						// 壁データレコード位置
	MINT		nHaiKabe;										// 壁数
	MhPlcInfo	HaiKabeI;										// 追加壁

	MINT		ic1, ic2, ic3b, ic3, ic4b, ic4;

	this->Print( Mstr( "121205外壁との交差チェック前　屋根"));

	MGGLINE2(GlnGKb, MAXGKABE);
	MGGPOLYG2(GpgGKb, MAXGKABE, MAXGKABE, MAXGKABE);
	MGPOLYG2(pgRfm, MAXRFMKJ);
	MgGPolyg2	GpgRfm(3);
	MgGPolyg3	GPgRfm(3);
	MgGInt		GiCd(10);
	MgPolyg3	*pPgRfm;
	MgGInt		*ppgCdRfm;
	MgPolyg3	*pPgRfmN;
	MhRfm		Rfm;

	//　家モデルよりカレント階の上階の全ての壁を取得する
	MINT iKai = InpMode::GetKai() + 1;
	nHaiKabe = HaitiIn::GetPts( iKai, MP_GP_TAIRYOKU, NULL, Mstr( "外壁"), MAXHAIKABE, pHaiKabe, pPlcPos);
																// 家モデルよりカレント階の上階の全ての壁を取得する
	if ( nHaiKabe <= 0)
		return ist;												//		上階外壁なし
	for ( ic1=0; ic1<nHaiKabe; ic1++)
		GlnGKb += MgLine2C( pHaiKabe[ic1]->GetPIPlcIti());

	MGeo::MakeGPg2InGLn2( GlnGKb, &GpgGKb);
	if ( GpgGKb.m_n == 0)
		return ist;

	MgPlane3	*pPln1;

//MBTRCON;
	for ( ic1=0; ic1<m_GRfm.m_n; ic1++) {
		pPln1 = &m_GRfm.m_pst[ic1].m_Pln;						// 屋根面1を取り出す
		pPgRfm = &m_GRfm.m_pst[ic1].m_Pg;
		ppgCdRfm = &m_GRfm.m_pst[ic1].m_cdPg;

		//if ( MGeo::Zero( pPln1->v.z))								// XY平面に垂直の屋根面はスキップ
		//	continue;
		pgRfm = (*pPgRfm);										// XY平面に投影し２次元化

		ist1 = MGeo::MergePgGPgtoGPg2( MC_DIFF, pgRfm, GpgGKb, &GpgRfm);
		if ( ist1 == MC_MATCH)									// 屋根面形状が外壁と交差なしでそのまま残る場合は
			continue;											// 屋根面形状の変更なし
		GPgRfm.Set( GpgRfm, *pPln1);							// 屋根平面状の３次元化

		if ( GPgRfm.m_n == 0) {									// 屋根面形状が上階外壁内に全て隠れる場合は削除
			m_GRfm.m_pst[ic1].m_Pg.m_n = 0;
			m_GRfm.m_pst[ic1].m_cdPg.m_n = 0;
		} else {												// 屋根面形状の一部が重なる場合は、屋根面形状と屋根構成線コードを修正
																// 複数に分かれる場合は追加
			for ( ic2=0; ic2<GPgRfm.m_n; ic2++) {
				GiCd.m_n = 0;
				
				// 作成された屋根面毎に元の屋根面を参照して屋根構成線コードを設定する
				pPgRfmN = &GPgRfm.m_Pg[ic2];
				for ( ic3b=pPgRfmN->m_n-1,ic3=0; ic3<pPgRfmN->m_n; ic3b=ic3,ic3++) {
					
					for ( ic4b=pPgRfm->m_n-1,ic4=0; ic4<pPgRfm->m_n; ic4b=ic4,ic4++) {
						if ( MGeo::ChkLn3OnLn3( MgLine3(pPgRfmN->m_P[ic3b], pPgRfmN->m_P[ic3]), MgLine3(pPgRfm->m_P[ic4b], pPgRfm->m_P[ic4])))
							break;
					}
					if ( ic4 < pPgRfm->m_n)
						GiCd += ppgCdRfm->m_i[ic4];
					else
						GiCd += NHRL_MIZUKIRI;
				}
				if ( ic2 == 0) {
					(*pPgRfm) = (*pPgRfmN);							// 屋根面形状を修正
					(*ppgCdRfm) = GiCd;
				} else {
					Rfm.m_Pln = *pPln1;								// 屋根面形状を追加
					Rfm.m_Pg = (*pPgRfmN);
					Rfm.m_cdPg = GiCd;
					m_GRfm += Rfm;
				}
//				for ( ic2=3; ic2<m_GRfm.m_pst[ic1].m_Pg.m_n; ic2++)			// 屋根構成線の線種をダミーに設定する
//					m_GRfm.m_pst[ic1].m_cdPg += MHRL_DUMMY;
			}
		}
	}
	ist = 0;
	return ist;
}

////////////////////////////////////////////////////////////////////////////
//	屋根面１に対し屋根面2の交差をチェック時に境界を含めるか否かを判定する

static bool MgChkRfmCls(							// (  O) true:	交差チェック時に屋根面2の境界を含める
													//		 false:	交差チェック時に屋根面2の境界を含めない
				const	MgLine3		&LnJim1,		// (I  ) 屋根面1の地廻り線
				const	MgPlane3	&Pln1,			// (I  ) 屋根面1の平面係数
				const	MgLine3		&LnJim2,		// (I  ) 屋根面2の地廻り線
				const	MgPlane3	&Pln2			// (I  ) 屋根面2の平面係数
				)
{
	bool	bst;
	MINT	ist1, ist2;
	bool	bst1, bst2;

	MGeo::ChkLn3OnPln3WS( LnJim1, Pln2, &ist1);							// 屋根面1の地廻り線位置が、屋根面2の上下どちらかを判定
	MGeo::ChkLn3OnPln3WS( LnJim2, Pln1, &ist2);							// 屋根面2の地廻り線位置が、屋根面1の上下どちらかを判定

	bst1 = !( ist1 & MC_PS_LOWER || ist1 & MC_PE_LOWER);	// 屋根面1の地廻り線1が屋根面2の上または上側
	bst2 = !( ist2 & MC_PS_LOWER || ist2 & MC_PE_LOWER);	// 屋根面2の地廻り線2が屋根面1の上または上側

	if ( !bst1 && bst2)									// 地廻り線1が屋根面2の下側　かつ　地廻り線2が屋根面1の上または上側
		bst = false;									// 交差チェックで境界は含めない
	else
		bst = true;
	return bst;
}

////////////////////////////////////////////////////////////////////////////
//	同一直線上の線分群と線分群の重なり部分の取り出し
//	（注: 入力データはＸ方向並びのデータ）
static void MgAndGLineOnULineAdd(
				const	MgGLine2	&Gln1,			// (I  ) 線分群1	(1つの直線上の線分群)
				const	MgGLine2	&Gln2,			// (I  ) 線分群2	(線分群1と同一直線上の線分群)
						MgGLine2	*Glno			// (  O) 線分群1と線分群2との重なった部分の線分群
													//		 ＝0: 重なり線分なし
													//		 ≠0: 重なり線分あり
				)
{
	MINT		ic1, ic2;
	MgLine2		lno;
	MREAL		y1;
	MgVect2		V1, V2;
	MGGLINE2( Glnw1, MX_LIN0);
	MGGLINE2( Glnw2, MX_LIN0);
	MgLine2		lnw1, lnw2;
	MREAL		d1, d2, dd;

	Glno->m_n = 0;
	if ( Gln1.m_n == 0 || Gln2.m_n == 0)
		return;

	V1 = Gln1.m_ln[0].p[1] - Gln1.m_ln[0].p[0];
	V1.SetUnitize();

	//	右回転し( x = x cos(a) + y sin(a); y = -x sin(a) + y cos(a); )、
	//  Ｘ軸に平行にする
	y1 = - Gln1.m_ln[0].p[0].x * V1.y + Gln1.m_ln[0].p[0].y * V1.x;
	for ( ic1=0; ic1<Gln1.m_n; ic1++) {
//		pln1 = &Gln1.m_ln[ic1];
		lnw1 = MgLine2( MgPoint2( Gln1.m_ln[ic1].p[0].x * V1.x + Gln1.m_ln[ic1].p[0].y * V1.y, y1),
						MgPoint2( Gln1.m_ln[ic1].p[1].x * V1.x + Gln1.m_ln[ic1].p[1].y * V1.y, y1));
		Glnw1 += lnw1;
	}
	for ( ic1=0; ic1<Gln2.m_n; ic1++) {
		lnw2 = MgLine2( MgPoint2( Gln2.m_ln[ic1].p[0].x * V1.x + Gln2.m_ln[ic1].p[0].y * V1.y, y1),
						MgPoint2( Gln2.m_ln[ic1].p[1].x * V1.x + Gln2.m_ln[ic1].p[1].y * V1.y, y1));
		for ( ic2=0; ic2<Glnw1.m_n; ic2++) {
			if ( lnw2.p[0].x < lnw2.p[1].x) {
				d1 = MGMAX( lnw2.p[0].x, Glnw1.m_ln[ic2].p[0].x);
				d2 = MGMIN( lnw2.p[1].x, Glnw1.m_ln[ic2].p[1].x);
			} else {
				d1 = MGMAX( lnw2.p[1].x, Glnw1.m_ln[ic2].p[0].x);
				d2 = MGMIN( lnw2.p[0].x, Glnw1.m_ln[ic2].p[1].x);
			}
			dd = d2 - d1;
			if ( dd > MGPTOL->D) {											// 重なりがMgTol()->D超の場合は重なり有り

				// 左回転し(x = x cos(a) - y sin(a); y = x sin(a) + y cos(a); )、
				// 出力する
				lnw1 = MgLine2( MgPoint2( d1 * V1.x - y1 * V1.y, d1 * V1.y + y1 * V1.x),
								MgPoint2( d2 * V1.x - y1 * V1.y, d2 * V1.y + y1 * V1.x));
				(*Glno) += lnw1;
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////
//	重なる屋根面を1つにまとめる
void MhRoofInfo::ConnectRfm()
{
	MINT		ist1;
	MINT		ic1, ic2, ic3, ic3b, ic4, ic4b;
	MINT		nRfm;
	MgPlane3	Pln1, Pln2;
	MgGPolyg3	GPgw;
	MgPolyg3	Pgw;
	MgGInt		GpgCd;
	bool		fSet;
	MgPolyg3	*pPg1, *pPg2;
	MgLine3		Ln3, Ln4;

	nRfm = m_GRfm.m_n;
	for ( ic1=0; ic1<nRfm; ic1++) {
		if ( m_GRfm.m_pst[ic1].m_Pg.m_n == 0)							// マージにて削除した屋根面を読み飛ばす
			continue;
		Pln1 = m_GRfm.m_pst[ic1].m_Pln;								// 屋根面1を取り出す
		for ( ic2=ic1+1; ic2<nRfm; ic2++) {
			if ( m_GRfm.m_pst[ic2].m_Pg.m_n == 0)						// 削除した屋根面（形状無しの屋根面）は無視
				continue;
			Pln2 = m_GRfm.m_pst[ic2].m_Pln;							// 屋根面2を取り出す
			if ( Pln1 != Pln2)
				continue;										// 同一平面上に無い屋根面は無視

			ist1 = MGeo::Merge2PgtoGPg3( MC_OR, m_GRfm.m_pst[ic1].m_Pg, m_GRfm.m_pst[ic2].m_Pg, &GPgw);
			if ( MF_CHECK_OR( ist1, MC_INT) && GPgw.m_n == 1) {			// 結合有りの場合は
				// 屋根構成線種の検索用
				Pgw = m_GRfm.m_pst[ic1].m_Pg;
				Pgw += m_GRfm.m_pst[ic2].m_Pg;
				pPg1 = &m_GRfm.m_pst[ic1].m_Pg;
				pPg2 = &m_GRfm.m_pst[ic2].m_Pg;
				GpgCd.m_n = 0;
				for ( ic3b=GPgw.m_Pg[0].m_n-1,ic3=0; ic3<GPgw.m_Pg[0].m_n; ic3b=ic3,ic3++) {
					Ln3 = MgLine3( GPgw.m_Pg[0].m_P[ic3b], GPgw.m_Pg[0].m_P[ic3]);
					fSet = false;
					for ( ic4b=pPg1->m_n-1,ic4=0; !fSet && ic4<pPg1->m_n; ic4b=ic4,ic4++) {
						Ln4 = MgLine3( pPg1->m_P[ic4b], pPg1->m_P[ic4]);
						if ( MGeo::ChkLn3OnLn3( Ln3, Ln4)) {
							GpgCd += m_GRfm.m_pst[ic1].m_cdPg.m_i[ic4];
							fSet = true;
						}
					}
					for ( ic4b=pPg2->m_n-1,ic4=0; !fSet && ic4<pPg2->m_n; ic4b=ic4,ic4++) {
						Ln4 = MgLine3( pPg2->m_P[ic4b], pPg2->m_P[ic4]);
						if ( MGeo::ChkLn3OnLn3( Ln3, Ln4)) {
							GpgCd += m_GRfm.m_pst[ic2].m_cdPg.m_i[ic4];
							fSet = true;
						}
					}
					ASSERT(GpgCd.m_n == ic3 + 1);
				}

				// 屋根面形状の修正
				m_GRfm.m_pst[ic1].m_Pg = GPgw.m_Pg[0];				// 屋根面1の屋根面形状を修正
				m_GRfm.m_pst[ic1].m_cdPg = GpgCd;					// 屋根線種コードを修正
				m_GRfm.m_pst[ic2].m_Pg.m_n = 0;						// 屋根面2は削除
				m_GRfm.m_pst[ic2].m_cdPg.m_n = 0;

				ic2 = ic1 + 1;									// 屋根面2の取り出しを屋根面1の次からやり直す
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////
// 屋根面の軒線、けらば線をオフセットする
void MhRoofInfo::OffsetRfm()
{
	MINT		ic1, ic2;
	MINT		nRfm;
	MhRfm		*Rfm;
	MgPlane3	Pln1;
	MINT		ib2, ib1;
	MgPoint2	pb2, pb1, pc2;
	MgPoint2	pb0;
	MgVect2		vb1, vc2;
	MgLine2		lnb1, lnc2;
	MgPolyg3	Pg1;

	nRfm = m_GRfm.m_n;
	for ( ic1=0; ic1<nRfm; ic1++) {
		Rfm = &m_GRfm.m_pst[ic1];
		if ( Rfm->m_Pg.m_n == 0)										// マージにて削除した屋根面はスキップ
			continue;
		Pln1 = Rfm->m_Pln;										// 屋根面1を取り出す
//		if ( MgZero( Pln1.v.z))									// XY平面に垂直の屋根面はスキップ
		if ( Rfm->m_icd == MHRL_YAGIRI)							// 矢切はスキップ
			continue;
		Pg1 = Rfm->m_Pg;
		ib2 = Pg1.m_n-2;
		pb2 = MgPoint2C( Pg1.m_P[ib2]);
		ib1 = ib2 + 1;
		pb1 = MgPoint2C( Pg1.m_P[ib1]);
		ASSERT( Pg1.m_n == Rfm->m_cdPg.m_n);
		for ( ic2=0; ic2<Pg1.m_n; ic2++) {
			pc2 = MgPoint2C( Pg1.m_P[ic2]);
			vb1 = pb1 - pb2;
			vb1.SetUnitize();
			lnb1 = MgLine2( pb2, pb1) + vb1.RotR90() * McRfmOffset[Rfm->m_cdPg[ib1]];
			vc2 = pc2 - pb1;
			vc2.SetUnitize();
			lnc2 = MgLine2( pb1, pc2) + vc2.RotR90() * McRfmOffset[Rfm->m_cdPg[ic2]];
			MGeo::Intr2Ln2( lnb1, lnc2, &pb0);
			Rfm->m_Pg.m_P[ib1] = MgPoint3C( pb0, Pln1.GetZ( pb0));
			pb2 = pb1;
			ib1 = ic2;
			pb1 = pc2;
		}
	}
}

////////////////////////////////////////////////////////////////////////////
// 屋根面の図形を作成する
void MhRoofInfo::CreateRfmZukei()
{
	MINT		nLine, nLineS;
	MINT		szZukei;
	MINT		ic0, ic1, ic2, icz;

	nLineS = 0;
	for ( ic0=0; ic0<m_GRfm.m_n; ic0++)
		nLineS += m_GRfm.m_pst[ic0].m_Pg.m_n;

	szZukei = sizeof(MhZukei) + sizeof(MgLine3) * (nLineS - 1);			// 線分[nLine]本分の図形データサイズ
	m_pZukei = (MhZukei*)new char[szZukei];
	m_pZukei->m_obj = MHZK_DISP;											// 図形表示用
	m_pZukei->m_iZTp = MHZK_GLINE;											// 線分群
	m_pZukei->m_pNext = NULL;
	m_pZukei->m_nZukei = nLineS;

	icz = 0;
	for ( ic0=0; ic0<m_GRfm.m_n; ic0++) {
		nLine = m_GRfm.m_pst[ic0].m_Pg.m_n;
		ic1 = nLine - 1;
		for ( ic2=0; ic2<nLine; ic1=ic2,ic2++) {
			m_pZukei->m_lnZukei[icz++] = MgLine3( m_GRfm.m_pst[ic0].m_Pg.m_P[ic1], 
									 			  m_GRfm.m_pst[ic0].m_Pg.m_P[ic2]);
		}
	}
}

////////////////////////////////////////////////////////////////////////////
// トレース
void MhJim::Print( MCHAR* s, MINT ic)
{
#ifdef _DEBUG
	Msprintf( MbLog::m_Str, Mstr( "%s	MhJim[%2d]	(%7.1f,%7.1f)	Cd: %d	fInp: %d	rKb:%7.1f	Rfm: %d")
						   Mstr( "	LnR:(%7.1f,%7.1f,%7.1f)	(%7.1f,%7.1f,%7.1f)\n"),
								s, ic, m_p.x, m_p.y, m_icd, m_ifInp, m_rKb, m_iRfm,
								m_LnR.p[0].x, m_LnR.p[0].y, m_LnR.p[0].z, m_LnR.p[1].x, m_LnR.p[1].y, m_LnR.p[1].z);
	MBTRCPRBF;
#endif
}
void MgKs1::Print( MCHAR* s, MINT ic)
{
#ifdef _DEBUG
	Msprintf( MbLog::m_Str, Mstr( "%s	MgKs1[%2d]	LnR: (%7.1f,%7.1f,%7.1f) (%7.1f,%7.1f,%7.1f)")
						   Mstr( "	Typ: %d	Cd: %d	fProc: %d	Rfm = %d, %d\n"),
								s, ic, m_Ln.p[0].x, m_Ln.p[0].y, m_Ln.p[0].z, m_Ln.p[1].x, m_Ln.p[1].y, m_Ln.p[1].z,
								m_itpLn, m_iCd, m_ifProc, m_iRfm[0], m_iRfm[1]);
	MBTRCPRBF;
#endif
}
void MhRfm::Print( MCHAR* s, MINT ic)
{
#ifdef _DEBUG
	Msprintf( MbLog::m_Str, Mstr( "%s	MhRfm[%2d]	pln: (%7.3f,%7.3f,%7.3f),%7.1f	Cd: %d\n"),
								s, ic, m_Pln.v.x, m_Pln.v.y, m_Pln.v.z, m_Pln.d, m_icd);
	MBTRCPRBF;
	m_cdPg.Print( Mstr( "MhRfm pgCd"));
	m_Pg.Print( Mstr( "MhRfm pg"));
#endif
}

void MhRoofInfo::Print( MCHAR* s)
{
#ifdef _DEBUG
	Msprintf( MbLog::m_Str, Mstr( "%s	MhRoofInfo	Kai: %d	Kb:%7.1f	RkH:%7.1f	RfmA:%7.1f	NUmA:%7.1f\n")
						    Mstr( "			MnZ:%7.1f,%7.1f	NkD:%7.1f,%7.1f	KrD:%7.1f,%7.1f")
						    Mstr( "	tpNU: %d	tpNS: %d	HKmA:%7.1f	NUk:%7.1f	NkH:%7.1f	cdURS:  %d\n"),
								 s, m_iKai, m_rKb, m_rRkH, m_rRfmA, m_rNUmA,
								 m_rMnZ[0], m_rMnZ[1], m_rNkD[0], m_rNkD[1], m_rKrD[0], m_rKrD[1],
								 m_itpNU, m_itpNS, m_rHKmA, m_rNUk, m_rNkH, m_icdURS);
	m_GJim1.Print( Mstr( "GJim1"));								// 地廻り線（入力）
	m_GKs1.Print( Mstr( "GKs1"));								// 構成線（入力）
	m_GRfm.Print( Mstr( "GRfm"));								// 屋根面
#endif
}

} // namespace MC
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MhKabe.cpp
//
//		壁データを正規化する
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"
#include "MgLib.h"
#define DLL_EXPORT_IEMODEL_DO
#include "MmDefine.h"
#include "MmLib.h"
#include "MdLib.h"
#include "MbList.h"
#include "MsBitSet.h"
#include "MhPlcInfo.h"

#define		MAXHAIKABE		500									// 配列制限値
#define		EXPKABELN		1000								// 壁線算出用の仮延長量

namespace MC
{

static bool	z_fUpdateKabe = false;								// 壁データ　追加、修正フラグ

////////////////////////////////////////////////////////////////////////////
//    壁データ　追加、修正フラグをセットする
void IeModel::MhKabeSetUpFlg()
{
	z_fUpdateKabe = true;
}

////////////////////////////////////////////////////////////////////////////
//    壁を正規化する(交差部で分割し、直進部で連結する)
//	  事前に　MhKabeSetUpFlg();　で修正があったことを要設定

void IeModel::MhNormKabe(
				MINT		nProc								// 処理数　0 : 全壁処理	
																//		  >0 : 最後のnProc壁を処理する
				)
{
	MhPlcInfo*	pHaiKabe[MAXHAIKABE+2];							// 壁データ
	MPOSITION	pPlcPos[MAXHAIKABE+2];							// 壁データレコード位置
	MINT		fPlc[MAXHAIKABE+2];								// -1: 削除壁, 0: 初期からの有効壁, 1: 追加した有効壁, 
																//  2: 追加壁と端部で交差する有効壁
	MINT		iPlcCn[2];										// 始点、終点での他壁との交差状況
																// -2: 未接続, -1: 交差, >=0: 直進接続
	MINT		nHaiKabe, nHaiKabeI;							// 壁数

	MhPlcInfo	HaiKabeI;										// 追加壁

	MINT		ic1, ic2, icst, ics;
	MgPoint3	po, p0, p1;
	MgLine3		lnKabe1;
	MINT		ist, istr, istl;
	MINT		ist1, ist2;
	MINT		ist1s2, ist1e2, ist2s1, ist2e1;

	MINT		nZukei;											// 図形線分の本数
	MgLine3		lnZukei[6];										// 図形線分
	MINT		szZukei;										// 図形線分データサイズ
	MINT		idivdel;										// 分割削除フラグ 0: 無分割、1: 分割、2: 削除 

	bool		fModify;

	if ( !z_fUpdateKabe) MQUIT;

	//　家モデルよりカレント階の全ての壁を取得する
	MINT iKai = InpMode::GetKai();
	nHaiKabeI = HaitiIn::GetPts( iKai, MP_GP_TAIRYOKU, Mstr( "壁"), NULL, MAXHAIKABE, pHaiKabe, pPlcPos);
																					// 家モデルよりカレント階の全ての壁を取得する
		ASSERT( nHaiKabeI >= 0);													//		壁数オーバーフロー　<ERROR>
	nHaiKabe = nHaiKabeI;
	for ( ic1=nHaiKabe-1; ic1>=0; ic1--) fPlc[ic1] = 0;								// 有効壁

	//	追加修正壁の壁データの先頭位置を求める
	if ( nProc == 0)
		icst = 0;																	// 全壁検索
	else
		icst = max( nHaiKabe - nProc, 0);											// 最後の[nProc]個の壁を追加壁として処理する

	for ( ic1=icst; ic1<nHaiKabe; ic1++) fPlc[ic1] = 1;								// 追加壁

	//	交差壁を分割し、重なり壁を削除する
	ics = icst;
	for ( ic1=0; ic1<nHaiKabe; ic1++) {												// 壁1
		if ( fPlc[ic1] < 0)
			continue;																// 削除壁は比較対象外
		idivdel = 0;
		if ( ic1 >= icst) ics = 0;
		for ( ic2=ics; ic2<nHaiKabe; ic2++) {										// 壁2
			if ( ic1 == ic2 || fPlc[ic2] < 0)
				continue;															// 同一壁と削除壁は比較対象外
			if ( !MGeo::ChkMMR2OnMMR2( MgMinMaxR2( MgLine2C( pHaiKabe[ic1]->m_lnPlc)),
								   MgMinMaxR2( MgLine2C(pHaiKabe[ic2]->m_lnPlc))))
				continue;															// MinMaxの重なりなしの場合は比較対象外
			if ( MGeo::Parallel( pHaiKabe[ic1]->m_lnPlc, pHaiKabe[ic2]->m_lnPlc)) {	// 平行
				ist = MGeo::ChkPt3OnLn3WS( pHaiKabe[ic2]->m_lnPlc.p[0], pHaiKabe[ic1]->m_lnPlc, &ist2s1);	// 壁2の始点が壁1のどこに乗っているか調べる
				ist = MGeo::ChkPt3OnLn3WS( pHaiKabe[ic2]->m_lnPlc.p[1], pHaiKabe[ic1]->m_lnPlc, &ist2e1);	// 壁2の終点が壁1のどこに乗っているか調べる
				ist = MGeo::ChkPt3OnLn3WS( pHaiKabe[ic1]->m_lnPlc.p[0], pHaiKabe[ic2]->m_lnPlc, &ist1s2);	// 壁1の始点が壁2のどこに乗っているか調べる
				ist = MGeo::ChkPt3OnLn3WS( pHaiKabe[ic1]->m_lnPlc.p[1], pHaiKabe[ic2]->m_lnPlc, &ist1e2);	// 壁1の終点が壁2のどこに乗っているか調べる
					// 重なって含まれる壁を削除する
				if ( ( ist1s2 & (MC_ON_PS | MC_INSIDE | MC_ON_PE)) &&				// 壁1が壁2に重なるまたは含まれる場合				*** 外壁、内壁の調査残 ***
					( ist1e2 & (MC_ON_PS | MC_INSIDE | MC_ON_PE))) {
					idivdel = 2;													//		壁1を削除
				} else if ( ist2s1 & MC_INSIDE) {									// 壁2の始点が壁1内で重なっている
					po = pHaiKabe[ic2]->m_lnPlc.p[0];								//		壁1を壁2の始点で分割
					idivdel = 1;
				} else if ( ist2e1 & MC_INSIDE) {									// 壁2の終点が壁1内で重なっている
					po = pHaiKabe[ic2]->m_lnPlc.p[1];								//		壁1を壁2の終点で分割
					idivdel = 1;
				} else {
					continue;														// 重なり無しの壁は分割対象外
				}
			}

			if ( idivdel == 0) {
				ist = MGeo::Intr2Ln3( pHaiKabe[ic1]->m_lnPlc, pHaiKabe[ic2]->m_lnPlc, &po);
				if ( MF_CHECK_OR( ist, (MC_INT | MC_CONNECTION))) {					// 交差
					ist = MGeo::ChkPt3OnLn3WS( po, pHaiKabe[ic1]->m_lnPlc, &ist1);
					if ( ist1 == (MC_ON_LINE | MC_INSIDE)) {						// 壁1の内部で交差があり分割する
						idivdel = 1;
					} else {
						if ( fPlc[ic2] == 1)										// 追加壁と端部で交差する壁
							fPlc[ic1] = 2;
					}
				}
			}

			if ( idivdel == 1) {													// 分割
				ASSERT( nHaiKabe < MAXHAIKABE);										//		壁数オーバーフロー　<ERROR>

				HaiKabeI.Copy( *pHaiKabe[ic1]);										//		分割対象の壁と同一内容の追加壁データを作成する

				fPlc[ic1] = -1;

//D				MINT ist1;
				MhPlcInfo *pPlcEnM;
//D				ist1 = MdPtsModify( pHaiKabe[ic1], &pPlcEnM);						//		修正先レコードを求める
				HaitiDb::MdPtsModify( pHaiKabe[ic1], &pPlcEnM);								//		修正先レコードを求める

//D				if ( ist1 == 3) {
					pPlcEnM->m_lnPlc.p[1] = po;										//		元の壁は始点から交差点まで残す
					pPlcPos[nHaiKabe] = (MPOSITION)( pPlcEnM - HaitiDb::MdGetSizeOfHd());
					pHaiKabe[nHaiKabe] = pPlcEnM;
//D				} else {
//D					pHaiKabe[ic1]->m_lnPlc.p[1] = po;								//		元の壁は始点から交差点まで残す
//D					pPlcPos[nHaiKabe] = pPlcPos[ic1];
//D					pHaiKabe[nHaiKabe] = pHaiKabe[ic1];
//D				}
				fPlc[nHaiKabe] = 0;
				nHaiKabe++;

				HaiKabeI.m_lnPlc.p[0] = po;											//		追加壁は交差点から終点まで残す
				pPlcPos[nHaiKabe] = HaitiDb::MdPtsAdd( &HaiKabeI, 1);						//		分割による壁を追加
				pHaiKabe[nHaiKabe] = (MhPlcInfo*)HaitiDb::MdPtsGet( pPlcPos[nHaiKabe]);

				fPlc[nHaiKabe] = 0;
				nHaiKabe++;
				break;

			} else if ( idivdel == 2) {												// 削除
				HaitiDb::MdPtsDelete( pPlcPos[ic1]);											//		壁1を削除
				fPlc[ic1] = -1;
				break;
			}
		}
	}

	//	壁の連結
	for ( ic1=icst; ic1<nHaiKabe; ic1++) {
		if ( fPlc[ic1] < 0)
			continue;																// 削除壁は比較対象外
		fModify = false;
		iPlcCn[0] = -2;																// 始点側連結壁なし
		iPlcCn[1] = -2;																// 終点側連結壁なし
		lnKabe1 = pHaiKabe[ic1]->m_lnPlc;											// 結合線分算出用

		for ( ic2=0; ic2<nHaiKabe; ic2++) {
			if ( ic1 == ic2 || fPlc[ic2] < 0)
				continue;															// 同一壁と削除壁は比較対象外
			if ( !MGeo::ChkMMR2OnMMR2( MgMinMaxR2( MgLine2C( pHaiKabe[ic1]->m_lnPlc)),
					   MgMinMaxR2( MgLine2C( pHaiKabe[ic2]->m_lnPlc))))
				continue;															// MinMaxの重なりなしの場合は比較対象外
			if ( MGeo::Parallel( pHaiKabe[ic1]->m_lnPlc, pHaiKabe[ic2]->m_lnPlc)) {	// 平行
				ist = MGeo::ChkPt3OnLn3WS( pHaiKabe[ic2]->m_lnPlc.p[0], pHaiKabe[ic1]->m_lnPlc, &ist2s1);	// 壁2の始点が壁1のどこに乗っているか調べる
				ist = MGeo::ChkPt3OnLn3WS( pHaiKabe[ic2]->m_lnPlc.p[1], pHaiKabe[ic1]->m_lnPlc, &ist2e1);	// 壁2の終点が壁1のどこに乗っているか調べる

				if ( ( ist2s1 & (MC_ON_PS | MC_INSIDE | MC_ON_PE)) &&				// 壁2が壁1に重なるまたは含まれる場合は			*** 外壁、内壁の調査残 ***
					( ist2e1 & (MC_ON_PS | MC_INSIDE | MC_ON_PE))) {
					ASSERT( FALSE);													//		重なり部は削除済みなので処理エラー　<ERROR>

				} else if ( (( ist2s1 & MC_ON_PS) || ( ist2e1 & MC_ON_PS)) && ( iPlcCn[0] == -2)) {
																					// 壁1の始点で単一連結
					if ( ist2s1 & MC_ON_PS) {										//		壁2の始点で連結
						lnKabe1.p[0] = pHaiKabe[ic2]->m_lnPlc.p[1];
					} else if ( ist2e1 & MC_ON_PS) {								//		壁2の終点で連結
						lnKabe1.p[0] = pHaiKabe[ic2]->m_lnPlc.p[0];
					}
					iPlcCn[0] = ic2;												//		始点側に連結する壁を設定

				} else if ( (( ist2s1 & MC_ON_PE) || ( ist2e1 & MC_ON_PE)) && ( iPlcCn[1] == -2)) {
																					// 壁1の終点で単一連結
					if ( ist2s1 & MC_ON_PE) {										//		壁2の始点で連結
						lnKabe1.p[1] = pHaiKabe[ic2]->m_lnPlc.p[1];
					} else if ( ist2e1 & MC_ON_PE) {								//		壁2の終点で連結
						lnKabe1.p[1] = pHaiKabe[ic2]->m_lnPlc.p[0];
					}
					iPlcCn[1] = ic2;												// 終点側に連結する壁を設定
				}
				continue;
			}

			ist = MGeo::Intr2Ln3( pHaiKabe[ic1]->m_lnPlc, pHaiKabe[ic2]->m_lnPlc, &po);
			if ( MF_CHECK_OR( ist, (MC_INT | MC_CONNECTION))) {						// 交差( または 連結)(連結は前の平行処理でスキップされここには入らない)
				ist = MGeo::ChkPt3OnLn3WS( po, pHaiKabe[ic1]->m_lnPlc, &ist1);
					if ( ist1 == (MC_ON_LINE | MC_INSIDE))							// 壁内部で交差あり
					ASSERT( FALSE);													//		交差点で分割済みなので処理エラー　<ERROR>
				// 交差壁の図形作成
				if ( ist1 == (MC_ON_LINE | MC_ON_PS)) {
					iPlcCn[0] = -1;													//		始点側に交差する壁ありを設定
				} else if ( ist1 == (MC_ON_LINE | MC_ON_PE)) {
					iPlcCn[1] = -1;													// 		終点側に交差する壁ありを設定
				}
			}
		}
		if ( iPlcCn[0] >= 0) {
			pHaiKabe[ic1]->m_lnPlc.p[0] = lnKabe1.p[0];								// 壁１を延長
			HaitiDb::MdPtsDelete( pPlcPos[iPlcCn[0]]);										// 始点側単一連結壁を削除
			fPlc[iPlcCn[0]] = -1;
			fModify = true;
		}
		if ( iPlcCn[1] >= 0) {
			pHaiKabe[ic1]->m_lnPlc.p[1] = lnKabe1.p[1];								// 壁１を延長
			HaitiDb::MdPtsDelete( pPlcPos[iPlcCn[1]]);										// 終点側単一連結壁を削除
			fPlc[iPlcCn[1]] = -1;
			fModify = true;
		}
		if ( fModify)
			ic1--;
	}

	//	壁の仮の図形作成
	szZukei = sizeof(MhZukei) + sizeof(MgLine3);									// 線分２本分の図形データサイズ

	for ( ic1=0; ic1<nHaiKabe; ic1++) {
		if ( fPlc[ic1] < 0 || (fPlc[ic1] != 2 && ic1 < icst))
			continue;																// 削除壁または追加修正以外の壁で追加修正壁と無交差は対象外
		lnKabe1 = pHaiKabe[ic1]->m_lnPlc;											// 結合線分算出用
		MgVect3	vtutLn1 = MGeo::UnitizeV3( MgVect3C( lnKabe1));
		lnKabe1.p[0] = lnKabe1.p[0] - vtutLn1 * EXPKABELN;
		lnKabe1.p[1] = lnKabe1.p[1] + vtutLn1 * EXPKABELN;

		if ( pHaiKabe[ic1]->m_pZukei == NULL) {
			pHaiKabe[ic1]->m_pZukei = (MhZukei*)new char[szZukei];					// 図形線分
			pHaiKabe[ic1]->m_pZukei->m_pNext = NULL;
		}

		pHaiKabe[ic1]->m_pZukei->m_nZukei = 2;
		pHaiKabe[ic1]->m_pZukei->m_lnZukei[0] = lnKabe1 + vtutLn1.RotR90() * pHaiKabe[ic1]->GetMbWidthR();
		pHaiKabe[ic1]->m_pZukei->m_lnZukei[1] = lnKabe1 + vtutLn1.RotL90() * pHaiKabe[ic1]->GetMbWidthL();
	}

	//	壁の仮の図形を交差壁でカットし正規の図形作成
	for ( ic1=0; ic1<nHaiKabe; ic1++) {
		if ( fPlc[ic1] < 0 || (fPlc[ic1] != 2 && ic1 < icst))
			continue;																// 削除壁または追加修正以外の壁で追加修正壁と無交差は対象外
		iPlcCn[0] = -2;																// 始点側連結壁なし
		iPlcCn[1] = -2;																// 終点側連結壁なし
		nZukei = 2;
		lnZukei[0] = pHaiKabe[ic1]->m_pZukei->m_lnZukei[0];							// 図形線分 右側
		lnZukei[1] = pHaiKabe[ic1]->m_pZukei->m_lnZukei[1];							// 図形線分	左側
		lnKabe1 = pHaiKabe[ic1]->m_lnPlc;											// 壁線形状算出用壁芯線
		for ( ic2=0; ic2<nHaiKabe; ic2++) {
			if ( ic1 == ic2 || fPlc[ic2] < 0)
				continue;															// 同一壁と削除壁は比較対象外

			if ( !MGeo::ChkMMR2OnMMR2( MgMinMaxR2( MgLine2C( pHaiKabe[ic1]->m_lnPlc)),
							MgMinMaxR2( MgLine2C( pHaiKabe[ic2]->m_lnPlc))))
				continue;															// MinMaxの重なりなしの場合は比較対象外
			if ( MGeo::Parallel( pHaiKabe[ic1]->m_lnPlc, pHaiKabe[ic2]->m_lnPlc)) {	// 平行
				ist = MGeo::ChkPt3OnLn3WS( pHaiKabe[ic2]->m_lnPlc.p[0], pHaiKabe[ic1]->m_lnPlc, &ist2s1);	// 壁2の始点が壁1のどこに乗っているか調べる
//				if ( !( ist1 & MC_ON_LINE))
//					continue;														// 離れている壁は結合対象外
				ist = MGeo::ChkPt3OnLn3WS( pHaiKabe[ic2]->m_lnPlc.p[1], pHaiKabe[ic1]->m_lnPlc, &ist2e1);	// 壁2の終点が壁1のどこに乗っているか調べる

				if ( ( ist2s1 & (MC_ON_PS | MC_INSIDE | MC_ON_PE)) &&				// 壁2が壁1に重なるまたは含まれる場合は			*** 外壁、内壁の調査残 ***
					( ist2e1 & (MC_ON_PS | MC_INSIDE | MC_ON_PE))) {
					ASSERT( FALSE);													//		重なり部は削除済みなので処理エラー　<ERROR>

				} else if ( (( ist2s1 & MC_ON_PS) || ( ist2e1 & MC_ON_PS)) && ( iPlcCn[0] == -2)) {
																					// 壁1の始点で単一連結
					iPlcCn[0] = ic2;												//		始点側に連結する壁を設定

				} else if ( (( ist2s1 & MC_ON_PE) || ( ist2e1 & MC_ON_PE)) && ( iPlcCn[1] == -2)) {
																					// 壁1の終点で単一連結
					iPlcCn[1] = ic2;												//		終点側に連結する壁を設定
				}
				continue;
			}

			ist = MGeo::Intr2Ln3( pHaiKabe[ic1]->m_lnPlc, pHaiKabe[ic2]->m_lnPlc, &po);
			if ( MF_CHECK_OR( ist, (MC_INT | MC_CONNECTION))) {						// 交差( または 連結)(連結は前の平行処理でスキップされここには入らない)
				ist = MGeo::ChkPt3OnLn3WS( po, pHaiKabe[ic1]->m_lnPlc, &ist1);
				if ( ist1 == (MC_ON_LINE | MC_INSIDE))								// 壁内部で交差あり
					ASSERT( FALSE);													//		交差点で分割済みなので処理エラー　<ERROR>
				ist = MGeo::ChkPt3OnLn3WS( po, pHaiKabe[ic2]->m_lnPlc, &ist2);
				if ( ist2 == (MC_ON_LINE | MC_INSIDE))								// 壁内部で交差あり
					ASSERT( FALSE);													// 		交差点で分割済みなので処理エラー　<ERROR>

				// 交差壁の図形作成
                MgVect3 vtutLn2 = MGeo::UnitizeV3( MgVect3C( pHaiKabe[ic2]->m_lnPlc));
				MgULine3 ULnR = MgULine3( pHaiKabe[ic2]->m_pZukei->m_lnZukei[0].p[0], vtutLn2);
				MgULine3 ULnL = MgULine3( pHaiKabe[ic2]->m_pZukei->m_lnZukei[1].p[0], vtutLn2);

				if ( ( ist1 & (MC_ON_PS | MC_ON_PE)) == ( ist2 & (MC_ON_PS | MC_ON_PE))) {	// 壁の向きが対向
					istr = MGeo::IntrLnULn3( lnZukei[0], ULnL, &p0);
					istl = MGeo::IntrLnULn3( lnZukei[1], ULnR, &p1);
				} else {															// 壁の向きが同方向
					istr = MGeo::IntrLnULn3( lnZukei[0], ULnR, &p0);
					istl = MGeo::IntrLnULn3( lnZukei[1], ULnL, &p1);
				}
				if ( ist1 & MC_ON_PS) {
					if ( MF_CHECK_OR( istr, (MC_INT | MC_CONNECTION))) lnZukei[0].p[0] = p0;
					if ( MF_CHECK_OR( istl, (MC_INT | MC_CONNECTION))) lnZukei[1].p[0] = p1;
					iPlcCn[0] = -1;													// 始点点側に交差する壁ありを設定
				} else if ( ist1 == (MC_ON_LINE | MC_ON_PE)) {
					if ( MF_CHECK_OR( istr, (MC_INT | MC_CONNECTION))) lnZukei[0].p[1] = p0;
					if ( MF_CHECK_OR( istl, (MC_INT | MC_CONNECTION))) lnZukei[1].p[1] = p1;
					iPlcCn[1] = -1;													// 終点側に交差する壁ありを設定
				}
			}
		}

		if ( iPlcCn[0] == -2 || iPlcCn[1] == -2) {
			MgVect3	vtutLn1 = MGeo::UnitizeV3( MgVect3C( lnKabe1));
			lnKabe1.p[0] = lnKabe1.p[0] - vtutLn1 * 44.5;
			lnKabe1.p[1] = lnKabe1.p[1] + vtutLn1 * 44.5;

			if ( iPlcCn[0] == -2) {													// 始点が開放端
				lnZukei[0].p[0] = lnKabe1.p[0] + vtutLn1.RotR90() * pHaiKabe[ic1]->GetMbWidthR();
				lnZukei[1].p[0] = lnKabe1.p[0] + vtutLn1.RotL90() * pHaiKabe[ic1]->GetMbWidthL();
				lnZukei[nZukei].p[0] = lnZukei[1].p[0];
				lnZukei[nZukei].p[1] = lnZukei[0].p[0];
				nZukei++;
			}
			if ( iPlcCn[1] == -2) {													// 終点が開放端
				lnZukei[0].p[1] = lnKabe1.p[1] + vtutLn1.RotR90() * pHaiKabe[ic1]->GetMbWidthR();
				lnZukei[1].p[1] = lnKabe1.p[1] + vtutLn1.RotL90() * pHaiKabe[ic1]->GetMbWidthL();
				lnZukei[nZukei].p[0] = lnZukei[0].p[1];
				lnZukei[nZukei].p[1] = lnZukei[1].p[1];
				nZukei++;
			}
		}

		if ( nZukei > pHaiKabe[ic1]->m_pZukei->m_nZukei) {
			szZukei = sizeof(MhZukei) + sizeof(MgLine3) * (nZukei - 1);				// 線分nZukei本分の図形データサイズ
			MBFREE(pHaiKabe[ic1]->m_pZukei);
			pHaiKabe[ic1]->m_pZukei = (MhZukei*)new char[szZukei];					// 図形線分
			pHaiKabe[ic1]->m_pZukei->m_pNext = NULL;
		}

		lnZukei[1].SetRev();
		for ( ic2=0; ic2<nZukei; ic2++)
			pHaiKabe[ic1]->m_pZukei->m_lnZukei[ic2] = lnZukei[ic2];
		pHaiKabe[ic1]->m_pZukei->m_nZukei = nZukei;
	}

	z_fUpdateKabe = false;
exit:;
//DBG #ifdef _DEBUG
//DBG	Msprintf( MbLog::m_Str, Mstr( "MhNormKabe ===================================\n"));
//DBG	MBTRCPRBF;
//DBG	BuzaiCode::MhPrintallMhPlcInfo( Mstr( "MhNormKabe"));
//DBG #endif
}

} // namespace MC
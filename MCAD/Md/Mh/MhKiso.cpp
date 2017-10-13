//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MhKiso.cpp
//
//		基礎データを正規化する
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"
#include "MgLib.h"

#define DLL_EXPORT_IEMODEL_DO
#include "MhLib.h"
#include "MmDefine.h"
#include "MmLib.h"
#include "MdLib.h"
#include "MbList.h"
#include "MsBitSet.h"
#include "MhPlcInfo.h"

#define		MAXHAIKISO		500									// 配列制限値
#define		EXPKISOLN		1000								// 基礎線算出用の仮延長量

namespace MC
{

static bool	z_fUpdateKiso = false;								// 基礎データ　追加、修正フラグ

////////////////////////////////////////////////////////////////////////////
//    基礎データ　追加、修正フラグをセットする
void IeModel::MhKisoSetUpFlg()
{
	z_fUpdateKiso = true;
}

////////////////////////////////////////////////////////////////////////////
//    基礎を正規化する(交差部で分割し、直進部で連結する)
//	  事前に　MhKisoSetUpFlg();　で修正があったことを要設定

void IeModel::MhNormKiso(
				MINT		nProc								// 処理数　0 : 全基礎処理	
																//		  >0 : 最後のnProc基礎を処理する
				)
{
	MhPlcInfo* pHaiKiso[MAXHAIKISO+2];						// 基礎データ
	MPOSITION	pPlcPos[MAXHAIKISO+2];						// 基礎データレコード位置
	MINT		fPlc[MAXHAIKISO+2];							// -1: 削除基礎, 0: 初期からの有効基礎, 1: 追加した有効基礎, 
																//  2: 追加基礎と端部で交差する有効基礎
	MINT		iPlcCn[2];									// 始点、終点での他基礎との交差状況
																// -2: 未接続, -1: 交差, >=0: 直進接続
	MINT		nHaiKiso, nHaiKisoI;							// 基礎数
	MhPlcInfo	HaiKisoI;										// 追加基礎

	MINT		ic1, ic2, icst, ics;
	MgPoint3	po, p0, p1;
	MgLine3		lnKiso1;
	MINT		ist, istr, istl;
	MINT		ist1, ist2;
	MINT		ist1s2, ist1e2, ist2s1, ist2e1;

	MINT		nZukei;											// 図形線分の本数
	MgLine3		lnZukei[6];										// 図形線分
	MINT		szZukei;										// 図形線分データサイズ
	MINT		idivdel;										// 分割削除フラグ 0: 無分割、1: 分割、2: 削除 

	bool		fModify;

//	if ( !z_fUpdateKiso && nProc != 0) MQUIT;
	if ( !z_fUpdateKiso) MQUIT;

	//　家モデルよりカレント階の全ての基礎を取得する
	MINT iKai = InpMode::GetKai();
	nHaiKisoI = HaitiIn::GetPts( iKai, MP_GP_KISO, Mstr( "基礎"), NULL, MAXHAIKISO, pHaiKiso, pPlcPos);
																					// 家モデルよりカレント階の全ての基礎を取得する
		ASSERT( nHaiKisoI >= 0);													//		基礎数オーバーフロー　<ERROR>
	nHaiKiso = nHaiKisoI;
	for ( ic1=nHaiKiso-1; ic1>=0; ic1--) fPlc[ic1] = 0;							// 有効基礎

	//	追加修正基礎の基礎データの先頭位置を求める
	if ( nProc == 0)
		icst = 0;																	// 全基礎検索
	else
		icst = max( nHaiKiso - nProc, 0);											// 最後の[nProc]個の基礎が追加修正基礎として処理する

	for ( ic1=icst; ic1<nHaiKiso; ic1++) fPlc[ic1] = 1;							// 追加基礎

	//	交差基礎を分割し、重なり基礎を削除する
	ics = icst;
	for ( ic1=0; ic1<nHaiKiso; ic1++) {												// 基礎1
		if ( fPlc[ic1] < 0)
			continue;																// 削除基礎は比較対象外
		idivdel = 0;
		if ( ic1 >= icst) ics = 0;
		for ( ic2=ics; ic2<nHaiKiso; ic2++) {										// 基礎2
			if ( ic1 == ic2 || fPlc[ic2] < 0)
				continue;															// 同一基礎と削除基礎は比較対象外
			if ( !MGeo::ChkMMR2OnMMR2( MgMinMaxR2( MgLine2C( pHaiKiso[ic1]->m_lnPlc)),
						    MgMinMaxR2( MgLine2C(pHaiKiso[ic2]->m_lnPlc))))
				continue;															// MinMaxの重なりなしの場合は比較対象外
			if ( MGeo::Parallel( pHaiKiso[ic1]->m_lnPlc, pHaiKiso[ic2]->m_lnPlc)) {	// 平行
				ist = MGeo::ChkPt3OnLn3WS( pHaiKiso[ic2]->m_lnPlc.p[0], pHaiKiso[ic1]->m_lnPlc, &ist2s1);	// 基礎2の始点が基礎1のどこに乗っているか調べる
				ist = MGeo::ChkPt3OnLn3WS( pHaiKiso[ic2]->m_lnPlc.p[1], pHaiKiso[ic1]->m_lnPlc, &ist2e1);	// 基礎2の終点が基礎1のどこに乗っているか調べる
				ist = MGeo::ChkPt3OnLn3WS( pHaiKiso[ic1]->m_lnPlc.p[0], pHaiKiso[ic2]->m_lnPlc, &ist1s2);	// 基礎1の始点が基礎2のどこに乗っているか調べる
				ist = MGeo::ChkPt3OnLn3WS( pHaiKiso[ic1]->m_lnPlc.p[1], pHaiKiso[ic2]->m_lnPlc, &ist1e2);	// 基礎1の終点が基礎2のどこに乗っているか調べる
					// 重なって含まれる基礎を削除する
				if ( ( ist1s2 & (MC_ON_PS | MC_INSIDE | MC_ON_PE)) &&					//		基礎1が基礎2に重なるまたは含まれる場合				*** 外基礎、内基礎の調査残 ***
					( ist1e2 & (MC_ON_PS | MC_INSIDE | MC_ON_PE))) {
					idivdel = 2;													//			基礎1を削除
				} else if ( ist2s1 & MC_INSIDE) {									//		基礎2の始点が基礎1内で重なっている
					po = pHaiKiso[ic2]->m_lnPlc.p[0];								//			基礎1を基礎2の始点で分割
					idivdel = 1;
				} else if ( ist2e1 & MC_INSIDE) {									//		基礎2の終点が基礎1内で重なっている
					po = pHaiKiso[ic2]->m_lnPlc.p[1];								//			基礎1を基礎2の終点で分割
					idivdel = 1;
				} else {
					continue;														//		重なり無しの基礎は分割対象外
				}
			}

			if ( idivdel == 0) {
				ist = MGeo::Intr2Ln3( pHaiKiso[ic1]->m_lnPlc, pHaiKiso[ic2]->m_lnPlc, &po);
				if ( MF_CHECK_OR( ist, (MC_INT | MC_CONNECTION))) {					// 交差
					ist = MGeo::ChkPt3OnLn3WS( po, pHaiKiso[ic1]->m_lnPlc, &ist1);
					if ( ist1 == (MC_ON_LINE | MC_INSIDE)) {							//		基礎1の内部で交差があり分割する
						idivdel = 1;
					} else {
						if ( fPlc[ic2] == 1)										//		追加基礎と端部で交差する基礎
							fPlc[ic1] = 2;
					}
				}
			}

			if ( idivdel == 1) {														// 分割
				ASSERT( nHaiKiso < MAXHAIKISO);										//		基礎数オーバーフロー　<ERROR>

				HaiKisoI = *pHaiKiso[ic1];											//		分割対象の基礎と同一内容の追加基礎データを作成する
				HaiKisoI.m_pZukei = NULL;

				fPlc[ic1] = -1;

				pHaiKiso[ic1]->m_lnPlc.p[1] = po;									//		元の基礎は始点から交差点まで残す
				pPlcPos[nHaiKiso] = pPlcPos[ic1];
				pHaiKiso[nHaiKiso] = pHaiKiso[ic1];
				fPlc[nHaiKiso] = 0;
				nHaiKiso++;

				HaiKisoI.m_lnPlc.p[0] = po;										//		追加基礎は交差点から終点まで残す
				pPlcPos[nHaiKiso] = HaitiDb::MdPtsAdd( &HaiKisoI, 1);						//		分割による基礎を追加
				pHaiKiso[nHaiKiso] = (MhPlcInfo*)HaitiDb::MdPtsGet( pPlcPos[nHaiKiso]);
				fPlc[nHaiKiso] = 0;
				nHaiKiso++;
				break;

			} else if ( idivdel == 2) {												// 削除
				HaitiDb::MdPtsDelete( pPlcPos[ic1]);										//		基礎1を削除
				fPlc[ic1] = -1;
				break;
			}
		}
	}

	//	基礎の連結
	for ( ic1=icst; ic1<nHaiKiso; ic1++) {
		if ( fPlc[ic1] < 0)
			continue;																// 削除基礎は比較対象外
		fModify = false;
		iPlcCn[0] = -2;															// 始点側連結基礎なし
		iPlcCn[1] = -2;															// 終点側連結基礎なし
		lnKiso1 = pHaiKiso[ic1]->m_lnPlc;											// 結合線分算出用

		for ( ic2=0; ic2<nHaiKiso; ic2++) {
			if ( ic1 == ic2 || fPlc[ic2] < 0)
				continue;															// 同一基礎と削除基礎は比較対象外
			if ( !MGeo::ChkMMR2OnMMR2( MgMinMaxR2( MgLine2C( pHaiKiso[ic1]->m_lnPlc)),
							MgMinMaxR2( MgLine2C( pHaiKiso[ic2]->m_lnPlc))))
				continue;															// MinMaxの重なりなしの場合は比較対象外
			if ( MGeo::Parallel( pHaiKiso[ic1]->m_lnPlc, pHaiKiso[ic2]->m_lnPlc)) {	// 平行
				ist = MGeo::ChkPt3OnLn3WS( pHaiKiso[ic2]->m_lnPlc.p[0], pHaiKiso[ic1]->m_lnPlc, &ist2s1);	// 基礎2の始点が基礎1のどこに乗っているか調べる
				ist = MGeo::ChkPt3OnLn3WS( pHaiKiso[ic2]->m_lnPlc.p[1], pHaiKiso[ic1]->m_lnPlc, &ist2e1);	// 基礎2の終点が基礎1のどこに乗っているか調べる

				if ( ( ist2s1 & (MC_ON_PS | MC_INSIDE | MC_ON_PE)) &&					//		基礎2が基礎1に重なるまたは含まれる場合は			*** 外基礎、内基礎の調査残 ***
					( ist2e1 & (MC_ON_PS | MC_INSIDE | MC_ON_PE))) {
					ASSERT( FALSE);													//			重なり部は削除済みなので処理エラー　<ERROR>

				} else if ( (( ist2s1 & MC_ON_PS) || ( ist2e1 & MC_ON_PS)) && ( iPlcCn[0] == -2)) {
																					//		基礎1の始点で単一連結
					if ( ist2s1 & MC_ON_PS) {										//			基礎2の始点で連結
						lnKiso1.p[0] = pHaiKiso[ic2]->m_lnPlc.p[1];
//						fPlc[ic2] = 2;											// 追加修正基礎と交差ありを設定
					} else if ( ist2e1 & MC_ON_PS) {									//			基礎2の終点で連結
						lnKiso1.p[0] = pHaiKiso[ic2]->m_lnPlc.p[0];
//						fPlc[ic2] = 2;											// 追加修正基礎と交差ありを設定
					}
					iPlcCn[0] = ic2;												//			始点側に連結する基礎を設定

				} else if ( (( ist2s1 & MC_ON_PE) || ( ist2e1 & MC_ON_PE)) && ( iPlcCn[1] == -2)) {
																					//		基礎1の終点で単一連結
					if ( ist2s1 & MC_ON_PE) {										//			基礎2の始点で連結
						lnKiso1.p[1] = pHaiKiso[ic2]->m_lnPlc.p[1];
//						fPlc[ic2] = 2;											// 追加修正基礎と交差ありを設定
					} else if ( ist2e1 & MC_ON_PE) {									//			基礎2の終点で連結
						lnKiso1.p[1] = pHaiKiso[ic2]->m_lnPlc.p[0];
//						fPlc[ic2] = 2;											// 追加修正基礎と交差ありを設定
					}
					iPlcCn[1] = ic2;												//			終点側に連結する基礎を設定
				}
				continue;
			}

			ist = MGeo::Intr2Ln3( pHaiKiso[ic1]->m_lnPlc, pHaiKiso[ic2]->m_lnPlc, &po);
			if ( MF_CHECK_OR( ist, (MC_INT | MC_CONNECTION))) {							// 交差( または 連結)(連結は前の平行処理でスキップされここには入らない)
				ist = MGeo::ChkPt3OnLn3WS( po, pHaiKiso[ic1]->m_lnPlc, &ist1);
					if ( ist1 == (MC_ON_LINE | MC_INSIDE))							//		基礎内部で交差あり
					ASSERT( FALSE);													//			交差点で分割済みなので処理エラー　<ERROR>
				// 交差基礎の図形作成
				if ( ist1 == (MC_ON_LINE | MC_ON_PS)) {
					iPlcCn[0] = -1;												//			始点側に交差する基礎ありを設定
				} else if ( ist1 == (MC_ON_LINE | MC_ON_PE)) {
					iPlcCn[1] = -1;												//			終点側に交差する基礎ありを設定
				}
//				fPlc[ic2] = 2;													// 追加修正基礎と交差ありを設定
			}
		}
		if ( iPlcCn[0] >= 0) {
			pHaiKiso[ic1]->m_lnPlc.p[0] = lnKiso1.p[0];							// 基礎１を延長
			HaitiDb::MdPtsDelete( pPlcPos[iPlcCn[0]]);									// 始点側単一連結基礎を削除
			fPlc[iPlcCn[0]] = -1;
			fModify = true;
		}
		if ( iPlcCn[1] >= 0) {
			pHaiKiso[ic1]->m_lnPlc.p[1] = lnKiso1.p[1];							// 基礎１を延長
			HaitiDb::MdPtsDelete( pPlcPos[iPlcCn[1]]);									// 終点側単一連結基礎を削除
			fPlc[iPlcCn[1]] = -1;
			fModify = true;
		}
		if ( fModify)
			ic1--;
//		if ( iPlcCn[0] != 0 || iPlcCn[1] != 0)
//			fPlc[ic2] = 2;														// 追加修正基礎と交差ありを設定
	}

	//	基礎の仮の図形作成
	szZukei = sizeof(MhZukei) + sizeof(MgLine3);									// 線分２本分の図形データサイズ

	for ( ic1=0; ic1<nHaiKiso; ic1++) {
		if ( fPlc[ic1] < 0 || (fPlc[ic1] != 2 && ic1 < icst))
			continue;																// 削除基礎または追加修正以外の基礎で追加修正基礎と無交差は対象外
		lnKiso1 = pHaiKiso[ic1]->m_lnPlc;											// 結合線分算出用
		MgVect3	vtutLn1 = MGeo::UnitizeV3( MgVect3C( lnKiso1));
		lnKiso1.p[0] = lnKiso1.p[0] - vtutLn1 * EXPKISOLN;
		lnKiso1.p[1] = lnKiso1.p[1] + vtutLn1 * EXPKISOLN;

		if ( pHaiKiso[ic1]->m_pZukei == NULL) {
			pHaiKiso[ic1]->m_pZukei = (MhZukei*)new char[szZukei];					// 図形線分
			pHaiKiso[ic1]->m_pZukei->m_pNext = NULL;
		}

		pHaiKiso[ic1]->m_pZukei->m_nZukei = 2;
		pHaiKiso[ic1]->m_pZukei->m_lnZukei[0] = lnKiso1 + vtutLn1.RotR90() * pHaiKiso[ic1]->GetMbWidthR();
		pHaiKiso[ic1]->m_pZukei->m_lnZukei[1] = lnKiso1 + vtutLn1.RotL90() * pHaiKiso[ic1]->GetMbWidthL();
	}

	//	基礎の仮の図形を交差基礎でカットし正規の図形作成
	for ( ic1=0; ic1<nHaiKiso; ic1++) {
		if ( fPlc[ic1] < 0 || (fPlc[ic1] != 2 && ic1 < icst))
			continue;																// 削除基礎または追加修正以外の基礎で追加修正基礎と無交差は対象外
		iPlcCn[0] = -2;															// 始点側連結基礎なし
		iPlcCn[1] = -2;															// 終点側連結基礎なし
		nZukei = 2;
		lnZukei[0] = pHaiKiso[ic1]->m_pZukei->m_lnZukei[0];							// 図形線分 右側
		lnZukei[1] = pHaiKiso[ic1]->m_pZukei->m_lnZukei[1];							// 図形線分	左側
		lnKiso1 = pHaiKiso[ic1]->m_lnPlc;											// 基礎線形状算出用基礎芯線
		for ( ic2=0; ic2<nHaiKiso; ic2++) {
			if ( ic1 == ic2 || fPlc[ic2] < 0)
				continue;															// 同一基礎と削除基礎は比較対象外

			if ( !MGeo::ChkMMR2OnMMR2( MgMinMaxR2( MgLine2C( pHaiKiso[ic1]->m_lnPlc)),
							MgMinMaxR2( MgLine2C( pHaiKiso[ic2]->m_lnPlc))))
				continue;															// MinMaxの重なりなしの場合は比較対象外
			if ( MGeo::Parallel( pHaiKiso[ic1]->m_lnPlc, pHaiKiso[ic2]->m_lnPlc)) {	// 平行
				ist = MGeo::ChkPt3OnLn3WS( pHaiKiso[ic2]->m_lnPlc.p[0], pHaiKiso[ic1]->m_lnPlc, &ist2s1);	// 基礎2の始点が基礎1のどこに乗っているか調べる
//				if ( !( ist1 & MC_ON_LINE))
//					continue;														//		離れている基礎は結合対象外
				ist = MGeo::ChkPt3OnLn3WS( pHaiKiso[ic2]->m_lnPlc.p[1], pHaiKiso[ic1]->m_lnPlc, &ist2e1);	// 基礎2の終点が基礎1のどこに乗っているか調べる

				if ( ( ist2s1 & (MC_ON_PS | MC_INSIDE | MC_ON_PE)) &&					//		基礎2が基礎1に重なるまたは含まれる場合は			*** 外基礎、内基礎の調査残 ***
					( ist2e1 & (MC_ON_PS | MC_INSIDE | MC_ON_PE))) {
					ASSERT( FALSE);													//			重なり部は削除済みなので処理エラー　<ERROR>

				} else if ( (( ist2s1 & MC_ON_PS) || ( ist2e1 & MC_ON_PS)) && ( iPlcCn[0] == -2)) {
																					//		基礎1の始点で単一連結
					iPlcCn[0] = ic2;												//			始点側に連結する基礎を設定

				} else if ( (( ist2s1 & MC_ON_PE) || ( ist2e1 & MC_ON_PE)) && ( iPlcCn[1] == -2)) {
																					//		基礎1の終点で単一連結
					iPlcCn[1] = ic2;												//			終点側に連結する基礎を設定
				}
				continue;
			}

			ist = MGeo::Intr2Ln3( pHaiKiso[ic1]->m_lnPlc, pHaiKiso[ic2]->m_lnPlc, &po);
			if ( MF_CHECK_OR( ist, (MC_INT | MC_CONNECTION))) {							// 交差( または 連結)(連結は前の平行処理でスキップされここには入らない)
				ist = MGeo::ChkPt3OnLn3WS( po, pHaiKiso[ic1]->m_lnPlc, &ist1);
				if ( ist1 == (MC_ON_LINE | MC_INSIDE))								//		基礎内部で交差あり
					ASSERT( FALSE);													//			交差点で分割済みなので処理エラー　<ERROR>
				ist = MGeo::ChkPt3OnLn3WS( po, pHaiKiso[ic2]->m_lnPlc, &ist2);
				if ( ist2 == (MC_ON_LINE | MC_INSIDE))								//		基礎内部で交差あり
					ASSERT( FALSE);													//			交差点で分割済みなので処理エラー　<ERROR>

				// 交差基礎の図形作成
                MgVect3 vtutLn2 = MGeo::UnitizeV3( MgVect3C( pHaiKiso[ic2]->m_lnPlc));
				MgULine3 ULnR = MgULine3( pHaiKiso[ic2]->m_pZukei->m_lnZukei[0].p[0], vtutLn2);
				MgULine3 ULnL = MgULine3( pHaiKiso[ic2]->m_pZukei->m_lnZukei[1].p[0], vtutLn2);

				if ( ( ist1 & (MC_ON_PS | MC_ON_PE)) == ( ist2 & (MC_ON_PS | MC_ON_PE))) {	// 基礎の向きが対向
					istr = MGeo::IntrLnULn3( lnZukei[0], ULnL, &p0);
					istl = MGeo::IntrLnULn3( lnZukei[1], ULnR, &p1);
				} else {																// 基礎の向きが同方向
					istr = MGeo::IntrLnULn3( lnZukei[0], ULnR, &p0);
					istl = MGeo::IntrLnULn3( lnZukei[1], ULnL, &p1);
				}
				if ( ist1 & MC_ON_PS) {
					if ( MF_CHECK_OR( istr, (MC_INT | MC_CONNECTION)))
						lnZukei[0].p[0] = p0;
					if ( MF_CHECK_OR( istl, (MC_INT | MC_CONNECTION)))
						lnZukei[1].p[0] = p1;
					iPlcCn[0] = -1;												//			始点点側に交差する基礎ありを設定
				} else if ( ist1 == (MC_ON_LINE | MC_ON_PE)) {
					if ( MF_CHECK_OR( istr, (MC_INT | MC_CONNECTION)))
						lnZukei[0].p[1] = p0;
					if ( MF_CHECK_OR( istl, (MC_INT | MC_CONNECTION)))
						lnZukei[1].p[1] = p1;
					iPlcCn[1] = -1;												//			終点側に交差する基礎ありを設定
				}
			}
		}

		if ( iPlcCn[0] == -2 || iPlcCn[1] == -2) {
			MgVect3	vtutLn1 = MGeo::UnitizeV3( MgVect3C( lnKiso1));
			lnKiso1.p[0] = lnKiso1.p[0] - vtutLn1 * 60.0;
			lnKiso1.p[1] = lnKiso1.p[1] + vtutLn1 * 60.0;

			if ( iPlcCn[0] == -2) {													// 始点が開放端
				lnZukei[0].p[0] = lnKiso1.p[0] + vtutLn1.RotR90() * pHaiKiso[ic1]->GetMbWidthR();
				lnZukei[1].p[0] = lnKiso1.p[0] + vtutLn1.RotL90() * pHaiKiso[ic1]->GetMbWidthL();
				lnZukei[nZukei].p[0] = lnZukei[1].p[0];
				lnZukei[nZukei].p[1] = lnZukei[0].p[0];
				nZukei++;

			}
			if ( iPlcCn[1] == -2) {													// 終点が開放端
				lnZukei[0].p[1] = lnKiso1.p[1] + vtutLn1.RotR90() * pHaiKiso[ic1]->GetMbWidthR();
				lnZukei[1].p[1] = lnKiso1.p[1] + vtutLn1.RotL90() * pHaiKiso[ic1]->GetMbWidthL();
				lnZukei[nZukei].p[0] = lnZukei[0].p[1];
				lnZukei[nZukei].p[1] = lnZukei[1].p[1];
				nZukei++;
			}
		}

		if ( nZukei > pHaiKiso[ic1]->m_pZukei->m_nZukei) {
			szZukei = sizeof(MhZukei) + sizeof(MgLine3) * (nZukei - 1);				// 線分nZukei本分の図形データサイズ
			MBFREE(pHaiKiso[ic1]->m_pZukei);
			pHaiKiso[ic1]->m_pZukei = (MhZukei*)new char[szZukei];					// 図形線分
			pHaiKiso[ic1]->m_pZukei->m_pNext = NULL;
		}

		lnZukei[1].SetRev();
		for ( ic2=0; ic2<nZukei; ic2++)
			pHaiKiso[ic1]->m_pZukei->m_lnZukei[ic2] = lnZukei[ic2];
		pHaiKiso[ic1]->m_pZukei->m_nZukei = nZukei;
	}

	z_fUpdateKiso = false;
exit:
	;
}

} // namespace MC
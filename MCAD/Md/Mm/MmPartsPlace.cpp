//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MmPtsPlc.cpp
//
//		部品配置操作
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"
#include "MgLib.h"
#include "MgMat.h"
#include "MbDefine.h"
#include "MbCod.h"
#include "MmGridNum.h"
#include "MmDrag.h"
#include "MmWnd.h"
#include "MmDefine.h"

#define DLL_EXPORT_MC_HAITIIN_DO
#include "MmLib.h"

#include "MmSystemProperty.h"

#include "resource.h"
#include "..\\MCAD\\resource.h"

#include "MsBitSet.h"
#include "MbList.h"
#include "MhLib.h"

#include "MhInp.h"

#define	DLL_EXPORT_MC_INPATTR_DO 
#include "MmInpAttr.h"

#include "MmOpt.h"
#include "MdLib.h"

#include "MmHist.h"
#include "MmValid.h"

#define	 DLL_EXPORT_HAITICMD_DO
#include "MmCmd.h"

#include "MhPts1.h"

namespace MC
{

/////////////////////////////////////////////////////////////////////////////
//	部品を１本配置する
void HaitiCmd::MmPtsPlc(
				const	MgPoint3	*Pt,		// (I  ) 配置点、配置方向点
				const	MgVect3		vUpPlc,		// (I  ) 配置上方向
				const	MgPolyg2	*ppg		// (I  ) 図形用の区画　または NULL
				)
{
	MhPlcInfo	PlcEn;
	MhZukei*	pZukei;
	MINT		szZukei;
	MgVect3		vBz = Pt[1] - Pt[0];
	MREAL		rBzLng_2 = vBz * vBz;
	if ( rBzLng_2 < MMIN_BZILEN_2) {
		//	部材長さ不足エラー表示 **************************************
		MQUIT;
	}
/*D
	MsBitSet	*pOptv;
	MsBitSet	*pHstv;

	MhAddOpt( &pOptv);							// Opt対応　新規追加レコード
	PlcEn.SetPIOpt1( pOptv);					// Opt対応
	MhHistAdd( &pHstv);							// 履歴管理対応　新規追加レコード
	PlcEn.SetPIVer( pHstv);						// 履歴管理対応
*/
//D	MdPtsAdd( &PlcEn, 1);
	
	PlcEn.SetPIKai( MhPlcInp::GetKai());
	PlcEn.SetPIIdTpPts( MhPlcInp::GetIdTpPts());
	PlcEn.SetPITpPts( MhPlcInp::GetTpPts());
	PlcEn.SetPIIdMbr( MhPlcInp::GetIdMbr());
	PlcEn.SetPIMbr( MhPlcInp::GetMbr());
	PlcEn.SetPILenHosei( 0, MhPlcInp::GetLenHosei( 0));
	PlcEn.SetPILenHosei( 1, MhPlcInp::GetLenHosei( 1));
	if ( PlcEn.IsPanel() || PlcEn.IsKaiko()) {
		PlcEn.SetPIMaeHosei( MhPlcInp::GetMaeHosei());
		PlcEn.SetPIOku( MhPlcInp::GetOku());
		PlcEn.SetPIOkuHosei(MhPlcInp::GetOkuHosei());
		PlcEn.SetPIHgt( 0, MhPlcInp::GetHgt( 0));
	} else {
		PlcEn.SetPISinZure( MhPlcInp::GetSinZure());
		PlcEn.SetPIPlcZure( MhPlcInp::GetPlcZure());
		PlcEn.SetPIHgt( 0, MhPlcInp::GetHgt( 0));
		PlcEn.SetPIHgt( 1, MhPlcInp::GetHgt( 1));
	}

	if ( PlcEn.IsFrame()) {										// たて枠 | 束　の上端高さを設定
		if ( MGeo::GT( PlcEn.m_rHgt[1], 0.f)) {
			PlcEn.SetPIHgt( 1, PlcEn.m_rHgt[1]);
		} else {
			PlcEn.SetPIHgt( 1, MmSystemProperty::GetStnd( PlcEn.m_iKai, MM_STNDH_CEILING) -	// 天井基準
							   MmSystemProperty::GetStnd( PlcEn.m_iKai, MM_STNDH_LOWER) +		// 下基準
							   PlcEn.m_rHgt[1]);
		}
	}
	PlcEn.SetPIPanelNo( MhPlcInp::GetPanelNo());
	PlcEn.SetPIKaikoNo( MhPlcInp::GetKaikoNo());

	PlcEn.SetPIPlcIti( 0, Pt[0]);
	PlcEn.SetPIPlcIti( 1, Pt[1]);
	PlcEn.SetPIUpPlc( vUpPlc);

	PlcEn.SetPIZukei( NULL);
	PlcEn.SetPIAuxAttr( MhPlcInp::GetpAuxAttr());
	MhPlcInp::SetpAuxAttr( NULL);

	if ( ppg != NULL && ppg->m_n>=2) {							// 図形用の区画データあり
		MINT nLine = ppg->m_n;
		if ( ppg->m_n == 2) nLine--;								// 2点の場合は直線1本

		szZukei = sizeof(MhZukei) + sizeof(MgLine3) * (nLine - 1);			// 線分[nLine]本分の図形データサイズ
		pZukei = (MhZukei*)new char[szZukei];

		pZukei->m_obj = MHZK_DISP;								// 図形表示用
		pZukei->m_iZTp = MHZK_GLINE;								// 線分群

		pZukei->m_pNext = NULL;
		pZukei->m_nZukei = nLine;
		for (MINT ic1=0; ic1<nLine; ic1++) {
			MINT ic2 = ic1 + 1;
			if ( ic2 == nLine) ic2 = 0;
			pZukei->m_lnZukei[ic1] = MgLine3( MgVect3C( ppg->m_p[ic1]), MgVect3C( ppg->m_p[ic2]));
		}
		PlcEn.m_pZukei = pZukei;
		//
		szZukei = sizeof(MhZukei) - sizeof(MgLine3) + sizeof(MgPoint3) * (ppg->m_n);	// 点[ppg->n]個分の図形データサイズ
		pZukei = (MhZukei*)new char[szZukei];

		pZukei->m_obj = MHZK_AREA;								// 図形領域判定用
		pZukei->m_iZTp = MHZK_GPOINT;								// ポリゴン(点群)

		pZukei->m_pNext = NULL;
		pZukei->m_nZukei = ppg->m_n;
		MgPoint2* ppt = (MgPoint2*)&pZukei->m_lnZukei[0];
		for (MINT ic1=0; ic1<ppg->m_n; ic1++) {
			ppt[ic1] = ppg->m_p[ic1];
		}
		PlcEn.m_pZukei->m_pNext = pZukei;
	}
//D	MdInjectPts( &PlcEn);
	HaitiDb::MdPtsAdd( &PlcEn, 1);
exit:;
}

/////////////////////////////////////////////////////////////////////////////
//	部品配置を取得する
//	返値 =-1: オーバーフロー, ≧0: 選択部品配置数
MINT HaitiIn::GetPts(
						MINT		iKai,			// (I  ) 階
						MINT		iIdTpPts,		// (I  ) 部品ID
				const	MCHAR*		cGeneralName,	// (I  ) 総称 または NULL
				const	MCHAR*		cNmPts1,		// (I  ) 操作用部材名 または NULL
						MINT		szPlcEn,		// (I  ) 部品配置最大数
						MhPlcInfo*	*pPlcEno,		// (  O) 選択部品配置
						MPOSITION	*pPlcpos		// (  O) 選択[部品配置]位置 または NULL
				)
{
	MPOSITION	pos1;
	MhPlcInfo* pPlcEn;
//	MsBitSet	*pOptv;
//	MsBitSet*	pHstv;

//	MINT szHE = szPlcEn - 1;
	MINT szHE = szPlcEn;

	MINT iHE = 0;
	for (pPlcEn = HaitiDb::MdGetHeadPts( &pos1); pPlcEn!=0;
		 pPlcEn = HaitiDb::MdGetNextPts( &pos1)) {
		if ( !HaitiIn::ChkPts( iKai, iIdTpPts, cGeneralName, cNmPts1, pPlcEn))
			continue;											// 対象外部品配置
		if ( !MmChkValidParts( pPlcEn))							// オプションと履歴のチェック
			continue;

		if ( iHE >= szHE) {										// 選択部品配置　オーバフロー　<ERROR>
			iHE = -1;
			break;
		}
		pPlcEno[iHE] = pPlcEn;
		if ( pPlcpos)
			pPlcpos[iHE] = pos1;
		iHE++;
	}
	return iHE;
}

/////////////////////////////////////////////////////////////////////////////
//	部品配置を調べる
//	返値 true : 対象部品配置, false : 対象外部品配置
bool HaitiIn::ChkPts(
						MINT		iKai,			// (I  ) 階 または NULL
						MINT		iIdTpPts,		// (I  ) 部品ID または NULL
				const	MCHAR*		cGeneralName,	// (I  ) 総称 または NULL
				const	MCHAR*		cNmPts1,	// (I  ) 操作用部材名 または NULL
						MhPlcInfo	*pPlcEn1		// (I  ) 調査部品配置
				)
{
	bool	bSt = false;
	if ( iKai != NULL && pPlcEn1->GetPIKai() != iKai)
		MQUIT;													// 異なる階の部材は対象外
	if ( iIdTpPts != NULL && pPlcEn1->GetPTCdGp() != iIdTpPts)
		MQUIT;													// 異なる構成の部材は対象外
	if ( cGeneralName != NULL &&
		Mstrcmp( pPlcEn1->GetPTNmGeneral(), cGeneralName) != 0)
		MQUIT;													// 異なる総称の部材は対象外
	if ( cNmPts1 != NULL &&
		Mstrcmp( pPlcEn1->GetPTNmPts1(), cNmPts1) != 0)
		MQUIT;													// 異なる操作用部材名の部材は対象外
	bSt = true;
exit:
	return bSt;
}

#define		MC_HITBZI_SZ	20
/////////////////////////////////////////////////////////////////////////////
//  部材配置を検索する
//	返値 検索結果　または　null:ヒットなし
MhPlcInfo* HaitiIn::SrchBuzai(
						MmWndInfo*	pWndInfo,	// (I  ) ウィンドウ管理情報
						MgPoint2&	ptMouthR,	// (I  ) 検索指示座標
						MINT		iCdBuzai,	// (I  ) 部材コード　または　NULL(全)
												//       MP_BZICD_PANEL(全パネル)
						MgPolyg2*	ppgPtsShape	// (  O) 検出多角形　または　NULL
				)
{
	MINT		ist1;
	MINT		ic1;
	MPOSITION	pos1;
	MhPlcInfo	*pPlcEn;
//	MsBitSet	*pOptv;
//	MsBitSet*	pHstv;														// 履歴管理対応
	MhPlcInfo	*pHitBziEn[MC_HITBZI_SZ];
	MREAL		rAHitBziEn[MC_HITBZI_SZ];
	MgPolyg2	pgHitBzi;
	MREAL		rArea;

	MINT		nHitBziEn = 0;
	MINT		iHitEnMin = 0;
   
	MINT		iKaiC   = InpMode::GetKai();						// 階  	(1,2,3)
	MINT		iGpC = InpMode::GetKGp();							// 構成

	MREAL		rAMinHitBziEn = MREALMAX;

	MINT		iBuzaiType = iCdBuzai / MP_BZICD_TYPE;
	if ( iCdBuzai - iBuzaiType * MP_BZICD_TYPE) iBuzaiType = -1;

	for ( pPlcEn = HaitiDb::MdGetHeadPts( &pos1); pPlcEn!=0;
		  pPlcEn = HaitiDb::MdGetNextPts( &pos1)) {

		if ( pPlcEn->GetPIKai() != iKaiC)
			continue;											// 異なる階の部材は検索しない
		if ( pPlcEn->GetPTCdGp() != iGpC)
			continue;											// 異なる構成の部材は検索しない
			
		if ( !MmChkValidParts( pPlcEn))							// オプションと履歴のチェック
			continue;

		if ( iCdBuzai) {											// 部材コードが指定されている場合はチェックする
			if ( (pPlcEn->GetPTCdBuzai() / MP_BZICD_TYPE == iBuzaiType) ||
				(pPlcEn->GetPTCdBuzai() == iCdBuzai))
				;
			else
				continue;										// 異なる部材コードの部材は検索しない
		} 

		HaitiIn::PtsShape( pPlcEn,	&pgHitBzi);							// 部材形状を求める
		// 部材形状のMIN/MAXを求める
		MgMinMaxR2 rMinMax;
		rMinMax.SetInit();
		for ( ic1=0; ic1<pgHitBzi.m_n; ic1++) 
			rMinMax.Ins2( pgHitBzi.m_p[ic1]);
		// 指示部材を検索
		if ( MGeo::ChkPt2InMMR2( ptMouthR, rMinMax)) {				// 部材形状のMIN/MAXで検査し
			if ( MGeo::ChkPt2OnPg2WS( ptMouthR, pgHitBzi, &ist1)) {	// さらに部材形状で検査する
				pHitBziEn[nHitBziEn] = pPlcEn;
				// 複数該当の場合は面積最小を選ぶ
				rArea = pgHitBzi.Area();
				rAHitBziEn[nHitBziEn] = rArea;
				if ( rArea < rAMinHitBziEn) {
					rAMinHitBziEn = rArea;
					if ( ppgPtsShape)
						*ppgPtsShape = pgHitBzi;
					iHitEnMin = nHitBziEn;
				}
				nHitBziEn++;
//				return pPlcEn;									// 指示部材
			}
		}
	}
	if ( nHitBziEn)
		return pHitBziEn[iHitEnMin];
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
//	長さ調整部材に接触する部材と交差する部材を求める
void HaitiIn::MmSrchCrossBuzai(
						MmWndInfo	*pWndInfo,		// (I  ) ウィンドウ管理情報
						MhPlcInfo	*pBziInfo1,		// (I  ) 長さ調整部材
						MINT		iCdBuzai,		// (I  ) 検索対象の部材コード
						MhPlcInfo	**pbTchBzi,		// (  O) 長さ調整後の部材に接触する部材
						MhPlcInfo	**pbCrsBzi		// (  O) 長さ調整後の部材と交差する部材
				)
{
	MINT		ist1;
	MINT		ic1;
	MINT		iTch = 0;
	MINT		iCrs = 0;
	MgLine2		lnBziSin1;
	MgPolyg2	pgBzi1;
	MPOSITION	pos1;
	MhPlcInfo	*pPlcEn;
	MgLine2		lnBziSin2;
	MgPolyg2	pgBzi2;
	MINT		nT;
	MINT		iIntr;
	MgPoint2	po;

	MINT		nBzi2En = 0;
	MINT		iHitEnMin = 0;
   
	MINT		iKaiC   = InpMode::GetKai();						// 階  	(1,2,3)
	MINT		iGpC = InpMode::GetKGp();							// 構成

	MREAL		rAMinBzi2En = MREALMAX;

	MINT		iBuzaiType = iCdBuzai / MP_BZICD_TYPE;
	if ( iCdBuzai - iBuzaiType * MP_BZICD_TYPE) iBuzaiType = -1;

	HaitiIn::PtsShape( pBziInfo1, &pgBzi1);							// 長さ調整部材の部材形状を求める
	// 長さ調整部材の部材形状のMIN/MAXを求める
	MgMinMaxR2 rMinMax1;
	rMinMax1.SetInit();											// 部材形状のMIN/MAXを求める
	for ( ic1=0; ic1<pgBzi1.m_n; ic1++) 
		rMinMax1.Ins2( pgBzi1.m_p[ic1]);
	BuzaiCode::MhBziSin( pBziInfo1, &lnBziSin1);							// 芯線を求める

	for ( pPlcEn = HaitiDb::MdGetHeadPts( &pos1); pPlcEn!=0;
		  pPlcEn = HaitiDb::MdGetNextPts( &pos1)) {

		if ( pPlcEn == pBziInfo1)
			continue;											// 長さ調整部材は無視

		if ( pPlcEn->GetPIKai() != iKaiC)
			continue;											// 異なる階の部材は検索しない
		if ( pPlcEn->GetPTCdGp() != iGpC)
			continue;											// 異なる構成の部材は検索しない

		if ( !MmChkValidParts( pPlcEn))							// オプションと履歴のチェック
			continue;

		if ( iCdBuzai) {											// 検索対象の部材コードが指定されている場合はチェックする
			if ( (pPlcEn->GetPTCdBuzai() / MP_BZICD_TYPE == iBuzaiType) ||
				(pPlcEn->GetPTCdBuzai() == iCdBuzai))
				;
			else
				continue;										// 異なる部材コードの部材は検索しない
		} 

		HaitiIn::PtsShape( pPlcEn,	&pgBzi2);							// 検索部材の部材形状を求める
		// 検索部材の部材形状のMIN/MAXを求める
		MgMinMaxR2 rMinMax2;
		rMinMax2.SetInit();										// 部材形状のMIN/MAXを求める
		for ( ic1=0; ic1<pgBzi2.m_n; ic1++)
			rMinMax2.Ins2( pgBzi2.m_p[ic1]);
		BuzaiCode::MhBziSin( pPlcEn, &lnBziSin2);							// 芯線を求める

		// 交差または端部接続部材を検索
		if ( MGeo::ChkMMR2OnMMR2( rMinMax1, rMinMax2)) {				// 部材形状のMIN/MAXで重なりありかを検査する
			iIntr = MGeo::Intr2Ln2( lnBziSin2, lnBziSin1, &po);
			if ( iIntr != MC_PARALLEL) {							// 交差または接触
				nT = 0;
				for ( ic1=0; ic1<pgBzi2.m_n; ic1++) {			// 検索部部位の構成点が長さ調整部材内にある場合は接触とみなす
					if ( MGeo::ChkPt2OnPg2WS( pgBzi2.m_p[ic1], pgBzi1, &ist1))
						nT++;
				}
				if ( nT >= 1) {
//					MgVect2 vt0 = lnBziSin2.p[1] - lnBziSin2.p[0];
//					MgVect2 vt1 = lnBziSin1.p[0] - lnBziSin2.p[0];
//					MgVect2 vt2 = lnBziSin1.p[1] - lnBziSin2.p[0];
//					if ( MgLE( (vt1 ^ vt0) * (vt2 ^ vt0), 0)) 
						pbTchBzi[iTch++] = pPlcEn;
				} else if ( MF_CHECK_OR( iIntr, (MC_INT | MC_CONNECTION))) {
					pbCrsBzi[iCrs++] = pPlcEn;
				}
			}
		}
	}
	pbTchBzi[iTch] = NULL;
	pbCrsBzi[iCrs] = NULL;
}

/////////////////////////////////////////////////////////////////////////////
//  部材の形状を求める
void HaitiIn::PtsShape(
						MhPlcInfo	*pPlcEn,	// 部材配置レコード
						MgPolyg2*	pgPtsShape	// 部材形状
				)
{
	MINT		iKaiC   = InpMode::GetKai();						// 階  	(1,2,3)
	MINT		iGpC = InpMode::GetKGp();							// 構成

	MgLine2		LnPts;
	MgPoint2	ptPtsN;
	MINT		iKeijoF;
	MREAL		rWidthR, rWidth;
	MgVect2		VtWidthR, VtWidth;
	MgPoint2	ptW[2];
	MgVect2		VtW, VtUtW;
	MgPoint2	ptH;
	MgPoint2	ptK[4];
	MgVect2		vptH;
	MgVect2		vOffset = MgVect2( 0., 20.);
	
	MINT		ic1;

	ptW[0] = *(MgPoint2*)&(pPlcEn->GetPIPlcIti( 0));
	ptW[1] = *(MgPoint2*)&(pPlcEn->GetPIPlcIti( 1));

	// 部材の形を求め検索する
	VtW = ptW[1] - ptW[0];										// 芯線
	VtUtW = MGeo::UnitizeV2( VtW);									// 部材の形を求める
	if ( pPlcEn->IsFrame()) {									// たて枠用の形状作成
		VtWidthR = (pPlcEn->GetMbTWidthR() + pPlcEn->GetPISinZure()) * VtUtW.RotR90(); 
		VtWidth = pPlcEn->GetMbTWidth() * VtUtW.RotL90(); 
		ptH = ptW[0] + pPlcEn->GetPIPlcZure() * VtUtW;
		ptK[0] = ptH + pPlcEn->GetMbTHeightF() * VtUtW + VtWidthR;
		ptK[1] = ptH - pPlcEn->GetMbTHeightB() * VtUtW + VtWidthR;
		ptK[2] = ptK[1] + VtWidth;
		ptK[3] = ptK[0] + VtWidth;
		iKeijoF = 0;											// 形状算出済
	} else if ( pPlcEn->IsPanel() || pPlcEn->IsKaiko()) {		// パネル形状 | 開口形状
//		if ( pPlcEn->IsYukaPanel() || pPlcEn->IsTenjoPanel()) {
		if ( pPlcEn->IsYuka() || pPlcEn->IsTenjo()) {
			rWidthR = pPlcEn->GetPIMaeHosei();					// 手前側補正
			rWidth = pPlcEn->GetPIOkuHosei() + pPlcEn->GetPIOku() +
							   pPlcEn->GetPIMaeHosei();			// 手前側補正 + 奥行き + 奥行き補正
			iKeijoF = 2;
//		} else if ( pPlcEn->IsYanePanel()) {
		} else if ( pPlcEn->IsYane()) {
			rWidthR = pPlcEn->GetPIMaeHosei();					// 手前側補正
			rWidth = pPlcEn->GetPIOkuHosei() + pPlcEn->GetPIOku() +
							   pPlcEn->GetPIMaeHosei();			// 手前側補正 + 奥行き + 奥行き補正
			iKeijoF = 2;
		} else {
			rWidthR = pPlcEn->GetMbTWidthR() + pPlcEn->GetPISinZure(); 
			rWidth = pPlcEn->GetMbTWidth(); 
			iKeijoF = 3;
		}
	} else {													// 横置部材の形状作成
		rWidthR = pPlcEn->GetMbWidthR() + pPlcEn->GetPISinZure(); 
		rWidth = pPlcEn->GetMbWidth(); 
		iKeijoF = 1;
	}

	if ( iKeijoF != 0) {											// 形状算出
		VtWidthR = rWidthR * VtUtW.RotR90(); 
		VtWidth = rWidth * VtUtW.RotL90(); 
		ptK[0] = ptW[0] - pPlcEn->GetPILenHosei( 0) * VtUtW + VtWidthR;
		ptK[1] = ptW[1] + pPlcEn->GetPILenHosei( 1) * VtUtW + VtWidthR;
		ptK[2] = ptK[1] + VtWidth;
		ptK[3] = ptK[0] + VtWidth;
	}

//L		pCod->SetLineAttr( MPS_SOLID, 1, PtsLineColor);
//		pZukei = pPlcEn->m_pZukei;
//		if ( pZukei) {											// 図形データ有りの場合は、それより検索
//			for ( ic=0; ic<pZukei->m_nZukei; ic++) {
//				pCod->Line( MgLine2C( pZukei->m_lnZukei[ic]));
//			}
//		} else {												// 図形データなしの場合は求めた形状より検索
	pgPtsShape->m_n = 0;

	for ( ic1=0; ic1<4; ic1++) 
		(*pgPtsShape) += ptK[ic1];
//			pCod->Polygon( ptK, 4);
//		}

//		if ( pPlcEn->GetPITpPts()->m_iBr == 2)
//			DrawPart( pCod, iGpC, pPlcEn);
}

/////////////////////////////////////////////////////////////////////////////
//	部材の配置点と長さ補正値を修正
void HaitiIn::MhModBzPH(
						MINT		iInpKbCd,		// (I  ) 入力点区分コード
						MINT		iMov,			// (I  ) 修正側　(0:始点、1:終点)
				const	MgPoint3	&PtInt,			// (I  ) 配置点
						MREAL		rLH,			// (I  ) 長さ補整値
						MhPlcInfo	*pPlcEnR		// (I O) 長さ調整部材
				)
{
	MhPlcInfo	*pPlcEnM;							// 長さ調整部材2

	if ( ( pPlcEnR->GetPIPlcIti( iMov) != PtInt) ||
		( !MGeo::Equal(pPlcEnR->GetPILenHosei( iMov), rLH)) ||
		 pPlcEnR->GetPIInpKbCd() == iInpKbCd) {

		HaitiDb::MdPtsModify( pPlcEnR, &pPlcEnM);
		iInpKbCd = pPlcEnM->GetPIInpKbCd();
		if ( iInpKbCd == MP_INPKB_1PT) {
			if ( iMov == 0) {
				pPlcEnM->SetPIPlcIti( 0, pPlcEnM->GetPIPlcIti(1));
			} else {
				pPlcEnM->SetPIPlcIti( iMov, PtInt);
				pPlcEnM->SetPILenHosei( iMov, rLH);
			}

		} else {
			pPlcEnM->SetPIPlcIti( iMov, PtInt);
			pPlcEnM->SetPILenHosei( iMov, rLH);
		}

	}
}

/////////////////////////////////////////////////////////////////////////////
//	調整先が部材で示された部材の長さ調整
MINT HaitiIn::MhAdjBzL(										// (  O) ステイタス　
													//	MC_PARALLEL (-1) 交差なし（平行）
													//	MC_TWIST    (-2) 交差なし（ねじれ）
													//	MC_NINT	    (0)	交差なし
													//	MC_INT      (1)	交差あり長さ調整
						MINT		iKati,			// (I  ) 勝ち負けフラグ(1:勝ち、0:負け)
						MhPlcInfo *pPlcEn1,			// (I O) 長さ調整部材1
				const	MgPoint3	&Pt1,			// (I  ) 長さ調整指示点（部材1の残す側を示す最寄の点）
						MhPlcInfo *pPlcEn2			// (I  ) 長さ調整先を示す部材2
				)
{
	MINT		ist;
	MINT		ist1, ist2;
	
	MhSenBuzai	Bz1, Bz2;
	MgPoint3	PtInt;
	MREAL		s0, s1, s2;							// 長さ調整指示点と部材1の部材2に対する左右位置
	MINT		iMov;								// 部材1のカット端 0:配置点側 1:配置方向店側
	MgPoint3	PtC;
	MgULine3	ULnBz1;								// 
	MgULine3	ULnBz2;
	MgPoint3	PtI[2];
	MREAL		rLH;
	MREAL		rLnWH;

//	if ( pPlcEn1 == pPlcEn2)									// 長さ調整部材と調整先の部材が同じ場合は
//		return;													// 長さ調整対象外

	MgVect3	VtUtZ( 0., 0., 1.);

	Bz1 = *pPlcEn1;
	Bz2 = *pPlcEn2;
	
	ist = MhSenBuzai::MhOn(  Bz1, Bz2, &PtInt, &ist1, &ist2);
	if ( ist != MC_INT && ist != MC_NINT) goto exit;			// 交差なしのため無処理
	
	iMov = ist2 - 1;
	
	s0 = (Bz2.vtUt ^ (Pt1 - Bz2.LnWH.p[0])) * VtUtZ;			// s0>0: 長さ調整指示点は長さ調整先の部材の左側
	s1 = (Bz2.vtUt ^ (Bz1.LnWH.p[0] - Bz2.LnWH.p[0])) * VtUtZ;	// s1>0: 長さ調整部材の配置点は長さ調整先の部材の左側
	s2 = (Bz2.vtUt ^ (Bz1.LnWH.p[1] - Bz2.LnWH.p[0])) * VtUtZ;	// s2>0: 長さ調整部材の配置方向点は長さ調整先の部材の左側

	if ( ist1 == MC_CLOSS) {
		if (s0 * s1 > 0) {
			iMov = 1;											// 配置方向点側の長さを補正
		} else {
			iMov = 0;											// 配置点側の長さを補正
		}
	}

	// 部材1の残す方の位置が部材２の左右どちらにあるか判定し、カット直線（調整先の勝ちまたは負け側の直線）を求める
	bool bLeft = (iMov == 1 && MGLEFT( s1) ||
				  iMov == 0 && MGLEFT( s2));					// true: 部材2の左側にあり
	if ( iKati == 1)
		bLeft = ! bLeft;										// 勝ちの場合は、左右逆にする
	if ( bLeft) {
		ULnBz2 = Bz2.ULnW[1];
	} else {
		ULnBz2 = Bz2.ULnW[0];
	}

	ULnBz1 = Bz1.ULnW[0];										// 部材1左側直線
	ist = MGeo::Intr2ULn3( ULnBz1, ULnBz2, &PtI[0]);			// カット直線との交点
	if ( ist != MC_INT)
		goto exit;

	ULnBz1 = Bz1.ULnW[1];										// 部材1右側直線
	ist = MGeo::Intr2ULn3( ULnBz1, ULnBz2, &PtI[1]);			// カット直線との交点
	if ( ist != MC_INT)
		goto exit;

	if ( iMov == 0) {											// 最大の長さ補正値を求める
		rLH = MGMAX( (PtI[0] - PtInt) * (- Bz1.vtUt), (PtI[1] - PtInt) * (- Bz1.vtUt));
	} else {
		rLH = MGMAX( (PtI[0] - PtInt) * (Bz1.vtUt), (PtI[1] - PtInt) * (Bz1.vtUt));
	}

	if ( MGABS( rLH) > MMAX_HOSEI_DBG)
		rLH = MMAX_HOSEI_DBG;									// デバッグ用　エラーチェックを可能にするため999をセット

	if ( Bz1.Ln.p[1 - iMov] == PtInt) {
		rLnWH = Bz1.rLH[1 - iMov] + rLH;
		if ( !MGeo::Zero( rLnWH))
			MhModBzPH( MP_INPKB_1PT, iMov, Bz1.vtUt, rLH, pPlcEn1);	// 長さ調整
	} else {
		MhModBzPH( MP_INPKB_LEN2PT, iMov, PtInt, rLH, pPlcEn1);		// 長さ調整
	}
exit:
	return ist;
}

/////////////////////////////////////////////////////////////////////////////
//	調整先が平行部材で示された部材の長さ調整
void HaitiIn::MhAdjBzL(										// (  O) ステイタス　
													//	MC_PARALLEL (-1) 交差なし（平行）
													//	MC_TWIST    (-2) 交差なし（ねじれ）
													//	MC_NINT	    (0)	交差なし
													//	MC_INT      (1)	交差あり
						MhPlcInfo *pPlcEn1,			// (I O) 長さ調整部材1
				const	MgPoint3	&Pt1,			// (I  ) 部材1の長さ調整する側を示す最寄の点
						MhPlcInfo *pPlcEn2,			// (I  ) 長さ調整先を示す部材2
				const	MgPoint3	&Pt2			// (I  ) 部材2の長さ調整先を示す最寄の点
				)
{
	MhSenBuzai	Bz1, Bz2;
	MINT		iRef;

	Bz1 = *pPlcEn1;
	Bz2 = *pPlcEn2;

	if ( MGeo::Dist2Pt3( Bz2.LnWH.p[0], Pt2) < MGeo::Dist2Pt3( Bz2.LnWH.p[1], Pt2)) {
		iRef = 0;												// 調整先を部材2の配置点側とする
	} else {
		iRef = 1;												// 調整先を部材2の配置方向点側とする
	}


	MREAL		rLH;
	MREAL		rLnWH;
	MINT		iMov;

	MgPoint3	PtIntWH;
	MgPoint3	PtInt;
	MREAL		s1, s2;

	// 部材1の残す方の位置が部材２指定端部の前後ろどちらにあるか判定し、カット直線を求める
	MGeo::PerpLnPt3( Bz1.Ln, Bz2.LnWH.p[iRef], &PtIntWH);
	
	s1 = (Bz1.LnWH.p[0] - PtIntWH) * Bz1.vtUt;
	s2 = (Bz1.LnWH.p[1] - PtIntWH) * Bz1.vtUt;

	if ( s1 > 0 && s2 > 0) {									// 長さ調整部材は長さ調整先の部材の前側にあり
		if ( s1 < s2) {
			iMov = 0;											// 配置点側の長さを補正
		} else {
			iMov = 1;											// 配置方向点側の長さを補正
		}
	} else if(s1 < 0 && s2 < 0) {								// 長さ調整部材は長さ調整先の部材の後側にあり
		if ( s1 > s2) {
			iMov = 0;											// 配置点側の長さを補正
		} else {
			iMov = 1;											// 配置方向点側の長さを補正
		}
	} else {													// 長さ調整部材は長さ調整先の部材の前後にまたがってあり
		if ( MGeo::Dist2Pt3( Bz1.LnWH.p[0], Pt1) < MGeo::Dist2Pt3( Bz1.LnWH.p[1], Pt1)) {
			iMov = 0;											// 配置点側の長さを補正
		} else {
			iMov = 1;											// 配置方向点側の長さを補正
		}
	}

	MGeo::PerpLnPt3( Bz1.Ln, Bz2.Ln.p[iRef], &PtInt);
	rLH = Bz2.rLH[iRef];
	if (Bz1.vtUt * Bz2.vtUt > 0) {
		if ( iMov != iRef)
			rLH = -rLH;
	} else {
		if ( iMov == iRef)
			rLH = -rLH;
	}

	if (Bz1.Ln.p[1 - iMov] == PtInt) {
		rLnWH = Bz1.rLH[1 - iMov] + rLH;
		if ( !MGeo::Zero( rLnWH))
			MhModBzPH( MP_INPKB_1PT, iMov, Bz1.vtUt, rLH, pPlcEn1);	// 長さ調整
	} else {
		MhModBzPH( MP_INPKB_LEN2PT, iMov, PtInt, rLH, pPlcEn1);		// 長さ調整
	}
}

/////////////////////////////////////////////////////////////////////////////
//	調整先が点座標で示された部材の長さ調整を行う
void HaitiIn::MhAdjBzL(
						MhPlcInfo *pPlcEn1,			// 長さ調整部材1
				const	MgPoint3	&Pt1,			// 部材1の長さ調整する側を示す最寄の点1
				const	MgPoint3	&Pt2			// 長さ調整先を示す点2
				)
{
	MINT		ist;
	MhSenBuzai	Bz1;
	MREAL		rLH;
	MREAL		rLnWH;
	MINT		iMov;
	MgPoint3	PtInt;
	MREAL		s1, s2;


	Bz1 = *pPlcEn1;

	MGeo::PerpLnPt3( Bz1.Ln, Pt2, &PtInt);
	
	s1 = (Bz1.Ln.p[0] - PtInt) * Bz1.vtUt;
	s2 = (Bz1.Ln.p[1] - PtInt) * Bz1.vtUt;

	if ( s1 > 0 && s2 > 0) {									// 始点終点共に配置方向側なので
		iMov = 0;												// 配置点の方が近く配置点側の長さ補正を修正
	} else if(s1 < 0 && s2 < 0) {								// 始点終点共に配置点側なので
		iMov = 1;												// 配置方向点の方が近く配置方向点側の長さ補正を修正
	} else {
		if ( MGeo::Dist2Pt3( Bz1.Ln.p[0], Pt1) < MGeo::Dist2Pt3( Bz1.Ln.p[1], Pt1)) {
			iMov = 0;											// 配置点側の長さ補正
		} else {
			iMov = 1;											// 配置方向点側の長さ補正
		}
	}

	ist = InpAttr::GetComboAttrR( MC_CMB_LHS1, &rLH);				// 長さ補正値を取得する
	if ( Bz1.Ln.p[1 - iMov] == PtInt) {
		rLnWH = Bz1.rLH[1 - iMov] + rLH;
		if ( !MGeo::Zero( rLnWH))
			MhModBzPH( MP_INPKB_1PT, iMov, Bz1.vtUt, rLH, pPlcEn1);	// 長さ調整
	} else {
		MhModBzPH( MP_INPKB_LEN2PT, iMov, PtInt, rLH, pPlcEn1);		// 長さ調整
	}
}

/////////////////////////////////////////////////////////////////////////////
//	長さ調整前後で接触状態と交差状態が変化する部材を求める
void HaitiIn::MhChngCrossBuzai(
						MhPlcInfo	**pbTchBziI,	// (I  ) 長さ調整前に長さ調整部材に接触する部材
						MhPlcInfo	**pbCrsBziI,	// (I  ) 長さ調整前に長さ調整部材と交差する部材
						MhPlcInfo	**pbTchBziO,	// (I  ) 長さ調整後に長さ調整部材に接触する部材
						MhPlcInfo	**pbCrsBziO,	// (I  ) 長さ調整後に長さ調整部材と交差する部材
						MhPlcInfo	**pbFRtoTCBzi,	// (  O) 長さ調整前に長さ調整部材と離れていたが
													//		 長さ調整後に接触または交差した部材
						MhPlcInfo	**pbTCtoFRBzi,	// (  O) 長さ調整前に長さ調整部材と接触または交差していたが
													//		 長さ調整後に離れた部材
						MhPlcInfo	**pbTCtoTCBzi	// (  O) 長さ調整前に長さ調整部材に接触または交差していたが
													//		 、長さ調整後にも接触または交差している部材
				)
{
	MINT	ic0, ic1, ic2;
	MhPlcInfo* TCBziI[MSZ_CROSS_BZI];
	MhPlcInfo* TCBziO[MSZ_CROSS_BZI];
	MINT	if0;
//
	ic0 = 0;
	for ( ic1=0; pbTchBziI[ic1]!=NULL; ic1++)
		TCBziI[ic0++] = pbTchBziI[ic1];
	for ( ic1=0; pbCrsBziI[ic1]!=NULL; ic1++)
		TCBziI[ic0++] = pbCrsBziI[ic1];
	TCBziI[ic0] = NULL;
//
	ic0 = 0;
	for ( ic1=0; pbTchBziO[ic1]!=NULL; ic1++)
		TCBziO[ic0++] = pbTchBziO[ic1];
	for ( ic1=0; pbCrsBziO[ic1]!=NULL; ic1++)
		TCBziO[ic0++] = pbCrsBziO[ic1];
	TCBziO[ic0] = NULL;
//
	ic0 = 0;
	for ( ic1=0; TCBziO[ic1]!=NULL; ic1++) {
		if0 = 0;
		for ( ic2=0; TCBziI[ic2]!=NULL; ic2++) {
			if (TCBziI[ic2] == TCBziO[ic1]) {
				if0 = 1;
				break;
			}
		}
		if ( if0 == 1)
			continue;
		pbFRtoTCBzi[ic0++] = TCBziO[ic1];
	}
	pbFRtoTCBzi[ic0] = NULL;
//
	ic0 = 0;
	for ( ic1=0; TCBziI[ic1]!=NULL; ic1++) {
		if0 = 0;
		for ( ic2=0; TCBziO[ic2]!=NULL; ic2++) {
			if (TCBziO[ic2] == TCBziI[ic1]) {
				if0 = 1;
				break;
			}
		}
		if ( if0 == 1)
			continue;
		pbTCtoFRBzi[ic0++] = TCBziI[ic1];
	}
	pbTCtoFRBzi[ic0] = NULL;
//
	ic0 = 0;
	for ( ic1=0; TCBziO[ic1]!=NULL; ic1++) {
		for ( ic2=0; TCBziI[ic2]!=NULL; ic2++) {
			if (TCBziI[ic2] == TCBziO[ic1]) {
				pbTCtoTCBzi[ic0++] = TCBziO[ic1];
				break;
			}
		}
		if (TCBziO[ic1] == TCBziI[ic2])
			continue;
	}
	pbTCtoTCBzi[ic0] = NULL;
}

} // namespace MC
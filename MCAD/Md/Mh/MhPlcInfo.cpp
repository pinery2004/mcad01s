//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MhPlcInfo.cpp
//
//		部材配置読み書き
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"
#include "MgLib.h"

#include "MsBitSet.h"
#include "MmOpt.h"
#include "MmHist.h"

#include "MhLib.h"
#include "MhPlcInfo.h"

#include "MbFile.h"
#include "MmTenkai.h"

#include "MhDefine.h"

//void MdPtsDbInit();
//void MdRedoPtsDbInit();
//void MdRoofDbInit();

namespace MC
{

////////////////////////////////////////////////////////////////////////////
//	配置部材の読み込み
//	[返値] ステイタス     0 : 正常  -1 : 異常

MINT MhPlcInfo::Load(							//
						MhMdlIO*	i_phMdl		// 読み込みファイル　ハンドル
				)
{
	MINT		ist = 0;
	MINT		iType;
	MINT		iSize;
	MINT		nVer;
	
	void*		pEnt;
	MhPlcInfo*	pPlcEn = 0;
	MhTateguInfo*	pAuxTategu;
	JTTenkaiPr*	pAuxTenkai;
	MhHistV*	pVer;
	MsBitSet*	pOpt;
	MhZukei*	pZukei;
	MINT		szZukei;
	MCHAR*		pcMbr;
	MINT		iMbr;
	MINT		iCdBuzai;

	bool		fPlcSzChk = true;
	bool		fRoofSzChk = true;

	MgVect3		vLnPlc;
	
	bool		bEor = false;

	// 配置レコード開始

	//pPlcEn = (MhPlcInfo*)malloc( sizeof( MhPlcInfo));
	//
	//m_pVer = 0;
	//m_pOpt1 = 0;
	//m_pOpt2 = 0;
	//m_pAuxTategu = 0;
	//m_pZukei = 0;
	//m_iDisplayFlg = 0;
	//m_ptCmnt1 = MgPoint2( 0., 0.);

	memset( this, 0, sizeof( MhPlcInfo));

	do {
		iSize = i_phMdl->Read( &iType, (void**)&pEnt);

		if ( iSize <= 0)
			break;

		switch( iType) {
			case MDR_ATRCD_HAITIEN_EOR:											// 配置 レコード終了r
				ASSERT( iSize == sizeof(MINT));
				MBFREE( pEnt);													// TYPE_HAITIEN_EORレコード用エリアの削除
				
				m_pTpPts	= BuzaiCode::MhGetpTpPts( m_iIdTpPts);
				m_pMbr		= BuzaiCode::MhGetpMbr( m_iMbrId);
//				m_pZukei	= NULL;
				
				bEor = true;
				break;
			case MDR_ATRCD_HAITIEN_IKAI:										// 階
				ASSERT( iSize == sizeof(MINT));
				m_iKai = *(MINT*)pEnt;
				MBFREE( pEnt);
				break;
			case MDR_ATRCD_HAITIEN_ICDHGT:										// 配置点
				ASSERT( iSize == sizeof(MINT));
				m_iCdHgt = *(MINT*)pEnt;
				MBFREE( pEnt);
				break;
			case MDR_ATRCD_HAITIEN_IPANELNO:									// 配置点
				ASSERT( iSize == sizeof(MINT));
				m_iPanelNo = *(MINT*)pEnt;
				MBFREE( pEnt);
				break;
			case MDR_ATRCD_HAITIEN_IKAIKONO:									// 開口番号
				ASSERT( iSize == sizeof(MINT));
				m_iKaikoNo = *(MINT*)pEnt;
				MBFREE( pEnt);
				break;
//			case MDR_ATRCD_HAITIEN_IPARTSTPID:
			case MDR_ATRCD_HAITIEN_IPARTSTPCD:									// 部品配置タイプコード 
				ASSERT( iSize == sizeof(MINT));
				iCdBuzai = *(MINT*)pEnt;										// (部材コード)
				m_iIdTpPts = BuzaiCode::MhGetPIIdTpPts( iCdBuzai);
				MBFREE( pEnt);
				break;
//			case MDR_ATRCD_HAITIEN_IMEMBERID:									// メンバー
//				ASSERT( iSize == sizeof(MINT));
//				m_iMbrId = *(MINT*)pEnt;
//				MBFREE( pEnt);
//				break;
			case MDR_ATRCD_HAITIEN_MEMBER:										// メンバー
				ASSERT( iSize == sizeof( MhTateguInfo));
				pcMbr = (MCHAR*)pEnt;
				iMbr = BuzaiCode::MhGetIdMbr( pcMbr);
				if ( iMbr < 0) {
					iMbr = 0;
				}
				m_iMbrId	= iMbr;
				m_pMbr		= BuzaiCode::MhGetpMbr( m_iMbrId);
				MBFREE( pcMbr);
				break;
			case MDR_ATRCD_HAITIEN_LNHAITI:										// 配置点、配置方向点
				ASSERT( iSize == sizeof(MgLine3));
				m_lnPlc = *(MgLine3*)pEnt;
				vLnPlc = m_lnPlc.p[1] - m_lnPlc.p[0];
				vLnPlc.SetUnitize();
				m_vUpPlc = vLnPlc ^ MgVect3( 0., 0., 1.);
				MBFREE( pEnt);
				break;
			case MDR_ATRCD_HAITIEN_PTUPHAITI:									// 上方向
				ASSERT( iSize == sizeof(MgPoint3));
				m_vUpPlc = *(MgPoint3*)pEnt;
				MBFREE( pEnt);
				break;
			case MDR_ATRCD_HAITIEN_RSINZURE:									// 材軸芯ずれ量
				ASSERT( iSize == sizeof(MREAL));
				m_rSinZure = *(MREAL*)pEnt;
				MBFREE( pEnt);
				break;
			case MDR_ATRCD_HAITIEN_RHAITIZURE:									// 配置点ずれ量 
				ASSERT( iSize == sizeof(MREAL));
				m_rPlcZure = *(MREAL*)pEnt;
				MBFREE( pEnt);
				break;
			case MDR_ATRCD_HAITIEN_RNAGASAHOSEI:								// 長さ補正値1、長さ補正値2
				ASSERT( iSize == sizeof(MREAL) * 2);
				m_rLngHosei[0] = *(MREAL*)pEnt;
				m_rLngHosei[1] = *(((MREAL*)pEnt)+1);
				MBFREE( pEnt);
				break;
			case MDR_ATRCD_HAITIEN_RTAKASA:										// 高さ
				ASSERT( iSize == sizeof(MREAL) * 2);
				m_rHgt[0] = *(MREAL*)pEnt;
				m_rHgt[1] = *(((MREAL*)pEnt)+1);
				MBFREE( pEnt);
				break;
			case MDR_ATRCD_HAITIEN_TATEGU:										// 建具属性
				ASSERT( iSize == sizeof( MhTateguInfo));
				pAuxTategu = (MhTateguInfo*)pEnt;
				m_pAuxTategu = pAuxTategu;
				break;
			case MDR_ATRCD_HAITIEN_TENKAI:										// 住棟展開
				ASSERT( iSize == sizeof(JTTenkaiPr));
				pAuxTenkai = (JTTenkaiPr*)pEnt;
				m_pAuxTenkai = pAuxTenkai;
				JTTenkai::SetJTTPara( pAuxTenkai);
				break;
			case MDR_ATRCD_HAITIEN_VERSION:										// バージョン
				ASSERT( iSize == sizeof(MhHistV));
				pVer = (MhHistV*)pEnt;
				m_pVer = pVer;
				break;
			case MDR_ATRCD_HAITIEN_OPTION1:										// ＯＰＴ群
				ASSERT( iSize == sizeof(MsBitSet));
				pOpt = (MsBitSet*)pEnt;
				m_pOpt1 = pOpt;
				break;
			case MDR_ATRCD_HAITIEN_OPTION2:										// ＯＰＴ群
				ASSERT( iSize == sizeof(MsBitSet));
				pOpt = (MsBitSet*)pEnt;
				m_pOpt2 = pOpt;
				break;
			case MDR_ATRCD_OPTION_KANRI:										// ＯＰＴ群管理
				MhOptV::nbmGOpt = iSize / sizeof( MhOpt);
				memcpy( MhOptV::bmGOpt, pEnt, iSize);
				MBFREE( pEnt);
				break;
			case MDR_ATRCD_VERSION_KANRI0:										// 履歴管理
				nVer = 0;
				memcpy( &MhHistV::nVer, pEnt, iSize);
				MBFREE( pEnt);
				break;
			case MDR_ATRCD_VERSION_KANRI1:										// 履歴管理　バージョン
				MhHistV::sVerC[nVer] = (MCHAR*)pEnt;
				MBFREE( pEnt);
				break;
			case MDR_ATRCD_VERSION_KANRI2:										// 履歴管理　直前バージョン
				MhHistV::sVerB[nVer] = (MCHAR*)pEnt;
				MBFREE( pEnt);
				break;
			case MDR_ATRCD_VERSION_KANRI3:										// 履歴管理　日付
				MhHistV::sVerD[nVer++] = (MCHAR*)pEnt;
				MBFREE( pEnt);
				break;
			case MDR_ATRCD_HAITIEN_ZUKEI:
				pZukei = (MhZukei*)pEnt;
				szZukei = sizeof( MhZukei) + sizeof( MgLine3) * (pZukei->m_nZukei - 1);
				ASSERT( iSize == szZukei);
				SetPIZukei( pZukei);
				GetPIZukei()->m_pNext = 0;
				break;
			case MDR_ATRCD_HAITIEN_ZUKEIN:
				pZukei->m_pNext = (MhZukei*)pEnt;								// 直前の図形データにリンクさせる
				pZukei = (MhZukei*)pEnt;
				szZukei = sizeof( MhZukei) + sizeof( MgLine3) * (pZukei->m_nZukei - 1);
				ASSERT( iSize == szZukei);
				pZukei->m_pNext = 0;
				break;
			default:
				MBFREE( pEnt);
		}
	} while ( !bEor);
    return ist;
}

////////////////////////////////////////////////////////////////////////////
//	部材配置レコード書き込み
//	[返値] ステイタス     0 : 正常  -1 : 異常

MINT MhPlcInfo::Save(							//
				MhMdlIO*		i_phMdl			// 書き込みファイル　ハンドル
				)
{
	MINT		iDumy = 0;
//	MhZukei*	pZukei;
//	MINT		szZukei;

	i_phMdl->Write( MDW_ATRCD_HAITIEN, &iDumy);									// 配置レコード
		
//	MINT iKai = GetPIKai();														// 階
	i_phMdl->Write( MDW_ATRCD_HAITIEN_IKAI, &m_iKai);
//	MINT iCdHgt = GetPICdHgt();													// 取り付け高さコード
	i_phMdl->Write( MDW_ATRCD_HAITIEN_ICDHGT, &m_iCdHgt);
//	MINT iDisplayFlag = GetPIDisplayFlg();										// 表示フラグ(図形,注記1,注記2)
	i_phMdl->Write( MDW_ATRCD_HAITIEN_DISPLAYFLAG, &m_iDisplayFlg);
//	MINT iPanelNo = GetPIPanelNo();												// パネル番号
	i_phMdl->Write( MDW_ATRCD_HAITIEN_IPANELNO, &m_iPanelNo);
//	MINT iKaikoNo = GetPIKaikoNo();												// 開口番号
	i_phMdl->Write( MDW_ATRCD_HAITIEN_IKAIKONO, &m_iKaikoNo);
	MINT iCdBuzaiW = GetPTCdBuzaiW();											// 部材コード
	i_phMdl->Write( MDW_ATRCD_HAITIEN_IPARTSTPCD, &iCdBuzaiW);
//	MgLine3 lnPlc = GetPIPlcIti();												// p[0]: 始点 | 配置点
																				// p[1]: 終点 | 配置方向点
	i_phMdl->Write( MDW_ATRCD_HAITIEN_LNHAITI, (MREAL*)&m_lnPlc, sizeof(MgLine3));
//	MgPoint3 ptUpPlc = GetPIUpPlc();											// 配置上方向点
	i_phMdl->Write( MDW_ATRCD_HAITIEN_PTUPHAITI, (MREAL*)&m_vUpPlc, sizeof(MgPoint3));
//	MREAL rSinZure = GetPISinZure();											// 材軸芯ずれ量　	>0:右側、<0:左側
	i_phMdl->Write( MDW_ATRCD_HAITIEN_RSINZURE, &m_rSinZure);
//	MREAL rPlcZure = GetPIPlcZure();											// 配置点ずれ量		>0:前、　<0:後
	i_phMdl->Write( MDW_ATRCD_HAITIEN_RHAITIZURE, &m_rPlcZure);
//	MREAL* prLngHosei = GetPILenHosei();										// 長さ補正値（始点側、終点側）
	i_phMdl->Write( MDW_ATRCD_HAITIEN_RNAGASAHOSEI, m_rLngHosei, 2);
//	MREAL* prHgt = GetPIHgt();													// 高さ（取り付け高さ、高さ）
	i_phMdl->Write( MDW_ATRCD_HAITIEN_RTAKASA, m_rHgt, 2);
//	MgPoint2 ptCmnt1 = GetPICmntPt();											// 注記表示位置
	i_phMdl->Write( MDW_ATRCD_HAITIEN_PTCMNT1, (MREAL*)&m_ptCmnt1, sizeof(MgPoint2));
	MCHAR	cMemBerCode[16];
	Mstrcpy( cMemBerCode, GetMbCdMbrW());										// 寸法型式 (書き込み用　通常m_cCodeと同じ VerUp時に変更した内容が入る)
	i_phMdl->Write( MDW_ATRCD_HAITIEN_MEMBER, cMemBerCode, sizeof(cMemBerCode));
	if ( GetPIAuxAttr() != 0) {
		if ( IsTategu())														// 建具情報
//			i_phMdl->Write( MDW_ATRCD_HAITIEN_TATEGU, GetPIAuxTategu(), sizeof(MhTateguInfo));
			GetPIAuxTategu()->Save( i_phMdl);
		else if ( IsTenkai())														// 住棟展開
//			i_phMdl->Write( MDW_ATRCD_HAITIEN_TENKAI, GetPIAuxTenkai(), sizeof(JTTenkaiPr));
			GetPIAuxTenkai()->Save( i_phMdl);
	}
//	if ( GetPIVer() != 0)														// バージョン
	if ( m_pVer != 0)															// バージョン
		i_phMdl->Write( MDW_ATRCD_HAITIEN_VERSION, (MUINT*)m_pVer, sizeof(MhHistV));
//	if ( GetPIOpt1() != 0)														// ＯＰＴ群
	if ( m_pOpt1 != 0)															// ＯＰＴ群
		i_phMdl->Write( MDW_ATRCD_HAITIEN_OPTION1, (MUINT*)m_pOpt1, sizeof(MsBitSet));
//	if ( GetPIOpt2() != 0)														// ＯＰＴ群
	if ( m_pOpt2 != 0)															// ＯＰＴ群
		i_phMdl->Write( MDW_ATRCD_HAITIEN_OPTION2, (MUINT*)m_pOpt2, sizeof(MsBitSet));

//	if ( (pZukei = GetPIZukei()) != 0) {										// 図形
	if ( m_pZukei != 0) {										// 図形
//S		szZukei = sizeof( MhZukei) + sizeof( MgLine3) * (pZukei->m_nZukei - 1);
//		i_phMdl->Write( MDW_ATRCD_HAITIEN_ZUKEI, pZukei, szZukei);
//		for ( pZukei=pZukei->m_pNext; pZukei != 0; pZukei=pZukei->m_pNext) { 
//			szZukei = sizeof( MhZukei) + sizeof( MgLine3) * (pZukei->m_nZukei - 1);
//			i_phMdl->Write( MDW_ATRCD_HAITIEN_ZUKEIN, pZukei, szZukei);
//		}
		m_pZukei->Save( i_phMdl);
	}
	i_phMdl->Write( MDW_ATRCD_HAITIEN_EOR, &iDumy);								// 配置レコード終了

	return 0;
}

////////////////////////////////////////////////////////////////////////////
//	部材配置レコード 建具情報　書き込み
//	[返値] ステイタス     0 : 正常  -1 : 異常

MINT MhTateguInfo::Save(							//
				MhMdlIO*		i_phMdl			// 書き込みファイル　ハンドル
				)
{
	MINT			iDumy = 0;
	//MhZukei*		pZukei;
	//MINT			 szZukei;

	i_phMdl->Write( MDW_ATRCD_HAITIEN_TATEGU, &iDumy);							// 配置レコード
		
	MCHAR	cKCode[MHTTSZ_CODE+1];
	Mstrcpy( cKCode, GetKCode());												// 開口コード
	i_phMdl->Write( MDW_ATRCD_HAITIEN_CCODE, cKCode, sizeof(cKCode));
	MREAL rROW = GetROW();														// 開口幅
	i_phMdl->Write( MDW_ATRCD_HAITIEN_RROW, &rROW);
	MREAL rROH = GetROH();														// 開口高さ
	i_phMdl->Write( MDW_ATRCD_HAITIEN_RROH, &rROH);
	MREAL rHeight= GetHeight();													// 取り付け高さ(まぐさ下端高さ)
	i_phMdl->Write( MDW_ATRCD_HAITIEN_RHEIGHT, &rHeight);
	MREAL* prMUSz =	GetMUSz();													// 左右まぐさ受長さ
	i_phMdl->Write( MDW_ATRCD_HAITIEN_RMUSZ, prMUSz, 2);

	i_phMdl->Write( MDW_ATRCD_HAITIEN_TATEGU_EOR, &iDumy);						// 配置レコード終了

	return 0;
}

////////////////////////////////////////////////////////////////////////////
//	部材配置レコード 図形情報 書き込み
//	[返値] ステイタス     0 : 正常  -1 : 異常

MINT MhZukei::Save(							//
				MhMdlIO*		i_phMdl			// 書き込みファイル　ハンドル
				)
{
	MINT			iDumy = 0;

	i_phMdl->Write( MDW_ATRCD_HAITIEN_TATEGU, &iDumy);							// 図形レコード
		

	i_phMdl->Write( MDW_ATRCD_HAITIEN_TATEGU_EOR, &iDumy);						// 図形レコード終了

	return 0;
}

////////////////////////////////////////////////////////////////////////////
//	住棟展開レコード 書き込み
//	[返値] ステイタス     0 : 正常  -1 : 異常

MINT JTTenkaiPr::Save(							//
				MhMdlIO*		i_phMdl			// 書き込みファイル　ハンドル
				)
{
	MINT			iDumy = 0;

	i_phMdl->Write( MDW_ATRCD_HAITIEN_TATEGU, &iDumy);							// 図形レコード
		

	i_phMdl->Write( MDW_ATRCD_HAITIEN_TATEGU_EOR, &iDumy);						// 図形レコード終了

	return 0;
}

} // namespace MC
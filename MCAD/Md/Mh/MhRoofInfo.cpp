//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MhRoofInfo.cpp
//
//		屋根読み書き
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"
#include "MgLib.h"

#include "MdLib.h"

#include "MsBitSet.h"
#include "MmOpt.h"
#include "MmHist.h"

#include "MhLib.h"
#include "MhRoof.h"

#include "MbFile.h"
#include "MmTenkai.h"

#include "MhDefine.h"

//#include "Heap.h"
class Heap
{
    private:
	unsigned long getCombinedSizesOfProcessHeaps(void);
	
    public:
	Heap();
	virtual ~Heap();
	
	bool ValidateAllHeaps(void);
	bool ValidateHeap(HANDLE heap);
	
	unsigned long GetCombinedSizesOfProcessHeaps(void);
	
	unsigned long GetHeapSize(HANDLE heap);
};

PZBUF*	____pzbuf;

//void MdPtsDbInit();
//void MdRedoPtsDbInit();
//void MdRoofDbInit();

namespace MC
{

MINT MhRoofInfo::Load(								//
				MhMdlIO*		i_phMdl		// 書き込みファイル　ハンドル
				)
{
	MINT		ist = 0;
	MhMdlIO		hMdl;
	MINT		iType;
	MINT		iSize;
	void*			pEnt;
	MhPlcInfo*		pPlcEn = 0;
//	MhRoofInfo*		pRoofEn;
	MhJim*			pJim1;
	MhRfm*			pRfm;
	MgPoint3*		pPt;
	MINT*			piCd;

	bool			fPlcSzChk = true;
	bool			fRoofSzChk = true;

	bool			bEor = false;

	MgVect3			vLnPlc;

//	屋根レコード
//	ASSERT( iSize == sizeof(MhRoofInfo));
	//if ( i_iSize == sizeof(MhRoofInfo)) {
	//	pRoofEn = *(MhRoofInfo**)io_ppEnt;
	//}else {
	//	if ( fRoofSzChk) {
	//		MessageBox( NULL, Mstr( "屋根データレコードサイズエラー"), Mstr( "レコードサイズチェック"), MB_OK);
	//		fRoofSzChk = false;
	//	}
	//	pRoofEn = (MhRoofInfo*)realloc( *io_ppEnt, sizeof( MhRoofInfo));
	//}
//	pRoofEn = (MhRoofInfo*)malloc( sizeof( MhRoofInfo));
	this->InitAllAtr( 0);										// 子のサイズとレコード数をクリア

	do {
		iSize = i_phMdl->Read( &iType, (void**)&pEnt);
//===
		static	int iCyane = 0;
		TRACE( "%d; iType=%d\n", ++iCyane, iType);

//    HANDLE hHeaps[10000];
//	
//    DWORD numHeaps = GetProcessHeaps( 10000, hHeaps);
//	__debugbreak();												// ブレークポイント
////S	for( unsigned int i=0; i < numHeaps; i++) {
////	    if ( !ValidateHeap(hHeaps[i])) {
////			__debugbreak();
////		}
////	}
////---

		if ( iSize <= 0)
			break;

		switch( iType) {
			case MDR_ATRCD_ROOFEN_EOR:											// 屋根 eor
				ASSERT( iSize == sizeof(MINT));
				MBFREE( pEnt);													// MDR_ATRCD_ROOFEN_EORレコード用エリアの削除

				CreateRfmZukei();
				bEor = true;
				break;
			case MDR_ATRCD_JIM:													// 地廻り
				ASSERT( iSize == sizeof(MhJim));
				pJim1 = (MhJim*)pEnt;

				*(this->GetpGJim1()) += (*pJim1);				// 仮対応　MDR_ATRCD_JIM_EORなしの為	ZZZZZZZZZZZZZ 091018
				MBFREE( pJim1);									// 仮対応

				break;
			case MDR_ATRCD_JIM_EOR:												// 地廻り
				ASSERT( iSize == sizeof(MINT));
				MBFREE( pEnt);													// MDR_ATRCD_JIM_EORレコード用エリアの削除

				*(this->GetpGJim1()) += (*pJim1);
				MBFREE( pJim1);	
				break;
			case MDR_ATRCD_RFM:													// 屋根面
				ASSERT( iSize == sizeof(MhRfm));
				pRfm = (MhRfm*)pEnt;
				break;
			case MDR_ATRCD_RFM_EOR:												// 屋根面 eor
				ASSERT( iSize == sizeof(MINT));
				MBFREE( pEnt);													// MDR_ATRCD_RFM_EORレコード用エリアの削除

				*(this->GetpGRfm()) += (*pRfm);
				pRfm->Free();
				MBFREE( pRfm);
				break;
			case MDR_ATRCD_RFM_PG_PT:												// 屋根面構成点 
				ASSERT( iSize == sizeof(MgPoint3) * pRfm->m_Pg.m_n);
				pPt = (MgPoint3*)pEnt;
				pRfm->m_Pg.m_P = pPt;
				break;
			case MDR_ATRCD_RFM_ICD_I:												// 屋根面コード
				ASSERT( iSize == sizeof(MINT) * pRfm->m_cdPg.m_n);
				piCd = (MINT*)pEnt;
				pRfm->m_cdPg.m_i = piCd;
				break;
			default:
				MBFREE( pEnt);
		}
	} while( !bEor);
    return ist;
}

////////////////////////////////////////////////////////////////////////////
//	屋根配置レコード書き込み
//	[返値] ステイタス     0 : 正常  -1 : 異常

MINT MhRoofInfo::Save(							//
				MhMdlIO*		i_phMdl			// 書き込みファイル　ハンドル
				)
{
	MINT		ic1;
	MINT		iDumy = 0;

//	i_phMdl->Write( MDW_ATRCD_ROOFEN, sizeof(MINT), &iDumy);
	i_phMdl->Write( MDW_ATRCD_ROOFEN, &iDumy, sizeof(MINT));

//	i_phMdl->Write( MDW_ATRCD_ROOFEN, this, sizeof(MhRoofInfo));

	i_phMdl->Write( MDW_ATRCD_ROOFEN_KAI, &m_iKai, sizeof(MINT));

//class MhRoofInfo: public MOBJECT
//{
//
//	MINT				m_iKai;						// 階( 1, 2, 3)
//	MgGrpS<MhJim>		m_GJim1;					// 地廻り線（入力）
//													// （（頂点・地廻り線コード・入力フラグ)n）
//	MgPoint2			m_ptH;						// 方向を示す点（入力）
//	MgGrpS<MgKs1>		m_GKs1;						// 構成線（入力）
//													// （（構成線分・構成コード・構成線情報)n）
//	MgGrpF<MhRfm>		m_GRfm;						// 屋根面
//													// （（(頂点・構成コード)n)n）
//	MgGPolyg3			m_GNUm;						// 軒裏面
//													// （（(頂点)n)n）
//	MgGPolyg3			m_GHm;						// へり面
//													// （（(頂点)n)n）
//	
////	MhRoofInfo*			m_Oya;						// 親屋根
//	MINT				m_itpRf;					// 屋根タイプ
//	MREAL				m_rKb;						// 勾配
//	MREAL				m_rRkH;						// 屋根基準高さ変更量
//	MREAL				m_rRfmA;					// 屋根面厚さ(89,140,182)
//	MREAL				m_rNUmA;					// 軒裏面厚さ(89,140)
//	MREAL				m_rMnZ[2];					// 棟ずれ量１・２
//	MREAL				m_rNkD[2];					// 軒の出
//	MREAL				m_rKrD[2];					// けらばの出
//	MINT				m_itpNU;					// 軒裏タイプ１・２
//	MINT				m_itpNS;					// 軒先タイプ１・２
//	MREAL				m_rHKmA;					// 鼻隠し面厚さ
//	MREAL				m_rNUk;						// 軒裏返し量
//	MREAL				m_rNkH;						// 軒の高さ（台形ドーマ用）
//	MINT				m_icdURS;					// 下屋根コード
////	MINT				m_icdRR;					// 大屋根コード
//	MhZukei*			m_pZukei;					// 図形
//
//	MINT				m_imdDsp;					// 表示モード (0: 屋根面  1:構成線)
//	COLORREF			m_colDispD;					// 表示色（データ指定）
//	COLORREF			m_colDispC;					// 表示色（現在）
////	MnDisp*				m_iDispM;					// 表示情報（面）
////	MnDisp*				m_iDispL;					// 表示情報（線分）
//
//	MINT				m_nRtm;						// 立面数
//	MINT*				m_pifRitm;					// 立面表示(面フラグ)  0:面  1:穴
//	MINT*				m_pnPtRitm;					// 立面表示(頂点数)
//	MgPoint3*			m_pPtRitm;					// 立面表示(頂点)
////	MnDisp*				m_pLnRitm;					// 立面表示情報（線分）
//
////	MhTexture*			m_KsInfo[NKSINFO];			// 構成情報
//
//	MsBitSet*			m_pVer;					// バージョン制御 (0: 全バージョンで表示)
//	MsBitSet*			m_pOpt1;					// 表示オンOPT群セット (NULL: オプション設定無しでも表示)
//	MsBitSet*			m_pOpt2;					// 表示オフOPT群セット (NULL: 無視)
//


	for ( ic1=0; ic1 < GetpGJim1()->m_n; ic1++) {
//		i_phMdl->Write( MDW_ATRCD_JIM, &GetpGJim1()->m_pst[ic1], sizeof(MhJim));
//		i_phMdl->Write( MDW_ATRCD_JIM_EOR, &iDumy, sizeof(MINT));

		GetpGJim1()->m_pst[ic1].Save( i_phMdl);
	}

	for ( ic1=0; ic1 < GetpGRfm()->m_n; ic1++) {
		if ( GetpGRfm()->m_pst[ic1].m_Pg.m_n != 0) {
//			i_phMdl->Write( MDW_ATRCD_RFM, &GetpGRfm()->m_pst[ic1], sizeof(MhRfm));
//			i_phMdl->Write( MDW_ATRCD_RFM_PG_PT, (MREAL*)GetpGRfm()->m_pst[ic1].m_Pg.m_P, sizeof(MgPoint3) * GetpGRfm()->m_pst[ic1].m_Pg.m_n);
//			i_phMdl->Write( MDW_ATRCD_RFM_ICD_I, GetpGRfm()->m_pst[ic1].m_cdPg.m_i, sizeof(MINT) * GetpGRfm()->m_pst[ic1].m_cdPg.m_n);
//			i_phMdl->Write( MDW_ATRCD_RFM_EOR, &iDumy, sizeof(MINT));

			GetpGRfm()->m_pst[ic1].Save( i_phMdl);
		}
	}
	i_phMdl->Write( MDW_ATRCD_ROOFEN_EOR, &iDumy, sizeof(MINT));

    return 0;
}

////////////////////////////////////////////////////////////////////////////
//	屋根配置レコード書き込み
//	[返値] ステイタス     0 : 正常  -1 : 異常

MINT MhJim::Save(							//
				MhMdlIO*		i_phMdl			// 書き込みファイル　ハンドル
				)
{
//	MINT		ic1;
	MINT		iDumy = 0;

	i_phMdl->Write( MDW_ATRCD_JIM, &iDumy, sizeof(MINT));
	i_phMdl->Write( MDW_ATRCD_JIM_P, &iDumy, sizeof(MINT));
	i_phMdl->Write( MDW_ATRCD_JIM_ICD, &iDumy, sizeof(MINT));
	i_phMdl->Write( MDW_ATRCD_JIM_IFINP, &iDumy, sizeof(MINT));
	i_phMdl->Write( MDW_ATRCD_JIM_RKB, &iDumy, sizeof(MINT));
	i_phMdl->Write( MDW_ATRCD_JIM_IRFM, &iDumy, sizeof(MINT));
	i_phMdl->Write( MDW_ATRCD_JIM_LNR, &iDumy, sizeof(MINT));
	i_phMdl->Write( MDW_ATRCD_JIM_ILNRCD, &iDumy, sizeof(MINT));
	i_phMdl->Write( MDW_ATRCD_JIM_EOR, &iDumy, sizeof(MINT));

	return 0;
}

MINT MhRfm::Save(
				MhMdlIO*		i_phMdl			// 書き込みファイル　ハンドル
				)
{
	MINT			iDumy = 0;

	i_phMdl->Write( MDW_ATRCD_HAITIEN_TATEGU, &iDumy);							// 図形レコード
		

	i_phMdl->Write( MDW_ATRCD_HAITIEN_TATEGU_EOR, &iDumy);						// 図形レコード終了

	return 0;
}

} // namespace MC
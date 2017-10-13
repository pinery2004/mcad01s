//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgConnect.cpp
//
//		
//
//
//  K.Matsu           09/20/07    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"
#include "MbDebug.h"

#define	DLL_EXPORT_CONNECT_DO

#include "MgLib.h"
#include "MgConnect.h"	

#define PROG_NAME MBCstr( "MgConnect")
//
//======================( ３次元 )==============================
//		ウィングドエッジデータ構造
//
MgNode::MgNode(
						MINT	iSzNode		// (I  ) ノード数
				)
{
	MINT	iN1;

	m_Node = (MINT *)new char[iSzNode * 12 * sizeof(MINT)];
	
	for ( iN1=0; iN1<iSzNode; iN1++ ) {
		EMPT( iN1)->nlnk = iN1 + 1;
		EMPT( iN1)->plnk = iN1 - 1;
	}
	EMPT( 0)->plnk = iSzNode - 1;
	EMPT( iSzNode - 1)->nlnk = 0;
}

void MgNode::wing(
						MINT	iE1,
						MINT	iE2
				)
{
	MgEdgeNode *pE1 = EDGE( iE1);
	MgEdgeNode *pE2 = EDGE( iE2);
	
	if ( pE1->PVT == pE2->PVT ) {
		if ( pE1->PFACE == pE2->NFACE ) {
			pE1->PCW = iE2;
			pE2->NCCW = iE1;
		} else if ( pE1->NFACE == pE2->PFACE ) {
			pE1->NCCW = iE2;
			pE2->PCW = iE1;
		}
	} else if ( pE1->PVT == pE2->NVT ) {
		if ( pE1->PFACE == pE2->PFACE ) {
			pE1->PCW = iE2;
			pE2->PCCW = iE1;
		} else if ( pE1->NFACE == pE2->NFACE ) {
			pE1->NCCW = iE2;
			pE2->NCW = iE1;
		}
	} else if ( pE1->NVT == pE2->PVT ) {
		if ( pE1->PFACE == pE2->PFACE ) {
			pE1->PCCW = iE2;
			pE2->PCW = iE1;
		} else if ( pE1->NFACE == pE2->NFACE ) {
			pE1->NCW = iE2;
			pE2->NCCW = iE1;
		}
	} else if ( pE1->NVT == pE2->NVT ) {
		if ( pE1->PFACE == pE2->NFACE ) {
			pE1->PCCW = iE2;
			pE2->NCW = iE1;
		} else if ( pE1->NFACE == pE2->PFACE ) {
			pE1->NCW = iE2;
			pE2->PCCW = iE1;
		}
	}
}

//	一つのソリッドSn、一つの面分Fn、一つの頂点Vnよりなる縮退した点をあらわす多面体を作る
MINT MgNode::mvsf(							// (  O) ソリッドノードＩＤ
						MINT	*piFn,		// (  O) 面分ノードＩＤ
						MINT	*piVn		// (  O) 頂点ノードＩＤ
				 )
{
	MINT iSn = mks();						// ソリッド
	*piFn = mkf( iSn);						// 面分
	*piVn = mkv( iSn);						// 頂点

	SOLD( iSn)->NFACE = SOLD( iSn)->PFACE = *piFn;
	FACE( *piFn)->NFACE = FACE( *piFn)->PFACE = iSn;
	SOLD( iSn)->NVT = SOLD( iSn)->PVT = *piVn;
	VRTX( *piVn)->NVT = VRTX( *piVn)->PVT = iSn;
	return iSn;
}

//	ソリッドS1の面分F1上に、その構成頂点V1に接続して、新しい稜線Enと新しい頂点Vnを作る
MINT MgNode::mev(							// (  O) 頂点ノードＩＤ
						MINT	iS1,		// (I  ) ソリッドノードＩＤ
						MINT	iF1,		// (I  ) 面分ノードＩＤ
						MINT	iV1,		// (I  ) 頂点ノードＩＤ
						MINT*	piEn		// (  O) 稜線ノードＩＤ
				)
{
	MINT iVn = mkv( iS1);					// 頂点
	*piEn = mke( iS1);						// 稜線
	
	VRTX( iVn)->PED = *piEn;

	EDGE( *piEn)->NFACE = EDGE( *piEn)->PFACE = iF1;
	EDGE( *piEn)->NVT = iVn;
	EDGE( *piEn)->PVT = iV1;
	return iVn;
}

//	稜線E1と頂点V1を削除する
MINT MgNode::kev(
						MINT	iE1			// (I  ) 稜線ノードＩＤ
				)
{
	MINT iVd = EDGE( iE1)->PVT;
	klv( EDGE( iE1)->NVT);
	kle( iE1);								// 残作業->この稜線にリンクしている平面と頂点のリンク先の変更
	return iVd;
}

//	split edge, make vertex
//	ソリッドS1の面分F1上に、新しい稜線Enと新しい頂点Vnを作ることにより稜線を分割する
MINT MgNode::semv(							// (  O) 頂点ノードＩＤ
						MINT	iS1,		// (I  ) ソリッドノードＩＤ
						MINT	iE1,		// (I  ) 面分ノードＩＤ
						MINT*	piEn		// (  O) 稜線ノードＩＤ
				)
{
	MINT iVn = mkv( iS1);					// 頂点
	*piEn = mke( iS1);						// 稜線

	EDGE( *piEn)->PFACE = EDGE( iE1)->PFACE;
	EDGE( *piEn)->NFACE = EDGE( iE1)->NFACE;
	EDGE( *piEn)->PVT = EDGE( iE1)->PVT;
	EDGE( *piEn)->NVT = iVn;
	EDGE( *piEn)->PED = *piEn;

	EDGE( iE1)->PVT = iVn;
	return iVn;
}

//	join edges, kill vertex
//	頂点V1と続く稜線Edを削除することにより頂点V1をまたぐ２つの稜線を結合する
MINT MgNode::jekv(							// (  O) 稜線ノードＩＤ
						MINT	iVd			// (I  ) 頂点ノードＩＤ
				)
{
	MINT iCW;
	MINT iCCW;
	MINT iV1;
	MINT iE1;
	MINT iEd = VRTX( iVd)->PED;

	if ( EDGE( iEd)->NVT == iVd) {
		if (EDGE( iEd)->NCW != EDGE( iEd)->NCCW)
			MbSysError( PROG_NAME, 999);
		iE1 = EDGE( iEd)->NCW;
		iCW = EDGE( iEd)->PCW;
		iCCW = EDGE( iEd)->PCCW;
		iV1 = EDGE( iEd)->PVT;
	} else {
		if (EDGE( iEd)->PCW != EDGE( iEd)->PCCW)
			MbSysError( PROG_NAME, 999);
		iE1 = EDGE( iEd)->PCW;
		iCW = EDGE( iEd)->NCW;
		iCCW = EDGE( iEd)->NCCW;
		iV1 = EDGE( iEd)->NVT;
	}

	if ( EDGE( iE1)->PVT == iVd) {
		EDGE( iE1)->PCW = iCW;
		EDGE( iE1)->PCCW = iCCW;
		EDGE( iE1)->PVT = iV1;
	} else {
		EDGE( iE1)->NCW = iCW;
		EDGE( iE1)->NCCW = iCCW; 
		EDGE( iE1)->NVT = iV1;
	}

	klv( iV1);
	kle( iEd);
	return iE1;
}

//	ソリッドS1の面分F1上に、新しい稜線Enと新しい面分Fnを作ることにより面分を分割する
MINT MgNode::mef(							// (  O) 稜線ノードＩＤ
						MINT	iS1,		// (I  ) ソリッドノードＩＤ
						MINT	iV1,		// (I  ) 頂点ノードＩＤ１
						MINT	iF1,		// (I  ) 面分ノードＩＤ
						MINT	iV2,		// (I  ) 頂点ノードＩＤ２
						MINT*	piFn		// (  O) 面分ノードＩＤ
				)
{
	MINT iEn = mke( iS1);					// 稜線
	*piFn = mkf( iS1);						// 頂点

	EDGE( iEn)->PFACE = iF1;
	EDGE( iEn)->NFACE = *piFn;
	EDGE( iEn)->PVT = iV1;
	EDGE( iEn)->NVT = iV2;

	FACE( *piFn)->PED = iEn;
	return iEn;
}

//	稜線Enおよび面分Fnを削除し、En.PFACEを出力する
MINT MgNode::kef(							// (  O) 面分ノードＩＤ
						MINT	iEd			// (I  ) 稜線ノードＩＤ
				)
{
	MINT iFd = EDGE( iEd)->NFACE;
	MINT iF1 = EDGE( iEd)->PFACE;


	kle( iEd);
	klf( iFd);
	return iF1;
}

//	リング上の１点と外部ループ上の１点を指定して、それらを結ぶ稜線を作ることにより、リングを除去する
MINT MgNode::mekr(							// (  O) 
						MINT	iS1,		// (I  ) ソリッドノードＩＤ
						MINT	iV1,		// (I  ) 頂点ノードＩＤ１
						MINT	iV2			// (I  ) 頂点ノードＩＤ２
				)
{
	MINT iEn = mke( iS1);
	EDGE( iEn)->PVT = iV2;
	EDGE( iEn)->NVT = iV1;
	return iEn;
}

//	リング上の１点と外部ループ上の１点を指定して、それらを結ぶ稜線を作ることにより、リングを除去する
MINT MgNode::kekr(							// (  O) 
						MINT	iEd			// (I  ) 稜線ノードＩＤ
				)
{
	kle(iEd);
	return iEn;
}



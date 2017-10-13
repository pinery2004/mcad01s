#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgPoint.h
//
//		線分の結合
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
//

#ifdef DLL_EXPORT_CNCT
	#undef DLL_EXPORT_CNCT
#endif

#ifdef DLL_EXPORT_CONECT_DO
	#pragma message( "<<< MgConnect・dll_EXport >>>")
	#define DLL_EXPORT_CNCT							__declspec( dllexport)
#else
	#ifdef _DEBUG								// Release Mode で inline関数がundefになる不具合対応
		#ifdef DLL_NO_IMPORT_GEO_DO
			#define DLL_EXPORT_CNCT
		#else
//			#pragma message( "=== MgConnect・dll_IMport ===")
			#define DLL_EXPORT_CNCT						__declspec( dllimport)
		#endif
	#else
		#define DLL_EXPORT_CNCT
	#endif
#endif

#define		MSZNODEBF	10000
#define		MSZNODE		12

#define		MASK8		0x80
#define		MASK7		0x40
#define		MASK6		0x20
#define		MASK5		0x10
#define		MASK4		0x08
#define		MASK3		0x04
#define		MASK2		0x02
#define		MASK1		0x01

namespace MC
{

//
//======================( ３次元 )==============================
//		ウィングドエッジデータ構造
//
class DLL_EXPORT_CNCT MgNode
{
public:
//	MINT m_Node[MSZNODEBF][MSZNODE];
	MINT*		m_Node;
	
	MgNode( MINT iSzNode);
	
	void wing( MINT iE1, MINT iE2);
	class MgWorldNode* WRLD();
	class MgEmptyNode* EMPT( MINT iM1);
	class MgSolidNode* SOLD( MINT iB1);
	class MgEdgeNode* EDGE( MINT iE1);
	class MgFaceNode* FACE( MINT iF1);
	class MgVertexNode* VRTX( MINT iV1);
	void CLR( MINT iN1);
	void CLR( void* pN1);

	//	面分、稜線、頂点の各リグは空の状態でソリッドのノードを精製し、
	//	ワールドブロックのソリッドのリングに含める
	MINT mks();
	//	新しい使用可能なノードを生成する
	MINT mkm();
	//	新しい面分のノードを生成し、面分のリング中に含める(リンクの最終に付ける)
	MINT mkf( MINT iS1);
	//	新しい稜線のノードを生成し、稜線のリング中に含める(リンクの最終に付ける)
	MINT mke( MINT iS1);
	//	新しい頂点のノードを生成し、頂点のリング中に含める(リンクの最終に付ける)
	MINT mkv( MINT iS1);
	// ノードをリングより除去し、自由領域への返却を行う
	void klm( MINT iM1);
	// ソリッドのノードをリングより除去し、自由領域への返却を行う
	void kls( MINT iS1);
	// 面分のノードをリングより除去し、自由領域への返却を行う
	void  klf( MINT iF1);
	// 稜線のノードをリングより除去し、自由領域への返却を行う
	void  kle( MINT iE1);
	// 頂点のノードをリングより除去し、自由領域への返却を行う
	void  klv( MINT iV1);
	//	一つのソリッドSn、一つの面分Fn、一つの頂点Vnよりなる縮退した点をあらわす多面体を作る
	MINT mvsf( MINT	*piFn, MINT *piVn);
	//	ソリッドS1の面分F1上に、その構成頂点V1に接続して、新しい稜線Enと新しい頂点Vnを作る
	MINT mev( MINT iS1, MINT iF1, MINT iV1, MINT* piEn);
	//	稜線E1と頂点V1を削除する
	MINT kev( MINT iE1);
	//	ソリッドS1の面分F1上に、新しい稜線Enと新しい頂点Vnを作ることにより稜線を分割する
	MINT semv( MINT iS1, INT iE1, MINT* piEn);
	//	join edges, kill vertex
	//	頂点V1と続く稜線Edを削除することにより頂点V1をまたぐ２つの稜線を結合する
	MINT jekv( MINT	iVd);
	//	ソリッドS1の面分F1上に、新しい稜線Enと新しい面分Fnを作ることにより面分を分割する
	MINT mef( MINT iS1, MINT iV1, MINT iF1, MINT iV2, MINT*	piFn);
	//	稜線Enおよび面分Fnを削除し、En.PFACEを出力する
	MINT kef( MINT iEd);
};

class DLL_EXPORT_CNCT MgWorldNode
{
public:
	MUINT		TMP1[3];
	MINT		St;							// 状態	
	MUSHORT		NSOLD;						// Body ring
	MUSHORT		PSOLD;
	MINT		TMP3[6];
	MUSHORT		nlnk;						// 空きエリア リンク
	MUSHORT		plnk;
};

class DLL_EXPORT_CNCT MgEmptyNode
{
public:
	MUINT		TMP1[3];
	MINT		St;							// 状態	
	MUINT		TMP2[2];
	MUSHORT		NEMPT;						// Empty ring
	MUSHORT		PEMPT;
	MINT		TMP3[4];
	MUSHORT		nlnk;						// User links
	MUSHORT		plnk;
};

class DLL_EXPORT_CNCT MgSolidNode
{
public:
	MUINT		TMP;
	MUINT		PNAME1;
	MUINT		PNAME2;
	MINT		St;							// 状態	
	MUSHORT		NFACE;						// Fase ring
	MUSHORT		PFACE;
	MUSHORT		NED;						// Edge ring
	MUSHORT		PED;
	MUSHORT		NVT;						// Vertex ring
	MUSHORT		PVT;
	MUSHORT		DAD;						// Pts Tree
	MUSHORT		SON;
	MUSHORT		BRO;						// Pts Tree
	MUSHORT		SIS;
	MUSHORT		alt;						// Body TRAM
	MUSHORT		TRAM;
	MUSHORT		CW;							// Body ring
	MUSHORT		CCW;
	MUSHORT		nlnk;						// User links
	MUSHORT		plnk;
};

class DLL_EXPORT_CNCT MgFaceNode
{
public:	
	MBYTE		Tp;							// 線種	( 1:閉区画、2:複合線分区画 )
// ↓
	MINT		AA;							// FacePlane
	MINT		BB;							// normal
	MINT		CC;							// vector
	MINT		St;							// 状態	
											// 		MASK8:	有効フラグ
											// 
	MUSHORT		NFACE;						// Face ring
	MUSHORT		PFACE;
//
	MUSHORT		Ncnt;						// First edge
	MUSHORT		PED;
	MREAL		KK;							// Distance to origin
	MUINT		rgb;						// red, grn, blue, wht Reflectivities(反射率)
	MUINT		lrgb;						// Lr, Lg, Lb, Lw, Sm, Sn Lumns & Spec Coef(スペック係数) 
	MUSHORT		alt;						// Temporaries
	MUSHORT		slt2;
	MINT		OO;							// Video Intensity
	MUSHORT		nlnk;						// User links
	MUSHORT		plnk;
};

class DLL_EXPORT_CNCT MgEdgeNode
{
public:
//	MBYTE		Tp;							// 線種	( 1:線分、2:円弧、3:円 )
// ↓
	MINT		AA;							// display Coord( x1dc, y1dc) or
	MINT		BB;							// 2-D Edge Coet or
	MINT		CC;							// 3-D line Cosines
	MINT		St;							// 状態	
											// 		MASK8:	有効フラグ
											// 		MASK7:	始点→終点方向の領域定義済み
											// 		MASK6:	終点→始点方向の領域定義済み
											// 
	
//	MINT		idLine;						// 線分ID
	MUSHORT		NFACE;						// Two Faces
	MUSHORT		PFACE;
	MUSHORT		NED;						// Edge ring
	MUSHORT		PED;
	MUSHORT		NVT;						// Two Vertices
	MUSHORT		PVT;
	MUSHORT		NCW;						// Clockwise Wing Edges
	MUSHORT		PCW;
	MUSHORT		NCCW;						// CCW Wing Edges
	MUSHORT		PCCW;
	MUSHORT		alt;						// Temporaries
	MUSHORT		slt2;
	MUSHORT		CW;							// Temporaries
	MUSHORT		CCW;
	MUSHORT		nlnk;						// User links
	MUSHORT		plnk;
//	union _z{
//		MgLine2	Ln;							// 線分
//		MgArc	Arc;						// 円弧
//		MgCir	Cir;						// 円
//	} Z;
//	MgMinMax Mm;							// MinMax

	MUSHORT fcwv( MUSHORT v0);				// 頂点V0の周りに、時計回転向きに回転した場合の次の面分を求める
	MUSHORT fccwv( MUSHORT v0);				// 頂点V0の周りに、反時計回転向きに回転した場合の次の面分を求める
	MUSHORT ecwv( MUSHORT v0);				// 頂点V0の周りに、時計回転向きに回転した場合の次のEdgeを求める
	MUSHORT eccwv( MUSHORT v0);				// 頂点V0の周りに、反時計回転向きに回転した場合の次のEdgeを求める
	MUSHORT ecwf( MUSHORT f0);				// 面分f0の周りに、時計回転向きに回転した場合の次のEdgeを求める
	MUSHORT eccwf( MUSHORT f0);				// 面分f0の周りに、反時計回転向きに回転した場合の次のEdgeを求める
	MUSHORT vcwf( MUSHORT f0);				// 面分f0の周りに、時計回転向きに回転した場合の次のEdgeを求める
	MUSHORT vccwf( MUSHORT f0);				// 面分f0の周りに、反時計回転向きに回転した場合の次のEdgeを求める
};

class DLL_EXPORT_CNCT MgVertexNode
{
public:
	MgPoint3	Pt;							// 点
// ↓
	MREAL		XWC;						// World Locus
	MREAL		YWC;						// 
	MREAL		ZWC;						// 
	MINT		St;							// 状態	
											// 		MASK8:	有効フラグ
											// 
	MUSHORT		XDC;						// Display Locus
	MUSHORT		YDC;
	MUSHORT		Tjoint;						// First Edge
	MUSHORT		PED;
	MUSHORT		NVT;						// Vertex ring
	MUSHORT		PVT;
	MREAL		XPP;						// Perspective
	MREAL		YPP;						// Projected Locus
	MREAL		ZPP;						//	alt alt2
	MUSHORT		cw;							// Temporaries
	MUSHORT		ccw;
	MUSHORT		nlnk;						// User links
	MUSHORT		plnk;
};


#include "MgConnect1.h"

} // namespace MC
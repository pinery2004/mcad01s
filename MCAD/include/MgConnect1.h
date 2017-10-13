
// 頂点V0の周りに、時計回転向きに回転した場合の次の面分を求める
MUSHORT MgEdgeNode::fcwv( MUSHORT v0)
{
	return ( ( v0 == PVT) ? NFACE: PFACE);
}

// 頂点V0の周りに、反時計回転向きに回転した場合の次の面分を求める
MUSHORT MgEdgeNode::fccwv( MUSHORT v0)
{
	return ( ( v0 == PVT) ? PFACE: NFACE);
}

// 頂点V0の周りに、時計回転向きに回転した場合の次のEdgeを求める
MUSHORT MgEdgeNode::ecwv( MUSHORT v0)
{
	return ( ( v0 = PVT) ? NCCW: PCCW);
}

// 頂点V0の周りに、反時計回転向きに回転した場合の次のEdgeを求める
MUSHORT MgEdgeNode::eccwv( MUSHORT v0)
{
	return ( ( v0 == PVT) ? NCW: PCW);
}

// 面分f0の周りに、時計回転向きに回転した場合の次のEdgeを求める
MUSHORT MgEdgeNode::ecwf( MUSHORT f0)
{
	return ( ( f0 == PFACE) ? PCW: NCW);
}

// 面分f0の周りに、反時計回転向きに回転した場合の次のEdgeを求める
MUSHORT MgEdgeNode::eccwf( MUSHORT f0)
{
	return ( ( f0 == PFACE) ? PCCW: NCCW);
}

// 面分f0の周りに、時計回転向きに回転した場合の次のEdgeを求める
MUSHORT MgEdgeNode::vcwf( MUSHORT f0)
{
	return ( ( f0 == PFACE) ? PCW: NCW);
}

// 面分f0の周りに、反時計回転向きに回転した場合の次のEdgeを求める
MUSHORT MgEdgeNode::vccwf( MUSHORT f0)
{
	return ( ( f0 == PFACE) ? PCCW: NCCW);
}

// ワールドノードへのポインタを得る
inline MgWorldNode* MgNode::WRLD()			// (  O) ワールドノードへのポインタ
{
	return ( (MgWorldNode *)m_Node);
}

// 空ノードＩＤから空ノードへのポインタを得る
inline MgEmptyNode* MgNode::EMPT(			// (  O) 空ノードへのポインタ
						MINT	iM1			// (I  ) 空ノードＩＤ(配列番号)
				)	
{
	return ( (MgEmptyNode *)&m_Node[iM1 * MSZNODE]);
}

// ソリッドノードＩＤからソリッドノードへのポインタを得る
inline MgSolidNode* MgNode::SOLD(			// (  O) ソリッドノードへのポインタ
						MINT	iB1			// (I  ) ソリッドノードＩＤ(配列番号)
				)
{
	return ( (MgSolidNode *)&m_Node[iB1 * MSZNODE]);
}

// 稜線ノードＩＤから稜線ノードへのポインタを得る
inline MgEdgeNode* MgNode::EDGE(			// (  O) 稜線ノードへのポインタ
						MINT	iE1			// (I  ) 稜線ノードＩＤ(配列番号)
				)
{
	return ( (MgEdgeNode *)&m_Node[iE1 * MSZNODE]);
}

// 面ノードＩＤから面ノードへのポインタを得る
inline MgFaceNode* MgNode::FACE(			// (  O) 面ノードへのポインタ
						MINT	iF1			// (I  ) 面ノードＩＤ(配列番号)
				)
{
	return ( (MgFaceNode *)&m_Node[iF1 * MSZNODE]);
}

// 頂点ノードＩＤから頂点ノードへのポインタを得る
inline MgVertexNode* MgNode::VRTX(			// (  O) 頂点ノードへのポインタ
						MINT	iV1			// (I  ) 頂点ノードＩＤ(配列番号)
				)
{
	return ( (MgVertexNode *)&m_Node[iV1 * MSZNODE]);
}

// ノードをゼロクリアする
inline void MgNode::CLR(
						MINT	iN1			// (I  ) ノードＩＤ(配列番号)
				)
{
	memset( &m_Node[iN1], 0, MSZNODE * sizeof(MINT));
}

// ノードをゼロクリアする
inline void MgNode::CLR(
						void*	pN1			// (I  ) ノードへのポインタ
				)
{
	memset( pN1, 0, MSZNODE * sizeof(MINT));
}

//	空ノードを生成する
inline MINT MgNode::mkm()					// (  O) 空きノードＩＤ(配列番号)
{
	// 自由領域の先頭ノードを使用可能なノードとする
	MINT iM1 = WRLD()->nlnk;
	
	// 自由領域からのリンクを取り除く
	WRLD()->nlnk = EMPT( iM1)->nlnk;							// 取得ノードの次のノード
	EMPT( WRLD()->nlnk)->plnk = 0;								// WORLDノード

	return iM1;
}

//	面分、稜線、頂点の各リングは空の状態でソリッドのノードを精製し、
//	ワールドブロックのソリッドのリングに含める
inline MINT MgNode::mks()					// (  O) ソリッドノードＩＤ(配列番号)
{
	// 使用可能なノードをソリッドのノード	とする
	MINT iS1 = mkm();
	
	//	面分、稜線、頂点の各リングは空の状態にする
	SOLD( iS1)->NFACE = SOLD( iS1)->PFACE = iS1;				// 面リングは空
	SOLD( iS1)->NED = SOLD( iS1)->PED = iS1;					// 稜線リングは空
	SOLD( iS1)->NVT = SOLD( iS1)->PVT = iS1;					// 頂点リングは空

	return iS1;
}

//	新しい面分のノードを生成し、面分のリング中に含める(リンクの最終に付ける)
inline MINT MgNode::mkf(					// (  O) 面分ノードＩＤ(配列番号)
						MINT	iS1			// (I  ) ソリッドノードＩＤ(配列番号)
				)
{
	// 使用可能なノードを面分のノードとする
	MINT iF1 = mkm();
	
	// 面分のノードをソリッドの面分のリンクの最終に付ける
	FACE( iF1)->NFACE = iS1;
	FACE( iF1)->PFACE = SOLD( iS1)->PFACE;
	FACE( FACE( iF1)->PFACE)->NFACE = iF1;
	SOLD( iS1)->PFACE = iF1;

	return iF1;
}

//	新しい稜線のノードを生成し、稜線のリング中に含める(リンクの最終に付ける)
inline MINT MgNode::mke(					// (  O) 稜線ノードＩＤ(配列番号)
						MINT	iS1			// (I  ) ソリッドノードＩＤ(配列番号)
				)
{
	// 使用可能なノードを稜線のノードとする
	MINT iE1 = mkm();

	// 稜線のノードをソリッドの稜線のリンクの最終に付ける
	EDGE( iE1)->NED = iS1;
	EDGE( iE1)->PED = SOLD( iS1)->PED;
	EDGE( EDGE( iE1)->PED)->NED = iE1;
	SOLD( iS1)->PED = iE1;

	return iE1;
}

//	新しい頂点のノードを生成し、頂点のリング中に含める(リンクの最終に付ける)
inline MINT MgNode::mkv(						// (  O) 頂点ノードＩＤ(配列番号)
						MINT	iS1				// (I  ) ソリッドノードＩＤ(配列番号)
				)
{
	// 使用可能なノードを頂点のノードとする
	MINT iV1 = mkm();

	// 頂点のノードをソリッドの頂点のリンクの最終に付ける
	VRTX( iV1)->NVT = iS1;
	VRTX( iV1)->PVT = SOLD( iS1)->PVT;
	VRTX( VRTX( iV1)->PVT)->NVT = iV1;
	SOLD( iS1)->PVT = iV1;

	return iV1;
}

// ノードをリングより除去し、自由領域への返却を行う
inline void MgNode::klm(
						MINT	iM1				// (I  ) ノードＩＤ (配列番号)
				)
{
	// ノードをリングより除去する
	MINT iMn = EMPT( iM1)->nlnk;
	MINT iMp = EMPT( iM1)->plnk;
	EMPT( iMn)->plnk = iMp;
	EMPT( iMp)->nlnk = iMn;

	// フリーになったノードを、自由領域の先頭ノードとしてリンク付ける
	EMPT( iM1)->nlnk = 0;
	EMPT( iM1)->plnk = WRLD()->plnk;
	WRLD()->plnk = iM1;
}

// ソリッドのノードをリングより除去し、自由領域への返却を行う
inline void MgNode::kls(
						MINT	iS1				// (I  ) ソリッドのノードＩＤ (配列番号)
				)
{
	klm( iS1);
}

// 面分のノードをリングより除去し、自由領域への返却を行う
inline void  MgNode::klf(
						 MINT	iF1				// (I  ) 面分ノードＩＤ (配列番号)
				)
{
	klm( iF1);
}

// 稜線のノードをリングより除去し、自由領域への返却を行う
inline void  MgNode::kle(
						 MINT	iE1				// (I  ) 稜線ノードＩＤ (配列番号)
				)
{
	klm( iE1);
}

// 頂点のノードをリングより除去し、自由領域への返却を行う
inline void  MgNode::klv(
						 MINT	iV1				// (I  ) 頂点ノードＩＤ (配列番号)
				)
{
	klm( iV1);
}


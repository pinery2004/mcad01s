#pragma once

namespace MC
{

////////////////////////////////////////////////////////////////////////////
//	ＤＸＦファイルの読込み
MINT MDxfFlLoad();

struct MdPoint3 {
	MREAL		x;
	MREAL		y;
	MREAL		z;
};

struct MdLine3 {
	MdPoint3	p[2];
};

struct MdCir3 {
	MdPoint3	plu;									// Left Up
	MdPoint3	prd;									// Right Down
};

struct MdArc3 {
	MdPoint3	plu;									// Left Up
	MdPoint3	prd;									// Right Down
	MdPoint3	ps;										// Start
	MdPoint3	pe;										// End
};

struct MdText3 {
	MdPoint3	Pnt;									// 配置原点（座標値）
	MREAL		Ang;									// 角度    （角度：単位度）
	MREAL		Hei;									// 文字高さ（高さ：単位mm）
	MREAL		Msc;									// 文字高さとスペーシングの比率
	MINT		Num;									// 文字数  （個数）
	MCHAR*		String; 								// 文字列
};

struct MdFace3 {
	MINT		fClose;									// 1:Polygon	0:PlyLine
	MINT		n;										// 頂点数 <= 4
	MINT		p[4];									// 頂点
};

struct  MdMesh3 {
	MINT		nMesh;									// ﾒｯｼｭ数
	MINT*		nVerNumPnt;								// ﾒｯｼｭ毎の頂点数列
	MINT		nPntNo;									// 頂点番号数
	MINT*		PntNo;									// 頂点番号列
	MINT		nPt;									// 頂点数
	MgPoint3*	Pt;										// 頂点列
};

union MdPrm {											// 図形要素
	MdPoint3	Dot;									// 点
	MdLine3		Line;									// 線
	MdCir3		Circle;									// 円
	MdArc3		Arc;									// 円弧
	MdText3		Text;									// 文字列
	MdFace3		Face;									// 面
	MdMesh3		Mesh;									// メッシュ
};

} // namespace MC
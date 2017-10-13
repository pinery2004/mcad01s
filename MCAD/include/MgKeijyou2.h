#pragma once

namespace MC
{

// 図形型

// 点型
// MgPoint2	or MgVect2
// MgPoint3 or MgVect3

// 直線型
// MgULine2
// MgULine3

// 線分型
// MgLine2
// MgLine3

// 円型
struct	MgCir2 {
	MgPoint2			CPnt;				// 中心点
	MREAL			Rad;				// 半径
};

struct	MgCir3 {
	MgPoint3		CPnt;				// 中心点
	MgPoint3		Vect;				// 平面の単位法線ﾍﾞｸﾄﾙ
	MREAL			Rad;				// 半径
};
/*
struct	MgCirs2 {
	MgPoint2			Pnt1;				// 外接する左上
	MgPoint2			Pnt2;				// 外接する右下
};

struct	MgCirs3 {
	MgPoint3		Pnt1;				// 外接する左上
	MgPoint3		Pnt2;				// 外接する右下
};
*/
// 円弧型
struct	MgArc2 {
	MgPoint2			CPnt;				// 中心点
	MgPoint2			Pnt[2];				// 始点、終点
};

struct	MgAarc3 {
	MgPoint3		CPnt;				// 中心点
	MgPoint3		Vect;				// 平面の単位法線ﾍﾞｸﾄﾙ
 	MgPoint3		Pnt[2];				// 始点、終点
};
/*
struct	MgArcx2 {
	MgPoint2			CPnt;				// 中心点（中心点座標値）
	MREAL 			Rad;				// 半径（長さ）
	MREAL 			Sta;				// 始点角（角度：単位度）
	MREAL 			End;				// 終点角（角度：単位度）
};

struct	MgArcx3 {
	MgPoint3		CPnt;				// 中心点（中心点座標値）
	MREAL 			Rad;				// 半径（長さ）
	MREAL 			Sta;				// 始点角（角度：単位度）
	MREAL 			End;				// 終点角（角度：単位度）
};

struct	MgArcs2 {
	MgPoint2			Pnt1;				// 外接する左上
	MgPoint2			Pnt2;				// 外接する右下
	MgPoint2			Pnt3;				// 開始点
	MgPoint2			Pnt4;				// 終了点
};

struct	MgArcs3 {
	MgPoint3		Pnt1;				// 外接する左上
	MgPoint3		Pnt2;				// 外接する右下
	MgPoint3		Pnt3;				// 開始点
	MgPoint3		Pnt4;				// 終了点
};
*/
// 楕円型
struct	MgEps2 {
	MgPoint2			CPnt;				// 中心点( x, y)
	MgPoint2			FPnt;				// 焦点
	MREAL			Lrad;				// 長径
};

struct	MgEps3 {
	MgPoint3		CPnt;				// 中心点(x, y, z)
	MgPoint3		FPnt;				// 焦点  (x, y, z)
	MgPoint3		Vect;				// 平面の単位法線ﾍﾞｸﾄﾙ(x, y, z)
	MREAL			Lrad;				// 長径
};

// 楕円弧型
struct	MgAel2 {
	MgPoint2			CPnt;				// 中心点
	MgPoint2			FPnt;				// 焦点
	MREAL			Lrad;				// 長径
	MgPoint2			SPnt;				// 始点
	MgPoint2			EPnt;				// 終点
};

struct	MgAeLn3  {
	MgPoint3		CPnt;				// 中心点
	MgPoint3		FPnt;				// 焦点
	MgPoint3		Vect;				// 平面の単位法線ﾍﾞｸﾄﾙ
	MREAL			Lrad;				// 長径
	MgPoint3		SPnt;				// 始点
	MgPoint3		EPnt;				// 終点
};

// 面型
struct	MgPLn3 {
	MREAL			Dist;				// 原点からの距離
	MgPoint3		Vect;				// 単位法線ﾍﾞｸﾄﾙ
};

// 円筒面型
struct	MgCyLn3  {
	MgPoint3		Pnt;				// 軸上の点
	MgPoint3		Vect;				// 中心軸ﾍﾞｸﾄﾙ
	MREAL			Rad;				// 半径
	MREAL			Height;				// 高さ
};

// 連続線分型 *******************
struct MgPLine2 {						// ２次元連続線分
	MINT			NumPnt;				// 頂点数
	MgPoint2*		Pnt;				// 頂点座標へのポインタ
};

struct MgPLine3 {						// ３次元連続線分
	MINT			NumPnt;				// 頂点数
	MgPoint2*		Pnt;				// 頂点座標へのポインタ
};
/*
// 面
struct MgFace3 {
	MINT			CFlag;				// 始点･終点   0:閉じない  1:閉じる
	MINT			NumPnt;				// 頂点数
	MgPoint3		Pnt[4];				// 頂点座標
};
*/
/*
// 領域
struct	MgArea2 {						// ２次元領域
	MINT			NumPnt;				// 点列数
	MgPoint2*		Pnt;				// 座標値へのﾎﾟｲﾝﾀ
	MgArea2*			NxtAra;				// 次領域へのﾎﾟｲﾝﾀ
	MgArea2*			AnaAra;				// 穴領域へのﾎﾟｲﾝﾀ
};

struct	MgArea3 {						// ３次元領域
	MINT			NumPnt;				// 点列数
	MgPoint3*		Pnt;				// 座標値へのﾎﾟｲﾝﾀ
	MgArea3*		NxtAra;				// 次領域へのﾎﾟｲﾝﾀ
	MgArea3*		AnArea;				// 穴領域へのﾎﾟｲﾝﾀ
};

// メッシュ
struct	MgMesh3 {
	MINT			Num;				// ﾒｯｼｭ数
	MINT*			VerNumPnt;			// ﾒｯｼｭ毎の頂点数列
	MINT			NumPntNo;			// 頂点番号数
	MINT*			PntNo;				// 頂点番号列
	MINT			NumPnt;				// 頂点数
	MgPoint3*		Pnt;				// 頂点列
};
*/
// スプライン曲線の構成点属性
struct	g_mgCsc2 {
	MgPoint2			Pnt;				// 構成点座標値
	MgPoint2			Vect;				// 構成点の方向単位ベクトル
	MREAL			In;					// 入力方向の大きさ
	MREAL			Out;				// 出力方向の大きさ
};

struct	g_mgCsc3 {
	MgPoint3		Pnt;				// 構成点座標値
	MgPoint3		Vect;				// 構成点の方向単位ベクトル;
	MREAL			In; 				// 入力方向の大きさ
	MREAL			Out;				// 出力方向の大きさ
};

// スプライン曲線型
struct	MgSpl2 {
	MINT			Num;				// 点の数
	g_mgCsc*			Spl;				// 点列と各点での1次微係数
};

struct	MgSPln3 {
	MINT			Num;				// 点の数
	g_mgCsc3*			Spl;				// 点列と各点での1次微係数
};

// ベクトル
struct	MgVec2 {
	MgPoint2			Vect;				// 単位ベクトル
	MREAL			Val;				// 大きさ
};

struct	MgVec3 {
	MgPoint3		Vect;				// 単位ベクトル
	MREAL			Val;				// 大きさ
};


// スプライン曲線上の区間番号およびパラメタ値
struct	MgParm {
	MINT			Kno;				// 区間番号
	MREAL			Par;				// パラメタ値
};

} // namespace MC
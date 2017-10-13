#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: .h
//
//		
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
namespace MC
{

//===========================================================================
//		形状演算
//
// ---------------------( ２次元 )------------------------------
//
//	同次座標による２次元点
//
struct MgPoint2D{
	union{
		MREAL x;
		MREAL v[1];
	};
	MREAL y;
	MREAL w;
public:
	MgPoint2D(MREAL x, MREAL y, MREAL w)
	{MgPoint2D::x = x; MgPoint2D::y = y; MgPoint2D::w = w;}
};
//
// ---------------------( ３次元 )------------------------------
//
//	同次座標による３次元点
//
struct MgPoint2D3 {
	union{
		MREAL	x;
		MREAL v[1];
	};
		MREAL y;
		MREAL z;
		MREAL w;
public:
	MgPoint2D3()			{}
	MgPoint2D3(MREAL x, MREAL y, MREAL z, MREAL w)
				{MgPoint2D3::x = x; MgPoint2D3::y = y;
				 MgPoint2D3::z = z; MgPoint2D3::w = w;}
	friend MREAL operator ^ (MgPoint2D3 p1, MgPoint2D3 p2)					// 内積 cos(191)
				 		{return (p1.x*p2.x + p1.y*p2.y + p1.z*p2.z + p1.w*p2.w);}
	friend MgPoint2D3 operator * (MgPoint2D3 p1, MgPoint2D3 p2)					// 外積 sin(190)
				 		{return MgPoint2D3(p1.y*p2.z - p1.z*p2.y,
						                  p1.z*p2.x - p1.x*p2.z,
									      p1.x*p2.y - p1.y*p2.x, 
										  p1.w*p2.w);}
};
//
//	同次座標による３次元線分
//
struct MgLine2D3 {
	MgPoint2D3 p[2];
public:
	MgLine2D3(const MgPoint2D3& p1, const MgPoint2D3& p2)
			{p[0] = p1; p[1] = p2;}
};
//
//	同次座標による３次元３角形
//
struct MgTriangleD3 {
	MgPoint2D3	p[3];
public:
	MgTriangleD3(const MgPoint2D3& p1, const MgPoint2D3& p2, const MgPoint2D3& p3)
				{p[0] = p1; p[1] = p2; p[2] = p3;}
};
//
//	同次座標による３次元多角形
//
struct MgPolyg2onD3 {
	int	n;
	MgPoint2&	VDT;			//	頂点座標
	int	&PN;				//	縁の線分の回転方向
	int	&CC;				//	頂点の凹凸
	int	&TR;				//	３角形分割線分
};
//
//	面分に関連する特性値と関係データ
//
struct MgPlane3D3 {
	MREAL	a;				//	平面係数	平面の表向き単位ベクトル
	MREAL	b;				//				平面の表向き単位ベクトル
	MREAL	c;				//				平面の表向き単位ベクトル
	MREAL	d;				//				原点から面分平面に至る
							//				符号付き垂直距離
	MREAL	s;				//	面積
//	----------------------------------------------------------------
	MREAL	H;				//	高さ
	MREAL	A;				//　面積
//
	MREAL	PVO;			//	原点との角錐の体積
	MREAL PVA;			//	
//
	MREAL	NX;				//	面ベクトル	平面の表向きベクトル
	MREAL	NY;				//				平面の表向きベクトル
	MREAL	NZ;				//				平面の表向きベクトル
	MREAL	D;				//				原点から面分平面に至る
							//				符号付き垂直距離
//
	MREAL	Ma;				//	最近傍交差稜線　α
	MREAL	Mb;				//				　　β
//	MREAL	c;				//				平面の表向き単位ベクトル
//	MREAL	d;				//				原点から免分平面に至る
//							//				符号付き垂直距離
//	MREAL	a;				//	平面係数	平面の表向き単位ベクトル
//	MREAL	b;				//				平面の表向き単位ベクトル
//
//
	MREAL	I;				//	内積
//
	MREAL	RX;				//	点座標		X
	MREAL	RY;				//				Y
	MREAL	RZ;				//				Z
	MREAL	RW;				//				W
public:
	MgPlane3D3()				{}
	MgPlane3D3(const MgPoint2D3& p0, const MgPoint2D3& p1, const MgPoint2D3& p2);
	MgPlane3D3(const MgMat3 &m1);

};
//
//	交差状態、遮蔽状態を表すテーブル
//
struct MgISD3 {
	int	i;					//	図形１線分内外
							//	1:その点より干渉が続く
							//	0:その点のみで干渉、その点で干渉が終了
							//	  、またはまったく干渉していない
	int	a;					//	図形１パラメータ
							//	交点の位置（図形１頂点番号相対位置）
	int	b;					//	図形２パラメータ
							//	交点の位置（図形２頂点番号相対位置）
							//	、または図形１の端点が
							//	-1:図形２の内部、-2:図形２の外部
};
//
//
struct MgCVTD3 {
};
//
int MgIntersect(const MgLine2D3 &Ln1, const MgLine2D3 &Ln2, int *ist);
//	２線分の重なり、交差テスト(0,1)
int MgIntersect(const MgPoint2D3& p1, const MgTriangleD3 &t1, int *ist);
//	点と３角形の半空間テスト(2)
int MgInside(const MgTriangleD3 &t1, const MgPoint2D3& p1, int *ist);
//	３角形と点（平面）の内外テスト(3)
int MgIntersect(const MgLine2D3 &Ln1, const MgTriangleD3 &t1);
//	線分と３角形の交差テスト(4,5,6)
void  MgMkCVT(const MgPolyg2onD3 &pg1, MgCVTD3 *cvt);
//	面分の特性を求める(10,11)
void  MbPartition(const MgPolyg2onD3 &pg1, MgCVTD3 *cvt);
//	面分の３角形分割(12)
int MgVolume(const MgPoint2D3& p1, const MgPolyg2onD3 &pg1, MgCVTD3 *cvt);
//	点と面分の錐体体積の算出、距離(20)
int MgInside(const MgPolyg2onD3 &pg1, const MgPoint2D3& p1, int *ist);
//	面分に対する点（平面）の内外テスト(21)
int MgShade(const MgPolyg2onD3 &pg1, const MgPoint2D3& p1, int *ist);
//	面分に対する点（平面）の遮蔽(22)
int MgIntersect(const MgLine2D3 &Ln1, const MgPolyg2onD3 &Ln2, int *ist);
//	線分と面分の交差テスト(30,31,32)
int MgNearestEdge(const MgLine2D3 &Ln1, const MgPolyg2onD3 &Ln2, int *ist);
//	最近傍交差稜線(33)
int MgInside(const MgPolyg2onD3 &pg1, const MgLine2D3 &Ln1, int *ist);
//	面分に対する線分（平面）の内外テスト(34)
int MgShade(const MgPolyg2onD3 &pg1, const MgLine2D3 &Ln1, int *ist);
//	面分に対する線分（平面）の遮蔽(35)
MgPoint2D3 MbRatio(const MgPoint2D3& p1, const MgPoint2D3& p2, MREAL r1);
//	線分ＶａＶｂと内分比r1より、線分ＶａＶｂをr1:(1-r1)に内分する点の
//	同時座標値の算出(192)
void MgSValue(const MgPoint2D3& p1, int a, int b, MgCVTD3 *cvt);
//	点p1(Vx)とa,bより、Sxijk(i=a..b)をSARY[i]に書き込む(200)
//	(j=i+1, k=i+2)
void MgSValue(const MgPoint2D3& p1, const MgPoint2D3& p2, int a, int b, MgCVTD3 *cvt);
//	点p1(Vx)、点p2(Vy)とa,bより、Sxyij(i=a..b)をSARY[i]に書き込む(220)
//	(j=i+1)
void MgSValue(const MgPoint2D3& p1, const MgPoint2D3& p2, const MgPoint2& p3, 
			  int a, int b, MgCVTD3 *cvt);
//	点p1(Vx)、点p2(Vy)、点p3(Vz)とa,bより、Sxyzi(i=a..b)をSARY[i]に書き込む(220)


} // namespace MC
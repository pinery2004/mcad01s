//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MdDsipModel.cpp
//
//		家モデルのディスプレイリスト作成
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"

#include "MgLib.h"

#include <GL/gl.h>
#include <GL/glu.h>

#include "MsOpengl.h"
#include "MsMaterial.h"

#define	DLL_EXPORT_DISPLIST_DO
#include "MdDispList.h"

#define		MP_PANEL_LINE			1
#define		MP_PANEL_RECTANGULAR	2

#pragma warning( disable : 4305 )

namespace MC
{

void Cube( GLfloat lenght);

// マテリアルコード

enum MPMATERIAL1 {
	MTR_JITTAI = 0,
	MTR_TOUMEI
};

enum MPMATERIAL2 {
	MTR_YANE = 0,								// 構造部材
	MTR_YAGIRI,									// 屋根、壁　半透明
	MTR_KABE,									// 矢切、壁　実体
	MTR_BUZAI,									// 屋根　実体
	MTR_KISO,									// 基礎　実体
	MTR_PANEL									// パネル　内側
};

static	MINT	z_iMaterialC1;					// カレントのマテリアル GL_FRONT
static	MATERIAL_LIST	z_Material[8];			// { 0:屋根、1:矢切、2:壁、3:部材、4:基礎、5:パネル_内側、6:パネル_枠、7:壁・屋根半透明}
static	MINT	z_iMtrNo[2][7] = { { 0, 1, 2, 3, 4, 5, 6}, { 7, 7, 7, 7, 7, 7}};
												// 部材表示属性番号
												//		{ 0:実体、1:半透明}、
												//		{ 0:屋根、1:矢切、2:壁、3:部材、4:基礎、5:パネル_内側、6:パネル_枠}
static	LIGHT_MODEL		z_Lmodel;
static	LIGHT			z_Light[5];
void	MsSetLight_MaterialParameter1( void);

////////////////////////////////////////////////////////////////////////////
//【機能】光源、物体パラメータの設定 1
void DispList::MsGlInitialize( void)
{
	// Initialize
	// 3Dシーンの背景を濃いグレーにする。
	// (Color バッファをクリアするときには濃いグレーに設定することの指定)
	glClearColor( 0.2f, 0.2f, 0.2f, 0.0f);

	// Depth バッファの指定 (初期値は1.0であり省略可能)
	glClearDepth( 1.0f);

	// 上の２命令は::glClear(GL_COLOR_BUFFER_BIT | BL_DEPTH_BUFFER_BIT)にて有効となるが
	// glClearは画面の書き換えのたびに実行を要するのでOnPaintで実行する

//	glDepthFunc(GL_LEQUAL);
	// Depth バッファを有効化
	glEnable( GL_DEPTH_TEST);

//【機能】光源、物体パラメータの設定 1
	MsSetLight_MaterialParameter1();
}

////////////////////////////////////////////////////////////////////////////
//【機能】ビューポートサイズの変更
void DispList::MsOnSize(UINT nType, int cx, int cy)
{

	if( 0>= cx || 0>=cy) return;

	glViewport( 0, 0, cx, cy );
//
	glMatrixMode( GL_PROJECTION );								// <PROJECTION MATRIX SETUP>
	glLoadIdentity();											// Reset Projection matrix
/*
	if ( cx <= cy)
		glOrtho(  -2., 2., -2.*(GLfloat)(cy/cx),				// 正射影
				  2.*(GLfloat)(cy/cx), -10.,10.);
	else
		glOrtho( -2.*(GLfloat)(cy/cx),							// 正射影
				 2.*(GLfloat)(cy/cx),  -2., 2., -10.,10.);
*/
//	glFrustum(-1.0, 1.0, -1.0, 1.0, 2.0, 7.0);					// 透視法射影　視体積 縦横座標、縦横比（aspect）、前面(front)と後面(back)の位置を
	gluPerspective( 50.0f, (GLfloat)(cx/cy), 0.5f, 25.0f );		// 透視法射影　視体積 視角度、縦横比（aspect）、前面(front)と後面(back)の位置

	glMatrixMode( GL_MODELVIEW );								// <MODEL VIEW MATRIX SETUP>
	glLoadIdentity();											// Reset Model view matrix
	
//DD	glTranslatef (0.0f, 0.0f, -5.0f);						// 視体積の中に入るよう移動する
	glTranslated (0.0f, 0.0f, -5.0f);							// 視体積の中に入るよう移動する
	
	DispList::SetLightModel1();									// 光源のパラメータの設定1
	DispList::SetLight();											// 光源の設定1
}

////////////////////////////////////////////////////////////////////////////
//【機能】光源、物体パラメータの設定 1
void DispList::MsSetLight_MaterialParameter1( void)
{
	// LIGHT MODEL
	GLSet4f( z_Lmodel.ambient, 0.2, 0.2, 0.2, 1.0);
	z_Lmodel.local_view = 0.0;
	z_Lmodel.two_size   = 0.0;

	// 環境光 LIGHT AMBIENT
	// 物体表面上に均一な光を当て、物体の明暗や影は出来ない．曇りの日の光と同様． 

	// 拡散光 LIGHT DIFFUSE
	// 光は物体表面で全方向に等しく拡散する．光源の位置と法線ベクトルだけで明暗は決定．

	// 鏡面光(ハイライト) LIGHT SPECULAR
	// 特定方向にのみに乱反射するため，光源の位置，法線ベクトル，そして視点の位置に依存して計算．

	// 光源の位置，方向 LIGHT POSITION
	// 光源の位置は3次元空間で一意に指定．物体より近距離の光源は，位置が物体の照光に及ぼす影響大．
	// また，太陽のように極めて遠い光源は、光線が平行になるため，光線の方向が重要．
	// 位置が重要な意味を持つ光源をpositionalな光源，方向が重要な光源をdirectionalな光源という． 

	// スポットライト LIGHT SPOTLIGHT
	// スポットライトとは，光線を放射する範囲が円錐のような形状になるものである．
	// スポットの大きさを角度で指定．なおスポットライトを使用する場合は，
	// 円錐の軸方向を指定して，光線の向きを指定する．

	// 光源の減衰 LIGHT ATTENUATION
	// 現実の光は光源から遠ざかると光量が減少します．減衰係数によって減少量を指定します．
	// ただし，directionalな光源は無限遠に存在するので，減衰係数の指定は意味をなしません． 

	GLSet4f(	z_Light[0].ambient,		0.0,  0.0,  0.0,  1.0);	// 環境光
	GLSet4f(	z_Light[0].diffuse,		0.3,  0.3,  1.3,  1.0);	// 拡散光
	GLSet4f(	z_Light[0].specular,	0.1,  0.1,  0.1,  1.0);	// 鏡面光(ハイライト)
	GLSet4f(	z_Light[0].position,	0.0,  0.0,  1.0,  0.0);	// 光源の位置，方向
	GLSet3f(	z_Light[0].spot_direction,   0.0,  0.0, -1.0);	// スポットライト

	z_Light[0].spot_exponet	= 0.0;
	z_Light[0].spot_cutoff	= 180.0;
	z_Light[0].KC = 1.0;										// 光源の減衰
	z_Light[0].KL = 0.0;
	z_Light[0].KQ = 0.0;

	// 赤
	GLSet4f(	z_Light[1].ambient,		0.0,  0.0,  0.0,  1.0);	// 環境光
	GLSet4f(	z_Light[1].diffuse,		0.5,  0.2,  0.2,  1.0);	// 拡散光
	GLSet4f(	z_Light[1].specular,	0.2,  0.1,  0.1,  1.0);	// 鏡面光(ハイライト)
	GLSet4f(	z_Light[1].position,	2.0,  1.0, -0.5,  1.0);	// 光源の位置，方向

	GLSet4f(	z_Light[2].ambient,		0.0,  0.0,  0.0,  1.0);	// 環境光
	GLSet4f(	z_Light[2].diffuse,		0.3,  0.3,  0.3,  1.0);	// 拡散光
	GLSet4f(	z_Light[2].specular,	0.2,  0.1,  0.1,  1.0);	// 鏡面光(ハイライト)
	GLSet4f(	z_Light[2].position,	2.0,  1.0, -1.0,  1.0);	// 光源の位置，方向

	// 青
	GLSet4f(	z_Light[3].ambient,		0.0,  0.0,  0.0,  1.0);	// 環境光
	GLSet4f(	z_Light[3].diffuse,		0.3,  0.3,  0.6,  1.0);	// 拡散光
	GLSet4f(	z_Light[3].specular,	0.05, 0.05, 0.1,  1.0);	// 鏡面光(ハイライト)
	GLSet4f(	z_Light[3].position,   -0.5, -1.5, -1.0,  1.0);	// 光源の位置，方向

	GLSet4f(	z_Light[4].ambient,		0.0,  0.0,  0.0,  1.0);	// 環境光
	GLSet4f(	z_Light[4].diffuse,		0.2,  0.3,  0.3,  1.0);	// 拡散光
	GLSet4f(	z_Light[4].specular,	0.0,  0.1,  0.1,  1.0);	// 鏡面光(ハイライト)
	GLSet4f(	z_Light[4].position,	2.0,  0.5,  0.5,  0.0);	// 光源の位置，方向

	// 環境光の拡散色 MATERIAL AMBIENT
	// 環境光に対してその物体がどんな色成分を拡散するかを表す．
	// 例えば赤色を指定すると，	たとえ白色光源でも物体は全体的に赤色を帯びる．

	// 拡散光の拡散色 MATERIAL DIFFUSE
	// 拡散光に対してその物体がどんな色成分を拡散するかを表す．
	// 例えば緑を指定すると，たとえ白色光源でも物体の明暗は緑のグラデーションで表現される．

	// 鏡面光の反射色 MATERIAL SPECULAR
	// 鏡面光に対してその物体がどんな色成分を拡散するかを表す．
	// 例えば青色を指定すると，たとえ白色光源でも物体は青色のハイライトを出す．

	// 光沢 MATERIAL SHININESS
	// 鏡面光では物体にハイライトが出るが，物体の質感によってはハイライトが小さくなったり
	// 大きくなったりします．光沢が高いほど，ハイライトが小さく明るい状態(焦点が合っている)になる．
	// この光沢度も物体側特有のパラメータである．

	// 放射色 MATERIAL EMISSION
	// 物体が自ら発している色である．この色成分は光源には無関係になるため，光源のある状況下で
	// 光源の影響を受けない物体(シーン内のランプ等)に使われたりします．

	// 屋根	実体
	GLSet4f(	z_Material[0].ambient,	0.12, 0.23, 0.23, 1.0);	// 環境光の拡散色
	GLSet4f(	z_Material[0].diffuse,	0.12, 0.23, 0.23, 1.0);	// 拡散光の拡散色
	GLSet4f(	z_Material[0].specular,	0.12, 0.23, 0.23, 1.0);	// 鏡面光の反射色
	z_Material[0].shininess[0] = 10.0;							// 光沢
	GLSet4f(	z_Material[0].emission,	0.12, 0.23, 0.34, 1.0);	// 放射色
	
	// 矢切	実体
	GLSet4f(	z_Material[1].ambient,	0.45, 0.5, 0.35, 1.0);	// 環境光の拡散色
	GLSet4f(	z_Material[1].diffuse,	0.56, 0.6, 0.35, 1.0);	// 拡散光の拡散色
	GLSet4f(	z_Material[1].specular,	0.45, 0.5, 0.35, 1.0);	// 鏡面光の反射色
	z_Material[1].shininess[0] = 10.0;							// 光沢
	GLSet4f(	z_Material[1].emission,	0.6, 0.5, 0.4, 1.0);	// 放射色

	// 壁	実体
	memcpy( &z_Material[2], &z_Material[1], sizeof( MATERIAL_LIST));

	// 部材
	GLSet4f(	z_Material[3].ambient,	0.6, 0.6, 0.2, 1.0);	// 環境光の拡散色
	GLSet4f(	z_Material[3].diffuse,	0.8, 0.8, 0.2, 1.0);	// 拡散光の拡散色
	GLSet4f(	z_Material[3].specular,	0.5, 0.5, 0.2, 1.0);	// 鏡面光の反射色
	z_Material[3].shininess[0] = 0.0;							// 光沢
	GLSet4f(	z_Material[3].emission,	0.2, 0.2, 0.1, 1.0);	// 放射色

	// 基礎	実体
	GLSet4f(	z_Material[4].ambient,	0.40, 0.45, 0.38, 1.0);	// 環境光の拡散色
	GLSet4f(	z_Material[4].diffuse,	0.45, 0.45, 0.37, 1.0);	// 拡散光の拡散色
	GLSet4f(	z_Material[4].specular,	0.1, 0.1, 0.1, 1.0);	// 鏡面光の反射色
	z_Material[4].shininess[0] = 10.0;							// 光沢
	GLSet4f(	z_Material[4].emission,	0.43, 0.43, 0.37, 1.0);	// 放射色
	
	// パネル 内側
	GLSet4f(	z_Material[5].ambient,	0.90, 0.25, 0.18, 1.0);	// 環境光の拡散色
	GLSet4f(	z_Material[5].diffuse,	0.95, 0.25, 0.17, 1.0);	// 拡散光の拡散色
	GLSet4f(	z_Material[5].specular,	0.1, 0.1, 0.1, 1.0);	// 鏡面光の反射色
	z_Material[5].shininess[0] = 10.0;							// 光沢
	GLSet4f(	z_Material[5].emission,	0.93, 0.23, 0.17, 1.0);	// 放射色

	// パネル 枠
	memcpy( &z_Material[6], &z_Material[1], sizeof( MATERIAL_LIST));

	// 壁、屋根	半透明
	GLSet4f(	z_Material[7].ambient,	0.1, 0.1, 0.1, 1.0);	// 環境光の拡散色
	GLSet4f(	z_Material[7].diffuse,	0.2, 0.2, 0.2, 1.0);	// 拡散光の拡散色
	GLSet4f(	z_Material[7].specular,	0.1, 0.2, 0.1, 1.0);	// 鏡面光の反射色
	z_Material[7].shininess[0] = 100.0;							// 光沢
}

void DispList::SetLightModel1( void)
{
	// LIGHT MODEL
	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, z_Lmodel.ambient);
	glLightModelf( GL_LIGHT_MODEL_LOCAL_VIEWER, z_Lmodel.local_view);
	glLightModelf( GL_LIGHT_MODEL_TWO_SIDE, z_Lmodel.two_size);
}

////////////////////////////////////////////////////////////////////////////
//【機能】光源の設定
//
void DispList::SetLight( void)
{
	// LIGHT 0
	glEnable( GL_LIGHT0 );
	glLightfv(	GL_LIGHT0, GL_AMBIENT, 				z_Light[0].ambient);
	glLightfv(	GL_LIGHT0, GL_DIFFUSE, 				z_Light[0].diffuse);
	glLightfv(	GL_LIGHT0, GL_SPECULAR, 			z_Light[0].specular);
	glLightfv(	GL_LIGHT0, GL_POSITION, 			z_Light[0].position);

	glLightfv(	GL_LIGHT0, GL_SPOT_DIRECTION, 		z_Light[0].spot_direction);
	glLightf(	GL_LIGHT0, GL_SPOT_EXPONENT, 		z_Light[0].spot_exponet);
	glLightf(	GL_LIGHT0, GL_SPOT_CUTOFF, 			z_Light[0].spot_cutoff);

	glLightf(	GL_LIGHT0, GL_CONSTANT_ATTENUATION, z_Light[0].KC);
	glLightf(	GL_LIGHT0, GL_LINEAR_ATTENUATION, 	z_Light[0].KL);
	glLightf(	GL_LIGHT0, GL_QUADRATIC_ATTENUATION,z_Light[0].KQ);

	// LIGHT 1
	glEnable( GL_LIGHT1 );
	glLightfv(	GL_LIGHT1, GL_AMBIENT,				z_Light[1].ambient);
	glLightfv(	GL_LIGHT1, GL_DIFFUSE,				z_Light[1].diffuse);
	glLightfv(	GL_LIGHT1, GL_SPECULAR,				z_Light[1].specular);
	glLightfv(	GL_LIGHT1, GL_POSITION,				z_Light[1].position);

	// LIGHT 2
	glEnable( GL_LIGHT2 );
	glLightfv(	GL_LIGHT2, GL_AMBIENT,				z_Light[2].ambient);
	glLightfv(	GL_LIGHT2, GL_DIFFUSE,				z_Light[2].diffuse);
	glLightfv(	GL_LIGHT2, GL_SPECULAR,				z_Light[2].specular);
	glLightfv(	GL_LIGHT2, GL_POSITION,				z_Light[2].position);

	// LIGHT 3
	glEnable( GL_LIGHT3 );
	glLightfv(	GL_LIGHT3, GL_AMBIENT,				z_Light[3].ambient);
	glLightfv(	GL_LIGHT3, GL_DIFFUSE,				z_Light[3].diffuse);
	glLightfv(	GL_LIGHT3, GL_SPECULAR,				z_Light[3].specular);
	glLightfv(	GL_LIGHT3, GL_POSITION,				z_Light[3].position);

	// LIGHT 4
	glEnable( GL_LIGHT4 );
	glLightfv(	GL_LIGHT4, GL_AMBIENT,				z_Light[4].ambient);
	glLightfv(	GL_LIGHT4, GL_DIFFUSE,				z_Light[4].diffuse);
	glLightfv(	GL_LIGHT4, GL_SPECULAR,				z_Light[4].specular);
	glLightfv(	GL_LIGHT4, GL_POSITION,				z_Light[4].position);
}

////////////////////////////////////////////////////////////////////////////
//	【機能】部材表示属性(Material)を設定する
//			
void DispList::SetMaterial(
						MINT		i_iT,			// 表示タイプ
						MINT		i_iB			// 部位
				)
{
	MINT	iMtrNo = z_iMtrNo[i_iT][i_iB];
	if ( iMtrNo != z_iMaterialC1) {
		z_iMaterialC1 = iMtrNo;

		// MATERIAL
		glMaterialfv( GL_FRONT, GL_AMBIENT,		z_Material[iMtrNo].ambient);
	    glMaterialfv( GL_FRONT, GL_DIFFUSE,		z_Material[iMtrNo].diffuse);
		glMaterialfv( GL_FRONT, GL_SPECULAR,	z_Material[iMtrNo].specular);
	    glMaterialfv( GL_FRONT, GL_SHININESS,	z_Material[iMtrNo].shininess);
		glMaterialfv( GL_FRONT, GL_EMISSION,	z_Material[iMtrNo].emission);
	}
}

////////////////////////////////////////////////////////////////////////////
//	【機能】直方体のディスプレイリストを作成する (MakeDisplaylist)
//			
void DispList::DspRectangular( 
						MgPoint3	PT[2][2][2],	// 直方体の頂点座標		(論理座標)
				const	MgVect3		&VuLng,			// 始点から終点方向の単位ベクトル
				const	MgVect3		&VuRt,			// 右方向の単位ベクトル
				const	MgVect3		&VuUp			// 上方向の単位ベクトル
				)
{
	glBegin( GL_QUADS);
		glNormal3r( -VuUp.x, -VuUp.y, -VuUp.z);					// 面の法線ベクトル 下方向
		glVertex3r( PT[1][0][0].x, PT[1][0][0].y, PT[1][0][0].z);
		glVertex3r( PT[0][0][0].x, PT[0][0][0].y, PT[0][0][0].z);
		glVertex3r( PT[0][1][0].x, PT[0][1][0].y, PT[0][1][0].z);
		glVertex3r( PT[1][1][0].x, PT[1][1][0].y, PT[1][1][0].z);
	glEnd();

	glBegin( GL_QUADS);
		glNormal3r( VuUp.x, VuUp.y, VuUp.z);					// 面の法線ベクトル 上方向
		glVertex3r( PT[0][0][1].x, PT[0][0][1].y, PT[0][0][1].z);
		glVertex3r( PT[1][0][1].x, PT[1][0][1].y, PT[1][0][1].z);
		glVertex3r( PT[1][1][1].x, PT[1][1][1].y, PT[1][1][1].z);
		glVertex3r( PT[0][1][1].x, PT[0][1][1].y, PT[0][1][1].z);
	glEnd();

	glBegin( GL_QUADS);
		glNormal3r( VuLng.x, VuLng.y, VuLng.z);					// 面の法線ベクトル 前方向
		glVertex3r( PT[1][0][1].x, PT[1][0][1].y, PT[1][0][1].z);
		glVertex3r( PT[1][0][0].x, PT[1][0][0].y, PT[1][0][0].z);
		glVertex3r( PT[1][1][0].x, PT[1][1][0].y, PT[1][1][0].z);
		glVertex3r( PT[1][1][1].x, PT[1][1][1].y, PT[1][1][1].z);
	glEnd();

	glBegin( GL_QUADS);
		glNormal3r( -VuLng.x, -VuLng.y, -VuLng.z);				// 面の法線ベクトル 後方向
		glVertex3r( PT[0][0][0].x, PT[0][0][0].y, PT[0][0][0].z);
		glVertex3r( PT[0][0][1].x, PT[0][0][1].y, PT[0][0][1].z);
		glVertex3r( PT[0][1][1].x, PT[0][1][1].y, PT[0][1][1].z);
		glVertex3r( PT[0][1][0].x, PT[0][1][0].y, PT[0][1][0].z);
	glEnd();

	glBegin( GL_QUADS);
		glNormal3r( -VuRt.x, -VuRt.y, -VuRt.z);					// 面の法線ベクトル 左方向
		glVertex3r( PT[0][1][1].x, PT[0][1][1].y, PT[0][1][1].z);
		glVertex3r( PT[1][1][1].x, PT[1][1][1].y, PT[1][1][1].z);
		glVertex3r( PT[1][1][0].x, PT[1][1][0].y, PT[1][1][0].z);
		glVertex3r( PT[0][1][0].x, PT[0][1][0].y, PT[0][1][0].z);
	glEnd();

	glBegin( GL_QUADS);
		glNormal3r( VuRt.x, VuRt.y, VuRt.z);					// 面の法線ベクトル 右方向
		glVertex3r( PT[0][0][0].x, PT[0][0][0].y, PT[0][0][0].z);
		glVertex3r( PT[1][0][0].x, PT[1][0][0].y, PT[1][0][0].z);
		glVertex3r( PT[1][0][1].x, PT[1][0][1].y, PT[1][0][1].z);
		glVertex3r( PT[0][0][1].x, PT[0][0][1].y, PT[0][0][1].z);
	glEnd();
}

////////////////////////////////////////////////////////////////////////////
//	【機能】３角形のディスプレイリストを作成する
//				(隙間を埋める為に若干大きめに作成する）（要検討）
//			
void DispList::DspTriangles( 
				const	MgPolyg3	&PGTri,			// 三角形		(論理座標)
				const	MgVect3		&VuUp			// ３角形平面の法線ベクトル
				)
{
//#define TRIANGLE_HOSEI 1
#ifdef TRIANGLE_HOSEI
	MgPoint3	Pc;
	MgPoint3	Pc0, Pc1, Pc2;

	Pc = (PGTri.p[0] + PGTri.p[1] + PGTri.p[2]) / 3.;
	Pc0 = (PGTri.p[0] - Pc) * 0.0005;
	Pc1 = (PGTri.p[1] - Pc) * 0.0005;
	Pc2 = (PGTri.p[2] - Pc) * 0.0005;
	PGTri.p[0] += Pc0;
	PGTri.p[1] += Pc1;
	PGTri.p[2] += Pc2;
#endif
	glBegin( GL_TRIANGLES);
		glNormal3r( VuUp.x, VuUp.y, VuUp.z);					// 面の法線ベクトル
		glVertex3r( PGTri.m_P[0].x, PGTri.m_P[0].y, PGTri.m_P[0].z);
		glVertex3r( PGTri.m_P[1].x, PGTri.m_P[1].y, PGTri.m_P[1].z);
		glVertex3r( PGTri.m_P[2].x, PGTri.m_P[2].y, PGTri.m_P[2].z);
	glEnd();
}

////////////////////////////////////////////////////////////////////////////
//	【機能】点のディスプレイリストを作成する
//			
void DispList::DrawPoint(
				const	MgPoint3	&i_Pt,				// 点		(実座標)
				const	MgPoint3	&i_PtCtr,			// 構造家モデルの中心
						MREAL		i_rB				// ３次元表示倍率
				)
{
	MgPoint3	Pt;

	Pt = DPtoDSP( i_Pt, 0., i_PtCtr, i_rB);

	glDisable( GL_LIGHTING);
	glColor4f( 1.0, 0.0, 1.0, 1.0);
	glPointSize( 2.0);
	glBegin( GL_POINTS);
		glVertex3rv( (GLfloat *)Pt.v);
	glEnd();
	glEnable( GL_LIGHTING);
}

////////////////////////////////////////////////////////////////////////////
//	【機能】線分のディスプレイリストを作成する
//			
void DispList::DrawLine(
				const	MgLine3		&i_Ln,				// 線分		(実座標)
				const	MgPoint3	&i_PtCtr,			// 構造家モデルの中心
						MREAL		i_rB				// ３次元表示倍率
				)
{
	MgLine3	Ln;

	Ln = DPtoDSP( i_Ln, 0., i_PtCtr, i_rB);

	glDisable( GL_LIGHTING);
	glColor4f( 1.0, 0.0, 0.0, 1.0);
	glBegin( GL_LINES);
		glVertex3rv( Ln.p[0].v);
		glVertex3rv( Ln.p[1].v);
	glEnd();
	glEnable( GL_LIGHTING);
}


////////////////////////////////////////////////////////////////////////////
//	【機能】４角形のディスプレイリストを作成する
//				(底辺と上辺より作成)
//			
void DispList::DspQuads( 
				const	MgLine3		&LnLf,			// 底辺（右辺）(論理座標)
				const	MgLine3		&LnRt			// 上辺（左辺）(論理座標)
				)
{
	MgVect3		VtLng, VtLf;						// 部材の長さ方向ベクトル、左方向ベクトル
	MgVect3		VuLng, VuUp;						// 部材の長さ方向単位ベクトル、上向単位ベクトル
	
	VtLng = LnLf.p[1] - LnLf.p[0];
	VtLf = LnRt.p[0] - LnLf.p[0];
	VuUp = VtLng ^ VtLf;
	VuUp.SetUnitize( g_gTol.L);									// 論理座標

	glBegin( GL_QUADS);
		glNormal3r( VuUp.x, VuUp.y, VuUp.z);					// 面の法線ベクトル
		glVertex3r( LnLf.p[0].x, LnLf.p[0].y, LnLf.p[0].z);
		glVertex3r( LnLf.p[1].x, LnLf.p[1].y, LnLf.p[1].z);
		glVertex3r( LnRt.p[1].x, LnRt.p[1].y, LnRt.p[1].z);
		glVertex3r( LnRt.p[0].x, LnRt.p[0].y, LnRt.p[0].z);
	glEnd();
}

////////////////////////////////////////////////////////////////////////////
//	【機能】穴付き多角形のディスプレイリストを作成する
//			
void DispList::DrawPolygon(
				const	MgGPolyg3	&GPgR,			// 穴付き多角形	(実座標)
				const	MgPoint3	&PtCtr,			// 構造家モデルの中心
						MREAL		rB				// ３次元表示倍率
				)
{
	MINT			ist1;
	MINT			ic2;
	MgVect3			V1, V2;
	MgVect3			VuRt;
	MGPOLYG3( PgW, 100);
	MgGPolyg3		GPgW(30);
	MGPOLYG3( zPg3, 3);

	V1 = GPgR.m_Pg[0].m_P[1] - GPgR.m_Pg[0].m_P[0];
	V2 = GPgR.m_Pg[0].m_P[2] - GPgR.m_Pg[0].m_P[0];
	VuRt = V1 ^ V2;
	VuRt.SetUnitize();

	MGeo::ConnectGPgtoPg3( GPgR, VuRt, &PgW);

	GPgR.Print( Mstr( "GPgR"));
	VuRt.Print( Mstr( "VuRt"));
	PgW.Print( Mstr( "PgW"));

	ist1 = MGeo::DivideTriPg3( PgW, VuRt, &GPgW);

	GPgW.Print( Mstr( "GPgW"));

	for ( ic2=0; ic2<GPgW.m_n; ic2++) {
		DPtoDSP3( GPgW.m_Pg[ic2], 0., MgVect3( 0., 0., 0.), PtCtr, rB, &zPg3);		// 本関数の引数の数は要検討	
		DspTriangles( zPg3, VuRt);
	}
}

} // namespace MC
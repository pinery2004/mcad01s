//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MsDispModel.cpp
//
//		家モデルのディスプレイリスト作成
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"

#include "MgLib.h"
#include "MmSystemProperty.h"

#include "MhInp.h"
#define DLL_EXPORT_MDLDISPLIST_DO
#include "MnMdlDispList.h"
#include "MnLib.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "GlLib.h"

#include "MsOpengl.h"

#define		MP_PANEL_LINE			1
#define		MP_PANEL_RECTANGULAR	2

namespace MC
{

void Cube( GLfloat lenght);

static	GLuint	z_DispListFig[4];			// 図形ディスプレイリスト
											//	0: 部材		マテリアル付き
											//	1: 壁・矢切	マテリアルなし
											//	2: 屋根		マテリアルなし
static	GLuint	z_DispListMat[10];			// マテリアルディスプレイリスト
											//	0: 屋根
											//	1: 壁・矢切
											//	2: 半透明

//											//	2: パネル
//											//	3: パネル枠
//											//	4: 部材
//											//	5: 基礎
//											//	6: 半透明

////////////////////////////////////////////////////////////////////////////
//	【機能】パネルの形状を求める
//			
void Panelfig( 
						MhPlcInfo	*i_pPlcEn,		// パネル
				const	MgPoint3	&PtCtr,
						MREAL		rB,
						MgPoint3	PT[2][2][2],	// 頂点座標
						MgVect3		*VuW,			// 巾方向
						MgVect3		*VuF,			// 手前方向
						MgVect3		*VuU			// 上方向
				)
{
	MSTNDH		iCdHgt;								// 取り付け高さコード
	MINT		iULCd;								// 上下付けコード (0:下付け, 1:上付け)
	MREAL		rZ;
	MgLine3		LnPlc;
	MgLine3		LnBz;
	MREAL		rH1, rH2, rTH, rOY;

	MgVect3		VtLng, VtRt, VtUp, VtUph;			// 部材の長さ方向ベクトル、右方向ベクトル、上方向ベクトル、パネルはみ出し部
	MgVect3		VuLng, VuRt, VuUp;					// 部材の長さ方向単位ベクトル、右方向単位ベクトル、上方向単位ベクトル
	MgVect3		VtTkH;								// 取り付け高さ
	MgVect3		VtTH, VtOY;							// 手前側補正値、奥行き
	MgVect2		vuRt;								// 屋根パネル用奥行き方向２次元ベクトル
	MREAL		rlXY;								// 屋根パネル用奥行き方向２次元ベクトルのXY平面上長さ
	
	iCdHgt = i_pPlcEn->GetPTCdHgt();
	iULCd = i_pPlcEn->GetPTCdToritk();

	rZ = MmSystemProperty::GetStnd( i_pPlcEn->GetPIKai(), iCdHgt);
	LnPlc = i_pPlcEn->GetPIPlcIti();

	VuLng = LnPlc.Vu();
	if ( MGeo::Zero( VuLng.z))
		VuRt = VuLng.RotR90();
	else {
		MREAL r1_D = 1.f / (MREAL)sqrt( VuLng.x * VuLng.x + VuLng.y * VuLng.y);
		VuRt.Set( VuLng.y * r1_D, - VuLng.x * r1_D, 0.f);
	}
	VuUp = i_pPlcEn->GetPIUpPlc();
	VuRt = VuLng ^ VuUp;
	
	VtTkH = i_pPlcEn->GetPIHgt( 0)  * VuUp;											// 取り付け高さ
	LnPlc.p[0] = LnPlc.p[0] - i_pPlcEn->GetPILenHosei( 0) * VuLng + VtTkH;
	LnPlc.p[1] = LnPlc.p[1] + i_pPlcEn->GetPILenHosei( 1) * VuLng + VtTkH;

	// 表示座標系
	LnBz = DPtoDSP( LnPlc, rZ, PtCtr, rB);
	rH1 = i_pPlcEn->GetMbHeight() * rB;
	rH2 = 15.f * rB;
	rTH = i_pPlcEn->GetPIMaeHosei() * rB;						// パネル手前側補正値
	rOY = ( i_pPlcEn->GetPIMaeHosei() + i_pPlcEn->GetPIOku() +
		   i_pPlcEn->GetPIOkuHosei()) * rB;						// パネル手前側補正値 + 奥行長 + 奥側補正値
	if ( i_pPlcEn->IsYanePanel()) {
		vuRt = MgVect2C( VuRt);
		rlXY = MGeo::Abs( vuRt);
		if ( !MGeo::Zero( rlXY)) {
			rTH /= rlXY;
			rOY /= rlXY;
		}
	}

	VtLng = LnBz.p[1] - LnBz.p[0];
	VtTH = VuRt * rTH;
	VtOY = VuRt * rOY;
	VtUp = VuUp * rH1;
	VtUph = VuUp * rH2;

	if ( iULCd == 0) 
		PT[0][0][0].Set( LnBz.p[0] + VtTH);						// 下付け材
	else
		PT[0][0][0].Set( LnBz.p[0] + VtTH - VtUp);				// 上付け材
	PT[0][0][1].Set( PT[0][0][0] + VtUp + VtUph);				//
	PT[0][1][0].Set( PT[0][0][0] - VtOY);						//
	PT[0][1][1].Set( PT[0][0][1] - VtOY);						//
	PT[1][0][0].Set( PT[0][0][0] + VtLng);						//
	PT[1][0][1].Set( PT[0][0][1] + VtLng);						//
	PT[1][1][0].Set( PT[0][1][0] + VtLng);						//
	PT[1][1][1].Set( PT[0][1][1] + VtLng);						//

//	DispList::Rectangular( PT, VuLng, VuRt, VuUp);
	*VuW = VuLng;
	*VuF = VuRt;
	*VuU = VuUp;
}

////////////////////////////////////////////////////////////////////////////
//	【機能】パネルのディスプレイリストを作成する
//			
void MdlDispList::DrawPanel( 
						MhPlcInfo*	i_pPlcEn,	// パネル配置情報
				const	MgPoint3	&i_PtCtr,	//
						MREAL		i_rB,		//
						MINT		i_iMode		// 表示モード 1:パネル上部線分 2:パネル形状直方体 3:線分+パネル形状
				)
{

	MgPoint3	PT[2][2][2];					// 頂点座標
	MgVect2		vuRt;							// 屋根パネル用奥行き方向２次元ベクトル
	MgVect3		VuW, VuF, VuU;					// 部材の長さ方向単位ベクトル、右方向単位ベクトル、上方向単位ベクトル

	Panelfig( i_pPlcEn, i_PtCtr, i_rB, PT, &VuW, &VuF, &VuU);
	if ( i_iMode & MP_PANEL_RECTANGULAR) {
		DispList::SetMaterial( 0, 1);
		DispList::DspRectangular( PT, VuF, VuW, VuU);
		DispList::SetMaterial( 0, 5);
//		DispList::Quads( MgLine3( PT[0][1]), MgLine3( PT[0][0]));			
//		DispList::Quads( MgLine3( PT[1][1]), MgLine3( PT[0][1]));			
//		DispList::Quads( MgLine3( PT[1][0]), MgLine3( PT[1][1]));			
//		DispList::Quads( MgLine3( PT[0][0]), MgLine3( PT[1][0]));
	}
	if ( i_iMode & MP_PANEL_LINE) {
//		glColor3f(1.0,0.,0.);
		DispList::SetMaterial( 0, 5);
		glBegin(GL_LINE_STRIP);
			glVertex3r( PT[0][1][1].x, PT[0][1][1].y, PT[0][1][1].z);			
			glVertex3r( PT[1][1][1].x, PT[1][1][1].y, PT[1][1][1].z);			
			glVertex3r( PT[1][0][1].x, PT[1][0][1].y, PT[1][0][1].z);			
			glVertex3r( PT[0][0][1].x, PT[0][0][1].y, PT[0][0][1].z);	
			glVertex3r( PT[0][1][1].x, PT[0][1][1].y, PT[0][1][1].z);	
		glEnd();
	}
}


////////////////////////////////////////////////////////////////////////////
//	ディスプレイリストを作成する
MINT MdlDispList::OpenDL( 
						MINT		inmDispList		// DisplayList番号
				)
{
	// make display list number
	if ( ( z_DispListFig[inmDispList] = glGenLists( 1)) == 0)
		return 1;

	// ディスプレイリストを作成する
	glNewList( z_DispListFig[inmDispList], GL_COMPILE);

//	glPushMatrix();
	return 0;
}

////////////////////////////////////////////////////////////////////////////
//	ディスプレイリストの作成を終了する
void MdlDispList::CloseDL()
{
//	glPopMatrix();
	glEndList();
}

////////////////////////////////////////////////////////////////////////////
//	構造家モデルの３次元ディスプレイリストを作成する	屋根、半透明
//
MINT MdlDispList::MakeDispListMat0(
						MREAL		rB,				// 表示倍率
				const	MgPoint3	&PtCtr			// 中心座標
				)
{
	if ( ( z_DispListMat[0] = glGenLists( 1)) == 0)
		return 1;
	// ディスプレイリストを作成する
	glNewList( z_DispListMat[0], GL_COMPILE);

	DispList::SetMaterial( 0, 0);									// 屋根

	MdlDispList::CloseDL();
	return 0;
}

////////////////////////////////////////////////////////////////////////////
//	構造家モデルの３次元ディスプレイリストを作成する	壁、矢切　
//
MINT MdlDispList::MakeDispListMat1(
						MREAL		rB,				// 表示倍率
				const	MgPoint3	&PtCtr			// 中心座標
				)
{
	if ( ( z_DispListMat[1] = glGenLists( 1)) == 0)
		return 1;
	// ディスプレイリストを作成する
	glNewList( z_DispListMat[1], GL_COMPILE);

	DispList::SetMaterial( 0, 1);									// 壁、矢切

	MdlDispList::CloseDL();
	return 0;
}

////////////////////////////////////////////////////////////////////////////
//	構造家モデルの３次元ディスプレイリストを作成する	壁、矢切、屋根　半透明
//
MINT MdlDispList::MakeDispListMat2(
						MREAL		rB,				// 表示倍率
				const	MgPoint3	&PtCtr			// 中心座標
				)
{
	if ( ( z_DispListMat[2] = glGenLists( 1)) == 0)
		return 1;
	// ディスプレイリストを作成する
	glNewList( z_DispListMat[2], GL_COMPILE);

	DispList::SetMaterial( 1, 0);									// 屋根、壁　半透明

	MdlDispList::CloseDL();
	return 0;
}

////////////////////////////////////////////////////////////////////////////
//【機能】面表示　または　ワイヤフレーム表示 選択
//
void MsViewSWMode(
						MINT		imdWS			// 表示選択		1: 面表示、2: ワイヤフレーム表示
				)
{
	switch ( imdWS)
	{
	case 1:														// 面表示
		// SetColor for Wireframe mode
		glEnable(GL_BLEND);

		glEnable( GL_LIGHTING);
		glEnable( GL_DEPTH_TEST);								// 前後関係を明確にする為にデプステストを有効にする
//		glEnable( GL_NORMALIZE);								// glScalefを使う場合は法線の長さも変化するので
																// GL_NORMALIZEのEnableで単位ベクトル化する
		glDisable( GL_NORMALIZE);

		// Front face Fill
		glPolygonMode( GL_FRONT, GL_FILL);

		//// Back face Line (Only wireframe mode)
		// glPolygonMode( GL_BACK, GL_LINE);

		// BackFace Culling
		glCullFace( GL_BACK);

		// Culling mode set
		glEnable( GL_CULL_FACE);

		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		break;

	case 11:													// 面表示
		// SetColor for Wireframe mode
//		glEnable(GL_BLEND);

		glEnable( GL_LIGHTING);
		glEnable( GL_DEPTH_TEST);								// 前後関係を明確にする為にデプステストを有効にする
//		glEnable( GL_NORMALIZE);								// glScalefを使う場合は法線の長さも変化するので
//																// GL_NORMALIZEのEnableで単位ベクトル化する
		glDisable( GL_NORMALIZE);
																// 面を塗り潰しで描く
		// Front face Fill
		glPolygonMode( GL_FRONT, GL_FILL);

		// BackFace Culling
		glCullFace( GL_BACK);

//		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);

		// Culling mode set
		glEnable( GL_CULL_FACE);

		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		break;

	case 12:													// ポリゴンオフセット機能を有効にして
																// 面の境界線を描くモードにして再度面を描画する。
////U		glColor4f( 0., 0., 0., 0.);
////
////		glDisable( GL_LIGHTING);
////		glDisable( GL_DEPTH_TEST);
////		glDisable( GL_NORMALIZE);

		glEnable( GL_POLYGON_OFFSET_LINE);
		glPolygonOffset( 1.0, 1.0);
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
//		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
//		glCullFace( GL_BACK);
//		glEnable( GL_CULL_FACE);
		break;

	case 2:														// ワイヤーフレーム
		// SetColor for Wireframe mode
		glColor4f( 1., 1., 1., 1.);

		glDisable( GL_LIGHTING);
		glDisable( GL_DEPTH_TEST);
		glDisable( GL_NORMALIZE);

		// Front Line
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);

		// Culling mode set
		glDisable( GL_CULL_FACE);
		break;

	case 3:														// ワイヤーフレーム
		// SetColor for Wireframe mode
		glColor4f( 1., 1., 1., 1.);

		glDisable( GL_LIGHTING);
		glDisable( GL_DEPTH_TEST);
		glDisable( GL_NORMALIZE);

		// Polygon Offset Mode
		glEnable( GL_POLYGON_OFFSET_LINE);
//		glPolygonOffset( 1.0, 1.0);
		glPolygonOffset( 5.0, 5.0);

		// Front Line
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);

		// Culling mode set
		glDisable( GL_CULL_FACE);
		break;
	}
}

GLint MsDrawPlane( GLdouble w, GLdouble d ,GLdouble h);		//ADD
GLint MsDrawBox(GLdouble d, GLdouble w, GLdouble h);		//ADD

//N #define __ORIGINAL__
////////////////////////////////////////////////////////////////////////////
//【機能】ワイヤフレーム表示　または　面表示する
//
void MsDrawDisplayList(
						int			iFWire		// 表示タイプ　0: 面表示、1:半透明、2:部材のみ、3:ワイヤフレーム表示
				)
{
//	MsDrawBox(2.0, 3.0, 1.0);			//ADD
//	MsDrawPlane( 5.0, 5.0 , 0.0);		//ADD

	switch( iFWire)
	{
	case 0:
#ifdef __ORIGINAL__		
		MsViewSWMode( 1);										// 面表示

	glEnable(GL_DEPTH_TEST);									// 前後テスト
	glBlendFunc(GL_ONE, GL_ZERO);								// 実態表示設定(前1.0,後0.0: 後の図形を非表示)　

		glCallList( z_DispListFig[1]);							// マテリアル付き　部材+基礎

		glCallList( z_DispListMat[1]);							// マテリアル(壁+矢切)	実体
		glCallList( z_DispListFig[2]);							// 壁+矢切

		glCallList( z_DispListMat[0]);							// マテリアル(屋根)		実体
		glCallList( z_DispListFig[3]);							// 屋根
		break;
		
#else
		MsViewSWMode( 11);										// 面表示
 
		glCallList( z_DispListFig[1]);							// マテリアル付き　部材+基礎

		MsViewSWMode( 12);										// 面表示

		glCallList( z_DispListFig[1]);							// マテリアル付き　部材+基礎

		break;
#endif
	case 1:
		MsViewSWMode( 1);										// 面表示

	glEnable(GL_DEPTH_TEST);									// 前後テスト
	glBlendFunc(GL_ONE, GL_ZERO);								// 実態表示設定(前1.0,後0.0: 後の図形を非表示)　

		glCallList( z_DispListFig[1]);
		glCallList( z_DispListMat[2]);

		glDepthMask(GL_FALSE);									// 屋根、壁　半透明
		glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);					// 半透明設定

		glCallList( z_DispListFig[2]);							// 壁+矢切　半透明
		glCallList( z_DispListFig[3]);							// 屋根		半透明
		break;
		
	case 2:
		MsViewSWMode( 1);										// 面表示

	glEnable(GL_DEPTH_TEST);									// 前後テスト
	glBlendFunc(GL_ONE, GL_ZERO);								// 実態表示設定(前1.0,後0.0: 後の図形を非表示)　

//DEL		glCallList( z_DispListFig[1]);							// マテリアル付き　部材+基礎
		break;
		
	case 3:
		MsViewSWMode( 2);										// ワイヤフレーム表示

	glEnable(GL_DEPTH_TEST);									// 前後テスト
	glBlendFunc(GL_ONE, GL_ZERO);								// 実態表示設定(前1.0,後0.0: 後の図形を非表示)　

		glCallList( z_DispListFig[1]);							// 部材+基礎
		glCallList( z_DispListFig[2]);							// 壁+屋根
		glCallList( z_DispListFig[3]);							// 屋根
//		glCallList( z_DispListFig[1]);							// 部材+基礎
		break;

	case 4:
		MsViewSWMode( 1);										// 面表示

	glEnable(GL_DEPTH_TEST);									// 前後テスト
	glBlendFunc(GL_ONE, GL_ZERO);								// 実態表示設定(前1.0,後0.0: 後の図形を非表示)　

		glCallList( z_DispListFig[1]);							// マテリアル付き　部材+基礎

		glCallList( z_DispListMat[1]);							// マテリアル(壁+矢切)	実体
		glCallList( z_DispListFig[2]);							// 壁+矢切

		glCallList( z_DispListMat[0]);							// マテリアル(屋根)		実体
		glCallList( z_DispListFig[3]);							// 屋根

		MsViewSWMode( 3);										// オフセットワイヤーフレーム表示

	glEnable(GL_DEPTH_TEST);									// 前後テスト
	glBlendFunc(GL_ONE, GL_ZERO);								// 実態表示設定(前1.0,後0.0: 後の図形を非表示)　

		glCallList( z_DispListFig[1]);							// マテリアル付き　部材+基礎
	}

	glFlush();
	glDepthMask(GL_TRUE);										// 半透明テスト

	//glDisable(GL_LIGHTING);									// 半透明テスト		NU?
	//glDisable(GL_DEPTH_TEST);									// 半透明テスト		NU?
	//glDisable(GL_BLEND);										// 半透明テスト		NU?

//	auxSwapBuffers();
//DEL	glutSwapBuffers();

}



GLint MsDrawPlane( GLdouble w, GLdouble d ,GLdouble h)
{
	GLdouble norm[3];
	glPushMatrix();
	glBegin( GL_POLYGON );
		norm[0]=1;norm[1]=0;norm[2]=0;
		glNormal3dv (norm);
		glVertex3d( w/2.0 , h ,   d/2.0 );
		glVertex3d( w/2.0 , h ,  -d/2.0  );
		glVertex3d(-w/2.0 , h ,  -d/2.0  );
		glVertex3d(-w/2.0 , h ,   d/2.0  );
	glEnd();
	glPopMatrix();

	return(0) ;
}

GLint MsDrawBox(GLdouble d, GLdouble w, GLdouble h)
{
	GLdouble norm[3];
	
	glPushMatrix();
	glTranslated(-d/2.0,-w/2.0,-h/2.0);
	glBegin( GL_POLYGON );
		norm[0]=0;norm[1]=-1;norm[2]=0;
		glNormal3dv (norm);
		glVertex3d( 0 , 0 , 0 );
		glVertex3d( d , 0 , 0 );
		glVertex3d( d , 0 , h );
		glVertex3d( 0 , 0 , h );
	glEnd();
	glBegin( GL_POLYGON );
		norm[0]=0;norm[1]=-1;norm[2]=0;
		glNormal3dv (norm);
		glVertex3d( 0 , w , 0 );
		glVertex3d( d , w , 0 );
		glVertex3d( d , w , h );
		glVertex3d( 0 , w , h );
	glEnd();

	glBegin( GL_POLYGON );
		norm[0]=0;norm[1]=0;norm[2]=1;
		glNormal3dv (norm);
		glVertex3d( 0 , 0 , h );
		glVertex3d( d , 0 , h );
		glVertex3d( d , w , h );
		glVertex3d( 0 , w , h );
	glEnd();
	glBegin( GL_POLYGON );
		norm[0]=0;norm[1]=0;norm[2]=1;
		glNormal3dv (norm);
		glVertex3d( 0 , 0 , 0 );
		glVertex3d( d , 0 , 0 );
		glVertex3d( d , w , 0 );
		glVertex3d( 0 , w , 0 );
	glEnd();

	glBegin( GL_POLYGON );
		norm[0]=-1;norm[1]=0;norm[2]=0;
		glNormal3dv (norm);
		glVertex3d( 0 , 0 , 0 );
		glVertex3d( 0 , 0 , h );
		glVertex3d( 0 , w , h );
		glVertex3d( 0 , w , 0 );
	glEnd();
	glBegin( GL_POLYGON );
		norm[0]=-1;norm[1]=0;norm[2]=0;
		glNormal3dv (norm);
		glVertex3d( d , 0 , 0 );
		glVertex3d( d , 0 , h );
		glVertex3d( d , w , h );
		glVertex3d( d , w , 0 );
	glEnd();
	glPopMatrix();


	return(0) ;

}

////////////////////////////////////////////////////////////////////////////
//【機能】描画
BOOL MsRenderScene1(
						int			i_iFWire,	// 表示タイプ　0: 面表示、1:半透明、2:部材のみ、3:ワイヤフレーム表示
						MREAL		i_rScl,		// 拡大率
						MREAL		i_rRotX,	// 回転角	V3( ｘ，ｙ，ｚ)
						MREAL		i_rRotY,	// 回転角	V3( ｘ，ｙ，ｚ)
						MREAL		i_rRotZ,	// 回転角	V3( ｘ，ｙ，ｚ)
						MREAL		i_rMovX,	// 移動量	V3( ｘ，ｙ，ｚ)
						MREAL		i_rMovY,	// 移動量	V3( ｘ，ｙ，ｚ)
						MREAL		i_rMovZ		// 移動量	V3( ｘ，ｙ，ｚ)
				)
{
	// 背景色の消去
//Org	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT| GL_STENCIL_BUFFER_BIT) ;

	//======
		//オブジェクト描画モードにする。
	glMatrixMode(GL_MODELVIEW) ;
	glLoadIdentity() ;
	//////////////////////////////////////////////
	//必要な機能の有効化
	//
	glEnable(GL_NORMALIZE) ;
	glEnable(GL_AUTO_NORMAL) ;
	glShadeModel (GL_SMOOTH);
	glEnable( GL_DEPTH_TEST ); //Zバッファを使用する
	//----

	// マトリックスのスタック
	glPushMatrix();


	// 11/10/25
	gluLookAt(0, 2.0, -5.0,   0,0,0,    0,1,0);


	// 回転 X→Y→Zの順に回転
    glRotatef( i_rRotZ, 0.0f, 0.0f, 1.0f );
    glRotatef( i_rRotY, 0.0f, 1.0f, 0.0f );
	glRotatef( i_rRotX, 1.0f, 0.0f, 0.0f );

	// 拡大縮小
	glScalef( i_rScl, i_rScl, i_rScl); 

	// 移動
	glTranslatef( i_rMovX, i_rMovY, i_rMovZ);

#if(1)
	// ディスプレイリスト描画
	MsDrawDisplayList( i_iFWire);

	// マトリックスのスタック
    glPopMatrix();
#else

	MsDrawBox(2.0, 3.0, 1.0);
	MsDrawPlane( 5.0, 5.0 , 0.0);

	//座標系を戻しておく
	glPopMatrix();

#endif
//===
	glDisable(GL_AUTO_NORMAL) ;
	glDisable(GL_NORMALIZE) ;
//---
	// ダブルバッファ切り替え
    glFinish();
    return TRUE;
}

} // namespace MC

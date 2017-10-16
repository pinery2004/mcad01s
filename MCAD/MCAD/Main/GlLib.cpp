#include	"StdAfx.h"
#include	"MCAD.h"
#include	< GL/gl.h >
#include	< GL/glu.h >
#include	"GlLib.h"
// OpenGL

GLint GlLib::InitializeOpenGL(void)
{
	//3Dシーンの背景を黒にする。
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f) ;	

	glDepthFunc(GL_LEQUAL) ;
	//デプスバッファを有効化
	glEnable(GL_DEPTH_TEST) ;

	//Lightingを有効化
	glEnable(GL_LIGHTING) ; 
	glEnable(GL_LIGHT0) ;
	glEnable(GL_LIGHT1) ;

	return (0) ;
}

int GlLib::MakeGLObject()
{
	//ここに事前に作成しておくオブジェクトの記述を加える
	//

	return (0) ;
}
#if(0)
void GlLib::DrawGLobject()
{
	//シーンをクリアする。色情報、デプスバッファをクリア
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT| GL_STENCIL_BUFFER_BIT) ;
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

	// マトリックスのスタック
	glPushMatrix();

	//ここで呼び出すと、カメラ座標上に光源が置かれる。
//	Lighting();

	//gluLookAt( 視点の位置x,y,z,  視界の中心位置の参照点座標x,y,z, 視界の上方向のベクトルx,y,z);
	// 以下のように呼び出すと,視点(0,2,-5)，原点方向を見て，そのときy軸が上になるように眺める
	gluLookAt(0, 2.0, -5.0,   0,0,0,    0,1,0);

	Lighting();

	//照明を点灯する。
	glEnable(GL_LIGHTING) ; 
	glEnable(GL_LIGHT0) ;

	GLfloat light0_position[] = {1.0, 5.0, -3.0, 1.0} ;
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	DrawBox(2.0, 3.0, 1.0);
	DrawPlane( 5.0, 5.0 , 0.0);

	//座標系を戻しておく
	glPopMatrix();

	glDisable(GL_AUTO_NORMAL) ;
	glDisable(GL_NORMALIZE) ;
	return;
}
#endif

#if(1)
void GlLib::DrawGLobject1(
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
	//シーンをクリアする。色情報、デプスバッファをクリア
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT| GL_STENCIL_BUFFER_BIT) ;
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

	// マトリックスのスタック
	glPushMatrix();

	//ここで呼び出すと、カメラ座標上に光源が置かれる。
//	Lighting();

	//gluLookAt( 視点の位置x,y,z,  視界の中心位置の参照点座標x,y,z, 視界の上方向のベクトルx,y,z);
	// 以下のように呼び出すと,視点(0,2,-5)，原点方向を見て，そのときy軸が上になるように眺める
	gluLookAt(0, 2.0, -5.0,   0,0,0,    0,1,0);

	Lighting();

	//照明を点灯する。
	glEnable(GL_LIGHTING) ; 
	glEnable(GL_LIGHT0) ;

	GLfloat light0_position[] = {1.0, 5.0, -3.0, 1.0} ;
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
#if(1)
	// 回転 X→Y→Zの順に回転
    glRotatef( i_rRotZ, 0.0f, 0.0f, 1.0f );
    glRotatef( i_rRotY, 0.0f, 1.0f, 0.0f );
	glRotatef( i_rRotX, 1.0f, 0.0f, 0.0f );

	// 拡大縮小
	glScalef( i_rScl, i_rScl, i_rScl); 

	// 移動
	glTranslatef( i_rMovX, i_rMovY, i_rMovZ);
#endif

	DrawBox(2.0, 3.0, 1.0);
	DrawPlane( 5.0, 5.0 , 0.0);

	//座標系を戻しておく
	glPopMatrix();

	glDisable(GL_AUTO_NORMAL) ;
	glDisable(GL_NORMALIZE) ;
	return;
}
#endif 

int GlLib::SetDCPixelFormat( HDC hdc)
{

    static PIXELFORMATDESCRIPTOR pfd = {
        sizeof (PIXELFORMATDESCRIPTOR), 		// Specifies the size of this data structure
        1,                                          			// Specifies the version of this data structure
        PFD_DRAW_TO_WINDOW |					// ピクセルバッファのビットフラグの設定
        PFD_SUPPORT_OPENGL |
        PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,                        // RGBA pixel values
        32,                                   // 32-bitカラーと指定
        0, 0, 0, 0, 0, 0,                     // Specifies the number of red bitplanes in each RGBA color buffer
        0, 0,                                 // Specifies the number of alpha bitplanes in each RGBA color buffer
        0, 0, 0, 0, 0,                        // Specifies the total number of bitplanes in the accumulation buffer
        32,                                   // Specifies the depth(bit) of the depth (z-axis) buffer
        32,                                   // Specifies the depth of the stencil buffer
        0,                                    // Specifies the number of auxiliary buffers
        PFD_MAIN_PLANE,                       // Layer type　Ignored...
        0,                                    // Specifies the number of overlay and underlay planes
        0,                                    // Ignored
		0,                                    // Specifies the transparent color or index of an underlay plane
        0                                     // Ignored
    };

    int nPixelFormat;
    
    nPixelFormat = ChoosePixelFormat (hdc, &pfd);
    if (SetPixelFormat(hdc, nPixelFormat, &pfd) == FALSE) {
      return FALSE ;
    }

    if (DescribePixelFormat(hdc, nPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd) == 0) {
      return FALSE ;
    }

    return TRUE ;
}

void GlLib::ViewSetting(GLint w,GLint h)
{
	glMatrixMode(GL_PROJECTION) ;
	glLoadIdentity() ;
	// gluPerspective( 視野角度[deg],  アスペクト比(ウィンドウの幅/高さ), 
	//		描画する距離範囲（視点から最も近い点までの距離）, 視点から最も遠い点までの距離);
	gluPerspective(60, (GLfloat)w/(GLfloat)h, 0.5, 15.0) ;
	// glViewport( 左下のピクセル座標X, 左下のピクセル座標Y, ウィンドウ幅，高さ );
	glViewport (0, 0, w , h) ;
	glMatrixMode(GL_MODELVIEW) ;
	glLoadIdentity ();
	return;
}

void GlLib::Lighting()
{
	//照光処理を有効化
	glEnable(GL_LIGHTING);
	//両面照光モードに設定する
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE );
	//光源に関係しない環境光を設定
	GLfloat model_ambient[]={0.4f,0.4f,0.4f,1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);
	//光源の環境光(ambient),拡散光(diffuse),鏡面光(specular)を設定
	GLfloat light0_ambient[]={0.5f,0.5f,0.5f,1.0f};
	GLfloat light0_diffuse[]={1.0f,1.0f,1.0f,1.0f};
	GLfloat light0_specular[]={1.0f,1.0f,1.0f,1.0f};
	glLightfv(GL_LIGHT0, GL_AMBIENT,  light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	//光源の位置を設定する．
	GLfloat light0_position[]={0.0f,0.0f,1.0f,1.0f};
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	//ライト0を有効化
	glEnable(GL_LIGHT0);
}

GLint GlLib::DrawPlane( GLdouble w, GLdouble d ,GLdouble h)
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

GLint GlLib::DrawBox(GLdouble d, GLdouble w, GLdouble h)
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

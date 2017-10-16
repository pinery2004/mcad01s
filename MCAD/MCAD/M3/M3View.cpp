////////////////////////////////////////////////////////////////////////////
// ChildView2.cpp : 実装ファイル
//

#include "stdafx.h"
#include "MCAD.h"
#include "ChildView2.h"
#include "MmWnd.h"


#include	< GL/gl.h >
#include	< GL/glu.h >
#include	"GlLib.h"

#include "MgLib.h"
#include "MnLib.h"
#include "MdDispList.h"
#define DLL_EXPORT_DISPLIST_DO
#include "MnMdlDispList.h"


namespace MC
{

//S void MhGetBCtr(	MREAL *rB, MgPoint3 *PtCtr);
//S int DispList::DrawIeMdl( MREAL rB, const MgPoint3 &PtCtr);

void ModelDraw( void);
//void Cube( GLfloat lenght);

//void MsSetLight_MaterialParameter1();

void SetLightModel1();
void SetLight();
void DrawDisplayList( int iWire);
//void SetMaterial1();
//void SetMaterial2();


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
				);

// CChildView2

static const MCHAR* ErrorStrings[] = {
    {Mstr( "No Error")},                   // 0
    {Mstr( "Unable to get a CD")},         // 1
    {Mstr( "ChoosePixelFormat failed")},   // 2
    {Mstr( "SelectPixelFormat failed")},   // 3
    {Mstr( "wglCreateContext failed")},    // 4
    {Mstr( "wglCreateCurrect failed")},    // 5
    {Mstr( "wglDeleteCotext failed")},     // 6
    {Mstr( "SwapBuffer failed")},          // 7
};

//void CChildView2::ShowError(int e){
void ShowError(int e){
    AfxMessageBox(ErrorStrings[e]);
}

// CChildView2 メッセージ ハンドラ

M3View::M3View() : m_hRC(0)
{
}
void M3View::OnPaint()
{
#if(Disp3DBox)		// 11/10/
	GlLib::DrawGLobject();
	GlLib::DrawGLobject1(m_iFWire, m_rScl, m_rRotX, m_rRotY, m_rRotZ,
		m_rMovX, m_rMovY, m_rMovZ);
#else
	MsRenderScene1(m_iFWire, m_rScl, m_rRotX, m_rRotY, m_rRotZ,
		m_rMovX, m_rMovY, m_rMovZ);
#endif
}

////////////////////////////////////////////////////////////////////////////

int M3View::OnCreate( HDC hdc)
{
	MREAL		rB;								// 表示倍率
	MgPoint3	PtCtr;							// 中心座標


	//	OpenGL用にPixcel Formatを指定
    if( !SetDCPixelFormat( hdc) ) return FALSE;

     //	与えられたDCと両立可能なRendering contextの生成 
	if( 0 == (m_hRC = wglCreateContext( hdc))){
        ShowError(4);
        return FALSE;
    }

    // Rendering contextを現在のcontextとして設定
	if( FALSE == wglMakeCurrent( hdc, m_hRC)){
        ShowError(5);
        return FALSE;
    }
    
    // 3Dシーンを初期化
    InitializeOpenGL();
    
	// 3Dオブジェクトを生成(Displaylist作成)
	MdlDispList::MhGetBCtr( &rB, &PtCtr);									// ３次元ディスプレイリスト作成用の表示倍率と中心座標を求める
	
	MdlDispList::DrawIeMdlK( rB, PtCtr);

	return 0;
}

//BOOL CChildView2::SetDCPixelFormat( HDC hdc)
BOOL M3View::SetDCPixelFormat( HDC hdc)
{
	//　ウィンドウのピクセルフォーマットの設定 
    PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),							// Specifies the size of this data structure
		1,														// Specifies the version of this data structure
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,		// ピクセルバッファのビットフラグの設定
		PFD_TYPE_RGBA,											// RGBA pixel values
		24,														// (or 32) 24-bitカラーと指定
		0, 0, 0, 0, 0, 0,										// Specifies the number of red bitplanes in each RGBA color buffer
		0,														// Specifies the number of alpha bitplanes in each RGBA color buffer
		0,
		0,														// Specifies the total number of bitplanes in the accumulation buffer
		0,  0, 0, 0,
		32,														// Specifies the depth(bit) of the depth (z-axis) buffer
		0,														// (or 32) Specifies the depth of the stencil buffer
		0,														// Specifies the number of auxiliary buffers
		PFD_MAIN_PLANE,											// Layer type　Ignored...
		0,														// Specifies the number of overlay and underlay planes
		0, 0, 0													// Ignored, Specifies the transparent color or index of an underlay plane, Ignored

    };
	//要求されたフォーマットに最も近いピクセルフォーマットのインデックスを要求 
	int pixelformat;
	if( 0 == (pixelformat = ChoosePixelFormat( hdc, &pfd))){
		ShowError(2);
		return FALSE;
	}
	//ピクセルフォーマットの設定 
	if( FALSE == SetPixelFormat( hdc, pixelformat, &pfd)){
		ShowError(3);
		return FALSE;
	}
//	if (0 == DescribePixelFormat(hdc, nPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd)) {
//		ShowError(3);
//		return FALSE ;
//	}
	return TRUE;
}

void MsGlInitialize();

// OnCreate から最初に実行される　イニシャライズ処理
BOOL M3View::InitializeOpenGL( void)
{
	// 面表示に設定
	m_iFWire = 0;

	// バッファ初期化
	DispList::MsGlInitialize();

	// Lightingを有効化
	
	// Light model,Light,Material パラメータの作成1	
	DispList::MsSetLight_MaterialParameter1();									

	// 回転角を初期設定
	m_rRotX = -50.;
	m_rRotY = -32.;
	m_rRotZ = -22.;

	// 移動量を初期設定
	m_rMovX = 0.;
	m_rMovY = 0.;
	m_rMovZ = 0.;

	// 座標変換マトリックスの初期化
	m_rScl = 1.;
	m_mTrans = MGeo::Mat3RotRevZ( m_rRotZ) * MGeo::Mat3RotRevY( m_rRotY) * MGeo::Mat3RotRevX( m_rRotX);

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////

void M3View::OnDestroy()
{
	// レンダリング コンテキストとデバイス コンテキストの解放
	if( FALSE == ::wglMakeCurrent( 0, 0)) ShowError(2);
	if( FALSE == ::wglDeleteContext( m_hRC )) ShowError(6);
}

//void MsOnSize(UINT nType, int cx, int cy);

void M3View::OnSize(UINT nType, int cx, int cy)
{
	DispList::MsOnSize( nType, cx, cy);
}

void M3View::OnMouseMove(
						MINT		nFlags,		// 1: 移動
						MgPoint3	PtD,
						MgPoint3	RtD,
						MREAL		SclD		// 拡大縮小量
				)
{
	// TODO : ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	CPoint	iPtD;												// マウス移動量	
	MgVect3	rdMov1;
	MgVect3	rdMov2;
	MgMat3	mTr1, mTr2, mTr3, mTrW, mTrW1, mTrW2;				// 座標変換マトリックス (work)
	MgVect2	vRx, vRy, vRz;										// Ｘ軸回転角, Ｙ軸回転角, Ｚ軸回転角

	if ( nFlags & MK_CONTROL) {
		rdMov1 = 2.67f * PtD;

		rdMov2 = rdMov1 / m_rScl * m_mTrans;

		m_rMovX += rdMov2.x;
		m_rMovY += rdMov2.y;
		m_rMovZ += rdMov2.z;
		
	} else if ( nFlags & MK_LBUTTON) {
		vRx = MgVect2( 1., - RtD.y * 3).Unitize();				// Ｘ軸回転角　(正方向:	Ｘ軸に対して左回転)
		mTr1 = MGeo::Mat3RotRevXV( vRx);							//					   (Ｘ軸から見て右回転)
		mTrW1 = mTr1 * m_mTrans;

		vRy = MgVect2( 1., RtD.x * 3).Unitize();				// Ｙ軸回転角　(正方向:	Ｙ軸に対して右回転)
		mTr2 = MGeo::Mat3RotRevYV( vRy);							//					   (Ｙ軸から見て左回転)
		mTrW2 = mTr2 * mTrW1;

		vRz = MgVect2( 1., RtD.z * 3).Unitize();				// 下は (正方向:Ｚ軸に対して右回転)
		mTr3 = MGeo::Mat3RotRevZV( vRz);							//			   (Ｚ軸から見て左回転)
		mTrW = mTr3 * mTrW2;
		m_mTrans = mTrW;

	} else if ( nFlags & MK_RBUTTON) {
		m_rScl *= (1 + SclD);									// 拡大縮小
																// Ｚ軸回転角　画面中央より
																// 上は (正方向:Ｚ軸に対して左回転) で、
																//			   (Ｚ軸から見て右回転)
		vRz = MgVect2( 1., RtD.z * 3).Unitize();				// 下は (正方向:Ｚ軸に対して右回転)
																//			   (Ｚ軸から見て左回転)
		mTr1 = MGeo::Mat3RotRevZV( vRz);
		mTrW = mTr1 * m_mTrans;
		m_mTrans = mTrW;
	}
	m_rRotX = - (MREAL)atan2( -m_mTrans.m[2][1], m_mTrans.m[2][2]) * MC_RTD;
	m_rRotY = - (MREAL)asin( m_mTrans.m[2][0]) * MC_RTD;	
	m_rRotZ = - (MREAL)atan2( -m_mTrans.m[1][0], m_mTrans.m[0][0]) * MC_RTD;
}

} // namespace MC

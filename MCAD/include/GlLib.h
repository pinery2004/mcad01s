// OpenGL 使用 関数
#pragma once

// OpenGL
class GlLib{
public:
	static GLint InitializeOpenGL( void);

	static int MakeGLObject( void);

	static void DrawGLobject( void);
	static void GlLib::DrawGLobject1(
						int			i_iFWire,	// 表示タイプ　0: 面表示、1:半透明、2:部材のみ、3:ワイヤフレーム表示
						MREAL		i_rScl,		// 拡大率
						MREAL		i_rRotX,	// 回転角	V3( ｘ，ｙ，ｚ)
						MREAL		i_rRotY,	// 回転角	V3( ｘ，ｙ，ｚ)
						MREAL		i_rRotZ,	// 回転角	V3( ｘ，ｙ，ｚ)
						MREAL		i_rMovX,	// 移動量	V3( ｘ，ｙ，ｚ)
						MREAL		i_rMovY,	// 移動量	V3( ｘ，ｙ，ｚ)
						MREAL		i_rMovZ		// 移動量	V3( ｘ，ｙ，ｚ)
	);
	static int SetDCPixelFormat( HDC hdc);

	static void ViewSetting(GLint w,GLint h);

	static void Lighting();

	static GLint DrawPlane( GLdouble w, GLdouble d ,GLdouble h);

	static GLint DrawBox(GLdouble d, GLdouble w, GLdouble h);

};
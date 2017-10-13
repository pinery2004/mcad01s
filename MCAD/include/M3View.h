#pragma once

#include "MgMat.h"

namespace MC
{

class	M3View
{
public:
    HGLRC	m_hRC;

	MINT	m_iFWire;											// ワイヤハーネス表示フラグ
																//　(1: ワイヤハーネス表示 0:面表示)

	MREAL	m_rRotX;											// Ｘ軸回転角
	MREAL	m_rRotY;											// Ｙ軸回転角
	MREAL	m_rRotZ;											// Ｚ軸回転角

	MREAL	m_rMovX;											// Ｘ軸方向移動量
	MREAL	m_rMovY;											// Ｙ軸方向移動量
	MREAL	m_rMovZ;											// Ｚ軸方向移動量

	MREAL	m_rScl;												// 座標変換スケール
	MgMat3	m_mTrans;											// 座標変換マトリックス

	M3View();
	int OnCreate( HDC hdc);
	void OnDestroy();
	void OnPaint();
	void OnSize( UINT nType, int cx, int cy);
	void OnMouseMove( MINT nFlags, MgPoint3	PtDnt, MgPoint3	RtDnt, MREAL SclD);
	BOOL RenderScene( void );
    BOOL SetDCPixelFormat( HDC hdc);
    BOOL InitializeOpenGL( void);

};

//【機能】描画
//BOOL RenderScene( int i_iFWire,	MREAL i_rScl, MgVect3& i_VRot, MgVect3& i_VMov);
BOOL MsRenderScene1( MINT i_iFWire, MREAL i_rScl,
				  MREAL i_rRotX, MREAL i_rRotY, MREAL i_rRotZ,
				  MREAL i_rMovX, MREAL i_rMovY, MREAL i_rMovZ);

} // namespace MC
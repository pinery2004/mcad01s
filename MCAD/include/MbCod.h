#pragma once
//==========================================================================================
//  Copyright ( C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MbCod.h
//
//		座標系 座標変換、図形、文字表示
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
//
#include "MgMat.h"
#include "MgGeo.h"
#include "MbDefine.h"

#define		MT_SZMINMAXRS	2					// 実座標( Min/Max)スタックサイズ
#define		SZFONT			32					// フォント最大文字数

#define		MT_LEFT			1
#define		MT_CENTER		2
#define		MT_RIGHT		3
#define		MT_UPPER		1
#define		MT_LOWER		3

#define		MT_FIXEDSIZE	TRUE
#define		MT_FREESIZE		FALSE

#ifdef DLL_EXPORT
	#undef DLL_EXPORT
#endif

#ifdef DLL_EXPORT_COD_DO
	#define DLL_EXPORT							__declspec( dllexport)
#else
//	#define DLL_EXPORT							__declspec( dllimport)
	#define DLL_EXPORT
#endif

namespace MC
{

/////////////////////////////////////////////////////////////////////////////
//	座標系 座標変換、図形、文字表示
//
class DLL_EXPORT MbCod
{
protected:
	MgMinMaxI2		m_iMinMaxD;					// 表示座標( Min/Max)
	MgMinMaxI2		m_iMinMaxL;					// 論理座標( Min/Max)
												// ***** 注意 *****
												// 本システム内部では全てのMAPMODEでY方向の上の座標>下の座標に変換
												// ( 例　MM_TEXTの場合はY座標×( -1))した値を使用している
												// 従って、図形を描くには、論理Y座標×m_iUPY　の値を使用する事

	MINT			m_iUpY;						// オリジナル論理座標のY方向( 1:上>下, -1:上<下)				
	MgMinMaxR2		m_rMinMaxRI;				// 実座標( Min/Max)初期値
	MgMinMaxR2		m_rMinMaxRS[MT_SZMINMAXRS];	// 実座標
	CPoint			m_ptCurMouseL;				// カレントのマウス位置（論理座標）
	MgMat3			m_mat;						// 座標変換マトリックス
	MREAL			m_sclRPtoLP;				// RPtoLPの座標変換倍率
	MgVect2			m_vsclLPtoDP;				// LPtoDPの座標変換倍率( x,y)
	CDC*			m_pDC;						// 表示　デバイスコンテキスト
	MINT			m_iMapMode;					// マップモード
//	線
public:
static	MINT		z_iLineStyle;				// 線スタイル
static	MREAL		z_fLineWidth;				// 線幅					( 単位　MT_FIXEDSIZE:ポイント, MT_FREESIZE:mm)
static	DWORD		z_rgbLine;					// 線色( RGB)
static	bool		z_bFixedLineWidth;			// 表示線幅固定フラグ
//	文字列
static	MCHAR		z_sTextFont[SZFONT];		// フォント
static	MREAL		z_fTextHeight;				// 文字高さ				( 単位　MT_FIXEDSIZE:ポイント, MT_FREESIZE:mm)
static	MINT		z_iTextPosUL;				// 文字列上下方向基準位置( TA_TOP( 1:上,2:中),TA_BASELINE( 3:下))
static	MINT		z_iTextPosLR;				// 文字列左右方向基準位置( TA_LEFT( 1:左),TA_CENTER( 2:中),TA_RIGHT( 3:右))
static	DWORD		z_rgbText;					// 文字色( RGB)
static	MgVect2		z_vTextDirect;				// 文字列表示方向		単位ベクトル
static	MgPoint2	z_pTextOffset;				// 文字位置オフセット	( 単位　MT_FIXEDSIZE:ポイント, MT_FREESIZE:mm)
static	bool		z_bFixedTextSize;			// 表示文字サイズ固定フラグ
//
public:
	MbCod( );
/////////////////////////////////////////////////////////////////////////////
//  マップモードを設定する
	MINT	SetMapMode( MINT nMapMode);

/////////////////////////////////////////////////////////////////////////////
//  マップモードを得る
	MINT	GetMapMode( )
						{ return m_iMapMode;}

/////////////////////////////////////////////////////////////////////////////
//  設定したマップモードでY方向の座標値の大きい方が上であるかを調べる
	bool	ChkMapModeY( )
						{ return ( m_iUpY > 0);}

/////////////////////////////////////////////////////////////////////////////
//  表示　デバイスコンテキストを設定する
	void	SetDC( CDC *pDC)
						{ m_pDC = pDC;}

/////////////////////////////////////////////////////////////////////////////
//  表示　デバイスコンテキストを得る
	CDC*	GetDC( )
						{ return m_pDC;}

/////////////////////////////////////////////////////////////////////////////
//  表示座標枠を設定する( 設定時に、Y座標 *= ( -1))
	void	SetWinD( 
				MINT	iStX,				// 表示座標　始点( 左上)X
				MINT	iStY,				// 表示座標　始点( 左上)始点Y
				MINT	iWidth,				// 表示座標　幅
				MINT	iHeight				// 表示座標　高さ
				)
						{ m_iMinMaxD.min.x = iStX; 
						  m_iMinMaxD.min.y = - ( iStY + iHeight);
						  m_iMinMaxD.max.x = iStX + iWidth;
						  m_iMinMaxD.max.y = - iStY;}
/////////////////////////////////////////////////////////////////////////////
//  表示座標枠を得る( 読み込み時に、Y座標 *= ( -1))
	void	GetWinD( 
				MINT	*piStX,				// 表示座標　始点( 左上)X
				MINT	*piStY,				// 表示座標　始点( 左上)Y
				MINT	*piWidth,			// 表示座標　幅
				MINT	*piHeight			// 表示座標　高さ
				)
						{ *piStX = m_iMinMaxD.min.x;
						  *piStY = - m_iMinMaxD.max.y;
						  *piWidth = m_iMinMaxD.max.x - m_iMinMaxD.min.x;
						  *piHeight = m_iMinMaxD.max.y - m_iMinMaxD.min.y;}
/////////////////////////////////////////////////////////////////////////////
//  論理座標枠を設定する( 上方向のY座標小の場合は設定時に、Y座標 *= ( -1))
	void	SetWinL( 
				MINT	iStX,				// 表示座標　始点( 左上)X
				MINT	iStY,				// 表示座標　始点( 左上)始点Y
				MINT	iWidth,				// 表示座標　幅
				MINT	iHeight				// 表示座標　高さ
				);
/////////////////////////////////////////////////////////////////////////////
//  論理座標枠を得る( 上方向のY座標小の場合は読み込み時に、Y座標 *= ( -1))
	void	GetWinL( 
				MINT	*piStX,				// 表示座標　始点( 左上)X
				MINT	*piStY,				// 表示座標　始点( 左上)Y
				MINT	*piWidth,			// 表示座標　幅
				MINT	*piHeight			// 表示座標　高さ
				);
/////////////////////////////////////////////////////////////////////////////
// オリジナル論理座標のY方向( 1:上>下, -1:上<下)				
	MINT	GetUpL( )
						{ return m_iUpY;}
/////////////////////////////////////////////////////////////////////////////
//  表示座標MinMaxを設定する( Y座標 *= ( -1))
	void	SetMinMaxD( MgMinMaxI2 iMinMaxD)
						{ m_iMinMaxD = iMinMaxD;}
/////////////////////////////////////////////////////////////////////////////
//  表示座標MinMaxを設定する
// 070907	void	SetMinMaxD( MINT iMinX, MINT iMinY, MINT iMaxX, MINT iMaxY)
// 070907						{
// 070907						  m_iMinMaxD.min.x = iMinX;
// 070907						  m_iMinMaxD.min.y = iMinY;
// 070907						  m_iMinMaxD.max.x = iMaxX;
// 070907						  m_iMinMaxD.max.y = iMaxY;}
/////////////////////////////////////////////////////////////////////////////
//  表示座標MinMaxを得る
	MgMinMaxI2 GetMinMaxD( )
						{ return m_iMinMaxD;}
/////////////////////////////////////////////////////////////////////////////
//  表示座標MinMaxを得る
// 070907	void	GetMinMaxD( MINT* iMinX, MINT* iMinY, MINT* iMaxX, MINT* iMaxY)
// 070907						{
// 070907						  *iMinX = m_iMinMaxD.min.x;
// 070907						  *iMinY = m_iMinMaxD.min.y;
// 070907						  *iMaxX = m_iMinMaxD.max.x;
// 070907						  *iMaxY = m_iMinMaxD.max.y;}
/////////////////////////////////////////////////////////////////////////////
//  論理座標MinMaxを設定する
	void	SetMinMaxL( MgMinMaxI2 iMinMaxL)
						{ m_iMinMaxL = iMinMaxL;}
/////////////////////////////////////////////////////////////////////////////
//  論理座標MinMaxを設定する
// 070907	void	SetMinMaxL( MINT iMinX, MINT iMinY, MINT iMaxX, MINT iMaxY)
// 070907						{
// 070907						  m_iMinMaxL.min.x = iMinX;
// 070907						  m_iMinMaxL.min.y = iMinY;
// 070907						  m_iMinMaxL.max.x = iMaxX;
// 070907						  m_iMinMaxL.max.y = iMaxY;}
/////////////////////////////////////////////////////////////////////////////
//  論理座標MinMaxを得る
	MgMinMaxI2 GetMinMaxL( )
						{ return m_iMinMaxL;}
/////////////////////////////////////////////////////////////////////////////
//  論理座標MinMaxを得る
// 070907	void	GetMinMaxL( MINT* iMinX, MINT* iMinY, MINT* iMaxX, MINT* iMaxY)
// 070907						{
// 070907						  *iMinX = m_iMinMaxL.min.x;
// 070907						  *iMinY = m_iMinMaxL.min.y;
// 070907						  *iMaxX = m_iMinMaxL.max.x;
// 070907						  *iMaxY = m_iMinMaxL.max.y;}
/////////////////////////////////////////////////////////////////////////////
//  実座標MinMax初期値を設定する
	void	SetMinMaxRI( MgMinMaxR2& rMinMaxRI)
						{ m_rMinMaxRI = rMinMaxRI;}
/////////////////////////////////////////////////////////////////////////////
//  実座標MinMax初期値を得る
	MgMinMaxR2 GetMinMaxRI( )
						{ return m_rMinMaxRI;}
/////////////////////////////////////////////////////////////////////////////
//  実座標MinMax初期値を実座標MinMaxに設定する
	void	InitMinMaxRS( )
						{ m_rMinMaxRS[0] = m_rMinMaxRS[1] = m_rMinMaxRI;}
/////////////////////////////////////////////////////////////////////////////
//  カレントの実座標MinMaxを設定する
	void	SetMinMaxRS( MgMinMaxR2& rMinMaxRS)
						{ m_rMinMaxRS[0] = rMinMaxRS;}
/////////////////////////////////////////////////////////////////////////////
//  カレントの実座標MinMaxを得る
	MgMinMaxR2 GetMinMaxRS( )
						{ return m_rMinMaxRS[0];}
/////////////////////////////////////////////////////////////////////////////
//  実座標MinMaxを先頭( カレント)に追加する（ｐｕｓｈ）
//D	void	PushMinMaxRS( MgMinMaxR2& rMinMaxRS)
//D						{ m_rMinMaxRS[1] = m_rMinMaxRS[0];
//D						  m_rMinMaxRS[0] = rMinMaxRS;}
/////////////////////////////////////////////////////////////////////////////
//  先頭( カレント)の実座標MinMaxを取り出す（ＰＯＰ）
//D	MgMinMaxR2 PopMinMaxRS( )
//D						{ MgMinMaxR2 rMinMax;
//D						  rMinMax = m_rMinMaxRS[0];
//D						  m_rMinMaxRS[0] = m_rMinMaxRS[1];
//D						  m_rMinMaxRS[1] = rMinMax;
//D						  return rMinMax;}

/////////////////////////////////////////////////////////////////////////////
//  実座標から論理座標への座標変換マトリックスと
//	論理座標から表示座標への変換倍率を設定する
	void	SetMat( );

/////////////////////////////////////////////////////////////////////////////
//  現在のマウス位置を設定する
	void	SetCurMousePtL( CPoint ptCurMous)
						{ m_ptCurMouseL = ptCurMous;}

/////////////////////////////////////////////////////////////////////////////
//  現在のマウス位置を得る
	CPoint	GetCurMousePtL( )
						{ return m_ptCurMouseL;}

/////////////////////////////////////////////////////////////////////////////
//  現在のマウス位置を得る
	MgPoint2 GetCurMousePtR( )
						{ return LPtoRP( m_ptCurMouseL);}

/////////////////////////////////////////////////////////////////////////////
//  座標変換する　実座標　→　論理座標
	MINT	RPtoLP( 	MREAL rRP);
	CPoint	RPtoLP( MgPoint2 pRP);

/////////////////////////////////////////////////////////////////////////////
//  座標変換する　論理座標　→　表示座標
	MINT	LPtoDP( MINT   iLP);
	CPoint	LPtoDP( CPoint pLP);

/////////////////////////////////////////////////////////////////////////////
//  座標変換する　実座標　→　表示座標
	MINT	RPtoDP( MREAL   rRP);
	CPoint	RPtoDP( MgPoint2 pRP);

/////////////////////////////////////////////////////////////////////////////
//  座標変換する　表示座標　→　論理座標
	MINT	DPtoLP( MINT   iDP);
	CPoint	DPtoLP( CPoint pDP);

/////////////////////////////////////////////////////////////////////////////
//  座標変換する　論理座標　→　実座標
	MREAL		LPtoRP( MINT i);
	MgPoint2	LPtoRP( CPoint p);

/////////////////////////////////////////////////////////////////////////////
//  座標変換する　表示座標　→　実座標
	MREAL		DPtoRP( MINT   iDP);
	MgPoint2	DPtoRP( CPoint pDP);
	
/////////////////////////////////////////////////////////////////////////////
//  線属性　線種、線幅、線色を設定する
	void	SetLineAttr( 
						MPENSTYLE	iLineStyle		= MPS_SOLID,
						MREAL		fLineWidth		= 1,
						DWORD		rgbLine			= RGB( 0, 0, 0),
						bool		bFixedLineWidth = TRUE
						);

/////////////////////////////////////////////////////////////////////////////
//  	線種を設定する
	void	SetLineStyle( MPENSTYLE iLineStyle);

/////////////////////////////////////////////////////////////////////////////
//  	線幅を設定する
	void	SetLineWidth( MREAL fLineWidth);

/////////////////////////////////////////////////////////////////////////////
//  	線色を設定する
	void	SetLineColor( DWORD rgbLine);
/////////////////////////////////////////////////////////////////////////////
//  	表示線幅固定フラグを設定する
	void	SetFixedLineWidth( bool bFixedLineWidth = TRUE);	// 表示線幅固定フラグ
																//			TRUE  :	表示線幅固定
																//			FALSE : 表示線幅自由

/////////////////////////////////////////////////////////////////////////////
//  文字属性　フォント、文字高さ、文字方向、オフセット文字位置を設定する
	void	SetTextAttr( 
						MCHAR*		sTextFont,
						MREAL		fTextHeight		= 10,
						MINT		iPosUL			= MT_LOWER,
						MINT		iPosLR			= MT_LEFT,
						DWORD		rgbText			= RGB( 0, 0, 0),
						MgVect2&	vDirect			= MgPoint2( 1., 0.),
						MgPoint2&	pOffset			= MgPoint2( 0., 0.),
						bool		bFixedTextSize	= TRUE
						);

/////////////////////////////////////////////////////////////////////////////
//  	フォントを設定する
	void	SetTextFont( MCHAR* sTextFont);

/////////////////////////////////////////////////////////////////////////////
//  	文字高さを設定する
	void	SetTextHeight( MREAL fTextHeight);

/////////////////////////////////////////////////////////////////////////////
//  	文字位置を設定する
	void	SetTextIchi( MINT iPosUL, MINT iPosLR);

/////////////////////////////////////////////////////////////////////////////
//  	文字色を設定する
	void	SetTextColor( DWORD rgb);

/////////////////////////////////////////////////////////////////////////////
//  	文字方向を設定する
	void	SetTextDirect( MgVect2& vDirect = MgPoint2( 1., 0.));

/////////////////////////////////////////////////////////////////////////////
//  	オフセットを設定する
	void	SetTextOffset( MgPoint2& pOffset = MgPoint2( 0., 0.));

/////////////////////////////////////////////////////////////////////////////
//  	文字サイズ固定フラグを設定する
	void	SetFixedTextSize( bool bFixedTextSize = FALSE);

/////////////////////////////////////////////////////////////////////////////
//  文字列を表示する
	void	Text( const MgPoint2 &pa1, MCHAR* str);

/////////////////////////////////////////////////////////////////////////////
//  線分を表示する
	void	Line( const MgLine2 &ln1)
						{ Line( ln1.p[0], ln1.p[1]);}

/////////////////////////////////////////////////////////////////////////////
//  線分を表示する
	void	Line( const MgPoint2 &pa1, const MgPoint2 &pa2);

/////////////////////////////////////////////////////////////////////////////
//  長方形を表示する
	void	Rect( const MgPoint2 &pa1, const MgPoint2 &pa2);

/////////////////////////////////////////////////////////////////////////////
//  長方形を表示する
	void	Rect( const MgRect2 &rt1)
						{ Rect( rt1.p[0], rt1.p[1]);}

/////////////////////////////////////////////////////////////////////////////
//  長方形を表示する
//C	void	Rect( const MgPoint2 &pa1, const MgPoint2 &pa2);

/////////////////////////////////////////////////////////////////////////////
//  折れ線を表示する
	void	Polyline( MgPoint2 *pb, int inpb);

/////////////////////////////////////////////////////////////////////////////
//  多角形を表示する
	void	Polygon( MgPoint2 *pb, int inpb);

/////////////////////////////////////////////////////////////////////////////
//  円弧を表示する
	void	Arc1( const MgPoint2 &pac, MREAL rh, MREAL a1, MREAL a2);

};
/*
//
/////////////////////////////////////////////////////////////////////////////
//  線属性　線種、線幅、線色を設定する
	void	SetLineAttr( 
						MPENSTYLE	iLineStyle		= MPS_SOLID,
						MREAL		fLineWidth		= 1,
						DWORD		rgbLine			= RGB( 0, 0, 0),
						bool		bFixedLineWidth = TRUE
						);
*/
// inline //

/////////////////////////////////////////////////////////////////////////////
//  座標変換する　実座標　→　論理座標
inline CPoint MbCod::RPtoLP( MgPoint2 p)
{
	MgPoint2 p1 = p * m_mat;
	return MgCPointC( p1);
}

inline MINT	MbCod::RPtoLP( 
				MREAL		rRP
)
{
	return MINT( rRP * m_sclRPtoLP);
}

/////////////////////////////////////////////////////////////////////////////
//  座標変換する　論理座標　→　表示座標
inline CPoint MbCod::LPtoDP( 
				CPoint		pLP
				)
{
	return CPoint( MINT( pLP.x * m_vsclLPtoDP.x),
				   MINT( pLP.y * m_vsclLPtoDP.y));
}

inline MINT	MbCod::LPtoDP( 
				MINT   		iLP
				)
{
	return ( int)( iLP * m_vsclLPtoDP.x);
}

/////////////////////////////////////////////////////////////////////////////
//  座標変換する　実座標　→　表示座標
inline CPoint MbCod::RPtoDP( 
				MgPoint2	pRP
				)
{
	MgPoint2 pLP = pRP * m_mat; 
	return CPoint( MINT( pLP.x * m_vsclLPtoDP.x),
				   MINT( pLP.y * m_vsclLPtoDP.y));
}

inline MINT	MbCod::RPtoDP( 
				MREAL	   rRP
				)
{
	return ( MINT)( rRP * m_sclRPtoLP * m_vsclLPtoDP.x);
}

/////////////////////////////////////////////////////////////////////////////
//  座標変換する　表示座標　→　論理座標
inline CPoint MbCod::DPtoLP( 
				CPoint		pDP
				)
{
	return CPoint( MINT( pDP.x / m_vsclLPtoDP.x),
				   MINT( pDP.y / m_vsclLPtoDP.y));
}

inline MINT	MbCod::DPtoLP( 
				MINT		iDP
				)
{
	return ( int)( iDP / m_vsclLPtoDP.x);
}

/////////////////////////////////////////////////////////////////////////////
//  座標変換する　論理座標　→　実座標
inline MgPoint2 MbCod::LPtoRP( 
				CPoint		p
				)
{
	MgPoint2 MbP2_t1= p;
	return MbP2_t1 * MGeo::Mat3Inv( m_mat);
}

inline MREAL MbCod::LPtoRP( 
				MINT		i
				)
{
	return MREAL( i / m_sclRPtoLP);
}

/////////////////////////////////////////////////////////////////////////////
//  座標変換する　表示座標　→　実座標
inline MREAL MbCod::DPtoRP( 
				MINT	   iDP
				)
{
	return ( iDP / ( m_sclRPtoLP * m_vsclLPtoDP.x));
}

inline MgPoint2 MbCod::DPtoRP( 
				CPoint		pDP
				)
{
	MgPoint2 pLP = MgPoint2( pDP.x / m_vsclLPtoDP.x,
							 pDP.y / m_vsclLPtoDP.y);
	return pLP * MGeo::Mat3Inv( m_mat);
}

} // namespace MC

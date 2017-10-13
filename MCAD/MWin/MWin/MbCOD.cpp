//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: .cpp
//
//		
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include <math.h>
#include "MbBasic.h"

#define	DLL_EXPORT_COD_DO

#include "MgTol.h"

#include "MbDefine.h"
#include "MgMat.h"

#include "MgLib.h"
#include "MbCod.h"

#define		MCFIXEDSZ		10

namespace MC
{

/////////////////////////////////////////////////////////////////////////////
//  スタティック属性値の初期設定
MINT		MbCod::z_iLineStyle		= MPS_SOLID;					// ペンスタイル
MREAL		MbCod::z_fLineWidth		= 4.;							// ペン幅				(単位　MT_FIXEDSIZE:ポイント, MT_FREESIZE:mm)
DWORD		MbCod::z_rgbLine		= RGB( 200, 128, 100);			// 色(RGB)
bool		MbCod::z_bFixedLineWidth = TRUE;						// 線幅固定サイズフラグ

MCHAR		MbCod::z_sTextFont[]	= _T("ＭＳ Ｐゴシック");		// フォント
MREAL		MbCod::z_fTextHeight	= 10.;							// 文字高さ				(単位　MT_FIXEDSIZE:ポイント, MT_FREESIZE:mm)
MINT		MbCod::z_iTextPosUL		= 3;							// 文字列上下方向基準位置(TA_TOP(1:上,2:中),TA_BOTTOM(3:下),TA_BASELINE(4:ベース))
MINT		MbCod::z_iTextPosLR		= 1;							// 文字列左右方向基準位置(TA_LEFT(1:左),TA_CENTER(2:中),TA_RIGHT(3:右))
DWORD		MbCod::z_rgbText		= RGB( 200, 128, 100);			// 色(RGB)
MgVect2		MbCod::z_vTextDirect	= MgVect2( 1.f, 0.);			// 文字列表示方向
MgPoint2	MbCod::z_pTextOffset	= MgPoint2( 0., 0.);			// 文字位置オフセット	(単位　MT_FIXEDSIZE:ポイント, MT_FREESIZE:mm)
bool		MbCod::z_bFixedTextSize	= TRUE;							// 文字列固定サイズフラグ

/////////////////////////////////////////////////////////////////////////////
//  ペンタイプの変換テーブル
static MINT PSTbl[] = {
	PS_SOLID,				// MPS_SOLID		0:実線のペンを作成します。 
	PS_DASH,				// MPS_DASH			1:破線のペンを作成します。ペン幅がデバイス単位 1 以下の場合だけ有効です。 
	PS_DOT,					// MPS_DOT 			2:点線のペンを作成します。ペン幅がデバイス単位 1 以下の場合だけ有効です。 
	PS_DASHDOT,				// MPS_DASHDOT 		3:一点破線のペンを作成します。ペン幅がデバイス単位 1 以下の場合だけ有効です。 
	PS_DASHDOTDOT,			// MPS_DASHDOTDOT	4:二点破線のペンを作成します。ペン幅がデバイス単位 1 以下の場合だけ有効です。 
	PS_NULL					// MPS_NULL			5:null のペンを作成します。 
};

//     以下のiPenはプログラム作成時のサンプルで不要
MINT	iPen[] = {
// nPenStyle 
// ペンのスタイルを指定します。最初の構文のコンストラクタのパラメータには、次のいずれかの値を指定できます。 
	PS_SOLID,				// 0:実線のペンを作成します。 
	PS_DASH,				// 1:破線のペンを作成します。ペン幅がデバイス単位 1 以下の場合だけ有効です。 
	PS_DOT,					// 2:点線のペンを作成します。ペン幅がデバイス単位 1 以下の場合だけ有効です。 
	PS_DASHDOT,				// 3:一点破線のペンを作成します。ペン幅がデバイス単位 1 以下の場合だけ有効です。 
	PS_DASHDOTDOT,			// 4:二点破線のペンを作成します。ペン幅がデバイス単位 1 以下の場合だけ有効です。 
	PS_NULL,				// 5:null のペンを作成します。 
	PS_INSIDEFRAME,			// 6:外接する四角形を指定する Windows の GDI の出力関数 (たとえば、Ellipse、Rectangle、RoundRect、Pie、Chord メンバ関数) で作成される閉じた図形の枠の内部に直線を描画するペンを作成します。外接する四角形を指定しない Windows の GDI 出力関数 (たとえば、LineTo メンバ関数) で作成される図形のときは、ペンの描画領域は図形の枠に制限されません。 

// CPen コンストラクタの 2 番目の構文では、型、スタイル、端点キャップ、結合属性の組み合わせを指定します。各カテゴリの値はビットごとの OR 演算子 (| ) を使って組み合わせます。). ペンの型は次のいずれかです。 

	PS_GEOMETRIC,			// 0x10000:ジオメトリック ペンを作成します。 
	PS_COSMETIC,			// 0:コスメティック ペンを作成します。 

// CPen コンストラクタの 2 番目の構文の nPenStyle に指定するペン スタイルに次のスタイルが追加されます。 

	PS_ALTERNATE,			// 8:1 つおきにピクセルを設定するペンを作成します。(このスタイルは、コスメティック ペンだけに適用できます。 
	PS_USERSTYLE,			// 7:ユーザーが提供するスタイル配列を使うペンを作成します。 

// 端点キャップは次のいずれかの値になります。 

	PS_ENDCAP_ROUND,		// 0:端点キャップは円になります。 
	PS_ENDCAP_SQUARE,		// 0x100:端点キャップは四角形になります。 
	PS_ENDCAP_FLAT,			// 0x200:端点キャップは平面になります。 

// 結合は次のいずれかの値になります。 

	PS_JOIN_BEVEL,			// 0x1000:斜め結合になります。 
	PS_JOIN_MITER,			// 0x2000:SetMiterLimit 関数で設定した現在の制限内の場合、マイター結合になります。制限を超えたときは斜め結合になります。 
	PS_JOIN_ROUND			// 0:円結合になります。 
};

/////////////////////////////////////////////////////////////////////////////
//  座標変換
MbCod::MbCod()
{
	MREAL rr = g_gTol.S;

	m_iMinMaxD.SetUnit();
	m_iMinMaxL.SetUnit();
	m_rMinMaxRS[0].SetUnit();
	SetMat();
}

/////////////////////////////////////////////////////////////////////////////
//  論理座標画面枠を設定する(上方向のY座標小の場合は設定時に、Y座標 *= (-1))
void MbCod::SetWinL(
						MINT	iStX,			// 表示座標　始点(左上)X
						MINT	iStY,			// 表示座標　始点(左上)始点Y
						MINT	iWidth,			// 表示座標　幅
						MINT	iHeight			// 表示座標　高さ
				)
{
	m_iMinMaxL.min.x = iStX;
	m_iMinMaxL.max.x = iStX + iWidth;
	if ( iHeight < 0) {
		m_iUpY = 1;
		m_iMinMaxL.min.y = iStY + iHeight;
		m_iMinMaxL.max.y = iStY;
	} else {
		m_iUpY = -1;
		m_iMinMaxL.min.y = -(iStY + iHeight);
		m_iMinMaxL.max.y = -iStY;
	}
}

/////////////////////////////////////////////////////////////////////////////
//  論理座標画面枠を得る(上方向のY座標小の場合は読み込み時に、Y座標 *= (-1))
void MbCod::GetWinL(
						MINT	*piStX,			// 表示座標　始点(左上)X
						MINT	*piStY,			// 表示座標　始点(左上)Y
						MINT	*piWidth,		// 表示座標　幅
						MINT	*piHeight		// 表示座標　高さ
				)
{
	*piStX = m_iMinMaxL.min.x;
	*piStY = m_iMinMaxL.max.y * m_iUpY;
	*piWidth = m_iMinMaxL.max.x - m_iMinMaxL.min.x;
	*piHeight =(m_iMinMaxL.min.y - m_iMinMaxL.max.y) * m_iUpY;
}

/////////////////////////////////////////////////////////////////////////////
//  マップモードを設定する
// パラメータ
// nMapMode 
//	新しいマップ モードを指定する。次のいずれかの値になる。 
//	以下の○印はx 座標の正方向は右、y 座標の正方向は上になる。	
//
//	・MM_ANISOTROPIC  論理単位は、任意にスケーリングされた軸上の任意の単位に変換される。
//						マップ モードに MM_ANISOTROPIC を設定しても、現在のウィンドウまたは
//						ビューポートの設定は変更されない。単位、向き、スケーリングを変更
//						するときは、SetWindowExt メンバ関数と SetViewportExt メンバ関数を
//						使う必要がある。 
//	○MM_HIENGLISH	  それぞれの論理単位は、0.001 インチに変換される。
//	○MM_HIMETRIC	  それぞれの論理単位は、0.01 mm に変換される。
//	・MM_ISOTROPIC	  論理単位は、等しくスケーリングされた軸上の任意の単位に変換される。
//						x 軸方向の 1 単位は、y 軸方向の 1 単位と同じになる。
//						両軸の希望する単位と向きを指定するときは、SetWindowExt メンバ関数と
//						SetViewportExt メンバ関数を使う。　GDI は、必要に応じて、x 単位と
//						y 単位が同じサイズになるように調整する。 
//	○MM_LOENGLISH    それぞれの論理単位は、0.01 インチに変換される。
//	○MM_LOMETRIC     それぞれの論理単位は、0.1 mm に変換される。
//	・MM_TEXT		  それぞれの論理単位は、1 デバイス ピクセルに変換される。
//	○MM_TWIPS		  それぞれの論理単位は、1 ポイントの 1/20 に変換される。
//						1 ポイントは 1/72 インチなので、1 twip は 1/1440 インチになる。
// 戻り値
//	直前のマップ モードを返す。
//
// 解説
//	マップ モードは、論理単位をデバイス単位に変換するときの縮尺を定義し、デバイスの x 軸と y 軸の
//	向きも定義する。　GDI は、マップ モードを使って、論理座標を適切なデバイス座標に変換する。
//	MM_TEXT モードを使うと、アプリケーションが作業するデバイス ピクセルは、1 ピクセルが 1 単位になる。
//	ピクセルの物理サイズは、デバイスごとに異なる。 
//
//	MM_HIENGLISH、MM_HIMETRIC、MM_LOENGLISH、MM_LOMETRIC、MM_TWIPS モードは、アプリケーションが物理単位
//	(インチや mm のような) で描画するときに便利である。
//	MM_ISOTROPIC モードは、縦横比が 1:1 なので、イメージの正確な形を保存する必要があるときに便利である。
//	MM_ANISOTROPIC モードは、x 座標と y 座標を個別に調節できるようにする。
//
// メモ 
//	デバイス コンテキストを右から左へのレイアウトに変更するために SetLayout を呼び出すと、
//	SetLayout が自動的にマップ モードを MM_ISOTROPIC に変更する。
//
MINT MbCod::SetMapMode(
						MINT		iMapMode
				)
{
	if ( iMapMode == 0) {
		iMapMode = m_iMapMode;
	} else {
		m_iMapMode = iMapMode;
	}
	return m_pDC->SetMapMode( m_iMapMode);
}

/////////////////////////////////////////////////////////////////////////////
//  実座標から論理座標への座標変換マトリックスと
//	論理座標から表示座標への変換倍率を設定する
void MbCod::SetMat()
{
	MREAL s0, s1, s2;
	m_mat.SetUnit();
	s1 = (m_iMinMaxL.max.x - m_iMinMaxL.min.x) /
		 (m_rMinMaxRS[0].max.x - m_rMinMaxRS[0].min.x);
	s2 = (m_iMinMaxL.max.y - m_iMinMaxL.min.y) /
		 (m_rMinMaxRS[0].max.y - m_rMinMaxRS[0].min.y);
	s0 = __min(s1, s2);
	m_mat.m[0][0] = s0;
	m_mat.m[1][1] = s0;

	m_mat.m[2][2] = s0;
	m_mat.m[3][0] = (m_iMinMaxL.min.x + m_iMinMaxL.max.x) * 0.5f -
					s0*(m_rMinMaxRS[0].min.x + m_rMinMaxRS[0].max.x) * 0.5f;
	m_mat.m[3][1] = (m_iMinMaxL.min.y + m_iMinMaxL.max.y) * 0.5f -
					s0*(m_rMinMaxRS[0].min.y + m_rMinMaxRS[0].max.y) * 0.5f;

	m_sclRPtoLP = s0;

	m_vsclLPtoDP.x = MREAL( m_iMinMaxD.max.x - m_iMinMaxD.min.x) /
					 MREAL( m_iMinMaxL.max.x - m_iMinMaxL.min.x);
	m_vsclLPtoDP.y = - MREAL( m_iMinMaxD.max.y - m_iMinMaxD.min.y) /
					 MREAL( m_iMinMaxL.max.y - m_iMinMaxL.min.y);
}

/////////////////////////////////////////////////////////////////////////////
// 描画処理
//#define			MT_LEFT			1
//#define			MT_CENTER		2
//#define			MT_RIGHT		3
//#define			MT_UPPER		1
//#define			MT_LOWER		3

//#define			MT_FIXEDSIZE	TRUE
//#define			MT_FREESIZE		FALSE

/////////////////////////////////////////////////////////////////////////////
//  線種、線幅、線色を設定する
void MbCod::SetLineAttr(
						MPENSTYLE	iLineStyle,					// 線種
																//			MPS_SOLID		(0) :	実線
																//			MPS_DASH		(1) :	破線
																//			MPS_DOT			(2) :	点線
																//			MPS_DASHDOT		(3) :	１点鎖線
																//			MPS_DASHDOTDOT	(4) :	２点鎖線
																//			MPS_NULL		(5) :	無色
						MREAL		fLineWidth,					// 線幅
																//			線幅固定モードの場合はドット数、
																//			線幅自由モードの場合は(1/10)mm
						DWORD		rgbLine,					// 線色	RGB
						bool		bFixedLineWidth				// 線幅固定モードフラグ
																//			TRUE  :	線幅固定
																//			FALSE : 線幅自由
				)
{
	z_iLineStyle 		= iLineStyle;
	z_fLineWidth 		= fLineWidth;
	z_rgbLine 			= rgbLine;
	z_bFixedLineWidth	= bFixedLineWidth;
}

/////////////////////////////////////////////////////////////////////////////
//  	線種を設定する
void MbCod::SetLineStyle(
						MPENSTYLE	iLineStyle					// 線種
																//			MPS_SOLID		(0) :	実線
																//			MPS_DASH		(1) :	破線
																//			MPS_DOT			(2) :	点線
																//			MPS_DASHDOT		(3) :	１点鎖線
																//			MPS_DASHDOTDOT	(4) :	２点鎖線
																//			MPS_NULL		(5) :	無色
				)
{
	z_iLineStyle = iLineStyle;
}

/////////////////////////////////////////////////////////////////////////////
//  	線幅を設定する
void MbCod::SetLineWidth(
						MREAL		fLineWidth					// 線幅
																//			線幅固定モードの場合はドット数、
																//			線幅自由モードの場合はmm
				)
{
	z_fLineWidth = fLineWidth;
}

/////////////////////////////////////////////////////////////////////////////
//  	線色を設定する
void MbCod::SetLineColor(
							DWORD	rgbLine						// 線色	RGB
					)
{
	z_rgbLine = rgbLine;
}

/////////////////////////////////////////////////////////////////////////////
//  	線幅固定フラグを設定する
void MbCod::SetFixedLineWidth(
							bool	bFixedLineWidth)			// 表示線幅固定フラグ
																//			TRUE  :	線幅固定
																//			FALSE : 線幅自由
{
	z_bFixedLineWidth = bFixedLineWidth;
}

/////////////////////////////////////////////////////////////////////////////
//  	フォント、文字高さ、文字方向、オフセット文字位置を設定する
void MbCod::SetTextAttr(
						MCHAR*		sFont,						// 文字フォント
						MREAL		fHeight,					// 文字高さ	
																//			文字線幅固定モードの場合はドット数、
																//			文字線幅自由モードの場合は(1/10)mm
						MINT		iPosUL,						// 文字上下基準位置
																//			MT_UPPER	: 上
																//			MT_CENTER	: 中
																//			MT_LOWER	: 下
						MINT		iPosLR,						// 文字左右基準位置
																//			MT_LEFT		: 左
																//			MT_CENTER	: 中
																//			MT_RIGHT	: 右
						DWORD		rgbText,					// 文字色	RGB
						MgVect2&	vDirect,					// 文字表示方向	(dx, dy)	(長さ自由)
						MgPoint2&	pOffset,					// 文字表示位置オフセット (x, y)
																//			文字線幅固定モードの場合はドット数、
																//			文字線幅自由モードの場合は(1/10)mm
						bool		bFixedTextSize				// 文字線幅固定モード
																//			MT_FIXEDSIZE : 文字線幅固定(TRUE)
																//			MT_FREESIZE	 : 文字線幅自由(FALSE)
				)
{
	SetTextFont( sFont);
	z_fTextHeight = fHeight;
	SetTextIchi( iPosUL, iPosLR);

	z_rgbText			= rgbText;
	z_vTextDirect		= vDirect.Unitize();
	z_pTextOffset		= pOffset;
	z_bFixedTextSize	= bFixedTextSize;
}

/////////////////////////////////////////////////////////////////////////////
//  	フォントを設定する
void MbCod::SetTextFont(
						MCHAR*		sFont						// フォント
				)
{
	if (sFont == NULL)
		Mstrcpy( z_sTextFont, Mstr( "ＭＳ ゴシック"));
	else
		Mstrcpy( z_sTextFont, sFont);
}

/////////////////////////////////////////////////////////////////////////////
//  	文字高さを設定する
void MbCod::SetTextHeight(
						MREAL		fHeight						// 文字高さ	
																//			文字線幅固定モードの場合はドット数、
																//			文字線幅自由モードの場合はmm
				)
{
	z_fTextHeight = fHeight;
}

/////////////////////////////////////////////////////////////////////////////
//  	文字列の配置基準位置を設定する
void MbCod::SetTextIchi(
						MINT		iPosUL,						// 文字上下基準位置
																//			MT_UPPER	: 上
																//			MT_CENTER	: 中
																//			MT_LOWER	: 下
						MINT		iPosLR						// 文字左右基準位置
																//			MT_LEFT		: 左
																//			MT_CENTER	: 中
																//			MT_RIGHT	: 右
				)
{
	if ( iPosUL >= 1 && iPosUL <= 3)
		z_iTextPosUL = iPosUL;
	else
		z_iTextPosUL = 1;

	if ( iPosLR >= 1 && iPosLR <= 3)
		z_iTextPosLR = iPosLR;
	else
		z_iTextPosLR = 1;
}

/////////////////////////////////////////////////////////////////////////////
//  	文字色を設定する
void MbCod::SetTextColor(
						DWORD		rgbText						// 文字色	RGB
				)
{
	z_rgbText = rgbText;
}

/////////////////////////////////////////////////////////////////////////////
//  	文字方向を設定する
void MbCod::SetTextDirect(
						MgVect2&	vDirect						// 文字表示方向	(dx, dy)	(長さ自由)
				)
{
	z_vTextDirect = vDirect.Unitize();
}

/////////////////////////////////////////////////////////////////////////////
//  	オフセットを設定する
void MbCod::SetTextOffset(
						MgPoint2&	pOffset						// 文字表示位置オフセット (x, y)
																//			文字線幅固定モードの場合はドット数、
																//			文字線幅自由モードの場合はmm
				)
{
	z_pTextOffset = pOffset;
}

/////////////////////////////////////////////////////////////////////////////
//  	表示文字サイズ固定フラグを設定する
void MbCod::SetFixedTextSize(
						bool		bFixedTextSize				//	表示文字サイズ固定モード
																//			TRUE  :	表示文字サイズ固定
																//			FALSE : 表示文字サイズ自由
				)
{
	z_bFixedTextSize = bFixedTextSize;
}
/////////////////////////////////////////////////////////////////////////////
//  文字列を表示する
void MbCod::Text(
						const	MgPoint2	&pa1,
								MCHAR*		str
				)
{
	MgPoint2	p1r;											// 表示位置
	CPoint		p1s;											// 表示位置
	MREAL		fAngle;											// 表示角度（度）
	MINT		iAngle;											// 表示角度（度） × 10
	MINT		iTextPosLR;
	MINT		iTextPosUL;
	MgVect2		vDirect;										// 表示方向（単位ベクトル）
	MgVect2		vOffset;										// 表示オフセット

//	MINT		TAUD[] = { TA_TOP, TA_TOP, TA_BOTTOM};			// 上下文字基準位置変換テーブル　TA_BASELINEは未使用
	MINT		TAUD[] = { TA_TOP, TA_TOP, TA_BASELINE};		// 上下文字基準位置変換テーブル　TA_BOTTOMは未使用
	MINT		TALR[] = { TA_LEFT, TA_CENTER, TA_RIGHT};		// 左右文字基準位置変換テーブル
	MINT		iTextPos;

	CPen		Pen;
	LOGFONT		lf;
	CFont		font;
	MINT		iTextHeight;									// 文字表示高さ

	memset( &lf, 0, sizeof( LOGFONT));

	vDirect = z_vTextDirect;
	vOffset = z_pTextOffset;

	fAngle = MGeo::GetAngleVH2( vDirect);							// 左回転角度(度)
	if (fAngle <= 105 && fAngle >= -75) {
		iTextPosUL = z_iTextPosUL;
		iTextPosLR = z_iTextPosLR;
	} else {													// 表示角度により文字表示方向反転
		fAngle -= 180.;	if (fAngle <= -180) fAngle += 360;
		vDirect.SetRot180();
		vOffset.SetRot180();
		iTextPosUL = 4 - z_iTextPosUL;
		iTextPosLR = 4 - z_iTextPosLR;
	
	}
	iAngle = MINT( fAngle * 10);								// 文字列表示方向　左回転角度(1/10度 四捨五入) 
	lf.lfEscapement	= - iAngle * m_iUpY;						// 文字表示角度	上方向オリジナル論理Y座標大の場合は右回転(1/10度)
																//				上方向オリジナル論理Y座標小の場合は左回転(1/10度)
	if (z_bFixedTextSize) {
		iTextHeight = MINT( z_fTextHeight * 10);
		if (z_iTextPosUL == 2) 
			vOffset.y += (z_fTextHeight * 0.5f * 1.2f);			// 中段表示用補正 ( /2 /10 * (補正　1.2)
		vOffset /= (m_sclRPtoLP * m_vsclLPtoDP.x);				// x方向とy方向とでずれが異なったためm_vsclLPtoDP.yは不使用
	} else {
		iTextHeight = MINT( z_fTextHeight * m_sclRPtoLP * 10 *
			(MGeo::Abs( MgPoint2( MGeo::Abs(m_vsclLPtoDP.y * vDirect.x),
								  MGeo::Abs(m_vsclLPtoDP.x * vDirect.y)))));
//		vOffset *= (m_sclRPtoLP * m_vsclLPtoDP.x);				// x方向とy方向とでずれが異なったためm_vsclLPtoDP.yは不使用
		if (z_iTextPosUL == 2) 
			vOffset.y += (z_fTextHeight * 0.5f * 1.2f);			// 中段表示用補正 ( /2 /10 * (補正　1.2)
	}
	lf.lfHeight		= iTextHeight;								// 文字表示高さ	単位 1/10ポイント

	Mstrcpy( lf.lfFaceName, z_sTextFont);						// 字体
	VERIFY( font.CreatePointFontIndirect( &lf, m_pDC));
	
	CFont* pBackup = m_pDC->SelectObject( &font);				// フォントの設定

	iTextPos = TAUD[iTextPosUL - 1] | TALR[iTextPosLR - 1];
	m_pDC->SetTextAlign( iTextPos);								// 文字列表示基準位置の指定

//	m_pDC->SetBkColor( RGB( 100, 100, 100));
	m_pDC->SetBkMode( TRANSPARENT);								// 透明背景色
	m_pDC->SetTextColor( z_rgbText);							// 文字色


	vOffset.SetRot( vDirect);

	p1r = pa1 + vOffset;
	p1s = RPtoLP( p1r);

//	m_pDC->TextOut( (MINT)p1s.x, (MINT)p1s.y * m_iUpY, str, (MINT)strlen(str));
	m_pDC->TextOut( (MINT)p1s.x, (MINT)p1s.y * m_iUpY, str);

	m_pDC->SelectObject( pBackup);
	font.DeleteObject();
}

/////////////////////////////////////////////////////////////////////////////
//  線分を表示する	( "MbCod.h" で 定義済み )
//void MbCod::Line(
//				const	MgLine2		&ln1
//				)
//{
//	Line( ln1.p[0], ln1.p[1]);
//}

/////////////////////////////////////////////////////////////////////////////
//  線分を表示する
void MbCod::Line(
				const	MgPoint2	&pa1,
				const	MgPoint2	&pa2
				)
{
	CPoint		p1, p2;
	MINT		iPenStyle;
	CPen		Pen;
	CGdiObject* pBackup;
//	MgVect2		vDirect = pa2 - pa1;
	MINT		iLineWidth;										// 線幅

	p1 = RPtoLP( pa1);
	p2 = RPtoLP( pa2);
/*
//	if (abs(p1.x - p2.x) < MgTol.D) {
	if (MgZero( vDirect.x)) {
		if (p1.y < m_iMinMaxL.min.y)		p1.y = m_iMinMaxL.min.y;
		else if (p1.y > m_iMinMaxL.max.y)	p1.y = m_iMinMaxL.max.y;
		if (p2.y < m_iMinMaxL.min.y)		p2.y = m_iMinMaxL.min.y;
		else if (p2.y > m_iMinMaxL.max.y)	p2.y = m_iMinMaxL.max.y;
	}
//	if (abs(p1.y - p2.y) < MgTol.D) {
	if (MgZero( vDirect.y)) {
		if (p1.x < m_iMinMaxL.min.x)		p1.x = m_iMinMaxL.min.x;
		else if (p1.x > m_iMinMaxL.max.x)	p1.x = m_iMinMaxL.max.x;
		if (p2.x < m_iMinMaxL.min.x)		p2.x = m_iMinMaxL.min.x;
		else if (p2.x > m_iMinMaxL.max.x)	p2.x = m_iMinMaxL.max.x;
	}
*/
	iPenStyle = PSTbl[z_iLineStyle];

	if (z_bFixedLineWidth)
		iLineWidth = MGMAX( MINT( z_fLineWidth / m_vsclLPtoDP.x), 1);
	else
		iLineWidth = MINT( z_fLineWidth * m_sclRPtoLP);

	Pen.CreatePen(iPenStyle, iLineWidth, z_rgbLine);
	pBackup = m_pDC->SelectObject( &Pen);

	m_pDC->MoveTo( p1.x, p1.y * m_iUpY);
	m_pDC->LineTo( p2.x, p2.y * m_iUpY);

	m_pDC->SelectObject( pBackup);
	Pen.DeleteObject();
}

/////////////////////////////////////////////////////////////////////////////
//  長方形を表示する ( "MbCod.h" で 定義済み )
//void MbCod::Rect(
//				const	MgRect2		&rt1
//				)
//{
//	MgPoint2		p1, p2;
//	MINT		iPenStyle;
//	CPen		Pen;
//	CGdiObject* pBackup;
//	MINT		iLineWidth;										// 線幅

//	p1 = RPtoLP( rt1.p[0]);
//	p2 = RPtoLP( rt1.p[1]);

//	iPenStyle = PSTbl[z_iLineStyle];

//	if (z_bFixedLineWidth)
//		iLineWidth = MGMAX( MINT( z_fLineWidth / m_vsclLPtoDP.x), 1);
//	else
//		iLineWidth = MINT( z_fLineWidth * m_sclRPtoLP);

//	Pen.CreatePen( iPenStyle, iLineWidth, z_rgbLine);
//	pBackup = m_pDC->SelectObject( &Pen);

//	m_pDC->Rectangle( (MINT)p1.x, (MINT)p1.y * m_iUpY, (MINT)p2.x, (MINT)p2.y * m_iUpY);
//	m_pDC->SelectObject( pBackup);
//	Pen.DeleteObject();
//}

/////////////////////////////////////////////////////////////////////////////
//  長方形を表示する
void MbCod::Rect(
				const	MgPoint2	&pa1,
				const	MgPoint2	&pa2
				)
{
	CPoint		p1, p2;
	MINT		iPenStyle;
	CPen		Pen;
	CGdiObject* pBackup;
	MINT		iLineWidth;										// 線幅

	p1 = RPtoLP( pa1);
	p2 = RPtoLP( pa2);

	iPenStyle = PSTbl[z_iLineStyle];

	if (z_bFixedLineWidth)
		iLineWidth = MGMAX( MINT( z_fLineWidth / m_vsclLPtoDP.x), 1);
	else
		iLineWidth = MINT( z_fLineWidth * m_sclRPtoLP);

	Pen.CreatePen(iPenStyle, iLineWidth, z_rgbLine);
	pBackup = m_pDC->SelectObject( &Pen);

//	m_pDC->Rectangle( p1.x, p1.y * m_iUpY, p2.x, p2.y * m_iUpY);
	m_pDC->Rectangle( (MINT)p1.x, (MINT)p1.y * m_iUpY, (MINT)p2.x, (MINT)p2.y * m_iUpY);
	m_pDC->SelectObject( pBackup);
	Pen.DeleteObject();
}

/////////////////////////////////////////////////////////////////////////////
//  折れ線を表示する
void MbCod::Polyline(
						MgPoint2*	pb,
						MINT		inpb
				)
{
	CPoint		p1;
	CPoint		lpb[200];
	MINT		iPenStyle;
	CPen		Pen;
	CGdiObject* pBackup;
	MINT		iLineWidth;										// 線幅

	for (int lpc=0; lpc<inpb; lpc++) {
		p1 = RPtoLP( pb[lpc]);
		lpb[lpc].x = p1.x;
		lpb[lpc].y = p1.y * m_iUpY;
	}
	
	iPenStyle = PSTbl[z_iLineStyle];

	if (z_bFixedLineWidth)
		iLineWidth = MGMAX( MINT( z_fLineWidth / m_vsclLPtoDP.x), 1);
	else
		iLineWidth = MINT( z_fLineWidth * m_sclRPtoLP);


	Pen.CreatePen( iPenStyle, iLineWidth, z_rgbLine);
	pBackup = m_pDC->SelectObject( &Pen);

	m_pDC->Polyline( lpb, inpb);
	m_pDC->SelectObject( pBackup);
	Pen.DeleteObject();
}

/////////////////////////////////////////////////////////////////////////////
//  多角形を表示する
void MbCod::Polygon(
						MgPoint2*	pb,
						int			inpb
				)
{
	MINT		lpc;
	CPoint		p1;
	CPoint		lpb[200];
	MINT		iPenStyle;
	CPen		Pen;
	CGdiObject* pBackup;
	MINT		iLineWidth;										// 線幅

	for (lpc=0; lpc<inpb; lpc++) {
		p1 = RPtoLP( pb[lpc]);
		lpb[lpc].x = p1.x;
		lpb[lpc].y = p1.y * m_iUpY;
	}
	lpb[lpc] = lpb[0];
	inpb++;
	
	iPenStyle = PSTbl[z_iLineStyle];

	if (z_bFixedLineWidth)
		iLineWidth = MGMAX( MINT( z_fLineWidth / m_vsclLPtoDP.x), 1);
	else
		iLineWidth = MINT( z_fLineWidth * m_sclRPtoLP);


	Pen.CreatePen( iPenStyle, iLineWidth, z_rgbLine);
	pBackup = m_pDC->SelectObject( &Pen);

//D	CBrush* pTempBrush = NULL;
//D	CBrush OrigBrush;
//D	// CBrush::CBrush(COLORREF crColor)
//D	CBrush brush2(RGB(255,0,0));   // Solid red brush.
//D	pTempBrush = (CBrush*)m_pDC->SelectObject(&brush2);
//D	// Save original brush.
//D	OrigBrush.FromHandle((HBRUSH)pTempBrush);

	m_pDC->Polyline( lpb, inpb);
	m_pDC->SelectObject( pBackup);

//D	// Reselect original brush into device context.
//D	m_pDC->SelectObject(&OrigBrush);
	Pen.DeleteObject();
}

/////////////////////////////////////////////////////////////////////////////
//  円弧を表示する
void MbCod::Arc1(
				const MgPoint2	&pac,
				MREAL			rh,
				MREAL			a1,
				MREAL			a2
				)
{
	CPoint		pc;
	MREAL		rhw;
	MINT		ihw;

	CPoint		plu, prd, pst, ped;
	MINT		iPenStyle;
	CPen		Pen;
	CGdiObject* pBackup;
	MINT		iLineWidth;										// 線幅

	pc = RPtoLP( pac);
	rhw = m_sclRPtoLP * rh;
	ihw = MINT( rhw);
//	rhw = RPtoLP( rh);
	CPoint pr( ihw, -ihw);
	plu = pc - pr;
	prd = pc + pr;

	MREAL a3 = a1 + a2;
	pst.x = MINT( rhw * (MREAL)cos(a1 * MC_DTR)) + pc.x;
	pst.y = MINT( rhw * (MREAL)sin(a1 * MC_DTR)) + pc.y;
	ped.x = MINT( rhw * (MREAL)cos(a3 * MC_DTR)) + pc.x;
	ped.y = MINT( rhw * (MREAL)sin(a3 * MC_DTR)) + pc.y;

	iPenStyle = PSTbl[z_iLineStyle];

	if (z_bFixedLineWidth)
		iLineWidth = MGMAX( MINT( z_fLineWidth / m_vsclLPtoDP.x), 1);
	else
		iLineWidth = MINT( z_fLineWidth * m_sclRPtoLP);

	Pen.CreatePen( iPenStyle, iLineWidth, z_rgbLine);
	pBackup = m_pDC->SelectObject( &Pen);

	m_pDC->Arc(	plu.x, plu.y * m_iUpY, 
				prd.x, prd.y * m_iUpY, 
				pst.x, pst.y * m_iUpY, 
				ped.x, ped.y * m_iUpY);
	m_pDC->SelectObject( pBackup);
	Pen.DeleteObject();
}

} // namespace MC
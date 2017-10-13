/******************************************************************************************************************
              ＤＸＦ  プログラムファイル (MDxf.cpp)

[機能] 色番号から色を得る
[返値] = 0 正常  < 0エラー
******************************************************************************************************************/

#include	"MDxf.h"

typedef struct _basecolor {
	MINT		Red;
	MINT		Green;
	MINT		Blue;
} RGBSTR;

static RGBSTR BasColor[10] = { {   5,    5,    5 },										// 黒（該当なしのはず）
							   { 250,    5,    5 },										// 赤
							   { 250,  250,    5 },										// 黄
							   {   5,  250,    5 },										// 緑
							   {   5,  250,  250 },										// シアン
							   {   5,    5,  250 },										// 青
							   { 250,    5,  250 },										// マゼンダ
							   { 250,  250,  250 },										// 白
							   {   5,    5,    5 },										// 黒（該当なしのはず）
							   {   5,    5,    5 } } ;									// 黒（該当なしのはず）

static RGBSTR MidColor[24] = { { 250,    5,    5 },										// 赤
							   { 250,   63,    5 },
							   { 250,  127,    5 },										// 橙
							   { 250,  191,    5 },
							   { 250,  250,    5 },										// 黄
							   { 191,  250,    5 },
							   { 127,  250,    5 },										// 黄緑
							   {  63,  250,    5 },
							   {   5,  250,    5 },										// 緑
							   {   5,  250,   63 },
							   {   5,  250,  127 },										// 藍
							   {   5,  250,  191 },
							   {   5,  250,  250 },										// シアン
							   {   5,  191,  250 },
							   {   5,  127,  250 },										// 青緑
							   {   5,   63,  250 },
							   {   5,    5,  250 },										// 青
							   {  63,    5,  250 },
							   { 127,    5,  250 },										// 紫
							   { 191,    5,  250 },
							   { 250,    5,  250 },										// マゼンダ
							   { 250,    5,  191 },
							   { 250,    5,  127 },										// アズキ
							   { 250,    5,   63 } };


COLORREF MDxfGetCol           (														// (  O) RGBｶﾗｰ値
								MINT				Color)								// (I  ) 色番号
{
	COLORREF			Rgb;															// RGBｶﾗｰ値
	MINT				Red;															// 赤
	MINT				Green;															// 緑
	MINT				Blue;															// 青


	MDxfGetCol(Color, &Red, &Green, &Blue);

	Rgb = RGB(Red, Green, Blue);

	return(Rgb);
}

HVOID MDxfGetCol              (
								MINT				Color,								// (I  ) 色番号
								MINT*				Red,								// (  O) 赤
								MINT*				Green,								// (  O) 緑
								MINT*				Blue)								// (  O) 青
{
	MINT				Id;																// 割り当て番号（２桁）
	MINT				Kido;															// 輝度（１桁）


	if (Color <=0 || Color > 255)  goto  PRG_EXIT;										// 色番号ﾁｪｯｸ

	// 色番号から割り当て番号と輝度を得る
	Id   = Color / 10;
	Kido = Color % 10;

	if (Id == 0) {																		// 基本色
		*Red   = BasColor[Kido].Red;
		*Green = BasColor[Kido].Green;
		*Blue  = BasColor[Kido].Blue;
	} else if (Id == 25) {																// ｸﾞﾚｰ系を６階調に分ける
		*Red   = MINT(1.0f * Kido / 6.0f * 250);
		*Green = MINT(1.0f * Kido / 6.0f * 250);
		*Blue  = MINT(1.0f * Kido / 6.0f * 250);
	} else {
		if (Kido % 2 == 0) {															// 黒と混合
			*Red   = MINT(MidColor[Id-1].Red   * (1.0f - Kido / 10.0f));
			*Green = MINT(MidColor[Id-1].Green * (1.0f - Kido / 10.0f));
			*Blue  = MINT(MidColor[Id-1].Blue  * (1.0f - Kido / 10.0f));
		} else {																		// 白色混合
			/* 式：  ( RGB + 1.0(White) ) / 2 * ( 1 - ( Kido - 1 )/10 )
				     ------------------------   -----------------------
					   白色混合の色             輝度(1が明るく9が暗いので1のとき最大) */
			*Red   = MINT((MidColor[Id-1].Red   + 250) * 0.5f * (1.1f - Kido / 10.0f));
			*Green = MINT((MidColor[Id-1].Green + 250) * 0.5f * (1.1f - Kido / 10.0f));
			*Blue  = MINT((MidColor[Id-1].Blue  + 250) * 0.5f * (1.1f - Kido / 10.0f));
		}
	}

PRG_EXIT:

	return;
}

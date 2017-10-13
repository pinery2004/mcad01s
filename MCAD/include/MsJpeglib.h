#pragma once
/********************************************************************************************************************************

              ＪＰｅｇ変換  ヘッダファイル (HsJpegLib.h)

********************************************************************************************************************************/

#include		<setjmp.h>
#include		"MsJpegLib.h"

// 定数

// ディザリングの種類
#define		HJPDTNONE		0															// ﾃﾞｨｻﾞﾘﾝｸﾞを行わない
#define		HJPDTFS  		1															// ﾌﾛｲﾄﾞ･ｽﾀｲﾝﾍﾞﾙｸﾞ方式
#define		HJPDTORD		2															// ｵｰﾀﾞﾘﾝｸﾞ方式

// JPEGファイルのイメージ変換の種類

#define		HJPGPROG		0															// ﾌﾟﾛｸﾞﾚｯｼﾌﾞJPEGﾌｧｲﾙを作成する
#define		HJPGGRAY		1															// ｸﾞﾚｰｽｹｰﾙに変換する
#define		HJPGHORMIR		10															// 水平方向にﾐﾗｰ反転させる（左右反転）
#define		HJPGVERMIR		11															// 垂直方向にﾐﾗｰ反転させる（上下反転）
#define		HJPGROT90		20															// 時計回りに90度回転させる
#define		HJPGROT180		21															// 時計回りに180度回転させる
#define		HJPGROT270		22															// 時計回りに270度回転させる（反時計回りに90度）
#define		HJPGTRNSP		31															// 左上と右下を結ぶ対角線を軸として反転させる
#define		HJPGTRNSV		32															// 左下と右上を結ぶ対角線を軸として反転させる

//HHHH
// 画像圧縮オプション
typedef struct _picmprsvars {
	MUINT	Quality;																	// JPEG：品質(0～100,ﾃﾞﾌｫﾙﾄ85)
	MUINT	Smooth;																		// JPEG：ｽﾑｰｼﾞﾝｸﾞ(0～100,デフォルト0)
} PiCmprsVars;

// ディザリング・オプション
typedef struct _pirdcvars {
	MUINT	Colors;																		// 色数
	MUINT	DitherOpt;																	// ﾃﾞｨｻﾞﾘﾝｸﾞの種類
} PiRdcVars;

// ＢＭＰファイルのヘッダ情報
typedef struct _pgbmpflhead {
	MSHORT		Type;																	// BMという文字のASCII値
	MINT		Size;																	// ﾌｧｲﾙのｻｲｽﾞ
	MSHORT		Reserved1;																// 常に0
	MSHORT		Reserved2;																// 常に0
	MINT		OffBit;																	// ﾌｧｲﾙの始まりからﾋﾞｯﾄﾏｯﾌﾟまでのﾊﾞｲﾄ数
} PGBMPFLHEAD;
typedef struct _pgbmpinfhead {
	MINT		Size;																	// この構造体のﾊﾞｲﾄｻｲｽﾞ
	MINT		Width;																	// ﾋﾞｯﾄﾏｯﾌﾟの幅のﾋﾟｸｾﾙ値
	MINT		Height;																	// ﾋﾞｯﾄﾏｯﾌﾟの高さのﾋﾟｸｾﾙ値
	MSHORT		Planes;																	// 常に1
	MSHORT		BitCount;																// 1ﾋﾟｸｾﾙあたりのﾋﾞｯﾄ数
	MINT		Compression;															// 圧縮の種類 常に0
	MINT		SizeImage;																// ﾋﾞｯﾄﾏｯﾌﾟのｻｲｽﾞを表すﾊﾞｲﾄ数	
	MINT		XPixPerMeter;															// 1ﾒｰﾄﾙあたりの水平方向ﾋﾟｸｾﾙ数
	MINT		YPixPerMeter;															// 1ﾒｰﾄﾙあたりの垂直方向ﾋﾟｸｾﾙ数
	MINT		ColUsed;																// 使用される色の数
	MINT		ColImprtant;															// 重要な色の数
} PGBMPINFHEAD;

// カラーマップ部
typedef struct _pgrgbquad {
	MBYTE		Blue;																	// ｶﾗｰﾏｯﾌﾟｴﾝﾄﾘの青の値
	MBYTE		Green;																	// ｶﾗｰﾏｯﾌﾟｴﾝﾄﾘの緑の値
	MBYTE		Red;																	// ｶﾗｰﾏｯﾌﾟｴﾝﾄﾘの赤の値
	MBYTE		Reserved;																// 常に0
} PGRGBQUAD;

// ＢＭＰファイル
typedef struct _pgbmpfile {
	PGBMPFLHEAD		FileHead;															// ﾌｧｲﾙﾍｯﾀﾞ部
	PGBMPINFHEAD	InfHead;															// ﾋﾞｯﾄﾏｯﾌﾟﾍｯﾀﾞ部
	PGRGBQUAD		RgbQuad[256];														// ｶﾗｰﾏｯﾌﾟ部
} PGBMPFILE;


// IJG のエラーハンドリング用定義
typedef struct _pi_ijg_error_mgr {
	struct jpeg_error_mgr	pub;														// "public"フィールド
	jmp_buf					setjmp_buffer;												// 呼び出し側へのﾘﾀｰﾝ用
} PiIjgErrorMgr;

typedef struct _pi_ijg_error_mgr*	PiIjgErrorPtr;

/******************************************************************************************************************
[ 機能 ] 非公開関数のプロトタイプ宣言 (IJG の関数内のエラーを上位関数に返す)
[ 返値 ] なし
******************************************************************************************************************/
void MsJpegErrExit            (
								j_common_ptr		cinfo);								// (I  ) JPEG圧縮･伸張のﾊﾟﾗﾒｰﾀ用構造体へのﾎﾟｲﾝﾀ


/////////////////////////////////////////////////////////////////////////////
//		JPEGファイルをBMAPフォーマットに変換
MINT MsJpegJpgToBmp(															// (  O) ｽﾃｰﾀｽ
								MCHAR*				i_FLName1,					// (I  ) 変換元画像ﾌｧｲﾙ名
								MCHAR*				i_FLName2,					// (I  ) 変換後画像ﾌｧｲﾙ名
								MINT				i_MFlag     = 0,			// (I  ) 1:左右ﾐﾗｰ
								MREAL				i_Scale     = 100,			// (I  ) 画像ｻｲｽﾞの縮小率
								MUINT				i_Colors    = 0,			// (I  ) 色数（==0：削減しない）
								MUINT				i_DitherOpt = 0);			// (I  ) ﾃﾞｨｻﾞﾘﾝｸﾞの種類

/////////////////////////////////////////////////////////////////////////////
//		BMAPファイルをJPEGフォーマットに圧縮・変換
MINT MsJpegBmpToJpg(															// (  O) ｽﾃｰﾀｽ
								MCHAR*				i_FLName1,					// (I  ) 変換元画像ﾌｧｲﾙ名
								MCHAR*				i_FLName2,					// (I  ) 変換後画像ﾌｧｲﾙ名
								MUINT				i_Quality = 100,			// (I  ) 圧縮率   (0～100)
								MUINT				i_Smooth  = 100);			// (I  ) ｽﾑｰｼﾞﾝｸﾞ (0～100)

/////////////////////////////////////////////////////////////////////////////
//		IJG 内部でエラーが発生した場合の処理
void MsJpegErrExit(
								j_common_ptr		i_cinfo);					// (I ) JPEG圧縮･伸張のﾊﾟﾗﾒｰﾀ用構造体へのﾎﾟｲﾝﾀ

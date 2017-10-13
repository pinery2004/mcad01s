/******************************************************************************************************************
[ 機能 ] BMAPファイルをJPEGフォーマットに圧縮・変換
[ 返値 ] ステータス        0 : 正常     -1 : 異常
******************************************************************************************************************/
#include "stdafx.h"
#define			 XMD_H																	/* X11/xmd.h correctly defines INT32 */

#include		"cdjpeg.h"																// IJG ヘッダファイル
#include		"MbBasic.h"
#include		"MsJpegLib.h"

//HHHH
MINT MsJpegBmpToJpg(							// (  O) ｽﾃｰﾀｽ
						MCHAR*	FLName1,		// (I  ) 変換元画像ﾌｧｲﾙ名
						MCHAR*	FLName2,		// (I  ) 変換後画像ﾌｧｲﾙ名
						MUINT	Quality,		// (I  ) 圧縮率   (0～100)
						MUINT	Smooth)			// (I  ) ｽﾑｰｼﾞﾝｸﾞ (0～100)
{
	MINT						Status;			// ｽﾃｰﾀｽ
	struct jpeg_compress_struct	CInfo;			// JPEG圧縮のﾊﾟﾗﾒｰﾀを含む構造体
	PiIjgErrorMgr				JpErr;			// IJG のｴﾗｰを DigiD に返す仕組み
	cjpeg_source_ptr			SrcMgr;			// 入力ﾌｧｲﾙの管理用構造体
	FILE*						InFile;			// 入力ﾌｧｲﾙﾎﾟｲﾝﾀ
	FILE*						OutFile;		// 出力ﾌｧｲﾙﾎﾟｲﾝﾀ
	JDIMENSION					NScan;			//


	Status = -1;

	// ファイルを開く
	if ((InFile = Mfopen( FLName1, READ_BINARY)) == NULL)    goto  PRG_EXIT;

	if ((OutFile = Mfopen( FLName2, WRITE_BINARY)) == NULL) {
		fclose( InFile);
		goto  PRG_EXIT;
	}

	// エラー処理の初期化
	CInfo.err = jpeg_std_error(&JpErr.pub);
	JpErr.pub.error_exit = MsJpegErrExit;												// ｴﾗｰ処理を置き換え
	if (setjmp(JpErr.setjmp_buffer)) {													// ｴﾗｰｼｸﾞﾅﾙ発生
		jpeg_destroy_compress(&CInfo);
		fclose(InFile); fclose(OutFile);
		goto  PRG_EXIT;
	}

	jpeg_create_compress(&CInfo);														// JPEG圧縮のﾊﾟﾗﾒｰﾀを初期化

	CInfo.in_color_space = JCS_RGB;														// 仮の値
	jpeg_set_defaults(&CInfo);															// ﾃﾞﾌｫﾙﾄのﾊﾟﾗﾒｰﾀを設定

	SrcMgr = jinit_read_bmp(&CInfo);													// BMPファイル読込み設定
	SrcMgr->input_file = InFile;

	(*SrcMgr->start_input) (&CInfo, SrcMgr);											// 入力ﾌｧｲﾙのﾍｯﾀﾞからﾌｧｲﾙｻｲｽﾞとｶﾗｰｽﾍﾟｰｽを得る

	jpeg_default_colorspace(&CInfo);													// ｶﾗｰｽﾍﾟｰｽに依存するﾃﾞﾌｫﾙﾄ情報を再設定

	CInfo.smoothing_factor = Smooth;													// ｽﾑｰｼﾞﾝｸﾞ
	jpeg_set_quality( &CInfo,Quality, true);											// 圧縮率

	jpeg_stdio_dest(&CInfo, OutFile);													// 出力先ﾌｧｲﾙを設定

	// 圧縮処理開始
	jpeg_start_compress(&CInfo, TRUE);

	// ﾃﾞｰﾀ出力/
	while (CInfo.next_scanline < CInfo.image_height) {
		NScan = (*SrcMgr->get_pixel_rows) (&CInfo, SrcMgr);
		(void) jpeg_write_scanlines(&CInfo, SrcMgr->buffer, NScan);
	}

	// 終了処理およびﾒﾓﾘの解放
	(*SrcMgr->finish_input) (&CInfo, SrcMgr);
	jpeg_finish_compress(&CInfo);
	jpeg_destroy_compress(&CInfo);

	// ﾌｧｲﾙ ｸﾛｰｽﾞ
	fclose(InFile);
	fclose(OutFile);

	// 警告ﾁｪｯｸ
	if (JpErr.pub.num_warnings) {
		Status = JpErr.pub.num_warnings + 1;
	} else {
		Status = 0;
	}

PRG_EXIT:

	return(Status);
}

/******************************************************************************************************************
[ 機能 ] JPEGファイルをBMAPフォーマットに変換
[ 返値 ] ステータス        0 : 正常     -1 : 異常
******************************************************************************************************************/
#include "stdafx.h"
#define			 XMD_H																	/* X11/xmd.h correctly defines INT32 */

#include		"cdjpeg.h"																// IJG ヘッダファイル
#include		"MbBasic.h"
#include		"MsJpegLib.h"

//HHHH
MINT MsJpegJpgToBmp            (														// (  O) ｽﾃｰﾀｽ
								MCHAR*				FLName1,							// (I  ) 変換元画像ﾌｧｲﾙ名
								MCHAR*				FLName2,							// (I  ) 変換後画像ﾌｧｲﾙ名
								MINT				MFlag,								// (I  ) 1:左右ﾐﾗｰ
								MREAL				Scale,								// (I  ) 画像ｻｲｽﾞの縮小率
								MUINT				Colors,								// (I  ) 色数（==0：削減しない）
								MUINT				DitherOpt)							// (I  ) ﾃﾞｨｻﾞﾘﾝｸﾞの種類
{
	MINT							Status;
	struct jpeg_decompress_struct	CInfo;												// JPEG伸張のﾊﾟﾗﾒｰﾀを含む構造体
	PiIjgErrorMgr					JpErr;												// IJG のｴﾗｰを DigiD に返す仕組み
	djpeg_dest_ptr					DstMgr;												// 入力ファイルの管理用構造体
	FILE*							InFile;												// 入力ﾌｧｲﾙﾎﾟｲﾝﾀ
	FILE*							OutFile;											// 出力ﾌｧｲﾙﾎﾟｲﾝﾀ
	JDIMENSION						NScan;												//


	Status = -1;

	// ファイルを開く
	if ( ( InFile = Mfopen( FLName1, READ_BINARY)) == NULL)
		goto  PRG_EXIT;

	if ( ( OutFile = Mfopen(FLName2, WRITE_BINARY)) == NULL) {
		fclose(InFile);
		goto  PRG_EXIT;
	}


	// エラー処理の初期化
	CInfo.err = jpeg_std_error(&JpErr.pub);
	JpErr.pub.error_exit = MsJpegErrExit;												// ｴﾗｰ処理を置き換え
	if (setjmp(JpErr.setjmp_buffer)) {													// ｴﾗｰｼｸﾞﾅﾙ発生
		jpeg_destroy_decompress(&CInfo);
		fclose(InFile); fclose(OutFile);
		goto  PRG_EXIT;
	}

	jpeg_create_decompress(&CInfo);														// JPEG伸張のためのﾊﾟﾗﾒｰﾀを初期化


	jpeg_stdio_src(&CInfo, InFile);														// 入力元ﾌｧｲﾙを設定
	(void)jpeg_read_header(&CInfo, TRUE);												// ﾌｧｲﾙﾍｯﾀﾞを読込む

	// スケール  実数で指定された値を、1/1、1/2、1/4、1/8のいずれかに近似
	if (Scale <= (MREAL)0.1875 ) {														// 1/8
		CInfo.scale_num   = 1;
		CInfo.scale_denom = 8;	
	} else if ( Scale <= (MREAL)0.375) {												// 1/4
		CInfo.scale_num   = 1;
		CInfo.scale_denom = 4;
	} else if ( Scale <= (MREAL)0.75) {													// 1/2
		CInfo.scale_num   = 1;
		CInfo.scale_denom = 2;
	} else {																			// 1/1
		CInfo.scale_num   = 1;
		CInfo.scale_denom = 1;
	}

	// 色数の削減（量子化）
	if (Colors > 0) {
		CInfo.quantize_colors          = TRUE;
		CInfo.desired_number_of_colors = Colors;
	}

	// ディザリングの指定
	if (DitherOpt == HJPDTORD) {														// ｵｰﾀﾞﾘﾝｸﾞ方式
		CInfo.dither_mode = JDITHER_ORDERED;
	} else if (DitherOpt == HJPDTFS) {													// ﾌﾛｲﾄﾞ･ｽﾀｲﾝﾍﾞﾙｸﾞ方式
		CInfo.dither_mode = JDITHER_FS;			
	} else {																			// ﾃﾞｨｻﾞﾘﾝｸﾞを行わない（未定義の値を含む）
		CInfo.dither_mode = JDITHER_NONE;
	}

	// 出力ファイルの準備
	DstMgr = jinit_write_bmp(&CInfo,FALSE);

	DstMgr->output_file = OutFile;

	// 伸張処理開始
	(void) jpeg_start_decompress(&CInfo);

	// ﾃﾞｰﾀ出力
	(*DstMgr->start_output) (&CInfo, DstMgr);											// 出力ﾌｧｲﾙのﾍｯﾀﾞ書き出し

	while (CInfo.output_scanline < CInfo.output_height) {
		NScan = jpeg_read_scanlines(&CInfo, DstMgr->buffer, DstMgr->buffer_height);
		(*DstMgr->put_pixel_rows) (&CInfo, DstMgr, NScan, MFlag);
	}

	// 終了処理およびﾒﾓﾘの解放
	(*DstMgr->finish_output) (&CInfo, DstMgr);
	(void) jpeg_finish_decompress(&CInfo);
	jpeg_destroy_decompress(&CInfo);


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

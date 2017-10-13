/******************************************************************************************************************
[ 機能 ] IJG 内部でエラーが発生した場合の処理
[ 返値 ] なし
******************************************************************************************************************/
#include "stdafx.h"
#define			 XMD_H																	/* X11/xmd.h correctly defines INT32 */

#include		"cdjpeg.h"																// IJG ヘッダファイル
#include		"MbBasic.h"
#include		"MsLib.h"
#include		"MsJpegLib.h"

//HHHH
void MsJpegErrExit(
								j_common_ptr		cinfo)								// (I ) JPEG圧縮･伸張のﾊﾟﾗﾒｰﾀ用構造体へのﾎﾟｲﾝﾀ
{
	PiIjgErrorPtr		myerr;
	char				buffer[JMSG_LENGTH_MAX];
//	MCHAR*				uni_buffer;


	/* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
	myerr = (PiIjgErrorPtr) cinfo->err;

	/* エラーメッセージの表示 */
	(*cinfo->err->format_message) ( cinfo, buffer, sizeof(buffer));

	/* Return control to the setjmp point */
	longjmp(myerr->setjmp_buffer, 1);

}

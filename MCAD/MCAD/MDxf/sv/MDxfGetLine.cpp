/******************************************************************************************************************
              ＤＸＦ  プログラムファイル (MDxf.cpp)

[機能] ＤＸＦファイルより２ライン読込み編集する （コメント行は読み飛ばす）
[返値] = 0 正常  < 0エラー
******************************************************************************************************************/

#include		"HsFileLib.h"
#include		"MDxf.h"


MINT MDxfGetLine          (														// (  O) ｽﾃｰﾀｽ
							CStdioFile*			FLPtr,								// (I  ) ﾌｧｲﾙ識別子
							MINT*				Code,								// (  O) ｸﾞﾙｰﾌﾟｺｰﾄﾞ
							MCHAR*				Data)								// (  O) 文字列
{
	MINT			Status;															// ｽﾃｰﾀｽ
	MCHAR			RBuff[512];														// 読込みﾊﾞｯﾌｧ


	Status = 1;

	while (1) {
		Status = HsFileReadS(FLPtr, 511, RBuff);									// 1行読込む
		if (Status < 0)  break;
		if ((*Code = BwsAtoI(RBuff)) < 0)  break;

		Status = HsFileReadS(FLPtr, 511, RBuff);									// 1行読込む
		if (Status < 0)  break;

		if (*Code == 999)  continue;												// ｺﾒﾝﾄ読飛ばし

		BwsSscanf(RBuff, BwsStr("%s"), Data);
		Status = 0;
		break;
	}

	return(Status);
}

/******************************************************************************************************************
              ＤＸＦ  プログラムファイル (MDxf.cpp)

[機能] ヘッダセクションを読み込み、円弧の回転方向データを変数に設定する
[返値] = 0 正常  < 0エラー
******************************************************************************************************************/


#include	"MDxf.h"


MINT MDxfGetHdSec         (														// (  O) ｽﾃｰﾀｽ
							CStdioFile*			FLPtr,								// (I  ) ﾌｧｲﾙ識別子
							MDxfInf*			DxfInf)								// (I/O) DXF情報
{
	MINT			Status;															// ｽﾃｰﾀｽ
	MINT			Code;
	MCHAR			Data[512];
	MINT			Dir;															// ANGDIR変数の値


	Status = 0;

	while (MDxfGetLine(FLPtr, &Code, Data) == 0) {
		if (BwsStrCmp(Data, BwsStr("ENDSEC")) == 0) {
			 break;
		} else if (BwsStrCmp(Data, BwsStr("$ANGDIR")) == 0) {						// 角度方向の設定
			if (MDxfGetLine(FLPtr, &Code, Data) != 0) {
				Status = -1;
				break;
			}
			if (Code == 70) {
				BwsSscanf(Data, BwsStr("%d"), &Dir);
				if (Dir == 1) {														// 時計方向
					DxfInf->m_AngDir = -1;
				} else if (Dir == 0) {												// 反時計方向
					DxfInf->m_AngDir = 1;
				}
			} else {																// $ANGDIRに不適切ｸﾞﾙｰﾌﾟｺｰﾄﾞ
				Status = -1;
				break;
			}
		}
	}

	return(Status);
}

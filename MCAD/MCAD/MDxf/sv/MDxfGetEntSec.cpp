/******************************************************************************************************************
              ＤＸＦ  プログラムファイル (MDxf.cpp)

[機能] エンティティセクションを読込み、プリミティブに展開する
[返値] = 0 正常  < 0エラー
******************************************************************************************************************/


#include	"MDxf.h"


MINT MDxfGetEntSec        (														// (  O) ｽﾃｰﾀｽ
							CStdioFile*			FLPtr,								// (I  ) ﾌｧｲﾙ識別子
							MDxfInf*			DxfInf)								// (I/O) DXF情報
{
	MINT			Status;															// ｽﾃｰﾀｽ
	MINT			Code;
	MCHAR			Data[512];


	if ((Status = MDxfGetLine(FLPtr, &Code, Data)) != 0)  goto  PRG_EXIT;

	while (1) {
		if (BwsStrCmp(Data, BwsStr("ENDSEC")) == 0)  break;							// ｾｸｼｮﾝ終了

		// 分離記号でエンティティタイプ別処理
		if (Code == 0) {
			if (BwsStrCmp(Data, BwsStr("LINE")) == 0) {
				Status = MDxfGetPrmLin(FLPtr, Data, DxfInf, NULL);
				if (Status < 0) break;
				continue;															// 先読みしているので読み込まない
			} else if (BwsStrCmp(Data, BwsStr("POLYLINE")) == 0) {
				Status = MDxfGetPrmPln(FLPtr, Data, DxfInf, NULL);
				if (Status < 0)  break;
				continue;															// 先読みしているので読み込まない
			} else if (BwsStrCmp(Data, BwsStr("POINT")) == 0) {
				Status = MDxfGetPrmDot(FLPtr, Data, DxfInf, NULL);
				if (Status < 0 )  break;
				continue;															// 先読みしているので読み込まない
			} else if (BwsStrCmp(Data, BwsStr("CIRCLE")) == 0) {
				Status = MDxfGetPrmCir(FLPtr, Data, DxfInf, NULL);
				if (Status < 0)  break;
				continue;															// 先読みしているので読み込まない
			} else if (BwsStrCmp(Data,  BwsStr("ARC")) == 0) {
				Status = MDxfGetPrmArc(FLPtr, Data, DxfInf, NULL);
				if (Status < 0)  break;
				continue;															// 先読みしているので読み込まない
			} else if (BwsStrCmp(Data,  BwsStr("TEXT") ) == 0) {
				Status = MDxfGetPrmTxt(FLPtr, Data, DxfInf, NULL);
				if (Status < 0)  break;
				continue;															// 先読みしているので読み込まない
			} else if (BwsStrCmp(Data, BwsStr("ATTRIB")) == 0) {
				Status = MDxfGetPrmAtb(FLPtr, Data, DxfInf, NULL);
				if (Status < 0)  break;
				continue;															// 先読みしているので読み込まない
			} else if (BwsStrCmp(Data, BwsStr("3DFACE")) == 0) {
				Status = MDxfGetPrmFac(FLPtr, Data, DxfInf, NULL);
				if (Status < 0)  break;
				continue;															// 先読みしているので読み込まない
			} else if (BwsStrCmp(Data, BwsStr("INSERT")) == 0) {
				Status = MDxfGetPrmIns(FLPtr, Data, DxfInf, NULL);
				if (Status < 0)  break;
				continue;															// 先読みしているので読み込まない
			} else {																// その他のｴﾝﾃｨﾃｨは未対応
				//
			}
		}
		if ((Status = MDxfGetLine(FLPtr, &Code, Data)) < 0)  goto  PRG_EXIT;
	}

PRG_EXIT:

	return(Status);
}

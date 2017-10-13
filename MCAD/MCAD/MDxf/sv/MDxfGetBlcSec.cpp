/******************************************************************************************************************
              ＤＸＦ  プログラムファイル (MDxf.cpp)

[機能] ブロックセクションを読込み、ブロックテーブルを作成する
[返値] = 0 正常  < 0エラー
******************************************************************************************************************/


#include	"MDxf.h"


MINT MDxfGetBlcSec        (														// (  O) ｽﾃｰﾀｽ
							CStdioFile*			FLPtr,								// (I  ) ﾌｧｲﾙ識別子
							MDxfInf*			DxfInf)								// (I/O) DXF情報
{
	MINT			Status;															// ｽﾃｰﾀｽ
	MINT			Code;
	MCHAR			Data[512];
	MDxfBlock*		BlkPtr = NULL;
	MCHAR			Name[NAMMAX];
	MCHAR			LyName[NAMMAX];
	MgPoint3			InsPnt;
	HBOOL			GetTbl;															// ﾃｰﾌﾞﾙ読込みﾌﾗｸﾞ


	Status = 0;

	while  ((Status = MDxfGetLine(FLPtr, &Code, Data)) == 0) {
		if (BwsStrCmp(Data, BwsStr("ENDSEC")) == 0) {								// ｾｸｼｮﾝ終了
			break;
		} else if (BwsStrCmp(Data, BwsStr("BLOCK")) == 0) {							// ﾌﾞﾛｯｸの作成
			BlkPtr = DxfInf->AddBlock();											// ﾌﾞﾛｯｸ情報追加

			GetTbl = HFALSE;
			if ((Status = MDxfGetLine(FLPtr, &Code, Data)) != 0)  break;

			while  (1) {
				if (BwsStrCmp(Data, BwsStr("ENDBLK")) == 0) {						// ﾌﾞﾛｯｸ終了
					if (GetTbl == HTRUE) {
						BlkPtr->SetInf(Name, LyName, 0, 0, &InsPnt);				// ﾌﾞﾛｯｸ情報をｾｯﾄ
					}
					break;
				}

				// データの取得
				if (Code == 2) {													// ﾌﾞﾛｯｸ名
					GetTbl = HTRUE;
					BwsSscanf(Data, BwsStr("%s"), Name);
				} else if (Code == 8) {												// ﾚｲﾔ名
					GetTbl = HTRUE;
					BwsSscanf(Data, BwsStr("%s"), LyName);
				} else if (Code == 10) {											// 挿入基点Ｘ
					GetTbl = HTRUE;
					BwsSscanf(Data, BwsStr("%f"), &InsPnt.X);
				} else if (Code == 20) {											// 挿入基点Ｙ
					GetTbl = HTRUE;
					BwsSscanf(Data, BwsStr("%f"), &InsPnt.Y);
				} else if (Code == 30) {											// 挿入基点Ｚ
					GetTbl = HTRUE;
					BwsSscanf(Data, BwsStr("%f"), &InsPnt.Z);
				} else if (Code == 0) {												// 区切り文字
					if (BwsStrCmp(Data, BwsStr("ENDBLK")) == 0)  continue;			// ﾌﾞﾛｯｸ終了

					// 図形情報を得る
					if (BwsStrCmp(Data, BwsStr("LINE")) == 0) {
						Status = MDxfGetPrmLin(FLPtr, Data, DxfInf, BlkPtr);
						if (Status < 0) break;
						continue;													// 先読みしているので読み込まない

					} else if (BwsStrCmp(Data, BwsStr("POLYLINE")) == 0) {
						Status = MDxfGetPrmPln(FLPtr, Data, DxfInf, BlkPtr);
						if (Status < 0)  break;
						continue;													// 先読みしているので読み込まない

					} else if (BwsStrCmp(Data, BwsStr("POINT")) == 0) {
						Status = MDxfGetPrmDot(FLPtr, Data, DxfInf, BlkPtr);
						if (Status < 0 )  break;
						continue;													// 先読みしているので読み込まない

					} else if (BwsStrCmp(Data, BwsStr("CIRCLE")) == 0) {
						Status = MDxfGetPrmCir(FLPtr, Data, DxfInf, BlkPtr);
						if (Status < 0)  break;
						continue;													// 先読みしているので読み込まない

					} else if (BwsStrCmp(Data, BwsStr("ARC")) == 0) {
						Status = MDxfGetPrmCir(FLPtr, Data, DxfInf, BlkPtr);
						if (Status < 0)  break;
						continue;													// 先読みしているので読み込まない

					} else if (BwsStrCmp(Data, BwsStr("TEXT")) == 0 ) {
						Status = MDxfGetPrmTxt(FLPtr, Data, DxfInf, BlkPtr);
						if (Status < 0)  break;
						continue;													// 先読みしているので読み込まない

					} else if (BwsStrCmp(Data, BwsStr("ATTRIB")) == 0) {
						Status = MDxfGetPrmAtb(FLPtr, Data, DxfInf, BlkPtr);
						if (Status < 0) break;
						continue;													// 先読みしているので読み込まない

					} else if (BwsStrCmp(Data, BwsStr("3DFACE")) == 0) {
						Status = MDxfGetPrmFac(FLPtr, Data, DxfInf, BlkPtr);
						if (Status < 0 )  break;
						continue;													// 先読みしているので読み込まない

					} else if (BwsStrCmp(Data, BwsStr("INSERT")) == 0) {
						Status = MDxfGetPrmIns(FLPtr, Data, DxfInf, BlkPtr);
						if (Status < 0)  break;
						continue;													// 先読みしているので読み込まない
					}
				}

				if ((Status = MDxfGetLine(FLPtr, &Code, Data)) != 0)  break;
			}
		}
	}

	return(Status);

}

/******************************************************************************************************************
              ＤＸＦ  プログラムファイル (MDxf.cpp)

[機能] フォントテーブルの読込みと設定
[返値] = 0 正常  < 0エラー
******************************************************************************************************************/


#include	"MDxf.h"


MINT MDxfGetTblFont       (														// (  O) ｽﾃｰﾀｽ
							CStdioFile*			FLPtr,								// (I  ) ﾌｧｲﾙ識別子
							MDxfInf*			DxfInf)								// (I/O) DXF情報
{
	MINT			Status;															// ｽﾃｰﾀｽ
	MINT			Code;
	MCHAR			Data[512];
	MCHAR			Name[NAMMAX];
	MINT			FCode;
	HBOOL			GetTbl;															// ﾃｰﾌﾞﾙ読込みﾌﾗｸﾞ


	GetTbl = HFALSE;
	BwsStrCpy(Name, BwsStr(""));

	// フォントテーブルの読み込み
	while  ((Status = MDxfGetLine(FLPtr, &Code, Data)) == 0) {
		if (Code == 2) {															// ﾌｫﾝﾄ名
			GetTbl = HTRUE;
			BwsSscanf(Data, BwsStr("%s"), Name);
		} else if (Code == 70) {													// 標準ﾌﾗｸﾞ(通常64)
			GetTbl = HTRUE;
			BwsSscanf(Data, BwsStr("%d"), &FCode);
		} else if (Code == 0) {														// 区切り文字
			if (GetTbl == HTRUE) {
				DxfInf->AddFonts(Name, FCode);										// ﾌｫﾝﾄ情報追加
				GetTbl = HFALSE;													// 次ﾃﾞｰﾀに向け初期化
			}
			if (BwsStrCmp(Data, BwsStr("ENDTAB")) == 0)  break;						// ﾌｫﾝﾄﾃｰﾌﾞﾙ終了
		}
	}

	return(Status);
}
/******************************************************************************************************************
[機能] 線種テーブルの読込みと設定
[返値] = 0 正常  < 0エラー
******************************************************************************************************************/
MINT MDxfGetTblLin        (														// (  O) ｽﾃｰﾀｽ
							CStdioFile*			FLPtr,								// (I  ) ﾌｧｲﾙ識別子
							MDxfInf*			DxfInf)								// (I/O) DXF情報
{
	MINT			Status;															// ｽﾃｰﾀｽ
	MINT			Code;
	MCHAR			Data[512];
	MCHAR			Name[NAMMAX];
	MINT			LineNo;
	HBOOL			GetTbl;															// ﾃｰﾌﾞﾙ読込みﾌﾗｸﾞ
	MINT			DashNum;														// ﾀﾞｯｼｭ長さ(ｸﾞﾙｰﾌﾟｺｰﾄﾞ49)ﾃﾞｰﾀ数


	GetTbl  = HFALSE;
	DashNum = 0;

	// 線種テーブルの読み込み
	while  ((Status = MDxfGetLine(FLPtr, &Code, Data)) == 0) {
		if (Code == 2) {															// 線種名
			GetTbl = HTRUE;
			BwsSscanf(Data, BwsStr("%s"), Name);
		} else if (Code == 49) {													// ﾀﾞｯｼｭ長さ
			DashNum++;
		} else if (Code == 0) {														// 区切り文字
			if (GetTbl == HTRUE)	{
				// 線種の判定
				switch  (DashNum) {
					case  0:														// 実線
						LineNo = HLINE_SOLD;										// (1)
						break;
					case  2:														// 破線
						LineNo = HLINE_DOT;											// (4)
						break;
					case  4:														// 一点鎖線
						LineNo = HLINE_DASH1PS;										// (5)
						break;
					default:														// 二点鎖線
						LineNo = HLINE_DASH2P;										// (7)
						break;
				}

				DxfInf->AddLines(Name, LineNo);										// 線情報追加

				GetTbl  = HFALSE;													// 次ﾃﾞｰﾀのため初期化
				DashNum = 0;

			}
			if (BwsStrCmp(Data, BwsStr("ENDTAB")) == 0)  break; 					// 線種ﾃｰﾌﾞﾙ終了
		}
	}

	return(Status);
}
/******************************************************************************************************************
[機能] レイヤテーブルの読込みと設定
[返値] = 0 正常  < 0エラー
******************************************************************************************************************/
MINT MDxfGetTblLyr            (													// (  O) ｽﾃｰﾀｽ
								CStdioFile*			FLPtr,							// (I  ) ﾌｧｲﾙ識別子
								MDxfInf*			DxfInf)							// (I/O) DXF情報
{
	MINT			Status;															// ｽﾃｰﾀｽ
	MINT			Code;
	MCHAR			Data[512];
	MCHAR			Name[NAMMAX];
	MINT			Color;
	MINT			LineNo;
	MDxfLines*		LinPtr;
	HBOOL			GetTbl;															// ﾃｰﾌﾞﾙ読込みﾌﾗｸﾞ


	GetTbl        = HFALSE;

	// レイヤテーブルを読み込む
	while  ((Status = MDxfGetLine(FLPtr, &Code, Data)) == 0) {
		if (Code == 2) {															// ﾚｲﾔ名
			GetTbl = HTRUE;
			BwsSscanf(Data, BwsStr("%s"), Name);
		} else if (Code == 62) {													// 色番号
			GetTbl = HTRUE;
			BwsSscanf(Data, BwsStr("%d"), &Color);
		} else if (Code == 6) {														// 線種
			GetTbl = HTRUE;
			LinPtr = DxfInf->GetLines(Data, &LineNo);								// 線情報検索
			if (LinPtr == 0)  LineNo = HLINE_SOLD;									// 線名がみつからない
		} else if (Code == 0) {														// 区切り
			if (GetTbl == HTRUE) {
				DxfInf->AddLayer(Name, Color, LineNo);								// ﾚｲﾔ情報追加
				GetTbl = HFALSE;													// 次ﾃﾞｰﾀのために初期化
			}
			if (BwsStrCmp(Data, BwsStr("ENDTAB")) == 0)  break;						// ﾚｲﾔﾃｰﾌﾞﾙ終了
		}
	}

	return(Status);
}
/******************************************************************************************************************
[機能] テーブルセクションを読込む。対象となるテーブルは,LTYPE(線種),STYLE(文字ｽﾀｲﾙ),LAYER(画層)
[返値] = 0 正常  < 0エラー
******************************************************************************************************************/
MINT MDxfGetTblSec            (													// (   O) ｽﾃｰﾀｽ
								CStdioFile*			FLPtr,							// (I  ) ﾌｧｲﾙ識別子
								MDxfInf*			DxfInf)							// (I/O) DXF情報
{
	MINT			Status;
	MINT			Code;
	MCHAR			Data[512];


	Status = 0;

	while  ((Status = MDxfGetLine(FLPtr, &Code, Data)) == 0) {
		if (BwsStrCmp(Data, BwsStr("ENDSEC")) == 0) {								// ｾｸｼｮﾝ終了
			break;
		} else if (BwsStrCmp(Data, BwsStr("LTYPE")) == 0) {		 					// 線種対応ﾃｰﾌﾞﾙの作成 
			Status = MDxfGetTblLin(FLPtr, DxfInf);
			if (Status < 0)  break;
		} else if (BwsStrCmp(Data, BwsStr("STYLE")) == 0) {							// 漢字種対応ﾃｰﾌﾞﾙの作成
			Status = MDxfGetTblFont(FLPtr, DxfInf);
			if (Status < 0)  break;
		} else if (BwsStrCmp(Data, BwsStr("LAYER")) == 0) {
			Status = MDxfGetTblLyr(FLPtr, DxfInf);									// ﾚｲﾔﾃｰﾌﾞﾙの作成
			if (Status < 0)  break;
		}
	}

	return(Status);
}

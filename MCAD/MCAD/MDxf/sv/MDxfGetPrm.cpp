/******************************************************************************************************************
              ＤＸＦ  プログラムファイル (MDxf.cpp)

[機能] 円弧情報を読み込み、レイヤもしくはブロック情報のプリミティブリストへ追加する
[返値] = 0 正常  < 0エラー  > 0 警告 非表示項目である
******************************************************************************************************************/

#include		"HEtcLib.h"
#include		"MgLib.h"
#include		"MDxf.h"


// 読込み頂点データタイプ
enum {
	POLYVER,
	POLYMESH
};

// 読込み用頂点データ
typedef struct _hxvertex {
	MINT		Type;																	// 読込み頂点ﾃﾞｰﾀﾀｲﾌﾟ
	union {
		struct {
			MFLOAT		Pnt[3];															// 頂点
			MINT		dmy;
			MFLOAT		Tan;															// ふくらみ（tan)
		};
		struct {
			MINT		VerNo[4];														// 頂点番号
			MINT		dmy;
		};
	};
} HXVERTEX;


MINT MDxfGetPrmArc        (															// (  O) ｽﾃｰﾀｽ
							CStdioFile*			FLPtr,									// (I  ) ﾌｧｲﾙ識別子
							MCHAR*				Data,									// (  O) 読込み領域
							MDxfInf*			DxfInf,									// (I/O) DXF情報
							MDxfBlock*			Block)									// (I  ) ﾌﾞﾛｯｸ
																						//       == NULL ﾚｲﾔへ読込む
																						//       != NULL 指定ﾌﾞﾛｯｸへ読込む
{
	MINT			Status;																// ｽﾃｰﾀｽ
	MINT			Code;
	MINT			PrmType;															// ﾌﾟﾘﾐﾃｨﾌﾞﾀｲﾌﾟ
	MINT			Color;																// 色番号  （番号）
	MINT			LineNo;																// 線種番号（番号）
	MINT			LineWd;																// 線幅番号（番号）
	HXPRIM			Prim;
	MCHAR			LyName[NAMMAX];														// ﾚｲﾔ名
	MCHAR			LnName[NAMMAX];														// 線種名
	HBOOL			LyrOK;																// ﾚｲﾔ情報があったか
	MDxfLayer*		LayPtr;
	MDxfLines*		LinPtr;
	MDxfPrim*		PrmPtr;
	HXARC3			ArcW1;																// 円弧
	MINT			LColor;																// 色番号  （番号）
	MINT			LLineNo;															// 線種番号（番号）


	// 初期設定
	LyrOK   = HFALSE;																	// ﾚｲﾔ名定義なし
	PrmType = HPRIM_ARC;																// ﾀｲﾌﾟ  ：円弧
	Color   = 256;																		// 色番号：
	LineWd  = 3;																		// 線太さ：細線固定
	BwsStrCpy(LnName, BwsStr("BYLAYER"));												// 線種名：
	ZeroMemory(&Prim, sizeof(HXPRIM));													// ｸﾘｱ
	ArcW1.CPnt.X = ArcW1.CPnt.Y = ArcW1.CPnt.Z = 0.f;

	// グループコード０が登場するまで読み込んで円弧データ作成
	while  ((Status = MDxfGetLine(FLPtr, &Code, Data)) == 0) {
		if (Code == 6) {																// 線種名
			BwsSscanf(Data, BwsStr("%s"), LnName);
		} else if (Code == 8) {															// ﾚｲﾔ名
			LyrOK = HTRUE;
			BwsSscanf(Data, BwsStr("%s"), LyName);
		} else if (Code == 10) {														// 中心点X
			BwsSscanf(Data, BwsStr("%f"), &ArcW1.CPnt.X);
		} else if (Code == 20) {														// 中心点Y
			BwsSscanf(Data, BwsStr("%f"), &ArcW1.CPnt.Y);
		} else if (Code == 30) {														// 中心点Z
			BwsSscanf(Data, BwsStr("%f"), &ArcW1.CPnt.Z);
		} else if (Code == 40) {														// 半径
			BwsSscanf(Data, BwsStr("%f"), &ArcW1.Rad);
		} else if (Code == 50) {														// 開始角度
			BwsSscanf(Data, BwsStr("%f"), &ArcW1.Sta);
		} else if (Code == 51) {														// 終了角度
			BwsSscanf(Data, BwsStr("%f"), &ArcW1.End);
		} else if (Code == 62) {														// 色番号
			BwsSscanf(Data, BwsStr("%d"), &Color);
		} else if (Code == 0) {															// 区切り文字
			if (LyrOK != HTRUE) {														// ﾚｲﾔ名が未定義
				Status = -1;
				goto  PRG_EXIT;
			}
			break;
		}
	}

	HgTrnArcInf(HPLANXY, &ArcW1,&Prim.Arc.Pnt1, &Prim.Arc.Pnt2, &Prim.Arc.Pnt3, &Prim.Arc.Pnt4);	// 円弧情報を変換
	LayPtr = DxfInf->GetLayer(LyName, &LColor, &LLineNo);								// ﾚｲﾔ情報検索
	if (LayPtr == NULL)  goto  PRG_EXIT;

	// 色をチェック
	if (Color == 0) {																	// BYBLOCK
		// 挿入時に検討
	} else if (Color == 256) {															// BYLAYER
		Color = LColor;
	} else if (Color < 0) {																// 画層OFF
		Status = 1;																		// 非表示項目は読込まない
		goto  PRG_EXIT;
	}

	// 線をチェック
	if (BwsStrCmp(LnName, BwsStr("BYLAYER")) == 0) {
		LineNo = LLineNo;																// 代入先の指定法方修正
	} else {
		LinPtr = DxfInf->GetLines(LnName, &LineNo);
//		if (LinPtr == 0)  goto  PRG_EXIT;												// とりあえず  CONTINUOUS
		if (LinPtr == 0)  LineNo = LLineNo;;											// とりあえず
	}

	// 情報をリストへ追加
	if (Block == NULL) {																// ﾚｲﾔへ追加
		PrmPtr = LayPtr->AddPrim(PrmType, Color, LineNo, LineWd, &Prim);				// 図形情報ｺﾝｽﾄﾗｸﾀ
	} else {																			// ﾌﾞﾛｯｸへ追加
		PrmPtr = Block->AddPrim(PrmType, Color, LineNo, LineWd, &Prim);					// 図形情報ｺﾝｽﾄﾗｸﾀ
	}

PRG_EXIT:

	return(Status);
}
/******************************************************************************************************************
[機能] 文字情報を読込み、レイヤもしくはブロック情報のプリミティブリストへ追加する
[返値] = 0 正常  < 0エラー  > 0 警告 非表示項目である
******************************************************************************************************************/
MINT MDxfGetPrmAtb        (															// (  O) ｽﾃｰﾀｽ
							CStdioFile*			FLPtr,									// (I  ) ﾌｧｲﾙ識別子
							MCHAR*				Data,									// (  O) 読込み領域
							MDxfInf*			DxfInf,									// (I/O) DXF情報
							MDxfBlock*			Block)									// (I  ) ﾌﾞﾛｯｸ
																						//       == NULL ﾚｲﾔへ読込む
																						//       != NULL 指定ﾌﾞﾛｯｸへ読込む
{
	MINT			Status;																// ｽﾃｰﾀｽ
	MINT			Code;
	MINT			PrmType;															// ﾌﾟﾘﾐﾃｨﾌﾞﾀｲﾌﾟ
	MINT			Color;																// 色番号  （番号）
	HXPRIM			Prim;
	MCHAR			FnName[NAMMAX];														// ﾌｫﾝﾄ名
	MCHAR			LyName[NAMMAX];														// ﾚｲﾔ名
	MCHAR			LnName[NAMMAX];														// 線種名
	HBOOL			LyrOK;																// ﾚｲﾔ情報があったか
	MDxfLayer*		LayPtr;																// ﾚｲﾔ情報ﾎﾟｲﾝﾀ
	MDxfFonts*		FntPtr;																// ﾌｫﾝﾄ情報ﾎﾟｲﾝﾀ
	MDxfPrim*		PrmPtr;
	MINT			LColor;																// 色番号  （番号）
	MINT			FCode;																// ﾌｫﾝﾄ番号（番号）
	MINT			AtrFlg;																// 属性フラグ
	MINT			TxtLen;																// 文字列長さ


	// 初期設定
	LyrOK   = HFALSE;																	// ﾚｲﾔ名定義なし
	PrmType = HPRIM_TXT;																// ﾀｲﾌﾟ  ：文字
	Color   = 256;																		// 色番号：
	BwsStrCpy(FnName, BwsStr("STANDARD"));												// ﾌｫﾝﾄ名：STANDARD
	BwsStrCpy(LnName, BwsStr("BYLAYER"));												// 線種名：
	ZeroMemory(&Prim, sizeof(HXPRIM));													// ｸﾘｱ
	AtrFlg  = 0;																		// 属性フラグ：0

	while  ((Status = MDxfGetLine(FLPtr, &Code, Data)) == 0) {
		if (Code == 6)	{																// 線種名
			BwsSscanf(Data, BwsStr("%s"), LnName);
		} else if (Code == 7) {															// 文字ｽﾀｲﾙ名
			BwsSscanf(Data, BwsStr("%s"), FnName);
		} else if (Code == 8) {															// ﾚｲﾔ名
			LyrOK = HTRUE;
			BwsSscanf(Data, BwsStr("%s"), LyName);
		} else if (Code == 1) {															// 文字列
			TxtLen = BwsStrLen(Data);
			if ((Prim.Text.String = (MCHAR *)malloc(sizeof(MCHAR) * (TxtLen+1))) == NULL)  HMFAIL;
			Prim.Text.Num = TxtLen;														// 文字数
			BwsStrCpy(Prim.Text.String, Data);
		} else if (Code == 10) {														// 開始点X
			BwsSscanf(Data, BwsStr("%f"), &Prim.Text.Pnt.X);
		} else if (Code == 20) {														// 開始点Y
			BwsSscanf(Data, BwsStr("%f"), &Prim.Text.Pnt.Y);
		} else if (Code == 30) {														// 開始点Z
			BwsSscanf(Data, BwsStr("%f"), &Prim.Text.Pnt.Z);
		} else if (Code == 40) {														// 文字高さ
			BwsSscanf(Data, BwsStr("%f"), &Prim.Text.Hei);
		} else if (Code == 50 ) {														// 回転角度
			BwsSscanf(Data, BwsStr("%f"), &Prim.Text.Ang);
		} else if (Code == 62) {														// 色番号
			BwsSscanf(Data, BwsStr("%d"), &Color);
		} else if (Code == 70) {														// 属性フラグ
			BwsSscanf(Data, BwsStr("%d"), &AtrFlg);
		} else if (Code == 0) {															// 区切り文字
			if (LyrOK != HTRUE) {														// ﾚｲﾔ名が未定義
				Status = -1;
				goto  PRG_EXIT;
			}
			break;
		}
	}

	if ((AtrFlg & 1) == 1)  goto  PRG_EXIT;												// 属性ビット１は非表示

	LayPtr = DxfInf->GetLayer(LyName, &LColor);											// ﾚｲﾔ情報検索
	if (LayPtr == NULL)  goto  PRG_EXIT;

	// 色をチェック
	if (Color == 0) {																	// BYBLOCK
		// 挿入時に検討
	} else if (Color == 256) {															// BYLAYER
		Color = LColor;
	} else if (Color < 0) {																// 画層OFF
		Status = 1;																		// 非表示項目は読込まない
		goto  PRG_EXIT;
	}

	FntPtr = DxfInf->GetFonts(FnName, &FCode);											// ﾌｫﾝﾄ情報検索
	if (FntPtr == NULL)  goto  PRG_EXIT;

	if ((FCode & 4) == 4) {																// 文字方向４(縦)のﾋﾞｯﾄが立っている
		Prim.Text.Msc = -1.0f;
	} else {
		Prim.Text.Msc =  1.0f;
	}

	// 情報をリストへ追加
	if (Block == NULL) {																// ﾚｲﾔへ追加
		PrmPtr = LayPtr->AddPrim(PrmType, Color, 0, 3, &Prim);							// 図形情報ｺﾝｽﾄﾗｸﾀ
	} else {																			// ﾌﾞﾛｯｸへ追加
		PrmPtr = Block->AddPrim(PrmType, Color, 0, 3, &Prim);							// 図形情報ｺﾝｽﾄﾗｸﾀ
	}

PRG_EXIT:

	if (Prim.Text.String)  free(Prim.Text.String);

	return(Status);
}
/******************************************************************************************************************
[機能] 円情報を読込み、レイヤもしくはブロック情報のプリミティブリストへ追加する
[返値] = 0 正常  < 0エラー  > 0 警告 非表示項目である
******************************************************************************************************************/
MINT MDxfGetPrmCir        (															// (  O) ｽﾃｰﾀｽ
							CStdioFile*			FLPtr,									// (I  ) ﾌｧｲﾙ識別子
							MCHAR*				Data,									// (  O) 読込み領域
							MDxfInf*			DxfInf,									// (I/O) DXF情報
							MDxfBlock*			Block)									// (I  ) ﾌﾞﾛｯｸ
																						//       == NULL ﾚｲﾔへ読込む
																						//       != NULL 指定ﾌﾞﾛｯｸへ読込む
{
	MINT			Status;																// ｽﾃｰﾀｽ
	MINT			Code;
	MINT			PrmType;															// ﾌﾟﾘﾐﾃｨﾌﾞﾀｲﾌﾟ
	MINT			Color;																// 色番号  （番号）
	MINT			LineNo;																// 線種番号（番号）
	MINT			LineWd;																// 線幅番号（番号）
	HXPRIM			Prim;
	MCHAR			LyName[NAMMAX];														// ﾚｲﾔ名
	MCHAR			LnName[NAMMAX];														// 線種名
	HBOOL			LyrOK;																// ﾚｲﾔ情報があったか
	MDxfLayer*		LayPtr;
	MDxfLines*		LinPtr;
	MDxfPrim*		PrmPtr;
	MgPoint3			CPnt;
	MFLOAT			Radius;
	MINT			LColor;																// 色番号  （番号）
	MINT			LLineNo;															// 線種番号（番号）


	// 初期設定
	LyrOK   = HFALSE;																	// ﾚｲﾔ名定義なし
	PrmType = HPRIM_CIR;																// ﾀｲﾌﾟ  ：円
	Color   = 256;																		// 色番号：
	LineWd  = 3;																		// 線太さ：細線固定
	BwsStrCpy(LnName, BwsStr("BYLAYER"));												// 線種名：
	ZeroMemory(&Prim, sizeof(HXPRIM));													// ｸﾘｱ
	CPnt.X = CPnt.Y = CPnt.Z = 0.f;

	// グループコード０が登場するまで読み込み円データ作成
	while  ((Status = MDxfGetLine(FLPtr, &Code, Data)) == 0) {
		if (Code == 6) {																// 線種名
			BwsSscanf(Data, BwsStr("%s"), LnName);
		} else if (Code == 8) {															// ﾚｲﾔ名
			LyrOK = HTRUE;
			BwsSscanf(Data, BwsStr("%s"), LyName);
		} else if (Code == 10) {														// 中心点X
			BwsSscanf(Data, BwsStr("%f"), &CPnt.X);
		} else if (Code == 20) {														// 中心点Y
			BwsSscanf(Data, BwsStr("%f"), &CPnt.Y);
		} else if (Code == 30) {														// 中心点Z
			BwsSscanf(Data, BwsStr("%f"), &CPnt.Z);
		} else if (Code == 40) {														// 半径
			BwsSscanf(Data, BwsStr("%f"), &Radius);
		} else if (Code == 62) {														// 色番号
			BwsSscanf(Data, BwsStr("%d"), &Color);
		} else if (Code == 0) {															// 区切り文字
			if (LyrOK != HTRUE) {														// ﾚｲﾔ名が未定義
				Status = -1;
				goto  PRG_EXIT;
			}
			break;
		}
	}

	HgTrnCirInf(HPLANXY, &CPnt, Radius, &Prim.Circle.Pnt1, &Prim.Circle.Pnt2);			// 円情報を変換

	LayPtr = DxfInf->GetLayer(LyName, &LColor, &LLineNo);								// ﾚｲﾔ情報検索
	if (LayPtr == NULL)  goto  PRG_EXIT;

	// 色をチェック
	if (Color == 0) {																	// BYBLOCK
		// 挿入時に検討
	} else if (Color == 256) {															// BYLAYER
		Color = LColor;
	} else if (Color < 0) {																// 画層OFF
		Status = 1;																		// 非表示項目は読込まない
		goto  PRG_EXIT;
	}

	// 線をチェック
	if (BwsStrCmp(LnName, BwsStr("BYLAYER")) == 0) {
		LineNo = LLineNo;																// 代入先の指定法方修正
	} else {
		LinPtr = DxfInf->GetLines(LnName, &LineNo);
//		if (LinPtr == 0)  goto  PRG_EXIT;												// とりあえず  CONTINUOUS
		if (LinPtr == 0)  LineNo = LLineNo;;											// とりあえず
	}

	// 情報をリストへ追加
	if (Block == NULL) {																// ﾚｲﾔへ追加
		PrmPtr = LayPtr->AddPrim(PrmType, Color, LineNo, LineWd, &Prim);				// 図形情報ｺﾝｽﾄﾗｸﾀ
	} else {																			// ﾌﾞﾛｯｸへ追加
		PrmPtr = Block->AddPrim(PrmType, Color, LineNo, LineWd, &Prim);					// 図形情報ｺﾝｽﾄﾗｸﾀ
	}

PRG_EXIT:

	return(Status);
}
/******************************************************************************************************************
[機能] 点情報を読込み、レイヤもしくはブロック情報のプリミティブリストへ追加する
[返値] = 0 正常  < 0エラー  > 0 警告 非表示項目である
******************************************************************************************************************/
MINT MDxfGetPrmDot        (															// (  O) ｽﾃｰﾀｽ
							CStdioFile*			FLPtr,									// (I  ) ﾌｧｲﾙ識別子
							MCHAR*				Data,									// (  O) 読込み領域
							MDxfInf*			DxfInf,									// (I/O) DXF情報
							MDxfBlock*			Block)									// (I  ) ﾌﾞﾛｯｸ
																						//       == NULL ﾚｲﾔへ読込む
																						//       != NULL 指定ﾌﾞﾛｯｸへ読込む
{
	MINT			Status;																// ｽﾃｰﾀｽ
	MINT			Code;
	MINT			PrmType;															// ﾌﾟﾘﾐﾃｨﾌﾞﾀｲﾌﾟ
	MINT			Color;																// 色番号  （番号）
	HXPRIM			Prim;
	MCHAR			LyName[NAMMAX];														// ﾚｲﾔ名
	MCHAR			LnName[NAMMAX];														// 線種名
	HBOOL			LyrOK;																// ﾚｲﾔ情報があったか
	MDxfLayer*		LayPtr;
	MDxfPrim*		PrmPtr;
	MINT			LColor;																// 色番号  （番号）


	// 初期設定
	LyrOK   = HFALSE;																	// ﾚｲﾔ名定義なし
	PrmType = HPRIM_DOT;																// ﾀｲﾌﾟ  ：線
	Color   = 256;																		// 色番号：
	BwsStrCpy(LnName, BwsStr("BYLAYER"));												// 線種名：
	ZeroMemory(&Prim, sizeof(HXPRIM));													// ｸﾘｱ

	// グループコード０が登場するまで読み込んでデータ作成
	while  ((Status = MDxfGetLine(FLPtr, &Code, Data)) == 0) {
		if (Code == 6) {																// 線種名
			BwsSscanf(Data, BwsStr("%s"), LnName);
		} else if (Code == 8) {															// ﾚｲﾔ名
			LyrOK = HTRUE;
			BwsSscanf(Data, BwsStr("%s"), LyName );
		} else if (Code == 10) {														// 点X
			BwsSscanf(Data, BwsStr("%f"), &Prim.Dot.X);
		} else if (Code == 20) {														// 点Y
			BwsSscanf(Data, BwsStr("%f"), &Prim.Dot.Y);
		} else if (Code == 30)	{														// 点Z
			BwsSscanf(Data, BwsStr("%f"), &Prim.Dot.Z);
		} else if (Code == 62) {														// 色番号
			BwsSscanf(Data, BwsStr("%d"), &Color);
		} else if (Code == 0) {															// 区切り文字（終了）
			if (LyrOK != HTRUE) {														// ﾚｲﾔ名が未定義
				Status = -1;
				goto  PRG_EXIT;
			}
			break;
		}
	}

	LayPtr = DxfInf->GetLayer(LyName, &LColor);											// ﾚｲﾔ情報検索
	if (LayPtr == NULL)  goto  PRG_EXIT;

	// 色をチェック
	if (Color == 0) {																	// BYBLOCK
		// 挿入時に検討
	} else if (Color == 256) {															// BYLAYER
		Color = LColor;
	} else if (Color < 0) {																// 画層OFF
		Status = 1;																		// 非表示項目は読込まない
		goto  PRG_EXIT;
	}

	// 情報をリストへ追加
	if (Block == NULL) {																// ﾚｲﾔへ追加
		PrmPtr = LayPtr->AddPrim(PrmType, Color, 0, 3, &Prim);							// 図形情報ｺﾝｽﾄﾗｸﾀ
	} else {																			// ﾌﾞﾛｯｸへ追加
		PrmPtr = Block->AddPrim(PrmType, Color, 0, 3, &Prim);							// 図形情報ｺﾝｽﾄﾗｸﾀ
	}

PRG_EXIT:

	return(Status);
}

/******************************************************************************************************************
[機能] 面情報を読込み、レイヤもしくはブロック情報のプリミティブリストへ追加する
[返値] = 0 正常  < 0エラー  > 0 警告 非表示項目である
******************************************************************************************************************/
MINT MDxfGetPrmFac        (															// (  O) ｽﾃｰﾀｽ
							CStdioFile*			FLPtr,									// (I  ) ﾌｧｲﾙ識別子
							MCHAR*				Data,									// (  O) 読込み領域
							MDxfInf*			DxfInf,									// (I/O) DXF情報
							MDxfBlock*			Block)									// (I  ) ﾌﾞﾛｯｸ
																						//       == NULL ﾚｲﾔへ読込む
																						//       != NULL 指定ﾌﾞﾛｯｸへ読込む
{
	MINT			Status;																// ｽﾃｰﾀｽ
	MINT			Code;
	MINT			PrmType;															// ﾌﾟﾘﾐﾃｨﾌﾞﾀｲﾌﾟ
	MINT			Color;																// 色番号  （番号）
	MINT			LineNo;																// 線種番号（番号）
	MINT			LineWd;																// 線幅番号（番号）
	HXPRIM			Prim;
	MCHAR			LyName[NAMMAX];														// ﾚｲﾔ名
	MCHAR			LnName[NAMMAX];														// 線種名
	HBOOL			LyrOK;																// ﾚｲﾔ情報があったか
	MDxfLayer*		LayPtr;
	MDxfLines*		LinPtr;
	MDxfPrim*		PrmPtr;
	MINT			LColor;																// 色番号  （番号）
	MINT			LLineNo;															// 線種番号（番号）


	// 初期設定
	LyrOK   = HFALSE;																	// ﾚｲﾔ名定義なし
	PrmType = HPRIM_POL;																// ﾀｲﾌﾟ  ：面
	Color   = 256;																		// 色番号：
	LineWd  = 3;																		// 線太さ：細線固定
	BwsStrCpy(LnName, BwsStr("BYLAYER"));												// 線種名：
	ZeroMemory(&Prim, sizeof(HXPRIM));													// ｸﾘｱ

	Prim.Face.CFlag = 1;																// 始終点 1:閉じる
	// グループコード０まで読み込んでデータ作成
	while  ((Status = MDxfGetLine(FLPtr, &Code, Data)) == 0) {
		if (Code == 6) {																// 線種名
			BwsSscanf(Data, BwsStr("%s"), LnName);
		} else if (Code == 8) {															// ﾚｲﾔ名
			LyrOK = HTRUE;
			BwsSscanf(Data, BwsStr("%s"), LyName);
		} else if (Code == 10) {														// 点１X
			if (Prim.Face.NumPnt < 1)  Prim.Face.NumPnt = 1;
			BwsSscanf(Data, BwsStr("%f"), &Prim.Face.Pnt[0].X);
		} else if (Code == 20) {														// 点１Y
			BwsSscanf(Data, BwsStr("%f"), &Prim.Face.Pnt[0].Y);
		} else if (Code == 30) {														// 点１Z
			BwsSscanf(Data, BwsStr("%f"), &Prim.Face.Pnt[0].Z);
		} else if (Code == 11) {														// 点２X
			if (Prim.Face.NumPnt < 2)  Prim.Face.NumPnt = 2;
			BwsSscanf(Data, BwsStr("%f"), &Prim.Face.Pnt[1].X);
		} else if (Code == 21) {														// 点２Y
			BwsSscanf(Data, BwsStr("%f"), &Prim.Face.Pnt[1].Y);
		} else if (Code == 31) {														// 点２Z
			BwsSscanf(Data, BwsStr("%f"), &Prim.Face.Pnt[1].Z);
		} else if (Code == 12) {														// 点３X
			if (Prim.Face.NumPnt < 3)  Prim.Face.NumPnt = 3;
			BwsSscanf(Data, BwsStr("%f"), &Prim.Face.Pnt[2].X);
		} else if (Code == 22) {														// 点３Y
			BwsSscanf(Data, BwsStr("%f"), &Prim.Face.Pnt[2].Y );
		} else if (Code == 32)	{														// 点３Z
			BwsSscanf(Data, BwsStr("%f"), &Prim.Face.Pnt[2].Z );
		} else if (Code == 13)	{														// 点４X
			if (Prim.Face.NumPnt < 4)  Prim.Face.NumPnt = 4;
			BwsSscanf(Data, BwsStr("%f"), &Prim.Face.Pnt[3].X );
		} else if (Code == 23)	{														// 点４Y
			BwsSscanf(Data, BwsStr("%f"), &Prim.Face.Pnt[3].Y );
		} else if (Code == 33) {														// 点４Z
			BwsSscanf(Data, BwsStr("%f"), &Prim.Face.Pnt[3].Z );
		} else if (Code == 62) {														// 色番号
			BwsSscanf(Data, BwsStr("%d"), &Color);
		} else if (Code == 70) {														// 始終点
			BwsSscanf(Data, BwsStr("%d"), &Prim.Face.CFlag);
		} else if (Code == 0) {															// 区切り文字
			if (LyrOK != HTRUE) {														// ﾚｲﾔ名が未定義
				Status = -1;
				goto  PRG_EXIT;
			}
			break;
		}
	}

	LayPtr = DxfInf->GetLayer(LyName, &LColor, &LLineNo);								// ﾚｲﾔ情報検索
	if (LayPtr == NULL)  goto  PRG_EXIT;

	// 色をチェック
	if (Color == 0) {																	// BYBLOCK
		// 挿入時に検討
	} else if (Color == 256) {															// BYLAYER
		Color = LColor;
	} else if (Color < 0) {																// 画層OFF
		Status = 1;																		// 非表示項目は読込まない
		goto  PRG_EXIT;
	}

	// 線をチェック
	if (BwsStrCmp(LnName, BwsStr("BYLAYER")) == 0) {
		LineNo = LLineNo;																// 代入先の指定法方修正
	} else {
		LinPtr = DxfInf->GetLines(LnName, &LineNo);
		if (LinPtr == NULL)  goto  PRG_EXIT;
	}

	// 情報をリストへ追加
	if (Block == NULL) {																// ﾚｲﾔへ追加
		PrmPtr = LayPtr->AddPrim(PrmType, Color, LineNo, LineWd, &Prim);				// 図形情報ｺﾝｽﾄﾗｸﾀ
	} else {																			// ﾌﾞﾛｯｸへ追加
		PrmPtr = Block->AddPrim(PrmType, Color, LineNo, LineWd, &Prim);					// 図形情報ｺﾝｽﾄﾗｸﾀ
	}

PRG_EXIT:

	return(Status);
}
/******************************************************************************************************************
[機能] ブロックを挿入する
[返値] = 0 正常  < 0エラー  > 0 警告 非表示項目である
******************************************************************************************************************/
MINT MDxfGetPrmIns        (															// (  O) ｽﾃｰﾀｽ
							CStdioFile*			FLPtr,									// (I  ) ﾌｧｲﾙ識別子
							MCHAR*				Data,									// (  O) 読込み領域
							MDxfInf*			DxfInf,									// (I/O) DXF情報
							MDxfBlock*			Block)									// (I  ) ﾌﾞﾛｯｸ
																						//       == NULL ﾚｲﾔへ読込む
																						//       != NULL 指定ﾌﾞﾛｯｸへ読込む
{
	MINT			Status;																// ｽﾃｰﾀｽ
	MINT			Code;
	MINT			Color;																// 色番号  （番号）
	MINT			LineNo;																// 線種番号（番号）
	MCHAR			BlName[NAMMAX];														// ﾌﾞﾛｯｸ名
	MCHAR			LyName[NAMMAX];														// ﾚｲﾔ名
	HBOOL			LyrOK;																// ﾚｲﾔ情報があったか
	MDxfBlock*		BlkPtr;
	MDxfLayer*		LayPtr;
	MDxfPrim*		PrmPtr;
	MINT			LColor;																// 色番号  （番号）
	MINT			LLineNo;															// 線種番号（番号）
	MgPoint3			InsPnt;																// 挿入点
	MgPoint3			InsScl;																// 尺度
	MFLOAT			Rotate;																// 回転角度
	MgPoint3			BasPnt;																// 基点
	MINT			ic;


	// 初期設定
	LyrOK    = HFALSE;																	// ﾚｲﾔ名定義なし
	InsPnt.X = InsPnt.Y = InsPnt.Z = 0.0f;												// 挿入点ﾃﾞﾌｫﾙﾄ0.0
	InsScl.X = InsScl.Y = InsScl.Z = 1.0f;												// 倍率ﾃﾞﾌｫﾙﾄ1.0
	Rotate   = 0.0f;																	// 回転角度ﾃﾞﾌｫﾙﾄ0.0

	// グループコード０が登場するまで読み込み、データ作成
	while  ((Status = MDxfGetLine(FLPtr, &Code, Data)) == 0) {
		if (Code == 2) {																// 挿入ﾌﾞﾛｯｸ名
			BwsSscanf(Data, BwsStr("%s"), BlName);
		} else if (Code == 8) {															// ﾚｲﾔ名
			LyrOK = HTRUE;
			BwsSscanf(Data, BwsStr("%s"), LyName);
		} else if (Code == 10) {														// 挿入点X
			BwsSscanf(Data, BwsStr("%f"), &InsPnt.X);
		} else if (Code == 20) {														// 挿入点Y
			BwsSscanf(Data, BwsStr("%f"), &InsPnt.Y);
		} else if (Code == 30) {														// 挿入点Z
			BwsSscanf(Data, BwsStr("%f"), &InsPnt.Z);
		} else if (Code == 41) {														// X尺度
			BwsSscanf(Data, BwsStr("%f"), &InsScl.X);
		} else if (Code == 42) {														// Y尺度
			BwsSscanf(Data, BwsStr("%f"), &InsScl.Y);
		} else if (Code == 43) {														// Z尺度
			BwsSscanf(Data, BwsStr("%f"), &InsScl.Z);
		} else if (Code == 50) {														// 回転角度
			BwsSscanf(Data, BwsStr("%f"), &Rotate);
		} else if (Code == 0) {															// 区切り文字
			if (LyrOK != HTRUE) {														// ﾚｲﾔ名が未定義
				Status = -1;
				goto  PRG_EXIT;
			}
			break;
		}
	}

	// 挿入先の色と線番号を得る
	if (Block == NULL) {																// ﾚｲﾔへ追加
		LayPtr = DxfInf->GetLayer(LyName, &LColor, &LLineNo);							// ﾚｲﾔ情報検索
		if (LayPtr == NULL)  goto  PRG_EXIT;
	} else {																			// ﾌﾞﾛｯｸへ追加
		Color  = Block->m_Color;
		LineNo = Block->m_LineNo;
	}

	BlkPtr = DxfInf->GetBlock(BlName, NULL, NULL, NULL, &BasPnt);						// ﾌﾞﾛｯｸ情報検索
	if (BlkPtr == NULL)  goto  PRG_EXIT;

	if (BlkPtr->m_PrimList != NULL) {
		for (ic = 0 ; ic < (MINT)BlkPtr->m_PrimList->Number() ; ic++) {					// 図形情報数
			PrmPtr = (MDxfPrim *)BlkPtr->m_PrimList->Access(ic);						// ﾎﾟｲﾝﾀを得る

			// 情報をリストへ追加
			if (Block == NULL) {														// ﾚｲﾔへ追加
				PrmPtr = LayPtr->AddPrim(PrmPtr->m_PrmType, PrmPtr->m_Color,			// 図形情報ｺﾝｽﾄﾗｸﾀ
										 PrmPtr->m_LineNo,  PrmPtr->m_LineWd, &PrmPtr->m_Prim);
			} else {																	// ﾌﾞﾛｯｸへ追加
				PrmPtr = Block->AddPrim(PrmPtr->m_PrmType, PrmPtr->m_Color,				// 図形情報ｺﾝｽﾄﾗｸﾀ
										PrmPtr->m_LineNo,  PrmPtr->m_LineWd, &PrmPtr->m_Prim);
			}
			PrmPtr->Transform(Color, &BasPnt, &InsPnt, &InsScl, Rotate);				// 図形の変形
		}
	}

PRG_EXIT:

	return(Status);
}
/******************************************************************************************************************
[機能] 線情報を読み込み、レイヤもしくはブロック情報のプリミティブリストへ追加する
[返値] = 0 正常  < 0エラー  > 0 警告 非表示項目である
******************************************************************************************************************/
MINT MDxfGetPrmLin        (															// (  O) ｽﾃｰﾀｽ
							CStdioFile*			FLPtr,									// (I  ) ﾌｧｲﾙ識別子
							MCHAR*				Data,									// (  O) 読込み領域
							MDxfInf*			DxfInf,									// (I/O) DXF情報
							MDxfBlock*			Block)									// (I  ) ﾌﾞﾛｯｸ
																						//       == NULL ﾚｲﾔへ読込む
																						//       != NULL 指定ﾌﾞﾛｯｸへ読込む
{
	MINT			Status;																// ｽﾃｰﾀｽ
	MINT			Code;
	MINT			PrmType;															// ﾌﾟﾘﾐﾃｨﾌﾞﾀｲﾌﾟ
	MINT			Color;																// 色番号  （番号）
	MINT			LineNo;																// 線種番号（番号）
	MINT			LineWd;																// 線幅番号（番号）
	HXPRIM			Prim;
	MCHAR			LyName[NAMMAX];														// ﾚｲﾔ名
	MCHAR			LnName[NAMMAX];														// 線種名
	HBOOL			LyrOK;																// ﾚｲﾔ情報があったか
	MDxfLayer*		LayPtr;
	MDxfLines*		LinPtr;
	MDxfPrim*		PrmPtr;
	MINT			LColor;																// 色番号  （番号）
	MINT			LLineNo;															// 線種番号（番号）


	// 初期設定
	LyrOK   = HFALSE;																	// ﾚｲﾔ名定義なし
	PrmType = HPRIM_LIN;																// ﾀｲﾌﾟ  ：線
	Color   = 256;																		// 色番号：
	LineWd  = 3;																		// 線太さ：細線固定
	BwsStrCpy(LnName, BwsStr("BYLAYER"));												// 線種名：
	ZeroMemory(&Prim, sizeof(HXPRIM));													// ｸﾘｱ

	// グループコード０が登場するまで読み込んで線データ作成
	while  ((Status = MDxfGetLine(FLPtr, &Code, Data)) == 0) {
		if (Code == 6) {																// 線種名
			BwsSscanf(Data, BwsStr("%s"), LnName);
		} else if (Code == 8) {															// レイヤ名
			LyrOK = HTRUE;
			BwsSscanf(Data, BwsStr("%s"), LyName);
		} else if (Code == 10) {														// 始点X
			BwsSscanf(Data, BwsStr("%f"), &Prim.Line.Pnt[0].X);
		} else if (Code == 20) {														// 始点Y
			BwsSscanf(Data, BwsStr("%f"), &Prim.Line.Pnt[0].Y);
		} else if (Code == 30) {														// 始点Z
			BwsSscanf(Data, BwsStr("%f"), &Prim.Line.Pnt[0].Z);
		} else if (Code == 11) {														// 終点X
			BwsSscanf(Data, BwsStr("%f"), &Prim.Line.Pnt[1].X);
		} else if (Code == 21) {														// 終点Y
			BwsSscanf(Data, BwsStr("%f"), &Prim.Line.Pnt[1].Y);
		} else if (Code == 31) {														// 終点Z
			BwsSscanf(Data, BwsStr("%f"), &Prim.Line.Pnt[0].Z);
		} else if (Code == 62) {														// 色番号
			BwsSscanf(Data, BwsStr("%d"), &Color);
		} else if (Code == 0) {															// 区切り文字
			if (LyrOK != HTRUE) {														// ﾚｲﾔ名が未定義
				Status = -1;
				goto  PRG_EXIT;
			}
			break;
		}
	}

	LayPtr = DxfInf->GetLayer(LyName, &LColor, &LLineNo);								// ﾚｲﾔ情報検索
	if (LayPtr == NULL)  goto  PRG_EXIT;

	// 色をチェック
	if (Color == 0) {																	// BYBLOCK
		// 挿入時に検討
	} else if (Color == 256) {															// BYLAYER
		Color = LColor;
	} else if (Color < 0) {																// 画層OFF
		Status = 1;																		// 非表示項目は読込まない
		goto  PRG_EXIT;
	}

	// 線をチェック
	if (BwsStrCmp(LnName, BwsStr("BYLAYER")) == 0) {
		LineNo = LLineNo;																// 代入先の指定法方修正
	} else {
		LinPtr = DxfInf->GetLines(LnName, &LineNo);
//		if (LinPtr == 0)  goto  PRG_EXIT;												// とりあえず  CONTINUOUS
		if (LinPtr == 0)  LineNo = LLineNo;;											// とりあえず
	}

	// 情報をリストへ追加
	if (Block == NULL) {																// ﾚｲﾔへ追加
		PrmPtr = LayPtr->AddPrim(PrmType, Color, LineNo, LineWd, &Prim);				// 図形情報ｺﾝｽﾄﾗｸﾀ
	} else {																			// ﾌﾞﾛｯｸへ追加
		PrmPtr = Block->AddPrim(PrmType, Color, LineNo, LineWd, &Prim);					// 図形情報ｺﾝｽﾄﾗｸﾀ
	}

PRG_EXIT:

	return(Status);
}
/******************************************************************************************************************
[機能] ポリライン情報を読込み、レイヤもしくはブロック情報のプリミティブリストへ追加する
[返値] = 0 正常  < 0エラー  > 0 警告 非表示項目である
******************************************************************************************************************/
MINT MDxfGetPrmPln        (															// (  O) ｽﾃｰﾀｽ
							CStdioFile*			FLPtr,									// (I  ) ﾌｧｲﾙ識別子
							MCHAR*				Data,									// (  O) 読込み領域
							MDxfInf*			DxfInf,									// (I/O) DXF情報
							MDxfBlock*			Block)									// (I  ) ﾌﾞﾛｯｸ
																						//       == NULL ﾚｲﾔへ読込む
																						//       != NULL 指定ﾌﾞﾛｯｸへ読込む
{
	MINT			Status;																// ｽﾃｰﾀｽ
	MINT			Code;
	MINT			PrmType;															// ﾌﾟﾘﾐﾃｨﾌﾞﾀｲﾌﾟ
	MINT			Color;																// 色番号  （番号）
	MINT			LineNo;																// 線種番号（番号）
	MINT			LineWd;																// 線幅番号（番号）
	HXPRIM			Prim;
	MCHAR			LyName[NAMMAX];														// ﾚｲﾔ名
	MCHAR			LnName[NAMMAX];														// 線種名
	HBOOL			LyrOK;																// ﾚｲﾔ情報があったか
	MDxfLayer*		LayPtr;
	MDxfLines*		LinPtr;
	MDxfPrim*		PrmPtr;
	MINT			LColor;																// 色番号  （番号）
	MINT			LLineNo;															// 線種番号（番号）
	MINT			Flag;																// ﾎﾟﾘﾗｲﾝﾌﾗｸﾞ
	MINT			VerNum;																// 作業用頂点数
	HXVERTEX*		Vertex     = NULL;													// 作業用頂点
	MINT			MNum;																// ﾒｯｼｭ数
	MINT*			MVerNumPnt = NULL;													// ﾒｯｼｭ毎の頂点数列
	MINT			MNumPntNo;															// 頂点番号数
	MINT*			MPntNo     = NULL;													// 頂点番号列
	MINT			MNumPnt;															// 頂点数
	MgPoint3*		MPnt       = NULL;													// 頂点列
	MINT			ic, jc;																// ﾙｰﾌﾟｶｳﾝﾀ


	// 頂点データ用領域の確保
	if ((Vertex = (HXVERTEX *)malloc(sizeof(HXVERTEX) * 4096)) == NULL)  HMFAIL;

	VerNum   = 0;
	ZeroMemory(&Vertex[VerNum], sizeof(HXVERTEX));										// ｸﾘｱ
	LyrOK    = HFALSE;																	// ﾚｲﾔ名定義なし
	PrmType  = HPRIM_LIN;																// ﾀｲﾌﾟ：線
	Flag     = 0;																		// ﾌﾗｸﾞﾃﾞﾌｫﾙﾄ：0
	Color    = 256;																		// 色番号：BYLAYER
	LineWd   = 3;																		// 線太さ：細線固定
	BwsStrCpy(LnName, BwsStr("BYLAYER"));												// 線種名：BYLAYER
	ZeroMemory(&Prim, sizeof(HXPRIM));													// ｸﾘｱ

	while ((Status = MDxfGetLine(FLPtr, &Code, Data)) == 0) {
		if (Code == 6) {																// 線種名
			BwsSscanf(Data, BwsStr("%s"), LnName);
		} else if (Code == 8) {															// ﾚｲﾔ名
			LyrOK = HTRUE;
			BwsSscanf(Data, BwsStr("%s"), LyName);
		} else if (Code == 62) {														// 色番号
			BwsSscanf(Data, BwsStr("%d"), &Color);
		} else if (Code == 70){															// ﾌﾗｸﾞ
			BwsSscanf(Data, BwsStr("%d"), &Flag);
		} else if (Code == 0) {															// 区切り文字
			if (BwsStrCmp(Data, BwsStr("VERTEX")) == 0) {
				Status = MDxfGetLine(FLPtr, &Code, Data);								// ひとつ先読み
				while (1) {
					if (Code == 10) {
						BwsSscanf(Data, BwsStr("%f"), &Vertex[VerNum].Pnt[0]);
						Vertex[VerNum].Type = POLYVER;
					} else if (Code == 20) {
						BwsSscanf(Data, BwsStr("%f"), &Vertex[VerNum].Pnt[1]);
						Vertex[VerNum].Type = POLYVER;
					} else if (Code == 30) {
						BwsSscanf(Data, BwsStr("%f"), &Vertex[VerNum].Pnt[2]);
						Vertex[VerNum].Type = POLYVER;
					} else if (Code == 42) {
						BwsSscanf(Data, BwsStr("%f"), &Vertex[VerNum].Tan);
						Vertex[VerNum].Type = POLYVER;
					} else if (Code == 71) {											// 頂点番号1
						BwsSscanf(Data, BwsStr("%d"), &Vertex[VerNum].VerNo[0]);
						Vertex[VerNum].Type = POLYMESH;
					} else if (Code == 72) {											// 頂点番号2
						BwsSscanf(Data, BwsStr("%d"), &Vertex[VerNum].VerNo[1]);
						Vertex[VerNum].Type = POLYMESH;
					} else if (Code == 73) {											// 頂点番号3
						BwsSscanf(Data, BwsStr("%d"), &Vertex[VerNum].VerNo[2]);
						Vertex[VerNum].Type = POLYMESH;
					} else if (Code == 74) {											// 頂点番号
						BwsSscanf(Data, BwsStr("%d"), &Vertex[VerNum].VerNo[3]);
						Vertex[VerNum].Type = POLYMESH;
					} else if (Code == 0) {
						VerNum++;
						ZeroMemory(&Vertex[VerNum], sizeof(HXVERTEX));					// ｸﾘｱ
						if (BwsStrCmp(Data, BwsStr("SEQEND")) == 0)  break;

						if (_msize(Vertex) < (HUINT)sizeof(HXVERTEX) * (VerNum+1)) {
							if ((Vertex = (HXVERTEX *)realloc(Vertex, sizeof(HXVERTEX) * (VerNum+256))) == NULL)  HMFAIL;
						}
					}
					Status = MDxfGetLine(FLPtr, &Code, Data);
				}
			}
			if (BwsStrCmp(Data, BwsStr("SEQEND")) == 0) {
				if (LyrOK != HTRUE ) {													// ﾚｲﾔ名が未定義
					Status = -1;
					goto  PRG_EXIT;
				}
				if (VerNum <= 0) {														// 頂点ﾃﾞｰﾀがない
					Status = -1;
					goto  PRG_EXIT;
				}
				break;
			}
		}
	}

	LayPtr = DxfInf->GetLayer(LyName, &LColor, &LLineNo);								// ﾚｲﾔ情報検索
	if (LayPtr == NULL)  goto  PRG_EXIT;

	// 色をチェック
	if (Color == 0) {																	// BYBLOCK
		// 挿入時に検討
	} else if (Color == 256) {															// BYLAYER
		Color = LColor;
	} else if (Color < 0) {																// 画層OFF
		Status = 1;																		// 非表示項目は読込まない
		goto  PRG_EXIT;
	}

	// 線をチェック
	if (BwsStrCmp(LnName, BwsStr("BYLAYER")) == 0) {
		LineNo = LLineNo;																// 代入先の指定法方修正
	} else {
		LinPtr = DxfInf->GetLines(LnName, &LineNo);
		if (LinPtr == 0)  goto  PRG_EXIT;
	}


	// ポリラインとポリメッシュを分岐する
	if (((Flag & 16) == 16) || ((Flag & 32) == 32) || ( (Flag & 64) == 64)) {			// ﾎﾟﾘﾒｯｼｭ
		// メッシュプリミティブ情報のメモリを確保
		MNum      = 0;																	// ﾒｯｼｭ数
		MNumPntNo = 0;																	// 頂点番号列数
		MNumPnt   = 0;																	// 頂点数
		for (ic = 0 ; ic < VerNum ; ic++) {
			switch (Vertex[ic].Type) {
				case  POLYMESH:															// ﾒｯｼｭﾃﾞｰﾀ
					MNum++;																// ﾒｯｼｭ数
					for (jc = 0 ; jc < 4 ; jc++) {
						if ((Vertex+ic)->VerNo[jc] > 0)  MNumPntNo++;
					}
					break;
				case  POLYVER:															// 頂点ﾃﾞｰﾀ
					MNumPnt++;
					break;
				default:
					break;
			}
		}

		if ((MVerNumPnt = (MINT *)malloc(sizeof(MINT)       * MNum))      == NULL)  HMFAIL;		// ﾒｯｼｭ毎の頂点数列
		if ((MPntNo     = (MINT *)malloc(sizeof(MINT)       * MNumPntNo)) == NULL)  HMFAIL;		// 頂点番号列
		if ((MPnt       = (MgPoint3 *)malloc(sizeof(MgPoint3) * MNumPnt))   == NULL)  HMFAIL;		// 頂点列

		// 情報作成
		MNum      = 0;																	// ﾒｯｼｭ数
		MNumPntNo = 0;																	// 頂点番号列数
		MNumPnt   = 0;																	// 頂点数
		for (ic = 0 ; ic < VerNum ; ic++) {
			switch (Vertex[ic].Type) {
				case  POLYMESH:															// ﾒｯｼｭﾃﾞｰﾀ
					*(MVerNumPnt + MNum) = 0;											// ﾒｯｼｭの頂点数
					for (jc = 0 ; jc < 4 ; jc++) {
						if ((Vertex+ic)->VerNo[jc] > 0) {
							*(MPntNo+MNumPntNo) = ((Vertex+ic)->VerNo[jc]) - 1;
							MNumPntNo++;
							(*(MVerNumPnt + MNum))++;									// ﾒｯｼｭの頂点数
						}
					}
					MNum++;
					break;
				case  POLYVER:															// 頂点ﾃﾞｰﾀ
					MPntNo[MNumPnt] = ic;
					memcpy(&MPnt[MNumPnt], (Vertex+ic)->Pnt, sizeof(MgPoint3));
					MNumPnt++;
					break;
			default:
				break;
			}
		}
		// プリミティブ作成
		Prim.Mesh.Num       = MNum;														// ﾒｯｼｭ数
		Prim.Mesh.VerNumPnt = MVerNumPnt;												// ﾒｯｼｭ毎の頂点数列
		Prim.Mesh.NumPntNo  = MNumPntNo;												// 頂点番号数
		Prim.Mesh.PntNo     = MPntNo;													// 頂点番号列
		Prim.Mesh.NumPnt    = MNumPnt;													// 頂点数
		Prim.Mesh.Pnt       = MPnt;														// 頂点列

		// 情報をリストへ追加
		if (Block == NULL) {															// ﾚｲﾔへ追加
			PrmPtr = LayPtr->AddPrim(HPRIM_MSH, Color, LineNo, LineWd, &Prim);			// 図形情報ｺﾝｽﾄﾗｸﾀ
		} else {																		// ﾌﾞﾛｯｸへ追加
			PrmPtr = Block->AddPrim(HPRIM_MSH, Color, LineNo, LineWd, &Prim);			// 図形情報ｺﾝｽﾄﾗｸﾀ
		}
	} else if ((Flag & 9) == 9) {														// ﾎﾟﾘﾒｯｼｭ
		MNum      = 1;
		MNumPntNo = 0;
		MNumPnt   = 0;
		if ((MVerNumPnt = (MINT *)malloc(sizeof(MINT)       * 1))      == NULL)  HMFAIL;		// ﾒｯｼｭ毎の頂点数列
		if ((MPntNo     = (MINT *)malloc(sizeof(MINT)       * VerNum)) == NULL)  HMFAIL;		// 頂点番号列
		if ((MPnt       = (MgPoint3 *)malloc(sizeof(MgPoint3) * VerNum)) == NULL)  HMFAIL;		// 頂点列
		for (ic = 0 ; ic < VerNum ; ic++) {
			MNumPntNo++;
			MPntNo[MNumPnt] = ic;
			memcpy(&MPnt[MNumPnt], (Vertex+ic)->Pnt, sizeof(MgPoint3));
			MNumPnt++;
		}
		MVerNumPnt[0] = MNumPnt;
		// プリミティブ作成
		Prim.Mesh.Num       = MNum;														// ﾒｯｼｭ数
		Prim.Mesh.VerNumPnt = MVerNumPnt;												// ﾒｯｼｭ毎の頂点数列
		Prim.Mesh.NumPntNo  = MNumPntNo;												// 頂点番号数
		Prim.Mesh.PntNo     = MPntNo;													// 頂点番号列
		Prim.Mesh.NumPnt    = MNumPnt;													// 頂点数
		Prim.Mesh.Pnt       = MPnt;														// 頂点列

		// 情報をリストへ追加
		if (Block == NULL) {															// ﾚｲﾔへ追加
			PrmPtr = LayPtr->AddPrim(HPRIM_MSH, Color, LineNo, LineWd, &Prim);			// 図形情報ｺﾝｽﾄﾗｸﾀ
		} else {																		// ﾌﾞﾛｯｸへ追加
			PrmPtr = Block->AddPrim(HPRIM_MSH, Color, LineNo, LineWd, &Prim);			// 図形情報ｺﾝｽﾄﾗｸﾀ
		}
	} else {																			// ﾎﾟﾘﾗｲﾝ
		// ポリラインと円弧のデータを作成する
		for (ic = 0 ; ic < (VerNum - 1) ; ic++) {
			if (Vertex[ic].Tan == 0.0f) {												// 線分
				memcpy(&Prim.Line.Pnt[0], &Vertex[ic].Pnt,   sizeof(MgPoint3));
				memcpy(&Prim.Line.Pnt[1], &Vertex[ic+1].Pnt, sizeof(MgPoint3));
			} else {																	// 膨らみあり：円弧
				PrmType = HPRIM_ARC;													// 円弧         (5)
				Status  = MDxfPlnToArc((MgPoint3 *)&Vertex[ic].Pnt, (MgPoint3 *)&Vertex[ic+1].Pnt,
									    Vertex[ic].Tan, &Prim.Arc);
			}
			// 情報をリストへ追加
			if (Block == NULL) {														// ﾚｲﾔへ追加
				PrmPtr = LayPtr->AddPrim(PrmType, Color, LineNo, LineWd, &Prim);		// 図形情報ｺﾝｽﾄﾗｸﾀ
			} else {																	// ﾌﾞﾛｯｸへ追加
				PrmPtr = Block->AddPrim(PrmType, Color, LineNo, LineWd, &Prim);			// 図形情報ｺﾝｽﾄﾗｸﾀ
			}
		}

		// クローズしたポリラインの場合の頂点データ指示ポインタはVertexでなくてic(=Vertex-1)である
		if ((Flag & 1) == 1) {															// ﾌﾗｸﾞ１が立っている
			if ((Vertex+ic)->Tan == 0.0f) {												// 線分
				memcpy(&Prim.Line.Pnt[0], &Vertex[ic].Pnt, sizeof(MgPoint3));
				memcpy(&Prim.Line.Pnt[1], &Vertex[0].Pnt,  sizeof(MgPoint3));
			} else {																	// 膨らみあり：円弧
				PrmType = HPRIM_ARC;													// 円弧         (5)
				Status  = MDxfPlnToArc((MgPoint3 *)&Vertex[ic].Pnt, (MgPoint3 *)&Vertex[0].Pnt,
									    Vertex[ic].Tan, &Prim.Arc);
			}
			// 情報をリストへ追加
			if (Block == NULL) {														// ﾚｲﾔへ追加
				PrmPtr = LayPtr->AddPrim(PrmType, Color, LineNo, LineWd, &Prim);		// 図形情報ｺﾝｽﾄﾗｸﾀ
			} else {																	// ﾌﾞﾛｯｸへ追加
				PrmPtr = Block->AddPrim(PrmType, Color, LineNo, LineWd, &Prim);			// 図形情報ｺﾝｽﾄﾗｸﾀ
			}
		}
	}

PRG_EXIT:

	HFREE(Vertex);
	HFREE(MVerNumPnt);
	HFREE(MPntNo);
	HFREE(MPnt);

	return(Status);
}
/******************************************************************************************************************
[機能] 文字情報を読込み、レイヤもしくはブロック情報のプリミティブリストへ追加する
[返値] = 0 正常  < 0エラー  > 0 警告 非表示項目である
******************************************************************************************************************/
MINT MDxfGetPrmTxt        (															// (  O) ｽﾃｰﾀｽ
							CStdioFile*			FLPtr,									// (I  ) ﾌｧｲﾙ識別子
							MCHAR*				Data,									// (  O) 読込み領域
							MDxfInf*			DxfInf,									// (I/O) DXF情報
							MDxfBlock*			Block)									// (I  ) ﾌﾞﾛｯｸ
																						//       == NULL ﾚｲﾔへ読込む
																						//       != NULL 指定ﾌﾞﾛｯｸへ読込む
{
	MINT			Status;																// ｽﾃｰﾀｽ
	MINT			Code;
	MINT			PrmType;															// ﾌﾟﾘﾐﾃｨﾌﾞﾀｲﾌﾟ
	MINT			Color;																// 色番号  （番号）
	HXPRIM			Prim;
	MCHAR			FnName[NAMMAX];														// ﾌｫﾝﾄ名
	MCHAR			LyName[NAMMAX];														// ﾚｲﾔ名
	MCHAR			LnName[NAMMAX];														// 線種名
	HBOOL			LyrOK;																// ﾚｲﾔ情報があったか
	MDxfLayer*		LayPtr;																// ﾚｲﾔ情報ﾎﾟｲﾝﾀ
	MDxfFonts*		FntPtr;																// ﾌｫﾝﾄ情報ﾎﾟｲﾝﾀ
	MDxfPrim*		PrmPtr;
	MINT			LColor;																// 色番号  （番号）
	MINT			FCode;																// ﾌｫﾝﾄ番号（番号）
	MINT			TxtLen;																// 文字列長さ


	// 初期設定
	LyrOK            = HFALSE;															// ﾚｲﾔ名定義なし
	PrmType          = HPRIM_TXT;														// ﾀｲﾌﾟ  ：文字
	Prim.Text.String = NULL;															// 文字列：NULL
	Color            = 256;																// 色番号：
	BwsStrCpy(FnName, BwsStr("STANDARD"));												// ﾌｫﾝﾄ名：STANDARD
	BwsStrCpy(LnName, BwsStr("BYLAYER"));												// 線種名：
	ZeroMemory(&Prim, sizeof(HXPRIM));													// ｸﾘｱ

	while ((Status = MDxfGetLine(FLPtr, &Code, Data)) == 0) {
		if (Code == 6) {																// 線種名
			BwsSscanf(Data, BwsStr("%s"), LnName);
		} else if (Code == 7) {															// 文字ｽﾀｲﾙ名
			BwsSscanf(Data, BwsStr("%s"), FnName);
		} else if (Code == 8) {															// ﾚｲﾔ名
			LyrOK = HTRUE;
			BwsSscanf(Data, BwsStr("%s"), LyName);
		} else if (Code == 1) {															// 文字列
			TxtLen = BwsStrLen(Data);
			if ((Prim.Text.String = (MCHAR *)malloc(sizeof(MCHAR) * (TxtLen+1))) == NULL)  HMFAIL;
			Prim.Text.Num = TxtLen;														// 文字数
			BwsStrCpy(Prim.Text.String, Data);
		} else if (Code == 10) {														// 開始点X
			BwsSscanf(Data, BwsStr("%f"), &Prim.Text.Pnt.X);
		} else if (Code == 20) {														// 開始点Y
			BwsSscanf(Data, BwsStr("%f"), &Prim.Text.Pnt.Y);
		} else if (Code == 30) {														// 開始点Z
			BwsSscanf(Data, BwsStr("%f"), &Prim.Text.Pnt.Z);
		} else if (Code == 40) {														// 文字高さ
			BwsSscanf(Data, BwsStr("%f"), &Prim.Text.Hei);
		} else if (Code == 50) {														// 回転角度
			BwsSscanf(Data, BwsStr("%f"), &Prim.Text.Ang);
		} else if (Code == 62) {														// 色番号
			BwsSscanf(Data, BwsStr("%d"), &Color);
		} else if (Code == 0) {															// 区切り文字
			if (LyrOK != HTRUE) {														// ﾚｲﾔ名が未定義
				Status = -1;
				goto  PRG_EXIT;
			}
			break;
		}
	}

	LayPtr = DxfInf->GetLayer(LyName, &LColor);											// ﾚｲﾔ情報検索
	if (LayPtr == NULL)  goto  PRG_EXIT;

	// 色をチェック
	if (Color == 0) {																	// BYBLOCK
		// 挿入時に検討
	} else if (Color == 256) {															// BYLAYER
		Color = LColor;
	} else if (Color < 0) {																// 画層OFF
		Status = 1;																		// 非表示項目は読込まない
		goto  PRG_EXIT;
	}

	FntPtr = DxfInf->GetFonts(FnName, &FCode);											// ﾌｫﾝﾄ情報検索
	if (FntPtr == NULL)  goto  PRG_EXIT;

	if ((FCode & 4) == 4) {																// 文字方向４（縦）のビットが立っている
		Prim.Text.Msc = -1.0f;
	} else {
		Prim.Text.Msc =  1.0f;
	}

	// 情報をリストへ追加
	if (Block == NULL) {																// ﾚｲﾔへ追加
		PrmPtr = LayPtr->AddPrim(PrmType, Color, 0, 3, &Prim);							// 図形情報ｺﾝｽﾄﾗｸﾀ
	} else {																			// ﾌﾞﾛｯｸへ追加
		PrmPtr = Block->AddPrim(PrmType, Color, 0, 3, &Prim);							// 図形情報ｺﾝｽﾄﾗｸﾀ
	}

PRG_EXIT:

	if (Prim.Text.String)  free(Prim.Text.String);

	return(Status);
}

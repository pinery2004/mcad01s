/******************************************************************************************************************
              ＤＸＦ  プログラムファイル (MDxf.cpp)

[機能] ＤＸＦファイル読込み
[返値] = 0 正常  < 0エラー
******************************************************************************************************************/


#include		"MCad.h"
#include		"MgLib.h"
//#include		"HsFileLib.h"
#include		"MDxf.h"


MDxfInf* MDxfRead            (														// (  O) DXF情報
								MCHAR*				FlName,								// (I  ) ﾌｧｲﾙ名
								MINT				VewTyp,								// (I  ) ﾋﾞｭｰ種類
								MINT				ZukVec,								// (I  ) 1:上下 2:左右 4:-90°5:+90°
								MFLOAT				MVal,								// (I  ) 移動量
								MINT				FAFlag)								// (I  ) 面の向き調整   0:しない  1:する
{
	MDxfInf*			DxfInf;															// DXF情報
	CStdioFile*			FLPtr;															// ﾌｧｲﾙ識別子
	MINT				Code;
	MCHAR				Data[512];
	MgMinMax3			Window;															// ｳｨﾝﾄﾞｳｻｲｽﾞ
	MgMinMax3			ViewPort;														// ﾋﾞｭｰﾎﾟｰﾄ
	HMTRX3				Matrix;															// 回転ﾏﾄﾘｯｸｽ
	HMTRX3				Matrix2;														// 回転ﾏﾄﾘｯｸｽ
	MgPoint3			Move;															// 移動量
	MgMinMax3			MinMax;
	MINT				ItrSts;															// ｽﾃｰﾀｽ


	DxfInf = NULL;

	FLPtr = HsFileOpenS(FlName, 1, 2);													// ﾌｧｲﾙｵｰﾌﾟﾝ(ﾃｷｽﾄ)
	if (FLPtr == NULL)  goto  PRG_EXIT;

	Window.Min.X   = Window.Min.Y   = Window.Min.Z   = 0.0f;
	Window.Max.X   = Window.Max.Y   = Window.Max.Z   = 0.0f;
	ViewPort.Min.X = ViewPort.Min.Y = ViewPort.Min.Z = 0.0f;
	ViewPort.Max.X = ViewPort.Max.Y = ViewPort.Max.Z = 0.0f;

	DxfInf = new MDxfInf(VewTyp, 1.0f, -1, 1.0f, 0, &Window, &ViewPort, FAFlag);		// DXF情報ｺﾝｽﾄﾗｸﾀ
	DxfInf->AddLayer(BwsStr("0"), 0, HLINE_SOLD);										// ﾚｲﾔ"0"追加  色:白  線番号:実線

	while (MDxfGetLine(FLPtr, &Code, Data) == 0) {										// 読込み
		if (BwsStrCmp(Data, BwsStr("SECTION")) == 0) {
			if (MDxfGetLine(FLPtr, &Code, Data) != 0) goto  PRG_ERROR;
			if (Code != 2)  goto  PRG_ERROR;

			// セクションの振り分け
			if (BwsStrCmp(Data, BwsStr("HEADER")) == 0) {
				ItrSts = MDxfGetHdSec(FLPtr, DxfInf);
				if (ItrSts != 0)  goto  PRG_ERROR;
			} else if (BwsStrCmp(Data, BwsStr("TABLES")) == 0) {
				ItrSts = MDxfGetTblSec(FLPtr, DxfInf);
				if (ItrSts != 0)  goto  PRG_ERROR;
			} else if (BwsStrCmp(Data, BwsStr("BLOCKS")) == 0) {
				ItrSts = MDxfGetBlcSec(FLPtr, DxfInf);
				if (ItrSts != 0)  goto  PRG_ERROR;
			} else if (BwsStrCmp(Data, BwsStr("ENTITIES")) == 0) {
				ItrSts = MDxfGetEntSec(FLPtr, DxfInf);
				if (ItrSts != 0)  goto  PRG_ERROR;
			} else if (BwsStrCmp(Data, BwsStr("CLASSES")) == 0) {
				ItrSts = MDxfGetClsSec(FLPtr);
				if (ItrSts != 0)  goto  PRG_ERROR;
			} else if (BwsStrCmp(Data, BwsStr("OBJECTS")) == 0) {
				ItrSts = MDxfGetObjSec(FLPtr);
				if (ItrSts != 0)  goto  PRG_ERROR;
			} else {
				goto  PRG_ERROR;
			}
		} else if (BwsStrCmp(Data, BwsStr("EOF")) == 0) {								// DXFﾌｧｲﾙ正常に終了
			ItrSts = 0;
			break;
		} else {																		// ｴﾗｰ
			if (ItrSts != 0)  goto  PRG_ERROR;
			break;
		}
	}

	if (VewTyp >= HVIEWT_SYO && VewTyp <= HVIEWT_RSK)  ItrSts = DxfInf->TransformYZ();	// 立面はYとZを交換

	Move.X = Move.Y = Move.Z = 0.f;
	if (ZukVec == 1) {
		HgUDMirMtxt(Matrix);															// 上下反転
		ItrSts = DxfInf->Transform(1, Matrix);											// 移動･回転
	} else if (ZukVec == 2) {															// 左右対称
		HgUDMirMtxt(Matrix);															// 上下反転
		HgGetMtxByAng(180.f, Matrix2);
		HgMultiMtx(Matrix, Matrix2);													// ﾏﾄﾘｯｸｽとﾏﾄﾘｯｸｽを掛ける
		ItrSts = DxfInf->Transform(1, Matrix);											// 移動･回転
	} else if (ZukVec == 3) {															// 180°回転
		HgGetMtxByAng(180.f, Matrix);
		ItrSts = DxfInf->Transform(0, Matrix);											// 移動･回転
	} else if (ZukVec == 4) {															// -90°回転
		HgGetMtxByAng(-90.f, Matrix);
		ItrSts = DxfInf->Transform(0, Matrix);											// 移動･回転
	} else if (ZukVec == 5) {															// 90°回転
		HgGetMtxByAng(+90.f, Matrix);
		ItrSts = DxfInf->Transform(0, Matrix);											// 移動･回転
	} else if (ZukVec == 6) {															// X移動
		Move.X = -MVal;
		ItrSts = DxfInf->Transform(0, NULL, &Move);										// 移動･回転
	} else if (ZukVec == 7) {															// Y移動
		Move.Y = -MVal;
		ItrSts = DxfInf->Transform(0, NULL, &Move);										// 移動･回転
	} else if (ZukVec == 8) {															// Z移動
		Move.Z = -MVal;
		ItrSts = DxfInf->Transform(0, NULL, &Move);										// 移動･回転
	} else if (ZukVec == 9) {															// 中心補正
		DxfInf->GetMinMax(0, NULL, NULL, NULL, &MinMax);
		Move.X = +(MinMax.Max.X - MinMax.Min.X) / 2.f;
		Move.Y = -(MinMax.Max.Y - MinMax.Min.Y) / 2.f;
//		Move.Z = +(MinMax.Max.Z - MinMax.Min.Z) / 2.f;
		ItrSts = DxfInf->Transform(0, NULL, &Move);										// 移動
	} else if (ZukVec == 12) {															// 左右対称&中心補正
		DxfInf->GetMinMax(0, NULL, NULL, NULL, &MinMax);
		HgUDMirMtxt(Matrix);															// 上下反転
		HgGetMtxByAng(180.f, Matrix2);
		HgMultiMtx(Matrix, Matrix2);													// ﾏﾄﾘｯｸｽとﾏﾄﾘｯｸｽを掛ける
		Move.X = +(MinMax.Max.X - MinMax.Min.X) / 2.f;
		Move.Y = +(MinMax.Max.Y - MinMax.Min.Y) / 2.f;
		Move.Z = +(MinMax.Max.Z - MinMax.Min.Z) / 2.f;
		ItrSts = DxfInf->Transform(1, Matrix, &Move);									// 移動･回転
	} else if (ZukVec == 15) {															// 90°回転&中心補正
		DxfInf->GetMinMax(0, NULL, NULL, NULL, &MinMax);
		HgGetMtxByAng(+90.f, Matrix);
		Move.X = +(MinMax.Max.Y - MinMax.Min.Y) / 2.f;
		Move.Y = -(MinMax.Max.X - MinMax.Min.X) / 2.f;
		Move.Z = +(MinMax.Max.Z - MinMax.Min.Z) / 2.f;
		ItrSts = DxfInf->Transform(0, Matrix, &Move);									// 移動･回転
	} else {
		ItrSts = DxfInf->Transform();													// 最少最大ｾｯﾄ
	}

PRG_ERROR:

	ItrSts = HsFileCloseS(FLPtr, 0);													// ﾌｧｲﾙｸﾛｰｽﾞ(ﾃｷｽﾄ)

PRG_EXIT:

	return(DxfInf);
}

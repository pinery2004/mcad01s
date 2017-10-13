/*********************************************************************************************************************************
              ＤＸＦ  プログラムファイル (MDxf.cpp)

********************************************************************************************************************************/

#include		"MgLib.h"
#include		"HEtcLib.h"
#include		"HsView.h"
#include		"HsLib.h"
#include		"HnLib.h"
#include		"MDxfR14.h"
#include		"HsDirectXLib.h"
#include		"HsDHon.h"
#include		"HsRoof.h"
#include		"HsDHai.h"
#include		"MDxf.h"


#define			 OCOLOR			RGB(255, 0, 0)											// 配置点色


// 最小･最大を設定
static HVOID SetMinMax   (MgPoint3* Pnt, MgMinMax3* MinMax);
static HVOID SetMinMax   (MgPoint2* Pnt, MgMinMax3* MinMax);
// Ｖｅｒｔｅｘへ情報を設定
static MINT SetVertex    (MINT NumPnt, MgPoint3* Pnt, D3DCOLOR Rgb, MINT* NumTri, HDXVERTEX* Vertex, MINT* IPtr);
// ローカル座標系へ変換
static HVOID TrnWrdToLoc (MgPoint3* TOPnt, MgPoint3* TUvw, MgPoint3* Pnt1, MgPoint2* Pnt2);


/******************************************************************************************************************
[ MDxfInf::MDxfInf ]                 機能  ＤＸＦ情報コンストラクタ
******************************************************************************************************************/
MDxfInf::MDxfInf             (
								MINT				VewTyp,								// (I  ) ﾋﾞｭｰ種類
								MFLOAT				ProtRate,							// (I  ) ﾌﾟﾛｯﾄﾚｰﾄ
								MINT				AngDir,								// (I  ) 円弧方向 1:時計 -1:反時計
								MFLOAT				TextRate,							// (I  ) 文字変換倍率
								MINT				ZmnSize,							// (I  ) 図面ｻｲｽﾞ
								MgMinMax3*			Window,								// (I  ) ｳｨﾝﾄﾞｳｻｲｽﾞ
								MgMinMax3*			ViewPort,							// (I  ) ﾋﾞｭｰﾎﾟｰﾄ
								MINT				FAFlag)								// (I  ) 面の向き調整   0:しない  1:する

{
	// オプションを設定
	m_VewTyp       = VewTyp,															// ﾋﾞｭｰ種類
	m_ProtRate     = ProtRate;															// ﾌﾟﾛｯﾄﾚｰﾄ
	m_AngDir       = AngDir;															// 反時計周り
	m_Symtry       = 0;																	// 対称ﾌﾗｸﾞ       0:なし      1:あり
	m_SymtryC      = 0;																	// 現在の対称ﾌﾗｸﾞ 0:なし      1:あり
	m_Matrix[0][0] = 1.f; m_Matrix[0][1] = 0.f; m_Matrix[0][2] = 0.f;					// 変換ﾏﾄﾘｸｽ
	m_Matrix[1][0] = 0.f; m_Matrix[1][1] = 1.f; m_Matrix[1][2] = 0.f;
	m_Matrix[2][0] = 0.f; m_Matrix[2][1] = 0.f; m_Matrix[2][2] = 1.f;
	m_Move.X       = m_Move.Y   = m_Move.Z   = 0.f;										// 移動量
	m_Scale[0]     = m_Scale[1] = m_Scale[2] = 1.0f;									// 拡大率
	m_TextRate     = TextRate;															// 文字変換倍率
	m_ZmnSize      = ZmnSize;															// 図面ｻｲｽﾞ
	memcpy(&m_Window,   Window,   sizeof(MgMinMax3));									// ｳｨﾝﾄﾞｳｻｲｽﾞ
	memcpy(&m_ViewPort, ViewPort, sizeof(MgMinMax3));									// ﾋﾞｭｰﾎﾟｰﾄ
	m_FAFlag       = FAFlag;															// 面の向き調整   0:しない    1:する
	m_MinMax.Min.X = m_MinMax.Min.Y = m_MinMax.Min.Z =  FLT_MAX;						// 最小･最大
	m_MinMax.Max.X = m_MinMax.Max.Y = m_MinMax.Max.Z = -FLT_MAX;
	m_TextureY     = NULL;																// ﾃｷｽﾁｬ（矢印）
	m_VertBufY     = NULL;																// DirectX 頂点ﾊﾞｯﾌｧ（矢印）
	m_NumTriY      = 0;																	// ３角形の数       （矢印）
	m_TextFlg      = 0;																	// ﾃｷｽﾁｬﾌﾗｸﾞ      0:管理      1:内臓

	m_BlockList = new HlDList();									 					// ﾌﾞﾛｯｸ･双方向ﾎﾟｲﾝﾀ
	m_LayerList = new HlDList();									 					// ﾚｲﾔ･双方向ﾎﾟｲﾝﾀ
	m_LinesList = new HlDList();									 					// 線種･双方向ﾎﾟｲﾝﾀ
	m_FontsList = new HlDList();									 					// ﾌｫﾝﾄ･双方向ﾎﾟｲﾝﾀ

}

/******************************************************************************************************************
[ MDxfInf::~MDxfInf ]                機能  ＤＸＦ情報デストラクタ
******************************************************************************************************************/
MDxfInf::~MDxfInf          ()
{

	if (m_TextureY)  RELEASE(m_TextureY);												// ﾃｷｽﾁｬ（矢印）
	m_TextureY = NULL;

	if (m_VertBufY)  RELEASE(m_VertBufY);												// DirectX 頂点ﾊﾞｯﾌｧ（矢印）
	m_VertBufY = NULL;

	ClrBlock();
	HDELETE(m_BlockList);										 						// ﾌﾞﾛｯｸ･双方向ﾎﾟｲﾝﾀ

	ClrLayer();
	HDELETE(m_LayerList);										 						// ﾚｲﾔ･双方向ﾎﾟｲﾝﾀ

	ClrLines();
	HDELETE(m_LinesList);										 						// 線種･双方向ﾎﾟｲﾝﾀ

	ClrFonts();
	HDELETE(m_FontsList);										 						// ﾌｫﾝﾄ･双方向ﾎﾟｲﾝﾀ

}

/******************************************************************************************************************
[ MDxfInf::AddBlock ]                 機能  ＤＸＦ情報にブロック情報を追加
******************************************************************************************************************/
MDxfBlock* MDxfInf::AddBlock ()														// (  O) ﾌﾞﾛｯｸ情報ｲﾝｽﾀﾝｽ
{
	MDxfBlock*			Ptr;															// ﾌﾞﾛｯｸ情報ｲﾝｽﾀﾝｽ


	Ptr = new MDxfBlock();																// ﾌﾞﾛｯｸ情報ｺﾝｽﾄﾗｸﾀ
	m_BlockList->Inject((HVOID *)Ptr);													// ﾌﾞﾛｯｸ情報ﾘｽﾄへ追加

	return(Ptr);
}

/******************************************************************************************************************
[ MDxfInf::AddLayer ]                 機能  ＤＸＦ情報にレイヤ情報を追加
******************************************************************************************************************/
MINT MDxfInf::AddLayer        (														// (  O) ｽﾃｰﾀｽ  0:追加  1:更新
								MCHAR*				Name,								// (I  ) ﾚｲﾔ名
								MINT				Color,								// (I  ) 色番号
								MINT				LineNo)								// (I  ) 線番号
{
	MINT				Status;
	MDxfLayer*			Ptr;															// ﾚｲﾔ情報
	MINT				No;


	Status = 0;

	if (m_LayerList != NULL) {
		Ptr = (MDxfLayer *)m_LayerList->Head();										// 先頭のﾎﾟｲﾝﾀを得る
		No  = 0;
		while (Ptr != NULL) {
			if (BwsStrCmp(Name, Ptr->m_Name) == 0) {									// ﾚｲﾔ名一致
				Ptr->Modify(Color, LineNo);
				Status = 1;																// 更新
				break;
			}
			Ptr = (MDxfLayer *)m_LayerList->Next();									// 次のﾎﾟｲﾝﾀを得る
			No++;
		}
		if (Status == 0) {
			Ptr = new MDxfLayer(Name, Color, LineNo);									// ﾚｲﾔ情報ｺﾝｽﾄﾗｸﾀ
			m_LayerList->Inject((HVOID *)Ptr);											// ﾚｲﾔ情報ﾘｽﾄへ追加
		}
	}

	return(Status);
}

/******************************************************************************************************************
[ MDxfInf::AddLines ]                 機能  ＤＸＦ情報に線情報を追加
******************************************************************************************************************/
MINT MDxfInf::AddLines    (															// (  O) ｽﾃｰﾀｽ  0:正常  1:ｴﾗｰ
							MCHAR*				Name,									// (I  ) 線名
							MINT				LineNo)									// (I  ) 線番号
{
	MINT				Status;
	MDxfLines*			Ptr;															// 線情報


	Status = 0;

	Ptr    = new MDxfLines(Name, LineNo);												// 線情報ｺﾝｽﾄﾗｸﾀ
	m_LinesList->Inject((HVOID *)Ptr);													// 線情報ﾘｽﾄへ追加

	return(Status);
}

/******************************************************************************************************************
[ MDxfInf::AddFonts ]                 機能  ＤＸＦ情報にフォント情報を追加
******************************************************************************************************************/
MINT MDxfInf::AddFonts    (															// (  O) ｽﾃｰﾀｽ  0:正常  1:ｴﾗｰ
							MCHAR*				Name,									// (I  ) ﾌｫﾝﾄ名
							MINT				FCode)									// (I  ) ﾌｫﾝﾄｺｰﾄﾞ
{
	MINT				Status;
	MDxfFonts*			Ptr;															// ﾌｫﾝﾄ情報の構造体


	Status = 0;

	Ptr    = new MDxfFonts(Name, FCode);												// ﾌｫﾝﾄ情報ｺﾝｽﾄﾗｸﾀ
	m_FontsList->Inject((HVOID *)Ptr);													// ﾌｫﾝﾄ情報ﾘｽﾄへ追加

	return(Status);
}

/******************************************************************************************************************
[ MDxfInf::GetBlock ]                 機能  ＤＸＦ情報より指定したブロック情報を検索
******************************************************************************************************************/
MDxfBlock* MDxfInf::GetBlock (														// (  O) ﾌﾞﾛｯｸ情報  NULL:見つからず
								MCHAR*			Name,									// (I  ) ﾌﾞﾛｯｸ名
								MCHAR*			LName,									// (  O) ﾚｲﾔ名
								MINT*			Color,									// (  O) 色番号
								MINT*			LineNo,									// (  O) 線番号
								MgPoint3*		InsPnt)									// (  O) 挿入基点
{
	MDxfBlock*			Ptr;															// ﾌﾞﾛｯｸ情報


	if (m_BlockList != NULL) {
		Ptr = (MDxfBlock *)m_BlockList->Head();										// 先頭のﾎﾟｲﾝﾀを得る
		while (Ptr != NULL) {
			if (BwsStrCmp(Name, Ptr->m_Name) == 0) {									// ﾌﾞﾛｯｸ名一致
				if (LName)   BwsStrCpy(LName, Ptr->m_LName);							// ﾚｲﾔ名
				if (Color)   *Color  = Ptr->m_Color;
				if (LineNo)  *LineNo = Ptr->m_LineNo;
				if (InsPnt)  memcpy(InsPnt, &Ptr->m_InsPnt, sizeof(MgPoint3));
				return(Ptr);
			}
			Ptr = (MDxfBlock *)m_BlockList->Next();									// 次のﾎﾟｲﾝﾀを得る
		}
	}

	return(NULL);																		// 見つからず
}

/******************************************************************************************************************
[ MDxfInf::GetLayer ]                 機能  ＤＸＦ情報より指定したレイヤ情報を検索
******************************************************************************************************************/
MDxfLayer* MDxfInf::GetLayer (														// (  O) ﾚｲﾔ情報  NULL:見つからず
								MCHAR*			Name,									// (I  ) ﾚｲﾔ名
								MINT*			Color,									// (  O) 色番号
								MINT*			LineNo)									// (  O) 線番号
{
	MDxfLayer*			Ptr;															// ﾚｲﾔ情報


	if (m_LayerList != NULL) {
		Ptr = (MDxfLayer *)m_LayerList->Head();										// 先頭のﾎﾟｲﾝﾀを得る
		while (Ptr != NULL) {
			if (BwsStrCmp(Name, Ptr->m_Name) == 0) {									// ﾚｲﾔ名一致
				if (Color)   *Color  = Ptr->m_Color;
				if (LineNo)  *LineNo = Ptr->m_LineNo;
				return(Ptr);
			}
			Ptr = (MDxfLayer *)m_LayerList->Next();									// 次のﾎﾟｲﾝﾀを得る
		}
	}

	Ptr = (MDxfLayer *)m_LayerList->Head();											// 先頭のﾎﾟｲﾝﾀを得る
	if (Color)   *Color  = Ptr->m_Color;
	if (LineNo)  *LineNo = Ptr->m_LineNo;
	return(Ptr);																		// 見つからず
}

/******************************************************************************************************************
[ MDxfInf::GetLines ]                 機能  ＤＸＦ情報より指定した線情報を検索
******************************************************************************************************************/
MDxfLines* MDxfInf::GetLines (														// (  O) 線情報のｲﾝｽﾀﾝｽ
								MCHAR*			Name,									// (I  ) 線名
								MINT*			LineNo)									// (  O) 線番号
{
	MDxfLines*			Ptr;															// 線情報


	if (m_LinesList != NULL) {
		Ptr = (MDxfLines *)m_LinesList->Head();										// 先頭のﾎﾟｲﾝﾀを得る
		while (Ptr != NULL) {
			if (BwsStrCmp(Name, Ptr->m_Name) == 0) {									// 線名一致
				if (LineNo)  *LineNo = Ptr->m_LineNo;
				return(Ptr);
			}
			Ptr = (MDxfLines *)m_LinesList->Next();									// 次のﾎﾟｲﾝﾀを得る
		}
	}

	return(NULL);
}

/******************************************************************************************************************
[ MDxfInf::GetFonts ]                 機能  ＤＸＦ情報より指定したフォント情報を検索
******************************************************************************************************************/
MDxfFonts* MDxfInf::GetFonts (														// (  O) ﾌｫﾝﾄ情報のｲﾝｽﾀﾝｽ
								MCHAR*			Name,									// (I  ) ﾌｫﾝﾄ名
								MINT*			FCode)									// (  O) ﾌｫﾝﾄ番号
{
	MDxfFonts*			Ptr;															// ﾌｫﾝﾄ情報


	if (m_FontsList != NULL) {
		Ptr = (MDxfFonts *)m_FontsList->Head();										// 先頭のﾎﾟｲﾝﾀを得る
		while (Ptr != NULL) {
			if (BwsStrCmp(Name, Ptr->m_Name) == 0) {									// ﾌｫﾝﾄ名一致
				if (FCode)  *FCode = Ptr->m_FCode;
				return(Ptr);
			}
			Ptr = (MDxfFonts *)m_FontsList->Next();									// 次のﾎﾟｲﾝﾀを得る
		}
	}

	return(NULL);
}

/******************************************************************************************************************
[ MDxfInf::ClrBlock ]                 機能  ＤＸＦ情報のブロック情報をクリア
******************************************************************************************************************/
HVOID MDxfInf::ClrBlock       ()
{
	MDxfBlock*			Ptr;															// ﾌﾞﾛｯｸ情報へのﾎﾟｲﾝﾀ


	if (m_BlockList != NULL) {
		Ptr = (MDxfBlock *)m_BlockList->Head();										// 先頭のﾎﾟｲﾝﾀを得る
		while (Ptr != NULL) {
			delete  Ptr;
			Ptr = (MDxfBlock *)m_BlockList->Next();									// 次のﾎﾟｲﾝﾀを得る
		}
	}
}

/******************************************************************************************************************
[ MDxfInf::ClrLayer ]                 機能  ＤＸＦ情報のレイヤ情報をクリア
******************************************************************************************************************/
HVOID MDxfInf::ClrLayer       ()
{
	MDxfLayer*			Ptr;															// ﾚｲﾔ情報へのﾎﾟｲﾝﾀ


	if (m_LayerList != NULL) {
		Ptr = (MDxfLayer *)m_LayerList->Head();										// 先頭のﾎﾟｲﾝﾀを得る
		while (Ptr != NULL) {
			delete  Ptr;
			Ptr = (MDxfLayer *)m_LayerList->Next();									// 次のﾎﾟｲﾝﾀを得る
		}
	}
}

/******************************************************************************************************************
[ MDxfInf::ClrLines ]                 機能  ＤＸＦ情報の線情報をクリア
******************************************************************************************************************/
HVOID MDxfInf::ClrLines       ()
{
	MDxfLines*			Ptr;															// 線情報へのﾎﾟｲﾝﾀ


	if (m_LinesList != NULL) {
		Ptr = (MDxfLines *)m_LinesList->Head();										// 先頭のﾎﾟｲﾝﾀを得る
		while (Ptr != NULL) {
			delete  Ptr;
			Ptr = (MDxfLines *)m_LinesList->Next();									// 次のﾎﾟｲﾝﾀを得る
		}
	}
}

/******************************************************************************************************************
[ MDxfInf::ClrFonts ]                 機能  ＤＸＦ情報のフォント情報をクリア
******************************************************************************************************************/
HVOID MDxfInf::ClrFonts       ()
{
	MDxfFonts*			Ptr;															// ﾌｫﾝﾄ情報へのﾎﾟｲﾝﾀ


	if (m_FontsList != NULL) {
		Ptr = (MDxfFonts *)m_FontsList->Head();										// 先頭のﾎﾟｲﾝﾀを得る
		while (Ptr != NULL) {
			delete  Ptr;
			Ptr = (MDxfFonts *)m_FontsList->Next();									// 次のﾎﾟｲﾝﾀを得る
		}
	}
}

/******************************************************************************************************************
[ MDxfInf::Transform ]                機能  ＤＸＦ情報の変形
******************************************************************************************************************/
MINT MDxfInf::Transform       (														// (  O) ｽﾃｰﾀｽ  0:正常  1:ｴﾗｰ
								MINT				Symtry,								// (I  ) 対称ﾌﾗｸﾞ  0:なし  1:あり
								HMTRX3				Matrix,								// (I  ) 変換ﾏﾄﾘｸｽ
								MgPoint3*			Move,								// (I  ) 移動量
								MFLOAT*				Scale)								// (I  ) ｽｹｰﾙ
{
	MINT				Status;
	MDxfLayer*			LayPtr;															// ﾚｲﾔ情報
	MDxfBlock*			BlkPtr;
	MDxfPrim*			PrmPtr;


	Status = 0;

	m_Symtry = m_SymtryC = Symtry;
	if (Matrix)  memcpy(&m_Matrix, Matrix, sizeof(HMTRX3));
	if (Move)    memcpy(&m_Move,   Move,   sizeof(MgPoint3));
	if (Scale)   memcpy(&m_Scale,  Scale,  sizeof(MFLOAT)*3);

	if (m_LayerList != NULL) {
		LayPtr = (MDxfLayer *)m_LayerList->Head();										// ﾚｲﾔ情報先頭
		while (LayPtr != NULL) {
			if (LayPtr->m_PrimList != NULL) {
				PrmPtr = (MDxfPrim *)LayPtr->m_PrimList->Head();						// 図形情報先頭
				while (PrmPtr != NULL) {
					PrmPtr->Transform(1, Symtry, m_Matrix, &m_Move, m_Scale, &m_MinMax);// 図形の変形
					PrmPtr = (MDxfPrim *)LayPtr->m_PrimList->Next();					// 図形情報次
				}
			}
			LayPtr = (MDxfLayer *)m_LayerList->Next();									// ﾚｲﾔ情報次
		}
	}

	if (m_BlockList != NULL) {
		BlkPtr = (MDxfBlock *)m_BlockList->Head();										// ﾌﾞﾛｯｸ情報先頭
		while (BlkPtr != NULL) {
			if (BlkPtr->m_PrimList != NULL) {
				PrmPtr = (MDxfPrim *)BlkPtr->m_PrimList->Head();						// 図形情報先頭
				while (PrmPtr != NULL) {
					PrmPtr->Transform(1, Symtry, m_Matrix, &m_Move, m_Scale, &m_MinMax);// 図形の変形
					PrmPtr = (MDxfPrim *)BlkPtr->m_PrimList->Next();					// 図形情報次
				}
			}
			BlkPtr = (MDxfBlock *)m_BlockList->Next();									// ﾌﾞﾛｯｸ情報次
		}
	}

	return(Status);
}

/******************************************************************************************************************
[ MDxfInf::TransformYZ ]              機能  ＤＸＦ情報の変形（Ｙ→Ｚ）
******************************************************************************************************************/
MINT MDxfInf::TransformYZ     ()														// (  O) ｽﾃｰﾀｽ  0:正常  1:ｴﾗｰ
{
	MINT				Status;
	MDxfLayer*			LayPtr;															// ﾚｲﾔ情報
	MDxfBlock*			BlkPtr;
	MDxfPrim*			PrmPtr;


	Status = 0;

	if (m_LayerList != NULL) {
		LayPtr = (MDxfLayer *)m_LayerList->Head();										// ﾚｲﾔ情報先頭
		while (LayPtr != NULL) {
			if (LayPtr->m_PrimList != NULL) {
				PrmPtr = (MDxfPrim *)LayPtr->m_PrimList->Head();						// 図形情報先頭
				while (PrmPtr != NULL) {
					PrmPtr->TransformYZ(&m_MinMax);										// 図形の変形
					PrmPtr = (MDxfPrim *)LayPtr->m_PrimList->Next();					// 図形情報次
				}
			}
			LayPtr = (MDxfLayer *)m_LayerList->Next();									// ﾚｲﾔ情報次
		}
	}

	if (m_BlockList != NULL) {
		BlkPtr = (MDxfBlock *)m_BlockList->Head();										// ﾌﾞﾛｯｸ情報先頭
		while (BlkPtr != NULL) {
			if (BlkPtr->m_PrimList != NULL) {
				PrmPtr = (MDxfPrim *)BlkPtr->m_PrimList->Head();						// 図形情報先頭
				while (PrmPtr != NULL) {
					PrmPtr->TransformYZ(&m_MinMax);										// 図形の変形
					PrmPtr = (MDxfPrim *)BlkPtr->m_PrimList->Next();					// 図形情報次
				}
			}
			BlkPtr = (MDxfBlock *)m_BlockList->Next();									// ﾌﾞﾛｯｸ情報次
		}
	}

	return(Status);
}

/******************************************************************************************************************
[ MDxfInf::GetMinMax ]                機能  最小最大を得る
******************************************************************************************************************/
HVOID MDxfInf::GetMinMax      (														// (  O) ｽﾃｰﾀｽ  0:正常  1:ｴﾗｰ
								MgMinMax3*			MinMax)								// (  O) 最小･最大
{
	memcpy(MinMax, &m_MinMax, sizeof(MgMinMax3));

	return;
}

/******************************************************************************************************************
[ MDxfInf::GetMinMax ]                機能  回転・移動後の最小最大を得る
******************************************************************************************************************/
HVOID MDxfInf::GetMinMax      (														// (  O) ｽﾃｰﾀｽ  0:正常  1:ｴﾗｰ
								MINT				Symtry,								// (I  ) 対称ﾌﾗｸﾞ  0:なし  1:あり
								HMTRX3				Matrix,								// (I  ) 変換ﾏﾄﾘｸｽ
								MgPoint3*			Move,								// (I  ) 移動量
								MFLOAT*				Scale,								// (I  ) ｽｹｰﾙ
								MgMinMax3*			MinMax)								// (  O) 最小･最大
{
	HMTRX3				WMatrix;														// 変換ﾏﾄﾘｸｽ
	MgPoint3				WMove;															// 移動量
	MFLOAT				WScale[3];														// ｽｹｰﾙ
	MDxfLayer*			LayPtr;															// ﾚｲﾔ情報
	MDxfBlock*			BlkPtr;
	MDxfPrim*			PrmPtr;


	if (Matrix) {
		memcpy(WMatrix, Matrix, sizeof(HMTRX3));
	} else {
		WMatrix[0][0] = 1.f; WMatrix[0][1] = 0.f; WMatrix[0][2] = 0.f;
		WMatrix[1][0] = 0.f; WMatrix[1][1] = 1.f; WMatrix[1][2] = 0.f;
		WMatrix[2][0] = 0.f; WMatrix[2][1] = 0.f; WMatrix[2][2] = 1.f;
	}
	if (Move) {
		memcpy(&WMove, Move, sizeof(MgPoint3));
	} else {
		WMove.X = WMove.Y = WMove.Z = 0.f;
	}
	if (Scale) {
		memcpy(&WScale, Scale, sizeof(MFLOAT)*3);
	} else {
		WScale[0] = WScale[1] = WScale[2] = 1.0f;
	}

	MinMax->Min.X = MinMax->Min.Y = MinMax->Min.Z =  FLT_MAX;
	MinMax->Max.X = MinMax->Max.Y = MinMax->Max.Z = -FLT_MAX;

	if (m_LayerList != NULL) {
		LayPtr = (MDxfLayer *)m_LayerList->Head();										// ﾚｲﾔ情報先頭
		while (LayPtr != NULL) {
			if (LayPtr->m_PrimList != NULL) {
				PrmPtr = (MDxfPrim *)LayPtr->m_PrimList->Head();						// 図形情報先頭
				while (PrmPtr != NULL) {
					PrmPtr->Transform(0, Symtry, WMatrix, &WMove, WScale, MinMax);		// 図形の変形
					PrmPtr = (MDxfPrim *)LayPtr->m_PrimList->Next();					// 図形情報次
				}
			}
			LayPtr = (MDxfLayer *)m_LayerList->Next();									// ﾚｲﾔ情報次
		}
	}

	if (m_BlockList != NULL) {
		BlkPtr = (MDxfBlock *)m_BlockList->Head();										// ﾌﾞﾛｯｸ情報先頭
		while (BlkPtr != NULL) {
			if (BlkPtr->m_PrimList != NULL) {
				PrmPtr = (MDxfPrim *)BlkPtr->m_PrimList->Head();						// 図形情報先頭
				while (PrmPtr != NULL) {
					PrmPtr->Transform(0, Symtry, WMatrix, &WMove, WScale, MinMax);		// 図形の変形
					PrmPtr = (MDxfPrim *)BlkPtr->m_PrimList->Next();					// 図形情報次
				}
			}
			BlkPtr = (MDxfBlock *)m_BlockList->Next();									// ﾌﾞﾛｯｸ情報次
		}
	}

	return;
}

/******************************************************************************************************************
[ MDxfInf::GetMinMaxF ]               機能  最小最大よりなる面を得る
******************************************************************************************************************/
HVOID MDxfInf::GetMinMaxF     (														// (  O) ｽﾃｰﾀｽ  0:正常  1:ｴﾗｰ
								MINT				VewTyp,								// (I  ) 平正背左右裏
								HMTRX3				Matrix,								// (I  ) 変換ﾏﾄﾘｸｽ
								MgPoint3*			Move,								// (I  ) 移動量
								MgPoint3*			Pnt1)								// (  O) 最小･最大の領域(4点)
{
	HMTRX3				MatrixW;														// 変換ﾏﾄﾘｸｽ
	MgPoint3				MoveW;															// 移動量
	MINT				ic;


	if (Matrix) {
		memcpy(MatrixW, Matrix, sizeof(HMTRX3));
	} else {
		MatrixW[0][0] = 1.f; MatrixW[0][1] = 0.f; MatrixW[0][2] = 0.f;
		MatrixW[1][0] = 0.f; MatrixW[1][1] = 1.f; MatrixW[1][2] = 0.f;
		MatrixW[2][0] = 0.f; MatrixW[2][1] = 0.f; MatrixW[2][2] = 1.f;
	}
	if (Move) {
		memcpy(&MoveW, Move, sizeof(MgPoint3));
	} else {
		MoveW.X = MoveW.Y = MoveW.Z = 0.f;
	}

	if (VewTyp == HVIEWT_HEI) {															// 平面    (1)
		Pnt1[0].X = m_MinMax.Min.X; Pnt1[0].Y = m_MinMax.Min.Y; Pnt1[0].Z = m_MinMax.Max.Z;
		Pnt1[1].X = m_MinMax.Max.X; Pnt1[1].Y = m_MinMax.Min.Y; Pnt1[1].Z = m_MinMax.Max.Z;
		Pnt1[2].X = m_MinMax.Max.X; Pnt1[2].Y = m_MinMax.Max.Y; Pnt1[2].Z = m_MinMax.Max.Z;
		Pnt1[3].X = m_MinMax.Min.X; Pnt1[3].Y = m_MinMax.Max.Y; Pnt1[3].Z = m_MinMax.Max.Z;
	} else if (VewTyp == HVIEWT_SYO) {													// 正面    (2)
		Pnt1[0].X = m_MinMax.Min.X; Pnt1[0].Y = m_MinMax.Min.Y; Pnt1[0].Z = m_MinMax.Min.Z;
		Pnt1[1].X = m_MinMax.Max.X; Pnt1[1].Y = m_MinMax.Min.Y; Pnt1[1].Z = m_MinMax.Min.Z;
		Pnt1[2].X = m_MinMax.Max.X; Pnt1[2].Y = m_MinMax.Min.Y; Pnt1[2].Z = m_MinMax.Max.Z;
		Pnt1[3].X = m_MinMax.Min.X; Pnt1[3].Y = m_MinMax.Min.Y; Pnt1[3].Z = m_MinMax.Max.Z;
	} else if (VewTyp == HVIEWT_HAI) {													// 背面    (3)
		Pnt1[0].X = m_MinMax.Max.X; Pnt1[0].Y = m_MinMax.Max.Y; Pnt1[0].Z = m_MinMax.Min.Z;
		Pnt1[1].X = m_MinMax.Min.X; Pnt1[1].Y = m_MinMax.Max.Y; Pnt1[1].Z = m_MinMax.Min.Z;
		Pnt1[2].X = m_MinMax.Min.X; Pnt1[2].Y = m_MinMax.Max.Y; Pnt1[2].Z = m_MinMax.Max.Z;
		Pnt1[3].X = m_MinMax.Max.X; Pnt1[3].Y = m_MinMax.Max.Y; Pnt1[3].Z = m_MinMax.Max.Z;
	} else if (VewTyp == HVIEWT_LSK) {													// 左側面  (4)
		Pnt1[0].X = m_MinMax.Min.X; Pnt1[0].Y = m_MinMax.Max.Y; Pnt1[0].Z = m_MinMax.Min.Z;
		Pnt1[1].X = m_MinMax.Min.X; Pnt1[1].Y = m_MinMax.Min.Y; Pnt1[1].Z = m_MinMax.Min.Z;
		Pnt1[2].X = m_MinMax.Min.X; Pnt1[2].Y = m_MinMax.Min.Y; Pnt1[2].Z = m_MinMax.Max.Z;
		Pnt1[3].X = m_MinMax.Min.X; Pnt1[3].Y = m_MinMax.Max.Y; Pnt1[3].Z = m_MinMax.Max.Z;
	} else if (VewTyp == HVIEWT_RSK) {													// 右側面  (5)
		Pnt1[0].X = m_MinMax.Max.X; Pnt1[0].Y = m_MinMax.Min.Y; Pnt1[0].Z = m_MinMax.Min.Z;
		Pnt1[1].X = m_MinMax.Max.X; Pnt1[1].Y = m_MinMax.Max.Y; Pnt1[1].Z = m_MinMax.Min.Z;
		Pnt1[2].X = m_MinMax.Max.X; Pnt1[2].Y = m_MinMax.Max.Y; Pnt1[2].Z = m_MinMax.Max.Z;
		Pnt1[3].X = m_MinMax.Max.X; Pnt1[3].Y = m_MinMax.Min.Y; Pnt1[3].Z = m_MinMax.Max.Z;
	} else if (VewTyp == HVIEWT_URA) {													// 裏面    (6)
		Pnt1[0].X = m_MinMax.Max.X; Pnt1[0].Y = m_MinMax.Min.Y; Pnt1[0].Z = m_MinMax.Min.Z;
		Pnt1[1].X = m_MinMax.Min.X; Pnt1[1].Y = m_MinMax.Min.Y; Pnt1[1].Z = m_MinMax.Min.Z;
		Pnt1[2].X = m_MinMax.Min.X; Pnt1[2].Y = m_MinMax.Max.Y; Pnt1[2].Z = m_MinMax.Min.Z;
		Pnt1[3].X = m_MinMax.Max.X; Pnt1[3].Y = m_MinMax.Max.Y; Pnt1[3].Z = m_MinMax.Min.Z;
	}

	for (ic = 0 ; ic < 4 ; ic++) {
		HgTrnPntByMtx(&Pnt1[ic], MatrixW, &MoveW, &Pnt1[ic]);
	}

	return;
}

/******************************************************************************************************************
[ MDxfInf::Show ]                     機能  ＤＸＦ情報を表示する
******************************************************************************************************************/
MINT MDxfInf::Show            (														// (  O) ｽﾃｰﾀｽ  0:正常  1:ｴﾗｰ
								MINT				MainId,								// (I  ) ﾒｲﾝID
								MINT				SubsId,								// (I  ) ｻﾌﾞID
								COLORREF			Color,								// (I  ) 表示色
								MINT				Symtry,								// (I  ) 対称ﾌﾗｸﾞ  0:なし  1:あり
								HMTRX3				Matrix,								// (I  ) 変換ﾏﾄﾘｸｽ
								MgPoint3*			Move,								// (I  ) 移動量
								MFLOAT				Scale,								// (I  ) ｽｹｰﾙ
								MgPoint2*			CutPnt,								// (I  ) ｶｯﾄ線 + 方向点
								MgMinMax3*			MinMax)								// (  O) 最小･最大
{
	MINT				Status;
	HMTRX3				WMatrix;														// 変換ﾏﾄﾘｸｽ
	MgPoint3				WMove;															// 移動量
	MgMinMax3			WMinMax;														// 最小･最大
	MDxfLayer*			LayPtr;															// ﾚｲﾔ情報
	MDxfBlock*			BlkPtr;
	MDxfPrim*			PrmPtr;
	CDC*				MemDC;															// (I  ) ﾒﾓﾘDC
	MgPoint2				OPnt[2];


	Status = 0;

	if (Matrix) {
		memcpy(&WMatrix, Matrix, sizeof(HMTRX3));
	} else {
		WMatrix[0][0] = 1.f; WMatrix[0][1] = 0.f; WMatrix[0][2] = 0.f;
		WMatrix[1][0] = 0.f; WMatrix[1][1] = 1.f; WMatrix[1][2] = 0.f;
		WMatrix[2][0] = 0.f; WMatrix[2][1] = 0.f; WMatrix[2][2] = 1.f;
	}
	if (Move) {
		memcpy(&WMove, Move, sizeof(MgPoint3));
	} else {
		WMove.X = WMove.Y = WMove.Z = 0.f;
	}

	WMinMax.Min.X = WMinMax.Min.Y = WMinMax.Min.Z =  FLT_MAX;
	WMinMax.Max.X = WMinMax.Max.Y = WMinMax.Max.Z = -FLT_MAX;

	Status = HsViewCntlGet(MainId, SubsId, NULL, &MemDC);								// ﾋﾞｭｰを検索
	if (Status == 0) {
		if (m_LayerList != NULL) {
			LayPtr = (MDxfLayer *)m_LayerList->Head();									// ﾚｲﾔ情報先頭
			while (LayPtr != NULL) {
				if (LayPtr->m_PrimList != NULL) {
					PrmPtr = (MDxfPrim *)LayPtr->m_PrimList->Head();					// 図形情報先頭
					while (PrmPtr != NULL) {
						PrmPtr->Show(MemDC, MainId, SubsId, Color, Symtry, WMatrix,
									 &WMove, Scale, CutPnt, &WMinMax);					// 表示
						PrmPtr = (MDxfPrim *)LayPtr->m_PrimList->Next();				// 図形情報次
					}
				}
				LayPtr = (MDxfLayer *)m_LayerList->Next();								// ﾚｲﾔ情報次
			}
		}

		if (m_BlockList != NULL) {
			BlkPtr = (MDxfBlock *)m_BlockList->Head();									// ﾌﾞﾛｯｸ情報先頭
			while (BlkPtr != NULL) {
				if (BlkPtr->m_PrimList != NULL) {
					PrmPtr = (MDxfPrim *)BlkPtr->m_PrimList->Head();					// 図形情報先頭
					while (PrmPtr != NULL) {
						PrmPtr->Show(MemDC, MainId, SubsId, Color, Symtry, WMatrix,
									 &WMove, Scale, CutPnt, &WMinMax);					// 表示
						PrmPtr = (MDxfPrim *)BlkPtr->m_PrimList->Next();				// 図形情報次
					}
				}
				BlkPtr = (MDxfBlock *)m_BlockList->Next();								// ﾌﾞﾛｯｸ情報次
			}
		}

		if (MinMax)  memcpy(MinMax, &WMinMax, sizeof(MgMinMax3));

		if (MainId == 9) {																// 補助は原点を表示
			OPnt[0].X = OPnt[0].Y = 0.f;
			HgTrnPntByMtx(&OPnt[0], WMatrix, &WMove, &OPnt[1]);
			HnMark(MemDC, MainId, SubsId, 2, 1, 0, 5, OCOLOR, 1, &OPnt[1]);				// Ｘを描く
		}
		if (CutPnt) {																	// ｶｯﾄ線あり
			HsCutShowLine(MemDC, MainId, SubsId, HLINE_SOLD, Color, CutPnt);			// ｶｯﾄ線を描く
		}
	}

	return(Status);
}

/******************************************************************************************************************
[ MDxfInf::MakeRL ]                   機能  立面情報の作成（線）
******************************************************************************************************************/
MINT MDxfInf::MakeRL          (														// (  O) ｽﾃｰﾀｽ  0:正常  1:ｴﾗｰ
								MINT				VewCod,								// (I  ) ﾋﾞｭｰｺｰﾄﾞ 0:東 1:西 2:南 3:北
								MINT				Kai,								// (I  ) 階
								HnDisp*				DispRL,								// (I  ) 立面表示情報（線分）
								COLORREF			Rgb,								// (I  ) 表示色
								MINT				Symtry,								// (I  ) 対称ﾌﾗｸﾞ  0:なし  1:あり
								HMTRX3				Matrix,								// (I  ) 変換ﾏﾄﾘｸｽ
								MgPoint3*			Move,								// (I  ) 移動量
								MFLOAT				Scale,								// (I  ) ｽｹｰﾙ
								MgPoint3*			TOPnt,								// (I  ) 2次元⇔3次元変換用原点
								MgPoint3*			TUvw,								// (I  ) 2次元⇔3次元変換用UVW
								MgMinMax3*			MinMax)								// (  O) 最小･最大
{
	MINT				Status;
	HMTRX3				WMatrix;														// 変換ﾏﾄﾘｸｽ
	MgPoint3				WMove;															// 移動量
	MgMinMax3			WMinMax;														// 最小･最大
	MDxfLayer*			LayPtr;															// ﾚｲﾔ情報
	MDxfBlock*			BlkPtr;
	MDxfPrim*			PrmPtr;


	Status = 0;

	if (Matrix) {
		memcpy(&WMatrix, Matrix, sizeof(HMTRX3));
	} else {
		WMatrix[0][0] = 1.f; WMatrix[0][1] = 0.f; WMatrix[0][2] = 0.f;
		WMatrix[1][0] = 0.f; WMatrix[1][1] = 1.f; WMatrix[1][2] = 0.f;
		WMatrix[2][0] = 0.f; WMatrix[2][1] = 0.f; WMatrix[2][2] = 1.f;
	}
	if (Move) {
		memcpy(&WMove, Move, sizeof(MgPoint3));
	} else {
		WMove.X = WMove.Y = WMove.Z = 0.f;
	}

	WMinMax.Min.X = WMinMax.Min.Y = WMinMax.Min.Z =  FLT_MAX;
	WMinMax.Max.X = WMinMax.Max.Y = WMinMax.Max.Z = -FLT_MAX;

	if (m_LayerList != NULL) {
		LayPtr = (MDxfLayer *)m_LayerList->Head();									// ﾚｲﾔ情報先頭
		while (LayPtr != NULL) {
			if (LayPtr->m_PrimList != NULL) {
				PrmPtr = (MDxfPrim *)LayPtr->m_PrimList->Head();					// 図形情報先頭
				while (PrmPtr != NULL) {
					PrmPtr->MakeRL(VewCod, Kai, DispRL, Rgb, Symtry, WMatrix, &WMove, Scale, TOPnt, TUvw, &WMinMax);	// 立面情報の作成(線)
					PrmPtr = (MDxfPrim *)LayPtr->m_PrimList->Next();				// 図形情報次
				}
			}
			LayPtr = (MDxfLayer *)m_LayerList->Next();								// ﾚｲﾔ情報次
		}
	}

	if (m_BlockList != NULL) {
		BlkPtr = (MDxfBlock *)m_BlockList->Head();									// ﾌﾞﾛｯｸ情報先頭
		while (BlkPtr != NULL) {
			if (BlkPtr->m_PrimList != NULL) {
				PrmPtr = (MDxfPrim *)BlkPtr->m_PrimList->Head();					// 図形情報先頭
				while (PrmPtr != NULL) {
					PrmPtr->MakeRL(VewCod, Kai, DispRL, Rgb, Symtry, WMatrix, &WMove, Scale, TOPnt, TUvw, &WMinMax);	// 立面情報の作成(線)
					PrmPtr = (MDxfPrim *)BlkPtr->m_PrimList->Next();				// 図形情報次
				}
			}
			BlkPtr = (MDxfBlock *)m_BlockList->Next();								// ﾌﾞﾛｯｸ情報次
		}
	}

	if (MinMax)  memcpy(MinMax, &WMinMax, sizeof(MgMinMax3));

	return(Status);
}

/******************************************************************************************************************
[ MDxfInf::NumDxfPrim ]               機能  プリミティブ数を得る
******************************************************************************************************************/
MINT MDxfInf::NumDxfPrim      (														// (  O) ﾌﾟﾘﾐﾃｨﾌﾞ数
								MINT				Symtry,								// (I  ) 対称ﾌﾗｸﾞ  0:なし  1:あり
								HMTRX3				Matrix,								// (I  ) 変換ﾏﾄﾘｸｽ
								MgPoint3*			Move,								// (I  ) 移動量
								MFLOAT				Scale,								// (I  ) ｽｹｰﾙ
								MgPoint2*			CutPnt)								// (I  ) ｶｯﾄ線 + 方向点
{
	MINT				Num;
	HMTRX3				WMatrix;														// 変換ﾏﾄﾘｸｽ
	MgPoint3				WMove;															// 移動量
	MDxfLayer*			LayPtr;															// ﾚｲﾔ情報
	MDxfBlock*			BlkPtr;
	MDxfPrim*			PrmPtr;


	Num = 0;

	if (Matrix) {
		memcpy(&WMatrix, Matrix, sizeof(HMTRX3));
	} else {
		WMatrix[0][0] = 1.f; WMatrix[0][1] = 0.f; WMatrix[0][2] = 0.f;
		WMatrix[1][0] = 0.f; WMatrix[1][1] = 1.f; WMatrix[1][2] = 0.f;
		WMatrix[2][0] = 0.f; WMatrix[2][1] = 0.f; WMatrix[2][2] = 1.f;
	}
	if (Move) {
		memcpy(&WMove, Move, sizeof(MgPoint3));
	} else {
		WMove.X = WMove.Y = WMove.Z = 0.f;
	}

	if (m_LayerList != NULL) {
		LayPtr = (MDxfLayer *)m_LayerList->Head();										// ﾚｲﾔ情報先頭
		while (LayPtr != NULL) {
			if (LayPtr->m_PrimList != NULL) {
				PrmPtr = (MDxfPrim *)LayPtr->m_PrimList->Head();						// 図形情報先頭
				while (PrmPtr != NULL) {
					Num += PrmPtr->NumDxfPrim(Symtry, WMatrix, &WMove, Scale, CutPnt);	// ﾌﾟﾘﾐﾃｨﾌﾞ数を得る
					PrmPtr = (MDxfPrim *)LayPtr->m_PrimList->Next();					// 図形情報次
				}
			}
			LayPtr = (MDxfLayer *)m_LayerList->Next();									// ﾚｲﾔ情報次
		}
	}

	if (m_BlockList != NULL) {
		BlkPtr = (MDxfBlock *)m_BlockList->Head();										// ﾌﾞﾛｯｸ情報先頭
		while (BlkPtr != NULL) {
			if (BlkPtr->m_PrimList != NULL) {
				PrmPtr = (MDxfPrim *)BlkPtr->m_PrimList->Head();						// 図形情報先頭
				while (PrmPtr != NULL) {
					Num += PrmPtr->NumDxfPrim(Symtry, WMatrix, &WMove, Scale, CutPnt);	// ﾌﾟﾘﾐﾃｨﾌﾞ数を得る
					PrmPtr = (MDxfPrim *)BlkPtr->m_PrimList->Next();					// 図形情報次
				}
			}
			BlkPtr = (MDxfBlock *)m_BlockList->Next();									// ﾌﾞﾛｯｸ情報次
		}
	}

	if (CutPnt) {																		// ｶｯﾄ線あり
		Num += HsCutNumDxfPrim(CutPnt);													// ｶｯﾄ線のﾌﾟﾘﾐﾃｨﾌﾞ数
	}

	return(Num);
}

/******************************************************************************************************************
[ MDxfInf::DxfR14Out ]                機能   ＤＸＦ（Ｒ１４）形式出力
******************************************************************************************************************/
MINT MDxfInf::DxfR14Out       (														// (  O) ｽﾃｰﾀｽ  0:正常 以外:ｴﾗｰ
								CStdioFile*			FLPtr,								// (I  ) ﾌｧｲﾙ識別子
								MINT*				NumHnd,								// (I/O) ﾊﾝﾄﾞﾙ数
								MCHAR*				LayNam,								// (I  ) ﾚｲﾔ名
								MINT				Symtry,								// (I  ) 対称ﾌﾗｸﾞ  0:なし  1:あり
								HMTRX3				Matrix,								// (I  ) 変換ﾏﾄﾘｸｽ
								MgPoint3*			Move,								// (I  ) 移動量
								MFLOAT				Scale,								// (I  ) ｽｹｰﾙ
								MgPoint2*			CutPnt)								// (I  ) ｶｯﾄ線 + 方向点
{
	MINT				Status;
	HMTRX3				WMatrix;														// 変換ﾏﾄﾘｸｽ
	MgPoint3				WMove;															// 移動量
	MDxfLayer*			LayPtr;															// ﾚｲﾔ情報
	MDxfBlock*			BlkPtr;
	MDxfPrim*			PrmPtr;


	Status = 0;

	if (Matrix) {
		memcpy(&WMatrix, Matrix, sizeof(HMTRX3));
	} else {
		WMatrix[0][0] = 1.f; WMatrix[0][1] = 0.f; WMatrix[0][2] = 0.f;
		WMatrix[1][0] = 0.f; WMatrix[1][1] = 1.f; WMatrix[1][2] = 0.f;
		WMatrix[2][0] = 0.f; WMatrix[2][1] = 0.f; WMatrix[2][2] = 1.f;
	}
	if (Move) {
		memcpy(&WMove, Move, sizeof(MgPoint3));
	} else {
		WMove.X = WMove.Y = WMove.Z = 0.f;
	}

	if (m_LayerList != NULL) {
		LayPtr = (MDxfLayer *)m_LayerList->Head();										// ﾚｲﾔ情報先頭
		while (LayPtr != NULL) {
			if (LayPtr->m_PrimList != NULL) {
				PrmPtr = (MDxfPrim *)LayPtr->m_PrimList->Head();						// 図形情報先頭
				while (PrmPtr != NULL) {
					Status = PrmPtr->DxfR14Out(FLPtr, NumHnd, LayNam, Symtry, WMatrix, &WMove, Scale, CutPnt);	// DXF(R14)形式出力
					PrmPtr = (MDxfPrim *)LayPtr->m_PrimList->Next();					// 図形情報次
				}
			}
			LayPtr = (MDxfLayer *)m_LayerList->Next();									// ﾚｲﾔ情報次
		}
	}

	if (m_BlockList != NULL) {
		BlkPtr = (MDxfBlock *)m_BlockList->Head();										// ﾌﾞﾛｯｸ情報先頭
		while (BlkPtr != NULL) {
			if (BlkPtr->m_PrimList != NULL) {
				PrmPtr = (MDxfPrim *)BlkPtr->m_PrimList->Head();						// 図形情報先頭
				while (PrmPtr != NULL) {
					Status = PrmPtr->DxfR14Out(FLPtr, NumHnd, LayNam, Symtry, WMatrix, &WMove, Scale, CutPnt);	// DXF(R14)形式出力
					PrmPtr = (MDxfPrim *)BlkPtr->m_PrimList->Next();					// 図形情報次
				}
			}
			BlkPtr = (MDxfBlock *)m_BlockList->Next();									// ﾌﾞﾛｯｸ情報次
		}
	}

	if (CutPnt) {																		// ｶｯﾄ線あり
		Status = HsCutDxfR14Out(FLPtr, NumHnd, LayNam, HLINE_SOLD, CutPnt);				// ｶｯﾄ線を描く
	}

	return(Status);
}

/******************************************************************************************************************
[ MDxfInf::GetLayBlkNum ]             機能  レイヤ・ブロック数を得る
******************************************************************************************************************/
MINT MDxfInf::GetLayBlkNum    ()														// (  O) ﾚｲﾔ･ﾌﾞﾛｯｸ数
{
	MINT				Num;


	Num = (MINT)m_LayerList->Number() + (MINT)m_BlockList->Number();

	return(Num);
}

/******************************************************************************************************************
[ MDxfInf::GetLayBlkInf ]             機能  レイヤ・ブロックの検索
******************************************************************************************************************/
MINT MDxfInf::GetLayBlkInf    (														// (  O) ｽﾃｰﾀｽ  0:正常  1:ｴﾗｰ
								MINT				Idx,								// (I  ) ｲﾝﾃﾞｯｸｽ
								MCHAR*				Name,								// (  O) ﾚｲﾔ･ﾌﾞﾛｯｸ名
								COLORREF*			Rgb)								// (  O) RGBｶﾗｰ値
{
	MINT				Status;
	MINT				No;
	MDxfLayer*			LayPtr;															// ﾚｲﾔ情報
	MDxfBlock*			BlkPtr;


	Status = 1;																			// なし
	No     = Idx;

	if (No < (MINT)m_LayerList->Number()) {
		LayPtr = (MDxfLayer *)m_LayerList->Access(No);									// ﾎﾟｲﾝﾀを得る
		BwsStrCpy(Name, LayPtr->m_Name);
		if (Rgb)  *Rgb    = LayPtr->m_Rgb;
		Status = 0;
	}
	No -= (MINT)m_LayerList->Number();

	if (Status == 1) {
		if (No < (MINT)m_BlockList->Number()) {
			BlkPtr = (MDxfBlock *)m_BlockList->Access(No);								// ﾎﾟｲﾝﾀを得る
			BwsStrCpy(Name, BlkPtr->m_Name);
			if (Rgb)  *Rgb = BlkPtr->m_Rgb;
			Status = 0;
		}
	}

	return(Status);
}

/******************************************************************************************************************
[ MDxfInf::SetLayBlkRgb ]             機能  レイヤ・ブロックにＲＧＢを設定する
******************************************************************************************************************/
MINT MDxfInf::SetLayBlkRgb    (														// (  O) ｽﾃｰﾀｽ  0:正常  1:ｴﾗｰ
								MCHAR*				Name,								// (I  ) ﾚｲﾔ･ﾌﾞﾛｯｸ名
								COLORREF			Rgb)								// (I  ) RGBｶﾗｰ値
{
	MINT				Status;
	MDxfLayer*			LayPtr;															// ﾚｲﾔ情報
	MDxfBlock*			BlkPtr;


	if (m_LayerList != NULL) {
		LayPtr = (MDxfLayer *)m_LayerList->Head();										// 先頭のﾎﾟｲﾝﾀを得る
		while (LayPtr != NULL) {
			if (BwsStrCmp(Name, LayPtr->m_Name) == 0) {									// ﾌﾞﾛｯｸ名一致
				LayPtr->m_Rgb = Rgb;
				Status = 0;
				break;
			}
			LayPtr = (MDxfLayer *)m_LayerList->Next();									// 次のﾎﾟｲﾝﾀを得る
		}
	}

	if (m_BlockList != NULL) {
		BlkPtr = (MDxfBlock *)m_BlockList->Head();										// 先頭のﾎﾟｲﾝﾀを得る
		while (BlkPtr != NULL) {
			if (BwsStrCmp(Name, BlkPtr->m_Name) == 0) {									// ﾌﾞﾛｯｸ名一致
				BlkPtr->m_Rgb = Rgb;
				Status = 0;
				break;
			}
			BlkPtr = (MDxfBlock *)m_BlockList->Next();									// 次のﾎﾟｲﾝﾀを得る
		}
	}

	return(Status);
}

/******************************************************************************************************************
[ MDxfInf::SetDirectX ]               機能  ＤＸＦ情報をＤｉｒｅｃｔＸに設定する
******************************************************************************************************************/
MINT MDxfInf::SetDirectX      (														// (  O) ｽﾃｰﾀｽ  0:正常  1:ｴﾗｰ
								MINT				DMode,								// (I  ) 0;線 1:面 2:両方
								LPDIRECT3DDEVICE8	LpD3DDEV,							// (I  ) DirectXﾃﾞﾊﾞｲｽ
								HstackTexture*		TextList,							// (I  ) ﾃｷｽﾁｬﾘｽﾄ
								HstackShowObj*		ShowList,							// (I  ) 表示対象ﾘｽﾄ
								MCHAR				TexNam[50][64],						// (I  ) ﾃｷｽﾁｬ名
								COLORREF*			RgbInf,								// (I  ) 表示色
								MINT				ArrFlg,								// (I  ) 矢印表示ﾌﾗｸﾞ  0:なし 1:あり
								MINT				Symtry,								// (I  ) 対称ﾌﾗｸﾞ  0:なし  1:あり
								HMTRX3				Matrix,								// (I  ) 変換ﾏﾄﾘｸｽ
								MgPoint3*			Move,								// (I  ) 移動量
								MgMinMax3*			MinMax)								// (  O) 最小･最大
{
	HMTRX3				WMatrix;														// 変換ﾏﾄﾘｸｽ
	MgPoint3				WMove;															// 移動量
	MINT				Status;
	MDxfLayer*			LayPtr;															// ﾚｲﾔ情報
	MDxfBlock*			BlkPtr;
	MDxfPrim*			PrmPtr;
	MgMinMax3			WMinMax;														// 最小･最大
	MCHAR				PasNam[256];
	MINT				No;
	MINT				SymW;
	MINT				ItrSts;


	Status = 0;

	if (Matrix) {
		memcpy(&WMatrix, Matrix, sizeof(HMTRX3));
	} else {
		WMatrix[0][0] = 1.f; WMatrix[0][1] = 0.f; WMatrix[0][2] = 0.f;
		WMatrix[1][0] = 0.f; WMatrix[1][1] = 1.f; WMatrix[1][2] = 0.f;
		WMatrix[2][0] = 0.f; WMatrix[2][1] = 0.f; WMatrix[2][2] = 1.f;
	}
	if (Move) {
		memcpy(&WMove, Move, sizeof(MgPoint3));
	} else {
		WMove.X = WMove.Y = WMove.Z = 0.f;
	}
	if (Matrix) {
		m_Symtry = m_SymtryC = SymW = Symtry;
	} else {
		SymW = 0;
	}

	WMinMax.Min.X = WMinMax.Min.Y = WMinMax.Min.Z =  FLT_MAX;							// 最小･最大
	WMinMax.Max.X = WMinMax.Max.Y = WMinMax.Max.Z = -FLT_MAX;

	if (m_LayerList != NULL) {
		No = 0;
		LayPtr = (MDxfLayer *)m_LayerList->Head();										// ﾚｲﾔ情報先頭
		while (LayPtr != NULL) {
			if (TexNam && BwsStrCmp(TexNam[No], BwsStr("")) != 0) {
				ItrSts = HeAddEnvNam(3, TexNam[No], PasNam);							// 環境変数を加えたﾊﾟｽを得る
			} else {
				if (m_VewTyp == HVIEWT_3D) {
					ItrSts = HeAddEnvNam(3, BwsStr("ＲＧＢ/250250250.bmp"), PasNam);	// 環境変数を加えたﾊﾟｽを得る
				} else {
					ItrSts = HeAddEnvNam(3, BwsStr("ＲＧＢ/070070070.bmp"), PasNam);	// 環境変数を加えたﾊﾟｽを得る
				}
			}
			if (TextList) {
				LayPtr->m_Texture = TextList->Add(PasNam);								// ﾃｷｽﾁｬ追加
				m_TextFlg = 0;															// ﾃｷｽﾁｬﾌﾗｸﾞ      0:管理      1:内臓
			} else {
				HsDirectSetTexture(PasNam, &LayPtr->m_Texture);							// ﾃｷｽﾁｬをｾｯﾄ
				m_TextFlg = 1;															// ﾃｷｽﾁｬﾌﾗｸﾞ      0:管理      1:内臓
			}
			if (RgbInf)  LayPtr->m_Rgb = RgbInf[No];

			if (LayPtr->m_PrimList != NULL) {
				PrmPtr = (MDxfPrim *)LayPtr->m_PrimList->Head();						// 図形情報先頭
				while (PrmPtr != NULL) {
					PrmPtr->Transform(1, SymW, WMatrix, &WMove, NULL, &WMinMax);		// 図形の変形
					if (DMode == 1 || DMode == 2)  PrmPtr->SetDirectXF(LpD3DDEV, LayPtr->m_Rgb, m_FAFlag);	// DirectXにｾｯﾄ
					if (DMode == 0 || DMode == 2)  PrmPtr->SetDirectXE(LpD3DDEV, m_Symtry);					// DirectXにｾｯﾄ
					PrmPtr = (MDxfPrim *)LayPtr->m_PrimList->Next();					// 図形情報次
				}
			}
			if (ShowList) {
				if (m_VewTyp == HVIEWT_3D) {
					ShowList->Add(LayPtr->m_Texture, 13, (HDBID)this);				// 表示追加
				} else {
					ShowList->Add(LayPtr->m_Texture, 14, (HDBID)this);				// 表示追加
				}
			}
			No++;
			LayPtr = (MDxfLayer *)m_LayerList->Next();									// ﾚｲﾔ情報次
		}
	}

	if (m_BlockList != NULL) {
		No = 0;
		BlkPtr = (MDxfBlock *)m_BlockList->Head();										// ﾌﾞﾛｯｸ情報先頭
		while (BlkPtr != NULL) {
			if (TexNam && BwsStrCmp(TexNam[No], BwsStr("")) != 0) {
				ItrSts = HeAddEnvNam(3, TexNam[No], PasNam);							// 環境変数を加えたﾊﾟｽを得る
			} else {
				if (m_VewTyp == HVIEWT_3D) {
					ItrSts = HeAddEnvNam(3, BwsStr("ＲＧＢ/250250250.bmp"), PasNam);	// 環境変数を加えたﾊﾟｽを得る
				} else {
					ItrSts = HeAddEnvNam(3, BwsStr("ＲＧＢ/070070070.bmp"), PasNam);	// 環境変数を加えたﾊﾟｽを得る
				}
			}
			if (TextList) {
				BlkPtr->m_Texture = TextList->Add(PasNam);								// ﾃｷｽﾁｬ追加
				m_TextFlg = 0;															// ﾃｷｽﾁｬﾌﾗｸﾞ      0:管理      1:内臓
			} else {
				HsDirectSetTexture(PasNam, &BlkPtr->m_Texture);							// ﾃｷｽﾁｬをｾｯﾄ
				m_TextFlg = 1;															// ﾃｷｽﾁｬﾌﾗｸﾞ      0:管理      1:内臓
			}
			if (RgbInf)  BlkPtr->m_Rgb = RgbInf[No];

			if (BlkPtr->m_PrimList != NULL) {
				PrmPtr = (MDxfPrim *)BlkPtr->m_PrimList->Head();						// 図形情報先頭
				while (PrmPtr != NULL) {
					PrmPtr->Transform(1, SymW, WMatrix, &WMove, NULL, &WMinMax);		// 図形の変形
					if (DMode == 1 || DMode == 2)  PrmPtr->SetDirectXF(LpD3DDEV, BlkPtr->m_Rgb, m_FAFlag);	// DirectXにｾｯﾄ
					if (DMode == 0 || DMode == 2)  PrmPtr->SetDirectXE(LpD3DDEV, m_Symtry);					// DirectXにｾｯﾄ
					PrmPtr = (MDxfPrim *)BlkPtr->m_PrimList->Next();					// 図形情報次
				}
			}
			if (ShowList) {
				if (m_VewTyp == HVIEWT_3D) {
					ShowList->Add(BlkPtr->m_Texture, 13, (HDBID)this);				// 表示追加
				} else {
					ShowList->Add(BlkPtr->m_Texture, 14, (HDBID)this);				// 表示追加
				}
			}
			No++;
			BlkPtr = (MDxfBlock *)m_BlockList->Next();									// ﾌﾞﾛｯｸ情報次
		}
	}

	if (ArrFlg)  ItrSts = SetDirectXY(LpD3DDEV);										// 矢印をDirectXに設定する

	if (MinMax)  memcpy(MinMax, &WMinMax, sizeof(MgMinMax3));

	return(Status);
}

/******************************************************************************************************************
[ MDxfInf::ShowDirectX ]              機能  ＤＸＦ情報をＤｉｒｅｃｔＸで表示する
******************************************************************************************************************/
HVOID MDxfInf::ShowDirectX    (														// (  O) ｽﾃｰﾀｽ  0:正常  1:ｴﾗｰ
								MINT					DMode,							// (I  ) 0;線 1:面 2:両方
								LPDIRECT3DDEVICE8		LpD3DDEV,						// (I  ) DirectXﾃﾞﾊﾞｲｽ
								LPDIRECT3DTEXTURE8		Texture)						// (I  ) ﾃｷｽﾁｬ
{
	MDxfLayer*			LayPtr;															// ﾚｲﾔ情報
	MDxfBlock*			BlkPtr;
	MDxfPrim*			PrmPtr;


	if (m_LayerList != NULL) {
		LayPtr = (MDxfLayer *)m_LayerList->Head();										// ﾚｲﾔ情報先頭
		while (LayPtr != NULL) {
			if (m_TextFlg == 1) {														// 内臓
				LpD3DDEV->SetTexture(0, NULL);											// ﾃｸｽﾁｬｸﾘｱ
				LpD3DDEV->SetTexture(0, LayPtr->m_Texture);								// ﾃｸｽﾁｬ設定
			} else {
				if (Texture && Texture != LayPtr->m_Texture) {
					LayPtr = (MDxfLayer *)m_LayerList->Next();							// ﾚｲﾔ情報次
					continue;
				}
			}
			if (LayPtr->m_PrimList != NULL) {
				PrmPtr = (MDxfPrim *)LayPtr->m_PrimList->Head();						// 図形情報先頭
				while (PrmPtr != NULL) {
					if (DMode == 1 || DMode == 2)  PrmPtr->ShowDirectXF(LpD3DDEV);		// DirectXで表示
					if (DMode == 0 || DMode == 2)  PrmPtr->ShowDirectXE(LpD3DDEV);		// DirectXで表示
					PrmPtr = (MDxfPrim *)LayPtr->m_PrimList->Next();					// 図形情報次
				}
			}
			LayPtr = (MDxfLayer *)m_LayerList->Next();									// ﾚｲﾔ情報次
		}
	}

	if (m_BlockList != NULL) {
		BlkPtr = (MDxfBlock *)m_BlockList->Head();										// ﾌﾞﾛｯｸ情報先頭
		while (BlkPtr != NULL) {
			if (m_TextFlg == 1) {														// 内臓
				LpD3DDEV->SetTexture(0, NULL);											// ﾃｸｽﾁｬｸﾘｱ
				LpD3DDEV->SetTexture(0, BlkPtr->m_Texture);								// ﾃｸｽﾁｬ設定
			} else {
				if (Texture && Texture != BlkPtr->m_Texture) {
					BlkPtr = (MDxfBlock *)m_BlockList->Next();							// ﾌﾞﾛｯｸ情報次
					continue;
				}
			}
			if (BlkPtr->m_PrimList != NULL) {
				PrmPtr = (MDxfPrim *)BlkPtr->m_PrimList->Head();						// 図形情報先頭
				while (PrmPtr != NULL) {
					if (DMode == 1 || DMode == 2)  PrmPtr->ShowDirectXF(LpD3DDEV);		// DirectXで表示
					if (DMode == 0 || DMode == 2)  PrmPtr->ShowDirectXE(LpD3DDEV);		// DirectXで表示
					PrmPtr = (MDxfPrim *)BlkPtr->m_PrimList->Next();					// 図形情報次
				}
			}
			BlkPtr = (MDxfBlock *)m_BlockList->Next();									// ﾌﾞﾛｯｸ情報次
		}
	}

	ShowDirectXY(LpD3DDEV);																// 矢印をDirectXで表示する

	return;
}

/******************************************************************************************************************
[ MDxfInf::RelDirectX ]               機能  ＤＸＦ情報をＤｉｒｅｃｔＸから開放する
******************************************************************************************************************/
MINT MDxfInf::RelDirectX      (														// (  O) ｽﾃｰﾀｽ  0:正常  1:ｴﾗｰ
								MINT					DMode)							// (I  ) 0;線 1:面 2:両方
{
	MINT				Status;
	MDxfLayer*			LayPtr;															// ﾚｲﾔ情報
	MDxfBlock*			BlkPtr;
	MDxfPrim*			PrmPtr;


	Status = 0;

	if (m_LayerList != NULL) {
		LayPtr = (MDxfLayer *)m_LayerList->Head();										// ﾚｲﾔ情報先頭
		while (LayPtr != NULL) {
			if (LayPtr->m_PrimList != NULL) {
				PrmPtr = (MDxfPrim *)LayPtr->m_PrimList->Head();						// 図形情報先頭
				while (PrmPtr != NULL) {
					if (DMode == 1 || DMode == 2)  PrmPtr->RelDirectXF();				// 開放
					if (DMode == 0 || DMode == 2)  PrmPtr->RelDirectXE();				// 開放
					PrmPtr = (MDxfPrim *)LayPtr->m_PrimList->Next();					// 図形情報次
				}
			}
			if (m_TextFlg == 1)  RELEASE(LayPtr->m_Texture);							// 内臓 ﾃｸｽﾁｬ解放
			LayPtr->m_Texture = NULL;
			LayPtr = (MDxfLayer *)m_LayerList->Next();									// ﾚｲﾔ情報次
		}
	}

	if (m_BlockList != NULL) {
		BlkPtr = (MDxfBlock *)m_BlockList->Head();										// ﾌﾞﾛｯｸ情報先頭
		while (BlkPtr != NULL) {
			if (BlkPtr->m_PrimList != NULL) {
				PrmPtr = (MDxfPrim *)BlkPtr->m_PrimList->Head();						// 図形情報先頭
				while (PrmPtr != NULL) {
					if (DMode == 1 || DMode == 2)  PrmPtr->RelDirectXF();				// 開放
					if (DMode == 0 || DMode == 2)  PrmPtr->RelDirectXE();				// 開放
					PrmPtr = (MDxfPrim *)BlkPtr->m_PrimList->Next();					// 図形情報次
				}
			}
			if (m_TextFlg == 1)  RELEASE(BlkPtr->m_Texture);							// 内臓 ﾃｸｽﾁｬ解放
			BlkPtr->m_Texture = NULL;
			BlkPtr = (MDxfBlock *)m_BlockList->Next();									// ﾌﾞﾛｯｸ情報次
		}
	}

	RelDirectXY();																		// 矢印をDirectXよりから開放する

	m_TextFlg = 0;

	return(Status);
}

/******************************************************************************************************************
[ MDxfInf::SetDirectXY ]              機能  矢印をＤｉｒｅｃｔＸに設定する
******************************************************************************************************************/
MINT MDxfInf::SetDirectXY (															// (  O) ｽﾃｰﾀｽ  0:正常  1:ｴﾗｰ
							LPDIRECT3DDEVICE8		LpD3DDEV)							// (I  ) DirectXﾃﾞﾊﾞｲｽ
{
	MINT				Status;
	HDXVERTEX*			Vertex  = NULL;
	HDXVERTEX*			VMemory = NULL;
	MCHAR				PasNam[256];
	MFLOAT				X1, X2, Y1,Y2, Z1, Z2;
	MFLOAT				XLeng, YLeng, ZLeng, MLeng;
	MgPoint3				Pnt[4];
	D3DCOLOR			Rgb;															// 色
	MINT				IPtr;
	MINT				ItrSts;


	Status = 1;

	if ((Vertex = (HDXVERTEX *)malloc(sizeof(HDXVERTEX) * 256)) == NULL)  HMFAIL;

	ItrSts = HeAddEnvNam(3, BwsStr("ＲＧＢ/250250250.bmp"), PasNam);					// 環境変数を加えたﾊﾟｽを得る
	HsDirectSetTexture(PasNam, &m_TextureY);											// ﾃｷｽﾁｬをｾｯﾄ

	LpD3DDEV->SetVertexShader(HDX_LVERTEX);												// 頂点ﾌｫｰﾏｯﾄ設定

	m_NumTriY = IPtr = 0;

	XLeng = m_MinMax.Max.X - m_MinMax.Min.X;
	YLeng = m_MinMax.Max.Y - m_MinMax.Min.Y;
	ZLeng = m_MinMax.Max.Z - m_MinMax.Min.Z;
	MLeng = HMAX(XLeng, YLeng);
	MLeng = HMAX(MLeng, ZLeng) / 100;

	// Ｘ方向
 	Rgb = D3DCOLOR_RGBA(255, 0, 0, 255);
	X1  = m_MinMax.Min.X - XLeng / 5; X2 = m_MinMax.Max.X + XLeng / 5;
	Y1  = -MLeng                    ; Y2 = +MLeng;
	Z1  = -MLeng                    ; Z2 = +MLeng;

	Pnt[0].X = X1  ; Pnt[0].Y = Y1  ; Pnt[0].Z = Z1;									// 底面
	Pnt[1].X = X1  ; Pnt[1].Y = Y1  ; Pnt[1].Z = Z2;
	Pnt[2].X = X1  ; Pnt[2].Y = Y2  ; Pnt[2].Z = Z2;
	Pnt[3].X = X1  ; Pnt[3].Y = Y2  ; Pnt[3].Z = Z1;
	ItrSts = SetVertex(4, Pnt, Rgb, &m_NumTriY, Vertex, &IPtr);							// Vertexへ情報を設定
	if (ItrSts != 0)  goto  PRG_EXIT;

	Pnt[0].X = X1  ; Pnt[0].Y = Y1  ; Pnt[0].Z = Z1;									// 前
	Pnt[1].X = X2  ; Pnt[1].Y = 0.f ; Pnt[1].Z = 0.f;
	Pnt[2].X = X1  ; Pnt[2].Y = Y1  ; Pnt[2].Z = Z2;
	ItrSts = SetVertex(3, Pnt, Rgb, &m_NumTriY, Vertex, &IPtr);							// Vertexへ情報を設定
	if (ItrSts != 0)  goto  PRG_EXIT;

	Pnt[0].X = X1  ; Pnt[0].Y = Y2  ; Pnt[0].Z = Z1;									// 奥
	Pnt[1].X = X1  ; Pnt[1].Y = Y2  ; Pnt[1].Z = Z2;
	Pnt[2].X = X2  ; Pnt[2].Y = 0.f ; Pnt[2].Z = 0.f;
	ItrSts = SetVertex(3, Pnt, Rgb, &m_NumTriY, Vertex, &IPtr);							// Vertexへ情報を設定
	if (ItrSts != 0)  goto  PRG_EXIT;

	Pnt[0].X = X1  ; Pnt[0].Y = Y1  ; Pnt[0].Z = Z1;									// 下
	Pnt[1].X = X1  ; Pnt[1].Y = Y2  ; Pnt[1].Z = Z1;
	Pnt[2].X = X2  ; Pnt[2].Y = 0.f ; Pnt[2].Z = 0.f;
	ItrSts = SetVertex(3, Pnt, Rgb, &m_NumTriY, Vertex, &IPtr);							// Vertexへ情報を設定
	if (ItrSts != 0)  goto  PRG_EXIT;

	Pnt[0].X = X1  ; Pnt[0].Y = Y1  ; Pnt[0].Z = Z2;									// 上
	Pnt[1].X = X2  ; Pnt[1].Y = 0.f ; Pnt[1].Z = 0.f;
	Pnt[2].X = X1  ; Pnt[2].Y = Y2  ; Pnt[2].Z = Z2;
	ItrSts = SetVertex(3, Pnt, Rgb, &m_NumTriY, Vertex, &IPtr);							// Vertexへ情報を設定
	if (ItrSts != 0)  goto  PRG_EXIT;

	// Ｙ方向
 	Rgb = D3DCOLOR_RGBA(0, 255, 0, 255);
	X1  = -MLeng                    ; X2 = +MLeng;
	Y1  = m_MinMax.Min.Y - YLeng / 5; Y2 = m_MinMax.Max.Y + YLeng / 5;
	Z1  = -MLeng                    ; Z2 = +MLeng;

	Pnt[0].X = X1  ; Pnt[0].Y = Y1  ; Pnt[0].Z = Z1;									// 底面
	Pnt[1].X = X2  ; Pnt[1].Y = Y1  ; Pnt[1].Z = Z1;
	Pnt[2].X = X2  ; Pnt[2].Y = Y1  ; Pnt[2].Z = Z2;
	Pnt[3].X = X1  ; Pnt[3].Y = Y1  ; Pnt[3].Z = Z2;
	ItrSts = SetVertex(4, Pnt, Rgb, &m_NumTriY, Vertex, &IPtr);							// Vertexへ情報を設定
	if (ItrSts != 0)  goto  PRG_EXIT;

	Pnt[0].X = X1  ; Pnt[0].Y = Y1  ; Pnt[0].Z = Z1;									// 前
	Pnt[1].X = X1  ; Pnt[1].Y = Y1  ; Pnt[1].Z = Z2;
	Pnt[2].X = 0.f ; Pnt[2].Y = Y2  ; Pnt[2].Z = 0.f;
	ItrSts = SetVertex(3, Pnt, Rgb, &m_NumTriY, Vertex, &IPtr);							// Vertexへ情報を設定
	if (ItrSts != 0)  goto  PRG_EXIT;

	Pnt[0].X = X2  ; Pnt[0].Y = Y1  ; Pnt[0].Z = Z1;									// 奥
	Pnt[1].X = 0.f ; Pnt[1].Y = Y2  ; Pnt[1].Z = 0.f;
	Pnt[2].X = X2  ; Pnt[2].Y = Y1  ; Pnt[2].Z = Z2;
	ItrSts = SetVertex(3, Pnt, Rgb, &m_NumTriY, Vertex, &IPtr);							// Vertexへ情報を設定
	if (ItrSts != 0)  goto  PRG_EXIT;

	Pnt[0].X = X1  ; Pnt[0].Y = Y1  ; Pnt[0].Z = Z1;									// 下
	Pnt[1].X = 0.f ; Pnt[1].Y = Y2  ; Pnt[1].Z = 0.f;
	Pnt[2].X = X2  ; Pnt[2].Y = Y1  ; Pnt[2].Z = Z1;
	ItrSts = SetVertex(3, Pnt, Rgb, &m_NumTriY, Vertex, &IPtr);							// Vertexへ情報を設定
	if (ItrSts != 0)  goto  PRG_EXIT;

	Pnt[0].X = X1  ; Pnt[0].Y = Y1  ; Pnt[0].Z = Z2;									// 上
	Pnt[1].X = X2  ; Pnt[1].Y = Y1  ; Pnt[1].Z = Z2;
	Pnt[2].X = 0.f ; Pnt[2].Y = Y2  ; Pnt[2].Z = 0.f;
	ItrSts = SetVertex(3, Pnt, Rgb, &m_NumTriY, Vertex, &IPtr);							// Vertexへ情報を設定
	if (ItrSts != 0)  goto  PRG_EXIT;

	// Ｚ方向
 	Rgb = D3DCOLOR_RGBA(0, 0, 255, 255);
	X1  = -MLeng                    ; X2 = +MLeng;
	Y1  = -MLeng                    ; Y2 = +MLeng;
	Z1  = m_MinMax.Min.Z - ZLeng / 5; Z2 = m_MinMax.Max.Z + ZLeng / 5;

	Pnt[0].X = X1  ; Pnt[0].Y = Y1  ; Pnt[0].Z = Z1;									// 底面
	Pnt[1].X = X1  ; Pnt[1].Y = Y2  ; Pnt[1].Z = Z1;
	Pnt[2].X = X2  ; Pnt[2].Y = Y2  ; Pnt[2].Z = Z1;
	Pnt[3].X = X2  ; Pnt[3].Y = Y1  ; Pnt[3].Z = Z1;
	ItrSts = SetVertex(4, Pnt, Rgb, &m_NumTriY, Vertex, &IPtr);							// Vertexへ情報を設定
	if (ItrSts != 0)  goto  PRG_EXIT;

	Pnt[0].X = X1  ; Pnt[0].Y = Y1  ; Pnt[0].Z = Z1;									// 前
	Pnt[1].X = X1  ; Pnt[1].Y = Y2  ; Pnt[1].Z = Z1;
	Pnt[2].X = 0.f ; Pnt[2].Y = 0.f ; Pnt[2].Z = Z2;
	ItrSts = SetVertex(3, Pnt, Rgb, &m_NumTriY, Vertex, &IPtr);							// Vertexへ情報を設定
	if (ItrSts != 0)  goto  PRG_EXIT;

	Pnt[0].X = X2  ; Pnt[0].Y = Y1  ; Pnt[0].Z = Z1;									// 奥
	Pnt[1].X = 0.f ; Pnt[1].Y = 0.f ; Pnt[1].Z = Z2;
	Pnt[2].X = X2  ; Pnt[2].Y = Y2  ; Pnt[2].Z = Z1;
	ItrSts = SetVertex(3, Pnt, Rgb, &m_NumTriY, Vertex, &IPtr);							// Vertexへ情報を設定
	if (ItrSts != 0)  goto  PRG_EXIT;

	Pnt[0].X = X1  ; Pnt[0].Y = Y1  ; Pnt[0].Z = Z1;									// 下
	Pnt[1].X = X2  ; Pnt[1].Y = Y1  ; Pnt[1].Z = Z1;
	Pnt[2].X = 0.f ; Pnt[2].Y = 0.f ; Pnt[2].Z = Z2;
	ItrSts = SetVertex(3, Pnt, Rgb, &m_NumTriY, Vertex, &IPtr);							// Vertexへ情報を設定
	if (ItrSts != 0)  goto  PRG_EXIT;

	Pnt[0].X = X1  ; Pnt[0].Y = Y2  ; Pnt[0].Z = Z1;									// 上
	Pnt[1].X = 0.f ; Pnt[1].Y = 0.f ; Pnt[1].Z = Z2;
	Pnt[2].X = X2  ; Pnt[2].Y = Y2  ; Pnt[2].Z = Z1;
	ItrSts = SetVertex(3, Pnt, Rgb, &m_NumTriY, Vertex, &IPtr);							// Vertexへ情報を設定
	if (ItrSts != 0)  goto  PRG_EXIT;

	LpD3DDEV->CreateVertexBuffer(sizeof(HDXVERTEX) * m_NumTriY * 3, 0, HDX_LVERTEX, D3DPOOL_MANAGED, &m_VertBufY);
	m_VertBufY->Lock(0, 0, (BYTE **)&VMemory, 0);
	memcpy(VMemory, Vertex, sizeof(HDXVERTEX) * m_NumTriY * 3);
	m_VertBufY->Unlock();

	Status = 0;

PRG_EXIT:

	HFREE(Vertex);

	return(Status);
}

/******************************************************************************************************************
[ MDxfInf::ShowDirectXY ]             機能  矢印をＤｉｒｅｃｔＸで表示する
******************************************************************************************************************/
HVOID MDxfInf::ShowDirectXY   (
								LPDIRECT3DDEVICE8		LpD3DDEV)						// (I  ) DirectXﾃﾞﾊﾞｲｽ
{
	if (m_TextureY) {
		LpD3DDEV->SetTexture(0, NULL);													// ﾃｸｽﾁｬｸﾘｱ
		LpD3DDEV->SetTexture(0, m_TextureY);											// ﾃｸｽﾁｬ設定
	}

	if (m_NumTriY > 0) {
		LpD3DDEV->SetVertexShader(HDX_LVERTEX);											// 頂点ﾌｫｰﾏｯﾄ設定

		LpD3DDEV->SetStreamSource(0, m_VertBufY, sizeof(HDXVERTEX));
		LpD3DDEV->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_NumTriY);						// ３角形の数
		LpD3DDEV->SetStreamSource(0, NULL, 0);
	}
}

/******************************************************************************************************************
[ MDxfInf::RelDirectXY ]              機能   矢印をＤｉｒｅｃｔＸから開放する
******************************************************************************************************************/
HVOID MDxfInf::RelDirectXY    ()
{
	if (m_TextureY)  RELEASE(m_TextureY);												// ﾃｷｽﾁｬ（矢印）
	m_TextureY = NULL;

	if (m_VertBufY)  RELEASE(m_VertBufY);												// DirectX 頂点ﾊﾞｯﾌｧ
	m_VertBufY = NULL;
	m_NumTriY  = 0;																		// ３角形の数
}

/******************************************************************************************************************
[ MDxfBlock::MDxfBlock ]             機能  コンストラクタ
******************************************************************************************************************/
MDxfBlock::MDxfBlock     ()
{
	BwsStrCpy(m_Name,  BwsStr(""));														// ﾌﾞﾛｯｸ名
	BwsStrCpy(m_LName, BwsStr(""));														// ﾚｲﾔ名
	m_Color    = 0;																		// 色番号
	m_Rgb      = RGB(250, 250, 250);
	m_Texture  = NULL;
	m_LineNo   = 0;																		// 線番号
	m_InsPnt.X = m_InsPnt.Y = m_InsPnt.Z = 0.0f;										// 挿入基点
	m_PrimList = new HlDList();										 					// ﾌﾟﾘﾐﾃｨﾌﾞ･双方向ﾎﾟｲﾝﾀ
}

/******************************************************************************************************************
[ MDxfBlock::MDxfBlock ]             機能  コンストラクタ
******************************************************************************************************************/
MDxfBlock::MDxfBlock     (
							MCHAR*			Name,										// (I  ) ﾌﾞﾛｯｸ名
							MCHAR*			LName,										// (I  ) ﾚｲﾔ名
							MINT			Color,										// (I  ) 色番号
							MINT			LineNo,										// (I  ) 線番号
							MgPoint3*		InsPnt)										// (I  ) 挿入基点
{
	BwsStrCpy(m_Name,   Name);															// ﾌﾞﾛｯｸ名
	BwsStrCpy(m_LName, LName);															// ﾚｲﾔ名
	m_Color    = Color;																	// 色番号
	m_Rgb      = MDxfGetCol(m_Color);
	m_Texture  = NULL;
	m_LineNo   = LineNo;																// 線番号
	memcpy(&m_InsPnt, InsPnt, sizeof(MgPoint3));											// 挿入基点
	m_PrimList = new HlDList();										 					// ﾌﾟﾘﾐﾃｨﾌﾞ･双方向ﾎﾟｲﾝﾀ
}

/******************************************************************************************************************
[ MDxfBlock::~MDxfBlock ]            機能  デストラクタ
******************************************************************************************************************/
MDxfBlock::~MDxfBlock    ()
{
	MDxfPrim*			Ptr;															// 図形情報へのﾎﾟｲﾝﾀ


	if (m_PrimList != NULL) {
		Ptr = (MDxfPrim *)m_PrimList->Head();											// 先頭のﾎﾟｲﾝﾀを得る
		while (Ptr != NULL) {
			delete  Ptr;
			Ptr = (MDxfPrim *)m_PrimList->Next();										// 次のﾎﾟｲﾝﾀを得る
		}
		delete m_PrimList;										 						// 図形･双方向ﾎﾟｲﾝﾀ
		m_PrimList = NULL;
	}
}

/******************************************************************************************************************
[ MDxfBlock::SetInf ]                 機能  ブロック情報をセット
******************************************************************************************************************/
HVOID MDxfBlock::SetInf   (
							MCHAR*			Name,										// (I  ) ﾌﾞﾛｯｸ名
							MCHAR*			LName,										// (I  ) ﾚｲﾔ名
							MINT			Color,										// (I  ) 色番号
							MINT			LineNo,										// (I  ) 線番号
							MgPoint3*		InsPnt)										// (I  ) 挿入基点
{
	BwsStrCpy(m_Name,   Name);															// ﾌﾞﾛｯｸ名
	BwsStrCpy(m_LName, LName);															// ﾚｲﾔ名
	m_Color    = Color;																	// 色番号
	m_Rgb      = MDxfGetCol(m_Color);
	m_Texture  = NULL;
	m_LineNo   = LineNo;																// 線番号
	memcpy(&m_InsPnt, InsPnt, sizeof(MgPoint3));											// 挿入基点
}

/******************************************************************************************************************
[ MDxfBlock::AddPrim ]                機能  ブロック情報に図形情報を追加
******************************************************************************************************************/
MDxfPrim* MDxfBlock::AddPrim (														// (  O) 図形情報のｲﾝｽﾀﾝｽ
								MINT				PrmType,							// (I  ) ﾌﾟﾘﾐﾃｨﾌﾞﾀｲﾌﾟ
								MINT				Color,								// (I  ) 色番号
								MINT				LineNo,								// (I  ) 線種番号（番号）
								MINT				LineWd,								// (I  ) 線幅番号（番号）
								HXPRIM*				Prim)								// (I  ) 図形
{
	MDxfPrim*			Ptr;															// 図形情報


	Ptr = new MDxfPrim(PrmType, Color, LineNo, LineWd, Prim);							// 図形情報ｺﾝｽﾄﾗｸﾀ
	m_PrimList->Inject((HVOID *)Ptr);													// 図形情報ﾘｽﾄへ追加

	return(Ptr);
}


/******************************************************************************************************************
[ MDxfLayer::MDxfLayer ]             機能  コンストラクタ
******************************************************************************************************************/
MDxfLayer::MDxfLayer         (
								MCHAR*				Name,								// (I  ) ﾚｲﾔ名
								MINT				Color,								// (I  ) 色番号
								MINT				LineNo)								// (I  ) 線番号
{
	BwsStrCpy(m_Name, Name);															// ﾚｲﾔ名
	m_Color    = Color;																	// 色番号
	m_Rgb = MDxfGetCol(m_Color);
	m_Texture  = NULL;
	m_LineNo   = LineNo;																// 線番号
	m_PrimList = new HlDList();										 					// ﾌﾟﾘﾐﾃｨﾌﾞ･双方向ﾎﾟｲﾝﾀ

}

/******************************************************************************************************************
[ MDxfLayer::~MDxfLayer ]            機能  デストラクタ
******************************************************************************************************************/
MDxfLayer::~MDxfLayer        ()
{
	MDxfPrim*			Ptr;															// 図形情報へのﾎﾟｲﾝﾀ


	if (m_PrimList != NULL) {
		Ptr = (MDxfPrim *)m_PrimList->Head();											// 先頭のﾎﾟｲﾝﾀを得る
		while (Ptr != NULL) {
			delete  Ptr;
			Ptr = (MDxfPrim *)m_PrimList->Next();										// 次のﾎﾟｲﾝﾀを得る
		}
		delete m_PrimList;										 						// 図形･双方向ﾎﾟｲﾝﾀ
		m_PrimList = NULL;
	}

}

/******************************************************************************************************************
[ MDxfLayer::Modify ]                 機能  更新
******************************************************************************************************************/
MDxfLayer::Modify             (
								MINT				Color,								// (I  ) 色番号
								MINT				LineNo)								// (I  ) 線番号
{
	m_Color  = Color;																	// 色番号
	m_Rgb = MDxfGetCol(m_Color);
	m_LineNo = LineNo;																	// 線番号

}

/******************************************************************************************************************
[ MDxfLayer::AddPrim ]                機能  レイヤ情報に図形情報を追加
******************************************************************************************************************/
MDxfPrim* MDxfLayer::AddPrim (														// (  O) 図形情報のｲﾝｽﾀﾝｽ
								MINT				PrmType,							// (I  ) ﾌﾟﾘﾐﾃｨﾌﾞﾀｲﾌﾟ
								MINT				Color,								// (I  ) 色番号
								MINT				LineNo,								// (I  ) 線種番号（番号）
								MINT				LineWd,								// (I  ) 線幅番号（番号）
								HXPRIM*				Prim)								// (I  ) 図形
{
	MDxfPrim*			Ptr;															// 図形情報


	Ptr = new MDxfPrim(PrmType, Color, LineNo, LineWd, Prim);							// 図形情報ｺﾝｽﾄﾗｸﾀ
	m_PrimList->Inject((HVOID *)Ptr);													// 図形情報ﾘｽﾄへ追加

	return(Ptr);
}

/******************************************************************************************************************
[ MDxfLines::MDxfLines ]             機能  線情報コンストラクタ
******************************************************************************************************************/
MDxfLines::MDxfLines     (
							MCHAR*			Name,										// (I  ) 線名
							MINT			LineNo)										// (I  ) 線番号
{
	BwsStrCpy(m_Name, Name);															// 線名
	m_LineNo = LineNo;																	// 線番号

}

/******************************************************************************************************************
[ MDxfLines::~MDxfLines ]            機能  線情報デストラクタ
******************************************************************************************************************/
MDxfLines::~MDxfLines    ()
{
}

/******************************************************************************************************************
[ MDxfFonts::MDxfFonts ]             機能  フォント情報コンストラクタ
******************************************************************************************************************/
MDxfFonts::MDxfFonts     (
							MCHAR*			Name,										// (I  ) ﾌｫﾝﾄ名
							MINT			FCode)										// (I  ) ﾌｫﾝﾄｺｰﾄﾞ
{
	BwsStrCpy(m_Name, Name);															// ﾌｫﾝﾄ名
	m_FCode = FCode;																	// ﾌｫﾝﾄｺｰﾄﾞ

}

/******************************************************************************************************************
[ MDxfFonts::~MDxfFonts ]            機能  フォント情報デストラクタ
******************************************************************************************************************/
MDxfFonts::~MDxfFonts    ()
{
}


/******************************************************************************************************************
[ MDxfPrim::MDxfPrim ]               機能  図形情報コンストラクタ
******************************************************************************************************************/
MDxfPrim::MDxfPrim           (
								MINT				PrmType,							// (I  ) ﾌﾟﾘﾐﾃｨﾌﾞﾀｲﾌﾟ
								MINT				Color,								// (I  ) 色番号
								MINT				LineNo,								// (I  ) 線種番号（番号）
								MINT				LineWd,								// (I  ) 線幅番号（番号）
								HXPRIM*				Prim)								// (I  ) 図形
{
	MINT*			VerNumPnt = NULL;													// ﾒｯｼｭ毎の頂点数列
	MINT*			PntNo     = NULL;													// 頂点番号列
	MgPoint3*		Pnt       = NULL;													// 頂点列
	MCHAR*			String    = NULL;													// 文字列

	m_PrmType = PrmType;																// ﾌﾟﾘﾐﾃｨﾌﾞﾀｲﾌﾟ
	m_Color   = Color;																	// 色番号
	m_LineNo  = LineNo;																	// 線種番号（番号）
	m_LineWd  = LineWd;																	// 線幅番号（番号）
	memcpy(&m_Prim, Prim, sizeof(HXPRIM));												// 図形
	m_NumTri  = 0;																		// ３角形の数
	m_VertBuf = NULL;																	// DirectX 頂点ﾊﾞｯﾌｧ
	m_NumLne  = 0;																		// 線の数
	m_LineBuf = NULL;																	// DirectX 線分ﾊﾞｯﾌｧ

	if (m_PrmType == HPRIM_MSH) {														// ﾀｲﾌﾟ:ﾒｯｼｭ
		if ((VerNumPnt = (MINT    *)malloc(sizeof(MINT)    * m_Prim.Mesh.Num))      == NULL)  HMFAIL;	// ﾒｯｼｭ毎の頂点数列
		if ((PntNo     = (MINT    *)malloc(sizeof(MINT)    * m_Prim.Mesh.NumPntNo)) == NULL)  HMFAIL;	// 頂点番号列
		if ((Pnt       = (MgPoint3 *)malloc(sizeof(MgPoint3) * m_Prim.Mesh.NumPnt))   == NULL)  HMFAIL;	// 頂点列

		memcpy(VerNumPnt, m_Prim.Mesh.VerNumPnt, sizeof(HUINT)   * m_Prim.Mesh.Num);
		memcpy(PntNo,     m_Prim.Mesh.PntNo,     sizeof(HUINT)   * m_Prim.Mesh.NumPntNo);
		memcpy(Pnt,       m_Prim.Mesh.Pnt,       sizeof(MgPoint3) * m_Prim.Mesh.NumPnt);

		m_Prim.Mesh.VerNumPnt = VerNumPnt;												// ﾒｯｼｭ毎の頂点数列
		m_Prim.Mesh.PntNo     = PntNo;													// 頂点番号列
		m_Prim.Mesh.Pnt       = Pnt;													// 頂点列
	} else if (m_PrmType == HPRIM_TXT) {												// ﾀｲﾌﾟ:文字
		if ((String = (MCHAR *)malloc(sizeof(MCHAR) * (m_Prim.Text.Num+1))) == NULL)  HMFAIL;
		BwsStrCpy(String, m_Prim.Text.String);
		m_Prim.Text.String = String;													// 文字列
	}
}

/******************************************************************************************************************
[ MDxfPrim::~MDxfPrim ]              機能  図形情報デストラクタ
******************************************************************************************************************/
MDxfPrim::~MDxfPrim      ()
{
	RELEASE(m_VertBuf);																	// DirectX 頂点ﾊﾞｯﾌｧ
	RELEASE(m_LineBuf);																	// DirectX 線分ﾊﾞｯﾌｧ

	if (m_PrmType == HPRIM_MSH) {														// ﾀｲﾌﾟ:ﾒｯｼｭ
		HFREE(m_Prim.Mesh.VerNumPnt);
		HFREE(m_Prim.Mesh.PntNo);
		HFREE(m_Prim.Mesh.Pnt);
	}
}

/******************************************************************************************************************
[ MDxfPrim::Transform ]               機能  図形情報の変形
******************************************************************************************************************/
HVOID MDxfPrim::Transform     (
								MINT				Color,								// (I  ) 色番号
								MgPoint3*			Base,								// (I  ) 基点
								MgPoint3*			Insert,								// (I  ) 挿入点
								MgPoint3*			Scale,								// (I  ) 尺度
								MFLOAT				Rotate)								// (I  ) 回転
{
	HMTRX3			Matrix;																// 変換ﾏﾄﾘｸｽ
	MgPoint3			Move;																// 移動量
	MFLOAT			Radian;																// 回転角度（radian）
	MFLOAT			MScale;																// 半径など平面上の均等な倍率
	MINT			ic;


	// 変換マトリクス
	Radian       = Rotate * HPI / 180.0f;
	Matrix[0][0] = (MFLOAT)(Scale->X * cos(Radian));
	Matrix[0][1] = (MFLOAT)(Scale->Y * sin(Radian) * (-1.0f));
	Matrix[0][2] = 0.0f;
	Matrix[1][0] = (MFLOAT)(Scale->X * sin(Radian));
	Matrix[1][1] = (MFLOAT)(Scale->Y * cos(Radian));
	Matrix[1][2] = 0.0f;
	Matrix[2][0] = 0.0f;
	Matrix[2][1] = 0.0f;
	Matrix[2][2] = 1.0f;

	// 移動量
	Move.X = (Insert->X) - (Base->X);
	Move.Y = (Insert->Y) - (Base->Y);
	Move.Z = (Insert->Z) - (Base->Z);

	// 均等倍率を設定
	MScale = HMAX(Scale->X, Scale->Y);
	MScale = HMAX(MScale,   Scale->Z);

	if (m_Color != 0)  m_Color = Color;

	switch  (m_PrmType) {
		case  HPRIM_DOT:																// 点
			HgTrnPntByMtx(&m_Prim.Dot, Matrix, &Move, &m_Prim.Dot);
			break;
		case  HPRIM_LIN:																// 線
			HgTrnPntByMtx(&m_Prim.Line.Pnt[0], Matrix, &Move, &m_Prim.Line.Pnt[0]);
			HgTrnPntByMtx(&m_Prim.Line.Pnt[1], Matrix, &Move, &m_Prim.Line.Pnt[1]);
			break;
		case  HPRIM_CIR:																// 円:
			HgTrnPntByMtx(&m_Prim.Circle.Pnt1, Matrix, &Move, &m_Prim.Circle.Pnt1);
			HgTrnPntByMtx(&m_Prim.Circle.Pnt2, Matrix, &Move, &m_Prim.Circle.Pnt2);
			break;
		case  HPRIM_ARC:																// 円弧
			HgTrnPntByMtx(&m_Prim.Arc.Pnt1, Matrix, &Move, &m_Prim.Arc.Pnt1);
			HgTrnPntByMtx(&m_Prim.Arc.Pnt2, Matrix, &Move, &m_Prim.Arc.Pnt2);
			HgTrnPntByMtx(&m_Prim.Arc.Pnt3, Matrix, &Move, &m_Prim.Arc.Pnt3);
			HgTrnPntByMtx(&m_Prim.Arc.Pnt4, Matrix, &Move, &m_Prim.Arc.Pnt4);
			break;
		case  HPRIM_TXT:																// 文字列
			HgTrnPntByMtx(&m_Prim.Text.Pnt, Matrix, &Move, &m_Prim.Text.Pnt);
			m_Prim.Text.Hei = m_Prim.Text.Hei * MScale;
			break;
		case  HPRIM_POL:																// 面
			for (ic = 0 ; ic < 4 ; ic++ )	{
				HgTrnPntByMtx(&m_Prim.Face.Pnt[ic], Matrix, &Move, &m_Prim.Face.Pnt[ic]);
			}
			break;
		case  HPRIM_MSH:																// ﾒｯｼｭ
			for (ic = 0 ; ic < m_Prim.Mesh.NumPnt ; ic++) {
				HgTrnPntByMtx(&m_Prim.Mesh.Pnt[ic], Matrix, &Move, &m_Prim.Mesh.Pnt[ic]);
			}
			break;
		default:
			break;
	}
}

/******************************************************************************************************************
[ MDxfPrim::Transform ]               機能  図形情報の変形
******************************************************************************************************************/
HVOID MDxfPrim::Transform     (
								MINT				TFlag,								// (I  ) 0:変換せず 1:変換
								MINT				Symtry,								// (I  ) 対称ﾌﾗｸﾞ  0:なし  1:あり
								HMTRX3				Matrix,								// (I  ) 変換ﾏﾄﾘｸｽ
								MgPoint3*			Move,								// (I  ) 移動量
								MFLOAT*				Scale,								// (I  ) ｽｹｰﾙ
								MgMinMax3*			MinMax)								// (  O) 最小･最大
{
	HMTRX3				WMatrix;														// 変換ﾏﾄﾘｸｽ
	MgPoint3				WMove;															// 移動量
	MFLOAT				WScale;
	MgPoint3*			PntW1 = NULL;
	MINT				ic;


	if ((PntW1 = (MgPoint3 *)malloc(sizeof(MgPoint3) * 2048)) == NULL)  HMFAIL;

	// 変換マトリクスを作成
	if (Matrix) {
		memcpy(&WMatrix, Matrix, sizeof(HMTRX3));
	} else {
		WMatrix[0][0] = 1.f; WMatrix[0][1] = 0.f; WMatrix[0][2] = 0.f;
		WMatrix[1][0] = 0.f; WMatrix[1][1] = 1.f; WMatrix[1][2] = 0.f;
		WMatrix[2][0] = 0.f; WMatrix[2][1] = 0.f; WMatrix[2][2] = 1.f;
	}

	if (Move) {
		memcpy(&WMove, Move, sizeof(MgPoint3));
	} else {
		WMove.X = WMove.Y = WMove.Z = 0.f;
	}

	WScale = 1.0f;
	if (Scale) {
		WMatrix[0][0] = WMatrix[0][0] * Scale[0];
		WMatrix[1][1] = WMatrix[1][1] * Scale[1];
		WMatrix[2][2] = WMatrix[2][2] * Scale[2];
		WScale        = Scale[0];
	}

	switch  (m_PrmType) {
		case  HPRIM_DOT:																// 点                (1)
			if (TFlag) {
				HgTrnPntByMtx(&m_Prim.Dot, WMatrix, &WMove, &m_Prim.Dot);
				SetMinMax(&m_Prim.Dot, MinMax);											// 最小･最大を更新
			} else {
				HgTrnPntByMtx(&m_Prim.Dot, WMatrix, &WMove, &PntW1[0]);
				SetMinMax(&PntW1[0], MinMax);											// 最小･最大を更新
			}
			break;
		case  HPRIM_LIN:																// 線                (2)
			if (TFlag) {
				HgTrnPntByMtx(&m_Prim.Line.Pnt[0], WMatrix, &WMove, &m_Prim.Line.Pnt[0]);
				SetMinMax(&m_Prim.Line.Pnt[0], MinMax);									// 最小･最大を更新
				HgTrnPntByMtx(&m_Prim.Line.Pnt[1], WMatrix, &WMove, &m_Prim.Line.Pnt[1]);
				SetMinMax(&m_Prim.Line.Pnt[1], MinMax);									// 最小･最大を更新
			} else {
				HgTrnPntByMtx(&m_Prim.Line.Pnt[0], WMatrix, &WMove, &PntW1[0]);
				SetMinMax(&PntW1[0], MinMax);											// 最小･最大を更新
				HgTrnPntByMtx(&m_Prim.Line.Pnt[1], WMatrix, &WMove, &PntW1[1]);
				SetMinMax(&PntW1[1], MinMax);											// 最小･最大を更新
			}
			break;
		case  HPRIM_CIR:																// 円                (4)
			if (TFlag) {
				HgTrnPntByMtx(&m_Prim.Circle.Pnt1, WMatrix, &WMove, &PntW1[0]);
				HgTrnPntByMtx(&m_Prim.Circle.Pnt2, WMatrix, &WMove, &PntW1[1]);
				m_Prim.Circle.Pnt1 = PntW1[0];
				m_Prim.Circle.Pnt2 = PntW1[1];

//				m_Prim.Circle.Pnt1.X = HMIN(PntW1[0].X, PntW1[1].X);
//				m_Prim.Circle.Pnt1.Y = HMIN(PntW1[0].Y, PntW1[1].Y);
//				m_Prim.Circle.Pnt1.Z = HMIN(PntW1[0].Z, PntW1[1].Z);
//				m_Prim.Circle.Pnt2.X = HMAX(PntW1[0].X, PntW1[1].X);
//				m_Prim.Circle.Pnt2.Y = HMAX(PntW1[0].Y, PntW1[1].Y);
//				m_Prim.Circle.Pnt2.Z = HMAX(PntW1[0].Z, PntW1[1].Z);
				SetMinMax(&m_Prim.Circle.Pnt1, MinMax);									// 最小･最大を更新
				SetMinMax(&m_Prim.Circle.Pnt2, MinMax);									// 最小･最大を更新
			} else {
				HgTrnPntByMtx(&m_Prim.Circle.Pnt1, WMatrix, &WMove, &PntW1[0]);
				SetMinMax(&PntW1[0], MinMax);											// 最小･最大を更新
				HgTrnPntByMtx(&m_Prim.Circle.Pnt2, WMatrix, &WMove, &PntW1[0]);
				SetMinMax(&PntW1[0], MinMax);											// 最小･最大を更新
			}
			break;
		case  HPRIM_ARC:																// 円弧              (5)
			if (TFlag) {
				HgTrnPntByMtx(&m_Prim.Arc.Pnt1, WMatrix, &WMove, &PntW1[0]);
				HgTrnPntByMtx(&m_Prim.Arc.Pnt2, WMatrix, &WMove, &PntW1[1]);
				HgTrnPntByMtx(&m_Prim.Arc.Pnt3, WMatrix, &WMove, &PntW1[2]);
				HgTrnPntByMtx(&m_Prim.Arc.Pnt4, WMatrix, &WMove, &PntW1[3]);
				m_Prim.Arc.Pnt1 = PntW1[0];
				m_Prim.Arc.Pnt2 = PntW1[1];
				m_Prim.Arc.Pnt3 = PntW1[2];
				m_Prim.Arc.Pnt4 = PntW1[3];

//				m_Prim.Arc.Pnt1.X = HMIN(PntW1[0].X, PntW1[1].X);
//				m_Prim.Arc.Pnt1.Y = HMIN(PntW1[0].Y, PntW1[1].Y);
//				m_Prim.Arc.Pnt1.Z = HMIN(PntW1[0].Z, PntW1[1].Z);
//				m_Prim.Arc.Pnt2.X = HMAX(PntW1[0].X, PntW1[1].X);
//				m_Prim.Arc.Pnt2.Y = HMAX(PntW1[0].Y, PntW1[1].Y);
//				m_Prim.Arc.Pnt2.Z = HMAX(PntW1[0].Z, PntW1[1].Z);
//				if (Symtry == 0) {														// 対称なし
//					m_Prim.Arc.Pnt3 = PntW1[2];
//					m_Prim.Arc.Pnt4 = PntW1[3];
//				} else {																// 対称あり
//					m_Prim.Arc.Pnt3 = PntW1[3];
//					m_Prim.Arc.Pnt4 = PntW1[2];
//				}
				SetMinMax(&m_Prim.Arc.Pnt3, MinMax);									// 最小･最大を更新
				SetMinMax(&m_Prim.Arc.Pnt4, MinMax);									// 最小･最大を更新
			} else {
				HgTrnPntByMtx(&m_Prim.Arc.Pnt1, WMatrix, &WMove, &PntW1[0]);
				HgTrnPntByMtx(&m_Prim.Arc.Pnt2, WMatrix, &WMove, &PntW1[0]);
				HgTrnPntByMtx(&m_Prim.Arc.Pnt3, WMatrix, &WMove, &PntW1[0]);
				SetMinMax(&PntW1[0], MinMax);											// 最小･最大を更新
				HgTrnPntByMtx(&m_Prim.Arc.Pnt4, WMatrix, &WMove, &PntW1[0]);
				SetMinMax(&PntW1[0], MinMax);											// 最小･最大を更新
			}
			break;
		case  HPRIM_TXT:																// 文字列            (6)
			if (TFlag) {
				HgTrnPntByMtx(&m_Prim.Text.Pnt, WMatrix, &WMove, &m_Prim.Text.Pnt);
				SetMinMax(&m_Prim.Text.Pnt, MinMax);									// 最小･最大を更新
				m_Prim.Text.Hei = m_Prim.Text.Hei * WScale;
			} else {
				HgTrnPntByMtx(&m_Prim.Text.Pnt, WMatrix, &WMove, &PntW1[0]);
				SetMinMax(&PntW1[0], MinMax);											// 最小･最大を更新
			}
			break;
		case  HPRIM_POL:																// 面
			if (TFlag) {
				for (ic = 0 ; ic < 4 ; ic++ ) {
					HgTrnPntByMtx(&m_Prim.Face.Pnt[ic], WMatrix, &WMove, &PntW1[ic]);
				}
				for (ic = 0 ; ic < 4 ; ic++ ) {
					if (Symtry == 0) {
						m_Prim.Face.Pnt[ic] = PntW1[ic];
					} else {
						m_Prim.Face.Pnt[ic] = PntW1[3-ic];
					}
					SetMinMax(&m_Prim.Face.Pnt[ic], MinMax);							// 最小･最大を更新
				}
			} else {
				for (ic = 0 ; ic < 4 ; ic++ ) {
					HgTrnPntByMtx(&m_Prim.Face.Pnt[ic], WMatrix, &WMove, &PntW1[0]);
					SetMinMax(&PntW1[0], MinMax);										// 最小･最大を更新
				}
			}
			break;
		case  HPRIM_MSH:																// ﾒｯｼｭ              (7)
			if (TFlag) {
				for (ic = 0 ; ic < m_Prim.Mesh.NumPnt ; ic++) {
					HgTrnPntByMtx(&m_Prim.Mesh.Pnt[ic], WMatrix, &WMove, &PntW1[ic]);
				}
				for (ic = 0 ; ic < m_Prim.Mesh.NumPnt ; ic++ ) {
					if (Symtry == 0) {
						m_Prim.Mesh.Pnt[ic] = PntW1[ic];
					} else {
						m_Prim.Mesh.Pnt[ic] = PntW1[m_Prim.Mesh.NumPnt-1-ic];
					}
					SetMinMax(&m_Prim.Mesh.Pnt[ic], MinMax);							// 最小･最大を更新
				}
			} else {
				for (ic = 0 ; ic < m_Prim.Mesh.NumPnt ; ic++) {
					HgTrnPntByMtx(&m_Prim.Mesh.Pnt[ic], WMatrix, &WMove, &PntW1[0]);
					SetMinMax(&PntW1[0], MinMax);										// 最小･最大を更新
				}
			}
			break;
		default:
			break;
	}

	HFREE(PntW1);

	return;
}

/******************************************************************************************************************
[ MDxfPrim::TransformYZ ]             機能  図形情報の変形（Ｙ→Ｚ）
******************************************************************************************************************/
HVOID MDxfPrim::TransformYZ   (
								MgMinMax3*			MinMax)								// (  O) 最小･最大
{
	MINT				ic;


	switch  (m_PrmType) {
		case  HPRIM_DOT:																// 点                (1)
			m_Prim.Dot.Z = m_Prim.Dot.Y; m_Prim.Dot.Y = 0.f;							//
			SetMinMax(&m_Prim.Dot, MinMax);												// 最小･最大を更新
			break;
		case  HPRIM_LIN:																// 線                (2)
			m_Prim.Line.Pnt[0].Z = m_Prim.Line.Pnt[0].Y; m_Prim.Line.Pnt[0].Y = 0.f;
			m_Prim.Line.Pnt[1].Z = m_Prim.Line.Pnt[1].Y; m_Prim.Line.Pnt[1].Y = 0.f;
			SetMinMax(&m_Prim.Line.Pnt[0], MinMax);										// 最小･最大を更新
			SetMinMax(&m_Prim.Line.Pnt[1], MinMax);										// 最小･最大を更新
			break;
		case  HPRIM_CIR:																// 円                (4)
			m_Prim.Circle.Pnt1.Z = m_Prim.Circle.Pnt1.Y; m_Prim.Circle.Pnt1.Y = 0.f;
			m_Prim.Circle.Pnt2.Z = m_Prim.Circle.Pnt2.Y; m_Prim.Circle.Pnt2.Y = 0.f;
			SetMinMax(&m_Prim.Circle.Pnt1, MinMax);										// 最小･最大を更新
			SetMinMax(&m_Prim.Circle.Pnt2, MinMax);										// 最小･最大を更新
			break;
		case  HPRIM_ARC:																// 円弧              (5)
			m_Prim.Arc.Pnt1.Z = m_Prim.Arc.Pnt1.Y; m_Prim.Arc.Pnt1.Y = 0.f;
			m_Prim.Arc.Pnt2.Z = m_Prim.Arc.Pnt2.Y; m_Prim.Arc.Pnt2.Y = 0.f;
			m_Prim.Arc.Pnt3.Z = m_Prim.Arc.Pnt3.Y; m_Prim.Arc.Pnt3.Y = 0.f;
			m_Prim.Arc.Pnt4.Z = m_Prim.Arc.Pnt4.Y; m_Prim.Arc.Pnt4.Y = 0.f;
			SetMinMax(&m_Prim.Arc.Pnt3, MinMax);										// 最小･最大を更新
			SetMinMax(&m_Prim.Arc.Pnt4, MinMax);										// 最小･最大を更新
			break;
		case  HPRIM_TXT:																// 文字列            (6)
			m_Prim.Text.Pnt.Z = m_Prim.Text.Pnt.Y; m_Prim.Text.Pnt.Y = 0.f;
			SetMinMax(&m_Prim.Text.Pnt, MinMax);										// 最小･最大を更新
			break;
		case  HPRIM_POL:																// 面
			for (ic = 0 ; ic < 4 ; ic++ ) {
				m_Prim.Face.Pnt[ic].Z = m_Prim.Face.Pnt[ic].Y; m_Prim.Face.Pnt[ic].Y = 0.f;
				SetMinMax(&m_Prim.Face.Pnt[ic], MinMax);								// 最小･最大を更新
			}
			break;
		case  HPRIM_MSH:																// ﾒｯｼｭ              (7)
			for (ic = 0 ; ic < m_Prim.Mesh.NumPnt ; ic++) {
				m_Prim.Mesh.Pnt[ic].Z = m_Prim.Mesh.Pnt[ic].Y; m_Prim.Mesh.Pnt[ic].Y = 0.f;
				SetMinMax(&m_Prim.Mesh.Pnt[ic], MinMax);								// 最小･最大を更新
			}
			break;
		default:
			break;
	}

	return;
}

/******************************************************************************************************************
[ MDxfPrim::Show ]                    機能  図形情報を表示する
******************************************************************************************************************/
HVOID MDxfPrim::Show          (
								CDC*				MemDC,								// (I  ) ﾒﾓﾘDC
								MINT				MainId,								// (I  ) ﾒｲﾝID
								MINT				SubsId,								// (I  ) ｻﾌﾞID
								COLORREF			Color,								// (I  ) 表示色
								MINT				Symtry,								// (I  ) 対称ﾌﾗｸﾞ  0:なし  1:あり
								HMTRX3				Matrix,								// (I  ) 変換ﾏﾄﾘｸｽ
								MgPoint3*			Move,								// (I  ) 移動量
								MFLOAT				Scale,								// (I  ) ｽｹｰﾙ
								MgPoint2*			CutPnt,								// (I  ) ｶｯﾄ線 + 方向点
								MgMinMax3*			MinMax)								// (  O) 最小･最大
{
	MgPoint3				Pnt1[4];
	MgPoint2				Pnt2[4];
	MgPoint2				Pnt3[4];
	MgPoint2				Pnt4[4];
	MgPoint2				Pnt5[4];
	MgPoint2				Pnt6[4];
	HCIR2				CirW1;
	MINT				Num;
	MINT				Idx, No;
	MINT				ItrSts;
	MINT				ic, jc, nc;


	switch  (m_PrmType) {
		case  HPRIM_DOT:																// 点
			HgTrnPntByMtx(&m_Prim.Dot, Matrix, Move, &Pnt1[0]);
			memcpy(&Pnt2[0], &Pnt1[0], sizeof(MgPoint2));
			if (CutPnt) {																// ｶｯﾄあり
				ItrSts = HgCutPntByCutL(CutPnt, &Pnt2[0]);
				if (ItrSts == -1)  goto  PRG_EXIT;										// すべてｶｯﾄ
			}
			SetMinMax(&Pnt1[0], MinMax);												// 最小･最大を更新
			break;
		case  HPRIM_LIN:																// 線
			HgTrnPntByMtx(&m_Prim.Line.Pnt[0], Matrix, Move, &Pnt1[0]);
			HgTrnPntByMtx(&m_Prim.Line.Pnt[1], Matrix, Move, &Pnt1[1]);
			memcpy(&Pnt2[0], &Pnt1[0], sizeof(MgPoint2));
			memcpy(&Pnt2[1], &Pnt1[1], sizeof(MgPoint2));
			memcpy(Pnt3, Pnt2, sizeof(MgPoint2) * 2);
			if (CutPnt) {																// ｶｯﾄあり
				ItrSts = HgCutLneByCutL(CutPnt, Pnt2, Pnt3);
				if (ItrSts == -1)   goto  PRG_EXIT;										// すべてｶｯﾄ
			}
			memcpy(&Pnt1[0], &Pnt3[0], sizeof(MgPoint2));
			memcpy(&Pnt1[1], &Pnt3[1], sizeof(MgPoint2));
			SetMinMax(&Pnt1[0], MinMax);												// 最小･最大を更新
			SetMinMax(&Pnt1[1], MinMax);												// 最小･最大を更新
			HnLine(MemDC, MainId, SubsId, m_LineNo, 0, 1, Color, 2, Pnt3);				// 線(線の太さは１固定)
			break;
		case  HPRIM_CIR:																// 円:
			HgTrnCirInf((MgPoint2 *)&m_Prim.Circle.Pnt1, (MgPoint2 *)&m_Prim.Circle.Pnt2, &CirW1);	// 外接する左下,右上より円構造体
			HgTrnPntByMtx(&CirW1.CPnt, Matrix, Move, &CirW1.CPnt);
			CirW1.Rad = CirW1.Rad * Scale;
			HgTrnCirInf(&CirW1, &Pnt2[0], &Pnt2[1]);									// 円構造体より外接する左下,右上
			ItrSts = 0;
			if (CutPnt) {																// ｶｯﾄあり
				ItrSts = HgCutCirByCutL(CutPnt, &Pnt2[0], &Pnt2[1], &Pnt2[2], &Pnt2[3]);
				if (ItrSts == -1)   goto  PRG_EXIT;										// すべてｶｯﾄ
			}
			if (ItrSts == 0) {															// ｶｯﾄなし
				SetMinMax(&Pnt1[0], MinMax);											// 最小･最大を更新
				SetMinMax(&Pnt1[1], MinMax);											// 最小･最大を更新
				HnCir(MemDC, MainId, SubsId, m_LineNo, 0, 1, Color, &Pnt2[0], &Pnt2[1]);// 円を描く(線の太さは１に固定)
			} else {																	// ｶｯﾄあり
				Pnt1[0].X = Pnt2[2].X; Pnt1[0].Y = Pnt2[2].Y;
				Pnt1[1].X = Pnt2[3].X; Pnt1[1].Y = Pnt2[2].Y;
				SetMinMax(&Pnt1[0], MinMax);											// 最小･最大を更新
				SetMinMax(&Pnt1[1], MinMax);											// 最小･最大を更新
				HnArc(MemDC, MainId, SubsId, m_LineNo, 0, 1, Color, &Pnt2[0], &Pnt2[1], &Pnt2[2], &Pnt2[3]);	// 円弧(線の太さは１固定)
			}
			break;
		case  HPRIM_ARC:																// 円弧
			HgTrnCirInf((MgPoint2 *)&m_Prim.Arc.Pnt1, (MgPoint2 *)&m_Prim.Arc.Pnt2, &CirW1);		// 外接する左下,右上より円構造体
			HgTrnPntByMtx(&CirW1.CPnt, Matrix, Move, &CirW1.CPnt);
			HgTrnPntByMtx(&m_Prim.Arc.Pnt3, Matrix, Move, &Pnt1[2]);
			HgTrnPntByMtx(&m_Prim.Arc.Pnt4, Matrix, Move, &Pnt1[3]);

			ItrSts = HgGetCPntByPnt(&CirW1.CPnt, CirW1.Rad, (MgPoint2 *)&Pnt1[2], (MgPoint2 *)&Pnt1[3], &CirW1.CPnt);	// 移動後の円の中点
			HgTrnCirInf(&CirW1, &Pnt2[0], &Pnt2[1]);									// 円構造体より外接する左下,右上
			if (Symtry == 0) {															// 対称なし
				memcpy(&Pnt2[2], &Pnt1[2], sizeof(MgPoint2));
				memcpy(&Pnt2[3], &Pnt1[3], sizeof(MgPoint2));
			} else {																	// 対称あり
				memcpy(&Pnt2[2], &Pnt1[3], sizeof(MgPoint2));
				memcpy(&Pnt2[3], &Pnt1[2], sizeof(MgPoint2));
			}
			ItrSts = 0;
			if (CutPnt) {																// ｶｯﾄあり
				ItrSts = HgCutArcByCutL(CutPnt, &Pnt2[0], &Pnt2[1], &Pnt2[2], &Pnt2[3], Pnt3, Pnt4, Pnt5, Pnt6);
				if (ItrSts == -1)   goto  PRG_EXIT;										// すべてｶｯﾄ
			}
			if (ItrSts == 0) {															// ｶｯﾄなし
				SetMinMax(&Pnt1[2], MinMax);											// 最小･最大を更新
				SetMinMax(&Pnt1[3], MinMax);											// 最小･最大を更新
				HnArc(MemDC, MainId, SubsId, m_LineNo, 0, 1, Color, &Pnt2[0], &Pnt2[1], &Pnt2[2], &Pnt2[3]);	// 円弧(線の太さは１固定)
			} else {																	// ｶｯﾄあり
				for (ic = 0 ; ic < ItrSts ; ic++) {
					Pnt1[0].X = Pnt5[ic].X; Pnt1[0].Y = Pnt5[ic].Y;
					Pnt1[1].X = Pnt6[ic].X; Pnt1[1].Y = Pnt6[ic].Y;
					SetMinMax(&Pnt1[0], MinMax);										// 最小･最大を更新
					SetMinMax(&Pnt1[1], MinMax);										// 最小･最大を更新
					HnArc(MemDC, MainId, SubsId, m_LineNo, 0, 1, Color, &Pnt3[ic], &Pnt4[ic], &Pnt5[ic], &Pnt6[ic]);	// 円弧
				}
			}
			break;
		case  HPRIM_TXT:																// 文字列
			break;
		case  HPRIM_POL:																// 面
			Num = m_Prim.Face.NumPnt;
			if (HgCmpPnt(&m_Prim.Face.Pnt[0], &m_Prim.Face.Pnt[m_Prim.Face.NumPnt-1], 1.0f) == 1) {	// 始点･終点同じ
				Num--;
			} else {
				if (m_Prim.Face.CFlag == 0)  Num--;
			}

			for (ic = 0 ; ic < Num ; ic++) {
				nc = HPTR(ic+1, m_Prim.Face.NumPnt);
				HgTrnPntByMtx(&m_Prim.Face.Pnt[ic], Matrix, Move, &Pnt1[0]);
				SetMinMax(&Pnt1[0], MinMax);											// 最小･最大を更新
				memcpy(&Pnt2[0], &Pnt1[0], sizeof(MgPoint2));
				HgTrnPntByMtx(&m_Prim.Face.Pnt[nc], Matrix, Move, &Pnt1[1]);
				SetMinMax(&Pnt1[1], MinMax);											// 最小･最大を更新
				memcpy(&Pnt2[1], &Pnt1[1], sizeof(MgPoint2));
				HnLine(MemDC, MainId, SubsId, m_LineNo, 0, 1, Color, 2, Pnt2);			// 線を描く(線の太さは１に固定)
			}
			break;
		case  HPRIM_MSH:																// ﾒｯｼｭ
			No = 0;
			for (ic = 0 ; ic < m_Prim.Mesh.Num ; ic++) {								// 面の数
				for (jc = 0 ; jc < m_Prim.Mesh.VerNumPnt[ic] ; jc++) {					// 頂点数
					nc  = HPTR(jc+1, m_Prim.Mesh.VerNumPnt[ic]);
					Idx = m_Prim.Mesh.PntNo[No+jc];
					HgTrnPntByMtx(&m_Prim.Mesh.Pnt[Idx], Matrix, Move, &Pnt1[0]);
					memcpy(&Pnt2[0], &Pnt1[0], sizeof(MgPoint2));
					Idx = m_Prim.Mesh.PntNo[No+nc];
					HgTrnPntByMtx(&m_Prim.Mesh.Pnt[Idx], Matrix, Move, &Pnt1[1]);
					memcpy(&Pnt2[1], &Pnt1[0], sizeof(MgPoint2));
					HnLine(MemDC, MainId, SubsId, m_LineNo, 0, 1, Color, 2, Pnt2);		// 線を描く(線の太さは１に固定)
				}
				No += m_Prim.Mesh.VerNumPnt[ic];
			}
			break;
		default:
			break;
	}

PRG_EXIT:

	return;
}

/******************************************************************************************************************
[ MDxfPrim::MakeRL ]                  機能  立面情報の作成（線）
******************************************************************************************************************/
HVOID MDxfPrim::MakeRL        (
								MINT				VewCod,								// (I  ) ﾋﾞｭｰｺｰﾄﾞ 0:東 1:西 2:南 3:北
								MINT				Kai,								// (I  ) 階
								HnDisp*				DispRL,								// (I  ) 立面表示情報（線分）
								COLORREF			Rgb,								// (I  ) 表示色
								MINT				Symtry,								// (I  ) 対称ﾌﾗｸﾞ  0:なし  1:あり
								HMTRX3				Matrix,								// (I  ) 変換ﾏﾄﾘｸｽ
								MgPoint3*			Move,								// (I  ) 移動量
								MFLOAT				Scale,								// (I  ) ｽｹｰﾙ
								MgPoint3*			TOPnt,								// (I  ) 2次元⇔3次元変換用原点
								MgPoint3*			TUvw,								// (I  ) 2次元⇔3次元変換用UVW
								MgMinMax3*			MinMax)								// (  O) 最小･最大
{
	MgPoint3				PntW1[4];
	MgPoint2				PntW2[4];
	MgPoint2				PntW3[4];
	MINT				Num;
	MINT				NumL;
	HLINE2*				LineW1   = NULL;
	MINT				Idx, No;
	MINT				ItrSts;
	MINT				ic, jc, kc, nc;


	if ((LineW1 = (HLINE2  *)malloc(sizeof(HLINE2) * 256)) == NULL)  HMFAIL;

	switch  (m_PrmType) {
		case  HPRIM_DOT:																// 点
			HgTrnPntByMtx(&m_Prim.Dot, Matrix, Move, &PntW1[0]);
			TrnWrdToLoc(TOPnt, TUvw, &PntW1[0], &PntW2[0]);								// ﾛｰｶﾙ座標系へ変換
			SetMinMax(&PntW2[0], MinMax);												// 最小･最大を更新
			break;
		case  HPRIM_LIN:																// 線
			HgTrnPntByMtx(&m_Prim.Line.Pnt[0], Matrix, Move, &PntW1[0]);
			HgTrnPntByMtx(&m_Prim.Line.Pnt[1], Matrix, Move, &PntW1[1]);
			NumL = HiddenLine(VewCod, Kai, TOPnt, TUvw, 2, PntW1, LineW1);				// 陰線消去
			for (ic = 0 ; ic < NumL ; ic++) {
				DispRL->AddLine(m_LineNo, 0, 1, Rgb, 2, (MgPoint2 *)&LineW1[ic]);		// 線を描く
				SetMinMax((MgPoint2 *)&LineW1[ic].Pnt[0], MinMax);						// 最小･最大を更新
				SetMinMax((MgPoint2 *)&LineW1[ic].Pnt[1], MinMax);						// 最小･最大を更新
			}
			break;
		case  HPRIM_CIR:																// 円:
			HgTrnPntByMtx(&m_Prim.Circle.Pnt1, Matrix, Move, &PntW1[0]);
			HgTrnPntByMtx(&m_Prim.Circle.Pnt2, Matrix, Move, &PntW1[1]);

			ItrSts = HiddenCheck(VewCod, Kai, TOPnt, TUvw, 2, PntW1);					// 陰線のﾁｪｯｸ
			if (ItrSts == 0)  break;

			TrnWrdToLoc(TOPnt, TUvw, &PntW1[0], &PntW2[0]);								// ﾛｰｶﾙ座標系へ変換
			TrnWrdToLoc(TOPnt, TUvw, &PntW1[1], &PntW2[1]);								// ﾛｰｶﾙ座標系へ変換
			PntW3[0].X = HMIN(PntW2[0].X, PntW2[1].X);
			PntW3[0].Y = HMIN(PntW2[0].Y, PntW2[1].Y);
			PntW3[1].X = HMAX(PntW2[0].X, PntW2[1].X);
			PntW3[1].Y = HMAX(PntW2[0].Y, PntW2[1].Y);
			SetMinMax(&PntW3[0], MinMax);												// 最小･最大を更新
			SetMinMax(&PntW3[1], MinMax);												// 最小･最大を更新
			DispRL->AddCir(m_LineNo, 0, 1, Rgb, &PntW3[0], &PntW3[1]);					// 円を描く
			break;
		case  HPRIM_ARC:																// 円弧
			HgTrnPntByMtx(&m_Prim.Arc.Pnt1, Matrix, Move, &PntW1[0]);
			HgTrnPntByMtx(&m_Prim.Arc.Pnt2, Matrix, Move, &PntW1[1]);
			HgTrnPntByMtx(&m_Prim.Arc.Pnt3, Matrix, Move, &PntW1[2]);
			HgTrnPntByMtx(&m_Prim.Arc.Pnt4, Matrix, Move, &PntW1[3]);

			ItrSts = HiddenCheck(VewCod, Kai, TOPnt, TUvw, 2, &PntW1[2]);				// 陰線のﾁｪｯｸ
			if (ItrSts == 0)  break;

			TrnWrdToLoc(TOPnt, TUvw, &PntW1[0], &PntW2[0]);								// ﾛｰｶﾙ座標系へ変換
			TrnWrdToLoc(TOPnt, TUvw, &PntW1[1], &PntW2[1]);								// ﾛｰｶﾙ座標系へ変換
			TrnWrdToLoc(TOPnt, TUvw, &PntW1[2], &PntW2[2]);								// ﾛｰｶﾙ座標系へ変換
			TrnWrdToLoc(TOPnt, TUvw, &PntW1[3], &PntW2[3]);								// ﾛｰｶﾙ座標系へ変換
			PntW3[0].X = HMIN(PntW2[0].X, PntW2[1].X);
			PntW3[0].Y = HMIN(PntW2[0].Y, PntW2[1].Y);
			PntW3[1].X = HMAX(PntW2[0].X, PntW2[1].X);
			PntW3[1].Y = HMAX(PntW2[0].Y, PntW2[1].Y);
			if ((Symtry == 0 && TUvw->X > 0.f) || (Symtry == 1 && TUvw->X < 0.f) ||
				(Symtry == 0 && TUvw->Y > 0.f) || (Symtry == 1 && TUvw->Y < 0.f)) {
				PntW3[2] = PntW2[2];
				PntW3[3] = PntW2[3];
			} else {																	// 対称
				PntW3[2] = PntW2[3];
				PntW3[3] = PntW2[2];
			}
			SetMinMax(&PntW3[2], MinMax);												// 最小･最大を更新
			SetMinMax(&PntW3[3], MinMax);												// 最小･最大を更新
			DispRL->AddArc(m_LineNo, 0, 1, Rgb, &PntW3[0], &PntW3[1], &PntW3[2], &PntW3[3]);	// 円弧を描く
			break;
		case  HPRIM_TXT:																// 文字列
			break;
		case  HPRIM_POL:																// 面
			Num = m_Prim.Face.NumPnt;
			if (HgCmpPnt(&m_Prim.Face.Pnt[0], &m_Prim.Face.Pnt[m_Prim.Face.NumPnt-1], 1.0f) == 1) {	// 始点･終点同じ
				Num--;
			} else {
				if (m_Prim.Face.CFlag == 0)  Num--;
			}

			for (ic = 0 ; ic < Num ; ic++) {
				nc = HPTR(ic+1, m_Prim.Face.NumPnt);

				HgTrnPntByMtx(&m_Prim.Face.Pnt[ic], Matrix, Move, &PntW1[0]);
				HgTrnPntByMtx(&m_Prim.Face.Pnt[nc], Matrix, Move, &PntW1[1]);
				NumL = HiddenLine(VewCod, Kai, TOPnt, TUvw, 2, PntW1, LineW1);			// 陰線消去
				for (jc = 0 ; jc < NumL ; jc++) {
					DispRL->AddLine(m_LineNo, 0, 1, Rgb, 2, (MgPoint2 *)&LineW1[jc]);	// 線を描く
					SetMinMax((MgPoint2 *)&LineW1[jc].Pnt[0], MinMax);					// 最小･最大を更新
					SetMinMax((MgPoint2 *)&LineW1[jc].Pnt[1], MinMax);					// 最小･最大を更新
				}
			}
			break;
		case  HPRIM_MSH:																// ﾒｯｼｭ
			No = 0;
			for (ic = 0 ; ic < m_Prim.Mesh.Num ; ic++) {								// 面の数
				for (jc = 0 ; jc < m_Prim.Mesh.VerNumPnt[ic] ; jc++) {					// 頂点数
					nc  = HPTR(jc+1, m_Prim.Mesh.VerNumPnt[ic]);
					Idx = m_Prim.Mesh.PntNo[No+jc];
					HgTrnPntByMtx(&m_Prim.Mesh.Pnt[Idx], Matrix, Move, &PntW1[0]);
					Idx = m_Prim.Mesh.PntNo[No+nc];
					HgTrnPntByMtx(&m_Prim.Mesh.Pnt[Idx], Matrix, Move, &PntW1[1]);
					NumL = HiddenLine(VewCod, Kai, TOPnt, TUvw, 2, PntW1, LineW1);		// 陰線消去
					for (kc = 0 ; kc < NumL ; kc++) {
						DispRL->AddLine(m_LineNo, 0, 1, Rgb, 2, (MgPoint2 *)&LineW1[kc]);// 線を描く
						SetMinMax((MgPoint2 *)&LineW1[kc].Pnt[0], MinMax);				// 最小･最大を更新
						SetMinMax((MgPoint2 *)&LineW1[kc].Pnt[1], MinMax);				// 最小･最大を更新
					}
				}
				No += m_Prim.Mesh.VerNumPnt[ic];
			}
			break;
		default:
			break;
	}

	HFREE(LineW1);

	return;
}

/******************************************************************************************************************
[ MDxfPrim::HiddenLine ]              機能  陰線消去
******************************************************************************************************************/
MINT MDxfPrim::HiddenLine     (														// (  O) 線分数
 								MINT				VewCod,								// (I  ) ﾋﾞｭｰｺｰﾄﾞ 0:東 1:西 2:南 3:北
								MINT				Kai,								// (I  ) 階
								MgPoint3*			TOPnt,								// (I  ) 2次元⇔3次元変換用原点
								MgPoint3*			TUvw,								// (I  ) 2次元⇔3次元変換用UVW
								MINT				NumPnt1,							// (I  ) 頂点数
								MgPoint3*			Pnt1,								// (I  ) 頂点
								HLINE2*				Line1)								// (  O) 線分
{
	MINT				NumL;															// 線分数
	MINT				NumC;
	MINT				NumW1;
	MgPoint2*			PntC     = NULL;
	MINT*				MFlagW   = NULL;
	MINT*				NumPntW1 = NULL;
	MgPoint2*			PntW1    = NULL;
	MgPoint2				PntW2;
	MgPoint3				PntW3;
	HLINE2				LineW1;
	MgMinMax2			MinMaxW1;
	MFLOAT				DepthW1;
	MINT				SFlag;
	MINT				ItrSts;
	MINT				ic, jc, nc;


	NumL = 0;																			// 線分数

	if ((PntC     = (MgPoint2 *)malloc(sizeof(MgPoint2) * 256)) == NULL)  HMFAIL;
	if ((MFlagW   = (MINT    *)malloc(sizeof(MINT)    *  64)) == NULL)  HMFAIL;
	if ((NumPntW1 = (MINT    *)malloc(sizeof(MINT)    *  64)) == NULL)  HMFAIL;
	if ((PntW1    = (MgPoint2 *)malloc(sizeof(MgPoint2) * 256)) == NULL)  HMFAIL;

	HgTrnWrdToLoc(TOPnt, TUvw, NumPnt1, Pnt1, PntW1);									// ﾛｰｶﾙ座標系へ変換
	ItrSts = HgGetMinMax(NumPnt1, PntW1, &MinMaxW1);									// 最小最大

	for (ic = 0 ; ic < NumPnt1 ; ic++) {
		nc = HPTR(ic+1, NumPnt1);
		LineW1.Pnt[0] = PntW1[ic]; LineW1.Pnt[1] = PntW1[nc];
		NumC  = HsDHonGetCrsPnt(VewCod, TOPnt, TUvw, Kai, NULL, &MinMaxW1, &LineW1, PntC);				// 交点数を得る
		NumW1 = HsRoofGetCrsPnt(VewCod, TOPnt, TUvw, Kai, NULL, -1, &MinMaxW1, &LineW1, &PntC[NumC]);	// 交点数を得る
		NumC += NumW1;
		NumW1 = HsDHaiGetCrsPnt(VewCod, TOPnt, TUvw, Kai, NULL, &MinMaxW1, &LineW1, &PntC[NumC]);		// 交点数を得る
		NumC += NumW1;

		HgPackCrsPnt(&LineW1, &NumC, PntC);												// 重複を除く
		for (jc = 0 ; jc < NumC - 1 ; jc++) {											// 交点数
			PntW2.X = (PntC[jc].X + PntC[jc+1].X) / 2;									// 中点
			PntW2.Y = (PntC[jc].Y + PntC[jc+1].Y) / 2;
			HgTrnLocToWrd(TOPnt, TUvw, 1, &PntW2, &PntW3);								// ﾜｰﾙﾄﾞ座標値に変換
			DepthW1 = HsGetDepth(VewCod, &PntW3, NumPnt1, Pnt1);						// 奥行きを求める
			if (VewCod == 0 || VewCod == 3) {											// 0:東 3:北
				DepthW1 += 10.f;
			} else {																	// 1:西 2:南
				DepthW1 -= 10.f;
			}
			SFlag = HsDHonChkVisible(VewCod, TOPnt, TUvw, Kai, NULL, &PntW2, &PntW3, DepthW1);				// 可視を判断
			if (SFlag == 1) {															// 見える
				SFlag = HsRoofChkVisible(VewCod, TOPnt, TUvw, Kai, NULL, -1, &PntW2, &PntW3, DepthW1);		// 可視を判断
				if (SFlag == 1) {
					SFlag = HsDHaiChkVisible(VewCod, TOPnt, TUvw, Kai, NULL, &PntW2, &PntW3, DepthW1);		// 可視を判断
					if (SFlag == 1) {
						Line1[NumL].Pnt[0] = PntC[jc]; Line1[NumL].Pnt[1] = PntC[jc+1]; NumL++;
					}
				}
			}
		}
	}

	HFREE(PntC);
	HFREE(MFlagW);
	HFREE(NumPntW1);
	HFREE(PntW1);

	return(NumL);
}
/******************************************************************************************************************
[ MDxfPrim::HiddenCheck ]             機能  陰線チェック（最小・最大）
******************************************************************************************************************/
MINT MDxfPrim::HiddenCheck    (														// (  O) 0:見えない 1:見える
 								MINT				VewCod,								// (I  ) ﾋﾞｭｰｺｰﾄﾞ 0:東 1:西 2:南 3:北
								MINT				Kai,								// (I  ) 階
								MgPoint3*			TOPnt,								// (I  ) 2次元⇔3次元変換用原点
								MgPoint3*			TUvw,								// (I  ) 2次元⇔3次元変換用UVW
								MINT				NumPnt1,							// (I  ) 頂点数
								MgPoint3*			Pnt1)								// (I  ) 頂点
{
	MINT				Status;
	MgPoint2*			PntW1 = NULL;
	MFLOAT				DepthW1;
	MINT				ic;


	Status = 1;																			// 見える

	if ((PntW1 = (MgPoint2 *)malloc(sizeof(MgPoint2) * NumPnt1)) == NULL)  HMFAIL;

	HgTrnWrdToLoc(TOPnt, TUvw, NumPnt1, Pnt1, PntW1);									// ﾛｰｶﾙ座標系へ変換
	for (ic = 0 ; ic < NumPnt1 ; ic++) {
		DepthW1 = HsGetDepth(VewCod, &Pnt1[ic], NumPnt1, Pnt1);							// 奥行きを求める
		if (VewCod == 0 || VewCod == 3) {												// 0:東 3:北
			DepthW1 += 10.f;
		} else {																		// 1:西 2:南
			DepthW1 -= 10.f;
		}
		Status = HsDHonChkVisible(VewCod, TOPnt, TUvw, Kai, NULL, &PntW1[ic], &Pnt1[ic], DepthW1);		// 可視を判断
		if (Status == 0)  break;														// 見えない
		Status = HsRoofChkVisible(VewCod, TOPnt, TUvw, Kai, NULL, -1, &PntW1[ic], &Pnt1[ic], DepthW1);	// 可視を判断
		if (Status == 0)  break;														// 見えない
		Status = HsDHaiChkVisible(VewCod, TOPnt, TUvw, Kai, NULL, &PntW1[ic], &Pnt1[ic], DepthW1);		// 可視を判断
		if (Status == 0)  break;														// 見えない
	}

	HFREE(PntW1);

	return(Status);
}

/******************************************************************************************************************
[ MDxfPrim::NumDxfPrim ]              機能  プリミティブ数を得る
******************************************************************************************************************/
MINT MDxfPrim::NumDxfPrim     (														// (  O) ﾌﾟﾘﾐﾃｨﾌﾞ数
								MINT				Symtry,								// (I  ) 対称ﾌﾗｸﾞ  0:なし  1:あり
								HMTRX3				Matrix,								// (I  ) 変換ﾏﾄﾘｸｽ
								MgPoint3*			Move,								// (I  ) 移動量
								MFLOAT				Scale,								// (I  ) ｽｹｰﾙ
								MgPoint2*			CutPnt)								// (I  ) ｶｯﾄ線 + 方向点
{
	MINT				Num;
	MgPoint3				Pnt1[4];
	MgPoint2				Pnt2[4];
	MgPoint2				Pnt3[4];
	MgPoint2				Pnt4[4];
	MgPoint2				Pnt5[4];
	MgPoint2				Pnt6[4];
	HCIR2				CirW1;
	MINT				ItrSts;
	MINT				ic, jc;


	Num = 0;
	switch  (m_PrmType) {
		case  HPRIM_DOT:																// 点
			HgTrnPntByMtx(&m_Prim.Dot, Matrix, Move, &Pnt1[0]);
			memcpy(&Pnt2[0], &Pnt1[0], sizeof(MgPoint2));
			if (CutPnt) {																// ｶｯﾄあり
				ItrSts = HgCutPntByCutL(CutPnt, &Pnt2[0]);
				if (ItrSts == -1)  goto  PRG_EXIT;
			}
			Num = 1;
			break;
		case  HPRIM_LIN:																// 線
			HgTrnPntByMtx(&m_Prim.Line.Pnt[0], Matrix, Move, &Pnt1[0]);
			HgTrnPntByMtx(&m_Prim.Line.Pnt[1], Matrix, Move, &Pnt1[1]);
			memcpy(&Pnt2[0], &Pnt1[0], sizeof(MgPoint2));
			memcpy(&Pnt2[1], &Pnt1[1], sizeof(MgPoint2));
			if (CutPnt) {																// ｶｯﾄあり
				ItrSts = HgCutLneByCutL(CutPnt, Pnt2, Pnt3);
				if (ItrSts == -1)   goto  PRG_EXIT;										// すべてｶｯﾄ
			}
			Num = 1;
			break;
		case  HPRIM_CIR:																// 円:
			HgTrnCirInf((MgPoint2 *)&m_Prim.Circle.Pnt1, (MgPoint2 *)&m_Prim.Circle.Pnt2, &CirW1);	// 外接する左下,右上より円構造体
			HgTrnPntByMtx(&CirW1.CPnt, Matrix, Move, &CirW1.CPnt);
			CirW1.Rad = CirW1.Rad * Scale;
			HgTrnCirInf(&CirW1, &Pnt2[0], &Pnt2[1]);									// 円構造体より外接する左下,右上
			if (CutPnt) {																// ｶｯﾄあり
				ItrSts = HgCutCirByCutL(CutPnt, &Pnt2[0], &Pnt2[1], &Pnt2[2], &Pnt2[3]);
				if (ItrSts == -1)   goto  PRG_EXIT;										// すべてｶｯﾄ
			}
			Num = 1;
			break;
		case  HPRIM_ARC:																// 円弧
			HgTrnCirInf((MgPoint2 *)&m_Prim.Arc.Pnt1, (MgPoint2 *)&m_Prim.Arc.Pnt2, &CirW1);	// 外接する左下,右上より円構造体
			HgTrnPntByMtx(&CirW1.CPnt, Matrix, Move, &CirW1.CPnt);
			CirW1.Rad = CirW1.Rad * Scale;
			HgTrnCirInf(&CirW1, &Pnt2[0], &Pnt2[1]);									// 円構造体より外接する左下,右上
			HgTrnPntByMtx(&m_Prim.Arc.Pnt3, Matrix, Move, &Pnt1[2]);
			HgTrnPntByMtx(&m_Prim.Arc.Pnt4, Matrix, Move, &Pnt1[3]);
			if (Symtry == 0) {															// 対称なし
				memcpy(&Pnt2[2], &Pnt1[2], sizeof(MgPoint2));
				memcpy(&Pnt2[3], &Pnt1[3], sizeof(MgPoint2));
			} else {																	// 対称あり
				memcpy(&Pnt2[2], &Pnt1[3], sizeof(MgPoint2));
				memcpy(&Pnt2[3], &Pnt1[2], sizeof(MgPoint2));
			}
			ItrSts = 0;
			if (CutPnt) {																// ｶｯﾄあり
				ItrSts = HgCutArcByCutL(CutPnt, &Pnt2[0], &Pnt2[1], &Pnt2[2], &Pnt2[3], Pnt3, Pnt4, Pnt5, Pnt6);
				if (ItrSts == -1)   goto  PRG_EXIT;										// すべてｶｯﾄ
			}
			if (ItrSts == 0) {															// ｶｯﾄなし
				Num = 1;
			} else {																	// ｶｯﾄあり
				Num = ItrSts;
			}
			break;
		case  HPRIM_TXT:																// 文字列
			Num = 1;
			break;
		case  HPRIM_POL:																// 面
			Num = m_Prim.Face.NumPnt;
			if (HgCmpPnt(&m_Prim.Face.Pnt[0], &m_Prim.Face.Pnt[m_Prim.Face.NumPnt-1], 1.0f) == 1) {	// 始点･終点同じ
				Num--;
			} else {
				if (m_Prim.Face.CFlag == 0)  Num--;
			}

			break;
		case  HPRIM_MSH:																// ﾒｯｼｭ
			for (ic = 0 ; ic < m_Prim.Mesh.Num ; ic++) {								// 面の数
				for (jc = 0 ; jc < m_Prim.Mesh.VerNumPnt[ic] ; jc++) {					// 頂点数
					Num++;
				}
			}
			break;
		default:
			break;
	}

PRG_EXIT:

	return(Num);
}

/******************************************************************************************************************
[ MDxfPrim::DxfR14Out ]               機能   ＤＸＦ（Ｒ１４）形式出力
******************************************************************************************************************/
MINT MDxfPrim::DxfR14Out      (														// (  O) ｽﾃｰﾀｽ  0:正常 以外:ｴﾗｰ
								CStdioFile*			FLPtr,								// (I  ) ﾌｧｲﾙ識別子
								MINT*				NumHnd,								// (I/O) ﾊﾝﾄﾞﾙ数
								MCHAR*				LayNam,								// (I  ) ﾚｲﾔ名
								MINT				Symtry,								// (I  ) 対称ﾌﾗｸﾞ  0:なし  1:あり
								HMTRX3				Matrix,								// (I  ) 変換ﾏﾄﾘｸｽ
								MgPoint3*			Move,								// (I  ) 移動量
								MFLOAT				Scale,								// (I  ) ｽｹｰﾙ
								MgPoint2*			CutPnt)
{
	MINT				Status;
	MgPoint3				Pnt1[4];
	MgPoint2				Pnt2[4];
	MgPoint2				Pnt3[4];
	MgPoint2				Pnt4[4];
	MgPoint2				Pnt5[4];
	MgPoint2				Pnt6[4];
	HCIR2				CirW1;
	MINT				Num;
	MINT				Idx, No;
	MINT				ItrSts;
	MINT				ic, jc, nc;


	Status = 0;

	switch  (m_PrmType) {
		case  HPRIM_DOT:																// 点
			HgTrnPntByMtx(&m_Prim.Dot, Matrix, Move, &Pnt1[0]);
			memcpy(&Pnt2[0], &Pnt1[0], sizeof(MgPoint2));
			if (CutPnt) {																// ｶｯﾄあり
				ItrSts = HgCutPntByCutL(CutPnt, &Pnt2[0]);
				if (ItrSts == -1)  goto  PRG_EXIT;										// すべてｶｯﾄ
			}
			Status = MDxfR14Dot(FLPtr, NumHnd, LayNam, 1, 1, &Pnt2[0]);
			break;
		case  HPRIM_LIN:																// 線
			HgTrnPntByMtx(&m_Prim.Line.Pnt[0], Matrix, Move, &Pnt1[0]);
			HgTrnPntByMtx(&m_Prim.Line.Pnt[1], Matrix, Move, &Pnt1[1]);
			memcpy(&Pnt2[0], &Pnt1[0], sizeof(MgPoint2));
			memcpy(&Pnt2[1], &Pnt1[1], sizeof(MgPoint2));
			memcpy(Pnt3, Pnt2, sizeof(MgPoint2) * 2);
			if (CutPnt) {																// ｶｯﾄあり
				ItrSts = HgCutLneByCutL(CutPnt, Pnt2, Pnt3);
				if (ItrSts == -1)   goto  PRG_EXIT;										// すべてｶｯﾄ
			}
			Status = MDxfR14Line(FLPtr, NumHnd, LayNam, m_LineNo, 1, 2, Pnt3);
			break;
		case  HPRIM_CIR:																// 円:
			HgTrnCirInf((MgPoint2 *)&m_Prim.Circle.Pnt1, (MgPoint2 *)&m_Prim.Circle.Pnt2, &CirW1);	// 外接する左下,右上より円構造体
			HgTrnPntByMtx(&CirW1.CPnt, Matrix, Move, &CirW1.CPnt);
			CirW1.Rad = CirW1.Rad * Scale;
			HgTrnCirInf(&CirW1, &Pnt2[0], &Pnt2[1]);									// 円構造体より外接する左下,右上
			ItrSts = 0;
			if (CutPnt) {																// ｶｯﾄあり
				ItrSts = HgCutCirByCutL(CutPnt, &Pnt2[0], &Pnt2[1], &Pnt2[2], &Pnt2[3]);
				if (ItrSts == -1)   goto  PRG_EXIT;										// すべてｶｯﾄ
			}
			if (ItrSts == 0) {															// ｶｯﾄなし
				Status = MDxfR14Cir(FLPtr, NumHnd, LayNam, m_LineNo, 1, &Pnt2[0], &Pnt2[1]);						// 円
			} else {																	// ｶｯﾄあり
				Status = MDxfR14Arc(FLPtr, NumHnd, LayNam, m_LineNo, 1, &Pnt2[0], &Pnt2[1], &Pnt2[2], &Pnt2[3]);	// 円弧
			}
			break;
		case  HPRIM_ARC:																// 円弧
			HgTrnCirInf((MgPoint2 *)&m_Prim.Arc.Pnt1, (MgPoint2 *)&m_Prim.Arc.Pnt2, &CirW1);	// 外接する左下,右上より円構造体
			HgTrnPntByMtx(&CirW1.CPnt, Matrix, Move, &CirW1.CPnt);
			CirW1.Rad = CirW1.Rad * Scale;
			HgTrnCirInf(&CirW1, &Pnt2[0], &Pnt2[1]);									// 円構造体より外接する左下,右上
			HgTrnPntByMtx(&m_Prim.Arc.Pnt3, Matrix, Move, &Pnt1[2]);
			HgTrnPntByMtx(&m_Prim.Arc.Pnt4, Matrix, Move, &Pnt1[3]);
			if (Symtry == 0) {															// 対称なし
				memcpy(&Pnt2[2], &Pnt1[2], sizeof(MgPoint2));
				memcpy(&Pnt2[3], &Pnt1[3], sizeof(MgPoint2));
			} else {																	// 対称あり
				memcpy(&Pnt2[2], &Pnt1[3], sizeof(MgPoint2));
				memcpy(&Pnt2[3], &Pnt1[2], sizeof(MgPoint2));
			}
			ItrSts = 0;
			if (CutPnt) {																// ｶｯﾄあり
				ItrSts = HgCutArcByCutL(CutPnt, &Pnt2[0], &Pnt2[1], &Pnt2[2], &Pnt2[3], Pnt3, Pnt4, Pnt5, Pnt6);
				if (ItrSts == -1)   goto  PRG_EXIT;										// すべてｶｯﾄ
			}
			if (ItrSts == 0) {															// ｶｯﾄなし
				Status = MDxfR14Arc(FLPtr, NumHnd, LayNam, m_LineNo, 1, &Pnt2[0], &Pnt2[1], &Pnt2[2], &Pnt2[3]);	// 円弧
			} else {																	// ｶｯﾄあり
				for (ic = 0 ; ic < ItrSts ; ic++) {
					Status = MDxfR14Arc(FLPtr, NumHnd, LayNam, m_LineNo, 1, &Pnt3[ic], &Pnt4[ic], &Pnt5[ic], &Pnt6[ic]);	// 円弧
				}
			}
			break;
		case  HPRIM_TXT:																// 文字列
			break;
		case  HPRIM_POL:																// 面
			Num = m_Prim.Face.NumPnt;
			if (HgCmpPnt(&m_Prim.Face.Pnt[0], &m_Prim.Face.Pnt[m_Prim.Face.NumPnt-1], 1.0f) == 1) {	// 始点･終点同じ
				Num--;
			} else {
				if (m_Prim.Face.CFlag == 0)  Num--;
			}

			for (ic = 0 ; ic < Num ; ic++) {
				nc = HPTR(ic+1, m_Prim.Face.NumPnt);
				HgTrnPntByMtx(&m_Prim.Face.Pnt[ic], Matrix, Move, &Pnt1[0]);
				memcpy(&Pnt2[0], &Pnt1[0], sizeof(MgPoint2));
				HgTrnPntByMtx(&m_Prim.Face.Pnt[nc], Matrix, Move, &Pnt1[1]);
				memcpy(&Pnt2[1], &Pnt1[1], sizeof(MgPoint2));
				Status = MDxfR14Line(FLPtr, NumHnd, LayNam, m_LineNo, 1, 2, Pnt2);
			}
			break;
		case  HPRIM_MSH:																// ﾒｯｼｭ
			No = 0;
			for (ic = 0 ; ic < m_Prim.Mesh.Num ; ic++) {								// 面の数
				for (jc = 0 ; jc < m_Prim.Mesh.VerNumPnt[ic] ; jc++) {					// 頂点数
					nc  = HPTR(jc+1, m_Prim.Mesh.VerNumPnt[ic]);
					Idx = m_Prim.Mesh.PntNo[No+jc];
					HgTrnPntByMtx(&m_Prim.Mesh.Pnt[Idx], Matrix, Move, &Pnt1[0]);
					memcpy(&Pnt2[0], &Pnt1[0], sizeof(MgPoint2));
					Idx = m_Prim.Mesh.PntNo[No+nc];
					HgTrnPntByMtx(&m_Prim.Mesh.Pnt[Idx], Matrix, Move, &Pnt1[1]);
					memcpy(&Pnt2[1], &Pnt1[0], sizeof(MgPoint2));
					Status = MDxfR14Line(FLPtr, NumHnd, LayNam, m_LineNo, 1, 2, Pnt2);
				}
				No += m_Prim.Mesh.VerNumPnt[ic];
			}
			break;
		default:
			break;
	}

PRG_EXIT:

	return(Status);
}

/******************************************************************************************************************
[ MDxfPrim::SetDirectXF ]              機能  面をＤｉｒｅｃｔＸに設定する
******************************************************************************************************************/
HVOID MDxfPrim::SetDirectXF   (
								LPDIRECT3DDEVICE8	LpD3DDEV,							// (I  ) DirectXﾃﾞﾊﾞｲｽ
								COLORREF			Rgb1,								// (I  ) RGBｶﾗｰ値
								MINT				FAFlag)								// (I  ) 面の向き調整   0:しない  1:する
{
	HDXVERTEX*			Vertex  = NULL;
	HDXVERTEX*			VMemory = NULL;
	MgPoint3				PntW1[64];
	MgPoint3				PntW2[64];
	MgPoint2				PntT[64];
	D3DCOLOR			Rgb;															// 色
	MINT				Red, Green, Blue;
	MgPoint3				Vect;															// 法線方向
	MINT				Num;
	MINT				NumTri;
	MINT				Ip1, Ip2, Idx;
	MINT				ic, jc, kc;


	if ((Vertex = (HDXVERTEX *)malloc(sizeof(HDXVERTEX) * 2048 * 3)) == NULL)  HMFAIL;

	LpD3DDEV->SetVertexShader(HDX_LVERTEX);												// 頂点ﾌｫｰﾏｯﾄ設定

	Red   = REDCOL(Rgb1); Green = GREENCOL(Rgb1); Blue  = BLUECOL(Rgb1);
	Rgb   = D3DCOLOR_RGBA(Red, Green, Blue, 255);
	/*
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	#ifdef _DEBUG
	MCHAR			StrBuf[256];
	BwsSprintf(StrBuf, BwsStr("ＤＸＦ タイプ   [%d] \n"), m_PrmType);
	TRACE(StrBuf);
	#endif
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	*/
	switch  (m_PrmType) {
		case  HPRIM_DOT:																// 点
			break;
		case  HPRIM_LIN:																// 線
			/*
			/////////////////////////////////////////////////////////////////////////////////////////////////////////
			#ifdef _DEBUG
			BwsSprintf(StrBuf, BwsStr("ＬＩＮＥ        [%d] \n"), 2);
			TRACE(StrBuf);
			for (ic = 0 ; ic < 2 ; ic++) {
				BwsSprintf(StrBuf, BwsStr("%8.2f %8.2f %8.2f \n"), m_Prim.Line.Pnt[ic].X, m_Prim.Line.Pnt[ic].Y, m_Prim.Line.Pnt[ic].Z); 
				TRACE(StrBuf);
			}
			#endif
			/////////////////////////////////////////////////////////////////////////////////////////////////////////
			*/
			break;
		case  HPRIM_CIR:																// 円:
			break;
		case  HPRIM_ARC:																// 円弧
			break;
		case  HPRIM_TXT:																// 文字列
			break;

		case  HPRIM_POL:																// 面
			/*
			/////////////////////////////////////////////////////////////////////////////////////////////////////////
			#ifdef _DEBUG
			BwsSprintf(StrBuf, BwsStr("３ＤＦＡＣＥ    [%d] \n"), m_Prim.Face.NumPnt);
			TRACE(StrBuf);
			for (ic = 0 ; ic < m_Prim.Face.NumPnt ; ic++) {
				BwsSprintf(StrBuf, BwsStr("%8.2f %8.2f %8.2f \n"), m_Prim.Face.Pnt[ic].X, m_Prim.Face.Pnt[ic].Y, m_Prim.Face.Pnt[ic].Z); 
				TRACE(StrBuf);
			}
			#endif
			/////////////////////////////////////////////////////////////////////////////////////////////////////////
			*/
/*
			HgGetVectByPln(m_Prim.Face.Pnt, &Vect);										// 法線ﾍﾞｸﾄﾙ
			m_NumTri = HgDivAraToTri(m_Prim.Face.NumPnt, m_Prim.Face.Pnt, PntW1);		// ３角形分割
			HsDirectSetTVect(m_Prim.Face.Pnt, m_NumTri * 3, PntW1, 2000.f, 500.f, PntW2);	// ﾃｷｽﾁｬ座標値を得る
			for (ic = 0 ; ic < m_NumTri * 3 ; ic++) { 
				HsDirectSetLVertex(&PntW1[ic], &Vect, Rgb, PntW2[ic].X, PntW2[ic].Y, &Vertex[ic]);
			}

			LpD3DDEV->CreateVertexBuffer(sizeof(HDXVERTEX) * m_NumTri * 3, 0, HDX_LVERTEX, D3DPOOL_MANAGED, &m_VertBuf);
			m_VertBuf->Lock(0, 0, (BYTE **)&VMemory, 0);
			memcpy(VMemory, Vertex, sizeof(HDXVERTEX) * m_NumTri * 3);
			m_VertBuf->Unlock();
*/
			Num = m_Prim.Face.NumPnt;
			if (HgCmpPnt(&m_Prim.Face.Pnt[0], &m_Prim.Face.Pnt[m_Prim.Face.NumPnt-1], 1.0f) == 1) {	// 始点･終点同じ
				Num--;
			}
			m_NumTri = Num - 2;
			if (FAFlag == 1) {															// 面の向き調整
				HgGetVectByPln(m_Prim.Face.Pnt, &Vect);									// 法線ﾍﾞｸﾄﾙ
				if (Vect.X < -0.5 || Vect.Y > 0.5 || Vect.Z < -0.5) {
					for (ic = 0 ; ic < m_Prim.Face.NumPnt ; ic++) {						// 周り順逆転
						PntW1[m_Prim.Face.NumPnt-ic-1] = m_Prim.Face.Pnt[ic];
					}
				} else {
					for (ic = 0 ; ic < m_Prim.Face.NumPnt ; ic++) {
						PntW1[ic] = m_Prim.Face.Pnt[ic];
					}
				}
			} else {																	// 調整しない
				for (ic = 0 ; ic < m_Prim.Face.NumPnt ; ic++) {							// 周り順逆転
					PntW1[m_Prim.Face.NumPnt-ic-1] = m_Prim.Face.Pnt[ic];
				}
			}
			HgGetVectByPln(Num, PntW1, &Vect);											// 法線ﾍﾞｸﾄﾙ
			HsDirectSetTVect(PntW1, Num, PntW1, 2000.f, 500.f, PntT);					// ﾃｷｽﾁｬ座標値を得る
			for (ic = 0 ; ic < Num ; ic++) { 
				HsDirectSetLVertex(&PntW1[ic], &Vect, Rgb, PntT[ic].X, PntT[ic].Y, &Vertex[ic]);
			}

			LpD3DDEV->CreateVertexBuffer(sizeof(HDXVERTEX) * Num, 0, HDX_LVERTEX, D3DPOOL_MANAGED, &m_VertBuf);
			m_VertBuf->Lock(0, 0, (BYTE **)&VMemory, 0);
			memcpy(VMemory, Vertex, sizeof(HDXVERTEX) * Num);
			m_VertBuf->Unlock();

			break;

		case  HPRIM_MSH:																// ﾒｯｼｭ
			m_NumTri = Ip1 = Ip2 = 0;
			for (ic = 0 ; ic < m_Prim.Mesh.Num ; ic++) {								// 面の数
				for (jc = 0 ; jc < m_Prim.Mesh.VerNumPnt[ic] ; jc++) {					// 頂点数
					Idx                                   = m_Prim.Mesh.PntNo[Ip1];
					PntW1[m_Prim.Mesh.VerNumPnt[ic]-jc-1] = m_Prim.Mesh.Pnt[Idx];
					Ip1++;
				}
				NumTri = HgDivAraToTriN(m_Prim.Mesh.VerNumPnt[ic], PntW1, PntW2);		// ３角形分割

				for (jc = 0 ; jc < NumTri ; jc++) {
					HgGetVectByPln(&PntW2[jc*3], &Vect);								// 法線ﾍﾞｸﾄﾙ
					HsDirectSetTVect(&PntW2[jc*3], 3, &PntW2[jc*3], 2000.f, 500.f, PntT);	// ﾃｷｽﾁｬ座標値を得る
					for (kc = 0 ; kc < 3 ; kc++) {
						HsDirectSetLVertex(&PntW2[jc*3+kc], &Vect, Rgb, PntT[kc].X, PntT[kc].Y, &Vertex[Ip2]);
						Ip2++;
					}
				}
				m_NumTri +=NumTri;
			}
			if (m_NumTri > 0) {
				LpD3DDEV->CreateVertexBuffer(sizeof(HDXVERTEX) * m_NumTri * 3, 0, HDX_LVERTEX, D3DPOOL_MANAGED, &m_VertBuf);
				m_VertBuf->Lock(0, 0, (BYTE **)&VMemory, 0);
				memcpy(VMemory, Vertex, sizeof(HDXVERTEX) * m_NumTri * 3);
				m_VertBuf->Unlock();
			}
			break;
		default:
			break;
	}

	HFREE(Vertex);

	return;
}

/******************************************************************************************************************
[ MDxfPrim::SetDirectXE ]             機能  エッジをＤｉｒｅｃｔＸに設定する
******************************************************************************************************************/
HVOID MDxfPrim::SetDirectXE   (
								LPDIRECT3DDEVICE8	LpD3DDEV,							// (I  ) DirectXﾃﾞﾊﾞｲｽ
								MINT				Symtry)								// (I  ) 対称ﾌﾗｸﾞ  0:なし  1:あり
{
	HDXLVERTEX*			Vertex  = NULL;
	HDXLVERTEX*			VMemory = NULL;
	MgPoint3				PntW1[64];
	MgPoint2				PntW2[64];
	MgPoint2				PntW3[64];
	HCIR2				CirW1;
	HARC2				ArcW1;
	HPLN3				Pln;
	MgPoint3				Uvw[3];
	MINT				Ip1, Ip2, Idx;
	MINT				ItrSts;
	MINT				ic, jc, nc;

	/*
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	#ifdef _DEBUG
	MCHAR			StrBuf[256];
	BwsSprintf(StrBuf, BwsStr("ＤＸＦ タイプ   [%d] \n"), m_PrmType);
	TRACE(StrBuf);
	#endif
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	*/

	if ((Vertex = (HDXLVERTEX *)malloc(sizeof(HDXLVERTEX) * 2048 * 6)) == NULL)  HMFAIL;

	LpD3DDEV->SetVertexShader(HDX_LLVERTEX);											// 頂点ﾌｫｰﾏｯﾄ設定
	switch  (m_PrmType) {
		case  HPRIM_DOT:																// 点
			break;
		case  HPRIM_LIN:																// 線
			/*
			/////////////////////////////////////////////////////////////////////////////////////////////////////////
			#ifdef _DEBUG
			BwsSprintf(StrBuf, BwsStr("ＬＩＮＥ        [%d] \n"), 2);
			TRACE(StrBuf);
			for (ic = 0 ; ic < 2 ; ic++) {
				BwsSprintf(StrBuf, BwsStr("%8.2f %8.2f %8.2f \n"), m_Prim.Line.Pnt[ic].X, m_Prim.Line.Pnt[ic].Y, m_Prim.Line.Pnt[ic].Z); 
				TRACE(StrBuf);
			}
			#endif
			/////////////////////////////////////////////////////////////////////////////////////////////////////////
			*/
			for (ic = 0 ; ic < 2 ; ic++) {
				Vertex[ic*2  ].Pnt.x = m_Prim.Line.Pnt[ic].X;
				Vertex[ic*2  ].Pnt.y = m_Prim.Line.Pnt[ic].Y;
				Vertex[ic*2  ].Pnt.z = m_Prim.Line.Pnt[ic].Z;
				Vertex[ic*2  ].Rgb   = D3DCOLOR_RGBA(1, 1, 1, 1);
				Vertex[ic*2+1].Pnt.x = m_Prim.Line.Pnt[ic+1].X;
				Vertex[ic*2+1].Pnt.y = m_Prim.Line.Pnt[ic+1].Y;
				Vertex[ic*2+1].Pnt.z = m_Prim.Line.Pnt[ic+1].Z;
				Vertex[ic*2+1].Rgb   = D3DCOLOR_RGBA(1, 1, 1, 1);
			}
			LpD3DDEV->CreateVertexBuffer(sizeof(HDXLVERTEX) * 2 * 2, 0, HDX_LLVERTEX, D3DPOOL_MANAGED, &m_LineBuf);
			m_LineBuf->Lock(0, 0, (BYTE **)&VMemory, 0);
			memcpy(VMemory, Vertex, sizeof(HDXLVERTEX) * 2 * 2);
			m_LineBuf->Unlock();
			break;
		case  HPRIM_CIR:																// 円:
			/*
			/////////////////////////////////////////////////////////////////////////////////////////////////////////
			#ifdef _DEBUG
			BwsSprintf(StrBuf, BwsStr("ＣＩＲ        [%d] \n"), 2);
			TRACE(StrBuf);
			BwsSprintf(StrBuf, BwsStr("%8.2f %8.2f %8.2f \n"), m_Prim.Circle.Pnt1.X, m_Prim.Circle.Pnt1.Y, m_Prim.Circle.Pnt1.Z);
			TRACE(StrBuf);
			BwsSprintf(StrBuf, BwsStr("%8.2f %8.2f %8.2f \n"), m_Prim.Circle.Pnt2.X, m_Prim.Circle.Pnt2.Y, m_Prim.Circle.Pnt2.Z);
			TRACE(StrBuf);
			#endif
			/////////////////////////////////////////////////////////////////////////////////////////////////////////
			*/

			PntW1[0]   = m_Prim.Circle.Pnt1;											// 開始
			PntW1[1]   = m_Prim.Circle.Pnt2;											// 終了
			PntW1[1].Z = m_Prim.Circle.Pnt1.Z;											// 終了
			PntW1[2]   = m_Prim.Circle.Pnt2;											// 終了

			ItrSts = HgGetPlnByPnt(PntW1, &Pln);										// ３点より平面の標準形式を求める
			ItrSts = HgGetLocVect(&PntW1[0], &PntW1[1], &Pln.Vect, Uvw);				// ﾛｰｶﾙ座標系の単位方向ﾍﾞｸﾄﾙを求める
			HgTrnWrdToLoc(&PntW1[0], Uvw, 3, PntW1, PntW2);								// ﾛｰｶﾙ座標系へ変換

			HgTrnCirInf(&PntW2[0], &PntW2[1], &CirW1);									// 円造体

			m_NumLne = HgGetHPntByCir(&CirW1, 19, PntW2);
			HgTrnLocToWrd(&PntW1[0], Uvw, m_NumLne, PntW2, PntW1);						// ﾜｰﾙﾄﾞ座標値に変換

			for (ic = 0 ; ic < m_NumLne-1 ; ic++) {
				Vertex[ic*2  ].Pnt.x = PntW1[ic].X;   Vertex[ic*2  ].Pnt.y = PntW1[ic  ].Y; Vertex[ic*2  ].Pnt.z = PntW1[ic  ].Z;
				Vertex[ic*2  ].Rgb   = D3DCOLOR_RGBA(1, 1, 1, 1);
				Vertex[ic*2+1].Pnt.x = PntW1[ic+1].X; Vertex[ic*2+1].Pnt.y = PntW1[ic+1].Y; Vertex[ic*2+1].Pnt.z = PntW1[ic+1].Z;
				Vertex[ic*2+1].Rgb   = D3DCOLOR_RGBA(1, 1, 1, 1);
			}

			if (m_NumLne > 1) {
				LpD3DDEV->CreateVertexBuffer(sizeof(HDXLVERTEX) * (m_NumLne-1) * 2, 0, HDX_LLVERTEX, D3DPOOL_MANAGED, &m_LineBuf);
				m_LineBuf->Lock(0, 0, (BYTE **)&VMemory, 0);
				memcpy(VMemory, Vertex, sizeof(HDXLVERTEX) * (m_NumLne-1) * 2);
				m_LineBuf->Unlock();
			}
			break;
		case  HPRIM_ARC:																// 円弧
			/*
			/////////////////////////////////////////////////////////////////////////////////////////////////////////
			#ifdef _DEBUG
			BwsSprintf(StrBuf, BwsStr("ＡＲＣ        [%d] \n"), 2);
			TRACE(StrBuf);
			BwsSprintf(StrBuf, BwsStr("%8.2f %8.2f %8.2f \n"), m_Prim.Arc.Pnt1.X, m_Prim.Arc.Pnt1.Y, m_Prim.Arc.Pnt1.Z);
			TRACE(StrBuf);
			BwsSprintf(StrBuf, BwsStr("%8.2f %8.2f %8.2f \n"), m_Prim.Arc.Pnt2.X, m_Prim.Arc.Pnt2.Y, m_Prim.Arc.Pnt2.Z);
			TRACE(StrBuf);
			BwsSprintf(StrBuf, BwsStr("%8.2f %8.2f %8.2f \n"), m_Prim.Arc.Pnt3.X, m_Prim.Arc.Pnt3.Y, m_Prim.Arc.Pnt3.Z);
			TRACE(StrBuf);
			BwsSprintf(StrBuf, BwsStr("%8.2f %8.2f %8.2f \n"), m_Prim.Arc.Pnt4.X, m_Prim.Arc.Pnt4.Y, m_Prim.Arc.Pnt4.Z);
			TRACE(StrBuf);
			#endif
			/////////////////////////////////////////////////////////////////////////////////////////////////////////
			*/
			PntW1[0]   = m_Prim.Arc.Pnt1;												// 終了
			PntW1[1]   = m_Prim.Arc.Pnt2;												// 終了
			PntW1[1].Z = m_Prim.Arc.Pnt1.Z;												// 終了
			PntW1[2]   = m_Prim.Arc.Pnt2;												// 終了

			ItrSts = HgGetPlnByPnt(PntW1, &Pln);										// ３点より平面の標準形式を求める
			ItrSts = HgGetLocVect(&PntW1[0], &PntW1[1], &Pln.Vect, Uvw);				// ﾛｰｶﾙ座標系の単位方向ﾍﾞｸﾄﾙを求める
			HgTrnWrdToLoc(&PntW1[0], Uvw, 4, (MgPoint3 *)&m_Prim.Arc, PntW2);			// ﾛｰｶﾙ座標系へ変換

			PntW3[0].X = HMIN(PntW2[0].X, PntW2[1].X);
			PntW3[0].Y = HMIN(PntW2[0].Y, PntW2[1].Y);
			PntW3[1].X = HMAX(PntW2[0].X, PntW2[1].X);
			PntW3[1].Y = HMAX(PntW2[0].Y, PntW2[1].Y);
//			if (Symtry == 0) {															// 対称なし
				PntW3[2] = PntW2[2];
				PntW3[3] = PntW2[3];
//			} else {
//				PntW3[2] = PntW2[3];
//				PntW3[3] = PntW2[2];
//			}
			HgTrnArcInf(&PntW3[0], &PntW3[1], &PntW3[2], &PntW3[3], &ArcW1);			// 円弧構造体

			m_NumLne = HgGetHPntByArc(&ArcW1, 0, PntW2);
			HgTrnLocToWrd(&PntW1[0], Uvw, m_NumLne, PntW2, PntW1);						// ﾜｰﾙﾄﾞ座標値に変換

			for (ic = 0 ; ic < m_NumLne-1 ; ic++) {
				Vertex[ic*2  ].Pnt.x = PntW1[ic].X;   Vertex[ic*2  ].Pnt.y = PntW1[ic  ].Y; Vertex[ic*2  ].Pnt.z = PntW1[ic  ].Z;
				Vertex[ic*2  ].Rgb   = D3DCOLOR_RGBA(1, 1, 1, 1);
				Vertex[ic*2+1].Pnt.x = PntW1[ic+1].X; Vertex[ic*2+1].Pnt.y = PntW1[ic+1].Y; Vertex[ic*2+1].Pnt.z = PntW1[ic+1].Z;
				Vertex[ic*2+1].Rgb   = D3DCOLOR_RGBA(1, 1, 1, 1);
			}
			if (m_NumLne > 1) {
				LpD3DDEV->CreateVertexBuffer(sizeof(HDXLVERTEX) * (m_NumLne-1) * 2, 0, HDX_LLVERTEX, D3DPOOL_MANAGED, &m_LineBuf);
				m_LineBuf->Lock(0, 0, (BYTE **)&VMemory, 0);
				memcpy(VMemory, Vertex, sizeof(HDXLVERTEX) * (m_NumLne-1) * 2);
				m_LineBuf->Unlock();
			}
			break;
		case  HPRIM_TXT:																// 文字列
			break;

		case  HPRIM_POL:																// 面
			/*
			/////////////////////////////////////////////////////////////////////////////////////////////////////////
			#ifdef _DEBUG
			BwsSprintf(StrBuf, BwsStr("３ＤＦＡＣＥ    [%d] \n"), m_Prim.Face.NumPnt);
			TRACE(StrBuf);
			for (ic = 0 ; ic < m_Prim.Face.NumPnt ; ic++) {
				BwsSprintf(StrBuf, BwsStr("%8.2f %8.2f %8.2f \n"), m_Prim.Face.Pnt[ic].X, m_Prim.Face.Pnt[ic].Y, m_Prim.Face.Pnt[ic].Z); 
				TRACE(StrBuf);
			}
			#endif
			/////////////////////////////////////////////////////////////////////////////////////////////////////////
			*/
			m_NumLne = m_Prim.Face.NumPnt;
			if (HgCmpPnt(&m_Prim.Face.Pnt[0], &m_Prim.Face.Pnt[m_Prim.Face.NumPnt-1], 1.0f) == 1) {	// 始点･終点同じ
				m_NumLne--;
			} else {
				if (m_Prim.Face.CFlag == 0)  m_NumLne--;
			}
			for (ic = 0 ; ic < m_Prim.Face.NumPnt ; ic++) {								// 周り順逆転
				PntW1[m_Prim.Face.NumPnt-ic-1] = m_Prim.Face.Pnt[ic];
			}
			for (ic = 0 ; ic < m_NumLne ; ic++) {
				nc = HPTR(ic+1, m_NumLne);
				Vertex[ic*2  ].Pnt.x = PntW1[ic].X; Vertex[ic*2  ].Pnt.y = PntW1[ic].Y; Vertex[ic*2  ].Pnt.z = PntW1[ic].Z;
				Vertex[ic*2  ].Rgb   = D3DCOLOR_RGBA(1, 1, 1, 1);
				Vertex[ic*2+1].Pnt.x = PntW1[nc].X; Vertex[ic*2+1].Pnt.y = PntW1[nc].Y; Vertex[ic*2+1].Pnt.z = PntW1[nc].Z;
				Vertex[ic*2+1].Rgb   = D3DCOLOR_RGBA(1, 1, 1, 1);
			}

			LpD3DDEV->CreateVertexBuffer(sizeof(HDXLVERTEX) * m_NumLne * 2, 0, HDX_LLVERTEX, D3DPOOL_MANAGED, &m_LineBuf);
			m_LineBuf->Lock(0, 0, (BYTE **)&VMemory, 0);
			memcpy(VMemory, Vertex, sizeof(HDXLVERTEX) * m_NumLne * 2);
			m_LineBuf->Unlock();
			break;

		case  HPRIM_MSH:																// ﾒｯｼｭ
			m_NumLne = Ip1 = Ip2 = 0;
			for (ic = 0 ; ic < m_Prim.Mesh.Num ; ic++) {								// 面の数
				for (jc = 0 ; jc < m_Prim.Mesh.VerNumPnt[ic] ; jc++) {					// 頂点数
					Idx                                   = m_Prim.Mesh.PntNo[Ip1];
					PntW1[m_Prim.Mesh.VerNumPnt[ic]-jc-1] = m_Prim.Mesh.Pnt[Idx];
					Ip1++;
				}
				for (jc = 0 ; jc < m_Prim.Mesh.VerNumPnt[ic] ; jc++) {					// 頂点数
					nc = HPTR(jc+1, m_Prim.Mesh.VerNumPnt[ic]);
					Vertex[Ip2].Pnt.x = PntW1[jc].X; Vertex[Ip2].Pnt.y = PntW1[jc].Y; Vertex[Ip2].Pnt.z = PntW1[jc].Z;
					Vertex[Ip2].Rgb   = D3DCOLOR_RGBA(1, 1, 1, 1);
					Ip2++;
					Vertex[Ip2].Pnt.x = PntW1[nc].X; Vertex[Ip2].Pnt.y = PntW1[nc].Y; Vertex[Ip2].Pnt.z = PntW1[nc].Z;
					Vertex[Ip2].Rgb   = D3DCOLOR_RGBA(1, 1, 1, 1);
					Ip2++;
					m_NumLne++;
				}
			}
			if (m_NumLne > 0) {
				LpD3DDEV->CreateVertexBuffer(sizeof(HDXLVERTEX) * m_NumLne * 2, 0, HDX_LLVERTEX, D3DPOOL_MANAGED, &m_LineBuf);
				m_LineBuf->Lock(0, 0, (BYTE **)&VMemory, 0);
				memcpy(VMemory, Vertex, sizeof(HDXLVERTEX) * m_NumLne * 2);
				m_LineBuf->Unlock();
			}
			break;
		default:
			break;
	}

	HFREE(Vertex);																		// SHIFT,CTRL

	return;
}

/******************************************************************************************************************
[ MDxfPrim::ShowDirectXF ]            機能  面をＤｉｒｅｃｔＸで表示する
******************************************************************************************************************/
HVOID MDxfPrim::ShowDirectXF  (
								LPDIRECT3DDEVICE8		LpD3DDEV)						// (I  ) DirectXﾃﾞﾊﾞｲｽ
{
	LpD3DDEV->SetVertexShader(HDX_LVERTEX);												// 頂点ﾌｫｰﾏｯﾄ設定

	switch  (m_PrmType) {
		case  HPRIM_DOT:																// 点
			break;
		case  HPRIM_LIN:																// 線
			break;
		case  HPRIM_CIR:																// 円:
			break;
		case  HPRIM_ARC:																// 円弧
			break;
		case  HPRIM_TXT:																// 文字列
			break;
		case  HPRIM_POL:																// 面
		case  HPRIM_MSH:																// ﾒｯｼｭ
			if (m_NumTri > 0) {
				LpD3DDEV->SetStreamSource(0, m_VertBuf, sizeof(HDXVERTEX));
				LpD3DDEV->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, m_NumTri);				// ３角形の数
				LpD3DDEV->SetStreamSource(0, NULL, 0);
			}
			break;
		default:
			break;
	}

	return;
}

/******************************************************************************************************************
[ MDxfPrim::ShowDirectXE ]            機能  エッジをＤｉｒｅｃｔＸで表示する
******************************************************************************************************************/
HVOID MDxfPrim::ShowDirectXE  (
								LPDIRECT3DDEVICE8		LpD3DDEV)						// (I  ) DirectXﾃﾞﾊﾞｲｽ
{
	LpD3DDEV->SetVertexShader(HDX_LLVERTEX);											// 頂点ﾌｫｰﾏｯﾄ設定

	switch  (m_PrmType) {
		case  HPRIM_DOT:																// 点
			break;
		case  HPRIM_LIN:																// 線
			LpD3DDEV->SetStreamSource(0, m_LineBuf, sizeof(HDXLVERTEX));
			LpD3DDEV->DrawPrimitive(D3DPT_LINELIST, 0, 1);								// 線分の数
			LpD3DDEV->SetStreamSource(0, NULL, 0);
			break;
		case  HPRIM_CIR:																// 円:
		case  HPRIM_ARC:																// 円弧
			if (m_NumLne > 0) {
				LpD3DDEV->SetStreamSource(0, m_LineBuf, sizeof(HDXLVERTEX));
				LpD3DDEV->DrawPrimitive(D3DPT_LINELIST, 0, m_NumLne);					// 線分の数
				LpD3DDEV->SetStreamSource(0, NULL, 0);
			}
			break;
		case  HPRIM_TXT:																// 文字列
			break;
		case  HPRIM_POL:																// 面
		case  HPRIM_MSH:																// ﾒｯｼｭ
			if (m_NumLne > 0) {
				LpD3DDEV->SetStreamSource(0, m_LineBuf, sizeof(HDXLVERTEX));
				LpD3DDEV->DrawPrimitive(D3DPT_LINELIST, 0, m_NumLne);					// 線分の数
				LpD3DDEV->SetStreamSource(0, NULL, 0);
			}
			break;
		default:
			break;
	}

	return;
}

/******************************************************************************************************************
[ MDxfPrim::RelDirectXF ]              機能  面をＤｉｒｅｃｔＸより開放する
******************************************************************************************************************/
HVOID MDxfPrim::RelDirectXF   ()
{
	RELEASE(m_VertBuf);																	// DirectX 頂点ﾊﾞｯﾌｧ
	m_VertBuf = NULL;
	m_NumTri  = 0;																		// ３角形の数

	return;
}

/******************************************************************************************************************
[ MDxfPrim::RelDirectXE ]             機能  エッジをＤｉｒｅｃｔＸより開放する
******************************************************************************************************************/
HVOID MDxfPrim::RelDirectXE   ()
{
	RELEASE(m_LineBuf);																	// DirectX 線分ﾊﾞｯﾌｧ
	m_LineBuf = NULL;																	// DirectX 線分ﾊﾞｯﾌｧ
	m_NumLne  = 0;																		// 線分の数

	return;
}

/******************************************************************************************************************
[機能] 最小･最大を設定
[返値] なし
******************************************************************************************************************/
static HVOID SetMinMax         (
								MgPoint3*			Pnt,								// (I  ) 座標値
								MgMinMax3*			MinMax)								// (I/O) 最小･最大
{
	if (Pnt->X < MinMax->Min.X)  MinMax->Min.X = Pnt->X;
	if (Pnt->Y < MinMax->Min.Y)  MinMax->Min.Y = Pnt->Y;
	if (Pnt->Z < MinMax->Min.Z)  MinMax->Min.Z = Pnt->Z;
	if (Pnt->X > MinMax->Max.X)  MinMax->Max.X = Pnt->X;
	if (Pnt->Y > MinMax->Max.Y)  MinMax->Max.Y = Pnt->Y;
	if (Pnt->Z > MinMax->Max.Z)  MinMax->Max.Z = Pnt->Z;

	return;
}
static HVOID SetMinMax         (
								MgPoint2*			Pnt,								// (I  ) 座標値
								MgMinMax3*			MinMax)								// (I/O) 最小･最大
{
	if (Pnt->X < MinMax->Min.X)  MinMax->Min.X = Pnt->X;
	if (Pnt->Y < MinMax->Min.Y)  MinMax->Min.Y = Pnt->Y;
	if (Pnt->X > MinMax->Max.X)  MinMax->Max.X = Pnt->X;
	if (Pnt->Y > MinMax->Max.Y)  MinMax->Max.Y = Pnt->Y;

	return;
}

/******************************************************************************************************************
[機能] Ｖｅｒｔｅｘへ情報を設定
[返値] ステータス      ０：正常  １：エラー
******************************************************************************************************************/
static MINT SetVertex          (														// (  O) ｽﾃｰﾀｽ  0:正常 1:ｴﾗｰ
								MINT				NumPnt,								// (I  ) 頂点数
								MgPoint3*			Pnt,								// (I  ) 座標値
								D3DCOLOR			Rgb,								// (I  ) 色
								MINT*				NumTri,								// (I/O) ３角形数
								HDXVERTEX*			Vertex,								// (I/O) DirectX
								MINT*				IPtr)								// (I/O) ﾎﾟｲﾝﾀ
{
	MINT				Status;
	MgPoint3				Vect;															// 法線方向
	MgPoint3				PntT[64];
	MgPoint2				PntX[64];
	MINT				IWork;
	MINT				ic;


	Status = 1;

	HgGetVectByPln(Pnt, &Vect);															// 法線ﾍﾞｸﾄﾙ
	IWork = HgDivAraToTri(NumPnt, Pnt, PntT);											// ３角形分割
	HsDirectSetTVect(Pnt, IWork * 3, PntT, 2000.f, 500.f, PntX);						// ﾃｷｽﾁｬ座標値を得る
	for (ic = 0 ; ic < IWork * 3 ; ic++) { 
		HsDirectSetLVertex(&PntT[ic], &Vect, Rgb, PntX[ic].X, PntX[ic].Y, &Vertex[*IPtr]);
		(*IPtr)++;
	}
	(*NumTri) += IWork;
	Status     = 0;

	return(Status);
}

/******************************************************************************************************************
[機能] ローカル座標系へ変換
[返値] なし
******************************************************************************************************************/
static HVOID TrnWrdToLoc       (
								MgPoint3*			TOPnt,								// (I  ) 2次元⇔3次元変換用原点
								MgPoint3*			TUvw,								// (I  ) 2次元⇔3次元変換用UVW
								MgPoint3*			Pnt1,								// (I  ) 3次元
								MgPoint2*			Pnt2)								// (  O) 2次元
{

	if (TOPnt && TUvw) {
		HgTrnWrdToLoc(TOPnt, TUvw, 1, Pnt1, Pnt2);										// ﾛｰｶﾙ座標系へ変換
	} else {
		memcpy(Pnt2, Pnt1, sizeof(MgPoint2));
	}

	return;
}

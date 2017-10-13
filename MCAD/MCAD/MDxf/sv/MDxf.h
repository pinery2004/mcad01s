/*********************************************************************************************************************************
              ＤＸＦ  ヘッダファイル (HsDxf.h)

********************************************************************************************************************************/

#ifndef			_HSDXF_
	#define		_HSDXF_

#include "MCad.h"
//#include		"HBase.h"
//#include		"HXBase.h"#define			 NAMMAX				64													// 名前最大長
//#include		"HnDisp.h"
//#include		"HlDList.h"
//#include		"HstackId.h"

typedef union _hxprim {									// 図形
	HPOINT3			Dot;								// 点
	HLINE3			Line;								// 線
	HCIR3S			Circle;								// 円
	HARC3S			Arc;								// 円弧
	HXTEXT			Text;								// 文字列
	HFACE3			Face;								// 面
	HXMESH3			Mesh;								// ﾒｯｼｭ
} HXPRIM;


/*************************************************************************************************************
[ HsDxfPrim ]                          図形情報
***************************************************************************************************************/

class   HsDxfPrim
{
public:

	MINT						m_PrmType;												// ﾌﾟﾘﾐﾃｨﾌﾞﾀｲﾌﾟ
	MINT						m_Color;												// 色番号
	MINT						m_LineNo;												// 線種番号（番号）
	MINT						m_LineWd;												// 線幅番号（番号）
	HXPRIM						m_Prim;													// 図形
	MINT						m_NumTri;												// ３角形の数
	LPDIRECT3DVERTEXBUFFER8		m_VertBuf;												// DirectX 頂点ﾊﾞｯﾌｧ
	MINT						m_NumLne;												// 線分の数
	LPDIRECT3DVERTEXBUFFER8		m_LineBuf;												// ３次元の線分を構成する線

	// コンストラクタ
	HsDxfPrim                  (
								MINT				PrmType,							// (I  ) ﾌﾟﾘﾐﾃｨﾌﾞﾀｲﾌﾟ
								MINT				Color,								// (I  ) 色番号
								MINT				LineNo,								// (I  ) 線種番号（番号）
								MINT				LineWd,								// (I  ) 線幅番号（番号）
								HXPRIM*				Prim);								// (I  ) 図形

	// デストラクタ
	~HsDxfPrim                 ();

	// 変形
	HVOID Transform            (
								MINT				Color,								// (I  ) 色番号
								HPOINT3*			Base,								// (I  ) 基点
								HPOINT3*			Insert,								// (I  ) 挿入点
								HPOINT3*			Scale,								// (I  ) 尺度
								HREAL				Rotate);							// (I  ) 回転
	HVOID Transform            (
								MINT				TFlag,								// (I  ) 0:変換せず 1:変換
								MINT				Symtry,								// (I  ) 対称ﾌﾗｸﾞ  0:なし  1:あり
								HMTRX3				Matrix,								// (I  ) 変換ﾏﾄﾘｸｽ
								HPOINT3*			Move,								// (I  ) 移動量
								HREAL*				Scale,								// (I  ) ｽｹｰﾙ
								HMINMAX3*			MinMax);							// (  O) 最小･最大

	// 変形（Ｙ→Ｚ）
	HVOID TransformYZ          (
								HMINMAX3*			MinMax);							// (  O) 最小･最大

	// 図形情報を表示する
	HVOID Show                 (
								CDC*				MemDC,								// (I  ) ﾒﾓﾘDC
								MINT				MainId,								// (I  ) ﾒｲﾝID
								MINT				SubsId,								// (I  ) ｻﾌﾞID
								COLORREF			Color,								// (I  ) 表示色
								MINT				Symtry,								// (I  ) 対称ﾌﾗｸﾞ  0:なし  1:あり
								HMTRX3				Matrix,								// (I  ) 変換ﾏﾄﾘｸｽ
								HPOINT3*			Move,								// (I  ) 移動量
								HREAL				Scale,								// (I  ) ｽｹｰﾙ
								HPOINT2*			CutPnt = NULL,						// (I  ) ｶｯﾄ線 + 方向点
								HMINMAX3*			MinMax = NULL);						// (  O) 最小･最大

	// 立面情報の作成（線）
	HVOID MakeRL               (
								MINT				VewCod,								// (I  ) ﾋﾞｭｰｺｰﾄﾞ 0:東 1:西 2:南 3:北
								MINT				Kai,								// (I  ) 階
								HnDisp*				DispRL,								// (I  ) 立面表示情報（線分）
								COLORREF			Rgb,								// (I  ) 表示色
								MINT				Symtry,								// (I  ) 対称ﾌﾗｸﾞ  0:なし  1:あり
								HMTRX3				Matrix,								// (I  ) 変換ﾏﾄﾘｸｽ
								HPOINT3*			Move,								// (I  ) 移動量
								HREAL				Scale,								// (I  ) ｽｹｰﾙ
								HPOINT3*			TOPnt  = NULL,						// (I  ) 2次元⇔3次元変換用原点
								HPOINT3*			TUvw   = NULL,						// (I  ) 2次元⇔3次元変換用UVW
								HMINMAX3*			MinMax = NULL);						// (  O) 最小･最大

	// 陰線消去
	MINT HiddenLine            (														// (  O) 線分数
								MINT				VewCod,								// (I  ) ﾋﾞｭｰｺｰﾄﾞ 0:東 1:西 2:南 3:北
								MINT				Kai,								// (I  ) 階
								HPOINT3*			TOPnt,								// (I  ) 2次元⇔3次元変換用原点
								HPOINT3*			TUvw,								// (I  ) 2次元⇔3次元変換用UVW
								MINT				NumPnt1,							// (I  ) 頂点数
								HPOINT3*			Pnt1,								// (I  ) 頂点
								HLINE2*				Line1);								// (I  ) 線分

	// 陰線チェック（最小・最大）
	MINT HiddenCheck           (														// (  O) 0:見えない 1:見える
 								MINT				VewCod,								// (I  ) ﾋﾞｭｰｺｰﾄﾞ 0:東 1:西 2:南 3:北
								MINT				Kai,								// (I  ) 階
								HPOINT3*			TOPnt,								// (I  ) 2次元⇔3次元変換用原点
								HPOINT3*			TUvw,								// (I  ) 2次元⇔3次元変換用UVW
								MINT				NumPnt1,							// (I  ) 頂点数
								HPOINT3*			Pnt1);								// (I  ) 頂点

	// プリミティブ数を得る
	MINT NumDxfPrim            (														// (  O) ﾌﾟﾘﾐﾃｨﾌﾞ数
								MINT				Symtry,								// (I  ) 対称ﾌﾗｸﾞ  0:なし  1:あり
								HMTRX3				Matrix,								// (I  ) 変換ﾏﾄﾘｸｽ
								HPOINT3*			Move,								// (I  ) 移動量
								HREAL				Scale,								// (I  ) ｽｹｰﾙ
								HPOINT2*			CutPnt);							// (I  ) ｶｯﾄ線 + 方向点

	// ＤＸＦ（Ｒ１４）形式出力
	MINT DxfR14Out             (														// (  O) ｽﾃｰﾀｽ  0:正常 以外:ｴﾗｰ
								CStdioFile*			FLPtr,								// (I  ) ﾌｧｲﾙ識別子
								MINT*				NumHnd,								// (I/O) ﾊﾝﾄﾞﾙ数
								HCHAR*				LayNam,								// (I  ) ﾚｲﾔ名
								MINT				Symtry,								// (I  ) 対称ﾌﾗｸﾞ  0:なし  1:あり
								HMTRX3				Matrix,								// (I  ) 変換ﾏﾄﾘｸｽ
								HPOINT3*			Move,								// (I  ) 移動量
								HREAL				Scale,								// (I  ) ｽｹｰﾙ
								HPOINT2*			CutPnt = NULL);

	// 面をＤｉｒｅｃｔＸに設定する
	HVOID SetDirectXF          (
								LPDIRECT3DDEVICE8	LpD3DDEV,							// (I  ) DirectXﾃﾞﾊﾞｲｽ
								COLORREF			Rgb1,								// (I  ) RGBｶﾗｰ値
								MINT				FAFlag);							// (I  ) 面の向き調整   0:しない  1:する
	// エッジをＤｉｒｅｃｔＸに設定する
	HVOID SetDirectXE          (
								LPDIRECT3DDEVICE8	LpD3DDEV,							// (I  ) DirectXﾃﾞﾊﾞｲｽ
								MINT				Symtry);							// (I  ) 対称ﾌﾗｸﾞ  0:なし  1:あり

	// 面をＤｉｒｅｃｔＸで表示する
	HVOID ShowDirectXF         (														// (  O) ｽﾃｰﾀｽ
								LPDIRECT3DDEVICE8	LpD3DDEV);							// (I  ) DirectXﾃﾞﾊﾞｲｽ
	// エッジをＤｉｒｅｃｔＸで表示する
	HVOID ShowDirectXE         (														// (  O) ｽﾃｰﾀｽ
								LPDIRECT3DDEVICE8	LpD3DDEV);							// (I  ) DirectXﾃﾞﾊﾞｲｽ

	// 面をＤｉｒｅｃｔＸより開放する
	HVOID RelDirectXF          ();
	// エッジをＤｉｒｅｃｔＸより開放する
	HVOID RelDirectXE          ();


private:

};


/*************************************************************************************************************
[ HsDxfBlock ]                         ブロック情報
***************************************************************************************************************/

class   HsDxfBlock
{
public:

	HCHAR						m_Name[NAMMAX];											// ﾌﾞﾛｯｸ名
	HCHAR						m_LName[NAMMAX];										// ﾚｲﾔ名
	MINT						m_Color;												// 色番号
	COLORREF					m_Rgb;													// RGBｶﾗｰ値
	LPDIRECT3DTEXTURE8			m_Texture;												// ﾃｷｽﾁｬ
	MINT						m_LineNo;												// 線番号
	HPOINT3						m_InsPnt;												// 挿入基点
	HlDList*					m_PrimList;												// ﾌﾟﾘﾐﾃｨﾌﾞ･ﾘｽﾄのﾎﾟｲﾝﾀ

	// コンストラクタ
	HsDxfBlock                 ();
	HsDxfBlock                 (
								HCHAR*				Name,								// (I  ) ﾚｲﾔ名
								HCHAR*				LName,								// (I  ) ﾚｲﾔ名
								MINT				Color,								// (I  ) 色番号
								MINT				LineNo,								// (I  ) 線番号
								HPOINT3*			InsPnt);							// (I  ) 挿入基点

	// デストラクタ
	~HsDxfBlock                ();

	// ブロック情報をセット
	HVOID SetInf               (
								HCHAR*				Name,								// (I  ) ﾌﾞﾛｯｸ名
								HCHAR*				LName,								// (I  ) ﾚｲﾔ名
								MINT				Color,								// (I  ) 色番号
								MINT				LineNo,								// (I  ) 線番号
								HPOINT3*			InsPnt);							// (I  ) 挿入基点

	// 図形情報を追加
	HsDxfPrim* AddPrim         (														// (  O) 図形情報のｲﾝｽﾀﾝｽ
								MINT				PrmType,							// (I  ) ﾌﾟﾘﾐﾃｨﾌﾞﾀｲﾌﾟ
								MINT				Color,								// (I  ) 色番号
								MINT				LineNo,								// (I  ) 線種番号（番号）
								MINT				LineWd,								// (I  ) 線幅番号（番号）
								HXPRIM*				Prim);								// (I  ) 図形


private:

};


/*************************************************************************************************************
[ HsDxfLayer ]                         レイヤ情報
***************************************************************************************************************/

class   HsDxfLayer
{
public:

	HCHAR						m_Name[NAMMAX];											// ﾚｲﾔ名
	MINT						m_Color;												// 色番号
	COLORREF					m_Rgb;													// RGBｶﾗｰ値
	LPDIRECT3DTEXTURE8			m_Texture;												// ﾃｷｽﾁｬ
	MINT						m_LineNo;												// 線番号
	HlDList*					m_PrimList;												// ﾌﾟﾘﾐﾃｨﾌﾞ･ﾘｽﾄのﾎﾟｲﾝﾀ

	// コンストラクタ
	HsDxfLayer                 (
								HCHAR*				Name,								// (I  ) ﾚｲﾔ名
								MINT				Color,								// (I  ) 色番号
								MINT				LineNo);							// (I  ) 線番号

	// デストラクタ
	~HsDxfLayer                ();

	// 更新
	Modify                     (
								MINT				Color,								// (I  ) 色番号
								MINT				LineNo);							// (I  ) 線番号

	// 図形情報を追加
	HsDxfPrim* AddPrim         (														// (  O) 図形情報のｲﾝｽﾀﾝｽ
								MINT				PrmType,							// (I  ) ﾌﾟﾘﾐﾃｨﾌﾞﾀｲﾌﾟ
								MINT				Color,								// (I  ) 色番号
								MINT				LineNo,								// (I  ) 線種番号（番号）
								MINT				LineWd,								// (I  ) 線幅番号（番号）
								HXPRIM*				Prim);								// (I  ) 図形


private:

};

/*************************************************************************************************************
[ HsDxfLines ]                         線情報
***************************************************************************************************************/

class   HsDxfLines
{
public:

	HCHAR						m_Name[NAMMAX];											// 線名
	MINT						m_LineNo;												// 線番号

	// コンストラクタ
	HsDxfLines                 (
								HCHAR*				Name,								// (I  ) 線名
								MINT				LIneNo);							// (I  ) 線番号

	// デストラクタ
	~HsDxfLines                ();


private :

};


/*************************************************************************************************************
[ HsDxfFonts ]                         フォント情報
***************************************************************************************************************/

class   HsDxfFonts
{
public:

	HCHAR						m_Name[NAMMAX];											// ﾌｫﾝﾄ名
	MINT						m_FCode;												// ﾌｫﾝﾄｺｰﾄﾞ

	// コンストラクタ
	HsDxfFonts                 (
								HCHAR*				Name,								// (I  ) ﾌｫﾝﾄ名
								MINT				_FCode);							// (I  ) ﾌｫﾝﾄｺｰﾄﾞ

	// デストラクタ
	~HsDxfFonts                ();


private:

};


/*************************************************************************************************************
[ HsDxfInf ]                           ＤＸＦ情報
***************************************************************************************************************/

class   HsDxfInf
{
public:

	MINT						m_VewTyp;												// ﾋﾞｭｰ種類
	HREAL						m_ProtRate;												// ﾌﾟﾛｯﾄﾚｰﾄ
	MINT						m_AngDir;												// 円弧方向       1:時計周り -1:反時計周り
	MINT						m_Symtry;												// 対称ﾌﾗｸﾞ       0:なし      1:あり
	MINT						m_SymtryC;												// 現在の対称ﾌﾗｸﾞ 0:なし      1:あり
	HMTRX3						m_Matrix;												// 変換ﾏﾄﾘｸｽ
	HPOINT3						m_Move;													// 移動量
	HREAL						m_Scale[3];												// 拡大率
	HREAL						m_TextRate;												// 文字変換倍率
	MINT						m_ZmnSize;												// 図面ｻｲｽﾞ
	HMINMAX3					m_Window;												// ｳｨﾝﾄﾞｳｻｲｽﾞ
	HMINMAX3					m_ViewPort;												// ﾋﾞｭｰﾎﾟｰﾄ
	MINT						m_FAFlag;												// 面の向き調整   0:しない    1:する
	HMINMAX3					m_MinMax;												// 最小･最大
	LPDIRECT3DTEXTURE8			m_TextureY;												// ﾃｷｽﾁｬ（矢印）
	LPDIRECT3DVERTEXBUFFER8		m_VertBufY;												// DirectX 頂点ﾊﾞｯﾌｧ（矢印）
	MINT						m_NumTriY;												// ３角形の数       （矢印）
	MINT						m_TextFlg;												// ﾃｷｽﾁｬﾌﾗｸﾞ      0:管理      1:内臓
	HlDList*					m_BlockList;											// ﾌﾞﾛｯｸ･ﾘｽﾄのﾎﾟｲﾝﾀ
	HlDList*					m_LayerList;											// ﾚｲﾔ･ﾘｽﾄのﾎﾟｲﾝﾀ
	HlDList*					m_LinesList;											// 線種･ﾘｽﾄのﾎﾟｲﾝﾀ
	HlDList*					m_FontsList;											// ﾌｫﾝﾄ･ﾘｽﾄのﾎﾟｲﾝﾀ

	// コンストラクタ
	HsDxfInf                   (
								MINT				VewTyp,								// (I  ) ﾋﾞｭｰ種類
								HREAL				ProtRate,							// (I  ) ﾌﾟﾛｯﾄﾚｰﾄ
								MINT				AngDir,								// (I  ) 円弧方向 1:時計 -1:反時計
								HREAL				TextRate,							// (I  ) 文字変換倍率
								MINT				ZmnSize,							// (I  ) 図面ｻｲｽﾞ
								HMINMAX3*			Window,								// (I  ) ｳｨﾝﾄﾞｳｻｲｽﾞ
								HMINMAX3*			ViewPort,							// (I  ) ﾋﾞｭｰﾎﾟｰﾄ
								MINT				FAFlag);							// (I  ) 面の向き調整   0:しない  1:する

	// デストラクタ
	~HsDxfInf                  ();

	// ブロック情報を追加
	HsDxfBlock* AddBlock       ();														// (  O) ﾌﾞﾛｯｸ情報ｲﾝｽﾀﾝｽ

	// レイヤ情報を追加
	MINT AddLayer              (														// (  O) ｽﾃｰﾀｽ  0:追加  1:更新
								HCHAR*				Name,								// (I  ) ﾚｲﾔ名
								MINT				Color,								// (I  ) 色番号
								MINT				LineNo);							// (I  ) 線番号

	// 線情報を追加
	MINT AddLines              (														// (  O) ｽﾃｰﾀｽ
								HCHAR*				Name,								// (I  ) 線名
								MINT				LineNo);							// (I  ) 線番号

	// フォント情報を追加
	MINT AddFonts              (														// (  O) ｽﾃｰﾀｽ
								HCHAR*				Name,								// (I  ) ﾌｫﾝﾄ名
								MINT				FCode);								// (I  ) ﾌｫﾝﾄｺｰﾄﾞ

	// ブロック情報を検索
	HsDxfBlock* GetBlock       (														// (  O) ﾌﾞﾛｯｸ情報  NULL:見つからず
								HCHAR*				Name,								// (I  ) ﾌﾞﾛｯｸ名
								HCHAR*				LName  = NULL,						// (  O) ﾚｲﾔ名
								MINT*				Color  = NULL,						// (  O) 色番号
								MINT*				LineNo = NULL,						// (  O) 線番号
								HPOINT3*			InsPnt = NULL);						// (  O) 挿入基点

	// レイヤ情報を検索
	HsDxfLayer* GetLayer       (														// (  O) ﾚｲﾔ情報  NULL:見つからず
								HCHAR*				Name,								// (I  ) ﾚｲﾔ名
								MINT*				Color  = NULL,						// (  O) 色番号
								MINT*				LineNo = NULL);						// (  O) 線番号

	// 線情報を検索
	HsDxfLines* GetLines       (														// (  O) ｽﾃｰﾀｽ  0:正常  1:なし
								HCHAR*				Name,								// (I  ) 線名
								MINT*				LineNo = NULL);						// (  O) 線番号

	// フォント情報を検索
	HsDxfFonts* GetFonts       (														// (  O) ｽﾃｰﾀｽ  0:正常  1:なし
								HCHAR*				Name,								// (I  ) ﾌｫﾝﾄ名
								MINT*				FCode = NULL);						// (  O) ﾌｫﾝﾄ番号

	// ブロック情報をクリア
	HVOID ClrBlock             ();

	// レイヤ情報をクリア
	HVOID ClrLayer             ();

	// 線情報をクリア
	HVOID ClrLines             ();

	// フォント情報をクリア
	HVOID ClrFonts             ();

	// 変形
	MINT Transform             (														// (  O) ｽﾃｰﾀｽ     0:正常  1:ｴﾗｰ
								MINT				Symtry = 0,							// (I  ) 対称ﾌﾗｸﾞ  0:なし  1:あり
								HMTRX3				Matrix = NULL,						// (I  ) 変換ﾏﾄﾘｸｽ
								HPOINT3*			Move   = NULL,						// (I  ) 移動量
								HREAL*				Scale  = NULL);						// (I  ) ｽｹｰﾙ

	// 変形（Ｙ→Ｚ）
	MINT TransformYZ           ();														// (  O) ｽﾃｰﾀｽ  0:正常  1:ｴﾗｰ

	// 最小最大を得る
	HVOID GetMinMax            (														// (  O) ｽﾃｰﾀｽ  0:正常  1:ｴﾗｰ
								HMINMAX3*			MinMax);							// (  O) 最小･最大

	// 回転・移動後の最小最大を得る
	HVOID GetMinMax            (
								MINT				Symtry,								// (I  ) 対称ﾌﾗｸﾞ  0:なし  1:あり
								HMTRX3				Matrix,								// (I  ) 変換ﾏﾄﾘｸｽ  NULL:変換なし
								HPOINT3*			Move,								// (I  ) 移動量     NULL:移動なし
								HREAL*				Scale,								// (I  ) ｽｹｰﾙ       NULL:ｽｹｰﾙなし
								HMINMAX3*			MinMax);							// (  O) 最小･最大

	// 最小最大よりなる面を得る
	HVOID GetMinMaxF           (														// (  O) ｽﾃｰﾀｽ  0:正常  1:ｴﾗｰ
								MINT				VewTyp,								// (I  ) 平正背左右裏
								HMTRX3				Matrix,								// (I  ) 変換ﾏﾄﾘｸｽ
								HPOINT3*			Move,								// (I  ) 移動量
								HPOINT3*			Pnt1);								// (  O) 最小･最大の領域(4点)

	// ＤＸＦ情報を表示する
	MINT Show                  (														// (  O) ｽﾃｰﾀｽ
								MINT				MainId,								// (I  ) ﾒｲﾝID
								MINT				SubsId,								// (I  ) ｻﾌﾞID
								COLORREF			Color,								// (I  ) 表示色
								MINT				Symtry = 0,							// (I  ) 対称ﾌﾗｸﾞ      0:なし  1:あり
								HMTRX3				Matrix = NULL,						// (I  ) 変換ﾏﾄﾘｸｽ
								HPOINT3*			Move   = NULL,						// (I  ) 移動量
								HREAL				Scale  = 1.0f,						// (I  ) ｽｹｰﾙ
								HPOINT2*			CutPnt = NULL,						// (I  ) ｶｯﾄ線 + 方向点
								HMINMAX3*			MinMax = NULL);						// (  O) 最小･最大

	// 立面情報の作成（線）
	MINT MakeRL                (														// (  O) ｽﾃｰﾀｽ  0:正常  1:ｴﾗｰ
								MINT				VewCod,								// (I  ) ﾋﾞｭｰｺｰﾄﾞ 0:東 1:西 2:南 3:北
								MINT				Kai,								// (I  ) 階
								HnDisp*				DispRL,								// (I  ) 立面表示情報（線分）
								COLORREF			Rgb,								// (I  ) 表示色
								MINT				Symtry = 0,							// (I  ) 対称ﾌﾗｸﾞ  0:なし  1:あり
								HMTRX3				Matrix = NULL,						// (I  ) 変換ﾏﾄﾘｸｽ
								HPOINT3*			Move   = NULL,						// (I  ) 移動量
								HREAL				Scale  = 1.0,						// (I  ) ｽｹｰﾙ
								HPOINT3*			TOPnt  = NULL,						// (I  ) 2次元⇔3次元変換用原点
								HPOINT3*			TUvw   = NULL,						// (I  ) 2次元⇔3次元変換用UVW
								HMINMAX3*			MinMax = NULL);						// (  O) 最小･最大

	// プリミティブ数を得る
	MINT NumDxfPrim            (														// (  O) ﾌﾟﾘﾐﾃｨﾌﾞ数
								MINT				Symtry = 0,							// (I  ) 対称ﾌﾗｸﾞ  0:なし  1:あり
								HMTRX3				Matrix = NULL,						// (I  ) 変換ﾏﾄﾘｸｽ
								HPOINT3*			Move   = NULL,						// (I  ) 移動量
								HREAL				Scale  = 1.0f,						// (I  ) ｽｹｰﾙ
								HPOINT2*			CutPnt = NULL);						// (I  ) ｶｯﾄ線 + 方向点

	// ＤＸＦ（Ｒ１４）形式出力
	MINT DxfR14Out             (														// (  O) ｽﾃｰﾀｽ  0:正常 以外:ｴﾗｰ
								CStdioFile*			FLPtr,								// (I  ) ﾌｧｲﾙ識別子
								MINT*				NumHnd,								// (I/O) ﾊﾝﾄﾞﾙ数
								HCHAR*				LayNam,								// (I  ) ﾚｲﾔ名
								MINT				Symtry = 0,							// (I  ) 対称ﾌﾗｸﾞ  0:なし  1:あり
								HMTRX3				Matrix = NULL,						// (I  ) 変換ﾏﾄﾘｸｽ
								HPOINT3*			Move   = NULL,						// (I  ) 移動量
								HREAL				Scale  = 1.0f,						// (I  ) ｽｹｰﾙ
								HPOINT2*			CutPnt = NULL);						// (I  ) ｶｯﾄ線 + 方向点

	// レイヤ・ブロック数を得る
	MINT GetLayBlkNum          ();														// (  O) ﾚｲﾔ･ﾌﾞﾛｯｸ数

	// レイヤ・ブロックの検索
	MINT GetLayBlkInf          (														// (  O) ｽﾃｰﾀｽ  0:正常  1:ｴﾗｰ
								MINT				Idx,								// (I  ) ｲﾝﾃﾞｯｸｽ
								HCHAR*				Name = NULL,						// (  O) ﾚｲﾔ･ﾌﾞﾛｯｸ名
								COLORREF*			Rgb  = NULL);						// (  O) RGBｶﾗｰ値

	// レイヤ・ブロックにＲＧＢを設定する
	MINT SetLayBlkRgb          (														// (  O) ｽﾃｰﾀｽ  0:正常  1:ｴﾗｰ
								HCHAR*				Name,								// (I  ) ﾚｲﾔ･ﾌﾞﾛｯｸ名
								COLORREF			Rgb);								// (I  ) RGBｶﾗｰ値

	MINT SetDirectX            (														// (  O) ｽﾃｰﾀｽ  0:正常  1:ｴﾗｰ
								MINT				DMode,								// (I  ) 0;線 1:面 2:両方
								LPDIRECT3DDEVICE8	LpD3DDEV,							// (I  ) DirectXﾃﾞﾊﾞｲｽ
								HstackTexture*		TextList,							// (I  ) ﾃｷｽﾁｬﾘｽﾄ
								HstackShowObj*		ShowList,							// (I  ) 表示対象ﾘｽﾄ
								HCHAR				TexNam[50][64] = NULL,				// (I  ) ﾃｷｽﾁｬ名
								COLORREF*			RgbInf = NULL,						// (I  ) 表示色
								MINT				ArrFlg = 0,							// (I  ) 矢印表示ﾌﾗｸﾞ  0:なし 1:あり
								MINT				Symtry = 0,							// (I  ) 対称ﾌﾗｸﾞ  0:なし  1:あり
								HMTRX3				Matrix = NULL,						// (I  ) 変換ﾏﾄﾘｸｽ
								HPOINT3*			Move   = NULL,						// (I  ) 移動量
								HMINMAX3*			MinMax = NULL);						// (  O) 最小･最大

	// ＤＸＦ情報をＤｉｒｅｃｔＸで表示する
	HVOID ShowDirectX          (
								MINT				DMode,								// (I  ) 0;線 1:面 2:両方
								LPDIRECT3DDEVICE8	LpD3DDEV,							// (I  ) DirectXﾃﾞﾊﾞｲｽ
								LPDIRECT3DTEXTURE8	Texture = NULL);					// (I  ) ﾃｷｽﾁｬ

	// ＤＸＦ情報をＤｉｒｅｃｔＸから開放する
	MINT RelDirectX            (
								MINT				DMode);								// (I  ) 0;線 1:面 2:両方

	// 矢印をＤｉｒｅｃｔＸに設定する
	MINT SetDirectXY           (														// (  O) ｽﾃｰﾀｽ  0:正常  1:ｴﾗｰ
								LPDIRECT3DDEVICE8		LpD3DDEV);						// (I  ) DirectXﾃﾞﾊﾞｲｽ

	//  矢印をＤｉｒｅｃｔＸで表示する
	HVOID ShowDirectXY         (
								LPDIRECT3DDEVICE8		LpD3DDEV);						// (I  ) DirectXﾃﾞﾊﾞｲｽ

	// 矢印をＤｉｒｅｃｔＸから開放する
	HVOID RelDirectXY          ();


private:

};


/******************************************************************************************************************
[機能] ＤＸＦファイル読込み
[返値] ＤＸＦ情報
******************************************************************************************************************/
HsDxfInf* HsDxfRead            (														// (  O) DXF情報
								HCHAR*				FlName,								// (I  ) ﾌｧｲﾙ名
								MINT				VewTyp,								// (I  ) ﾋﾞｭｰ種類
								MINT				ZukVec = 0,							// (I  ) 1:上下  2:左右  4:-90°  5:+90°
																						//       6:X移動 7:Y移動 8:Z移動
								HREAL				MVal   = 0.f,						// (I  ) 移動量
								MINT				FAFlag = 0);						// (I  ) 面の向き調整   0:しない  1:する

/******************************************************************************************************************
[機能] ヘッダセクションを読み込み、円弧の回転方向データを変数に設定する
[返値] = 0 正常  < 0エラー
******************************************************************************************************************/
MINT HsDxfGetHdSec             (														// (  O) ｽﾃｰﾀｽ
								CStdioFile*			FLPtr,								// (I  ) ﾌｧｲﾙ識別子
								HsDxfInf*			DxfInf);							// (I/O) DXF情報

/******************************************************************************************************************
[機能] ブロックセクションを読込み、ブロックテーブルを作成する
[返値] = 0 正常  < 0エラー
******************************************************************************************************************/
MINT HsDxfGetBlcSec            (														// (   O) ｽﾃｰﾀｽ
								CStdioFile*			FLPtr,								// (I  ) ﾌｧｲﾙ識別子
								HsDxfInf*			DxfInf);							// (I/O) DXF情報

/******************************************************************************************************************
[機能] エンティティセクションを読込み、プリミティブに展開する
[返値] = 0 正常  < 0エラー
******************************************************************************************************************/
MINT HsDxfGetEntSec            (														//(  O)終了ステータス
								CStdioFile*			FLPtr,								// (I  ) ﾌｧｲﾙ識別子
								HsDxfInf*			DxfInf);							// (I/O) DXF情報

/******************************************************************************************************************
[機能] クラスセクションを読み飛ばす
[返値] = 0 正常  < 0エラー  > 0 警告 非表示項目である
******************************************************************************************************************/
MINT HsDxfGetClsSec            (														// (  O) ｽﾃｰﾀｽ
								CStdioFile*			FLPtr);								// (I  ) ﾌｧｲﾙ識別子

/******************************************************************************************************************
[機能] オブジェクトセクションを読み飛ばす
[返値] = 0 正常  < 0エラー  > 0 警告 非表示項目である
******************************************************************************************************************/
MINT HsDxfGetObjSec            (														// (  O) ｽﾃｰﾀｽ
								CStdioFile*			FLPtr);								// (I  ) ﾌｧｲﾙ識別子

/******************************************************************************************************************
[機能] フォントテーブルの読込みと設定
[返値] = 0 正常  < 0エラー
******************************************************************************************************************/
MINT HsDxfGetTblFont           (														// (  O) ｽﾃｰﾀｽ
								CStdioFile*			FLPtr,								// (I  ) ﾌｧｲﾙ識別子
								HsDxfInf*			DxfInf);							// (I/O) DXF情報

/******************************************************************************************************************
[機能] 線種テーブルの読込みと設定
[返値] = 0 正常  < 0エラー
******************************************************************************************************************/
MINT HsDxfGetTblLin            (														// (  O) ｽﾃｰﾀｽ
								CStdioFile*			FLPtr,								// (I  ) ﾌｧｲﾙ識別子
								HsDxfInf*			DxfInf);							// (I/O) DXF情報

/******************************************************************************************************************
[機能] レイヤテーブルの読込みと設定
[返値] = 0 正常  < 0エラー
******************************************************************************************************************/
MINT HsDxfGetTblLyr            (														// (  O) ｽﾃｰﾀｽ
								CStdioFile*			FLPtr,								// (I  ) ﾌｧｲﾙ識別子
								HsDxfInf*			DxfInf);							// (I/O) DXF情報

/******************************************************************************************************************
[機能] テーブルセクションを読込む。対象となるテーブルは,LTYPE(線種),STYLE(文字ｽﾀｲﾙ),LAYER(画層)
[返値] = 0 正常  < 0エラー
******************************************************************************************************************/
MINT HsDxfGetTblSec            (														// (   O) ｽﾃｰﾀｽ
								CStdioFile*			FLPtr,								// (I  ) ﾌｧｲﾙ識別子
								HsDxfInf*			DxfInf);								// (I/O) DXF情報

/******************************************************************************************************************
[機能] 円弧情報を読み込み、レイヤもしくはブロック情報のプリミティブリストへ追加する
[返値] = 0 正常  < 0エラー  > 0 警告 非表示項目である
******************************************************************************************************************/
MINT HsDxfGetPrmArc            (														// (  O) ｽﾃｰﾀｽ
								CStdioFile*			FLPtr,								// (I  ) ﾌｧｲﾙ識別子
								HCHAR*				Data,								// (  O) 読込み領域
								HsDxfInf*			DxfInf,								// (I/O) DXF情報
								HsDxfBlock*			Block);								// (I  ) ﾌﾞﾛｯｸ
																						//       == NULL ﾚｲﾔへ読込む
																						//       != NULL 指定ﾌﾞﾛｯｸへ読込む

/******************************************************************************************************************
[機能] 文字情報を読込み、レイヤもしくはブロック情報のプリミティブリストへ追加する
[返値] = 0 正常  < 0エラー  > 0 警告 非表示項目である
******************************************************************************************************************/
MINT HsDxfGetPrmAtb            (														// (  O) ｽﾃｰﾀｽ
								CStdioFile*			FLPtr,								// (I  ) ﾌｧｲﾙ識別子
								HCHAR*				Data,								// (  O) 読込み領域
								HsDxfInf*			DxfInf,								// (I/O) DXF情報
								HsDxfBlock*			Block);								// (I  ) ﾌﾞﾛｯｸ
																						//       == NULL ﾚｲﾔへ読込む
																						//       != NULL 指定ﾌﾞﾛｯｸへ読込む

/******************************************************************************************************************
[機能] 円情報を読込み、レイヤもしくはブロック情報のプリミティブリストへ追加する
[返値] = 0 正常  < 0エラー  > 0 警告 非表示項目である
******************************************************************************************************************/
MINT HsDxfGetPrmCir            (														// (  O) ｽﾃｰﾀｽ
								CStdioFile*			FLPtr,								// (I  ) ﾌｧｲﾙ識別子
								HCHAR*				Data,								// (  O) 読込み領域
								HsDxfInf*			DxfInf,								// (I/O) DXF情報
								HsDxfBlock*			Block);								// (I  ) ﾌﾞﾛｯｸ
																						//       == NULL ﾚｲﾔへ読込む
																						//       != NULL 指定ﾌﾞﾛｯｸへ読込む

/******************************************************************************************************************
[機能] 点情報を読込み、レイヤもしくはブロック情報のプリミティブリストへ追加する
[返値] = 0 正常  < 0エラー  > 0 警告 非表示項目である
******************************************************************************************************************/
MINT HsDxfGetPrmDot            (														// (  O) ｽﾃｰﾀｽ
								CStdioFile*			FLPtr,								// (I  ) ﾌｧｲﾙ識別子
								HCHAR*				Data,								// (  O) 読込み領域
								HsDxfInf*			DxfInf,								// (I/O) DXF情報
								HsDxfBlock*			Block);								// (I  ) ﾌﾞﾛｯｸ
																						//       == NULL ﾚｲﾔへ読込む
																						//       != NULL 指定ﾌﾞﾛｯｸへ読込む

/******************************************************************************************************************
[機能] 面情報を読込み、レイヤもしくはブロック情報のプリミティブリストへ追加する
[返値] = 0 正常  < 0エラー  > 0 警告 非表示項目である
******************************************************************************************************************/
MINT HsDxfGetPrmFac            (														// (  O) ｽﾃｰﾀｽ
								CStdioFile*			FLPtr,								// (I  ) ﾌｧｲﾙ識別子
								HCHAR*				Data,								// (  O) 読込み領域
								HsDxfInf*			DxfInf,								// (I/O) DXF情報
								HsDxfBlock*			Block);								// (I  ) ﾌﾞﾛｯｸ
																						//       == NULL ﾚｲﾔへ読込む
																						//       != NULL 指定ﾌﾞﾛｯｸへ読込む

/******************************************************************************************************************
[機能] ブロックを挿入する
[返値] = 0 正常  < 0エラー  > 0 警告 非表示項目である
******************************************************************************************************************/
MINT HsDxfGetPrmIns            (														// (  O) ｽﾃｰﾀｽ
								CStdioFile*			FLPtr,								// (I  ) ﾌｧｲﾙ識別子
								HCHAR*				Data,								// (  O) 読込み領域
								HsDxfInf*			DxfInf,								// (I/O) DXF情報
								HsDxfBlock*			Block);								// (I  ) ﾌﾞﾛｯｸ
																					//       == NULL ﾚｲﾔへ読込む
																					//       != NULL 指定ﾌﾞﾛｯｸへ読込む

/******************************************************************************************************************
[機能] 線情報を読み込み、レイヤもしくはブロック情報のプリミティブリストへ追加する
[返値] = 0 正常  < 0エラー  > 0 警告 非表示項目である
******************************************************************************************************************/
MINT HsDxfGetPrmLin            (														// (  O) ｽﾃｰﾀｽ
								CStdioFile*			FLPtr,								// (I  ) ﾌｧｲﾙ識別子
								HCHAR*				Data,								// (  O) 読込み領域
								HsDxfInf*			DxfInf,								// (I/O) DXF情報
								HsDxfBlock*			Block);								// (I  ) ﾌﾞﾛｯｸ
																						//       == NULL ﾚｲﾔへ読込む
																						//       != NULL 指定ﾌﾞﾛｯｸへ読込む

/******************************************************************************************************************
[機能] ポリライン情報を読込み、レイヤもしくはブロック情報のプリミティブリストへ追加する
[返値] = 0 正常  < 0エラー  > 0 警告 非表示項目である
******************************************************************************************************************/
MINT HsDxfGetPrmPln            (														// (  O) ｽﾃｰﾀｽ
								CStdioFile*			FLPtr,								// (I  ) ﾌｧｲﾙ識別子
								HCHAR*				Data,								// (  O) 読込み領域
								HsDxfInf*			DxfInf,								// (I/O) DXF情報
								HsDxfBlock*			Block);								// (I  ) ﾌﾞﾛｯｸ
																						//       == NULL ﾚｲﾔへ読込む
																						//       != NULL 指定ﾌﾞﾛｯｸへ読込む

/******************************************************************************************************************
[機能] 文字情報を読込み、レイヤもしくはブロック情報のプリミティブリストへ追加する
[返値] = 0 正常  < 0エラー  > 0 警告 非表示項目である
******************************************************************************************************************/
MINT HsDxfGetPrmTxt            (														// (  O) ｽﾃｰﾀｽ
								CStdioFile*			FLPtr,								// (I  ) ﾌｧｲﾙ識別子
								HCHAR*				Data,								// (  O) 読込み領域
								HsDxfInf*			DxfInf,								// (I/O) DXF情報
								HsDxfBlock*			Block);								// (I  ) ﾌﾞﾛｯｸ
																						//       == NULL ﾚｲﾔへ読込む
																						//       != NULL 指定ﾌﾞﾛｯｸへ読込む

/******************************************************************************************************************
[機能] ポリライン上の２点と膨らみから、円弧情報を作成する
[返値] = 0 正常  < 0エラー
******************************************************************************************************************/
MINT HsDxfPlnToArc             (
								HPOINT3*			SPnt,								// (I  ) 開始点
								HPOINT3*			EPnt,								// (I  ) 終了点
								HREAL				Tan,								// (I  ) 膨らみ
								HARC3S*				Arc);								// (  O) 円弧情報

/******************************************************************************************************************
[機能] 始点終点と中心角度から円弧の中心と半径を求める  （２次元図形を扱い２点のZ座標が0.0でない場合はエラーとする）
[返値] = 0 正常  < 0エラー 
******************************************************************************************************************/
MINT HsDxfCalArcAtr            (
								HPOINT3*			Pnt1,								// (I  ) １点目
								HPOINT3*			Pnt2,								// (I  ) ２点目
								HREAL				Ang,								// (I  ) 中心角度
								HPOINT3*			Center,								// (  O) 中心点
								HREAL*				Radius);							// (  O) 半径

/******************************************************************************************************************
[機能] 円弧の点の角度を求める  （円弧はXY平面（Z=0)上にのっているものとする）
[返値] = 0 正常  < 0エラー 
******************************************************************************************************************/
HREAL HsDxfGetAngle            (														// (  O) 角度
								HPOINT3*			Center,								// (I  ) 円弧中心
								HPOINT3*			Pnt);								// (I  ) 円弧上の点

/******************************************************************************************************************
[機能] ＤＸＦファイルより２ライン読込み編集する （コメント行は読み飛ばす）
[返値] = 0 正常  < 0エラー
******************************************************************************************************************/
MINT HsDxfGetLine              (														// (  O) ｽﾃｰﾀｽ
								CStdioFile*			FLPtr,								// (I  ) ﾌｧｲﾙ識別子
								MINT*				Code,								// (  O) ｸﾞﾙｰﾌﾟｺｰﾄﾞ
								HCHAR*				Data);								// (  O) 文字列

/******************************************************************************************************************
[機能] 色番号から色を得る
[返値] = 0 正常  < 0エラー
******************************************************************************************************************/
COLORREF HsDxfGetCol           (														// (  O) RGBｶﾗｰ値
								MINT				ColNo);								// (I  ) 色番号

HVOID    HsDxfGetCol           (														// (  O) RGBｶﾗｰ値
								MINT				ColNo,								// (I  ) 色番号
								MINT*				Red,								// (  O) 赤
								MINT*				Green,								// (  O) 緑
								MINT*				Blue);								// (  O) 青


#endif			//  _HSDXF_

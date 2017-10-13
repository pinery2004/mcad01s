#pragma once

#ifdef DLL_EXPORT
	#undef DLL_EXPORT
#endif

#ifdef DLL_EXPORT_HAITICMD_DO
	#pragma message( "<<< HaitiDb・dll_EXport >>>")
	#define DLL_EXPORT			__declspec( dllexport)
#else
	#ifdef _DEBUG								// Release Mode で inline関数がundefになる不具合対応
		#ifdef DLL_NO_IMPORT_MD_DO
			#define DLL_EXPORT
		#else
//			#pragma message( "=== HaitiDb・dll_IMport ===")
			#define DLL_EXPORT	__declspec( dllimport)
		#endif
	#else
		#define DLL_EXPORT
	#endif
#endif

namespace MC
{

class MgPolyg2;
class MgGInt;

class DLL_EXPORT HaitiCmd
{
public:
/////////////////////////////////////////////////////////////////////////////
//	部材表確認変更入力
static void MmPtsEditInp(
				CWnd*		pWnd					// (I  ) ウィンドウのインスタンス
				);

/////////////////////////////////////////////////////////////////////////////
//	部材表確認変更実行
//ZZ	void MmPtsEditXqt();

/////////////////////////////////////////////////////////////////////////////
//	部材表確認変更キャンセル
//ZZ	void MmPtsEditCancel();

/////////////////////////////////////////////////////////////////////////////
//	部材表変更終了
static void MmPtsEditEnd();

/////////////////////////////////////////////////////////////////////////////
//	部品を配置する
static void MmPtsPlc(
				const	MgPoint3	*Pt,			// (I  ) 配置点、配置方向点
				const	MgVect3		vUp = MgVect3( 0., 0., 1.),	// (I  ) 左方向
				const	MgPolyg2	*pPg = NULL		// (I  ) 図形用の区画　または NULL
				);

/////////////////////////////////////////////////////////////////////////////
//	屋根を配置する
static MINT RoofPlc(										// (  O) ステイタス	0: 正常、-1: 屋根作成エラー
				const	MgPolyg2	&Pg,			// (I  ) 地廻り区画
				const	MgGInt		&GifInp,		// (I  ) 入力フラグ(1:寄棟 5:切妻)
				const	MgPoint2	&Pth			// (I  ) 方向点
				);

/////////////////////////////////////////////////////////////////////////////
//	ＵＮＤＯ
static void MmUndo();
static bool MmUndoIsNotEmpty();

/////////////////////////////////////////////////////////////////////////////
//	ＲＥＤＯ
static void MmRedo();
static bool MmRedoIsNotEmpty();

/////////////////////////////////////////////////////////////////////////////
//	イニシャライズＵＮＤＯ保存領域
static void MdClearRedo();

////////////////////////////////////////////////////////////////////////////
//	コマンド実行の事前準備
static void MmPresetCmd();

};

} // namespace MC
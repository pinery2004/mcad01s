#pragma once

#include "MgMat.h"

#ifdef DLL_EXPORT
	#undef DLL_EXPORT
#endif

#ifdef DLL_EXPORT_TENKAI_DO
	#pragma message( "<<< Tenkai・dll_EXport >>>")
	#define DLL_EXPORT							__declspec( dllexport)
#else
	#ifdef _DEBUG								// Release Mode で inline関数がundefになる不具合対応
		#ifdef DLL_NO_IMPORT_MD_DO
			#define DLL_EXPORT
		#else
//			#pragma message( "=== Tenkai・dll_IMport ===")
			#define DLL_EXPORT						__declspec( dllimport)
		#endif
	#else
		#define DLL_EXPORT
	#endif
#endif

namespace MC
{

class	MmWndInfo;
class	MhMdlIO;

#define		MX_NJUKO	8

/////////////////////////////////////////////////////////////////////////////
//	住棟展開
class JTTenkaiPr
{
public:
	MUCHAR	nJuko;								// 住戸数
	MUCHAR	ifH[MX_NJUKO];						// 反転フラグ	0: 正展開、1: 反転展開
	MUCHAR	nOJuko[MX_NJUKO];					// 展開元住戸
	MREAL	rMDJuko[MX_NJUKO];					// 縦方向オフセット
	MREAL	rDJuko[MX_NJUKO];					// 住戸奥行
	MREAL	rWJuko[MX_NJUKO];					// 住戸巾

	MINT	Load(												// ＤＢより１レコード読み込み
						MhMdlIO*	i_phMdl						// 読み込みファイル　ハンドル
				);
	MINT	Save(												// ＤＢへ１レコード書き込み
						MhMdlIO*	i_phMdl						// 書き込みファイル　ハンドル
				);
};

class DLL_EXPORT JTTenkai {
public:
	static	JTTenkaiPr 	z_TenkaiPara;			// 住棟展開パラメータ
	static	MgMat2		z_matTenkai[MX_NJUKO];	// 住棟展開マトリックス
	static	MgMat2		z_matRTenkai[MX_NJUKO];	// 住棟展開逆マトリックス
	static	MgMinMaxR2	z_mmTenkai[MX_NJUKO];	// 住戸領域ＭｉｎＭａｘ
	static	MINT		z_iJuko;				// 選択住戸
	static	MUINT		z_iTenkai[MX_NJUKO];	// 住棟展開制御

public:
	/////////////////////////////////////////////////////////////////////////////
	//【機能】修正住棟展開情報を初期化する
	static void InitJTT();

	/////////////////////////////////////////////////////////////////////////////
	//	住棟展開入力
	static void InpJTT(
					CWnd*		pWnd					// (I  ) ウィンドウのインスタンス
					);

	/////////////////////////////////////////////////////////////////////////////
	//	住棟展開終了
	static void EndJTT();


	/////////////////////////////////////////////////////////////////////////////
	//	住棟展開マトリックスを設定する
	static void SetJTTMAT();

	/////////////////////////////////////////////////////////////////////////////
	//	住棟展開パラメータを取得する
	static JTTenkaiPr* GetJTTPara();					// (  O) 住棟展開パラメータ

	/////////////////////////////////////////////////////////////////////////////
	//	住棟展開パラメータを設定する
	static void SetJTTPara(
							JTTenkaiPr* pAuxTenkai		// (I  ) 住棟展開パラメータ
					);

	/////////////////////////////////////////////////////////////////////////////
	//	住戸選択する
	static void SetJTTJuuko(
							MINT iJuko					// (I  ) 選択住戸番号　または　0
					);

	/////////////////////////////////////////////////////////////////////////////
	//	住戸区画の創成
	static void CreJTTArea();
};

} // namespace MC
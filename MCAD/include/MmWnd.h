#pragma once
/////////////////////////////////////////////////////////////////////////////**************
//  ウィンドウ管理

#include "MbCod.h"
#include "MmGridNum.h"
#include "MmDrag.h"


// 図種類
#define			MTSIKITI			100			// 		100:敷地
#define			MTHEIMENZU			101			// 		101:平面図
#define			MTRITUMENZU			102			// 		102:立面図
#define			MTSEKKEI3DZU		103			// 		103:３次元図
#define			MTKOUZOU3DZU		104			// 		104:構造３次元図
#define			MTKISOFUSEZU		200			// 		200:基礎伏図
#define			MTDODAIFUSEZU		210			// 		210:土台伏図
#define			MTYUKAFUSEZU		220			//		220:床伏図
#define			MTKABEFUSEZU		230			// 		230:壁伏図 
#define			MTTENJOFUSEZU		240			// 		240:天井伏図
#define			MTYANEFUSEZU		250			// 		250:屋根伏図
#define			MTYUKAPANERUZU		320			//		320:床パネル
#define			MTKABEPANERUZU		330			//		330:壁パネル
#define			MTTENJOPANERUZU		340			//		340:天井パネル
#define			MTYANEPANERUZU		350			//		350:屋根パネル

// シフトキーとコントロールキーの両方が押されている事を確認
#define CONTROL_SHIFT_FLAGS(n)  ((n & (MK_SHIFT|MK_CONTROL)) == (MK_SHIFT|MK_CONTROL))

// コントロールキーが押されている事を確認
#define CONTROL_FLAGS(n)  ((n & MK_CONTROL) == MK_CONTROL)

// シフトキーが押されている事を確認
#define SHIFT_FLAGS(n)  ((n & MK_SHIFT) == MK_SHIFT)

#ifdef DLL_EXPORT
	#undef DLL_EXPORT
#endif

#ifdef DLL_EXPORT_WNDINFO_DO
	#pragma message( "<<< WndInfo・dll_EXport >>>")
	#define DLL_EXPORT							__declspec( dllexport)
#else
	#ifdef _DEBUG								// Release Mode で inline関数がundefになる不具合対応
		#ifdef DLL_NO_IMPORT_MD_DO
			#define DLL_EXPORT
		#else
//			#pragma message( "=== WndInfo・dll_IMport ===")
			#define DLL_EXPORT						__declspec( dllimport)
		#endif
	#else
		#define DLL_EXPORT
	#endif
#endif

namespace MC
{

class MmWndInfo;

/////////////////////////////////////////////////////////////////////////////
//	ウィンドウ管理情報 

//class	MmWndInfo: public MOBJECT, public MmGridNum, public MbCod
class DLL_EXPORT MmWndInfo: public MOBJECT, public MmGridNum, public MbCod
{
public :
	MINT			m_iWndSyu;					// ウィンドウ種類(1,2,3,4,5)
	MINT			m_iZuSyu;					// 図種類
												// 		100:敷地
												// 		101:平面図
												// 		102:立面図
												// 		103:３次元図
												// 		104:構造３次元図
												// 		200:基礎伏図
												// 		210:土台伏図
												//		220:床伏図
												// 		230:壁伏図 
												// 		240:天井伏図
												// 		250:屋根伏図
												//		320:床パネル
												//		330:壁パネル
												//		340:天井パネル
												//		350:屋根パネル
	MINT			m_iKai;						// 階      (0:不使用 -2～5階)
	MINT			m_iZuNum;					// 図番号　(0:不使用｜図種毎追番｜パネル番号）
	MINT			m_iHoko;					// 方向　　0:不使用　1:北 2:西 3:南 4:東
	CWnd*			m_pWnd;						// ウィンドウのインスタンス
	CMDIChildWnd*	m_pChildFrm;				// フレームのインスタンス
	
	MINT			m_iCursor;					// カレントカーソル番号
	
	MmDrag*			m_pDrag;					// ドラッギング操作情報

	CBitmap*		m_pMemBmp;					// ウィンドウイメージビットマップ
	CBitmap*		m_pOldMemBmp;				// 保存用ウィンドウイメージビットマップ
	CDC*			m_pMemDC;					// ウィンドウイメージメモリＤＣ

	// コンストラクタ
	MmWndInfo()		{};

	MmWndInfo(
						MINT		iWndSyu,		// (I  ) ウィンドウ種類(1,2,3,4,5)
						MINT		iZuSYu,			// (I  ) 図種類
						MINT		iKai,			// (I  ) 階
						MINT		iZuNum			// (I  ) 図番号
				);

	// デストラクタ
	~MmWndInfo();

	// 属性読み書き
	void	SetWndSyu( const MINT iWndSyu)
						{ m_iWndSyu = iWndSyu;}
	MINT	GetWndSyu()
						{ return m_iWndSyu;}
	void	SetZuSyu( const MINT iZuSyu)
						{ m_iZuSyu = iZuSyu;}
	MINT	GetZuSyu()
						{ return m_iZuSyu;}
	void	SetKai( const MINT iKai)
						{ m_iKai = iKai;}
	MINT	GetKai()
						{ return m_iKai;}
	void	SetZuNum( const MINT iZuNum)
						{ m_iZuNum = iZuNum;}
	MINT	GetZuNum()
						{ return m_iZuNum;}
	void	SetWnd( CWnd* pWnd)
						{ m_pWnd = pWnd;}
	CWnd*	GetWnd()
						{ return m_pWnd;}
	void	SetFrame( CMDIChildWnd* pChildFrm)
						{ m_pChildFrm = pChildFrm;}
	CMDIChildWnd* GetFrame()
						{ return m_pChildFrm;}
	MINT	SetCursor( const MINT iCursor)
						{ MINT iPrevCursor = m_iCursor;
						  m_iCursor = iCursor;
						  return iPrevCursor;}
	MINT	GetCursor()
						{ return m_iCursor;}
	void	SetDrag( MmDrag* pDrag)
						{ m_pDrag = pDrag;}
	MmDrag*	GetDrag()
						{ return m_pDrag;}
	void	DeleteDrag();

//	void	SetMemBmp( CBitmap*	pMemBmp)
//						{ m_pMemBmp = pMemBmp;}
//	CBitmap* GetMemBmp()
//						{ return m_pMemBmp;}
//	void	SetOldMemBmp( CBitmap*	pOldMemBmp)
//						{ m_pOldMemBmp = pOldMemBmp;}
//	CBitmap* GetOldMemBmp()
//						{ return m_pOldMemBmp;}

	void	SetMemDC( CDC*	pMemDC)
						{ m_pMemDC = pMemDC;}
	CDC*	GetMemDC()
						{ return m_pMemDC;}

	void	CreateMDC();
	void	DeleteMDC();
	void	AsignMDC();
	void	FreeMDC();
	void 	ClearMDC();
};

#ifdef DLL_EXPORT
	#undef DLL_EXPORT
#endif

#ifdef DLL_EXPORT_MC_WINDOWCTRL_DO
	#pragma message( "<<< MC::WindowCtrl・dll_EXport >>>")
	#define DLL_EXPORT							__declspec( dllexport)
#else
	#ifdef DLL_NO_IMPORT_MD_DO
		#define DLL_EXPORT
	#else
//		#pragma message( "=== MC::WindowCtrl・dll_IMport ===")
		#define DLL_EXPORT						__declspec( dllimport)
	#endif
#endif

class DLL_EXPORT WindowCtrl
{
public:
	/////////////////////////////////////////////////////////////////////////////
	//  ウィンドウ管理

	/////////////////////////////////////////////////////////////////////////////
	//	グリッド数変更入力
	static void MmGridNumInp(
					CWnd*		pWnd					// (I  ) ウィンドウのインスタンス
					);

	/////////////////////////////////////////////////////////////////////////////
	//	グリッド数設定	（シングルグリッド用）
	static void MmGridNumSet(
					MINT*		nGrid					// (I  ) グリッド数（北、西、南、東）
					);

	/////////////////////////////////////////////////////////////////////////////
	//	グリッド数読み込み	（シングルグリッド用）
	static void MmGridNumGet(
					MINT*		nGrid					// (  O) グリッド数（北、西、南、東）
					);

	/////////////////////////////////////////////////////////////////////////////
	//	グリッド数変更表示実行
	static void MmGridNumXqt();

	/////////////////////////////////////////////////////////////////////////////
	//	グリッド数変更キャンセル
	static void MmGridNumCancel();

	/////////////////////////////////////////////////////////////////////////////
	//	グリッド数変更終了
	static void MmGridNumEnd();

public:
	/////////////////////////////////////////////////////////////////////////////
	//	先頭のウィンドウを取得
	//	返値 =NULL: レコードなし !=NULL: 読み込み[部品配置]ポインタ
	static MmWndInfo* MmGetHeadWnd(
							MPOSITION	*pWndPos = NULL
					);

	/////////////////////////////////////////////////////////////////////////////
	//	次のウィンドウを取得
	//	返値 =NULL: レコードなし !=NULL: 読み込み[部品配置]ポインタ
	static MmWndInfo* MmGetNextWnd(
							MPOSITION	*pWndPos = NULL
					);

	/////////////////////////////////////////////////////////////////////////////
	//	カレントのウィンドウ種別を設定

	static void	MmWndSetSyubetsuC(
							MINT		iWndSyu,		// (I  ) ウィンドウ種類
							MINT		iZuSyu,			// (I  ) 図種類
							MINT		iKai,			// (I  ) 階
							MINT		iZuNum			// (I  ) 図番号
					);

	/////////////////////////////////////////////////////////////////////////////
	//	カレントウィンドウのフレームのインスタンスを設定

	static void	MmWndSetFrameC(
							MINT		iWndSyu,		// (I  ) ウィンドウ種類
							CMDIChildWnd* pChildFrm		// (I  ) フレームのインスタンス
					);

	/////////////////////////////////////////////////////////////////////////////
	//	カレントウィンドウのウィンドウのインスタンスを設定

	static void	MmWndSetWndC(
							MINT		iWndSyu,		// (I  ) ウィンドウ種類
							CWnd*		m_pWnd			// (I  ) ウィンドウのインスタンス
					);

	/////////////////////////////////////////////////////////////////////////////
	//	コマンドメニュー処理
	static void MmWndKCmdXqt(
							MINT		i_idCmd			// コマンドID
					);

	/////////////////////////////////////////////////////////////////////////////
	//  カレントウィンドウを追加する

	static MINT MmWndKAddC();									// (  O) ステイタス 0:正常  1:エラー

	/////////////////////////////////////////////////////////////////////////////
	//  ウィンドウを追加する

	static MINT MmWndKAdd(										// (  O) ステイタス 0:正常  1:エラー
							MINT		iWndSyu,		// (I  ) ウィンドウ種類(1,2,3,4,5)
							MINT		iZuSYu,			// (I  ) 図種類
							MINT		iKai,			// (I  ) 階
							MINT		iZuNum			// (I  ) 図番号
					);

	/////////////////////////////////////////////////////////////////////////////
	//  ウィンドウを検索する

	static MmWndInfo* MmWndKFindSyu(							// (  O) ウィンドウ管理sg情報のポインタ
							MINT		iWndSyu,		// (I  ) ウィンドウ種類(1,2,3,4,5)
							MINT		iZuSYu,			// (I  ) 図種類
							MINT		iKai,			// (I  ) 階
							MINT		iZuNum			// (I  ) 図番号
					);

	static MmWndInfo* WindowCtrl::MmWndKFindWnd(							// (  O) ウィンドウ管理情報のポインタ
							CWnd*		m_pWnd			// (I  ) ウィンドウのインスタンス
					);

	static MmWndInfo* MmWndKFindFrm(							// (  O) ウィンドウ情報へのﾎﾟｲﾝﾀ
							CMDIChildWnd* pChildFrm		// (I  ) フレームのインスタンス
					);

	/////////////////////////////////////////////////////////////////////////////
	//	ウィンドウを削除する
	/*
	static MINT MmWndKDelete(									// (  O) ステイタス 0:正常  1:エラー
					MINT		iWndSyu,				// (I  ) ウィンドウ種類(1,2,3,4,5)
					MINT		iZuSYu,					// (I  ) 図種類
					MINT		iKai,					// (I  ) 階
					MINT		iZuNum					// (I  ) 図番号
					);

	static MINT MmWndKDelete(									// (  O) ステイタス 0:正常  1:エラー
					CWnd*		pWnd					// (I  ) ウィンドウのインスタンス
					);
	*/
	static MINT MmWndKDelete(									// (  O) ステイタス 0:正常  1:エラー
							MmWndInfo*	pWndInfo		// (I  ) ウィンドウ管理情報
					);

	/////////////////////////////////////////////////////////////////////////////
	//	ウィンドウを開放する

	static MINT MmWndKFree(									// (  O) ステイタス 0:正常  1:エラー
							MINT		iWndSyu,		// (I  ) ウィンドウ種類(1,2,3,4,5)
							MINT		iZuSYu,			// (I  ) 図種類
							MINT		iKai,			// (I  ) 階
							MINT		iZuNum			// (I  ) 図番号
					);

	/////////////////////////////////////////////////////////////////////////////
	//	全ウィンドウを削除する

	static MINT MmWndKDeleteAll();								// (  O) ステイタス 0:正常  1:エラー

	/////////////////////////////////////////////////////////////////////////////
	//	カレントのウィンドウを取得する

	static MmWndInfo* MmWndKGetCurWnd();					// (  O) ステイタス  NULL : エラー 

	/////////////////////////////////////////////////////////////////////////////
	//  ウィンドウを再表示する（メモリーＤＣに書き込み画面に表示する）

	static MINT MmWndKReDraw();

	/////////////////////////////////////////////////////////////////////////////
	//  全ウィンドウを再表示する（メモリーＤＣに書き込み画面に表示する）

	static MINT MmWndKReDrawAll();

	/////////////////////////////////////////////////////////////////////////////
	//  ウィンドウを表示する

	static MINT MmWndKDraw(									// (  O) ステイタス 0:正常  1:エラー
							MmWndInfo*	pWndInfo		// (I  ) ウィンドウ管理情報
					);

	//=====================================================================================
	//以下未承認プログラム


	/////////////////////////////////////////////////////////////////////////////
	//  ウィンドウイメージメモリＤＣにウィンドウを表示

	static void MmWndKDrawMDC(									// (  O) ステイタス 0:正常  1:エラー
							MmWndInfo*	pWndInfo		// (I  ) ウィンドウ管理情報
					);

	/////////////////////////////////////////////////////////////////////////////
	//	グリッドを表示する

	static void MmWndKDrawGrid(
							MmWndInfo*	pWndInfo,		// (I  ) ウィンドウ管理情報
							MbCod*		pCod
					);

	/////////////////////////////////////////////////////////////////////////////
	//	壁芯線を表示する

	static void MmWndKDrawKabe(
							MbCod*		pCod,			// 座標系
							MINT		iKaiC,			// 階  	(1,2,3)
							MINT		iGpC			// 構成
					);

	/////////////////////////////////////////////////////////////////////////////
	//	基礎芯線を表示する

	static void MmWndKDrawKiso(
							MbCod*		pCod,			// 座標系
							MINT		iKaiC,			// 階  	(1,2,3)
							MINT		iGpC			// 構成
					);

	/////////////////////////////////////////////////////////////////////////////
	//	配置部品を表示する

	static void MmWndKDrawPts(
							MbCod*		pCod,			// 座標系
							MINT		iKaiC,			// 階  	(1,2,3)
							MINT		iGpC			// 構成
					);

	/////////////////////////////////////////////////////////////////////////////
	//	屋根を表示する

	static void MmWndKDrawRoof(
							MbCod*		pCod,			// 座標系
							MINT		iKaiC,			// 階  	(1,2,3)
							MINT		iGpC			// 構成
					);

	/////////////////////////////////////////////////////////////////////////////
	//	テンポラリ表示

	static void MmWndKDrawTemp(
							MmWndInfo*	pWndInfo,		// (I  ) ウィンドウ管理情報
							MbCod*		pCod
					);

	/////////////////////////////////////////////////////////////////////////////
	//  ウィンドウイメージメモリＤＣに平面図を表示

	static MINT MmWndKDrawHeimenzu(
							MmWndInfo*	pWndInfo		// (I  ) ウィンドウ管理情報
					);

	/////////////////////////////////////////////////////////////////////////////
	//  座標系の設定

	static MINT MmWndKSetCod(
							MbCod*		pCod			// (　O) 表示座標系
					);

	/////////////////////////////////////////////////////////////////////////////
	//  ＤＣに構造図を描く

	static void MmDrawStructure(								// (  O) ステイタス　0:正常  1:エラー
						MmWndInfo*	pWndInfo,			// (I  ) ウィンドウ管理情報
						MbCod*		pCod
					);
};

} // namespace MC
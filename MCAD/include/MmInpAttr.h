#pragma once

#include "MmDefine.h"

#define		MMAX_HOSEI_DBG	999
#define		MSZ_CROSS_BZI	100

namespace MC
{

#ifdef DLL_EXPORT
	#undef DLL_EXPORT
#endif

#ifdef DLL_EXPORT_MC_INPATTR_DO
	#pragma message( "<<< MC::InpAttr・dll_EXport >>>")
	#define DLL_EXPORT							__declspec( dllexport)
#else
	#ifdef DLL_NO_IMPORT_MD_DO
		#define DLL_EXPORT
	#else
//		#pragma message( "=== MC::InpAttr・dll_IMport ===")
		#define DLL_EXPORT						__declspec( dllimport)
	#endif
#endif

class DLL_EXPORT InpAttr
{
public:
	////////////////////////////////////////////////////////////////////////////
	//	部材属性入力用コンボボックスの設定

	static void SetComboPts();

	/////////////////////////////////////////////////////////////////////////////
	//	部材属性入力用コンボボックスのイニシャライズ

	static void InitComboAttr(
							MINT		i_iMode = MP_AT_NONE // 1:部材入力　2:床天井パネル 3:屋根パネル
					);

	///////////////////////////////////////////////////////////////////////////////
	//	属性値入力用コンボボックスの値を取り込む

	static void GetComboAttrA( void);

	//////////////////////////////////////////////////////////////////////////////
	//	属性値入力用コンボボックスに実数値を表示する

	static void SetComboAttrR(
							MCCMBATTR	i_iAttr,		// 属性ID
							MREAL		i_rValue		// 表示する実数値
					);

	///////////////////////////////////////////////////////////////////////////////
	//	属性値入力用コンボボックスの実数値を取得する

	static MINT GetComboAttrR(							// ステイタス0(未設定) 1(設定)
							MCCMBATTR	i_iAttr,		// 属性ID
							MREAL*		o_prValue		// 実数値
					);

	///////////////////////////////////////////////////////////////////////////////
	//	属性値入力用コンボボックスに整数値を表示する

	static void SetComboAttrI(
							MCCMBATTR	i_iAttr,		// 属性ID
							MINT		i_iValue 		// 表示する整数値
					);

	///////////////////////////////////////////////////////////////////////////////
	//	属性値入力用コンボボックスの整数を取得する

	static MINT GetComboAttrI(							// ステイタス0(未設定) 1(設定)
							MCCMBATTR	i_iAttr,		// 属性ID
							MINT*		o_piValue		// 整数値
					);

	///////////////////////////////////////////////////////////////////////////////
	//	チェックボックスのチェックマークを設定する

	static void SetCheckAttr(
							MCCHKATTR	i_iAttr,		// 属性ID
							MINT		i_iCheckAttr	// チェックマーク　0:OFF, 1:ON
					);

	///////////////////////////////////////////////////////////////////////////////
	//	チェックボックスのチェック有無を取得する
	//

	static MINT GetCheckAttr(							// ステイタス0(未設定) 1(設定)
							MCCHKATTR	i_iAttr,		// 属性ID
							MINT*		o_piCheckAttr	// チェックマーク　0:OFF, 1:ON
					);


	/////////////////////////////////////////////////////////////////////////////
	//	パネル番号コンポボックスの項目を設定する

	static void InitComboPanelNo();

	///////////////////////////////////////////////////////////////////////////////
	//	パネル番号コンポボックスにパネル番号を表示する

	static void SetComboPanelNo(
							MINT		i_iPanelNo		// パネル番号
					);

	///////////////////////////////////////////////////////////////////////////////
	//	パネル番号選択用コンポボックスよりパネル番号を取得する
	//							＝-1: 未選択
	//							＝ 0: 全
	//							≧ 1: パネル番号

	static MINT GetComboPanelNo();

	///////////////////////////////////////////////////////////////////////////////
	//	部材選択用コンポボックスの項目を設定する

	static void InitComboTpPtsAttr();

	///////////////////////////////////////////////////////////////////////////////
	//	部材選択項目番号を設定する

	static void SetComboKmIdTpPts(
							MINT		i_iKmIdTpPts	// 部材選択項目番号
					);

	///////////////////////////////////////////////////////////////////////////////
	//	部材選択用コンポボックスに部材を設定する
	//							＝-1: 該当なし
	//							≧ 0: 部材ID

	static MINT SetComboCdTpPts(
							MCHAR* 		i_pcNmPts1		// （操作用）部材名
					);

	///////////////////////////////////////////////////////////////////////////////
	//	部材選択用コンポボックスより部材IDを取得する
	//							＝-1: 該当なし
	//							≧ 0: 部材ID

	static MINT GetIdTpPts(
							MCHAR* 		i_pcNmPts1		// （操作用）部材名
					);

	///////////////////////////////////////////////////////////////////////////////
	//	部材選択用コンポボックスのカレントの部材IDを取得する
	//							＝-1: 未選択
	//							≧ 0: 部材ID

	static MINT GetCurIdTpPts();


	///////////////////////////////////////////////////////////////////////////////
	//	寸法型式選択用コンポボックスの項目を設定する

	static void InitComboPtsMbr();

	///////////////////////////////////////////////////////////////////////////////
	//	寸法型式選択項目番号を設定する

	static void SetComboKmIdMbr(
							MINT		i_iKmIdMbr		// 寸法型式選択項目番号
					);

	///////////////////////////////////////////////////////////////////////////////
	//	寸法型式選択項目番号を取得する
	//							＝-1: 該当なし
	//							≧ 0: 部材ID
	static MINT GetComboKmIdMbr( MCHAR* i_pcCdMbr);

	///////////////////////////////////////////////////////////////////////////////
	//	寸法型式選択用コンポボックスに寸法型式を設定する
	//							＝-1: 該当なし
	//							≧ 0: 寸法型式ID

	static MINT SetComboCdMbr(
							MCHAR* 		i_pcCdMbr
					);

	///////////////////////////////////////////////////////////////////////////////
	//	寸法型式IDを取得する
	//							＝-1: 該当なし
	//							≧ 0: 寸法型式ID

	static MINT GetComboCdMbr(
							MCHAR* 		i_pcCdMbr
					);

	///////////////////////////////////////////////////////////////////////////////
	//	寸法型式選択用コンポボックスより寸法型式IDを取得する
	//	（カレントの寸法型式IDを取得する）
	//							＝-1: 未選択
	//							≧ 0: 部材ID

	static MINT GetComboIdMbr();

	/////////////////////////////////////////////////////////////////////////////
	//	ダイアログ関連

	///////////////////////////////////////////////////////////////////////////////
	//	ダイアログバーの次の項目を設定する
	//		組、分類、部品タイプ、寸法型式
	//

	static MINT SetDialogBar(							// ステイタス 0:正常 -1:エラー
							MINT		i_iKumi,		// 組
							MINT		i_iBr,			// 分類
							MCHAR*		i_pcTpPts,		// 部品タイプ
							MCHAR*		i_pcMbr			// 寸法型式 または NULL
					);
};

#ifdef DLL_EXPORT
	#undef DLL_EXPORT
#endif

#ifdef DLL_EXPORT_MC_HAITIIN_DO
	#pragma message( "<<< MC::HaitiIn・dll_EXport >>>")
	#define DLL_EXPORT							__declspec( dllexport)
#else
	#ifdef DLL_NO_IMPORT_MD_DO
		#define DLL_EXPORT
	#else
//		#pragma message( "=== MC::HaitiIn・dll_IMport ===")
		#define DLL_EXPORT						__declspec( dllimport)
	#endif
#endif

class DLL_EXPORT HaitiIn
{
public:
	///////////////////////////////////////////////////////////////////////////////
	//	配置コード選択用コンボボックスに配置コードを表示し
	//	カレントの配置コードを設定する

	static void SetComboCdPlc(
							MINT		i_iCdPlc	// 配置コード
					);

	///////////////////////////////////////////////////////////////////////////////
	//	配置コード選択用コンボボックスより配置コードを取得する
	//	（カレントの配置コードを取得する）
	//							＝0 : 任意
	//							＝1 : 壁芯付き
	//							＝2 : 屋根構成線付き

	static MINT GetComboCdPlc();


	/////////////////////////////////////////////////////////////////////////////
	//	部品配置を取得する
	//	返値 =-1: オーバーフロー, ≧0: 選択部品配置数

	static MINT GetPts(
							MINT		i_iKai,			// 階
							MINT		i_iIdTpPts,		// 部品ID
					const	MCHAR*		i_pcGeneralName,// 総称 または NULL
					const	MCHAR*		i_pcNmPts1,		// 操作用部材名 または NULL
							MINT		i_szPlcEn,		// 部品配置最大数
					class	MhPlcInfo*	*o_pPlcEn,		// 選択部品配置
					class	MPOSITION*	o_pPlcpos = NULL// 選択[部品配置]位置 または NULL
					);

	/////////////////////////////////////////////////////////////////////////////
	//	部品配置を調べる
	//	返値 true : 対象部品配置, false : 対象外部品配置

	static bool ChkPts(
							MINT		i_iKai,			// 階 または NULL
							MINT		i_iIdTpPts,		// 部品ID または NULL
					const	MCHAR*		i_pcGeneralName,// 総称 または NULL
					const	MCHAR*		i_pcNmPts1,		// 操作用部材名 または NULL
					class	MhPlcInfo*	i_pPlcEn		// 調査部品配置
					);

	/////////////////////////////////////////////////////////////////////////////
	//  部材配置を検索する
	//	返値 検索結果　または　null:ヒットなし
	static MhPlcInfo* SrchBuzai(
					class	MmWndInfo*	i_pWndInfo,		// ウィンドウ管理情報
					class	MgPoint2&	i_ptMouthR,		// 検索指示座標
							MINT		i_iCdBuzai,		// 部材コード　または　NULL(全)
														// MP_BZICD_PANEL(全パネル)
					class	MgPolyg2*	o_ppgPtsShape	// 検出多角形　または　NULL
					);


	/////////////////////////////////////////////////////////////////////////////
	//  配置部品の形状を求める
	static void PtsShape(
					class	MhPlcInfo*	i_pWndInfo,		// ウィンドウ管理情報
					class	MgPolyg2*	o_pgPtsShape	// 部材の形状
					);

	/////////////////////////////////////////////////////////////////////////////
	//	１配置部品を表示する
	static void DrawPart( 
					class	MbCod*		i_pCod,			// 座標系
							MINT		i_iGpC,			// 構成
					class	MhPlcInfo*	i_pPlcEn		// 部品配置管理情報
					);

	/////////////////////////////////////////////////////////////////////////////
	//	部材の配置点と長さ補正値を修正
	static void HaitiIn::MhModBzPH(
							MINT		iInpKbCd,		// (I  ) 入力点区分コード
							MINT		iMov,			// (I  ) 修正側　(0:始点、1:終点)
					const	MgPoint3	&PtInt,			// (I  ) 配置点
							MREAL		rLH,			// (I  ) 長さ補整値
							MhPlcInfo	*pPlcEnR		// (I O) 長さ調整部材
					);

	/////////////////////////////////////////////////////////////////////////////
	//	調整先が部材で示された部材の長さ調整
	static MINT MhAdjBzL(								// (  O) ステイタス　
														//	MC_PARALLEL (-1) 交差なし（平行）
														//	MC_TWIST    (-2) 交差なし（ねじれ）
														//	MC_NINT	    (0)	交差なし
														//	MC_INT      (1)	交差あり
							MINT		iKati,			// (I  ) 勝ち負けフラグ(1:勝ち、0:負け)
							MhPlcInfo*	pPlcEn1,		// (I O) 長さ調整部材1
					const	MgPoint3	&Pt1,			// (I  ) 部材1の長さ調整する側を示す最寄の点
							MhPlcInfo*	pPlcEn2		// (I  ) 長さ調整先を示す部材2
					);

	/////////////////////////////////////////////////////////////////////////////
	//	調整先が平行部材で示された部材の長さ調整
	static void MhAdjBzL(								// (  O) ステイタス　
														//	MC_PARALLEL (-1) 交差なし（平行）
														//	MC_TWIST    (-2) 交差なし（ねじれ）
														//	MC_NINT	    (0)	交差なし
														//	MC_INT      (1)	交差あり
							MhPlcInfo*	pPlcEn1,		// (I O) 長さ調整部材1
					const	MgPoint3	&Pt1,			// (I  ) 部材1の長さ調整する側を示す最寄の点
							MhPlcInfo*	pPlcEn2,		// (I  ) 長さ調整先を示す部材2
					const	MgPoint3	&Pt2			// (I  ) 部材2の長さ調整先を示す最寄の点
					);

	/////////////////////////////////////////////////////////////////////////////
	//	調整先が点座標で示された部材の長さ調整
	static void MhAdjBzL(
							MhPlcInfo*	pPlcEn1,	// 長さ調整部材1
					const	MgPoint3	&Pt1,		// 部材1の長さ調整する側を示す最寄の点1
					const	MgPoint3	&Pt2		// 長さ調整先を示す点2
					);

	/////////////////////////////////////////////////////////////////////////////
	//	長さ調整後の部材に接触する部材と交差する部材を求める
	static void MmSrchCrossBuzai(
							MmWndInfo*	pWndInfo,		// (I  ) ウィンドウ管理情報
							MhPlcInfo*	pBziInfo1,		// (I  ) 長さ調整部材
							MINT		iBuzaiCd,		// (I  ) 部材コード
							MhPlcInfo**	pbTchBzi,		// (  O) 長さ調整後の部材に接触する部材
							MhPlcInfo**	pbCrsBzi		// (  O) 長さ調整後の部材と交差する部材
					);

	/////////////////////////////////////////////////////////////////////////////
	//	長さ調整前後で接触状態と交差状態が変化する部材を求める
	static void MhChngCrossBuzai(
							MhPlcInfo	**pbTchBziI,	// (I  ) 長さ調整前に長さ調整部材に接触する部材
							MhPlcInfo	**pbCrsBziI,	// (I  ) 長さ調整前に長さ調整部材と交差する部材
							MhPlcInfo	**pbTchBziO,	// (I  ) 長さ調整後に長さ調整部材に接触する部材
							MhPlcInfo	**pbCrsBziO,	// (I  ) 長さ調整後に長さ調整部材と交差する部材
							MhPlcInfo	**pbFRtoTCBzi,	// (  O) 長さ調整前に長さ調整部材と離れていたが
														//		 長さ調整後に接触または交差した部材
							MhPlcInfo	**pbTCtoFRBzi,	// (  O) 長さ調整前に長さ調整部材と接触または交差していたが
														//		 長さ調整後に離れた部材
							MhPlcInfo	**pbTCtoTCBzi	// (  O) 長さ調整前に長さ調整部材に接触または交差していたが
														//		 、長さ調整後にも接触または交差している部材
					);

	/////////////////////////////////////////////////////////////////////////////
	//	屋根を配置する

	static MINT RoofPlc(								// ステイタス	0: 正常、-1: 屋根配置エラー
			const	class	MgPolyg2&	i_pgJim,		// 地廻り区画
			const	class	MgGInt&		i_GifInp,		// 地廻り線種類(仮想キー(nflag)  MK_SHIFT(004): シフトキー)
			const	class	MgPoint2&	i_pth			// 方向点
					);

	/////////////////////////////////////////////////////////////////////////////
	//	屋根面を検索する
	//	返値 =NULL: 該当屋根面なし !=NULL: 該当屋根面数
	static class MhRfm*	SrchRfm(					// 屋根面　または　NULL
							MINT		i_iMode,		// 検索モード	指示点に該当する屋根面が複数ある場合の優先選択条件
														//				MC_PRI_MIN_AREA(0):	 面積が小を優先
														//				MC_PRI_MIN_HIGHT(1): 高さが小を優先
														//				MC_PRI_MAX_HIGHT(2) | その他: 高さが大を優先	
					const	MgPoint2&	i_pt1			// 指示点
					);

	////////////////////////////////////////////////////////////////////////////
	//	カレント選択の屋根面を設定する

	static void SetCurRfm(
					class	MhRfm* 		pRfm			// (I  ) カレント選択の屋根面
				);

	////////////////////////////////////////////////////////////////////////////
	//	カレント選択の屋根面を返す
	static MhRfm* GetCurRfm();

};

////////////////////////////////////////////////////////////////////////////
// Input

#define		MAXHAIKABE		500							// 配列制限値
#define		MC_KABE_TOL		44.5						// 壁判定トレランス
#define		MC_MARUME_TOL	44.5						// 丸め対象線分判定トレランス

#ifdef DLL_EXPORT
	#undef DLL_EXPORT
#endif

#ifdef DLL_EXPORT_MC_INPUT_DO
	#pragma message( "<<< MC::Input・dll_EXport >>>")
	#define DLL_EXPORT							__declspec( dllexport)
#else
	#ifdef DLL_NO_IMPORT_MD_DO
		#define DLL_EXPORT
	#else
//		#pragma message( "=== MC::Input・dll_IMport ===")
		#define DLL_EXPORT						__declspec( dllimport)
	#endif
#endif

class DLL_EXPORT Input
{
public:
	/////////////////////////////////////////////////////////////////////////////
	//	長さ２点入力

	static MINT GetLen2Pt(
							MINT		i_iMode,		// 入力モード
														//			　0: 長さ２点入力
														//			　1: 連続長さ２点の終点入力
														//					始点は前回入力(pLn)の終点とする
														//			　2: 方向１点入力
					class	MgPoint2*	o_pptln,		// 入力丸め座標（長さ２点座標 ＋　複数部材の領域をあらわす３点目）
					class	MgPoint2*	o_pptln_org		// 入力オリジナル座標（長さ２点座標 ＋　複数部材の領域をあらわす３点目） 
					);

	/////////////////////////////////////////////////////////////////////////////
	//	１点入力

	static MINT Get1Pt(
					class	MgPoint2*	o_ppt1,			// 入力丸め座標（点）
					class	MgPoint2*	o_ppt1_org		// 入力オリジナル座標（点）
					);

	/////////////////////////////////////////////////////////////////////////////
	//	連続した長さ２点入力用の２点目の入力(外壁入力用)

	static MINT GetLenEPt(
					class	MgLine2*	o_pLn
					);

	/////////////////////////////////////////////////////////////////////////////
	//	長方形区画入力

	static MINT GetRect2Pt(
					class	MgLine2*	o_pLn
					);

	/////////////////////////////////////////////////////////////////////////////
	//	区画入力

	static MINT GetArea(
					class	MgPolyg2*	o_pPg1
					);

	/////////////////////////////////////////////////////////////////////////////
	//	区画入力 
	//	各辺に　シフトキー有無のフラグ付き

	static MINT GetAreaI(
					class	MgPolyg2*	pPg1,			// (  O) 地廻り区画
					class	MgGInt*		pGifInp			// (  O) 地廻り線種類(仮想キー(nflag)  MK_SHIFT(004): シフトキー)
				);

	/////////////////////////////////////////////////////////////////////////////
	//	壁芯丸め線を取得する

	static void GetMarumeKabeLine(
							MINT		i_iKai,			// 検索階
			const	class	MgPoint2&	i_pti,			// 入力座標
					class	MgGLine2*	o_pGLn			// 対象壁芯
					);

	/////////////////////////////////////////////////////////////////////////////
	//	屋根構成線丸め線を取得する
	//
	static void MmGetMarumeYaneLine(
							MINT		iKai,			// 検索階
			const	class	MgPoint2	&Pi,			// 入力座標
					class	MgGLine2*	pGLn			// 対象屋根構成線
					);

	/////////////////////////////////////////////////////////////////////////////
	//	丸め
	static void	Marume(									// 座標値を丸める
			const	class	MgPoint2&	Pi,				// 入力座標
					class	MgPoint2*	pPo				// 丸め後の座標
				);

};

#ifdef DLL_EXPORT
	#undef DLL_EXPORT
#endif

#ifdef DLL_EXPORT_MC_INPMODE_DO
	#pragma message( "<<< MC::InpMode・dll_EXport >>>")
	#define DLL_EXPORT							__declspec( dllexport)
#else
	#ifdef DLL_NO_IMPORT_MD_DO
		#define DLL_EXPORT
	#else
//		#pragma message( "=== MC::InpMode・dll_IMport ===")
		#define DLL_EXPORT						__declspec( dllimport)
	#endif
#endif

class DLL_EXPORT InpMode
{
public:
	/////////////////////////////////////////////////////////////////////////////
	//	創成モード		削除、修正、移動、複写、属性表示

	static void SetMode(
							MINT 		i_iMode
					);
	static MINT GetMode();

	///////////////////////////////////////////////////////////////////////////////
	//	コンボボックスに入力点区分を表示し
	//	カレントの入力点区分を設定する

	static void SetComboInpKb(
							MINT		i_iInpKb		// 入力点区分
					);

	///////////////////////////////////////////////////////////////////////////////
	//	入力点区分選択用コンボボックスより入力点区分を取得する
	//	（カレントの入力点区分を取得する）
	//							＝1 : １点入力
	//							＝2 : 方向１点入力
	//							＝3 : 長さ２点入力
	//							＝4 : 区画入力
	//							＝5 : 自由入力

	static MINT GetComboInpKb();

	///////////////////////////////////////////////////////////////////////////////
	//	丸目コード選択用コンボボックスに丸目コードを表示し
	//	カレントの丸目コードを設定する

	static void SetComboCdMarume(
							MINT		i_iCdMarume		// 丸目コード
					);

	///////////////////////////////////////////////////////////////////////////////
	//	丸目コード選択用コンボボックスより丸目コードを取得する
	//	（カレントの丸目コードを取得する）
	//							＝0 : 丸めなし
	//							＝1 : １／１グリッド丸め
	//							＝2 : １／２グリッド丸め
	//							＝3 : １／３グリッド丸め
	//							＝4 : １／４グリッド丸め
	//							＝5 : １／６グリッド丸め
	//							＝5 : １／８グリッド丸め

	static MINT GetComboCdMarume();

	/////////////////////////////////////////////////////////////////////////////
	//	階				１階、２階、３階

	static void SetKai(
							MINT 		i_iKai
					);
	static MINT GetKai();

	/////////////////////////////////////////////////////////////////////////////
	// 設計入力構成		住戸、住棟、物件、屋根、敷地、壁量

	static void SetSGp(
							MINT		i_iGp
					);
	static MINT GetSGp();

	/////////////////////////////////////////////////////////////////////////////
	// 設計入力種類

	static void SetSBr(
							MINT		i_iBr
					);
	static MINT GetSBr();

	/////////////////////////////////////////////////////////////////////////////
	// 設計入力選択

	static void SetSSel(
							MINT		i_iSel
					);
	static MINT GetSSel();

	/////////////////////////////////////////////////////////////////////////////
	// 構造入力構成		屋根、小屋、天井、耐力壁、壁、床、基礎

	static void SetKGp(
							MINT		i_iGp
					);
	static MINT GetKGp();

	/////////////////////////////////////////////////////////////////////////////
	// 構造入力種類		部材、金物、パネル、その他

	static void SetKBr(
							MINT		i_iBr
					);
	static MINT GetKBr();

	/////////////////////////////////////////////////////////////////////////////
	// 構造入力選択		選択1、選択2、選択3						未使用

	static void SetKSel(
							MINT		i_iSel
					);
	static MINT GetKSel();

	/////////////////////////////////////////////////////////////////////////////
	//	部材、金物、パネル、その他のラジオボタンの選択入力許可を設定

	static void SetKBrB(
							bool		i_bBuzai,
							bool		i_bKanagu,
							bool		i_bPanel,
							bool		i_bOther
					);

	/////////////////////////////////////////////////////////////////////////////
	//	属性値入力モードを取得する

	static MINT GetAtMd();

};

}// namespace MC
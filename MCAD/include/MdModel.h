#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MdModel.h
//
//		ＭＣＡＤモデル
//
//
#include "MdAry.h"
#include "MdAryH.h"

#include "MdmDefine.h"
#include "MdmSet.h"

#include "MdmGrp.h"
#include "MdmScm.h"
#include "MdmLyr.h"
#include "MdmSetC.h"
#include "MdmEnt.h"
#include "MdmWin.h"

namespace MC
{

class MDGRPSET : public MdAryH_F_D<MdmSetGRP> 			// グループセット
{
public:
	MDGRPSET() : MdAryH_F_D<MdmSetGRP>() {};
};

class MDSCMSET : public MdAryH_F_D<MdmSetSCM> 			// スキーマセット
{
public:
	MDSCMSET() : MdAryH_F_D<MdmSetSCM>() {};
};

class MDLYRSET : public MdAryH_F_D<MdmSetLYR> 			// レイヤーセット
{
public:
	MDLYRSET() : MdAryH_F_D<MdmSetLYR>() {};
};

class MDBRISET : public MdAryH_F_D<MdmSetSCM> 			// 分類セット
{
public:
	MDBRISET() : MdAryH_F_D<MdmSetSCM>() {};
};

class MDENTSET : public MdAryH_F_D<MdmEnt>				// エンティティセット
{
public:
	MDENTSET() : MdAryH_F_D<MdmEnt>() {};
};

class MDPTSSET : public MdAryH_F_D<MdmEnt> 				// 部品セット
{
public:
	MDPTSSET() : MdAryH_F_D<MdmEnt>() {};
};

class MDWINSET : public MdAryH_F_D<MdmWin>				// ウィンドウセット
{
public:
	MDWINSET() : MdAryH_F_D<MdmWin>() {};
};

#ifdef DLL_EXPORT
	#undef DLL_EXPORT
#endif

#ifdef DLL_EXPORT_MDMODEL_DO
	#pragma message( "<<< MdModel・dll_EXport >>>")
	#define DLL_EXPORT							__declspec( dllexport)
#else
	#ifdef DLL_NO_IMPORT_MD_DO
		#define DLL_EXPORT
	#else
//		#pragma message( "=== MdModel・dll_IMport ===")
		#define DLL_EXPORT						__declspec( dllimport)
	#endif
#endif

//==========================================================================================
//	モデル
class DLL_EXPORT MdModel
{
public:
	MCHAR		m_cMdlNm[MDMSZ_NMMODEL];	// モデル名
	MDGRPSET	m_GrpBf;					// グループセット
	MDSCMSET	m_ScmBf;					// スキーマセット
	MDLYRSET	m_LyrBf;					// レイヤーセット
	MDBRISET	m_BriBf;					// 分類セット
	MDENTSET	m_EntBf;					// エンティティセット
	MDPTSSET	m_PtsBf;					// 部品セット
	MDWINSET	m_WinBf;					// ウィンドウセット

	MgMat3		m_MBaseTrans;				// 基準座標変換マトリックス
	MgMat3		m_MCurTrans;				// カレント座標変換マトリックス

	MDID		m_idCurGrp;					// カレントグループID
	MDID		m_idCurWin;					// カレントウィンドウID
	MINT		m_itpCurWin;				// カレントウィンドウタイプ ( 0:３Ｄ、1：図面)
	MDID		m_idCurLyr;					// カレントレイヤーID
	MDID		m_idCurScm;					// カレントスキーマID
	MINT		m_mdTenkai[28];				// ポリゴン展開処理モード
												//	 0: openGL対応、1:生成構造,球等図形情報をそのまま返す											
												//	 1: 面ベクトル付加　0:無し 1:付加(default)
												//	 2: 滑度　0:不変(線分) 1:変(円)  2:変えない(立体等)
												//	 3: box記録の状態 0:無記録 3:中途AJで追加 6:完
												//	 4: 滑度(粗さ付の有無) 0:無 1:有
												//	 5: 未使用
												//	 6:  頂点ベクトルの付加指示 1:付加 !=1:無
												//	 7: 属性の扱い指示  0:属性はコピーない　1:属性はコピーする
												//	 8: 円･円弧を有意点列に変換する　1:変換する(default)　2:変換しない
												//	 9: スプラインの分割方式 1:曲率による分割 2:等分割
												//	10: 面の表現  1:メッシュで出力　1以外：メッシュ出力なし
												//	11: マッピング　　未使用
												//	12: box取り出し 0:無し 1:取り出す 2:BOXのみ　参:agml2
												//	13: 面の分割に関する方式   1:面→32点以内の凸面  2:凹面→凸面  3:面→△▽面
												//	14: 未使用
												//	15: 円などの分割フラグ
												//	16: 文字の出力方法　-1:線で出力 0:面で出力 1:そのまま 2:寸法線もそのまま
												//	17: 面の出力モード 0:面で取り出す   1:線で取り出す   2:点で取り出す
												//	18: "図形のチェック方法　
												//       0:チェックなし　
												//       1:面に歪みがある場合エラー(st=5)にする 
												//       2:面に歪みがある場合,再分割する"
												//	19: 0:半径に依存する分割　実質未使用　0でしか使用していない
												//	20: 面の分割数
												//	21: 作業面図形　1：線分を得る　2：点列を得る
												//	22:  ＰＯＶ－ＲＡＹ図形出力の制御
												//	23: 作業面の細分度数(1,2,3,4,,, )　内部
												//	24:  特殊文字の変換
												//	25:  true font の間隔の制御
												//	26:  テクスチャ対応図形出力の制御　0:off　1:出力する
												//	27: あ！動く図面新設モード  0:default 1:変形操作中　2005/07/13
												//  
												//  
	MREAL	m_rmdTenkai[7];				// ポリゴン展開処理パラメータ		
										// 100: 粗さ度合いを考慮後の単位長さ　agupzm = agupix*ax[7];　内部データ
										// 101: ピクセルの大きさ
										// 102: 未使用
										// 103: 未使用
										// 104: 未使用
										// 105: 基本粗さ度合い
										// 106: 内部データ　分割数の最小値を決めるときに使用
public:
	MdModel();
	~MdModel();

	// デバッグ用トレース出力
	void Print( MCHAR* s);
};

//	トレース
inline void MdModel::Print( MCHAR* s)
{
	MBTRCPRINTS( s);
	MbLog::Print( Mstr( "◇グループセット	"), MDC_NONE_ID);
	m_ScmBf.Print( Mstr( "◇スキーマセット	"), MDC_NONE_ID);
	m_LyrBf.Print( Mstr( "◇レイヤーセット	"), MDC_NONE_ID);
	m_BriBf.Print( Mstr( "◇分類セット		"), MDC_NONE_ID);
	m_EntBf.Print( Mstr( "◇エンティティセット"), MDC_NONE_ID);
	m_PtsBf.Print( Mstr( "◇部品セット		"), MDC_NONE_ID);
	m_WinBf.Print( Mstr( "◇ウィンドウセット	"), MDC_NONE_ID);

	MBTRCPRINTS( Mstr( "◇カレント"));
	MBTRCPRINTI( Mstr( "	カレントグループID		"), m_idCurGrp);
	MBTRCPRINTI( Mstr( "	カレントウィンドウID	"), m_idCurWin);
	MBTRCPRINTI( Mstr( "	カレントウィンドウタイプ ( 0:３Ｄ、1：図面)"), m_itpCurWin);
	MBTRCPRINTI( Mstr( "	カレントレイヤーID		"), m_idCurLyr);
	MBTRCPRINTI( Mstr( "	カレントスキーマID		"), m_idCurScm);
	MBTRCPRINTI( Mstr( "	ポリゴン展開処理モード	"), m_itpCurWin);
	MBTRCPRINTIN( Mstr( "	ポリゴン展開処理モード	"), m_mdTenkai, 28);
	MBTRCPRINTFN( Mstr( "	ポリゴン展開処理パラメータ"), m_rmdTenkai, 7);
}

} // namespace MC
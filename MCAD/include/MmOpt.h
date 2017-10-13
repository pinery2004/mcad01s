#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MmOpt.h
//
//		オプション設定
//						
//
//==========================================================================================

#include "MsBitSet.h"

#ifdef DLL_EXPORT
	#undef DLL_EXPORT
#endif

#ifdef DLL_EXPORT_MHOPT_DO
	#pragma message( "<<< MHOPT・dll_EXport >>>")
	#define DLL_EXPORT							__declspec( dllexport)
#else
	#ifdef _DEBUG								// Release Mode で inline関数がundefになる不具合対応
		#ifdef DLL_NO_IMPORT_MD_DO
			#define DLL_EXPORT
		#else
//			#pragma message( "=== MHOPT・dll_IMport ===")
			#define DLL_EXPORT						__declspec( dllimport)
		#endif
	#else
		#define DLL_EXPORT
	#endif
#endif

namespace MC
{

class MhPlcInfo;

/////////////////////////////////////////////////////////////////////////////
//【機能】選択ＯＰＴ群 クラス
class DLL_EXPORT MhOpt
{
	MUINT	mb;
public:
	MhOpt()		{}
	MhOpt( MUINT i)
					{ mb = i;}
//	MhOpt & operator	= ( MhOpt &opt)
//					{ *this = opt;}
	MUINT GetOpt()
					{ return mb;}
	bool IsStd()
					{ return (mb <= 1);}
	friend MhOpt operator	~ ( MhOpt &opt)
					{ MhOpt optw; optw.mb = ~opt.mb; return optw;}
	MhOpt operator & ( const MhOpt &opt) const
					{ MhOpt optw; optw.mb = mb & opt.mb; return optw;}
	MhOpt operator | ( const MhOpt &opt) const
					{ MhOpt optw; optw.mb = mb | opt.mb; return optw;}
	MhOpt operator &= ( const MhOpt& opt1)
					{ mb &= opt1.mb; return *this;} 
	MhOpt operator |= ( const MhOpt& opt1)
					{ mb |= opt1.mb; return *this;} 
	bool operator == ( const MhOpt &Opt) const
					{ return (mb == Opt.mb);}

	/////////////////////////////////////////////////////////////////////////////
	//【機能】履歴管理コードの領域を開放する
	static void MhHistFre(
							MsBitSet*		*pHstv		// (I O) 開放履歴管理 NULL: 開放不要
					);

	/////////////////////////////////////////////////////////////////////////////
	//【機能】新規レコード追加時に設定する履歴管理コードを取得する
	//		　（履歴管理コードが入るエリアの確保は本関数で行う）
	static void MhHistAdd(
							MsBitSet*		*ppHstv		// (  O) 追加レコードに設定する履歴管理コードのポインタ NULL: 全履歴管理
					 );

	/////////////////////////////////////////////////////////////////////////////
	//【機能】レコード修正時に設定する履歴管理コードを取得する
	//【返値】修正後のレコード構成コード
	//			1 : 修正元履歴管理はカレントバージョンのみ設定されている場合で、
	//				修正元レコードは履歴管理変更なしで修正内容を上書きする。
	//				追加レコードはなし。
	//			3 : 修正元履歴管理にカレントバージョン以外のバージョンを含む場合で、
	//				カレントバージョン以外の履歴部分は、修正元レコードをそのまま残し履歴管理コードのみ修正する。
	//				さらに、カレントバージョンのみの履歴管理を持つ修正後のレコードを新たに追加する。
	//				（履歴管理コードが入るエリアの開放と確保は本関数で行う）
	static MINT MhHistMod(
							MhPlcInfo	 *pPlcEnR,		// (I O) 修正元部材
							MhPlcInfo* *ppPlcEnM		// (  O) 修正先部材
					);

	/////////////////////////////////////////////////////////////////////////////
	//【機能】レコード削除時に設定する履歴管理コードを取得する
	//【返値】削除後のレコード構成コード
	//			1 : 削除元履歴管理はカレントバージョンと同一の場合で、
	//				削除元レコードは削除する。
	//			3 : 削除元履歴管理にカレントバージョン以外の履歴管理を含む場合で、
	//				カレントバージョン以外の履歴管理部分は、削除元レコードをそのまま残し履歴管理コードのみ修正する。
	//				（履歴管理コードが入るエリアの開放と確保は本関数で行う）


	static MINT MhHistDel(
	//						MsBitSet		*pHstvR,		// (I O) 削除元レコードの履歴管理群
							MsBitSet*		*ppHstvMod		// (I O) 削除後のレコードの表示ＯＮ履歴管理群
					);

};

#ifdef DLL_EXPORT
	#undef DLL_EXPORT
#endif

#ifdef DLL_EXPORT_MHOPTV_DO
	#pragma message( "<<< MHOPTV・dll_EXport >>>")
	#define DLL_EXPORT							__declspec( dllexport)
#else
	#ifdef _DEBUG								// Release Mode で inline関数がundefになる不具合対応
		#ifdef DLL_NO_IMPORT_MD_DO
			#define DLL_EXPORT
		#else
//			#pragma message( "=== MHOPTV・dll_IMport ===")
			#define DLL_EXPORT						__declspec( dllimport)
		#endif
	#else
		#define DLL_EXPORT
	#endif
#endif

/////////////////////////////////////////////////////////////////////////////
//【機能】OPT群セット クラス

#define	MSZ_OPTV	3
#define	MSZ_OPTVB	(3 * 32)

class DLL_EXPORT MhOptV: public MsBitSet
{
public:
												// ＯＰＴ群管理情報
	static	bool		fcDB;					// カレント選択ＯＰＴ群をDBへ反映フラグ
												//		(TRUE:反映済、FALSE: 未反映)
	static	bool		fcStd;					// カレント選択ＯＰＴ群==標準　フラグ
												//		(TRUE:標準、FALSE:標準以外のＯＰＴ群有り)
	static	bool		fDispStd;				// 標準オプション表示フラグ
												//		(TRUE:表示ＯＮ、FALSE:表示ＯＦＦ)
	static	MhOpt		bmCurGOpt;				// カレント選択ＯＰＴ群
	static	MsBitSet	bmCurRGOptSet;			// カレント選択ＯＰＴ群で読込み対象のOPT群セット
	static	MsBitSet	bmCurWGOptSet;			// カレント選択ＯＰＴ群で書込み対象のOPT群セット
	static	MsBitSet	bmCurGOptSet;			// カレント選択ＯＰＴ群相当のOPT群セット
	static	MINT		nbmGOpt;				// 登録OPT群数
	static	MhOpt		bmGOpt[MSZ_OPTVB];		// 登録OPT群

public:
//	MhOptV()			{}

	/////////////////////////////////////////////////////////////////////////////
	//【機能】カレント選択ＯＰＴ群で読込み対象のOPT群セットを取得する
	static MsBitSet	MhOptBScRGet();
							
	/////////////////////////////////////////////////////////////////////////////
	//【機能】カレント選択ＯＰＴ群で書込み対象のOPT群セットを取得する
	static MsBitSet	MhOptBScWGet();
							
	/////////////////////////////////////////////////////////////////////////////
	//【機能】カレント選択ＯＰＴ群相当のOPT群セットを取得する
	static MsBitSet	MhOptBScGet();

	/////////////////////////////////////////////////////////////////////////////
	//【機能】ＯＰＴ群管理情報を初期化する
	static void MmOptInit();

	/////////////////////////////////////////////////////////////////////////////
	//【機能】ＯＰＴ群選択チェックボックスの選択状況をカレント選択ＯＰＴ群に反映する
	static void MmOptSet(
							MINT		nOpt,		// (I  ) チェックボックス番号
							MINT		iChk		// (I  ) チェックボックス選択状況
					);

	/////////////////////////////////////////////////////////////////////////////
	//【機能】カレント選択ＯＰＴ群取得
	//【返値】ＯＰＴ群有無
	//			TRUE: 標準
	//			FALSE: 標準以外のオプション設定あり
	static bool MmOptGetC(
							MhOpt*	pOpt			// (  O) カレント選択ＯＰＴ群
					);

	/////////////////////////////////////////////////////////////////////////////
	//【機能】ＤＢレコードのOPT群セットがカレント選択ＯＰＴ群を含むか調べる
	//【返値】選択ＯＰＴ群含みフラグ
	//			TRUE:  含む
	//			FALSE: 含まない
	static bool MmOptChkC(
							MsBitSet	*pOptv		// (I  ) ＤＢレコードのOPT群セット
					);

	/////////////////////////////////////////////////////////////////////////////
	//【機能】ＤＢレコードのOPT群セットがカレント選択ＯＰＴ群を含むか調べる
	//【返値】選択ＯＰＴ群含みフラグ
	//			TRUE : 含む		表示ＯＦＦ
	//			FALSE: 含まない	表示ＯＮ
	static bool MmOptOffChkC(
							MsBitSet	*pOptv		// (I  ) ＤＢレコードのOPT群セット
					);

	/////////////////////////////////////////////////////////////////////////////
	//【機能】ＤＢレコード修正後のレコード構成コードを取得する
	//		　（修正もとのＯＰＴ群のOPT群セットとカレント選択ＯＰＴ群の書込み対象OPT群セットを
	//		　	比較してどのような修正処理のレコードであるか調べる）
	//【返値】修正後のレコード構成コード
	//			1: 修正元レコードのOPT群セット ＝ 書込み対象のOPT群セット
	//				ＯＰＴ群の変更は不要で、属性修正のみ変更する
	//			2: 修正元レコードのOPT群セット ⊂ 書込み対象のOPT群セット
	//				ＯＰＴ群の変更と属性変更する
	//			3: その他 ( 修正元レコードのOPT群セット ⊃ 書込み対象のOPT群セット || 部分的に重なっている )
	//				（修正元レコードのOPT群セット ⊃ （書込み対象のOPT群セット ∩ 修正元レコードのOPT群セット)）
	//				修正元レコードはＯＰＴ群変更し、新しく書込み対象のOPT群セットのＯＰＴ群を持つ
	//				コピーレコードを作製しその属性を変更する
	//
	static MINT MmOptModChk(								// (  O) 修正後のレコード構成コード
							MsBitSet*	*ppOptvRN,	// (I O) 修正前後レコードのOPT群セット　	(修正後のレコード構成コード >= 2 の時有効)
							MsBitSet*	*ppOptvWN,	// (  O) 修正用に追加レコードのOPT群セット　(修正後のレコード構成コード == 3 の時有効)
							MhOptV*	*ppOptv			// (  O) ＯＰＴ群管理情報
					);

	/////////////////////////////////////////////////////////////////////////////
	//【機能】レコード削除時に設定するＯＰＴ群コードを取得する
	//【返値】削除後のレコード構成コード
	//			1 : 削除元ＯＰＴ群は書込みＯＰＴ群と同一の場合で、
	//				削除元レコードは削除する。
	//			2 :	削除元ＯＰＴ群は書込みＯＰＴ群に全て含まれかつ追加ＯＰＴ群がある場合で、
	//				削除元レコードは削除する。
	//			3 : 削除元ＯＰＴ群に書込みＯＰＴ群以外のＯＰＴ群を含む場合で、
	//				書込みＯＰＴ群以外のＯＰＴ群部分は、削除元レコードをそのまま残しＯＰＴ群コードのみ修正する。
	//				（ＯＰＴ群コードが入るエリアの開放と確保は本関数で行う）
	//
	static MINT MmOptDelChk(								// (  O) 削除後のレコード構成コード
							MsBitSet*	*ppOptvRN,	// (  O) 削除前後のレコードの表示ＯＮOPT群セット
							MsBitSet*	*ppOptvDN	// (  O) 削除前後のレコードの表示ＯＦＦOPT群セット
					);

	/////////////////////////////////////////////////////////////////////////////
	//【機能】登録OPT群より選択ＯＰＴ群を検索し無ければ追加登録する
	//		　選択ＯＰＴ群を全て含む登録OPT群の集まりに相当するOPT群セットを
	//【返値】検索状況
	//			0:無しであり追加
	//			1:有り
	//
	static MINT MmOptSrch(
							MhOpt	Opt				// (I  ) 検索ＯＰＴ群
					);

	/////////////////////////////////////////////////////////////////////////////
	//【機能】選択ＯＰＴ群で読込み対象のOPT群セットと
	//		  書込み対象のOPT群セットを求める
	//【返値】
	//			TRUE: 標準
	//			FALSE: 標準以外のオプション設定あり
	//
	static bool MmOptIncSrch(
							MhOpt	Opt				// (I  ) 選択ＯＰＴ群
					);

	/////////////////////////////////////////////////////////////////////////////
	// カレント選択ＯＰＴ群に相当のOPT群セットをDBの参照対象レコードへ反映
	static void MmOptAddDB();

};

} // namespace MC
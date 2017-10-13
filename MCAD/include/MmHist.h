#pragma once

#include "MsBitSet.h"

#ifdef DLL_EXPORT
	#undef DLL_EXPORT
#endif

#ifdef DLL_EXPORT_MHHISTV_DO
	#pragma message( "<<< MHHISTV・dll_EXport >>>")
	#define DLL_EXPORT							__declspec( dllexport)
#else
	#ifdef _DEBUG								// Release Mode で inline関数がundefになる不具合対応
		#ifdef DLL_NO_IMPORT_MD_DO
			#define DLL_EXPORT
		#else
//			#pragma message( "=== MHHISTV・dll_IMport ===")
			#define DLL_EXPORT						__declspec( dllimport)
		#endif
	#else
		#define DLL_EXPORT
	#endif
#endif


namespace MC
{

/////////////////////////////////////////////////////////////////////////////
//【機能】バージョン郡 クラス
#define	MSZ_HIST	3
#define	MSZ_HISTB	(3 * 32)

class MhHistV: public MsBitSet
{
//	MUINT	ver[MSZ_HIST];					// バージョン　バージョン郡

public:
											// バージョン管理情報
//	static	bool		fcCng_NU;			// カレント選択バージョンの変更有無
											//		(TRUE: 変更有り、FALSE: 変更無し)
//	static	bool		fcDB;				// カレント選択バージョンをDBへ反映フラグ
											//		(TRUE:反映済、FALSE: 未反映)
	static	bool		fcStd;				// カレント選択バージョン==標準　フラグ
											//		(TRUE:標準、FALSE:標準以外のバージョン有り)
	static	MsBitSet	vVerC;				// カレントバージョン
	static	MsBitSet	vVerB;				// コピー元バージョン
	static	MINT		nVer;				// 登録バージョン数
	static	CString		sVerC[MSZ_HISTB];	// 登録バージョン名郡
	static	CString		sVerB[MSZ_HISTB];	// コピー元バージョン名郡
	static	CString		sVerD[MSZ_HISTB];	// 修正日郡

public:
	MhHistV()	{}
};

class DLL_EXPORT MhHist
{
public:
/////////////////////////////////////////////////////////////////////////////
//【機能】修正履歴管理情報を初期化する
static void MmHistInit();

/////////////////////////////////////////////////////////////////////////////
//【機能】修正履歴管理情報を得る
static void MmHistGet(
						MINT*		m_pnVer,
						CString*	*m_ppsVerC,
						CString*	*m_ppsVerB,
						CString*	*m_ppsVerD
				);

/////////////////////////////////////////////////////////////////////////////
//【機能】バージョンＵＰ
//【返値】ステイタス	0: 正常	-1: 元のバージョン無し	-2: 先のバージョンが既に有り
static MINT MmVerUp(
						CString		sVerB,
						CString		sVerC,
						MINT*		m_pnVer,
						CString*	*m_ppsVerC,
						CString*	*m_ppsVerB,
						CString*	*m_ppsVerD
				);

/////////////////////////////////////////////////////////////////////////////
//【機能】バージョン名変更
//【返値】ステイタス	0: 正常	-1: 元のバージョン無し	-2: 先のバージョンが既に有り
static MINT MmVerCng(
						CString		sVerB,
						CString		sVerC,
						MINT*		m_pnVer,
						CString*	*m_ppsVerC,
						CString*	*m_ppsVerB,
						CString*	*m_ppsVerD
				);

/////////////////////////////////////////////////////////////////////////////
//【機能】バージョン名削除
//【返値】ステイタス	0: 正常	-1: バージョン無し
static MINT MmVerDel(
						CString		sVer,
						MINT*		m_pnVer,
						CString*	*m_ppsVer
				);

/////////////////////////////////////////////////////////////////////////////
//【機能】カレント選択バージョンを得る
//【返値】バージョン有無
//			TRUE: 標準
//			FALSE: 標準以外のバージョン設定あり
static bool MmVerGetC(
						MsBitSet*	pVer			// (  O) カレント選択バージョン
				);

/////////////////////////////////////////////////////////////////////////////
//【機能】ＤＢレコードの履歴管理がカレントバージョンを含むか調べる
//【返値】カレントバージョン含みフラグ
//			TRUE : 含む
//			FALSE: 含まない
static bool MmHistChkC(
						MsBitSet	*pHstv			// (I  ) ＤＢレコードのOPT群セット
				);

/////////////////////////////////////////////////////////////////////////////
//【機能】ＤＢレコードのバージョン郡がカレント選択バージョンを含むか調べる
//【返値】選択バージョン含みフラグ
//			TRUE:  含む
//			FALSE: 含まない
static bool MmVerChkC(
						MsBitSet	*pVerv			// (I  ) ＤＢレコードのバージョン郡
				);


/////////////////////////////////////////////////////////////////////////////
//【機能】ＤＢレコード修正時のレコード処理タイプを取得する
//		　（修正元のバージョンの履歴管理とカレントバージョン該当の履歴管理を
//		　	比較してどのような修正処理のレコードであるか調べる）
//【返値】修正後のレコード構成コード
//			1: 修正元レコードの履歴管理 ＝ 書込み対象の履歴管理
//				バージョンの変更は不要で、属性修正のみ変更する
//			3: その他 ( 修正元レコードの履歴管理 ⊃ カレントバージョン該当の履歴管理 )
//				修正元レコードはバージョン変更し、新しく書込み対象の履歴管理のバージョンを持つ
//				コピーレコードを作製しその属性を変更する
//
static MINT MmHistModChk(							// (  O) 修正後のレコード構成コード
//						MsBitSet	*pHstvR,		// (I O) 修正元レコードの履歴管理
						MsBitSet*	*ppHstvMod,		// (I O) 修正元レコードに設定する履歴管理　(修正後のレコード構成コード >= 2 の時有効)
						MsBitSet*	*ppHstvAdd		// (  O) 修正用に追加レコードの履歴管理　(修正後のレコード構成コード == 3 の時有効)
				);

/////////////////////////////////////////////////////////////////////////////
//【機能】レコード削除時に設定するバージョンコードを取得する
//【返値】削除後のレコード構成コード
//			1 : 削除元バージョンは書込みバージョンと同一の場合で、
//				削除元レコードは削除する。
//			3 : 削除元バージョンに書込みバージョン以外のバージョンを含む場合で、
//				カレントバージョンのみ削除する。
//				カレントバージョン以外のバージョン部分は、削除元レコードをそのまま残す。（バージョンコードのみ修正）
//				（バージョンコードが入るエリアの開放と確保は本関数で行う）
//
static MINT MmHistDelChk(							// (  O) 削除後のレコード構成コード
//						MsBitSet	*pHstvR,		// (I O) 削除元レコードの履歴管理
//													// 		 削除後レコードの履歴管理に変更する
						MsBitSet*	*ppHstvMod		// (I O) 削除後のレコードの表示ＯＮ履歴管理
				);

/////////////////////////////////////////////////////////////////////////////
//【機能】バージョン選択
//【返値】ステイタス	0: 正常	-1: 選択バージョン無し
static MINT MmSetVer(
						CString		sVerB,
						CString		sVerC,
						MINT*		m_pnVer,
						CString*	*m_ppsVerC,
						CString*	*m_ppsVerB,
						CString*	*m_ppsVerD
				);
};

} // namespace MC
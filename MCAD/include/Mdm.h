#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MdModel.h
//
//		ＭＣＡＤモデル
//
//

namespace MC
{

#ifdef DLL_EXPORT
	#undef DLL_EXPORT
#endif

#ifdef DLL_EXPORT_MDM_DO
	#pragma message( "<<< Mdm・dll_EXport >>>")
	#define DLL_EXPORT							__declspec( dllexport)
#else
	#ifdef _DEBUG								// Release Mode で inline関数がundefになる不具合対応
		#ifdef DLL_NO_IMPORT_MD_DO
			#pragma message( "=== Mdm・dll_none ===")
			#define DLL_EXPORT
		#else
			#pragma message( "=== Mdm・dll_IMport ===")
			#define DLL_EXPORT						__declspec( dllimport)
		#endif
	#else
		#pragma message( "=== Mdm・dll_none ===")
		#define DLL_EXPORT
	#endif
#endif

class DLL_EXPORT Mdm
{
//private:
public:
	static CArray<MdModel*>	ms_pMdl;			// モデルリスト
	static MDID				ms_idCurMdl;		// カレントモデルID
	static MdModel*			ms_pCurMdl;			// カレントモデル


public:
	//===========================================================================
	//【機能】モデルを初期化する
	//【返値】なし
	static MINT MdmInitialize( void);

	//===========================================================================
	//【機能】モデルの使用を終了する
	//【返値】なし
	static MINT Mdm::MdmTerminate( void);

	//===========================================================================
	//【機能】モデルを追加し、モデルIDを取得
	//【返値】モデルID
	static MDID AddModel(
						MCHAR*	i_cMdlNm		// モデル名
						);
//S	{
//		// s
//	}

	//===========================================================================
	//【機能】モデル数を取得する
	//【返値】モデル数
	static MINT GetModelCount()
	{
		return (MINT)ms_pMdl.GetCount();
	}

	//===========================================================================
	//【機能】モデル名からモデルIDを取得する
	//【返値】モデルID
	static MDID GetModelId(
						MCHAR*	i_cMdlNm		// モデル名
						);
//S	{
//		MDID idMdl = 0;
//		for ( int iC=0; iC<ms_pMdl.GetCount(); iC++) {
//			if ( Mstrcmp( ms_pMdl[iC]->m_cMdlNm, i_cMdlNm)) {
//				idMdl = MHNtoID( iC);
//				break;
//			}
//		}
//		return idMdl; 
//	}

	//===========================================================================
	//【機能】モデルIDからモデル名を取得する
	//【返値】モデルID
	static MDID GetModelName(
						MDID	i_idMdl,		// モデルID
						MCHAR*	o_cMdlNm		// モデル名
						);

	//===========================================================================
	//【機能】カレントモデルを設定する
	//【返値】モデル
	static MdModel* SetCurModel(
						MDID	i_idnModel		// モデルID
						)
	{
		ms_idCurMdl = i_idnModel;
		ms_pCurMdl = ms_pMdl[MIDtoHN(i_idnModel)];
		return ms_pCurMdl;
	}

	//===========================================================================
	//【機能】カレントモデルIDを取得する
	//【返値】モデルID
	static MDID	GetCurModelId()
	{
		return ms_idCurMdl;
	}

	//===========================================================================
	//【機能】カレントモデルを取得する
	//【返値】モデル
	static MdModel* GetCurModel()
	{
		return ms_pCurMdl;
	}

	//===========================================================================
	//【機能】モデルIDからモデルを取得する
	//【返値】モデル
	static MdModel* GetModel( MDID i_idnModel = 1)
	{
		return ms_pMdl[MIDtoHN(i_idnModel)];
	}

};

} // namespace MC
#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MdDispList.h
//
//		ＭＣＡＤモデル
//						ディスプレイリスト
//
//
//==========================================================================================

namespace MC
{

#ifdef DLL_EXPORT
	#undef DLL_EXPORT
#endif

#ifdef DLL_EXPORT_DISPLIST_DO
	#pragma message( "<<< DispList・dll_EXport >>>")
	#define DLL_EXPORT							__declspec( dllexport)
#else
	#ifdef _DEBUG								// Release Mode で inline関数がundefになる不具合対応
		#ifdef DLL_NO_IMPORT_SUB_DO
			#define DLL_EXPORT
		#else
//			#pragma message( "=== MgPlane・dll_IMport ===")
			#define DLL_EXPORT					__declspec( dllimport)
		#endif
	#else
		#define DLL_EXPORT
	#endif
#endif

class DLL_EXPORT DispList
{
public:

	/////////////////////////////////////////////////////////////////////////////
	//	【機能】全表示エンティティのディスプレイリストを作成する
	//
	static MINT DrawAllEnt(							//
					const	MgPoint3&	PtCtr,		// 構造家モデルの中心
							MREAL		rB			// ３次元表示倍率
					);

	/////////////////////////////////////////////////////////////////////////////
	//	【機能】図形のディスプレイリストを作成する
	static MINT	DrawFig(
					class	MDFIG&		Fig,		// 図形
					const	MgPoint3	&PtCtr,		// 構造家モデルの中心
							MREAL		rB			// ３次元表示倍率
					);

	
	////////////////////////////////////////////////////////////////////////////
	//	【機能】点のディスプレイリストを作成する		(実座標)
	//			
	static void DrawPoint(
					const	MgPoint3	&i_Pt,		// 点						(実座標)
					const	MgPoint3	&i_PtCtr,	// 構造家モデルの中心
							MREAL		i_rB		// ３次元表示倍率
					);

	////////////////////////////////////////////////////////////////////////////
	//	【機能】線分のディスプレイリストを作成する		(実座標)
	//			
	static void DrawLine( 
					const	MgLine3		&Ln,		// 三角形					(実座標)
					const	MgPoint3	&PtCtr,		// 構造家モデルの中心
							MREAL		rB			// ３次元表示倍率
					);


	////////////////////////////////////////////////////////////////////////////
	//	【機能】３角形のディスプレイリストを作成する	(論理座標)
	//				(隙間を埋める為に若干大きめに作成する）（要検討）
	//			
	static void DspTriangles( 
					const	MgPolyg3	&PGTri,			// 三角形				(論理座標)
					const	MgVect3		&VuUp			// ３角形平面の法線ベクトル
					);

	////////////////////////////////////////////////////////////////////////////
	//	【機能】４角形のディスプレイリストを作成する	(論理座標)
	//				(底辺と上辺より作成)
	//			
	static void DspQuads( 
					const	MgLine3		&LnLf,			// 底辺（右辺）			(論理座標)
					const	MgLine3		&LnRt			// 上辺（左辺）			(論理座標)
					);

	////////////////////////////////////////////////////////////////////////////
	//	【機能】穴付き多角形のディスプレイリストを作成する	(実座標)
	//			
	static void DrawPolygon(
					const	MgGPolyg3	&GPgR,			// 穴付き多角形			(実座標)
					const	MgPoint3	&PtCtr,			// 構造家モデルの中心	(実座標)
							MREAL		rB				// ３次元表示倍率
					);

	////////////////////////////////////////////////////////////////////////////
	//	【機能】直方体のディスプレイリストを作成する	(論理座標)
	//			
	static void DspRectangular( 
							MgPoint3	PT[2][2][2],	// 直方体の頂点座標		(論理座標)
					const	MgVect3		&VuLng,			// 始点から終点方向の単位ベクトル
					const	MgVect3		&VuRt,			// 右方向の単位ベクトル
					const	MgVect3		&VuUp			// 上方向の単位ベクトル
					);

	////////////////////////////////////////////////////////////////////////////
	//【機能】光源、物体パラメータの設定 1
	static void MsSetLight_MaterialParameter1( void);

	static void SetLightModel1( void);

	////////////////////////////////////////////////////////////////////////////
	//【機能】光源の設定
	//
	static void SetLight( void);

	////////////////////////////////////////////////////////////////////////////
	//【機能】光源、物体パラメータの設定 1
	static void MsGlInitialize( void);

	////////////////////////////////////////////////////////////////////////////
	//	【機能】部材表示属性(Material)を設定する
	//			
	static void SetMaterial(
							MINT		i_iT,			// 表示タイプ	{ 0:実体、1:半透明}、
							MINT		i_iB			// 部位			{ 0:屋根、1:矢切、2:壁、3:部材、4:基礎、5:パネル_内側、6:パネル_枠}
					);

	////////////////////////////////////////////////////////////////////////////
	//【機能】ビューポートサイズの変更
	static void MsOnSize(UINT nType, int cx, int cy);

};

////#ifdef DLL_EXPORT
////	#undef DLL_EXPORT
////#endif
////
////#ifdef DLL_EXPORT_GEOMETRY_DO
////	#pragma message( "<<< DispList・dll_EXport >>>")
////	#define DLL_EXPORT							__declspec( dllexport)
////#else
////	#ifdef _DEBUG								// Release Mode で inline関数がundefになる不具合対応
////		#ifdef DLL_NO_IMPORT_SUB_DO
////			#define DLL_EXPORT
////		#else
//////			#pragma message( "=== MgPlane・dll_IMport ===")
////			#define DLL_EXPORT					__declspec( dllimport)
////		#endif
////	#else
////		#define DLL_EXPORT
////	#endif
////#endif

////class DLL_EXPORT Geometry
////{
////public:

////////////////////////////////////////////////////////////////////////////
//	【機能】実座標系の点座標を３次元表示座標(論理座標)に変換する

inline MgPoint3 DPtoDSP(						//							(論理座標)
				const	MgPoint3	&i_Pt,		// 相対実座標系の点座標		(実座標)
				const	MREAL		&i_rZ,		// 基準高さ					(実座標)
				const	MgPoint3	&i_PtCtr,	// 実座標の中心座標			(実座標)
						MREAL		i_rB		// 実座標から３次元表示座標に変換の倍率
				)
{
	return MgPoint3( ( i_Pt.x - i_PtCtr.x) * i_rB, 
					 ( i_Pt.y - i_PtCtr.y) * i_rB,
					 ( i_Pt.z + i_rZ - i_PtCtr.z) * i_rB
				);
}

////////////////////////////////////////////////////////////////////////////
//	【機能】実座標系の線分座標を３次元表示座標(論理座標)に変換する

inline MgLine3 DPtoDSP(							//											(論理座標)
				const	MgLine3		&i_Ln,		// 相対実座標系の線分座標					(実座標)
				const	MREAL		&i_rZ,		// 基準高さ									(実座標)
				const	MgPoint3	&i_PtCtr,	// 実座標の中心座標							(実座標)
						MREAL		i_rB		// 実座標から３次元表示座標に変換の倍率
				)
{
	return MgLine3( 
			MgPoint3( ( i_Ln.p[0].x - i_PtCtr.x) * i_rB, ( i_Ln.p[0].y - i_PtCtr.y) * i_rB,
					  ( i_Ln.p[0].z + i_rZ - i_PtCtr.z) * i_rB),
			MgPoint3( ( i_Ln.p[1].x - i_PtCtr.x) * i_rB, ( i_Ln.p[1].y - i_PtCtr.y) * i_rB,
					  ( i_Ln.p[1].z + i_rZ - i_PtCtr.z) * i_rB) 
				);
}

////////////////////////////////////////////////////////////////////////////
//	【機能】実座標系の屋根面３角形座標を３次元表示座標に変換する
//
inline void DPtoDSP3(
				const	MgPolyg3	&i_PgI,		// 屋根面３角形（基準高さからの相対高さ）	(実座標)
				const	MREAL		&i_rZ,		// 基準高さ									(実座標)
				const	MgVect3		&i_VtR,		// 屋根基準面から屋根面への移動ベクトル		(実座標)
				const	MgPoint3	&i_PtCtr,	// 実座標の中心座標							(実座標)
						MREAL		i_rB,		// 実座標から３次元表示座標に変換の倍率
						MgPolyg3*	o_pPgO		// ３次元表示上面							(論理座標)
				)
{
	MINT	ic0;
	o_pPgO->m_n = 3;
	for ( ic0=0; ic0<3; ic0++) {
		o_pPgO->m_P[ic0].x = ( i_PgI.m_P[ic0].x + i_VtR.x - i_PtCtr.x) * i_rB;
		o_pPgO->m_P[ic0].y = ( i_PgI.m_P[ic0].y + i_VtR.y - i_PtCtr.y) * i_rB;
		o_pPgO->m_P[ic0].z = ( i_PgI.m_P[ic0].z + i_VtR.z + i_rZ - i_PtCtr.z) * i_rB;
	}
}

inline void DPtoDSP3R(
				const	MgPolyg3	&i_PgI,		// 屋根面３角形（基準高さからの相対高さ）	(実座標)
				const	MREAL		&i_rZ,		// 基準高さ									(実座標)
				const	MgVect3		&i_VtR,		// 屋根基準面から屋根面への移動ベクトル		(実座標)
				const	MgPoint3	&i_PtCtr,		// 実座標の中心座標							(実座標)
						MREAL		i_rB,			// 実座標から３次元表示座標に変換の倍率
						MgPolyg3*	o_pPgO		// ３次元表示下面(裏方向)					(論理座標)
				)
{
	MINT	ic0, ic1;
	o_pPgO->m_n = 3;
	for ( ic0=0,ic1=2; ic0<3; ic0++,ic1--) {
		o_pPgO->m_P[ic1].x = (i_PgI.m_P[ic0].x + i_VtR.x - i_PtCtr.x) * i_rB;
		o_pPgO->m_P[ic1].y = (i_PgI.m_P[ic0].y + i_VtR.y - i_PtCtr.y) * i_rB;
		o_pPgO->m_P[ic1].z = (i_PgI.m_P[ic0].z + i_VtR.z + i_rZ - i_PtCtr.z) * i_rB;
	}
}


} // namespace MC

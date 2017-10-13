#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MdModel.h
//
//		ＭＣＡＤモデル
//
//

//==========================================================================================
//	ウィンドウ情報
namespace MC
{

class MdmWin
{
public:
	MINT		m_itpWin;						// ウィンドウタイプ ( 0:３Ｄ、1：図面)　　// 次元
	MINT		m_iProj;						// 透視方法			透視投影と平行投影
	MDID		m_idFn;							// 機能番号
	MgPoint3	m_ViewPt;						// 視点／注視点（含む履歴）
	MREAL		m_rZClip[2];					// Zクリッピング値
	MgPoint2	m_Size[2];						// サイズ
	MREAL		m_Scale;						// 縮尺etc
	// 物の表示／非表示　制御

	// ウィンドウに物を表示する時には、
	//   ウィンドウの機能番号に対応する関係名で図形を取り出す。
	//   無い場合には、３次元の図形を取り出して表示を行う。
public:
	void	Init()		{}
	void	Free()		{}
	// デバッグ用トレース出力
	void Print( MCHAR* i_s, MINT i_i);
};

//	トレース
inline void MdmWin::Print( MCHAR* i_s, MINT i_i)
{
#ifdef _DEBUG
	//Msprintf( MbLog::m_Str, Mstr( "%s < MdmSetP > ID = %d\n"), i_s, i_i);
	//MBTRCPRBF;
	//MBTRCPRINTI( Mstr( "		ウィンドウタイプ ( 0:３Ｄ、1：図面)", m_itpWin);
	//MBTRCPRINTI( Mstr( "		透視方法			透視投影と平行投影"), m_iProj);
	//MBTRCPRINTI( Mstr( "		機能番号"), m_idFn);
	//MBTRCPRINTIN( Mstr( "		視点／注視点（含む履歴）", m_ViewPt, 3);
	//MBTRCPRINTFN( Mstr( "		Zクリッピング値"), m_rZClip, 2);
	//MBTRCPRINTFN( Mstr( "		サイズ"), m_Size, 2);
	//MBTRCPRINTFN( Mstr( "		縮尺etc", m_Scale);
#endif
}

//==========================================================================================
class MdmCordinate
{
public:	
	// デバッグ用トレース出力
	void Print( MCHAR* i_s, MINT i_i);
};
//	トレース
inline void MdmCordinate::Print( MCHAR* i_s, MINT i_i)
{
#ifdef _DEBUG
	Msprintf( MbLog::m_Str, Mstr( "%s	< MdmCordinate > ID = %d\n"), i_s, i_i);
	MBTRCPRBF;
#endif
}

} // namespace MC
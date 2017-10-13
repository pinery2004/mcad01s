//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MhFile.cpp
//
//		構造家モデル読み書きＩＯ処理
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"
#include "afxdlgs.h"

#include "MbDebug.h"
#include "MgDefine.h"
#include "MgTol.h"

#include "MgAngle.h"
#include "MgPoint.h"
#include "MgLine.h"
#include "MgPlane.h"

#include "MmDefine.h"
#include "MsBitSet.h"
#include "MmOpt.h"

#define DLL_EXPORT_IEMODEL_DO
#include "MhLib.h"

#include "MbFile.h"
#include "MsLib.h"

namespace MC
{

static MCHAR*	 z_cTitle = Mstr( "MCAD");
static MCHAR*	 z_cVer = Mstr( "V0.01");

////////////////////////////////////////////////////////////////////////////
//【機能】構造家モデル

////////////////////////////////////////////////////////////////////////////
//【機能】コンストラクタ

MhMdlIO::MhMdlIO()
{
	memset( &m_Header, 0, sizeof(MhMdlHeader));
	Mstrcpy( m_Header.cTitle, z_cTitle);
	Mstrcpy( m_Header.cVer, z_cVer);
	m_Header.cTime = MsGetCurrentTime();								// 作成日時 
}

////////////////////////////////////////////////////////////////////////////
//【機能】デストラクタ

MhMdlIO::~MhMdlIO          ()
{
}

////////////////////////////////////////////////////////////////////////////
//【機能】ファイル読み書き処理開始宣言
//【返値】==  0 : 正常終了
//		  == -2 : 既にオープンされている
//		  ≦ -3 : その他エラー

MINT MhMdlIO::Open(
				const	DWORD		iMode,			// (I ) 使用モード
													//		== MBREAD:	読み込み専用
													//		== MBWRITE:	書き込み専用
				const	MCHAR*		cFilePath
				)
{
	MINT		ist = -1;
	MINT		OutSiz;
	MUINT		nOpenFlag;										// アクセスモード

	Mstrcpy( m_MdlPath, cFilePath);

	if ( iMode == MBREAD) {
		nOpenFlag = CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary;
	} else if ( iMode == MBWRITE) {
		nOpenFlag = CFile::modeWrite | CFile::modeCreate | CFile::shareExclusive;
	}
	
	if ( !m_File.Open( cFilePath, nOpenFlag)) {
		ist = -3;
		MQUIT;
	}

	if ( iMode == MBREAD) {
		OutSiz = m_File.Read( &m_Header, sizeof( MhMdlHeader));
		if ( OutSiz != sizeof( MhMdlHeader))
			MQUIT;
		if ( Mstrcmp( m_Header.cTitle, z_cTitle) != 0)
			MQUIT;
		MINT	itr;
		OutSiz = m_File.Read( &itr, sizeof( MINT));
		OutSiz = m_File.Read( &itr, sizeof( MINT));
	} else if ( iMode == MBWRITE) {
		m_File.Write( &m_Header, sizeof(MhMdlHeader));
		MINT	it = 333;
		m_File.Write( &it, sizeof(MINT));
		it = 777;
		m_File.Write( &it, sizeof(MINT));
	}
	ist = 0;
exit:
	return( ist);
}


////////////////////////////////////////////////////////////////////////////
//【機能】ファイル読み書き処理終了宣言
//【返値】==  0 : 正常終了
//		  ≠  0 : その他エラー

MINT MhMdlIO::Close()
{
	MINT		ist;
//	MINT		ist1;
	MINT		iSize = 0;

	ist = -1;

	if ( m_iMode == MBWRITE) {
		m_File.Write( &iSize, sizeof(MINT));
		m_File.Write( &iSize, sizeof(MINT));
	}

	m_File.Close();

	return ( ist);
}

////////////////////////////////////////////////////////////////////////////
//【機能】ファイル読み込み
//【返値】 > 0 : 読み込みバイト数
//		  == 0 : 終了
//		   < 0 : その他エラー

MINT MhMdlIO::Read(
						MINT*		piType,			// (  O) ﾀｲﾌﾟ
						void**		Buf			// (  O) 読込み領域
				)
{
	MINT		iSizeR;
	MINT		iSizeW;

	iSizeW = m_File.Read( piType, sizeof(MINT));

	if ( iSizeW != sizeof(MINT)) {
		iSizeR = -1;
		MQUIT;
	}

	iSizeW = m_File.Read( &iSizeR, sizeof(MINT));

	if ( iSizeW != sizeof(MINT)) {
		iSizeR = -1;
		MQUIT;
	}
	if ( iSizeR == 0)
		MQUIT;													// 終了

	//int* pTest = new int[2];
//===
	static int iCtrc = 0;
	TRACE( "%d: new char[%d]\n", ++iCtrc, iSizeR);
	if( iCtrc == 5157) {
		int iii = 1;
	}
//---
	*Buf = (byte *)new char[iSizeR];
	ASSERT( *Buf);												// メモリ　りアロックエラー　<ERROR>
	iSizeW = m_File.Read( *Buf, iSizeR);

	if ( iSizeW != iSizeR) {
		iSizeR = -1;
		MQUIT;
	}
exit:
	return ( iSizeR);
}

//U ////////////////////////////////////////////////////////////////////////////
////【機能】任意型データのファイル書き込み
////【返値】==  0 : 正常終了
////		  ≠  0 : その他エラー
//
//MINT MhMdlIO::Write(
//				const	MINT		i_tp,			// ﾀｲﾌﾟ
//				const	void*		i_bf,			// 書込み領域
//				const	MINT		i_sz			// 書込みﾊﾞｲﾄ数
//				)
//{
//	MINT		ist;
//	ist = -1;
//	m_File.Write( &i_tp, sizeof(MINT));							// ﾀｲﾌﾟ
//	m_File.Write( &i_sz, sizeof(MINT));							// ｻｲｽﾞ
//	m_File.Write( i_bf, i_sz);									// 任意型データ
//	ist = 0;
//	return( ist);
//}

////////////////////////////////////////////////////////////////////////////
//【機能】SHORT型データのファイル書き込み
//【返値】==  0 : 正常終了
//		  ≠  0 : その他エラー

MINT MhMdlIO::Write(
				const	MINT		i_tp,			// ﾀｲﾌﾟ
				const	MSHORT*		i_ibf,			// 書込みSHORT配列
				const	MINT		i_iwd			// 書込みSHORT型ワード数
				)
{
	MINT		ist;
	ist = -1;
	m_File.Write( &i_tp, sizeof(MINT));							// ﾀｲﾌﾟ
	m_File.Write( &i_iwd, sizeof(MINT));						// ｻｲｽﾞ
	m_File.Write( i_ibf, i_iwd * sizeof(MSHORT));				// 整数配列
	ist = 0;
	return( ist);
}

////////////////////////////////////////////////////////////////////////////
//【機能】整数型データのファイル書き込み
//【返値】==  0 : 正常終了
//		  ≠  0 : その他エラー

MINT MhMdlIO::Write(
				const	MINT		i_tp,			// ﾀｲﾌﾟ
				const	MINT*		i_ibf,			// 書込み整数配列
				const	MINT		i_iwd			// 書込みワード数
				)
{
	MINT		ist;
	ist = -1;
	m_File.Write( &i_tp, sizeof(MINT));							// ﾀｲﾌﾟ
	m_File.Write( &i_iwd, sizeof(MINT));						// ｻｲｽﾞ
	m_File.Write( i_ibf, i_iwd * sizeof(MINT));					// 整数配列
	ist = 0;
	return( ist);
}

////////////////////////////////////////////////////////////////////////////
//【機能】整数型データのファイル書き込み
//【返値】==  0 : 正常終了
//		  ≠  0 : その他エラー

MINT MhMdlIO::Write(
				const	MINT		i_tp,			// ﾀｲﾌﾟ
				const	MUINT*		i_ibf,			// 書込み整数配列
				const	MINT		i_iwd			// 書込みワード数
				)
{
	MINT		ist;
	ist = -1;
	m_File.Write( &i_tp, sizeof(MINT));							// ﾀｲﾌﾟ
	m_File.Write( &i_iwd, sizeof(MINT));						// ｻｲｽﾞ
	m_File.Write( i_ibf, i_iwd * sizeof(MINT));					// 整数配列
	ist = 0;
	return( ist);
}

////////////////////////////////////////////////////////////////////////////
//【機能】FLOAT型データのファイル書き込み
//【返値】==  0 : 正常終了
//		  ≠  0 : その他エラー

MINT MhMdlIO::Write(
				const	MINT		i_tp,			// ﾀｲﾌﾟ
				const	float*		i_rbf,			// 書込み実数配列
				const	MINT		i_iwd			// 書込みワード数
				)
{
	MINT		ist;
	ist = -1;
	m_File.Write( &i_tp, sizeof(MINT));							// ﾀｲﾌﾟ
	m_File.Write( &i_iwd, sizeof(MINT));						// ｻｲｽﾞ
	m_File.Write( i_rbf, i_iwd * sizeof(float));				// 実数配列
	ist = 0;
	return( ist);
}

////////////////////////////////////////////////////////////////////////////
//【機能】DOUBLE型データのファイル書き込み
//【返値】==  0 : 正常終了
//		  ≠  0 : その他エラー

MINT MhMdlIO::Write(
				const	MINT		i_tp,			// ﾀｲﾌﾟ
				const	double*		i_rbf,			// 書込み実数配列
				const	MINT		i_iwd			// 書込みワード数
				)
{
	MINT		ist;
	ist = -1;
	m_File.Write( &i_tp, sizeof(MINT));							// ﾀｲﾌﾟ
	m_File.Write( &i_iwd, sizeof(MINT));						// ｻｲｽﾞ
	m_File.Write( i_rbf, i_iwd * sizeof(double));				// 実数配列
	ist = 0;
	return( ist);
}

//////////////////////////////////////////////////////////////////////////////
////【機能】BYTE型データのファイル書き込み
////【返値】==  0 : 正常終了
////		  ≠  0 : その他エラー
//
//MINT MhMdlIO::Write(
//				const	MINT		i_tp,			// ﾀｲﾌﾟ
//				const	MBYTE*		i_ibf,			// 書込みBYTE配列
//				const	MINT		i_ibt			// 書込みBYTE型バイト数
//				)
//{
//	MINT		ist;
//	ist = -1;
//	m_File.Write( &i_tp, sizeof(MINT));							// ﾀｲﾌﾟ
//	m_File.Write( &i_ibt, sizeof(MBYTE));						// ｻｲｽﾞ
//	m_File.Write( i_ibf, i_ibt * sizeof(MBYTE));				// 整数配列
//	ist = 0;
//	return( ist);
//}

////////////////////////////////////////////////////////////////////////////
//【機能】文字(char)型データのファイル書き込み
//【返値】==  0 : 正常終了
//		  ≠  0 : その他エラー

MINT MhMdlIO::Write(
				const	MINT		i_tp,			// ﾀｲﾌﾟ
				const	char*		i_cDat,			// 書込み文字列
				const	MINT		i_sz			// 書込み文字数
				)
{
	MINT		ist;
	ist = -1;
	m_File.Write( &i_tp, sizeof(MINT));							// ﾀｲﾌﾟ
	m_File.Write( &i_sz, sizeof(MINT));							// ｻｲｽﾞ
	m_File.Write( i_cDat, i_sz);								// 文字列
	ist = 0;
	return( ist);
}

////////////////////////////////////////////////////////////////////////////
//【機能】文字(_TCHAR)型データのファイル書き込み
//【返値】==  0 : 正常終了
//		  ≠  0 : その他エラー

MINT MhMdlIO::Write(
				const	MINT		i_tp,			// ﾀｲﾌﾟ
				const	_TCHAR*		i_cDat,			// 書込み文字列
				const	MINT		i_sz			// 書込み文字数
				)
{
	MINT		ist;
	ist = -1;
	m_File.Write( &i_tp, sizeof(MINT));							// ﾀｲﾌﾟ
	m_File.Write( &i_sz, sizeof(MINT));							// ｻｲｽﾞ
	m_File.Write( i_cDat, i_sz * sizeof(_TCHAR));				// 文字列
	ist = 0;
	return( ist);
}

////////////////////////////////////////////////////////////////////////////
//【機能】文字(_TCHAR)型データのファイル書き込み
//【返値】==  0 : 正常終了
//		  ≠  0 : その他エラー

MINT MhMdlIO::Write(
				const	MINT		i_tp,			// ﾀｲﾌﾟ
				const	CString*	i_sDat,			// 書込み文字列
				const	MINT		i_sz			// 書込み文字数
				)
{
	MINT		ist;
	ist = -1;
	m_File.Write( &i_tp, sizeof(MINT));							// ﾀｲﾌﾟ
	m_File.Write( &i_sz, sizeof(MINT));							// ｻｲｽﾞ
	m_File.Write( i_sDat, i_sz * sizeof(_TCHAR));				// 文字列
	ist = 0;
	return( ist);
}

////////////////////////////////////////////////////////////////////////////
//	ファイルダイアログを表示しフルパスを取得する
//	[返値] ステイタス     0 : キャンセル  1 : ＯＫ

MINT IeModel::MhFileDialog(									//
				const	MINT		fRead,			// (I  ) 読み取りフラグ		true:開く	false:保存
				const	MCHAR*		cPathI,			// (I  ) 指定ファイル名（フルパス）
						MCHAR*		cPathO)			// (  O) 選択ファイル名（フルパス）
{
	MINT			ist;
	CFileDialog*	DlgFile = NULL;
	MCHAR			cFldPath[MX_PATH];
	MCHAR			cFileName[MX_PATH];
	MCHAR*			pcFileName = NULL;


	ist = 0;

	Mstrcpy( cPathO, Mstr( ""));

	DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ENABLESIZING;

	if ( cPathI) {																		// 指定ファイル名あり
		IeModel::MhSeparateFname( cPathI, cFldPath, cFileName, (MCHAR*)NULL, (MCHAR*)NULL);
		pcFileName = cFileName;
	}

	CFileDialog FDlg( fRead,  Mstr( "mdk"), pcFileName, dwFlags,
						 	    Mstr( "構造家モデルファイル(*.mdk)|*.mdk|全ファイル(*.*)|*.*||"), NULL);
	if ( cPathI)
		FDlg.m_ofn.lpstrInitialDir = cFldPath;

	if ( FDlg.DoModal() == IDOK) {													// ダイアログを表示する
		Mstrcpy( cPathO, FDlg.GetPathName());
		ist = 1;
	}
	return( ist);
}

////////////////////////////////////////////////////////////////////////////
//	フルパス名から、フォルダーパス名、ファイル名、ファイルタイトル、
//					ファイル拡張子を取得する
void IeModel::MhSeparateFname(
				const	MCHAR*		cFullPath,		// (I  ) ファイルフルパス名
						MCHAR*		cFolderPath,	// (  O) フォルダーパス名	または　NULL
						MCHAR*		cFileName,		// (  O) ファイル名			または　NULL
						MCHAR*		cFileTitle,		// (  O) ファイルタイトル	または　NULL
						MCHAR*		cFileExt		// (  O) ファイル拡張子		または　NULL
				)
{
	MINT		ist = 0;
	MINT		ic;
	MINT		szFullPath;
	MINT		szPath;
	MINT		szName;
	MINT		szTitle;

	szFullPath = (MINT)Mstrlen( cFullPath);
	for ( ic=szFullPath-1; ic>=0; ic--) {
		if ( cFullPath[ic] == Mchar('\\'))
			break;
	}
	szPath = ic;
	if ( cFolderPath) {
		if ( szPath > 1) {
			Mstrncpy( cFolderPath, cFullPath, szPath);
			cFolderPath[szPath] = NULL;
		} else {
			Mstrcpy( cFolderPath, Mstr( ".\\"));
		}
	}
	if ( cFileName)
		Mstrcpy( cFileName, &cFullPath[szPath+1]);
	
	szName = szFullPath - szPath - 1;
	if ( cFileTitle || cFileExt) {
		for ( ic=szName-1; ic>=0; ic--) {
			if ( cFileName[ic] == Mchar('.'))
				break;
		}
		szTitle = ic;
		if ( cFileTitle) {
			if ( szTitle < 0) {
				Mstrcpy( cFileTitle, cFileName);
			} else {
				if ( szTitle)
					Mstrncpy( cFileTitle, cFileName, szTitle);
				cFileTitle[szTitle] = NULL;
			}
		}
		if ( cFileExt) {
			if ( szTitle < 0) {
				cFileExt[0] = NULL;
			} else {
                Mstrcpy( cFileExt, &cFileName[szTitle+1]);
			}
		}
	}
}

} // namespace MC
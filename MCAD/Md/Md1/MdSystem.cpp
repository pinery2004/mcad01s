//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MdSystem.cpp
//
//		システム
//
//
//  K.Matsu           01/06/08    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"
#include "MgDefine.h"
#include "MgPoint.h"
#include "MgRect.h"
#include "MgPolygon.h"
#include "MgMat.h"

#include "MdModel.h"

namespace MC
{

//===========================================================================
//		データベースオープン
//
//		指定されたモードにより、AUZファイル名をオープンする。
//		既存ファイルの場合は、保存されている環境で画面が表示される。
//		オープンをCALLする前に、カレントDBをクローズしておくこと。
//
//		オープンモードは、指定データベースファイルへのアクセスモード。
//		シェアモードは、指定データベースファイルへの共有モードで、本関数でオープン後、
//		CloseAuzModelでクローズするまでの間のアクセスを制限する。
//
//		＜補足＞
//		・リードオンリーでシェアする場合
//			1つ目のオープン時に
//			オープンモードを”READ”、シェアモードを”READ”を指定し、
//
//			2つ目のオープン時にも同様に
//			オープンモードを”READ”、シェアモードを”READ”を指定するとシェアエラーにならない。
//
//		・リードライトでシェアする場合
//			1つ目のオープン時に
//			オープンモードを”OLD”、シェアモードを”WRITE”を指定し、
//
//			2つ目のオープン時にも同様に
//			オープンモードを”OLD”、シェアモードを”WRITE”を指定するとシェアエラーにならない。
//
MINT	MdsOpenAuzModel (							// ステータス
												//	0: 正常
												//	2: 引数不正
												//	3: データベースがクローズされていない
												//	4: NEWTOでファイルが存在した
												//	5: OLD、READでファイルが存在しない
												//	6: シェアエラー
												//	7: その他オープンエラー

						MCHAR*		i_sFName,	// データベースファイル名
						MCHAR*		i_smdOpen,	// オープンモード
												//	NEWTO     新規（ファイルが存在すればエラー）
												//	NEWOVER  新規（ファイルが存在すれば削除）
												//	OLD　      既存（READ/WRITEでアクセス）
												//	READ       既存（READ-ONLYでアクセス）
						MCHAR*		i_smdhare	// シェアモード
												//	NON　　　　不可
												//	READ　　　READのみ可
												//	WRITE  　　READ/WRITE可
				)
{
	return 0;
}

//
//===========================================================================
//		データベース上書き保存
//
//		オープン中のデータベースファイルを上書き保存する。
//
MINT	MdsSaveAuzModel ()							// ステータス
												//	0:正常
												//	7:ファイル名未定（テンポラリオープン中）
												//	8:書き込み不可（READ-ONLY）
												//	その他:書き込みエラー
{
	return 0;
}

//
//===========================================================================
//		データベース名前を付けて保存
//
//		オープン中のデータベースファイルに名前を付けて保存する。
//
MINT	MdsSaveasAuzModel  (						// ステータス
												//	0: 正常
												//	2: モード不正
												//	4: TOでファイルが存在した
												//	8: 書き込みエラー
												//	その他:内部エラー
						MCHAR*		i_sFName,	// データベースファイル名
						MCHAR*		i_smdSave	// モード
												//	TO		ファイルが存在すればエラー
												//	OVER	ファイルが存在すれば削除
				)
{
	return 0;
}

//
//===========================================================================
//		データベースクローズ
//
//		オープン中のデータベースファイルをクローズする。
//
MINT	MdsCloseAuzModel (							// ステータス
												//　0:正常
												//　2:モード不正
												// その他:内部エラー
						MCHAR*		i_smdClose	// クローズモード
												//	CANCEL   キャンセル
												//	OVER　      保存
				)
{
	return 0;
}

//
//===========================================================================
//		環境設定の識別文字列からパス文字列を得る
//
//		環境設定の識別文字列からパス文字列を得る。
//		初期値は、システムディレクトリーにあるauz.sysファイルに記述してある。
//
MINT	MdsGetEnvPath  (						// ステータス
												//	0: 正常
												//	2: モード不正
												//	4: TOでファイルが存在した
												//	8: 書き込みエラー
												//	その他:内部エラー
						MCHAR*		i_sKey[],	// 識別文字列
						MCHAR*		o_sPath[],	// パス文字列
						MINT		i_szPath	// パス文字列バッファサイズ
				)
{
	return 0;
}

//
//===========================================================================
//		環境設定に識別文字列とパス文字列を設定する
//
//		環境設定の識別文字列に対するパス文字列を設定する。
//		識別文字列は、１６バイトまでとする。
//		パス文字列は２６０バイトまでとする。
//
MINT	MdsSetEnvPath  (						// ステータス
												//	0: 正常
												//	2: モード不正
												//	4: TOでファイルが存在した
												//	8: 書き込みエラー
												//	その他:内部エラー
						MCHAR*		i_sKey[],	// 識別文字列
						MCHAR*		i_sPath[]	// パス文字列
				)
{
	return 0;
}

//
//===========================================================================
//		環境設定の識別文字列からパス文字列を得る
//
//		環境設定の識別文字列からパス文字列を得る。
//		初期値は、システムディレクトリーにあるauz.sysファイルに記述してある。
//
MINT	MdsGetTitle  (						// ステータス
												//	0: 正常
												//	2: モード不正
												//	4: TOでファイルが存在した
												//	8: 書き込みエラー
												//	その他:内部エラー
						MCHAR*		i_sKey[],	// 識別文字列
						MCHAR*		o_sPath[],	// パス文字列
						MINT		i_szPath	// パス文字列バッファサイズ
				)
{
	return 0;
}

//
//===========================================================================
//		タイトル文字列の設定
//
//		ウィンドウタイトルに表示する文字列を設定する。
//		現在のインデックスは
//		　　　　0:モジュール名
//		　　　　1:データベースファイル名
//		　　　　2:カンレトグループ名
//		　　　　3:カンレトレイヤー名
//		で利用している。
//
MINT	MdsSetTitle  (								// ステータス
												//	0:正常
												//　2:tidx不正
												//	その他:内部エラー
						MINT		i_szPath,	// タイトルインデックス
						MCHAR*		o_sPath[]	// タイトル文字列
				)
{
	return 0;
}

//
//===========================================================================
//		タイトル文字列の表示
//
//		指定されているタイトル文字を合成して表示する。
//
MINT	MdsDispTitle  ()
{
	return 0;
}

//
//===========================================================================
//		データベースファイル名を得る
//
//		現在オープンされているデータベースファイル名を得る。
//
MINT	MdsGetAuzModelName  (								// ステータス
												//	0:正常
												//	1:バッファーオーバフロー-
						MCHAR*		o_sPath[]	// データベースファイル名
				)
{
	return 0;
}

//
//===========================================================================
//		システムディレクトリの問い合わせ
//
//		システムディレクトリを問い合わせる。
//
MINT	MdsGetExeDir  (							// ステータス
												//	0:正常
												//	1:バッファーオーバフロー-
						MCHAR*		o_sPath[]	// システムディレクトリパス
				)
{
	return 0;
}

//
//===========================================================================
//		詳細エラーコードを得る
//
MINT	MdsGetErr  (								// 詳細エラーコード
												//	0:正常
												//	1:バッファーオーバフロー-
						MCHAR*		o_sPath[]	// システムディレクトリパス
				)
{
	return 0;
}

} // namespace MC
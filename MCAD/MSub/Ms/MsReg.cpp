////////////////////////////////////////////////////////////////////////////
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MsReg.cpp
//
//		
//
//
//  K.Matsu           08/01/04    Created.
#include "stdafx.h"
#include "MbBasic.h"
#include	"MsLib.h"
#include	<atlbase.h>

#define			HOLDNUM			4

namespace MC
{

static MCHAR* HOLDER[HOLDNUM] = { Mstr("データ/属性"),								// 0
								  Mstr("データ/図形"),								// 1
								  Mstr("データ/テーブル"),							// 2
								  Mstr("データ/テキスチャ") };						// 3

static MCHAR*	RegSub = Mstr( "SOFTWARE\\KMCreate\\Home_Designer\\Model");
static MCHAR*	SubKey = Mstr( "Path");
//HHHH
////////////////////////////////////////////////////////////////////////////
//	プロセスを起動し終了するまで待つ。
MINT MsProcXqt(									// ｽﾃｰﾀｽ	0 : プロセスを実行し終了   -1 : プロセスの実行に失敗
						MCHAR*	i_AplName,		// 実行するﾓｼﾞｭｰﾙ名
						MCHAR*	i_cCmdLine)		// ｱﾌﾟﾘｹｰｼｮﾝに渡すｺﾏﾝﾄﾞﾗｲﾝ
{
	MINT		Status;							// ｽﾃｰﾀｽ
	STARTUPINFO	StatInf;						// ﾌﾟﾛｾｽのﾒｲﾝｳｨﾝﾄﾞｳの表示情報構造体
	PROCESS_INFORMATION	ProcInf;				// ﾌﾟﾛｾｽに関する情報構造体
	BOOL		BolSts;							// CreateProcesss関数のｽﾃｰﾀｽ
	HANDLE		HProc = NULL;					// 新しいﾌﾟﾛｾｽのﾊﾝﾄﾞﾙ
	DWORD		ItrSts;							// ｼｸﾞﾅﾙ結果
	DWORD		ExitFlg;						// ﾌﾟﾛｾｽの終了状態

	Status = 0;
	memset(&StatInf, 0, sizeof(StatInf));
	memset(&ProcInf, 0, sizeof(ProcInf));

	// プロセスの作成
	if ( ( BolSts = CreateProcess( i_AplName, i_cCmdLine, 0, 0, false, NORMAL_PRIORITY_CLASS,
								   0, 0, &StatInf, &ProcInf)) == false) {
		Status = -1;
		goto  PRG_EXIT;
	}
	HProc = ProcInf.hProcess;															// ﾌﾟﾛｾｽﾊﾝﾄﾞﾙのｾｯﾄ

	do {
		while ( ( ItrSts = WaitForSingleObject( HProc, 200)) == WAIT_TIMEOUT);				// 待機
		GetExitCodeProcess( HProc, &ExitFlg);

	} while ( ExitFlg == STILL_ACTIVE);													// 終了まで待つ

	if ( HProc != 0)  CloseHandle(HProc);												// ﾊﾝﾄﾞﾙをｸﾛｰｽﾞする

PRG_EXIT:

	return( Status);
}

////////////////////////////////////////////////////////////////////////////
//		環境変数を得る
MINT MsGetenv(									// ｽﾃｰﾀｽ	0 : 正常   -1 : エラー
						MCHAR*	i_cEnvNam,		// 環境変数名
						MCHAR*	o_cEnvVal)		// 環境変数の値
{
	MINT		iSt;							// ｽﾃｰﾀｽ
	MCHAR*		cEnvPtr;						// 環境変数ﾎﾟｲﾝﾀ


	iSt = -1;

	cEnvPtr = _tgetenv( i_cEnvNam);
	if ( cEnvPtr == NULL)  goto  PRG_EXIT;

	Mstrcpy( o_cEnvVal, cEnvPtr);
	iSt = 0;

PRG_EXIT:

	return iSt;
}

////////////////////////////////////////////////////////////////////////////
//		環境変数を加えたパス名を得る
MINT MsAddEnvNam(								// ｽﾃｰﾀｽ	0 : 正常   -1 : エラー
						MINT	i_iType,		// 0;属性 1:図形 2:ﾃｰﾌﾞﾙ 3:ﾃｷｽﾁｬ -1:HTEMP
						MCHAR*	i_cFilNam,		// ﾌｧｲﾙ名
						MCHAR*	o_cPasNam)		// 環境変数を加えたﾊﾟｽ名
{
	MINT		Status;							// ｽﾃｰﾀｽ
	MINT		Leng;
	MINT		ItrSts;


	Status = -1;

	if ( i_iType >= 0 && i_iType < HOLDNUM) {
		ItrSts = MsGetenv( Mstr("MCAD"), o_cPasNam);
		if ( ItrSts != 0)  Mstrcpy( o_cPasNam, Mstr("E:/MCAD"));

		Leng = (MINT)Mstrlen( o_cPasNam);													// 文字数
		if ( Mstrncmp( &o_cPasNam[Leng-1], Mstr("/"), 1) != 0)  Mstrcat( o_cPasNam, Mstr("/"));

		Mstrcat( o_cPasNam, HOLDER[i_iType]);

		if ( i_cFilNam) {
			Mstrcat( o_cPasNam, Mstr("/"));
			Mstrcat( o_cPasNam, i_cFilNam);
		}
	} else if ( i_iType == -1) {
		ItrSts = MsGetenv( Mstr("MTEMP"), o_cPasNam);
		if ( ItrSts != 0)  Mstrcpy( o_cPasNam, Mstr("C:/TEMP"));

		Leng = (MINT)Mstrlen( o_cPasNam);													// 文字数
		if ( Mstrncmp( &o_cPasNam[Leng-1], Mstr("/"), 1) != 0)  Mstrcat( o_cPasNam, Mstr("/"));

		if ( i_cFilNam)  Mstrcat( o_cPasNam, i_cFilNam);
	}

	Status = 0;

	return(Status);
}

////////////////////////////////////////////////////////////////////////////
//		レジストリの登録
MINT MsSetRegistry( void)						// (  O) ｽﾃｰﾀｽ	     0 : 正常   -1 : エラー
{
	MINT		Status;							// ｽﾃｰﾀｽ
	CRegKey		RKey;
	MINT		ItrSts;


	Status = -1;

	ItrSts = RKey.Open( HKEY_LOCAL_MACHINE, RegSub);
	if (ItrSts == ERROR_SUCCESS) {														// 既にあり
		RKey.Close();
		Status = 0;
	} else {
		ItrSts = RKey.Create(HKEY_LOCAL_MACHINE, RegSub); 
		if (ItrSts == ERROR_SUCCESS) {
			RKey.SetValue(Mstr( ""), SubKey);
//			RKey.SetKeyValue( SubKey, Mstr(""));
			RKey.Close();
			Status = 0;
		}
	}

	return(Status);
}

////////////////////////////////////////////////////////////////////////////
//		レジストリよりパスを取得
MINT MsGetRegistryPath(							// (  O) ｽﾃｰﾀｽ	0 : 正常   -1 : エラー
						MCHAR*	PName)			// (  O) ﾊﾟｽﾎﾙﾀﾞｰ
{
	MINT		Status;							// ｽﾃｰﾀｽ
	CRegKey		RKey;
	MCHAR		NameW[MX_PATH];
	MINT		Size;
	MINT		ItrSts;

	Status = -1;
	Mstrcpy(NameW, Mstr(""));

	ItrSts = RKey.Open( HKEY_LOCAL_MACHINE, RegSub, KEY_READ);
	if ( ItrSts == ERROR_SUCCESS) {														// あり
		Size = sizeof( NameW);
		RKey.QueryValue( NameW, SubKey, (MULONG *)&Size);
		if ( ItrSts == ERROR_SUCCESS)  Status = 0;
		RKey.Close();
	}
	Mstrcpy( PName, NameW);

	return( Status);
}

////////////////////////////////////////////////////////////////////////////
//		レジストリにパスを設定
MINT MsSetRegistryPath(							// (  O) ｽﾃｰﾀｽ	0 : 正常   -1 : エラー
						MCHAR*	PName)			// (I  ) ﾊﾟｽﾎﾙﾀﾞｰ
{
	MINT		Status;							// ｽﾃｰﾀｽ
	CRegKey		RKey;
	MINT		ItrSts;


	Status = -1;

	ItrSts = RKey.Open( HKEY_LOCAL_MACHINE, RegSub);
	if ( ItrSts == ERROR_SUCCESS) {														// あり
		RKey.SetValue( PName, SubKey);
		RKey.Close();
		Status = 0;
	}

	return(Status);
} 

} // namespace MC
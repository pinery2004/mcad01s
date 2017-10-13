//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MhPts.cpp
//
//		部材マスタを読み込む
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"

#include "MgLib.h"

#include "MmDefine.h"
#include "MsBitSet.h"
#include "MmOpt.h"
#include "MmHist.h"

#include "MbList.h"
#define DLL_EXPORT_BUZAICODE_DO
#include "MhLib.h"

#include "MhPlcInfo.h"

#include "MmLib.h"

#include "MgGrp.h"

#include "MhRoof.h"
#include "MdLib.h"

#include "MhPts1.h"
#include "MbFile.h"
#include "MmSystemProperty.h"

namespace MC
{

void MhPlcInfo::Copy( const MhPlcInfo &Ent)
{
	MINT		szZukei;
	MINT		szVer;
	MINT		szOpt1;
	MINT		szOpt2;
	MhZukei*	pZukei;
	MsBitSet*	pVer;
	MsBitSet*	pOpt1;
	MsBitSet*	pOpt2;

	MBFREE( m_pZukei);
	MBFREE( m_pOpt1);
	MBFREE( m_pOpt2);

	*this = Ent;
	if ( Ent.m_pZukei) {
//		szZukei = sizeof( *Ent.m_pZukei) + sizeof( MgLine3) * (Ent.m_pZukei->m_nZukei - 1);
		szZukei = sizeof( MhZukei) + sizeof( MgLine3) * (Ent.m_pZukei->m_nZukei - 1);
		pZukei = (MhZukei*)new char[szZukei];
		memcpy( pZukei, Ent.m_pZukei, szZukei);
		m_pZukei = pZukei;
	} else {
		m_pZukei = NULL;
	}
	if ( Ent.m_pVer) {
		szVer = sizeof( MsBitSet);
		pVer = (MsBitSet*)new char[szVer];
	
#ifdef _DEBUG
		Msprintf( MbLog::m_Str, Mstr( "MALLOC VERSION TRC %x %x %x\n"), pVer->GetSOB()[0], pVer->GetSOB()[1], pVer->GetSOB()[2]);
		MBTRCPRBF;
#endif

		memcpy( pVer, Ent.m_pVer, szVer);
		m_pVer = pVer;
	} else {
		m_pOpt2 = NULL;
	}
	if ( Ent.m_pOpt1) {
		szOpt1 = sizeof( MsBitSet);
		pOpt1 = (MsBitSet*)new char[szOpt1];

#ifdef _DEBUG
		Msprintf( MbLog::m_Str, Mstr( "MALLOC OPTION Copy1 TRC %x %x %x\n"), pOpt1->GetSOB()[0], pOpt1->GetSOB()[1], pOpt1->GetSOB()[2]);
		MBTRCPRBF;
#endif

		memcpy( pOpt1, Ent.m_pOpt1, szOpt1);
		m_pOpt1 = pOpt1;
	} else {
		m_pOpt1 = NULL;
	}
	if ( Ent.m_pOpt2) {
		szOpt2 = sizeof( MsBitSet);
		pOpt2 = (MsBitSet*)new char[szOpt2];

#ifdef _DEBUG
		Msprintf( MbLog::m_Str, Mstr( "MALLOC OPTION Copy2 TRC %x %x %x\n"), pOpt2->SOB(0), pOpt2->SOB(1), pOpt2->SOB(2));
		MBTRCPRBF;
#endif

		memcpy( pOpt2, Ent.m_pOpt2, szOpt2);
		m_pOpt2 = pOpt2;
	} else {
		m_pOpt2 = NULL;
	}

	return;
}					// コピー

////////////////////////////////////////////////////////////////////////////
//    部材マスタ

static	MINT		z_nGp;
static	MhGp		z_Gp[MMAX_KUMI];
static	MINT		z_nTpPts;
static	MhTpPts		z_TpPts[MMAX_PARTSTP];
static	MINT		z_nMbr;
static	MhMbr		z_Mbr[MMAX_MEMBER];
struct	BZMTBK
{
	MCHAR*	c;
	MINT	i;
};
static	const	BZMTBK	z_Br[] =	
{
	Mstr( "部材"),				MP_BR_BUZAI,
	Mstr( "金物"),				MP_BR_KANAGU,
	Mstr( "パネル"),			MP_BR_PANEL,
	Mstr( "屋根"),				MP_BR_YANE,
	Mstr( "壁"),				MP_BR_KABE,
	Mstr( "基礎"),				MP_BR_KISO,
	Mstr( "線分"),				MP_BR_SENBUN,
	Mstr( "区画"),				MP_BR_KUKAKU,
	Mstr( "開口"),				MP_BR_KAIKO,
	Mstr( "注記"),				MP_BR_CYUKI,
	Mstr( "その他"),			MP_BR_OTHER
};

#define	NBUNRUI		(sizeof(z_Br)/sizeof(BZMTBK))
static	const	BZMTBK	z_InpKb[] =
{
//	Mstr( "無指定"),			MP_INPKB_NONE,
	Mstr( "１点"),				MP_INPKB_1PT,
	Mstr( "方向１点"),			MP_INPKB_DIR1PT,
	Mstr( "長さ２点"),			MP_INPKB_LEN2PT,
	Mstr( "区画"),				MP_INPKB_AREA,
	Mstr( "自由"),				MP_INPKB_FREE
};
#define	NInpKb	(sizeof(z_InpKb)/sizeof(BZMTBK))
static	const	BZMTBK	z_CdHgt[] =
{
	Mstr( "SGL"),				MP_HGTCD_SGL,			//ZZZZZ
	Mstr( "基礎"),				MP_HGTCD_KISOKJ,
	Mstr( "下基準"),			MP_HGTCD_SITAKJ,
	Mstr( "上基準"),			MP_HGTCD_UEKJ,
	Mstr( "天井基準"),			MP_HGTCD_TENJOKJ,
	Mstr( "屋根基準"),			MP_HGTCD_YANEKJ
};
#define	NHEIGHTCD	(sizeof(z_CdHgt)/sizeof(BZMTBK))
static	const	BZMTBK	z_CdPlc[] =
{
	Mstr( "任意"),				MP_HAICD_NINI,
	Mstr( "壁芯付き"),			MP_HAICD_KABESINZUKE,
	Mstr( "屋根構成線付き"),	MP_HAICD_YANEKOUSEISENZUKE
};
#define	NHAITICD	(sizeof(z_CdPlc)/sizeof(BZMTBK))
/*
static	const	MCHAR*	z_cCdIzon[] =
{
	Mstr( "無し"),
	Mstr( "水平"), Mstr( "垂直"), Mstr( "天井根太上"),
	Mstr( "屋根面なり"), Mstr( "屋根面なり垂直"), Mstr( "垂木下"),
	Mstr( "垂木下屋根面平行"), Mstr( "垂木下垂直"), Mstr( "軒先なり")
};
static	const	MINT	z_iCdIzon[] =
{
	MP_IZNCD_N, MP_IZNCD_SUIHEI, MP_IZNCD_SUICYOKU, MP_IZNCD_TENJONETAUE,
	MP_IZNCD_YANEMENNARI, MP_IZNCD_YANEMENNARISUICYOKU, MP_IZNCD_TARUKISITA,
	MP_IZNCD_TARUKISITAYANEMENHEIKOU, MP_IZNCD_TARUKISITASUICYOKU, MP_IZNCD_NOKISAKINARI 
};
*/										  
static	const	BZMTBK	z_CdIzon[] =
{
	Mstr( "無し"),				MP_IZNCD_N,
	Mstr( "水平"),				MP_IZNCD_SUIHEI,
	Mstr( "垂直"),				MP_IZNCD_SUICYOKU,
	Mstr( "天井根太上"),		MP_IZNCD_TENJONETAUE,
	Mstr( "屋根面なり"),		MP_IZNCD_YANEMENNARI,
	Mstr( "屋根面なり鉛直"),	MP_IZNCD_YANEMENNARIENCYOKU,
	Mstr( "屋根面なり垂直"),	MP_IZNCD_YANEMENNARISUICYOKU,
	Mstr( "屋根構成線なり"),	MP_IZNCD_YANEKOSEISENNARI,
	Mstr( "垂木下"),			MP_IZNCD_TARUKISITA,
	Mstr( "垂木下屋根面平行"),
								MP_IZNCD_TARUKISITAYANEMENHEIKOU,
	Mstr( "垂木下垂直"),		MP_IZNCD_TARUKISITASUICYOKU,
	Mstr( "軒先なり"),			MP_IZNCD_NOKISAKINARI
};
#define	NIZONCD	(sizeof(z_CdIzon)/sizeof(BZMTBK))
static	const	BZMTBK	z_CdMarume[] =
{
	Mstr( "無し"),				MP_MRMCDN,
	Mstr( "Ｍ１"),				MP_MRMCD1,
	Mstr( "Ｍ２"),				MP_MRMCD2,
	Mstr( "Ｍ３"),				MP_MRMCD3,
	Mstr( "Ｍ４"),				MP_MRMCD4,
	Mstr( "Ｍ６"),				MP_MRMCD6,
	Mstr( "Ｍ８"),				MP_MRMCD8
};
#define	NMARUMECD	(sizeof(z_CdMarume)/sizeof(BZMTBK))

////////////////////////////////////////////////////////////////////////////
//    コード変換
//		return :    -1: 入力コードエラー　≧0: ID

static MINT GetCd( const MCHAR* cCd, MINT nCd, const BZMTBK* pCd)
{
	MINT	ic;
	MINT	iCd = -1;
	for ( ic=0; ic<nCd; ic++) {
		if ( Mstrcmp( cCd, pCd[ic].c) == 0) {
			iCd = (MSTNDH)ic;
			break;
		}
	}
//	ASSERT( iCd >= 0);										// テーブル　変換エラー　<ERROR>
	if ( iCd >= 0)
		iCd = pCd[iCd].i;
	return iCd;
}

////////////////////////////////////////////////////////////////////////////
//    エラー表示
//		return :    -1: 入力コードエラー　≧0: ID
void GetCdErrorMsg(
						MCHAR*	sTable,				// (I  ) テーブル名
						MCHAR*	sLineNo,			// (I  ) 行番号
						MCHAR*	sKoumoku,			// (I  ) 項目名
						MCHAR*	sCont				// (I  ) 項目内容
				)
{
	CString ErrorMsg;
	ErrorMsg.Format(Mstr( "テーブル(%s)の行番号%s 項目(%s)の%sが未登録です"), 
							sTable, sLineNo, sKoumoku, sCont);
	MessageBox( NULL, ErrorMsg, Mstr( "テーブル読込みエラー"), MB_OK);
}

////////////////////////////////////////////////////////////////////////////
//    部材マスタを読み込む

void MhLoadPtsMst()
{
	MINT		ist;
	MINT		ic1;
	MINT		ik;

	MCHAR		TablePath[MX_PATH];
	MBHZDT		csvTbl;

	MhGp		*MKM;							// 構成レコード
	MhTpPts		*MPT;							// 部材種類レコード
	MhMbr		*MBR;							// 寸法型式レコード

	MCHAR		CmntF[2];
	MCHAR		cLineNo[3];
	MCHAR		cGp[16];
	MCHAR		cBr[16];
	MCHAR		cInpKb[8];
	MCHAR		cCdHgt[8];
	MCHAR		cCdPlc[10];
	MCHAR		cCdIzon[10];
	MCHAR		cCdMarume[8];
	MCHAR		cCdToritk[2];

	//	構成（組）テーブルの読み込み

//S	Mstrcpy( TablePath, MmSystemProperty::GetPath( MM_PATH_PARTS_FLDR));
//S	Mstrcat( TablePath, Mstr( "構成.csv"));
	MmSystemProperty::GetEnvPath( MP_PATH_PARTSTABLE, MmSystemProperty::GetStr( MM_STR_CSV_KOUSEI), TablePath);

	ist = csvTbl.Open( TablePath);
	ASSERT( ist == 0);											// 構成テーブル　読み込み開始エラー　<ERROR>

	for ( ic1=1 ; ic1<MMAX_KUMI && ist!=MBEOF; ) {
		ist = csvTbl.Head( CmntF, 1);
		if ( ist == MBEOF) break;
		if ( *CmntF == '#') continue;

		MKM = &z_Gp[ic1];
		MKM->SetCdGp( ic1);
		ist = csvTbl.GetStr( MKM->GetNmGp(), MHKSSZ_NMKOSEI);			// ・構成（組）設定
		ist = csvTbl.GetStr( MKM->GetNmFusezu(), MHKSSZ_NMFUSEZU);		// ・伏図名設定
		if ( ist == MBEOF) break;
		ic1++;
	}
	ASSERT( ic1 <= MMAX_KUMI);									// 構成テーブル　オーバフロー　<ERROR>
	z_nGp = ic1;

	ist = csvTbl.Close();

	//	部品種類テーブルの読み込み

//S	Mstrcpy( TablePath, MmSystemProperty::GetPath( MM_PATH_PARTS_FLDR));
//S	Mstrcat( TablePath, Mstr( "部品.csv"));
	MmSystemProperty::GetEnvPath( MP_PATH_PARTSTABLE, MmSystemProperty::GetStr( MM_STR_CSV_PARTS), TablePath);

	ist = csvTbl.Open( TablePath);
	ASSERT( ist == 0);											// 部品種類テーブル　読み込み開始エラー　<ERROR>

	for ( ic1=0 ;ic1<MMAX_PARTSTP && ist!=MBEOF; ) {
		ist = csvTbl.Head( CmntF, 1);
		if ( ist == MBEOF) break;
		if ( *CmntF == '#') continue;									// '#'　コメント

		MPT = &z_TpPts[ic1];											// 部品種類レコード

		ist = csvTbl.GetStr( cLineNo, Msizeof( cLineNo));				// ・行番号　読み飛ばし
		ist = csvTbl.GetStr( cGp, Msizeof( cGp));						// ・構成コード設定
		if ( ist == MBEOF) break;
		for ( ik=1; ik<z_nGp; ik++) {
			if ( Mstrcmp( cGp, z_Gp[ik].GetNmGp()) == 0) {
				MPT->SetPTGp( &z_Gp[ik]);
//				MPT->m_iGp = ik;
				break;
			}
		}
		ASSERT( ik < z_nGp);											// 部品種類テーブル　構成(組)エラー　<ERROR>　print

		ist = csvTbl.GetStr( cBr, Msizeof( cBr));						// ・分類コード設定
		if ( ist == MBEOF) break;
		MPT->SetPTBr( GetCd( cBr, NBUNRUI, z_Br));	
		ist = csvTbl.GetStr( MPT->GetPTNmGeneral(), MHPTSZ_NMGENERAL);	// ・総称設定
		ist = csvTbl.GetStr( MPT->GetPTNmPts1(), MHPTSZ_NMPARTS1);		// ・操作用部材名設定
		ist = csvTbl.GetStr( MPT->GetPTNmPts2(), MHPTSZ_NMPARTS2);		// ・積算用部材名設定
		MINT iPTColor;		ist = csvTbl.GetInt( &iPTColor, 1);			// ・色
		MPT->SetPTColor( iPTColor); 
		MINT iPTCdBuzaiR;	ist = csvTbl.GetInt( &iPTCdBuzaiR, 1);		// ・部材コード設定(読み込み用)
		MPT->SetPTCdBuzaiR( iPTCdBuzaiR);
		MINT iPTCdBuzaiW;	ist = csvTbl.GetInt( &iPTCdBuzaiW, 1);		// ・部材コード設定(書き込み用 Ver Up に使用)
		MPT->SetPTCdBuzaiW( iPTCdBuzaiW);
		ist = csvTbl.GetStr( MPT->GetPTTpMbr(), MHPTSZ_TPMEMBER);		// ・寸法型式選択用種類コード設定
		ist = csvTbl.GetStr( MPT->GetPTCdMbr(), MHPTSZ_CDMEMBER);		// ・寸法型式コード設定
		ist = csvTbl.GetStr( cInpKb, Msizeof( cInpKb));					// ・入力コード
		MPT->SetPTInpKb( GetCd( cInpKb, NInpKb, z_InpKb));	
		ist = csvTbl.GetStr( cCdHgt, Msizeof( cCdHgt));					// ・取り付け高さコード
		MPT->SetPTCdHgt( (MSTNDH)GetCd( cCdHgt, NHEIGHTCD, z_CdHgt));								
		ist = csvTbl.GetStr( cCdPlc, Msizeof( cCdPlc));					// ・配置コード
		MPT->SetPTCdPlc( GetCd( cCdPlc, NHAITICD, z_CdPlc));							
		ist = csvTbl.GetStr( cCdIzon, Msizeof( cCdPlc));				// ・依存コード
		MPT->SetPTCdIzon( GetCd( cCdIzon, NIZONCD, z_CdIzon));								
		ist = csvTbl.GetStr( cCdMarume, Msizeof( cCdMarume));			// ・丸めコード
		MPT->SetPTCdMarume( GetCd( cCdMarume, NMARUMECD, z_CdMarume));	
		ist = csvTbl.GetStr( cCdToritk, Msizeof( cCdToritk));			// ・取り付け位置コード('上':上付け(1), '下':下付け(0))
//		if ( cCdToritk[0] == MCHAR( '上'))
		if ( cCdToritk[0] == Mchar( '上'))
			MPT->SetPTCdToritk( 1);
		else	
			MPT->SetPTCdToritk( 0);
		if ( ist == MBEOF) break;
		ic1++;
	}
	ASSERT( ic1 <= MMAX_PARTSTP);										// 部品種類テーブル　オーバフロー　<ERROR>
	z_nTpPts = ic1;
	
	ist = csvTbl.Close();
	
	//	寸法型式テーブルの読み込み
//S	MCHAR		TablePath1[MX_PATH];
//S	Mstrcpy( TablePath1, MmSystemProperty::GetPath( MM_PATH_PARTS_FLDR));
//S	Mstrcat( TablePath1, _T("寸法型式.csv"));
	MmSystemProperty::GetEnvPath( MP_PATH_PARTSTABLE, MmSystemProperty::GetStr( MM_STR_CSV_MEMBER), TablePath);

	ist = csvTbl.Open( TablePath);
	ASSERT( ist == 0);													// 寸法型式テーブル　読み込み開始エラー　<ERROR>
	
	for ( ic1=0; ic1<MMAX_MEMBER && ist!=MBEOF; ) {
		ist = csvTbl.Head( CmntF, 1);
		if ( ist == MBEOF) break;
		if ( *CmntF == '#') continue;

		MBR = &z_Mbr[ic1];
		ist = csvTbl.GetStr( cLineNo, Msizeof( cLineNo));				// ・行番号　読み飛ばし
		ist = csvTbl.GetStr( MBR->m_cMbCode, MHMBSZ_CODE);				// ・寸法型式設定
		ist = csvTbl.GetReal( (MREAL*)&MBR->m_rMbWidth, 1);				// ・幅設定
		ist = csvTbl.GetReal( (MREAL*)&MBR->m_rMbSZure, 1);				// ・芯ずれ量設定
		ist = csvTbl.GetReal( (MREAL*)&MBR->m_rMbHeight, 1);			// ・高さ設定
		ist = csvTbl.GetStr( MBR->m_cMbType, MHMBSZ_TYPE);				// ・寸法型式選択用種類コード設定
		ist = csvTbl.GetStr( MBR->m_cMbCodeW, MHMBSZ_CODEW);			// ・寸法型式設定　(書き込み用　通常m_cCodeと同じ VerUp時に変更した内容が入る)
		if ( ist == MBEOF) break;
		ic1++;
	}
	if ( Mstrcmp( z_Mbr[0].m_cMbCode, Mstr( "無し")) == 0)
		z_Mbr[0].m_cMbCode[0] = NULL;									// 先頭の寸法型式=="無し"はNULLに置き換える
	ASSERT( ic1 <= MMAX_MEMBER);										// 寸法型式テーブル　オーバフロー　<ERROR>
	z_nMbr = ic1;

	ist = csvTbl.Close();
}

// 部材マスターを読み込む
void IeModel::MhInitPts()
{
	MhLoadPtsMst();
}

// 組レコード数を取得する
MINT BuzaiCode::MhGetNoOfGp()
{
	return z_nGp;
}

// 組レコードを取得する
//MhGp* BuzaiCode::MhGeTpGp(
//				MINT		iGp									// 読み込み構成レコード番号
//				)
//{
//	return &z_Gp[iGp];
//}

// 部品種類レコード数を取得する
MINT BuzaiCode::MhGetNoOfTpPts()
{
	return z_nTpPts;
}

// 部品種類レコードを取得する
MhTpPts* BuzaiCode::MhGetpTpPts(
				MINT		iTpPts								// 読み込み部品種類レコード番号
				)
{
	return &z_TpPts[iTpPts];
}

// 部品IDを取得する
MINT BuzaiCode::MhGetPIIdTpPts(
						MINT	iCdBuzai						// 部材コード
				)
{
	MINT		ic1;
	MINT		iIdTpPts = -1;
	
	for ( ic1=0; ic1<z_nTpPts; ic1++) {
		if ( iCdBuzai == z_TpPts[ic1].GetPTCdBuzai()) {
			iIdTpPts = ic1;
		}
	}
	return iIdTpPts;
}

// 寸法型式レコード数を取得する
MINT BuzaiCode::MhGetNoOfMbr()
{
	return z_nMbr;
}

// 寸法型式レコードを取得する
MhMbr* BuzaiCode::MhGetpMbr(
				MINT		iMbr								// 読み込み寸法型式レコード番号
				)
{
	MhMbr*	pMbr;
	if ( iMbr >= 0)
		pMbr = &z_Mbr[iMbr];
	else 
		pMbr = &z_Mbr[0];
	return pMbr;
}

// 寸法型式IDを取得する
MINT	BuzaiCode::MhGetIdMbr(
						MCHAR*		pMbr						// 寸法型式
				)
{
	MINT		ic1;
	MINT		iMbr = -1;
	
	for ( ic1=0; ic1<z_nMbr; ic1++) {
		if ( Mstrcmp( pMbr, z_Mbr[ic1].GetMbCode()) == 0) {
			iMbr = ic1;
		}
	}
	return iMbr;
}

// 部材の２次元芯線を取得する
void	BuzaiCode::MhBziSin(
						MhPlcInfo	*pBziInfo,					// (I  ) 部材
						MgLine2		*plnBziSin					// (  O) 部材の芯線
				)
{
	MgLine2		LnPts;
	MgPoint2	ptW[2];
	MgVect2		VtW, VtUtW;
	MgVect2		VtSZ;								// 芯ずれ
	MgPoint2	ptS[2];
	
	ptW[0] = *(MgPoint2*)&(pBziInfo->GetPIPlcIti( 0));
	ptW[1] = *(MgPoint2*)&(pBziInfo->GetPIPlcIti( 1));

	// 部材の形を求め検索する
	VtW = ptW[1] - ptW[0];													// 芯線
	VtUtW = MGeo::UnitizeV2( VtW);												// 部材の形を求める
	if ( pBziInfo->IsFrame()) {												// たて枠用の形状作成
	}
	VtSZ = pBziInfo->GetPISinZure() * VtUtW.RotR90();
	ptS[0] = ptW[0] - pBziInfo->GetPILenHosei( 0) * VtUtW + VtSZ;
	ptS[1] = ptW[1] + pBziInfo->GetPILenHosei( 1) * VtUtW + VtSZ;
	*plnBziSin = MgLine2( ptS[0], ptS[1]);
}

// 配置データ　トレース
void	MhPlcInfo::Print(MCHAR* s)
{
#ifdef _DEBUG
//	Msprintf( MbLog::m_Str, Mstr( "%s 	MhPlcInfo\n", s);
	MCHAR sTrc[256], sCat[256];
	MUINT*	pVmb;

	m_lnPlc.Print( Mstr( "MhPlcInfo Line") );
//	Msprintf( MbLog::m_Str, Mstr( "%s OnOpt %x %x %x, OffOpt %x %x %x\n"), s, m_pOpt1);
	if ( m_pOpt1) {
		pVmb = m_pOpt1->GetSOB();
		Msprintf( sTrc, Mstr( "OnOpt %x %x %x"), pVmb[0], pVmb[1], pVmb[2]);
	} else {
		Mstrcpy( sTrc, Mstr( "OnOpt NULL"));
	}
	if ( m_pOpt2) {
		pVmb = m_pOpt2->GetSOB();
		Msprintf( sCat, Mstr( " OffOpt %x %x %x"), pVmb[0], pVmb[1], pVmb[2]);
	} else {
		Mstrcpy( sCat, Mstr( " OffOpt NULL"));
	}
	Mstrcat( sTrc, sCat);

	Msprintf( MbLog::m_Str, Mstr( "%s %s\n"), s, sTrc);
	MBTRCPRBF;
#endif
}

// 全配置内容をトレース
void	BuzaiCode::MhPrintallMhPlcInfo(MCHAR* s)
{
#ifdef _DEBUG
	MhPlcInfo* pPlcEn;
	MPOSITION	PtsPos;

	Msprintf( MbLog::m_Str, Mstr( "%s 	All MhPlcInfo\n"), s);
	for (pPlcEn = HaitiDb::MdGetHeadPts( &PtsPos); pPlcEn!=0; pPlcEn = HaitiDb::MdGetNextPts( &PtsPos)) {
		pPlcEn->Print( s);
	}
	MBTRCPRBF;
#endif
}

//　横置部材と横置部材の位置関係を調べる。
//　２線部材が重なっている場合は、交差なし（平行）とみなす。
//  ２線部材が重なりなく直線的に交差している場合はその交差点を交点とみなす。
MINT	MhSenBuzai::MhOn(										// (  O) ステイタス
													//	MC_PARALLEL	(-1) 交差なし（平行）
													//  MC_CONNECTION(4) 接続	 （平行）
													//	MC_TWIST    (-2) 交差なし（ねじれ）
													//	MC_NINT	    (0)	交差なし
													//	MC_INT      (1)	交差あり
				const	MhSenBuzai	&Bz1,			// (I  ) 線部材1
				const	MhSenBuzai	&Bz2,			// (I  ) 線部材2
						MgPoint3	*Po,			// (  O) 交点
						MINT		*ist1,			// (  O) 補助ステイタス1
													// 			MC_LEFT:	線部材1は線部材2の左側にあり	（交差なし）
													// 			MC_RIGHT:	線部材1は線部材2の右側にあり	（交差なし）
													//			MC_CLOSS:	線部材1は線部材2と交差			（交差あり）
													//			MC_ON_LINE:	線部材1は線部材2の直線上にあり	（平行）
						MINT		*ist2			// (  O) 補助ステイタス2
													// 			MC_PS_NEAR:	線部材1は線部材2より始点側が遠い（交差なしまたは交差あり）
													// 			MC_PE_NEAR:	線部材1は線部材2より終点側が遠い（交差なしまたは交差あり）
													// 			MC_PARALEL:	線部材1は線部材2より始点と終点は等距離	（平行）
				)
{
	MINT		ist;
	
	MgPoint3	PtInt;
	MREAL		s1, s2;								// 長さ調整指示点と部材1の部材2に対する左右位置

	MgVect3	VtUtZ( 0., 0., 1.);

	ist = MGeo::Intr2Ln3( Bz1.Ln, Bz2.Ln, Po);							// 交点
	if ( !MF_CHECK_OR( ist, (MC_INT | MC_NINT))) {
		if ( MGeo::ChkLn3OnLn3( Bz1.LnWH, Bz2.LnWH)) {
			*ist1 = MC_ON_LINE;									// 線部材1は線部材2の直線上にあり	（平行）
		} else {
			*ist1 = 0;
		}
		*ist2 = MC_PARALLEL;
		goto exit;												// 交差なしのため無処理
	}

																// 長さ調整部材の調整先の部材に対する左右位置とカット端を求める
	s1 = (Bz2.vtUt ^ (Bz1.LnWH.p[0] - Bz2.LnWH.p[0])) * VtUtZ;	// s1>0: 長さ調整部材の配置点は長さ調整先の部材の左側
	s2 = (Bz2.vtUt ^ (Bz1.LnWH.p[1] - Bz2.LnWH.p[0])) * VtUtZ;	// s2>0: 長さ調整部材の配置方向点は長さ調整先の部材の左側

	if ( s1 > 0 && s2 > 0) {										// 長さ調整部材は長さ調整先の部材の左側にあり
		*ist1 = MC_LEFT;
//		if ( s1 < s2) {
//			*ist2 = MC_PS_NEAR;									// 配置点側の長さを補正
//		} else {
//			*ist2 = MC_PE_NEAR;									// 配置方向点側の長さを補正
//		}
	} else if ( s1 < 0 && s2 < 0) {								// 長さ調整部材は長さ調整先の部材の右側にあり
		*ist1 = MC_RIGHT;
//		if ( s1 > s2) {
//			*ist2 = MC_PS_NEAR;									// 配置点側の長さを補正
//		} else {
//			*ist2 = MC_PE_NEAR;									// 配置方向点側の長さを補正
//		}
	} else {													// 長さ調整部材は長さ調整先の部材の左右側にまたがっている
		*ist1 = MC_CLOSS;
	}
	if ( MGABS(s1) < MGABS(s2)) {
			*ist2 = MC_PS_NEAR;									// 配置点側の長さを補正
	} else {
			*ist2 = MC_PE_NEAR;									// 配置方向点側の長さを補正
	}
exit:;
	return ist;
}

} // namespace MC
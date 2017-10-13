//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MhDefine.cpp
//
//		ＤＢ読み書き定義
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
//
// 外部ファイルＤＢ	属性コード(読込用)
//
#define		MX_HAITI	5000
#define		MX_POINT	500
#define		MX_INT		500

#define		MDR_ATRCD_GRID					010

#define		MDR_ATRCD_HAITIEN				100						// 配置レコード
#define		MDR_ATRCD_HAITIEN_EOR			199						// End of Record
#define			MDR_ATRCD_HAITIEN_IKAI			100010
#define			MDR_ATRCD_HAITIEN_ICDHGT		100020
#define			MDR_ATRCD_HAITIEN_IPANELNO		100030
#define			MDR_ATRCD_HAITIEN_IKAIKONO		100040
#define			MDR_ATRCD_HAITIEN_IPARTSTPID	100050
#define			MDR_ATRCD_HAITIEN_IPARTSTPCD	100051
#define			MDR_ATRCD_HAITIEN_IMEMBERID		100060
#define			MDR_ATRCD_HAITIEN_IMEMBERCD		100061
#define			MDR_ATRCD_HAITIEN_LNHAITI		100070
#define			MDR_ATRCD_HAITIEN_PTUPHAITI		100071
#define			MDR_ATRCD_HAITIEN_RSINZURE		100080
#define			MDR_ATRCD_HAITIEN_RHAITIZURE	100090
#define			MDR_ATRCD_HAITIEN_RNAGASAHOSEI	100100
#define			MDR_ATRCD_HAITIEN_RTAKASA		100110
#define			MDR_ATRCD_HAITIEN_DISPLAYFLAG	100120
#define			MDR_ATRCD_HAITIEN_PTCMNT1		100130
#define		MDR_ATRCD_HAITIEN_MEMBER		101

#define		MDR_ATRCD_HAITIEN_TATEGU		110					// 建具レコード
//#define		MDR_ATRCD_HAITIEN_TATEGU		1100					// 建具レコード
//#define		MDR_ATRCD_HAITIEN_TATEGU_EOR	1199
#define			MDR_ATRCD_HAITIEN_CCODE			110010
#define			MDR_ATRCD_HAITIEN_RROW			110020
#define			MDR_ATRCD_HAITIEN_RROH			110030
#define			MDR_ATRCD_HAITIEN_RHEIGHT		110040
#define			MDR_ATRCD_HAITIEN_RMUSZ			110050

#define		MDR_ATRCD_HAITIEN_TENKAI		111						// 配置展開
//#define			MDR_ATRCD_HAITIEN_TENKAIPARA	111010

#define		MDR_ATRCD_HAITIEN_VERSION		120						// バージョン
#define		MDR_ATRCD_VERSION_KANRI0		121
#define		MDR_ATRCD_VERSION_KANRI1		122
#define		MDR_ATRCD_VERSION_KANRI2		123
#define		MDR_ATRCD_VERSION_KANRI3		124
#define		MDR_ATRCD_HAITIEN_OPTION1		130						// バージョン
#define		MDR_ATRCD_HAITIEN_OPTION2		131
#define		MDR_ATRCD_OPTION_KANRI			132

#define		MDR_ATRCD_HAITIEN_ZUKEI			151						// 図形レコード
#define		MDR_ATRCD_HAITIEN_ZUKEIN		152

#define		MDR_ATRCD_ROOFEN				200
#define		MDR_ATRCD_ROOFEN_EOR			299						// End of Record
#define		MDR_ATRCD_JIM					210
#define		MDR_ATRCD_JIM_EOR				219

#define		MDR_ATRCD_RFM					220
#define		MDR_ATRCD_RFM_PG				221

#define		MDR_ATRCD_RFM_PG_PT				222
#define		MDR_ATRCD_RFM_ICD				223
#define		MDR_ATRCD_RFM_ICD_I				224
#define		MDR_ATRCD_RFM_EOR				229

//==========================================================================================
//
// 外部ファイルＤＢ	属性コード(書込用)
//
#define		MX_HAITI	5000
#define		MX_POINT	500
#define		MX_INT		500

#define		MDW_ATRCD_GRID					010
#define		MDW_ATRCD_HAITIEN				100
#define		MDW_ATRCD_HAITIEN_EOR			199						// End of Record
#define			MDW_ATRCD_HAITIEN_IKAI			100010
#define			MDW_ATRCD_HAITIEN_ICDHGT		100020
#define			MDW_ATRCD_HAITIEN_IPANELNO		100030
#define			MDW_ATRCD_HAITIEN_IKAIKONO		100040
#define			MDW_ATRCD_HAITIEN_IPARTSTPID	100050
#define			MDW_ATRCD_HAITIEN_IPARTSTPCD	100051
#define			MDW_ATRCD_HAITIEN_IMEMBERID		100060
#define			MDW_ATRCD_HAITIEN_IMEMBERCD		100061
#define			MDW_ATRCD_HAITIEN_LNHAITI		100070
#define			MDW_ATRCD_HAITIEN_PTUPHAITI		100071
#define			MDW_ATRCD_HAITIEN_RSINZURE		100080
#define			MDW_ATRCD_HAITIEN_RHAITIZURE	100090
#define			MDW_ATRCD_HAITIEN_RNAGASAHOSEI	100100
#define			MDW_ATRCD_HAITIEN_RTAKASA		100110
#define			MDW_ATRCD_HAITIEN_DISPLAYFLAG	100120
#define			MDW_ATRCD_HAITIEN_PTCMNT1		100130
#define		MDW_ATRCD_HAITIEN_MEMBER		101
#define		MDW_ATRCD_HAITIEN_TATEGU		11000
#define		MDW_ATRCD_HAITIEN_TATEGU_EOR	11099
#define			MDW_ATRCD_HAITIEN_CCODE			110010
#define			MDW_ATRCD_HAITIEN_RROW			110020
#define			MDW_ATRCD_HAITIEN_RROH			110030
#define			MDW_ATRCD_HAITIEN_RHEIGHT		110040
#define			MDW_ATRCD_HAITIEN_RMUSZ			110050
#define		MDW_ATRCD_HAITIEN_TENKAI		11100
#define		MDW_ATRCD_HAITIEN_TENKAI_EOR	11199
#define			MDW_ATRCD_HAITIEN_TENKAIPARA	111010
#define		MDW_ATRCD_HAITIEN_VERSION		120
#define		MDW_ATRCD_VERSION_KANRI0		121
#define		MDW_ATRCD_VERSION_KANRI1		122
#define		MDW_ATRCD_VERSION_KANRI2		123
#define		MDW_ATRCD_VERSION_KANRI3		124
#define		MDW_ATRCD_HAITIEN_OPTION1		130
#define		MDW_ATRCD_HAITIEN_OPTION2		131
#define		MDW_ATRCD_OPTION_KANRI			132
#define		MDW_ATRCD_HAITIEN_ZUKEI			151
#define		MDW_ATRCD_HAITIEN_ZUKEIN		152

#define		MDW_ATRCD_ROOFEN				200
#define		MDW_ATRCD_ROOFEN_EOR			299						// End of Record
#define			MDW_ATRCD_ROOFEN_KAI			200010
#define			MDW_ATRCD_ROOFEN_PTH			200020
#define			MDW_ATRCD_ROOFEN_GNUM			200030
#define			MDW_ATRCD_ROOFEN_GHM			200040
#define			MDW_ATRCD_ROOFEN_TPRF			200050
#define			MDW_ATRCD_ROOFEN_KB				200060
#define			MDW_ATRCD_ROOFEN_RKH			200070
#define			MDW_ATRCD_ROOFEN_RFMA			200080
#define			MDW_ATRCD_ROOFEN_NUMA			200090
#define			MDW_ATRCD_ROOFEN_MNZ			200100
#define			MDW_ATRCD_ROOFEN_NKD			200110
#define			MDW_ATRCD_ROOFEN_KRD			200120
#define			MDW_ATRCD_ROOFEN_TPNU			200130
#define			MDW_ATRCD_ROOFEN_TPNS			200140
#define			MDW_ATRCD_ROOFEN_HKMA			200150
#define			MDW_ATRCD_ROOFEN_NKH			200160
#define			MDW_ATRCD_ROOFEN_CDURS			200170

#define		MDW_ATRCD_JIM					210
#define		MDW_ATRCD_JIM_EOR				219
#define			MDW_ATRCD_JIM_P					210010
#define			MDW_ATRCD_JIM_ICD				210020
#define			MDW_ATRCD_JIM_IFINP				210030
#define			MDW_ATRCD_JIM_RKB				210040
#define			MDW_ATRCD_JIM_IRFM				210050
#define			MDW_ATRCD_JIM_LNR				210060
#define			MDW_ATRCD_JIM_ILNRCD			210070

#define		MDW_ATRCD_KSI					220
#define		MDW_ATRCD_KSI_EOR				229
#define			MDW_ATRCD_KSI_LN				220010
#define			MDW_ATRCD_KSI_TPLN				220020
#define			MDW_ATRCD_KSI_CD				220030
#define			MDW_ATRCD_KSI_FPROC				220040
#define			MDW_ATRCD_KSI_RFM				220050


#define		MDW_ATRCD_RFM					230
#define		MDW_ATRCD_RFM_PG				231

#define		MDW_ATRCD_RFM_PG_PT				232
#define		MDW_ATRCD_RFM_ICD				233
#define		MDW_ATRCD_RFM_ICD_I				234
#define		MDW_ATRCD_RFM_EOR				239

#define		MDW_ATRCD_TENKAI				300
#define		MDW_ATRCD_TENKAI_EOR			309
#define			MDW_ATRCD_TENKAI_NJUKO			300010
#define			MDW_ATRCD_TENKAI_TPLN			300020

//
//	内部ＤＢ
//
//	定数
#define		MD_MAX_BKAI				2											// 最大地下階数
#define		MD_MAX_FKAI				5											// 最大地上階数
#define		MD_MAX_AKAI				( MD_MAX_BKAI + 1 + MD_MAX_FKAI)			// 最大全階数（最大地上＋地階 + 地下階数）
//#define		MD_MAX_SCM_KZTP			4											// 最大構造スキーマ種類数

#define		MDIDGRPK(G,K)			( G + K + MD_MAX_BKAI) 
#define		MDIDSCMT(S,T)			( S + T)

// グループ
#define		MD_NM_GRP_WIN			Mstr( "ウィンドウ")
#define		MD_NH_GRP_WIN			0
#define		MD_NM_GRP_SHIKITI		Mstr( "敷地")
#define		MD_NH_GRP_SHIKITI		1
#define		MD_NM_GRP_GAIKOU		Mstr( "外構")
#define		MD_NH_GRP_GAIKOU		1
#define		MD_NM_GRP_ISHOU			Mstr( "意匠")
#define		MD_NH_GRP_ISHOU			2
#define		MD_NM_GRP_KOUZOU		Mstr( "構造")
#define		MD_NH_GRP_KOUZOU		( MD_NH_GRP_ISHOU + MD_MAX_AKAI)

#define		MD_N_GRP				( MD_NH_GRP_KOUZOU + MD_MAX_AKAI)

//// スキーマ
// 外構の種類
#define		MD_NM_SCMS_KUKAKU		Mstr( "図形")				// 区画、線分
#define		MD_NH_SCMS_KUKAKU		0
#define		MD_NM_SCMS_HAITI		Mstr( "配置")
#define		MD_NH_SCMS_HAITI		1
#define		MD_NM_SCMS_PANEL		Mstr( "線")
#define		MD_NH_SCMS_PANEL		2
#define		MD_NM_SCMS_BUZAI		Mstr( "面")
#define		MD_NH_SCMS_BUZAI		3
#define		MD_NM_SCMS_CHUKI		Mstr( "注記")
#define		MD_NH_SCMS_CHUKI		4

// 意匠の種類
#define		MD_NM_SCMI_KUKAKU		Mstr( "図形")				// 区画、線分
#define		MD_NH_SCMI_KUKAKU		0
#define		MD_NM_SCMI_HAITI		Mstr( "配置")
#define		MD_NH_SCMI_HAITI		1
#define		MD_NM_SCMI_WALL			Mstr( "壁")
#define		MD_NH_SCMI_WALL			2
#define		MD_NM_SCMI_ROOM			Mstr( "部屋")
#define		MD_NH_SCMI_ROOM			3
#define		MD_NM_SCMI_TATEGU		Mstr( "建具")
#define		MD_NH_SCMI_TATEGU		4
#define		MD_NM_SCMI_CHUKI		Mstr( "注記")
#define		MD_NH_SCMI_CHUKI		5

// 構造部材の種類(KIND)
#define		MD_NM_SCMK_KUKAKU		Mstr( "図形")				// 区画、線分
#define		MD_NH_SCMK_KUKAKU		0
#define		MD_NM_SCMK_BUZAI		Mstr( "部材")
#define		MD_NH_SCMK_BUZAI		1
#define		MD_NM_SCMK_PANEL		Mstr( "パネル")
#define		MD_NH_SCMK_PANEL		2
#define		MD_NM_SCMK_KANAGU		Mstr( "金物")
#define		MD_NH_SCMK_KANAGU		3
#define		MD_NM_SCMK_CHUKI		Mstr( "注記")
#define		MD_NH_SCMK_CHUKI		4

#define		MD_N_SCMK				( MD_NH_SCMK_CHUKI + 1)		// スキーマ種類数

// スキーマ
// 種類分けを行うスキーマ
#define		MD_NM_SCM_GRID			Mstr( "グリッド")
#define		MD_NH_SCM_GRID			0
#define		MD_NM_SCM_HAITI			Mstr( "配置")
#define		MD_NH_SCM_HAITI			1
#define		MD_NM_SCM_GAIKOU		Mstr( "外構")
#define		MD_NH_SCM_GAIKOU		1
#define		MD_NM_SCM_ISYOU			Mstr( "意匠")
#define		MD_NH_SCM_ISYOU			1

#define		MD_N_SCM1

// 構造部材の種類分けを行うスキーマ
#define		MD_NM_SCM_KISO			Mstr( "基礎")
#define		MD_NH_SCM_KISO			2
#define		MD_NM_SCM_DODAI			Mstr( "土台")
#define		MD_NH_SCM_DODAI			( MD_NH_SCM_KISO + MD_N_SCMK)
#define		MD_NM_SCM_YUKA			Mstr( "床")
#define		MD_NH_SCM_YUKA			( MD_NH_SCM_DODAI + MD_N_SCMK)
#define		MD_NM_SCM_KABE			Mstr( "壁")
#define		MD_NH_SCM_KABE			( MD_NH_SCM_YUKA + MD_N_SCMK)
#define		MD_NM_SCM_TENJO			Mstr( "天井")
#define		MD_NH_SCM_TENJO			( MD_NH_SCM_KABE + MD_N_SCMK)
#define		MD_NM_SCM_KOYA			Mstr( "小屋")
#define		MD_NH_SCM_KOYA			( MD_NH_SCM_TENJO + MD_N_SCMK)
#define		MD_NM_SCM_YANE			Mstr( "屋根")
#define		MD_NH_SCM_YANE			( MD_NH_SCM_KOYA + MD_N_SCMK)

#define		MD_N_SCMA				( MD_NH_SCM_YANE + MD_N_SCMK)
#define		MD_N_SCM2				( MD_N_SCMA - MD_N_SCM1)
#define		MD_N_SCM				7

// レイヤー
#define		MD_ID_LYR_GRID			0
#define		MD_ID_LYR_HAITI			1
#define		MD_ID_LYR_YANE			2

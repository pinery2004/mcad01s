#pragma once

//===========================================================================
//		選択
//
enum MPSELCD {
	MP_SENTAKU_ISYOU = 1,						// 意匠入力
	MP_SENTAKU_KOUZOU,							// 構造入力
};

//===========================================================================
//		入力点区分
//
enum MPInpKb {
	MP_INPKB_NONE,								// 無指定入力
	MP_INPKB_1PT = 1,							// １点入力
	MP_INPKB_DIR1PT,							// 方向１点入力
	MP_INPKB_LEN2PT,							// 長さ２点入力
	MP_INPKB_AREA,								// 区画入力
	MP_INPKB_FREE								// 自由入力
};

//===========================================================================
//		高さ基準コード
//
enum MPHEIGHTCD {
	MP_HGTCD_SGL = 0,							// SGL					//ZZZZZ
	MP_HGTCD_KISOKJ,							// 基礎基準
	MP_HGTCD_SITAKJ,							// 下基準
	MP_HGTCD_UEKJ,								// 上基準
	MP_HGTCD_TENJOKJ,							// 天井基準
	MP_HGTCD_YANEKJ								// 屋根基準
};

//===========================================================================
//		グリッド丸めコード
//
enum MPMARUMECD {
	MP_MRMCDN = 0,								// 丸め無し  
	MP_MRMCD1,									// Ｎ／１グリッド丸め
	MP_MRMCD2,									// Ｎ／２グリッド丸め
	MP_MRMCD3,									// Ｎ／３グリッド丸め
	MP_MRMCD4,									// Ｎ／４グリッド丸め
	MP_MRMCD6,									// Ｎ／６グリッド丸め
	MP_MRMCD8									// Ｎ／８グリッド丸め
};

//===========================================================================
//		依存コード
//
enum MPHAITICD {
	MP_HAICD_NINI = 0,							// 任意
	MP_HAICD_KABESINZUKE,						// 壁芯付け
	MP_HAICD_YANEKOUSEISENZUKE					// 屋根構成線付け
};

//===========================================================================
//		依存コード
//
enum MPIZONCD {
	MP_IZNCD_N = 0,								// 無し
	MP_IZNCD_SUIHEI,							// 水平
	MP_IZNCD_SUICYOKU,							// 垂直
	MP_IZNCD_TENJONETAUE,						// 天井根太上
	MP_IZNCD_YANEMENNARI,						// 屋根面なり
	MP_IZNCD_YANEMENNARIENCYOKU,				// 屋根面なり鉛直
	MP_IZNCD_YANEMENNARISUICYOKU,				// 屋根面なり垂直
	MP_IZNCD_YANEKOSEISENNARI,					// 屋根構成線なり
	MP_IZNCD_TARUKISITA,						// 垂木下
	MP_IZNCD_TARUKISITAYANEMENHEIKOU,			// 垂木下屋根面平行
	MP_IZNCD_TARUKISITASUICYOKU,				// 垂木下垂直
	MP_IZNCD_NOKISAKINARI						// 軒先なり
};

//===========================================================================
//		階、組、部材、選択
//
//	階
#define		MMAX_KAI		3					// 最大階数
enum MPKAI {
	MP_KAI1 = 1,								// １階
	MP_KAI2,									// ２階
	MP_KAI3										// ３階
};

//	入力モード
enum MPMODE {
	MP_MD_CREATE = 1,							// 創成モード
	MP_MD_DELETE,								// 削除モード
	MP_MD_MODIFY,								// 修正モード
	MP_MD_MOVE,									// 移動モード
	MP_MD_COPY,									// 複写モード
	MP_MD_PROPERTY								// 属性表示モード
};

//	組
#define		MMAX_KUMI		10					// 最大組数
enum MPKOSEI {
	MP_GP_KISO = 1,								// "基礎"
	MP_GP_DODAI,								// "土台"
	MP_GP_YUKA,									// "床組"
	MP_GP_KABE,									// "壁枠組"
	MP_GP_TAIRYOKU,								// "耐力壁"
	MP_GP_TENJO,								// "天井組"
	MP_GP_KOYA,									// "小屋組"
	MP_GP_YAGIRI,								// "矢切"
	MP_GP_YANE,									// "屋根"

	MP_GP_JYUKO = 101,							// "住戸"
	MP_GP_JYUTO,								// "住棟"
	MP_GP_BUKKEN,								// "物件"
	MP_GP_S_YANE,								// "設計屋根"
	MP_GP_SIKITI,								// "敷地"
	MP_GP_HEKIRYOU								// "壁量"
};

//	分類
enum MPBUNRUI {
	MP_BR_BUZAI = 0,							// "部材"
	MP_BR_KANAGU,								// "金物"
	MP_BR_PANEL,								// "パネル"
	MP_BR_YANE,									// "屋根"
	MP_BR_KABE,									// "壁"
	MP_BR_KISO,									// "基礎"
	MP_BR_SENBUN,								// "線分"
	MP_BR_KUKAKU,								// "区画"
	MP_BR_KAIKO,								// "開口"
	MP_BR_CYUKI,								// "注記"
	MP_BR_OTHER = 10							// "その他"
};

// 選択
enum MPSELECT {
	MP_SEL1 = 1,								// 選択１
	MP_SEL2,									// 選択２
	MP_SEL3										// 選択３
};

//	属性値入力モード
enum MPATTR {
	MP_AT_AUTO = -1,							// 自動
	MP_AT_NONE = 0,								// 属性値入力なし
	MP_AT_HRZ_PARTS,							// 水平部材入力、傾斜部材入力
	MP_AT_VRT_PARTS,							// 垂直部材入力
	MP_AT_YTPANEL,								// 床天井パネル入力
	MP_AT_YANEPANEL,							// 屋根パネル入力
	MP_AT_ADJLNG,								// 部材長さ調整入力
	MP_AT_YANE,									// 屋根入力
	MP_AT_TATEGU								// 垂直部材入力
};

//===========================================================================
//		コンボボックスサイズ
#define		MX_CMB_PARTSNM	300					// 部材名コンボボックスサイズ
#define		MX_CMB_CDMEMBER	200					// 寸法型式コンボボックスサイズ
#define		MX_CMB_PANELNO	300					// パネル番号コンボボックスサイズ

#define		MX_CMB_ATTR		30					// 属性値コンボボックスサイズ

//	属性入力用コンボボックスの属性ID
enum MCCMBATTR {
	MC_CMB_NULL = 100,							// 無設定　コンボボックス非表示
	MC_CMB_LHS1,								// 長さ補正値１
	MC_CMB_LHS2,								// 長さ補正値２
	MC_CMB_ZJSZ,								// 材軸芯ずれ量
	MC_CMB_HAIZ,								// 配置点ずれ量
	MC_CMB_TRTH,								// 取り付け高さ
	MC_CMB_KROH,								// ＲＯＨ
	MC_CMB_INTR,								// 間隔
	MC_CMB_HONS,								// 本数
//	MC_CMB_HHS1,								// 幅補正１
//	MC_CMB_HHS2,								// 幅補正２
	MC_CMB_FHS1,								// 床・天井パネル手前側補正
	MC_CMB_FHS2,								// 屋根パネル用手前側補正
	MC_CMB_OKYK,								// 奥行き
	MC_CMB_OHS2,								// 奥行き補正
	MC_CMB_KOBY,								// 屋根勾配
	MC_CMB_NKDE,								// 軒の出
	MC_CMB_KRDE,								// ケラバの出
	MC_CMB_UPRH,								// 上端高さ
	MC_CMB_LWRH,								// 下端高さ
	MC_CMB_LFH1,								// 床・天井パネル左巾補正
	MC_CMB_RTH1,								// 床・天井パネル右巾補正
	MC_CMB_LFH2,								// 屋根パネル用左巾補正
	MC_CMB_RTH2,								// 屋根パネル用右巾補正
};

//	属性入力用チェックボックスの属性ID
enum MCCHKATTR {
	MC_CHK_NULL = 200,							// 無設定　チェックボックス非表示
	MC_CHK_KATI,								// 勝ち
	MC_CHK_MULT,								// 複数
	MC_CHK_INTC,								// 交差部材調整
	MC_CHK_NKTP									// 軒先タイプ(0:勾配収まり、1:垂直)
};

#define		MC_INT_JIDO 	9999				// 自動
#define		MC_STR_JIDO 	Mstr( "自動")		// 自動
#define		MC_INT_AREA 	9998				// 領域
#define		MC_STR_AREA 	Mstr( "領域指定")	// 領域

#define		MMAX_BZI_HONSU	100					// 最大配置部品本数(N本配置)

//	ドラッギング入力モード（ラバーバンド表示形状）
#define		MC_RBND_LINE	1					// 線分表示 Line
#define		MC_RBND_RECT	2					// 長方形表示 Rectangle
#define		MC_RBND_POLYGON	3					// 多角形表示 Polygon
#define		MC_RBND_PARALOG	4					// 平行四辺形表示 Parallelogram

//	属性入力　部材種類
//enum MCINPATTRTYP {
//	MC_INP_HOR_PARTS,							// 横置き部材
//	MC_INP_VER_PARTS,							// 縦置き部材
//	MC_INP_HOR_PANEL,							// 横置きパネル
//	MC_INP_VER_PANEL,							// 縦置きパネル
//	MC_INP_TATEGU								// 建具
//};

#define		MMAX_PARTSTP	300										// 最大部材種類数

#define		MMAX_BZILEN		30000.									// 最大部材長さ30m
#define		MMIN_BZILEN		1.										// 最小部材長さ
#define		MMIN_BZILEN_2	(MMIN_BZILEN * MMIN_BZILEN)				// 最小部材長さ * 最小部材長さ

	
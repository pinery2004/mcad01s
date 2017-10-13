#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MddDefine.h
//
//		ＭＣＡＤモデル
//						表示用Define
//
//
//==========================================================================================
//
//	図形コード
//

#ifdef	NOT_USED

#define		MZCD_PNT		0x01				// 点
#define		MZCD_PNTV		0x02				// 点
#define		MZCD_ULIN		0x03				// 直線
#define		MZCD_HLIN		0x04				// 半直線
#define		MZCD_LIN		0x05				// 線分
#define		MZCD_PLY		0x06				// 折れ線
#define		MZCD_PLYV		0x07				// 折れ線 VECTOR
#define		MZCD_PAC		0x08				// 線分|円弧 折れ線

#define		MZCD_RCT		0x09				// 長方形
#define		MZCD_PGN		0x0A				// 多角形
#define		MZCD_PGNV		0x0B				// 多角形 VECTOR
#define		MZCD_ARA		0x0C				// 閉領域

#define		MZCD_HRCT		0x0D				// 穴付長方形
#define		MZCD_HPLN		0x0E				// 穴付多角形
#define		MZCD_HARA		0x0F				// 穴付閉領域

#define		MZCD_CIR		0x10				// 円
#define		MZCD_ARC		0x11				// 円弧
#define		MZCD_ELN		0x12				// 楕円
#define		MZCD_ELA		0x13				// 楕円弧

#define		MZCD_SPLN		0x14				// スプライン曲線

#define		MZCD_MPLO		0x18				// 複合曲線（開）
#define		MZCD_MPLC		0x19				// 複合曲線（閉）

#define		MZCD_BMP		0x1C				// ビットマップ

// 文字
#define		MZCD_MOJZ		0x20				// 文字属性
#define		MZCD_MOJN		0x21				// 文字列

// 寸法
#define		MZCD_HSHN		0x28				// 寸法 水平/垂直/距離/平行
#define		MZCD_HSAN		0x29				// 寸法 角度
#define		MZCD_HSDN		0x2A				// 寸法 直径
#define		MZCD_HSRN		0x2B				// 寸法 半径

// 次元
#define		MZCD_2D			0x40				// 2Dプリミティブ 変換用パラメータ
#define		MZCD_3D			0x00				// 3Dプリミティブ 変換用パラメータ

// 面
#define		MZCD_LNS		0x81				// 線形構造面
#define		MZCD_TGS		0x82				// 移動生成面（平行）
#define		MZCD_RGS		0x83				// 回転生成面
#define		MZCD_TRGS		0x84				// 移動回転生成面
#define		MZCD_TGS2		0x85				// 移動生成面（回転）
#define		MZCD_TGS3		0x86				// 移動生成面（軸固）
#define		MZCD_HIM		0x87				// 高さを与える面
#define		MZCD_WIM		0x88				// 幅を与える面

// 平面
#define		MZCD_IPLN		0x8C				// 無限平面
#define		MZCD_PLN		0x8D				// 平面

// 立体
#define		MZCD_LNV		0x91				// 線形構造立体
#define		MZCD_TGV		0x92				// 移動生成立体（平行）
#define		MZCD_RGV		0x93				// 回転生成立体
#define		MZCD_TRGV		0x94				// 移動回転生成立体
#define		MZCD_TGV2		0x95				// 移動生成立体（回転）
#define		MZCD_TGV3		0x96				// 移動生成立体（軸固）
#define		MZCD_HIR		0x97				// 高さを与える立体
#define		MZCD_GSPR		0x98				// 幅を与える立体
#define		MZCD_HBR		0x99				// 幅と高さを与える立体
#define		MZCD_BOV2		0x9A				// 板状立体

#define		MZCD_UNN		0x9C				// 集合
#define		MZCD_GFS		0x9D				// 多面
#define		MZCD_GFO		0x9E				// 多面体

// 柱
#define		MZCD_KKP		0xA1				// 角（柱）
#define		MZCD_KKB		0xA2				// 角（梁）
#define		MZCD_RKP		0xA3				// 丸（柱）
#define		MZCD_RKB		0xA4				// 丸（梁）
#define		MZCD_HIRB		0xA5				// Ｈ鋼（梁）
#define		MZCD_HIRP		0xA6				// Ｈ鋼（柱）
#define		MZCD_POL		0xA7				// 多角形
#define		MZCD_PHIL		0xA8				// 柱体
#define		MZCD_PYRA		0xA9				// 錐体
#define		MZCD_TRPY		0xAA				// 錐台
#define		MZCD_SPHE		0xAB				// 球
#define		MZCD_UNNS		0xAC				// 縮尺対応和集合

// 光源
#define		MZCD_LIG		0xB1				// 光源
#define		MZCD_KAN		0xB2				// 環境光

// 属性
#define		MZAT_PUSH		0xC0				// 属性ＰＵＳＨ
#define		MZAT_PULL		0xC1				// 属性ＰＵＬＬ
#define		MZAT_ST			0xC2				// 表示属性START
#define		MZAT_MRK		0xC3				// マーカ
#define		MZAT_SLD		0xC4				// 塗りつぶし
#define		MZAT_LTP		0xC5				// 線種
#define		MZAT_COL		0xC6				// 色
#define		MZAT_KCL		0xC7				// 透過係数
#define		MZAT_LTB		0xC8				// 線幅
#define		MZAT_CA			0xC9				// 材質環境光
#define		MZAT_CD			0xCA				// 材質拡散光
#define		MZAT_CS			0xCB				// 材質鏡面光
#define		MZAT_CE			0xCC				// 材質放射光
#define		MZAT_SS			0xCD				// 材質鏡面指数
#define		MZAT_TX			0xCE				// テクスチャ
#define		MZAT_TXZ		0xCF				// テクスチャ座標
#define		MZAT_TXP		0xD0				// テクスチャパラメータ
#define		MZAT_REF		0xD1				// 鏡面反射率
#define		MZAT_POV		0xD2				// POV-RAY出力用属性
#define		MZAT_END		0xD3				// 表示属性START

// 部材配置
#define		MZCD_PNZ		0xE0				// 点部材

// 作業面
#define		MZCD_SAGY		0xF0				// 作業面

//#define		MZCD_BUF		0xxx				// 図形情報表現

//	次元
#define		MMD_2D			1					// ２次元
#define		MMD_3D			2					// ３次元

//	スケール
#define		MSCL_50A		1					// 1/50A
#define		MSCL_50B		2					// 1/50B
#define		MSCL_100A		3					// 1/100A
#define		MSCL_100B		4					// 1/100B
#define		MSCL_200A		5					// 1/200A
#define		MSCL_200B		6					// 1/200B

//	ビュー種類
#define		MVCD_HEIMEN		1					// 平面
#define		MVCD_SYOUMEN	2					// 正面
#define		MVCD_HAIMEN		3					// 背面
#define		MVCD_LFTSOK		4					// 左側面
#define		MVCD_RGTSOK		5					// 右側面
#define		MVCD_URAMEN		6					// 裏面

//	図形演算
#define		MZL_ADD			1					// 和
#define		MZL_SUB			2					// 差
#define		MZL_MLT			3					// 積

//	対象指示有無
#define		MSYM_NO			0					// なし
#define		MSYM_X			1					// Ｘ軸対称、ＹＺ平面対称
#define		MSYM_Y			2					// Ｙ軸対称、ＺＸ平面対称
#define		MSYM_Z			3					// Ｚ軸対称、ＸＹ平面対称

//	平面の法線ベクトル方向
#define		MVC_PLX			1					// ＋Ｘ方向
#define		MVC_MNX			2					// －Ｘ方向
#define		MVC_PLY			3					// ＋Ｙ方向
#define		MVC_MNY			4					// －Ｙ方向
#define		MVC_PLZ			5					// ＋Ｚ方向
#define		MVC_MNZ			6					// －Ｚ方向
#define		MVC_ETC			7					// その他方向

//	平面フラグ
#define		MMEN_XY			1					// ＸＹ平面
#define		MMEN_YZ			2					// ＹＺ平面
#define		MMEN_ZX			3					// ＺＸ平面
#define		MMEN_2D			4					// ２次元座標

//	ビューコード
#define		MVW_PXY			1					// ＋ＸＹ（ＵＰ）
#define		MVW_MXY			2					// －ＸＹ（ＤＯＷＮ）
#define		MVW_PYZ			3					// ＋ＹＺ（ＥＡＳＴ）
#define		MVW_MYZ			4					// －ＹＺ（ＷＥＳＴ）
#define		MVW_PZX			5					// ＋ＺＸ（ＳＯＵＴＨ）
#define		MVW_MZX			6					// －ＺＸ（ＮＯＲＴＨ）

//	指示方向
#define		MDIR_NO			0					// なし
#define		MDIR_RIT		1					// 右側
#define		MDIR_LFT		2					// 左側
#define		MDIR_PLS		3					// ＋側
#define		MDIR_MNS		4					// －側
#define		MDIR_OUT		5					// 外側
#define		MDIR_IN			6					// 内側
#define		MDIR_UP			7					// 上側
#define		MDIR_DWN		8					// 下側

// 図形プリミティブ
#define		MDZCODE(X)		m_st[X]
#define		MDZNEXT(X)		m_st[X]
#define		MDZPRIM(X)		m_st[X+1]

//#define		MDZCODE(X)		(*((short*)&m_st[X]))
//#define		MDZNEXT(X)		(X+((short*)&m_st[X])[1])
//#define		MDZPRIM(X)		m_st[X+1]

#endif
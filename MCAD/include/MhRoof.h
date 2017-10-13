///////////////////////////////////////////////////////////////////////////////
//	  屋根
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include	"MgGrp.h"
#include	"MhZukei.h"

#define HDBID MINT

#define		MC_PRI_MIN_AREA		0					// 面積が小を優先に屋根面を検索
#define		MC_PRI_MIN_HIGHT	1					// 高さが小を優先に屋根面を検索
#define		MC_PRI_MAX_HIGHT	2					// 高さが大を優先に屋根面を検索

namespace MC
{

class MmWndInfo;

//	屋根構成線タイプコード
typedef enum _roof_sen_type {
	MHRL_DUMMY,										// ダミー
	MHRL_NOKI,										// 軒
	MHRL_KERABA,									// けらば
	MHRL_MUNE,										// 棟
	MHRL_RYOU,										// 稜
	MHRL_TANI,										// 谷
	NHRL_MIZUKIRI,									// 水切り
	MHRL_KATANAGARENOKI,							// 片流れ軒
	MHRL_YANEUEMIZUKIRI,							// 親屋根の上に乗る子屋根の構成線
	MHRL_YAGIRI										// 矢切
} ROOF_SEN_TYPE;

//	屋根タイプコード
typedef enum _roof_type {
	MHRF_YOSEMUNE,									// 寄棟
	MHRF_KIRITUMA,									// 切妻
	MHRF_HANKIRITUMA,								// 半切妻
	MHRF_IRIMOYA,									// 入母屋
	MHRF_KATANAGARE,								// 片流れ
	MHRF_MANSADO,									// マンサード
	MHRF_YOSEMUNEOSAMARI,							// 寄棟納まり
	MHRF_FUKIOROSI,									// 葺降ろし
	MHRF_CUT,										// カット
	MHRF_DOMA,										// ドーマ
	MHRF_KAIKOU,									// 開口
	MHRF_FLAT,										// フラット
	MHRF_MANUALMEN									// マニュアル面
} ROOF_TYPE;

typedef enum _mgrdtp{
	MGRDTP_GROUP = 100,
	MGRDTP_GJIM1,									// 地廻り線（入力）
	MGRDTP_GJIM2,									// 地廻り線（合成後）
	MGRDTP_GKS1,									// 構成線（入力）
	MGRDTP_GKS2,									// 構成線（合成後）
	MGRDTP_GTN,										// 谷線
	MGRDTP_GTRFM									// 屋根面
} MGRDTp;

class MhRoofInfo;

class	MhRfm;
//
//======================( ２次元 )==============================
//		MhJim 地廻り線（入力）
//

class MhJim
{
public:
	MgPoint2	m_p;								// 地廻り線右端の頂点
	MINT		m_icd;								// 地廻り線コード
	MINT		m_ifInp;							// 入力フラグ
	MREAL		m_rKb;								// 勾配
	MINT		m_iRfm;								// 屋根面配列番号
	MgLine3		m_LnR;								// 右側屋根構成線（延長）
	MINT		m_icdLnR;							// 右側屋根構成線コード
public:
	void Free()			{}

	MINT Load(								// ＤＢより１レコード読み込み
				MhMdlIO*		i_phMdl		// 書き込みファイル　ハンドル
				);
	MINT Save(								// ＤＢへ１レコード書き込み
				MhMdlIO*		i_phMdl		// 書き込みファイル　ハンドル
				);

	void Print( MCHAR* s, MINT ic = -1);
};
//
//======================( ３次元 )==============================
//		MgKs1 構成線（入力）
//

class MgKs1
{
public:
	MgLine3		m_Ln;								// 構成線分
	MINT		m_itpLn;							// 線分タイプ(0: 半直線　1:線分)
	MINT		m_iCd;								// 構成コード
	MINT		m_ifProc;							// 計算処理用フラグ
	MINT		m_iRfm[2];							// 屋根面配列番号 (左、右)
public:
	void Free()			{}

	void Print( MCHAR* s, MINT ic = -1);
};
//
//======================( ３次元 )==============================
//		MhRfm 屋根面
//

class MhRfm
{
public:
	MgPlane3	m_Pln;								// 屋根面係数
	MgPolyg3	m_Pg;								// 屋根面形状（屋根構成線）
	MgGInt		m_cdPg;								// 屋根構成線コード
	MINT		m_icd;								// 構成コード
public:
	void Init()	
						{ m_Pg.Init(); m_cdPg.Init();}
	void Free()													// 領域開放
						{ m_Pg.Free(); m_cdPg.Free();}

	MINT Load(								// ＤＢより１レコード読み込み
				MhMdlIO*		i_phMdl		// 書き込みファイル　ハンドル
				);
	MINT Save(								// ＤＢへ１レコード書き込み
				MhMdlIO*		i_phMdl		// 書き込みファイル　ハンドル
				);

	void Print( MCHAR* s, MINT ic = -1);
};

//
////////////////////////////////////////////////////////////////
//		屋根

class MhRoofInfo: public MOBJECT
{

	friend	class	HaitiIn;							//ZZZZZ 091115
	friend	class	MdRoofQueue;
	friend	MINT	MdRoofModify( MhRoofInfo *pRfInfoR, MhRoofInfo* *ppRfInfoM);
	friend	void	MdRoofDelete( MPOSITION pPosition);
	friend	MINT	RoofPlc( const MgPolyg2 &pgJim, const MgGInt &gint, const MgPoint2 &pth);
	friend	void	MmWndKDrawRoof( MmWndInfo* pWndInfo);

//public:
	MINT				m_iKai;						// 階( 1, 2, 3)
	MgGrpS<MhJim>		m_GJim1;					// 地廻り線（入力）
													// （（頂点・地廻り線コード・入力フラグ)n）
	MgPoint2			m_ptH;						// 方向を示す点（入力）
	MgGrpS<MgKs1>		m_GKs1;						// 構成線（入力）
													// （（構成線分・構成コード・構成線情報)n）
	MgGrpF<MhRfm>		m_GRfm;						// 屋根面
													// （（(頂点・構成コード)n)n）
	MgGPolyg3			m_GNUm;						// 軒裏面
													// （（(頂点)n)n）
	MgGPolyg3			m_GHm;						// へり面
													// （（(頂点)n)n）
	
//	MhRoofInfo*			m_Oya;						// 親屋根
	MINT				m_itpRf;					// 屋根タイプ
	MREAL				m_rKb;						// 勾配
	MREAL				m_rRkH;						// 屋根基準高さ変更量
	MREAL				m_rRfmA;					// 屋根面厚さ(89,140,182)
	MREAL				m_rNUmA;					// 軒裏面厚さ(89,140)
	MREAL				m_rMnZ[2];					// 棟ずれ量１・２
	MREAL				m_rNkD[2];					// 軒の出
	MREAL				m_rKrD[2];					// けらばの出
	MINT				m_itpNU;					// 軒裏タイプ１・２
	MINT				m_itpNS;					// 軒先タイプ１・２
	MREAL				m_rHKmA;					// 鼻隠し面厚さ
	MREAL				m_rNUk;						// 軒裏返し量
	MREAL				m_rNkH;						// 軒の高さ（台形ドーマ用）
	MINT				m_icdURS;					// 下屋根コード
//	MINT				m_icdRR;					// 大屋根コード
	MhZukei*			m_pZukei;					// 図形

	MINT				m_imdDsp;					// 表示モード (0: 屋根面  1:構成線)
	COLORREF			m_colDispD;					// 表示色（データ指定）
	COLORREF			m_colDispC;					// 表示色（現在）
//	MnDisp*				m_iDispM;					// 表示情報（面）
//	MnDisp*				m_iDispL;					// 表示情報（線分）

	MINT				m_nRtm;						// 立面数
	MINT*				m_pifRitm;					// 立面表示(面フラグ)  0:面  1:穴
	MINT*				m_pnPtRitm;					// 立面表示(頂点数)
	MgPoint3*			m_pPtRitm;					// 立面表示(頂点)
//	MnDisp*				m_pLnRitm;					// 立面表示情報（線分）

//	MhTexture*			m_KsInfo[NKSINFO];			// 構成情報

	MsBitSet*			m_pVer;					// バージョン制御 (0: 全バージョンで表示)
	MsBitSet*			m_pOpt1;					// 表示オンOPT群セット (NULL: オプション設定無しでも表示)
	MsBitSet*			m_pOpt2;					// 表示オフOPT群セット (NULL: 無視)

public:
	MhRoofInfo();
	~MhRoofInfo();

	MINT Load(								// ＤＢより１レコード読み込み
				MhMdlIO*		i_phMdl		// 書き込みファイル　ハンドル
				);
	MINT Save(								// ＤＢへ１レコード書き込み
				MhMdlIO*		i_phMdl		// 書き込みファイル　ハンドル
				);

	
	MgGrpF<MhRfm>*	GetpGRfm()
								{
									return	&m_GRfm;
								}
	MgGrpS<MhJim>*	GetpGJim1()
								{
									return	&m_GJim1;
								}
	MhZukei*		GetpZukei()
								{
									return	m_pZukei;
								}
	
	MINT	GetKai()
								{
									return m_iKai;
								}
	

	void	FreeAllAtr();										// 属性用に確保した領域を開放する
	void	InitAllAtr( MINT ifPtr = 1);						// 属性をイニシャライズ、領域へのポインタをクリア
	void	SetPIVer( MsBitSet* pVer)
								{
									m_pVer = pVer;
								}								// バージョン制御
	void	SetPIOpt1( MsBitSet* pOptv)
								{
									m_pOpt1 = pOptv;
								}								// 表示オンOPT群セット
	void	FreePIOpt1()
								{
									MBFREE( m_pOpt1);
								}								// 表示オンOPT群セットを無しに設定
	void	SetPIOpt2( MsBitSet* pOptv)
								{
									m_pOpt2 = pOptv;
								}								// 表示オフOPT群セット
	void	FreePIOpt2()
								{
									MBFREE( m_pOpt1);
								}								// ＯＰＴ群表示ＯＦＦを無しに設定

	MsBitSet*	GetPIVer()
								{
									return m_pVer;
								}								// バージョン制御
	MsBitSet*	GetPIOpt1()
								{
									return m_pOpt1;
								}								// 表示オンOPT群セット
	MsBitSet*	GetPIOpt2()
								{
									return m_pOpt2;
								}								// 表示オフOPT群セット

//	{ if ( m_pZukei) { MBFREE( m_pZukei->m_pNext); free( m_pZukei);}}

	// 地廻り線にコードを設定する
	void SetJimCd(
				const	MgPolyg2	&pgJim,			// (I  ) 地廻り区画
				const	MgGInt		&GifInp,		// (I  ) 地廻り線種類(仮想キー(nflag)  MK_SHIFT(004): シフトキー)
				const	MgPoint2	&pth			// (I  ) 方向点
				);

	// 属性入力値を取り込み設定する 
	void SetAttr();

	// 地廻り線に対応する屋根面を作成する 
	void CreateRfmForJim();

	// 地廻り線に接続する構成線を作成する 
	void CreateKsCwJim();

	// 他の屋根面との交差を調べ交差線分で屋根面形状をカットする
	MINT AdjustRfm1();								// (  O) ステイタス　0: 正常　-1: 屋根面形状作成エラー

	// 上階外壁との交差を調べ交差線分で屋根面形状をカットする
	MINT AdjustRfm2();								// (  O) ステイタス　0: 正常　-1: 屋根面形状作成エラー

	// 重なる屋根面を1つにまとめる
	void ConnectRfm();

	// 屋根面の軒線、けらば線をオフセットする
	void OffsetRfm();

	// 屋根面の図形を作成する
	void CreateRfmZukei();

	// トレース
	void Print( MCHAR* s);
};

////////////////////////////////////////////////////////////////////////////
//    コンストラクタ
inline	MhRoofInfo::MhRoofInfo()
{
	InitAllAtr();
}

////////////////////////////////////////////////////////////////////////////
//	デストラクタ
inline	MhRoofInfo::~MhRoofInfo()
{
	FreeAllAtr();
}

} // namespace MC
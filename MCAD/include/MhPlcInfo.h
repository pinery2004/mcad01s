#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MhPlcInfo.h
//
//		配置物レコード
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "MhIeModel.h"
#include "MhGp.h"
#include "MhMbr.h"

#include "MhTategu.h"
#include "MhZukei.h"
#include "MhTpPts.h"

#include "MhLib.h"

namespace MC
{

// 配置型
class	MhPlcInfo: public MOBJECT
{
	friend	class	MdPtsQueue;
	friend	void	MdPtsModify( MhPlcInfo *pPlcEnR, MhPlcInfo* *ppPlcEnM);
	friend	void	MdPtsDelete( MPOSITION pPosition);
	friend	void	MmPtsPlc( const MgPoint3 *Pt, const MgVect3 vUpPlc, const MgPolyg2 *pPg);
	friend	void	MmWndKDrawPts( class MmWndInfo* pWndInfo);
	friend	void	IeModel::MhNormKabe( MINT nProc);
	friend	void	MhNormKiso( MINT nProc);
	//friend	MINT	MhMdlLoad( MCHAR* PathI);
	friend	MINT	MhBziHaitiLoad(
						MhMdlIO*	i_phMdl		// 読み込みファイル　ハンドル
						//void**	io_ppEnt,	// エンティティ
						//MINT		i_iSize		// 項目サイズ
				);
public:
	MsBitSet*			m_pVer;					// バージョン制御 (NULL: 全バージョンで表示)
	MsBitSet*			m_pOpt1;				// 表示オンOPT群セット (NULL: オプション設定無しでも表示)
	MsBitSet*			m_pOpt2;				// 表示オフOPT群セット (NULL: 無視)
	MUINT				m_iTenkai;				// 住棟展開制御 (NULL: 無視)
	MBYTE				m_iKai;					// 階( 1, 2, 3)
	MBYTE				m_iCdHgt;				// 取り付け高さコード
	MBYTE				m_iInpKbCd;				// 入力点区分コード
	MBYTE				m_iDisplayFlg;			// 表示フラグ(図形,注記1,注記2)
	MSHORT				m_iPanelNo;				// パネル番号
	MSHORT				m_iKaikoNo;				// 開口番号
	MSHORT				m_iIdTpPts;				// 部品ID
	MSHORT				m_iMbrId;				// 寸法型式ID
	MhTpPts*			m_pTpPts;				// 部品種類
	MhMbr*				m_pMbr;					// 寸法型式
	MgLine3				m_lnPlc;				// p[0]: 始点 | 配置点
												// p[1]: 終点 | 配置方向点
	MgVect3				m_vUpPlc;				// 配置上方向(配置点の上側方向)
	MREAL				m_rSinZure;				// 材軸芯ずれ量		>0:右側、<0:左側
	MREAL				m_rPlcZure;				// 配置点ずれ量		>0:前、　<0:後
	MREAL				m_rLngHosei[2];			// 長さ補正値（始点側、終点側）
	MREAL				m_rHgt[2];				// 高さ（取り付け高さ、高さ）
	MgPoint2			m_ptCmnt1;				// 注記表示位置
	union	{									// 補助属性
		class MhAuxAttr*	m_pAuxAttr;			// 補助属性総称
		class MhTateguInfo*	m_pAuxTategu;		// 建具
		class JTTenkaiPr*	m_pAuxTenkai;		// 住棟展開
	};
	MhZukei*			m_pZukei;				// 図形

public:
	MhPlcInfo()	{
				InitPtrTypeAtr();}
	void Copy( const MhPlcInfo &Ent);
	~MhPlcInfo()	{
				FreeAllAtr();}

	void	FreeAllAtr()	{									// 属性用に確保した領域を開放する
					MBFREE( m_pVer);
					MBFREE( m_pOpt1);
					MBFREE( m_pOpt2);
					MBFREE( m_pAuxAttr);
					if ( m_pZukei) 
						{ MBFREE( m_pZukei->m_pNext);
						delete (char*)m_pZukei;
						m_pZukei = 0;
					}
				}
	MINT	Load(												// ＤＢより１レコード読み込み
						MhMdlIO*	i_phMdl						// 読み込みファイル　ハンドル
				);
	MINT	Save(												// ＤＢへ１レコード書き込み
						MhMdlIO*	i_phMdl						// 書き込みファイル　ハンドル
				);

	void	InitPtrTypeAtr()	{								// 属性領域付きの属性をイニシャライズ、領域へのポインタをクリア
								m_pVer = 0; m_pOpt1 = 0; m_pOpt2 = 0;
								m_pAuxAttr = 0; m_pZukei = 0;}
	void	SetPIVer( MsBitSet* pVer)	{						// バージョン制御
								m_pVer = pVer;}
	void	FreePIVer()		{									// バージョン制御を無しに設定
								MBFREE( m_pVer);}
	void	SetPIOpt1( MsBitSet* pOptv)	{						// 表示オンOPT群セット
								m_pOpt1 = pOptv;}				
	void	FreePIOpt1()	{									// 表示オンOPT群セットを無しに設定
								MBFREE( m_pOpt1);}			
	void	SetPIOpt2( MsBitSet* pOptv)	{						// 表示オフOPT群セット
								m_pOpt2 = pOptv;}				
	void	FreePIOpt2()	{									// ＯＰＴ群表示ＯＦＦを無しに設定
								MBFREE( m_pOpt1);}			
	void	SetPITenkai( MUINT iTenkai)	{						// 住棟展開制御
								m_iTenkai = iTenkai;}				
	void	SetPIKai( MINT iKai)	{							// 階
								m_iKai = iKai;}				
	void	SetPICdHgt( MINT iCdCdHgt)	{						// 取り付け高さコード
								m_iCdHgt = iCdCdHgt;}			
	void	SetPIInpKbCd( MINT iInpKbCd)	{					// 入力店区分コード
								m_iInpKbCd = iInpKbCd;}		
	void	SetPIDisplayFlg( MINT iDisplayFlg)	{				// 表示フラグ(図形,注記1,注記2)
								m_iDisplayFlg = iDisplayFlg;}	
	void	SetPIPanelNo( MINT iPanelNo)	{					// パネル番号
								m_iPanelNo = iPanelNo;}		
	void	SetPIKaikoNo( MINT iKaikoNo)	{					// 開口番号
								m_iKaikoNo = iKaikoNo;}		
	void	SetPIIdTpPts( MINT iIdTpPts)	{					// 部品ID
								m_iIdTpPts = iIdTpPts;}		
	void 	SetPIIdMbr( MINT iMbrId)	{						// 寸法型式ID
								m_iMbrId = iMbrId;}		
	void	SetPITpPts( MhTpPts* pTpPts)	{					// 部品種類
								m_pTpPts = pTpPts;}			
	void	SetPIMbr( MhMbr* pMbr)	{							// 寸法型式							
								m_pMbr = pMbr;}				
	void 	SetPIPlcIti( MgLine3 lnPlc)	{						// 配置位置（始点・終点）
								m_lnPlc = lnPlc;}				// p[0]: 始点 | 配置点
																// p[1]: 終点 | 配置方向点
	void 	SetPIPlcIti( MINT iti, MgPoint3 ptPlc)	{			// 配置位置（始点・終点） 
								m_lnPlc.p[iti] = ptPlc;}		// p[0]: 始点 | 配置点
																// p[1]: 終点 | 配置方向点
	void 	SetPIUpPlc( MgVect3 vUpPlc)	{						// 配置上方向
								m_vUpPlc = vUpPlc;}			
	void	SetPISinZure ( MREAL rSinZure)	{					// 材軸芯ずれ量　	>0:右側、<0:左側
								m_rSinZure = rSinZure;}		
	void 	SetPIPlcZure( MREAL rPlcZure)	{					// 配置点ずれ量		>0:前、　<0:後
								m_rPlcZure = rPlcZure;}		
	void	SetPILenHosei( MINT iti, MREAL rLngHosei)	{		// 長さ補正値（始点側(0)、終点側(1)）
								m_rLngHosei[iti] = rLngHosei;}
	void	SetPIHgt( MINT iti, MREAL rHgt)	{					// 高さ（取り付け高さ(0)、高さ(1)）
								m_rHgt[iti] = rHgt;}			
	void	SetPICmntPt( MgPoint2	ptCmnt)	{					// 注記表示位置
								m_ptCmnt1 = ptCmnt;}			
	void	SetPIAuxAttr( class MhAuxAttr* pAuxAttr)	{		// 補助属性　総称
								m_pAuxAttr = pAuxAttr;}		
	void	SetPIAuxTategu( MhTateguInfo* pAuxTategu)	{		// 補助属性　建具
								m_pAuxTategu = pAuxTategu;}	
	void	SetPIAuxTenkai( JTTenkaiPr* pAuxTenkai)	{			// 補助属性　住棟展開
								m_pAuxTenkai = pAuxTenkai;}		
	void 	SetPIZukei( MhZukei* pZukei)	{					// 図形								
								m_pZukei = pZukei;}				

	void	SetPIMaeHosei( MREAL rMaeHosei)	{					// 手前側補正
								m_rSinZure = rMaeHosei;}
	void	SetPIOku( MREAL rOku)	{							// 奥行
								m_rPlcZure = rOku;}
	void	SetPIOkuHosei( MREAL rOkuHosei)	{					// 奥側補正
								m_rHgt[1] = rOkuHosei;}

	MsBitSet*	GetPIVer()	{	
								return m_pVer;}					// バージョン制御
	MsBitSet*	GetPIOpt1()	{	
								return m_pOpt1;}				// 表示オンOPT群セット
	MsBitSet*	GetPIOpt2()	{	
								return m_pOpt2;}				// 表示オフOPT群セット
	MUINT		GetPITenkai()	{
								return m_iTenkai;}				// 住棟展開制御
	MINT		GetPIKai()	{
								return m_iKai;}					// 階
	MINT		GetPICdHgt()	{
								return m_iCdHgt;}				// 取り付け高さコード
	MINT		GetPIInpKbCd()	{
								return m_iInpKbCd;}				// 入力点区分コード
	MINT		GetPIDisplayFlg()	{
								return m_iDisplayFlg;}			// 表示フラグ(図形,注記1,注記2)
	MINT		GetPIPanelNo()	{
								return m_iPanelNo;}				// パネル番号
	MINT		GetPIKaikoNo()	{
								return m_iKaikoNo;}				// 開口番号
	MINT		GetPIIdTpPts()	{
								return m_iIdTpPts;}				// 部品ID
	MINT 		GetPIIdMbr()	{
								return m_iMbrId;}				// 寸法型式ID
	MhTpPts*	GetPITpPts()	{
								return m_pTpPts;}				// 部品種類
	MhMbr*		GetPIMbr()		{	
								return m_pMbr;}					// 寸法型式
	MgLine3 	GetPIPlcIti()	{
								return m_lnPlc;}				// p[0]: 始点 | 配置点
																// p[1]: 終点 | 配置方向点
	MgPoint3 	GetPIPlcIti( MINT iti)	{ 
								return m_lnPlc.p[iti];}			// p[0]: 始点 | 配置点
																// p[1]: 終点 | 配置方向点
	MgVect3 	GetPIUpPlc()	{
								return m_vUpPlc;}				// 配置上方向
	MREAL		GetPISinZure ()	{
								return m_rSinZure;}				// 材軸芯ずれ量　	>0:右側、<0:左側
	MREAL 		GetPIPlcZure()	{
								return m_rPlcZure;}				// 配置点ずれ量		>0:前、　<0:後
	MREAL*		GetPILenHosei()	{
								return m_rLngHosei;}			// 長さ補正値（始点側(0)、終点側(1)）
	MREAL		GetPILenHosei( MINT iti)	{
								return m_rLngHosei[iti];}		// 長さ補正値（始点側(0)、終点側(1)）
	MREAL*		GetPIHgt()		{	
								return m_rHgt;}					// 高さ（取り付け高さ(0)、高さ(1)）
	MREAL		GetPIHgt( MINT iti)	{
								return m_rHgt[iti];}			// 高さ（取り付け高さ(0)、高さ(1)）
	MgPoint2	GetPICmntPt()	{
								return m_ptCmnt1;}				// 注記表示位置
	class MhAuxAttr*	GetPIAuxAttr()	{
								return m_pAuxAttr;}				// 補助属性　総称
	MhTateguInfo*	GetPIAuxTategu()	{
								return m_pAuxTategu;}			// 補助属性　建具
	JTTenkaiPr* GetPIAuxTenkai()	{
								return m_pAuxTenkai;}			// 補助属性　住棟展開
	MhZukei*	GetPIZukei()	{
								return m_pZukei;}				// 図形
	MREAL		GetPIMaeHosei()	{
								return m_rSinZure;}				// パネル手前側補正値
	MREAL		GetPIOku()	{	
								return m_rPlcZure;}				// パネル奥行長
	MREAL		GetPIOkuHosei()	{
								return m_rHgt[1];}				// パネル奥側補正値

//G	MINT	GetPTId()		{		
//								return m_pTpPts->m_iPTId;}
	MINT	GetPTCdGp()		{		
								return m_pTpPts->m_pPTGp->m_iCdGp;}	// 構成(組)コード( 屋根、小屋、天井、耐力壁、壁、床、基礎)
	MINT	GetPTBr()		{		
								return m_pTpPts->m_iPTBr;}			// 分類コード("部材", "金物", "パネル", "屋根", "壁", "基礎", "線分", "区画", "注記", "その他")
	MCHAR*	GetPTNmGeneral(){
								return m_pTpPts->m_sPTNmGeneral;}	// 総称
	MCHAR*	GetPTNmPts1()	{	
								return m_pTpPts->m_sPTNmPts1;}		// 操作用部材名1
	MCHAR*	GetPTNmPts2()	{	
								return m_pTpPts->m_sPTNmPts2;}		// 積算用部材名2
	DWORD	GetPTColor()	{	
								return m_pTpPts->m_dPTColor;}		// 色
	MINT	GetPTCdBuzai()	{	
								return m_pTpPts->m_iPTCdBuzaiR;}	// 部材コード	読込み用
	MINT	GetPTCdBuzaiW()	{	
								return m_pTpPts->m_iPTCdBuzaiW;}	// 部材コード	書き込み用(部材種類テーブルVerUpメインテナンス用)
	MCHAR*	GetPTTpMbr()	{	
								return m_pTpPts->m_sPTTpMbr;}		// 寸法型式選択用種類コード
	MCHAR*	GetPTCdMbr()	{
								return m_pTpPts->m_sPTCdMbr;}		// 代表寸法型式
	MINT	GetPTInpKb()	{	
								return m_pTpPts->m_iPTKbInp;}		// 入力コード
	MSTNDH	GetPTCdHgt()	{	
								return m_pTpPts->m_iPTCdHgt;}		// 取り付け高さ基準コード
	MINT	GetPTCdPlc()	{	
								return m_pTpPts->m_iPTCdPlc;}		// 配置コード
	MINT	GetPTCdIzon()	{	
								return m_pTpPts->m_iPTCdIzon;}		// 依存コード
	MINT	GetPTCdMarume()	{	
								return m_pTpPts->m_iPTCdMarume;}	// 丸めコード
	MINT	GetPTCdToritk()	{	
								return m_pTpPts->m_iPTCdToritk;}	// 取り付けコード　上下付けコード (0:下付け, 1:上付け)

	MCHAR*	GetMbCdMbr()	{	
								return m_pMbr->m_cMbCode;}			// 寸法型式
	MCHAR*	GetMbCdMbrW()	{	
								return m_pMbr->m_cMbCodeW;}			// 寸法型式 (書き込み用　通常m_cCodeと同じ VerUp時に変更した内容が入る)
	MREAL	GetMbHeight()	{	
								return m_pMbr->m_rMbHeight;}		// 部材高さ
	MREAL	GetMbWidth()	{	
								return m_pMbr->m_rMbWidth;}			// 部材幅
	MREAL	GetMbWidthR()	{	
								return m_pMbr->m_rMbWidth * 0.5f + m_pMbr->m_rMbSZure;}	// 部材右側幅
	MREAL	GetMbWidthL()	{	
								return m_pMbr->m_rMbWidth * 0.5f - m_pMbr->m_rMbSZure;}	// 部材左側幅
	MREAL	GetMbTWidth()	{	
								return m_pMbr->m_rMbHeight;}		// たて枠用巾
	MREAL	GetMbTWidthR()	{	
								return m_pMbr->m_rMbHeight * 0.5f;}	// たて枠用右側幅
	MREAL	GetMbTWidthL()	{	
								return m_pMbr->m_rMbHeight * 0.5f;}	// たて枠用左側幅
	MREAL	GetMbTHeight()	{	
								return m_pMbr->m_rMbWidth;}			// たて枠用厚さ
	MREAL	GetMbTHeightF()	{	
								return m_pMbr->m_rMbWidth * 0.5f;}	// たて枠用厚さ/2.
	MREAL	GetMbTHeightB()	{	
								return m_pMbr->m_rMbWidth * 0.5f;}	// たて枠用厚さ/2.
	MCHAR*	GetMbType()		{		
								return m_pMbr->m_cMbType;}			// 寸法型式選択用種類コード

	bool	IsYane()		{		
								return m_pTpPts->IsYane();}			// 屋根
	bool	IsTenjo()		{		
								return m_pTpPts->IsTenjo();}		// 天井
	bool	IsYuka()		{		
								return m_pTpPts->IsYuka();}			// 床
	bool	IsKiso()		{		
								return m_pTpPts->IsKiso();}			// 基礎
 
	bool	IsTategu()		{		
								return m_pTpPts->IsTategu();}		// 建具
	bool	IsTenkai()		{		
								return m_pTpPts->IsTenkai();}		// 住棟展開
	bool	IsFrame()		{		
								return m_pTpPts->IsFrame();}		// 垂直( たて枠| 束)
	bool	IsPanel()		{		
								return m_pTpPts->IsPanel();}		// パネル
	bool	IsKaiko()		{		
								return m_pTpPts->IsKaiko();}		// 開口
	bool	IsKabe()		{		
								return m_pTpPts->IsKabe();}			// 壁
	bool	IsTaiKabeArea()	{	
								return m_pTpPts->IsTaiKabeArea();}	// 耐力壁区画
	bool	IsTaiKabeLine()	{	
								return m_pTpPts->IsTaiKabeLine();}	// 耐力壁線
	bool	IsSijiKabeLine()	{
								return m_pTpPts->IsSijiKabeLine();}	// 支持壁線
	bool	IsOukaZai()		{		
								return m_pTpPts->IsOukaZai();}		// 横袈材
	bool	IsYukaPanel()	{	
								return m_pTpPts->IsYukaPanel();}	// 床パネル
	bool	IsKabePanel()	{	
								return m_pTpPts->IsKabePanel();}	// 壁パネル
	bool	IsTenjoPanel()	{	
								return m_pTpPts->IsTenjoPanel();}	// 天井パネル
	bool	IsYanePanel()	{	
								return m_pTpPts->IsYanePanel();}	// 屋根パネル
	
	void	Print(MCHAR *s);
};

} // namespace MC
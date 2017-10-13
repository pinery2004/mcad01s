//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MsDispModel.cpp
//
//		家モデルのディスプレイリスト作成
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MCAD.h"

#include "MmWnd.h"

#include "MgLib.h"
#include "MmDefine.h"
#include "MmSystemProperty.h"

#include "MsBitSet.h"
#include "MbList.h"
#include "MhLib.h"

#include "MgGrp.h"
#include "MhRoof.h"
#include "MdLib.h"

#include "MhInp.h"

#include "MmOpt.h"
#include "MmHist.h"

#include "MdFig.h"

#include "MdDispList.h"
#include "MmValid.h"

#define	DLL_EXPORT_MDLDISPLIST_DO
#include "MnMdlDispList.h"
#include "MnLib.h"

#include <GL/gl.h>						//ADD
#include <GL/glu.h>						//ADD
#include <GL/glut.h>					//ADD
#include "GlLib.h"						//ADD

#define		MP_PANEL_LINE			1
#define		MP_PANEL_RECTANGULAR	2

namespace MC
{

////////////////////////////////////////////////////////////////////////////
//	【機能】家モデルのMINMAXを取得する
//
	void MdlDispList::MhGetMinMaxMdl( MgMinMaxR3	*prMmMdl)
{
	MhPlcInfo*		pPlcEn;
	MPOSITION		posH;
	MgMinMaxR3		rMmMdl;
	MINT			maxKai;
	MhRoofInfo*		pRoofEn;
	MPOSITION		posR;
	MgLine3			LnPlc;

	MREAL			rRoofZ;
	MINT			ic1, ic2;
	MgPoint3		Py1;


	// 家モデルのMINMAXを取得する
	rMmMdl.SetInit();

	maxKai = 0;
	for ( pPlcEn = HaitiDb::MdGetHeadPts( &posH); pPlcEn!=0;
		  pPlcEn = HaitiDb::MdGetNextPts( &posH)) {
		if ( !MmChkValidParts( pPlcEn))							// オプションと履歴のチェック
			continue;

//		LnPlc.Set( pPlcEn->m_lnPlc);
		LnPlc = pPlcEn->GetPIPlcIti();
		rMmMdl.Ins3( LnPlc);
		rMmMdl.min.z = 0;
		if (maxKai < pPlcEn->GetPIKai())
			maxKai = pPlcEn->GetPIKai();
	}

	rMmMdl.max.z = MmSystemProperty::GetStnd( maxKai, MM_STNDH_ROOF) + 200.f;
	for ( pRoofEn = HaitiDb::MdGetHeadRoof( &posR); pRoofEn!=0;
		  pRoofEn = HaitiDb::MdGetNextRoof( &posR)) {
		if ( !MmChkValidRoof( pRoofEn))							// オプションと履歴のチェック
			continue;
		rRoofZ = MmSystemProperty::GetStnd( pRoofEn->GetKai(),MM_STNDH_ROOF);
		for ( ic1=0; ic1<pRoofEn->GetpGRfm()->m_n; ic1++) {
			for ( ic2=0; ic2<pRoofEn->GetpGRfm()->m_pst[ic1].m_Pg.m_n; ic2++) {
				Py1 = pRoofEn->GetpGRfm()->m_pst[ic1].m_Pg.m_P[ic2];
				rMmMdl.Ins3( Py1);
			}
		}
	}
	*prMmMdl = rMmMdl;
}

////////////////////////////////////////////////////////////////////////////
//	【機能】壁のディスプレイリストを作成する 実態、半透明
//			
void MdlDispList::DrawKabe(
						MhPlcInfo	*i_pPlcEn,		// 壁
				const	MgPoint3	&PtCtr,			// 構造家モデルの中心
						MREAL		rB				// ３次元表示倍率
				)
{
	MINT		ist1;	
	MREAL		rZU, rZL;
	MINT		ic1;

	MgLine3		LnKabe0;
	MgVect3		vuLnKabe0;
	MgLine3		LnKabe1, LnKabe2;
	MgLine3		LnKabe3;
	MgVect3		vl;
	MREAL		dl;

	MGPOLYG3( PgW, 4);
	MGPOLYG3( PgR, 4);
	MGPOLYG3( PgL, 4);
	MgGPolyg3	GPgR;
	MgGPolyg3	GPgL;

	rZU = MmSystemProperty::GetStnd( i_pPlcEn->GetPIKai(), MM_STNDH_CEILING);	// 天井基準
	rZL = MmSystemProperty::GetStnd( i_pPlcEn->GetPIKai(), MM_STNDH_LOWER);	// 下基準
	LnKabe0 = i_pPlcEn->GetPIPlcIti();
	vuLnKabe0 = LnKabe0.Vu();
	LnKabe1 = i_pPlcEn->GetPIZukei()->m_lnZukei[0];				// 右側壁面線分
	LnKabe2 = i_pPlcEn->GetPIZukei()->m_lnZukei[1];				// 左側壁面線分

	// 右側壁面
	PgW.m_n = 4;
	PgW[0] = MgPoint3C( MgPoint2C( LnKabe1.p[0]), rZL);
	PgW[1] = MgPoint3C( MgPoint2C( LnKabe1.p[1]), rZL);
	PgW[2] = MgPoint3C( MgPoint2C( LnKabe1.p[1]), rZU);
	PgW[3] = MgPoint3C( MgPoint2C( LnKabe1.p[0]), rZU);
	GPgR += PgW;

	// 左側壁面
	PgW.m_n = 4;
	PgW[0] = MgPoint3C( MgPoint2C( LnKabe2.p[0]), rZL);
	PgW[1] = MgPoint3C( MgPoint2C( LnKabe2.p[1]), rZL);
	PgW[2] = MgPoint3C( MgPoint2C( LnKabe2.p[1]), rZU);
	PgW[3] = MgPoint3C( MgPoint2C( LnKabe2.p[0]), rZU);
	GPgL += PgW;

	// 建具の開口と重なり有りなら開口を追加する
	MhPlcInfo *pPlcTEn;
	MPOSITION	PtsPos;
	MhTateguInfo	*pAuxTategu;
	for ( pPlcTEn = HaitiDb::MdGetHeadPts( &PtsPos); pPlcTEn!=0;
		  pPlcTEn = HaitiDb::MdGetNextPts( &PtsPos)) {
		if ( !MmChkValidParts( i_pPlcEn))						// オプションと履歴のチェック
			continue;

		if ( pPlcTEn->GetPIKai() == i_pPlcEn->GetPIKai() &&
			Mstrcmp( (&pPlcTEn->GetPITpPts()->GetPTNmGeneral()[2]), Mstr( "建具")) == 0) {
			if ( MGeo::ChkLn3OnLn3WS( pPlcTEn->GetPIPlcIti(), i_pPlcEn->GetPIPlcIti(), &ist1)) {
				MdlDispList::TateguHole( i_pPlcEn, pPlcTEn, &PgR, &PgL);
				pAuxTategu = (MhTateguInfo*)pPlcTEn->GetPIAuxTategu();
				if ( MGeo::GE( pAuxTategu->GetHeight(), rZU) ||
					 MGeo::LE( pAuxTategu->GetHeight() - pAuxTategu->GetROH(),  0.) ||
					 MGeo::Abs( ist1) == 2) {						// 壁と建具が部分的に重なる
					 PgR.Rev();
					 PgL.Rev();
					 MGeo::MergeGPgPgtoGPg3( MC_DIFF, GPgR, PgR, &GPgR);		// (  O) 穴付き多角形3
					 MGeo::MergeGPgPgtoGPg3( MC_DIFF, GPgL, PgL, &GPgL);		// (  O) 穴付き多角形3
				} else {										// 建具が壁の中に含まれ
					GPgR += PgR;
					GPgL += PgL;
				}
			}
		}
	}

	MBTRCPRINTS( L"３Ｄ壁デバッグ");

	DispList::DrawPolygon( GPgR, PtCtr, rB);

	GPgR.Print( Mstr( "KabeR"));

	DispList::DrawPolygon( GPgL, PtCtr, rB);

	GPgL.Print( Mstr( "KabeL"));

	//　壁上下面
	dl = (LnKabe1.p[0] - LnKabe0.p[0]) * vuLnKabe0;				// 交差部の隙間を埋める
	if (dl > MGPTOL->D)
		LnKabe1.p[0] -= (dl * vuLnKabe0);
	dl = (LnKabe1.p[1] - LnKabe0.p[1]) * vuLnKabe0;
	if (dl < - MGPTOL->D)
		LnKabe1.p[1] -= (dl * vuLnKabe0);
	dl = (LnKabe2.p[1] - LnKabe0.p[0]) * vuLnKabe0;
	if (dl > MGPTOL->D)
		LnKabe2.p[1] -= (dl * vuLnKabe0);
	dl = (LnKabe2.p[0] - LnKabe0.p[1]) * vuLnKabe0;
	if (dl < - MGPTOL->D)
		LnKabe2.p[0] -= (dl * vuLnKabe0);
	LnKabe2.SetRev();
	DispList::DspQuads( DPtoDSP( LnKabe2, rZL, PtCtr, rB), DPtoDSP( LnKabe1, rZL, PtCtr, rB));			// 壁下面
	DispList::DspQuads( DPtoDSP( LnKabe1, rZU, PtCtr, rB), DPtoDSP( LnKabe2, rZU, PtCtr, rB));			// 壁上面

	for ( ic1=2; ic1<i_pPlcEn->GetPIZukei()->m_nZukei; ic1++) {
		LnKabe3 = i_pPlcEn->GetPIZukei()->m_lnZukei[ic1];		// 壁端部面線分
		DispList::DspQuads( DPtoDSP( LnKabe3, rZL, PtCtr, rB), DPtoDSP( LnKabe3, rZU, PtCtr, rB));		// 壁端部面
	}
}

////////////////////////////////////////////////////////////////////////////
//	【機能】屋根のディスプレイリストを作成する　実体用
//			
void MdlDispList::DrawYane1(
						MhRoofInfo	*pRoofEn,		// 横置部材
				const	MgPoint3	&PtCtr,			// 構造家モデルの中心
						MREAL		rB				// ３次元表示倍率
				)
{
	MINT			ist1;
	MINT			ic0, ic1, ic2;
	MINT			ic0b;
	MgVect3			VtU, VtL;
	MgLine3			LnU, LnL;
	MgLine3			LncU, LncL;
	MREAL			rZ;
	MgGPolyg3		GPg2(10);
	MGPOLYG3( Pg3U, 3);
	MGPOLYG3( Pg3L, 3);
	MhRfm			*pRfm2;

	for ( ic1=0; ic1<pRoofEn->GetpGRfm()->m_n; ic1++) {
		pRfm2 = &pRoofEn->GetpGRfm()->m_pst[ic1];
		if ( MGeo::Zero( pRfm2->m_Pln.v.z, MGPTOL->A)) {
			// 矢切
			continue;
		} else {
			// 屋根面
			MREAL	rYaneHHosei = 130.f / pRfm2->m_Pln.v.z;

			VtU = MgVect3( 0., 0., rYaneHHosei);
			VtL = MgVect3( 0., 0., 0.);
			
//			DispList::SetMaterial( 0, 0);
		}
		ist1 = MGeo::DivideTriPg3( pRfm2->m_Pg, pRfm2->m_Pln.v, &GPg2);
		rZ = MmSystemProperty::GetStnd( pRoofEn->GetKai(), MM_STNDH_ROOF);
		for ( ic2=0; ic2<GPg2.m_n; ic2++) {
			DPtoDSP3R( GPg2.m_Pg[ic2], rZ, VtL, PtCtr, rB, &Pg3L);
			DPtoDSP3( GPg2.m_Pg[ic2], rZ, VtU, PtCtr, rB, &Pg3U);
			DispList::DspTriangles( Pg3L, -pRfm2->m_Pln.v);
			DispList::DspTriangles( Pg3U, pRfm2->m_Pln.v);
		}
		for ( ic0b=pRfm2->m_Pg.m_n-1,ic0=0; ic0<pRfm2->m_Pg.m_n; ic0b=ic0,ic0++) {
			if (pRfm2->m_cdPg.m_i[ic0] == MHRL_NOKI || pRfm2->m_cdPg.m_i[ic0] == MHRL_KERABA) {
				LnU = MgLine3( pRfm2->m_Pg.m_P[ic0b], pRfm2->m_Pg.m_P[ic0]) + VtU;
				LnL = MgLine3( pRfm2->m_Pg.m_P[ic0b], pRfm2->m_Pg.m_P[ic0]) + VtL;
				LncU = DPtoDSP( LnU, rZ, PtCtr, rB);
				LncL = DPtoDSP( LnL, rZ, PtCtr, rB);
				DispList::DspQuads( LncL, LncU);
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////
//	【機能】矢切のディスプレイリストを作成する　実体用
//			
void MdlDispList::DrawYagiri(
						MhRoofInfo	*pRoofEn,		// 横置部材
				const	MgPoint3	&PtCtr,			// 構造家モデルの中心
						MREAL		rB				// ３次元表示倍率
				)
{
	MINT			ist1;
	MINT			ic0, ic1, ic2;
	MINT			ic0b;
	MgVect3			VtU, VtL;
	MgLine3			LnU, LnL;
	MgLine3			LncU, LncL;
	MREAL			rZ;
	MgGPolyg3		GPg2(10);
	MGPOLYG3( Pg3U, 3);
	MGPOLYG3( Pg3L, 3);
	MhRfm			*pRfm2;

	for ( ic1=0; ic1<pRoofEn->GetpGRfm()->m_n; ic1++) {
		pRfm2 = &pRoofEn->GetpGRfm()->m_pst[ic1];
		if ( MGeo::Zero( pRfm2->m_Pln.v.z, MGPTOL->A)) {
			// 矢切
			VtU = pRfm2->m_Pln.v * (89. * 0.5);
			VtL = -VtU;

//			DispList::SetMaterial( 0, 1);
		} else {
			// 屋根面
			continue;
		}
		ist1 = MGeo::DivideTriPg3( pRfm2->m_Pg, pRfm2->m_Pln.v, &GPg2);
		rZ = MmSystemProperty::GetStnd( pRoofEn->GetKai(), MM_STNDH_ROOF);
		for ( ic2=0; ic2<GPg2.m_n; ic2++) {
			DPtoDSP3R( GPg2.m_Pg[ic2], rZ, VtL, PtCtr, rB, &Pg3L);
			DPtoDSP3( GPg2.m_Pg[ic2], rZ, VtU, PtCtr, rB, &Pg3U);
			DispList::DspTriangles( Pg3L, -pRfm2->m_Pln.v);
			DispList::DspTriangles( Pg3U, pRfm2->m_Pln.v);
		}
		for ( ic0b=pRfm2->m_Pg.m_n-1,ic0=0; ic0<pRfm2->m_Pg.m_n; ic0b=ic0,ic0++) {
			if (pRfm2->m_cdPg.m_i[ic0] == MHRL_NOKI || pRfm2->m_cdPg.m_i[ic0] == MHRL_KERABA) {
				LnU = MgLine3( pRfm2->m_Pg.m_P[ic0b], pRfm2->m_Pg.m_P[ic0]) + VtU;
				LnL = MgLine3( pRfm2->m_Pg.m_P[ic0b], pRfm2->m_Pg.m_P[ic0]) + VtL;
				LncU = DPtoDSP( LnU, rZ, PtCtr, rB);
				LncL = DPtoDSP( LnL, rZ, PtCtr, rB);
				DispList::DspQuads( LncL, LncU);
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////
//	【機能】屋根のディスプレイリストを作成する　半透明
//			
void MdlDispList::DrawYane2(
						MhRoofInfo	*pRoofEn,		// 横置部材
				const	MgPoint3	&PtCtr,			// 構造家モデルの中心
						MREAL		rB				// ３次元表示倍率
				)
{
	MINT			ist1;
	MINT			ic0, ic1, ic2;
	MINT			ic0b;
	MgVect3			VtU, VtL;
	MgLine3			LnU, LnL;
	MgLine3			LncU, LncL;
	MREAL			rZ;
	MgGPolyg3		GPg2(10);
	MGPOLYG3( Pg3U, 3);
	MGPOLYG3( Pg3L, 3);
	MhRfm			*pRfm2;

	for ( ic1=0; ic1<pRoofEn->GetpGRfm()->m_n; ic1++) {
		pRfm2 = &pRoofEn->GetpGRfm()->m_pst[ic1];
		if ( MGeo::Zero( pRfm2->m_Pln.v.z, MGPTOL->A)) {
			// 矢切
			VtU = pRfm2->m_Pln.v * (89. * 0.5);
			VtL = -VtU;
		} else {
			// 屋根面
			VtU = MgVect3( 0., 0., 120.);
			VtL = MgVect3( 0., 0., 0.);
		}
		ist1 = MGeo::DivideTriPg3( pRfm2->m_Pg, pRfm2->m_Pln.v, &GPg2);
		rZ = MmSystemProperty::GetStnd( pRoofEn->GetKai(), MM_STNDH_ROOF);
		for ( ic2=0; ic2<GPg2.m_n; ic2++) {
			DPtoDSP3R( GPg2.m_Pg[ic2], rZ, VtL, PtCtr, rB, &Pg3L);
			DPtoDSP3( GPg2.m_Pg[ic2], rZ, VtU, PtCtr, rB, &Pg3U);
			DispList::DspTriangles( Pg3L, -pRfm2->m_Pln.v);
			DispList::DspTriangles( Pg3U, pRfm2->m_Pln.v);
		}
		for ( ic0b=pRfm2->m_Pg.m_n-1,ic0=0; ic0<pRfm2->m_Pg.m_n; ic0b=ic0,ic0++) {
			if (pRfm2->m_cdPg.m_i[ic0] == MHRL_NOKI || pRfm2->m_cdPg.m_i[ic0] == MHRL_KERABA) {
				LnU = MgLine3( pRfm2->m_Pg.m_P[ic0b], pRfm2->m_Pg.m_P[ic0]) + VtU;
				LnL = MgLine3( pRfm2->m_Pg.m_P[ic0b], pRfm2->m_Pg.m_P[ic0]) + VtL;
				LncU = DPtoDSP( LnU, rZ, PtCtr, rB);
				LncL = DPtoDSP( LnL, rZ, PtCtr, rB);
				DispList::DspQuads( LncL, LncU);
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////
//	構造家モデルのデータを全て読み込み３次元ディスプレイリスト作成用の
//  表示倍率と中心座標を求める
//
void MdlDispList::MhGetBCtr(
				MREAL		*rB,					// 表示倍率
				MgPoint3	*PtCtr					// 中心座標
				)
{
	MgMinMaxR3		rMmMdl;
	MgMinMaxR2		rmmmdl;
	MREAL			rW;
	MREAL			rWh;

//	MmWndInfo*		pWndInfo;						// カレントウィンドウを取得する
	MgPoint3		pW;

	// 家モデルのMINMAXを取得する
	MhGetMinMaxMdl( &rMmMdl);
	rW = MGMAX( (rMmMdl.max.x - rMmMdl.min.x), (rMmMdl.max.y - rMmMdl.min.y));
	if ( MGeo::LE( rW, 0.)) {
		rMmMdl.SetUnit();
	}

	// 家の中心座標、表示倍率 1/rWh
	PtCtr->Set( rMmMdl.GetCenter());
	rWh = MGMAX( rW, (rMmMdl.max.z - rMmMdl.min.z)) * 0.5f;

	*rB = 1.8f / rWh;											// 1.8倍で表示
}

////////////////////////////////////////////////////////////////////////////
//	構造家モデルの３次元ディスプレイリストを作成する	部材、基礎	実体
//	
int MdlDispList::DrawIeMdl(
						MREAL		rB,				// 表示倍率
				const	MgPoint3	&PtCtr			// 中心座標
				)
{
	MdlDispList::DrawIeMdl1( rB, PtCtr);
	MdlDispList::DrawIeMdl2( rB, PtCtr);
	MdlDispList::DrawIeMdl3( rB, PtCtr);
	MdlDispList::MakeDispListMat0( rB, PtCtr);
	MdlDispList::MakeDispListMat1( rB, PtCtr);
	MdlDispList::MakeDispListMat2( rB, PtCtr);
	return 0;
}

GLint MsDrawPlane( GLdouble w, GLdouble d ,GLdouble h);		//ADD
GLint MsDrawBox(GLdouble d, GLdouble w, GLdouble h);		//ADD
GLint MsDrawPlane( GLdouble w, GLdouble d ,GLdouble h)
{
	GLdouble norm[3];
	glPushMatrix();
	glBegin( GL_POLYGON );
		norm[0]=1;norm[1]=0;norm[2]=0;
		glNormal3dv (norm);
		glVertex3d( w/2.0 , h ,   d/2.0 );
		glVertex3d( w/2.0 , h ,  -d/2.0  );
		glVertex3d(-w/2.0 , h ,  -d/2.0  );
		glVertex3d(-w/2.0 , h ,   d/2.0  );
	glEnd();
	glPopMatrix();

	return(0) ;
}

GLint MsDrawBox(GLdouble d, GLdouble w, GLdouble h)
{
	GLdouble norm[3];
	
	glPushMatrix();
	glTranslated(-d/2.0,-w/2.0,-h/2.0);
	glBegin( GL_POLYGON );
		norm[0]=0;norm[1]=-1;norm[2]=0;
		glNormal3dv (norm);
		glVertex3d( 0 , 0 , 0 );
		glVertex3d( d , 0 , 0 );
		glVertex3d( d , 0 , h );
		glVertex3d( 0 , 0 , h );
	glEnd();
	glBegin( GL_POLYGON );
		norm[0]=0;norm[1]=-1;norm[2]=0;
		glNormal3dv (norm);
		glVertex3d( 0 , w , 0 );
		glVertex3d( d , w , 0 );
		glVertex3d( d , w , h );
		glVertex3d( 0 , w , h );
	glEnd();

	glBegin( GL_POLYGON );
		norm[0]=0;norm[1]=0;norm[2]=1;
		glNormal3dv (norm);
		glVertex3d( 0 , 0 , h );
		glVertex3d( d , 0 , h );
		glVertex3d( d , w , h );
		glVertex3d( 0 , w , h );
	glEnd();
	glBegin( GL_POLYGON );
		norm[0]=0;norm[1]=0;norm[2]=1;
		glNormal3dv (norm);
		glVertex3d( 0 , 0 , 0 );
		glVertex3d( d , 0 , 0 );
		glVertex3d( d , w , 0 );
		glVertex3d( 0 , w , 0 );
	glEnd();

	glBegin( GL_POLYGON );
		norm[0]=-1;norm[1]=0;norm[2]=0;
		glNormal3dv (norm);
		glVertex3d( 0 , 0 , 0 );
		glVertex3d( 0 , 0 , h );
		glVertex3d( 0 , w , h );
		glVertex3d( 0 , w , 0 );
	glEnd();
	glBegin( GL_POLYGON );
		norm[0]=-1;norm[1]=0;norm[2]=0;
		glNormal3dv (norm);
		glVertex3d( d , 0 , 0 );
		glVertex3d( d , 0 , h );
		glVertex3d( d , w , h );
		glVertex3d( d , w , 0 );
	glEnd();
	glPopMatrix();


	return(0) ;

}

////////////////////////////////////////////////////////////////////////////
//	構造家モデルの３次元ディスプレイリストを作成する	部材、基礎	実体
//	
MINT MdlDispList::DrawIeMdl1(
						MREAL		rB,				// 表示倍率
				const	MgPoint3	&PtCtr			// 中心座標
				)
{
	// MhPlcInfo*	pPlcEn;
	// MPOSITION	posH;

	if (  MdlDispList::OpenDL( 1))
		return 1;

	MsDrawBox( 2.0, 3.0, 1.0);			//ADD
	MsDrawPlane( 5.0, 5.0 , 0.0);		//ADD
/*
	for ( pPlcEn = HaitiDb::MdGetHeadPts( &posH); pPlcEn!=0;
		  pPlcEn = HaitiDb::MdGetNextPts( &posH)) {

		if ( !MmChkValidParts( pPlcEn))							// オプションと履歴のチェック
			continue;

		//	部材のディスプレイリストを作成する
		if (Mstrcmp( pPlcEn->GetPTNmGeneral(), Mstr( "壁")) == 0 ||
			Mstrcmp( (&pPlcEn->GetPTNmGeneral()[2]), Mstr( "建具")) == 0)			// 壁と建具は読み飛ばし
			continue;

		//	たて枠のディスプレイリストを作成する
		if (pPlcEn->IsFrame()) {
			DispList::SetMaterial( 0, 3);
			MdlDispList::DrawTatewaku( pPlcEn, PtCtr, rB);

		//	パネルのディスプレイリストを作成する
		} else if (pPlcEn->IsPanel()) {
			DispList::SetMaterial( 0, 3);
			MdlDispList::DrawPanel( pPlcEn, PtCtr, rB, 3);

		//	床、天井、屋根の開口のディスプレイリストを作成する
		} else if (pPlcEn->IsKaiko()) {
			DispList::SetMaterial( 0, 3);
			MdlDispList::DrawKaiko( pPlcEn, PtCtr, rB);

		//	基礎のディスプレイリストを作成する
		} else if (pPlcEn->IsKiso()) {
			DispList::SetMaterial( 0, 4);
			MdlDispList::DrawKiso( pPlcEn, PtCtr, rB);

		//	横置部材のディスプレイリストを作成する
		} else if (pPlcEn->IsOukaZai()) {
			DispList::SetMaterial( 0, 3);
			MdlDispList::DrawOukaZai( pPlcEn, PtCtr, rB);
		}
	}
	// 図形情報のディスプレイリストを作成する
	DispList::DrawAllEnt( PtCtr, rB);
*/
	MdlDispList::CloseDL();
	return 0;
}

////////////////////////////////////////////////////////////////////////////
//	構造家モデルの３次元ディスプレイリストを作成する	壁、矢切　マテリアルなし
//
int MdlDispList::DrawIeMdl2(
						MREAL		rB,				// 表示倍率
				const	MgPoint3	&PtCtr			// 中心座標
				)
{
	MhPlcInfo*	pPlcEn;
	MPOSITION	posH;
	MhRoofInfo*	pRoofEn;
	MPOSITION	posR;

	if ( MdlDispList::OpenDL( 2))
		return 1;

	//	壁のディスプレイリストを作成する
	for ( pPlcEn = HaitiDb::MdGetHeadPts( &posH); pPlcEn!=0;
		  pPlcEn = HaitiDb::MdGetNextPts( &posH)) {

		if ( !MmChkValidParts( pPlcEn))							// オプションと履歴のチェック
			continue;

		//	壁のディスプレイリストを作成する
		if (Mstrcmp( pPlcEn->GetPTNmGeneral(), Mstr( "壁")) == 0) {
			MdlDispList::DrawKabe( pPlcEn, PtCtr, rB);

		//	建具の開口のディスプレイリストを作成する
		} else if (Mstrcmp( (&pPlcEn->GetPTNmGeneral()[2]), Mstr( "建具")) == 0) {
			MdlDispList::DrawTategu( NULL, pPlcEn, PtCtr, rB);
		}
	}

	//	矢切のディスプレイリストを作成する
	for ( pRoofEn = HaitiDb::MdGetHeadRoof( &posR); pRoofEn!=0;
		  pRoofEn = HaitiDb::MdGetNextRoof( &posR)) {

		if ( !MmChkValidRoof( pRoofEn))							// オプションと履歴のチェック
			continue;

		MdlDispList::DrawYagiri( pRoofEn, PtCtr, rB);
	}

	MdlDispList::CloseDL();
	return 0;
}

////////////////////////////////////////////////////////////////////////////
//	構造家モデルの３次元ディスプレイリストを作成する	屋根　マテリアルなし
//
int MdlDispList::DrawIeMdl3(
						MREAL		rB,				// 表示倍率
				const	MgPoint3	&PtCtr			// 中心座標
				)
{

	MhRoofInfo*		pRoofEn;
	MPOSITION		posR;

	if ( MdlDispList::OpenDL( 3))
		return 1;

	//	屋根のディスプレイリストを作成する
	for ( pRoofEn = HaitiDb::MdGetHeadRoof( &posR); pRoofEn!=0;
		  pRoofEn = HaitiDb::MdGetNextRoof( &posR)) {
		if ( !MmChkValidRoof( pRoofEn))							// オプションと履歴のチェック
			continue;
		MdlDispList::DrawYane1( pRoofEn, PtCtr, rB);
	}

	MdlDispList::CloseDL();
	return 0;
}

} // namespace MC
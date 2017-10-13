//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MhFile.cpp
//
//		“ü—Í‘®«’l‚ğ‰Šúİ’è‚·‚é
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"

#include "MbDebug.h"
#include "MgDefine.h"
#include "MgTol.h"

#include "MgAngle.h"
#include "MgPoint.h"
#include "MgLine.h"
#include "MgPlane.h"
#include "MgPolygon.h"

#include "MmDefine.h"
#include "MsBitSet.h"
#include "MmOpt.h"
#include "MmHist.h"

#include "MbList.h"
#define DLL_EXPORT_IEMODEL_DO
#include "MhLib.h"

#include "MmLib.h"
#define	DLL_EXPORT_MHTATEGUINP_DO
#define	DLL_EXPORT_MHPLCINP_DO
#include "MhInp.h"

namespace MC
{

MhPlcInfo		MhPlcInp::m_PIInp;
MhTateguInfo	MhTateguInp::m_TateguInp;

////////////////////////////////////////////////////////////////////////////
//    “ü—Í‘®«’l‚ğ‰Šú‰»‚·‚é
void IeModel::MhInitInpAt()
{
	// •”Ş“ü—Í‘®«

	MINT	iKai;
	iKai = MC::InpMode::GetKai();
	MhPlcInp::SetKai( iKai);
	MhPlcInp::SetCdHgt( 0);
	MhPlcInp::SetPanelNo( 0);
	MhPlcInp::SetKaikoNo( 0);
//	MhPlcInp::SetIdTpPts( 0);
	MhPlcInp::SetIdMbr( 0);
//	MhPlcInp::SetpTpPts( NULL);
//Z	MhPlcInp::SetpMbr( BuzaiCode::MhGetpMbr( 0));

	MhPlcInp::SetSinZure( 0.f);
	MhPlcInp::SetPlcZure( 0.f);
	MhPlcInp::SetLenHosei( 0, 0.f);
	MhPlcInp::SetLenHosei( 1, 0.f);
	MhPlcInp::SetHgt( 0, 0.f);
	MhPlcInp::SetHgt( 1, 0.f);
	MhPlcInp::SetpAuxAttr( NULL);
/*
	// Œš‹ï“ü—Í‘®«

	MhSetG_InpTateguCode( Mstr( ""));
	MhSetG_InpTateguROW( 910.f);
	MhSetG_InpTateguROH( 1000.f);
	MhSetG_InpTateguHeight( 2100.f);
	MhSetG_InpTateguMU( 38.f, 0);
	MhSetG_InpTateguMU( 38.f, 1);
*/
}

////S MhTategu* MhpInpTategu()
////{
////	return &g_hInpTateguAttr;
////}
////MhPlcInfo* MhpInpPI()
////{
////	return &g_hInpPIAttr;
////}

} // namespace MC
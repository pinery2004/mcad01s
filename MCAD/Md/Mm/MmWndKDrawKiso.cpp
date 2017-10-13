//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MmDrag.cpp
//
//		ƒhƒ‰ƒbƒMƒ“ƒO‘€ì
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"
#include "MgLib.h"
#include "MgMat.h"
#include "MbDefine.h"
#include "MbCod.h"
#include "MmGridNum.h"
#include "MmDrag.h"
#include "MmWnd.h"
#include "MmDefine.h"
#include "MmLib.h"
#include "MmSystemProperty.h"

#include "MmCmd.h"
#include "resource.h"
#include "MsBitSet.h"
#include "MbList.h"
#include "MhLib.h"

#include "MhInp.h"

#include "MmOpt.h"
#include "MdLib.h"

#include "MmHist.h"
#include "MmValid.h"

namespace MC
{

/////////////////////////////////////////////////////////////////////////////
//	Šî‘bcü‚ð•\Ž¦‚·‚é
	void WindowCtrl::MmWndKDrawKiso(
						MbCod*		pCod,			// À•WŒn
						MINT		iKaiC,			// ŠK  	(1,2,3)
						MINT		iGpC			// \¬
				)
{
	MPOSITION	pos1;
	MhPlcInfo	*pPlcEn;
//	MsBitSet	*pOptv;
//	MsBitSet*	pHstv;
	MgPoint2	ptW[2];
	DWORD		PtsLineColor;

	MREAL		rKisoSinZ = pCod->LPtoRP( MINT( pCod->DPtoLP(1)));
	MgVect2		vKisoSinZ = MgVect2( rKisoSinZ, rKisoSinZ);

	// ‚PŠK‚Ì“y‘ä‚É‚ÍAŠî‘bcü‚ð•\Ž¦‚·‚é

	if ( iKaiC == 1 && iGpC == MP_GP_DODAI) {

		for ( pPlcEn = HaitiDb::MdGetHeadPts( &pos1); pPlcEn!=0;
			  pPlcEn = HaitiDb::MdGetNextPts( &pos1)) {

			if ( pPlcEn->IsKiso()) {								// Šî‘b(‘Ï—ÍŠî‘b)•”Þ

			if ( !MmChkValidParts( pPlcEn))							// ƒIƒvƒVƒ‡ƒ“‚Æ—š—ð‚Ìƒ`ƒFƒbƒN
				continue;

				ptW[0] = (*(MgPoint2*)&(pPlcEn->GetPIPlcIti( 0))) + vKisoSinZ;
				ptW[1] = (*(MgPoint2*)&(pPlcEn->GetPIPlcIti( 1))) + vKisoSinZ;

				PtsLineColor = MmSystemProperty::GetColor( MM_COLOR_KISOSIN);
				pCod->SetLineAttr( MPS_DASHDOT, 1, PtsLineColor);
				pCod->Line( MgLine2( ptW[0], ptW[1]));
			}
		}
	}
}

} // namespace MC
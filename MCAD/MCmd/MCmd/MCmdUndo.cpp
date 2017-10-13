//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: .cpp
//
//		
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "McAPI.h"

namespace MC
{

void MCmdLine();

////////////////////////////////////////////////////////////////////////////
//	ÇtÇmÇcÇn
void MCmdUndo()
{

	HaitiCmd::MmUndo();

	WindowCtrl::MmWndKReDraw();

//	ist1 = InpAttr::SetDialogBar( MP_GP_YUKA, MP_BR_BUZAI, Mstr( "è∞ç™ëæ"), Mstr( "210"));
//	if ( ist1 == 0)
		MCmdLine();
}

/////////////////////////////////////////////////////////////////////////////
//	ÇqÇdÇcÇn
void MCmdRedo()
{
	HaitiCmd::MmRedo();

	WindowCtrl::MmWndKReDraw();

	MCmdLine();
}

/////////////////////////////////////////////////////////////////////////////
//	ÇtÇmÇcÇnï€ë∂óÃàÊÇÃÉNÉäÉA
void MCmdClearRedo()
{
	HaitiCmd::MdClearRedo();

	WindowCtrl::MmWndKReDraw();
}

} // namespace MC
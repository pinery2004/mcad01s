//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgsEuler.cpp
//
//		ÉIÉCÉâÅ[
//
//  K.Matsu			10/07/07	Created.
//==========================================================================================
//
#include "stdafx.h"

#include "MgsEuler.h"
#include <cassert>
#include "MgsPrint.h"


namespace euler
{

	MgSolid* MgSolid::m_sol = NULL;
	MgIdS MgSolid::m_cur_id = NULL;
	MgIdS MgsFace::m_cur_id = NULL;
	MgIdS MgsVertex::m_cur_id = NULL;

}//end namespace euler


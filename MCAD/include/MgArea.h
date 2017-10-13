#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: .h
//
//		
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "MgLine.h"

namespace MC
{

struct	MgArea2H;

struct	MgArea2{
	MINT	nallocpnt;
	MINT	npnt;
	MgPoint2 *pnt;
	MINT	nAreaH;
	MgArea2H	*AreaH;
};
struct MgArea2H{
	MgArea2H	*next;
	MINT	npnt;
	MgPoint2*	pnt;
};
struct	MgGArea{
	MINT	nArea;
	MgArea2S	*Area;
};
//
/////////////////////////////////////////////////////
//
//	ŒŠ•t‘½ŠpŒ`‚g‚P‚Æ‚g‚Q‚ÌˆÊ’uŠÖŒW‚ð’²‚×‚é
MINT	MgCArea(MgGArea);

} // namespace MC
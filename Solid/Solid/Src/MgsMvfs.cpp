//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgsMvfs.cpp
//
//		処理	新規にソリッドと面、ループと頂点を作成する
//
//
//  K.Matsu			10/07/07	Created.
//==========================================================================================
//
#include "stdafx.h"
#include "MgsEuler.h"


namespace euler
{

//	新規に面とループと点を作成する
//
//	ソリッド --> 面 --> ループ -->
//
//		   ･---- 線分2 ----･
//		   | ･-- 線分1 --･ |
//		   | |		     | |	
//		   | ･--> 点 ----･ |
//		   ･-----    <-----･
//
MgSolid* MgSolid::mvfs( MgsPoint const& pt)
{
	MgSolid*	newSol;
	MgsVertex*	newVtx;
	MgsFace*	newFce;

	newSol = new MgSolid;

	//newSol->new_face()->new_loop( newSol->new_vertex( pt) );

	newVtx = newSol->new_vertex( pt);
	newFce = newSol->new_face();
	newFce->new_loop( newVtx);

	return newSol;
}




}//end namespace euler


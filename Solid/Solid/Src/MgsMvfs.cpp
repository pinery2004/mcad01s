//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgsMvfs.cpp
//
//		����	�V�K�Ƀ\���b�h�ƖʁA���[�v�ƒ��_���쐬����
//
//
//  K.Matsu			10/07/07	Created.
//==========================================================================================
//
#include "stdafx.h"
#include "MgsEuler.h"


namespace euler
{

//	�V�K�ɖʂƃ��[�v�Ɠ_���쐬����
//
//	�\���b�h --> �� --> ���[�v -->
//
//		   �---- ����2 ----�
//		   | �-- ����1 --� |
//		   | |		     | |	
//		   | �--> �_ ----� |
//		   �-----    <-----�
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


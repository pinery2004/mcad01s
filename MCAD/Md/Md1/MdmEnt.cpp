//==========================================================================================
//  Copyright(C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MdmEnt.cpp
//
//		物
//
//
//  K.Matsu           01/06/08    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"
#include "MgDefine.h"

#include "MdFig.h"
#include "MdZukei.h"
#include "MgLib.h"

#include "MdModel.h"
#include "MdmDB.h"

namespace MC
{

//===========================================================================
//		エンティテイの属性開放
//
void MdmEnt::Free()
{
	m_GidC.Free();
	m_Grfc.Free();
	switch ( m_iTpAtr)
	{
	case 0:
	case 1:
		MBDELETE( m_pAtr);
		break;
	case 2:
		((MhPlcInfo*)m_pAtr)->FreeAllAtr();
		MBDELETE( m_pAtr);
		break;
	case 3:
		((MhRoofInfo*)m_pAtr)->FreeAllAtr();
		MBDELETE( m_pAtr);
		break;
	}
	m_Fig.Free();
}

} // namespace MC
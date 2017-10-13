//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MmDrag.cpp
//
//		ドラッギング操作
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
#include "stdafx.h"
#include "MbBasic.h"
#include "MgLib.h"
#include "MgMat.h"
#define  DLL_EXPORT_HAITICMD_DO
#include "MmCmd.h"
#include "MbDefine.h"
#include "MbCod.h"
#include "MmGridNum.h"
#include "MmDrag.h"
#include "MmWnd.h"
#include "MmDefine.h"
#include "MmLib.h"
#include "MmSystemProperty.h"

#include "resource.h"

#include "MsBitSet.h"
#include "MmHist.h"

namespace MC
{

////////////////////////////////////////////////////////////////////////////
//	コマンド実行の事前準備
void HaitiCmd::MmPresetCmd()
{
	HaitiCmd::MdClearRedo();
}

} // namespace MC
#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MhIeModel.h
//
//		家モデル
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================

#include	"MmDefine.h"
#include	"MsBitSet.h"

#define		MMAX_KAI		3										// 最大階数
#define		MMAX_KUMI		10										// 最大組数
#define		MMAX_PARTSTP	300										// 最大部材種類数
#define		MMAX_MEMBER		400										// 最大寸法型式数

#define		MMAX_BZILEN		30000.									// 最大部材長さ30m
#define		MMIN_BZILEN		1.										// 最小部材長さ
#define		MMIN_BZILEN_2	(MMIN_BZILEN * MMIN_BZILEN)				// 最小部材長さ * 最小部材長さ



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
#include "MgDefine.h"

//===========================================================================
//		�}�`���Z ���e�덷

#include "MbPlex.h"

#ifdef DLL_EXPORT_TOL_DT
	#undef DLL_EXPORT_TOL_DT
#endif

#ifdef DLL_EXPORT_TOLERANCE_DO
	#pragma message( "<<< MgTol_Dt�Edll_EXport >>>")
	#define DLL_EXPORT_TOL_DT							__declspec( dllexport)		// ����DLL����̎Q�Ƃ��\�ɂ���
#else
	#ifdef DLL_NO_IMPORT_GEO_DO
//		#pragma message( "=== MgTol_Dt�Edll ===")
		#define DLL_EXPORT_TOL_DT
	#else
//		#pragma message( "=== MgTol_Dt�Edll_IMport ===")
		#define DLL_EXPORT_TOL_DT					__declspec( dllimport)
	#endif
#endif

#ifdef DLL_EXPORT_TOL
	#undef DLL_EXPORT_TOL
#endif

#ifdef DLL_EXPORT_TOLERANCE_DO
	#pragma message( "<<< MgTol�Edll_EXport >>>")
	#define DLL_EXPORT_TOL							__declspec( dllexport)		// ����DLL����̎Q�Ƃ��\�ɂ���
#else
	#ifdef _DEBUG								// Release Mode �� inline�֐���undef�ɂȂ�s��Ή�
		#ifdef DLL_NO_IMPORT_GEO_DO
//			#pragma message( "=== MgTol�Edll ===")
			#define DLL_EXPORT_TOL
		#else
//			#pragma message( "=== MgTol�Edll_IMport ===")
			#define DLL_EXPORT_TOL						__declspec( dllimport)
		#endif
	#else
//		#pragma message( "=== MgTol_Dt�Edll ===")
		#define DLL_EXPORT_TOL
	#endif
#endif

class DLL_EXPORT_TOL_DT MgTol_Dt
{
public:
//===========================================================================
//		���e�덷
//
//	static	MREAL	DN;					//	�[������p	Normarize�p�����ŏ�����
//	static	MREAL	ZN;					//	�[������p	Normarize�p�������ŏ������i���ݕs�g�p�j
	static	MREAL	D;					//	������W����p	��������		���e�덷
	static	MREAL	L;					//	����_�����W����p	��������	���e�덷
	static	MREAL	A;					//	����p�x����p	�p�x(���W�A��)	���e�덷
	static	MREAL	S;					//	���ꕽ�ʔ���p	����			���e�덷
	static	MREAL	D_2;				//	������W����p	��������		���e�덷**2
	static	MREAL	L_2;				//	����_�����W����p	��������	���e�덷**2
	static	MREAL	A_2;				//	����p�x����p	�p�x(���W�A��)	���e�덷**2
	static	MREAL	S_2;				//	���ꕽ�ʔ���p	����			���e�덷**2
};

class DLL_EXPORT_TOL MgTol: public MgTol_Dt
{
public:
	MgTol(void){}
	~MgTol(void){}

public:
	//	����
	static MREAL MgAbs(MREAL x)
						{return ((x > 0) ? x : -x);}
	static MINT MgAbs(MINT x)
						{return ((x > 0) ? x : -x);}
	static bool MgZero(MREAL x, MREAL tol = D)
						{return (x < tol && x > -tol);}
	static bool MgEqual(MREAL x, MREAL y, MREAL tol = D)
						{return (x - y < tol && y - x < tol);}
	static bool MgEQ(MREAL x, MREAL y, MREAL tol = D)					// MgEqual�Ɠ���
						{return (x - y < tol && y - x < tol);}
	static bool MgLT(MREAL x, MREAL y, MREAL tol = D)
						{return (x < y - tol);}
	static bool MgLE(MREAL x, MREAL y, MREAL tol = D)
						{return (x <= y + tol);}
	static bool MgGT(MREAL x, MREAL y, MREAL tol = D)
						{return (x > y + tol);}
	static bool MgGE(MREAL x, MREAL y, MREAL tol = D)
						{return (x >= y - tol);}
};

extern	MgTol_Dt	g_gTol;

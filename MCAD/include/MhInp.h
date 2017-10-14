////////////////////////////////////////////////////////////////////////////
//	  配置物
////////////////////////////////////////////////////////////////////////////
#pragma once

#include	"MhPlcInfo.h"

#ifdef DLL_EXPORT
	#undef DLL_EXPORT
#endif

#ifdef DLL_EXPORT_MHTATEGUINP_DO
	#pragma message( "<<< MHTATEGUINP・dll_none >>>")
	#define DLL_EXPORT							__declspec( dllexport)
#else
//	#ifdef _DEBUG								// Release Mode で inline関数がundefになる不具合対応
		#ifdef DLL_NO_IMPORT_MD_DO
			#pragma message( "=== MHTATEGUINP・dll_none ===")
			#define DLL_EXPORT
		#else
			#pragma message( "=== MHTATEGUINP・dll_IMport ===")
			#define DLL_EXPORT						__declspec( dllimport)
		#endif
//	#else
//		#pragma message( "=== MHTATEGUINP・dll_none ===")
//		#define DLL_EXPORT
//	#endif
#endif

namespace MC
{

class  DLL_EXPORT MhTateguInp
{
public:
	static MhTateguInfo		m_TateguInp;

	//	配置用建具データ入力バッファに属性を設定
	static void SetKCode( MCHAR* cKCode)	
															{ m_TateguInp.SetKCode( cKCode);}
	static void SetROW( MREAL rROW)
															{ m_TateguInp.SetROW( rROW);}
	static void SetROH( MREAL rROH)
															{ m_TateguInp.SetROH( rROH);}
	static void SetHeight( MREAL rHeight)
															{ m_TateguInp.SetHgt( rHeight);}
	static void SetMU( MREAL rMUSz, MINT ik)
															{ m_TateguInp.SetMUSz( rMUSz, ik);}
	//	配置用建具データ入力バッファから属性を取得
	static MhTateguInfo* GetpTateguInp()
															{ return &m_TateguInp;}
	static void	 GetKCode( MCHAR* cKCode)
															{ Mstrcpy( cKCode, m_TateguInp.GetKCode());}
	static MREAL GetROW()		
															{ return m_TateguInp.GetROW();}
	static MREAL GetROH()				
															{ return m_TateguInp.GetROH();}
	static MREAL GetHeight()						
															{ return m_TateguInp.GetHeight();}
	static MREAL GetMUSz( MINT ik)					
															{ return m_TateguInp.GetMUSz( ik);}

};


#ifdef DLL_EXPORT
	#undef DLL_EXPORT
#endif

#ifdef DLL_EXPORT_MHPLCINP_DO
	#pragma message( "<<< MHPLCINP・dll_none >>>")
	#define DLL_EXPORT							__declspec( dllexport)
#else
//	#ifdef _DEBUG								// Release Mode で inline関数がundefになる不具合対応
		#ifdef DLL_NO_IMPORT_MD_DO
			#pragma message( "=== MHPLCINP・dll_none ===")
			#define DLL_EXPORT
		#else
			#pragma message( "=== MHPLCINP・dll_IMport ===")
			#define DLL_EXPORT						__declspec( dllimport)
		#endif
//	#else
//		#pragma message( "=== MHPLCINP・dll_none ===")
//		#define DLL_EXPORT
//	#endif
#endif


class DLL_EXPORT MhPlcInp
{
public:
	static MhPlcInfo	m_PIInp;

	//	配置データ入力バッファに属性を設定
	static void SetKai( MINT iKai)					
															{ m_PIInp.SetPIKai( iKai);}
	static void SetCdHgt( MINT iCdHgt)				
															{ m_PIInp.SetPICdHgt( iCdHgt);}
	static void SetPanelNo( MINT iPanelNo)			
															{ m_PIInp.SetPIPanelNo( iPanelNo);}
	static void SetKaikoNo( MINT iKaikoNo)			
															{ m_PIInp.SetPIKaikoNo( iKaikoNo);}
	static void SetIdTpPts( MINT iIdTpPts)			
															{ m_PIInp.SetPIIdTpPts( iIdTpPts);}
	static void SetIdMbr( MINT iMbrId)				
															{ m_PIInp.SetPIIdMbr( iMbrId);}
	static void SetpTpPts( MhTpPts* pTpPts)		
															{ m_PIInp.SetPITpPts( pTpPts);}
	static void SetpMbr( MhMbr* pMbr)				
															{ m_PIInp.SetPIMbr( pMbr);}

	static void SetLenHosei( MINT ik, MREAL rLngHosei)
															{ m_PIInp.SetPILenHosei( ik, rLngHosei);}
	static void SetSinZure( MREAL rSinZure)		
															{ m_PIInp.SetPISinZure( rSinZure);}
	static void SetPlcZure( MREAL rPlcZure)		
															{ m_PIInp.SetPIPlcZure( rPlcZure);}
	static void SetHgt( MINT ik, MREAL rHgt)
															{ m_PIInp.SetPIHgt( ik, rHgt);}
	static void SetMaeHosei( MREAL rMaeHosei)
															{ m_PIInp.SetPISinZure( rMaeHosei);}
	static void SetOku( MREAL rOku)				
															{ m_PIInp.SetPIPlcZure( rOku);}
	static void SetOkuHosei( MREAL rOkuHosei)
															{ m_PIInp.SetPIHgt( 1, rOkuHosei);}

	static void	SetpAuxAttr( MhAuxAttr* pAuxAttr)
															{ m_PIInp.SetPIAuxAttr( pAuxAttr);}
	static void	SetpAuxTategu( MhTateguInfo* pAuxTategu)
															{ m_PIInp.SetPIAuxTategu( pAuxTategu);}
	static void	SetpAuxTenkai( JTTenkaiPr* pAuxTenkai)
															{ m_PIInp.SetPIAuxTenkai( pAuxTenkai);}
	//	配置データ入力バッファから属性を取得
	static MINT GetKai()							
															{ return m_PIInp.GetPIKai();}
	static MINT	GetCdHgt()							
															{ return m_PIInp.GetPICdHgt();}
	static MINT	GetPanelNo()						
															{ return m_PIInp.GetPIPanelNo();}
	static MINT	GetKaikoNo()						
															{ return m_PIInp.GetPIKaikoNo();}
	static MINT	GetIdTpPts()						
															{ return m_PIInp.GetPIIdTpPts();}
	static MINT	GetIdMbr()							
															{ return m_PIInp.GetPIIdMbr();}
	static MhTpPts* GetTpPts()						
															{ return m_PIInp.GetPITpPts();}
	static MhMbr*  GetMbr()						
															{ return m_PIInp.GetPIMbr();}

	static MREAL GetLenHosei( MINT ik)				
															{ return m_PIInp.GetPILenHosei( ik);}
	static MREAL GetSinZure()						
															{ return m_PIInp.GetPISinZure();}
	static MREAL GetPlcZure()						
															{ return m_PIInp.GetPIPlcZure();}
	static MREAL GetHgt( MINT ik)					
															{ return m_PIInp.GetPIHgt( ik);}
	static MREAL GetMaeHosei()						
															{ return m_PIInp.GetPISinZure();}
	static MREAL GetOku()							
															{ return m_PIInp.GetPIPlcZure();}
	static MREAL GetOkuHosei()						
															{ return m_PIInp.GetPIHgt( 1);}

	static MhAuxAttr* GetpAuxAttr()				
															{ return m_PIInp.GetPIAuxAttr();}
	static MhTateguInfo*  GetpAuxTategu()				
															{ return m_PIInp.GetPIAuxTategu();}
	static JTTenkaiPr*  GetpAuxTenkai()			
															{ return m_PIInp.GetPIAuxTenkai();}
};

} // namespace MC

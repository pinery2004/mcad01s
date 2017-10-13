#pragma once

#if defined( DBDLL )
	#define	DB_CLASS __declspec( dllexport)
	#define	DB_DECLSPEC __declspec( dllexport)
#else 
	//#if defined( DBSTATIC )
		#define	DB_CLASS 
		#define	DB_DECLSPEC 
	//#else
	//	#define	DB_CLASS __declspec( dllimport)
	//	#define	DB_DECLSPEC __declspec( dllimport)
	//#endif
#endif

namespace MC
{

// ***********************************************************
// This class acts as a singleton containing data shared
// by DLLs and the ICAD executable
//
class BHatchBuilderFactory;

//class DB_CLASS MCadSharedGlobals
class MCadSharedGlobals
	{
	public:
		static void SetMCadAppInstance( HINSTANCE hInstance );
		static HINSTANCE GetMCadAppInstance( void);
		static void SetMCadResourceInstance( HINSTANCE hInstance );
		static HINSTANCE GetMCadResourceInstance( void);
		static void SetDialogIsModal( bool bValue );
		static bool GetDialogIsModal( void);
		static void SetHatchFactory (BHatchBuilderFactory *hatchFact);
		static BHatchBuilderFactory* GetHatchFactory();
		static void SetCmdMessage( int (*pfunct)(const MCHAR* ,...));
		static void CallCmdMessage(const MCHAR* );  // message printed on command line
	private:
//		static HINSTANCE zm_hAppInstance;
//		static HINSTANCE zm_hResourceInstance;
//		static bool zm_bDialogIsModal;
//		static BHatchBuilderFactory *m_hatchFactory;
//		static int (*m_pMessageFunct) (const MCHAR*,...);

	// ***********************
	// Trick to make sure this class is never instanced
	//
	private:
		MCadSharedGlobals();
		~MCadSharedGlobals();
	};


//	int  MdAextrucpy(MREAL **extrup, MgPoint2 newextru);
void MdAstrncpy(LPTSTR *c, LPCTSTR source, int maxnc);
//	void MdMstrncpy(LPTSTR *c, LPCTSTR source, int maxnc);

class MdStr
{
public:
//b	MCHAR*	c;
	MCHAR*	c;
	
	MdStr( LPCTSTR source, int maxnc)
						{ c = NULL; if ( *source) MdAstrncpy( &c, source, maxnc);}
	~MdStr()
						{ MBDELETE( c);}
};


} // namespace MC
/* C:\ICAD\PRJ\LIB\DB\DB1.CPP
 * Copyright (C) 1988-1998 Visio Corporation. All rights reserved.
 * 
 * Abstract
 * 
 * <TODO : Add Abstract here>
 * 
 */ 

// *********************************************************************
// *  DB1.CPP                                                          *
// *  Copyright (C) 1997, 1998 by Visio Corporation                    *
// *********************************************************************

	// Moved these initializations from vars.h to here
	// DB_DEFVARS is used elsewhere, so is still defined
#include "stdafx.h"
#define DB_DEFVARS 1

#define DLL_EXPORT_CMDS_DO

#include "MdCmds.h"

namespace MC
{

#ifdef DWGCNVT
    char *db_icfontnm="DWG",*db_icfontfn="DWG.FNT";
#else
    char *db_icfontnm="ICAD",*db_icfontfn="ICAD.FNT";
#endif

    char *db_tolfontnm="GDT";
    char *db_ourtolfontnm="IC-GDT";




// *****************************************************************
// MCadSharedGlobals class
//
// These are variables that are shared between DLLs and the ICAD
// executable.  The MCadSharedGlobals class entirely consists of
// static methods and members, so it acts as a singleton.
//
HINSTANCE MCadSharedGlobals::zm_hAppInstance = NULL;
HINSTANCE MCadSharedGlobals::zm_hResourceInstance = NULL;
int (*MCadSharedGlobals::m_pMessageFunct) (const char*,...) = NULL;

void MCadSharedGlobals::SetMCadAppInstance( HINSTANCE hInstance )
{
	zm_hAppInstance = hInstance;
}

HINSTANCE MCadSharedGlobals::GetMCadAppInstance( void )
{
	ASSERT( zm_hAppInstance != NULL );
	return zm_hAppInstance;
}

void MCadSharedGlobals::SetMCadResourceInstance( HINSTANCE hInstance )
{
	zm_hResourceInstance = hInstance;
}

HINSTANCE MCadSharedGlobals::GetMCadResourceInstance( void )
{
	// This ASSERT gets triggered during startup, so it is off for now
	//
//	ASSERT( zm_hResourceInstance != NULL );
	return zm_hResourceInstance;
}

void MCadSharedGlobals::SetCmdMessage(int (*pfunct)(const char *,...))
{
	m_pMessageFunct = pfunct;
}

void MCadSharedGlobals::CallCmdMessage(const char *message)
{
	if ( m_pMessageFunct)
		(*m_pMessageFunct)(message);
}

static ProgressMeter *gpProgressMeter = NULL;
void SetAdviseProgressMeter( ProgressMeter *pMeter )
{
	gpProgressMeter = pMeter;
}


void	(*db_grafreedofp)(void *) = NULL;
int		db_lasterror = 0;
int		db_sysvarbufsz[3];

int db_progressstart() 
{
	if ( gpProgressMeter ) {
		gpProgressMeter->Start();
		return RTNORM;
	}
	return RTERROR;	
}
	
int db_progressstop() 
{
	if ( gpProgressMeter ) {
		gpProgressMeter->Stop();
		return RTNORM;
	}
	return RTERROR;	
}
	
int db_progresspercent(int percentDone) 
{
	if ( gpProgressMeter ) {
		gpProgressMeter->Percent( percentDone);
		return RTNORM;
	}
	return RTERROR;	
}

} // namespace MC	

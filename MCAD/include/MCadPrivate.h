#pragma once
/* D:\ICADDEV\PRJ\ICAD\ICADPRIVATE.H
 * Copyright (C) 1997-1998 Visio Corporation. All rights reserved.
 * 
 * Abstract
 * 
 * Definitions not shared outside ICAD directory
 * 
 */ 

namespace MC
{

struct MT_AliasList 
	{
    MCHAR  *alias;				// Global alias
	int	   localAliasIndex;		// Index to local alias in resource file
    MCHAR  *GName;				// Global command name
    int    cmdIndex;			// Index to local command name in resource file
    MCHAR  *LName;				// Local command name
    MCHAR  *prompt;				// Used only if its a shell command.
    int    retval;
    int    crmode;
    int    flags;
    struct SafeAliasList *next;
	};


struct SafeAliasList : MT_AliasList
	{
	SafeAliasList()
		{
		alias = NULL;
		GName = NULL;
		LName = NULL;
		prompt = NULL;
		next = NULL;
		
		cmdIndex = 0;
		retval = 0;
		crmode = 0;
		flags = 0;
		}

	~SafeAliasList()
		{
		}

	SafeAliasList& operator =( SafeAliasList& other)
		{
		delete [] alias;
		delete [] GName;
		delete [] LName;
		delete [] prompt;

		alias = other.alias;
		GName = other.GName;
		LName = other.LName;
		prompt = other.prompt;

		other.alias = NULL;
		other.GName = NULL;
		other.LName = NULL;
		other.prompt = NULL;
		
		return *this;
		}
	};


} // namespace MC
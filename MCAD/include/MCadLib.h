#pragma once
#include "Md.h"

#define MC_MCADBUF    512   /* max mcad buffer for getstring & tile    */
//#define MC_MCADNMLEN  256   /* number of MCHAR in MCAD name            */
//#define MC_SLBHEADER  32L   /* length of slide library header          */
//#define MC_SLBDIRHEAD 36L   /* length of slide library directory       */
#define MC_XDLIMIT    16384 /* max length of total eed                 */

//#define MC_Pt2Cpy(P0,P1) (P0)[0]=(P1)[0],(P0)[1]=(P1)[1]
//#define MC_Pt3Cpy(P0,P1) (P0)[0]=(P1)[0],(P0)[1]=(P1)[1],(P0)[2]=(P1)[2]
//#define MC_EnCpy(E0,E1) (E0)[0]=(E1)[0],(E0)[1]=(E1)[1]

	// ResourceString -- class for loading resource strings
	// WARNING - NOT MByte Enabled - use a BSTR..
	// and don't create static string in production (resource only)
//#ifdef	NDEBUG
//#define	_ResourceString( id,string)	(__ResourceString( id,NULL))
//#else
//#define	_ResourceString( id,string)	(__ResourceString( id,string))
//#endif
//
//#define	ResourceString( id,string)	((LPCTSTR)_ResourceString( id,_T(string)))
//
//namespace MC
//{
//
////class DB_CLASS __ResourceString
//class __ResourceString
//	{
//public:
//	__ResourceString( int resourceID, LPCTSTR t);
//	~__ResourceString();
//	operator LPCTSTR();
//
//protected:
//	LPCTSTR			m_loaded;
//	LPCTSTR			m_string;
//	int				m_id;
//	TCHAR			m_buffer[32];			
//	};
//
//MCHAR          *ic_nfgets(MCHAR* s, short n, FILE *stream);
//void           ic_trim(MCHAR* ss, MCHAR* be);
//
//} // namespace MC
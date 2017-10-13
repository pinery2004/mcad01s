/* F:\BLD\PRJ\LIB\CMDS\CMDS.H
 * Copyright (C) 1997-1998 Visio Corporation. All rights reserved.
 *
 * Abstract
 *
 * <TODO : Add Abstract here>
 *
 */

#pragma once
#include "MCad.h"

#include <time.h>
#include <limits.h>

#ifdef DLL_EXPORT_CMDS
	#undef DLL_EXPORT_CMDS
#endif

#ifdef DLL_EXPORT_CMDS_DO
	#pragma message( "<<< MdCmdsÅEdll_EXport >>>")
	#define DLL_EXPORT_CMDS							__declspec( dllexport)
#else
	#ifdef DLL_NO_IMPORT_SUB_DO
		#define DLL_EXPORT_CMDS
	#else
//		#pragma message( "=== MdCmdsÅEdll_IMport ===")
		#define DLL_EXPORT_CMDS						__declspec( dllimport)
	#endif
#endif


#define IC_MCADBUF	256

//********************************************************************************
//	 These are the defines for our prompts to the user when						**
//	 something they are trying to do won't work.								**
//	 They are used with cmd_ErrorPrompt in cmds11.cpp							**
//   Look there for a sensible grouping, and to see what they do.				**
//********************************************************************************

	#define CMD_ERR_YESNO				700
	#define CMD_ERR_TRYAGAIN			701
	#define CMD_ERR_NOENT				702
	#define CMD_ERR_NOCONTINUE			703
	#define CMD_ERR_NOUNERASE			704
	#define CMD_ERR_NOUNDO				705
	#define CMD_ERR_NOMOREUNDO			706
	#define CMD_ERR_ENTTYPE				707
	#define CMD_ERR_SELECTION			708
	#define CMD_ERR_LINEAR				709
	#define CMD_ERR_LINE				710
	#define CMD_ERR_POLYLINE			711
	#define CMD_ERR_2DPOLYLINE			712
	#define CMD_ERR_ARC					713
	#define CMD_ERR_CIRCLE				714
	#define CMD_ERR_COLORS				715
	#define CMD_ERR_POLYSIDES			716
	#define CMD_ERR_DIVSEGS				717
	#define CMD_ERR_SEGSKWORD			718
	#define CMD_ERR_NUMERIC				719
	#define CMD_ERR_NONZERO				720
	#define CMD_ERR_POSITIVE			721
	#define CMD_ERR_POSNOZERO			722
	#define CMD_ERR_KWORD				723
	#define CMD_ERR_PT					724
	#define CMD_ERR_PTKWORD				725
	#define CMD_ERR_2DPT				726
	#define CMD_ERR_2DPTKWORD			727
	#define CMD_ERR_ANG2NDPT			728
	#define CMD_ERR_ANG2NDPTKWORD		729
	#define CMD_ERR_ANG2PTS				730
	#define CMD_ERR_DIST				731
	#define CMD_ERR_DIST2PTS			732
	#define CMD_ERR_DIST2PTSKWORD		733
	#define CMD_ERR_DIST2NDPT			734
	#define CMD_ERR_DIST2NDPTKWORD		735
	#define CMD_ERR_RADDCIRC			736
	#define CMD_ERR_TEXTNAME			737
	#define CMD_ERR_BLOCKNAME			738
	#define CMD_ERR_FINDBLOCK			739
	#define CMD_ERR_LTYPENAME			740
	#define CMD_ERR_FINDLTYPE			741
	#define CMD_ERR_FINDLAYER			742
	#define CMD_ERR_NULLTAG				743
	#define CMD_ERR_ELEVDIFFZ			744
	#define CMD_ERR_OBLIQUELG			745
	#define CMD_ERR_CANTCLOSE			746
	#define CMD_ERR_NOCIRCLE			747
	#define CMD_ERR_BADDATA				748
	#define CMD_ERR_REMOVEEED			749
	#define CMD_ERR_UPDATEENT			750
	#define CMD_ERR_CREATEENT			751
	#define CMD_ERR_CREATEATTDEF		752
	#define CMD_ERR_CREATETEXT			753
	#define CMD_ERR_CREATEDONUT			754
	#define CMD_ERR_MOREMEM				756
	#define CMD_ERR_FINDUCS				757
	#define CMD_ERR_DASHDOT				758
	#define CMD_ERR_LTYPEPAT			759
	#define CMD_ERR_NOSAVEDVIEWS		760
	#define CMD_ERR_NAMETOOLONG			761
	#define CMD_ERR_NOPAPERVIEW 		762
	#define CMD_ERR_COLOR1255			763
	#define CMD_ERR_COLOR0255			764
	#define CMD_ERR_COLOR1256			765
	#define CMD_ERR_COLOR0256			766
	#define CMD_ERR_SNAPTARGET			767
	#define CMD_ERR_CHAMNONPOLY			768
	#define CMD_ERR_CHAM3DPOLY			769
	#define CMD_ERR_CHAMDIFFPOLY		770
	#define CMD_ERR_CHAMNONLINEAR		771
	#define CMD_ERR_CHAMNOLENGTH		772
	#define CMD_ERR_CHAMPARALLEL		773
	#define CMD_ERR_CHAM2LINEAR			774
	#define CMD_ERR_FILPOLYSEGS			775
	#define CMD_ERR_FILNOTPOSS			776
	#define CMD_ERR_FILCLOSEFAR			777
	#define CMD_ERR_FILTOOCLOSE			778
	#define CMD_ERR_FILNOLENGTH			779
	#define CMD_ERR_FILRADIUSLG			780
	#define CMD_ERR_NOINTERSECT			781
	#define CMD_ERR_BREAKENT			782
	#define CMD_ERR_BREAKBLOCK			783
	#define CMD_ERR_MODENT				784
	#define CMD_ERR_CANTGETDATA			785
	#define CMD_ERR_CREATELINE			786
	#define CMD_ERR_BREAKDISTINCT		787
	#define CMD_ERR_CHAMDISTLG1			788
	#define CMD_ERR_CHAMANGLE			789
	#define CMD_ERR_EXTENDENT			790
	#define CMD_ERR_CLOSEARC			791
	#define CMD_ERR_UNRECOGSNAP			792
	#define CMD_ERR_UNRECOGVAR			793
	#define CMD_ERR_NUMRANGE			794
	#define CMD_ERR_FILDIFFPOLY			795
	#define CMD_ERR_GPS					796
	#define CMD_ERR_FINDHELP			797
	#define CMD_ERR_FINDTEXT			798
	#define CMD_ERR_GETDWGNAME			799
	#define CMD_ERR_OPENDWG				800
	#define CMD_ERR_UNRECOGCMD			801
	#define CMD_ERR_NODYNZOOM			802
	#define CMD_ERR_DEFBLOCK			803
	#define CMD_ERR_BUILDENT			804
	#define CMD_ERR_CANTRENAME			805
	#define CMD_ERR_RESERVEDBLOCK		806
	#define CMD_ERR_NOMODELVIEW 		807
	#define CMD_ERR_UNITTYPE			808
	#define CMD_ERR_DECIMALS			809
	#define CMD_ERR_DENOMINATOR			810
	#define CMD_ERR_NOTATABLET			811
	#define CMD_ERR_OPENGLCHOOSE		812
	#define CMD_ERR_OPENGLSET			813
	#define CMD_ERR_UNTERMINATED		814
	#define CMD_ERR_NUMTWO2DPT			815
	#define CMD_ERR_LOADDIALOG			816
	#define CMD_ERR_HATCHEXPPREV		817
	#define CMD_ERR_NOPATTERNS			818
	#define CMD_ERR_FINDPATTERN			819
	#define CMD_ERR_MIXCLOSEOPEN		820
	#define CMD_ERR_2PTSURFACE			821
	#define CMD_ERR_NOUCSDEFS			822
	#define CMD_ERR_DELETEUCS			823
	#define CMD_ERR_SAVEUCS				824
	#define CMD_ERR_FINDSNAP			825
	#define CMD_ERR_DIFFPTS				826
	#define CMD_ERR_NOPLANES			827
	#define CMD_ERR_ONSAMELINE			828
	#define CMD_ERR_MESHSIZE			829
	#define CMD_ERR_OFFSMALLRAD			830
	#define CMD_ERR_OFFZEROLEN			831
	#define CMD_ERR_PEDITPFACE			832
	#define CMD_ERR_CLOSEDPLINE			833
	#define CMD_ERR_CLOSEDMPLINE		834
	#define CMD_ERR_CLOSEDNPLINE		835
	#define CMD_ERR_NOTCLOSEDPLINE		836
//	#define CMD_ERR_NOTCLOSEDMPLINE	    NOT USED
	#define CMD_ERR_NOTCLOSEDNPLINE		837
	#define CMD_ERR_JOINCLOSEDPLINE		838
//	#define CMD_ERR_BEGOFPLINE			NOT USED
//	#define CMD_ERR_ENDOFPLINE			NOT USED
	#define CMD_ERR_BREAKDELETE			841
	#define CMD_ERR_COINCVERPTS			842
//	#define CMD_ERR_COINCENDPTS			843  Not used yet, but Rick suggested that I will need it
	#define CMD_ERR_LENGTHENENT			843
	#define CMD_ERR_LENCLOSEPOLY		844
	#define CMD_ERR_LENBYANG			845
	#define CMD_ERR_LENBYDIST			846
	#define CMD_ERR_LENBYDYNPT			847
	#define CMD_ERR_ARCSIZE				848
	#define CMD_ERR_BADENTNAME			849
	#define CMD_ERR_NOREDO  			850
	#define CMD_ERR_PARALLEL			851
	#define CMD_ERR_LINORDANG			852
	#define CMD_ERR_ARCCIRCLE			853
	#define CMD_ERR_LINEARCCIRCLE		854
	#define CMD_ERR_DIMENSION			855

	#define CMD_ERR_UNABLE				856						// Used

	#define CMD_ERR_NODIMSTYLES			857
	#define CMD_ERR_DISPDIMVARS			858
	#define CMD_ERR_LINEARCCIRPLY		859
	#define CMD_ERR_FITCURVE			860
	#define CMD_ERR_FINDVIEW			861
	//#define CMD_ERR_RADDONUT			862 **NOT USED
	#define CMD_ERR_NODWGWINDOW			863
	#define CMD_ERR_CHAMPOLYARC			864
	#define CMD_ERR_BULGESIZE			865
	#define CMD_ERR_INSERTDEF			866
	#define CMD_ERR_FINDDIMSTYLE		867
	#define CMD_ERR_NOQUICKSNAP			868
	#define CMD_ERR_CHAMFERPLADJ        869
    #define CMD_ERR_NONCOINCENDPTS      870
	#define CMD_ERR_TOOMUCHDATA			871
	#define CMD_ERR_FINDCONVFACT		872
	#define CMD_ERR_NOPREVZOOM			873
	#define CMD_ERR_UNRECOGMENU			874
	#define CMD_ERR_GRIDTOOSMALL		875
	#define CMD_ERR_GRIDTOOLARGE		876
	#define CMD_ERR_POSORZERO			877
	#define CMD_ERR_UNRECOGENTRY		878
	#define CMD_ERR_PTOUTSIDELIMITS		879
	#define CMD_ERR_NODESTROY			880
	#define CMD_ERR_CHAMTOOCLOSE		881
	#define CMD_ERR_NOEFFECT			882
	#define CMD_ERR_TILEMODE0			883
	#define CMD_ERR_TILEMODE1			884
	#define CMD_ERR_BDRYSET				885
	#define CMD_ERR_MESH3D				886
	#define CMD_ERR_NOXREFFOUND			887
	#define CMD_ERR_COPLANAR			888
	#define CMD_ERR_DUPENTITY			889
	#define CMD_ERR_OSNAPENT			890
	#define CMD_ERR_NOEXTDWGS			891
	#define CMD_ERR_INT0TO16			892
    #define CMD_ERR_NOATTRIBS           893
	#define CMD_ERR_XREFEXIST           894
	#define CMD_ERR_NOTINUCS			895
	#define CMD_ERR_FINDFILE			896
	#define CMD_ERR_GOTFILE				897
	#define CMD_ERR_BLKREF				898
	#define	CMD_ERR_XREFREF				899
	#define	CMD_ERR_ATTRIBUTE			900
	#define CMD_ERR_CHAMDISTLG2			901
	#define CMD_ERR_CHAMDISTLG3			902
    #define CMD_ERR_DUPFILE             903
    #define CMD_ERR_ZOOMIN              904
    #define CMD_ERR_ZOOMOUT             905
	#define CMD_ERR_UNSUPPORTED         906
	#define CMD_ERR_SYSVARVAL			907
	#define CMD_ERR_FILLETPLINE			908
	#define CMD_ERR_FILLETCAP			909
    #define CMD_ERR_NOTBFOUND           911
    #define CMD_ERR_NOVIEWPORTS         912
	#define CMD_ERR_XPLODXREF			913
	#define CMD_ERR_ANYNAME				914
	#define CMD_ERR_DWGHASIMAGE			915
	#define CMD_ERR_DWGHASACIS			916
	#define CMD_ERR_DWGHASHATCH			917
	#define CMD_ERR_DWGHASPROXY			918
	#define CMD_ERR_BADGEOMETRY			919
	#define CMD_ERR_NOTINPSPACE			920
    #define CMD_ERR_NOTHINGTODO         921
    #define CMD_ERR_FINDSTYLE           922
    #define CMD_ERR_NOTRANSPARENT       923
	#define CMD_ERR_PLINESEGS			924
	#define CMD_ERR_NOSELECT			925
	#define CMD_ERR_BLKNAMETOOLONG		926
    #define CMD_ERR_SURFTABS            927
    #define CMD_ERR_FINDFILE2           928
	#define CMD_ERR_HATCHBDRY			929
	#define CMD_ERR_LAYERNAME			930
	#define CMD_ERR_ANGLE				931
	#define CMD_ERR_SPHEREDIVS			932
	#define CMD_ERR_OFFSHARPCURVE		933
	#define CMD_ERR_DONTUSEEND			934
	#define CMD_ERR_OPTIONNOTSUP		935
	#define CMD_ERR_ZOOMPERCENT		    936
	#define CMD_ERR_TRIMENT				937
	#define CMD_ERR_NOVPCONFIG			938
	#define CMD_ERR_XRCURLAY			939
	#define CMD_ERR_NOFROZEN            940
    #define CMD_ERR_NOLASTAXIS          941
    #define CMD_ERR_ANGLEPI		        942
    #define CMD_ERR_INSERTACIS			943
    #define CMD_ERR_INSERTPROXY			944
    #define CMD_ERR_FINDXREF			945
    #define CMD_ERR_OPENXREF			946
    #define CMD_ERR_LOADXREF			947
    #define CMD_ERR_INSERTXREF          948
	#define CMD_ERR_INSERTHATCH         949
	#define CMD_ERR_INSERTIMAGE         950
	#define CMD_ERR_PASTEACIS           951
	#define CMD_ERR_PASTEPROXY			952
	#define CMD_ERR_PASTEHATCH			953
	#define CMD_ERR_PASTEIMAGE			954
	#define CMD_ERR_PERSPECTIVE			955
	#define CMD_ERR_UNDODISABLE         956
    #define CMD_ERR_BADARGUMENT         957
	#define CMD_ERR_MAXACTVP            958
	#define CMD_ERR_BADRENDER           959
	#define CMD_ERR_NOCHANGEABLE        960
	#define CMD_ERR_SAVEFILE			961
	#define CMD_ERR_CANTDEFUCS			962
	#define CMD_ERR_NOACTIVEVP			963
	#define CMD_ERR_SHADEPERSP			964 //temporary error stmt for shade
	#define CMD_ERR_HIDEPERSP			965 //temporary error stmt for hide
	#define CMD_ERR_EMPTYDWG			966
	#define CMD_ERR_DXFNEWONLY          967
	#define CMD_ERR_UNUSEDVARS          968
	#define CMD_ERR_EXPORTFILE          969
	#define CMD_ERR_INTRANGE            970
	#define CMD_ERR_WBLOCKACIS          971
	#define CMD_ERR_WBLOCKPROXY         972
	#define CMD_ERR_WBLOCKHATCH         973
	#define CMD_ERR_WBLOCKIMAGE         974
	#define CMD_ERR_INVALIDDLL   		975
	#define CMD_ERR_SYSNOMEM     		976
	#define CMD_ERR_ACCESS_DENIED		977
	#define CMD_ERR_OUTOFMEMORY  		978
	#define CMD_ERR_NOT_READY   		979
	#define CMD_ERR_PROC_NOTFOUND		980
	#define CMD_ERR_DLLNOTFOUND  		981
	#define CMD_ERR_DLLLOADPROB  		982
	#define CMD_ERR_CANTFINDAPP  		983
	#define CMD_ERR_UNLOADING  		    985
	#define CMD_ERR_BADENTPOINT  		986
	#define CMD_ERR_FINDUNLOAD  		987
	#define CMD_ERR_ENTSINSET  		    988
	#define CMD_ERR_CANTFINDFILE	    989
	#define CMD_ERR_CANTOPENFILE	    990
	#define CMD_ERR_CANTLOADREN			991
	#define CMD_ERR_FLATTENED			992
	#define CMD_ERR_NOTPARALLEL			993
	#define CMD_ERR_CANTFINDMENU		994
	#define CMD_ERR_CANTFINDDWG			995
	#define CMD_ERR_PLEASESPEC			996
	#define CMD_ERR_BADVPORTNUM			997
	#define CMD_ERR_GETOPENFILE			998
	#define CMD_ERR_CANTOPENDWG			999
	#define CMD_ERR_REBUILDTABLE	   1000
	#define CMD_ERR_BUILDTABLE		   1001
	#define CMD_ERR_CREATEBLOCK		   1002
	#define CMD_ERR_NOTABLEITEM		   1003
	#define CMD_ERR_ALREADYEXISTS	   1004
	#define CMD_ERR_WASRENAMEDAS	   1005
	#define CMD_ERR_EXPLODEACIS        1006
	#define CMD_ERR_UNSUPPORTEDA2KENT  1007
	#define CMD_ERR_DIMBLKDOESNOTEXIST 1008
	#define CMD_ERR_DIMBLK1DOESNOTEXIST 1009
	#define CMD_ERR_DIMBLK2DOESNOTEXIST 1010

namespace MC
{

DLL_EXPORT_CMDS void MdCmdErrorPrompt( int ErrNo, short DispMode, void *arg1=NULL, void *arg2=NULL, void *arg3=NULL);

	// ResourceString -- class for loading resource strings
	// WARNING - NOT MByte Enabled - use a BSTR..
	// and don't create static string in production (resource only)

#ifdef	NDEBUG
	#define	_ResourceString(id,string)	(__ResourceString(id,NULL))
#else
	#define	_ResourceString(id,string)	(__ResourceString(id,string))
#endif

#define	ResourceString(id,string)	((LPCTSTR)_ResourceString(id,_T(string)))

class DLL_EXPORT_CMDS __ResourceString
{
public:
	__ResourceString( int resourceID, LPCTSTR t);
	~__ResourceString();
	operator LPCTSTR();

protected:
	LPCTSTR			m_loaded;
	LPCTSTR			m_string;
	int				m_id;
	TCHAR			m_buffer[32];			
};

/*D.Gavrilov*/// The following macros are used for loading and concatenating
// resource strings which are the parts of a one string. It's for localising
// (Loc@le cannot process strings longer than 255 symbols).

#define LOADSTRINGS_2(id)	CString s_part1, s_part2, s_all;	\
							s_part1.LoadString(id##__part1);	\
							s_part2.LoadString(id##__part2);	\
							s_all = s_part1 + s_part2;
#define LOADSTRINGS_3(id)	CString s_part1, s_part2, s_part3, s_all;	\
							s_part1.LoadString(id##__part1);			\
							s_part2.LoadString(id##__part2);			\
							s_part3.LoadString(id##__part3);			\
							s_all = s_part1 + s_part2 + s_part3;
#define LOADSTRINGS_4(id)	CString s_part1, s_part2, s_part3, s_part4, s_all;	\
							s_part1.LoadString(id##__part1);					\
							s_part2.LoadString(id##__part2);					\
							s_part3.LoadString(id##__part3);					\
							s_part4.LoadString(id##__part4);					\
							s_all = s_part1 + s_part2 + s_part3 + s_part4;
#define LOADSTRINGS_5(id)	CString s_part1, s_part2, s_part3, s_part4, s_part5, s_all;	\
							s_part1.LoadString(id##__part1);							\
							s_part2.LoadString(id##__part2);							\
							s_part3.LoadString(id##__part3);							\
							s_part4.LoadString(id##__part4);							\
							s_part5.LoadString(id##__part5);							\
							s_all = s_part1 + s_part2 + s_part3 + s_part4 + s_part5;

#define LOAD_COMMAND_OPTIONS_2(id)	CString s_part1, s_part2, s_all;	\
									s_part1.LoadString(id##__part1);	\
									s_part2.LoadString(id##__part2);	\
									s_all = s_part1 + " " + s_part2;
#define LOAD_COMMAND_OPTIONS_3(id)	CString s_part1, s_part2, s_part3, s_all;	\
									s_part1.LoadString(id##__part1);			\
									s_part2.LoadString(id##__part2);			\
									s_part3.LoadString(id##__part3);			\
									s_all = s_part1 + " " + s_part2 + " " + s_part3;
#define LOAD_COMMAND_OPTIONS_4(id)	CString s_part1, s_part2, s_part3, s_part4, s_all;	\
									s_part1.LoadString(id##__part1);					\
									s_part2.LoadString(id##__part2);					\
									s_part3.LoadString(id##__part3);					\
									s_part4.LoadString(id##__part4);					\
									s_all = s_part1 + " " + s_part2 + " " + s_part3 + " " + s_part4;
#define LOAD_COMMAND_OPTIONS_5(id)	CString s_part1, s_part2, s_part3, s_part4, s_part5, s_all;	\
									s_part1.LoadString(id##__part1);							\
									s_part2.LoadString(id##__part2);							\
									s_part3.LoadString(id##__part3);							\
									s_part4.LoadString(id##__part4);							\
									s_part5.LoadString(id##__part5);							\
									s_all = s_part1 + " " + s_part2 + " " + s_part3 + " " + s_part4 + " " + s_part5;

// ***********************************************************
// ÇlÇbÇ`ÇcÇÃé¿çsÉÇÉWÉÖÅ[ÉãÇ∆ÇcÇkÇkÇ≈ã§óL
//

///////////////////
// íËêî
#define	RTNORM		0
#define	RTERROR	-5001

class DLL_EXPORT_CMDS ProgressMeter
{
public:
	virtual void Start()=0;
	virtual void Stop()=0;
	virtual void Percent( int percentDone)=0;
};

DLL_EXPORT_CMDS void SetAdviseProgressMeter( ProgressMeter *pMeter );
DLL_EXPORT_CMDS int db_progresspercent(int percentDone); 
DLL_EXPORT_CMDS int db_progressstart(); 
DLL_EXPORT_CMDS int db_progressstop(); 
//
//// date conversion
//DLL_EXPORT_CMDS void db_jul2greg(double dJul,short *pYear,short *pMonth,short *pDay,short *pDow,short *pHour,short *pMin,double *pSec);


// ***********************************************************
// ÇlÇbÇ`ÇcÇÃé¿çsÉÇÉWÉÖÅ[ÉãÇ∆ÇcÇkÇkÇ≈ã§óL
//
class DLL_EXPORT_CMDS MCadSharedGlobals
{
	public:
		static void SetMCadAppInstance( HINSTANCE hInstance );
		static HINSTANCE GetMCadAppInstance( void );

		static void SetMCadResourceInstance( HINSTANCE hInstance );
		static HINSTANCE GetMCadResourceInstance( void );

		static void SetCmdMessage(int (*pfunct)(const char *,...));
		static void CallCmdMessage(const char *);  // message printed on command line

	private:
		static HINSTANCE zm_hAppInstance;
		static HINSTANCE zm_hResourceInstance;
		static int (*m_pMessageFunct) (const char*,...);

	private:
	public:
		MCadSharedGlobals();
		~MCadSharedGlobals();
};

} // namespace MC
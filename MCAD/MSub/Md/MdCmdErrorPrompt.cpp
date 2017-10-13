/* D:\ICADDEV\PRJ\LIB\CMDS\ERRORPROMPT.CPP
 * Copyright (C) 1997-1998 Visio Corporation. All rights reserved.
 * 
 * Abstract
 * 
 * <TODO : Add Abstract here>
 * 
 */ 
//*********************************************************************
// This file has all of the EED command from AFD
//*********************************************************************
// THIS FILE HAS BEEN GLOBALIZED!
//** Includes
#include "stdafx.h"
#include "MCAD.h"

#define DLL_EXPORT_CMDS_DO

#include "MdCmds.h"/*DNT*/
#include "..\\MCAD\\res\\MCADrc2.h"

namespace MC
{

//inline
__ResourceString:: __ResourceString( int resourceID, LPCTSTR t) 
		: m_string( t), m_loaded( NULL), m_id( resourceID)
{
}


//inline this for performance if necessary
//makes rebuilds easier for now if it isn't inlined
__ResourceString:: operator LPCTSTR()
{
	if ( !m_loaded)
		{
		TCHAR	buffer[1000];

										// load string into static
		int	size = ::LoadString( MCadSharedGlobals::GetMCadResourceInstance(), m_id,
								 buffer, sizeof( buffer));
		ASSERT( size < sizeof( buffer)-1 );

										// allocate buffer if immediate too small
		LPTSTR	dest;
		if ( size < sizeof( m_buffer) )
			dest = m_buffer;
		else
			dest = new TCHAR[ size+1];

										// copy the string value
										// (dest is a non-const alias)
		if ( dest )
			_tcscpy( dest, buffer);

		m_loaded = dest;

#ifdef USBUILD
		ASSERT( strsame( m_string, m_loaded) );
#endif
		}

	return m_loaded;
}

__ResourceString:: ~__ResourceString()
{
	if ( m_loaded != m_buffer )
		delete[] const_cast<LPTSTR>(m_loaded);
}



bool        cmd_GaveACISMsg;
bool        cmd_GaveProxyMsg;
bool        cmd_GaveHatchMsg;
bool        cmd_GaveImageMsg;
bool        cmd_GaveA2KEntMsg;

/*********************************************************************
**	Function:	cmd_ErrorPrompt()				     4/15/96		**
**	Purpose:	Keeps all the error message strings in one place.	**
**                                                                  **
**  Notes:  When you make up new ones, please try to be consistent  **
**  in making up the ErrNo names, and kind to the users.			**
**  It's easy to confuse these in the ErrNo names...look out:		**
**			2DPT	2-Dimensional point, as in Pick a 2D point		**
**			2NDPT	Second point, as in Pick 2nd point				**
**			2PTS	2 Points, as in Pick two points					**
**	When you make up new ones, please follow the above convention.	**
**                                                                  **
**  - When the command line is not displayed, the error messages    **
**  appear in a message box so the user doesn't miss them.          **
**  - Be sure to add new ones to the #defines in cmds.h also.       **
**                                                                  **
**  Example:                                                        **
**          cmd_ErrorPrompt(CMD_ERR_YESNO,0);                       **
**                                                                  **
*********************************************************************/

void MdCmdErrorPrompt(int ErrNo, short DispMode, void *arg1, void *arg2, void *arg3)
{
	// mode =1 for message box by default
	// mode =0 for command line (or msg box if no command line)
	// mode=-1 for command line only

	CString	ErrPrompt;
	MCHAR	fs1[IC_MCADBUF];
//	struct resbuf rb1;

	CString strTitle;

	switch (ErrNo){
//
//		// Really generic ones, or things which aren't supported in current release...
//		case CMD_ERR_YESNO:			ErrPrompt=ResourceString(IDC_ERRORPROMPT_YES_OR_NO__P_0, "Yes or No, please." );												break;
//		case CMD_ERR_TRYAGAIN:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_PLEASE_TRY_A_1, "Please try again:" );												break;
//									// Use this especially where the command prompt is repeated after it.
//									// Avoid this where possible, as it doesn't provide any information to the user.
//									// I have tried to use it only where the prompt use to say "*Invalid*".
//		case CMD_ERR_NODWGWINDOW:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_THIS_COMMAND_2, "This command is available only when a drawing is open." );			break;
//		case CMD_ERR_NOEFFECT:      ErrPrompt=ResourceString(IDC_ERRORPROMPT_THIS_COMMAND_3, "This command had no effect." );									break;
//		case CMD_ERR_UNSUPPORTED:   ErrPrompt=ResourceString(IDC_ERRORPROMPT_THIS_COMMAND_4, "This command is not yet supported." );								break;
//		case CMD_ERR_OPTIONNOTSUP:  ErrPrompt=ResourceString(IDC_ERRORPROMPT_THIS_OPTION__5, "This option is not yet supported." );								break;
//		case CMD_ERR_NOTRANSPARENT: ErrPrompt=ResourceString(IDC_ERRORPROMPT_THIS_COMMAND_6, "This command cannot be used transparently." );						break;
//		case CMD_ERR_DONTUSEEND:    ErrPrompt=ResourceString(IDC_ERRORPROMPT_TO_CLOSE_INT_7, "To close IntelliCAD, please select Exit from the File menu." );	break;
//		case CMD_ERR_SHADEPERSP:    ErrPrompt=ResourceString(IDC_ERRORPROMPT_SHADING_IN_P_8, "Shading in Perspective mode is not yet supported." );				break;
//		case CMD_ERR_HIDEPERSP:     ErrPrompt=ResourceString(IDC_ERRORPROMPT_HIDING_IN_PE_9, "Hiding in Perspective mode is not yet supported." );				break;
//		case CMD_ERR_UNUSEDVARS:    ErrPrompt=ResourceString(IDC_ERRORPROMPT_THIS_VARIAB_10, "This variable is not yet supported. Value entered will be stored but will have no effect." );			break;
//
//		// Unsupported entities...
//		case CMD_ERR_DWGHASIMAGE:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_THIS_DRAWIN_11, "This drawing contains one or more image entities that will not display.\nThese entities WILL be stored and saved back to the drawing." );			break;
//		case CMD_ERR_DWGHASACIS:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_THIS_DRAWIN_12, "This drawing contains one or more ACIS entities that will not display.\nThese entities WILL be stored and saved back to the drawing." );			break;
//		case CMD_ERR_DWGHASHATCH:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_THIS_DRAWIN_13, "This drawing contains one or more R14 hatch entities that will not display.\nThese entities WILL be stored and saved back to the drawing." );		break;
//		case CMD_ERR_UNSUPPORTEDA2KENT:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_A2K_ENT, "This drawing contains one or more A2K-specific entities that will not display.\nThese entities WILL be stored and saved back to the drawing." );		break;
//		case CMD_ERR_DWGHASPROXY:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_THIS_DRAWIN_14, "This drawing contains one or more proxy entities that will not display.\nThese entities WILL be stored and saved back to the drawing." );		break;
//		case CMD_ERR_INSERTACIS:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_THIS_DRAWIN_15, "This drawing contains one or more ACIS entities.\nThese entities WILL NOT insert and will be ignored." );			break;
//		case CMD_ERR_INSERTPROXY:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_THIS_DRAWIN_16, "This drawing contains one or more proxy entities.\nThese entities WILL NOT insert and will be ignored." );		break;
//		case CMD_ERR_INSERTHATCH:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_THIS_DRAWIN_17, "This drawing contains one or more R14 hatch entities.\nThese entities WILL NOT insert and will be ignored." );		break;
//		case CMD_ERR_INSERTIMAGE:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_THIS_DRAWIN_18, "This drawing contains one or more image entities.\nThese entities WILL NOT insert and will be ignored." );		break;
//		case CMD_ERR_PASTEACIS:	    ErrPrompt=ResourceString(IDC_ERRORPROMPT_THIS_PASTE__19, "This paste from clipboard contains one or more ACIS entities.\nThese entities WILL NOT insert and will be ignored." );			break;
//		case CMD_ERR_PASTEPROXY:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_THIS_PASTE__20, "This paste from clipboard contains one or more proxy entities.\nThese entities WILL NOT insert and will be ignored." );		break;
//		case CMD_ERR_PASTEHATCH:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_THIS_PASTE__21, "This paste from clipboard contains one or more R14 hatch entities.\nThese entities WILL NOT insert and will be ignored." );		break;
//		case CMD_ERR_PASTEIMAGE:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_THIS_PASTE__22, "This paste from clipboard contains one or more image entities.\nThese entities WILL NOT WBLOCKed and will be ignored." );		break;
//		case CMD_ERR_WBLOCKACIS:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_WBLOCKING_O_23, "WBLOCKing one or more ACIS entities is not yet supported.\nThese entities WILL NOT WBLOCKed and will be ignored." );			break;
//		case CMD_ERR_WBLOCKPROXY:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_WBLOCKING_O_24, "WBLOCKing one or more proxy entities is not yet supported.\nThese entities WILL NOT WBLOCKed and will be ignored." );		break;
//		case CMD_ERR_WBLOCKHATCH:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_WBLOCKING_O_25, "WBLOCKing one or more R14 hatch entities is not yet supported.\nThese entities WILL NOT WBLOCKed and will be ignored." );		break;
//		case CMD_ERR_WBLOCKIMAGE:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_WBLOCKING_O_26, "WBLOCKing one or more image entities is not yet supported.\nThese entities WILL NOT WBLOCKed and will be ignored." );		break;
//
//		// There's nothing there...
//		case CMD_ERR_EMPTYDWG:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_THAT_COMMAN_27, "That command is unavailable in an empty drawing." );				break;
//		case CMD_ERR_NOLASTAXIS:    ErrPrompt=ResourceString(IDC_ERRORPROMPT_NO_PREVIOUS_28, "No previous axis to use." );	                    				break;
//		case CMD_ERR_NOENT:			ErrPrompt=ResourceString(IDC_ERRORPROMPT_NO_ENTITY_W_29, "No entity was found there. Please pick again." );					break;
//		case CMD_ERR_NOSELECT:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_NO_ENTITIES_30, "No entities selected." );											break;
//		case CMD_ERR_NOCONTINUE:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_THERE_IS_NO_31, "There is nothing from which to continue." );						break;
//		case CMD_ERR_NOUNERASE:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_THERE_IS_NO_32, "There is nothing to un-erase." );									break;
//		case CMD_ERR_NOUNDO:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_THERE_IS_NO_33, "There is nothing to undo." );										break;
//		case CMD_ERR_NOREDO:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_THERE_IS_NO_34, "There is nothing to redo." );										break;
//		case CMD_ERR_NOMOREUNDO:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_THERE_IS_NO_35, "There is nothing more to undo." );									break;
//		case CMD_ERR_NOPREVZOOM:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_THERE_IS_NO_36, "There is no previous zoom." );										break;
//		case CMD_ERR_UNDODISABLE:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNDO_REDO_I_37, "Undo/Redo is disabled, use the UNDO command to enable." );										break;
//		case CMD_ERR_DIMBLKDOESNOTEXIST: ErrPrompt=ResourceString(IDS_NO_DIMBLK, "\nDIMBLK does not exist -- cannot save." );	break;
//		case CMD_ERR_DIMBLK1DOESNOTEXIST: ErrPrompt=ResourceString(IDS_NO_DIMBLK1, "\nDIMBLK1 does not exist -- cannot save." );	break;
//		case CMD_ERR_DIMBLK2DOESNOTEXIST: ErrPrompt=ResourceString(IDS_NO_DIMBLK2, "\nDIMBLK2 does not exist -- cannot save." );	break;
//
//		// User picked the wrong kind of entity...
//		case CMD_ERR_ENTTYPE:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO_U_38, "Unable to use the selected entity. Please pick again." );			break;
//									// Mostly used in place of "*Invalid object*"
//        case CMD_ERR_SELECTION:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO_U_39, "Unable to use that selection.  Please try again:" );				break;
//        case CMD_ERR_CANTDEFUCS:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_CANNOT_DEFI_40, "Cannot define a coordinate system based upon the selected entity." );	break;
//		case CMD_ERR_PTOUTSIDELIMITS:ErrPrompt=ResourceString(IDC_ERRORPROMPT_SELECTED_PO_41, "Selected point is outside limits. Please select another point." );break;
//		case CMD_ERR_NOTINUCS:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_AN_ENTITY_W_42, "An entity which is defined in the current UCS is needed." );		break;
//		case CMD_ERR_LINEAR:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_A_LINE__ARC_43, "A line, arc, or polyline is needed. Please pick again." );			break;
//		case CMD_ERR_LINEARCCIRPLY: ErrPrompt=ResourceString(IDC_ERRORPROMPT_A_LINE__ARC_44, "A line, arc, circle, or polyline is needed. Please pick again." ); break;
//		case CMD_ERR_LINE:			ErrPrompt=ResourceString(IDC_ERRORPROMPT_A_LINE_IS_N_45, "A line is needed here. Please pick again." );						break;
//		case CMD_ERR_POLYLINE:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_A_POLYLINE__46, "A polyline is needed here. Please pick again." );					break;
//		case CMD_ERR_2DPOLYLINE:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_A_2D_POLYLI_47, "A 2D polyline is needed. Please pick again." );					break;
//		case CMD_ERR_ARC:			ErrPrompt=ResourceString(IDC_ERRORPROMPT_AN_ARC_IS_N_48, "An arc is needed here. Please pick again." );						break;
//									// For changing an arc to a circle.
//		case CMD_ERR_CIRCLE:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_A_CIRCLE_IS_49, "A circle is needed for this. Please pick again." );				break;
//		case CMD_ERR_ARCCIRCLE:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_AN_ARC_OR_C_50, "An arc or circle is needed here. Please pick again." );			break;
//		case CMD_ERR_LINEARCCIRCLE:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_A_LINE__ARC_51, "A line, arc, or circle is needed. Please pick again." );			break;
//		case CMD_ERR_DIMENSION:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_A_DIMENSION_52, "A dimension is needed. Please select again." );					break;
//		case CMD_ERR_LINORDANG:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_A_LINEAR__O_53, "A Linear, Ordinate, or Angular associative dimension is needed." );break;
//		case CMD_ERR_COPLANAR:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_COPLANAR_EN_54, "Coplanar entities are needed for this operation." );				break;
//		case CMD_ERR_ATTRIBUTE:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_A_BLOCK_WIT_55, "A block with at least one attribute is needed." ); 				break;
//		case CMD_ERR_FILLETPLINE:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO_F_56, "Unable to fillet polylines with rays,xlines and circles." );		break;
//		case CMD_ERR_FILLETCAP:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_CAP_FILLETI_57, "Cap filleting requires that the first entity be a line or ray." );	break;
//		case CMD_ERR_XPLODXREF:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_EXTERNALLY__58, "Externally referenced blocks cannot be exploded" );				break;
//		case CMD_ERR_HATCHBDRY:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO_U_59, "Unable to use the selected entities as a hatch boundary" );		break;
//		case CMD_ERR_TRIMENT:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_CANNOT_TRIM_60, "Cannot trim the selected entity" );								break;
//		case CMD_ERR_NOFROZEN:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_CURRENT_LAY_61, "Current layer cannot be a frozen layer" );							break;
//
//		// User entered a number not within the right range...
//		case CMD_ERR_ANGLE:			ErrPrompt=ResourceString(IDC_ERRORPROMPT_PLEASE_SELE_62, "Please select two points or enter a valid numeric angle." );		break;
//		case CMD_ERR_ANGLEPI:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_AN_ANGLE_BE_63, "An angle between 0 and 180 degrees is required." );				break;
//		case CMD_ERR_COLOR1255:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_AVAILABLE_C_64, "Available colors are 1-255, or you can use a color name (red, blue, etc.)." );break;
//									// This is the range for color numbers, without BYLAYER or BYBLOCK.
//		case CMD_ERR_COLOR0255:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_AVAILABLE_C_65, "Available colors are 0-255, or you can use a color name (red, blue, etc.)." );break;
//									// This is the range for color numbers, including zero, which is BYBLOCK.
//		case CMD_ERR_COLOR1256:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_AVAILABLE_C_66, "Available colors are 1-256, or you can use a color name (red, blue, etc.)." );break;
//									// This is the range for color numbers, including 256, which is BYLAYER.
//		case CMD_ERR_COLOR0256:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_AVAILABLE_C_67, "Available colors are 0-256, or you can enter a color name (red, blue, etc.)." );break;
//									// This is the range for color numbers, including BYBLOCK(0) and BYLAYER(256).
//		case CMD_ERR_INT0TO16:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_A_NUMBER_BE_68, "A number between 0 and 16 is needed." );							break;
//		case CMD_ERR_ZOOMPERCENT:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_A_NUMBER_BE_69, "A number between 1 and 20000 is needed." );						break;
//		case CMD_ERR_POLYSIDES:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_A_POLYGON_C_70, "A polygon can have between 3 and 1024 sides." );					break;
//		case CMD_ERR_DIVSEGS:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_YOU_CAN_DIV_71, "You can divide using 2 to 32767 segments:" );						break;
//		case CMD_ERR_SEGSKWORD:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_YOU_CAN_DIV_72, "You can divide using 2 to 32767 segments, or choose an option:" );	break;
//		case CMD_ERR_SURFTABS:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_THE_VALID_R_73, "The valid range is 2 to 32767 divisions:" );                   	break;
//		case CMD_ERR_INTRANGE:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_AN_INTEGER__74, "An integer (whole number) between -32768 and 32767 is needed:" ); 	break;
//		case CMD_ERR_DASHDOT:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_A_LINETYPE__75, "A linetype can have between 2 and 12 dash/dot specs:" );			break;
//		case CMD_ERR_SNAPTARGET:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_THE_SNAP_TA_76, "The snap target height can be any number between 1 and 50:" );		break;
//		case CMD_ERR_MESHSIZE:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_THE_MESH_SI_77, "The mesh size can be any integer between 2 and 256." );			break;
//									// This is the range for N and M mesh sizes.
//		case CMD_ERR_MESH3D:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_MORE_SECTIO_78, "More sections are required to define this 3D entity." );			break;
//		case CMD_ERR_SPHEREDIVS:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_AT_LEAST_TW_79, "At least two divisions are needed to create this entity." );		break;
//		case CMD_ERR_UNITTYPE:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_THE_AVAILAB_80, "The available unit types are 1 through 5." );						break;
//									// This is the range for the 5 kinds of LUNITS and AUNITS.
//		case CMD_ERR_DECIMALS:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_YOU_CAN_CHO_81, "You can choose between 0 and 8 for number of decimal places:" );	break;
//									// This is the range for the number of decimal places to display LUNITS & AUNITS.
//		case CMD_ERR_DENOMINATOR:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_PLEASE_ENTE_82, "Please enter 1, 2, 4, 8, 16, 32, 64, 128, or 256 for the greatest denominator:" );	break;
//									// These are the denominators available to display LUNITS & AUNITS.
//		case CMD_ERR_ARCSIZE:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_AN_ARC_CAN__83, "An arc can be any size between 0 and 360 degrees." );				break;
//									// This is a generic one when nothing else works, like when the range changes.
//		case CMD_ERR_BULGESIZE:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_AN_ANGLE_BE_84, "An angle between -360 and +360 degrees is needed." );				break;
//									//This is the range for bulge angles
//		case CMD_ERR_NUMRANGE:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_THE_VALUE_M_85, "The value must be within the specified range." );					break;
//									// This is a generic one when nothing else works, like when the range changes.
//		case CMD_ERR_GRIDTOOSMALL:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_THE_GRID_SP_86, "The grid spacing is too small to display in this view." );			break;
//		case CMD_ERR_GRIDTOOLARGE:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_THE_GRID_SP_87, "The grid spacing is too large to display in this view." );			break;
//		case CMD_ERR_SYSVARVAL:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_THE_SPECIFI_88, "The specified value is not within the system variable range" );	break;
//
//		// User entered the wrong kind of number...
//		case CMD_ERR_NUMERIC:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_A_NUMERIC_V_89, "A numeric value is needed. Please try again:" );					break;
//		case CMD_ERR_NONZERO:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_A_VALUE_OTH_90, "A value other than zero is needed. Please try again:" );			break;
//		case CMD_ERR_POSITIVE:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_A_POSITIVE__91, "A positive value is needed. Please try again:" );					break;
//		case CMD_ERR_POSNOZERO:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_A_POSITIVE__92, "A positive, non-zero value is needed. Please try again:" );		break;
//		case CMD_ERR_POSORZERO:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_A_POSITIVE__93, "A positive value, or zero, is needed. Please try again:" );		break;
//
//		// User entered the wrong kind of input...
//		case CMD_ERR_BADARGUMENT:   ErrPrompt=ResourceString(IDC_ERRORPROMPT_BAD_ARGUMEN_94, "Bad argument type." );                                             break;
//		case CMD_ERR_NOTHINGTODO:   ErrPrompt=ResourceString(IDC_ERRORPROMPT_NOTHING_TO__95, "Nothing to do. Single element array." );                           break;
//        case CMD_ERR_DUPENTITY:     ErrPrompt=ResourceString(IDC_ERRORPROMPT_THIS_ENTITY_96, "This entity is already selected:" );                               break;
//        case CMD_ERR_KWORD:			ErrPrompt=ResourceString(IDC_ERRORPROMPT_PLEASE_CHOO_97, "Please choose an option from the list:" );							break;
//									// Use especially when the prompt list is displayed again after this message.
//		case CMD_ERR_PT:			ErrPrompt=ResourceString(IDC_ERRORPROMPT_A_POINT_IS__98, "A point is needed. Please try again:" );							break;
//									// Use instead of "Invalid point".
//		case CMD_ERR_PTKWORD:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_PLEASE_PICK_99, "Please pick a point, or choose from the list:" );					break;
//		case CMD_ERR_2DPT:			ErrPrompt=ResourceString(IDC_ERRORPROMPT_A_2D__NOT__100, "A 2D (not 3D) point is needed. Please try again:" );				break;
//									// Use instead of "Invalid 2D point".
//		case CMD_ERR_2DPTKWORD:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_PLEASE_PIC_101, "Please pick a 2D point, or choose from the list:" );				break;
//		case CMD_ERR_NUMTWO2DPT:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_PLEASE_ENT_102, "Please enter a number, or pick two 2D corner points." );			break;
//		case CMD_ERR_ANG2NDPT:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_ENTER_A_NU_103, "Enter a numeric angle, or pick a 2nd point:" );					break;
//		case CMD_ERR_ANG2NDPTKWORD:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_ENTER_A_NU_104, "Enter a numeric angle, 2nd point, or choose from list:" );			break;
//		case CMD_ERR_ANG2PTS:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_PLEASE_ENT_105, "Please enter a numeric angle, or pick two points." );				break;
//		case CMD_ERR_DIST:			ErrPrompt=ResourceString(IDC_ERRORPROMPT_A_DISTANCE_106, "A distance is needed.  Please try again:" );						break;
//		case CMD_ERR_DIST2PTS:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_PLEASE_ENT_107, "Please enter a distance, or pick two points." );					break;
//		case CMD_ERR_DIST2PTSKWORD:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_ENTER_A_DI_108, "Enter a distance, pick two points, or choose an option." );		break;
//		case CMD_ERR_DIST2NDPT:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_A_DISTANCE_109, "A distance or 2nd point is needed. Please try again:" );			break;
//		case CMD_ERR_DIST2NDPTKWORD:ErrPrompt=ResourceString(IDC_ERRORPROMPT_ENTER_A_DI_110, "Enter a distance, pick a point, or choose an option:" );		    break;
//		case CMD_ERR_RADDCIRC:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_ENTER_A_RA_111, "Enter a radius, pick a point, or choose the Diameter option:" );	break;
//
//		// User entered a name that either isn't the right kind of name, or could not be found...
//			// Use the ones that say "FINDXXX" when you cannot locate the file.
//			// Use the ones that say "XXXNAME" when the name entered cannot be a valid filename.
//		case CMD_ERR_FINDHELP:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__112, "Unable to find the requested help file, Check installation." );						break;
//		case CMD_ERR_FINDSTYLE:     ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__113, "Unable to find the requested text style." );                       break;
//        case CMD_ERR_FINDTEXT:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__114, "Unable to find the requested text style. Please try again.\nYou can use the STYLE command to create a new text style." );	break;
//		case CMD_ERR_NOVPCONFIG:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__115, "Unable to find that viewport configuration." );					break;
//		case CMD_ERR_FINDFILE:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__116, "Unable to find the requested file." );								break;
//		case CMD_ERR_FINDXREF:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__117, "Unable to locate external reference." );							break;
//		case CMD_ERR_BADRENDER:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__118, "Unable to find rendering files (%s), Check installation." );		break;
//		case CMD_ERR_SAVEFILE:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__119, "Unable to Save to the requested file name. The file is open or read-only." );	break;
//		case CMD_ERR_EXPORTFILE:    ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__120, "Unable to Export a file to itself." );	                            break;
//		case CMD_ERR_CANTFINDFILE:  ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__121, "Unable to find the requested file: \"%s\"." );	    break;
//		case CMD_ERR_CANTOPENFILE:  ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__123, "Unable to open the requested file: \"%s\"." );	    break;
//
//		//FINDFILE2 used in STYLE, where user can specify 2 files simultaneously
//		case CMD_ERR_FINDFILE2:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__124, "Unable to find the second requested file." );						break;
//		case CMD_ERR_GOTFILE:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_REQUESTED__125, "Requested file found in search path." );							break;
//		case CMD_ERR_ANYNAME:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__126, "Unable to recognize entry as a valid name. Please try again." );	break;
//		case CMD_ERR_BLOCKNAME:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__127, "Unable to recognize entry as a block name. Please try again." );	break;
//		case CMD_ERR_LAYERNAME:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__128, "Unable to recognize entry as a layer name. Please try again." );	break;
//		case CMD_ERR_FINDBLOCK:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__129, "Unable to find a block with that name. Please try again." );		break;
//		case CMD_ERR_LTYPENAME:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__130, "Unable to recognize entry as a linetype name. Please try again" );	break;
//		case CMD_ERR_LTYPEPAT:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_INVALID_NU_131, "Invalid number or bad continuation." );							break;
//									// TODO - Above is for linetype patterns... We need a better way to say this.
//		case CMD_ERR_FINDLTYPE:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__132, "Unable to find a linetype with that name.  --\n-- You can use the LINETYPE command to load it." );				break;
//		case CMD_ERR_FINDLAYER:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__133, "Unable to find a layer with that name. Please try again." );		break;
//		case CMD_ERR_FINDVIEW:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__134, "Unable to find a view with that name. Please try again." );		break;
//		case CMD_ERR_FINDUCS:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__135, "Unable to find a UCS with that name. Please try again." );			break;
//		case CMD_ERR_NOUCSDEFS:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_NO_PREVIOU_136, "No previous UCS definitions found." );								break;
//		case CMD_ERR_DELETEUCS:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__137, "Unable to delete UCS." );											break;
//		case CMD_ERR_SAVEUCS:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__138, "Unable to save UCS." );											break;
//		case CMD_ERR_FINDSNAP:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__139, "Unable to find a snapshot with that name." );						break;
//		case CMD_ERR_NOPLANES:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_NO_PLANES__140, "No planes have been defined in this drawing" );					break;
//		case CMD_ERR_NOPATTERNS:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_NO_PATTERN_141, "No patterns have been defined in this drawing" );					break;
//		case CMD_ERR_FINDPATTERN:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__142, "Unable to find a hatch pattern with that name" );					break;
//		case CMD_ERR_GETDWGNAME:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__143, "Unable to get the name of the current drawing." );					break;
//		case CMD_ERR_OPENDWG:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_AN_ERROR_O_144, "An error occurred while attempting to open the requested drawing." );	break;
//		case CMD_ERR_OPENXREF:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_AN_ERROR_O_145, "An error occurred while attempting to open a nested external reference." );	break;
//		case CMD_ERR_LOADXREF:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_AN_ERROR_O_146, "An error occurred while attempting to load a nested external reference." );	break;
//		case CMD_ERR_UNRECOGSNAP:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__147, "Unable to recognize entity snap mode/s." );						break;
//		case CMD_ERR_UNRECOGVAR:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNRECOGNIZ_148, "Unrecognized variable. Type \"SETVAR,?\" for a list of variables." );	break;
//		case CMD_ERR_UNRECOGCMD:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__150, "Unable to recognize command.  Please try again." );				break;
//		case CMD_ERR_UNRECOGMENU:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__151, "Unable to recognize command in menu." );							break;
//		case CMD_ERR_UNRECOGENTRY:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__152, "Unable to recognize entry.  Please try again." );					break;
//		case CMD_ERR_LOADDIALOG:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_AN_ERROR_O_153, "An error occurred while attempting to load the dialog box." );		break;
//		case CMD_ERR_FINDDIMSTYLE:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__154, "Unable to find a dimension style with that name." );				break;  // May be needed in cmds12, but not yet.
//		case CMD_ERR_NODIMSTYLES:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_NO_DIMENSI_155, "No dimension styles have been defined in this drawing." );			break;
//		case CMD_ERR_DISPDIMVARS:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__156, "Unable to display the variables or dimension style \"*UNNAMED\"." );break;
//		case CMD_ERR_FINDCONVFACT:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__157, "Unable to find proper conversion factor in \"intellicad.unt\"." );break;
//		case CMD_ERR_NOXREFFOUND:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__158, "Unable to find an XRef by that name." );							break;
//		case CMD_ERR_NOATTRIBS:     ErrPrompt=ResourceString(IDC_ERRORPROMPT_NO_ATTRIBU_159, "No Attributes were selected." );                                   break;
//		case CMD_ERR_NOTBFOUND:	    ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__160, "Unable to find a toolbar by that name." );							break;
//		case CMD_ERR_DXFNEWONLY:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_DXFIN_IS_O_161, "DXFIN is only possible in a NEW empty drawing." );					    break;
//
//		// User tried to do something that can't be done...
//		case CMD_ERR_NOCHANGEABLE:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_ENTITIES_S_162, "Entities selected were not changeable." );					                break;
//		case CMD_ERR_BADGEOMETRY:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__163, "Unable to create an entity with that geometry." );					break;
//		case CMD_ERR_CANTRENAME:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_THAT_NAME__164, "That name is reserved by IntelliCAD, and cannot be renamed." );	break;
//		case CMD_ERR_RESERVEDBLOCK:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_THAT_BLOCK_165, "That block is reserved by IntelliCAD, and cannot be renamed." );	break;
//		case CMD_ERR_NULLTAG:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_THE_ATTRIB_166, "The attribute needs a name (alphanumeric, no spaces)." );			break;
//		case CMD_ERR_ELEVDIFFZ:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_CANNOT_CHA_167, "Cannot change elevation of entities with differing Z coordinates." );	break;
//		case CMD_ERR_OBLIQUELG:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_A_SMALLER__168, "A smaller obliquing angle is needed.  Enter new angle:" );			break;
//		case CMD_ERR_CANTCLOSE:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_CANNOT_CLO_169, "Cannot close until two or more segments are drawn." );				break;
//		case CMD_ERR_NOCIRCLE:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_NO_CIRCLE__170, "No circle is possible." );											break;
//		case CMD_ERR_CHAMNONPOLY:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__171, "Unable to chamfer a non-polyline entity with itself." );			break;
//		case CMD_ERR_CHAM3DPOLY:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__172, "Unable to chamfer 3D polyline entities." );						break;
//		case CMD_ERR_CHAMDIFFPOLY:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__173, "Unable to chamfer segments from different polylines." );			break;
//		case CMD_ERR_CHAMNONLINEAR:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__174, "Unable to chamfer with a non-linear entity." );					break;
//		case CMD_ERR_CHAMNOLENGTH:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__175, "Unable to chamfer lines of zero length." );						break;
//		case CMD_ERR_CHAMPARALLEL:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__176, "Unable to chamfer lines that are parallel." );						break;
//		case CMD_ERR_CHAMTOOCLOSE:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_CHAMFER_PO_177, "Chamfer point too close to start/end of segment." );				break;
//		case CMD_ERR_CHAM2LINEAR:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_TWO_LINEAR_178, "Two linear entities are needed for chamfering" );					break;
//		case CMD_ERR_CHAMDISTLG1:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_FIRST_DIST_179, "First distance is too large to perform requested chamfer." );		break;
//		case CMD_ERR_CHAMDISTLG2:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_SECOND_DIS_180, "Second distance is too large to perform requested chamfer." );		break;
//		case CMD_ERR_CHAMDISTLG3:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_DISTANCE_I_181, "Distance is too large to perform requested chamfer on line." );	break;
//		case CMD_ERR_CHAMFERPLADJ:  ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__182, "Unable to determine which vertex of polyline to chamfer." );		break;
//		case CMD_ERR_CHAMANGLE:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_CHAMFER_AN_183, "Chamfer angle is too large to perform requested chamfer." );		break;
//		case CMD_ERR_CHAMPOLYARC:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__184, "Unable to find intersection between line and straight segment of polyline." );	break;
//		case CMD_ERR_FILPOLYSEGS:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_DISTINCT_P_185, "Distinct polyline segments are needed." );							break;
//		case CMD_ERR_FILDIFFPOLY:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__186, "Unable to fillet segments from different polylines." );			break;
//		case CMD_ERR_FILNOTPOSS:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__187, "Unable to fillet the selected entities." );						break;
//		case CMD_ERR_FILCLOSEFAR:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_FILLET_POI_188, "Fillet point too close to segment start/end, or selected point too far from polyline." );	break;
//		case CMD_ERR_FILTOOCLOSE:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_FILLET_POI_189, "Fillet point too close to start/end of segment." );				break;
//		case CMD_ERR_FILNOLENGTH:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__190, "Unable to fillet lines of zero length." );							break;
//		case CMD_ERR_FILRADIUSLG:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_RADIUS_IS__191, "Radius is too large to perform requested fillet." );				break;
//		case CMD_ERR_NOINTERSECT:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_NO_INTERSE_192, "No intersection exists between the selected entities." );			break;
//		case CMD_ERR_BREAKENT:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_THE_SELECT_193, "The selected entity cannot be broken." );							break;
//		case CMD_ERR_BREAKBLOCK:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_BLOCKS_CAN_194, "Blocks cannot be broken." );										break;
//		case CMD_ERR_BREAKDISTINCT:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_DISTINCT_P_195, "Distinct points are needed to break this entity." );				break;
//		case CMD_ERR_MODENT:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__196, "Unable to modify the selected entity." );							break;
//		case CMD_ERR_EXTENDENT:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__197, "Unable to extend the selected entity." );							break;
//		case CMD_ERR_LENGTHENENT:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__198, "Unable to lengthen the selected entity." );						break;
//		case CMD_ERR_LENCLOSEPOLY:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_A_CLOSED_P_199, "A closed polyline cannot be lengthened." );						break;
//		case CMD_ERR_LENBYANG:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_THIS_ENTIT_200, "This entity cannot be lengthened using angle." );					break;
//		case CMD_ERR_LENBYDIST:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_THIS_ENTIT_201, "This entity cannot be lengthened by the specified distance." );	break;
//		case CMD_ERR_LENBYDYNPT:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_THIS_ENTIT_202, "This entity cannot be lengthened using a dynamic point." );		break;
//		case CMD_ERR_CLOSEARC:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_AN_ARC_CAN_203, "An arc cannot be closed." );										break;
//		case CMD_ERR_NOTATABLET:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_THE_CURREN_204, "The current pointing device is not a tablet." );					break;
//		case CMD_ERR_HATCHEXPPREV:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_HATCH_CANN_205, "Hatch cannot be exploded in preview mode." );						break;
//		case CMD_ERR_MIXCLOSEOPEN:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_CANNOT_MIX_206, "Cannot mix closed and open paths." );								break;
//		case CMD_ERR_2PTSURFACE:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_TWO_POINTS_207, "Two points cannot define a surface." );							break;
//		case CMD_ERR_DIFFPTS:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_THE_POINTS_208, "The points for this operation need to be distinct." );				break;
//		case CMD_ERR_ONSAMELINE:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_THE_THREE__209, "The three points cannot all be along one line." );					break;
//		case CMD_ERR_OFFSMALLRAD:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_THE_RADIUS_210, "The radius is too small to offset to the inside using that distance." ); break;  // for circles
//		case CMD_ERR_OFFSHARPCURVE:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_THE_ENTITY_211, "The entity curves too sharply to offset to this side using that distance." ); break;  // for ellipses & splines
//		case CMD_ERR_OFFZEROLEN:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_AN_ENTITY__212, "An entity of zero length cannot be offset." );						break;
//		case CMD_ERR_PEDITPFACE:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_A_POLYFACE_213, "A polyface mesh cannot be edited useing PEDIT." );					break;
//		case CMD_ERR_CLOSEDPLINE:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_THE_SELECT_214, "The selected polyline is already closed." );						break;
//		case CMD_ERR_CLOSEDMPLINE:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_THE_SELECT_215, "The selected polyline is already closed in the M direction." );	break;
//		case CMD_ERR_CLOSEDNPLINE:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_THE_SELECT_216, "The selected polyline is already closed in the N direction." );	break;
//		case CMD_ERR_NOTCLOSEDPLINE:  ErrPrompt=ResourceString(IDC_ERRORPROMPT_THE_SELECT_217, "The selected polyline is not closed." );							break;
////		case CMD_ERR_NOTCLOSEDMPLINE: ErrPrompt="The selected polyline is not closed in the M direction.";		break;  Not used anywhere yet.  It's here just in case.
//		case CMD_ERR_NOTCLOSEDNPLINE: ErrPrompt=ResourceString(IDC_ERRORPROMPT_THE_SELECT_218, "The selected polyline is not closed in the N direction." );		break;
//		case CMD_ERR_JOINCLOSEDPLINE: ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__219, "Unable to join to a closed polyline." );							break;
//		case CMD_ERR_BREAKDELETE:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_BREAKING_T_220, "Breaking this entity would have deleted it.  Cancelled." );		break;
//		case CMD_ERR_COINCVERPTS:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__221, "Unable to use coincident vertex points for this operation." );		break;
////		case CMD_ERR_COINCENDPTS:	ErrPrompt="Cannot use coincident endpoints for this operation.";			break;  Not used anywhere yet.  It's here just in case.
//		case CMD_ERR_NONCOINCENDPTS:ErrPrompt=ResourceString(IDC_ERRORPROMPT_LINES_WITH_222, "Lines with coincident endpoints are needed." );					break;
//		case CMD_ERR_PARALLEL:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__223, "Unable to use parallel lines here.  Please pick again." );			break;
//		case CMD_ERR_FITCURVE:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__224, "Unable to fit a curve to the selected polyline." );				break;
//		case CMD_ERR_NOQUICKSNAP:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_QUICK_IS_A_225, "Quick is available only when other snap modes are active." );		break;
//		case CMD_ERR_NOVIEWPORTS:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_THERE_ARE__226, "There are no VIEWPORTS, use the MVIEW command to create some." );	break;
//		case CMD_ERR_NOACTIVEVP:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_ALL_VIEWPO_227, "All VIEWPORTS are turned OFF, use the MVIEW command to turn ON." );	break;
//		case CMD_ERR_TILEMODE0:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_TILEMODE_M_228, "TILEMODE must be set to Off (0) before using this command." );		break;
//		case CMD_ERR_TILEMODE1:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_TILEMODE_M_229, "TILEMODE must be set to On (1) before using this command." );		break;
//		case CMD_ERR_BDRYSET:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_THE_SELECT_230, "The selected entities do not define a boundary." );				break;
//		case CMD_ERR_OSNAPENT:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__231, "Unable to locate entity snap point." );							break;
//		case CMD_ERR_XREFEXIST:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_A_NON_REFE_232, "A non-referenced block with that XRef name exists in the drawing." );	break;
//		case CMD_ERR_INSERTXREF:    ErrPrompt=ResourceString(IDC_ERRORPROMPT_INSERTING__233, "Inserting an Externally Referenced drawing (Xref) as an Xref." );	break;
//		case CMD_ERR_BLKREF:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_A_BLOCK_CA_234, "A block cannot be self-referencing." );							break;
//		case CMD_ERR_XREFREF:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_THE_CURREN_235, "The current drawing cannot have an External Reference to itself." );break;
//		case CMD_ERR_DUPFILE:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_THE_CURREN_236, "The current drawing cannot be inserted into itself." );			break;
//		case CMD_ERR_ZOOMIN:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_YOU_HAVE_R_237, "You have reached the limits of zooming in.  Unable to zoom in any closer." );		break;
//		case CMD_ERR_ZOOMOUT:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_YOU_HAVE_R_238, "You have reached the limits of zooming out.  Unable to zoom out any further." );	break;
//		case CMD_ERR_PLINESEGS:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_YOU_HAVE_R_239, "You have reached the limit of the number of segments per polyline." );			break;
//		case CMD_ERR_XRCURLAY:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_CANNOT_SET_240, "Cannot set an Externally Referenced layer as current layer." );				break;
//		case CMD_ERR_PERSPECTIVE:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_THAT_COMMA_241, "That command is not available in Perspective mode.\nUse DVIEW or VPOINT to turn perspective mode off." );	break;
//		case CMD_ERR_MAXACTVP:	    ErrPrompt=ResourceString(IDC_ERRORPROMPT_MAXIMUM_NU_242, "Maximum number of active view ports reached (check setvar MAXACTVP)." );		break;
//
//		// There is a problem somewhere with the drawing or program...
//		case CMD_ERR_BADDATA:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_BAD_DATA_F_243, "Bad data format on entity." );										break;
//		case CMD_ERR_CANTGETDATA:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__244, "Unable to get Entity Data for that entity." );						break;
//		case CMD_ERR_TOOMUCHDATA:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_TOO_MUCH_E_245, "Too much Entity Data--Ignored." );									break;
//		case CMD_ERR_REMOVEEED:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__246, "Unable to remove old Entity Data." );								break;
//		case CMD_ERR_UPDATEENT:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__247, "Unable to update entity." );										break;
//		case CMD_ERR_CREATEENT:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__248, "Unable to create the requested entity." );							break;
//		case CMD_ERR_CREATEATTDEF:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__249, "Unable to create the requested AttDef entity." );					break;
//		case CMD_ERR_CREATETEXT:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__250, "Unable to create the requested text entity." );					break;
//		case CMD_ERR_CREATEDONUT:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__251, "Unable to create the requested donut." );							break;
//		case CMD_ERR_CREATELINE:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__252, "Unable to create the requested line." );							break;
//		case CMD_ERR_DEFBLOCK:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__253, "Unable to define the requested block(s)." );						break;
//		case CMD_ERR_BUILDENT:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__254, "Unable to build the requested entity or entities." );				break;
//		case CMD_ERR_UNTERMINATED:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNTERMINAT_255, "Unterminated complex entity." );									break;
//			// Suggestions for a way to say this (above)?
//		case CMD_ERR_INSERTDEF:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_ANY_DRAWIN_256, "Any drawing definitions in the requested block have been imported." ); break;
//		case CMD_ERR_BADENTNAME:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_INVALID_EN_257, "Invalid entity name encountered." );								break;
//			// Can I put the line and donut ones into CREATEENT? (Lots in cmds4)
//		case CMD_ERR_NOSAVEDVIEWS:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_THIS_DRAWI_258, "This drawing contains no saved views." );							break;
//		case CMD_ERR_NOTINPSPACE:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_THIS_COMMA_259, "This command is not available in Paper space." );					break;
//		case CMD_ERR_NOPAPERVIEW:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_CANNOT_RES_260, "Cannot restore a paper space view into model space." );			break;
//		case CMD_ERR_NOMODELVIEW:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_CANNOT_RES_261, "Cannot restore a model space view into paper space." );			break;
//		case CMD_ERR_NODYNZOOM:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_DYNAMIC_ZO_262, "Dynamic zoom is not yet supported." );								break;
//		case CMD_ERR_NODESTROY:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_HANDLES_CA_263, "Handles cannot be disabled once they have been created." );		break;
//		case CMD_ERR_NAMETOOLONG:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_A_NAME_CAN_264, "A name can have up to 31 characters:" );							break;
//		case CMD_ERR_BLKNAMETOOLONG:ErrPrompt=ResourceString(IDC_ERRORPROMPT_A_BLOCK_NA_265, "A block name can have up to 31 characters:" );						break;
//		case CMD_ERR_GPS:			ErrPrompt=ResourceString(IDC_ERRORPROMPT_AN_ERROR_H_266, "An error has occurred in GPS" );									break;
//		// I would love to kill these next two.  They're scary, and do not give the user any information they can use.  (cmds1)
//		case CMD_ERR_OPENGLCHOOSE:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_OPENGL_ERR_267, "OpenGL error: ChoosePixelFormat failed." );						break;
//		case CMD_ERR_OPENGLSET:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_OPENGL_ERR_268, "OpenGL error: SetPixelFormat failed." );							break;
//		case CMD_ERR_MOREMEM:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_MORE_MEMOR_269, "More memory is needed to do this operation" );						break;
//		// If they see this one, they're toast.
//		case CMD_ERR_UNABLE:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_INTELLICAD_270, "IntelliCAD is unable to complete this operation." );				break;
		case CMD_ERR_UNABLE:
		    strTitle.LoadString(AFX_IDS_APP_TITLE);
			strTitle.LoadString(IDS_HATCHEDITERROR);
			ErrPrompt=ResourceString(IDC_ERRORPROMPT_INTELLICAD_270, "IntelliCAD is unable to complete this operation." );
			break;
									// Replace this where possible.  I've only used it when the old message just said "ERROR" (+/-)
//		case CMD_ERR_NOEXTDWGS:		ErrPrompt=ResourceString(IDC_ERRORPROMPT_NO_DRAWING_271, "No drawings are accessible for querying, use \"Access External Drawings\"." );				break;
//
//		// Errors loading/unloading SDS apps
//		case CMD_ERR_INVALIDDLL:    ErrPrompt=ResourceString(IDC_ERRORPROMPT___INVALID__273, "Error loading \"%s\": Invalid Dynamic-link library (DLL)." ); break;
//		case CMD_ERR_SYSNOMEM:      ErrPrompt=ResourceString(IDC_ERRORPROMPT____NSYSTEM_274, "Error loading \"%s\": \nSystem is out of memory, executable file is corrupt,\nor relocations are invalid." ); break;
//		case CMD_ERR_ACCESS_DENIED: ErrPrompt=ResourceString(IDC_ERRORPROMPT___SHARING__275, "Error loading \"%s\": Sharing or network-protection error." ); break;
//		case CMD_ERR_OUTOFMEMORY:   ErrPrompt=ResourceString(IDC_ERRORPROMPT_INSUFFICIE_276, "Insufficient memory to start application \"%s\"." ); break;
//		case CMD_ERR_NOT_READY:     ErrPrompt=ResourceString(IDC_ERRORPROMPT__REQUIRES__278, "Application \"%s\" requires Microsoft Windows 32-bit extensions." ); break;
//		case CMD_ERR_PROC_NOTFOUND: ErrPrompt=ResourceString(IDC_ERRORPROMPT___UNRESOLV_279, "Error loading \"%s\": Unresolved function at run time." ); break;
//		case CMD_ERR_DLLNOTFOUND:   ErrPrompt=ResourceString(IDC_ERRORPROMPT_ONE_OF_THE_280, "One of the library files needed to run \"%s\" cannot be found." ); break;
//		case CMD_ERR_DLLLOADPROB:   ErrPrompt=ResourceString(IDC_ERRORPROMPT_SYSTEM_ERR_282, "System error loading \"%s\" (%ld)." ); break;
//		case CMD_ERR_CANTFINDAPP:   ErrPrompt=ResourceString(IDC_ERRORPROMPT_ERROR_LOAD_284, "Error loading application: Could not find %s." ); break;
//		case CMD_ERR_BADENTPOINT:   ErrPrompt=ResourceString(IDC_ERRORPROMPT_CAN_T_GET__285, "Can't get Entry Point function in DLL; Load failed %s" ); break;
//		case CMD_ERR_UNLOADING:     ErrPrompt=ResourceString(IDC_ERRORPROMPT_ERROR_UNLO_286, "Error unloading \"%s\" (%ld)." ); break;
//		case CMD_ERR_FINDUNLOAD:    ErrPrompt=ResourceString(IDC_ERRORPROMPT_ERROR_LOAD_284, "Error loading application: Could not find %s." ); break;
//		case CMD_ERR_ENTSINSET:     ErrPrompt=ResourceString(IDC_ERRORPROMPT_ENTITIES_I_287, "Entities in set: %ld" ); break;
//
//// I'm throwing all mine right here and will organize them once I have a bunch made up.
//		case CMD_ERR_CANTLOADREN:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__288, "Unable to load rendering code (%s)" );	break;
//		case CMD_ERR_FLATTENED:		ErrPrompt=ResourceString(IDC_ERRORPROMPT__LI_ENTITI_289, "%li entities flattened. " );	break;
//		case CMD_ERR_NOTPARALLEL:	ErrPrompt=ResourceString(IDC_ERRORPROMPT__LI_ENTITI_290, "%li entities not parallel to UCS." );	break;
//		case CMD_ERR_CANTFINDMENU:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__291, "Unable to find loaded menu \"%s\"." );	break;
//		case CMD_ERR_CANTFINDDWG:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__292, "Unable to find drawing \"%s\"." ); break;
//		case CMD_ERR_PLEASESPEC:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_PLEASE_SPE_293, "Please specify... %s " ); break;
//		case CMD_ERR_BADVPORTNUM:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_CAN_T_SET__294, "Can't set CVPORT to %i" ); break;
//		case CMD_ERR_GETOPENFILE:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_INTERNAL_E_295, "Internal error, GetOpenFileName returned Error # %x" ); break;
//		case CMD_ERR_CANTOPENDWG:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_ERROR_OPEN_296, "Error opening drawing \"%s\"." ); break;
//
//		case CMD_ERR_REBUILDTABLE:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_ERROR_REBU_297, "Error rebuilding %s table." );	break;
//		case CMD_ERR_BUILDTABLE:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_ERROR_BUIL_298, "Error building %s table." );	break;
//		case CMD_ERR_CREATEBLOCK:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_ERROR___FA_299, "Error:  Failed to create block entity (%ld)." );	break;
//		case CMD_ERR_NOTABLEITEM:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_UNABLE_TO__300, "Unable to find %s %s." );	break;
//		case CMD_ERR_ALREADYEXISTS:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_A__S_WITH__301, "A %s with that name already exists." );	break;
//		case CMD_ERR_WASRENAMEDAS:	ErrPrompt=ResourceString(IDC_ERRORPROMPT_THE__S_WAS_302, "The %s was renamed as %s." );	break;
//		
//		// No reason to define a separate CMD error - The ranges aren't even close to crossing.
//		case IDC_ERROR_BPOLY_PATH:	ErrPrompt=ResourceString(IDC_ERROR_BPOLY_PATH, "The area you selected is invalid.");	break;
//		case IDC_ERROR_BPOLY_ELLIPSESPLINE: ErrPrompt=ResourceString(IDC_ERROR_BPOLY_ELLIPSESPLINE, "You cannot use ellipses or splines to create a polyline boundary.");	break;
//		case IDC_R12HATCH_TOOMANYLINES:	ErrPrompt= ResourceString(IDC_R12HATCH_TOOMANYLINES, "The line segments of the selected hatch pattern are too dense for the specified area.  Try using a larger hatch scale to make the segments less dense or select a hatch pattern with fewer segments.");	break;
//		case IDC_ERROR_SELECT_HATCH:	ErrPrompt= ResourceString(IDC_ERROR_SELECT_HATCH, "The selected entity is not a hatch entity.");	break;
////		case CMD_ERR_:	ErrPrompt=;	break;
//				
//
//
//		// If all else fails...
		default:					ErrPrompt=ResourceString(IDC_ERRORPROMPT_INTELLICAD_270, "IntelliCAD is unable to complete this operation." );

	}

	_snwprintf( fs1, sizeof(fs1), ErrPrompt, arg1, arg2, arg3);

	//// Clear the command buffers to stop script files.
	//if ( ErrNo!=CMD_ERR_DWGHASIMAGE && ErrNo!=CMD_ERR_DWGHASACIS && ErrNo!=CMD_ERR_DWGHASHATCH &&
	//	 ErrNo!=CMD_ERR_DWGHASPROXY && ErrNo!=CMD_ERR_UNSUPPORTEDA2KENT && ErrNo!=CMD_ERR_GRIDTOOSMALL &&
	//	 ErrNo!=CMD_ERR_GRIDTOOLARGE && ErrNo!=CMD_ERR_ZOOMIN && ErrNo!=CMD_ERR_ZOOMOUT)
	//	SDS_FreeEventList(0);

 //   SDS_getvar( NULL, DB_QWNDLCMD, &rb1,SDS_CURDWG, &SDS_CURCFG, &SDS_CURSES);
	//// Windows plays its own sound when the dialog pops up (?), so I moved the sound to command line error messages only.
	//if ( ( DispMode || ( rb1.resval.rint==0)) && DispMode!=-1) {
	//	if ( cmd_InsideOpenCommand || cmd_InsideNewCommand || SDS_WasFromPipeLine)
	//		goto nomsgbox;
	//	sds_alert(fs1);
	//} else {
	//	if(DispMode!=-1) cmd_PlaySound(1);
	//	nomsgbox: ;
	//	if ( ErrNo==CMD_ERR_DWGHASIMAGE || ErrNo==CMD_ERR_DWGHASACIS || ErrNo==CMD_ERR_DWGHASHATCH ||
	//	     ErrNo==CMD_ERR_DWGHASPROXY || ErrNo==CMD_ERR_UNSUPPORTEDA2KENT || ErrNo==CMD_ERR_GRIDTOOSMALL ||
	//		 ErrNo==CMD_ERR_GRIDTOOLARGE)
	//		sds_printf("\n%s\n"/*DNT*/,fs1);
	//	else
	//		sds_printf("\n%s"/*DNT*/,fs1);
	//}
    //SDS_SetLastPrompt(ErrPrompt);
}

} // namespace MC

#include "MCAD.h"
#include "MhPts.h"
#include "MbFile.h"
#include "MmSystemProperty.h"
#include "MmWnd.h"
#include "MmLib.h"
#include "MDxf.h"

#define		MSZ_DXFLINE				128

///////////////////////////////////////////////////////////////////////////
//�@�c�w�e�t�@�C���̎��̍��ړǍ���

MINT MDxfGetIntItem(
						MBHZDT	&dxfFile,				// [I O} DXF�t�@�C���n���h��
						MINT	*piData					// [  O] �����f�[�^					
					)
{
	MINT	ist;

	ist = dxfFile.NextLine();
	ist = dxfFile.GetInt( piData, 1);
	if (ist != MBEOF)
		ist = 0;
	return ist;
}

///////////////////////////////////////////////////////////////////////////
//�@�c�w�e�t�@�C���̎��̍��ړǍ���

MINT MDxfGetFltItem(
						MBHZDT	&dxfFile,				// [I O} DXF�t�@�C���n���h��
						MFLOAT	*pfData					// [  O] �����f�[�^					
					)
{
	MINT	ist;

	ist = dxfFile.NextLine();
	ist = dxfFile.GetFlt( pfData, 1);
	if (ist != MBEOF)
		ist = 0;
	return ist;
}

///////////////////////////////////////////////////////////////////////////
//�@�c�w�e�t�@�C���̎��̍��ړǍ���

MINT MDxfGetStrItem(
						MBHZDT	&dxfFile,				// [I O} DXF�t�@�C���n���h��
						MCHAR	*sCode					// [  O] �R�[�h 
					)
{
	MINT	ist;

	ist = dxfFile.Head( sCode, MSZ_DXFLINE);
	if (ist != MBEOF)
		ist = 0;
	return ist;
}

///////////////////////////////////////////////////////////////////////////
//�@�c�w�e�t�@�C���̎��̍��ړǍ���

MINT MDxfGetStrData(
						MINT	iFirst,					// [I  ] 1: �t�@�C���̐擪�@0: ����
						MBHZDT	&dxfFile,				// [I O} DXF�t�@�C���n���h��
						MINT	*piType,				// [  O] �^�C�v					
						MCHAR	*sCode					// [  O] �R�[�h 
					)
{
	MINT	ist;

	*piType = 0;
	sCode[0] = NULL;

	if (iFirst == 0)
		ist = dxfFile.NextLine();
	ist = dxfFile.GetInt( piType, 1);
	if (ist != MBEOF)
		ist = dxfFile.Head( sCode, MSZ_DXFLINE);
	if (ist != MBEOF)
		ist = 0;
		
	return ist;
}

///////////////////////////////////////////////////////////////////////////
//�@�c�w�e�t�@�C���̃Z�N�V������ǂݔ�΂�

MINT MDxf_sektion_skip( 
						MBHZDT	&dxfFile				// [I O} DXF�t�@�C���n���h��
				)
{
	MINT		ist;

	MINT		iType;
	MCHAR		sCode[MSZ_DXFLINE];

	while ((ist = MDxfGetStrData( 0, dxfFile, &iType, sCode)) == 0 &&
		   Mstrcmp( sCode, Mstr( "ENDSEC")) != 0);

	return ist;
}

///////////////////////////////////////////////////////////////////////////
//�@�c�w�e�t�@�C���̓_�v�f��Ǎ��� ( "POINT")
//
MINT MDxf_point_get(
						MBHZDT	&dxfFile,				// [I O} DXF�t�@�C���n���h��
						MCHAR   *sCode,					// [  O] Next entity
						MdPrm	*uPrm
				)
{
	MINT		ist;
	MINT		ist1;
	bool		fEnd = false;

	MINT		iType;
	MCHAR		sLineName[MSZ_DXFLINE];
	MCHAR		sLayerName[MSZ_DXFLINE];
	MgPoint3	Point;
	MINT		iColor;
	
	MCHAR		sDummy[MSZ_DXFLINE];

	sLineName[0] = NULL;
	sLayerName[0] = NULL;
	iColor = 1;
	
	while (!fEnd && (ist = MDxfGetIntItem( dxfFile, &iType)) == 0) {
		switch (iType) {
			case 6:
				ist1 = MDxfGetStrItem( dxfFile, sLineName); 
				break;
			case 8:
				ist1 = MDxfGetStrItem( dxfFile, sLayerName); 
				break;
			case 10:
				ist1 = MDxfGetFltItem( dxfFile, &Point.x); 
				break;
			case 20:
				ist1 = MDxfGetFltItem( dxfFile, &Point.y); 
				break;
			case 30:
				ist1 = MDxfGetFltItem( dxfFile, &Point.z); 
				break;
			case 62:
				ist1 = MDxfGetIntItem( dxfFile, &iColor); 
				break;
			case 0:
				fEnd = true;
				ist1 = MDxfGetStrItem( dxfFile, sCode); 
				break;
			default:
				ist1 = MDxfGetStrItem( dxfFile, sDummy); 
				break;
		}
	}
	return ist1;
}

///////////////////////////////////////////////////////////////////////////
//�@�c�w�e�t�@�C���̐��v�f��Ǎ��� ( "LINE")
//			ist1 == MDxf_line_get();
MINT MDxf_line_get(
						MBHZDT	&dxfFile,				// [I O} DXF�t�@�C���n���h��
						MCHAR   *sCode,					// [  O] Next entity
						MdPrm	*uPrm
				)
{
	MINT		ist;
	MINT		ist1;
	bool		fEnd = false;

	MINT		iType;
	MCHAR		sLineName[MSZ_DXFLINE];
	MCHAR		sLayerName[MSZ_DXFLINE];
	MgLine3		Line;
	MINT		iColor;
	
	MCHAR		sDummy[MSZ_DXFLINE];

	sLineName[0] = NULL;
	sLayerName[0] = NULL;
	iColor = 1;
	
	while (!fEnd && (ist = MDxfGetIntItem( dxfFile, &iType)) == 0) {
		switch (iType) {
			case 6:
				ist1 = MDxfGetStrItem( dxfFile, sLineName); 
				break;
			case 8:
				ist1 = MDxfGetStrItem( dxfFile, sLayerName); 
				break;
			case 10:
				ist1 = MDxfGetFltItem( dxfFile, &Line.p[0].x); 
				break;
			case 20:
				ist1 = MDxfGetFltItem( dxfFile, &Line.p[0].y); 
				break;
			case 30:
				ist1 = MDxfGetFltItem( dxfFile, &Line.p[0].z); 
				break;
			case 11:
				ist1 = MDxfGetFltItem( dxfFile, &Line.p[1].x); 
				break;
			case 21:
				ist1 = MDxfGetFltItem( dxfFile, &Line.p[1].y); 
				break;
			case 31:
				ist1 = MDxfGetFltItem( dxfFile, &Line.p[1].z); 
				break;
			case 62:
				ist1 = MDxfGetIntItem( dxfFile, &iColor); 
				break;
			case 0:
				fEnd = true;
				ist1 = MDxfGetStrItem( dxfFile, sCode); 
				break;
			default:
				ist1 = MDxfGetStrItem( dxfFile, sDummy); 
				break;
		}
	}
	uPrm->Line = Line;
	return ist1;
}

///////////////////////////////////////////////////////////////////////////
//�@�c�w�e�t�@�C���̃|�����C���v�f��Ǎ��� ( "POLYLINE")
MINT MDxf_polyline_get(
						MBHZDT	&dxfFile,				// [I O} DXF�t�@�C���n���h��
						MCHAR   *sCode,					// [  O] Next entity
						MdPrm	*uPrm
				)
{
	MINT		ist;
	MINT		ist1;
	bool		fEnd = false;

	MINT		iType;
	MCHAR		sLineName[MSZ_DXFLINE];
	MCHAR		sLayerName[MSZ_DXFLINE];
	MgLine3		Line;
	MINT		iColor;
	
	MCHAR		sDummy[MSZ_DXFLINE];

	sLineName[0] = NULL;
	sLayerName[0] = NULL;
	iColor = 1;
	
	while (!fEnd && (ist = MDxfGetIntItem( dxfFile, &iType)) == 0) {
		switch (iType) {
			case 6:
				ist1 = MDxfGetStrItem( dxfFile, sLineName); 
				break;
			case 8:
				ist1 = MDxfGetStrItem( dxfFile, sLayerName); 
				break;
			case 10:
				ist1 = MDxfGetFltItem( dxfFile, &Line.p[0].x); 
				break;
			case 20:
				ist1 = MDxfGetFltItem( dxfFile, &Line.p[0].y); 
				break;
			case 30:
				ist1 = MDxfGetFltItem( dxfFile, &Line.p[0].z); 
				break;
			case 11:
				ist1 = MDxfGetFltItem( dxfFile, &Line.p[1].x); 
				break;
			case 21:
				ist1 = MDxfGetFltItem( dxfFile, &Line.p[1].y); 
				break;
			case 31:
				ist1 = MDxfGetFltItem( dxfFile, &Line.p[1].z); 
				break;
			case 62:
				ist1 = MDxfGetIntItem( dxfFile, &iColor); 
				break;
			case 0:
				fEnd = true;
				ist1 = MDxfGetStrItem( dxfFile, sCode); 
				break;
			default:
				ist1 = MDxfGetStrItem( dxfFile, sDummy); 
				break;
		}
	}
	return ist1;
}

///////////////////////////////////////////////////////////////////////////
//�@�c�w�e�t�@�C���̉~�v�f��Ǎ��� ( "CIRCLE")
//			ist1 == MDxf_circle_get();
//
MINT MDxf_circle_get(
						MBHZDT	&dxfFile,				// [I O} DXF�t�@�C���n���h��
						MCHAR   *sCode,					// [  O] Next entity
						MdPrm	*uPrm
				)
{
	MINT		ist;
	MINT		ist1;
	bool		fEnd = false;

	MINT		iType;
	MCHAR		sLineName[MSZ_DXFLINE];
	MCHAR		sLayerName[MSZ_DXFLINE];
	MgPoint3	ptCenter;
	MFLOAT		Radius;
	MINT		iColor;
	
	MCHAR		sDummy[MSZ_DXFLINE];

	sLineName[0] = NULL;
	sLayerName[0] = NULL;
	iColor = 1;
	
	while (!fEnd && (ist = MDxfGetIntItem( dxfFile, &iType)) == 0) {
		switch (iType) {
			case 6:
				ist1 = MDxfGetStrItem( dxfFile, sLineName); 
				break;
			case 8:
				ist1 = MDxfGetStrItem( dxfFile, sLayerName); 
				break;
			case 10:
				ist1 = MDxfGetFltItem( dxfFile, &ptCenter.x); 
				break;
			case 20:
				ist1 = MDxfGetFltItem( dxfFile, &ptCenter.y); 
				break;
			case 30:
				ist1 = MDxfGetFltItem( dxfFile, &ptCenter.z); 
				break;
			case 40:
				ist1 = MDxfGetFltItem( dxfFile, &Radius); 
				break;
			case 62:
				ist1 = MDxfGetIntItem( dxfFile, &iColor); 
				break;
			case 0:
				fEnd = true;
				ist1 = MDxfGetStrItem( dxfFile, sCode); 
				break;
			default:
				ist1 = MDxfGetStrItem( dxfFile, sDummy); 
				break;
		}
	}
	return ist1;
}

///////////////////////////////////////////////////////////////////////////
//�@�c�w�e�t�@�C���̉~�ʗv�f��Ǎ��� ( "ARC")
//			ist1 == MDxf_arc_get();
MINT MDxf_arc_get(
						MBHZDT	&dxfFile,				// [I O} DXF�t�@�C���n���h��
						MCHAR   *sCode,					// [  O] Next entity
						MdPrm	*uPrm
				)
{
	MINT		ist;
	MINT		ist1;
	bool		fEnd = false;

	MINT		iType;
	MCHAR		sLineName[MSZ_DXFLINE];
	MCHAR		sLayerName[MSZ_DXFLINE];
	MgPoint3	ptCenter;
	MFLOAT		Radius;
	MFLOAT		rStart;
	MFLOAT		rEnd;
	MINT		iColor;
	
	MCHAR		sDummy[MSZ_DXFLINE];

	sLineName[0] = NULL;
	sLayerName[0] = NULL;
	iColor = 1;
	
	while (!fEnd && (ist = MDxfGetIntItem( dxfFile, &iType)) == 0) {
		switch (iType) {
			case 6:
				ist1 = MDxfGetStrItem( dxfFile, sLineName); 
				break;
			case 8:
				ist1 = MDxfGetStrItem( dxfFile, sLayerName); 
				break;
			case 10:
				ist1 = MDxfGetFltItem( dxfFile, &ptCenter.x); 
				break;
			case 20:
				ist1 = MDxfGetFltItem( dxfFile, &ptCenter.y); 
				break;
			case 30:
				ist1 = MDxfGetFltItem( dxfFile, &ptCenter.z); 
				break;
			case 40:
				ist1 = MDxfGetFltItem( dxfFile, &Radius); 
				break;
			case 50:
				ist1 = MDxfGetFltItem( dxfFile, &rStart); 
				break;
			case 51:
				ist1 = MDxfGetFltItem( dxfFile, &rEnd); 
				break;
			case 62:
				ist1 = MDxfGetIntItem( dxfFile, &iColor); 
				break;
			case 0:
				fEnd = true;
				ist1 = MDxfGetStrItem( dxfFile, sCode); 
				break;
			default:
				ist1 = MDxfGetStrItem( dxfFile, sDummy); 
				break;
		}
	}
	return ist1;
}

///////////////////////////////////////////////////////////////////////////
//�@�c�w�e�t�@�C���̕�����v�f��Ǎ��� ( "TEXT")
//			ist1 == MDxf_text_get();
MINT MDxf_text_get(
						MBHZDT	&dxfFile,				// [I O} DXF�t�@�C���n���h��
						MCHAR   *sCode,					// [  O] Next entity
						MdPrm	*uPrm
				)
{
	MINT		ist;
	MINT		ist1;
	bool		fEnd = false;

	MINT		iType;
	MCHAR		sLineName[MSZ_DXFLINE];
	MCHAR		sFontName[MSZ_DXFLINE];
	MCHAR		sLayerName[MSZ_DXFLINE];
	MgPoint3	Point;
	MFLOAT		rHeight;
	MFLOAT		rAngle;
	MINT		iColor;
	
	MCHAR		sDummy[MSZ_DXFLINE];

	sLineName[0] = NULL;
	sLayerName[0] = NULL;
	iColor = 1;
	
	while (!fEnd && (ist = MDxfGetIntItem( dxfFile, &iType)) == 0) {
		switch (iType) {
			case 6:
				ist1 = MDxfGetStrItem( dxfFile, sLineName); 
				break;
			case 7:
				ist1 = MDxfGetStrItem( dxfFile, sFontName); 
				break;
			case 8:
				ist1 = MDxfGetStrItem( dxfFile, sLayerName); 
				break;
			case 10:
				ist1 = MDxfGetFltItem( dxfFile, &Point.x); 
				break;
			case 20:
				ist1 = MDxfGetFltItem( dxfFile, &Point.y); 
				break;
			case 30:
				ist1 = MDxfGetFltItem( dxfFile, &Point.z); 
				break;
			case 40:
				ist1 = MDxfGetFltItem( dxfFile, &rHeight); 
				break;
			case 50:
				ist1 = MDxfGetFltItem( dxfFile, &rAngle); 
				break;
			case 62:
				ist1 = MDxfGetIntItem( dxfFile, &iColor); 
				break;
			case 0:
				fEnd = true;
				ist1 = MDxfGetStrItem( dxfFile, sCode); 
				break;
			default:
				ist1 = MDxfGetStrItem( dxfFile, sDummy); 
				break;
		}
	}
	return ist1;
}

///////////////////////////////////////////////////////////////////////////
//�@�c�w�e�t�@�C���̑����v�f��Ǎ��� ( "ATTRIB")
//			ist1 == MDxf_attrib_get();
MINT MDxf_attrib_get(
						MBHZDT	&dxfFile,				// [I O} DXF�t�@�C���n���h��
						MCHAR   *sCode					// [  O] Next entity
				)
{
	MINT		ist;
	MINT		ist1;
	bool		fEnd = false;

	MINT		iType;
	MCHAR		sLineName[MSZ_DXFLINE];
	MCHAR		sFontName[MSZ_DXFLINE];
	MCHAR		sLayerName[MSZ_DXFLINE];
	MCHAR		sText[MSZ_DXFLINE];
	MgPoint3	Point;
	MFLOAT		rHeight;
	MFLOAT		rAngle;
	MINT		iColor;
	
	MCHAR		sDummy[MSZ_DXFLINE];

	sLineName[0] = NULL;
	sLayerName[0] = NULL;
	iColor = 1;
	
	while (!fEnd && (ist = MDxfGetIntItem( dxfFile, &iType)) == 0) {
		switch (iType) {
			case 6:
				ist1 = MDxfGetStrItem( dxfFile, sLineName); 
				break;
			case 7:
				ist1 = MDxfGetStrItem( dxfFile, sFontName); 
				break;
			case 8:
				ist1 = MDxfGetStrItem( dxfFile, sLayerName); 
				break;
			case 1:
				ist1 = MDxfGetStrItem( dxfFile, sText); 
				break;
			case 10:
				ist1 = MDxfGetFltItem( dxfFile, &Point.x); 
				break;
			case 20:
				ist1 = MDxfGetFltItem( dxfFile, &Point.y); 
				break;
			case 30:
				ist1 = MDxfGetFltItem( dxfFile, &Point.z); 
				break;
			case 40:
				ist1 = MDxfGetFltItem( dxfFile, &rHeight); 
				break;
			case 50:
				ist1 = MDxfGetFltItem( dxfFile, &rAngle); 
				break;
			case 62:
				ist1 = MDxfGetIntItem( dxfFile, &iColor); 
				break;
			case 0:
				fEnd = true;
				ist1 = MDxfGetStrItem( dxfFile, sCode); 
				break;
			default:
				ist1 = MDxfGetStrItem( dxfFile, sDummy); 
				break;
		}
	}
	return ist1;
}

///////////////////////////////////////////////////////////////////////////
//�@�c�w�e�t�@�C���̂R�����ʗv�f��Ǎ��� ( "3DFACE")
//			ist1 == MDxf_3dface_get();
MINT MDxf_3dface_get(
						MBHZDT	&dxfFile,				// [I O} DXF�t�@�C���n���h��
						MCHAR   *sCode,					// [  O] Next entity
						MdPrm	*uPrm
				)
{
	MINT		ist;
	MINT		ist1;
	bool		fEnd = false;

	MINT		iType;
	MCHAR		sLineName[MSZ_DXFLINE];
	MCHAR		sLayerName[MSZ_DXFLINE];
	MgPoint3	ptFace[4];
	MINT		iColor;
	MINT		iC;
	
	MCHAR		sDummy[MSZ_DXFLINE];

	sLineName[0] = NULL;
	sLayerName[0] = NULL;
	iColor = 1;
	
	while (!fEnd && (ist = MDxfGetIntItem( dxfFile, &iType)) == 0) {
		switch (iType) {
			case 6:
				ist1 = MDxfGetStrItem( dxfFile, sLineName); 
				break;
			case 8:
				ist1 = MDxfGetStrItem( dxfFile, sLayerName); 
				break;
			case 10:
				ist1 = MDxfGetFltItem( dxfFile, &ptFace[0].x); 
				break;
			case 20:
				ist1 = MDxfGetFltItem( dxfFile, &ptFace[0].y); 
				break;
			case 30:
				ist1 = MDxfGetFltItem( dxfFile, &ptFace[0].z); 
				break;
			case 11:
				ist1 = MDxfGetFltItem( dxfFile, &ptFace[1].x); 
				break;
			case 21:
				ist1 = MDxfGetFltItem( dxfFile, &ptFace[1].y); 
				break;
			case 31:
				ist1 = MDxfGetFltItem( dxfFile, &ptFace[1].z); 
				break;
			case 12:
				ist1 = MDxfGetFltItem( dxfFile, &ptFace[2].x); 
				break;
			case 22:
				ist1 = MDxfGetFltItem( dxfFile, &ptFace[2].y); 
				break;
			case 32:
				ist1 = MDxfGetFltItem( dxfFile, &ptFace[2].z); 
				break;
			case 13:
				ist1 = MDxfGetFltItem( dxfFile, &ptFace[3].x); 
				break;
			case 23:
				ist1 = MDxfGetFltItem( dxfFile, &ptFace[3].y); 
				break;
			case 33:
				ist1 = MDxfGetFltItem( dxfFile, &ptFace[3].z); 
				break;
			case 62:
				ist1 = MDxfGetIntItem( dxfFile, &iColor); 
				break;
			case 70:
				ist1 = MDxfGetIntItem( dxfFile, &iC); 
				break;
			case 0:
				fEnd = true;
				ist1 = MDxfGetStrItem( dxfFile, sCode); 
				break;
			default:
				ist1 = MDxfGetStrItem( dxfFile, sDummy); 
				break;
		}
	}
	return ist1;
}

///////////////////////////////////////////////////////////////////////////
//�@�c�w�e�t�@�C���̑}���v�f��Ǎ��� ( "INSERT")
//			ist1 == MDxf_insert_get();
MINT MDxf_insert_get(
						MBHZDT	&dxfFile,				// [I O} DXF�t�@�C���n���h��
						MCHAR   *sCode					// [  O] Next entity
				)
{
	MINT		ist;
	MINT		ist1;
	bool		fEnd = false;

	MINT		iType;
	MCHAR		sLineName[MSZ_DXFLINE];
	MCHAR		sLayerName[MSZ_DXFLINE];
	MgLine3		Line;
	MINT		iColor;
	
	MCHAR		sDummy[MSZ_DXFLINE];

	sLineName[0] = NULL;
	sLayerName[0] = NULL;
	iColor = 1;
	
	while (!fEnd && (ist = MDxfGetIntItem( dxfFile, &iType)) == 0) {
		switch (iType) {
			case 6:
				ist1 = MDxfGetStrItem( dxfFile, sLineName); 
				break;
			case 8:
				ist1 = MDxfGetStrItem( dxfFile, sLayerName); 
				break;
			case 10:
				ist1 = MDxfGetFltItem( dxfFile, &Line.p[0].x); 
				break;
			case 20:
				ist1 = MDxfGetFltItem( dxfFile, &Line.p[0].y); 
				break;
			case 30:
				ist1 = MDxfGetFltItem( dxfFile, &Line.p[0].z); 
				break;
			case 11:
				ist1 = MDxfGetFltItem( dxfFile, &Line.p[1].x); 
				break;
			case 21:
				ist1 = MDxfGetFltItem( dxfFile, &Line.p[1].y); 
				break;
			case 31:
				ist1 = MDxfGetFltItem( dxfFile, &Line.p[1].z); 
				break;
			case 62:
				ist1 = MDxfGetIntItem( dxfFile, &iColor); 
				break;
			case 0:
				fEnd = true;
				ist1 = MDxfGetStrItem( dxfFile, sCode); 
				break;
			default:
				ist1 = MDxfGetStrItem( dxfFile, sDummy); 
				break;
		}
	}
	return ist1;
}

///////////////////////////////////////////////////////////////////////////
//�@�c�w�e�t�@�C���̑}���v�f��Ǎ��� ( "INSERT")
//			ist1 == MDxf_insert_get();
MINT MDxf_other_get(
						MBHZDT	&dxfFile,				// [I O} DXF�t�@�C���n���h��
						MCHAR   *sCode					// [  O] Next entity
				)
{
	MINT		ist;
	MINT		ist1;
	bool		fEnd = false;

	MINT		iType;
	MCHAR		sLineName[MSZ_DXFLINE];
	MCHAR		sLayerName[MSZ_DXFLINE];
	MgLine3		Line;
	MINT		iColor;
	
	MCHAR		sDummy[MSZ_DXFLINE];

	sLineName[0] = NULL;
	sLayerName[0] = NULL;
	iColor = 1;
	
	while (!fEnd && (ist = MDxfGetIntItem( dxfFile, &iType)) == 0) {
		switch (iType) {
			case 6:
				ist1 = MDxfGetStrItem( dxfFile, sLineName); 
				break;
			case 8:
				ist1 = MDxfGetStrItem( dxfFile, sLayerName); 
				break;
			case 10:
				ist1 = MDxfGetFltItem( dxfFile, &Line.p[0].x); 
				break;
			case 20:
				ist1 = MDxfGetFltItem( dxfFile, &Line.p[0].y); 
				break;
			case 30:
				ist1 = MDxfGetFltItem( dxfFile, &Line.p[0].z); 
				break;
			case 11:
				ist1 = MDxfGetFltItem( dxfFile, &Line.p[1].x); 
				break;
			case 21:
				ist1 = MDxfGetFltItem( dxfFile, &Line.p[1].y); 
				break;
			case 31:
				ist1 = MDxfGetFltItem( dxfFile, &Line.p[1].z); 
				break;
			case 62:
				ist1 = MDxfGetIntItem( dxfFile, &iColor); 
				break;
			case 0:
				fEnd = true;
				ist1 = MDxfGetStrItem( dxfFile, sCode); 
				break;
			default:
				ist1 = MDxfGetStrItem( dxfFile, sDummy); 
				break;
		}
	}
	return ist1;
}

///////////////////////////////////////////////////////////////////////////
//�@�c�w�e�t�@�C���̃G���e�B�e�B�Z�N�V������Ǎ���

MINT MDxf_entities_section_read(
//						MBHdxfFile, DxfInf);
						MBHZDT	&dxfFile				// [I O} DXF�t�@�C���n���h��
				)
{
	MINT		ist;

	MINT		ist1;

	MINT		iType;
	MCHAR		sCode[MSZ_DXFLINE];

	MdPrm		uPrm;

	MmCurrentWndFocus();
	MmWndInfo* pWndInfo = MmWndKGetCurrentWnd();				// �J�����g�E�B���h�E�擾

	MbCod*	pCod = pWndInfo;

	CClientDC	dc( pWndInfo->GetWnd());
	DWORD		rgbTempRfm = MmSystemPropGetColor( MM_COLOR_TEMP);

	pCod->SetLineAttr( MPS_SOLID, 1, rgbTempRfm);

//	pCod->Polygon( ppgBziFig->p, ppgBziFig->n);

	ist1 = MDxfGetStrData( 0, dxfFile, &iType, sCode);
	while (ist1 == 0 &&	Mstrcmp( sCode, Mstr( "ENDSEC")) != 0) {
//		if (iType == 0) continue;

		if ( Mstrcmp( sCode, Mstr( "POINT")) == 0) {
			ist1 = MDxf_point_get( dxfFile, sCode, &uPrm);

		} else if( Mstrcmp( sCode, Mstr( "LINE")) == 0) {
			ist1 = MDxf_line_get( dxfFile, sCode, &uPrm);

		} else if( Mstrcmp( sCode, Mstr( "POLYLINE")) == 0) {
			ist1 = MDxf_polyline_get( dxfFile, sCode, &uPrm);

		} else if( Mstrcmp( sCode, Mstr( "CIRCLE")) == 0) {
			ist1 = MDxf_circle_get( dxfFile, sCode, &uPrm);

		} else if( Mstrcmp( sCode, Mstr( "ARC")) == 0) {
			ist1 = MDxf_arc_get( dxfFile, sCode, &uPrm);

		} else if( Mstrcmp( sCode, Mstr( "TEXT")) == 0) {
			ist1 = MDxf_text_get( dxfFile, sCode, &uPrm);

		} else if( Mstrcmp( sCode, Mstr( "ATTRIB")) == 0) {
			ist1 = MDxf_attrib_get( dxfFile, sCode);

		} else if( Mstrcmp( sCode, Mstr( "3DFACE")) == 0) {
			ist1 = MDxf_3dface_get( dxfFile, sCode, &uPrm);

		} else if( Mstrcmp( sCode, Mstr( "INSERT")) == 0) {
			ist1 = MDxf_insert_get( dxfFile, sCode);

		} else {
			ist1 = MDxf_other_get( dxfFile, sCode);
		}
	}
	return ist1;
}

////////////////////////////////////////////////////////////////////////////
//	�c�w�e�t�@�C���̓ǂݍ���
//	�X�e�C�^�X     0 : ����  < 0 : �G���[

MINT MDxfFlLoad(									//
//						MCHAR*		PathI			// (I  ) �ǂݍ��݃t�@�C���@�t���p�X
				)
{
	MINT		ist;
	MINT		istg;
	MINT		ist1;

	MCHAR		DxfPath[MX_PATH];
	MBHZDT		dxfFile;

	MINT		iType;
	MCHAR		sCode[MSZ_DXFLINE];
	
	//	�c�w�e�t�@�C���̓ǂݍ���

	Mstrcpy( DxfPath, MmSystemPropGetStr( MM_STR_PARTS_DIR));
//	Mstrcat( DxfPath, _T("Test1.dxf"));
	Mstrcat( DxfPath, _T("Test1.txt"));

	ist = dxfFile.Open( DxfPath);
	ASSERT( ist == 0);

	istg = 0;

	istg =  MDxfGetStrData( 1, dxfFile, &iType, sCode);

	while (istg != MBEOF) {
		if (Mstrcmp( sCode, Mstr("SECTION")) == 0) {

			istg =  MDxfGetStrData( 0, dxfFile, &iType, sCode);
			if (iType != 2)
				goto  exit;

			// �Z�N�V�����ʓǍ���
			if (Mstrcmp( sCode, Mstr("HEADER")) == 0) {
//A				ist1 = MDxf_header_section_read( dxfFile, DxfInf);
				ist1 = MDxf_sektion_skip( dxfFile);

			} else if (Mstrcmp( sCode, Mstr("CLASSES")) == 0) {
//				ist1 = MDxf_classes_section_read( dxfFile);
				ist1 = MDxf_sektion_skip( dxfFile);

			} else if (Mstrcmp( sCode, Mstr("TABLES")) == 0) {
//A				ist1 = MDxf_tables_section_read( dxfFile, DxfInf);
				ist1 = MDxf_sektion_skip( dxfFile);

			} else if (Mstrcmp( sCode, Mstr("BLOCKS")) == 0) {
//A				ist1 = MDxf_blocks_section_read( dxfFile, DxfInf);
				ist1 = MDxf_sektion_skip( dxfFile);

			} else if (Mstrcmp( sCode, Mstr("ENTITIES")) == 0) {
//				ist1 = MDxf_entities_section_read( dxfFile, DxfInf);
				ist1 = MDxf_entities_section_read( dxfFile);
//				ist1 = MDxf_sektion_skip( dxfFile);

			} else if (Mstrcmp( sCode, Mstr("OBJECTS")) == 0) {
//				ist1 = MDxf_objects_section_read( dxfFile);
				ist1 = MDxf_sektion_skip( dxfFile);

			} else {
				ist1 = -1;
			}
			if (ist1 != 0)
				goto exit;

		} else if (Mstrcmp( sCode, Mstr("EOF")) == 0) {
			ist1 = 0;
			break;

		} else {
			ist1 = -2;
			goto exit;
		}
		istg =  MDxfGetStrData( 0, dxfFile, &iType, sCode);
	}
exit:
	ist = dxfFile.Close();

	return ist1;
}

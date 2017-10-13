/*********************************************************************************************************************************
              �{��  �v���O�����t�@�C�� (HsDHon.cpp)

********************************************************************************************************************************/

#include		"MCad.h"
//#include		"HBase.h"
#include		"MgLib.h"
//#include		"HsFileLib.h"
#include		"MDxfR14.h"


#define			 STRLNG			64														// �����񒷂�

// �w�b�_�[
static MCHAR HEADER[19][STRLNG] =
{
	BwsStr("  0"),    BwsStr("SECTION"),   BwsStr("  2"), BwsStr("HEADER"),        BwsStr("  9"),  BwsStr("$ACADVER"),
	BwsStr("  1"),    BwsStr("AC1014"),    BwsStr("  9"), BwsStr("$HANDLING"),     BwsStr("  70"), BwsStr("     1"),
	BwsStr("  9"),    BwsStr("$HANDSEED"), BwsStr("  5"), BwsStr("hcnt[15]"),      BwsStr("  0"),  BwsStr("ENDSEC"),
	BwsStr("$E$N$D$")
};

// �N���X
static MCHAR CLASS[7][STRLNG] =
{
	BwsStr("  0"),   BwsStr("SECTION"),    BwsStr("  2"), BwsStr("CLASSES"),       BwsStr("  0"),  BwsStr("ENDSEC"),
	BwsStr("$E$N$D$")
};

// �I�u�W�F�N�g
static MCHAR OBJECT[89][STRLNG] =
{
	BwsStr("  0"),  BwsStr("SECTION"),     BwsStr("  2"), BwsStr("OBJECTS"),         BwsStr("  0"), BwsStr("DICTIONARY"),
	BwsStr("  5"),  BwsStr("C"),           BwsStr("100"), BwsStr("AcDbDictionary"),  BwsStr("  3"), BwsStr("ACAD_GROUP"),
	BwsStr("350"),  BwsStr("D"),           BwsStr("  3"), BwsStr("ACAD_MLINESTYLE"), BwsStr("350"), BwsStr("1B"),
	BwsStr("  0"),  BwsStr("DICTIONARY"),  BwsStr("  5"), BwsStr("D"),               BwsStr("102"), BwsStr("{ACAD_REACTORS"),
	BwsStr("330"),  BwsStr("C"),           BwsStr("102"), BwsStr("}"),               BwsStr("100"), BwsStr("AcDbDictionary"),
	BwsStr("  0"),  BwsStr("DICTIONARY"),  BwsStr("  5"), BwsStr("1B"),              BwsStr("102"), BwsStr("{ACAD_REACTORS"),
	BwsStr("330"),  BwsStr("C"),           BwsStr("102"), BwsStr("}"),               BwsStr("100"), BwsStr("AcDbDictionary"),
	BwsStr("  3"),  BwsStr("STANDARD"),    BwsStr("350"), BwsStr("1C"),              BwsStr("  0"), BwsStr("MLINESTYLE"),
	BwsStr("  5"),  BwsStr("1C"),          BwsStr("102"), BwsStr("{ACAD_REACTORS"),  BwsStr("330"), BwsStr("1B"),
	BwsStr("102"),  BwsStr("}"),           BwsStr("100"), BwsStr("AcDbMlineStyle"),  BwsStr("  2"), BwsStr("STANDARD"),
	BwsStr(" 70"),  BwsStr("     0"),      BwsStr("  3"), BwsStr(""),                BwsStr(" 62"), BwsStr("   256"),
	BwsStr(" 51"),  BwsStr("90.0"),        BwsStr(" 52"), BwsStr("90.0"),            BwsStr(" 71"), BwsStr("     2"),
	BwsStr(" 49"),  BwsStr("0.5"),         BwsStr(" 62"), BwsStr("   256"),          BwsStr("  6"), BwsStr("BYLAYER"),
	BwsStr(" 49"),  BwsStr("-0.5"),        BwsStr(" 62"), BwsStr("   256"),          BwsStr("  6"), BwsStr("BYLAYER"),
	BwsStr("  0"),  BwsStr("ENDSEC"),      BwsStr("  0"), BwsStr("EOF"),             BwsStr("$E$N$D$")
};

// �r���[�|�[�g
static MCHAR VIEWPORT[47][STRLNG] =
{
	BwsStr("  0"),  BwsStr("SECTION"),     BwsStr("  2"), BwsStr("TABLES"),        BwsStr("  0"), BwsStr("TABLE"),
	BwsStr("  2"),  BwsStr("VPORT"),       BwsStr("  5"), BwsStr("8"),             BwsStr("100"), BwsStr("AcDbSymbolTable"),
	BwsStr(" 70"),  BwsStr("     2"),      BwsStr("  0"), BwsStr("VPORT"),         BwsStr("  5"), BwsStr("1E"),
	BwsStr("100"),  BwsStr("AcDbSymbolTableRecord"),      BwsStr("100"), BwsStr("AcDbViewportTableRecord"),
	BwsStr("  2"),  BwsStr("*ACTIVE"),     BwsStr(" 70"), BwsStr("     0"),        BwsStr(" 10"), BwsStr("0.0"),
	BwsStr(" 20"),  BwsStr("0.0"),         BwsStr(" 11"), BwsStr("1.0"),           BwsStr(" 21"), BwsStr("1.0"),
	BwsStr(" 12"),  BwsStr("276.003074"),  BwsStr(" 22"), BwsStr("148.5"),         BwsStr(" 40"), BwsStr("297.0"),
	BwsStr(" 41"),  BwsStr("1.858607"),	   BwsStr(" 42"), BwsStr("50.0"),          BwsStr("  0"), BwsStr("ENDTAB"),
	BwsStr("$E$N$D$")
};


// ������
static MCHAR LTYPEINF[251][STRLNG] =
{
	BwsStr("  0"), BwsStr("TABLE"),
	BwsStr("  2"), BwsStr("LTYPE"),        BwsStr("  5"), BwsStr("5"),             BwsStr("100"), BwsStr("AcDbSymbolTable"),
	BwsStr(" 70"), BwsStr("     9"),
	BwsStr("  0"), BwsStr("LTYPE"),        BwsStr("  5"), BwsStr("12"),            BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbLinetypeTableRecord"),                              BwsStr("  2"), BwsStr("BYBLOCK"),
	BwsStr(" 70"), BwsStr("     0"),       BwsStr("  3"), BwsStr(""),              BwsStr(" 72"), BwsStr("    65"),
	BwsStr(" 73"), BwsStr("     0"),       BwsStr(" 40"), BwsStr("0.0"),           BwsStr(" 74"), BwsStr("     0"),
	BwsStr("  0"), BwsStr("LTYPE"),        BwsStr("  5"), BwsStr("13"),            BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbLinetypeTableRecord"),                              BwsStr("  2"), BwsStr("BYLAYER"),
	BwsStr(" 70"), BwsStr("     0"),       BwsStr("  3"), BwsStr(""),              BwsStr(" 72"), BwsStr("    65"),
	BwsStr(" 73"), BwsStr("     0"),       BwsStr(" 40"), BwsStr("0.0"),           BwsStr(" 74"), BwsStr("     0"),
	BwsStr("  0"), BwsStr("LTYPE"),        BwsStr("  5"), BwsStr("14"),            BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbLinetypeTableRecord"),                              BwsStr("  2"), BwsStr("CONTINUOUS"),
	BwsStr(" 70"), BwsStr("    64"),       BwsStr("  3"), BwsStr("Solid line"),    BwsStr(" 72"), BwsStr("    65"),
	BwsStr(" 73"), BwsStr("     0"),       BwsStr(" 40"), BwsStr("0.0"),           BwsStr(" 74"), BwsStr("     0"),
	BwsStr("  0"), BwsStr("LTYPE"),        BwsStr("  5"), BwsStr("1F"),            BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbLinetypeTableRecord"),                              BwsStr("  2"), BwsStr("HIDDEN"),
	BwsStr(" 70"), BwsStr("    64"),       BwsStr("  3"), BwsStr("- - - - - - - - - -"),
	BwsStr(" 72"), BwsStr("    65"),       BwsStr(" 73"), BwsStr("     2"),        BwsStr(" 40"), BwsStr("0.375"),
	BwsStr(" 49"), BwsStr("0.25"),         BwsStr(" 74"), BwsStr("     0"),        BwsStr(" 49"), BwsStr("-0.125"),
	BwsStr(" 74"), BwsStr("     0"),
	BwsStr("  0"), BwsStr("LTYPE"),        BwsStr("  5"), BwsStr("20"),            BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbLinetypeTableRecord"),                              BwsStr("  2"), BwsStr("DOT"),
	BwsStr(" 70"), BwsStr("    64"),       BwsStr("  3"), BwsStr("...................."),
	BwsStr(" 72"), BwsStr("    65"),       BwsStr(" 73"), BwsStr("     2"),        BwsStr(" 40"), BwsStr("0.25"),
	BwsStr(" 49"), BwsStr("0.0"),          BwsStr(" 74"), BwsStr("     0"),        BwsStr(" 49"), BwsStr("-0.25"),
	BwsStr(" 74"), BwsStr("     0"),
	BwsStr("  0"), BwsStr("LTYPE"),        BwsStr("  5"), BwsStr("21"),            BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbLinetypeTableRecord"),                              BwsStr("  2"), BwsStr("CENTER"),
	BwsStr(" 70"), BwsStr("    64"),       BwsStr("  3"), BwsStr("----- - ----- - -----"),
	BwsStr(" 72"), BwsStr("    65"),       BwsStr(" 73"), BwsStr("     4"),        BwsStr(" 40"), BwsStr("2.0"),
	BwsStr(" 49"), BwsStr("1.25"),         BwsStr(" 74"), BwsStr("     0"),        BwsStr(" 49"), BwsStr("-0.25"),
	BwsStr(" 74"), BwsStr("     0"),       BwsStr(" 49"), BwsStr("0.25"),          BwsStr(" 74"), BwsStr("     0"),
	BwsStr(" 49"), BwsStr("-0.25"),        BwsStr(" 74"), BwsStr("     0"),
	BwsStr("  0"), BwsStr("LTYPE"),        BwsStr("  5"), BwsStr("22"),            BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbLinetypeTableRecord"),                              BwsStr("  2"), BwsStr("PHANTOM"),
	BwsStr(" 70"), BwsStr("    64"),       BwsStr("  3"), BwsStr("----- - - ----- - - -----"),
	BwsStr(" 72"), BwsStr("    65"),       BwsStr(" 73"), BwsStr("     6"),        BwsStr(" 40"), BwsStr("2.5"),
	BwsStr(" 49"), BwsStr("1.25"),         BwsStr(" 74"), BwsStr("     0"),        BwsStr(" 49"), BwsStr("-0.25"),
	BwsStr(" 74"), BwsStr("     0"),       BwsStr(" 49"), BwsStr("0.25"),          BwsStr(" 74"), BwsStr("     0"),
	BwsStr(" 49"), BwsStr("-0.25"),        BwsStr(" 74"), BwsStr("     0"),        BwsStr(" 49"), BwsStr("0.25"),
	BwsStr(" 74"), BwsStr("     0"),       BwsStr(" 49"), BwsStr("-0.25"),         BwsStr(" 74"), BwsStr("     0"),
	BwsStr("  0"), BwsStr("LTYPE"),        BwsStr("  5"), BwsStr("23"),            BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbLinetypeTableRecord"),                              BwsStr("  2"), BwsStr("CENTERL"),
	BwsStr(" 70"), BwsStr("    64"),       BwsStr("  3"), BwsStr("------ - ------ - ------"),
	BwsStr(" 72"), BwsStr("    65"),       BwsStr(" 73"), BwsStr("     4"),        BwsStr(" 40"), BwsStr("4.0"),
	BwsStr(" 49"), BwsStr("3.25"),         BwsStr(" 74"), BwsStr("     0"),        BwsStr(" 49"), BwsStr("-0.25"),
	BwsStr(" 74"), BwsStr("     0"),       BwsStr(" 49"), BwsStr("0.25"),          BwsStr(" 74"), BwsStr("     0"),
	BwsStr(" 49"), BwsStr("-0.25"),        BwsStr(" 74"), BwsStr("     0"),
	BwsStr("  0"), BwsStr("ENDTAB"),       BwsStr("$E$N$D$")
};

// ���C���O���
static MCHAR LAYINF00[27][STRLNG] =
{
	BwsStr("  0"), BwsStr("TABLE"),             BwsStr("  2"), BwsStr("LAYER"),    BwsStr("  5"), BwsStr("2"),
	BwsStr("100"), BwsStr("AcDbSymbolTable"),   BwsStr(" 70"), BwsStr("     2"),
	BwsStr("  0"), BwsStr("LAYER"),             BwsStr("  5"), BwsStr("24"),       BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbLayerTableRecord"),                                 BwsStr("  2"), BwsStr("PEN0"),
	BwsStr(" 70"), BwsStr("    64"),            BwsStr(" 62"), BwsStr("     7"),   BwsStr("  6"), BwsStr("CONTINUOUS"),
	BwsStr("$E$N$D$")
};
static MCHAR LAYINF01[17][STRLNG] =
{
	BwsStr("  0"), BwsStr("LAYER"),             BwsStr("  5"), BwsStr("25"),       BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbLayerTableRecord"),                                 BwsStr("  2"), BwsStr("0"),
	BwsStr(" 70"), BwsStr("    64"),            BwsStr(" 62"), BwsStr("     7"),   BwsStr("  6"), BwsStr("CONTINUOUS"),
	BwsStr("$E$N$D$")
};

// ���C�����
static MCHAR LAYINF33[539][STRLNG] =
{
	BwsStr("  0"), BwsStr("TABLE"),             BwsStr("  2"), BwsStr("LAYER"),    BwsStr("  5"), BwsStr("2"),
	BwsStr("100"), BwsStr("AcDbSymbolTable"),   BwsStr(" 70"), BwsStr("    33"),
	BwsStr("  0"), BwsStr("LAYER"),             BwsStr("  5"), BwsStr("24"),       BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbLayerTableRecord"),                                 BwsStr("  2"), BwsStr("PEN0"),
	BwsStr(" 70"), BwsStr("    64"),            BwsStr(" 62"), BwsStr("     7"),   BwsStr("  6"), BwsStr("CONTINUOUS"),
	BwsStr("  0"), BwsStr("LAYER"),             BwsStr("  5"), BwsStr("25"),       BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbLayerTableRecord"),                                 BwsStr("  2"), BwsStr("0"),
	BwsStr(" 70"), BwsStr("    64"),            BwsStr(" 62"), BwsStr("     7"),   BwsStr("  6"), BwsStr("CONTINUOUS"),
	BwsStr("  0"), BwsStr("LAYER"),             BwsStr("  5"), BwsStr("26"),       BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbLayerTableRecord"),                                 BwsStr("  2"), BwsStr("1"),
	BwsStr(" 70"), BwsStr("    64"),            BwsStr(" 62"), BwsStr("     7"),   BwsStr("  6"), BwsStr("CONTINUOUS"),
	BwsStr("  0"), BwsStr("LAYER"),             BwsStr("  5"), BwsStr("27"),       BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbLayerTableRecord"),                                 BwsStr("  2"), BwsStr("2"),
	BwsStr(" 70"), BwsStr("    64"),            BwsStr(" 62"), BwsStr("     7"),   BwsStr("  6"), BwsStr("CONTINUOUS"),
	BwsStr("  0"), BwsStr("LAYER"),             BwsStr("  5"), BwsStr("28"),       BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbLayerTableRecord"),                                 BwsStr("  2"), BwsStr("3"),
	BwsStr(" 70"), BwsStr("    64"),            BwsStr(" 62"), BwsStr("     7"),   BwsStr("  6"), BwsStr("CONTINUOUS"),
	BwsStr("  0"), BwsStr("LAYER"),             BwsStr("  5"), BwsStr("29"),       BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbLayerTableRecord"),                                 BwsStr("  2"), BwsStr("4"),
	BwsStr(" 70"), BwsStr("    64"),            BwsStr(" 62"), BwsStr("     7"),   BwsStr("  6"), BwsStr("CONTINUOUS"),
	BwsStr("  0"), BwsStr("LAYER"),             BwsStr("  5"), BwsStr("2A"),       BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbLayerTableRecord"),                                 BwsStr("  2"), BwsStr("5"),
	BwsStr(" 70"), BwsStr("    64"),            BwsStr(" 62"), BwsStr("     7"),   BwsStr("  6"), BwsStr("CONTINUOUS"),
	BwsStr("  0"), BwsStr("LAYER"),             BwsStr("  5"), BwsStr("2B"),       BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbLayerTableRecord"),                                 BwsStr("  2"), BwsStr("6"),
	BwsStr(" 70"), BwsStr("    64"),            BwsStr(" 62"), BwsStr("     7"),   BwsStr("  6"), BwsStr("CONTINUOUS"),
	BwsStr("  0"), BwsStr("LAYER"),             BwsStr("  5"), BwsStr("2C"),       BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbLayerTableRecord"),                                 BwsStr("  2"), BwsStr("7"),
	BwsStr(" 70"), BwsStr("    64"),            BwsStr(" 62"), BwsStr("     7"),   BwsStr("  6"), BwsStr("CONTINUOUS"),
	BwsStr("  0"), BwsStr("LAYER"),             BwsStr("  5"), BwsStr("2D"),       BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbLayerTableRecord"),                                 BwsStr("  2"), BwsStr("8"),
	BwsStr(" 70"), BwsStr("    64"),            BwsStr(" 62"), BwsStr("     7"),   BwsStr("  6"), BwsStr("CONTINUOUS"),
	BwsStr("  0"), BwsStr("LAYER"),             BwsStr("  5"), BwsStr("2E"),       BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbLayerTableRecord"),                                 BwsStr("  2"), BwsStr("9"),
	BwsStr(" 70"), BwsStr("    64"),            BwsStr(" 62"), BwsStr("     7"),   BwsStr("  6"), BwsStr("CONTINUOUS"),
	BwsStr("  0"), BwsStr("LAYER"),             BwsStr("  5"), BwsStr("2F"),       BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbLayerTableRecord"),                                 BwsStr("  2"), BwsStr("10"),
	BwsStr(" 70"), BwsStr("    64"),            BwsStr(" 62"), BwsStr("     7"),   BwsStr("  6"), BwsStr("CONTINUOUS"),
	BwsStr("  0"), BwsStr("LAYER"),             BwsStr("  5"), BwsStr("30"),       BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbLayerTableRecord"),                                 BwsStr("  2"), BwsStr("11"),
	BwsStr(" 70"), BwsStr("    64"),            BwsStr(" 62"), BwsStr("     7"),   BwsStr("  6"), BwsStr("CONTINUOUS"),
	BwsStr("  0"), BwsStr("LAYER"),             BwsStr("  5"), BwsStr("31"),       BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbLayerTableRecord"),                                 BwsStr("  2"), BwsStr("12"),
	BwsStr(" 70"), BwsStr("    64"),            BwsStr(" 62"), BwsStr("     7"),   BwsStr("  6"), BwsStr("CONTINUOUS"),
	BwsStr("  0"), BwsStr("LAYER"),             BwsStr("  5"), BwsStr("32"),       BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbLayerTableRecord"),                                 BwsStr("  2"), BwsStr("13"),
	BwsStr(" 70"), BwsStr("    64"),            BwsStr(" 62"), BwsStr("     7"),   BwsStr("  6"), BwsStr("CONTINUOUS"),
	BwsStr("  0"), BwsStr("LAYER"),             BwsStr("  5"), BwsStr("33"),       BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbLayerTableRecord"),                                 BwsStr("  2"), BwsStr("14"),
	BwsStr(" 70"), BwsStr("    64"),            BwsStr(" 62"), BwsStr("     7"),   BwsStr("  6"), BwsStr("CONTINUOUS"),
	BwsStr("  0"), BwsStr("LAYER"),             BwsStr("  5"), BwsStr("34"),       BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbLayerTableRecord"),                                 BwsStr("  2"), BwsStr("15"),
	BwsStr(" 70"), BwsStr("    64"),            BwsStr(" 62"), BwsStr("     7"),   BwsStr("  6"), BwsStr("CONTINUOUS"),
	BwsStr("  0"), BwsStr("LAYER"),             BwsStr("  5"), BwsStr("35"),       BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbLayerTableRecord"),                                 BwsStr("  2"), BwsStr("16"),
	BwsStr(" 70"), BwsStr("    64"),            BwsStr(" 62"), BwsStr("     7"),   BwsStr("  6"), BwsStr("CONTINUOUS"),
	BwsStr("  0"), BwsStr("LAYER"),             BwsStr("  5"), BwsStr("36"),       BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbLayerTableRecord"),                                 BwsStr("  2"), BwsStr("17"),
	BwsStr(" 70"), BwsStr("    64"),            BwsStr(" 62"), BwsStr("     7"),   BwsStr("  6"), BwsStr("CONTINUOUS"),
	BwsStr("  0"), BwsStr("LAYER"),             BwsStr("  5"), BwsStr("37"),       BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbLayerTableRecord"),                                 BwsStr("  2"), BwsStr("18"),
	BwsStr(" 70"), BwsStr("    64"),            BwsStr(" 62"), BwsStr("     7"),   BwsStr("  6"), BwsStr("CONTINUOUS"),
	BwsStr("  0"), BwsStr("LAYER"),             BwsStr("  5"), BwsStr("38"),       BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbLayerTableRecord"),                                 BwsStr("  2"), BwsStr("19"),
	BwsStr(" 70"), BwsStr("    64"),            BwsStr(" 62"), BwsStr("     7"),   BwsStr("  6"), BwsStr("CONTINUOUS"),
	BwsStr("  0"), BwsStr("LAYER"),             BwsStr("  5"), BwsStr("39"),       BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbLayerTableRecord"),                                 BwsStr("  2"), BwsStr("20"),
	BwsStr(" 70"), BwsStr("    64"),            BwsStr(" 62"), BwsStr("     7"),   BwsStr("  6"), BwsStr("CONTINUOUS"),
	BwsStr("  0"), BwsStr("LAYER"),             BwsStr("  5"), BwsStr("3A"),       BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbLayerTableRecord"),                                 BwsStr("  2"), BwsStr("21"),
	BwsStr(" 70"), BwsStr("    64"),            BwsStr(" 62"), BwsStr("     7"),   BwsStr("  6"), BwsStr("CONTINUOUS"),
	BwsStr("  0"), BwsStr("LAYER"),             BwsStr("  5"), BwsStr("3B"),       BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbLayerTableRecord"),                                 BwsStr("  2"), BwsStr("22"),
	BwsStr(" 70"), BwsStr("    64"),            BwsStr(" 62"), BwsStr("     7"),   BwsStr("  6"), BwsStr("CONTINUOUS"),
	BwsStr("  0"), BwsStr("LAYER"),             BwsStr("  5"), BwsStr("3C"),       BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbLayerTableRecord"),                                 BwsStr("  2"), BwsStr("23"),
	BwsStr(" 70"), BwsStr("    64"),            BwsStr(" 62"), BwsStr("     7"),   BwsStr("  6"), BwsStr("CONTINUOUS"),
	BwsStr("  0"), BwsStr("LAYER"),             BwsStr("  5"), BwsStr("3D"),       BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbLayerTableRecord"),                                 BwsStr("  2"), BwsStr("24"),
	BwsStr(" 70"), BwsStr("    64"),            BwsStr(" 62"), BwsStr("     7"),   BwsStr("  6"), BwsStr("CONTINUOUS"),
	BwsStr("  0"), BwsStr("LAYER"),             BwsStr("  5"), BwsStr("3E"),       BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbLayerTableRecord"),                                 BwsStr("  2"), BwsStr("25"),
	BwsStr(" 70"), BwsStr("    64"),            BwsStr(" 62"), BwsStr("     7"),   BwsStr("  6"), BwsStr("CONTINUOUS"),
	BwsStr("  0"), BwsStr("LAYER"),             BwsStr("  5"), BwsStr("3F"),       BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbLayerTableRecord"),                                 BwsStr("  2"), BwsStr("26"),
	BwsStr(" 70"), BwsStr("    64"),            BwsStr(" 62"), BwsStr("     7"),   BwsStr("  6"), BwsStr("CONTINUOUS"),
	BwsStr("  0"), BwsStr("LAYER"),             BwsStr("  5"), BwsStr("40"),       BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbLayerTableRecord"),                                 BwsStr("  2"), BwsStr("27"),
	BwsStr(" 70"), BwsStr("    64"),            BwsStr(" 62"), BwsStr("     7"),   BwsStr("  6"), BwsStr("CONTINUOUS"),
	BwsStr("  0"), BwsStr("LAYER"),             BwsStr("  5"), BwsStr("41"),       BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbLayerTableRecord"),                                 BwsStr("  2"), BwsStr("28"),
	BwsStr(" 70"), BwsStr("    64"),            BwsStr(" 62"), BwsStr("     7"),   BwsStr("  6"), BwsStr("CONTINUOUS"),
	BwsStr("  0"), BwsStr("LAYER"),             BwsStr("  5"), BwsStr("42"),       BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbLayerTableRecord"),                                 BwsStr("  2"), BwsStr("29"),
	BwsStr(" 70"), BwsStr("    64"),            BwsStr(" 62"), BwsStr("     7"),   BwsStr("  6"), BwsStr("CONTINUOUS"),
	BwsStr("  0"), BwsStr("LAYER"),             BwsStr("  5"), BwsStr("43"),       BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbLayerTableRecord"),                                 BwsStr("  2"), BwsStr("30"),
	BwsStr(" 70"), BwsStr("    64"),            BwsStr(" 62"), BwsStr("     7"),   BwsStr("  6"), BwsStr("CONTINUOUS"),
	BwsStr("  0"), BwsStr("LAYER"),             BwsStr("  5"), BwsStr("44"),       BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbLayerTableRecord"),                                 BwsStr("  2"), BwsStr("31"),
	BwsStr(" 70"), BwsStr("    64"),            BwsStr(" 62"), BwsStr("     7"),   BwsStr("  6"), BwsStr("CONTINUOUS"),
	BwsStr("$E$N$D$")
};

// ���̏��
static MCHAR FONTSINF[39][STRLNG] =
{
	BwsStr("  0"), BwsStr("ENDTAB"),    BwsStr("  0"), BwsStr("TABLE"),             BwsStr("  2"), BwsStr("STYLE"),
	BwsStr("  5"), BwsStr("3"),         BwsStr("100"), BwsStr("AcDbSymbolTable"),   BwsStr(" 70"), BwsStr("     7"),
	BwsStr("  0"), BwsStr("STYLE"),     BwsStr("  5"), BwsStr("F"),                 BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbTextStyleTableRecord"),                              BwsStr("  2"), BwsStr("STANDARD"),
	BwsStr(" 70"), BwsStr("     0"),    BwsStr(" 40"), BwsStr("0.0"),               BwsStr(" 41"), BwsStr("1.0"),
	BwsStr(" 50"), BwsStr("0.0"),       BwsStr(" 71"), BwsStr("     0"),            BwsStr(" 42"), BwsStr("0.2"),
	BwsStr("  3"), BwsStr("txt"),       BwsStr("  4"), BwsStr("bigfont"),           BwsStr("$E$N$D$")
};

// �X�^�C�����
static MCHAR STYLEINF[33][STRLNG] =
{
	BwsStr("  0"),  BwsStr("STYLE"),       BwsStr("  5"),  BwsStr("hnd[3]"),       BwsStr("100"),  BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"),  BwsStr("AcDbTextStyleTableRecord"),                            BwsStr("  2"),  BwsStr("styl[9]"),
	BwsStr(" 70"),  BwsStr("     0"),      BwsStr(" 40"),  BwsStr("0.0"),          BwsStr(" 41"),  BwsStr("1.0"),
	BwsStr(" 50"),  BwsStr("0.0"),         BwsStr(" 71"),  BwsStr("     0"),       BwsStr(" 42"),  BwsStr("0.2"),
	BwsStr("  3"),  BwsStr("file[23]"),    BwsStr("  4"),  BwsStr(""),             BwsStr("1001"), BwsStr("ACAD"),
	BwsStr("1000"), BwsStr("font[29]"),    BwsStr("1071"), BwsStr("num[31]"),      BwsStr("$E$N$D$")
};

// �u�h�d�v�E�t�b�r�E�`�o�o�h�c�E�c�h�l�r�s�x�k�d�E�a�k�n�b�j�Q�q�d�b�n�q�c
static MCHAR ETCINF[219][STRLNG] =
{
	BwsStr("  0"), BwsStr("ENDTAB"),    BwsStr("  0"), BwsStr("TABLE"),              BwsStr("  2"), BwsStr("VIEW"),
	BwsStr("  5"), BwsStr("6"),         BwsStr("100"), BwsStr("AcDbSymbolTable"),    BwsStr(" 70"), BwsStr("     0"),
	BwsStr("  0"), BwsStr("ENDTAB"),
	BwsStr("  0"), BwsStr("TABLE"),     BwsStr("  2"), BwsStr("UCS"),                BwsStr("  5"), BwsStr("7"),
	BwsStr("100"), BwsStr("AcDbSymbolTable"),                                        BwsStr(" 70"), BwsStr("     0"),
	BwsStr("  0"), BwsStr("ENDTAB"),
	BwsStr("  0"), BwsStr("TABLE"),     BwsStr("  2"), BwsStr("APPID"),              BwsStr("  5"), BwsStr("9"),
	BwsStr("100"), BwsStr("AcDbSymbolTable"),                                        BwsStr(" 70"), BwsStr("     1"),
	BwsStr("  0"), BwsStr("APPID"),     BwsStr("  5"), BwsStr("10"),                 BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbRegAppTableRecord"),                                  BwsStr("  2"), BwsStr("ACAD"),
	BwsStr(" 70"), BwsStr("     0"),    BwsStr("  0"), BwsStr("ENDTAB"),
	BwsStr("  0"), BwsStr("TABLE"),     BwsStr("  2"), BwsStr("DIMSTYLE"),           BwsStr("  5"), BwsStr("A"),
	BwsStr("100"), BwsStr("AcDbSymbolTable"),                                        BwsStr(" 70"), BwsStr("     1"),
	BwsStr("  0"), BwsStr("DIMSTYLE"),  BwsStr("105"), BwsStr("1D"),                 BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbDimStyleTableRecord"),                                BwsStr("  2"), BwsStr("STANDARD"),
	BwsStr(" 70"), BwsStr("     0"),    BwsStr("  3"), BwsStr(""),                   BwsStr("  4"), BwsStr(""),
	BwsStr("  5"), BwsStr(""),          BwsStr("  6"), BwsStr(""),                   BwsStr("  7"), BwsStr(""),
	BwsStr(" 40"), BwsStr("1.0"),       BwsStr(" 41"), BwsStr("0.18"),               BwsStr(" 42"), BwsStr("0.0625"),
	BwsStr(" 43"), BwsStr("0.38"),      BwsStr(" 44"), BwsStr("0.18"),               BwsStr(" 45"), BwsStr("0.0"),
	BwsStr(" 46"), BwsStr("0.0"),       BwsStr(" 47"), BwsStr("0.0"),                BwsStr(" 48"), BwsStr("0.0"),
	BwsStr("140"), BwsStr("0.18"),      BwsStr("141"), BwsStr("0.09"),               BwsStr("142"), BwsStr("0.0"),
	BwsStr("143"), BwsStr("25.4"),      BwsStr("144"), BwsStr("1.0"),                BwsStr("145"), BwsStr("0.0"),
	BwsStr("146"), BwsStr("1.0"),       BwsStr("147"), BwsStr("0.09"),               BwsStr(" 71"), BwsStr("     0"),
	BwsStr(" 72"), BwsStr("     0"),    BwsStr(" 73"), BwsStr("     1"),             BwsStr(" 74"), BwsStr("     1"),
	BwsStr(" 75"), BwsStr("     0"),    BwsStr(" 76"), BwsStr("     0"),             BwsStr(" 77"), BwsStr("     0"),
	BwsStr(" 78"), BwsStr("     0"),    BwsStr("170"), BwsStr("     0"),             BwsStr("171"), BwsStr("     2"),
	BwsStr("172"), BwsStr("     0"),    BwsStr("173"), BwsStr("     0"),             BwsStr("174"), BwsStr("     0"),
	BwsStr("175"), BwsStr("     0"),    BwsStr("176"), BwsStr("     0"),             BwsStr("177"), BwsStr("     0"),
	BwsStr("178"), BwsStr("     0"),    BwsStr("270"), BwsStr("     2"),             BwsStr("271"), BwsStr("     4"),
	BwsStr("272"), BwsStr("     4"),    BwsStr("273"), BwsStr("     2"),             BwsStr("274"), BwsStr("     2"),
	BwsStr("340"), BwsStr("F"),         BwsStr("275"), BwsStr("     0"),             BwsStr("280"), BwsStr("     0"),
	BwsStr("281"), BwsStr("     0"),    BwsStr("282"), BwsStr("     0"),             BwsStr("283"), BwsStr("     1"),
	BwsStr("284"), BwsStr("     0"),    BwsStr("285"), BwsStr("     0"),             BwsStr("286"), BwsStr("     0"),
	BwsStr("287"), BwsStr("     3"),    BwsStr("288"), BwsStr("     0"),             BwsStr("  0"), BwsStr("ENDTAB"),
	BwsStr("  0"), BwsStr("TABLE"),     BwsStr("  2"),  BwsStr("BLOCK_RECORD"),      BwsStr("  5"), BwsStr("1"),
	BwsStr("100"), BwsStr("AcDbSymbolTable"),                                        BwsStr(" 70"), BwsStr("     0"),
	BwsStr("  0"), BwsStr("BLOCK_RECORD"),                                           BwsStr("  5"), BwsStr("18"),
	BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),                                  BwsStr("100"), BwsStr("AcDbBlockTableRecord"),
	BwsStr("  2"), BwsStr("*MODEL_SPACE"),                                           BwsStr("  0"), BwsStr("BLOCK_RECORD"),
	BwsStr("  5"), BwsStr("15"),        BwsStr("100"), BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"), BwsStr("AcDbBlockTableRecord"),                                   BwsStr("  2"), BwsStr("*PAPER_SPACE"),
	BwsStr("  0"), BwsStr("ENDTAB"),    BwsStr("  0"), BwsStr("ENDSEC"),             BwsStr("$E$N$D$")
};

// �u���b�N���
static MCHAR BLOCKINF[77][STRLNG] =
{
	BwsStr("  0"), BwsStr("SECTION"),   BwsStr("  2"), BwsStr("BLOCKS"),           BwsStr("  0"), BwsStr("BLOCK"),
	BwsStr("  5"), BwsStr("19"),        BwsStr("100"), BwsStr("AcDbEnt"),       BwsStr("  8"), BwsStr("0"),
	BwsStr("100"), BwsStr("AcDbBlockBegin"),                                       BwsStr("  2"), BwsStr("*MODEL_SPACE"),
	BwsStr(" 70"), BwsStr("     0"),    BwsStr(" 10"), BwsStr("0.0"),              BwsStr(" 20"), BwsStr("0.0"),
	BwsStr(" 30"), BwsStr("0.0"),       BwsStr("  3"), BwsStr("*MODEL_SPACE"),     BwsStr("  1"), BwsStr(""),
	BwsStr("  0"), BwsStr("ENDBLK"),    BwsStr("  5"), BwsStr("1A"),               BwsStr("100"), BwsStr("AcDbEnt"),
	BwsStr("  8"), BwsStr("0"),         BwsStr("100"), BwsStr("AcDbBlockEnd"),     BwsStr("  0"), BwsStr("BLOCK"),
	BwsStr("  5"), BwsStr("16"),        BwsStr("100"), BwsStr("AcDbEnt"),       BwsStr(" 67"), BwsStr("     1"),
	BwsStr("  8"), BwsStr("0"),         BwsStr("100"), BwsStr("AcDbBlockBegin"),   BwsStr("  2"), BwsStr("*PAPER_SPACE"),
	BwsStr(" 70"), BwsStr("     0"),    BwsStr(" 10"), BwsStr("0.0"),              BwsStr(" 20"), BwsStr("0.0"),
	BwsStr(" 30"), BwsStr("0.0"),       BwsStr("  3"), BwsStr("*PAPER_SPACE"),     BwsStr("  1"), BwsStr(""),
	BwsStr("  0"), BwsStr("ENDBLK"),    BwsStr("  5"), BwsStr("17"),               BwsStr("100"), BwsStr("AcDbEnt"),
	BwsStr("  8"), BwsStr("0"),         BwsStr("100"), BwsStr("AcDbBlockEnd"),     BwsStr("  0"), BwsStr("ENDSEC"),
	BwsStr("$E$N$D$")
};

// �_�\��
static MCHAR DOTINF[17][STRLNG] =
{
	BwsStr("  0"), BwsStr("POINT"),        BwsStr("  5"), BwsStr("hcnt[03]"),      BwsStr("100"), BwsStr("AcDbEnt"),
	BwsStr("  8"), BwsStr("lay[07]"),      BwsStr(" 62"), BwsStr("col[09]"),       BwsStr("100"), BwsStr("AcDbPoint"),
	BwsStr(" 10"), BwsStr("x[13]"),        BwsStr(" 20"), BwsStr("y[15]"),         BwsStr("$E$N$D$")
};

// ���\��
static MCHAR LINEINF[23][STRLNG] =
{
	BwsStr("  0"), BwsStr("LINE"),         BwsStr("  5"), BwsStr("hcnt[03]"),      BwsStr("100"), BwsStr("AcDbEnt"),
	BwsStr("  8"), BwsStr("lay[07]"),      BwsStr("  6"), BwsStr("ltyp[09]"),      BwsStr(" 62"), BwsStr("col[11]"),
	BwsStr("100"), BwsStr("AcDbLine"),     BwsStr(" 10"), BwsStr("sx[15]"),        BwsStr(" 20"), BwsStr("sy[17]"),
	BwsStr(" 11"), BwsStr("ex[19]"),       BwsStr(" 21"), BwsStr("ey[21]"),        BwsStr("$E$N$D$"),
};

// �~�\��
static MCHAR CIRINF[21][STRLNG] =
{
	BwsStr("  0"), BwsStr("CIRCLE"),       BwsStr("  5"), BwsStr("hcnt[03]"),      BwsStr("100"), BwsStr("AcDbEnt"),
	BwsStr("  8"), BwsStr("lay[07]"),      BwsStr("  6"), BwsStr("ltyp[09]"),      BwsStr(" 62"), BwsStr("col[11]"),
	BwsStr("100"), BwsStr("AcDbCircle"),   BwsStr(" 10"), BwsStr("x[15]"),         BwsStr(" 20"), BwsStr("y[17"),
	BwsStr(" 40"), BwsStr("rad[19]"),      BwsStr("$E$N$D$")
};

// �~�ʍ\��
static MCHAR ARCINF[27][STRLNG] =
{
	BwsStr("  0"), BwsStr("ARC"),          BwsStr("  5"), BwsStr("hcnt[03]"),      BwsStr("100"), BwsStr("AcDbEnt"),
	BwsStr("  8"), BwsStr("lay[07]"),      BwsStr("  6"), BwsStr("ltype"),         BwsStr(" 62"), BwsStr("col[11]"),
	BwsStr("100"), BwsStr("AcDbCircle"),   BwsStr(" 10"), BwsStr("x[15]"),         BwsStr(" 20"), BwsStr("y[17]"),
	BwsStr(" 40"), BwsStr("rad[19]"),      BwsStr("100"), BwsStr("AcDbArc"),       BwsStr(" 50"), BwsStr("sa[23]"),
	BwsStr(" 51"), BwsStr("ea[25]"),       BwsStr("$E$N$D$")
};

// �������
static MCHAR TEXTINF[29][STRLNG] =
{
	BwsStr("  0"), BwsStr("TEXT"),         BwsStr("  5"), BwsStr("hcnt[03]"),      BwsStr("100"), BwsStr("AcDbEnt"),
	BwsStr("  8"), BwsStr("lay[07]"),      BwsStr(" 62"), BwsStr("col[09]"),       BwsStr("100"), BwsStr("AcDbText"),
	BwsStr(" 10"), BwsStr("x[13]"),        BwsStr(" 20"), BwsStr("y[15]"),         BwsStr("  1"), BwsStr("str[17]"),
	BwsStr(" 40"), BwsStr("hei[19]"),      BwsStr(" 50"), BwsStr("ang[21]"),       BwsStr(" 51"), BwsStr("ang2[23]"),
	BwsStr("  7"), BwsStr("Sty[25]"),      BwsStr("100"), BwsStr("AcDbText"),      BwsStr("$E$N$D$")
};

// �G���e�B�e�C���
static MCHAR ENTINF[5][STRLNG] =
{
	BwsStr("  0"), BwsStr("SECTION"),      BwsStr("  2"), BwsStr("ENTITIES"),      BwsStr("$E$N$D$")
};

// �d�n�e
static MCHAR EOFINF[3][STRLNG] =
{
	BwsStr("  0"), BwsStr("ENDSEC"),       BwsStr("$E$N$D$")
};

// �F�R�[�h�𓾂�
static HVOID DxfGetCol     (MINT Color1, MINT* Color2);
// ��������햼�𓾂�
static HVOID DxfGetLName   (MINT LType, MCHAR* LName);
// �c�����̊J�n�ʒu�����߂�
static HVOID DxfGetTatePnt (MgPoint2* Pnt1, MFLOAT Height, MFLOAT Angle, MgPoint2* Pnt2);
// �I�v�V�����ɑΉ��������C�����擾
static HVOID DxfGetLayer   (MINT LayOpt, MCHAR* Layer1, MCHAR* Layer2);
// ��������c�w�e�t�@�C���֏o��
static MINT  DxfWrite       (CStdioFile* FLPtr, MCHAR String[][STRLNG]);


/******************************************************************************************************************
[ �@�\ ]  �c�w�e�I�[�v���i�q�P�S�`���j
******************************************************************************************************************/
CStdioFile* MDxfR14Open       (														// (  O) ̧�َ��ʎq
								MCHAR*				FlName,								// (I  ) ̧�ٖ�
								MINT				Option,								// (I  ) ��߼��
								MINT				NumPrim)							// (I  ) ����ò�ސ�
{
	CStdioFile*			FLPtr;															// ̧�َ��ʎq
	MINT				NumHnd;															// ���ِ�
	MINT				ItrSts;
	MINT				ic;


	FLPtr = HsFileOpenS(FlName, 2, 3);													// ̧�ٵ����(÷��)
	if (FLPtr == NULL)  goto  PRG_EXIT;

	NumHnd  = 75;																		// ����ِ������l(10�i) R14J�Ή�
	NumHnd += 2;																		// ���ِ�

	// �w�b�_�E�N���X�E�e�[�u���E�u���b�N�E�I�u�W�F�N�g����ݒ�
	// �ő�����ْl��ݒ�
	NumHnd += NumPrim;																	// ����è�ސ�
	BwsSprintf(HEADER[15], BwsStr("%X"), NumHnd);
	DxfWrite(FLPtr, HEADER);															// ͯ�ޏ��   ������
	DxfWrite(FLPtr, CLASS);																// �׽���    ������
	DxfWrite(FLPtr, VIEWPORT);															// VPORTð��� ������

	// �I�v�V�������Ƃ̐ݒ� ����E���C��
	switch (Option) {
		case  0: 																		// ��߼�ݖ���
			DxfWrite(FLPtr, LTYPEINF);													// ������   ������
			DxfWrite(FLPtr, LAYINF00);													// ڲԏ��    ������
			DxfWrite(FLPtr, LAYINF01);													// ڲԏ��    ������
			break;
		case  1:																		// ��߼�� -l
			DxfWrite(FLPtr, LTYPEINF);													// ������   ������
			DxfWrite(FLPtr, LAYINF33);													// ڲԏ��    ������
			break;
		case  2: 																		// ��߼�� -h
			DxfWrite(FLPtr, LTYPEINF);													// ������   ������
			DxfWrite(FLPtr, LAYINF00);													// ڲԏ��    ������
			DxfWrite(FLPtr, LAYINF01);													// ڲԏ��    ������
			break;
    }

	DxfWrite(FLPtr, FONTSINF);															// ����ð��ُ�����

	// ����ð��ُ�������
	for (ic = 0; ic < 1 ; ic++){
		ItrSts = MDxfR14Style(FLPtr, &NumHnd, BwsStr("�W��"), BwsStr("MS Gothic.ttf"),
							   BwsStr("MS Gothic"), BwsStr("32817"));
	}

	DxfWrite(FLPtr, ETCINF);															// VIEW�UCS�APPID�DIMSTYLE�BLOCK_RECORDð��ُ�����
	DxfWrite(FLPtr, BLOCKINF);															// ��ۯ����  ������
	DxfWrite(FLPtr, ENTINF);															// ENTITES��񏑍���

PRG_EXIT:

	return(FLPtr);
}

/******************************************************************************************************************
[ �@�\ ]  �c�w�e�N���[�Y�i�q�P�S�`���j
******************************************************************************************************************/
HVOID MDxfR14Close            (														// (  O) �ð��   0:���� 1:�װ
								CStdioFile*			FLPtr)								// (I  ) ̧�َ��ʎq
{
	MINT			ItrSts;


	DxfWrite(FLPtr, EOFINF);															// EOF���    ������
	DxfWrite(FLPtr, OBJECT);															// OBJECTS    ������

	ItrSts = HsFileCloseS(FLPtr, 1);													// ̧�ٸ۰��(÷��)

	return;
}

/******************************************************************************************************************
[ �@�\ ]  �X�^�C���̏o��
******************************************************************************************************************/
MINT MDxfR14Style             (														// (  O) �ð��   0:���� 1:�װ 
								CStdioFile*			FLPtr,								// (I  ) ̧�َ��ʎq
								MINT*				NumHnd,								// (I  ) ����ِ�
								MCHAR*				Style,								// (I  ) ����
								MCHAR*				FileN,								// (I  ) ̧�ٖ�
								MCHAR*				FontN,								// (I  ) ̫�Ė�
								MCHAR*				Volue)								// (I  ) ��ح��
{
	MINT			Status;


	BwsSprintf(STYLEINF[ 3], BwsStr("%X"), *NumHnd);									// ����ِ�
	BwsStrCpy (STYLEINF[ 9], Style);													// ����      �W��
	BwsStrCpy (STYLEINF[23], FileN);													// ̧�ٖ�    MS Gothic.ttf
	BwsStrCpy (STYLEINF[29], FontN);													// ̫�Ė�    MS Gothic
	BwsStrCpy (STYLEINF[31], Volue);													// ��ح��    32817

	Status = DxfWrite(FLPtr, STYLEINF);
	(*NumHnd)++;

	return(Status);
}

/******************************************************************************************************************
[ �@�\ ]  �_�̏o��
******************************************************************************************************************/
MINT MDxfR14Dot               (														// (  O) �ð��   0:���� 1:�װ
								CStdioFile*			FLPtr,								// (I  ) ̧�َ��ʎq
								MINT*				NumHnd,								// (I/O) ����ِ�
								MCHAR*				LayNam,								// (I  ) ڲԖ�
								MINT				Color,								// (I  ) �F
								MINT				NumPnt,								// (I  ) ���_��
								MgPoint2*			Pnt1)								// (I  ) �_
{
	MINT			Status;
	MCHAR			Layer[STRLNG];
	MINT			ColorW;
	MINT			ic;


	if (Color == 0) {
		BwsStrCpy(Layer, BwsStr("PEN0"));
	} else {
		BwsStrCpy(Layer, LayNam);
	}
	DxfGetCol(Color, &ColorW);

	BwsStrCpy (DOTINF[ 7], Layer);
	BwsSprintf(DOTINF[ 9], BwsStr("%d"),   ColorW);

	for (ic = 0 ; ic < NumPnt-1 ; ic++) {
		BwsSprintf(DOTINF[ 3], BwsStr("%X"),   *NumHnd);								// ����ِ�
		BwsSprintf(DOTINF[13], BwsStr("%.3f"), Pnt1[ic].X);
		BwsSprintf(DOTINF[15], BwsStr("%.3f"), Pnt1[ic].Y);
		Status = DxfWrite(FLPtr, DOTINF);
		(*NumHnd)++;
	}

	return(Status);
}

/******************************************************************************************************************
[ �@�\ ]  ���̏o��
******************************************************************************************************************/
MINT MDxfR14Line              (														// (  O) �ð��   0:���� 1:�װ
								CStdioFile*			FLPtr,								// (I  ) ̧�َ��ʎq
								MINT*				NumHnd,								// (I/O) ����ِ�
								MCHAR*				LayNam,								// (I  ) ڲԖ�
								MINT				LType,								// (I  ) ����
								MINT				Color,								// (I  ) �F
								MINT				NumPnt,								// (I  ) ���_��
								MgPoint2*			Pnt)								// (I  ) ���_
{
	MINT			Status;
	MCHAR			Layer[STRLNG];
	MINT			ColorW;
	MCHAR			LName[STRLNG];
	MINT			ic;


	if (Color == 0) {
		BwsStrCpy(Layer, BwsStr("PEN0"));
	} else {
		BwsStrCpy(Layer, LayNam);
	}
	DxfGetLName(LType, LName);
	DxfGetCol(Color, &ColorW);

	BwsStrCpy (LINEINF[ 7], Layer);
	BwsStrCpy (LINEINF[ 9], LName);
	BwsSprintf(LINEINF[11], BwsStr("%d"),   ColorW);

	for (ic = 0 ; ic < NumPnt-1 ; ic++) {
		BwsSprintf(LINEINF[ 3], BwsStr("%X"),   *NumHnd);									// ����ِ�
		BwsSprintf(LINEINF[15], BwsStr("%.3f"), Pnt[ic  ].X);
		BwsSprintf(LINEINF[17], BwsStr("%.3f"), Pnt[ic  ].Y);
		BwsSprintf(LINEINF[19], BwsStr("%.3f"), Pnt[ic+1].X);
		BwsSprintf(LINEINF[21], BwsStr("%.3f"), Pnt[ic+1].Y);
		Status = DxfWrite(FLPtr, LINEINF);
		(*NumHnd)++;
	}

	return(Status);
}

/******************************************************************************************************************
[ �@�\ ]  �~�̏o��
******************************************************************************************************************/
MINT MDxfR14Cir               (														// (  O) �ð��   0:���� 1:�װ
								CStdioFile*			FLPtr,								// (I  ) ̧�َ��ʎq
								MINT*				NumHnd,								// (I/O) ����ِ�
								MCHAR*				LayNam,								// (I  ) ڲԖ�
								MINT				LType,								// (I  ) ����
								MINT				Color,								// (I  ) �F
								MgPoint2*			Pnt1,								// (I  ) �O�ڂ��鍶��
								MgPoint2*			Pnt2)								// (I  ) �O�ڂ���E��
{
	MINT			Status;
	MCHAR			Layer[STRLNG];
	MINT			ColorW;
	MCHAR			LName[STRLNG];
	HCIR2			CirW;																// �~


	if (Color == 0) {
		BwsStrCpy(Layer, BwsStr("PEN0"));
	} else {
		BwsStrCpy(Layer, LayNam);
	}
	DxfGetLName(LType, LName);
	DxfGetCol(Color, &ColorW);
	HgTrnCirInf(Pnt1, Pnt2, &CirW);														// ����,�E����~�\���̂�

	BwsSprintf(CIRINF[ 3], BwsStr("%X"),   *NumHnd);									// ����ِ�
	BwsStrCpy (CIRINF[ 7], Layer);
	BwsStrCpy (CIRINF[ 9], LName);
	BwsSprintf(CIRINF[11], BwsStr("%d"),   ColorW);
	BwsSprintf(CIRINF[15], BwsStr("%.3f"), CirW.CPnt.X);
	BwsSprintf(CIRINF[17], BwsStr("%.3f"), CirW.CPnt.Y);
	BwsSprintf(CIRINF[19], BwsStr("%.3f"), CirW.Rad);

	Status = DxfWrite(FLPtr, CIRINF);
	(*NumHnd)++;

	return(Status);
}

/******************************************************************************************************************
[ �@�\ ]  �~�ʂ̏o��
******************************************************************************************************************/
MINT MDxfR14Arc               (														// (  O) �ð��   0:���� 1:�װ
								CStdioFile*			FLPtr,								// (I  ) ̧�َ��ʎq
								MINT*				NumHnd,								// (I/O) ����ِ�
								MCHAR*				LayNam,								// (I  ) ڲԖ�
								MINT				LType,								// (I  ) ����
								MINT				Color,								// (I  ) �F
								MgPoint2*			Pnt1,								// (I  ) �O�ڂ��鍶��
								MgPoint2*			Pnt2,								// (I  ) �O�ڂ���E��
								MgPoint2*			Pnt3,								// (I  ) �`��J�n�_
								MgPoint2*			Pnt4)								// (I  ) �`��I���_
{
	MINT			Status;
	MCHAR			Layer[STRLNG];
	MINT			ColorW;
	MCHAR			LName[STRLNG];
	HXARC2			ArcW;																// �~��


	if (Color == 0) {
		BwsStrCpy(Layer, BwsStr("PEN0"));
	} else {
		BwsStrCpy(Layer, LayNam);
	}
	DxfGetLName(LType, LName);
	DxfGetCol(Color, &ColorW);
	HgTrnArcInf(Pnt1, Pnt2, Pnt3, Pnt4, &ArcW);											// �~�ʍ\���̂�

	BwsSprintf(ARCINF[ 3], BwsStr("%X"),   *NumHnd);									// ����ِ�
	BwsStrCpy (ARCINF[ 7], Layer);
	BwsStrCpy (ARCINF[ 9], LName);
	BwsSprintf(ARCINF[11], BwsStr("%d"),   ColorW);
	BwsSprintf(ARCINF[15], BwsStr("%.3f"), ArcW.CPnt.X);
	BwsSprintf(ARCINF[17], BwsStr("%.3f"), ArcW.CPnt.Y);
	BwsSprintf(ARCINF[19], BwsStr("%.3f"), ArcW.Rad);
	BwsSprintf(ARCINF[23], BwsStr("%.3f"), ArcW.Sta);
	BwsSprintf(ARCINF[25], BwsStr("%.3f"), ArcW.End);

	Status = DxfWrite(FLPtr, ARCINF);
	(*NumHnd)++;

	return(Status);
}

/******************************************************************************************************************
[ �@�\ ]  �����̏o��
******************************************************************************************************************/
MINT MDxfR14Text              (														// (  O) �ð��   0:���� 1:�װ
								CStdioFile*			FLPtr,								// (I  ) ̧�َ��ʎq
								MINT*				NumHnd,								// (I/O) ����ِ�
								MCHAR*				LayNam,								// (I  ) ڲԖ�
								MINT				Color,								// (I  ) �F
								MgPoint2*			Pnt1,								// (I  ) �����z�u�_
								MgPoint2*			Pnt2,								// (I  ) ���������_
								MFLOAT				Height,								// (I  ) ��������
								MCHAR*				String,								// (I  ) ������
								MINT				Italic,								// (I  ) ��د�   0:Off 1:On
								MCHAR*				SName)								// (I  ) ��������
{
	MINT			Status;
	MCHAR			Layer[STRLNG];
	MINT			ColorW;
	MFLOAT			AngleW1;
	MFLOAT			AngleW2;


	if (Color == 0) {
		BwsStrCpy(Layer, BwsStr("PEN0"));
	} else {
		BwsStrCpy(Layer, LayNam);
	}
	DxfGetCol(Color, &ColorW);
	AngleW1 = HgGetAngByPnt(Pnt1, Pnt2);												// �_�P���猩���_�Q�̊p�x
	AngleW2 = 0.f; if (Italic)  AngleW2 = 30.f;

	BwsSprintf(TEXTINF[ 3], BwsStr("%X"),   *NumHnd);									// ����ِ�
	BwsStrCpy (TEXTINF[ 7], Layer);
	BwsSprintf(TEXTINF[ 9], BwsStr("%d"),   ColorW);
	BwsSprintf(TEXTINF[13], BwsStr("%.3f"), Pnt1->X);
	BwsSprintf(TEXTINF[15], BwsStr("%.3f"), Pnt1->Y);
	BwsStrCpy (TEXTINF[17], String);
	BwsSprintf(TEXTINF[19], BwsStr("%.3f"), Height);
	BwsSprintf(TEXTINF[21], BwsStr("%.3f"), AngleW1);
	BwsSprintf(TEXTINF[23], BwsStr("%.3f"), AngleW2);
	BwsStrCpy (TEXTINF[25], SName);

	Status = DxfWrite(FLPtr, TEXTINF);
	(*NumHnd)++;

	return(Status);
}

/******************************************************************************************************************
[ �@�\ ]  �����̏o��
******************************************************************************************************************/
MINT MDxfR14Kanji             (														// (  O) �ð��   0:���� 1:�װ
								CStdioFile*			FLPtr,								// (I  ) ̧�َ��ʎq
								MINT*				NumHnd,								// (I/O) ����ِ�
								MCHAR*				LayNam,								// (I  ) ڲԖ�
								MINT				Color,								// (I  ) �F
								MINT				TFlag,								// (I  ) �c�׸�  0:�� 1:�c
								MgPoint2*			Pnt1,								// (I  ) �����z�u�_
								MgPoint2*			Pnt2,								// (I  ) ���������_
								MFLOAT				Height,								// (I  ) ��������
								MCHAR*				String,								// (I  ) ������
								MINT				Italic,								// (I  ) ��د�   0:Off 1:On
								MCHAR*				SName)								// (I  ) ��������
{
	MINT			Status;
	MCHAR			Layer[STRLNG];
	MINT			ColorW;
	MFLOAT			AngleW1;
	MFLOAT			AngleW2;


	if (Color == 0) {
		BwsStrCpy(Layer, BwsStr("PEN0"));
	} else {
		BwsStrCpy(Layer, LayNam);
	}
	DxfGetCol(Color, &ColorW);
	AngleW1 = HgGetAngByPnt(Pnt1, Pnt2);												// �_�P���猩���_�Q�̊p�x
	AngleW2 = 0.f; if (Italic)  AngleW2 = 30.f;

	BwsSprintf(TEXTINF[ 3], BwsStr("%X"),   *NumHnd);									// ����ِ�
	BwsStrCpy (TEXTINF[ 7], Layer);
	BwsSprintf(TEXTINF[ 9], BwsStr("%d"),   ColorW);
	BwsSprintf(TEXTINF[13], BwsStr("%.3f"), Pnt1->X);
	BwsSprintf(TEXTINF[15], BwsStr("%.3f"), Pnt1->Y);
	BwsStrCpy (TEXTINF[17], String);
	BwsSprintf(TEXTINF[19], BwsStr("%.3f"), Height);
	BwsSprintf(TEXTINF[21], BwsStr("%.3f"), AngleW1);
	BwsSprintf(TEXTINF[23], BwsStr("%.3f"), AngleW2);
	BwsStrCpy (TEXTINF[25], SName);

	Status = DxfWrite(FLPtr, TEXTINF);
	(*NumHnd)++;

	return(Status);
}

/******************************************************************************************************************
[ �@�\ ]  �F�R�[�h�𓾂�
******************************************************************************************************************/
static HVOID DxfGetCol         (
								MINT				Color1,								// (I  ) �F
								MINT*				Color2)								// (  O) �F
{
	switch (Color1) {
		case  1:
			*Color2 = 1;
			break;
		case  2:
			*Color2 = 5;
			break;
		case  3:
			*Color2 = 3;
			break;
		case  4:
			*Color2 = 6;
			break;
		case  5:
			*Color2 = 2;
			break;
		case  6:
			*Color2 = 4;
			break;
		case  7:
			*Color2 = 7;
			break;
		default:
			*Color2 = 7;
	}
}

/******************************************************************************************************************
[ �@�\ ]  ��������햼�𓾂�
******************************************************************************************************************/
static HVOID DxfGetLName       (
								MINT				LType,								// (I  ) ����
								MCHAR*				LName)								// (  O) ���햼
{
	switch (LType) {
		case  HLINE_DASHS:																// �j���i�ׁj                 (2)
		case  HLINE_DASHL:																// �j���i�e�j                 (3)
			BwsStrCpy(LName, BwsStr("HIDDEN"));											// - - - - - - - - - -
			break;
		case  HLINE_DASH1PS:															// �P�_����                   (5)
			BwsStrCpy(LName, BwsStr("CENTER"));											// ----- - ----- - -----
			break;
		case  HLINE_DASH2P:																// �Q�_����                   (7)
			BwsStrCpy(LName, BwsStr("PHANTOM"));										// ----- - - ----- - - -----
			break;
		case  HLINE_DOT:																// �_��                       (4)
			BwsStrCpy(LName, BwsStr("DOT"));											// ....................
			break;
		case  HLINE_DASH1PL:															// �P�_����                   (6)
			BwsStrCpy(LName, BwsStr("CENTERL"));										// ------ - ------ - ------
			break;
		default:
			BwsStrCpy(LName, BwsStr("CONTINUOUS"));										// Solid line
	}
}

/******************************************************************************************************************
[ �@�\ ]  �c�����̊J�n�ʒu�����߂�
******************************************************************************************************************/
static HVOID DxfGetTatePnt     (
								MgPoint2*			Pnt1,								// (I  ) �c�����̊�_
								MFLOAT				Height,								// (I  ) �����̍���
								MFLOAT				Angle,								// (I  ) �����̊p�x
								MgPoint2*			Pnt2)								// (  O) �c�����̊�_
{
	MFLOAT			XyW1[2];
	MFLOAT			XyW2[2];
	HDOUBLE			Work1;
	HDOUBLE			Work2;
	HDOUBLE			Rad1;


	XyW1[0] = Height / 2;
	XyW1[1] = Height;

	Rad1    = (HDOUBLE)Angle / 180.0 * 4.0 * atan(1.0);
	Work1   =  cos(Rad1) * (HDOUBLE)XyW1[0];
	Work2   = -sin(Rad1) * (HDOUBLE)XyW1[1];
	XyW2[0] = (MFLOAT)(Work1 + Work2);
	Work1   =  sin(Rad1) * (HDOUBLE)XyW1[0];
	Work2   =  cos(Rad1) * (HDOUBLE)XyW1[1];
	XyW2[1] = (MFLOAT)(Work1 + Work2);

	Pnt2->X = Pnt1->X + XyW2[0];
	Pnt2->Y = Pnt1->Y + XyW2[1];
}

/******************************************************************************************************************
[ �@�\ ]  �I�v�V�����ɑΉ��������C�����擾
******************************************************************************************************************/
static HVOID DxfGetLayer       (
								MINT				LayOpt,								// (I  ) ڲ԰��߼��
								MCHAR*				Layer1,								//�iI  �jڲԖ�1
								MCHAR*				Layer2)								//�i  O�jڲԖ�2
{
	switch (LayOpt) {																	// ڲ԰�߼��
		case  0:																		// 0�w���
			BwsStrCpy(Layer2, BwsStr("0"));
			break;
		case  1:																		// ���͑w�Ɠ����w
			BwsStrCpy(Layer2, Layer1);
			break;
		default:
			break;
	}
}

/******************************************************************************************************************
[ �@�\ ]  ��������c�w�e�t�@�C���֏o��
******************************************************************************************************************/
static MINT DxfWrite           (
								CStdioFile*			FLPtr,								// (I  ) ̧�َ��ʎq
								MCHAR				String[][STRLNG])					// (I  ) �o�͕�����
{
	MINT				Status;
	MINT				Idx;


	Status = 1;

	Idx    = 0;
	while (1) {
		if (BwsStrCmp(BwsStr("$E$N$D$"), String[Idx]) == 0)  break;

		Status = HsFileWriteS(FLPtr, String[Idx]);
		if (Status != 0)  goto  PRG_EXIT;
		Idx++;
	}
	Status = 0;

PRG_EXIT:

	return(Status);
}

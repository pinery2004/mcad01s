/*********************************************************************************************************************************
              本体  プログラムファイル (HsDHon.cpp)

********************************************************************************************************************************/

#include		"MCad.h"
//#include		"HBase.h"
#include		"MgLib.h"
//#include		"HsFileLib.h"
#include		"MDxfR14.h"


#define			 STRLNG			64														// 文字列長さ

// ヘッダー
static MCHAR HEADER[19][STRLNG] =
{
	BwsStr("  0"),    BwsStr("SECTION"),   BwsStr("  2"), BwsStr("HEADER"),        BwsStr("  9"),  BwsStr("$ACADVER"),
	BwsStr("  1"),    BwsStr("AC1014"),    BwsStr("  9"), BwsStr("$HANDLING"),     BwsStr("  70"), BwsStr("     1"),
	BwsStr("  9"),    BwsStr("$HANDSEED"), BwsStr("  5"), BwsStr("hcnt[15]"),      BwsStr("  0"),  BwsStr("ENDSEC"),
	BwsStr("$E$N$D$")
};

// クラス
static MCHAR CLASS[7][STRLNG] =
{
	BwsStr("  0"),   BwsStr("SECTION"),    BwsStr("  2"), BwsStr("CLASSES"),       BwsStr("  0"),  BwsStr("ENDSEC"),
	BwsStr("$E$N$D$")
};

// オブジェクト
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

// ビューポート
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


// 線種情報
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

// レイヤ０情報
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

// レイヤ情報
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

// 字体情報
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

// スタイル情報
static MCHAR STYLEINF[33][STRLNG] =
{
	BwsStr("  0"),  BwsStr("STYLE"),       BwsStr("  5"),  BwsStr("hnd[3]"),       BwsStr("100"),  BwsStr("AcDbSymbolTableRecord"),
	BwsStr("100"),  BwsStr("AcDbTextStyleTableRecord"),                            BwsStr("  2"),  BwsStr("styl[9]"),
	BwsStr(" 70"),  BwsStr("     0"),      BwsStr(" 40"),  BwsStr("0.0"),          BwsStr(" 41"),  BwsStr("1.0"),
	BwsStr(" 50"),  BwsStr("0.0"),         BwsStr(" 71"),  BwsStr("     0"),       BwsStr(" 42"),  BwsStr("0.2"),
	BwsStr("  3"),  BwsStr("file[23]"),    BwsStr("  4"),  BwsStr(""),             BwsStr("1001"), BwsStr("ACAD"),
	BwsStr("1000"), BwsStr("font[29]"),    BwsStr("1071"), BwsStr("num[31]"),      BwsStr("$E$N$D$")
};

// ＶＩＥＷ・ＵＣＳ・ＡＰＰＩＤ・ＤＩＭＳＴＹＬＥ・ＢＬＯＣＫ＿ＲＥＣＯＲＤ
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

// ブロック情報
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

// 点構造
static MCHAR DOTINF[17][STRLNG] =
{
	BwsStr("  0"), BwsStr("POINT"),        BwsStr("  5"), BwsStr("hcnt[03]"),      BwsStr("100"), BwsStr("AcDbEnt"),
	BwsStr("  8"), BwsStr("lay[07]"),      BwsStr(" 62"), BwsStr("col[09]"),       BwsStr("100"), BwsStr("AcDbPoint"),
	BwsStr(" 10"), BwsStr("x[13]"),        BwsStr(" 20"), BwsStr("y[15]"),         BwsStr("$E$N$D$")
};

// 線構造
static MCHAR LINEINF[23][STRLNG] =
{
	BwsStr("  0"), BwsStr("LINE"),         BwsStr("  5"), BwsStr("hcnt[03]"),      BwsStr("100"), BwsStr("AcDbEnt"),
	BwsStr("  8"), BwsStr("lay[07]"),      BwsStr("  6"), BwsStr("ltyp[09]"),      BwsStr(" 62"), BwsStr("col[11]"),
	BwsStr("100"), BwsStr("AcDbLine"),     BwsStr(" 10"), BwsStr("sx[15]"),        BwsStr(" 20"), BwsStr("sy[17]"),
	BwsStr(" 11"), BwsStr("ex[19]"),       BwsStr(" 21"), BwsStr("ey[21]"),        BwsStr("$E$N$D$"),
};

// 円構造
static MCHAR CIRINF[21][STRLNG] =
{
	BwsStr("  0"), BwsStr("CIRCLE"),       BwsStr("  5"), BwsStr("hcnt[03]"),      BwsStr("100"), BwsStr("AcDbEnt"),
	BwsStr("  8"), BwsStr("lay[07]"),      BwsStr("  6"), BwsStr("ltyp[09]"),      BwsStr(" 62"), BwsStr("col[11]"),
	BwsStr("100"), BwsStr("AcDbCircle"),   BwsStr(" 10"), BwsStr("x[15]"),         BwsStr(" 20"), BwsStr("y[17"),
	BwsStr(" 40"), BwsStr("rad[19]"),      BwsStr("$E$N$D$")
};

// 円弧構造
static MCHAR ARCINF[27][STRLNG] =
{
	BwsStr("  0"), BwsStr("ARC"),          BwsStr("  5"), BwsStr("hcnt[03]"),      BwsStr("100"), BwsStr("AcDbEnt"),
	BwsStr("  8"), BwsStr("lay[07]"),      BwsStr("  6"), BwsStr("ltype"),         BwsStr(" 62"), BwsStr("col[11]"),
	BwsStr("100"), BwsStr("AcDbCircle"),   BwsStr(" 10"), BwsStr("x[15]"),         BwsStr(" 20"), BwsStr("y[17]"),
	BwsStr(" 40"), BwsStr("rad[19]"),      BwsStr("100"), BwsStr("AcDbArc"),       BwsStr(" 50"), BwsStr("sa[23]"),
	BwsStr(" 51"), BwsStr("ea[25]"),       BwsStr("$E$N$D$")
};

// 文字情報
static MCHAR TEXTINF[29][STRLNG] =
{
	BwsStr("  0"), BwsStr("TEXT"),         BwsStr("  5"), BwsStr("hcnt[03]"),      BwsStr("100"), BwsStr("AcDbEnt"),
	BwsStr("  8"), BwsStr("lay[07]"),      BwsStr(" 62"), BwsStr("col[09]"),       BwsStr("100"), BwsStr("AcDbText"),
	BwsStr(" 10"), BwsStr("x[13]"),        BwsStr(" 20"), BwsStr("y[15]"),         BwsStr("  1"), BwsStr("str[17]"),
	BwsStr(" 40"), BwsStr("hei[19]"),      BwsStr(" 50"), BwsStr("ang[21]"),       BwsStr(" 51"), BwsStr("ang2[23]"),
	BwsStr("  7"), BwsStr("Sty[25]"),      BwsStr("100"), BwsStr("AcDbText"),      BwsStr("$E$N$D$")
};

// エンティテイ情報
static MCHAR ENTINF[5][STRLNG] =
{
	BwsStr("  0"), BwsStr("SECTION"),      BwsStr("  2"), BwsStr("ENTITIES"),      BwsStr("$E$N$D$")
};

// ＥＯＦ
static MCHAR EOFINF[3][STRLNG] =
{
	BwsStr("  0"), BwsStr("ENDSEC"),       BwsStr("$E$N$D$")
};

// 色コードを得る
static HVOID DxfGetCol     (MINT Color1, MINT* Color2);
// 線種より線種名を得る
static HVOID DxfGetLName   (MINT LType, MCHAR* LName);
// 縦漢字の開始位置を求める
static HVOID DxfGetTatePnt (MgPoint2* Pnt1, MFLOAT Height, MFLOAT Angle, MgPoint2* Pnt2);
// オプションに対応したレイヤを取得
static HVOID DxfGetLayer   (MINT LayOpt, MCHAR* Layer1, MCHAR* Layer2);
// 文字列をＤＸＦファイルへ出力
static MINT  DxfWrite       (CStdioFile* FLPtr, MCHAR String[][STRLNG]);


/******************************************************************************************************************
[ 機能 ]  ＤＸＦオープン（Ｒ１４形式）
******************************************************************************************************************/
CStdioFile* MDxfR14Open       (														// (  O) ﾌｧｲﾙ識別子
								MCHAR*				FlName,								// (I  ) ﾌｧｲﾙ名
								MINT				Option,								// (I  ) ｵﾌﾟｼｮﾝ
								MINT				NumPrim)							// (I  ) ﾌﾟﾘﾐﾃｲﾌﾞ数
{
	CStdioFile*			FLPtr;															// ﾌｧｲﾙ識別子
	MINT				NumHnd;															// ｽﾀｲﾙ数
	MINT				ItrSts;
	MINT				ic;


	FLPtr = HsFileOpenS(FlName, 2, 3);													// ﾌｧｲﾙｵｰﾌﾟﾝ(ﾃｷｽﾄ)
	if (FLPtr == NULL)  goto  PRG_EXIT;

	NumHnd  = 75;																		// ﾊﾝﾄﾞﾙ数初期値(10進) R14J対応
	NumHnd += 2;																		// ｽﾀｲﾙ数

	// ヘッダ・クラス・テーブル・ブロック・オブジェクト情報を設定
	// 最大ﾊﾝﾄﾞﾙ値を設定
	NumHnd += NumPrim;																	// ﾌﾟﾘﾐﾃｨﾌﾞ数
	BwsSprintf(HEADER[15], BwsStr("%X"), NumHnd);
	DxfWrite(FLPtr, HEADER);															// ﾍｯﾀﾞ情報   書込み
	DxfWrite(FLPtr, CLASS);																// ｸﾗｽ情報    書込み
	DxfWrite(FLPtr, VIEWPORT);															// VPORTﾃｰﾌﾞﾙ 書込み

	// オプションごとの設定 線種・レイヤ
	switch (Option) {
		case  0: 																		// ｵﾌﾟｼｮﾝ無し
			DxfWrite(FLPtr, LTYPEINF);													// 線種情報   書込み
			DxfWrite(FLPtr, LAYINF00);													// ﾚｲﾔ情報    書込み
			DxfWrite(FLPtr, LAYINF01);													// ﾚｲﾔ情報    書込み
			break;
		case  1:																		// ｵﾌﾟｼｮﾝ -l
			DxfWrite(FLPtr, LTYPEINF);													// 線種情報   書込み
			DxfWrite(FLPtr, LAYINF33);													// ﾚｲﾔ情報    書込み
			break;
		case  2: 																		// ｵﾌﾟｼｮﾝ -h
			DxfWrite(FLPtr, LTYPEINF);													// 線種情報   書込み
			DxfWrite(FLPtr, LAYINF00);													// ﾚｲﾔ情報    書込み
			DxfWrite(FLPtr, LAYINF01);													// ﾚｲﾔ情報    書込み
			break;
    }

	DxfWrite(FLPtr, FONTSINF);															// 字体ﾃｰﾌﾞﾙ書込み

	// ｽﾀｲﾙﾃｰﾌﾞﾙ書き込み
	for (ic = 0; ic < 1 ; ic++){
		ItrSts = MDxfR14Style(FLPtr, &NumHnd, BwsStr("標準"), BwsStr("MS Gothic.ttf"),
							   BwsStr("MS Gothic"), BwsStr("32817"));
	}

	DxfWrite(FLPtr, ETCINF);															// VIEW･UCS･APPID･DIMSTYLE･BLOCK_RECORDﾃｰﾌﾞﾙ書込み
	DxfWrite(FLPtr, BLOCKINF);															// ﾌﾞﾛｯｸ情報  書込み
	DxfWrite(FLPtr, ENTINF);															// ENTITES情報書込み

PRG_EXIT:

	return(FLPtr);
}

/******************************************************************************************************************
[ 機能 ]  ＤＸＦクローズ（Ｒ１４形式）
******************************************************************************************************************/
HVOID MDxfR14Close            (														// (  O) ｽﾃｰﾀｽ   0:正常 1:ｴﾗｰ
								CStdioFile*			FLPtr)								// (I  ) ﾌｧｲﾙ識別子
{
	MINT			ItrSts;


	DxfWrite(FLPtr, EOFINF);															// EOF情報    書込み
	DxfWrite(FLPtr, OBJECT);															// OBJECTS    書込み

	ItrSts = HsFileCloseS(FLPtr, 1);													// ﾌｧｲﾙｸﾛｰｽﾞ(ﾃｷｽﾄ)

	return;
}

/******************************************************************************************************************
[ 機能 ]  スタイルの出力
******************************************************************************************************************/
MINT MDxfR14Style             (														// (  O) ｽﾃｰﾀｽ   0:正常 1:ｴﾗｰ 
								CStdioFile*			FLPtr,								// (I  ) ﾌｧｲﾙ識別子
								MINT*				NumHnd,								// (I  ) ﾊﾝﾄﾞﾙ数
								MCHAR*				Style,								// (I  ) ｽﾀｲﾙ
								MCHAR*				FileN,								// (I  ) ﾌｧｲﾙ名
								MCHAR*				FontN,								// (I  ) ﾌｫﾝﾄ名
								MCHAR*				Volue)								// (I  ) ﾎﾞﾘｭｰﾑ
{
	MINT			Status;


	BwsSprintf(STYLEINF[ 3], BwsStr("%X"), *NumHnd);									// ﾊﾝﾄﾞﾙ数
	BwsStrCpy (STYLEINF[ 9], Style);													// ｽﾀｲﾙ      標準
	BwsStrCpy (STYLEINF[23], FileN);													// ﾌｧｲﾙ名    MS Gothic.ttf
	BwsStrCpy (STYLEINF[29], FontN);													// ﾌｫﾝﾄ名    MS Gothic
	BwsStrCpy (STYLEINF[31], Volue);													// ﾎﾞﾘｭｰﾑ    32817

	Status = DxfWrite(FLPtr, STYLEINF);
	(*NumHnd)++;

	return(Status);
}

/******************************************************************************************************************
[ 機能 ]  点の出力
******************************************************************************************************************/
MINT MDxfR14Dot               (														// (  O) ｽﾃｰﾀｽ   0:正常 1:ｴﾗｰ
								CStdioFile*			FLPtr,								// (I  ) ﾌｧｲﾙ識別子
								MINT*				NumHnd,								// (I/O) ﾊﾝﾄﾞﾙ数
								MCHAR*				LayNam,								// (I  ) ﾚｲﾔ名
								MINT				Color,								// (I  ) 色
								MINT				NumPnt,								// (I  ) 頂点数
								MgPoint2*			Pnt1)								// (I  ) 点
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
		BwsSprintf(DOTINF[ 3], BwsStr("%X"),   *NumHnd);								// ﾊﾝﾄﾞﾙ数
		BwsSprintf(DOTINF[13], BwsStr("%.3f"), Pnt1[ic].X);
		BwsSprintf(DOTINF[15], BwsStr("%.3f"), Pnt1[ic].Y);
		Status = DxfWrite(FLPtr, DOTINF);
		(*NumHnd)++;
	}

	return(Status);
}

/******************************************************************************************************************
[ 機能 ]  線の出力
******************************************************************************************************************/
MINT MDxfR14Line              (														// (  O) ｽﾃｰﾀｽ   0:正常 1:ｴﾗｰ
								CStdioFile*			FLPtr,								// (I  ) ﾌｧｲﾙ識別子
								MINT*				NumHnd,								// (I/O) ﾊﾝﾄﾞﾙ数
								MCHAR*				LayNam,								// (I  ) ﾚｲﾔ名
								MINT				LType,								// (I  ) 線種
								MINT				Color,								// (I  ) 色
								MINT				NumPnt,								// (I  ) 頂点数
								MgPoint2*			Pnt)								// (I  ) 頂点
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
		BwsSprintf(LINEINF[ 3], BwsStr("%X"),   *NumHnd);									// ﾊﾝﾄﾞﾙ数
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
[ 機能 ]  円の出力
******************************************************************************************************************/
MINT MDxfR14Cir               (														// (  O) ｽﾃｰﾀｽ   0:正常 1:ｴﾗｰ
								CStdioFile*			FLPtr,								// (I  ) ﾌｧｲﾙ識別子
								MINT*				NumHnd,								// (I/O) ﾊﾝﾄﾞﾙ数
								MCHAR*				LayNam,								// (I  ) ﾚｲﾔ名
								MINT				LType,								// (I  ) 線種
								MINT				Color,								// (I  ) 色
								MgPoint2*			Pnt1,								// (I  ) 外接する左上
								MgPoint2*			Pnt2)								// (I  ) 外接する右下
{
	MINT			Status;
	MCHAR			Layer[STRLNG];
	MINT			ColorW;
	MCHAR			LName[STRLNG];
	HCIR2			CirW;																// 円


	if (Color == 0) {
		BwsStrCpy(Layer, BwsStr("PEN0"));
	} else {
		BwsStrCpy(Layer, LayNam);
	}
	DxfGetLName(LType, LName);
	DxfGetCol(Color, &ColorW);
	HgTrnCirInf(Pnt1, Pnt2, &CirW);														// 左下,右上より円構造体へ

	BwsSprintf(CIRINF[ 3], BwsStr("%X"),   *NumHnd);									// ﾊﾝﾄﾞﾙ数
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
[ 機能 ]  円弧の出力
******************************************************************************************************************/
MINT MDxfR14Arc               (														// (  O) ｽﾃｰﾀｽ   0:正常 1:ｴﾗｰ
								CStdioFile*			FLPtr,								// (I  ) ﾌｧｲﾙ識別子
								MINT*				NumHnd,								// (I/O) ﾊﾝﾄﾞﾙ数
								MCHAR*				LayNam,								// (I  ) ﾚｲﾔ名
								MINT				LType,								// (I  ) 線種
								MINT				Color,								// (I  ) 色
								MgPoint2*			Pnt1,								// (I  ) 外接する左上
								MgPoint2*			Pnt2,								// (I  ) 外接する右下
								MgPoint2*			Pnt3,								// (I  ) 描画開始点
								MgPoint2*			Pnt4)								// (I  ) 描画終了点
{
	MINT			Status;
	MCHAR			Layer[STRLNG];
	MINT			ColorW;
	MCHAR			LName[STRLNG];
	HXARC2			ArcW;																// 円弧


	if (Color == 0) {
		BwsStrCpy(Layer, BwsStr("PEN0"));
	} else {
		BwsStrCpy(Layer, LayNam);
	}
	DxfGetLName(LType, LName);
	DxfGetCol(Color, &ColorW);
	HgTrnArcInf(Pnt1, Pnt2, Pnt3, Pnt4, &ArcW);											// 円弧構造体へ

	BwsSprintf(ARCINF[ 3], BwsStr("%X"),   *NumHnd);									// ﾊﾝﾄﾞﾙ数
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
[ 機能 ]  文字の出力
******************************************************************************************************************/
MINT MDxfR14Text              (														// (  O) ｽﾃｰﾀｽ   0:正常 1:ｴﾗｰ
								CStdioFile*			FLPtr,								// (I  ) ﾌｧｲﾙ識別子
								MINT*				NumHnd,								// (I/O) ﾊﾝﾄﾞﾙ数
								MCHAR*				LayNam,								// (I  ) ﾚｲﾔ名
								MINT				Color,								// (I  ) 色
								MgPoint2*			Pnt1,								// (I  ) 文字配置点
								MgPoint2*			Pnt2,								// (I  ) 文字方向点
								MFLOAT				Height,								// (I  ) 文字高さ
								MCHAR*				String,								// (I  ) 文字列
								MINT				Italic,								// (I  ) ｲﾀﾘｯｸ   0:Off 1:On
								MCHAR*				SName)								// (I  ) 文字ﾀｲﾌﾟ
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
	AngleW1 = HgGetAngByPnt(Pnt1, Pnt2);												// 点１から見た点２の角度
	AngleW2 = 0.f; if (Italic)  AngleW2 = 30.f;

	BwsSprintf(TEXTINF[ 3], BwsStr("%X"),   *NumHnd);									// ﾊﾝﾄﾞﾙ数
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
[ 機能 ]  漢字の出力
******************************************************************************************************************/
MINT MDxfR14Kanji             (														// (  O) ｽﾃｰﾀｽ   0:正常 1:ｴﾗｰ
								CStdioFile*			FLPtr,								// (I  ) ﾌｧｲﾙ識別子
								MINT*				NumHnd,								// (I/O) ﾊﾝﾄﾞﾙ数
								MCHAR*				LayNam,								// (I  ) ﾚｲﾔ名
								MINT				Color,								// (I  ) 色
								MINT				TFlag,								// (I  ) 縦ﾌﾗｸﾞ  0:横 1:縦
								MgPoint2*			Pnt1,								// (I  ) 文字配置点
								MgPoint2*			Pnt2,								// (I  ) 文字方向点
								MFLOAT				Height,								// (I  ) 文字高さ
								MCHAR*				String,								// (I  ) 文字列
								MINT				Italic,								// (I  ) ｲﾀﾘｯｸ   0:Off 1:On
								MCHAR*				SName)								// (I  ) 文字ﾀｲﾌﾟ
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
	AngleW1 = HgGetAngByPnt(Pnt1, Pnt2);												// 点１から見た点２の角度
	AngleW2 = 0.f; if (Italic)  AngleW2 = 30.f;

	BwsSprintf(TEXTINF[ 3], BwsStr("%X"),   *NumHnd);									// ﾊﾝﾄﾞﾙ数
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
[ 機能 ]  色コードを得る
******************************************************************************************************************/
static HVOID DxfGetCol         (
								MINT				Color1,								// (I  ) 色
								MINT*				Color2)								// (  O) 色
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
[ 機能 ]  線種より線種名を得る
******************************************************************************************************************/
static HVOID DxfGetLName       (
								MINT				LType,								// (I  ) 線種
								MCHAR*				LName)								// (  O) 線種名
{
	switch (LType) {
		case  HLINE_DASHS:																// 破線（細）                 (2)
		case  HLINE_DASHL:																// 破線（粗）                 (3)
			BwsStrCpy(LName, BwsStr("HIDDEN"));											// - - - - - - - - - -
			break;
		case  HLINE_DASH1PS:															// １点鎖線                   (5)
			BwsStrCpy(LName, BwsStr("CENTER"));											// ----- - ----- - -----
			break;
		case  HLINE_DASH2P:																// ２点鎖線                   (7)
			BwsStrCpy(LName, BwsStr("PHANTOM"));										// ----- - - ----- - - -----
			break;
		case  HLINE_DOT:																// 点線                       (4)
			BwsStrCpy(LName, BwsStr("DOT"));											// ....................
			break;
		case  HLINE_DASH1PL:															// １点鎖線                   (6)
			BwsStrCpy(LName, BwsStr("CENTERL"));										// ------ - ------ - ------
			break;
		default:
			BwsStrCpy(LName, BwsStr("CONTINUOUS"));										// Solid line
	}
}

/******************************************************************************************************************
[ 機能 ]  縦漢字の開始位置を求める
******************************************************************************************************************/
static HVOID DxfGetTatePnt     (
								MgPoint2*			Pnt1,								// (I  ) 縦漢字の基準点
								MFLOAT				Height,								// (I  ) 漢字の高さ
								MFLOAT				Angle,								// (I  ) 漢字の角度
								MgPoint2*			Pnt2)								// (  O) 縦漢字の基準点
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
[ 機能 ]  オプションに対応したレイヤを取得
******************************************************************************************************************/
static HVOID DxfGetLayer       (
								MINT				LayOpt,								// (I  ) ﾚｲﾔｰｵﾌﾟｼｮﾝ
								MCHAR*				Layer1,								//（I  ）ﾚｲﾔ名1
								MCHAR*				Layer2)								//（  O）ﾚｲﾔ名2
{
	switch (LayOpt) {																	// ﾚｲﾔｰﾌﾟｼｮﾝ
		case  0:																		// 0層をｾｯﾄ
			BwsStrCpy(Layer2, BwsStr("0"));
			break;
		case  1:																		// 入力層と同じ層
			BwsStrCpy(Layer2, Layer1);
			break;
		default:
			break;
	}
}

/******************************************************************************************************************
[ 機能 ]  文字列をＤＸＦファイルへ出力
******************************************************************************************************************/
static MINT DxfWrite           (
								CStdioFile*			FLPtr,								// (I  ) ﾌｧｲﾙ識別子
								MCHAR				String[][STRLNG])					// (I  ) 出力文字列
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

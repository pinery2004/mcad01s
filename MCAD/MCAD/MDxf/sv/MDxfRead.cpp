/******************************************************************************************************************
              �c�w�e  �v���O�����t�@�C�� (MDxf.cpp)

[�@�\] �c�w�e�t�@�C���Ǎ���
[�Ԓl] = 0 ����  < 0�G���[
******************************************************************************************************************/


#include		"MCad.h"
#include		"MgLib.h"
//#include		"HsFileLib.h"
#include		"MDxf.h"


MDxfInf* MDxfRead            (														// (  O) DXF���
								MCHAR*				FlName,								// (I  ) ̧�ٖ�
								MINT				VewTyp,								// (I  ) �ޭ����
								MINT				ZukVec,								// (I  ) 1:�㉺ 2:���E 4:-90��5:+90��
								MFLOAT				MVal,								// (I  ) �ړ���
								MINT				FAFlag)								// (I  ) �ʂ̌�������   0:���Ȃ�  1:����
{
	MDxfInf*			DxfInf;															// DXF���
	CStdioFile*			FLPtr;															// ̧�َ��ʎq
	MINT				Code;
	MCHAR				Data[512];
	MgMinMax3			Window;															// ����޳����
	MgMinMax3			ViewPort;														// �ޭ��߰�
	HMTRX3				Matrix;															// ��]��د��
	HMTRX3				Matrix2;														// ��]��د��
	MgPoint3			Move;															// �ړ���
	MgMinMax3			MinMax;
	MINT				ItrSts;															// �ð��


	DxfInf = NULL;

	FLPtr = HsFileOpenS(FlName, 1, 2);													// ̧�ٵ����(÷��)
	if (FLPtr == NULL)  goto  PRG_EXIT;

	Window.Min.X   = Window.Min.Y   = Window.Min.Z   = 0.0f;
	Window.Max.X   = Window.Max.Y   = Window.Max.Z   = 0.0f;
	ViewPort.Min.X = ViewPort.Min.Y = ViewPort.Min.Z = 0.0f;
	ViewPort.Max.X = ViewPort.Max.Y = ViewPort.Max.Z = 0.0f;

	DxfInf = new MDxfInf(VewTyp, 1.0f, -1, 1.0f, 0, &Window, &ViewPort, FAFlag);		// DXF���ݽ�׸�
	DxfInf->AddLayer(BwsStr("0"), 0, HLINE_SOLD);										// ڲ�"0"�ǉ�  �F:��  ���ԍ�:����

	while (MDxfGetLine(FLPtr, &Code, Data) == 0) {										// �Ǎ���
		if (BwsStrCmp(Data, BwsStr("SECTION")) == 0) {
			if (MDxfGetLine(FLPtr, &Code, Data) != 0) goto  PRG_ERROR;
			if (Code != 2)  goto  PRG_ERROR;

			// �Z�N�V�����̐U�蕪��
			if (BwsStrCmp(Data, BwsStr("HEADER")) == 0) {
				ItrSts = MDxfGetHdSec(FLPtr, DxfInf);
				if (ItrSts != 0)  goto  PRG_ERROR;
			} else if (BwsStrCmp(Data, BwsStr("TABLES")) == 0) {
				ItrSts = MDxfGetTblSec(FLPtr, DxfInf);
				if (ItrSts != 0)  goto  PRG_ERROR;
			} else if (BwsStrCmp(Data, BwsStr("BLOCKS")) == 0) {
				ItrSts = MDxfGetBlcSec(FLPtr, DxfInf);
				if (ItrSts != 0)  goto  PRG_ERROR;
			} else if (BwsStrCmp(Data, BwsStr("ENTITIES")) == 0) {
				ItrSts = MDxfGetEntSec(FLPtr, DxfInf);
				if (ItrSts != 0)  goto  PRG_ERROR;
			} else if (BwsStrCmp(Data, BwsStr("CLASSES")) == 0) {
				ItrSts = MDxfGetClsSec(FLPtr);
				if (ItrSts != 0)  goto  PRG_ERROR;
			} else if (BwsStrCmp(Data, BwsStr("OBJECTS")) == 0) {
				ItrSts = MDxfGetObjSec(FLPtr);
				if (ItrSts != 0)  goto  PRG_ERROR;
			} else {
				goto  PRG_ERROR;
			}
		} else if (BwsStrCmp(Data, BwsStr("EOF")) == 0) {								// DXF̧�ِ���ɏI��
			ItrSts = 0;
			break;
		} else {																		// �װ
			if (ItrSts != 0)  goto  PRG_ERROR;
			break;
		}
	}

	if (VewTyp >= HVIEWT_SYO && VewTyp <= HVIEWT_RSK)  ItrSts = DxfInf->TransformYZ();	// ���ʂ�Y��Z������

	Move.X = Move.Y = Move.Z = 0.f;
	if (ZukVec == 1) {
		HgUDMirMtxt(Matrix);															// �㉺���]
		ItrSts = DxfInf->Transform(1, Matrix);											// �ړ����]
	} else if (ZukVec == 2) {															// ���E�Ώ�
		HgUDMirMtxt(Matrix);															// �㉺���]
		HgGetMtxByAng(180.f, Matrix2);
		HgMultiMtx(Matrix, Matrix2);													// ��د������د�����|����
		ItrSts = DxfInf->Transform(1, Matrix);											// �ړ����]
	} else if (ZukVec == 3) {															// 180����]
		HgGetMtxByAng(180.f, Matrix);
		ItrSts = DxfInf->Transform(0, Matrix);											// �ړ����]
	} else if (ZukVec == 4) {															// -90����]
		HgGetMtxByAng(-90.f, Matrix);
		ItrSts = DxfInf->Transform(0, Matrix);											// �ړ����]
	} else if (ZukVec == 5) {															// 90����]
		HgGetMtxByAng(+90.f, Matrix);
		ItrSts = DxfInf->Transform(0, Matrix);											// �ړ����]
	} else if (ZukVec == 6) {															// X�ړ�
		Move.X = -MVal;
		ItrSts = DxfInf->Transform(0, NULL, &Move);										// �ړ����]
	} else if (ZukVec == 7) {															// Y�ړ�
		Move.Y = -MVal;
		ItrSts = DxfInf->Transform(0, NULL, &Move);										// �ړ����]
	} else if (ZukVec == 8) {															// Z�ړ�
		Move.Z = -MVal;
		ItrSts = DxfInf->Transform(0, NULL, &Move);										// �ړ����]
	} else if (ZukVec == 9) {															// ���S�␳
		DxfInf->GetMinMax(0, NULL, NULL, NULL, &MinMax);
		Move.X = +(MinMax.Max.X - MinMax.Min.X) / 2.f;
		Move.Y = -(MinMax.Max.Y - MinMax.Min.Y) / 2.f;
//		Move.Z = +(MinMax.Max.Z - MinMax.Min.Z) / 2.f;
		ItrSts = DxfInf->Transform(0, NULL, &Move);										// �ړ�
	} else if (ZukVec == 12) {															// ���E�Ώ�&���S�␳
		DxfInf->GetMinMax(0, NULL, NULL, NULL, &MinMax);
		HgUDMirMtxt(Matrix);															// �㉺���]
		HgGetMtxByAng(180.f, Matrix2);
		HgMultiMtx(Matrix, Matrix2);													// ��د������د�����|����
		Move.X = +(MinMax.Max.X - MinMax.Min.X) / 2.f;
		Move.Y = +(MinMax.Max.Y - MinMax.Min.Y) / 2.f;
		Move.Z = +(MinMax.Max.Z - MinMax.Min.Z) / 2.f;
		ItrSts = DxfInf->Transform(1, Matrix, &Move);									// �ړ����]
	} else if (ZukVec == 15) {															// 90����]&���S�␳
		DxfInf->GetMinMax(0, NULL, NULL, NULL, &MinMax);
		HgGetMtxByAng(+90.f, Matrix);
		Move.X = +(MinMax.Max.Y - MinMax.Min.Y) / 2.f;
		Move.Y = -(MinMax.Max.X - MinMax.Min.X) / 2.f;
		Move.Z = +(MinMax.Max.Z - MinMax.Min.Z) / 2.f;
		ItrSts = DxfInf->Transform(0, Matrix, &Move);									// �ړ����]
	} else {
		ItrSts = DxfInf->Transform();													// �ŏ��ő徯�
	}

PRG_ERROR:

	ItrSts = HsFileCloseS(FLPtr, 0);													// ̧�ٸ۰��(÷��)

PRG_EXIT:

	return(DxfInf);
}

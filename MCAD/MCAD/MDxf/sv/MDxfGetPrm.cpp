/******************************************************************************************************************
              �c�w�e  �v���O�����t�@�C�� (MDxf.cpp)

[�@�\] �~�ʏ���ǂݍ��݁A���C���������̓u���b�N���̃v���~�e�B�u���X�g�֒ǉ�����
[�Ԓl] = 0 ����  < 0�G���[  > 0 �x�� ��\�����ڂł���
******************************************************************************************************************/

#include		"HEtcLib.h"
#include		"MgLib.h"
#include		"MDxf.h"


// �Ǎ��ݒ��_�f�[�^�^�C�v
enum {
	POLYVER,
	POLYMESH
};

// �Ǎ��ݗp���_�f�[�^
typedef struct _hxvertex {
	MINT		Type;																	// �Ǎ��ݒ��_�ް�����
	union {
		struct {
			MFLOAT		Pnt[3];															// ���_
			MINT		dmy;
			MFLOAT		Tan;															// �ӂ���݁itan)
		};
		struct {
			MINT		VerNo[4];														// ���_�ԍ�
			MINT		dmy;
		};
	};
} HXVERTEX;


MINT MDxfGetPrmArc        (															// (  O) �ð��
							CStdioFile*			FLPtr,									// (I  ) ̧�َ��ʎq
							MCHAR*				Data,									// (  O) �Ǎ��ݗ̈�
							MDxfInf*			DxfInf,									// (I/O) DXF���
							MDxfBlock*			Block)									// (I  ) ��ۯ�
																						//       == NULL ڲԂ֓Ǎ���
																						//       != NULL �w����ۯ��֓Ǎ���
{
	MINT			Status;																// �ð��
	MINT			Code;
	MINT			PrmType;															// ����è������
	MINT			Color;																// �F�ԍ�  �i�ԍ��j
	MINT			LineNo;																// ����ԍ��i�ԍ��j
	MINT			LineWd;																// �����ԍ��i�ԍ��j
	HXPRIM			Prim;
	MCHAR			LyName[NAMMAX];														// ڲԖ�
	MCHAR			LnName[NAMMAX];														// ���햼
	HBOOL			LyrOK;																// ڲԏ�񂪂�������
	MDxfLayer*		LayPtr;
	MDxfLines*		LinPtr;
	MDxfPrim*		PrmPtr;
	HXARC3			ArcW1;																// �~��
	MINT			LColor;																// �F�ԍ�  �i�ԍ��j
	MINT			LLineNo;															// ����ԍ��i�ԍ��j


	// �����ݒ�
	LyrOK   = HFALSE;																	// ڲԖ���`�Ȃ�
	PrmType = HPRIM_ARC;																// ����  �F�~��
	Color   = 256;																		// �F�ԍ��F
	LineWd  = 3;																		// �������F�א��Œ�
	BwsStrCpy(LnName, BwsStr("BYLAYER"));												// ���햼�F
	ZeroMemory(&Prim, sizeof(HXPRIM));													// �ر
	ArcW1.CPnt.X = ArcW1.CPnt.Y = ArcW1.CPnt.Z = 0.f;

	// �O���[�v�R�[�h�O���o�ꂷ��܂œǂݍ���ŉ~�ʃf�[�^�쐬
	while  ((Status = MDxfGetLine(FLPtr, &Code, Data)) == 0) {
		if (Code == 6) {																// ���햼
			BwsSscanf(Data, BwsStr("%s"), LnName);
		} else if (Code == 8) {															// ڲԖ�
			LyrOK = HTRUE;
			BwsSscanf(Data, BwsStr("%s"), LyName);
		} else if (Code == 10) {														// ���S�_X
			BwsSscanf(Data, BwsStr("%f"), &ArcW1.CPnt.X);
		} else if (Code == 20) {														// ���S�_Y
			BwsSscanf(Data, BwsStr("%f"), &ArcW1.CPnt.Y);
		} else if (Code == 30) {														// ���S�_Z
			BwsSscanf(Data, BwsStr("%f"), &ArcW1.CPnt.Z);
		} else if (Code == 40) {														// ���a
			BwsSscanf(Data, BwsStr("%f"), &ArcW1.Rad);
		} else if (Code == 50) {														// �J�n�p�x
			BwsSscanf(Data, BwsStr("%f"), &ArcW1.Sta);
		} else if (Code == 51) {														// �I���p�x
			BwsSscanf(Data, BwsStr("%f"), &ArcW1.End);
		} else if (Code == 62) {														// �F�ԍ�
			BwsSscanf(Data, BwsStr("%d"), &Color);
		} else if (Code == 0) {															// ��؂蕶��
			if (LyrOK != HTRUE) {														// ڲԖ�������`
				Status = -1;
				goto  PRG_EXIT;
			}
			break;
		}
	}

	HgTrnArcInf(HPLANXY, &ArcW1,&Prim.Arc.Pnt1, &Prim.Arc.Pnt2, &Prim.Arc.Pnt3, &Prim.Arc.Pnt4);	// �~�ʏ���ϊ�
	LayPtr = DxfInf->GetLayer(LyName, &LColor, &LLineNo);								// ڲԏ�񌟍�
	if (LayPtr == NULL)  goto  PRG_EXIT;

	// �F���`�F�b�N
	if (Color == 0) {																	// BYBLOCK
		// �}�����Ɍ���
	} else if (Color == 256) {															// BYLAYER
		Color = LColor;
	} else if (Color < 0) {																// ��wOFF
		Status = 1;																		// ��\�����ڂ͓Ǎ��܂Ȃ�
		goto  PRG_EXIT;
	}

	// �����`�F�b�N
	if (BwsStrCmp(LnName, BwsStr("BYLAYER")) == 0) {
		LineNo = LLineNo;																// �����̎w��@���C��
	} else {
		LinPtr = DxfInf->GetLines(LnName, &LineNo);
//		if (LinPtr == 0)  goto  PRG_EXIT;												// �Ƃ肠����  CONTINUOUS
		if (LinPtr == 0)  LineNo = LLineNo;;											// �Ƃ肠����
	}

	// �������X�g�֒ǉ�
	if (Block == NULL) {																// ڲԂ֒ǉ�
		PrmPtr = LayPtr->AddPrim(PrmType, Color, LineNo, LineWd, &Prim);				// �}�`���ݽ�׸�
	} else {																			// ��ۯ��֒ǉ�
		PrmPtr = Block->AddPrim(PrmType, Color, LineNo, LineWd, &Prim);					// �}�`���ݽ�׸�
	}

PRG_EXIT:

	return(Status);
}
/******************************************************************************************************************
[�@�\] ��������Ǎ��݁A���C���������̓u���b�N���̃v���~�e�B�u���X�g�֒ǉ�����
[�Ԓl] = 0 ����  < 0�G���[  > 0 �x�� ��\�����ڂł���
******************************************************************************************************************/
MINT MDxfGetPrmAtb        (															// (  O) �ð��
							CStdioFile*			FLPtr,									// (I  ) ̧�َ��ʎq
							MCHAR*				Data,									// (  O) �Ǎ��ݗ̈�
							MDxfInf*			DxfInf,									// (I/O) DXF���
							MDxfBlock*			Block)									// (I  ) ��ۯ�
																						//       == NULL ڲԂ֓Ǎ���
																						//       != NULL �w����ۯ��֓Ǎ���
{
	MINT			Status;																// �ð��
	MINT			Code;
	MINT			PrmType;															// ����è������
	MINT			Color;																// �F�ԍ�  �i�ԍ��j
	HXPRIM			Prim;
	MCHAR			FnName[NAMMAX];														// ̫�Ė�
	MCHAR			LyName[NAMMAX];														// ڲԖ�
	MCHAR			LnName[NAMMAX];														// ���햼
	HBOOL			LyrOK;																// ڲԏ�񂪂�������
	MDxfLayer*		LayPtr;																// ڲԏ���߲��
	MDxfFonts*		FntPtr;																// ̫�ď���߲��
	MDxfPrim*		PrmPtr;
	MINT			LColor;																// �F�ԍ�  �i�ԍ��j
	MINT			FCode;																// ̫�Ĕԍ��i�ԍ��j
	MINT			AtrFlg;																// �����t���O
	MINT			TxtLen;																// �����񒷂�


	// �����ݒ�
	LyrOK   = HFALSE;																	// ڲԖ���`�Ȃ�
	PrmType = HPRIM_TXT;																// ����  �F����
	Color   = 256;																		// �F�ԍ��F
	BwsStrCpy(FnName, BwsStr("STANDARD"));												// ̫�Ė��FSTANDARD
	BwsStrCpy(LnName, BwsStr("BYLAYER"));												// ���햼�F
	ZeroMemory(&Prim, sizeof(HXPRIM));													// �ر
	AtrFlg  = 0;																		// �����t���O�F0

	while  ((Status = MDxfGetLine(FLPtr, &Code, Data)) == 0) {
		if (Code == 6)	{																// ���햼
			BwsSscanf(Data, BwsStr("%s"), LnName);
		} else if (Code == 7) {															// �������ٖ�
			BwsSscanf(Data, BwsStr("%s"), FnName);
		} else if (Code == 8) {															// ڲԖ�
			LyrOK = HTRUE;
			BwsSscanf(Data, BwsStr("%s"), LyName);
		} else if (Code == 1) {															// ������
			TxtLen = BwsStrLen(Data);
			if ((Prim.Text.String = (MCHAR *)malloc(sizeof(MCHAR) * (TxtLen+1))) == NULL)  HMFAIL;
			Prim.Text.Num = TxtLen;														// ������
			BwsStrCpy(Prim.Text.String, Data);
		} else if (Code == 10) {														// �J�n�_X
			BwsSscanf(Data, BwsStr("%f"), &Prim.Text.Pnt.X);
		} else if (Code == 20) {														// �J�n�_Y
			BwsSscanf(Data, BwsStr("%f"), &Prim.Text.Pnt.Y);
		} else if (Code == 30) {														// �J�n�_Z
			BwsSscanf(Data, BwsStr("%f"), &Prim.Text.Pnt.Z);
		} else if (Code == 40) {														// ��������
			BwsSscanf(Data, BwsStr("%f"), &Prim.Text.Hei);
		} else if (Code == 50 ) {														// ��]�p�x
			BwsSscanf(Data, BwsStr("%f"), &Prim.Text.Ang);
		} else if (Code == 62) {														// �F�ԍ�
			BwsSscanf(Data, BwsStr("%d"), &Color);
		} else if (Code == 70) {														// �����t���O
			BwsSscanf(Data, BwsStr("%d"), &AtrFlg);
		} else if (Code == 0) {															// ��؂蕶��
			if (LyrOK != HTRUE) {														// ڲԖ�������`
				Status = -1;
				goto  PRG_EXIT;
			}
			break;
		}
	}

	if ((AtrFlg & 1) == 1)  goto  PRG_EXIT;												// �����r�b�g�P�͔�\��

	LayPtr = DxfInf->GetLayer(LyName, &LColor);											// ڲԏ�񌟍�
	if (LayPtr == NULL)  goto  PRG_EXIT;

	// �F���`�F�b�N
	if (Color == 0) {																	// BYBLOCK
		// �}�����Ɍ���
	} else if (Color == 256) {															// BYLAYER
		Color = LColor;
	} else if (Color < 0) {																// ��wOFF
		Status = 1;																		// ��\�����ڂ͓Ǎ��܂Ȃ�
		goto  PRG_EXIT;
	}

	FntPtr = DxfInf->GetFonts(FnName, &FCode);											// ̫�ď�񌟍�
	if (FntPtr == NULL)  goto  PRG_EXIT;

	if ((FCode & 4) == 4) {																// ���������S(�c)���ޯĂ������Ă���
		Prim.Text.Msc = -1.0f;
	} else {
		Prim.Text.Msc =  1.0f;
	}

	// �������X�g�֒ǉ�
	if (Block == NULL) {																// ڲԂ֒ǉ�
		PrmPtr = LayPtr->AddPrim(PrmType, Color, 0, 3, &Prim);							// �}�`���ݽ�׸�
	} else {																			// ��ۯ��֒ǉ�
		PrmPtr = Block->AddPrim(PrmType, Color, 0, 3, &Prim);							// �}�`���ݽ�׸�
	}

PRG_EXIT:

	if (Prim.Text.String)  free(Prim.Text.String);

	return(Status);
}
/******************************************************************************************************************
[�@�\] �~����Ǎ��݁A���C���������̓u���b�N���̃v���~�e�B�u���X�g�֒ǉ�����
[�Ԓl] = 0 ����  < 0�G���[  > 0 �x�� ��\�����ڂł���
******************************************************************************************************************/
MINT MDxfGetPrmCir        (															// (  O) �ð��
							CStdioFile*			FLPtr,									// (I  ) ̧�َ��ʎq
							MCHAR*				Data,									// (  O) �Ǎ��ݗ̈�
							MDxfInf*			DxfInf,									// (I/O) DXF���
							MDxfBlock*			Block)									// (I  ) ��ۯ�
																						//       == NULL ڲԂ֓Ǎ���
																						//       != NULL �w����ۯ��֓Ǎ���
{
	MINT			Status;																// �ð��
	MINT			Code;
	MINT			PrmType;															// ����è������
	MINT			Color;																// �F�ԍ�  �i�ԍ��j
	MINT			LineNo;																// ����ԍ��i�ԍ��j
	MINT			LineWd;																// �����ԍ��i�ԍ��j
	HXPRIM			Prim;
	MCHAR			LyName[NAMMAX];														// ڲԖ�
	MCHAR			LnName[NAMMAX];														// ���햼
	HBOOL			LyrOK;																// ڲԏ�񂪂�������
	MDxfLayer*		LayPtr;
	MDxfLines*		LinPtr;
	MDxfPrim*		PrmPtr;
	MgPoint3			CPnt;
	MFLOAT			Radius;
	MINT			LColor;																// �F�ԍ�  �i�ԍ��j
	MINT			LLineNo;															// ����ԍ��i�ԍ��j


	// �����ݒ�
	LyrOK   = HFALSE;																	// ڲԖ���`�Ȃ�
	PrmType = HPRIM_CIR;																// ����  �F�~
	Color   = 256;																		// �F�ԍ��F
	LineWd  = 3;																		// �������F�א��Œ�
	BwsStrCpy(LnName, BwsStr("BYLAYER"));												// ���햼�F
	ZeroMemory(&Prim, sizeof(HXPRIM));													// �ر
	CPnt.X = CPnt.Y = CPnt.Z = 0.f;

	// �O���[�v�R�[�h�O���o�ꂷ��܂œǂݍ��݉~�f�[�^�쐬
	while  ((Status = MDxfGetLine(FLPtr, &Code, Data)) == 0) {
		if (Code == 6) {																// ���햼
			BwsSscanf(Data, BwsStr("%s"), LnName);
		} else if (Code == 8) {															// ڲԖ�
			LyrOK = HTRUE;
			BwsSscanf(Data, BwsStr("%s"), LyName);
		} else if (Code == 10) {														// ���S�_X
			BwsSscanf(Data, BwsStr("%f"), &CPnt.X);
		} else if (Code == 20) {														// ���S�_Y
			BwsSscanf(Data, BwsStr("%f"), &CPnt.Y);
		} else if (Code == 30) {														// ���S�_Z
			BwsSscanf(Data, BwsStr("%f"), &CPnt.Z);
		} else if (Code == 40) {														// ���a
			BwsSscanf(Data, BwsStr("%f"), &Radius);
		} else if (Code == 62) {														// �F�ԍ�
			BwsSscanf(Data, BwsStr("%d"), &Color);
		} else if (Code == 0) {															// ��؂蕶��
			if (LyrOK != HTRUE) {														// ڲԖ�������`
				Status = -1;
				goto  PRG_EXIT;
			}
			break;
		}
	}

	HgTrnCirInf(HPLANXY, &CPnt, Radius, &Prim.Circle.Pnt1, &Prim.Circle.Pnt2);			// �~����ϊ�

	LayPtr = DxfInf->GetLayer(LyName, &LColor, &LLineNo);								// ڲԏ�񌟍�
	if (LayPtr == NULL)  goto  PRG_EXIT;

	// �F���`�F�b�N
	if (Color == 0) {																	// BYBLOCK
		// �}�����Ɍ���
	} else if (Color == 256) {															// BYLAYER
		Color = LColor;
	} else if (Color < 0) {																// ��wOFF
		Status = 1;																		// ��\�����ڂ͓Ǎ��܂Ȃ�
		goto  PRG_EXIT;
	}

	// �����`�F�b�N
	if (BwsStrCmp(LnName, BwsStr("BYLAYER")) == 0) {
		LineNo = LLineNo;																// �����̎w��@���C��
	} else {
		LinPtr = DxfInf->GetLines(LnName, &LineNo);
//		if (LinPtr == 0)  goto  PRG_EXIT;												// �Ƃ肠����  CONTINUOUS
		if (LinPtr == 0)  LineNo = LLineNo;;											// �Ƃ肠����
	}

	// �������X�g�֒ǉ�
	if (Block == NULL) {																// ڲԂ֒ǉ�
		PrmPtr = LayPtr->AddPrim(PrmType, Color, LineNo, LineWd, &Prim);				// �}�`���ݽ�׸�
	} else {																			// ��ۯ��֒ǉ�
		PrmPtr = Block->AddPrim(PrmType, Color, LineNo, LineWd, &Prim);					// �}�`���ݽ�׸�
	}

PRG_EXIT:

	return(Status);
}
/******************************************************************************************************************
[�@�\] �_����Ǎ��݁A���C���������̓u���b�N���̃v���~�e�B�u���X�g�֒ǉ�����
[�Ԓl] = 0 ����  < 0�G���[  > 0 �x�� ��\�����ڂł���
******************************************************************************************************************/
MINT MDxfGetPrmDot        (															// (  O) �ð��
							CStdioFile*			FLPtr,									// (I  ) ̧�َ��ʎq
							MCHAR*				Data,									// (  O) �Ǎ��ݗ̈�
							MDxfInf*			DxfInf,									// (I/O) DXF���
							MDxfBlock*			Block)									// (I  ) ��ۯ�
																						//       == NULL ڲԂ֓Ǎ���
																						//       != NULL �w����ۯ��֓Ǎ���
{
	MINT			Status;																// �ð��
	MINT			Code;
	MINT			PrmType;															// ����è������
	MINT			Color;																// �F�ԍ�  �i�ԍ��j
	HXPRIM			Prim;
	MCHAR			LyName[NAMMAX];														// ڲԖ�
	MCHAR			LnName[NAMMAX];														// ���햼
	HBOOL			LyrOK;																// ڲԏ�񂪂�������
	MDxfLayer*		LayPtr;
	MDxfPrim*		PrmPtr;
	MINT			LColor;																// �F�ԍ�  �i�ԍ��j


	// �����ݒ�
	LyrOK   = HFALSE;																	// ڲԖ���`�Ȃ�
	PrmType = HPRIM_DOT;																// ����  �F��
	Color   = 256;																		// �F�ԍ��F
	BwsStrCpy(LnName, BwsStr("BYLAYER"));												// ���햼�F
	ZeroMemory(&Prim, sizeof(HXPRIM));													// �ر

	// �O���[�v�R�[�h�O���o�ꂷ��܂œǂݍ���Ńf�[�^�쐬
	while  ((Status = MDxfGetLine(FLPtr, &Code, Data)) == 0) {
		if (Code == 6) {																// ���햼
			BwsSscanf(Data, BwsStr("%s"), LnName);
		} else if (Code == 8) {															// ڲԖ�
			LyrOK = HTRUE;
			BwsSscanf(Data, BwsStr("%s"), LyName );
		} else if (Code == 10) {														// �_X
			BwsSscanf(Data, BwsStr("%f"), &Prim.Dot.X);
		} else if (Code == 20) {														// �_Y
			BwsSscanf(Data, BwsStr("%f"), &Prim.Dot.Y);
		} else if (Code == 30)	{														// �_Z
			BwsSscanf(Data, BwsStr("%f"), &Prim.Dot.Z);
		} else if (Code == 62) {														// �F�ԍ�
			BwsSscanf(Data, BwsStr("%d"), &Color);
		} else if (Code == 0) {															// ��؂蕶���i�I���j
			if (LyrOK != HTRUE) {														// ڲԖ�������`
				Status = -1;
				goto  PRG_EXIT;
			}
			break;
		}
	}

	LayPtr = DxfInf->GetLayer(LyName, &LColor);											// ڲԏ�񌟍�
	if (LayPtr == NULL)  goto  PRG_EXIT;

	// �F���`�F�b�N
	if (Color == 0) {																	// BYBLOCK
		// �}�����Ɍ���
	} else if (Color == 256) {															// BYLAYER
		Color = LColor;
	} else if (Color < 0) {																// ��wOFF
		Status = 1;																		// ��\�����ڂ͓Ǎ��܂Ȃ�
		goto  PRG_EXIT;
	}

	// �������X�g�֒ǉ�
	if (Block == NULL) {																// ڲԂ֒ǉ�
		PrmPtr = LayPtr->AddPrim(PrmType, Color, 0, 3, &Prim);							// �}�`���ݽ�׸�
	} else {																			// ��ۯ��֒ǉ�
		PrmPtr = Block->AddPrim(PrmType, Color, 0, 3, &Prim);							// �}�`���ݽ�׸�
	}

PRG_EXIT:

	return(Status);
}

/******************************************************************************************************************
[�@�\] �ʏ���Ǎ��݁A���C���������̓u���b�N���̃v���~�e�B�u���X�g�֒ǉ�����
[�Ԓl] = 0 ����  < 0�G���[  > 0 �x�� ��\�����ڂł���
******************************************************************************************************************/
MINT MDxfGetPrmFac        (															// (  O) �ð��
							CStdioFile*			FLPtr,									// (I  ) ̧�َ��ʎq
							MCHAR*				Data,									// (  O) �Ǎ��ݗ̈�
							MDxfInf*			DxfInf,									// (I/O) DXF���
							MDxfBlock*			Block)									// (I  ) ��ۯ�
																						//       == NULL ڲԂ֓Ǎ���
																						//       != NULL �w����ۯ��֓Ǎ���
{
	MINT			Status;																// �ð��
	MINT			Code;
	MINT			PrmType;															// ����è������
	MINT			Color;																// �F�ԍ�  �i�ԍ��j
	MINT			LineNo;																// ����ԍ��i�ԍ��j
	MINT			LineWd;																// �����ԍ��i�ԍ��j
	HXPRIM			Prim;
	MCHAR			LyName[NAMMAX];														// ڲԖ�
	MCHAR			LnName[NAMMAX];														// ���햼
	HBOOL			LyrOK;																// ڲԏ�񂪂�������
	MDxfLayer*		LayPtr;
	MDxfLines*		LinPtr;
	MDxfPrim*		PrmPtr;
	MINT			LColor;																// �F�ԍ�  �i�ԍ��j
	MINT			LLineNo;															// ����ԍ��i�ԍ��j


	// �����ݒ�
	LyrOK   = HFALSE;																	// ڲԖ���`�Ȃ�
	PrmType = HPRIM_POL;																// ����  �F��
	Color   = 256;																		// �F�ԍ��F
	LineWd  = 3;																		// �������F�א��Œ�
	BwsStrCpy(LnName, BwsStr("BYLAYER"));												// ���햼�F
	ZeroMemory(&Prim, sizeof(HXPRIM));													// �ر

	Prim.Face.CFlag = 1;																// �n�I�_ 1:����
	// �O���[�v�R�[�h�O�܂œǂݍ���Ńf�[�^�쐬
	while  ((Status = MDxfGetLine(FLPtr, &Code, Data)) == 0) {
		if (Code == 6) {																// ���햼
			BwsSscanf(Data, BwsStr("%s"), LnName);
		} else if (Code == 8) {															// ڲԖ�
			LyrOK = HTRUE;
			BwsSscanf(Data, BwsStr("%s"), LyName);
		} else if (Code == 10) {														// �_�PX
			if (Prim.Face.NumPnt < 1)  Prim.Face.NumPnt = 1;
			BwsSscanf(Data, BwsStr("%f"), &Prim.Face.Pnt[0].X);
		} else if (Code == 20) {														// �_�PY
			BwsSscanf(Data, BwsStr("%f"), &Prim.Face.Pnt[0].Y);
		} else if (Code == 30) {														// �_�PZ
			BwsSscanf(Data, BwsStr("%f"), &Prim.Face.Pnt[0].Z);
		} else if (Code == 11) {														// �_�QX
			if (Prim.Face.NumPnt < 2)  Prim.Face.NumPnt = 2;
			BwsSscanf(Data, BwsStr("%f"), &Prim.Face.Pnt[1].X);
		} else if (Code == 21) {														// �_�QY
			BwsSscanf(Data, BwsStr("%f"), &Prim.Face.Pnt[1].Y);
		} else if (Code == 31) {														// �_�QZ
			BwsSscanf(Data, BwsStr("%f"), &Prim.Face.Pnt[1].Z);
		} else if (Code == 12) {														// �_�RX
			if (Prim.Face.NumPnt < 3)  Prim.Face.NumPnt = 3;
			BwsSscanf(Data, BwsStr("%f"), &Prim.Face.Pnt[2].X);
		} else if (Code == 22) {														// �_�RY
			BwsSscanf(Data, BwsStr("%f"), &Prim.Face.Pnt[2].Y );
		} else if (Code == 32)	{														// �_�RZ
			BwsSscanf(Data, BwsStr("%f"), &Prim.Face.Pnt[2].Z );
		} else if (Code == 13)	{														// �_�SX
			if (Prim.Face.NumPnt < 4)  Prim.Face.NumPnt = 4;
			BwsSscanf(Data, BwsStr("%f"), &Prim.Face.Pnt[3].X );
		} else if (Code == 23)	{														// �_�SY
			BwsSscanf(Data, BwsStr("%f"), &Prim.Face.Pnt[3].Y );
		} else if (Code == 33) {														// �_�SZ
			BwsSscanf(Data, BwsStr("%f"), &Prim.Face.Pnt[3].Z );
		} else if (Code == 62) {														// �F�ԍ�
			BwsSscanf(Data, BwsStr("%d"), &Color);
		} else if (Code == 70) {														// �n�I�_
			BwsSscanf(Data, BwsStr("%d"), &Prim.Face.CFlag);
		} else if (Code == 0) {															// ��؂蕶��
			if (LyrOK != HTRUE) {														// ڲԖ�������`
				Status = -1;
				goto  PRG_EXIT;
			}
			break;
		}
	}

	LayPtr = DxfInf->GetLayer(LyName, &LColor, &LLineNo);								// ڲԏ�񌟍�
	if (LayPtr == NULL)  goto  PRG_EXIT;

	// �F���`�F�b�N
	if (Color == 0) {																	// BYBLOCK
		// �}�����Ɍ���
	} else if (Color == 256) {															// BYLAYER
		Color = LColor;
	} else if (Color < 0) {																// ��wOFF
		Status = 1;																		// ��\�����ڂ͓Ǎ��܂Ȃ�
		goto  PRG_EXIT;
	}

	// �����`�F�b�N
	if (BwsStrCmp(LnName, BwsStr("BYLAYER")) == 0) {
		LineNo = LLineNo;																// �����̎w��@���C��
	} else {
		LinPtr = DxfInf->GetLines(LnName, &LineNo);
		if (LinPtr == NULL)  goto  PRG_EXIT;
	}

	// �������X�g�֒ǉ�
	if (Block == NULL) {																// ڲԂ֒ǉ�
		PrmPtr = LayPtr->AddPrim(PrmType, Color, LineNo, LineWd, &Prim);				// �}�`���ݽ�׸�
	} else {																			// ��ۯ��֒ǉ�
		PrmPtr = Block->AddPrim(PrmType, Color, LineNo, LineWd, &Prim);					// �}�`���ݽ�׸�
	}

PRG_EXIT:

	return(Status);
}
/******************************************************************************************************************
[�@�\] �u���b�N��}������
[�Ԓl] = 0 ����  < 0�G���[  > 0 �x�� ��\�����ڂł���
******************************************************************************************************************/
MINT MDxfGetPrmIns        (															// (  O) �ð��
							CStdioFile*			FLPtr,									// (I  ) ̧�َ��ʎq
							MCHAR*				Data,									// (  O) �Ǎ��ݗ̈�
							MDxfInf*			DxfInf,									// (I/O) DXF���
							MDxfBlock*			Block)									// (I  ) ��ۯ�
																						//       == NULL ڲԂ֓Ǎ���
																						//       != NULL �w����ۯ��֓Ǎ���
{
	MINT			Status;																// �ð��
	MINT			Code;
	MINT			Color;																// �F�ԍ�  �i�ԍ��j
	MINT			LineNo;																// ����ԍ��i�ԍ��j
	MCHAR			BlName[NAMMAX];														// ��ۯ���
	MCHAR			LyName[NAMMAX];														// ڲԖ�
	HBOOL			LyrOK;																// ڲԏ�񂪂�������
	MDxfBlock*		BlkPtr;
	MDxfLayer*		LayPtr;
	MDxfPrim*		PrmPtr;
	MINT			LColor;																// �F�ԍ�  �i�ԍ��j
	MINT			LLineNo;															// ����ԍ��i�ԍ��j
	MgPoint3			InsPnt;																// �}���_
	MgPoint3			InsScl;																// �ړx
	MFLOAT			Rotate;																// ��]�p�x
	MgPoint3			BasPnt;																// ��_
	MINT			ic;


	// �����ݒ�
	LyrOK    = HFALSE;																	// ڲԖ���`�Ȃ�
	InsPnt.X = InsPnt.Y = InsPnt.Z = 0.0f;												// �}���_��̫��0.0
	InsScl.X = InsScl.Y = InsScl.Z = 1.0f;												// �{����̫��1.0
	Rotate   = 0.0f;																	// ��]�p�x��̫��0.0

	// �O���[�v�R�[�h�O���o�ꂷ��܂œǂݍ��݁A�f�[�^�쐬
	while  ((Status = MDxfGetLine(FLPtr, &Code, Data)) == 0) {
		if (Code == 2) {																// �}����ۯ���
			BwsSscanf(Data, BwsStr("%s"), BlName);
		} else if (Code == 8) {															// ڲԖ�
			LyrOK = HTRUE;
			BwsSscanf(Data, BwsStr("%s"), LyName);
		} else if (Code == 10) {														// �}���_X
			BwsSscanf(Data, BwsStr("%f"), &InsPnt.X);
		} else if (Code == 20) {														// �}���_Y
			BwsSscanf(Data, BwsStr("%f"), &InsPnt.Y);
		} else if (Code == 30) {														// �}���_Z
			BwsSscanf(Data, BwsStr("%f"), &InsPnt.Z);
		} else if (Code == 41) {														// X�ړx
			BwsSscanf(Data, BwsStr("%f"), &InsScl.X);
		} else if (Code == 42) {														// Y�ړx
			BwsSscanf(Data, BwsStr("%f"), &InsScl.Y);
		} else if (Code == 43) {														// Z�ړx
			BwsSscanf(Data, BwsStr("%f"), &InsScl.Z);
		} else if (Code == 50) {														// ��]�p�x
			BwsSscanf(Data, BwsStr("%f"), &Rotate);
		} else if (Code == 0) {															// ��؂蕶��
			if (LyrOK != HTRUE) {														// ڲԖ�������`
				Status = -1;
				goto  PRG_EXIT;
			}
			break;
		}
	}

	// �}����̐F�Ɛ��ԍ��𓾂�
	if (Block == NULL) {																// ڲԂ֒ǉ�
		LayPtr = DxfInf->GetLayer(LyName, &LColor, &LLineNo);							// ڲԏ�񌟍�
		if (LayPtr == NULL)  goto  PRG_EXIT;
	} else {																			// ��ۯ��֒ǉ�
		Color  = Block->m_Color;
		LineNo = Block->m_LineNo;
	}

	BlkPtr = DxfInf->GetBlock(BlName, NULL, NULL, NULL, &BasPnt);						// ��ۯ���񌟍�
	if (BlkPtr == NULL)  goto  PRG_EXIT;

	if (BlkPtr->m_PrimList != NULL) {
		for (ic = 0 ; ic < (MINT)BlkPtr->m_PrimList->Number() ; ic++) {					// �}�`���
			PrmPtr = (MDxfPrim *)BlkPtr->m_PrimList->Access(ic);						// �߲���𓾂�

			// �������X�g�֒ǉ�
			if (Block == NULL) {														// ڲԂ֒ǉ�
				PrmPtr = LayPtr->AddPrim(PrmPtr->m_PrmType, PrmPtr->m_Color,			// �}�`���ݽ�׸�
										 PrmPtr->m_LineNo,  PrmPtr->m_LineWd, &PrmPtr->m_Prim);
			} else {																	// ��ۯ��֒ǉ�
				PrmPtr = Block->AddPrim(PrmPtr->m_PrmType, PrmPtr->m_Color,				// �}�`���ݽ�׸�
										PrmPtr->m_LineNo,  PrmPtr->m_LineWd, &PrmPtr->m_Prim);
			}
			PrmPtr->Transform(Color, &BasPnt, &InsPnt, &InsScl, Rotate);				// �}�`�̕ό`
		}
	}

PRG_EXIT:

	return(Status);
}
/******************************************************************************************************************
[�@�\] ������ǂݍ��݁A���C���������̓u���b�N���̃v���~�e�B�u���X�g�֒ǉ�����
[�Ԓl] = 0 ����  < 0�G���[  > 0 �x�� ��\�����ڂł���
******************************************************************************************************************/
MINT MDxfGetPrmLin        (															// (  O) �ð��
							CStdioFile*			FLPtr,									// (I  ) ̧�َ��ʎq
							MCHAR*				Data,									// (  O) �Ǎ��ݗ̈�
							MDxfInf*			DxfInf,									// (I/O) DXF���
							MDxfBlock*			Block)									// (I  ) ��ۯ�
																						//       == NULL ڲԂ֓Ǎ���
																						//       != NULL �w����ۯ��֓Ǎ���
{
	MINT			Status;																// �ð��
	MINT			Code;
	MINT			PrmType;															// ����è������
	MINT			Color;																// �F�ԍ�  �i�ԍ��j
	MINT			LineNo;																// ����ԍ��i�ԍ��j
	MINT			LineWd;																// �����ԍ��i�ԍ��j
	HXPRIM			Prim;
	MCHAR			LyName[NAMMAX];														// ڲԖ�
	MCHAR			LnName[NAMMAX];														// ���햼
	HBOOL			LyrOK;																// ڲԏ�񂪂�������
	MDxfLayer*		LayPtr;
	MDxfLines*		LinPtr;
	MDxfPrim*		PrmPtr;
	MINT			LColor;																// �F�ԍ�  �i�ԍ��j
	MINT			LLineNo;															// ����ԍ��i�ԍ��j


	// �����ݒ�
	LyrOK   = HFALSE;																	// ڲԖ���`�Ȃ�
	PrmType = HPRIM_LIN;																// ����  �F��
	Color   = 256;																		// �F�ԍ��F
	LineWd  = 3;																		// �������F�א��Œ�
	BwsStrCpy(LnName, BwsStr("BYLAYER"));												// ���햼�F
	ZeroMemory(&Prim, sizeof(HXPRIM));													// �ر

	// �O���[�v�R�[�h�O���o�ꂷ��܂œǂݍ���Ő��f�[�^�쐬
	while  ((Status = MDxfGetLine(FLPtr, &Code, Data)) == 0) {
		if (Code == 6) {																// ���햼
			BwsSscanf(Data, BwsStr("%s"), LnName);
		} else if (Code == 8) {															// ���C����
			LyrOK = HTRUE;
			BwsSscanf(Data, BwsStr("%s"), LyName);
		} else if (Code == 10) {														// �n�_X
			BwsSscanf(Data, BwsStr("%f"), &Prim.Line.Pnt[0].X);
		} else if (Code == 20) {														// �n�_Y
			BwsSscanf(Data, BwsStr("%f"), &Prim.Line.Pnt[0].Y);
		} else if (Code == 30) {														// �n�_Z
			BwsSscanf(Data, BwsStr("%f"), &Prim.Line.Pnt[0].Z);
		} else if (Code == 11) {														// �I�_X
			BwsSscanf(Data, BwsStr("%f"), &Prim.Line.Pnt[1].X);
		} else if (Code == 21) {														// �I�_Y
			BwsSscanf(Data, BwsStr("%f"), &Prim.Line.Pnt[1].Y);
		} else if (Code == 31) {														// �I�_Z
			BwsSscanf(Data, BwsStr("%f"), &Prim.Line.Pnt[0].Z);
		} else if (Code == 62) {														// �F�ԍ�
			BwsSscanf(Data, BwsStr("%d"), &Color);
		} else if (Code == 0) {															// ��؂蕶��
			if (LyrOK != HTRUE) {														// ڲԖ�������`
				Status = -1;
				goto  PRG_EXIT;
			}
			break;
		}
	}

	LayPtr = DxfInf->GetLayer(LyName, &LColor, &LLineNo);								// ڲԏ�񌟍�
	if (LayPtr == NULL)  goto  PRG_EXIT;

	// �F���`�F�b�N
	if (Color == 0) {																	// BYBLOCK
		// �}�����Ɍ���
	} else if (Color == 256) {															// BYLAYER
		Color = LColor;
	} else if (Color < 0) {																// ��wOFF
		Status = 1;																		// ��\�����ڂ͓Ǎ��܂Ȃ�
		goto  PRG_EXIT;
	}

	// �����`�F�b�N
	if (BwsStrCmp(LnName, BwsStr("BYLAYER")) == 0) {
		LineNo = LLineNo;																// �����̎w��@���C��
	} else {
		LinPtr = DxfInf->GetLines(LnName, &LineNo);
//		if (LinPtr == 0)  goto  PRG_EXIT;												// �Ƃ肠����  CONTINUOUS
		if (LinPtr == 0)  LineNo = LLineNo;;											// �Ƃ肠����
	}

	// �������X�g�֒ǉ�
	if (Block == NULL) {																// ڲԂ֒ǉ�
		PrmPtr = LayPtr->AddPrim(PrmType, Color, LineNo, LineWd, &Prim);				// �}�`���ݽ�׸�
	} else {																			// ��ۯ��֒ǉ�
		PrmPtr = Block->AddPrim(PrmType, Color, LineNo, LineWd, &Prim);					// �}�`���ݽ�׸�
	}

PRG_EXIT:

	return(Status);
}
/******************************************************************************************************************
[�@�\] �|�����C������Ǎ��݁A���C���������̓u���b�N���̃v���~�e�B�u���X�g�֒ǉ�����
[�Ԓl] = 0 ����  < 0�G���[  > 0 �x�� ��\�����ڂł���
******************************************************************************************************************/
MINT MDxfGetPrmPln        (															// (  O) �ð��
							CStdioFile*			FLPtr,									// (I  ) ̧�َ��ʎq
							MCHAR*				Data,									// (  O) �Ǎ��ݗ̈�
							MDxfInf*			DxfInf,									// (I/O) DXF���
							MDxfBlock*			Block)									// (I  ) ��ۯ�
																						//       == NULL ڲԂ֓Ǎ���
																						//       != NULL �w����ۯ��֓Ǎ���
{
	MINT			Status;																// �ð��
	MINT			Code;
	MINT			PrmType;															// ����è������
	MINT			Color;																// �F�ԍ�  �i�ԍ��j
	MINT			LineNo;																// ����ԍ��i�ԍ��j
	MINT			LineWd;																// �����ԍ��i�ԍ��j
	HXPRIM			Prim;
	MCHAR			LyName[NAMMAX];														// ڲԖ�
	MCHAR			LnName[NAMMAX];														// ���햼
	HBOOL			LyrOK;																// ڲԏ�񂪂�������
	MDxfLayer*		LayPtr;
	MDxfLines*		LinPtr;
	MDxfPrim*		PrmPtr;
	MINT			LColor;																// �F�ԍ�  �i�ԍ��j
	MINT			LLineNo;															// ����ԍ��i�ԍ��j
	MINT			Flag;																// ���ײ��׸�
	MINT			VerNum;																// ��Ɨp���_��
	HXVERTEX*		Vertex     = NULL;													// ��Ɨp���_
	MINT			MNum;																// ү����
	MINT*			MVerNumPnt = NULL;													// ү�����̒��_����
	MINT			MNumPntNo;															// ���_�ԍ���
	MINT*			MPntNo     = NULL;													// ���_�ԍ���
	MINT			MNumPnt;															// ���_��
	MgPoint3*		MPnt       = NULL;													// ���_��
	MINT			ic, jc;																// ٰ�߶���


	// ���_�f�[�^�p�̈�̊m��
	if ((Vertex = (HXVERTEX *)malloc(sizeof(HXVERTEX) * 4096)) == NULL)  HMFAIL;

	VerNum   = 0;
	ZeroMemory(&Vertex[VerNum], sizeof(HXVERTEX));										// �ر
	LyrOK    = HFALSE;																	// ڲԖ���`�Ȃ�
	PrmType  = HPRIM_LIN;																// ���߁F��
	Flag     = 0;																		// �׸���̫�āF0
	Color    = 256;																		// �F�ԍ��FBYLAYER
	LineWd   = 3;																		// �������F�א��Œ�
	BwsStrCpy(LnName, BwsStr("BYLAYER"));												// ���햼�FBYLAYER
	ZeroMemory(&Prim, sizeof(HXPRIM));													// �ر

	while ((Status = MDxfGetLine(FLPtr, &Code, Data)) == 0) {
		if (Code == 6) {																// ���햼
			BwsSscanf(Data, BwsStr("%s"), LnName);
		} else if (Code == 8) {															// ڲԖ�
			LyrOK = HTRUE;
			BwsSscanf(Data, BwsStr("%s"), LyName);
		} else if (Code == 62) {														// �F�ԍ�
			BwsSscanf(Data, BwsStr("%d"), &Color);
		} else if (Code == 70){															// �׸�
			BwsSscanf(Data, BwsStr("%d"), &Flag);
		} else if (Code == 0) {															// ��؂蕶��
			if (BwsStrCmp(Data, BwsStr("VERTEX")) == 0) {
				Status = MDxfGetLine(FLPtr, &Code, Data);								// �ЂƂ�ǂ�
				while (1) {
					if (Code == 10) {
						BwsSscanf(Data, BwsStr("%f"), &Vertex[VerNum].Pnt[0]);
						Vertex[VerNum].Type = POLYVER;
					} else if (Code == 20) {
						BwsSscanf(Data, BwsStr("%f"), &Vertex[VerNum].Pnt[1]);
						Vertex[VerNum].Type = POLYVER;
					} else if (Code == 30) {
						BwsSscanf(Data, BwsStr("%f"), &Vertex[VerNum].Pnt[2]);
						Vertex[VerNum].Type = POLYVER;
					} else if (Code == 42) {
						BwsSscanf(Data, BwsStr("%f"), &Vertex[VerNum].Tan);
						Vertex[VerNum].Type = POLYVER;
					} else if (Code == 71) {											// ���_�ԍ�1
						BwsSscanf(Data, BwsStr("%d"), &Vertex[VerNum].VerNo[0]);
						Vertex[VerNum].Type = POLYMESH;
					} else if (Code == 72) {											// ���_�ԍ�2
						BwsSscanf(Data, BwsStr("%d"), &Vertex[VerNum].VerNo[1]);
						Vertex[VerNum].Type = POLYMESH;
					} else if (Code == 73) {											// ���_�ԍ�3
						BwsSscanf(Data, BwsStr("%d"), &Vertex[VerNum].VerNo[2]);
						Vertex[VerNum].Type = POLYMESH;
					} else if (Code == 74) {											// ���_�ԍ�
						BwsSscanf(Data, BwsStr("%d"), &Vertex[VerNum].VerNo[3]);
						Vertex[VerNum].Type = POLYMESH;
					} else if (Code == 0) {
						VerNum++;
						ZeroMemory(&Vertex[VerNum], sizeof(HXVERTEX));					// �ر
						if (BwsStrCmp(Data, BwsStr("SEQEND")) == 0)  break;

						if (_msize(Vertex) < (HUINT)sizeof(HXVERTEX) * (VerNum+1)) {
							if ((Vertex = (HXVERTEX *)realloc(Vertex, sizeof(HXVERTEX) * (VerNum+256))) == NULL)  HMFAIL;
						}
					}
					Status = MDxfGetLine(FLPtr, &Code, Data);
				}
			}
			if (BwsStrCmp(Data, BwsStr("SEQEND")) == 0) {
				if (LyrOK != HTRUE ) {													// ڲԖ�������`
					Status = -1;
					goto  PRG_EXIT;
				}
				if (VerNum <= 0) {														// ���_�ް����Ȃ�
					Status = -1;
					goto  PRG_EXIT;
				}
				break;
			}
		}
	}

	LayPtr = DxfInf->GetLayer(LyName, &LColor, &LLineNo);								// ڲԏ�񌟍�
	if (LayPtr == NULL)  goto  PRG_EXIT;

	// �F���`�F�b�N
	if (Color == 0) {																	// BYBLOCK
		// �}�����Ɍ���
	} else if (Color == 256) {															// BYLAYER
		Color = LColor;
	} else if (Color < 0) {																// ��wOFF
		Status = 1;																		// ��\�����ڂ͓Ǎ��܂Ȃ�
		goto  PRG_EXIT;
	}

	// �����`�F�b�N
	if (BwsStrCmp(LnName, BwsStr("BYLAYER")) == 0) {
		LineNo = LLineNo;																// �����̎w��@���C��
	} else {
		LinPtr = DxfInf->GetLines(LnName, &LineNo);
		if (LinPtr == 0)  goto  PRG_EXIT;
	}


	// �|�����C���ƃ|�����b�V���𕪊򂷂�
	if (((Flag & 16) == 16) || ((Flag & 32) == 32) || ( (Flag & 64) == 64)) {			// ���ү��
		// ���b�V���v���~�e�B�u���̃��������m��
		MNum      = 0;																	// ү����
		MNumPntNo = 0;																	// ���_�ԍ���
		MNumPnt   = 0;																	// ���_��
		for (ic = 0 ; ic < VerNum ; ic++) {
			switch (Vertex[ic].Type) {
				case  POLYMESH:															// ү���ް�
					MNum++;																// ү����
					for (jc = 0 ; jc < 4 ; jc++) {
						if ((Vertex+ic)->VerNo[jc] > 0)  MNumPntNo++;
					}
					break;
				case  POLYVER:															// ���_�ް�
					MNumPnt++;
					break;
				default:
					break;
			}
		}

		if ((MVerNumPnt = (MINT *)malloc(sizeof(MINT)       * MNum))      == NULL)  HMFAIL;		// ү�����̒��_����
		if ((MPntNo     = (MINT *)malloc(sizeof(MINT)       * MNumPntNo)) == NULL)  HMFAIL;		// ���_�ԍ���
		if ((MPnt       = (MgPoint3 *)malloc(sizeof(MgPoint3) * MNumPnt))   == NULL)  HMFAIL;		// ���_��

		// ���쐬
		MNum      = 0;																	// ү����
		MNumPntNo = 0;																	// ���_�ԍ���
		MNumPnt   = 0;																	// ���_��
		for (ic = 0 ; ic < VerNum ; ic++) {
			switch (Vertex[ic].Type) {
				case  POLYMESH:															// ү���ް�
					*(MVerNumPnt + MNum) = 0;											// ү���̒��_��
					for (jc = 0 ; jc < 4 ; jc++) {
						if ((Vertex+ic)->VerNo[jc] > 0) {
							*(MPntNo+MNumPntNo) = ((Vertex+ic)->VerNo[jc]) - 1;
							MNumPntNo++;
							(*(MVerNumPnt + MNum))++;									// ү���̒��_��
						}
					}
					MNum++;
					break;
				case  POLYVER:															// ���_�ް�
					MPntNo[MNumPnt] = ic;
					memcpy(&MPnt[MNumPnt], (Vertex+ic)->Pnt, sizeof(MgPoint3));
					MNumPnt++;
					break;
			default:
				break;
			}
		}
		// �v���~�e�B�u�쐬
		Prim.Mesh.Num       = MNum;														// ү����
		Prim.Mesh.VerNumPnt = MVerNumPnt;												// ү�����̒��_����
		Prim.Mesh.NumPntNo  = MNumPntNo;												// ���_�ԍ���
		Prim.Mesh.PntNo     = MPntNo;													// ���_�ԍ���
		Prim.Mesh.NumPnt    = MNumPnt;													// ���_��
		Prim.Mesh.Pnt       = MPnt;														// ���_��

		// �������X�g�֒ǉ�
		if (Block == NULL) {															// ڲԂ֒ǉ�
			PrmPtr = LayPtr->AddPrim(HPRIM_MSH, Color, LineNo, LineWd, &Prim);			// �}�`���ݽ�׸�
		} else {																		// ��ۯ��֒ǉ�
			PrmPtr = Block->AddPrim(HPRIM_MSH, Color, LineNo, LineWd, &Prim);			// �}�`���ݽ�׸�
		}
	} else if ((Flag & 9) == 9) {														// ���ү��
		MNum      = 1;
		MNumPntNo = 0;
		MNumPnt   = 0;
		if ((MVerNumPnt = (MINT *)malloc(sizeof(MINT)       * 1))      == NULL)  HMFAIL;		// ү�����̒��_����
		if ((MPntNo     = (MINT *)malloc(sizeof(MINT)       * VerNum)) == NULL)  HMFAIL;		// ���_�ԍ���
		if ((MPnt       = (MgPoint3 *)malloc(sizeof(MgPoint3) * VerNum)) == NULL)  HMFAIL;		// ���_��
		for (ic = 0 ; ic < VerNum ; ic++) {
			MNumPntNo++;
			MPntNo[MNumPnt] = ic;
			memcpy(&MPnt[MNumPnt], (Vertex+ic)->Pnt, sizeof(MgPoint3));
			MNumPnt++;
		}
		MVerNumPnt[0] = MNumPnt;
		// �v���~�e�B�u�쐬
		Prim.Mesh.Num       = MNum;														// ү����
		Prim.Mesh.VerNumPnt = MVerNumPnt;												// ү�����̒��_����
		Prim.Mesh.NumPntNo  = MNumPntNo;												// ���_�ԍ���
		Prim.Mesh.PntNo     = MPntNo;													// ���_�ԍ���
		Prim.Mesh.NumPnt    = MNumPnt;													// ���_��
		Prim.Mesh.Pnt       = MPnt;														// ���_��

		// �������X�g�֒ǉ�
		if (Block == NULL) {															// ڲԂ֒ǉ�
			PrmPtr = LayPtr->AddPrim(HPRIM_MSH, Color, LineNo, LineWd, &Prim);			// �}�`���ݽ�׸�
		} else {																		// ��ۯ��֒ǉ�
			PrmPtr = Block->AddPrim(HPRIM_MSH, Color, LineNo, LineWd, &Prim);			// �}�`���ݽ�׸�
		}
	} else {																			// ���ײ�
		// �|�����C���Ɖ~�ʂ̃f�[�^���쐬����
		for (ic = 0 ; ic < (VerNum - 1) ; ic++) {
			if (Vertex[ic].Tan == 0.0f) {												// ����
				memcpy(&Prim.Line.Pnt[0], &Vertex[ic].Pnt,   sizeof(MgPoint3));
				memcpy(&Prim.Line.Pnt[1], &Vertex[ic+1].Pnt, sizeof(MgPoint3));
			} else {																	// �c��݂���F�~��
				PrmType = HPRIM_ARC;													// �~��         (5)
				Status  = MDxfPlnToArc((MgPoint3 *)&Vertex[ic].Pnt, (MgPoint3 *)&Vertex[ic+1].Pnt,
									    Vertex[ic].Tan, &Prim.Arc);
			}
			// �������X�g�֒ǉ�
			if (Block == NULL) {														// ڲԂ֒ǉ�
				PrmPtr = LayPtr->AddPrim(PrmType, Color, LineNo, LineWd, &Prim);		// �}�`���ݽ�׸�
			} else {																	// ��ۯ��֒ǉ�
				PrmPtr = Block->AddPrim(PrmType, Color, LineNo, LineWd, &Prim);			// �}�`���ݽ�׸�
			}
		}

		// �N���[�Y�����|�����C���̏ꍇ�̒��_�f�[�^�w���|�C���^��Vertex�łȂ���ic(=Vertex-1)�ł���
		if ((Flag & 1) == 1) {															// �׸ނP�������Ă���
			if ((Vertex+ic)->Tan == 0.0f) {												// ����
				memcpy(&Prim.Line.Pnt[0], &Vertex[ic].Pnt, sizeof(MgPoint3));
				memcpy(&Prim.Line.Pnt[1], &Vertex[0].Pnt,  sizeof(MgPoint3));
			} else {																	// �c��݂���F�~��
				PrmType = HPRIM_ARC;													// �~��         (5)
				Status  = MDxfPlnToArc((MgPoint3 *)&Vertex[ic].Pnt, (MgPoint3 *)&Vertex[0].Pnt,
									    Vertex[ic].Tan, &Prim.Arc);
			}
			// �������X�g�֒ǉ�
			if (Block == NULL) {														// ڲԂ֒ǉ�
				PrmPtr = LayPtr->AddPrim(PrmType, Color, LineNo, LineWd, &Prim);		// �}�`���ݽ�׸�
			} else {																	// ��ۯ��֒ǉ�
				PrmPtr = Block->AddPrim(PrmType, Color, LineNo, LineWd, &Prim);			// �}�`���ݽ�׸�
			}
		}
	}

PRG_EXIT:

	HFREE(Vertex);
	HFREE(MVerNumPnt);
	HFREE(MPntNo);
	HFREE(MPnt);

	return(Status);
}
/******************************************************************************************************************
[�@�\] ��������Ǎ��݁A���C���������̓u���b�N���̃v���~�e�B�u���X�g�֒ǉ�����
[�Ԓl] = 0 ����  < 0�G���[  > 0 �x�� ��\�����ڂł���
******************************************************************************************************************/
MINT MDxfGetPrmTxt        (															// (  O) �ð��
							CStdioFile*			FLPtr,									// (I  ) ̧�َ��ʎq
							MCHAR*				Data,									// (  O) �Ǎ��ݗ̈�
							MDxfInf*			DxfInf,									// (I/O) DXF���
							MDxfBlock*			Block)									// (I  ) ��ۯ�
																						//       == NULL ڲԂ֓Ǎ���
																						//       != NULL �w����ۯ��֓Ǎ���
{
	MINT			Status;																// �ð��
	MINT			Code;
	MINT			PrmType;															// ����è������
	MINT			Color;																// �F�ԍ�  �i�ԍ��j
	HXPRIM			Prim;
	MCHAR			FnName[NAMMAX];														// ̫�Ė�
	MCHAR			LyName[NAMMAX];														// ڲԖ�
	MCHAR			LnName[NAMMAX];														// ���햼
	HBOOL			LyrOK;																// ڲԏ�񂪂�������
	MDxfLayer*		LayPtr;																// ڲԏ���߲��
	MDxfFonts*		FntPtr;																// ̫�ď���߲��
	MDxfPrim*		PrmPtr;
	MINT			LColor;																// �F�ԍ�  �i�ԍ��j
	MINT			FCode;																// ̫�Ĕԍ��i�ԍ��j
	MINT			TxtLen;																// �����񒷂�


	// �����ݒ�
	LyrOK            = HFALSE;															// ڲԖ���`�Ȃ�
	PrmType          = HPRIM_TXT;														// ����  �F����
	Prim.Text.String = NULL;															// ������FNULL
	Color            = 256;																// �F�ԍ��F
	BwsStrCpy(FnName, BwsStr("STANDARD"));												// ̫�Ė��FSTANDARD
	BwsStrCpy(LnName, BwsStr("BYLAYER"));												// ���햼�F
	ZeroMemory(&Prim, sizeof(HXPRIM));													// �ر

	while ((Status = MDxfGetLine(FLPtr, &Code, Data)) == 0) {
		if (Code == 6) {																// ���햼
			BwsSscanf(Data, BwsStr("%s"), LnName);
		} else if (Code == 7) {															// �������ٖ�
			BwsSscanf(Data, BwsStr("%s"), FnName);
		} else if (Code == 8) {															// ڲԖ�
			LyrOK = HTRUE;
			BwsSscanf(Data, BwsStr("%s"), LyName);
		} else if (Code == 1) {															// ������
			TxtLen = BwsStrLen(Data);
			if ((Prim.Text.String = (MCHAR *)malloc(sizeof(MCHAR) * (TxtLen+1))) == NULL)  HMFAIL;
			Prim.Text.Num = TxtLen;														// ������
			BwsStrCpy(Prim.Text.String, Data);
		} else if (Code == 10) {														// �J�n�_X
			BwsSscanf(Data, BwsStr("%f"), &Prim.Text.Pnt.X);
		} else if (Code == 20) {														// �J�n�_Y
			BwsSscanf(Data, BwsStr("%f"), &Prim.Text.Pnt.Y);
		} else if (Code == 30) {														// �J�n�_Z
			BwsSscanf(Data, BwsStr("%f"), &Prim.Text.Pnt.Z);
		} else if (Code == 40) {														// ��������
			BwsSscanf(Data, BwsStr("%f"), &Prim.Text.Hei);
		} else if (Code == 50) {														// ��]�p�x
			BwsSscanf(Data, BwsStr("%f"), &Prim.Text.Ang);
		} else if (Code == 62) {														// �F�ԍ�
			BwsSscanf(Data, BwsStr("%d"), &Color);
		} else if (Code == 0) {															// ��؂蕶��
			if (LyrOK != HTRUE) {														// ڲԖ�������`
				Status = -1;
				goto  PRG_EXIT;
			}
			break;
		}
	}

	LayPtr = DxfInf->GetLayer(LyName, &LColor);											// ڲԏ�񌟍�
	if (LayPtr == NULL)  goto  PRG_EXIT;

	// �F���`�F�b�N
	if (Color == 0) {																	// BYBLOCK
		// �}�����Ɍ���
	} else if (Color == 256) {															// BYLAYER
		Color = LColor;
	} else if (Color < 0) {																// ��wOFF
		Status = 1;																		// ��\�����ڂ͓Ǎ��܂Ȃ�
		goto  PRG_EXIT;
	}

	FntPtr = DxfInf->GetFonts(FnName, &FCode);											// ̫�ď�񌟍�
	if (FntPtr == NULL)  goto  PRG_EXIT;

	if ((FCode & 4) == 4) {																// ���������S�i�c�j�̃r�b�g�������Ă���
		Prim.Text.Msc = -1.0f;
	} else {
		Prim.Text.Msc =  1.0f;
	}

	// �������X�g�֒ǉ�
	if (Block == NULL) {																// ڲԂ֒ǉ�
		PrmPtr = LayPtr->AddPrim(PrmType, Color, 0, 3, &Prim);							// �}�`���ݽ�׸�
	} else {																			// ��ۯ��֒ǉ�
		PrmPtr = Block->AddPrim(PrmType, Color, 0, 3, &Prim);							// �}�`���ݽ�׸�
	}

PRG_EXIT:

	if (Prim.Text.String)  free(Prim.Text.String);

	return(Status);
}

/******************************************************************************************************************
              �c�w�e  �v���O�����t�@�C�� (MDxf.cpp)

[�@�\] �t�H���g�e�[�u���̓Ǎ��݂Ɛݒ�
[�Ԓl] = 0 ����  < 0�G���[
******************************************************************************************************************/


#include	"MDxf.h"


MINT MDxfGetTblFont       (														// (  O) �ð��
							CStdioFile*			FLPtr,								// (I  ) ̧�َ��ʎq
							MDxfInf*			DxfInf)								// (I/O) DXF���
{
	MINT			Status;															// �ð��
	MINT			Code;
	MCHAR			Data[512];
	MCHAR			Name[NAMMAX];
	MINT			FCode;
	HBOOL			GetTbl;															// ð��ٓǍ����׸�


	GetTbl = HFALSE;
	BwsStrCpy(Name, BwsStr(""));

	// �t�H���g�e�[�u���̓ǂݍ���
	while  ((Status = MDxfGetLine(FLPtr, &Code, Data)) == 0) {
		if (Code == 2) {															// ̫�Ė�
			GetTbl = HTRUE;
			BwsSscanf(Data, BwsStr("%s"), Name);
		} else if (Code == 70) {													// �W���׸�(�ʏ�64)
			GetTbl = HTRUE;
			BwsSscanf(Data, BwsStr("%d"), &FCode);
		} else if (Code == 0) {														// ��؂蕶��
			if (GetTbl == HTRUE) {
				DxfInf->AddFonts(Name, FCode);										// ̫�ď��ǉ�
				GetTbl = HFALSE;													// ���ް��Ɍ���������
			}
			if (BwsStrCmp(Data, BwsStr("ENDTAB")) == 0)  break;						// ̫��ð��ُI��
		}
	}

	return(Status);
}
/******************************************************************************************************************
[�@�\] ����e�[�u���̓Ǎ��݂Ɛݒ�
[�Ԓl] = 0 ����  < 0�G���[
******************************************************************************************************************/
MINT MDxfGetTblLin        (														// (  O) �ð��
							CStdioFile*			FLPtr,								// (I  ) ̧�َ��ʎq
							MDxfInf*			DxfInf)								// (I/O) DXF���
{
	MINT			Status;															// �ð��
	MINT			Code;
	MCHAR			Data[512];
	MCHAR			Name[NAMMAX];
	MINT			LineNo;
	HBOOL			GetTbl;															// ð��ٓǍ����׸�
	MINT			DashNum;														// �ޯ������(��ٰ�ߺ���49)�ް���


	GetTbl  = HFALSE;
	DashNum = 0;

	// ����e�[�u���̓ǂݍ���
	while  ((Status = MDxfGetLine(FLPtr, &Code, Data)) == 0) {
		if (Code == 2) {															// ���햼
			GetTbl = HTRUE;
			BwsSscanf(Data, BwsStr("%s"), Name);
		} else if (Code == 49) {													// �ޯ������
			DashNum++;
		} else if (Code == 0) {														// ��؂蕶��
			if (GetTbl == HTRUE)	{
				// ����̔���
				switch  (DashNum) {
					case  0:														// ����
						LineNo = HLINE_SOLD;										// (1)
						break;
					case  2:														// �j��
						LineNo = HLINE_DOT;											// (4)
						break;
					case  4:														// ��_����
						LineNo = HLINE_DASH1PS;										// (5)
						break;
					default:														// ��_����
						LineNo = HLINE_DASH2P;										// (7)
						break;
				}

				DxfInf->AddLines(Name, LineNo);										// �����ǉ�

				GetTbl  = HFALSE;													// ���ް��̂��ߏ�����
				DashNum = 0;

			}
			if (BwsStrCmp(Data, BwsStr("ENDTAB")) == 0)  break; 					// ����ð��ُI��
		}
	}

	return(Status);
}
/******************************************************************************************************************
[�@�\] ���C���e�[�u���̓Ǎ��݂Ɛݒ�
[�Ԓl] = 0 ����  < 0�G���[
******************************************************************************************************************/
MINT MDxfGetTblLyr            (													// (  O) �ð��
								CStdioFile*			FLPtr,							// (I  ) ̧�َ��ʎq
								MDxfInf*			DxfInf)							// (I/O) DXF���
{
	MINT			Status;															// �ð��
	MINT			Code;
	MCHAR			Data[512];
	MCHAR			Name[NAMMAX];
	MINT			Color;
	MINT			LineNo;
	MDxfLines*		LinPtr;
	HBOOL			GetTbl;															// ð��ٓǍ����׸�


	GetTbl        = HFALSE;

	// ���C���e�[�u����ǂݍ���
	while  ((Status = MDxfGetLine(FLPtr, &Code, Data)) == 0) {
		if (Code == 2) {															// ڲԖ�
			GetTbl = HTRUE;
			BwsSscanf(Data, BwsStr("%s"), Name);
		} else if (Code == 62) {													// �F�ԍ�
			GetTbl = HTRUE;
			BwsSscanf(Data, BwsStr("%d"), &Color);
		} else if (Code == 6) {														// ����
			GetTbl = HTRUE;
			LinPtr = DxfInf->GetLines(Data, &LineNo);								// ����񌟍�
			if (LinPtr == 0)  LineNo = HLINE_SOLD;									// �������݂���Ȃ�
		} else if (Code == 0) {														// ��؂�
			if (GetTbl == HTRUE) {
				DxfInf->AddLayer(Name, Color, LineNo);								// ڲԏ��ǉ�
				GetTbl = HFALSE;													// ���ް��̂��߂ɏ�����
			}
			if (BwsStrCmp(Data, BwsStr("ENDTAB")) == 0)  break;						// ڲ�ð��ُI��
		}
	}

	return(Status);
}
/******************************************************************************************************************
[�@�\] �e�[�u���Z�N�V������Ǎ��ށB�ΏۂƂȂ�e�[�u����,LTYPE(����),STYLE(��������),LAYER(��w)
[�Ԓl] = 0 ����  < 0�G���[
******************************************************************************************************************/
MINT MDxfGetTblSec            (													// (   O) �ð��
								CStdioFile*			FLPtr,							// (I  ) ̧�َ��ʎq
								MDxfInf*			DxfInf)							// (I/O) DXF���
{
	MINT			Status;
	MINT			Code;
	MCHAR			Data[512];


	Status = 0;

	while  ((Status = MDxfGetLine(FLPtr, &Code, Data)) == 0) {
		if (BwsStrCmp(Data, BwsStr("ENDSEC")) == 0) {								// ����ݏI��
			break;
		} else if (BwsStrCmp(Data, BwsStr("LTYPE")) == 0) {		 					// ����Ή�ð��ق̍쐬 
			Status = MDxfGetTblLin(FLPtr, DxfInf);
			if (Status < 0)  break;
		} else if (BwsStrCmp(Data, BwsStr("STYLE")) == 0) {							// ������Ή�ð��ق̍쐬
			Status = MDxfGetTblFont(FLPtr, DxfInf);
			if (Status < 0)  break;
		} else if (BwsStrCmp(Data, BwsStr("LAYER")) == 0) {
			Status = MDxfGetTblLyr(FLPtr, DxfInf);									// ڲ�ð��ق̍쐬
			if (Status < 0)  break;
		}
	}

	return(Status);
}

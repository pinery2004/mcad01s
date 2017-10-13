/******************************************************************************************************************
              �c�w�e  �v���O�����t�@�C�� (MDxf.cpp)

[�@�\] �u���b�N�Z�N�V������Ǎ��݁A�u���b�N�e�[�u�����쐬����
[�Ԓl] = 0 ����  < 0�G���[
******************************************************************************************************************/


#include	"MDxf.h"


MINT MDxfGetBlcSec        (														// (  O) �ð��
							CStdioFile*			FLPtr,								// (I  ) ̧�َ��ʎq
							MDxfInf*			DxfInf)								// (I/O) DXF���
{
	MINT			Status;															// �ð��
	MINT			Code;
	MCHAR			Data[512];
	MDxfBlock*		BlkPtr = NULL;
	MCHAR			Name[NAMMAX];
	MCHAR			LyName[NAMMAX];
	MgPoint3			InsPnt;
	HBOOL			GetTbl;															// ð��ٓǍ����׸�


	Status = 0;

	while  ((Status = MDxfGetLine(FLPtr, &Code, Data)) == 0) {
		if (BwsStrCmp(Data, BwsStr("ENDSEC")) == 0) {								// ����ݏI��
			break;
		} else if (BwsStrCmp(Data, BwsStr("BLOCK")) == 0) {							// ��ۯ��̍쐬
			BlkPtr = DxfInf->AddBlock();											// ��ۯ����ǉ�

			GetTbl = HFALSE;
			if ((Status = MDxfGetLine(FLPtr, &Code, Data)) != 0)  break;

			while  (1) {
				if (BwsStrCmp(Data, BwsStr("ENDBLK")) == 0) {						// ��ۯ��I��
					if (GetTbl == HTRUE) {
						BlkPtr->SetInf(Name, LyName, 0, 0, &InsPnt);				// ��ۯ������
					}
					break;
				}

				// �f�[�^�̎擾
				if (Code == 2) {													// ��ۯ���
					GetTbl = HTRUE;
					BwsSscanf(Data, BwsStr("%s"), Name);
				} else if (Code == 8) {												// ڲԖ�
					GetTbl = HTRUE;
					BwsSscanf(Data, BwsStr("%s"), LyName);
				} else if (Code == 10) {											// �}����_�w
					GetTbl = HTRUE;
					BwsSscanf(Data, BwsStr("%f"), &InsPnt.X);
				} else if (Code == 20) {											// �}����_�x
					GetTbl = HTRUE;
					BwsSscanf(Data, BwsStr("%f"), &InsPnt.Y);
				} else if (Code == 30) {											// �}����_�y
					GetTbl = HTRUE;
					BwsSscanf(Data, BwsStr("%f"), &InsPnt.Z);
				} else if (Code == 0) {												// ��؂蕶��
					if (BwsStrCmp(Data, BwsStr("ENDBLK")) == 0)  continue;			// ��ۯ��I��

					// �}�`���𓾂�
					if (BwsStrCmp(Data, BwsStr("LINE")) == 0) {
						Status = MDxfGetPrmLin(FLPtr, Data, DxfInf, BlkPtr);
						if (Status < 0) break;
						continue;													// ��ǂ݂��Ă���̂œǂݍ��܂Ȃ�

					} else if (BwsStrCmp(Data, BwsStr("POLYLINE")) == 0) {
						Status = MDxfGetPrmPln(FLPtr, Data, DxfInf, BlkPtr);
						if (Status < 0)  break;
						continue;													// ��ǂ݂��Ă���̂œǂݍ��܂Ȃ�

					} else if (BwsStrCmp(Data, BwsStr("POINT")) == 0) {
						Status = MDxfGetPrmDot(FLPtr, Data, DxfInf, BlkPtr);
						if (Status < 0 )  break;
						continue;													// ��ǂ݂��Ă���̂œǂݍ��܂Ȃ�

					} else if (BwsStrCmp(Data, BwsStr("CIRCLE")) == 0) {
						Status = MDxfGetPrmCir(FLPtr, Data, DxfInf, BlkPtr);
						if (Status < 0)  break;
						continue;													// ��ǂ݂��Ă���̂œǂݍ��܂Ȃ�

					} else if (BwsStrCmp(Data, BwsStr("ARC")) == 0) {
						Status = MDxfGetPrmCir(FLPtr, Data, DxfInf, BlkPtr);
						if (Status < 0)  break;
						continue;													// ��ǂ݂��Ă���̂œǂݍ��܂Ȃ�

					} else if (BwsStrCmp(Data, BwsStr("TEXT")) == 0 ) {
						Status = MDxfGetPrmTxt(FLPtr, Data, DxfInf, BlkPtr);
						if (Status < 0)  break;
						continue;													// ��ǂ݂��Ă���̂œǂݍ��܂Ȃ�

					} else if (BwsStrCmp(Data, BwsStr("ATTRIB")) == 0) {
						Status = MDxfGetPrmAtb(FLPtr, Data, DxfInf, BlkPtr);
						if (Status < 0) break;
						continue;													// ��ǂ݂��Ă���̂œǂݍ��܂Ȃ�

					} else if (BwsStrCmp(Data, BwsStr("3DFACE")) == 0) {
						Status = MDxfGetPrmFac(FLPtr, Data, DxfInf, BlkPtr);
						if (Status < 0 )  break;
						continue;													// ��ǂ݂��Ă���̂œǂݍ��܂Ȃ�

					} else if (BwsStrCmp(Data, BwsStr("INSERT")) == 0) {
						Status = MDxfGetPrmIns(FLPtr, Data, DxfInf, BlkPtr);
						if (Status < 0)  break;
						continue;													// ��ǂ݂��Ă���̂œǂݍ��܂Ȃ�
					}
				}

				if ((Status = MDxfGetLine(FLPtr, &Code, Data)) != 0)  break;
			}
		}
	}

	return(Status);

}

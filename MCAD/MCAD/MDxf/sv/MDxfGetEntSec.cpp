/******************************************************************************************************************
              �c�w�e  �v���O�����t�@�C�� (MDxf.cpp)

[�@�\] �G���e�B�e�B�Z�N�V������Ǎ��݁A�v���~�e�B�u�ɓW�J����
[�Ԓl] = 0 ����  < 0�G���[
******************************************************************************************************************/


#include	"MDxf.h"


MINT MDxfGetEntSec        (														// (  O) �ð��
							CStdioFile*			FLPtr,								// (I  ) ̧�َ��ʎq
							MDxfInf*			DxfInf)								// (I/O) DXF���
{
	MINT			Status;															// �ð��
	MINT			Code;
	MCHAR			Data[512];


	if ((Status = MDxfGetLine(FLPtr, &Code, Data)) != 0)  goto  PRG_EXIT;

	while (1) {
		if (BwsStrCmp(Data, BwsStr("ENDSEC")) == 0)  break;							// ����ݏI��

		// �����L���ŃG���e�B�e�B�^�C�v�ʏ���
		if (Code == 0) {
			if (BwsStrCmp(Data, BwsStr("LINE")) == 0) {
				Status = MDxfGetPrmLin(FLPtr, Data, DxfInf, NULL);
				if (Status < 0) break;
				continue;															// ��ǂ݂��Ă���̂œǂݍ��܂Ȃ�
			} else if (BwsStrCmp(Data, BwsStr("POLYLINE")) == 0) {
				Status = MDxfGetPrmPln(FLPtr, Data, DxfInf, NULL);
				if (Status < 0)  break;
				continue;															// ��ǂ݂��Ă���̂œǂݍ��܂Ȃ�
			} else if (BwsStrCmp(Data, BwsStr("POINT")) == 0) {
				Status = MDxfGetPrmDot(FLPtr, Data, DxfInf, NULL);
				if (Status < 0 )  break;
				continue;															// ��ǂ݂��Ă���̂œǂݍ��܂Ȃ�
			} else if (BwsStrCmp(Data, BwsStr("CIRCLE")) == 0) {
				Status = MDxfGetPrmCir(FLPtr, Data, DxfInf, NULL);
				if (Status < 0)  break;
				continue;															// ��ǂ݂��Ă���̂œǂݍ��܂Ȃ�
			} else if (BwsStrCmp(Data,  BwsStr("ARC")) == 0) {
				Status = MDxfGetPrmArc(FLPtr, Data, DxfInf, NULL);
				if (Status < 0)  break;
				continue;															// ��ǂ݂��Ă���̂œǂݍ��܂Ȃ�
			} else if (BwsStrCmp(Data,  BwsStr("TEXT") ) == 0) {
				Status = MDxfGetPrmTxt(FLPtr, Data, DxfInf, NULL);
				if (Status < 0)  break;
				continue;															// ��ǂ݂��Ă���̂œǂݍ��܂Ȃ�
			} else if (BwsStrCmp(Data, BwsStr("ATTRIB")) == 0) {
				Status = MDxfGetPrmAtb(FLPtr, Data, DxfInf, NULL);
				if (Status < 0)  break;
				continue;															// ��ǂ݂��Ă���̂œǂݍ��܂Ȃ�
			} else if (BwsStrCmp(Data, BwsStr("3DFACE")) == 0) {
				Status = MDxfGetPrmFac(FLPtr, Data, DxfInf, NULL);
				if (Status < 0)  break;
				continue;															// ��ǂ݂��Ă���̂œǂݍ��܂Ȃ�
			} else if (BwsStrCmp(Data, BwsStr("INSERT")) == 0) {
				Status = MDxfGetPrmIns(FLPtr, Data, DxfInf, NULL);
				if (Status < 0)  break;
				continue;															// ��ǂ݂��Ă���̂œǂݍ��܂Ȃ�
			} else {																// ���̑��̴�èè�͖��Ή�
				//
			}
		}
		if ((Status = MDxfGetLine(FLPtr, &Code, Data)) < 0)  goto  PRG_EXIT;
	}

PRG_EXIT:

	return(Status);
}

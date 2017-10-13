/******************************************************************************************************************
              �c�w�e  �v���O�����t�@�C�� (MDxf.cpp)

[�@�\] �w�b�_�Z�N�V������ǂݍ��݁A�~�ʂ̉�]�����f�[�^��ϐ��ɐݒ肷��
[�Ԓl] = 0 ����  < 0�G���[
******************************************************************************************************************/


#include	"MDxf.h"


MINT MDxfGetHdSec         (														// (  O) �ð��
							CStdioFile*			FLPtr,								// (I  ) ̧�َ��ʎq
							MDxfInf*			DxfInf)								// (I/O) DXF���
{
	MINT			Status;															// �ð��
	MINT			Code;
	MCHAR			Data[512];
	MINT			Dir;															// ANGDIR�ϐ��̒l


	Status = 0;

	while (MDxfGetLine(FLPtr, &Code, Data) == 0) {
		if (BwsStrCmp(Data, BwsStr("ENDSEC")) == 0) {
			 break;
		} else if (BwsStrCmp(Data, BwsStr("$ANGDIR")) == 0) {						// �p�x�����̐ݒ�
			if (MDxfGetLine(FLPtr, &Code, Data) != 0) {
				Status = -1;
				break;
			}
			if (Code == 70) {
				BwsSscanf(Data, BwsStr("%d"), &Dir);
				if (Dir == 1) {														// ���v����
					DxfInf->m_AngDir = -1;
				} else if (Dir == 0) {												// �����v����
					DxfInf->m_AngDir = 1;
				}
			} else {																// $ANGDIR�ɕs�K�ظ�ٰ�ߺ���
				Status = -1;
				break;
			}
		}
	}

	return(Status);
}

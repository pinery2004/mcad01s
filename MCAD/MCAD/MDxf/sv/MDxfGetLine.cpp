/******************************************************************************************************************
              �c�w�e  �v���O�����t�@�C�� (MDxf.cpp)

[�@�\] �c�w�e�t�@�C�����Q���C���Ǎ��ݕҏW���� �i�R�����g�s�͓ǂݔ�΂��j
[�Ԓl] = 0 ����  < 0�G���[
******************************************************************************************************************/

#include		"HsFileLib.h"
#include		"MDxf.h"


MINT MDxfGetLine          (														// (  O) �ð��
							CStdioFile*			FLPtr,								// (I  ) ̧�َ��ʎq
							MINT*				Code,								// (  O) ��ٰ�ߺ���
							MCHAR*				Data)								// (  O) ������
{
	MINT			Status;															// �ð��
	MCHAR			RBuff[512];														// �Ǎ����ޯ̧


	Status = 1;

	while (1) {
		Status = HsFileReadS(FLPtr, 511, RBuff);									// 1�s�Ǎ���
		if (Status < 0)  break;
		if ((*Code = BwsAtoI(RBuff)) < 0)  break;

		Status = HsFileReadS(FLPtr, 511, RBuff);									// 1�s�Ǎ���
		if (Status < 0)  break;

		if (*Code == 999)  continue;												// ���ēǔ�΂�

		BwsSscanf(RBuff, BwsStr("%s"), Data);
		Status = 0;
		break;
	}

	return(Status);
}

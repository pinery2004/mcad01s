/******************************************************************************************************************
              �c�w�e  �v���O�����t�@�C�� (MDxf.cpp)

[�@�\] �|�����C����̂Q�_�Ɩc��݂���A�~�ʏ����쐬����
[�Ԓl] = 0 ����  < 0�G���[
******************************************************************************************************************/


#include	"MDxf.h"


MINT MDxfPlnToArc         (
							MgPoint3*			SPnt,								// (I  ) �J�n�_
							MgPoint3*			EPnt,								// (I  ) �I���_
							MFLOAT				Tan,								// (I  ) �c���
							HARC3S*				Arc)								// (  O) �~�ʏ��
{
	MINT				Status;														// �ð��
	MFLOAT				Ang;														// ���S�p�x�i�x�j
	MgPoint3				Pnt[2];														// ��Ɨp
	MgPoint3				CPnt;														// ���S�_
	MFLOAT				Radius;
	MFLOAT				Sta, Opa;
	MFLOAT				Radian;


	Status = 0;

	// ���v����̏ꍇ�͔����v����ɏC���n�_�ƏI�_�����ւ���
	if (Tan < 0) {
		memcpy(&Pnt[0], EPnt, sizeof(MgPoint3));
		memcpy(&Pnt[1], SPnt, sizeof(MgPoint3));
	} else {
		memcpy(&Pnt[0], SPnt, sizeof(MgPoint3));
		memcpy(&Pnt[1], EPnt, sizeof(MgPoint3));
	}

	// ���S�p�����߂�
	Ang = (MFLOAT)(HABS(atan(Tan) / HPI * 720.0f)); 										//  Tan = tan(Ang / 4)

	Status = MDxfCalArcAtr(&Pnt[0], &Pnt[1], Ang, &CPnt, &Radius);						// ���S�_�Ɣ��a�����߂�
	if (Status != 0) goto  PRG_EXIT;

	// �n�_�p�A�I�_�p�����߂�
	Sta = MDxfGetAngle(&CPnt, &Pnt[0]);
	Opa = MDxfGetAngle(&CPnt, &Pnt[2]);

	Arc->Pnt1.X = CPnt.X - Radius;														// �O�ڂ��鍶��
	Arc->Pnt1.Y = CPnt.Y - Radius;
	Arc->Pnt1.Z = CPnt.Z;
	Arc->Pnt2.X = CPnt.X + Radius;														// �O�ڂ���E��
	Arc->Pnt2.Y = CPnt.Y + Radius;
	Arc->Pnt2.Z = CPnt.Z;
	Radian      = Sta / 180.0f * HPI;													// ׼ޱ݂�
	Arc->Pnt3.X = CPnt.X + Radius * (MFLOAT)cos((HDOUBLE)Radian);
	Arc->Pnt3.Y = CPnt.Y + Radius * (MFLOAT)sin((HDOUBLE)Radian);
	Arc->Pnt3.Z = CPnt.Z;
	Radian      = Opa / 180.0f * HPI;													// ׼ޱ݂�
	Arc->Pnt4.X = CPnt.X + Radius * (MFLOAT)cos((HDOUBLE)Radian);
	Arc->Pnt4.Y = CPnt.Y + Radius * (MFLOAT)sin((HDOUBLE)Radian);
	Arc->Pnt4.Z = CPnt.Z;

PRG_EXIT:

	return(Status);
}

/******************************************************************************************************************
[�@�\] �n�_�I�_�ƒ��S�p�x����~�ʂ̒��S�Ɣ��a�����߂�  �i�Q�����}�`�������Q�_��Z���W��0.0�łȂ��ꍇ�̓G���[�Ƃ���j
[�Ԓl] = 0 ����  < 0�G���[ 
******************************************************************************************************************/
MINT MDxfCalArcAtr        (
							MgPoint3*			Pnt1,								// (I  ) �P�_��
							MgPoint3*			Pnt2,								// (I  ) �Q�_��
							MFLOAT				Ang,								// (I  ) ���S�p�x
							MgPoint3*			Center,								// (  O) ���S�_
							MFLOAT*				Radius)								// (  O) ���a
{
	MINT				Status;														// �ð��
	MgPoint3				MidPnt;														// �Q�_�̒��_
	MgPoint3				Vector;														// �Q�_�̒��S����~�ʒ��S�ւ̕����x�N�g��
	MFLOAT				Height;														// �Q�_�̒��S����~�ʒ��S�܂ł̋���
	HDOUBLE				Radian;														// ���S�p�iradian�j
	HDOUBLE				Dst;														// �Q�_�ԋ���


	Status = -1;

	// �Q�_��Z���W���`�F�b�N
	if (!HCMP(Pnt1->Z, Pnt2->Z))  goto  PRG_EXIT;	

	//	�Q�_�Ԃ̋��������߂�
	Dst = sqrt((Pnt2->X - Pnt1->X) * (Pnt2->X - Pnt1->X) + 
			   (Pnt2->Y - Pnt1->Y) * (Pnt2->Y - Pnt1->Y) + 
			   (Pnt2->Z - Pnt1->Z) * (Pnt2->Z - Pnt1->Z));

	if (Dst < HDISTOL)  goto  PRG_EXIT;

	Radian  = 0.5 * Ang * HPI / 180.0f;												// ���S�p��1/2�����߂�(׼ޱݒP��)
	*Radius = (MFLOAT)(Dst * 0.5f / sin(Radian));									// ���a�����߂�

	// �Q�_�̒��_���璆�S�܂ł̋��������߂�
	Height = (MFLOAT)(Dst * 0.5f / tan(Radian));

	MidPnt.X = (Pnt1->X + Pnt2->X) * 0.5f;											// �Q�_�̒��_�����߂�
	MidPnt.Y = (Pnt1->Y + Pnt2->Y) * 0.5f;
	MidPnt.Z = (Pnt1->Y + Pnt2->Y) * 0.5f;

	// ���_��ʂ蒆�S�֌����������x�N�g�������߂�
	Vector.X = (MFLOAT)((-1.0f) * (Pnt2->X - Pnt1->X) / Dst);
	Vector.Y = (MFLOAT)((Pnt2->Y - Pnt1->Y) / Dst);
	Vector.Z = 0.0f;

	// �p�x���画�肵�Ē��S�_�𓾂�
	if ((180 < Ang) && (Ang < 360)) {
		Center->X = MidPnt.X - Height * Vector.X;
		Center->Y = MidPnt.Y - Height * Vector.Y;
		Center->Z = MidPnt.Z - Height * Vector.Z;
	} else {
		Center->X = MidPnt.X + Height * Vector.X;
		Center->Y = MidPnt.Y + Height * Vector.Y;
		Center->Z = MidPnt.Z + Height * Vector.Z;
	}
	Status = 0;

PRG_EXIT:

	return(Status);
}
/******************************************************************************************************************
[�@�\] �~�ʂ̓_�̊p�x�����߂�  �i�~�ʂ�XY���ʁiZ=0)��ɂ̂��Ă�����̂Ƃ���j
[�Ԓl] = 0 ����  < 0�G���[ 
******************************************************************************************************************/
MFLOAT MDxfGetAngle        (														// (  O) �p�x
							MgPoint3*			Center,								// (I  ) �~�ʒ��S
							MgPoint3*			Pnt)								// (I  ) �~�ʏ�̓_
{
	HDOUBLE				SaX;														// X���W����
	HDOUBLE				SaY;														// Y���W����
	HDOUBLE				Angle;														// �p�x�i�߂�l�j


	Angle = 0.0f;																	// �p�x�i�߂�l�j

	// XY���ʏォ�ǂ����`�F�b�N
	if ((Center->Z != 0.0f) || (Pnt->Z != 0.0f))  goto  PRG_EXIT;

	// 2�_�̍��������߂�
	SaX = Pnt->X - Center->X;
	SaY = Pnt->Y - Center->Y;

	// �p�x�`�F�b�N
	if (SaX == 0.0) {																// Y����
		if (SaY > 0) {
			Angle = 90.0f;															// 90�x
		} else {
			Angle = 270.0f;															// 270�x
		}
	} else {
		Angle = atan(SaY / SaX) * 180.0f / HPI;
	}

PRG_EXIT:

	return((MFLOAT)Angle);
}

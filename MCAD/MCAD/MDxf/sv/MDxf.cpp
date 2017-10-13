/*********************************************************************************************************************************
              �c�w�e  �v���O�����t�@�C�� (MDxf.cpp)

********************************************************************************************************************************/

#include		"MgLib.h"
#include		"HEtcLib.h"
#include		"HsView.h"
#include		"HsLib.h"
#include		"HnLib.h"
#include		"MDxfR14.h"
#include		"HsDirectXLib.h"
#include		"HsDHon.h"
#include		"HsRoof.h"
#include		"HsDHai.h"
#include		"MDxf.h"


#define			 OCOLOR			RGB(255, 0, 0)											// �z�u�_�F


// �ŏ���ő��ݒ�
static HVOID SetMinMax   (MgPoint3* Pnt, MgMinMax3* MinMax);
static HVOID SetMinMax   (MgPoint2* Pnt, MgMinMax3* MinMax);
// �u�����������֏���ݒ�
static MINT SetVertex    (MINT NumPnt, MgPoint3* Pnt, D3DCOLOR Rgb, MINT* NumTri, HDXVERTEX* Vertex, MINT* IPtr);
// ���[�J�����W�n�֕ϊ�
static HVOID TrnWrdToLoc (MgPoint3* TOPnt, MgPoint3* TUvw, MgPoint3* Pnt1, MgPoint2* Pnt2);


/******************************************************************************************************************
[ MDxfInf::MDxfInf ]                 �@�\  �c�w�e���R���X�g���N�^
******************************************************************************************************************/
MDxfInf::MDxfInf             (
								MINT				VewTyp,								// (I  ) �ޭ����
								MFLOAT				ProtRate,							// (I  ) ��ۯ�ڰ�
								MINT				AngDir,								// (I  ) �~�ʕ��� 1:���v -1:�����v
								MFLOAT				TextRate,							// (I  ) �����ϊ��{��
								MINT				ZmnSize,							// (I  ) �}�ʻ���
								MgMinMax3*			Window,								// (I  ) ����޳����
								MgMinMax3*			ViewPort,							// (I  ) �ޭ��߰�
								MINT				FAFlag)								// (I  ) �ʂ̌�������   0:���Ȃ�  1:����

{
	// �I�v�V������ݒ�
	m_VewTyp       = VewTyp,															// �ޭ����
	m_ProtRate     = ProtRate;															// ��ۯ�ڰ�
	m_AngDir       = AngDir;															// �����v����
	m_Symtry       = 0;																	// �Ώ��׸�       0:�Ȃ�      1:����
	m_SymtryC      = 0;																	// ���݂̑Ώ��׸� 0:�Ȃ�      1:����
	m_Matrix[0][0] = 1.f; m_Matrix[0][1] = 0.f; m_Matrix[0][2] = 0.f;					// �ϊ���ظ�
	m_Matrix[1][0] = 0.f; m_Matrix[1][1] = 1.f; m_Matrix[1][2] = 0.f;
	m_Matrix[2][0] = 0.f; m_Matrix[2][1] = 0.f; m_Matrix[2][2] = 1.f;
	m_Move.X       = m_Move.Y   = m_Move.Z   = 0.f;										// �ړ���
	m_Scale[0]     = m_Scale[1] = m_Scale[2] = 1.0f;									// �g�嗦
	m_TextRate     = TextRate;															// �����ϊ��{��
	m_ZmnSize      = ZmnSize;															// �}�ʻ���
	memcpy(&m_Window,   Window,   sizeof(MgMinMax3));									// ����޳����
	memcpy(&m_ViewPort, ViewPort, sizeof(MgMinMax3));									// �ޭ��߰�
	m_FAFlag       = FAFlag;															// �ʂ̌�������   0:���Ȃ�    1:����
	m_MinMax.Min.X = m_MinMax.Min.Y = m_MinMax.Min.Z =  FLT_MAX;						// �ŏ���ő�
	m_MinMax.Max.X = m_MinMax.Max.Y = m_MinMax.Max.Z = -FLT_MAX;
	m_TextureY     = NULL;																// ÷����i���j
	m_VertBufY     = NULL;																// DirectX ���_�ޯ̧�i���j
	m_NumTriY      = 0;																	// �R�p�`�̐�       �i���j
	m_TextFlg      = 0;																	// ÷����׸�      0:�Ǘ�      1:����

	m_BlockList = new HlDList();									 					// ��ۯ���o�����߲��
	m_LayerList = new HlDList();									 					// ڲԥ�o�����߲��
	m_LinesList = new HlDList();									 					// ����o�����߲��
	m_FontsList = new HlDList();									 					// ̫�ĥ�o�����߲��

}

/******************************************************************************************************************
[ MDxfInf::~MDxfInf ]                �@�\  �c�w�e���f�X�g���N�^
******************************************************************************************************************/
MDxfInf::~MDxfInf          ()
{

	if (m_TextureY)  RELEASE(m_TextureY);												// ÷����i���j
	m_TextureY = NULL;

	if (m_VertBufY)  RELEASE(m_VertBufY);												// DirectX ���_�ޯ̧�i���j
	m_VertBufY = NULL;

	ClrBlock();
	HDELETE(m_BlockList);										 						// ��ۯ���o�����߲��

	ClrLayer();
	HDELETE(m_LayerList);										 						// ڲԥ�o�����߲��

	ClrLines();
	HDELETE(m_LinesList);										 						// ����o�����߲��

	ClrFonts();
	HDELETE(m_FontsList);										 						// ̫�ĥ�o�����߲��

}

/******************************************************************************************************************
[ MDxfInf::AddBlock ]                 �@�\  �c�w�e���Ƀu���b�N����ǉ�
******************************************************************************************************************/
MDxfBlock* MDxfInf::AddBlock ()														// (  O) ��ۯ����ݽ�ݽ
{
	MDxfBlock*			Ptr;															// ��ۯ����ݽ�ݽ


	Ptr = new MDxfBlock();																// ��ۯ����ݽ�׸�
	m_BlockList->Inject((HVOID *)Ptr);													// ��ۯ����ؽĂ֒ǉ�

	return(Ptr);
}

/******************************************************************************************************************
[ MDxfInf::AddLayer ]                 �@�\  �c�w�e���Ƀ��C������ǉ�
******************************************************************************************************************/
MINT MDxfInf::AddLayer        (														// (  O) �ð��  0:�ǉ�  1:�X�V
								MCHAR*				Name,								// (I  ) ڲԖ�
								MINT				Color,								// (I  ) �F�ԍ�
								MINT				LineNo)								// (I  ) ���ԍ�
{
	MINT				Status;
	MDxfLayer*			Ptr;															// ڲԏ��
	MINT				No;


	Status = 0;

	if (m_LayerList != NULL) {
		Ptr = (MDxfLayer *)m_LayerList->Head();										// �擪���߲���𓾂�
		No  = 0;
		while (Ptr != NULL) {
			if (BwsStrCmp(Name, Ptr->m_Name) == 0) {									// ڲԖ���v
				Ptr->Modify(Color, LineNo);
				Status = 1;																// �X�V
				break;
			}
			Ptr = (MDxfLayer *)m_LayerList->Next();									// �����߲���𓾂�
			No++;
		}
		if (Status == 0) {
			Ptr = new MDxfLayer(Name, Color, LineNo);									// ڲԏ��ݽ�׸�
			m_LayerList->Inject((HVOID *)Ptr);											// ڲԏ��ؽĂ֒ǉ�
		}
	}

	return(Status);
}

/******************************************************************************************************************
[ MDxfInf::AddLines ]                 �@�\  �c�w�e���ɐ�����ǉ�
******************************************************************************************************************/
MINT MDxfInf::AddLines    (															// (  O) �ð��  0:����  1:�װ
							MCHAR*				Name,									// (I  ) ����
							MINT				LineNo)									// (I  ) ���ԍ�
{
	MINT				Status;
	MDxfLines*			Ptr;															// �����


	Status = 0;

	Ptr    = new MDxfLines(Name, LineNo);												// �����ݽ�׸�
	m_LinesList->Inject((HVOID *)Ptr);													// �����ؽĂ֒ǉ�

	return(Status);
}

/******************************************************************************************************************
[ MDxfInf::AddFonts ]                 �@�\  �c�w�e���Ƀt�H���g����ǉ�
******************************************************************************************************************/
MINT MDxfInf::AddFonts    (															// (  O) �ð��  0:����  1:�װ
							MCHAR*				Name,									// (I  ) ̫�Ė�
							MINT				FCode)									// (I  ) ̫�ĺ���
{
	MINT				Status;
	MDxfFonts*			Ptr;															// ̫�ď��̍\����


	Status = 0;

	Ptr    = new MDxfFonts(Name, FCode);												// ̫�ď��ݽ�׸�
	m_FontsList->Inject((HVOID *)Ptr);													// ̫�ď��ؽĂ֒ǉ�

	return(Status);
}

/******************************************************************************************************************
[ MDxfInf::GetBlock ]                 �@�\  �c�w�e�����w�肵���u���b�N��������
******************************************************************************************************************/
MDxfBlock* MDxfInf::GetBlock (														// (  O) ��ۯ����  NULL:�����炸
								MCHAR*			Name,									// (I  ) ��ۯ���
								MCHAR*			LName,									// (  O) ڲԖ�
								MINT*			Color,									// (  O) �F�ԍ�
								MINT*			LineNo,									// (  O) ���ԍ�
								MgPoint3*		InsPnt)									// (  O) �}����_
{
	MDxfBlock*			Ptr;															// ��ۯ����


	if (m_BlockList != NULL) {
		Ptr = (MDxfBlock *)m_BlockList->Head();										// �擪���߲���𓾂�
		while (Ptr != NULL) {
			if (BwsStrCmp(Name, Ptr->m_Name) == 0) {									// ��ۯ�����v
				if (LName)   BwsStrCpy(LName, Ptr->m_LName);							// ڲԖ�
				if (Color)   *Color  = Ptr->m_Color;
				if (LineNo)  *LineNo = Ptr->m_LineNo;
				if (InsPnt)  memcpy(InsPnt, &Ptr->m_InsPnt, sizeof(MgPoint3));
				return(Ptr);
			}
			Ptr = (MDxfBlock *)m_BlockList->Next();									// �����߲���𓾂�
		}
	}

	return(NULL);																		// �����炸
}

/******************************************************************************************************************
[ MDxfInf::GetLayer ]                 �@�\  �c�w�e�����w�肵�����C����������
******************************************************************************************************************/
MDxfLayer* MDxfInf::GetLayer (														// (  O) ڲԏ��  NULL:�����炸
								MCHAR*			Name,									// (I  ) ڲԖ�
								MINT*			Color,									// (  O) �F�ԍ�
								MINT*			LineNo)									// (  O) ���ԍ�
{
	MDxfLayer*			Ptr;															// ڲԏ��


	if (m_LayerList != NULL) {
		Ptr = (MDxfLayer *)m_LayerList->Head();										// �擪���߲���𓾂�
		while (Ptr != NULL) {
			if (BwsStrCmp(Name, Ptr->m_Name) == 0) {									// ڲԖ���v
				if (Color)   *Color  = Ptr->m_Color;
				if (LineNo)  *LineNo = Ptr->m_LineNo;
				return(Ptr);
			}
			Ptr = (MDxfLayer *)m_LayerList->Next();									// �����߲���𓾂�
		}
	}

	Ptr = (MDxfLayer *)m_LayerList->Head();											// �擪���߲���𓾂�
	if (Color)   *Color  = Ptr->m_Color;
	if (LineNo)  *LineNo = Ptr->m_LineNo;
	return(Ptr);																		// �����炸
}

/******************************************************************************************************************
[ MDxfInf::GetLines ]                 �@�\  �c�w�e�����w�肵������������
******************************************************************************************************************/
MDxfLines* MDxfInf::GetLines (														// (  O) �����̲ݽ�ݽ
								MCHAR*			Name,									// (I  ) ����
								MINT*			LineNo)									// (  O) ���ԍ�
{
	MDxfLines*			Ptr;															// �����


	if (m_LinesList != NULL) {
		Ptr = (MDxfLines *)m_LinesList->Head();										// �擪���߲���𓾂�
		while (Ptr != NULL) {
			if (BwsStrCmp(Name, Ptr->m_Name) == 0) {									// ������v
				if (LineNo)  *LineNo = Ptr->m_LineNo;
				return(Ptr);
			}
			Ptr = (MDxfLines *)m_LinesList->Next();									// �����߲���𓾂�
		}
	}

	return(NULL);
}

/******************************************************************************************************************
[ MDxfInf::GetFonts ]                 �@�\  �c�w�e�����w�肵���t�H���g��������
******************************************************************************************************************/
MDxfFonts* MDxfInf::GetFonts (														// (  O) ̫�ď��̲ݽ�ݽ
								MCHAR*			Name,									// (I  ) ̫�Ė�
								MINT*			FCode)									// (  O) ̫�Ĕԍ�
{
	MDxfFonts*			Ptr;															// ̫�ď��


	if (m_FontsList != NULL) {
		Ptr = (MDxfFonts *)m_FontsList->Head();										// �擪���߲���𓾂�
		while (Ptr != NULL) {
			if (BwsStrCmp(Name, Ptr->m_Name) == 0) {									// ̫�Ė���v
				if (FCode)  *FCode = Ptr->m_FCode;
				return(Ptr);
			}
			Ptr = (MDxfFonts *)m_FontsList->Next();									// �����߲���𓾂�
		}
	}

	return(NULL);
}

/******************************************************************************************************************
[ MDxfInf::ClrBlock ]                 �@�\  �c�w�e���̃u���b�N�����N���A
******************************************************************************************************************/
HVOID MDxfInf::ClrBlock       ()
{
	MDxfBlock*			Ptr;															// ��ۯ����ւ��߲��


	if (m_BlockList != NULL) {
		Ptr = (MDxfBlock *)m_BlockList->Head();										// �擪���߲���𓾂�
		while (Ptr != NULL) {
			delete  Ptr;
			Ptr = (MDxfBlock *)m_BlockList->Next();									// �����߲���𓾂�
		}
	}
}

/******************************************************************************************************************
[ MDxfInf::ClrLayer ]                 �@�\  �c�w�e���̃��C�������N���A
******************************************************************************************************************/
HVOID MDxfInf::ClrLayer       ()
{
	MDxfLayer*			Ptr;															// ڲԏ��ւ��߲��


	if (m_LayerList != NULL) {
		Ptr = (MDxfLayer *)m_LayerList->Head();										// �擪���߲���𓾂�
		while (Ptr != NULL) {
			delete  Ptr;
			Ptr = (MDxfLayer *)m_LayerList->Next();									// �����߲���𓾂�
		}
	}
}

/******************************************************************************************************************
[ MDxfInf::ClrLines ]                 �@�\  �c�w�e���̐������N���A
******************************************************************************************************************/
HVOID MDxfInf::ClrLines       ()
{
	MDxfLines*			Ptr;															// �����ւ��߲��


	if (m_LinesList != NULL) {
		Ptr = (MDxfLines *)m_LinesList->Head();										// �擪���߲���𓾂�
		while (Ptr != NULL) {
			delete  Ptr;
			Ptr = (MDxfLines *)m_LinesList->Next();									// �����߲���𓾂�
		}
	}
}

/******************************************************************************************************************
[ MDxfInf::ClrFonts ]                 �@�\  �c�w�e���̃t�H���g�����N���A
******************************************************************************************************************/
HVOID MDxfInf::ClrFonts       ()
{
	MDxfFonts*			Ptr;															// ̫�ď��ւ��߲��


	if (m_FontsList != NULL) {
		Ptr = (MDxfFonts *)m_FontsList->Head();										// �擪���߲���𓾂�
		while (Ptr != NULL) {
			delete  Ptr;
			Ptr = (MDxfFonts *)m_FontsList->Next();									// �����߲���𓾂�
		}
	}
}

/******************************************************************************************************************
[ MDxfInf::Transform ]                �@�\  �c�w�e���̕ό`
******************************************************************************************************************/
MINT MDxfInf::Transform       (														// (  O) �ð��  0:����  1:�װ
								MINT				Symtry,								// (I  ) �Ώ��׸�  0:�Ȃ�  1:����
								HMTRX3				Matrix,								// (I  ) �ϊ���ظ�
								MgPoint3*			Move,								// (I  ) �ړ���
								MFLOAT*				Scale)								// (I  ) ����
{
	MINT				Status;
	MDxfLayer*			LayPtr;															// ڲԏ��
	MDxfBlock*			BlkPtr;
	MDxfPrim*			PrmPtr;


	Status = 0;

	m_Symtry = m_SymtryC = Symtry;
	if (Matrix)  memcpy(&m_Matrix, Matrix, sizeof(HMTRX3));
	if (Move)    memcpy(&m_Move,   Move,   sizeof(MgPoint3));
	if (Scale)   memcpy(&m_Scale,  Scale,  sizeof(MFLOAT)*3);

	if (m_LayerList != NULL) {
		LayPtr = (MDxfLayer *)m_LayerList->Head();										// ڲԏ��擪
		while (LayPtr != NULL) {
			if (LayPtr->m_PrimList != NULL) {
				PrmPtr = (MDxfPrim *)LayPtr->m_PrimList->Head();						// �}�`���擪
				while (PrmPtr != NULL) {
					PrmPtr->Transform(1, Symtry, m_Matrix, &m_Move, m_Scale, &m_MinMax);// �}�`�̕ό`
					PrmPtr = (MDxfPrim *)LayPtr->m_PrimList->Next();					// �}�`���
				}
			}
			LayPtr = (MDxfLayer *)m_LayerList->Next();									// ڲԏ��
		}
	}

	if (m_BlockList != NULL) {
		BlkPtr = (MDxfBlock *)m_BlockList->Head();										// ��ۯ����擪
		while (BlkPtr != NULL) {
			if (BlkPtr->m_PrimList != NULL) {
				PrmPtr = (MDxfPrim *)BlkPtr->m_PrimList->Head();						// �}�`���擪
				while (PrmPtr != NULL) {
					PrmPtr->Transform(1, Symtry, m_Matrix, &m_Move, m_Scale, &m_MinMax);// �}�`�̕ό`
					PrmPtr = (MDxfPrim *)BlkPtr->m_PrimList->Next();					// �}�`���
				}
			}
			BlkPtr = (MDxfBlock *)m_BlockList->Next();									// ��ۯ����
		}
	}

	return(Status);
}

/******************************************************************************************************************
[ MDxfInf::TransformYZ ]              �@�\  �c�w�e���̕ό`�i�x���y�j
******************************************************************************************************************/
MINT MDxfInf::TransformYZ     ()														// (  O) �ð��  0:����  1:�װ
{
	MINT				Status;
	MDxfLayer*			LayPtr;															// ڲԏ��
	MDxfBlock*			BlkPtr;
	MDxfPrim*			PrmPtr;


	Status = 0;

	if (m_LayerList != NULL) {
		LayPtr = (MDxfLayer *)m_LayerList->Head();										// ڲԏ��擪
		while (LayPtr != NULL) {
			if (LayPtr->m_PrimList != NULL) {
				PrmPtr = (MDxfPrim *)LayPtr->m_PrimList->Head();						// �}�`���擪
				while (PrmPtr != NULL) {
					PrmPtr->TransformYZ(&m_MinMax);										// �}�`�̕ό`
					PrmPtr = (MDxfPrim *)LayPtr->m_PrimList->Next();					// �}�`���
				}
			}
			LayPtr = (MDxfLayer *)m_LayerList->Next();									// ڲԏ��
		}
	}

	if (m_BlockList != NULL) {
		BlkPtr = (MDxfBlock *)m_BlockList->Head();										// ��ۯ����擪
		while (BlkPtr != NULL) {
			if (BlkPtr->m_PrimList != NULL) {
				PrmPtr = (MDxfPrim *)BlkPtr->m_PrimList->Head();						// �}�`���擪
				while (PrmPtr != NULL) {
					PrmPtr->TransformYZ(&m_MinMax);										// �}�`�̕ό`
					PrmPtr = (MDxfPrim *)BlkPtr->m_PrimList->Next();					// �}�`���
				}
			}
			BlkPtr = (MDxfBlock *)m_BlockList->Next();									// ��ۯ����
		}
	}

	return(Status);
}

/******************************************************************************************************************
[ MDxfInf::GetMinMax ]                �@�\  �ŏ��ő�𓾂�
******************************************************************************************************************/
HVOID MDxfInf::GetMinMax      (														// (  O) �ð��  0:����  1:�װ
								MgMinMax3*			MinMax)								// (  O) �ŏ���ő�
{
	memcpy(MinMax, &m_MinMax, sizeof(MgMinMax3));

	return;
}

/******************************************************************************************************************
[ MDxfInf::GetMinMax ]                �@�\  ��]�E�ړ���̍ŏ��ő�𓾂�
******************************************************************************************************************/
HVOID MDxfInf::GetMinMax      (														// (  O) �ð��  0:����  1:�װ
								MINT				Symtry,								// (I  ) �Ώ��׸�  0:�Ȃ�  1:����
								HMTRX3				Matrix,								// (I  ) �ϊ���ظ�
								MgPoint3*			Move,								// (I  ) �ړ���
								MFLOAT*				Scale,								// (I  ) ����
								MgMinMax3*			MinMax)								// (  O) �ŏ���ő�
{
	HMTRX3				WMatrix;														// �ϊ���ظ�
	MgPoint3				WMove;															// �ړ���
	MFLOAT				WScale[3];														// ����
	MDxfLayer*			LayPtr;															// ڲԏ��
	MDxfBlock*			BlkPtr;
	MDxfPrim*			PrmPtr;


	if (Matrix) {
		memcpy(WMatrix, Matrix, sizeof(HMTRX3));
	} else {
		WMatrix[0][0] = 1.f; WMatrix[0][1] = 0.f; WMatrix[0][2] = 0.f;
		WMatrix[1][0] = 0.f; WMatrix[1][1] = 1.f; WMatrix[1][2] = 0.f;
		WMatrix[2][0] = 0.f; WMatrix[2][1] = 0.f; WMatrix[2][2] = 1.f;
	}
	if (Move) {
		memcpy(&WMove, Move, sizeof(MgPoint3));
	} else {
		WMove.X = WMove.Y = WMove.Z = 0.f;
	}
	if (Scale) {
		memcpy(&WScale, Scale, sizeof(MFLOAT)*3);
	} else {
		WScale[0] = WScale[1] = WScale[2] = 1.0f;
	}

	MinMax->Min.X = MinMax->Min.Y = MinMax->Min.Z =  FLT_MAX;
	MinMax->Max.X = MinMax->Max.Y = MinMax->Max.Z = -FLT_MAX;

	if (m_LayerList != NULL) {
		LayPtr = (MDxfLayer *)m_LayerList->Head();										// ڲԏ��擪
		while (LayPtr != NULL) {
			if (LayPtr->m_PrimList != NULL) {
				PrmPtr = (MDxfPrim *)LayPtr->m_PrimList->Head();						// �}�`���擪
				while (PrmPtr != NULL) {
					PrmPtr->Transform(0, Symtry, WMatrix, &WMove, WScale, MinMax);		// �}�`�̕ό`
					PrmPtr = (MDxfPrim *)LayPtr->m_PrimList->Next();					// �}�`���
				}
			}
			LayPtr = (MDxfLayer *)m_LayerList->Next();									// ڲԏ��
		}
	}

	if (m_BlockList != NULL) {
		BlkPtr = (MDxfBlock *)m_BlockList->Head();										// ��ۯ����擪
		while (BlkPtr != NULL) {
			if (BlkPtr->m_PrimList != NULL) {
				PrmPtr = (MDxfPrim *)BlkPtr->m_PrimList->Head();						// �}�`���擪
				while (PrmPtr != NULL) {
					PrmPtr->Transform(0, Symtry, WMatrix, &WMove, WScale, MinMax);		// �}�`�̕ό`
					PrmPtr = (MDxfPrim *)BlkPtr->m_PrimList->Next();					// �}�`���
				}
			}
			BlkPtr = (MDxfBlock *)m_BlockList->Next();									// ��ۯ����
		}
	}

	return;
}

/******************************************************************************************************************
[ MDxfInf::GetMinMaxF ]               �@�\  �ŏ��ő���Ȃ�ʂ𓾂�
******************************************************************************************************************/
HVOID MDxfInf::GetMinMaxF     (														// (  O) �ð��  0:����  1:�װ
								MINT				VewTyp,								// (I  ) �����w���E��
								HMTRX3				Matrix,								// (I  ) �ϊ���ظ�
								MgPoint3*			Move,								// (I  ) �ړ���
								MgPoint3*			Pnt1)								// (  O) �ŏ���ő�̗̈�(4�_)
{
	HMTRX3				MatrixW;														// �ϊ���ظ�
	MgPoint3				MoveW;															// �ړ���
	MINT				ic;


	if (Matrix) {
		memcpy(MatrixW, Matrix, sizeof(HMTRX3));
	} else {
		MatrixW[0][0] = 1.f; MatrixW[0][1] = 0.f; MatrixW[0][2] = 0.f;
		MatrixW[1][0] = 0.f; MatrixW[1][1] = 1.f; MatrixW[1][2] = 0.f;
		MatrixW[2][0] = 0.f; MatrixW[2][1] = 0.f; MatrixW[2][2] = 1.f;
	}
	if (Move) {
		memcpy(&MoveW, Move, sizeof(MgPoint3));
	} else {
		MoveW.X = MoveW.Y = MoveW.Z = 0.f;
	}

	if (VewTyp == HVIEWT_HEI) {															// ����    (1)
		Pnt1[0].X = m_MinMax.Min.X; Pnt1[0].Y = m_MinMax.Min.Y; Pnt1[0].Z = m_MinMax.Max.Z;
		Pnt1[1].X = m_MinMax.Max.X; Pnt1[1].Y = m_MinMax.Min.Y; Pnt1[1].Z = m_MinMax.Max.Z;
		Pnt1[2].X = m_MinMax.Max.X; Pnt1[2].Y = m_MinMax.Max.Y; Pnt1[2].Z = m_MinMax.Max.Z;
		Pnt1[3].X = m_MinMax.Min.X; Pnt1[3].Y = m_MinMax.Max.Y; Pnt1[3].Z = m_MinMax.Max.Z;
	} else if (VewTyp == HVIEWT_SYO) {													// ����    (2)
		Pnt1[0].X = m_MinMax.Min.X; Pnt1[0].Y = m_MinMax.Min.Y; Pnt1[0].Z = m_MinMax.Min.Z;
		Pnt1[1].X = m_MinMax.Max.X; Pnt1[1].Y = m_MinMax.Min.Y; Pnt1[1].Z = m_MinMax.Min.Z;
		Pnt1[2].X = m_MinMax.Max.X; Pnt1[2].Y = m_MinMax.Min.Y; Pnt1[2].Z = m_MinMax.Max.Z;
		Pnt1[3].X = m_MinMax.Min.X; Pnt1[3].Y = m_MinMax.Min.Y; Pnt1[3].Z = m_MinMax.Max.Z;
	} else if (VewTyp == HVIEWT_HAI) {													// �w��    (3)
		Pnt1[0].X = m_MinMax.Max.X; Pnt1[0].Y = m_MinMax.Max.Y; Pnt1[0].Z = m_MinMax.Min.Z;
		Pnt1[1].X = m_MinMax.Min.X; Pnt1[1].Y = m_MinMax.Max.Y; Pnt1[1].Z = m_MinMax.Min.Z;
		Pnt1[2].X = m_MinMax.Min.X; Pnt1[2].Y = m_MinMax.Max.Y; Pnt1[2].Z = m_MinMax.Max.Z;
		Pnt1[3].X = m_MinMax.Max.X; Pnt1[3].Y = m_MinMax.Max.Y; Pnt1[3].Z = m_MinMax.Max.Z;
	} else if (VewTyp == HVIEWT_LSK) {													// ������  (4)
		Pnt1[0].X = m_MinMax.Min.X; Pnt1[0].Y = m_MinMax.Max.Y; Pnt1[0].Z = m_MinMax.Min.Z;
		Pnt1[1].X = m_MinMax.Min.X; Pnt1[1].Y = m_MinMax.Min.Y; Pnt1[1].Z = m_MinMax.Min.Z;
		Pnt1[2].X = m_MinMax.Min.X; Pnt1[2].Y = m_MinMax.Min.Y; Pnt1[2].Z = m_MinMax.Max.Z;
		Pnt1[3].X = m_MinMax.Min.X; Pnt1[3].Y = m_MinMax.Max.Y; Pnt1[3].Z = m_MinMax.Max.Z;
	} else if (VewTyp == HVIEWT_RSK) {													// �E����  (5)
		Pnt1[0].X = m_MinMax.Max.X; Pnt1[0].Y = m_MinMax.Min.Y; Pnt1[0].Z = m_MinMax.Min.Z;
		Pnt1[1].X = m_MinMax.Max.X; Pnt1[1].Y = m_MinMax.Max.Y; Pnt1[1].Z = m_MinMax.Min.Z;
		Pnt1[2].X = m_MinMax.Max.X; Pnt1[2].Y = m_MinMax.Max.Y; Pnt1[2].Z = m_MinMax.Max.Z;
		Pnt1[3].X = m_MinMax.Max.X; Pnt1[3].Y = m_MinMax.Min.Y; Pnt1[3].Z = m_MinMax.Max.Z;
	} else if (VewTyp == HVIEWT_URA) {													// ����    (6)
		Pnt1[0].X = m_MinMax.Max.X; Pnt1[0].Y = m_MinMax.Min.Y; Pnt1[0].Z = m_MinMax.Min.Z;
		Pnt1[1].X = m_MinMax.Min.X; Pnt1[1].Y = m_MinMax.Min.Y; Pnt1[1].Z = m_MinMax.Min.Z;
		Pnt1[2].X = m_MinMax.Min.X; Pnt1[2].Y = m_MinMax.Max.Y; Pnt1[2].Z = m_MinMax.Min.Z;
		Pnt1[3].X = m_MinMax.Max.X; Pnt1[3].Y = m_MinMax.Max.Y; Pnt1[3].Z = m_MinMax.Min.Z;
	}

	for (ic = 0 ; ic < 4 ; ic++) {
		HgTrnPntByMtx(&Pnt1[ic], MatrixW, &MoveW, &Pnt1[ic]);
	}

	return;
}

/******************************************************************************************************************
[ MDxfInf::Show ]                     �@�\  �c�w�e����\������
******************************************************************************************************************/
MINT MDxfInf::Show            (														// (  O) �ð��  0:����  1:�װ
								MINT				MainId,								// (I  ) Ҳ�ID
								MINT				SubsId,								// (I  ) ���ID
								COLORREF			Color,								// (I  ) �\���F
								MINT				Symtry,								// (I  ) �Ώ��׸�  0:�Ȃ�  1:����
								HMTRX3				Matrix,								// (I  ) �ϊ���ظ�
								MgPoint3*			Move,								// (I  ) �ړ���
								MFLOAT				Scale,								// (I  ) ����
								MgPoint2*			CutPnt,								// (I  ) ��Đ� + �����_
								MgMinMax3*			MinMax)								// (  O) �ŏ���ő�
{
	MINT				Status;
	HMTRX3				WMatrix;														// �ϊ���ظ�
	MgPoint3				WMove;															// �ړ���
	MgMinMax3			WMinMax;														// �ŏ���ő�
	MDxfLayer*			LayPtr;															// ڲԏ��
	MDxfBlock*			BlkPtr;
	MDxfPrim*			PrmPtr;
	CDC*				MemDC;															// (I  ) ���DC
	MgPoint2				OPnt[2];


	Status = 0;

	if (Matrix) {
		memcpy(&WMatrix, Matrix, sizeof(HMTRX3));
	} else {
		WMatrix[0][0] = 1.f; WMatrix[0][1] = 0.f; WMatrix[0][2] = 0.f;
		WMatrix[1][0] = 0.f; WMatrix[1][1] = 1.f; WMatrix[1][2] = 0.f;
		WMatrix[2][0] = 0.f; WMatrix[2][1] = 0.f; WMatrix[2][2] = 1.f;
	}
	if (Move) {
		memcpy(&WMove, Move, sizeof(MgPoint3));
	} else {
		WMove.X = WMove.Y = WMove.Z = 0.f;
	}

	WMinMax.Min.X = WMinMax.Min.Y = WMinMax.Min.Z =  FLT_MAX;
	WMinMax.Max.X = WMinMax.Max.Y = WMinMax.Max.Z = -FLT_MAX;

	Status = HsViewCntlGet(MainId, SubsId, NULL, &MemDC);								// �ޭ�������
	if (Status == 0) {
		if (m_LayerList != NULL) {
			LayPtr = (MDxfLayer *)m_LayerList->Head();									// ڲԏ��擪
			while (LayPtr != NULL) {
				if (LayPtr->m_PrimList != NULL) {
					PrmPtr = (MDxfPrim *)LayPtr->m_PrimList->Head();					// �}�`���擪
					while (PrmPtr != NULL) {
						PrmPtr->Show(MemDC, MainId, SubsId, Color, Symtry, WMatrix,
									 &WMove, Scale, CutPnt, &WMinMax);					// �\��
						PrmPtr = (MDxfPrim *)LayPtr->m_PrimList->Next();				// �}�`���
					}
				}
				LayPtr = (MDxfLayer *)m_LayerList->Next();								// ڲԏ��
			}
		}

		if (m_BlockList != NULL) {
			BlkPtr = (MDxfBlock *)m_BlockList->Head();									// ��ۯ����擪
			while (BlkPtr != NULL) {
				if (BlkPtr->m_PrimList != NULL) {
					PrmPtr = (MDxfPrim *)BlkPtr->m_PrimList->Head();					// �}�`���擪
					while (PrmPtr != NULL) {
						PrmPtr->Show(MemDC, MainId, SubsId, Color, Symtry, WMatrix,
									 &WMove, Scale, CutPnt, &WMinMax);					// �\��
						PrmPtr = (MDxfPrim *)BlkPtr->m_PrimList->Next();				// �}�`���
					}
				}
				BlkPtr = (MDxfBlock *)m_BlockList->Next();								// ��ۯ����
			}
		}

		if (MinMax)  memcpy(MinMax, &WMinMax, sizeof(MgMinMax3));

		if (MainId == 9) {																// �⏕�͌��_��\��
			OPnt[0].X = OPnt[0].Y = 0.f;
			HgTrnPntByMtx(&OPnt[0], WMatrix, &WMove, &OPnt[1]);
			HnMark(MemDC, MainId, SubsId, 2, 1, 0, 5, OCOLOR, 1, &OPnt[1]);				// �w��`��
		}
		if (CutPnt) {																	// ��Đ�����
			HsCutShowLine(MemDC, MainId, SubsId, HLINE_SOLD, Color, CutPnt);			// ��Đ���`��
		}
	}

	return(Status);
}

/******************************************************************************************************************
[ MDxfInf::MakeRL ]                   �@�\  ���ʏ��̍쐬�i���j
******************************************************************************************************************/
MINT MDxfInf::MakeRL          (														// (  O) �ð��  0:����  1:�װ
								MINT				VewCod,								// (I  ) �ޭ����� 0:�� 1:�� 2:�� 3:�k
								MINT				Kai,								// (I  ) �K
								HnDisp*				DispRL,								// (I  ) ���ʕ\�����i�����j
								COLORREF			Rgb,								// (I  ) �\���F
								MINT				Symtry,								// (I  ) �Ώ��׸�  0:�Ȃ�  1:����
								HMTRX3				Matrix,								// (I  ) �ϊ���ظ�
								MgPoint3*			Move,								// (I  ) �ړ���
								MFLOAT				Scale,								// (I  ) ����
								MgPoint3*			TOPnt,								// (I  ) 2������3�����ϊ��p���_
								MgPoint3*			TUvw,								// (I  ) 2������3�����ϊ��pUVW
								MgMinMax3*			MinMax)								// (  O) �ŏ���ő�
{
	MINT				Status;
	HMTRX3				WMatrix;														// �ϊ���ظ�
	MgPoint3				WMove;															// �ړ���
	MgMinMax3			WMinMax;														// �ŏ���ő�
	MDxfLayer*			LayPtr;															// ڲԏ��
	MDxfBlock*			BlkPtr;
	MDxfPrim*			PrmPtr;


	Status = 0;

	if (Matrix) {
		memcpy(&WMatrix, Matrix, sizeof(HMTRX3));
	} else {
		WMatrix[0][0] = 1.f; WMatrix[0][1] = 0.f; WMatrix[0][2] = 0.f;
		WMatrix[1][0] = 0.f; WMatrix[1][1] = 1.f; WMatrix[1][2] = 0.f;
		WMatrix[2][0] = 0.f; WMatrix[2][1] = 0.f; WMatrix[2][2] = 1.f;
	}
	if (Move) {
		memcpy(&WMove, Move, sizeof(MgPoint3));
	} else {
		WMove.X = WMove.Y = WMove.Z = 0.f;
	}

	WMinMax.Min.X = WMinMax.Min.Y = WMinMax.Min.Z =  FLT_MAX;
	WMinMax.Max.X = WMinMax.Max.Y = WMinMax.Max.Z = -FLT_MAX;

	if (m_LayerList != NULL) {
		LayPtr = (MDxfLayer *)m_LayerList->Head();									// ڲԏ��擪
		while (LayPtr != NULL) {
			if (LayPtr->m_PrimList != NULL) {
				PrmPtr = (MDxfPrim *)LayPtr->m_PrimList->Head();					// �}�`���擪
				while (PrmPtr != NULL) {
					PrmPtr->MakeRL(VewCod, Kai, DispRL, Rgb, Symtry, WMatrix, &WMove, Scale, TOPnt, TUvw, &WMinMax);	// ���ʏ��̍쐬(��)
					PrmPtr = (MDxfPrim *)LayPtr->m_PrimList->Next();				// �}�`���
				}
			}
			LayPtr = (MDxfLayer *)m_LayerList->Next();								// ڲԏ��
		}
	}

	if (m_BlockList != NULL) {
		BlkPtr = (MDxfBlock *)m_BlockList->Head();									// ��ۯ����擪
		while (BlkPtr != NULL) {
			if (BlkPtr->m_PrimList != NULL) {
				PrmPtr = (MDxfPrim *)BlkPtr->m_PrimList->Head();					// �}�`���擪
				while (PrmPtr != NULL) {
					PrmPtr->MakeRL(VewCod, Kai, DispRL, Rgb, Symtry, WMatrix, &WMove, Scale, TOPnt, TUvw, &WMinMax);	// ���ʏ��̍쐬(��)
					PrmPtr = (MDxfPrim *)BlkPtr->m_PrimList->Next();				// �}�`���
				}
			}
			BlkPtr = (MDxfBlock *)m_BlockList->Next();								// ��ۯ����
		}
	}

	if (MinMax)  memcpy(MinMax, &WMinMax, sizeof(MgMinMax3));

	return(Status);
}

/******************************************************************************************************************
[ MDxfInf::NumDxfPrim ]               �@�\  �v���~�e�B�u���𓾂�
******************************************************************************************************************/
MINT MDxfInf::NumDxfPrim      (														// (  O) ����è�ސ�
								MINT				Symtry,								// (I  ) �Ώ��׸�  0:�Ȃ�  1:����
								HMTRX3				Matrix,								// (I  ) �ϊ���ظ�
								MgPoint3*			Move,								// (I  ) �ړ���
								MFLOAT				Scale,								// (I  ) ����
								MgPoint2*			CutPnt)								// (I  ) ��Đ� + �����_
{
	MINT				Num;
	HMTRX3				WMatrix;														// �ϊ���ظ�
	MgPoint3				WMove;															// �ړ���
	MDxfLayer*			LayPtr;															// ڲԏ��
	MDxfBlock*			BlkPtr;
	MDxfPrim*			PrmPtr;


	Num = 0;

	if (Matrix) {
		memcpy(&WMatrix, Matrix, sizeof(HMTRX3));
	} else {
		WMatrix[0][0] = 1.f; WMatrix[0][1] = 0.f; WMatrix[0][2] = 0.f;
		WMatrix[1][0] = 0.f; WMatrix[1][1] = 1.f; WMatrix[1][2] = 0.f;
		WMatrix[2][0] = 0.f; WMatrix[2][1] = 0.f; WMatrix[2][2] = 1.f;
	}
	if (Move) {
		memcpy(&WMove, Move, sizeof(MgPoint3));
	} else {
		WMove.X = WMove.Y = WMove.Z = 0.f;
	}

	if (m_LayerList != NULL) {
		LayPtr = (MDxfLayer *)m_LayerList->Head();										// ڲԏ��擪
		while (LayPtr != NULL) {
			if (LayPtr->m_PrimList != NULL) {
				PrmPtr = (MDxfPrim *)LayPtr->m_PrimList->Head();						// �}�`���擪
				while (PrmPtr != NULL) {
					Num += PrmPtr->NumDxfPrim(Symtry, WMatrix, &WMove, Scale, CutPnt);	// ����è�ސ��𓾂�
					PrmPtr = (MDxfPrim *)LayPtr->m_PrimList->Next();					// �}�`���
				}
			}
			LayPtr = (MDxfLayer *)m_LayerList->Next();									// ڲԏ��
		}
	}

	if (m_BlockList != NULL) {
		BlkPtr = (MDxfBlock *)m_BlockList->Head();										// ��ۯ����擪
		while (BlkPtr != NULL) {
			if (BlkPtr->m_PrimList != NULL) {
				PrmPtr = (MDxfPrim *)BlkPtr->m_PrimList->Head();						// �}�`���擪
				while (PrmPtr != NULL) {
					Num += PrmPtr->NumDxfPrim(Symtry, WMatrix, &WMove, Scale, CutPnt);	// ����è�ސ��𓾂�
					PrmPtr = (MDxfPrim *)BlkPtr->m_PrimList->Next();					// �}�`���
				}
			}
			BlkPtr = (MDxfBlock *)m_BlockList->Next();									// ��ۯ����
		}
	}

	if (CutPnt) {																		// ��Đ�����
		Num += HsCutNumDxfPrim(CutPnt);													// ��Đ�������è�ސ�
	}

	return(Num);
}

/******************************************************************************************************************
[ MDxfInf::DxfR14Out ]                �@�\   �c�w�e�i�q�P�S�j�`���o��
******************************************************************************************************************/
MINT MDxfInf::DxfR14Out       (														// (  O) �ð��  0:���� �ȊO:�װ
								CStdioFile*			FLPtr,								// (I  ) ̧�َ��ʎq
								MINT*				NumHnd,								// (I/O) ����ِ�
								MCHAR*				LayNam,								// (I  ) ڲԖ�
								MINT				Symtry,								// (I  ) �Ώ��׸�  0:�Ȃ�  1:����
								HMTRX3				Matrix,								// (I  ) �ϊ���ظ�
								MgPoint3*			Move,								// (I  ) �ړ���
								MFLOAT				Scale,								// (I  ) ����
								MgPoint2*			CutPnt)								// (I  ) ��Đ� + �����_
{
	MINT				Status;
	HMTRX3				WMatrix;														// �ϊ���ظ�
	MgPoint3				WMove;															// �ړ���
	MDxfLayer*			LayPtr;															// ڲԏ��
	MDxfBlock*			BlkPtr;
	MDxfPrim*			PrmPtr;


	Status = 0;

	if (Matrix) {
		memcpy(&WMatrix, Matrix, sizeof(HMTRX3));
	} else {
		WMatrix[0][0] = 1.f; WMatrix[0][1] = 0.f; WMatrix[0][2] = 0.f;
		WMatrix[1][0] = 0.f; WMatrix[1][1] = 1.f; WMatrix[1][2] = 0.f;
		WMatrix[2][0] = 0.f; WMatrix[2][1] = 0.f; WMatrix[2][2] = 1.f;
	}
	if (Move) {
		memcpy(&WMove, Move, sizeof(MgPoint3));
	} else {
		WMove.X = WMove.Y = WMove.Z = 0.f;
	}

	if (m_LayerList != NULL) {
		LayPtr = (MDxfLayer *)m_LayerList->Head();										// ڲԏ��擪
		while (LayPtr != NULL) {
			if (LayPtr->m_PrimList != NULL) {
				PrmPtr = (MDxfPrim *)LayPtr->m_PrimList->Head();						// �}�`���擪
				while (PrmPtr != NULL) {
					Status = PrmPtr->DxfR14Out(FLPtr, NumHnd, LayNam, Symtry, WMatrix, &WMove, Scale, CutPnt);	// DXF(R14)�`���o��
					PrmPtr = (MDxfPrim *)LayPtr->m_PrimList->Next();					// �}�`���
				}
			}
			LayPtr = (MDxfLayer *)m_LayerList->Next();									// ڲԏ��
		}
	}

	if (m_BlockList != NULL) {
		BlkPtr = (MDxfBlock *)m_BlockList->Head();										// ��ۯ����擪
		while (BlkPtr != NULL) {
			if (BlkPtr->m_PrimList != NULL) {
				PrmPtr = (MDxfPrim *)BlkPtr->m_PrimList->Head();						// �}�`���擪
				while (PrmPtr != NULL) {
					Status = PrmPtr->DxfR14Out(FLPtr, NumHnd, LayNam, Symtry, WMatrix, &WMove, Scale, CutPnt);	// DXF(R14)�`���o��
					PrmPtr = (MDxfPrim *)BlkPtr->m_PrimList->Next();					// �}�`���
				}
			}
			BlkPtr = (MDxfBlock *)m_BlockList->Next();									// ��ۯ����
		}
	}

	if (CutPnt) {																		// ��Đ�����
		Status = HsCutDxfR14Out(FLPtr, NumHnd, LayNam, HLINE_SOLD, CutPnt);				// ��Đ���`��
	}

	return(Status);
}

/******************************************************************************************************************
[ MDxfInf::GetLayBlkNum ]             �@�\  ���C���E�u���b�N���𓾂�
******************************************************************************************************************/
MINT MDxfInf::GetLayBlkNum    ()														// (  O) ڲԥ��ۯ���
{
	MINT				Num;


	Num = (MINT)m_LayerList->Number() + (MINT)m_BlockList->Number();

	return(Num);
}

/******************************************************************************************************************
[ MDxfInf::GetLayBlkInf ]             �@�\  ���C���E�u���b�N�̌���
******************************************************************************************************************/
MINT MDxfInf::GetLayBlkInf    (														// (  O) �ð��  0:����  1:�װ
								MINT				Idx,								// (I  ) ���ޯ��
								MCHAR*				Name,								// (  O) ڲԥ��ۯ���
								COLORREF*			Rgb)								// (  O) RGB�װ�l
{
	MINT				Status;
	MINT				No;
	MDxfLayer*			LayPtr;															// ڲԏ��
	MDxfBlock*			BlkPtr;


	Status = 1;																			// �Ȃ�
	No     = Idx;

	if (No < (MINT)m_LayerList->Number()) {
		LayPtr = (MDxfLayer *)m_LayerList->Access(No);									// �߲���𓾂�
		BwsStrCpy(Name, LayPtr->m_Name);
		if (Rgb)  *Rgb    = LayPtr->m_Rgb;
		Status = 0;
	}
	No -= (MINT)m_LayerList->Number();

	if (Status == 1) {
		if (No < (MINT)m_BlockList->Number()) {
			BlkPtr = (MDxfBlock *)m_BlockList->Access(No);								// �߲���𓾂�
			BwsStrCpy(Name, BlkPtr->m_Name);
			if (Rgb)  *Rgb = BlkPtr->m_Rgb;
			Status = 0;
		}
	}

	return(Status);
}

/******************************************************************************************************************
[ MDxfInf::SetLayBlkRgb ]             �@�\  ���C���E�u���b�N�ɂq�f�a��ݒ肷��
******************************************************************************************************************/
MINT MDxfInf::SetLayBlkRgb    (														// (  O) �ð��  0:����  1:�װ
								MCHAR*				Name,								// (I  ) ڲԥ��ۯ���
								COLORREF			Rgb)								// (I  ) RGB�װ�l
{
	MINT				Status;
	MDxfLayer*			LayPtr;															// ڲԏ��
	MDxfBlock*			BlkPtr;


	if (m_LayerList != NULL) {
		LayPtr = (MDxfLayer *)m_LayerList->Head();										// �擪���߲���𓾂�
		while (LayPtr != NULL) {
			if (BwsStrCmp(Name, LayPtr->m_Name) == 0) {									// ��ۯ�����v
				LayPtr->m_Rgb = Rgb;
				Status = 0;
				break;
			}
			LayPtr = (MDxfLayer *)m_LayerList->Next();									// �����߲���𓾂�
		}
	}

	if (m_BlockList != NULL) {
		BlkPtr = (MDxfBlock *)m_BlockList->Head();										// �擪���߲���𓾂�
		while (BlkPtr != NULL) {
			if (BwsStrCmp(Name, BlkPtr->m_Name) == 0) {									// ��ۯ�����v
				BlkPtr->m_Rgb = Rgb;
				Status = 0;
				break;
			}
			BlkPtr = (MDxfBlock *)m_BlockList->Next();									// �����߲���𓾂�
		}
	}

	return(Status);
}

/******************************************************************************************************************
[ MDxfInf::SetDirectX ]               �@�\  �c�w�e�����c�����������w�ɐݒ肷��
******************************************************************************************************************/
MINT MDxfInf::SetDirectX      (														// (  O) �ð��  0:����  1:�װ
								MINT				DMode,								// (I  ) 0;�� 1:�� 2:����
								LPDIRECT3DDEVICE8	LpD3DDEV,							// (I  ) DirectX���޲�
								HstackTexture*		TextList,							// (I  ) ÷���ؽ�
								HstackShowObj*		ShowList,							// (I  ) �\���Ώ�ؽ�
								MCHAR				TexNam[50][64],						// (I  ) ÷�����
								COLORREF*			RgbInf,								// (I  ) �\���F
								MINT				ArrFlg,								// (I  ) ���\���׸�  0:�Ȃ� 1:����
								MINT				Symtry,								// (I  ) �Ώ��׸�  0:�Ȃ�  1:����
								HMTRX3				Matrix,								// (I  ) �ϊ���ظ�
								MgPoint3*			Move,								// (I  ) �ړ���
								MgMinMax3*			MinMax)								// (  O) �ŏ���ő�
{
	HMTRX3				WMatrix;														// �ϊ���ظ�
	MgPoint3				WMove;															// �ړ���
	MINT				Status;
	MDxfLayer*			LayPtr;															// ڲԏ��
	MDxfBlock*			BlkPtr;
	MDxfPrim*			PrmPtr;
	MgMinMax3			WMinMax;														// �ŏ���ő�
	MCHAR				PasNam[256];
	MINT				No;
	MINT				SymW;
	MINT				ItrSts;


	Status = 0;

	if (Matrix) {
		memcpy(&WMatrix, Matrix, sizeof(HMTRX3));
	} else {
		WMatrix[0][0] = 1.f; WMatrix[0][1] = 0.f; WMatrix[0][2] = 0.f;
		WMatrix[1][0] = 0.f; WMatrix[1][1] = 1.f; WMatrix[1][2] = 0.f;
		WMatrix[2][0] = 0.f; WMatrix[2][1] = 0.f; WMatrix[2][2] = 1.f;
	}
	if (Move) {
		memcpy(&WMove, Move, sizeof(MgPoint3));
	} else {
		WMove.X = WMove.Y = WMove.Z = 0.f;
	}
	if (Matrix) {
		m_Symtry = m_SymtryC = SymW = Symtry;
	} else {
		SymW = 0;
	}

	WMinMax.Min.X = WMinMax.Min.Y = WMinMax.Min.Z =  FLT_MAX;							// �ŏ���ő�
	WMinMax.Max.X = WMinMax.Max.Y = WMinMax.Max.Z = -FLT_MAX;

	if (m_LayerList != NULL) {
		No = 0;
		LayPtr = (MDxfLayer *)m_LayerList->Head();										// ڲԏ��擪
		while (LayPtr != NULL) {
			if (TexNam && BwsStrCmp(TexNam[No], BwsStr("")) != 0) {
				ItrSts = HeAddEnvNam(3, TexNam[No], PasNam);							// ���ϐ����������߽�𓾂�
			} else {
				if (m_VewTyp == HVIEWT_3D) {
					ItrSts = HeAddEnvNam(3, BwsStr("�q�f�a/250250250.bmp"), PasNam);	// ���ϐ����������߽�𓾂�
				} else {
					ItrSts = HeAddEnvNam(3, BwsStr("�q�f�a/070070070.bmp"), PasNam);	// ���ϐ����������߽�𓾂�
				}
			}
			if (TextList) {
				LayPtr->m_Texture = TextList->Add(PasNam);								// ÷����ǉ�
				m_TextFlg = 0;															// ÷����׸�      0:�Ǘ�      1:����
			} else {
				HsDirectSetTexture(PasNam, &LayPtr->m_Texture);							// ÷������
				m_TextFlg = 1;															// ÷����׸�      0:�Ǘ�      1:����
			}
			if (RgbInf)  LayPtr->m_Rgb = RgbInf[No];

			if (LayPtr->m_PrimList != NULL) {
				PrmPtr = (MDxfPrim *)LayPtr->m_PrimList->Head();						// �}�`���擪
				while (PrmPtr != NULL) {
					PrmPtr->Transform(1, SymW, WMatrix, &WMove, NULL, &WMinMax);		// �}�`�̕ό`
					if (DMode == 1 || DMode == 2)  PrmPtr->SetDirectXF(LpD3DDEV, LayPtr->m_Rgb, m_FAFlag);	// DirectX�ɾ��
					if (DMode == 0 || DMode == 2)  PrmPtr->SetDirectXE(LpD3DDEV, m_Symtry);					// DirectX�ɾ��
					PrmPtr = (MDxfPrim *)LayPtr->m_PrimList->Next();					// �}�`���
				}
			}
			if (ShowList) {
				if (m_VewTyp == HVIEWT_3D) {
					ShowList->Add(LayPtr->m_Texture, 13, (HDBID)this);				// �\���ǉ�
				} else {
					ShowList->Add(LayPtr->m_Texture, 14, (HDBID)this);				// �\���ǉ�
				}
			}
			No++;
			LayPtr = (MDxfLayer *)m_LayerList->Next();									// ڲԏ��
		}
	}

	if (m_BlockList != NULL) {
		No = 0;
		BlkPtr = (MDxfBlock *)m_BlockList->Head();										// ��ۯ����擪
		while (BlkPtr != NULL) {
			if (TexNam && BwsStrCmp(TexNam[No], BwsStr("")) != 0) {
				ItrSts = HeAddEnvNam(3, TexNam[No], PasNam);							// ���ϐ����������߽�𓾂�
			} else {
				if (m_VewTyp == HVIEWT_3D) {
					ItrSts = HeAddEnvNam(3, BwsStr("�q�f�a/250250250.bmp"), PasNam);	// ���ϐ����������߽�𓾂�
				} else {
					ItrSts = HeAddEnvNam(3, BwsStr("�q�f�a/070070070.bmp"), PasNam);	// ���ϐ����������߽�𓾂�
				}
			}
			if (TextList) {
				BlkPtr->m_Texture = TextList->Add(PasNam);								// ÷����ǉ�
				m_TextFlg = 0;															// ÷����׸�      0:�Ǘ�      1:����
			} else {
				HsDirectSetTexture(PasNam, &BlkPtr->m_Texture);							// ÷������
				m_TextFlg = 1;															// ÷����׸�      0:�Ǘ�      1:����
			}
			if (RgbInf)  BlkPtr->m_Rgb = RgbInf[No];

			if (BlkPtr->m_PrimList != NULL) {
				PrmPtr = (MDxfPrim *)BlkPtr->m_PrimList->Head();						// �}�`���擪
				while (PrmPtr != NULL) {
					PrmPtr->Transform(1, SymW, WMatrix, &WMove, NULL, &WMinMax);		// �}�`�̕ό`
					if (DMode == 1 || DMode == 2)  PrmPtr->SetDirectXF(LpD3DDEV, BlkPtr->m_Rgb, m_FAFlag);	// DirectX�ɾ��
					if (DMode == 0 || DMode == 2)  PrmPtr->SetDirectXE(LpD3DDEV, m_Symtry);					// DirectX�ɾ��
					PrmPtr = (MDxfPrim *)BlkPtr->m_PrimList->Next();					// �}�`���
				}
			}
			if (ShowList) {
				if (m_VewTyp == HVIEWT_3D) {
					ShowList->Add(BlkPtr->m_Texture, 13, (HDBID)this);				// �\���ǉ�
				} else {
					ShowList->Add(BlkPtr->m_Texture, 14, (HDBID)this);				// �\���ǉ�
				}
			}
			No++;
			BlkPtr = (MDxfBlock *)m_BlockList->Next();									// ��ۯ����
		}
	}

	if (ArrFlg)  ItrSts = SetDirectXY(LpD3DDEV);										// ����DirectX�ɐݒ肷��

	if (MinMax)  memcpy(MinMax, &WMinMax, sizeof(MgMinMax3));

	return(Status);
}

/******************************************************************************************************************
[ MDxfInf::ShowDirectX ]              �@�\  �c�w�e�����c�����������w�ŕ\������
******************************************************************************************************************/
HVOID MDxfInf::ShowDirectX    (														// (  O) �ð��  0:����  1:�װ
								MINT					DMode,							// (I  ) 0;�� 1:�� 2:����
								LPDIRECT3DDEVICE8		LpD3DDEV,						// (I  ) DirectX���޲�
								LPDIRECT3DTEXTURE8		Texture)						// (I  ) ÷���
{
	MDxfLayer*			LayPtr;															// ڲԏ��
	MDxfBlock*			BlkPtr;
	MDxfPrim*			PrmPtr;


	if (m_LayerList != NULL) {
		LayPtr = (MDxfLayer *)m_LayerList->Head();										// ڲԏ��擪
		while (LayPtr != NULL) {
			if (m_TextFlg == 1) {														// ����
				LpD3DDEV->SetTexture(0, NULL);											// ø����ر
				LpD3DDEV->SetTexture(0, LayPtr->m_Texture);								// ø����ݒ�
			} else {
				if (Texture && Texture != LayPtr->m_Texture) {
					LayPtr = (MDxfLayer *)m_LayerList->Next();							// ڲԏ��
					continue;
				}
			}
			if (LayPtr->m_PrimList != NULL) {
				PrmPtr = (MDxfPrim *)LayPtr->m_PrimList->Head();						// �}�`���擪
				while (PrmPtr != NULL) {
					if (DMode == 1 || DMode == 2)  PrmPtr->ShowDirectXF(LpD3DDEV);		// DirectX�ŕ\��
					if (DMode == 0 || DMode == 2)  PrmPtr->ShowDirectXE(LpD3DDEV);		// DirectX�ŕ\��
					PrmPtr = (MDxfPrim *)LayPtr->m_PrimList->Next();					// �}�`���
				}
			}
			LayPtr = (MDxfLayer *)m_LayerList->Next();									// ڲԏ��
		}
	}

	if (m_BlockList != NULL) {
		BlkPtr = (MDxfBlock *)m_BlockList->Head();										// ��ۯ����擪
		while (BlkPtr != NULL) {
			if (m_TextFlg == 1) {														// ����
				LpD3DDEV->SetTexture(0, NULL);											// ø����ر
				LpD3DDEV->SetTexture(0, BlkPtr->m_Texture);								// ø����ݒ�
			} else {
				if (Texture && Texture != BlkPtr->m_Texture) {
					BlkPtr = (MDxfBlock *)m_BlockList->Next();							// ��ۯ����
					continue;
				}
			}
			if (BlkPtr->m_PrimList != NULL) {
				PrmPtr = (MDxfPrim *)BlkPtr->m_PrimList->Head();						// �}�`���擪
				while (PrmPtr != NULL) {
					if (DMode == 1 || DMode == 2)  PrmPtr->ShowDirectXF(LpD3DDEV);		// DirectX�ŕ\��
					if (DMode == 0 || DMode == 2)  PrmPtr->ShowDirectXE(LpD3DDEV);		// DirectX�ŕ\��
					PrmPtr = (MDxfPrim *)BlkPtr->m_PrimList->Next();					// �}�`���
				}
			}
			BlkPtr = (MDxfBlock *)m_BlockList->Next();									// ��ۯ����
		}
	}

	ShowDirectXY(LpD3DDEV);																// ����DirectX�ŕ\������

	return;
}

/******************************************************************************************************************
[ MDxfInf::RelDirectX ]               �@�\  �c�w�e�����c�����������w����J������
******************************************************************************************************************/
MINT MDxfInf::RelDirectX      (														// (  O) �ð��  0:����  1:�װ
								MINT					DMode)							// (I  ) 0;�� 1:�� 2:����
{
	MINT				Status;
	MDxfLayer*			LayPtr;															// ڲԏ��
	MDxfBlock*			BlkPtr;
	MDxfPrim*			PrmPtr;


	Status = 0;

	if (m_LayerList != NULL) {
		LayPtr = (MDxfLayer *)m_LayerList->Head();										// ڲԏ��擪
		while (LayPtr != NULL) {
			if (LayPtr->m_PrimList != NULL) {
				PrmPtr = (MDxfPrim *)LayPtr->m_PrimList->Head();						// �}�`���擪
				while (PrmPtr != NULL) {
					if (DMode == 1 || DMode == 2)  PrmPtr->RelDirectXF();				// �J��
					if (DMode == 0 || DMode == 2)  PrmPtr->RelDirectXE();				// �J��
					PrmPtr = (MDxfPrim *)LayPtr->m_PrimList->Next();					// �}�`���
				}
			}
			if (m_TextFlg == 1)  RELEASE(LayPtr->m_Texture);							// ���� ø������
			LayPtr->m_Texture = NULL;
			LayPtr = (MDxfLayer *)m_LayerList->Next();									// ڲԏ��
		}
	}

	if (m_BlockList != NULL) {
		BlkPtr = (MDxfBlock *)m_BlockList->Head();										// ��ۯ����擪
		while (BlkPtr != NULL) {
			if (BlkPtr->m_PrimList != NULL) {
				PrmPtr = (MDxfPrim *)BlkPtr->m_PrimList->Head();						// �}�`���擪
				while (PrmPtr != NULL) {
					if (DMode == 1 || DMode == 2)  PrmPtr->RelDirectXF();				// �J��
					if (DMode == 0 || DMode == 2)  PrmPtr->RelDirectXE();				// �J��
					PrmPtr = (MDxfPrim *)BlkPtr->m_PrimList->Next();					// �}�`���
				}
			}
			if (m_TextFlg == 1)  RELEASE(BlkPtr->m_Texture);							// ���� ø������
			BlkPtr->m_Texture = NULL;
			BlkPtr = (MDxfBlock *)m_BlockList->Next();									// ��ۯ����
		}
	}

	RelDirectXY();																		// ����DirectX��肩��J������

	m_TextFlg = 0;

	return(Status);
}

/******************************************************************************************************************
[ MDxfInf::SetDirectXY ]              �@�\  �����c�����������w�ɐݒ肷��
******************************************************************************************************************/
MINT MDxfInf::SetDirectXY (															// (  O) �ð��  0:����  1:�װ
							LPDIRECT3DDEVICE8		LpD3DDEV)							// (I  ) DirectX���޲�
{
	MINT				Status;
	HDXVERTEX*			Vertex  = NULL;
	HDXVERTEX*			VMemory = NULL;
	MCHAR				PasNam[256];
	MFLOAT				X1, X2, Y1,Y2, Z1, Z2;
	MFLOAT				XLeng, YLeng, ZLeng, MLeng;
	MgPoint3				Pnt[4];
	D3DCOLOR			Rgb;															// �F
	MINT				IPtr;
	MINT				ItrSts;


	Status = 1;

	if ((Vertex = (HDXVERTEX *)malloc(sizeof(HDXVERTEX) * 256)) == NULL)  HMFAIL;

	ItrSts = HeAddEnvNam(3, BwsStr("�q�f�a/250250250.bmp"), PasNam);					// ���ϐ����������߽�𓾂�
	HsDirectSetTexture(PasNam, &m_TextureY);											// ÷������

	LpD3DDEV->SetVertexShader(HDX_LVERTEX);												// ���_̫�ϯĐݒ�

	m_NumTriY = IPtr = 0;

	XLeng = m_MinMax.Max.X - m_MinMax.Min.X;
	YLeng = m_MinMax.Max.Y - m_MinMax.Min.Y;
	ZLeng = m_MinMax.Max.Z - m_MinMax.Min.Z;
	MLeng = HMAX(XLeng, YLeng);
	MLeng = HMAX(MLeng, ZLeng) / 100;

	// �w����
 	Rgb = D3DCOLOR_RGBA(255, 0, 0, 255);
	X1  = m_MinMax.Min.X - XLeng / 5; X2 = m_MinMax.Max.X + XLeng / 5;
	Y1  = -MLeng                    ; Y2 = +MLeng;
	Z1  = -MLeng                    ; Z2 = +MLeng;

	Pnt[0].X = X1  ; Pnt[0].Y = Y1  ; Pnt[0].Z = Z1;									// ���
	Pnt[1].X = X1  ; Pnt[1].Y = Y1  ; Pnt[1].Z = Z2;
	Pnt[2].X = X1  ; Pnt[2].Y = Y2  ; Pnt[2].Z = Z2;
	Pnt[3].X = X1  ; Pnt[3].Y = Y2  ; Pnt[3].Z = Z1;
	ItrSts = SetVertex(4, Pnt, Rgb, &m_NumTriY, Vertex, &IPtr);							// Vertex�֏���ݒ�
	if (ItrSts != 0)  goto  PRG_EXIT;

	Pnt[0].X = X1  ; Pnt[0].Y = Y1  ; Pnt[0].Z = Z1;									// �O
	Pnt[1].X = X2  ; Pnt[1].Y = 0.f ; Pnt[1].Z = 0.f;
	Pnt[2].X = X1  ; Pnt[2].Y = Y1  ; Pnt[2].Z = Z2;
	ItrSts = SetVertex(3, Pnt, Rgb, &m_NumTriY, Vertex, &IPtr);							// Vertex�֏���ݒ�
	if (ItrSts != 0)  goto  PRG_EXIT;

	Pnt[0].X = X1  ; Pnt[0].Y = Y2  ; Pnt[0].Z = Z1;									// ��
	Pnt[1].X = X1  ; Pnt[1].Y = Y2  ; Pnt[1].Z = Z2;
	Pnt[2].X = X2  ; Pnt[2].Y = 0.f ; Pnt[2].Z = 0.f;
	ItrSts = SetVertex(3, Pnt, Rgb, &m_NumTriY, Vertex, &IPtr);							// Vertex�֏���ݒ�
	if (ItrSts != 0)  goto  PRG_EXIT;

	Pnt[0].X = X1  ; Pnt[0].Y = Y1  ; Pnt[0].Z = Z1;									// ��
	Pnt[1].X = X1  ; Pnt[1].Y = Y2  ; Pnt[1].Z = Z1;
	Pnt[2].X = X2  ; Pnt[2].Y = 0.f ; Pnt[2].Z = 0.f;
	ItrSts = SetVertex(3, Pnt, Rgb, &m_NumTriY, Vertex, &IPtr);							// Vertex�֏���ݒ�
	if (ItrSts != 0)  goto  PRG_EXIT;

	Pnt[0].X = X1  ; Pnt[0].Y = Y1  ; Pnt[0].Z = Z2;									// ��
	Pnt[1].X = X2  ; Pnt[1].Y = 0.f ; Pnt[1].Z = 0.f;
	Pnt[2].X = X1  ; Pnt[2].Y = Y2  ; Pnt[2].Z = Z2;
	ItrSts = SetVertex(3, Pnt, Rgb, &m_NumTriY, Vertex, &IPtr);							// Vertex�֏���ݒ�
	if (ItrSts != 0)  goto  PRG_EXIT;

	// �x����
 	Rgb = D3DCOLOR_RGBA(0, 255, 0, 255);
	X1  = -MLeng                    ; X2 = +MLeng;
	Y1  = m_MinMax.Min.Y - YLeng / 5; Y2 = m_MinMax.Max.Y + YLeng / 5;
	Z1  = -MLeng                    ; Z2 = +MLeng;

	Pnt[0].X = X1  ; Pnt[0].Y = Y1  ; Pnt[0].Z = Z1;									// ���
	Pnt[1].X = X2  ; Pnt[1].Y = Y1  ; Pnt[1].Z = Z1;
	Pnt[2].X = X2  ; Pnt[2].Y = Y1  ; Pnt[2].Z = Z2;
	Pnt[3].X = X1  ; Pnt[3].Y = Y1  ; Pnt[3].Z = Z2;
	ItrSts = SetVertex(4, Pnt, Rgb, &m_NumTriY, Vertex, &IPtr);							// Vertex�֏���ݒ�
	if (ItrSts != 0)  goto  PRG_EXIT;

	Pnt[0].X = X1  ; Pnt[0].Y = Y1  ; Pnt[0].Z = Z1;									// �O
	Pnt[1].X = X1  ; Pnt[1].Y = Y1  ; Pnt[1].Z = Z2;
	Pnt[2].X = 0.f ; Pnt[2].Y = Y2  ; Pnt[2].Z = 0.f;
	ItrSts = SetVertex(3, Pnt, Rgb, &m_NumTriY, Vertex, &IPtr);							// Vertex�֏���ݒ�
	if (ItrSts != 0)  goto  PRG_EXIT;

	Pnt[0].X = X2  ; Pnt[0].Y = Y1  ; Pnt[0].Z = Z1;									// ��
	Pnt[1].X = 0.f ; Pnt[1].Y = Y2  ; Pnt[1].Z = 0.f;
	Pnt[2].X = X2  ; Pnt[2].Y = Y1  ; Pnt[2].Z = Z2;
	ItrSts = SetVertex(3, Pnt, Rgb, &m_NumTriY, Vertex, &IPtr);							// Vertex�֏���ݒ�
	if (ItrSts != 0)  goto  PRG_EXIT;

	Pnt[0].X = X1  ; Pnt[0].Y = Y1  ; Pnt[0].Z = Z1;									// ��
	Pnt[1].X = 0.f ; Pnt[1].Y = Y2  ; Pnt[1].Z = 0.f;
	Pnt[2].X = X2  ; Pnt[2].Y = Y1  ; Pnt[2].Z = Z1;
	ItrSts = SetVertex(3, Pnt, Rgb, &m_NumTriY, Vertex, &IPtr);							// Vertex�֏���ݒ�
	if (ItrSts != 0)  goto  PRG_EXIT;

	Pnt[0].X = X1  ; Pnt[0].Y = Y1  ; Pnt[0].Z = Z2;									// ��
	Pnt[1].X = X2  ; Pnt[1].Y = Y1  ; Pnt[1].Z = Z2;
	Pnt[2].X = 0.f ; Pnt[2].Y = Y2  ; Pnt[2].Z = 0.f;
	ItrSts = SetVertex(3, Pnt, Rgb, &m_NumTriY, Vertex, &IPtr);							// Vertex�֏���ݒ�
	if (ItrSts != 0)  goto  PRG_EXIT;

	// �y����
 	Rgb = D3DCOLOR_RGBA(0, 0, 255, 255);
	X1  = -MLeng                    ; X2 = +MLeng;
	Y1  = -MLeng                    ; Y2 = +MLeng;
	Z1  = m_MinMax.Min.Z - ZLeng / 5; Z2 = m_MinMax.Max.Z + ZLeng / 5;

	Pnt[0].X = X1  ; Pnt[0].Y = Y1  ; Pnt[0].Z = Z1;									// ���
	Pnt[1].X = X1  ; Pnt[1].Y = Y2  ; Pnt[1].Z = Z1;
	Pnt[2].X = X2  ; Pnt[2].Y = Y2  ; Pnt[2].Z = Z1;
	Pnt[3].X = X2  ; Pnt[3].Y = Y1  ; Pnt[3].Z = Z1;
	ItrSts = SetVertex(4, Pnt, Rgb, &m_NumTriY, Vertex, &IPtr);							// Vertex�֏���ݒ�
	if (ItrSts != 0)  goto  PRG_EXIT;

	Pnt[0].X = X1  ; Pnt[0].Y = Y1  ; Pnt[0].Z = Z1;									// �O
	Pnt[1].X = X1  ; Pnt[1].Y = Y2  ; Pnt[1].Z = Z1;
	Pnt[2].X = 0.f ; Pnt[2].Y = 0.f ; Pnt[2].Z = Z2;
	ItrSts = SetVertex(3, Pnt, Rgb, &m_NumTriY, Vertex, &IPtr);							// Vertex�֏���ݒ�
	if (ItrSts != 0)  goto  PRG_EXIT;

	Pnt[0].X = X2  ; Pnt[0].Y = Y1  ; Pnt[0].Z = Z1;									// ��
	Pnt[1].X = 0.f ; Pnt[1].Y = 0.f ; Pnt[1].Z = Z2;
	Pnt[2].X = X2  ; Pnt[2].Y = Y2  ; Pnt[2].Z = Z1;
	ItrSts = SetVertex(3, Pnt, Rgb, &m_NumTriY, Vertex, &IPtr);							// Vertex�֏���ݒ�
	if (ItrSts != 0)  goto  PRG_EXIT;

	Pnt[0].X = X1  ; Pnt[0].Y = Y1  ; Pnt[0].Z = Z1;									// ��
	Pnt[1].X = X2  ; Pnt[1].Y = Y1  ; Pnt[1].Z = Z1;
	Pnt[2].X = 0.f ; Pnt[2].Y = 0.f ; Pnt[2].Z = Z2;
	ItrSts = SetVertex(3, Pnt, Rgb, &m_NumTriY, Vertex, &IPtr);							// Vertex�֏���ݒ�
	if (ItrSts != 0)  goto  PRG_EXIT;

	Pnt[0].X = X1  ; Pnt[0].Y = Y2  ; Pnt[0].Z = Z1;									// ��
	Pnt[1].X = 0.f ; Pnt[1].Y = 0.f ; Pnt[1].Z = Z2;
	Pnt[2].X = X2  ; Pnt[2].Y = Y2  ; Pnt[2].Z = Z1;
	ItrSts = SetVertex(3, Pnt, Rgb, &m_NumTriY, Vertex, &IPtr);							// Vertex�֏���ݒ�
	if (ItrSts != 0)  goto  PRG_EXIT;

	LpD3DDEV->CreateVertexBuffer(sizeof(HDXVERTEX) * m_NumTriY * 3, 0, HDX_LVERTEX, D3DPOOL_MANAGED, &m_VertBufY);
	m_VertBufY->Lock(0, 0, (BYTE **)&VMemory, 0);
	memcpy(VMemory, Vertex, sizeof(HDXVERTEX) * m_NumTriY * 3);
	m_VertBufY->Unlock();

	Status = 0;

PRG_EXIT:

	HFREE(Vertex);

	return(Status);
}

/******************************************************************************************************************
[ MDxfInf::ShowDirectXY ]             �@�\  �����c�����������w�ŕ\������
******************************************************************************************************************/
HVOID MDxfInf::ShowDirectXY   (
								LPDIRECT3DDEVICE8		LpD3DDEV)						// (I  ) DirectX���޲�
{
	if (m_TextureY) {
		LpD3DDEV->SetTexture(0, NULL);													// ø����ر
		LpD3DDEV->SetTexture(0, m_TextureY);											// ø����ݒ�
	}

	if (m_NumTriY > 0) {
		LpD3DDEV->SetVertexShader(HDX_LVERTEX);											// ���_̫�ϯĐݒ�

		LpD3DDEV->SetStreamSource(0, m_VertBufY, sizeof(HDXVERTEX));
		LpD3DDEV->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_NumTriY);						// �R�p�`�̐�
		LpD3DDEV->SetStreamSource(0, NULL, 0);
	}
}

/******************************************************************************************************************
[ MDxfInf::RelDirectXY ]              �@�\   �����c�����������w����J������
******************************************************************************************************************/
HVOID MDxfInf::RelDirectXY    ()
{
	if (m_TextureY)  RELEASE(m_TextureY);												// ÷����i���j
	m_TextureY = NULL;

	if (m_VertBufY)  RELEASE(m_VertBufY);												// DirectX ���_�ޯ̧
	m_VertBufY = NULL;
	m_NumTriY  = 0;																		// �R�p�`�̐�
}

/******************************************************************************************************************
[ MDxfBlock::MDxfBlock ]             �@�\  �R���X�g���N�^
******************************************************************************************************************/
MDxfBlock::MDxfBlock     ()
{
	BwsStrCpy(m_Name,  BwsStr(""));														// ��ۯ���
	BwsStrCpy(m_LName, BwsStr(""));														// ڲԖ�
	m_Color    = 0;																		// �F�ԍ�
	m_Rgb      = RGB(250, 250, 250);
	m_Texture  = NULL;
	m_LineNo   = 0;																		// ���ԍ�
	m_InsPnt.X = m_InsPnt.Y = m_InsPnt.Z = 0.0f;										// �}����_
	m_PrimList = new HlDList();										 					// ����è�ޥ�o�����߲��
}

/******************************************************************************************************************
[ MDxfBlock::MDxfBlock ]             �@�\  �R���X�g���N�^
******************************************************************************************************************/
MDxfBlock::MDxfBlock     (
							MCHAR*			Name,										// (I  ) ��ۯ���
							MCHAR*			LName,										// (I  ) ڲԖ�
							MINT			Color,										// (I  ) �F�ԍ�
							MINT			LineNo,										// (I  ) ���ԍ�
							MgPoint3*		InsPnt)										// (I  ) �}����_
{
	BwsStrCpy(m_Name,   Name);															// ��ۯ���
	BwsStrCpy(m_LName, LName);															// ڲԖ�
	m_Color    = Color;																	// �F�ԍ�
	m_Rgb      = MDxfGetCol(m_Color);
	m_Texture  = NULL;
	m_LineNo   = LineNo;																// ���ԍ�
	memcpy(&m_InsPnt, InsPnt, sizeof(MgPoint3));											// �}����_
	m_PrimList = new HlDList();										 					// ����è�ޥ�o�����߲��
}

/******************************************************************************************************************
[ MDxfBlock::~MDxfBlock ]            �@�\  �f�X�g���N�^
******************************************************************************************************************/
MDxfBlock::~MDxfBlock    ()
{
	MDxfPrim*			Ptr;															// �}�`���ւ��߲��


	if (m_PrimList != NULL) {
		Ptr = (MDxfPrim *)m_PrimList->Head();											// �擪���߲���𓾂�
		while (Ptr != NULL) {
			delete  Ptr;
			Ptr = (MDxfPrim *)m_PrimList->Next();										// �����߲���𓾂�
		}
		delete m_PrimList;										 						// �}�`��o�����߲��
		m_PrimList = NULL;
	}
}

/******************************************************************************************************************
[ MDxfBlock::SetInf ]                 �@�\  �u���b�N�����Z�b�g
******************************************************************************************************************/
HVOID MDxfBlock::SetInf   (
							MCHAR*			Name,										// (I  ) ��ۯ���
							MCHAR*			LName,										// (I  ) ڲԖ�
							MINT			Color,										// (I  ) �F�ԍ�
							MINT			LineNo,										// (I  ) ���ԍ�
							MgPoint3*		InsPnt)										// (I  ) �}����_
{
	BwsStrCpy(m_Name,   Name);															// ��ۯ���
	BwsStrCpy(m_LName, LName);															// ڲԖ�
	m_Color    = Color;																	// �F�ԍ�
	m_Rgb      = MDxfGetCol(m_Color);
	m_Texture  = NULL;
	m_LineNo   = LineNo;																// ���ԍ�
	memcpy(&m_InsPnt, InsPnt, sizeof(MgPoint3));											// �}����_
}

/******************************************************************************************************************
[ MDxfBlock::AddPrim ]                �@�\  �u���b�N���ɐ}�`����ǉ�
******************************************************************************************************************/
MDxfPrim* MDxfBlock::AddPrim (														// (  O) �}�`���̲ݽ�ݽ
								MINT				PrmType,							// (I  ) ����è������
								MINT				Color,								// (I  ) �F�ԍ�
								MINT				LineNo,								// (I  ) ����ԍ��i�ԍ��j
								MINT				LineWd,								// (I  ) �����ԍ��i�ԍ��j
								HXPRIM*				Prim)								// (I  ) �}�`
{
	MDxfPrim*			Ptr;															// �}�`���


	Ptr = new MDxfPrim(PrmType, Color, LineNo, LineWd, Prim);							// �}�`���ݽ�׸�
	m_PrimList->Inject((HVOID *)Ptr);													// �}�`���ؽĂ֒ǉ�

	return(Ptr);
}


/******************************************************************************************************************
[ MDxfLayer::MDxfLayer ]             �@�\  �R���X�g���N�^
******************************************************************************************************************/
MDxfLayer::MDxfLayer         (
								MCHAR*				Name,								// (I  ) ڲԖ�
								MINT				Color,								// (I  ) �F�ԍ�
								MINT				LineNo)								// (I  ) ���ԍ�
{
	BwsStrCpy(m_Name, Name);															// ڲԖ�
	m_Color    = Color;																	// �F�ԍ�
	m_Rgb = MDxfGetCol(m_Color);
	m_Texture  = NULL;
	m_LineNo   = LineNo;																// ���ԍ�
	m_PrimList = new HlDList();										 					// ����è�ޥ�o�����߲��

}

/******************************************************************************************************************
[ MDxfLayer::~MDxfLayer ]            �@�\  �f�X�g���N�^
******************************************************************************************************************/
MDxfLayer::~MDxfLayer        ()
{
	MDxfPrim*			Ptr;															// �}�`���ւ��߲��


	if (m_PrimList != NULL) {
		Ptr = (MDxfPrim *)m_PrimList->Head();											// �擪���߲���𓾂�
		while (Ptr != NULL) {
			delete  Ptr;
			Ptr = (MDxfPrim *)m_PrimList->Next();										// �����߲���𓾂�
		}
		delete m_PrimList;										 						// �}�`��o�����߲��
		m_PrimList = NULL;
	}

}

/******************************************************************************************************************
[ MDxfLayer::Modify ]                 �@�\  �X�V
******************************************************************************************************************/
MDxfLayer::Modify             (
								MINT				Color,								// (I  ) �F�ԍ�
								MINT				LineNo)								// (I  ) ���ԍ�
{
	m_Color  = Color;																	// �F�ԍ�
	m_Rgb = MDxfGetCol(m_Color);
	m_LineNo = LineNo;																	// ���ԍ�

}

/******************************************************************************************************************
[ MDxfLayer::AddPrim ]                �@�\  ���C�����ɐ}�`����ǉ�
******************************************************************************************************************/
MDxfPrim* MDxfLayer::AddPrim (														// (  O) �}�`���̲ݽ�ݽ
								MINT				PrmType,							// (I  ) ����è������
								MINT				Color,								// (I  ) �F�ԍ�
								MINT				LineNo,								// (I  ) ����ԍ��i�ԍ��j
								MINT				LineWd,								// (I  ) �����ԍ��i�ԍ��j
								HXPRIM*				Prim)								// (I  ) �}�`
{
	MDxfPrim*			Ptr;															// �}�`���


	Ptr = new MDxfPrim(PrmType, Color, LineNo, LineWd, Prim);							// �}�`���ݽ�׸�
	m_PrimList->Inject((HVOID *)Ptr);													// �}�`���ؽĂ֒ǉ�

	return(Ptr);
}

/******************************************************************************************************************
[ MDxfLines::MDxfLines ]             �@�\  �����R���X�g���N�^
******************************************************************************************************************/
MDxfLines::MDxfLines     (
							MCHAR*			Name,										// (I  ) ����
							MINT			LineNo)										// (I  ) ���ԍ�
{
	BwsStrCpy(m_Name, Name);															// ����
	m_LineNo = LineNo;																	// ���ԍ�

}

/******************************************************************************************************************
[ MDxfLines::~MDxfLines ]            �@�\  �����f�X�g���N�^
******************************************************************************************************************/
MDxfLines::~MDxfLines    ()
{
}

/******************************************************************************************************************
[ MDxfFonts::MDxfFonts ]             �@�\  �t�H���g���R���X�g���N�^
******************************************************************************************************************/
MDxfFonts::MDxfFonts     (
							MCHAR*			Name,										// (I  ) ̫�Ė�
							MINT			FCode)										// (I  ) ̫�ĺ���
{
	BwsStrCpy(m_Name, Name);															// ̫�Ė�
	m_FCode = FCode;																	// ̫�ĺ���

}

/******************************************************************************************************************
[ MDxfFonts::~MDxfFonts ]            �@�\  �t�H���g���f�X�g���N�^
******************************************************************************************************************/
MDxfFonts::~MDxfFonts    ()
{
}


/******************************************************************************************************************
[ MDxfPrim::MDxfPrim ]               �@�\  �}�`���R���X�g���N�^
******************************************************************************************************************/
MDxfPrim::MDxfPrim           (
								MINT				PrmType,							// (I  ) ����è������
								MINT				Color,								// (I  ) �F�ԍ�
								MINT				LineNo,								// (I  ) ����ԍ��i�ԍ��j
								MINT				LineWd,								// (I  ) �����ԍ��i�ԍ��j
								HXPRIM*				Prim)								// (I  ) �}�`
{
	MINT*			VerNumPnt = NULL;													// ү�����̒��_����
	MINT*			PntNo     = NULL;													// ���_�ԍ���
	MgPoint3*		Pnt       = NULL;													// ���_��
	MCHAR*			String    = NULL;													// ������

	m_PrmType = PrmType;																// ����è������
	m_Color   = Color;																	// �F�ԍ�
	m_LineNo  = LineNo;																	// ����ԍ��i�ԍ��j
	m_LineWd  = LineWd;																	// �����ԍ��i�ԍ��j
	memcpy(&m_Prim, Prim, sizeof(HXPRIM));												// �}�`
	m_NumTri  = 0;																		// �R�p�`�̐�
	m_VertBuf = NULL;																	// DirectX ���_�ޯ̧
	m_NumLne  = 0;																		// ���̐�
	m_LineBuf = NULL;																	// DirectX �����ޯ̧

	if (m_PrmType == HPRIM_MSH) {														// ����:ү��
		if ((VerNumPnt = (MINT    *)malloc(sizeof(MINT)    * m_Prim.Mesh.Num))      == NULL)  HMFAIL;	// ү�����̒��_����
		if ((PntNo     = (MINT    *)malloc(sizeof(MINT)    * m_Prim.Mesh.NumPntNo)) == NULL)  HMFAIL;	// ���_�ԍ���
		if ((Pnt       = (MgPoint3 *)malloc(sizeof(MgPoint3) * m_Prim.Mesh.NumPnt))   == NULL)  HMFAIL;	// ���_��

		memcpy(VerNumPnt, m_Prim.Mesh.VerNumPnt, sizeof(HUINT)   * m_Prim.Mesh.Num);
		memcpy(PntNo,     m_Prim.Mesh.PntNo,     sizeof(HUINT)   * m_Prim.Mesh.NumPntNo);
		memcpy(Pnt,       m_Prim.Mesh.Pnt,       sizeof(MgPoint3) * m_Prim.Mesh.NumPnt);

		m_Prim.Mesh.VerNumPnt = VerNumPnt;												// ү�����̒��_����
		m_Prim.Mesh.PntNo     = PntNo;													// ���_�ԍ���
		m_Prim.Mesh.Pnt       = Pnt;													// ���_��
	} else if (m_PrmType == HPRIM_TXT) {												// ����:����
		if ((String = (MCHAR *)malloc(sizeof(MCHAR) * (m_Prim.Text.Num+1))) == NULL)  HMFAIL;
		BwsStrCpy(String, m_Prim.Text.String);
		m_Prim.Text.String = String;													// ������
	}
}

/******************************************************************************************************************
[ MDxfPrim::~MDxfPrim ]              �@�\  �}�`���f�X�g���N�^
******************************************************************************************************************/
MDxfPrim::~MDxfPrim      ()
{
	RELEASE(m_VertBuf);																	// DirectX ���_�ޯ̧
	RELEASE(m_LineBuf);																	// DirectX �����ޯ̧

	if (m_PrmType == HPRIM_MSH) {														// ����:ү��
		HFREE(m_Prim.Mesh.VerNumPnt);
		HFREE(m_Prim.Mesh.PntNo);
		HFREE(m_Prim.Mesh.Pnt);
	}
}

/******************************************************************************************************************
[ MDxfPrim::Transform ]               �@�\  �}�`���̕ό`
******************************************************************************************************************/
HVOID MDxfPrim::Transform     (
								MINT				Color,								// (I  ) �F�ԍ�
								MgPoint3*			Base,								// (I  ) ��_
								MgPoint3*			Insert,								// (I  ) �}���_
								MgPoint3*			Scale,								// (I  ) �ړx
								MFLOAT				Rotate)								// (I  ) ��]
{
	HMTRX3			Matrix;																// �ϊ���ظ�
	MgPoint3			Move;																// �ړ���
	MFLOAT			Radian;																// ��]�p�x�iradian�j
	MFLOAT			MScale;																// ���a�ȂǕ��ʏ�̋ϓ��Ȕ{��
	MINT			ic;


	// �ϊ��}�g���N�X
	Radian       = Rotate * HPI / 180.0f;
	Matrix[0][0] = (MFLOAT)(Scale->X * cos(Radian));
	Matrix[0][1] = (MFLOAT)(Scale->Y * sin(Radian) * (-1.0f));
	Matrix[0][2] = 0.0f;
	Matrix[1][0] = (MFLOAT)(Scale->X * sin(Radian));
	Matrix[1][1] = (MFLOAT)(Scale->Y * cos(Radian));
	Matrix[1][2] = 0.0f;
	Matrix[2][0] = 0.0f;
	Matrix[2][1] = 0.0f;
	Matrix[2][2] = 1.0f;

	// �ړ���
	Move.X = (Insert->X) - (Base->X);
	Move.Y = (Insert->Y) - (Base->Y);
	Move.Z = (Insert->Z) - (Base->Z);

	// �ϓ��{����ݒ�
	MScale = HMAX(Scale->X, Scale->Y);
	MScale = HMAX(MScale,   Scale->Z);

	if (m_Color != 0)  m_Color = Color;

	switch  (m_PrmType) {
		case  HPRIM_DOT:																// �_
			HgTrnPntByMtx(&m_Prim.Dot, Matrix, &Move, &m_Prim.Dot);
			break;
		case  HPRIM_LIN:																// ��
			HgTrnPntByMtx(&m_Prim.Line.Pnt[0], Matrix, &Move, &m_Prim.Line.Pnt[0]);
			HgTrnPntByMtx(&m_Prim.Line.Pnt[1], Matrix, &Move, &m_Prim.Line.Pnt[1]);
			break;
		case  HPRIM_CIR:																// �~:
			HgTrnPntByMtx(&m_Prim.Circle.Pnt1, Matrix, &Move, &m_Prim.Circle.Pnt1);
			HgTrnPntByMtx(&m_Prim.Circle.Pnt2, Matrix, &Move, &m_Prim.Circle.Pnt2);
			break;
		case  HPRIM_ARC:																// �~��
			HgTrnPntByMtx(&m_Prim.Arc.Pnt1, Matrix, &Move, &m_Prim.Arc.Pnt1);
			HgTrnPntByMtx(&m_Prim.Arc.Pnt2, Matrix, &Move, &m_Prim.Arc.Pnt2);
			HgTrnPntByMtx(&m_Prim.Arc.Pnt3, Matrix, &Move, &m_Prim.Arc.Pnt3);
			HgTrnPntByMtx(&m_Prim.Arc.Pnt4, Matrix, &Move, &m_Prim.Arc.Pnt4);
			break;
		case  HPRIM_TXT:																// ������
			HgTrnPntByMtx(&m_Prim.Text.Pnt, Matrix, &Move, &m_Prim.Text.Pnt);
			m_Prim.Text.Hei = m_Prim.Text.Hei * MScale;
			break;
		case  HPRIM_POL:																// ��
			for (ic = 0 ; ic < 4 ; ic++ )	{
				HgTrnPntByMtx(&m_Prim.Face.Pnt[ic], Matrix, &Move, &m_Prim.Face.Pnt[ic]);
			}
			break;
		case  HPRIM_MSH:																// ү��
			for (ic = 0 ; ic < m_Prim.Mesh.NumPnt ; ic++) {
				HgTrnPntByMtx(&m_Prim.Mesh.Pnt[ic], Matrix, &Move, &m_Prim.Mesh.Pnt[ic]);
			}
			break;
		default:
			break;
	}
}

/******************************************************************************************************************
[ MDxfPrim::Transform ]               �@�\  �}�`���̕ό`
******************************************************************************************************************/
HVOID MDxfPrim::Transform     (
								MINT				TFlag,								// (I  ) 0:�ϊ����� 1:�ϊ�
								MINT				Symtry,								// (I  ) �Ώ��׸�  0:�Ȃ�  1:����
								HMTRX3				Matrix,								// (I  ) �ϊ���ظ�
								MgPoint3*			Move,								// (I  ) �ړ���
								MFLOAT*				Scale,								// (I  ) ����
								MgMinMax3*			MinMax)								// (  O) �ŏ���ő�
{
	HMTRX3				WMatrix;														// �ϊ���ظ�
	MgPoint3				WMove;															// �ړ���
	MFLOAT				WScale;
	MgPoint3*			PntW1 = NULL;
	MINT				ic;


	if ((PntW1 = (MgPoint3 *)malloc(sizeof(MgPoint3) * 2048)) == NULL)  HMFAIL;

	// �ϊ��}�g���N�X���쐬
	if (Matrix) {
		memcpy(&WMatrix, Matrix, sizeof(HMTRX3));
	} else {
		WMatrix[0][0] = 1.f; WMatrix[0][1] = 0.f; WMatrix[0][2] = 0.f;
		WMatrix[1][0] = 0.f; WMatrix[1][1] = 1.f; WMatrix[1][2] = 0.f;
		WMatrix[2][0] = 0.f; WMatrix[2][1] = 0.f; WMatrix[2][2] = 1.f;
	}

	if (Move) {
		memcpy(&WMove, Move, sizeof(MgPoint3));
	} else {
		WMove.X = WMove.Y = WMove.Z = 0.f;
	}

	WScale = 1.0f;
	if (Scale) {
		WMatrix[0][0] = WMatrix[0][0] * Scale[0];
		WMatrix[1][1] = WMatrix[1][1] * Scale[1];
		WMatrix[2][2] = WMatrix[2][2] * Scale[2];
		WScale        = Scale[0];
	}

	switch  (m_PrmType) {
		case  HPRIM_DOT:																// �_                (1)
			if (TFlag) {
				HgTrnPntByMtx(&m_Prim.Dot, WMatrix, &WMove, &m_Prim.Dot);
				SetMinMax(&m_Prim.Dot, MinMax);											// �ŏ���ő���X�V
			} else {
				HgTrnPntByMtx(&m_Prim.Dot, WMatrix, &WMove, &PntW1[0]);
				SetMinMax(&PntW1[0], MinMax);											// �ŏ���ő���X�V
			}
			break;
		case  HPRIM_LIN:																// ��                (2)
			if (TFlag) {
				HgTrnPntByMtx(&m_Prim.Line.Pnt[0], WMatrix, &WMove, &m_Prim.Line.Pnt[0]);
				SetMinMax(&m_Prim.Line.Pnt[0], MinMax);									// �ŏ���ő���X�V
				HgTrnPntByMtx(&m_Prim.Line.Pnt[1], WMatrix, &WMove, &m_Prim.Line.Pnt[1]);
				SetMinMax(&m_Prim.Line.Pnt[1], MinMax);									// �ŏ���ő���X�V
			} else {
				HgTrnPntByMtx(&m_Prim.Line.Pnt[0], WMatrix, &WMove, &PntW1[0]);
				SetMinMax(&PntW1[0], MinMax);											// �ŏ���ő���X�V
				HgTrnPntByMtx(&m_Prim.Line.Pnt[1], WMatrix, &WMove, &PntW1[1]);
				SetMinMax(&PntW1[1], MinMax);											// �ŏ���ő���X�V
			}
			break;
		case  HPRIM_CIR:																// �~                (4)
			if (TFlag) {
				HgTrnPntByMtx(&m_Prim.Circle.Pnt1, WMatrix, &WMove, &PntW1[0]);
				HgTrnPntByMtx(&m_Prim.Circle.Pnt2, WMatrix, &WMove, &PntW1[1]);
				m_Prim.Circle.Pnt1 = PntW1[0];
				m_Prim.Circle.Pnt2 = PntW1[1];

//				m_Prim.Circle.Pnt1.X = HMIN(PntW1[0].X, PntW1[1].X);
//				m_Prim.Circle.Pnt1.Y = HMIN(PntW1[0].Y, PntW1[1].Y);
//				m_Prim.Circle.Pnt1.Z = HMIN(PntW1[0].Z, PntW1[1].Z);
//				m_Prim.Circle.Pnt2.X = HMAX(PntW1[0].X, PntW1[1].X);
//				m_Prim.Circle.Pnt2.Y = HMAX(PntW1[0].Y, PntW1[1].Y);
//				m_Prim.Circle.Pnt2.Z = HMAX(PntW1[0].Z, PntW1[1].Z);
				SetMinMax(&m_Prim.Circle.Pnt1, MinMax);									// �ŏ���ő���X�V
				SetMinMax(&m_Prim.Circle.Pnt2, MinMax);									// �ŏ���ő���X�V
			} else {
				HgTrnPntByMtx(&m_Prim.Circle.Pnt1, WMatrix, &WMove, &PntW1[0]);
				SetMinMax(&PntW1[0], MinMax);											// �ŏ���ő���X�V
				HgTrnPntByMtx(&m_Prim.Circle.Pnt2, WMatrix, &WMove, &PntW1[0]);
				SetMinMax(&PntW1[0], MinMax);											// �ŏ���ő���X�V
			}
			break;
		case  HPRIM_ARC:																// �~��              (5)
			if (TFlag) {
				HgTrnPntByMtx(&m_Prim.Arc.Pnt1, WMatrix, &WMove, &PntW1[0]);
				HgTrnPntByMtx(&m_Prim.Arc.Pnt2, WMatrix, &WMove, &PntW1[1]);
				HgTrnPntByMtx(&m_Prim.Arc.Pnt3, WMatrix, &WMove, &PntW1[2]);
				HgTrnPntByMtx(&m_Prim.Arc.Pnt4, WMatrix, &WMove, &PntW1[3]);
				m_Prim.Arc.Pnt1 = PntW1[0];
				m_Prim.Arc.Pnt2 = PntW1[1];
				m_Prim.Arc.Pnt3 = PntW1[2];
				m_Prim.Arc.Pnt4 = PntW1[3];

//				m_Prim.Arc.Pnt1.X = HMIN(PntW1[0].X, PntW1[1].X);
//				m_Prim.Arc.Pnt1.Y = HMIN(PntW1[0].Y, PntW1[1].Y);
//				m_Prim.Arc.Pnt1.Z = HMIN(PntW1[0].Z, PntW1[1].Z);
//				m_Prim.Arc.Pnt2.X = HMAX(PntW1[0].X, PntW1[1].X);
//				m_Prim.Arc.Pnt2.Y = HMAX(PntW1[0].Y, PntW1[1].Y);
//				m_Prim.Arc.Pnt2.Z = HMAX(PntW1[0].Z, PntW1[1].Z);
//				if (Symtry == 0) {														// �Ώ̂Ȃ�
//					m_Prim.Arc.Pnt3 = PntW1[2];
//					m_Prim.Arc.Pnt4 = PntW1[3];
//				} else {																// �Ώ̂���
//					m_Prim.Arc.Pnt3 = PntW1[3];
//					m_Prim.Arc.Pnt4 = PntW1[2];
//				}
				SetMinMax(&m_Prim.Arc.Pnt3, MinMax);									// �ŏ���ő���X�V
				SetMinMax(&m_Prim.Arc.Pnt4, MinMax);									// �ŏ���ő���X�V
			} else {
				HgTrnPntByMtx(&m_Prim.Arc.Pnt1, WMatrix, &WMove, &PntW1[0]);
				HgTrnPntByMtx(&m_Prim.Arc.Pnt2, WMatrix, &WMove, &PntW1[0]);
				HgTrnPntByMtx(&m_Prim.Arc.Pnt3, WMatrix, &WMove, &PntW1[0]);
				SetMinMax(&PntW1[0], MinMax);											// �ŏ���ő���X�V
				HgTrnPntByMtx(&m_Prim.Arc.Pnt4, WMatrix, &WMove, &PntW1[0]);
				SetMinMax(&PntW1[0], MinMax);											// �ŏ���ő���X�V
			}
			break;
		case  HPRIM_TXT:																// ������            (6)
			if (TFlag) {
				HgTrnPntByMtx(&m_Prim.Text.Pnt, WMatrix, &WMove, &m_Prim.Text.Pnt);
				SetMinMax(&m_Prim.Text.Pnt, MinMax);									// �ŏ���ő���X�V
				m_Prim.Text.Hei = m_Prim.Text.Hei * WScale;
			} else {
				HgTrnPntByMtx(&m_Prim.Text.Pnt, WMatrix, &WMove, &PntW1[0]);
				SetMinMax(&PntW1[0], MinMax);											// �ŏ���ő���X�V
			}
			break;
		case  HPRIM_POL:																// ��
			if (TFlag) {
				for (ic = 0 ; ic < 4 ; ic++ ) {
					HgTrnPntByMtx(&m_Prim.Face.Pnt[ic], WMatrix, &WMove, &PntW1[ic]);
				}
				for (ic = 0 ; ic < 4 ; ic++ ) {
					if (Symtry == 0) {
						m_Prim.Face.Pnt[ic] = PntW1[ic];
					} else {
						m_Prim.Face.Pnt[ic] = PntW1[3-ic];
					}
					SetMinMax(&m_Prim.Face.Pnt[ic], MinMax);							// �ŏ���ő���X�V
				}
			} else {
				for (ic = 0 ; ic < 4 ; ic++ ) {
					HgTrnPntByMtx(&m_Prim.Face.Pnt[ic], WMatrix, &WMove, &PntW1[0]);
					SetMinMax(&PntW1[0], MinMax);										// �ŏ���ő���X�V
				}
			}
			break;
		case  HPRIM_MSH:																// ү��              (7)
			if (TFlag) {
				for (ic = 0 ; ic < m_Prim.Mesh.NumPnt ; ic++) {
					HgTrnPntByMtx(&m_Prim.Mesh.Pnt[ic], WMatrix, &WMove, &PntW1[ic]);
				}
				for (ic = 0 ; ic < m_Prim.Mesh.NumPnt ; ic++ ) {
					if (Symtry == 0) {
						m_Prim.Mesh.Pnt[ic] = PntW1[ic];
					} else {
						m_Prim.Mesh.Pnt[ic] = PntW1[m_Prim.Mesh.NumPnt-1-ic];
					}
					SetMinMax(&m_Prim.Mesh.Pnt[ic], MinMax);							// �ŏ���ő���X�V
				}
			} else {
				for (ic = 0 ; ic < m_Prim.Mesh.NumPnt ; ic++) {
					HgTrnPntByMtx(&m_Prim.Mesh.Pnt[ic], WMatrix, &WMove, &PntW1[0]);
					SetMinMax(&PntW1[0], MinMax);										// �ŏ���ő���X�V
				}
			}
			break;
		default:
			break;
	}

	HFREE(PntW1);

	return;
}

/******************************************************************************************************************
[ MDxfPrim::TransformYZ ]             �@�\  �}�`���̕ό`�i�x���y�j
******************************************************************************************************************/
HVOID MDxfPrim::TransformYZ   (
								MgMinMax3*			MinMax)								// (  O) �ŏ���ő�
{
	MINT				ic;


	switch  (m_PrmType) {
		case  HPRIM_DOT:																// �_                (1)
			m_Prim.Dot.Z = m_Prim.Dot.Y; m_Prim.Dot.Y = 0.f;							//
			SetMinMax(&m_Prim.Dot, MinMax);												// �ŏ���ő���X�V
			break;
		case  HPRIM_LIN:																// ��                (2)
			m_Prim.Line.Pnt[0].Z = m_Prim.Line.Pnt[0].Y; m_Prim.Line.Pnt[0].Y = 0.f;
			m_Prim.Line.Pnt[1].Z = m_Prim.Line.Pnt[1].Y; m_Prim.Line.Pnt[1].Y = 0.f;
			SetMinMax(&m_Prim.Line.Pnt[0], MinMax);										// �ŏ���ő���X�V
			SetMinMax(&m_Prim.Line.Pnt[1], MinMax);										// �ŏ���ő���X�V
			break;
		case  HPRIM_CIR:																// �~                (4)
			m_Prim.Circle.Pnt1.Z = m_Prim.Circle.Pnt1.Y; m_Prim.Circle.Pnt1.Y = 0.f;
			m_Prim.Circle.Pnt2.Z = m_Prim.Circle.Pnt2.Y; m_Prim.Circle.Pnt2.Y = 0.f;
			SetMinMax(&m_Prim.Circle.Pnt1, MinMax);										// �ŏ���ő���X�V
			SetMinMax(&m_Prim.Circle.Pnt2, MinMax);										// �ŏ���ő���X�V
			break;
		case  HPRIM_ARC:																// �~��              (5)
			m_Prim.Arc.Pnt1.Z = m_Prim.Arc.Pnt1.Y; m_Prim.Arc.Pnt1.Y = 0.f;
			m_Prim.Arc.Pnt2.Z = m_Prim.Arc.Pnt2.Y; m_Prim.Arc.Pnt2.Y = 0.f;
			m_Prim.Arc.Pnt3.Z = m_Prim.Arc.Pnt3.Y; m_Prim.Arc.Pnt3.Y = 0.f;
			m_Prim.Arc.Pnt4.Z = m_Prim.Arc.Pnt4.Y; m_Prim.Arc.Pnt4.Y = 0.f;
			SetMinMax(&m_Prim.Arc.Pnt3, MinMax);										// �ŏ���ő���X�V
			SetMinMax(&m_Prim.Arc.Pnt4, MinMax);										// �ŏ���ő���X�V
			break;
		case  HPRIM_TXT:																// ������            (6)
			m_Prim.Text.Pnt.Z = m_Prim.Text.Pnt.Y; m_Prim.Text.Pnt.Y = 0.f;
			SetMinMax(&m_Prim.Text.Pnt, MinMax);										// �ŏ���ő���X�V
			break;
		case  HPRIM_POL:																// ��
			for (ic = 0 ; ic < 4 ; ic++ ) {
				m_Prim.Face.Pnt[ic].Z = m_Prim.Face.Pnt[ic].Y; m_Prim.Face.Pnt[ic].Y = 0.f;
				SetMinMax(&m_Prim.Face.Pnt[ic], MinMax);								// �ŏ���ő���X�V
			}
			break;
		case  HPRIM_MSH:																// ү��              (7)
			for (ic = 0 ; ic < m_Prim.Mesh.NumPnt ; ic++) {
				m_Prim.Mesh.Pnt[ic].Z = m_Prim.Mesh.Pnt[ic].Y; m_Prim.Mesh.Pnt[ic].Y = 0.f;
				SetMinMax(&m_Prim.Mesh.Pnt[ic], MinMax);								// �ŏ���ő���X�V
			}
			break;
		default:
			break;
	}

	return;
}

/******************************************************************************************************************
[ MDxfPrim::Show ]                    �@�\  �}�`����\������
******************************************************************************************************************/
HVOID MDxfPrim::Show          (
								CDC*				MemDC,								// (I  ) ���DC
								MINT				MainId,								// (I  ) Ҳ�ID
								MINT				SubsId,								// (I  ) ���ID
								COLORREF			Color,								// (I  ) �\���F
								MINT				Symtry,								// (I  ) �Ώ��׸�  0:�Ȃ�  1:����
								HMTRX3				Matrix,								// (I  ) �ϊ���ظ�
								MgPoint3*			Move,								// (I  ) �ړ���
								MFLOAT				Scale,								// (I  ) ����
								MgPoint2*			CutPnt,								// (I  ) ��Đ� + �����_
								MgMinMax3*			MinMax)								// (  O) �ŏ���ő�
{
	MgPoint3				Pnt1[4];
	MgPoint2				Pnt2[4];
	MgPoint2				Pnt3[4];
	MgPoint2				Pnt4[4];
	MgPoint2				Pnt5[4];
	MgPoint2				Pnt6[4];
	HCIR2				CirW1;
	MINT				Num;
	MINT				Idx, No;
	MINT				ItrSts;
	MINT				ic, jc, nc;


	switch  (m_PrmType) {
		case  HPRIM_DOT:																// �_
			HgTrnPntByMtx(&m_Prim.Dot, Matrix, Move, &Pnt1[0]);
			memcpy(&Pnt2[0], &Pnt1[0], sizeof(MgPoint2));
			if (CutPnt) {																// ��Ă���
				ItrSts = HgCutPntByCutL(CutPnt, &Pnt2[0]);
				if (ItrSts == -1)  goto  PRG_EXIT;										// ���ׂĶ��
			}
			SetMinMax(&Pnt1[0], MinMax);												// �ŏ���ő���X�V
			break;
		case  HPRIM_LIN:																// ��
			HgTrnPntByMtx(&m_Prim.Line.Pnt[0], Matrix, Move, &Pnt1[0]);
			HgTrnPntByMtx(&m_Prim.Line.Pnt[1], Matrix, Move, &Pnt1[1]);
			memcpy(&Pnt2[0], &Pnt1[0], sizeof(MgPoint2));
			memcpy(&Pnt2[1], &Pnt1[1], sizeof(MgPoint2));
			memcpy(Pnt3, Pnt2, sizeof(MgPoint2) * 2);
			if (CutPnt) {																// ��Ă���
				ItrSts = HgCutLneByCutL(CutPnt, Pnt2, Pnt3);
				if (ItrSts == -1)   goto  PRG_EXIT;										// ���ׂĶ��
			}
			memcpy(&Pnt1[0], &Pnt3[0], sizeof(MgPoint2));
			memcpy(&Pnt1[1], &Pnt3[1], sizeof(MgPoint2));
			SetMinMax(&Pnt1[0], MinMax);												// �ŏ���ő���X�V
			SetMinMax(&Pnt1[1], MinMax);												// �ŏ���ő���X�V
			HnLine(MemDC, MainId, SubsId, m_LineNo, 0, 1, Color, 2, Pnt3);				// ��(���̑����͂P�Œ�)
			break;
		case  HPRIM_CIR:																// �~:
			HgTrnCirInf((MgPoint2 *)&m_Prim.Circle.Pnt1, (MgPoint2 *)&m_Prim.Circle.Pnt2, &CirW1);	// �O�ڂ��鍶��,�E����~�\����
			HgTrnPntByMtx(&CirW1.CPnt, Matrix, Move, &CirW1.CPnt);
			CirW1.Rad = CirW1.Rad * Scale;
			HgTrnCirInf(&CirW1, &Pnt2[0], &Pnt2[1]);									// �~�\���̂��O�ڂ��鍶��,�E��
			ItrSts = 0;
			if (CutPnt) {																// ��Ă���
				ItrSts = HgCutCirByCutL(CutPnt, &Pnt2[0], &Pnt2[1], &Pnt2[2], &Pnt2[3]);
				if (ItrSts == -1)   goto  PRG_EXIT;										// ���ׂĶ��
			}
			if (ItrSts == 0) {															// ��ĂȂ�
				SetMinMax(&Pnt1[0], MinMax);											// �ŏ���ő���X�V
				SetMinMax(&Pnt1[1], MinMax);											// �ŏ���ő���X�V
				HnCir(MemDC, MainId, SubsId, m_LineNo, 0, 1, Color, &Pnt2[0], &Pnt2[1]);// �~��`��(���̑����͂P�ɌŒ�)
			} else {																	// ��Ă���
				Pnt1[0].X = Pnt2[2].X; Pnt1[0].Y = Pnt2[2].Y;
				Pnt1[1].X = Pnt2[3].X; Pnt1[1].Y = Pnt2[2].Y;
				SetMinMax(&Pnt1[0], MinMax);											// �ŏ���ő���X�V
				SetMinMax(&Pnt1[1], MinMax);											// �ŏ���ő���X�V
				HnArc(MemDC, MainId, SubsId, m_LineNo, 0, 1, Color, &Pnt2[0], &Pnt2[1], &Pnt2[2], &Pnt2[3]);	// �~��(���̑����͂P�Œ�)
			}
			break;
		case  HPRIM_ARC:																// �~��
			HgTrnCirInf((MgPoint2 *)&m_Prim.Arc.Pnt1, (MgPoint2 *)&m_Prim.Arc.Pnt2, &CirW1);		// �O�ڂ��鍶��,�E����~�\����
			HgTrnPntByMtx(&CirW1.CPnt, Matrix, Move, &CirW1.CPnt);
			HgTrnPntByMtx(&m_Prim.Arc.Pnt3, Matrix, Move, &Pnt1[2]);
			HgTrnPntByMtx(&m_Prim.Arc.Pnt4, Matrix, Move, &Pnt1[3]);

			ItrSts = HgGetCPntByPnt(&CirW1.CPnt, CirW1.Rad, (MgPoint2 *)&Pnt1[2], (MgPoint2 *)&Pnt1[3], &CirW1.CPnt);	// �ړ���̉~�̒��_
			HgTrnCirInf(&CirW1, &Pnt2[0], &Pnt2[1]);									// �~�\���̂��O�ڂ��鍶��,�E��
			if (Symtry == 0) {															// �Ώ̂Ȃ�
				memcpy(&Pnt2[2], &Pnt1[2], sizeof(MgPoint2));
				memcpy(&Pnt2[3], &Pnt1[3], sizeof(MgPoint2));
			} else {																	// �Ώ̂���
				memcpy(&Pnt2[2], &Pnt1[3], sizeof(MgPoint2));
				memcpy(&Pnt2[3], &Pnt1[2], sizeof(MgPoint2));
			}
			ItrSts = 0;
			if (CutPnt) {																// ��Ă���
				ItrSts = HgCutArcByCutL(CutPnt, &Pnt2[0], &Pnt2[1], &Pnt2[2], &Pnt2[3], Pnt3, Pnt4, Pnt5, Pnt6);
				if (ItrSts == -1)   goto  PRG_EXIT;										// ���ׂĶ��
			}
			if (ItrSts == 0) {															// ��ĂȂ�
				SetMinMax(&Pnt1[2], MinMax);											// �ŏ���ő���X�V
				SetMinMax(&Pnt1[3], MinMax);											// �ŏ���ő���X�V
				HnArc(MemDC, MainId, SubsId, m_LineNo, 0, 1, Color, &Pnt2[0], &Pnt2[1], &Pnt2[2], &Pnt2[3]);	// �~��(���̑����͂P�Œ�)
			} else {																	// ��Ă���
				for (ic = 0 ; ic < ItrSts ; ic++) {
					Pnt1[0].X = Pnt5[ic].X; Pnt1[0].Y = Pnt5[ic].Y;
					Pnt1[1].X = Pnt6[ic].X; Pnt1[1].Y = Pnt6[ic].Y;
					SetMinMax(&Pnt1[0], MinMax);										// �ŏ���ő���X�V
					SetMinMax(&Pnt1[1], MinMax);										// �ŏ���ő���X�V
					HnArc(MemDC, MainId, SubsId, m_LineNo, 0, 1, Color, &Pnt3[ic], &Pnt4[ic], &Pnt5[ic], &Pnt6[ic]);	// �~��
				}
			}
			break;
		case  HPRIM_TXT:																// ������
			break;
		case  HPRIM_POL:																// ��
			Num = m_Prim.Face.NumPnt;
			if (HgCmpPnt(&m_Prim.Face.Pnt[0], &m_Prim.Face.Pnt[m_Prim.Face.NumPnt-1], 1.0f) == 1) {	// �n�_��I�_����
				Num--;
			} else {
				if (m_Prim.Face.CFlag == 0)  Num--;
			}

			for (ic = 0 ; ic < Num ; ic++) {
				nc = HPTR(ic+1, m_Prim.Face.NumPnt);
				HgTrnPntByMtx(&m_Prim.Face.Pnt[ic], Matrix, Move, &Pnt1[0]);
				SetMinMax(&Pnt1[0], MinMax);											// �ŏ���ő���X�V
				memcpy(&Pnt2[0], &Pnt1[0], sizeof(MgPoint2));
				HgTrnPntByMtx(&m_Prim.Face.Pnt[nc], Matrix, Move, &Pnt1[1]);
				SetMinMax(&Pnt1[1], MinMax);											// �ŏ���ő���X�V
				memcpy(&Pnt2[1], &Pnt1[1], sizeof(MgPoint2));
				HnLine(MemDC, MainId, SubsId, m_LineNo, 0, 1, Color, 2, Pnt2);			// ����`��(���̑����͂P�ɌŒ�)
			}
			break;
		case  HPRIM_MSH:																// ү��
			No = 0;
			for (ic = 0 ; ic < m_Prim.Mesh.Num ; ic++) {								// �ʂ̐�
				for (jc = 0 ; jc < m_Prim.Mesh.VerNumPnt[ic] ; jc++) {					// ���_��
					nc  = HPTR(jc+1, m_Prim.Mesh.VerNumPnt[ic]);
					Idx = m_Prim.Mesh.PntNo[No+jc];
					HgTrnPntByMtx(&m_Prim.Mesh.Pnt[Idx], Matrix, Move, &Pnt1[0]);
					memcpy(&Pnt2[0], &Pnt1[0], sizeof(MgPoint2));
					Idx = m_Prim.Mesh.PntNo[No+nc];
					HgTrnPntByMtx(&m_Prim.Mesh.Pnt[Idx], Matrix, Move, &Pnt1[1]);
					memcpy(&Pnt2[1], &Pnt1[0], sizeof(MgPoint2));
					HnLine(MemDC, MainId, SubsId, m_LineNo, 0, 1, Color, 2, Pnt2);		// ����`��(���̑����͂P�ɌŒ�)
				}
				No += m_Prim.Mesh.VerNumPnt[ic];
			}
			break;
		default:
			break;
	}

PRG_EXIT:

	return;
}

/******************************************************************************************************************
[ MDxfPrim::MakeRL ]                  �@�\  ���ʏ��̍쐬�i���j
******************************************************************************************************************/
HVOID MDxfPrim::MakeRL        (
								MINT				VewCod,								// (I  ) �ޭ����� 0:�� 1:�� 2:�� 3:�k
								MINT				Kai,								// (I  ) �K
								HnDisp*				DispRL,								// (I  ) ���ʕ\�����i�����j
								COLORREF			Rgb,								// (I  ) �\���F
								MINT				Symtry,								// (I  ) �Ώ��׸�  0:�Ȃ�  1:����
								HMTRX3				Matrix,								// (I  ) �ϊ���ظ�
								MgPoint3*			Move,								// (I  ) �ړ���
								MFLOAT				Scale,								// (I  ) ����
								MgPoint3*			TOPnt,								// (I  ) 2������3�����ϊ��p���_
								MgPoint3*			TUvw,								// (I  ) 2������3�����ϊ��pUVW
								MgMinMax3*			MinMax)								// (  O) �ŏ���ő�
{
	MgPoint3				PntW1[4];
	MgPoint2				PntW2[4];
	MgPoint2				PntW3[4];
	MINT				Num;
	MINT				NumL;
	HLINE2*				LineW1   = NULL;
	MINT				Idx, No;
	MINT				ItrSts;
	MINT				ic, jc, kc, nc;


	if ((LineW1 = (HLINE2  *)malloc(sizeof(HLINE2) * 256)) == NULL)  HMFAIL;

	switch  (m_PrmType) {
		case  HPRIM_DOT:																// �_
			HgTrnPntByMtx(&m_Prim.Dot, Matrix, Move, &PntW1[0]);
			TrnWrdToLoc(TOPnt, TUvw, &PntW1[0], &PntW2[0]);								// ۰�ٍ��W�n�֕ϊ�
			SetMinMax(&PntW2[0], MinMax);												// �ŏ���ő���X�V
			break;
		case  HPRIM_LIN:																// ��
			HgTrnPntByMtx(&m_Prim.Line.Pnt[0], Matrix, Move, &PntW1[0]);
			HgTrnPntByMtx(&m_Prim.Line.Pnt[1], Matrix, Move, &PntW1[1]);
			NumL = HiddenLine(VewCod, Kai, TOPnt, TUvw, 2, PntW1, LineW1);				// �A������
			for (ic = 0 ; ic < NumL ; ic++) {
				DispRL->AddLine(m_LineNo, 0, 1, Rgb, 2, (MgPoint2 *)&LineW1[ic]);		// ����`��
				SetMinMax((MgPoint2 *)&LineW1[ic].Pnt[0], MinMax);						// �ŏ���ő���X�V
				SetMinMax((MgPoint2 *)&LineW1[ic].Pnt[1], MinMax);						// �ŏ���ő���X�V
			}
			break;
		case  HPRIM_CIR:																// �~:
			HgTrnPntByMtx(&m_Prim.Circle.Pnt1, Matrix, Move, &PntW1[0]);
			HgTrnPntByMtx(&m_Prim.Circle.Pnt2, Matrix, Move, &PntW1[1]);

			ItrSts = HiddenCheck(VewCod, Kai, TOPnt, TUvw, 2, PntW1);					// �A��������
			if (ItrSts == 0)  break;

			TrnWrdToLoc(TOPnt, TUvw, &PntW1[0], &PntW2[0]);								// ۰�ٍ��W�n�֕ϊ�
			TrnWrdToLoc(TOPnt, TUvw, &PntW1[1], &PntW2[1]);								// ۰�ٍ��W�n�֕ϊ�
			PntW3[0].X = HMIN(PntW2[0].X, PntW2[1].X);
			PntW3[0].Y = HMIN(PntW2[0].Y, PntW2[1].Y);
			PntW3[1].X = HMAX(PntW2[0].X, PntW2[1].X);
			PntW3[1].Y = HMAX(PntW2[0].Y, PntW2[1].Y);
			SetMinMax(&PntW3[0], MinMax);												// �ŏ���ő���X�V
			SetMinMax(&PntW3[1], MinMax);												// �ŏ���ő���X�V
			DispRL->AddCir(m_LineNo, 0, 1, Rgb, &PntW3[0], &PntW3[1]);					// �~��`��
			break;
		case  HPRIM_ARC:																// �~��
			HgTrnPntByMtx(&m_Prim.Arc.Pnt1, Matrix, Move, &PntW1[0]);
			HgTrnPntByMtx(&m_Prim.Arc.Pnt2, Matrix, Move, &PntW1[1]);
			HgTrnPntByMtx(&m_Prim.Arc.Pnt3, Matrix, Move, &PntW1[2]);
			HgTrnPntByMtx(&m_Prim.Arc.Pnt4, Matrix, Move, &PntW1[3]);

			ItrSts = HiddenCheck(VewCod, Kai, TOPnt, TUvw, 2, &PntW1[2]);				// �A��������
			if (ItrSts == 0)  break;

			TrnWrdToLoc(TOPnt, TUvw, &PntW1[0], &PntW2[0]);								// ۰�ٍ��W�n�֕ϊ�
			TrnWrdToLoc(TOPnt, TUvw, &PntW1[1], &PntW2[1]);								// ۰�ٍ��W�n�֕ϊ�
			TrnWrdToLoc(TOPnt, TUvw, &PntW1[2], &PntW2[2]);								// ۰�ٍ��W�n�֕ϊ�
			TrnWrdToLoc(TOPnt, TUvw, &PntW1[3], &PntW2[3]);								// ۰�ٍ��W�n�֕ϊ�
			PntW3[0].X = HMIN(PntW2[0].X, PntW2[1].X);
			PntW3[0].Y = HMIN(PntW2[0].Y, PntW2[1].Y);
			PntW3[1].X = HMAX(PntW2[0].X, PntW2[1].X);
			PntW3[1].Y = HMAX(PntW2[0].Y, PntW2[1].Y);
			if ((Symtry == 0 && TUvw->X > 0.f) || (Symtry == 1 && TUvw->X < 0.f) ||
				(Symtry == 0 && TUvw->Y > 0.f) || (Symtry == 1 && TUvw->Y < 0.f)) {
				PntW3[2] = PntW2[2];
				PntW3[3] = PntW2[3];
			} else {																	// �Ώ�
				PntW3[2] = PntW2[3];
				PntW3[3] = PntW2[2];
			}
			SetMinMax(&PntW3[2], MinMax);												// �ŏ���ő���X�V
			SetMinMax(&PntW3[3], MinMax);												// �ŏ���ő���X�V
			DispRL->AddArc(m_LineNo, 0, 1, Rgb, &PntW3[0], &PntW3[1], &PntW3[2], &PntW3[3]);	// �~�ʂ�`��
			break;
		case  HPRIM_TXT:																// ������
			break;
		case  HPRIM_POL:																// ��
			Num = m_Prim.Face.NumPnt;
			if (HgCmpPnt(&m_Prim.Face.Pnt[0], &m_Prim.Face.Pnt[m_Prim.Face.NumPnt-1], 1.0f) == 1) {	// �n�_��I�_����
				Num--;
			} else {
				if (m_Prim.Face.CFlag == 0)  Num--;
			}

			for (ic = 0 ; ic < Num ; ic++) {
				nc = HPTR(ic+1, m_Prim.Face.NumPnt);

				HgTrnPntByMtx(&m_Prim.Face.Pnt[ic], Matrix, Move, &PntW1[0]);
				HgTrnPntByMtx(&m_Prim.Face.Pnt[nc], Matrix, Move, &PntW1[1]);
				NumL = HiddenLine(VewCod, Kai, TOPnt, TUvw, 2, PntW1, LineW1);			// �A������
				for (jc = 0 ; jc < NumL ; jc++) {
					DispRL->AddLine(m_LineNo, 0, 1, Rgb, 2, (MgPoint2 *)&LineW1[jc]);	// ����`��
					SetMinMax((MgPoint2 *)&LineW1[jc].Pnt[0], MinMax);					// �ŏ���ő���X�V
					SetMinMax((MgPoint2 *)&LineW1[jc].Pnt[1], MinMax);					// �ŏ���ő���X�V
				}
			}
			break;
		case  HPRIM_MSH:																// ү��
			No = 0;
			for (ic = 0 ; ic < m_Prim.Mesh.Num ; ic++) {								// �ʂ̐�
				for (jc = 0 ; jc < m_Prim.Mesh.VerNumPnt[ic] ; jc++) {					// ���_��
					nc  = HPTR(jc+1, m_Prim.Mesh.VerNumPnt[ic]);
					Idx = m_Prim.Mesh.PntNo[No+jc];
					HgTrnPntByMtx(&m_Prim.Mesh.Pnt[Idx], Matrix, Move, &PntW1[0]);
					Idx = m_Prim.Mesh.PntNo[No+nc];
					HgTrnPntByMtx(&m_Prim.Mesh.Pnt[Idx], Matrix, Move, &PntW1[1]);
					NumL = HiddenLine(VewCod, Kai, TOPnt, TUvw, 2, PntW1, LineW1);		// �A������
					for (kc = 0 ; kc < NumL ; kc++) {
						DispRL->AddLine(m_LineNo, 0, 1, Rgb, 2, (MgPoint2 *)&LineW1[kc]);// ����`��
						SetMinMax((MgPoint2 *)&LineW1[kc].Pnt[0], MinMax);				// �ŏ���ő���X�V
						SetMinMax((MgPoint2 *)&LineW1[kc].Pnt[1], MinMax);				// �ŏ���ő���X�V
					}
				}
				No += m_Prim.Mesh.VerNumPnt[ic];
			}
			break;
		default:
			break;
	}

	HFREE(LineW1);

	return;
}

/******************************************************************************************************************
[ MDxfPrim::HiddenLine ]              �@�\  �A������
******************************************************************************************************************/
MINT MDxfPrim::HiddenLine     (														// (  O) ������
 								MINT				VewCod,								// (I  ) �ޭ����� 0:�� 1:�� 2:�� 3:�k
								MINT				Kai,								// (I  ) �K
								MgPoint3*			TOPnt,								// (I  ) 2������3�����ϊ��p���_
								MgPoint3*			TUvw,								// (I  ) 2������3�����ϊ��pUVW
								MINT				NumPnt1,							// (I  ) ���_��
								MgPoint3*			Pnt1,								// (I  ) ���_
								HLINE2*				Line1)								// (  O) ����
{
	MINT				NumL;															// ������
	MINT				NumC;
	MINT				NumW1;
	MgPoint2*			PntC     = NULL;
	MINT*				MFlagW   = NULL;
	MINT*				NumPntW1 = NULL;
	MgPoint2*			PntW1    = NULL;
	MgPoint2				PntW2;
	MgPoint3				PntW3;
	HLINE2				LineW1;
	MgMinMax2			MinMaxW1;
	MFLOAT				DepthW1;
	MINT				SFlag;
	MINT				ItrSts;
	MINT				ic, jc, nc;


	NumL = 0;																			// ������

	if ((PntC     = (MgPoint2 *)malloc(sizeof(MgPoint2) * 256)) == NULL)  HMFAIL;
	if ((MFlagW   = (MINT    *)malloc(sizeof(MINT)    *  64)) == NULL)  HMFAIL;
	if ((NumPntW1 = (MINT    *)malloc(sizeof(MINT)    *  64)) == NULL)  HMFAIL;
	if ((PntW1    = (MgPoint2 *)malloc(sizeof(MgPoint2) * 256)) == NULL)  HMFAIL;

	HgTrnWrdToLoc(TOPnt, TUvw, NumPnt1, Pnt1, PntW1);									// ۰�ٍ��W�n�֕ϊ�
	ItrSts = HgGetMinMax(NumPnt1, PntW1, &MinMaxW1);									// �ŏ��ő�

	for (ic = 0 ; ic < NumPnt1 ; ic++) {
		nc = HPTR(ic+1, NumPnt1);
		LineW1.Pnt[0] = PntW1[ic]; LineW1.Pnt[1] = PntW1[nc];
		NumC  = HsDHonGetCrsPnt(VewCod, TOPnt, TUvw, Kai, NULL, &MinMaxW1, &LineW1, PntC);				// ��_���𓾂�
		NumW1 = HsRoofGetCrsPnt(VewCod, TOPnt, TUvw, Kai, NULL, -1, &MinMaxW1, &LineW1, &PntC[NumC]);	// ��_���𓾂�
		NumC += NumW1;
		NumW1 = HsDHaiGetCrsPnt(VewCod, TOPnt, TUvw, Kai, NULL, &MinMaxW1, &LineW1, &PntC[NumC]);		// ��_���𓾂�
		NumC += NumW1;

		HgPackCrsPnt(&LineW1, &NumC, PntC);												// �d��������
		for (jc = 0 ; jc < NumC - 1 ; jc++) {											// ��_��
			PntW2.X = (PntC[jc].X + PntC[jc+1].X) / 2;									// ���_
			PntW2.Y = (PntC[jc].Y + PntC[jc+1].Y) / 2;
			HgTrnLocToWrd(TOPnt, TUvw, 1, &PntW2, &PntW3);								// ܰ��ލ��W�l�ɕϊ�
			DepthW1 = HsGetDepth(VewCod, &PntW3, NumPnt1, Pnt1);						// ���s�������߂�
			if (VewCod == 0 || VewCod == 3) {											// 0:�� 3:�k
				DepthW1 += 10.f;
			} else {																	// 1:�� 2:��
				DepthW1 -= 10.f;
			}
			SFlag = HsDHonChkVisible(VewCod, TOPnt, TUvw, Kai, NULL, &PntW2, &PntW3, DepthW1);				// ���𔻒f
			if (SFlag == 1) {															// ������
				SFlag = HsRoofChkVisible(VewCod, TOPnt, TUvw, Kai, NULL, -1, &PntW2, &PntW3, DepthW1);		// ���𔻒f
				if (SFlag == 1) {
					SFlag = HsDHaiChkVisible(VewCod, TOPnt, TUvw, Kai, NULL, &PntW2, &PntW3, DepthW1);		// ���𔻒f
					if (SFlag == 1) {
						Line1[NumL].Pnt[0] = PntC[jc]; Line1[NumL].Pnt[1] = PntC[jc+1]; NumL++;
					}
				}
			}
		}
	}

	HFREE(PntC);
	HFREE(MFlagW);
	HFREE(NumPntW1);
	HFREE(PntW1);

	return(NumL);
}
/******************************************************************************************************************
[ MDxfPrim::HiddenCheck ]             �@�\  �A���`�F�b�N�i�ŏ��E�ő�j
******************************************************************************************************************/
MINT MDxfPrim::HiddenCheck    (														// (  O) 0:�����Ȃ� 1:������
 								MINT				VewCod,								// (I  ) �ޭ����� 0:�� 1:�� 2:�� 3:�k
								MINT				Kai,								// (I  ) �K
								MgPoint3*			TOPnt,								// (I  ) 2������3�����ϊ��p���_
								MgPoint3*			TUvw,								// (I  ) 2������3�����ϊ��pUVW
								MINT				NumPnt1,							// (I  ) ���_��
								MgPoint3*			Pnt1)								// (I  ) ���_
{
	MINT				Status;
	MgPoint2*			PntW1 = NULL;
	MFLOAT				DepthW1;
	MINT				ic;


	Status = 1;																			// ������

	if ((PntW1 = (MgPoint2 *)malloc(sizeof(MgPoint2) * NumPnt1)) == NULL)  HMFAIL;

	HgTrnWrdToLoc(TOPnt, TUvw, NumPnt1, Pnt1, PntW1);									// ۰�ٍ��W�n�֕ϊ�
	for (ic = 0 ; ic < NumPnt1 ; ic++) {
		DepthW1 = HsGetDepth(VewCod, &Pnt1[ic], NumPnt1, Pnt1);							// ���s�������߂�
		if (VewCod == 0 || VewCod == 3) {												// 0:�� 3:�k
			DepthW1 += 10.f;
		} else {																		// 1:�� 2:��
			DepthW1 -= 10.f;
		}
		Status = HsDHonChkVisible(VewCod, TOPnt, TUvw, Kai, NULL, &PntW1[ic], &Pnt1[ic], DepthW1);		// ���𔻒f
		if (Status == 0)  break;														// �����Ȃ�
		Status = HsRoofChkVisible(VewCod, TOPnt, TUvw, Kai, NULL, -1, &PntW1[ic], &Pnt1[ic], DepthW1);	// ���𔻒f
		if (Status == 0)  break;														// �����Ȃ�
		Status = HsDHaiChkVisible(VewCod, TOPnt, TUvw, Kai, NULL, &PntW1[ic], &Pnt1[ic], DepthW1);		// ���𔻒f
		if (Status == 0)  break;														// �����Ȃ�
	}

	HFREE(PntW1);

	return(Status);
}

/******************************************************************************************************************
[ MDxfPrim::NumDxfPrim ]              �@�\  �v���~�e�B�u���𓾂�
******************************************************************************************************************/
MINT MDxfPrim::NumDxfPrim     (														// (  O) ����è�ސ�
								MINT				Symtry,								// (I  ) �Ώ��׸�  0:�Ȃ�  1:����
								HMTRX3				Matrix,								// (I  ) �ϊ���ظ�
								MgPoint3*			Move,								// (I  ) �ړ���
								MFLOAT				Scale,								// (I  ) ����
								MgPoint2*			CutPnt)								// (I  ) ��Đ� + �����_
{
	MINT				Num;
	MgPoint3				Pnt1[4];
	MgPoint2				Pnt2[4];
	MgPoint2				Pnt3[4];
	MgPoint2				Pnt4[4];
	MgPoint2				Pnt5[4];
	MgPoint2				Pnt6[4];
	HCIR2				CirW1;
	MINT				ItrSts;
	MINT				ic, jc;


	Num = 0;
	switch  (m_PrmType) {
		case  HPRIM_DOT:																// �_
			HgTrnPntByMtx(&m_Prim.Dot, Matrix, Move, &Pnt1[0]);
			memcpy(&Pnt2[0], &Pnt1[0], sizeof(MgPoint2));
			if (CutPnt) {																// ��Ă���
				ItrSts = HgCutPntByCutL(CutPnt, &Pnt2[0]);
				if (ItrSts == -1)  goto  PRG_EXIT;
			}
			Num = 1;
			break;
		case  HPRIM_LIN:																// ��
			HgTrnPntByMtx(&m_Prim.Line.Pnt[0], Matrix, Move, &Pnt1[0]);
			HgTrnPntByMtx(&m_Prim.Line.Pnt[1], Matrix, Move, &Pnt1[1]);
			memcpy(&Pnt2[0], &Pnt1[0], sizeof(MgPoint2));
			memcpy(&Pnt2[1], &Pnt1[1], sizeof(MgPoint2));
			if (CutPnt) {																// ��Ă���
				ItrSts = HgCutLneByCutL(CutPnt, Pnt2, Pnt3);
				if (ItrSts == -1)   goto  PRG_EXIT;										// ���ׂĶ��
			}
			Num = 1;
			break;
		case  HPRIM_CIR:																// �~:
			HgTrnCirInf((MgPoint2 *)&m_Prim.Circle.Pnt1, (MgPoint2 *)&m_Prim.Circle.Pnt2, &CirW1);	// �O�ڂ��鍶��,�E����~�\����
			HgTrnPntByMtx(&CirW1.CPnt, Matrix, Move, &CirW1.CPnt);
			CirW1.Rad = CirW1.Rad * Scale;
			HgTrnCirInf(&CirW1, &Pnt2[0], &Pnt2[1]);									// �~�\���̂��O�ڂ��鍶��,�E��
			if (CutPnt) {																// ��Ă���
				ItrSts = HgCutCirByCutL(CutPnt, &Pnt2[0], &Pnt2[1], &Pnt2[2], &Pnt2[3]);
				if (ItrSts == -1)   goto  PRG_EXIT;										// ���ׂĶ��
			}
			Num = 1;
			break;
		case  HPRIM_ARC:																// �~��
			HgTrnCirInf((MgPoint2 *)&m_Prim.Arc.Pnt1, (MgPoint2 *)&m_Prim.Arc.Pnt2, &CirW1);	// �O�ڂ��鍶��,�E����~�\����
			HgTrnPntByMtx(&CirW1.CPnt, Matrix, Move, &CirW1.CPnt);
			CirW1.Rad = CirW1.Rad * Scale;
			HgTrnCirInf(&CirW1, &Pnt2[0], &Pnt2[1]);									// �~�\���̂��O�ڂ��鍶��,�E��
			HgTrnPntByMtx(&m_Prim.Arc.Pnt3, Matrix, Move, &Pnt1[2]);
			HgTrnPntByMtx(&m_Prim.Arc.Pnt4, Matrix, Move, &Pnt1[3]);
			if (Symtry == 0) {															// �Ώ̂Ȃ�
				memcpy(&Pnt2[2], &Pnt1[2], sizeof(MgPoint2));
				memcpy(&Pnt2[3], &Pnt1[3], sizeof(MgPoint2));
			} else {																	// �Ώ̂���
				memcpy(&Pnt2[2], &Pnt1[3], sizeof(MgPoint2));
				memcpy(&Pnt2[3], &Pnt1[2], sizeof(MgPoint2));
			}
			ItrSts = 0;
			if (CutPnt) {																// ��Ă���
				ItrSts = HgCutArcByCutL(CutPnt, &Pnt2[0], &Pnt2[1], &Pnt2[2], &Pnt2[3], Pnt3, Pnt4, Pnt5, Pnt6);
				if (ItrSts == -1)   goto  PRG_EXIT;										// ���ׂĶ��
			}
			if (ItrSts == 0) {															// ��ĂȂ�
				Num = 1;
			} else {																	// ��Ă���
				Num = ItrSts;
			}
			break;
		case  HPRIM_TXT:																// ������
			Num = 1;
			break;
		case  HPRIM_POL:																// ��
			Num = m_Prim.Face.NumPnt;
			if (HgCmpPnt(&m_Prim.Face.Pnt[0], &m_Prim.Face.Pnt[m_Prim.Face.NumPnt-1], 1.0f) == 1) {	// �n�_��I�_����
				Num--;
			} else {
				if (m_Prim.Face.CFlag == 0)  Num--;
			}

			break;
		case  HPRIM_MSH:																// ү��
			for (ic = 0 ; ic < m_Prim.Mesh.Num ; ic++) {								// �ʂ̐�
				for (jc = 0 ; jc < m_Prim.Mesh.VerNumPnt[ic] ; jc++) {					// ���_��
					Num++;
				}
			}
			break;
		default:
			break;
	}

PRG_EXIT:

	return(Num);
}

/******************************************************************************************************************
[ MDxfPrim::DxfR14Out ]               �@�\   �c�w�e�i�q�P�S�j�`���o��
******************************************************************************************************************/
MINT MDxfPrim::DxfR14Out      (														// (  O) �ð��  0:���� �ȊO:�װ
								CStdioFile*			FLPtr,								// (I  ) ̧�َ��ʎq
								MINT*				NumHnd,								// (I/O) ����ِ�
								MCHAR*				LayNam,								// (I  ) ڲԖ�
								MINT				Symtry,								// (I  ) �Ώ��׸�  0:�Ȃ�  1:����
								HMTRX3				Matrix,								// (I  ) �ϊ���ظ�
								MgPoint3*			Move,								// (I  ) �ړ���
								MFLOAT				Scale,								// (I  ) ����
								MgPoint2*			CutPnt)
{
	MINT				Status;
	MgPoint3				Pnt1[4];
	MgPoint2				Pnt2[4];
	MgPoint2				Pnt3[4];
	MgPoint2				Pnt4[4];
	MgPoint2				Pnt5[4];
	MgPoint2				Pnt6[4];
	HCIR2				CirW1;
	MINT				Num;
	MINT				Idx, No;
	MINT				ItrSts;
	MINT				ic, jc, nc;


	Status = 0;

	switch  (m_PrmType) {
		case  HPRIM_DOT:																// �_
			HgTrnPntByMtx(&m_Prim.Dot, Matrix, Move, &Pnt1[0]);
			memcpy(&Pnt2[0], &Pnt1[0], sizeof(MgPoint2));
			if (CutPnt) {																// ��Ă���
				ItrSts = HgCutPntByCutL(CutPnt, &Pnt2[0]);
				if (ItrSts == -1)  goto  PRG_EXIT;										// ���ׂĶ��
			}
			Status = MDxfR14Dot(FLPtr, NumHnd, LayNam, 1, 1, &Pnt2[0]);
			break;
		case  HPRIM_LIN:																// ��
			HgTrnPntByMtx(&m_Prim.Line.Pnt[0], Matrix, Move, &Pnt1[0]);
			HgTrnPntByMtx(&m_Prim.Line.Pnt[1], Matrix, Move, &Pnt1[1]);
			memcpy(&Pnt2[0], &Pnt1[0], sizeof(MgPoint2));
			memcpy(&Pnt2[1], &Pnt1[1], sizeof(MgPoint2));
			memcpy(Pnt3, Pnt2, sizeof(MgPoint2) * 2);
			if (CutPnt) {																// ��Ă���
				ItrSts = HgCutLneByCutL(CutPnt, Pnt2, Pnt3);
				if (ItrSts == -1)   goto  PRG_EXIT;										// ���ׂĶ��
			}
			Status = MDxfR14Line(FLPtr, NumHnd, LayNam, m_LineNo, 1, 2, Pnt3);
			break;
		case  HPRIM_CIR:																// �~:
			HgTrnCirInf((MgPoint2 *)&m_Prim.Circle.Pnt1, (MgPoint2 *)&m_Prim.Circle.Pnt2, &CirW1);	// �O�ڂ��鍶��,�E����~�\����
			HgTrnPntByMtx(&CirW1.CPnt, Matrix, Move, &CirW1.CPnt);
			CirW1.Rad = CirW1.Rad * Scale;
			HgTrnCirInf(&CirW1, &Pnt2[0], &Pnt2[1]);									// �~�\���̂��O�ڂ��鍶��,�E��
			ItrSts = 0;
			if (CutPnt) {																// ��Ă���
				ItrSts = HgCutCirByCutL(CutPnt, &Pnt2[0], &Pnt2[1], &Pnt2[2], &Pnt2[3]);
				if (ItrSts == -1)   goto  PRG_EXIT;										// ���ׂĶ��
			}
			if (ItrSts == 0) {															// ��ĂȂ�
				Status = MDxfR14Cir(FLPtr, NumHnd, LayNam, m_LineNo, 1, &Pnt2[0], &Pnt2[1]);						// �~
			} else {																	// ��Ă���
				Status = MDxfR14Arc(FLPtr, NumHnd, LayNam, m_LineNo, 1, &Pnt2[0], &Pnt2[1], &Pnt2[2], &Pnt2[3]);	// �~��
			}
			break;
		case  HPRIM_ARC:																// �~��
			HgTrnCirInf((MgPoint2 *)&m_Prim.Arc.Pnt1, (MgPoint2 *)&m_Prim.Arc.Pnt2, &CirW1);	// �O�ڂ��鍶��,�E����~�\����
			HgTrnPntByMtx(&CirW1.CPnt, Matrix, Move, &CirW1.CPnt);
			CirW1.Rad = CirW1.Rad * Scale;
			HgTrnCirInf(&CirW1, &Pnt2[0], &Pnt2[1]);									// �~�\���̂��O�ڂ��鍶��,�E��
			HgTrnPntByMtx(&m_Prim.Arc.Pnt3, Matrix, Move, &Pnt1[2]);
			HgTrnPntByMtx(&m_Prim.Arc.Pnt4, Matrix, Move, &Pnt1[3]);
			if (Symtry == 0) {															// �Ώ̂Ȃ�
				memcpy(&Pnt2[2], &Pnt1[2], sizeof(MgPoint2));
				memcpy(&Pnt2[3], &Pnt1[3], sizeof(MgPoint2));
			} else {																	// �Ώ̂���
				memcpy(&Pnt2[2], &Pnt1[3], sizeof(MgPoint2));
				memcpy(&Pnt2[3], &Pnt1[2], sizeof(MgPoint2));
			}
			ItrSts = 0;
			if (CutPnt) {																// ��Ă���
				ItrSts = HgCutArcByCutL(CutPnt, &Pnt2[0], &Pnt2[1], &Pnt2[2], &Pnt2[3], Pnt3, Pnt4, Pnt5, Pnt6);
				if (ItrSts == -1)   goto  PRG_EXIT;										// ���ׂĶ��
			}
			if (ItrSts == 0) {															// ��ĂȂ�
				Status = MDxfR14Arc(FLPtr, NumHnd, LayNam, m_LineNo, 1, &Pnt2[0], &Pnt2[1], &Pnt2[2], &Pnt2[3]);	// �~��
			} else {																	// ��Ă���
				for (ic = 0 ; ic < ItrSts ; ic++) {
					Status = MDxfR14Arc(FLPtr, NumHnd, LayNam, m_LineNo, 1, &Pnt3[ic], &Pnt4[ic], &Pnt5[ic], &Pnt6[ic]);	// �~��
				}
			}
			break;
		case  HPRIM_TXT:																// ������
			break;
		case  HPRIM_POL:																// ��
			Num = m_Prim.Face.NumPnt;
			if (HgCmpPnt(&m_Prim.Face.Pnt[0], &m_Prim.Face.Pnt[m_Prim.Face.NumPnt-1], 1.0f) == 1) {	// �n�_��I�_����
				Num--;
			} else {
				if (m_Prim.Face.CFlag == 0)  Num--;
			}

			for (ic = 0 ; ic < Num ; ic++) {
				nc = HPTR(ic+1, m_Prim.Face.NumPnt);
				HgTrnPntByMtx(&m_Prim.Face.Pnt[ic], Matrix, Move, &Pnt1[0]);
				memcpy(&Pnt2[0], &Pnt1[0], sizeof(MgPoint2));
				HgTrnPntByMtx(&m_Prim.Face.Pnt[nc], Matrix, Move, &Pnt1[1]);
				memcpy(&Pnt2[1], &Pnt1[1], sizeof(MgPoint2));
				Status = MDxfR14Line(FLPtr, NumHnd, LayNam, m_LineNo, 1, 2, Pnt2);
			}
			break;
		case  HPRIM_MSH:																// ү��
			No = 0;
			for (ic = 0 ; ic < m_Prim.Mesh.Num ; ic++) {								// �ʂ̐�
				for (jc = 0 ; jc < m_Prim.Mesh.VerNumPnt[ic] ; jc++) {					// ���_��
					nc  = HPTR(jc+1, m_Prim.Mesh.VerNumPnt[ic]);
					Idx = m_Prim.Mesh.PntNo[No+jc];
					HgTrnPntByMtx(&m_Prim.Mesh.Pnt[Idx], Matrix, Move, &Pnt1[0]);
					memcpy(&Pnt2[0], &Pnt1[0], sizeof(MgPoint2));
					Idx = m_Prim.Mesh.PntNo[No+nc];
					HgTrnPntByMtx(&m_Prim.Mesh.Pnt[Idx], Matrix, Move, &Pnt1[1]);
					memcpy(&Pnt2[1], &Pnt1[0], sizeof(MgPoint2));
					Status = MDxfR14Line(FLPtr, NumHnd, LayNam, m_LineNo, 1, 2, Pnt2);
				}
				No += m_Prim.Mesh.VerNumPnt[ic];
			}
			break;
		default:
			break;
	}

PRG_EXIT:

	return(Status);
}

/******************************************************************************************************************
[ MDxfPrim::SetDirectXF ]              �@�\  �ʂ��c�����������w�ɐݒ肷��
******************************************************************************************************************/
HVOID MDxfPrim::SetDirectXF   (
								LPDIRECT3DDEVICE8	LpD3DDEV,							// (I  ) DirectX���޲�
								COLORREF			Rgb1,								// (I  ) RGB�װ�l
								MINT				FAFlag)								// (I  ) �ʂ̌�������   0:���Ȃ�  1:����
{
	HDXVERTEX*			Vertex  = NULL;
	HDXVERTEX*			VMemory = NULL;
	MgPoint3				PntW1[64];
	MgPoint3				PntW2[64];
	MgPoint2				PntT[64];
	D3DCOLOR			Rgb;															// �F
	MINT				Red, Green, Blue;
	MgPoint3				Vect;															// �@������
	MINT				Num;
	MINT				NumTri;
	MINT				Ip1, Ip2, Idx;
	MINT				ic, jc, kc;


	if ((Vertex = (HDXVERTEX *)malloc(sizeof(HDXVERTEX) * 2048 * 3)) == NULL)  HMFAIL;

	LpD3DDEV->SetVertexShader(HDX_LVERTEX);												// ���_̫�ϯĐݒ�

	Red   = REDCOL(Rgb1); Green = GREENCOL(Rgb1); Blue  = BLUECOL(Rgb1);
	Rgb   = D3DCOLOR_RGBA(Red, Green, Blue, 255);
	/*
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	#ifdef _DEBUG
	MCHAR			StrBuf[256];
	BwsSprintf(StrBuf, BwsStr("�c�w�e �^�C�v   [%d] \n"), m_PrmType);
	TRACE(StrBuf);
	#endif
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	*/
	switch  (m_PrmType) {
		case  HPRIM_DOT:																// �_
			break;
		case  HPRIM_LIN:																// ��
			/*
			/////////////////////////////////////////////////////////////////////////////////////////////////////////
			#ifdef _DEBUG
			BwsSprintf(StrBuf, BwsStr("�k�h�m�d        [%d] \n"), 2);
			TRACE(StrBuf);
			for (ic = 0 ; ic < 2 ; ic++) {
				BwsSprintf(StrBuf, BwsStr("%8.2f %8.2f %8.2f \n"), m_Prim.Line.Pnt[ic].X, m_Prim.Line.Pnt[ic].Y, m_Prim.Line.Pnt[ic].Z); 
				TRACE(StrBuf);
			}
			#endif
			/////////////////////////////////////////////////////////////////////////////////////////////////////////
			*/
			break;
		case  HPRIM_CIR:																// �~:
			break;
		case  HPRIM_ARC:																// �~��
			break;
		case  HPRIM_TXT:																// ������
			break;

		case  HPRIM_POL:																// ��
			/*
			/////////////////////////////////////////////////////////////////////////////////////////////////////////
			#ifdef _DEBUG
			BwsSprintf(StrBuf, BwsStr("�R�c�e�`�b�d    [%d] \n"), m_Prim.Face.NumPnt);
			TRACE(StrBuf);
			for (ic = 0 ; ic < m_Prim.Face.NumPnt ; ic++) {
				BwsSprintf(StrBuf, BwsStr("%8.2f %8.2f %8.2f \n"), m_Prim.Face.Pnt[ic].X, m_Prim.Face.Pnt[ic].Y, m_Prim.Face.Pnt[ic].Z); 
				TRACE(StrBuf);
			}
			#endif
			/////////////////////////////////////////////////////////////////////////////////////////////////////////
			*/
/*
			HgGetVectByPln(m_Prim.Face.Pnt, &Vect);										// �@���޸��
			m_NumTri = HgDivAraToTri(m_Prim.Face.NumPnt, m_Prim.Face.Pnt, PntW1);		// �R�p�`����
			HsDirectSetTVect(m_Prim.Face.Pnt, m_NumTri * 3, PntW1, 2000.f, 500.f, PntW2);	// ÷������W�l�𓾂�
			for (ic = 0 ; ic < m_NumTri * 3 ; ic++) { 
				HsDirectSetLVertex(&PntW1[ic], &Vect, Rgb, PntW2[ic].X, PntW2[ic].Y, &Vertex[ic]);
			}

			LpD3DDEV->CreateVertexBuffer(sizeof(HDXVERTEX) * m_NumTri * 3, 0, HDX_LVERTEX, D3DPOOL_MANAGED, &m_VertBuf);
			m_VertBuf->Lock(0, 0, (BYTE **)&VMemory, 0);
			memcpy(VMemory, Vertex, sizeof(HDXVERTEX) * m_NumTri * 3);
			m_VertBuf->Unlock();
*/
			Num = m_Prim.Face.NumPnt;
			if (HgCmpPnt(&m_Prim.Face.Pnt[0], &m_Prim.Face.Pnt[m_Prim.Face.NumPnt-1], 1.0f) == 1) {	// �n�_��I�_����
				Num--;
			}
			m_NumTri = Num - 2;
			if (FAFlag == 1) {															// �ʂ̌�������
				HgGetVectByPln(m_Prim.Face.Pnt, &Vect);									// �@���޸��
				if (Vect.X < -0.5 || Vect.Y > 0.5 || Vect.Z < -0.5) {
					for (ic = 0 ; ic < m_Prim.Face.NumPnt ; ic++) {						// ���菇�t�]
						PntW1[m_Prim.Face.NumPnt-ic-1] = m_Prim.Face.Pnt[ic];
					}
				} else {
					for (ic = 0 ; ic < m_Prim.Face.NumPnt ; ic++) {
						PntW1[ic] = m_Prim.Face.Pnt[ic];
					}
				}
			} else {																	// �������Ȃ�
				for (ic = 0 ; ic < m_Prim.Face.NumPnt ; ic++) {							// ���菇�t�]
					PntW1[m_Prim.Face.NumPnt-ic-1] = m_Prim.Face.Pnt[ic];
				}
			}
			HgGetVectByPln(Num, PntW1, &Vect);											// �@���޸��
			HsDirectSetTVect(PntW1, Num, PntW1, 2000.f, 500.f, PntT);					// ÷������W�l�𓾂�
			for (ic = 0 ; ic < Num ; ic++) { 
				HsDirectSetLVertex(&PntW1[ic], &Vect, Rgb, PntT[ic].X, PntT[ic].Y, &Vertex[ic]);
			}

			LpD3DDEV->CreateVertexBuffer(sizeof(HDXVERTEX) * Num, 0, HDX_LVERTEX, D3DPOOL_MANAGED, &m_VertBuf);
			m_VertBuf->Lock(0, 0, (BYTE **)&VMemory, 0);
			memcpy(VMemory, Vertex, sizeof(HDXVERTEX) * Num);
			m_VertBuf->Unlock();

			break;

		case  HPRIM_MSH:																// ү��
			m_NumTri = Ip1 = Ip2 = 0;
			for (ic = 0 ; ic < m_Prim.Mesh.Num ; ic++) {								// �ʂ̐�
				for (jc = 0 ; jc < m_Prim.Mesh.VerNumPnt[ic] ; jc++) {					// ���_��
					Idx                                   = m_Prim.Mesh.PntNo[Ip1];
					PntW1[m_Prim.Mesh.VerNumPnt[ic]-jc-1] = m_Prim.Mesh.Pnt[Idx];
					Ip1++;
				}
				NumTri = HgDivAraToTriN(m_Prim.Mesh.VerNumPnt[ic], PntW1, PntW2);		// �R�p�`����

				for (jc = 0 ; jc < NumTri ; jc++) {
					HgGetVectByPln(&PntW2[jc*3], &Vect);								// �@���޸��
					HsDirectSetTVect(&PntW2[jc*3], 3, &PntW2[jc*3], 2000.f, 500.f, PntT);	// ÷������W�l�𓾂�
					for (kc = 0 ; kc < 3 ; kc++) {
						HsDirectSetLVertex(&PntW2[jc*3+kc], &Vect, Rgb, PntT[kc].X, PntT[kc].Y, &Vertex[Ip2]);
						Ip2++;
					}
				}
				m_NumTri +=NumTri;
			}
			if (m_NumTri > 0) {
				LpD3DDEV->CreateVertexBuffer(sizeof(HDXVERTEX) * m_NumTri * 3, 0, HDX_LVERTEX, D3DPOOL_MANAGED, &m_VertBuf);
				m_VertBuf->Lock(0, 0, (BYTE **)&VMemory, 0);
				memcpy(VMemory, Vertex, sizeof(HDXVERTEX) * m_NumTri * 3);
				m_VertBuf->Unlock();
			}
			break;
		default:
			break;
	}

	HFREE(Vertex);

	return;
}

/******************************************************************************************************************
[ MDxfPrim::SetDirectXE ]             �@�\  �G�b�W���c�����������w�ɐݒ肷��
******************************************************************************************************************/
HVOID MDxfPrim::SetDirectXE   (
								LPDIRECT3DDEVICE8	LpD3DDEV,							// (I  ) DirectX���޲�
								MINT				Symtry)								// (I  ) �Ώ��׸�  0:�Ȃ�  1:����
{
	HDXLVERTEX*			Vertex  = NULL;
	HDXLVERTEX*			VMemory = NULL;
	MgPoint3				PntW1[64];
	MgPoint2				PntW2[64];
	MgPoint2				PntW3[64];
	HCIR2				CirW1;
	HARC2				ArcW1;
	HPLN3				Pln;
	MgPoint3				Uvw[3];
	MINT				Ip1, Ip2, Idx;
	MINT				ItrSts;
	MINT				ic, jc, nc;

	/*
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	#ifdef _DEBUG
	MCHAR			StrBuf[256];
	BwsSprintf(StrBuf, BwsStr("�c�w�e �^�C�v   [%d] \n"), m_PrmType);
	TRACE(StrBuf);
	#endif
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	*/

	if ((Vertex = (HDXLVERTEX *)malloc(sizeof(HDXLVERTEX) * 2048 * 6)) == NULL)  HMFAIL;

	LpD3DDEV->SetVertexShader(HDX_LLVERTEX);											// ���_̫�ϯĐݒ�
	switch  (m_PrmType) {
		case  HPRIM_DOT:																// �_
			break;
		case  HPRIM_LIN:																// ��
			/*
			/////////////////////////////////////////////////////////////////////////////////////////////////////////
			#ifdef _DEBUG
			BwsSprintf(StrBuf, BwsStr("�k�h�m�d        [%d] \n"), 2);
			TRACE(StrBuf);
			for (ic = 0 ; ic < 2 ; ic++) {
				BwsSprintf(StrBuf, BwsStr("%8.2f %8.2f %8.2f \n"), m_Prim.Line.Pnt[ic].X, m_Prim.Line.Pnt[ic].Y, m_Prim.Line.Pnt[ic].Z); 
				TRACE(StrBuf);
			}
			#endif
			/////////////////////////////////////////////////////////////////////////////////////////////////////////
			*/
			for (ic = 0 ; ic < 2 ; ic++) {
				Vertex[ic*2  ].Pnt.x = m_Prim.Line.Pnt[ic].X;
				Vertex[ic*2  ].Pnt.y = m_Prim.Line.Pnt[ic].Y;
				Vertex[ic*2  ].Pnt.z = m_Prim.Line.Pnt[ic].Z;
				Vertex[ic*2  ].Rgb   = D3DCOLOR_RGBA(1, 1, 1, 1);
				Vertex[ic*2+1].Pnt.x = m_Prim.Line.Pnt[ic+1].X;
				Vertex[ic*2+1].Pnt.y = m_Prim.Line.Pnt[ic+1].Y;
				Vertex[ic*2+1].Pnt.z = m_Prim.Line.Pnt[ic+1].Z;
				Vertex[ic*2+1].Rgb   = D3DCOLOR_RGBA(1, 1, 1, 1);
			}
			LpD3DDEV->CreateVertexBuffer(sizeof(HDXLVERTEX) * 2 * 2, 0, HDX_LLVERTEX, D3DPOOL_MANAGED, &m_LineBuf);
			m_LineBuf->Lock(0, 0, (BYTE **)&VMemory, 0);
			memcpy(VMemory, Vertex, sizeof(HDXLVERTEX) * 2 * 2);
			m_LineBuf->Unlock();
			break;
		case  HPRIM_CIR:																// �~:
			/*
			/////////////////////////////////////////////////////////////////////////////////////////////////////////
			#ifdef _DEBUG
			BwsSprintf(StrBuf, BwsStr("�b�h�q        [%d] \n"), 2);
			TRACE(StrBuf);
			BwsSprintf(StrBuf, BwsStr("%8.2f %8.2f %8.2f \n"), m_Prim.Circle.Pnt1.X, m_Prim.Circle.Pnt1.Y, m_Prim.Circle.Pnt1.Z);
			TRACE(StrBuf);
			BwsSprintf(StrBuf, BwsStr("%8.2f %8.2f %8.2f \n"), m_Prim.Circle.Pnt2.X, m_Prim.Circle.Pnt2.Y, m_Prim.Circle.Pnt2.Z);
			TRACE(StrBuf);
			#endif
			/////////////////////////////////////////////////////////////////////////////////////////////////////////
			*/

			PntW1[0]   = m_Prim.Circle.Pnt1;											// �J�n
			PntW1[1]   = m_Prim.Circle.Pnt2;											// �I��
			PntW1[1].Z = m_Prim.Circle.Pnt1.Z;											// �I��
			PntW1[2]   = m_Prim.Circle.Pnt2;											// �I��

			ItrSts = HgGetPlnByPnt(PntW1, &Pln);										// �R�_��蕽�ʂ̕W���`�������߂�
			ItrSts = HgGetLocVect(&PntW1[0], &PntW1[1], &Pln.Vect, Uvw);				// ۰�ٍ��W�n�̒P�ʕ����޸�ق����߂�
			HgTrnWrdToLoc(&PntW1[0], Uvw, 3, PntW1, PntW2);								// ۰�ٍ��W�n�֕ϊ�

			HgTrnCirInf(&PntW2[0], &PntW2[1], &CirW1);									// �~����

			m_NumLne = HgGetHPntByCir(&CirW1, 19, PntW2);
			HgTrnLocToWrd(&PntW1[0], Uvw, m_NumLne, PntW2, PntW1);						// ܰ��ލ��W�l�ɕϊ�

			for (ic = 0 ; ic < m_NumLne-1 ; ic++) {
				Vertex[ic*2  ].Pnt.x = PntW1[ic].X;   Vertex[ic*2  ].Pnt.y = PntW1[ic  ].Y; Vertex[ic*2  ].Pnt.z = PntW1[ic  ].Z;
				Vertex[ic*2  ].Rgb   = D3DCOLOR_RGBA(1, 1, 1, 1);
				Vertex[ic*2+1].Pnt.x = PntW1[ic+1].X; Vertex[ic*2+1].Pnt.y = PntW1[ic+1].Y; Vertex[ic*2+1].Pnt.z = PntW1[ic+1].Z;
				Vertex[ic*2+1].Rgb   = D3DCOLOR_RGBA(1, 1, 1, 1);
			}

			if (m_NumLne > 1) {
				LpD3DDEV->CreateVertexBuffer(sizeof(HDXLVERTEX) * (m_NumLne-1) * 2, 0, HDX_LLVERTEX, D3DPOOL_MANAGED, &m_LineBuf);
				m_LineBuf->Lock(0, 0, (BYTE **)&VMemory, 0);
				memcpy(VMemory, Vertex, sizeof(HDXLVERTEX) * (m_NumLne-1) * 2);
				m_LineBuf->Unlock();
			}
			break;
		case  HPRIM_ARC:																// �~��
			/*
			/////////////////////////////////////////////////////////////////////////////////////////////////////////
			#ifdef _DEBUG
			BwsSprintf(StrBuf, BwsStr("�`�q�b        [%d] \n"), 2);
			TRACE(StrBuf);
			BwsSprintf(StrBuf, BwsStr("%8.2f %8.2f %8.2f \n"), m_Prim.Arc.Pnt1.X, m_Prim.Arc.Pnt1.Y, m_Prim.Arc.Pnt1.Z);
			TRACE(StrBuf);
			BwsSprintf(StrBuf, BwsStr("%8.2f %8.2f %8.2f \n"), m_Prim.Arc.Pnt2.X, m_Prim.Arc.Pnt2.Y, m_Prim.Arc.Pnt2.Z);
			TRACE(StrBuf);
			BwsSprintf(StrBuf, BwsStr("%8.2f %8.2f %8.2f \n"), m_Prim.Arc.Pnt3.X, m_Prim.Arc.Pnt3.Y, m_Prim.Arc.Pnt3.Z);
			TRACE(StrBuf);
			BwsSprintf(StrBuf, BwsStr("%8.2f %8.2f %8.2f \n"), m_Prim.Arc.Pnt4.X, m_Prim.Arc.Pnt4.Y, m_Prim.Arc.Pnt4.Z);
			TRACE(StrBuf);
			#endif
			/////////////////////////////////////////////////////////////////////////////////////////////////////////
			*/
			PntW1[0]   = m_Prim.Arc.Pnt1;												// �I��
			PntW1[1]   = m_Prim.Arc.Pnt2;												// �I��
			PntW1[1].Z = m_Prim.Arc.Pnt1.Z;												// �I��
			PntW1[2]   = m_Prim.Arc.Pnt2;												// �I��

			ItrSts = HgGetPlnByPnt(PntW1, &Pln);										// �R�_��蕽�ʂ̕W���`�������߂�
			ItrSts = HgGetLocVect(&PntW1[0], &PntW1[1], &Pln.Vect, Uvw);				// ۰�ٍ��W�n�̒P�ʕ����޸�ق����߂�
			HgTrnWrdToLoc(&PntW1[0], Uvw, 4, (MgPoint3 *)&m_Prim.Arc, PntW2);			// ۰�ٍ��W�n�֕ϊ�

			PntW3[0].X = HMIN(PntW2[0].X, PntW2[1].X);
			PntW3[0].Y = HMIN(PntW2[0].Y, PntW2[1].Y);
			PntW3[1].X = HMAX(PntW2[0].X, PntW2[1].X);
			PntW3[1].Y = HMAX(PntW2[0].Y, PntW2[1].Y);
//			if (Symtry == 0) {															// �Ώ̂Ȃ�
				PntW3[2] = PntW2[2];
				PntW3[3] = PntW2[3];
//			} else {
//				PntW3[2] = PntW2[3];
//				PntW3[3] = PntW2[2];
//			}
			HgTrnArcInf(&PntW3[0], &PntW3[1], &PntW3[2], &PntW3[3], &ArcW1);			// �~�ʍ\����

			m_NumLne = HgGetHPntByArc(&ArcW1, 0, PntW2);
			HgTrnLocToWrd(&PntW1[0], Uvw, m_NumLne, PntW2, PntW1);						// ܰ��ލ��W�l�ɕϊ�

			for (ic = 0 ; ic < m_NumLne-1 ; ic++) {
				Vertex[ic*2  ].Pnt.x = PntW1[ic].X;   Vertex[ic*2  ].Pnt.y = PntW1[ic  ].Y; Vertex[ic*2  ].Pnt.z = PntW1[ic  ].Z;
				Vertex[ic*2  ].Rgb   = D3DCOLOR_RGBA(1, 1, 1, 1);
				Vertex[ic*2+1].Pnt.x = PntW1[ic+1].X; Vertex[ic*2+1].Pnt.y = PntW1[ic+1].Y; Vertex[ic*2+1].Pnt.z = PntW1[ic+1].Z;
				Vertex[ic*2+1].Rgb   = D3DCOLOR_RGBA(1, 1, 1, 1);
			}
			if (m_NumLne > 1) {
				LpD3DDEV->CreateVertexBuffer(sizeof(HDXLVERTEX) * (m_NumLne-1) * 2, 0, HDX_LLVERTEX, D3DPOOL_MANAGED, &m_LineBuf);
				m_LineBuf->Lock(0, 0, (BYTE **)&VMemory, 0);
				memcpy(VMemory, Vertex, sizeof(HDXLVERTEX) * (m_NumLne-1) * 2);
				m_LineBuf->Unlock();
			}
			break;
		case  HPRIM_TXT:																// ������
			break;

		case  HPRIM_POL:																// ��
			/*
			/////////////////////////////////////////////////////////////////////////////////////////////////////////
			#ifdef _DEBUG
			BwsSprintf(StrBuf, BwsStr("�R�c�e�`�b�d    [%d] \n"), m_Prim.Face.NumPnt);
			TRACE(StrBuf);
			for (ic = 0 ; ic < m_Prim.Face.NumPnt ; ic++) {
				BwsSprintf(StrBuf, BwsStr("%8.2f %8.2f %8.2f \n"), m_Prim.Face.Pnt[ic].X, m_Prim.Face.Pnt[ic].Y, m_Prim.Face.Pnt[ic].Z); 
				TRACE(StrBuf);
			}
			#endif
			/////////////////////////////////////////////////////////////////////////////////////////////////////////
			*/
			m_NumLne = m_Prim.Face.NumPnt;
			if (HgCmpPnt(&m_Prim.Face.Pnt[0], &m_Prim.Face.Pnt[m_Prim.Face.NumPnt-1], 1.0f) == 1) {	// �n�_��I�_����
				m_NumLne--;
			} else {
				if (m_Prim.Face.CFlag == 0)  m_NumLne--;
			}
			for (ic = 0 ; ic < m_Prim.Face.NumPnt ; ic++) {								// ���菇�t�]
				PntW1[m_Prim.Face.NumPnt-ic-1] = m_Prim.Face.Pnt[ic];
			}
			for (ic = 0 ; ic < m_NumLne ; ic++) {
				nc = HPTR(ic+1, m_NumLne);
				Vertex[ic*2  ].Pnt.x = PntW1[ic].X; Vertex[ic*2  ].Pnt.y = PntW1[ic].Y; Vertex[ic*2  ].Pnt.z = PntW1[ic].Z;
				Vertex[ic*2  ].Rgb   = D3DCOLOR_RGBA(1, 1, 1, 1);
				Vertex[ic*2+1].Pnt.x = PntW1[nc].X; Vertex[ic*2+1].Pnt.y = PntW1[nc].Y; Vertex[ic*2+1].Pnt.z = PntW1[nc].Z;
				Vertex[ic*2+1].Rgb   = D3DCOLOR_RGBA(1, 1, 1, 1);
			}

			LpD3DDEV->CreateVertexBuffer(sizeof(HDXLVERTEX) * m_NumLne * 2, 0, HDX_LLVERTEX, D3DPOOL_MANAGED, &m_LineBuf);
			m_LineBuf->Lock(0, 0, (BYTE **)&VMemory, 0);
			memcpy(VMemory, Vertex, sizeof(HDXLVERTEX) * m_NumLne * 2);
			m_LineBuf->Unlock();
			break;

		case  HPRIM_MSH:																// ү��
			m_NumLne = Ip1 = Ip2 = 0;
			for (ic = 0 ; ic < m_Prim.Mesh.Num ; ic++) {								// �ʂ̐�
				for (jc = 0 ; jc < m_Prim.Mesh.VerNumPnt[ic] ; jc++) {					// ���_��
					Idx                                   = m_Prim.Mesh.PntNo[Ip1];
					PntW1[m_Prim.Mesh.VerNumPnt[ic]-jc-1] = m_Prim.Mesh.Pnt[Idx];
					Ip1++;
				}
				for (jc = 0 ; jc < m_Prim.Mesh.VerNumPnt[ic] ; jc++) {					// ���_��
					nc = HPTR(jc+1, m_Prim.Mesh.VerNumPnt[ic]);
					Vertex[Ip2].Pnt.x = PntW1[jc].X; Vertex[Ip2].Pnt.y = PntW1[jc].Y; Vertex[Ip2].Pnt.z = PntW1[jc].Z;
					Vertex[Ip2].Rgb   = D3DCOLOR_RGBA(1, 1, 1, 1);
					Ip2++;
					Vertex[Ip2].Pnt.x = PntW1[nc].X; Vertex[Ip2].Pnt.y = PntW1[nc].Y; Vertex[Ip2].Pnt.z = PntW1[nc].Z;
					Vertex[Ip2].Rgb   = D3DCOLOR_RGBA(1, 1, 1, 1);
					Ip2++;
					m_NumLne++;
				}
			}
			if (m_NumLne > 0) {
				LpD3DDEV->CreateVertexBuffer(sizeof(HDXLVERTEX) * m_NumLne * 2, 0, HDX_LLVERTEX, D3DPOOL_MANAGED, &m_LineBuf);
				m_LineBuf->Lock(0, 0, (BYTE **)&VMemory, 0);
				memcpy(VMemory, Vertex, sizeof(HDXLVERTEX) * m_NumLne * 2);
				m_LineBuf->Unlock();
			}
			break;
		default:
			break;
	}

	HFREE(Vertex);																		// SHIFT,CTRL

	return;
}

/******************************************************************************************************************
[ MDxfPrim::ShowDirectXF ]            �@�\  �ʂ��c�����������w�ŕ\������
******************************************************************************************************************/
HVOID MDxfPrim::ShowDirectXF  (
								LPDIRECT3DDEVICE8		LpD3DDEV)						// (I  ) DirectX���޲�
{
	LpD3DDEV->SetVertexShader(HDX_LVERTEX);												// ���_̫�ϯĐݒ�

	switch  (m_PrmType) {
		case  HPRIM_DOT:																// �_
			break;
		case  HPRIM_LIN:																// ��
			break;
		case  HPRIM_CIR:																// �~:
			break;
		case  HPRIM_ARC:																// �~��
			break;
		case  HPRIM_TXT:																// ������
			break;
		case  HPRIM_POL:																// ��
		case  HPRIM_MSH:																// ү��
			if (m_NumTri > 0) {
				LpD3DDEV->SetStreamSource(0, m_VertBuf, sizeof(HDXVERTEX));
				LpD3DDEV->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, m_NumTri);				// �R�p�`�̐�
				LpD3DDEV->SetStreamSource(0, NULL, 0);
			}
			break;
		default:
			break;
	}

	return;
}

/******************************************************************************************************************
[ MDxfPrim::ShowDirectXE ]            �@�\  �G�b�W���c�����������w�ŕ\������
******************************************************************************************************************/
HVOID MDxfPrim::ShowDirectXE  (
								LPDIRECT3DDEVICE8		LpD3DDEV)						// (I  ) DirectX���޲�
{
	LpD3DDEV->SetVertexShader(HDX_LLVERTEX);											// ���_̫�ϯĐݒ�

	switch  (m_PrmType) {
		case  HPRIM_DOT:																// �_
			break;
		case  HPRIM_LIN:																// ��
			LpD3DDEV->SetStreamSource(0, m_LineBuf, sizeof(HDXLVERTEX));
			LpD3DDEV->DrawPrimitive(D3DPT_LINELIST, 0, 1);								// �����̐�
			LpD3DDEV->SetStreamSource(0, NULL, 0);
			break;
		case  HPRIM_CIR:																// �~:
		case  HPRIM_ARC:																// �~��
			if (m_NumLne > 0) {
				LpD3DDEV->SetStreamSource(0, m_LineBuf, sizeof(HDXLVERTEX));
				LpD3DDEV->DrawPrimitive(D3DPT_LINELIST, 0, m_NumLne);					// �����̐�
				LpD3DDEV->SetStreamSource(0, NULL, 0);
			}
			break;
		case  HPRIM_TXT:																// ������
			break;
		case  HPRIM_POL:																// ��
		case  HPRIM_MSH:																// ү��
			if (m_NumLne > 0) {
				LpD3DDEV->SetStreamSource(0, m_LineBuf, sizeof(HDXLVERTEX));
				LpD3DDEV->DrawPrimitive(D3DPT_LINELIST, 0, m_NumLne);					// �����̐�
				LpD3DDEV->SetStreamSource(0, NULL, 0);
			}
			break;
		default:
			break;
	}

	return;
}

/******************************************************************************************************************
[ MDxfPrim::RelDirectXF ]              �@�\  �ʂ��c�����������w���J������
******************************************************************************************************************/
HVOID MDxfPrim::RelDirectXF   ()
{
	RELEASE(m_VertBuf);																	// DirectX ���_�ޯ̧
	m_VertBuf = NULL;
	m_NumTri  = 0;																		// �R�p�`�̐�

	return;
}

/******************************************************************************************************************
[ MDxfPrim::RelDirectXE ]             �@�\  �G�b�W���c�����������w���J������
******************************************************************************************************************/
HVOID MDxfPrim::RelDirectXE   ()
{
	RELEASE(m_LineBuf);																	// DirectX �����ޯ̧
	m_LineBuf = NULL;																	// DirectX �����ޯ̧
	m_NumLne  = 0;																		// �����̐�

	return;
}

/******************************************************************************************************************
[�@�\] �ŏ���ő��ݒ�
[�Ԓl] �Ȃ�
******************************************************************************************************************/
static HVOID SetMinMax         (
								MgPoint3*			Pnt,								// (I  ) ���W�l
								MgMinMax3*			MinMax)								// (I/O) �ŏ���ő�
{
	if (Pnt->X < MinMax->Min.X)  MinMax->Min.X = Pnt->X;
	if (Pnt->Y < MinMax->Min.Y)  MinMax->Min.Y = Pnt->Y;
	if (Pnt->Z < MinMax->Min.Z)  MinMax->Min.Z = Pnt->Z;
	if (Pnt->X > MinMax->Max.X)  MinMax->Max.X = Pnt->X;
	if (Pnt->Y > MinMax->Max.Y)  MinMax->Max.Y = Pnt->Y;
	if (Pnt->Z > MinMax->Max.Z)  MinMax->Max.Z = Pnt->Z;

	return;
}
static HVOID SetMinMax         (
								MgPoint2*			Pnt,								// (I  ) ���W�l
								MgMinMax3*			MinMax)								// (I/O) �ŏ���ő�
{
	if (Pnt->X < MinMax->Min.X)  MinMax->Min.X = Pnt->X;
	if (Pnt->Y < MinMax->Min.Y)  MinMax->Min.Y = Pnt->Y;
	if (Pnt->X > MinMax->Max.X)  MinMax->Max.X = Pnt->X;
	if (Pnt->Y > MinMax->Max.Y)  MinMax->Max.Y = Pnt->Y;

	return;
}

/******************************************************************************************************************
[�@�\] �u�����������֏���ݒ�
[�Ԓl] �X�e�[�^�X      �O�F����  �P�F�G���[
******************************************************************************************************************/
static MINT SetVertex          (														// (  O) �ð��  0:���� 1:�װ
								MINT				NumPnt,								// (I  ) ���_��
								MgPoint3*			Pnt,								// (I  ) ���W�l
								D3DCOLOR			Rgb,								// (I  ) �F
								MINT*				NumTri,								// (I/O) �R�p�`��
								HDXVERTEX*			Vertex,								// (I/O) DirectX
								MINT*				IPtr)								// (I/O) �߲��
{
	MINT				Status;
	MgPoint3				Vect;															// �@������
	MgPoint3				PntT[64];
	MgPoint2				PntX[64];
	MINT				IWork;
	MINT				ic;


	Status = 1;

	HgGetVectByPln(Pnt, &Vect);															// �@���޸��
	IWork = HgDivAraToTri(NumPnt, Pnt, PntT);											// �R�p�`����
	HsDirectSetTVect(Pnt, IWork * 3, PntT, 2000.f, 500.f, PntX);						// ÷������W�l�𓾂�
	for (ic = 0 ; ic < IWork * 3 ; ic++) { 
		HsDirectSetLVertex(&PntT[ic], &Vect, Rgb, PntX[ic].X, PntX[ic].Y, &Vertex[*IPtr]);
		(*IPtr)++;
	}
	(*NumTri) += IWork;
	Status     = 0;

	return(Status);
}

/******************************************************************************************************************
[�@�\] ���[�J�����W�n�֕ϊ�
[�Ԓl] �Ȃ�
******************************************************************************************************************/
static HVOID TrnWrdToLoc       (
								MgPoint3*			TOPnt,								// (I  ) 2������3�����ϊ��p���_
								MgPoint3*			TUvw,								// (I  ) 2������3�����ϊ��pUVW
								MgPoint3*			Pnt1,								// (I  ) 3����
								MgPoint2*			Pnt2)								// (  O) 2����
{

	if (TOPnt && TUvw) {
		HgTrnWrdToLoc(TOPnt, TUvw, 1, Pnt1, Pnt2);										// ۰�ٍ��W�n�֕ϊ�
	} else {
		memcpy(Pnt2, Pnt1, sizeof(MgPoint2));
	}

	return;
}

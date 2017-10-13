/*********************************************************************************************************************************
              �c�w�e  �w�b�_�t�@�C�� (HsDxf.h)

********************************************************************************************************************************/

#ifndef			_HSDXF_
	#define		_HSDXF_

#include "MCad.h"
//#include		"HBase.h"
//#include		"HXBase.h"#define			 NAMMAX				64													// ���O�ő咷
//#include		"HnDisp.h"
//#include		"HlDList.h"
//#include		"HstackId.h"

typedef union _hxprim {									// �}�`
	HPOINT3			Dot;								// �_
	HLINE3			Line;								// ��
	HCIR3S			Circle;								// �~
	HARC3S			Arc;								// �~��
	HXTEXT			Text;								// ������
	HFACE3			Face;								// ��
	HXMESH3			Mesh;								// ү��
} HXPRIM;


/*************************************************************************************************************
[ HsDxfPrim ]                          �}�`���
***************************************************************************************************************/

class   HsDxfPrim
{
public:

	MINT						m_PrmType;												// ����è������
	MINT						m_Color;												// �F�ԍ�
	MINT						m_LineNo;												// ����ԍ��i�ԍ��j
	MINT						m_LineWd;												// �����ԍ��i�ԍ��j
	HXPRIM						m_Prim;													// �}�`
	MINT						m_NumTri;												// �R�p�`�̐�
	LPDIRECT3DVERTEXBUFFER8		m_VertBuf;												// DirectX ���_�ޯ̧
	MINT						m_NumLne;												// �����̐�
	LPDIRECT3DVERTEXBUFFER8		m_LineBuf;												// �R�����̐������\�������

	// �R���X�g���N�^
	HsDxfPrim                  (
								MINT				PrmType,							// (I  ) ����è������
								MINT				Color,								// (I  ) �F�ԍ�
								MINT				LineNo,								// (I  ) ����ԍ��i�ԍ��j
								MINT				LineWd,								// (I  ) �����ԍ��i�ԍ��j
								HXPRIM*				Prim);								// (I  ) �}�`

	// �f�X�g���N�^
	~HsDxfPrim                 ();

	// �ό`
	HVOID Transform            (
								MINT				Color,								// (I  ) �F�ԍ�
								HPOINT3*			Base,								// (I  ) ��_
								HPOINT3*			Insert,								// (I  ) �}���_
								HPOINT3*			Scale,								// (I  ) �ړx
								HREAL				Rotate);							// (I  ) ��]
	HVOID Transform            (
								MINT				TFlag,								// (I  ) 0:�ϊ����� 1:�ϊ�
								MINT				Symtry,								// (I  ) �Ώ��׸�  0:�Ȃ�  1:����
								HMTRX3				Matrix,								// (I  ) �ϊ���ظ�
								HPOINT3*			Move,								// (I  ) �ړ���
								HREAL*				Scale,								// (I  ) ����
								HMINMAX3*			MinMax);							// (  O) �ŏ���ő�

	// �ό`�i�x���y�j
	HVOID TransformYZ          (
								HMINMAX3*			MinMax);							// (  O) �ŏ���ő�

	// �}�`����\������
	HVOID Show                 (
								CDC*				MemDC,								// (I  ) ���DC
								MINT				MainId,								// (I  ) Ҳ�ID
								MINT				SubsId,								// (I  ) ���ID
								COLORREF			Color,								// (I  ) �\���F
								MINT				Symtry,								// (I  ) �Ώ��׸�  0:�Ȃ�  1:����
								HMTRX3				Matrix,								// (I  ) �ϊ���ظ�
								HPOINT3*			Move,								// (I  ) �ړ���
								HREAL				Scale,								// (I  ) ����
								HPOINT2*			CutPnt = NULL,						// (I  ) ��Đ� + �����_
								HMINMAX3*			MinMax = NULL);						// (  O) �ŏ���ő�

	// ���ʏ��̍쐬�i���j
	HVOID MakeRL               (
								MINT				VewCod,								// (I  ) �ޭ����� 0:�� 1:�� 2:�� 3:�k
								MINT				Kai,								// (I  ) �K
								HnDisp*				DispRL,								// (I  ) ���ʕ\�����i�����j
								COLORREF			Rgb,								// (I  ) �\���F
								MINT				Symtry,								// (I  ) �Ώ��׸�  0:�Ȃ�  1:����
								HMTRX3				Matrix,								// (I  ) �ϊ���ظ�
								HPOINT3*			Move,								// (I  ) �ړ���
								HREAL				Scale,								// (I  ) ����
								HPOINT3*			TOPnt  = NULL,						// (I  ) 2������3�����ϊ��p���_
								HPOINT3*			TUvw   = NULL,						// (I  ) 2������3�����ϊ��pUVW
								HMINMAX3*			MinMax = NULL);						// (  O) �ŏ���ő�

	// �A������
	MINT HiddenLine            (														// (  O) ������
								MINT				VewCod,								// (I  ) �ޭ����� 0:�� 1:�� 2:�� 3:�k
								MINT				Kai,								// (I  ) �K
								HPOINT3*			TOPnt,								// (I  ) 2������3�����ϊ��p���_
								HPOINT3*			TUvw,								// (I  ) 2������3�����ϊ��pUVW
								MINT				NumPnt1,							// (I  ) ���_��
								HPOINT3*			Pnt1,								// (I  ) ���_
								HLINE2*				Line1);								// (I  ) ����

	// �A���`�F�b�N�i�ŏ��E�ő�j
	MINT HiddenCheck           (														// (  O) 0:�����Ȃ� 1:������
 								MINT				VewCod,								// (I  ) �ޭ����� 0:�� 1:�� 2:�� 3:�k
								MINT				Kai,								// (I  ) �K
								HPOINT3*			TOPnt,								// (I  ) 2������3�����ϊ��p���_
								HPOINT3*			TUvw,								// (I  ) 2������3�����ϊ��pUVW
								MINT				NumPnt1,							// (I  ) ���_��
								HPOINT3*			Pnt1);								// (I  ) ���_

	// �v���~�e�B�u���𓾂�
	MINT NumDxfPrim            (														// (  O) ����è�ސ�
								MINT				Symtry,								// (I  ) �Ώ��׸�  0:�Ȃ�  1:����
								HMTRX3				Matrix,								// (I  ) �ϊ���ظ�
								HPOINT3*			Move,								// (I  ) �ړ���
								HREAL				Scale,								// (I  ) ����
								HPOINT2*			CutPnt);							// (I  ) ��Đ� + �����_

	// �c�w�e�i�q�P�S�j�`���o��
	MINT DxfR14Out             (														// (  O) �ð��  0:���� �ȊO:�װ
								CStdioFile*			FLPtr,								// (I  ) ̧�َ��ʎq
								MINT*				NumHnd,								// (I/O) ����ِ�
								HCHAR*				LayNam,								// (I  ) ڲԖ�
								MINT				Symtry,								// (I  ) �Ώ��׸�  0:�Ȃ�  1:����
								HMTRX3				Matrix,								// (I  ) �ϊ���ظ�
								HPOINT3*			Move,								// (I  ) �ړ���
								HREAL				Scale,								// (I  ) ����
								HPOINT2*			CutPnt = NULL);

	// �ʂ��c�����������w�ɐݒ肷��
	HVOID SetDirectXF          (
								LPDIRECT3DDEVICE8	LpD3DDEV,							// (I  ) DirectX���޲�
								COLORREF			Rgb1,								// (I  ) RGB�װ�l
								MINT				FAFlag);							// (I  ) �ʂ̌�������   0:���Ȃ�  1:����
	// �G�b�W���c�����������w�ɐݒ肷��
	HVOID SetDirectXE          (
								LPDIRECT3DDEVICE8	LpD3DDEV,							// (I  ) DirectX���޲�
								MINT				Symtry);							// (I  ) �Ώ��׸�  0:�Ȃ�  1:����

	// �ʂ��c�����������w�ŕ\������
	HVOID ShowDirectXF         (														// (  O) �ð��
								LPDIRECT3DDEVICE8	LpD3DDEV);							// (I  ) DirectX���޲�
	// �G�b�W���c�����������w�ŕ\������
	HVOID ShowDirectXE         (														// (  O) �ð��
								LPDIRECT3DDEVICE8	LpD3DDEV);							// (I  ) DirectX���޲�

	// �ʂ��c�����������w���J������
	HVOID RelDirectXF          ();
	// �G�b�W���c�����������w���J������
	HVOID RelDirectXE          ();


private:

};


/*************************************************************************************************************
[ HsDxfBlock ]                         �u���b�N���
***************************************************************************************************************/

class   HsDxfBlock
{
public:

	HCHAR						m_Name[NAMMAX];											// ��ۯ���
	HCHAR						m_LName[NAMMAX];										// ڲԖ�
	MINT						m_Color;												// �F�ԍ�
	COLORREF					m_Rgb;													// RGB�װ�l
	LPDIRECT3DTEXTURE8			m_Texture;												// ÷���
	MINT						m_LineNo;												// ���ԍ�
	HPOINT3						m_InsPnt;												// �}����_
	HlDList*					m_PrimList;												// ����è�ޥؽĂ��߲��

	// �R���X�g���N�^
	HsDxfBlock                 ();
	HsDxfBlock                 (
								HCHAR*				Name,								// (I  ) ڲԖ�
								HCHAR*				LName,								// (I  ) ڲԖ�
								MINT				Color,								// (I  ) �F�ԍ�
								MINT				LineNo,								// (I  ) ���ԍ�
								HPOINT3*			InsPnt);							// (I  ) �}����_

	// �f�X�g���N�^
	~HsDxfBlock                ();

	// �u���b�N�����Z�b�g
	HVOID SetInf               (
								HCHAR*				Name,								// (I  ) ��ۯ���
								HCHAR*				LName,								// (I  ) ڲԖ�
								MINT				Color,								// (I  ) �F�ԍ�
								MINT				LineNo,								// (I  ) ���ԍ�
								HPOINT3*			InsPnt);							// (I  ) �}����_

	// �}�`����ǉ�
	HsDxfPrim* AddPrim         (														// (  O) �}�`���̲ݽ�ݽ
								MINT				PrmType,							// (I  ) ����è������
								MINT				Color,								// (I  ) �F�ԍ�
								MINT				LineNo,								// (I  ) ����ԍ��i�ԍ��j
								MINT				LineWd,								// (I  ) �����ԍ��i�ԍ��j
								HXPRIM*				Prim);								// (I  ) �}�`


private:

};


/*************************************************************************************************************
[ HsDxfLayer ]                         ���C�����
***************************************************************************************************************/

class   HsDxfLayer
{
public:

	HCHAR						m_Name[NAMMAX];											// ڲԖ�
	MINT						m_Color;												// �F�ԍ�
	COLORREF					m_Rgb;													// RGB�װ�l
	LPDIRECT3DTEXTURE8			m_Texture;												// ÷���
	MINT						m_LineNo;												// ���ԍ�
	HlDList*					m_PrimList;												// ����è�ޥؽĂ��߲��

	// �R���X�g���N�^
	HsDxfLayer                 (
								HCHAR*				Name,								// (I  ) ڲԖ�
								MINT				Color,								// (I  ) �F�ԍ�
								MINT				LineNo);							// (I  ) ���ԍ�

	// �f�X�g���N�^
	~HsDxfLayer                ();

	// �X�V
	Modify                     (
								MINT				Color,								// (I  ) �F�ԍ�
								MINT				LineNo);							// (I  ) ���ԍ�

	// �}�`����ǉ�
	HsDxfPrim* AddPrim         (														// (  O) �}�`���̲ݽ�ݽ
								MINT				PrmType,							// (I  ) ����è������
								MINT				Color,								// (I  ) �F�ԍ�
								MINT				LineNo,								// (I  ) ����ԍ��i�ԍ��j
								MINT				LineWd,								// (I  ) �����ԍ��i�ԍ��j
								HXPRIM*				Prim);								// (I  ) �}�`


private:

};

/*************************************************************************************************************
[ HsDxfLines ]                         �����
***************************************************************************************************************/

class   HsDxfLines
{
public:

	HCHAR						m_Name[NAMMAX];											// ����
	MINT						m_LineNo;												// ���ԍ�

	// �R���X�g���N�^
	HsDxfLines                 (
								HCHAR*				Name,								// (I  ) ����
								MINT				LIneNo);							// (I  ) ���ԍ�

	// �f�X�g���N�^
	~HsDxfLines                ();


private :

};


/*************************************************************************************************************
[ HsDxfFonts ]                         �t�H���g���
***************************************************************************************************************/

class   HsDxfFonts
{
public:

	HCHAR						m_Name[NAMMAX];											// ̫�Ė�
	MINT						m_FCode;												// ̫�ĺ���

	// �R���X�g���N�^
	HsDxfFonts                 (
								HCHAR*				Name,								// (I  ) ̫�Ė�
								MINT				_FCode);							// (I  ) ̫�ĺ���

	// �f�X�g���N�^
	~HsDxfFonts                ();


private:

};


/*************************************************************************************************************
[ HsDxfInf ]                           �c�w�e���
***************************************************************************************************************/

class   HsDxfInf
{
public:

	MINT						m_VewTyp;												// �ޭ����
	HREAL						m_ProtRate;												// ��ۯ�ڰ�
	MINT						m_AngDir;												// �~�ʕ���       1:���v���� -1:�����v����
	MINT						m_Symtry;												// �Ώ��׸�       0:�Ȃ�      1:����
	MINT						m_SymtryC;												// ���݂̑Ώ��׸� 0:�Ȃ�      1:����
	HMTRX3						m_Matrix;												// �ϊ���ظ�
	HPOINT3						m_Move;													// �ړ���
	HREAL						m_Scale[3];												// �g�嗦
	HREAL						m_TextRate;												// �����ϊ��{��
	MINT						m_ZmnSize;												// �}�ʻ���
	HMINMAX3					m_Window;												// ����޳����
	HMINMAX3					m_ViewPort;												// �ޭ��߰�
	MINT						m_FAFlag;												// �ʂ̌�������   0:���Ȃ�    1:����
	HMINMAX3					m_MinMax;												// �ŏ���ő�
	LPDIRECT3DTEXTURE8			m_TextureY;												// ÷����i���j
	LPDIRECT3DVERTEXBUFFER8		m_VertBufY;												// DirectX ���_�ޯ̧�i���j
	MINT						m_NumTriY;												// �R�p�`�̐�       �i���j
	MINT						m_TextFlg;												// ÷����׸�      0:�Ǘ�      1:����
	HlDList*					m_BlockList;											// ��ۯ��ؽĂ��߲��
	HlDList*					m_LayerList;											// ڲԥؽĂ��߲��
	HlDList*					m_LinesList;											// ����ؽĂ��߲��
	HlDList*					m_FontsList;											// ̫�ĥؽĂ��߲��

	// �R���X�g���N�^
	HsDxfInf                   (
								MINT				VewTyp,								// (I  ) �ޭ����
								HREAL				ProtRate,							// (I  ) ��ۯ�ڰ�
								MINT				AngDir,								// (I  ) �~�ʕ��� 1:���v -1:�����v
								HREAL				TextRate,							// (I  ) �����ϊ��{��
								MINT				ZmnSize,							// (I  ) �}�ʻ���
								HMINMAX3*			Window,								// (I  ) ����޳����
								HMINMAX3*			ViewPort,							// (I  ) �ޭ��߰�
								MINT				FAFlag);							// (I  ) �ʂ̌�������   0:���Ȃ�  1:����

	// �f�X�g���N�^
	~HsDxfInf                  ();

	// �u���b�N����ǉ�
	HsDxfBlock* AddBlock       ();														// (  O) ��ۯ����ݽ�ݽ

	// ���C������ǉ�
	MINT AddLayer              (														// (  O) �ð��  0:�ǉ�  1:�X�V
								HCHAR*				Name,								// (I  ) ڲԖ�
								MINT				Color,								// (I  ) �F�ԍ�
								MINT				LineNo);							// (I  ) ���ԍ�

	// ������ǉ�
	MINT AddLines              (														// (  O) �ð��
								HCHAR*				Name,								// (I  ) ����
								MINT				LineNo);							// (I  ) ���ԍ�

	// �t�H���g����ǉ�
	MINT AddFonts              (														// (  O) �ð��
								HCHAR*				Name,								// (I  ) ̫�Ė�
								MINT				FCode);								// (I  ) ̫�ĺ���

	// �u���b�N��������
	HsDxfBlock* GetBlock       (														// (  O) ��ۯ����  NULL:�����炸
								HCHAR*				Name,								// (I  ) ��ۯ���
								HCHAR*				LName  = NULL,						// (  O) ڲԖ�
								MINT*				Color  = NULL,						// (  O) �F�ԍ�
								MINT*				LineNo = NULL,						// (  O) ���ԍ�
								HPOINT3*			InsPnt = NULL);						// (  O) �}����_

	// ���C����������
	HsDxfLayer* GetLayer       (														// (  O) ڲԏ��  NULL:�����炸
								HCHAR*				Name,								// (I  ) ڲԖ�
								MINT*				Color  = NULL,						// (  O) �F�ԍ�
								MINT*				LineNo = NULL);						// (  O) ���ԍ�

	// ����������
	HsDxfLines* GetLines       (														// (  O) �ð��  0:����  1:�Ȃ�
								HCHAR*				Name,								// (I  ) ����
								MINT*				LineNo = NULL);						// (  O) ���ԍ�

	// �t�H���g��������
	HsDxfFonts* GetFonts       (														// (  O) �ð��  0:����  1:�Ȃ�
								HCHAR*				Name,								// (I  ) ̫�Ė�
								MINT*				FCode = NULL);						// (  O) ̫�Ĕԍ�

	// �u���b�N�����N���A
	HVOID ClrBlock             ();

	// ���C�������N���A
	HVOID ClrLayer             ();

	// �������N���A
	HVOID ClrLines             ();

	// �t�H���g�����N���A
	HVOID ClrFonts             ();

	// �ό`
	MINT Transform             (														// (  O) �ð��     0:����  1:�װ
								MINT				Symtry = 0,							// (I  ) �Ώ��׸�  0:�Ȃ�  1:����
								HMTRX3				Matrix = NULL,						// (I  ) �ϊ���ظ�
								HPOINT3*			Move   = NULL,						// (I  ) �ړ���
								HREAL*				Scale  = NULL);						// (I  ) ����

	// �ό`�i�x���y�j
	MINT TransformYZ           ();														// (  O) �ð��  0:����  1:�װ

	// �ŏ��ő�𓾂�
	HVOID GetMinMax            (														// (  O) �ð��  0:����  1:�װ
								HMINMAX3*			MinMax);							// (  O) �ŏ���ő�

	// ��]�E�ړ���̍ŏ��ő�𓾂�
	HVOID GetMinMax            (
								MINT				Symtry,								// (I  ) �Ώ��׸�  0:�Ȃ�  1:����
								HMTRX3				Matrix,								// (I  ) �ϊ���ظ�  NULL:�ϊ��Ȃ�
								HPOINT3*			Move,								// (I  ) �ړ���     NULL:�ړ��Ȃ�
								HREAL*				Scale,								// (I  ) ����       NULL:���قȂ�
								HMINMAX3*			MinMax);							// (  O) �ŏ���ő�

	// �ŏ��ő���Ȃ�ʂ𓾂�
	HVOID GetMinMaxF           (														// (  O) �ð��  0:����  1:�װ
								MINT				VewTyp,								// (I  ) �����w���E��
								HMTRX3				Matrix,								// (I  ) �ϊ���ظ�
								HPOINT3*			Move,								// (I  ) �ړ���
								HPOINT3*			Pnt1);								// (  O) �ŏ���ő�̗̈�(4�_)

	// �c�w�e����\������
	MINT Show                  (														// (  O) �ð��
								MINT				MainId,								// (I  ) Ҳ�ID
								MINT				SubsId,								// (I  ) ���ID
								COLORREF			Color,								// (I  ) �\���F
								MINT				Symtry = 0,							// (I  ) �Ώ��׸�      0:�Ȃ�  1:����
								HMTRX3				Matrix = NULL,						// (I  ) �ϊ���ظ�
								HPOINT3*			Move   = NULL,						// (I  ) �ړ���
								HREAL				Scale  = 1.0f,						// (I  ) ����
								HPOINT2*			CutPnt = NULL,						// (I  ) ��Đ� + �����_
								HMINMAX3*			MinMax = NULL);						// (  O) �ŏ���ő�

	// ���ʏ��̍쐬�i���j
	MINT MakeRL                (														// (  O) �ð��  0:����  1:�װ
								MINT				VewCod,								// (I  ) �ޭ����� 0:�� 1:�� 2:�� 3:�k
								MINT				Kai,								// (I  ) �K
								HnDisp*				DispRL,								// (I  ) ���ʕ\�����i�����j
								COLORREF			Rgb,								// (I  ) �\���F
								MINT				Symtry = 0,							// (I  ) �Ώ��׸�  0:�Ȃ�  1:����
								HMTRX3				Matrix = NULL,						// (I  ) �ϊ���ظ�
								HPOINT3*			Move   = NULL,						// (I  ) �ړ���
								HREAL				Scale  = 1.0,						// (I  ) ����
								HPOINT3*			TOPnt  = NULL,						// (I  ) 2������3�����ϊ��p���_
								HPOINT3*			TUvw   = NULL,						// (I  ) 2������3�����ϊ��pUVW
								HMINMAX3*			MinMax = NULL);						// (  O) �ŏ���ő�

	// �v���~�e�B�u���𓾂�
	MINT NumDxfPrim            (														// (  O) ����è�ސ�
								MINT				Symtry = 0,							// (I  ) �Ώ��׸�  0:�Ȃ�  1:����
								HMTRX3				Matrix = NULL,						// (I  ) �ϊ���ظ�
								HPOINT3*			Move   = NULL,						// (I  ) �ړ���
								HREAL				Scale  = 1.0f,						// (I  ) ����
								HPOINT2*			CutPnt = NULL);						// (I  ) ��Đ� + �����_

	// �c�w�e�i�q�P�S�j�`���o��
	MINT DxfR14Out             (														// (  O) �ð��  0:���� �ȊO:�װ
								CStdioFile*			FLPtr,								// (I  ) ̧�َ��ʎq
								MINT*				NumHnd,								// (I/O) ����ِ�
								HCHAR*				LayNam,								// (I  ) ڲԖ�
								MINT				Symtry = 0,							// (I  ) �Ώ��׸�  0:�Ȃ�  1:����
								HMTRX3				Matrix = NULL,						// (I  ) �ϊ���ظ�
								HPOINT3*			Move   = NULL,						// (I  ) �ړ���
								HREAL				Scale  = 1.0f,						// (I  ) ����
								HPOINT2*			CutPnt = NULL);						// (I  ) ��Đ� + �����_

	// ���C���E�u���b�N���𓾂�
	MINT GetLayBlkNum          ();														// (  O) ڲԥ��ۯ���

	// ���C���E�u���b�N�̌���
	MINT GetLayBlkInf          (														// (  O) �ð��  0:����  1:�װ
								MINT				Idx,								// (I  ) ���ޯ��
								HCHAR*				Name = NULL,						// (  O) ڲԥ��ۯ���
								COLORREF*			Rgb  = NULL);						// (  O) RGB�װ�l

	// ���C���E�u���b�N�ɂq�f�a��ݒ肷��
	MINT SetLayBlkRgb          (														// (  O) �ð��  0:����  1:�װ
								HCHAR*				Name,								// (I  ) ڲԥ��ۯ���
								COLORREF			Rgb);								// (I  ) RGB�װ�l

	MINT SetDirectX            (														// (  O) �ð��  0:����  1:�װ
								MINT				DMode,								// (I  ) 0;�� 1:�� 2:����
								LPDIRECT3DDEVICE8	LpD3DDEV,							// (I  ) DirectX���޲�
								HstackTexture*		TextList,							// (I  ) ÷���ؽ�
								HstackShowObj*		ShowList,							// (I  ) �\���Ώ�ؽ�
								HCHAR				TexNam[50][64] = NULL,				// (I  ) ÷�����
								COLORREF*			RgbInf = NULL,						// (I  ) �\���F
								MINT				ArrFlg = 0,							// (I  ) ���\���׸�  0:�Ȃ� 1:����
								MINT				Symtry = 0,							// (I  ) �Ώ��׸�  0:�Ȃ�  1:����
								HMTRX3				Matrix = NULL,						// (I  ) �ϊ���ظ�
								HPOINT3*			Move   = NULL,						// (I  ) �ړ���
								HMINMAX3*			MinMax = NULL);						// (  O) �ŏ���ő�

	// �c�w�e�����c�����������w�ŕ\������
	HVOID ShowDirectX          (
								MINT				DMode,								// (I  ) 0;�� 1:�� 2:����
								LPDIRECT3DDEVICE8	LpD3DDEV,							// (I  ) DirectX���޲�
								LPDIRECT3DTEXTURE8	Texture = NULL);					// (I  ) ÷���

	// �c�w�e�����c�����������w����J������
	MINT RelDirectX            (
								MINT				DMode);								// (I  ) 0;�� 1:�� 2:����

	// �����c�����������w�ɐݒ肷��
	MINT SetDirectXY           (														// (  O) �ð��  0:����  1:�װ
								LPDIRECT3DDEVICE8		LpD3DDEV);						// (I  ) DirectX���޲�

	//  �����c�����������w�ŕ\������
	HVOID ShowDirectXY         (
								LPDIRECT3DDEVICE8		LpD3DDEV);						// (I  ) DirectX���޲�

	// �����c�����������w����J������
	HVOID RelDirectXY          ();


private:

};


/******************************************************************************************************************
[�@�\] �c�w�e�t�@�C���Ǎ���
[�Ԓl] �c�w�e���
******************************************************************************************************************/
HsDxfInf* HsDxfRead            (														// (  O) DXF���
								HCHAR*				FlName,								// (I  ) ̧�ٖ�
								MINT				VewTyp,								// (I  ) �ޭ����
								MINT				ZukVec = 0,							// (I  ) 1:�㉺  2:���E  4:-90��  5:+90��
																						//       6:X�ړ� 7:Y�ړ� 8:Z�ړ�
								HREAL				MVal   = 0.f,						// (I  ) �ړ���
								MINT				FAFlag = 0);						// (I  ) �ʂ̌�������   0:���Ȃ�  1:����

/******************************************************************************************************************
[�@�\] �w�b�_�Z�N�V������ǂݍ��݁A�~�ʂ̉�]�����f�[�^��ϐ��ɐݒ肷��
[�Ԓl] = 0 ����  < 0�G���[
******************************************************************************************************************/
MINT HsDxfGetHdSec             (														// (  O) �ð��
								CStdioFile*			FLPtr,								// (I  ) ̧�َ��ʎq
								HsDxfInf*			DxfInf);							// (I/O) DXF���

/******************************************************************************************************************
[�@�\] �u���b�N�Z�N�V������Ǎ��݁A�u���b�N�e�[�u�����쐬����
[�Ԓl] = 0 ����  < 0�G���[
******************************************************************************************************************/
MINT HsDxfGetBlcSec            (														// (   O) �ð��
								CStdioFile*			FLPtr,								// (I  ) ̧�َ��ʎq
								HsDxfInf*			DxfInf);							// (I/O) DXF���

/******************************************************************************************************************
[�@�\] �G���e�B�e�B�Z�N�V������Ǎ��݁A�v���~�e�B�u�ɓW�J����
[�Ԓl] = 0 ����  < 0�G���[
******************************************************************************************************************/
MINT HsDxfGetEntSec            (														//(  O)�I���X�e�[�^�X
								CStdioFile*			FLPtr,								// (I  ) ̧�َ��ʎq
								HsDxfInf*			DxfInf);							// (I/O) DXF���

/******************************************************************************************************************
[�@�\] �N���X�Z�N�V������ǂݔ�΂�
[�Ԓl] = 0 ����  < 0�G���[  > 0 �x�� ��\�����ڂł���
******************************************************************************************************************/
MINT HsDxfGetClsSec            (														// (  O) �ð��
								CStdioFile*			FLPtr);								// (I  ) ̧�َ��ʎq

/******************************************************************************************************************
[�@�\] �I�u�W�F�N�g�Z�N�V������ǂݔ�΂�
[�Ԓl] = 0 ����  < 0�G���[  > 0 �x�� ��\�����ڂł���
******************************************************************************************************************/
MINT HsDxfGetObjSec            (														// (  O) �ð��
								CStdioFile*			FLPtr);								// (I  ) ̧�َ��ʎq

/******************************************************************************************************************
[�@�\] �t�H���g�e�[�u���̓Ǎ��݂Ɛݒ�
[�Ԓl] = 0 ����  < 0�G���[
******************************************************************************************************************/
MINT HsDxfGetTblFont           (														// (  O) �ð��
								CStdioFile*			FLPtr,								// (I  ) ̧�َ��ʎq
								HsDxfInf*			DxfInf);							// (I/O) DXF���

/******************************************************************************************************************
[�@�\] ����e�[�u���̓Ǎ��݂Ɛݒ�
[�Ԓl] = 0 ����  < 0�G���[
******************************************************************************************************************/
MINT HsDxfGetTblLin            (														// (  O) �ð��
								CStdioFile*			FLPtr,								// (I  ) ̧�َ��ʎq
								HsDxfInf*			DxfInf);							// (I/O) DXF���

/******************************************************************************************************************
[�@�\] ���C���e�[�u���̓Ǎ��݂Ɛݒ�
[�Ԓl] = 0 ����  < 0�G���[
******************************************************************************************************************/
MINT HsDxfGetTblLyr            (														// (  O) �ð��
								CStdioFile*			FLPtr,								// (I  ) ̧�َ��ʎq
								HsDxfInf*			DxfInf);							// (I/O) DXF���

/******************************************************************************************************************
[�@�\] �e�[�u���Z�N�V������Ǎ��ށB�ΏۂƂȂ�e�[�u����,LTYPE(����),STYLE(��������),LAYER(��w)
[�Ԓl] = 0 ����  < 0�G���[
******************************************************************************************************************/
MINT HsDxfGetTblSec            (														// (   O) �ð��
								CStdioFile*			FLPtr,								// (I  ) ̧�َ��ʎq
								HsDxfInf*			DxfInf);								// (I/O) DXF���

/******************************************************************************************************************
[�@�\] �~�ʏ���ǂݍ��݁A���C���������̓u���b�N���̃v���~�e�B�u���X�g�֒ǉ�����
[�Ԓl] = 0 ����  < 0�G���[  > 0 �x�� ��\�����ڂł���
******************************************************************************************************************/
MINT HsDxfGetPrmArc            (														// (  O) �ð��
								CStdioFile*			FLPtr,								// (I  ) ̧�َ��ʎq
								HCHAR*				Data,								// (  O) �Ǎ��ݗ̈�
								HsDxfInf*			DxfInf,								// (I/O) DXF���
								HsDxfBlock*			Block);								// (I  ) ��ۯ�
																						//       == NULL ڲԂ֓Ǎ���
																						//       != NULL �w����ۯ��֓Ǎ���

/******************************************************************************************************************
[�@�\] ��������Ǎ��݁A���C���������̓u���b�N���̃v���~�e�B�u���X�g�֒ǉ�����
[�Ԓl] = 0 ����  < 0�G���[  > 0 �x�� ��\�����ڂł���
******************************************************************************************************************/
MINT HsDxfGetPrmAtb            (														// (  O) �ð��
								CStdioFile*			FLPtr,								// (I  ) ̧�َ��ʎq
								HCHAR*				Data,								// (  O) �Ǎ��ݗ̈�
								HsDxfInf*			DxfInf,								// (I/O) DXF���
								HsDxfBlock*			Block);								// (I  ) ��ۯ�
																						//       == NULL ڲԂ֓Ǎ���
																						//       != NULL �w����ۯ��֓Ǎ���

/******************************************************************************************************************
[�@�\] �~����Ǎ��݁A���C���������̓u���b�N���̃v���~�e�B�u���X�g�֒ǉ�����
[�Ԓl] = 0 ����  < 0�G���[  > 0 �x�� ��\�����ڂł���
******************************************************************************************************************/
MINT HsDxfGetPrmCir            (														// (  O) �ð��
								CStdioFile*			FLPtr,								// (I  ) ̧�َ��ʎq
								HCHAR*				Data,								// (  O) �Ǎ��ݗ̈�
								HsDxfInf*			DxfInf,								// (I/O) DXF���
								HsDxfBlock*			Block);								// (I  ) ��ۯ�
																						//       == NULL ڲԂ֓Ǎ���
																						//       != NULL �w����ۯ��֓Ǎ���

/******************************************************************************************************************
[�@�\] �_����Ǎ��݁A���C���������̓u���b�N���̃v���~�e�B�u���X�g�֒ǉ�����
[�Ԓl] = 0 ����  < 0�G���[  > 0 �x�� ��\�����ڂł���
******************************************************************************************************************/
MINT HsDxfGetPrmDot            (														// (  O) �ð��
								CStdioFile*			FLPtr,								// (I  ) ̧�َ��ʎq
								HCHAR*				Data,								// (  O) �Ǎ��ݗ̈�
								HsDxfInf*			DxfInf,								// (I/O) DXF���
								HsDxfBlock*			Block);								// (I  ) ��ۯ�
																						//       == NULL ڲԂ֓Ǎ���
																						//       != NULL �w����ۯ��֓Ǎ���

/******************************************************************************************************************
[�@�\] �ʏ���Ǎ��݁A���C���������̓u���b�N���̃v���~�e�B�u���X�g�֒ǉ�����
[�Ԓl] = 0 ����  < 0�G���[  > 0 �x�� ��\�����ڂł���
******************************************************************************************************************/
MINT HsDxfGetPrmFac            (														// (  O) �ð��
								CStdioFile*			FLPtr,								// (I  ) ̧�َ��ʎq
								HCHAR*				Data,								// (  O) �Ǎ��ݗ̈�
								HsDxfInf*			DxfInf,								// (I/O) DXF���
								HsDxfBlock*			Block);								// (I  ) ��ۯ�
																						//       == NULL ڲԂ֓Ǎ���
																						//       != NULL �w����ۯ��֓Ǎ���

/******************************************************************************************************************
[�@�\] �u���b�N��}������
[�Ԓl] = 0 ����  < 0�G���[  > 0 �x�� ��\�����ڂł���
******************************************************************************************************************/
MINT HsDxfGetPrmIns            (														// (  O) �ð��
								CStdioFile*			FLPtr,								// (I  ) ̧�َ��ʎq
								HCHAR*				Data,								// (  O) �Ǎ��ݗ̈�
								HsDxfInf*			DxfInf,								// (I/O) DXF���
								HsDxfBlock*			Block);								// (I  ) ��ۯ�
																					//       == NULL ڲԂ֓Ǎ���
																					//       != NULL �w����ۯ��֓Ǎ���

/******************************************************************************************************************
[�@�\] ������ǂݍ��݁A���C���������̓u���b�N���̃v���~�e�B�u���X�g�֒ǉ�����
[�Ԓl] = 0 ����  < 0�G���[  > 0 �x�� ��\�����ڂł���
******************************************************************************************************************/
MINT HsDxfGetPrmLin            (														// (  O) �ð��
								CStdioFile*			FLPtr,								// (I  ) ̧�َ��ʎq
								HCHAR*				Data,								// (  O) �Ǎ��ݗ̈�
								HsDxfInf*			DxfInf,								// (I/O) DXF���
								HsDxfBlock*			Block);								// (I  ) ��ۯ�
																						//       == NULL ڲԂ֓Ǎ���
																						//       != NULL �w����ۯ��֓Ǎ���

/******************************************************************************************************************
[�@�\] �|�����C������Ǎ��݁A���C���������̓u���b�N���̃v���~�e�B�u���X�g�֒ǉ�����
[�Ԓl] = 0 ����  < 0�G���[  > 0 �x�� ��\�����ڂł���
******************************************************************************************************************/
MINT HsDxfGetPrmPln            (														// (  O) �ð��
								CStdioFile*			FLPtr,								// (I  ) ̧�َ��ʎq
								HCHAR*				Data,								// (  O) �Ǎ��ݗ̈�
								HsDxfInf*			DxfInf,								// (I/O) DXF���
								HsDxfBlock*			Block);								// (I  ) ��ۯ�
																						//       == NULL ڲԂ֓Ǎ���
																						//       != NULL �w����ۯ��֓Ǎ���

/******************************************************************************************************************
[�@�\] ��������Ǎ��݁A���C���������̓u���b�N���̃v���~�e�B�u���X�g�֒ǉ�����
[�Ԓl] = 0 ����  < 0�G���[  > 0 �x�� ��\�����ڂł���
******************************************************************************************************************/
MINT HsDxfGetPrmTxt            (														// (  O) �ð��
								CStdioFile*			FLPtr,								// (I  ) ̧�َ��ʎq
								HCHAR*				Data,								// (  O) �Ǎ��ݗ̈�
								HsDxfInf*			DxfInf,								// (I/O) DXF���
								HsDxfBlock*			Block);								// (I  ) ��ۯ�
																						//       == NULL ڲԂ֓Ǎ���
																						//       != NULL �w����ۯ��֓Ǎ���

/******************************************************************************************************************
[�@�\] �|�����C����̂Q�_�Ɩc��݂���A�~�ʏ����쐬����
[�Ԓl] = 0 ����  < 0�G���[
******************************************************************************************************************/
MINT HsDxfPlnToArc             (
								HPOINT3*			SPnt,								// (I  ) �J�n�_
								HPOINT3*			EPnt,								// (I  ) �I���_
								HREAL				Tan,								// (I  ) �c���
								HARC3S*				Arc);								// (  O) �~�ʏ��

/******************************************************************************************************************
[�@�\] �n�_�I�_�ƒ��S�p�x����~�ʂ̒��S�Ɣ��a�����߂�  �i�Q�����}�`�������Q�_��Z���W��0.0�łȂ��ꍇ�̓G���[�Ƃ���j
[�Ԓl] = 0 ����  < 0�G���[ 
******************************************************************************************************************/
MINT HsDxfCalArcAtr            (
								HPOINT3*			Pnt1,								// (I  ) �P�_��
								HPOINT3*			Pnt2,								// (I  ) �Q�_��
								HREAL				Ang,								// (I  ) ���S�p�x
								HPOINT3*			Center,								// (  O) ���S�_
								HREAL*				Radius);							// (  O) ���a

/******************************************************************************************************************
[�@�\] �~�ʂ̓_�̊p�x�����߂�  �i�~�ʂ�XY���ʁiZ=0)��ɂ̂��Ă�����̂Ƃ���j
[�Ԓl] = 0 ����  < 0�G���[ 
******************************************************************************************************************/
HREAL HsDxfGetAngle            (														// (  O) �p�x
								HPOINT3*			Center,								// (I  ) �~�ʒ��S
								HPOINT3*			Pnt);								// (I  ) �~�ʏ�̓_

/******************************************************************************************************************
[�@�\] �c�w�e�t�@�C�����Q���C���Ǎ��ݕҏW���� �i�R�����g�s�͓ǂݔ�΂��j
[�Ԓl] = 0 ����  < 0�G���[
******************************************************************************************************************/
MINT HsDxfGetLine              (														// (  O) �ð��
								CStdioFile*			FLPtr,								// (I  ) ̧�َ��ʎq
								MINT*				Code,								// (  O) ��ٰ�ߺ���
								HCHAR*				Data);								// (  O) ������

/******************************************************************************************************************
[�@�\] �F�ԍ�����F�𓾂�
[�Ԓl] = 0 ����  < 0�G���[
******************************************************************************************************************/
COLORREF HsDxfGetCol           (														// (  O) RGB�װ�l
								MINT				ColNo);								// (I  ) �F�ԍ�

HVOID    HsDxfGetCol           (														// (  O) RGB�װ�l
								MINT				ColNo,								// (I  ) �F�ԍ�
								MINT*				Red,								// (  O) ��
								MINT*				Green,								// (  O) ��
								MINT*				Blue);								// (  O) ��


#endif			//  _HSDXF_

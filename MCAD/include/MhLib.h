#pragma once

#define	TRACE_IO		0			// Read / Write �g���[�X�@�t���O�@( 0 | 1)		�W���l(0)
#define DEBUG1			0			// 
#define Disp3DBox		0			// 1: 3D�̉ƃ��f���̑����3D�̔���\������, 0: 3D�̉ƃ��f����\������

namespace MC
{

class	MhPlcInfo;
class	MhRoofInfo;

class	MhMbr;
class	MhGp;
class	MhTpPts;

/////////////////////////////////////////////////////////////////////////////
//�y�@�\�z�萔
//

#define MH_INS	1
#define MH_MOD	2
#define MH_DEL	3

#ifdef DLL_EXPORT
	#undef DLL_EXPORT
#endif

#ifdef DLL_EXPORT_IEMODEL_DO
	#pragma message( "<<< IeModel�Edll_EXport >>>")
	#define DLL_EXPORT							__declspec( dllexport)
#else
	#ifdef DLL_NO_IMPORT_MD_DO
		#define DLL_EXPORT
	#else
//		#pragma message( "=== IeModel�Edll_IMport ===")
		#define DLL_EXPORT						__declspec( dllimport)
	#endif
#endif

class DLL_EXPORT IeModel
{
public:
	/////////////////////////////////////////////////////////////////////////////
	//�y�@�\�z�ėp�֐�
	//

	/////////////////////////////////////////////////////////////////////////////
	//�y�@�\�z�ƃ��f���C���t���O�̐ݒ�
	static void MhSetModFIeMdl( bool mod);

	/////////////////////////////////////////////////////////////////////////////
	//�y�@�\�z�ƃ��f���C���t���O�̌���
	static bool MhGetModFIeMdl();

	////////////////////////////////////////////////////////////////////////////
	//�y�@�\�z���i�f�[�^��ǂݍ���

	static void MhInitPts();

	////////////////////////////////////////////////////////////////////////////
	//�y�@�\�z���͑����l������������

	static void MhInitInpAt();

	////////////////////////////////////////////////////////////////////////////
	//�y�@�\�z�ǃf�[�^�@�ǉ��A�C���t���O���Z�b�g����

	static void MhKabeSetUpFlg();

	////////////////////////////////////////////////////////////////////////////
	//�y�@�\�z�ǂ𐳋K������(�������ŕ������A���i���ŘA������)

	static void MhNormKabe(
					const	MINT		nProc			// (I  ) ������	  0 : �S�Ǐ���	
														//				 >0 : �Ō��nProc�ǂ���������
					);

	////////////////////////////////////////////////////////////////////////////
	//�y�@�\�z��b�f�[�^�@�ǉ��A�C���t���O���Z�b�g����

	static void MhKisoSetUpFlg();

	////////////////////////////////////////////////////////////////////////////
	//�y�@�\�z��b�𐳋K������(�������ŕ������A���i���ŘA������)

	static void MhNormKiso(
					const	MINT		nProc			// (I  ) �������@0 : �S��b����	
														//				>0 : �Ō��nProc��b����������
					);

	////////////////////////////////////////////////////////////////////////////
	//�y�@�\�z�ǂݍ��݉ƃ��f���t�@�C�������擾����
	//�y�Ԓl�z	0 : �L�����Z���A1 : �n�j

	static MINT MhFileDialog(
					const	MINT		DFlag,			// (I  ) 0:�J��  1:�ۑ�
					const	MCHAR*		Path1,			// (I  ) �O���̧�ٖ�  (���߽)
							MCHAR*		Path2			// (  O) �I������̧�ٖ�(���߽)
					);

	////////////////////////////////////////////////////////////////////////////
	//�y�@�\�z�t���p�X������A�t�H���_�[�p�X���A�t�@�C�����A�t�@�C���^�C�g���A
	//						�@�t�@�C���g���q���擾����

	static void MhSeparateFname(
					const	MCHAR*		cFilePath,			// (I  ) �t�@�C�����i�t���p�X�j
							MCHAR*		cFolderPath = NULL,	// (  O) �t�H���_�[�p�X��	�܂��́@NULL
							MCHAR*		cFileName = NULL,	// (  O) �t�@�C����			�܂��́@NULL
							MCHAR*		cFileTitle = NULL,	// (  O) �t�@�C���^�C�g��	�܂��́@NULL
							MCHAR*		cFileExt = NULL		// (  O) �t�@�C���g���q		�܂��́@NULL
					);
	////////////////////////////////////////////////////////////////////////////
	//�y�@�\�z�\���ƃ��f���ۑ�
	//�y�Ԓl�z	0 : ����A1 : �ُ�

	static MINT MhMdlSave(
					const	MCHAR*		cFilePath		// (I  ) �\���ƃ��f���ۑ���t�@�C�����i�t���p�X�j
					);

	////////////////////////////////////////////////////////////////////////////
	//�y�@�\�z�\���ƃ��f���ǂݍ���
	//�y�Ԓl�z  0 : ����A1 : �ُ�

	static MINT MhMdlLoad(
							MCHAR*		cFilePath		// (I  ) �ǂݍ��ݍ\���ƃ��f���t�@�C�����i�t���p�X�j
					);


//	static class MhTategu* IeModel::MhpInpTategu();
//	{
//		return &g_hInpTateguAttr;
//	}
//	static class MhPlcInfo* IeModel::MhpInpPI();
//	{
//		return &g_hInpPIAttr;
//	}

};

////////////////////////////////////////////////////////////////////////////
//�y�@�\�z�\���ƃ��f���w�b�_

class	MhMdlHeader
{
public:
	MCHAR		cTitle[12];							// �^�C�g��
	MCHAR		cVer[8];						// �o�[�W����
	MTIME		cAscTime[24];
	CTime		cTime;
};

////////////////////////////////////////////////////////////////////////////
//�y�@�\�z�\���ƃ��f��

class MhMdlIO
{
public:
	MINT		m_iMode;							// �ǂݏ������[�h	MBREAD  : �ǂݍ���
													//					MBWRITE : ��������
	CFile		m_File;								// �\���ƃ��f��CFile�I�u�W�F�N�g
	MCHAR		m_MdlPath[MX_PATH];					// �\���ƃ��f���t�@�C�����i�t���p�X�j
	MhMdlHeader	m_Header;							// �\���ƃ��f���w�b�_�[

//	�R���X�g���N�^
	MhMdlIO();

//	�f�X�g���N�^
	~MhMdlIO();

	////////////////////////////////////////////////////////////////////////////
	//�y�@�\�z�t�@�C���ǂݏ��������J�n�錾
	//�y�Ԓl�z==  0 : ����I��
	//		  == -2 : ���ɃI�[�v������Ă���
	//		  �� -3 : ���̑��G���[

	MINT Open(
				const	DWORD		iMode,			// (I ) �g�p���[�h
													//		== MBREAD:	�ǂݍ��ݐ�p
													//		== MBWRITE:	�������ݐ�p
				const	MCHAR*		cFilePath
				);

	////////////////////////////////////////////////////////////////////////////
	//�y�@�\�z�t�@�C���ǂݏ��������I���錾
	//�y�Ԓl�z==  0 : ����I��
	//		  ��  0 : ���̑��G���[

	MINT Close();

	////////////////////////////////////////////////////////////////////////////
	//�y�@�\�z�t�@�C���ǂݍ���
	//�y�Ԓl�z>=  0 : ����I�� �ǂݍ��݃o�C�g��
	//		   <  0 : ���̑��G���[

	MINT Read(
						MINT*		piType,			// (  O) ����
						void**		Buff			// (  O) �Ǎ��ݗ̈�
				);

	//////////////////////////////////////////////////////////////////////////////
	////�y�@�\�z�C�ӌ^�f�[�^�̃t�@�C����������
	////�y�Ԓl�z==  0 : ����I��
	////		  ��  0 : ���̑��G���[
	//
	//MINT MhMdlIO::Write(
	//				const	MINT		i_tp,			// ����
	//				const	void*		i_bf,			// �����ݗ̈�
	//				const	MINT		i_sz			// �������޲Đ�
	//				);

	////////////////////////////////////////////////////////////////////////////
	//�y�@�\�zSHORT�^�f�[�^�̃t�@�C����������
	//�y�Ԓl�z==  0 : ����I��
	//		  ��  0 : ���̑��G���[

	MINT Write(
				const	MINT		i_tp,			// ����
				const	MSHORT*		i_ibf,			// ������SHORT�z��
				const	MINT		i_iwd = 1		// �����݃��[�h��
				);

	////////////////////////////////////////////////////////////////////////////
	//�y�@�\�z�����^�f�[�^�̃t�@�C����������
	//�y�Ԓl�z==  0 : ����I��
	//		  ��  0 : ���̑��G���[

	MINT Write(
				const	MINT		i_tp,			// ����
				const	MINT*		i_ibf,			// �����ݐ����z��
				const	MINT		i_iwd = 1		// �����݃��[�h��
				);

	////////////////////////////////////////////////////////////////////////////
	//�y�@�\�z�����^�f�[�^�̃t�@�C����������
	//�y�Ԓl�z==  0 : ����I��
	//		  ��  0 : ���̑��G���[

	MINT Write(
				const	MINT		i_tp,			// ����
				const	MUINT*		i_ibf,			// �����ݐ����z��
				const	MINT		i_iwd = 1		// �����݃��[�h��
				);

	////////////////////////////////////////////////////////////////////////////
	//�y�@�\�zFLOAT�^�f�[�^�̃t�@�C����������
	//�y�Ԓl�z==  0 : ����I��
	//		  ��  0 : ���̑��G���[

	MINT Write(
				const	MINT		i_tp,			// ����
				const	float*		i_rbf,			// �����ݎ����z��
				const	MINT		i_iwd = 1		// �����݃��[�h��
				);

	////////////////////////////////////////////////////////////////////////////
	//�y�@�\�zDOUBLE�^�f�[�^�̃t�@�C����������
	//�y�Ԓl�z==  0 : ����I��
	//		  ��  0 : ���̑��G���[

	MINT Write(
				const	MINT		i_tp,			// ����
				const	double*		i_rbf,			// �����ݎ����z��
				const	MINT		i_iwd = 1		// �����݃��[�h��
				);

	//////////////////////////////////////////////////////////////////////////////
	////�y�@�\�zBYTE�^�f�[�^�̃t�@�C����������
	////�y�Ԓl�z==  0 : ����I��
	////		  ��  0 : ���̑��G���[
	//
	//MINT MhMdlIO::Write(
	//				const	MINT		i_tp,			// ����
	//				const	MBYTE*		i_ibf,			// ������BYTE�z��
	//				const	MINT		i_ibt = 1		// ������BYTE�^�o�C�g��
	//				);

	////////////////////////////////////////////////////////////////////////////
	//�y�@�\�z�����^(char)�f�[�^�̃t�@�C����������
	//�y�Ԓl�z==  0 : ����I��
	//		  ��  0 : ���̑��G���[

	MINT Write(
				const	MINT		i_tp,			// ����
				const	char*		i_cDat,			// �����ݕ�����
				const	MINT		i_sz = 1		// �����ݕ�����
				);

	////////////////////////////////////////////////////////////////////////////
	//�y�@�\�z�����^(_TCHAR)�f�[�^�̃t�@�C����������
	//�y�Ԓl�z==  0 : ����I��
	//		  ��  0 : ���̑��G���[

	MINT Write(
				const	MINT		i_tp,			// ����
				const	_TCHAR*		i_cDat,			// �����ݕ�����
				const	MINT		i_sz			// �����ݕ�����
				);

	////////////////////////////////////////////////////////////////////////////
	//�y�@�\�z�����^(_TCHAR)�f�[�^�̃t�@�C����������
	//�y�Ԓl�z==  0 : ����I��
	//		  ��  0 : ���̑��G���[

	MINT Write(
				const	MINT		i_tp,			// ����
				const	CString*	i_sDat,			// �����ݕ�����
				const	MINT		i_sz = 1		// �����ݕ�����
				);

	////////////////////////////////////////////////////////////////////////////
	//�y�@�\�z�����^(_TCHAR)�f�[�^�̃t�@�C����������
	//�y�Ԓl�z==  0 : ����I��
	//		  ��  0 : ���̑��G���[

	MINT Write(
				const	MINT		i_tp,			// ����
				const	bool*		i_bDat,			// �����ݕ�����
				const	MINT		i_sz			// �����ݕ�����
				);

	////////////////////////////////////////////////////////////////////////////////
	//////�y�@�\�z�t�@�C���ǂݍ���
	//////�y�Ԓl�z>=  0 : ����I�� �ǂݍ��݃o�C�g��
	//////		   <  0 : ���̑��G���[
	////
	////	MINT Read(
	////						MINT*		piType,			// (  O) ����
	////						void**		Buff			// (  O) �Ǎ��ݗ̈�
	////				);
	////
	////////////////////////////////////////////////////////////////////////////////
	//////�y�@�\�z�t�@�C����������
	//////�y�Ԓl�z==  0 : ����I��
	//////		  ��  0 : ���̑��G���[
	////
	////	MINT Write(
	////				const	MINT		iType,			// (I  ) ����
	////				const	MINT		Size,			// (I  ) �����݃T�C�Y�i�o�C�g���j
	////				const	void*		Buff			// (I  ) �����ݗ̈�
	////				);
	////
	////////////////////////////////////////////////////////////////////////////////
	//////�y�@�\�z�t�@�C���ǂݍ���
	//////�y�Ԓl�z>=  0 : ����I�� �ǂݍ��݃o�C�g��
	//////		   <  0 : ���̑��G���[
	////
	////	MINT Read(
	////						MINT*		piType,			// (  O) ����
	////						void**		Buff			// (  O) �Ǎ��ݗ̈�
	////				);
	////
	////////////////////////////////////////////////////////////////////////////////
	//////�y�@�\�z�t�@�C����������
	//////�y�Ԓl�z==  0 : ����I��
	//////		  ��  0 : ���̑��G���[
	////
	////	MINT Write(
	////				const	MINT		iType,			// (I  ) ����
	////				const	MINT		Size,			// (I  ) �����݃T�C�Y�i�o�C�g���j
	////				const	void*		Buff			// (I  ) �����ݗ̈�
	////				);

};

class MsBitSet;

class Option
{
public:
	/////////////////////////////////////////////////////////////////////////////
	//�y�@�\�z�V�K���R�[�h�ǉ����ɐݒ肷��n�o�s�Q�R�[�h���擾����
	//		�@�i�n�o�s�Q�R�[�h������G���A�̊m�ۂ͖{�֐��ōs���j
	static void MhAddOpt(
							MsBitSet*		*ppOptv		// (  O) �ǉ����R�[�h�ɐݒ肷��n�o�s�Q�R�[�h�̃|�C���^ NULL: �S�n�o�s�Q
					 );

	/////////////////////////////////////////////////////////////////////////////
	//�y�@�\�z���R�[�h�C�����ɐݒ肷��n�o�s�Q�R�[�h���擾����
	//�y�Ԓl�z�C����̃��R�[�h�\���R�[�h
	//			1 : �C�����n�o�s�Q�͏����݂n�o�s�Q�Ɠ���̏ꍇ�ŁA
	//				�C�������R�[�h�͂n�o�s�Q�ύX�Ȃ��ŏC�����e���㏑������B
	//				�ǉ����R�[�h�͂Ȃ��B
	//			2 :	�C�����n�o�s�Q�͏����݂n�o�s�Q�ɑS�Ċ܂܂ꂩ�ǉ��n�o�s�Q������ꍇ�ŁA
	//				�C�������R�[�h�͏����݂n�o�s�Q�ɕύX���A�C�����e���㏑������B
	//				�ǉ����R�[�h�͂Ȃ��B
	//			3 : �C�����n�o�s�Q�ɏ����݂n�o�s�Q�ȊO�̂n�o�s�Q���܂ޏꍇ�ŁA
	//				�����݂n�o�s�Q�ȊO�̂n�o�s�Q�����́A�C�������R�[�h�����̂܂܎c���n�o�s�Q�R�[�h�̂ݏC������B
	//				����ɁA�����݂n�o�s�Q�����C����̃��R�[�h��V���ɒǉ�����B
	//				�i�n�o�s�Q�R�[�h������G���A�̊J���Ɗm�ۂ͖{�֐��ōs���j
	static MINT MhModOpt(
							MhPlcInfo	 *pPlcEnR,		// (I O) �C��������
							MhPlcInfo* *ppPlcEnM		// (  O) �C���敔��
					);

	/////////////////////////////////////////////////////////////////////////////
	//�y�@�\�z���R�[�h�폜���ɐݒ肷��n�o�s�Q�R�[�h���擾����
	//�y�Ԓl�z�폜��̃��R�[�h�\���R�[�h
	//			1 : �폜���n�o�s�Q�͏����݂n�o�s�Q�Ɠ���̏ꍇ�ŁA
	//				�폜�����R�[�h�͍폜����B
	//			2 :	�폜���n�o�s�Q�͏����݂n�o�s�Q�ɑS�Ċ܂܂ꂩ�ǉ��n�o�s�Q������ꍇ�ŁA
	//				�폜�����R�[�h�͍폜����B
	//			3 : �폜���n�o�s�Q�ɏ����݂n�o�s�Q�ȊO�̂n�o�s�Q���܂ޏꍇ�ŁA
	//				�����݂n�o�s�Q�ȊO�̂n�o�s�Q�����́A�폜�����R�[�h�����̂܂܎c���n�o�s�Q�R�[�h�̂ݏC������B
	//				�i�n�o�s�Q�R�[�h������G���A�̊J���Ɗm�ۂ͖{�֐��ōs���j


	static MINT MhDelOpt(
	//D						MsBitSet		*pOptR,			// (I O) �폜�����R�[�h��OPT�Q�Z�b�g
							MsBitSet*		*ppOptRN1,		// (I O) �폜�O��̃��R�[�h�̕\���n�m�@OPT�Q�Z�b�g
							MsBitSet*		*ppOptRN2		// (I O) �폜�O��̃��R�[�h�̕\���n�e�eOPT�Q�Z�b�g
					);

	/////////////////////////////////////////////////////////////////////////////
	//�y�@�\�z�n�o�s�Q�R�[�h�̗̈���J������
	static void MhFreOpt(
							MsBitSet	*pOptv		// (I O) �J������n�o�s�Q�R�[�h�̃|�C���^ NULL: �J���s�v
					);

};

class Version {

	/////////////////////////////////////////////////////////////////////////////
	//�y�@�\�z�J�����g�o�[�W�����̐ݒ�
	static bool MhSetCurVer(
					const	MUINT		*iVer		// (I  ) �o�[�W���� NULL: �S�o�[�W����
					 );

	/////////////////////////////////////////////////////////////////////////////
	//�y�@�\�z�J�����g�o�[�W�����t�o
	static bool MhUpCurVer();

	/////////////////////////////////////////////////////////////////////////////
	//�y�@�\�z�J�����g�o�[�W�����̎捞
	static bool MhGetCurVer(
							MUINT*		*piVer		// (  O) �o�[�W���� NULL: �S�o�[�W����
					 );

	/////////////////////////////////////////////////////////////////////////////
	//�y�@�\�z�V�K���R�[�h�ǉ����ɐݒ肷��o�[�W�����R�[�h���擾����
	//		�@�i�o�[�W�����R�[�h���쐬�����̃|�C���^��Ԃ��j
	//�y�Ԓl�z�o�[�W�����R�[�h�̃|�C���^
	//		  NULL : �S�o�[�W�����L��
	static MUINT* MhAddVer();

	/////////////////////////////////////////////////////////////////////////////
	//�y�@�\�z���R�[�h�C�����ɐݒ肷��o�[�W�����R�[�h���擾����
	//�y�Ԓl�z�C����̃o�[�W�����R�[�h�̃|�C���^
	//		  NULL : �Ǎ��o�[�W�����R�[�h�Ɠ�����e�ł���A�C�����R�[�h�͓Ǎ����R�[�h�ɏ㏑�����鎖
	static MUINT* MhModVer(
							MUINT		*iVer		// (I O) �C�������R�[�h�̃o�[�W�����R�[�h�̃|�C���^
					);

	/////////////////////////////////////////////////////////////////////////////
	//�y�@�\�z���R�[�h�폜���ɐݒ肷��n�o�s�Q���擾����
	//�y�Ԓl�z�폜��̃o�[�W�����R�[�h�̃|�C���^
	//		  �@NULL : ���R�[�h���ŁA�폜���R�[�h�͍폜���鎖
	//		  ��NULL : �o�[�W�����ύX�A�폜���R�[�h�̓o�[�W�����R�[�h�̂ݏC������
	static MUINT* MhDelVer(
							MUINT*		*piVer		// (I O) �폜���R�[�h�̃o�[�W�����R�[�h�̃|�C���^
					);

	/////////////////////////////////////////////////////////////////////////////
	//�y�@�\�z�o�[�W�����R�[�h�̗̈���J������
	static void MhFreVer(
							MUINT*		*piVer		// (I O) �J���o�[�W���� NULL: �J���s�v
					);
};

#ifdef DLL_EXPORT
	#undef DLL_EXPORT
#endif

#ifdef DLL_EXPORT_BUZAICODE_DO
	#pragma message( "<<< BuzaiCode�Edll_EXport >>>")
	#define DLL_EXPORT								__declspec( dllexport)
#else
	#ifdef _DEBUG								// Release Mode �� inline�֐���undef�ɂȂ�s��Ή�
		#ifdef DLL_NO_IMPORT_MD_DO
			#define DLL_EXPORT
		#else
//			#pragma message( "=== BuzaiCode�Edll_IMport ===")
			#define DLL_EXPORT						__declspec( dllimport)
		#endif
	#else
		#define DLL_EXPORT
	#endif
#endif

class DLL_EXPORT BuzaiCode
{
public:
	// �g���R�[�h�����擾����
	static MINT MhGetNoOfGp();

	// �g���R�[�h���擾����
	static MhGp* MhGetpGp(
							MINT		iGp						// �ǂݍ��ݍ\��(�g)���R�[�h�ԍ�
					);

	// ���i��ރ��R�[�h�����擾����
	static MINT	BuzaiCode::MhGetNoOfTpPts();

	// ���i��ރ��R�[�h���擾����
	static MhTpPts* BuzaiCode::MhGetpTpPts(
							MINT		iTpPts					// �ǂݍ��ݕ��i��ރ��R�[�h�ԍ�
					);

	// ���iID���擾����
	static MINT BuzaiCode::MhGetPIIdTpPts(
							MINT		iCdBuzai					// ���ރR�[�h
					);

	// ���@�^�����R�[�h�����擾����
	static MINT	BuzaiCode::MhGetNoOfMbr();

	// ���@�^�����R�[�h���擾����
	static MhMbr*	BuzaiCode::MhGetpMbr(
							MINT		iMbr						// �ǂݍ��ݐ��@�^�����R�[�h�ԍ�
					);

	// ���@�^��ID���擾����
	static MINT	BuzaiCode::MhGetIdMbr(
							MCHAR*		pMbr						// ���@�^��
					);

	// ���ނ̂Q�����c�����擾����
	static void	BuzaiCode::MhBziSin(
							MhPlcInfo	*pBziInfo1,					// (I  ) ����
							MgLine2		*lnBziSin1					// (  O) ���ނ̐c��
					);

	// �S�z�u���e���g���[�X
	static void	BuzaiCode::MhPrintallMhPlcInfo(MCHAR* s);
};

} // namespace MC
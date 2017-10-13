/*******************************************************************************
										Created on 06/15/02 By K.Matsubayashi
										Copyright 2002 K.Matsubayashi
	�f�[�^�x�[�X���W���[��

	MDB.cpp

*******************************************************************************/
//
#include <stdio.h>
#include "MbBasic.h"
#include "Mbstd.h"
#include <..\include\mdb.h>

static		int	iSzRecHdr = (sizeof( MRID) + sizeof( MRINT) * 4 + 3) / sizeof( int); 
static		int	iSzItmHdr = (sizeof( MRINT) * 2 + 3) / sizeof( int); 
//
/*******************************************************************************

	MDB�̎g�p�J�n

*******************************************************************************/
CMDB::CMDB(
			int iMaxSzMdbBuf,			// MDB�̗̈�T�C�Y
			int iMaxMdbRId,				// �ő僌�R�[�hID (1�`iMaxMdbRId)
			int iMaxSzMdbR,				// �ő僌�R�[�h�T�C�Y
			int iMinSzMdbR				// �ŏ����R�[�h�T�C�Y
		 )
{
	MDBR *pMdbR;
	int	nRec;
	int	n;
	int	iSz;
	int	nRecB;
//
	m_MaxSzMdbBuf = iMaxSzMdbBuf;
	m_MaxMdbRId = iMaxMdbRId;
	m_MaxSzMdbR = iMaxSzMdbR;
	m_MinSzMdbR = iMinSzMdbR;
//	m_iSzEmptyRec = iiSzEmptyRec;
	
													// MDB�̑S�N���A�����̈��p��
	m_pMdbBuf = new int[m_MaxSzMdbBuf];
			MASTRACE( m_pMdbBuf, sizeof(int)*m_MaxSzMdbBuf);
	MbFill( m_pMdbBuf, 0, m_MaxSzMdbBuf);
													// �C���f�b�N�X���R�[�h��ݒ�
	m_pMdbRId = new MDBRID[iMaxMdbRId];
			MASTRACE( m_pMdbRId, sizeof(MDBRID)*iMaxMdbRId);
													// �󂫃C���f�b�N�X���R�[�h�̃����N��ݒ�
	for (n=1; n<m_MaxMdbRId;n++) {
		m_pMdbRId[n].Model = 0;
		m_pMdbRId[n].idBefore = n-1;
		m_pMdbRId[n].idNext = n+1;
	}
	m_pMdbRId[1].idBefore = F_NOLINKI;
	m_pMdbRId[m_MaxMdbRId-1].idNext = F_NOLINKI;
													// �󂫃C���f�b�N�X���R�[�h�̐擪�h�c��ݒ�
	m_id1stEmptyR = 1;
													// �󂫃G���A�̃����N�ݒ�
	m_n1stEmptyR = 0;

	nRec = 0;
	nRecB = F_NOLINKP;
													// �󂫃��R�[�h�ւ̃|�C���^���R�[�h��ݒ�
	while (nRec < m_MaxSzMdbBuf) {
		pMdbR = (MDBR *)&m_pMdbBuf[nRec];
													// �󂫃G���A���R�[�h�̃��R�[�h�T�C�Y�ɍő僌�R�[�h�T�C�Y��ݒ�
													// �ő僌�R�[�h�T�C�Y���蓖�Č�̎c��̃T�C�Y���ŏ����R�[�h�T�C�Y
													// �ȉ��̏ꍇ�́A�c��S�Ă��󂫃G���A���R�[�h�̃��R�[�h�T�C�Y�ɐݒ�
		if (m_MaxSzMdbBuf-nRec <= m_MaxSzMdbR+m_MinSzMdbR) {
			iSz = m_MaxSzMdbBuf-nRec;
		} else {
			iSz = m_MaxSzMdbR;
		}
		pMdbR->iSzRec = iSz;
		pMdbR->nBefore = nRecB;
		nRecB = nRec;
		nRec += iSz;
		pMdbR->nNext = nRec;
	};
	pMdbR->nNext = F_NOLINKP;
}
//
//	�l�c�a�̎g�p�I��
//
CMDB::~CMDB(){
	delete	m_pMdbBuf;
	delete	m_pMdbRId;
}
//
//		�l�c�a�̎g�p�J�n�錾���s���B
//	Bool	OpenDB(HMDB hMdb, LPCSTR pszFileName, UINT nOpenFlags, 
//                                  CFileException *pError = NULL);
//		�l�c�a�̎g�p�I���錾���s���B	
//	void Close(HMDB hMdb, UINT nCloseFlags);

/*******************************************************************************

	�G���e�B�e�B���R�[�h�̋󂫗̈�𓾂�
  
	return			>=0:�G���e�B�e�B���R�[�h�ʒu
	  				F_OVERFLOW: �󂫗̈�s��

*******************************************************************************/
int CMDB::GetEmptyArea(
						int iSzRec					// (I  ) �G���e�B�e�B���R�[�h�T�C�Y
					 )
{
	int 	nCur;						// �����󂫃G���A�ʒu
	MDBR 	*pCur;						// �@���|�C���^
	int 	nNext;						// �󂫗̈悩��K�v�̈�擾��̎c��̋󂫃G���A�̈ʒu
	MDBR 	*pNext;						// �@���|�C���^
	int		iSzNext;					// �@���̈�T�C�Y
//
	nCur = m_n1stEmptyR;
	pCur = (MDBR *)&m_pMdbBuf[nCur];
													// ���R�[�h������󂫗̈������	
	for (;;) {
		iSzNext = pCur->iSzRec - iSzRec;
		if (iSzNext >= 0) {							// �G���e�B�e�B���R�[�h�T�C�Y�𖞑�����󂫗̈悠��
			break;
		}
		if (pCur->nNext == F_NOLINKP) {						// ���󂫗̈�Ȃ�
			return F_OVERFLOW;
		}
		nCur = pCur->nNext;
		pCur = (MDBR *)&m_pMdbBuf[nCur];
//		pCur = (MDBR *)&m_pMdbBuf[pCur->nNext];
	}
//
													// �󂫗̈���K�v�ȃT�C�Y�̗̈���擾��̎c���
													// �ŏ��󂫗̈�T�C�Y�Ɣ�r����
	if (iSzNext < m_MinSzMdbR) {
													// �ŏ��󂫗̈�T�C�Y��菬�����ꍇ�͕���������
													// �󂫗̈�Ƃ��ĕԂ��A�O��̋󂫗̈�̃����N��ݒ�
		((MDBR *)&m_pMdbBuf[pCur->nNext])->nBefore = pCur->nBefore; 
		if (pCur->nBefore == F_NOLINKP) {
			m_n1stEmptyR = pCur->nNext;
		} else {
			((MDBR *)&m_pMdbBuf[pCur->nBefore])->nNext = pCur->nNext;
		}
	} else {
													// �ŏ��󂫗̈�T�C�Y�ȏ�̏ꍇ�͕�������
													// �󂫗̈�擾��̑O��̋󂫗̈�̃����N��ݒ�
		nNext = nCur + iSzRec;
		pNext = (MDBR *)&m_pMdbBuf[nNext];
		pNext->iSzRec = iSzNext;
		pNext->nNext = pCur->nNext;
		pNext->nBefore = pCur->nBefore;
		((MDBR *)&m_pMdbBuf[pCur->nNext])->nBefore = nNext; 
		if (pCur->nBefore == F_NOLINKP) {
			m_n1stEmptyR = nNext;
		} else {
			((MDBR *)&m_pMdbBuf[pCur->nBefore])->nNext = nNext;
		}
													// ����ꂽ�󂫃G���A�̃T�C�Y��ݒ�
		pCur->iSzRec = iSzRec;
	}
//													// ����ꂽ�󂫗̈�̓��e(�����N)���N���A
	pCur->nNext = 0;
	pCur->nBefore = 0;
	return nCur;
}
/*******************************************************************************

	�G���e�B�e�B���R�[�h��ǉ�����

	return
			>=1: �G���e�B�e�B���R�[�h�h�c
	  		F_OVERFLOW: �󂫃G���A�s��
			F_OVERFLOW: �󂫃G���e�B�e�B���R�[�h�h�c�Ȃ�

*******************************************************************************/
int CMDB::InsertRec(
						CMDBR *cMdbR,				// (I  ) �G���e�B�e�B���R�[�h
						int Model					// (I  ) �G���e�B�e�B���R�[�h�̌^�R�[�h
					)
{
	int		iSzRec;						// �ǉ����R�[�h�T�C�Y
	MRID	idEmptyRec;					// �󂫗̈�h�c
	int		iSzEmptyRec;				// �󂫗̈惌�R�[�h�T�C�Y
	int		nEmptyRec;					// �󂫗̈�ʒu
	MDBR*	MdbR;

	MdbR = cMdbR->m_pMdbr;
													// �󂫗̈�Ƌ󂫗̈�̂h�c�𓾂�
	iSzRec =  MdbR->iSzRec;
	nEmptyRec = GetEmptyArea( iSzRec);
	if (nEmptyRec < 0) return nEmptyRec;
	idEmptyRec = GetEmptyRecID();
	if (idEmptyRec <= 0) return idEmptyRec;
													// �󂫗̈�Ƀ��R�[�h���e�Ƃh�c����������
	iSzEmptyRec = ((MDBR *)&m_pMdbBuf[nEmptyRec])->iSzRec;
	MbCopy((int *)&m_pMdbBuf[nEmptyRec],(int *)MdbR, iSzRec);
	((MDBR *)&m_pMdbBuf[nEmptyRec])->idRec = idEmptyRec;
	((MDBR *)&m_pMdbBuf[nEmptyRec])->iSzRec = iSzEmptyRec;
													// �G���e�B�e�B���R�[�h�h�c���R�[�h��
													// �^�C�v�R�[�h�ƃG���e�B�e�B���R�[�h�ʒu��ݒ�
	((MDBRID *)&m_pMdbRId[idEmptyRec])->Model = Model;
	((MDBRID *)&m_pMdbRId[idEmptyRec])->nRec = nEmptyRec;
	return idEmptyRec;
} // InsertRec
/*******************************************************************************

	�g�p�ς݃G���e�B�e�B���R�[�h�G���A���J�����󂫂Ƃ���

*******************************************************************************/
void CMDB::PutEmptyArea(
						int nRec					// (I  ) �G���e�B�e�B���R�[�h�̈ʒu
					  )
{
	int		icngflg;					// �����t���O
	MDBR	*pCur;						// �ԋp�G���e�B�e�B���R�[�h�ւ̃|�C���^
	MRINT	iSzCur;						// �@���T�C�Y
	int		nNext;						// �ԋp�G���e�B�e�B���R�[�h�ɘA������G���e�B�e�B���R�[�h�ʒu
	MDBR	*pNext;						// �@���|�C���^
	MRID	idNext;						// �@���h�c
	int		iSzNext;					// �@���T�C�Y
	int		n1stEmptyR;					// �ԋp�O�̐擪�󂫃G���e�B�e�B���R�[�h�ʒu

	pCur = (MDBR *)&m_pMdbBuf[nRec];
	iSzCur  = pCur->iSzRec;
													// �ԋp�G���e�B�e�B���R�[�h���󂫃G���e�B�e�B���R�[�h��
													// �擪�Ƀ����N����
	n1stEmptyR = m_n1stEmptyR;
	((MDBR *)&m_pMdbBuf[n1stEmptyR])->nBefore = nRec;
	m_n1stEmptyR = nRec;
													// �ԋp�G���e�B�e�B���R�[�h�h�c = 0(��)
	pCur->idRec = 0;
													// �ԋp�G���e�B�e�B���R�[�h���ڃG���A���N���A����
	MbFill((int *)&m_pMdbBuf[nRec+iSzRecHdr], 0, iSzCur-iSzRecHdr);
	pCur->nBefore = F_NOLINKP;
	pCur->nNext = n1stEmptyR;
													// �ԋp�G���e�B�e�B���R�[�h�ɘA������G���A���󂫃G���A�ł���A���A
													// ���G���A�̍��v�T�C�Y���ő�G���e�B�e�B���R�[�h�T�C�Y��菬�����Ȃ�
													// �A������1�̋󂫃G���A�Ƃ���
	do {
													// �A������G���A�𓾂�
		nNext = nRec + iSzCur;
		pNext = (MDBR *)&m_pMdbBuf[nNext];
		idNext = pNext->idRec;
		iSzNext = pNext->iSzRec;
    
		if ((idNext == F_NOLINKI) && (iSzCur + iSzNext <= m_MaxSzMdbR)) {
													// �A������G���A�ƘA������
			iSzCur += iSzNext;
			pCur->iSzRec = iSzCur;
													// �A������G���A�ւ̃|�C���^���X�L�b�v����
			if (pNext->nNext != F_NOLINKP) 
				((MDBR *)&m_pMdbBuf[pNext->nNext])->nBefore = pNext->nBefore;
			if (pNext->nBefore != F_NOLINKP) 
				((MDBR *)&m_pMdbBuf[pNext->nBefore])->nNext = pNext->nNext;
													// ��������G���A�̃w�b�_�[���ƃ����N�����N���A
			pNext->nBefore = 0;
			pNext->nNext = 0;
			MbFill((int *)pNext, 0, iSzRecHdr);
													// �����t���O�𗧂Ă�
			icngflg = 1;
		} else {
													// �����t���O������
			icngflg = 0;
		}
	} while (icngflg == 1);							// �����t���O�������Ă���ԌJ��Ԃ�
	
}
/*******************************************************************************

	�G���e�B�e�B���R�[�h���폜����B

	return	0:����
			F_NOITEM   :�G���e�B�e�B���R�[�h�h�c�̌��
	  		F_POSERROR :�G���e�B�e�B���R�[�h�̈ʒu���
			F_HDRERROR :�G���e�B�e�B���R�[�h�̃w�b�_�[�`�����

*******************************************************************************/
int CMDB::DeleteRec(
						int idRec					// �iI) �G���e�B�e�B���R�[�h�h�c
					)
{
	int		nRec;						// �폜���R�[�h�ʒu
													// �G���e�B�e�B���R�[�h�h�c�̃`�F�b�N
	if (idRec < 1 || idRec >= m_MaxMdbRId-1 ||
				m_pMdbRId[idRec].Model==0) return F_NOITEM;
													// �폜���R�[�h�ʒu�Ƃh�c�̃`�F�b�N
	nRec   = m_pMdbRId[idRec].nRec;
	if (nRec < 0 || nRec >= m_MaxSzMdbBuf) return F_POSERROR;
	if (idRec != ((MDBR *)&m_pMdbBuf[nRec])->idRec) return F_HDRERROR;
													// �󂫃��R�[�h�ɂ���
	PutEmptyArea(nRec);
													// �󂫃��R�[�h�h�c�ɂ���
	PutEmptyRecID(idRec);
	return 0;
}
/*******************************************************************************

	�G���e�B�e�B���R�[�h���C������B

	return	0:����
			F_NOITEM   :�G���e�B�e�B���R�[�h�h�c�̌��
	  		F_POSERROR :�G���e�B�e�B���R�[�h�̈ʒu���
			F_HDRERROR :�G���e�B�e�B���R�[�h�̃w�b�_�[�`�����

*******************************************************************************/
int CMDB::ModifyRec(
						int idRec,					// ( I) �C���G���e�B�e�B���R�[�h�h�c
						CMDBR *pcMdbR,				// ( I) �C����G���e�B�e�B���R�[�h���e
						int Model					// ( I)	�C����G���e�B�e�B���R�[�h�̌^�R�[�h
					)
{
	int		nRec;						// �C���G���e�B�e�B���R�[�h�ʒu
	MRINT	iSzRec;						// �C���O�G���e�B�e�B���R�[�h�T�C�Y
	MRINT	iSzin;						// �C����G���e�B�e�B���R�[�h�T�C�Y
	MDBR*	MdbR;						// �C����G���e�B�e�B���R�[�h���e����

	MdbR = pcMdbR->m_pMdbr;
													// �C���G���e�B�e�B���R�[�h�h�c�ƌ^���`�F�b�N
	if (idRec < 1 || idRec >= m_MaxMdbRId-1 ||
				m_pMdbRId[idRec].Model==0) return F_NOITEM;
													// �C���G���e�B�e�B���R�[�h�ʒu���`�F�b�N
	nRec = m_pMdbRId[idRec].nRec;
	if (nRec < 0 || nRec >= m_MaxSzMdbBuf) return F_POSERROR;
													// �C���O�G���e�B�e�B���R�[�h���̂h�c���`�F�b�N
	if (idRec != ((MDBR *)&m_pMdbBuf[nRec])->idRec) return F_HDRERROR;
	
	iSzin = MdbR->iSzRec;
	iSzRec = ((MDBR *)&m_pMdbBuf[nRec])->iSzRec;
													// �C���O���C����̃G���e�B�e�B���R�[�h�T�C�Y����̏ꍇ
													// ���R�[�h�G���A���J�����ēx�m��
	if (iSzin <= iSzRec-m_MinSzMdbR || iSzin > iSzRec) {  
		PutEmptyArea( nRec);
		nRec = GetEmptyArea( iSzin);
		iSzRec = ((MDBR *)&m_pMdbBuf[nRec])->iSzRec;
	}
													// �G���e�B�e�B���R�[�h���e�̃R�s�[
	MbCopy( (int *)&m_pMdbBuf[nRec],(int *)MdbR, iSzin);
	((MDBR *)&m_pMdbBuf[nRec])->idRec = idRec;
	((MDBR *)&m_pMdbBuf[nRec])->iSzRec = iSzRec;
													// �h�c���R�[�h�Ɍ^�ƃG���e�B�e�B���R�[�h�ʒu��ݒ�
	((MDBRID *)&m_pMdbRId[idRec])->Model = Model;
	((MDBRID *)&m_pMdbRId[idRec])->nRec = nRec;
	return 0;
}
/*
													// �󂫗̈�Ƀ��R�[�h���e�Ƃh�c����������
	iSzEmptyRec = ((MDBR *)&m_pMdbBuf[nEmptyRec])->iSzRec;
	MbCopy((int *)&m_pMdbBuf[nEmptyRec],(int *)MdbR, iSzRec);
	((MDBR *)&m_pMdbBuf[nEmptyRec])->idRec = idEmptyRec;
	((MDBR *)&m_pMdbBuf[nEmptyRec])->iSzRec = iSzEmptyRec;
													// �G���e�B�e�B���R�[�h�h�c���R�[�h��
													// �^�C�v�R�[�h�ƃG���e�B�e�B���R�[�h�ʒu��ݒ�
	((MDBRID *)&m_pMdbRId[idEmptyRec])->Model = Model;
	((MDBRID *)&m_pMdbRId[idEmptyRec])->nRec = nEmptyRec;

*/
/*******************************************************************************

	�G���e�B�e�B���R�[�h�𓾂�B

	return 0:����
			F_NOITEM(-1)  :�G���e�B�e�B���R�[�h�h�c�̌��
	  		F_POSERROR(-2):�G���e�B�e�B���R�[�h�̈ʒu���
			F_HDRERROR(-3):�G���e�B�e�B���R�[�h�̃w�b�_�[�`�����

*******************************************************************************/
int CMDB::GetRec(
						int idRec,					// ( I) �G���e�B�e�B���R�[�h�h�c
						CMDBR *pcMdbRB				// (O) �G���e�B�e�B���R�[�h�o�b�t�@
				)
{
	int		nRec;						// �ǂݍ��݃G���e�B�e�B���R�[�h�ʒu
	int		iSzRB;						// �G���e�B�e�B���R�[�h�o�b�t�@�T�C�Y
	int		iSzRec;						// �ǂݍ��݃G���e�B�e�B���R�[�h�o�b�t�@�T�C�Y
	int		iSzD;						// �ǂݍ��݃o�b�t�@�]�T
	MDBR*	MdbRB;						// �G���e�B�e�B���R�[�h���e����

	MdbRB = pcMdbRB->m_pMdbr;
													// �G���e�B�e�B���R�[�h�h�c�ƌ^���`�F�b�N
	if (idRec < 1 || idRec >= m_MaxMdbRId-1 ||
				m_pMdbRId[idRec].Model==0) return F_NOITEM;
													// �G���e�B�e�B���R�[�h�ʒu���`�F�b�N
	nRec = m_pMdbRId[idRec].nRec;
	if (nRec < 0 || nRec >= m_MaxSzMdbBuf) return F_POSERROR;
													// �G���e�B�e�B���R�[�h���̂h�c���`�F�b�N
	if (idRec != ((MDBR *)&m_pMdbBuf[nRec])->idRec) return F_HDRERROR;

	iSzRB = MdbRB->iSzRec;
	iSzRec = ((MDBR *)&m_pMdbBuf[nRec])->iSzRec;
	iSzD = iSzRB - iSzRec;							// �ǂݍ��݃o�b�t�@�]�T
													// �ǂݍ��݃G���e�B�e�B���R�[�h�̎g�p�ς�
	if (iSzD < 0) {									// �]�T�Ȃ��̏ꍇ�ǂݍ��݃o�b�t�@�̃T�C�Y�ύX
		pcMdbRB->FreeBuf();
		pcMdbRB->AsgBuf( iSzRec);
		iSzRB = iSzRec;
	}
													// �ǂݍ��݃G���e�B�e�B���R�[�h�̃R�s�[
	MbCopy( MdbRB->iRec0, (int *)&m_pMdbBuf[nRec], iSzRec);
													// �c����N���A
	if (iSzD > 0) MbFill( &MdbRB->iRec0[iSzRec], 0, iSzD);
													// �ǂݍ��݃o�b�t�@�T�C�Y�̐ݒ�
    MdbRB->iSzRec = iSzRB;
	return 0;
}
/*******************************************************************************

	�G���e�B�e�B���R�[�h���e����

*******************************************************************************/
void CMDB::PrintRec(MCHAR *head, int nRec, int num)
{
	MDBR *pRec = (MDBR *)&m_pMdbBuf[nRec];
	printf("%s <%d>	idRec=%d,	iSzRec=%d,",
		    head, nRec, pRec->idRec, pRec->iSzRec);

	if (pRec->idRec == 0) {							// �󂫃G���A
		printf( "	Before=%d,	Next=%d\n",
			   pRec->nBefore, pRec->nNext);
	} else {										// �G���e�B�e�B���R�[�h
		printf( "	iType=%d,	iSzUse=%d\n",
	           pRec->iType, pRec->iSzUsed);
		if (pRec->iSzRec <= 7) {
			for (int i=0; i<pRec->iSzRec-2; i++)
				printf( "r[%d]=%d, ", i, pRec->iRec[i]);
		} else {
			printf( "	r[0]=%d, r[1]=%d, r[2]=%d,,, r[%d]=%d, r[%d]=%d",
			     pRec->iRec[0], pRec->iRec[1], pRec->iRec[2],
				 num-5, pRec->iRec[num-5], num-4, pRec->iRec[num-4]);
		}
		printf("\n");
	}
}
/*******************************************************************************

	�G���e�B�e�B���R�[�h�h�c���R�[�h����

*******************************************************************************/
void CMDB::PrintID(
						MCHAR *head
				 )
{
	MDBRID *pRID = m_pMdbRId;

	printf( "%s\n",head);
	for (int i=1; i<4; i++) {
		if(pRID[i].Model == 0) {
			printf( "  :%d:	Model=%d,	idBefore=%d,	idNext=%d\n", 
					i, pRID[i].Model, pRID[i].idBefore, pRID[i].idNext);
		} else {
			printf( "  :%d:	Model=%d,	nRec=%d\n", 
					i, pRID[i].Model, pRID[i].nRec);
		}
	}
	i = m_MaxMdbRId - 1;
	if(pRID[i].Model == 0) {
		printf( "  :%d:	Model=%d,	idBefore=%d,	idNext=%d\n", 
				i, pRID[i].Model, pRID[i].idBefore, pRID[i].idNext);
	} else {
		printf( "  :%d:	Model=%d,	nRec=%d\n", 
				i, pRID[i].Model, pRID[i].nRec);
	}

}
/*******************************************************************************

	�l�c�a���e����

*******************************************************************************/
void CMDB::PrintMdb()
{
	int		nRec;
	int		iSz;

	nRec = 0;

	PrintID("RID "); 

	for (int i=1; i<5; i++) {
		iSz = ((MDBR *)&m_pMdbBuf[nRec])->iSzRec;
		PrintRec( "Rec ", nRec, iSz);
		nRec += iSz;
	}
//	nRec = ((MDBR *)&m_pMdbBuf[0])->nBefore;
//	iSz = ((MDBR *)&m_pMdbBuf[nRec])->iSzRec;
//	PrintRec( "Rec ", nRec, iSz);
}
/*******************************************************************************

	�G���e�B�e�B���R�[�h�̍쐬

*******************************************************************************/
CMDBR::CMDBR(
						int iSzRec					// ( I) �G���e�B�e�C���R�[�h�T�C�Y
			)
{
	m_pMdbr = (MDBR*)new int[iSzRec];
			MASTRACE(m_pMdbr,sizeof(MDBR)*iSzRec);
	MbFill( m_pMdbr->iRec0, 0, iSzRec);
	m_pMdbr->iSzRec = iSzRec;
	m_pMdbr->iSzUsed = iSzRecHdr;
}//	CMDBR
CMDBR::~CMDBR()
{
	delete m_pMdbr;
}//	~CMDBR
void	CMDBR::AsgBuf(
					   int iSzRec					// ( I) �G���e�B�e�C���R�[�h�T�C�Y
					 )
{
	m_pMdbr = (MDBR*)new int[iSzRec];
			MASTRACE(m_pMdbr,sizeof(MDBR)*iSzRec);
}
void	CMDBR::FreeBuf()
{
	delete m_pMdbr;
}
/*******************************************************************************

	�G���e�B�e�B���R�[�h�ɑ�����ǉ��܂��͏C������B

	return   F_NOERROR:  ����
  			 F_OVERFLOW: �󂫃G���A�s��

*******************************************************************************/
int CMDBR::InsertItem(
						int idItem,					// ( I) �����h�c
						int* pItem,					// ( I) �����l
						int iSzItem					// ( I) �����T�C�Y
					)
{
	MRINT	iSzRec;						// �G���e�B�e�B���R�[�h�T�C�Y
	MRINT	iSzUse;						// �g�p�ς݃G���e�B�e�B���R�[�h�T�C�Y
	int		iSzUseN;					// �ǉ��C����̃��R�[�h�T�C�Y
	int*	ipRec;						// ���R�[�h�̐擪�ʒu�̃|�C���^
	MRINT	nItem;						// �����p�̑����ʒu
	MDBRI	*pMdbri;					// �����p�̑����|�C���^
	int		iSz;						// �����p�̑����̃T�C�Y
	int		iSzItemC;					// �����h�c�������������̃T�C�Y
	int		imv;						// �ǉ��܂��͏C�����鑮���ɑ�����̑����̃T�C�Y

	iSzItem += iSzItmHdr;							//	�A�C�e���T�C�Y�Ƀw�b�_�[����ǉ�
	iSzRec = m_pMdbr->iSzRec;						//
	iSzUse = m_pMdbr->iSzUsed;						//
	ipRec  = m_pMdbr->iRec0;
													//
													//	���R�[�h���A�C�e��������
	imv = 0;
	iSzItemC = 0;
	for (nItem=iSzRecHdr; nItem<iSzUse;) {
		pMdbri = (MDBRI *)&ipRec[nItem];
		iSz = pMdbri->iSzItem;
		if (idItem <= pMdbri->idItem) {
			if (idItem == pMdbri->idItem) iSzItemC = iSz;
			imv = iSzUse - (nItem + iSzItemC);
			break;
		}
		nItem += iSz;
	}
													//
													// �����R�s�[
	iSzUseN = iSzUse - iSzItemC + iSzItem;
//	�ǉ��C����̎g�p���R�[�h�T�C�Y(�w�b�_�T�C�Y����)�����R�[�h�T�C�Y�@�|���@�I�[�o�t���[
	if (iSzUseN > iSzRec) {
		return F_OVERFLOW;
	}
													// �ǉ��C�������̌��̑������ړ���
													// �ǉ��C�������̃T�C�Y�̋󂫃G���A���쐬
	if (imv != 0) 
		MbCopy( &ipRec[nItem+iSzItem], &ipRec[nItem+iSzItemC], imv);
													// �����̐ݒ�
	pMdbri = (MDBRI *)&ipRec[nItem];
	pMdbri->idItem = idItem;
	pMdbri->iSzItem = iSzItem;
	MbCopy((int *)&ipRec[nItem+iSzItmHdr], (int*)pItem, iSzItem-iSzItmHdr);
	m_pMdbr->iSzUsed = iSzUseN;
	return F_NOERROR;
}// InsertItem
/*******************************************************************************

	�G���e�B�e�B���R�[�h���瑮���A�C�e�����폜����B

	return   F_NOERROR: ����
  			 F_NOITEM:  �����Ȃ�

*******************************************************************************/
int CMDBR::DeleteItem(
						int idItem					// ( I) �����h�c
					)
{
	MRINT	iSzUse;						// �g�p���R�[�h�T�C�Y
	int*	ipRec;						// ���R�[�h�̐擪�ʒu�̃|�C���^�@ 
	MRINT	nItem;						// �����p�̑����ʒu
	MDBRI	*pMdbri;					// �����p�̑����|�C���^
	int		imv;						// �����폜�ɂ��f�[�^�ړ��T�C�Y
	int		iSz;						// �����p�̑����̃T�C�Y
//
	iSzUse = m_pMdbr->iSzUsed;
	ipRec  = m_pMdbr->iRec0;
//
//	���R�[�h���A�C�e��������
//
	for (nItem=iSzRecHdr; nItem<iSzUse;) {
		pMdbri = (MDBRI *)&ipRec[nItem];
		iSz = pMdbri->iSzItem;
		if (idItem <= pMdbri->idItem) {
			if (idItem == pMdbri->idItem) {
				imv = iSzUse - (nItem + iSz);
				MbCopy( &ipRec[nItem], &ipRec[nItem+iSz], imv);
				MbFill( &ipRec[iSzUse-iSz], 0, iSz);
				m_pMdbr->iSzUsed = iSzUse - iSz;
				return F_NOERROR;
			} else {
				return F_NOITEM;
			}
		}
		nItem += iSz;
	}
	return F_NOITEM;
}// InsertItem
/*******************************************************************************

	�G���e�B�e�B���R�[�h���瑮���𓾂�B

	return  >0:         �����T�C�Y�i����j
	  		F_NOITEM:   �����Ȃ�
			F_OVERFLOW: �I�[�o�t���[   ?????

*******************************************************************************/
int	CMDBR::GetItem(
						int idItem,					// ( I) �����h�c
						int* pItem,					// (O) �����l
						int iSzItem					// ( I) �����T�C�Y
				 )
{
	MRINT	iSzUse;
	int*	ipRec;
	MRINT	nItem;
	MDBRI	*pMdbri;
	int		iSz;
	
	iSzUse = m_pMdbr->iSzUsed;
	ipRec  = m_pMdbr->iRec0;
//
//	���R�[�h���A�C�e��������
	for (nItem=iSzRecHdr; nItem<iSzUse;) {
		pMdbri = (MDBRI *)&ipRec[nItem];
		iSz = pMdbri->iSzItem;
		if (idItem <= pMdbri->idItem) {
			if (idItem == pMdbri->idItem) {
				if (iSz-iSzItmHdr > iSzItem) return F_OVERFLOW;
				MbCopy( (int *)pItem, (int*)&ipRec[nItem+iSzItmHdr], iSz);
				return iSz-iSzItmHdr;
			}
			break;
		}
		nItem += iSz;
	}
	return F_NOITEM;
}// InsertItem
void CMDBR::PrintRec()
{
	printf("idRec=%d,iSzRec=%d,iType=%d,iSzUse=%d\n",
		m_pMdbr->idRec, m_pMdbr->iSzRec, m_pMdbr->iType,
		m_pMdbr->iSzUsed);
	for (int i=0; i<m_pMdbr->iSzRec-2; i++) {
		printf("iRec[%d]=%08x,", i, m_pMdbr->iRec[i]);
		if (i%4 == 3) printf("\n");
	}
	printf("\n");
}// PrintRec
/*******************************************************************************

	�󂫃G���e�B�e�B���R�[�h�h�c�����o��
			�擪�󂫃G���e�B�e�B���R�[�h�h�c�����o������擪�ɂ���

	return  �G���e�B�e�B���R�[�h�h�c
			F_NOERROR:  ����
			F_OVERFLOW: �󂫃G���e�B�e�B���R�[�h�h�c�Ȃ�

*******************************************************************************/
int	CMDB::GetEmptyRecID()
{
	int idFirst;						// �擪�󂫃G���e�B�e�B���R�[�h�h�c
	int idNext;							// ���̋󂫃G���e�B�e�B���R�[�h�h�c

	if (m_id1stEmptyR == 0) return F_OVERFLOW;
													// �擪�̋󂫃G���e�B�e�B���R�[�h�h�c��Ԃ�
													// ���G���e�B�e�B���R�[�h�h�c���R�[�h�̌���������N���N���A
	idFirst = m_id1stEmptyR;
//	idNext = ((MDBRID *)&m_pMdbRId[idFirst])->idNext;
//	((MDBRID *)&m_pMdbRId[idFirst])->idNext = F_NOLINKI;
	idNext = m_pMdbRId[idFirst].idNext;
	m_pMdbRId[idFirst].idNext = F_NOLINKI;
													// ���̋󂫃G���e�B�e�B���R�[�h�h�c��擪�Ƃ���
													// ���G���e�B�e�B���R�[�h�h�c���R�[�h�̑O���������N�������N�Ȃ��ɂ���
//	((MDBRID *)&m_pMdbRId[idNext])->idBefore = F_NOLINKI;
	m_pMdbRId[idNext].idBefore = F_NOLINKI;
	m_id1stEmptyR = idNext;
	
	return idFirst;
}
/*******************************************************************************

	�g�p�ς݃G���e�B�e�B���R�[�h�h�c���󂫂Ƃ���
	�i�G���e�B�e�B���R�[�h�h�c���󂫃G���e�B�e�B���R�[�h�h�c�Ƃ���j

*******************************************************************************/
void CMDB::PutEmptyRecID(
						  int idRec					// (I  ) �G���e�B�e�B���R�[�h�h�c
						)
{
													// �G���e�B�e�B���R�[�h�h�c���R�[�h�̋󂫃t���O(iType=0)�̐ݒ��
													// �O���������N�������N�Ȃ��ɂ���
	m_pMdbRId[idRec].Model = 0;
	m_pMdbRId[idRec].idBefore = F_NOLINKI;
													// �擪�󂫃G���e�B�e�B���R�[�h�h�c���R�[�h�̑O�Ƀ����N����
	m_pMdbRId[idRec].idNext = m_id1stEmptyR;
	m_pMdbRId[m_id1stEmptyR].idBefore = idRec;
	m_id1stEmptyR = idRec;
}

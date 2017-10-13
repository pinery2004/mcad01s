/*******************************************************************************
										Created on 06/15/02 By K.Matsubayashi
										Copyright 2002 K.Matsubayashi
	データベースモジュール

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

	MDBの使用開始

*******************************************************************************/
CMDB::CMDB(
			int iMaxSzMdbBuf,			// MDBの領域サイズ
			int iMaxMdbRId,				// 最大レコードID (1～iMaxMdbRId)
			int iMaxSzMdbR,				// 最大レコードサイズ
			int iMinSzMdbR				// 最小レコードサイズ
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
	
													// MDBの全クリアした領域を用意
	m_pMdbBuf = new int[m_MaxSzMdbBuf];
			MASTRACE( m_pMdbBuf, sizeof(int)*m_MaxSzMdbBuf);
	MbFill( m_pMdbBuf, 0, m_MaxSzMdbBuf);
													// インデックスレコードを設定
	m_pMdbRId = new MDBRID[iMaxMdbRId];
			MASTRACE( m_pMdbRId, sizeof(MDBRID)*iMaxMdbRId);
													// 空きインデックスレコードのリンクを設定
	for (n=1; n<m_MaxMdbRId;n++) {
		m_pMdbRId[n].Model = 0;
		m_pMdbRId[n].idBefore = n-1;
		m_pMdbRId[n].idNext = n+1;
	}
	m_pMdbRId[1].idBefore = F_NOLINKI;
	m_pMdbRId[m_MaxMdbRId-1].idNext = F_NOLINKI;
													// 空きインデックスレコードの先頭ＩＤを設定
	m_id1stEmptyR = 1;
													// 空きエリアのリンク設定
	m_n1stEmptyR = 0;

	nRec = 0;
	nRecB = F_NOLINKP;
													// 空きレコードへのポインタレコードを設定
	while (nRec < m_MaxSzMdbBuf) {
		pMdbR = (MDBR *)&m_pMdbBuf[nRec];
													// 空きエリアレコードのレコードサイズに最大レコードサイズを設定
													// 最大レコードサイズ割り当て後の残りのサイズが最小レコードサイズ
													// 以下の場合は、残り全てを空きエリアレコードのレコードサイズに設定
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
//	ＭＤＢの使用終了
//
CMDB::~CMDB(){
	delete	m_pMdbBuf;
	delete	m_pMdbRId;
}
//
//		ＭＤＢの使用開始宣言を行う。
//	Bool	OpenDB(HMDB hMdb, LPCSTR pszFileName, UINT nOpenFlags, 
//                                  CFileException *pError = NULL);
//		ＭＤＢの使用終了宣言を行う。	
//	void Close(HMDB hMdb, UINT nCloseFlags);

/*******************************************************************************

	エンティティレコードの空き領域を得る
  
	return			>=0:エンティティレコード位置
	  				F_OVERFLOW: 空き領域不足

*******************************************************************************/
int CMDB::GetEmptyArea(
						int iSzRec					// (I  ) エンティティレコードサイズ
					 )
{
	int 	nCur;						// 検索空きエリア位置
	MDBR 	*pCur;						// 　同ポインタ
	int 	nNext;						// 空き領域から必要領域取得後の残りの空きエリアの位置
	MDBR 	*pNext;						// 　同ポインタ
	int		iSzNext;					// 　同領域サイズ
//
	nCur = m_n1stEmptyR;
	pCur = (MDBR *)&m_pMdbBuf[nCur];
													// レコードが入る空き領域を検索	
	for (;;) {
		iSzNext = pCur->iSzRec - iSzRec;
		if (iSzNext >= 0) {							// エンティティレコードサイズを満足する空き領域あり
			break;
		}
		if (pCur->nNext == F_NOLINKP) {						// 同空き領域なし
			return F_OVERFLOW;
		}
		nCur = pCur->nNext;
		pCur = (MDBR *)&m_pMdbBuf[nCur];
//		pCur = (MDBR *)&m_pMdbBuf[pCur->nNext];
	}
//
													// 空き領域より必要なサイズの領域を取得後の残りと
													// 最小空き領域サイズと比較する
	if (iSzNext < m_MinSzMdbR) {
													// 最小空き領域サイズより小さい場合は分割せずに
													// 空き領域として返す、前後の空き領域のリンクを設定
		((MDBR *)&m_pMdbBuf[pCur->nNext])->nBefore = pCur->nBefore; 
		if (pCur->nBefore == F_NOLINKP) {
			m_n1stEmptyR = pCur->nNext;
		} else {
			((MDBR *)&m_pMdbBuf[pCur->nBefore])->nNext = pCur->nNext;
		}
	} else {
													// 最小空き領域サイズ以上の場合は分割する
													// 空き領域取得後の前後の空き領域のリンクを設定
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
													// 得られた空きエリアのサイズを設定
		pCur->iSzRec = iSzRec;
	}
//													// 得られた空き領域の内容(リンク)をクリア
	pCur->nNext = 0;
	pCur->nBefore = 0;
	return nCur;
}
/*******************************************************************************

	エンティティレコードを追加する

	return
			>=1: エンティティレコードＩＤ
	  		F_OVERFLOW: 空きエリア不足
			F_OVERFLOW: 空きエンティティレコードＩＤなし

*******************************************************************************/
int CMDB::InsertRec(
						CMDBR *cMdbR,				// (I  ) エンティティレコード
						int Model					// (I  ) エンティティレコードの型コード
					)
{
	int		iSzRec;						// 追加レコードサイズ
	MRID	idEmptyRec;					// 空き領域ＩＤ
	int		iSzEmptyRec;				// 空き領域レコードサイズ
	int		nEmptyRec;					// 空き領域位置
	MDBR*	MdbR;

	MdbR = cMdbR->m_pMdbr;
													// 空き領域と空き領域のＩＤを得る
	iSzRec =  MdbR->iSzRec;
	nEmptyRec = GetEmptyArea( iSzRec);
	if (nEmptyRec < 0) return nEmptyRec;
	idEmptyRec = GetEmptyRecID();
	if (idEmptyRec <= 0) return idEmptyRec;
													// 空き領域にレコード内容とＩＤを書き込む
	iSzEmptyRec = ((MDBR *)&m_pMdbBuf[nEmptyRec])->iSzRec;
	MbCopy((int *)&m_pMdbBuf[nEmptyRec],(int *)MdbR, iSzRec);
	((MDBR *)&m_pMdbBuf[nEmptyRec])->idRec = idEmptyRec;
	((MDBR *)&m_pMdbBuf[nEmptyRec])->iSzRec = iSzEmptyRec;
													// エンティティレコードＩＤレコードに
													// タイプコードとエンティティレコード位置を設定
	((MDBRID *)&m_pMdbRId[idEmptyRec])->Model = Model;
	((MDBRID *)&m_pMdbRId[idEmptyRec])->nRec = nEmptyRec;
	return idEmptyRec;
} // InsertRec
/*******************************************************************************

	使用済みエンティティレコードエリアを開放し空きとする

*******************************************************************************/
void CMDB::PutEmptyArea(
						int nRec					// (I  ) エンティティレコードの位置
					  )
{
	int		icngflg;					// 結合フラグ
	MDBR	*pCur;						// 返却エンティティレコードへのポインタ
	MRINT	iSzCur;						// 　同サイズ
	int		nNext;						// 返却エンティティレコードに連続するエンティティレコード位置
	MDBR	*pNext;						// 　同ポインタ
	MRID	idNext;						// 　同ＩＤ
	int		iSzNext;					// 　同サイズ
	int		n1stEmptyR;					// 返却前の先頭空きエンティティレコード位置

	pCur = (MDBR *)&m_pMdbBuf[nRec];
	iSzCur  = pCur->iSzRec;
													// 返却エンティティレコードを空きエンティティレコードの
													// 先頭にリンクする
	n1stEmptyR = m_n1stEmptyR;
	((MDBR *)&m_pMdbBuf[n1stEmptyR])->nBefore = nRec;
	m_n1stEmptyR = nRec;
													// 返却エンティティレコードＩＤ = 0(空き)
	pCur->idRec = 0;
													// 返却エンティティレコード項目エリアをクリアする
	MbFill((int *)&m_pMdbBuf[nRec+iSzRecHdr], 0, iSzCur-iSzRecHdr);
	pCur->nBefore = F_NOLINKP;
	pCur->nNext = n1stEmptyR;
													// 返却エンティティレコードに連続するエリアが空きエリアであり、かつ、
													// 両エリアの合計サイズが最大エンティティレコードサイズより小さいなら
													// 連結して1つの空きエリアとする
	do {
													// 連続するエリアを得る
		nNext = nRec + iSzCur;
		pNext = (MDBR *)&m_pMdbBuf[nNext];
		idNext = pNext->idRec;
		iSzNext = pNext->iSzRec;
    
		if ((idNext == F_NOLINKI) && (iSzCur + iSzNext <= m_MaxSzMdbR)) {
													// 連続するエリアと連結する
			iSzCur += iSzNext;
			pCur->iSzRec = iSzCur;
													// 連続するエリアへのポインタをスキップする
			if (pNext->nNext != F_NOLINKP) 
				((MDBR *)&m_pMdbBuf[pNext->nNext])->nBefore = pNext->nBefore;
			if (pNext->nBefore != F_NOLINKP) 
				((MDBR *)&m_pMdbBuf[pNext->nBefore])->nNext = pNext->nNext;
													// 結合するエリアのヘッダー部とリンク部をクリア
			pNext->nBefore = 0;
			pNext->nNext = 0;
			MbFill((int *)pNext, 0, iSzRecHdr);
													// 結合フラグを立てる
			icngflg = 1;
		} else {
													// 結合フラグを消す
			icngflg = 0;
		}
	} while (icngflg == 1);							// 結合フラグが立っている間繰り返す
	
}
/*******************************************************************************

	エンティティレコードを削除する。

	return	0:正常
			F_NOITEM   :エンティティレコードＩＤの誤り
	  		F_POSERROR :エンティティレコードの位置誤り
			F_HDRERROR :エンティティレコードのヘッダー形式誤り

*******************************************************************************/
int CMDB::DeleteRec(
						int idRec					// （I) エンティティレコードＩＤ
					)
{
	int		nRec;						// 削除レコード位置
													// エンティティレコードＩＤのチェック
	if (idRec < 1 || idRec >= m_MaxMdbRId-1 ||
				m_pMdbRId[idRec].Model==0) return F_NOITEM;
													// 削除レコード位置とＩＤのチェック
	nRec   = m_pMdbRId[idRec].nRec;
	if (nRec < 0 || nRec >= m_MaxSzMdbBuf) return F_POSERROR;
	if (idRec != ((MDBR *)&m_pMdbBuf[nRec])->idRec) return F_HDRERROR;
													// 空きレコードにする
	PutEmptyArea(nRec);
													// 空きレコードＩＤにする
	PutEmptyRecID(idRec);
	return 0;
}
/*******************************************************************************

	エンティティレコードを修正する。

	return	0:正常
			F_NOITEM   :エンティティレコードＩＤの誤り
	  		F_POSERROR :エンティティレコードの位置誤り
			F_HDRERROR :エンティティレコードのヘッダー形式誤り

*******************************************************************************/
int CMDB::ModifyRec(
						int idRec,					// ( I) 修正エンティティレコードＩＤ
						CMDBR *pcMdbR,				// ( I) 修正後エンティティレコード内容
						int Model					// ( I)	修正後エンティティレコードの型コード
					)
{
	int		nRec;						// 修正エンティティレコード位置
	MRINT	iSzRec;						// 修正前エンティティレコードサイズ
	MRINT	iSzin;						// 修正後エンティティレコードサイズ
	MDBR*	MdbR;						// 修正後エンティティレコード内容実体

	MdbR = pcMdbR->m_pMdbr;
													// 修正エンティティレコードＩＤと型をチェック
	if (idRec < 1 || idRec >= m_MaxMdbRId-1 ||
				m_pMdbRId[idRec].Model==0) return F_NOITEM;
													// 修正エンティティレコード位置をチェック
	nRec = m_pMdbRId[idRec].nRec;
	if (nRec < 0 || nRec >= m_MaxSzMdbBuf) return F_POSERROR;
													// 修正前エンティティレコード内のＩＤをチェック
	if (idRec != ((MDBR *)&m_pMdbBuf[nRec])->idRec) return F_HDRERROR;
	
	iSzin = MdbR->iSzRec;
	iSzRec = ((MDBR *)&m_pMdbBuf[nRec])->iSzRec;
													// 修正前より修正後のエンティティレコードサイズが大の場合
													// レコードエリアを開放し再度確保
	if (iSzin <= iSzRec-m_MinSzMdbR || iSzin > iSzRec) {  
		PutEmptyArea( nRec);
		nRec = GetEmptyArea( iSzin);
		iSzRec = ((MDBR *)&m_pMdbBuf[nRec])->iSzRec;
	}
													// エンティティレコード内容のコピー
	MbCopy( (int *)&m_pMdbBuf[nRec],(int *)MdbR, iSzin);
	((MDBR *)&m_pMdbBuf[nRec])->idRec = idRec;
	((MDBR *)&m_pMdbBuf[nRec])->iSzRec = iSzRec;
													// ＩＤレコードに型とエンティティレコード位置を設定
	((MDBRID *)&m_pMdbRId[idRec])->Model = Model;
	((MDBRID *)&m_pMdbRId[idRec])->nRec = nRec;
	return 0;
}
/*
													// 空き領域にレコード内容とＩＤを書き込む
	iSzEmptyRec = ((MDBR *)&m_pMdbBuf[nEmptyRec])->iSzRec;
	MbCopy((int *)&m_pMdbBuf[nEmptyRec],(int *)MdbR, iSzRec);
	((MDBR *)&m_pMdbBuf[nEmptyRec])->idRec = idEmptyRec;
	((MDBR *)&m_pMdbBuf[nEmptyRec])->iSzRec = iSzEmptyRec;
													// エンティティレコードＩＤレコードに
													// タイプコードとエンティティレコード位置を設定
	((MDBRID *)&m_pMdbRId[idEmptyRec])->Model = Model;
	((MDBRID *)&m_pMdbRId[idEmptyRec])->nRec = nEmptyRec;

*/
/*******************************************************************************

	エンティティレコードを得る。

	return 0:正常
			F_NOITEM(-1)  :エンティティレコードＩＤの誤り
	  		F_POSERROR(-2):エンティティレコードの位置誤り
			F_HDRERROR(-3):エンティティレコードのヘッダー形式誤り

*******************************************************************************/
int CMDB::GetRec(
						int idRec,					// ( I) エンティティレコードＩＤ
						CMDBR *pcMdbRB				// (O) エンティティレコードバッファ
				)
{
	int		nRec;						// 読み込みエンティティレコード位置
	int		iSzRB;						// エンティティレコードバッファサイズ
	int		iSzRec;						// 読み込みエンティティレコードバッファサイズ
	int		iSzD;						// 読み込みバッファ余裕
	MDBR*	MdbRB;						// エンティティレコード内容実体

	MdbRB = pcMdbRB->m_pMdbr;
													// エンティティレコードＩＤと型をチェック
	if (idRec < 1 || idRec >= m_MaxMdbRId-1 ||
				m_pMdbRId[idRec].Model==0) return F_NOITEM;
													// エンティティレコード位置をチェック
	nRec = m_pMdbRId[idRec].nRec;
	if (nRec < 0 || nRec >= m_MaxSzMdbBuf) return F_POSERROR;
													// エンティティレコード内のＩＤをチェック
	if (idRec != ((MDBR *)&m_pMdbBuf[nRec])->idRec) return F_HDRERROR;

	iSzRB = MdbRB->iSzRec;
	iSzRec = ((MDBR *)&m_pMdbBuf[nRec])->iSzRec;
	iSzD = iSzRB - iSzRec;							// 読み込みバッファ余裕
													// 読み込みエンティティレコードの使用済み
	if (iSzD < 0) {									// 余裕なしの場合読み込みバッファのサイズ変更
		pcMdbRB->FreeBuf();
		pcMdbRB->AsgBuf( iSzRec);
		iSzRB = iSzRec;
	}
													// 読み込みエンティティレコードのコピー
	MbCopy( MdbRB->iRec0, (int *)&m_pMdbBuf[nRec], iSzRec);
													// 残りをクリア
	if (iSzD > 0) MbFill( &MdbRB->iRec0[iSzRec], 0, iSzD);
													// 読み込みバッファサイズの設定
    MdbRB->iSzRec = iSzRB;
	return 0;
}
/*******************************************************************************

	エンティティレコード内容を印書

*******************************************************************************/
void CMDB::PrintRec(MCHAR *head, int nRec, int num)
{
	MDBR *pRec = (MDBR *)&m_pMdbBuf[nRec];
	printf("%s <%d>	idRec=%d,	iSzRec=%d,",
		    head, nRec, pRec->idRec, pRec->iSzRec);

	if (pRec->idRec == 0) {							// 空きエリア
		printf( "	Before=%d,	Next=%d\n",
			   pRec->nBefore, pRec->nNext);
	} else {										// エンティティレコード
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

	エンティティレコードＩＤレコードを印書

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

	ＭＤＢ内容を印書

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

	エンティティレコードの作成

*******************************************************************************/
CMDBR::CMDBR(
						int iSzRec					// ( I) エンティテイレコードサイズ
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
					   int iSzRec					// ( I) エンティテイレコードサイズ
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

	エンティティレコードに属性を追加または修正する。

	return   F_NOERROR:  正常
  			 F_OVERFLOW: 空きエリア不足

*******************************************************************************/
int CMDBR::InsertItem(
						int idItem,					// ( I) 属性ＩＤ
						int* pItem,					// ( I) 属性値
						int iSzItem					// ( I) 属性サイズ
					)
{
	MRINT	iSzRec;						// エンティティレコードサイズ
	MRINT	iSzUse;						// 使用済みエンティティレコードサイズ
	int		iSzUseN;					// 追加修正後のレコードサイズ
	int*	ipRec;						// レコードの先頭位置のポインタ
	MRINT	nItem;						// 検索用の属性位置
	MDBRI	*pMdbri;					// 検索用の属性ポインタ
	int		iSz;						// 検索用の属性のサイズ
	int		iSzItemC;					// 属性ＩＤが等しい属性のサイズ
	int		imv;						// 追加または修正する属性に続く後の属性のサイズ

	iSzItem += iSzItmHdr;							//	アイテムサイズにヘッダー部を追加
	iSzRec = m_pMdbr->iSzRec;						//
	iSzUse = m_pMdbr->iSzUsed;						//
	ipRec  = m_pMdbr->iRec0;
													//
													//	レコード内アイテムを検索
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
													// 属性コピー
	iSzUseN = iSzUse - iSzItemC + iSzItem;
//	追加修正後の使用レコードサイズ(ヘッダサイズ込み)＞レコードサイズ　－＞　オーバフロー
	if (iSzUseN > iSzRec) {
		return F_OVERFLOW;
	}
													// 追加修正属性の後ろの属性を移動し
													// 追加修正属性のサイズの空きエリアを作成
	if (imv != 0) 
		MbCopy( &ipRec[nItem+iSzItem], &ipRec[nItem+iSzItemC], imv);
													// 属性の設定
	pMdbri = (MDBRI *)&ipRec[nItem];
	pMdbri->idItem = idItem;
	pMdbri->iSzItem = iSzItem;
	MbCopy((int *)&ipRec[nItem+iSzItmHdr], (int*)pItem, iSzItem-iSzItmHdr);
	m_pMdbr->iSzUsed = iSzUseN;
	return F_NOERROR;
}// InsertItem
/*******************************************************************************

	エンティティレコードから属性アイテムを削除する。

	return   F_NOERROR: 正常
  			 F_NOITEM:  属性なし

*******************************************************************************/
int CMDBR::DeleteItem(
						int idItem					// ( I) 属性ＩＤ
					)
{
	MRINT	iSzUse;						// 使用レコードサイズ
	int*	ipRec;						// レコードの先頭位置のポインタ　 
	MRINT	nItem;						// 検索用の属性位置
	MDBRI	*pMdbri;					// 検索用の属性ポインタ
	int		imv;						// 属性削除によるデータ移動サイズ
	int		iSz;						// 検索用の属性のサイズ
//
	iSzUse = m_pMdbr->iSzUsed;
	ipRec  = m_pMdbr->iRec0;
//
//	レコード内アイテムを検索
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

	エンティティレコードから属性を得る。

	return  >0:         属性サイズ（正常）
	  		F_NOITEM:   属性なし
			F_OVERFLOW: オーバフロー   ?????

*******************************************************************************/
int	CMDBR::GetItem(
						int idItem,					// ( I) 属性ＩＤ
						int* pItem,					// (O) 属性値
						int iSzItem					// ( I) 属性サイズ
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
//	レコード内アイテムを検索
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

	空きエンティティレコードＩＤを取り出す
			先頭空きエンティティレコードＩＤを取り出し次を先頭にする

	return  エンティティレコードＩＤ
			F_NOERROR:  正常
			F_OVERFLOW: 空きエンティティレコードＩＤなし

*******************************************************************************/
int	CMDB::GetEmptyRecID()
{
	int idFirst;						// 先頭空きエンティティレコードＩＤ
	int idNext;							// 次の空きエンティティレコードＩＤ

	if (m_id1stEmptyR == 0) return F_OVERFLOW;
													// 先頭の空きエンティティレコードＩＤを返す
													// 同エンティティレコードＩＤレコードの後方向リンクをクリア
	idFirst = m_id1stEmptyR;
//	idNext = ((MDBRID *)&m_pMdbRId[idFirst])->idNext;
//	((MDBRID *)&m_pMdbRId[idFirst])->idNext = F_NOLINKI;
	idNext = m_pMdbRId[idFirst].idNext;
	m_pMdbRId[idFirst].idNext = F_NOLINKI;
													// 次の空きエンティティレコードＩＤを先頭とする
													// 同エンティティレコードＩＤレコードの前方向リンクをリンクなしにする
//	((MDBRID *)&m_pMdbRId[idNext])->idBefore = F_NOLINKI;
	m_pMdbRId[idNext].idBefore = F_NOLINKI;
	m_id1stEmptyR = idNext;
	
	return idFirst;
}
/*******************************************************************************

	使用済みエンティティレコードＩＤを空きとする
	（エンティティレコードＩＤを空きエンティティレコードＩＤとする）

*******************************************************************************/
void CMDB::PutEmptyRecID(
						  int idRec					// (I  ) エンティティレコードＩＤ
						)
{
													// エンティティレコードＩＤレコードの空きフラグ(iType=0)の設定と
													// 前方向リンクをリンクなしにする
	m_pMdbRId[idRec].Model = 0;
	m_pMdbRId[idRec].idBefore = F_NOLINKI;
													// 先頭空きエンティティレコードＩＤレコードの前にリンクする
	m_pMdbRId[idRec].idNext = m_id1stEmptyR;
	m_pMdbRId[m_id1stEmptyR].idBefore = idRec;
	m_id1stEmptyR = idRec;
}

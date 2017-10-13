#pragma once

/*******************************************************************************
										Created on 06/15/02 By K.Matsubayashi
										Copyright 2002 K.Matsubayashi
	データベースモジュール

	MDB.h

*******************************************************************************/
//
/*
#define	MaxSzMdbBuf		2000000				// ＭＤＢバッファサイズ
#define MaxMdbRecId		30000				// エンティティレコードのＩＤ最大値
#define	MaxSzMdbRec		62383				// エンティティレコード最大サイズ
#define MinszMdbRec		10					// エンティティレコード最小サイズ
*/
// 制限値
#define	MaxSzMdbBuf		2000000				// ＭＤＢバッファサイズ
#define MaxMdbRecId		30					// エンティティレコードのＩＤ最大値
#define	MaxSzMdbRec		21					// エンティティレコード最大サイズ
#define MinSzMdbRec		8					// エンティティレコード最小サイズ
// ステイタス
#define	FEMPTY			0					// 空き
#define F_OVERFLOW		-101				// オーバーフローエラーステイタス
#define F_NOERROR		0					// 正常
#define F_NOITEM		-1					// 属性なし　ワーニング
#define F_IDERROR		-201				// エンティティレコードＩＤエラー
#define F_POSERROR		-202				// エンティティレコード位置エラー
#define F_HDRERROR		-203				// エンティティレコードヘッダ形式エラー
// フラグ
#define F_NOLINKP		-1					// レコード位置のリンクなし(先頭または最終リンク)
#define F_NOLINKI		0					// ＩＤのリンクなし(先頭または最終リンク)

typedef	unsigned short	MRINT;				// レコードＩＤ型
typedef			 int	MRID;				// レコードＩＤ型

namespace MC
{

/*******************************************************************************
// 
//	エンティティ・レコード
//
*******************************************************************************/
typedef struct {
	union {
		int		iRec0[1];					// エンティティレコードクリア用
		struct {
			MRID	idRec;					// エンティティレコードＩＤ(0:空き)
			MRINT	iSzBuf;					// エンティティバッファサイズ(ヘッダ部込みサイズ)
			MRINT	iSzRec;					// エンティティレコードサイズ(ヘッダ部込みサイズ)
		};
	};
	union {
		int		iRec1[1];
		//	内容がデータレコードの場合
		struct {
			MRINT	iType;					// エンティティレコードタイプ
			MRINT	iSzUsed;				// エンティティレコード使用済サイズ(ヘッダ部込みサイズ)
			int		iRec[1];
		};									// エンティティレコード・バッファ
		//	内容が空きエリアの場合
		struct	{
			int		nBefore;				// 前の空きエンティティレコードへのポインタ
											//  (＝0:先頭）
			int		nNext;					// 次の空きエンティティレコードへのポインタ
											//  (＝0:最終）
		};
	};
} MDBR;

/*******************************************************************************

	エンティティ・レコード

	項目処理

*******************************************************************************/
class CMDBR
{
public:
	MDBR*	m_pMdbr;						// ＭＤＢレコード
public:
	CMDBR( int iSzRec);
	CMDBR()							{};
	~CMDBR();
	void	AsgBuf( int iSzRec);
	void	FreeBuf();
	int		GetSzRec()				{return m_pMdbr->iSzRec;}
	int		GetSzUsed()				{return m_pMdbr->iSzUsed;}
	int		GetType()				{return m_pMdbr->iType;}
	int		GetId()					{return m_pMdbr->idRec;}
	int		GetBefore()				{return m_pMdbr->nBefore;}
	int		GetNext()				{return m_pMdbr->nNext;}
	int*	GetBufP()				{return m_pMdbr->iRec;}

	void	PutSzRec( int iSz)		{m_pMdbr->iSzRec = iSz;}
	void	PutSzUsed( int iSz)		{m_pMdbr->iSzUsed = iSz;}
	void	PutType( int it)			{m_pMdbr->iType = it;}
	void	PutId( int id)			{m_pMdbr->idRec = id;}
	void	PutBefore( int n)		{m_pMdbr->nBefore = n;}
	void	PutNext( int n)			{m_pMdbr->nNext = n;}

	int		InsertItem( int idItem, int* pItem, int iSzItem);
	int		DeleteItem( int idItem);
	int		GetItem   ( int idItem, int* pItem, int iSzItem);
	void	PrintRec();
};// CMDBR
/*******************************************************************************

	エンティティ・レコードＩＤレコード

*******************************************************************************/
typedef struct {
	int	Model;								// エンティティレコードの型
											//	(＝0:空きレコード、≠0:使用済みレコード)
	union { 
											// 使用済みレコードの場合
		int	nRec;							// 　エンティティレコードの位置
//
		struct {							// 空きレコードの場合
			MRID idBefore;					// 　前の空きエンティティレコードＩＤ
											//  (＝0:先頭）
			MRID idNext;					// 　次の空きエンティティレコードＩＤ
											//  (＝0:最終）
		};
	};
} MDBRID;
/*******************************************************************************

	属性（ヘッダ+内容）

*******************************************************************************/
typedef struct {
	MRINT	idItem;							// アイテムＩＤ
	MRINT	iSzItem;						// アイテムサイズ
	int		iItem[1];						// アイテム内容
} MDBRI;
/*******************************************************************************

	ＭＤＢ　レコード処理

*******************************************************************************/
class CMDB
{
	MCHAR*	m_sFileName;					// ＭＤＢのファイル名
	int		*m_pMdbBuf;						// ＭＤＢバッファ[MaxSzMdbBuf]
	int		m_iSzMdbBuf;					// ＭＤＢのサイズ
	int		m_update;						// ＭＤＢの修正フラグ

	MDBRID	*m_pMdbRId;						// エンティティレコードＩＤレコードポインタ
	MRID	m_id1stEmptyR;					// 先頭空きエンティティレコードＩＤ
	int		m_n1stEmptyR;					// 先頭空きエンティティレコード位置
//
	int		m_MaxSzMdbBuf;					// ＭＤＢバッファサイズ
	int		m_MaxMdbRId;					// エンティティレコードＩＤ最大値
	int		m_MaxSzMdbR;					// エンティティ・レコード最大サイズ
	int		m_MinSzMdbR;					// エンティティ・レコード最小サイズ
//	int		m_iSzEmptyRec;					// ダミー空きエンティティレコードサイズ
public:
	CMDB	( int iMaxSzMdbBuf, int iMaxMdbRId, int iMaxSzMdbR,
			 int iMinSzMdbR);
//			 , int iiSzEmptyRec);
	~CMDB();
//	Bool	OpenDB(HMDB hMdb, LPCSTR pszFileName, UINT nOpenFlags, 
//                                  CFileException *pError = NULL);
//	void	Close(HMDB hMdb, UINT nCloseFlags);
	int		InsertRec(CMDBR *pMdbR, int iType);
	int		GetRec   ( int idR, CMDBR *pMdbR);
	int		ModifyRec( int idR, CMDBR *pMdbR, int iType);
	int		DeleteRec( int idR);

	int		GetEmptyArea ( int iSzRec);
	int		GetEmptyRecID();
	void	PutEmptyArea ( int nRec);
	void	PutEmptyRecID( int idRec);
	void	PrintRec(MCHAR* head, int nRec, int num);
	void	PrintID (MCHAR* head);
	void	PrintMdb();
};// CMDB

} // namespace MC
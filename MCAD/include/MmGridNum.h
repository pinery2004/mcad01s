#pragma once
/////////////////////////////////////////////////////////////////////////////
//  ウィンドウ管理

#define			MGRID_GRP_MAX		10

#ifdef DLL_EXPORT
	#undef DLL_EXPORT
#endif

#ifdef DLL_EXPORT_GRIDNUM_DO
	#pragma message( "<<< GridNum・dll_EXport >>>")
	#define DLL_EXPORT							__declspec( dllexport)
#else
	#ifdef _DEBUG								// Release Mode で inline関数がundefになる不具合対応
		#ifdef DLL_NO_IMPORT_MD_DO
			#define DLL_EXPORT
		#else
//			#pragma message( "=== GridNum・dll_IMport ===")
			#define DLL_EXPORT						__declspec( dllimport)
		#endif
	#else
		#define DLL_EXPORT
	#endif
#endif

namespace MC
{

//class	DLL_EXPORT	MmWndInfo;
class	MmWndInfo;
/////////////////////////////////////////////////////////////////////////////
//	MmGridNum.h 

class DLL_EXPORT MmGridNum
{
public :

	bool			m_bFlgDrawG;				// グリッド表示フラグ
	MgPoint2		m_PtOriginG;				// グリッド原点座標
	MINT			m_iMinG[2];					// 縦(0)横(1)のグリッド最小位置
	MINT			m_nGrpG[2];				// 縦(0)横(1)のグリッドグループ数
	MINT			m_nGrid[2][MGRID_GRP_MAX];	// 縦(0)横(1)のグリッド数
	MREAL			m_PichG[2][MGRID_GRP_MAX];	// 縦(0)横(1)のグリッド間隔（ピッチ）
	MgMinMaxR2		m_rMinMaxG;					// グリッド領域の最小最大
	
	// コンストラクタ
	MmGridNum();
	
	// デストラクタ
	~MmGridNum()	{};

	// 属性読み書き
	void	SetDrawDispFlgG( bool bFlgDrawG)					// グリッド表示フラグの設定
						{ m_bFlgDrawG = bFlgDrawG;}
	bool	GetDrawFlgG()										// グリッド表示フラグを取得
						{ return m_bFlgDrawG;}
	void	SetOriginPtG( MgPoint2 PtOriginG)					// グリッド原点座標の設定
						{ m_PtOriginG = PtOriginG;}
	MgPoint2 GetOriginPtG()										// グリッド原点座標を取得
						{ return m_PtOriginG;}
	void	SetMinG( MINT*	iMinG)								// 縦横のグリッド最小位置の設定
						{ m_iMinG[0] = iMinG[0];
						  m_iMinG[0] = iMinG[0];}
	void	GetMinG( MINT*	iMinG)								// 縦横のグリッド最小位置を取得
						{ iMinG[0] = m_iMinG[0];
						  iMinG[0] = m_iMinG[0];}
						
	void 	SetGridNum( MINT* nGrid);							// グリッド数（北、西、南、東）

	void 	GetGridNum( MINT* nGrid);							// グリッド数（北、西、南、東）

	void	SetNumG( MINT nGrid[2][MGRID_GRP_MAX]);				// 縦横のグリッド数の設定

	void	GetNumG( MINT nGrid[2][MGRID_GRP_MAX]);				// 縦横のグリッド数を取得

	void	SetPichG( MREAL PichG[2][MGRID_GRP_MAX]);			// 縦横のグリッド間隔の設定

	void	GetPichG( MREAL PichG[2][MGRID_GRP_MAX]);			// 縦横のグリッド間隔を取得
	
	void	SetMinMaxG( MgMinMaxR2& rMinMaxG)					// グリッド領域の最小最大の設定
						{ m_rMinMaxG = rMinMaxG;} 
	MgMinMaxR2 GetMinMaxG()										// グリッド領域の最小最大を取得
						{ return m_rMinMaxG;}
	MgMinMaxR2 GetMinMaxGA(
						MbCod*		pCod
				);

	void	DrawGrid(											// グリッドを表示する
						 MmWndInfo*	pWndInfo,
						 MbCod*		pCod
				);

	////void	Marume(												// 座標値を丸める
	////			const	MgPoint2	&Pi,						// 入力座標
	////					MgPoint2	*pPo						// 丸め後の座標
	////			);

	void	GetMarumeGridLine(									// 最寄のグリッド丸め線を取得する
						MINT		iCdMarume,					// グリッド丸め分割値
				const	MgPoint2	&Pi,      					// 入力座標
						MgLine2		*pLn						// 縦横の最寄のグリッド丸め線分[2本]
				);

//	friend	bool operator == ( const MmWndInfo &W1, const MmWndInfo &W2);	// b = (W1 == W2) 判定
};

} // namespace MC
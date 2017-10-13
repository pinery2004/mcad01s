// ChildView5.cpp : 実装ファイル
//

#include "stdafx.h"
#include "MCAD.h"
#include "ChildView5.h"

#include "MgMat.h"
#include "MbCod.h"
#include "MmGridNum.h"
#include "MmDrag.h"
#include "MmWnd.h"

#include "MmDefine.h"
#include "MmLib.h"

// CChildView5

IMPLEMENT_DYNCREATE(CChildView5, CWrapperView)					// PrintPreview Modify

CChildView5::CChildView5()
{
}

CChildView5::~CChildView5()
{
}

BEGIN_MESSAGE_MAP(CChildView5, CWnd)
	ON_WM_PAINT()
//	ON_WM_CLOSE()
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CChildView5 メッセージ ハンドラ

BOOL CChildView5::PreCreateWindow(CREATESTRUCT& cs)
{
	MINT	ist;
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS,
		NULL,reinterpret_cast<HBRUSH>(COLOR_WINDOW+1),NULL); 

	MC::WindowCtrl::MmWndSetWndC( 5, this);										// ウィンドウ管理
	ist = MC::WindowCtrl::MmWndKAddC();											// ウィンドウ管理
	_ASSERTE(ist == 0);											// ウィンドウ管理

	return TRUE;
}

void CChildView5::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO : ここにメッセージ ハンドラ コードを追加します。
	MC::MmWndInfo* pWndInfo = MC::WindowCtrl::MmWndKFindWnd( this);

	if (pWndInfo == NULL)										// PrintPreview画面を直接closeした時、View Close後に
																// Preview Closeし、ここに入ってくるため判定を追加
		return;													// Debug用
	else														// Debug用
		(pWndInfo->GetFrame())->SetWindowText(Mstr( "WINDOW_05"));

//C	OnDraw( &dc);												// PrintPreview

	// 描画メッセージで CWnd::OnPaint() を呼び出さないでください。

//C	-------------------------------------------------------------------------------------------------

//	MmWndKInitMouse();

	//	マップモードを設定
	MINT iOldMM = pWndInfo->SetMapMode( NULL);

	// メモリーＤＣの内容を画面に表示
	MC::WindowCtrl::MmWndKDraw( pWndInfo);

	// クロスヘアーカーソル表示
//C	Window::ReDrawCrossHair();

	//	ラバーバンド図形を再表示する
//C	Window::ReDrawDragging();
	
	//	マップモードを戻す
	pWndInfo->m_pMemDC->SetMapMode( iOldMM);

	// メッセージの描画のために CWnd::OnPaint() を呼び出さないでください。
}
/*
void CChildView5::OnClose()
{
	// TODO : ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	MmWndInfo*	pWndInfo = MmWndKFind( this);					// 管理情報の開放
	MmWndKDelete( pWndInfo);

	CWnd::OnClose();
}
*/
void CChildView5::OnDraw(CDC* pDC)								// PrintPreview
{																// PrintPreview
	// PreviusPrint 表示用　ＤＣ，ウインドウエリア設定
	CRect client1;												// PrintPreview
	MC::MbCod cod1;

	cod1.SetDC( pDC);
	GetClientRect(&client1);									// PrintPreview

	MINT iDleft = client1.left;
	MINT iDtop  = client1.top;
	MINT iDwidth = client1.right - client1.left;
	MINT iDhight = client1.bottom - client1.top;

	cod1.SetWinD( iDleft, iDtop, iDwidth, iDhight);

	MC::MmWndInfo* pWndInfo = MC::WindowCtrl::MmWndKFindWnd( this);

	MC::WindowCtrl::MmDrawStructure( pWndInfo, &cod1);
}

void CChildView5::OnFilePrint()									// PrintPreview
{																// PrintPreview
	// TODO : ここにコマンド ハンドラ コードを追加します。		// PrintPreview
	CView::OnFilePrint();										// PrintPreview
}																// PrintPreview

int CChildView5::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWrapperView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO :  ここに特定な作成コードを追加してください。

	return 0;
}

void CChildView5::OnSize(UINT nType, int cx, int cy)
{
	CWrapperView::OnSize(nType, cx, cy);

	// TODO : ここにメッセージ ハンドラ コードを追加します。
	MINT	iOldMM;

	if (cx > 0 && cy > 0) {
		CPoint	ptMouthD( cx, cy);								// 画面サイズ(表示座標)
		CPoint	ptMouthL = ptMouthD;

		CClientDC dc( this);

		iOldMM = dc.SetMapMode( MMAPMODE);
		dc.DPtoLP( &ptMouthL);									// 画面サイズ(論理座標)
		dc.SetMapMode( iOldMM);

																// グリッド表示数取り込み
		MC::MmWndInfo* pWndInfo1 = MC::WindowCtrl::MmWndKFindSyu( 1, MTHEIMENZU, 1, 1);
		MC::MmWndInfo* pWndInfo5 = MC::WindowCtrl::MmWndKFindWnd( this);
		if (pWndInfo5 == NULL)
			return;
		MINT nGrid[4];
		pWndInfo1->GetGridNum( nGrid);
		pWndInfo5->SetGridNum( nGrid);
		pWndInfo5->SetMinMaxRS( pWndInfo5->GetMinMaxGA( pWndInfo5));	//	実座標(Min/Max)
//		pWndInfo5->SetMat();							// 座標変換用マトリックスの設定				//ZZ? なぜ無いの 070901
		
		MC::ViewInp::Size( this, ptMouthD, ptMouthL);					// ウィンドウサイズ設定
	}
}


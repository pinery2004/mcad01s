////////////////////////////////////////////////////////////////////////////
// ChildView2.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "MCAD.h"

#include "MgMat.h"
#include "MbCod.h"
#include "MmGridNum.h"
#include "MmDrag.h"
#include "MmWnd.h"

#include	< GL/gl.h >
#include	< GL/glu.h >
#include	"GlLib.h"

#include "ChildView2.h"

#define DEBUG1 0

namespace MC
{

//S void MhGetBCtr(	MREAL *rB, MC::MgPoint3 *PtCtr);

void ModelDraw( void);

void MsSetLight_MaterialParameter1();

void SetLightModel1();
void SetLight();
void DrawDisplayList( int iWire);

void ShowError( int e);

} // namespace MC

// CChildView2

static const MCHAR* pErrorStrings[] = {
    {Mstr( "No Error")},                   // 0
    {Mstr( "Unable to get a CD")},         // 1
    {Mstr( "ChoosePixelFormat failed")},   // 2
    {Mstr( "SelectPixelFormat failed")},   // 3
    {Mstr( "wglCreateContext failed")},    // 4
    {Mstr( "wglCreateCurrect failed")},    // 5
    {Mstr( "wglDeleteCotext failed")},     // 6
    {Mstr( "SwapBuffer failed")},          // 7
};

IMPLEMENT_DYNAMIC( CChildView2, CWnd)

CChildView2::CChildView2() : m_pDC( 0)
{
}

CChildView2::~CChildView2()
{
}

BEGIN_MESSAGE_MAP( CChildView2, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
//	ON_WM_CLOSE()
END_MESSAGE_MAP()

// CChildView2 ���b�Z�[�W �n���h��

BOOL CChildView2::PreCreateWindow( CREATESTRUCT& cs)
{
	MINT	ist;
	if ( !CWnd::PreCreateWindow( cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor( NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);


	MC::WindowCtrl::MmWndSetWndC( 2, this);										// �E�B���h�E�Ǘ�
	ist = MC::WindowCtrl::MmWndKAddC();											// �E�B���h�E�Ǘ�
	_ASSERTE( ist == 0);														// �E�B���h�E�Ǘ�

	return TRUE;
}

void CChildView2::OnPaint()
{
	CPaintDC dc(this); // �`��̃f�o�C�X �R���e�L�X�g
	
	SetWindowPos(&wndTopMost , 0, 0, 0, 0,
		SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE | SWP_NOZORDER);

//	MC::MmWndInfo* pWndInfo = MC::WindowCtrl::MmWndKFind( this);					// �E�B���h�E�Ǘ�
	MC::MmWndInfo* pWndInfo = MC::WindowCtrl::MmWndKFindSyu( 2, MTHEIMENZU, 1, 1);	// �E�B���h�E�Ǘ�
	(pWndInfo->GetFrame())->SetWindowText(Mstr( "WINDOW_02"));	// �E�B���h�E�Ǘ�

	// �`�惁�b�Z�[�W�� CWnd::OnPaint() ���Ăяo���Ȃ��ł��������B

#if( DEBUG1)
	GlLib::DrawGLobject();
	SwapBuffers( m_pDC->m_hDC);    // Double buffer
#else

	m_M3View.OnPaint();
	if( FALSE == SwapBuffers( m_pDC->GetSafeHdc())) MC::ShowError(7);	// Orig
//	SwapBuffers( m_pDC->m_hDC);    // Double buffer
#endif
}

BOOL CChildView2::Create( LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
						  DWORD dwStyle, const RECT& rect,
						  CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// Setup the shared menu

	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

////////////////////////////////////////////////////////////////////////////

int CChildView2::OnCreate( LPCREATESTRUCT lpCreateStruct)
{
//	MREAL		rB;								// �\���{��
	MC::MgPoint3	PtCtr;						// ���S���W

	if (CWnd::OnCreate( lpCreateStruct) == -1)
		return -1;

	//	DC�̐���
	m_pDC = new CClientDC( this);
	if( NULL == m_pDC ){
		MC::ShowError( 1);
		return FALSE;
	}
	m_M3View.OnCreate( m_pDC->m_hDC);

#if( DEBUG1)
	GlLib::SetDCPixelFormat( m_pDC->m_hDC);						// OpenGL�p��Pixel Format���w��
	m_GLRC = wglCreateContext( m_pDC->m_hDC);					// Rendering context�̐���
	wglMakeCurrent( m_pDC->m_hDC, m_GLRC);						// ���݂�context�ݒ�

	GlLib::InitializeOpenGL();									//3D�V�[��������������֐���p�ӂ���
	GlLib::MakeGLObject();										//3D�I�u�W�F�N�g�𐶐�����
#endif

	return 0;
}

//BOOL CChildView2::SetDCPixelFormat( HDC hdc)

// OnCreate ����ŏ��Ɏ��s�����@�C�j�V�����C�Y����
//BOOL CChildView2::InitializeOpenGL( void)

////////////////////////////////////////////////////////////////////////////

void CChildView2::OnDestroy()
{
	CWnd::OnDestroy();

	// �����_�����O �R���e�L�X�g�ƃf�o�C�X �R���e�L�X�g�̉��
	m_M3View.OnDestroy();

	if( m_pDC ) delete m_pDC;
}

BOOL CChildView2::OnEraseBkgnd( CDC* pDC)
{
//	return CWnd::OnEraseBkgnd(pDC);
	return TRUE;
}

//void MsOnSize( UINT nType, int cx, int cy);

void CChildView2::OnSize( UINT nType, int cx, int cy)
{
	CWnd::OnSize( nType, cx, cy);

																				//	MsOnSize( nType, cx, cy);//
#if( DEBUG1)
	GlLib::ViewSetting(cx, cy);
#endif

	m_M3View.OnSize( nType, cx, cy);


}

void CChildView2::OnLButtonDown( UINT nFlags, CPoint point)
{
	m_iPtLast = point;
	SetCapture();

	CWnd::OnLButtonDown( nFlags, point);
}

void CChildView2::OnLButtonUp( UINT nFlags, CPoint point)
{
	ReleaseCapture();

	CWnd::OnLButtonUp(nFlags, point);
}

void CChildView2::OnMouseMove( UINT nFlags, CPoint point)
{
	CPoint	iPtD;												// �}�E�X�ړ���	
	CRect	iRcWin;												// �E�B���h�E�̈� (�`��̈�)
	CPoint	iSzWin;												// �E�B���h�E�̕��ƍ���
	MC::MgVect3	PtD( 0., 0., 0.);
	MC::MgVect3	RtD( 0., 0., 0.);
	MREAL	SclD;

	if (!(nFlags & MK_LBUTTON) && !( nFlags & MK_RBUTTON))
		MQUIT;													// �}�E�X�̍��܂��͉E�{�^����������Ă��Ȃ��ꍇ�͖���

	GetClientRect( iRcWin);
	
	iPtD.x = ( point.x - m_iPtLast.x) * 2;						// �}�E�X�ړ���	
//S	iPtD.y = ( m_iPtLast.y - point.y) * 2;						// Y�͏オ�������ł���(��: 0:�� �`�@1024:��)
	iPtD.y = MMDIRY * (LONG)( m_iPtLast.y - point.y) * 2;				// Y�͏オ�������ł���(��: 1024:�� �`�@0:��)

	if ( ( MGABS( iPtD.x) < 5) && ( MGABS( iPtD.y) < 5)) 
		MQUIT;													// ����ړ��ʖ����̏ꍇ�͖���

	iSzWin = CPoint( iRcWin.right - iRcWin.left,
					 iRcWin.bottom - iRcWin.top);  

	//	Ctrl + Left Button:   ���E�ړ��A�㉺�ړ�
	if ( nFlags & MK_CONTROL && nFlags & MK_LBUTTON) {
		PtD.x = (MREAL)iPtD.x / (MREAL)iSzWin.x;
		PtD.y = (MREAL)iPtD.y / (MREAL)iSzWin.y;

	//	Ctrl + Right Button:   ____�A�O��ړ�
	} else if ( nFlags & MK_CONTROL && nFlags & MK_RBUTTON) {
		PtD.z = (MREAL)iPtD.y / (MREAL)iSzWin.y;

	//	Left Button:   �x����]�A�w����]
	} else 	if ( nFlags & MK_LBUTTON) {
		RtD.x = (MREAL)iPtD.x / (MREAL)iSzWin.x;
		RtD.y = - (MREAL)iPtD.y / (MREAL)iSzWin.y;

		RtD.z = - ((MREAL)iPtD.y * ((MREAL)point.x / (MREAL)iSzWin.x * 2 - 1) +		// �O���̕��̂w����]�Ƃx����]�͂y����]
				(MREAL)iPtD.x * ((MREAL)point.y / (MREAL)iSzWin.y * 2 -1)) /		// �ɂ��e�����y�ڂ��l�ɂ���
				(MREAL)iSzWin.x;;													// ���� (������:�y���ɑ΂��ĉE��])

	//	Right Button:   �y��]�A�g��k��
	} else if ( nFlags & MK_RBUTTON) {
		RtD.z = (MREAL)iPtD.x / (MREAL)iSzWin.x;
		SclD  = (MREAL)iPtD.y / (MREAL)iSzWin.y;

		if (point.y < (iRcWin.top + iRcWin.bottom)/2)								// �y����]�p�@��ʒ������
			RtD.z = - RtD.z;														// ��� (������:�y���ɑ΂��č���]) �ŁA
																					//			   (�y�����猩�ĉE��])
																					// ���� (������:�y���ɑ΂��ĉE��])
																					//			   (�y�����猩�č���])
	}
	m_M3View.OnMouseMove( nFlags, PtD, RtD, SclD);

	m_iPtLast = point;

	PostMessage(WM_PAINT);										// �ĕ\���̃��b�Z�[�W�𑗂菈���̏I����҂����Ɏ��ֈڂ�
exit:
	CWnd::OnMouseMove(nFlags, point);
}

void CChildView2::OnRButtonDown(UINT nFlags, CPoint point)
{
	m_iPtLast = point;
	SetCapture();

	CWnd::OnRButtonDown(nFlags, point);
}

void CChildView2::OnRButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();

	CWnd::OnRButtonUp(nFlags, point);
}

void CChildView2::OnMButtonDown(UINT nFlags, CPoint point)
{
	m_M3View.m_iFWire++;
	if (m_M3View.m_iFWire >= 5 || (nFlags != 24 && m_M3View.m_iFWire >= 3))
		m_M3View.m_iFWire = 0;
	PostMessage(WM_PAINT);										// �ĕ\���̃��b�Z�[�W�𑗂菈���̏I����҂����Ɏ��ֈڂ�

	CWnd::OnMButtonDown(nFlags, point);
}

void CChildView2::OnMButtonUp(UINT nFlags, CPoint point)
{
	CWnd::OnMButtonUp(nFlags, point);
}
/*
void CChildView2::OnClose()
{
	MC::MmWndInfo*	pWndInfo = MC::WindowCtrl::MmWndKFind( this);					// �Ǘ����̊J��
	MC::WindowCtrl::MmWndKDelete( pWndInfo);

	CWnd::OnClose();
}
*/
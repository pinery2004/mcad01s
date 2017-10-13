////////////////////////////////////////////////////////////////////////////
// ChildView2.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "MCAD.h"
#include "ChildView2.h"
#include "MmWnd.h"


#include	< GL/gl.h >
#include	< GL/glu.h >
#include	"GlLib.h"

#include "MgLib.h"
#include "MnLib.h"
#include "MdDispList.h"
#define DLL_EXPORT_DISPLIST_DO
#include "MnMdlDispList.h"


namespace MC
{

//S void MhGetBCtr(	MREAL *rB, MgPoint3 *PtCtr);
//S int DispList::DrawIeMdl( MREAL rB, const MgPoint3 &PtCtr);

void ModelDraw( void);
//void Cube( GLfloat lenght);

//void MsSetLight_MaterialParameter1();

void SetLightModel1();
void SetLight();
void DrawDisplayList( int iWire);
//void SetMaterial1();
//void SetMaterial2();


////////////////////////////////////////////////////////////////////////////
//�y�@�\�z�`��
BOOL MsRenderScene1(
						int			i_iFWire,	// �\���^�C�v�@0: �ʕ\���A1:�������A2:���ނ̂݁A3:���C���t���[���\��
						MREAL		i_rScl,		// �g�嗦
						MREAL		i_rRotX,	// ��]�p	V3( ���C���C��)
						MREAL		i_rRotY,	// ��]�p	V3( ���C���C��)
						MREAL		i_rRotZ,	// ��]�p	V3( ���C���C��)
						MREAL		i_rMovX,	// �ړ���	V3( ���C���C��)
						MREAL		i_rMovY,	// �ړ���	V3( ���C���C��)
						MREAL		i_rMovZ		// �ړ���	V3( ���C���C��)
				);

// CChildView2

static const MCHAR* ErrorStrings[] = {
    {Mstr( "No Error")},                   // 0
    {Mstr( "Unable to get a CD")},         // 1
    {Mstr( "ChoosePixelFormat failed")},   // 2
    {Mstr( "SelectPixelFormat failed")},   // 3
    {Mstr( "wglCreateContext failed")},    // 4
    {Mstr( "wglCreateCurrect failed")},    // 5
    {Mstr( "wglDeleteCotext failed")},     // 6
    {Mstr( "SwapBuffer failed")},          // 7
};

//void CChildView2::ShowError(int e){
void ShowError(int e){
    AfxMessageBox(ErrorStrings[e]);
}

// CChildView2 ���b�Z�[�W �n���h��

M3View::M3View() : m_hRC(0)
{
}
void M3View::OnPaint()
{
#if(1)		// 11/10/
	MsRenderScene1( m_iFWire, m_rScl, m_rRotX, m_rRotY, m_rRotZ,
								    m_rMovX, m_rMovY, m_rMovZ);
#else
//	GlLib::DrawGLobject();
	GlLib::DrawGLobject1( m_iFWire, m_rScl, m_rRotX, m_rRotY, m_rRotZ,
								    m_rMovX, m_rMovY, m_rMovZ);
#endif
}

////////////////////////////////////////////////////////////////////////////

int M3View::OnCreate( HDC hdc)
{
	MREAL		rB;								// �\���{��
	MgPoint3	PtCtr;							// ���S���W


	//	OpenGL�p��Pixcel Format���w��
    if( !SetDCPixelFormat( hdc) ) return FALSE;

     //	�^����ꂽDC�Ɨ����\��Rendering context�̐��� 
	if( 0 == (m_hRC = wglCreateContext( hdc))){
        ShowError(4);
        return FALSE;
    }

    // Rendering context�����݂�context�Ƃ��Đݒ�
	if( FALSE == wglMakeCurrent( hdc, m_hRC)){
        ShowError(5);
        return FALSE;
    }
    
    // 3D�V�[����������
    InitializeOpenGL();
    
	// 3D�I�u�W�F�N�g�𐶐�(Displaylist�쐬)
	MdlDispList::MhGetBCtr( &rB, &PtCtr);									// �R�����f�B�X�v���C���X�g�쐬�p�̕\���{���ƒ��S���W�����߂�
	
	MdlDispList::DrawIeMdl( rB, PtCtr);

	return 0;
}

//BOOL CChildView2::SetDCPixelFormat( HDC hdc)
BOOL M3View::SetDCPixelFormat( HDC hdc)
{
	//�@�E�B���h�E�̃s�N�Z���t�H�[�}�b�g�̐ݒ� 
    PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),							// Specifies the size of this data structure
		1,														// Specifies the version of this data structure
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,		// �s�N�Z���o�b�t�@�̃r�b�g�t���O�̐ݒ�
		PFD_TYPE_RGBA,											// RGBA pixel values
		24,														// (or 32) 24-bit�J���[�Ǝw��
		0, 0, 0, 0, 0, 0,										// Specifies the number of red bitplanes in each RGBA color buffer
		0,														// Specifies the number of alpha bitplanes in each RGBA color buffer
		0,
		0,														// Specifies the total number of bitplanes in the accumulation buffer
		0,  0, 0, 0,
		32,														// Specifies the depth(bit) of the depth (z-axis) buffer
		0,														// (or 32) Specifies the depth of the stencil buffer
		0,														// Specifies the number of auxiliary buffers
		PFD_MAIN_PLANE,											// Layer type�@Ignored...
		0,														// Specifies the number of overlay and underlay planes
		0, 0, 0													// Ignored, Specifies the transparent color or index of an underlay plane, Ignored

    };
	//�v�����ꂽ�t�H�[�}�b�g�ɍł��߂��s�N�Z���t�H�[�}�b�g�̃C���f�b�N�X��v�� 
	int pixelformat;
	if( 0 == (pixelformat = ChoosePixelFormat( hdc, &pfd))){
		ShowError(2);
		return FALSE;
	}
	//�s�N�Z���t�H�[�}�b�g�̐ݒ� 
	if( FALSE == SetPixelFormat( hdc, pixelformat, &pfd)){
		ShowError(3);
		return FALSE;
	}
//	if (0 == DescribePixelFormat(hdc, nPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd)) {
//		ShowError(3);
//		return FALSE ;
//	}
	return TRUE;
}

void MsGlInitialize();

// OnCreate ����ŏ��Ɏ��s�����@�C�j�V�����C�Y����
BOOL M3View::InitializeOpenGL( void)
{
	// �ʕ\���ɐݒ�
	m_iFWire = 0;

	// �o�b�t�@������
	DispList::MsGlInitialize();

	// Lighting��L����
	
	// Light model,Light,Material �p�����[�^�̍쐬1	
	DispList::MsSetLight_MaterialParameter1();									

	// ��]�p�������ݒ�
	m_rRotX = -50.;
	m_rRotY = -32.;
	m_rRotZ = -22.;

	// �ړ��ʂ������ݒ�
	m_rMovX = 0.;
	m_rMovY = 0.;
	m_rMovZ = 0.;

	// ���W�ϊ��}�g���b�N�X�̏�����
	m_rScl = 1.;
	m_mTrans = MGeo::Mat3RotRevZ( m_rRotZ) * MGeo::Mat3RotRevY( m_rRotY) * MGeo::Mat3RotRevX( m_rRotX);

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////

void M3View::OnDestroy()
{
	// �����_�����O �R���e�L�X�g�ƃf�o�C�X �R���e�L�X�g�̉��
	if( FALSE == ::wglMakeCurrent( 0, 0)) ShowError(2);
	if( FALSE == ::wglDeleteContext( m_hRC )) ShowError(6);
}

//void MsOnSize(UINT nType, int cx, int cy);

void M3View::OnSize(UINT nType, int cx, int cy)
{
	DispList::MsOnSize( nType, cx, cy);
}

void M3View::OnMouseMove(
						MINT		nFlags,		// 1: �ړ�
						MgPoint3	PtD,
						MgPoint3	RtD,
						MREAL		SclD		// �g��k����
				)
{
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	CPoint	iPtD;												// �}�E�X�ړ���	
	MgVect3	rdMov1;
	MgVect3	rdMov2;
	MgMat3	mTr1, mTr2, mTr3, mTrW, mTrW1, mTrW2;				// ���W�ϊ��}�g���b�N�X (work)
	MgVect2	vRx, vRy, vRz;										// �w����]�p, �x����]�p, �y����]�p

	if ( nFlags & MK_CONTROL) {
		rdMov1 = 2.67f * PtD;

		rdMov2 = rdMov1 / m_rScl * m_mTrans;

		m_rMovX += rdMov2.x;
		m_rMovY += rdMov2.y;
		m_rMovZ += rdMov2.z;
		
	} else if ( nFlags & MK_LBUTTON) {
		vRx = MgVect2( 1., - RtD.y * 3).Unitize();				// �w����]�p�@(������:	�w���ɑ΂��č���])
		mTr1 = MGeo::Mat3RotRevXV( vRx);							//					   (�w�����猩�ĉE��])
		mTrW1 = mTr1 * m_mTrans;

		vRy = MgVect2( 1., RtD.x * 3).Unitize();				// �x����]�p�@(������:	�x���ɑ΂��ĉE��])
		mTr2 = MGeo::Mat3RotRevYV( vRy);							//					   (�x�����猩�č���])
		mTrW2 = mTr2 * mTrW1;

		vRz = MgVect2( 1., RtD.z * 3).Unitize();				// ���� (������:�y���ɑ΂��ĉE��])
		mTr3 = MGeo::Mat3RotRevZV( vRz);							//			   (�y�����猩�č���])
		mTrW = mTr3 * mTrW2;
		m_mTrans = mTrW;

	} else if ( nFlags & MK_RBUTTON) {
		m_rScl *= (1 + SclD);									// �g��k��
																// �y����]�p�@��ʒ������
																// ��� (������:�y���ɑ΂��č���]) �ŁA
																//			   (�y�����猩�ĉE��])
		vRz = MgVect2( 1., RtD.z * 3).Unitize();				// ���� (������:�y���ɑ΂��ĉE��])
																//			   (�y�����猩�č���])
		mTr1 = MGeo::Mat3RotRevZV( vRz);
		mTrW = mTr1 * m_mTrans;
		m_mTrans = mTrW;
	}
	m_rRotX = - (MREAL)atan2( -m_mTrans.m[2][1], m_mTrans.m[2][2]) * MC_RTD;
	m_rRotY = - (MREAL)asin( m_mTrans.m[2][0]) * MC_RTD;	
	m_rRotZ = - (MREAL)atan2( -m_mTrans.m[1][0], m_mTrans.m[0][0]) * MC_RTD;
}

} // namespace MC
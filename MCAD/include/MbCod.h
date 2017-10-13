#pragma once
//==========================================================================================
//  Copyright ( C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MbCod.h
//
//		���W�n ���W�ϊ��A�}�`�A�����\��
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
//
#include "MgMat.h"
#include "MgGeo.h"
#include "MbDefine.h"

#define		MT_SZMINMAXRS	2					// �����W( Min/Max)�X�^�b�N�T�C�Y
#define		SZFONT			32					// �t�H���g�ő啶����

#define		MT_LEFT			1
#define		MT_CENTER		2
#define		MT_RIGHT		3
#define		MT_UPPER		1
#define		MT_LOWER		3

#define		MT_FIXEDSIZE	TRUE
#define		MT_FREESIZE		FALSE

#ifdef DLL_EXPORT
	#undef DLL_EXPORT
#endif

#ifdef DLL_EXPORT_COD_DO
	#define DLL_EXPORT							__declspec( dllexport)
#else
//	#define DLL_EXPORT							__declspec( dllimport)
	#define DLL_EXPORT
#endif

namespace MC
{

/////////////////////////////////////////////////////////////////////////////
//	���W�n ���W�ϊ��A�}�`�A�����\��
//
class DLL_EXPORT MbCod
{
protected:
	MgMinMaxI2		m_iMinMaxD;					// �\�����W( Min/Max)
	MgMinMaxI2		m_iMinMaxL;					// �_�����W( Min/Max)
												// ***** ���� *****
												// �{�V�X�e�������ł͑S�Ă�MAPMODE��Y�����̏�̍��W>���̍��W�ɕϊ�
												// ( ��@MM_TEXT�̏ꍇ��Y���W�~( -1))�����l���g�p���Ă���
												// �]���āA�}�`��`���ɂ́A�_��Y���W�~m_iUPY�@�̒l���g�p���鎖

	MINT			m_iUpY;						// �I���W�i���_�����W��Y����( 1:��>��, -1:��<��)				
	MgMinMaxR2		m_rMinMaxRI;				// �����W( Min/Max)�����l
	MgMinMaxR2		m_rMinMaxRS[MT_SZMINMAXRS];	// �����W
	CPoint			m_ptCurMouseL;				// �J�����g�̃}�E�X�ʒu�i�_�����W�j
	MgMat3			m_mat;						// ���W�ϊ��}�g���b�N�X
	MREAL			m_sclRPtoLP;				// RPtoLP�̍��W�ϊ��{��
	MgVect2			m_vsclLPtoDP;				// LPtoDP�̍��W�ϊ��{��( x,y)
	CDC*			m_pDC;						// �\���@�f�o�C�X�R���e�L�X�g
	MINT			m_iMapMode;					// �}�b�v���[�h
//	��
public:
static	MINT		z_iLineStyle;				// ���X�^�C��
static	MREAL		z_fLineWidth;				// ����					( �P�ʁ@MT_FIXEDSIZE:�|�C���g, MT_FREESIZE:mm)
static	DWORD		z_rgbLine;					// ���F( RGB)
static	bool		z_bFixedLineWidth;			// �\�������Œ�t���O
//	������
static	MCHAR		z_sTextFont[SZFONT];		// �t�H���g
static	MREAL		z_fTextHeight;				// ��������				( �P�ʁ@MT_FIXEDSIZE:�|�C���g, MT_FREESIZE:mm)
static	MINT		z_iTextPosUL;				// ������㉺������ʒu( TA_TOP( 1:��,2:��),TA_BASELINE( 3:��))
static	MINT		z_iTextPosLR;				// �����񍶉E������ʒu( TA_LEFT( 1:��),TA_CENTER( 2:��),TA_RIGHT( 3:�E))
static	DWORD		z_rgbText;					// �����F( RGB)
static	MgVect2		z_vTextDirect;				// ������\������		�P�ʃx�N�g��
static	MgPoint2	z_pTextOffset;				// �����ʒu�I�t�Z�b�g	( �P�ʁ@MT_FIXEDSIZE:�|�C���g, MT_FREESIZE:mm)
static	bool		z_bFixedTextSize;			// �\�������T�C�Y�Œ�t���O
//
public:
	MbCod( );
/////////////////////////////////////////////////////////////////////////////
//  �}�b�v���[�h��ݒ肷��
	MINT	SetMapMode( MINT nMapMode);

/////////////////////////////////////////////////////////////////////////////
//  �}�b�v���[�h�𓾂�
	MINT	GetMapMode( )
						{ return m_iMapMode;}

/////////////////////////////////////////////////////////////////////////////
//  �ݒ肵���}�b�v���[�h��Y�����̍��W�l�̑傫��������ł��邩�𒲂ׂ�
	bool	ChkMapModeY( )
						{ return ( m_iUpY > 0);}

/////////////////////////////////////////////////////////////////////////////
//  �\���@�f�o�C�X�R���e�L�X�g��ݒ肷��
	void	SetDC( CDC *pDC)
						{ m_pDC = pDC;}

/////////////////////////////////////////////////////////////////////////////
//  �\���@�f�o�C�X�R���e�L�X�g�𓾂�
	CDC*	GetDC( )
						{ return m_pDC;}

/////////////////////////////////////////////////////////////////////////////
//  �\�����W�g��ݒ肷��( �ݒ莞�ɁAY���W *= ( -1))
	void	SetWinD( 
				MINT	iStX,				// �\�����W�@�n�_( ����)X
				MINT	iStY,				// �\�����W�@�n�_( ����)�n�_Y
				MINT	iWidth,				// �\�����W�@��
				MINT	iHeight				// �\�����W�@����
				)
						{ m_iMinMaxD.min.x = iStX; 
						  m_iMinMaxD.min.y = - ( iStY + iHeight);
						  m_iMinMaxD.max.x = iStX + iWidth;
						  m_iMinMaxD.max.y = - iStY;}
/////////////////////////////////////////////////////////////////////////////
//  �\�����W�g�𓾂�( �ǂݍ��ݎ��ɁAY���W *= ( -1))
	void	GetWinD( 
				MINT	*piStX,				// �\�����W�@�n�_( ����)X
				MINT	*piStY,				// �\�����W�@�n�_( ����)Y
				MINT	*piWidth,			// �\�����W�@��
				MINT	*piHeight			// �\�����W�@����
				)
						{ *piStX = m_iMinMaxD.min.x;
						  *piStY = - m_iMinMaxD.max.y;
						  *piWidth = m_iMinMaxD.max.x - m_iMinMaxD.min.x;
						  *piHeight = m_iMinMaxD.max.y - m_iMinMaxD.min.y;}
/////////////////////////////////////////////////////////////////////////////
//  �_�����W�g��ݒ肷��( �������Y���W���̏ꍇ�͐ݒ莞�ɁAY���W *= ( -1))
	void	SetWinL( 
				MINT	iStX,				// �\�����W�@�n�_( ����)X
				MINT	iStY,				// �\�����W�@�n�_( ����)�n�_Y
				MINT	iWidth,				// �\�����W�@��
				MINT	iHeight				// �\�����W�@����
				);
/////////////////////////////////////////////////////////////////////////////
//  �_�����W�g�𓾂�( �������Y���W���̏ꍇ�͓ǂݍ��ݎ��ɁAY���W *= ( -1))
	void	GetWinL( 
				MINT	*piStX,				// �\�����W�@�n�_( ����)X
				MINT	*piStY,				// �\�����W�@�n�_( ����)Y
				MINT	*piWidth,			// �\�����W�@��
				MINT	*piHeight			// �\�����W�@����
				);
/////////////////////////////////////////////////////////////////////////////
// �I���W�i���_�����W��Y����( 1:��>��, -1:��<��)				
	MINT	GetUpL( )
						{ return m_iUpY;}
/////////////////////////////////////////////////////////////////////////////
//  �\�����WMinMax��ݒ肷��( Y���W *= ( -1))
	void	SetMinMaxD( MgMinMaxI2 iMinMaxD)
						{ m_iMinMaxD = iMinMaxD;}
/////////////////////////////////////////////////////////////////////////////
//  �\�����WMinMax��ݒ肷��
// 070907	void	SetMinMaxD( MINT iMinX, MINT iMinY, MINT iMaxX, MINT iMaxY)
// 070907						{
// 070907						  m_iMinMaxD.min.x = iMinX;
// 070907						  m_iMinMaxD.min.y = iMinY;
// 070907						  m_iMinMaxD.max.x = iMaxX;
// 070907						  m_iMinMaxD.max.y = iMaxY;}
/////////////////////////////////////////////////////////////////////////////
//  �\�����WMinMax�𓾂�
	MgMinMaxI2 GetMinMaxD( )
						{ return m_iMinMaxD;}
/////////////////////////////////////////////////////////////////////////////
//  �\�����WMinMax�𓾂�
// 070907	void	GetMinMaxD( MINT* iMinX, MINT* iMinY, MINT* iMaxX, MINT* iMaxY)
// 070907						{
// 070907						  *iMinX = m_iMinMaxD.min.x;
// 070907						  *iMinY = m_iMinMaxD.min.y;
// 070907						  *iMaxX = m_iMinMaxD.max.x;
// 070907						  *iMaxY = m_iMinMaxD.max.y;}
/////////////////////////////////////////////////////////////////////////////
//  �_�����WMinMax��ݒ肷��
	void	SetMinMaxL( MgMinMaxI2 iMinMaxL)
						{ m_iMinMaxL = iMinMaxL;}
/////////////////////////////////////////////////////////////////////////////
//  �_�����WMinMax��ݒ肷��
// 070907	void	SetMinMaxL( MINT iMinX, MINT iMinY, MINT iMaxX, MINT iMaxY)
// 070907						{
// 070907						  m_iMinMaxL.min.x = iMinX;
// 070907						  m_iMinMaxL.min.y = iMinY;
// 070907						  m_iMinMaxL.max.x = iMaxX;
// 070907						  m_iMinMaxL.max.y = iMaxY;}
/////////////////////////////////////////////////////////////////////////////
//  �_�����WMinMax�𓾂�
	MgMinMaxI2 GetMinMaxL( )
						{ return m_iMinMaxL;}
/////////////////////////////////////////////////////////////////////////////
//  �_�����WMinMax�𓾂�
// 070907	void	GetMinMaxL( MINT* iMinX, MINT* iMinY, MINT* iMaxX, MINT* iMaxY)
// 070907						{
// 070907						  *iMinX = m_iMinMaxL.min.x;
// 070907						  *iMinY = m_iMinMaxL.min.y;
// 070907						  *iMaxX = m_iMinMaxL.max.x;
// 070907						  *iMaxY = m_iMinMaxL.max.y;}
/////////////////////////////////////////////////////////////////////////////
//  �����WMinMax�����l��ݒ肷��
	void	SetMinMaxRI( MgMinMaxR2& rMinMaxRI)
						{ m_rMinMaxRI = rMinMaxRI;}
/////////////////////////////////////////////////////////////////////////////
//  �����WMinMax�����l�𓾂�
	MgMinMaxR2 GetMinMaxRI( )
						{ return m_rMinMaxRI;}
/////////////////////////////////////////////////////////////////////////////
//  �����WMinMax�����l�������WMinMax�ɐݒ肷��
	void	InitMinMaxRS( )
						{ m_rMinMaxRS[0] = m_rMinMaxRS[1] = m_rMinMaxRI;}
/////////////////////////////////////////////////////////////////////////////
//  �J�����g�̎����WMinMax��ݒ肷��
	void	SetMinMaxRS( MgMinMaxR2& rMinMaxRS)
						{ m_rMinMaxRS[0] = rMinMaxRS;}
/////////////////////////////////////////////////////////////////////////////
//  �J�����g�̎����WMinMax�𓾂�
	MgMinMaxR2 GetMinMaxRS( )
						{ return m_rMinMaxRS[0];}
/////////////////////////////////////////////////////////////////////////////
//  �����WMinMax��擪( �J�����g)�ɒǉ�����i���������j
//D	void	PushMinMaxRS( MgMinMaxR2& rMinMaxRS)
//D						{ m_rMinMaxRS[1] = m_rMinMaxRS[0];
//D						  m_rMinMaxRS[0] = rMinMaxRS;}
/////////////////////////////////////////////////////////////////////////////
//  �擪( �J�����g)�̎����WMinMax�����o���i�o�n�o�j
//D	MgMinMaxR2 PopMinMaxRS( )
//D						{ MgMinMaxR2 rMinMax;
//D						  rMinMax = m_rMinMaxRS[0];
//D						  m_rMinMaxRS[0] = m_rMinMaxRS[1];
//D						  m_rMinMaxRS[1] = rMinMax;
//D						  return rMinMax;}

/////////////////////////////////////////////////////////////////////////////
//  �����W����_�����W�ւ̍��W�ϊ��}�g���b�N�X��
//	�_�����W����\�����W�ւ̕ϊ��{����ݒ肷��
	void	SetMat( );

/////////////////////////////////////////////////////////////////////////////
//  ���݂̃}�E�X�ʒu��ݒ肷��
	void	SetCurMousePtL( CPoint ptCurMous)
						{ m_ptCurMouseL = ptCurMous;}

/////////////////////////////////////////////////////////////////////////////
//  ���݂̃}�E�X�ʒu�𓾂�
	CPoint	GetCurMousePtL( )
						{ return m_ptCurMouseL;}

/////////////////////////////////////////////////////////////////////////////
//  ���݂̃}�E�X�ʒu�𓾂�
	MgPoint2 GetCurMousePtR( )
						{ return LPtoRP( m_ptCurMouseL);}

/////////////////////////////////////////////////////////////////////////////
//  ���W�ϊ�����@�����W�@���@�_�����W
	MINT	RPtoLP( 	MREAL rRP);
	CPoint	RPtoLP( MgPoint2 pRP);

/////////////////////////////////////////////////////////////////////////////
//  ���W�ϊ�����@�_�����W�@���@�\�����W
	MINT	LPtoDP( MINT   iLP);
	CPoint	LPtoDP( CPoint pLP);

/////////////////////////////////////////////////////////////////////////////
//  ���W�ϊ�����@�����W�@���@�\�����W
	MINT	RPtoDP( MREAL   rRP);
	CPoint	RPtoDP( MgPoint2 pRP);

/////////////////////////////////////////////////////////////////////////////
//  ���W�ϊ�����@�\�����W�@���@�_�����W
	MINT	DPtoLP( MINT   iDP);
	CPoint	DPtoLP( CPoint pDP);

/////////////////////////////////////////////////////////////////////////////
//  ���W�ϊ�����@�_�����W�@���@�����W
	MREAL		LPtoRP( MINT i);
	MgPoint2	LPtoRP( CPoint p);

/////////////////////////////////////////////////////////////////////////////
//  ���W�ϊ�����@�\�����W�@���@�����W
	MREAL		DPtoRP( MINT   iDP);
	MgPoint2	DPtoRP( CPoint pDP);
	
/////////////////////////////////////////////////////////////////////////////
//  �������@����A�����A���F��ݒ肷��
	void	SetLineAttr( 
						MPENSTYLE	iLineStyle		= MPS_SOLID,
						MREAL		fLineWidth		= 1,
						DWORD		rgbLine			= RGB( 0, 0, 0),
						bool		bFixedLineWidth = TRUE
						);

/////////////////////////////////////////////////////////////////////////////
//  	�����ݒ肷��
	void	SetLineStyle( MPENSTYLE iLineStyle);

/////////////////////////////////////////////////////////////////////////////
//  	������ݒ肷��
	void	SetLineWidth( MREAL fLineWidth);

/////////////////////////////////////////////////////////////////////////////
//  	���F��ݒ肷��
	void	SetLineColor( DWORD rgbLine);
/////////////////////////////////////////////////////////////////////////////
//  	�\�������Œ�t���O��ݒ肷��
	void	SetFixedLineWidth( bool bFixedLineWidth = TRUE);	// �\�������Œ�t���O
																//			TRUE  :	�\�������Œ�
																//			FALSE : �\���������R

/////////////////////////////////////////////////////////////////////////////
//  ���������@�t�H���g�A���������A���������A�I�t�Z�b�g�����ʒu��ݒ肷��
	void	SetTextAttr( 
						MCHAR*		sTextFont,
						MREAL		fTextHeight		= 10,
						MINT		iPosUL			= MT_LOWER,
						MINT		iPosLR			= MT_LEFT,
						DWORD		rgbText			= RGB( 0, 0, 0),
						MgVect2&	vDirect			= MgPoint2( 1., 0.),
						MgPoint2&	pOffset			= MgPoint2( 0., 0.),
						bool		bFixedTextSize	= TRUE
						);

/////////////////////////////////////////////////////////////////////////////
//  	�t�H���g��ݒ肷��
	void	SetTextFont( MCHAR* sTextFont);

/////////////////////////////////////////////////////////////////////////////
//  	����������ݒ肷��
	void	SetTextHeight( MREAL fTextHeight);

/////////////////////////////////////////////////////////////////////////////
//  	�����ʒu��ݒ肷��
	void	SetTextIchi( MINT iPosUL, MINT iPosLR);

/////////////////////////////////////////////////////////////////////////////
//  	�����F��ݒ肷��
	void	SetTextColor( DWORD rgb);

/////////////////////////////////////////////////////////////////////////////
//  	����������ݒ肷��
	void	SetTextDirect( MgVect2& vDirect = MgPoint2( 1., 0.));

/////////////////////////////////////////////////////////////////////////////
//  	�I�t�Z�b�g��ݒ肷��
	void	SetTextOffset( MgPoint2& pOffset = MgPoint2( 0., 0.));

/////////////////////////////////////////////////////////////////////////////
//  	�����T�C�Y�Œ�t���O��ݒ肷��
	void	SetFixedTextSize( bool bFixedTextSize = FALSE);

/////////////////////////////////////////////////////////////////////////////
//  �������\������
	void	Text( const MgPoint2 &pa1, MCHAR* str);

/////////////////////////////////////////////////////////////////////////////
//  ������\������
	void	Line( const MgLine2 &ln1)
						{ Line( ln1.p[0], ln1.p[1]);}

/////////////////////////////////////////////////////////////////////////////
//  ������\������
	void	Line( const MgPoint2 &pa1, const MgPoint2 &pa2);

/////////////////////////////////////////////////////////////////////////////
//  �����`��\������
	void	Rect( const MgPoint2 &pa1, const MgPoint2 &pa2);

/////////////////////////////////////////////////////////////////////////////
//  �����`��\������
	void	Rect( const MgRect2 &rt1)
						{ Rect( rt1.p[0], rt1.p[1]);}

/////////////////////////////////////////////////////////////////////////////
//  �����`��\������
//C	void	Rect( const MgPoint2 &pa1, const MgPoint2 &pa2);

/////////////////////////////////////////////////////////////////////////////
//  �܂����\������
	void	Polyline( MgPoint2 *pb, int inpb);

/////////////////////////////////////////////////////////////////////////////
//  ���p�`��\������
	void	Polygon( MgPoint2 *pb, int inpb);

/////////////////////////////////////////////////////////////////////////////
//  �~�ʂ�\������
	void	Arc1( const MgPoint2 &pac, MREAL rh, MREAL a1, MREAL a2);

};
/*
//
/////////////////////////////////////////////////////////////////////////////
//  �������@����A�����A���F��ݒ肷��
	void	SetLineAttr( 
						MPENSTYLE	iLineStyle		= MPS_SOLID,
						MREAL		fLineWidth		= 1,
						DWORD		rgbLine			= RGB( 0, 0, 0),
						bool		bFixedLineWidth = TRUE
						);
*/
// inline //

/////////////////////////////////////////////////////////////////////////////
//  ���W�ϊ�����@�����W�@���@�_�����W
inline CPoint MbCod::RPtoLP( MgPoint2 p)
{
	MgPoint2 p1 = p * m_mat;
	return MgCPointC( p1);
}

inline MINT	MbCod::RPtoLP( 
				MREAL		rRP
)
{
	return MINT( rRP * m_sclRPtoLP);
}

/////////////////////////////////////////////////////////////////////////////
//  ���W�ϊ�����@�_�����W�@���@�\�����W
inline CPoint MbCod::LPtoDP( 
				CPoint		pLP
				)
{
	return CPoint( MINT( pLP.x * m_vsclLPtoDP.x),
				   MINT( pLP.y * m_vsclLPtoDP.y));
}

inline MINT	MbCod::LPtoDP( 
				MINT   		iLP
				)
{
	return ( int)( iLP * m_vsclLPtoDP.x);
}

/////////////////////////////////////////////////////////////////////////////
//  ���W�ϊ�����@�����W�@���@�\�����W
inline CPoint MbCod::RPtoDP( 
				MgPoint2	pRP
				)
{
	MgPoint2 pLP = pRP * m_mat; 
	return CPoint( MINT( pLP.x * m_vsclLPtoDP.x),
				   MINT( pLP.y * m_vsclLPtoDP.y));
}

inline MINT	MbCod::RPtoDP( 
				MREAL	   rRP
				)
{
	return ( MINT)( rRP * m_sclRPtoLP * m_vsclLPtoDP.x);
}

/////////////////////////////////////////////////////////////////////////////
//  ���W�ϊ�����@�\�����W�@���@�_�����W
inline CPoint MbCod::DPtoLP( 
				CPoint		pDP
				)
{
	return CPoint( MINT( pDP.x / m_vsclLPtoDP.x),
				   MINT( pDP.y / m_vsclLPtoDP.y));
}

inline MINT	MbCod::DPtoLP( 
				MINT		iDP
				)
{
	return ( int)( iDP / m_vsclLPtoDP.x);
}

/////////////////////////////////////////////////////////////////////////////
//  ���W�ϊ�����@�_�����W�@���@�����W
inline MgPoint2 MbCod::LPtoRP( 
				CPoint		p
				)
{
	MgPoint2 MbP2_t1= p;
	return MbP2_t1 * MGeo::Mat3Inv( m_mat);
}

inline MREAL MbCod::LPtoRP( 
				MINT		i
				)
{
	return MREAL( i / m_sclRPtoLP);
}

/////////////////////////////////////////////////////////////////////////////
//  ���W�ϊ�����@�\�����W�@���@�����W
inline MREAL MbCod::DPtoRP( 
				MINT	   iDP
				)
{
	return ( iDP / ( m_sclRPtoLP * m_vsclLPtoDP.x));
}

inline MgPoint2 MbCod::DPtoRP( 
				CPoint		pDP
				)
{
	MgPoint2 pLP = MgPoint2( pDP.x / m_vsclLPtoDP.x,
							 pDP.y / m_vsclLPtoDP.y);
	return pLP * MGeo::Mat3Inv( m_mat);
}

} // namespace MC
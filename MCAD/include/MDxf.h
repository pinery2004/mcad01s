#pragma once

namespace MC
{

////////////////////////////////////////////////////////////////////////////
//	�c�w�e�t�@�C���̓Ǎ���
MINT MDxfFlLoad();

struct MdPoint3 {
	MREAL		x;
	MREAL		y;
	MREAL		z;
};

struct MdLine3 {
	MdPoint3	p[2];
};

struct MdCir3 {
	MdPoint3	plu;									// Left Up
	MdPoint3	prd;									// Right Down
};

struct MdArc3 {
	MdPoint3	plu;									// Left Up
	MdPoint3	prd;									// Right Down
	MdPoint3	ps;										// Start
	MdPoint3	pe;										// End
};

struct MdText3 {
	MdPoint3	Pnt;									// �z�u���_�i���W�l�j
	MREAL		Ang;									// �p�x    �i�p�x�F�P�ʓx�j
	MREAL		Hei;									// ���������i�����F�P��mm�j
	MREAL		Msc;									// ���������ƃX�y�[�V���O�̔䗦
	MINT		Num;									// ������  �i���j
	MCHAR*		String; 								// ������
};

struct MdFace3 {
	MINT		fClose;									// 1:Polygon	0:PlyLine
	MINT		n;										// ���_�� <= 4
	MINT		p[4];									// ���_
};

struct  MdMesh3 {
	MINT		nMesh;									// ү����
	MINT*		nVerNumPnt;								// ү�����̒��_����
	MINT		nPntNo;									// ���_�ԍ���
	MINT*		PntNo;									// ���_�ԍ���
	MINT		nPt;									// ���_��
	MgPoint3*	Pt;										// ���_��
};

union MdPrm {											// �}�`�v�f
	MdPoint3	Dot;									// �_
	MdLine3		Line;									// ��
	MdCir3		Circle;									// �~
	MdArc3		Arc;									// �~��
	MdText3		Text;									// ������
	MdFace3		Face;									// ��
	MdMesh3		Mesh;									// ���b�V��
};

} // namespace MC
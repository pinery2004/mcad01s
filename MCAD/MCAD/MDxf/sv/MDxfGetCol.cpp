/******************************************************************************************************************
              �c�w�e  �v���O�����t�@�C�� (MDxf.cpp)

[�@�\] �F�ԍ�����F�𓾂�
[�Ԓl] = 0 ����  < 0�G���[
******************************************************************************************************************/

#include	"MDxf.h"

typedef struct _basecolor {
	MINT		Red;
	MINT		Green;
	MINT		Blue;
} RGBSTR;

static RGBSTR BasColor[10] = { {   5,    5,    5 },										// ���i�Y���Ȃ��̂͂��j
							   { 250,    5,    5 },										// ��
							   { 250,  250,    5 },										// ��
							   {   5,  250,    5 },										// ��
							   {   5,  250,  250 },										// �V�A��
							   {   5,    5,  250 },										// ��
							   { 250,    5,  250 },										// �}�[���_
							   { 250,  250,  250 },										// ��
							   {   5,    5,    5 },										// ���i�Y���Ȃ��̂͂��j
							   {   5,    5,    5 } } ;									// ���i�Y���Ȃ��̂͂��j

static RGBSTR MidColor[24] = { { 250,    5,    5 },										// ��
							   { 250,   63,    5 },
							   { 250,  127,    5 },										// ��
							   { 250,  191,    5 },
							   { 250,  250,    5 },										// ��
							   { 191,  250,    5 },
							   { 127,  250,    5 },										// ����
							   {  63,  250,    5 },
							   {   5,  250,    5 },										// ��
							   {   5,  250,   63 },
							   {   5,  250,  127 },										// ��
							   {   5,  250,  191 },
							   {   5,  250,  250 },										// �V�A��
							   {   5,  191,  250 },
							   {   5,  127,  250 },										// ��
							   {   5,   63,  250 },
							   {   5,    5,  250 },										// ��
							   {  63,    5,  250 },
							   { 127,    5,  250 },										// ��
							   { 191,    5,  250 },
							   { 250,    5,  250 },										// �}�[���_
							   { 250,    5,  191 },
							   { 250,    5,  127 },										// �A�Y�L
							   { 250,    5,   63 } };


COLORREF MDxfGetCol           (														// (  O) RGB�װ�l
								MINT				Color)								// (I  ) �F�ԍ�
{
	COLORREF			Rgb;															// RGB�װ�l
	MINT				Red;															// ��
	MINT				Green;															// ��
	MINT				Blue;															// ��


	MDxfGetCol(Color, &Red, &Green, &Blue);

	Rgb = RGB(Red, Green, Blue);

	return(Rgb);
}

HVOID MDxfGetCol              (
								MINT				Color,								// (I  ) �F�ԍ�
								MINT*				Red,								// (  O) ��
								MINT*				Green,								// (  O) ��
								MINT*				Blue)								// (  O) ��
{
	MINT				Id;																// ���蓖�Ĕԍ��i�Q���j
	MINT				Kido;															// �P�x�i�P���j


	if (Color <=0 || Color > 255)  goto  PRG_EXIT;										// �F�ԍ�����

	// �F�ԍ����犄�蓖�Ĕԍ��ƋP�x�𓾂�
	Id   = Color / 10;
	Kido = Color % 10;

	if (Id == 0) {																		// ��{�F
		*Red   = BasColor[Kido].Red;
		*Green = BasColor[Kido].Green;
		*Blue  = BasColor[Kido].Blue;
	} else if (Id == 25) {																// ��ڰ�n���U�K���ɕ�����
		*Red   = MINT(1.0f * Kido / 6.0f * 250);
		*Green = MINT(1.0f * Kido / 6.0f * 250);
		*Blue  = MINT(1.0f * Kido / 6.0f * 250);
	} else {
		if (Kido % 2 == 0) {															// ���ƍ���
			*Red   = MINT(MidColor[Id-1].Red   * (1.0f - Kido / 10.0f));
			*Green = MINT(MidColor[Id-1].Green * (1.0f - Kido / 10.0f));
			*Blue  = MINT(MidColor[Id-1].Blue  * (1.0f - Kido / 10.0f));
		} else {																		// ���F����
			/* ���F  ( RGB + 1.0(White) ) / 2 * ( 1 - ( Kido - 1 )/10 )
				     ------------------------   -----------------------
					   ���F�����̐F             �P�x(1�����邭9���Â��̂�1�̂Ƃ��ő�) */
			*Red   = MINT((MidColor[Id-1].Red   + 250) * 0.5f * (1.1f - Kido / 10.0f));
			*Green = MINT((MidColor[Id-1].Green + 250) * 0.5f * (1.1f - Kido / 10.0f));
			*Blue  = MINT((MidColor[Id-1].Blue  + 250) * 0.5f * (1.1f - Kido / 10.0f));
		}
	}

PRG_EXIT:

	return;
}

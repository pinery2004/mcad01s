/******************************************************************************************************************
              ＤＸＦ  プログラムファイル (MDxf.cpp)

[機能] ポリライン上の２点と膨らみから、円弧情報を作成する
[返値] = 0 正常  < 0エラー
******************************************************************************************************************/


#include	"MDxf.h"


MINT MDxfPlnToArc         (
							MgPoint3*			SPnt,								// (I  ) 開始点
							MgPoint3*			EPnt,								// (I  ) 終了点
							MFLOAT				Tan,								// (I  ) 膨らみ
							HARC3S*				Arc)								// (  O) 円弧情報
{
	MINT				Status;														// ｽﾃｰﾀｽ
	MFLOAT				Ang;														// 中心角度（度）
	MgPoint3				Pnt[2];														// 作業用
	MgPoint3				CPnt;														// 中心点
	MFLOAT				Radius;
	MFLOAT				Sta, Opa;
	MFLOAT				Radian;


	Status = 0;

	// 時計周りの場合は反時計周りに修正始点と終点を入れ替える
	if (Tan < 0) {
		memcpy(&Pnt[0], EPnt, sizeof(MgPoint3));
		memcpy(&Pnt[1], SPnt, sizeof(MgPoint3));
	} else {
		memcpy(&Pnt[0], SPnt, sizeof(MgPoint3));
		memcpy(&Pnt[1], EPnt, sizeof(MgPoint3));
	}

	// 中心角を求める
	Ang = (MFLOAT)(HABS(atan(Tan) / HPI * 720.0f)); 										//  Tan = tan(Ang / 4)

	Status = MDxfCalArcAtr(&Pnt[0], &Pnt[1], Ang, &CPnt, &Radius);						// 中心点と半径を求める
	if (Status != 0) goto  PRG_EXIT;

	// 始点角、終点角を求める
	Sta = MDxfGetAngle(&CPnt, &Pnt[0]);
	Opa = MDxfGetAngle(&CPnt, &Pnt[2]);

	Arc->Pnt1.X = CPnt.X - Radius;														// 外接する左上
	Arc->Pnt1.Y = CPnt.Y - Radius;
	Arc->Pnt1.Z = CPnt.Z;
	Arc->Pnt2.X = CPnt.X + Radius;														// 外接する右下
	Arc->Pnt2.Y = CPnt.Y + Radius;
	Arc->Pnt2.Z = CPnt.Z;
	Radian      = Sta / 180.0f * HPI;													// ﾗｼﾞｱﾝへ
	Arc->Pnt3.X = CPnt.X + Radius * (MFLOAT)cos((HDOUBLE)Radian);
	Arc->Pnt3.Y = CPnt.Y + Radius * (MFLOAT)sin((HDOUBLE)Radian);
	Arc->Pnt3.Z = CPnt.Z;
	Radian      = Opa / 180.0f * HPI;													// ﾗｼﾞｱﾝへ
	Arc->Pnt4.X = CPnt.X + Radius * (MFLOAT)cos((HDOUBLE)Radian);
	Arc->Pnt4.Y = CPnt.Y + Radius * (MFLOAT)sin((HDOUBLE)Radian);
	Arc->Pnt4.Z = CPnt.Z;

PRG_EXIT:

	return(Status);
}

/******************************************************************************************************************
[機能] 始点終点と中心角度から円弧の中心と半径を求める  （２次元図形を扱い２点のZ座標が0.0でない場合はエラーとする）
[返値] = 0 正常  < 0エラー 
******************************************************************************************************************/
MINT MDxfCalArcAtr        (
							MgPoint3*			Pnt1,								// (I  ) １点目
							MgPoint3*			Pnt2,								// (I  ) ２点目
							MFLOAT				Ang,								// (I  ) 中心角度
							MgPoint3*			Center,								// (  O) 中心点
							MFLOAT*				Radius)								// (  O) 半径
{
	MINT				Status;														// ｽﾃｰﾀｽ
	MgPoint3				MidPnt;														// ２点の中点
	MgPoint3				Vector;														// ２点の中心から円弧中心への方向ベクトル
	MFLOAT				Height;														// ２点の中心から円弧中心までの距離
	HDOUBLE				Radian;														// 中心角（radian）
	HDOUBLE				Dst;														// ２点間距離


	Status = -1;

	// ２点のZ座標をチェック
	if (!HCMP(Pnt1->Z, Pnt2->Z))  goto  PRG_EXIT;	

	//	２点間の距離を求める
	Dst = sqrt((Pnt2->X - Pnt1->X) * (Pnt2->X - Pnt1->X) + 
			   (Pnt2->Y - Pnt1->Y) * (Pnt2->Y - Pnt1->Y) + 
			   (Pnt2->Z - Pnt1->Z) * (Pnt2->Z - Pnt1->Z));

	if (Dst < HDISTOL)  goto  PRG_EXIT;

	Radian  = 0.5 * Ang * HPI / 180.0f;												// 中心角の1/2を求める(ﾗｼﾞｱﾝ単位)
	*Radius = (MFLOAT)(Dst * 0.5f / sin(Radian));									// 半径を求める

	// ２点の中点から中心までの距離を求める
	Height = (MFLOAT)(Dst * 0.5f / tan(Radian));

	MidPnt.X = (Pnt1->X + Pnt2->X) * 0.5f;											// ２点の中点を求める
	MidPnt.Y = (Pnt1->Y + Pnt2->Y) * 0.5f;
	MidPnt.Z = (Pnt1->Y + Pnt2->Y) * 0.5f;

	// 中点を通り中心へ向かう方向ベクトルを求める
	Vector.X = (MFLOAT)((-1.0f) * (Pnt2->X - Pnt1->X) / Dst);
	Vector.Y = (MFLOAT)((Pnt2->Y - Pnt1->Y) / Dst);
	Vector.Z = 0.0f;

	// 角度から判定して中心点を得る
	if ((180 < Ang) && (Ang < 360)) {
		Center->X = MidPnt.X - Height * Vector.X;
		Center->Y = MidPnt.Y - Height * Vector.Y;
		Center->Z = MidPnt.Z - Height * Vector.Z;
	} else {
		Center->X = MidPnt.X + Height * Vector.X;
		Center->Y = MidPnt.Y + Height * Vector.Y;
		Center->Z = MidPnt.Z + Height * Vector.Z;
	}
	Status = 0;

PRG_EXIT:

	return(Status);
}
/******************************************************************************************************************
[機能] 円弧の点の角度を求める  （円弧はXY平面（Z=0)上にのっているものとする）
[返値] = 0 正常  < 0エラー 
******************************************************************************************************************/
MFLOAT MDxfGetAngle        (														// (  O) 角度
							MgPoint3*			Center,								// (I  ) 円弧中心
							MgPoint3*			Pnt)								// (I  ) 円弧上の点
{
	HDOUBLE				SaX;														// X座標差分
	HDOUBLE				SaY;														// Y座標差分
	HDOUBLE				Angle;														// 角度（戻り値）


	Angle = 0.0f;																	// 角度（戻り値）

	// XY平面上かどうかチェック
	if ((Center->Z != 0.0f) || (Pnt->Z != 0.0f))  goto  PRG_EXIT;

	// 2点の差分を求める
	SaX = Pnt->X - Center->X;
	SaY = Pnt->Y - Center->Y;

	// 角度チェック
	if (SaX == 0.0) {																// Y軸上
		if (SaY > 0) {
			Angle = 90.0f;															// 90度
		} else {
			Angle = 270.0f;															// 270度
		}
	} else {
		Angle = atan(SaY / SaX) * 180.0f / HPI;
	}

PRG_EXIT:

	return((MFLOAT)Angle);
}

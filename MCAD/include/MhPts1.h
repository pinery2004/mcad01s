////////////////////////////////////////////////////////////////////////////
//	  配置物
////////////////////////////////////////////////////////////////////////////
#pragma once

namespace MC
{

class	MhSenBuzai
{
public:
	MgLine3		Ln;									// 部材の配置点、配置方向点
	MgVect3		vtUt;								// 部材の配置方向
	MREAL		rW[2];								// 部材の右幅、左幅
	MREAL		rLH[2];								// 部材の長さ補正値
	MgLine3		LnWH;								// 部材の補正考慮実長さの線分
	MgULine3	ULnW[2];							// 部材の右辺、左辺　直線

public:
	MhSenBuzai()		{}
	MhSenBuzai( MhPlcInfo &PlcEn)		{ 
		Ln     = PlcEn.GetPIPlcIti();						// 長さ調整側の部材
		vtUt   = Ln.Vu();
		rLH[0] = PlcEn.GetPILenHosei( 0);
		rLH[1] = PlcEn.GetPILenHosei( 1);
		rW[0]  = PlcEn.GetMbWidthR() + PlcEn.GetPISinZure();
		rW[1]  = PlcEn.GetMbWidthL() - PlcEn.GetPISinZure();
		LnWH = MgLine3( Ln.p[0] - rLH[0] * vtUt + PlcEn.GetPISinZure() * vtUt.RotR90(),
						Ln.p[1] + rLH[1] * vtUt + PlcEn.GetPISinZure() * vtUt.RotR90());
		ULnW[0] = MgULine3( Ln.p[0] + vtUt.RotR90() * rW[0], vtUt);
		ULnW[1] = MgULine3( Ln.p[0] + vtUt.RotL90() * rW[1], vtUt);
	}
/*
	void operator = (MhPlcInfo &PlcEn) {
		Ln     = PlcEn.GetPIPlcIti();						// 長さ調整側の部材
		vtUt   = Ln.V();
		rLH[0] = PlcEn.GetPILenHosei( 0);
		rLH[1] = PlcEn.GetPILenHosei( 1);
		rW[0]  = PlcEn.GetMbWidthR() + PlcEn.GetPISinZure();
		rW[1]  = PlcEn.GetMbWidthL() - PlcEn.GetPISinZure();
	}	
*/
	//　横置部材と横置部材の位置関係を調べる。
	//　２線部材が重なっている場合は、交差なし（平行）とみなす。
	//  ２線部材が重なりなく直線的に交差している場合はその交差点を交点とみなす。
	static MINT	MhOn(										// ステイタス
														// MC_PARALLEL	(-1) 交差なし（平行）
														// MC_NINT	    (0)	交差なし
														// MC_INT		(1)	交差あり
					const	MhSenBuzai&	i_Bz1,			// 線部材1
					const	MhSenBuzai&	i_Bz2,			// 線部材2
							MgPoint3*	o_pPo,			// 交点
							MINT*		o_pist1,		// 補助ステイタス1
														//		MC_LEFT:	線部材1は線部材2の左側にあり	（交差なし）
														//		MC_RIGHT:	線部材1は線部材2の右側にあり	（交差なし）
														//		MC_CLOSS:	線部材1は線部材2と交差			（交差あり）
														//		MC_ON_LINE:	線部材1は線部材2の直線上にあり	（平行）
							MINT*		o_pist2			// 補助ステイタス2
														//		MC_PS_FAR:	線部材1は線部材2より始点側が遠い（交差なしまたは交差あり）
														//		MC_PE_FAR:	線部材1は線部材2より終点側が遠い（交差なしまたは交差あり）
														//		MC_PARALEL:	線部材1は線部材2より始点と終点は等距離	（平行）
					);

	};


} // namespace MC
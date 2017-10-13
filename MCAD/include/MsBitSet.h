#pragma once

/////////////////////////////////////////////////////////////////////////////
//【機能】ビット集合(SOB) クラス
#define	MSZ_SOBW		3
#define	MSZ_SOBB	(3 * 32)

namespace MC
{

class MsBitSet
{
	MUINT	sob[MSZ_SOBW];					// ビットの集合
public:
	MsBitSet()	{}
	MsBitSet( MUINT i1, MUINT i2, MUINT i3)
					{ sob[0] = i1; sob[1] = i2; sob[2] = i3;}
//
	MUINT* GetSOB()
					{ return sob;}
	MUINT SOB(int i)
	{
		return sob[i];
	}
	bool IsNull()
					{ return ( sob[0] == 0 && sob[1] == 0 && sob[2] == 0);}
	bool IsStdOn()
					{ return ( sob[0] && 1);}
//	bool IsStd()
//					{ return ( sob[0] == 1 && sob[1] == 0 && sob[2] == 0);}
//	bool IsFirst()
//					{ return ( sob[0] == 1 && sob[1] == 0 && sob[2] == 0);}

	void SetMask( MINT iId)
					{ ASSERT ( iId>=0 && iId<MSZ_SOBB);		// ID該当のビットのみ１のビット集合(SOB)を算出する
					  MINT iw = iId / 32;
					  MINT ib = iId - iw * 32;
					  sob[0] = sob[1] = sob[2] = 0;
//					  if ( iw == 0) sob[iw] = 1 << ib;}
					  if ( iw <= 2) sob[iw] = 1 << ib;}

	friend MsBitSet operator ~ ( MsBitSet &Verv)
					{ MsBitSet Vervw;
					  Vervw.sob[0] = ~Verv.sob[0];
					  Vervw.sob[1] = ~Verv.sob[1];
					  Vervw.sob[2] = ~Verv.sob[2];
					  return Vervw;}

	MsBitSet operator & ( const MsBitSet &Verv) const
					{ MsBitSet Vervw;
					  Vervw.sob[0] = sob[0] & Verv.sob[0];
					  Vervw.sob[1] = sob[1] & Verv.sob[1];
					  Vervw.sob[2] = sob[2] & Verv.sob[2];
					  return Vervw;}

	MsBitSet operator | ( const MsBitSet &Verv) const
					{ MsBitSet Vervw;
					  Vervw.sob[0] = sob[0] | Verv.sob[0];
					  Vervw.sob[1] = sob[1] | Verv.sob[1];
					  Vervw.sob[2] = sob[2] | Verv.sob[2];
					  return Vervw;}

	MsBitSet operator &= ( const MsBitSet &Verv)
					{ sob[0] &= Verv.sob[0];
					  sob[1] &= Verv.sob[1];
					  sob[2] &= Verv.sob[2];
					  return *this;}

	MsBitSet operator |= ( const MsBitSet &Verv)
					{ sob[0] |= Verv.sob[0];
					  sob[1] |= Verv.sob[1];
					  sob[2] |= Verv.sob[2];
					  return *this;}

	bool operator && ( const MsBitSet &Verv) const					// 重なりチェック
					{ return ((sob[0] & Verv.sob[0]) != 0 ||
							  (sob[1] & Verv.sob[1]) != 0 ||
							  (sob[2] & Verv.sob[2]) != 0);}

	bool operator == ( const MsBitSet &Verv) const
					{ return ((sob[0] == Verv.sob[0]) &&
							  (sob[1] == Verv.sob[1]) &&
							  (sob[2] == Verv.sob[2]));}
};

} // namespace MC
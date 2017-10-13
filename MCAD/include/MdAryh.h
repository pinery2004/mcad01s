#pragma once
//===================================================================
//		MdAryH
//
//		構造体群 
//				・構造体の集合を定義する為のベースクラス
//
//				派生クラス
//
//					MdAryH_S:	固定長の構造体で開放時には、各entityのFreeは行わない
//					MdAryH_F:	可変長の構造体で開放時には、各entityのFreeも行う
//

#include	"MdmDefine.h"

namespace MC
{

template <typename T> class MdAryH
{
												// 開放時に子もFREEする
public:
	MUSHORT		m_sz;							// 最大構造体数
	MUSHORT		m_n;							// 構造体数
	T*			m_st;							// 構造体
public:
	MdAryH()	
						{ m_sz = m_n = 0; m_st = NULL;}
	MdAryH( int i_sz);

	void Alloc( int i_sz);									// 指定最大構造体数の領域を確保、エラー表示用にProg名称を入力
	
	void ReSize( int i_sz);									// 最大構造体数の変更と領域確保、エラー表示用にProg名称を入力

	void Init()			{ m_sz = m_n = 0; m_st = NULL;}		// この群がさらに群になった場合の最終データより後のデータのクリア用

	T& operator [] (int idx)								// ()のチェック機能と領域拡張機能を取り外し高速版	
						{ return m_st[MIDtoHN( idx)];}

	// デバッグ用トレース出力
	void PrintC( MCHAR* i_s);
};

//	コンストラクタ
template <typename T> inline MdAryH<T>::MdAryH(
				int		i_sz								// 構造体群の初期に確保する構造体の数
				)
{
	m_sz = i_sz;
	m_n = 0;
	MBMALLOC( m_st, T, i_sz);
}

//	構造体群領域の確保
template <typename T> inline void MdAryH<T>::Alloc( int i_sz)
{
	MBMALLOC( m_st, T, i_sz);									//	構成構造体の領域をmallocで確保
	memset( m_st, 0, sizeof( T) * ( i_sz));
	m_sz = i_sz;
}

//	構造体群領域の拡張
template <typename T> inline void MdAryH<T>::ReSize( int i_sz)
{
	if ( i_sz > m_sz) {
		T	*stb = m_st;
		MBMALLOC( m_st, T, i_sz);		 						//	構成点の新領域をmallocで確保
		if ( m_sz)
			memcpy( m_st, stb, m_sz * sizeof( T));				//	構成点の領域にデータがあった場合は新領域へコピーする
		if ( stb)												//	構成点の領域をmallocで確保していた場合はその領域を開放する
			delete (char*)stb;									//
		memset( &m_st[m_sz], 0, sizeof( T) * ( i_sz - m_sz));
		m_sz = i_sz;
	}
}

//	トレース
template <typename T> inline void MdAryH<T>::PrintC(MCHAR* i_s)
{
#ifdef _DEBUG
	Msprintf( MbLog::m_Str, Mstr( "%s	< MdAryH > サイズ: %d，頂点数: %d\n"),
									i_s, m_sz, m_n);
	MBTRCPRBF;
	for ( int iC=0; iC<m_n; iC++) {
		m_st[iC].Print( i_s);
	}
#endif
}

//===================================================================
//		MdAryH_S
//
//		構造体群 
//				・構造体(固定長クラスデータのentity)の集合を定義する
//				・開放時に全体のfreeだけ行い、各entity毎のfreeは行わない
//
template <typename T>
class MdAryH_S : public MdAryH<T>
{
public:
	MdAryH_S() : MdAryH() {};
	MdAryH_S( int i_sz) : MdAryH( i_sz) {};
	~MdAryH_S();
	void Free();
	void operator += ( const T &i_st);							// 構成構造体の追加	構造体群 += 構造体
	void operator -= ( const T &i_st);							// 構造体群の追加	構造体群 -= 構造体
	void operator = ( const MdAryH_S<T> &i_Gst);				// 複写				構造体群 = 構造体群
	void operator += ( const MdAryH_S<T> &i_Gst);				// 構造体群の追加	構造体群 += 構造体群
	void operator ++ ( int);									// 最後の１個の構造体を追加(中身が空の構造体を追加)
	T& operator () ( int i_idx);								// 構成構造体を()で指定して読み書きする
																// 最大数の次を指定した場合は領域を拡張する
																// さらに大きな値の指定の場合はオーバフローエラーとする
	//// デバッグ用トレース出力
	//void Print( MCHAR* i_s);
};

////	コンストラクタ
//template <typename T> inline MdAryH_S<T>::MdAryH_S( MUSHORT i_sz) : base( 100) {}

//	デストラクタ
template <typename T> inline MdAryH_S<T>::~MdAryH_S()
{
	MBFREE( m_st);
}

template <typename T> inline void MdAryH_S<T>::Free()
{
	if ( m_st) {
		MBFREE( m_st);
		m_sz = 0;
		m_n = 0;
	}
}

//	構成構造体の追加	構造体群 += 構造体
template <typename T> inline void MdAryH_S<T>::operator += (const T &i_st)					// Gst -= st1
{
	if ( m_n >= m_sz)
		ReSize( m_sz + MGMAX( m_sz, 4));
																//	２倍に領域拡張(拡張後の最低領域サイズは４とする)
	m_st[m_n] = i_st;
	m_n++;
}

//	構成構造体の削除	構造体群 -= 構造体
template <typename T> inline void MdAryH_S<T>::operator -= (const T &i_st)
{
	int iC;
	bool bFnd = false;
	for ( iC=0; iC<m_n; iC++) {
		if ( m_st[iC] == i_st) {
			for ( iC++; iC<m_n; iC++) {
				m_st[iC-1] = m_st[iC];
			}
			m_n --;
			bFnd = true;
			break;
		}
	}
	ASSERT( bFnd);
}

//	複写				構造体群 = 構造体群
template <typename T> inline void MdAryH_S<T>::operator = (const MdAryH_S<T>& i_Gst)
{
	MUSHORT	nn = i_Gst.m_n;
	if ( nn > m_sz) {
			MBFREE( m_st);										//	構成点の領域をmallocで確保していた場合は一旦free
		MBMALLOC( m_st, T, nn);									//	構成点の領域をmallocで確保
		m_sz = nn;
	}
	memcpy( m_st, i_Gst.m_st, sizeof( T) * nn);
	m_n = nn;
}

//	構造体群の追加		構造体群 += 構造体群
template <typename T> inline void MdAryH_S<T>::operator += (const MdAryH_S<T>& i_Gst)			// Gst += Gst1
{
	MUSHORT	nn = m_n + i_Gst.m_n;
	if ( nn > sz)
		ReSize( nn, Mstr( "MdAryH_S += Gst"));
	memcpy( &st[m_n], i_Gst.st, sizeof( T) * i_Gst.m_n);
	m_n = nn;
}

//	空の構成構造体を追加	構造体群 ++
template <typename T> inline void MdAryH_S<T>::operator ++ ( int)							// Gst ++
{
	if (m_n >= m_sz)
		ReSize( m_sz + MGMAX( m_sz, 4));
																//	２倍に領域拡張(拡張後の最低領域サイズは４とする)
	m_n++;
}

//	構成構造体を()で指定して読み書きする
//	例　Gst1(n).p, Gst1(n).i1, Gst1(n).i2
template <typename T> inline T& MdAryH_S<T>::operator () (int i_idx)
{
	if ( i_idx == m_sz) {
		ReSize( m_sz + MGMAX( m_sz, 4));							//	２倍に領域拡張(拡張後の最低領域サイズは４とする)
	} else if ( idx > m_sz) {
		MbSysError( MBCstr( "MdAryH()"), MC_ERR_OVERFLOW);
	}
	for (; m_n<=i_idx; n++) m_st[m_n].SetInit();						// 最終データより後のデータはクリアし、n = idx+1　にする
	return m_st[i_idx];
}

//===================================================================
//===================================================================
//		MdAryH_S_C
//
//		固定長のクラス(構造体)型データセット 
//
template <typename T>
class MdAryH_S_C : public MdAryH_S<T>
{
public:
	MdAryH_S_C() : MdAryH_S() {};
	MdAryH_S_C( MUSHORT i_sz) : MdAryH_S( i_sz) {};

	// デバッグ用トレース出力
	void Print( MCHAR* s, MINT i_i);
};

//	トレース
template <typename T> inline void MdAryH_S_C<T>::Print(MCHAR* i_s, MINT i_i)
{
#ifdef _DEBUG
	if ( m_n) {
		if ( i_i == MDC_NONE_ID) {
			Msprintf( MbLog::m_Str, Mstr( "\t\t%s < MdAryH_S_C > サイズ = %d、データ数 = %d\n"),
											i_s, m_sz, m_n);
		} else {
			Msprintf( MbLog::m_Str, Mstr( "\t\t%s < MdAryH_S_C > ID = %d，サイズ = %d、データ数 = %d\n"),
											i_s, i_i, m_sz, m_n);
		}
		MBTRCPRBF;
		for (MUSHORT iC=0; iC<m_n; iC++) {
			m_st[iC].Print( i_s, iC);
		}
	}
#endif
}

//===================================================================
//===================================================================
//		MdAryH_S_I
//
//		整数型データセット 
//
template <typename T>
class MdAryH_S_I : public MdAryH_S<T>
{
public:
	MdAryH_S_I() : MdAryH_S() {};
	MdAryH_S_I( MUSHORT i_sz) : MdAryH_S( i_sz) {};

	// デバッグ用トレース出力
	void Print( MCHAR* s, MINT i_i);
};

//	トレース
template <typename T> inline void MdAryH_S_I<T>::Print(MCHAR* i_s, MINT i_i)
{
#ifdef _DEBUG
	if ( m_n) {
		if ( i_i == MDC_NONE_ID) {
			Msprintf( MbLog::m_Str, Mstr( "\t\t\t%s < MdAryH_S_I > 整数型データセット，サイズ =%d，データ数 = %d\n"),
											i_s, m_sz, m_n);
		} else {
			Msprintf( MbLog::m_Str, Mstr( "\t\t\t%s < MdAryH_S_I > ID = %d，整数型データセット，サイズ =%d，データ数 = %d\n"),
											i_s, i_i, m_sz, m_n);
		}
		MBTRCPRBF;
		if ( m_n)
			MBTRCPRINTIN( Mstr( "\t\t\tデータ"), m_st, m_n);
	}
#endif
}

//===================================================================
//===================================================================
//		MdAryH_S_B
//
//		整数型データセット 
//
template <typename T>
class MdAryH_S_B : public MdAryH_S<T>
{
public:
	MdAryH_S_B() : MdAryH_S() {};
	MdAryH_S_B( MUSHORT i_sz) : MdAryH_S( i_sz) {};

	// デバッグ用トレース出力
	void PrintB( MCHAR* i_s, MINT i_i);
};

//	トレース
template <typename T> inline void MdAryH_S_B<T>::PrintB(MCHAR* i_s, MINT i_i)
{
#ifdef _DEBUG
	if ( i_i == MDC_NONE_ID) {
		Msprintf( MbLog::m_Str, Mstr( "\t\t\t%s < MdAryH_S_B > 整数型データセット，サイズ: %d，データ数: %d\n"),
										i_s, m_sz, m_n);
	} else {
		Msprintf( MbLog::m_Str, Mstr( "\t\t\t%s < MdAryH_S_B > ID = %d，整数型データセット，サイズ: %d，データ数: %d\n"),
										i_s, i_i, m_sz, m_n);
	}
	MBTRCPRBF;
	if ( m_n)
		MBTRCPRINTBN( Mstr( "			データ"), m_st, m_n);
#endif
}

//===================================================================
//===================================================================
//		MdAryH_F
//
//		構造体群 
//				・構造体(可変長クラスデータのentity)の集合を定義する
//				・開放時に全体のfreeだけでなく、各entity毎のfreeを行う
//
template <typename T>
class MdAryH_F : public MdAryH<T>
{
public:
	MdAryH_F() : MdAryH() {};
	MdAryH_F( MUSHORT i_sz) : MdAryH( i_sz) {};

	~MdAryH_F();
	void Free();
	void operator += (const T& i_st);							// 構成構造体の追加	構造体群 += 構造体
	void operator = (const MdAryH_F<T>& i_Gst);					// 複写				構造体群 = 構造体群
	void operator += (const MdAryH_F<T>& i_Gst);				// 構造体群の追加	構造体群 += 構造体群
	void operator ++ ( int);									// 最後の１個の構造体を追加(中身が空の構造体を追加)
	T& operator () ( int i_idx);								// 構成構造体を()で指定して読み書きする
																// 最大数の次を指定した場合は領域を拡張する
																// さらに大きな値の指定の場合はオーバフローエラーとする
	// デバッグ用トレース出力
	void Print( MCHAR* i_s, MINT i_i);
};

//	デストラクタ
template <typename T> inline MdAryH_F<T>::~MdAryH_F()
{
	MUSHORT	ic;
	if ( m_st) {
		for( ic=0; ic<m_n; ic++)									// MdAryH_S　に、子の構造体の領域開放を追加 
			if ( MDSPACEID( &m_st[ic]) != MDID_DELETE)			//
				delete [] &m_st[ic];							//	領域開放
		MBFREE( m_st);
	}
}

//	領域開放
template <typename T> inline void MdAryH_F<T>::Free()
{
	MUSHORT	ic;
	if ( m_st) {
		for( ic=0; ic<m_n; ic++) {
			if ( MDSPACEID( &m_st[ic]) != MDID_DELETE)
				m_st[ic].Free();								// 領域開放
		}
		MBFREE( m_st);
		m_sz = 0;
		m_n = 0;
	}
}

//	構成構造体の追加	構造体群 += 構造体
template <typename T> inline void MdAryH_F<T>::operator += (const T& i_st)					// Gst += i_st
{
	if (m_n >= m_sz)
		ReSize( m_sz + MGMAX( m_sz, 4), Mstr( "MdAryH_F += i_st"));
																//	２倍に領域拡張(拡張後の最低領域サイズは４とする)
	m_st[n].Init();
	m_st[n] = i_st;
	m_n++;
}

//	複写				構造体群 = 構造体群
template <typename T> inline void MdAryH_F<T>::operator = (const MdAryH_F<T>& i_Gst)
{
	MUSHORT	iC;
	MUSHORT	nn = i_Gst.m_n;
	if ( nn > m_sz) {
			MBFREE( m_st);										//	構成点の領域をmallocで確保していた場合は一旦free
		MBMALLOC( m_st, T, nn);									//	構成点の領域をmallocで確保
		m_sz = nn;
	}
//	memcpy( st, i_Gst.st, sizeof( T) * nn);
	for ( iC=0; iC<i_Gst.m_n; iC++) {
		m_st[m_n].Init();
		m_st[iC] = i_Gst.m_st[iC];
	}
	m_n = nn;
}

//	構造体群の追加		構造体群 += 構造体群
template <typename T> inline void MdAryH_F<T>::operator += (const MdAryH_F<T>& i_Gst)			// Gst += i_Gst
{
	MUSHORT	nn = n + Gst.n;
	if ( nn > sz)
		ReSize( nn, Mstr( "MdAryH_F += Gst"));

	for ( iC=0; iC<Gst.n; iC++) {
		st[iC+n].Init();
		st[iC+n] = Gst.m_st[iC];
	}
	n = nn;
}

//	空の構成構造体を追加	構造体群 ++
template <typename T> inline void MdAryH_F<T>::operator ++ ( int)							// Gst ++
{
	if (m_n >= m_sz)
		ReSize( m_sz + MGMAX( m_sz, 4));
																//	２倍に領域拡張(拡張後の最低領域サイズは４とする)
	m_st[m_n].Init();
	m_n++;
}

//	構成構造体を()で指定して読み書きする
//	例　Gst1(n).p, Gst1(n).i1, Gst1(n).i2
template <typename T> inline T& MdAryH_F<T>::operator () (int i_idx)
{
	if ( idx == m_sz) {
		ReSize( m_sz + MGMAX( m_sz, 4));	//	２倍に領域拡張(拡張後の最低領域サイズは４とする)
	} else if ( i_idx > m_sz) {
		MbSysError( MBCstr( "MdAryH_F()"), MC_ERR_OVERFLOW);
	}
	for (; m_n<=i_idx; n++)										// 最終データより後のデータはクリアし、n = idx+1　にする
		m_st[m_n].SetInit();
	return m_st[i_idx];
}

//	トレース
template <typename T> inline void MdAryH_F<T>::Print(MCHAR* i_s, int i_i)
{
	MBTRCPRBF;
	for (int ic=0; ic<m_n; ic++) {
		m_st[ic].Print( i_s, i_i, ic);
	}
}

} // namespace MC
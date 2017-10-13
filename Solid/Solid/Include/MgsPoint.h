#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MgsPoint.h
//
//		点の定義、編集
//
//
//  K.Matsu			10/07/07	Created.
//==========================================================================================
//
#include <cassert>
#include <iosfwd>

namespace euler
{

const float pi = 3.14159265f;
  
class MgsPoint
{
	float m_pt[4];

public:
	float& operator[](int i)
	{
		return m_pt[i];
	}

	float const& operator[](int i) const
	{
		return m_pt[i];
	}

	MgsPoint(float x=.0f, float y=.0f, float z=.0f, float w=1.0f)
	{
		m_pt[0] = x;
		m_pt[1] = y;
		m_pt[2] = z;
		m_pt[3] = w;
	}

	MgsPoint operator + (MgsPoint const& pt) const
	{
		assert( m_pt[3] == pt[3]);

		MgsPoint ptRet( *this);

		return ptRet += pt;
	}

	MgsPoint& operator += (MgsPoint const& pt)
	{
		assert( m_pt[3] == pt[3]);

		m_pt[0] += pt[0];
		m_pt[1] += pt[1];
		m_pt[2] += pt[2];
		return *this;
	}

};

std::ostream& operator << (std::ostream& os, MgsPoint const& pt);

typedef MgsPoint MgsVect;


}//end namespace euler



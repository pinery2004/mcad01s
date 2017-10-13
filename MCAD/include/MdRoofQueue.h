#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MdPtsQueue.h
//
//		屋根 データベース
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
namespace MC
{

//	屋根キュー
class MdRoofQueue : public MbListE
{
public:
	MdRoofQueue(  MINT nBk, MINT iSz);
	virtual ~MdRoofQueue( void);

	MPOSITION Push( MhRoofInfo *pRoofEn);
	MPOSITION Inject( MhRoofInfo *pRoofEn);

	void RemoveAll( void);
};

} // namespace MC
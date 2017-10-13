#pragma once
//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: MdPtsQueue.h
//
//		�z�u�� �f�[�^�x�[�X
//
//
//  K.Matsu           08/01/04    Created.
//==========================================================================================
namespace MC
{


//	���ރL���[
class MdPtsQueue : public MbListE
{
public:
	MdPtsQueue(  MINT nBk, MINT iSz);
	virtual ~MdPtsQueue( void);

	MPOSITION Push( MhPlcInfo *pPlcEn);
	MPOSITION Inject( MhPlcInfo *pPlcEn);

	void RemoveAll( void);
};

} // namespace MC
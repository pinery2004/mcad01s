/* MmDrag.H
* 
 * Abstract
 * MmDrag.h classes : declaration
 *
 */ 
#pragma once

#include "MCAD.h"

namespace MC
{

class MmWndInfo;
enum MmDragType
{
	NO,
	ZOOM,									// ズーム処理のマウス左ボタンダウン移動中
	ZOOM_WAIT,								// ズーム処理のマウス左ボタンダウンの移動待ち
	PAN,									// 移動処理中
	ROT_CYL,
	ROT_CYL_X,
	ROT_CYL_Y,
	ROT_CYL_Z,
	ROT_SPH
};

class MmDrag
{
	public:
		MmDrag() {};
		MmDrag(MmWndInfo* pWndInfo, MgPoint2 ptStart);
		~MmDrag(void);

		void SetMotion( MmDragType Motion)
						{ m_Motion = Motion;}
		MmDragType GetMotion()
						{ return m_Motion;}
		void SetLastPt( MgPoint2 ptLatest)
						{ m_ptLast = ptLatest;}
//		MgPoint2 GetLastPt()
//						{ return m_ptLast;}
		void SetStartPt( MgPoint2 ptLatest)
						{ m_ptStart = ptLatest;};
		MgPoint2 GetStartPt()
						{ return m_ptStart;};
		virtual bool Continue( MmWndInfo* pWndInfo, MgPoint2 ptLatest);

	protected:
		MmWndInfo*		m_pWndInfo;
		MgPoint2		m_ptStart;
		MgPoint2		m_ptLast;
		MINT			m_iPrevCursorMode;

		void			RedrawAfter( MmWndInfo* pWndInfo);

	public:
		MmDragType		m_Motion;
};
	

class MmDragZoom : public MmDrag
{
	public:
		MmDragZoom( MmWndInfo* pWndInfo, MgPoint2 ptStart);

		bool Continue( MmWndInfo* pWndInfo, MgPoint2 ptLatest);

};
	

class MmDragPan : public MmDrag
{
	public:
		MmDragPan( MmWndInfo* pWndInfo, MgPoint2 ptStart);

		bool Continue( MmWndInfo* pWndInfo, MgPoint2 ptLatest);
};

// _MmDrag_H

} // namespace MC
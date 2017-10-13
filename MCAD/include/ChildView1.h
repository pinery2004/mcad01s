// ChildView1.h : CChildView1 クラスのインターフェイス
//


#pragma once


// CChildView1 ウィンドウ

class CChildView1 : public CWnd
{
// コンストラクション
public:
	CChildView1();

// 属性
public:
//	CMainFrame* m_pMain;

// 操作
public:

// オーバーライド
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 実装
public:
	virtual ~CChildView1();

	// 生成された、メッセージ割り当て関数
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnMyMessagePtsEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMyMessageGridNum(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMyMessageStatusBar(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMyMessageOpt(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMyMessageHist(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMyMessageTenkai(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
//1c	afx_msg void OnCancel1Pt();
//1c	afx_msg void OnUpdateCancel1Pt(CCmdUI *pCmdUI);
//1c	afx_msg void OnCancelAll();
//1c	afx_msg void OnUpdateCancelAll(CCmdUI *pCmdUI);
//1c	afx_msg void OnInpKai();
//1c	afx_msg void OnUpdateInpKai(CCmdUI *pCmdUI);
//1c	afx_msg void OnInpAll();
//1c	afx_msg void OnUpdateInpAll(CCmdUI *pCmdUI);
//1c	afx_msg void OnInpEnd();
//1c	afx_msg void OnUpdateInpEnd(CCmdUI *pCmdUI);
//1c	afx_msg void OnModeCreate();
//1c	afx_msg void OnUpdateModeCreate(CCmdUI *pCmdUI);
//1c	afx_msg void OnModeDelete();
//1c	afx_msg void OnUpdateModeDelete(CCmdUI *pCmdUI);
//1c	afx_msg void OnModeModify();
//1c	afx_msg void OnUpdateModeModify(CCmdUI *pCmdUI);
//1c	afx_msg void OnModeMove();
//1c	afx_msg void OnUpdateModeMove(CCmdUI *pCmdUI);
//1c	afx_msg void OnModeCopy();
//1c	afx_msg void OnUpdateModeCopy(CCmdUI *pCmdUI);
//1c	afx_msg void OnModeProperty();
//1c	afx_msg void OnUpdateModeProperty(CCmdUI *pCmdUI);
	afx_msg void OnZoomInit();
	afx_msg void OnUpdateZoomInit(CCmdUI *pCmdUI);
//1c	afx_msg void OnZoomPan();
//1c	afx_msg void OnUpdateZoomPan(CCmdUI *pCmdUI);
//1c	afx_msg void OnZoomSz();
//1c	afx_msg void OnUpdateZoomSz(CCmdUI *pCmdUI);
//1c	afx_msg void OnUndo();
//1c	afx_msg void OnUpdateUndo(CCmdUI *pCmdUI);
//1c	afx_msg void OnRedo();
//1c	afx_msg void OnUpdateRedo(CCmdUI *pCmdUI);
//1c	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
//1c	afx_msg void OnPtsEdit();
//1c	afx_msg void OnUpdatePtsEdit(CCmdUI *pCmdUI);
//1c	afx_msg void OnGridNum();
//1c	afx_msg void OnUpdateGridNum(CCmdUI *pCmdUI);
//1c	afx_msg void OnOpt();
//1c	afx_msg void OnHist();
//1c	afx_msg void OnTenkai();
//1c	afx_msg void OnChukimove();
//1c	afx_msg void OnUpdateChukimove(CCmdUI *pCmdUI);
//1c	afx_msg void OnChukidel();
//1c	afx_msg void OnUpdateChukidel(CCmdUI *pCmdUI);
//1c	afx_msg void OnChukidlgon();
//1c	afx_msg void OnUpdateChukidlgon(CCmdUI *pCmdUI);
//1c	afx_msg void OnPanelBefore();
//1c	afx_msg void OnUpdatePanelBefore(CCmdUI *pCmdUI);
//1c	afx_msg void OnPanelNext();
//1c	afx_msg void OnUpdatePanelNext(CCmdUI *pCmdUI);
//1c	afx_msg void OnPanelAll();
//1c	afx_msg void OnUpdatePanelAll(CCmdUI *pCmdUI);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnCancelMode();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//C	afx_msg void OnClose();
	afx_msg void OnP0010();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnP0020();
	afx_msg void OnP0030();
	afx_msg void OnP0040();
	afx_msg void OnP0050();
	afx_msg void OnP0060();
	afx_msg void OnP0070();
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
//	afx_msg void OnFilePrintPreview();
//	afx_msg void OnUpdateFilePrintPreview(CCmdUI *pCmdUI);
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
public:
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
public:
	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
};

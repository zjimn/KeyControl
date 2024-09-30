#pragma once
#include <afxwin.h>

class CCenteredEdit : public CEdit {
public:
    CCenteredEdit();

    void SetBkColor(COLORREF color);

    void ResetColor();

    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

    afx_msg void OnMouseMove(UINT nFlags, CPoint point);

    void SetCustomFont(CFont *pFont);

    afx_msg void OnSetFont(CFont *pFont, BOOL bRedraw);

    virtual ~CCenteredEdit();

protected:
    DECLARE_MESSAGE_MAP()
    afx_msg

    void OnPaint();

    afx_msg BOOL OnEraseBkgnd(CDC *pDC);

    afx_msg void OnSetFocus(CWnd *pOldWnd);

    afx_msg void OnKillFocus(CWnd *pNewWnd);

    afx_msg HBRUSH CtlColor(CDC *pDC, UINT nCtlColor);

    virtual void PreSubclassWindow();

private:
    bool m_bHasFocus;
    CBrush m_brush;
    COLORREF m_bgColor;
};

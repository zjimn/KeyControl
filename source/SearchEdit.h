#pragma once

enum {
    FOCUS_DEFAULT, FOCUS_FONTBOLD, FOCUS_SHADOWFOCUS_BKCOLOR
};

class CSearchEdit : public CEdit {
public:
    CSearchEdit();

    ~CSearchEdit();

    HBRUSH CtlColor(CDC *pDC, UINT nCtlColor);

    void SetIconId(UINT idcnId) { m_iConId = idcnId; }
    void SetGrayString(CString strPath) { m_strGrayString = strPath; }

private:
    CRect m_rectEdit;
    CFont m_Font;
    BOOL m_bHover;
    BOOL m_bTrack;
    BOOL m_bUser;
    BOOL m_bEmpty;
    COLORREF m_clrBorderNormal;
    COLORREF m_clrBorderHover;
    int m_nFocusStyle;
    CString m_strGrayString;
    CString m_strFaceName;
    int m_nPointSize;
    UINT m_iConId;
    CRect m_expandedRect;
    int m_expandOffset;

public:
    DECLARE_MESSAGE_MAP()

    afx_msg void OnNcPaint();

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	afx_msg void OnMouseLeave(UINT nFlags, CPoint point);

	afx_msg void OnChar(UINT nchar, UINT nRepCnt, UINT nFlags);

	afx_msg void OnEnChange();

	afx_msg void OnSetFocus(CWnd *pOldWnd);

	afx_msg void OnkillFocus(CWnd *pNewWnd);

	afx_msg void DrawLoupe(CDC *pDC, CPoint pt);

    void DrawLoupe(CDC *pDC, CPoint pt, int width, int height);

    Gdiplus::Bitmap *LoadBitmapFromIconResource(int resourceId);

	afx_msg BOOL OnEraseBkgnd(CDC *pDC);
};

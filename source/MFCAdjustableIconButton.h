#pragma once
#include <afxwin.h>

class CMFCAdjustableIconButton : public CMFCButton {
public:
    CMFCAdjustableIconButton();

    void SetCustomIcon(int nIconResourceID, int nMarginLeft = noSetMarginFlag, int nMarginTop = noSetMarginFlag);

    void SetIconSize(int nWidth, int nHeight);

    void SetText(CString strText);

    void SetButtonTextMargin(int marginLeft);

    void SetButtonTextColor(COLORREF color);

    void SetTextFont(CFont *pFont, BOOL bRedraw = TRUE);

    void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

    void SetHoverState(COLORREF bgColor, COLORREF textColor, UINT iconID, bool reDraw = false);

    void SetClickState(COLORREF bgColor, COLORREF textColor, UINT iconID, bool reDraw = false);

    void SetDefaultState(COLORREF bgColor, COLORREF textColor, UINT iconID, bool reDraw = false);

    static int const noSetMarginFlag = -10000;

protected:
    virtual void OnDraw(CDC *pDC, const CRect &rect, UINT uiState) override;

    DECLARE_MESSAGE_MAP()
    BOOL m_bIsHighlighted;

    BOOL m_bIsClicked;

    afx_msg void OnMouseMove(UINT nFlags, CPoint point);

    afx_msg void OnMouseLeave();

    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

    afx_msg void OnKillFocus(CWnd *pNewWnd);

    afx_msg void OnSetFocus(CWnd *pOldWnd);

private:
    bool m_bIconMarginSetLeft;
    bool m_bIconMarginSetTop;
    bool m_btextMarginSetLeft;
    int m_nIconMarginLeft = -1;
    int m_nIconMarginTop = -1;
    int m_nIconWidth;
    int m_nIconHeight;
    CImageList m_imgList;
    CString m_strText;
    int m_textMarginLeft;
    COLORREF m_textColor = RGB(255, 255, 255);
    CFont m_Font;
    COLORREF m_hoverBgColor = NULL;
    COLORREF m_hoverTextColor = NULL;
    UINT m_hoverIcon;
    COLORREF m_clickBgColor = NULL;
    COLORREF m_clickTextColor = NULL;
    UINT m_clickIcon;
    COLORREF m_defaultBgColor = NULL;
    COLORREF m_defaultTextColor = NULL;
    UINT m_defaultIcon;
};

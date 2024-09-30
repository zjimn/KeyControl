#include "pch.h"
#include "MFCAdjustableIconButton.h"

BEGIN_MESSAGE_MAP(CMFCAdjustableIconButton, CMFCButton)
    ON_WM_PAINT()
    ON_WM_KEYDOWN()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_KILLFOCUS()
    ON_WM_SETFOCUS()
    ON_WM_MOUSELEAVE() 
END_MESSAGE_MAP()

CMFCAdjustableIconButton::CMFCAdjustableIconButton()
    : m_bIconMarginSetLeft(false), m_bIconMarginSetTop(false), m_nIconMarginLeft(10), m_nIconWidth(22),
    m_nIconHeight(22) {
    m_bIsHighlighted = FALSE;
    m_bIsClicked = FALSE;
}

void CMFCAdjustableIconButton::SetCustomIcon(int nIconResourceID, int nMarginLeft, int nMarginTop) {
    if (nMarginLeft != noSetMarginFlag) {
        m_nIconMarginLeft = nMarginLeft;
        m_bIconMarginSetLeft = true;
    }
    if (nMarginTop != noSetMarginFlag) {
        m_nIconMarginTop = nMarginTop;
        m_bIconMarginSetTop = true;
    }
    m_imgList.DeleteImageList();
    if (nIconResourceID != 0) {
        HICON hIcon = (HICON)LoadImage(AfxGetInstanceHandle(),
            MAKEINTRESOURCE(nIconResourceID),
            IMAGE_ICON,
            m_nIconWidth, m_nIconHeight,
            LR_DEFAULTCOLOR);
        if (hIcon) {
            m_imgList.Create(m_nIconWidth, m_nIconHeight, ILC_COLOR32 | ILC_MASK, 1, 1);
            m_imgList.Add(hIcon);
            DestroyIcon(hIcon);
        }
    }
    Invalidate();
}

void CMFCAdjustableIconButton::SetIconSize(int nWidth, int nHeight) {
    m_nIconWidth = nWidth;
    m_nIconHeight = nHeight;
    Invalidate();
}

void CMFCAdjustableIconButton::SetText(CString strText) {
    m_strText = strText;
    Invalidate();
}

void CMFCAdjustableIconButton::OnDraw(CDC* pDC, const CRect& rect, UINT uiState) {
    COLORREF bgColor = m_defaultBgColor;
    UINT hIcon = m_defaultIcon;
    if (m_bIsClicked) {
        bgColor = m_clickBgColor;
        if (m_hoverTextColor) {
            m_textColor = m_clickTextColor;
        }
        hIcon = m_clickIcon;
    }
    else if (m_bIsHighlighted) {
        bgColor = m_hoverBgColor;
        if (m_hoverTextColor) {
            m_textColor = m_hoverTextColor;
        }
        hIcon = m_hoverIcon;
    }
    BYTE r = GetRValue(bgColor);
    BYTE g = GetGValue(bgColor);
    BYTE b = GetBValue(bgColor);
    if (bgColor) {
        pDC->FillSolidRect(rect, bgColor);
    }
    CMFCButton::OnDraw(pDC, rect, uiState);
    if (!m_imgList.GetSafeHandle()) {
        return;
    }
    CRect iconRect;
    if (!m_bIconMarginSetLeft) {
        iconRect.left = (rect.Width() - m_nIconWidth) / 2;
        iconRect.top = (rect.Height() - m_nIconHeight) / 2;
        iconRect.right = iconRect.left + m_nIconWidth;
        iconRect.bottom = iconRect.top + m_nIconHeight;
    }
    else {
        iconRect.left = rect.left + m_nIconMarginLeft;
        iconRect.right = iconRect.left + m_nIconWidth;
        iconRect.bottom = iconRect.top + m_nIconHeight;
        int buttonHeight = rect.Height();
        int iconHeight = iconRect.Height();
        int iconTop = (buttonHeight - iconHeight) / 2;
        iconRect.top = iconTop;
        iconRect.bottom = iconTop + iconHeight;
    }
    if (!m_bIconMarginSetTop) {
        iconRect.top = (rect.Height() - m_nIconHeight) / 2;
        iconRect.bottom = iconRect.top + m_nIconHeight;
    }
    else {
        int iconHeight = iconRect.Height();
        int iconTop = rect.top + m_nIconMarginTop;
        iconRect.top = iconTop;
        iconRect.bottom = iconTop + iconHeight;
    }
    m_imgList.Draw(pDC, 0, iconRect.TopLeft(), ILD_NORMAL);
    CString strText = m_strText;
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(m_textColor);
    CRect textRect = rect;
    if (!m_bIconMarginSetLeft) {
        textRect.left = iconRect.right + 2;
    }
    else {
        textRect.left += m_nIconWidth + 2 * m_bIconMarginSetLeft;
    }
    if (m_btextMarginSetLeft) {
        textRect.left = m_textMarginLeft;
    }
    pDC->DrawText(strText, textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void CMFCAdjustableIconButton::SetButtonTextMargin(int marginLeft) {
    if (marginLeft != noSetMarginFlag) {
        m_btextMarginSetLeft = marginLeft;
    }
    m_textMarginLeft = marginLeft;
}

void CMFCAdjustableIconButton::SetButtonTextColor(COLORREF color) {
    m_textColor = color;
}

void CMFCAdjustableIconButton::SetTextFont(CFont* pFont, BOOL bRedraw) {
    if (pFont) {
        if (m_Font.GetSafeHandle()) {
            m_Font.DeleteObject();
        }
        m_Font.Attach(pFont->GetSafeHandle());
        if (bRedraw) {
            Invalidate();
            UpdateWindow();
        }
    }
}

void CMFCAdjustableIconButton::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
    if (nChar == VK_SPACE) {
        return;
    }
    CButton::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMFCAdjustableIconButton::SetHoverState(COLORREF bgColor, COLORREF textColor, UINT iconID, bool reDraw) {
    m_hoverBgColor = bgColor;
    m_hoverTextColor = textColor;
    m_hoverIcon = iconID;
    if (reDraw) {
        Invalidate();
        UpdateWindow();
    }
}

void CMFCAdjustableIconButton::SetClickState(COLORREF bgColor, COLORREF textColor, UINT iconID, bool reDraw) {
    m_clickBgColor = bgColor;
    m_clickTextColor = textColor;
    m_clickIcon = iconID;
    if (reDraw) {
        Invalidate();
        UpdateWindow();
    }
}

void CMFCAdjustableIconButton::SetDefaultState(COLORREF bgColor, COLORREF textColor, UINT iconID, bool reDraw) {
    m_defaultBgColor = bgColor;
    m_defaultTextColor = textColor;
    m_defaultIcon = iconID;
    if (reDraw) {
        Invalidate();
        UpdateWindow();
    }
}

void CMFCAdjustableIconButton::OnMouseMove(UINT nFlags, CPoint point) {
    if (!m_bIsHighlighted) {
        m_bIsHighlighted = TRUE;
        Invalidate();
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(TRACKMOUSEEVENT);
        tme.dwFlags = TME_LEAVE;
        tme.hwndTrack = m_hWnd;
        _TrackMouseEvent(&tme);
    }
    CMFCButton::OnMouseMove(nFlags, point);
}

void CMFCAdjustableIconButton::OnMouseLeave() {
    if (m_bIsHighlighted) {
        m_bIsHighlighted = FALSE;
        Invalidate();
    }
    CMFCButton::OnMouseLeave();
}

void CMFCAdjustableIconButton::OnLButtonDown(UINT nFlags, CPoint point) {
    m_bIsClicked = TRUE;
    Invalidate();
    CMFCButton::OnLButtonDown(nFlags, point);
}

void CMFCAdjustableIconButton::OnLButtonUp(UINT nFlags, CPoint point) {
    m_bIsClicked = FALSE;
    Invalidate();
    CMFCButton::OnLButtonUp(nFlags, point);
}

void CMFCAdjustableIconButton::OnKillFocus(CWnd* pNewWnd) {
    if (m_bIsHighlighted) {
        m_bIsHighlighted = FALSE;
        Invalidate();
    }
    CMFCButton::OnKillFocus(pNewWnd);
}

void CMFCAdjustableIconButton::OnSetFocus(CWnd* pOldWnd) {
    CMFCButton::OnSetFocus(pOldWnd);
    if (m_bIsHighlighted) {
        Invalidate();
    }
}

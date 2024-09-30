#include "pch.h"
#include "MFCDeleteButton.h"
#include <afxwin.h>

BEGIN_MESSAGE_MAP(CMFCDeleteButton, CMFCButton)
    ON_WM_PAINT()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSELEAVE()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

CMFCDeleteButton::CMFCDeleteButton()
    : m_bMouseHover(FALSE) {
}

void CMFCDeleteButton::OnMouseMove(UINT nFlags, CPoint point) {
    if (!m_bMouseHover) {
        TRACKMOUSEEVENT tme = { 0 };
        tme.cbSize = sizeof(tme);
        tme.dwFlags = TME_HOVER | TME_LEAVE;
        tme.hwndTrack = GetSafeHwnd();
        TrackMouseEvent(&tme);
        m_bMouseHover = TRUE;
        Invalidate();
        UpdateWindow();
    }
    CMFCButton::OnMouseMove(nFlags, point);
}

void CMFCDeleteButton::OnMouseLeave() {
    m_bMouseHover = FALSE;
    Invalidate();
    CMFCButton::OnMouseLeave();
}

void CMFCDeleteButton::OnLButtonDown(UINT nFlags, CPoint point) {
    m_bIsClicked = TRUE;
    Invalidate();
    UpdateWindow();
    CMFCButton::OnLButtonDown(nFlags, point);
}

void CMFCDeleteButton::OnLButtonUp(UINT nFlags, CPoint point) {
    m_bIsClicked = FALSE;
    Invalidate();
    UpdateWindow();
    CMFCButton::OnLButtonUp(nFlags, point);
}

void CMFCDeleteButton::OnPaint() {
    CPaintDC dc(this);
    CRect rect;
    GetClientRect(&rect);
    UINT nState = GetState();
    if (m_bIsClicked) {
        dc.FillSolidRect(rect, RGB(202, 0, 5));
        dc.SetTextColor(RGB(255, 255, 255));
    }
    else if (m_bMouseHover) {
        dc.FillSolidRect(rect, RGB(232, 30, 35));
        dc.SetTextColor(RGB(255, 255, 255));
    }
    else {
        dc.FillSolidRect(rect, RGB(240, 240, 240));
        dc.SetTextColor(RGB(255, 0, 0));
    }
    CFont font;
    LOGFONT lf = { 0 };
    CString fontName = _T("Arial");
    lf.lfHeight = 40;
    _tcscpy_s(lf.lfFaceName, fontName);
    font.CreateFontIndirect(&lf);
    CFont* pOldFont = dc.SelectObject(&font);
    CString strText;
    GetWindowText(strText);
    RECT rectInternal = rect;
    dc.DrawText(strText, &rectInternal, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
    dc.SelectObject(pOldFont);
}

void CMFCDeleteButton::SetTextFont(CFont* pFont, BOOL bRedraw) {
    if (pFont) {
        if (m_TextFont.GetSafeHandle()) {
            m_TextFont.DeleteObject();
        }
        m_TextFont.Attach(pFont->GetSafeHandle());
        if (bRedraw) {
            Invalidate();
            UpdateWindow();
        }
    }
}

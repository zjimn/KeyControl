#include "pch.h"
#include "TransparentBgStatic.h"
#include "ColorSettings.h"

CTransparentBgStatic::CTransparentBgStatic()
    : m_backgroundColor(RGB(255, 255, 255)), m_pFont(nullptr) {
}

CTransparentBgStatic::~CTransparentBgStatic() {
}

BEGIN_MESSAGE_MAP(CTransparentBgStatic, CStatic)
    ON_WM_PAINT()
END_MESSAGE_MAP()

void CTransparentBgStatic::SetBackgroundColor(COLORREF color) {
    m_backgroundColor = color;
    Invalidate();
}

void CTransparentBgStatic::SetCustomFont(CFont* pFont) {
    m_pFont = pFont;
    Invalidate();
}

void CTransparentBgStatic::OnPaint() {
    CPaintDC dc(this);
    CRect rect;
    GetClientRect(&rect);
    CFont* pOldFont = nullptr;
    if (m_pFont != nullptr) {
        pOldFont = dc.SelectObject(m_pFont);
    }
    dc.SetBkMode(TRANSPARENT);
    CString text;
    GetWindowText(text);
    dc.SetTextColor(RGB(255, 255, 255));
    dc.DrawText(text, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    if (m_pFont != nullptr && pOldFont != nullptr) {
        dc.SelectObject(pOldFont);
    }
}

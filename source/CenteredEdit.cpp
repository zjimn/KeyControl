#include "pch.h"
#include "CenteredEdit.h"
#include "ColorSettings.h"

CCenteredEdit::CCenteredEdit() : m_bHasFocus(false), m_bgColor(GetSysColor(COLOR_WINDOW)){
}

CCenteredEdit::~CCenteredEdit(){
}

BEGIN_MESSAGE_MAP(CCenteredEdit, CEdit)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
    ON_WM_SETFOCUS()
    ON_WM_KILLFOCUS()
    ON_WM_CTLCOLOR_REFLECT()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

void CCenteredEdit::OnPaint(){
    CPaintDC dc(this); 
    CString strText;
    GetWindowText(strText);
    CFont font;
    font.CreateFont(25, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Î¢ÈíÑÅºÚ"));               
    CFont* pOldFont = dc.SelectObject(&font);
    CRect rect;
    GetClientRect(&rect);
    dc.FillSolidRect(&rect, m_bgColor);
    dc.SetTextColor(CColorSettings::TextColor);
    dc.SetBkMode(TRANSPARENT);
    dc.DrawText(strText, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

BOOL CCenteredEdit::OnEraseBkgnd(CDC* pDC){
    return TRUE;
}

void CCenteredEdit::OnSetFocus(CWnd* pOldWnd){
    CEdit::OnSetFocus(pOldWnd);
    m_bHasFocus = true;
    m_bgColor = CColorSettings::SelectedKeyColor; 
    Invalidate(); 
}

void CCenteredEdit::OnKillFocus(CWnd* pNewWnd){
    CEdit::OnKillFocus(pNewWnd);
    m_bHasFocus = false;
    m_bgColor = GetSysColor(COLOR_WINDOW); 
    Invalidate(); 
}

HBRUSH CCenteredEdit::CtlColor(CDC* pDC, UINT ){
    pDC->SetBkColor(m_bgColor);
    static CBrush brush(m_bgColor); 
    return (HBRUSH)brush.GetSafeHandle();
}

void CCenteredEdit::SetBkColor(COLORREF color){
    m_bgColor = color;
    Invalidate(); 
}

void CCenteredEdit::ResetColor(){
    m_bgColor = GetSysColor(COLOR_WINDOW);
    Invalidate(); 
}

void CCenteredEdit::OnLButtonDown(UINT nFlags, CPoint point){
    if (m_bHasFocus) {
    }
    else {
        CEdit::OnLButtonDown(nFlags, point);
    }
}

void CCenteredEdit::OnLButtonDblClk(UINT nFlags, CPoint point){
}

void CCenteredEdit::OnMouseMove(UINT nFlags, CPoint point){
}

void CCenteredEdit::SetCustomFont(CFont* pFont){
    SendMessage(WM_SETFONT, (WPARAM)pFont->GetSafeHandle(), TRUE);
}

void CCenteredEdit::PreSubclassWindow(){
    CEdit::PreSubclassWindow(); 
    CFont* pFont = GetParent()->GetFont();
    if (pFont)
    {
        SetFont(pFont);
    }
}

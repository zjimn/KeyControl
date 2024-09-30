#include "pch.h"
#include "ButtonUtils.h"

void CButtonUtils::SetButtonIconAtMarginLeft(CMFCButton* pButton, UINT nIconResourceID, int marginLeft){
    SetButtonIcon(pButton, nIconResourceID, iconWidth, iconHeight, marginLeft);
}

void CButtonUtils::SetButtonIconAtCenter(CMFCButton* pButton, UINT nIconResourceID, int iconWidth, int iconHeight){
    CRect rectButton;
    pButton->GetWindowRect(&rectButton);
    pButton->GetParent()->ScreenToClient(&rectButton);
    int marginTop = (rectButton.Height() - iconHeight) / 2;
    int marginLeft = (rectButton.Width() - iconWidth) / 2;
    SetButtonIcon(pButton, nIconResourceID, iconWidth, iconHeight, marginLeft, marginTop);
}

void CButtonUtils::SetButtonIcon(CMFCButton* pButton, UINT nIconResourceID, int iconWidth, int iconHeight, int marginLeft){
    CRect rectButton;
    pButton->GetWindowRect(&rectButton);
    pButton->GetParent()->ScreenToClient(&rectButton);
    int marginTop = (rectButton.Height() - iconHeight) / 2;
    SetButtonIcon(pButton, nIconResourceID, iconWidth, iconHeight, marginLeft, marginTop);
}

void CButtonUtils::SetButtonIcon(CMFCButton* pButton, UINT nIconResourceID, int iconWidth, int iconHeight, int marginLeft, int marginTop){
    if (pButton == nullptr || nIconResourceID == 0)
        return;
    CRect rectButton;
    pButton->GetWindowRect(&rectButton);
    pButton->GetParent()->ScreenToClient(&rectButton);
    int iconX = rectButton.left +marginLeft + iconWidth;
    int iconY = rectButton.top + marginTop;
    HICON hIcon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(nIconResourceID),
        IMAGE_ICON, iconWidth, iconHeight, LR_DEFAULTCOLOR);
    if (hIcon)
    {
        pButton->SetIcon(hIcon);
    }
}

void CButtonUtils::SetButtonTextColor(CMFCButton* button, COLORREF textColor) {
    SetButtonColor(button, textColor, NULL);
}

void CButtonUtils::SetButtonBgColor(CMFCButton* button, COLORREF bgColor) {
    SetButtonColor(button, NULL, bgColor);
}

void CButtonUtils::SetButtonColor(CMFCButton* button, COLORREF textColor, COLORREF bgColor) {
    button->m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT; 
    if (textColor != NULL) {
        button->SetTextColor(textColor);
    }
        if (bgColor != NULL) {
            button->SetFaceColor(bgColor);
        }
}

void CButtonUtils::SetButtonTextMarginAndCenter(CMFCButton& button, int marginLeft){
    CRect marginRect(marginLeft, 0, 0, 0);
    button.SetTextMargin(marginRect);
    CRect buttonRect;
    button.GetClientRect(&buttonRect);
    int buttonHeight = buttonRect.Height();
    CDC* pDC = button.GetDC();
    CFont* pOldFont = pDC->SelectObject(button.GetFont());
    CString text;
    button.GetWindowText(text);
    CSize textSize = pDC->GetTextExtent(text);
    pDC->SelectObject(pOldFont);
    button.ReleaseDC(pDC);
    int textHeight = textSize.cy;
    int topMargin = (buttonHeight - textHeight) / 2;
    button.SetTextMargin(CRect(marginLeft, topMargin, 0, topMargin));
    button.Invalidate();
    button.UpdateWindow();
}

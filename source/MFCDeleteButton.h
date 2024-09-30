#pragma once

class CMFCDeleteButton : public CMFCButton {
public:
    CMFCDeleteButton();

    void SetTextFont(CFont *pFont, BOOL bRedraw);

protected:
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);

    afx_msg void OnMouseLeave();

    void OnLButtonDown(UINT nFlags, CPoint point);

    void OnLButtonUp(UINT nFlags, CPoint point);

    virtual void OnPaint();

    bool IsFontValid(const CFont *pFont);

    DECLARE_MESSAGE_MAP()

private:
    BOOL m_bMouseHover;
    CFont m_TextFont;
    bool m_bIsClicked;
};

#pragma once

class CTransparentBgStatic : public CStatic {
public:
    CTransparentBgStatic();

    virtual ~CTransparentBgStatic();

    void SetBackgroundColor(COLORREF color);

    void SetCustomFont(CFont *pFont);

protected:
    afx_msg void OnPaint();

    DECLARE_MESSAGE_MAP()

private:
    COLORREF m_backgroundColor;
    CFont *m_pFont;
};

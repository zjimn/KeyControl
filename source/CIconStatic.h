#pragma once
class CCIconStatic : public CStatic {
public:
    CCIconStatic();

    virtual ~CCIconStatic();

    void SetIconResource(UINT nIDResource, int marginLeft = -1, int marginTop = -1);

    void SetIconSize(int width, int height);

protected:
    afx_msg void OnPaint();

    DECLARE_MESSAGE_MAP()

private:
    HICON m_hIcon;
    int m_marginLeft;
    int m_marginTop;
    int m_iconWidth;
    int m_iconHeight;
    UINT m_nIDResource;
};

#include "pch.h"
#include "CIconStatic.h"

CCIconStatic::CCIconStatic():m_hIcon(nullptr), m_marginLeft(-1), m_marginTop(-1), m_iconWidth(32), m_iconHeight(32), m_nIDResource(0){
}

CCIconStatic::~CCIconStatic(){
    if (m_hIcon)
    {
        ::DestroyIcon(m_hIcon);
    }
}

BEGIN_MESSAGE_MAP(CCIconStatic, CStatic)
    ON_WM_PAINT()
END_MESSAGE_MAP()

void CCIconStatic::SetIconResource(UINT nIDResource, int marginLeft, int marginTop)
{
    m_nIDResource = nIDResource;  
    m_marginLeft = marginLeft;
    m_marginTop = marginTop;
    if (m_hIcon)
    {
        ::DestroyIcon(m_hIcon);
    }
    m_hIcon = static_cast<HICON>(::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(m_nIDResource), IMAGE_ICON, m_iconWidth, m_iconHeight, 0));
}

void CCIconStatic::SetIconSize(int width, int height)
{
    m_iconWidth = width;
    m_iconHeight = height;
    if (m_nIDResource != 0)
    {
        if (m_hIcon)
        {
            ::DestroyIcon(m_hIcon);
        }
        m_hIcon = static_cast<HICON>(::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(m_nIDResource), IMAGE_ICON, m_iconWidth, m_iconHeight, 0));
        Invalidate(); 
    }
}

void CCIconStatic::OnPaint()
{
    CPaintDC dc(this); 
    CRect rect;
    GetClientRect(&rect);
    dc.SetBkMode(TRANSPARENT);
    if (m_hIcon)
    {
        int x = (m_marginLeft == -1) ? (rect.Width() - m_iconWidth) / 2 : m_marginLeft;
        int y = (m_marginTop == -1) ? (rect.Height() - m_iconHeight) / 2 : m_marginTop;
        ::DrawIconEx(dc.m_hDC, x, y, m_hIcon, m_iconWidth, m_iconHeight, 0, NULL, DI_NORMAL);
    }
}

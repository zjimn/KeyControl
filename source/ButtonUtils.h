#pragma once
#include <afxwin.h>

class CButtonUtils {
public:
    static void SetButtonIconAtMarginLeft(CMFCButton *pButton, UINT nIconResourceID, int marginLeft);

    static void SetButtonIconAtCenter(CMFCButton *pButton, UINT nIconResourceID, int iconWidth = 22,
                                      int iconHeight = 22);

    static void SetButtonIcon(CMFCButton *pButton, UINT nIconResourceID, int iconWidth = 22, int iconHeight = 22,
                              int marginLeft = 20);

    static void SetButtonIcon(CMFCButton *pButton, UINT nIconResourceID, int iconWidth, int iconHeight, int marginLeft,
                              int marginTop);

    static void SetButtonTextColor(CMFCButton *button, COLORREF textColor);

    static void SetButtonBgColor(CMFCButton *button, COLORREF bgColor);

    static void SetButtonColor(CMFCButton *button, COLORREF textColor, COLORREF bgColor);

    static void SetButtonTextMarginAndCenter(CMFCButton &button, int marginLeft);

    static HICON ChangeIconColor(HICON hIcon, COLORREF newColor);

private:
    CButtonUtils() = default;

    static int const iconWidth = 22;
    static int const iconHeight = 22;
    static int const marginLeft = 20;
};

#pragma once
#ifndef COLOR_SETTINGS_H
#define COLOR_SETTINGS_H
#include <windows.h>

class CColorSettings {
public:
    static COLORREF const MenuBackgroundColor = RGB(255, 255, 255);
    static COLORREF const MenuTextColor = RGB(100, 100, 100);
    static COLORREF const TextColor = RGB(80, 80, 80);
    static COLORREF const SelectedKeyColor = RGB(173, 216, 230);
    static COLORREF const TitleBgColor = RGB(170, 170, 170);
};
#endif

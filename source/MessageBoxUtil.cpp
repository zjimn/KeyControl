#include "pch.h"
#include "MessageBoxUtil.h"

void MessageBoxUtil::Show(HWND hwnd, LPCWSTR message, bool showSound, LPCWSTR title) {
    RECT rect;
    GetClientRect(hwnd, &rect);
    POINT center;
    center.x = rect.left + (rect.right - rect.left) / 2;
    center.y = rect.top + (rect.bottom - rect.top) / 2;
    HWND tempWnd = CreateWindowEx(
        0,
        L"Static",
        L"Temp",
        WS_POPUP,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        hwnd, NULL, GetModuleHandle(NULL), NULL
    );
    if (tempWnd) {
        int msgBoxWidth = 300; 
        int msgBoxHeight = 100; 
        int x = center.x - msgBoxWidth / 2;
        int y = center.y - msgBoxHeight / 2;
        if (showSound) {
            MessageBeep(MB_ICONEXCLAMATION);
        }
        MessageBox(tempWnd, message, title, MB_OK | MB_ICONERROR | MB_TOPMOST | MB_SETFOREGROUND);
        DestroyWindow(tempWnd);
    }
}

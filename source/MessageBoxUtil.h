#pragma once
#ifndef MESSAGEBOXUTIL_H
#define MESSAGEBOXUTIL_H
#include <windows.h>

class MessageBoxUtil {
public:
    static void Show(HWND hwnd, LPCWSTR message, bool showSound, LPCWSTR title = L"����");
};
#endif

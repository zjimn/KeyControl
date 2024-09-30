#pragma once
#ifndef WINDOWUTILS_H
#define WINDOWUTILS_H
#include <windows.h>
#include <vector>
#include <string>

class CWindowUtils {
public:
    static std::vector<HWND> FindAllVisibleNonSystemWindows();

    static std::string GetWindowClassName(HWND hwnd);

    static HICON GetWindowIcon(HWND hWnd);

    static CString GetWindowTitle(HWND hwnd);

    static void ResizeWindow(CWnd *pWnd, int nNewWidth, int nNewHeight);

    static constexpr HWND TopMost = HWND_TOPMOST;
    static constexpr HWND NotTopMost = HWND_NOTOPMOST;
    static constexpr HWND Top = HWND_TOP;
    static constexpr HWND Bottom = HWND_BOTTOM;

private:
    static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);

    static bool IsSystemWindow(std::string className);

    static std::vector<HWND> m_windowHandles;
    static std::vector<HWND> g_windows;
};
#endif

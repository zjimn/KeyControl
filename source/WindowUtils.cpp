#include "pch.h"
#include "WindowUtils.h"
#include <string>
#include <set>

std::vector<HWND> CWindowUtils::m_windowHandles;

BOOL CALLBACK CWindowUtils::EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    std::vector<HWND>* pWindowList = reinterpret_cast<std::vector<HWND>*>(lParam);
    LONG_PTR style = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
    const int classNameSize = 256;
    char className[classNameSize];
    GetClassNameA(hwnd, className, classNameSize);
    std::string classNameStr(className);
    if (IsWindowVisible(hwnd) && !(style & WS_EX_TOOLWINDOW) &&
        !IsSystemWindow(classNameStr)) {
        pWindowList->push_back(hwnd);
    }
    return TRUE;
}

std::vector<HWND> CWindowUtils::FindAllVisibleNonSystemWindows() {
    std::vector<HWND> windowList;
    EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&windowList));
    return windowList;
}

bool CWindowUtils::IsSystemWindow(std::string className) {
    static const std::set<std::string> systemWindowClassNames = {
        "SysListView32",
        "MSTaskListWClass",
        "CabinetWClass",
        "Shell_TrayWnd",
        "Button",
        "ToolbarWindow32",
        "MSSettingsUI",
        "ApplicationFrameWindow",
        "TaskmgrWindow",
        "TaskManagerWindow",
        "Shell_SecondaryTrayWnd",
        "DWMNotificationWindow",
        "ExploreWClass",
        "WorkerW",
        "Progman",
        "Shell_TrayWnd",
        "TrayNotifyWnd",
        "Windows.UI.Core.CoreWindow",
        "TWizardForm",
        "TApplication",
        "TWindowDisabler-Window",
        "KeyControlClass",
        "TCompileForm",
    };
    return systemWindowClassNames.find(className) != systemWindowClassNames.end();
}

std::string CWindowUtils::GetWindowClassName(HWND hwnd) {
    const int classNameSize = 256;
    char className[classNameSize];
    if (GetClassNameA(hwnd, className, classNameSize)) {
        return std::string(className);
    }
    else {
        return "";
    }
}

HICON CWindowUtils::GetWindowIcon(HWND hWnd) {
    HICON hIcon = (HICON)SendMessage(hWnd, WM_GETICON, ICON_BIG, 0);
    if (hIcon == nullptr) {
        hIcon = (HICON)GetClassLongPtr(hWnd, GCLP_HICON);
    }
    if (hIcon == nullptr) {
        hIcon = (HICON)SendMessage(hWnd, WM_GETICON, ICON_SMALL, 0);
    }
    if (hIcon == nullptr) {
        hIcon = (HICON)GetClassLongPtr(hWnd, GCLP_HICONSM);
    }
    return hIcon;
}

CString CWindowUtils::GetWindowTitle(HWND hwnd)
{
    CString windowTitle;
    int length = ::GetWindowTextLength(hwnd);
    if (length > 0)
    {
        ::GetWindowText(hwnd, windowTitle.GetBuffer(length + 1), length + 1);
        windowTitle.ReleaseBuffer();
    }
    return windowTitle;
}

void CWindowUtils::ResizeWindow(CWnd* pWnd, int nNewWidth, int nNewHeight)
{
    if (pWnd == nullptr || !pWnd->GetSafeHwnd())
    {
        return;
    }
    CRect rect;
    pWnd->GetWindowRect(&rect);
    rect.right = rect.left + nNewWidth;
    rect.bottom = rect.top + nNewHeight;
    pWnd->MoveWindow(rect);
}
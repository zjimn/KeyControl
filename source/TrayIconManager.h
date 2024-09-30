#pragma once
#include <shellapi.h>

class CTrayIconManager {
public:
    CTrayIconManager();

    ~CTrayIconManager();

    void Initialize(CWnd *pParentWnd, UINT uID, UINT uCallbackMessage, HICON hIcon, const CString &strTooltip);

    void ShowIcon();

    void HideIcon();

    void UpdateTooltip(const CString &strTooltip);

    void ShowContextMenu();

private:
    NOTIFYICONDATA m_nid;
    BOOL m_bIconVisible;
    CWnd *m_pParentWnd;
};

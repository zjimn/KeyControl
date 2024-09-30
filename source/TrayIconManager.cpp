#include "pch.h"
#include "TrayIconManager.h"
#include "resource.h"

CTrayIconManager::CTrayIconManager() {
    memset(&m_nid, 0, sizeof(NOTIFYICONDATA));
    m_bIconVisible = FALSE;
    m_pParentWnd = nullptr;
}

CTrayIconManager::~CTrayIconManager() {
    HideIcon();
}

void CTrayIconManager::Initialize(CWnd* pParentWnd, UINT uID, UINT uCallbackMessage, HICON hIcon,
    const CString& strTooltip) {
    m_nid.cbSize = sizeof(NOTIFYICONDATA);
    m_nid.hWnd = pParentWnd->GetSafeHwnd();
    m_nid.uID = uID;
    m_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    m_nid.hIcon = hIcon;
    m_nid.uCallbackMessage = uCallbackMessage;
    _tcscpy_s(m_nid.szTip, strTooltip);
    m_pParentWnd = pParentWnd;
    ShowIcon();
}

void CTrayIconManager::ShowIcon() {
    if (!m_bIconVisible) {
        Shell_NotifyIcon(NIM_ADD, &m_nid);
        m_bIconVisible = TRUE;
    }
}

void CTrayIconManager::HideIcon() {
    if (m_bIconVisible) {
        Shell_NotifyIcon(NIM_DELETE, &m_nid);
        m_bIconVisible = FALSE;
    }
}

void CTrayIconManager::UpdateTooltip(const CString& strTooltip) {
    _tcscpy_s(m_nid.szTip, strTooltip);
    if (m_bIconVisible) {
        Shell_NotifyIcon(NIM_MODIFY, &m_nid);
    }
}

void CTrayIconManager::ShowContextMenu() {
    if (!m_pParentWnd)
        return;
    CMenu menu;
    menu.CreatePopupMenu();
    menu.AppendMenu(MF_STRING, ID_TRAY_OPEN, _T("打开"));
    menu.AppendMenu(MF_STRING, ID_TRAY_EXIT, _T("退出"));
    POINT pt;
    GetCursorPos(&pt);
    SetForegroundWindow(m_pParentWnd->GetSafeHwnd());
    menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, m_pParentWnd);
    PostMessage(m_pParentWnd->GetSafeHwnd(), WM_NULL, 0, 0);
}

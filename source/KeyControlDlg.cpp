#include "pch.h"
#include "framework.h"
#include "KeyControlDlg.h"
#include "afxdialogex.h"
#include "SettingsDlg.h"
#include <iostream>
#include <stdexcept>
#include <afx.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <map>
#include "StringUtils.h"
#include <afxwin.h> 
#include <mutex>
#include <Windows.h>
#include "WindowUtils.h"
#include "StringSorter.h"
#include "ButtonUtils.h"
#include "ColorSettings.h"
#include "resource.h"

typedef void (*HAHKFunc)(LPCTSTR , int , int, HWND);

BEGIN_MESSAGE_MAP(CKeyControlDlg, CDialogEx)
	ON_WM_KEYUP()
	ON_BN_CLICKED(IDC_BUTTON_SETTINGS, &CKeyControlDlg::OnBnClickedButtonSettings)
	ON_BN_CLICKED(IDOK, &CKeyControlDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CKeyControlDlg::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_EDIT_INPUT, &CKeyControlDlg::OnEnChangeEditInput)
	ON_LBN_SELCHANGE(IDC_LISTBOX_WINDOWS, &CKeyControlDlg::OnLbnSelchangeWindowsListBox)
	ON_WM_LBUTTONDOWN()
	ON_WM_CTLCOLOR()
	ON_WM_DRAWITEM()
	ON_WM_SETFOCUS()
	ON_MESSAGE(WM_ITEM_CLICKED, &CKeyControlDlg::OnItemClicked)
	ON_WM_HOTKEY()
	ON_MESSAGE(ID_SEND_TRAY_MESSAGE, &CKeyControlDlg::OnTrayIcon)
	ON_MESSAGE(ID_SEND_WINDOW_FRONT_MESSAGE, &CKeyControlDlg::OnAppInstanceMessage)
	ON_COMMAND(ID_TRAY_OPEN, &CKeyControlDlg::OnTrayOpen)
	ON_COMMAND(ID_TRAY_EXIT, &CKeyControlDlg::OnTrayExit)
END_MESSAGE_MAP()

CKeyControlDlg::CKeyControlDlg(CWnd* pParent )
	: CDialogEx(IDD_KEY_CONTROL_DIALOG, pParent)
{
	m_appIcon = AfxGetApp()->LoadIcon(IDC_MYICON);
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKeyControlDlg::DoDataExchange(CDataExchange* pDX) {
    CDialogEx::DoDataExchange(pDX);
}

HHOOK g_hKeyboardHook = NULL;
HHOOK g_hKeyboardUpHook = NULL;
HINSTANCE g_hWindowInstance = NULL;
std::map<WORD, bool> g_keyState;

void CKeyControlDlg::OnLbnSelchangeWindowsListBox() {
    int nSel = m_ListBoxWindows.GetCurSel();
    if (nSel != LB_ERR) {
        HWND currentSelected = m_ListBoxWindows.GetItem(nSel).hwnd;
        HWND preSelected = m_SelectedWindow;
        m_SelectedWindow = currentSelected;
        CKeyControlDlg* pDlg = (CKeyControlDlg*)AfxGetMainWnd();
        if (m_SelectedWindow != m_hWnd) {
            bool isIconic = listBoxPreviousIsIconicMap[m_SelectedWindow];
            ::SetForegroundWindow(pDlg->m_PreviousFocusWindow);
            if (isIconic) {
                ::ShowWindow(m_SelectedWindow, SW_MINIMIZE);
            }
        }
        if (m_SelectedWindow) {
            bool isIconic = ::IsIconic(m_SelectedWindow);
            if (!m_PreSelectedWindow || m_PreSelectedWindow != m_SelectedWindow) {
                if (m_bIsKeyMapSeted) {
                    SetSwitchButtonState(WAIT);
                }
            }
        }
        m_PreSelectedWindow = m_SelectedWindow;
    }
}

void CKeyControlDlg::OnBnClickedButtonSettings() {
    m_HotkeyRegistrar.UnRegisterHotKeys();
    CSettingsDlg* settingsDlg = new CSettingsDlg(IDD_SETTINGS_DIALOG, m_hWnd, m_HotkeyRegistrar, this);
    if (settingsDlg->DoModal() == IDC_SETTINGS_BUTTON_OK) {
        InputKeyValuesGroup inputKeyValuesGroup = settingsDlg->LoadnputKeyValuesGroupData();
        m_HotkeyRegistrar = settingsDlg->GetHotkeyRegistrar();
        m_bIsKeyMapSeted = inputKeyValuesGroup.firstValues.size() > 0;
        SetSettingIconColor(!m_bIsKeyMapSeted);
        if (m_bIsKeyMapSeted && m_buttonState == STOP && m_ListBoxWindows.GetCurSel() != -1) {
            SetSwitchButtonState(WAIT);
        }
        else if (!m_bIsKeyMapSeted) {
            SetSwitchButtonState(STOP);
        }
        m_SettingInputKeyValuesGroup = inputKeyValuesGroup;
    }
    if (m_bIsKeyMapSeted && m_buttonState == RUNNING) {
        m_HotkeyRegistrar.RegisterHotKeys();
    }
    else {
        m_HotkeyRegistrar.UnRegisterHotKeys();
    }
}

void CKeyControlDlg::SetSettingIconColor(bool red) {
    if (red) {
        m_SettingsButton->SetCustomIcon(IDR_RED_SETTING_ICON);
    }
    else {
        m_SettingsButton->SetCustomIcon(IDR_SETTING_ICON);
    }
}

void CKeyControlDlg::OnBnClickedCancel() {
    EndDialog(IDCANCEL);
}

void CKeyControlDlg::OnEnChangeEditInput() {
    if (m_bIsInited) {
        UpdateWindowList();
        UpdateListBoxScrollBar(m_ListBoxWindows);
    }
}

void CKeyControlDlg::UpdateWindowList() {
    CString filterTitle;
    if (m_EditInput) {
        m_EditInput.GetWindowText(filterTitle);
    }
    m_WindowTitles.clear();
    m_WindowHandles.clear();
    std::vector<HWND> windowHwnds = CWindowUtils::FindAllVisibleNonSystemWindows();
    m_ListBoxWindows.ResetContent();
    std::vector<ListItem> windowList;
    for (HWND hwnd : windowHwnds) {
        ListItem listItem;
        listItem.hwnd = hwnd;
        CString titleStr = CWindowUtils::GetWindowTitle(hwnd);
        CStringUtils::Trim(titleStr);
        listItem.title = titleStr;
        windowList.push_back(listItem);
    }
    int selectedIndex = -1;
    CStringSorter::SortWindowList(windowList);
    for (ListItem listItem : windowList) {
        HWND hwnd = listItem.hwnd;
        CString title = listItem.title;
        if (hwnd == m_hWnd) {
            continue;
        }
        if (title.IsEmpty()) {
            continue;
        }
        bool contains = CStringUtils::ContainsCharsInOrder(CString(title), filterTitle);
        if (contains) {
            HICON icon = CWindowUtils::GetWindowIcon(hwnd);
            m_ListBoxWindows.AddWindowItem(hwnd, title, icon);
        }
        else {
            continue;
        }
        if (m_SelectedWindow && m_SelectedWindow == hwnd) {
            selectedIndex = m_ListBoxWindows.GetCount() - 1;
        }
    }
    if (selectedIndex > -1) {
        m_ListBoxWindows.SetCurSel(selectedIndex);
        if (m_buttonState == STOP && m_bIsKeyMapSeted) {
            SetSwitchButtonState(WAIT);
        }
    }
    else {
        m_ListBoxWindows.SetCurSel(-1);
        if (m_buttonState != STOP) {
            SetSwitchButtonState(STOP);
        }
    }
}

void CKeyControlDlg::SetSwitchButtonState(ButtonState state) {
    CButton* pButton = (CButton*)GetDlgItem(IDOK);
    if (state == STOP) {
        m_HotkeyRegistrar.UnRegisterHotKeys();
        pButton->EnableWindow(false);
        CButtonUtils::SetButtonIcon(m_OkButton, IDR_PLAY_ICON);
        m_OkButton->SetHoverState(RGB(204, 204, 204), RGB(255, 255, 255), IDR_PLAY_ICON, true);
        m_OkButton->SetClickState(RGB(204, 204, 204), RGB(255, 255, 255), IDR_QUIT_ICON, true);
        m_OkButton->SetDefaultState(RGB(204, 204, 204), RGB(255, 255, 255), IDR_QUIT_ICON, true);
        m_buttonState = STOP;
    }
    if (state == WAIT) {
        m_HotkeyRegistrar.UnRegisterHotKeys();
        pButton->EnableWindow(true);
        CButtonUtils::SetButtonIcon(m_OkButton, IDR_PLAY_ICON);
        m_OkButton->SetHoverState(RGB(40, 255, 137), RGB(255, 255, 255), IDR_PLAY_ICON, true);
        m_OkButton->SetClickState(RGB(0, 215, 97), RGB(255, 255, 255), IDR_QUIT_ICON, true);
        m_OkButton->SetDefaultState(RGB(20, 235, 117), RGB(255, 255, 255), IDR_QUIT_ICON, true);
        m_buttonState = WAIT;
    }
    if (state == RUNNING) {
        m_HotkeyRegistrar.RegisterHotKeys();
        pButton->EnableWindow(true);
        m_OkButton->SetHoverState(RGB(255, 99, 71), RGB(255, 255, 255), IDR_PLAY_ICON, true);
        m_OkButton->SetClickState(RGB(215, 59, 31), RGB(255, 255, 255), IDR_QUIT_ICON, true);
        m_OkButton->SetDefaultState(RGB(235, 79, 51), RGB(255, 255, 255), IDR_QUIT_ICON, true);
        CButtonUtils::SetButtonIcon(m_OkButton, IDR_STOP_ICON);
        pButton->EnableWindow(true);
        m_buttonState = RUNNING;
    }
}

void CKeyControlDlg::OnBnClickedOk() {
    if (m_buttonState == STOP) {
    }
    if (m_buttonState == WAIT) {
        SetSwitchButtonState(RUNNING);
        if (::IsIconic(m_SelectedWindow)) {
            ::ShowWindow(m_SelectedWindow, SW_SHOWDEFAULT);
        }
        ::SetForegroundWindow(m_SelectedWindow);
        ::SetForegroundWindow(m_hWnd);
    }
    else if (m_buttonState == RUNNING) {
        SetSwitchButtonState(WAIT);
    }
    int selectedIndex = m_ListBoxWindows.GetCurSel();
    if (selectedIndex != LB_ERR) {
        if (m_SelectedWindow) {
            m_IsWindowSelected = TRUE;
        }
        else {
            m_IsWindowSelected = FALSE;
        }
    }
}

void CKeyControlDlg::UpdateListBoxScrollBar(CCustomListBox& listBox) {
    CRect rect;
    listBox.GetClientRect(&rect);
    int itemHeight = listBox.GetItemHeight(0);
    int itemCount = listBox.GetCount();
    int totalHeight = itemHeight * itemCount;
    if (totalHeight > rect.Height()) {
        listBox.ModifyStyle(0, WS_VSCROLL);
    }
    else {
        listBox.ModifyStyle(WS_VSCROLL, 0);
    }
    listBox.Invalidate();
}

BOOL CKeyControlDlg::OnInitDialog() {
    m_EditHidden.Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, CRect(217, 0, 400, 10 + 40), this, IDC_HIDDEN_FOCUS);
    m_EditHidden.ShowWindow(false);
    m_bIsInited = false;
    std::vector<HWND> windows = CWindowUtils::FindAllVisibleNonSystemWindows();
    CDialogEx::OnInitDialog();
    CWindowUtils::ResizeWindow(this, 790, 606);
    CreateSettingAndQuitButton();
    createListBox();
    createEditInput();
    m_TrayIconManager.Initialize(this, IDR_MAINFRAME, ID_SEND_TRAY_MESSAGE, AfxGetApp()->LoadIcon(IDR_MAINFRAME),
        CWindowUtils::GetWindowTitle(m_hWnd));
    m_EditInput.ModifyStyle(WS_BORDER, 0);
    SetIcon(m_appIcon, TRUE);
    SetIcon(m_hIcon, FALSE);
    StartListenKeyUp();
    CSettingsDlg settingsDlg;
    InputKeyValuesGroup inputKeyValuesGroup = settingsDlg.LoadnputKeyValuesGroupData();
    m_bIsKeyMapSeted = inputKeyValuesGroup.firstValues.size() > 0;
    InitCreateAfterOnInitDialog();
    m_SettingInputKeyValuesGroup = inputKeyValuesGroup;
    m_HotkeyRegistrar.RegisterHotKeys(m_hWnd, m_SettingInputKeyValuesGroup);
    CMFCAdjustableIconButton* switchButton = new CMFCAdjustableIconButton();
    switchButton->Create(_T(""), BS_TEXT | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(597, 40, 722, 80), this, IDOK);
    m_OkButton = switchButton;
    SetSwitchButtonState(STOP);
    CButtonUtils::SetButtonIcon(m_OkButton, IDR_PLAY_ICON);
    m_bIsInited = true;
    UpdateWindowList();
    return TRUE;
}

void CKeyControlDlg::createListBox() {
    m_ListBoxWindows.Create(
        WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_OWNERDRAWVARIABLE | LBS_NOTIFY | LBS_HASSTRINGS | WS_VSCROLL |
        WS_HSCROLL,
        CRect(44, 107, 555, 498),
        this,
        IDC_LISTBOX_WINDOWS);
    CFont* font = new CFont();
    font->CreateFont(22, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Microsoft YaHei"));
    m_ListBoxWindows.SetFont(font, 1);
    m_ListBoxWindows.SetParentWnd(this);
}

void CKeyControlDlg::createEditInput() {
    CFont* font = new CFont();
    font->CreateFont(22, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Microsoft YaHei"));
    m_EditInput.Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, CRect(58, 50, 505, 72), this, IDC_EDIT_INPUT);
    m_EditInput.SetFont(font, 1);
}

void CKeyControlDlg::CreateSettingAndQuitButton() {
    CFont* font = new CFont();
    font->CreateFont(17, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("SimHei"));
    m_SettingsButton = new CMFCAdjustableIconButton();
    CMFCAdjustableIconButton* cancelButton = new CMFCAdjustableIconButton();
    m_SettingsButton->Create(_T("   设定"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(596, 107, 723, 147), this,
        IDC_BUTTON_SETTINGS);
    cancelButton->Create(_T("   退出"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(596, 458, 723, 498), this, IDCANCEL);
    m_SettingsButton->SetFont(font, 1);
    m_SettingsButton->SetTextColor(CColorSettings::MenuTextColor);
    m_SettingsButton->SetButtonTextMargin(40);
    m_SettingsButton->SetCustomIcon(IDR_RED_SETTING_ICON, 28, 7);
    cancelButton->SetFont(font, 1);
    cancelButton->SetTextColor(CColorSettings::MenuTextColor);
    cancelButton->SetButtonTextMargin(40);
    cancelButton->SetCustomIcon(IDR_QUIT_ICON, 28, 8);
}

void CKeyControlDlg::OnLButtonDown(UINT nFlags, CPoint point) {
    CRect dlgRect;
    GetClientRect(&dlgRect);
    CPoint dlgRight = dlgRect.BottomRight();
    dlgRight.x += dlgRect.left;
    CRect btnRect;
    m_EditInput.GetWindowRect(&btnRect);
    ScreenToClient(&btnRect);
    int distanceToRight = dlgRight.x - btnRect.right;
    if (point.x >= btnRect.right + 15 && point.x <= btnRect.right + 45 &&
        point.y >= (btnRect.top - 5) && point.y <= (btnRect.bottom + 1)) {
        UpdateWindowList();
    }
    CDialogEx::OnLButtonDown(nFlags, point);
}

void CKeyControlDlg::InitCreateAfterOnInitDialog() {
    m_EditInput.SetIconId(IDR_SEARCH_ICON);
    m_EditInput.PostMessage(WM_KILLFOCUS, 0, 0);
    CFont* settingButtonFont = new CFont();
    settingButtonFont->CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS,
        _T("Microsoft YaHei Heavy"));
    SetSettingIconColor(!m_bIsKeyMapSeted);
}

void CKeyControlDlg::OnDestroy() {
    StopListenKeyUp();
    CDialogEx::OnDestroy();
    m_TrayIconManager.HideIcon();
}

LRESULT CKeyControlDlg::OnItemClicked(WPARAM wParam, LPARAM lParam) {
    HWND hSenderWnd = (HWND)wParam;
    COPYDATASTRUCT* pCDS = (COPYDATASTRUCT*)lParam;
    if (pCDS && pCDS->dwData == 0) {
        if (m_ListBoxWindows.GetCount() == 0) {
            return 0;
        }
        ItemIndexData* pData = (ItemIndexData*)pCDS->lpData;
        CString selectedWindowTitle;
        HWND hwnd = pData->hwnd;
        if (!IsWindow(hwnd)) {
        }
        if (pData->eventType == EventType::MouseDown) {
            if (hwnd != NULL) {
                bool isIconic = ::IsIconic(hwnd);
                if (isIconic) {
                    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
                }
                m_SelectedWindow = hwnd;
                CKeyControlDlg* pDlg = (CKeyControlDlg*)AfxGetMainWnd();
                listBoxPreviousIsIconicMap.insert_or_assign(hwnd, isIconic);
                pDlg->m_PreviousFocusWindow = ::GetForegroundWindow();
                ::SetForegroundWindow(hwnd);
            }
        }
    }
    return 0;
}

LRESULT CKeyControlDlg::OnAppInstanceMessage(WPARAM wParam, LPARAM lParam) {
    ShowWindow(SW_RESTORE);
    SetForegroundWindow();
    return 0;
}

LRESULT CKeyControlDlg::OnTrayIcon(WPARAM wParam, LPARAM lParam) {
    UINT uMsg = (UINT)lParam;
    if (uMsg == WM_RBUTTONDOWN) {
        m_TrayIconManager.ShowContextMenu();
    }
    else if (uMsg == WM_LBUTTONDOWN) {
        ShowWindow(SW_RESTORE);
        SetForegroundWindow();
    }
    return 0;
}

void CKeyControlDlg::OnTrayOpen() {
    ShowWindow(SW_RESTORE);
    SetForegroundWindow();
}

void CKeyControlDlg::OnTrayExit() {
    PostMessage(WM_CLOSE);
}

std::unordered_set<BYTE> g_pressedKeys;

void SimulateKeyEvent(BYTE vkCode, BOOL keyDown) {
    INPUT ip;
    ip.type = INPUT_KEYBOARD;
    ip.ki.wVk = vkCode;
    ip.ki.wScan = 0;
    ip.ki.dwFlags = keyDown ? 0 : KEYEVENTF_KEYUP;
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;
    SendInput(1, &ip, sizeof(INPUT));
}

void RecordPressedKeys() {
    for (int vkCode = 0; vkCode < 256; ++vkCode) {
        if (GetAsyncKeyState(vkCode) & 0x8000) {
            g_pressedKeys.insert(vkCode);
        }
    }
}

void ReleaseKeyState() {
    for (BYTE vkCode : g_pressedKeys) {
        SimulateKeyEvent(vkCode, FALSE);
    }
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void RestoreKeyState() {
    const BYTE specialKeys[] = { VK_SHIFT, VK_CONTROL, VK_MENU, VK_LWIN, VK_RWIN };
    std::unordered_set<BYTE> specialKeysSet(specialKeys, specialKeys + sizeof(specialKeys) / sizeof(BYTE));
    for (BYTE vkCode : g_pressedKeys) {
        if (specialKeysSet.find(vkCode) != specialKeysSet.end()) {
            SimulateKeyEvent(vkCode, TRUE);
        }
    }
    g_pressedKeys.clear();
}

void CKeyControlDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2) {
    if (m_HotKeyPressManager.IsHotKeyPressed(nHotKeyId)) {
        CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
        return;
    }
    m_HotKeyPressManager.AddHotKeyId(nHotKeyId);
    m_PreviousFocusWindow = ::GetForegroundWindow();
    ::SetForegroundWindow(m_SelectedWindow);
    Sleep(50);
    m_hotKeyIdQueue.push(nHotKeyId);
    RecordPressedKeys();
    m_bListenKeyUp = false;
    ReleaseKeyState();
    ProcessMessages();
    SendKey(nHotKeyId, WM_KEYDOWN);
    RestoreKeyState();
    m_bListenKeyUp = true;
    CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}

void ProcessMessages() {
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void CKeyControlDlg::SendKey(UINT nHotKeyId, UINT message) {
    std::vector<UINT> keyCodes;
    m_HotkeyRegistrar.GetHotKeyData().GetHotKeyCodes(nHotKeyId, keyCodes);
    for (UINT keyCode : keyCodes) {
        ::PostMessage(m_SelectedWindow, message, keyCode, 0);
    }
}

static LRESULT CALLBACK KeyboardUpProc(int nCode, WPARAM wParam, LPARAM lParam) {
    CKeyControlDlg* pDlg = (CKeyControlDlg*)AfxGetMainWnd();
    KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)lParam;
    if ((wParam == WM_KEYUP || wParam == WM_SYSKEYUP) && !pDlg->m_hotKeyIdQueue.empty() && pDlg->m_bListenKeyUp) {
        while (!pDlg->m_hotKeyIdQueue.empty()) {
            UINT hotKeyId = pDlg->m_hotKeyIdQueue.front();
            pDlg->m_hotKeyIdQueue.pop();
            PostMessage(pDlg->m_hWnd, WM_USER + 1, 0, 0);
            SimulateKeyEvent(VK_F24, FALSE);
            pDlg->m_HotKeyPressManager.RemoveHotKeyId(hotKeyId);
            if (pDlg->m_hotKeyIdQueue.empty()) {
                ::SetForegroundWindow(pDlg->m_PreviousFocusWindow);
                break;
            }
        }
        return CallNextHookEx(g_hKeyboardUpHook, nCode, wParam, lParam);
    }
    return CallNextHookEx(g_hKeyboardUpHook, nCode, wParam, lParam);
}

void StartListenKeyUp() {
    g_hKeyboardUpHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardUpProc, NULL, 0);
}

void StopListenKeyUp() {
    if (g_hKeyboardUpHook != NULL) {
        UnhookWindowsHookEx(g_hKeyboardUpHook);
        g_hKeyboardUpHook = NULL;
    }
}

void ClearAndSimulateKeyRelease() {
    BYTE keyboardState[256];
    if (GetKeyboardState(keyboardState)) {
        for (int i = 0; i < 256; i++) {
            if (keyboardState[i] & 0x80) {
                keybd_event(i, 0, KEYEVENTF_KEYUP, 0);
            }
        }
        memset(keyboardState, 0, sizeof(keyboardState));
        SetKeyboardState(keyboardState);
    }
}

BOOL CKeyControlDlg::PreTranslateMessage(MSG* pMsg) {
    if (pMsg->message == WM_KEYDOWN) {
        if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) {
            return TRUE;
        }
    }
    return CWnd::PreTranslateMessage(pMsg);
}

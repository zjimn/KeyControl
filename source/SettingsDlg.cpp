#include "pch.h"
#include "afxdialogex.h"
#include <afxwin.h> 
#include "CenteredEdit.h"
#include "ConfigFileHandler.h"
#include <algorithm>
#include "KeyboardUtils.h"
#include "StringUtils.h"
#include "CIconStatic.h"
#include "MessageBoxUtil.h"
#include "ColorSettings.h"
#include "WindowUtils.h"
#include "SettingsDlg.h"
#include "resource.h"

IMPLEMENT_DYNAMIC(CSettingsDlg, CDialogEx)

CSettingsDlg::CSettingsDlg(UINT nIDTemplate, HWND hwndA, CHotkeyRegistrar hotkeyRegistrar, CWnd* pParent)
    : CDialogEx(nIDTemplate, pParent), m_parentHwnd(hwndA), m_pActiveEdit(nullptr)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_SETTING_FOR_DLG_ICON);
    m_HotkeyRegistrar = hotkeyRegistrar;
}

CSettingsDlg::CSettingsDlg(CWnd* pParent )
    : CDialogEx(IDD_SETTINGS_DIALOG, pParent), m_pActiveEdit(nullptr)
{
}

CSettingsDlg::~CSettingsDlg()
{
    for (auto pEdit : m_InputEdits)
    {
        delete pEdit;
    }
}

void CSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_BUTTON_ADD, m_AddButton);
}

BEGIN_MESSAGE_MAP(CSettingsDlg, CDialogEx)
    ON_CONTROL_RANGE(EN_CHANGE, IDC_EDIT_INPUT_BASE, IDC_EDIT_INPUT_BASE + 99, &CSettingsDlg::OnEnChangeInputBox)
    ON_CONTROL_RANGE(EN_SETFOCUS, IDC_EDIT_INPUT_BASE, IDC_EDIT_INPUT_BASE + 99, &CSettingsDlg::OnEditSetFocus)
    ON_CONTROL_RANGE(BN_CLICKED, IDC_GROUP_DELETE_BUTTON_BASE, IDC_GROUP_DELETE_BUTTON_BASE + 99, &CSettingsDlg::OnBnClickedGroupDeleteButton)
    ON_BN_CLICKED(IDC_BUTTON_ADD, &CSettingsDlg::OnBnClickedAddButton)
    ON_BN_CLICKED(IDC_SETTINGS_BUTTON_OK, &CSettingsDlg::OnBnClickedOk)
    ON_WM_CLOSE()
    ON_WM_KEYDOWN()
    ON_WM_SIZING()
    ON_WM_SIZE()
    ON_WM_CTLCOLOR()
    ON_WM_DRAWITEM()
    ON_WM_TIMER()
    ON_WM_HOTKEY()
    ON_WM_MOVE()
    ON_WM_VSCROLL()
    ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

BOOL CSettingsDlg::OnInitDialog() {
    m_inited = false;
    CRect rect(0, 0, 100, 50);
    WORD style = static_cast<WORD>(WS_CHILD | WS_VISIBLE | SS_LEFT);
    m_EditHidden.Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, CRect(217, 0, 400, 10 + 40), this, IDC_HIDDEN_FOCUS);
    m_EditHidden.ShowWindow(false);
    CreateSaveButton();
    CreateTitleBg();
    CreateScrollablePane();
    CreateAddButton();
    CDialogEx::OnInitDialog();
    m_OpenToolTip.Create(this);
    m_OpenToolTip.Activate(TRUE);
    m_screenHeight = GetSystemMetrics(SM_CYSCREEN);
    ModifyStyleEx(WS_EX_CLIENTEDGE, 0);
    CWindowUtils::ResizeWindow(this, 802, 610);
    ModifyStyle(0, WS_VSCROLL);
    EnableScrollBarCtrl(SB_VERT, FALSE);
    SetScrollRange(SB_VERT, 0, 3000);
    SetScrollPos(SB_VERT, 0);
    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);
    m_Font.CreateFont(25, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Microsoft YaHei Bold"));
    deleteButtonFont = new CFont();
    deleteButtonFont->CreateFont(25, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS,
        _T("Arial"));
    m_bInInit = true;
    SetKeyValuesGroupDataForDisplay(LoadnputKeyValuesGroupData());
    m_bInInit = false;
    isSettingKey = false;
    if (m_InputGroups.size() == 0) {
        for (int i = 0; i < 4; ++i) {
            AddInputGroup();
        }
    }
    SetMyScrollRange();
    m_inited = true;
    return TRUE;
}

void CSettingsDlg::ShowVerticalScrollBar(bool bShow) {
    LONG lStyle = GetWindowLong(GetSafeHwnd(), GWL_STYLE);
    if (bShow) {
        lStyle |= WS_VSCROLL;
    }
    else {
        lStyle &= ~WS_VSCROLL;
    }
    SetWindowLong(GetSafeHwnd(), GWL_STYLE, lStyle);
    SetWindowPos(nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER | SWP_FRAMECHANGED);
}

BOOL CSettingsDlg::PreTranslateMessage(MSG* pMsg) {
    if (pMsg->message == WM_SYSKEYDOWN || pMsg->message == WM_KEYDOWN) {
        OnKeyDown((UINT)pMsg->wParam, HIWORD(pMsg->lParam), LOWORD(pMsg->lParam));
        return TRUE;
    }
    else if (pMsg->message == WM_SYSKEYUP || pMsg->message == WM_KEYUP) {
        OnKeyUp((UINT)pMsg->wParam, HIWORD(pMsg->lParam), LOWORD(pMsg->lParam));
        return TRUE;
    }
    if (pMsg->message == WM_MOUSEMOVE) {
        m_OpenToolTip.RelayEvent(pMsg);
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}

void CSettingsDlg::CreateScrollablePane() {
}

void CSettingsDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
    int nScrollPos = GetScrollPos(SB_VERT);
    bool hitScrollBar = false;
    switch (nSBCode) {
    case SB_LINEUP:
        nScrollPos = GetScrollPos(SB_VERT) - 60;
        hitScrollBar = true;
        break;
    case SB_LINEDOWN:
        nScrollPos = GetScrollPos(SB_VERT) + 60;
        hitScrollBar = true;
        break;
    case SB_PAGEUP:
        nScrollPos = GetScrollPos(SB_VERT) - 60 * 4;
        hitScrollBar = true;
        break;
    case SB_PAGEDOWN:
        nScrollPos = GetScrollPos(SB_VERT) + 60 * 4;
        hitScrollBar = true;
        break;
    case SB_THUMBPOSITION:
    case SB_THUMBTRACK:
        nScrollPos = nPos;
        break;
    }
    int nMinPos, nMaxPos;
    GetScrollRange(SB_VERT, &nMinPos, &nMaxPos);
    nScrollPos = max(nMinPos, min(nMaxPos, nScrollPos));
    if (IsScrollBarEnabled(GetSafeHwnd(), SB_VERT)) {
        SetScrollPos(SB_VERT, nScrollPos);
        m_nScrollPos = nScrollPos;
        UpdateLayout(SCROLL);
        RefreshWindow();
    }
    else {
        m_nScrollPos = 0;
        UpdateLayout(SCROLL);
    }
    if (hitScrollBar) {
        RefreshWindow();
    }
    CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CSettingsDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {
    int nScrollPos = GetScrollPos(SB_VERT);
    if (zDelta > 0) {
        nScrollPos -= 10;
    }
    else if (zDelta < 0) {
        nScrollPos += 10;
    }
    int nMinPos, nMaxPos;
    GetScrollRange(SB_VERT, &nMinPos, &nMaxPos);
    nScrollPos = max(nMinPos, min(nMaxPos, nScrollPos));
    if (IsScrollBarEnabled(GetSafeHwnd(), SB_VERT)) {
        SetScrollPos(SB_VERT, nScrollPos);
        m_nScrollPos = nScrollPos;
        UpdateLayout(SCROLL);
    }
    return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}

void CSettingsDlg::RefreshWindow() {
    RECT rect;
    ::GetClientRect(m_hWnd, &rect);
    this->InvalidateRect(&rect, TRUE);
    this->UpdateWindow();
}

void CSettingsDlg::CreateTitleBg() {
    CFont* pFont = new CFont();
    pFont->CreateFont(26, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Microsoft YaHei"));
    CRect backgroundRect(50, 40, 550, 80);
    m_staticBackground.Create(_T(""), WS_CHILD | WS_VISIBLE | SS_NOTIFY, backgroundRect, this, IDC_TITLE_STATIC_ID);
    m_staticInput.Create(_T("Hotkey"), WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(48, 0, 153, 40), &m_staticBackground);
    m_staticMap.Create(_T("Map Key"), WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(348, 0, 448, 40), &m_staticBackground);
    m_staticInput.SetCustomFont(pFont);
    m_staticMap.SetCustomFont(pFont);
    HICON hIcon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_KEY_MAP_ICON),
        IMAGE_ICON, 22, 22, LR_DEFAULTCOLOR);
    if (hIcon) {
        m_staticInput.SetIcon(hIcon);
    }
}

void CSettingsDlg::CreateAddButton() {
    CFont* pFont = new CFont();
    pFont->CreateFont(26, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Microsoft YaHei"));
    m_AddButton.Create(_T(""), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(480, 0, 540, 20), this, IDC_BUTTON_ADD);
    m_AddButton.SetFont(pFont);
    m_AddButton.SetTextColor(RGB(255, 255, 255));
    m_AddButton.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
    m_AddButton.SetFaceColor(RGB(99, 244, 119));
    m_AddButton.SetCustomIcon(IDR_ADD_ICON, CMFCAdjustableIconButton::noSetMarginFlag, 6);
    m_AddButton.SetHoverState(RGB(109, 254, 129), RGB(255, 255, 255), IDR_PLAY_ICON);
    m_AddButton.SetClickState(RGB(89, 234, 109), RGB(255, 255, 255), IDR_QUIT_ICON);
    m_AddButton.SetDefaultState(RGB(99, 244, 119), RGB(255, 255, 255), IDR_QUIT_ICON);
}

void CSettingsDlg::CreateSaveButton() {
    m_SaveButton.Create(_T("Save"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(620, 40, 730, 80), this,
        IDC_SETTINGS_BUTTON_OK);
    CFont* pFont = new CFont();
    pFont->CreateFont(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Microsoft YaHei"));
    m_SaveButton.SetFont(pFont);
    m_SaveButton.SetTextColor(RGB(255, 255, 255));
    m_SaveButton.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
    m_SaveButton.SetFaceColor(RGB(0, 191, 255));
    m_SaveButton.SetHoverState(RGB(20, 201, 255), RGB(255, 255, 255), IDR_PLAY_ICON);
    m_SaveButton.SetClickState(RGB(0, 181, 235), RGB(255, 255, 255), IDR_QUIT_ICON);
    m_SaveButton.SetDefaultState(RGB(10, 191, 245), RGB(255, 255, 255), IDR_QUIT_ICON);
}

InputGroup CSettingsDlg::AddInputGroup() {
    int yPosition = 40;
    if (m_InputGroups.size() > 0) {
        CRect buttonRect;
        m_AddButton.GetWindowRect(&buttonRect);
        ScreenToClient(&buttonRect);
        yPosition = buttonRect.top;
    }
    return AddInputGroup(yPosition);
}

InputGroup CSettingsDlg::AddInputGroup(int yPosition) {
    InputGroup group;
    const int spacing = 30;
    int offset = getNextIdOffset();
    CString strMessage;
    group.pEdit1 = new CCenteredEdit();
    int id = IDC_EDIT_INPUT_BASE + (offset * 2);
    group.pEdit1->Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, CRect(200, yPosition, 200, yPosition + 40), this, id);
    group.pEdit1->SetCustomFont(&m_Font);
    group.pEdit1->ModifyStyle(0, ES_CENTER);
    group.pEdit1->SetReadOnly(TRUE);
    id++;
    group.pEdit2 = new CCenteredEdit();
    group.pEdit2->Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, CRect(400, yPosition, 400, yPosition + 40), this, id);
    group.pEdit2->SetFont(&m_Font);
    group.pEdit2->ModifyStyle(0, ES_CENTER);
    group.pEdit2->SetReadOnly(TRUE);
    group.pIcon = new CCIconStatic();
    id = IDC_KEY_MAP_ICON_BASE + offset;
    group.pIcon->Create(_T(""), WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(312, yPosition, 312, yPosition + 30), this,
        id);
    group.pIcon->SetIconResource(IDR_KEY_MAP_ICON, -1, 5);
    group.pIcon->SetIconSize(30, 30);
    id = IDC_TOGGLE_BUTTON_BASE + offset;
    group.pToggleOpenFlagButton = new CButton();
    group.pToggleOpenFlagButton->Create(_T(""), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
        CRect(470, yPosition, 470, yPosition + 20), this, id);
    group.pToggleOpenFlagButton->SetCheck(0);
    m_OpenToolTip.AddTool(group.pToggleOpenFlagButton, _T(" enable this?"));
    group.isOpen = -1;
    id = IDC_GROUP_DELETE_BUTTON_BASE + offset;
    group.pDeleteButton = new CMFCDeleteButton();
    group.pDeleteButton->Create(_T("×"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        CRect(540, yPosition, 540, yPosition + 20), this, id);
    group.pDeleteButton->SetFont(deleteButtonFont, 1);
    group.pDeleteButton->SetTextFont(deleteButtonFont, 1);
    group.pDeleteButton->SetTextColor(RGB(255, 99, 71));
    group.pDeleteButton->m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
    group.pDeleteButton->ModifyStyle(0, BS_OWNERDRAW);
    m_InputGroups.push_back(group);
    UpdateLayout(ADD_ITEM);
    return group;
}

int CSettingsDlg::getNextIdOffset() {
    std::vector<InputGroup> inputGroups = m_InputGroups;
    int currentId = IDC_TOGGLE_BUTTON_BASE;
    int offset = 0;
    std::vector<int> ids;
    for (auto& group : inputGroups) {
        ids.push_back(group.pToggleOpenFlagButton->GetDlgCtrlID());
    }
    std::sort(ids.begin(), ids.end(), [](const int& id1, const int& id2) {
        return id1 < id2;
        });
    for (int i = 0; i < inputGroups.size(); i++) {
        currentId = IDC_TOGGLE_BUTTON_BASE + i;
        int id = ids[i];
        if (id > currentId) {
            return currentId - IDC_TOGGLE_BUTTON_BASE;
        }
        if (i == inputGroups.size() - 1) {
            offset = id - IDC_TOGGLE_BUTTON_BASE + 1;
        }
    }
    return offset;
}

void CSettingsDlg::DeleteInputGroup(int index) {
    if (index >= 0 && index < m_InputGroups.size()) {
        InputGroup& group = m_InputGroups[index];
        delete group.pEdit1;
        delete group.pEdit2;
        delete group.pIcon;
        delete group.pToggleOpenFlagButton;
        delete group.pDeleteButton;
        m_InputGroups.erase(m_InputGroups.begin() + index);
        SetMyScrollRange();
        UpdateLayout(DELETE_ITEM);
    }
    else {
        CString strMessage;
        strMessage.Format(_T("%d "), index);
        OutputDebugString(strMessage);
    }
}

void CSettingsDlg::SetWindowSize(int height) {
    CRect rect;
    GetWindowRect(&rect);
    int newHeight = height;
    int newWidth = rect.Width();
    SetWindowPos(NULL, rect.left, rect.top, newWidth, newHeight, SWP_NOZORDER | SWP_NOMOVE | SWP_NOCOPYBITS);
}

void CSettingsDlg::UpdateLayout(UpdateState updateState = ADD_ITEM) {
    bool showScrollBar = ShowMyScrollBar();
    if (!m_scrollBarEnable && m_preScrollBarState) {
        m_nScrollPos = 0;
    }
    int ypos = -m_nScrollPos + 40;
    m_staticBackground.MoveWindow(50, ypos, 500, 40);
    m_SaveButton.MoveWindow(620, ypos, 110, 40);
    int yPosition = -m_nScrollPos + 120;
    for (int i = 0; i < m_InputGroups.size(); ++i) {
        InputGroup& group = m_InputGroups[i];
        group.pEdit1->MoveWindow(50, yPosition, 200, 40);
        group.pEdit2->MoveWindow(350, yPosition, 200, 40);
        group.pIcon->MoveWindow(285, yPosition, 30, 30);
        group.pToggleOpenFlagButton->MoveWindow(620, yPosition, 40, 40);
        group.pDeleteButton->MoveWindow(690, yPosition, 40, 40);
        yPosition += 60;
    }
    if (m_AddButton) {
        m_AddButton.MoveWindow(50, yPosition + 20, 680, 40);
    }
    CRect buttonRect;
    m_AddButton.GetWindowRect(&buttonRect);
    int height = yPosition + buttonRect.Height() + 150;
    if (!m_scrollBarEnable && m_preScrollBarState || (updateState == DELETE_ITEM && m_scrollBarEnable)) {
        RefreshWindow();
    }
    if (!showScrollBar) {
        SetWindowSize(height < 610 ? 610 : height);
    }
}

bool CSettingsDlg::IsScrollBarEnabled(HWND hwnd, int nBar) {
    SCROLLBARINFO sbi;
    sbi.cbSize = sizeof(SCROLLBARINFO);
    if (::GetScrollBarInfo(hwnd, OBJID_VSCROLL, &sbi)) {
        if (!(sbi.rgstate[0] & STATE_SYSTEM_INVISIBLE) && !(sbi.rgstate[0] & STATE_SYSTEM_UNAVAILABLE)) {
            return true;
        }
    }
    if (!::IsWindowVisible(hwnd)) {
        return false;
    }
    return false;
}

bool CSettingsDlg::ShowMyScrollBar() {
    bool showScrollBar = m_InputGroups.size() * 60 + 260 > m_screenHeight * m_displayScrollWhenToScreenPercent;
    return showScrollBar;
}

void CSettingsDlg::SetMyScrollRange() {
    CRect buttonRect;
    m_AddButton.GetWindowRect(&buttonRect);
    ScreenToClient(&buttonRect);
    int contentHeight = static_cast<int>(m_InputGroups.size()) * 60 + 240;
    CRect clientRect;
    GetClientRect(&clientRect);
    int windowHeight = clientRect.Height();
    int scrollRange = contentHeight - windowHeight;
    SetScrollRange(SB_VERT, 0, scrollRange, FALSE);
    int nMinPos, nMaxPos;
    GetScrollRange(SB_VERT, &nMinPos, &nMaxPos);
    int nScrollPos = max(nMinPos, min(nMaxPos, m_nScrollPos));
    SetScrollPos(SB_VERT, nScrollPos);
    m_nScrollPos = nScrollPos;
    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
    si.nMin = 0;
    si.nMax = contentHeight - 1;
    si.nPage = windowHeight;
    m_preScrollBarState = m_scrollBarEnable;
    m_scrollBarEnable = si.nMax > static_cast<int>(si.nPage);
    si.nPos = m_nScrollPos;
    SetScrollInfo(SB_VERT, &si, SIF_PAGE);
}

void CSettingsDlg::OnBnClickedGroupDeleteButton(UINT nID) {
    int index = 0;
    for (const auto& group : m_InputGroups) {
        int id = group.pDeleteButton->GetDlgCtrlID();
        if (id == nID) {
            break;
        }
        index++;
    }
    if (index >= 0 && index < m_InputGroups.size()) {
        DeleteInputGroup(index);
    }
}

void CSettingsDlg::OnBnClickedAddButton() {
    CRect buttonRect;
    m_AddButton.GetWindowRect(&buttonRect);
    ScreenToClient(&buttonRect);
    ScreenToClient(&buttonRect);
    AddInputGroup(buttonRect.top);
    SetMyScrollRange();
}

void CSettingsDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
    CString currentText;
    if (m_pActiveEdit != nullptr && m_pActiveEdit->GetSafeHwnd() != nullptr) {
        UINT controlID = m_pActiveEdit->GetDlgCtrlID();
        bool filter = (controlID - IDC_EDIT_INPUT_BASE) % 2 == 0;
        m_pActiveEdit->GetWindowText(currentText);
        m_KeyStates[nChar] = true;
        RecordKey(nChar, TRUE, filter);
    }
}

void CSettingsDlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {
    if (m_pActiveEdit) {
        m_KeyStates[nChar] = false;
        RecordKey(nChar, FALSE, false);
        CheckInputs(m_pActiveEdit->GetDlgCtrlID());
    }
}

void CSettingsDlg::RecordKey(UINT nChar, BOOL isKeyDown, BOOL filter) {
    CString keyStr;
    if (isKeyDown) {
        if (!isSettingKey) {
            CString currentText;
            m_pActiveEdit->SetWindowText(currentText);
            isSettingKey = true;
        }
        auto it = std::find(m_PressedKeys.begin(), m_PressedKeys.end(), nChar);
        if (it != m_PressedKeys.end()) {
            CString pressedKeyStr;
            m_PressedKeys = FilterNoPressedKeys(m_PressedKeys);
            m_PressedKeys = FilterPressedKeys(m_PressedKeys, filter);
            m_PressedKeys = FilterPressedExceptKeys(m_PressedKeys);
            m_PressedKeys = FilterFullPressedExceptKeys(m_PressedKeys);
            for (UINT key : m_PressedKeys) {
                pressedKeyStr.Append((CString)"  ");
                CString keyStr = KeyboardUtils::KeyCodeToString(key);
                pressedKeyStr.Append(keyStr);
            }
            pressedKeyStr.Trim();
            m_pActiveEdit->SetWindowText(pressedKeyStr);
            return;
        }
        keyStr = KeyboardUtils::KeyCodeToString(nChar);
        if (m_pActiveEdit) {
            m_PressedKeys.push_back(nChar);
            for (int key : m_PressedKeys) {
                TRACE("key: %d \n", key);
            }
            m_PressedKeys = FilterNoPressedKeys(m_PressedKeys);
            m_PressedKeys = FilterPressedKeys(m_PressedKeys, filter);
            m_PressedKeys = FilterPressedExceptKeys(m_PressedKeys);
            m_PressedKeys = FilterFullPressedExceptKeys(m_PressedKeys);
            CString pressedKeyStr;
            for (UINT key : m_PressedKeys) {
                pressedKeyStr.Append((CString)"  ");
                CString keyStr = KeyboardUtils::KeyCodeToString(key);
                pressedKeyStr.Append(keyStr);
            }
            pressedKeyStr.Trim();
            m_pActiveEdit->SetWindowText(pressedKeyStr);
            m_KeyStates[nChar] = isKeyDown;
        }
    }
    else {
        if (m_pActiveEdit) {
            isSettingKey = false;
            CString currentText;
            m_PressedKeys.erase(std::remove(m_PressedKeys.begin(), m_PressedKeys.end(), nChar), m_PressedKeys.end());
        }
    }
    if (m_pActiveEdit) {
        if (m_PressedKeys.size() > 0) {
            ::HideCaret(m_pActiveEdit->GetSafeHwnd());
        }
        else {
            ::ShowCaret(m_pActiveEdit->GetSafeHwnd());
        }
    }
}

std::vector<int> CSettingsDlg::FilterPressedExceptKeys(const std::vector<int>& m_PressedKeys) {
    const std::vector<int> exceptKeys = {
        KeyboardUtils::UNDEFINED_KEYCODE,
    };
    std::vector<int> result;
    for (int i = 0; i < m_PressedKeys.size(); i++) {
        int key = m_PressedKeys[i];
        key = KeyboardUtils::ChangeKeyCode(key);
        auto it = std::find(exceptKeys.begin(), exceptKeys.end(), key);
        if (it != exceptKeys.end()) {
            continue;
        }
        result.push_back(key);
    }
    return result;
}

std::vector<int> CSettingsDlg::FilterFullPressedExceptKeys(const std::vector<int>& m_PressedKeys) {
    const std::vector<std::vector<int> > exceptKeys = {
        {VK_LWIN},
        {VK_RWIN},
        {VK_MENU, VK_F4},
        {VK_MENU, VK_TAB},
        {VK_CONTROL,VK_MENU, VK_DELETE},
        {VK_MENU, VK_CONTROL,VK_DELETE},
        {VK_CONTROL,VK_ESCAPE},
        {VK_LWIN, 76},
        {VK_RWIN, 76},
        {VK_RWIN, 69},
        {VK_RWIN, 82}
    };
    std::vector<int> pressedKeys = m_PressedKeys;
    std::sort(pressedKeys.begin(), pressedKeys.end());
    std::vector<int> empty;
    for (int i = 0; i < m_PressedKeys.size(); i++) {
        for (std::vector<int> keys : exceptKeys) {
            if (pressedKeys == keys) {
                return empty;
            }
        }
    }
    return m_PressedKeys;
}

std::vector<int> CSettingsDlg::FilterPressedExceptLastKeys(const std::vector<int>& m_PressedKeys) {
    const std::vector<int> exceptKeys = {
    };
    std::vector<int> result;
    for (int i = 0; i < m_PressedKeys.size(); i++) {
        int key = m_PressedKeys[i];
        key = KeyboardUtils::ChangeKeyCode(key);
        if (i == m_PressedKeys.size() - 1) {
            auto it = std::find(exceptKeys.begin(), exceptKeys.end(), key);
            if (it != exceptKeys.end()) {
                break;
            }
        }
        result.push_back(key);
    }
    return result;
}

std::vector<int> CSettingsDlg::FilterNoPressedKeys(const std::vector<int>& m_PressedKeys) {
    std::vector<int> result;
    for (int i = 0; i < m_PressedKeys.size(); i++) {
        int key = m_PressedKeys[i];
        BYTE keyState[256];
        if (GetKeyboardState(keyState)) {
            if ((keyState[key] & 0x80) == 0) {
                continue;
            }
        }
        result.push_back(key);
    }
    return result;
}

std::vector<int> CSettingsDlg::FilterPressedKeys(const std::vector<int>& m_PressedKeys, bool filter) {
    if (!filter) {
        return m_PressedKeys;
    }
    const std::vector<int> specialKeys = {
        VK_SHIFT,
        VK_CONTROL,
        VK_MENU,
        VK_LWIN,
        VK_RWIN
    };
    const std::vector<int> exceptFirstKeys = {
    };
    const std::vector<int> exceptKeys = {
        KeyboardUtils::UNDEFINED_KEYCODE
    };
    std::vector<int> result;
    for (int i = 0; i < m_PressedKeys.size(); i++) {
        int key = m_PressedKeys[i];
        if (i == 0) {
            auto it = std::find(exceptFirstKeys.begin(), exceptFirstKeys.end(), key);
            if (it != exceptFirstKeys.end()) {
                break;
            }
        }
        auto it = std::find(specialKeys.begin(), specialKeys.end(), key);
        if (it != specialKeys.end()) {
            result.push_back(key);
        }
        else {
            result.push_back(key);
            break;
        }
    }
    return result;
}

HBRUSH CSettingsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
    if (nCtlColor == CTLCOLOR_STATIC) {
        if (pWnd->GetDlgCtrlID() == IDC_TITLE_STATIC_ID) {
            pDC->SetBkColor(CColorSettings::TitleBgColor);
            static CBrush brush(CColorSettings::TitleBgColor);
            return brush;
        }
    }
    return hbr;
}

CString CSettingsDlg::getConvertKeyStr(UINT nChar) {
    CString keyStr;
    switch (nChar) {
    case VK_SHIFT:
        keyStr = _T("Shift");
        break;
    case VK_CONTROL:
        keyStr = _T("Ctrl");
        break;
    case VK_MENU:
        keyStr = _T("Alt");
        break;
    case VK_SPACE:
        keyStr = _T("Space");
        break;
    default:
        keyStr.Format(_T("%c"), nChar);
        break;
    }
    return keyStr;
}

void CSettingsDlg::OnEnChangeInputBox(UINT nID) {
    if (!m_bInInit) {
        int index = 0;
        for (const auto& group : m_InputGroups) {
            int id1 = group.pEdit1->GetDlgCtrlID();
            int id2 = group.pEdit2->GetDlgCtrlID();
            if (id1 == nID || id2 == nID) {
                break;
            }
            index++;
        }
        InputGroup group = m_InputGroups[index];
        if (group.isOpen == -1) {
            group.pToggleOpenFlagButton->SetCheck(1);
            group.isOpen = 1;
            m_InputGroups[index] = group;
        }
    }
}

void CSettingsDlg::OnEditSetFocus(UINT nID) {
    CEdit* pEdit = (CEdit*)GetDlgItem(nID);
    if (pEdit) {
        CString text;
        m_pActiveEdit = pEdit;
        if (text.IsEmpty()) {
            OutputDebugString(_T("Edit box set focus: "));
            OutputDebugString(_T("\n"));
        }
        m_KeyStates.clear();
        ::HideCaret(m_pActiveEdit->GetSafeHwnd());
    }
}

void CSettingsDlg::OnBnClickedOk() {
    bool pass = checkAllInputKeys();
    if (!pass) {
        return;
    }
    SavenputKeyValuesGroupData();
    ConfigFileHandler configHandler;
    InputKeyValuesGroup inputKeyValuesGroup = LoadnputKeyValuesGroupData(false);
    bool success = m_HotkeyRegistrar.RegisterHotKeys(inputKeyValuesGroup);
    if (!success) {
        MessageBoxUtil::Show(m_hWnd, L"hotkey registration failed", true);
        return;
    }
    configHandler.SaveData(inputKeyValuesGroup.ToString());
    std::string loadData;
    configHandler.LoadData(loadData);
    EndDialog(IDC_SETTINGS_BUTTON_OK);
}

bool CSettingsDlg::checkAllInputKeys() {
    InputKeyValuesGroup inputKeyValuesGroup = LoadnputKeyValuesGroupData(false);
    bool result = true;
    for (size_t i = m_InputGroups.size(); i-- > 0;) {
        InputGroup inputGroup = m_InputGroups[i];
        bool pass = CheckInputs(inputGroup.pEdit1->GetDlgCtrlID());
        if (!pass) {
            result = false;
        }
    }
    return result;
}

void CSettingsDlg::OnClose() {
    EndDialog(IDC_SETTINGS_BUTTON_CANCEL);
}

std::vector<InputGroup> CSettingsDlg::GetInputGroups() const {
    return m_InputGroups;
}

void CSettingsDlg::SavenputKeyValuesGroupData() {
    std::vector<CString> firstValues;
    std::vector<InputKeyValues> allValues;
    for (InputGroup const& group : m_InputGroups) {
        CString edit1Value;
        CString edit2Value;
        group.pEdit1->GetWindowText(edit1Value);
        group.pEdit2->GetWindowText(edit2Value);
        int isOpen;
        if (group.pToggleOpenFlagButton->GetCheck() == 1) {
            isOpen = 1;
        }
        else {
            isOpen = group.isOpen > -1 ? group.pToggleOpenFlagButton->GetCheck() : group.isOpen;
        }
        InputKeyValues inputKeyValues(edit1Value, edit2Value, true);
        inputKeyValues.isOpen = isOpen;
        allValues.push_back(inputKeyValues);
        firstValues.push_back(edit1Value);
    }
    InputKeyValuesGroup inputKeyValuesGroup(firstValues, allValues);
    m_InputKeyValuesGroup = inputKeyValuesGroup;
}

bool CSettingsDlg::CheckInputs(UINT nID) {
    bool result = true;
    bool isEdit1 = false;
    for (InputGroup const& group : m_InputGroups) {
        int id = group.pEdit1->GetDlgCtrlID();
        if (nID == id) {
            isEdit1 = true;
        }
    }
    for (InputGroup group : m_InputGroups) {
        CString edit1Value;
        CCenteredEdit* currentEdit = (CCenteredEdit*)GetDlgItem(nID);
        int currentId = currentEdit->GetDlgCtrlID();
        int id = group.pEdit1->GetDlgCtrlID();
        CString currentEdit1Value;
        currentEdit->GetWindowText(currentEdit1Value);
        group.pEdit1->GetWindowText(edit1Value);
        bool haveCommonCharacters = CStringUtils::HaveSameCharacterFrequency(
            CStringUtils::CStringToString(currentEdit1Value), CStringUtils::CStringToString(edit1Value));
        if (!currentEdit1Value.IsEmpty() && haveCommonCharacters && currentId != id && isEdit1) {
            currentEdit->SetBkColor(RGB(255, 0, 0));
            CCenteredEdit* edit1 = (CCenteredEdit*)group.pEdit1;
            edit1->SetBkColor(RGB(255, 0, 0));
            currentEdit->SetWindowText(_T(""));
            SetTimer(1, 1000, NULL);
            result = false;
        }
    }
    return result;
}

void CSettingsDlg::OnTimer(UINT_PTR nIDEvent) {
    if (nIDEvent == 1) {
        for (InputGroup group : m_InputGroups) {
            CCenteredEdit* edit1 = (CCenteredEdit*)group.pEdit1;
            if (::GetFocus() == edit1->GetSafeHwnd()) {
                edit1->SetBkColor(CColorSettings::SelectedKeyColor);
            }
            else {
                edit1->ResetColor();
            }
        }
        KillTimer(1);
    }
    CDialogEx::OnTimer(nIDEvent);
}

InputKeyValuesGroup CSettingsDlg::LoadnputKeyValuesGroupData(bool loadFile) {
    InputKeyValuesGroup inputKeyValuesGroup;
    if (m_InputKeyValuesGroup.allValues.empty() && loadFile) {
        std::string data;
        ConfigFileHandler configFileHandler;
        configFileHandler.LoadData(data);
        inputKeyValuesGroup.FromString(data);
    }
    else {
        inputKeyValuesGroup = m_InputKeyValuesGroup;
    }
    return inputKeyValuesGroup;
}

void CSettingsDlg::SetKeyValuesGroupDataForDisplay(InputKeyValuesGroup& inputKeyValuesGroup) {
    int addCount = static_cast<int>(inputKeyValuesGroup.allValues.size() - m_InputGroups.size());
    for (int i = 0; i < addCount; i++) {
        AddInputGroup();
    }
    for (int i = 0; i < inputKeyValuesGroup.allValues.size(); i++) {
        InputGroup& inputGroup = m_InputGroups[i];
        InputKeyValues value = inputKeyValuesGroup.allValues[i];
        CString edit1Value;
        CString edit2Value;
        inputGroup.pEdit1->SetWindowTextW(value.edit1Value);
        inputGroup.pEdit2->SetWindowTextW(value.edit2Value);
        inputGroup.pToggleOpenFlagButton->SetCheck(value.isOpen == 1 ? 1 : 0);
        inputGroup.isOpen = value.isOpen;
    }
}

CHotkeyRegistrar CSettingsDlg::GetHotkeyRegistrar() {
    return m_HotkeyRegistrar;
}

void CSettingsDlg::OnDestroy() {
    CDialogEx::OnDestroy();
}

#pragma once
#include <vector>
#include "InputKeyValuesGroup.h"
#include <queue>
#include <memory>
#include "CustomListBox.h"
#include <map>
#include "SearchEdit.h"
#include "MFCAdjustableIconButton.h"
#include "HotKeyPressManager.h"
#include "HotkeyRegistrar.h"
#include "TrayIconManager.h"

enum ButtonState {
    STOP, WAIT, RUNNING
};

void StartListenKeyUp();

void StopListenKeyUp();

void ProcessMessages();

void SimulateKeyEvent(BYTE vkCode, BOOL keyDown);

void RecordPressedKeys();

void ReleaseKeyState();

void RestoreKeyState();

void ClearAndSimulateKeyRelease();

class CKeyControlDlg : public CDialogEx {
public:
    CKeyControlDlg(CWnd *pParent = nullptr);
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_KEY_CONTROL_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange *pDX);

    CWinThread *m_pThread;

    CHotkeyRegistrar m_HotkeyRegistrar;

    HICON m_appIcon;
    HICON m_hIcon;
    BOOL m_IsWindowSelected;

    virtual BOOL OnInitDialog();

    void createListBox();

    void createEditInput();

    void CreateSettingAndQuitButton();

    void InitCreateAfterOnInitDialog();

    void OnBnClickedButtonSettings();

    void SetSwitchButtonState(ButtonState state);

    void OnBnClickedOk();

    void UpdateListBoxScrollBar(CCustomListBox &listBox);

    void SetSettingIconColor(bool red);

    void UpdateWindowList();

    afx_msg void OnBnClickedCancel();

    afx_msg void OnEnChangeEditInput();

    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

    afx_msg LRESULT OnItemClicked(WPARAM wParam, LPARAM lParam);

    afx_msg LRESULT OnAppInstanceMessage(WPARAM wParam, LPARAM lParam);

    afx_msg LRESULT OnTrayIcon(WPARAM wParam, LPARAM lParam);

    afx_msg void OnTrayOpen();

    afx_msg void OnTrayExit();

    afx_msg void OnDestroy();

    afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);

    afx_msg void OnLbnSelchangeWindowsListBox();

    DECLARE_MESSAGE_MAP()

public:
    void SendKey(UINT nHotKeyId, UINT);

    BOOL PreTranslateMessage(MSG *pMsg);

    std::queue<UINT> m_hotKeyIdQueue;
    HWND m_PreviousFocusWindow;
    CHotKeyPressManager m_HotKeyPressManager;
    bool m_bListenKeyUp = false;

private:
    CTrayIconManager m_TrayIconManager;
    CFont m_alibabaPuHuiTiRegular;
    CFont m_alibabaPuHuiTiMed;
    CFont m_alibabaPuHuiTiHeavy;
    bool g_bBlockKeys = false;
    bool g_bShiftPressed = false;
    CFont* cancelButtonFont;
    CEdit m_EditHidden;
    CSearchEdit m_EditInput;
    CStatic m_iconBackground;
    CMFCAdjustableIconButton* m_SettingsButton;
    CCustomListBox m_ListBoxWindows;
    CMFCAdjustableIconButton* m_OkButton;
    CMFCAdjustableIconButton m_CancelButton;
    HWND m_SelectedWindow;
    HWND m_PreSelectedWindow;
    int m_buttonState = STOP;
    BOOL m_bIsStarted;
    BOOL m_bIsInited;
    BOOL m_bIsKeyMapSeted;
    std::vector<CString> m_WindowTitles;
    std::vector<HWND> m_WindowHandles;
    std::vector<HWND> m_HitedToKeys;
    InputKeyValuesGroup m_SettingInputKeyValuesGroup;
    std::queue<INPUT> m_SendKeyQueue;
    std::queue<std::string> m_SendKeyStrsQueue;
    std::map<HWND, int> listBoxPreviousFocusWindowMap;
    std::map<HWND, bool> listBoxPreviousIsIconicMap;
    CStatic m_iconSearch;
};

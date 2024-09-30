#pragma once
#include "afxwin.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "InputKeyValuesGroup.h"
#include "TransparentBgStatic.h"
#include "CIconStatic.h"
#include "MFCAdjustableIconButton.h"
#include "MFCDeleteButton.h"
#include "CenteredEdit.h"
#include "HotKeyData.h"
#include "HotkeyRegistrar.h"
#include <memory>

enum UpdateState {
    ADD_ITEM, DELETE_ITEM, SCROLL
};

class KeyMapData {
public:
    CString keyFromStr;
    CString keyToStr;
};

class KeyMapDataList {
public:
    std::vector<CString> pressedKeys;
};

class InputGroup {
public:
    CCenteredEdit *pEdit1;
    CCenteredEdit *pEdit2;
    CButton *pToggleOpenFlagButton;
    CButton *pToggleBackPreWindowButton;
    CCIconStatic *pIcon;
    int isOpen;
    CMFCDeleteButton *pDeleteButton;
};

class CSettingsDlg : public CDialogEx {
    DECLARE_DYNAMIC(CSettingsDlg)

    CSettingsDlg(UINT nIDTemplate, HWND hwndA, CHotkeyRegistrar hotkeyRegistrar, CWnd *pParent = nullptr);

    CSettingsDlg(CWnd *pParent = nullptr);

    virtual ~CSettingsDlg();

protected:
    virtual void DoDataExchange(CDataExchange *pDX);

    afx_msg void OnEnChangeInputBox(UINT nID);

    afx_msg void OnEditSetFocus(UINT nID);

    afx_msg void OnBnClickedOk();

    afx_msg void OnClose();

    bool checkAllInputKeys();

    DECLARE_MESSAGE_MAP()

public:
    CHotkeyRegistrar GetHotkeyRegistrar();

    InputKeyValuesGroup LoadnputKeyValuesGroupData(bool loadFile = true);

private:
    virtual BOOL OnInitDialog();

    void ShowVerticalScrollBar(bool bShow);

    BOOL PreTranslateMessage(MSG* pMsg) override;

    void CreateScrollablePane();

    void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

    BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

    void RefreshWindow();

    void CreateTitleBg();

    void CreateAddButton();

    void CreateSaveButton();

    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

    afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

    afx_msg void OnBnClickedAddButton();

    afx_msg void OnDestroy();

    void OnBnClickedGroupDeleteButton(UINT nID);

    std::vector<InputGroup> GetInputGroups() const;

    void SetKeyValuesGroupDataForDisplay(InputKeyValuesGroup& inputKeyValuesGroup);

    void RecordKey(UINT nChar, BOOL isKeyDown, BOOL filter);

    std::vector<int> FilterPressedExceptKeys(const std::vector<int> &m_PressedKeys);

    std::vector<int> FilterFullPressedExceptKeys(const std::vector<int> &m_PressedKeys);

    std::vector<int> FilterPressedExceptLastKeys(const std::vector<int> &m_PressedKeys);

    std::vector<int> FilterNoPressedKeys(const std::vector<int> &m_PressedKeys);

    std::vector<int> FilterPressedKeys(const std::vector<int> &m_PressedKeys, bool filter);

    HBRUSH OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor);

    CString getConvertKeyStr(UINT nChar);

    InputGroup AddInputGroup(int yPosition);

    int getNextIdOffset();

    InputGroup AddInputGroup();

    void DeleteInputGroup(int index);

    void SetWindowSize(int height);

    void UpdateLayout(UpdateState updateState);

    bool IsScrollBarEnabled(HWND hwnd, int nBar);

    bool ShowMyScrollBar();

    void SetMyScrollRange();

    void SavenputKeyValuesGroupData();

    bool CheckInputs(UINT nID);

    void OnTimer(UINT_PTR nIDEvent);

    CHotkeyRegistrar m_HotkeyRegistrar;
    CToolTipCtrl m_OpenToolTip;
    std::vector<CEdit *> m_InputEdits;
    CString m_RecordedKeys;
    CEdit *m_pActiveEdit;
    HICON m_hIcon;
    CMFCAdjustableIconButton m_AddButton;
    CMFCAdjustableIconButton m_SaveButton;
    CEdit *m_pCurrentEdit;
    CFont m_Font;
    std::unordered_map<int, bool> m_KeyStates;
    std::vector<int> m_PressedKeys;
    InputKeyValuesGroup m_InputKeyValuesGroup;
    CEdit m_EditHidden;
    std::vector<InputGroup> m_InputGroups;
    CStatic m_staticBackground;
    CTransparentBgStatic m_staticInput;
    CTransparentBgStatic m_staticMap;
    CCIconStatic m_staticIcon;
    CHotKeyData m_HotKeyData;
    bool m_bResizing = false;
    bool m_bInInit = false;
    bool isSettingKey = false;
    CFont *deleteButtonFont;
    HWND m_parentHwnd;
    std::unordered_set<int> m_HotKeyIds;
    int m_screenHeight = 0;
    int m_nContentHeight = 0;
    int m_nScrollPos = 0;
    bool m_inited = false;
    float m_displayScrollWhenToScreenPercent = 0.7f;
    bool m_scrollBarEnable = false;
    bool m_preScrollBarState = false;
};

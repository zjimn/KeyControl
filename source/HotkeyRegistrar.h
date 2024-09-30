#pragma once
#include "HotKeyData.h"
#include "InputKeyValuesGroup.h"

class CHotkeyRegistrar {
public:
    CHotkeyRegistrar();

    CHotkeyRegistrar(HWND hwnd);

    bool RegisterHotKeys(InputKeyValuesGroup inputKeyValuesGroup);

    bool RegisterHotKeys(HWND parentHwnd, InputKeyValuesGroup inputKeyValuesGroup);

    bool RegisterHotKeys();

    bool UnRegisterHotKeys();

    bool CleanHotKeys();

    int GetNextHotKeyId();

    CHotKeyData GetHotKeyData();

private:
    int m_currentHotKeyId;
    HWND m_parentHwnd;
    CHotKeyData m_hotKeyData;
    InputKeyValuesGroup m_inputKeyValuesGroup;
};

#include "pch.h"
#include "HotkeyRegistrar.h"
#include "HotKeyHandler.h"

CHotkeyRegistrar::CHotkeyRegistrar() {
}

CHotkeyRegistrar::CHotkeyRegistrar(HWND hwnd) {
    m_parentHwnd = hwnd;
}

bool CHotkeyRegistrar::RegisterHotKeys(InputKeyValuesGroup inputKeyValuesGroup) {
    return RegisterHotKeys(m_parentHwnd, inputKeyValuesGroup);
}

bool CHotkeyRegistrar::RegisterHotKeys(HWND parentHwnd, InputKeyValuesGroup inputKeyValuesGroup) {
    m_parentHwnd = parentHwnd;
    CleanHotKeys();
    for (InputKeyValues values : inputKeyValuesGroup.allValues) {
        CString value1 = values.edit1Value;
        CString value2 = values.edit2Value;
        if (values.isOpen != 1 || value1.IsEmpty()) {
            continue;
        }
        CHotKeyHandler hotKeyHandler(values.edit1Value, values.edit2Value);
        int hotKeyId = GetNextHotKeyId();
        bool success = hotKeyHandler.RegisterGlobalHotKey(parentHwnd, hotKeyId);
        if (!success) {
            CleanHotKeys();
            TRACE("error: %d\n", hotKeyId);
            return false;
        }
        else {
            m_hotKeyData.SaveHotKeyData(hotKeyId, hotKeyHandler.GetMapKeyCodes());
        }
    }
    m_inputKeyValuesGroup = inputKeyValuesGroup;
    return true;
}

bool CHotkeyRegistrar::RegisterHotKeys() {
    return RegisterHotKeys(m_parentHwnd, m_inputKeyValuesGroup);
}

bool CHotkeyRegistrar::UnRegisterHotKeys() {
    return CleanHotKeys();
}

bool CHotkeyRegistrar::CleanHotKeys() {
    bool resutl = true;
    for (UINT keyId : m_hotKeyData.GetHotKeyIds()) {
        CHotKeyHandler hotKeyHandler;
        bool success = hotKeyHandler.UnRegisterGlobalHotKey(m_parentHwnd, keyId);
        if (!success) {
            resutl = false;
        }
        m_hotKeyData.DeleteHotKeyData(keyId);
    }
    return resutl;
}

int CHotkeyRegistrar::GetNextHotKeyId() {
    return m_currentHotKeyId++;
}

CHotKeyData CHotkeyRegistrar::GetHotKeyData() {
    return m_hotKeyData;
}

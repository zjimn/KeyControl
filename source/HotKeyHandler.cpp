#include "pch.h"
#include "HotKeyHandler.h"
#include <sstream>
#include <vector>
#include "KeyboardUtils.h"
#include "MessageBoxUtil.h"

CHotKeyHandler::CHotKeyHandler() {
}

CHotKeyHandler::CHotKeyHandler(const CString &hotKeyStr1, const CString &hotKeyStr2) {
    m_parseHotKeyState = ParseHotKeyString(hotKeyStr1, m_nKey1, m_modifiers);
    m_parseMapKeyState = ParseMapKeyString(hotKeyStr2, m_mapKeyCodes);
}

bool CHotKeyHandler::RegisterGlobalHotKey(HWND hWnd, UINT nHotKeyId) {
    if (!m_parseHotKeyState || !m_parseMapKeyState) {
        return false;
    }
    for (int i = 0; i < 5; i++) {
        bool success = RegisterHotKey(hWnd, nHotKeyId, m_modifiers, m_nKey1);
        if (success) {
            return true;
        }
        Sleep(100);
    }
    return false;
}

bool CHotKeyHandler::UnRegisterGlobalHotKey(HWND hWnd, UINT nHotKeyId) {
    for (int i = 0; i < 5; i++) {
        bool success = UnregisterHotKey(hWnd, nHotKeyId);
        if (success) {
            return true;
        }
        Sleep(100);
    }
    return false;
}

bool CHotKeyHandler::ParseMapKeyString(const CString &hotKeyStr, std::vector<UINT> &keys) {
    std::vector<CString> tokens;
    std::wistringstream iss(hotKeyStr.GetString());
    std::wstring token;
    while (iss >> token) {
        tokens.push_back(CString(token.c_str()));
    }
    std::vector<UINT> result;
    for (size_t i = 0; i < tokens.size(); ++i) {
        CString token = tokens[i]; {
            CString keyStr(token);
            UINT key = KeyboardUtils::StringToKeyCode(keyStr);
            if (key == KeyboardUtils::UNDEFINED_KEYCODE) {
                return false;
            }
            key = KeyboardUtils::StringToKeyCode(keyStr) & 0xFF;
            result.push_back(key);
        }
    }
    keys = result;
    return true;
}

std::vector<UINT> CHotKeyHandler::GetMapKeyCodes() {
    return m_mapKeyCodes;
}

bool CHotKeyHandler::ParseHotKeyString(const CString &hotKeyStr, UINT &key, UINT &modifiers) {
    modifiers = 0;
    std::map<CString, UINT> modifierMap = {
        {_T("Shift"), MOD_SHIFT},
        {_T("Ctrl"), MOD_CONTROL},
        {_T("Alt"), MOD_ALT},
        {_T("Win"), MOD_WIN}
    };
    std::vector<CString> tokens;
    std::wistringstream iss(hotKeyStr.GetString());
    std::wstring token;
    while (iss >> token) {
        tokens.push_back(CString(token.c_str()));
    }
    bool keyExist = false;
    for (size_t i = 0; i < tokens.size(); ++i) {
        CString token = tokens[i];
        if (modifierMap.find(token) != modifierMap.end()) {
            modifiers |= modifierMap[token];
        } else {
            if (keyExist) {
                return false;
            }
            keyExist = true;
            CString keyStr(token);
            key = KeyboardUtils::StringToKeyCode(keyStr);
            if (key == KeyboardUtils::UNDEFINED_KEYCODE) {
                MessageBoxUtil::Show(m_hwnd, L"hotkey is incorrect", true);
                return false;
            }
            key = KeyboardUtils::StringToKeyCode(keyStr) & 0xFF;
        }
    }
    return true;
}

int main() {
    CHotKeyHandler handler(L"Shift Ctrl  CapsLock", L"Ctrl  D");
}

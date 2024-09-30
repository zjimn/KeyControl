#include "pch.h"
#include "KeyboardUtils.h"
#include <Windows.h>
#include <algorithm>
#include <cctype>
#include <string>
#include "StringUtils.h"
std::map<int, CString> KeyboardUtils::keyCodeToStringMap;
std::map<CString, int> KeyboardUtils::stringToKeyCodeMap;

void KeyboardUtils::InitializeMappings() {
    if (keyCodeToStringMap.empty()) {
        keyCodeToStringMap = {
            {VK_UP, L"Up"},
            {VK_DOWN, L"Down"},
            {VK_LEFT, L"Left"},
            {VK_RIGHT, L"Right"},
            {VK_RETURN, L"Enter"},
            {VK_TAB, L"Tab"},
            {VK_ESCAPE, L"Esc"},
            {VK_SPACE, L"Space"},
            {VK_BACK, L"Backspace"},
            {VK_DELETE, L"Delete"},
            {VK_INSERT, L"Insert"},
            {VK_HOME, L"Home"},
            {VK_END, L"End"},
            {VK_PRIOR, L"PgUp"},
            {VK_NEXT, L"PgDn"},
            {VK_F1, L"F1"},
            {VK_F2, L"F2"},
            {VK_F3, L"F3"},
            {VK_F4, L"F4"},
            {VK_F5, L"F5"},
            {VK_F6, L"F6"},
            {VK_F7, L"F7"},
            {VK_F8, L"F8"},
            {VK_F9, L"F9"},
            {VK_F10, L"F10"},
            {VK_F11, L"F11"},
            {VK_F12, L"F12"},
            {VK_LSHIFT, L"Shift"},
            {VK_RSHIFT, L"Shift"},
            {VK_LCONTROL, L"Ctrl"},
            {VK_RCONTROL, L"Ctrl"},
            {VK_LMENU, L"Alt"},
            {VK_RMENU, L"Alt"},
            {VK_LWIN, L"Win"},
            {VK_RWIN, L"Win"},
            {VK_NUMLOCK, L"NumLock"},
            {VK_SHIFT, L"Shift"},
            {VK_MENU, L"Alt"},
            {VK_CONTROL, L"Ctrl"},
            {VK_CAPITAL, L"CapsLock"},
            {VK_OEM_1, L";"},
            {VK_OEM_PLUS, L"="},
            {VK_OEM_COMMA, L","},
            {VK_OEM_MINUS, L"-"},
            {VK_OEM_PERIOD, L"."},
            {VK_OEM_2, L"/"},
            {VK_OEM_3, L"`"},
            {VK_OEM_4, L"["},
            {VK_OEM_5, L"\\"},
            {VK_OEM_6, L"]"},
            {VK_OEM_7, L"'"},
            {VK_OEM_8, L"Oem8"},
            {VK_OEM_102, L"<"},
            {VK_DECIMAL, L"."},
            {VK_DIVIDE, L"/"},
            {VK_MULTIPLY, L"*"},
            {VK_SUBTRACT, L"-"},
            {VK_ADD, L"+"},
            {VK_NUMPAD0, L"0"},
            {VK_NUMPAD1, L"1"},
            {VK_NUMPAD2, L"2"},
            {VK_NUMPAD3, L"3"},
            {VK_NUMPAD4, L"4"},
            {VK_NUMPAD5, L"5"},
            {VK_NUMPAD6, L"6"},
            {VK_NUMPAD7, L"7"},
            {VK_NUMPAD8, L"8"},
            {VK_NUMPAD9, L"9"}
        };
        for (int i = 0; i <= 9; i++) {
            int vkCode = static_cast<int>('0' + i);
            keyCodeToStringMap.insert({ vkCode, CStringUtils::IntToCString(char(i)) });
        }
        for (char ch = 'A'; ch <= 'Z'; ++ch) {
            int vkCode = charToVkCode(ch);
            std::string str(1, ch);
            keyCodeToStringMap.insert({ vkCode, CStringUtils::StringToCString(str) });
        }
        for (char ch = 'a'; ch <= 'z'; ++ch) {
            int vkCode = charToVkCode(ch);
            std::string str(1, ch);
            keyCodeToStringMap.insert({ vkCode, CStringUtils::StringToCString(str) });
        }
        for (const auto& pair : keyCodeToStringMap) {
            stringToKeyCodeMap[pair.second] = pair.first;
        }
    }
}

int KeyboardUtils::charToVkCode(char character) {
    if (std::isalpha(character)) {
        return static_cast<int>(std::toupper(character));
    }
    return UNDEFINED_KEYCODE;
}

CString KeyboardUtils::KeyCodeToString(int keyCode) {
    InitializeMappings();
    auto it = keyCodeToStringMap.find(keyCode);
    if (it != keyCodeToStringMap.end() && it->first == keyCode) {
        return it->second;
    }
    return UNDEFINED_KEYSTR;
}

bool KeyboardUtils::Exist(int keyCode) {
    InitializeMappings();
    auto it = keyCodeToStringMap.find(keyCode);
    return it != keyCodeToStringMap.end() && it->first == keyCode;
}

int KeyboardUtils::ChangeKeyCode(int keyCode) {
    InitializeMappings();
    auto it = keyCodeToStringMap.find(keyCode);
    if (it != keyCodeToStringMap.end() && it->first == keyCode) {
        return keyCode;
    }
    return UNDEFINED_KEYCODE;
}

int KeyboardUtils::StringToKeyCode(const CString keyName) {
    InitializeMappings();
    auto it = stringToKeyCodeMap.find(keyName);
    if (it != stringToKeyCodeMap.end() && it->first == keyName) {
        return it->second;
    }
    if (keyName.GetLength() == 1 && keyName[0] >= 'A' && keyName[0] <= 'Z') {
        return static_cast<WORD>(keyName[0]);
    }
    return UNDEFINED_KEYCODE;
}

int KeyboardUtils::ChangeKeyCodeToNormal(int keyCode) {
    CString keyCodeStr = KeyCodeToString(keyCode);
    return StringToKeyCode(keyCodeStr);
}

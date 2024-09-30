#include "pch.h"
#include "VkToAHKConverter.h"
#include <unordered_map>
#include <cctype>
#include <iomanip>
#include <map>

std::unordered_map<UINT, std::string> VkToAHKConverter::vkMapping;

bool VkToAHKConverter::initialized = false;

std::string VkToAHKConverter::ConvertVkToAHKString(UINT vkCode) {
    InitializeMapping();
    auto it = vkMapping.find(vkCode);
    if (it != vkMapping.end() && it->first == vkCode) {
        return it->second;
    }
    return "null";
}

void VkToAHKConverter::InitializeMapping() {
    if (vkMapping.empty()) {
        vkMapping = {
            {VK_UP, "{Up}"},
            {VK_DOWN, "{Down}"},
            {VK_LEFT, "{Left}"},
            {VK_RIGHT, "{Right}"},
            {VK_RETURN, "{Enter}"},
            {VK_TAB, "{Tab}"},
            {VK_ESCAPE, "{Esc}"},
            {VK_SPACE, "{Space}"},
            {VK_BACK, "{Backspace}"},
            {VK_DELETE, "{Delete}"},
            {VK_INSERT, "{Insert}"},
            {VK_HOME, "{Home}"},
            {VK_END, "{End}"},
            {VK_PRIOR, "{PgUp}"},
            {VK_NEXT, "{PgDn}"},
            {VK_F1, "{F1}"},
            {VK_F2, "{F2}"},
            {VK_F3, "{F3}"},
            {VK_F4, "{F4}"},
            {VK_F5, "{F5}"},
            {VK_F6, "{F6}"},
            {VK_F7, "{F7}"},
            {VK_F8, "{F8}"},
            {VK_F9, "{F9}"},
            {VK_F10, "{F10}"},
            {VK_F11, "{F11}"},
            {VK_F12, "{F12}"},
            {VK_LSHIFT, "{LShift}"},
            {VK_RSHIFT, "{RShift}"},
            {VK_LCONTROL, "{LCtrl}"},
            {VK_RCONTROL, "{RCtrl}"},
            {VK_LMENU, "{LAlt}"},
            {VK_RMENU, "{RAlt}"},
            {VK_LWIN, "{LWin}"},
            {VK_RWIN, "{RWin}"},
            {VK_NUMLOCK, "{NumLock}"},
            {VK_SHIFT, "{Shift}"},
            {VK_MENU, "{Alt}"},
            {VK_CONTROL, "{Ctrl}"},
            {VK_CAPITAL, "{CapsLock}"},
            {VK_OEM_1, "{Semicolon}"},
            {VK_OEM_PLUS, "{Equal}"},
            {VK_OEM_COMMA, "{Comma}"},
            {VK_OEM_MINUS, "{Minus}"},
            {VK_OEM_PERIOD, "{Period}"},
            {VK_OEM_2, "{Slash}"},
            {VK_OEM_3, "{Grave}"},
            {VK_OEM_4, "{LeftBracket}"},
            {VK_OEM_5, "{Backslash}"},
            {VK_OEM_6, "{RightBracket}"},
            {VK_OEM_7, "{Quote}"},
            {VK_OEM_8, "{Oem8}"},
            {VK_OEM_102, "{Less}"},
            {VK_DECIMAL, "{Decimal}"},
            {VK_DIVIDE, "{Divide}"},
            {VK_MULTIPLY, "{Multiply}"},
            {VK_SUBTRACT, "{Subtract}"},
            {VK_ADD, "{Add}"},
            {VK_NUMPAD0, "{Numpad0}"},
            {VK_NUMPAD1, "{Numpad1}"},
            {VK_NUMPAD2, "{Numpad2}"},
            {VK_NUMPAD3, "{Numpad3}"},
            {VK_NUMPAD4, "{Numpad4}"},
            {VK_NUMPAD5, "{Numpad5}"},
            {VK_NUMPAD6, "{Numpad6}"},
            {VK_NUMPAD7, "{Numpad7}"},
            {VK_NUMPAD8, "{Numpad8}"},
            {VK_NUMPAD9, "{Numpad9}"}
        };
        for (int i = 0; i <= 9; i++) {
            int vkCode = charToVkCode(i);
            std::string str(1, i);
            vkMapping.insert({ vkCode, str });
        }
        for (char ch = 'A'; ch <= 'Z'; ++ch) {
            int vkCode = charToVkCode(ch);
            std::string str(1, ch);
            vkMapping.insert({ vkCode, str });
        }
        for (char ch = 'a'; ch <= 'z'; ++ch) {
            int vkCode = charToVkCode(ch);
            std::string str(1, ch);
            vkMapping.insert({ vkCode, str });
        }
    }
}

int VkToAHKConverter::charToVkCode(char character) {
    if (std::isalpha(character)) {
        return static_cast<int>(std::toupper(character));
    }
    return -1;
}

#pragma once
#include <string>
#include <map>

class KeyboardUtils {
public:
    static CString KeyCodeToString(int keyCode);

    static bool Exist(int keyCode);

    static int ChangeKeyCode(int keyCode);

    static int StringToKeyCode(const CString keyName);

    static int ChangeKeyCodeToNormal(int keyCode);

    static int const UNDEFINED_KEYCODE = -1;
    static inline CString const UNDEFINED_KEYSTR = _T("UNDEFINED_KEYCODE");

private:
    static void InitializeMappings();

    static int charToVkCode(char character);

    static std::map<int, CString> keyCodeToStringMap;
    static std::map<CString, int> stringToKeyCodeMap;
};

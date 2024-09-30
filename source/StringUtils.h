#ifndef STRINGUTILS_H
#define STRINGUTILS_H
#include <string>
#include <vector>

class CStringUtils {
public:
    static std::string Trim(const std::string &str);

    static CString Trim(CString &str);

    static std::string TrimLeft(const std::string &str);

    static std::string TrimRight(const std::string &str);

    static std::vector<CString> SplitCStringBySpace(const CString &str, TCHAR delimiter);

    static WORD CStringToWORD(const CString &str);

    static CString WordToCString(WORD value);

    static std::string CStringToString(CString value);

    static CString StringToCString(std::string value);

    static int StringToInt(std::string value);

    static int CStringToInt(const CString &cstr);

    static std::string IntToString(const int &num);

    static CString IntToCString(const int &num);

    static bool HaveSameCharacterFrequency(const std::string &str1, const std::string &str2);

    static bool ContainsCharsInOrder(const CString &mainStr, const CString &subStr);

    static std::wstring ConvertToWideString(const std::string &narrowStr);

    static std::string ConvertToNarrowString(const std::wstring &wideStr);

    static std::wstring ConvertUtf8ToUtf16(const std::string &utf8Str);

    static std::string WStringToString(const std::wstring &wstr);

private:
    static std::string TrimImpl(const std::string &str, bool left, bool right);
};
#endif

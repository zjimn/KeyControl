#pragma once
#include <vector>
#include <string>
#include "CustomListBox.h"

class CStringSorter {
public:
    static std::wstring CStringToWString(const CString &str);

    static bool CompareStrings(const CString &str1, const CString &str2);

    static bool Compare(const ListItem &a, const ListItem &b);

    static void SortWindowList(std::vector<ListItem> &windowList);

private:
    static std::wstring GetPinyinInitial(wchar_t ch);

    static bool IsChinese(wchar_t ch);

    static bool IsLetter(wchar_t ch);

    static bool IsDigit(wchar_t ch);
};

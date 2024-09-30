#include "pch.h"
#include "StringSorter.h"
#include <algorithm>
#include <locale>
#include <cctype>
#include "PinyinConverter.h"

std::wstring CStringSorter::GetPinyinInitial(wchar_t ch) {
    std::wstring str(1, ch);
    str = PinyinConverter::GetPinyin(str);
    return str.substr(0, 1);
}

bool CStringSorter::IsChinese(wchar_t ch) {
    return ch >= 0x4E00 && ch <= 0x9FA5;
}

bool CStringSorter::IsLetter(wchar_t ch) {
    return std::isalpha(ch, std::locale::classic());
}

bool CStringSorter::IsDigit(wchar_t ch) {
    return std::isdigit(ch, std::locale::classic());
}

std::wstring CStringSorter::CStringToWString(const CString& str) {
    return std::wstring(str.GetString());
}

bool CStringSorter::CompareStrings(const CString& str1, const CString& str2) {
    std::wstring wstr1 = CStringToWString(str1);
    std::wstring wstr2 = CStringToWString(str2);
    size_t minLength = (wstr1.size() < wstr2.size()) ? wstr1.size() : wstr2.size();
    for (size_t i = 0; i < minLength; ++i) {
        wchar_t ch1 = wstr1[i];
        wchar_t ch2 = wstr2[i];
        bool isChinese1 = IsChinese(ch1);
        bool isChinese2 = IsChinese(ch2);
        bool isLetter1 = IsLetter(ch1);
        bool isLetter2 = IsLetter(ch2);
        bool isDigit1 = IsDigit(ch1);
        bool isDigit2 = IsDigit(ch2);
        if (isChinese1 && !isChinese2)
            return true;
        if (!isChinese1 && isChinese2)
            return false;
        if (isChinese1 && isChinese2) {
            std::wstring pinyin1 = GetPinyinInitial(ch1);
            std::wstring pinyin2 = GetPinyinInitial(ch2);
            if (pinyin1 != pinyin2)
                return pinyin1 < pinyin2;
        }
        if (isLetter1 && !isLetter2)
            return true;
        if (!isLetter1 && isLetter2)
            return false;
        if (isDigit1 && !isDigit2)
            return true;
        if (!isDigit1 && isDigit2)
            return false;
        if (ch1 != ch2)
            return ch1 < ch2;
    }
    return wstr1.size() < wstr2.size();
}

bool CStringSorter::Compare(const ListItem& a, const ListItem& b) {
    return CompareStrings(a.title, b.title);
}

void CStringSorter::SortWindowList(std::vector<ListItem>& windowList) {
    std::sort(windowList.begin(), windowList.end(), CStringSorter::Compare);
}

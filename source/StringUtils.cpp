#include "pch.h"
#include "StringUtils.h"
#include <unordered_map>
#include <stdexcept>

std::string CStringUtils::TrimImpl(const std::string& str, bool left, bool right) {
    if (str.empty()) {
        return str;
    }
    std::string result = str;
    if (left) {
        result.erase(result.begin(), std::find_if(result.begin(), result.end(), [](unsigned char ch) {
            return !std::isspace(ch);
            }));
    }
    if (right) {
        result.erase(std::find_if(result.rbegin(), result.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
            }).base(), result.end());
    }
    return result;
}

std::string CStringUtils::Trim(const std::string& str) {
    return TrimImpl(str, true, true);
}

CString CStringUtils::Trim(CString& str) {
    if (str.IsEmpty()) {
        return str;
    }
    int start = 0;
    while (start < str.GetLength() && std::isspace(static_cast<unsigned char>(str[start]))) {
        ++start;
    }
    int end = str.GetLength() - 1;
    while (end >= start && std::isspace(static_cast<unsigned char>(str[end]))) {
        --end;
    }
    str = str.Mid(start, end - start + 1);
    return str;
}

std::string CStringUtils::TrimLeft(const std::string& str) {
    return TrimImpl(str, true, false);
}

std::string CStringUtils::TrimRight(const std::string& str) {
    return TrimImpl(str, false, true);
}

std::vector<CString> CStringUtils::SplitCStringBySpace(const CString& str, TCHAR delimiter) {
    std::vector<CString> result;
    int start = 0;
    int end = str.Find(delimiter, start);
    while (end != -1) {
        result.push_back(str.Mid(start, end - start));
        start = end + 1;
        end = str.Find(delimiter, start);
    }
    result.push_back(str.Mid(start));
    return result;
}

WORD CStringUtils::CStringToWORD(const CString& str) {
    int number = _ttoi(str);
    if (number >= 0 && number <= 0xFFFF) {
        return static_cast<WORD>(number);
    }
    else {
        return 0;
    }
}

CString CStringUtils::WordToCString(WORD value) {
    std::wstring wstr = std::to_wstring(value);
    CString cstr(wstr.c_str());
    return cstr;
}

std::string CStringUtils::CStringToString(CString value) {
    std::string str(CW2A(value.GetString()));
    return str;
}

CString CStringUtils::StringToCString(std::string value) {
    CString cstr(value.c_str());
    return cstr;
}

int CStringUtils::StringToInt(std::string value) {
    return std::stoi(value);
}

int CStringUtils::CStringToInt(const CString& cstr) {
    std::string stdStr = std::string(CT2A(cstr.GetString()));
    return std::stoi(stdStr);
}

std::string CStringUtils::IntToString(const int& num) {
    std::string str = std::to_string(num);
    return str;
}

CString CStringUtils::IntToCString(const int& num) {
    std::string str = std::to_string(num);
    return CStringUtils::StringToCString(str);
}

bool CStringUtils::HaveSameCharacterFrequency(const std::string& str1, const std::string& str2) {
    if (str1.length() != str2.length()) {
        return false;
    }
    std::unordered_map<char, int> countMap1;
    std::unordered_map<char, int> countMap2;
    for (const char& ch : str1) {
        countMap1[ch]++;
    }
    for (const char& ch : str2) {
        countMap2[ch]++;
    }
    return countMap1 == countMap2;
}

bool CStringUtils::ContainsCharsInOrder(const CString& mainStr, const CString& subStr) {
    int mainIndex = 0;
    int subIndex = 0;
    while (mainIndex < mainStr.GetLength() && subIndex < subStr.GetLength()) {
        if (mainStr[mainIndex] == subStr[subIndex]) {
            subIndex++;
        }
        mainIndex++;
    }
    return subIndex == subStr.GetLength();
}

std::wstring CStringUtils::ConvertToWideString(const std::string& narrowStr) {
    int wideSize = MultiByteToWideChar(CP_UTF8, 0, narrowStr.c_str(), -1, nullptr, 0);
    std::wstring wideStr(wideSize, 0);
    MultiByteToWideChar(CP_UTF8, 0, narrowStr.c_str(), -1, &wideStr[0], wideSize);
    return wideStr;
}

std::string CStringUtils::ConvertToNarrowString(const std::wstring& wideStr) {
    int narrowSize = WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string narrowStr(narrowSize, 0);
    WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, &narrowStr[0], narrowSize, nullptr, nullptr);
    return narrowStr;
}

std::wstring CStringUtils::ConvertUtf8ToUtf16(const std::string& utf8Str) {
    int utf16Size = MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, nullptr, 0);
    if (utf16Size == 0) {
        throw std::runtime_error("Conversion failed");
    }
    std::wstring utf16Str(utf16Size, 0);
    int result = MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, &utf16Str[0], utf16Size);
    if (result == 0) {
        throw std::runtime_error("Conversion failed");
    }
    return utf16Str;
}

std::string CStringUtils::WStringToString(const std::wstring& wstr) {
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), static_cast<int>(wstr.length()), NULL, 0, NULL,
        NULL);
    std::string str(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), static_cast<int>(wstr.length()), &str[0], size_needed, NULL, NULL);
    return str;
}

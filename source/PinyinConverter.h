#pragma once
#include <string>
#include <map>
#include <vector>

class PinyinConverter {
public:
    static std::wstring GetPinyin(const std::wstring &chinese);

    static bool Initialize();

private:
    static bool LoadPinyinDict();

    static std::map<wchar_t, std::wstring> m_pinyinDict;
    static constexpr wchar_t PinyinDictFile[] = L"res\\pinyin.txt";
};

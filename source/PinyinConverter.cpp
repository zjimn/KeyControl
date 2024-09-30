#include "pch.h"
#include "PinyinConverter.h"
#include <fstream>
#include <sstream>
#include <locale>
#include <map>
#include "StringUtils.h"
std::map<wchar_t, std::wstring> PinyinConverter::m_pinyinDict;

bool PinyinConverter::Initialize() {
    return LoadPinyinDict();
}

bool load = PinyinConverter::Initialize();

bool PinyinConverter::LoadPinyinDict() {
    std::ifstream file(PinyinDictFile, std::ios::binary);
    if (!file.is_open())
        return false;
    std::ostringstream ss;
    ss << file.rdbuf();
    std::string utf8_content = ss.str();
    std::wstring content = CStringUtils::ConvertUtf8ToUtf16(utf8_content);
    std::wistringstream wss(content);
    std::wstring line;
    while (std::getline(wss, line)) {
        std::wistringstream line_stream(line);
        std::wstring chinese, pinyin;
        std::getline(line_stream, chinese, L'=');
        std::getline(line_stream, pinyin);
        for (wchar_t ch : chinese)
            m_pinyinDict[ch] = pinyin;
    }
    return true;
}

std::wstring PinyinConverter::GetPinyin(const std::wstring& chinese) {
    std::wstring pinyin;
    for (wchar_t ch : chinese) {
        auto it = m_pinyinDict.find(ch);
        if (it != m_pinyinDict.end())
            pinyin += it->second + L" ";
        else
            pinyin += ch;
    }
    return pinyin;
}

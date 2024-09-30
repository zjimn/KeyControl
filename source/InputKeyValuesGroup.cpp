#include "pch.h"
#include "KeyboardUtils.h"
#include "StringUtils.h"
#include "InputKeyValuesGroup.h"
#include <sstream>

std::vector<std::vector<CString> > InputKeyValuesGroup::getFirstKeyCodeValues() {
    std::vector<std::vector<CString> > result;
    for (const CString& keyStrs : firstValues) {
        std::vector<CString> keys = CStringUtils::SplitCStringBySpace(keyStrs, ' ');
        std::vector<CString> keyCodeStrs;
        for (CString key : keys) {
            std::string str = std::string(CT2A(key));
            int keyCode = KeyboardUtils::StringToKeyCode(key);
            std::string tempStr = std::to_string(keyCode);
            CString keyCodeCStr(tempStr.c_str());
            keyCodeStrs.push_back(keyCodeCStr);
        }
    }
    return result;
}

std::string InputKeyValuesGroup::ToString() const {
    std::string result;
    for (const auto& values : allValues) {
        result += values.ToString() + "\n";
    }
    return result;
}

InputKeyValuesGroup InputKeyValuesGroup::FromString(const std::string& str) {
    InputKeyValuesGroup group;
    std::string data = str;
    CString firstValue;
    size_t pos = data.find('\n');
    while (pos != std::string::npos) {
        std::string token = data.substr(0, pos);
        if (!token.empty()) {
            InputKeyValues inputKeyValues;
            group.allValues.push_back(inputKeyValues.FromString(token));
            allValues.push_back(inputKeyValues.FromString(token));
            group.firstValues.push_back(inputKeyValues.edit1Value);
            firstValues.push_back(inputKeyValues.edit1Value);
        }
        data = data.substr(pos + 1);
        pos = data.find('\n');
    }
    return group;
}

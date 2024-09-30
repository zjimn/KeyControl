#include "pch.h"
#include "InputKeyValues.h"
#include "StringUtils.h"
#include <sstream>

std::string InputKeyValues::ToString() const {
    std::stringstream ss;
    std::string first = CStringUtils::Trim(std::string(CT2CA(edit1Value)));
    std::string second = CStringUtils::Trim(std::string(CT2CA(edit2Value)));
    ss << first << " -> " << second << " : " << isOpen;
    return ss.str();
}

InputKeyValues InputKeyValues::FromString(const std::string &str) {
    InputKeyValues values;
    size_t pos1 = str.find("->");
    if (pos1 != std::string::npos) {
        std::string part1 = CStringUtils::Trim(str.substr(0, pos1));
        size_t pos2 = str.find(":");
        std::string part2 = CStringUtils::Trim(str.substr(pos1 + 2, pos2 - pos1 - 2));
        std::string isOpenStr = CStringUtils::Trim(str.substr(pos2 + 1));
        isOpen = CStringUtils::StringToInt(isOpenStr);
        edit1Value = CString(part1.c_str());
        edit2Value = CString(part2.c_str());
        values.edit1Value = edit1Value;
        values.edit2Value = edit2Value;
        values.isOpen = isOpen;
    }
    return values;
}

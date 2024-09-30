#pragma once
#include <afxwin.h>
#include <vector>
#include "InputKeyValues.h"

class InputKeyValuesGroup {
public:
    std::vector<CString> firstValues;
    std::vector<InputKeyValues> allValues;

    std::vector<std::vector<CString> > getFirstKeyCodeValues();

    InputKeyValuesGroup() = default;

    InputKeyValuesGroup(const std::vector<CString> &firstVals, const std::vector<InputKeyValues> &values)
        : firstValues(firstVals), allValues(values) {
    }

    std::string ToString() const;

    InputKeyValuesGroup FromString(const std::string &str);

private:
    std::vector<std::vector<CString> > firstKeyCodeValues;
};

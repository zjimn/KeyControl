#pragma
#include <string>

class InputKeyValues {
public:
    CString edit1Value;
    CString edit2Value;
    int isOpen;

    InputKeyValues() = default;

    InputKeyValues(const CString &edit1Val, const CString &edit2Val, const bool isOpen)
        : edit1Value(edit1Val), edit2Value(edit2Val), isOpen(isOpen) {
    }

    std::string ToString() const;

    InputKeyValues FromString(const std::string &str);
};

#pragma once
#ifndef VKTOAHKCONVERTER_H
#define VKTOAHKCONVERTER_H
#include <unordered_map>
#include <windows.h>
#include <string>

class VkToAHKConverter {
public:
    static std::string ConvertVkToAHKString(UINT vkCode);

private:
    static void InitializeMapping();

    static int charToVkCode(char character);

    static std::unordered_map<UINT, std::string> vkMapping;
    static bool initialized;
};
#endif

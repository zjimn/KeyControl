#include "pch.h"
#include "ProcessLauncher.h"
#include <windows.h>
#include <iostream>

std::wstring StringToWString(const std::string& str) {
    std::wstring wstr(str.begin(), str.end());
    return wstr;
}

ProcessLauncher::ProcessLauncher(const std::string& executable, const std::string& arguments)
    : executable_(executable), arguments_(arguments) {
}

bool ProcessLauncher::Launch() {
    STARTUPINFO si = { 0 };
    PROCESS_INFORMATION pi = { 0 };
    si.cb = sizeof(STARTUPINFO);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
    std::wstring wExecutable = StringToWString(executable_);
    std::wstring wArguments = StringToWString(arguments_);
    std::wstring wCommandLine = wExecutable + L" " + wArguments;
    if (!CreateProcessW(
        NULL,
        &wCommandLine[0],
        NULL,
        NULL,
        FALSE,
        CREATE_NO_WINDOW,
        NULL,
        NULL,
        &si,
        &pi
    )) {
        std::cerr << "CreateProcessW failed with error code: " << GetLastError() << std::endl;
        return false;
    }
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    std::cout << "Execution successful." << std::endl;
    return true;
}

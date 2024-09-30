#include "pch.h"
#include "ConfigFileHandler.h"
#include <Windows.h>
#include <fstream>
#include <ShlObj.h> 
#include <string>
#include <AclAPI.h>
#include <iostream>
#include <sstream>

#include "StringUtils.h"

ConfigFileHandler::ConfigFileHandler(const std::string& filename)
    : m_filename(filename){
}

ConfigFileHandler::ConfigFileHandler(){
}

std::string ConfigFileHandler::GetDocumentsDirectory() const
{
    WCHAR path[MAX_PATH];
    if (SHGetFolderPathW(nullptr, CSIDL_PERSONAL, nullptr, 0, path) == S_OK)
    {
        std::wstring widePath(path);
        std::string narrowPath = CStringUtils::WStringToString(widePath); // 进行转换
        return narrowPath + "\\KeyControl";
    }
    return "";
}

static std::wstring GetExeDirectory()
{
    wchar_t exePath[MAX_PATH];
    if (GetModuleFileName(NULL, exePath, MAX_PATH))
    {
        std::wstring exeDir(exePath);
        size_t pos = exeDir.find_last_of(L"\\/");
        std::wstring path = (pos != std::wstring::npos) ? exeDir.substr(0, pos) : exeDir;
        return path;
    }
    return L"";
}

bool ConfigFileHandler::CreateDir(const std::string& path) const{
    return ::CreateDirectoryA(path.c_str(), nullptr) || GetLastError() == ERROR_ALREADY_EXISTS;
}

bool ConfigFileHandler::CreateDir(const std::wstring& path) const{
    return ::CreateDirectoryW(path.c_str(), nullptr) || GetLastError() == ERROR_ALREADY_EXISTS;
}

static std::wstring GetAppDataPath() {
    WCHAR path[MAX_PATH];
    HRESULT result = SHGetFolderPathW(NULL, CSIDL_APPDATA, NULL, 0, path);
    if (SUCCEEDED(result)) {
        return std::wstring(path);
    }
    else {
        std::wcerr << L"Failed to get AppData path. Error code: " << result << std::endl;
        return L"";
    }
}

bool ConfigFileHandler::SaveData(const std::string data){
    std::wstring appDataPath = GetAppDataPath();
    std::wstring dir = appDataPath + L"\\KeyControl";
    if (dir.empty() || !CreateDir(dir))
    {
        return false;
    }
    std::wstring fullPath = dir + L"\\" + std::wstring(m_filename.begin(), m_filename.end());
    std::ofstream ofs(fullPath);
    if (!ofs.is_open())
    {
        return false;
    }
    ofs << data << std::endl;
    ofs.close();
    return true;
}

static bool SetFolderPermissions(const std::wstring& folderPath) {
    PSID pEveryoneSID = NULL;
    PACL pDACL = NULL;
    PSECURITY_DESCRIPTOR pSD = NULL;
    SID_IDENTIFIER_AUTHORITY SIDAuthWorld = SECURITY_WORLD_SID_AUTHORITY;
    if (!AllocateAndInitializeSid(&SIDAuthWorld, 1, SECURITY_WORLD_RID, 0, 0, 0, 0, 0, 0, 0, &pEveryoneSID)) {
        std::cerr << "AllocateAndInitializeSid Error: " << GetLastError() << std::endl;
        return false;
    }
    if (GetNamedSecurityInfoW(folderPath.c_str(), SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, &pDACL, NULL, &pSD) != ERROR_SUCCESS) {
        std::cerr << "GetNamedSecurityInfo Error: " << GetLastError() << std::endl;
        FreeSid(pEveryoneSID);
        return false;
    }
    EXPLICIT_ACCESS ea;
    ZeroMemory(&ea, sizeof(EXPLICIT_ACCESS));
    ea.grfAccessPermissions = FILE_ALL_ACCESS;
    ea.grfAccessMode = SET_ACCESS;
    ea.grfInheritance = SUB_CONTAINERS_AND_OBJECTS_INHERIT;
    ea.Trustee.TrusteeForm = TRUSTEE_IS_SID;
    ea.Trustee.TrusteeType = TRUSTEE_IS_GROUP;
    ea.Trustee.ptstrName = (LPTSTR)pEveryoneSID;
    PACL pNewDACL = NULL;
    if (SetEntriesInAclW(1, &ea, pDACL, &pNewDACL) != ERROR_SUCCESS) {
        std::cerr << "SetEntriesInAcl Error: " << GetLastError() << std::endl;
        LocalFree(pSD);
        FreeSid(pEveryoneSID);
        return false;
    }
    if (SetNamedSecurityInfoW(const_cast<LPWSTR>(folderPath.c_str()), SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, pNewDACL, NULL) != ERROR_SUCCESS) {
        std::cerr << "SetNamedSecurityInfo Error: " << GetLastError() << std::endl;
        LocalFree(pNewDACL);
        LocalFree(pSD);
        FreeSid(pEveryoneSID);
        return false;
    }
    LocalFree(pNewDACL);
    LocalFree(pSD);
    FreeSid(pEveryoneSID);
    return true;
}

bool ConfigFileHandler::LoadData(std::string& data) const {
    std::wstring appDataPath = GetAppDataPath();
    std::wstring dir = appDataPath + L"\\KeyControl";
    if (dir.empty() || !CreateDir(dir))
    {
        return false;
    }
    std::wstring fullPath = dir + L"\\" + std::wstring(m_filename.begin(), m_filename.end());
    std::ifstream file(fullPath);
    if (!file.is_open()) {
        return false;  
    }
    std::ostringstream oss;
    oss << file.rdbuf();  
    data = oss.str();
    file.close();
    return true;
}

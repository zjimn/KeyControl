#pragma once
#ifndef __AFXWIN_H__
#error "Including 'pch.h' before this file is required to generate the PCH"
#endif

class CKeyControlApp : public CWinApp {
public:
    CKeyControlApp();

    virtual BOOL InitInstance();

    int ExitInstance();

    DECLARE_MESSAGE_MAP()

private:
    BOOL CheckSingleInstance();

    void BringExistingInstanceToFront();

    HWND m_hWnd;
    HANDLE m_hMutex;
    ULONG_PTR m_gdiplusToken;
};

extern CKeyControlApp theApp;

#pragma once
#ifndef __AFXWIN_H__
#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
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

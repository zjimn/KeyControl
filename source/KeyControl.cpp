#include "pch.h"
#include "framework.h"
#include "KeyControl.h"
#include "KeyControlDlg.h"
#include "resource.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CKeyControlApp, CWinApp)
    ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CKeyControlApp::CKeyControlApp() {
    m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}

CKeyControlApp theApp;

BOOL CKeyControlApp::InitInstance() {
    CWinApp::InitInstance();
    WNDCLASS wc;
    ::GetClassInfo(AfxGetInstanceHandle(), _T("#32770"), &wc);
    wc.lpszClassName = _T("KeyControlClass");
    AfxRegisterClass(&wc);
    ::GetClassInfo(AfxGetInstanceHandle(), _T("#32770"), &wc);
    wc.lpszClassName = _T("KeyControlSettingClass");
    AfxRegisterClass(&wc);
    if (!CheckSingleInstance()) {
        BringExistingInstanceToFront();
        return FALSE;
    }
    AfxEnableControlContainer();
    CShellManager* pShellManager = new CShellManager;
    CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
    SetRegistryKey(_T("Jim\\KeyControlApp"));
    Gdiplus::GdiplusStartupInput gdiplusStartupin;
    Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupin, NULL);
    CKeyControlDlg dlg;
    m_pMainWnd = &dlg;
    INT_PTR nResponse = dlg.DoModal();
    if (nResponse == IDOK) {
    }
    else if (nResponse == IDCANCEL) {
    }
    if (pShellManager != nullptr) {
        delete pShellManager;
    }
    return FALSE;
}

int CKeyControlApp::ExitInstance() {
    GdiplusShutdown(m_gdiplusToken);
    return CWinApp::ExitInstance();
}

BOOL CKeyControlApp::CheckSingleInstance() {
    m_hMutex = CreateMutex(NULL, TRUE, _T("KeyControlMutex"));
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        return FALSE;
    }
    return TRUE;
}

void CKeyControlApp::BringExistingInstanceToFront() {
    HWND hExistingWnd = FindWindow(_T("KeyControlClass"), NULL);
    if (hExistingWnd) {
        ::SendMessage(hExistingWnd, ID_SEND_WINDOW_FRONT_MESSAGE, 0, 0);
    }
}

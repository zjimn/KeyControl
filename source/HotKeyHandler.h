#pragma once
#include <afxwin.h>
#include <map>
#include <vector>

class CHotKeyHandler {
public:
    CHotKeyHandler();

    CHotKeyHandler(const CString &hotKeyStr1, const CString &hotKeyStr2);

    bool RegisterGlobalHotKey(HWND hWnd, UINT nHotKeyId);

    bool UnRegisterGlobalHotKey(HWND hWnd, UINT nHotKeyId);

    bool ParseMapKeyString(const CString &hotKeyStr, std::vector<UINT> &keys);

    std::vector<UINT> GetMapKeyCodes();

private:
    UINT m_nKey1;
    UINT m_nKey2;
    std::vector<UINT> m_mapKeyCodes;
    UINT m_modifiers;
    HWND m_hwnd;
    bool m_parseHotKeyState;
    bool m_parseMapKeyState;

    bool ParseHotKeyString(const CString &hotKeyStr, UINT &key, UINT &modifiers);
};

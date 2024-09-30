#pragma once
#include <set>

class CHotKeyPressManager {
public:
    CHotKeyPressManager();

    ~CHotKeyPressManager();

    void AddHotKeyId(UINT nHotKeyId);

    void RemoveHotKeyId(UINT nHotKeyId);

    bool IsHotKeyPressed(UINT nHotKeyId) const;

private:
    std::set<UINT> m_hotKeyIds;
};

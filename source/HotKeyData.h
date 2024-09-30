#pragma once
#include <map>
#include <vector>

class CHotKeyData {
public:
    void SaveHotKeyData(UINT nHotKeyId, const std::vector<UINT> &keyCollection);

    bool GetHotKeyCodes(UINT nHotKeyId, std::vector<UINT> &keyCollection) const;

    std::vector<UINT> GetHotKeyIds() const;

    void DeleteHotKeyData(UINT nHotKeyId);

private:
    std::map<UINT, std::vector<UINT> > m_hotKeyData;
};

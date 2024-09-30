#include "pch.h"
#include "HotKeyData.h"

void CHotKeyData::SaveHotKeyData(UINT nHotKeyId, const std::vector<UINT>& keyCollection) {
    m_hotKeyData[nHotKeyId] = keyCollection;
}

bool CHotKeyData::GetHotKeyCodes(UINT nHotKeyId, std::vector<UINT>& keyCollection) const {
    auto it = m_hotKeyData.find(nHotKeyId);
    if (it != m_hotKeyData.end()) {
        keyCollection = it->second;
        return true;
    }
    return false;
}

std::vector<UINT> CHotKeyData::GetHotKeyIds() const {
    std::vector<UINT> keys;
    for (const auto& pair : m_hotKeyData) {
        keys.push_back(pair.first);
    }
    return keys;
}

void CHotKeyData::DeleteHotKeyData(UINT nHotKeyId) {
    auto it = m_hotKeyData.find(nHotKeyId);
    if (it != m_hotKeyData.end()) {
        m_hotKeyData.erase(it);
    }
}

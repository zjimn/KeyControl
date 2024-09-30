#include "pch.h"
#include "HotKeyPressManager.h"

CHotKeyPressManager::CHotKeyPressManager() {
}

CHotKeyPressManager::~CHotKeyPressManager() {
}

void CHotKeyPressManager::AddHotKeyId(UINT nHotKeyId) {
    m_hotKeyIds.insert(nHotKeyId);
}

void CHotKeyPressManager::RemoveHotKeyId(UINT nHotKeyId) {
    m_hotKeyIds.erase(nHotKeyId);
}

bool CHotKeyPressManager::IsHotKeyPressed(UINT nHotKeyId) const {
    return m_hotKeyIds.find(nHotKeyId) != m_hotKeyIds.end();
}

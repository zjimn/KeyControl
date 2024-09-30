#pragma once
#pragma once
#include <afxwin.h>
#include <vector>

struct ListItem {
    HWND hwnd;
    CString title;
};

enum class EventType {
    MouseDown = 1,
    MouseUp = 2
};

struct ItemIndexData {
    HWND hwnd;
    int nIndex;
    EventType eventType;
};

class CCustomListBox : public CListBox {
public:
    CCustomListBox();

    void AddWindowItem(const HWND hwnd, CString title, HICON hIcon);

    void AddWindowItem(const ListItem &item, HICON hIcon);

    void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) override;

    void SetParentWnd(CWnd *pWnd) { m_pParentWnd = pWnd; }

    ListItem GetItem(int index) const;

protected:
    void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) override;

    void PreSubclassWindow() override;

    void OnLButtonDown(UINT nFlags, CPoint point);

    void OnLButtonUp(UINT nFlags, CPoint point);

    void SendItemIndexMessage(CPoint point, EventType eventType);

    void OnPaint();

    void OnMouseMove(UINT nFlags, CPoint point);

    void OnMouseLeave();

    void OnTimer(UINT_PTR nIDEvent);

    DECLARE_MESSAGE_MAP()

private:
    CFont m_font;
    CWnd *m_pParentWnd;
    CArray<HICON, HICON> m_icons;
    std::vector<ListItem> m_itemList;
    CMap<int, int, ListItem, ListItem> m_itemMap;
    int m_nHoverIndex;
    BOOL m_bTracking;
};

#include "pch.h"
#include "CustomListBox.h"
#include "resource.h"

BEGIN_MESSAGE_MAP(CCustomListBox, CListBox)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()

CCustomListBox::CCustomListBox() {
}

void CCustomListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) {
    lpMeasureItemStruct->itemHeight = 30;
}

void CCustomListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) {
    if (lpDrawItemStruct == nullptr || lpDrawItemStruct->itemID == -1) {
        return;
    }
    CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
    CString strText;
    GetText(lpDrawItemStruct->itemID, strText);
    if (lpDrawItemStruct->itemState & ODS_SELECTED)
        pDC->SetTextColor(RGB(255, 255, 255));
    else
        pDC->SetTextColor(RGB(80, 80, 80));
    if (lpDrawItemStruct->itemState & ODS_SELECTED)
        pDC->SetBkColor(RGB(0, 120, 215));
    else
        pDC->SetBkColor(RGB(255, 255, 255));
    CRect rect = lpDrawItemStruct->rcItem;
    COLORREF bgColor;
    if (lpDrawItemStruct->itemID % 2 == 0) {
        bgColor = RGB(255, 255, 255);
    }
    else {
        bgColor = RGB(240, 240, 240);
    }
    int nItem = lpDrawItemStruct->itemID;
    if (nItem == m_nHoverIndex) {
        bgColor = RGB(230, 230, 230);
    }
    if (lpDrawItemStruct->itemState & ODS_SELECTED) {
        bgColor = RGB(173, 216, 230);
    }
    pDC->FillSolidRect(&rect, bgColor);
    CFont* font = new CFont();
    pDC->SelectObject(font);
    int iconSize = 16;
    int itemHeight = lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top;
    int iconY = lpDrawItemStruct->rcItem.top + (itemHeight - iconSize) / 2;
    HICON hIcon = m_icons[lpDrawItemStruct->itemID];
    if (hIcon) {
        DrawIconEx(pDC->GetSafeHdc(), lpDrawItemStruct->rcItem.left + 10, iconY,
            hIcon, iconSize, iconSize, 0, nullptr, DI_NORMAL);
    }
    CRect textRect = lpDrawItemStruct->rcItem;
    textRect.DeflateRect(40, 0, 20, 0);
    pDC->DrawText(strText, &textRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    CPen pen(PS_SOLID, 1, RGB(200, 200, 200));
    CPen* pOldPen = pDC->SelectObject(&pen);
    int y = lpDrawItemStruct->rcItem.bottom - 1;
    pDC->MoveTo(lpDrawItemStruct->rcItem.left, y);
    pDC->LineTo(lpDrawItemStruct->rcItem.right, y);
    pDC->SelectObject(pOldPen);
    if (lpDrawItemStruct->itemState & ODS_FOCUS) {
        pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);
    }
}

void CCustomListBox::PreSubclassWindow() {
    CListBox::PreSubclassWindow();
    if (!GetSafeHwnd())
        return;
    CFont font;
    LOGFONT logFont;
    memset(&logFont, 0, sizeof(LOGFONT));
    lstrcpy(logFont.lfFaceName, _T("Microsoft YaHei"));
    logFont.lfHeight = 20;
    logFont.lfWeight = FW_NORMAL;
    font.CreateFontIndirect(&logFont);
    SetFont(&font);
}

void CCustomListBox::OnLButtonDown(UINT nFlags, CPoint point) {
    int nCount = GetCount();
    BOOL bOutside = TRUE;
    for (int i = 0; i < nCount; ++i) {
        CRect rect;
        GetItemRect(i, &rect);
        if (rect.PtInRect(point)) {
            bOutside = FALSE;
            break;
        }
    }
    if (bOutside) {
        return;
    }
    SendItemIndexMessage(point, EventType::MouseDown);
    CListBox::OnLButtonDown(nFlags, point);
}

void CCustomListBox::OnLButtonUp(UINT nFlags, CPoint point) {
    SendItemIndexMessage(point, EventType::MouseUp);
    CListBox::OnLButtonUp(nFlags, point);
}

void CCustomListBox::SendItemIndexMessage(CPoint point, EventType eventType) {
    BOOL bOutside = TRUE;
    int nIndex = ItemFromPoint(point, bOutside);
    if (nIndex != LB_ERR) {
        HWND hwnd = 0;
        ListItem item = { nullptr, _T("") };
        if (m_itemMap.Lookup(nIndex, item)) {
            hwnd = item.hwnd;
        }
        ItemIndexData indexData;
        indexData.hwnd = hwnd;
        indexData.nIndex = nIndex;
        indexData.eventType = eventType;
        if (m_pParentWnd) {
            COPYDATASTRUCT cds;
            cds.dwData = 0;
            cds.cbData = sizeof(indexData);
            cds.lpData = &indexData;
            m_pParentWnd->SendMessage(WM_ITEM_CLICKED, (WPARAM)WM_ITEM_CLICKED, (LPARAM)&cds);
        }
    }
}

void CCustomListBox::AddWindowItem(const HWND hwnd, CString title, HICON hIcon) {
    int index = InsertString(GetCount(), title);
    if (index >= 0) {
        m_icons.InsertAt(index, hIcon);
    }
    ListItem item;
    item.hwnd = hwnd;
    item.title = title;
    m_itemMap.SetAt(index, item);
}

void CCustomListBox::AddWindowItem(const ListItem& item, HICON hIcon) {
    AddWindowItem(item.hwnd, item.title, hIcon);
}

ListItem CCustomListBox::GetItem(int index) const {
    ListItem item = { nullptr, _T("") };
    if (m_itemMap.Lookup(index, item)) {
        return item;
    }
    return item;
}

void CCustomListBox::OnPaint() {
    CPaintDC dc(this);
    CListBox::OnPaint();
    CRect rect;
    GetClientRect(&rect);
    rect.DeflateRect(1, 1);
    CPen pen(PS_SOLID, 1, RGB(100, 100, 100));
    CPen* pOldPen = dc.SelectObject(&pen);
    dc.Rectangle(rect);
    dc.SelectObject(pOldPen);
}

void CCustomListBox::OnMouseMove(UINT nFlags, CPoint point) {
    BOOL bOutside = FALSE;
    int nItem = ItemFromPoint(point, bOutside);
    if (!bOutside && nItem != m_nHoverIndex) {
        if (m_nHoverIndex != -1) {
            CRect oldRect;
            GetItemRect(m_nHoverIndex, &oldRect);
            InvalidateRect(&oldRect, FALSE);
        }
        m_nHoverIndex = nItem;
        CRect newRect;
        GetItemRect(nItem, &newRect);
        InvalidateRect(&newRect, FALSE);
        UpdateWindow();
    }
    if (!m_bTracking) {
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(tme);
        tme.dwFlags = TME_LEAVE;
        tme.hwndTrack = m_hWnd;
        if (TrackMouseEvent(&tme)) {
            m_bTracking = TRUE;
        }
    }
    CListBox::OnMouseMove(nFlags, point);
}

void CCustomListBox::OnMouseLeave() {
    m_nHoverIndex = -1;
    Invalidate();
    m_bTracking = FALSE;
    CListBox::OnMouseLeave();
}

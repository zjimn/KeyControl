#include "pch.h"
#include "SearchEdit.h"
#include <gdiplus.h>
#include <gdipluseffects.h>

BEGIN_MESSAGE_MAP(CSearchEdit, CEdit)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

CSearchEdit::CSearchEdit()
{
	m_clrBorderNormal = RGB(122, 122, 122);
	m_clrBorderHover = RGB(51, 139, 192);
	m_strFaceName = L"Microsoft YaHei Regular";
	m_nPointSize = 100;
	m_Font.CreatePointFont(
		m_nPointSize, m_strFaceName);
	m_strGrayString = L"请输入文本";
	m_nFocusStyle = FOCUS_DEFAULT;
	m_bHover = false;
	m_bTrack = true;
	m_bUser = false;
	m_bEmpty = true;
}

CSearchEdit::~CSearchEdit() {
}

HBRUSH CSearchEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
	HBRUSH hbr;
	hbr = ::CreateSolidBrush(RGB(255, 255, 255)); 
	if (m_bEmpty)pDC->SetTextColor(RGB(100, 100, 100));
	else
		pDC->SetTextColor(RGB(0, 0, 0));
	return hbr;
}

void CSearchEdit::OnNcPaint() {
}

void CSearchEdit::OnMouseMove(UINT nFlags, CPoint point) {
	if (m_bTrack) {
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_HOVER | TME_LEAVE;
		tme.hwndTrack = this->GetSafeHwnd();
		tme.dwHoverTime = HOVER_DEFAULT;
		::TrackMouseEvent(&tme);
		m_bTrack = false;
	}
	m_bHover = true;
	CEdit::OnMouseMove(nFlags, point);
}

void CSearchEdit::OnMouseLeave(UINT nFlags, CPoint point) {
	m_bHover = false;
	m_bTrack = false;
	CEdit::OnMouseLeave();
}

void CSearchEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) {
	m_bUser = true;
	CString str;
	GetWindowText(str);
	if (str.GetLength() <= 2)
		this->Invalidate();
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CSearchEdit::OnEnChange() {
	if (m_bUser) {
		CString str;
		GetWindowText(str);
		m_bEmpty = str.IsEmpty();
	}
}

void CSearchEdit::OnSetFocus(CWnd* pOldWnd) {
	CEdit::OnSetFocus(pOldWnd);
	if (m_bEmpty) {
		m_bUser = false;
	}
}

void CSearchEdit::OnkillFocus(CWnd* pNewWnd) {
	CEdit::OnKillFocus(pNewWnd);
	m_bUser = false;
	if (m_bEmpty)
		SetWindowTextW(m_strGrayString);
}

void CSearchEdit::DrawLoupe(CDC*pDC, CPoint pt) {
	DrawLoupe(pDC, pt, 22, 22);
}

void CSearchEdit::DrawLoupe(CDC* pDC, CPoint pt, int width, int height) {
	CWnd* pParent = GetParent();
	HDC hdc = ::GetDC(pParent->m_hWnd);
	Graphics graphics(hdc);
	Gdiplus::Bitmap* pBitmap = LoadBitmapFromIconResource(m_iConId);
	CRect rcWindow;
	GetWindowRect(rcWindow);
	pParent->ScreenToClient(rcWindow);
	Rect targetRect(
		static_cast<INT>(rcWindow.left + pt.x),  
		static_cast<INT>(rcWindow.top + pt.y),   
		static_cast<INT>(width),                 
		static_cast<INT>(height)                 
	);
	graphics.DrawImage(pBitmap, targetRect);
	delete pBitmap;
	::ReleaseDC(pParent->m_hWnd, hdc);
}

Gdiplus::Bitmap* CSearchEdit::LoadBitmapFromIconResource(int resourceId) {
	HICON hIcon = LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(resourceId));
	if (!hIcon) {
		return nullptr; 
	}
	ICONINFO iconInfo;
	if (!GetIconInfo(hIcon, &iconInfo)) {
		DestroyIcon(hIcon);
		return nullptr; 
	}
	HBITMAP hBitmap = iconInfo.hbmColor ? iconInfo.hbmColor : iconInfo.hbmMask;
	Gdiplus::Bitmap* pBitmap = Gdiplus::Bitmap::FromHBITMAP(hBitmap, nullptr);
	DestroyIcon(hIcon);
	if (iconInfo.hbmColor) {
		DeleteObject(iconInfo.hbmColor);
	}
	if (iconInfo.hbmMask) {
		DeleteObject(iconInfo.hbmMask);
	}
	return pBitmap;
}

BOOL CSearchEdit::OnEraseBkgnd(CDC*pDC)
{
	COLORREF clr; if (m_bHover)
		clr = m_clrBorderHover; else
		clr = m_clrBorderNormal;
	CPen pen; pen.CreatePen(PS_SOLID, 1, clr);
		CRect rctBorder;
		GetWindowRect(&rctBorder);
	m_rectEdit = rctBorder; 
	ScreenToClient(&rctBorder); 
CPen*OldPen;
	rctBorder.left -= 15;
	rctBorder.right += 50;
	rctBorder.top -= 10;
	rctBorder.bottom += 8;
	OldPen = pDC->SelectObject(&pen);
	pDC->Rectangle(&rctBorder);
	pDC->SelectObject(OldPen);
	CPoint ptLoupe;
	ptLoupe.x = rctBorder.right - 30;
	ptLoupe.y = rctBorder.top + 8; 
	pen.DeleteObject();
	DrawLoupe(pDC, ptLoupe);
	return CEdit::OnEraseBkgnd(pDC);
}

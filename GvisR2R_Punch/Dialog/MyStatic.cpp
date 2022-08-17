// MyStatic.cpp : implementation file
//

#include "stdafx.h"
#include "../GvisR2R_Punch.h"
#include "MyStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyStatic

CMyStatic::CMyStatic()
{
	m_crText = GetSysColor(COLOR_WINDOWTEXT);
// 	m_hBrush = ::CreateSolidBrush(GetSysColor(COLOR_3DFACE));
	m_hBrush = NULL;

	::GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT),sizeof(m_lf),&m_lf);

	m_bFont = FALSE;
	BOOL bCr = m_font.CreateFontIndirect(&m_lf);
	if(bCr)
		m_bFont = TRUE;

	m_bState = FALSE;
	m_hCursor = NULL;

	m_hParentWnd = NULL;
	m_nCtrlID = NULL; 

//	m_hwndBrush = ::CreateSolidBrush(GetSysColor(COLOR_3DFACE));

}

CMyStatic::~CMyStatic()
{
	if(m_bFont)
	{
		if(m_font.DeleteObject())
			m_bFont = FALSE;
	}
	if(::DeleteObject(m_hBrush))
		m_hBrush = FALSE;
// 	if(::DeleteObject(m_hwndBrush))
// 		m_hwndBrush = FALSE;
}


BEGIN_MESSAGE_MAP(CMyStatic, CStatic)
	//{{AFX_MSG_MAP(CMyStatic)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyStatic message handlers

HBRUSH CMyStatic::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a non-NULL brush if the parent's handler should not be called

	if (CTLCOLOR_STATIC == nCtlColor)
	{
		pDC->SelectObject(&m_font);
		pDC->SetTextColor(m_crText);
		pDC->SetBkMode(TRANSPARENT);
		
		if(m_hParentWnd && m_nCtrlID)
			::PostMessage(m_hParentWnd, WM_MYSTATIC_REDRAW, (WPARAM)NULL, (LPARAM)m_nCtrlID);
	}

	return m_hBrush;
	//return NULL;
}

void CMyStatic::SetHwnd(HWND hParentWnd, int nCtrlID)
{
	m_hParentWnd = hParentWnd;
	m_nCtrlID = nCtrlID;
}

void CMyStatic::SetText(const CString& strText)
{
	m_strText = strText;
	SetWindowText(strText);
	return;
}

void CMyStatic::SetTextColor(COLORREF crText)
{
	m_crText = crText;
	RedrawWindow();
	return;
}

void CMyStatic::SetFontBold(BOOL bBold)
{	
	m_bBold = bBold;
	m_lf.lfWeight = bBold ? FW_BOLD : FW_NORMAL;
	ReconstructFont();
	RedrawWindow();
	return;
}

void CMyStatic::SetFontUnderline(BOOL bSet)
{	
	m_lf.lfUnderline = bSet;
	ReconstructFont();
	RedrawWindow();
	return;
}

void CMyStatic::SetFontItalic(BOOL bSet)
{
	m_lf.lfItalic = bSet;
	ReconstructFont();
	RedrawWindow();
	return;	
}

void CMyStatic::SetSunken(BOOL bSet)
{
	if (!bSet)
		ModifyStyleEx(WS_EX_STATICEDGE,0,SWP_DRAWFRAME);
	else
		ModifyStyleEx(0,WS_EX_STATICEDGE,SWP_DRAWFRAME);
		
	return;	
}

void CMyStatic::SetBorder(BOOL bSet)
{
	if (!bSet)
		ModifyStyle(WS_BORDER,0,SWP_DRAWFRAME);
	else
		ModifyStyle(0,WS_BORDER,SWP_DRAWFRAME);
		
	return;	
}

void CMyStatic::SetFontSize(int nSize)
{
	m_fontSize = nSize;
	nSize*=-1;
	m_lf.lfHeight = nSize;
	ReconstructFont();
	RedrawWindow();
	return;
}

void CMyStatic::SetBkColor(COLORREF crBkgnd)
{
	m_crBkgnd = crBkgnd;
	
	if (m_hBrush)
	{
		if(::DeleteObject(m_hBrush))
			m_hBrush = FALSE;
	}

	if(!m_hBrush)
		m_hBrush = ::CreateSolidBrush(crBkgnd);

	RedrawWindow();
	return;
}

void CMyStatic::SetFontName(const CString& strFont)
{	
	m_fontName = strFont;
	//strcpy(m_lf.lfFaceName,strFont);
	_stprintf(m_lf.lfFaceName, _T("%s"), strFont);
	ReconstructFont();
	RedrawWindow();
	return;
}

void CMyStatic::ReconstructFont()
{
	if(m_bFont)
	{
		if(m_font.DeleteObject())
			m_bFont = FALSE;
	}
	
	BOOL bCreated = FALSE;
	if(!m_bFont)
	{
		bCreated = m_font.CreateFontIndirect(&m_lf);
		if(bCreated)
			m_bFont = TRUE;
	}

	ASSERT(bCreated);
}

void CMyStatic::Refresh()
{
	RedrawWindow();
}

void CMyStatic::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_hParentWnd && m_nCtrlID)
	{
		m_ptMouse = point;
		::PostMessage(m_hParentWnd, WM_MYSTATIC_MOUSE_MOVE, (WPARAM)NULL, (LPARAM)m_nCtrlID);
	}

	CStatic::OnMouseMove(nFlags, point);
}

CPoint CMyStatic::GetMousePoint()
{
	return m_ptMouse;
}

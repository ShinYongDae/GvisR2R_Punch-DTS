// MyEdit.cpp : implementation file
//

#include "stdafx.h"
#include "../gvisr2r_punch.h"
#include "MyEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyEdit

CMyEdit::CMyEdit()
{
	m_hParentWnd = NULL;
	m_nCtrlID = NULL; 

	m_crText = GetSysColor(COLOR_WINDOWTEXT);
	m_hBrush = ::CreateSolidBrush(GetSysColor(COLOR_3DFACE));

	::GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT),sizeof(m_lf),&m_lf);

	m_bFont = FALSE;
	BOOL bCr = m_font.CreateFontIndirect(&m_lf);
	if(bCr)
		m_bFont = TRUE;
}

CMyEdit::~CMyEdit()
{
	if(m_bFont)
	{
		if(m_font.DeleteObject())
			m_bFont = FALSE;
	}
	
	if(::DeleteObject(m_hBrush))
		m_hBrush = FALSE;
}


BEGIN_MESSAGE_MAP(CMyEdit, CEdit)
	//{{AFX_MSG_MAP(CMyEdit)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyEdit message handlers

HBRUSH CMyEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// TODO: Change any attributes of the DC here
	if (CTLCOLOR_STATIC == nCtlColor)
	{
		pDC->SelectObject(&m_font);
		pDC->SetTextColor(m_crText);
		pDC->SetBkMode(TRANSPARENT);
	}

	if (CTLCOLOR_EDIT == nCtlColor)
	{
		pDC->SelectObject(&m_font);
		pDC->SetTextColor(m_crText);
		pDC->SetBkMode(TRANSPARENT);
	}
	
	// TODO: Return a non-NULL brush if the parent's handler should not be called

	return m_hBrush;
}


void CMyEdit::SetText(const CString& strText)
{
	m_strText = strText;
	SetWindowText(strText);
	return;
}

void CMyEdit::SetTextColor(COLORREF crText)
{
	m_crText = crText;
	RedrawWindow();
	return;
}

void CMyEdit::SetFontBold(BOOL bBold)
{	
	m_bBold = bBold;
	m_lf.lfWeight = bBold ? FW_BOLD : FW_NORMAL;
	ReconstructFont();
	RedrawWindow();
	return;
}

void CMyEdit::SetFontUnderline(BOOL bSet)
{	
	m_lf.lfUnderline = bSet;
	ReconstructFont();
	RedrawWindow();
	return;
}

void CMyEdit::SetFontItalic(BOOL bSet)
{
	m_lf.lfItalic = bSet;
	ReconstructFont();
	RedrawWindow();
	return;	
}

void CMyEdit::SetSunken(BOOL bSet)
{
	if (!bSet)
		ModifyStyleEx(WS_EX_STATICEDGE,0,SWP_DRAWFRAME);
	else
		ModifyStyleEx(0,WS_EX_STATICEDGE,SWP_DRAWFRAME);
		
	return;	
}

void CMyEdit::SetBorder(BOOL bSet)
{
	if (!bSet)
		ModifyStyle(WS_BORDER,0,SWP_DRAWFRAME);
	else
		ModifyStyle(0,WS_BORDER,SWP_DRAWFRAME);
		
	return;	
}

void CMyEdit::SetFontSize(int nSize)
{
	m_fontSize = nSize;
	nSize*=-1;
	m_lf.lfHeight = nSize;
	ReconstructFont();
	RedrawWindow();
	return;
}

void CMyEdit::SetBkColor(COLORREF crBkgnd)
{
	m_crBkgnd = crBkgnd;
	
	if (m_hBrush)
	{
		if(::DeleteObject(m_hBrush))
			m_hBrush = FALSE;
	}

	if(!m_hBrush)
		m_hBrush = ::CreateSolidBrush(crBkgnd);

	return;
}

void CMyEdit::SetFontName(const CString& strFont)
{	
	m_fontName = strFont;
	_stprintf(m_lf.lfFaceName, _T("%s"), strFont);	//strcpy(m_lf.lfFaceName,strFont);
	ReconstructFont();
	RedrawWindow();
	return;
}

void CMyEdit::Refresh()
{
	RedrawWindow(NULL, NULL, RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_FRAME);
}

void CMyEdit::ReconstructFont()
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


void CMyEdit::SetHwnd(HWND hParentWnd, int nCtrlID)
{
	m_hParentWnd = hParentWnd;
	m_nCtrlID = nCtrlID;
}

void CMyEdit::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_hParentWnd && m_nCtrlID)
		::PostMessage(m_hParentWnd, WM_MYEDIT_VSCROLL, (WPARAM)NULL, (LPARAM)m_nCtrlID);
	
	CEdit::OnVScroll(nSBCode, nPos, pScrollBar);
}

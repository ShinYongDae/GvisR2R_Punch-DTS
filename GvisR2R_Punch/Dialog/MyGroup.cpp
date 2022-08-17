// MyGroup.cpp : implementation file
//

#include "stdafx.h"
#include "../gvisr2r_punch.h"
#include "MyGroup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyGroup

CMyGroup::CMyGroup()
{
	m_hParentWnd = NULL;
	m_nCtrlID = NULL; 
	m_hwndBrush = NULL;
	m_hBrush = NULL;
	m_bFont = FALSE;
	
	m_crText = GetSysColor(COLOR_WINDOWTEXT);
	m_hBrush = ::CreateSolidBrush(GetSysColor(COLOR_3DFACE));

	::GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT),sizeof(m_lf),&m_lf);

	m_bFont = TRUE;
	BOOL bCr = m_font.CreateFontIndirect(&m_lf);
	if(bCr)
		m_bFont = TRUE;
	
	m_hwndBrush = ::CreateSolidBrush(GetSysColor(COLOR_3DFACE));
}

CMyGroup::~CMyGroup()
{
	if(m_bFont)
	{
		if(m_font.DeleteObject())
			m_bFont = FALSE;
	}

	if (m_hBrush)
	{
		if(::DeleteObject(m_hBrush))
			m_hBrush = NULL;
	}
	if (m_hwndBrush)
	{
		if(::DeleteObject(m_hwndBrush))
			m_hwndBrush = NULL;
	}
}


BEGIN_MESSAGE_MAP(CMyGroup, CStatic)
	//{{AFX_MSG_MAP(CMyGroup)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyGroup message handlers

void CMyGroup::SetHwnd(HWND hParentWnd, int nCtrlID)
{
	m_hParentWnd = hParentWnd;
	m_nCtrlID = nCtrlID;
}


CRect CMyGroup::GetImgRect(CRect rClient)
{
	CRect r=rClient;

	CPoint centre=r.CenterPoint();

	//Center the icon 
	r.left	= r.left+2;
	r.right	= r.right-2;
	r.top	= r.top+2;
	r.bottom= r.bottom-2;

	return r;
}

void CMyGroup::SetFontName(const CString& strFont)
{	
	m_fontName = strFont;
	//strcpy(m_lf.lfFaceName,strFont);
	_stprintf(m_lf.lfFaceName, _T("%s"), strFont);
	ReconstructFont();
	RedrawWindow();
	return;
}

void CMyGroup::SetFontBold(BOOL bBold)
{	
	m_bBold = bBold;
	m_lf.lfWeight = bBold ? FW_BOLD : FW_NORMAL;
	ReconstructFont();
	RedrawWindow();
	return;
}

void CMyGroup::SetFontSize(int nSize)
{
	m_fontSize = nSize;
	nSize*=-1;
	m_lf.lfHeight = nSize;
	ReconstructFont();
	RedrawWindow();
	return;
}

void CMyGroup::ReconstructFont()
{
	if(m_bFont)
	{
		if(m_font.DeleteObject())
			m_bFont = FALSE;
	}

	BOOL bCreated = m_font.CreateFontIndirect(&m_lf);
	if(bCreated)
		m_bFont = TRUE;

	ASSERT(bCreated);
}

void CMyGroup::SetBkColor(COLORREF crBkgnd)
{
	m_crBkgnd = crBkgnd;
// 	RedrawWindow();
}

void CMyGroup::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	DrawRound(&dc);
	DrawBkColor(&dc, m_crBkgnd);
	DrawText(&dc);
	if(m_hParentWnd && m_nCtrlID)
		::PostMessage(m_hParentWnd, WM_MYGROUP_REDRAW, (WPARAM)NULL, (LPARAM)m_nCtrlID);

	// Do not call CStatic::OnPaint() for painting messages
}

void CMyGroup::DrawRound(CPaintDC *pDc)
{
	CRect rect(0,0,0,0);
	GetClientRect(&rect);
	CPoint pt;
	pt.x = 10;
	pt.y = 10;
	pDc->RoundRect(rect,pt);
}

void CMyGroup::DrawBkColor(CPaintDC *pDc, COLORREF crBkgnd)
{
	CRect rect(0,0,0,0);
	GetClientRect(&rect);

	CRect ImgRect = GetImgRect(rect);

	pDc->FillSolidRect(&ImgRect, crBkgnd);

}

void CMyGroup::DrawText(CDC* pDC)
{
    // Draw the text
	CString strText;
	GetWindowText(strText);

    if (!strText.IsEmpty())
    {
		HFONT hOldFont=NULL;
		//Sets the font
		if(m_font.GetSafeHandle() != NULL)
		{
			hOldFont =(HFONT) pDC->SelectObject(m_font.GetSafeHandle());
		}

        CSize Extent = pDC->GetTextExtent(strText);
		CRect rect(0,0,0,0), txtRect(0,0,0,0);
		GetClientRect(&rect);
		txtRect.left = rect.left+3;
		txtRect.top = rect.top+5;
		txtRect.right = rect.right-3;
		txtRect.bottom = txtRect.top + Extent.cy;

 		CPoint pt( txtRect.CenterPoint().x - Extent.cx/2, txtRect.CenterPoint().y - Extent.cy/2 );

		int nMode = pDC->SetBkMode(TRANSPARENT);
		COLORREF crTextOld = pDC->SetTextColor(m_crText);

		pDC->TextOut(pt.x, pt.y, strText);

        pDC->SetBkMode(nMode);

		//Reset the old font
		if(hOldFont != NULL)
		{
			pDC->SelectObject(hOldFont);
		}
    }

}

void CMyGroup::SetTextColor(COLORREF crText)
{
    m_crText = crText;
}

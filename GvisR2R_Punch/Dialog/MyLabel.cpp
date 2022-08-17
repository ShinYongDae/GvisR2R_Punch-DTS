// MyLabel.cpp : implementation file
//

#include "stdafx.h"
#include "../gvisr2r_punch.h"
#include "MyLabel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyLabel

CMyLabel::CMyLabel()
{
	m_hwndBrush = NULL;
	m_hBrush = NULL;
	
	m_hBitmap0 = NULL;
	m_hBitmap1 = NULL;
	m_hBitmapBk0 = NULL;
	m_hBitmapBk1 = NULL;
	m_hIcon0 = NULL;
	m_hIcon1 = NULL;

	m_pBitmap0 = NULL;
	m_pBitmap1 = NULL;
	m_pBitmapBk0 = NULL;
	m_pBitmapBk1 = NULL;
	m_pIcon0 = NULL;
	m_pIcon1 = NULL;

	m_bBoarder = FALSE;
	m_nImgPos = LBL_POS_LEFT;

	m_Size = CSize(0,0);
	m_nImg = 0;

	m_Image0 = NULL;
	m_Image1 = NULL;
	m_Bk0 = NULL;
	m_Bk1 = NULL;

	m_nBk[LBL_IMG_UP] = NULL;
	m_nBk[LBL_IMG_DN] = NULL;
	m_nImage[LBL_IMG_UP] = NULL;
	m_nImage[LBL_IMG_DN] = NULL;

	for(int i=0; i<2; i++)
	{
		m_nImage[i] = NULL;
		m_nBk[i] = NULL;
	}


	m_crText = GetSysColor(COLOR_WINDOWTEXT);
	m_hBrush = ::CreateSolidBrush(GetSysColor(COLOR_3DFACE));

	::GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT),sizeof(m_lf),&m_lf);

	m_bFont = FALSE;
	BOOL bCreated = m_font.CreateFontIndirect(&m_lf);
	if(bCreated)
		m_bFont = TRUE;

	m_bTimer = FALSE;
	m_bState = FALSE;
	m_bLink = TRUE;
	m_hCursor = NULL;
	m_Type = _None;

	m_hwndBrush = ::CreateSolidBrush(GetSysColor(COLOR_3DFACE));
}


CMyLabel::~CMyLabel()
{
	DelImgList();

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

void CMyLabel::DelImgList()
{
	if(m_hBitmap0)
	{
		if(DeleteObject(m_hBitmap0))
			m_hBitmap0 = NULL;
	}

	if(m_hBitmap1)
	{
		if(DeleteObject(m_hBitmap1))
			m_hBitmap1 = NULL;
	}

	if(m_hBitmapBk0)
	{
		if(DeleteObject(m_hBitmapBk0))
			m_hBitmapBk0 = NULL;
	}

	if(m_hBitmapBk1)
	{
		if(DeleteObject(m_hBitmapBk1))
			m_hBitmapBk1 = NULL;
	}

	if(m_hIcon0)
	{
		if(DestroyIcon(m_hIcon0))
			m_hIcon0 = NULL;
	}

	if(m_hIcon1)
	{
		if(DestroyIcon(m_hIcon1))
			m_hIcon1 = NULL;
	}


	if(m_pBitmap0)
	{
		delete m_pBitmap0;
		m_pBitmap0 = NULL;
	}

	if(m_pBitmap1)
	{
		delete m_pBitmap1;
		m_pBitmap1 = NULL;
	}

	if(m_pBitmapBk0)
	{
		delete m_pBitmapBk0;
		m_pBitmapBk0 = NULL;
	}

	if(m_pBitmapBk1)
	{
		delete m_pBitmapBk1;
		m_pBitmapBk1 = NULL;
	}

	if(m_pIcon0)
	{
		delete m_pIcon0;
		m_pIcon0 = NULL;
	}

	if(m_pIcon1)
	{
		delete m_pIcon1;
		m_pIcon1 = NULL;
	}	

	if(m_nBk[LBL_IMG_UP])
	{
		m_Bk0->Remove(0);
		m_Bk0->SetImageCount(0);
		m_Bk0->DeleteImageList();
		m_nBk[LBL_IMG_UP] = NULL;
	}
	if(m_nBk[LBL_IMG_DN])
	{
		m_Bk1->Remove(0);
		m_Bk1->SetImageCount(0);
		m_Bk1->DeleteImageList();
		m_nBk[LBL_IMG_DN] = NULL;
	}
	if(m_nImage[LBL_IMG_UP])
	{
		m_Image0->Remove(0);
		m_Image0->SetImageCount(0);
		m_Image0->DeleteImageList();
		m_nImage[LBL_IMG_UP] = NULL;
	}
	if(m_nImage[LBL_IMG_DN])
	{
		m_Image1->Remove(0);
		m_Image1->SetImageCount(0);
		m_Image1->DeleteImageList();
		m_nImage[LBL_IMG_DN] = NULL;
	}

	if(m_Bk0)
	{
		delete m_Bk0;
		m_Bk0 = NULL;
	}

	if(m_Bk1)
	{
		delete m_Bk1;
		m_Bk1 = NULL;
	}

	if(m_Image0)
	{
		delete m_Image0;
		m_Image0 = NULL;
	}

	if(m_Image1)
	{
		delete m_Image1;
		m_Image1 = NULL;
	}

}

BOOL CMyLabel::LoadBkImage(CString strImgPath, int nImg)
{
	CSize sz;
	CRect	rClient;

	GetClientRect(&rClient);
	rClient.DeflateRect(2,2,2,2);
	sz.cx = rClient.Width();
	sz.cy = rClient.Height();

	switch(nImg)
	{
	case LBL_IMG_UP:
		if(!m_pBitmapBk0)
			m_pBitmapBk0 = new CBitmap;
		if(!m_hBitmapBk0)
		{
			m_hBitmapBk0 = (HBITMAP)::LoadImage(NULL, strImgPath, IMAGE_BITMAP, sz.cx, sz.cy, LR_LOADFROMFILE);
// 			GetObject(m_hBitmapBk0, sizeof(BITMAP), (LPSTR)&iSrcBitmap);
// 			m_pBitmapBk0->CreateBitmap(iSrcBitmap.bmWidth, iSrcBitmap.bmHeight, iSrcBitmap.bmPlanes, iSrcBitmap.bmBitsPixel, NULL);
			m_pBitmapBk0->Attach(m_hBitmapBk0);
		}
		break;
	case LBL_IMG_DN:
		if(!m_pBitmapBk1)
			m_pBitmapBk1 = new CBitmap;
		if(!m_hBitmapBk1)
		{
			m_hBitmapBk1 = (HBITMAP)::LoadImage(NULL, strImgPath, IMAGE_BITMAP, sz.cx, sz.cy, LR_LOADFROMFILE);
// 			GetObject(m_hBitmapBk1, sizeof(BITMAP), (LPSTR)&iSrcBitmap);
// 			m_pBitmapBk1->CreateBitmap(iSrcBitmap.bmWidth, iSrcBitmap.bmHeight, iSrcBitmap.bmPlanes, iSrcBitmap.bmBitsPixel, NULL);
			m_pBitmapBk1->Attach(m_hBitmapBk1);
		}
		break;
	}

	return TRUE;
}

BOOL CMyLabel::LoadImage(CString strImgPath, int nImg, CSize sz, int nImgPos)
{
	if(strImgPath.IsEmpty())
		return FALSE;

	if(sz.cx == 0 || sz.cy == 0)
		sz = CSize(50,50);

	int nPos = strImgPath.ReverseFind('.');
	CString strExt = strImgPath.Right(strImgPath.GetLength()-nPos-1);

	switch(nImg)
	{
	case LBL_IMG_UP:
		if(strExt == _T("bmp") || strExt == _T("BMP"))
		{
			if(!m_pBitmap0)
				m_pBitmap0 = new CBitmap;
			if(!m_hBitmap0)
			{
				m_hBitmap0 = (HBITMAP)::LoadImage(NULL, strImgPath, IMAGE_BITMAP, sz.cx, sz.cy, LR_LOADFROMFILE);
// 				GetObject(m_hBitmap0, sizeof(BITMAP), (LPSTR)&iSrcBitmap);
// 				m_pBitmap0->CreateBitmap(iSrcBitmap.bmWidth, iSrcBitmap.bmHeight, iSrcBitmap.bmPlanes, iSrcBitmap.bmBitsPixel, NULL);
				m_pBitmap0->Attach(m_hBitmap0);
			}
		}
		else if(strExt == _T("ico") || strExt == _T("ICO"))
		{
			if(!m_pIcon0)
				m_pIcon0 = new CBitmap;
			if(!m_hIcon0)
			{
				m_hIcon0 = (HICON)::LoadImage(NULL, strImgPath, IMAGE_ICON, sz.cx, sz.cy, LR_LOADFROMFILE);
				m_pIcon0->Attach(m_hIcon0);
			}
		}
		else
			return FALSE;

		if(m_hBitmap0)
		{
			if(!m_Image0)
			{
				m_Image0 = new CImageList;
				if(m_Image0->Create(sz.cx,sz.cy,ILC_COLORDDB,0,1))//Create imagelist from bitmap
				{
					//Add bitmap to the image list. Use default button color as mask color
					if(m_hBitmapBk0)
 					{
						CBitmap bmpBk;
						bmpBk.Attach(m_hBitmapBk0);
						int r = m_Image0->Add(m_pBitmap0,m_pBitmapBk0);//mask color
 					}
 					else
 						int r = m_Image0->Add(m_pBitmap0,RGB_BLACK);//mask color
					m_nImage[nImg] = 1;
				}
			}
			else
			{
				;
			}
		}

		if(m_hIcon0)		
		{
			if(!m_Image0)
			{
				m_Image0 = new CImageList;
				if(m_Image0->Create(sz.cx,sz.cy,ILC_COLORDDB|ILC_MASK,0,1))//Create imagelist from bitmap
				{
					//Add bitmap to the image list. Use default button color as mask color
					int r = m_Image0->Add(m_hIcon0);//mask color
					int n = m_Image0->GetImageCount();
					m_nImage[nImg] = 1;
				}
			}
			else
			{
				;
			}
		}
		break;

	case LBL_IMG_DN:
		if(strExt == _T("bmp") || strExt == _T("BMP"))
		{
			if(!m_pBitmap1)
				m_pBitmap1 = new CBitmap;
			if(!m_hBitmap1)
			{
				m_hBitmap1 = (HBITMAP)::LoadImage(NULL, strImgPath, IMAGE_BITMAP, sz.cx, sz.cy, LR_LOADFROMFILE);
// 				GetObject(m_hBitmap1, sizeof(BITMAP), (LPSTR)&iSrcBitmap);
// 				m_pBitmap1->CreateBitmap(iSrcBitmap.bmWidth, iSrcBitmap.bmHeight, iSrcBitmap.bmPlanes, iSrcBitmap.bmBitsPixel, NULL);
				m_pBitmap1->Attach(m_hBitmap1);
			}
		}
		else if(strExt == _T("ico") || strExt == _T("ICO"))
		{
			if(!m_pIcon1)
				m_pIcon1 = new CBitmap;
			if(!m_hIcon1)
			{
				m_hIcon1 = (HICON)::LoadImage(NULL, strImgPath, IMAGE_ICON, sz.cx, sz.cy, LR_LOADFROMFILE);
				m_pIcon1->Attach(m_hBitmap1);
			}
		}
		else
			return FALSE;

		if(m_hBitmap1)
		{
			if(!m_Image1)
			{
				m_Image1 = new CImageList;
				if(m_Image1->Create(sz.cx,sz.cy,ILC_COLORDDB,0,1))//Create imagelist from bitmap
				{
					//Add bitmap to the image list. Use default button color as mask color
					if(m_hBitmapBk1)
 					{
						int r = m_Image1->Add(m_pBitmap1,m_pBitmapBk1);//mask color
 					}
 					else
 						int r = m_Image1->Add(m_pBitmap1,RGB_BLACK);//mask color
					m_nImage[nImg] = 1;
				}
			}
			else
			{
				;
			}
		}

		if(m_hIcon1)		
		{
			if(!m_Image1)
			{
				m_Image1 = new CImageList;
				if(m_Image1->Create(sz.cx,sz.cy,ILC_COLORDDB|ILC_MASK,0,1))//Create imagelist from bitmap
				{
					//Add bitmap to the image list. Use default button color as mask color
					int r = m_Image1->Add(m_hIcon1);//mask color
					int n = m_Image1->GetImageCount();
					m_nImage[nImg] = 1;
				}
			}
			else
			{
				;
			}
		}
		break;
	}

	m_Size = sz;
	m_nImgPos = nImgPos;

	return TRUE;
}

// CMyLabel& CMyLabel::SetText(const CString& strText)
// {
// 	m_strText = strText;
// 	SetWindowText(strText);
// 	RedrawWindow();
// 	return *this;
// }

// void CMyLabel::SetText(CString sText)
// {
// 	m_sCaption = sText;
// 	RedrawWindow();
// }

CMyLabel& CMyLabel::SetTextColor(COLORREF crText)
{
	m_crText = crText;
	RedrawWindow();
	return *this;
}

CMyLabel& CMyLabel::SetFontBold(BOOL bBold)
{	
	m_bBold = bBold;
	m_lf.lfWeight = bBold ? FW_BOLD : FW_NORMAL;
	ReconstructFont();
	RedrawWindow();
	return *this;
}

CMyLabel& CMyLabel::SetFontUnderline(BOOL bSet)
{	
	m_lf.lfUnderline = bSet;
	ReconstructFont();
	RedrawWindow();
	return *this;
}

CMyLabel& CMyLabel::SetFontItalic(BOOL bSet)
{
	m_lf.lfItalic = bSet;
	ReconstructFont();
	RedrawWindow();
	return *this;	
}

CMyLabel& CMyLabel::SetSunken(BOOL bSet)
{
	if (!bSet)
		ModifyStyleEx(WS_EX_STATICEDGE,0,SWP_DRAWFRAME);
	else
		ModifyStyleEx(0,WS_EX_STATICEDGE,SWP_DRAWFRAME);
		
	return *this;	
}

CMyLabel& CMyLabel::SetBorder(BOOL bSet)
{
	if (!bSet)
		ModifyStyle(WS_BORDER,0,SWP_DRAWFRAME);
	else
		ModifyStyle(0,WS_BORDER,SWP_DRAWFRAME);
		
	return *this;	
}

CMyLabel& CMyLabel::SetFontSize(int nSize)
{
	m_fontSize = nSize;
	nSize*=-1;
	m_lf.lfHeight = nSize;
	ReconstructFont();
	RedrawWindow();
	return *this;
}

CMyLabel& CMyLabel::SetBkColor(COLORREF crBkgnd)
{
	m_crBkgnd = crBkgnd;
	
	if (m_hBrush)
	{
		if(::DeleteObject(m_hBrush))
			m_hBrush = NULL;
	}

	m_hBrush = ::CreateSolidBrush(crBkgnd);
	return *this;
}

CMyLabel& CMyLabel::SetFontName(const CString& strFont)
{	
	m_fontName = strFont;
	//strcpy(m_lf.lfFaceName,strFont);
	_stprintf(m_lf.lfFaceName, _T("%s"), strFont);
	ReconstructFont();
	RedrawWindow();
	return *this;
}


BEGIN_MESSAGE_MAP(CMyLabel, CStatic)
	//{{AFX_MSG_MAP(CMyLabel)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_TIMER()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyLabel message handlers

HBRUSH CMyLabel::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a non-NULL brush if the parent's handler should not be called

// 	if (CTLCOLOR_STATIC == nCtlColor)
// 	{
// 		DrawBkImg();
// 
// 		pDC->SelectObject(&m_font);
// 		pDC->SetTextColor(m_crText);
// 		pDC->SetBkMode(TRANSPARENT);
// 
// 
// 	}
// 
// 
// 	if (m_Type == Background)
// 	{
// 		if (!m_bState)
// 			return m_hwndBrush;
// 	}

	return m_hBrush;
}

void CMyLabel::ReconstructFont()
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


CMyLabel& CMyLabel::FlashText(BOOL bActivate)
{
	if (m_bTimer)
	{
		SetWindowText(m_strText);
		KillTimer(1);
	}

	if (bActivate)
	{
		GetWindowText(m_strText);
		m_bState = FALSE;
		
		m_bTimer = TRUE;
		SetTimer(1,500,NULL);
		m_Type = _Text;
	}

	return *this;
}

CMyLabel& CMyLabel::FlashBackground(BOOL bActivate)
{

	if (m_bTimer)
		KillTimer(1);

	if (bActivate)
	{
		m_bState = FALSE;

		m_bTimer = TRUE;
		SetTimer(1,500,NULL);

		m_Type = _Background;
	}

	return *this;
}


void CMyLabel::OnTimer(UINT_PTR nIDEvent)//(UINT nIDEvent)
{
	if(nIDEvent == 1)
	{
		KillTimer(1);
		m_bState = !m_bState;

		switch (m_Type)
		{
			case _Text:
				if (m_bState)
					SetWindowText(_T(""));
				else
					SetWindowText(m_strText);
			break;

			case _Background:
				InvalidateRect(NULL,FALSE);
				UpdateWindow();
			break;
		}
	}

	if(nIDEvent == TIM_LBL_REFRESH)
	{
		KillTimer(TIM_LBL_REFRESH);
		Invalidate();
	}
	
	CStatic::OnTimer(nIDEvent);
}

CMyLabel& CMyLabel::SetLink(BOOL bLink)
{
	m_bLink = bLink;

	if (bLink)
		ModifyStyle(0,SS_NOTIFY);
	else
		ModifyStyle(SS_NOTIFY,0);

	return *this;
}

/*
void CMyLabel::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CString strLink;

	GetWindowText(strLink);
	ShellExecute(NULL,"open",strLink,NULL,NULL,SW_SHOWNORMAL);
		
	CStatic::OnLButtonDown(nFlags, point);
}
*/
BOOL CMyLabel::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (m_hCursor)
	{
		::SetCursor(m_hCursor);
		return TRUE;
	}

	return CStatic::OnSetCursor(pWnd, nHitTest, message);
}

CMyLabel& CMyLabel::SetLinkCursor(HCURSOR hCursor)
{
	m_hCursor = hCursor;
	return *this;
}


void CMyLabel::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
// 	DWORD ctlStyle = ((CStatic*)this)->GetStyle();
// 	if(ctlStyle & WS_BORDER)
// 		m_bBoarder = TRUE;
// 	else
// 		m_bBoarder = FALSE;

	//ModifyStyle(0, BS_OWNERDRAW);	// make the button owner drawn
	m_style = GetWindowLong(this->GetSafeHwnd(), GWL_STYLE); // 스타일을 꺼내달라
	
	CStatic::PreSubclassWindow();
}


CRect CMyLabel::GetImgRect(CRect rClient)
{
	CRect r=rClient;

	switch(m_nImg)
	{
	case LBL_IMG_UP:
	case LBL_IMG_DN:
		if(m_nImage[m_nImg])
		{
			if(m_Image0->GetImageCount()==1)
			{
				CPoint center=r.CenterPoint();

				switch(m_nImgPos)
				{
				case LBL_POS_LEFT:
					r.left	= r.left+3;
					r.right	= r.left+m_Size.cx;
					r.top	= center.y-m_Size.cy/2;
					r.bottom= center.y+m_Size.cy/2;
					break;
				case LBL_POS_TOP:
					r.left	= center.x-m_Size.cx/2;
					r.right	= center.x+m_Size.cx/2;
					r.top	= r.top+7;
					r.bottom= r.top+m_Size.cy;
					break;
				case LBL_POS_RIGHT:
					r.right	= r.right-3;
					r.left	= r.right-m_Size.cx;
					r.top	= center.y-m_Size.cy/2;
					r.bottom= center.y+m_Size.cy/2;
					break;
				case LBL_POS_BOTTOM:
					r.left	= center.x-m_Size.cx/2;
					r.right	= center.x+m_Size.cx/2;
					r.bottom= r.bottom-7;
					r.top	= r.bottom-m_Size.cy;
					break;
				case LBL_POS_CENTER:
					r.left	= center.x-m_Size.cx/2;
					r.right	= center.x+m_Size.cx/2;
					r.top = center.y-m_Size.cy/2;
					r.bottom = center.y+m_Size.cy/2;
					break;
				}
			}
		}
		break;

// 	case 1:
// 		if(m_nImage[m_nImg])
// 		{
// 			if(m_Image1->GetImageCount()==1)
// 			{
// 				CPoint center=r.CenterPoint();
// 
// 				switch(m_nImgPos)
// 				{
// 				case LBL_POS_LEFT:
// 					r.left	= r.left+3;
// 					r.right	= r.left+m_Size.cx;
// 					r.top	= center.y-m_Size.cy/2;
// 					r.bottom= center.y+m_Size.cy/2;
// 					break;
// 				case LBL_POS_TOP:
// 					r.left	= center.x-m_Size.cx/2;
// 					r.right	= center.x+m_Size.cx/2;
// 					r.top	= r.top+7;
// 					r.bottom= r.top+m_Size.cy;
// 					break;
// 				case LBL_POS_RIGHT:
// 					r.right	= r.right-3;
// 					r.left	= r.right-m_Size.cx;
// 					r.top	= center.y-m_Size.cy/2;
// 					r.bottom= center.y+m_Size.cy/2;
// 					break;
// 				case LBL_POS_BOTTOM:
// 					r.left	= center.x-m_Size.cx/2;
// 					r.right	= center.x+m_Size.cx/2;
// 					r.bottom= r.bottom-7;
// 					r.top	= r.bottom-m_Size.cy;
// 					break;
// 				}
// 			}
// 		}
// 		break;
	}

	if(r.left<0)
		r.left = rClient.left;
	if(r.right>rClient.right)
		r.right = rClient.right;
	if(r.top<0)
		r.top = rClient.top;
	if(r.bottom>rClient.bottom)
		r.bottom = rClient.bottom;

	return r;
}

// CRect CMyLabel::GetImgRect(CRect rClient)
// {
// 	CRect r=rClient;
// 
// 	if(m_nImage[m_nImg])
// 	{
// 		switch(m_nImg)
// 		{
// 		case 0:
// 			if(m_Image0->GetImageCount()==1)
// 			{
// 				CPoint centre=r.CenterPoint();
// 
// 				//Center the icon 
// 				r.left	= r.left+3;
// 				r.right	= r.right+3;
// 				r.top	= centre.y-m_Size.cy/2;
// 				r.bottom = centre.y+m_Size.cy/2;
// 			}
// 			break;
// 
// 		case 1:
// 			if(m_Image1->GetImageCount()==1)
// 			{
// 				CPoint centre=r.CenterPoint();
// 
// 				//Center the icon 
// 				r.left	= r.left+3;
// 				r.right	= r.right+3;
// 				r.top	= centre.y-m_Size.cy/2;
// 				r.bottom = centre.y+m_Size.cy/2;
// 			}
// 			break;
// 		}
// 	}
// /*
// //	if(m_pBitmapBk)
// 	if(m_pBitmapBk0 || m_pBitmapBk1)
// 	{
// 		CPoint centre=r.CenterPoint();
// 
// 		//Center the icon 
// 		r.left	= r.left+2;
// 		r.right	= r.right-2;
// 		r.top	= r.top+2;
// 		r.bottom= r.bottom-2;
// 	}*/
// 	return r;
// }

void CMyLabel::Refresh()
{
	RedrawWindow();
}

void CMyLabel::RefreshText()
{
	CDC* dc = this->GetDC();
//	CClientDC dc(this);
	CRect rect(0,0,0,0), TxtRect(0,0,0,0);
	GetClientRect(&rect);

	HFONT hOldFont=NULL;
	//Sets the font
	if(m_font.GetSafeHandle() != NULL)
		hOldFont =(HFONT) dc->SelectObject(m_font.GetSafeHandle());

	CString strText;
 	GetWindowText(strText);
	CSize Extent = dc->GetTextExtent(strText);
	CPoint pt0( rect.CenterPoint().x - Extent.cx/2, rect.CenterPoint().y - Extent.cy/2 );
	CPoint pt1( rect.CenterPoint().x + Extent.cx/2, rect.CenterPoint().y + Extent.cy/2 );

	//Reset the old font
	if(hOldFont != NULL)
		dc->SelectObject(hOldFont);


	TxtRect.top = pt0.y;	TxtRect.left = pt0.x;
	TxtRect.bottom = pt1.y;	TxtRect.right = pt1.x;
	CRect ImgRect = TxtRect;

	// create memory device context
	CDC* memDC = new CDC; memDC->CreateCompatibleDC(dc);
	CBitmap* old;

	switch(m_nImg)
	{
	case LBL_IMG_UP:
		if(m_hBitmapBk0)
		{
			// buffer bitmap
			old = memDC->SelectObject(m_pBitmapBk0);
			
			dc->BitBlt
			(
				pt0.x,					// specifies the x-coordinate (in logical units) of the upper-left corner of the destination rectangle
				pt0.y,					// specifies the y-coordinate (in logical units) of the upper-left corner of the destination rectangle
				ImgRect.Width(),		// specifies the width (in logical units) of the destination rectangle and source bitmap
				ImgRect.Height(),		// specifies the height (in logical units) of the destination rectangle and source bitmap
				memDC,				// specifies the source device context
				0,					// specifies the x-coordinate (in logical units) of the upper-left corner of the source rectangle
				0,					// specifies the x-coordinate (in logical units) of the upper-left corner of the source rectangle
				SRCCOPY				// specifies the raster operation to be performed
			);//SRCCOPY

			// reselect first bitmap
			memDC->SelectObject(old);

// 			// delete device context and reset pointer
// 			delete memDC; memDC = 0;
		}
		break;

	case LBL_IMG_DN:
		if(m_hBitmapBk1)
		{
 			old = memDC->SelectObject(m_pBitmapBk1);
			
			dc->BitBlt
			(
				pt0.x,					// specifies the x-coordinate (in logical units) of the upper-left corner of the destination rectangle
				pt0.y,					// specifies the y-coordinate (in logical units) of the upper-left corner of the destination rectangle
				ImgRect.Width(),		// specifies the width (in logical units) of the destination rectangle and source bitmap
				ImgRect.Height(),		// specifies the height (in logical units) of the destination rectangle and source bitmap
				memDC,				// specifies the source device context
				0,					// specifies the x-coordinate (in logical units) of the upper-left corner of the source rectangle
				0,					// specifies the x-coordinate (in logical units) of the upper-left corner of the source rectangle
				SRCCOPY				// specifies the raster operation to be performed
			);//SRCCOPY

			// reselect first bitmap
			memDC->SelectObject(old);

// 			// delete device context and reset pointer
// 			delete memDC; memDC = 0;
		}
		break;
	}

	// delete device context and reset pointer
	delete memDC; memDC = 0;

	DrawText();
}


void CMyLabel::DrawBkImg(CPaintDC* dc) 
{
	CRect rect(0,0,0,0);
	GetClientRect(&rect);

	CRect ImgRect = GetImgRect(rect);

	// create memory device context
	CDC* memDC = new CDC; memDC->CreateCompatibleDC(dc);
	CBitmap* old;

	switch(m_nImg)
	{
	case LBL_IMG_UP:	// Up
		if(m_hBitmapBk0)
		{
			// buffer bitmap
			old = memDC->SelectObject(m_pBitmapBk0);
			
			dc->BitBlt
			(
				2,					// specifies the x-coordinate (in logical units) of the upper-left corner of the destination rectangle
				2,					// specifies the y-coordinate (in logical units) of the upper-left corner of the destination rectangle
				rect.Width(),		// specifies the width (in logical units) of the destination rectangle and source bitmap
				rect.Height(),		// specifies the height (in logical units) of the destination rectangle and source bitmap
				memDC,				// specifies the source device context
				0,					// specifies the x-coordinate (in logical units) of the upper-left corner of the source rectangle
				0,					// specifies the x-coordinate (in logical units) of the upper-left corner of the source rectangle
				SRCCOPY				// specifies the raster operation to be performed
			);//SRCCOPY

			// reselect first bitmap
			memDC->SelectObject(old);

// 			// delete device context and reset pointer
// 			delete memDC; memDC = 0;
		}

		if(m_nImage[m_nImg])
		{
//				if(m_nImg==IMG_UP)
				m_Image0->Draw(dc,0,ImgRect.TopLeft(),ILD_TRANSPARENT);//Use ImageList draw routine
// 				if(m_nImg==IMG_DN)
// 					m_Image0->Draw(pDC,0,ImgRect.TopLeft()+CPoint(1,1),ILD_TRANSPARENT);//Use ImageList draw routine
		}

		break;

	case LBL_IMG_DN:	// Dn
		if(m_hBitmapBk1)
		{
 			old = memDC->SelectObject(m_pBitmapBk1);
			
			dc->BitBlt
			(
				2,					// specifies the x-coordinate (in logical units) of the upper-left corner of the destination rectangle
				2,					// specifies the y-coordinate (in logical units) of the upper-left corner of the destination rectangle
				rect.Width(),		// specifies the width (in logical units) of the destination rectangle and source bitmap
				rect.Height(),		// specifies the height (in logical units) of the destination rectangle and source bitmap
				memDC,				// specifies the source device context
				0,					// specifies the x-coordinate (in logical units) of the upper-left corner of the source rectangle
				0,					// specifies the x-coordinate (in logical units) of the upper-left corner of the source rectangle
				SRCCOPY				// specifies the raster operation to be performed
			);//SRCCOPY

			// reselect first bitmap
			memDC->SelectObject(old);

// 			// delete device context and reset pointer
// 			delete memDC; memDC = 0;
		}
		if(m_nImage[m_nImg])
		{
//				if(m_nImg==IMG_UP)
				m_Image1->Draw(dc,0,ImgRect.TopLeft(),ILD_TRANSPARENT);//Use ImageList draw routine
// 				if(m_nImg==IMG_DN)
// 					m_Image0->Draw(pDC,0,ImgRect.TopLeft()+CPoint(1,1),ILD_TRANSPARENT);//Use ImageList draw routine
		}
		break;
	}

	// delete device context and reset pointer
	delete memDC; memDC = 0;
}

void CMyLabel::DrawBoarder(int RndX, int RndY, BOOL bDraw)
{
	m_bBoarder = bDraw;
	m_ptRound.x = RndX;
	m_ptRound.y = RndY;
}

void CMyLabel::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	if(m_bBoarder)
	{
		CRect rect(0,0,0,0);
		GetClientRect(&rect);
		dc.RoundRect(rect,m_ptRound);
	}
	
	CString str;
	GetWindowText(str);
	DrawBkImg(&dc);
	DrawText();

	
	// Do not call CStatic::OnPaint() for painting messages
}

void CMyLabel::SetText(CString sText)
{
// 	m_sCaption = _T(" ";
// 	RedrawWindow();
	m_sCaption = sText;
	RedrawWindow();
}

CString CMyLabel::GetText()
{
	return m_sCaption;
}

void CMyLabel::DrawText()
{
	CString strText, strPars, strRem;

	BOOL bRedraw=FALSE;
	//GetWindowText(strText);
	if(m_sCaption.IsEmpty())
		GetWindowText(strText);
	else
	{
		GetWindowText(strText);
		if(strText != m_sCaption)
			bRedraw = TRUE;

		strText = m_sCaption;
	}


    // Draw the text
    if (!strText.IsEmpty())
    {
		CClientDC dc(this);
		
		CRect rect(0,0,0,0);
		GetClientRect(&rect);

		HFONT hOldFont=NULL;
		//Sets the font
		if(m_font.GetSafeHandle() != NULL)
		{
			hOldFont =(HFONT) dc.SelectObject(m_font.GetSafeHandle());
		}

        CSize Extent = dc.GetTextExtent(strText);
		CPoint pt( rect.CenterPoint().x - Extent.cx/2, rect.CenterPoint().y - Extent.cy/2 );

		int nMode = dc.SetBkMode(TRANSPARENT);
		COLORREF crTextOld = dc.SetTextColor(m_crText);


		int nPos, nLine=0;
		CString strLineText[MAX_LINE_LABEL];
		CPoint LinePos[MAX_LINE_LABEL];

		nLine++;

		do
		{
			nPos = strText.Find(_T("_"), 0);
			if(nPos == 0)
			{
				strText.SetAt(nPos, _T(' '));
			}
				
			nPos = strText.Find(_T("&&"), 0);
			if(nPos >= 0)
				strText.Delete(nPos);

			nPos = strText.Find(_T("\\r"), 0);
			if(nPos > 0)
			{
				strPars = strText.Left(nPos);
				strRem = strText.Right(strText.GetLength()-nPos-2);
				strLineText[nLine-1] = strPars;
				strText = strRem;
				nLine++;
			}
			else
			{
				nPos = strText.Find(_T("\r"), 0);
				if(nPos > 0)
				{
					strPars = strText.Left(nPos);
					strRem = strText.Right(strText.GetLength()-nPos-1);
					strLineText[nLine-1] = strPars;
					strText = strRem;
					nLine++;
				}
			}
			
		}while(nPos > 0);

		strLineText[nLine-1] = strText;
		LinePos[0].y = pt.y - (Extent.cy/2 * (nLine-1)) - (LINE_SPACE_LABEL * (nLine-1) / 2);

		for(int nL=0; nL<nLine; nL++)
		{
			Extent = dc.GetTextExtent(strLineText[nL]);
// 			LinePos[nL].x = rect.CenterPoint().x - Extent.cx/2;
			if((m_style & SS_CENTER) == SS_CENTER)
			{
				LinePos[nL].x = rect.CenterPoint().x - Extent.cx/2; 
			}
			else if((m_style & SS_RIGHT) == SS_RIGHT)
			{
				LinePos[nL].x = rect.right - Extent.cx; 
			}
			else if((m_style & SS_LEFT) == SS_LEFT)
			{
				LinePos[nL].x = 0; 
			}
			else
			{
				LinePos[nL].x = rect.CenterPoint().x - Extent.cx/2; 
			}
			if(nL>0)
				LinePos[nL].y = LinePos[nL-1].y + Extent.cy + LINE_SPACE_LABEL;

// 			if(bRedraw)
// 			{
// 				CString sSpace="";
// 				for(int nC=0; nC<strLineText[nL].GetLength(); nC++)
// 					sSpace += _T(" ";			
// 				dc.TextOut(LinePos[nL].x, LinePos[nL].y, sSpace);
// 			}
// 			else
				dc.TextOut(LinePos[nL].x, LinePos[nL].y, strLineText[nL]);
		}


		//dc.TextOut(pt.x, pt.y, strText);

        dc.SetBkMode(nMode);

		//Reset the old font
		if(hOldFont != NULL)
		{
			dc.SelectObject(hOldFont);
		}
    }
}

int CMyLabel::GetImageBk()
{
	return(m_nImg);
}

void CMyLabel::SetImageBk(int nImg)
{
	m_nImg = nImg;
	RedrawWindow();
}

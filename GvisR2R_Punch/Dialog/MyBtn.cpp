// MyBtn.cpp : implementation file
//

#include "stdafx.h"
#include "../GvisR2R_Punch.h"
#include "MyBtn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "../GvisR2R_PunchView.h"
extern CGvisR2R_PunchView* pView;

const CSize cButtonMargin(50,50);		// number of pixels wider button is than icon

///////////////////////////////////////////////////////////////////////
//	Class CHitRgn
//
//  Encapsulates a button hit region
//	You can either just use a rect region inwhich case the overhead of
//  a CRgn is unnecessary, or can set up a more complex region.
///////////////////////////////////////////////////////////////////////

//
// Set to a simple rect region in client coords
//
CHitRgn& CHitRgn::operator=(const CRect& r)	
{
	InitData();					//Remove current data
	m_rBox=r;						//Simple rect region
	m_rBox.NormalizeRect();//Just in case

	return *this;
}

/////////////////////////////////////////////////////////////////////////////
// Is the pt (client coords) inside the region
//
int CHitRgn::GetRgnBox(LPRECT lpRect) const
{
	if(HaveCRgn())
		{
		return CRgn::GetRgnBox(lpRect);
		}
	else //Just get the stored rect
		{
		*lpRect=GetCachedBox();
		return SIMPLEREGION;	//Mark as valid
		}
}

/////////////////////////////////////////////////////////////////////////////
// Is the pt (client coords) inside the region
//
bool	CHitRgn::Inside(CPoint pt)const
{
	if(GetCachedBox().PtInRect(pt))//Must be in bounding box
		{
		if(HaveCRgn())	//If have a real region check inside it
			return PtInRegion(pt)!=FALSE;
		return true;
		}
	return false;
}

///////////////////////////////////////////////////////////////////////
//	Class CMyBtn
///////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CMyBtn

CMyBtn::CMyBtn()
{
	m_hParentWnd = NULL;
	m_nCtrlID = NULL; 
	m_hBrush = NULL;
	m_nType = BTN_TYPE_DEFAULT;

	m_nImgPos = BTN_POS_LEFT;
	
	m_Size = CSize(0,0);
	m_chkState = DFCS_BUTTONPUSH;
	m_nImg = BTN_IMG_UP;
	m_nImgBk = BTN_IMG_UP;

	m_crText = RGB_BLACK;
	m_bFont = FALSE;

	m_Image0 = NULL;
	m_Image1 = NULL;
	m_Bk0 = NULL;
	m_Bk1 = NULL;

	m_nBk[BTN_IMG_UP] = NULL;
	m_nBk[BTN_IMG_DN] = NULL;
	m_nImage[BTN_IMG_UP] = NULL;
	m_nImage[BTN_IMG_DN] = NULL;

	m_strUp = _T("");
	m_strDn = _T("");
	m_nTxtDispSt = -1;

	for(int i=0; i<2; i++)
	{
		m_nImage[i] = NULL;
		m_nBk[i] = NULL;
	}

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

	m_bFont = FALSE;

	m_bFrame = FALSE;
	m_bBoarder = TRUE;

	m_pBoundryPen = new CPen;
	m_pBoundryPen->CreatePen(PS_INSIDEFRAME|PS_SOLID,1,RGB(0,0,0));
}

CMyBtn::~CMyBtn()
{
	DelImgList();

    if(m_pBoundryPen)
	{
		if(m_pBoundryPen->DeleteObject())
		{
			delete m_pBoundryPen;
			m_pBoundryPen = NULL;
		}
	}

    if(m_bFont)
	{
		if(m_Font.DeleteObject())
			m_bFont = FALSE;
	}

	if (m_hBrush)
	{
		if(::DeleteObject(m_hBrush))
			m_hBrush = NULL;
	}
}

void CMyBtn::DelImgList()
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

	if(m_nBk[BTN_IMG_UP])
	{
		m_Bk0->Remove(0);
		m_Bk0->SetImageCount(0);
		m_Bk0->DeleteImageList();
		m_nBk[BTN_IMG_UP] = NULL;
	}
	if(m_nBk[BTN_IMG_DN])
	{
		m_Bk1->Remove(0);
		m_Bk1->SetImageCount(0);
		m_Bk1->DeleteImageList();
		m_nBk[BTN_IMG_DN] = NULL;
	}
	if(m_nImage[BTN_IMG_UP])
	{
		m_Image0->Remove(0);
		m_Image0->SetImageCount(0);
		m_Image0->DeleteImageList();
		m_nImage[BTN_IMG_UP] = NULL;
	}
	if(m_nImage[BTN_IMG_DN])
	{
		m_Image1->Remove(0);
		m_Image1->SetImageCount(0);
		m_Image1->DeleteImageList();
		m_nImage[BTN_IMG_DN] = NULL;
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

BEGIN_MESSAGE_MAP(CMyBtn, CButton)
	//{{AFX_MSG_MAP(CMyBtn)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_DRAWITEM()
	//}}AFX_MSG_MAP
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyBtn message handlers

void CMyBtn::SetHwnd(HWND hParentWnd, int nCtrlID)
{
	m_hParentWnd = hParentWnd;
	m_nCtrlID = nCtrlID;
}

BOOL CMyBtn::LoadBkImage(CString strImgPath, int nImg)
{
	CSize sz;
	CRect	rClient;

	GetClientRect(&rClient);
//	rClient.DeflateRect(3,3,10,10);
	sz.cx = rClient.Width()-6;
	sz.cy = rClient.Height()-6;

	switch(nImg)
	{
	case BTN_IMG_UP:
		if(!m_pBitmapBk0)
			m_pBitmapBk0 = new CBitmap;
		if(!m_hBitmapBk0)
		{
			m_hBitmapBk0 = (HBITMAP)::LoadImage(NULL, strImgPath, IMAGE_BITMAP, sz.cx, sz.cy, LR_LOADFROMFILE);
// 			GetObject(m_hBitmapBk0, sizeof(BITMAP), (LPSTR)&iSrcBitmap);
// 			m_pBitmapBk0->CreateBitmap(iSrcBitmap.bmWidth, iSrcBitmap.bmHeight, iSrcBitmap.bmPlanes, iSrcBitmap.bmBitsPixel, NULL);
			m_pBitmapBk0->Attach(m_hBitmapBk0);
		}

		if(!m_Bk0)
		{
			m_Bk0 = new CImageList;
			if(m_Bk0->Create(sz.cx,sz.cy,ILC_COLORDDB,0,1))//Create imagelist from bitmap
			{
				//Add bitmap to the image list. Use default button color as mask color
				int r = m_Bk0->Add(m_pBitmapBk0,m_pBitmapBk0);//mask color
				m_nBk[nImg] = 1;
			}
		}
		else
		{
			;
		}
		break;

	case BTN_IMG_DN:
		if(!m_pBitmapBk1)
			m_pBitmapBk1 = new CBitmap;
		if(!m_hBitmapBk1)
		{
			m_hBitmapBk1 = (HBITMAP)::LoadImage(NULL, strImgPath, IMAGE_BITMAP, sz.cx, sz.cy, LR_LOADFROMFILE);
// 			GetObject(m_hBitmapBk1, sizeof(BITMAP), (LPSTR)&iSrcBitmap);
// 			m_pBitmapBk1->CreateBitmap(iSrcBitmap.bmWidth, iSrcBitmap.bmHeight, iSrcBitmap.bmPlanes, iSrcBitmap.bmBitsPixel, NULL);
			m_pBitmapBk1->Attach(m_hBitmapBk1);
		}

		if(!m_Bk1)
		{
			m_Bk1 = new CImageList;
			if(m_Bk1->Create(sz.cx,sz.cy,ILC_COLORDDB,0,1))//Create imagelist from bitmap
			{
				//Add bitmap to the image list. Use default button color as mask color
				int r = m_Bk1->Add(m_pBitmapBk1,m_pBitmapBk1);//mask color
				m_nBk[nImg] = 1;
			}
		}
		else
		{
			;
		}
		break;
	}

	return TRUE;
}

BOOL CMyBtn::LoadImage(CString strImgPath, int nImg, CSize sz, int nImgPos)
{
	if(strImgPath.IsEmpty())
		return FALSE;

	if(sz.cx == 0 || sz.cy == 0)
		sz = CSize(50,50);


	int nPos = strImgPath.ReverseFind('.');
	CString strExt = strImgPath.Right(strImgPath.GetLength()-nPos-1);
	switch(nImg)
	{
	case BTN_IMG_UP:
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
				if(m_Image0->Create(sz.cx,sz.cy,ILC_COLORDDB|ILC_MASK,0,1))//Create imagelist from bitmap
				{
					//Add bitmap to the image list. Use default button color as mask color
					if(m_hBitmapBk0)
 					{
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

	case BTN_IMG_DN:
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
				m_pIcon1->Attach(m_hIcon1);
			}
		}
		else
			return FALSE;

		if(m_hBitmap1)
		{
			if(!m_Image1)
			{
				m_Image1 = new CImageList;
				if(m_Image1->Create(sz.cx,sz.cy,ILC_COLORDDB|ILC_MASK,0,1))//Create imagelist from bitmap
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

CRect CMyBtn::GetImgRect(CRect rClient)
{
	CRect r=rClient;

	switch(m_nImgBk)
	{
	case BTN_IMG_UP:
		if(m_nImage[m_nImgBk])
		{
			if(m_Image0->GetImageCount()==1)
			{
				CPoint center=r.CenterPoint();

				switch(m_nImgPos)
				{
				case BTN_POS_LEFT:
					r.left	= r.left+3;
					r.right	= r.left+m_Size.cx;
					r.top	= center.y-m_Size.cy/2;
					r.bottom= center.y+m_Size.cy/2;
					break;
				case BTN_POS_TOP:
					r.left	= center.x-m_Size.cx/2;
					r.right	= center.x+m_Size.cx/2;
					r.top	= r.top+7;
					r.bottom= r.top+m_Size.cy;
					break;
				case BTN_POS_RIGHT:
					r.right	= r.right-3;
					r.left	= r.right-m_Size.cx;
					r.top	= center.y-m_Size.cy/2;
					r.bottom= center.y+m_Size.cy/2;
					break;
				case BTN_POS_BOTTOM:
					r.left	= center.x-m_Size.cx/2;
					r.right	= center.x+m_Size.cx/2;
					r.bottom= r.bottom-7;
					r.top	= r.bottom-m_Size.cy;
					break;
				}
			}
		}
		break;

	case BTN_IMG_DN:
		if(m_nImage[m_nImgBk])
		{
			if(m_Image1->GetImageCount()==1)
			{
				CPoint center=r.CenterPoint();

				switch(m_nImgPos)
				{
				case BTN_POS_LEFT:
					r.left	= r.left+3;
					r.right	= r.left+m_Size.cx;
					r.top	= center.y-m_Size.cy/2;
					r.bottom= center.y+m_Size.cy/2;
					break;
				case BTN_POS_TOP:
					r.left	= center.x-m_Size.cx/2;
					r.right	= center.x+m_Size.cx/2;
					r.top	= r.top+7;
					r.bottom= r.top+m_Size.cy;
					break;
				case BTN_POS_RIGHT:
					r.right	= r.right-3;
					r.left	= r.right-m_Size.cx;
					r.top	= center.y-m_Size.cy/2;
					r.bottom= center.y+m_Size.cy/2;
					break;
				case BTN_POS_BOTTOM:
					r.left	= center.x-m_Size.cx/2;
					r.right	= center.x+m_Size.cx/2;
					r.bottom= r.bottom-7;
					r.top	= r.bottom-m_Size.cy;
					break;
				}
			}
		}
		break;
	}
	return r;
}

CRect CMyBtn::GetBorderRect(const CRect& rButton,CSize sInflate)
{	
	CRect r=rButton;
	r.InflateRect(sInflate);
	return r;
}

void CMyBtn::DrawImg(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	const CSize cSelectionMargin(1,1);// number of pixels wider selection is than icon
	const CSize cFocusMargin(2,2);		// number of pixels wider focus is than icon
	const CSize cEdgeMargin(3,3);		// number of pixels wider focus is than icon

	CDC* pDC   = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rect = lpDrawItemStruct->rcItem;
	UINT state = lpDrawItemStruct->itemState;

	CRect ImgRect = GetImgRect(rect);

	switch(m_nImgBk)
	{
	case BTN_IMG_UP:
		if (state & ODS_DISABLED)
		{
// 			if(m_hIcon0)
// 			{
// 				HICON hIcon=m_Image0->ExtractIcon(0);
// 				pDC->DrawState(ImgRect.TopLeft(),m_Size,hIcon,DSS_DISABLED,(HBRUSH)NULL);
// 				DestroyIcon(hIcon);
// 			}
			if(m_nBk[m_nImgBk])
			{
				rect.DeflateRect(3,3,0,0);
				m_Bk0->Draw(pDC,0,rect.TopLeft(),ILD_TRANSPARENT);//Use ImageList draw routine
			}
		}
		else
		{
			if(m_nBk[m_nImgBk])
			{
				rect.DeflateRect(3,3,0,0);
				m_Bk0->Draw(pDC,0,rect.TopLeft(),ILD_TRANSPARENT);//Use ImageList draw routine
			}
		}
		break;

	case BTN_IMG_DN:
		if (state & ODS_DISABLED)
		{
// 			if(m_hIcon1)
// 			{
// 				HICON hIcon=m_Image1->ExtractIcon(0);
// 				pDC->DrawState(ImgRect.TopLeft(),m_Size,hIcon,DSS_DISABLED,(HBRUSH)NULL);
// 				DestroyIcon(hIcon);
// 			}
			if(m_nBk[m_nImgBk])
			{
				rect.DeflateRect(3,3,0,0);
				m_Bk1->Draw(pDC,0,rect.TopLeft(),ILD_TRANSPARENT);//Use ImageList draw routine
			}
		}
		else
		{
			if(m_nBk[m_nImgBk])
			{
				rect.DeflateRect(3,3,0,0);
				m_Bk1->Draw(pDC,0,rect.TopLeft()+CPoint(1,1),ILD_TRANSPARENT);//Use ImageList draw routine
			}
		}
		break;
	}

	switch(m_nImg)
	{
	case BTN_IMG_UP:
		if (state & ODS_DISABLED)
		{
// 			if(m_hIcon0)
// 			{
// 				HICON hIcon=m_Image0->ExtractIcon(0);
// 				pDC->DrawState(ImgRect.TopLeft(),m_Size,hIcon,DSS_DISABLED,(HBRUSH)NULL);
// 				DestroyIcon(hIcon);
// 			}
			if(m_nImage[m_nImg])
			{
//				if(m_nImg==IMG_UP)
					m_Image0->Draw(pDC,0,ImgRect.TopLeft(),ILD_TRANSPARENT);//Use ImageList draw routine
// 				if(m_nImg==IMG_DN)
// 					m_Image0->Draw(pDC,0,ImgRect.TopLeft()+CPoint(1,1),ILD_TRANSPARENT);//Use ImageList draw routine
			}
		}
		else
		{
			if(m_nImage[m_nImg])
			{
//				if(m_nImg==IMG_UP)
					m_Image0->Draw(pDC,0,ImgRect.TopLeft(),ILD_TRANSPARENT);//Use ImageList draw routine
// 				if(m_nImg==IMG_DN)
// 					m_Image0->Draw(pDC,0,ImgRect.TopLeft()+CPoint(1,1),ILD_TRANSPARENT);//Use ImageList draw routine
			}
		}
		break;

	case BTN_IMG_DN:
		if (state & ODS_DISABLED)
		{
// 			if(m_hIcon1)
// 			{
// 				HICON hIcon=m_Image1->ExtractIcon(0);
// 				pDC->DrawState(ImgRect.TopLeft(),m_Size,hIcon,DSS_DISABLED,(HBRUSH)NULL);
// 				DestroyIcon(hIcon);
// 			}
			if(m_nImage[m_nImg])
			{
// 				if(m_nImg==IMG_UP)
// 					m_Image1->Draw(pDC,0,ImgRect.TopLeft(),ILD_TRANSPARENT);//Use ImageList draw routine
// 				if(m_nImg==IMG_DN)
					m_Image1->Draw(pDC,0,ImgRect.TopLeft(),ILD_TRANSPARENT);//Use ImageList draw routine
			}
		}
		else
		{
			if(m_nImage[m_nImg])
			{
// 				if(m_nImg==IMG_UP)
// 					m_Image1->Draw(pDC,0,ImgRect.TopLeft(),ILD_TRANSPARENT);//Use ImageList draw routine
// 				if(m_nImg==IMG_DN)
					m_Image1->Draw(pDC,0,ImgRect.TopLeft()+CPoint(1,1),ILD_TRANSPARENT);//Use ImageList draw routine
			}
		}
		break;
	}
}

void CMyBtn::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	CDC* pDC   = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rect = lpDrawItemStruct->rcItem;
	UINT state = lpDrawItemStruct->itemState;


    // draw the control edges (DrawFrameControl is handy!)
	if(m_bFrame)
		pDC->DrawFrameControl(rect, DFC_BUTTON, m_chkState);

	CPoint pt;
	pt.x = 10;
	pt.y = 10;

	CPen *pOldPen;
	pOldPen = pDC->SelectObject(m_pBoundryPen);
	
	if(m_bBoarder)
		pDC->RoundRect(rect,pt);

// 	if (state & ODS_SELECTED)
//         pDC->DrawFrameControl(rect, DFC_BUTTON, DFCS_BUTTONPUSH | DFCS_PUSHED);
//     else
//         pDC->DrawFrameControl(rect, DFC_BUTTON, DFCS_BUTTONPUSH);

	pDC->SelectObject(pOldPen);   

    // Fill the interior color if necessary
    rect.DeflateRect( CSize(GetSystemMetrics(SM_CXEDGE), GetSystemMetrics(SM_CYEDGE)));

//     if (m_bOverControl)
//         pDC->FillSolidRect(rect, RGB(255, 255, 0)); // yellow

    // Draw Image
	switch(m_nImg)
	{
	case 0:
		if(m_hBitmap0 || m_hBitmapBk0 || m_hIcon0)
			DrawImg(lpDrawItemStruct);
		break;
	case 1:
		if(m_hBitmap1 || m_hBitmapBk1 || m_hIcon1)
			DrawImg(lpDrawItemStruct);
		break;
	}

	DrawText(pDC, rect, state);


}

void CMyBtn::DrawText(CDC* pDC, CRect rect, UINT state)
{
    // Draw the text
	CString strWnd, strText, strPars, strRem;
	GetWindowText(strWnd);

	if(m_nTxtDispSt < 0)
	{
		if(m_chkState & DFCS_PUSHED)
		{
			if(!m_strDn.IsEmpty())
				strText = m_strDn;
			else
				strText = strWnd;
		}
		else
		{
			if(!m_strUp.IsEmpty())
				strText = m_strUp;
			else
				strText = strWnd;
		}
	}
	else
	{
		if(m_nTxtDispSt == BTN_UP)
		{
			if(!m_strUp.IsEmpty())
				strText = m_strUp;
			else
				strText = strWnd;
		}
		else if(m_nTxtDispSt == BTN_DN)
		{
			if(!m_strDn.IsEmpty())
				strText = m_strDn;
			else
				strText = strWnd;
		}
	}

    if (!strText.IsEmpty())
    {
		HFONT hOldFont=NULL;
		//Sets the font
		if(m_Font.GetSafeHandle() != NULL)
		{
			hOldFont =(HFONT) pDC->SelectObject(m_Font.GetSafeHandle());
		}

		CPoint pt;
		CSize Extent = pDC->GetTextExtent(strText);
//  	CPoint pt( rect.CenterPoint().x - Extent.cx/2, rect.CenterPoint().y - Extent.cy/2 );
/*
		if((m_style & BS_CENTER) == BS_CENTER)
		{
			pt.x = rect.CenterPoint().x - Extent.cx/2; 
			pt.y = rect.CenterPoint().y - Extent.cy/2;
		}
		else if((m_style & BS_RIGHT) == BS_RIGHT)
		{
			pt.x = rect.right - Extent.cx; 
			pt.y = rect.CenterPoint().y - Extent.cy/2;
		}
		else if((m_style & BS_LEFT) == BS_LEFT)
		{
			pt.x = 0; 
			pt.y = rect.CenterPoint().y - Extent.cy/2;
		}
*/
		pt.y = rect.CenterPoint().y - Extent.cy/2;

// 		if (state & ODS_SELECTED) 
//             pt.Offset(1,1);

		int nMode = pDC->SetBkMode(TRANSPARENT);
		COLORREF crTextOld = pDC->SetTextColor(m_crText);

// 		if (state & ODS_DISABLED)
// 			pDC->DrawState(pt, Extent, strText, DSS_DISABLED, TRUE, 0, (HBRUSH)NULL);
// 		else 
		{
			int nPos, nLine=0;
			CString strLineText[BTN_MAX_LINE];
			CPoint LinePos[BTN_MAX_LINE];

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
			LinePos[0].y = pt.y - (Extent.cy/2 * (nLine-1)) - (BTN_LINE_SPACE * (nLine-1) / 2);

			if (state & ODS_SELECTED || m_nImgBk==BTN_IMG_DN) 
				LinePos[0].y++;

			for(int nL=0; nL<nLine; nL++)
			{
				Extent = pDC->GetTextExtent(strLineText[nL]);
//  					LinePos[nL].x = rect.CenterPoint().x - Extent.cx/2;
// 					LinePos[nL].x = pt.x;

				if((m_style & BS_CENTER) == BS_CENTER)
				{
					LinePos[nL].x = rect.CenterPoint().x - Extent.cx/2; 
				}
				else if((m_style & BS_RIGHT) == BS_RIGHT)
				{
					LinePos[nL].x = rect.right - Extent.cx; 
				}
				else if((m_style & BS_LEFT) == BS_LEFT)
				{
					LinePos[nL].x = 0; 
				}
				else
				{
					LinePos[nL].x = rect.CenterPoint().x - Extent.cx/2; 
				}

				if (state & ODS_SELECTED || m_nImgBk==BTN_IMG_DN) 
					LinePos[nL].x++;

				if(nL>0)
					LinePos[nL].y = LinePos[nL-1].y + Extent.cy + BTN_LINE_SPACE;

				pDC->TextOut(LinePos[nL].x, LinePos[nL].y, strLineText[nL]);
			}
		}

        pDC->SetBkMode(nMode);

		//Reset the old font
		if(hOldFont != NULL)
		{
			pDC->SelectObject(hOldFont);
		}
    }

}

void CMyBtn::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CButton::PreSubclassWindow();

	m_style = GetWindowLong(this->GetSafeHwnd(), GWL_STYLE); // 스타일을 꺼내달라
	SetType();
	ModifyStyle(0, BS_OWNERDRAW);	// make the button owner drawn

	CallModifyHitRegion();
}

bool CMyBtn::CallModifyHitRegion()
{
	//To stop reentry on call to MoveWindow() below
	static bool bLock=false;	
	if(bLock)
		return false;
	bLock=true;
	
	//Update the hit region to users definition
	//
	CRect rBox;
	CRect	rClient;
	bool bValid=true;

	GetClientRect(&rClient);

	//Default region to the client rect
	m_HitRegion=rClient;		

	// Allow the user to set the window region which defines the hit test area.
	if(ModifyHitRegion(rClient,m_HitRegion))
	{
		int	nType=m_HitRegion.GetRgnBox(&rBox);
		bValid=nType!=ERROR && nType!=NULLREGION;//Is it a valid region

		if(bValid)	
		{
			CWnd* pParent=GetParent();
			// Check the new bounding box is valid then resize the button. 
			//
			bValid=!rBox.IsRectNull() && !rBox.IsRectEmpty() && pParent!=NULL;

			if(bValid)
			{
				m_HitRegion.CacheRgnBox();//Cache the client bound for quick access
				
				//Modify button size if needed
				//
				if(rClient!=rBox)
				{
					ClientToScreen(rBox);
					pParent->ScreenToClient(rBox);
					MoveWindow(rBox.left,rBox.top,rBox.Width(),rBox.Height(),true);
				}
			}
			else//If not valid reset the default region to the client rect
			{
				m_HitRegion=rClient;	
			}
		}
	}
	bLock=false;
	return bValid;
}

bool CMyBtn::ModifyHitRegion(const CRect& rClient,CHitRgn& hitRgn)
{
//	hitRgn=GetBorderRect(GetImgRect(rClient), cButtonMargin);
	return true;
}

void CMyBtn::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_nCtrlID == IDC_CHK_MK_1 && m_hParentWnd == pView->m_pDlgMenu01->GetSafeHwnd())
		return;
	if (m_nCtrlID == IDC_CHK_MK_2 && m_hParentWnd == pView->m_pDlgMenu01->GetSafeHwnd())
		return;
	if (m_nCtrlID == IDC_CHK_MK_3 && m_hParentWnd == pView->m_pDlgMenu01->GetSafeHwnd())
		return;
	if (m_nCtrlID == IDC_CHK_MK_4 && m_hParentWnd == pView->m_pDlgMenu01->GetSafeHwnd())
		return;



	if(m_ctlStyle == BS_AUTOCHECKBOX)
	{
		if(m_chkState & DFCS_PUSHED)
		{
			m_nImgBk = BTN_IMG_UP; // Up_Img
			m_chkState = DFCS_BUTTONPUSH;
			if(m_hParentWnd && m_nCtrlID)
				::PostMessage(m_hParentWnd, WM_MYBTN_UP, (WPARAM)NULL, (LPARAM)m_nCtrlID);
		}
		else
		{
			m_nImgBk = BTN_IMG_DN; // Dn_Img
			m_chkState = DFCS_BUTTONPUSH | DFCS_PUSHED;
			if(m_hParentWnd && m_nCtrlID)
				::PostMessage(m_hParentWnd, WM_MYBTN_DOWN, (WPARAM)NULL, (LPARAM)m_nCtrlID);
		}
	}
	else if(m_ctlStyle == BS_RADIOBUTTON)
	{
	}
	else
	{
		m_nImgBk = BTN_IMG_DN; // Dn_Img
		m_chkState = DFCS_BUTTONPUSH | DFCS_PUSHED;
		if(m_hParentWnd && m_nCtrlID)
			::PostMessage(m_hParentWnd, WM_MYBTN_DOWN, (WPARAM)NULL, (LPARAM)m_nCtrlID);
	}

	CButton::OnLButtonDown(nFlags, point);
}

void CMyBtn::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_nCtrlID == IDC_CHK_MK_1 && m_hParentWnd == pView->m_pDlgMenu01->GetSafeHwnd())
		return;
	if (m_nCtrlID == IDC_CHK_MK_2 && m_hParentWnd == pView->m_pDlgMenu01->GetSafeHwnd())
		return;
	if (m_nCtrlID == IDC_CHK_MK_3 && m_hParentWnd == pView->m_pDlgMenu01->GetSafeHwnd())
		return;
	if (m_nCtrlID == IDC_CHK_MK_4 && m_hParentWnd == pView->m_pDlgMenu01->GetSafeHwnd())
		return;

	if(m_ctlStyle == BS_AUTOCHECKBOX)
	{
	}
	else if(m_ctlStyle == BS_RADIOBUTTON)
	{
	}
	else
	{
		m_nImgBk = BTN_IMG_UP; // Up_Img
		m_chkState = DFCS_BUTTONPUSH;
		if(m_hParentWnd && m_nCtrlID)
			::PostMessage(m_hParentWnd, WM_MYBTN_UP, (WPARAM)NULL, (LPARAM)m_nCtrlID);
	}

	CButton::OnLButtonUp(nFlags, point);
}

BOOL CMyBtn::GetCheck()
{
	if(m_chkState & DFCS_PUSHED)
		return TRUE;

	return FALSE;
}

void CMyBtn::SetCheck(BOOL bChk)
{
	if(bChk)
	{
		m_nImgBk = BTN_IMG_DN; // Dn_Img
		m_chkState = DFCS_BUTTONPUSH | DFCS_PUSHED;
//		::PostMessage(m_hParentWnd, WM_MYBTN_DOWN, (WPARAM)NULL, (LPARAM)m_nCtrlID);
	}
	else
	{
		m_nImgBk = BTN_IMG_UP; // Up_Img
		m_chkState = DFCS_BUTTONPUSH;
//		::PostMessage(m_hParentWnd, WM_MYBTN_UP, (WPARAM)NULL, (LPARAM)m_nCtrlID);
	}

	Invalidate();
}


void CMyBtn::SetFont(CString srtFntName, int nSize, BOOL bBold)
{	
    if(m_bFont)
	{
		if(m_Font.DeleteObject())
			m_bFont = FALSE;
	}

	LOGFONT lfCtrl = {0};
	lfCtrl.lfOrientation	= 0 ;
	lfCtrl.lfEscapement		= 0 ;

	lfCtrl.lfHeight			= nSize;
	lfCtrl.lfWeight			= bBold ? FW_BOLD : FW_NORMAL;

	lfCtrl.lfItalic			= FALSE;
	lfCtrl.lfUnderline		= FALSE;
	lfCtrl.lfStrikeOut		= FALSE;

	lfCtrl.lfCharSet		= DEFAULT_CHARSET;
	lfCtrl.lfQuality		= DEFAULT_QUALITY;
	lfCtrl.lfOutPrecision	= OUT_DEFAULT_PRECIS;
	lfCtrl.lfPitchAndFamily	= DEFAULT_PITCH;
	_tcscpy(lfCtrl.lfFaceName, srtFntName);

	if(!m_bFont)
	{
		BOOL bCr = m_Font.CreateFontIndirect(&lfCtrl);
		if(bCr)
			m_bFont = TRUE;
	}
}

void CMyBtn::SetBkColor(COLORREF crBkgnd)
{
	m_crBkgnd = crBkgnd;	
}

void CMyBtn::SetText(CString str, int nSt)
{
    if(nSt==BTN_DN)
		m_strDn = str;
	else if(nSt==BTN_UP)
		m_strUp = str;
}

void CMyBtn::SetTextColor(COLORREF crText)
{
    m_crText = crText;
}

void CMyBtn::DispText(int nSt)
{
    m_nTxtDispSt = nSt;
}

void CMyBtn::SetBtnType(int nType)
{
    m_nType = nType;
	SetType();
}

void CMyBtn::SetFrame(BOOL bOn)
{
	m_bFrame = bOn;
}

void CMyBtn::SetBoarder(BOOL bOn)
{
	m_bBoarder = bOn;
}

void CMyBtn::SetType()
{
	if(m_nType == BTN_TYPE_DEFAULT)
		m_ctlStyle = GetButtonStyle();
	else
	{
		switch(m_nType)
		{
		case BTN_TYPE_CHECK:
			m_ctlStyle = BS_AUTOCHECKBOX;
			break;
		case BTN_TYPE_RADIO:
			m_ctlStyle = BS_RADIOBUTTON;
			break;
		}
	}
}

int CMyBtn::GetImage()
{
	return(m_nImg);
}

void CMyBtn::SetImage(int nImg)
{
	m_nImg = nImg;
}

int CMyBtn::GetImageBk()
{
	return(m_nImgBk);
}

void CMyBtn::SetImageBk(int nImg)
{
	m_nImgBk = nImg;
}

void CMyBtn::Refresh()
{
	RedrawWindow();
}


void CMyBtn::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_hParentWnd && m_nCtrlID)
		::PostMessage(m_hParentWnd, WM_MYBTN_DBLCLK, (WPARAM)NULL, (LPARAM)m_nCtrlID);

	CButton::OnLButtonDblClk(nFlags, point);
}

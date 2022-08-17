#if !defined(AFX_MYBTN_H__FA1EA721_FD2D_4703_9089_CBC34A74F224__INCLUDED_)
#define AFX_MYBTN_H__FA1EA721_FD2D_4703_9089_CBC34A74F224__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyBtn.h : header file
//
#include "../Global/GlobalDefine.h"

#define BTN_TYPE_DEFAULT	0
#define BTN_TYPE_CHECK		1
#define BTN_TYPE_RADIO		2

#define BTN_POS_LEFT			0
#define BTN_POS_TOP				1
#define BTN_POS_RIGHT			2
#define BTN_POS_BOTTOM			3

#define BTN_IMG_UP				0
#define BTN_IMG_DN				1

#define BTN_MAX_LINE	10
#define BTN_LINE_SPACE	2

///////////////////////////////////////////////////////////////////////
//	Class CHitRgn
//
//  Encapsulates a button hit region
//	You can either just use a rect region in which case the overhead of
//  a CRgn is unnecessary, or can set up a more complex CRgn region.
///////////////////////////////////////////////////////////////////////

class CHitRgn: public CRgn
{
public:
	CHitRgn():m_rBox(0,0,0,0){}
	~CHitRgn(){FreeCRgn();}	//Clean up

	int				GetRgnBox(LPRECT lpRect) const;	// Overload CRgn:: version to allow for simple rects
	CHitRgn&	operator=(const CRect&);				// Set hit zone to a simple rect region in client coords
	bool			Inside(CPoint pt)const;					// test if pt in client coords is in the hit region

private:
	CRect			m_rBox;	//Simple hit box bounds in client coords

	void			InitData()					{ FreeCRgn(); m_rBox=CRect(0,0,0,0);}
	bool			HaveCRgn()const			{ return GetSafeHandle()!=NULL;}	
	void			FreeCRgn()					{ if(HaveCRgn())DeleteObject();}	// Free GDI object if active

	void			CacheRgnBox()				{ if(HaveCRgn()) GetRgnBox(m_rBox);}//Copy the CRgn bounds to m_rBox
	CRect			GetCachedBox()const	{return m_rBox;}//Get cached box

	friend class CMyBtn;
};


/////////////////////////////////////////////////////////////////////////////
// CMyBtn window
	
class CMyBtn : public CButton
{
	long m_style;
	CImageList *m_Image0, *m_Image1, *m_Bk0, *m_Bk1;
	HBITMAP m_hBitmap0, m_hBitmap1, m_hBitmapBk0, m_hBitmapBk1;
	CBitmap *m_pBitmap0, *m_pBitmap1, *m_pBitmapBk0, *m_pBitmapBk1;
	HICON m_hIcon0, m_hIcon1;
	CBitmap *m_pIcon0, *m_pIcon1;

	int m_nImg, m_nImgBk;
	int m_nImage[2], m_nBk[2];
	CSize m_Size;
	CHitRgn	m_HitRegion;	//Default is the client rect
	UINT m_chkState;
	UINT m_ctlStyle;
	int m_nType, m_nCtrlID, m_nImgPos;
	BOOL m_bBoarder, m_bFrame;

	LOGFONT		m_lf;
	COLORREF	m_crText;
	HBRUSH		m_hBrush;
	HBRUSH		m_hwndBrush;
	CFont		m_Font;
	BOOL		m_bFont;
	COLORREF	m_crBkgnd;

	CPen *m_pBoundryPen;

	CString m_strUp, m_strDn;
	int m_nTxtDispSt;

	void DrawImg(LPDRAWITEMSTRUCT lpDrawItemStruct);
	CRect GetBorderRect(const CRect& rButton,CSize sInflate);
	CRect GetImgRect(CRect rClient);
	bool ModifyHitRegion(const CRect& rClient,CHitRgn& hitRgn);
	void SetType();
	void DrawText(CDC* pDC, CRect rect, UINT state);

	//Support for a user defined hit region
	bool	CallModifyHitRegion();


// Construction
public:
	CMyBtn();

// Attributes
public:
	HWND m_hParentWnd;

// Operations
public:
	void SetHwnd(HWND hParentWnd=NULL, int nCtrlID=NULL);
	BOOL LoadImage(CString strImgPath, int nImg=0, CSize sz=CSize(0,0), int nImgPos=BTN_POS_LEFT);
	BOOL LoadBkImage(CString strImgPath, int nImg=0);
	BOOL GetCheck();
	void SetCheck(BOOL bChk);
	COLORREF GetBkColor() {return m_crBkgnd;}
	void SetBkColor(COLORREF crBkgnd=RGB_WHITE);
	void SetTextColor(COLORREF crText=RGB_BLACK);
	void SetText(CString str, int nSt=BTN_UP);
	void DispText(int nSt=-1);
	void SetFont(CString srtFntName, int nSize=10, BOOL bBold=FALSE);
	void SetBtnType(int nType);
	void SetImage(int nImg);
	int GetImage();
	void SetImageBk(int nImg);
	int GetImageBk();
	void DelImgList();
	void Refresh();
	void SetFrame(BOOL bOn=FALSE);
	void SetBoarder(BOOL bOn=TRUE);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyBtn)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyBtn();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyBtn)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYBTN_H__FA1EA721_FD2D_4703_9089_CBC34A74F224__INCLUDED_)

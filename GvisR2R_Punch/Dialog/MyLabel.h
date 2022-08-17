#if !defined(AFX_MYLABEL_H__A4EABEC5_2E8C_11D1_B79F_00805F9ECE10__INCLUDED_)
#define AFX_MYLABEL_H__A4EABEC5_2E8C_11D1_B79F_00805F9ECE10__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Label.h : header file
//
#include "../Global/GlobalDefine.h"

#define LBL_POS_LEFT			0
#define LBL_POS_TOP				1
#define LBL_POS_RIGHT			2
#define LBL_POS_BOTTOM			3
#define LBL_POS_CENTER			4

#define LBL_IMG_UP				0
#define LBL_IMG_DN				1

#define TIM_LBL_REFRESH			231

#define MAX_LINE_LABEL		10
#define LINE_SPACE_LABEL	2

/////////////////////////////////////////////////////////////////////////////
// CMyLabel window
enum MyFlashType {_None, _Text, _Background };

class CMyLabel : public CStatic
{
	long m_style;
	CImageList *m_Image0, *m_Image1, *m_Bk0, *m_Bk1;
	HBITMAP m_hBitmap0, m_hBitmap1, m_hBitmapBk0, m_hBitmapBk1;
	HICON m_hIcon0, m_hIcon1;
	CBitmap *m_pBitmap0, *m_pBitmap1, *m_pBitmapBk0, *m_pBitmapBk1;
	CBitmap *m_pIcon0, *m_pIcon1;
//	CBitmap* m_pBitmapBk;
	int m_nImg, m_nItems, m_nImgPos;
	int m_nImage[2], m_nBk[2];
	CSize m_Size;
	BOOL m_bBoarder, m_bFont;
	CPoint m_ptRound;
	CString m_sCaption;

	void DrawBkImg(CPaintDC* dc);
	void DrawText();
	CRect GetImgRect(CRect rClient);
	
// Construction
public:
	CMyLabel();
	COLORREF GetBkColor() {return m_crBkgnd;}
	COLORREF GetTextColor(){return m_crText;}
	//CString GetText(){return m_strText;}
	BOOL	GetFontBold(){return m_bBold;}
	CString GetFontName(){return m_fontName;}
	int GetFontSize(){return m_fontSize;}

	CMyLabel& SetBkColor(COLORREF crBkgnd=RGB_WHITE);
	CMyLabel& SetTextColor(COLORREF crText=RGB_BLACK);
//	CMyLabel& SetText(const CString& strText);
	CMyLabel& SetFontBold(BOOL bBold);
	CMyLabel& SetFontName(const CString& strFont);
	CMyLabel& SetFontUnderline(BOOL bSet);
	CMyLabel& SetFontItalic(BOOL bSet);
	CMyLabel& SetFontSize(int nSize);
	CMyLabel& SetSunken(BOOL bSet);
	CMyLabel& SetBorder(BOOL bSet);
	CMyLabel& FlashText(BOOL bActivate);
	CMyLabel& FlashBackground(BOOL bActivate);
	CMyLabel& SetLink(BOOL bLink);
	CMyLabel& SetLinkCursor(HCURSOR hCursor);

// Attributes
public:
	BOOL LoadBkImage(CString strImgPath, int nImg=0);
	BOOL LoadImage(CString strImgPath, int nImg=0, CSize sz=CSize(0,0), int nImgPos=LBL_POS_LEFT);
	void DrawBoarder(int RndX=10, int RndY=10, BOOL bDraw=TRUE);
	void DelImgList();
	void Refresh();
	int GetImageBk();
	void SetImageBk(int nImg);
	void RefreshText();
	void SetText(CString sText);
	CString GetText();


protected:
	void ReconstructFont();
	COLORREF	m_crText;
	COLORREF	m_crBkgnd;
	HBRUSH		m_hBrush;
	HBRUSH		m_hwndBrush;
	LOGFONT		m_lf;
	CFont		m_font;
	int			m_fontSize;
	CString		m_fontName;
	BOOL		m_bBold;
	CString		m_strText;
	BOOL		m_bState;
	BOOL		m_bTimer;
	BOOL		m_bLink;
	MyFlashType	m_Type;
	HCURSOR		m_hCursor;
			// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyLabel)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyLabel();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyLabel)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);// (UINT nIDEvent);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYLABEL_H__A4EABEC5_2E8C_11D1_B79F_00805F9ECE10__INCLUDED_)

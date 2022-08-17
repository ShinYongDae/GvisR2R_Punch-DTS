#if !defined(AFX_MYSTATIC_H__2F38C016_9D8C_4B00_ADF1_63159B957509__INCLUDED_)
#define AFX_MYSTATIC_H__2F38C016_9D8C_4B00_ADF1_63159B957509__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyStatic.h : header file
//
#include "../Global/GlobalDefine.h"

/////////////////////////////////////////////////////////////////////////////
// CMyStatic window

class CMyStatic : public CStatic
{
	BOOL m_bFont;
	COLORREF	m_crText;
	COLORREF	m_crBkgnd;
	HBRUSH		m_hBrush;
//	HBRUSH		m_hwndBrush;
	LOGFONT		m_lf;
	CFont		m_font;
	int			m_fontSize;
	CString		m_fontName;
	BOOL		m_bBold;
	CString		m_strText;
	BOOL		m_bState;
	HCURSOR		m_hCursor;
	int			m_nCtrlID;

	CPoint		m_ptMouse;

	void ReconstructFont();

// Construction
public:
	CMyStatic();

// Attributes
public:
	HWND m_hParentWnd;

// Operations
public:
	COLORREF GetBkColor() {return m_crBkgnd;}
	COLORREF GetTextColor(){return m_crText;}
	CString GetText(){return m_strText;}
	BOOL	GetFontBold(){return m_bBold;}
	CString GetFontName(){return m_fontName;}
	int GetFontSize(){return m_fontSize;}

	void SetHwnd(HWND hParentWnd=NULL, int nCtrlID=NULL);
	void SetBkColor(COLORREF crBkgnd=RGB_WHITE);
	void SetTextColor(COLORREF crText=RGB_BLACK);
	void SetText(const CString& strText);
	void SetFontBold(BOOL bBold);
	void SetFontName(const CString& strFont);
	void SetFontUnderline(BOOL bSet);
	void SetFontItalic(BOOL bSet);
	void SetFontSize(int nSize);
	void SetSunken(BOOL bSet);
	void SetBorder(BOOL bSet);
	void Refresh();

	CPoint GetMousePoint();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyStatic)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSTATIC_H__2F38C016_9D8C_4B00_ADF1_63159B957509__INCLUDED_)

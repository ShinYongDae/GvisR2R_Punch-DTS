#if !defined(AFX_MYEDIT_H__6DF70625_680E_46C6_B7EC_5E1B5BEE5934__INCLUDED_)
#define AFX_MYEDIT_H__6DF70625_680E_46C6_B7EC_5E1B5BEE5934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyEdit.h : header file
//
#include "../Global/GlobalDefine.h"

/////////////////////////////////////////////////////////////////////////////
// CMyEdit window

class CMyEdit : public CEdit
{
	int			m_nCtrlID;
// 	HWND		m_hParentWnd;

	BOOL		m_bFont;
	COLORREF	m_crBkgnd;
	COLORREF	m_crText;
	HBRUSH		m_hBrush;
	LOGFONT		m_lf;
	int			m_fontSize;
	CString		m_fontName;
	BOOL		m_bBold;
	CString		m_strText;

	void ReconstructFont();


// Construction
public:
	CMyEdit();

// Attributes
public:
	HWND		m_hParentWnd;
	CFont		m_font;

// Operations
public:
	void SetHwnd(HWND hParentWnd, int nCtrlID);

	COLORREF GetBkColor() {return m_crBkgnd;}
	COLORREF GetTextColor(){return m_crText;}
	CString GetText(){return m_strText;}
	BOOL	GetFontBold(){return m_bBold;}
	CString GetFontName(){return m_fontName;}
	int GetFontSize(){return m_fontSize;}

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

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyEdit)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYEDIT_H__6DF70625_680E_46C6_B7EC_5E1B5BEE5934__INCLUDED_)

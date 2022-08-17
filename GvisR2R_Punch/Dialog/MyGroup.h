#if !defined(AFX_MYGROUP_H__820B9BBB_8A59_43A8_BC64_9CB8F52AA9CC__INCLUDED_)
#define AFX_MYGROUP_H__820B9BBB_8A59_43A8_BC64_9CB8F52AA9CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyGroup.h : header file
//
#include "../Global/GlobalDefine.h"

/////////////////////////////////////////////////////////////////////////////
// CMyGroup window

class CMyGroup : public CStatic
{
	BOOL		m_bFont;
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
	int			m_nCtrlID;

	void ReconstructFont();
	CRect GetImgRect(CRect rClient);
	void DrawBkColor(CPaintDC *pDc, COLORREF crBkgnd);
	void DrawRound(CPaintDC *pDc);
	void DrawText(CDC* pDC);

// Construction
public:
	CMyGroup();

// Attributes
public:
	HWND m_hParentWnd;

// Operations
public:
	void SetHwnd(HWND hParentWnd=NULL, int nCtrlID=NULL);
	COLORREF GetBkColor() {return m_crBkgnd;}
	void SetBkColor(COLORREF crBkgnd=RGB_WHITE);
	void SetTextColor(COLORREF crText=RGB_BLACK);
	void SetFontName(const CString& strFont);
	void SetFontSize(int nSize);
	void SetFontBold(BOOL bBold);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyGroup)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyGroup();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyGroup)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYGROUP_H__820B9BBB_8A59_43A8_BC64_9CB8F52AA9CC__INCLUDED_)

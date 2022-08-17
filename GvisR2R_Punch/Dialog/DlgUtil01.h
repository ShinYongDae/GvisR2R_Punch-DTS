#if !defined(AFX_DLGUTIL01_H__E791A0C6_0497_4E02_A125_EE9430BFE182__INCLUDED_)
#define AFX_DLGUTIL01_H__E791A0C6_0497_4E02_A125_EE9430BFE182__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgUtil01.h : header file
//

// #include "Vision.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgUtil01 dialog

class CDlgUtil01 : public CDialog
{
// Construction
public:
	CDlgUtil01(CWnd* pParent = NULL);   // standard constructor
	~CDlgUtil01();

	CRect* m_pRect;
// 	CVision* m_pVision;

	BOOL Create();
	void AtDlgShow();
	void AtDlgHide();

// Dialog Data
	//{{AFX_DATA(CDlgUtil01)
	enum { IDD = IDD_DLG_UTIL_01 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgUtil01)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgUtil01)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGUTIL01_H__E791A0C6_0497_4E02_A125_EE9430BFE182__INCLUDED_)

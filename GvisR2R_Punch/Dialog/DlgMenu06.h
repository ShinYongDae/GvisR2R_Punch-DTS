#if !defined(AFX_DLGMENU06_H__75EE004F_9202_4E98_8378_A75CC1777324__INCLUDED_)
#define AFX_DLGMENU06_H__75EE004F_9202_4E98_8378_A75CC1777324__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMenu06.h : header file
//


#include "MyGL.h"
#include "MyStatic.h"


/////////////////////////////////////////////////////////////////////////////
// CDlgMenu06 dialog

class CDlgMenu06 : public CDialog
{
	CWnd* m_pParent;
	CRect* m_pRect;
	CMyGL *m_pMyGL_Inner, *m_pMyGL_Outer;
	CMyStatic myStcReelmapInner, myStcReelmapOuter;

	void AtDlgShow();
	void AtDlgHide();

	void InitStatic();

// Construction
public:
	CDlgMenu06(CWnd* pParent = NULL);   // standard constructor
	~CDlgMenu06();

	BOOL Create();

	void InitGL();
	void ResetGL();
	void SetPnlNum();
	void SetPnlDefNum();
	void SetRmapRgn();

	void RefreshRmap();

	BOOL SetSerialReelmap(int nSerial, BOOL bDumy = FALSE);
	void DispReelmap(int nSerial, BOOL bDumy = FALSE);

	afx_msg LRESULT OnDrawReelMapInner(WPARAM wPara, LPARAM lPara);
	afx_msg LRESULT OnDrawReelMapOuter(WPARAM wPara, LPARAM lPara);
	afx_msg LRESULT OnMyStaticRedraw(WPARAM wPara, LPARAM lPara);

// Dialog Data
	//{{AFX_DATA(CDlgMenu06)
	enum { IDD = IDD_DLG_MENU_06 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMenu06)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMenu04)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);// (UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMENU06_H__75EE004F_9202_4E98_8378_A75CC1777324__INCLUDED_)

#if !defined(AFX_DLGUSER_H__E742A694_8C94_4D6E_B944_B2AE4BCAD955__INCLUDED_)
#define AFX_DLGUSER_H__E742A694_8C94_4D6E_B944_B2AE4BCAD955__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgUser.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgUser dialog

class CDlgUser : public CDialog
{
	CFont m_FontOfListCtrl;
	CString m_strOrgUserName;

	int ReadUserNameListFile();
	void WriteUserNameListFile();
	void DisplayUserNameList();

// Construction
public:
	CDlgUser(CWnd* pParent = NULL);   // standard constructor
	~CDlgUser();

// Dialog Data
	//{{AFX_DATA(CDlgUser)
	enum { IDD = IDD_DLG_USER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgUser)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgUser)
	virtual BOOL OnInitDialog();
	afx_msg void OnChk00();
	afx_msg void OnBtn00();
	afx_msg void OnChk02();
	afx_msg void OnChk01();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGUSER_H__E742A694_8C94_4D6E_B944_B2AE4BCAD955__INCLUDED_)

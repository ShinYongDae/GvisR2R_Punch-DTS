#if !defined(AFX_DLGUTIL02_H__651C30B5_F2FE_4797_8D2A_5911222EEA87__INCLUDED_)
#define AFX_DLGUTIL02_H__651C30B5_F2FE_4797_8D2A_5911222EEA87__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgUtil02.h : header file
//

#include "MyBtn.h"
#include "MyLabel.h"
#include "MyStatic.h"

#define MAX_UTIL02_STC				1
#define MAX_UTIL02_STC_DATA			1
#define MAX_UTIL02_BTN				16

/////////////////////////////////////////////////////////////////////////////
// CDlgUtil02 dialog

class CDlgUtil02 : public CDialog
{
	CMyBtn myBtn[MAX_UTIL02_BTN];
	CMyLabel myStcTitle[MAX_UTIL02_STC];
	CMyStatic myStcData[MAX_UTIL02_STC_DATA];

	BOOL m_bLoadImg;

	void LoadImg();
	void DelImg();

	void InitStatic();
	void InitStcTitle();
	void InitStcData();
	void InitBtn();

// Construction
public:
	CDlgUtil02(CWnd* pParent = NULL);   // standard constructor
	~CDlgUtil02();

	CRect* m_pRect;

	BOOL Create();
	void AtDlgShow();
	void AtDlgHide();

// Dialog Data
	//{{AFX_DATA(CDlgUtil02)
	enum { IDD = IDD_DLG_UTIL_02 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgUtil02)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgUtil02)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGUTIL02_H__651C30B5_F2FE_4797_8D2A_5911222EEA87__INCLUDED_)

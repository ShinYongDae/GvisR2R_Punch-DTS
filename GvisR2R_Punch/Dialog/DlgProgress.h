#if !defined(AFX_DLGPROGRESS_H__A97B137C_AFDF_4C55_9674_04821871A529__INCLUDED_)
#define AFX_DLGPROGRESS_H__A97B137C_AFDF_4C55_9674_04821871A529__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgProgress.h : header file
//
#include "../resource.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgProgress dialog

class CDlgProgress : public CDialog
{
// Construction
public:
	CDlgProgress(CWnd* pParent = NULL);   // standard constructor
	CDlgProgress(CFormView* pView);
	~CDlgProgress();
	BOOL Create(CString strCaption);

// Dialog Data
	//{{AFX_DATA(CDlgProgress)
	enum { IDD = IDD_PROGRESS };
	CProgressCtrl	m_ctrlProgress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgProgress)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL CheckCancelButton();
	void SetCaption(CString strCaption);
	int StepIt();
	int OffsetPos(int nPos);
	int SetStep(int nStep);
	void SetRange(int nLower,int nUpper);
	int  SetPos(int nPos);
	void Quit();

private: 
	void UpdatePercent(int nNewPos);
	void PumpMessages();
	void ReEnableParent();
	CFormView *m_pView;
	BOOL m_bParentDisabled;
	BOOL m_bCancel;
	CString m_strCaption;
	int m_nLower;
    int m_nUpper;
    int m_nStep;


protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgProgress)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROGRESS_H__A97B137C_AFDF_4C55_9674_04821871A529__INCLUDED_)

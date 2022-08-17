#if !defined(AFX_DLGUTIL06_H__49EB7683_4F7F_43F7_8138_AB0D88786440__INCLUDED_)
#define AFX_DLGUTIL06_H__49EB7683_4F7F_43F7_8138_AB0D88786440__INCLUDED_

#ifdef USE_FLUCK

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgUtil06.h : header file
//

#include "MyStatic.h"
#include "MyBtn.h"

#define MAX_UTIL06_BTN		7
#define MAX_UTIL06_STC		46
#define MAX_UTIL06_STC_DATA	21

#define TIM_GET		70

/////////////////////////////////////////////////////////////////////////////
// CDlgUtil06 dialog

class CDlgUtil06 : public CDialog
{
	CRect* m_pRect;
	BOOL m_bLoadImg;
// 	CMyBtn myBtn[MAX_UTIL06_BTN];
	CMyStatic myStcTitle[MAX_UTIL06_STC];
	CMyStatic myStcData[MAX_UTIL06_STC_DATA];
	double m_dCurrPos[2][2];

	void AtDlgShow();
	void AtDlgHide();
	void LoadImg();
	void DelImg();
	void InitBtn();
	void InitStatic();
	void InitStcTitle();
	void InitStcData();
	BOOL ShowKeypad(int nCtlID, CPoint ptSt, int nDir);
	void Disp();


// Construction
public:
	CDlgUtil06(CWnd* pParent = NULL);   // standard constructor
	~CDlgUtil06();

	CMyBtn myBtn[MAX_UTIL06_BTN];
	
	BOOL Create();

	BOOL MoveMeasPos(int nId);
	void SaveMeasPos(int nId);

// Dialog Data
	//{{AFX_DATA(CDlgUtil06)
	enum { IDD = IDD_DLG_UTIL_06 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgUtil06)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgUtil06)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	afx_msg void OnStc132();
	afx_msg void OnStc136();
	afx_msg void OnStc139();
	afx_msg void OnStc143();
	afx_msg void OnStc147();
	afx_msg void OnStc150();
	afx_msg void OnStc153();
	afx_msg void OnStc93();
	afx_msg void OnStc97();
	afx_msg void OnStc110();
	afx_msg void OnStc113();
	afx_msg void OnStc117();
	afx_msg void OnStc120();
	afx_msg void OnStc123();
	afx_msg void OnStc161();
	afx_msg void OnStc165();
	afx_msg void OnBtnMeasStart();
	afx_msg void OnTimer(UINT_PTR nIDEvent);// (UINT nIDEvent);
	afx_msg void OnBtnMeasMove();
	afx_msg void OnBtnMeasDown();
	afx_msg void OnBtnMeasSave();
	afx_msg void OnBtnMeasMove2();
	afx_msg void OnBtnMeasDown2();
	afx_msg void OnBtnMeasSave2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #ifdef USE_FLUCK

#endif // !defined(AFX_DLGUTIL06_H__49EB7683_4F7F_43F7_8138_AB0D88786440__INCLUDED_)

#if !defined(AFX_DLGUTIL04_H__5AD52783_D5AC_4936_89AF_DDFE5F3B0F85__INCLUDED_)
#define AFX_DLGUTIL04_H__5AD52783_D5AC_4936_89AF_DDFE5F3B0F85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgUtil04.h : header file
//

#include "MyStatic.h"
#include "MyLabel.h"
#include "MyBtn.h"

#define MAX_UTIL04_STC				71
#define MAX_UTIL04_LABEL			32
#define MAX_UTIL04_BTN				4

#define TIM_DISP_STS				450

/////////////////////////////////////////////////////////////////////////////
// CDlgUtil04 dialog

class CDlgUtil04 : public CDialog
{
	CMyBtn myBtn[MAX_UTIL04_BTN];
	CMyStatic myStc[MAX_UTIL04_STC];
	CMyLabel myLabel[MAX_UTIL04_LABEL];

	BOOL m_bLoadImg;
	BOOL m_bTIM_DISP_STS;

	int m_nSel;
	int m_nMaxSeg, m_nInSeg, m_nOutSeg;
	int m_nComboIndex;
	CString m_sAddrIn[16], m_sAddrOut[16], m_sMRegOut[16], m_sMDblOut[16], m_sSDblOut[16];

	void LoadImg();
	void DelImg();

	void InitStatic();
	void InitLabel();
	void InitBtn();
	void InitCombo();
	void SetCombo(int nSel);
	void SetBtn(int nSel=0);

	void Disp(int nIdx);
	void DispListMp2100(int nIndex);
// 	void DispListSliceIo(int nIndex);
	void DispStatus();
	void SetTextIn(int nRow, int nCol, CString sText);
	void SetTextOut(int nRow, int nCol, CString sText);
	BOOL ApplyCombo1();
	void ShowDlgMyIo(CString sSym, CString sText, int nIdx);

	
// Construction
public:
	CDlgUtil04(CWnd* pParent = NULL);   // standard constructor
	~CDlgUtil04();

	CRect* m_pRect;

	BOOL Create();
	void AtDlgShow();
	void AtDlgHide();

// Dialog Data
	//{{AFX_DATA(CDlgUtil04)
	enum { IDD = IDD_DLG_UTIL_04 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgUtil04)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgUtil04)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnUtil040();
	afx_msg void OnBtnUtil041();
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnStcUtil0456();
	afx_msg void OnChk1();
	afx_msg void OnChk2();
	afx_msg void OnTimer(UINT_PTR nIDEvent);// (UINT nIDEvent);
	afx_msg void OnStcUtil0459();
	afx_msg void OnStcUtil0462();
	afx_msg void OnStcUtil0465();
	afx_msg void OnStcUtil0468();
	afx_msg void OnStcUtil0471();
	afx_msg void OnStcUtil0474();
	afx_msg void OnStcUtil0477();
	afx_msg void OnStcUtil0480();
	afx_msg void OnStcUtil0483();
	afx_msg void OnStcUtil0486();
	afx_msg void OnStcUtil0489();
	afx_msg void OnStcUtil0492();
	afx_msg void OnStcUtil0495();
	afx_msg void OnStcUtil0498();
	afx_msg void OnStcUtil04101();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGUTIL04_H__5AD52783_D5AC_4936_89AF_DDFE5F3B0F85__INCLUDED_)

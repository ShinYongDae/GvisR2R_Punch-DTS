#if !defined(AFX_DLGUTIL05_H__43ACF262_44F9_42F7_B1AC_4AC1AF2A989C__INCLUDED_)
#define AFX_DLGUTIL05_H__43ACF262_44F9_42F7_B1AC_4AC1AF2A989C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgUtil05.h : header file
//
#include "MyPic.h"
#include "MyBtn.h"
#include "MyStatic.h"

#define TIM_DLG_MY_IO 210

/////////////////////////////////////////////////////////////////////////////
// CDlgUtil05 dialog

class CDlgUtil05 : public CDialog
{
	BOOL m_bLoadImg;
	CRect* m_pRect;
	CWnd* m_pParent;
	CMyBtn myBtn[3];
	CMyPic myPic;
	CMyStatic m_stc[2];
	CString m_strParam[2];
	CString m_strSym, m_strOutName;
	BOOL m_bTIM_DLG_MY_IO, m_bTIM_DLG_MY_IO_Lock;
	int	m_nSegment, m_nIdxSliceIo, m_nIdxMpeIo;


	void AtDlgShow();
	void AtDlgHide();
	void LoadImg();
	void DelImg();
	void InitPic();
	void InitStatic();
	void InitBtn();
	BOOL IsDblSol();
	void Disp();
	
// Construction
public:
	CDlgUtil05(CWnd* pParent = NULL);   // standard constructor
	~CDlgUtil05();

	BOOL Create();
	void SetParamMp2100(CString *pParam);
	void SetParamSliceIo(int nSegment, int nIdx);
	void SetParamMpeIo(int nSegment, int nIdx);
	void SetDisp(CString strParam0, CString strParam1);
	void DispIo();
	afx_msg LRESULT  OnMyIo(WPARAM wPara, LPARAM lPara);

// Dialog Data
	//{{AFX_DATA(CDlgUtil05)
	enum { IDD = IDD_DLG_UTIL_05 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgUtil05)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgUtil05)
	afx_msg void OnBtn00();
	afx_msg void OnBtn01();
	afx_msg void OnBtn02();
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);// (UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGUTIL05_H__43ACF262_44F9_42F7_B1AC_4AC1AF2A989C__INCLUDED_)

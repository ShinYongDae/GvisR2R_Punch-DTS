#if !defined(AFX_DLGMSGBOX_H__5004120D_CC77_482A_949C_4B785A724589__INCLUDED_)
#define AFX_DLGMSGBOX_H__5004120D_CC77_482A_949C_4B785A724589__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMsgBox.h : header file
//
#include "Label.h"
#include "../resource.h"
#include <afxmt.h>


/////////////////////////////////////////////////////////////////////////////
// CDlgMsgBox dialog

class CDlgMsgBox : public CDialog
{
	CRect *m_pRect, *m_pRectView;

	DWORD m_dwStartTick, m_dwEndTick;
	
// Construction
public:
	CDlgMsgBox(CWnd* pParent = NULL);   // standard constructor
	CDlgMsgBox(CFormView* pView,CString strTitle,CString strMsg,DWORD dwInMilliSeconds,COLORREF color); // Modaless Dialog Step2
	~CDlgMsgBox();

// Attributes
public:
	BOOL m_bModify;
	BOOL m_bAlive;
	DWORD m_DispTime;
	HBRUSH m_hBkColor; 

// Operations
public:
	BOOL Create();
	void Close(); // Modaless Dialog Step3
	void TansText();
	void SetDispMsg(CString strMsg, CString strTitle, DWORD dwInMilliSeconds, COLORREF color);
	void GetDispMsg(CString &strMsg, CString &strTitle);

private:
	CFormView* m_pView;
	COLORREF m_TextColor;
	CString m_strTitle;
	CString m_strMsg;
	BOOL SetCtrlColor(const UINT nID, CString strText,COLORREF bgColor,COLORREF fgColor);
	CEvent      m_evtMsgBoxThread;
	CWinThread *m_pMsgBoxThread;
	HANDLE		m_hMsgBoxThread;
	void StartThread(); 
	void StopThread();
	
	void WaitUntilThreadEnd(HANDLE hThread);
	static UINT DispMsgThread(LPVOID pParam);
// Dialog Data
	//{{AFX_DATA(CDlgMsgBox)
	enum { IDD = IDD_DLG_DISPMSG};
	CLabel	m_lbTitleMsg;
	CLabel	m_lbRemainTime;
	CLabel	m_lbMsg;
	CLabel  m_lbLogo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMsgBox)
	public:
	virtual BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMsgBox)
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual void OnOK();
	afx_msg void OnTime();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);// (UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMSGBOX_H__5004120D_CC77_482A_949C_4B785A724589__INCLUDED_)

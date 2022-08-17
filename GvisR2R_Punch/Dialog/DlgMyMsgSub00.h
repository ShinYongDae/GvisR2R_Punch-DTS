#if !defined(AFX_DLGMYMSGSUB00_H__14F11912_891C_4D1D_B621_B53866632B2A__INCLUDED_)
#define AFX_DLGMYMSGSUB00_H__14F11912_891C_4D1D_B621_B53866632B2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMyMsgSub00.h : header file
//
#include "../resource.h"
#include "../Process/ThreadTask.h"

#include "MyPic.h"
#include "MyBtn.h"
#include "MyLabel.h"
#include "MyEdit.h"
#include "MyStatic.h"
#include "DlgMyMsg.h"

#ifndef FONT_SIZE
	#define FONT_SIZE	16
#endif
#ifndef LINE_SPACE
	#define LINE_SPACE	5
#endif

#define TIM_MYMSG00			213
#define TIM_KILL_FOCUS00		214

/////////////////////////////////////////////////////////////////////////////
// CDlgMyMsgSub00 dialog

class CDlgMyMsgSub00 : public CDlgMyMsg
{
	BOOL m_bLoadImg;
// 	HWND m_hParentWnd;
	CRect* m_pRect;
	CWnd* m_pParent;
	CMyPic myPic;
	CMyBtn myBtn00;
	
	CMyLabel myLabel00;
	CMyEdit myEdit00;
// 	CMyStatic m_stc00;


	CString m_sMsg, m_sPrev, m_sPrevEdit;

	BOOL m_bThreadAlive;
	CThreadTask m_ThreadTask;

	BOOL m_bResetTime;

	void AtDlgShow();
	void AtDlgHide();
	void InitPic();
	void InitStatic();
	void InitEdit();
	void InitLabel();
	void InitBtn();
	void LoadImg();
	void DelImg();
	void KillFocus(int nID);

	void StartThread();
	void StopThread();

// Construction
public:
	CDlgMyMsgSub00(CWnd* pParent=NULL, int nIDD=IDD);   // standard constructor
	~CDlgMyMsgSub00();


	HWND m_hParentWnd;
	BOOL m_bExit;
	int m_nStTimer;
	int m_nTimeOut;

	HWND m_hStcTime;
	CWnd* m_pStcTime;
	CMyStatic m_stc00;

	BOOL Create();
	afx_msg LRESULT OnMyBtnDown(WPARAM wPara, LPARAM lPara);
	afx_msg LRESULT OnMyBtnUp(WPARAM wPara, LPARAM lPara);
	void SetHwnd(HWND hParentWnd);
	void Refresh();
	afx_msg LRESULT OnMyMsg00(WPARAM wPara, LPARAM lPara);

	static BOOL ThreadProc(LPVOID lpContext);
	void CloseMsgDlg();
	void DispTime();
// 	void DispTime(HWND hDlg);

// Dialog Data
	//{{AFX_DATA(CDlgMyMsgSub00)
	enum { IDD = IDD_DLG_MY_MSG_SUB00 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMyMsgSub00)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMyMsgSub00)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);// (UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMYMSGSUB00_H__14F11912_891C_4D1D_B621_B53866632B2A__INCLUDED_)

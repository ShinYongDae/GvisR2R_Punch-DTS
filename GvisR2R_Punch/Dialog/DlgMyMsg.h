#if !defined(AFX_DLGMYMSG_H__6BF6D8B8_DB6A_4786_B09B_8B30449FBF40__INCLUDED_)
#define AFX_DLGMYMSG_H__6BF6D8B8_DB6A_4786_B09B_8B30449FBF40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMyMsg.h : header file
//

#include <Afxmt.h>
#include <afxtempl.h>

#include "../resource.h"

#define DEFAULT_TIME_OUT			120000		// [mSec]
#define MB_TIME_OUT					-1
#define MAX_THREAD_ID				100

typedef struct _MyMsgBox
{
	CString strMsg;
	int nThreadIdx;
	int nType;
	int nTimOut;
	BOOL bRepeat;

	void Init()
	{
		strMsg = _T("");
		nThreadIdx = 0;
		nType = MB_OK;
		nTimOut = DEFAULT_TIME_OUT;
		bRepeat = TRUE;
	}

	_MyMsgBox()
	{
		Init();
	}
	_MyMsgBox(CString sMsg, int MsgId, int Type, int TimOut, BOOL Repeat=TRUE)
	{
		strMsg = sMsg;
		nThreadIdx = MsgId;
		nType = Type;
		nTimOut = TimOut;
		bRepeat = Repeat;
	}

}stMyMsgBox;
typedef CArray <stMyMsgBox, stMyMsgBox> CArMyMsgBox;

typedef struct _MyMsgBoxRtnWait
{
	CString strMsg;
	int nThreadIdx;
	int nRtnVal;

	void Init()
	{
		strMsg = _T("");
		nThreadIdx = -1;
		nRtnVal = -1;
	}

	_MyMsgBoxRtnWait()
	{
		Init();
	}

	_MyMsgBoxRtnWait(CString sMsg, int MsgId, int RtnVal)
	{
		strMsg = sMsg;
		nThreadIdx = MsgId;
		nRtnVal = RtnVal;
	}

}stMyMsgBoxRtnWait;
typedef CArray <stMyMsgBoxRtnWait, stMyMsgBoxRtnWait> CArMyMsgBoxRtnWait;

typedef CArray <int, int> CArMyMsgBoxRtn;

/////////////////////////////////////////////////////////////////////////////
// CDlgMyMsg dialog

class CDlgMyMsg : public CDialog
{
	CCriticalSection m_csSetMsg, m_csDispMsg;
	CArMyMsgBox  m_ArrayMyMsgBox, m_ArrayMyMsgBoxSync, m_ArrayMyMsgBoxAsync;
	CArMyMsgBoxRtn  m_ArrayMyMsgBoxRtn;
	CArMyMsgBoxRtnWait m_ArrayMyMsgBoxRtnWait;
	CRect* m_pRect;
// 	HWND m_hParentWnd;
	CWnd* m_pParent;

	int m_nTimeOut;
	int m_nRtnVal;
	BOOL m_bSetMsg;

	CDlgMyMsg* m_pDlgMyMsg;

	virtual void AtDlgShow();
	virtual void AtDlgHide();
	BOOL DispMsgFromTimer();
	int DoDispMsg(CString sMsg, int nThreadIdx=0, int nType=MB_OK, int nTimeOut=DEFAULT_TIME_OUT);
	void StopTimer();

// Construction
public:
	CDlgMyMsg(CWnd* pParent=NULL, int nIDD=IDD);   // standard constructor
	~CDlgMyMsg();

	HWND m_hParentWnd;
	int m_nPosX, m_nPosY;
	BOOL m_bCloseMsg;
	BOOL m_bTimDlgMyMsg;
	int m_nThreadIdBuf[MAX_THREAD_ID];

	virtual BOOL Create();

	afx_msg LRESULT OnMyMsgExit(WPARAM wPara, LPARAM lPara);

	int GetTimeOut();
	void SetMsg(CString strMsg, int nThreadIdx=0, int nType=MB_TIME_OUT, int nTimeOut=DEFAULT_TIME_OUT);
	CString GetMsg();
	void RemoveMsg();
	void SetRtnVal(int nVal);
	int GetRtnVal(int nThreadIdx=0);
	int GetWaitRtnVal(int nThreadIdx=0);
	int WaitRtnVal(int nThreadIdx=1);

	int DispMsg(CString sMsg, int nThreadIdx=0, int nType=MB_OK, int nTimeOut=DEFAULT_TIME_OUT);
	void SetWndPos(int nPosX=0, int nPosY=0);
	void GetWndPos(int &nPosX, int &nPosY);

	int SyncMsgBox(CString sMsg, int nThreadIdx=0, int nType=MB_OK, int nTimOut=DEFAULT_TIME_OUT);
	int AsyncMsgBox(CString sMsg, int nThreadIdx=1, int nType=MB_OK, int nTimOut=DEFAULT_TIME_OUT);

// Dialog Data
	//{{AFX_DATA(CDlgMyMsg)
	enum { IDD = IDD_DLG_MY_MSG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMyMsg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMyMsg)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);// (UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMYMSG_H__6BF6D8B8_DB6A_4786_B09B_8B30449FBF40__INCLUDED_)

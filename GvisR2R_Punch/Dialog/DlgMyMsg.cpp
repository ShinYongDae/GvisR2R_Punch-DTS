// DlgMyMsg.cpp : implementation file
//

#include "stdafx.h"
#include "../GvisR2R_Punch.h"
#include "DlgMyMsg.h"

#include "DlgMyMsgSub00.h"
#include "DlgMyMsgSub01.h"
#include "DlgMyMsgSub02.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMyMsg dialog


CDlgMyMsg::CDlgMyMsg(CWnd* pParent /*=NULL*/, int nIDD)
	: CDialog(nIDD, pParent)//: CDialog(CDlgMyMsg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMyMsg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pParent = pParent;
	if (pParent)
		m_hParentWnd = pParent->GetSafeHwnd();

	m_pRect = NULL;

	m_pDlgMyMsg = NULL;

	m_nRtnVal = 0;
	m_bTimDlgMyMsg = FALSE;
	m_bSetMsg = FALSE;
	m_bCloseMsg = FALSE;

	m_nPosX = 0;
	m_nPosY = 0;

	if(m_ArrayMyMsgBox.GetSize() > 0)
		m_ArrayMyMsgBox.RemoveAll();
	if(m_ArrayMyMsgBoxSync.GetSize() > 0)
		m_ArrayMyMsgBoxSync.RemoveAll();
	if(m_ArrayMyMsgBoxAsync.GetSize() > 0)
		m_ArrayMyMsgBoxAsync.RemoveAll();
	if(m_ArrayMyMsgBoxRtn.GetSize() > 0)
		m_ArrayMyMsgBoxRtn.RemoveAll();
	if(m_ArrayMyMsgBoxRtnWait.GetSize() > 0)
		m_ArrayMyMsgBoxRtnWait.RemoveAll();
}

CDlgMyMsg::~CDlgMyMsg()
{
	StopTimer();

	if(m_pDlgMyMsg) 
	{
		m_pDlgMyMsg->DestroyWindow();
		delete m_pDlgMyMsg;
		m_pDlgMyMsg = NULL;		
	}

	if(m_ArrayMyMsgBox.GetSize() > 0)
		m_ArrayMyMsgBox.RemoveAll();
	if(m_ArrayMyMsgBoxSync.GetSize() > 0)
		m_ArrayMyMsgBoxSync.RemoveAll();
	if(m_ArrayMyMsgBoxAsync.GetSize() > 0)
		m_ArrayMyMsgBoxAsync.RemoveAll();
	if(m_ArrayMyMsgBoxRtn.GetSize() > 0)
		m_ArrayMyMsgBoxRtn.RemoveAll();
	if(m_ArrayMyMsgBoxRtnWait.GetSize() > 0)
		m_ArrayMyMsgBoxRtnWait.RemoveAll();

	if(m_pRect)
	{
		delete m_pRect;
		m_pRect = NULL;
	}
}

void CDlgMyMsg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMyMsg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMyMsg, CDialog)
	//{{AFX_MSG_MAP(CDlgMyMsg)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MYMSG_EXIT, OnMyMsgExit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMyMsg message handlers

BOOL CDlgMyMsg::Create()
{
	return CDialog::Create(CDlgMyMsg::IDD);
	//return FALSE;
}

BOOL CDlgMyMsg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if(!m_bTimDlgMyMsg)
	{
		m_bTimDlgMyMsg = TRUE;
		SetTimer(0, 100, NULL);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMyMsg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
}

LRESULT CDlgMyMsg::OnMyMsgExit(WPARAM wPara, LPARAM lPara)
{
 	AtDlgHide();
	//::PostMessage(m_hParentWnd, WM_MYMSG_EXIT, (WPARAM)NULL, (LPARAM)NULL);

	if(m_pDlgMyMsg)
	{
		if(m_pDlgMyMsg->IsWindowVisible())
			m_pDlgMyMsg->ShowWindow(SW_HIDE);
	}

// 	if(m_pDlgMyMsg) 
// 	{
// 		m_pDlgMyMsg->DestroyWindow();
// 		delete m_pDlgMyMsg;
// 		m_pDlgMyMsg = NULL;		
// 	}

	int nCount = m_ArrayMyMsgBoxAsync.GetSize();
	if(nCount > 0)
	{
		m_ArrayMyMsgBoxAsync.RemoveAt(0);
		Sleep(100);
	}

	return 0L;
}

void CDlgMyMsg::StopTimer()
{
	m_bTimDlgMyMsg = FALSE;
	m_bCloseMsg = TRUE;
}

void CDlgMyMsg::OnTimer(UINT_PTR nIDEvent)//(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(0 == nIDEvent)
	{
		KillTimer(0);

		DispMsgFromTimer();
		if(m_bTimDlgMyMsg)
			SetTimer(0, 100, NULL);
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CDlgMyMsg::AtDlgShow()
{
}

void CDlgMyMsg::AtDlgHide()
{
}

void CDlgMyMsg::SetMsg(CString strMsg, int nThreadIdx, int nType, int nTimeOut)
{

	m_bSetMsg = TRUE;

	m_csSetMsg.Lock();

	CString sLocalMsg = strMsg;
	int nLocalThreadIdx = nThreadIdx;
	int nLocalType = nType;
	int nLocalTimeOut = nTimeOut;

	stMyMsgBox stData(sLocalMsg, nLocalThreadIdx, nLocalType, nLocalTimeOut); 
	m_ArrayMyMsgBox.Add(stData);
	
	m_csSetMsg.Unlock();

	m_bSetMsg = FALSE;

}

CString CDlgMyMsg::GetMsg()
{
	int nCount = m_ArrayMyMsgBox.GetSize();
	if(nCount<=0)
		return _T("");
	
	stMyMsgBox stDispMsg;
	stDispMsg = m_ArrayMyMsgBox.GetAt(0);
	CString sMsg = stDispMsg.strMsg;

	return sMsg;
}

int CDlgMyMsg::GetTimeOut()
{
	int nCount = m_ArrayMyMsgBox.GetSize();
	if(nCount<=0)
		return 0;
	
	stMyMsgBox stDispMsg;
	stDispMsg = m_ArrayMyMsgBox.GetAt(0);
	int nTimeOut = stDispMsg.nTimOut;

	return nTimeOut;
}

void CDlgMyMsg::SetRtnVal(int nVal)
{
	m_ArrayMyMsgBoxRtn.Add(nVal);
}

void CDlgMyMsg::GetWndPos(int &nPosX, int &nPosY)
{
	nPosX = m_nPosX;
	nPosY = m_nPosY;
}

void CDlgMyMsg::SetWndPos(int nPosX, int nPosY)
{
// 	m_nPosX = nPosX;
// 	m_nPosY = nPosY;
}

int CDlgMyMsg::GetWaitRtnVal(int nThreadIdx)
{
	int nRtn = -1;
	int nCount = m_ArrayMyMsgBoxRtnWait.GetSize();
	if(nCount > 0)
	{
 		stMyMsgBoxRtnWait stMsgRtnWait;
		for(int i=0; i<nCount; i++)
		{
			stMsgRtnWait = m_ArrayMyMsgBoxRtnWait.GetAt(i);
			int ThreadIdx = stMsgRtnWait.nThreadIdx;
			if(nThreadIdx == ThreadIdx)
			{
				nRtn = stMsgRtnWait.nRtnVal;
				m_ArrayMyMsgBoxRtnWait.RemoveAt(i);

				return nRtn;
			}
		}
	}

	return nRtn; // m_nRtnVal;
}

int CDlgMyMsg::GetRtnVal(int nThreadIdx)
{
	int i = 0;
	int nRtn = -1;
	int nCount = m_ArrayMyMsgBoxRtn.GetSize();
	if(nCount > 0)
	{
 		stMyMsgBox stDispMsg;
		for(i=0; i<nCount; i++)
		{
			stDispMsg = m_ArrayMyMsgBox.GetAt(i);
			CString sMsg = stDispMsg.strMsg;
			int MsgId = stDispMsg.nThreadIdx;
			if(nThreadIdx == MsgId)
			{
				if(i==0)
				{
					nRtn = m_ArrayMyMsgBoxRtn.GetAt(0);
					m_ArrayMyMsgBoxRtn.RemoveAt(0);

					stMyMsgBoxRtnWait stRtnWait(sMsg, nThreadIdx, nRtn);
					m_ArrayMyMsgBoxRtnWait.Add(stRtnWait);

					break;
				}
			}
		}

		if(i>=nCount)
			return nRtn;
	}

	return nRtn; // m_nRtnVal;
}


int CDlgMyMsg::WaitRtnVal(int nThreadIdx)
{
	int nCount = m_ArrayMyMsgBoxAsync.GetSize();

	int nRtn = -1;
	if(nCount > 0)
	{
 		stMyMsgBox stDispMsg;
		stDispMsg = m_ArrayMyMsgBoxAsync.GetAt(0);

		int ThreadIdx = stDispMsg.nThreadIdx;
		int Type = stDispMsg.nType;
		if(MB_YESNO == Type || MB_OK == Type)
		{
			MSG message;
			while((m_nThreadIdBuf[nThreadIdx] = GetWaitRtnVal(nThreadIdx)) < 0)
			{
				Sleep(100);
				if(::PeekMessage(&message,NULL,0,0,PM_REMOVE))
				{
					::TranslateMessage(&message);
					::DispatchMessage(&message);
				}
			}

			nCount = m_ArrayMyMsgBoxAsync.GetSize();
			if(nCount > 0)
				m_ArrayMyMsgBoxAsync.RemoveAt(0);

			Sleep(100);

			nRtn = m_nThreadIdBuf[nThreadIdx];
		}
	}
	return nRtn;
}

int CDlgMyMsg::DispMsg(CString sMsg, int nThreadIdx, int nType, int nTimeOut)
{
	int nRtnVal = 0;	
	SetMsg(sMsg, nThreadIdx, nType, nTimeOut);

	return nRtnVal;
}

BOOL CDlgMyMsg::DispMsgFromTimer()
{
	if(m_bSetMsg)
		return TRUE;

	int nRtn = -1;
	int nCount0 = m_ArrayMyMsgBox.GetSize();
	int nCount1 = m_ArrayMyMsgBoxAsync.GetSize();
	int nCount2 = m_ArrayMyMsgBoxSync.GetSize();

	if(nCount0 > 0 && nCount1 < 1 && nCount2 < 1)
	{
		stMyMsgBox stDispMsg;

		m_csSetMsg.Lock();
		Sleep(100);
		stDispMsg = m_ArrayMyMsgBox.GetAt(0);
		m_ArrayMyMsgBox.RemoveAt(0);
		m_ArrayMyMsgBoxAsync.Add(stDispMsg);
 
		CString sLocalMsg = stDispMsg.strMsg;
		int nLocalThreadIdx = stDispMsg.nThreadIdx;
		int nLocalType = stDispMsg.nType;
		int nLocalTimeOut = stDispMsg.nTimOut;
		m_csSetMsg.Unlock();

 		nRtn = DoDispMsg(sLocalMsg, nLocalThreadIdx, nLocalType, nLocalTimeOut);//m_nRtnVal
	
		if(nCount0 > 1)
			return TRUE;
	}

	return FALSE;
}

void CDlgMyMsg::RemoveMsg()
{
	int nRtn = -1;
	int nCount = m_ArrayMyMsgBox.GetSize();
	if(nCount > 0)
	{
		m_csSetMsg.Lock();
		m_ArrayMyMsgBox.RemoveAt(0);
		m_csSetMsg.Unlock();
	}
}

int CDlgMyMsg::DoDispMsg(CString sMsg, int nThreadIdx, int nType, int nTimeOut)
{
	if(!m_pParent)
		return -1;
	else if(!m_pParent->GetSafeHwnd())
		return -1;

	m_csDispMsg.Lock();

	CString sLocalMsg = sMsg;
	int nLocalThreadIdx = nThreadIdx;
	int nLocalType = nType;
	int nLocalTimeOut = nTimeOut;

	int nRtnVal = -1; // Reply(-1) is None.
	CDlgMyMsg* pDlgMyMsg=NULL;
	CDlgMyMsgSub00* pDlgMyMsgSub00=NULL;
	CDlgMyMsgSub01* pDlgMyMsgSub01=NULL;
	CDlgMyMsgSub02* pDlgMyMsgSub02=NULL;

	if(MB_TIME_OUT == nType)
	{
		if(m_pDlgMyMsg)
		{
			m_pDlgMyMsg->SetMsg(sLocalMsg, nLocalThreadIdx, nLocalType, nLocalTimeOut);
			if(!m_pDlgMyMsg->IsWindowVisible())
				m_pDlgMyMsg->ShowWindow(SW_SHOW);
			m_pDlgMyMsg->RedrawWindow();
			Sleep(100);
		}
		else
		{
			m_pDlgMyMsg = new CDlgMyMsgSub00(this); 

			m_pDlgMyMsg->SetMsg(sLocalMsg, nLocalThreadIdx, nLocalType, nLocalTimeOut);
			m_pDlgMyMsg->Create();
			if(!m_pDlgMyMsg->IsWindowVisible())
				m_pDlgMyMsg->ShowWindow(SW_SHOW);
			m_pDlgMyMsg->RedrawWindow();
		}
		
		m_csDispMsg.Unlock();
		return nRtnVal;
	}
	else if(MB_YESNO == nType)
	{
		pDlgMyMsgSub01 = new CDlgMyMsgSub01(this);
		pDlgMyMsg = (CDlgMyMsg*)pDlgMyMsgSub01;
	}
	else if(MB_OK == nType)
	{
		pDlgMyMsgSub02 = new CDlgMyMsgSub02(this);
		pDlgMyMsg = (CDlgMyMsg*)pDlgMyMsgSub02;
	}

	if(pDlgMyMsg)
	{
		pDlgMyMsg->SetMsg(sLocalMsg, nLocalThreadIdx, nLocalType, nLocalTimeOut);
		pDlgMyMsg->DoModal();

		nRtnVal = pDlgMyMsg->GetRtnVal(nThreadIdx);

		delete pDlgMyMsg;
		pDlgMyMsg = NULL;
	
		stMyMsgBoxRtnWait stRtnWait(sLocalMsg, nLocalThreadIdx, nRtnVal);
		m_ArrayMyMsgBoxRtnWait.Add(stRtnWait);

 		Sleep(100);
		::PostMessage(m_hParentWnd, WM_MYMSG_EXIT, (WPARAM)NULL, (LPARAM)NULL);
	}

	m_csDispMsg.Unlock();
	return nRtnVal;
}


int CDlgMyMsg::AsyncMsgBox(CString sMsg, int nThreadIdx, int nType, int nTimOut)
{
	int nRtn=-1;

	SetMsg(sMsg, nThreadIdx, nType, nTimOut);

	if(nType==MB_OK || nType==MB_YESNO)
		nRtn = WaitRtnVal(nThreadIdx);

	return nRtn;
}

int CDlgMyMsg::SyncMsgBox(CString sMsg, int nThreadIdx, int nType, int nTimOut)
{
	int nRtnVal = -1; // Reply(-1) is None.
	CDlgMyMsg* pDlgMyMsg=NULL;
	CDlgMyMsgSub00* pDlgMyMsgSub00=NULL;
	CDlgMyMsgSub01* pDlgMyMsgSub01=NULL;
	CDlgMyMsgSub02* pDlgMyMsgSub02=NULL;

	if(MB_TIME_OUT == nType)
	{
		if(m_pDlgMyMsg)
		{
			m_pDlgMyMsg->SetMsg(sMsg, nThreadIdx, nType, nTimOut);
			if(!m_pDlgMyMsg->IsWindowVisible())
				m_pDlgMyMsg->ShowWindow(SW_SHOW);
			m_pDlgMyMsg->RedrawWindow();
		}
		else
		{
			m_pDlgMyMsg = new CDlgMyMsgSub00(this);

			m_pDlgMyMsg->SetWndPos(m_nPosX, m_nPosY);
			m_pDlgMyMsg->SetMsg(sMsg, nThreadIdx, nType, nTimOut);
			m_pDlgMyMsg->Create();
			if(!m_pDlgMyMsg->IsWindowVisible())
				m_pDlgMyMsg->ShowWindow(SW_SHOW);
			m_pDlgMyMsg->RedrawWindow();
		}
		
		return nRtnVal;
	}
	else if(MB_YESNO == nType)
	{
		pDlgMyMsgSub01 = new CDlgMyMsgSub01(this);
		pDlgMyMsg = (CDlgMyMsg*)pDlgMyMsgSub01;
	}
	else if(MB_OK == nType)
	{
		pDlgMyMsgSub02 = new CDlgMyMsgSub02(this);
		pDlgMyMsg = (CDlgMyMsg*)pDlgMyMsgSub02;
	}

	stMyMsgBox stData(sMsg, nThreadIdx, nType, nTimOut); 
	m_ArrayMyMsgBoxSync.Add(stData);

	pDlgMyMsg->SetMsg(sMsg, nThreadIdx, nType, nTimOut);
	pDlgMyMsg->DoModal();

	nRtnVal = pDlgMyMsg->GetRtnVal(nThreadIdx);

	delete pDlgMyMsg;
	pDlgMyMsg = NULL;

	int nCount = m_ArrayMyMsgBoxSync.GetSize();
	if(nCount > 0)
		m_ArrayMyMsgBoxSync.RemoveAt(nCount-1);

	::PostMessage(m_hParentWnd, WM_MYMSG_EXIT, (WPARAM)NULL, (LPARAM)NULL);

	return nRtnVal;
}

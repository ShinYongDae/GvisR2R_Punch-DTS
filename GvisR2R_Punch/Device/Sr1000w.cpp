#include "stdafx.h"
#include "Sr1000w.h"

#include "SR1000WDEF.h"
#include "../Global/GlobalDefine.h"
#include "../GvisR2R_PunchDoc.h"
#include "../GvisR2R_PunchView.h"

extern CGvisR2R_PunchDoc* pDoc;
extern CGvisR2R_PunchView* pView;

CSr1000w::CSr1000w(CString sAddrCli, CString sAddrSvr, CString sPortSvr, CWnd* pParent /*=NULL*/)
{
	if (pParent)
		m_hParentWnd = pParent->GetSafeHwnd();
	//m_strPortSvr = _T("2000");
	//m_strAddrSvr = _T("");
	//m_strAddrCli = _T("");
	m_pClient = NULL;
	m_nServerID = ID_SR1000W;
	m_bWaitForResponse = FALSE;
	m_strResponse = _T("");
	m_nCmd = SrNone;
	m_bCmdF = FALSE;

	RECT rt = { 0,0,0,0 };
	Create(NULL, _T("None"), WS_CHILD, rt, FromHandle(m_hParentWnd), (UINT)this);

	StartClient(sAddrCli, sAddrSvr, sPortSvr);
	Sleep(10);

	m_pThread = NULL;
	//StartThread();
}


CSr1000w::~CSr1000w()
{
	if (m_pClient)
	{
		StopClient();
	}
}

BEGIN_MESSAGE_MAP(CSr1000w, CWnd)
	//{{AFX_MSG_MAP(CSr1000w)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_MESSAGE(WM_CLIENT_RECEIVED, wmClientReceived)
	ON_MESSAGE(WM_CLIENT_CLOSED, wmClientClosed)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CSr1000w::SetHwnd(HWND hParentWnd)
{
	m_hParentWnd = hParentWnd;
}

void CSr1000w::StartClient(CString sAddrCli, CString sAddrSvr, CString sPortSvr)
{
	if (!m_pClient)
	{
		m_strAddrCli = sAddrCli;
		m_strAddrSvr = sAddrSvr;
		m_strPortSvr = sPortSvr;

		m_pClient = new CTcpIpClient(this);
		m_pClient->Init(sAddrCli, sAddrSvr, _tstoi(sPortSvr));
		m_pClient->SetServer(m_nServerID);
		m_pClient->Start();
	}
}

void CSr1000w::StopClient()
{
	if (m_pClient)
	{
		//if (!m_pClient->Stop()) // Called Destroy Function.
		//{
		//	Sleep(30);
		//	delete m_pClient;
		//}
		m_pClient->Stop();
		Sleep(10);
		m_pClient->StopThread();
		Sleep(10);
		delete m_pClient;
		m_pClient = NULL;
		Sleep(10);
	}
}

LRESULT CSr1000w::wmClientClosed(WPARAM wParam, LPARAM lParam)
{
	int nServerID = (int)wParam;

	switch (nServerID)
	{
	case ID_SR1000W:
		//if (m_pClient)
		//	m_pClient = NULL;
		break;
	}

	return (LRESULT)1;
}


LRESULT CSr1000w::wmClientReceived(WPARAM wParam, LPARAM lParam)
{
	return (LRESULT)OnClientReceived(wParam, lParam);
}

int CSr1000w::OnClientReceived(WPARAM wParam, LPARAM lParam)
{
	int nServerID = (int)wParam;
	CString sReceived = (LPCTSTR)lParam;

	if (sReceived == _T("Fail to connect."))
		return 1;

	switch (nServerID)
	{
	case ID_SR1000W:
		if (sReceived.GetLength() > 0)
		{
			if (CheckResponse(m_nCmd, sReceived))
			{
				;
			}
			else
			{
				pView->MsgBox(_T("Error-CheckResponse"));
				//AfxMessageBox(_T("Error-CheckResponse"));
			}
		}
		break;
	}

	return 1;
}

BOOL CSr1000w::CheckResponse(int nCmd, CString sResponse)
{
	int nPos;
	CString sParsing;

	switch (nCmd)
	{
	case SrTriggerInputOn:
		if (nPos = sResponse.Find(_T("ERROR")) > 0)
		{
			pView->MsgBox(_T("Error-Mdx response"));
			//AfxMessageBox(_T("Error-Mdx response"));
			m_strResponse = sResponse;
			m_bWaitForResponse = FALSE;
		}
		else
		{
			m_strResponse = sResponse;
			m_bWaitForResponse = FALSE;
			if (m_hParentWnd)
				::PostMessage(m_hParentWnd, WM_CLIENT_RECEIVED_SR, (WPARAM)SrTriggerInputOn, (LPARAM)&m_strResponse); // "OrderNum-ShotNum" (9bytes'-'3bytes)
		}
		return TRUE;
	}

	return FALSE;
}


BOOL CSr1000w::IsRunning()
{
	return (m_bWaitForResponse);
}

BOOL CSr1000w::DoRead2DCode()
{
	if (m_bWaitForResponse)
	{
		pView->MsgBox(_T("Error - m_bWaitForResponse[ID_SR1000W] is TRUE."));
		//AfxMessageBox(_T("Error - m_bWaitForResponse[ID_SR1000W] is TRUE."));
		return FALSE;
	}

	//if (m_pClient)
	//{
	//	m_nCmd = SrTriggerInputOn;	// _T("LON");
	//	m_bWaitForResponse = TRUE;
	//	return TRUE;
	//}

	if (m_pClient)
	{
		m_bWaitForResponse = TRUE;
		m_nCmd = SrTriggerInputOn;	// _T("LON");
		CString sMsg = _T(SR_TriggerInputOn);	// _T("LON");
		if (WriteComm(sMsg) > 0)
		{
			m_strResponse = _T("");
			return TRUE;
		}
		else
		{
			m_nCmd = SrNone;
			m_bWaitForResponse = FALSE;
		}
	}

	return FALSE;
}

BOOL CSr1000w::Get2DCode(CString &sData)
{
	if (!m_bWaitForResponse)
	{
		sData = m_strResponse;
		return TRUE;
	}

	return FALSE;
}

int CSr1000w::WriteComm(CString sMsg, DWORD dwTimeout)
{
	return m_pClient->WriteComm(sMsg);
}

// Thread
void CSr1000w::StartThread() // Worker Thread 备悼包访 Step8
{
	if (m_pThread == NULL)
	{
		m_bModify = TRUE;
		m_evtThread.ResetEvent();
		m_pThread = AfxBeginThread(RunThread, this);
		if (m_pThread)
			m_hThread = m_pThread->m_hThread;
	}
}

void CSr1000w::StopThread() // Worker Thread 备悼包访 Step9
{
	if (m_pThread != NULL)
	{
		m_evtThread.SetEvent();
		WaitUntilThreadEnd(m_hThread);
	}
	m_pThread = NULL;
	m_bModify = FALSE;
}

void CSr1000w::WaitUntilThreadEnd(HANDLE hThread) // Worker Thread 备悼包访 Step6
{
	TRACE("WaitUntilThreadEnd(0x%08x:RunThread) Entering\n", hThread);
	MSG message;
	const DWORD dwTimeOut = 500000;
	DWORD dwStartTick = GetTickCount();
	DWORD dwExitCode;
	while (GetExitCodeThread(hThread, &dwExitCode) && dwExitCode == STILL_ACTIVE && m_bAlive) {
		// Time Out Check
		if (GetTickCount() >= (dwStartTick + dwTimeOut))
		{
			//pView->MsgBox("WaitUntilThreadEnd() Time Out!!!");
			AfxMessageBox(_T("WaitUntilThreadEnd() Time Out!!!", NULL, MB_OK | MB_ICONSTOP));
			return;
		}
		if (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&message);
			::DispatchMessage(&message);
		}
	}
	TRACE("WaitUntilThreadEnd(0x%08x:DispMsgThread) Exit\n", hThread);
}

UINT CSr1000w::RunThread(LPVOID pParam)
{
	CSr1000w* pSr = (CSr1000w*)pParam;
	DWORD dwMilliseconds = 10;
	pSr->m_bAlive = TRUE;

	MSG message;
	CString sMsg;

	while (WAIT_OBJECT_0 != ::WaitForSingleObject(pSr->m_evtThread, dwMilliseconds))
	{
		Sleep(100);
		//if (pSr->m_bWaitForResponse && !pSr->m_bCmdF)
		//{
		//	pSr->m_bCmdF = TRUE;

		//	switch (pSr->m_nCmd)
		//	{
		//	case SrTriggerInputOn:
		//		sMsg = _T(SR_TriggerInputOn);	// _T("LON");
		//		if (pSr->WriteComm(sMsg) > 0)
		//		{
		//			pSr->m_strResponse = _T("");
		//		}
		//		else
		//		{
		//			pSr->m_nCmd = SrNone;
		//			pSr->m_bWaitForResponse = FALSE;
		//		}
		//		break;
		//	}
		//}
		//else if (!pSr->m_bWaitForResponse && pSr->m_bCmdF)
		//{
		//	pSr->m_bCmdF = FALSE;
		//}
	}

	pSr->m_bAlive = FALSE;
	return 0;
}

void CSr1000w::Close()
{
	StopThread();
	Sleep(10);
}

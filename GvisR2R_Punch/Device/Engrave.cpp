#include "stdafx.h"
#include "Engrave.h"

#include "EngraveDef.h"
#include "../Global/GlobalDefine.h"
#include "../GvisR2R_PunchDoc.h"
#include "../GvisR2R_PunchView.h"

extern CGvisR2R_PunchDoc* pDoc;
extern CGvisR2R_PunchView* pView;

BEGIN_MESSAGE_MAP(CEngrave, CWnd)
	//{{AFX_MSG_MAP(CEngrave)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_MESSAGE(WM_UPDATE_CONNECTION, OnUpdateConnection)
	ON_WM_TIMER()
	ON_MESSAGE(WM_CLIENT_RECEIVED, wmClientReceived)
	ON_MESSAGE(WM_CLIENT_CLOSED, wmClientClosed)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CEngrave::CEngrave(CString sAddrCli, CString sAddrSvr, CString sPortSvr, CWnd* pParent /*=NULL*/)
{
	if(pParent)
		m_hParentWnd = pParent->GetSafeHwnd();

	m_pClient = NULL;
	m_nServerID = ID_ENGRAVE;
	m_bWaitForResponse = FALSE;
	m_strResponse = _T("");
	m_nCmd = _None_;
	m_bCmdF = FALSE;
	m_bAutoConnect = TRUE;

	RECT rt = { 0,0,0,0 };
	Create(NULL, _T("None"), WS_CHILD, rt, FromHandle(m_hParentWnd), (UINT)this);

	StartClient(sAddrCli, sAddrSvr, sPortSvr);
	Sleep(10);

	m_pThread = NULL;
	m_bTIM_CHECK_CONNECT = FALSE;
}


CEngrave::~CEngrave()
{
	m_bTIM_CHECK_CONNECT = FALSE;

	if (m_pClient)
	{
		m_bAutoConnect = FALSE;
		StopClient();
	}
}


///////////////////////////////////////////////////////////////////////////////
// OnUpdateConnection
// This message is sent by server manager to indicate connection status
LRESULT CEngrave::OnUpdateConnection(WPARAM wParam, LPARAM lParam)
{
	UINT uEvent = (UINT)wParam;
	CEngrave* pClient = reinterpret_cast<CEngrave*>(lParam);

	if (pClient != NULL)
	{
		// Server socket is now connected, we need to pick a new one
		if (uEvent == EVT_CONSUCCESS)
		{
			//pGlobalDoc->m_bConnect = TRUE;
			m_bWaitForResponse = FALSE;
			m_bTIM_CHECK_CONNECT = TRUE;
			SetTimer(TIM_CHECK_CONNECT, DELAY_CHECK_CONNECT, NULL);
		}
		else if (uEvent == EVT_CONFAILURE || uEvent == EVT_CONDROP)
		{
			// Disconnect socket
			StopClient();
			// 자동접속시도
			if (uEvent == EVT_CONDROP && m_bAutoConnect)
				SetTimer(TIM_CONNECT, 9000, NULL);
		}
	}

	return 1L;
}


void CEngrave::SetHwnd(HWND hParentWnd)
{
	m_hParentWnd = hParentWnd;
}

void CEngrave::StartClient(CString sAddrCli, CString sAddrSvr, CString sPortSvr)
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

void CEngrave::StopClient()
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

LRESULT CEngrave::wmClientClosed(WPARAM wParam, LPARAM lParam)
{
	int nServerID = (int)wParam;

	switch (nServerID)
	{
	case ID_ENGRAVE:
		if (m_pClient)
			m_pClient = NULL;
		break;
	}

	return (LRESULT)1;
}


SOCKET_DATA CEngrave::GetSocketData()
{
	return m_SocketData;
}

LRESULT CEngrave::wmClientReceived(WPARAM wParam, LPARAM lParam)
{
	int nServerID = (int)wParam;

	if (nServerID == ID_ENGRAVE)
	{
		m_SocketData = m_pClient->GetSocketData();
	}


	return (LRESULT)OnClientReceived(wParam, lParam);
}

int CEngrave::OnClientReceived(WPARAM wParam, LPARAM lParam)
{
	int nServerID = (int)wParam;
	//CString sReceived = (LPCTSTR)lParam;

	//if (sReceived == _T("Fail to connect."))
	//	return 1;

	if (m_hParentWnd)
		::PostMessage(m_hParentWnd, WM_CLIENT_RECEIVED, (WPARAM)ID_ENGRAVE, (LPARAM)&m_SocketData); // "OrderNum-ShotNum" (9bytes'-'3bytes)
	m_bWaitForResponse = FALSE;
/*
	switch (nServerID)
	{
	case ID_ENGRAVE:
		if (m_hParentWnd)
			::PostMessage(m_hParentWnd, WM_CLIENT_RECEIVED, (WPARAM)ID_ENGRAVE, (LPARAM)&m_SocketData); // "OrderNum-ShotNum" (9bytes'-'3bytes)
		m_bWaitForResponse = FALSE;
		//if (sReceived.GetLength() > 0)
		//{
		//	if (CheckResponse(m_nCmd, sReceived))
		//	{
		//		;
		//	}
		//	else
		//	{
		//		pView->MsgBox(_T("Error-CheckResponse"));
		//		//AfxMessageBox(_T("Error-CheckResponse"));
		//	}
		//}
		break;
	}
*/
	return 1;
}

BOOL CEngrave::CheckResponse(int nCmd, CString sResponse)
{
	int nPos;
	CString sParsing;

	switch (nCmd)
	{
	//case SrTriggerInputOn:
	//	if (nPos = sResponse.Find(_T("ERROR")) > 0)
	//	{
	//		pView->MsgBox(_T("Error-Mdx response"));
	//		//AfxMessageBox(_T("Error-Mdx response"));
	//		m_strResponse = sResponse;
	//		m_bWaitForResponse = FALSE;
	//	}
	//	else
	//	{
	//		m_strResponse = sResponse;
	//		m_bWaitForResponse = FALSE;
	//		if (m_hParentWnd)
	//			::PostMessage(m_hParentWnd, WM_CLIENT_RECEIVED_SR, (WPARAM)SrTriggerInputOn, (LPARAM)&m_strResponse); // "OrderNum-ShotNum" (9bytes'-'3bytes)
	//	}
	//	return TRUE;
		;
	}

	return FALSE;
}


BOOL CEngrave::IsRunning()
{
	return (m_bWaitForResponse);
}

BOOL CEngrave::ReadComm(CString &sData)
{
	if (!m_bWaitForResponse)
	{
		sData = m_strResponse;
		return TRUE;
	}

	return FALSE;
}

int CEngrave::WriteComm(CString sMsg, DWORD dwTimeout)
{
	return m_pClient->WriteComm(sMsg);
}


// Thread
void CEngrave::StartThread() // Worker Thread 구동관련 Step8
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

void CEngrave::StopThread() // Worker Thread 구동관련 Step9
{
	if (m_pThread != NULL)
	{
		m_evtThread.SetEvent();
		WaitUntilThreadEnd(m_hThread);
	}
	m_pThread = NULL;
	m_bModify = FALSE;
}

void CEngrave::WaitUntilThreadEnd(HANDLE hThread) // Worker Thread 구동관련 Step6
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

UINT CEngrave::RunThread(LPVOID pParam)
{
	CEngrave* pMk = (CEngrave*)pParam;
	DWORD dwMilliseconds = 10;
	pMk->m_bAlive = TRUE;

	MSG message;
	CString sMsg;

	while (WAIT_OBJECT_0 != ::WaitForSingleObject(pMk->m_evtThread, dwMilliseconds))
	{
		Sleep(100);
	}

	pMk->m_bAlive = FALSE;
	return 0;
}

void CEngrave::Close()
{
	StopThread();
	Sleep(10);
}


// General Function

BOOL CEngrave::SendCommand(SOCKET_DATA SocketData, BOOL bWait)
{
	if (!m_pClient)
		return FALSE;

	BOOL bRtn = TRUE;
	SocketData.nTxPC = _Punch;		// Client
	SocketData.nRxPC = _Engrave;	// Server
	m_pClient->WriteCommData(SocketData, INFINITE);

	if (bWait)
	{
		m_bWaitForResponse = TRUE;
		bRtn = WaitResponse();
	}

	return bRtn;
}

BOOL CEngrave::WaitResponse()
{
	MSG message;
	DWORD dwStartTick = GetTickCount();

	while (IsRunning())
	{
		if (GetTickCount() - dwStartTick > DELAY_RESPONSE)
		{
			// Disconnect socket
			//AfxMessageBox(_T("WaitResponse() Time Out!!!"));
			m_pClient->SetConnectedStatus(FALSE);
			return FALSE;
		}

		if (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&message);
			::DispatchMessage(&message);
		}
		Sleep(30);
	}

	return TRUE;
}

BOOL CEngrave::IsConnected()
{
	if (!m_pClient)
		return FALSE;

	return m_pClient->IsConnected();
}

void CEngrave::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	SOCKET_DATA SocketData;

	switch (nIDEvent)
	{
	case TIM_CONNECT:
		KillTimer(TIM_CONNECT);
		SetTimer(TIM_CONNECT + 1, 1000, NULL);
		break;
	case TIM_CONNECT + 1:
		KillTimer(TIM_CONNECT + 1);
		StartClient(m_strAddrCli, m_strAddrSvr, m_strPortSvr);
		SetTimer(TIM_CONNECT + 2, 1000, NULL);
		break;
	case TIM_CONNECT + 2:
		KillTimer(TIM_CONNECT + 2);
		if(!IsConnected())
			SetTimer(TIM_CONNECT + 1, 1000, NULL);
		break;
	case TIM_CHECK_CONNECT:
		KillTimer(TIM_CHECK_CONNECT);
		if (m_pClient && m_bTIM_CHECK_CONNECT)
		{
			if(!IsRunning())
			{
				SocketData.nCmdCode = _GetSig;
				SocketData.nMsgID = _Connect;
				SendCommand(SocketData);
			}
			if (m_bTIM_CHECK_CONNECT)
				SetTimer(TIM_CHECK_CONNECT, DELAY_CHECK_CONNECT, NULL);
		}
		break;
	}
	CWnd::OnTimer(nIDEvent);
}

// Communcation

void CEngrave::GetSysSignal(SOCKET_DATA SockData)
{
	GetSignalMain(SockData);
	GetSignalTorqueMotor(SockData);
	GetSignalInductionMotor(SockData);
	GetSignalCore150mm(SockData);
	GetSignalEtc(SockData);
	GetSignalRecoiler(SockData);
	GetSignalPunch(SockData);
	GetSignalAOIDn(SockData);
	GetSignalAOIUp(SockData);
	GetSignalEngrave(SockData);
	GetSignalUncoiler(SockData);
}

void CEngrave::GetSignalMain(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	switch (nMsgId)
	{
	case _Ready:
		pDoc->BtnStatus.Main.Ready  = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _Run:
		pDoc->BtnStatus.Main.Run  = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _Reset:
		pDoc->BtnStatus.Main.Reset = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _Stop:
		pDoc->BtnStatus.Main.Stop = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _Auto:
		pDoc->BtnStatus.Main.Auto = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _Manual:
		pDoc->BtnStatus.Main.Manual = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	}
}

void CEngrave::GetSignalTorqueMotor(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	switch (nMsgId)
	{
	case _MkTq:
		pDoc->BtnStatus.Tq.Mk = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _AoiTq:
		pDoc->BtnStatus.Tq.Aoi = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _EngTq:
		pDoc->BtnStatus.Tq.Eng = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	}
}

void CEngrave::GetSignalInductionMotor(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	switch (nMsgId)
	{
	case _CcwModRe:
		pDoc->BtnStatus.Induct.Rc = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _CcwModUn:
		pDoc->BtnStatus.Induct.Uc = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	}
}

void CEngrave::GetSignalCore150mm(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	switch (nMsgId)
	{
	case _Core150Re:
		pDoc->BtnStatus.Core150.Rc = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _Core150Un:
		pDoc->BtnStatus.Core150.Uc = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	}
}

void CEngrave::GetSignalEtc(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	switch (nMsgId)
	{
	case _EmgAoi:
		pDoc->BtnStatus.Etc.EmgAoi = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	}
}

void CEngrave::GetSignalRecoiler(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	switch (nMsgId)
	{
	case _Relation:
		pDoc->BtnStatus.Rc.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _MvCwRe:
		pDoc->BtnStatus.Rc.FdCw = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _MvCcwRe:
		pDoc->BtnStatus.Rc.FdCcw = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _PrdChuckRe:
		pDoc->BtnStatus.Rc.ReelChuck = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _DancerUpRe:
		pDoc->BtnStatus.Rc.DcRlUpDn = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _PasteUpLfRe:
		pDoc->BtnStatus.Rc.ReelJoinL = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _PasteUpRtRe:
		pDoc->BtnStatus.Rc.ReelJoinR = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _PasteVacRe:
		pDoc->BtnStatus.Rc.ReelJoinVac = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _PprChuckRe:
		pDoc->BtnStatus.Rc.PprChuck = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _PprCwRe:
		pDoc->BtnStatus.Rc.PprCw = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _PprCcwRe:
		pDoc->BtnStatus.Rc.PprCcw = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _DoRe:
		pDoc->BtnStatus.Rc.Rewine = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _PrdPprRe:
		pDoc->BtnStatus.Rc.RewineReelPpr = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	}
}

void CEngrave::GetSignalPunch(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	switch (nMsgId)
	{
	case _Relation:
		pDoc->BtnStatus.Mk.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _MvCwMk:
		pDoc->BtnStatus.Mk.FdCw = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _MvCcwMk:
		pDoc->BtnStatus.Mk.FdCcw = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _FdVacMk:
		pDoc->BtnStatus.Mk.FdVac = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _PushUpMk:
		pDoc->BtnStatus.Mk.PushUp = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _TblBlwMk:
		pDoc->BtnStatus.Mk.TblBlw = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _TblVacMk:
		pDoc->BtnStatus.Mk.TblVac = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _FdClampMk:
		pDoc->BtnStatus.Mk.FdClp = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _TensClampMk:
		pDoc->BtnStatus.Mk.TqClp = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _OnePnlMk:
		pDoc->BtnStatus.Mk.MvOne = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _DancerUpMk:
		pDoc->BtnStatus.Mk.DcRSol = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	}
}

void CEngrave::GetSignalAOIDn(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	switch (nMsgId)
	{
	case _Relation:
		pDoc->BtnStatus.AoiDn.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _MvCwAoiDn:
		pDoc->BtnStatus.AoiDn.FdCw = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _MvCcwAoiDn:
		pDoc->BtnStatus.AoiDn.FdCcw = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _FdVacAoiDn:
		pDoc->BtnStatus.AoiDn.FdVac = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _PushUpAoiDn:
		pDoc->BtnStatus.AoiDn.PushUp = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _TblBlwAoiDn:
		pDoc->BtnStatus.AoiDn.TblBlw = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _TblVacAoiDn:
		pDoc->BtnStatus.AoiDn.TblVac = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _FdClampAoiDn:
		pDoc->BtnStatus.AoiDn.FdClp = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _TensClampAoiDn:
		pDoc->BtnStatus.AoiDn.TqClp = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _OnePnlAoiDn:
		pDoc->BtnStatus.AoiDn.MvOne = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _ClrRollAoiDn:
		pDoc->BtnStatus.AoiDn.ClrRoll = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _ClrSonicAoiDn:
		pDoc->BtnStatus.AoiDn.SonicBlw = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _TestAoiDn:
		pDoc->BtnStatus.AoiDn.Test = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _ResetAoiDn:
		pDoc->BtnStatus.AoiDn.Reset = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _LotEndAoiDn:
		pDoc->BtnStatus.AoiDn.LotEnd = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	}
}

void CEngrave::GetSignalAOIUp(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	switch (nMsgId)
	{
	case _Relation:
		pDoc->BtnStatus.AoiUp.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _MvCwAoiUp:
		pDoc->BtnStatus.AoiUp.FdCw = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _MvCcwAoiUp:
		pDoc->BtnStatus.AoiUp.FdCcw = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _FdVacAoiUp:
		pDoc->BtnStatus.AoiUp.FdVac = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _PushUpAoiUp:
		pDoc->BtnStatus.AoiUp.PushUp = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _TblBlwAoiUp:
		pDoc->BtnStatus.AoiUp.TblBlw = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _TblVacAoiUp:
		pDoc->BtnStatus.AoiUp.TblVac = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _FdClampAoiUp:
		pDoc->BtnStatus.AoiUp.FdClp = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _TensClampAoiUp:
		pDoc->BtnStatus.AoiUp.TqClp = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _OnePnlAoiUp:
		pDoc->BtnStatus.AoiUp.MvOne = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _ClrRollAoiUp:
		pDoc->BtnStatus.AoiUp.ClrRoll = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _TestAoiUp:
		pDoc->BtnStatus.AoiUp.Test = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _ResetAoiUp:
		pDoc->BtnStatus.AoiUp.Reset = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _LotEndAoiUp:
		pDoc->BtnStatus.AoiUp.LotEnd = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	}
}

void CEngrave::GetSignalEngrave(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	switch (nMsgId)
	{
	case _Relation:
		pDoc->BtnStatus.Eng.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _MvCwEng:
		pDoc->BtnStatus.Eng.FdCw = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _MvCcwEng:
		pDoc->BtnStatus.Eng.FdCcw = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _FdVacEng:
		pDoc->BtnStatus.Eng.FdVac = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _PushUpEng:
		pDoc->BtnStatus.Eng.PushUp = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _TblBlwEng:
		pDoc->BtnStatus.Eng.TblBlw = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _TblVacEng:
		pDoc->BtnStatus.Eng.TblVac = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _FdClampEng:
		pDoc->BtnStatus.Eng.FdClp = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _TensClampEng:
		pDoc->BtnStatus.Eng.TqClp = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _OnePnlEng:
		pDoc->BtnStatus.Eng.MvOne = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _DancerUpEng:
		pDoc->BtnStatus.Eng.DcRSol = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _ClrSonicEng:
		pDoc->BtnStatus.Eng.SonicBlw = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	}
}

void CEngrave::GetSignalUncoiler(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	switch (nMsgId)
	{
	case _Relation:
		pDoc->BtnStatus.Uc.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _MvCwUn:
		pDoc->BtnStatus.Uc.FdCw = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _MvCcwUn:
		pDoc->BtnStatus.Uc.FdCcw = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _PrdChuckUn:
		pDoc->BtnStatus.Uc.ReelChuck = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _DancerUpUn:
		pDoc->BtnStatus.Uc.DcRlUpDn = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _PasteUpLfUn:
		pDoc->BtnStatus.Uc.ReelJoinL = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _PasteUpRtUn:
		pDoc->BtnStatus.Uc.ReelJoinR = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _PasteVacUn:
		pDoc->BtnStatus.Uc.ReelJoinVac = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _PprChuckUn:
		pDoc->BtnStatus.Uc.PprChuck = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _PprCwUn:
		pDoc->BtnStatus.Uc.PprCw = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _PprCcwUn:
		pDoc->BtnStatus.Uc.PprCcw = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _ClrRollUpUn:
		pDoc->BtnStatus.Uc.ClRlUpDn = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _ClrRollPushUn:
		pDoc->BtnStatus.Uc.ClRlPshUpDn = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	}
}

// Start for GetSysData()

void CEngrave::GetSysData(SOCKET_DATA SockData)
{
	GetOpInfo(SockData);
	GetInfo(SockData);
	GetTotRatio(SockData);
	GetStTime(SockData);
	GetRunTime(SockData);
	GetEdTime(SockData);
	GetStripRatio(SockData);
	GetDef(SockData);
	Get2DReader(SockData);
	GetEngInfo(SockData);
	GetFdInfo(SockData);
	GetAoiInfo(SockData);
	GetMkInfo(SockData);
	GetMkInfoLf(SockData);
	GetMkInfoRt(SockData);
}

void CEngrave::GetOpInfo(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	switch (nMsgId)
	{
	case _OpName:
		pDoc->WorkingInfo.LastJob.sSelUserName = CharToString(SockData.strData);
		break;
	case _DualTest:
		pDoc->WorkingInfo.LastJob.bDualTest = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _SampleTest:
		pDoc->WorkingInfo.LastJob.bSampleTest = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _SampleShotNum:
		sVal.Format(_T("%d"), (int)SockData.nData1);
		pDoc->WorkingInfo.LastJob.sSampleTestShotNum = sVal;
		break;
	case _TestMode:
		pDoc->WorkingInfo.LastJob.nTestMode = (int)SockData.nData1; // MODE_NONE = 0, MODE_INNER = 1, MODE_OUTER = 2
		break;
	case _RecoilerCcw:
		pDoc->WorkingInfo.LastJob.bOneMetal = (SockData.nData1 > 0) ? TRUE : FALSE;	// OneMetal : TRUE -> SetTwoMetal(FALSE);
		break;
	case _UncoilerCcw:
		pDoc->WorkingInfo.LastJob.bTwoMetal = (SockData.nData1 > 0) ? TRUE : FALSE;	// TwoMetal : TRUE -> SetTwoMetal(TRUE);
		break;
	case _AlignMethode:
		pDoc->WorkingInfo.LastJob.nAlignMethode = (int)SockData.nData1; // TWO_POINT, FOUR_POINT
		break;
	case _DoorRecoiler:
		pDoc->WorkingInfo.LastJob.bRclDrSen = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _DoorAoiUp:
		pDoc->WorkingInfo.LastJob.bAoiUpDrSen = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _DoorAoiDn:
		pDoc->WorkingInfo.LastJob.bAoiDnDrSen = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _DoorMk:
		pDoc->WorkingInfo.LastJob.bMkDrSen = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _DoorEngrave:
		pDoc->WorkingInfo.LastJob.bEngvDrSen = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _DoorUncoiler:
		pDoc->WorkingInfo.LastJob.bUclDrSen = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _SaftyMk:
		pDoc->WorkingInfo.LastJob.bMkSftySen = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _CleannerAoiUp:
		pDoc->WorkingInfo.LastJob.bUseAoiUpCleanRoler = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _CleannerAoiDn:
		pDoc->WorkingInfo.LastJob.bUseAoiDnCleanRoler = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _UltraSonicAoiDn:
		pDoc->WorkingInfo.LastJob.bUseAoiDnCleanner = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _UltraSonicEngrave:
		pDoc->WorkingInfo.LastJob.bUseEngraveCleanner = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _TotReelLen:
		pDoc->WorkingInfo.LastJob.sReelTotLen = CharToString(SockData.strData);
		break;
	case _OnePnlLen:
		pDoc->WorkingInfo.Motion.sMkFdDist = CharToString(SockData.strData);
		break;
	case _TempPause:
		pDoc->WorkingInfo.LastJob.bTempPause = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _TempStopLen:
		pDoc->WorkingInfo.LastJob.sTempPauseLen = CharToString(SockData.strData);
		break;
	case _LotCut:
		pDoc->WorkingInfo.LastJob.bLotSep = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _LotCutLen:
		pDoc->WorkingInfo.LastJob.sLotSepLen = CharToString(SockData.strData);
		break;
	case _LotCutPosLen:
		pDoc->WorkingInfo.LastJob.sLotCutPosLen = CharToString(SockData.strData);
		break;
	case _LmtTotYld:
		pDoc->WorkingInfo.LastJob.sLmtTotYld = CharToString(SockData.strData);
		break;
	case _LmtPatlYld:
		pDoc->WorkingInfo.LastJob.sLmtPatlYld = CharToString(SockData.strData);
		break;
	case _StripOutRatio:
		pDoc->WorkingInfo.LastJob.sStripOutRatio = CharToString(SockData.strData);
		break;
	case _CustomNeedRatio:
		pDoc->WorkingInfo.LastJob.sCustomNeedRatio = CharToString(SockData.strData);
		break;
	case _NumRangeFixDef:
		pDoc->WorkingInfo.LastJob.sNumRangeFixDef = CharToString(SockData.strData);
		break;
	case _NumContFixDef:
		pDoc->WorkingInfo.LastJob.sNumContFixDef = CharToString(SockData.strData);
		break;
	case _UltraSonicStTim:
		pDoc->WorkingInfo.LastJob.sUltraSonicCleannerStTim = CharToString(SockData.strData);
		break;
	}
}

void CEngrave::GetInfo(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	switch (nMsgId)
	{
	case _OpName:
		pDoc->WorkingInfo.LastJob.sSelUserName = CharToString(SockData.strData);
		break;
	case _ModelUpName:
		pDoc->WorkingInfo.LastJob.sModelUp = CharToString(SockData.strData);
		break;
	case _ModelDnName:
		pDoc->WorkingInfo.LastJob.sModelDn = CharToString(SockData.strData);
		break;
	case _LotUpName:
		pDoc->WorkingInfo.LastJob.sLotUp = CharToString(SockData.strData);
		break;
	case _LotDnName:
		pDoc->WorkingInfo.LastJob.sLotDn = CharToString(SockData.strData);
		break;
	case _LayerUpName:
		pDoc->WorkingInfo.LastJob.sLayerUp = CharToString(SockData.strData);
		break;
	case _LayerDnName:
		pDoc->WorkingInfo.LastJob.sLayerDn = CharToString(SockData.strData);
		break;
	case _TotReelLen:
		pDoc->WorkingInfo.LastJob.sReelTotLen = CharToString(SockData.strData);
		break;
	case _PartVel:
		pDoc->WorkingInfo.LastJob.sPartialSpd = CharToString(SockData.strData);
		break;
	case _TempPause:
		pDoc->WorkingInfo.LastJob.bTempPause = (SockData.nData1 > 0) ? TRUE : FALSE;
		break;
	case _TempStopLen:
		pDoc->WorkingInfo.LastJob.sTempPauseLen = CharToString(SockData.strData);
		break;
	case _LotCutLen:
		pDoc->WorkingInfo.LastJob.sLotCutPosLen = CharToString(SockData.strData);
		break;
	case _LotSerial:
		pDoc->WorkingInfo.LastJob.sLotSerial = CharToString(SockData.strData);
		break;
	case _MkVerfyLen:
		pDoc->WorkingInfo.LastJob.sVerifyLen = CharToString(SockData.strData);
		break;
	default:
		break;
	}
}

void CEngrave::GetTotRatio(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	switch (nMsgId)
	{
	case _DefNumUp:
		pDoc->m_nBad[0] = SockData.nData1;
		break;
	case _DefRtoUp:
		pDoc->m_dBadRatio[0] = (double)SockData.fData1;
		break;
	case _GoodNumUp:
		pDoc->m_nGood[0] = SockData.nData1;
		break;
	case _GoodRtoUp:
		pDoc->m_dGoodRatio[0] = (double)SockData.fData1;
		break;
	case _TestNumUp:
		pDoc->m_nTestNum[0] = SockData.nData1;
		break;
	case _DefNumDn:
		pDoc->m_nBad[1] = SockData.nData1;
		break;
	case _DefRtoDn:
		pDoc->m_dBadRatio[1] = (double)SockData.fData1;
		break;
	case _GoodNumDn:
		pDoc->m_nGood[1] = SockData.nData1;
		break;
	case _GoodRtoDn:
		pDoc->m_dGoodRatio[1] = (double)SockData.fData1;
		break;
	case _TestNumDn:
		pDoc->m_nTestNum[1] = SockData.nData1;
		break;
	case _DefNumTot:
		pDoc->m_nBad[2] = SockData.nData1;
		break;
	case _DefRtoTot:
		pDoc->m_dBadRatio[2] = (double)SockData.fData1;
		break;
	case _GoodNumTot:
		pDoc->m_nGood[2] = SockData.nData1;
		break;
	case _GoodRtoTot:
		pDoc->m_dGoodRatio[2] = (double)SockData.fData1;
		break;
	case _TestNumTot:
		pDoc->m_nTestNum[2] = SockData.nData1;
		break;
	default:
		break;
	}
}

void CEngrave::GetStTime(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	switch (nMsgId)
	{
	case _LotStTime:
		pDoc->m_sLotStTime = CharToString(SockData.strData);
		break;
	default:
		break;
	}
}

void CEngrave::GetRunTime(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	switch (nMsgId)
	{
	case _LotRunTime:
		pDoc->m_sLotRunTime = CharToString(SockData.strData);
		break;
	default:
		break;
	}
}

void CEngrave::GetEdTime(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	switch (nMsgId)
	{
	case _LotEdTime:
		pDoc->m_sLotEdTime = CharToString(SockData.strData);
		break;
	default:
		break;
	}
}

void CEngrave::GetStripRatio(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	switch (nMsgId)
	{
	case _1LnGoodRtoUp:
		pDoc->m_dStripRatio[0][0] = (double)SockData.fData1;
		break;
	case _2LnGoodRtoUp:
		pDoc->m_dStripRatio[0][1] = (double)SockData.fData1;
		break;
	case _3LnGoodRtoUp:
		pDoc->m_dStripRatio[0][2] = (double)SockData.fData1;
		break;
	case _4LnGoodRtoUp:
		pDoc->m_dStripRatio[0][3] = (double)SockData.fData1;
		break;
	case _AllLnGoodRtoUp:
		pDoc->m_dStripRatio[0][4] = (double)SockData.fData1;
		break;
	case _1LnGoodRtoDn:
		pDoc->m_dStripRatio[1][0] = (double)SockData.fData1;
		break;
	case _2LnGoodRtoDn:
		pDoc->m_dStripRatio[1][1] = (double)SockData.fData1;
		break;
	case _3LnGoodRtoDn:
		pDoc->m_dStripRatio[1][2] = (double)SockData.fData1;
		break;
	case _4LnGoodRtoDn:
		pDoc->m_dStripRatio[1][3] = (double)SockData.fData1;
		break;
	case _AllLnGoodRtoDn:
		pDoc->m_dStripRatio[1][4] = (double)SockData.fData1;
		break;
	case _1LnGoodRtoTot:
		pDoc->m_dStripRatio[2][0] = (double)SockData.fData1;
		break;
	case _2LnGoodRtoTot:
		pDoc->m_dStripRatio[2][1] = (double)SockData.fData1;
		break;
	case _3LnGoodRtoTot:
		pDoc->m_dStripRatio[2][2] = (double)SockData.fData1;
		break;
	case _4LnGoodRtoTot:
		pDoc->m_dStripRatio[2][3] = (double)SockData.fData1;
		break;
	case _AllLnGoodRtoTot:
		pDoc->m_dStripRatio[2][4] = (double)SockData.fData1;
		break;
	default:
		break;
	}
}

void CEngrave::GetDef(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	switch (nMsgId)
	{
	case _DefNumOpen:
		pDoc->m_nDef[DEF_OPEN] = SockData.nData1; // IDC_STC_DEF_OPEN
		break;
	case _DefNumShort:
		pDoc->m_nDef[DEF_SHORT] = SockData.nData1; // IDC_STC_DEF_SHORT
		break;
	case _DefNumUshort:
		pDoc->m_nDef[DEF_USHORT] = SockData.nData1; // IDC_STC_DEF_U_SHORT
		break;
	case _DefNumLnW:
		pDoc->m_nDef[DEF_SPACE] = SockData.nData1; // IDC_STC_DEF_SPACE
		break;
	case _DefExtr:
		pDoc->m_nDef[DEF_EXTRA] = SockData.nData1; // IDC_STC_DEF_EXTRA
		break;
	case _DefNumProt:
		pDoc->m_nDef[DEF_PROTRUSION] = SockData.nData1; // IDC_STC_DEF_PROT
		break;
	case _DefNumPhole:
		pDoc->m_nDef[DEF_PINHOLE] = SockData.nData1; // IDC_STC_DEF_P_HOLE
		break;
	case _DefNumPad:
		pDoc->m_nDef[DEF_PAD] = SockData.nData1; // IDC_STC_DEF_PAD
		break;
	case _DefNumHopen:
		pDoc->m_nDef[DEF_HOLE_OPEN] = SockData.nData1; // IDC_STC_DEF_H_OPEN
		break;
	case _DefNumHmiss:
		pDoc->m_nDef[DEF_HOLE_MISS] = SockData.nData1; // IDC_STC_DEF_H_MISS
		break;
	case _DefNumHpos:
		pDoc->m_nDef[DEF_HOLE_POSITION] = SockData.nData1; // IDC_STC_DEF_H_POS
		break;
	case _DefNumHdef:
		pDoc->m_nDef[DEF_HOLE_DEFECT] = SockData.nData1; // IDC_STC_DEF_H_DEF
		break;
	case _DefNumNick:
		pDoc->m_nDef[DEF_NICK] = SockData.nData1; // IDC_STC_DEF_NICK
		break;
	case _DefNumPoi:
		pDoc->m_nDef[DEF_POI] = SockData.nData1; // IDC_STC_DEF_POI
		break;
	case _DefNumVhOpen:
		pDoc->m_nDef[DEF_VH_OPEN] = SockData.nData1; // IDC_STC_DEF_VH_OPEN
		break;
	case _DefNumVhMiss:
		pDoc->m_nDef[DEF_VH_MISS] = SockData.nData1; // IDC_STC_DEF_VH_MISS
		break;
	case _DefNumVhPos:
		pDoc->m_nDef[DEF_VH_POSITION] = SockData.nData1; // IDC_STC_DEF_VH_POS
		break;
	case _DefNumVhd:
		pDoc->m_nDef[DEF_VH_DEF] = SockData.nData1; // IDC_STC_DEF_VH_DEF
		break;
	case _DefNumLight:
		pDoc->m_nDef[DEF_LIGHT] = SockData.nData1; // IDC_STC_DEF_LIGHT
		break;
	case _DefNumEnick:
		pDoc->m_nDef[DEF_EDGE_NICK] = SockData.nData1;
		break;
	case _DefNumEprot:
		pDoc->m_nDef[DEF_EDGE_PROT] = SockData.nData1;
		break;
	case _DefNumEspace:
		pDoc->m_nDef[DEF_EDGE_SPACE] = SockData.nData1;
		break;
	case _DefNumUdd1:
		pDoc->m_nDef[DEF_USER_DEFINE_1] = SockData.nData1;
		break;
	case _DefNumNrw:
		pDoc->m_nDef[DEF_NARROW] = SockData.nData1;
		break;
	case _DefNumWide:
		pDoc->m_nDef[DEF_WIDE] = SockData.nData1;
		break;
	default:
		break;
	}
}

void CEngrave::Get2DReader(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	switch (nMsgId)
	{
	case _2DEngLen:
		pDoc->WorkingInfo.Motion.s2DEngLen = CharToString(SockData.strData);
		break;
	case _2DAoiLen:
		pDoc->WorkingInfo.Motion.s2DAoiLen = CharToString(SockData.strData);
		break;
	case _2DMkLen:
		pDoc->WorkingInfo.Motion.s2DMkLen = CharToString(SockData.strData);
		break;
	case _2DMoveVel:
		pDoc->WorkingInfo.Motion.s2DMoveVel = CharToString(SockData.strData);
		break;
	case _2DMoveAcc:
		pDoc->WorkingInfo.Motion.s2DMoveAcc = CharToString(SockData.strData);
		break;
	case _2DOneShotLen:
		pDoc->WorkingInfo.Motion.s2DOneShotRemainLen = CharToString(SockData.strData);
		break;
	default:
		break;
	}
}

void CEngrave::GetEngInfo(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	switch (nMsgId)
	{
	case _EngLeadPitch:
		pDoc->WorkingInfo.Motion.sEngraveFdLead = CharToString(SockData.strData);
		break;
	case _EngPushOffLen:
		pDoc->WorkingInfo.Motion.sEngraveFdVacOff = CharToString(SockData.strData);
		break;
	case _EngTqVal:
		pDoc->WorkingInfo.Motion.sEngraveTq = CharToString(SockData.strData);
		break;
	case _EngAoiLen:
		pDoc->WorkingInfo.Motion.sEngAoiLen = CharToString(SockData.strData);
		break;
	case _EngFdDiffMax:
		pDoc->WorkingInfo.Motion.sEngFdDiffMax = CharToString(SockData.strData);
		break;
	case _EngFdDiffRng:
		pDoc->WorkingInfo.Motion.sEngFdDiffRng = CharToString(SockData.strData);
		break;
	case _EngFdDiffNum:
		pDoc->WorkingInfo.Motion.sEngFdDiffNum = CharToString(SockData.strData);
		break;
	case _EngBuffInitPos:
		pDoc->WorkingInfo.Motion.sEngBuffInitPos = CharToString(SockData.strData);
		break;
	case _EngBuffCurrPos:
		pDoc->WorkingInfo.Motion.sEngBuffCurrPos = CharToString(SockData.strData);
		break;
	default:
		break;
	}
}

void CEngrave::GetFdInfo(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	switch (nMsgId)
	{
	case _FdVel:
		pDoc->WorkingInfo.Motion.sMkFdVel = CharToString(SockData.strData);
		break;
	case _FdAcc:
		pDoc->WorkingInfo.Motion.sMkFdAcc = CharToString(SockData.strData);
		break;
	case _OnePnlLen:
		pDoc->WorkingInfo.Motion.sMkFdDist = CharToString(SockData.strData);
		break;
	case _OnePnlVel:
		pDoc->WorkingInfo.Motion.sMkFdVel = CharToString(SockData.strData);
		break;
	case _OnePnlAcc:
		pDoc->WorkingInfo.Motion.sMkFdAcc = CharToString(SockData.strData);
		break;
	case _FdDiffMax:
		pDoc->WorkingInfo.Motion.sLmtFdErr = CharToString(SockData.strData);
		break;
	case _FdDiffRng:
		pDoc->WorkingInfo.Motion.sLmtFdAdjOffSet = CharToString(SockData.strData);
		break;
	case _FdDiffNum:
		pDoc->WorkingInfo.Motion.sLmtFdOvrNum = CharToString(SockData.strData);
		break;
	default:
		break;
	}
}

void CEngrave::GetAoiInfo(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	switch (nMsgId)
	{
	case _AoiLeadPitch:
		pDoc->WorkingInfo.Motion.sAoiFdLead = CharToString(SockData.strData);
		break;
	case _AoiPushOffLen:
		pDoc->WorkingInfo.Motion.sAoiFdVacOff = CharToString(SockData.strData);
		break;
	case _AoiTqVal:
		pDoc->WorkingInfo.Motion.sAoiTq = CharToString(SockData.strData);
		break;
	case _AoiBuffShotNum:
		pDoc->WorkingInfo.Motion.sFdAoiAoiDistShot = CharToString(SockData.strData);
		break;
	case _AoiMkLen:
		pDoc->WorkingInfo.Motion.sFdMkAoiInitDist = CharToString(SockData.strData);
		break;
	default:
		break;
	}
}

void CEngrave::GetMkInfo(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	switch (nMsgId)
	{
	case _MkLeadPitch:
		pDoc->WorkingInfo.Motion.sMkFdLead = CharToString(SockData.strData);
		break;
	case _MkPushOffLen:
		pDoc->WorkingInfo.Motion.sMkTq = CharToString(SockData.strData);
		break;
	case _MkBuffInitPos:
		pDoc->WorkingInfo.Motion.sStBufPos = CharToString(SockData.strData);
		break;
	case _MkBuffCurrPos:
		pDoc->m_dMkBuffCurrPos = (double)SockData.fData1;
		break;
	case _MkNumLf:
		pDoc->WorkingInfo.Marking[0].nMkCnt = SockData.nData1;
		break;
	case _MkNumRt:
		pDoc->WorkingInfo.Marking[1].nMkCnt = SockData.nData1;
		break;
	case _MkMaxNumLf:
		pDoc->WorkingInfo.Marking[0].nMkLimit = SockData.nData1;
		break;
	case _MkMaxNumRt:
		pDoc->WorkingInfo.Marking[1].nMkLimit = SockData.nData1;
		break;
	default:
		break;
	}
}

void CEngrave::GetMkInfoLf(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	switch (nMsgId)
	{
	case _MkInitPosLf:
		pDoc->WorkingInfo.Marking[0].sWaitPos = CharToString(SockData.strData);
		break;
	case _MkInitVelLf:
		pDoc->WorkingInfo.Marking[0].sWaitVel = CharToString(SockData.strData);
		break;
	case _MkInitAccLf:
		pDoc->WorkingInfo.Marking[0].sWaitAcc = CharToString(SockData.strData);
		break;
	case _MkFnlPosLf:
		pDoc->WorkingInfo.Marking[0].sMarkingPos = CharToString(SockData.strData);
		break;
	case _MkFnlVelLf:
		pDoc->WorkingInfo.Marking[0].sMarkingVel = CharToString(SockData.strData);
		break;
	case _MkFnlAccLf:
		pDoc->WorkingInfo.Marking[0].sMarkingAcc = CharToString(SockData.strData);
		break;
	case _MkFnlTqLf:
		pDoc->WorkingInfo.Marking[0].sMarkingToq = CharToString(SockData.strData);
		break;
	case _MkHgtPosX1Lf:
		pDoc->WorkingInfo.Marking[0].sMeasurePosX[0] = CharToString(SockData.strData);
		break;
	case _MkHgtPosY1Lf:
		pDoc->WorkingInfo.Marking[0].sMeasurePosY[0] = CharToString(SockData.strData);
		break;
	case _MkHgtPosX2Lf:
		pDoc->WorkingInfo.Marking[0].sMeasurePosX[1] = CharToString(SockData.strData);
		break;
	case _MkHgtPosY2Lf:
		pDoc->WorkingInfo.Marking[0].sMeasurePosY[1] = CharToString(SockData.strData);
		break;
	case _MkHgtPosX3Lf:
		pDoc->WorkingInfo.Marking[0].sMeasurePosX[2] = CharToString(SockData.strData);
		break;
	case _MkHgtPosY3Lf:
		pDoc->WorkingInfo.Marking[0].sMeasurePosY[2] = CharToString(SockData.strData);
		break;
	case _MkHgtPosX4Lf:
		pDoc->WorkingInfo.Marking[0].sMeasurePosX[3] = CharToString(SockData.strData);
		break;
	case _MkHgtPosY4Lf:
		pDoc->WorkingInfo.Marking[0].sMeasurePosY[3] = CharToString(SockData.strData);
		break;
	default:
		break;
	}
}

void CEngrave::GetMkInfoRt(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	switch (nMsgId)
	{
	case _MkInitPosRt:
		pDoc->WorkingInfo.Marking[1].sWaitPos = CharToString(SockData.strData);
		break;
	case _MkInitVelRt:
		pDoc->WorkingInfo.Marking[1].sWaitVel = CharToString(SockData.strData);
		break;
	case _MkInitAccRt:
		pDoc->WorkingInfo.Marking[1].sWaitAcc = CharToString(SockData.strData);
		break;
	case _MkFnlPosRt:
		pDoc->WorkingInfo.Marking[1].sMarkingPos = CharToString(SockData.strData);
		break;
	case _MkFnlVelRt:
		pDoc->WorkingInfo.Marking[1].sMarkingVel = CharToString(SockData.strData);
		break;
	case _MkFnlAccRt:
		pDoc->WorkingInfo.Marking[1].sMarkingAcc = CharToString(SockData.strData);
		break;
	case _MkFnlTqRt:
		pDoc->WorkingInfo.Marking[1].sMarkingToq = CharToString(SockData.strData);
		break;
	case _MkHgtPosX1Rt:
		pDoc->WorkingInfo.Marking[1].sMeasurePosX[0] = CharToString(SockData.strData);
		break;
	case _MkHgtPosY1Rt:
		pDoc->WorkingInfo.Marking[1].sMeasurePosY[0] = CharToString(SockData.strData);
		break;
	case _MkHgtPosX2Rt:
		pDoc->WorkingInfo.Marking[1].sMeasurePosX[1] = CharToString(SockData.strData);
		break;
	case _MkHgtPosY2Rt:
		pDoc->WorkingInfo.Marking[1].sMeasurePosY[1] = CharToString(SockData.strData);
		break;
	case _MkHgtPosX3Rt:
		pDoc->WorkingInfo.Marking[1].sMeasurePosX[2] = CharToString(SockData.strData);
		break;
	case _MkHgtPosY3Rt:
		pDoc->WorkingInfo.Marking[1].sMeasurePosY[2] = CharToString(SockData.strData);
		break;
	case _MkHgtPosX4Rt:
		pDoc->WorkingInfo.Marking[1].sMeasurePosX[3] = CharToString(SockData.strData);
		break;
	case _MkHgtPosY4Rt:
		pDoc->WorkingInfo.Marking[1].sMeasurePosY[3] = CharToString(SockData.strData);
		break;
	default:
		break;
	}
}

// End for GetSysData()

// Start for SetSysSignal()

BOOL CEngrave::SetSysSignal()
{
	if (!IsRunning())
	{
		SetSignalMain();
		SetSignalTorqueMotor();
		SetSignalInductionMotor();
		SetSignalCore150mm();
		//SetSignalEtc();
		SetSignalRecoiler();
		SetSignalPunch();
		SetSignalAOIDn();
		SetSignalAOIUp();
		SetSignalEngrave();
		SetSignalUncoiler();

		return TRUE;
	}

	return FALSE;
}

void CEngrave::SetSignalMain()
{
	SwReady(pDoc->BtnStatus.Main.Ready);		// 마킹부 운전준비 스위치 "MB005503" IDC_CHK_34
	SwRun(pDoc->BtnStatus.Main.Run);			// 마킹부 운전 스위치 "MB005501" IDC_CHK_0
	SwReset(pDoc->BtnStatus.Main.Reset);		// 마킹부 리셋 스위치 "MB005504" IDC_CHK_1
	SwStop(pDoc->BtnStatus.Main.Stop);			// 마킹부 정지 스위치 "MB005502" IDC_CHK_33
	SwAuto(pDoc->BtnStatus.Main.Auto);			// 마킹부 자동 스위치 MB005505,	X4205	,	마킹부 자동/수동 (ON)
	SwManual(pDoc->BtnStatus.Main.Manual);		// 마킹부 수동 스위치 MB005505,	X4205	,	마킹부 자동/수동 (OFF)
}

void CEngrave::SetSignalTorqueMotor()
{
	SwMkTq(pDoc->BtnStatus.Tq.Mk); 				// 마킹부 텐션 ON (PC가 ON/OFF시킴) "MB440155" IDC_CHK_2
	// pDoc->WorkingInfo.Motion.bMkTq
	SwAoiTq(pDoc->BtnStatus.Tq.Aoi); 			// 검사부 텐션 ON (PC가 ON/OFF시킴) "MB440156" IDC_CHK_3
	// pDoc->WorkingInfo.Motion.bAoiTq
	SwEngTq(pDoc->BtnStatus.Tq.Eng); 			// 각인부 텐션 ON (PC가 ON/OFF시킴) "MB440154" IDC_CHK_84
	// pDoc->WorkingInfo.Motion.bEngraveTq
}

void CEngrave::SetSignalInductionMotor()
{
	SwRcInductionCcw(pDoc->BtnStatus.Induct.Rc);	// One Metal IDC_CHK_68		
	//pView->SetTwoMetal(FALSE, TRUE);
	SwUcInductionCcw(pDoc->BtnStatus.Induct.Uc);	// Two Metal IDC_CHK_69
	//pView->SetTwoMetal(TRUE, TRUE);
}

void CEngrave::SetSignalCore150mm()
{
	SwRcCore150mm(pDoc->BtnStatus.Core150.Rc);	// Recoiler IDC_CHK_70		
	//SetCore150mmRecoiler(TRUE);
	SwUcCore150mm(pDoc->BtnStatus.Core150.Uc);	// Uncoiler IDC_CHK_71		
	//SetCore150mmUncoiler(TRUE);
}

void CEngrave::SetSignalEtc()
{
	//SwAoiEmg(pDoc->BtnStatus.Etc.EmgAoi);
}

void CEngrave::SetSignalRecoiler()
{
	SwRcRelation(pDoc->BtnStatus.Rc.Relation);						// 리코일러 연동 온/오프 스위치 "MB005801" IDC_CHK_4
	SwRcFdCw(pDoc->BtnStatus.Rc.FdCw);								// 리코일러 제품휠 정회전 스위치 "MB00580C" IDC_CHK_5
	SwRcFdCcw(pDoc->BtnStatus.Rc.FdCcw);							// 리코일러 제품휠 역회전 스위치 "MB00580D" IDC_CHK_6
	SwRcReelChuck(pDoc->BtnStatus.Rc.ReelChuck);					// 리코일러 제품척 클램프 스위치 "MB00580B" IDC_CHK_41
	SwRcDcRlUpDn(pDoc->BtnStatus.Rc.DcRlUpDn);						// 리코일러 댄서롤 상승/하강 스위치 "MB005802" IDC_CHK_42
	SwRcReelJoinL(pDoc->BtnStatus.Rc.ReelJoinL);					// 리코일러 제품 이음매(상/좌) 스위치 "MB005805" IDC_CHK_43
	SwRcReelJoinR(pDoc->BtnStatus.Rc.ReelJoinR);					// 리코일러 제품 이음매(하/우) 스위치 "MB005806" IDC_CHK_7
	SwRcReelJoinVac(pDoc->BtnStatus.Rc.ReelJoinVac);				// 리코일러 제품 이음매 진공 스위치 "MB00580F" IDC_CHK_8
	SwRcPaperChuck(pDoc->BtnStatus.Rc.PprChuck);					// 리코일러 간지척 클램프 스위치 "MB005808" IDC_CHK_44
	SwRcPaperCw(pDoc->BtnStatus.Rc.PprCw);							// 리코일러 간지휠 정회전 스위치 "MB005809" IDC_CHK_45
	SwRcPaperCcw(pDoc->BtnStatus.Rc.PprCcw);						// 리코일러 간지휠 역회전 스위치 "MB00580A" IDC_CHK_46
	SwRcRewinder(pDoc->BtnStatus.Rc.Rewine);						// 리코일러 Rewinder 동작 스위치 "MB005803" IDC_CHK_66
	SwRcRewinderReelPaper(pDoc->BtnStatus.Rc.RewineReelPpr);		// 리코일러 Rewinder 제품 & 간지 스위치 "MB005804" IDC_CHK_67
}

void CEngrave::SetSignalPunch()
{
	SwMkRelation(pDoc->BtnStatus.Mk.Relation);			// 마킹부 연동 온/오프 스위치 "MB005511" IDC_CHK_9
	SwMkFdCw(pDoc->BtnStatus.Mk.FdCw);					// 마킹부 피딩 정회전 스위치 "MB005513" IDC_CHK_10
	SwMkFdCcw(pDoc->BtnStatus.Mk.FdCcw);				// 마킹부 피딩 역회전 스위치 "MB005514" IDC_CHK_11
	SwMkFdVac(pDoc->BtnStatus.Mk.FdVac);				// 마킹부 피딩 진공 스위치 "MB005515" IDC_CHK_12
	SwMkPushUp(pDoc->BtnStatus.Mk.PushUp);				// 마킹부 제품푸쉬 스위치 "MB005516" // (토크 진공 스위치) - X IDC_CHK_13
	SwMkTblBlw(pDoc->BtnStatus.Mk.TblBlw);				// 마킹부 테이블 브로워 스위치 "MB005512" IDC_CHK_14
	SwMkTblVac(pDoc->BtnStatus.Mk.TblVac);				// 마킹부 테이블 진공 스위치 "MB005517" IDC_CHK_15
	SwMkFdClp(pDoc->BtnStatus.Mk.FdClp);				// 마킹부 피딩 클램프 스위치 "MB005519" IDC_CHK_51
	SwMkTqClp(pDoc->BtnStatus.Mk.TqClp);				// 마킹부 텐션 클램프 스위치 "MB00551A" IDC_CHK_52
	SwMkMvOne(pDoc->BtnStatus.Mk.MvOne);				// 마킹부 한판넬 이송 스위치 "MB440151" IDC_CHK_16
	SwMkLsrPt(pDoc->BtnStatus.Mk.LsrPt);				// 마킹부 레이져마크 스위치 "MB005518" IDC_CHK_49
	SwMkDcRSol(pDoc->BtnStatus.Mk.DcRSol);				// 마킹부 댄서롤 상승/하강 스위치 "MB00551B", "X421B" IDC_CHK_48
}

void CEngrave::SetSignalAOIDn()
{
	SwAoiDnRelation(pDoc->BtnStatus.AoiDn.Relation);			// 검사부 하 연동 온/오프 스위치 "MB005701" IDC_CHK_55
	SwAoiDnFdCw(pDoc->BtnStatus.AoiDn.FdCw);					// 검사부 하 피딩 정회전 스위치 "MB005703" IDC_CHK_56
	SwAoiDnFdCcw(pDoc->BtnStatus.AoiDn.FdCcw);					// 검사부 하 피딩 역회전 스위치 "MB005704" IDC_CHK_57
	SwAoiDnFdVac(pDoc->BtnStatus.AoiDn.FdVac);					// 검사부 하 피딩 진공 스위치 "MB005705" IDC_CHK_58
	SwAoiDnPushUp(pDoc->BtnStatus.AoiDn.PushUp);				// 검사부 하 제품푸쉬 스위치 "MB005706" IDC_CHK_59 // (토크 진공 스위치) - X
	SwAoiDnTblBlw(pDoc->BtnStatus.AoiDn.TblBlw);				// 검사부 하 테이블 브로워 스위치 "MB005702" IDC_CHK_60
	SwAoiDnTblVac(pDoc->BtnStatus.AoiDn.TblVac);				// 검사부 하 테이블 진공 스위치 "MB005707" IDC_CHK_61
	SwAoiDnFdClp(pDoc->BtnStatus.AoiDn.FdClp);					// 검사부 하 피딩 클램프 스위치 "MB005709" IDC_CHK_64
	SwAoiDnTqClp(pDoc->BtnStatus.AoiDn.TqClp);					// 검사부 하 텐션 클램프 스위치 "MB00570A" IDC_CHK_65
	SwAoiDnMvOne(pDoc->BtnStatus.AoiDn.MvOne);					// 검사부 하 한판넬 이송 스위치 "MB440151" IDC_CHK_62
	SwAoiDnLsrPt(pDoc->BtnStatus.AoiDn.LsrPt);					// 검사부 하 레이져마크 스위치 "MB005708" IDC_CHK_63
	SwAoiDnSonicBlw(pDoc->BtnStatus.AoiDn.SonicBlw);			// 검사부 하 초음파 세정기 스위치 "MB44014F"  IDC_CHK_88 // pDoc->WorkingInfo.LastJob.bAoiDnCleanner
}

void CEngrave::SetSignalAOIUp()
{
	SwAoiUpRelation(pDoc->BtnStatus.AoiUp.Relation);	// 검사부 상 연동 온/오프 스위치 "MB005601" IDC_CHK_17
	SwAoiUpFdCw(pDoc->BtnStatus.AoiUp.FdCw);			// 검사부 상 피딩 정회전 스위치 "MB005603" IDC_CHK_18
	SwAoiUpFdCcw(pDoc->BtnStatus.AoiUp.FdCcw);			// 검사부 상 피딩 역회전 스위치 "MB005604" IDC_CHK_19
	SwAoiUpFdVac(pDoc->BtnStatus.AoiUp.FdVac);			// 검사부 상 피딩 진공 스위치 "MB005605" IDC_CHK_20
	SwAoiUpPushUp(pDoc->BtnStatus.AoiUp.PushUp);		// 검사부 상 제품푸쉬 스위치 "MB005606" IDC_CHK_21 // (토크 진공 스위치) - X
	SwAoiUpTblBlw(pDoc->BtnStatus.AoiUp.TblBlw);		// 검사부 상 테이블 브로워 스위치 "MB005602" IDC_CHK_22
	SwAoiUpTblVac(pDoc->BtnStatus.AoiUp.TblVac);		// 검사부 상 테이블 진공 스위치 "MB005607" IDC_CHK_23
	SwAoiUpFdClp(pDoc->BtnStatus.AoiUp.FdClp);			// 검사부 상 피딩 클램프 스위치 "MB005609" IDC_CHK_53
	SwAoiUpTqClp(pDoc->BtnStatus.AoiUp.TqClp);			// 검사부 상 텐션 클램프 스위치 "MB00560A" IDC_CHK_54
	SwAoiUpMvOne(pDoc->BtnStatus.AoiUp.MvOne);			// 검사부 상 한판넬 이송 스위치  "MB440151" IDC_CHK_24
	SwAoiUpLsrPt(pDoc->BtnStatus.AoiUp.LsrPt);			// 검사부 상 레이져마크 스위치 "MB005608" IDC_CHK_50
}

void CEngrave::SetSignalEngrave()
{
	SwEngRelation(pDoc->BtnStatus.Eng.Relation);		// 각인부 연동 온/오프 스위치 IDC_CHK_72
	SwEngFdCw(pDoc->BtnStatus.Eng.FdCw);				// 각인부 피딩 정회전 스위치 IDC_CHK_73
	SwEngFdCcw(pDoc->BtnStatus.Eng.FdCcw);				// 각인부 피딩 역회전 스위치 IDC_CHK_74
	SwEngFdVac(pDoc->BtnStatus.Eng.FdVac);				// 각인부 피딩 진공 스위치 IDC_CHK_75
	SwEngPushUp(pDoc->BtnStatus.Eng.PushUp);			// 각인부 제품푸쉬 스위치 IDC_CHK_76 // (토크 진공 스위치) - X
	SwEngTblBlw(pDoc->BtnStatus.Eng.TblBlw);			// 각인부 테이블 브로워 스위치 IDC_CHK_77
	SwEngTblVac(pDoc->BtnStatus.Eng.TblVac);			// 각인부 테이블 진공 스위치 IDC_CHK_78
	SwEngFdClp(pDoc->BtnStatus.Eng.FdClp);				// 각인부 피딩 클램프 스위치 IDC_CHK_82
	SwEngTqClp(pDoc->BtnStatus.Eng.TqClp);				// 각인부 텐션 클램프 스위치 IDC_CHK_83
	SwEngMvOne(pDoc->BtnStatus.Eng.MvOne);				// 각인부 한판넬 이송 스위치  "MB440151" IDC_CHK_79
	SwEngLsrPt(pDoc->BtnStatus.Eng.LsrPt);				// 각인부 레이져마크 스위치 "" IDC_CHK_81
	SwEngSonicBlw(pDoc->BtnStatus.Eng.SonicBlw);		// 각인부 초음파 세정기 스위치 "MB44014E" IDC_CHK_87 pDoc->WorkingInfo.LastJob.bEngraveCleanner
	SwEngDcRSol(pDoc->BtnStatus.Eng.DcRSol);			// 각인부 댄서롤 상승/하강 스위치 IDC_CHK_80
}

void CEngrave::SetSignalUncoiler()
{
	SwUcRelation(pDoc->BtnStatus.Uc.Relation);			// 언코일러 연동 온/오프 스위치 "MB005401" IDC_CHK_25
	SwUcFdCw(pDoc->BtnStatus.Uc.FdCw);					// 언코일러 제품휠 정회전 스위치 "MB00540C" IDC_CHK_26
	SwUcFdCcw(pDoc->BtnStatus.Uc.FdCcw);				// 언코일러 제품휠 역회전 스위치 "MB00540D" IDC_CHK_27
	SwUcReelChuck(pDoc->BtnStatus.Uc.ReelChuck);		// 언코일러 제품척 클램프 스위치 "MB00540B" IDC_CHK_35
	SwUcDcRlUpDn(pDoc->BtnStatus.Uc.DcRlUpDn);			// 언코일러 댄서롤 상승/하강 스위치 "MB005402" IDC_CHK_28
	SwUcReelJoinL(pDoc->BtnStatus.Uc.ReelJoinL);		// 언코일러 제품 이음매(상/좌) 스위치 "MB005405" IDC_CHK_30
	SwUcReelJoinR(pDoc->BtnStatus.Uc.ReelJoinR);		// 언코일러 제품 이음매(하/우) 스위치 "MB005406" IDC_CHK_37
	SwUcReelJoinVac(pDoc->BtnStatus.Uc.ReelJoinVac);	// 언코일러 제품 이음매 진공 스위치 "MB00540F" IDC_CHK_38
	SwUcPaperChuck(pDoc->BtnStatus.Uc.PprChuck);		// 언코일러 간지척 클램프 스위치 "MB005408" IDC_CHK_31
	SwUcPaperCw(pDoc->BtnStatus.Uc.PprCw);				// 언코일러 간지휠 정회전 스위치 "MB005409" IDC_CHK_32
	SwUcPaperCcw(pDoc->BtnStatus.Uc.PprCcw);			// 언코일러 간지휠 역회전 스위치 "MB00540A" IDC_CHK_39
	SwUcClRlUpDn(pDoc->BtnStatus.Uc.ClRlUpDn);			// 언코일러 클린롤러 상승/하강 스위치 "MB005403" IDC_CHK_29
	SwUcClRlPshUpDn(pDoc->BtnStatus.Uc.ClRlPshUpDn);	// 언코일러 클린롤러누름 상승/하강 스위치 "MB005404" IDC_CHK_36
}

// End SetSysSignal()

// Start for SetSysData()

BOOL CEngrave::SetSysData()
{
	if (!IsRunning())
	{
		SetOpInfo();
		SetInfo();
		SetTotRatio();
		SetStTime();
		SetRunTime();
		SetEdTime();
		SetStripRatio();
		SetDef();
		Set2DReader();
		SetEngInfo();
		SetFdInfo();
		SetAoiInfo();
		SetMkInfo();
		SetMkInfoLf();
		SetMkInfoRt();

		return TRUE;
	}

	return FALSE;
}

void CEngrave::SetOpInfo()
{
	SetOpName();
	SetDualTest();
	SetSampleTest();
	SetSampleShotNum();
	SetTestMode();
	SetRecoilerCcw();
	SetUncoilerCcw();
	SetAlignMethode();
	SetDoorRecoiler();
	SetDoorAoiUp();
	SetDoorAoiDn();
	SetDoorMk();
	SetDoorEngrave();
	SetDoorUncoiler();
	SetSaftyMk();
	SetCleannerAoiUp();
	SetCleannerAoiDn();
	SetUltraSonicAoiDn();
	SetUltraSonicEngrave();
	SetTotReelLen();
	SetOnePnlLen();
	SetTempPause();
	SetTempStopLen();
	SetLotCut();
	SetLotCutLen();
	SetLotCutPosLen();
	SetLmtTotYld();
	SetLmtPatlYld();
	SetStripOutRatio();
	SetCustomNeedRatio();
	SetNumRangeFixDef();
	SetNumContFixDef();
	SetUltraSonicStTim();

/*	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	CString sVal;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _OpName;
	StringToChar(pDoc->WorkingInfo.LastJob.sSelUserName, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _DualTest;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bDualTest ? 1 : 0;
	SendCommand(SocketData);

	SocketData.nMsgID = _SampleTest;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bSampleTest ? 1 : 0;
	SendCommand(SocketData);

	SocketData.nMsgID = _SampleShotNum;
	SocketData.nData1 = _ttoi(pDoc->WorkingInfo.LastJob.sSampleTestShotNum);
	SendCommand(SocketData);

	SocketData.nMsgID = _TestMode;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.nTestMode; // MODE_NONE = 0, MODE_INNER = 1, MODE_OUTER = 2
	SendCommand(SocketData);

	SocketData.nMsgID = _RecoilerCcw;						// OneMetal : TRUE -> SetTwoMetal(FALSE);
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bOneMetal ? 1 : 0;
	SendCommand(SocketData);

	SocketData.nMsgID = _UncoilerCcw;						// TwoMetal : TRUE -> SetTwoMetal(TRUE);
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bTwoMetal ? 1 : 0;
	SendCommand(SocketData);

	SocketData.nMsgID = _AlignMethode;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.nAlignMethode; // TWO_POINT, FOUR_POINT
	SendCommand(SocketData);

	SocketData.nMsgID = _DoorRecoiler;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bRclDrSen ? 1 : 0;
	SendCommand(SocketData);

	SocketData.nMsgID = _DoorAoiUp;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bAoiUpDrSen ? 1 : 0;
	SendCommand(SocketData);

	SocketData.nMsgID = _DoorAoiDn;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bAoiDnDrSen ? 1 : 0;
	SendCommand(SocketData);

	SocketData.nMsgID = _DoorMk;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bMkDrSen ? 1 : 0;
	SendCommand(SocketData);

	SocketData.nMsgID = _DoorEngrave;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bEngvDrSen ? 1 : 0;
	SendCommand(SocketData);

	SocketData.nMsgID = _DoorUncoiler;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bUclDrSen ? 1 : 0;
	SendCommand(SocketData);

	SocketData.nMsgID = _SaftyMk;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bMkSftySen ? 1 : 0;
	SendCommand(SocketData);

	SocketData.nMsgID = _CleannerAoiUp;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bUseAoiUpCleanRoler ? 1 : 0;
	SendCommand(SocketData);

	SocketData.nMsgID = _CleannerAoiDn;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bUseAoiDnCleanRoler ? 1 : 0;
	SendCommand(SocketData);

	SocketData.nMsgID = _UltraSonicAoiDn;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bUseAoiDnCleanner ? 1 : 0;
	SendCommand(SocketData);

	SocketData.nMsgID = _UltraSonicEngrave;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bUseEngraveCleanner ? 1 : 0;
	SendCommand(SocketData);

	SocketData.nMsgID = _TotReelLen;
	StringToChar(pDoc->WorkingInfo.LastJob.sReelTotLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _OnePnlLen;
	sprintf(SocketData.strData, "%.3f", pDoc->GetOnePnlLen());
	SendCommand(SocketData);

	SocketData.nMsgID = _TempPause;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bTempPause ? 1 : 0;
	SendCommand(SocketData);

	SocketData.nMsgID = _TempStopLen;
	StringToChar(pDoc->WorkingInfo.LastJob.sTempPauseLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _LotCut;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bLotSep ? 1 : 0;	// pDoc->m_pReelMap->m_bUseLotSep
	SendCommand(SocketData);

	SocketData.nMsgID = _LotCutLen;
	StringToChar(pDoc->WorkingInfo.LastJob.sLotSepLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _LotCutPosLen;
	StringToChar(pDoc->WorkingInfo.LastJob.sLotCutPosLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _LmtTotYld;
	StringToChar(pDoc->WorkingInfo.LastJob.sLmtTotYld, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _LmtPatlYld;
	StringToChar(pDoc->WorkingInfo.LastJob.sLmtPatlYld, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _StripOutRatio;
	StringToChar(pDoc->WorkingInfo.LastJob.sStripOutRatio, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _CustomNeedRatio;
	StringToChar(pDoc->WorkingInfo.LastJob.sCustomNeedRatio, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _NumRangeFixDef;
	StringToChar(pDoc->WorkingInfo.LastJob.sNumRangeFixDef, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _NumContFixDef;
	StringToChar(pDoc->WorkingInfo.LastJob.sNumContFixDef, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _UltraSonicStTim;
	StringToChar(pDoc->WorkingInfo.LastJob.sUltraSonicCleannerStTim, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);*/
}

void CEngrave::SetInfo()
{
	SetModelUpName();
	SetModelDnName();
	SetLotUpName();
	SetLotDnName();
	SetLayerUpName();
	SetLayerDnName();
	SetOrderNum();
	SetShotNum();
	SetTotOpRto();
	SetTotVel();
	SetPartVel();
	SetMkDoneLen();
	SetAoiDnDoneLen();
	SetAoiUpDoneLen();
	SetLotSerial();
	SetMkVerfyLen();

	/*SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	CString sVal;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _OpName;
	StringToChar(pDoc->WorkingInfo.LastJob.sSelUserName, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _ModelUpName;
	StringToChar(pDoc->WorkingInfo.LastJob.sModelUp, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _ModelDnName;
	StringToChar(pDoc->WorkingInfo.LastJob.sModelDn, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _LotUpName;
	StringToChar(pDoc->WorkingInfo.LastJob.sLotUp, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _LotDnName;
	StringToChar(pDoc->WorkingInfo.LastJob.sLotDn, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _LayerUpName;
	StringToChar(pDoc->WorkingInfo.LastJob.sLayerUp, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _LayerDnName;
	StringToChar(pDoc->WorkingInfo.LastJob.sLayerDn, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	// _OrderNum
	//_ShotNum

	SocketData.nMsgID = _TotReelLen;
	StringToChar(pDoc->WorkingInfo.LastJob.sReelTotLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	// _TotOpRto
	// _LotOpRto
	// _TotVel

	SocketData.nMsgID = _PartVel;
	StringToChar(pDoc->WorkingInfo.LastJob.sPartialSpd, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _TempPause;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bTempPause ? 1 : 0;
	SendCommand(SocketData);

	SocketData.nMsgID = _TempStopLen;
	StringToChar(pDoc->WorkingInfo.LastJob.sTempPauseLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _LotCutLen;
	StringToChar(pDoc->WorkingInfo.LastJob.sLotSepLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	// _MkDoneLen
	// _AoiDnDoneLen
	// _AoiUpDoneLen

	SocketData.nMsgID = _LotSerial;
	StringToChar(pDoc->WorkingInfo.LastJob.sLotSerial, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _MkVerfyLen;
	StringToChar(pDoc->WorkingInfo.LastJob.sVerifyLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);*/
}

void CEngrave::SetTotRatio()
{
	SetDefNumUp();
	SetDefRtoUp();
	SetGoodNumUp();
	SetGoodRtoUp();
	SetTestNumUp();
	SetDefNumDn();
	SetDefRtoDn();
	SetGoodNumDn();
	SetGoodRtoDn();
	SetTestNumDn();
	SetDefNumTot();
	SetDefRtoTot();
	SetGoodNumTot();
	SetGoodRtoTot();
	SetTestNumTot();

/*	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	CString str;
	int nGood = 0, nBad = 0, nTot = 0, nVal;
	double dRatio = 0.0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	// < 전체 수율 >

	// 상면
	pDoc->m_pReelMapUp->GetPcsNum(nGood, nBad);
	nTot = nGood + nBad;

	SocketData.nMsgID = _DefNumUp;
	SocketData.nData1 = nBad;
	SendCommand(SocketData);

	if (nTot > 0)
		dRatio = ((double)nBad / (double)nTot) * 100.0;
	else
		dRatio = 0.0;

	SocketData.nMsgID = _DefRtoUp;
	SocketData.fData1 = dRatio;
	SendCommand(SocketData);

	SocketData.nMsgID = _GoodNumUp;
	SocketData.nData1 = nGood;
	SendCommand(SocketData);

	if (nTot > 0)
		dRatio = ((double)nGood / (double)nTot) * 100.0;
	else
		dRatio = 0.0;

	SocketData.nMsgID = _GoodRtoUp;
	SocketData.fData1 = dRatio;
	SendCommand(SocketData);

	SocketData.nMsgID = _TestNumUp;
	SocketData.nData1 = nTot;
	SendCommand(SocketData);

	if (bDualTest)
	{
		// 하면
		pDoc->m_pReelMapDn->GetPcsNum(nGood, nBad);
		nTot = nGood + nBad;

		SocketData.nMsgID = _DefNumDn;
		SocketData.nData1 = nBad;
		SendCommand(SocketData);

		if (nTot > 0)
			dRatio = ((double)nBad / (double)nTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _DefRtoDn;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		SocketData.nMsgID = _GoodNumDn;
		SocketData.nData1 = nGood;
		SendCommand(SocketData);

		if (nTot > 0)
			dRatio = ((double)nGood / (double)nTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _GoodRtoDn;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		SocketData.nMsgID = _TestNumDn;
		SocketData.nData1 = nTot;
		SendCommand(SocketData);

		// 전체
		pDoc->m_pReelMapAllDn->GetPcsNum(nGood, nBad);
		nTot = nGood + nBad;

		SocketData.nMsgID = _DefNumTot;
		SocketData.nData1 = nTot;
		SendCommand(SocketData);

		if (nTot > 0)
			dRatio = ((double)nBad / (double)nTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _DefRtoTot;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		SocketData.nMsgID = _GoodNumTot;
		SocketData.nData1 = nGood;
		SendCommand(SocketData);

		if (nTot > 0)
			dRatio = ((double)nGood / (double)nTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _GoodRtoTot;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		SocketData.nMsgID = _TestNumTot;
		SocketData.nData1 = nTot;
		SendCommand(SocketData);
	}*/
}

void CEngrave::SetStTime()
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	CString str = _T("");
	if(pView)
	{
		if (pView->m_pDlgMenu01)
		{
			str = pView->m_pDlgMenu01->GetStTime();
			char cData[BUFFER_DATA_SIZE];
			SocketData.nMsgID = _LotStTime;
			StringToChar(str, cData);
			sprintf(SocketData.strData, "%s", cData);
			SendCommand(SocketData);
		}
	}
}

void CEngrave::SetRunTime()
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	CString str = _T("");
	if (pView)
	{
		if (pView->m_pDlgMenu01)
		{
			str = pView->m_pDlgMenu01->GetRunTime();
			char cData[BUFFER_DATA_SIZE];
			SocketData.nMsgID = _LotRunTime;
			StringToChar(str, cData);
			sprintf(SocketData.strData, "%s", cData);
			SendCommand(SocketData);
		}
	}
}

void CEngrave::SetEdTime()
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	CString str = _T("");
	if (pView)
	{
		if (pView->m_pDlgMenu01)
		{
			str = pView->m_pDlgMenu01->GetEdTime();
			char cData[BUFFER_DATA_SIZE];
			SocketData.nMsgID = _LotEdTime;
			StringToChar(str, cData);
			sprintf(SocketData.strData, "%s", cData);
			SendCommand(SocketData);
		}
	}
}

void CEngrave::SetStripRatio()
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	CString str;
	int nGood = 0, nBad = 0, nTot = 0, nStTot = 0, nSum = 0, nVal[2][4];
	int nMer[4];
	double dRatio = 0.0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	for (int i = 0; i < 2; i++)
	{
		for (int k = 0; k < 4; k++)
			nVal[i][k] = 0;
	}

	// < 스트립 별 수율 >
	pDoc->m_pReelMapUp->GetPcsNum(nGood, nBad);
	nTot = nGood + nBad;
	nStTot = nTot / 4;

	// 상면
	nSum = 0;
	nVal[0][0] = pDoc->m_pReelMapUp->GetDefStrip(0);
	nSum += nVal[0][0];
	if (nTot > 0)
		dRatio = ((double)(nStTot - nVal[0][0]) / (double)nStTot) * 100.0;
	else
		dRatio = 0.0;

	SocketData.nMsgID = _1LnGoodRtoUp;
	SocketData.fData1 = dRatio;
	SendCommand(SocketData);

	nVal[0][1] = pDoc->m_pReelMapUp->GetDefStrip(1);
	nSum += nVal[0][1];
	if (nTot > 0)
		dRatio = ((double)(nStTot - nVal[0][1]) / (double)nStTot) * 100.0;
	else
		dRatio = 0.0;

	SocketData.nMsgID = _2LnGoodRtoUp;
	SocketData.fData1 = dRatio;
	SendCommand(SocketData);

	nVal[0][2] = pDoc->m_pReelMapUp->GetDefStrip(2);
	nSum += nVal[0][2];
	if (nTot > 0)
		dRatio = ((double)(nStTot - nVal[0][2]) / (double)nStTot) * 100.0;
	else
		dRatio = 0.0;

	SocketData.nMsgID = _3LnGoodRtoUp;
	SocketData.fData1 = dRatio;
	SendCommand(SocketData);

	nVal[0][3] = pDoc->m_pReelMapUp->GetDefStrip(3);
	nSum += nVal[0][3];
	if (nTot > 0)
		dRatio = ((double)(nStTot - nVal[0][3]) / (double)nStTot) * 100.0;
	else
		dRatio = 0.0;

	SocketData.nMsgID = _4LnGoodRtoUp;
	SocketData.fData1 = dRatio;
	SendCommand(SocketData);

	if (nTot > 0)
		dRatio = ((double)(nTot - nSum) / (double)nTot) * 100.0;
	else
		dRatio = 0.0;

	SocketData.nMsgID = _AllLnGoodRtoUp;
	SocketData.fData1 = dRatio;
	SendCommand(SocketData);


	if (bDualTest)
	{
		// 하면
		nSum = 0;
		nVal[1][0] = pDoc->m_pReelMapDn->GetDefStrip(0);
		nSum += nVal[1][0];
		if (nTot > 0)
			dRatio = ((double)(nStTot - nVal[1][0]) / (double)nStTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _1LnGoodRtoDn;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		nVal[1][1] = pDoc->m_pReelMapDn->GetDefStrip(1);
		nSum += nVal[1][1];
		if (nTot > 0)
			dRatio = ((double)(nStTot - nVal[1][1]) / (double)nStTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _2LnGoodRtoDn;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		nVal[1][2] = pDoc->m_pReelMapDn->GetDefStrip(2);
		nSum += nVal[1][2];
		if (nTot > 0)
			dRatio = ((double)(nStTot - nVal[1][2]) / (double)nStTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _3LnGoodRtoDn;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		nVal[1][3] = pDoc->m_pReelMapDn->GetDefStrip(3);
		nSum += nVal[1][3];
		if (nTot > 0)
			dRatio = ((double)(nStTot - nVal[1][3]) / (double)nStTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _4LnGoodRtoDn;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		if (nTot > 0)
			dRatio = ((double)(nTot - nSum) / (double)nTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _AllLnGoodRtoDn;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		// 상면 + 하면
		nSum = 0;
		nMer[0] = pDoc->m_pReelMapAllUp->GetDefStrip(0);
		nSum += nMer[0];
		if (nTot > 0)
			dRatio = ((double)(nStTot - nMer[0]) / (double)nStTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _1LnGoodRtoTot;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		nMer[1] = pDoc->m_pReelMapAllUp->GetDefStrip(1);
		nSum += nMer[1];
		if (nTot > 0)
			dRatio = ((double)(nStTot - nMer[1]) / (double)nStTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _2LnGoodRtoTot;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		nMer[2] = pDoc->m_pReelMapAllUp->GetDefStrip(2);
		nSum += nMer[2];
		if (nTot > 0)
			dRatio = ((double)(nStTot - nMer[2]) / (double)nStTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _3LnGoodRtoTot;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		nMer[3] = pDoc->m_pReelMapAllUp->GetDefStrip(3);
		nSum += nMer[3];
		if (nTot > 0)
			dRatio = ((double)(nStTot - nMer[3]) / (double)nStTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _4LnGoodRtoTot;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		if (nTot > 0)
			dRatio = ((double)(nTot - nSum) / (double)nTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _AllLnGoodRtoTot;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);
	}
}

void CEngrave::SetDef()
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	CString str;
	int nNum = 0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	CReelMap* pReelMap;

	if (bDualTest)
	{
		switch (pView->m_nSelRmap)
		{
		case RMAP_UP:
			pReelMap = pDoc->m_pReelMapUp;
			break;
		case RMAP_DN:
			pReelMap = pDoc->m_pReelMapDn;
			break;
		case RMAP_ALLUP:
			pReelMap = pDoc->m_pReelMapAllUp;
			break;
		case RMAP_ALLDN:
			pReelMap = pDoc->m_pReelMapAllDn;
			break;
		}
	}
	else
		pReelMap = pDoc->m_pReelMapUp;

	nNum = pReelMap->GetDefNum(DEF_OPEN); // IDC_STC_DEF_OPEN
	SocketData.nMsgID = _DefNumOpen;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_SHORT); // IDC_STC_DEF_SHORT
	SocketData.nMsgID = _DefNumShort;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_USHORT); // IDC_STC_DEF_U_SHORT
	SocketData.nMsgID = _DefNumUshort;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_SPACE); // IDC_STC_DEF_SPACE
	SocketData.nMsgID = _DefNumLnW;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_EXTRA); // IDC_STC_DEF_EXTRA
	SocketData.nMsgID = _DefExtr;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_PROTRUSION); // IDC_STC_DEF_PROT
	SocketData.nMsgID = _DefNumProt;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_PINHOLE); // IDC_STC_DEF_P_HOLE
	SocketData.nMsgID = _DefNumPhole;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_PAD); // IDC_STC_DEF_PAD
	SocketData.nMsgID = _DefNumPad;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_HOLE_OPEN); // IDC_STC_DEF_H_OPEN
	SocketData.nMsgID = _DefNumHopen;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_HOLE_MISS); // IDC_STC_DEF_H_MISS
	SocketData.nMsgID = _DefNumHmiss;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_HOLE_POSITION); // IDC_STC_DEF_H_POS
	SocketData.nMsgID = _DefNumHpos;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_HOLE_DEFECT); // IDC_STC_DEF_H_DEF
	SocketData.nMsgID = _DefNumHdef;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_NICK); // IDC_STC_DEF_NICK
	SocketData.nMsgID = _DefNumNick;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_POI); // IDC_STC_DEF_POI
	SocketData.nMsgID = _DefNumPoi;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_VH_OPEN); // IDC_STC_DEF_VH_OPEN
	SocketData.nMsgID = _DefNumVhOpen;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_VH_MISS); // IDC_STC_DEF_VH_MISS
	SocketData.nMsgID = _DefNumVhMiss;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_VH_POSITION); // IDC_STC_DEF_VH_POS
	SocketData.nMsgID = _DefNumVhPos;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_VH_DEF); // IDC_STC_DEF_VH_DEF
	SocketData.nMsgID = _DefNumVhd;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_LIGHT); // IDC_STC_DEF_LIGHT
	SocketData.nMsgID = _DefNumLight;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_EDGE_NICK);
	SocketData.nMsgID = _DefNumEnick;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_EDGE_PROT);
	SocketData.nMsgID = _DefNumEprot;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_EDGE_SPACE);
	SocketData.nMsgID = _DefNumEspace;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_USER_DEFINE_1);
	SocketData.nMsgID = _DefNumUdd1;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_NARROW);
	SocketData.nMsgID = _DefNumNrw;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);

	nNum = pReelMap->GetDefNum(DEF_WIDE);
	SocketData.nMsgID = _DefNumWide;
	SocketData.nData1 = nNum;
	SendCommand(SocketData);
	
}

void CEngrave::Set2DReader()
{
	Set2DEngLen();
	Set2DAoiLen();
	Set2DMkLen();
	Set2DMoveVel();
	Set2DMoveAcc();
	Set2DOneShotLen();

/*	char cData[BUFFER_DATA_SIZE];
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _2DEngLen; 
	StringToChar(pDoc->WorkingInfo.Motion.s2DEngLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _2DAoiLen; 
	StringToChar(pDoc->WorkingInfo.Motion.s2DAoiLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _2DMkLen; 
	StringToChar(pDoc->WorkingInfo.Motion.s2DMkLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _2DMoveVel; 
	StringToChar(pDoc->WorkingInfo.Motion.s2DMoveVel, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _2DMoveAcc; 
	StringToChar(pDoc->WorkingInfo.Motion.s2DMoveAcc, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _2DOneShotLen; 
	StringToChar(pDoc->WorkingInfo.Motion.s2DOneShotRemainLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);*/
}

void CEngrave::SetEngInfo()
{
	SetEngLeadPitch();
	SetEngPushOffLen();
	SetEngTqVal();
	SetEngAoiLen();
	SetEngFdDiffMax();
	SetEngFdDiffRng();
	SetEngFdDiffNum();
	SetEngBuffInitPos();
	SetEngBuffCurrPos();

/*	char cData[BUFFER_DATA_SIZE];
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _EngLeadPitch; 
	StringToChar(pDoc->WorkingInfo.Motion.sEngraveFdLead, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _EngPushOffLen; 
	StringToChar(pDoc->WorkingInfo.Motion.sEngraveFdVacOff, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _EngTqVal; 
	StringToChar(pDoc->WorkingInfo.Motion.sEngraveTq, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _EngAoiLen; 
	StringToChar(pDoc->WorkingInfo.Motion.sEngAoiLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _EngFdDiffMax; 
	StringToChar(pDoc->WorkingInfo.Motion.sEngFdDiffMax, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _EngFdDiffRng; 
	StringToChar(pDoc->WorkingInfo.Motion.sEngFdDiffRng, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _EngFdDiffNum; 
	StringToChar(pDoc->WorkingInfo.Motion.sEngFdDiffNum, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _EngBuffInitPos; 
	StringToChar(pDoc->WorkingInfo.Motion.sEngBuffInitPos, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _EngBuffCurrPos; 
	StringToChar(pDoc->WorkingInfo.Motion.sEngBuffCurrPos, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);*/
}

void CEngrave::SetFdInfo()
{
	SetFdVel();
	SetFdAcc();
	SetOnePnlVel();
	SetOnePnlAcc();
	SetFdDiffMax();
	SetFdDiffRng();
	SetFdDiffNum();

/*	char cData[BUFFER_DATA_SIZE];
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _FdVel; 
	StringToChar(pDoc->WorkingInfo.Motion.sMkFdVel, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _FdAcc; 
	StringToChar(pDoc->WorkingInfo.Motion.sMkFdAcc, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _OnePnlLen;
	sprintf(SocketData.strData, "%.3f", pDoc->GetOnePnlLen());
	SendCommand(SocketData);

	SocketData.nMsgID = _OnePnlVel;
	sprintf(SocketData.strData, "%.3f", pDoc->GetOnePnlVel());
	SendCommand(SocketData);

	SocketData.nMsgID = _OnePnlAcc;
	sprintf(SocketData.strData, "%.3f", pDoc->GetOnePnlAcc());
	SendCommand(SocketData);

	SocketData.nMsgID = _FdDiffMax;
	sprintf(SocketData.strData, "%.3f", pDoc->GetFdErrLmt());
	SendCommand(SocketData);

	SocketData.nMsgID = _FdDiffRng;
	sprintf(SocketData.strData, "%.3f", pDoc->GetFdErrRng());
	SendCommand(SocketData);

	SocketData.nMsgID = _FdDiffNum;
	sprintf(SocketData.strData, "%.3f", pDoc->GetFdErrNum());
	SendCommand(SocketData);*/
}

void CEngrave::SetAoiInfo()
{
	SetAoiLeadPitch();
	SetAoiPushOffLen();
	SetAoiTqVal();
	SetAoiBuffShotNum();
	SetAoiMkLen();

/*	char cData[BUFFER_DATA_SIZE];
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _AoiLeadPitch; 
	StringToChar(pDoc->WorkingInfo.Motion.sAoiFdLead, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _AoiPushOffLen; 
	StringToChar(pDoc->WorkingInfo.Motion.sAoiFdVacOff, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _AoiTqVal; 
	StringToChar(pDoc->WorkingInfo.Motion.sAoiTq, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _AoiBuffShotNum; 
	StringToChar(pDoc->WorkingInfo.Motion.sFdAoiAoiDistShot, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _AoiMkLen; 
	StringToChar(pDoc->WorkingInfo.Motion.sFdMkAoiInitDist, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);*/
}

void CEngrave::SetMkInfo()
{
	SetMkLeadPitch();
	SetMkPushOffLen();
	SetMkBuffInitPos();
	SetMkBuffCurrPos();
	SetMkNumLf();
	SetMkNumRt();
	SetMkMaxNumLf();
	SetMkMaxNumRt();

/*	char cData[BUFFER_DATA_SIZE];
	CString str;
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _MkLeadPitch; 
	StringToChar(pDoc->WorkingInfo.Motion.sMkFdLead, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _MkPushOffLen; 
	StringToChar(pDoc->WorkingInfo.Motion.sMkFdVacOff, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _MkPushOffLen; 
	StringToChar(pDoc->WorkingInfo.Motion.sMkTq, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _MkBuffInitPos; 
	StringToChar(pDoc->WorkingInfo.Motion.sStBufPos, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _MkBuffCurrPos;
	double dBufEnc = (double)pDoc->m_pMpeData[0][1] / 1000.0;	// 마킹부 버퍼 엔코더 값(단위 mm * 1000)
	//sprintf(SocketData.strData, "%.1f", dBufEnc);
	SocketData.fData1 = (float)dBufEnc;
	SendCommand(SocketData);

	SocketData.nMsgID = _MkNumLf;
	SocketData.nData1 = pDoc->GetMkCntL();
	//sprintf(SocketData.strData, "%d", pDoc->GetMkCntL());
	SendCommand(SocketData);

	SocketData.nMsgID = _MkNumRt;
	SocketData.nData1 = pDoc->GetMkCntR();
	//sprintf(SocketData.strData, "%d", pDoc->GetMkCntR());
	SendCommand(SocketData);

	SocketData.nMsgID = _MkMaxNumLf;
	SocketData.nData1 =  pDoc->GetMkLimitL();
	//sprintf(SocketData.strData, "%d", pDoc->GetMkLimitL());
	SendCommand(SocketData);

	SocketData.nMsgID = _MkMaxNumRt;
	SocketData.nData1 = pDoc->GetMkLimitR();
	//sprintf(SocketData.strData, "%d", pDoc->GetMkLimitR());
	SendCommand(SocketData);*/
}

void CEngrave::SetMkInfoLf()
{
	SetMkInitPosLf();
	SetMkInitVelLf();
	SetMkInitAccLf();
	SetMkFnlPosLf();
	SetMkFnlVelLf();
	SetMkFnlAccLf();
	SetMkFnlTqLf();
	SetMkHgtPosX1Lf();
	SetMkHgtPosY1Lf();
	SetMkHgtPosX2Lf();
	SetMkHgtPosY2Lf();
	SetMkHgtPosX3Lf();
	SetMkHgtPosY3Lf();
	SetMkHgtPosX4Lf();
	SetMkHgtPosY4Lf();

/*	char cData[BUFFER_DATA_SIZE];
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _MkInitPosLf; 
	StringToChar(pDoc->WorkingInfo.Marking[0].sWaitPos, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _MkInitVelLf; 
	StringToChar(pDoc->WorkingInfo.Marking[0].sWaitVel, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _MkInitAccLf; 
	StringToChar(pDoc->WorkingInfo.Marking[0].sWaitAcc, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _MkFnlPosLf; 
	StringToChar(pDoc->WorkingInfo.Marking[0].sMarkingPos, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _MkFnlVelLf; 
	StringToChar(pDoc->WorkingInfo.Marking[0].sMarkingVel, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _MkFnlAccLf; 
	StringToChar(pDoc->WorkingInfo.Marking[0].sMarkingAcc, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _MkFnlTqLf; 
	StringToChar(pDoc->WorkingInfo.Marking[0].sMarkingToq, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _MkHgtPosX1Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX1_1());
	SendCommand(SocketData);

	SocketData.nMsgID = _MkHgtPosY1Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY1_1());
	SendCommand(SocketData);

	SocketData.nMsgID = _MkHgtPosX2Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX1_2());
	SendCommand(SocketData);

	SocketData.nMsgID = _MkHgtPosY2Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY1_2());
	SendCommand(SocketData);

	SocketData.nMsgID = _MkHgtPosX3Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX1_3());
	SendCommand(SocketData);

	SocketData.nMsgID = _MkHgtPosY3Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY1_3());
	SendCommand(SocketData);

	SocketData.nMsgID = _MkHgtPosX4Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX1_4());
	SendCommand(SocketData);

	SocketData.nMsgID = _MkHgtPosY4Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY1_4());
	SendCommand(SocketData);*/
}

void CEngrave::SetMkInfoRt()
{
	SetMkInitPosRt();
	SetMkInitVelRt();
	SetMkInitAccRt();
	SetMkFnlPosRt();
	SetMkFnlVelRt();
	SetMkFnlAccRt();
	SetMkFnlTqRt();
	SetMkHgtPosX1Rt();
	SetMkHgtPosY1Rt();
	SetMkHgtPosX2Rt();
	SetMkHgtPosY2Rt();
	SetMkHgtPosX3Rt();
	SetMkHgtPosY3Rt();
	SetMkHgtPosX4Rt();
	SetMkHgtPosY4Rt();

/*	char cData[BUFFER_DATA_SIZE];
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _MkInitPosRt; 
	StringToChar(pDoc->WorkingInfo.Marking[1].sWaitPos, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _MkInitVelRt; 
	StringToChar(pDoc->WorkingInfo.Marking[1].sWaitVel, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _MkInitAccRt; 
	StringToChar(pDoc->WorkingInfo.Marking[1].sWaitAcc, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _MkFnlPosRt; 
	StringToChar(pDoc->WorkingInfo.Marking[1].sMarkingPos, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _MkFnlVelRt; 
	StringToChar(pDoc->WorkingInfo.Marking[1].sMarkingVel, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _MkFnlAccRt; 
	StringToChar(pDoc->WorkingInfo.Marking[1].sMarkingAcc, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _MkFnlTqRt; 
	StringToChar(pDoc->WorkingInfo.Marking[1].sMarkingToq, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);

	SocketData.nMsgID = _MkHgtPosX1Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX2_1());
	SendCommand(SocketData);

	SocketData.nMsgID = _MkHgtPosY1Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY2_1());
	SendCommand(SocketData);

	SocketData.nMsgID = _MkHgtPosX2Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX2_2());
	SendCommand(SocketData);

	SocketData.nMsgID = _MkHgtPosY2Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY2_2());
	SendCommand(SocketData);

	SocketData.nMsgID = _MkHgtPosX3Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX2_3());
	SendCommand(SocketData);

	SocketData.nMsgID = _MkHgtPosY3Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY2_3());
	SendCommand(SocketData);

	SocketData.nMsgID = _MkHgtPosX4Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX2_4());
	SendCommand(SocketData);

	SocketData.nMsgID = _MkHgtPosY4Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY2_4());
	SendCommand(SocketData);*/
}

// SetOpInfo()
void CEngrave::SetOpName()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _OpName;
	StringToChar(pDoc->WorkingInfo.LastJob.sSelUserName, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetDualTest()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _DualTest;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bDualTest ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetSampleTest()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _SampleTest;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bSampleTest ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetSampleShotNum()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _SampleShotNum;
	SocketData.nData1 = _ttoi(pDoc->WorkingInfo.LastJob.sSampleTestShotNum);
	SendCommand(SocketData);
}

void CEngrave::SetTestMode()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _TestMode;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.nTestMode; // MODE_NONE = 0, MODE_INNER = 1, MODE_OUTER = 2
	SendCommand(SocketData);
}

void CEngrave::SetRecoilerCcw()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _RecoilerCcw;						// OneMetal : TRUE -> SetTwoMetal(FALSE);
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bOneMetal ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetUncoilerCcw()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _UncoilerCcw;						// TwoMetal : TRUE -> SetTwoMetal(TRUE);
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bTwoMetal ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetAlignMethode()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _AlignMethode;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.nAlignMethode; // TWO_POINT, FOUR_POINT
	SendCommand(SocketData);
}

void CEngrave::SetDoorRecoiler()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _DoorRecoiler;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bRclDrSen ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDoorAoiUp()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _DoorAoiUp;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bAoiUpDrSen ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDoorAoiDn()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _DoorAoiDn;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bAoiDnDrSen ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDoorMk()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _DoorMk;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bMkDrSen ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDoorEngrave()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _DoorEngrave;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bEngvDrSen ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDoorUncoiler()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _DoorUncoiler;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bUclDrSen ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetSaftyMk()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _SaftyMk;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bMkSftySen ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetCleannerAoiUp()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _CleannerAoiUp;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bUseAoiUpCleanRoler ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetCleannerAoiDn()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _CleannerAoiDn;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bUseAoiDnCleanRoler ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetUltraSonicAoiDn()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _UltraSonicAoiDn;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bUseAoiDnCleanner ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetUltraSonicEngrave()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _UltraSonicEngrave;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bUseEngraveCleanner ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetTotReelLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _TotReelLen;
	StringToChar(pDoc->WorkingInfo.LastJob.sReelTotLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetOnePnlLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _OnePnlLen;
	sprintf(SocketData.strData, "%.3f", pDoc->GetOnePnlLen());
	SendCommand(SocketData);
}

void CEngrave::SetTempPause()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _TempPause;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bTempPause ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetTempStopLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _TempStopLen;
	StringToChar(pDoc->WorkingInfo.LastJob.sTempPauseLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetLotCut()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _LotCut;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bLotSep ? 1 : 0;	// pDoc->m_pReelMap->m_bUseLotSep
	SendCommand(SocketData);
}

void CEngrave::SetLotCutLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _LotCutLen;
	StringToChar(pDoc->WorkingInfo.LastJob.sLotSepLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetLotCutPosLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _LotCutPosLen;
	StringToChar(pDoc->WorkingInfo.LastJob.sLotCutPosLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetLmtTotYld()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _LmtTotYld;
	StringToChar(pDoc->WorkingInfo.LastJob.sLmtTotYld, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetLmtPatlYld()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _LmtPatlYld;
	StringToChar(pDoc->WorkingInfo.LastJob.sLmtPatlYld, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetStripOutRatio()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _StripOutRatio;
	StringToChar(pDoc->WorkingInfo.LastJob.sStripOutRatio, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetCustomNeedRatio()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _CustomNeedRatio;
	StringToChar(pDoc->WorkingInfo.LastJob.sCustomNeedRatio, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetNumRangeFixDef()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _NumRangeFixDef;
	StringToChar(pDoc->WorkingInfo.LastJob.sNumRangeFixDef, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetNumContFixDef()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _NumContFixDef;
	StringToChar(pDoc->WorkingInfo.LastJob.sNumContFixDef, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetUltraSonicStTim()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _UltraSonicStTim;
	StringToChar(pDoc->WorkingInfo.LastJob.sUltraSonicCleannerStTim, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

// SetInfo()
void CEngrave::SetModelUpName()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _ModelUpName;
	StringToChar(pDoc->WorkingInfo.LastJob.sModelUp, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetModelDnName()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _ModelDnName;
	StringToChar(pDoc->WorkingInfo.LastJob.sModelDn, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetLotUpName()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _LotUpName;
	StringToChar(pDoc->WorkingInfo.LastJob.sLotUp, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetLotDnName()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _LotDnName;
	StringToChar(pDoc->WorkingInfo.LastJob.sLotDn, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetLayerUpName()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _LayerUpName;
	StringToChar(pDoc->WorkingInfo.LastJob.sLayerUp, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetLayerDnName()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _LayerDnName;
	StringToChar(pDoc->WorkingInfo.LastJob.sLayerDn, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetOrderNum()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _OrderNum;
	StringToChar(pDoc->WorkingInfo.LastJob.sEngraveOrderNum, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetShotNum()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _ShotNum;
	StringToChar(pDoc->WorkingInfo.LastJob.sEngraveLastShot, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetTotOpRto()
{
	if (!pDoc || !pView || !pView->m_pDlgFrameHigh)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];
	CString sVal;

	SocketData.nMsgID = _TotOpRto;
	double dFdTotLen = (double)pView->m_pDlgFrameHigh->m_nMkLastShot * _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen);
	sVal.Format(_T("%d"), (int)(dFdTotLen / _tstof(pDoc->WorkingInfo.LastJob.sReelTotLen) * 100.0));
	StringToChar(sVal, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetTotVel()
{
	if (!pView)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];
	CString sVal;

	SocketData.nMsgID = _TotVel;
	sVal.Format(_T("%.1f"), pView->GetTotVel());
	StringToChar(sVal, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetPartVel()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _PartVel;
	StringToChar(pDoc->WorkingInfo.LastJob.sPartialSpd, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkDoneLen()
{
	if (!pDoc || !pView || !pView->m_pDlgFrameHigh)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];
	CString sVal;

	SocketData.nMsgID = _MkDoneLen;
	double dFdTotLen = (double)pView->m_pDlgFrameHigh->m_nMkLastShot * _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen);
	sVal.Format(_T("%.2f"), dFdTotLen / 1000.0);	// [M]
	StringToChar(sVal, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetAoiDnDoneLen()
{
	if (!pView)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];
	CString sVal;

	SocketData.nMsgID = _AoiDnDoneLen;
	sVal.Format(_T("%.2f"), pView->GetAoiDnFdLen() / 1000.0);	// [M]
	StringToChar(sVal, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetAoiUpDoneLen()
{
	if (!pView)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];
	CString sVal;

	SocketData.nMsgID = _AoiUpDoneLen;
	sVal.Format(_T("%.2f"), pView->GetAoiUpFdLen() / 1000.0);	// [M]
	StringToChar(sVal, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetLotSerial()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _LotSerial;
	StringToChar(pDoc->WorkingInfo.LastJob.sLotSerial, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkVerfyLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _MkVerfyLen;
	StringToChar(pDoc->WorkingInfo.LastJob.sVerifyLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

// SetTotRatio()
void CEngrave::SetDefNumUp()
{
	if (!pDoc || !pDoc->m_pReelMapUp)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;

	
	pDoc->m_pReelMapUp->GetPcsNum(nGood, nBad); // 상면
	nTot = nGood + nBad;

	SocketData.nMsgID = _DefNumUp;
	SocketData.nData1 = nBad;
	SendCommand(SocketData);
}

void CEngrave::SetDefRtoUp()
{
	if (!pDoc || !pDoc->m_pReelMapUp)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;
	double dRatio = 0.0;
	
	pDoc->m_pReelMapUp->GetPcsNum(nGood, nBad); // 상면

	if (nTot > 0)
		dRatio = ((double)nBad / (double)nTot) * 100.0;
	else
		dRatio = 0.0;

	SocketData.nMsgID = _DefRtoUp;
	SocketData.fData1 = dRatio;
	SendCommand(SocketData);
}

void CEngrave::SetGoodNumUp()
{
	if (!pDoc || !pDoc->m_pReelMapUp)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0;
	
	pDoc->m_pReelMapUp->GetPcsNum(nGood, nBad); // 상면

	SocketData.nMsgID = _GoodNumUp;
	SocketData.nData1 = nGood;
	SendCommand(SocketData);
}

void CEngrave::SetGoodRtoUp()
{
	if (!pDoc || !pDoc->m_pReelMapUp)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;
	double dRatio = 0.0;

	pDoc->m_pReelMapUp->GetPcsNum(nGood, nBad); // 상면

	if (nTot > 0)
		dRatio = ((double)nGood / (double)nTot) * 100.0;
	else
		dRatio = 0.0;

	SocketData.nMsgID = _GoodRtoUp;
	SocketData.fData1 = dRatio;
	SendCommand(SocketData);
}

void CEngrave::SetTestNumUp()
{
	if (!pDoc || !pDoc->m_pReelMapUp)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;

	pDoc->m_pReelMapUp->GetPcsNum(nGood, nBad); // 상면
	nTot = nGood + nBad;

	SocketData.nMsgID = _TestNumUp;
	SocketData.nData1 = nTot;
	SendCommand(SocketData);
}

void CEngrave::SetDefNumDn()
{
	if (!pDoc || !pDoc->m_pReelMapDn)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{		
		pDoc->m_pReelMapDn->GetPcsNum(nGood, nBad); // 하면
		nTot = nGood + nBad;

		SocketData.nMsgID = _DefNumDn;
		SocketData.nData1 = nBad;
		SendCommand(SocketData);
	}
}

void CEngrave::SetDefRtoDn()
{
	if (!pDoc || !pDoc->m_pReelMapDn)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;
	double dRatio = 0.0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		pDoc->m_pReelMapDn->GetPcsNum(nGood, nBad); // 하면
		nTot = nGood + nBad;

		if (nTot > 0)
			dRatio = ((double)nBad / (double)nTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _DefRtoDn;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);
	}
}

void CEngrave::SetGoodNumDn()
{
	if (!pDoc || !pDoc->m_pReelMapDn)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		pDoc->m_pReelMapDn->GetPcsNum(nGood, nBad); // 하면

		SocketData.nMsgID = _GoodNumDn;
		SocketData.nData1 = nGood;
		SendCommand(SocketData);
	}
}

void CEngrave::SetGoodRtoDn()
{
	if (!pDoc || !pDoc->m_pReelMapDn)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;
	double dRatio = 0.0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		pDoc->m_pReelMapDn->GetPcsNum(nGood, nBad); // 하면
		nTot = nGood + nBad;

		if (nTot > 0)
			dRatio = ((double)nGood / (double)nTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _GoodRtoDn;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);
	}
}

void CEngrave::SetTestNumDn()
{
	if (!pDoc || !pDoc->m_pReelMapDn)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		pDoc->m_pReelMapDn->GetPcsNum(nGood, nBad); // 하면
		nTot = nGood + nBad;

		SocketData.nMsgID = _TestNumDn;
		SocketData.nData1 = nTot;
		SendCommand(SocketData);
	}
}

void CEngrave::SetDefNumTot()
{
	if (!pDoc || !pDoc->m_pReelMapDn)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{		
		pDoc->m_pReelMapAllDn->GetPcsNum(nGood, nBad); // 전체
		nTot = nGood + nBad;

		SocketData.nMsgID = _DefNumTot;
		SocketData.nData1 = nTot;
		SendCommand(SocketData);
	}
}

void CEngrave::SetDefRtoTot()
{
	if (!pDoc || !pDoc->m_pReelMapDn)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;
	double dRatio = 0.0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		pDoc->m_pReelMapAllDn->GetPcsNum(nGood, nBad); // 전체
		nTot = nGood + nBad;

		if (nTot > 0)
			dRatio = ((double)nBad / (double)nTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _DefRtoTot;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);
	}
}

void CEngrave::SetGoodNumTot()
{
	if (!pDoc || !pDoc->m_pReelMapDn)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		pDoc->m_pReelMapAllDn->GetPcsNum(nGood, nBad); // 전체

		SocketData.nMsgID = _GoodNumTot;
		SocketData.nData1 = nGood;
		SendCommand(SocketData);
	}
}

void CEngrave::SetGoodRtoTot()
{
	if (!pDoc || !pDoc->m_pReelMapDn)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;
	double dRatio = 0.0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		pDoc->m_pReelMapAllDn->GetPcsNum(nGood, nBad); // 전체
		nTot = nGood + nBad;

		if (nTot > 0)
			dRatio = ((double)nGood / (double)nTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _GoodRtoTot;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);
	}
}

void CEngrave::SetTestNumTot()
{
	if (!pDoc || !pDoc->m_pReelMapDn)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		pDoc->m_pReelMapAllDn->GetPcsNum(nGood, nBad); // 전체
		nTot = nGood + nBad;

		SocketData.nMsgID = _TestNumTot;
		SocketData.nData1 = nTot;
		SendCommand(SocketData);
	}
}

// Set2DReader()
void CEngrave::Set2DEngLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _2DEngLen;
	StringToChar(pDoc->WorkingInfo.Motion.s2DEngLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::Set2DAoiLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _2DAoiLen;
	StringToChar(pDoc->WorkingInfo.Motion.s2DAoiLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::Set2DMkLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _2DMkLen;
	StringToChar(pDoc->WorkingInfo.Motion.s2DMkLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::Set2DMoveVel()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _2DMoveVel;
	StringToChar(pDoc->WorkingInfo.Motion.s2DMoveVel, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::Set2DMoveAcc()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _2DMoveAcc;
	StringToChar(pDoc->WorkingInfo.Motion.s2DMoveAcc, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::Set2DOneShotLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _2DOneShotLen;
	StringToChar(pDoc->WorkingInfo.Motion.s2DOneShotRemainLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

// SetEngInfo()
void CEngrave::SetEngLeadPitch()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _EngLeadPitch;
	StringToChar(pDoc->WorkingInfo.Motion.sEngraveFdLead, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetEngPushOffLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _EngPushOffLen;
	StringToChar(pDoc->WorkingInfo.Motion.sEngraveFdVacOff, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetEngTqVal()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _EngTqVal;
	StringToChar(pDoc->WorkingInfo.Motion.sEngraveTq, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetEngAoiLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _EngAoiLen;
	StringToChar(pDoc->WorkingInfo.Motion.sEngAoiLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetEngFdDiffMax()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _EngFdDiffMax;
	StringToChar(pDoc->WorkingInfo.Motion.sEngFdDiffMax, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetEngFdDiffRng()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _EngFdDiffRng;
	StringToChar(pDoc->WorkingInfo.Motion.sEngFdDiffRng, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetEngFdDiffNum()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _EngFdDiffNum;
	StringToChar(pDoc->WorkingInfo.Motion.sEngFdDiffNum, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetEngBuffInitPos()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _EngBuffInitPos;
	StringToChar(pDoc->WorkingInfo.Motion.sEngBuffInitPos, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetEngBuffCurrPos()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _EngBuffCurrPos;
	StringToChar(pDoc->WorkingInfo.Motion.sEngBuffCurrPos, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

// SetFdInfo()
void CEngrave::SetFdVel()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _FdVel;
	StringToChar(pDoc->WorkingInfo.Motion.sMkFdVel, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetFdAcc()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _FdAcc;
	StringToChar(pDoc->WorkingInfo.Motion.sMkFdAcc, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetOnePnlVel()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _OnePnlVel;
	sprintf(SocketData.strData, "%.3f", pDoc->GetOnePnlVel());
	SendCommand(SocketData);
}

void CEngrave::SetOnePnlAcc()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _OnePnlAcc;
	sprintf(SocketData.strData, "%.3f", pDoc->GetOnePnlAcc());
	SendCommand(SocketData);
}

void CEngrave::SetFdDiffMax()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _FdDiffMax;
	sprintf(SocketData.strData, "%.3f", pDoc->GetFdErrLmt());
	SendCommand(SocketData);
}

void CEngrave::SetFdDiffRng()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _FdDiffRng;
	sprintf(SocketData.strData, "%.3f", pDoc->GetFdErrRng());
	SendCommand(SocketData);
}

void CEngrave::SetFdDiffNum()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _FdDiffNum;
	sprintf(SocketData.strData, "%.3f", pDoc->GetFdErrNum());
	SendCommand(SocketData);
}

// SetAoiInfo()
void CEngrave::SetAoiLeadPitch()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _AoiLeadPitch;
	StringToChar(pDoc->WorkingInfo.Motion.sAoiFdLead, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetAoiPushOffLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _AoiPushOffLen;
	StringToChar(pDoc->WorkingInfo.Motion.sAoiFdVacOff, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetAoiTqVal()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _AoiTqVal;
	StringToChar(pDoc->WorkingInfo.Motion.sAoiTq, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetAoiBuffShotNum()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _AoiBuffShotNum;
	StringToChar(pDoc->WorkingInfo.Motion.sFdAoiAoiDistShot, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetAoiMkLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _AoiMkLen;
	StringToChar(pDoc->WorkingInfo.Motion.sFdMkAoiInitDist, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

// SetMkInfo()
void CEngrave::SetMkLeadPitch()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _MkLeadPitch;
	StringToChar(pDoc->WorkingInfo.Motion.sMkFdLead, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkPushOffLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _MkPushOffLen;
	StringToChar(pDoc->WorkingInfo.Motion.sMkFdVacOff, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkBuffInitPos()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _MkBuffInitPos;
	StringToChar(pDoc->WorkingInfo.Motion.sStBufPos, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkBuffCurrPos()
{
	if (!pDoc || !pDoc->m_pMpeData)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _MkBuffCurrPos;
	double dBufEnc = (double)pDoc->m_pMpeData[0][1] / 1000.0;	// 마킹부 버퍼 엔코더 값(단위 mm * 1000)
																//sprintf(SocketData.strData, "%.1f", dBufEnc);
	SocketData.fData1 = (float)dBufEnc;
	SendCommand(SocketData);
}

void CEngrave::SetMkNumLf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _MkNumLf;
	SocketData.nData1 = pDoc->GetMkCntL();
	//sprintf(SocketData.strData, "%d", pDoc->GetMkCntL());
	SendCommand(SocketData);
}

void CEngrave::SetMkNumRt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _MkNumRt;
	SocketData.nData1 = pDoc->GetMkCntR();
	//sprintf(SocketData.strData, "%d", pDoc->GetMkCntR());
	SendCommand(SocketData);
}

void CEngrave::SetMkMaxNumLf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _MkMaxNumLf;
	SocketData.nData1 = pDoc->GetMkLimitL();
	//sprintf(SocketData.strData, "%d", pDoc->GetMkLimitL());
	SendCommand(SocketData);
}

void CEngrave::SetMkMaxNumRt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _MkMaxNumRt;
	SocketData.nData1 = pDoc->GetMkLimitR();
	//sprintf(SocketData.strData, "%d", pDoc->GetMkLimitR());
	SendCommand(SocketData);
}

// SetMkInfoLf()
void CEngrave::SetMkInitPosLf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _MkInitPosLf;
	StringToChar(pDoc->WorkingInfo.Marking[0].sWaitPos, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkInitVelLf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _MkInitVelLf;
	StringToChar(pDoc->WorkingInfo.Marking[0].sWaitVel, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkInitAccLf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _MkInitAccLf;
	StringToChar(pDoc->WorkingInfo.Marking[0].sWaitAcc, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkFnlPosLf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _MkFnlPosLf;
	StringToChar(pDoc->WorkingInfo.Marking[0].sMarkingPos, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkFnlVelLf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _MkFnlVelLf;
	StringToChar(pDoc->WorkingInfo.Marking[0].sMarkingVel, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkFnlAccLf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _MkFnlAccLf;
	StringToChar(pDoc->WorkingInfo.Marking[0].sMarkingAcc, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkFnlTqLf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _MkFnlTqLf;
	StringToChar(pDoc->WorkingInfo.Marking[0].sMarkingToq, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosX1Lf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _MkHgtPosX1Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX1_1());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosY1Lf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _MkHgtPosY1Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY1_1());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosX2Lf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _MkHgtPosX2Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX1_2());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosY2Lf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _MkHgtPosY2Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY1_2());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosX3Lf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _MkHgtPosX3Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX1_3());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosY3Lf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _MkHgtPosY3Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY1_3());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosX4Lf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _MkHgtPosX4Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX1_4());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosY4Lf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _MkHgtPosY4Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY1_4());
	SendCommand(SocketData);
}

// SetMkInfoRt()
void CEngrave::SetMkInitPosRt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _MkInitPosRt;
	StringToChar(pDoc->WorkingInfo.Marking[1].sWaitPos, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkInitVelRt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _MkInitVelRt;
	StringToChar(pDoc->WorkingInfo.Marking[1].sWaitVel, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkInitAccRt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _MkInitAccRt;
	StringToChar(pDoc->WorkingInfo.Marking[1].sWaitAcc, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkFnlPosRt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _MkFnlPosRt;
	StringToChar(pDoc->WorkingInfo.Marking[1].sMarkingPos, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkFnlVelRt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _MkFnlVelRt;
	StringToChar(pDoc->WorkingInfo.Marking[1].sMarkingVel, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkFnlAccRt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _MkFnlAccRt;
	StringToChar(pDoc->WorkingInfo.Marking[1].sMarkingAcc, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkFnlTqRt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _MkFnlTqRt;
	StringToChar(pDoc->WorkingInfo.Marking[1].sMarkingToq, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosX1Rt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _MkHgtPosX1Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX2_1());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosY1Rt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _MkHgtPosY1Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY2_1());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosX2Rt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _MkHgtPosX2Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX2_2());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosY2Rt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _MkHgtPosY2Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY2_2());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosX3Rt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _MkHgtPosX3Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX2_3());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosY3Rt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _MkHgtPosY3Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY2_3());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosX4Rt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _MkHgtPosX4Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX2_4());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosY4Rt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _MkHgtPosY4Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY2_4());
	SendCommand(SocketData);
}

// End for SetSysData()


// Start Switch

// Main
void CEngrave::SwReady(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _Ready;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRun(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _Run;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwReset(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _Reset;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwStop(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _Stop;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAuto(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _Auto;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwManual(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _Manual;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

// Torque Motor
void CEngrave::SwMkTq(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _MkTq;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiTq(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _AoiTq;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngTq(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _EngTq;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

// Induction Motor
void CEngrave::SwRcInductionCcw(BOOL bOn)	// SetOneMetal
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _CcwModRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcInductionCcw(BOOL bOn)	// SetTwoMetal
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _CcwModUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

// Core 150mm
void CEngrave::SwRcCore150mm(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _Core150Re;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcCore150mm(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _Core150Un;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

// Recoiler
void CEngrave::SwRcRelation(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _Relation;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcFdCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _MvCwRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcFdCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _MvCcwRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcReelChuck(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _PrdChuckRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcDcRlUpDn(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _DancerUpRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcReelJoinL(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _PasteUpLfRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcReelJoinR(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _PasteUpRtRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcReelJoinVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _PasteVacRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcPaperChuck(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _PprChuckRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcPaperCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _PprCwRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcPaperCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _PprCcwRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcRewinder(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _DoRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcRewinderReelPaper(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _PrdPprRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}


// Punching
void CEngrave::SwMkRelation(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _Relation;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwMkFdCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _MvCwMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwMkFdCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _MvCcwMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwMkFdVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _FdVacMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwMkPushUp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _PushUpMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwMkTblBlw(BOOL bOn)	
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _TblBlwMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwMkTblVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _TblVacMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwMkFdClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _FdClampMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwMkTqClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _TensClampMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwMkMvOne(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _OnePnlMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwMkLsrPt(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _LsrMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwMkDcRSol(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _DancerUpMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

// AOIDn
void CEngrave::SwAoiDnRelation(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _Relation;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnFdCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _MvCwAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnFdCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _MvCcwAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnFdVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _FdVacAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnPushUp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _PushUpAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnTblBlw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _TblBlwAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnTblVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _TblVacAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnFdClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _FdClampAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnTqClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _TensClampAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnMvOne(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _OnePnlAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnLsrPt(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _LsrAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}


void CEngrave::SwAoiDnClrRoll(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _ClrRollAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnSonicBlw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _ClrSonicAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}


void CEngrave::SwAoiDnTest(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _TestAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnReset(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _ResetAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnLotEnd(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _LotEndAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}



// AOIUp
void CEngrave::SwAoiUpRelation(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _Relation;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpFdCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _MvCwAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpFdCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _MvCcwAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpFdVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _FdVacAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpPushUp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _PushUpAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpTblBlw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _TblBlwAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpTblVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _TblVacAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpFdClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _FdClampAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpTqClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _TensClampAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpMvOne(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _OnePnlAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpLsrPt(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _LsrAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpClrRoll(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _ClrRollAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}


void CEngrave::SwAoiUpTest(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _TestAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpReset(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _ResetAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpLotEnd(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _LotEndAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}


// Engrave
void CEngrave::SwEngRelation(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _Relation;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngFdCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _MvCwEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngFdCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _MvCcwEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngFdVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _FdVacEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngPushUp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _PushUpEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngTblBlw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _TblBlwEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngTblVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _TblVacEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngFdClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _FdClampEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngTqClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _TensClampEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngMvOne(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _OnePnlEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngLsrPt(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _LsrEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngDcRSol(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _DancerUpEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngSonicBlw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _ClrSonicEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}


// Uncoiler
void CEngrave::SwUcRelation(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _Relation;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcFdCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _MvCwUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcFdCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _MvCcwUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcReelChuck(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _PrdChuckUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcDcRlUpDn(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _DancerUpUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcReelJoinL(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _PasteUpLfUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcReelJoinR(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _PasteUpRtUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcReelJoinVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _PasteVacUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcPaperChuck(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _PprChuckUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcPaperCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _PprCwUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcPaperCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _PprCcwUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcClRlUpDn(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _ClrRollUpUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcClRlPshUpDn(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _ClrRollPushUn; 
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}


// Etc
void CEngrave::SwAoiEmg(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _EmgAoi;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

// End Switch

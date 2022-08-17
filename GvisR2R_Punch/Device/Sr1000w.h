#pragma once

#include "afxwin.h"
#include "TcpIpClient.h"


class CSr1000w : public CWnd
{
	HWND m_hParentWnd;
	CTcpIpClient* m_pClient;

	CString	m_strPortSvr, m_strAddrSvr;
	CString	m_strAddrCli;
	int m_nServerID;

	BOOL m_bWaitForResponse;
	CString m_strResponse;
	int m_nCmd;
	BOOL m_bCmdF;

	void StartClient(CString sAddrCli, CString sAddrSvr, CString sPortSvr);
	void StopClient();
	BOOL CheckResponse(int nCmd, CString sResponse);

public:
	CSr1000w(CString sAddrCli, CString sAddrSvr, CString sPortSvr, CWnd* pParent=NULL);
	~CSr1000w();

	void SetHwnd(HWND hParentWnd);
	BOOL Get2DCode(CString &sData);			// SR1000W - Response
	BOOL IsRunning();
	BOOL DoRead2DCode();
	int WriteComm(CString sMsg, DWORD dwTimeout = INFINITE);

	// Thread
	CEvent      m_evtThread;
	CWinThread *m_pThread;
	HANDLE		m_hThread;
	BOOL		m_bModify;
	BOOL		m_bAlive;

	void StartThread();
	void StopThread();
	void WaitUntilThreadEnd(HANDLE hThread);
	static UINT RunThread(LPVOID pParam);
	void Close();

	int OnClientReceived(WPARAM wParam, LPARAM lParam);

protected:
	afx_msg LRESULT wmClientReceived(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT wmClientClosed(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};


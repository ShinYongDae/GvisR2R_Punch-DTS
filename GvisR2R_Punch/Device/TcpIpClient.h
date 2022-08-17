#pragma once

#include "CEasyThread.h"
#include "SocketComm.h"

class CTcpIpClient : public CSocketComm //, public CEasyThread
{
	HWND m_hParentWnd;
	CWnd* m_pParent;
	//BOOL m_bConnected;	// Connection is done.
	CString m_strServerIP;
	CString m_strClientIP;
	int m_nBufferId;
	CString m_strReceived;

	BYTE* m_pReceiveBuffer;
	BYTE* m_pCurrentBuffer;

	int ReadComm();
	int ReadCommData(SOCKET_DATA &SocketData, DWORD dwSize, DWORD dwTimeout);
	int ReadPacketData(SOCKET_DATA &SocketData, DWORD dwSize, DWORD dwTimeout);

public:
	CTcpIpClient(CWnd* pParent = NULL);
	virtual ~CTcpIpClient();
	virtual void OnEvent(UINT uEvent);

	CRITICAL_SECTION m_sc;
	int m_nNetworkPort;
	BOOL m_bStop;

	void Init(CString strClientIP, CString strServerIP, int nPort);
	BOOL Start();
	BOOL Stop();

	SOCKET_DATA GetSocketData();

	virtual int Running();
	virtual void OnDataReceived();


	// Thread
	CEvent      m_evtThread;
	CWinThread *m_pThread;
	HANDLE		m_hThread;
	BOOL		m_bModify;
	BOOL		m_bAlive;

	void StopThread();
	void WaitUntilThreadEnd(HANDLE hThread);

protected:
	static UINT WINAPI SocketThreadProc(LPVOID pParam);


};


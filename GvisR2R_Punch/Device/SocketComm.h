///////////////////////////////////////////////////////////////////////////////
// FILE : SocketComm.h
// Header file for CSocketComm class
// CSocketComm
//     Generic class for Socket Communication
///////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef _SOCKETCOMM_H_
#define _SOCKETCOMM_H_
#if _MSC_VER > 1000

#endif // _MSC_VER > 1000

/* Windows */
//#define _CRT_SECURE_NO_DEPRECATE
#include <winsock2.h>
#include <WS2tcpip.h>
#pragma comment (lib, "Ws2_32.lib")
#pragma warning(disable:4996) 


#include <vector>
#include <list>
#include <cstdlib>

#define BUFFER_DATA_SIZE	16384

typedef struct tagSocketData {
	int nMsgLen;			// Message Length 
	int nMsgID;				// message ID
	int nTxPC;				// Transmit PC Id
	int nRxPC;				// Message Receive PC
	int nCmdCode;			// Command Code
	int nData1;				// Integer Param1
	int nData2;				// Integer Param2
	int nData3;				// Integer Param3
	int nData4;				// Integer Param4
	float fData1;			// float Param1
	float fData2;			// float Param2
	float fData3;			// float Param3
	float fData4;			// float Param4
	char  strData[BUFFER_DATA_SIZE];

	tagSocketData()
	{
		nMsgLen = 0;			// Message Length 
		nMsgID = 0;				// message ID
		nTxPC = 0;				// Transmit PC Id
		nRxPC = 0;				// Message Receive PC
		nCmdCode = 0;			// Command Code
		nData1 = 0;				// Integer Param1
		nData2 = 0;				// Integer Param2
		nData3 = 0;				// Integer Param3
		nData4 = 0;				// Integer Param4
		fData1 = 0.0;			// float Param1
		fData2 = 0.0;			// float Param2
		fData3 = 0.0;			// float Param3
		fData4 = 0.0;			// float Param4
		memset(strData, 0, sizeof(char)*BUFFER_DATA_SIZE);
	}
}SOCKET_DATA;



#define SOCKET_ERROR (-1)


// Event value
#define EVT_CONSUCCESS		0x0000	// Connection established
#define EVT_CONFAILURE		0x0001	// General failure - Wait Connection failed
#define EVT_CONDROP			0x0002	// Connection dropped
#define EVT_ZEROLENGTH		0x0003	// Zero length message
#define EVT_CONSUCCESS_EX	0x0004	// Connection established

#define BUFFER_DATA_SIZE	15752 //170206 lgh expend
#define HOSTNAME_SIZE	    MAX_PATH
#define STRING_LENGTH	    40
#define PROTOCOL_DEFAULT_LENGTH 52

#define ID_NONE			-1
// Client
#define ID_SR1000W		0	// Device is server.
#define ID_MDX2500		1	// Device is server.
#define ID_PUNCH		2
// Server
#define ID_ENGRAVE		2

#define WM_UPDATE_CONNECTION	(WM_USER+0x1234)

struct SockAddrIn : public SOCKADDR_IN 
{
public:
	SockAddrIn() { Clear(); }
	SockAddrIn(const SockAddrIn& sin) { Copy( sin ); }
	~SockAddrIn() { }
	SockAddrIn& Copy(const SockAddrIn& sin);
	void	Clear() { memset(this, 0, sizeof(SOCKADDR_IN)); }
	bool	IsEqual(const SockAddrIn& sin);
	bool	IsGreater(const SockAddrIn& sin);
	bool	IsLower(const SockAddrIn& pm);
	bool	IsNull() const { return ((sin_addr.s_addr==0L)&&(sin_port==0)); }
	ULONG	GetIPAddr() const { return sin_addr.s_addr; }
	short	GetPort() const { return sin_port; }
	bool	CreateFrom(LPCTSTR sAddr, LPCTSTR sService, int nFamily = AF_INET);
	SockAddrIn& operator=(const SockAddrIn& sin) { return Copy( sin ); }
	bool	operator==(const SockAddrIn& sin) { return IsEqual( sin ); }
	bool	operator!=(const SockAddrIn& sin) { return !IsEqual( sin ); }
	bool	operator<(const SockAddrIn& sin)  { return IsLower( sin ); }
	bool	operator>(const SockAddrIn& sin)  { return IsGreater( sin ); }
	bool	operator<=(const SockAddrIn& sin) { return !IsGreater( sin ); }
	bool	operator>=(const SockAddrIn& sin) { return !IsLower( sin ); }
	operator LPSOCKADDR() { return (LPSOCKADDR)(this); }
	size_t	Size() const { return sizeof(SOCKADDR_IN); }
	void	SetAddr(SOCKADDR_IN* psin) { memcpy(this, psin, Size()); }
};

typedef std::list<SockAddrIn> CSockAddrList;

class CSocketComm
{
	//BOOL m_bConnected;	// Connection is done.
	CString m_strLastError;
	int m_nServer;


	// Create a Socket - Server side
	bool CreateSocket(LPCTSTR strServiceName, int nProtocol);
	bool CreateSocket(CString strIP, LPCTSTR strServiceName, int nFamily);
	void CloseComm();		// Close Socket
	static bool ShutdownConnection(SOCKET sock);  // Shutdown a connection
	CString GetSockErorMsg(int Code);

public:
	CSocketComm();
	virtual ~CSocketComm();

	SOCKET_DATA m_SocketData;

	BOOL IsConnected();
	void SetConnectedStatus(BOOL bConnected);

	bool IsOpen() const;	// Is Socket valid?
	SOCKET GetSocket() const;	// return socket handle
	bool GetSockName(SockAddrIn& saddr_in);	// Get Socket name - address
	bool GetPeerName(SockAddrIn& saddr_in);	// Get Peer Socket name - address
	void StopComm();		// Stop Socket thread

	bool ListenFrom(CString strIP, LPCTSTR strServiceName, int nFamily);							// use on Server (listen)
	static SOCKET WaitForConnection(SOCKET server_sock, sockaddr* acceptIp, int* lenthOfIp);		// use on Server (accept)
	static SOCKET WaitForConnection(SOCKET server_sock, sockaddr_in* acceptIp, int* lenthOfIp);		// use on Server (accept)
	bool ConnectTo(CString strClientIP, LPCTSTR strServerIP, LPCTSTR strServiceName, int nFamily);	// use on Client (connect)

	virtual void OnDataReceived();
	int WriteComm(const LPBYTE lpBuffer, DWORD dwCount, DWORD dwTimeout = INFINITE);
	int WriteComm(CString sMsg, DWORD dwTimeout = INFINITE);

	static USHORT GetPortNumber(LPCTSTR strServiceName);	// Get service port number
	static ULONG GetIPAddress(LPCTSTR strHostName);	// Get IP address of a host
	static bool GetLocalName(LPTSTR strName, UINT nSize);	// GetLocalName
	static bool GetLocalAddress(LPTSTR strAddress, UINT nSize);	// GetLocalAddress

	void StringToChar(CString str, char *szStr);
	CString CharToString(char *szStr);

	DWORD WriteCommData(SOCKET_DATA SocketData, DWORD dwTimeout, DWORD dwStrLen = -1);
	BOOL Shutdown();

protected:
	HANDLE		m_hComm;		// Serial Comm handle
	HANDLE		m_hThread;		// Thread Comm handle
	BOOL		m_bConnected;	// Connection is done.

protected:


public:
	void SetServer(int nServerId = 0);
	int GetServerID();
};

#endif // _SOCKETCOMM_H_


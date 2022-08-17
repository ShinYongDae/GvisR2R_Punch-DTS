///////////////////////////////////////////////////////////////////////////////
//	File:		SocketComm.cpp
//	Version:	1.3
//
//	Author:		Ernest Laurentin
//	E-mail:		elaurentin@netzero.net
//
//	Implementation of the CSocketComm and associated classes.
//
//	This code may be used in compiled form in any way you desire. This
//	file may be redistributed unmodified by any means PROVIDING it is
//	not sold for profit without the authors written consent, and
//	providing that this notice and the authors name and all copyright
//	notices remains intact.
//
//	This file is provided "as is" with no expressed or implied warranty.
//	The author accepts no liability for any damage/loss of business that
//	this c++ class may cause.
//
//	Version history
//
//	1.0	- Initial release.
//	1.1 - Add support for Smart Addressing mode
//  1.2 - Fix various issues with address list (in UDP mode)
//  1.3 - Fix bug when sending message to broadcast address
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "SR1000_Test.h"
#include <winsock.h>
#include "SocketComm.h"

// 디버그창에 메모리 릭위치 출력 정의  
//#ifdef _DEBUG
//#include <crtdbg.h>
//#endif

// C++에서 디버그 출력창에 메모리 릭 위치 알려주는 구문(MFC는 자동생성) 
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


const DWORD DEFAULT_TIMEOUT = 5000L;




///////////////////////////////////////////////////////////////////////////////
// SockAddrIn Struct

///////////////////////////////////////////////////////////////////////////////
// Copy
SockAddrIn& SockAddrIn::Copy(const SockAddrIn& sin)
{
	memcpy(this, &sin, Size());
	return *this;
}

///////////////////////////////////////////////////////////////////////////////
// IsEqual
bool SockAddrIn::IsEqual(const SockAddrIn& sin)
{
	// Is it Equal? - ignore 'sin_zero'
	return (memcmp(this, &sin, Size()-sizeof(sin_zero)) == 0);
}

///////////////////////////////////////////////////////////////////////////////
// IsGreater
bool SockAddrIn::IsGreater(const SockAddrIn& sin)
{
	// Is it Greater? - ignore 'sin_zero'
	return (memcmp(this, &sin, Size()-sizeof(sin_zero)) > 0);
}

///////////////////////////////////////////////////////////////////////////////
// IsLower
bool SockAddrIn::IsLower(const SockAddrIn& sin)
{
	// Is it Lower? - ignore 'sin_zero'
	return (memcmp(this, &sin, Size()-sizeof(sin_zero)) < 0);
}

///////////////////////////////////////////////////////////////////////////////
// CreateFrom
bool SockAddrIn::CreateFrom(LPCTSTR sAddr, LPCTSTR sService, int nFamily /*=AF_INET*/)
{
	Clear();
	sin_addr.s_addr = htonl( CSocketComm::GetIPAddress(sAddr) );
	sin_port = htons( CSocketComm::GetPortNumber( sService ) );
	sin_family = nFamily;
	return !IsNull();
}






BOOL CSocketComm::IsConnected()
{
	return m_bConnected;
}

void CSocketComm::SetConnectedStatus(BOOL bConnected)
{
	m_bConnected = bConnected;
}


///////////////////////////////////////////////////////////////////////////////
// Construct & Destruct
CSocketComm::CSocketComm() :
	m_hComm(INVALID_HANDLE_VALUE),m_hThread(NULL)//, m_hMutex(NULL)
{
	m_bConnected = 0;
	//m_SocketData.nMsgID = 0;	//130529 hjc add
	//m_pRecvBuffer = nullptr;

	m_nServer = ID_NONE;
	m_SocketData.nMsgID = 0;
}

CSocketComm::~CSocketComm()
{
	if (IsOpen())
	{
		StopComm(); 
		Sleep(100);
	}
}

///////////////////////////////////////////////////////////////////////////////
// Members
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// IsOpen
bool CSocketComm::IsOpen() const
{
	return ( INVALID_HANDLE_VALUE != m_hComm );
}


///////////////////////////////////////////////////////////////////////////////
// GetSocket
SOCKET CSocketComm::GetSocket() const
{
	return (SOCKET) m_hComm;
}


///////////////////////////////////////////////////////////////////////////////
// OnDataReceived
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//				This function is PURE Virtual, you MUST overwrite it.  This is
//				called every time new data is available.
// PARAMETERS:
///////////////////////////////////////////////////////////////////////////////
void CSocketComm::OnDataReceived()
{
	int k = 0;
}


///////////////////////////////////////////////////////////////////////////////
// GetPortNumber
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//				Returns a port number based on service name or port number string
// PARAMETERS:
//	LPCTSTR strServiceName: Service name or port string
///////////////////////////////////////////////////////////////////////////////
USHORT CSocketComm::GetPortNumber( LPCTSTR strServiceName )
{
	LPSERVENT   lpservent;
	USHORT      nPortNumber = 0;

	if ( _istdigit( strServiceName[0] ) ) {
		nPortNumber = (USHORT) _ttoi( strServiceName );
	}
	else {
#ifdef _UNICODE
		char pstrService[HOSTNAME_SIZE];
		WideCharToMultiByte(CP_ACP, 0, strServiceName, -1, pstrService, sizeof(pstrService), NULL, NULL );
#else
		LPCTSTR pstrService = strServiceName;
#endif
		// Convert network byte order to host byte order
		if ( (lpservent = getservbyname( pstrService, NULL )) != NULL )
			nPortNumber = ntohs( lpservent->s_port );
	}

	return nPortNumber;
}


///////////////////////////////////////////////////////////////////////////////
// GetIPAddress
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//		Returns an IP address.
//			- It tries to convert the string directly
//			- If that fails, it tries to resolve it as a hostname
// PARAMETERS:
//	LPCTSTR strHostName: host name to get IP address
///////////////////////////////////////////////////////////////////////////////
ULONG CSocketComm::GetIPAddress( LPCTSTR strHostName )
{
	LPHOSTENT	lphostent;
	ULONG		uAddr = INADDR_NONE;
	TCHAR       strLocal[HOSTNAME_SIZE] = { 0 };

	// if no name specified, get local
	if ( NULL == strHostName )
	{
		GetLocalName(strLocal, sizeof(strLocal));
		strHostName = strLocal;
	}

#ifdef _UNICODE
	char strHost[HOSTNAME_SIZE] = { 0 };
	WideCharToMultiByte(CP_ACP, 0, strHostName, -1, strHost, sizeof(strHost), NULL, NULL );
#else
	LPCTSTR strHost = strHostName;
#endif

	// Check for an Internet Protocol dotted address string
	uAddr = inet_addr( strHost );

	if ( (INADDR_NONE == uAddr) && (strcmp( strHost, "255.255.255.255" )) )
	{
		// It's not an address, then try to resolve it as a hostname
		if ( lphostent = gethostbyname( strHost ) )
			uAddr = *((ULONG *) lphostent->h_addr_list[0]);
	}
	
	return ntohl( uAddr );
}


///////////////////////////////////////////////////////////////////////////////
// GetLocalName
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//				Get local computer name.  Something like: "mycomputer.myserver.net"
// PARAMETERS:
//	LPTSTR strName: name of the computer is returned here
//	UINT nSize: size max of buffer "strName"
///////////////////////////////////////////////////////////////////////////////
bool CSocketComm::GetLocalName(LPTSTR strName, UINT nSize)
{
	if (strName != NULL && nSize > 0)
	{
		char strHost[HOSTNAME_SIZE] = { 0 };

		// get host name, if fail, SetLastError is set
		if (SOCKET_ERROR != gethostname(strHost, sizeof(strHost)))
		{
			struct hostent* hp;
			hp = gethostbyname(strHost);
			if (hp != NULL)	{
				strcpy(strHost, hp->h_name);
			}

			// check if user provide enough buffer
			if (strlen(strHost) > nSize)
			{
				SetLastError(ERROR_INSUFFICIENT_BUFFER);
				return false;
			}

			// Unicode conversion
#ifdef _UNICODE
			return (0 != MultiByteToWideChar(CP_ACP, 0, strHost, -1, strName, nSize ));
#else
			_tcscpy(strName, strHost);
			return true;
#endif
		}
	}
	else
		SetLastError(ERROR_INVALID_PARAMETER);
	return false;
}


///////////////////////////////////////////////////////////////////////////////
// GetLocalAddress
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//				Get TCP address of local computer in dot format ex: "127.0.0.0"
// PARAMETERS:
//	LPTSTR strAddress: pointer to hold address string, must be long enough
//	UINT nSize: maximum size of this buffer
///////////////////////////////////////////////////////////////////////////////
bool CSocketComm::GetLocalAddress(LPTSTR strAddress, UINT nSize)
{
	// Get computer local address
	if (strAddress != NULL && nSize > 0)
	{
		char strHost[HOSTNAME_SIZE] = { 0 };

		// get host name, if fail, SetLastError is called
		if (SOCKET_ERROR != gethostname(strHost, sizeof(strHost)))
		{
			struct hostent* hp;
			hp = gethostbyname(strHost);
			if (hp != NULL && hp->h_addr_list[0] != NULL)
			{
				// Address is four bytes (32-bit)
				if ( hp->h_length < 4)
					return false;

				// Convert address to . format
				strHost[0] = 0;

				// Create Address string
				sprintf(strHost, "%u.%u.%u.%u",
					(UINT)(((PBYTE) hp->h_addr_list[0])[0]),
					(UINT)(((PBYTE) hp->h_addr_list[0])[1]),
					(UINT)(((PBYTE) hp->h_addr_list[0])[2]),
					(UINT)(((PBYTE) hp->h_addr_list[0])[3]));

				// check if user provide enough buffer
				if (strlen(strHost) > nSize)
				{
					SetLastError(ERROR_INSUFFICIENT_BUFFER);
					return false;
				}

			// Unicode conversion
#ifdef _UNICODE
				return (0 != MultiByteToWideChar(CP_ACP, 0, strHost, -1, strAddress,nSize ));
#else
				_tcscpy(strAddress, strHost);
				return true;
#endif
			}
		}
	}
	else
		SetLastError(ERROR_INVALID_PARAMETER);
	return false;
}


///////////////////////////////////////////////////////////////////////////////
// WaitForConnection
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//				Wait for a network connection.  Only for connection type of socket
//				This function may fail, in this case it returns "INVALID_SOCKET"
// PARAMETERS:
//	SOCKET sock: a socket capable of receiving new connection (TCP: SOCK_STREAM)
///////////////////////////////////////////////////////////////////////////////

SOCKET CSocketComm::WaitForConnection(SOCKET server_sock, sockaddr* acceptIp, int* lenthOfIp)
{
	// Accept an incoming connection - blocking
	// no information about remote address is returned
	return accept(server_sock, acceptIp, lenthOfIp); // return sock for client 
}

SOCKET CSocketComm::WaitForConnection(SOCKET server_sock, sockaddr_in* acceptIp, int* lenthOfIp)
{
	// Accept an incoming connection - blocking
	// no information about remote address is returned
	return accept(server_sock, (sockaddr*)acceptIp, lenthOfIp); // return sock for client 
/*
	char ip[MAX_PATH];
	LPTSTR strAddress = m_sAcceptIp.GetBuffer();
	SOCKET client_socket = accept(server_sock, (struct sockaddr *)acceptIp, lenthOfIp);
	inet_ntop(AF_INET, &(acceptIp->sin_addr), ip, INET_ADDRSTRLEN);
	MultiByteToWideChar(CP_ACP, 0, ip, -1, strAddress, sizeof(ip));
	//m_sAcceptIp = CString(strAddress);
	return client_socket; // return sock for client 
*/
}

///////////////////////////////////////////////////////////////////////////////
// ShutdownConnection
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//				Shutdown a connection and close socket.  This will force all
//				transmission/reception to fail.
// PARAMETERS:
//	SOCKET sock: Socket to close
///////////////////////////////////////////////////////////////////////////////
bool CSocketComm::ShutdownConnection(SOCKET sock)
{
	shutdown(sock, SD_BOTH);
	return ( 0 == closesocket( sock ));
}

BOOL CSocketComm::Shutdown()
{
	return ShutdownConnection((SOCKET)m_hComm);
	//shutdown((SOCKET)m_hComm, SD_RECEIVE);
	//return (0 == closesocket((SOCKET)m_hComm));
}
///////////////////////////////////////////////////////////////////////////////
// GetSockName
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//				retrieves the local name for a socket
// PARAMETERS:
//	SockAddrIn& saddr_in: object to store address
///////////////////////////////////////////////////////////////////////////////
bool CSocketComm::GetSockName(SockAddrIn& saddr_in)
{
	if (IsOpen())
	{
		int namelen = saddr_in.Size();
		return (SOCKET_ERROR != getsockname(GetSocket(), (LPSOCKADDR)saddr_in, &namelen));
	}

	return false;
}


///////////////////////////////////////////////////////////////////////////////
// GetPeerName
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//				retrieves the name of the peer to which a socket is connected
// PARAMETERS:
//	SockAddrIn& saddr_in: object to store address
///////////////////////////////////////////////////////////////////////////////
bool CSocketComm::GetPeerName(SockAddrIn& saddr_in)
{
	if (IsOpen())
	{
		int namelen = saddr_in.Size();
		return (SOCKET_ERROR != getpeername(GetSocket(), (LPSOCKADDR)saddr_in, &namelen));	
	}

	return false;
}


///////////////////////////////////////////////////////////////////////////////
// CreateSocket
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//				This function creates a new socket for connection (SOCK_STREAM).
//				A connectionless socket should not call "accept()" since it cannot receive new
//				connection.  This is used as SERVER socket
// PARAMETERS:
//	LPCTSTR strServiceName: Service name or port number
//	int nFamily: address family to use (set to AF_INET)
///////////////////////////////////////////////////////////////////////////////
bool CSocketComm::CreateSocket(LPCTSTR strServiceName, int nFamily)
{
	// Socket is already opened
	if ( IsOpen() )
		return false;

	// Create a Socket that is bound to a specific service provide
	// nFamily: (AF_INET), IPv4 주소 체컖E사퓖E

	SOCKET sock = socket(nFamily, SOCK_STREAM, 0);
	if (INVALID_SOCKET != sock)
	{
		// Associate a local address with the socket
		SockAddrIn sockAddr;
		sockAddr.CreateFrom(NULL, strServiceName, nFamily);

		if (::bind(sock, (LPSOCKADDR)sockAddr, sockAddr.Size()) == SOCKET_ERROR)
		{
			int nError = WSAGetLastError();
			m_strLastError = GetSockErorMsg(nError);

			closesocket( sock );
			return false;
		}

		// Listen to the socket, only valid for connection socket
		if ( SOCKET_ERROR == listen(sock, SOMAXCONN))
		{
			int nError = WSAGetLastError();
			m_strLastError = GetSockErorMsg(nError);
			closesocket(sock);
			return false;
		}

		// Success, now we may save this socket
		m_hComm = (HANDLE)sock;
	}

	return (INVALID_SOCKET != sock);
}

bool CSocketComm::CreateSocket(CString strIP, LPCTSTR strServiceName, int nFamily)
{
	// Socket is already opened
	if (IsOpen())
		return false;

	// Create a Socket that is bound to a specific service provide
	// nFamily: (AF_INET), IPv4 주소

	SOCKET sock = socket(nFamily, SOCK_STREAM, 0);
	if (INVALID_SOCKET != sock)
	{
		// Associate a local address with the socket
		SockAddrIn sockAddr;
		sockAddr.CreateFrom(strIP, strServiceName, nFamily);

		int nSize = sockAddr.Size();
		if (::bind(sock, (LPSOCKADDR)sockAddr, nSize) == SOCKET_ERROR)
		{
			int nError = WSAGetLastError();
			m_strLastError = GetSockErorMsg(nError);

			closesocket(sock);
			return false;
		}

		// Listen to the socket, only valid for connection socket
		if (SOCKET_ERROR == listen(sock, SOMAXCONN))
		{
			int nError = WSAGetLastError();
			m_strLastError = GetSockErorMsg(nError);
			closesocket(sock);
			return false;
		}

		// Success, now we may save this socket
		m_hComm = (HANDLE)sock;
	}

	return (INVALID_SOCKET != sock);
}

bool CSocketComm::ListenFrom(CString strIP, LPCTSTR strServiceName, int nFamily)
{
	m_bConnected = CreateSocket(strIP, strServiceName, nFamily);
	return m_bConnected;
}


///////////////////////////////////////////////////////////////////////////////
// ConnectTo
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//				Establish connection with a server service or port
// PARAMETERS:
//	LPCTSTR strDestination: hostname or address to connect (in .dot format)
//	LPCTSTR strServiceName: Service name or port number
//	int nFamily: address family to use (set to AF_INET)
///////////////////////////////////////////////////////////////////////////////
bool CSocketComm::ConnectTo(CString strClientIP, LPCTSTR strServerIP,  LPCTSTR strServiceName, int nFamily)
{
	int nErr = 0;

	// Socket is already opened
	if (IsOpen())
		return false;


	// Create a Socket that is bound to a specific service provide
	// nFamily: (AF_INET)

	SOCKET sock = socket(nFamily, SOCK_STREAM, 0);
	if (INVALID_SOCKET != sock)
	{
		// Associate a local address with the socket
		SockAddrIn sockAddr;
		if (false == sockAddr.CreateFrom(strClientIP, TEXT("0"), nFamily))
		{
			int nError = WSAGetLastError();
			m_strLastError = GetSockErorMsg(nError);
			closesocket(sock);
			return false;
		}

		struct linger ling;
		ling.l_onoff = 1;
		ling.l_linger = 0;

		int nRetCode = setsockopt(sock, SOL_SOCKET, SO_LINGER, (char*)&ling, sizeof(ling));

		if (SOCKET_ERROR == bind(sock, (const sockaddr *)&sockAddr, (int)sizeof(sockAddr)))
		{
			int nError = WSAGetLastError();
			m_strLastError = GetSockErorMsg(nError);
			closesocket(sock);
			return false;
		}

		// Now get destination address & port
		sockAddr.CreateFrom(strServerIP, strServiceName);

		// try to connect - if fail, server not ready
		if (SOCKET_ERROR == connect(sock, (LPSOCKADDR)sockAddr, sockAddr.Size()))
		{
			int nError = WSAGetLastError();
			m_strLastError = GetSockErorMsg(nError);
			closesocket(sock);
			return false;
		}

		// Success, now we may save this socket
		m_hComm = (HANDLE)sock;
	}
	else
	{
		nErr = WSAGetLastError();
	}

	m_bConnected = (INVALID_SOCKET != sock);
	return m_bConnected;
}

///////////////////////////////////////////////////////////////////////////////
// CloseComm
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//		Close Socket Communication
// PARAMETERS:
//		None
///////////////////////////////////////////////////////////////////////////////
void CSocketComm::CloseComm()
{
	if (IsOpen())
	{
		ShutdownConnection((SOCKET)m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;
		m_bConnected = FALSE;
	}
}


///////////////////////////////////////////////////////////////////////////////
// StopComm
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//		Close Socket and Stop Communication thread
// PARAMETERS:
//		None
///////////////////////////////////////////////////////////////////////////////
void CSocketComm::StopComm()
{
	Sleep(100);
	
	if (IsOpen())
	{
		CloseComm();
		Sleep(100);
	}
}

///////////////////////////////////////////////////////////////////////////////
// WriteComm
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//		Writes data to the Socket Communication
// PARAMETERS:
//		const LPBYTE lpBuffer: data to write
//		DWORD dwCount: maximum characters to write
//		DWORD dwTimeout: timeout to use in millisecond
///////////////////////////////////////////////////////////////////////////////
int CSocketComm::WriteComm(CString sMsg, DWORD dwTimeout)
{
	char buffer[1024];
	StringToChar(sMsg, buffer);
	int nLen = strlen(buffer);
	if (m_nServer == ID_SR1000W)
	{
		buffer[nLen] = 0x0d;
		//buffer[nLen+1] = 0;
	}
	else if (m_nServer == ID_MDX2500)
	{
		buffer[nLen] = 0x0d;
		//buffer[nLen] = '\r';
	}
	else  // ID_ENGRAVE
	{
		buffer[nLen] = 0;
	}

	return WriteComm((BYTE*)buffer, strlen(buffer), dwTimeout);
}

int CSocketComm::WriteComm(const LPBYTE lpBuffer, DWORD dwCount, DWORD dwTimeout)
{

	// Accept 0 bytes message
	if (!IsOpen() || NULL == lpBuffer)
		return -1;

	fd_set	fdWrite  = { 0 };
	TIMEVAL	stTime;
	TIMEVAL	*pstTime = NULL;

	//160107 LGH 
	if ( INFINITE != dwTimeout ) {
		stTime.tv_sec = dwTimeout/1000;
		stTime.tv_usec = (dwTimeout * 1000 ) % 1000000;
		pstTime = &stTime;
	}

	SOCKET s = (SOCKET) m_hComm;
	// Set Descriptor
	if ( !FD_ISSET( s, &fdWrite ) )
		FD_SET( s, &fdWrite );

	// Select function set write timeout
	int dwBytesWritten = 0L;
	int res = select( s+1, NULL, &fdWrite, NULL, pstTime );
	if ( res > 0)
	{
		// Send to peer-connection
		res = send( s, (LPCSTR)lpBuffer, dwCount, 0);
		if(res != dwCount)
			int kk = 0;

		dwBytesWritten = (int)((res >= 0) ? (res) : (-1));
		
	}

	return dwBytesWritten;
}


CString CSocketComm::GetSockErorMsg(int Code)
{

	CString strDefault;

	strDefault.Format(_T("Code :%d"), Code);

	switch (Code)
	{

	case WSA_INVALID_HANDLE: return _T("WSA_INVALID_HANDLE[6] 지정된 이벤트 객체 핸들이 잘못 되었습니다.");
	case WSA_NOT_ENOUGH_MEMORY: return _T("WSA_NOT_ENOUGH_MEMORY[8] 메모리가 부족합니다.");
	case WSA_INVALID_PARAMETER: return _T("WSA_INVALID_PARAMETER[87] 하나 이상의 잘못된 인자가 사용되었습니다");

	case WSA_OPERATION_ABORTED: return _T("WSA_OPERATION_ABORTED[995]	overlapped(:12) 연산이 중단 되었음");

	case WSA_IO_INCOMPLETE:	return _T("WSA_IO_INCOMPLETE[996]	Overlapped 입출력 이벤트가 신호 상태가 아니다.");

	case WSA_IO_PENDING:	return _T("WSA_IO_PENDING[997]	Overlapped 연산은 나중에 완료될 것이다.중첩 연산을 위한 준비가 되었으나, 즉시 완료되지 않았을 경우발생");

	case WSAEINTR:	return _T("WSAEINTR[10004]	WSACancelBlockingCall()에 의해 블록화 호출이 취소 : Interrupted system call");

	case WSAEBADF:	return _T("WSAEBADF[10009]	잘못된 파일 기술자가 사용되었음");

	case WSAEACCES:	return _T("WSAEACCES[10013]	요청한 주소가 브로드캐스트 주소인데 setsockopt()로 SO_BROADCAST 가 설정되지 않았다.");
	case WSAEFAULT:	return _T("WSAEFAULT[10014]	잘못된 주소를 사용했음");

	case WSAEINVAL:	return _T("WSAEINVAL[10022]	바인딩 실패.이미 bind된 소켓에 바인드하거나 주소체계가 일관적이지 않을 때");
	case WSAEMFILE:	return _T("WSAEMFILE[10024]	너무 많은 파일이 열려있음");
	case WSAEWOULDBLOCK:	return _T("WSAEWOULDBLOCK[10035]	non overlapped 소켓 : 비 봉쇄 소켓에 아직 읽을 데이터가 없음, overlapped(:12) 소켓 : 너무 많은 중첩 입출력 요구가 있음");

	case WSAEINPROGRESS:	return _T("WSAEINPROGRESS[10036]	블록화 함수가 진행되는 동안 부적절한 윈속 API함수가 호출.");

	case WSAEALREADY:	return _T("WSAEALREADY[10037]	비봉쇄모드 소켓에서 아직 진행중인 작업임. (connect가 완료되지 않은 상태에서 connect 함수의 호출등..)");

	case WSAENOTSOCK:	return _T("WSAENOTSOCK[10038]	잘못된 소켓기술자를 사용했음");

	case WSAEDESTADDRREQ:	return _T("WSAEDESTADDRREQ[10039]	목적지 주소를 명시하지 않았음");

	case WSAEMSGSIZE:	return _T("WSAEMSGSIZE[10040]	송수신에 사용된 데이터가 버퍼의 크기를 초과해서 크기에 맞게 잘렸음");
	case WSAEPROTOTYPE:	return _T("WSAEPROTOTYPE[10041]	소켓에 맞지 않는 프로토콜을 사용했음");

	case WSAENOPROTOOPT:	return _T("WSAENOPROTOOPT[10042]	잘못된 옵션을 사용했음.지원되지 않는 옵션으로 getsockopt함수를 호출하는 등.");

	case WSAESOCKTNOSUPPORT:	return _T("WSAESOCKTNOSUPPORT[10044]	소켓타입이 지원하지 않는 주소체계를 사용했음");

	case WSAEOPNOTSUPP:	return _T("WSAEOPNOTSUPP[10045]	소켓이 지원하지 않는 명령을 사용했음.listen()함수를 데이터그램 통신(SO_DGRAM)에서 호출");

	case WSAEPFNOSUPPORT:	return _T("WSAEPFNOSUPPORT[10046]	지원하지 않는 프로토콜을 사용했음");

	case WSAEAFNOSUPPORT:	return _T("WSAEAFNOSUPPORT[10047]	윈속에서 지원하지 않는 주소체계를 사용했음");

	case WSAEADDRINUSE:	return _T("WSAEADDRINUSE[10048]	지정된 주소가 이미 사용 중임");

	case WSAEADDRNOTAVAIL:	return _T("WSAEADDRNOTAVAIL[10049]	사용할 수 없는 주소임");
	case WSAENETDOWN:	return _T("WSAENETDOWN[10050]	네트워크 서브 시스템에 문제가 있음.네트워크 접속 끊김등.");
	case WSAENETUNREACH:	return _T("WSAENETUNREACH[10051]	네크워크에 접근할 수 없음");
	case WSAENETRESET:	return _T("WSAENETRESET[10052]	네트워크 재설정으로 연결이 끊어졌음.");
	case WSAECONNABORTED:	return _T("WSAECONNABORTED[10053]	타임아웃 혹은 상대방의 접속종료들과 같은 소프트웨어적인 문제로 연결이 끊겼음.");
	case WSAECONNRESET:	return _T("WSAECONNRESET[10054]	연결이 원격 호스트에 의해 재설정되었음.");
	case WSAENOBUFS:	return _T("WSAENOBUFS[10055]	남아있는 버퍼공간이 없어서 소켓을 사용할 수 없음");
	case WSAEISCONN:	return _T("WSAEISCONN[10056]	이미 연결이 완료된 소켓임.connect로 연결이 완료된 소켓에 다시 connect를 시도할 경우");
	case WSAENOTCONN:	return _T("WSAENOTCONN[10057]	연결되지 않은 소켓임.연결되지 않은 소켓에 읽고 쓰는 경우");
	case WSAESHUTDOWN:	return _T("WSAESHUTDOWN[10058]	소켓이 종료되었음.종료된 소켓에 데이터를 읽고 쓰려 경우");
	case WSAETOOMANYREFS:	return _T("WSAETOOMANYREFS[10059]	Too many references");
	case WSAETIMEDOUT:	return _T("WSAETIMEDOUT[10060]	접속시도 제한 시간 초과");
	case WSAECONNREFUSED:	return _T("WSAECONNREFUSED[10061]	서버가 연결시도를 거절함.수신 대기열이 가득찬 상태에서 클라이언트의 connect 호출이 있을 경우");
	case WSAELOOP:	return _T("WSAELOOP[10062]	너무 많은 심볼릭링크가 사용되었음");
	case WSAENAMETOOLONG:	return _T("WSAENAMETOOLONG[10063]	파일이름이 너무 김");
	case WSAEHOSTDOWN:	return _T("WSAEHOSTDOWN[10064]	호스트 다운");
	case WSAEHOSTUNREACH:	return _T("WSAEHOSTUNREACH[10065]	호스트로의 경로를 설정할 수 없음");
	case WSAENOTEMPTY:	return _T("WSAENOTEMPTY[10066]	네트워크 서브시스템이 아직 통신할 준비가 되어 있지 않음.WSAStartup에 의해 반환");
	case WSAEPROCLIM:	return _T("WSAEPROCLIM[10067]	너무 많은 프로세스가 생성되었음");

	case WSAEUSERS:	return _T("WSAEUSERS[10068] 사용자가 너무 많음");
	case WSAEDQUOT:	return _T("WSAEDQUOT[10069]	디스크 허용 할당량 초과");
	case WSAESTALE:	return _T("WSAESTALE[10070]	Stale NFS file handle");
	case WSAEREMOTE:	return _T("WSAEREMOTE[10071]	Too many levels of remote in path");

	case WSAEDISCON:	return _T("WSAEDISCON[10101]	종료가 진행중임");
	case WSASYSNOTREADY:	return _T("WSASYSNOTREADY[10091]	네트워크 서브시스템이 아직 통신할 준비가 되어 있지 않았음");


	case WSAVERNOTSUPPORTED:	return _T("WSAVERNOTSUPPORTED[10092]	윈속이 지원하지 않는 프로토콜 버전을 사용했음.WSAStartUp()에서 반환");

	case WSANOTINITIALISED:	return _T("WSANOTINITIALISED[10093]	WSAStartup() 함수가 성공적으로 실행되지 않은 상황에서 윈속 함수를 호출했을 때");

	case WSAHOST_NOT_FOUND:	return _T("WSAHOST_NOT_FOUND[11001]	요청된 호스트를 찾을 수 없음");

	case WSATRY_AGAIN:	return _T("WSATRY_AGAIN[11002]	요청된 호스트를 찾을 수 없음");

	case WSANO_RECOVERY:	return _T("WSANO_RECOVERY[11003]	복구할 수 없는 에러가 발생하였음");

	case WSANO_DATA:	return _T("WSANO_DATA[11004]	요청한 이름이 유효하지만 정보를 찾지 못했음.gethostbyaddr()등에서 발생");

	default:

		return strDefault;

		break;
	}

}

void CSocketComm::StringToChar(CString str, char *szStr)
{
	int nLen = str.GetLength();
	strcpy(szStr, CT2A(str));
	szStr[nLen] = _T('\0');
}

CString CSocketComm::CharToString(char *szStr)
{
	CString strRet;

	int nLen = strlen(szStr) + sizeof(char);
	wchar_t *tszTemp = NULL;
	tszTemp = new WCHAR[nLen];

	MultiByteToWideChar(CP_ACP, 0, szStr, -1, tszTemp, nLen * sizeof(WCHAR));

	strRet.Format(_T("%s"), (CString)tszTemp);
	if (tszTemp)
	{
		delete[] tszTemp;
		tszTemp = NULL;
	}
	return strRet;
}

void CSocketComm::SetServer(int nServerId)
{
	m_nServer = nServerId;	// WriteComm함수에서 Last String 처리.
}

int CSocketComm::GetServerID()
{
	return m_nServer;
}

DWORD CSocketComm::WriteCommData(SOCKET_DATA SocketData, DWORD dwTimeout, DWORD dwStrLen)
{
	//_ASSERTE(IsOpen());

	// Accept 0 bytes message
	if (!IsOpen())
		return 0L;

	fd_set	fdWrite = { 0 };
	TIMEVAL	stTime;
	TIMEVAL	*pstTime = NULL;

	if (INFINITE != dwTimeout) {
		stTime.tv_sec = dwTimeout / 1000;
		stTime.tv_usec = dwTimeout % 1000;
		pstTime = &stTime;
	}

	SOCKET s = (SOCKET)m_hComm;
	// Set Descriptor
	if (!FD_ISSET(s, &fdWrite))
		FD_SET(s, &fdWrite);

	// Select function set write timeout
	DWORD dwBytesWritten = 0L;
	int res = select(s + 1, NULL, &fdWrite, NULL, pstTime);
	if (res > 0)
	{
		// Send to peer-connection

		int nSize;
		if (dwStrLen == -1)
			nSize = strlen(SocketData.strData);
		else
			nSize = dwStrLen;
		int dwCount = 52 + nSize;
		
		SocketData.nMsgLen = dwCount;
		SocketData.nMsgLen = htonl(SocketData.nMsgLen);		// Message Length 
		SocketData.nMsgID = htonl(SocketData.nMsgID);		// message ID
		SocketData.nCmdCode = htonl(SocketData.nCmdCode);	// Command Code
		SocketData.nTxPC = htonl(SocketData.nTxPC);			// Transmit PC Id
		SocketData.nRxPC = htonl(SocketData.nRxPC);			// Message Receive PC
		SocketData.nData1 = htonl(SocketData.nData1);		// Integer Param1
		SocketData.nData2 = htonl(SocketData.nData2);		// Integer Param2
		SocketData.nData3 = htonl(SocketData.nData3);		// Integer Param3
		SocketData.nData4 = htonl(SocketData.nData4);		// Integer Param4
		SocketData.fData1 = htonl(SocketData.fData1);		// float Param1
		SocketData.fData2 = htonl(SocketData.fData2);		// float Param2
		SocketData.fData3 = htonl(SocketData.fData3);		// float Param3
		SocketData.fData4 = htonl(SocketData.fData4);		// float Param4

		res = send(s, (LPSTR)&SocketData, dwCount, 0);

		dwBytesWritten = (DWORD)((res >= 0) ? (res) : (-1));
	}

	return dwBytesWritten;
}

//Rs232.cpp Rs232c통신을 하기 위한 클래스
#include "stdafx.h"
#include "Rs232.h"
#include "../Global/GlobalDefine.h"
#include "../Global/GlobalFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CRs232, CObject)



CRs232::CRs232()
{
	m_hComDev = NULL;
	m_nFlowCtrl = FC_XONXOFF ;
	m_bConnected = FALSE ;

	m_osRead.hEvent = NULL;
	m_osWrite.hEvent = NULL;

	m_dwBaudRate = CBR_9600;
	m_bCR = FALSE;
	m_bLF = FALSE;
	m_bOnLine = FALSE;

	//memset(m_cRxBuffer, NULL, sizeof(BYTE)*MAXBLOCK );
	m_cRxBuffer = _T("");
}

CRs232::~CRs232()
{
	if (m_bConnected)
		CloseConnection();
//	DestroyRs232();
}

//BEGIN_MESSAGE_MAP(CRs232, CObject)
	//{{AFX_MSG_MAP(CRs232)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
//END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CRs232 message handlers
//Rs232WatchProc()
//통신을 하는 프로세저 즉 데이타가 들어왔을대 감시하는
//루틴 본루틴은 OpenComPort 함수 실행시 프로시저로 연결됨
//OpenComPort 함수 참조
DWORD Rs232WatchProc(LPVOID lpData)
{
	DWORD       dwEvtMask ;
	OVERLAPPED  os ;
	CRs232*      npRs232 = (CRs232*) lpData ;
	char InData[MAXBLOCK + 1];
	int        nLength ;
	//idRs232Dev 라는 핸들에 아무런 com 포트가 안붙어 잇으면
	// 에라 리턴
	if ( npRs232 == NULL || !npRs232->IsKindOf( RUNTIME_CLASS( CRs232 ) ) )
		return (DWORD)(-1);
	if ( !npRs232->m_hComDev )
		return (DWORD)(-1);

	memset( &os, 0, sizeof( OVERLAPPED ) ) ;


	os.hEvent = CreateEvent(	NULL,    // no security
								TRUE,    // explicit reset req
								FALSE,   // initial event reset
								NULL ) ; // no name
	if ( os.hEvent == NULL )
	{
		MessageBox( NULL, _T("Failed to create event for thread!"), _T("Rs232 Error!"),
					MB_ICONEXCLAMATION | MB_OK ) ;
		return ( FALSE ) ;
	}

	if (!SetCommMask(npRs232->m_hComDev, EV_RXCHAR ))
		return ( FALSE ) ;

	while (npRs232->m_bConnected)
	{
		dwEvtMask = 0 ;

		WaitCommEvent(npRs232->m_hComDev, &dwEvtMask, NULL );

		if ((dwEvtMask & EV_RXCHAR) == EV_RXCHAR)
		{

			do
			{
				if(!npRs232->m_bConnected)
					break;

				memset(InData, 0, sizeof(char)*MAXBLOCK);
				if (nLength = npRs232->ReadRs232Block((LPSTR) InData, MAXBLOCK ))
				{
					npRs232->SetReadData(InData);
					//이곳에서 데이타를 받는다.
				}
			}
			while ( nLength > 0 ) ;
		}
	}


	CloseHandle( os.hEvent ) ;


	return( TRUE ) ;

} 

//데이타를 일고 데이타를 읽었다는
//메세지를 리턴한다.
void CRs232::SetReadData(LPSTR data)
{
	//lstrcpy((LPSTR)m_cRxBuffer,(LPSTR)data);
	m_cRxBuffer = CharToString(data);
	//ConverData
	//설정된 윈도우에 WM_RECEIVEDATA메세지를
	//날려주어 현제 데이타가 들어왔다는것을
	//알려준다.
	SendMessage(m_hwnd,WM_RECEIVE_RS232,0,0);
}

//메세지를 전달할 hwnd설정
void CRs232::SetHwnd(HWND hwnd)
{
	m_hwnd=hwnd;
}

//컴포트를 설정한다.
void CRs232::SetComPort(int port,DWORD rate,BYTE bytesize,BYTE stop,BYTE parity)
{
	m_nPort=port;
	m_dwBaudRate=rate;
	m_nByteSize=bytesize;
	m_nStopBits=stop;
	m_nParity=parity;
}

//XonOff 즉 리턴값 더블 설정
void CRs232::SetXonOff(BOOL chk)
{
	m_bXonXoff=chk;
}

void CRs232::SetDtrRts(BYTE chk)
{
	m_nFlowCtrl=chk;
}

void CRs232::ClearReceive()
{
	//memset(m_cRxBuffer, NULL, sizeof(BYTE)*MAXBLOCK );
	m_cRxBuffer = _T("");
}

//컴포트 정보를 만든다.
//이것을 만들때 이전에 할일이
// SetComPort(); -> SetXonOff() ->SetDtrRts() 한다음 설정한다.
BOOL CRs232::CreateRs232Info()
{


	m_osWrite.Offset = 0 ;
	m_osWrite.OffsetHigh = 0 ;
	m_osRead.Offset = 0 ;
	m_osRead.OffsetHigh = 0 ;

	//이벤트 창구 설정
	m_osRead.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL ) ; 
	if (m_osRead.hEvent == NULL)
	{
		return FALSE ;
	}
	m_osWrite.hEvent = CreateEvent( NULL,   TRUE,  FALSE,   NULL ) ;
	if (m_osWrite.hEvent == NULL)
	{
		CloseHandle( m_osRead.hEvent ) ;
		return FALSE;
	}


	return TRUE ;

} 

//com 포트를 열고 연결을 시도한다.
//OpenComport()
BOOL CRs232::OpenComPort()
{
	m_bOnLine = FALSE;

	if(m_dwBaudRate <=110)
		m_dwBaudRate = CBR_110;
	else if(m_dwBaudRate <=300)
		m_dwBaudRate = CBR_300;
	else if(m_dwBaudRate <=600)
		m_dwBaudRate = CBR_600;  
	else if(m_dwBaudRate <=1200)
		m_dwBaudRate = CBR_1200; 
	else if(m_dwBaudRate <=2400)
		m_dwBaudRate = CBR_2400; 
	else if(m_dwBaudRate <=4800)
		m_dwBaudRate = CBR_4800; 
	else if(m_dwBaudRate <=9600)
		m_dwBaudRate = CBR_9600; 
	else if(m_dwBaudRate <=14400)
		m_dwBaudRate = CBR_14400; 
	else if(m_dwBaudRate <=19200)
		m_dwBaudRate = CBR_19200; 
	else if(m_dwBaudRate <=38400)
		m_dwBaudRate = CBR_38400; 
	else if(m_dwBaudRate <=56000)
		m_dwBaudRate = CBR_56000; 
	else if(m_dwBaudRate <=57600)
		m_dwBaudRate = CBR_57600; 
	else if(m_dwBaudRate <=115200)
		m_dwBaudRate = CBR_115200; 
	else if(m_dwBaudRate <=128000)
		m_dwBaudRate = CBR_128000; 
	else if(m_dwBaudRate <=256000)
		m_dwBaudRate = CBR_256000; 
	else
		m_dwBaudRate = CBR_9600;

	if(!CreateRs232Info())
		return FALSE;

	//char       szPort[ 15 ];
	CString szPort;

	BOOL       bRetVal=FALSE;
	COMMTIMEOUTS  CommTimeOuts;


	if (m_nPort > MAXPORTS)
		szPort.Format(_T("%s"), _T("\\\\.\\TELNET"));		//lstrcpy(szPort, "\\\\.\\TELNET");
	else
		szPort.Format(_T("COM%d"), m_nPort);

	// COMM device를 화일형식으로 연결한다.
	if(m_hComDev)
	{
		CloseHandle(m_hComDev);
		m_hComDev = NULL;
	}

	if( (m_hComDev = CreateFile( szPort, GENERIC_READ | GENERIC_WRITE,
																  0,                    // exclusive access
																  NULL,                 // no security attrs
																  OPEN_EXISTING,
																  FILE_ATTRIBUTE_NORMAL | 
																  FILE_FLAG_OVERLAPPED, // overlapped I/O
																  NULL )) == (HANDLE) -1 )
	{
		if(m_osRead.hEvent)
		{
			CloseHandle( m_osRead.hEvent );
			m_osRead.hEvent = NULL;
		}
		if(m_osWrite.hEvent)
		{
			CloseHandle( m_osWrite.hEvent );
			m_osWrite.hEvent = NULL;
		}

		m_hComDev = NULL;
		return ( FALSE ) ;
	}
	else
	{
		//컴포트에서 데이타를 교환하는 방법을 char단위를 기본으로 설정
		//하자
		SetCommMask( m_hComDev, EV_RXCHAR ) ;
		SetupComm( m_hComDev, 4096, 4096 ) ;
		//디바이스에 쓰레기가 있을지 모르니까 깨끗이 청소를 하자!
		PurgeComm( m_hComDev, PURGE_TXABORT | PURGE_RXABORT |
									  PURGE_TXCLEAR | PURGE_RXCLEAR ) ;


		CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF ;
		CommTimeOuts.ReadTotalTimeoutMultiplier = 0;//1;// 0 ;
		CommTimeOuts.ReadTotalTimeoutConstant = 1000 ;
		CommTimeOuts.WriteTotalTimeoutMultiplier = 0;//2*CBR_9600 / m_dwBaudRate;//0 ;
		CommTimeOuts.WriteTotalTimeoutConstant = 1000 ;
		SetCommTimeouts( m_hComDev, &CommTimeOuts ) ;
	}

	bRetVal = SetupConnection() ;

	if (bRetVal)//연결이 되었다면 fRetVal TRUE이므로
	{
		m_bConnected = TRUE ;//연결되었다고 말해줌
		//프로시저를 CommWatchProc에 연결하니까 나중에 데이타가 왔다갔다
		//하면 모든 내용은 CommWatchProc가 담당한다.
		AfxBeginThread((AFX_THREADPROC)Rs232WatchProc,(LPVOID)this);
	}
	else
	{
		DestroyRs232();
	}

	m_bOnLine = bRetVal;
	return ( bRetVal ) ;
} 

//화일로 설정된 컴포트와 실질 포트와 연결을 시킨다.
//SetupConnection 이전에 CreateComPort를 해주어야 한다.
BOOL CRs232::SetupConnection()
{
	BOOL       bRetVal ;
//	BYTE       bSet ;
	DCB        dcb ;

	dcb.DCBlength = sizeof( DCB ) ;

	GetCommState( m_hComDev, &dcb ) ;//dcb의 기본값을 받는다.

	//이부분을 수정해야 합니다.
	dcb.BaudRate = m_dwBaudRate;//전송속도
	dcb.ByteSize = 8;//bByteSize ;//데이타비트
	dcb.Parity = 0;//m_nParity;//패리티 체크 ( 0 ~ 4 = no,odd,even,mark,space )
	dcb.StopBits = ONESTOPBIT;//m_nStopBits;//스톱비트 
// 	ONESTOPBIT		1	stop bit 
// 	ONE5STOPBITS	1.5 stop bits 
// 	TWOSTOPBITS		2	stop bits 

/*
	dcb.fOutxDsrFlow =0 ;//Dsr Flow
	dcb.fDtrControl = DTR_CONTROL_DISABLE;// DTR_CONTROL_ENABLE ;//Dtr Control
	dcb.fOutxCtsFlow = 0 ;//Cts Flow
	dcb.fRtsControl = RTS_CONTROL_DISABLE;// RTS_CONTROL_ENABLE ; //Ctr Control
	dcb.fInX = dcb.fOutX = 1 ; //XON/XOFF 관한것
	dcb.XonChar = ASCII_XON ;
	dcb.XoffChar = ASCII_XOFF ;
	dcb.XonLim = 100 ;
	dcb.XoffLim = 100 ;

	dcb.fBinary = FALSE ;
	dcb.fParity = FALSE ;
// 	dcb.fBinary = TRUE ;
// 	dcb.fParity = TRUE ;
*/
	bRetVal = SetCommState( m_hComDev, &dcb ) ; //변경된 Dcb 설정

	return ( bRetVal ) ;

} 

//컴포트로 부터 데이타를 읽는다.
int CRs232::ReadRs232Block(LPSTR lpszBlock, int nMaxLength )
{
	BOOL       bReadStat ;
	COMSTAT    ComStat ;
	DWORD      dwErrorFlags;
	DWORD      dwLength;

	// only try to read number of bytes in queue 
	ClearCommError( m_hComDev, &dwErrorFlags, &ComStat ) ;
	dwLength = min( (DWORD) nMaxLength, ComStat.cbInQue ) ;

	if (dwLength > 0)
	{
		bReadStat = ReadFile( m_hComDev, lpszBlock,
							dwLength, &dwLength, &m_osRead ) ;
		if (!bReadStat)
		{
			//이곳에 에라를 넣는것이다.
			//즉 ReadFile 했을때 데이타가 제대로 안나오면 bReadState에 여러
			//에라 코드를 리턴한다. 이때 복구할수있으면 좋지만 실질적인
			//복구가 불가능하다 따라서 재송출을 해달라는 메세지를 해주는것이
			//좋다.

			dwLength = 0;
			ClearCommError( m_hComDev, &dwErrorFlags, &ComStat );
		}
	}

	return ( dwLength ) ;

} 

//컴포트를 완전히 해제한다.

BOOL CRs232::DestroyRs232()
{
	if (m_bConnected)
		CloseConnection() ;

	if(m_osRead.hEvent)
	{
		CloseHandle( m_osRead.hEvent ) ;
		m_osRead.hEvent = NULL;
	}
	if(m_osWrite.hEvent)
	{
		CloseHandle( m_osWrite.hEvent ) ;
		m_osWrite.hEvent = NULL;
	}

	return TRUE;

} 

//연결을 닫는다.
BOOL CRs232::CloseConnection()
{
	if(m_hComDev)
	{
		// set connected flag to FALSE

		m_bConnected = FALSE ;

		// disable event notification and wait for thread
		// to halt

		SetCommMask( m_hComDev, 0 ) ;
		Sleep(30);
		EscapeCommFunction( m_hComDev, CLRDTR ) ;
		PurgeComm( m_hComDev, PURGE_TXABORT | PURGE_RXABORT |
								PURGE_TXCLEAR | PURGE_RXCLEAR ) ;
		CloseHandle( m_hComDev ) ;
		m_hComDev = NULL;
	}
	return TRUE;

} 



BOOL CRs232::WriteRs232Block( LPSTR lpByte , DWORD dwBytesToWrite)
{

	BOOL        fWriteStat ;
	DWORD       dwBytesWritten ;
	char		*pBuff;
	int			nLenBuff;

	if(m_hComDev)
	{
		if(m_bCR && m_bLF)
		{
			pBuff = new char[dwBytesToWrite+3];
			strcpy(pBuff, lpByte);
			pBuff[dwBytesToWrite] = ASCII_CR;
			pBuff[dwBytesToWrite+1] = ASCII_LF;
			nLenBuff = dwBytesToWrite + 2;
		}
		else if(m_bCR && !m_bLF)
		{
			pBuff = new char[dwBytesToWrite+2];
			strcpy(pBuff, lpByte);
			pBuff[dwBytesToWrite] = ASCII_CR;
			nLenBuff = dwBytesToWrite + 1;
		}
		else if(!m_bCR && m_bLF)
		{
			pBuff = new char[dwBytesToWrite+2];
			strcpy(pBuff, lpByte);
			pBuff[dwBytesToWrite] = ASCII_LF;
			nLenBuff = dwBytesToWrite + 1;
		}
		else
		{
			pBuff = new char[dwBytesToWrite+1];
			strcpy(pBuff, lpByte);
			nLenBuff = dwBytesToWrite;
		}

// 		fWriteStat = WriteFile( m_hComDev, lpByte, dwBytesToWrite,
// 												&dwBytesWritten, &m_osWrite ) ;
		fWriteStat = WriteFile( m_hComDev, pBuff, nLenBuff,
												&dwBytesWritten, &m_osWrite ) ;

		delete pBuff;

		if (!fWriteStat) 
		{
			//컴포트에 데이타를 제대로 써넣지 못했을경우이다.
			//이때는 어떻게 할까 그것은 사용자 마음이겠다.
			//다시 보내고 싶으면 제귀송출을 하면 된다.
			//그러나 주의점 무한 루프를 돌수 있다는점을 생각하라
		}
		return TRUE;
	}

	return FALSE;
} 

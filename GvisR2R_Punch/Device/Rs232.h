//Rs232.h
//Rs232c를 하기위한 클래스 헤더
#if !defined(AFX_Rs232c_H__INCLUDED_)
#define AFX_Rs232c_H__INCLUDED_

#include "../Global/GlobalDefine.h"

#define MAXBLOCK        80
#define MAXPORTS        20



// global stuff



// function prototypes (private)

/////////////////////////////////////////////////////////////////////////////
// CRs232 window

class CRs232 : public CObject
{

	DECLARE_DYNCREATE( CRs232 )
public:
	HANDLE      m_hComDev ;//컴포트 디바이스 연결 핸들
	BOOL        m_bConnected;//컴포트가 연결되면 1로 설정
	//BYTE       m_cRxBuffer[ MAXBLOCK + 1] ;//컴포트에서 들어오는 데이타
	CString       m_cRxBuffer;//컴포트에서 들어오는 데이타
	HWND m_hwnd;//메세지를 전달할 윈도우 플러그

// Construction
public:
	CRs232();

	void ClearReceive();

	void SetXonOff(BOOL chk);//XonOff 설정
	//컴포트를 설정함
	void SetComPort(int port,DWORD rate,BYTE bytesize,BYTE stop,BYTE parity);
	//Dtr Rts설정
	void SetDtrRts(BYTE chk);
	//Rs232 포트를 만든다.
	BOOL CreateRs232Info();
	//Rs232 포트를 해제한다.
	BOOL DestroyRs232();
	//컴포트에서 데이타를 받는다.
	int  ReadRs232Block( LPSTR, int ) ;
	//컴포트에 데이타를 넣는다.
	BOOL WriteRs232Block( LPSTR, DWORD);
	BOOL OpenComPort() ;//컴포트를 열고 연결을 시도한다.
	//포트를 연결한다.
	BOOL SetupConnection() ;
	//연결을 해제한다.
	BOOL CloseConnection() ;
	//읽은 데이타를 버퍼에 저장한다.
	void SetReadData(LPSTR data);
	//메세재를 보낼 윈도우 플래를 설정한다.
	void SetHwnd(HWND hwnd);


// Attributes
public:
	BYTE			m_nPort;
	BOOL			m_bXonXoff;
	BYTE			m_nByteSize, m_nFlowCtrl, m_nParity, m_nStopBits ;
	DWORD			m_dwBaudRate ;
	HANDLE			m_hWatchThread;
	HWND			m_hTermWnd ;
	DWORD			m_dwThreadID ;
	OVERLAPPED		m_osWrite, m_osRead ;

	BOOL			m_bCR, m_bLF;
	BOOL			m_bOnLine;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRs232)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRs232();

	// Generated message map functions
//	DECLARE_MESSAGE_MAP()
protected:
};

#endif
/////////////////////////////////////////////////////////////////////////////

// MpDevice.cpp : implementation file
//

#include "stdafx.h"
#include "../GvisR2R_Punch.h"
#include "MpDevice.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "../GvisR2R_PunchDoc.h"
#include "../GvisR2R_PunchView.h"

extern CGvisR2R_PunchDoc* pDoc;
extern CGvisR2R_PunchView* pView;

/////////////////////////////////////////////////////////////////////////////
// CMpDevice

CMpDevice::CMpDevice(CWnd* pParent)
{
	m_pParent = pParent;
	m_nPrevDecodeRatio = 0;
#ifdef USE_MPE
	m_hController = 0;	
#endif
	if(!Create(NULL, _T("MPE"), WS_CHILD, CRect(0,0,0,0), m_pParent, (UINT)this))
		AfxMessageBox(_T("CMpDevice::Create() Failed!!!"));
}

CMpDevice::~CMpDevice()
{
	CloseMpDevice();
}


BEGIN_MESSAGE_MAP(CMpDevice, CWnd)
	//{{AFX_MSG_MAP(CMpDevice)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMpDevice message handlers

void CMpDevice::CloseMpDevice() 
{
	// TODO: Add your control notification handler code here

#ifdef USE_MPE
	if(m_hController)	
	{
		ymcClearDevice(m_hDevice);
		ymcCloseController(m_hController);
	}
#endif
	
}


BOOL CMpDevice::Init(unsigned short nCpuNumber, unsigned short nPortNumber)
{
	///////////////////////////////////////
	// COM_DEVICE 구조체를 초기화합니다. //
	///////////////////////////////////////
#ifdef USE_MPE

 	m_cs.Lock();

	memset( &m_ComDevice, 0x00, sizeof(COM_DEVICE) );
	m_ComDevice.ComDeviceType = COMDEVICETYPE_PCI_MODE;		// PCI 버스 통신
	m_ComDevice.CpuNumber = (WORD)nCpuNumber;				// CPU 번호
	m_ComDevice.PortNumber = (WORD)nPortNumber;				// 포토 번호
	m_ComDevice.Timeout = 10000;							// 통신 프로세스의 타임 아웃 시간

	m_dwReturnVal = ymcOpenController(&m_ComDevice, &m_hController);
	if (m_dwReturnVal != MP_SUCCESS)
	{
		pView->DispMsg(_T("콘트롤러 오픈 실패."), _T("경고"), RGB_RED, DELAY_TIME_MSG);
 		m_cs.Unlock();
		return FALSE;
	}

	m_cs.Unlock();
#endif

	return TRUE;
}

BOOL CMpDevice::ClearRegisterM()
{	

	return TRUE;
}

BOOL CMpDevice::SetIoDataValue(CString strRegAddr, long OutputData)
{
#ifdef USE_MPE
	//char chrRegAddr[10];
	//strcpy(chrRegAddr, strRegAddr);
	//TCHAR chrRegAddr[10];
	//wsprintf(chrRegAddr, TEXT("%s"), strRegAddr);
	char* chrRegAddr;
	chrRegAddr = StringToChar(strRegAddr);

	m_dwReturnVal = ymcSetController(m_hController);
	if (m_dwReturnVal !=MP_SUCCESS)
	{
		pView->DispMsg(_T("Controller 핸들을 취득할 수 없습니다."), _T("경고"), RGB_RED, DELAY_TIME_MSG);
		return FALSE;
	}
	
	m_dwReturnVal = ymcGetRegisterDataHandle((LPBYTE)chrRegAddr, &m_hOutRegData);
	delete chrRegAddr;

	if (m_dwReturnVal !=MP_SUCCESS)
	{
		pView->DispMsg(_T("출력 레지스터 핸들을 취득할 수 없습니다."), _T("경고"), RGB_RED, DELAY_TIME_MSG);
		return FALSE;
	}
	
	m_dwReturnVal = ymcSetRegisterData(m_hOutRegData, 1, &OutputData);
	if (m_dwReturnVal !=MP_SUCCESS)
	{	
		pView->DispMsg(_T("출력 데이터 설정 실패."), _T("경고"), RGB_RED, DELAY_TIME_MSG);
		return FALSE;
	}
#endif
	return TRUE;
}

long CMpDevice::Read(CString strRegAddr)
{
	long lData = 0L;

#ifdef USE_MPE
	long lReadBuffer=0,lReadBuffer2=0, nAddrLastNum=0;	
	CString strData;
	int nthBit;
	BOOL bOdd;
	char chDataType = strRegAddr.GetAt(1);

	switch(chDataType)
	{
	case 'B': 
	case 'b': 
		// Make Bit mask
		strData = strRegAddr.Right(1);
		nthBit = atoh(strData);
		lReadBuffer2 = (0x01 << nthBit);
		// Erase Bit data
		strRegAddr.Delete(strRegAddr.GetLength()-1);
		// Adjust Address
		strData = strRegAddr.Right(1);
		nAddrLastNum = _tstoi(strData);
		bOdd = nAddrLastNum%2;
		if(bOdd)
		{
			strRegAddr.SetAt(strRegAddr.GetLength()-1,('0'+nAddrLastNum-1));
			strRegAddr.SetAt(1,'L');
			if(!GetIoDataValue(strRegAddr, lReadBuffer))
			{
				AfxMessageBox(_T("Mp2100m - Reading Error!!!"));
				return 0;
			}
			lReadBuffer = (lReadBuffer>>16);
		}
		else
		{
			strRegAddr.SetAt(1,'L');
			if(!GetIoDataValue(strRegAddr, lReadBuffer))
			{
				AfxMessageBox(_T("Mp2100m - Reading Error!!!"));
				return 0;
			}
		}
		if(lReadBuffer & lReadBuffer2)
			lData = 1;
		else
			lData = 0;

		break;
	case 'W':
	case 'w':
		strData = strRegAddr.Right(1);
		nAddrLastNum = _tstoi(strData);
		bOdd = nAddrLastNum%2;
		if(bOdd)
		{
			strRegAddr.SetAt(1,'L');
			strRegAddr.SetAt(strRegAddr.GetLength()-1,('0'+nAddrLastNum-1));
			if(!GetIoDataValue(strRegAddr, lReadBuffer))
			{
				AfxMessageBox(_T("Mp2100m - Reading Error!!!"));
				return 0;
			}
			lData = (lReadBuffer>>16) & (0x0000FFFF);
			lData = (lData >= 0x8000) ? (0xFFFF0000 | lData) : lData;
		}
		else
		{
			strRegAddr.SetAt(1,'L');
			if(!GetIoDataValue(strRegAddr, lReadBuffer))
			{
				AfxMessageBox(_T("Mp2100m - Reading Error!!!"));
				return 0;
			}
			lData = lReadBuffer & (0x0000FFFF);
			lData = (lData >= 0x8000) ? (0xFFFF0000 | lData) : lData;
		}
		break;
	case 'L':
	case 'l':
		strData = strRegAddr.Right(1);
		nAddrLastNum = _tstoi(strData);
		bOdd = nAddrLastNum%2;
		if(bOdd)
		{
			strRegAddr.SetAt(strRegAddr.GetLength()-1,('0'+nAddrLastNum-1));
			if(!GetIoDataValue(strRegAddr, lReadBuffer))
			{
				AfxMessageBox(_T("Mp2100m - Reading Error!!!"));
				return 0;
			}
			strRegAddr.SetAt(strRegAddr.GetLength()-1,('0'+nAddrLastNum+1));
			if(!GetIoDataValue(strRegAddr, lReadBuffer))
			{
				AfxMessageBox(_T("Mp2100m - Reading Error!!!"));
				return 0;
			}
			
			lReadBuffer = (lReadBuffer>>16) & 0xFFFF;
			lReadBuffer2 = (lReadBuffer2<<16) & 0xFFFF0000;
			lReadBuffer |= lReadBuffer2;
			lData = lReadBuffer;
		}
		else
		{
			if(!GetIoDataValue(strRegAddr, lReadBuffer))
			{
				AfxMessageBox(_T("Mp2100m - Reading Error!!!"));
				return 0;
			}
			lData = lReadBuffer;
		}
		break;
	case 'F':
	case 'f':
		//ymcConvertFix2Float();
		lData = lReadBuffer;
		break;
	}
	
//	m_bOnWritingM1 = FALSE;

#endif

	return lData;
}

int CMpDevice::Read(CString strRegAddr, int nSize, unsigned short *pData)
{
	int nSzData = 0;
#ifdef USE_MPE
	nSzData = GetIo(strRegAddr, pData, nSize);
#endif
	return nSzData;
}

int CMpDevice::Read(CString strRegAddr, int nSize, unsigned long *pData)
{
	int nSzData = 0;
#ifdef USE_MPE
	nSzData = GetIo(strRegAddr, pData, nSize);
#endif
	return nSzData;
}

unsigned long CMpDevice::ReadBits(CString strRegAddr, int nSize) // strRegAddr is "MBxxxxxx", nSize is 16Bits or 32Bits.
{
	unsigned long lData = 0L;
	WORD Word=0;
	DWORD dWord=0;

#ifdef USE_MPE
	char chRegType = strRegAddr.GetAt(0);
	if(chRegType=='M' || chRegType=='m' || chRegType=='I' || chRegType=='i')
	{
		char chDataType = strRegAddr.GetAt(1);
		if(chDataType == 'B' || chDataType == 'b')
		{
			switch (nSize)
			{
			case 16:
				strRegAddr.SetAt(1, 'W');
				strRegAddr = strRegAddr.Left(strRegAddr.GetLength()-1);
				nSize = 1;
				GetIo(strRegAddr, (LPVOID)&Word, nSize);
				return ((long)Word);
			case 32:
				strRegAddr.SetAt(1, 'L');
				strRegAddr = strRegAddr.Left(strRegAddr.GetLength()-1);
				nSize = 1;
				GetIo(strRegAddr, (LPVOID)&dWord, nSize);
				return ((long)dWord);
			}
			
		}
	}
	lData = Read(strRegAddr);
#endif
	return lData;
}

BOOL CMpDevice::SetIo(CString strRegAddr, LPVOID pIoData, int nIoSize)
{
#ifdef USE_MPE
	m_cs.Lock();
	//char chrRegAddr[10];
	//strcpy(chrRegAddr, strRegAddr);
	//TCHAR chrRegAddr[10];
	//wsprintf(chrRegAddr, TEXT("%s"), strRegAddr);
	char* chrRegAddr;
	chrRegAddr = StringToChar(strRegAddr);

	m_dwReturnVal = ymcSetController(m_hController);
	if (m_dwReturnVal !=MP_SUCCESS)
	{
		pView->DispMsg(_T("Controller 핸들을 취득할 수 없습니다."), _T("경고"), RGB_RED, DELAY_TIME_MSG);
		m_cs.Unlock();
		return FALSE;
	}
	
	m_dwReturnVal = ymcGetRegisterDataHandle((LPBYTE)chrRegAddr, &m_hOutRegData);
	delete chrRegAddr;

	if (m_dwReturnVal !=MP_SUCCESS)
	{
		pView->DispMsg(_T("출력 레지스터 핸들을 취득할 수 없습니다."), _T("경고"), RGB_RED, DELAY_TIME_MSG);
		m_cs.Unlock();
		return FALSE;
	}

	char chDataType = strRegAddr.GetAt(1);

	if(chDataType=='W' || chDataType=='w')
	{
		unsigned short *pData = (unsigned short *)pIoData;
		m_dwReturnVal = ymcSetRegisterData(m_hOutRegData, (DWORD)nIoSize, pData);
		if (m_dwReturnVal !=MP_SUCCESS)
		{	
			m_cs.Unlock();
			return FALSE;
		}
	}

	if(chDataType=='L' || chDataType=='l')
	{
		unsigned long *pData = (unsigned long *)pIoData;
		m_dwReturnVal = ymcSetRegisterData(m_hOutRegData, (DWORD)nIoSize, pData);
		if (m_dwReturnVal !=MP_SUCCESS)
		{	
			m_cs.Unlock();
			return FALSE;
		}
	}

// 	m_dwReturnVal = ymcSetRegisterData(m_hOutRegData, (DWORD)nIoSize, pIoData);
// 	if (m_dwReturnVal !=MP_SUCCESS)
// 	{	
// 		m_cs.Unlock();
// 		return FALSE;
// 	}
	m_cs.Unlock();
#endif
	return TRUE;
}

int CMpDevice::GetIo(CString strRegAddr, LPVOID pIoData, int nIoSize)	// nRtn : GetDataSize
{
	DWORD nRtn = 0;
	if(nIoSize <= 0)
		return 0;
#ifdef USE_MPE
	m_cs.Lock();

	DWORD dwCount = 0, wRtn = MP_SUCCESS;
	//char chrRegAddr[10];
	//strcpy(chrRegAddr, strRegAddr);
	//TCHAR chrRegAddr[10];
	//wsprintf(chrRegAddr, TEXT("%s"), strRegAddr);
	char* chrRegAddr;
	chrRegAddr = StringToChar(strRegAddr);


	HREGISTERDATA hRData;
	wRtn = ymcSetController(m_hController);
	if (wRtn !=MP_SUCCESS)
	{
		//pView->MyMsgBox(_T("ymcSetController Error!!!");
		pView->MsgBox(_T("ymcSetController Error!!!"));
		return 0;
	}
	wRtn = ymcGetRegisterDataHandle((LPBYTE)chrRegAddr, &hRData);
	delete chrRegAddr;

	if (wRtn !=MP_SUCCESS)
	{
		//pView->MyMsgBox(_T("ymcGetRegisterDataHandle Error!!!");
		pView->MsgBox(_T("ymcGetRegisterDataHandle Error!!!"));
		m_cs.Unlock();
		return 0;
	}

	char chDataType = strRegAddr.GetAt(1);

	if(chDataType=='W' || chDataType=='w')
	{
		short *pData = (short *)pIoData;
		wRtn = ymcGetRegisterData(hRData, (DWORD)nIoSize, pData, &nRtn);
		if (wRtn!=MP_SUCCESS)
		{	
			//pView->MyMsgBox(_T("ymcGetRegisterData Error!!!");
			pView->MsgBox(_T("ymcGetRegisterData Error!!!"));
			m_cs.Unlock();
			return 0;
		}
	}

	if(chDataType=='L' || chDataType=='l')
	{
		long *pData = (long *)pIoData;
		wRtn = ymcGetRegisterData(hRData, (DWORD)nIoSize, pData, &nRtn);
		if (wRtn!=MP_SUCCESS)
		{	
			//pView->MyMsgBox(_T("ymcGetRegisterData Error!!!");
			pView->MsgBox(_T("ymcGetRegisterData Error!!!"));
			m_cs.Unlock();
			return 0;
		}
	}
// 	wRtn = ymcGetRegisterData(hRData, (DWORD)nIoSize, pIoData, &nRtn);
// 	if (wRtn!=MP_SUCCESS)
// 	{	
// 		pView->MyMsgBox(_T("ymcGetRegisterData Error!!!");
// 		m_cs.Unlock();
// 		return 0;
// 	}
	if((int)nRtn != nIoSize)
	{	
		//pView->MyMsgBox(_T("ymcGetRegisterData Return size Error!!!");
		pView->MsgBox(_T("ymcGetRegisterData Return size Error!!!"));
		m_cs.Unlock();
		return 0;
	}

	m_cs.Unlock();
#endif
	return (int)nRtn;
}

BOOL CMpDevice::GetIoDataValue(CString strRegAddr, long &InputData)
{
#ifdef USE_MPE
	DWORD dwCount = 0;
	//char chrRegAddr[10];
	//strcpy(chrRegAddr, strRegAddr);
	//TCHAR chrRegAddr[10];
	//wsprintf(chrRegAddr, TEXT("%s"), strRegAddr);
	char* chrRegAddr;
	chrRegAddr = StringToChar(strRegAddr);

	m_dwReturnVal = ymcSetController(m_hController);
	if (m_dwReturnVal !=MP_SUCCESS)
	{
		pView->DispMsg(_T("Controller 핸들을 취득할 수 없습니다."), _T("경고"), RGB_RED, DELAY_TIME_MSG);
		return FALSE;
	}
	
	m_dwReturnVal = ymcGetRegisterDataHandle((LPBYTE)chrRegAddr, &m_hInRegData);
	delete chrRegAddr;

	if (m_dwReturnVal !=MP_SUCCESS)
	{
		pView->DispMsg(_T("입력 레지스터 핸들을 취득할 수 없습니다."), _T("경고"), RGB_RED, DELAY_TIME_MSG);
		return FALSE;
	}
	m_dwReturnVal = ymcGetRegisterData(m_hInRegData, 1, &InputData, &dwCount);
	if (m_dwReturnVal!=MP_SUCCESS)
	{	
		pView->DispMsg(_T("입력 데이터 설정 실패."), _T("경고"), RGB_RED, DELAY_TIME_MSG);
		return FALSE;
	}
#endif
	return TRUE;
}

BOOL CMpDevice::WriteRegisterDataO(int nAddr, long lData)
{	
#ifndef USE_MPE
	if(nAddr&0x01)
	{
		return FALSE;
	}

	CString strAddr;
	strAddr.Format(_T("OL%04x"), nAddr);
	
	if(!SetIoDataValue(strAddr, lData))
	{
		return FALSE;
	}
#endif	
	return TRUE;
}


long CMpDevice::GetRegisterDataO(int nAddr)
{	
	long lRtn=0;
#ifndef USE_MPE
	if(nAddr&0x01)
	{
		return 0;
	}

	CString strAddr;
	strAddr.Format(_T("OL%04x"), nAddr);

	if(!GetIoDataValue(strAddr, lRtn))
		return 0;
#endif

	return lRtn;
}


BOOL CMpDevice::Write(CString strRegAddr, long lData, BOOL bCheck) 
{
#ifdef USE_MPE
	m_cs.Lock();

	if(strRegAddr.IsEmpty())
	{
		m_cs.Unlock();
		return FALSE;
	}

	CString strAddr;
	strAddr = strRegAddr;

	long lWriteBuffer,lReadBuffer;	
	CString strData;
	int nthBit, nAddrLastNum;
	BOOL bOdd;
	char chDataType = strAddr.GetAt(1);

	switch(chDataType)
	{
	case 'B': 
	case 'b': 
		// Make Bit mask
		strData = strAddr.Right(1);
		nthBit = atoh(strData);
		// Erase Bit data
		strAddr.Delete(strAddr.GetLength()-1);
		// Adjust Address
		strData = strAddr.Right(1);
		nAddrLastNum = _tstoi(strData);
		bOdd = nAddrLastNum%2;
		if(bOdd)
		{
			//strAddr.SetAt(strAddr.GetLength()-1,'0');
			int nDecrese = nAddrLastNum - 1;
			strAddr.SetAt(strAddr.GetLength()-1,'0'+nDecrese);

			strAddr.SetAt(1,'L');
			if(!GetIoDataValue(strAddr, lReadBuffer))
			{
//				AfxMessageBox(_T("Mp2100m - Reading Error!!!"));
				m_cs.Unlock();
				return FALSE;
			}


			if(lData)
				lWriteBuffer = lReadBuffer | (0x01<<(16+nthBit));
			else
				lWriteBuffer = lReadBuffer & (~(0x01<<(16+nthBit)));
		}
		else
		{
			strAddr.SetAt(1,'L');
			if(!GetIoDataValue(strAddr, lReadBuffer))
			{
//				AfxMessageBox(_T("Mp2100m - Reading Error!!!"));
				m_cs.Unlock();
				return FALSE;
			}

			if(lData)
				lWriteBuffer = lReadBuffer | (0x01<<(nthBit));
			else
				lWriteBuffer = lReadBuffer & (~(0x01<<(nthBit)));
		}
		
		if(!SetIoDataValue(strAddr, lWriteBuffer))
		{
			m_cs.Unlock();
			return FALSE;
		}		
		
		break;
	case 'W':
	case 'w':
		strData = strAddr.Right(1);
		nAddrLastNum = _tstoi(strData);
		bOdd = nAddrLastNum%2;
		if(bOdd)
		{
			strAddr.SetAt(strAddr.GetLength()-1,('0'+nAddrLastNum-1));
			strAddr.SetAt(1,'L');
			if(!GetIoDataValue(strAddr, lReadBuffer))
			{
//				AfxMessageBox(_T("Mp2100m - Reading Error!!!"));
				m_cs.Unlock();
				return FALSE;
			}

			lData = (lData<<16);
			lWriteBuffer = (lReadBuffer & 0x0000FFFF) | lData;
		}
		else
		{
			strAddr.SetAt(1,'L');
			if(!GetIoDataValue(strAddr, lReadBuffer))
			{
//				AfxMessageBox(_T("Mp2100m - Reading Error!!!"));
				m_cs.Unlock();
				return FALSE;
			}
			lWriteBuffer = (lReadBuffer & 0xFFFF0000) | (lData & 0x0000FFFF);
		}
		
		if(!SetIoDataValue(strAddr, lWriteBuffer))
		{
			m_cs.Unlock();
			return FALSE;
		}

		break;
	case 'L':
	case 'l':
		strData = strAddr.Right(1);
		nAddrLastNum = _tstoi(strData);
		bOdd = nAddrLastNum%2;
		if(bOdd)
		{
			strAddr.SetAt(strAddr.GetLength()-1,('0'+nAddrLastNum-1));
			if(!GetIoDataValue(strAddr, lReadBuffer))
			{
//				AfxMessageBox(_T("Mp2100m - Reading Error!!!"));
				m_cs.Unlock();
				return FALSE;
			}
			lWriteBuffer = (lReadBuffer & 0x0000FFFF) | (lData<<16);
			if(!SetIoDataValue(strAddr, lWriteBuffer))
			{
				m_cs.Unlock();
				return FALSE;
			}

			strAddr.SetAt(strAddr.GetLength()-1,('0'+nAddrLastNum+1));
			if(!GetIoDataValue(strAddr, lReadBuffer))
			{
//				AfxMessageBox(_T("Mp2100m - Reading Error!!!"));
				m_cs.Unlock();
				return FALSE;
			}
			lWriteBuffer = (lReadBuffer & 0xFFFF0000) | (lData>>16);
			if(!SetIoDataValue(strAddr, lWriteBuffer))
			{
				m_cs.Unlock();
				return FALSE;
			}
		}
		else
		{
			lWriteBuffer = lData;
			if(!SetIoDataValue(strAddr, lWriteBuffer))
			{
				m_cs.Unlock();
				return FALSE;
			}
		}
		break;
	case 'F':
	case 'f':
		lWriteBuffer = lData;
		if(!SetIoDataValue(strAddr, lWriteBuffer))
		{
			m_cs.Unlock();
			return FALSE;
		}
		
		break;
	}

	if(bCheck)
	{
		long lRetData,nCount=0;
	
		do{
			lRetData = Read(strRegAddr);
			nCount++;
			if(lData != lRetData)
				Sleep(10);
		}while(lData != lRetData && nCount < 3);

		if(nCount >= 3)
		{
			m_cs.Unlock();
			return FALSE;
		}
	}

	m_cs.Unlock();
#endif

	return TRUE;
}

BOOL CMpDevice::WriteRegisterDataM(int nAddr, long lData)
{	
	if(nAddr&0x01)
	{
		return FALSE;
	}

	CString strAddr;
	strAddr.Format(_T("ML%05d"), nAddr);
	
	m_cs.Lock();
	if(!SetIoDataValue(strAddr, lData))
	{
		m_cs.Unlock();
		return FALSE;
	}
	
	m_cs.Unlock();
	return TRUE;
}

BOOL CMpDevice::GetRegisterBitM(int nAddr, int nBit, BOOL &bFlag)
{

	if(nBit > 31 || nBit < 0)
	{
		return FALSE;
	}

	long lReadBuffer;
	BOOL bUpperRegister=FALSE;

	m_cs.Lock();

	if(nAddr&0x01)
	{
		bUpperRegister=TRUE;
		nAddr -= 1;
		if(!ReadRegisterDataM(nAddr, lReadBuffer))
		{
			m_cs.Unlock();
			return FALSE;
		}
	}
	else
	{
		if(!ReadRegisterDataM(nAddr, lReadBuffer))
		{
			m_cs.Unlock();
			return FALSE;
		}
	}

	if(bUpperRegister)
	{
		if((lReadBuffer >> (nBit+16)) & 0x01)
			bFlag = TRUE;
		else
			bFlag = FALSE;
	}
	else
	{
		if((lReadBuffer >> nBit) & 0x01)
			bFlag = TRUE;
		else
			bFlag = FALSE;
	}


	m_cs.Unlock();
	return TRUE;
}

BOOL CMpDevice::SetRegisterBitM(int nAddr, int nBit, BOOL bFlag)
{
	
	if(nBit > 31 || nBit < 0)
	{
		return FALSE;
	}

	long lReadBuffer;
	BOOL bUpperRegister=FALSE;
	
	m_cs.Lock();

	if(nAddr&0x01)
	{
		bUpperRegister=TRUE;
		nAddr -= 1;
		if(!ReadRegisterDataM(nAddr, lReadBuffer))
		{
			m_cs.Unlock();
			return FALSE;
		}
	}
	else
	{
		if(!ReadRegisterDataM(nAddr, lReadBuffer))
		{
			m_cs.Unlock();
			return FALSE;
		}
	}

	if(bUpperRegister)
	{
		if(bFlag)
			lReadBuffer |= 0x00000001 << (nBit+16) ;
		else
			lReadBuffer &= ~(0x00000001 << (nBit+16)) ;
	}
	else
	{
		if(bFlag)
			lReadBuffer |= 0x00000001 << nBit ;
		else
			lReadBuffer &= ~(0x00000001 << nBit) ;
	}

	CString strAddr;
	strAddr.Format(_T("ML%05d"), nAddr);
	
	if(!SetIoDataValue(strAddr, lReadBuffer))
	{
		m_cs.Unlock();
		return FALSE;
	}

	m_cs.Unlock();
	return TRUE;
}

BOOL CMpDevice::ReadRegisterDataM(int nAddr, long &lReadBuffer)
{
	if(nAddr&0x01)
	{
		return FALSE;
	}
	
	CString strAddr;
	strAddr.Format(_T("ML%05d"), nAddr);

	m_cs.Lock();
	if(!GetIoDataValue(strAddr, lReadBuffer))
	{
		m_cs.Unlock();
		return FALSE;
	}

	m_cs.Unlock();
	return TRUE;
}

BOOL CMpDevice::ReadTempRegDataM(int nAddr, long &lReadBuffer)
{
	if(nAddr&0x01)
		return FALSE;

//	lReadBuffer = (long)m_wRegisterDataM[nAddr+1] * 65536L + (long)m_wRegisterDataM[nAddr];

	return TRUE;
}

int CMpDevice::atoh(CString &strVal)
{
	int nHexVal=0;
	int nLen = strVal.GetLength();
	CString strChar, strPas;
	if(nLen > 0)
	{
		for(int i=1; i<=nLen; i++)
		{
			strPas = strVal.Left(i);
			int nLenPars = strPas.GetLength();
			if(nLenPars > 1)
				strPas.Delete(0, nLenPars-1);
			strChar = strPas;
			
			strChar.MakeUpper();
			if(strChar == _T("A"))	nHexVal += int(10.0 * pow(16,(nLen-i)));
			else if(strChar == _T("B"))	nHexVal += int(11.0 * pow(16,(nLen-i)));
			else if(strChar == _T("C"))	nHexVal += int(12.0 * pow(16,(nLen-i)));
			else if(strChar == _T("D"))	nHexVal += int(13.0 * pow(16,(nLen-i)));
			else if(strChar == _T("E"))	nHexVal += int(14.0 * pow(16,(nLen-i)));
			else if(strChar == _T("F"))	nHexVal += int(15.0 * pow(16,(nLen-i)));
			else
				nHexVal += int(_tstof(strChar) * pow(16,(nLen-i)));
		}
	}

	return nHexVal;
}


// Extention I/O...............

unsigned short CMpDevice::Read(int nSeg)
{
	long nData=0;
	unsigned short usVal;
	CString strRegAddr, sAddr;
	char chRegType, chDataType;

	if(nSeg < pDoc->MkIo.MpeIo.nInSeg)
		strRegAddr=pDoc->MkIo.MpeIo.pAddrIn[nSeg][0];
	else
		strRegAddr=pDoc->MkIo.MpeIo.pAddrOut[nSeg-pDoc->MkIo.MpeIo.nInSeg][0];

	chRegType = strRegAddr.GetAt(0);
	if(chRegType=='M' || chRegType=='m' || chRegType=='I' || chRegType=='i' || chRegType=='O' || chRegType=='o')
	{
		chDataType = strRegAddr.GetAt(1);
		switch(chDataType)
		{
		case 'B': 
		case 'b': 
			nData = ReadBits(sAddr, 16);
			break;
		}
	}

	usVal = (unsigned short)(nData & 0x0000FFFF);

	return usVal;
}

unsigned short CMpDevice::ReadI(int nSeg)
{
	long nData=0;
	unsigned short usVal;
	CString strRegAddr, sAddr;
	char chRegType, chDataType;

	if(nSeg < pDoc->MkIo.MpeIo.nInSeg)
		strRegAddr=pDoc->MkIo.MpeIo.pSymIn[nSeg][0];
	else
		strRegAddr=pDoc->MkIo.MpeIo.pAddrOut[nSeg-pDoc->MkIo.MpeIo.nInSeg][0];

	chRegType = strRegAddr.GetAt(0);
	if(chRegType=='M' || chRegType=='m' || chRegType=='I' || chRegType=='i' || chRegType=='O' || chRegType=='o')
	{
		chDataType = strRegAddr.GetAt(1);
		switch(chDataType)
		{
		case 'B': 
		case 'b': 
			nData = ReadBits(sAddr, 16);
			break;
		}
	}

	usVal = (unsigned short)(nData & 0x0000FFFF);

	return usVal;
}


int CMpDevice::ReadI(int *pSeg, int nTot, unsigned short *pData)
{
	int nSzData = 0;
	CString strRegAddr[2], sAddr;
	int nStSeg, nEdSeg, nSize, nStAddr, nEdAddr;
	char chRegType, chDataType;

	if(pSeg[0] < pDoc->MkIo.MpeIo.nInSeg)
	{
		nStSeg = pSeg[0];
		nEdSeg = pSeg[nTot-1];
		strRegAddr[0] = pDoc->MkIo.MpeIo.pSymIn[nStSeg][0];
		strRegAddr[1] = pDoc->MkIo.MpeIo.pSymIn[nEdSeg][0];
	}
	else
	{
		nStSeg = pSeg[0]-pDoc->MkIo.MpeIo.nInSeg;
		nEdSeg = pSeg[nTot-1]-pDoc->MkIo.MpeIo.nInSeg;
		strRegAddr[0] = pDoc->MkIo.MpeIo.pAddrOut[nStSeg][0];
		strRegAddr[1] = pDoc->MkIo.MpeIo.pAddrOut[nEdSeg][0];
	}


	// Erase Bit data
	strRegAddr[0].Delete(strRegAddr[0].GetLength()-1);
	strRegAddr[1].Delete(strRegAddr[1].GetLength()-1);

	nStAddr = _tstoi(strRegAddr[0].Right(strRegAddr[0].GetLength()-2));
	nEdAddr = _tstoi(strRegAddr[1].Right(strRegAddr[1].GetLength()-2));

	if((nEdAddr-nStAddr) > 0)
	{
		sAddr = strRegAddr[0];
		nSize = (nEdAddr-nStAddr)+1;
	}
	else
	{
		sAddr = strRegAddr[1];
		nSize = (nStAddr-nEdAddr)+1;
	}

	chRegType = sAddr.GetAt(0);
	if(chRegType=='M' || chRegType=='m' || chRegType=='I' || chRegType=='i' || chRegType=='O' || chRegType=='o')
	{
		chDataType = sAddr.GetAt(1);
		switch(chDataType)
		{
		case 'B': 
			sAddr.SetAt(1,'W');
			nSzData = GetIo(sAddr, pData, nSize);
			break;

		case 'b': 
			sAddr.SetAt(1,'w');
			nSzData = GetIo(sAddr, pData, nSize);
			break;
		}
	}

	return nSzData;
}

int CMpDevice::ReadIO(int *pSeg, int nTot, unsigned short *pData)
{
	int nSzData = 0;
	CString strRegAddr[2], sAddr;
	int nStSeg, nEdSeg, nSize, nStAddr, nEdAddr;
	char chRegType, chDataType;

	if(pSeg[0] < pDoc->MkIo.MpeIo.nInSeg)
	{
		nStSeg = pSeg[0];
		nEdSeg = pSeg[nTot-1];
		strRegAddr[0] = pDoc->MkIo.MpeIo.pAddrIn[nStSeg][0];
		strRegAddr[1] = pDoc->MkIo.MpeIo.pAddrIn[nEdSeg][0];
	}
	else
	{
		nStSeg = pSeg[0]-pDoc->MkIo.MpeIo.nInSeg;
		nEdSeg = pSeg[nTot-1]-pDoc->MkIo.MpeIo.nInSeg;
		strRegAddr[0] = pDoc->MkIo.MpeIo.pAddrOut[nStSeg][0];
		strRegAddr[1] = pDoc->MkIo.MpeIo.pAddrOut[nEdSeg][0];
	}


	// Erase Bit data
	strRegAddr[0].Delete(strRegAddr[0].GetLength()-1);
	strRegAddr[1].Delete(strRegAddr[1].GetLength()-1);

	nStAddr = _tstoi(strRegAddr[0].Right(strRegAddr[0].GetLength()-2));
	nEdAddr = _tstoi(strRegAddr[1].Right(strRegAddr[1].GetLength()-2));

	if((nEdAddr-nStAddr) > 0)
	{
		sAddr = strRegAddr[0];
		nSize = (nEdAddr-nStAddr)+1;
	}
	else
	{
		sAddr = strRegAddr[1];
		nSize = (nStAddr-nEdAddr)+1;
	}

	chRegType = sAddr.GetAt(0);
	if(chRegType=='M' || chRegType=='m' || chRegType=='I' || chRegType=='i' || chRegType=='O' || chRegType=='o')
	{
		chDataType = sAddr.GetAt(1);
		switch(chDataType)
		{
		case 'B': 
			sAddr.SetAt(1,'W');
			nSzData = GetIo(sAddr, pData, nSize);
			break;

		case 'b': 
			sAddr.SetAt(1,'w');
			nSzData = GetIo(sAddr, pData, nSize);
			break;
		}
	}

	return nSzData;
}

int CMpDevice::ReadSignal(int *pSeg, int nTot, unsigned short *pData)
{
	int nSzData = 0;
	CString strRegAddr[2], sAddr;
	int nStSeg, nEdSeg, nSize, nStAddr, nEdAddr;
	char chRegType, chDataType;

	if(pSeg[0] < pDoc->MkIo.MpeSignal.nInSeg)
	{
		nStSeg = pSeg[0];
		nEdSeg = pSeg[nTot-1];
		strRegAddr[0] = pDoc->MkIo.MpeSignal.pAddrIn[nStSeg][0];
		strRegAddr[1] = pDoc->MkIo.MpeSignal.pAddrIn[nEdSeg][0];
	}
	else
	{
		nStSeg = pSeg[0]-pDoc->MkIo.MpeSignal.nInSeg;
		nEdSeg = pSeg[nTot-1]-pDoc->MkIo.MpeSignal.nInSeg;
		strRegAddr[0] = pDoc->MkIo.MpeSignal.pAddrOut[nStSeg][0];
		strRegAddr[1] = pDoc->MkIo.MpeSignal.pAddrOut[nEdSeg][0];
	}


	// Erase Bit data
	strRegAddr[0].Delete(strRegAddr[0].GetLength()-1);
	strRegAddr[1].Delete(strRegAddr[1].GetLength()-1);

	nStAddr = _tstoi(strRegAddr[0].Right(strRegAddr[0].GetLength()-2));
	nEdAddr = _tstoi(strRegAddr[1].Right(strRegAddr[1].GetLength()-2));

	if((nEdAddr-nStAddr) > 0)
	{
		sAddr = strRegAddr[0];
		nSize = (nEdAddr-nStAddr)+1;
	}
	else
	{
		sAddr = strRegAddr[1];
		nSize = (nStAddr-nEdAddr)+1;
	}

	chRegType = sAddr.GetAt(0);
	if(chRegType=='M' || chRegType=='m' || chRegType=='I' || chRegType=='i' || chRegType=='O' || chRegType=='o')
	{
		chDataType = sAddr.GetAt(1);
		switch(chDataType)
		{
		case 'B': 
			sAddr.SetAt(1,'W');
			nSzData = GetIo(sAddr, pData, nSize);
			break;

		case 'b': 
			sAddr.SetAt(1,'w');
			nSzData = GetIo(sAddr, pData, nSize);
			break;
		}
	}

	return nSzData;
}

int CMpDevice::ReadData(int *pSeg, int nTot, long **pData)
{
	int idx, k;
	int nSzData = 0;
	CString strRegAddr[2], sAddr;
	int nStSeg, nEdSeg, nSize, nStAddr, nEdAddr;
	char chRegType, chDataType;

	for(idx=0; idx<nTot; idx++)
	{
		if(pSeg[0] < pDoc->MkIo.MpeData.nInSeg)
		{
		nStSeg = pSeg[idx];
		sAddr = pDoc->MkIo.MpeData.pAddrIn[nStSeg][0];
		}
		else
		{
			nStSeg = pSeg[idx]-pDoc->MkIo.MpeData.nInSeg;
			sAddr = pDoc->MkIo.MpeData.pAddrOut[nStSeg][0];
		}

		nSize = 16;

		long *pLong = new long[nSize];
		memset(pLong, 0, sizeof(long)*nSize);

		nSzData += GetIo(sAddr, pLong, nSize);
		for(k=0; k<16; k++)
			pData[idx][k] = pLong[k];

		delete [] pLong;
	}

	return nSzData;
}


void CMpDevice::Write(int nSeg, unsigned short nOut, BOOL bFromSliceIO)
{
	int nOutSeg;
// 	if(!bFromSliceIO)
		nOutSeg = nSeg - pDoc->MkIo.MpeIo.nInSeg;
// 	else
// 		nOutSeg = nSeg - pDoc->MkIo.SliceIo.nInSeg;

	if(nOutSeg<0)
		return;

	CString strRegAddr, sAddr;
	char chRegType, chDataType;

// 	if(!bFromSliceIO)
		strRegAddr=pDoc->MkIo.MpeIo.pAddrOut[nOutSeg][0];
// 	else
// 		strRegAddr=pDoc->MkIo.SliceIo.pAddrOut[nOutSeg][0];

	chRegType = strRegAddr.GetAt(0);
	if(chRegType=='M' || chRegType=='m' || chRegType=='O' || chRegType=='o')
	{
		// Erase Bit data
		strRegAddr.Delete(strRegAddr.GetLength()-1);
		chDataType = strRegAddr.GetAt(1);
		switch(chDataType)
		{
		case 'B': 
			strRegAddr.SetAt(1,'W');
			Write(strRegAddr, (long)nOut);
			break;
		case 'b': 
			strRegAddr.SetAt(1,'w');
			Write(strRegAddr, (long)nOut);
			break;
		}
	}
}

void CMpDevice::Write(int *pSeg, int nTot, unsigned short *pData, BOOL bFromSliceIO)
{
	CString strRegAddr[2], sAddr;
	int nStSeg, nEdSeg, nSize, nStAddr, nEdAddr;
	char chRegType, chDataType;

// 	if(!bFromSliceIO)
// 	{
		nStSeg = pSeg[0] - pDoc->MkIo.MpeIo.nInSeg;
		nEdSeg = pSeg[nTot-1] - pDoc->MkIo.MpeIo.nInSeg;
// 	}
// 	else
// 	{
// 		if(pSeg[0] < pDoc->MkIo.SliceIo.nInSeg)
// 		{
// 			nStSeg = pSeg[0];
// 			nEdSeg = pSeg[nTot-1];
// 		}
// 		else
// 		{
// 			nStSeg = pSeg[0] - pDoc->MkIo.SliceIo.nInSeg;
// 			nEdSeg = pSeg[nTot-1] - pDoc->MkIo.SliceIo.nInSeg;
// 		}
// 	}

	if(nStSeg<0 || nEdSeg<0)
		return;

// 	if(!bFromSliceIO)
// 	{
		strRegAddr[0] = pDoc->MkIo.MpeIo.pAddrOut[nStSeg][0];
		strRegAddr[1] = pDoc->MkIo.MpeIo.pAddrOut[nEdSeg][0];
// 	}
// 	else
// 	{
// 		if(pSeg[0] < pDoc->MkIo.SliceIo.nInSeg)
// 		{
// 			strRegAddr[0] = pDoc->MkIo.SliceIo.pAddrIn[nStSeg][0];
// 			strRegAddr[1] = pDoc->MkIo.SliceIo.pAddrIn[nEdSeg][0];
// 		}
// 		else
// 		{
// 			strRegAddr[0] = pDoc->MkIo.SliceIo.pAddrOut[nStSeg][0];
// 			strRegAddr[1] = pDoc->MkIo.SliceIo.pAddrOut[nEdSeg][0];
// 		}
// 	}

	// Erase Bit data
	strRegAddr[0].Delete(strRegAddr[0].GetLength()-1);
	strRegAddr[1].Delete(strRegAddr[1].GetLength()-1);

	nStAddr = _tstoi(strRegAddr[0].Right(strRegAddr[0].GetLength()-2));
	nEdAddr = _tstoi(strRegAddr[1].Right(strRegAddr[1].GetLength()-2));

	if((nEdAddr-nStAddr) > 0)
	{
		sAddr = strRegAddr[0];
		nSize = (nEdAddr-nStAddr)+1;
	}
	else
	{
		sAddr = strRegAddr[1];
		nSize = (nStAddr-nEdAddr)+1;
	}

	chRegType = sAddr.GetAt(0);
	if(chRegType=='M' || chRegType=='m' || chRegType=='I' || chRegType=='i')
	{
		chDataType = sAddr.GetAt(1);
		switch(chDataType)
		{
		case 'B': 
			sAddr.SetAt(1,'W');
			SetIo(sAddr, pData, nSize);
			break;

		case 'b': 
			sAddr.SetAt(1,'w');
			SetIo(sAddr, pData, nSize);
			break;
		}
	}

}


void CMpDevice::GetMpeIO()
{
	// Reference(System APIs)
	// ymcGetGroupRegisterData : Gets the discontinuous register.
	/*
	DWORD					rc;							// Motion API return value
	REGISTER_INFO			RegInfo[2];					// Stores the discontinuous register information
	HREGISTERDATA			hRegisterData[2];			// Stores the register data handle
	WORD					pRegisterWData[30];			// Buffer of 2*30 bytes
	DWORD					pRegisterLData[10];			// Buffer of  4*10 bytes
	DWORD					GroupNumber;				// Setting Register Information Number 

	// Gets the regiter data handle of group 1
	rc = ymcGetRegisterDataHandle((LPBYTE)"MW00100", &hRegisterData[0]);
	// Error check processing
	if (rc != MP_SUCCESS)
	{
		;// MessageBox::Show(_T("ymcGetRegisterDataHandle ERROR"));
	}
	// Gets the regiter data handle of group 2
	rc = ymcGetRegisterDataHandle((LPBYTE)"ML00200", &hRegisterData[1]);
	// Error check processing
	if (rc != MP_SUCCESS)
	{
		;// MessageBox::Show(_T("ymcGetRegisterDataHandle ERROR"));
	}

	// information of group 1
	RegInfo[0].hRegisterData = hRegisterData[0];           // Register handle
	RegInfo[0].RegisterDataNumber = 30;                       // The number of register data
	RegInfo[0].pRegisterData = &pRegisterWData[0];      // Store register data

														// information of group 2
	RegInfo[1].hRegisterData = hRegisterData[1];           // Register handle
	RegInfo[1].RegisterDataNumber = 10;                       // The number of register data
	RegInfo[1].pRegisterData = &pRegisterLData[0];       // Store register data

	GroupNumber = 2;                                                  // Setting Register Information Number
																	  // Sets the register data for two groups of the discontinuous register information
	rc = ymcGetGroupRegisterData(GroupNumber, &RegInfo[0]);
	// Error check processing
	if (rc != MP_SUCCESS)
	{
		MessageBox(_T("ymcSetGroupRegisterData ERROR"), _T("ERROR"), MB_ICONERROR);
	}
	*/

	/* 
	DWORD					rc;							// Motion API return value
	REGISTER_INFO			RegInfo[2];					// Stores the discontinuous register information
	HREGISTERDATA			hRegisterData[2];			// Stores the register data handle.
	WORD					pRegisterWData[30];			// Buffer of 2*30 bytes
	DWORD					pRegisterLData[10];			// Buffer of  4*10 bytes
	DWORD					GroupNumber;				// Setting Reigster Information Number

	// Gets the regiter data handle of group 1
	rc = ymcGetRegisterDataHandle((LPBYTE)"MW00100", &hRegisterData[0]);
	// Error check processing
	if (rc != MP_SUCCESS)
	{
		MessageBox(_T("ymcGetRegisterDataHandle ERROR"), _T("ERROR"), MB_ICONERROR);
	}
	// Gets the regiter data handle of group 2
	rc = ymcGetRegisterDataHandle((LPBYTE)"ML00200", &hRegisterData[1]);
	// Error check processing
	if (rc != MP_SUCCESS)
	{
		MessageBox(_T("ymcGetRegisterDataHandle ERROR"), _T("ERROR"), MB_ICONERROR);
	}

	// information of group 1
	RegInfo[0].hRegisterData = hRegisterData[0];				// Register handle
	RegInfo[0].RegisterDataNumber = 30;							// The number of register data
	RegInfo[0].pRegisterData = &pRegisterWData[0];				// Store register data

																// information of group 2
	RegInfo[1].hRegisterData = hRegisterData[1];				// Register handle
	RegInfo[1].RegisterDataNumber = 10;							// The number of register data
	RegInfo[1].pRegisterData = &pRegisterLData[0];				// Store register data

	GroupNumber = 2;											// Setting Register Information Number
																// Sets the register data for two groups of the discontinuous register information
	rc = ymcGetGroupRegisterData(GroupNumber, &RegInfo[0]);
	// Error check processing
	if (rc != MP_SUCCESS)
	{
		MessageBox(_T("ymcSetGroupRegisterData ERROR"), _T("ERROR"), MB_ICONERROR);
	}
	*/

#ifdef USE_MPE
	// MpeIO
	int nSize, nIdx, nLoop, nSt, nBit;
	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;
	int nGrpStep = pDoc->MkIo.MpeIo.nGrpStep;
	int nGrpIn = pDoc->MkIo.MpeIo.nGrpIn;
	int nGrpOut = pDoc->MkIo.MpeIo.nGrpOut;

	if (nGrpIn + nGrpOut > 15)
	{
		AfxMessageBox(_T("Error - nInSeg+nOutSeg > 15"));
		return;
	}

	CString sAddr;
	//char cAddr[MAX_PATH];
	//TCHAR cAddr[MAX_PATH];
	char* cAddr;
	DWORD dwRC;
	HREGISTERDATA	hRegisterData[15] = { 0 };
	REGISTER_INFO	RegInfo[15];
	WORD RegisterWData[15][4];
	WORD RegWData;

	// Group Read...
	//nSt = pDoc->MkIo.MpeIo.nGrpInSt;
	nSt = 0;
	for (nLoop = 0; nLoop < nGrpIn; nLoop++)
	{
		sAddr = pDoc->MkIo.MpeIo.pAddrIn[nSt][0];
		sAddr.SetAt(1, 'W');
		//strcpy(cAddr, sAddr.Left(7));
		//wsprintf(cAddr, TEXT("%s"), sAddr.Left(7));
		cAddr = StringToChar(sAddr.Left(7));

		dwRC = ymcGetRegisterDataHandle((LPBYTE)cAddr, &hRegisterData[nLoop]);
		delete cAddr;

		RegInfo[nLoop].hRegisterData = hRegisterData[nLoop];	// Register handle
		RegInfo[nLoop].RegisterDataNumber = nGrpStep;		// The number of register data
		RegInfo[nLoop].pRegisterData = RegisterWData[nLoop];		// The number of register data
		nSt += nGrpStep;
	}

	pView->m_bChkMpeIoOut = FALSE;

	// Group Read...

	nSt = 0;
	for (nLoop = 0; nLoop < nGrpOut; nLoop++)
	{
		sAddr = pDoc->MkIo.MpeIo.pAddrOut[nSt][0];
		sAddr.SetAt(1, 'W');
		//strcpy(cAddr, sAddr.Left(7));
		//wsprintf(cAddr, TEXT("%s"), sAddr.Left(7));
		cAddr = StringToChar(sAddr.Left(7));

		dwRC = ymcGetRegisterDataHandle((LPBYTE)cAddr, &hRegisterData[nLoop + nGrpIn]);
		delete cAddr;

		RegInfo[nLoop + nGrpIn].hRegisterData = hRegisterData[nLoop + nGrpIn];	// Register handle
		RegInfo[nLoop + nGrpIn].RegisterDataNumber = nGrpStep;		// The number of register data
		RegInfo[nLoop + nGrpIn].pRegisterData = RegisterWData[nLoop + nGrpIn];		// The number of register data
		nSt += nGrpStep;
	}

	// Sets the register data for two groups of the discontinuous register information

	nSt = 0;

	for (nLoop = 0; nLoop < (nGrpIn+nGrpOut); nLoop++)
	{
		dwRC = ymcGetGroupRegisterData(1, &RegInfo[nLoop]);

		// Error check processing
		if (dwRC != MP_SUCCESS)
		{
			AfxMessageBox(_T("ymcGetGroupRegisterData ERROR"));
			return;
		}

		for (nIdx = 0; nIdx < nGrpStep; nIdx++)
		{
			RegWData = *((WORD*)RegInfo[nLoop].pRegisterData + nIdx);
			pDoc->m_pMpeIo[nIdx + nSt] = (unsigned short)RegWData;
		}
		nSt += nGrpStep;
		Sleep(10);
	}

/*	// MpeIO
	int nSize, nIdx, nLoop, nSt, nBit;
	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;
	int nGrpStep = pDoc->MkIo.MpeIo.nGrpStep;
	int nGrpIn = pDoc->MkIo.MpeIo.nGrpIn;
	int nGrpOut = pDoc->MkIo.MpeIo.nGrpOut;

	if(nGrpIn+nGrpOut > 15)
	{
		AfxMessageBox(_T("Error - nInSeg+nOutSeg > 15"));
		return;
	} 

	CString sAddr;
	//char cAddr[MAX_PATH];
	//TCHAR cAddr[MAX_PATH];
	char* cAddr;
	DWORD dwRC;
	HREGISTERDATA	hRegisterData[15] = {0};
	REGISTER_INFO	RegInfo[15];
	WORD RegisterWData[15][4];
	WORD RegWData;

	// Group Read...
	//nSt = pDoc->MkIo.MpeIo.nGrpInSt;
	nSt = 0;
	for(nLoop=0; nLoop<nGrpIn; nLoop++)
	{
		sAddr = pDoc->MkIo.MpeIo.pAddrIn[nSt][0];
		sAddr.SetAt(1,'W');
		//strcpy(cAddr, sAddr.Left(7));
		//wsprintf(cAddr, TEXT("%s"), sAddr.Left(7));
		cAddr = StringToChar(sAddr.Left(7));

		dwRC = ymcGetRegisterDataHandle((LPBYTE)cAddr, &hRegisterData[nLoop]);
		delete cAddr;

		RegInfo[nLoop].hRegisterData = hRegisterData[nLoop];	// Register handle
		RegInfo[nLoop].RegisterDataNumber = nGrpStep;		// The number of register data
		RegInfo[nLoop].pRegisterData = RegisterWData[nLoop];		// The number of register data
		nSt += nGrpStep;
	}

	pView->m_bChkMpeIoOut = FALSE;

	// Group Read...
	//nSt = pDoc->MkIo.MpeIo.nGrpOutSt;
	nSt = 0;
	for(nLoop=0; nLoop<nGrpOut; nLoop++)
	{
		sAddr = pDoc->MkIo.MpeIo.pAddrOut[nSt][0];
		sAddr.SetAt(1,'W');
		//strcpy(cAddr, sAddr.Left(7));
		//wsprintf(cAddr, TEXT("%s"), sAddr.Left(7));
		cAddr = StringToChar(sAddr.Left(7));

		dwRC = ymcGetRegisterDataHandle((LPBYTE)cAddr, &hRegisterData[nLoop+nGrpIn]);
		delete cAddr;

		RegInfo[nLoop+nGrpIn].hRegisterData = hRegisterData[nLoop+nGrpIn];	// Register handle
		RegInfo[nLoop+nGrpIn].RegisterDataNumber = nGrpStep;		// The number of register data
		RegInfo[nLoop+nGrpIn].pRegisterData = RegisterWData[nLoop+nGrpIn];		// The number of register data
		nSt += nGrpStep;
	}

	// Sets the register data for two groups of the discontinuous register information
	//dwRC = ymcGetGroupRegisterData( (nGrpIn+nGrpOut), &RegInfo[0] );
	dwRC = ymcGetGroupRegisterData(1, &RegInfo[0]);
	// Error check processing
	if( dwRC != MP_SUCCESS )
	{
		AfxMessageBox(_T("ymcGetGroupRegisterData ERROR"));
		return;
	}

	nSt = pDoc->MkIo.MpeIo.nGrpInSt;
	for(nLoop=0; nLoop<nGrpIn; nLoop++)
	{
		for(nIdx=0; nIdx<nGrpStep; nIdx++)
		{
			RegWData = *((WORD*)RegInfo[nLoop].pRegisterData+nIdx);
			pDoc->m_pMpeIo[nIdx+nSt] = (unsigned short)RegWData;
		}
		nSt += nGrpStep;
	}

	nSt = pDoc->MkIo.MpeIo.nGrpOutSt;
	for(nLoop=0; nLoop<nGrpOut; nLoop++)
	{
		for(nIdx=0; nIdx<nGrpStep; nIdx++)
		{
			RegWData = *((WORD*)RegInfo[nLoop+nGrpIn].pRegisterData+nIdx);
			pDoc->m_pMpeIo[nIdx+nSt] = (unsigned short)RegWData;
		}
		nSt += nGrpStep;
	}*/
#endif
}


void CMpDevice::GetMpeSignal()
{
#ifdef USE_MPE
	// MpeIO-I
	int nSize, nIdx, nLoop, nSt, k;
	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;
	int nGrpStep = pDoc->MkIo.MpeIo.nGrpStep;
	int nGrpIn = pDoc->MkIo.MpeIo.nGrpIn;
	int nGrpOut = pDoc->MkIo.MpeIo.nGrpOut;

	CString sAddr;
	//char cAddr[MAX_PATH];
	//TCHAR cAddr[MAX_PATH];
	char* cAddr;
	DWORD dwRC;
	HREGISTERDATA	hRegisterData[15] = { 0 };
	REGISTER_INFO	RegInfo[15];
	WORD RegisterWData[15][4];
	WORD RegWData;
	long RegisterLData[15][64];
	long RegLData;

	// Group Read...
	//nSt = pDoc->MkIo.MpeIo.nGrpInSt;
	nSt = 0;
	for (nLoop = 0; nLoop < nGrpIn; nLoop++)
	{
		sAddr = pDoc->MkIo.MpeIo.pSymIn[nSt][0];
		sAddr.SetAt(1, 'W');
		//strcpy(cAddr, sAddr.Left(6));
		//wsprintf(cAddr, TEXT("%s"), sAddr.Left(6));
		cAddr = StringToChar(sAddr.Left(6));
		dwRC = ymcGetRegisterDataHandle((LPBYTE)cAddr, &hRegisterData[nLoop]);
		delete cAddr;

		RegInfo[nLoop].hRegisterData = hRegisterData[nLoop];	// Register handle
		RegInfo[nLoop].RegisterDataNumber = nGrpStep;			// The number of register data
		RegInfo[nLoop].pRegisterData = RegisterWData[nLoop];	// The number of register data

		dwRC = ymcGetGroupRegisterData(1, &RegInfo[nLoop]);
		Sleep(10);

		// Error check processing
		if (dwRC != MP_SUCCESS)
		{
			AfxMessageBox(_T("ymcGetGroupRegisterData ERROR"));
			return;
		}
		
		for (nIdx = 0; nIdx < nGrpStep; nIdx++)
		{
			RegWData = *((WORD*)RegInfo[nLoop].pRegisterData + nIdx);
			pDoc->m_pMpeIb[nIdx + nSt] = (unsigned short)RegWData;
		}
		nSt += nGrpStep;
	}

	sAddr = pDoc->MkIo.MpeSignal.pAddrIn[0][0];
	sAddr.SetAt(1, 'W');
	//strcpy(cAddr, sAddr.Left(7));
	//wsprintf(cAddr, TEXT("%s"), sAddr.Left(7));
	cAddr = StringToChar(sAddr.Left(7));
	dwRC = ymcGetRegisterDataHandle((LPBYTE)cAddr, &hRegisterData[nLoop]);
	delete cAddr;

	RegInfo[nLoop].hRegisterData = hRegisterData[nLoop];	// Register handle
	RegInfo[nLoop].RegisterDataNumber = nGrpStep;			// The number of register data
	RegInfo[nLoop].pRegisterData = RegisterWData[nLoop];	// The number of register data

	dwRC = ymcGetGroupRegisterData(1, &RegInfo[nLoop]);
	Sleep(10);

	// Error check processing
	if (dwRC != MP_SUCCESS)
	{
		AfxMessageBox(_T("ymcGetGroupRegisterData ERROR"));
		return;
	}

	nSt = pDoc->MkIo.MpeSignal.nGrpInSt;
	for (nIdx = 0; nIdx < nGrpStep; nIdx++)
	{
		RegWData = *((WORD*)RegInfo[nLoop].pRegisterData + nIdx);
		pDoc->m_pMpeSignal[nIdx + nSt] = (unsigned short)RegWData;
	}
	
	nLoop++;

	sAddr = pDoc->MkIo.MpeSignal.pAddrOut[0][0];
	sAddr.SetAt(1, 'W');
	//strcpy(cAddr, sAddr.Left(7));
	//wsprintf(cAddr, TEXT("%s"), sAddr.Left(7));
	cAddr = StringToChar(sAddr.Left(7));
	dwRC = ymcGetRegisterDataHandle((LPBYTE)cAddr, &hRegisterData[nLoop]);
	delete cAddr;

	RegInfo[nLoop].hRegisterData = hRegisterData[nLoop];	// Register handle
	RegInfo[nLoop].RegisterDataNumber = nGrpStep;			// The number of register data
	RegInfo[nLoop].pRegisterData = RegisterWData[nLoop];	// The number of register data

	dwRC = ymcGetGroupRegisterData(1, &RegInfo[nLoop]);
	Sleep(10);

	// Error check processing
	if (dwRC != MP_SUCCESS)
	{
		AfxMessageBox(_T("ymcGetGroupRegisterData ERROR"));
		return;
	}

	nSt = pDoc->MkIo.MpeSignal.nGrpOutSt;
	for (nIdx = 0; nIdx < nGrpStep; nIdx++)
	{
		RegWData = *((WORD*)RegInfo[nLoop].pRegisterData + nIdx);
		pDoc->m_pMpeSignal[nIdx + nSt] = (unsigned short)RegWData;
	}
	
	nLoop++;

	sAddr = pDoc->MkIo.MpeData.pAddrIn[0][0];
	//strcpy(cAddr, sAddr);
	//wsprintf(cAddr, TEXT("%s"), sAddr);
	cAddr = StringToChar(sAddr);
	dwRC = ymcGetRegisterDataHandle((LPBYTE)cAddr, &hRegisterData[nLoop]);
	delete cAddr;

	RegInfo[nLoop].hRegisterData = hRegisterData[nLoop];	// Register handle
	RegInfo[nLoop].RegisterDataNumber = nGrpStep * 16;		// The number of register data
	RegInfo[nLoop].pRegisterData = RegisterLData[nLoop];	// The number of register data

	dwRC = ymcGetGroupRegisterData(1, &RegInfo[nLoop]);
	Sleep(10);

	// Error check processing
	if (dwRC != MP_SUCCESS)
	{
		AfxMessageBox(_T("ymcGetGroupRegisterData ERROR"));
		return;
	}

	nSt = pDoc->MkIo.MpeData.nGrpInSt;
	for (nIdx = 0; nIdx < nGrpStep; nIdx++)
	{
		for (k = 0; k < 16; k++)
		{
			RegLData = *((long*)RegInfo[nLoop].pRegisterData + (k + 16 * nIdx));
			pDoc->m_pMpeData[nIdx + nSt][k] = (long)RegLData;
		}
	}

	nLoop++;

	pView->m_bChkMpeIoOut = FALSE;

	sAddr = pDoc->MkIo.MpeData.pAddrOut[0][0];
	//strcpy(cAddr, sAddr);
	//wsprintf(cAddr, TEXT("%s"), sAddr);
	cAddr = StringToChar(sAddr);
	dwRC = ymcGetRegisterDataHandle((LPBYTE)cAddr, &hRegisterData[nLoop]);
	delete cAddr;

	RegInfo[nLoop].hRegisterData = hRegisterData[nLoop];	// Register handle
	RegInfo[nLoop].RegisterDataNumber = nGrpStep * 16;		// The number of register data
	RegInfo[nLoop].pRegisterData = RegisterLData[nLoop];	// The number of register data

	dwRC = ymcGetGroupRegisterData(1, &RegInfo[nLoop]);
	Sleep(10);

	// Error check processing
	if (dwRC != MP_SUCCESS)
	{
		AfxMessageBox(_T("ymcGetGroupRegisterData ERROR"));
		return;
	}

	nSt = pDoc->MkIo.MpeData.nGrpOutSt;
	for (nIdx = 0; nIdx < nGrpStep; nIdx++)
	{
		for (k = 0; k < 16; k++)
		{
			RegLData = *((long*)RegInfo[nLoop].pRegisterData + (k + 16 * nIdx));
			pDoc->m_pMpeData[nIdx + nSt][k] = (long)RegLData;
		}
	}


/*
	// Sets the register data for two groups of the discontinuous register information
	dwRC = ymcGetGroupRegisterData((nGrpIn + 2 + 2), &RegInfo[0]);
	// Error check processing
	if (dwRC != MP_SUCCESS)
	{
		AfxMessageBox(_T("ymcSetGroupRegisterData ERROR"));
		return;
	}

	nSt = 0;
	for (nLoop = 0; nLoop < nGrpIn; nLoop++)
	{
		for (nIdx = 0; nIdx < nGrpStep; nIdx++)
		{
			RegWData = *((WORD*)RegInfo[nLoop].pRegisterData + nIdx);
			pDoc->m_pMpeIb[nIdx + nSt] = (unsigned short)RegWData;
		}
		nSt += nGrpStep;
	}

	nSt = pDoc->MkIo.MpeSignal.nGrpInSt;
	for (nIdx = 0; nIdx < nGrpStep; nIdx++)
	{
		RegWData = *((WORD*)RegInfo[nLoop].pRegisterData + nIdx);
		pDoc->m_pMpeSignal[nIdx + nSt] = (unsigned short)RegWData;
	}

	nLoop++;

	nSt = pDoc->MkIo.MpeSignal.nGrpOutSt;
	for (nIdx = 0; nIdx < nGrpStep; nIdx++)
	{
		RegWData = *((WORD*)RegInfo[nLoop].pRegisterData + nIdx);
		pDoc->m_pMpeSignal[nIdx + nSt] = (unsigned short)RegWData;
	}

	nLoop++;

	nSt = pDoc->MkIo.MpeData.nGrpInSt;
	for (nIdx = 0; nIdx < nGrpStep; nIdx++)
	{
		for (k = 0; k < 16; k++)
		{
			RegLData = *((long*)RegInfo[nLoop].pRegisterData + (k + 16 * nIdx));
			pDoc->m_pMpeData[nIdx + nSt][k] = (long)RegLData;
		}
	}

	nLoop++;

	nSt = pDoc->MkIo.MpeData.nGrpOutSt;
	for (nIdx = 0; nIdx < nGrpStep; nIdx++)
	{
		for (k = 0; k < 16; k++)
		{
			RegLData = *((long*)RegInfo[nLoop].pRegisterData + (k + 16 * nIdx));
			pDoc->m_pMpeData[nIdx + nSt][k] = (long)RegLData;
		}
	}
	*/
#endif
}

//void CMpDevice::GetMpeSignal()
//{
//	/*
//
//	nSt = 0;
//
//	for (nLoop = 0; nLoop < (nGrpIn+nGrpOut); nLoop++)
//	{
//		dwRC = ymcGetGroupRegisterData(1, &RegInfo[nLoop]);
//
//		// Error check processing
//		if (dwRC != MP_SUCCESS)
//		{
//			AfxMessageBox(_T("ymcGetGroupRegisterData ERROR"));
//			return;
//		}
//
//		for (nIdx = 0; nIdx < nGrpStep; nIdx++)
//		{
//			RegWData = *((WORD*)RegInfo[nLoop].pRegisterData + nIdx);
//			pDoc->m_pMpeIo[nIdx + nSt] = (unsigned short)RegWData;
//		}
//		nSt += nGrpStep;
//		Sleep(10);
//	}
//
//	*/
//#ifdef USE_MPE
//	// MpeIO-I
//	int nSize, nIdx, nLoop, nSt, k;
//	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
//	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;
//	int nGrpStep = pDoc->MkIo.MpeIo.nGrpStep;
//	int nGrpIn = pDoc->MkIo.MpeIo.nGrpIn;
//	int nGrpOut = pDoc->MkIo.MpeIo.nGrpOut;
//
//	CString sAddr;
//	//char cAddr[MAX_PATH];
//	//TCHAR cAddr[MAX_PATH];
//	char* cAddr;
//	DWORD dwRC;
//	HREGISTERDATA	hRegisterData[15] = { 0 };
//	REGISTER_INFO	RegInfo[15];
//	WORD RegisterWData[15][4];
//	WORD RegWData;
//	long RegisterLData[15][64];
//	long RegLData;
//
//
//	// Group Read...
//	//nSt = pDoc->MkIo.MpeIo.nGrpInSt;
//	//nSt = 0;
//
//	//for (nLoop = 0; nLoop < nGrpIn; nLoop++)
//	//{
//	//	sAddr = pDoc->MkIo.MpeIo.pSymIn[nSt][0];
//	//	sAddr.SetAt(1, 'W');
//	//	//strcpy(cAddr, sAddr.Left(6));
//	//	//wsprintf(cAddr, TEXT("%s"), sAddr.Left(6));
//	//	cAddr = StringToChar(sAddr.Left(6)); // IB01230 ~  IB0123F
//
//	//	dwRC = ymcGetRegisterDataHandle((LPBYTE)cAddr, &hRegisterData[nLoop]);
//	//	delete cAddr;
//
//	//	RegInfo[nLoop].hRegisterData = hRegisterData[nLoop];	// Register handle
//	//	RegInfo[nLoop].RegisterDataNumber = nGrpStep;		// The number of register data
//	//	RegInfo[nLoop].pRegisterData = RegisterWData[nLoop];		// The number of register data
//	//	nSt += nGrpStep;
//	//}
//
//	nLoop = 0;
//
//	sAddr = pDoc->MkIo.MpeSignal.pAddrIn[0][0];
//	sAddr.SetAt(1, 'W');
//	//strcpy(cAddr, sAddr.Left(7));
//	//wsprintf(cAddr, TEXT("%s"), sAddr.Left(7));
//	cAddr = StringToChar(sAddr.Left(7));
//
//	dwRC = ymcGetRegisterDataHandle((LPBYTE)cAddr, &hRegisterData[nLoop]);
//	delete cAddr;
//
//	RegInfo[nLoop].hRegisterData = hRegisterData[nLoop];	// Register handle
//	RegInfo[nLoop].RegisterDataNumber = nGrpStep;		// The number of register data
//	RegInfo[nLoop].pRegisterData = RegisterWData[nLoop];		// The number of register data
//
//	dwRC = ymcGetGroupRegisterData(1, &RegInfo[nLoop]);
//
//	// Error check processing
//	if (dwRC != MP_SUCCESS)
//	{
//		AfxMessageBox(_T("ymcGetGroupRegisterData ERROR"));
//		return;
//	}
//
//	nSt = pDoc->MkIo.MpeSignal.nGrpInSt;
//	for (nIdx = 0; nIdx < nGrpStep; nIdx++)
//	{
//		RegWData = *((WORD*)RegInfo[nLoop].pRegisterData + nIdx);
//		pDoc->m_pMpeSignal[nIdx + nSt] = (unsigned short)RegWData;
//	}
//	Sleep(10);
//
//	//for (nIdx = 0; nIdx < nGrpStep; nIdx++)
//	//{
//	//	RegWData = *((WORD*)RegInfo[nLoop].pRegisterData + nIdx);
//	//	pDoc->m_pMpeIo[nIdx + nSt] = (unsigned short)RegWData;
//	//}
//	//nSt += nGrpStep;
//
//	nLoop++;
//
//	sAddr = pDoc->MkIo.MpeSignal.pAddrOut[0][0];
//	sAddr.SetAt(1, 'W');
//	//strcpy(cAddr, sAddr.Left(7));
//	//wsprintf(cAddr, TEXT("%s"), sAddr.Left(7));
//	cAddr = StringToChar(sAddr.Left(7));
//
//	dwRC = ymcGetRegisterDataHandle((LPBYTE)cAddr, &hRegisterData[nLoop]);
//	delete cAddr;
//
//	RegInfo[nLoop].hRegisterData = hRegisterData[nLoop];	// Register handle
//	RegInfo[nLoop].RegisterDataNumber = nGrpStep;			// The number of register data
//	RegInfo[nLoop].pRegisterData = RegisterWData[nLoop];	// The number of register data
//
//	dwRC = ymcGetGroupRegisterData(1, &RegInfo[nLoop]);
//
//	// Error check processing
//	if (dwRC != MP_SUCCESS)
//	{
//		AfxMessageBox(_T("ymcGetGroupRegisterData ERROR"));
//		return;
//	}
//
//	nSt = pDoc->MkIo.MpeSignal.nGrpOutSt;
//	for (nIdx = 0; nIdx < nGrpStep; nIdx++)
//	{
//		RegWData = *((WORD*)RegInfo[nLoop].pRegisterData + nIdx);
//		pDoc->m_pMpeSignal[nIdx + nSt] = (unsigned short)RegWData;
//	}
//	Sleep(10);
//
//	nLoop++;
//
//	sAddr = pDoc->MkIo.MpeData.pAddrIn[0][0];
//	//strcpy(cAddr, sAddr);
//	//wsprintf(cAddr, TEXT("%s"), sAddr);
//	cAddr = StringToChar(sAddr);
//
//	dwRC = ymcGetRegisterDataHandle((LPBYTE)cAddr, &hRegisterData[nLoop]);
//	delete cAddr;
//
//	RegInfo[nLoop].hRegisterData = hRegisterData[nLoop];	// Register handle
//	RegInfo[nLoop].RegisterDataNumber = nGrpStep * 16;		// The number of register data
//	RegInfo[nLoop].pRegisterData = RegisterLData[nLoop];	// The number of register data
//
//	dwRC = ymcGetGroupRegisterData(1, &RegInfo[nLoop]);
//
//	// Error check processing
//	if (dwRC != MP_SUCCESS)
//	{
//		AfxMessageBox(_T("ymcGetGroupRegisterData ERROR"));
//		return;
//	}
//
//	nSt = pDoc->MkIo.MpeData.nGrpInSt;
//	for (nIdx = 0; nIdx < nGrpStep; nIdx++)
//	{
//		for (k = 0; k < 16; k++)
//		{
//			RegLData = *((long*)RegInfo[nLoop].pRegisterData + (k + 16 * nIdx));
//			pDoc->m_pMpeData[nIdx + nSt][k] = (long)RegLData;
//		}
//	}
//	Sleep(10);
//
//	nLoop++;
//	pView->m_bChkMpeIoOut = FALSE;
//
//	sAddr = pDoc->MkIo.MpeData.pAddrOut[0][0];
//	//strcpy(cAddr, sAddr);
//	//wsprintf(cAddr, TEXT("%s"), sAddr);
//	cAddr = StringToChar(sAddr);
//
//	dwRC = ymcGetRegisterDataHandle((LPBYTE)cAddr, &hRegisterData[nLoop]);
//	delete cAddr;
//
//	RegInfo[nLoop].hRegisterData = hRegisterData[nLoop];	// Register handle
//	RegInfo[nLoop].RegisterDataNumber = nGrpStep * 16;		// The number of register data
//	RegInfo[nLoop].pRegisterData = RegisterLData[nLoop];	// The number of register data
//
//	dwRC = ymcGetGroupRegisterData(1, &RegInfo[nLoop]);
//
//	// Error check processing
//	if (dwRC != MP_SUCCESS)
//	{
//		AfxMessageBox(_T("ymcGetGroupRegisterData ERROR"));
//		return;
//	}
//
//	nSt = pDoc->MkIo.MpeData.nGrpOutSt;
//	for (nIdx = 0; nIdx < nGrpStep; nIdx++)
//	{
//		for (k = 0; k < 16; k++)
//		{
//			RegLData = *((long*)RegInfo[nLoop].pRegisterData + (k + 16 * nIdx));
//			pDoc->m_pMpeData[nIdx + nSt][k] = (long)RegLData;
//		}
//	}
//	Sleep(10);
//
//	
///*	// MpeIO-I
//	int nSize, nIdx, nLoop, nSt, k;
//	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
//	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;
//	int nGrpStep = pDoc->MkIo.MpeIo.nGrpStep;
//	int nGrpIn = pDoc->MkIo.MpeIo.nGrpIn;
//	int nGrpOut = pDoc->MkIo.MpeIo.nGrpOut;
//
//	CString sAddr;
//	//char cAddr[MAX_PATH];
//	//TCHAR cAddr[MAX_PATH];
//	char* cAddr;
//	DWORD dwRC;
//	HREGISTERDATA	hRegisterData[15] = {0};
//	REGISTER_INFO	RegInfo[15];
//	WORD RegisterWData[15][4];
//	WORD RegWData;
//	long RegisterLData[15][64];
//	long RegLData;
//
//
//	// Group Read...
//	//nSt = pDoc->MkIo.MpeIo.nGrpInSt;
//	nSt = 0;
//	for(nLoop=0; nLoop<nGrpIn; nLoop++)
//	{
//		sAddr = pDoc->MkIo.MpeIo.pSymIn[nSt][0];
//		sAddr.SetAt(1,'W');
//		//strcpy(cAddr, sAddr.Left(6));
//		//wsprintf(cAddr, TEXT("%s"), sAddr.Left(6));
//		cAddr = StringToChar(sAddr.Left(6));
//
//		dwRC = ymcGetRegisterDataHandle((LPBYTE)cAddr, &hRegisterData[nLoop]);
//		delete cAddr;
//
//		RegInfo[nLoop].hRegisterData = hRegisterData[nLoop];	// Register handle
//		RegInfo[nLoop].RegisterDataNumber = nGrpStep;		// The number of register data
//		RegInfo[nLoop].pRegisterData = RegisterWData[nLoop];		// The number of register data
//		nSt += nGrpStep;
//	}
//
//	sAddr = pDoc->MkIo.MpeSignal.pAddrIn[0][0];
//	sAddr.SetAt(1,'W');
//	//strcpy(cAddr, sAddr.Left(7));
//	//wsprintf(cAddr, TEXT("%s"), sAddr.Left(7));
//	cAddr = StringToChar(sAddr.Left(7));
//
//	dwRC = ymcGetRegisterDataHandle((LPBYTE)cAddr, &hRegisterData[nLoop]);
//	delete cAddr;
//
//	RegInfo[nLoop].hRegisterData = hRegisterData[nLoop];	// Register handle
//	RegInfo[nLoop].RegisterDataNumber = nGrpStep;		// The number of register data
//	RegInfo[nLoop].pRegisterData = RegisterWData[nLoop];		// The number of register data
//
//	nLoop++;
//
//	sAddr = pDoc->MkIo.MpeSignal.pAddrOut[0][0];
//	sAddr.SetAt(1,'W');
//	//strcpy(cAddr, sAddr.Left(7));
//	//wsprintf(cAddr, TEXT("%s"), sAddr.Left(7));
//	cAddr = StringToChar(sAddr.Left(7));
//
//	dwRC = ymcGetRegisterDataHandle((LPBYTE)cAddr, &hRegisterData[nLoop]);
//	delete cAddr;
//
//	RegInfo[nLoop].hRegisterData = hRegisterData[nLoop];	// Register handle
//	RegInfo[nLoop].RegisterDataNumber = nGrpStep;		// The number of register data
//	RegInfo[nLoop].pRegisterData = RegisterWData[nLoop];		// The number of register data
//
//	nLoop++;
//
//	sAddr = pDoc->MkIo.MpeData.pAddrIn[0][0];
//	//strcpy(cAddr, sAddr);
//	//wsprintf(cAddr, TEXT("%s"), sAddr);
//	cAddr = StringToChar(sAddr);
//
//	dwRC = ymcGetRegisterDataHandle((LPBYTE)cAddr, &hRegisterData[nLoop]);
//	delete cAddr;
//
//	RegInfo[nLoop].hRegisterData = hRegisterData[nLoop];	// Register handle
//	RegInfo[nLoop].RegisterDataNumber = nGrpStep*16;		// The number of register data
//	RegInfo[nLoop].pRegisterData = RegisterLData[nLoop];		// The number of register data
//
//	nLoop++;
//	pView->m_bChkMpeIoOut = FALSE;
//
//	sAddr = pDoc->MkIo.MpeData.pAddrOut[0][0];
//	//strcpy(cAddr, sAddr);
//	//wsprintf(cAddr, TEXT("%s"), sAddr);
//	cAddr = StringToChar(sAddr);
//
//	dwRC = ymcGetRegisterDataHandle((LPBYTE)cAddr, &hRegisterData[nLoop]);
//	delete cAddr;
//
//	RegInfo[nLoop].hRegisterData = hRegisterData[nLoop];	// Register handle
//	RegInfo[nLoop].RegisterDataNumber = nGrpStep*16;		// The number of register data
//	RegInfo[nLoop].pRegisterData = RegisterLData[nLoop];		// The number of register data
//
//
//	// Sets the register data for two groups of the discontinuous register information
//	//dwRC = ymcGetGroupRegisterData( (nGrpIn+2+2), &RegInfo[0] );
//	dwRC = ymcGetGroupRegisterData( 1, &RegInfo[0] );
//	// Error check processing
//	if( dwRC != MP_SUCCESS )
//	{
//		AfxMessageBox(_T("ymcSetGroupRegisterData ERROR"));
//		return;
//	}
//
//	nSt = 0;
//	for(nLoop=0; nLoop<nGrpIn; nLoop++)
//	{
//		for(nIdx=0; nIdx<nGrpStep; nIdx++)
//		{
//			RegWData = *((WORD*)RegInfo[nLoop].pRegisterData+nIdx);
//			pDoc->m_pMpeIo[nIdx+nSt] = (unsigned short)RegWData;
//		}
//		nSt += nGrpStep;
//	}
//
//	nSt = pDoc->MkIo.MpeSignal.nGrpInSt;
//	for(nIdx=0; nIdx<nGrpStep; nIdx++)
//	{
//		RegWData = *((WORD*)RegInfo[nLoop].pRegisterData+nIdx);
//		pDoc->m_pMpeSignal[nIdx+nSt] = (unsigned short)RegWData;
//	}
//
//	nLoop++;
//
//	nSt = pDoc->MkIo.MpeSignal.nGrpOutSt;
//	for(nIdx=0; nIdx<nGrpStep; nIdx++)
//	{
//		RegWData = *((WORD*)RegInfo[nLoop].pRegisterData+nIdx);
//		pDoc->m_pMpeSignal[nIdx+nSt] = (unsigned short)RegWData;
//	}
//
//	nLoop++;
//
//	nSt = pDoc->MkIo.MpeData.nGrpInSt;
//	for(nIdx=0; nIdx<nGrpStep; nIdx++)
//	{
//		for(k=0; k<16; k++)
//		{
//			RegLData = *((long*)RegInfo[nLoop].pRegisterData+(k+16*nIdx));
//			pDoc->m_pMpeData[nIdx+nSt][k] = (long)RegLData;
//		}
//	}
//
//	nLoop++;
//
//	nSt = pDoc->MkIo.MpeData.nGrpOutSt;
//	for(nIdx=0; nIdx<nGrpStep; nIdx++)
//	{
//		for(k=0; k<16; k++)
//		{
//			RegLData = *((long*)RegInfo[nLoop].pRegisterData+(k+16*nIdx));
//			pDoc->m_pMpeData[nIdx+nSt][k] = (long)RegLData;
//		}
//	}*/
//#endif
//}


#include "stdafx.h"

#include "CamIRayple.h"
#include "RGBConvert.h"


#include "../GvisR2R_PunchDoc.h"
#include "../GvisR2R_PunchView.h"

extern CGvisR2R_PunchDoc* pDoc;
extern CGvisR2R_PunchView* pView;

#define DEFAULT_SHOW_RATE (30)

// Data frame callback function
static void OnConnect(const IMV_SConnectArg* pConnectArg, void* pUser)
{
	CCamIRayple* pSingleDisplayDlg = (CCamIRayple*)pUser;
	if (!pSingleDisplayDlg)
	{
		TRACE("pSingleDisplayDlg is NULL!\n");
		return;
	}

	if (pConnectArg == NULL)
	{
		TRACE("pConnectArg is NULL!\n");
		return;
	}

	pSingleDisplayDlg->deviceLinkNotifyProc(*pConnectArg);
}

// Data frame callback function
static void onGetFrame(IMV_Frame* pFrame, void* pUser)
{
	CCamIRayple* pSingleDisplayDlg = (CCamIRayple*)pUser;
	if (!pSingleDisplayDlg)
	{
		TRACE("pSingleDisplayDlg is NULL!\n");
		return;
	}

	if (pFrame == NULL)
	{
		TRACE("pFrame is NULL\n");
		return;
	}

	pSingleDisplayDlg->frameProc(*pFrame);
}

// display thread
static unsigned int __stdcall displayThread(void* pUser)
{
	CCamIRayple* pSingleDisplayDlg = (CCamIRayple*)pUser;
	if (!pSingleDisplayDlg)
	{
		TRACE("pSingleDisplayDlg is NULL!\n");
		return -1;
	}

	pSingleDisplayDlg->displayProc();

	return 0;
}

CCamIRayple::CCamIRayple(int nIdx, HWND hCtrl, CWnd* pParent /*=NULL*/)
{
	m_nIdxCam = nIdx;
	m_pParent = pParent;
	m_hWndDisplay = hCtrl;
	::GetWindowRect(m_hWndDisplay, &m_rcDisplay);

	m_devHandle = NULL;
	m_displayThreadHandle = NULL;

	m_nWidth = 640;
	m_nHeight = 480;
	m_bConnected = FALSE;
	m_bResumeGrabbing = FALSE;
	m_bRunning = FALSE;
	m_sCameraKey = _T("");
	m_sFormat = _T("");
	m_sTriggerMode = _T("");
	m_dExposureEdit = 0.0;
	m_dFrameRateEdit = 0.0;
	m_dGainEdit = 0.0;
	m_dDisplayInterval = 0.0;
	m_lCamWidth = 640;
	m_lCamHeight = 480;

	setDisplayFPS(30);   // Default display 30 frames

	IMV_DeviceList deviceInfoList;
	if (IMV_OK != IMV_EnumDevices(&deviceInfoList, interfaceTypeAll))
	{
		AfxMessageBox(_T("Enum devices failed!"));
		return;
	}
	
	CString sCamSn;
	for (unsigned int index = 0; index < deviceInfoList.nDevNum; index++)
	{ 
		sCamSn = CharToString(deviceInfoList.pDevInfo[index].serialNumber);
		if(pDoc->WorkingInfo.System.sCamSn[nIdx] == sCamSn)
			m_sCameraKey = CharToString(deviceInfoList.pDevInfo[index].cameraKey);
	}

	if (m_sCameraKey.IsEmpty())
		AfxMessageBox(_T("Error - Camera serial number setting."));

	m_Render.setHandle(m_hWndDisplay);

	Connect(TRUE);
}


CCamIRayple::~CCamIRayple()
{
	//Connect(FALSE);
	while (!m_listImages.empty())
	{
		FrameBuffer* pConvertedImageDrop = m_listImages.front();
		m_listImages.pop_front();
		if (pConvertedImageDrop)
		{
			delete pConvertedImageDrop;
			pConvertedImageDrop = NULL;
		}
	}

	if (m_displayThreadHandle)
	{
		WaitForSingleObject(m_displayThreadHandle, INFINITE);
		CloseHandle(m_displayThreadHandle);
		m_displayThreadHandle = NULL;
	}

	if(m_devHandle)
	{
		IMV_DestroyHandle(m_devHandle);
		m_devHandle = NULL;
	}

}

void CCamIRayple::StringToChar(CString str, char *szStr)
{
	int nLen = str.GetLength();
	strcpy(szStr, CT2A(str));
	szStr[nLen] = _T('\0');
}

CString CCamIRayple::CharToString(char *szStr)
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

TCHAR* CCamIRayple::CharToTCHAR(char *str)
{
	int nLength = strlen(str) + sizeof(char);

	LPWSTR lpWideCharStr = new WCHAR[nLength];
	MultiByteToWideChar(CP_ACP, 0, str, -1, lpWideCharStr, nLength * sizeof(TCHAR));

	TCHAR *tszStr = NULL;
	tszStr = new TCHAR[nLength];

	USES_CONVERSION;
	_tcscpy(tszStr, W2T(lpWideCharStr));

	delete[] lpWideCharStr;
	return tszStr;
}

char* CCamIRayple::TCHARToChar(TCHAR *tszStr)
{
	char *szStr = NULL;
	int nLength;

	USES_CONVERSION;
	LPWSTR lpWideCharStr = T2W(tszStr);

	nLength = WideCharToMultiByte(CP_ACP, 0, lpWideCharStr, -1, NULL, NULL, NULL, NULL);
	szStr = new char[nLength + 1];
	WideCharToMultiByte(CP_ACP, 0, lpWideCharStr, -1, szStr, nLength, NULL, NULL);

	return szStr;
}

char* CCamIRayple::TCHARToChar(const TCHAR *tszStr)
{
	char *szStr = NULL;
	int nLength;

#ifdef UNICODE
	nLength = WideCharToMultiByte(CP_ACP, 0, tszStr, -1, NULL, NULL, NULL, NULL);
	szStr = new char[nLength + 1];
	WideCharToMultiByte(CP_ACP, 0, tszStr, -1, szStr, nLength, NULL, NULL);
#else
	USES_CONVERSION;
	LPWSTR lpWideCharStr = T2W(tszStr);

	nLength = WideCharToMultiByte(CP_ACP, 0, lpWideCharStr, -1, NULL, NULL, NULL, NULL);
	szStr = new char[nLength + 1];
	WideCharToMultiByte(CP_ACP, 0, lpWideCharStr, -1, szStr, nLength, NULL, NULL);
#endif

	return szStr;
}

BOOL CCamIRayple::OneshotGrab()
{
	CString sMsg;

	//int ret = IMV_OK;
	//IMV_HANDLE devHandle = (IMV_HANDLE)m_devHandle;
	//IMV_Frame frame;

	//if (NULL == devHandle)
	//{
	//	return FALSE;
	//}

	//// Get a frame image
	//ret = IMV_GetFrame(devHandle, &frame, 500);
	//if (IMV_OK != ret)
	//{
	//	sMsg.Format(_T("Get frame failed! ErrorCode[%d]\n"), ret);
	//	AfxMessageBox(sMsg);
	//	return FALSE;
	//}

	FrameBuffer* pConvertedImage = getConvertedImage();
	if (NULL != pConvertedImage)
	{
#ifdef USE_MIL
		//if (((CVision*)m_pParent)->m_pMil)
		//{
		//	if (m_nIdxCam == 0)
		//		((CVision*)m_pParent)->m_pMil->BufPutColor2d0((int)m_lCamWidth, (int)m_lCamHeight, (TCHAR*)frame.pData);
		//	else if (m_nIdxCam == 1)
		//		((CVision*)m_pParent)->m_pMil->BufPutColor2d1((int)m_lCamWidth, (int)m_lCamHeight, (TCHAR*)frame.pData);
		//}

		if (((CVision*)m_pParent)->m_pMil)
		{
			if (m_nIdxCam == 0)
				((CVision*)m_pParent)->m_pMil->BufPutColor2d0((int)pConvertedImage->Width(), (int)pConvertedImage->Height(), (TCHAR*)pConvertedImage->bufPtr());
			else if (m_nIdxCam == 1)
				((CVision*)m_pParent)->m_pMil->BufPutColor2d1((int)pConvertedImage->Width(), (int)pConvertedImage->Height(), (TCHAR*)pConvertedImage->bufPtr());
		}
#endif
		delete pConvertedImage;
		pConvertedImage = NULL;
	}
	else
	{
		AfxMessageBox(_T("Get frame failed!"));
		return FALSE;
	}

	//// Free image buffer
	//ret = IMV_ReleaseFrame(devHandle, &frame);
	//if (IMV_OK != ret)
	//{
	//	sMsg.Format(_T("Release frame failed! ErrorCode[%d]\n"), ret);
	//	AfxMessageBox(sMsg);
	//	return FALSE;
	//}

	return TRUE;
}

// display thread
void CCamIRayple::displayProc()
{
	while (m_bRunning)
	{
		FrameBuffer* pConvertedImage = getConvertedImage();

		if (NULL != pConvertedImage)
		{
			m_lCamWidth = pConvertedImage->Width();
			m_lCamHeight = pConvertedImage->Height();
#ifdef USE_MIL
			if(((CVision*)m_pParent)->m_pMil)
			{ 
				if(m_nIdxCam == 0)
					((CVision*)m_pParent)->m_pMil->BufPutColor2d0((int)m_lCamWidth, (int)m_lCamHeight, (TCHAR*)pConvertedImage->bufPtr());
				else if(m_nIdxCam == 1)
					((CVision*)m_pParent)->m_pMil->BufPutColor2d1((int)m_lCamWidth, (int)m_lCamHeight, (TCHAR*)pConvertedImage->bufPtr());
			}
#else
			m_Render.display(pConvertedImage->bufPtr(), (int)pConvertedImage->Width(), (int)pConvertedImage->Height(), pConvertedImage->PixelFormat());
#endif
			delete pConvertedImage;
			pConvertedImage = NULL;
		}
	}

	clearConvertedImage();
}

void CCamIRayple::deviceLinkNotifyProc(IMV_SConnectArg connectArg)
{
	if (!m_devHandle)
	{
		TRACE("m_devHandle is NULL\n");
		return;
	}

	if (connectArg.event == offLine)
	{
		m_bResumeGrabbing = IMV_IsGrabbing(m_devHandle);
		// Not grabbing status before disConnect
		if (!m_bResumeGrabbing)
		{
			return;
		}

		StopStreamGrabbing(TRUE);
	}
	else if (onLine == connectArg.event)
	{
		// Close camera 
		if (IMV_OK != IMV_Close(m_devHandle))
		{
			AfxMessageBox(_T("Close camera failed!"));
			return;
		}

		while (IMV_OK != IMV_Open(m_devHandle))
		{
			Sleep(500);
		}

		// Device connection status event callback function again
		if (IMV_OK != IMV_SubscribeConnectArg(m_devHandle, OnConnect, this))
		{
			AfxMessageBox(_T("Subscribe connect Failed!"));
			return;
		}

		// Not grabbing status before disConnect
		if (m_bResumeGrabbing)
		{
			// Start grabbing 
			StartStreamGrabbing(true);
		}
	}
}

void CCamIRayple::frameProc(IMV_Frame frame)
{
	// determine whether to display. If the upper display limit (30 frames) is exceeded, transcoding and display processing will not be performed
	if (!this->isTimeToDisplay())
	{
		return;
	}

	// transcoding
	FrameBuffer* pConvertFrameBuffer = new FrameBuffer(m_devHandle, frame);
	addConvertedImage(pConvertFrameBuffer);
}

void CCamIRayple::setDisplayFPS(int nFPS)
{
	m_mxTime.enter();
	if (nFPS > 0)
	{
		m_dDisplayInterval = 1000 * 1000 * 1000 / nFPS;
	}
	else
	{
		m_dDisplayInterval = 0;
	}
	m_mxTime.leave();
}

BOOL CCamIRayple::isTimeToDisplay()
{
	m_mxTime.enter();

	// not display
	if (m_dDisplayInterval <= 0)
	{
		m_mxTime.leave();
		return false;
	}

	// the first frame must be displayed
	if (m_nFirstFrameTime == 0 || m_nLastFrameTime == 0)
	{
		m_nFirstFrameTime = CTool::getCurTimeMicroSecond() * 1000;;
		m_nLastFrameTime = m_nFirstFrameTime;
		m_mxTime.leave();
		TRACE("set m_nFirstFrameTime: %I64d\n", m_nFirstFrameTime);
		return true;
	}

	uint64_t nCurTimeTmp = CTool::getCurTimeMicroSecond() * 1000;
	uint64_t nAcquisitionInterval = nCurTimeTmp - m_nLastFrameTime;
	if (nAcquisitionInterval > m_dDisplayInterval)
	{
		m_nLastFrameTime = nCurTimeTmp;
		m_mxTime.leave();
		return true;
	}

	// time interval between the current frame and the first frame
	uint64_t nPre = (m_nLastFrameTime - m_nFirstFrameTime) % m_dDisplayInterval;
	if (nPre + nAcquisitionInterval > m_dDisplayInterval)
	{
		m_nLastFrameTime = nCurTimeTmp;
		m_mxTime.leave();
		return true;
	}

	m_mxTime.leave();
	return false;
}


FrameBuffer* CCamIRayple::getConvertedImage()
{
	FrameBuffer* pConvertedImage = NULL;

	while (m_bRunning)
	{
		m_mutexQue.enter();

		if (m_listImages.empty())
		{
			m_mutexQue.leave();
			//Sleep(1) is to prevent the get frame callback from delivering to the display que slower than the display, resulting in high CPU utilization
			Sleep(1);

			continue;
		}
		else
		{
			pConvertedImage = m_listImages.front();
			m_listImages.pop_front();

			m_mutexQue.leave();

			break;
		}
	}

	return pConvertedImage;
}

void CCamIRayple::clearConvertedImage()
{
	m_mutexQue.enter();
	while (!m_listImages.empty())
	{
		FrameBuffer* pConvertedImageDrop = m_listImages.front();
		m_listImages.pop_front();
		if (pConvertedImageDrop)
		{
			delete pConvertedImageDrop;
			pConvertedImageDrop = NULL;
		}
	}
	m_mutexQue.leave();
}

void CCamIRayple::addConvertedImage(FrameBuffer* &pConvertedImage)
{
	static bool bThrowCtrl = true;

	m_mutexQue.enter();

	if (m_listImages.size() > DEFAULT_SHOW_RATE)
	{
		if (bThrowCtrl)
		{
			FrameBuffer* pConvertedImageDrop = m_listImages.front();
			m_listImages.pop_front();
			if (pConvertedImageDrop)
			{
				delete pConvertedImageDrop;
				pConvertedImageDrop = NULL;
			}
			bThrowCtrl = false;
		}
		else
		{
			bThrowCtrl = true;
			m_mutexQue.leave();
			return;
		}
	}

	m_listImages.push_back(pConvertedImage);
	m_mutexQue.leave();
}

void CCamIRayple::Connect(BOOL bConnect)
{
	if (bConnect)
	{
		if (m_devHandle)
		{
			IMV_DestroyHandle(m_devHandle);
			m_devHandle = NULL;
		}

		//if (!UpdateData(TRUE))
		//{
		//	return;
		//}

		//std::string cameraKey = m_sCameraKey.GetBuffer(m_sCameraKey.GetLength());
		//if (IMV_OK != IMV_CreateHandle(&m_devHandle, modeByCameraKey, (void*)cameraKey.c_str()))
		char cameraKey[1024];
		StringToChar(m_sCameraKey, cameraKey);

		if (IMV_OK != IMV_CreateHandle(&m_devHandle, modeByCameraKey, (void*)cameraKey))
		{
			AfxMessageBox(_T("Create handle Failed!"));
			return;
		}

		if (IMV_OK != IMV_Open(m_devHandle))
		{
			AfxMessageBox(_T("Open camera Failed!"));
			return;
		}

		m_bConnected = true;

		// Device connection status event callback function
		if (IMV_OK != IMV_SubscribeConnectArg(m_devHandle, OnConnect, this))
		{
			AfxMessageBox(_T("Subscribe connect Failed!"));
			return;
		}

		// initialize camera properties
		initTriggerModeParamProperty();
		Sleep(100);
		Getparam(); //OnBnClickedBtnGetparam();
	}
	else
	{
		StopStreamGrabbing(FALSE);

		if ((NULL != m_devHandle) && (IMV_OK != IMV_Close(m_devHandle)))
		{
			AfxMessageBox(_T("Close camera Failed!"));
		}

		IMV_DestroyHandle(m_devHandle);
		m_devHandle = NULL;

		m_bConnected = FALSE;
	}
}

void CCamIRayple::Getparam()
{
	initPixelFormatParamProperty();
	Sleep(100);
	initParamProperty();

	//UpdateData(FALSE);
}

void CCamIRayple::initTriggerModeParamProperty()
{
	IMV_String triggerModeSymbol;
	int ret = IMV_GetEnumFeatureSymbol(m_devHandle, "TriggerMode", &triggerModeSymbol);
	if (IMV_OK != ret)
	{
		AfxMessageBox(_T("Get TriggerMode symbol value failed!"));
		return;
	}

	//m_sTriggerMode = CString(_T(triggerModeSymbol.str));
	m_sTriggerMode = CharToString(triggerModeSymbol.str);

	unsigned int nEntryNum = 0;
	ret = IMV_GetEnumFeatureEntryNum(m_devHandle, "TriggerMode", &nEntryNum);
	if (IMV_OK != ret)
	{
		AfxMessageBox(_T("Get TriggerMode settable enumeration number failed!"));
		return;
	}

	IMV_EnumEntryList enumEntryList;
	enumEntryList.nEnumEntryBufferSize = sizeof(IMV_EnumEntryInfo) * nEntryNum;
	enumEntryList.pEnumEntryInfo = (IMV_EnumEntryInfo*)malloc(sizeof(IMV_EnumEntryInfo) * nEntryNum);
	if (NULL == enumEntryList.pEnumEntryInfo)
	{
		AfxMessageBox(_T("Malloc pEnumEntryInfo failed!"));
		return;
	}

	if (IMV_OK != IMV_GetEnumFeatureEntrys(m_devHandle, "TriggerMode", &enumEntryList))
	{
		free(enumEntryList.pEnumEntryInfo);
		enumEntryList.pEnumEntryInfo = NULL;
		AfxMessageBox(_T("Get TriggerMode settable enumeration value list failed!"));
		return;
	}

	//for (unsigned int iIndex = 0; iIndex < nEntryNum; ++iIndex)
	//{
	//	pCmbTriggerMode->InsertString(iIndex, (LPCTSTR)enumEntryList.pEnumEntryInfo[iIndex].name);
	//}
	bool bEnable = IMV_FeatureIsWriteable(m_devHandle, "TriggerMode");
	//pCmbTriggerMode->EnableWindow(bEnable);

	free(enumEntryList.pEnumEntryInfo);
	enumEntryList.pEnumEntryInfo = NULL;
}

void CCamIRayple::initPixelFormatParamProperty()
{
	IMV_String pixelFormatSymbol;
	int ret = IMV_GetEnumFeatureSymbol(m_devHandle, "PixelFormat", &pixelFormatSymbol);
	if (IMV_OK != ret)
	{
		AfxMessageBox(_T("Get PixelFormat symbol value failed!"));
		return;
	}

	//m_sFormat = CString(_T(pixelFormatSymbol.str));
	m_sFormat = CharToString(pixelFormatSymbol.str);

	unsigned int nEntryNum = 0;
	ret = IMV_GetEnumFeatureEntryNum(m_devHandle, "PixelFormat", &nEntryNum);
	if (IMV_OK != ret)
	{
		AfxMessageBox(_T("Get PixelFormat settable enumeration number failed!"));
		return;
	}

	IMV_EnumEntryList enumEntryList;
	enumEntryList.nEnumEntryBufferSize = sizeof(IMV_EnumEntryInfo) * nEntryNum;
	enumEntryList.pEnumEntryInfo = (IMV_EnumEntryInfo*)malloc(sizeof(IMV_EnumEntryInfo) * nEntryNum);
	if (NULL == enumEntryList.pEnumEntryInfo)
	{
		AfxMessageBox(_T("Malloc pEnumEntryInfo failed!"));
		return;
	}

	if (IMV_OK != IMV_GetEnumFeatureEntrys(m_devHandle, "PixelFormat", &enumEntryList))
	{
		free(enumEntryList.pEnumEntryInfo);
		enumEntryList.pEnumEntryInfo = NULL;
		AfxMessageBox(_T("Get PixelFormat settable enumeration value list failed!"));
		return;
	}

	//for (unsigned int iIndex = 0; iIndex < nEntryNum; ++iIndex)
	//{
	//	pCmbPixelFormat->InsertString(iIndex, (LPCTSTR)enumEntryList.pEnumEntryInfo[iIndex].name);
	//}

	bool bEnable = IMV_FeatureIsWriteable(m_devHandle, "PixelFormat");
	//pCmbPixelFormat->EnableWindow(bEnable);

	free(enumEntryList.pEnumEntryInfo);
	enumEntryList.pEnumEntryInfo = NULL;
}

void CCamIRayple::initParamProperty()
{
	if (IMV_OK != IMV_GetDoubleFeatureValue(m_devHandle, "AcquisitionFrameRate", &m_dFrameRateEdit))
	{
		AfxMessageBox(_T("Get AcquisitionFrameRate value failed!"));
		return;
	}

	if (IMV_OK != IMV_GetDoubleFeatureValue(m_devHandle, "ExposureTime", &m_dExposureEdit))
	{
		AfxMessageBox(_T("Get ExposureTime value failed!"));
		return;
	}

	if (IMV_OK != IMV_GetDoubleFeatureValue(m_devHandle, "GainRaw", &m_dGainEdit))
	{
		AfxMessageBox(_T("Get GainRaw value failed!"));
		return;
	}
}

void CCamIRayple::SetParamProperty()
{
	if (!IMV_IsGrabbing(m_devHandle))
	{
		//std::string pixelFormat = m_Format.GetBuffer(m_Format.GetLength());
		//if (IMV_OK != IMV_SetEnumFeatureSymbol(m_devHandle, "PixelFormat", pixelFormat.c_str()))
		char pixelFormat[1024];
		StringToChar(m_sFormat, pixelFormat);
		if (IMV_OK != IMV_SetEnumFeatureSymbol(m_devHandle, "PixelFormat", pixelFormat))
		{
			AfxMessageBox(_T("Set PixelFormat symbol value failed!"));
		}
	}

	if (IMV_OK != IMV_SetBoolFeatureValue(m_devHandle, "AcquisitionFrameRateEnable", true))
	{
		AfxMessageBox(_T("Set AcquisitionFrameRateEnable value failed!"));
	}

	if (IMV_OK != IMV_SetDoubleFeatureValue(m_devHandle, "AcquisitionFrameRate", m_dFrameRateEdit))
	{
		AfxMessageBox(_T("Set AcquisitionFrameRate value failed!"));
	}

	if (IMV_OK != IMV_SetDoubleFeatureValue(m_devHandle, "ExposureTime", m_dExposureEdit))
	{
		AfxMessageBox(_T("Set PixelFormat value failed!"));
	}

	if (IMV_OK != IMV_SetDoubleFeatureValue(m_devHandle, "GainRaw", m_dGainEdit))
	{
		AfxMessageBox(_T("Set PixelFormat value failed!"));
	}
}

void CCamIRayple::StartLive()
{
	if (NULL == m_devHandle)
	{
		return;
	}

	if (!IMV_IsGrabbing(m_devHandle))
	{
		StartStreamGrabbing(TRUE);
	}
}

void CCamIRayple::StopLive()
{
	if (NULL == m_devHandle)
	{
		return;
	}

	if (IMV_IsGrabbing(m_devHandle))
	{
		StopStreamGrabbing(FALSE);
	}
}

// start grabbing
BOOL CCamIRayple::StartStreamGrabbing(bool bResumeConnect)
{
	if (NULL == m_devHandle)
	{
		return false;
	}

	if (!bResumeConnect)
	{
		Getparam();
	}

	// open display
	if (!m_Render.isOpen())
	{
		m_Render.open();
		if (!m_Render.isOpen())
		{
			return false;
		}
	}

	if (IMV_OK != IMV_AttachGrabbing(m_devHandle, onGetFrame, this))
	{
		AfxMessageBox(_T("Attach grabbing Failed!"));
		return false;
	}

	Sleep(100);
	int nErr = IMV_StartGrabbing(m_devHandle);
	if (IMV_OK != nErr)
	{
		Sleep(100);
		nErr = IMV_StartGrabbing(m_devHandle);
		if (IMV_OK != nErr)
		{
			//AfxMessageBox(_T("Start Grabbing Failed!"));
			return FALSE; 
		}
	}

	m_bRunning = TRUE;

	// start display thread
	m_displayThreadHandle = (HANDLE)_beginthreadex(NULL,
		0,
		displayThread,
		this,
		CREATE_SUSPENDED,
		NULL);

	if (!m_displayThreadHandle)
	{
		TRACE("Failed to create display thread!\n");
		return false;
	}
	else
	{
		ResumeThread(m_displayThreadHandle);
	}


	SetTriggermode(m_sTriggerMode);

	return true;
}

// stop grabbing
BOOL CCamIRayple::StopStreamGrabbing(bool bResumeConnect)
{
	m_bRunning = FALSE;

	if (m_displayThreadHandle)
	{
		WaitForSingleObject(m_displayThreadHandle, INFINITE);
		CloseHandle(m_displayThreadHandle);
		m_displayThreadHandle = NULL;
	}

	m_Render.close();

	if (!IMV_IsGrabbing(m_devHandle))
	{
		return true;
	}

	if (IMV_OK != IMV_StopGrabbing(m_devHandle))
	{
		AfxMessageBox(_T("Start Grabbing Failed!"));
		return false;
	}

	if (!bResumeConnect)
	{
		SetTriggermode(m_sTriggerMode);
	}

	return true;
}

void CCamIRayple::SetTriggermode(CString sTriggerMode)
{
	//std::string triggermode = szBuf;

	if (IMV_OK != IMV_SetEnumFeatureSymbol(m_devHandle, "TriggerSelector", "FrameStart"))
	{
		AfxMessageBox(_T("Set TriggerSelector Failed!"));
		return;
	}

	char triggermode[1024];
	StringToChar(sTriggerMode, triggermode);

	//if (IMV_OK != IMV_SetEnumFeatureSymbol(m_devHandle, "TriggerMode", triggermode.c_str()))
	if (IMV_OK != IMV_SetEnumFeatureSymbol(m_devHandle, "TriggerMode", triggermode))
	{
		AfxMessageBox(_T("Set TriggerMode Failed!"));
	}

	m_sTriggerMode = sTriggerMode;

	BOOL isEnable = FALSE;

	if ((triggermode == "On") && IMV_IsGrabbing(m_devHandle))
	{
		isEnable = TRUE;
	}
}

int CCamIRayple::GetImgWidth()
{
	return m_nWidth;
}
int CCamIRayple::GetImgHeight()
{
	return m_nHeight;
}

// CamCrevis.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GvisR2R.h"
#include "Vision.h"
#include "CamCrevis.h"


#ifdef USE_CREVIS

extern CGvisR2RApp theApp;

// CCamCrevis

IMPLEMENT_DYNAMIC(CCamCrevis, CWnd)

CCamCrevis::CCamCrevis(int nIdx, HWND hCtrl, CWnd* pParent /*=NULL*/)
{
	m_nIdxCam = nIdx;
	m_pParent = pParent;
	m_hWndDisplay = hCtrl;
	::GetWindowRect(m_hWndDisplay, &m_rcDisplay);

	m_bThreadAlive = FALSE;	


	// TL
	m_hDevice					= NULL;
	m_hDevicePort				= NULL;
	m_hRegister					= NULL;

	// View
	m_pImage					= NULL;
	m_scrollPosition.x			= 0;
	m_scrollPosition.y			= 0;

	// Grab
//	m_hGrabThread				= NULL;	
	m_eTerminate				= NULL;	
	m_AcqCount					= 0;
	m_bIsGrabbing				= FALSE;

	//Frame info
	m_FrameCounter              = 0;
	m_OldFrameCounter			= 0;
	m_currentFrameRate          = 0;
	m_oldTime                   = 0; 
	m_LostImageCounter			= 0;

	m_GrayLevel					= 0;
	m_Width						= 0;
	m_Height					= 0;

	m_pCtrlDlg					= NULL;
	InitializeCriticalSection(&m_csImg);

	// Processing
//	m_pSetSharpeningDlg			= NULL;
	m_pCpyImg					= NULL;
	m_SharpnessType				= SHARPENESS_DISABLE;
	m_LaplacianFactor			= 0;
	m_LoGFactor					= 0;


	RECT rt={0,0,0,0};
	Create(NULL, _T("None"), WS_CHILD, rt, pParent, (UINT)this);

	Init();
}

CCamCrevis::~CCamCrevis()
{
	if(m_bThreadAlive)
	{
		m_ThreadTask.Stop();
		Sleep(20);
	}

	CloseDevice();

	if(m_eTerminate)
	{
		CloseHandle(m_eTerminate);
		m_eTerminate = NULL;
	}

	DeleteImage();

	DeleteCriticalSection(&m_csImg);
}


BEGIN_MESSAGE_MAP(CCamCrevis, CWnd)
	ON_MESSAGE(WM_MESSAGE_GRAB_THREAD, OnMessageGrabThread)
END_MESSAGE_MAP()



// CCamCrevis 메시지 처리기입니다.

CString CCamCrevis::GetLasetError()
{
	CString strErr;
	char szBuffer[4096];
	size_t iSize = 4096;

	strErr = "Error :\t";
	GC_ERROR status = GCGetLastError ( NULL, szBuffer, &iSize);
	if (status == GC_ERR_SUCCESS)  
		strErr += szBuffer;
	else
		strErr += "No additional information\r\n";

	return strErr;
}

void CCamCrevis::Init() 
{
	CloseDevice();

	if(!OpenDevice())	
		return;

	DeleteImage();
	if(!CreateImage())	
		return;

	// Get display name
	char szBuff[1024];
	INFO_DATATYPE iType;
	size_t iSize = 1024;
	m_strDispName.Empty();
	GC_ERROR status = DevGetInfo(m_hDevice, DEVICE_INFO_DISPLAYNAME, &iType, szBuff, &iSize);
	if(status < 0)
	{
		AfxMessageBox(GetLasetError());
		m_strDispName = "No Information";
	}
	else
	{
		m_strDispName = szBuff;
	}

	// Set camera information 
	//CMainFrame*   pParent = (CMainFrame*)GetParentFrame();
	//pParent->SetCameraInfo(m_strDispName);

	::InvalidateRect(m_hWndDisplay, NULL, FALSE);

	m_eTerminate = CreateEvent(NULL, true, false, NULL);

}


// Live Image Grab Process =======================================================
void CCamCrevis::StartThread() 
{
	m_bRtnThread = FALSE;
	m_bThreadAlive = TRUE;	
	m_ThreadTask.Start( GetSafeHwnd(),this,ThreadProc);// Start the thread
}

void CCamCrevis::StopThread() 
{
	long hResult = (long)m_ThreadTask.Stop();// Stop the thread
}

// Home thread body
BOOL CCamCrevis::ThreadProc( LPVOID lpContext )
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CCamCrevis* pThread = reinterpret_cast< CCamCrevis* >( lpContext );
	//	DWORD dwTimePeriod = 10; // 10ms sec sleep

	pThread->m_bRtnThread = FALSE;
	pThread->m_bThreadAlive = TRUE;	

	pThread->m_cs.Lock();
	pThread->m_bRtnThread = pThread->GrabThreadProc(pThread);
	pThread->m_cs.Unlock();

	pThread->m_bThreadAlive = FALSE;

	return (pThread->m_bRtnThread);
}


DWORD CCamCrevis::GrabThreadProc(void *pClassInst)
{
	//CCamCrevis	*pClass					= (CCamCrevis *)pClassInst;
	CCamCrevis		*pClass					= reinterpret_cast< CCamCrevis* >( pClassInst );
	DEV_HANDLE		hDevice					= pClass->m_hDevice;
	PORT_HANDLE		hDevicePort				= pClass->m_hDevicePort;
	HANDLE			hReg					= pClass->m_hRegister;
	int32_t			AcqCount				= pClass->m_AcqCount;
	CString			strDataType				= pClass->m_strDataType;
	int32_t			Width					= (int32_t)pClass->m_Width;
	int32_t			Height					= (int32_t)pClass->m_Height;
	DS_HANDLE		hDatastream				= NULL;
	EVENT_HANDLE	pEventNewBuffer			= NULL;
	GC_ERROR		status;
	INFO_DATATYPE   Type;
	BYTE*			pImageBuffer[COUNT_ALLOC_BUFF];
	void			*pNewImageEventData[2];
	int32_t			AcqStartStop;
	size_t			Size, ImageSize;
	uint32_t		AllocBuffNum, i;
	BUFFER_HANDLE	pBuffId = NULL;
	int64_t			i64Val;

	// Open DataStream
	hDatastream = pClass->GetDataStream(hDevice);
	if(hDatastream == NULL)	
	{
		pClass->PostMessage(WM_MESSAGE_GRAB_THREAD, ON_ERROR_GRAB_THREAD, 0);
		return -1;
	}

	// Alloc and queue buffer
	if(strDataType == "Mono8")
	{
		ImageSize = Width*Height;		// One pixel is one byte
	}
	else if(strDataType == "Mono10")
	{
		ImageSize = Width*Height*2;		// One pixel is two byte
	}
	else if(strDataType == "Mono12")
	{
		ImageSize = Width*Height*2;		// One pixel is two byte
	}
	else if(strDataType == "Mono14")
	{
		ImageSize = Width*Height*2;		// One pixel is two byte
	}
	// for color model
	else if(strDataType == "BayerBG8")
	{
		ImageSize = Width*Height;
	}
	else if(strDataType == "RGB8Packed")
	{
		ImageSize = Width*Height*3;
	}

	if(AcqCount == 1)		AllocBuffNum = 1;
	else					AllocBuffNum = COUNT_ALLOC_BUFF;

	memset(pImageBuffer, 0, sizeof(pImageBuffer));	
	for(i=0; i<AllocBuffNum; i++)
	{
		pImageBuffer[i] = new BYTE[ImageSize];
		status = DSAnnounceBuffer(hDatastream, pImageBuffer[i], ImageSize, NULL, &pBuffId);
		if(status < 0)	break;
		status = DSQueueBuffer(hDatastream, pBuffId);
		if(status < 0)	break;
	}
	if(status < 0)		goto ERROR_GRAB_THREAD;

	// Register New Buffer Event
	status = GCRegisterEvent(hDatastream, EVENT_NEW_BUFFER, &pEventNewBuffer);
	if (status < 0)		goto ERROR_GRAB_THREAD;

	// Run Acquisition engine
	status = DSStartAcquisition(hDatastream, ACQ_START_FLAGS_DEFAULT, (uint64_t)AcqCount);
	if (status < 0)		goto ERROR_GRAB_THREAD;

	// Send Acquisition Start
	AcqStartStop = 1;
	Size = sizeof(int32_t);
	status = RegSetData(hReg, REGMAP_ACQ_START_AND_STOP, REGISTER_INFO_EXECUTE, &Type, &AcqStartStop, &Size);
	if(status < 0)	
	{
		// Stop Acquisition engine
		DSStopAcquisition(hDatastream);
		goto ERROR_GRAB_THREAD;
	}

	// Notify to Viwer 
	pClass->PostMessage(WM_MESSAGE_GRAB_THREAD, ON_START_GRAB_THREAD, 0);

	while((WaitForSingleObject(pClass->m_eTerminate, 0) != WAIT_OBJECT_0) && (AcqCount != 0))
	{
		size_t iSize = sizeof(pNewImageEventData);
		status = EventGetData(pEventNewBuffer, &pNewImageEventData, &iSize, 100);
		if ( status  == GC_ERR_TIMEOUT) 
		{
			continue;
		}
		else if ( status  < 0)		
		{
			break;
		}
		else
		{
			void* pBase = NULL;
			iSize = sizeof(pBase);
			DSGetBufferInfo(hDatastream, pNewImageEventData[0], BUFFER_INFO_BASE, NULL, &pBase, &iSize );

			iSize = sizeof(int64_t);
			status = DSGetInfo(hDatastream, STREAM_INFO_NUM_DELIVERED, &Type, &i64Val, &iSize );
			if (status < 0)				
				break;

			pClass->m_FrameCounter = (double)((UINT)i64Val);

			iSize = sizeof(int64_t);
			status = DSGetInfo(hDatastream, STREAM_INFO_NUM_UNDERRUN, &Type, &i64Val, &iSize );
			if (status < 0)				
				break;
			pClass->m_LostImageCounter = (UINT)i64Val;
			AcqCount--;
			pClass->ReceiveImage(pBase);
			status = DSQueueBuffer(hDatastream, pNewImageEventData[0]);
			if (status < 0)				
				break;

		}
	}

	// Stop Acquisition engine
	DSStopAcquisition(hDatastream);

	// Unregister Event
	GCUnregisterEvent(hDatastream, EVENT_NEW_BUFFER);

	// Cleanup
	DSFlushQueue(hDatastream, ACQ_QUEUE_INPUT_TO_OUTPUT);
	DSFlushQueue(hDatastream, ACQ_QUEUE_OUTPUT_DISCARD);

	DSClose(hDatastream);
	hDatastream = NULL;

	// Send Acquisition Stop
	if((status >= 0) || (status == GC_ERR_TIMEOUT))
	{
		AcqStartStop = 0;
		Size = sizeof(int32_t);
		status = RegSetData(hReg, REGMAP_ACQ_START_AND_STOP, REGISTER_INFO_EXECUTE, &Type, &AcqStartStop, &Size);
	}

	// Free buffer
	for(i=0; i<AllocBuffNum; i++)		
	{
		if(pImageBuffer[i])	
			delete pImageBuffer[i];
		pImageBuffer[i] = NULL;
	}

	if(status < 0)
	{
		pClass->PostMessage(WM_MESSAGE_GRAB_THREAD, ON_ERROR_GRAB_THREAD, 0);
	}
	else
	{
		pClass->PostMessage(WM_MESSAGE_GRAB_THREAD, ON_STOP_GRAB_THREAD, 0);
	}

	return 0;

ERROR_GRAB_THREAD:
	// Cleanup
	DSFlushQueue(hDatastream, ACQ_QUEUE_INPUT_TO_OUTPUT);
	DSFlushQueue(hDatastream, ACQ_QUEUE_OUTPUT_DISCARD);

	// Close datastream
	DSClose(hDatastream);
	hDatastream = NULL;

	// Free buffer
	for(i=0; i<AllocBuffNum; i++)		
	{
		if(pImageBuffer[i])	
			delete pImageBuffer[i];
		pImageBuffer[i] = NULL;
	}

	pClass->PostMessage(WM_MESSAGE_GRAB_THREAD, ON_ERROR_GRAB_THREAD, 0);
	return -1;
}

LRESULT CCamCrevis::OnMessageGrabThread(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case ON_START_GRAB_THREAD:
		//{
		//	if(m_pCtrlDlg)
		//		((CControlPanelDlg*)m_pCtrlDlg)->EnableControls();
		//}
		break;
	case ON_ERROR_GRAB_THREAD:
		//{
		//	m_bIsGrabbing = FALSE;
		//	if(m_pCtrlDlg)
		//	{
		//		m_pCtrlDlg->DestroyWindow();
		//		delete m_pCtrlDlg;
		//		m_pCtrlDlg = NULL;
		//	}
		//	AfxMessageBox(GetLasetError());
		//}
		break;
	case ON_STOP_GRAB_THREAD:	
		//{
		//	m_bIsGrabbing = FALSE;
		//	if(m_pCtrlDlg)
		//	{
		//		((CControlPanelDlg*)m_pCtrlDlg)->EnableControls();
		//	}
		//}
		break;
	}

	return (LRESULT)0;
}

DS_HANDLE CCamCrevis::GetDataStream(DEV_HANDLE hDev)
{
	DS_HANDLE		hDatastream		= NULL;		
	GC_ERROR		status			= GC_ERR_SUCCESS;	
	INFO_DATATYPE	iType			= INFO_DATATYPE_UNKNOWN;
	char			szBuffer[1024]	= {0,};
	size_t			iSize			= 1024;
	uint32_t		iNumStreams		= 0;

	status = DevGetNumDataStreams(hDev, &iNumStreams);
	if(status < 0)		return NULL;

	// First datastream open
	iSize = 1024;
	status = DevGetDataStreamID( hDev, 0, szBuffer, &iSize);
	if(status < 0)		return NULL;

	iSize = 1024;
	status = DevOpenDataStream( hDev, szBuffer, &hDatastream);
	if(status < 0)		return NULL;

	return hDatastream;
}

void CCamCrevis::ReceiveImage(void* pSrcImg)
{
	ImageProc(pSrcImg);

	EnterCriticalSection(&m_csImg);
	if(m_pImage == NULL)
	{
		LeaveCriticalSection(&m_csImg);
		return;
	}

	// for color model
	if(m_strDataType == "Mono8")				
	{
		int i;
		BYTE** pDest = (BYTE** )m_pImage->GetPtr();
		BYTE* pBuff = (BYTE*)pSrcImg;
		for(i=0; i<m_Height; i++)
		{
			memcpy(pDest[i], &(pBuff[i*m_Width]), sizeof(BYTE)*m_Width);
		}
	}
	else if(m_strDataType == "Mono10")								
	{
		int i, j, idx;
		BYTE** pDest = (BYTE** )m_pImage->GetPtr();
		WORD* pBuff = (WORD*)pSrcImg;
		for(j=0; j<m_Height; j++)
		{
			idx = j*m_Width;
			for(i=0; i<m_Width; i++)
			{
				pDest[j][i] = (BYTE)(pBuff[idx+i]>>2);
			}
		}
	}
	else if(m_strDataType == "Mono12")								
	{
		int i, j, idx;
		BYTE** pDest = (BYTE** )m_pImage->GetPtr();
		WORD* pBuff = (WORD*)pSrcImg;
		for(j=0; j<m_Height; j++)
		{
			idx = j*m_Width;
			for(i=0; i<m_Width; i++)
			{
				pDest[j][i] = (BYTE)(pBuff[idx+i]>>4);
			}
		}
	}

	else if(m_strDataType == "Mono14")								
	{
		int i, j, idx;
		BYTE** pDest = (BYTE** )m_pImage->GetPtr();
		WORD* pBuff = (WORD*)pSrcImg;
		for(j=0; j<m_Height; j++)
		{
			idx = j*m_Width;
			for(i=0; i<m_Width; i++)
			{
				pDest[j][i] = (BYTE)(pBuff[idx+i]>>6);
			}
		}
	}
	else if(m_strDataType == "BayerBG8")
	{
		int i;
		BYTE** pDest = (BYTE** )m_pImage->GetPtr();
		BYTE* pBuff = (BYTE*)pSrcImg;
		for(i=0; i<m_Height; i++)
		{
			memcpy(pDest[i], &(pBuff[i*m_Width]), sizeof(BYTE)*m_Width);
		}

		////m_pMil->BufPutColor2d0(640, 480, (TCHAR*)pBase);
		//if(m_nIdxCam==0)
		//	((CVision*)m_pParent)->m_pMil->BufPutColor2d0(640, 480, (TCHAR*)(*pDest2));
		//else if(m_nIdxCam==1)
		//	((CVision*)m_pParent)->m_pMil->BufPutColor2d1(640, 480, (TCHAR*)(*pDest2));
	}
	else if(m_strDataType == "RGB8Packed")								
	{
		int i, SrcIdx, DstIdx, Offset;
		BYTE* pBuff = (BYTE*)pSrcImg;
		BYTE* pDest = (BYTE*)m_pImage->GetDibBits();
		SrcIdx = (m_Height-1)*m_Width*3;
		DstIdx = 0;
		Offset = m_Width*3;
		for(i=m_Height-1; i>=0; i--)
		{
			memcpy(&(pDest[SrcIdx]), &(pBuff[DstIdx]), sizeof(BYTE)*m_Width*3);
			SrcIdx -= Offset;
			DstIdx += Offset;
		}


		//m_pMil->BufPutColor2d0(640, 480, (TCHAR*)pBase);
		if(m_nIdxCam==0)
			((CVision*)m_pParent)->m_pMil->BufPutColor2d0(640, 480, (TCHAR*)(pDest));
		else if(m_nIdxCam==1)
			((CVision*)m_pParent)->m_pMil->BufPutColor2d1(640, 480, (TCHAR*)(pDest));

	}
	LeaveCriticalSection(&m_csImg);

	//DrawImage();
}


void CCamCrevis::ImageProc(void* pSrcImg)
{
	if(m_SharpnessType != SHARPENESS_DISABLE)
		ProcSharpening(pSrcImg);

	// TODO: Add others processing
}

void CCamCrevis::DrawImage()
{
	//m_scrollPosition = GetScrollPosition();
	m_scrollPosition.x = 0;
	m_scrollPosition.y = 0;

	double scase = 1;
	int w = m_pImage->GetWidth();
	int h = m_pImage->GetHeight();

	//CDC* pDC = this->GetDC();
	//CDC MemDC;
	//MemDC.CreateCompatibleDC(pDC);	
	//CBitmap	bitmap;
	//bitmap.CreateCompatibleBitmap(pDC, w, h);
	//CBitmap* pOldBitmap = (CBitmap*)MemDC.SelectObject(&bitmap);

	CWnd* pWnd = FromHandle(m_hWndDisplay);
	if(!pWnd || !m_hWndDisplay)
		return;

	CDC* pDC = pWnd->GetDC();
	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);	
	CBitmap	bitmap;
	bitmap.CreateCompatibleBitmap(pDC, w, h);
	CBitmap* pOldBitmap = (CBitmap*)MemDC.SelectObject(&bitmap);

	//HDC hDC = ::GetDC(m_hWndDisplay);
	//HDC memDC = ::CreateCompatibleDC(hDC);
	//HBITMAP memBM = ::CreateCompatibleBitmap(hDC, w, h);
	//HBITMAP memPrevBM = (HBITMAP)SelectObject(memDC, memBM);

	// Draw Image
	EnterCriticalSection(&m_csImg);

	m_pImage->Draw(MemDC.m_hDC, 0, 0, w, h);		
	//m_pImage->Draw(memDC, 0, 0, w, h);

	//m_pMil->BufPutColor2d0(640, 480, m_pIds[0]->GetImg());

	LeaveCriticalSection(&m_csImg);

	pDC->BitBlt(	0,
		0,
		m_rcDisplay.Width(),
		m_rcDisplay.Height(),
		&MemDC,
		(int)(m_scrollPosition.x),
		(int)(m_scrollPosition.y),
		SRCCOPY);



	MemDC.SelectObject(pOldBitmap);
	bitmap.DeleteObject();
	ReleaseDC(&MemDC);
	ReleaseDC(pDC);
}


BOOL CCamCrevis::CreateImage()
{
	size_t			Size;
	char			szBuffer[100] = {0,};
	GC_ERROR		status;

	// Get Width
	Size = sizeof(int32_t);
	status = RegGetData(m_hRegister, REGMAP_WIDTH, REGISTER_INFO_VALUE, NULL, &m_Width, &Size);
	if(status < 0)
	{
		AfxMessageBox(GetLasetError());
		return FALSE;
	}

	// Get Heigth
	Size = sizeof(int32_t);
	status = RegGetData(m_hRegister, REGMAP_HEIGHT, REGISTER_INFO_VALUE, NULL, &m_Height, &Size);
	if(status < 0)
	{
		AfxMessageBox(GetLasetError());
		return FALSE;
	}

	// Get PixelFormat
	Size = 100;
	status = RegGetData(m_hRegister, REGMAP_PIXEL_FORMAT, REGISTER_INFO_STRING, NULL, szBuffer, &Size);
	m_strDataType = szBuffer;
	if(status < 0)
	{
		AfxMessageBox(GetLasetError());
		return FALSE;
	}

	// Create Image
	EnterCriticalSection(&m_csImg);
	m_pImage = new CImage;

	// for color model
	if(m_strDataType == "Mono8")
	{
		m_pImage->CreateGrayImage(m_Width, m_Height, 0); 

		// for image processing
		m_pCpyImg = malloc(m_Width*m_Height);
	}
	else if(m_strDataType == "Mono10")
	{
		m_pImage->CreateGrayImage(m_Width, m_Height, 0); 

		// for image processing
		m_pCpyImg = malloc(m_Width*m_Height*2);
	}
	else if(m_strDataType == "Mono12")
	{
		m_pImage->CreateGrayImage(m_Width, m_Height, 0); 

		// for image processing
		m_pCpyImg = malloc(m_Width*m_Height*2);
	}
	else if(m_strDataType == "Mono14")
	{
		m_pImage->CreateGrayImage(m_Width, m_Height, 0); 

		// for image processing
		m_pCpyImg = malloc(m_Width*m_Height*2);
	}
	else if(m_strDataType == "BayerBG8")
	{
		m_pImage->CreateGrayImage(m_Width, m_Height, 0);
	}
	else if(m_strDataType == "RGB8Packed")
	{
		m_pImage->CreateRGBImage(m_Width, m_Height, 0); 

		// for image processing
		m_pCpyImg = malloc(m_Width*m_Height*3);
	}
	LeaveCriticalSection(&m_csImg);

	CSize sizeTotal;
	sizeTotal.cx = m_Width;
	sizeTotal.cy = m_Height;
	//SetScrollSizes(MM_TEXT, sizeTotal);

	// Image information
	CString strImageInfo; 
	CString strImageSize;
	strImageSize.Format("%d X %d", m_Width, m_Height);
	strImageInfo = strImageSize + "  (" + m_strDataType +")";
	//((CMainFrame*)GetParentFrame())->SetImageInfo(strImageInfo);
	return TRUE;
}

void CCamCrevis::DeleteImage()
{
	EnterCriticalSection(&m_csImg);
	if(m_pImage)
	{
		m_pImage->FreePtr();
		delete m_pImage;
		m_pImage = NULL;
	}
	LeaveCriticalSection(&m_csImg);

	if(m_pCpyImg)
	{
		free(m_pCpyImg);
		m_pCpyImg = NULL;
	}
}


void CCamCrevis::ProcSharpening(void* pSrcImg)
{
	if(m_strDataType == "Mono8")				
	{	
		switch(m_SharpnessType)
		{
		case SHARPENESS_DISABLE:
			break;
		case SHARPENESS_LAPLACIAN:
			memcpy(m_pCpyImg, pSrcImg, m_Width*m_Height);
			HighBoostFiltering_LaplacianMono8((BYTE*)m_pCpyImg, (BYTE*)pSrcImg, m_Width, m_Height, m_LaplacianFactor);
			break;
		case SHARPENESS_LOG:
			memcpy(m_pCpyImg, pSrcImg, m_Width*m_Height);
			HighBoostFiltering_LoGMono8((BYTE*)m_pCpyImg, (BYTE*)pSrcImg, m_Width, m_Height, m_LoGFactor);
			break;
		}
	}
	else if(m_strDataType == "Mono10")								
	{
		switch(m_SharpnessType)
		{
		case SHARPENESS_DISABLE:
			break;
		case SHARPENESS_LAPLACIAN:
			memcpy(m_pCpyImg, pSrcImg, m_Width*m_Height*2);
			HighBoostFiltering_LaplacianMono10((WORD*)m_pCpyImg, (WORD*)pSrcImg, m_Width, m_Height, m_LaplacianFactor);
			break;
		case SHARPENESS_LOG:
			memcpy(m_pCpyImg, pSrcImg, m_Width*m_Height*2);
			HighBoostFiltering_LoGMono10((WORD*)m_pCpyImg, (WORD*)pSrcImg, m_Width, m_Height, m_LoGFactor);
			break;
		}
	}
	else if(m_strDataType == "Mono12")								
	{
		switch(m_SharpnessType)
		{
		case SHARPENESS_DISABLE:
			break;
		case SHARPENESS_LAPLACIAN:
			memcpy(m_pCpyImg, pSrcImg, m_Width*m_Height*2);
			HighBoostFiltering_LaplacianMono10((WORD*)m_pCpyImg, (WORD*)pSrcImg, m_Width, m_Height, m_LaplacianFactor);
			break;
		case SHARPENESS_LOG:
			memcpy(m_pCpyImg, pSrcImg, m_Width*m_Height*2);
			HighBoostFiltering_LoGMono10((WORD*)m_pCpyImg, (WORD*)pSrcImg, m_Width, m_Height, m_LoGFactor);
			break;
		}
	}
	else if(m_strDataType == "Mono14")								
	{
		switch(m_SharpnessType)
		{
		case SHARPENESS_DISABLE:
			break;
		case SHARPENESS_LAPLACIAN:
			memcpy(m_pCpyImg, pSrcImg, m_Width*m_Height*2);
			HighBoostFiltering_LaplacianMono10((WORD*)m_pCpyImg, (WORD*)pSrcImg, m_Width, m_Height, m_LaplacianFactor);
			break;
		case SHARPENESS_LOG:
			memcpy(m_pCpyImg, pSrcImg, m_Width*m_Height*2);
			HighBoostFiltering_LoGMono10((WORD*)m_pCpyImg, (WORD*)pSrcImg, m_Width, m_Height, m_LoGFactor);
			break;
		}
	}
	else if(m_strDataType == "BayerBG8")
	{
		return;	
	}
	else if(m_strDataType == "RGB8Packed")								
	{
		switch(m_SharpnessType)
		{
		case SHARPENESS_DISABLE:
			break;
		case SHARPENESS_LAPLACIAN:
			memcpy(m_pCpyImg, pSrcImg, m_Width*m_Height*3);
			HighBoostFiltering_LaplacianRGB8Packed((BYTE*)m_pCpyImg, (BYTE*)pSrcImg, m_Width, m_Height, m_LaplacianFactor);
			break;
		case SHARPENESS_LOG:
			memcpy(m_pCpyImg, pSrcImg, m_Width*m_Height*3);
			HighBoostFiltering_LoGRGB8Packed((BYTE*)m_pCpyImg, (BYTE*)pSrcImg, m_Width, m_Height, m_LoGFactor);
			break;
		}
	}
}

void CCamCrevis::HighBoostFiltering_LaplacianMono8(BYTE* pSrc, BYTE* pDest, int Width, int Height, int Factor)
{
	// used mask = { -1,	 -1,	-1,
	//				 -1,	  8,	-1, 
	//				 -1,	 -1,	-1	} 

	int i, j;
	int idx_line0, idx_line1, idx_line2;
	int idx_pixel0, idx_pixel1, idx_pixel2;
	int new_width, new_height;
	int R0, R1, sharpness, val;
	float apply_factor;

	new_width = Width - 1;	new_height = Height - 1;
	idx_line0 = 0;			idx_line1 = Width;		idx_line2 = Width*2;
	idx_pixel0 = 0;			idx_pixel1 = 1;			idx_pixel2 = 2;		

	apply_factor = 0.01f * Factor;

	// Laplacian of a Gaussian
	for(j=2; j<new_height; j++)
	{
		for(i=2; i<new_width; i++)
		{
			// calc edge
			R1 = pSrc[idx_line0+idx_pixel0] + pSrc[idx_line0+idx_pixel1] + pSrc[idx_line0+idx_pixel2] +
				pSrc[idx_line1+idx_pixel0] + pSrc[idx_line1+idx_pixel2] + 
				pSrc[idx_line2+idx_pixel0] + pSrc[idx_line2+idx_pixel1] + pSrc[idx_line2+idx_pixel2];

			R0 = pSrc[idx_line1+idx_pixel1]<<3;

			sharpness = (R0 - R1) * apply_factor;

			// Add edge with scale
			val = pSrc[idx_line2+idx_pixel2] + sharpness;
			pDest[idx_line2+idx_pixel2] = LIMIT(val,255);	// New value

			idx_pixel0++;
			idx_pixel1++;
			idx_pixel2++;
		}
		idx_pixel0 = 0;			idx_pixel1 = 1;
		idx_pixel2 = 2;		

		idx_line0 += Width;		idx_line1 += Width;
		idx_line2 += Width;	
	}
}

void CCamCrevis::HighBoostFiltering_LoGMono8(BYTE* pSrc, BYTE* pDest, int Width, int Height, int Factor)
{
	// used mask = {  0,	  0,	-1,		0,		0,	
	//				  0,	 -1,	-2,	   -1,		0,
	//				 -1,	 -2,    16,	   -2,	   -1,
	//				  0,	 -1,	-2,	   -1,		0,
	//				  0,	  0,	-1,	    0,		0 }

	int i, j;
	int idx_line0, idx_line1, idx_line2, idx_line3, idx_line4;
	int idx_pixel0, idx_pixel1, idx_pixel2, idx_pixel3, idx_pixel4;
	int new_width, new_height;
	int R0, R1, R2, sharpness, val;
	float apply_factor;

	new_width = Width - 2;	new_height = Height - 2;

	idx_line0 = 0;			idx_line1 = Width;
	idx_line2 = Width*2;	idx_line3 = Width*3;
	idx_line4 = Width*4;

	idx_pixel0 = 0;			idx_pixel1 = 1;
	idx_pixel2 = 2;			idx_pixel3 = 3;
	idx_pixel4 = 4;

	apply_factor = 0.01f * Factor;

	// Laplacian of a Gaussian
	for(j=2; j<new_height; j++)
	{
		for(i=2; i<new_width; i++)
		{
			// calc edge
			R1 = pSrc[idx_line0+idx_pixel2] + 
				pSrc[idx_line1+idx_pixel1] + pSrc[idx_line1+idx_pixel3] + 
				pSrc[idx_line2+idx_pixel0] + pSrc[idx_line2+idx_pixel4] +
				pSrc[idx_line3+idx_pixel1] + pSrc[idx_line1+idx_pixel3] +
				pSrc[idx_line4+idx_pixel2];

			R2 = pSrc[idx_line1+idx_pixel2] + 
				pSrc[idx_line2+idx_pixel1] + pSrc[idx_line1+idx_pixel3] +
				pSrc[idx_line3+idx_pixel2];
			R2 <<= 1;

			R0 = pSrc[idx_line2+idx_pixel2]<<4;

			sharpness = (R0 - R1 - R2) * apply_factor;

			// Add edge with scale
			val = pSrc[idx_line2+idx_pixel2] + sharpness;
			pDest[idx_line2+idx_pixel2] = LIMIT(val,255);	// New value

			idx_pixel0++;
			idx_pixel1++;
			idx_pixel2++;
			idx_pixel3++;
			idx_pixel4++;
		}
		idx_pixel0 = 0;			idx_pixel1 = 1;
		idx_pixel2 = 2;			idx_pixel3 = 3;
		idx_pixel4 = 4;

		idx_line0 += Width;		idx_line1 += Width;
		idx_line2 += Width;		idx_line3 += Width;
		idx_line4 += Width;
	}
}

void CCamCrevis::HighBoostFiltering_LaplacianMono10(WORD* pSrc, WORD* pDest, int Width, int Height, int Factor)
{
	// used mask = { -1,	 -1,	-1,
	//				 -1,	  8,	-1, 
	//				 -1,	 -1,	-1	} 

	int i, j;
	int idx_line0, idx_line1, idx_line2;
	int idx_pixel0, idx_pixel1, idx_pixel2;
	int new_width, new_height;
	int R0, R1, sharpness, val;
	float apply_factor;

	new_width = Width - 1;	new_height = Height - 1;

	idx_line0 = 0;			idx_line1 = Width;		idx_line2 = Width*2;
	idx_pixel0 = 0;			idx_pixel1 = 1;			idx_pixel2 = 2;		
	apply_factor = 0.01f * Factor;

	// Laplacian of a Gaussian
	for(j=2; j<new_height; j++)
	{
		for(i=2; i<new_width; i++)
		{
			// calc edge
			R1 = pSrc[idx_line0+idx_pixel0] + pSrc[idx_line0+idx_pixel1] + pSrc[idx_line0+idx_pixel2] +
				pSrc[idx_line1+idx_pixel0] + pSrc[idx_line1+idx_pixel2] + 
				pSrc[idx_line2+idx_pixel0] + pSrc[idx_line2+idx_pixel1] + pSrc[idx_line2+idx_pixel2];

			R0 = pSrc[idx_line1+idx_pixel1]<<3;

			sharpness = (R0 - R1) * apply_factor;

			// Add edge with scale
			val = pSrc[idx_line2+idx_pixel2] + sharpness;
			pDest[idx_line2+idx_pixel2] = LIMIT(val,1023);	// New value

			idx_pixel0++;
			idx_pixel1++;
			idx_pixel2++;
		}
		idx_pixel0 = 0;			idx_pixel1 = 1;
		idx_pixel2 = 2;		

		idx_line0 += Width;		idx_line1 += Width;
		idx_line2 += Width;	
	}
}

void CCamCrevis::HighBoostFiltering_LoGMono10(WORD* pSrc, WORD* pDest, int Width, int Height, int Factor)
{
	// used mask = {  0,	  0,	-1,		0,		0,	
	//				  0,	 -1,	-2,	   -1,		0,
	//				 -1,	 -2,    16,	   -2,	   -1,
	//				  0,	 -1,	-2,	   -1,		0,
	//				  0,	  0,	-1,	    0,		0 }

	int i, j;
	int idx_line0, idx_line1, idx_line2, idx_line3, idx_line4;
	int idx_pixel0, idx_pixel1, idx_pixel2, idx_pixel3, idx_pixel4;
	int new_width, new_height;
	int R0, R1, R2, sharpness, val;
	float apply_factor;

	new_width = Width - 2;	new_height = Height - 2;

	idx_line0 = 0;			idx_line1 = Width;
	idx_line2 = Width*2;	idx_line3 = Width*3;
	idx_line4 = Width*4;

	idx_pixel0 = 0;			idx_pixel1 = 1;
	idx_pixel2 = 2;			idx_pixel3 = 3;
	idx_pixel4 = 4;

	apply_factor = 0.01f * Factor;

	// Laplacian of a Gaussian
	for(j=2; j<new_height; j++)
	{
		for(i=2; i<new_width; i++)
		{
			// calc edge
			R1 = pSrc[idx_line0+idx_pixel2] + 
				pSrc[idx_line1+idx_pixel1] + pSrc[idx_line1+idx_pixel3] + 
				pSrc[idx_line2+idx_pixel0] + pSrc[idx_line2+idx_pixel4] +
				pSrc[idx_line3+idx_pixel1] + pSrc[idx_line1+idx_pixel3] +
				pSrc[idx_line4+idx_pixel2];

			R2 = pSrc[idx_line1+idx_pixel2] + 
				pSrc[idx_line2+idx_pixel1] + pSrc[idx_line1+idx_pixel3] +
				pSrc[idx_line3+idx_pixel2];
			R2 <<= 1;

			R0 = pSrc[idx_line2+idx_pixel2]<<4;

			sharpness = (R0 - R1 - R2) * apply_factor;

			// Add edge with scale
			val = pSrc[idx_line2+idx_pixel2] + sharpness;
			pDest[idx_line2+idx_pixel2] = LIMIT(val,1023);	// New value

			idx_pixel0++;
			idx_pixel1++;
			idx_pixel2++;
			idx_pixel3++;
			idx_pixel4++;
		}
		idx_pixel0 = 0;			idx_pixel1 = 1;
		idx_pixel2 = 2;			idx_pixel3 = 3;
		idx_pixel4 = 4;

		idx_line0 += Width;		idx_line1 += Width;
		idx_line2 += Width;		idx_line3 += Width;
		idx_line4 += Width;
	}
}

void CCamCrevis::HighBoostFiltering_LaplacianRGB8Packed(BYTE* pSrc, BYTE* pDest, int Width, int Height, int Factor)
{
	// used mask = { -1,	 -1,	-1,
	//				 -1,	  8,	-1, 
	//				 -1,	 -1,	-1	}

	int i, j;
	int idx_line0, idx_line1, idx_line2;
	int idx_pixel0, idx_pixel1, idx_pixel2;
	int new_width, new_height;
	int R0, R1, sharpness, val;
	float apply_factor;

	new_width = Width - 2;	new_height = Height - 2;

	idx_line0 = 0;			idx_line1 = Width*3;	idx_line2 = Width*3*2;	
	idx_pixel0 = 0;			idx_pixel1 = 3;			idx_pixel2 = 6;			
	apply_factor = 0.01f * Factor;

	// Laplacian of a Gaussian
	for(j=2; j<new_height; j++)
	{
		for(i=2; i<new_width; i++)
		{
			// calc edge using green pixel
			R1 = pSrc[idx_line0+idx_pixel0+1] + pSrc[idx_line0+idx_pixel1+1] + pSrc[idx_line0+idx_pixel2+1] +
				pSrc[idx_line1+idx_pixel0+1] + pSrc[idx_line1+idx_pixel2+1] + 
				pSrc[idx_line2+idx_pixel0+1] + pSrc[idx_line0+idx_pixel1+1] + pSrc[idx_line0+idx_pixel2+1];

			R0 = pSrc[idx_line1+idx_pixel1+1]<<3;

			sharpness = (R0 - R1) * apply_factor;

			// Add edge value
			val = pSrc[idx_line2+idx_pixel2] + sharpness;
			pDest[idx_line2+idx_pixel2] = LIMIT(val,255);	// New Blue	

			val = pSrc[idx_line2+idx_pixel2+1] + sharpness;
			pDest[idx_line2+idx_pixel2+1] = LIMIT(val,255);	// New Green

			val = pSrc[idx_line2+idx_pixel2+2] + sharpness;
			pDest[idx_line2+idx_pixel2+2] = LIMIT(val,255);	// New Red

			idx_pixel0 += 3;
			idx_pixel1 += 3;
			idx_pixel2 += 3;
		}
		idx_pixel0 = 0;				idx_pixel1 = 3;	
		idx_pixel2 = 6;				

		idx_line0 += (Width*3);		idx_line1 += (Width*3);
		idx_line2 += (Width*3);		
	}
}

void CCamCrevis::HighBoostFiltering_LoGRGB8Packed(BYTE* pSrc, BYTE* pDest, int Width, int Height, int Factor)
{
	// used mask = {  0,	  0,	-1,		0,		0,	
	//				  0,	 -1,	-2,	   -1,		0,
	//				 -1,	 -2,    16,	   -2,	   -1,
	//				  0,	 -1,	-2,	   -1,		0,
	//				  0,	  0,	-1,	    0,		0 }

	int i, j;
	int idx_line0, idx_line1, idx_line2, idx_line3, idx_line4;
	int idx_pixel0, idx_pixel1, idx_pixel2, idx_pixel3, idx_pixel4;
	int new_width, new_height;
	int R0, R1, R2, sharpness, val;
	float apply_factor;

	new_width = Width - 2;	new_height = Height - 2;

	idx_line0 = 0;			idx_line1 = Width*3;
	idx_line2 = Width*3*2;	idx_line3 = Width*3*3;
	idx_line4 = Width*3*4;

	idx_pixel0 = 0;			idx_pixel1 = 3;
	idx_pixel2 = 6;			idx_pixel3 = 9;
	idx_pixel4 = 12;

	apply_factor = 0.01f * Factor;

	// Laplacian of a Gaussian
	for(j=2; j<new_height; j++)
	{
		for(i=2; i<new_width; i++)
		{
			// calc edge using green pixel
			R1 = pSrc[idx_line0+idx_pixel2+1] + 
				pSrc[idx_line1+idx_pixel1+1] + pSrc[idx_line1+idx_pixel3+1] + 
				pSrc[idx_line2+idx_pixel0+1] + pSrc[idx_line2+idx_pixel4+1] +
				pSrc[idx_line3+idx_pixel1+1] + pSrc[idx_line1+idx_pixel3+1] +
				pSrc[idx_line4+idx_pixel2+1];

			R2 = pSrc[idx_line1+idx_pixel2+1] + 
				pSrc[idx_line2+idx_pixel1+1] + pSrc[idx_line1+idx_pixel3+1] +
				pSrc[idx_line3+idx_pixel2+1];
			R2 <<= 1;

			R0 = pSrc[idx_line2+idx_pixel2+1]<<4;

			sharpness = (R0 - R1 - R2) * apply_factor;

			// Add edge value
			val = pSrc[idx_line2+idx_pixel2] + sharpness;
			pDest[idx_line2+idx_pixel2] = LIMIT(val,255);	// New Blue	

			val = pSrc[idx_line2+idx_pixel2+1] + sharpness;
			pDest[idx_line2+idx_pixel2+1] = LIMIT(val,255);	// New Green

			val = pSrc[idx_line2+idx_pixel2+2] + sharpness;
			pDest[idx_line2+idx_pixel2+2] = LIMIT(val,255);	// New Red

			idx_pixel0 += 3;
			idx_pixel1 += 3;
			idx_pixel2 += 3;
			idx_pixel3 += 3;
			idx_pixel4 += 3;
		}
		idx_pixel0 = 0;				idx_pixel1 = 3;	
		idx_pixel2 = 6;				idx_pixel3 = 9;
		idx_pixel4 = 12;

		idx_line0 += (Width*3);		idx_line1 += (Width*3);
		idx_line2 += (Width*3);		idx_line3 += (Width*3);
		idx_line4 += (Width*3);
	}
}



// General Function ========================================================

BOOL CCamCrevis::OpenDevice()
{
	m_hDevice = SelectDevice(theApp.m_hInterface);
	if(!m_hDevice)					
		return FALSE;

	// Open Deviceport
	m_hDevicePort = GetDevicePort(m_hDevice);
	if(!m_hDevicePort)
	{
		CloseDevice();
		return FALSE;
	}

	// Open RegisterMap
	m_hRegister = GetRegMap(m_hDevicePort);
	if(m_hRegister == NULL)			return FALSE;

	return TRUE;
}

DEV_HANDLE CCamCrevis::SelectDevice(IF_HANDLE hInterface)
{
	GC_ERROR status;
	DEV_HANDLE hDev = NULL;

	status = IFUpdateDeviceList(hInterface, 0);	
	if(status < 0)
	{
		AfxMessageBox(GetLasetError());
		return NULL;
	}

	uint32_t NumDevs = 0;
	status = IFGetNumDevices(hInterface, &NumDevs);
	if(status < 0)
	{
		AfxMessageBox(GetLasetError());
		return NULL;
	}

	if(NumDevs < 1)
	{
		AfxMessageBox("No Device");
		return NULL;
	}

	uint32_t i;
	//CDeviceSelectDlg dlg;
	for(i=0; i<NumDevs; i++)
	{
		char szDevID[1024];
		size_t iSize = 1024;
		status = IFGetDeviceID(hInterface, i, szDevID, &iSize);	
		if(status < 0)	
		{
			AfxMessageBox(GetLasetError());
			return NULL;
		}
		else
		{
			//dlg.AddList(i, CString(szDevID)); 
			m_DeviceNameList.AddTail(CString(szDevID));
		}
	}

	//if(dlg.DoModal() != IDOK)
	//{
	//	return NULL;
	//}

	IFUpdateDeviceList(hInterface, 500);

	CString str, strSelDev;
	POSITION pos1;
	pos1 = m_DeviceNameList.GetHeadPosition();
	for(i =0; i<m_DeviceNameList.GetCount(); i++)
	{
		str = m_DeviceNameList.GetNext(pos1);
		if(i==m_nIdxCam)
			strSelDev = str;
	}

	status = IFOpenDevice(hInterface, strSelDev.GetBuffer(0), (DEVICE_ACCESS_FLAGS)0, &hDev); 
	if(status < 0)
	{
		AfxMessageBox(GetLasetError());
		return NULL;
	}

	return hDev;
}

PORT_HANDLE	CCamCrevis::GetDevicePort(DEV_HANDLE hDev)
{
	PORT_HANDLE hDevicePort = NULL;
	GC_ERROR status = DevGetPort( hDev, &hDevicePort);
	if(status < 0)
	{
		AfxMessageBox(GetLasetError());
		return NULL;
	}
	return hDevicePort;
}


HANDLE CCamCrevis::GetRegMap(PORT_HANDLE hDevPort)
{
	GC_ERROR	status	= GC_ERR_SUCCESS;
	HANDLE		hReg	= NULL;	

	status = RegOpen(hDevPort, &hReg);
	if(status < 0)
	{
		AfxMessageBox(GetLasetError());
		return NULL;
	}
	return hReg;
}

void CCamCrevis::CloseDevice()
{
	if(m_hRegister)
	{
		RegClose(m_hRegister);
		m_hRegister = NULL;
	}

	if(m_hDevicePort)				
		m_hDevicePort = NULL;

	if(m_hDevice)
	{
		DevClose(m_hDevice);
		m_hDevice = NULL;
	}
}

void CCamCrevis::StartLive()
{
	m_AcqCount = 0xFFFFFFFF;
	GrabStart();
}

void CCamCrevis::GrabStart()
{
	if(m_bIsGrabbing)				
		return;

	// Check thread resource
//	if(m_bThreadAlive)		
//		return;	

	// Get last image information and create view
	DeleteImage();
	if(!CreateImage())						
		return;		

	m_OldFrameCounter = m_FrameCounter = 0;

	//thread start
	m_bIsGrabbing = TRUE;
	ResetEvent(m_eTerminate);
	//m_hGrabThread = CreateThread(NULL, 0, GrabThreadProc, this, 0, NULL);
	StartThread();

	::InvalidateRect(m_hWndDisplay, NULL, TRUE);
}

void CCamCrevis::StopLive()
{
	GrabStop();
}

void CCamCrevis::GrabStop()
{
	if(!m_bIsGrabbing)	return;

	SetEvent(m_eTerminate);
	//WaitForSingleObject(m_hGrabThread, INFINITE);
	//CloseHandle(m_hGrabThread);
	StopThread();
	//m_hGrabThread = NULL;
	m_bIsGrabbing = FALSE;
}


int CCamCrevis::GetImgWidth()
{
	return m_Width;
}

int CCamCrevis::GetImgHeight()
{
	return m_Height;
}


BOOL CCamCrevis::GetFramesPerSecond(double* dFPS)
{
	return TRUE;
}


#endif

//////////////////////////////////////////////////////////////////////////////////////////////

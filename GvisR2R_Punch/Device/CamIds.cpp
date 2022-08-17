// CamIds.cpp : implementation file
//

#include "stdafx.h"
#include "GvisR2R.h"
#include "CamIds.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCamIds
#ifdef USE_IDS

CCamIds::CCamIds(int nIdx, HWND hCtrl, CWnd* pParent /*=NULL*/)
{
	m_pParent = pParent;
	m_nIdxCam = nIdx;
	m_nIdxSeq = 0;

	m_hWndDisplay = hCtrl;
	m_pcImageMemory = NULL;
	m_lMemoryId = 0;
	m_hCam = 0;
//	m_nRenderMode = IS_RENDER_FIT_TO_WINDOW;
	m_nRenderMode = IS_RENDER_NORMAL;
	m_nPosX = 0;
	m_nPosY = 0;
	m_nFlipHor = 0;
	m_nFlipVert = 0;

	int i;
	for( i = 0; i < SEQ_BUFFERS; i++ )
	{
		m_lSeqMemId[i] = 0;
		m_pcSeqImgMem[i] = NULL;
	}

	RECT rt={0,0,0,0};
	Create(NULL, _T("None"), WS_CHILD, rt, pParent, (UINT)this);
}

CCamIds::~CCamIds()
{
	SeqKill();
	ExitCamera();
}


BEGIN_MESSAGE_MAP(CCamIds, CWnd)
	//{{AFX_MSG_MAP(CCamIds)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(IS_UEYE_MESSAGE, OnUEyeMessage)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCamIds message handlers

int CCamIds::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	if(!OpenCamera())
		return 1;
 	
	//if(!LoadParameters())
	//	return 2;

	return 0;
}

BOOL CCamIds::OpenCamera()
{
#ifdef USE_IDS
	INT nRet = IS_NO_SUCCESS;
    ExitCamera();
	
	// init camera (open next available camera)
	m_hCam = (HIDS) 0;								    
	nRet = InitCamera(&m_hCam, m_hWndDisplay);
	if (nRet == IS_SUCCESS)
	{
        // Get sensor info
		is_GetSensorInfo(m_hCam, &m_sInfo);
		
		GetMaxImageSize(&m_nSizeX, &m_nSizeY);
		
        nRet = InitDisplayMode();
		
        if (nRet == IS_SUCCESS)
        {
			//is_SetExternalTrigger(m_hCam, IS_SET_TRIGGER_SOFTWARE);

			//SeqBuilt();

			// Enable Messages
			is_EnableMessage(m_hCam, IS_DEVICE_REMOVED, GetSafeHwnd());
			is_EnableMessage(m_hCam, IS_DEVICE_RECONNECTED, GetSafeHwnd());
			is_EnableMessage(m_hCam, IS_FRAME, GetSafeHwnd());
			
			// start live video
			if(!is_CaptureVideo( m_hCam, IS_DONT_WAIT ))
				return FALSE;
			
		}
        else
			AfxMessageBox(TEXT("Initializing the display mode failed!"), MB_ICONWARNING );
		
 		//if(is_SetRopEffect(m_hCam, IS_SET_ROP_MIRROR_LEFTRIGHT, TRUE, NULL) != IS_SUCCESS)
 		//	return false;

		return TRUE;
	}
	else
	{
		AfxMessageBox(TEXT("No uEye camera could be opened !"), MB_ICONWARNING );
		return FALSE;
    }
#endif

	return TRUE;
}

LRESULT CCamIds::OnUEyeMessage( WPARAM wParam, LPARAM lParam )
{
#ifdef USE_IDS

// 	if(m_nIdxCam==0)
// 		m_pParent->SendMessage(IS_UEYE_MESSAGE0, wParam, lParam);
// 	else if(m_nIdxCam==1)
// 		m_pParent->SendMessage(IS_UEYE_MESSAGE1, wParam, lParam);
// 
// 	return 0;


	
	switch ( wParam )
	{
	case IS_DEVICE_REMOVED:
		Beep( 400, 50 );
		break;
	case IS_DEVICE_RECONNECTED:
		Beep( 400, 50 );
		break;
	case IS_FRAME:
		if( m_pcImageMemory != NULL )
		{
			if(!RenderBitmap())
				return 0;
			//is_RenderBitmap( m_hCam, m_lMemoryId, m_hWndDisplay, m_nRenderMode );
			//is_RenderBitmap( m_hCam, m_lMemoryId, m_hWndDisplay, IS_RENDER_NORMAL );
		}
		break;
	}    
//	return 0;

	if(m_nIdxCam==0)
		m_pParent->SendMessage(IS_UEYE_MESSAGE0, wParam, lParam);
	else if(m_nIdxCam==1)
		m_pParent->SendMessage(IS_UEYE_MESSAGE1, wParam, lParam);

//	m_pParent->PostMessage(IS_UEYE_MESSAGE, wParam, lParam);

#endif

	return 0;
}

BOOL CCamIds::RenderBitmap()
{
#ifdef USE_IDS
	// find the latest image buffer and display the buffer
	//INT nNum;
	//TCHAR *pcMem, *pcMemLast;
	//if(IS_SUCCESS != is_GetActSeqBuf(m_hCam, &nNum, &pcMem, &pcMemLast))
	//	return FALSE;
	//int j;
	//for(j=0 ; j<SEQ_BUFFERS ; j++)
	//{
	//	if( pcMemLast == m_pcSeqImgMem[j] )
	//	{
	//		m_nIdxSeq = j;
	//		m_pcImageMemory = pcMemLast;
	//		break;
	//	}
	//}

	

	int nRet = 0;
	nRet = is_LockSeqBuf(m_hCam, m_lMemoryId, m_pcImageMemory);
	if (nRet != IS_SUCCESS)
	{
		AfxMessageBox(_T("Error lock"));
		return 0;
	}

//	is_RenderBitmap( m_hCam, m_lSeqMemId[j], m_hWndDisplay, IS_RENDER_FIT_TO_WINDOW);
	//if(IS_SUCCESS != is_RenderBitmap( m_hCam, m_lSeqMemId[j], m_hWndDisplay, IS_RENDER_NORMAL))
	//	return FALSE;
	if(IS_SUCCESS != is_RenderBitmap( m_hCam, m_lMemoryId, m_hWndDisplay, IS_RENDER_NORMAL))
		nRet = FALSE;
	
	// unlock buffer
	nRet = is_UnlockSeqBuf( m_hCam, m_lMemoryId, m_pcImageMemory);

#endif

	return TRUE;
}

BOOL CCamIds::ExitCamera()
{
	BOOL bRst = TRUE;

#ifdef USE_IDS
	if( 0 != m_hCam )
	{
		// Disable messages
		if(IS_SUCCESS != is_EnableMessage( m_hCam, IS_FRAME, NULL ))
		{
			AfxMessageBox(_T("Failed - is_EnableMessage()"));
			bRst = FALSE;
		}
		
		// Stop live video
		if(IS_SUCCESS != is_StopLiveVideo( m_hCam, IS_WAIT ))
		{
			AfxMessageBox(_T("Failed - is_StopLiveVideo()"));
			bRst = FALSE;
		}
		
		// Free the allocated buffer
		if( NULL != m_pcImageMemory )
		{
			if(IS_SUCCESS != is_FreeImageMem( m_hCam, m_pcImageMemory, m_lMemoryId ))
			{
				AfxMessageBox(_T("Failed - is_FreeImageMem()"));
				bRst = FALSE;
			}
			else 
				m_pcImageMemory = NULL;
		}
 		
		// Close camera
		if(IS_SUCCESS == is_ExitCamera( m_hCam ))
		{
			m_hCam = NULL;
			bRst = TRUE;
		}
	}
	else
		return TRUE;
#endif

	return bRst;
}

int CCamIds::InitDisplayMode()
{
	INT nRet = IS_NO_SUCCESS;

#ifdef USE_IDS
    if (NULL == m_hCam)
        return IS_NO_SUCCESS;
	
    if (NULL != m_pcImageMemory)
    {
        is_FreeImageMem( m_hCam, m_pcImageMemory, m_lMemoryId );
    }
    m_pcImageMemory = NULL;
	
	// Set display mode to DIB
    nRet = is_SetDisplayMode(m_hCam, IS_SET_DM_DIB);
	if (IS_COLORMODE_BAYER == m_sInfo.nColorMode)
    {
		// setup the color depth to the current windows setting
        is_GetColorDepth(m_hCam, &m_nBitsPerPixel, &m_nColorMode);
    }
    else if (IS_COLORMODE_CBYCRY == m_sInfo.nColorMode)
    {
        // for color camera models use RGB32 mode
        m_nColorMode = IS_SET_CM_RGB32;
        m_nBitsPerPixel = 32;
    }
    else
    {
        // for monochrome camera models use Y8 mode
        m_nColorMode = IS_SET_CM_Y8;
        m_nBitsPerPixel = 8;
    }
	
    // allocate an image memory.
    if (IS_SUCCESS != is_AllocImageMem(m_hCam, m_nSizeX, m_nSizeY, m_nBitsPerPixel, &m_pcImageMemory, &m_lMemoryId ))
    {
        AfxMessageBox(TEXT("Memory allocation failed!"), MB_ICONWARNING );
    }
    else
        is_SetImageMem( m_hCam, m_pcImageMemory, m_lMemoryId );
	
	
    if (IS_SUCCESS == nRet)
    {
        // set the desired color mode
        is_SetColorMode(m_hCam, m_nColorMode);
		
        // set the image size to capture
        is_SetImageSize(m_hCam, m_nSizeX, m_nSizeY);
    }   
#endif

    return nRet;
}

INT CCamIds::InitCamera (HIDS *hCam, HWND hWnd)
{
	INT nRet = IS_NO_SUCCESS;

#ifdef USE_IDS
    INT nRet = is_InitCamera (hCam, hWnd);	
    /************************************************************************************************/
    /*                                                                                              */
    /*  If the camera returns with "IS_STARTER_FW_UPLOAD_NEEDED", an upload of a new firmware       */
    /*  is necessary. This upload can take several seconds. We recommend to check the required      */
    /*  time with the function is_GetDuration().                                                    */
    /*                                                                                              */
    /*  In this case, the camera can only be opened if the flag "IS_ALLOW_STARTER_FW_UPLOAD"        */ 
    /*  is "OR"-ed to m_hCam. This flag allows an automatic upload of the firmware.                 */
    /*                                                                                              */                        
    /************************************************************************************************/
    if (IS_STARTER_FW_UPLOAD_NEEDED == nRet)
    {
        // Time for the firmware upload = 25 seconds by default
        INT nUploadTime = 25000;
        is_GetDuration (*hCam, IS_STARTER_FW_UPLOAD, &nUploadTime);
		
        CString Str1, Str2, Str3;
        Str1 = "This camera requires a new firmware. The upload will take about";
        Str2 = "seconds. Please wait ...";
        Str3.Format ("%s %d %s", Str1, nUploadTime / 1000, Str2);
        AfxMessageBox (Str3, MB_ICONWARNING);
		
        // Set mouse to hourglass
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
		
        // Try again to open the camera. This time we allow the automatic upload of the firmware by
        // specifying "IS_ALLOW_STARTER_FIRMWARE_UPLOAD"
        *hCam = (HIDS) (((INT)*hCam) | IS_ALLOW_STARTER_FW_UPLOAD); 
        nRet = is_InitCamera (hCam, hWnd);   
    }
#endif

    return nRet;
}

void CCamIds::GetMaxImageSize(INT *pnSizeX, INT *pnSizeY)
{
#ifdef USE_IDS
    // Check if the camera supports an arbitrary AOI
    INT nAOISupported = 0;
    BOOL bAOISupported = TRUE;
    if (is_ImageFormat(m_hCam,
		IMGFRMT_CMD_GET_ARBITRARY_AOI_SUPPORTED, 
		(void*)&nAOISupported, 
		sizeof(nAOISupported)) == IS_SUCCESS)
    {
        bAOISupported = (nAOISupported != 0);
    }
	
    if (bAOISupported)
    {
        // Get maximum image size
		SENSORINFO sInfo;
		is_GetSensorInfo (m_hCam, &sInfo);
		*pnSizeX = sInfo.nMaxWidth;
		*pnSizeY = sInfo.nMaxHeight;
    }
    else
    {
        // Get image size of the current format
        *pnSizeX = is_SetImageSize(m_hCam, IS_GET_IMAGE_SIZE_X, 0);
        *pnSizeY = is_SetImageSize(m_hCam, IS_GET_IMAGE_SIZE_Y, 0);
    }
#endif

}

BOOL CCamIds::LoadParameters()
{
#ifdef USE_IDS
   if ( m_hCam == 0 )
	{
        if(!OpenCamera())
			return FALSE;
	}
	
    if ( m_hCam != 0 )
    {
		TCHAR cCameraParamFileName[1000];
		sprintf(cCameraParamFileName, _T("\\cam\\set%d"), 1);

//		if( is_LoadParameters( m_hCam, NULL ) == IS_SUCCESS && m_pcImageMemory != NULL )
		if( is_LoadParameters( m_hCam, cCameraParamFileName ) == IS_SUCCESS && m_pcImageMemory != NULL )
		{
			// determine live capture
			BOOL bWasLive = (BOOL)(is_CaptureVideo( m_hCam, IS_GET_LIVE ));
			if( bWasLive )
                is_StopLiveVideo(m_hCam, IS_FORCE_VIDEO_STOP);  
			
            // realloc image mem with actual sizes and depth.
            is_FreeImageMem( m_hCam, m_pcImageMemory, m_lMemoryId );
            m_nSizeX = is_SetImageSize( m_hCam, IS_GET_IMAGE_SIZE_X, 0 );
            m_nSizeY = is_SetImageSize( m_hCam, IS_GET_IMAGE_SIZE_Y, 0 );
			
			int nPixelMode = (int)is_SetColorMode( m_hCam, IS_GET_COLOR_MODE );
            switch( nPixelMode )
            {
			case IS_SET_CM_RGB32:
				m_nBitsPerPixel = 32;
				break;
			case IS_SET_CM_RGB24:
				m_nBitsPerPixel = 24;
				break;
			case IS_SET_CM_RGB16:
			case IS_SET_CM_UYVY:
				m_nBitsPerPixel = 16;
				break;
			case IS_SET_CM_RGB15:
				m_nBitsPerPixel = 15;
				break;
			case IS_SET_CM_Y8:
			case IS_SET_CM_RGB8:
			case IS_SET_CM_BAYER:
			default:
				m_nBitsPerPixel = 8;
				break;
            }  
			
			// memory initialization
			is_AllocImageMem( m_hCam, m_nSizeX, m_nSizeY, m_nBitsPerPixel, &m_pcImageMemory, &m_lMemoryId);

			// set memory active
			is_SetImageMem(m_hCam, m_pcImageMemory, m_lMemoryId );

			// display initialization
			is_SetImageSize(m_hCam, m_nSizeX, m_nSizeY );

			// run live image again
			if( bWasLive )
				is_CaptureVideo(m_hCam, IS_WAIT);
				//is_CaptureVideo(m_hCam, IS_DONT_WAIT);
        }
		else
			return FALSE;
    }  
	else
		return FALSE;
#endif
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      CuEyeEventDlg::SeqBuilt()
//
// DESCRIPTION: Built a sequence for acquisition into the sequence buffer
//
///////////////////////////////////////////////////////////////////////////////
BOOL CCamIds::SeqBuilt()
{
#ifdef USE_IDS
	INT	Ret;
    int i;
	
	SeqKill();
	
	// alloc seq buffer
	for( i=0; i< SEQ_BUFFERS ; i++ )
	{
		// alloc memory
		Ret = is_AllocImageMem( m_hCam,
								m_nSizeX,
								m_nSizeY,
								m_nBitsPerPixel,
								&m_pcSeqImgMem[i],
								&m_lSeqMemId[i]);
		if( Ret != IS_SUCCESS )
		{
			AfxMessageBox( "ERROR: cannot allocate sufficient sequence buffer memory" , MB_ICONEXCLAMATION, 0 );
			is_ExitCamera( m_hCam );
			return FALSE;
			//PostQuitMessage( 0 );
		}
	}
	for( i=0; i< SEQ_BUFFERS ; i++ )
	{
		// put memory into seq buffer
		Ret = is_AddToSequence(	m_hCam, m_pcSeqImgMem[i], m_lSeqMemId[i]);
		if( Ret != IS_SUCCESS )
		{
			AfxMessageBox( "ERROR: cannot put buffer into sequence" , MB_ICONEXCLAMATION, 0 );
			return FALSE;
		}
	}
#endif
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      CuEyeEventDlg::SeqKill()
//
// DESCRIPTION: Release the sequence for acquisition into the sequence buffer
//
///////////////////////////////////////////////////////////////////////////////
BOOL CCamIds::SeqKill()
{
#ifdef USE_IDS
	INT		Ret;

	is_ClearSequence( m_hCam );
	
	for( int i=0; i< SEQ_BUFFERS ; i++ )
	{
		if( (m_pcSeqImgMem[i] != NULL) && (m_lSeqMemId[i] != 0) )
		{
			// free memory
			Ret = is_FreeImageMem( m_hCam,
							         m_pcSeqImgMem[i],
									 m_lSeqMemId[i]);
			m_pcSeqImgMem[i] = NULL;
			m_lSeqMemId[i] = 0;
		}
	}
#endif

	return TRUE;
}


void CCamIds::Acquire()
{
#ifdef USE_IDS
	if ( m_hCam == 0 )
		OpenCamera();
	
	if ( m_hCam != 0 )
	{
		if( is_FreezeVideo( m_hCam, IS_WAIT ) == IS_SUCCESS )
			is_RenderBitmap( m_hCam, m_lMemoryId, m_hWndDisplay, m_nRenderMode );
	}
#endif
}

int CCamIds::StartLive() 
{
#ifdef USE_IDS
	int nRst = 0;
	if( m_hCam == 0 )
	{
		if(!OpenCamera())
			return 1;
	}
	
	if( m_hCam != 0 )
    {
		// Capture live video
		is_CaptureVideo( m_hCam, IS_WAIT );		
    }
#endif

	return 0;
}

void CCamIds::StopLive() 
{
#ifdef USE_IDS
	// stop live immediately - force stop
	if( m_hCam != 0 )
		is_StopLiveVideo( m_hCam, IS_FORCE_VIDEO_STOP );	
#endif
}

TCHAR* CCamIds::GetImg()
{
	return m_pcImageMemory;
}

int CCamIds::GetSeqImgMemIdx()
{
	return m_nIdxSeq;
}

int CCamIds::GetBitsPerPixel()
{
	return m_nBitsPerPixel;
}

double CCamIds::GetFramRate()
{
	double dblFPS = 0.0;
#ifdef USE_IDS
	int nResult = (int)is_GetFramesPerSecond ( m_hCam, &dblFPS );
#endif
	return dblFPS;
}

#endif

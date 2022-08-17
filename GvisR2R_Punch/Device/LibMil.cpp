// LibMil.cpp : implementation file
//

#include "stdafx.h"
#include "LibMil.h"
#include "../Global/GlobalDefine.h"

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
// CLibMil

CLibMil::CLibMil(int nIdx, MIL_ID &MilSysId, HWND *hCtrl, CWnd* pParent /*=NULL*/)
{
	m_nIdx = nIdx;
	m_pParent = pParent;

	m_lFontName = M_FONT_DEFAULT_SMALL;
	m_dFontScaleX = 1.0;
	m_dFontScaleY = 1.0;
	
	MilApplication = M_NULL;			/* Application identifier.		*/
	MilSystem = MilSysId;				/* System identifier.			*/
	MilImageCam = M_NULL;			    /* Image buffer identifier.			*/
	MilImageCamRotate = M_NULL;			    /* Image buffer identifier.			*/
	MilImageCamFlip = M_NULL;			    /* Image buffer identifier.			*/

	for(int i=0; i<4; i++)
	{
		MilDisplay[i] = NULL;			/* Display identifier.			*/
		MilImage[i] = NULL;				/* Image buffer identifier.		*/
		MilDraw[i] = NULL;

		UserWindowHandle[i] = hCtrl[i];
	}

	m_nCameraImageSizeX = 0;
	m_nCameraImageSizeY = 0;
	m_nDisplaySizeX = 0;
	m_nDisplaySizeY = 0;
	m_dFitResizeRatio = 0.0;
	m_dFitResizeRatioX = 0.0;
	m_dFitResizeRatioY = 0.0;
	m_nDisplayFitMode = DISPLAY_FIT_MODE_MIN_RATIO;

	//==================================================
	// pattern matching
	m_PatternMatchingModel = M_NULL;
	m_PatternMatchingResult = M_NULL;

	m_dPatternMatchingResultPosX = NULL;
	m_dPatternMatchingResultPosY = NULL;
	m_dPatternMatchingResultAngle = NULL;
	m_dPatternMatchingResultScore = NULL;
	m_dPatternMatchingResultTime = 0.0;

	//==================================================

	if(!m_nIdx)
	{
		MappAlloc(M_DEFAULT, &MilApplication);
	#ifdef _DEBUG
		MappControl(M_ERROR, M_PRINT_ENABLE);
	#else
		MappControl(M_ERROR, M_PRINT_DISABLE);
	#endif
		MsysAlloc(M_SYSTEM_HOST, M_DEF_SYSTEM_NUM, M_DEFAULT, &MilSystem);
		MilSysId = MilSystem;
	}



	RECT rt={0,0,0,0};
	Create(NULL, _T("None"), WS_CHILD, rt, pParent, (UINT)this);
}

CLibMil::CLibMil(int nIdx, int *nPixelMode, HWND *hCtrl, CWnd* pParent /*=NULL*/)
{
	m_nIdx = nIdx;
	m_pParent = pParent;

	m_lFontName = M_FONT_DEFAULT_SMALL;
	m_dFontScaleX = 1.0;
	m_dFontScaleY = 1.0;
	
	MilApplication = M_NULL;				/* Application identifier.  */
	MilSystem = M_NULL;						/* System identifier.       */
	MilImageCam = M_NULL;					/* Image buffer identifier.		*/
	MilImageCamRotate = M_NULL;				/* Image buffer identifier.		*/
	MilImageCamFlip = M_NULL;				/* Image buffer identifier.		*/

	for(int i=0; i<4; i++)
	{
		MilDisplay[i] = NULL;			    /* Display identifier.		*/
		MilImage[i] = NULL;					/* Image buffer identifier.	*/
		MilDraw[i] = NULL;
 
		UserWindowHandle[i] = hCtrl[i];
		m_nPixelMode[i] = nPixelMode[i];
	}

	m_nCameraImageSizeX = 0;
	m_nCameraImageSizeY = 0;
	m_nDisplaySizeX = 0;
	m_nDisplaySizeY = 0;
	m_dFitResizeRatio = 0.0;
	m_dFitResizeRatioX = 0.0;
	m_dFitResizeRatioY = 0.0;
	m_nDisplayFitMode = DISPLAY_FIT_MODE_MIN_RATIO;

	//==================================================
	// pattern matching
	m_PatternMatchingModel = M_NULL;
	m_PatternMatchingResult = M_NULL;

	m_dPatternMatchingResultPosX = NULL;
	m_dPatternMatchingResultPosY = NULL;
	m_dPatternMatchingResultAngle = NULL;
	m_dPatternMatchingResultScore = NULL;
	m_dPatternMatchingResultTime = 0.0;

	//==================================================


	RECT rt={0,0,0,0};
	Create(NULL, _T("None"), WS_CHILD, rt, pParent, (UINT)this);
}

CLibMil::~CLibMil()
{
// 	/* Remove the MIL buffer from the display. */
// 	MdispSelect(MilDisplay[0], M_NULL);
// 	MdispSelect(MilDisplay[1], M_NULL);
// 	MdispSelect(MilDisplay[2], M_NULL);
// 	MdispSelect(MilDisplay[3], M_NULL);
// 	
// 	/* Free allocated objects. */
// 	MbufFree(MilImage[0]);
// 	MbufFree(MilImage[1]);
// 	MbufFree(MilImage[2]);
// 	MbufFree(MilImage[3]);
// 	MdispFree(MilDisplay[0]);
// 	MdispFree(MilDisplay[1]);
// 	MdispFree(MilDisplay[2]);
// 	MdispFree(MilDisplay[3]);

	if(MilImageCamFlip)
	{
		pView->m_nDebugStep = 100; pView->DispThreadTick();
		MbufFree(MilImageCamFlip);
		MilImageCamFlip = M_NULL;
	}

	if(MilImageCamRotate)
	{
		pView->m_nDebugStep = 101; pView->DispThreadTick();
		MbufFree(MilImageCamRotate);
		MilImageCamRotate = M_NULL;
	}

	if(MilImageCam)
	{
		pView->m_nDebugStep = 102; pView->DispThreadTick();
		MbufFree(MilImageCam);
		MilImageCam = M_NULL;
	}


	for(int i=0; i<4; i++)
	{
		if(MilDraw[i])
		{
			delete MilDraw[i];
			MilDraw[i] = NULL;
		}
		if(MilImage[i])
		{
			delete MilImage[i];
			MilImage[i] = NULL;
		}
		if(MilDisplay[i])
		{
			delete MilDisplay[i];
			MilDisplay[i] = NULL;
		}
	}

	if(!m_nIdx)
	{
		pView->m_nDebugStep = 300; pView->DispThreadTick();
		MsysFree(MilSystem);
		MappFree(MilApplication);
	}

// 	/* Free defaults. */
// 	MappFreeDefault(MilApplication, MilSystem, MilDisplay, M_NULL, MilImage);
}


BEGIN_MESSAGE_MAP(CLibMil, CWnd)
	//{{AFX_MSG_MAP(CLibMil)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CLibMil message handlers

int CLibMil::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here

// 	/* Allocate a default MIL application, system, display and image. */
// 	MappAllocDefault(M_COMPLETE, &MilApplication, &MilSystem, &MilDisplay, M_NULL, &MilImage);
// 
// 	/* If no allocation errors. */
// 	if (MappGetError(M_GLOBAL, M_NULL))
// 	{
// 		TRACE("System allocation error !\n\n");
// 		return -1;
// 	}
// 
// 	/* Perform graphic operations in the display image. */ 
// 	MgraColor(M_DEFAULT, 0xF0);
// 	MgraFont(M_DEFAULT, M_FONT_DEFAULT_LARGE);
// 	MgraText(M_DEFAULT, MilImage, 160L, 230L, MIL_TEXT(" Welcome to MIL !!! "));
// 	MgraColor(M_DEFAULT, 0xC0);
// 	MgraRect(M_DEFAULT, MilImage, 100L, 150L, 530L, 340L);
// 	MgraRect(M_DEFAULT, MilImage, 120L, 170L, 510L, 320L);
// 	MgraRect(M_DEFAULT, MilImage, 140L, 190L, 490L, 300L);
// 
// 	/* Print a message. */
// 	TRACE("\nSYSTEM ALLOCATION:\n");
// 	TRACE("------------------\n\n");
// 	TRACE("System allocation successful.\n\n");
// 	TRACE("     \"Welcome to MIL !!!\"\n\n");
#ifdef USE_IRAYPLE
	MbufAllocColor(MilSystem, 1, 720, 540, 8L + M_UNSIGNED, M_IMAGE + M_DISP + M_PROC, &MilImageCam);
	MbufClear(MilImageCam, 0);
	MbufAllocColor(MilSystem, 1, 720, 540, 8L + M_UNSIGNED, M_IMAGE + M_DISP + M_PROC, &MilImageCamRotate);
	MbufClear(MilImageCamRotate, 0);
	MbufAllocColor(MilSystem, 1, 720, 540, 8L + M_UNSIGNED, M_IMAGE + M_DISP + M_PROC, &MilImageCamFlip);
	MbufClear(MilImageCamFlip, 0);
#else
	MbufAllocColor(MilSystem, 3, 640, 480, 8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &MilImageCam);
	MbufClear(MilImageCam, 0);
	MbufAllocColor(MilSystem, 3, 640, 480, 8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &MilImageCamRotate);
	MbufClear(MilImageCamRotate, 0);
	MbufAllocColor(MilSystem, 3, 640, 480, 8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &MilImageCamFlip);
	MbufClear(MilImageCamFlip, 0);
#endif

	for(int i=0; i<4; i++)
	{
		if(UserWindowHandle[i])
		{
			MilDisplay[i] = new CLibMilDisp(MilSystem);			    /* Display identifier.				*/
#ifdef USE_IRAYPLE
			MilImage[i] = new CLibMilBuf(MilSystem, 1, 720, 540, 8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC);				/* Image buffer identifier.			*/
			DisplaySelect(MilDisplay[i], MilImage[i], UserWindowHandle[i], 720, 540, DISPLAY_FIT_MODE_CENTERVIEW);
#else
			//MilImage[i] = new CLibMilBuf(MilSystem, 3, 640, 480, 8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC);				/* Image buffer identifier.			*/
			MilImage[i] = new CLibMilBuf(MilSystem, 1, 640, 480, 8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC);				/* Image buffer identifier.			*/
			DisplaySelect(MilDisplay[i], MilImage[i], UserWindowHandle[i], 640, 480, DISPLAY_FIT_MODE_CENTERVIEW);
#endif
			CreateOverlay(MilDisplay[i], M_COLOR_GREEN);
			MilDraw[i] = new CLibMilDraw(MilSystem, MilDisplay[i]);
		}
	}

// 	if(UserWindowHandle[0])
// 		MdispAlloc(MilSystem, M_DEFAULT, _T("M_DEFAULT"), M_DEFAULT, &MilDisplay[0]);
// 	if(UserWindowHandle[1])
// 		MdispAlloc(MilSystem, M_DEFAULT, _T("M_DEFAULT"), M_DEFAULT, &MilDisplay[1]);
// 	if(UserWindowHandle[2])
// 		MdispAlloc(MilSystem, M_DEFAULT, _T("M_DEFAULT"), M_DEFAULT, &MilDisplay[2]);
// 	if(UserWindowHandle[3])
// 		MdispAlloc(MilSystem, M_DEFAULT, _T("M_DEFAULT"), M_DEFAULT, &MilDisplay[3]);

//	MbufAlloc2d(MilSystem, 640, 480, 8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &MilImage);
//	MbufAllocColor(MilSystem, 3, 640, 480, 8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &MilImage);

// 	if(UserWindowHandle[0])
// 		MbufAllocColor(MilSystem, 3, 640, 480, 8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &MilImage[0]);
// 	if(UserWindowHandle[1])
// 		MbufAllocColor(MilSystem, 3, 640, 480, 8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &MilImage[1]);
// 	if(UserWindowHandle[2])
// 		MbufAllocColor(MilSystem, 3, 640, 480, 8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &MilImage[2]);
// 	if(UserWindowHandle[3])
// 		MbufAllocColor(MilSystem, 3, 640, 480, 8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &MilImage[3]);

	//if(m_nPixelMode[0]==8)
	//	MbufAllocColor(MilSystem, 1, 1280, 1024, 8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &MilImage[0]);
	//else
	//	MbufAllocColor(MilSystem, 3, 1280, 1024, 8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &MilImage[0]);

	//if(m_nPixelMode[1]==8)
	//	MbufAllocColor(MilSystem, 1, 1280, 1024, 8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &MilImage[1]);
	//else
	//	MbufAllocColor(MilSystem, 3, 1280, 1024, 8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &MilImage[1]);

	/* Clear the buffer. */
// 	if(UserWindowHandle[0])
// 		MbufClear(MilImage[0], 0);
// 	if(UserWindowHandle[1])
// 		MbufClear(MilImage[1], 0);
// 	if(UserWindowHandle[2])
// 		MbufClear(MilImage[2], 0);
// 	if(UserWindowHandle[3])
// 		MbufClear(MilImage[3], 0);

	/* Select the MIL buffer to be displayed in the user-specified window. */
// 	if(UserWindowHandle[0])
// 		MdispSelectWindow(MilDisplay[0], MilImage[0], UserWindowHandle[0]);
// 	if(UserWindowHandle[1])
// 		MdispSelectWindow(MilDisplay[1], MilImage[1], UserWindowHandle[1]);
// 	if(UserWindowHandle[2])
// 		MdispSelectWindow(MilDisplay[2], MilImage[2], UserWindowHandle[2]);
// 	if(UserWindowHandle[3])
// 		MdispSelectWindow(MilDisplay[3], MilImage[3], UserWindowHandle[3]);
	

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void CLibMil::BufPut2d(int nIdx, long nSzX, long nSzY, TCHAR* pSrc)
{
	m_cs.Lock();
	MbufPut2d(MilImage[nIdx]->m_MilImage, 0, 0, nSzX, nSzY, pSrc);
	m_cs.Unlock();
}

void CLibMil::BufPutColor2d(int nIdx, int nBitsPerPixel, long nSzX, long nSzY, TCHAR* pSrc)
{
	m_cs.Lock();
	switch(nBitsPerPixel)
	{
	case 15:
		MbufPutColor2d(MilImage[nIdx]->m_MilImage, M_PACKED+M_BGR15, M_ALL_BANDS, 0, 0, nSzX, nSzY, pSrc);
		break;
	case 16:
		MbufPutColor2d(MilImage[nIdx]->m_MilImage, M_PACKED+M_BGR16, M_ALL_BANDS, 0, 0, nSzX, nSzY, pSrc);
		break;
	case 24:
		MbufPutColor2d(MilImage[nIdx]->m_MilImage, M_PACKED+M_BGR24, M_ALL_BANDS, 0, 0, nSzX, nSzY, pSrc);
		break;
	case 32:
		MbufPutColor2d(MilImage[nIdx]->m_MilImage, M_PACKED+M_BGR32, M_ALL_BANDS, 0, 0, nSzX, nSzY, pSrc);
		break;
	}
//	MbufSave(_T("C:\\test.TIF", MilImage));
	m_cs.Unlock();
}

void CLibMil::BufPutColor2d(int nIdx, long nSzX, long nSzY, TCHAR* pSrc)
{
	m_cs.Lock();
	MbufPutColor2d(MilImage[nIdx]->m_MilImage, M_PACKED+M_BGR24, M_ALL_BANDS, 0, 0, nSzX, nSzY, pSrc);
//	MbufPutColor2d(MilImage[nIdx], M_PACKED+M_BGR32, M_ALL_BANDS, 0, 0, nSzX, nSzY, pSrc);
//	MbufSave(_T("C:\\test.TIF", MilImage));
	m_cs.Unlock();
}

void CLibMil::BufPutColor2d0(long nSzX, long nSzY, TCHAR* pSrc)
{
#ifdef USE_IRAYPLE
	MbufPut2d(MilImageCam, 0, 0, nSzX, nSzY, pSrc);
#else
	MbufPutColor2d(MilImageCam, M_PACKED + M_BGR24, M_ALL_BANDS, 0, 0, nSzX, nSzY, pSrc);
#endif
	//MimRotate(MilImageCam, MilImageCamRotate, 180.0, M_DEFAULT, M_DEFAULT, M_DEFAULT, M_DEFAULT, M_DEFAULT);
	//MimFlip(MilImageCamRotate, MilImageCamFlip, M_FLIP_HORIZONTAL, M_DEFAULT);
	//MimFlip(MilImageCamRotate, MilImageCamFlip, M_FLIP_VERTICAL, M_DEFAULT);
	//MbufCopyColor(MilImageCamFlip, MilImage[0]->m_MilImage, M_RED);

	MbufCopyColor(MilImageCam, MilImage[0]->m_MilImage, M_RED);

//	MbufSave(_T("C:\\test.TIF", MilImage));
}

void CLibMil::BufPutColor2d1(long nSzX, long nSzY, TCHAR* pSrc)
{
#ifdef USE_IRAYPLE
	MbufPut2d(MilImageCam, 0, 0, nSzX, nSzY, pSrc);
#else
	MbufPutColor2d(MilImageCam, M_PACKED+M_BGR24, M_ALL_BANDS, 0, 0, nSzX, nSzY, pSrc);
#endif
	//MimRotate(MilImageCam, MilImageCamRotate, 180.0, M_DEFAULT, M_DEFAULT, M_DEFAULT, M_DEFAULT, M_DEFAULT);
	//MimFlip(MilImageCamRotate, MilImageCamFlip, M_FLIP_HORIZONTAL, M_DEFAULT);
	//MbufCopyColor(MilImageCamFlip, MilImage[0]->m_MilImage, M_RED);

	MbufCopyColor(MilImageCam, MilImage[0]->m_MilImage, M_RED);

//	MbufSave(_T("C:\\test.TIF", MilImage));
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void CLibMil::DisplaySelect(CLibMilDisp* MLibDisp, CLibMilBuf* MLibImage, HWND ClientWindowHandle, int nDisplaySizeX, int nDisplaySizeY, int nDisplayFitMode)
{
	DisplaySelect(MLibDisp->m_MilDisplay, MLibImage->m_MilImage, ClientWindowHandle, nDisplaySizeX, nDisplaySizeY, nDisplayFitMode);
}

void CLibMil::DisplaySelect(MIL_ID DisplayId, MIL_ID ImageId, HWND ClientWindowHandle, int nDisplaySizeX, int nDisplaySizeY, int nDisplayFitMode)
{
	int nBufferSizeX=0, nBufferSizeY=0;
	nBufferSizeX = MbufInquire(ImageId, M_SIZE_X, M_NULL);	//MbufInquire(ImageId, M_SIZE_X, &nBufferSizeX);
	nBufferSizeY = MbufInquire(ImageId, M_SIZE_Y, M_NULL);	//MbufInquire(ImageId, M_SIZE_Y, &nBufferSizeY);


	m_nDisplaySizeX = nDisplaySizeX;
	m_nDisplaySizeY = nDisplaySizeY;

 	CalcFitResizeRatio(nBufferSizeX, nBufferSizeY, nDisplaySizeX, nDisplaySizeY, nDisplayFitMode);
 	DisplayZoom(m_dFitResizeRatioX, m_dFitResizeRatioY);
 	DisplayPan(m_nCenterViewOffsetX, m_nCenterViewOffsetY);

	//MIL_ID MilRtn;
	//MIL_INT nInt = MdispInquire(DisplayId, M_EXTENDED_INIT_FLAG, &MilRtn);
	MdispSelectWindow(DisplayId, ImageId, ClientWindowHandle);
}

void CLibMil::DisplayZoom(double dValueX, double dValueY)
{
	if(UserWindowHandle[0])
		MdispZoom(MilDisplay[0]->m_MilDisplay, dValueX, dValueY);
}

void CLibMil::DisplayPan(double dValueX, double dValueY)
{
	if(UserWindowHandle[0])
		MdispPan(MilDisplay[0]->m_MilDisplay, dValueX, dValueY);
}

void CLibMil::CalcFitResizeRatio(int nCameraImageSizeX, int nCameraImageSizeY, int nDisplayRectSizeX, int nDisplayRectSizeY, int nDisplayFitMode)
{
	m_nDisplayFitMode = nDisplayFitMode;
	m_dFitResizeRatioX = (float)nDisplayRectSizeX / (float)nCameraImageSizeX;
	m_dFitResizeRatioY = (float)nDisplayRectSizeY / (float)nCameraImageSizeY;

	switch(nDisplayFitMode)
	{
	case DISPLAY_FIT_MODE_CENTERVIEW:
		m_dFitResizeRatio = 1.0;
		break;
	case DISPLAY_FIT_MODE_MIN_RATIO:
		m_dFitResizeRatio = min(m_dFitResizeRatioX, m_dFitResizeRatioY);
		break;
	case DISPLAY_FIT_MODE_MAX_RATIO:
		m_dFitResizeRatio = max(m_dFitResizeRatioX, m_dFitResizeRatioY);
		break;
	case DISPLAY_FIT_MODE_X_RATIO:
		m_dFitResizeRatio = m_dFitResizeRatioX;
		break;
	case DISPLAY_FIT_MODE_Y_RATIO:
		m_dFitResizeRatio = m_dFitResizeRatioY;
		break;
	case DISPLAY_FIT_MODE_XY_RATIO:
		break;
	}

	if(nDisplayFitMode != DISPLAY_FIT_MODE_XY_RATIO)
	{
		m_dFitResizeRatioX = m_dFitResizeRatio;
		m_dFitResizeRatioY = m_dFitResizeRatio;
		
		m_nCenterViewOffsetX = (int)(((m_dFitResizeRatioX*(float)nCameraImageSizeX) - nDisplayRectSizeX)/2 / m_dFitResizeRatioX);
		m_nCenterViewOffsetY = (int)(((m_dFitResizeRatioY*(float)nCameraImageSizeY) - nDisplayRectSizeY)/2 / m_dFitResizeRatioY);
	}
	else
	{
		m_nCenterViewOffsetX = 0;
		m_nCenterViewOffsetY = 0;
	}
}

MIL_ID CLibMil::GetSystemID()
{
	return MilSystem;
}

CLibMilBuf* CLibMil::AllocBuf(int SizeBand, long SizeX, long SizeY, long Type, BUFATTRTYPE Attribute)	// Color Buffer
{
	CLibMilBuf* pBuf = new CLibMilBuf(MilSystem, SizeBand, SizeX, SizeY, Type, Attribute);
	return pBuf;
}

CLibMilBuf* CLibMil::AllocBuf(long SizeX, long SizeY, long Type, BUFATTRTYPE Attribute)	// Color Buffer
{
	CLibMilBuf* pBuf = new CLibMilBuf(MilSystem, SizeX, SizeY, Type, Attribute);
	return pBuf;
}

CLibMilDisp* CLibMil::AllocDisp()
{
	CLibMilDisp* pDisp = new CLibMilDisp(MilSystem);
	return pDisp;
}

CLibMilDraw* CLibMil::AllocDraw(CLibMilDisp* MLibDisp)
{
	CLibMilDraw* pDraw = new CLibMilDraw(MilSystem, MLibDisp);
	return pDraw;
}

void CLibMil::CreateOverlay(CLibMilDisp* MLibDisp, long nColor)
{
	MIL_ID MDisplay = MLibDisp->m_MilDisplay;
//	MIL_ID MOverlay = MLibDisp->m_MilDisplayOverlay;
	MdispControl(MDisplay, M_OVERLAY, M_ENABLE);
 	
// 	MdispInquire(MDisplay, M_OVERLAY_ID, &MOverlay);
 	MdispInquire(MDisplay, M_OVERLAY_ID, &MLibDisp->m_MilDisplayOverlay);
	// Clear the overlay to transparent.
    MdispControl(MDisplay, M_OVERLAY_CLEAR, M_DEFAULT);
	MdispControl(MDisplay, M_WINDOW_OVR_WRITE, M_ENABLE);
	nColor = MdispInquire(MDisplay, M_TRANSPARENT_COLOR, M_NULL); //MdispInquire(MDisplay, M_TRANSPARENT_COLOR, &nColor); 
//	MbufClear(MOverlay, (double)nColor); 
	MbufClear(MLibDisp->m_MilDisplayOverlay, (double)nColor); 

//	MLibDisp->m_MilDisplayOverlay = MOverlay;
	MLibDisp->m_lOverlayColor = nColor;
}

void CLibMil::CreateOverlay()
{
	CreateOverlay(MilDisplay[0], M_COLOR_GREEN);
// 	MdispControl(MilDisplay[0], M_OVERLAY, M_ENABLE);
//  	
//  	MdispInquire(MilDisplay[0], M_OVERLAY_ID, &MilDisplay[0]->m_MilDisplayOverlay);
// 	// Clear the overlay to transparent.
//     MdispControl(MilDisplay[0], M_OVERLAY_CLEAR, M_DEFAULT);
// 	MdispControl(MilDisplay[0], M_WINDOW_OVR_WRITE, M_ENABLE);
// 	MdispInquire(MilDisplay[0], M_TRANSPARENT_COLOR, &m_lOverlayColor[0]); 
// 	MbufClear(MilDisplayOverlay[0], &m_lOverlayColor[0]); 
}

BOOL CLibMil::ClearLiveOverlay()
{
#ifdef USE_MIL
	MilDisplay[0]->ClearOverlay();
#endif
// 	if(MilDisplayOverlay[0])
// 	{
// 		MbufClear(MilDisplayOverlay[0], &m_lOverlayColor[0]);
// 		return TRUE;
// 	}
// 	else
		return FALSE;
}

BOOL CLibMil::ClearCenterMarkArea(long XStart, long YStart, long XEnd, long YEnd)
{
	if (!MilDraw[0])
		return FALSE;
	BOOL bRtn = MilDraw[0]->DrawRectFill(MilDisplay[0]->m_lOverlayColor, XStart, YStart, XEnd, YEnd);
	return bRtn;
}

BOOL CLibMil::DrawCross(double ForegroundColor, long XCenter, long YCenter, long XSize, long YSize, long XCenterSpace, long YCenterSpace)
{
	BOOL bRtn = TRUE;
#ifdef USE_MIL
	if (MilDraw[0])
		bRtn = MilDraw[0]->DrawCross(ForegroundColor, XCenter, YCenter, XSize, YSize, XCenterSpace, YCenterSpace);
#endif
	return bRtn;
}

void CLibMil::SetTextFontScale(double dFontScaleX, double dFontScaleY)
{
	if (MilDraw[0])
		MilDraw[0]->SetTextFontScale(dFontScaleX, dFontScaleY);	
}

void CLibMil::SetTextFont(long FontName)
{
	m_lFontName = FontName;
	if (MilDraw[0])
		MilDraw[0]->SetTextFont(FontName);	
}

void CLibMil::DrawText(CString str, long lX, long lY, long lColor)
{
	//char szText[MAX_PATH];
	//strcpy(szText, str);
	TCHAR szText[MAX_PATH];
	wsprintf(szText, TEXT("%s"), str);
	if (MilDraw[0])
		MilDraw[0]->SetDrawColor(lColor);
	if (MilDraw[0])
		MilDraw[0]->SetDrawBackColor(MilDisplay[0]->m_lOverlayColor);
	if (MilDraw[0])
		MilDraw[0]->DrawText(lX, lY, szText);
}

void CLibMil::SetDrawColor(long lForegroundColor)
{
	if(MilDraw[0])
		MilDraw[0]->SetDrawColor(lForegroundColor);
}


BOOL CLibMil::OneshotGrab(MIL_ID MilDestImage, int nColor)// nColor -> 0: Color, 1: Red, 2: Green, 3:Blue
{
#ifdef _DEBUG
	//MbufSave(_T("C:\\OneshotGrab.TIF"), MilImage[0]->m_MilImage);
#endif

	switch(nColor)
	{
	case GRAB_COLOR_COLOR:
	 	MbufCopy(MilImage[0]->m_MilImage, MilDestImage);
		break;
	case GRAB_COLOR_RED:
		MbufCopyColor(MilImage[0]->m_MilImage, MilDestImage, M_RED);
		break;
	case GRAB_COLOR_GREEN:
		MbufCopyColor(MilImage[0]->m_MilImage, MilDestImage, M_GREEN);
		break;
	case GRAB_COLOR_BLUE:
		MbufCopyColor(MilImage[0]->m_MilImage, MilDestImage, M_BLUE);
		break;
	default:
		MbufCopyColor(MilImage[0]->m_MilImage, MilDestImage, M_RED);
		break;
	}

#ifdef _DEBUG
	//MbufSave(_T("C:\\OneshotGrab.TIF"), MilDestImage);
#endif
	return TRUE;
}

//=============================================================================================================================
// pattern matching

void CLibMil::PatternMatchingAlloc(MIL_ID MilImage, double dScore)
{
	int nSizeX, nSizeY;
	nSizeX = MbufInquire(MilImage, M_SIZE_X, M_NULL);	//MbufInquire(MilImage, M_SIZE_X, &nSizeX);
	nSizeY = MbufInquire(MilImage, M_SIZE_Y, M_NULL);	//MbufInquire(MilImage, M_SIZE_Y, &nSizeY);


	if(m_PatternMatchingModel)
	{	
		MpatFree(m_PatternMatchingModel);
		m_PatternMatchingModel = M_NULL;
	}
	MpatAllocModel(MilSystem, MilImage, 0, 0, nSizeX, nSizeY, M_NORMALIZED, &m_PatternMatchingModel);

	/* Set the search accuracy to high. */
	MpatSetAccuracy(m_PatternMatchingModel, M_MEDIUM);//M_HIGH

	/* Set the search model speed to high. */
	MpatSetSpeed(m_PatternMatchingModel, M_MEDIUM); // M_HIGH


	MpatSetAngle(m_PatternMatchingModel, M_SEARCH_ANGLE_MODE, M_ENABLE);
	MpatSetAngle(m_PatternMatchingModel, M_SEARCH_ANGLE_DELTA_NEG, 45);
	MpatSetAngle(m_PatternMatchingModel, M_SEARCH_ANGLE_DELTA_POS, 45);
	MpatSetAngle(m_PatternMatchingModel, M_SEARCH_ANGLE_ACCURACY, M_DEFAULT);
// 	MpatSetAngle(m_PatternMatchingModel, M_SEARCH_ANGLE_ACCURACY, 0.5);
	MpatSetAngle(m_PatternMatchingModel, M_SEARCH_ANGLE_INTERPOLATION_MODE, M_DEFAULT);
// 	MpatSetAngle(m_PatternMatchingModel, M_SEARCH_ANGLE_INTERPOLATION_MODE, M_BILINEAR);

	MpatSetPosition(m_PatternMatchingModel, 0, 0, M_ALL, M_ALL);
	MpatSetAcceptance(m_PatternMatchingModel, dScore);
// 	MpatSetAcceptance(m_PatternMatchingModel, 10.0);
// 	MpatSetAcceptance(m_PatternMatchingModel, 30.0);
	MpatSetSearchParameter(m_PatternMatchingModel, M_FIRST_LEVEL, M_DEFAULT);
	MpatSetSearchParameter(m_PatternMatchingModel, M_LAST_LEVEL, M_DEFAULT);

	/* Preprocess the model. */
	MpatPreprocModel(MilImage, m_PatternMatchingModel, M_DEFAULT);

	// Edge Effect : iteration = 2
	MimConvolve(MilImage, MilImage, M_SMOOTH);
	MimConvolve(MilImage, MilImage, M_SMOOTH);
	MimConvolve(MilImage, MilImage, M_SMOOTH);
	MimConvolve(MilImage, MilImage, M_SMOOTH);
	MimConvolve(MilImage, MilImage, M_SMOOTH);
	MimConvolve(MilImage, MilImage, M_SMOOTH);
	MimConvolve(MilImage, MilImage, M_SMOOTH);

#ifdef _DEBUG
// 	char szFileName[100];
// 	sprintf(szFileName, "C:\\PinPatSth.tif");
//  	MbufSave(szFileName, MilImage);
#endif

	if(m_PatternMatchingResult)
	{
		MpatFree(m_PatternMatchingResult);
		m_PatternMatchingResult = M_NULL;
	}
	MpatAllocResult(MilSystem, PATTERN_MATCHING_RESULT_NUM_LIMIT, &m_PatternMatchingResult);
}

BOOL CLibMil::PatternMatchingAction(MIL_ID MilImage, BOOL bDraw)//, 
{
	if (!MilDraw[0])
		return FALSE;

	BOOL bRtn = TRUE;
	MIL_ID MilPatternMatchingResultDisplayBuffer = MilDisplay[0]->m_MilDisplayOverlay;
	MIL_ID GraphContextId = MilDraw[0]->m_MilGraphicContextID;

	if((!m_PatternMatchingResult)||(!m_PatternMatchingModel))
	{
//		pView->MsgBox(_T("You need Pattern Matching Allocation !!"));
// 		AfxMessageBox(_T("You need Pattern Matching Allocation !!"));
		return FALSE;
	}

	/* First, perform a dummy find operation for better function timing accuracy 
	  (model cache effect,...) and reset the timer.	*/
	MpatFindModel(MilImage, m_PatternMatchingModel, m_PatternMatchingResult);
	MappTimer(M_TIMER_RESET+M_SYNCHRONOUS, M_NULL);

	/* Find the model in the target buffer. */
	MpatFindModel(MilImage, m_PatternMatchingModel, m_PatternMatchingResult);

	/* Read the time spent in MpatFindModel. */
	MappTimer(M_TIMER_READ+M_SYNCHRONOUS, &m_dPatternMatchingResultTime);

	m_nPatternMatchingResultNum = 0;
	m_nPatternMatchingResultNum = MpatGetNumber(m_PatternMatchingResult, M_NULL);

	m_dPatternMatchingResultSelectScore = 0.0;
	/* If one model was found above the acceptance threshold. */
	if((m_nPatternMatchingResultNum > 0)&&(m_nPatternMatchingResultNum <= PATTERN_MATCHING_RESULT_NUM_LIMIT))
	{
		PatternMatchingResultDataFree();
		PatternMatchingResultDataAlloc(m_nPatternMatchingResultNum);

		/* Read results and draw a box around the model occurrence. */
		MpatGetResult(m_PatternMatchingResult, M_POSITION_X, m_dPatternMatchingResultPosX);
		MpatGetResult(m_PatternMatchingResult, M_POSITION_Y, m_dPatternMatchingResultPosY);
		MpatGetResult(m_PatternMatchingResult, M_ANGLE, m_dPatternMatchingResultAngle);
		MpatGetResult(m_PatternMatchingResult, M_SCORE, m_dPatternMatchingResultScore);

		double dBestScore = 0;
		for(int i=0; i<m_nPatternMatchingResultNum; i++)
		{
			if(dBestScore < m_dPatternMatchingResultScore[i])
			{
				dBestScore = m_dPatternMatchingResultScore[i];
				m_nPatternMatchingResultSelectNum = i;
			}
		}

		m_dPatternMatchingResultSelectPosX = m_dPatternMatchingResultPosX[m_nPatternMatchingResultSelectNum];
		m_dPatternMatchingResultSelectPosY = m_dPatternMatchingResultPosY[m_nPatternMatchingResultSelectNum];
		m_dPatternMatchingResultSelectAngle = m_dPatternMatchingResultAngle[m_nPatternMatchingResultSelectNum];
		m_dPatternMatchingResultSelectScore = m_dPatternMatchingResultScore[m_nPatternMatchingResultSelectNum];
	}
	else
	{
		bRtn = FALSE;
	}

	if(MilPatternMatchingResultDisplayBuffer)
	{
		if(GraphContextId == NULL)
			GraphContextId = M_DEFAULT;

		double lCurColor;
		MgraInquire(GraphContextId, M_COLOR, &lCurColor);
		MgraColor(GraphContextId, M_COLOR_RED);

		if(bDraw && bRtn)
		{
			MpatDraw(GraphContextId, m_PatternMatchingResult, 
							MilPatternMatchingResultDisplayBuffer, 
							M_DRAW_BOX+M_DRAW_POSITION,	  
							M_DEFAULT, M_DEFAULT);
			Sleep(30);
		}
// 		else
// 		{
// 			char szText[100];
//			sprintf(szText, _T("Pattern Matching Fail !"));
// 
// 			long lImageSizeX, lImageSizeY;
// 			MbufInquire(MilImage,M_SIZE_X,&lImageSizeX);
// 			MbufInquire(MilImage,M_SIZE_Y,&lImageSizeY);
// 
// 			MgraText(GraphContextId, MilPatternMatchingResultDisplayBuffer, lImageSizeX/2, lImageSizeY/2, szText);
// 		}
		MgraColor(GraphContextId, lCurColor);
	}
	
#ifdef _DEBUG
	TRACE(_T(" pat Pos X:%.2f, Y:%.2f, Score:%.2f, time:%.6f \n"),	m_dPatternMatchingResultSelectPosX, 
																	m_dPatternMatchingResultSelectPosY,
																	m_dPatternMatchingResultSelectAngle,
																	m_dPatternMatchingResultSelectScore);
#endif
	
	return bRtn;
}

void CLibMil::PatternMatchingFree()
{
	m_nPatternMatchingResultNum = 0;
	PatternMatchingResultDataFree();

	if(m_PatternMatchingResult)
	{
		MpatFree(m_PatternMatchingResult);
		m_PatternMatchingResult = M_NULL;
	}
	if(m_PatternMatchingModel)
	{	
		MpatFree(m_PatternMatchingModel);
		m_PatternMatchingModel = M_NULL;
	}
}

void CLibMil::PatternMatchingResultDataAlloc(int nObjectNum)
{
	m_dPatternMatchingResultPosX = (double*)malloc(sizeof(double) * nObjectNum);
	m_dPatternMatchingResultPosY = (double*)malloc(sizeof(double) * nObjectNum);
	m_dPatternMatchingResultAngle = (double*)malloc(sizeof(double) * nObjectNum);
	m_dPatternMatchingResultScore = (double*)malloc(sizeof(double) * nObjectNum);

	for(int i=0; i<nObjectNum; i++)
	{
		m_dPatternMatchingResultPosX[i] = 0.0;
		m_dPatternMatchingResultPosY[i] = 0.0;
		m_dPatternMatchingResultAngle[i] = 0.0;
		m_dPatternMatchingResultScore[i] = 0.0;
	}
}

void CLibMil::PatternMatchingResultDataFree()
{
	if(m_dPatternMatchingResultPosX != NULL)
	{
		free(m_dPatternMatchingResultPosX);
		m_dPatternMatchingResultPosX = NULL;
	}
	if(m_dPatternMatchingResultPosY != NULL)
	{
		free(m_dPatternMatchingResultPosY);
		m_dPatternMatchingResultPosY = NULL;
	}
	if(m_dPatternMatchingResultAngle != NULL)
	{
		free(m_dPatternMatchingResultAngle);
		m_dPatternMatchingResultAngle = NULL;
	}
	if(m_dPatternMatchingResultScore != NULL)
	{
		free(m_dPatternMatchingResultScore);
		m_dPatternMatchingResultScore = NULL;
	}
}


// Morphology
void CLibMil::Erode(MIL_ID SrcImageId, MIL_ID DestImageId, MIL_INT NbIteration, MIL_INT64 ProcMode)
{
	MimErode(SrcImageId, DestImageId, NbIteration, ProcMode);
}

void CLibMil::Dilate(MIL_ID SrcImageId, MIL_ID DestImageId, MIL_INT NbIteration, MIL_INT64 ProcMode)
{
	MimDilate(SrcImageId, DestImageId, NbIteration, ProcMode);
}

void CLibMil::Open(MIL_ID SrcImageId, MIL_ID DestImageId, MIL_INT NbIteration, MIL_INT64 ProcMode)
{
	MimOpen(SrcImageId, DestImageId, NbIteration, ProcMode);
}

void CLibMil::Close(MIL_ID SrcImageId, MIL_ID DestImageId, MIL_INT NbIteration, MIL_INT64 ProcMode)
{
	MimClose(SrcImageId, DestImageId, NbIteration, ProcMode);
}

void CLibMil::Filter(MIL_ID SrcImageId, MIL_ID DestImageId, MIL_ID KernelId, MIL_INT Rank, MIL_INT64 Mode)	// rank filter on the pixels in an image
{
	MimRank(SrcImageId, DestImageId, KernelId, Rank, Mode);
}

void CLibMil::Convolution(MIL_ID SrcImageId, MIL_ID DestImageId, MIL_ID KernelId)						// general convolution operation
{
	MimConvolve(SrcImageId, DestImageId, KernelId);
}

// Binarize
void CLibMil::Binarize(MIL_ID SrcImageId, MIL_ID DestImageId, MIL_INT64 Condition, MIL_DOUBLE CondLow, MIL_DOUBLE CondHigh)	// Binarize
{
	MimBinarize(SrcImageId, DestImageId, Condition, CondLow, CondHigh);
}

long CLibMil::Binarize(MIL_ID SrcImageId)	// Binarize
{
	return MimBinarize(SrcImageId, M_NULL,M_DEFAULT, M_DEFAULT, M_DEFAULT);
}


//=============================================================================================================================

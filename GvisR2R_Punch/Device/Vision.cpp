// Vision.cpp : implementation file
//

#include "stdafx.h"
#include "../gvisr2r_punch.h"
#include "Vision.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef USE_VISION

#include "../GvisR2R_PunchDoc.h"
#include "../GvisR2R_PunchView.h"

extern CGvisR2R_PunchDoc* pDoc;
extern CGvisR2R_PunchView* pView;

unsigned char BitConvLUT2[256] = {0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0, 0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0, 0x8, 0x88, 0x48, 0xc8, 0x28, 0xa8, 0x68, 0xe8, 0x18, 0x98, 0x58, 0xd8, 0x38, 0xb8, 0x78, 0xf8, 0x4, 0x84, 0x44, 0xc4, 0x24, 0xa4, 0x64, 0xe4, 0x14, 0x94, 0x54, 0xd4, 0x34, 0xb4, 0x74, 0xf4, 0xc, 0x8c, 0x4c, 0xcc, 0x2c, 0xac, 0x6c, 0xec, 0x1c, 0x9c, 0x5c, 0xdc, 0x3c, 0xbc, 0x7c, 0xfc, 0x2, 0x82, 0x42, 0xc2, 0x22, 0xa2, 0x62, 0xe2, 0x12, 0x92, 0x52, 0xd2, 0x32, 0xb2, 0x72, 0xf2, 0xa, 0x8a, 0x4a, 0xca, 0x2a, 0xaa, 0x6a, 0xea, 0x1a, 0x9a, 0x5a, 0xda, 0x3a, 0xba, 0x7a, 0xfa, 0x6, 0x86, 0x46, 0xc6, 0x26, 0xa6, 0x66, 0xe6, 0x16, 0x96, 0x56, 0xd6, 0x36, 0xb6, 0x76, 0xf6, 0xe, 0x8e, 0x4e, 0xce, 0x2e, 0xae, 0x6e, 0xee, 0x1e, 0x9e, 0x5e, 0xde, 0x3e, 0xbe, 0x7e, 0xfe, 0x1, 0x81, 0x41, 0xc1, 0x21, 0xa1, 0x61, 0xe1, 0x11, 0x91, 0x51, 0xd1, 0x31, 0xb1, 0x71, 0xf1, 0x9, 0x89, 0x49, 0xc9, 0x29, 0xa9, 0x69, 0xe9, 0x19, 0x99, 0x59, 0xd9, 0x39, 0xb9, 0x79, 0xf9, 0x5, 0x85, 0x45, 0xc5, 0x25, 0xa5, 0x65, 0xe5, 0x15, 0x95, 0x55, 0xd5, 0x35, 0xb5, 0x75, 0xf5, 0xd, 0x8d, 0x4d, 0xcd, 0x2d, 0xad, 0x6d, 0xed, 0x1d, 0x9d, 0x5d, 0xdd, 0x3d, 0xbd, 0x7d, 0xfd, 0x3, 0x83, 0x43, 0xc3, 0x23, 0xa3, 0x63, 0xe3, 0x13, 0x93, 0x53, 0xd3, 0x33, 0xb3, 0x73, 0xf3, 0xb, 0x8b, 0x4b, 0xcb, 0x2b, 0xab, 0x6b, 0xeb, 0x1b, 0x9b, 0x5b, 0xdb, 0x3b, 0xbb, 0x7b, 0xfb, 0x7, 0x87, 0x47, 0xc7, 0x27, 0xa7, 0x67, 0xe7, 0x17, 0x97, 0x57, 0xd7, 0x37, 0xb7, 0x77, 0xf7, 0xf, 0x8f, 0x4f, 0xcf, 0x2f, 0xaf, 0x6f, 0xef, 0x1f, 0x9f, 0x5f, 0xdf, 0x3f, 0xbf, 0x7f, 0xff};

/////////////////////////////////////////////////////////////////////////////
// CVision

CVision::CVision(int nIdx, MIL_ID MilSysId, HWND *hCtrl, CWnd* pParent /*=NULL*/) // hCtrl : Max(4)
{
	m_nTest = 0;
	m_pParent = pParent;
	m_nIdx = nIdx;
	m_MilSysId = MilSysId;

	// init camera
	m_hCtrl[0] = hCtrl[0];
	m_hCtrl[1] = hCtrl[1];
	m_hCtrl[2] = hCtrl[2];
	m_hCtrl[3] = hCtrl[3];

#ifdef USE_IRAYPLE
	m_pIRayple = NULL;
#endif

#ifdef USE_IDS
	m_pIds[0] = NULL;
	m_pIds[1] = NULL;
#endif

#ifdef USE_CREVIS
	m_pCrevis[0] = NULL;
	m_pCrevis[1] = NULL;
#endif

	m_nTrig[0] = 0;
	m_nTrig[1] = 0;

	// init Mil
	m_pMil = NULL;

	// for Def View........
	for(int i=0; i<DEF_VIEW_IMG_NUMBER; i++)
	{
		m_pMilDispCad[i] = NULL;
		m_pMilBufCad[i] = NULL;
		m_pMilOvrCad[i] = NULL;
		m_pMilDelOvrCad[i] = NULL;

		m_pMilDispDef[i] = NULL;
		m_pMilBufDef[i] = NULL;
		m_pMilOvrDef[i] = NULL;
		m_pMilDelOvrDef[i] = NULL;
	}

	m_pMilDispPin = NULL;
	m_pMilBufPin = NULL;
// 	m_pMilDispAlign = NULL;
	m_pMilBufAlign = NULL;
	m_pMilPinOverlay = NULL;
	m_pMilPinOverlayDelete = NULL;	
// 	m_pMilDispPcs = NULL;
// 	m_pMilBufPcs = NULL;

	MilCADImgBuf = M_NULL;
	MilBufCADTemp = M_NULL;
	MilBufPinTemp = M_NULL;
	MilPinImgBuf = M_NULL;
	MilBufAlignTemp[0] = M_NULL;
	MilAlignImgBuf[0] = M_NULL;
	MilBufAlignTemp[1] = M_NULL;
	MilAlignImgBuf[1] = M_NULL;
	MilBufAlignTemp[2] = M_NULL;
	MilAlignImgBuf[2] = M_NULL;
	MilBufAlignTemp[3] = M_NULL;
	MilAlignImgBuf[3] = M_NULL;

	m_nCenterMarkLineLength = 100;
	m_nDisplayCenterX = 320;
	m_nDisplayCenterY = 240;
	m_nCenterMarkSpaceLength = 30;

	m_nPinCtrX = 0;
	m_nPinCtrY = 0;
	m_nPinCrsLen = 0;


	// init motion
	for(int nAxis=0; nAxis<MAX_AXIS; nAxis++)
		m_dEnc[nAxis] = 0.0;
	m_dBufEnc = 0.0;
	m_dFdEnc = 0.0;


	RECT rt={0,0,0,0};
//	CreateEx(NULL, _T("None"), _T("None"), WS_POPUP, rt, pParent, (UINT)this);
 	Create(NULL, _T("None"), WS_CHILD, rt, pParent, (UINT)this);
}

CVision::~CVision()
{
#ifdef USE_IRAYPLE
	StopLive();
	Sleep(100);

	if (m_pIRayple)
	{
		delete m_pIRayple;
		m_pIRayple = NULL;
		Sleep(100);
	}
#endif

#ifdef USE_IDS
	if (m_pIds[0])
	{
		delete m_pIds[0];
		m_pIds[0] = NULL;
	}
	if (m_pIds[1])
	{
		delete m_pIds[1];
		m_pIds[1] = NULL;
	}
#endif

#ifdef USE_CREVIS
	StopLive();
	Sleep(100);

	if(m_pCrevis[0])
	{
		delete m_pCrevis[0];
		m_pCrevis[0] = NULL;
	}
	Sleep(100);

	if(m_pCrevis[1])
	{
		delete m_pCrevis[1];
		m_pCrevis[1] = NULL;
	}
	Sleep(100);
#endif


	// Temperary Buf for Pin View
	if(MilBufPinTemp != M_NULL)
	{
		pView->m_nDebugStep = 200; pView->DispThreadTick();
		MbufFree(MilBufPinTemp);
		MilBufPinTemp = M_NULL;
	}

	if(MilPinImgBuf != M_NULL)
	{
		pView->m_nDebugStep = 201; pView->DispThreadTick();
		MbufFree(MilPinImgBuf);
		MilPinImgBuf = M_NULL;
	}

	if(m_pMilBufPin)
	{
		pView->m_nDebugStep = 202; pView->DispThreadTick();
		delete m_pMilBufPin;
		m_pMilBufPin = NULL;
	}

	if(m_pMilDispPin)
	{
		pView->m_nDebugStep = 203; pView->DispThreadTick();
		delete m_pMilDispPin;
		m_pMilDispPin = NULL;
	}

	// Temperary Buf for Align View
	if(MilBufAlignTemp[0] != M_NULL)
	{
		pView->m_nDebugStep = 204; pView->DispThreadTick();
		MbufFree(MilBufAlignTemp[0]);
		MilBufAlignTemp[0] = M_NULL;
	}
	if(MilBufAlignTemp[1] != M_NULL)
	{
		pView->m_nDebugStep = 205; pView->DispThreadTick();
		MbufFree(MilBufAlignTemp[1]);
		MilBufAlignTemp[1] = M_NULL;
	}
	if (MilBufAlignTemp[2] != M_NULL)
	{
		pView->m_nDebugStep = 206; pView->DispThreadTick();
		MbufFree(MilBufAlignTemp[2]);
		MilBufAlignTemp[2] = M_NULL;
	}
	if (MilBufAlignTemp[3] != M_NULL)
	{
		pView->m_nDebugStep = 207; pView->DispThreadTick();
		MbufFree(MilBufAlignTemp[3]);
		MilBufAlignTemp[3] = M_NULL;
	}

	if(MilAlignImgBuf[0] != M_NULL)
	{
		pView->m_nDebugStep = 208; pView->DispThreadTick();
		MbufFree(MilAlignImgBuf[0]);
		MilAlignImgBuf[0] = M_NULL;
	}
	if(MilAlignImgBuf[1] != M_NULL)
	{
		pView->m_nDebugStep = 209; pView->DispThreadTick();
		MbufFree(MilAlignImgBuf[1]);
		MilAlignImgBuf[1] = M_NULL;
	}
	if (MilAlignImgBuf[2] != M_NULL)
	{
		pView->m_nDebugStep = 210; pView->DispThreadTick();
		MbufFree(MilAlignImgBuf[2]);
		MilAlignImgBuf[2] = M_NULL;
	}
	if (MilAlignImgBuf[3] != M_NULL)
	{
		pView->m_nDebugStep = 211; pView->DispThreadTick();
		MbufFree(MilAlignImgBuf[3]);
		MilAlignImgBuf[3] = M_NULL;
	}

	if(m_pMilBufAlign)
	{
		delete m_pMilBufAlign;
		m_pMilBufAlign = NULL;
	}

	// Temperary Buf for Pcs View
// 	if(MilBufPcsTemp != M_NULL)
// 	{
// 		MbufFree(MilBufPcsTemp);
// 		MilBufPcsTemp = M_NULL;
// 	}
// 
// 	if(MilPcsImgBuf != M_NULL)
// 	{
// 		MbufFree(MilPcsImgBuf);
// 		MilPcsImgBuf = M_NULL;
// 	}

// 	if(m_pMilBufPcs)
// 	{
// 		delete m_pMilBufPcs;
// 		m_pMilBufPcs = NULL;
// 	}
// 
// 	if(m_pMilDispPcs)
// 	{
// 		delete m_pMilDispPcs;
// 		m_pMilDispPcs = NULL;
// 	}

	// Temperary Buf for Def View
	if(MilBufCADTemp != M_NULL)
	{
		pView->m_nDebugStep = 212; pView->DispThreadTick();
		MbufFree(MilBufCADTemp);
		MilBufCADTemp = M_NULL;
	}

	if(MilCADImgBuf != M_NULL)
	{
		pView->m_nDebugStep = 213; pView->DispThreadTick();
		MbufFree(MilCADImgBuf);
		MilCADImgBuf = M_NULL;
	}

	// for Def View.....
	for(int i=0; i<DEF_VIEW_IMG_NUMBER; i++)
	{
		// for CAD...
		if(m_pMilOvrCad[i])
		{
			delete m_pMilOvrCad[i];
			m_pMilOvrCad[i] = NULL;
		}
		if(m_pMilDelOvrCad[i])
		{
			delete m_pMilDelOvrCad[i];
			m_pMilDelOvrCad[i] = NULL;
		}
		if(m_pMilBufCad[i])
		{
			delete m_pMilBufCad[i];
			m_pMilBufCad[i] = NULL;
		}
		if(m_pMilDispCad[i])
		{
			delete m_pMilDispCad[i];
			m_pMilDispCad[i] = NULL;
		}

		// for Def....
		if(m_pMilOvrDef[i])
		{
			delete m_pMilOvrDef[i];
			m_pMilOvrDef[i] = NULL;
		}
		if(m_pMilDelOvrDef[i])
		{
			delete m_pMilDelOvrDef[i];
			m_pMilDelOvrDef[i] = NULL;
		}
		if(m_pMilBufDef[i])
		{
			delete m_pMilBufDef[i];
			m_pMilBufDef[i] = NULL;
		}
		if(m_pMilDispDef[i])
		{
			delete m_pMilDispDef[i];
			m_pMilDispDef[i] = NULL;
		}
	}

	
	if(m_pMilPinOverlay)
	{
		delete m_pMilPinOverlay;
		m_pMilPinOverlay = NULL;
	}
	if(m_pMilPinOverlayDelete)
	{
		delete m_pMilPinOverlayDelete;
		m_pMilPinOverlayDelete = NULL;
	}

// 	if(m_pMilDrawOverlay)
// 	{
// 		delete m_pMilDrawOverlay;
// 		m_pMilDrawOverlay = NULL;
// 	}
// 	if(m_pMilDrawOverlayDelete)
// 	{
// 		delete m_pMilDrawOverlayDelete;
// 		m_pMilDrawOverlayDelete = NULL;
// 	}
//  	if(m_pMilAlignImageBuffer)
//  	{
//  		delete m_pMilAlignImageBuffer;
//  		m_pMilAlignImageBuffer = NULL;
//  	}
// 	if(m_pMilDisplayBuffer)
// 	{
// 		delete m_pMilDisplayBuffer;
// 		m_pMilDisplayBuffer = NULL;
// 	}
// 	if(m_pMilDisplay)
// 	{
// 		delete m_pMilDisplay;
// 		m_pMilDisplay = NULL;
// 	}

	if(m_pMil)
	{
		delete m_pMil;
		m_pMil = NULL;
	}
}


BEGIN_MESSAGE_MAP(CVision, CWnd)
	//{{AFX_MSG_MAP(CVision)
	ON_WM_CREATE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
#ifdef USE_IDS
	ON_MESSAGE(IS_UEYE_MESSAGE0, OnUEyeMessage0)
	ON_MESSAGE(IS_UEYE_MESSAGE1, OnUEyeMessage1)
#endif
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CVision message handlers

int CVision::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
//	SetTimer(0, 100, NULL);
// 	m_hCtrl[0] = m_pParent->GetDlgItem( IDC_DISPLAY1 )->m_hWnd;
// 	m_hCtrl[1] = m_pParent->GetDlgItem( IDC_DISPLAY2 )->m_hWnd;

	HWND handle = this->GetSafeHwnd();

#ifdef USE_IDS
	m_pIds[m_nIdx] = new CCamIds(m_nIdx, m_hCtrl[0], this);

	HWND hCtrl[4] = {0};
	int nPixelMode[4] = {0};
	hCtrl[0] = m_hCtrl[0];
	hCtrl[1] = m_hCtrl[1];
	hCtrl[2] = m_hCtrl[2];
	hCtrl[3] = m_hCtrl[3];
	nPixelMode[0] = m_pIds[0]->GetBitsPerPixel();
	nPixelMode[1] = m_pIds[1]->GetBitsPerPixel();
	m_pMil = new CLibMil(m_nIdx, nPixelMode, hCtrl, this);
#endif

#ifdef USE_CREVIS
	m_pCrevis[0] = new CCamCrevis(m_nIdx, m_hCtrl[0], this);
	//m_pCrevis[1] = new CCamCrevis(1, m_hCtrl[1], this);

	HWND hCtrl[4] = {0};
	hCtrl[0] = m_hCtrl[0];
	m_pMil = new CLibMil(m_nIdx, m_MilSysId, hCtrl, this);

	Sleep(100);
	StartLive();
#endif

#ifdef USE_IRAYPLE
	m_pIRayple = new CCamIRayple(m_nIdx, m_hCtrl[0], this);

#ifdef USE_MIL
	HWND hCtrl[4] = { 0 };
	hCtrl[0] = m_hCtrl[0];
	m_pMil = new CLibMil(m_nIdx, m_MilSysId, hCtrl, this);
#endif

	Sleep(100);
#endif

	return 0;
}


MIL_ID CVision::GetSystemID()
{
	return m_MilSysId;
}

void CVision::OnTimer(UINT_PTR nIDEvent)//(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == TIM_CLR_OVERLAY)
	{
		KillTimer(TIM_CLR_OVERLAY);
		ClearOverlay();
		DrawCenterMark();
		DispAxisPos(TRUE);
	}
	
	CWnd::OnTimer(nIDEvent);
}

/////////////////////////////////////////////////////////////////////////////

CWnd* CVision::GetParent()
{
	return m_pParent;
}

LRESULT CVision::OnUEyeMessage0( WPARAM wParam, LPARAM lParam )
{
#ifdef USE_IDS
	CString sVal;
	//sVal.Format(_T("%d"), m_pIds[0]->GetSeqImgMemIdx());
	//((CGvisR2RView*)m_pParent)->SetText(IDC_STATIC_SEQ_IDX_0, sVal);
	sVal.Format(_T("%.1f"), m_pIds[0]->GetFramRate());
	//((CGvisR2RView*)m_pParent)->SetText(IDC_STATIC_FRAME_RATE_0, sVal);

	return 0;

	int nPixelMode;
	switch ( wParam )
	{
	case IS_DEVICE_REMOVED:
		break;
	case IS_DEVICE_RECONNECTED:
		break;
	case IS_FRAME:
		if(m_pMil)
		{
			m_nTrig[0]++;
			if(!(m_nTrig[0]%MAX_TRIG))
			{
				m_nTrig[0] = 0;
				nPixelMode = m_pIds[0]->GetBitsPerPixel();
				if(nPixelMode==8)
					m_pMil->BufPut2d(0, 1280, 1024, m_pIds[0]->GetImg());
				else
					m_pMil->BufPutColor2d0(1280, 1024, m_pIds[0]->GetImg());
					//m_pMil->BufPutColor2d(0, nPixelMode, 1280, 1024, m_pIds[0]->GetImg());
					//m_pMil->BufPutColor2d(0, 1280, 1024, m_pIds[0]->GetImg());
				sVal.Format(_T("%d"), m_pIds[0]->GetSeqImgMemIdx());
				//((CGvisR2RView*)m_pParent)->SetText(IDC_STATIC_SEQ_IDX_0, sVal);
				sVal.Format(_T("%.1f"), m_pIds[0]->GetFramRate());
				//((CGvisR2RView*)m_pParent)->SetText(IDC_STATIC_FRAME_RATE_0, sVal);
			}
		}
		break;
	}    
#endif

	return 0;
}

LRESULT CVision::OnUEyeMessage1( WPARAM wParam, LPARAM lParam )
{
#ifdef USE_IDS
	CString sVal;

	//sVal.Format(_T("%d"), m_pIds[1]->GetSeqImgMemIdx());
	//((CGvisR2RView*)m_pParent)->SetText(IDC_STATIC_SEQ_IDX_1, sVal);
	sVal.Format(_T("%.1f"), m_pIds[1]->GetFramRate());
	//((CGvisR2RView*)m_pParent)->SetText(IDC_STATIC_FRAME_RATE_1, sVal);
	return 0;

	int nPixelMode;
	switch ( wParam )
	{
	case IS_DEVICE_REMOVED:
		break;
	case IS_DEVICE_RECONNECTED:
		break;
	case IS_FRAME:
		sVal.Format(_T("%.1f"), m_pIds[1]->GetFramRate());
		//((CGvisR2RView*)m_pParent)->SetText(IDC_STATIC_FRAME_RATE_1, sVal);
		if(m_pMil)
		{
			m_nTrig[1]++;
			if(!(m_nTrig[1]%MAX_TRIG))
			{
				m_nTrig[1] = 0;
				nPixelMode = m_pIds[1]->GetBitsPerPixel();
				if(nPixelMode==8)
					m_pMil->BufPut2d(1, 1280, 1024, m_pIds[1]->GetImg());
				else
					m_pMil->BufPutColor2d1(1280, 1024, m_pIds[1]->GetImg());
					//m_pMil->BufPutColor2d(1, nPixelMode, 1280, 1024, m_pIds[1]->GetImg());
					//m_pMil->BufPutColor2d(1, 1280, 1024, m_pIds[1]->GetImg());
				sVal.Format(_T("%d"), m_pIds[1]->GetSeqImgMemIdx());
				//((CGvisR2RView*)m_pParent)->SetText(IDC_STATIC_SEQ_IDX_1, sVal);
				sVal.Format(_T("%.1f"), m_pIds[1]->GetFramRate());
				//((CGvisR2RView*)m_pParent)->SetText(IDC_STATIC_FRAME_RATE_1, sVal);
			}
		}
		break;
	}    
#endif

	return 0;
}

BOOL CVision::StartLive(int nIdx)
{
#ifdef USE_IRAYPLE
	if (!m_pIRayple)
		return FALSE;

	//m_pIRayple->Connect(TRUE);
	m_pIRayple->StartLive();
#endif

#ifdef USE_CREVIS
	if(nIdx > 1)
		return FALSE;
	if(m_pCrevis[nIdx])
		m_pCrevis[nIdx]->StartLive();
#endif
	return TRUE;
}

BOOL CVision::StopLive(int nIdx)
{
#ifdef USE_IRAYPLE
	if (nIdx > 1)
		return FALSE;
	if (m_pIRayple)
		m_pIRayple->Connect(FALSE);
#endif

#ifdef USE_CREVIS
	if(nIdx > 1)
		return FALSE;
	if(m_pCrevis[nIdx])
		m_pCrevis[nIdx]->StopLive();
#endif
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////
void CVision::DisplaySelect(int nKind, HWND hDispCtrl, CRect rtDispCtrl, int nIdx) // nKind : CAD_image[0], Defect_image[1]
{
	if(!m_pMil)
		return;

	if(nKind == CAD_IMG)
	{
		if(m_pMilDispCad[nIdx])
			m_pMil->DisplaySelect(m_pMilDispCad[nIdx], m_pMilBufCad[nIdx], hDispCtrl, rtDispCtrl.Width(), rtDispCtrl.Height(), DISPLAY_FIT_MODE_CENTERVIEW);
	}

	else if(nKind == DEF_IMG)
	{
		if(m_pMilDispDef[nIdx])
			m_pMil->DisplaySelect(m_pMilDispDef[nIdx], m_pMilBufDef[nIdx], hDispCtrl, rtDispCtrl.Width(), rtDispCtrl.Height(), DISPLAY_FIT_MODE_CENTERVIEW);
	}
}

void CVision::SelDispCad(HWND hDispCtrl, CRect rtDispCtrl, int nIdx, int nDisplayFitMode)
{
	if(!m_pMil)
		return;


	// for CAD..........

	// Live Image Buffer set
	if(m_pMilBufCad[nIdx])
	{
		delete m_pMilBufCad[nIdx];
		m_pMilBufCad[nIdx] = NULL;
	}

 	if(!m_pMilBufCad[nIdx])
		m_pMilBufCad[nIdx] = m_pMil->AllocBuf(DEF_IMG_DISP_SIZEX, DEF_IMG_DISP_SIZEY, 1L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC);

	// Mil Display set
	if(m_pMilDispCad[nIdx])
	{
		delete m_pMilDispCad[nIdx];
		m_pMilDispCad[nIdx] = NULL;
	}

	if(!m_pMilDispCad[nIdx])
	{
		m_pMilDispCad[nIdx] = m_pMil->AllocDisp();
		m_pMil->DisplaySelect(m_pMilDispCad[nIdx], m_pMilBufCad[nIdx], hDispCtrl, rtDispCtrl.Width(), rtDispCtrl.Height(), DISPLAY_FIT_MODE_CENTERVIEW);
	}

	// Create Overlay
	if(m_pMilDispCad[nIdx])
	{
		m_pMil->CreateOverlay(m_pMilDispCad[nIdx], M_COLOR_GREEN);
 		Sleep(100);
	}

	// Draw
	if(m_pMilOvrCad[nIdx])
	{
		delete m_pMilOvrCad[nIdx];
		m_pMilOvrCad[nIdx] = NULL;
	}

	if(!m_pMilOvrCad[nIdx])
	{
		m_pMilOvrCad[nIdx] = m_pMil->AllocDraw(m_pMilDispCad[nIdx]);
 		m_pMilOvrCad[nIdx]->SetDrawColor(M_COLOR_RED);
 		m_pMilOvrCad[nIdx]->SetDrawBackColor(m_pMilDispCad[nIdx]->m_lOverlayColor);
	}

	if(m_pMilDelOvrCad[nIdx])
	{
		delete m_pMilDelOvrCad[nIdx];
		m_pMilDelOvrCad[nIdx] = NULL;
	}

	if(!m_pMilDelOvrCad[nIdx])
	{
		m_pMilDelOvrCad[nIdx] = m_pMil->AllocDraw(m_pMilDispCad[nIdx]);
 		m_pMilDelOvrCad[nIdx]->SetDrawColor(m_pMilDispCad[nIdx]->m_lOverlayColor);
 		m_pMilDelOvrCad[nIdx]->SetDrawBackColor(m_pMilDispCad[nIdx]->m_lOverlayColor);
	}

}

void CVision::SetOvrCadFontSz(int nIdxMkInfo)
{
	int nDisplayMargin = 3;
	double dFontSizePixelX = 1.5;
	double dFontSizePixelY = 1.5;

	if(m_pMilOvrCad[nIdxMkInfo])
		m_pMilOvrCad[nIdxMkInfo]->SetTextFontScale(dFontSizePixelX, dFontSizePixelY);
}

void CVision::SelDispDef(HWND hDispCtrl, CRect rtDispCtrl, int nIdx, int nDisplayFitMode)
{
	if(!m_pMil)
		return;

// 	// Mil System set

	// for Def.............

	// Live Image Buffer set
	if(m_pMilBufDef[nIdx])
	{
		delete m_pMilBufDef[nIdx];
		m_pMilBufDef[nIdx] = NULL;
	}

	if(!m_pMilBufDef[nIdx])
		m_pMilBufDef[nIdx] = m_pMil->AllocBuf(DEF_IMG_DISP_SIZEX,DEF_IMG_DISP_SIZEY,8L+M_UNSIGNED,M_IMAGE+M_DISP+M_PROC);

	// Mil Display set
	if(m_pMilDispDef[nIdx])
	{
		delete m_pMilDispDef[nIdx];
		m_pMilDispDef[nIdx] = NULL;
	}

	if(!m_pMilDispDef[nIdx])
	{
		m_pMilDispDef[nIdx] = m_pMil->AllocDisp();
		m_pMil->DisplaySelect(m_pMilDispDef[nIdx], m_pMilBufDef[nIdx], hDispCtrl, rtDispCtrl.Width(), rtDispCtrl.Height(), DISPLAY_FIT_MODE_CENTERVIEW);
	}

	// Create Overlay
	if(m_pMilDispDef[nIdx])
	{
		m_pMil->CreateOverlay(m_pMilDispDef[nIdx], M_COLOR_GREEN);
		Sleep(100);
	}

	// Draw
	if(m_pMilOvrDef[nIdx])
	{
		delete m_pMilOvrDef[nIdx];
		m_pMilOvrDef[nIdx] = NULL;
	}

	if(!m_pMilOvrDef[nIdx])
	{
		m_pMilOvrDef[nIdx] = m_pMil->AllocDraw(m_pMilDispDef[nIdx]);
 		m_pMilOvrDef[nIdx]->SetDrawColor(M_COLOR_GREEN);
 		m_pMilOvrDef[nIdx]->SetDrawBackColor(m_pMilDispDef[nIdx]->m_lOverlayColor);
	}

	if(m_pMilDelOvrDef[nIdx])
	{
		delete m_pMilDelOvrDef[nIdx];
		m_pMilDelOvrDef[nIdx] = NULL;
	}

	if(!m_pMilDelOvrDef[nIdx])
	{
		m_pMilDelOvrDef[nIdx] = m_pMil->AllocDraw(m_pMilDispDef[nIdx]);
 		m_pMilDelOvrDef[nIdx]->SetDrawColor(m_pMilDispDef[nIdx]->m_lOverlayColor);
 		m_pMilDelOvrDef[nIdx]->SetDrawBackColor(m_pMilDispDef[nIdx]->m_lOverlayColor);
	}
}

void CVision::InitCADBuf(int nLayer)
{
	REGION_STRIP *pCellRgn[2];
	pCellRgn[nLayer] = pDoc->m_Master[nLayer].m_pCellRgn;

	if(MilCADImgBuf)
		MbufFree(MilCADImgBuf);
	if (pCellRgn[nLayer])
	{
		if (m_pMil)
			MbufAlloc2d(m_pMil->GetSystemID(), pCellRgn[nLayer]->ProcSizeX, pCellRgn[nLayer]->ProcSizeY, 1L + M_UNSIGNED, M_IMAGE + M_PROC, &MilCADImgBuf);
	}


	if(MilBufCADTemp)
		MbufFree(MilBufCADTemp);
	if (m_pMil)
		MbufAlloc2d(m_pMil->GetSystemID(), DEF_IMG_DISP_SIZEX, DEF_IMG_DISP_SIZEY, 1L + M_UNSIGNED, M_IMAGE+M_PROC, &MilBufCADTemp);
}

void CVision::InitDispDef()
{
	for(int i=0; i<DEF_VIEW_IMG_NUMBER; i++)
	{
		// for Def....
		if(m_pMilOvrDef[i])
		{
			delete m_pMilOvrDef[i];
			m_pMilOvrDef[i] = NULL;
		}
		if(m_pMilDelOvrDef[i])
		{
			delete m_pMilDelOvrDef[i];
			m_pMilDelOvrDef[i] = NULL;
		}
		if(m_pMilBufDef[i])
		{
			delete m_pMilBufDef[i];
			m_pMilBufDef[i] = NULL;
		}
		if(m_pMilDispDef[i])
		{
			delete m_pMilDispDef[i];
			m_pMilDispDef[i] = NULL;
		}
	}
}

void CVision::ClrDispCad()
{
	// for CAD...
	for(int i=0; i<DEF_VIEW_IMG_NUMBER; i++)
	{
		if(m_pMilOvrCad[i])
			MbufClear(m_pMilOvrCad[i]->m_MilBuffer, (double)(m_pMilDispCad[i]->m_lOverlayColor));

		if(m_pMilDelOvrCad[i])
			MbufClear(m_pMilDelOvrCad[i]->m_MilBuffer, (double)(m_pMilDispCad[i]->m_lOverlayColor));

		if(m_pMilBufCad[i])
			MbufClear(m_pMilBufCad[i]->m_MilImage, 0L);
	}
}

void CVision::ClrDispDef()
{
	for(int i=0; i<DEF_VIEW_IMG_NUMBER; i++)
	{
		// for Def....
		if(m_pMilOvrDef[i])
			MbufClear(m_pMilOvrDef[i]->m_MilBuffer, (double)m_pMilDispDef[i]->m_lOverlayColor);

		if(m_pMilDelOvrDef[i])
			MbufClear(m_pMilDelOvrDef[i]->m_MilBuffer, (double)m_pMilDispDef[i]->m_lOverlayColor);

		if(m_pMilBufDef[i])
			MbufClear(m_pMilBufDef[i]->m_MilImage, 0L);
	}
}

void CVision::SelDispPin(HWND hDispCtrl, CRect rtDispCtrl, int nDisplayFitMode)
{
	// Mil System set
	if(!m_pMil)
		return;

	// for CAD..........

	// Live Image Buffer set
	if(m_pMilBufPin == NULL)
	{
		m_pMilBufPin = m_pMil->AllocBuf(3, PIN_IMG_DISP_SIZEX, PIN_IMG_DISP_SIZEY, 1L+M_UNSIGNED , M_IMAGE+M_DISP+M_PROC);
	}

	// Mil Display set
	if(m_pMilDispPin == NULL)
	{
		m_pMilDispPin = m_pMil->AllocDisp();
		m_pMil->DisplaySelect(m_pMilDispPin, m_pMilBufPin, hDispCtrl, rtDispCtrl.Width(), rtDispCtrl.Height(), DISPLAY_FIT_MODE_CENTERVIEW);
	}
	// Create Overlay
	if(m_pMilDispPin)
	{
		m_pMil->CreateOverlay(m_pMilDispPin, M_COLOR_GREEN);
		Sleep(30);
	}
	
	// Draw
	if(!m_pMilPinOverlay)
	{
		m_pMilPinOverlay = m_pMil->AllocDraw(m_pMilDispPin);
 		m_pMilPinOverlay->SetDrawColor(M_COLOR_GREEN);
 		m_pMilPinOverlay->SetDrawBackColor(m_pMilDispPin->m_lOverlayColor);
	}
	if(!m_pMilPinOverlayDelete)
	{
		m_pMilPinOverlayDelete = m_pMil->AllocDraw(m_pMilDispPin);
 		m_pMilPinOverlayDelete->SetDrawColor(m_pMilDispPin->m_lOverlayColor);
 		m_pMilPinOverlayDelete->SetDrawBackColor(m_pMilDispPin->m_lOverlayColor);
	}
}

void CVision::SelDispAlign(HWND hDispCtrl, CRect rtDispCtrl, int nDisplayFitMode)
{
	// Mil System set
	if(!m_pMil)
		return;

	// for CAD..........

	// Live Image Buffer set
	if(m_pMilBufAlign == NULL)
	{
		m_pMilBufAlign = m_pMil->AllocBuf(3, ALIGN_IMG_DISP_SIZEX, ALIGN_IMG_DISP_SIZEY, 1L+M_UNSIGNED , M_IMAGE+M_DISP+M_PROC);
	}
}

void CVision::ShiftDisp()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if(bDualTest)
	{
		for(int i=0; i<MENU01_STC_DEFINFO_HARF-1; i++)
		{
			MbufCopy(m_pMilBufCad[i+1]->m_MilImage, m_pMilBufCad[i]->m_MilImage);
			MbufCopy(m_pMilOvrCad[i+1]->m_MilBuffer, m_pMilOvrCad[i]->m_MilBuffer);
			MbufCopy(m_pMilBufDef[i+1]->m_MilImage, m_pMilBufDef[i]->m_MilImage);
		}
	}
	else
	{
		for(int i=0; i<MENU01_STC_DEFINFO_HARF*2-1; i++)
		{
			MbufCopy(m_pMilBufCad[i+1]->m_MilImage, m_pMilBufCad[i]->m_MilImage);
			MbufCopy(m_pMilOvrCad[i+1]->m_MilBuffer, m_pMilOvrCad[i]->m_MilBuffer);
			MbufCopy(m_pMilBufDef[i+1]->m_MilImage, m_pMilBufDef[i]->m_MilImage);
		}
	}
}

void CVision::ShowDispCad(int nIdxMkInfo, int nSerial, int nLayer, int nIdxDef) // From 0 To 12...for Screen display.
{
	if(m_pMilBufCad[nIdxMkInfo])
	{
		CropCadImg(nIdxMkInfo, nSerial, nLayer, nIdxDef);
	}
}

void CVision::CropCadImg(int nIdxMkInfo, int nSerial, int nLayer, int nIdxDef)
{
	if(!pDoc->m_pPcr[nLayer])
		return;
	short cell, cx, cy;
#ifndef USE_MIL
	cell = pDoc->m_pPcr[nLayer][TEST_SHOT-1]->m_pCell[0];			// for Test - BufIdx[0], DefIdx[0]
	cx = pDoc->m_pPcr[nLayer][TEST_SHOT-1]->m_pDefPos[0].x;		// for Test - BufIdx[0], DefIdx[0]
	cy = pDoc->m_pPcr[nLayer][TEST_SHOT-1]->m_pDefPos[0].y;		// for Test - BufIdx[0], DefIdx[0]
#else
	int nIdx = pDoc->GetPcrIdx1(nSerial);
	if(!pDoc->m_pPcr[nLayer][nIdx])
		return;
	if(!pDoc->m_pPcr[nLayer][nIdx]->m_pCell || !pDoc->m_pPcr[nLayer][nIdx]->m_pDefPos || 
		!pDoc->m_Master[nLayer].m_pCellRgn)
		return;

	cell = pDoc->m_pPcr[nLayer][nIdx]->m_pCell[nIdxDef];										// BufIdx[], DefIdx[]
	cx = pDoc->m_pPcr[nLayer][nIdx]->m_pDefPos[nIdxDef].x - pDoc->m_Master[nLayer].m_pCellRgn->StPosX[cell];		// BufIdx[], DefIdx[]
	cy = pDoc->m_pPcr[nLayer][nIdx]->m_pDefPos[nIdxDef].y - pDoc->m_Master[nLayer].m_pCellRgn->StPosY[cell];		// BufIdx[], DefIdx[]
#endif
	CropCadImg(cell, cx, cy, nIdxMkInfo, nLayer);
}

void CVision::CropCadImg(short cell, short cx, short cy, int BufID, int nLayer)
{
	CString str;

	int CellX, CellY;
	BOOL IsLeftSwath, IsRightSwath, IsTopNode, IsBottomNode;	// 패널 이미지의 가장자리부분을 나타냄.
	BOOL IsUpNode, IsDownNode;									// 스트립의 상하 가장자리부분을 나타냄.
	int dx, dy;
	int stx, sty, edx, edy;
	int CelNum;

	REGION_STRIP *pCellRgn[2];
	pCellRgn[nLayer] = pDoc->m_Master[nLayer].m_pCellRgn;

	CellX = cell / pDoc->m_Master[nLayer].m_pCellRgn->NodeNumY;
	CellY = cell % pDoc->m_Master[nLayer].m_pCellRgn->NodeNumY;

	dx = DEF_IMG_DISP_SIZEX;
	dy = DEF_IMG_DISP_SIZEY;

	stx = cx - dx / 2;
	sty = cy - dy / 2;
	edx = cx + dx / 2;
	edy = cy + dy / 2;

	IsUpNode = 0;
	IsDownNode = 0;

	if (pCellRgn[nLayer]->NodeNumY > 0)
	{
		IsLeftSwath = (CellX == 0 ? 1 : 0);
		IsRightSwath = ((CellX + 1) ==pCellRgn[nLayer]->NodeNumX ? 1 : 0);
		IsTopNode  = (CellY == 0 ? 1 : 0);
		IsBottomNode  = ((CellY + 1) == pCellRgn[nLayer]->NodeNumY ? 1 : 0);

		IsUpNode = (CellY % pCellRgn[nLayer]->nCellDivideY == 0 ? 1 : 0);
		IsDownNode = ((CellY % pCellRgn[nLayer]->nCellDivideY) == (pCellRgn[nLayer]->nCellDivideY-1) ? 1 : 0);
	}
	else
	{
		pView->MsgBox(_T("Invalid CAD Img Info"));
// 		AfxMessageBox(_T("Invalid CAD Img Info"));
		return;
	}


	if(stx < 0)
	{
		if(sty < 0)
		{
			if(IsLeftSwath && IsTopNode)
			{
				SetCADCoord(cell, stx, sty, 1, nLayer);
				CelNum = 1;
			}
			else if(IsLeftSwath)
			{
				if(IsUpNode)
				{
					SetCADCoord(cell, stx, sty, 1, nLayer);
					CelNum = 1;
					if(0 <= (pCellRgn[nLayer]->StPosY[cell] - pCellRgn[nLayer]->EdPosY[cell-1]))
					{
						if((pCellRgn[nLayer]->StPosY[cell] - pCellRgn[nLayer]->EdPosY[cell-1]) < -sty)
						{
							SetCADCoord(cell, stx, sty, 31, nLayer);
							CelNum = 2;
						}					
					}
					else if((pCellRgn[nLayer]->StPosY[cell] - pCellRgn[nLayer]->EdPosY[cell-1]) < 0)
					{
							SetCADCoord(cell, stx, sty, 41, nLayer);
							CelNum = 2;						
					}
				}
				else
				{
					SetCADCoord(cell, stx, sty, 1, nLayer);
					SetCADCoord(cell, stx, sty, 10, nLayer);
					CelNum = 2;
				}
			}
			else if(IsTopNode)
			{
				SetCADCoord(cell, stx, sty, 1, nLayer);
				SetCADCoord(cell, stx, sty, 24, nLayer);
				CelNum = 2;
			}
			else
			{
				if(IsUpNode)
				{
					SetCADCoord(cell, stx, sty, 1, nLayer);
					SetCADCoord(cell, stx, sty, 24, nLayer);
					CelNum = 2;
					if(0 <= (pCellRgn[nLayer]->StPosY[cell] - pCellRgn[nLayer]->EdPosY[cell-1]))
					{
						if((pCellRgn[nLayer]->StPosY[cell] - pCellRgn[nLayer]->EdPosY[cell-1]) < -sty)
						{
							SetCADCoord(cell, stx, sty, 30, nLayer);
							SetCADCoord(cell, stx, sty, 31, nLayer);
							CelNum = 4;
						}					
					}
					else if((pCellRgn[nLayer]->StPosY[cell] - pCellRgn[nLayer]->EdPosY[cell-1]) < 0)
					{
							SetCADCoord(cell, stx, sty, 40, nLayer);
							SetCADCoord(cell, stx, sty, 41, nLayer);
							CelNum = 4;						
					}
				}
				else
				{
					SetCADCoord(cell, stx, sty, 1, nLayer);
					SetCADCoord(cell, stx, sty, 9, nLayer);
					SetCADCoord(cell, stx, sty, 10, nLayer);
					SetCADCoord(cell, stx, sty, 24, nLayer);
					CelNum = 4;
				}
			}
		}
		else if(edy > pCellRgn[nLayer]->ProcSizeY)
		{
			if(IsLeftSwath && IsBottomNode)
			{
				SetCADCoord(cell, stx, sty, 7, nLayer);
				CelNum = 1;
			}
			else if(IsLeftSwath)
			{
				if(IsDownNode)
				{
					SetCADCoord(cell, stx, sty, 7, nLayer);
					CelNum = 1;
					if(0 <= (pCellRgn[nLayer]->StPosY[cell+1] - pCellRgn[nLayer]->EdPosY[cell]))
					{
						if((pCellRgn[nLayer]->StPosY[cell+1] - pCellRgn[nLayer]->EdPosY[cell]) < (DEF_IMG_DISP_SIZEY - (pCellRgn[nLayer]->ProcSizeY - sty)))
						{
							SetCADCoord(cell, stx, sty, 26, nLayer);
							CelNum = 2;
						}					
					}
					else if((pCellRgn[nLayer]->StPosY[cell+1] - pCellRgn[nLayer]->EdPosY[cell]) < 0)
					{
							SetCADCoord(cell, stx, sty, 36, nLayer);
							CelNum = 2;						
					}
				}
				else
				{
					SetCADCoord(cell, stx, sty, 7, nLayer);
					SetCADCoord(cell, stx, sty, 20, nLayer);
					CelNum = 2;
				}
			}
			else if(IsBottomNode)
			{
				SetCADCoord(cell, stx, sty, 7, nLayer);
				SetCADCoord(cell, stx, sty, 22, nLayer);
				CelNum = 2;
			}
			else
			{
				if(IsDownNode)
				{
					SetCADCoord(cell, stx, sty, 7, nLayer);
					SetCADCoord(cell, stx, sty, 22, nLayer);
					CelNum = 2;
					if(0 <= (pCellRgn[nLayer]->StPosY[cell+1] - pCellRgn[nLayer]->EdPosY[cell]))
					{
						if((pCellRgn[nLayer]->StPosY[cell+1] - pCellRgn[nLayer]->EdPosY[cell]) < (DEF_IMG_DISP_SIZEY - (pCellRgn[nLayer]->ProcSizeY - sty)))
						{
							SetCADCoord(cell, stx, sty, 25, nLayer);
							SetCADCoord(cell, stx, sty, 26, nLayer);
							CelNum = 4;
						}			
					}
					else if((pCellRgn[nLayer]->StPosY[cell+1] - pCellRgn[nLayer]->EdPosY[cell]) < 0)
					{
							SetCADCoord(cell, stx, sty, 35, nLayer);
							SetCADCoord(cell, stx, sty, 36, nLayer);
							CelNum = 4;
					}
				}
				else
				{
					SetCADCoord(cell, stx, sty, 7, nLayer);
					SetCADCoord(cell, stx, sty, 20, nLayer);
					SetCADCoord(cell, stx, sty, 21, nLayer);
					SetCADCoord(cell, stx, sty, 22, nLayer);
					CelNum = 4;
				}
			}
		}
		else
		{
			SetCADCoord(cell, stx, sty, 8, nLayer);
			CelNum = 1;
			if(!IsLeftSwath)
			{
				SetCADCoord(cell, stx, sty, 23, nLayer);
				CelNum = 2;
			}
		}
	}
	else if(edx > pCellRgn[nLayer]->ProcSizeX)
	{
		if(sty < 0)
		{
			if(IsRightSwath && IsTopNode)
			{
				SetCADCoord(cell, stx, sty, 3, nLayer);
				CelNum = 1;
			}
			else if(IsRightSwath)
			{
				if(IsUpNode)
				{
					SetCADCoord(cell, stx, sty, 3, nLayer);
					CelNum = 1;
					if(0 <= (pCellRgn[nLayer]->StPosY[cell] - pCellRgn[nLayer]->EdPosY[cell-1]))
					{
						if((pCellRgn[nLayer]->StPosY[cell] - pCellRgn[nLayer]->EdPosY[cell-1]) < -sty)
						{
							SetCADCoord(cell, stx, sty, 33, nLayer);
							CelNum = 2;
						}
					}
					else if((pCellRgn[nLayer]->StPosY[cell] - pCellRgn[nLayer]->EdPosY[cell-1]) < 0)
					{
							SetCADCoord(cell, stx, sty, 43, nLayer);
							CelNum = 2;
					}
				}
				else
				{
					SetCADCoord(cell, stx, sty, 3, nLayer);
					SetCADCoord(cell, stx, sty, 12, nLayer);
					CelNum = 2;
				}
			}
			else if(IsTopNode)
			{
				SetCADCoord(cell, stx, sty, 3, nLayer);
				SetCADCoord(cell, stx, sty, 14, nLayer);
				CelNum = 2;
			}
			else
			{
				if(IsUpNode)
				{
					SetCADCoord(cell, stx, sty, 3, nLayer);
					SetCADCoord(cell, stx, sty, 14, nLayer);
					CelNum = 2;
					if(0 <= (pCellRgn[nLayer]->StPosY[cell] - pCellRgn[nLayer]->EdPosY[cell-1]))
					{
						if((pCellRgn[nLayer]->StPosY[cell] - pCellRgn[nLayer]->EdPosY[cell-1]) < -sty)
						{
							SetCADCoord(cell, stx, sty, 33, nLayer);
							SetCADCoord(cell, stx, sty, 34, nLayer);
							CelNum = 4;
						}
					}
					else if((pCellRgn[nLayer]->StPosY[cell] - pCellRgn[nLayer]->EdPosY[cell-1]) < 0)
					{
							SetCADCoord(cell, stx, sty, 43, nLayer);
							SetCADCoord(cell, stx, sty, 44, nLayer);
							CelNum = 4;				
					}
				}
				else
				{
					SetCADCoord(cell, stx, sty, 3, nLayer);
					SetCADCoord(cell, stx, sty, 12, nLayer);
					SetCADCoord(cell, stx, sty, 13, nLayer);
					SetCADCoord(cell, stx, sty, 14, nLayer);
					CelNum = 4;
				}
			}
		}
		else if(edy > pCellRgn[nLayer]->ProcSizeY)
		{
			if(IsRightSwath && IsBottomNode)
			{
				SetCADCoord(cell, stx, sty, 5, nLayer);
				CelNum = 1;
			}
			else if(IsRightSwath)
			{
				if(IsDownNode)
				{
					SetCADCoord(cell, stx, sty, 5, nLayer);
					CelNum = 1;
					if(0 <= (pCellRgn[nLayer]->StPosY[cell+1] - pCellRgn[nLayer]->EdPosY[cell]))
					{
						if((pCellRgn[nLayer]->StPosY[cell+1] - pCellRgn[nLayer]->EdPosY[cell]) < (DEF_IMG_DISP_SIZEY - (pCellRgn[nLayer]->ProcSizeY - sty)))
						{
							SetCADCoord(cell, stx, sty, 28, nLayer);
							CelNum = 2;
						}
					}
					else if((pCellRgn[nLayer]->StPosY[cell+1] - pCellRgn[nLayer]->EdPosY[cell]) < 0)
					{
							SetCADCoord(cell, stx, sty, 38, nLayer);
							CelNum = 2;
					}
				}
				else
				{
					SetCADCoord(cell, stx, sty, 5, nLayer);
					SetCADCoord(cell, stx, sty, 18, nLayer);
					CelNum = 2;
				}
			}
			else if(IsBottomNode)
			{
				SetCADCoord(cell, stx, sty, 5, nLayer);
				SetCADCoord(cell, stx, sty, 16, nLayer);
				CelNum = 2;
			}
			else
			{
				if(IsDownNode)
				{
					SetCADCoord(cell, stx, sty, 5, nLayer);
					SetCADCoord(cell, stx, sty, 16, nLayer);
					CelNum = 2;
					if(0 <= (pCellRgn[nLayer]->StPosY[cell+1] - pCellRgn[nLayer]->EdPosY[cell]))
					{
						if((pCellRgn[nLayer]->StPosY[cell+1] - pCellRgn[nLayer]->EdPosY[cell]) < (DEF_IMG_DISP_SIZEY - (pCellRgn[nLayer]->ProcSizeY - sty)))
						{
							SetCADCoord(cell, stx, sty, 28, nLayer);
							SetCADCoord(cell, stx, sty, 29, nLayer);
							CelNum = 4;
						}

					}
					else if((pCellRgn[nLayer]->StPosY[cell+1] - pCellRgn[nLayer]->EdPosY[cell]) < 0)
					{
							SetCADCoord(cell, stx, sty, 38, nLayer);
							SetCADCoord(cell, stx, sty, 39, nLayer);
							CelNum = 4;
					}
				}
				else
				{
					SetCADCoord(cell, stx, sty, 5, nLayer);
					SetCADCoord(cell, stx, sty, 16, nLayer);
					SetCADCoord(cell, stx, sty, 17, nLayer);
					SetCADCoord(cell, stx, sty, 18, nLayer);
					CelNum = 4;
				}
			}
		}
		else
		{
			SetCADCoord(cell, stx, sty, 4, nLayer);
			CelNum = 1;
			if(!IsRightSwath)
			{
				SetCADCoord(cell, stx, sty, 15, nLayer);
				CelNum = 2;
			}
		}
	}
	else
	{
		if(sty < 0)
		{
			if(IsTopNode)
			{
				SetCADCoord(cell, stx, sty, 2, nLayer);
				CelNum = 1;
			}
			else if(IsUpNode)
			{
				SetCADCoord(cell, stx, sty, 2, nLayer);
				CelNum = 1;
				if(0 <= (pCellRgn[nLayer]->StPosY[cell] - pCellRgn[nLayer]->EdPosY[cell-1]))
				{
					if((pCellRgn[nLayer]->StPosY[cell] - pCellRgn[nLayer]->EdPosY[cell-1]) < -sty)
					{
						SetCADCoord(cell, stx, sty, 32, nLayer);
						CelNum = 2;
					}
				}
				else if((pCellRgn[nLayer]->StPosY[cell] - pCellRgn[nLayer]->EdPosY[cell-1]) < 0)
				{
						SetCADCoord(cell, stx, sty, 42, nLayer);
						CelNum = 2;
				}
			}
			else
			{
				SetCADCoord(cell, stx, sty, 2, nLayer);
				SetCADCoord(cell, stx, sty, 11, nLayer);
				CelNum = 2;
			}
		}
		else if(edy > pCellRgn[nLayer]->ProcSizeY)
		{
			if(IsBottomNode)
			{
				SetCADCoord(cell, stx, sty, 6, nLayer);
				CelNum = 1;
			}
			else if(IsDownNode)
			{
				SetCADCoord(cell, stx, sty, 6, nLayer);
				CelNum = 1;
				if(0 <= (pCellRgn[nLayer]->StPosY[cell+1] - pCellRgn[nLayer]->EdPosY[cell]))
				{
					if((pCellRgn[nLayer]->StPosY[cell+1] - pCellRgn[nLayer]->EdPosY[cell]) < (DEF_IMG_DISP_SIZEY - (pCellRgn[nLayer]->ProcSizeY - sty)))
					{
						SetCADCoord(cell, stx, sty, 27, nLayer);
						CelNum = 2;
					}
				}
				else if((pCellRgn[nLayer]->StPosY[cell+1] - pCellRgn[nLayer]->EdPosY[cell]) < 0)
				{
						SetCADCoord(cell, stx, sty, 37, nLayer);
						CelNum = 2;
				}
			}
			else
			{
				SetCADCoord(cell, stx, sty, 6, nLayer);
				SetCADCoord(cell, stx, sty, 19, nLayer);
				CelNum = 2;
			}
		}
		else
		{
			SetCADCoord(cell, stx, sty, 0, nLayer);
			CelNum = 1;
		}
	}

//	m_pMilBufCad[BufID]->BufferCopy(MilBufCADTemp, m_pMilBufCad[BufID]->m_MilImage);
//	MbufSave(_T("C:\\R2RSet\\Temp.tif"), MilBufCADTemp));

	MbufCopy(MilBufCADTemp, m_pMilBufCad[BufID]->m_MilImage);
//	MbufControl(m_pMilBufCad[BufID]->m_MilImage, M_MODIFIED, M_DEFAULT);
}

BOOL CVision::SetCADCoord(int CellNum, int StX, int StY, int Coord, int nLayer)
{
	BOOL RValue;
	long OrginalX, OrginalY;
	long DestX, DestY;
	long SizeX, SizeY;
	long EmpStripThick;

	REGION_STRIP *pCellRgn[2];
	pCellRgn[nLayer] = pDoc->m_Master[nLayer].m_pCellRgn;

	switch(Coord) 
	{
	case 0:		// In Current Cell
		if(StX < 0 || StY < 0)
		{
			RValue = FALSE;
			break;
		}
		OrginalX = StX;
		OrginalY = StY;
		SizeX = DEF_IMG_DISP_SIZEX;
		SizeY = DEF_IMG_DISP_SIZEY;
		DestX = 0;
		DestY = 0;
		LoadCADBuf(CellNum, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	case 1:		// Left Top Corner Inside
		if(StX >= 0 || StY >= 0)
		{
		   RValue = FALSE;
		   break;
		}
		OrginalX = 0;
		OrginalY = 0;
		SizeX = DEF_IMG_DISP_SIZEX+StX;
		SizeY = DEF_IMG_DISP_SIZEY+StY;
		DestX = -StX;
		DestY = -StY;
		LoadCADBuf(CellNum, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
		RValue = TRUE;
		break;
	case 2:		// Top Inside
	   if(StX < 0 || StY >= 0)
	   {
		   RValue = FALSE;
		   break;
	   }
	   OrginalX = StX;
	   OrginalY = 0;
	   SizeX = DEF_IMG_DISP_SIZEX;
	   SizeY = DEF_IMG_DISP_SIZEY+StY;
	   DestX = 0;
	   DestY = -StY;
	   LoadCADBuf(CellNum, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
	   RValue = TRUE;
	   break;
	case 3:		// Right Top Corner Inside
	   if(StX < 0 || StY >= 0)
	   {
		   RValue = FALSE;
		   break;
	   }
	   OrginalX = StX;
	   OrginalY = 0;
	   SizeX = pCellRgn[nLayer]->ProcSizeX - StX;
	   SizeY = DEF_IMG_DISP_SIZEY+StY;
	   DestX = 0;
	   DestY = -StY;
	   LoadCADBuf(CellNum, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
	   RValue = TRUE;
	   break;
	case 4:		// Right Inside
	   if(StX < 0 || StY < 0)
	   {
		   RValue = FALSE;
		   break;
	   }
	   OrginalX = StX;
	   OrginalY = StY;
	   SizeX = pCellRgn[nLayer]->ProcSizeX - StX;
	   SizeY = DEF_IMG_DISP_SIZEY;
	   DestX = 0;
	   DestY = 0;
	   LoadCADBuf(CellNum, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
	   RValue = TRUE;
	   break;
	case 5:		// Right Bottom Corner Inside
	   if(StX < 0 || StY < 0)
	   {
		   RValue = FALSE;
		   break;
	   }
	   OrginalX = StX;
	   OrginalY = StY;
	   SizeX = pCellRgn[nLayer]->ProcSizeX - StX;
	   SizeY = pCellRgn[nLayer]->ProcSizeY - StY;
	   DestX = 0;
	   DestY = 0;
	   LoadCADBuf(CellNum, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
	   RValue = TRUE;
	   break;
	case 6:		// Bottom Inside
	   if(StX < 0 || StY < 0)
	   {
		   RValue = FALSE;
		   break;
	   }
	   OrginalX = StX;
	   OrginalY = StY;
	   SizeX = DEF_IMG_DISP_SIZEX;
	   SizeY = pCellRgn[nLayer]->ProcSizeY - StY;
	   DestX = 0;
	   DestY = 0;
	   LoadCADBuf(CellNum, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
	   RValue = TRUE;
	   break;
	case 7:		// Left Bottom Corner Inside
	   if(StX >= 0 || StY < 0)
	   {
		   RValue = FALSE;
		   break;
	   }
	   OrginalX = 0;
	   OrginalY = StY;
	   SizeX = DEF_IMG_DISP_SIZEX + StX;
	   SizeY = pCellRgn[nLayer]->ProcSizeY - StY;
	   DestX = -StX;
	   DestY = 0;
	   LoadCADBuf(CellNum, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
	   RValue = TRUE;
	   break;
	case 8:		// Left Inside
	   if(StX >= 0 || StY < 0)
	   {
		   RValue = FALSE;
		   break;
	   }
	   OrginalX = 0;
	   OrginalY = StY;
	   SizeX = DEF_IMG_DISP_SIZEX + StX;
	   SizeY = DEF_IMG_DISP_SIZEY;
	   DestX = -StX;
	   DestY = 0;
	   LoadCADBuf(CellNum, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
	   RValue = TRUE;
	   break;
	case 9:		// Left Top Corner Outside
	   if(StX >= 0 || StY >= 0)
	   {
		   RValue = FALSE;
		   break;
	   }
	   OrginalX = pCellRgn[nLayer]->ProcSizeX + StX - pCellRgn[nLayer]->OvrXPix;
	   OrginalY = pCellRgn[nLayer]->ProcSizeY + StY - pCellRgn[nLayer]->OvrYPix;
	   SizeX = -StX;
	   SizeY = -StY;
	   DestX = 0;
	   DestY = 0;
	   LoadCADBuf(CellNum - pCellRgn[nLayer]->NodeNumY - 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
	   RValue = TRUE;
	   break;
	case 10:	// Top of Left Top Corner Outside
	   if(StX >= 0 || StY >= 0)
	   {
		   RValue = FALSE;
		   break;
	   }
	   OrginalX = 0;
	   OrginalY = pCellRgn[nLayer]->ProcSizeY + StY - pCellRgn[nLayer]->OvrYPix;
	   SizeX = DEF_IMG_DISP_SIZEX + StX;
	   SizeY = -StY;
	   DestX = -StX;
	   DestY = 0;
	   LoadCADBuf(CellNum - 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
	   RValue = TRUE;
	   break;
	case 11:		// Top Outside
	   if(StX < 0 || StY >= 0)
	   {
		   RValue = FALSE;
		   break;
	   }
	   OrginalX = StX;
	   OrginalY = pCellRgn[nLayer]->ProcSizeY + StY - pCellRgn[nLayer]->OvrYPix;
	   SizeX = DEF_IMG_DISP_SIZEX;
	   SizeY = -StY;
	   DestX = 0;
	   DestY = 0;
	   LoadCADBuf(CellNum - 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
	   RValue = TRUE;
	   break;
	case 12: 		// Top of Right Top Corner Outside
	   if(StX < 0 || StY >= 0)
	   {
		   RValue = FALSE;
		   break;
	   }
	   OrginalX = StX;
	   OrginalY = pCellRgn[nLayer]->ProcSizeY + StY - pCellRgn[nLayer]->OvrYPix;
	   SizeX = pCellRgn[nLayer]->ProcSizeX - StX;
	   SizeY = -StY;
	   DestX = 0;
	   DestY = 0;
	   LoadCADBuf(CellNum - 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
	   RValue = TRUE;
	   break;
	case 13:  	// Right Top Corner Outside
	   if(StX < 0 || StY >= 0)
	   {
		   RValue = FALSE;
		   break;
	   }
	   OrginalX = pCellRgn[nLayer]->OvrXPix;
	   OrginalY = pCellRgn[nLayer]->ProcSizeY + StY - pCellRgn[nLayer]->OvrYPix;
	   SizeX = DEF_IMG_DISP_SIZEX - (pCellRgn[nLayer]->ProcSizeX - StX);
	   SizeY = -StY;
	   DestX = pCellRgn[nLayer]->ProcSizeX - StX;
	   DestY = 0;
	   LoadCADBuf(CellNum + pCellRgn[nLayer]->NodeNumY - 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
	   RValue = TRUE;
	   break;
	case 14: 		// Right of Right Top Corner Outside
	   if(StX < 0 || StY >= 0)
	   {
		   RValue = FALSE;
		   break;
	   }
	   OrginalX = pCellRgn[nLayer]->OvrXPix;
	   OrginalY = 0;
	   SizeX = DEF_IMG_DISP_SIZEX - (pCellRgn[nLayer]->ProcSizeX - StX);
	   SizeY = DEF_IMG_DISP_SIZEY + StY;
	   DestX = pCellRgn[nLayer]->ProcSizeX - StX;
	   DestY = -StY;
	   LoadCADBuf(CellNum + pCellRgn[nLayer]->NodeNumY, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
	   RValue = TRUE;
	   break;
	case 15: 		// Right Outside
	   if(StX < 0 || StY < 0)
	   {
		   RValue = FALSE;
		   break;
	   }
	   OrginalX = pCellRgn[nLayer]->OvrXPix;
	   OrginalY = StY;
	   SizeX = DEF_IMG_DISP_SIZEX - (pCellRgn[nLayer]->ProcSizeX - StX);
	   SizeY = DEF_IMG_DISP_SIZEY;
	   DestX = pCellRgn[nLayer]->ProcSizeX - StX;
	   DestY = 0;
	   LoadCADBuf(CellNum + pCellRgn[nLayer]->NodeNumY, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
	   RValue = TRUE;
	   break;
	case 16: 		// Right of Right Bottom Corner Outside
	   if(StX < 0 || StY < 0)
	   {
		   RValue = FALSE;
		   break;
	   }
	   OrginalX = pCellRgn[nLayer]->OvrXPix;
	   OrginalY = StY;
	   SizeX = DEF_IMG_DISP_SIZEX - (pCellRgn[nLayer]->ProcSizeX - StX);
	   SizeY = pCellRgn[nLayer]->ProcSizeY - StY;
	   DestX = pCellRgn[nLayer]->ProcSizeX - StX;
	   DestY = 0;
	   LoadCADBuf(CellNum + pCellRgn[nLayer]->NodeNumY, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
	   RValue = TRUE;
	   break;
	case 17: 		// Right of Right Bottom Corner Outside
	   if(StX < 0 || StY < 0)
	   {
		   RValue = FALSE;
		   break;
	   }
	   OrginalX = pCellRgn[nLayer]->OvrXPix;
	   OrginalY = pCellRgn[nLayer]->OvrYPix;
	   SizeX = DEF_IMG_DISP_SIZEX - (pCellRgn[nLayer]->ProcSizeX - StX);
	   SizeY = DEF_IMG_DISP_SIZEY - (pCellRgn[nLayer]->ProcSizeY - StY);
	   DestX = pCellRgn[nLayer]->ProcSizeX - StX;
	   DestY = pCellRgn[nLayer]->ProcSizeY - StY;
	   LoadCADBuf(CellNum + pCellRgn[nLayer]->NodeNumY + 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
	   RValue = TRUE;
	   break;
	case 18: 		// Bottom of Right Bottom Corner Outside
	   if(StX < 0 || StY < 0)
	   {
		   RValue = FALSE;
		   break;
	   }
	   OrginalX = StX;
	   OrginalY = pCellRgn[nLayer]->OvrYPix;
	   SizeX = pCellRgn[nLayer]->ProcSizeX - StX;
	   SizeY = DEF_IMG_DISP_SIZEY - (pCellRgn[nLayer]->ProcSizeY - StY);
	   DestX = 0;
	   DestY = pCellRgn[nLayer]->ProcSizeY - StY;
	   LoadCADBuf(CellNum + 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
	   RValue = TRUE;
	   break;
	case 19: 		// Bottom Outside
	   if(StX < 0 || StY < 0)
	   {
		   RValue = FALSE;
		   break;
	   }
	   OrginalX = StX;
	   OrginalY = pCellRgn[nLayer]->OvrYPix;
	   SizeX = DEF_IMG_DISP_SIZEX;
	   SizeY = DEF_IMG_DISP_SIZEY - (pCellRgn[nLayer]->ProcSizeY - StY);
	   DestX = 0;
	   DestY = pCellRgn[nLayer]->ProcSizeY - StY;
	   LoadCADBuf(CellNum + 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
	   RValue = TRUE;
	   break;
	case 20: 		// Bottom of Left Bottom Corner Outside
	   if(StX >= 0 || StY < 0)
	   {
		   RValue = FALSE;
		   break;
	   }
	   OrginalX = 0;
	   OrginalY = pCellRgn[nLayer]->OvrYPix;
	   SizeX = DEF_IMG_DISP_SIZEX + StX;
	   SizeY = DEF_IMG_DISP_SIZEY - (pCellRgn[nLayer]->ProcSizeY - StY);
	   DestX = -StX;
	   DestY = pCellRgn[nLayer]->ProcSizeY - StY;
	   LoadCADBuf(CellNum + 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
	   RValue = TRUE;
	   break;
	case 21: 		// Left Bottom of Left Bottom Corner Outside
	   if(StX >= 0 || StY < 0)
	   {
		   RValue = FALSE;
		   break;
	   }
	   OrginalX = pCellRgn[nLayer]->ProcSizeX + StX - pCellRgn[nLayer]->OvrXPix;
	   OrginalY = pCellRgn[nLayer]->OvrYPix;
	   SizeX = -StX;
	   SizeY = DEF_IMG_DISP_SIZEY - (pCellRgn[nLayer]->ProcSizeY - StY);
	   DestX = 0;
	   DestY = pCellRgn[nLayer]->ProcSizeY - StY;
	   LoadCADBuf(CellNum - pCellRgn[nLayer]->NodeNumY + 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
	   RValue = TRUE;
	   break;
	case 22: 		// Left Bottom of Left Bottom Corner Outside
	   if(StX >= 0 || StY < 0)
	   {
		   RValue = FALSE;
		   break;
	   }
	   OrginalX = pCellRgn[nLayer]->ProcSizeX + StX - pCellRgn[nLayer]->OvrXPix;
	   OrginalY = StY;
	   SizeX = -StX;
	   SizeY = pCellRgn[nLayer]->ProcSizeY - StY;
	   DestX = 0;
	   DestY = 0;
	   LoadCADBuf(CellNum - pCellRgn[nLayer]->NodeNumY, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
	   RValue = TRUE;
	   break;
	case 23: 		// Left Bottom of Left Bottom Corner Outside
	   if(StX >= 0 || StY < 0)
	   {
		   RValue = FALSE;
		   break;
	   }
	   OrginalX = pCellRgn[nLayer]->ProcSizeX + StX - pCellRgn[nLayer]->OvrXPix;
	   OrginalY = StY;
	   SizeX = -StX;
	   SizeY = DEF_IMG_DISP_SIZEY;
	   DestX = 0;
	   DestY = 0;
	   LoadCADBuf(CellNum - pCellRgn[nLayer]->NodeNumY, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
	   RValue = TRUE;
	   break;
	case 24: 		// Left Bottom of Left Bottom Corner Outside
	   if(StX >= 0 || StY >= 0)
	   {
		   RValue = FALSE;
		   break;
	   }
	   OrginalX = pCellRgn[nLayer]->ProcSizeX + StX - pCellRgn[nLayer]->OvrXPix;
	   OrginalY = 0;
	   SizeX = -StX;
	   SizeY = DEF_IMG_DISP_SIZEY + StY;
	   DestX = 0;
	   DestY = -StY;
	   LoadCADBuf(CellNum - pCellRgn[nLayer]->NodeNumY, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
	   RValue = TRUE;
	   break;
	case 25: 		// Left Bottom of Left Bottom Corner Outside for Strip Mode
	   if(StX >= 0 || StY < 0)
	   {
		   RValue = FALSE;
		   break;
	   }
	   EmpStripThick = pCellRgn[nLayer]->StPosY[CellNum + 1] - pCellRgn[nLayer]->EdPosY[CellNum]; 
	   OrginalX = pCellRgn[nLayer]->ProcSizeX + StX - pCellRgn[nLayer]->OvrXPix;
	   OrginalY = 0;
	   SizeX = -StX;
	   SizeY = DEF_IMG_DISP_SIZEY - EmpStripThick - (pCellRgn[nLayer]->ProcSizeY - StY);
	   DestX = 0;
	   DestY = EmpStripThick + (pCellRgn[nLayer]->ProcSizeY - StY);
	   LoadCADBuf(CellNum - pCellRgn[nLayer]->NodeNumY + 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
	   RValue = TRUE;
	   break;
	case 26: 		// Bottom of Left Bottom Corner Outside for Strip Mode
	   if(StX >= 0 || StY < 0)
	   {
		   RValue = FALSE;
		   break;
	   }
	   EmpStripThick = pCellRgn[nLayer]->StPosY[CellNum + 1] - pCellRgn[nLayer]->EdPosY[CellNum]; 
	   OrginalX = 0;
	   OrginalY = 0;
	   SizeX = DEF_IMG_DISP_SIZEX + StX;
	   SizeY = DEF_IMG_DISP_SIZEY - EmpStripThick - (pCellRgn[nLayer]->ProcSizeY - StY);
	   DestX = -StX;
	   DestY = EmpStripThick + (pCellRgn[nLayer]->ProcSizeY - StY);
	   LoadCADBuf(CellNum + 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
	   RValue = TRUE;
	   break;
	case 27: 		// Bottom Outside for Strip Mode
	   if(StX < 0 || StY < 0)
	   {
		   RValue = FALSE;
		   break;
	   }
	   EmpStripThick = pCellRgn[nLayer]->StPosY[CellNum + 1] - pCellRgn[nLayer]->EdPosY[CellNum]; 
	   OrginalX = StX;
	   OrginalY = 0;
	   SizeX = DEF_IMG_DISP_SIZEX;
	   SizeY = DEF_IMG_DISP_SIZEY - EmpStripThick - (pCellRgn[nLayer]->ProcSizeY - StY);
	   DestX = 0;
	   DestY = EmpStripThick + (pCellRgn[nLayer]->ProcSizeY - StY);
	   LoadCADBuf(CellNum + 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
	   RValue = TRUE;
	   break;
	case 28: 		// Bottom of Right Bottom Corner Outside for Strip Mode
	   if(StX < 0 || StY < 0)
	   {
		   RValue = FALSE;
		   break;
	   }
	   EmpStripThick = pCellRgn[nLayer]->StPosY[CellNum + 1] - pCellRgn[nLayer]->EdPosY[CellNum]; 
	   OrginalX = StX;
	   OrginalY = 0;
	   SizeX = pCellRgn[nLayer]->ProcSizeX - StX;
	   SizeY = DEF_IMG_DISP_SIZEY - EmpStripThick - (pCellRgn[nLayer]->ProcSizeY - StY);
	   DestX = 0;
	   DestY = EmpStripThick + (pCellRgn[nLayer]->ProcSizeY - StY);
	   LoadCADBuf(CellNum + 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
	   RValue = TRUE;
	   break;
	case 29: 		// Right Bottom of Right Bottom Corner Outside for Strip Mode
	   if(StX < 0 || StY < 0)
	   {
		   RValue = FALSE;
		   break;
	   }
	   EmpStripThick = pCellRgn[nLayer]->StPosY[CellNum + 1] - pCellRgn[nLayer]->EdPosY[CellNum]; 
	   OrginalX = pCellRgn[nLayer]->OvrXPix;
	   OrginalY = 0;
	   SizeX = pCellRgn[nLayer]->ProcSizeX - StX;
	   SizeY = DEF_IMG_DISP_SIZEY - EmpStripThick - (pCellRgn[nLayer]->ProcSizeY - StY);
	   DestX = DEF_IMG_DISP_SIZEX - (pCellRgn[nLayer]->ProcSizeX - StX);
	   DestY = EmpStripThick + (pCellRgn[nLayer]->ProcSizeY - StY);
	   LoadCADBuf(CellNum + pCellRgn[nLayer]->NodeNumY + 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
	   RValue = TRUE;
	   break;
	case 30: 		// Left Top of Left Top Corner Outside for Strip Mode
	   if(StX >= 0 || StY >= 0)
	   {
		   RValue = FALSE;
		   break;
	   }
	   EmpStripThick = pCellRgn[nLayer]->StPosY[CellNum] - pCellRgn[nLayer]->EdPosY[CellNum-1]; 
	   OrginalX = pCellRgn[nLayer]->ProcSizeX + StX - pCellRgn[nLayer]->OvrXPix;
	   OrginalY = pCellRgn[nLayer]->ProcSizeY + StY + EmpStripThick;
	   SizeX = -StX;
	   SizeY = -StY - EmpStripThick;
	   DestX = 0;
	   DestY = 0;
	   LoadCADBuf(CellNum - pCellRgn[nLayer]->NodeNumY - 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
	   RValue = TRUE;
	   break;
	case 31: 		// Top of Left Top Corner Outside for Strip Mode
	   if(StX >= 0 || StY >= 0)
	   {
		   RValue = FALSE;
		   break;
	   }
	   EmpStripThick = pCellRgn[nLayer]->StPosY[CellNum] - pCellRgn[nLayer]->EdPosY[CellNum-1]; 
	   OrginalX = 0;
	   OrginalY = pCellRgn[nLayer]->ProcSizeY + StY + EmpStripThick;
	   SizeX = DEF_IMG_DISP_SIZEX + StX;
	   SizeY = -StY - EmpStripThick;
	   DestX = -StX;
	   DestY = 0;
	   LoadCADBuf(CellNum - 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
	   RValue = TRUE;
	   break;
	case 32: 		// Top Outside for Strip Mode
	   if(StX < 0 || StY >= 0)
	   {
		   RValue = FALSE;
		   break;
	   }
	   EmpStripThick = pCellRgn[nLayer]->StPosY[CellNum] - pCellRgn[nLayer]->EdPosY[CellNum-1]; 
	   OrginalX = StX;
	   OrginalY = pCellRgn[nLayer]->ProcSizeY + StY - EmpStripThick;
	   SizeX = DEF_IMG_DISP_SIZEX;
	   SizeY = -StY - EmpStripThick;
	   DestX = 0;
	   DestY = 0;
	   LoadCADBuf(CellNum - 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
	   RValue = TRUE;
	   break;
	case 33: 		// Top of Right Top Corner Outside for Strip Mode
	   if(StX < 0 || StY >= 0)
	   {
		   RValue = FALSE;
		   break;
	   }
	   EmpStripThick = pCellRgn[nLayer]->StPosY[CellNum] - pCellRgn[nLayer]->EdPosY[CellNum-1]; 
	   OrginalX = StX;
	   OrginalY = pCellRgn[nLayer]->ProcSizeY + StY - EmpStripThick;
	   SizeX = pCellRgn[nLayer]->ProcSizeX - StX;
	   SizeY = -StY - EmpStripThick;
	   DestX = 0;
	   DestY = 0;
	   LoadCADBuf(CellNum - 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
	   RValue = TRUE;
	   break;
	case 34: 		// Right Top of Right Top Corner Outside for Strip Mode
	   if(StX < 0 || StY >= 0)
	   {
		   RValue = FALSE;
		   break;
	   }
	   EmpStripThick = pCellRgn[nLayer]->StPosY[CellNum] - pCellRgn[nLayer]->EdPosY[CellNum-1]; 
	   OrginalX = pCellRgn[nLayer]->OvrXPix;
	   OrginalY = pCellRgn[nLayer]->ProcSizeY + StY - EmpStripThick;
	   SizeX = DEF_IMG_DISP_SIZEX - (pCellRgn[nLayer]->ProcSizeX - StX);
	   SizeY = -StY - EmpStripThick;
	   DestX = pCellRgn[nLayer]->ProcSizeX - StX;
	   DestY = 0;
	   LoadCADBuf(CellNum + pCellRgn[nLayer]->NodeNumY - 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
	   RValue = TRUE;
	   break;
	case 35: 		// Left Bottom of Left Bottom Corner Outside for Strip Mode
	   if(StX >= 0 || StY < 0)
	   {
		   RValue = FALSE;
		   break;
	   }
	   EmpStripThick = pCellRgn[nLayer]->EdPosY[CellNum] - pCellRgn[nLayer]->StPosY[CellNum + 1];
	   OrginalX = pCellRgn[nLayer]->ProcSizeX + StX - pCellRgn[nLayer]->OvrXPix;
	   OrginalY = EmpStripThick;
	   SizeX = -StX;
	   SizeY = DEF_IMG_DISP_SIZEY - (pCellRgn[nLayer]->ProcSizeY - StY);
	   DestX = 0;
	   DestY = pCellRgn[nLayer]->ProcSizeY - StY;
	   LoadCADBuf(CellNum - pCellRgn[nLayer]->NodeNumY + 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
	   RValue = TRUE;
	   break;
	case 36: 		// Bottom of Left Bottom Corner Outside for Strip Mode
	   if(StX >= 0 || StY < 0)
	   {
		   RValue = FALSE;
		   break;
	   }
	   EmpStripThick = pCellRgn[nLayer]->EdPosY[CellNum] - pCellRgn[nLayer]->StPosY[CellNum + 1];
	   OrginalX = 0;
	   OrginalY = EmpStripThick;
	   SizeX = DEF_IMG_DISP_SIZEX + StX;
	   SizeY = DEF_IMG_DISP_SIZEY - (pCellRgn[nLayer]->ProcSizeY - StY);
	   DestX = -StX;
	   DestY = pCellRgn[nLayer]->ProcSizeY - StY;
	   LoadCADBuf(CellNum + 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
	   RValue = TRUE;
	   break;
	case 37: 		// Bottom Outside for Strip Mode
	   if(StX < 0 || StY < 0)
	   {
		   RValue = FALSE;
		   break;
	   }
	   EmpStripThick = pCellRgn[nLayer]->EdPosY[CellNum] - pCellRgn[nLayer]->StPosY[CellNum + 1];
	   OrginalX = StX;
	   OrginalY = EmpStripThick;
	   SizeX = DEF_IMG_DISP_SIZEX;
	   SizeY = DEF_IMG_DISP_SIZEY - (pCellRgn[nLayer]->ProcSizeY - StY);
	   DestX = 0;
	   DestY = pCellRgn[nLayer]->ProcSizeY - StY;
	   LoadCADBuf(CellNum + 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
	   RValue = TRUE;
	   break;
	case 38: 		// Bottom of Right Bottom Corner Outside for Strip Mode
	   if(StX < 0 || StY < 0)
	   {
		   RValue = FALSE;
		   break;
	   }
	   EmpStripThick = pCellRgn[nLayer]->EdPosY[CellNum] - pCellRgn[nLayer]->StPosY[CellNum + 1];
	   OrginalX = StX;
	   OrginalY = EmpStripThick;
	   SizeX = pCellRgn[nLayer]->ProcSizeX - StX;
	   SizeY = DEF_IMG_DISP_SIZEY - (pCellRgn[nLayer]->ProcSizeY - StY);
	   DestX = 0;
	   DestY = pCellRgn[nLayer]->ProcSizeY - StY;
	   LoadCADBuf(CellNum + 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
	   RValue = TRUE;
	   break;
	case 39: 		// Right Bottom of Right Bottom Corner Outside for Strip Mode
	   if(StX < 0 || StY < 0)
	   {
		   RValue = FALSE;
		   break;
	   }
	   EmpStripThick = pCellRgn[nLayer]->EdPosY[CellNum] - pCellRgn[nLayer]->StPosY[CellNum + 1];
	   OrginalX = pCellRgn[nLayer]->OvrXPix;
	   OrginalY = EmpStripThick;
	   SizeX = pCellRgn[nLayer]->ProcSizeX - StX;
	   SizeY = DEF_IMG_DISP_SIZEY - (pCellRgn[nLayer]->ProcSizeY - StY);
	   DestX = DEF_IMG_DISP_SIZEX - (pCellRgn[nLayer]->ProcSizeX - StX);
	   DestY = pCellRgn[nLayer]->ProcSizeY - StY;
	   LoadCADBuf(CellNum + pCellRgn[nLayer]->NodeNumY + 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
	   RValue = TRUE;
	   break;
	case 40: 		// Left Top of Left Top Corner Outside for Strip Mode
	   if(StX >= 0 || StY >= 0)
	   {
		   RValue = FALSE;
		   break;
	   }
	   EmpStripThick = pCellRgn[nLayer]->EdPosY[CellNum] - pCellRgn[nLayer]->StPosY[CellNum + 1];
	   if(abs(StY) <= abs(EmpStripThick))
	   {
		   RValue = TRUE;
		   break;
	   }
	   OrginalX = pCellRgn[nLayer]->ProcSizeX + StX - pCellRgn[nLayer]->OvrXPix;
	   OrginalY = pCellRgn[nLayer]->ProcSizeY + StY;
	   SizeX = -StX;
	   SizeY = -StY - EmpStripThick;
	   DestX = 0;
	   DestY = 0;
	   LoadCADBuf(CellNum + pCellRgn[nLayer]->NodeNumY - 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
	   RValue = TRUE;
	   break;
	case 41: 		// Top of Left Top Corner Outside for Strip Mode
	   if(StX >= 0 || StY >= 0)
	   {
		   RValue = FALSE;
		   break;
	   }
	   EmpStripThick = pCellRgn[nLayer]->EdPosY[CellNum] - pCellRgn[nLayer]->StPosY[CellNum + 1];
	   if(abs(StY) <= abs(EmpStripThick))
	   {
		   RValue = TRUE;
		   break;
	   }
	   OrginalX = 0;
	   OrginalY = pCellRgn[nLayer]->ProcSizeY + StY;
	   SizeX = DEF_IMG_DISP_SIZEX + StX;
	   SizeY = -StY - EmpStripThick;
	   DestX = -StX;
	   DestY = 0;
	   LoadCADBuf(CellNum + pCellRgn[nLayer]->NodeNumY - 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
	   RValue = TRUE;
	   break;
	case 42: 		// Top Outside for Strip Mode
	   if(StX < 0 || StY >= 0)
	   {
		   RValue = FALSE;
		   break;
	   }
	   EmpStripThick = pCellRgn[nLayer]->EdPosY[CellNum] - pCellRgn[nLayer]->StPosY[CellNum + 1];
	   if(abs(StY) <= abs(EmpStripThick))
	   {
		   RValue = TRUE;
		   break;
	   }
	   OrginalX = StX;
	   OrginalY = pCellRgn[nLayer]->ProcSizeY + StY;
	   SizeX = DEF_IMG_DISP_SIZEX;
	   SizeY = -StY - EmpStripThick;
	   DestX = 0;
	   DestY = 0;
	   LoadCADBuf(CellNum + pCellRgn[nLayer]->NodeNumY - 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
	   RValue = TRUE;
	   break;
	case 43: 		// Top of Right Top Corner Outside for Strip Mode
	   if(StX < 0 || StY >= 0)
	   {
		   RValue = FALSE;
		   break;
	   }
	   EmpStripThick = pCellRgn[nLayer]->EdPosY[CellNum] - pCellRgn[nLayer]->StPosY[CellNum + 1];
	   if(abs(StY) <= abs(EmpStripThick))
	   {
		   RValue = TRUE;
		   break;
	   }
	   OrginalX = StX;
	   OrginalY = pCellRgn[nLayer]->ProcSizeY + StY;
	   SizeX = pCellRgn[nLayer]->ProcSizeX - StX;
	   SizeY = -StY - EmpStripThick;
	   DestX = 0;
	   DestY = 0;
	   LoadCADBuf(CellNum + pCellRgn[nLayer]->NodeNumY - 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
	   RValue = TRUE;
	   break;
	case 44: 		// Right Top of Right Top Corner Outside for Strip Mode
	   if(StX < 0 || StY >= 0)
	   {
		   RValue = FALSE;
		   break;
	   }
	   EmpStripThick = pCellRgn[nLayer]->EdPosY[CellNum] - pCellRgn[nLayer]->StPosY[CellNum + 1];
	   if(abs(StY) <= abs(EmpStripThick))
	   {
		   RValue = TRUE;
		   break;
	   }
	   OrginalX = pCellRgn[nLayer]->OvrXPix;
	   OrginalY = pCellRgn[nLayer]->ProcSizeY + StY;
	   SizeX = DEF_IMG_DISP_SIZEX - (pCellRgn[nLayer]->ProcSizeX - StX);
	   SizeY = -StY - EmpStripThick;
	   DestX = pCellRgn[nLayer]->ProcSizeX - StX;
	   DestY = 0;
	   LoadCADBuf(CellNum + pCellRgn[nLayer]->NodeNumY - 1, OrginalX, OrginalY, DestX, DestY, SizeX, SizeY, nLayer);
	   RValue = TRUE;
	   break;
	default:
		break;
	}

	return(RValue);
}

void CVision::LoadCADBuf(int CurrCell, long OrgStX, long OrgStY, long DesStX, long DesStY, long SizeX, long SizeY, int nLayer)
{
// 	InitCADBuf();
	MIL_ID MilBufCADCld = M_NULL, MilBufCADTempCld = M_NULL;
	long OrgX, OrgY, DstX, DstY, SzX, SzY;
	TiffData tdat;

#ifdef USE_CAM_MASTER
	REGION_STRIP *pCellRgn[2];
	pCellRgn[nLayer] = pDoc->m_Master[nLayer].m_pCellRgn;

	if (!MilCADImgBuf)
		return;

	if(VicFileLoadFromMem(MilCADImgBuf, pDoc->m_Master[nLayer].m_pCADCellImg[pDoc->m_Master[nLayer].CellInspID[CurrCell]], tdat))
	{
		if((OrgStX + SizeX) <= pCellRgn[nLayer]->ProcSizeX && (OrgStY + SizeY) <= pCellRgn[nLayer]->ProcSizeY)
		{
			if(SizeX < 0)
			{
				OrgX = OrgStX + SizeX;
				SzX = -SizeX;
			}
			else
			{
				OrgX = OrgStX;
				SzX = SizeX;
			}
			if(SizeY < 0)
			{
				OrgY = OrgStY + SizeY;
				SzY = -SizeY;
			}
			else
			{
				OrgY = OrgStY;
				SzY = SizeY;
			}
			MbufChild2d(MilCADImgBuf, OrgX, OrgY, SzX, SzY, &MilBufCADCld);
		}

		if((DesStX + SizeX) <= DEF_IMG_DISP_SIZEX && (DesStY + SizeY) <= DEF_IMG_DISP_SIZEY)
		{
			if(SizeX < 0)
			{
				DstX = DesStX + SizeX;
				SzX = -SizeX;
			}
			else
			{
				DstX = DesStX;
				SzX = SizeX;
			}
			if(SizeY < 0)
			{
				DstY = DesStY + SizeY;
				SzY = -SizeY;
			}
			else
			{
				DstY = DesStY;
				SzY = SizeY;
			}
			MbufChild2d(MilBufCADTemp, DstX, DstY, SzX, SzY, &MilBufCADTempCld);
		}
	}
#else
	if (!MilCADImgBuf)
		return;
	if(VicFileLoadFromMem(MilCADImgBuf, pDoc->m_Master[nLayer].m_pCADCellImg[pDoc->m_Master[nLayer].CellInspID[0]], tdat))
	{
		MbufChild2d(MilCADImgBuf, 0, 0, DEF_IMG_DISP_SIZEX, DEF_IMG_DISP_SIZEY, &MilBufCADCld);
		MbufChild2d(MilBufCADTemp, 0, 0, DEF_IMG_DISP_SIZEX, DEF_IMG_DISP_SIZEY, &MilBufCADTempCld);
	}
#endif


	if(MilBufCADCld != M_NULL && MilBufCADTempCld != M_NULL)
		MbufCopy(MilBufCADCld, MilBufCADTempCld);

	if (MilBufCADTempCld)
	{
		MbufFree(MilBufCADTempCld);
		MilBufCADTempCld = M_NULL;
	}
	
	if (MilBufCADCld)
	{
		MbufFree(MilBufCADCld);
		MilBufCADCld = M_NULL;
	}
}

BOOL CVision::VicGetImgInfo(UCHAR *pCADImg, TiffData &tdat)
{
	int rcode = tiffinfofrombuffer(pCADImg, &tdat);
	if(rcode != NO_ERROR)  // Fill structure 
		return FALSE;
	return TRUE;
}

BOOL CVision::VicFileLoadFromMem(MIL_ID MilImage, UCHAR *pCADImg, TiffData &tdat)
{
	int rcode;
	long PitchByte;
	unsigned char *HostAddr;
	int mVicImgBitCnt;
	unsigned int i, j, k;
// 	TiffData tdat;
	imgdes image;

	rcode = tiffinfofrombuffer(pCADImg, &tdat);
	if(rcode != NO_ERROR) { // Fill structure 
		return FALSE;
	}

	rcode = allocimage(&image, tdat.width, tdat.length,	tdat.vbitcount);
	if(rcode != NO_ERROR)
	{
		return FALSE;
	}

	rcode = loadtiffrombuffer(pCADImg, &image);
	if(rcode != NO_ERROR) // Free image on error
	{
		freeimage(&image);
		return FALSE;
	}

	mVicImgBitCnt = image.bmh->biBitCount;
	HostAddr = (unsigned char*) MbufInquire(MilImage, M_HOST_ADDRESS, M_NULL);
	PitchByte = MbufInquire(MilImage, M_PITCH_BYTE, M_NULL);	//MbufInquire(MilImage, M_PITCH_BYTE, &PitchByte);


	k = tdat.length - 1;
	for(i=0; i<tdat.length; i++)
	{
		for(j=0; j<tdat.width / 8; j++)
		{
			HostAddr[k * PitchByte + j] = BitConvLUT2[image.ibuff[i * image.buffwidth + j]];
		}
		if((tdat.width % 8) != 0)
		{
			HostAddr[k * PitchByte + j] = image.ibuff[i * image.buffwidth + j];
		}
		k--;
	}

	freeimage(&image);
	return TRUE;
}

void CVision::ShowDispDef(int nIdxMkInfo, int nSerial, int nLayer, int nDefPcs) // From 0 To 12...for Screen display.
{
	//char cPath[MAX_PATH];
	TCHAR cPath[MAX_PATH];
	if(m_pMilBufDef[nIdxMkInfo])
	{
		CString sPath;
#ifdef	TEST_MODE
		m_nTest++;
		sPath.Format(_T("%s%05d.tif"),PATH_DEF_IMG, m_nTest);
#else
		if(nLayer==0) // Up
		{
		sPath.Format(_T("%s%s\\%s\\%s\\DefImage\\%d\\%05d.tif"), pDoc->WorkingInfo.System.sPathOldFile, 
																 pDoc->WorkingInfo.LastJob.sModelUp, 
																 pDoc->WorkingInfo.LastJob.sLotUp, 
																 pDoc->WorkingInfo.LastJob.sLayerUp, 
															 nSerial, 
															 nDefPcs);
		}
		else if(nLayer==1) // Dn
		{
			sPath.Format(_T("%s%s\\%s\\%s\\DefImage\\%d\\%05d.tif"), pDoc->WorkingInfo.System.sPathOldFile, 
																 pDoc->WorkingInfo.LastJob.sModelDn, 
																 pDoc->WorkingInfo.LastJob.sLotDn, 
																 pDoc->WorkingInfo.LastJob.sLayerDn, 
																 nSerial, 
																 nDefPcs);
		}
#endif
		CFileFind findfile;
		if(findfile.FindFile(sPath))
		{
			//strcpy(cPath, sPath);
			_stprintf(cPath, _T("%s"), sPath);
			m_pMilBufDef[nIdxMkInfo]->BufferLoad(cPath);
		}
	}
}

void CVision::ClrOvrCad(int nIdxMkInfo)
{
	MbufClear(m_pMilOvrCad[nIdxMkInfo]->m_MilBuffer, (double)(m_pMilDispCad[nIdxMkInfo]->m_lOverlayColor));
// 	m_pMilDelOvrCad[nIdx]->SetTextFont(M_FONT_DEFAULT_SMALL);
// 	m_pMilDelOvrCad[nIdx]->DrawText(3, 3, "     ");
}

void CVision::ShowOvrCad(int nIdxMkInfo, int nSerial)
{
	ClrOvrCad(nIdxMkInfo);
 	m_pMilOvrCad[nIdxMkInfo]->SetTextFont(M_FONT_DEFAULT_SMALL);

	//char szText[30];
	//sprintf(szText, "%d", nSerial);
	TCHAR szText[30];
	_stprintf(szText, _T("%d"), nSerial);
	m_pMilOvrCad[nIdxMkInfo]->DrawText(3, 3, szText);
}

void CVision::ShowDispPin(int nLayer)
{
	LoadPinBuf(nLayer);
	if(MilBufPinTemp)
		MbufCopy(MilBufPinTemp, m_pMilBufPin->m_MilImage);
	DrawCrossOnPin(PIN_IMG_DISP_SIZEX/2, PIN_IMG_DISP_SIZEY/2, 14);
}

void CVision::DrawCrossOnPin(int nCenterX, int nCenterY, int nLineLength)
{
	if(!m_pMilDispPin)	// Overlay
		return;
	
	ClearPinCenterMarkArea(nCenterX, nCenterY, nLineLength);
	
	m_pMilPinOverlay->DrawCross(M_COLOR_RED, 
		nCenterX, 
		nCenterY, 
		nLineLength, 
		nLineLength, 
		0,
		0);
// 	m_pMilPinOverlay->DrawCross(M_COLOR_RED, 
// 		nCenterX+1, 
// 		nCenterY+1, 
// 		nLineLength, 
// 		nLineLength, 
// 		0,
// 		0);
}

// BOOL CVision::ClearPcsCenterMarkArea(int nCenterX, int nCenterY, int nLineLength)
// {
// 	if(!m_pMilDispPcs)
// 		return FALSE;
// 	
// 	if(!m_nPcsCtrX && !m_nPcsCtrY && !m_nPcsCrsLen)
// 	{
// 		m_nPcsCtrX = nCenterX;
// 		m_nPcsCtrY = nCenterY;
// 		m_nPcsCrsLen = nLineLength;
// 	}
// 	
// 	m_pMilPcsOverlayDelete->DrawRectFill(m_pMilDispPcs->m_lOverlayColor, 
// 		m_nPcsCtrX-m_nPcsCrsLen, 
// 		m_nPcsCtrY-m_nPcsCrsLen,
// 		m_nPcsCtrX+m_nPcsCrsLen,
// 		m_nPcsCtrY+m_nPcsCrsLen);
// 	m_nPcsCtrX = nCenterX;
// 	m_nPcsCtrY = nCenterY;
// 	m_nPcsCrsLen = nLineLength;
// 	
// 	return TRUE;
// }

void CVision::InitPinBuf()
{
	if(MilPinImgBuf != M_NULL)
		MbufFree(MilPinImgBuf);
	if(m_pMil)
		MbufAlloc2d(m_pMil->GetSystemID(), 1024, 1024, 1L + M_UNSIGNED, M_IMAGE+M_PROC, &MilPinImgBuf);

	if(MilBufPinTemp != M_NULL)
		MbufFree(MilBufPinTemp);
	if(m_pMil)
		MbufAlloc2d(m_pMil->GetSystemID(), PIN_IMG_DISP_SIZEX, PIN_IMG_DISP_SIZEY, 1L + M_UNSIGNED, M_IMAGE+M_PROC, &MilBufPinTemp);
}

void CVision::LoadPinBuf(int nLayer)
{
	InitPinBuf();

	if (!MilPinImgBuf)
		return;

	TiffData tdat;
	if(VicFileLoadFromMem(MilPinImgBuf, pDoc->m_Master[nLayer].m_pPinImg, tdat))
	{
		MIL_ID MilBufPinCld = M_NULL, MilBufPinTempCld = M_NULL;
// 		MIL_ID  MilOriginDisp = M_NULL;
// 		CGvisMilBuffer *MilPatRtImg=NULL;
// 		CGvisMilBuffer *MilOriginDisp=NULL;
		CLibMilBuf *MilPatRtImg = NULL;
		CLibMilBuf *MilOriginDisp = NULL;

		//MilPatRtImg = new CGvisMilBuffer(m_pMil, PIN_IMG_DISP_SIZEX, PIN_IMG_DISP_SIZEY, 1L+M_UNSIGNED, M_IMAGE+M_PROC);
		if (m_pMil)
			MilPatRtImg = m_pMil->AllocBuf(PIN_IMG_DISP_SIZEX, PIN_IMG_DISP_SIZEY, 1L + M_UNSIGNED, M_IMAGE + M_PROC);
/*
		if(m_nIdx==0)
		{
			MilOriginDisp = new CGvisMilBuffer(m_pMil, PIN_IMG_DISP_SIZEX, PIN_IMG_DISP_SIZEY, 1L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC);
// 			MbufAlloc2d(m_pMil->MilSystem, 1024, 1024, 1L + M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, MilOriginDisp);
//			MbufAlloc2d(m_pMil->MilSystem, PIN_IMG_DISP_SIZEX, PIN_IMG_DISP_SIZEY, 1L + M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &MilBufPinCld);
			MimResize(MilPinImgBuf, MilOriginDisp->m_MilImage, (double)PIN_IMG_DISP_SIZEX/1024.0, (double)PIN_IMG_DISP_SIZEY/1024.0, M_DEFAULT);
			MbufChild2d(MilOriginDisp->m_MilImage, 0, 0, PIN_IMG_DISP_SIZEX, PIN_IMG_DISP_SIZEY, &MilBufPinCld);
		}
		else
			MbufChild2d(MilPinImgBuf, (1024-PIN_IMG_DISP_SIZEX)/2, (1024-PIN_IMG_DISP_SIZEY)/2, PIN_IMG_DISP_SIZEX, PIN_IMG_DISP_SIZEY, &MilBufPinCld);
*/
		//MilOriginDisp = new CGvisMilBuffer(m_pMil, PIN_IMG_DISP_SIZEX, PIN_IMG_DISP_SIZEY, 1L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC);
		if (m_pMil)
			MilOriginDisp = m_pMil->AllocBuf(PIN_IMG_DISP_SIZEX, PIN_IMG_DISP_SIZEY, 1L + M_UNSIGNED, M_IMAGE + M_DISP + M_PROC);
		MimResize(MilPinImgBuf, MilOriginDisp->m_MilImage, (double)PIN_IMG_DISP_SIZEX/1024.0, (double)PIN_IMG_DISP_SIZEY/1024.0, M_DEFAULT);
		MbufChild2d(MilOriginDisp->m_MilImage, 0, 0, PIN_IMG_DISP_SIZEX, PIN_IMG_DISP_SIZEY, &MilBufPinCld);



		MbufChild2d(MilBufPinTemp, 0, 0, PIN_IMG_DISP_SIZEX, PIN_IMG_DISP_SIZEY, &MilBufPinTempCld);

// 		MimRotate(MilBufPinCld, MilPatRtImg->m_MilImage, 90.0, M_DEFAULT, M_DEFAULT, M_DEFAULT, M_DEFAULT, M_DEFAULT);
		MimRotate(MilBufPinCld, MilPatRtImg->m_MilImage, 0.0, M_DEFAULT, M_DEFAULT, M_DEFAULT, M_DEFAULT, M_DEFAULT);
		if(MilPatRtImg->m_MilImage != M_NULL && MilBufPinTempCld != M_NULL)
			MbufCopy(MilPatRtImg->m_MilImage, MilBufPinTempCld);


		if(MilPatRtImg != NULL)
		{
			delete (MilPatRtImg);
			MilPatRtImg = NULL;
		}

		if (MilBufPinTempCld)
		{
			MbufFree(MilBufPinTempCld);
			MilBufPinTempCld = M_NULL;
		}
		
		if (MilBufPinCld)
		{
			MbufFree(MilBufPinCld);
			MilBufPinCld = M_NULL;
		}

		if(MilOriginDisp != M_NULL)
		{
			delete (MilOriginDisp);
			MilOriginDisp = M_NULL;
		}
	}
}

void CVision::ShowDispAlign()
{
	LoadAlignBuf();
	if(MilBufAlignTemp[0])
		MbufCopy(MilBufAlignTemp[0], m_pMilBufAlign->m_MilImage);
}

void CVision::LoadAlignBuf()
{
	InitAlignBuf();

	TiffData tdat;
	MIL_ID MilBufAlignCld = M_NULL, MilBufAlignTempCld = M_NULL;

	if (!MilAlignImgBuf[0])
		return;

	if (pDoc->m_Master[0].m_pAlignImg[0])
	{
		if (VicFileLoadFromMem(MilAlignImgBuf[0], pDoc->m_Master[0].m_pAlignImg[0], tdat))
		{

			MbufChild2d(MilAlignImgBuf[0], (1024 - ALIGN_IMG_DISP_SIZEX) / 2, (1024 - ALIGN_IMG_DISP_SIZEY) / 2, ALIGN_IMG_DISP_SIZEX, ALIGN_IMG_DISP_SIZEY, &MilBufAlignCld);
			MbufChild2d(MilBufAlignTemp[0], 0, 0, ALIGN_IMG_DISP_SIZEX, ALIGN_IMG_DISP_SIZEY, &MilBufAlignTempCld);

			if (MilBufAlignCld != M_NULL && MilBufAlignTempCld != M_NULL)
				MbufCopy(MilBufAlignCld, MilBufAlignTempCld);

			if (MilBufAlignTempCld)
			{
				MbufFree(MilBufAlignTempCld);
				MilBufAlignTempCld = M_NULL;
			}

			if (MilBufAlignCld)
			{
				MbufFree(MilBufAlignCld);
				MilBufAlignCld = M_NULL;
			}
		}
	}

	if (!MilAlignImgBuf[1])
		return;

	if (pDoc->m_Master[0].m_pAlignImg[1])
	{
		if (VicFileLoadFromMem(MilAlignImgBuf[1], pDoc->m_Master[0].m_pAlignImg[1], tdat))
		{

			MbufChild2d(MilAlignImgBuf[1], (1024 - ALIGN_IMG_DISP_SIZEX) / 2, (1024 - ALIGN_IMG_DISP_SIZEY) / 2, ALIGN_IMG_DISP_SIZEX, ALIGN_IMG_DISP_SIZEY, &MilBufAlignCld);
			MbufChild2d(MilBufAlignTemp[1], 0, 0, ALIGN_IMG_DISP_SIZEX, ALIGN_IMG_DISP_SIZEY, &MilBufAlignTempCld);

			if (MilBufAlignCld != M_NULL && MilBufAlignTempCld != M_NULL)
				MbufCopy(MilBufAlignCld, MilBufAlignTempCld);

			if (MilBufAlignTempCld)
			{
				MbufFree(MilBufAlignTempCld);
				MilBufAlignTempCld = M_NULL;
			}

			if (MilBufAlignCld)
			{
				MbufFree(MilBufAlignCld);
				MilBufAlignCld = M_NULL;
			}
		}
	}

	if (!MilAlignImgBuf[2])
		return;

	if (pDoc->m_Master[0].m_pAlignImg[2])
	{
		if (VicFileLoadFromMem(MilAlignImgBuf[2], pDoc->m_Master[0].m_pAlignImg[2], tdat))
		{

			MbufChild2d(MilAlignImgBuf[2], (1024 - ALIGN_IMG_DISP_SIZEX) / 2, (1024 - ALIGN_IMG_DISP_SIZEY) / 2, ALIGN_IMG_DISP_SIZEX, ALIGN_IMG_DISP_SIZEY, &MilBufAlignCld);
			MbufChild2d(MilBufAlignTemp[2], 0, 0, ALIGN_IMG_DISP_SIZEX, ALIGN_IMG_DISP_SIZEY, &MilBufAlignTempCld);

			if (MilBufAlignCld != M_NULL && MilBufAlignTempCld != M_NULL)
				MbufCopy(MilBufAlignCld, MilBufAlignTempCld);

			if (MilBufAlignTempCld)
			{
				MbufFree(MilBufAlignTempCld);
				MilBufAlignTempCld = M_NULL;
			}

			if (MilBufAlignCld)
			{
				MbufFree(MilBufAlignCld);
				MilBufAlignCld = M_NULL;
			}
		}
	}

	if (!MilAlignImgBuf[3])
		return;

	if (pDoc->m_Master[0].m_pAlignImg[3])
	{
		if (VicFileLoadFromMem(MilAlignImgBuf[3], pDoc->m_Master[0].m_pAlignImg[3], tdat))
		{

			MbufChild2d(MilAlignImgBuf[3], (1024 - ALIGN_IMG_DISP_SIZEX) / 2, (1024 - ALIGN_IMG_DISP_SIZEY) / 2, ALIGN_IMG_DISP_SIZEX, ALIGN_IMG_DISP_SIZEY, &MilBufAlignCld);
			MbufChild2d(MilBufAlignTemp[3], 0, 0, ALIGN_IMG_DISP_SIZEX, ALIGN_IMG_DISP_SIZEY, &MilBufAlignTempCld);

			if (MilBufAlignCld != M_NULL && MilBufAlignTempCld != M_NULL)
				MbufCopy(MilBufAlignCld, MilBufAlignTempCld);

			if (MilBufAlignTempCld)
			{
				MbufFree(MilBufAlignTempCld);
				MilBufAlignTempCld = M_NULL;
			}

			if (MilBufAlignCld)
			{
				MbufFree(MilBufAlignCld);
				MilBufAlignCld = M_NULL;
			}
		}
	}
}

// void CVision::LoadAlignBuf()
// {
// 	InitAlignBuf();
// 
// 	TiffData tdat;
// 	if(VicFileLoadFromMem(MilAlignImgBuf, pDoc->m_Master[m_nIdx].m_pAlignImg[m_nIdx], tdat))
// 	{
// 		MIL_ID MilBufAlignCld = M_NULL, MilBufAlignTempCld = M_NULL;
// 
// 		MbufChild2d(MilAlignImgBuf, (1024-ALIGN_IMG_DISP_SIZEX)/2, (1024-ALIGN_IMG_DISP_SIZEY)/2, ALIGN_IMG_DISP_SIZEX, ALIGN_IMG_DISP_SIZEY, &MilBufAlignCld);
// 		MbufChild2d(MilBufAlignTemp, 0, 0, ALIGN_IMG_DISP_SIZEX, ALIGN_IMG_DISP_SIZEY, &MilBufAlignTempCld);
// 
// // 		CGvisMilBuffer *MilPatRtImg=NULL;
// // 		MilPatRtImg = new CGvisMilBuffer(m_pMil, ALIGN_IMG_DISP_SIZEX, ALIGN_IMG_DISP_SIZEY, 1L+M_UNSIGNED, M_IMAGE+M_PROC);
// // 		MimRotate(MilBufAlignCld, MilPatRtImg->m_MilImage, 90.0, M_DEFAULT, M_DEFAULT, M_DEFAULT, M_DEFAULT, M_DEFAULT);
// // 		if(MilPatRtImg->m_MilImage != M_NULL && MilBufAlignTempCld != M_NULL)
// // 			MbufCopy(MilPatRtImg->m_MilImage, MilBufAlignTempCld);
// 
// 		if(MilBufAlignCld != M_NULL && MilBufAlignTempCld != M_NULL)
// 			MbufCopy(MilBufAlignCld, MilBufAlignTempCld);
// 
// 
// // 		if(MilPatRtImg != M_NULL)
// // 		{
// // 			delete (MilPatRtImg);
// // 			MilPatRtImg = M_NULL;
// // 		}
// 
// 		if (MilBufAlignTempCld)
// 		{
// 			MbufFree(MilBufAlignTempCld);
// 			MilBufAlignTempCld = M_NULL;
// 		}
// 		
// 		if (MilBufAlignCld)
// 		{
// 			MbufFree(MilBufAlignCld);
// 			MilBufAlignCld = M_NULL;
// 		}
// 	}
// }


void CVision::ClearOverlay()
{
	if (m_pMil)
		m_pMil->ClearLiveOverlay();
// 	switch(nId)
// 	{
// 	case -1: // Live View...
// 		m_pMilDisplay->ClearOverlay();
// 		break;
// 	}
}

void CVision::DrawCenterMark(int nMode) // 0: Cross, 1: Rect, 2: None
{
	DrawCross();
// 	if(nMode == 0)
// 		DrawCross();
// 	else if(nMode == 1)
// 		DrawRect();
// 	else
// 		DrawNone();
}

void CVision::DrawCross()
{
#ifdef USE_IDS
	if(m_pIds[0])
		DrawCross(m_pIds->m_nSizeX/2, m_pIds->m_nSizeY/2, m_nCenterMarkLineLength);
#endif

#ifdef USE_CREVIS
	if(m_pCrevis[0])
		DrawCross(m_pCrevis[0]->GetImgWidth()/2, m_pCrevis[0]->GetImgHeight()/2, m_nCenterMarkLineLength);
#endif

#ifdef USE_IRAYPLE
	if (m_pIRayple)
		DrawCross(m_pIRayple->GetImgWidth() / 2, m_pIRayple->GetImgHeight() / 2, m_nCenterMarkLineLength);
#endif
}

BOOL CVision::ClearCenterMarkArea()
{
	if(!m_pMil)
		return FALSE;
	
	m_pMil->ClearCenterMarkArea(	m_nDisplayCenterX-m_nCenterMarkLineLength, 
									m_nDisplayCenterY-m_nCenterMarkLineLength,
									m_nDisplayCenterX+m_nCenterMarkLineLength,
									m_nDisplayCenterY+m_nCenterMarkLineLength);
// 	m_pMilDrawOverlayDelete->DrawRectFill(m_pMil->m_lOverlayColor[0], 
// 							m_nDisplayCenterX-m_nCenterMarkLineLength, 
// 							m_nDisplayCenterY-m_nCenterMarkLineLength,
// 							m_nDisplayCenterX+m_nCenterMarkLineLength,
// 							m_nDisplayCenterY+m_nCenterMarkLineLength);
	return TRUE;
}

void CVision::DrawCross(int nCenterX, int nCenterY, int nLineLength)
{
	if(!m_pMil)
		return;

	m_nDisplayCenterX = nCenterX;
	m_nDisplayCenterY = nCenterY;
	m_nCenterMarkLineLength = nLineLength;

	ClearCenterMarkArea();

	m_pMil->DrawCross(	M_COLOR_GREEN, 
						m_nDisplayCenterX, 
						m_nDisplayCenterY, 
						m_nCenterMarkLineLength, 
						m_nCenterMarkLineLength, 
						m_nCenterMarkSpaceLength,
						m_nCenterMarkSpaceLength);
// 	m_pMilDrawOverlay->DrawCross(M_COLOR_GREEN, 
// 						m_nDisplayCenterX, 
// 						m_nDisplayCenterY, 
// 						m_nCenterMarkLineLength, 
// 						m_nCenterMarkLineLength, 
// 						m_nCenterMarkSpaceLength,
// 						m_nCenterMarkSpaceLength);
}



void CVision::DrawText(CString str, long lX, long lY, long lColor)
{
	if(!m_pMil)
		return;

	m_pMil->DrawText(str, lX, lY, lColor);
// 	char szText[MAX_PATH];
// 
// 	strcpy(szText, str);
// 	m_pMilDrawOverlay->SetDrawColor(lColor);
// 	m_pMilDrawOverlay->DrawText(lX, lY, szText);
}

void CVision::SetDispAxisPos()
{
	if(!m_pMil)
		return;

	int nDisplayMargin = 3;
	m_dFontSizePixelX = 8.0;
	m_dFontSizePixelY = 16.0;
	int nDrawAxisPosCharNum = 10;

	m_dFontSizePixelX = m_dFontSizePixelX;
	m_dFontSizePixelY = m_dFontSizePixelY;
//	m_pMilDrawOverlay->SetTextFontScale(1.0, 1.0);
	m_pMil->SetTextFontScale(1.0, 1.0);

#ifdef USE_IDS
	m_ptDisplayAxisPosOffset.x = m_pIds->m_nSizeX - (int)((double)nDrawAxisPosCharNum * m_dFontSizePixelX) - nDisplayMargin;
#endif
#ifdef USE_CREVIS
	m_ptDisplayAxisPosOffset.x = m_pCrevis[0]->GetImgWidth() - (int)((double)nDrawAxisPosCharNum * m_dFontSizePixelX) - nDisplayMargin;
#endif
	m_ptDisplayAxisPosOffset.y = nDisplayMargin;
	m_nDisplayAxisPosLineHeight = (int)m_dFontSizePixelY;
}

void CVision::DispAxisPos(BOOL bForceWrite)
{
	double dFdEnc;

// 	if(!m_bDrawOverlayModeEnable)
// 		return;

	if(!m_pMil)
		return;

// 	if(!m_pMilDisplay || !m_pMilDrawOverlay)
// 		return;

// 	m_pMilDrawOverlay->SetDrawColor(M_COLOR_GREEN);
// 	m_pMilDrawOverlay->SetTextFont(M_FONT_DEFAULT_SMALL);
	m_pMil->SetDrawColor(M_COLOR_GREEN);
	m_pMil->SetTextFont(M_FONT_DEFAULT_SMALL);

	//char szText[30];
	TCHAR szText[30];

	if(m_nIdx==0)
	{
		if(fabs(m_dEnc[AXIS_X0]-pView->m_dEnc[AXIS_X0])>0.005 || bForceWrite)
		{
			m_dEnc[AXIS_X0] = pView->m_dEnc[AXIS_X0];
			//sprintf(szText, "X0:%3.3f", m_dEnc[AXIS_X0]);
			_stprintf(szText, TEXT("X0:%3.3f"), m_dEnc[AXIS_X0]);
	 		//m_pMilDrawOverlay->DrawClear();
			//m_pMilDrawOverlay->DrawText(m_ptDisplayAxisPosOffset.x, m_ptDisplayAxisPosOffset.y+m_nDisplayAxisPosLineHeight*0, szText);
			m_pMil->DrawText(szText, m_ptDisplayAxisPosOffset.x, m_ptDisplayAxisPosOffset.y+m_nDisplayAxisPosLineHeight*0, M_COLOR_GREEN);
		}
		if(fabs(m_dEnc[AXIS_Y0]-pView->m_dEnc[AXIS_Y0])>0.005 || bForceWrite)
		{
			m_dEnc[AXIS_Y0] = pView->m_dEnc[AXIS_Y0];
			//sprintf(szText, "Y0:%3.3f", m_dEnc[AXIS_Y0]);
			_stprintf(szText, TEXT("Y0:%3.3f"), m_dEnc[AXIS_Y0]);
			//m_pMilDrawOverlay->DrawText(m_ptDisplayAxisPosOffset.x, m_ptDisplayAxisPosOffset.y+m_nDisplayAxisPosLineHeight*1, szText);
			m_pMil->DrawText(szText, m_ptDisplayAxisPosOffset.x, m_ptDisplayAxisPosOffset.y+m_nDisplayAxisPosLineHeight*1, M_COLOR_GREEN);
		}
#ifdef USE_MPE
		dFdEnc = (double)pDoc->m_pMpeData[0][0];	// 마킹부 Feeding 엔코더 값(단위 mm )
		if(fabs(m_dFdEnc-dFdEnc)>0.05 || bForceWrite)
		{
			m_dFdEnc = dFdEnc;
			//sprintf(szText, "R:%3.2f", dFdEnc/1000.0); // [M]
			_stprintf(szText, TEXT("Rp:%3.2f"), dFdEnc/1000.0); // [M]
			//m_pMilDrawOverlay->DrawText(m_ptDisplayAxisPosOffset.x, m_ptDisplayAxisPosOffset.y+(m_nDisplayAxisPosLineHeight*2), szText);
			m_pMil->DrawText(szText, m_ptDisplayAxisPosOffset.x, m_ptDisplayAxisPosOffset.y+m_nDisplayAxisPosLineHeight*2, M_COLOR_GREEN);
		}
		dFdEnc = (double)pDoc->m_pMpeData[1][0];	// 각인부 Feeding 엔코더 값(단위 mm)
		if (fabs(m_dFdEnc - dFdEnc) > 0.05 || bForceWrite)
		{
			m_dFdEnc = dFdEnc;
			//sprintf(szText, "R:%3.2f", dFdEnc/1000.0); // [M]
			_stprintf(szText, TEXT("Re:%3.2f"), dFdEnc / 1000.0); // [M]
			//m_pMilDrawOverlay->DrawText(m_ptDisplayAxisPosOffset.x, m_ptDisplayAxisPosOffset.y+(m_nDisplayAxisPosLineHeight*2), szText);
			m_pMil->DrawText(szText, m_ptDisplayAxisPosOffset.x, m_ptDisplayAxisPosOffset.y + m_nDisplayAxisPosLineHeight * 3, M_COLOR_GREEN);
		}
#endif
	}
	else if(m_nIdx==1)
	{
		if(fabs(m_dEnc[AXIS_X1]-pView->m_dEnc[AXIS_X1])>0.005 || bForceWrite)
		{
			m_dEnc[AXIS_X1] = pView->m_dEnc[AXIS_X1];
			//sprintf(szText, "X1:%3.3f", m_dEnc[AXIS_X1]);
			_stprintf(szText, TEXT("X1:%3.3f"), m_dEnc[AXIS_X1]);
 			//m_pMilDrawOverlay->DrawClear();
			//m_pMilDrawOverlay->DrawText(m_ptDisplayAxisPosOffset.x, m_ptDisplayAxisPosOffset.y+m_nDisplayAxisPosLineHeight*0, szText);
			m_pMil->DrawText(szText, m_ptDisplayAxisPosOffset.x, m_ptDisplayAxisPosOffset.y+m_nDisplayAxisPosLineHeight*0, M_COLOR_GREEN);
		}
		if(fabs(m_dEnc[AXIS_Y1]-pView->m_dEnc[AXIS_Y1])>0.005 || bForceWrite)
		{
			m_dEnc[AXIS_Y1] = pView->m_dEnc[AXIS_Y1];
			//sprintf(szText, "Y1:%3.3f", m_dEnc[AXIS_Y1]);
			_stprintf(szText, TEXT("Y1:%3.3f"), m_dEnc[AXIS_Y1]);
			//m_pMilDrawOverlay->DrawText(m_ptDisplayAxisPosOffset.x, m_ptDisplayAxisPosOffset.y+m_nDisplayAxisPosLineHeight*1, szText);
			m_pMil->DrawText(szText, m_ptDisplayAxisPosOffset.x, m_ptDisplayAxisPosOffset.y+m_nDisplayAxisPosLineHeight*1, M_COLOR_GREEN);
		}
#ifdef USE_MPE
		double dBufEnc = (double)pDoc->m_pMpeData[0][1]	/ 1000.0;	// 마킹부 버퍼 엔코더 값(단위 mm * 1000)
		if(fabs(m_dBufEnc-dBufEnc)>0.05 || bForceWrite)
		{
			m_dBufEnc = dBufEnc;
			//sprintf(szText, "B:%3.1f", dBufEnc);
			_stprintf(szText, TEXT("B:%3.1f"), dBufEnc);
			//m_pMilDrawOverlay->DrawText(m_ptDisplayAxisPosOffset.x, m_ptDisplayAxisPosOffset.y+(m_nDisplayAxisPosLineHeight*2), szText);
			m_pMil->DrawText(szText, m_ptDisplayAxisPosOffset.x, m_ptDisplayAxisPosOffset.y+m_nDisplayAxisPosLineHeight*2, M_COLOR_GREEN);
		}
		dFdEnc = (double)pDoc->m_pMpeData[1][1];	// 각인부 Feeding 엔코더 값(단위 mm)
		if (fabs(m_dFdEnc - dFdEnc) > 0.05 || bForceWrite)
		{
			m_dFdEnc = dFdEnc;
			//sprintf(szText, "R:%3.2f", dFdEnc/1000.0); // [M]
			_stprintf(szText, TEXT("Re:%3.2f"), dFdEnc / 1000.0); // [M]
			//m_pMilDrawOverlay->DrawText(m_ptDisplayAxisPosOffset.x, m_ptDisplayAxisPosOffset.y+(m_nDisplayAxisPosLineHeight*2), szText);
			m_pMil->DrawText(szText, m_ptDisplayAxisPosOffset.x, m_ptDisplayAxisPosOffset.y + m_nDisplayAxisPosLineHeight * 3, M_COLOR_GREEN);
		}
#endif
	}
}

double CVision::CalcCameraPixelSize()
{
	double dVal = 1.0;

	if(!m_pMil)
		return dVal;

	int nRepeatMeasureNum = 10, nEffectiveMeasureNum = 6;
	int nRealMeasureNum = 0;
	CfPoint fptMoveDistance;
	fptMoveDistance.x = 1.0; fptMoveDistance.y = 1.0;
	CfPoint fptCameraPos[2];
	fptCameraPos[0].x = 0.0;
	fptCameraPos[0].y = 0.0;
	fptCameraPos[1].x = 0.0;
	fptCameraPos[1].y = 0.0;
	int i = 0;

	// 1. Move
#ifdef USE_IDS
	if(!pView->m_pMotion || !m_pIds)
	{
		dVal = 0.0;
		return dVal;
	}
#endif

#ifdef USE_CREVIS
	if(!pView->m_pMotion || !m_pCrevis)
	{
		dVal = 0.0;
		return dVal;
	}
#endif

#ifdef USE_IRAYPLE
	if (!pView->m_pMotion || !m_pIRayple)
	{
		dVal = 0.0;
		return dVal;
	}
#endif

	double pTgtPos[2], dCurrX, dCurrY;
	pTgtPos[1] = pView->m_pMotion->m_dPinPosY[m_nIdx];
	pTgtPos[0] = pView->m_pMotion->m_dPinPosX[m_nIdx];

	if(m_nIdx==0)
	{
		dCurrX = pView->m_dEnc[AXIS_X0];
		dCurrY = pView->m_dEnc[AXIS_Y0];
		if(dCurrX < -1000.0 || dCurrY < -1000.0)
		{
			//if(!pView->m_pMotion->Move(MS_X0Y0, pTgtPos, 0.3, ABS, WAIT))
			if(!pView->m_pMotion->Move(MS_X0Y0, pTgtPos, 0.3, ABS, WAIT))
				AfxMessageBox(_T("Move XY Error..."));
		}
		else
		{
			double fLen, fVel, fAcc, fJerk;
			fLen = sqrt( ((pTgtPos[0] - dCurrX) * (pTgtPos[0] - dCurrX)) + ((pTgtPos[1] - dCurrY) * (pTgtPos[1] - dCurrY)) );
			if(fLen > 0.001)
			{
				pView->m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X0, fLen, fVel, fAcc, fJerk);
				//if(!pView->m_pMotion->Move(MS_X0Y0, pTgtPos, fVel, fAcc, fAcc))
				if(!pView->m_pMotion->Move(MS_X0Y0, pTgtPos, fVel, fAcc, fAcc))
					AfxMessageBox(_T("Move XY Error..."));
			}
		}
	}
	else if(m_nIdx==1)
	{
		dCurrX = pView->m_dEnc[AXIS_X1];
		dCurrY = pView->m_dEnc[AXIS_Y1];
		if(dCurrX < -1000.0 || dCurrY < -1000.0)
		{
			//if(!pView->m_pMotion->Move(MS_X1Y1, pTgtPos, 0.3, ABS, WAIT))
			if(!pView->m_pMotion->Move(MS_X1Y1, pTgtPos, 0.3, ABS, WAIT))
				AfxMessageBox(_T("Move XY Error..."));
		}
		else
		{
			double fLen, fVel, fAcc, fJerk;
			fLen = sqrt( ((pTgtPos[0] - dCurrX) * (pTgtPos[0] - dCurrX)) + ((pTgtPos[1] - dCurrY) * (pTgtPos[1] - dCurrY)) );
			if(fLen > 0.001)
			{
				pView->m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X1, fLen, fVel, fAcc, fJerk);
				//if(!pView->m_pMotion->Move(MS_X1Y1, pTgtPos, fVel, fAcc, fAcc))
				if(!pView->m_pMotion->Move(MS_X1Y1, pTgtPos, fVel, fAcc, fAcc))
					AfxMessageBox(_T("Move XY Error..."));
			}
		}
	}

	Sleep(500);

	// 2-1. Measure set
	CLibMilBuf *MilGrabImg = NULL;
#ifdef USE_IDS
	MilGrabImg = m_pMil->AllocBuf(m_pIds->m_nSizeX, m_pIds->m_nSizeY, 8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC);
#endif

#ifdef USE_CREVIS
	MilGrabImg = m_pMil->AllocBuf((long)m_pCrevis[0]->GetImgWidth(), (long)m_pCrevis[0]->GetImgHeight(), 8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC);
#endif

#ifdef USE_IRAYPLE
	MilGrabImg = m_pMil->AllocBuf((long)m_pIRayple->GetImgWidth(), (long)m_pIRayple->GetImgHeight(), 8L + M_UNSIGNED, M_IMAGE + M_DISP + M_PROC);
#endif


	// 2-2. Create Model
#ifdef USE_IDS
// 	if(m_pIds->OneshotGrab(MilGrabImg, GRAB_COLOR_GREEN) == FALSE)
	if(m_pIds->OneshotGrab(MilGrabImg->m_MilImage, GRAB_COLOR_COLOR) == FALSE)
	{
		if(MilGrabImg)
			delete MilGrabImg;

		pView->MsgBox(_T("Image Grab Fail !!"));
// 		AfxMessageBox(_T("Image Grab Fail !!"));
		dVal = 0.0;
		return dVal;
	}

	MilGrabImg->ChildBuffer2d(m_pIds->m_nSizeX*3/8, m_pIds->m_nSizeY*3/8, m_pIds->m_nSizeX*2/8, m_pIds->m_nSizeY*2/8);
#endif

#ifdef USE_CREVIS
	StopLive();
	Sleep(100);

	if(m_pMil->OneshotGrab(MilGrabImg->m_MilImage, GRAB_COLOR_COLOR) == FALSE)
	{
		if(MilGrabImg)
			delete MilGrabImg;

		pView->MsgBox(_T("Image Grab Fail !!"));
// 		AfxMessageBox(_T("Image Grab Fail !!"));
		dVal = 0.0;
		return dVal;
	}

	StartLive();
	Sleep(100);

	int nSizeX = m_pCrevis[0]->GetImgWidth();
	int nSizeY = m_pCrevis[0]->GetImgHeight();
	MilGrabImg->ChildBuffer2d(nSizeX*3/8, nSizeY*3/8, nSizeX*2/8, nSizeY*2/8);

#endif

#ifdef USE_IRAYPLE
	// 	if(m_pIds->OneshotGrab(MilGrabImg, GRAB_COLOR_GREEN) == FALSE)
	if (m_pIRayple->OneshotGrab() == FALSE || m_pMil->OneshotGrab(MilGrabImg->m_MilImage, GRAB_COLOR_COLOR) == FALSE)
	{
		if (MilGrabImg)
			delete MilGrabImg;

		pView->MsgBox(_T("Image Grab Fail !!"));
		//AfxMessageBox(_T("Image Grab Fail !!"));
		dVal = 0.0;
		return dVal;
	}

	MilGrabImg->ChildBuffer2d(m_pIRayple->GetImgWidth() * 3 / 8, m_pIRayple->GetImgHeight() * 3 / 8, m_pIRayple->GetImgWidth() * 2 / 8, m_pIRayple->GetImgHeight() * 2 / 8);
#endif

//	m_pMil->PatternMatchingAlloc(MilGrabImg->m_MilImageChild);
	m_pMil->PatternMatchingAlloc(MilGrabImg->m_MilImageChild);
	

	// 2. Measure Position
	nRealMeasureNum = 0;
	for(i=0; i<nRepeatMeasureNum; i++)
	{
		Sleep(100);
#ifdef USE_IDS
//		if(m_pIds->OneshotGrab(MilGrabImg, GRAB_COLOR_GREEN) == FALSE)
		if(m_pIds->OneshotGrab(MilGrabImg->m_MilImage, GRAB_COLOR_COLOR) == FALSE)
		{
			if(MilGrabImg)
				delete MilGrabImg;
// 			m_pMil->GmfFree();
			m_pMil->PatternMatchingFree();

			pView->MsgBox(_T("Image Grab Fail !!"));
// 			AfxMessageBox(_T("Image Grab Fail !!"));
			dVal = 0.0;
			return dVal;
		}
#endif

#ifdef USE_CREVIS
		StopLive();
		Sleep(100);

		if(m_pMil->OneshotGrab(MilGrabImg->m_MilImage, GRAB_COLOR_COLOR) == FALSE)
		{
			if(MilGrabImg)
				delete MilGrabImg;
// 			m_pMil->GmfFree();
			m_pMil->PatternMatchingFree();

			pView->MsgBox(_T("Image Grab Fail !!"));
// 			AfxMessageBox(_T("Image Grab Fail !!"));
			dVal = 0.0;
			return dVal;
		}

		StartLive();
		Sleep(100);
#endif

#ifdef USE_IRAYPLE
		//if(m_pIds->OneshotGrab(MilGrabImg, GRAB_COLOR_GREEN) == FALSE)
		if (m_pIRayple->OneshotGrab() == FALSE || m_pMil->OneshotGrab(MilGrabImg->m_MilImage, GRAB_COLOR_COLOR) == FALSE)
		{
			if (MilGrabImg)
				delete MilGrabImg;
			//m_pMil->GmfFree();
			m_pMil->PatternMatchingFree();

			pView->MsgBox(_T("Image Grab Fail !!"));
			//AfxMessageBox(_T("Image Grab Fail !!"));
			dVal = 0.0;
			return dVal;
		}
#endif

#ifdef _DEBUG
 		// Grab Image Save
		//char szFileName[100];
 		//sprintf(szFileName, "C:\\CalcCameraPixelSize-target0.tif");
 		//MilGrabImg->BufferSave(szFileName);
		MbufSave(_T("C:\\CalcCameraPixelSize-target0.tif"), MilGrabImg->m_MilImage);
#endif

		//m_pMil->GmfFind(MilGrabImg->m_MilImage);
		//m_pMil->PatternMatchingAction(MilGrabImg->m_MilImageChild, m_pMilDrawOverlay->m_MilBuffer, m_pMilDrawOverlay->m_MilGraphicContextID);
 		if(m_pMil->PatternMatchingAction(MilGrabImg->m_MilImage))//, m_pMilDrawOverlay->m_MilBuffer, m_pMilDrawOverlay->m_MilGraphicContextID))
		{
			if(i > (nRepeatMeasureNum-nEffectiveMeasureNum-1))
			{
				fptCameraPos[0].x += m_pMil->m_dPatternMatchingResultSelectPosX;
				fptCameraPos[0].y += m_pMil->m_dPatternMatchingResultSelectPosY;
 				//fptCameraPos[0].x += m_pMil->m_dGmfResultPositionX[m_pMil->m_nGmfResultSelectNum];
 				//fptCameraPos[0].y += m_pMil->m_dGmfResultPositionY[m_pMil->m_nGmfResultSelectNum];

				nRealMeasureNum++;
			}
		}
	}
	if(nRealMeasureNum > 0)
	{
		fptCameraPos[0].x = fptCameraPos[0].x / (double)nRealMeasureNum;
		fptCameraPos[0].y = fptCameraPos[0].y / (double)nRealMeasureNum;
	}
	else
	{
		dVal = 0.0;
		return dVal;
	}


	// 3. Move X,Y 1mm
	if(m_nIdx==0)
	{
		pTgtPos[1] = pView->m_pMotion->m_dPinPosY[m_nIdx] + fptMoveDistance.y;
		pTgtPos[0] = pView->m_pMotion->m_dPinPosX[m_nIdx] + fptMoveDistance.x;
		dCurrX = pView->m_dEnc[AXIS_X0];	// pView->m_pMotion->GetActualPosition(AXIS_X);
		dCurrY = pView->m_dEnc[AXIS_Y0];	// pView->m_pMotion->GetActualPosition(AXIS_Y);
		if(dCurrX < -1000.0 || dCurrY < -1000.0)
		{
			//if(!pView->m_pMotion->Move(MS_X0Y0, pTgtPos, 0.3, ABS, WAIT))
			if(!pView->m_pMotion->Move(MS_X0Y0, pTgtPos, 0.3, ABS, WAIT))
				AfxMessageBox(_T("Move XY Error..."));
		}
		else
		{
			double fLen, fVel, fAcc, fJerk;
			fLen = sqrt( ((pTgtPos[0] - dCurrX) * (pTgtPos[0] - dCurrX)) + ((pTgtPos[1] - dCurrY) * (pTgtPos[1] - dCurrY)) );
			if(fLen > 0.001)
			{
				pView->m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X0, fLen, fVel, fAcc, fJerk);
				//if(!pView->m_pMotion->Move(MS_X0Y0, pTgtPos, fVel/10.0, fAcc/10.0, fAcc/10.0))
				if(!pView->m_pMotion->Move(MS_X0Y0, pTgtPos, fVel/10.0, fAcc/10.0, fAcc/10.0))
					AfxMessageBox(_T("Move XY Error..."));
			}
		}
	}
	else if(m_nIdx==1)
	{
		pTgtPos[1] = pView->m_pMotion->m_dPinPosY[m_nIdx] + fptMoveDistance.y;
		pTgtPos[0] = pView->m_pMotion->m_dPinPosX[m_nIdx] + fptMoveDistance.x;
		dCurrX = pView->m_dEnc[AXIS_X1];	// pView->m_pMotion->GetActualPosition(AXIS_X);
		dCurrY = pView->m_dEnc[AXIS_Y1];	// pView->m_pMotion->GetActualPosition(AXIS_Y);
		if(dCurrX < -1000.0 || dCurrY < -1000.0)
		{
			//if(!pView->m_pMotion->Move(MS_X1Y1, pTgtPos, 0.3, ABS, WAIT))
			if(!pView->m_pMotion->Move(MS_X1Y1, pTgtPos, 0.3, ABS, WAIT))
				AfxMessageBox(_T("Move XY Error..."));
		}
		else
		{
			double fLen, fVel, fAcc, fJerk;
			fLen = sqrt( ((pTgtPos[0] - dCurrX) * (pTgtPos[0] - dCurrX)) + ((pTgtPos[1] - dCurrY) * (pTgtPos[1] - dCurrY)) );
			if(fLen > 0.001)
			{
				pView->m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X1, fLen, fVel, fAcc, fJerk);
				//if(!pView->m_pMotion->Move(MS_X1Y1, pTgtPos, fVel/10.0, fAcc/10.0, fAcc/10.0))
				if(!pView->m_pMotion->Move(MS_X1Y1, pTgtPos, fVel/10.0, fAcc/10.0, fAcc/10.0))
					AfxMessageBox(_T("Move XY Error..."));
			}
		}
	}
	Sleep(500);


	// 4. Measure Position
	nRealMeasureNum = 0;
	for(i=0; i<nRepeatMeasureNum; i++)
	{
		Sleep(100);
#ifdef USE_IDS
//		if(m_pIds->OneshotGrab(MilGrabImg, GRAB_COLOR_GREEN) == FALSE)
		if(m_pIds->OneshotGrab(MilGrabImg->m_MilImage, GRAB_COLOR_COLOR) == FALSE)
		{
			if(MilGrabImg)
				delete MilGrabImg;
			m_pMil->PatternMatchingFree();

			pView->MsgBox(_T("Image Grab Fail !!"));
// 			AfxMessageBox(_T("Image Grab Fail !!"));
			dVal = 0.0;
			return dVal;
		}
#endif

#ifdef USE_CREVIS
		StopLive();
		Sleep(100);

		if(m_pMil->OneshotGrab(MilGrabImg->m_MilImage, GRAB_COLOR_COLOR) == FALSE)
		{
			if(MilGrabImg)
				delete MilGrabImg;
			m_pMil->PatternMatchingFree();

			pView->MsgBox(_T("Image Grab Fail !!"));
// 			AfxMessageBox(_T("Image Grab Fail !!"));
			dVal = 0.0;
			return dVal;
		}

		StartLive();
		Sleep(100);
#endif

#ifdef USE_IRAYPLE
		//if(m_pIds->OneshotGrab(MilGrabImg, GRAB_COLOR_GREEN) == FALSE)
		if (m_pIRayple->OneshotGrab() == FALSE || m_pMil->OneshotGrab(MilGrabImg->m_MilImage, GRAB_COLOR_COLOR) == FALSE)
		{
			if (MilGrabImg)
				delete MilGrabImg;
			m_pMil->PatternMatchingFree();

			pView->MsgBox(_T("Image Grab Fail !!"));
			//AfxMessageBox(_T("Image Grab Fail !!"));
			dVal = 0.0;
			return dVal;
		}
#endif


#ifdef _DEBUG
// 		// Grab Image Save
// 		sprintf(szFileName, "C:\\CalcCameraPixelSize-target1.tif");
// 		MilGrabImg->BufferSave(szFileName);
		MbufSave(_T("C:\\CalcCameraPixelSize-target1.tif"), MilGrabImg->m_MilImage);
#endif

		//m_pMil->GmfFind(MilGrabImg->m_MilImage);
		//m_pMil->PatternMatchingAction(MilGrabImg->m_MilImageChild, m_pMilDrawOverlay->m_MilBuffer, m_pMilDrawOverlay->m_MilGraphicContextID);
 		if(m_pMil->PatternMatchingAction(MilGrabImg->m_MilImage))//, m_pMilDrawOverlay->m_MilBuffer, m_pMilDrawOverlay->m_MilGraphicContextID))
		{
			if(i > (nRepeatMeasureNum-nEffectiveMeasureNum-1))
			{
				fptCameraPos[1].x += m_pMil->m_dPatternMatchingResultSelectPosX;
				fptCameraPos[1].y += m_pMil->m_dPatternMatchingResultSelectPosY;
 				//fptCameraPos[1].x += m_pMil->m_dGmfResultPositionX[m_pMil->m_nGmfResultSelectNum];
 				//fptCameraPos[1].y += m_pMil->m_dGmfResultPositionY[m_pMil->m_nGmfResultSelectNum];
				nRealMeasureNum++;
			}
		}
	}
	if(nRealMeasureNum > 0)
	{
		fptCameraPos[1].x = fptCameraPos[1].x / (double)nRealMeasureNum;
		fptCameraPos[1].y = fptCameraPos[1].y / (double)nRealMeasureNum;
	}
	else
	{
		dVal = 0.0;
		return dVal;
	}

	// 4-2. Measure End
	if(MilGrabImg)
		delete MilGrabImg;
// 	m_pMil->GmfFree();
	m_pMil->PatternMatchingFree();

	// 5. Calc Pixel Size
	double dPixelSizeX = 0.0, dPixelSizeY = 0.0;
#ifdef USE_IDS
	dPixelSizeX = m_pIds->CalcPixelSize(fabs(fptCameraPos[1].x-fptCameraPos[0].x), fptMoveDistance.x);
	dPixelSizeY = m_pIds->CalcPixelSize(fabs(fptCameraPos[1].y-fptCameraPos[0].y), fptMoveDistance.y);
	m_pIds->CalcPixelSize(fabs(fptCameraPos[1].x-fptCameraPos[0].x), fabs(fptCameraPos[1].y-fptCameraPos[0].y), fptMoveDistance.x, fptMoveDistance.y);
#endif

#ifdef USE_CREVIS
	dPixelSizeX = fptMoveDistance.x / fabs(fptCameraPos[1].x-fptCameraPos[0].x);
	dPixelSizeY = fptMoveDistance.y / fabs(fptCameraPos[1].y-fptCameraPos[0].y);
	//m_pIds->CalcPixelSize(fabs(fptCameraPos[1].x-fptCameraPos[0].x), fabs(fptCameraPos[1].y-fptCameraPos[0].y), fptMoveDistance.x, fptMoveDistance.y);
#endif

#ifdef USE_IRAYPLE
	dPixelSizeX = fptMoveDistance.x / fabs(fptCameraPos[1].x-fptCameraPos[0].x);
	dPixelSizeY = fptMoveDistance.y / fabs(fptCameraPos[1].y-fptCameraPos[0].y);
	//m_pIds->CalcPixelSize(fabs(fptCameraPos[1].x-fptCameraPos[0].x), fabs(fptCameraPos[1].y-fptCameraPos[0].y), fptMoveDistance.x, fptMoveDistance.y);
#endif

	// 6. Save Cam Resolution
	CString sItem, sData, sPath=PATH_WORKING_INFO;

	sItem.Format(_T("Vision%d"), m_nIdx);
	sData.Format(_T("%f"), dPixelSizeX);
	pDoc->WorkingInfo.Vision[m_nIdx].sResX = sData;
	::WritePrivateProfileString(sItem, _T("RESOLUTION_X"), sData, sPath);	
	sData.Format(_T("%f"), dPixelSizeY);
	pDoc->WorkingInfo.Vision[m_nIdx].sResY = sData;
	::WritePrivateProfileString(sItem, _T("RESOLUTION_Y"), sData, sPath);

// #ifdef USE_IDS
// 	if(m_pMeasure)
// 		m_pMeasure->SetPixelSize(m_pIds->m_dPixelSizeX, m_pIds->m_dPixelSizeY);
// #ifdef _DEBUG
// 	TRACE(_T("\n Pixel Size: %.6f,  X: %.6f, Y: %.6f"),m_pIds->m_dPixelSize, dPixelSizeX, dPixelSizeY);
// #endif
// 
// #endif

	pView->ClrDispMsg();

	SetClrOverlay();

	dVal = (dPixelSizeX+dPixelSizeY)/2.0;
	return dVal;
}

BOOL CVision::GrabIRayple(int nPos, BOOL bDraw)
{
	int i = 0, nRepeatMeasureNum = 1, nEffectiveMeasureNum = 1;
	double dAngle = 0.0, dScore = 0.0;
	double dPosX, dPosY;
	dPosX = 0.0;
	dPosY = 0.0;

#ifdef USE_IRAYPLE
	if (!m_pIRayple)
		return FALSE;

#ifdef USE_MIL
	int nSizeX = m_pIRayple->GetImgWidth();
	int nSizeY = m_pIRayple->GetImgHeight();
	CLibMilBuf *MilGrabImg = NULL, *MilGrabBinImg = NULL, *MilPatImg = NULL, *MilPatRzImg = NULL;//, *MilPatRtImg=NULL;
	MilGrabImg = m_pMil->AllocBuf(nSizeX, nSizeY, 8L + M_UNSIGNED, M_IMAGE + M_DISP + M_PROC);
	MilGrabBinImg = m_pMil->AllocBuf(nSizeX, nSizeY, 8L + M_UNSIGNED, M_IMAGE + M_DISP + M_PROC);

	DWORD dwSt = GetTickCount();
	double dRsRto = (_tstof(pDoc->WorkingInfo.Vision[m_nIdx].sCamPxlRes) / 10000.0) / _tstof(pDoc->WorkingInfo.Vision[m_nIdx].sResX);
	long lSzX = (long)((double)ALIGN_IMG_DISP_SIZEX * dRsRto);
	long lSzY = (long)((double)ALIGN_IMG_DISP_SIZEY * dRsRto);
	MilPatImg = m_pMil->AllocBuf(ALIGN_IMG_DISP_SIZEX, ALIGN_IMG_DISP_SIZEY, 8L + M_UNSIGNED, M_IMAGE + M_DISP + M_PROC);
	MilPatRzImg = m_pMil->AllocBuf(lSzX, lSzY, 8L + M_UNSIGNED, M_IMAGE + M_DISP + M_PROC);
	MimBinarize(MilBufAlignTemp[nPos], MilPatImg->m_MilImage, M_GREATER, 0, 0);
	MimResize(MilPatImg->m_MilImage, MilPatRzImg->m_MilImage, dRsRto, dRsRto, M_DEFAULT);

	dScore = _tstof(pDoc->WorkingInfo.Vision[m_nIdx].sStdScr);
	if (dScore < 10.0)
		dScore = 10.0;

	m_pMil->PatternMatchingAlloc(MilPatRzImg->m_MilImage, dScore);

	CString str;
	str.Format(_T("%d [mSec]"), GetTickCount() - dwSt);

	/////// 	pView->DispStsBar(str, 0);

	#ifdef _DEBUG
	//char szFileName[100];
	//sprintf(szFileName, "C:\\PinPat.tif");
	TCHAR szFileName[30];
	_stprintf(szFileName, _T("C:\\PinPat.tif"));
	MbufSave(szFileName, MilPatRzImg->m_MilImage);
	//MilPatRzImg->BufferSave(szFileName);
	//MilPatRtImg->BufferSave(szFileName);
	#endif

	int nRealMeasureNum;

	// Measure Position
	dScore = 0.0;
	nRealMeasureNum = 0;
	for (i = 0; i < nRepeatMeasureNum; i++)
	{
		//Sleep(30);
		//StopLive();
		//Sleep(100);
		if(m_pIRayple->OneshotGrab())
		{ 
			if (m_pMil->OneshotGrab(MilGrabImg->m_MilImage, GRAB_COLOR_COLOR) == FALSE)
			{
				m_pMil->PatternMatchingFree();

				if (bDraw)
					pView->MsgBox(_T("Image Grab Fail !!"));
				SetClrOverlay();

				if (MilGrabImg)
					delete MilGrabImg;
				if (MilGrabBinImg)
					delete MilGrabBinImg;
				if (MilPatImg)
					delete MilPatImg;
				if (MilPatRzImg)
					delete MilPatRzImg;

				//StartLive();
				//Sleep(30);

				return FALSE;
			}
		}

		//StartLive();
		//Sleep(30);


		MimBinarize(MilGrabImg->m_MilImage, MilGrabBinImg->m_MilImage, M_GREATER, 128, 0);

		MimConvolve(MilGrabImg->m_MilImage, MilGrabImg->m_MilImage, M_SMOOTH);
		MimConvolve(MilGrabImg->m_MilImage, MilGrabImg->m_MilImage, M_SMOOTH);
		MimConvolve(MilGrabImg->m_MilImage, MilGrabImg->m_MilImage, M_SMOOTH);
		MimConvolve(MilGrabImg->m_MilImage, MilGrabImg->m_MilImage, M_SMOOTH);
		MimConvolve(MilGrabImg->m_MilImage, MilGrabImg->m_MilImage, M_SMOOTH);
		MimConvolve(MilGrabImg->m_MilImage, MilGrabImg->m_MilImage, M_SMOOTH);
		MimConvolve(MilGrabImg->m_MilImage, MilGrabImg->m_MilImage, M_SMOOTH);

	#ifdef _DEBUG
		//sprintf(szFileName, "C:\\PinGrab.tif");
		//sprintf(szFileName, "C:\\AlignGrab0.tif");
		_stprintf(szFileName, _T("C:\\AlignGrab0.tif"));
		MbufSave(szFileName, MilGrabImg->m_MilImage);

		//sprintf(szFileName, "C:\\PinGrabBin.tif");
		//sprintf(szFileName, "C:\\AlignGrabBin0.tif");
		_stprintf(szFileName, _T("C:\\AlignGrabBin0.tif"));
		MbufSave(szFileName, MilGrabBinImg->m_MilImage);
	#endif

		if (m_pMil->PatternMatchingAction(MilGrabBinImg->m_MilImage, bDraw))//, m_pMilDrawOverlay->m_MilBuffer, m_pMilDrawOverlay->m_MilGraphicContextID, bDraw))
		{
			if (nRealMeasureNum < nEffectiveMeasureNum)
			{
				dPosX += m_pMil->m_dPatternMatchingResultSelectPosX;
				dPosY += m_pMil->m_dPatternMatchingResultSelectPosY;
				dAngle += m_pMil->m_dPatternMatchingResultSelectAngle;
				dScore += m_pMil->m_dPatternMatchingResultSelectScore;
				nRealMeasureNum++;
			}
			else
				break;
		}
	}

	if (dScore >= _tstof(pDoc->WorkingInfo.Vision[m_nIdx].sStdScr) && nRealMeasureNum > 0)
	{
		PtMtRst.dX = dPosX / (double)nRealMeasureNum;
		PtMtRst.dY = dPosY / (double)nRealMeasureNum;
		PtMtRst.dAngle = dAngle / (double)nRealMeasureNum;
		PtMtRst.dScore = dScore / (double)nRealMeasureNum;
	}
	else
	{
		//char szText[100];
		//sprintf(szText, _T("Pattern Matching Fail !"));
		CString sMsg = _T("Pattern Matching Fail !");

		long lImageSizeX, lImageSizeY;
		lImageSizeX = MbufInquire(MilPatRzImg->m_MilImage, M_SIZE_X, M_NULL);		//MbufInquire(MilPatRzImg->m_MilImage,M_SIZE_X,&lImageSizeX);
		lImageSizeY = MbufInquire(MilPatRzImg->m_MilImage, M_SIZE_Y, M_NULL);		//MbufInquire(MilPatRzImg->m_MilImage,M_SIZE_Y,&lImageSizeY);
																					//MgraText(m_pMilDrawOverlay->m_MilGraphicContextID, m_pMilDrawOverlay->m_MilBuffer, lImageSizeX/2, lImageSizeY/2, szText);
		m_pMil->DrawText(sMsg, lImageSizeX / 2, lImageSizeY / 2, RGB_RED);
		m_pMil->PatternMatchingFree();

		if (MilGrabImg)
			delete MilGrabImg;
		if (MilGrabBinImg)
			delete MilGrabBinImg;
		if (MilPatImg)
			delete MilPatImg;
		if (MilPatRzImg)
			delete MilPatRzImg;

		SetClrOverlay();

		//StartLive();
		//Sleep(30);

		return FALSE;
	}

	m_pMil->PatternMatchingFree();

	if (MilGrabImg)
		delete MilGrabImg;
	if (MilGrabBinImg)
		delete MilGrabBinImg;
	if (MilPatImg)
		delete MilPatImg;
	if (MilPatRzImg)
		delete MilPatRzImg;

	SetClrOverlay();
#endif // USE_MIL
#endif

	return TRUE;
}

BOOL CVision::GrabCrevis(int nPos, BOOL bDraw)
{
	int i = 0, nRepeatMeasureNum = 1, nEffectiveMeasureNum = 1;
	double dAngle = 0.0, dScore = 0.0;
	double dPosX, dPosY;
	dPosX = 0.0;
	dPosY = 0.0;

#ifdef USE_CREVIS
	if (!m_pCrevis)
		return FALSE;
#ifdef USE_MIL
	int nSizeX = m_pCrevis[0]->GetImgWidth();
	int nSizeY = m_pCrevis[0]->GetImgHeight();
	CLibMilBuf *MilGrabImg = NULL, *MilGrabBinImg = NULL, *MilPatImg = NULL, *MilPatRzImg = NULL;//, *MilPatRtImg=NULL;
	MilGrabImg = m_pMil->AllocBuf(nSizeX, nSizeY, 8L + M_UNSIGNED, M_IMAGE + M_DISP + M_PROC);
	MilGrabBinImg = m_pMil->AllocBuf(nSizeX, nSizeY, 8L + M_UNSIGNED, M_IMAGE + M_DISP + M_PROC);

	DWORD dwSt = GetTickCount();
	double dRsRto = (_tstof(pDoc->WorkingInfo.Vision[m_nIdx].sCamPxlRes) / 10000.0) / _tstof(pDoc->WorkingInfo.Vision[m_nIdx].sResX);
	long lSzX = (long)((double)ALIGN_IMG_DISP_SIZEX * dRsRto);
	long lSzY = (long)((double)ALIGN_IMG_DISP_SIZEY * dRsRto);
	MilPatImg = m_pMil->AllocBuf(ALIGN_IMG_DISP_SIZEX, ALIGN_IMG_DISP_SIZEY, 8L + M_UNSIGNED, M_IMAGE + M_DISP + M_PROC);
	MilPatRzImg = m_pMil->AllocBuf(lSzX, lSzY, 8L + M_UNSIGNED, M_IMAGE + M_DISP + M_PROC);
	MimBinarize(MilBufAlignTemp[nPos], MilPatImg->m_MilImage, M_GREATER, 0, 0);
	MimResize(MilPatImg->m_MilImage, MilPatRzImg->m_MilImage, dRsRto, dRsRto, M_DEFAULT);

	dScore = _tstof(pDoc->WorkingInfo.Vision[m_nIdx].sStdScr);
	if (dScore < 10.0)
		dScore = 10.0;

	m_pMil->PatternMatchingAlloc(MilPatRzImg->m_MilImage, dScore);

	CString str;
	str.Format(_T("%d [mSec]"), GetTickCount() - dwSt);

	/////// 	pView->DispStsBar(str, 0);

#ifdef _DEBUG
	char szFileName[100];
	sprintf(szFileName, "C:\\PinPat.tif");
	MbufSave(szFileName, MilPatRzImg->m_MilImage);
	//MilPatRzImg->BufferSave(szFileName);
	//MilPatRtImg->BufferSave(szFileName);
#endif

	int nRealMeasureNum;

	// Measure Position
	dScore = 0.0;
	nRealMeasureNum = 0;
	for (i = 0; i < nRepeatMeasureNum; i++)
	{
		// 		Sleep(30);
		// 		StopLive();
		// 		Sleep(100);

		if (m_pMil->OneshotGrab(MilGrabImg->m_MilImage, GRAB_COLOR_COLOR) == FALSE)
		{
			m_pMil->PatternMatchingFree();

			if (bDraw)
				pView->MsgBox(_T("Image Grab Fail !!"));
			SetClrOverlay();

			if (MilGrabImg)
				delete MilGrabImg;
			if (MilGrabBinImg)
				delete MilGrabBinImg;
			if (MilPatImg)
				delete MilPatImg;
			if (MilPatRzImg)
				delete MilPatRzImg;

			// 			StartLive();
			// 			Sleep(30);

			return FALSE;
		}

		// 		StartLive();
		// 		Sleep(30);


		MimBinarize(MilGrabImg->m_MilImage, MilGrabBinImg->m_MilImage, M_GREATER, 128, 0);

		MimConvolve(MilGrabImg->m_MilImage, MilGrabImg->m_MilImage, M_SMOOTH);
		MimConvolve(MilGrabImg->m_MilImage, MilGrabImg->m_MilImage, M_SMOOTH);
		MimConvolve(MilGrabImg->m_MilImage, MilGrabImg->m_MilImage, M_SMOOTH);
		MimConvolve(MilGrabImg->m_MilImage, MilGrabImg->m_MilImage, M_SMOOTH);
		MimConvolve(MilGrabImg->m_MilImage, MilGrabImg->m_MilImage, M_SMOOTH);
		MimConvolve(MilGrabImg->m_MilImage, MilGrabImg->m_MilImage, M_SMOOTH);
		MimConvolve(MilGrabImg->m_MilImage, MilGrabImg->m_MilImage, M_SMOOTH);

#ifdef _DEBUG
		//sprintf(szFileName, "C:\\PinGrab.tif");
		sprintf(szFileName, "C:\\AlignGrab0.tif");
		MbufSave(szFileName, MilGrabImg->m_MilImage);

		//sprintf(szFileName, "C:\\PinGrabBin.tif");
		sprintf(szFileName, "C:\\AlignGrabBin0.tif");
		MbufSave(szFileName, MilGrabBinImg->m_MilImage);
#endif

		if (m_pMil->PatternMatchingAction(MilGrabBinImg->m_MilImage, bDraw))//, m_pMilDrawOverlay->m_MilBuffer, m_pMilDrawOverlay->m_MilGraphicContextID, bDraw))
		{
			if (nRealMeasureNum < nEffectiveMeasureNum)
			{
				dPosX += m_pMil->m_dPatternMatchingResultSelectPosX;
				dPosY += m_pMil->m_dPatternMatchingResultSelectPosY;
				dAngle += m_pMil->m_dPatternMatchingResultSelectAngle;
				dScore += m_pMil->m_dPatternMatchingResultSelectScore;
				nRealMeasureNum++;
			}
			else
				break;
		}
	}

	if (dScore >= _tstof(pDoc->WorkingInfo.Vision[m_nIdx].sStdScr) && nRealMeasureNum > 0)
	{
		PtMtRst.dX = dPosX / (double)nRealMeasureNum;
		PtMtRst.dY = dPosY / (double)nRealMeasureNum;
		PtMtRst.dAngle = dAngle / (double)nRealMeasureNum;
		PtMtRst.dScore = dScore / (double)nRealMeasureNum;
	}
	else
	{
		// 		char szText[100];
		// 		sprintf(szText, _T("Pattern Matching Fail !"));
		CString sMsg = _T("Pattern Matching Fail !");

		long lImageSizeX, lImageSizeY;
		lImageSizeX = MbufInquire(MilPatRzImg->m_MilImage, M_SIZE_X, M_NULL);		//MbufInquire(MilPatRzImg->m_MilImage,M_SIZE_X,&lImageSizeX);
		lImageSizeY = MbufInquire(MilPatRzImg->m_MilImage, M_SIZE_Y, M_NULL);		//MbufInquire(MilPatRzImg->m_MilImage,M_SIZE_Y,&lImageSizeY);


																					//MgraText(m_pMilDrawOverlay->m_MilGraphicContextID, m_pMilDrawOverlay->m_MilBuffer, lImageSizeX/2, lImageSizeY/2, szText);
		m_pMil->DrawText(sMsg, lImageSizeX / 2, lImageSizeY / 2, RGB_RED);


		m_pMil->PatternMatchingFree();

		if (MilGrabImg)
			delete MilGrabImg;
		if (MilGrabBinImg)
			delete MilGrabBinImg;
		if (MilPatImg)
			delete MilPatImg;
		if (MilPatRzImg)
			delete MilPatRzImg;

		SetClrOverlay();

		// 		StartLive();
		// 		Sleep(30);

		return FALSE;
	}

	m_pMil->PatternMatchingFree();

	if (MilGrabImg)
		delete MilGrabImg;
	if (MilGrabBinImg)
		delete MilGrabBinImg;
	if (MilPatImg)
		delete MilPatImg;
	if (MilPatRzImg)
		delete MilPatRzImg;

	SetClrOverlay();

	// 	StartLive();
	// 	Sleep(30);
#endif // USE_MIL
#endif

	return TRUE;
}

BOOL CVision::GrabIds(int nPos, BOOL bDraw)
{
	int i = 0, nRepeatMeasureNum = 1, nEffectiveMeasureNum = 1;
	double dAngle = 0.0, dScore = 0.0;
	double dPosX, dPosY;
	dPosX = 0.0;
	dPosY = 0.0;

#ifdef USE_IDS
	if (!m_pIds)
		return FALSE;

#ifdef USE_MIL
	CLibMilBuf *MilGrabImg = NULL, *MilGrabBinImg = NULL, *MilPatImg = NULL, *MilPatRzImg = NULL;//, *MilPatRtImg=NULL;
	MilGrabImg = m_pMil->AllocBuf(m_pIds->m_nSizeX, m_pIds->m_nSizeY, 8L + M_UNSIGNED, M_IMAGE + M_DISP + M_PROC);
	MilGrabBinImg = m_pMil->AllocBuf(m_pIds->m_nSizeX, m_pIds->m_nSizeY, 8L + M_UNSIGNED, M_IMAGE + M_DISP + M_PROC);

	DWORD dwSt = GetTickCount();
	double dRsRto = (_tstof(pDoc->WorkingInfo.Vision[m_nIdx].sCamPxlRes) / 10000.0) / _tstof(pDoc->WorkingInfo.Vision[m_nIdx].sResX);
	long lSzX = (long)((double)ALIGN_IMG_DISP_SIZEX * dRsRto);
	long lSzY = (long)((double)ALIGN_IMG_DISP_SIZEY * dRsRto);
	// 	long lSzX = (long)((double)PIN_IMG_DISP_SIZEX * dRsRto);
	// 	long lSzY = (long)((double)PIN_IMG_DISP_SIZEY * dRsRto);
	// 	MilPatImg = new CGvisMilBuffer(m_pMil, PIN_IMG_DISP_SIZEX, PIN_IMG_DISP_SIZEY, 8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC);
	MilPatImg = m_pMil->AllocBuf(m_pMil, ALIGN_IMG_DISP_SIZEX, ALIGN_IMG_DISP_SIZEY, 8L + M_UNSIGNED, M_IMAGE + M_DISP + M_PROC);
	MilPatRzImg = m_pMil->AllocBuf(m_pMil, lSzX, lSzY, 8L + M_UNSIGNED, M_IMAGE + M_DISP + M_PROC);
	// 	MilPatRtImg = new CGvisMilBuffer(m_pMil, lSzX, lSzY, 8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC);
	MimBinarize(MilBufAlignTemp[nPos], MilPatImg->m_MilImage, M_GREATER, 0, 0);
	// 	MimBinarize(MilBufPinTemp, MilPatImg->m_MilImage, M_GREATER, 0, 0);
	MimResize(MilPatImg->m_MilImage, MilPatRzImg->m_MilImage, dRsRto, dRsRto, M_DEFAULT);
	// 	MimRotate(MilPatRzImg->m_MilImage, MilPatRtImg->m_MilImage, 90.0, M_DEFAULT, M_DEFAULT, M_DEFAULT, M_DEFAULT, M_DEFAULT);
	// 	m_pMil->PatternMatchingAlloc(MilPatRtImg->m_MilImage);

	dScore = _tstof(pDoc->WorkingInfo.Vision[m_nIdx].sStdScr);
	if (dScore < 10.0)
		dScore = 10.0;

	m_pMil->PatternMatchingAlloc(MilPatRzImg->m_MilImage, dScore);

	CString str;
	str.Format(_T("%d [mSec]"), GetTickCount() - dwSt);
	/////// 	pView->DispStsBar(str, 0);

	//#ifdef _DEBUG
	// 	char szFileName[100];
	// 	sprintf(szFileName, "C:\\PinPat.tif");
	// 	MilPatRzImg->BufferSave(szFileName);
	// // 	MilPatRtImg->BufferSave(szFileName);
	//#endif

	int nRealMeasureNum;

	// Measure Position
	nRealMeasureNum = 0;
	for (i = 0; i < nRepeatMeasureNum; i++)
	{
		Sleep(30);
		m_pIds->IDSStopLive();
		Sleep(100);

		if (m_pIds->OneshotGrab(MilGrabImg, GRAB_COLOR_GREEN, FALSE) == FALSE)
		{
			m_pMil->PatternMatchingFree();

			if (bDraw)
				pView->MsgBox(_T("Image Grab Fail !!"));
			SetClrOverlay();

			if (MilGrabImg)
				delete MilGrabImg;
			if (MilGrabBinImg)
				delete MilGrabBinImg;
			if (MilPatImg)
				delete MilPatImg;
			if (MilPatRzImg)
				delete MilPatRzImg;

			m_pIds->IDSStartLive();
			Sleep(30);

			return FALSE;
		}

		Sleep(30);


		MimBinarize(MilGrabImg->m_MilImage, MilGrabBinImg->m_MilImage, M_GREATER, 128, 0);

		MimConvolve(MilGrabImg->m_MilImage, MilGrabImg->m_MilImage, M_SMOOTH);
		MimConvolve(MilGrabImg->m_MilImage, MilGrabImg->m_MilImage, M_SMOOTH);
		MimConvolve(MilGrabImg->m_MilImage, MilGrabImg->m_MilImage, M_SMOOTH);
		MimConvolve(MilGrabImg->m_MilImage, MilGrabImg->m_MilImage, M_SMOOTH);
		MimConvolve(MilGrabImg->m_MilImage, MilGrabImg->m_MilImage, M_SMOOTH);
		MimConvolve(MilGrabImg->m_MilImage, MilGrabImg->m_MilImage, M_SMOOTH);
		MimConvolve(MilGrabImg->m_MilImage, MilGrabImg->m_MilImage, M_SMOOTH);

		//#ifdef _DEBUG
		//  	//sprintf(szFileName, "C:\\PinGrab.tif");
		// 		sprintf(szFileName, "C:\\AlignGrab0.tif");
		// 		MbufSave(szFileName, MilGrabImg->m_MilImage);
		// 
		// 		//sprintf(szFileName, "C:\\PinGrabBin.tif");
		// 		sprintf(szFileName, "C:\\AlignGrabBin0.tif");
		// 		MbufSave(szFileName, MilGrabBinImg->m_MilImage);
		//#endif

		if (m_pMil->PatternMatchingAction(MilGrabBinImg->m_MilImage, bDraw))//, m_pMilDrawOverlay->m_MilBuffer, m_pMilDrawOverlay->m_MilGraphicContextID, bDraw))
		{
			if (nRealMeasureNum < nEffectiveMeasureNum)
			{
				dPosX += m_pMil->m_dPatternMatchingResultSelectPosX;
				dPosY += m_pMil->m_dPatternMatchingResultSelectPosY;
				dAngle += m_pMil->m_dPatternMatchingResultSelectAngle;
				dScore += m_pMil->m_dPatternMatchingResultSelectScore;
				nRealMeasureNum++;
			}
			else
				break;
		}
	}

	if (dScore >= _tstof(pDoc->WorkingInfo.Vision[m_nIdx].sStdScr) && nRealMeasureNum > 0)
	{
		PtMtRst.dX = dPosX / (double)nRealMeasureNum;
		PtMtRst.dY = dPosY / (double)nRealMeasureNum;
		PtMtRst.dAngle = dAngle / (double)nRealMeasureNum;
		PtMtRst.dScore = dScore / (double)nRealMeasureNum;
	}
	else
	{
		// 		char szText[100];
		// 		sprintf(szText, _T("Pattern Matching Fail !"));
		CString sMsg = _T("Pattern Matching Fail !");
		long lImageSizeX, lImageSizeY;
		lImageSizeX = MbufInquire(MilPatRzImg->m_MilImage, M_SIZE_X, M_NULL);		//MbufInquire(MilPatRzImg->m_MilImage,M_SIZE_X,&lImageSizeX);
		lImageSizeY = MbufInquire(MilPatRzImg->m_MilImage, M_SIZE_Y, M_NULL);		//MbufInquire(MilPatRzImg->m_MilImage,M_SIZE_Y,&lImageSizeY);


																					//MgraText(m_pMilDrawOverlay->m_MilGraphicContextID, m_pMilDrawOverlay->m_MilBuffer, lImageSizeX/2, lImageSizeY/2, szText);
		m_pMil->DrawText(sMsg, lImageSizeX / 2, lImageSizeY / 2, RGB_RED);


		m_pMil->PatternMatchingFree();

		if (MilGrabImg)
			delete MilGrabImg;
		if (MilGrabBinImg)
			delete MilGrabBinImg;
		if (MilPatImg)
			delete MilPatImg;
		if (MilPatRzImg)
			delete MilPatRzImg;
		// 		if(MilPatRtImg)
		// 			delete MilPatRtImg;

		SetClrOverlay();

		m_pIds->IDSStartLive();
		Sleep(30);

		return FALSE;
	}

	m_pMil->PatternMatchingFree();

	if (MilGrabImg)
		delete MilGrabImg;
	if (MilGrabBinImg)
		delete MilGrabBinImg;
	if (MilPatImg)
		delete MilPatImg;
	if (MilPatRzImg)
		delete MilPatRzImg;
	// 	if(MilPatRtImg)
	// 		delete MilPatRtImg;

	SetClrOverlay();

	m_pIds->IDSStartLive();
	Sleep(30);
#endif // USE_MIL
#endif	
	
	return TRUE;
}

BOOL CVision::Grab(int nPos, BOOL bDraw)
{
	if(!m_pMil)
		return FALSE;

// 	int i=0, nRepeatMeasureNum=10, nEffectiveMeasureNum=6;
	int i=0, nRepeatMeasureNum=1, nEffectiveMeasureNum=1;
	double dAngle=0.0, dScore=0.0;
	double dPosX, dPosY;
	dPosX=0.0;
	dPosY=0.0;

#ifdef USE_IRAYPLE
	return GrabIRayple(nPos, bDraw);
#endif

#ifdef USE_IDS
	return GrabIds(nPos, bDraw);
#endif

#ifdef USE_CREVIS
	return GrabCrevis(nPos, bDraw);
#endif
	
	return TRUE;
}

void CVision::GetCameraSize(int &nX, int &nY)
{
#ifdef USE_IRAYPLE
	return GetIRaypleSize(nX, nY);
#endif

#ifdef USE_IDS
	if(m_pIds)
	{
		GetIdsSize(nX, nY);
	}
#endif

#ifdef USE_CREVIS
	if(m_pCrevis)
	{
		GetCrevisSize(nX, nY);
	}
#endif
}

void CVision::GetIRaypleSize(int &nX, int &nY)
{
#ifdef USE_IRAYPLE
	if(m_pIRayple)
	{
		nX = m_pIRayple->GetImgWidth();
		nY = m_pIRayple->GetImgHeight();
	}
#endif
}

void CVision::GetIdsSize(int &nX, int &nY)
{
#ifdef USE_IDS
	if(m_pIds)
	{
		nX = m_pIds->m_nSizeX;
		nY = m_pIds->m_nSizeY;
	}
#endif
}

void CVision::GetCrevisSize(int &nX, int &nY)
{
#ifdef USE_CREVIS
	if(m_pCrevis)
	{
		nX = m_pCrevis[0]->GetImgWidth();
		nY = m_pCrevis[0]->GetImgHeight();
	}
#endif
}

void CVision::SetClrOverlay()
{
	SetTimer(TIM_CLR_OVERLAY, 500, NULL);
}

void CVision::InitAlignBuf()
{
	if (MilAlignImgBuf[0] != M_NULL)
		MbufFree(MilAlignImgBuf[0]);
	if (m_pMil)
		MbufAlloc2d(m_pMil->GetSystemID(), 1024, 1024, 1L + M_UNSIGNED, M_IMAGE+M_PROC, &MilAlignImgBuf[0]);

	if (MilAlignImgBuf[1] != M_NULL)
		MbufFree(MilAlignImgBuf[1]);
	if (m_pMil)
		MbufAlloc2d(m_pMil->GetSystemID(), 1024, 1024, 1L + M_UNSIGNED, M_IMAGE + M_PROC, &MilAlignImgBuf[1]);

	if (MilAlignImgBuf[2] != M_NULL)
		MbufFree(MilAlignImgBuf[2]);
	if (m_pMil)
		MbufAlloc2d(m_pMil->GetSystemID(), 1024, 1024, 1L + M_UNSIGNED, M_IMAGE + M_PROC, &MilAlignImgBuf[2]);

	if (MilAlignImgBuf[3] != M_NULL)
		MbufFree(MilAlignImgBuf[3]);
	if (m_pMil)
		MbufAlloc2d(m_pMil->GetSystemID(), 1024, 1024, 1L + M_UNSIGNED, M_IMAGE + M_PROC, &MilAlignImgBuf[3]);

	if (MilBufAlignTemp[0] != M_NULL)
		MbufFree(MilBufAlignTemp[0]);
	if (m_pMil)
		MbufAlloc2d(m_pMil->GetSystemID(), ALIGN_IMG_DISP_SIZEX, ALIGN_IMG_DISP_SIZEY, 1L + M_UNSIGNED, M_IMAGE + M_PROC, &MilBufAlignTemp[0]);

	if (MilBufAlignTemp[1] != M_NULL)
		MbufFree(MilBufAlignTemp[1]);
	if (m_pMil)
		MbufAlloc2d(m_pMil->GetSystemID(), ALIGN_IMG_DISP_SIZEX, ALIGN_IMG_DISP_SIZEY, 1L + M_UNSIGNED, M_IMAGE + M_PROC, &MilBufAlignTemp[1]);

	if (MilBufAlignTemp[2] != M_NULL)
		MbufFree(MilBufAlignTemp[2]);
	if (m_pMil)
		MbufAlloc2d(m_pMil->GetSystemID(), ALIGN_IMG_DISP_SIZEX, ALIGN_IMG_DISP_SIZEY, 1L + M_UNSIGNED, M_IMAGE + M_PROC, &MilBufAlignTemp[2]);

	if (MilBufAlignTemp[3] != M_NULL)
		MbufFree(MilBufAlignTemp[3]);
	if (m_pMil)
		MbufAlloc2d(m_pMil->GetSystemID(), ALIGN_IMG_DISP_SIZEX, ALIGN_IMG_DISP_SIZEY, 1L + M_UNSIGNED, M_IMAGE + M_PROC, &MilBufAlignTemp[3]);
}

BOOL CVision::ClearPinCenterMarkArea(int nCenterX, int nCenterY, int nLineLength)
{
	if(!m_pMilDispPin)
		return FALSE;
	
	if(!m_nPinCtrX && !m_nPinCtrY && !m_nPinCrsLen)
	{
		m_nPinCtrX = nCenterX;
		m_nPinCtrY = nCenterY;
		m_nPinCrsLen = nLineLength;
	}
	
	m_pMilPinOverlayDelete->DrawRectFill(m_pMilDispPin->m_lOverlayColor, 
		m_nPinCtrX-m_nPinCrsLen, 
		m_nPinCtrY-m_nPinCrsLen,
		m_nPinCtrX+m_nPinCrsLen,
		m_nPinCtrY+m_nPinCrsLen);
	m_nPinCtrX = nCenterX;
	m_nPinCtrY = nCenterY;
	m_nPinCrsLen = nLineLength;
	
	return TRUE;
}

BOOL CVision::SaveMkImg(CString sPath)
{
#ifdef USE_IRAYPLE
	m_cs.Lock();
	CLibMilBuf *MilGrabImg = NULL;
	if (m_pMil)
		MilGrabImg = m_pMil->AllocBuf((long)m_pIRayple->GetImgWidth(), (long)m_pIRayple->GetImgHeight(), 8L + M_UNSIGNED, M_IMAGE + M_DISP + M_PROC);

	if (m_pIRayple->OneshotGrab() == FALSE)
	{
		//pView->MsgBox(_T("Image Grab Fail !!"));
		AfxMessageBox(_T("m_pIRayple->OneshotGrab() Fail !!"));
		if (MilGrabImg)
			delete MilGrabImg;
		m_cs.Unlock();
		return FALSE;
	}
	else if(m_pMil->OneshotGrab(MilGrabImg->m_MilImage, GRAB_COLOR_COLOR) == FALSE)
	{
		//pView->MsgBox(_T("Image Grab Fail !!"));
		AfxMessageBox(_T("m_pMil->OneshotGrab Fail !!"));
		if (MilGrabImg)
			delete MilGrabImg;
		m_cs.Unlock();
		return FALSE;
	}

	//if (m_pMil)
	//	MilOriginDisp = m_pMil->AllocBuf(PIN_IMG_DISP_SIZEX, PIN_IMG_DISP_SIZEY, 1L + M_UNSIGNED, M_IMAGE + M_DISP + M_PROC);
	//MimResize(MilGrabImg->m_MilImage, MilOriginDisp->m_MilImage, (double)0.5, (double)0.5, M_DEFAULT);

	//if(MilGrabImg && MilOriginDisp->m_MilImage)
	//	MbufSave(sPath, MilOriginDisp->m_MilImage);
	if(MilGrabImg && MilGrabImg->m_MilImage)
		MbufSave(sPath, MilGrabImg->m_MilImage);
	else
	{
		AfxMessageBox(_T("MbufSave() Fail !!"));
		//if (MilOriginDisp)
		//	delete MilOriginDisp;

		if (MilGrabImg)
			delete MilGrabImg;
		m_cs.Unlock();
		return FALSE;
	}
	//MilGrabImg->ChildBuffer2d(m_pIRayple->GetImgWidth() / 2 - 100, m_pIRayple->GetImgHeight() / 2 - 100, 200, 200);
	//MbufSave(sPath, MilGrabImg->m_MilImageChild);

	//if (MilOriginDisp)
	//	delete MilOriginDisp;
	if (MilGrabImg)
		delete MilGrabImg;
	m_cs.Unlock();
#endif
	return TRUE;
}

#endif
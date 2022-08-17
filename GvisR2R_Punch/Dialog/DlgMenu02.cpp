// DlgMenu02.cpp : implementation file
//

#include "stdafx.h"
#include "../gvisr2r_punch.h"
#include "DlgKeyNum.h"
#include "DlgMenu02.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "../MainFrm.h"
#include "../GvisR2R_PunchDoc.h"
#include "../GvisR2R_PunchView.h"

extern CMainFrame* pFrm;
extern CGvisR2R_PunchDoc* pDoc;
extern CGvisR2R_PunchView* pView;

/////////////////////////////////////////////////////////////////////////////
// CDlgMenu02 dialog


CDlgMenu02::CDlgMenu02(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMenu02::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMenu02)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	int nCam, nPoint;

	m_pDlgUtil01 = NULL;

	m_pRect = NULL;
	m_bLoadImg = FALSE;
	m_pPcsGL = NULL;
//	m_pPinGL = NULL;

	m_bTIM_LIGHT_UP = FALSE;
	m_bTIM_LIGHT_DN = FALSE;
	m_bTIM_LIGHT_UP2 = FALSE;
	m_bTIM_LIGHT_DN2 = FALSE;
	m_bTIM_BUF_ENC = FALSE;

	//m_bTIM_2D_READING = FALSE;
	//m_bTIM_TEST_2D_READING = FALSE;

	m_dStOffsetX = 0.0; 
	m_dStOffsetY = 0.0;

	for (nCam = 0; nCam < 2; nCam++)
	{
		for (nPoint = 0; nPoint < 4; nPoint++)
		{
			m_dMkFdOffsetX[nCam][nPoint] = 0.0;
			m_dMkFdOffsetY[nCam][nPoint] = 0.0;
		}
	}

	m_dAoiUpFdOffsetX = 0.0;
	m_dAoiUpFdOffsetY = 0.0;
	m_dAoiDnFdOffsetX = 0.0;
	m_dAoiDnFdOffsetY = 0.0;

	m_nBtnAlignCam0Pos = 0;
	m_nBtnAlignCam1Pos = 0;
	m_nSpd = 1;
// 	m_bMkDnSolOff = FALSE;

	m_nSelectCam0Pos = 0;
	m_nSelectCam1Pos = 0;

	m_lChk = 0L;
	m_pDlgUtil03 = NULL;
	//m_pDlgUtil06 = NULL;

	m_bLockTimer = FALSE;
}

CDlgMenu02::~CDlgMenu02()
{
	m_bTIM_LIGHT_UP = FALSE;
	m_bTIM_LIGHT_DN = FALSE;
	m_bTIM_LIGHT_UP2 = FALSE;
	m_bTIM_LIGHT_DN2 = FALSE;
	m_bTIM_BUF_ENC = FALSE;

// 	if(m_pVision)
// 	{
// 		delete m_pVision;
// 		m_pVision = NULL;
// 	}


	if(m_pDlgUtil01 != NULL) 
	{
		delete m_pDlgUtil01;
		m_pDlgUtil01 = NULL;
	}

	DelImg();

	if(m_pRect)
	{
		delete m_pRect;
		m_pRect = NULL;
	}

	if(m_pPcsGL)
	{
		delete m_pPcsGL;
		m_pPcsGL = NULL;
	}

// 	if(m_pPinGL)
// 	{
// 		delete m_pPinGL;
// 		m_pPinGL = NULL;
// 	}

	if(m_pDlgUtil03 != NULL) 
	{
		delete m_pDlgUtil03;
		m_pDlgUtil03 = NULL;
	}
	//if(m_pDlgUtil06 != NULL) 
	//{
	//	delete m_pDlgUtil06;
	//	m_pDlgUtil06 = NULL;
	//}
}

BOOL CDlgMenu02::Create()
{
	return CDialog::Create(CDlgMenu02::IDD);
}

void CDlgMenu02::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMenu02)
	DDX_Control(pDX, IDC_SLD_LT2, m_LightSlider2);
	DDX_Control(pDX, IDC_SLD_LT, m_LightSlider);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMenu02, CDialog)
	//{{AFX_MSG_MAP(CDlgMenu02)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_CHK_LT_ON, OnChkLtOn)
	ON_WM_VSCROLL()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHK_JOG_VF, OnChkJogVf)
	ON_BN_CLICKED(IDC_CHK_JOG_F, OnChkJogF)
	ON_BN_CLICKED(IDC_CHK_JOG_N, OnChkJogN)
	ON_BN_CLICKED(IDC_CHK_JOG_S, OnChkJogS)
	ON_BN_CLICKED(IDC_CHK_JOG_VS, OnChkJogVs)
	ON_BN_CLICKED(IDC_BTN_PIN_MOVE, OnBtnPinMove)
	ON_BN_CLICKED(IDC_BTN_PIN_SAVE, OnBtnPinSave)
	ON_BN_CLICKED(IDC_BTN_HOME_MOVE, OnBtnHomeMove)
	ON_BN_CLICKED(IDC_CHK_RES_POS_ST, OnChkResPosSt)
	ON_BN_CLICKED(IDC_CHK_MK_OFFSET_ST, OnChkMkOffsetSt)
	ON_BN_CLICKED(IDC_CHK_MK_OFFSET_ED, OnChkMkOffsetEd)
	ON_BN_CLICKED(IDC_BTN_ALIGN_MOVE, OnBtnAlignMove)
	ON_BN_CLICKED(IDC_BTN_ALIGN, OnBtnAlign)
	ON_BN_CLICKED(IDC_BTN_GRAB, OnBtnGrab)
	ON_BN_CLICKED(IDC_BTN_BUFF_HOME, OnBtnBuffHome)
	ON_BN_CLICKED(IDC_BTN_BUFF_INIT_MOVE, OnBtnBuffInitMove)
	ON_BN_CLICKED(IDC_BTN_BUFF_INIT_SAVE, OnBtnBuffInitSave)
	ON_BN_CLICKED(IDC_BTN_START_SAVE, OnBtnHomeSave)
	ON_BN_CLICKED(IDC_STC_ALIGN_STD_SCR, OnStcAlignStdScr)
	ON_BN_CLICKED(IDC_CHK_LT_ON2, OnChkLtOn2)
	ON_BN_CLICKED(IDC_CHK_RES_POS_ST2, OnChkResPosSt2)
	ON_BN_CLICKED(IDC_BTN_PIN_MOVE2, OnBtnPinMove2)
	ON_BN_CLICKED(IDC_BTN_HOME_MOVE2, OnBtnHomeMove2)
	ON_BN_CLICKED(IDC_BTN_START_SAVE2, OnBtnStartSave2)
	ON_BN_CLICKED(IDC_BTN_PIN_SAVE2, OnBtnPinSave2)
	ON_BN_CLICKED(IDC_CHK_MK_OFFSET_ST2, OnChkMkOffsetSt2)
	ON_BN_CLICKED(IDC_BTN_ALIGN2, OnBtnAlign2)
	ON_BN_CLICKED(IDC_CHK_MARKING_TEST2, OnChkMarkingTest2)
	ON_BN_CLICKED(IDC_CHK_MK_OFFSET_ED2, OnChkMkOffsetEd2)
	ON_BN_CLICKED(IDC_CHK_JOG_VF2, OnChkJogVf2)
	ON_BN_CLICKED(IDC_CHK_JOG_F2, OnChkJogF2)
	ON_BN_CLICKED(IDC_CHK_JOG_N2, OnChkJogN2)
	ON_BN_CLICKED(IDC_CHK_JOG_S2, OnChkJogS2)
	ON_BN_CLICKED(IDC_CHK_JOG_VS2, OnChkJogVs2)
	ON_BN_CLICKED(IDC_CHK_MK_POS1_1, OnChkMk0Pos1)
	ON_BN_CLICKED(IDC_CHK_MK_POS1_2, OnChkMk0Pos2)
	ON_BN_CLICKED(IDC_CHK_MK_POS1_3, OnChkMk0Pos3)
	ON_BN_CLICKED(IDC_CHK_MK_POS1_4, OnChkMk0Pos4)
	ON_BN_CLICKED(IDC_BTN_MK_POS1_MOVE, OnBtnMk0PosMove)
	ON_BN_CLICKED(IDC_CHK_MK_POS2_1, OnChkMk1Pos1)
	ON_BN_CLICKED(IDC_CHK_MK_POS2_2, OnChkMk1Pos2)
	ON_BN_CLICKED(IDC_CHK_MK_POS2_3, OnChkMk1Pos3)
	ON_BN_CLICKED(IDC_CHK_MK_POS2_4, OnChkMk1Pos4)
	ON_BN_CLICKED(IDC_BTN_MK_POS2_MOVE, OnBtnMk1PosMove)
	ON_BN_CLICKED(IDC_BTN_MK_POS1_MEAS, OnBtnMk0PosMeas)
	ON_BN_CLICKED(IDC_BTN_MK_POS2_MEAS, OnBtnMk1PosMeas)
	ON_BN_CLICKED(IDC_BTN_ALIGN_MOVE2, OnBtnAlignMove2)
	ON_BN_CLICKED(IDC_BTN_GRAB2, OnBtnGrab2)
	ON_BN_CLICKED(IDC_STC_ALIGN_STD_SCR2, OnStcAlignStdScr2)
	ON_BN_CLICKED(IDC_CHK_ELEC_TEST, OnChkElecTest)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_DRAW_PCSIMG, OnDrawPcsImg)
	ON_MESSAGE(WM_DRAW_PINIMG, OnDrawPinImg)
	ON_MESSAGE(WM_MYSTATIC_REDRAW, OnMyStaticRedraw)
//	ON_MESSAGE(WM_MYSTATIC_MOUSE_MOVE, OnMyStaticMouseMove)
	ON_MESSAGE(WM_MYBTN_DOWN, OnMyBtnDown)
	ON_MESSAGE(WM_MYBTN_UP, OnMyBtnUp)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMenu02 message handlers

void CDlgMenu02::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if(!m_pRect)
	{
		m_pRect = new CRect;
		
		this->GetClientRect(m_pRect);
		m_pRect->top = 75 + 2;
		m_pRect->bottom += 75 + 2;
		m_pRect->left = 3;
		m_pRect->right += 3;
		this->MoveWindow(m_pRect, TRUE);
	}

	if(bShow)
	{
		AtDlgShow();
	}
	else
	{
		AtDlgHide();
	}
}

void CDlgMenu02::AtDlgShow()
{
	LoadImg();

	CString str;
//	str.Format(_T("%.3f"), _tstof(pDoc->WorkingInfo.Vision[0].sMkOffsetX));
	myStcData[5].SetText(pDoc->WorkingInfo.Vision[0].sMkOffsetX);
//	str.Format(_T("%.3f"), _tstof(pDoc->WorkingInfo.Vision[0].sMkOffsetY));
	myStcData[6].SetText(pDoc->WorkingInfo.Vision[0].sMkOffsetY);

//	str.Format(_T("%.1f"), pDoc->WorkingInfo.Vision[0].dStdScr);
	myStcData[12].SetText(pDoc->WorkingInfo.Vision[0].sStdScr);

//	str.Format(_T("%.3f"), _tstof(pDoc->WorkingInfo.Vision[1].sMkOffsetX));
	myStcData2[5].SetText(pDoc->WorkingInfo.Vision[1].sMkOffsetX);
//	str.Format(_T("%.3f"), _tstof(pDoc->WorkingInfo.Vision[1].sMkOffsetY));
	myStcData2[6].SetText(pDoc->WorkingInfo.Vision[1].sMkOffsetY);

//	str.Format(_T("%.1f"), pDoc->WorkingInfo.Vision[1].dStdScr);
	myStcData2[12].SetText(pDoc->WorkingInfo.Vision[1].sStdScr);

	if(myBtn2[16].GetCheck())
	{
		m_lChk = 1;
		SetTimer(TIM_SHOW_MK_TEST, 50, NULL);
	}
	if(myBtn[16].GetCheck())
	{
		m_lChk = 2;
		SetTimer(TIM_SHOW_ELEC_TEST, 50, NULL);
	}

	SetJogSpd();
	SetJogSpd2();
// 	if(!pView->IsPinMkData())			// 핀위치와 마킹위치 저장 확인.
// 		AfxMessageBox(_T("마킹 Pin위치 값이 설정되지 않았습니다."));

	m_nMoveAlign[0] = 0;
	m_nMoveAlign[1] = 0;

	//Disp();
}

void CDlgMenu02::AtDlgHide()
{
	DelImg();

	if(pView->m_pDlgUtil01)
	{
		if(pView->m_pDlgUtil01->IsWindowVisible())
			pView->m_pDlgUtil01->ShowWindow(SW_HIDE);
	}

	if(m_pDlgUtil03)
	{
		if(m_pDlgUtil03->IsWindowVisible())
		{
			m_pDlgUtil03->ShowWindow(SW_HIDE);
		}
	}
	
	//if(m_pDlgUtil06)
	//{
	//	if(m_pDlgUtil06->IsWindowVisible())
	//	{
	//		m_pDlgUtil06->ShowWindow(SW_HIDE);
	//	}
	//}
}

void CDlgMenu02::LoadImg()
{
	if(m_bLoadImg)
		return;
	m_bLoadImg = TRUE;

	int i;
	for(i=0; i<MAX_MENU02_BTN; i++)
	{
		if(i==1)
		{
			myBtn[i].LoadBkImage(IMG_JOG_UP_DlgMenu02, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_JOG_UP_DlgMenu02, BTN_IMG_DN);
		}
		else if(i==2)
		{
			myBtn[i].LoadBkImage(IMG_JOG_DN_DlgMenu02, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_JOG_DN_DlgMenu02, BTN_IMG_DN);
		}
		else if(i==6)
		{
			myBtn[i].LoadBkImage(IMG_JOG_UP_DlgMenu02, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_JOG_UP_DlgMenu02, BTN_IMG_DN);
		}
		else if(i==7)
		{
			myBtn[i].LoadBkImage(IMG_JOG_DN_DlgMenu02, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_JOG_DN_DlgMenu02, BTN_IMG_DN);
		}
		else if(i==8)
		{
			myBtn[i].LoadBkImage(IMG_JOG_LF_DlgMenu02, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_JOG_LF_DlgMenu02, BTN_IMG_DN);
		}
		else if(i==9)
		{
			myBtn[i].LoadBkImage(IMG_JOG_RT_DlgMenu02, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_JOG_RT_DlgMenu02, BTN_IMG_DN);
		}
// 		else if(i==24)
// 		{
// 			myBtn[i].LoadBkImage(IMG_JOG_UP_DlgMenu02, BTN_IMG_UP);
// 			myBtn[i].LoadBkImage(IMG_JOG_UP_DlgMenu02, BTN_IMG_DN);
// 		}
// 		else if(i==25)
// 		{
// 			myBtn[i].LoadBkImage(IMG_JOG_DN_DlgMenu02, BTN_IMG_UP);
// 			myBtn[i].LoadBkImage(IMG_JOG_DN_DlgMenu02, BTN_IMG_DN);
// 		}
		else if(i!=19 && i!=24 && i!=25 && i!=26 && i!=27 && i!=28)
		{
			myBtn[i].LoadBkImage(IMG_BTN_UP_DlgMenu02, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_BTN_DN_DlgMenu02, BTN_IMG_DN);
		}
	}

	for(i=0; i<MAX_MENU02_BTN_DUO; i++)
	{
		if(i==1)
		{
			myBtn2[i].LoadBkImage(IMG_JOG_UP_DlgMenu02, BTN_IMG_UP);
			myBtn2[i].LoadBkImage(IMG_JOG_UP_DlgMenu02, BTN_IMG_DN);
		}
		else if(i==2)
		{
			myBtn2[i].LoadBkImage(IMG_JOG_DN_DlgMenu02, BTN_IMG_UP);
			myBtn2[i].LoadBkImage(IMG_JOG_DN_DlgMenu02, BTN_IMG_DN);
		}
		else if(i==6)
		{
			myBtn2[i].LoadBkImage(IMG_JOG_UP_DlgMenu02, BTN_IMG_UP);
			myBtn2[i].LoadBkImage(IMG_JOG_UP_DlgMenu02, BTN_IMG_DN);
		}
		else if(i==7)
		{
			myBtn2[i].LoadBkImage(IMG_JOG_DN_DlgMenu02, BTN_IMG_UP);
			myBtn2[i].LoadBkImage(IMG_JOG_DN_DlgMenu02, BTN_IMG_DN);
		}
		else if(i==8)
		{
			myBtn2[i].LoadBkImage(IMG_JOG_LF_DlgMenu02, BTN_IMG_UP);
			myBtn2[i].LoadBkImage(IMG_JOG_LF_DlgMenu02, BTN_IMG_DN);
		}
		else if(i==9)
		{
			myBtn2[i].LoadBkImage(IMG_JOG_RT_DlgMenu02, BTN_IMG_UP);
			myBtn2[i].LoadBkImage(IMG_JOG_RT_DlgMenu02, BTN_IMG_DN);
		}
		else
		{
			myBtn2[i].LoadBkImage(IMG_BTN_UP_DlgMenu02, BTN_IMG_UP);
			myBtn2[i].LoadBkImage(IMG_BTN_DN_DlgMenu02, BTN_IMG_DN);
		}
	}

}

void CDlgMenu02::DelImg()
{
	if(!m_bLoadImg)
		return;
	m_bLoadImg = FALSE;

	int i;
	for(i=0; i<MAX_MENU02_BTN; i++)
	{
		if(i!=18 && i!=19 && i!=24 && i!=25)
			myBtn[i].DelImgList();
	}
	for(i=0; i<MAX_MENU02_BTN_DUO; i++)
	{
		myBtn2[i].DelImgList();
	}
}

BOOL CDlgMenu02::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	HWND hW[2];
	CRect rt[2];
	void *pMilSys=NULL;

// 	if(pView->m_pVision[0])
// 	{
// 		hW[0] = GetDlgItem(IDC_STC_VISION)->GetSafeHwnd();
// 		GetDlgItem(IDC_STC_VISION)->GetWindowRect(&rt[0]);
// 		pMilSys = pView->m_pVision[0]->SelLive(hW[0], rt[0], pMilSys);
// 	}
// 
// 	if(pView->m_pVision[1])
// 	{
// 		hW[1] = GetDlgItem(IDC_STC_VISION_2)->GetSafeHwnd();
// 		GetDlgItem(IDC_STC_VISION_2)->GetWindowRect(&rt[1]);
// 		pMilSys = pView->m_pVision[1]->SelLive(hW[1], rt[1], pMilSys);
// 	}

// 	if(pView->m_pVision[0])
// 		pMilSys = pView->m_pVision[0]->SelLive2(hW[0], rt[0], pMilSys);
// 	if(pView->m_pVision[1])
// 		pMilSys = pView->m_pVision[1]->SelLive2(hW[1], rt[1], pMilSys);

#ifdef USE_VISION

	if(pView->m_pVision[1])
	{
		delete pView->m_pVision[1];
		pView->m_pVision[1] = NULL;
	}
	if(pView->m_pVision[0])
	{
		delete pView->m_pVision[0];
		pView->m_pVision[0] = NULL;
	}

	HWND hCtrlV0[4] = {0};
	hCtrlV0[0] = GetDlgItem(IDC_STC_VISION)->GetSafeHwnd();
	MIL_ID MilSys = M_NULL;
	pView->m_pVision[0] = new CVision(0, MilSys, hCtrlV0, this);
	MilSys = pView->m_pVision[0]->GetSystemID();

//#ifndef TEST_MODE
	HWND hCtrlV1[4] = { 0 };
	hCtrlV1[0] = GetDlgItem(IDC_STC_VISION_2)->GetSafeHwnd();
	pView->m_pVision[1] = new CVision(1, MilSys, hCtrlV1, this);
//#endif


	if(pView->m_pVision[0])
	{
		pView->m_pVision[0]->ClearOverlay();
		pView->m_pVision[0]->DrawCenterMark();
	}

	if(pView->m_pVision[1])
	{
		pView->m_pVision[1]->ClearOverlay();
		pView->m_pVision[1]->DrawCenterMark();
	}
#endif

	InitStatic();
	InitBtn();
//	InitGL();
	InitSlider();

	InitStaticDuo();
	InitBtnDuo();


	InitCadImg();


	myStcData[3].SetText(pDoc->WorkingInfo.Vision[0].sResX);
	myStcData[4].SetText(pDoc->WorkingInfo.Vision[0].sResY);

	myStcData2[3].SetText(pDoc->WorkingInfo.Vision[1].sResX);
	myStcData2[4].SetText(pDoc->WorkingInfo.Vision[1].sResY);

// 	if(pDoc->m_pSpecLocal)
// 	{
// 		CfPoint ptOfst(pDoc->m_pSpecLocal->m_dPcsOffsetX, pDoc->m_pSpecLocal->m_dPcsOffsetY);
// 		SetMkPos(ptOfst);
// 	}

	m_bTIM_BUF_ENC = TRUE;
 	SetTimer(TIM_BUF_ENC, 100, NULL);

	GetDlgItem(IDC_GRP_LT6)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_BUF_ROL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_BUF_HI)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_BUF_POS)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_BUFF_UP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_BUFF_DN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_BUFF_HOME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_BUFF_INIT_MOVE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_BUFF_INIT_SAVE)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_CHK_ELEC_TEST)->ShowWindow(SW_HIDE);

	
	ShowDlg(IDD_DLG_UTIL_03);
#ifdef USE_FLUCK
	ShowDlg(IDD_DLG_UTIL_06);
#endif

	//DispCenterMark();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMenu02::InitCadImg()
{
	HWND hWin;
	CRect rect;

#ifdef USE_VISION
	if(pView->m_pVision[0])
	{
// 		hW = GetDlgItem(IDC_STC_PCS_IMG)->GetSafeHwnd();
// 		GetDlgItem(IDC_STC_PCS_IMG)->GetWindowRect(&rt);
// 		pView->m_pVision[0]->SelDispPcs(hW, rt, 0);
// 		pView->m_pVision[0]->ShowDispPcs(0);

		hWin = GetDlgItem(IDC_STC_PIN_IMG)->GetSafeHwnd();
		GetDlgItem(IDC_STC_PIN_IMG)->GetWindowRect(&rect);
		pView->m_pVision[0]->SelDispPin(hWin, rect, 0);
		pView->m_pVision[0]->ShowDispPin(0);
//		pView->m_pVision[0]->DrawCrossOnPin(PIN_IMG_DISP_SIZEX/2, PIN_IMG_DISP_SIZEY/2, 10);

// 		hWin = GetDlgItem(IDC_STC_PIN_IMG)->GetSafeHwnd();
// 		GetDlgItem(IDC_STC_PIN_IMG)->GetWindowRect(&rect);
 		pView->m_pVision[0]->SelDispAlign(hWin, rect, 0);
		pView->m_pVision[0]->ShowDispAlign();

		pView->m_pVision[0]->SetDispAxisPos();
	}

	if(pView->m_pVision[1])
	{
// 		hW = GetDlgItem(IDC_STC_PCS_IMG)->GetSafeHwnd();
// 		GetDlgItem(IDC_STC_PCS_IMG)->GetWindowRect(&rt);
// 		pView->m_pVision[0]->SelDispPcs(hW, rt, 0);
// 		pView->m_pVision[0]->ShowDispPcs(1);

		hWin = GetDlgItem(IDC_STC_PIN_IMG_2)->GetSafeHwnd();
		GetDlgItem(IDC_STC_PIN_IMG_2)->GetWindowRect(&rect);
		pView->m_pVision[1]->SelDispPin(hWin, rect, 0);
		pView->m_pVision[1]->ShowDispPin(0);
//		pView->m_pVision[1]->DrawCrossOnPin(PIN_IMG_DISP_SIZEX/2, PIN_IMG_DISP_SIZEY/2, 10);

// 		hWin = GetDlgItem(IDC_STC_PIN_IMG_2)->GetSafeHwnd();
// 		GetDlgItem(IDC_STC_PIN_IMG_2)->GetWindowRect(&rect);
 		pView->m_pVision[1]->SelDispAlign(hWin, rect, 0);
		pView->m_pVision[1]->ShowDispAlign();

		pView->m_pVision[1]->SetDispAxisPos();
	}
#endif
}

void CDlgMenu02::InitSlider()
{
	m_LightSlider.SetRange(0,100);
	m_LightSlider.SetLineSize(1);
	m_LightSlider.SetPageSize(10);
	m_LightSlider.SetPos(100);
	m_LightSlider.SetTicFreq(10);

	m_LightSlider2.SetRange(0,100);
	m_LightSlider2.SetLineSize(1);
	m_LightSlider2.SetPageSize(10);
	m_LightSlider2.SetPos(100);
	m_LightSlider2.SetTicFreq(10);
}

// void CDlgMenu02::InitGL()
// {
// 	if(m_pPcsGL)
// 	{
// 		delete m_pPcsGL;
// 		m_pPcsGL = NULL;
// 	}
// 
// 	if(!m_pPcsGL)
// 	{
// 		m_pPcsGL = new CMyGL(this);
// 		m_pPcsGL->Init(IDC_STC_PCS_IMG);
// 	}
// 
// #ifdef TEST_MODE
// 	m_pPcsGL->LoadPcsImg(PATH_PCS_IMG_);
// #else
// 	m_pPcsGL->LoadPcsImg(PATH_PCS_IMG);
// #endif
// 
// }

void CDlgMenu02::InitBtn()
{
	myBtn[0].SubclassDlgItem(IDC_CHK_LT_ON, this);
	myBtn[0].SetHwnd(this->GetSafeHwnd(), IDC_CHK_LT_ON);
	myBtn[0].SetBtnType(BTN_TYPE_CHECK);

	myBtn[1].SubclassDlgItem(IDC_BTN_LT_UP, this);
	myBtn[1].SetHwnd(this->GetSafeHwnd(), IDC_BTN_LT_UP);
	myBtn[1].SetBoarder(FALSE);

	myBtn[2].SubclassDlgItem(IDC_BTN_LT_DN, this);
	myBtn[2].SetHwnd(this->GetSafeHwnd(), IDC_BTN_LT_DN);
	myBtn[2].SetBoarder(FALSE);

	myBtn[3].SubclassDlgItem(IDC_BTN_PIN_MOVE, this);
	myBtn[3].SetHwnd(this->GetSafeHwnd(), IDC_BTN_PIN_MOVE);

	myBtn[4].SubclassDlgItem(IDC_BTN_PIN_SAVE, this);
	myBtn[4].SetHwnd(this->GetSafeHwnd(), IDC_BTN_PIN_SAVE);

	myBtn[5].SubclassDlgItem(IDC_BTN_HOME_MOVE, this);
	myBtn[5].SetHwnd(this->GetSafeHwnd(), IDC_BTN_HOME_MOVE);

	myBtn[6].SubclassDlgItem(IDC_BTN_JOG_UP, this);
	myBtn[6].SetHwnd(this->GetSafeHwnd(), IDC_BTN_JOG_UP);
	myBtn[6].SetBoarder(FALSE);

	myBtn[7].SubclassDlgItem(IDC_BTN_JOG_DN, this);
	myBtn[7].SetHwnd(this->GetSafeHwnd(), IDC_BTN_JOG_DN);
	myBtn[7].SetBoarder(FALSE);

	myBtn[8].SubclassDlgItem(IDC_BTN_JOG_LF, this);
	myBtn[8].SetHwnd(this->GetSafeHwnd(), IDC_BTN_JOG_LF);
	myBtn[8].SetBoarder(FALSE);

	myBtn[9].SubclassDlgItem(IDC_BTN_JOG_RT, this);
	myBtn[9].SetHwnd(this->GetSafeHwnd(), IDC_BTN_JOG_RT);
	myBtn[9].SetBoarder(FALSE);

	myBtn[10].SubclassDlgItem(IDC_CHK_JOG_VF, this);
	myBtn[10].SetHwnd(this->GetSafeHwnd(), IDC_CHK_JOG_VF);
	myBtn[10].SetBtnType(BTN_TYPE_CHECK);

	myBtn[11].SubclassDlgItem(IDC_CHK_JOG_F, this);
	myBtn[11].SetHwnd(this->GetSafeHwnd(), IDC_CHK_JOG_F);
	myBtn[11].SetBtnType(BTN_TYPE_CHECK);

	myBtn[12].SubclassDlgItem(IDC_CHK_JOG_N, this);
	myBtn[12].SetHwnd(this->GetSafeHwnd(), IDC_CHK_JOG_N);
	myBtn[12].SetBtnType(BTN_TYPE_CHECK);

	myBtn[13].SubclassDlgItem(IDC_CHK_JOG_S, this);
	myBtn[13].SetHwnd(this->GetSafeHwnd(), IDC_CHK_JOG_S);
	myBtn[13].SetBtnType(BTN_TYPE_CHECK);

	myBtn[14].SubclassDlgItem(IDC_CHK_JOG_VS, this);
	myBtn[14].SetHwnd(this->GetSafeHwnd(), IDC_CHK_JOG_VS);
	myBtn[14].SetBtnType(BTN_TYPE_CHECK);

	myBtn[15].SubclassDlgItem(IDC_BTN_ALIGN, this);
	myBtn[15].SetHwnd(this->GetSafeHwnd(), IDC_BTN_ALIGN);

	myBtn[16].SubclassDlgItem(IDC_CHK_ELEC_TEST, this);
	myBtn[16].SetHwnd(this->GetSafeHwnd(), IDC_CHK_ELEC_TEST);
	myBtn[16].SetBtnType(BTN_TYPE_CHECK);

	myBtn[17].SubclassDlgItem(IDC_CHK_RES_POS_ST, this);
	myBtn[17].SetHwnd(this->GetSafeHwnd(), IDC_CHK_RES_POS_ST);
	myBtn[17].SetBtnType(BTN_TYPE_CHECK);

	myBtn[18].SubclassDlgItem(IDC_BTN_START_SAVE, this);
	myBtn[18].SetHwnd(this->GetSafeHwnd(), IDC_BTN_START_SAVE);
// 	myBtn[18].SubclassDlgItem(IDC_CHK_RES_POS_SET, this);
// 	myBtn[18].SetHwnd(this->GetSafeHwnd(), IDC_CHK_RES_POS_SET);

// 	myBtn[19].SubclassDlgItem(IDC_CHK_RES_POS_ED, this);
// 	myBtn[19].SetHwnd(this->GetSafeHwnd(), IDC_CHK_RES_POS_ED);

	myBtn[20].SubclassDlgItem(IDC_CHK_MK_OFFSET_ST, this);
	myBtn[20].SetHwnd(this->GetSafeHwnd(), IDC_CHK_MK_OFFSET_ST);
	myBtn[20].SetBtnType(BTN_TYPE_CHECK);

	myBtn[21].SubclassDlgItem(IDC_CHK_MK_OFFSET_ED, this);
	myBtn[21].SetHwnd(this->GetSafeHwnd(), IDC_CHK_MK_OFFSET_ED);
	myBtn[21].SetBtnType(BTN_TYPE_CHECK);

	myBtn[22].SubclassDlgItem(IDC_BTN_ALIGN_MOVE, this);
	myBtn[22].SetHwnd(this->GetSafeHwnd(), IDC_BTN_ALIGN_MOVE);

	myBtn[23].SubclassDlgItem(IDC_BTN_GRAB, this);
	myBtn[23].SetHwnd(this->GetSafeHwnd(), IDC_BTN_GRAB);

// 	myBtn[24].SubclassDlgItem(IDC_BTN_BUFF_UP, this);
// 	myBtn[24].SetHwnd(this->GetSafeHwnd(), IDC_BTN_BUFF_UP);
// 	myBtn[24].SetBoarder(FALSE);
// 	myBtn[25].SubclassDlgItem(IDC_BTN_BUFF_DN, this);
// 	myBtn[25].SetHwnd(this->GetSafeHwnd(), IDC_BTN_BUFF_DN);
// 	myBtn[25].SetBoarder(FALSE);
// 	myBtn[26].SubclassDlgItem(IDC_BTN_BUFF_HOME, this);
// 	myBtn[26].SetHwnd(this->GetSafeHwnd(), IDC_BTN_BUFF_HOME);
// 	myBtn[27].SubclassDlgItem(IDC_BUTTIDC_BTN_BUFF_INIT_MOVE, this);
// 	myBtn[27].SetHwnd(this->GetSafeHwnd(), IDC_BUTTIDC_BTN_BUFF_INIT_MOVE);
// 	myBtn[28].SubclassDlgItem(IDC_BUTTIDC_BTN_BUFF_INIT_SAVE, this);
// 	myBtn[28].SetHwnd(this->GetSafeHwnd(), IDC_BUTTIDC_BTN_BUFF_INIT_SAVE);

	myBtn[29].SubclassDlgItem(IDC_CHK_MK_POS1_1, this);
	myBtn[29].SetHwnd(this->GetSafeHwnd(), IDC_CHK_MK_POS1_1);
	myBtn[29].SetBtnType(BTN_TYPE_CHECK);

	myBtn[30].SubclassDlgItem(IDC_CHK_MK_POS1_2, this);
	myBtn[30].SetHwnd(this->GetSafeHwnd(), IDC_CHK_MK_POS1_2);
	myBtn[30].SetBtnType(BTN_TYPE_CHECK);

	myBtn[31].SubclassDlgItem(IDC_CHK_MK_POS1_3, this);
	myBtn[31].SetHwnd(this->GetSafeHwnd(), IDC_CHK_MK_POS1_3);
	myBtn[31].SetBtnType(BTN_TYPE_CHECK);

	myBtn[32].SubclassDlgItem(IDC_CHK_MK_POS1_4, this);
	myBtn[32].SetHwnd(this->GetSafeHwnd(), IDC_CHK_MK_POS1_4);
	myBtn[32].SetBtnType(BTN_TYPE_CHECK);

	myBtn[33].SubclassDlgItem(IDC_BTN_MK_POS1_MOVE, this);
	myBtn[33].SetHwnd(this->GetSafeHwnd(), IDC_BTN_MK_POS1_MOVE);

	myBtn[34].SubclassDlgItem(IDC_BTN_MK_POS1_MEAS, this);
	myBtn[34].SetHwnd(this->GetSafeHwnd(), IDC_BTN_MK_POS1_MEAS);

	int i;
	for(i=0; i<MAX_MENU02_BTN; i++)
	{
		if(i!=19 && i!=24 && i!=25 && i!=26 && i!=27 && i!=28)
		{
			myBtn[i].SetFont(_T("굴림체"),16,TRUE);
			myBtn[i].SetTextColor(RGB_BLACK);
//			myBtn[i].SetBtnType(BTN_TYPE_CHECK);
		}
	}

	myBtn[20].EnableWindow(TRUE);	// IDC_CHK_MK_OFFSET_ST
	myBtn[21].EnableWindow(FALSE);	// IDC_CHK_MK_OFFSET_ED
}

void CDlgMenu02::InitBtnDuo()
{
	myBtn2[0].SubclassDlgItem(IDC_CHK_LT_ON2, this);
	myBtn2[0].SetHwnd(this->GetSafeHwnd(), IDC_CHK_LT_ON2);
	myBtn2[0].SetBtnType(BTN_TYPE_CHECK);
	myBtn2[1].SubclassDlgItem(IDC_BTN_LT_UP2, this);
	myBtn2[1].SetHwnd(this->GetSafeHwnd(), IDC_BTN_LT_UP2);
	myBtn2[1].SetBoarder(FALSE);
	myBtn2[2].SubclassDlgItem(IDC_BTN_LT_DN2, this);
	myBtn2[2].SetHwnd(this->GetSafeHwnd(), IDC_BTN_LT_DN2);
	myBtn2[2].SetBoarder(FALSE);
	myBtn2[3].SubclassDlgItem(IDC_BTN_PIN_MOVE2, this);
	myBtn2[3].SetHwnd(this->GetSafeHwnd(), IDC_BTN_PIN_MOVE2);
	myBtn2[4].SubclassDlgItem(IDC_BTN_PIN_SAVE2, this);
	myBtn2[4].SetHwnd(this->GetSafeHwnd(), IDC_BTN_PIN_SAVE2);
	myBtn2[5].SubclassDlgItem(IDC_BTN_HOME_MOVE2, this);
	myBtn2[5].SetHwnd(this->GetSafeHwnd(), IDC_BTN_HOME_MOVE2);
	myBtn2[6].SubclassDlgItem(IDC_BTN_JOG_UP2, this);
	myBtn2[6].SetHwnd(this->GetSafeHwnd(), IDC_BTN_JOG_UP2);
	myBtn2[6].SetBoarder(FALSE);
	myBtn2[7].SubclassDlgItem(IDC_BTN_JOG_DN2, this);
	myBtn2[7].SetHwnd(this->GetSafeHwnd(), IDC_BTN_JOG_DN2);
	myBtn2[7].SetBoarder(FALSE);
	myBtn2[8].SubclassDlgItem(IDC_BTN_JOG_LF2, this);
	myBtn2[8].SetHwnd(this->GetSafeHwnd(), IDC_BTN_JOG_LF2);
	myBtn2[8].SetBoarder(FALSE);
	myBtn2[9].SubclassDlgItem(IDC_BTN_JOG_RT2, this);
	myBtn2[9].SetHwnd(this->GetSafeHwnd(), IDC_BTN_JOG_RT2);
	myBtn2[9].SetBoarder(FALSE);
	myBtn2[10].SubclassDlgItem(IDC_CHK_JOG_VF2, this);
	myBtn2[10].SetHwnd(this->GetSafeHwnd(), IDC_CHK_JOG_VF2);
	myBtn2[10].SetBtnType(BTN_TYPE_CHECK);
	myBtn2[11].SubclassDlgItem(IDC_CHK_JOG_F2, this);
	myBtn2[11].SetHwnd(this->GetSafeHwnd(), IDC_CHK_JOG_F2);
	myBtn2[11].SetBtnType(BTN_TYPE_CHECK);
	myBtn2[12].SubclassDlgItem(IDC_CHK_JOG_N2, this);
	myBtn2[12].SetHwnd(this->GetSafeHwnd(), IDC_CHK_JOG_N2);
	myBtn2[12].SetBtnType(BTN_TYPE_CHECK);
	myBtn2[13].SubclassDlgItem(IDC_CHK_JOG_S2, this);
	myBtn2[13].SetHwnd(this->GetSafeHwnd(), IDC_CHK_JOG_S2);
	myBtn2[13].SetBtnType(BTN_TYPE_CHECK);
	myBtn2[14].SubclassDlgItem(IDC_CHK_JOG_VS2, this);
	myBtn2[14].SetHwnd(this->GetSafeHwnd(), IDC_CHK_JOG_VS2);
	myBtn2[14].SetBtnType(BTN_TYPE_CHECK);
	myBtn2[15].SubclassDlgItem(IDC_BTN_ALIGN2, this);
	myBtn2[15].SetHwnd(this->GetSafeHwnd(), IDC_BTN_ALIGN2);
	myBtn2[16].SubclassDlgItem(IDC_CHK_MARKING_TEST2, this);
	myBtn2[16].SetHwnd(this->GetSafeHwnd(), IDC_CHK_MARKING_TEST2);
	myBtn2[16].SetBtnType(BTN_TYPE_CHECK);
	myBtn2[17].SubclassDlgItem(IDC_CHK_RES_POS_ST2, this);
	myBtn2[17].SetHwnd(this->GetSafeHwnd(), IDC_CHK_RES_POS_ST2);
	myBtn2[17].SetBtnType(BTN_TYPE_CHECK);
	myBtn2[18].SubclassDlgItem(IDC_BTN_START_SAVE2, this);
	myBtn2[18].SetHwnd(this->GetSafeHwnd(), IDC_BTN_START_SAVE2);
	myBtn2[19].SubclassDlgItem(IDC_CHK_MK_OFFSET_ST2, this);
	myBtn2[19].SetHwnd(this->GetSafeHwnd(), IDC_CHK_MK_OFFSET_ST2);
	myBtn2[19].SetBtnType(BTN_TYPE_CHECK);
	myBtn2[20].SubclassDlgItem(IDC_CHK_MK_OFFSET_ED2, this);
	myBtn2[20].SetHwnd(this->GetSafeHwnd(), IDC_CHK_MK_OFFSET_ED2);
	myBtn2[20].SetBtnType(BTN_TYPE_CHECK);
	myBtn2[21].SubclassDlgItem(IDC_BTN_ALIGN_MOVE2, this);
	myBtn2[21].SetHwnd(this->GetSafeHwnd(), IDC_BTN_ALIGN_MOVE2);
	myBtn2[22].SubclassDlgItem(IDC_BTN_GRAB2, this);
	myBtn2[22].SetHwnd(this->GetSafeHwnd(), IDC_BTN_GRAB2);

	myBtn2[23].SubclassDlgItem(IDC_BTN_MK_POS2_MEAS, this);
	myBtn2[23].SetHwnd(this->GetSafeHwnd(), IDC_BTN_MK_POS2_MEAS);
	myBtn2[24].SubclassDlgItem(IDC_CHK_MK_POS2_1, this);
	myBtn2[24].SetHwnd(this->GetSafeHwnd(), IDC_CHK_MK_POS2_1);
	myBtn2[24].SetBtnType(BTN_TYPE_CHECK);
	myBtn2[25].SubclassDlgItem(IDC_CHK_MK_POS2_2, this);
	myBtn2[25].SetHwnd(this->GetSafeHwnd(), IDC_CHK_MK_POS2_2);
	myBtn2[25].SetBtnType(BTN_TYPE_CHECK);
	myBtn2[26].SubclassDlgItem(IDC_CHK_MK_POS2_3, this);
	myBtn2[26].SetHwnd(this->GetSafeHwnd(), IDC_CHK_MK_POS2_3);
	myBtn2[26].SetBtnType(BTN_TYPE_CHECK);
	myBtn2[27].SubclassDlgItem(IDC_CHK_MK_POS2_4, this);
	myBtn2[27].SetHwnd(this->GetSafeHwnd(), IDC_CHK_MK_POS2_4);
	myBtn2[27].SetBtnType(BTN_TYPE_CHECK);
	myBtn2[28].SubclassDlgItem(IDC_BTN_MK_POS2_MOVE, this);
	myBtn2[28].SetHwnd(this->GetSafeHwnd(), IDC_BTN_MK_POS2_MOVE);

	int i;
	for(i=0; i<MAX_MENU02_BTN_DUO; i++)
	{
		myBtn2[i].SetFont(_T("굴림체"),16,TRUE);
		myBtn2[i].SetTextColor(RGB_BLACK);
//		myBtn2[i].SetBtnType(BTN_TYPE_CHECK);
	}

	myBtn2[19].EnableWindow(TRUE);	// IDC_CHK_MK_OFFSET_ST
	myBtn2[20].EnableWindow(FALSE);	// IDC_CHK_MK_OFFSET_ED
}

void CDlgMenu02::InitStatic()
{
// 	myStcPcsImg.SubclassDlgItem(IDC_STC_PCS_IMG, this);
// 	myStcPcsImg.SetHwnd(this->GetSafeHwnd(), IDC_STC_PCS_IMG);

// 	myStcPinImg.SubclassDlgItem(IDC_STC_PIN_IMG, this);
// 	myStcPinImg.SetHwnd(this->GetSafeHwnd(), IDC_STC_PIN_IMG);

	InitStcTitle();
	InitStcData();
}

void CDlgMenu02::InitStaticDuo()
{
	InitStcTitleDuo();
	InitStcDataDuo();
}

void CDlgMenu02::InitStcTitle()
{
// 	myStcTitle[0].SubclassDlgItem(IDC_STC_PCS_OFFSET, this);
// 	myStcTitle[1].SubclassDlgItem(IDC_STC_PCS_OFFSET_X, this);
// 	myStcTitle[2].SubclassDlgItem(IDC_STC_PCS_OFFSET_Y, this);
	myStcTitle[3].SubclassDlgItem(IDC_STC_RES, this);
	myStcTitle[4].SubclassDlgItem(IDC_STC_RES_X, this);
	myStcTitle[5].SubclassDlgItem(IDC_STC_RES_Y, this);
// 	myStcTitle[6].SubclassDlgItem(IDC_STC_RES_POS1, this);
// 	myStcTitle[7].SubclassDlgItem(IDC_STC_RES_POS2, this);
	myStcTitle[8].SubclassDlgItem(IDC_STC_MK_OFFSET, this);
	myStcTitle[9].SubclassDlgItem(IDC_STC_CAM_MK_X, this);
	myStcTitle[10].SubclassDlgItem(IDC_STC_CAM_MK_Y, this);
	myStcTitle[11].SubclassDlgItem(IDC_STC_MK_POS1, this);
	myStcTitle[12].SubclassDlgItem(IDC_STC_MK_POS2, this);
	myStcTitle[13].SubclassDlgItem(IDC_STC_ALN, this);
	myStcTitle[14].SubclassDlgItem(IDC_STC_ALN_X, this);
	myStcTitle[15].SubclassDlgItem(IDC_STC_ALN_Y, this);
	myStcTitle[16].SubclassDlgItem(IDC_STC_ALN_ANGL, this);
	myStcTitle[17].SubclassDlgItem(IDC_STC_ALN_SCR, this);
// 	myStcTitle[18].SubclassDlgItem(IDC_STC_BUF_ROL, this);
// 	myStcTitle[19].SubclassDlgItem(IDC_STC_BUF_HI, this);
// 	myStcTitle[20].SubclassDlgItem(IDC_STC_JOG, this);
	myStcTitle[21].SubclassDlgItem(IDC_STC_ALN_SCR2, this);
	myStcTitle[22].SubclassDlgItem(IDC_STC_ALN3, this);

	for(int i=0; i<MAX_MENU02_STC; i++)
	{
		if(i!=0 && i!=1 && i!=2 && i!=6 && i!=7 && i!=18 && i!=19 && i!=20)
		{
			myStcTitle[i].SetFontName(_T("Arial"));
			myStcTitle[i].SetFontSize(14);
			myStcTitle[i].SetFontBold(TRUE);
			myStcTitle[i].SetTextColor(RGB_NAVY);
			myStcTitle[i].SetBkColor(RGB_DLG_FRM);
		}
	}
}

void CDlgMenu02::InitStcTitleDuo()
{
	myStcTitle2[0].SubclassDlgItem(IDC_STC_RES2, this);
	myStcTitle2[1].SubclassDlgItem(IDC_STC_RES_X9, this);
	myStcTitle2[2].SubclassDlgItem(IDC_STC_RES_Y4, this);
	myStcTitle2[3].SubclassDlgItem(IDC_STC_MK_OFFSET4, this);
	myStcTitle2[4].SubclassDlgItem(IDC_STC_CAM_MK_X2, this);
	myStcTitle2[5].SubclassDlgItem(IDC_STC_CAM_MK_Y2, this);
	myStcTitle2[6].SubclassDlgItem(IDC_STC_MK_POS3, this);
	myStcTitle2[7].SubclassDlgItem(IDC_STC_MK_POS4, this);
	myStcTitle2[8].SubclassDlgItem(IDC_STC_ALN2, this);
	myStcTitle2[9].SubclassDlgItem(IDC_STC_ALN_X2, this);
	myStcTitle2[10].SubclassDlgItem(IDC_STC_ALN_Y2, this);
	myStcTitle2[11].SubclassDlgItem(IDC_STC_ALN_ANGL2, this);
	myStcTitle2[12].SubclassDlgItem(IDC_STC_ALN_SCR3, this);
	myStcTitle2[13].SubclassDlgItem(IDC_STC_ALN_SCR4, this);
	myStcTitle2[14].SubclassDlgItem(IDC_STC_ALN4, this);

	for(int i=0; i<MAX_MENU02_STC_DUO; i++)
	{
		myStcTitle2[i].SetFontName(_T("Arial"));
		myStcTitle2[i].SetFontSize(14);
		myStcTitle2[i].SetFontBold(TRUE);
		myStcTitle2[i].SetTextColor(RGB_NAVY);
		myStcTitle2[i].SetBkColor(RGB_DLG_FRM);
	}
}

void CDlgMenu02::InitStcData()
{
// 	myStcData[0].SubclassDlgItem(IDC_STC_MK_OFFSET_X, this);
// 	myStcData[1].SubclassDlgItem(IDC_STC_MK_OFFSET_Y, this);
	myStcData[2].SubclassDlgItem(IDC_STC_LT_VAL, this);
	myStcData[3].SubclassDlgItem(IDC_STC_RESOLUTION_X, this);
	myStcData[4].SubclassDlgItem(IDC_STC_RESOLUTION_Y, this);
	myStcData[5].SubclassDlgItem(IDC_STC_CAM_MK_OFFSET_X, this);
	myStcData[6].SubclassDlgItem(IDC_STC_CAM_MK_OFFSET_Y, this);
	myStcData[7].SubclassDlgItem(IDC_STC_ALIGN_X, this);
	myStcData[8].SubclassDlgItem(IDC_STC_ALIGN_Y, this);
	myStcData[9].SubclassDlgItem(IDC_STC_ALIGN_AGL, this);
	myStcData[10].SubclassDlgItem(IDC_STC_ALIGN_SCR, this);
//	myStcData[11].SubclassDlgItem(IDC_STC_BUF_POS, this);
	myStcData[12].SubclassDlgItem(IDC_STC_ALIGN_STD_SCR, this);

	myStcData[13].SubclassDlgItem(IDC_STC_MK_POS1_1, this);
	myStcData[14].SubclassDlgItem(IDC_STC_MK_POS1_2, this);
	myStcData[15].SubclassDlgItem(IDC_STC_MK_POS1_3, this);
	myStcData[16].SubclassDlgItem(IDC_STC_MK_POS1_4, this);

	for(int i=0; i<MAX_MENU02_STC_DATA; i++)
	{
		if(i!=0 && i!=1 && i!=11)
		{
			myStcData[i].SetFontName(_T("Arial"));
			if(i==2)
				myStcData[i].SetFontSize(22);
			else
				myStcData[i].SetFontSize(14);
			myStcData[i].SetFontBold(TRUE);
			myStcData[i].SetTextColor(RGB_BLACK);
			myStcData[i].SetBkColor(RGB_WHITE);
		}
	}

//	myStcData[6].EnableWindow(FALSE); // IDC_STC_CAM_MK_OFFSET_Y
}

void CDlgMenu02::InitStcDataDuo()
{
// 	myStcData2[0].SubclassDlgItem(IDC_STC_MK_OFFSET_X2, this);
// 	myStcData2[1].SubclassDlgItem(IDC_STC_MK_OFFSET_Y2, this);
	myStcData2[2].SubclassDlgItem(IDC_STC_LT_VAL2, this);
	myStcData2[3].SubclassDlgItem(IDC_STC_RESOLUTION_X2, this);
	myStcData2[4].SubclassDlgItem(IDC_STC_RESOLUTION_Y2, this);
	myStcData2[5].SubclassDlgItem(IDC_STC_CAM_MK_OFFSET_X2, this);
	myStcData2[6].SubclassDlgItem(IDC_STC_CAM_MK_OFFSET_Y2, this);
	myStcData2[7].SubclassDlgItem(IDC_STC_ALIGN_X2, this);
	myStcData2[8].SubclassDlgItem(IDC_STC_ALIGN_Y2, this);
	myStcData2[9].SubclassDlgItem(IDC_STC_ALIGN_AGL2, this);
	myStcData2[10].SubclassDlgItem(IDC_STC_ALIGN_SCR2, this);
	myStcData2[12].SubclassDlgItem(IDC_STC_ALIGN_STD_SCR2, this);

	myStcData2[13].SubclassDlgItem(IDC_STC_MK_POS2_1, this);
	myStcData2[14].SubclassDlgItem(IDC_STC_MK_POS2_2, this);
	myStcData2[15].SubclassDlgItem(IDC_STC_MK_POS2_3, this);
	myStcData2[16].SubclassDlgItem(IDC_STC_MK_POS2_4, this);

	for(int i=0; i<MAX_MENU02_STC_DATA_DUO; i++)
	{
		if(i!=0 && i!=1 && i!=11)
		{
			myStcData2[i].SetFontName(_T("Arial"));
			if(i==2)
				myStcData2[i].SetFontSize(22);
			else
				myStcData2[i].SetFontSize(14);
			myStcData2[i].SetFontBold(TRUE);
			myStcData2[i].SetTextColor(RGB_BLACK);
			myStcData2[i].SetBkColor(RGB_WHITE);
		}
	}

//	myStcData2[6].EnableWindow(FALSE); // IDC_STC_CAM_MK_OFFSET_Y
}

LRESULT CDlgMenu02::OnDrawPcsImg(WPARAM wPara, LPARAM lPara)
{
// 	if(m_pPcsGL)
// 	{
// 		m_pPcsGL->Draw();
// 	}
	return 0L;
}

LRESULT CDlgMenu02::OnDrawPinImg(WPARAM wPara, LPARAM lPara)
{
// 	if(m_pPinGL)
// 	{
// 		m_pPinGL->Draw();
// 	}
	return 0L;
}

LRESULT CDlgMenu02::OnMyStaticRedraw(WPARAM wPara, LPARAM lPara)
{
	int nCtrlID = (int)lPara;
	
	switch(nCtrlID)
	{
// 	case IDC_STC_PCS_IMG:
// 		this->PostMessage(WM_DRAW_PCSIMG, 0, 0);
// 		break;
	case IDC_STC_PIN_IMG:
		this->PostMessage(WM_DRAW_PINIMG, 0, 0);
		break;
	}

	return 0L;
}


void CDlgMenu02::SetLight(int nVal)
{
	if(pView->m_pLight)
	{
		pView->m_pLight->Set(_tstoi(pDoc->WorkingInfo.Light.sCh[0]), nVal);

		if(nVal)
		{
			if(!myBtn[0].GetCheck())
				myBtn[0].SetCheck(TRUE);
		}
		else
		{
			if(myBtn[0].GetCheck())
				myBtn[0].SetCheck(FALSE);
		}

		nVal = pView->m_pLight->Get(_tstoi(pDoc->WorkingInfo.Light.sCh[0]));

		int nPos;
		nPos = int(100.0*(1.0 - (nVal-0.5)/255.0));
		m_LightSlider.SetPos(nPos);
		CString str;
		str.Format(_T("%d"), nVal);
		myStcData[2].SetText(str);

		pDoc->WorkingInfo.Light.sVal[0] = str;
		::WritePrivateProfileString(_T("Light0"), _T("LIGHT_VALUE"), str, PATH_WORKING_INFO);		
	}
}

void CDlgMenu02::SetLight2(int nVal)
{
	if(pView->m_pLight)
	{
		pView->m_pLight->Set(_tstoi(pDoc->WorkingInfo.Light.sCh[1]), nVal);

		if(nVal)
		{
			if(!myBtn2[0].GetCheck())
				myBtn2[0].SetCheck(TRUE);
		}
		else
		{
			if(myBtn2[0].GetCheck())
				myBtn2[0].SetCheck(FALSE);
		}

		nVal = pView->m_pLight->Get(_tstoi(pDoc->WorkingInfo.Light.sCh[1]));

		int nPos;
		nPos = int(100.0*(1.0 - (nVal-0.5)/255.0));
		m_LightSlider2.SetPos(nPos);
		CString str;
		str.Format(_T("%d"), nVal);
		myStcData2[2].SetText(str);

		pDoc->WorkingInfo.Light.sVal[1] = str;
		::WritePrivateProfileString(_T("Light1"), _T("LIGHT_VALUE"), str, PATH_WORKING_INFO);		
	}
}

void CDlgMenu02::ResetLight()
{
	if(pView->m_pLight)
	{
		if(myBtn[0].GetCheck())
			myBtn[0].SetCheck(FALSE);

		pView->m_pLight->Reset(_tstoi(pDoc->WorkingInfo.Light.sCh[0]));
	}
}

void CDlgMenu02::ResetLight2()
{
	if(pView->m_pLight)
	{
		if(myBtn2[0].GetCheck())
			myBtn2[0].SetCheck(FALSE);

		pView->m_pLight->Reset(_tstoi(pDoc->WorkingInfo.Light.sCh[1]));
	}
}

// void CDlgMenu02::InitResMeas()
// {	
// 	myBtn[17].SetCheck(FALSE);	//	IDC_CHK_RES_POS_ST
// 	myBtn[18].SetCheck(FALSE);	//	IDC_CHK_RES_POS_SET
// 	myBtn[19].SetCheck(FALSE);	//	IDC_CHK_RES_POS_ED
// 
// 	myBtn[17].EnableWindow(TRUE);
// 	myBtn[18].EnableWindow(FALSE);
// 	myBtn[19].EnableWindow(FALSE);
// }

// void CDlgMenu02::SetResMeas(int nStep)
// {
// 	myBtn[17].SetCheck(FALSE);	//	IDC_CHK_RES_POS_ST
// 	myBtn[18].SetCheck(FALSE);	//	IDC_CHK_RES_POS_SET
// 	myBtn[19].SetCheck(FALSE);	//	IDC_CHK_RES_POS_ED
// 
// 	CString sMsg;
// 
// 	switch(nStep)
// 	{
// 	case 0:
// 		myBtn[17].SetCheck(TRUE);	//	IDC_CHK_RES_POS_ST
// 		sMsg.Format(_T("해상도 측정을 하시겠습니까?"));
// 		if(IDNO == pView->MyMsgBox(sMsg, MB_YESNO))
// 			return;		
// 		if(pView->m_pVision[0])
// 		{
// 			myBtn[17].EnableWindow(FALSE);
// 			pView->m_pVision[0]->ClearOverlay();
// 			pView->m_pVision[0]->DrawRect(120, 90, 100);
// 			myBtn[18].EnableWindow(TRUE);
// 		}
// 		break;
// 	case 1:
// 		if(pView->m_pVision[0])
// 		{
// 			pView->m_pVision[0]->ClearOverlay();
// 			myBtn[17].EnableWindow(FALSE);
// 			pView->m_pVision[0]->DrawRect(120, 90, 100);
// 			myBtn[18].EnableWindow(TRUE);
// 		}
// 		break;
// 	case 2:
// 		break;
// 	}
// }

int CDlgMenu02::GetJogSpd()
{
	return m_nSpd;
}

void CDlgMenu02::SetJogSpd()
{
	SetJogSpd(m_nSpd);
}

void CDlgMenu02::SetJogSpd2()
{
	SetJogSpd2(m_nSpd);
}

void CDlgMenu02::SetJogSpd(int nSpd)
{
	double dSpdX, dAccX, dSpdY, dAccY;

	m_nSpd = nSpd;

	myBtn[10].SetCheck(FALSE);	//	IDC_CHK_JOG_VF
	myBtn[11].SetCheck(FALSE);	//	IDC_CHK_JOG_F
	myBtn[12].SetCheck(FALSE);	//	IDC_CHK_JOG_N
	myBtn[13].SetCheck(FALSE);	//	IDC_CHK_JOG_S
	myBtn[14].SetCheck(FALSE);	//	IDC_CHK_JOG_VS

	switch(nSpd)
	{
	case 0: // VS
		myBtn[14].SetCheck(TRUE);	//	IDC_CHK_JOG_VS
		if(pView->m_pMotion)
		{
			dSpdY = pView->m_pMotion->m_pParamMotion[MS_Y0].Speed.fJogLowSpd / 150.0;
			dAccY = pView->m_pMotion->m_pParamMotion[MS_Y0].Speed.fJogAcc / 150.0;
			dSpdX = pView->m_pMotion->m_pParamMotion[MS_X0].Speed.fJogLowSpd / 150.0;
			dAccX = pView->m_pMotion->m_pParamMotion[MS_X0].Speed.fJogAcc / 150.0;
		}
		break;
	case 1: // S
		myBtn[13].SetCheck(TRUE);	//	IDC_CHK_JOG_S
		if(pView->m_pMotion)
		{
			dSpdY = pView->m_pMotion->m_pParamMotion[MS_Y0].Speed.fJogLowSpd / 10.0;
			dAccY = pView->m_pMotion->m_pParamMotion[MS_Y0].Speed.fJogAcc / 10.0;
			dSpdX = pView->m_pMotion->m_pParamMotion[MS_X0].Speed.fJogLowSpd / 10.0;
			dAccX = pView->m_pMotion->m_pParamMotion[MS_X0].Speed.fJogAcc / 10.0;
		}
		break;
	case 2: // N
		myBtn[12].SetCheck(TRUE);	//	IDC_CHK_JOG_N
		if(pView->m_pMotion)
		{
			dSpdY = pView->m_pMotion->m_pParamMotion[MS_Y0].Speed.fJogLowSpd;
			dAccY = pView->m_pMotion->m_pParamMotion[MS_Y0].Speed.fJogAcc;
			dSpdX = pView->m_pMotion->m_pParamMotion[MS_X0].Speed.fJogLowSpd;
			dAccX = pView->m_pMotion->m_pParamMotion[MS_X0].Speed.fJogAcc;
		}
		break;
	case 3: // F
		myBtn[11].SetCheck(TRUE);	//	IDC_CHK_JOG_F
		if(pView->m_pMotion)
		{
			dSpdY = pView->m_pMotion->m_pParamMotion[MS_Y0].Speed.fJogMidSpd;
			dAccY = pView->m_pMotion->m_pParamMotion[MS_Y0].Speed.fJogAcc;
			dSpdX = pView->m_pMotion->m_pParamMotion[MS_X0].Speed.fJogMidSpd;
			dAccX = pView->m_pMotion->m_pParamMotion[MS_X0].Speed.fJogAcc;
		}
		break;
	case 4: // VF
		myBtn[10].SetCheck(TRUE);	//	IDC_CHK_JOG_VF
		if(pView->m_pMotion)
		{
			dSpdY = pView->m_pMotion->m_pParamMotion[MS_Y0].Speed.fJogFastSpd;
			dAccY = pView->m_pMotion->m_pParamMotion[MS_Y0].Speed.fJogAcc;
			dSpdX = pView->m_pMotion->m_pParamMotion[MS_X0].Speed.fJogFastSpd;
			dAccX = pView->m_pMotion->m_pParamMotion[MS_X0].Speed.fJogAcc;
		}
		break;
	}

	if(pView->m_pMotion)
	{
		pView->m_pMotion->SetVMove(MS_Y0, dSpdY, dAccY);
//		if(!pView->ChkCollision())
			pView->m_pMotion->SetVMove(MS_X0, dSpdX, dAccX);
		m_nJogSpd = nSpd;
	}
}

void CDlgMenu02::SetJogSpd2(int nSpd)
{
	double dSpdX, dAccX, dSpdY, dAccY;

	m_nSpd = nSpd;

	myBtn2[10].SetCheck(FALSE);	//	IDC_CHK_JOG_VF2
	myBtn2[11].SetCheck(FALSE);	//	IDC_CHK_JOG_F2
	myBtn2[12].SetCheck(FALSE);	//	IDC_CHK_JOG_N2
	myBtn2[13].SetCheck(FALSE);	//	IDC_CHK_JOG_S2
	myBtn2[14].SetCheck(FALSE);	//	IDC_CHK_JOG_VS2

	switch(nSpd)
	{
	case 0: // VS
		myBtn2[14].SetCheck(TRUE);	//	IDC_CHK_JOG_VS22
		if(pView->m_pMotion)
		{
			dSpdY = pView->m_pMotion->m_pParamMotion[MS_Y1].Speed.fJogLowSpd / 150.0;
			dAccY = pView->m_pMotion->m_pParamMotion[MS_Y1].Speed.fJogAcc / 150.0;
			dSpdX = pView->m_pMotion->m_pParamMotion[MS_X1].Speed.fJogLowSpd / 150.0;
			dAccX = pView->m_pMotion->m_pParamMotion[MS_X1].Speed.fJogAcc / 150.0;
		}
		break;
	case 1: // S
		myBtn2[13].SetCheck(TRUE);	//	IDC_CHK_JOG_S2
		if(pView->m_pMotion)
		{
			dSpdY = pView->m_pMotion->m_pParamMotion[MS_Y1].Speed.fJogLowSpd / 10.0;
			dAccY = pView->m_pMotion->m_pParamMotion[MS_Y1].Speed.fJogAcc / 10.0;
			dSpdX = pView->m_pMotion->m_pParamMotion[MS_X1].Speed.fJogLowSpd / 10.0;
			dAccX = pView->m_pMotion->m_pParamMotion[MS_X1].Speed.fJogAcc / 10.0;
		}
		break;
	case 2: // N
		myBtn2[12].SetCheck(TRUE);	//	IDC_CHK_JOG_N2
		if(pView->m_pMotion)
		{
			dSpdY = pView->m_pMotion->m_pParamMotion[MS_Y1].Speed.fJogLowSpd;
			dAccY = pView->m_pMotion->m_pParamMotion[MS_Y1].Speed.fJogAcc;
			dSpdX = pView->m_pMotion->m_pParamMotion[MS_X1].Speed.fJogLowSpd;
			dAccX = pView->m_pMotion->m_pParamMotion[MS_X1].Speed.fJogAcc;
		}
		break;
	case 3: // F
		myBtn2[11].SetCheck(TRUE);	//	IDC_CHK_JOG_F2
		if(pView->m_pMotion)
		{
			dSpdY = pView->m_pMotion->m_pParamMotion[MS_Y1].Speed.fJogMidSpd;
			dAccY = pView->m_pMotion->m_pParamMotion[MS_Y1].Speed.fJogAcc;
			dSpdX = pView->m_pMotion->m_pParamMotion[MS_X1].Speed.fJogMidSpd;
			dAccX = pView->m_pMotion->m_pParamMotion[MS_X1].Speed.fJogAcc;
		}
		break;
	case 4: // VF
		myBtn2[10].SetCheck(TRUE);	//	IDC_CHK_JOG_VF2
		if(pView->m_pMotion)
		{
			dSpdY = pView->m_pMotion->m_pParamMotion[MS_Y1].Speed.fJogFastSpd;
			dAccY = pView->m_pMotion->m_pParamMotion[MS_Y1].Speed.fJogAcc;
			dSpdX = pView->m_pMotion->m_pParamMotion[MS_X1].Speed.fJogFastSpd;
			dAccX = pView->m_pMotion->m_pParamMotion[MS_X1].Speed.fJogAcc;
		}
		break;
	}

	if(pView->m_pMotion)
	{
		pView->m_pMotion->SetVMove(MS_Y1, dSpdY, dAccY);
//		if(!pView->ChkCollision())
			pView->m_pMotion->SetVMove(MS_X1, dSpdX, dAccX);
		m_nJogSpd = nSpd;
	}
}

void CDlgMenu02::OnChkLtOn() 
{
	// TODO: Add your control notification handler code here
	BOOL bOn = myBtn[0].GetCheck();
	if(bOn)
	{
		myBtn[1].EnableWindow(TRUE);
		GetDlgItem(IDC_SLD_LT)->EnableWindow(TRUE);
		myBtn[2].EnableWindow(TRUE);

		SetLight();
	}
	else
	{
		ResetLight();
		
		myBtn[1].EnableWindow(FALSE);
		GetDlgItem(IDC_SLD_LT)->EnableWindow(FALSE);
		myBtn[2].EnableWindow(FALSE);
	}
}

void CDlgMenu02::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	CString str;
	int LightValue;
	if(pScrollBar == (CScrollBar*)&m_LightSlider)
	{
		nPos = m_LightSlider.GetPos();
		str = myStcData[2].GetText();
		LightValue = (int)floor(((double)(100-nPos)/100.0)*255.0+0.5);
		pView->m_pLight->Set(_tstoi(pDoc->WorkingInfo.Light.sCh[0]), LightValue);
		str.Format(_T("%d"),LightValue);
		myStcData[2].SetText(str);

		pDoc->WorkingInfo.Light.sVal[0] = str;
		::WritePrivateProfileString(_T("Light0"), _T("LIGHT_VALUE"), str, PATH_WORKING_INFO);		
	}
	if(pScrollBar == (CScrollBar*)&m_LightSlider2)
	{
		nPos = m_LightSlider2.GetPos();
		str = myStcData2[2].GetText();
		LightValue = (int)floor(((double)(100-nPos)/100.0)*255.0+0.5);
		pView->m_pLight->Set(_tstoi(pDoc->WorkingInfo.Light.sCh[1]), LightValue);
		str.Format(_T("%d"),LightValue);
		myStcData2[2].SetText(str);

		pDoc->WorkingInfo.Light.sVal[1] = str;
		::WritePrivateProfileString(_T("Light1"), _T("LIGHT_VALUE"), str, PATH_WORKING_INFO);		
	}
	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

LRESULT CDlgMenu02::OnMyBtnDown(WPARAM wPara, LPARAM lPara)
{
	int nCtrlID = (int)lPara;
	SwMyBtnDown(nCtrlID);

	return 0L;
}

void CDlgMenu02::SwMyBtnDown(int nCtrlID)
{

	if(pView->m_bProbDn[0])
	{
		if(pView->m_pVoiceCoil[0])
		{
			pView->m_pVoiceCoil[0]->SearchHomeSmac(0);
			pView->m_pVoiceCoil[0]->MoveSmacShiftPos(0);
			pView->m_bProbDn[0] = FALSE;
			//if(m_pDlgUtil06)
			//	m_pDlgUtil06->myBtn[2].SetCheck(FALSE);
		}
	}

	if(pView->m_bProbDn[1])
	{
		if(pView->m_pVoiceCoil[1])
		{
			pView->m_pVoiceCoil[1]->SearchHomeSmac(1);
			pView->m_pVoiceCoil[1]->MoveSmacShiftPos(1);
			pView->m_bProbDn[1] = FALSE;
			//if(m_pDlgUtil06)
			//	m_pDlgUtil06->myBtn[6].SetCheck(FALSE);
		}
	}

	switch(nCtrlID)
	{
	case IDC_BTN_LT_UP:
		if(!m_bTIM_LIGHT_UP)
		{
			m_bTIM_LIGHT_UP = TRUE;
			SetTimer(TIM_LIGHT_UP, 100, NULL);
		}
		break;
	case IDC_BTN_LT_DN:
		if(!m_bTIM_LIGHT_DN)
		{
			m_bTIM_LIGHT_DN = TRUE;
			SetTimer(TIM_LIGHT_DN, 100, NULL);
		}
		break;
	case IDC_BTN_LT_UP2:
		if(!m_bTIM_LIGHT_UP2)
		{
			m_bTIM_LIGHT_UP2 = TRUE;
			SetTimer(TIM_LIGHT_UP2, 100, NULL);
		}
		break;
	case IDC_BTN_LT_DN2:
		if(!m_bTIM_LIGHT_DN2)
		{
			m_bTIM_LIGHT_DN2 = TRUE;
			SetTimer(TIM_LIGHT_DN2, 100, NULL);
		}
		break;
	case IDC_BTN_BUFF_UP:		// 마킹부 피딩 정회전 스위치
#ifdef USE_MPE
		if(pView->m_pMpe)
			pView->m_pMpe->Write(_T("MB005513"), 1);
#endif
		break;
	case IDC_BTN_BUFF_DN:		// 마킹부 피딩 역회전 스위치
#ifdef USE_MPE
		if(pView->m_pMpe)
			pView->m_pMpe->Write(_T("MB005514"), 1);
#endif
		break;
	case IDC_BTN_JOG_UP:
		if(pView->m_pMotion)
			pView->m_pMotion->VMove(MS_Y0, M_CCW);
		break;
	case IDC_BTN_JOG_DN:
		if(pView->m_pMotion)
			pView->m_pMotion->VMove(MS_Y0, M_CW);
		break;
	case IDC_BTN_JOG_RT:
		if(pView->m_pMotion)
			pView->m_pMotion->VMove(MS_X0, M_CW);
		break;
	case IDC_BTN_JOG_LF:
		if(pView->m_pMotion)
			pView->m_pMotion->VMove(MS_X0, M_CCW);
		break;
	case IDC_BTN_JOG_UP2:
		if(pView->m_pMotion)
			pView->m_pMotion->VMove(MS_Y1, M_CCW);
		break;
	case IDC_BTN_JOG_DN2:
		if(pView->m_pMotion)
			pView->m_pMotion->VMove(MS_Y1, M_CW);
		break;
	case IDC_BTN_JOG_RT2:
		if(pView->m_pMotion)
			pView->m_pMotion->VMove(MS_X1, M_CW);
		break;
	case IDC_BTN_JOG_LF2:
		if(pView->m_pMotion)
			pView->m_pMotion->VMove(MS_X1, M_CCW);
		break;
	}
}

LRESULT CDlgMenu02::OnMyBtnUp(WPARAM wPara, LPARAM lPara)
{
	int nCtrlID = (int)lPara;
	SwMyBtnUp(nCtrlID);
	return 0L;
}

void CDlgMenu02::SwMyBtnUp(int nCtrlID)
{
	switch(nCtrlID)
	{
	case IDC_BTN_LT_UP:
		m_bTIM_LIGHT_UP = FALSE;
		break;
	case IDC_BTN_LT_DN:
		m_bTIM_LIGHT_DN = FALSE;
		break;
	case IDC_BTN_LT_UP2:
		m_bTIM_LIGHT_UP2 = FALSE;
		break;
	case IDC_BTN_LT_DN2:
		m_bTIM_LIGHT_DN2 = FALSE;
		break;
	case IDC_BTN_BUFF_UP:		// 마킹부 피딩 정회전 스위치
#ifdef USE_MPE
		if(pView->m_pMpe)
			pView->m_pMpe->Write(_T("MB005513"), 0);
#endif
		break;
	case IDC_BTN_BUFF_DN:		// 마킹부 피딩 역회전 스위치
#ifdef USE_MPE
		if(pView->m_pMpe)
			pView->m_pMpe->Write(_T("MB005514"), 0);
#endif
		break;
	case IDC_BTN_JOG_UP:
	case IDC_BTN_JOG_DN:
		if(pView->m_pMotion)
		{
			pView->m_pMotion->EStop(MS_Y0);
// 			MsClr(MS_Y);
			ResetMotion(MS_Y0);
		}
		break;
	case IDC_BTN_JOG_RT:
	case IDC_BTN_JOG_LF:
		if(pView->m_pMotion)
		{
			pView->m_pMotion->EStop(MS_X0);
// 			MsClr(MS_X);
			ResetMotion(MS_X0);
		}
		break;
	case IDC_BTN_JOG_UP2:
	case IDC_BTN_JOG_DN2:
		if(pView->m_pMotion)
		{
			pView->m_pMotion->EStop(MS_Y1);
// 			MsClr(MS_Y);
			ResetMotion(MS_Y1);
		}
		break;
	case IDC_BTN_JOG_RT2:
	case IDC_BTN_JOG_LF2:
		if(pView->m_pMotion)
		{
			pView->m_pMotion->EStop(MS_X1);
// 			MsClr(MS_X);
			ResetMotion(MS_X1);
		}
		break;
	}
}
	
void CDlgMenu02::ResetMotion(int nMsId)
{
	if(!pView->m_pMotion)
		return;

	long lRtn = pView->m_pMotion->GetState(nMsId);  // -1 : MPIStateERROR, 0 : MPIStateIDLE, 1 : MPIStateSTOPPING, 2 : MPIStateMOVING
	if(lRtn==2)
	{
		if(nMsId==MS_X0 || nMsId==MS_Y0)
			pView->m_pMotion->Abort(MS_X0Y0);
		else if(nMsId==MS_X1 || nMsId==MS_Y1)
			pView->m_pMotion->Abort(MS_X1Y1);
		else
			pView->m_pMotion->Abort(nMsId);
		Sleep(30);
	}

	if(nMsId==MS_X0 || nMsId==MS_Y0)
		pView->m_pMotion->Clear(MS_X0Y0);
	else if(nMsId==MS_X1 || nMsId==MS_Y1)
		pView->m_pMotion->Clear(MS_X1Y1);
	else
		pView->m_pMotion->Clear(nMsId);

	Sleep(30);

	if(!pView->m_pMotion->IsEnable(nMsId))
	{
		if(nMsId==MS_X0Y0 || nMsId==MS_X0 || nMsId==MS_Y0)
		{
			pView->m_pMotion->ServoOnOff(AXIS_X0, TRUE);
			Sleep(30);
			pView->m_pMotion->ServoOnOff(AXIS_Y0, TRUE);
		}
		else if(nMsId==MS_X1Y1 || nMsId==MS_X1 || nMsId==MS_Y1)
		{
			pView->m_pMotion->ServoOnOff(AXIS_X1, TRUE);
			Sleep(30);
			pView->m_pMotion->ServoOnOff(AXIS_Y1, TRUE);
		}
		else
			pView->m_pMotion->ServoOnOff(nMsId, TRUE);
	
		Sleep(30);
	}
}

void CDlgMenu02::MsClr(int nMsId)
{
	if(pView->m_pMotion)
	{
		Sleep(30);

		long lRtn = pView->m_pMotion->GetState(nMsId);  // -1 : MPIStateERROR, 0 : MPIStateIDLE, 1 : MPIStateSTOPPING, 2 : MPIStateMOVING
		if(lRtn==2)//lRtn<0 || 
		{
			if(nMsId==MS_X0 || nMsId==MS_Y0)
				pView->m_pMotion->Abort(MS_X0Y0);
			else if(nMsId==MS_X1 || nMsId==MS_Y1)
				pView->m_pMotion->Abort(MS_X1Y1);
			else
				pView->m_pMotion->Abort(nMsId);
			Sleep(30);
		}

		if(nMsId==MS_X0 || nMsId==MS_Y0)
			pView->m_pMotion->Clear(MS_X0Y0);
		else if(nMsId==MS_X1 || nMsId==MS_Y1)
			pView->m_pMotion->Clear(MS_X1Y1);
		else
			pView->m_pMotion->Clear(nMsId);
		Sleep(30);

		if(lRtn==2)//lRtn<0 || 
		{
			if(nMsId==MS_X0Y0 || nMsId==MS_X0 || nMsId==MS_Y0)
			{
				pView->m_pMotion->ServoOnOff(AXIS_X0, TRUE);
				Sleep(30);
				pView->m_pMotion->ServoOnOff(AXIS_Y0, TRUE);
			}
			else if(nMsId==MS_X1Y1 || nMsId==MS_X1 || nMsId==MS_Y1)
			{
				pView->m_pMotion->ServoOnOff(AXIS_X1, TRUE);
				Sleep(30);
				pView->m_pMotion->ServoOnOff(AXIS_Y1, TRUE);
			}
			else
				pView->m_pMotion->ServoOnOff(nMsId, TRUE);
		
			Sleep(30);
		}
	}
}

void CDlgMenu02::OnTimer(UINT_PTR nIDEvent)//(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(nIDEvent);

	int nVal;
	if(!m_bLockTimer)
	{
		m_bLockTimer = TRUE;
		switch(nIDEvent)
		{
		case TIM_LIGHT_UP:
	//		KillTimer(TIM_LIGHT_UP);
			nVal = pView->m_pLight->Get(_tstoi(pDoc->WorkingInfo.Light.sCh[0]));
			nVal += 5;
			if(nVal > 255)
				nVal = 255;
			SetLight(nVal);
			if(m_bTIM_LIGHT_UP)
				SetTimer(TIM_LIGHT_UP, 100, NULL);
			break;
		case TIM_LIGHT_DN:
	//		KillTimer(TIM_LIGHT_DN);
			nVal = pView->m_pLight->Get(_tstoi(pDoc->WorkingInfo.Light.sCh[0]));
			nVal -= 5;
			if(nVal < 0)
				nVal = 0;
			SetLight(nVal);
			if(m_bTIM_LIGHT_DN)
				SetTimer(TIM_LIGHT_DN, 100, NULL);
			break;
		case TIM_LIGHT_UP2:
	//		KillTimer(TIM_LIGHT_UP2);
			nVal = pView->m_pLight->Get(_tstoi(pDoc->WorkingInfo.Light.sCh[1]));
			nVal += 5;
			if(nVal > 255)
				nVal = 255;
			SetLight2(nVal);
			if(m_bTIM_LIGHT_UP2)
				SetTimer(TIM_LIGHT_UP2, 100, NULL);
			break;
		case TIM_LIGHT_DN2:
	//		KillTimer(TIM_LIGHT_DN2);
			nVal = pView->m_pLight->Get(_tstoi(pDoc->WorkingInfo.Light.sCh[1]));
			nVal -= 5;
			if(nVal < 0)
				nVal = 0;
			SetLight2(nVal);
			if(m_bTIM_LIGHT_DN2)
				SetTimer(TIM_LIGHT_DN2, 100, NULL);
			break;

		case TIM_BUF_ENC:
	//		KillTimer(TIM_BUF_ENC);
			DispAxisPos();
			//DispCenterMark();
			
	// 		DispBufEnc();
			if(m_bTIM_BUF_ENC)
				SetTimer(TIM_BUF_ENC, 100, NULL);
			break;
		case TIM_MARKING_OFF:
	//		KillTimer(TIM_MARKING_OFF);
			MarkingOff();
			break;
		case TIM_MARKING2_OFF:
	//		KillTimer(TIM_MARKING2_OFF);
			MarkingOff2();
			break;
		case TIM_SHOW_MK_TEST:
	//		KillTimer(TIM_SHOW_MK_TEST);
			if(m_lChk == 1)
			{
				if(myBtn[16].GetCheck())
				{
					myBtn[16].SetCheck(FALSE);
					//((CButton*)GetDlgItem(IDC_CHK_MARKING_TEST2))->SetCheck(FALSE);	
					ChkElecTest();
				}
				ChkMarkingTest2();
			}
			break;
		case TIM_SHOW_ELEC_TEST:
//			KillTimer(TIM_SHOW_ELEC_TEST);
			if(m_lChk == 2)
			{
				if(myBtn2[16].GetCheck())
				{
					myBtn2[16].SetCheck(FALSE);
					//((CButton*)GetDlgItem(IDC_CHK_MARKING_TEST2))->SetCheck(FALSE);	
					ChkMarkingTest2();
				}
				ChkElecTest();
			}
			break;
		}

		m_bLockTimer = FALSE;
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CDlgMenu02::OnChkJogVf() 
{
	// TODO: Add your control notification handler code here
	SetJogSpd(4);
}

void CDlgMenu02::OnChkJogVf2() 
{
	// TODO: Add your control notification handler code here
	SetJogSpd2(4);	
}

void CDlgMenu02::OnChkJogF() 
{
	// TODO: Add your control notification handler code here
	SetJogSpd(3);
}

void CDlgMenu02::OnChkJogF2() 
{
	// TODO: Add your control notification handler code here
	SetJogSpd2(3);	
}

void CDlgMenu02::OnChkJogN() 
{
	// TODO: Add your control notification handler code here
	SetJogSpd(2);
}

void CDlgMenu02::OnChkJogN2() 
{
	// TODO: Add your control notification handler code here
	SetJogSpd2(2);	
}

void CDlgMenu02::OnChkJogS() 
{
	// TODO: Add your control notification handler code here
	SetJogSpd(1);
}

void CDlgMenu02::OnChkJogS2() 
{
	// TODO: Add your control notification handler code here
	SetJogSpd2(1);	
}

void CDlgMenu02::OnChkJogVs() 
{
	// TODO: Add your control notification handler code here
	SetJogSpd(0);
}

void CDlgMenu02::OnChkJogVs2() 
{
	// TODO: Add your control notification handler code here
	SetJogSpd2(0);	
}

void CDlgMenu02::OnBtnPinMove() 
{
	// TODO: Add your control notification handler code here
	MovePinPos();
}

void CDlgMenu02::OnBtnPinMove2() 
{
	// TODO: Add your control notification handler code here
	MovePinPos2();	
}

BOOL CDlgMenu02::MovePos(int nPos) 
{
	if(!pView->m_pMotion)
		return FALSE;

	if(pView->ChkCollision(AXIS_X0, _tstof(pDoc->WorkingInfo.Marking[0].sMeasurePosX[nPos])))
	{
		CfPoint ptPnt;
		ptPnt.x = _tstof(pDoc->WorkingInfo.Motion.sSafeZone);
		ptPnt.y = 0.0;//pView->m_dEnc[AXIS_Y1];
		pView->Move1(ptPnt, pView->m_bCam);
	}

	if(_tstof(pDoc->WorkingInfo.Marking[0].sMeasurePosX[nPos]) > 0.0 && _tstof(pDoc->WorkingInfo.Marking[0].sMeasurePosY[nPos]) > 0.0)
	{
		double dCurrX = pView->m_dEnc[AXIS_X0];
		double dCurrY = pView->m_dEnc[AXIS_X0];

		double pPos[2];
		pPos[0] = _tstof(pDoc->WorkingInfo.Marking[0].sMeasurePosX[nPos]);
		pPos[1] = _tstof(pDoc->WorkingInfo.Marking[0].sMeasurePosY[nPos]);

		double fLen, fVel, fAcc, fJerk;
		fLen = sqrt( ((pPos[0] - dCurrX) * (pPos[0] - dCurrX)) + ((pPos[1] - dCurrY) * (pPos[1] - dCurrY)) );
		if(fLen > 0.001)
		{
			pView->m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X0, fLen, fVel, fAcc, fJerk);
			if(!pView->m_pMotion->Move(MS_X0Y0, pPos, fVel, fAcc, fAcc))
			{
				if(!pView->m_pMotion->Move(MS_X0Y0, pPos, fVel, fAcc, fAcc))
					AfxMessageBox(_T("Move XY Error..."));
			}
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CDlgMenu02::MovePos2(int nPos) 
{
	if(!pView->m_pMotion)
		return FALSE;

	if(pView->ChkCollision(AXIS_X1, _tstof(pDoc->WorkingInfo.Marking[1].sMeasurePosX[nPos])))
	{
		CfPoint ptPnt;
		ptPnt.x = 0.0;
		ptPnt.y = 0.0;//pView->m_dEnc[AXIS_Y0];
		pView->Move0(ptPnt, pView->m_bCam);
	}

	if(_tstof(pDoc->WorkingInfo.Marking[1].sMeasurePosX[nPos]) > 0.0 && _tstof(pDoc->WorkingInfo.Marking[1].sMeasurePosY[nPos]) > 0.0)
	{
		double dCurrX = pView->m_dEnc[AXIS_X1];
		double dCurrY = pView->m_dEnc[AXIS_X1];

		double pPos[2];
		pPos[0] = _tstof(pDoc->WorkingInfo.Marking[1].sMeasurePosX[nPos]);
		pPos[1] = _tstof(pDoc->WorkingInfo.Marking[1].sMeasurePosY[nPos]);

		double fLen, fVel, fAcc, fJerk;
		fLen = sqrt( ((pPos[0] - dCurrX) * (pPos[0] - dCurrX)) + ((pPos[1] - dCurrY) * (pPos[1] - dCurrY)) );
		if(fLen > 0.001)
		{
			pView->m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X1, fLen, fVel, fAcc, fJerk);
			if(!pView->m_pMotion->Move(MS_X1Y1, pPos, fVel, fAcc, fAcc))
			{
				if(!pView->m_pMotion->Move(MS_X1Y1, pPos, fVel, fAcc, fAcc))
					AfxMessageBox(_T("Move XY Error..."));
			}
		}

		return TRUE;
	}

	return FALSE;
}


BOOL CDlgMenu02::MovePinPos() 
{
	if(!pView->m_pMotion)
		return FALSE;

	SetLight();

	if(pView->m_pMotion->m_dPinPosY[0] > 0.0 && pView->m_pMotion->m_dPinPosX[0] > 0.0)
	{
		double dCurrX = pView->m_dEnc[AXIS_X0];
		double dCurrY = pView->m_dEnc[AXIS_Y0];

		double pPos[2];
		pPos[0] = pView->m_pMotion->m_dPinPosX[0];
		pPos[1] = pView->m_pMotion->m_dPinPosY[0];

		if(pView->ChkCollision(AXIS_X0, pPos[0]))
		{
			CfPoint ptPnt;
			ptPnt.x =  _tstof(pDoc->WorkingInfo.Motion.sSafeZone);
			ptPnt.y = 0.0;//pView->m_dEnc[AXIS_Y1];
			pView->Move1(ptPnt);
		}

		double fLen, fVel, fAcc, fJerk;
		fLen = sqrt( ((pPos[0] - dCurrX) * (pPos[0] - dCurrX)) + ((pPos[1] - dCurrY) * (pPos[1] - dCurrY)) );
		if(fLen > 0.001)
		{
			pView->m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X0, fLen, fVel, fAcc, fJerk);
			if(!pView->m_pMotion->Move(MS_X0Y0, pPos, fVel, fAcc, fAcc))
			{
				if(!pView->m_pMotion->Move(MS_X0Y0, pPos, fVel, fAcc, fAcc))
					AfxMessageBox(_T("Move XY Error..."));
			}
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CDlgMenu02::MovePinPos2() 
{
	if(!pView->m_pMotion)
		return FALSE;

	SetLight2();

	if(pView->m_pMotion->m_dPinPosY[1] > 0.0 && pView->m_pMotion->m_dPinPosX[1] > 0.0)
	{
		double dCurrX = pView->m_dEnc[AXIS_X1];
		double dCurrY = pView->m_dEnc[AXIS_Y1];

		double pPos[2];
		pPos[0] = pView->m_pMotion->m_dPinPosX[1];
		pPos[1] = pView->m_pMotion->m_dPinPosY[1];

		if(pView->ChkCollision(AXIS_X1, pPos[0]))
		{
			CfPoint ptPnt;
			ptPnt.x =  0.0;
			ptPnt.y = 0.0;//pView->m_dEnc[AXIS_Y0];
			pView->Move0(ptPnt);
		}

		double fLen, fVel, fAcc, fJerk;
		fLen = sqrt( ((pPos[0] - dCurrX) * (pPos[0] - dCurrX)) + ((pPos[1] - dCurrY) * (pPos[1] - dCurrY)) );
		if(fLen > 0.001)
		{
			pView->m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X1, fLen, fVel, fAcc, fJerk);
			if(!pView->m_pMotion->Move(MS_X1Y1, pPos, fVel, fAcc, fAcc))
			{
				if(!pView->m_pMotion->Move(MS_X0Y0, pPos, fVel, fAcc, fAcc))
					AfxMessageBox(_T("Move XY Error..."));
			}
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CDlgMenu02::MoveAlign0(int nPos) 
{
	if(!pView->m_pMotion)
		return FALSE;

	SetLight();

	if (pDoc->WorkingInfo.LastJob.nAlignMethode == FOUR_POINT)
		return Move4PntAlign0(nPos);

	return Move2PntAlign0(nPos);
}

BOOL CDlgMenu02::Move2PntAlign0(int nPos)
{
	if (pView->m_pMotion->m_dPinPosY[0] > 0.0 && pView->m_pMotion->m_dPinPosX[0] > 0.0)
	{
		double dCurrX = pView->m_dEnc[AXIS_X0];
		double dCurrY = pView->m_dEnc[AXIS_Y0];

		double pPos[2];
		if (nPos == 0)
		{
			pPos[0] = pDoc->m_Master[0].m_stAlignMk.X0 + pView->m_pMotion->m_dPinPosX[0];
			pPos[1] = pDoc->m_Master[0].m_stAlignMk.Y0 + pView->m_pMotion->m_dPinPosY[0];
		}
		else if (nPos == 1)
		{
			pPos[0] = pDoc->m_Master[0].m_stAlignMk.X1 + pView->m_pMotion->m_dPinPosX[0];
			pPos[1] = pDoc->m_Master[0].m_stAlignMk.Y1 + pView->m_pMotion->m_dPinPosY[0];
		}

		if (pView->ChkCollision(AXIS_X0, pPos[0]))
		{
			CfPoint ptPnt;
			ptPnt.x = _tstof(pDoc->WorkingInfo.Motion.sSafeZone);
			ptPnt.y = 0.0;//pView->m_dEnc[AXIS_Y1];
			pView->Move1(ptPnt);
		}

		double fLen, fVel, fAcc, fJerk;
		fLen = sqrt(((pPos[0] - dCurrX) * (pPos[0] - dCurrX)) + ((pPos[1] - dCurrY) * (pPos[1] - dCurrY)));
		if (fLen > 0.001)
		{
			pView->m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X0, fLen, fVel, fAcc, fJerk);
			if (!pView->m_pMotion->Move(MS_X0Y0, pPos, fVel, fAcc, fAcc))
			{
				if (!pView->m_pMotion->Move(MS_X0Y0, pPos, fVel, fAcc, fAcc))
					AfxMessageBox(_T("Move XY Error..."));
			}
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CDlgMenu02::Move4PntAlign0(int nPos)
{
	if (pView->m_pMotion->m_dPinPosY[0] > 0.0 && pView->m_pMotion->m_dPinPosX[0] > 0.0)
	{
		double dCurrX = pView->m_dEnc[AXIS_X0];
		double dCurrY = pView->m_dEnc[AXIS_Y0];

		double pPos[2];
		if (nPos == 0)
		{
			pPos[0] = pDoc->m_Master[0].m_stAlignMk2.X0 + pView->m_pMotion->m_dPinPosX[0];
			pPos[1] = pDoc->m_Master[0].m_stAlignMk.Y0 + pView->m_pMotion->m_dPinPosY[0];
		}
		else if (nPos == 1)
		{
			pPos[0] = pDoc->m_Master[0].m_stAlignMk2.X1 + pView->m_pMotion->m_dPinPosX[0];
			pPos[1] = pDoc->m_Master[0].m_stAlignMk2.Y1 + pView->m_pMotion->m_dPinPosY[0];
		}
		else if (nPos == 2)
		{
			pPos[0] = pDoc->m_Master[0].m_stAlignMk2.X2 + pView->m_pMotion->m_dPinPosX[0];
			pPos[1] = pDoc->m_Master[0].m_stAlignMk2.Y2 + pView->m_pMotion->m_dPinPosY[0];
		}
		else if (nPos == 3)
		{
			pPos[0] = pDoc->m_Master[0].m_stAlignMk2.X3 + pView->m_pMotion->m_dPinPosX[0];
			pPos[1] = pDoc->m_Master[0].m_stAlignMk2.Y3 + pView->m_pMotion->m_dPinPosY[0];
		}

		if (pView->ChkCollision(AXIS_X0, pPos[0]))
		{
			CfPoint ptPnt;
			ptPnt.x = _tstof(pDoc->WorkingInfo.Motion.sSafeZone);
			ptPnt.y = 0.0;//pView->m_dEnc[AXIS_Y1];
			pView->Move1(ptPnt);
		}

		double fLen, fVel, fAcc, fJerk;
		fLen = sqrt(((pPos[0] - dCurrX) * (pPos[0] - dCurrX)) + ((pPos[1] - dCurrY) * (pPos[1] - dCurrY)));
		if (fLen > 0.001)
		{
			pView->m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X0, fLen, fVel, fAcc, fJerk);
			if (!pView->m_pMotion->Move(MS_X0Y0, pPos, fVel, fAcc, fAcc))
			{
				if (!pView->m_pMotion->Move(MS_X0Y0, pPos, fVel, fAcc, fAcc))
					AfxMessageBox(_T("Move XY Error..."));
			}
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CDlgMenu02::MoveAlign1(int nPos) 
{
	if(!pView->m_pMotion)
		return FALSE;

	SetLight2();

	if (pDoc->WorkingInfo.LastJob.nAlignMethode == FOUR_POINT)
		return Move4PntAlign1(nPos);

	return Move2PntAlign1(nPos);
}

BOOL CDlgMenu02::Move2PntAlign1(int nPos)
{
	if (pView->m_pMotion->m_dPinPosY[1] > 0.0 && pView->m_pMotion->m_dPinPosX[1] > 0.0)
	{
		double dCurrX = pView->m_dEnc[AXIS_X1];
		double dCurrY = pView->m_dEnc[AXIS_Y1];

		double pPos[2];
		if (nPos == 0)
		{
			pPos[0] = pDoc->m_Master[0].m_stAlignMk.X0 + pView->m_pMotion->m_dPinPosX[1];
			pPos[1] = pDoc->m_Master[0].m_stAlignMk.Y0 + pView->m_pMotion->m_dPinPosY[1];
		}
		else if (nPos == 1)
		{
			pPos[0] = pDoc->m_Master[0].m_stAlignMk.X1 + pView->m_pMotion->m_dPinPosX[1];
			pPos[1] = pDoc->m_Master[0].m_stAlignMk.Y1 + pView->m_pMotion->m_dPinPosY[1];
		}

		if (pView->ChkCollision(AXIS_X1, pPos[0]))
		{
			CfPoint ptPnt;
			ptPnt.x = 0.0;
			ptPnt.y = 0.0;//pView->m_dEnc[AXIS_Y0];
			pView->Move0(ptPnt);

		}

		double fLen, fVel, fAcc, fJerk;
		fLen = sqrt(((pPos[0] - dCurrX) * (pPos[0] - dCurrX)) + ((pPos[1] - dCurrY) * (pPos[1] - dCurrY)));
		if (fLen > 0.001)
		{
			pView->m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X1, fLen, fVel, fAcc, fJerk);
			if (!pView->m_pMotion->Move(MS_X1Y1, pPos, fVel, fAcc, fAcc))
			{
				if (!pView->m_pMotion->Move(MS_X1Y1, pPos, fVel, fAcc, fAcc))
					AfxMessageBox(_T("Move XY Error..."));
			}
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CDlgMenu02::Move4PntAlign1(int nPos)
{
	if (pView->m_pMotion->m_dPinPosY[1] > 0.0 && pView->m_pMotion->m_dPinPosX[1] > 0.0)
	{
		double dCurrX = pView->m_dEnc[AXIS_X1];
		double dCurrY = pView->m_dEnc[AXIS_Y1];

		double pPos[2];
		if (nPos == 0)
		{
			pPos[0] = pDoc->m_Master[0].m_stAlignMk2.X0 + pView->m_pMotion->m_dPinPosX[1];
			pPos[1] = pDoc->m_Master[0].m_stAlignMk2.Y0 + pView->m_pMotion->m_dPinPosY[1];
		}
		else if (nPos == 1)
		{
			pPos[0] = pDoc->m_Master[0].m_stAlignMk2.X1 + pView->m_pMotion->m_dPinPosX[1];
			pPos[1] = pDoc->m_Master[0].m_stAlignMk.Y1 + pView->m_pMotion->m_dPinPosY[1];
		}
		else if (nPos == 2)
		{
			pPos[0] = pDoc->m_Master[0].m_stAlignMk2.X2 + pView->m_pMotion->m_dPinPosX[1];
			pPos[1] = pDoc->m_Master[0].m_stAlignMk2.Y2 + pView->m_pMotion->m_dPinPosY[1];
		}
		else if (nPos == 3)
		{
			pPos[0] = pDoc->m_Master[0].m_stAlignMk2.X3 + pView->m_pMotion->m_dPinPosX[1];
			pPos[1] = pDoc->m_Master[0].m_stAlignMk2.Y3 + pView->m_pMotion->m_dPinPosY[1];
		}

		if (pView->ChkCollision(AXIS_X1, pPos[0]))
		{
			CfPoint ptPnt;
			ptPnt.x = 0.0;
			ptPnt.y = 0.0;//pView->m_dEnc[AXIS_Y0];
			pView->Move0(ptPnt);

		}

		double fLen, fVel, fAcc, fJerk;
		fLen = sqrt(((pPos[0] - dCurrX) * (pPos[0] - dCurrX)) + ((pPos[1] - dCurrY) * (pPos[1] - dCurrY)));
		if (fLen > 0.001)
		{
			pView->m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X1, fLen, fVel, fAcc, fJerk);
			if (!pView->m_pMotion->Move(MS_X1Y1, pPos, fVel, fAcc, fAcc))
			{
				if (!pView->m_pMotion->Move(MS_X1Y1, pPos, fVel, fAcc, fAcc))
					AfxMessageBox(_T("Move XY Error..."));
			}
		}

		return TRUE;
	}

	return FALSE;
}

void CDlgMenu02::SetPinPos(int nCam, CfPoint ptPnt) 
{
	if(pView->m_pMotion)
		pView->m_pMotion->SetPinPos(nCam, ptPnt.x, ptPnt.y);
	if(pDoc->m_Master[0].m_pPcsRgn)
		pDoc->m_Master[0].m_pPcsRgn->SetPinPos(nCam, ptPnt);
	if(pDoc->m_pSpecLocal)
		pDoc->m_pSpecLocal->SavePinPos(nCam, ptPnt);

	CString sData, sPath=PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), ptPnt.x);
	pDoc->WorkingInfo.Motion.sPinPosX[nCam] = sData;
	
	sData.Format(_T("%.3f"), ptPnt.y);
	pDoc->WorkingInfo.Motion.sPinPosY[nCam] = sData;

	if(nCam==0)
	{
		::WritePrivateProfileString(_T("Motion"), _T("PIN_POS0_X"), pDoc->WorkingInfo.Motion.sPinPosX[nCam], sPath);
		::WritePrivateProfileString(_T("Motion"), _T("PIN_POS0_Y"), pDoc->WorkingInfo.Motion.sPinPosY[nCam], sPath);
	}
	else if(nCam==1)
	{
		::WritePrivateProfileString(_T("Motion"), _T("PIN_POS1_X"), pDoc->WorkingInfo.Motion.sPinPosX[nCam], sPath);
		::WritePrivateProfileString(_T("Motion"), _T("PIN_POS1_Y"), pDoc->WorkingInfo.Motion.sPinPosY[nCam], sPath);
	}
}

void CDlgMenu02::OnBtnPinSave() 
{
	// TODO: Add your control notification handler code here
//	pView->ShiftMsgPos(0, -430);
//	if(IDNO == pView->MessageBox(_T("Do you want to save Pin Position?"), "",  MB_YESNO))
//	if(IDNO == pView->DoMyMsgBox(_T("Do you want to save Pin Position?"), MB_YESNO))
	if(IDNO == pView->MsgBox(_T("Do you want to save Pin Position?"), 0, MB_YESNO))
	{
//		pView->ShiftMsgPos(0, 0);
		return;
	}
//	pView->ShiftMsgPos(0, 0);

// 	pView->MyMsgBox(_T("Do you want to save Pin Position?"), MB_YESNO, (long)200);

	CfPoint ptPnt;
	ptPnt.x = pView->m_dEnc[AXIS_X0];
	ptPnt.y = pView->m_dEnc[AXIS_Y0];

	SetPinPos(0, ptPnt);

	if(pView->m_nMkStAuto > MK_ST+11 && pView->m_nMkStAuto < MK_ST+29)
		pView->m_nMkStAuto = MK_ST+11;

 	if(pDoc->m_pSpecLocal)
 	{
 		pDoc->SetMkPnt(CAM_LF);
// 		if(myBtn[16].GetCheck())
// 		{
// 			if(m_pDlgUtil03)
// 				m_pDlgUtil03->Disp(ROT_NONE);
// // 				m_pDlgUtil03->Disp(ROT_CCW_90);
// 		}
 	}

}

void CDlgMenu02::OnBtnPinSave2() 
{
	// TODO: Add your control notification handler code here

//	pView->ShiftMsgPos(0, -430);
//	if(IDNO == pView->MessageBox(_T("Do you want to save Pin Position?"), "",  MB_YESNO))
//	if(IDNO == pView->DoMyMsgBox(_T("Do you want to save Pin Position?"), MB_YESNO))
	if(IDNO == pView->MsgBox(_T("Do you want to save Pin Position?"), 0, MB_YESNO))
	{
//		pView->ShiftMsgPos(0, 0);
		return;
	}
//	pView->ShiftMsgPos(0, 0);

// 	pView->MyMsgBox(_T("Do you want to save Pin Position?"), MB_YESNO, (long)200);

	CfPoint ptPnt;
	ptPnt.x = pView->m_dEnc[AXIS_X1];
	ptPnt.y = pView->m_dEnc[AXIS_Y1];

	SetPinPos(1, ptPnt);

	if(pDoc->m_pSpecLocal)
	{
		pDoc->SetMkPnt(CAM_RT);
		if(myBtn2[16].GetCheck())
		{
			if(m_pDlgUtil03)
				m_pDlgUtil03->Disp(ROT_NONE);
// 				m_pDlgUtil03->Disp(ROT_CCW_90);
		}
	}	

	if(pView->m_nMkStAuto > MK_ST+11 && pView->m_nMkStAuto < MK_ST+29)
		pView->m_nMkStAuto = MK_ST+11;
}

// void CDlgMenu02::PinSave() 
// {
// 	CfPoint ptPnt;
// 	ptPnt.x = pView->m_dEnc[AXIS_X];
// 	ptPnt.y = pView->m_dEnc[AXIS_Y];
// 
// 	SetPinPos(ptPnt);
// 
// 	if(pDoc->m_pSpecLocal)
// 	{
// //		pDoc->SetMkPnt(CfPoint(pDoc->m_pSpecLocal->m_dPcsOffsetX, pDoc->m_pSpecLocal->m_dPcsOffsetY));
// 		if(myBtn[16].GetCheck())
// 		{
// 			if(pView->m_pDlgUtil03)
// 				pView->m_pDlgUtil03->Disp(ROT_NONE);
// // 				pView->m_pDlgUtil03->Disp(ROT_CCW_90);
// 		}
// 	}
// }

void CDlgMenu02::OnBtnHomeMove() 
{
	// TODO: Add your control notification handler code here
	ResetLight();

	double pTgtPos[2];
	pTgtPos[1] = _tstof(pDoc->WorkingInfo.Motion.sStPosY[0]);
	pTgtPos[0] = _tstof(pDoc->WorkingInfo.Motion.sStPosX[0]);
	double dCurrX = pView->m_dEnc[AXIS_X0]; // pView->m_pMotion->GetActualPosition(AXIS_X);
	double dCurrY = pView->m_dEnc[AXIS_Y0]; // pView->m_pMotion->GetActualPosition(AXIS_Y);

	if(pView->ChkCollision(AXIS_X0, pTgtPos[0]))
	{
		CfPoint ptPnt;
		ptPnt.x = _tstof(pDoc->WorkingInfo.Motion.sSafeZone);
		ptPnt.y = pView->m_dEnc[AXIS_Y1];
		pView->Move1(ptPnt);
	}

	double fLen, fVel, fAcc, fJerk;
	fLen = sqrt( ((pTgtPos[0] - dCurrX) * (pTgtPos[0] - dCurrX)) + ((pTgtPos[1] - dCurrY) * (pTgtPos[1] - dCurrY)) );
	if(fLen > 0.001)
	{
		if(pView->m_pMotion)
		{
			pView->m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X0, fLen, fVel, fAcc, fJerk);
			if(!pView->m_pMotion->Move(MS_X0Y0, pTgtPos, fVel, fAcc, fAcc))
			{
				if(!pView->m_pMotion->Move(MS_X0Y0, pTgtPos, fVel, fAcc, fAcc))
					AfxMessageBox(_T("Move XY Error..."));
			}
		}
	}
}

void CDlgMenu02::OnBtnHomeMove2() 
{
	// TODO: Add your control notification handler code here
	ResetLight2();

	double pTgtPos[2];
	pTgtPos[1] = _tstof(pDoc->WorkingInfo.Motion.sStPosY[1]);
	pTgtPos[0] = _tstof(pDoc->WorkingInfo.Motion.sStPosX[1]);
	double dCurrX = pView->m_dEnc[AXIS_X1]; // pView->m_pMotion->GetActualPosition(AXIS_X);
	double dCurrY = pView->m_dEnc[AXIS_Y1]; // pView->m_pMotion->GetActualPosition(AXIS_Y);

	if(pView->ChkCollision(AXIS_X1, pTgtPos[0]))
	{
		CfPoint ptPnt;
		ptPnt.x = 0.0;
		ptPnt.y = pView->m_dEnc[AXIS_Y0];
		pView->Move0(ptPnt);
	}

	double fLen, fVel, fAcc, fJerk;
	fLen = sqrt( ((pTgtPos[0] - dCurrX) * (pTgtPos[0] - dCurrX)) + ((pTgtPos[1] - dCurrY) * (pTgtPos[1] - dCurrY)) );
	if(fLen > 0.001)
	{
		if(pView->m_pMotion)
		{
			pView->m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X1, fLen, fVel, fAcc, fJerk);
			if(!pView->m_pMotion->Move(MS_X1Y1, pTgtPos, fVel, fAcc, fAcc))
			{
				if(!pView->m_pMotion->Move(MS_X1Y1, pTgtPos, fVel, fAcc, fAcc))
					AfxMessageBox(_T("Move XY Error..."));
			}
		}
	}	
}

void CDlgMenu02::OnBtnHomeSave() 
{
	// TODO: Add your control notification handler code here
//	pView->ShiftMsgPos(0, -430);
//	if(IDNO == pView->DoMyMsgBox(_T("Do you want to save Marking Start Position?"), MB_YESNO))
	if(IDNO == pView->MsgBox(_T("Do you want to save Marking Start Position?"), 0, MB_YESNO))
	{
//		pView->ShiftMsgPos(0, 0);
		return;
	}
//	pView->ShiftMsgPos(0, 0);

	double dX, dY;
	dX = pView->m_dEnc[AXIS_X0];
	dY = pView->m_dEnc[AXIS_Y0];

	if(dX > 50.0)
	{
		pView->DispMsg(_T("설정범위 초과"),_T("저장 실패 : X축 50.0 이하만 설장됨 ..."),RGB_GREEN,2000,TRUE);
		return;
	}

	CString sData, sPath=PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dX);
	pDoc->WorkingInfo.Motion.sStPosX[0] = sData;
	::WritePrivateProfileString(_T("Motion"), _T("START_POSITION_X0"), sData, sPath);
	
	sData.Format(_T("%.3f"), dY);
	pDoc->WorkingInfo.Motion.sStPosY[0] = sData;
	::WritePrivateProfileString(_T("Motion"), _T("START_POSITION_Y0"), sData, sPath);
}

void CDlgMenu02::OnBtnStartSave2() 
{
	// TODO: Add your control notification handler code here
//	pView->ShiftMsgPos(0, -430);
//	if(IDNO == pView->DoMyMsgBox(_T("Do you want to save Marking2 Start Position?"), MB_YESNO))
	if(IDNO == pView->MsgBox(_T("Do you want to save Marking2 Start Position?"), 0, MB_YESNO))
	{
//		pView->ShiftMsgPos(0, 0);
		return;
	}
//	pView->ShiftMsgPos(0, 0);


	double dX, dY;
	dX = pView->m_dEnc[AXIS_X1];
	dY = pView->m_dEnc[AXIS_Y1];

	if(dX < 250.0)
	{
		pView->DispMsg(_T("설정범위 초과"),_T("저장 실패 : X축 250.0 이상만 설장됨 ..."),RGB_GREEN,2000,TRUE);
		return;
	}

	CString sData, sPath=PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dX);
	pDoc->WorkingInfo.Motion.sStPosX[1] = sData;
	::WritePrivateProfileString(_T("Motion"), _T("START_POSITION_X1"), sData, sPath);
	
	sData.Format(_T("%.3f"), dY);
	pDoc->WorkingInfo.Motion.sStPosY[1] = sData;
	::WritePrivateProfileString(_T("Motion"), _T("START_POSITION_Y1"), sData, sPath);	
}

void CDlgMenu02::OnChkResPosSt() 
{
	// TODO: Add your control notification handler code here
#ifdef USE_VISION
	if(!pView->m_pVision[0])
		return;
	myBtn[17].SetCheck(TRUE);	//	IDC_CHK_RES_POS_ST
	myBtn[17].EnableWindow(FALSE);

	pView->m_pVision[0]->CalcCameraPixelSize();

	myBtn[17].EnableWindow(TRUE);
	myBtn[17].SetCheck(FALSE);	//	IDC_CHK_RES_POS_ST

	myStcData[3].SetText(pDoc->WorkingInfo.Vision[0].sResX);
	myStcData[4].SetText(pDoc->WorkingInfo.Vision[0].sResY);
#else
	return;
#endif
}

void CDlgMenu02::OnChkResPosSt2() 
{
	// TODO: Add your control notification handler code here
#ifdef USE_VISION
	if(!pView->m_pVision[1])
		return;

	myBtn2[17].SetCheck(TRUE);	//	IDC_CHK_RES_POS_ST
	myBtn2[17].EnableWindow(FALSE);

	pView->m_pVision[1]->CalcCameraPixelSize();

	myBtn2[17].EnableWindow(TRUE);
	myBtn2[17].SetCheck(FALSE);	//	IDC_CHK_RES_POS_ST

	myStcData2[3].SetText(pDoc->WorkingInfo.Vision[1].sResX);
	myStcData2[4].SetText(pDoc->WorkingInfo.Vision[1].sResY);	
#else
	return;
#endif
}

void CDlgMenu02::OnChkMkOffsetSt() 
{
	// TODO: Add your control notification handler code here
	BOOL bOn = myBtn[20].GetCheck();
	if(bOn)
	{
		myBtn[20].EnableWindow(FALSE);	// IDC_CHK_MK_OFFSET_ST
		m_dStOffsetX = pView->m_dEnc[AXIS_X0];
		m_dStOffsetY = pView->m_dEnc[AXIS_Y0];

// 		bOn = pDoc->m_pSliceIo[7] & (0x01<<10) ? TRUE : FALSE;	// 마킹부 토크 클램프 스위치 램프 -> 마킹부 마킹 실린더 SOL
// 		if(!bOn)
// 		{
// 			if(pView->m_pDlgMenu03)
// 				pView->m_pDlgMenu03->SwMkDnSol(TRUE);
// 			Sleep(300);
// 		}

		SwMarking();

		CString str = _T("Find mark with jog button.");
#ifdef USE_VISION
		if(pView->m_pVision[0])
		{
			pView->m_pVision[0]->DrawText(str, 200, 160, RGB_RED);
			pView->m_pVision[0]->DrawCenterMark();
		}
#endif
		myBtn[20].EnableWindow(TRUE);	// IDC_CHK_MK_OFFSET_ST
		myBtn[21].EnableWindow(TRUE);	// IDC_CHK_MK_OFFSET_ED
	}
	else
		myBtn[21].EnableWindow(FALSE);	// IDC_CHK_MK_OFFSET_ED
}

void CDlgMenu02::OnChkMkOffsetSt2() 
{
	// TODO: Add your control notification handler code here
	BOOL bOn = myBtn2[19].GetCheck();
	if(bOn)
	{
		myBtn2[19].EnableWindow(FALSE);	// IDC_CHK_MK_OFFSET_ST
		m_dStOffsetX = pView->m_dEnc[AXIS_X1];
		m_dStOffsetY = pView->m_dEnc[AXIS_Y1];

// 		bOn = pDoc->m_pSliceIo[7] & (0x01<<10) ? TRUE : FALSE;	// 마킹부 토크 클램프 스위치 램프 -> 마킹부 마킹 실린더 SOL
// 		if(!bOn)
// 		{
// 			if(pView->m_pDlgMenu03)
// 				pView->m_pDlgMenu03->SwMkDnSol(TRUE);
// 			Sleep(300);
// 		}

		SwMarking2();

		CString str = _T("Find mark with jog button.");
#ifdef USE_VISION
		if(pView->m_pVision[1])
		{
			pView->m_pVision[1]->DrawText(str, 200, 160, RGB_RED);
			pView->m_pVision[1]->DrawCenterMark();
		}
#endif
		myBtn2[19].EnableWindow(TRUE);	// IDC_CHK_MK_OFFSET_ST
		myBtn2[20].EnableWindow(TRUE);	// IDC_CHK_MK_OFFSET_ED
	}
	else
		myBtn2[20].EnableWindow(FALSE);	// IDC_CHK_MK_OFFSET_ED	
}

void CDlgMenu02::OnChkMkOffsetEd() 
{
	// TODO: Add your control notification handler code here
	BOOL bOn0 = myBtn[21].GetCheck();
	BOOL bOn1 = myBtn[20].GetCheck();
	if(bOn0 && bOn1)
	{
		myBtn[21].EnableWindow(FALSE);	// IDC_CHK_MK_OFFSET_ED
		myBtn[20].SetCheck(FALSE);

		if(!pView->m_pMotion)
		{
			myBtn[20].EnableWindow(TRUE);	// IDC_CHK_MK_OFFSET_ST
			return;
		}
		
		double dCurPosX, dCurPosY;
		dCurPosX = pView->m_dEnc[AXIS_X0]; // pView->m_pMotion->GetActualPosition(AXIS_X);	
		dCurPosY = pView->m_dEnc[AXIS_Y0]; // pView->m_pMotion->GetActualPosition(AXIS_Y);	

		double dMkOffsetX, dMkOffsetY;
		if(pDoc->WorkingInfo.Vision[0].sMkOffsetX.IsEmpty())
			dMkOffsetX = 0.0;
		else
			dMkOffsetX = _tstof(pDoc->WorkingInfo.Vision[0].sMkOffsetX);

		if(pDoc->WorkingInfo.Vision[0].sMkOffsetY.IsEmpty())
			dMkOffsetY = 0.0;
		else
			dMkOffsetY = _tstof(pDoc->WorkingInfo.Vision[0].sMkOffsetY);

		dMkOffsetX += m_dStOffsetX-dCurPosX;
		dMkOffsetY += m_dStOffsetY-dCurPosY;

		CString sData, sPath=PATH_WORKING_INFO;

		sData.Format(_T("%.3f"), dMkOffsetX);
		pDoc->WorkingInfo.Vision[0].sMkOffsetX = sData;
		::WritePrivateProfileString(_T("Vision0"), _T("MARKING_OFFSET_X"), sData, sPath);
		myStcData[5].SetText(sData);	// IDC_STC_CAM_MK_OFFSET_X

		sData.Format(_T("%.3f"), dMkOffsetY);
		pDoc->WorkingInfo.Vision[0].sMkOffsetY = sData;
		::WritePrivateProfileString(_T("Vision0"), _T("MARKING_OFFSET_Y"), sData, sPath);
		myStcData[6].SetText(sData);	// IDC_STC_CAM_MK_OFFSET_Y

#ifdef USE_VISION
		if(pView->m_pVision[0])
		{
			pView->m_pVision[0]->ClearOverlay();
			pView->m_pVision[0]->DrawCenterMark();
		}
#endif
		myBtn[21].SetCheck(FALSE);
	}
}

void CDlgMenu02::OnChkMkOffsetEd2() 
{
	// TODO: Add your control notification handler code here
	BOOL bOn0 = myBtn2[19].GetCheck();
	BOOL bOn1 = myBtn2[20].GetCheck();
	if(bOn0 && bOn1)
	{
		myBtn2[20].EnableWindow(FALSE);	// IDC_CHK_MK_OFFSET_ED2
		myBtn2[19].SetCheck(FALSE);

		if(!pView->m_pMotion)
		{
			myBtn2[19].EnableWindow(TRUE);	// IDC_CHK_MK_OFFSET_ST
			return;
		}
		
		double dCurPosX, dCurPosY;
		dCurPosX = pView->m_dEnc[AXIS_X1]; // pView->m_pMotion->GetActualPosition(AXIS_X);	
		dCurPosY = pView->m_dEnc[AXIS_Y1]; // pView->m_pMotion->GetActualPosition(AXIS_Y);	

		double dMkOffsetX, dMkOffsetY;
		if(pDoc->WorkingInfo.Vision[1].sMkOffsetX.IsEmpty())
			dMkOffsetX = 0.0;
		else
			dMkOffsetX = _tstof(pDoc->WorkingInfo.Vision[1].sMkOffsetX);

		if(pDoc->WorkingInfo.Vision[1].sMkOffsetY.IsEmpty())
			dMkOffsetY = 0.0;
		else
			dMkOffsetY = _tstof(pDoc->WorkingInfo.Vision[1].sMkOffsetY);

		dMkOffsetX += m_dStOffsetX-dCurPosX;
		dMkOffsetY += m_dStOffsetY-dCurPosY;

		CString sData, sPath=PATH_WORKING_INFO;

		sData.Format(_T("%.3f"), dMkOffsetX);
		pDoc->WorkingInfo.Vision[1].sMkOffsetX = sData;
		::WritePrivateProfileString(_T("Vision1"), _T("MARKING_OFFSET_X"), sData, sPath);
		myStcData2[5].SetText(sData);	// IDC_STC_CAM_MK_OFFSET_X

		sData.Format(_T("%.3f"), dMkOffsetY);
		pDoc->WorkingInfo.Vision[1].sMkOffsetY = sData;
		::WritePrivateProfileString(_T("Vision1"), _T("MARKING_OFFSET_Y"), sData, sPath);
		myStcData2[6].SetText(sData);	// IDC_STC_CAM_MK_OFFSET_Y

#ifdef USE_VISION
		if(pView->m_pVision[1])
		{
			pView->m_pVision[1]->ClearOverlay();
			pView->m_pVision[1]->DrawCenterMark();
		}
#endif
		myBtn2[20].SetCheck(FALSE);
	}	
}


void CDlgMenu02::OnBtnAlignMove() 
{
	// TODO: Add your control notification handler code here
	if(m_nMoveAlign[0] == 0)
	{
		MoveAlign0(0);
		m_nMoveAlign[0] = 1;
	}
	else
	{
		MoveAlign0(1);
		m_nMoveAlign[0] = 0;
	}
	
}

void CDlgMenu02::OnBtnGrab() 
{
	// TODO: Add your control notification handler code here
	if (pDoc->WorkingInfo.LastJob.nAlignMethode == FOUR_POINT)
		Grab4PntAlign();
	else
		Grab2PntAlign();
}

void CDlgMenu02::Grab2PntAlign()
{
	CString str;
	double dX, dY, dAgl, dScr;

	double dCurrX = pView->m_dEnc[AXIS_X0];
	double dCurrY = pView->m_dEnc[AXIS_Y0];

	double pPos[4];
	pPos[0] = pDoc->m_Master[0].m_stAlignMk.X0 + pView->m_pMotion->m_dPinPosX[0];
	pPos[1] = pDoc->m_Master[0].m_stAlignMk.Y0 + pView->m_pMotion->m_dPinPosY[0];
	pPos[2] = pDoc->m_Master[0].m_stAlignMk.X1 + pView->m_pMotion->m_dPinPosX[0];
	pPos[3] = pDoc->m_Master[0].m_stAlignMk.Y1 + pView->m_pMotion->m_dPinPosY[0];

	int nPos = -1;
	if ((dCurrX > pPos[0] - 0.1 && dCurrX < pPos[0] + 0.1) && (dCurrY > pPos[1] - 0.1 && dCurrY < pPos[1] + 0.1))
		nPos = 0;
	else if ((dCurrX > pPos[2] - 0.1 && dCurrX < pPos[2] + 0.1) && (dCurrY > pPos[3] - 0.1 && dCurrY < pPos[3] + 0.1))
		nPos = 1;

	if (nPos == -1)
	{
		AfxMessageBox(_T("Error - Grab2PntAlign() failed!"));
		return;
	}

#ifdef USE_VISION
	if (pView->m_pVision[0]->Grab(nPos))
	{
		GetPmRst0(dX, dY, dAgl, dScr);
		str.Format(_T("%.1f"), dX);
		pDoc->WorkingInfo.Motion.sAlignResultPosX[0][nPos] = str;
		myStcData[7].SetText(str);

		str.Format(_T("%.1f"), dY);
		pDoc->WorkingInfo.Motion.sAlignResultPosY[0][nPos] = str;
		myStcData[8].SetText(str);

		str.Format(_T("%.1f"), dAgl);
		pDoc->WorkingInfo.Motion.sAlignResultTheta[0][nPos] = str;
		myStcData[9].SetText(str);

		str.Format(_T("%.1f"), dScr);
		pDoc->WorkingInfo.Motion.sAlignResultScore[0][nPos] = str;
		myStcData[10].SetText(str);
	}
	else
	{
		myStcData[7].SetText(_T(""));
		myStcData[8].SetText(_T(""));
		myStcData[9].SetText(_T(""));
		myStcData[10].SetText(_T(""));
	}
#endif
}

void CDlgMenu02::Grab4PntAlign()
{
	CString str;
	double dX, dY, dAgl, dScr;

	double dCurrX = pView->m_dEnc[AXIS_X0];
	double dCurrY = pView->m_dEnc[AXIS_Y0];

	double pPos[8];
	pPos[0] = pDoc->m_Master[0].m_stAlignMk2.X0 + pView->m_pMotion->m_dPinPosX[0];
	pPos[1] = pDoc->m_Master[0].m_stAlignMk2.Y0 + pView->m_pMotion->m_dPinPosY[0];
	pPos[2] = pDoc->m_Master[0].m_stAlignMk2.X1 + pView->m_pMotion->m_dPinPosX[0];
	pPos[3] = pDoc->m_Master[0].m_stAlignMk2.Y1 + pView->m_pMotion->m_dPinPosY[0];

	pPos[4] = pDoc->m_Master[0].m_stAlignMk2.X2 + pView->m_pMotion->m_dPinPosX[0];
	pPos[5] = pDoc->m_Master[0].m_stAlignMk2.Y2 + pView->m_pMotion->m_dPinPosY[0];
	pPos[6] = pDoc->m_Master[0].m_stAlignMk2.X3 + pView->m_pMotion->m_dPinPosX[0];
	pPos[7] = pDoc->m_Master[0].m_stAlignMk2.Y3 + pView->m_pMotion->m_dPinPosY[0];

	int nPos = -1;
	if ((dCurrX > pPos[0] - 0.1 && dCurrX < pPos[0] + 0.1) && (dCurrY > pPos[1] - 0.1 && dCurrY < pPos[1] + 0.1))
		nPos = 0;
	else if ((dCurrX > pPos[2] - 0.1 && dCurrX < pPos[2] + 0.1) && (dCurrY > pPos[3] - 0.1 && dCurrY < pPos[3] + 0.1))
		nPos = 1;
	else if ((dCurrX > pPos[4] - 0.1 && dCurrX < pPos[4] + 0.1) && (dCurrY > pPos[5] - 0.1 && dCurrY < pPos[5] + 0.1))
			nPos = 2;
	else if ((dCurrX > pPos[6] - 0.1 && dCurrX < pPos[6] + 0.1) && (dCurrY > pPos[7] - 0.1 && dCurrY < pPos[7] + 0.1))
		nPos = 3;

	if (nPos == -1)
	{
		AfxMessageBox(_T("Error - Grab4PntAlign() failed!"));
		return;
	}

#ifdef USE_VISION
	if (pView->m_pVision[0]->Grab(nPos))
	{
		GetPmRst0(dX, dY, dAgl, dScr);
		str.Format(_T("%.1f"), dX);
		pDoc->WorkingInfo.Motion.sAlignResultPosX[0][nPos] = str;
		myStcData[7].SetText(str);

		str.Format(_T("%.1f"), dY);
		pDoc->WorkingInfo.Motion.sAlignResultPosY[0][nPos] = str;
		myStcData[8].SetText(str);

		str.Format(_T("%.1f"), dAgl);
		pDoc->WorkingInfo.Motion.sAlignResultTheta[0][nPos] = str;
		myStcData[9].SetText(str);

		str.Format(_T("%.1f"), dScr);
		pDoc->WorkingInfo.Motion.sAlignResultScore[0][nPos] = str;
		myStcData[10].SetText(str);
	}
	else
	{
		myStcData[7].SetText(_T(""));
		myStcData[8].SetText(_T(""));
		myStcData[9].SetText(_T(""));
		myStcData[10].SetText(_T(""));
	}
#endif
}

BOOL CDlgMenu02::GetPmRst0(double &dX, double &dY, double &dAgl, double &dScr)
{
#ifdef USE_VISION
	if(!pView->m_pVision[0])
		return FALSE;

	dX = pView->m_pVision[0]->PtMtRst.dX;
	dY = pView->m_pVision[0]->PtMtRst.dY;
	dAgl = pView->m_pVision[0]->PtMtRst.dAngle;
	dScr = pView->m_pVision[0]->PtMtRst.dScore;
#endif
	return TRUE;
}

BOOL CDlgMenu02::GetPmRst1(double &dX, double &dY, double &dAgl, double &dScr)
{
#ifdef USE_VISION
	if(!pView->m_pVision[1])
		return FALSE;

	dX = pView->m_pVision[1]->PtMtRst.dX;
	dY = pView->m_pVision[1]->PtMtRst.dY;
	dAgl = pView->m_pVision[1]->PtMtRst.dAngle;
	dScr = pView->m_pVision[1]->PtMtRst.dScore;
#endif
	return TRUE;
}

BOOL CDlgMenu02::Grab(int nPos, BOOL bMove)
{
	// TODO: Add your control notification handler code here
	if(bMove)
	{
		MovePinPos();
		Sleep(100);
	}

	CString str = _T("");
#ifdef USE_VISION
	if(pView->m_pVision[0])
	{
		DWORD dwSt = GetTickCount();
		if(pView->m_pVision[0]->Grab(nPos))
		{
			double dX, dY, dAgl, dScr;
			GetPmRst0(dX, dY, dAgl, dScr);
			str.Format(_T("%.1f"), dX);
			myStcData[7].SetText(str);
			str.Format(_T("%.1f"), dY);
			myStcData[8].SetText(str);
			str.Format(_T("%.1f"), dAgl);
			myStcData[9].SetText(str);
			str.Format(_T("%.1f"), dScr);
			myStcData[10].SetText(str);

			str.Format(_T("%d [mSec]"), GetTickCount()-dwSt);
// 			pView->DispStsBar(str, 0);
			return TRUE;
		}
		else
		{
			myStcData[7].SetText(_T(""));
			myStcData[8].SetText(_T(""));
			myStcData[9].SetText(_T(""));
			myStcData[10].SetText(_T(""));
		}
	}
#endif
// 	myStcData[7].SetText(str);
// 	myStcData[8].SetText(str);
// 	myStcData[9].SetText(str);
// 	myStcData[10].SetText(str);

	return FALSE;		
}

void CDlgMenu02::OnChkMarkingTest2() 
{
	// TODO: Add your control notification handler code here
	m_lChk = 1;
	SetTimer(TIM_SHOW_MK_TEST, 30, NULL);

	//ChkMarkingTest2();
}

void CDlgMenu02::ChkMarkingTest2() 
{
	BOOL bOn = myBtn2[16].GetCheck();
	if(bOn)
	{
		CRect rt;		
		GetDlgItem(IDC_STATIC_MK_TEST)->GetWindowRect(&rt);
		if(m_pDlgUtil03)
		{
			if(!m_pDlgUtil03->IsWindowVisible())
			{
				m_pDlgUtil03->ShowWindow(SW_SHOW);
			}
			m_pDlgUtil03->MoveWindow(rt, TRUE);
		}
	}
	else
	{
		if(m_pDlgUtil03)
		{
			if(m_pDlgUtil03->IsWindowVisible())
			{
				m_pDlgUtil03->ShowWindow(SW_HIDE);
			}
		}
		m_lChk = 0;
	}	
}

void CDlgMenu02::OnBtnAlign() 
{
	// TODO: Add your control notification handler code here
	BOOL bOn = myBtn[16].GetCheck();
	if(bOn)
	{
		myBtn[16].SetCheck(FALSE);
		//if(m_pDlgUtil06)
		//{
		//	if(m_pDlgUtil06->IsWindowVisible())
		//	{
		//		m_pDlgUtil06->ShowWindow(SW_HIDE);
		//	}
		//}
	}
	int nAlignNum = m_nBtnAlignCam0Pos;

	if (m_nBtnAlignCam0Pos == 0)
	{
//		if(IDNO == pView->DoMyMsgBox(_T("Do you want to Two Point Align Test?"), MB_YESNO))
		if(IDNO == pView->MsgBox(_T("Do you want to Two Point Align Test?"), 0, MB_YESNO))
			return;

		TwoPointAlign0(nAlignNum);
		m_nBtnAlignCam0Pos = 1;
	}
	else if (m_nBtnAlignCam0Pos == 1)
	{
		TwoPointAlign0(nAlignNum);
		m_nBtnAlignCam0Pos = 0;
	}
}

void CDlgMenu02::OnBtnAlign2() 
{
	// TODO: Add your control notification handler code here
	BOOL bOn = myBtn[16].GetCheck();
	if(bOn)
	{
		myBtn[16].SetCheck(FALSE);
		//if(m_pDlgUtil06)
		//{
		//	if(m_pDlgUtil06->IsWindowVisible())
		//	{
		//		m_pDlgUtil06->ShowWindow(SW_HIDE);
		//	}
		//}
	}

	int nAlignNum = m_nBtnAlignCam1Pos;

	if (m_nBtnAlignCam1Pos == 0)
	{
//		if(IDNO == pView->DoMyMsgBox(_T("Do you want to Two Point Align Test?"), MB_YESNO))
		if(IDNO == pView->MsgBox(_T("Do you want to Two Point Align Test?"), 0, MB_YESNO))
		 	return;

		TwoPointAlign1(nAlignNum);
		m_nBtnAlignCam1Pos = 1;
	}
	else if (m_nBtnAlignCam1Pos == 1)
	{
		TwoPointAlign1(nAlignNum);
		m_nBtnAlignCam1Pos = 0;
	}	

}

void CDlgMenu02::SwMarking()
{
	if(!pView->m_pMotion)
		return;

	double dCurPosX = pView->m_dEnc[AXIS_X0]; // pView->m_pMotion->GetActualPosition(AXIS_X);	
	double dCurPosY = pView->m_dEnc[AXIS_Y0]; // pView->m_pMotion->GetActualPosition(AXIS_Y);	
	double dStPosX = _tstof(pDoc->WorkingInfo.Motion.sStPosX[0]);
	double dStPosY = _tstof(pDoc->WorkingInfo.Motion.sStPosY[0]);
	if(dCurPosX < (dStPosX+10.0) && dCurPosX > (dStPosX-10.0)
		&& dCurPosY < (dStPosY+10.0) && dCurPosY > (dStPosY-10.0))
		return;

	SetMkCurPos(0);

	double dMkOffsetX, dMkOffsetY;
	if(pDoc->WorkingInfo.Vision[0].sMkOffsetX.IsEmpty())
		dMkOffsetX = 0.0;
	else
		dMkOffsetX = _tstof(pDoc->WorkingInfo.Vision[0].sMkOffsetX);

	if(pDoc->WorkingInfo.Vision[0].sMkOffsetY.IsEmpty())
		dMkOffsetY = 0.0;
	else
		dMkOffsetY = _tstof(pDoc->WorkingInfo.Vision[0].sMkOffsetY);

	double pPos[2];
	pPos[0] = m_dCurPosX[0] + dMkOffsetX;
	pPos[1] = m_dCurPosY[0] + dMkOffsetY;

	if(pView->ChkCollision(AXIS_X0, pPos[0]))
	{
		CfPoint ptPnt;
		ptPnt.x = _tstof(pDoc->WorkingInfo.Motion.sSafeZone);
		ptPnt.y = 0.0;//pView->m_dEnc[AXIS_Y1];
		pView->Move1(ptPnt);
	}

	double fLen, fVel, fAcc, fJerk;
	fLen = sqrt( dMkOffsetX*dMkOffsetX + dMkOffsetY*dMkOffsetY );
	if(fLen > 0.001)
	{
		pView->m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X0, fLen, fVel, fAcc, fJerk);
		if(!pView->m_pMotion->Move(MS_X0Y0, pPos, fVel, fAcc, fAcc))
		{
			if(!pView->m_pMotion->Move(MS_X0Y0, pPos, fVel, fAcc, fAcc))
				AfxMessageBox(_T("Move XY Error..."));
		}
	}


	if(pView->m_pVoiceCoil[0])
		pView->m_pVoiceCoil[0]->SetMarking(0);
	SetMkDlyOff(0);

}

void CDlgMenu02::SwMarking2()
{
	if(!pView->m_pMotion)
		return;

	double dCurPosX = pView->m_dEnc[AXIS_X1]; // pView->m_pMotion->GetActualPosition(AXIS_X);	
	double dCurPosY = pView->m_dEnc[AXIS_Y1]; // pView->m_pMotion->GetActualPosition(AXIS_Y);	
	double dStPosX = _tstof(pDoc->WorkingInfo.Motion.sStPosX[1]);
	double dStPosY = _tstof(pDoc->WorkingInfo.Motion.sStPosY[1]);
	if(dCurPosX < (dStPosX+10.0) && dCurPosX > (dStPosX-10.0)
		&& dCurPosY < (dStPosY+10.0) && dCurPosY > (dStPosY-10.0))
		return;

	SetMkCurPos(1);

	double dMkOffsetX, dMkOffsetY;
	if(pDoc->WorkingInfo.Vision[1].sMkOffsetX.IsEmpty())
		dMkOffsetX = 0.0;
	else
		dMkOffsetX = _tstof(pDoc->WorkingInfo.Vision[1].sMkOffsetX);

	if(pDoc->WorkingInfo.Vision[1].sMkOffsetY.IsEmpty())
		dMkOffsetY = 0.0;
	else
		dMkOffsetY = _tstof(pDoc->WorkingInfo.Vision[1].sMkOffsetY);

	double pPos[2];
	pPos[0] = m_dCurPosX[1] + dMkOffsetX;
	pPos[1] = m_dCurPosY[1] + dMkOffsetY;

	if(pView->ChkCollision(AXIS_X1, pPos[0]))
	{
		CfPoint ptPnt;
		ptPnt.x = 0.0;
		ptPnt.y = 0.0;//pView->m_dEnc[AXIS_Y0];
		pView->Move0(ptPnt);
	}

	double fLen, fVel, fAcc, fJerk;
	fLen = sqrt( dMkOffsetX*dMkOffsetX + dMkOffsetY*dMkOffsetY );
	if(fLen > 0.001)
	{
		pView->m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X1, fLen, fVel, fAcc, fJerk);
		if(!pView->m_pMotion->Move(MS_X1Y1, pPos, fVel, fAcc, fAcc))
		{
			if(!pView->m_pMotion->Move(MS_X1Y1, pPos, fVel, fAcc, fAcc))
				AfxMessageBox(_T("Move XY Error..."));
		}
	}


	if(pView->m_pVoiceCoil[1])
		pView->m_pVoiceCoil[1]->SetMarking(1);
	SetMkDlyOff(1);

}

void CDlgMenu02::SetMkCurPos(int nCam)
{
	if (nCam == 0)
	{
		m_dCurPosX[0] = pView->m_dEnc[AXIS_X0]; // pView->m_pMotion->GetActualPosition(AXIS_X);	
		m_dCurPosY[0] = pView->m_dEnc[AXIS_Y0]; // pView->m_pMotion->GetActualPosition(AXIS_Y);	
	}
	else
	{
		m_dCurPosX[1] = pView->m_dEnc[AXIS_X1]; // pView->m_pMotion->GetActualPosition(AXIS_X);	
		m_dCurPosY[1] = pView->m_dEnc[AXIS_Y1]; // pView->m_pMotion->GetActualPosition(AXIS_Y);
	}
	
}


void CDlgMenu02::SetMkDlyOff(int nCam, int mSec)
{
	if (nCam == 0)
		SetTimer(TIM_MARKING_OFF, WAIT_MARKING, NULL);
	else
		SetTimer(TIM_MARKING2_OFF, WAIT_MARKING, NULL);
}

void CDlgMenu02::MarkingOff()
{
	if(!pView->m_pMotion)
		return;
	
	// Ink Marking OFF
// 	pDoc->m_pSliceIo[7] &= ~(0x01<<12);	// 마킹부 마킹 콘트롤러 출력 1(전원공급) -> Purge
// 	pDoc->m_pSliceIo[7] &= ~(0x01<<13);	// 마킹부 마킹 콘트롤러 출력 2(전원공급) -> Sen

	double dMkOffsetX, dMkOffsetY;
	if(pDoc->WorkingInfo.Vision[0].sMkOffsetX.IsEmpty())
		dMkOffsetX = 0.0;
	else
		dMkOffsetX = _tstof(pDoc->WorkingInfo.Vision[0].sMkOffsetX);

	if(pDoc->WorkingInfo.Vision[0].sMkOffsetY.IsEmpty())
		dMkOffsetY = 0.0;
	else
		dMkOffsetY = _tstof(pDoc->WorkingInfo.Vision[0].sMkOffsetY);

	double fLen, fVel, fAcc, fJerk;
	fLen = sqrt( dMkOffsetX*dMkOffsetX + dMkOffsetY*dMkOffsetY );
	if(fLen > 0.001)
	{
		double pPos[2];
		pPos[0] = m_dCurPosX[0];
		pPos[1] = m_dCurPosY[0];

		if(pView->ChkCollision(AXIS_X0, pPos[0]))
		{
			CfPoint ptPnt;
			ptPnt.x = _tstof(pDoc->WorkingInfo.Motion.sSafeZone);
			ptPnt.y = 0.0;//pView->m_dEnc[AXIS_Y1];
			pView->Move1(ptPnt);
		}

		pView->m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X0, fLen, fVel, fAcc, fJerk);
		if(!pView->m_pMotion->Move(MS_X0Y0, pPos, fVel, fAcc, fAcc))
		{
			if(!pView->m_pMotion->Move(MS_X0Y0, pPos, fVel, fAcc, fAcc))
				AfxMessageBox(_T("Move XY Error..."));
		}
	}
}

void CDlgMenu02::MarkingOff2()
{
	if(!pView->m_pMotion)
		return;
	
	// Ink Marking OFF
// 	pDoc->m_pSliceIo[7] &= ~(0x01<<12);	// 마킹부 마킹 콘트롤러 출력 1(전원공급) -> Purge
// 	pDoc->m_pSliceIo[7] &= ~(0x01<<13);	// 마킹부 마킹 콘트롤러 출력 2(전원공급) -> Sen

	double dMkOffsetX, dMkOffsetY;
	if(pDoc->WorkingInfo.Vision[1].sMkOffsetX.IsEmpty())
		dMkOffsetX = 0.0;
	else
		dMkOffsetX = _tstof(pDoc->WorkingInfo.Vision[1].sMkOffsetX);

	if(pDoc->WorkingInfo.Vision[1].sMkOffsetY.IsEmpty())
		dMkOffsetY = 0.0;
	else
		dMkOffsetY = _tstof(pDoc->WorkingInfo.Vision[1].sMkOffsetY);

	double fLen, fVel, fAcc, fJerk;
	fLen = sqrt( dMkOffsetX*dMkOffsetX + dMkOffsetY*dMkOffsetY );
	if(fLen > 0.001)
	{
		double pPos[2];
		pPos[0] = m_dCurPosX[1];
		pPos[1] = m_dCurPosY[1];

		if(pView->ChkCollision(AXIS_X1, pPos[0]))
		{
			CfPoint ptPnt;
			ptPnt.x = 0.0;
			ptPnt.y = 0.0;//pView->m_dEnc[AXIS_Y0];
			pView->Move0(ptPnt);
		}

		pView->m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X1, fLen, fVel, fAcc, fJerk);
		if(!pView->m_pMotion->Move(MS_X1Y1, pPos, fVel, fAcc, fAcc))
		{
			if(!pView->m_pMotion->Move(MS_X1Y1, pPos, fVel, fAcc, fAcc))
				AfxMessageBox(_T("Move XY Error..."));
		}
	}
}

void CDlgMenu02::OnBtnBuffHome() 
{
	// TODO: Add your control notification handler code here
// 	if(!pView->m_pMotion)
// 		return;
// 
// 	pView->SetMkFdSts();
// 	Sleep(300);
	if(pDoc->WorkingInfo.Motion.bBufHomming)
	{
		pView->DispMsg(_T("Homming"),_T("Searching Buffer Home Position..."),RGB_GREEN,2000,TRUE);
		//pView->m_pMotion->SearchHomeBuf(FALSE);
#ifdef USE_MPE
		if(pView->m_pMpe)
			pView->m_pMpe->Write(_T("MB440152"), 1);	// 마킹부 버퍼롤러 홈동작 ON
#endif
		pView->m_bBufHomeDone = FALSE;
		if(pView->m_pDlgMenu03)
			pView->m_pDlgMenu03->ChkBufHomeDone();
	}	
}

void CDlgMenu02::OnBtnBuffInitMove() 
{
	// TODO: Add your control notification handler code here
	pView->DispMsg(_T("Moving"),_T("Searching Buffer Initial Position..."),RGB_GREEN,2000,TRUE);
#ifdef USE_MPE
	if(pView->m_pMpe)
		pView->m_pMpe->Write(_T("MB44015A"), 1);	// 마킹부 버퍼 초기위치 이동(PC가 ON, PLC가 OFF)
#endif
	if(pView->m_pDlgMenu03)
		pView->m_pDlgMenu03->ChkBufInitDone();

// 	if(!pView->m_pMotion)
// 		return;
// 
// 	pView->SetMkFdSts();
// 	Sleep(300);
// 
// 	double dDir;
// 	double fLen, fVel, fAcc, fJerk;
// 	double dStPos = pView->m_pMotion->m_dStBufPos;
// 	double dCurPosMkFd = (double)pDoc->m_pMpeData[0][0];	// 마킹부 Feeding 엔코더 값(단위 mm )
// 	double dCurPosBuf = (double)pDoc->m_pMpeData[0][1] / 1000.0;	// 마킹부 버퍼 엔코더 값(단위 mm * 1000)
// 
// 	fLen = dStPos - dCurPosBuf;
// 
// 	if(fLen > 0)
// 	{
// 		fLen = (dStPos-dCurPosBuf);
// 		dDir = (double)M_CCW;
// 	}
// 	else
// 	{
// 		fLen = (dCurPosBuf-dStPos);
// 		dDir = (double)M_CW;
// 	}
// 
// 	if(fLen > 0.001)
// 	{
// 		double dPos = dCurPosMkFd + dDir*fLen;
// 		pView->m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_MKFD, fLen, fVel, fAcc, fJerk);
// 		pView->m_pMotion->Move(MS_MKFD, dPos, fVel/10.0, fAcc/10.0, fAcc/10.0);
// 		fVel = pView->m_pMotion->m_pParamMotion[MS_MKFD].Home.f1stSpd;
// 		fAcc = pView->m_pMotion->m_pParamMotion[MS_MKFD].Home.fAcc;
// 		if(!pView->m_pMotion->Move(MS_MKFD, dDir*fLen, fVel, fAcc, fAcc, INC, WAIT))
// 		{
// 			if(!pView->m_pMotion->Move(MS_MKFD, dDir*fLen, fVel, fAcc, fAcc, INC, WAIT))
// 				AfxMessageBox(_T("Move XY Error..."));
// 		}
// 	}	
}

void CDlgMenu02::OnBtnBuffInitSave() 
{
	// TODO: Add your control notification handler code here
	if(!pView->m_pMotion || !pDoc->m_pMpeData)
		return;

	BOOL bOn = myBtn[16].GetCheck();
	if(bOn)
	{
		myBtn[16].SetCheck(FALSE);
		//if(m_pDlgUtil06)
		//{
		//	if(m_pDlgUtil06->IsWindowVisible())
		//	{
		//		m_pDlgUtil06->ShowWindow(SW_HIDE);
		//	}
		//}
	}

//	if(IDNO == pView->DoMyMsgBox(_T("Do you want to save Buffer Position?"), MB_YESNO))
	if(IDNO == pView->MsgBox(_T("Do you want to save Buffer Position?"), 0, MB_YESNO))
		return;

	double dBufEnc = (double)pDoc->m_pMpeData[0][1]	/ 1000.0;	// 마킹부 버퍼 엔코더 값(단위 mm * 1000)
	pView->SetBufInitPos(dBufEnc);
// 	pView->SetBufInitPos(pView->m_dEnc[AXIS_BUF]);
}

void CDlgMenu02::DispBufEnc()
{
// 	CString str;
// 	double dBufEnc = (double)pDoc->m_pMpeData[0][1]	/ 1000.0	// 마킹부 버퍼 엔코더 값(단위 mm * 1000)
// 	str.Format(_T("%.3f"), dBufEnc);
//	myStcData[11].SetText(str);
}

void CDlgMenu02::DispCenterMark()
{
#ifdef USE_VISION
	if (pView->m_pVision[0])
	{
		pView->m_pVision[0]->DrawCross();
	}

	if (pView->m_pVision[1])
	{
		pView->m_pVision[1]->DrawCross();
	}
#endif
}

void CDlgMenu02::DispAxisPos()
{
#ifdef USE_VISION
	if(pView->m_pVision[0])
	{
#ifdef TEST_MODE
		pView->m_pVision[0]->DispAxisPos(TRUE);
#else
		pView->m_pVision[0]->DispAxisPos();
#endif
	}

	if(pView->m_pVision[1])
	{
#ifdef TEST_MODE
		pView->m_pVision[1]->DispAxisPos(TRUE);
#else
		pView->m_pVision[1]->DispAxisPos();
#endif
	}
#endif
}


// void CDlgMenu02::SetMkPos(CfPoint ptOfst)
// {
// 	if(!pView->m_pVision[0])
// 		return;
// 	int nSzCtrlX, nSzCtrlY, nSzImgX, nSzImgY;
// // 	m_pPcsGL->GetPixelInfo(nSzCtrlX, nSzCtrlY, nSzImgX, nSzImgY);
// 	pView->m_pVision[0]->GetPixelInfo(nSzCtrlX, nSzCtrlY, nSzImgX, nSzImgY);
// 	double dImgPixelRes = pDoc->m_Master[0].MasterInfo.dPixelSize / 1000.0; // [mm]
// // 	double dImgPixelRes = pDoc->MasterInfo.dPixelSize / 1000.0; // [mm]
// 	int nPixX = int(ptOfst.x/dImgPixelRes);
// 	int nPixY = int(ptOfst.y/dImgPixelRes);
// 	CfPoint ptPnt;
// 	ptPnt.x = nSzImgX/2 + nPixX;
// 	ptPnt.y = nSzImgY/2 + nPixY;
// // 	m_pPcsGL->SetMkPos(ptPnt);
// 	pView->m_pVision[0]->SetMkPos(ptPnt);
// }

// void CDlgMenu02::DispLocalSpec()
// {
// 	CString str;
// 	str.Format(_T("%.3f"), pDoc->m_pSpecLocal->m_dPcsOffsetX);
// 	myStcData[0].SetText(str);
// 	str.Format(_T("%.3f"), pDoc->m_pSpecLocal->m_dPcsOffsetY);
// 	myStcData[1].SetText(str);
// }

void CDlgMenu02::ChgModel()
{
// 	m_pPcsGL->LoadPcsImg(PATH_PCS_IMG);

#ifdef USE_VISION
	if(pView->m_pVision[0])
	{
// 		pView->m_pVision[0]->ShowDispPcs(nLayer);
 		pView->m_pVision[0]->ShowDispPin(0);
		pView->m_pVision[0]->ShowDispAlign();
	}

	if(pView->m_pVision[1])
	{
// 		pView->m_pVision[0]->ShowDispPcs(nLayer);
 		pView->m_pVision[1]->ShowDispPin(0);
		pView->m_pVision[1]->ShowDispAlign();
	}
#endif
}

void CDlgMenu02::ChgModelUp()
{
#ifdef USE_VISION
	if(pView->m_pVision[0])
	{
// 		pView->m_pVision[0]->ShowDispPcs(nLayer);
 		pView->m_pVision[0]->ShowDispPin(0);
		pView->m_pVision[0]->ShowDispAlign();
	}
#endif
}

void CDlgMenu02::ChgModelDn()
{
#ifdef USE_VISION
	if(pView->m_pVision[1])
	{
// 		pView->m_pVision[0]->ShowDispPcs(nLayer);
 		pView->m_pVision[1]->ShowDispPin(0);
		pView->m_pVision[1]->ShowDispAlign();
	}
#endif
}

BOOL CDlgMenu02::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
#ifdef USE_VISION
	if(pView->m_pVision[0] && pView->m_pVision[1] && pView->m_pMotion)
	{
		CString sMsg;
		int nCamSzX, nCamSzY;
		pView->m_pVision[0]->GetCameraSize(nCamSzX, nCamSzY);

		double dResX, dResY;
		double dCenterX[2] = { 323, 963 };
		double dCenterY[2] = { 318, 318 };
		double pPos[2];

		if(pMsg->message == WM_LBUTTONDBLCLK && GetDlgItem(IDC_STC_VISION)->GetSafeHwnd() == pMsg->hwnd)
		{
			dResX = _tstof(pDoc->WorkingInfo.Vision[0].sResX);
			dResY = _tstof(pDoc->WorkingInfo.Vision[0].sResY);
			sMsg.Format(_T("%d,%d"), pMsg->pt.x, pMsg->pt.y);
			pFrm->DispStatusBar(sMsg, 6);

			double fLen, fVel, fAcc, fJerk;
			fLen = 1.0 * sqrt(((pMsg->pt.x - dCenterX[0]) * dResX * (pMsg->pt.x - dCenterX[0]) * dResX) + ((pMsg->pt.y - dCenterY[0]) * dResY * (pMsg->pt.y - dCenterY[0]) * dResY));
			if (fLen > 0.001)
			{
				pView->m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X0, fLen, fVel, fAcc, fJerk);

				pPos[0] = pView->m_dEnc[AXIS_X0] + (1.0 * (pMsg->pt.x - dCenterX[0]) * dResX);
				pPos[1] = pView->m_dEnc[AXIS_Y0] + (1.0 * (pMsg->pt.y - dCenterY[0]) * dResY);
				if (!pView->m_pMotion->Move(MS_X0Y0, pPos, fVel, fAcc, fAcc))
				{
					if (!pView->m_pMotion->Move(MS_X0Y0, pPos, fVel, fAcc, fAcc))
						AfxMessageBox(_T("Move X0Y0 Error..."));
				}
			}
		}
		else if(pMsg->message == WM_LBUTTONDBLCLK && GetDlgItem(IDC_STC_VISION_2)->GetSafeHwnd() == pMsg->hwnd)
		{
			dResX = _tstof(pDoc->WorkingInfo.Vision[1].sResX);
			dResY = _tstof(pDoc->WorkingInfo.Vision[1].sResY);
			sMsg.Format(_T("%d,%d"), pMsg->pt.x, pMsg->pt.y);
			pFrm->DispStatusBar(sMsg, 6);

			double fLen, fVel, fAcc, fJerk;
			fLen = 1.0 * sqrt(((pMsg->pt.x - dCenterX[1]) * dResX * (pMsg->pt.x - dCenterX[1]) * dResX) + ((pMsg->pt.y - dCenterY[1]) * dResY * (pMsg->pt.y - dCenterY[1]) * dResY));
			if (fLen > 0.001)
			{
				pView->m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X1, fLen, fVel, fAcc, fJerk);

				pPos[0] = pView->m_dEnc[AXIS_X1] + (1.0 * (pMsg->pt.x - dCenterX[1]) * dResX);
				pPos[1] = pView->m_dEnc[AXIS_Y1] + (1.0 * (pMsg->pt.y - dCenterY[1]) * dResY);
				if (!pView->m_pMotion->Move(MS_X1Y1, pPos, fVel, fAcc, fAcc))
				{
					if (!pView->m_pMotion->Move(MS_X1Y1, pPos, fVel, fAcc, fAcc))
						AfxMessageBox(_T("Move X1Y1 Error..."));
				}
			}
		}
	}
#endif
	if (pMsg->message != WM_KEYDOWN)
		return CDialog::PreTranslateMessage(pMsg);

	if ((pMsg->lParam & 0x40000000) == 0)
	{
		switch(pMsg->wParam)
		{
		case VK_RETURN:
			return TRUE;
		case VK_ESCAPE:
			return TRUE;
		case 'S':
		case 's':
			if(GetKeyState(VK_CONTROL) < 0) // Ctrl 키가 눌려진 상태
			{
				WINDOWPLACEMENT wndPlace;
				AfxGetMainWnd()->GetWindowPlacement(&wndPlace);
				wndPlace.showCmd |= SW_MAXIMIZE; 
				AfxGetMainWnd()->SetWindowPlacement(&wndPlace);	
			}
			break;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CDlgMenu02::ShowKeypad(int nCtlID, CPoint ptSt, int nDir)
{
	BOOL bAdj = TRUE;
	CString strData, strPrev;
	GetDlgItemText(nCtlID, strData);
	strPrev = strData;

	CString strMin, strMax;
	CRect rect(0,0,0,0);
	CDlgKeyNum *pDlg = new CDlgKeyNum(&strData, &rect, ptSt, nDir);
	pDlg->m_strCurr = strData;
	pDlg->m_strMin = strMin = pDoc->GetMin(IDD_DLG_MENU_01, nCtlID);
	pDlg->m_strMax = strMax = pDoc->GetMax(IDD_DLG_MENU_01, nCtlID);
	pDlg->DoModal();
	delete pDlg;

	if(!strMin.IsEmpty() && !strMax.IsEmpty())
	{
		if(_tstof(strData) < _tstof(strMin) || 
			_tstof(strData) > _tstof(strMax))
		{
			SetDlgItemText(nCtlID, strPrev);
			pView->DispMsg(_T("입력 범위를 벗어났습니다."), _T("주의"), RGB_YELLOW);
		}
		else
			SetDlgItemText(nCtlID, strData);
	}
	else
		SetDlgItemText(nCtlID, strData);
	
	return bAdj;
}

void CDlgMenu02::OnStcAlignStdScr() 
{
	// TODO: Add your control notification handler code here
	myStcData[12].SetBkColor(RGB_RED);
	myStcData[12].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_ALIGN_STD_SCR)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_ALIGN_STD_SCR, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[12].SetBkColor(RGB_WHITE);
	myStcData[12].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_ALIGN_STD_SCR)->GetWindowText(sData);
	pDoc->WorkingInfo.Vision[0].sStdScr = sData;
	::WritePrivateProfileString(_T("Vision0"), _T("ALIGN_STD_SCORE"), sData, PATH_WORKING_INFO);
}

void CDlgMenu02::OnChkLtOn2() 
{
	// TODO: Add your control notification handler code here
	BOOL bOn = myBtn2[0].GetCheck();
	if(bOn)
	{
		myBtn2[1].EnableWindow(TRUE);
		GetDlgItem(IDC_SLD_LT2)->EnableWindow(TRUE);
		myBtn2[2].EnableWindow(TRUE);

		SetLight2();
	}
	else
	{
		ResetLight2();
		
		myBtn2[1].EnableWindow(FALSE);
		GetDlgItem(IDC_SLD_LT2)->EnableWindow(FALSE);
		myBtn2[2].EnableWindow(FALSE);
	}	
}



void CDlgMenu02::OnChkMk0Pos1() 
{
	// TODO: Add your control notification handler code here
	if(myBtn[29].GetCheck())
	{
		m_nSelectCam0Pos = 1;
		myBtn[30].SetCheck(FALSE);
		myBtn[31].SetCheck(FALSE);
		myBtn[32].SetCheck(FALSE);
	}
	else
	{
		myBtn[29].SetCheck(TRUE);
	}	
}

void CDlgMenu02::OnChkMk0Pos2() 
{
	// TODO: Add your control notification handler code here
	if(myBtn[30].GetCheck())
	{
		m_nSelectCam0Pos = 2;
		myBtn[29].SetCheck(FALSE);
		myBtn[31].SetCheck(FALSE);
		myBtn[32].SetCheck(FALSE);
	}
	else
	{
		myBtn[30].SetCheck(TRUE);
	}	
}

void CDlgMenu02::OnChkMk0Pos3() 
{
	// TODO: Add your control notification handler code here
	if(myBtn[31].GetCheck())
	{
		m_nSelectCam0Pos = 3;
		myBtn[29].SetCheck(FALSE);
		myBtn[30].SetCheck(FALSE);
		myBtn[32].SetCheck(FALSE);
	}
	else
	{
		myBtn[31].SetCheck(TRUE);
	}	
}

void CDlgMenu02::OnChkMk0Pos4() 
{
	// TODO: Add your control notification handler code here
	if(myBtn[32].GetCheck())
	{
		m_nSelectCam0Pos = 4;
		myBtn[29].SetCheck(FALSE);
		myBtn[30].SetCheck(FALSE);
		myBtn[31].SetCheck(FALSE);
	}
	else
	{
		myBtn[32].SetCheck(TRUE);
	}	
}

void CDlgMenu02::OnBtnMk0PosMove() 
{
	// TODO: Add your control notification handler code here
	switch(m_nSelectCam0Pos) 
	{
	case 1:
		MovePos(0);
		break;
	case 2:
		MovePos(1);
		break;
	case 3:
		MovePos(2);
		break;
	case 4:
		MovePos(3);
		break;
	default:
		return;
	}	
}

void CDlgMenu02::OnChkMk1Pos1() 
{
	// TODO: Add your control notification handler code here
	if(myBtn2[24].GetCheck())
	{
		m_nSelectCam1Pos = 1;
		myBtn2[25].SetCheck(FALSE);
		myBtn2[26].SetCheck(FALSE);
		myBtn2[27].SetCheck(FALSE);
	}
	else
	{
		myBtn2[24].SetCheck(TRUE);
	}	
}

void CDlgMenu02::OnChkMk1Pos2() 
{
	// TODO: Add your control notification handler code here
	if(myBtn2[25].GetCheck())
	{
		m_nSelectCam1Pos = 2;
		myBtn2[24].SetCheck(FALSE);
		myBtn2[26].SetCheck(FALSE);
		myBtn2[27].SetCheck(FALSE);
	}
	else
	{
		myBtn2[25].SetCheck(TRUE);
	}	
}

void CDlgMenu02::OnChkMk1Pos3() 
{
	// TODO: Add your control notification handler code here
	if(myBtn2[26].GetCheck())
	{
		m_nSelectCam1Pos = 3;
		myBtn2[24].SetCheck(FALSE);
		myBtn2[25].SetCheck(FALSE);
		myBtn2[27].SetCheck(FALSE);
	}
	else
	{
		myBtn2[26].SetCheck(TRUE);
	}	
}

void CDlgMenu02::OnChkMk1Pos4() 
{
	// TODO: Add your control notification handler code here
	if(myBtn2[27].GetCheck())
	{
		m_nSelectCam1Pos = 4;
		myBtn2[24].SetCheck(FALSE);
		myBtn2[25].SetCheck(FALSE);
		myBtn2[26].SetCheck(FALSE);
	}
	else
	{
		myBtn2[27].SetCheck(TRUE);
	}	
}

void CDlgMenu02::OnBtnMk1PosMove() 
{
	// TODO: Add your control notification handler code here
	switch(m_nSelectCam1Pos) 
	{
	case 1:
		MovePos2(0);
		break;
	case 2:
		MovePos2(1);
		break;
	case 3:
		MovePos2(2);
		break;
	case 4:
		MovePos2(3);
		break;
	default:
		return;
	}	
}

void CDlgMenu02::OnBtnMk0PosMeas() 
{
	// TODO: Add your control notification handler code here
	CString strVal;
	double dFinalPos;

	switch(m_nSelectCam0Pos) 
	{
	case 1:
		dFinalPos = pView->m_pVoiceCoil[VOICE_COIL_FIRST_CAM]->GetSmacMeasureOfSurface(VOICE_COIL_FIRST_CAM);
		pView->m_pVoiceCoil[VOICE_COIL_FIRST_CAM]->ClearReceive();
		if(dFinalPos > 0.001)
		{
			strVal.Format(_T("%.2f"), dFinalPos);
			pDoc->WorkingInfo.VoiceCoil[VOICE_COIL_FIRST_CAM].sSmacHeightAtPos[0] = strVal;		
			myStcData[13].SetText(strVal);
			::WritePrivateProfileString(_T("VoiceCoil0"), _T("Smac_Distance_Pos1"), strVal, PATH_SMAC);
		}
		break;
	case 2:
		dFinalPos = pView->m_pVoiceCoil[VOICE_COIL_FIRST_CAM]->GetSmacMeasureOfSurface(VOICE_COIL_FIRST_CAM);
		pView->m_pVoiceCoil[VOICE_COIL_FIRST_CAM]->ClearReceive();
		if(dFinalPos > 0.001)
		{
			strVal.Format(_T("%.2f"), dFinalPos);
			pDoc->WorkingInfo.VoiceCoil[VOICE_COIL_FIRST_CAM].sSmacHeightAtPos[1] = strVal;		
			myStcData[14].SetText(strVal);
			::WritePrivateProfileString(_T("VoiceCoil0"), _T("Smac_Distance_Pos2"), strVal, PATH_SMAC);
		}
		break;
	case 3:
		dFinalPos = pView->m_pVoiceCoil[VOICE_COIL_FIRST_CAM]->GetSmacMeasureOfSurface(VOICE_COIL_FIRST_CAM);
		pView->m_pVoiceCoil[VOICE_COIL_FIRST_CAM]->ClearReceive();
		if(dFinalPos > 0.001)
		{
			strVal.Format(_T("%.2f"), dFinalPos);
			pDoc->WorkingInfo.VoiceCoil[VOICE_COIL_FIRST_CAM].sSmacHeightAtPos[2] = strVal;		
			myStcData[15].SetText(strVal);
			::WritePrivateProfileString(_T("VoiceCoil0"), _T("Smac_Distance_Pos3"), strVal, PATH_SMAC);
		}
		break;
	case 4:
		dFinalPos = pView->m_pVoiceCoil[VOICE_COIL_FIRST_CAM]->GetSmacMeasureOfSurface(VOICE_COIL_FIRST_CAM);
		pView->m_pVoiceCoil[VOICE_COIL_FIRST_CAM]->ClearReceive();
		if(dFinalPos > 0.001)
		{	
			strVal.Format(_T("%.2f"), dFinalPos);
			pDoc->WorkingInfo.VoiceCoil[VOICE_COIL_FIRST_CAM].sSmacHeightAtPos[3] = strVal;		
			myStcData[16].SetText(strVal);
			::WritePrivateProfileString(_T("VoiceCoil0"), _T("Smac_Distance_Pos4"), strVal, PATH_SMAC);
		}
		break;
	default:
		return;
	}

	pView->m_pVoiceCoil[VOICE_COIL_FIRST_CAM]->CalcAveSmacHeight(VOICE_COIL_FIRST_CAM);
}

void CDlgMenu02::OnBtnMk1PosMeas() 
{
	// TODO: Add your control notification handler code here
	CString strVal;
	double dFinalPos;

	switch(m_nSelectCam1Pos) 
	{
	case 1:
		dFinalPos = pView->m_pVoiceCoil[VOICE_COIL_SECOND_CAM]->GetSmacMeasureOfSurface(VOICE_COIL_SECOND_CAM);
		pView->m_pVoiceCoil[VOICE_COIL_SECOND_CAM]->ClearReceive();
		if(dFinalPos > 0.001)
		{
			strVal.Format(_T("%.2f"), dFinalPos);
			pDoc->WorkingInfo.VoiceCoil[VOICE_COIL_SECOND_CAM].sSmacHeightAtPos[0] = strVal;		
			myStcData2[13].SetText(strVal);
			::WritePrivateProfileString(_T("VoiceCoil1"), _T("Smac_Distance_Pos1"), strVal, PATH_SMAC);
		}
		break;
	case 2:
		dFinalPos = pView->m_pVoiceCoil[VOICE_COIL_SECOND_CAM]->GetSmacMeasureOfSurface(VOICE_COIL_SECOND_CAM);
		pView->m_pVoiceCoil[VOICE_COIL_SECOND_CAM]->ClearReceive();
		if(dFinalPos > 0.001)
		{
			strVal.Format(_T("%.2f"), dFinalPos);
			pDoc->WorkingInfo.VoiceCoil[VOICE_COIL_SECOND_CAM].sSmacHeightAtPos[1] = strVal;		
			myStcData2[14].SetText(strVal);
			::WritePrivateProfileString(_T("VoiceCoil1"), _T("Smac_Distance_Pos2"), strVal, PATH_SMAC);
		}
		break;
	case 3:
		dFinalPos = pView->m_pVoiceCoil[VOICE_COIL_SECOND_CAM]->GetSmacMeasureOfSurface(VOICE_COIL_SECOND_CAM);
		pView->m_pVoiceCoil[VOICE_COIL_SECOND_CAM]->ClearReceive();
		if(dFinalPos > 0.001)
		{
			strVal.Format(_T("%.2f"), dFinalPos);
			pDoc->WorkingInfo.VoiceCoil[VOICE_COIL_SECOND_CAM].sSmacHeightAtPos[2] = strVal;		
			myStcData2[15].SetText(strVal);
			::WritePrivateProfileString(_T("VoiceCoil1"), _T("Smac_Distance_Pos3"), strVal, PATH_SMAC);
		}
		break;
	case 4:
		dFinalPos = pView->m_pVoiceCoil[VOICE_COIL_SECOND_CAM]->GetSmacMeasureOfSurface(VOICE_COIL_SECOND_CAM);
		pView->m_pVoiceCoil[VOICE_COIL_SECOND_CAM]->ClearReceive();
		if(dFinalPos > 0.001)
		{	
			strVal.Format(_T("%.2f"), dFinalPos);
			pDoc->WorkingInfo.VoiceCoil[VOICE_COIL_SECOND_CAM].sSmacHeightAtPos[3] = strVal;		
			myStcData2[16].SetText(strVal);
			::WritePrivateProfileString(_T("VoiceCoil1"), _T("Smac_Distance_Pos4"), strVal, PATH_SMAC);
		}
		break;
	default:
		return;
	}

	pView->m_pVoiceCoil[VOICE_COIL_SECOND_CAM]->CalcAveSmacHeight(VOICE_COIL_SECOND_CAM);
}

void CDlgMenu02::OnBtnAlignMove2() 
{
	// TODO: Add your control notification handler code here
	if(m_nMoveAlign[1] == 0)
	{
		MoveAlign1(0);
		m_nMoveAlign[1] = 1;
	}
	else
	{
		MoveAlign1(1);
		m_nMoveAlign[1] = 0;
	}
}

void CDlgMenu02::OnBtnGrab2() 
{
	// TODO: Add your control notification handler code here
	if (pDoc->WorkingInfo.LastJob.nAlignMethode == FOUR_POINT)
		Grab4PntAlign2();
	else
		Grab2PntAlign2();
}

void CDlgMenu02::Grab2PntAlign2()
{
	CString str;
	double dX, dY, dAgl, dScr;

	double dCurrX = pView->m_dEnc[AXIS_X1];
	double dCurrY = pView->m_dEnc[AXIS_Y1];

	double pPos[8];
	pPos[0] = pDoc->m_Master[0].m_stAlignMk.X0 + pView->m_pMotion->m_dPinPosX[1];
	pPos[1] = pDoc->m_Master[0].m_stAlignMk.Y0 + pView->m_pMotion->m_dPinPosY[1];
	pPos[2] = pDoc->m_Master[0].m_stAlignMk.X1 + pView->m_pMotion->m_dPinPosX[1];
	pPos[3] = pDoc->m_Master[0].m_stAlignMk.Y1 + pView->m_pMotion->m_dPinPosY[1];

	int nPos = -1;
	if ((dCurrX > pPos[0] - 0.1 && dCurrX < pPos[0] + 0.1) && (dCurrY > pPos[1] - 0.1 && dCurrY < pPos[1] + 0.1))
		nPos = 0;
	else if ((dCurrX > pPos[2] - 0.1 && dCurrX < pPos[2] + 0.1) && (dCurrY > pPos[3] - 0.1 && dCurrY < pPos[3] + 0.1))
		nPos = 1;

	if (nPos == -1)
	{
		AfxMessageBox(_T("Error - Grab2PntAlign2() failed!"));
		return;
	}

#ifdef USE_VISION
	if (pView->m_pVision[1]->Grab(nPos))
	{
		GetPmRst1(dX, dY, dAgl, dScr);
		str.Format(_T("%.1f"), dX);
		pDoc->WorkingInfo.Motion.sAlignResultPosX[0][nPos] = str;
		myStcData2[7].SetText(str);

		str.Format(_T("%.1f"), dY);
		pDoc->WorkingInfo.Motion.sAlignResultPosY[0][nPos] = str;
		myStcData2[8].SetText(str);

		str.Format(_T("%.1f"), dAgl);
		pDoc->WorkingInfo.Motion.sAlignResultTheta[0][nPos] = str;
		myStcData2[9].SetText(str);

		str.Format(_T("%.1f"), dScr);
		pDoc->WorkingInfo.Motion.sAlignResultScore[0][nPos] = str;
		myStcData2[10].SetText(str);
	}
	else
	{
		myStcData2[7].SetText(_T(""));
		myStcData2[8].SetText(_T(""));
		myStcData2[9].SetText(_T(""));
		myStcData2[10].SetText(_T(""));
	}
#endif
}

void CDlgMenu02::Grab4PntAlign2()
{

	CString str;
	double dX, dY, dAgl, dScr;

	double dCurrX = pView->m_dEnc[AXIS_X1];
	double dCurrY = pView->m_dEnc[AXIS_Y1];

	double pPos[8];
	pPos[0] = pDoc->m_Master[0].m_stAlignMk2.X0 + pView->m_pMotion->m_dPinPosX[1];
	pPos[1] = pDoc->m_Master[0].m_stAlignMk2.Y0 + pView->m_pMotion->m_dPinPosY[1];
	pPos[2] = pDoc->m_Master[0].m_stAlignMk2.X1 + pView->m_pMotion->m_dPinPosX[1];
	pPos[3] = pDoc->m_Master[0].m_stAlignMk2.Y1 + pView->m_pMotion->m_dPinPosY[1];

	pPos[4] = pDoc->m_Master[0].m_stAlignMk2.X2 + pView->m_pMotion->m_dPinPosX[1];
	pPos[5] = pDoc->m_Master[0].m_stAlignMk2.Y2 + pView->m_pMotion->m_dPinPosY[1];
	pPos[6] = pDoc->m_Master[0].m_stAlignMk2.X3 + pView->m_pMotion->m_dPinPosX[1];
	pPos[7] = pDoc->m_Master[0].m_stAlignMk2.Y3 + pView->m_pMotion->m_dPinPosY[1];

	int nPos = -1;
	if ((dCurrX > pPos[0] - 0.1 && dCurrX < pPos[0] + 0.1) && (dCurrY > pPos[1] - 0.1 && dCurrY < pPos[1] + 0.1))
		nPos = 0;
	else if ((dCurrX > pPos[2] - 0.1 && dCurrX < pPos[2] + 0.1) && (dCurrY > pPos[3] - 0.1 && dCurrY < pPos[3] + 0.1))
		nPos = 1;
	else if ((dCurrX > pPos[4] - 0.1 && dCurrX < pPos[4] + 0.1) && (dCurrY > pPos[5] - 0.1 && dCurrY < pPos[5] + 0.1))
		nPos = 0;
	else if ((dCurrX > pPos[6] - 0.1 && dCurrX < pPos[6] + 0.1) && (dCurrY > pPos[7] - 0.1 && dCurrY < pPos[7] + 0.1))
		nPos = 1;

	if (nPos == -1)
	{
		AfxMessageBox(_T("Error - Grab4PntAlign2() failed!"));
		return;
	}

#ifdef USE_VISION
	if (pView->m_pVision[1]->Grab(nPos))
	{
		GetPmRst1(dX, dY, dAgl, dScr);
		str.Format(_T("%.1f"), dX);
		pDoc->WorkingInfo.Motion.sAlignResultPosX[0][nPos] = str;
		myStcData2[7].SetText(str);

		str.Format(_T("%.1f"), dY);
		pDoc->WorkingInfo.Motion.sAlignResultPosY[0][nPos] = str;
		myStcData2[8].SetText(str);

		str.Format(_T("%.1f"), dAgl);
		pDoc->WorkingInfo.Motion.sAlignResultTheta[0][nPos] = str;
		myStcData2[9].SetText(str);

		str.Format(_T("%.1f"), dScr);
		pDoc->WorkingInfo.Motion.sAlignResultScore[0][nPos] = str;
		myStcData2[10].SetText(str);
	}
	else
	{
		myStcData2[7].SetText(_T(""));
		myStcData2[8].SetText(_T(""));
		myStcData2[9].SetText(_T(""));
		myStcData2[10].SetText(_T(""));
	}
#endif
}

void CDlgMenu02::OnStcAlignStdScr2() 
{
	// TODO: Add your control notification handler code here
	myStcData2[12].SetBkColor(RGB_RED);
	myStcData2[12].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_ALIGN_STD_SCR2)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_ALIGN_STD_SCR2, pt, TO_BOTTOM|TO_RIGHT);

	myStcData2[12].SetBkColor(RGB_WHITE);
	myStcData2[12].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_ALIGN_STD_SCR2)->GetWindowText(sData);
	pDoc->WorkingInfo.Vision[1].sStdScr = sData;
	::WritePrivateProfileString(_T("Vision1"), _T("ALIGN_STD_SCORE"), sData, PATH_WORKING_INFO);
}

void CDlgMenu02::ShowDlg(int nID)
{
	switch(nID) 
	{
	case IDD_DLG_UTIL_03:
		if(!m_pDlgUtil03)
		{
			m_pDlgUtil03 = new CDlgUtil03(this);
			if(m_pDlgUtil03->GetSafeHwnd() == 0)
			{
				m_pDlgUtil03->Create();
				m_pDlgUtil03->ShowWindow(SW_SHOW);
			}
		}
		else
		{
			m_pDlgUtil03->ShowWindow(SW_SHOW);
		}
		break;	
#ifdef USE_FLUCK
	case IDD_DLG_UTIL_06:
		if(!m_pDlgUtil06)
		{
			m_pDlgUtil06 = new CDlgUtil06(this);
			if(m_pDlgUtil06->GetSafeHwnd() == 0)
			{
				m_pDlgUtil06->Create();
				m_pDlgUtil06->ShowWindow(SW_SHOW);
			}
		}
		else
		{
			m_pDlgUtil06->ShowWindow(SW_SHOW);
		}
		break;	
#endif
	}
}

void CDlgMenu02::ResetMkTestBtn()
{
//	m_lChk &= ~(0x01<<0);
	m_lChk = 0;
	SetTimer(TIM_SHOW_MK_TEST, 30, NULL);

// 	BOOL bOn0 = myBtn[16].GetCheck();
// 	if(bOn0)
// 		myBtn[16].SetCheck(FALSE);
// 
// 	BOOL bOn1 = myBtn2[16].GetCheck();
// 	if(bOn1)
// 		myBtn2[16].SetCheck(FALSE);
}

void CDlgMenu02::ChkElecTest() 
{
	// TODO: Add your control notification handler code here
	BOOL bOn = myBtn[16].GetCheck();
	if(bOn)
	{
		CRect rt;		
		GetDlgItem(IDC_STATIC_MK_TEST)->GetWindowRect(&rt);
		//if(m_pDlgUtil06)
		//{
		//	if(!m_pDlgUtil06->IsWindowVisible())
		//	{
		//		m_pDlgUtil06->ShowWindow(SW_SHOW);
		//	}
		//	m_pDlgUtil06->MoveWindow(rt, TRUE);
		//}
	}
	else
	{
		//if(m_pDlgUtil06)
		//{
		//	if(m_pDlgUtil06->IsWindowVisible())
		//	{
		//		m_pDlgUtil06->ShowWindow(SW_HIDE);

		//		if(pView->m_bProbDn[0])
		//		{
		//			if(pView->m_pVoiceCoil[0])
		//			{
		//				pView->m_pVoiceCoil[0]->SearchHomeSmac(0);
		//				pView->m_pVoiceCoil[0]->MoveSmacShiftPos(0);
		//				pView->m_bProbDn[0] = FALSE;
		//				if(m_pDlgUtil06)
		//					m_pDlgUtil06->myBtn[2].SetCheck(FALSE);
		//			}
		//		}

		//		if(pView->m_bProbDn[1])
		//		{
		//			if(pView->m_pVoiceCoil[1])
		//			{
		//				pView->m_pVoiceCoil[1]->SearchHomeSmac(1);
		//				pView->m_pVoiceCoil[1]->MoveSmacShiftPos(1);
		//				pView->m_bProbDn[1] = FALSE;
		//				if(m_pDlgUtil06)
		//					m_pDlgUtil06->myBtn[6].SetCheck(FALSE);
		//			}
		//		}

		//	}
		//}
		//m_lChk = 0;
	}
}


void CDlgMenu02::OnChkElecTest() 
{
	// TODO: Add your control notification handler code here
	m_lChk = 2;
	SetTimer(TIM_SHOW_ELEC_TEST, 150, NULL);
	
	//ChkElecTest();
}



BOOL CDlgMenu02::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB_DLG_FRM);
	return TRUE;

	//return CDialog::OnEraseBkgnd(pDC);
}


// OnePointAlign==================================================================
BOOL CDlgMenu02::OnePointAlign(CfPoint &ptPnt)
{
#ifdef USE_VISION
	double dRefPinX, dRefPinY; // Cam Pos.
	double dTgtPinX, dTgtPinY; // Grab Pos.
	double dResX, dResY, dResCam;
	CString str;

	m_dMkFdOffsetX[0][0] = 0.0;

	if (!pView->m_pMotion || !pView->m_pVision[0])
		return FALSE;

	if (pDoc->WorkingInfo.Vision[0].sResX.IsEmpty() || pDoc->WorkingInfo.Vision[0].sResY.IsEmpty())
		return FALSE;

	if (pView->m_pVision[0]->Grab(0))
	{
		double dX, dY, dAgl, dScr;
		GetPmRst0(dX, dY, dAgl, dScr);
		str.Format(_T("%.1f"), dX);
		myStcData[7].SetText(str);
		str.Format(_T("%.1f"), dY);
		myStcData[8].SetText(str);
		str.Format(_T("%.1f"), dAgl);
		myStcData[9].SetText(str);
		str.Format(_T("%.1f"), dScr);
		myStcData[10].SetText(str);
	}
	else
	{
		myStcData[7].SetText(_T(""));
		myStcData[8].SetText(_T(""));
		myStcData[9].SetText(_T(""));
		myStcData[10].SetText(_T(""));
		return FALSE;
	}

	int nCamSzX, nCamSzY;
	pView->m_pVision[0]->GetCameraSize(nCamSzX, nCamSzY);

	dResX = _tstof(pDoc->WorkingInfo.Vision[0].sResX);
	dResY = _tstof(pDoc->WorkingInfo.Vision[0].sResY);
	dResCam = _tstof(pDoc->WorkingInfo.Vision[0].sCamPxlRes) / 10000.0;

	dRefPinX = (double)pDoc->m_Master[0].m_pCellRgn->nCADPinPosPixX * dResCam;
	dRefPinY = (double)pDoc->m_Master[0].m_pCellRgn->nCADPinPosPixY * dResCam;

	double dX, dY, dAgl, dScr;
	GetPmRst0(dX, dY, dAgl, dScr);

	dTgtPinX = pView->m_pMotion->m_dPinPosX[0] + (dX - double(nCamSzX / 2)) * dResX;
	dTgtPinY = pView->m_pMotion->m_dPinPosY[0] + (dY - double(nCamSzY / 2)) * dResY;
	m_dMkFdOffsetX[0][0] = (double(nCamSzX / 2) - dX) * dResX;
	m_dMkFdOffsetY[0][0] = (double(nCamSzY / 2) - dY) * dResY;
	// 	dTgtPinY = pView->m_pMotion->m_dPinPosY + m_dMkFdOffsetX;

	ptPnt.x = dTgtPinX;
	ptPnt.y = dTgtPinY;

	pView->m_Align[0].SetAlignData(dRefPinX, dRefPinY, dTgtPinX, dTgtPinY);
	//	pDoc->m_pPcsRgn->SetMkPnt();
#endif
	return TRUE;
}

BOOL CDlgMenu02::OnePointAlign2(CfPoint &ptPnt)
{
#ifdef USE_VISION
	double dRefPinX, dRefPinY; // Cam Pos.
	double dTgtPinX, dTgtPinY; // Grab Pos.
	double dResX, dResY, dResCam;
	CString str;

	m_dMkFdOffsetX[1][0] = 0.0;

	if (!pView->m_pMotion || !pView->m_pVision[1])
		return FALSE;

	if (pDoc->WorkingInfo.Vision[1].sResX.IsEmpty() || pDoc->WorkingInfo.Vision[1].sResY.IsEmpty())
		return FALSE;

	if (pView->m_pVision[1]->Grab(0))
	{
		double dX, dY, dAgl, dScr;
		GetPmRst1(dX, dY, dAgl, dScr);
		str.Format(_T("%.1f"), dX);
		myStcData2[7].SetText(str);
		str.Format(_T("%.1f"), dY);
		myStcData2[8].SetText(str);
		str.Format(_T("%.1f"), dAgl);
		myStcData2[9].SetText(str);
		str.Format(_T("%.1f"), dScr);
		myStcData2[10].SetText(str);
	}
	else
	{
		myStcData2[7].SetText(_T(""));
		myStcData2[8].SetText(_T(""));
		myStcData2[9].SetText(_T(""));
		myStcData2[10].SetText(_T(""));
		return FALSE;
	}

	int nCamSzX, nCamSzY;
	pView->m_pVision[1]->GetCameraSize(nCamSzX, nCamSzY);

	dResX = _tstof(pDoc->WorkingInfo.Vision[1].sResX);
	dResY = _tstof(pDoc->WorkingInfo.Vision[1].sResY);
	dResCam = _tstof(pDoc->WorkingInfo.Vision[1].sCamPxlRes) / 10000.0;

	dRefPinX = (double)pDoc->m_Master[0].m_pCellRgn->nCADPinPosPixX * dResCam;
	dRefPinY = (double)pDoc->m_Master[0].m_pCellRgn->nCADPinPosPixY * dResCam;

	double dX, dY, dAgl, dScr;
	GetPmRst1(dX, dY, dAgl, dScr);

	dTgtPinX = pView->m_pMotion->m_dPinPosX[1] + (dX - double(nCamSzX / 2)) * dResX;
	dTgtPinY = pView->m_pMotion->m_dPinPosY[1] + (dY - double(nCamSzY / 2)) * dResY;
	m_dMkFdOffsetX[1][0] = (double(nCamSzX / 2) - dX) * dResX;
	m_dMkFdOffsetY[1][0] = (double(nCamSzY / 2) - dY) * dResY;
	// 	dTgtPinY = pView->m_pMotion->m_dPinPosY + m_dMkFdOffsetX;

	ptPnt.x = dTgtPinX;
	ptPnt.y = dTgtPinY;

	pView->m_Align[1].SetAlignData(dRefPinX, dRefPinY, dTgtPinX, dTgtPinY);
	//	pDoc->m_pPcsRgn->SetMkPnt();
#endif
	return TRUE;
}


// TwoPointAlign==================================================================
BOOL CDlgMenu02::TwoPointAlign(int nPos)
{
	BOOL bRtn[2];
	bRtn[0] = TwoPointAlign0(nPos);
	bRtn[1] = TwoPointAlign1(nPos);

	if (bRtn[0] && bRtn[1])
		return TRUE;
	return FALSE;
}

BOOL CDlgMenu02::TwoPointAlign0(int nPos)
{
#ifdef USE_VISION
	if (!pView->m_pMotion || !pView->m_pVision[0])
		return FALSE;
#endif
	if (pDoc->WorkingInfo.Vision[0].sResX.IsEmpty() || pDoc->WorkingInfo.Vision[0].sResY.IsEmpty())
		return FALSE;

	MoveAlign0(nPos);

	return (Do2PtAlign0(nPos, TRUE));
}

BOOL CDlgMenu02::Do2PtAlign0(int nPos, BOOL bDraw)
{
#ifdef USE_VISION
	double dRefPinX, dRefPinY; // Cam Pos.
	double dTgtPinX, dTgtPinY; // Grab Pos.
	double dResX, dResY, dResCam;
	double fLen, fVel, fAcc, fJerk;
	CString str;
	double pPos[2];
	CfPoint ptPnt;
	double dX, dY, dAgl, dScr;

	if (pView->m_pVision[0]->Grab(nPos, bDraw))
	{
		GetPmRst0(dX, dY, dAgl, dScr);
		str.Format(_T("%.1f"), dX);
		pDoc->WorkingInfo.Motion.sAlignResultPosX[0][nPos] = str;
		myStcData[7].SetText(str);

		str.Format(_T("%.1f"), dY);
		pDoc->WorkingInfo.Motion.sAlignResultPosY[0][nPos] = str;
		myStcData[8].SetText(str);

		str.Format(_T("%.1f"), dAgl);
		pDoc->WorkingInfo.Motion.sAlignResultTheta[0][nPos] = str;
		myStcData[9].SetText(str);

		str.Format(_T("%.1f"), dScr);
		pDoc->WorkingInfo.Motion.sAlignResultScore[0][nPos] = str;
		myStcData[10].SetText(str);
	}
	else
	{
		Sleep(100);

		if (pView->m_pVision[0]->Grab(nPos, bDraw))
		{
			GetPmRst0(dX, dY, dAgl, dScr);
			str.Format(_T("%.1f"), dX);
			pDoc->WorkingInfo.Motion.sAlignResultPosX[0][nPos] = str;
			myStcData[7].SetText(str);

			str.Format(_T("%.1f"), dY);
			pDoc->WorkingInfo.Motion.sAlignResultPosY[0][nPos] = str;
			myStcData[8].SetText(str);

			str.Format(_T("%.1f"), dAgl);
			pDoc->WorkingInfo.Motion.sAlignResultTheta[0][nPos] = str;
			myStcData[9].SetText(str);

			str.Format(_T("%.1f"), dScr);
			pDoc->WorkingInfo.Motion.sAlignResultScore[0][nPos] = str;
			myStcData[10].SetText(str);
		}
		else
		{
			myStcData[7].SetText(_T(""));
			myStcData[8].SetText(_T(""));
			myStcData[9].SetText(_T(""));
			myStcData[10].SetText(_T(""));
			return FALSE;
		}
	}

	int nCamSzX, nCamSzY;
	pView->m_pVision[0]->GetCameraSize(nCamSzX, nCamSzY);

	dResX = _tstof(pDoc->WorkingInfo.Vision[0].sResX);
	dResY = _tstof(pDoc->WorkingInfo.Vision[0].sResY);
	dResCam = _tstof(pDoc->WorkingInfo.Vision[0].sCamPxlRes) / 10000.0;

	GetPmRst0(dX, dY, dAgl, dScr);

	if (nPos == 0)
	{
		m_dMkFdOffsetX[0][0] = (double(nCamSzX / 2) - dX) * dResX; // -: 제품 덜옴, +: 제품 더옴.
		m_dMkFdOffsetY[0][0] = (double(nCamSzY / 2) - dY) * dResY; // -: 제품 나옴, +: 제품 들어감.

#ifdef USE_MPE
		//pView->IoWrite("ML45066", (long)(-1.0 * m_dMkFdOffsetX[0] * 1000.0));	// 마킹부 Feeding 롤러 Offset(*1000, +:더 보냄, -덜 보냄)
		pView->m_pMpe->Write(_T("ML45066"), (long)(-1.0 * m_dMkFdOffsetX[0][0] * 1000.0));
#endif
	}

	if (nPos == 1)
	{
		double dMkFdOffsetX = (double(nCamSzX / 2) - dX) * dResX; // -: 제품 덜옴, +: 제품 더옴.
		double dMkFdOffsetY = (double(nCamSzY / 2) - dY) * dResY; // -: 제품 나옴, +: 제품 들어감.
		m_dMkFdOffsetX[0][1] = dMkFdOffsetX;
		m_dMkFdOffsetY[0][1] = dMkFdOffsetY;

																  // Cam의 원점 기준의 Marking 이미지 좌표.
		double dRefAlignX0 = pDoc->m_Master[0].m_stAlignMk.X0 + pView->m_pMotion->m_dPinPosX[0]; // PCB좌표
		double dRefAlignY0 = pDoc->m_Master[0].m_stAlignMk.Y0 + pView->m_pMotion->m_dPinPosY[0]; // PCB좌표
		double dRefAlignX1 = pDoc->m_Master[0].m_stAlignMk.X1 + pView->m_pMotion->m_dPinPosX[0]; // PCB좌표
		double dRefAlignY1 = pDoc->m_Master[0].m_stAlignMk.Y1 + pView->m_pMotion->m_dPinPosY[0]; // PCB좌표

																								 // PCB상의 원점 기준의 Marking 이미지 좌표.
		double dTgtAlignX0 = (pDoc->m_Master[0].m_stAlignMk.X0 + pView->m_pMotion->m_dPinPosX[0]) - m_dMkFdOffsetX[0][0];
		double dTgtAlignY0 = (pDoc->m_Master[0].m_stAlignMk.Y0 + pView->m_pMotion->m_dPinPosY[0]) - m_dMkFdOffsetY[0][0];
		double dTgtAlignX1 = (pDoc->m_Master[0].m_stAlignMk.X1 + pView->m_pMotion->m_dPinPosX[0]) - dMkFdOffsetX;
		double dTgtAlignY1 = (pDoc->m_Master[0].m_stAlignMk.Y1 + pView->m_pMotion->m_dPinPosY[0]) - dMkFdOffsetY;

		int nNodeX = 0, nNodeY = 0;
		if (pDoc->m_Master[0].m_pPcsRgn)
		{
			nNodeX = pDoc->m_Master[0].m_pPcsRgn->nCol;
			nNodeY = pDoc->m_Master[0].m_pPcsRgn->nRow;
		}

		pView->m_Align[0].SetAlignData(dRefAlignX0, dRefAlignY0, dRefAlignX1, dRefAlignY1, dTgtAlignX0, dTgtAlignY0, dTgtAlignX1, dTgtAlignY1);

		CfPoint ptRef, ptTgt;
		int nCol, nRow, idx = 0;
		for (nCol = 0; nCol < nNodeX; nCol++)
		{
			for (nRow = 0; nRow < nNodeY; nRow++)
			{
				ptRef.x = pDoc->m_Master[0].m_stPcsMk[idx].X + pView->m_pMotion->m_dPinPosX[0];
				ptRef.y = pDoc->m_Master[0].m_stPcsMk[idx].Y + pView->m_pMotion->m_dPinPosY[0];
				pView->m_Align[0].LinearAlignment(ptRef, ptTgt);
				if (pDoc->m_Master[0].m_pPcsRgn)
				{
					pDoc->m_Master[0].m_pPcsRgn->pMkPnt[0][idx].x = ptTgt.x;
					pDoc->m_Master[0].m_pPcsRgn->pMkPnt[0][idx].y = ptTgt.y;
				}
				idx++;
			}
		}

	}
#endif
	return TRUE;
}


BOOL CDlgMenu02::TwoPointAlign1(int nPos)
{
#ifdef USE_VISION
	if (!pView->m_pMotion || !pView->m_pVision[1])
		return FALSE;

	if (pDoc->WorkingInfo.Vision[1].sResX.IsEmpty() || pDoc->WorkingInfo.Vision[1].sResY.IsEmpty())
		return FALSE;

	MoveAlign1(nPos);

	return (Do2PtAlign1(nPos, TRUE));
#else
	return TRUE;
#endif
}

BOOL CDlgMenu02::Do2PtAlign1(int nPos, BOOL bDraw)
{
#ifdef USE_VISION
	double dRefPinX, dRefPinY; // Cam Pos.
	double dTgtPinX, dTgtPinY; // Grab Pos.
	double dResX, dResY, dResCam;
	double fLen, fVel, fAcc, fJerk;
	CString str;
	double pPos[2];
	CfPoint ptPnt;
	double dX, dY, dAgl, dScr;

	if (pView->m_pVision[1]->Grab(nPos, bDraw))
	{
		GetPmRst1(dX, dY, dAgl, dScr);
		str.Format(_T("%.1f"), dX);
		pDoc->WorkingInfo.Motion.sAlignResultPosX[1][nPos] = str;
		myStcData2[7].SetText(str);

		str.Format(_T("%.1f"), dY);
		pDoc->WorkingInfo.Motion.sAlignResultPosY[1][nPos] = str;
		myStcData2[8].SetText(str);

		str.Format(_T("%.1f"), dAgl);
		pDoc->WorkingInfo.Motion.sAlignResultTheta[1][nPos] = str;
		myStcData2[9].SetText(str);

		str.Format(_T("%.1f"), dScr);
		pDoc->WorkingInfo.Motion.sAlignResultScore[1][nPos] = str;
		myStcData2[10].SetText(str);
	}
	else
	{
		Sleep(100);

		if (pView->m_pVision[1]->Grab(nPos, bDraw))
		{
			GetPmRst1(dX, dY, dAgl, dScr);
			str.Format(_T("%.1f"), dX);
			pDoc->WorkingInfo.Motion.sAlignResultPosX[1][nPos] = str;
			myStcData2[7].SetText(str);

			str.Format(_T("%.1f"), dY);
			pDoc->WorkingInfo.Motion.sAlignResultPosY[1][nPos] = str;
			myStcData2[8].SetText(str);

			str.Format(_T("%.1f"), dAgl);
			pDoc->WorkingInfo.Motion.sAlignResultTheta[1][nPos] = str;
			myStcData2[9].SetText(str);

			str.Format(_T("%.1f"), dScr);
			pDoc->WorkingInfo.Motion.sAlignResultScore[1][nPos] = str;
			myStcData2[10].SetText(str);
		}
		else
		{
			myStcData2[7].SetText(_T(""));
			myStcData2[8].SetText(_T(""));
			myStcData2[9].SetText(_T(""));
			myStcData2[10].SetText(_T(""));
			return FALSE;
		}
	}

	int nCamSzX, nCamSzY;
	pView->m_pVision[1]->GetCameraSize(nCamSzX, nCamSzY);

	dResX = _tstof(pDoc->WorkingInfo.Vision[1].sResX);
	dResY = _tstof(pDoc->WorkingInfo.Vision[1].sResY);
	dResCam = _tstof(pDoc->WorkingInfo.Vision[1].sCamPxlRes) / 10000.0;

	GetPmRst1(dX, dY, dAgl, dScr);

	if (nPos == 0)
	{
		m_dMkFdOffsetX[1][0] = (double(nCamSzX / 2) - dX) * dResX; // -: 제품 덜옴, +: 제품 더옴.
		m_dMkFdOffsetY[1][0] = (double(nCamSzY / 2) - dY) * dResY; // -: 제품 나옴, +: 제품 들어감.

#ifdef USE_MPE
		//pView->IoWrite("ML45066", (long)(-1.0 * m_dMkFdOffsetX[1] * 1000.0));	// 마킹부 Feeding 롤러 Offset(*1000, +:더 보냄, -덜 보냄)
		pView->m_pMpe->Write(_T("ML45066"), (long)(-1.0 * m_dMkFdOffsetX[1][0] * 1000.0));
#endif
	}

	if (nPos == 1)
	{
		double dMkFdOffsetX = (double(nCamSzX / 2) - dX) * dResX; // -: 제품 덜옴, +: 제품 더옴.
		double dMkFdOffsetY = (double(nCamSzY / 2) - dY) * dResY; // -: 제품 나옴, +: 제품 들어감.
		m_dMkFdOffsetX[1][1] = dMkFdOffsetX;
		m_dMkFdOffsetY[1][1] = dMkFdOffsetY;

		// Cam의 원점 기준의 Marking 이미지 좌표.
		double dRefAlignX0 = pDoc->m_Master[0].m_stAlignMk.X0 + pView->m_pMotion->m_dPinPosX[1]; // PCB좌표
		double dRefAlignY0 = pDoc->m_Master[0].m_stAlignMk.Y0 + pView->m_pMotion->m_dPinPosY[1]; // PCB좌표
		double dRefAlignX1 = pDoc->m_Master[0].m_stAlignMk.X1 + pView->m_pMotion->m_dPinPosX[1]; // PCB좌표
		double dRefAlignY1 = pDoc->m_Master[0].m_stAlignMk.Y1 + pView->m_pMotion->m_dPinPosY[1]; // PCB좌표

		// PCB상의 원점 기준의 Marking 이미지 좌표.
		double dTgtAlignX0 = (pDoc->m_Master[0].m_stAlignMk.X0 + pView->m_pMotion->m_dPinPosX[1]) - m_dMkFdOffsetX[1][0];
		double dTgtAlignY0 = (pDoc->m_Master[0].m_stAlignMk.Y0 + pView->m_pMotion->m_dPinPosY[1]) - m_dMkFdOffsetY[1][0];
		double dTgtAlignX1 = (pDoc->m_Master[0].m_stAlignMk.X1 + pView->m_pMotion->m_dPinPosX[1]) - dMkFdOffsetX;
		double dTgtAlignY1 = (pDoc->m_Master[0].m_stAlignMk.Y1 + pView->m_pMotion->m_dPinPosY[1]) - dMkFdOffsetY;

		int nNodeX = 0, nNodeY = 0;
		if (pDoc->m_Master[0].m_pPcsRgn)
		{
			nNodeX = pDoc->m_Master[0].m_pPcsRgn->nCol;
			nNodeY = pDoc->m_Master[0].m_pPcsRgn->nRow;
		}

		pView->m_Align[1].SetAlignData(dRefAlignX0, dRefAlignY0, dRefAlignX1, dRefAlignY1, dTgtAlignX0, dTgtAlignY0, dTgtAlignX1, dTgtAlignY1);

		CfPoint ptRef, ptTgt;
		int nCol, nRow, idx = 0;
		for (nCol = 0; nCol < nNodeX; nCol++)
		{
			for (nRow = 0; nRow < nNodeY; nRow++)
			{
				ptRef.x = pDoc->m_Master[0].m_stPcsMk[idx].X + pView->m_pMotion->m_dPinPosX[1];
				ptRef.y = pDoc->m_Master[0].m_stPcsMk[idx].Y + pView->m_pMotion->m_dPinPosY[1];
				pView->m_Align[1].LinearAlignment(ptRef, ptTgt);
				if (pDoc->m_Master[0].m_pPcsRgn)
				{
					pDoc->m_Master[0].m_pPcsRgn->pMkPnt[1][idx].x = ptTgt.x;
					pDoc->m_Master[0].m_pPcsRgn->pMkPnt[1][idx].y = ptTgt.y;
				}
				idx++;
			}
		}
#ifdef USE_FLUCK
		// for Elec Check Point
		ptRef.x = _tstof(pDoc->WorkingInfo.Probing[1].sMeasurePosX);
		ptRef.y = _tstof(pDoc->WorkingInfo.Probing[1].sMeasurePosY);
		pView->m_Align[1].LinearAlignment(ptRef, ptTgt); //.LinearAlignment(ptRef, ptTgt);
		pDoc->WorkingInfo.Fluck.dMeasPosX[1] = ptTgt.x;
		pDoc->WorkingInfo.Fluck.dMeasPosY[1] = ptTgt.y;
#endif
	}
#endif
	return TRUE;
}

// FourPointAlign==================================================================
BOOL CDlgMenu02::FourPointAlign(int nPos)
{
	BOOL bRtn[2];
	bRtn[0] = FourPointAlign0(nPos);
	bRtn[1] = FourPointAlign1(nPos);

	if (bRtn[0] && bRtn[1])
		return TRUE;
	return FALSE;
}

BOOL CDlgMenu02::FourPointAlign0(int nPos)
{
#ifdef USE_VISION
	if (!pView->m_pMotion || !pView->m_pVision[0])
		return FALSE;
#endif
	if (pDoc->WorkingInfo.Vision[0].sResX.IsEmpty() || pDoc->WorkingInfo.Vision[0].sResY.IsEmpty())
		return FALSE;

	MoveAlign0(nPos);

	return (Do4PtAlign0(nPos, TRUE));
}

BOOL CDlgMenu02::Do4PtAlign0(int nPos, BOOL bDraw)
{
#ifdef USE_VISION
	double dRefPinX, dRefPinY; // Cam Pos.
	double dTgtPinX, dTgtPinY; // Grab Pos.
	double dResX, dResY, dResCam;
	double fLen, fVel, fAcc, fJerk;
	CString str;
	double pPos[2];
	CfPoint ptPnt;
	double dX, dY, dAgl, dScr;

	if (pView->m_pVision[0]->Grab(nPos, bDraw))
	{
		GetPmRst0(dX, dY, dAgl, dScr);
		str.Format(_T("%.1f"), dX);
		pDoc->WorkingInfo.Motion.sAlignResultPosX[0][nPos] = str;
		myStcData[7].SetText(str);

		str.Format(_T("%.1f"), dY);
		pDoc->WorkingInfo.Motion.sAlignResultPosY[0][nPos] = str;
		myStcData[8].SetText(str);

		str.Format(_T("%.1f"), dAgl);
		pDoc->WorkingInfo.Motion.sAlignResultTheta[0][nPos] = str;
		myStcData[9].SetText(str);

		str.Format(_T("%.1f"), dScr);
		pDoc->WorkingInfo.Motion.sAlignResultScore[0][nPos] = str;
		myStcData[10].SetText(str);
	}
	else
	{
		Sleep(100);

		if (pView->m_pVision[0]->Grab(nPos, bDraw))
		{
			GetPmRst0(dX, dY, dAgl, dScr);
			str.Format(_T("%.1f"), dX);
			pDoc->WorkingInfo.Motion.sAlignResultPosX[0][nPos] = str;
			myStcData[7].SetText(str);

			str.Format(_T("%.1f"), dY);
			pDoc->WorkingInfo.Motion.sAlignResultPosY[0][nPos] = str;
			myStcData[8].SetText(str);

			str.Format(_T("%.1f"), dAgl);
			pDoc->WorkingInfo.Motion.sAlignResultTheta[0][nPos] = str;
			myStcData[9].SetText(str);

			str.Format(_T("%.1f"), dScr);
			pDoc->WorkingInfo.Motion.sAlignResultScore[0][nPos] = str;
			myStcData[10].SetText(str);
		}
		else
		{
			myStcData[7].SetText(_T(""));
			myStcData[8].SetText(_T(""));
			myStcData[9].SetText(_T(""));
			myStcData[10].SetText(_T(""));
			return FALSE;
		}
	}

	int nCamSzX, nCamSzY;
	pView->m_pVision[0]->GetCameraSize(nCamSzX, nCamSzY);

	dResX = _tstof(pDoc->WorkingInfo.Vision[0].sResX);
	dResY = _tstof(pDoc->WorkingInfo.Vision[0].sResY);
	dResCam = _tstof(pDoc->WorkingInfo.Vision[0].sCamPxlRes) / 10000.0;

	GetPmRst0(dX, dY, dAgl, dScr);

	if (nPos == 0)
	{
		m_dMkFdOffsetX[0][0] = (double(nCamSzX / 2) - dX) * dResX; // -: 제품 덜옴, +: 제품 더옴.
		m_dMkFdOffsetY[0][0] = (double(nCamSzY / 2) - dY) * dResY; // -: 제품 나옴, +: 제품 들어감.
#ifdef USE_MPE
		//pView->IoWrite("ML45066", (long)(-1.0 * m_dMkFdOffsetX[0] * 1000.0));	// 마킹부 Feeding 롤러 Offset(*1000, +:더 보냄, -덜 보냄)
		pView->m_pMpe->Write(_T("ML45066"), (long)(-1.0 * m_dMkFdOffsetX[0][0] * 1000.0));
#endif
	}
	else if (nPos == 1)
	{
		m_dMkFdOffsetX[0][1] = (double(nCamSzX / 2) - dX) * dResX; // -: 제품 덜옴, +: 제품 더옴.
		m_dMkFdOffsetY[0][1] = (double(nCamSzY / 2) - dY) * dResY; // -: 제품 나옴, +: 제품 들어감.
	}
	else if (nPos == 2)
	{
		m_dMkFdOffsetX[0][2] = (double(nCamSzX / 2) - dX) * dResX; // -: 제품 덜옴, +: 제품 더옴.
		m_dMkFdOffsetY[0][2] = (double(nCamSzY / 2) - dY) * dResY; // -: 제품 나옴, +: 제품 들어감.
	}
	else if (nPos == 3)
	{
		double dMkFdOffsetX = (double(nCamSzX / 2) - dX) * dResX; // -: 제품 덜옴, +: 제품 더옴.
		double dMkFdOffsetY = (double(nCamSzY / 2) - dY) * dResY; // -: 제품 나옴, +: 제품 들어감.
		m_dMkFdOffsetX[0][3] = dMkFdOffsetX;
		m_dMkFdOffsetY[0][3] = dMkFdOffsetY;

		// Cam의 원점 기준의 Marking 이미지 좌표.
		double dRefAlignX0 = pDoc->m_Master[0].m_stAlignMk2.X0 + pView->m_pMotion->m_dPinPosX[0]; // PCB좌표
		double dRefAlignY0 = pDoc->m_Master[0].m_stAlignMk2.Y0 + pView->m_pMotion->m_dPinPosY[0]; // PCB좌표
		double dRefAlignX1 = pDoc->m_Master[0].m_stAlignMk2.X1 + pView->m_pMotion->m_dPinPosX[0]; // PCB좌표
		double dRefAlignY1 = pDoc->m_Master[0].m_stAlignMk2.Y1 + pView->m_pMotion->m_dPinPosY[0]; // PCB좌표
		double dRefAlignX2 = pDoc->m_Master[0].m_stAlignMk2.X2 + pView->m_pMotion->m_dPinPosX[0]; // PCB좌표
		double dRefAlignY2 = pDoc->m_Master[0].m_stAlignMk2.Y2 + pView->m_pMotion->m_dPinPosY[0]; // PCB좌표
		double dRefAlignX3 = pDoc->m_Master[0].m_stAlignMk2.X3 + pView->m_pMotion->m_dPinPosX[0]; // PCB좌표
		double dRefAlignY3 = pDoc->m_Master[0].m_stAlignMk2.Y3 + pView->m_pMotion->m_dPinPosY[0]; // PCB좌표

		// PCB상의 원점 기준의 Marking 이미지 좌표.
		double dTgtAlignX0 = (pDoc->m_Master[0].m_stAlignMk2.X0 + pView->m_pMotion->m_dPinPosX[0]) - m_dMkFdOffsetX[0][0];
		double dTgtAlignY0 = (pDoc->m_Master[0].m_stAlignMk2.Y0 + pView->m_pMotion->m_dPinPosY[0]) - m_dMkFdOffsetY[0][0];
		double dTgtAlignX1 = (pDoc->m_Master[0].m_stAlignMk2.X1 + pView->m_pMotion->m_dPinPosX[0]) - m_dMkFdOffsetX[0][1];
		double dTgtAlignY1 = (pDoc->m_Master[0].m_stAlignMk2.Y1 + pView->m_pMotion->m_dPinPosY[0]) - m_dMkFdOffsetY[0][1];
		double dTgtAlignX2 = (pDoc->m_Master[0].m_stAlignMk2.X2 + pView->m_pMotion->m_dPinPosX[0]) - m_dMkFdOffsetX[0][2];
		double dTgtAlignY2 = (pDoc->m_Master[0].m_stAlignMk2.Y2 + pView->m_pMotion->m_dPinPosY[0]) - m_dMkFdOffsetY[0][2];
		double dTgtAlignX3 = (pDoc->m_Master[0].m_stAlignMk2.X3 + pView->m_pMotion->m_dPinPosX[0]) - m_dMkFdOffsetX[0][3];
		double dTgtAlignY3 = (pDoc->m_Master[0].m_stAlignMk2.Y3 + pView->m_pMotion->m_dPinPosY[0]) - m_dMkFdOffsetY[0][3];

		int nNodeX = 0, nNodeY = 0;
		if (pDoc->m_Master[0].m_pPcsRgn)
		{
			nNodeX = pDoc->m_Master[0].m_pPcsRgn->nCol;
			nNodeY = pDoc->m_Master[0].m_pPcsRgn->nRow;
		}

		//pView->m_Align[0].SetAlignData(dRefAlignX0, dRefAlignY0, dRefAlignX1, dRefAlignY1, dRefAlignX2, dRefAlignY2, dRefAlignX3, dRefAlignY3,
		//								dTgtAlignX0, dTgtAlignY0, dTgtAlignX1, dTgtAlignY1, dTgtAlignX2, dTgtAlignY2, dTgtAlignX3, dTgtAlignY3);

		CfPoint ptRef, ptTgt;
		int nCol, nRow, idx = 0;
		for (nCol = 0; nCol < nNodeX; nCol++)
		{
			for (nRow = 0; nRow < nNodeY; nRow++)
			{
				ptRef.x = pDoc->m_Master[0].m_stPcsMk[idx].X + pView->m_pMotion->m_dPinPosX[0];
				ptRef.y = pDoc->m_Master[0].m_stPcsMk[idx].Y + pView->m_pMotion->m_dPinPosY[0];
				pView->m_Align[0].BilinearAlignment(dRefAlignX0, dRefAlignY0, dRefAlignX1, dRefAlignY1, dRefAlignX2, dRefAlignY2, dRefAlignX3, dRefAlignY3,
													dTgtAlignX0, dTgtAlignY0, dTgtAlignX1, dTgtAlignY1, dTgtAlignX2, dTgtAlignY2, dTgtAlignX3, dTgtAlignY3, 
													ptRef.x, ptRef.y, &ptTgt.x, &ptTgt.y);
				if (pDoc->m_Master[0].m_pPcsRgn)
				{
					pDoc->m_Master[0].m_pPcsRgn->pMkPnt[0][idx].x = ptTgt.x;
					pDoc->m_Master[0].m_pPcsRgn->pMkPnt[0][idx].y = ptTgt.y;
				}
				idx++;
			}
		}

	}
#endif
	return TRUE;
}


BOOL CDlgMenu02::FourPointAlign1(int nPos)
{
#ifdef USE_VISION
	if (!pView->m_pMotion || !pView->m_pVision[1])
		return FALSE;
#else
	return TRUE;
#endif
	if (pDoc->WorkingInfo.Vision[1].sResX.IsEmpty() || pDoc->WorkingInfo.Vision[1].sResY.IsEmpty())
		return FALSE;

	MoveAlign1(nPos);

	return (Do4PtAlign1(nPos, TRUE));
}

BOOL CDlgMenu02::Do4PtAlign1(int nPos, BOOL bDraw)
{
#ifdef USE_VISION
	double dRefPinX, dRefPinY; // Cam Pos.
	double dTgtPinX, dTgtPinY; // Grab Pos.
	double dResX, dResY, dResCam;
	double fLen, fVel, fAcc, fJerk;
	CString str;
	double pPos[2];
	CfPoint ptPnt;
	double dX, dY, dAgl, dScr;

	if (pView->m_pVision[1]->Grab(nPos, bDraw))
	{
		GetPmRst1(dX, dY, dAgl, dScr);
		str.Format(_T("%.1f"), dX);
		pDoc->WorkingInfo.Motion.sAlignResultPosX[1][nPos] = str;
		myStcData2[7].SetText(str);

		str.Format(_T("%.1f"), dY);
		pDoc->WorkingInfo.Motion.sAlignResultPosY[1][nPos] = str;
		myStcData2[8].SetText(str);

		str.Format(_T("%.1f"), dAgl);
		pDoc->WorkingInfo.Motion.sAlignResultTheta[1][nPos] = str;
		myStcData2[9].SetText(str);

		str.Format(_T("%.1f"), dScr);
		pDoc->WorkingInfo.Motion.sAlignResultScore[1][nPos] = str;
		myStcData2[10].SetText(str);
	}
	else
	{
		Sleep(100);

		if (pView->m_pVision[1]->Grab(nPos, bDraw))
		{
			GetPmRst1(dX, dY, dAgl, dScr);
			str.Format(_T("%.1f"), dX);
			pDoc->WorkingInfo.Motion.sAlignResultPosX[1][nPos] = str;
			myStcData2[7].SetText(str);

			str.Format(_T("%.1f"), dY);
			pDoc->WorkingInfo.Motion.sAlignResultPosY[1][nPos] = str;
			myStcData2[8].SetText(str);

			str.Format(_T("%.1f"), dAgl);
			pDoc->WorkingInfo.Motion.sAlignResultTheta[1][nPos] = str;
			myStcData2[9].SetText(str);

			str.Format(_T("%.1f"), dScr);
			pDoc->WorkingInfo.Motion.sAlignResultScore[1][nPos] = str;
			myStcData2[10].SetText(str);
		}
		else
		{
			myStcData2[7].SetText(_T(""));
			myStcData2[8].SetText(_T(""));
			myStcData2[9].SetText(_T(""));
			myStcData2[10].SetText(_T(""));
			return FALSE;
		}
	}

	int nCamSzX, nCamSzY;
	pView->m_pVision[1]->GetCameraSize(nCamSzX, nCamSzY);

	dResX = _tstof(pDoc->WorkingInfo.Vision[1].sResX);
	dResY = _tstof(pDoc->WorkingInfo.Vision[1].sResY);
	dResCam = _tstof(pDoc->WorkingInfo.Vision[1].sCamPxlRes) / 10000.0;

	GetPmRst1(dX, dY, dAgl, dScr);

	if (nPos == 0)
	{
		m_dMkFdOffsetX[1][0] = (double(nCamSzX / 2) - dX) * dResX; // -: 제품 덜옴, +: 제품 더옴.
		m_dMkFdOffsetY[1][0] = (double(nCamSzY / 2) - dY) * dResY; // -: 제품 나옴, +: 제품 들어감.
#ifdef USE_MPE
		//pView->IoWrite("ML45066", (long)(-1.0 * m_dMkFdOffsetX[1] * 1000.0));	// 마킹부 Feeding 롤러 Offset(*1000, +:더 보냄, -덜 보냄)
		pView->m_pMpe->Write(_T("ML45066"), (long)(-1.0 * m_dMkFdOffsetX[1][0] * 1000.0));
#endif
	}
	else if (nPos == 1)
	{
		m_dMkFdOffsetX[1][1] = (double(nCamSzX / 2) - dX) * dResX; // -: 제품 덜옴, +: 제품 더옴.
		m_dMkFdOffsetY[1][1] = (double(nCamSzY / 2) - dY) * dResY; // -: 제품 나옴, +: 제품 들어감.
	}
	else if (nPos == 2)
	{
		m_dMkFdOffsetX[1][2] = (double(nCamSzX / 2) - dX) * dResX; // -: 제품 덜옴, +: 제품 더옴.
		m_dMkFdOffsetY[1][2] = (double(nCamSzY / 2) - dY) * dResY; // -: 제품 나옴, +: 제품 들어감.
	}
	else if (nPos == 3)
	{
		double dMkFdOffsetX = (double(nCamSzX / 2) - dX) * dResX; // -: 제품 덜옴, +: 제품 더옴.
		double dMkFdOffsetY = (double(nCamSzY / 2) - dY) * dResY; // -: 제품 나옴, +: 제품 들어감.
		m_dMkFdOffsetX[1][3] = dMkFdOffsetX;
		m_dMkFdOffsetY[1][3] = dMkFdOffsetY;

		// Cam의 원점 기준의 Marking 이미지 좌표.
		double dRefAlignX0 = pDoc->m_Master[0].m_stAlignMk2.X0 + pView->m_pMotion->m_dPinPosX[1]; // PCB좌표
		double dRefAlignY0 = pDoc->m_Master[0].m_stAlignMk2.Y0 + pView->m_pMotion->m_dPinPosY[1]; // PCB좌표
		double dRefAlignX1 = pDoc->m_Master[0].m_stAlignMk2.X1 + pView->m_pMotion->m_dPinPosX[1]; // PCB좌표
		double dRefAlignY1 = pDoc->m_Master[0].m_stAlignMk2.Y1 + pView->m_pMotion->m_dPinPosY[1]; // PCB좌표
		double dRefAlignX2 = pDoc->m_Master[0].m_stAlignMk2.X2 + pView->m_pMotion->m_dPinPosX[1]; // PCB좌표
		double dRefAlignY2 = pDoc->m_Master[0].m_stAlignMk2.Y2 + pView->m_pMotion->m_dPinPosY[1]; // PCB좌표
		double dRefAlignX3 = pDoc->m_Master[0].m_stAlignMk2.X3 + pView->m_pMotion->m_dPinPosX[1]; // PCB좌표
		double dRefAlignY3 = pDoc->m_Master[0].m_stAlignMk2.Y3 + pView->m_pMotion->m_dPinPosY[1]; // PCB좌표

		// PCB상의 원점 기준의 Marking 이미지 좌표.
		double dTgtAlignX0 = (pDoc->m_Master[0].m_stAlignMk2.X0 + pView->m_pMotion->m_dPinPosX[1]) - m_dMkFdOffsetX[1][0];
		double dTgtAlignY0 = (pDoc->m_Master[0].m_stAlignMk2.Y0 + pView->m_pMotion->m_dPinPosY[1]) - m_dMkFdOffsetY[1][0];
		double dTgtAlignX1 = (pDoc->m_Master[0].m_stAlignMk2.X1 + pView->m_pMotion->m_dPinPosX[1]) - m_dMkFdOffsetX[1][1];
		double dTgtAlignY1 = (pDoc->m_Master[0].m_stAlignMk2.Y1 + pView->m_pMotion->m_dPinPosY[1]) - m_dMkFdOffsetY[1][1];
		double dTgtAlignX2 = (pDoc->m_Master[0].m_stAlignMk2.X0 + pView->m_pMotion->m_dPinPosX[1]) - m_dMkFdOffsetX[1][2];
		double dTgtAlignY2 = (pDoc->m_Master[0].m_stAlignMk2.Y0 + pView->m_pMotion->m_dPinPosY[1]) - m_dMkFdOffsetY[1][2];
		double dTgtAlignX3 = (pDoc->m_Master[0].m_stAlignMk2.X1 + pView->m_pMotion->m_dPinPosX[1]) - m_dMkFdOffsetX[1][3];
		double dTgtAlignY3 = (pDoc->m_Master[0].m_stAlignMk2.Y1 + pView->m_pMotion->m_dPinPosY[1]) - m_dMkFdOffsetY[1][3];

		int nNodeX = 0, nNodeY = 0;
		if (pDoc->m_Master[0].m_pPcsRgn)
		{
			nNodeX = pDoc->m_Master[0].m_pPcsRgn->nCol;
			nNodeY = pDoc->m_Master[0].m_pPcsRgn->nRow;
		}

		//pView->m_Align[1].SetAlignData(dRefAlignX0, dRefAlignY0, dRefAlignX1, dRefAlignY1, dTgtAlignX0, dTgtAlignY0, dTgtAlignX1, dTgtAlignY1);

		CfPoint ptRef, ptTgt;
		int nCol, nRow, idx = 0;
		for (nCol = 0; nCol < nNodeX; nCol++)
		{
			for (nRow = 0; nRow < nNodeY; nRow++)
			{
				ptRef.x = pDoc->m_Master[0].m_stPcsMk[idx].X + pView->m_pMotion->m_dPinPosX[1];
				ptRef.y = pDoc->m_Master[0].m_stPcsMk[idx].Y + pView->m_pMotion->m_dPinPosY[1];
				pView->m_Align[0].BilinearAlignment(dRefAlignX0, dRefAlignY0, dRefAlignX1, dRefAlignY1, dRefAlignX2, dRefAlignY2, dRefAlignX3, dRefAlignY3,
													dTgtAlignX0, dTgtAlignY0, dTgtAlignX1, dTgtAlignY1, dTgtAlignX2, dTgtAlignY2, dTgtAlignX3, dTgtAlignY3,
													ptRef.x, ptRef.y, &ptTgt.x, &ptTgt.y);
				if (pDoc->m_Master[0].m_pPcsRgn)
				{
					pDoc->m_Master[0].m_pPcsRgn->pMkPnt[1][idx].x = ptTgt.x;
					pDoc->m_Master[0].m_pPcsRgn->pMkPnt[1][idx].y = ptTgt.y;
				}
				idx++;
			}
		}

#ifdef USE_FLUCK
		// for Elec Check Point
		ptRef.x = _tstof(pDoc->WorkingInfo.Probing[1].sMeasurePosX);
		ptRef.y = _tstof(pDoc->WorkingInfo.Probing[1].sMeasurePosY);
		pView->m_Align[0].BilinearAlignment(dRefAlignX0, dRefAlignY0, dRefAlignX1, dRefAlignY1, dRefAlignX2, dRefAlignY2, dRefAlignX3, dRefAlignY3,
											dTgtAlignX0, dTgtAlignY0, dTgtAlignX1, dTgtAlignY1, dTgtAlignX2, dTgtAlignY2, dTgtAlignX3, dTgtAlignY3,
											ptRef.x, ptRef.y, &ptTgt.x, &ptTgt.y);
		pDoc->WorkingInfo.Fluck.dMeasPosX[1] = ptTgt.x;
		pDoc->WorkingInfo.Fluck.dMeasPosY[1] = ptTgt.y;
#endif
	}
#endif
	return TRUE;
}


//====================================================================================================
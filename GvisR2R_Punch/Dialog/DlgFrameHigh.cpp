// DlgFrameHigh.cpp : implementation file
//

#include "stdafx.h"
#include "../gvisr2r_punch.h"
#include "DlgFrameHigh.h"

#include "../MainFrm.h"
#include "../GvisR2R_PunchDoc.h"
#include "../GvisR2R_PunchView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMainFrame* pFrm;
extern CGvisR2R_PunchDoc* pDoc;
extern CGvisR2R_PunchView* pView;

/////////////////////////////////////////////////////////////////////////////
// CDlgFrameHigh dialog


CDlgFrameHigh::CDlgFrameHigh(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFrameHigh::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFrameHigh)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pRect = NULL;
	m_bTIM_SIG_AOI = FALSE;
	m_sStc[0] = _T("");
	m_sStc[1] = _T("");
	m_sStc[2] = _T("");
	m_nMkLastShot = 0;
	m_nAoiLastShot[0] = 0;
	m_nAoiLastShot[1] = 0;
	//m_nEngraveLastShot = 0;
}

CDlgFrameHigh::~CDlgFrameHigh()
{
	DelImg();

	if(m_pRect)
	{
		delete m_pRect;
		m_pRect = NULL;
	}
}

BOOL CDlgFrameHigh::Create()
{
	return CDialog::Create(CDlgFrameHigh::IDD);
}

void CDlgFrameHigh::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFrameHigh)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFrameHigh, CDialog)
	//{{AFX_MSG_MAP(CDlgFrameHigh)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_CHK_MENU_01, OnChkMenu01)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHK_MENU_02, OnChkMenu02)
	ON_BN_CLICKED(IDC_CHK_MENU_03, OnChkMenu03)
	ON_BN_CLICKED(IDC_CHK_MENU_04, OnChkMenu04)
	ON_BN_CLICKED(IDC_CHK_MENU_05, OnChkMenu05)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFrameHigh message handlers

void CDlgFrameHigh::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if(!m_pRect)
	{
		m_pRect = new CRect;
		
		this->GetClientRect(m_pRect);
		m_pRect->top = 22;
		m_pRect->bottom += 22;
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

void CDlgFrameHigh::AtDlgShow()
{
	LoadImg();
}

void CDlgFrameHigh::AtDlgHide()
{

}
void CDlgFrameHigh::LoadImg()
{
// 	myLabel.LoadBkImage(IMG_BK_DlgFrameHigh);

	myChkMenu01.LoadImage(ICO_CHK_DlgFrameHigh, BTN_IMG_DN, CSize(30,30));
	myChkMenu01.LoadBkImage(IMG_BTN_UP_DlgFrameHigh, BTN_IMG_UP);
	myChkMenu01.LoadBkImage(IMG_BTN_DN_DlgFrameHigh, BTN_IMG_DN);

 	myChkMenu02.LoadImage(ICO_CHK_DlgFrameHigh, BTN_IMG_DN, CSize(30,30));
	myChkMenu02.LoadBkImage(IMG_BTN_UP_DlgFrameHigh, BTN_IMG_UP);
	myChkMenu02.LoadBkImage(IMG_BTN_DN_DlgFrameHigh, BTN_IMG_DN);

 	myChkMenu03.LoadImage(ICO_CHK_DlgFrameHigh, BTN_IMG_DN, CSize(30,30));
	myChkMenu03.LoadBkImage(IMG_BTN_UP_DlgFrameHigh, BTN_IMG_UP);
	myChkMenu03.LoadBkImage(IMG_BTN_DN_DlgFrameHigh, BTN_IMG_DN);

	myChkMenu04.LoadImage(ICO_CHK_DlgFrameHigh, BTN_IMG_DN, CSize(30,30));
	myChkMenu04.LoadBkImage(IMG_BTN_UP_DlgFrameHigh, BTN_IMG_UP);
	myChkMenu04.LoadBkImage(IMG_BTN_DN_DlgFrameHigh, BTN_IMG_DN);

 	myChkMenu05.LoadImage(ICO_CHK_DlgFrameHigh, BTN_IMG_DN, CSize(30,30));
	myChkMenu05.LoadBkImage(IMG_BTN_UP_DlgFrameHigh, BTN_IMG_UP);
	myChkMenu05.LoadBkImage(IMG_BTN_DN_DlgFrameHigh, BTN_IMG_DN);

	int i;
	for(i=0; i<MAX_FRMHIGH_LABEL; i++)
	{
		switch(i)
		{
		case 0:
		case 1:
			myLabel[i].LoadImage(ICO_LED_GRY_DlgFrameHigh, LBL_IMG_UP, CSize(28,28), LBL_POS_CENTER);
			myLabel[i].LoadImage(ICO_LED_BLU_DlgFrameHigh, LBL_IMG_DN, CSize(28,28), LBL_POS_CENTER);
			break;
		case 2:
		case 3:
			myLabel[i].LoadImage(ICO_LED_GRY_DlgFrameHigh, LBL_IMG_UP, CSize(28,28), LBL_POS_CENTER);
			myLabel[i].LoadImage(ICO_LED_RED_DlgFrameHigh, LBL_IMG_DN, CSize(28,28), LBL_POS_CENTER);
			break;
		case 4:
			myLabel[i].LoadImage(ICO_LED_GRY_DlgFrameHigh, LBL_IMG_UP, CSize(28, 28), LBL_POS_CENTER);
			myLabel[i].LoadImage(ICO_LED_BLU_DlgFrameHigh, LBL_IMG_DN, CSize(28, 28), LBL_POS_CENTER);
			break;
		}
	}
}

void CDlgFrameHigh::DelImg()
{
	myChkMenu01.DelImgList();
 	myChkMenu02.DelImgList();
 	myChkMenu03.DelImgList();
	myChkMenu04.DelImgList();
 	myChkMenu05.DelImgList();

	int i;
	for(i=0; i<MAX_FRMHIGH_LABEL; i++)
		myLabel[i].DelImgList();
}

void CDlgFrameHigh::SetChk(int nID)
{
	myChkMenu01.SetCheck(FALSE);
	myChkMenu02.SetCheck(FALSE);
	myChkMenu03.SetCheck(FALSE);
	myChkMenu04.SetCheck(FALSE);
	myChkMenu05.SetCheck(FALSE);

	myChkMenu01.SetImage(BTN_IMG_UP);
	myChkMenu02.SetImage(BTN_IMG_UP);
	myChkMenu03.SetImage(BTN_IMG_UP);
	myChkMenu04.SetImage(BTN_IMG_UP);
	myChkMenu05.SetImage(BTN_IMG_UP);

	myChkMenu01.SetTextColor(RGB_BLACK);
	myChkMenu02.SetTextColor(RGB_BLACK);
	myChkMenu03.SetTextColor(RGB_BLACK);
	myChkMenu04.SetTextColor(RGB_BLACK);
	myChkMenu05.SetTextColor(RGB_BLACK);

	switch(nID)
	{
	case IDC_CHK_MENU_01:
		myChkMenu01.SetImage(BTN_IMG_DN);
		myChkMenu01.SetCheck(TRUE);
		myChkMenu01.SetTextColor(RGB_NAVY);
		break;
	case IDC_CHK_MENU_02:
		myChkMenu02.SetImage(BTN_IMG_DN);
		myChkMenu02.SetCheck(TRUE);
		myChkMenu02.SetTextColor(RGB_NAVY);
		break;
	case IDC_CHK_MENU_03:
		myChkMenu03.SetImage(BTN_IMG_DN);
		myChkMenu03.SetCheck(TRUE);
		myChkMenu03.SetTextColor(RGB_NAVY);
		break;
	case IDC_CHK_MENU_04:
		myChkMenu04.SetImage(BTN_IMG_DN);
		myChkMenu04.SetCheck(TRUE);
		myChkMenu04.SetTextColor(RGB_NAVY);
		break;
	case IDC_CHK_MENU_05:
		myChkMenu05.SetImage(BTN_IMG_DN);
		myChkMenu05.SetCheck(TRUE);
		myChkMenu05.SetTextColor(RGB_NAVY);
		break;
	}
}

BOOL CDlgFrameHigh::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitLabel();
	InitBtn();
	InitPic();
	InitStatic();

	myStc[6].SetText(pDoc->WorkingInfo.LastJob.sMkLastShot);
	m_nMkLastShot = _tstoi(pDoc->WorkingInfo.LastJob.sMkLastShot);
	
	m_bTIM_SIG_AOI = TRUE;
	SetTimer(TIM_SIG_AOI, 100, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgFrameHigh::InitStatic()
{
	myStc[0].SubclassDlgItem(IDC_STC_0_0, this);	// Feeding Offset
	myStc[1].SubclassDlgItem(IDC_STC_0_1, this);	// MK
	myStc[2].SubclassDlgItem(IDC_STC_0_2, this);	// MK-Value
	myStc[3].SubclassDlgItem(IDC_STC_0_3, this);	// DN
	myStc[4].SubclassDlgItem(IDC_STC_0_4, this);	// DN-Value

	myStc[5].SubclassDlgItem(IDC_STC_0_5, this);	// Last
	myStc[6].SubclassDlgItem(IDC_STC_0_6, this);	// Last-Value

	myStc[7].SubclassDlgItem(IDC_STC_0_7, this);	// UP
	myStc[8].SubclassDlgItem(IDC_STC_0_8, this);	// UP-Value

	myStc[9].SubclassDlgItem(IDC_STC_0_9, this);	// LS
	myStc[10].SubclassDlgItem(IDC_STC_0_10, this);	// LS-Value

	for(int i=0; i<MAX_FRMHIGH_STC; i++)
	{
		myStc[i].SetFontName(_T("Arial"));
		myStc[i].SetFontBold(TRUE);
		switch(i)
		{
		case 0:
		case 5:
			myStc[i].SetFontSize(14);
			myStc[i].SetTextColor(RGB_NAVY);
			myStc[i].SetBkColor(RGB_ORANGE);
			break;
		case 1:
		case 3:
		case 7:
		case 9:
			myStc[i].SetFontSize(14);
			myStc[i].SetTextColor(RGB_NAVY);
			myStc[i].SetBkColor(RGB_LTYELLOW);
			break;
		case 2:
		case 4:
		case 8:
		case 10:
			myStc[i].SetFontSize(14);
			myStc[i].SetTextColor(RGB_RED);
			myStc[i].SetBkColor(RGB_WHITE);
			break;
		case 6:
			myStc[i].SetFontSize(14);
			myStc[i].SetTextColor(RGB_RED);
			myStc[i].SetBkColor(RGB_WHITE);
			break;
		}
	}

}

void CDlgFrameHigh::InitPic()
{
	myPic.Load(IMG_BK_DlgFrameHigh);
}

void CDlgFrameHigh::InitBtn()
{
	myChkMenu01.SetFont(_T("굴림체"),16,TRUE);
	myChkMenu01.SetTextColor(RGB_BLACK);
	myChkMenu01.SubclassDlgItem(IDC_CHK_MENU_01, this);
	myChkMenu01.SetHwnd(this->GetSafeHwnd(), IDC_CHK_MENU_01);
	//myChkMenu01.SetBtnType(BTN_TYPE_CHECK);

	myChkMenu02.SetFont(_T("굴림체"),16,TRUE);
	myChkMenu02.SetTextColor(RGB_BLACK);
	myChkMenu02.SubclassDlgItem(IDC_CHK_MENU_02, this);
	myChkMenu02.SetHwnd(this->GetSafeHwnd(), IDC_CHK_MENU_02);
	//myChkMenu02.SetBtnType(BTN_TYPE_CHECK);

	myChkMenu03.SetFont(_T("굴림체"),16,TRUE);
	myChkMenu03.SetTextColor(RGB_BLACK);
	myChkMenu03.SubclassDlgItem(IDC_CHK_MENU_03, this);
	myChkMenu03.SetHwnd(this->GetSafeHwnd(), IDC_CHK_MENU_03);
	//myChkMenu03.SetBtnType(BTN_TYPE_CHECK);

	myChkMenu04.SetFont(_T("굴림체"),16,TRUE);
	myChkMenu04.SetTextColor(RGB_BLACK);
	myChkMenu04.SubclassDlgItem(IDC_CHK_MENU_04, this);
	myChkMenu04.SetHwnd(this->GetSafeHwnd(), IDC_CHK_MENU_04);
	//myChkMenu04.SetBtnType(BTN_TYPE_CHECK);

	myChkMenu05.SetFont(_T("굴림체"),16,TRUE);
	myChkMenu05.SetTextColor(RGB_BLACK);
	myChkMenu05.SubclassDlgItem(IDC_CHK_MENU_05, this);
	myChkMenu05.SetHwnd(this->GetSafeHwnd(), IDC_CHK_MENU_05);
	//myChkMenu05.SetBtnType(BTN_TYPE_CHECK);
}

void CDlgFrameHigh::InitLabel()
{
	myLabel[0].SubclassDlgItem(IDC_STC_SIG00, this);	// pDoc->m_pSliceIo[10] & (0x01<<1);		// Out - 검사부 검사 테이블 진공 SOL
	myLabel[1].SubclassDlgItem(IDC_STC_SIG01, this);	// pDoc->m_pSliceIo[10] & (0x01<<0);		// Out - 검사부 검사 시작
	myLabel[2].SubclassDlgItem(IDC_STC_SIG02, this);	// pDoc->m_pSliceIo[5] & (0x01<<1));		// In - 검사부 테이블 진공 완료
	myLabel[3].SubclassDlgItem(IDC_STC_SIG03, this);	// pDoc->m_pSliceIo[5] & (0x01<<0));		// In - 검사부 검사 완료
	myLabel[4].SubclassDlgItem(IDC_STC_SIG04, this);	// pDoc->m_pSliceIo[5] & (0x01<<0));		// In - 검사부 검사 완료

	for(int i=0; i<MAX_FRMHIGH_LABEL; i++)
	{
		myLabel[i].SetFontName(_T("Arial"));
		myLabel[i].SetFontSize(18);
		myLabel[i].SetFontBold(TRUE);
		myLabel[i].SetTextColor(RGB_DARKRED);
		myLabel[i].SetImageBk(LBL_IMG_UP);
	}
}

void CDlgFrameHigh::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
// 	CRect rect(0,0,0,0);
// 	GetClientRect(&rect);
// 	myPic.Show(&dc,rect);
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgFrameHigh::OnChkMenu01() 
{
	// TODO: Add your control notification handler code here
	if(myChkMenu01.GetCheck())
	{
		SetChk(IDC_CHK_MENU_01);
		return;
	}
	ChkMenu01();
}

void CDlgFrameHigh::ChkMenu01() 
{
	// TODO: Add your control notification handler code here
	SetChk(IDC_CHK_MENU_01);
	pView->ShowDlg(IDD_DLG_MENU_01);		
}

void CDlgFrameHigh::OnChkMenu02() 
{
	// TODO: Add your control notification handler code here
	if(myChkMenu02.GetCheck())
	{
		SetChk(IDC_CHK_MENU_02);
		return;
	}
	ChkMenu02();
}

void CDlgFrameHigh::ChkMenu02() 
{
	// TODO: Add your control notification handler code here
	SetChk(IDC_CHK_MENU_02);
	pView->ShowDlg(IDD_DLG_MENU_02);		
}

void CDlgFrameHigh::OnChkMenu03() 
{
	// TODO: Add your control notification handler code here
	if(myChkMenu03.GetCheck())
	{
		SetChk(IDC_CHK_MENU_03);
		return;
	}
	ChkMenu03();	
}

void CDlgFrameHigh::ChkMenu03() 
{
	// TODO: Add your control notification handler code here
	SetChk(IDC_CHK_MENU_03);
	pView->ShowDlg(IDD_DLG_MENU_03);		
}

void CDlgFrameHigh::OnChkMenu04() 
{
	// TODO: Add your control notification handler code here
	int nRtn = pView->MyPassword(_T("\r\n           비밀번호를 입력하세요..."));
	if(nRtn)
	{
		if(myChkMenu04.GetCheck())
		{
			SetChk(IDC_CHK_MENU_04);
			return;
		}
		ChkMenu04();
	}
	else
		myChkMenu04.SetCheck(FALSE);
}

void CDlgFrameHigh::ChkMenu04() 
{
	// TODO: Add your control notification handler code here
	SetChk(IDC_CHK_MENU_04);
	pView->ShowDlg(IDD_DLG_MENU_04);		
}

void CDlgFrameHigh::OnChkMenu05() 
{
	// TODO: Add your control notification handler code here
	if(myChkMenu05.GetCheck())
	{
		SetChk(IDC_CHK_MENU_05);
		return;
	}
	ChkMenu05();		
}

void CDlgFrameHigh::ChkMenu05() 
{
	// TODO: Add your control notification handler code here
	SetChk(IDC_CHK_MENU_05);
	pView->ShowDlg(IDD_DLG_MENU_05);		
}

void CDlgFrameHigh::OnTimer(UINT_PTR nIDEvent)//(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent==TIM_SIG_AOI)
	{
		KillTimer(TIM_SIG_AOI);
		DispSigAoi();
		DispFdOffset();
		ChkFdEnc();
		if(m_bTIM_SIG_AOI)
			SetTimer(TIM_SIG_AOI, 100, NULL);
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CDlgFrameHigh::ChkFdEnc()
{
// 	double dLimit = (double)MAX_ENC_CNT / 2.0;
// 	double dCurPosMkFd = (double)pDoc->m_pMpeData[0][0];	// 마킹부 Feeding 엔코더 값(단위 mm )
// 	if(dCurPosMkFd > dLimit || dCurPosMkFd < - dLimit)
// 	{
// 		if(pView->m_pMotion)
// 			pView->m_pMotion->SetOriginPos(AXIS_MKFD);
// 	}
// 
// 	if(pView->m_dEnc[AXIS_AOIFD] > dLimit || pView->m_dEnc[AXIS_AOIFD] < - dLimit)
// 	{
// 		if(pView->m_pMotion)
// 			pView->m_pMotion->SetOriginPos(AXIS_AOIFD);
// 	}
}

void CDlgFrameHigh::DispFdOffset()
{
	CString str;
	double dOfsX, dOfsY;

	if(pView->m_pDlgMenu02)
	{
		dOfsX = 1.0*pView->m_pDlgMenu02->m_dMkFdOffsetX[0][0];
		dOfsY = pView->m_pDlgMenu02->m_dMkFdOffsetY[0][0];
		str.Format(_T("%.1f\r%.1f"), dOfsY, dOfsX);
		if(str != m_sStc[0])
		{
			m_sStc[0] = str;
			myStc[2].SetText(str);
		}

		dOfsX = 1.0*pView->m_pDlgMenu02->m_dAoiDnFdOffsetX;
		dOfsY = pView->m_pDlgMenu02->m_dAoiDnFdOffsetY;
		str.Format(_T("%.1f\r%.1f"), dOfsY, dOfsX);
		if(str != m_sStc[1])
		{
			m_sStc[1] = str;
			myStc[4].SetText(str);
		}

		dOfsX = 1.0*pView->m_pDlgMenu02->m_dAoiUpFdOffsetX;
		dOfsY = pView->m_pDlgMenu02->m_dAoiUpFdOffsetY;
		str.Format(_T("%.1f\r%.1f"), dOfsY, dOfsX);
		if(str != m_sStc[2])
		{
			m_sStc[2] = str;
			myStc[8].SetText(str);
		}
	}
}

void CDlgFrameHigh::DispSigAoi()
{
	if(!pView->m_pDlgMenu03)
		return;

	BOOL bOn;
// 	bOn	= pView->m_pDlgMenu03->IsAoiTblVac();				// Out - 검사부 검사 테이블 진공 SOL
	bOn	= pView->IsAoiTblVac();								// Out - 검사부 검사 테이블 진공 SOL
	if(bOn && myLabel[0].GetImageBk() != LBL_IMG_DN)
		myLabel[0].SetImageBk(LBL_IMG_DN);
	else if(!bOn && myLabel[0].GetImageBk() != LBL_IMG_UP)
		myLabel[0].SetImageBk(LBL_IMG_UP);

// 	bOn	= pView->m_pDlgMenu03->IsAoiTest();					// Out - 검사부 검사 시작
	bOn	= pView->IsTest();									// Out - 검사부 검사 시작
	if(bOn && myLabel[1].GetImageBk() != LBL_IMG_DN)
		myLabel[1].SetImageBk(LBL_IMG_DN);
	else if(!bOn && myLabel[1].GetImageBk() != LBL_IMG_UP)
		myLabel[1].SetImageBk(LBL_IMG_UP);

	bOn	= pView->m_pDlgMenu03->IsAoiTblVacDone();			// In - 검사부 테이블 진공 완료
	if(bOn && myLabel[2].GetImageBk() != LBL_IMG_DN)
		myLabel[2].SetImageBk(LBL_IMG_DN);
	else if(!bOn && myLabel[2].GetImageBk() != LBL_IMG_UP)
		myLabel[2].SetImageBk(LBL_IMG_UP);

// 	bOn	= pView->m_pDlgMenu03->IsAoiTestDone();				// In - 검사부 검사 완료
	bOn	= pView->IsTestDone();								// In - 검사부 검사 완료
	if(bOn && myLabel[3].GetImageBk() != LBL_IMG_DN)
		myLabel[3].SetImageBk(LBL_IMG_DN);
	else if(!bOn && myLabel[3].GetImageBk() != LBL_IMG_UP)			
		myLabel[3].SetImageBk(LBL_IMG_UP);

	bOn = pView->IsConnected();								// In - 각인부 TCP/IP 연결
	if (bOn && myLabel[4].GetImageBk() != LBL_IMG_DN)
		myLabel[4].SetImageBk(LBL_IMG_DN);
	else if (!bOn && myLabel[4].GetImageBk() != LBL_IMG_UP)
		myLabel[4].SetImageBk(LBL_IMG_UP);
}

BOOL CDlgFrameHigh::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
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

void CDlgFrameHigh::SetMkLastShot(int nSerial)
{
	if(nSerial < 0)
	{
		return;
	}

	m_nMkLastShot = nSerial;

	CString str;
	str.Format(_T("%d"), nSerial);
	myStc[6].SetText(str);

	CString sPath=PATH_WORKING_INFO;
	pDoc->WorkingInfo.LastJob.sMkLastShot = str;
	::WritePrivateProfileString(_T("Last Job"), _T("Mk Last Shot"), str, sPath);
}

void CDlgFrameHigh::SetAoiLastShot(int nAoi, int nSerial)
{
	if(nSerial < 0 || nAoi < 0  || nAoi > 1)
	{
		return;
	}

	m_nAoiLastShot[nAoi] = nSerial;

	CString str;
	str.Format(_T("%d"), nSerial);
	//myStc[6].SetText(str);

	CString sPath=PATH_WORKING_INFO;
	pDoc->WorkingInfo.LastJob.sAoiLastShot[nAoi] = str;
	if(nAoi==0)
		::WritePrivateProfileString(_T("Last Job"), _T("AoiUp Last Shot"), str, sPath);
	else if(nAoi==1)
		::WritePrivateProfileString(_T("Last Job"), _T("AoiDn Last Shot"), str, sPath);
}

void CDlgFrameHigh::SetDualTest(BOOL bOn)
{
	if(bOn)
	{
		if(!myStc[3].IsWindowEnabled())
			myStc[3].EnableWindow(TRUE);
		if(!myStc[4].IsWindowEnabled())
			myStc[4].EnableWindow(TRUE);
	}
	else
	{
		if(myStc[3].IsWindowEnabled())
			myStc[3].EnableWindow(FALSE);
		if(myStc[4].IsWindowEnabled())
			myStc[4].EnableWindow(FALSE);
	}
}

void CDlgFrameHigh::SetEngraveLastShot(int nSerial)
{
	if (nSerial < 0)
	{
		return;
	}

	//m_nEngraveLastShot = nSerial;

	CString str;
	str.Format(_T("%d"), nSerial);
	myStc[10].SetText(str);			// myStc[10].SubclassDlgItem(IDC_STC_0_10, this);	// LS-Value

	CString sPath = PATH_WORKING_INFO;
	pDoc->WorkingInfo.LastJob.sEngraveLastShot = str;
	::WritePrivateProfileString(_T("Last Job"), _T("Engrave Last Shot"), str, sPath);
}
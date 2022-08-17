// DlgInfo.cpp : implementation file
//

#include "stdafx.h"
#include "../gvisr2r_punch.h"
#include "DlgInfo.h"
#include "DlgKeyNum.h"
#include "DlgUser.h"

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
// CDlgInfo dialog


CDlgInfo::CDlgInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgInfo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bLoadImg = FALSE;
}

CDlgInfo::~CDlgInfo()
{
	if (pView->m_pDlgMenu01)
		pView->m_pDlgMenu01->UpdateData();
}

void CDlgInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgInfo)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgInfo, CDialog)
	//{{AFX_MSG_MAP(CDlgInfo)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_STC_0008, OnStc0008)
	ON_BN_CLICKED(IDC_CHK_000, OnChk000)
	ON_BN_CLICKED(IDC_CHK_001, OnChk001)
	ON_BN_CLICKED(IDC_CHK_002, OnChk002)
	ON_BN_CLICKED(IDC_CHK_003, OnChk003)
	ON_BN_CLICKED(IDC_CHK_004, OnChk004)
	ON_BN_CLICKED(IDC_CHK_005, OnChk005)
	ON_BN_CLICKED(IDC_CHK_006, OnChk006)
	ON_BN_CLICKED(IDC_CHK_007, OnChk007)
	ON_BN_CLICKED(IDC_CHK_008, OnChk008)
	ON_BN_CLICKED(IDC_CHK_009, OnChk009)
	ON_BN_CLICKED(IDC_CHK_010, OnChk010)
	ON_BN_CLICKED(IDC_CHK_011, OnChk011)
	ON_BN_CLICKED(IDC_STC_0012, OnStc0012)
	ON_BN_CLICKED(IDC_STC_0016, OnStc0016)
	ON_BN_CLICKED(IDC_STC_0020, OnStc0020)
	ON_BN_CLICKED(IDC_STC_0024, OnStc0024)
	ON_BN_CLICKED(IDC_STC_0030, OnStc0030)
	ON_BN_CLICKED(IDC_STC_0034, OnStc0034)
	ON_BN_CLICKED(IDC_STC_0037, OnStc0037)
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
	ON_BN_CLICKED(IDC_STC_174, OnStc174)
	ON_BN_CLICKED(IDC_STC_178, OnStc178)
	ON_BN_CLICKED(IDC_STC_32, OnStc32)
	ON_BN_CLICKED(IDC_STC_183, OnStc183)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_STC_61, OnStc61)
	ON_BN_CLICKED(IDC_CHK_USE_AOI_DUAL, OnChkUseAoiDual)
	ON_BN_CLICKED(IDC_CHK_USE_AOI_SINGLE, OnChkUseAoiSingle)
	ON_BN_CLICKED(IDC_CHK_SAMPLE_TEST, OnChkSampleTest)
	ON_BN_CLICKED(IDC_CHK_ONE_METAL, OnChkOneMetal)
	ON_BN_CLICKED(IDC_CHK_TWO_METAL, OnChkTwoMetal)
	ON_BN_CLICKED(IDC_CHK_USE_AOI_INNER, OnChkUseAoiInner)
	ON_BN_CLICKED(IDC_CHK_USE_AOI_OUTER, OnChkUseAoiOuter)
	ON_BN_CLICKED(IDC_STC_181, OnStc181)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHK_4_POINT_ALIGN, &CDlgInfo::OnBnClickedChk4PointAlign)
	ON_BN_CLICKED(IDC_CHK_2_POINT_ALIGN, &CDlgInfo::OnBnClickedChk2PointAlign)
	ON_BN_CLICKED(IDC_CHK_86, &CDlgInfo::OnBnClickedChk86)
	ON_BN_CLICKED(IDC_CHK_85, &CDlgInfo::OnBnClickedChk85)
	ON_BN_CLICKED(IDC_CHK_1187, &CDlgInfo::OnBnClickedChk1187)
	ON_BN_CLICKED(IDC_CHK_1188, &CDlgInfo::OnBnClickedChk1188)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgInfo message handlers

void CDlgInfo::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if(bShow)
	{
		AtDlgShow();
	}
	else
	{
		AtDlgHide();
	}	
}

void CDlgInfo::AtDlgShow()
{
	LoadImg();
	Disp();
}

void CDlgInfo::AtDlgHide()
{
	DelImg();
}

void CDlgInfo::LoadImg()
{
	if(m_bLoadImg)
		return;
	m_bLoadImg = TRUE;

	int i;
	for(i=0; i<MAX_INFO_BTN; i++)
	{
		myBtn[i].LoadBkImage(IMG_BTN_UP_DlgInfo, BTN_IMG_UP);
		myBtn[i].LoadBkImage(IMG_BTN_DN_DlgInfo, BTN_IMG_DN);
	}

	myBtnExit.LoadBkImage(MSG_BTN_BK_UP, BTN_IMG_UP);
	myBtnExit.LoadBkImage(MSG_BTN_BK_DN, BTN_IMG_DN);
 	myBtnExit.LoadImage(ICO_BTN_EXIT, BTN_IMG_UP, CSize(40,40), BTN_POS_LEFT);
 	myBtnExit.LoadImage(ICO_BTN_EXIT, BTN_IMG_DN, CSize(40,40), BTN_POS_LEFT);
}

void CDlgInfo::DelImg()
{
	if(!m_bLoadImg)
		return;
	m_bLoadImg = FALSE;

	int i;
	for(i=0; i<MAX_INFO_BTN; i++)
		myBtn[i].DelImgList();

	myBtnExit.DelImgList();
}

BOOL CDlgInfo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitStatic();
	InitBtn();

// 	GetDlgItem(IDC_STC_0014)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_CHK_001)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_STC_0015)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_STC_0016)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_STC_0017)->ShowWindow(SW_HIDE);
// 
// 	GetDlgItem(IDC_STC_0018)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_STC_0019)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_STC_0020)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_STC_0021)->ShowWindow(SW_HIDE);
// 
 	GetDlgItem(IDC_STC_0043)->ShowWindow(SW_HIDE);
 	GetDlgItem(IDC_CHK_008)->ShowWindow(SW_HIDE);
 	GetDlgItem(IDC_STC_0044)->ShowWindow(SW_HIDE);
 	GetDlgItem(IDC_CHK_009)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_STC_0046)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_CHK_011)->ShowWindow(SW_HIDE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgInfo::InitBtn()
{
	myBtn[0].SubclassDlgItem(IDC_CHK_000, this);
	myBtn[0].SetHwnd(this->GetSafeHwnd(), IDC_CHK_000);
	myBtn[0].SetBtnType(BTN_TYPE_CHECK);

	myBtn[1].SubclassDlgItem(IDC_CHK_001, this);
	myBtn[1].SetHwnd(this->GetSafeHwnd(), IDC_CHK_001);
	myBtn[1].SetBtnType(BTN_TYPE_CHECK);

	myBtn[2].SubclassDlgItem(IDC_CHK_002, this);
	myBtn[2].SetHwnd(this->GetSafeHwnd(), IDC_CHK_002);	
	myBtn[2].SetBtnType(BTN_TYPE_CHECK);

	myBtn[3].SubclassDlgItem(IDC_CHK_003, this);
	myBtn[3].SetHwnd(this->GetSafeHwnd(), IDC_CHK_003);
	myBtn[3].SetBtnType(BTN_TYPE_CHECK);

	myBtn[4].SubclassDlgItem(IDC_CHK_004, this);
	myBtn[4].SetHwnd(this->GetSafeHwnd(), IDC_CHK_004);
	myBtn[4].SetBtnType(BTN_TYPE_CHECK);

	myBtn[5].SubclassDlgItem(IDC_CHK_005, this);
	myBtn[5].SetHwnd(this->GetSafeHwnd(), IDC_CHK_005);
	myBtn[5].SetBtnType(BTN_TYPE_CHECK);

	myBtn[6].SubclassDlgItem(IDC_CHK_006, this);
	myBtn[6].SetHwnd(this->GetSafeHwnd(), IDC_CHK_006);
	myBtn[6].SetBtnType(BTN_TYPE_CHECK);

	myBtn[7].SubclassDlgItem(IDC_CHK_007, this);
	myBtn[7].SetHwnd(this->GetSafeHwnd(), IDC_CHK_007);	
	myBtn[7].SetBtnType(BTN_TYPE_CHECK);

	myBtn[8].SubclassDlgItem(IDC_CHK_008, this);
	myBtn[8].SetHwnd(this->GetSafeHwnd(), IDC_CHK_008);
	myBtn[8].SetBtnType(BTN_TYPE_CHECK);

	myBtn[9].SubclassDlgItem(IDC_CHK_009, this);
	myBtn[9].SetHwnd(this->GetSafeHwnd(), IDC_CHK_009);
	myBtn[9].SetBtnType(BTN_TYPE_CHECK);

	myBtn[10].SubclassDlgItem(IDC_CHK_010, this);
	myBtn[10].SetHwnd(this->GetSafeHwnd(), IDC_CHK_010);
	myBtn[10].SetBtnType(BTN_TYPE_CHECK);

	myBtn[11].SubclassDlgItem(IDC_CHK_011, this);
	myBtn[11].SetHwnd(this->GetSafeHwnd(), IDC_CHK_011);
	myBtn[11].SetBtnType(BTN_TYPE_CHECK);

	myBtn[12].SubclassDlgItem(IDC_CHK_USE_AOI_DUAL, this);
	myBtn[12].SetHwnd(this->GetSafeHwnd(), IDC_CHK_USE_AOI_DUAL);
	myBtn[12].SetBtnType(BTN_TYPE_CHECK);

	myBtn[13].SubclassDlgItem(IDC_CHK_USE_AOI_SINGLE, this);
	myBtn[13].SetHwnd(this->GetSafeHwnd(), IDC_CHK_USE_AOI_SINGLE);
	myBtn[13].SetBtnType(BTN_TYPE_CHECK);

	myBtn[14].SubclassDlgItem(IDC_CHK_SAMPLE_TEST, this);
	myBtn[14].SetHwnd(this->GetSafeHwnd(), IDC_CHK_SAMPLE_TEST);
	myBtn[14].SetBtnType(BTN_TYPE_CHECK);

	myBtn[15].SubclassDlgItem(IDC_CHK_ONE_METAL, this);
	myBtn[15].SetHwnd(this->GetSafeHwnd(), IDC_CHK_ONE_METAL);
	myBtn[15].SetBtnType(BTN_TYPE_CHECK);

	myBtn[16].SubclassDlgItem(IDC_CHK_TWO_METAL, this);
	myBtn[16].SetHwnd(this->GetSafeHwnd(), IDC_CHK_TWO_METAL);
	myBtn[16].SetBtnType(BTN_TYPE_CHECK);

	myBtn[17].SubclassDlgItem(IDC_CHK_2_POINT_ALIGN, this);
	myBtn[17].SetHwnd(this->GetSafeHwnd(), IDC_CHK_2_POINT_ALIGN);
	myBtn[17].SetBtnType(BTN_TYPE_CHECK);

	myBtn[18].SubclassDlgItem(IDC_CHK_4_POINT_ALIGN, this);
	myBtn[18].SetHwnd(this->GetSafeHwnd(), IDC_CHK_4_POINT_ALIGN);
	myBtn[18].SetBtnType(BTN_TYPE_CHECK);

	myBtn[19].SubclassDlgItem(IDC_CHK_1185, this); //하면AOI 클린롤러 
	myBtn[19].SetHwnd(this->GetSafeHwnd(), IDC_CHK_1185);
	myBtn[19].SetBtnType(BTN_TYPE_CHECK);

	myBtn[20].SubclassDlgItem(IDC_CHK_1186, this); //상면AOI 클린롤러 
	myBtn[20].SetHwnd(this->GetSafeHwnd(), IDC_CHK_1186);
	myBtn[20].SetBtnType(BTN_TYPE_CHECK);

	myBtn[21].SubclassDlgItem(IDC_CHK_1187, this); //AOI초음파세정기
	myBtn[21].SetHwnd(this->GetSafeHwnd(), IDC_CHK_1187);
	myBtn[21].SetBtnType(BTN_TYPE_CHECK);

	myBtn[22].SubclassDlgItem(IDC_CHK_1188, this); //각인부초음파세정기
	myBtn[22].SetHwnd(this->GetSafeHwnd(), IDC_CHK_1188);
	myBtn[22].SetBtnType(BTN_TYPE_CHECK);

	myBtn[23].SubclassDlgItem(IDC_CHK_USE_AOI_INNER, this); //WORK_MODE : MODE_INNER
	myBtn[23].SetHwnd(this->GetSafeHwnd(), IDC_CHK_USE_AOI_INNER);
	myBtn[23].SetBtnType(BTN_TYPE_CHECK);

	myBtn[24].SubclassDlgItem(IDC_CHK_USE_AOI_OUTER, this); //WORK_MODE : MODE_OUTER
	myBtn[24].SetHwnd(this->GetSafeHwnd(), IDC_CHK_USE_AOI_OUTER);
	myBtn[24].SetBtnType(BTN_TYPE_CHECK);


	int i;
	for(i=0; i<MAX_INFO_BTN; i++)
	{
		if(0 == i || 12 == i || 13 == i || 14 == i || 23 == i || 24 == i )
		{
			myBtn[i].SetFont(_T("굴림체"),16,TRUE);
			myBtn[i].SetTextColor(RGB_BLACK);
			//myBtn[i].SetBtnType(BTN_TYPE_CHECK);
		}

		if (15 == i || 16 == i)
		{
			myBtn[i].SetFont(_T("돋움체"),22,TRUE);
			//myBtn[i].SetBtnType(BTN_TYPE_CHECK);
		}

		if (0 != i && 12 != i && 13 != i && 14 != i && 15 != i && 16 != i && 23 != i && 24 != i)
		{
			myBtn[i].SetFont(_T("굴림체"),16,TRUE);
			myBtn[i].SetTextColor(RGB_BLACK);
			myBtn[i].SetText(_T("사용\rOff"), BTN_UP);
			myBtn[i].SetText(_T("사용\rON"), BTN_DN);
		}
	}

	myBtnExit.SubclassDlgItem(IDC_BTN_EXIT, this);
	myBtnExit.SetHwnd(this->GetSafeHwnd(), IDC_BTN_EXIT);
	myBtnExit.SetFont(_T("굴림체"),24,TRUE);
	myBtnExit.SetTextColor(RGB_BLACK);
}

void CDlgInfo::InitStatic()
{
	InitStcTitle();
	InitStcData();
}

void CDlgInfo::InitStcTitle()
{
	myStcTitle[0].SubclassDlgItem(IDC_STC_0000, this);
	myStcTitle[1].SubclassDlgItem(IDC_STC_0001, this);
	myStcTitle[2].SubclassDlgItem(IDC_STC_0002, this);
	myStcTitle[3].SubclassDlgItem(IDC_STC_0004, this);
	myStcTitle[4].SubclassDlgItem(IDC_STC_0005, this);
	myStcTitle[5].SubclassDlgItem(IDC_STC_0006, this);
	myStcTitle[6].SubclassDlgItem(IDC_STC_0007, this);
	myStcTitle[7].SubclassDlgItem(IDC_STC_0009, this);
	myStcTitle[8].SubclassDlgItem(IDC_STC_0010, this);
	myStcTitle[9].SubclassDlgItem(IDC_STC_0011, this);
	myStcTitle[10].SubclassDlgItem(IDC_STC_0013, this);
	myStcTitle[11].SubclassDlgItem(IDC_STC_0014, this);
	myStcTitle[12].SubclassDlgItem(IDC_STC_0015, this);
	myStcTitle[13].SubclassDlgItem(IDC_STC_0017, this);
	myStcTitle[14].SubclassDlgItem(IDC_STC_0018, this);
	myStcTitle[15].SubclassDlgItem(IDC_STC_0019, this);
	myStcTitle[16].SubclassDlgItem(IDC_STC_0021, this);
	myStcTitle[17].SubclassDlgItem(IDC_STC_0022, this);
	myStcTitle[18].SubclassDlgItem(IDC_STC_0023, this);
	myStcTitle[19].SubclassDlgItem(IDC_STC_0025, this);
	myStcTitle[20].SubclassDlgItem(IDC_STC_0026, this);
	myStcTitle[21].SubclassDlgItem(IDC_STC_0027, this);
	myStcTitle[22].SubclassDlgItem(IDC_STC_0028, this);
	myStcTitle[23].SubclassDlgItem(IDC_STC_0029, this);
	myStcTitle[24].SubclassDlgItem(IDC_STC_0031, this);
	myStcTitle[25].SubclassDlgItem(IDC_STC_0032, this);
	myStcTitle[26].SubclassDlgItem(IDC_STC_0033, this);
	myStcTitle[27].SubclassDlgItem(IDC_STC_0035, this);
	myStcTitle[28].SubclassDlgItem(IDC_STC_0036, this);
	myStcTitle[29].SubclassDlgItem(IDC_STC_0038, this);
	myStcTitle[30].SubclassDlgItem(IDC_STC_0039, this);
	myStcTitle[31].SubclassDlgItem(IDC_STC_0040, this);
	myStcTitle[32].SubclassDlgItem(IDC_STC_0041, this);
	myStcTitle[33].SubclassDlgItem(IDC_STC_0042, this);
	myStcTitle[34].SubclassDlgItem(IDC_STC_0043, this);
	myStcTitle[35].SubclassDlgItem(IDC_STC_0044, this);
	myStcTitle[36].SubclassDlgItem(IDC_STC_0045, this);
	myStcTitle[37].SubclassDlgItem(IDC_STC_0046, this);
	myStcTitle[38].SubclassDlgItem(IDC_STC_59, this);
	myStcTitle[39].SubclassDlgItem(IDC_STC_173, this);
	myStcTitle[40].SubclassDlgItem(IDC_STC_175, this);
	myStcTitle[41].SubclassDlgItem(IDC_STC_176, this);
	myStcTitle[42].SubclassDlgItem(IDC_STC_177, this);
	myStcTitle[43].SubclassDlgItem(IDC_STC_38, this);
	myStcTitle[44].SubclassDlgItem(IDC_STC_62, this);
	myStcTitle[45].SubclassDlgItem(IDC_STC_60, this);
	myStcTitle[46].SubclassDlgItem(IDC_STC_63, this);
	myStcTitle[47].SubclassDlgItem(IDC_STC_64, this);
	myStcTitle[48].SubclassDlgItem(IDC_STC_65, this);
	myStcTitle[49].SubclassDlgItem(IDC_STC_1145, this); //하면AOI 클린롤러
	myStcTitle[50].SubclassDlgItem(IDC_STC_1146, this); //상면AOI 클린롤러
	myStcTitle[51].SubclassDlgItem(IDC_STC_1147, this); //AOI초음파세정기
	myStcTitle[52].SubclassDlgItem(IDC_STC_1148, this); //각인부초음파세정기

	myStcTitle[53].SubclassDlgItem(IDC_STC_30, this); //초음파세정기 동작
	myStcTitle[54].SubclassDlgItem(IDC_STC_31, this); //검사시작 후
	myStcTitle[55].SubclassDlgItem(IDC_STC_33, this); //초

	myStcTitle[56].SubclassDlgItem(IDC_STC_73, this); //고객출하수율
	myStcTitle[57].SubclassDlgItem(IDC_STC_182, this); //양품율
	myStcTitle[58].SubclassDlgItem(IDC_STC_184, this); //%

	for(int i=0; i<MAX_INFO_STC; i++)
	{
		myStcTitle[i].SetFontName(_T("Arial"));
// 		if(47 == i)
// 			myStcTitle[i].SetFontSize(12);
// 		else
			myStcTitle[i].SetFontSize(14);
		
		switch(i)
		{
		case 0:
		case 3:
		case 20:
			myStcTitle[i].SetTextColor(RGB_NAVY);
			myStcTitle[i].SetBkColor(RGB_LTDKORANGE);
			myStcTitle[i].SetFontBold(TRUE);
			break;
		case 1:
		case 4:
		case 21:
		case 47:
			myStcTitle[i].SetTextColor(RGB_WHITE);
			myStcTitle[i].SetBkColor(RGB_DARKGREEN);
			myStcTitle[i].SetFontBold(TRUE);
			break;
		case 10:
		case 43:
			myStcTitle[i].SetTextColor(RGB_NAVY);
			myStcTitle[i].SetBkColor(RGB_WHITE);
			myStcTitle[i].SetFontBold(FALSE);
			break;
		case 7:
		case 13:
		case 16:
		case 19:
		case 24:
		case 27:
		case 40:
		case 55:
		case 58:
			myStcTitle[i].SetTextColor(RGB_NAVY);
			myStcTitle[i].SetBkColor(RGB_WHITE);
			myStcTitle[i].SetFontBold(TRUE);
			break;
		default:
			myStcTitle[i].SetTextColor(RGB_NAVY);
			myStcTitle[i].SetBkColor(RGB_LTYELLOW);
			myStcTitle[i].SetFontBold(TRUE);
			break;
		}
	}
}

void CDlgInfo::InitStcData()
{
	myStcData[0].SubclassDlgItem(IDC_STC_0003, this);
	myStcData[1].SubclassDlgItem(IDC_STC_0008, this);
	myStcData[2].SubclassDlgItem(IDC_STC_0012, this);
	myStcData[3].SubclassDlgItem(IDC_STC_0016, this);
	myStcData[4].SubclassDlgItem(IDC_STC_0020, this);
	myStcData[5].SubclassDlgItem(IDC_STC_0024, this);
	myStcData[6].SubclassDlgItem(IDC_STC_0030, this);
	myStcData[7].SubclassDlgItem(IDC_STC_0034, this);
	myStcData[8].SubclassDlgItem(IDC_STC_0037, this);
	myStcData[9].SubclassDlgItem(IDC_STC_174, this);
	myStcData[10].SubclassDlgItem(IDC_STC_178, this);
	myStcData[11].SubclassDlgItem(IDC_STC_61, this);
	myStcData[12].SubclassDlgItem(IDC_STC_181, this);

	myStcData[13].SubclassDlgItem(IDC_STC_32, this); // 초음파세정기 동작 검사시작 후 시작시간 [초]
	myStcData[14].SubclassDlgItem(IDC_STC_183, this); // 고객출하수율

	for(int i=0; i<MAX_INFO_STC_DATA; i++)
	{
		myStcData[i].SetFontName(_T("Arial"));
// 		if(12==i)
// 			myStcData[i].SetFontSize(12);
// 		else
			myStcData[i].SetFontSize(20);
		myStcData[i].SetFontBold(TRUE);
		myStcData[i].SetTextColor(RGB_BLACK);
		myStcData[i].SetBkColor(RGB_WHITE);
	}
}

BOOL CDlgInfo::ShowKeypad(int nCtlID, CPoint ptSt, int nDir)
{
	BOOL bAdj = TRUE;
	CString strData, strPrev;
	GetDlgItemText(nCtlID, strData);
	strPrev = strData;

	CString strMin, strMax;
	CRect rect(0,0,0,0);
	CDlgKeyNum *pDlg = new CDlgKeyNum(&strData, &rect, ptSt, nDir);
	pDlg->m_strCurr = strData;
	pDlg->m_strMin = strMin = pDoc->GetMin(IDD_DLG_INFO, nCtlID);
	pDlg->m_strMax = strMax = pDoc->GetMax(IDD_DLG_INFO, nCtlID);
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
	
// 	ScreenToClient(&rect);
// 	InvalidateRect(rect, FALSE);
	return bAdj;
}

void CDlgInfo::Disp()
{
	CString str;

	myStcData[0].SetText(pDoc->WorkingInfo.LastJob.sSelUserName);
	myStcData[1].SetText(pDoc->WorkingInfo.LastJob.sReelTotLen);
	myStcData[2].SetText(pDoc->WorkingInfo.LastJob.sOnePnlLen);	// pDoc->WorkingInfo.Motion.sMkFdDist
	myStcData[3].SetText(pDoc->WorkingInfo.LastJob.sLotSepLen);
	myStcData[4].SetText(pDoc->WorkingInfo.LastJob.sLotCutPosLen);
	myStcData[5].SetText(pDoc->WorkingInfo.LastJob.sTempPauseLen);
	myStcData[6].SetText(pDoc->WorkingInfo.LastJob.sLmtTotYld);
	myStcData[7].SetText(pDoc->WorkingInfo.LastJob.sLmtPatlYld);
	myStcData[8].SetText(pDoc->WorkingInfo.LastJob.sNumContFixDef);
	myStcData[9].SetText(pDoc->WorkingInfo.LastJob.sStripOutRatio);
	myStcData[10].SetText(pDoc->WorkingInfo.LastJob.sPartialSpd);
 	myStcData[11].SetText(pDoc->WorkingInfo.LastJob.sNumRangeFixDef);
 	myStcData[12].SetText(pDoc->WorkingInfo.LastJob.sSampleTestShotNum);
	myStcData[13].SetText(pDoc->WorkingInfo.LastJob.sUltraSonicCleannerStTim);
	myStcData[14].SetText(pDoc->WorkingInfo.LastJob.sCustomNeedRatio);

	if(pDoc->WorkingInfo.LastJob.bLotSep)
		myBtn[1].SetCheck(TRUE);
	else
		myBtn[1].SetCheck(FALSE);

	if(pDoc->WorkingInfo.LastJob.bTempPause)
		myBtn[2].SetCheck(TRUE);
	else
		myBtn[2].SetCheck(FALSE);

	if(pDoc->WorkingInfo.LastJob.bContFixDef)
		myBtn[3].SetCheck(TRUE);
	else
		myBtn[3].SetCheck(FALSE);

	if(pDoc->WorkingInfo.LastJob.bRclDrSen)
		myBtn[4].SetCheck(TRUE);
	else
		myBtn[4].SetCheck(FALSE);

	if(pDoc->WorkingInfo.LastJob.bAoiUpDrSen)
		myBtn[5].SetCheck(TRUE);
	else
		myBtn[5].SetCheck(FALSE);

	if(pDoc->WorkingInfo.LastJob.bMkDrSen)
		myBtn[6].SetCheck(TRUE);
	else
		myBtn[6].SetCheck(FALSE);

	if(pDoc->WorkingInfo.LastJob.bUclDrSen)
		myBtn[7].SetCheck(TRUE);
	else
		myBtn[7].SetCheck(FALSE);

	if(pDoc->WorkingInfo.LastJob.bDispMkPcs)
		myBtn[8].SetCheck(TRUE);
	else
		myBtn[8].SetCheck(FALSE);

	if(pDoc->WorkingInfo.LastJob.bStopFixDef)
		myBtn[9].SetCheck(TRUE);
	else
		myBtn[9].SetCheck(FALSE);

	if(pDoc->WorkingInfo.LastJob.bMkSftySen)
		myBtn[10].SetCheck(TRUE);
	else
		myBtn[10].SetCheck(FALSE);

	//if(pDoc->WorkingInfo.LastJob.bAoiSftySen)
	if (pDoc->WorkingInfo.LastJob.bAoiDnDrSen)
		myBtn[11].SetCheck(TRUE);
	else
		myBtn[11].SetCheck(FALSE);

	if(pDoc->WorkingInfo.LastJob.bDualTest)
	{
		myBtn[12].SetCheck(TRUE);
		myBtn[13].SetCheck(FALSE);
	}
	else
	{
		myBtn[12].SetCheck(FALSE);
		myBtn[13].SetCheck(TRUE);
	}

	if(pDoc->WorkingInfo.LastJob.bSampleTest)
		myBtn[14].SetCheck(TRUE);
	else
		myBtn[14].SetCheck(FALSE);

	if(pDoc->WorkingInfo.LastJob.bOneMetal)
	{
		myBtn[15].SetCheck(TRUE);
		myBtn[15].SetWindowText(_T("Recoiler\r역방향"));	
		myBtn[15].SetTextColor(RGB_DARKRED);
	}
	else
	{
		myBtn[15].SetCheck(FALSE);
		myBtn[15].SetWindowText(_T("Recoiler\r정방향"));
		myBtn[15].SetTextColor(RGB_BLUE);
	}

	if(pDoc->WorkingInfo.LastJob.bTwoMetal)
	{
		myBtn[16].SetCheck(TRUE);
		myBtn[16].SetWindowText(_T("Uncoiler\r역방향"));
		myBtn[16].SetTextColor(RGB_DARKRED);
	}
	else
	{
		myBtn[16].SetCheck(FALSE);
		myBtn[16].SetWindowText(_T("Uncoiler\r정방향"));
		myBtn[16].SetTextColor(RGB_BLUE);
	}

	if (pDoc->WorkingInfo.LastJob.bUseAoiDnCleanRoler)
		myBtn[19].SetCheck(TRUE);
	else
		myBtn[19].SetCheck(FALSE);

	if (pDoc->WorkingInfo.LastJob.bUseAoiUpCleanRoler)
		myBtn[20].SetCheck(TRUE);
	else
		myBtn[20].SetCheck(FALSE);

	if (pDoc->WorkingInfo.LastJob.bUseAoiDnCleanner) //AOI초음파세정기
		myBtn[21].SetCheck(TRUE);
	else
		myBtn[21].SetCheck(FALSE);

	if (pDoc->WorkingInfo.LastJob.bUseEngraveCleanner) //각인부초음파세정기
		myBtn[22].SetCheck(TRUE);
	else
		myBtn[22].SetCheck(FALSE);
}

void CDlgInfo::OnStc0008() 
{
	// TODO: Add your control notification handler code here
	myStcData[1].SetBkColor(RGB_RED);
	myStcData[1].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_0008)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_0008, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[1].SetBkColor(RGB_WHITE);
	myStcData[1].RedrawWindow();
	
	CString sVal;
	GetDlgItem(IDC_STC_0008)->GetWindowText(sVal);

 	pDoc->SetTotalReelDist(_tstoi(sVal));		
// 
// 	pDoc->WorkingInfo.LastJob.sReelTotLen = sVal;
// 	if(pDoc->m_pReelMap)
// 		pDoc->m_pReelMap->m_dTotLen = _tstof(sVal) * 1000.0;
// 	::WritePrivateProfileString(_T("Last Job"), _T("Reel Total Length", sVal, PATH_WORKING_INFO);
}

void CDlgInfo::OnStc0012() 
{
	// TODO: Add your control notification handler code here
	myStcData[2].SetBkColor(RGB_RED);
	myStcData[2].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_0012)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_0012, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[2].SetBkColor(RGB_WHITE);
	myStcData[2].RedrawWindow();
	
	CString sVal;
	GetDlgItem(IDC_STC_0012)->GetWindowText(sVal);
	pDoc->SetOnePnlLen(_tstof(sVal));

	pView->SetLotLastShot();
}

void CDlgInfo::OnStc0016() 
{
	// TODO: Add your control notification handler code here
	myStcData[3].SetBkColor(RGB_RED);
	myStcData[3].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_0016)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_0016, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[3].SetBkColor(RGB_WHITE);
	myStcData[3].RedrawWindow();
	
	CString sVal;
	GetDlgItem(IDC_STC_0016)->GetWindowText(sVal);
// 	pDoc->WorkingInfo.LastJob.sLotSepLen = sVal;
// 	if(pDoc->m_pReelMap)
// 		pDoc->m_pReelMap->m_dLotLen = _tstof(sVal);
// 	::WritePrivateProfileString(_T("Last Job"), _T("Lot Seperate Length", sVal, PATH_WORKING_INFO);

	pDoc->SetSeparateDist(_tstof(sVal));
	//pView->IoWrite(_T("ML45002", long(_tstof(sVal)*1000.0));	// Lot 분리 길이 (단위 M * 1000)
	//pView->m_pMpe->Write(_T("ML45002", long(_tstof(sVal)*1000.0));

	pView->SetLotLastShot();
}

void CDlgInfo::OnStc0020() 
{
	// TODO: Add your control notification handler code here
	myStcData[4].SetBkColor(RGB_RED);
	myStcData[4].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_0020)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_0020, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[4].SetBkColor(RGB_WHITE);
	myStcData[4].RedrawWindow();
	
	CString sVal;
	GetDlgItem(IDC_STC_0020)->GetWindowText(sVal);
	pDoc->SetCuttingDist(_tstof(sVal));
}

void CDlgInfo::OnStc0024() 
{
	// TODO: Add your control notification handler code here
	myStcData[5].SetBkColor(RGB_RED);
	myStcData[5].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_0024)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_0024, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[5].SetBkColor(RGB_WHITE);
	myStcData[5].RedrawWindow();
	
	CString sVal;
	GetDlgItem(IDC_STC_0024)->GetWindowText(sVal);
 	pDoc->SetStopDist(_tstof(sVal));	
// 
// 	pDoc->WorkingInfo.LastJob.sTempPauseLen = sVal;
// 	if(pDoc->m_pReelMap)
// 		pDoc->m_pReelMap->m_dTempPauseLen = _tstof(sVal);
// 	::WritePrivateProfileString(_T("Last Job"), _T("Temporary Pause Length", sVal, PATH_WORKING_INFO);	
// 
// 	//pView->IoWrite(_T("ML45006", long(_tstof(sVal)*1000.0));	// 일시정지 길이 (단위 M * 1000)
// 	pView->m_pMpe->Write(_T("ML45006", long(_tstof(sVal)*1000.0));
}

void CDlgInfo::OnStc0030() 
{
	// TODO: Add your control notification handler code here
	myStcData[6].SetBkColor(RGB_RED);
	myStcData[6].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_0030)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_0030, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[6].SetBkColor(RGB_WHITE);
	myStcData[6].RedrawWindow();
	
	CString sVal;
	GetDlgItem(IDC_STC_0030)->GetWindowText(sVal);
	pDoc->WorkingInfo.LastJob.sLmtTotYld = sVal;
	::WritePrivateProfileString(_T("Last Job"), _T("Limit Total Yield"), sVal, PATH_WORKING_INFO);	
}

void CDlgInfo::OnStc0034() 
{
	// TODO: Add your control notification handler code here
	myStcData[7].SetBkColor(RGB_RED);
	myStcData[7].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_0034)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_0034, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[7].SetBkColor(RGB_WHITE);
	myStcData[7].RedrawWindow();
	
	CString sVal;
	GetDlgItem(IDC_STC_0034)->GetWindowText(sVal);
	pDoc->WorkingInfo.LastJob.sLmtPatlYld = sVal;
	::WritePrivateProfileString(_T("Last Job"), _T("Limit Partial Yield"), sVal, PATH_WORKING_INFO);	
}

void CDlgInfo::OnStc0037() 
{
	// TODO: Add your control notification handler code here
	myStcData[8].SetBkColor(RGB_RED);
	myStcData[8].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_0037)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_0037, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[8].SetBkColor(RGB_WHITE);
	myStcData[8].RedrawWindow();
	
	CString sVal;
	GetDlgItem(IDC_STC_0037)->GetWindowText(sVal);
	pDoc->WorkingInfo.LastJob.sNumContFixDef = sVal;
	::WritePrivateProfileString(_T("Last Job"), _T("Number of Continuous Fix Defect"), sVal, PATH_WORKING_INFO);	
}

void CDlgInfo::OnStc32()
{
	// TODO: Add your control notification handler code here
	myStcData[13].SetBkColor(RGB_RED);
	myStcData[13].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_32)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_32, pt, TO_BOTTOM | TO_RIGHT);

	myStcData[13].SetBkColor(RGB_WHITE);
	myStcData[13].RedrawWindow();

	CString sVal;
	GetDlgItem(IDC_STC_32)->GetWindowText(sVal);
	pDoc->WorkingInfo.LastJob.sUltraSonicCleannerStTim = sVal;
	::WritePrivateProfileString(_T("Last Job"), _T("Ultra Sonic Cleanner Start Time"), sVal, PATH_WORKING_INFO);

	double dTime = _tstof(sVal) * 100.0;
	int nTime = int(dTime);
#ifdef USE_MPE
	if (pView->m_pMpe)
	{
		pView->m_pMpe->Write(_T("MW05940"), (long)nTime);	// AOI_Dn (단위 [초] * 100) : 1 is 10 mSec.
		pView->m_pMpe->Write(_T("MW05942"), (long)nTime);	// AOI_Up (단위 [초] * 100) : 1 is 10 mSec.
	}
#endif
}

void CDlgInfo::OnChk000() 
{
	// TODO: Add your control notification handler code here
	pView->ClrDispMsg();
	CDlgUser Dlg;
	Dlg.DoModal();

	myStcData[0].SetText(pDoc->WorkingInfo.LastJob.sSelUserName);
	myBtn[0].SetCheck(FALSE);

	if(pDoc->m_pReelMap)
		pDoc->m_pReelMap->m_sUser = pDoc->WorkingInfo.LastJob.sSelUserName;
	::WritePrivateProfileString(_T("Last Job"), _T("Operator Name"), pDoc->WorkingInfo.LastJob.sSelUserName, PATH_WORKING_INFO);
}

void CDlgInfo::OnChk001() 
{
	// TODO: Add your control notification handler code here
	BOOL bUse;
	if(myBtn[1].GetCheck())
	{
		bUse = TRUE;
#ifdef USE_MPE
		//pView->IoWrite(_T("MB440184", 1);	// 로트분리사용(PC가 On시키고, PC가 확인하고 Off시킴)-20141031
		pView->m_pMpe->Write(_T("MB440184"), 1);
#endif
		pView->SetLotLastShot();
	}
	else
	{
		bUse = FALSE;
#ifdef USE_MPE
		//pView->IoWrite(_T("MB440184", 0);	// 로트분리사용(PC가 On시키고, PC가 확인하고 Off시킴)-20141031
		pView->m_pMpe->Write(_T("MB440184"), 0);
#endif
	}

	pDoc->WorkingInfo.LastJob.bLotSep = bUse;
	if(pDoc->m_pReelMap)
		pDoc->m_pReelMap->m_bUseLotSep = bUse;

	CString sData = bUse ? _T("1") : _T("0");
	::WritePrivateProfileString(_T("Last Job"), _T("Use Lot seperate"), sData, PATH_WORKING_INFO);
}

void CDlgInfo::OnChk002() 
{
	// TODO: Add your control notification handler code here
	BOOL bUse = !pDoc->WorkingInfo.LastJob.bTempPause;
	Sleep(100);
	if (bUse)
	{
#ifdef USE_MPE
		pView->m_pMpe->Write(_T("MB440183"), 1);
#endif
		pView->ChkTempStop(TRUE);
		if (!myBtn[2].GetCheck())
		{
			myBtn[2].SetCheck(TRUE);
			//bUse = TRUE;
			//pView->IoWrite(_T("MB440183", 1);	// 일시정지사용(PC가 On시키고, PLC가 확인하고 Off시킴)-20141031
		}
	}
	else
	{
#ifdef USE_MPE
		pView->m_pMpe->Write(_T("MB440183"), 0);
#endif
		pView->ChkTempStop(FALSE);

		if (myBtn[2].GetCheck())
		{
			myBtn[2].SetCheck(FALSE);
			//bUse = FALSE;
			//pView->IoWrite(_T("MB440183", 0);	// 일시정지사용(PC가 On시키고, PLC가 확인하고 Off시킴)-20141031
		}
	}

	pDoc->WorkingInfo.LastJob.bTempPause = bUse;
	if(pDoc->m_pReelMap)
		pDoc->m_pReelMap->m_bUseTempPause = bUse;

	CString sData = bUse ? _T("1") : _T("0");
	::WritePrivateProfileString(_T("Last Job"), _T("Use Temporary Pause"), sData, PATH_WORKING_INFO);
}

void CDlgInfo::OnChk003() 
{
	// TODO: Add your control notification handler code here
	if(myBtn[3].GetCheck())
		pDoc->WorkingInfo.LastJob.bContFixDef = TRUE;
	else
		pDoc->WorkingInfo.LastJob.bContFixDef = FALSE;

	CString sData = pDoc->WorkingInfo.LastJob.bContFixDef ? _T("1") : _T("0");
	::WritePrivateProfileString(_T("Last Job"), _T("Use Continuous Fix Defect"), sData, PATH_WORKING_INFO);	
}

void CDlgInfo::OnChk004() 
{
	// TODO: Add your control notification handler code here
	if(myBtn[4].GetCheck())
	{
		pDoc->WorkingInfo.LastJob.bRclDrSen = TRUE;
		pDoc->Status.bDoorReF[DOOR_FR_RC] = FALSE;
		pDoc->Status.bDoorReF[DOOR_S_RC] = FALSE;
		pDoc->Status.bDoorReF[DOOR_BL_RC] = FALSE;
		pDoc->Status.bDoorReF[DOOR_BR_RC] = FALSE;
	}
	else
		pDoc->WorkingInfo.LastJob.bRclDrSen = FALSE;

	CString sData = pDoc->WorkingInfo.LastJob.bRclDrSen ? _T("1") : _T("0");
	::WritePrivateProfileString(_T("Last Job"), _T("Use Recoiler Door Sensor"), sData, PATH_WORKING_INFO);	
}

void CDlgInfo::OnChk005() 
{
	// TODO: Add your control notification handler code here
	if(myBtn[5].GetCheck())		// 상면 AOI DOOR SENSOR
	{
		pDoc->WorkingInfo.LastJob.bAoiUpDrSen = TRUE;
		pDoc->Status.bDoorAoiF[DOOR_FM_AOI_UP] = FALSE;
		pDoc->Status.bDoorAoiF[DOOR_FL_AOI_UP] = FALSE;
		pDoc->Status.bDoorAoiF[DOOR_FR_AOI_UP] = FALSE;
		pDoc->Status.bDoorAoiF[DOOR_BM_AOI_UP] = FALSE;
		pDoc->Status.bDoorAoiF[DOOR_BL_AOI_UP] = FALSE;
		pDoc->Status.bDoorAoiF[DOOR_BR_AOI_UP] = FALSE;
	}
	else
		pDoc->WorkingInfo.LastJob.bAoiUpDrSen = FALSE;

	CString sData = pDoc->WorkingInfo.LastJob.bAoiUpDrSen ? _T("1") : _T("0");
	::WritePrivateProfileString(_T("Last Job"), _T("Use AoiUp Door Sensor"), sData, PATH_WORKING_INFO);	
}

void CDlgInfo::OnChk006() 
{
	// TODO: Add your control notification handler code here
	if(myBtn[6].GetCheck())
	{
		pDoc->WorkingInfo.LastJob.bMkDrSen = TRUE;
		pDoc->Status.bDoorMkF[DOOR_FL_MK] = FALSE;
		pDoc->Status.bDoorMkF[DOOR_FR_MK] = FALSE;
		pDoc->Status.bDoorMkF[DOOR_BL_MK] = FALSE;
		pDoc->Status.bDoorMkF[DOOR_BR_MK] = FALSE;
	}
	else
		pDoc->WorkingInfo.LastJob.bMkDrSen = FALSE;

	CString sData = pDoc->WorkingInfo.LastJob.bMkDrSen ? _T("1") : _T("0");
	::WritePrivateProfileString(_T("Last Job"), _T("Use Marking Door Sensor"), sData, PATH_WORKING_INFO);	
}

void CDlgInfo::OnChk007() 
{
	// TODO: Add your control notification handler code here
	if(myBtn[7].GetCheck())
	{
		pDoc->WorkingInfo.LastJob.bUclDrSen = TRUE;
		pDoc->Status.bDoorUcF[DOOR_FL_UC] = FALSE;
		pDoc->Status.bDoorUcF[DOOR_FR_UC] = FALSE;
		pDoc->Status.bDoorUcF[DOOR_BL_UC] = FALSE;
		pDoc->Status.bDoorUcF[DOOR_BR_UC] = FALSE;
	}
	else
		pDoc->WorkingInfo.LastJob.bUclDrSen = FALSE;

	CString sData = pDoc->WorkingInfo.LastJob.bUclDrSen ? _T("1") : _T("0");
	::WritePrivateProfileString(_T("Last Job"), _T("Use Uncoiler Door Sensor"), sData, PATH_WORKING_INFO);	
}

// void CDlgInfo::OnChk008() 
// {
// 	// TODO: Add your control notification handler code here
// 	if(myBtn[8].GetCheck())
// 		pDoc->WorkingInfo.LastJob.bDispMkPcs = TRUE;
// 	else
// 		pDoc->WorkingInfo.LastJob.bDispMkPcs = FALSE;
// 
// 	CString sData = pDoc->WorkingInfo.LastJob.bDispMkPcs ? "1" : "0";
// 	::WritePrivateProfileString(_T("Last Job"), _T("Use Display Marked Piece", sData, PATH_WORKING_INFO);	
// }
void CDlgInfo::OnChk008() 
{
	//if(myBtn[8].GetCheck())
	//	pDoc->WorkingInfo.LastJob.bBufDrSen = TRUE;
	//else
	//	pDoc->WorkingInfo.LastJob.bBufDrSen = FALSE;

	//CString sData = pDoc->WorkingInfo.LastJob.bBufDrSen ? _T("1") : _T("0");
	//::WritePrivateProfileString(_T("Last Job"), _T("Use Buffer Door Sensor"), sData, PATH_WORKING_INFO);	

}

void CDlgInfo::OnChk009() 
{
	// TODO: Add your control notification handler code here
	if (myBtn[9].GetCheck())
	{
		pDoc->WorkingInfo.LastJob.bEngvDrSen = TRUE;
		pDoc->Status.bDoorEngvF[DOOR_FL_ENGV] = FALSE;
		pDoc->Status.bDoorEngvF[DOOR_FR_ENGV] = FALSE;
		pDoc->Status.bDoorEngvF[DOOR_BL_ENGV] = FALSE;
		pDoc->Status.bDoorEngvF[DOOR_BR_ENGV] = FALSE;
	}
	else
		pDoc->WorkingInfo.LastJob.bEngvDrSen = FALSE;

	CString sData = pDoc->WorkingInfo.LastJob.bEngvDrSen ? _T("1") : _T("0");
	::WritePrivateProfileString(_T("Last Job"), _T("Use Engrave Door Sensor"), sData, PATH_WORKING_INFO);

	//if(myBtn[9].GetCheck())
	//	pDoc->WorkingInfo.LastJob.bStopFixDef = TRUE;
	//else
	//	pDoc->WorkingInfo.LastJob.bStopFixDef = FALSE;

	//CString sData = pDoc->WorkingInfo.LastJob.bStopFixDef ? _T("1") : _T("0");
	//::WritePrivateProfileString(_T("Last Job"), _T("Use Fix Defect Stop"), sData, PATH_WORKING_INFO);	
}

void CDlgInfo::OnChk010() 
{
	// TODO: Add your control notification handler code here
	if(myBtn[10].GetCheck())
		pDoc->WorkingInfo.LastJob.bMkSftySen = TRUE;
	else
		pDoc->WorkingInfo.LastJob.bMkSftySen = FALSE;

	CString sData = pDoc->WorkingInfo.LastJob.bMkSftySen ? _T("1") : _T("0");
	::WritePrivateProfileString(_T("Last Job"), _T("Use Marking Safty Sensor"), sData, PATH_WORKING_INFO);	
}

void CDlgInfo::OnChk011() 
{
	// TODO: Add your control notification handler code here
	if (myBtn[11].GetCheck())		// 하면 AOI DOOR SENSOR
	{
		pDoc->WorkingInfo.LastJob.bAoiDnDrSen = TRUE;
		pDoc->Status.bDoorAoiF[DOOR_FM_AOI_DN] = FALSE;
		pDoc->Status.bDoorAoiF[DOOR_FL_AOI_DN] = FALSE;
		pDoc->Status.bDoorAoiF[DOOR_FR_AOI_DN] = FALSE;
		pDoc->Status.bDoorAoiF[DOOR_BM_AOI_DN] = FALSE;
		pDoc->Status.bDoorAoiF[DOOR_BL_AOI_DN] = FALSE;
		pDoc->Status.bDoorAoiF[DOOR_BR_AOI_DN] = FALSE;
	}
	else
		pDoc->WorkingInfo.LastJob.bAoiDnDrSen = FALSE;

	CString sData = pDoc->WorkingInfo.LastJob.bAoiDnDrSen ? _T("1") : _T("0");
	::WritePrivateProfileString(_T("Last Job"), _T("Use AoiDn Door Sensor"), sData, PATH_WORKING_INFO);


	//if(myBtn[11].GetCheck())
	//	pDoc->WorkingInfo.LastJob.bAoiSftySen = TRUE;
	//else
	//	pDoc->WorkingInfo.LastJob.bAoiSftySen = FALSE;

	//CString sData = pDoc->WorkingInfo.LastJob.bAoiSftySen ? _T("1") : _T("0");
	//::WritePrivateProfileString(_T("Last Job"), _T("Use Aoi Safty Sensor"), sData, PATH_WORKING_INFO);	
}

void CDlgInfo::OnBtnExit() 
{
	// TODO: Add your control notification handler code here
	if (pView->m_pDlgMenu01)
		pView->m_pDlgMenu01->UpdateData();

	OnOK();
}

void CDlgInfo::OnStc174() 
{
	// TODO: Add your control notification handler code here
	myStcData[9].SetBkColor(RGB_RED);
	myStcData[9].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_174)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_174, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[9].SetBkColor(RGB_WHITE);
	myStcData[9].RedrawWindow();
	
	CString sVal;
	GetDlgItem(IDC_STC_174)->GetWindowText(sVal);
	pDoc->WorkingInfo.LastJob.sStripOutRatio = sVal;
	::WritePrivateProfileString(_T("Last Job"), _T("Strip Out Ratio"), sVal, PATH_WORKING_INFO);	
}

void CDlgInfo::OnStc183() 
{
	// TODO: Add your control notification handler code here
	myStcData[14].SetBkColor(RGB_RED);
	myStcData[14].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_183)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_183, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[14].SetBkColor(RGB_WHITE);
	myStcData[14].RedrawWindow();
	
	CString sVal;
	GetDlgItem(IDC_STC_183)->GetWindowText(sVal);
	pDoc->WorkingInfo.LastJob.sCustomNeedRatio = sVal;
	::WritePrivateProfileString(_T("Last Job"), _T("Custom Need Ratio"), sVal, PATH_WORKING_INFO);	
}
void CDlgInfo::OnStc178() 
{
	// TODO: Add your control notification handler code here
	myStcData[10].SetBkColor(RGB_RED);
	myStcData[10].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_178)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_178, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[10].SetBkColor(RGB_WHITE);
	myStcData[10].RedrawWindow();
	
	CString sVal;
	GetDlgItem(IDC_STC_178)->GetWindowText(sVal);
	pDoc->WorkingInfo.LastJob.sPartialSpd = sVal;
	::WritePrivateProfileString(_T("Last Job"), _T("Partial Speed"), sVal, PATH_WORKING_INFO);	
}

HBRUSH CDlgInfo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if (nCtlColor == CTLCOLOR_DLG) {
   		pDC->SetBkMode(TRANSPARENT);
     	hbr = CreateSolidBrush(RGB(0,0,0));
  	} 	
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CDlgInfo::OnStc61() 
{
	// TODO: Add your control notification handler code here
	myStcData[11].SetBkColor(RGB_RED);
	myStcData[11].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_61)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_61, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[11].SetBkColor(RGB_WHITE);
	myStcData[11].RedrawWindow();
	
	CString sVal;
	GetDlgItem(IDC_STC_61)->GetWindowText(sVal);
	pDoc->WorkingInfo.LastJob.sNumRangeFixDef = sVal;
	::WritePrivateProfileString(_T("Last Job"), _T("Shot Num of Range in Fix Defect"), sVal, PATH_WORKING_INFO);	
	
}


void CDlgInfo::SetTestMode(int nMode)
{
	pDoc->WorkingInfo.LastJob.nTestMode = nMode; // MODE_NONE = 0, MODE_INNER = 1, MODE_OUTER = 2 .

	CString sData;
	sData.Format(_T("%d"), nMode);
	::WritePrivateProfileString(_T("Last Job"), _T("Test Mode"), sData, PATH_WORKING_INFO);

	switch (nMode)
	{
	case MODE_NONE:
		break;
	case MODE_INNER:
		break;
	case MODE_OUTER:
		break;
	default:
		break;
	}

	myBtn[23].RedrawWindow();
	myBtn[24].RedrawWindow();
}

void CDlgInfo::SetDualTest(BOOL bOn)
{
	pDoc->WorkingInfo.LastJob.bDualTest = bOn;

	CString sData = bOn ? _T("1") : _T("0");
	::WritePrivateProfileString(_T("Last Job"), _T("Use Dual AOI"), sData, PATH_WORKING_INFO);

	if(bOn)
	{
		sData = _T("6");
		myBtn[12].SetCheck(TRUE); // 양면 검사
		myBtn[13].SetCheck(FALSE); // 단면 검사
	}
	else
	{
		sData = _T("12");
		myBtn[12].SetCheck(FALSE); // 양면 검사
		myBtn[13].SetCheck(TRUE); // 단면 검사
	}
	
	myBtn[12].RedrawWindow();
	myBtn[13].RedrawWindow();

	pDoc->WorkingInfo.System.sMaxDispDefImg = sData;
	::WritePrivateProfileString(_T("System"), _T("MAX_DISP_DEF_IMAGE"), sData, PATH_WORKING_INFO);
	pView->SetDualTest(bOn);
}

void CDlgInfo::SetTwoMetal(BOOL bOn)
{
	BOOL bChk[2];
	bChk[0] = myBtn[15].GetCheck(); // IDC_CHK_ONE_METAL - Recoiler\r정방향 CW : FALSE
	bChk[1] = myBtn[16].GetCheck(); // IDC_CHK_TWO_METAL - Uncoiler\r정방향 CW : FALSE

	if(bOn)
	{
		if(bChk[1])
		{
			myBtn[16].SetTextColor(RGB_DARKRED);
			myBtn[16].SetWindowText(_T("Uncoiler\r역방향"));
		}
		else
		{
			myBtn[16].SetTextColor(RGB_BLUE);
			myBtn[16].SetWindowText(_T("Uncoiler\r정방향"));
		}
		pView->SetTwoMetal(bOn, bChk[1]);

// 		if(bChk[0])
// 			myBtn[15].SetCheck(FALSE);
// 		if(!bChk[1])
// 			myBtn[16].SetCheck(TRUE);

	}
	else
	{
		if(bChk[0])
		{
			myBtn[15].SetTextColor(RGB_DARKRED);
			myBtn[15].SetWindowText(_T("Recoiler\r역방향"));
		}
		else
		{
			myBtn[15].SetTextColor(RGB_BLUE);
			myBtn[15].SetWindowText(_T("Recoiler\r정방향"));
		}
		pView->SetTwoMetal(bOn, bChk[0]);

// 		if(!bChk[0])
// 			myBtn[15].SetCheck(TRUE);
// 		if(bChk[1])
// 			myBtn[16].SetCheck(FALSE);
	}

}

void CDlgInfo::OnChkUseAoiInner() 
{
	// TODO: Add your control notification handler code here
	BOOL bOn[2];
	bOn[0] = myBtn[23].GetCheck();
	bOn[1] = myBtn[24].GetCheck();

	if (bOn[0] && bOn[1])
	{
		myBtn[24].SetCheck(FALSE);
		SetTestMode(MODE_INNER);
	}
	else if (bOn[0] && !bOn[1])
		SetTestMode(MODE_INNER);
	else if (!bOn[0] && bOn[1])
		SetTestMode(MODE_OUTER);
	else
		SetTestMode(MODE_NONE);
}

void CDlgInfo::OnChkUseAoiOuter() 
{
	// TODO: Add your control notification handler code here
	BOOL bOn[2];
	bOn[0] = myBtn[23].GetCheck();
	bOn[1] = myBtn[24].GetCheck();

	if (bOn[0] && bOn[1])
	{
		myBtn[23].SetCheck(FALSE);
		SetTestMode(MODE_OUTER);
	}
	else if (bOn[0] && !bOn[1])
		SetTestMode(MODE_INNER);
	else if (!bOn[0] && bOn[1])
		SetTestMode(MODE_OUTER);
	else
		SetTestMode(MODE_NONE);
}

void CDlgInfo::OnChkUseAoiDual() 
{
	// TODO: Add your control notification handler code here
	SetDualTest(TRUE);
}

void CDlgInfo::OnChkUseAoiSingle() 
{
	// TODO: Add your control notification handler code here
	SetDualTest(FALSE);	
}

void CDlgInfo::OnChkSampleTest() 
{
	// TODO: Add your control notification handler code here
	BOOL bOn = myBtn[14].GetCheck();
	pDoc->WorkingInfo.LastJob.bSampleTest = bOn;

	CString sData = bOn ? _T("1") : _T("0");
	::WritePrivateProfileString(_T("Last Job"), _T("Sample Test On"), sData, PATH_WORKING_INFO);

#ifdef USE_MPE
	pView->m_pMpe->Write(_T("MB44017B"), (pDoc->WorkingInfo.LastJob.bSampleTest)?1:0);		// Sample 검사 On
#endif
}

void CDlgInfo::OnChkOneMetal() 
{
	// TODO: Add your control notification handler code here
	SetTwoMetal(FALSE);
}

void CDlgInfo::OnChkTwoMetal() 
{
	// TODO: Add your control notification handler code here
	SetTwoMetal(TRUE);
}

void CDlgInfo::OnStc181() 
{
	// TODO: Add your control notification handler code here
	myStcData[12].SetBkColor(RGB_RED);
	myStcData[12].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_181)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_181, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[12].SetBkColor(RGB_WHITE);
	myStcData[12].RedrawWindow();
	
	CString sVal;
	GetDlgItem(IDC_STC_181)->GetWindowText(sVal);
	pDoc->WorkingInfo.LastJob.sSampleTestShotNum = sVal;
	::WritePrivateProfileString(_T("Last Job"), _T("Sample Test Shot Num"), sVal, PATH_WORKING_INFO);
	
	long lData = (long)_tstoi(pDoc->WorkingInfo.LastJob.sSampleTestShotNum);//atoi
#ifdef USE_MPE
	pView->m_pMpe->Write(_T("ML45126"), lData);	// 샘플검사 Shot수
#endif
}


void CDlgInfo::OnBnClickedChk2PointAlign()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	myBtn[18].SetCheck(FALSE);

	if (myBtn[17].GetCheck())
	{
		CString sVal;
		pDoc->WorkingInfo.LastJob.nAlignMethode = TWO_POINT;
		sVal.Format(_T("%d"), pDoc->WorkingInfo.LastJob.nAlignMethode);
		::WritePrivateProfileString(_T("Last Job"), _T("Align Methode"), sVal, PATH_WORKING_INFO);
	}
	else
	{
		myBtn[17].SetCheck(TRUE);
	}
}


void CDlgInfo::OnBnClickedChk4PointAlign()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	myBtn[17].SetCheck(FALSE);

	if (myBtn[18].GetCheck())
	{
		CString sVal;
		pDoc->WorkingInfo.LastJob.nAlignMethode = FOUR_POINT;
		sVal.Format(_T("%d"), pDoc->WorkingInfo.LastJob.nAlignMethode);
		::WritePrivateProfileString(_T("Last Job"), _T("Align Methode"), sVal, PATH_WORKING_INFO);
	}
	else
	{
		myBtn[18].SetCheck(TRUE);
	}
}

void CDlgInfo::OnBnClickedChk85()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (myBtn[19].GetCheck()) //하면AOI 클린롤러
	{
		pDoc->WorkingInfo.LastJob.bUseAoiDnCleanRoler = TRUE;
#ifdef USE_MPE
		pView->m_pMpe->Write(_T("MB44010F"), 1);
#endif
	}
	else
	{
		pDoc->WorkingInfo.LastJob.bUseAoiDnCleanRoler = FALSE;
#ifdef USE_MPE
		pView->m_pMpe->Write(_T("MB44010F"), 0);
#endif
	}

	CString sData = pDoc->WorkingInfo.LastJob.bUseAoiDnCleanRoler ? _T("1") : _T("0");
	::WritePrivateProfileString(_T("Last Job"), _T("Use Dn Aoi CleanRoler"), sData, PATH_WORKING_INFO);
}

void CDlgInfo::OnBnClickedChk86()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (myBtn[20].GetCheck()) //상면AOI 클린롤러
	{
		pDoc->WorkingInfo.LastJob.bUseAoiUpCleanRoler = TRUE;
#ifdef USE_MPE
		pView->m_pMpe->Write(_T("MB44010E"), 1);
#endif
	}
	else
	{
		pDoc->WorkingInfo.LastJob.bUseAoiUpCleanRoler = FALSE;
#ifdef USE_MPE
		pView->m_pMpe->Write(_T("MB44010E"), 0);
#endif
	}

	CString sData = pDoc->WorkingInfo.LastJob.bUseAoiUpCleanRoler ? _T("1") : _T("0");
	::WritePrivateProfileString(_T("Last Job"), _T("Use Up Aoi CleanRoler"), sData, PATH_WORKING_INFO);
}


void CDlgInfo::OnBnClickedChk1187()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (myBtn[21].GetCheck()) //AOI초음파세정기
	{
		pDoc->WorkingInfo.LastJob.bUseAoiDnCleanner = TRUE;
#ifdef USE_MPE
		pView->m_pMpe->Write(_T("MB44016F"), 1);
#endif
	}
	else
	{
		pDoc->WorkingInfo.LastJob.bUseAoiDnCleanner = FALSE;
#ifdef USE_MPE
		pView->m_pMpe->Write(_T("MB44016F"), 0);
#endif
	}

	CString sData = pDoc->WorkingInfo.LastJob.bUseAoiDnCleanner ? _T("1") : _T("0");
	::WritePrivateProfileString(_T("Last Job"), _T("Use AoiDn Cleanner"), sData, PATH_WORKING_INFO);
}


void CDlgInfo::OnBnClickedChk1188()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (myBtn[22].GetCheck()) //각인부초음파세정기
	{
		pDoc->WorkingInfo.LastJob.bUseEngraveCleanner = TRUE;
#ifdef USE_MPE
		pView->m_pMpe->Write(_T("MB44016E"), 1);
#endif
	}
	else
	{
		pDoc->WorkingInfo.LastJob.bUseEngraveCleanner = FALSE;
#ifdef USE_MPE
		pView->m_pMpe->Write(_T("MB44016E"), 0);
#endif
	}

	CString sData = pDoc->WorkingInfo.LastJob.bUseEngraveCleanner ? _T("1") : _T("0");
	::WritePrivateProfileString(_T("Last Job"), _T("Use Engrave Cleanner"), sData, PATH_WORKING_INFO);
}

void CDlgInfo::UpdateData()
{
	Disp();
}

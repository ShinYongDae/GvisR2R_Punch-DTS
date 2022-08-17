// DlgUtil04.cpp : implementation file
//

#include "stdafx.h"
#include "../gvisr2r_punch.h"
#include "DlgUtil04.h"
#include "DlgUtil05.h"

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
// CDlgUtil04 dialog


CDlgUtil04::CDlgUtil04(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUtil04::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgUtil04)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pRect = NULL;
	m_bLoadImg = FALSE;
	m_nComboIndex = 0;
	m_nSel = 0;
	m_bTIM_DISP_STS = FALSE;
}

CDlgUtil04::~CDlgUtil04()
{
	m_bTIM_DISP_STS = FALSE;

	if(m_pRect)
	{
		delete m_pRect;
		m_pRect = NULL;
	}
}

BOOL CDlgUtil04::Create()
{
	return CDialog::Create(CDlgMenu04::IDD);
}

void CDlgUtil04::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgUtil04)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgUtil04, CDialog)
	//{{AFX_MSG_MAP(CDlgUtil04)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_UTIL04_0, OnBtnUtil040)
	ON_BN_CLICKED(IDC_BTN_UTIL04_1, OnBtnUtil041)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_BN_CLICKED(IDC_STC_UTIL04_56, OnStcUtil0456)
	ON_BN_CLICKED(IDC_CHK1, OnChk1)
	ON_BN_CLICKED(IDC_CHK2, OnChk2)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_STC_UTIL04_59, OnStcUtil0459)
	ON_BN_CLICKED(IDC_STC_UTIL04_62, OnStcUtil0462)
	ON_BN_CLICKED(IDC_STC_UTIL04_65, OnStcUtil0465)
	ON_BN_CLICKED(IDC_STC_UTIL04_68, OnStcUtil0468)
	ON_BN_CLICKED(IDC_STC_UTIL04_71, OnStcUtil0471)
	ON_BN_CLICKED(IDC_STC_UTIL04_74, OnStcUtil0474)
	ON_BN_CLICKED(IDC_STC_UTIL04_77, OnStcUtil0477)
	ON_BN_CLICKED(IDC_STC_UTIL04_80, OnStcUtil0480)
	ON_BN_CLICKED(IDC_STC_UTIL04_83, OnStcUtil0483)
	ON_BN_CLICKED(IDC_STC_UTIL04_86, OnStcUtil0486)
	ON_BN_CLICKED(IDC_STC_UTIL04_89, OnStcUtil0489)
	ON_BN_CLICKED(IDC_STC_UTIL04_92, OnStcUtil0492)
	ON_BN_CLICKED(IDC_STC_UTIL04_95, OnStcUtil0495)
	ON_BN_CLICKED(IDC_STC_UTIL04_98, OnStcUtil0498)
	ON_BN_CLICKED(IDC_STC_UTIL04_101, OnStcUtil04101)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgUtil04 message handlers

void CDlgUtil04::OnShowWindow(BOOL bShow, UINT nStatus) 
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

void CDlgUtil04::AtDlgShow()
{
	LoadImg();

	CComboBox* pCombo1 = ((CComboBox*)GetDlgItem(IDC_COMBO1));
	pCombo1->SetCurSel(m_nComboIndex);

	Disp(m_nComboIndex);

	m_bTIM_DISP_STS = TRUE;
	SetTimer(TIM_DISP_STS, 100, NULL);
}

void CDlgUtil04::AtDlgHide()
{
	m_bTIM_DISP_STS = FALSE;
	DelImg();
}

void CDlgUtil04::LoadImg()
{
	if(m_bLoadImg)
		return;
	m_bLoadImg = TRUE;

	int i;
	for(i=0; i<MAX_UTIL04_BTN; i++)
	{
		myBtn[i].LoadBkImage(IMG_BTN_UP_DlgUtil04, BTN_IMG_UP);
		myBtn[i].LoadBkImage(IMG_BTN_DN_DlgUtil04, BTN_IMG_DN);
	}

	for(i=0; i<MAX_UTIL04_LABEL/2; i++)
	{
		myLabel[i].LoadImage(ICO_LED_GRY_DlgUtil04, LBL_IMG_UP, CSize(35,35), LBL_POS_CENTER);
		myLabel[i].LoadImage(ICO_LED_BLU_DlgUtil04, LBL_IMG_DN, CSize(35,35), LBL_POS_CENTER);
		myLabel[i+MAX_UTIL04_LABEL/2].LoadImage(ICO_LED_GRY_DlgUtil04, LBL_IMG_UP, CSize(35,35), LBL_POS_CENTER);
		myLabel[i+MAX_UTIL04_LABEL/2].LoadImage(ICO_LED_RED_DlgUtil04, LBL_IMG_DN, CSize(35,35), LBL_POS_CENTER);
	}
}

void CDlgUtil04::DelImg()
{
	if(!m_bLoadImg)
		return;
	m_bLoadImg = FALSE;

	int i;

	for(i=0; i<MAX_UTIL04_BTN; i++)
		myBtn[i].DelImgList();

	for(i=0; i<MAX_UTIL04_LABEL/2; i++)
	{
		myLabel[i].DelImgList();
		myLabel[i+MAX_UTIL04_LABEL/2].DelImgList();
	}
}

BOOL CDlgUtil04::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitStatic();
	InitLabel();
	InitBtn();
	InitCombo();

	SetBtn(m_nSel);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgUtil04::InitCombo()
{
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 28; // 28 point
	lf.lfWeight = FW_EXTRABOLD;
	//strcpy(lf.lfFaceName, _T("굴림")); // 굴림체로 font setting
	_stprintf(lf.lfFaceName, _T("%s"), _T("굴림")); // 굴림체로 font setting

	CFont FontOfCombo;
	FontOfCombo.CreateFontIndirect(&lf);
	CComboBox* pCombo1 = ((CComboBox*)GetDlgItem(IDC_COMBO1));
	pCombo1->SetFont((CFont*)&FontOfCombo, TRUE);

	SetCombo(m_nSel);
}

void CDlgUtil04::SetCombo(int nSel)
{
	CComboBox* pCombo1 = ((CComboBox*)GetDlgItem(IDC_COMBO1));

	//char szData[200];
	TCHAR szData[200];
	CString strSeg, strIndex, strComboData, strPath;

// 	switch(nSel)
// 	{
// 	case 0:
		strPath.Format(_T("%s"), PATH_MP2100_IO);
// 		break;
// 	case 1:
// 		strPath.Format(_T("%s"), PATH_SLICE_IO);
// 		break;
// 	}


	if (0 < ::GetPrivateProfileString(_T("ADDRESS"), _T("InputStart"), NULL, szData, sizeof(szData), strPath))
		m_nMaxSeg = (long)_tstoi(szData);
	else
		m_nMaxSeg = (long)0L;


	if (0 < ::GetPrivateProfileString(_T("TOTAL"), _T("MAX_SEGMENT"), NULL, szData, sizeof(szData), strPath))
		m_nMaxSeg = (long)_tstoi(szData);
	else
		m_nMaxSeg = (long)0L;

	if (0 < ::GetPrivateProfileString(_T("TOTAL"), _T("IN_SEGMENT"), NULL, szData, sizeof(szData), strPath))
		m_nInSeg = (long)_tstoi(szData);
	else
		m_nInSeg = (long)0L;

	if (0 < ::GetPrivateProfileString(_T("TOTAL"), _T("OUT_SEGMENT"), NULL, szData, sizeof(szData), strPath))
		m_nOutSeg = (long)_tstoi(szData);
	else
		m_nOutSeg = (long)0L;

	int nMaxR = (m_nInSeg>m_nOutSeg)?m_nInSeg:m_nOutSeg;

	pCombo1->ResetContent();

	for(int nR=0; nR < nMaxR; nR++)
	{
		strIndex.Format(_T("%d"), nR);
		if (0 < ::GetPrivateProfileString(_T("COMBO_LIST"), strIndex, NULL, szData, sizeof(szData), strPath))
			strComboData = CString(szData);
		else
			continue;

		pCombo1->InsertString(nR, strComboData);
	}
}

void CDlgUtil04::InitBtn()
{
	myBtn[0].SubclassDlgItem(IDC_BTN_UTIL04_0, this);
	myBtn[0].SetHwnd(this->GetSafeHwnd(), IDC_BTN_UTIL04_0);
//	myBtn[0].SetBoarder(FALSE);
	myBtn[1].SubclassDlgItem(IDC_BTN_UTIL04_1, this);
	myBtn[1].SetHwnd(this->GetSafeHwnd(), IDC_BTN_UTIL04_1);
//	myBtn[1].SetBoarder(FALSE);
	myBtn[2].SubclassDlgItem(IDC_CHK1, this);
	myBtn[2].SetHwnd(this->GetSafeHwnd(), IDC_CHK1);
	myBtn[2].SetBtnType(BTN_TYPE_CHECK);
//	myBtn[2].SetBoarder(FALSE);
	myBtn[3].SubclassDlgItem(IDC_CHK2, this);
	myBtn[3].SetHwnd(this->GetSafeHwnd(), IDC_CHK2);
	myBtn[3].SetBtnType(BTN_TYPE_CHECK);
//	myBtn[3].SetBoarder(FALSE);
	
	int i;
	for(i=0; i<MAX_UTIL04_BTN; i++)
	{
		myBtn[i].SetFont(_T("굴림체"),14,TRUE);
		myBtn[i].SetTextColor(RGB_BLACK);
//		myBtn[i].SetBtnType(BTN_TYPE_CHECK);
	}

	myBtn[2].ShowWindow(SW_HIDE);
	myBtn[3].ShowWindow(SW_HIDE);
}

void CDlgUtil04::InitStatic()
{
	myStc[0].SubclassDlgItem(IDC_STC_UTIL04_0, this);
	myStc[1].SubclassDlgItem(IDC_STC_UTIL04_1, this);
	myStc[2].SubclassDlgItem(IDC_STC_UTIL04_2, this);
	myStc[3].SubclassDlgItem(IDC_STC_UTIL04_3, this);
	myStc[4].SubclassDlgItem(IDC_STC_UTIL04_52, this);
	myStc[5].SubclassDlgItem(IDC_STC_UTIL04_53, this);
	myStc[6].SubclassDlgItem(IDC_STC_UTIL04_54, this);

	myStc[7].SubclassDlgItem(IDC_STC_UTIL04_4, this);
	myStc[8].SubclassDlgItem(IDC_STC_UTIL04_7, this);
	myStc[9].SubclassDlgItem(IDC_STC_UTIL04_10, this);
	myStc[10].SubclassDlgItem(IDC_STC_UTIL04_13, this);
	myStc[11].SubclassDlgItem(IDC_STC_UTIL04_16, this);
	myStc[12].SubclassDlgItem(IDC_STC_UTIL04_19, this);
	myStc[13].SubclassDlgItem(IDC_STC_UTIL04_22, this);
	myStc[14].SubclassDlgItem(IDC_STC_UTIL04_25, this);
	myStc[15].SubclassDlgItem(IDC_STC_UTIL04_28, this);
	myStc[16].SubclassDlgItem(IDC_STC_UTIL04_31, this);
	myStc[17].SubclassDlgItem(IDC_STC_UTIL04_34, this);
	myStc[18].SubclassDlgItem(IDC_STC_UTIL04_37, this);
	myStc[19].SubclassDlgItem(IDC_STC_UTIL04_40, this);
	myStc[20].SubclassDlgItem(IDC_STC_UTIL04_43, this);
	myStc[21].SubclassDlgItem(IDC_STC_UTIL04_46, this);
	myStc[22].SubclassDlgItem(IDC_STC_UTIL04_49, this);

	myStc[23].SubclassDlgItem(IDC_STC_UTIL04_5, this);
	myStc[24].SubclassDlgItem(IDC_STC_UTIL04_8, this);
	myStc[25].SubclassDlgItem(IDC_STC_UTIL04_11, this);
	myStc[26].SubclassDlgItem(IDC_STC_UTIL04_14, this);
	myStc[27].SubclassDlgItem(IDC_STC_UTIL04_17, this);
	myStc[28].SubclassDlgItem(IDC_STC_UTIL04_20, this);
	myStc[29].SubclassDlgItem(IDC_STC_UTIL04_23, this);
	myStc[30].SubclassDlgItem(IDC_STC_UTIL04_26, this);
	myStc[31].SubclassDlgItem(IDC_STC_UTIL04_29, this);
	myStc[32].SubclassDlgItem(IDC_STC_UTIL04_32, this);
	myStc[33].SubclassDlgItem(IDC_STC_UTIL04_35, this);
	myStc[34].SubclassDlgItem(IDC_STC_UTIL04_38, this);
	myStc[35].SubclassDlgItem(IDC_STC_UTIL04_41, this);
	myStc[36].SubclassDlgItem(IDC_STC_UTIL04_44, this);
	myStc[37].SubclassDlgItem(IDC_STC_UTIL04_47, this);
	myStc[38].SubclassDlgItem(IDC_STC_UTIL04_50, this);

	myStc[39].SubclassDlgItem(IDC_STC_UTIL04_55, this);
	myStc[40].SubclassDlgItem(IDC_STC_UTIL04_58, this);
	myStc[41].SubclassDlgItem(IDC_STC_UTIL04_61, this);
	myStc[42].SubclassDlgItem(IDC_STC_UTIL04_64, this);
	myStc[43].SubclassDlgItem(IDC_STC_UTIL04_67, this);
	myStc[44].SubclassDlgItem(IDC_STC_UTIL04_70, this);
	myStc[45].SubclassDlgItem(IDC_STC_UTIL04_73, this);
	myStc[46].SubclassDlgItem(IDC_STC_UTIL04_76, this);
	myStc[47].SubclassDlgItem(IDC_STC_UTIL04_79, this);
	myStc[48].SubclassDlgItem(IDC_STC_UTIL04_82, this);
	myStc[49].SubclassDlgItem(IDC_STC_UTIL04_85, this);
	myStc[50].SubclassDlgItem(IDC_STC_UTIL04_88, this);
	myStc[51].SubclassDlgItem(IDC_STC_UTIL04_91, this);
	myStc[52].SubclassDlgItem(IDC_STC_UTIL04_94, this);
	myStc[53].SubclassDlgItem(IDC_STC_UTIL04_97, this);
	myStc[54].SubclassDlgItem(IDC_STC_UTIL04_100, this);

	myStc[55].SubclassDlgItem(IDC_STC_UTIL04_56, this);
	myStc[56].SubclassDlgItem(IDC_STC_UTIL04_59, this);
	myStc[57].SubclassDlgItem(IDC_STC_UTIL04_62, this);
	myStc[58].SubclassDlgItem(IDC_STC_UTIL04_65, this);
	myStc[59].SubclassDlgItem(IDC_STC_UTIL04_68, this);
	myStc[60].SubclassDlgItem(IDC_STC_UTIL04_71, this);
	myStc[61].SubclassDlgItem(IDC_STC_UTIL04_74, this);
	myStc[62].SubclassDlgItem(IDC_STC_UTIL04_77, this);
	myStc[63].SubclassDlgItem(IDC_STC_UTIL04_80, this);
	myStc[64].SubclassDlgItem(IDC_STC_UTIL04_83, this);
	myStc[65].SubclassDlgItem(IDC_STC_UTIL04_86, this);
	myStc[66].SubclassDlgItem(IDC_STC_UTIL04_89, this);
	myStc[67].SubclassDlgItem(IDC_STC_UTIL04_92, this);
	myStc[68].SubclassDlgItem(IDC_STC_UTIL04_95, this);
	myStc[69].SubclassDlgItem(IDC_STC_UTIL04_98, this);
	myStc[70].SubclassDlgItem(IDC_STC_UTIL04_101, this);

	for(int i=0; i<MAX_UTIL04_STC; i++)
	{
		myStc[i].SetFontName(_T("Arial"));
		myStc[i].SetFontBold(TRUE);
		switch(i)
		{
		case 0:
			myStc[i].SetFontSize(22);
			myStc[i].SetTextColor(RGB_DARKBLUE);
			myStc[i].SetBkColor(RGB_DLG_FRM);
			break;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
			myStc[i].SetFontSize(16);
			myStc[i].SetTextColor(RGB_BLACK);
			myStc[i].SetBkColor(RGB_LTPURPLE);
			break;
		default:
			myStc[i].SetFontSize(12);
			myStc[i].SetTextColor(RGB_BLUE);
			myStc[i].SetBkColor(RGB_WHITE);
			break;
		}
	}
}

void CDlgUtil04::InitLabel()
{
	myLabel[0].SubclassDlgItem(IDC_STC_UTIL04_6, this);
	myLabel[1].SubclassDlgItem(IDC_STC_UTIL04_9, this);
	myLabel[2].SubclassDlgItem(IDC_STC_UTIL04_12, this);
	myLabel[3].SubclassDlgItem(IDC_STC_UTIL04_15, this);
	myLabel[4].SubclassDlgItem(IDC_STC_UTIL04_18, this);
	myLabel[5].SubclassDlgItem(IDC_STC_UTIL04_21, this);
	myLabel[6].SubclassDlgItem(IDC_STC_UTIL04_24, this);
	myLabel[7].SubclassDlgItem(IDC_STC_UTIL04_27, this);
	myLabel[8].SubclassDlgItem(IDC_STC_UTIL04_30, this);
	myLabel[9].SubclassDlgItem(IDC_STC_UTIL04_33, this);
	myLabel[10].SubclassDlgItem(IDC_STC_UTIL04_36, this);
	myLabel[11].SubclassDlgItem(IDC_STC_UTIL04_39, this);
	myLabel[12].SubclassDlgItem(IDC_STC_UTIL04_42, this);
	myLabel[13].SubclassDlgItem(IDC_STC_UTIL04_45, this);
	myLabel[14].SubclassDlgItem(IDC_STC_UTIL04_48, this);
	myLabel[15].SubclassDlgItem(IDC_STC_UTIL04_51, this);

	myLabel[16].SubclassDlgItem(IDC_STC_UTIL04_57, this);
	myLabel[17].SubclassDlgItem(IDC_STC_UTIL04_60, this);
	myLabel[18].SubclassDlgItem(IDC_STC_UTIL04_63, this);
	myLabel[19].SubclassDlgItem(IDC_STC_UTIL04_66, this);
	myLabel[20].SubclassDlgItem(IDC_STC_UTIL04_69, this);
	myLabel[21].SubclassDlgItem(IDC_STC_UTIL04_72, this);
	myLabel[22].SubclassDlgItem(IDC_STC_UTIL04_75, this);
	myLabel[23].SubclassDlgItem(IDC_STC_UTIL04_78, this);
	myLabel[24].SubclassDlgItem(IDC_STC_UTIL04_81, this);
	myLabel[25].SubclassDlgItem(IDC_STC_UTIL04_84, this);
	myLabel[26].SubclassDlgItem(IDC_STC_UTIL04_87, this);
	myLabel[27].SubclassDlgItem(IDC_STC_UTIL04_90, this);
	myLabel[28].SubclassDlgItem(IDC_STC_UTIL04_93, this);
	myLabel[29].SubclassDlgItem(IDC_STC_UTIL04_96, this);
	myLabel[30].SubclassDlgItem(IDC_STC_UTIL04_99, this);
	myLabel[31].SubclassDlgItem(IDC_STC_UTIL04_102, this);

	for(int i=0; i<MAX_UTIL04_LABEL; i++)
	{
		myLabel[i].SetFontName(_T("Arial"));
		myLabel[i].SetFontSize(20);
		myLabel[i].SetFontBold(TRUE);
		myLabel[i].SetTextColor(RGB_BLACK);
		myLabel[i].SetBkColor(RGB_WHITE);
	}
}

void CDlgUtil04::SetBtn(int nSel)
{
	switch(nSel)
	{
	case 0:
		myBtn[2].SetCheck(TRUE);
		myBtn[3].SetCheck(FALSE);
		break;
	case 1:
		myBtn[2].SetCheck(FALSE);
		myBtn[3].SetCheck(TRUE);
		break;			
	}
}

void CDlgUtil04::Disp(int nIdx)
{
	CString strPrev=_T(""), strNext=_T("");
	CComboBox* pCombo1 = ((CComboBox*)GetDlgItem(IDC_COMBO1));
	int nMax = pCombo1->GetCount();

	if(nIdx>0)
		pCombo1->GetLBText(nIdx-1, strPrev);
	else
		strPrev = _T("없음");

	if(nIdx<nMax-1)
		pCombo1->GetLBText(nIdx+1, strNext);
	else
		strNext = _T("없음");

	myBtn[0].SetWindowText(strPrev);
	myBtn[1].SetWindowText(strNext);
	GetDlgItem(IDC_STATIC_PREV)->SetWindowText(strPrev);
	GetDlgItem(IDC_STATIC_NEXT)->SetWindowText(strNext);

// 	switch(m_nSel)
// 	{
// 	case 0:
		DispListMp2100(nIdx);
// 		break;
// 	case 1:
// 		DispListSliceIo(nIdx);
// 		break;
// 	}
}

void CDlgUtil04::SetTextIn(int nRow, int nCol, CString sText)
{
	int nIdx = -1;

	switch(nCol)
	{
	case 0:
		switch(nRow)
		{
		case 1:
			nIdx = 7;
			break;
		case 2:
			nIdx = 8;
			break;
		case 3:
			nIdx = 9;
			break;
		case 4:
			nIdx = 10;
			break;
		case 5:
			nIdx = 11;
			break;
		case 6:
			nIdx = 12;
			break;
		case 7:
			nIdx = 13;
			break;
		case 8:
			nIdx = 14;
			break;
		case 9:
			nIdx = 15;
			break;
		case 10:
			nIdx = 16;
			break;
		case 11:
			nIdx = 17;
			break;
		case 12:
			nIdx = 18;
			break;
		case 13:
			nIdx = 19;
			break;
		case 14:
			nIdx = 20;
			break;
		case 15:
			nIdx = 21;
			break;
		case 16:
			nIdx = 22;
			break;
		}
		break;
	case 1:
		switch(nRow)
		{
		case 1:
			nIdx = 23;
			break;
		case 2:
			nIdx = 24;
			break;
		case 3:
			nIdx = 25;
			break;
		case 4:
			nIdx = 26;
			break;
		case 5:
			nIdx = 27;
			break;
		case 6:
			nIdx = 28;
			break;
		case 7:
			nIdx = 29;
			break;
		case 8:
			nIdx = 30;
			break;
		case 9:
			nIdx = 31;
			break;
		case 10:
			nIdx = 32;
			break;
		case 11:
			nIdx = 33;
			break;
		case 12:
			nIdx = 34;
			break;
		case 13:
			nIdx = 35;
			break;
		case 14:
			nIdx = 36;
			break;
		case 15:
			nIdx = 37;
			break;
		case 16:
			nIdx = 38;
			break;
		}
		break;
	}

	if(nIdx < 0)
		return;

	myStc[nIdx].SetText(sText);
}

void CDlgUtil04::SetTextOut(int nRow, int nCol, CString sText)
{
	int nIdx = -1;

	switch(nCol)
	{
	case 0:
		switch(nRow)
		{
		case 1:
			nIdx = 39;
			break;
		case 2:
			nIdx = 40;
			break;
		case 3:
			nIdx = 41;
			break;
		case 4:
			nIdx = 42;
			break;
		case 5:
			nIdx = 43;
			break;
		case 6:
			nIdx = 44;
			break;
		case 7:
			nIdx = 45;
			break;
		case 8:
			nIdx = 46;
			break;
		case 9:
			nIdx = 47;
			break;
		case 10:
			nIdx = 48;
			break;
		case 11:
			nIdx = 49;
			break;
		case 12:
			nIdx = 50;
			break;
		case 13:
			nIdx = 51;
			break;
		case 14:
			nIdx = 52;
			break;
		case 15:
			nIdx = 53;
			break;
		case 16:
			nIdx = 54;
			break;
		}
		break;
	case 1:
		switch(nRow)
		{
		case 1:
			nIdx = 55;
			break;
		case 2:
			nIdx = 56;
			break;
		case 3:
			nIdx = 57;
			break;
		case 4:
			nIdx = 58;
			break;
		case 5:
			nIdx = 59;
			break;
		case 6:
			nIdx = 60;
			break;
		case 7:
			nIdx = 61;
			break;
		case 8:
			nIdx = 62;
			break;
		case 9:
			nIdx = 63;
			break;
		case 10:
			nIdx = 64;
			break;
		case 11:
			nIdx = 65;
			break;
		case 12:
			nIdx = 66;
			break;
		case 13:
			nIdx = 67;
			break;
		case 14:
			nIdx = 68;
			break;
		case 15:
			nIdx = 69;
			break;
		case 16:
			nIdx = 70;
			break;
		}
		break;
	}

	if(nIdx < 0)
		return;

	myStc[nIdx].SetText(sText);
}

void CDlgUtil04::DispListMp2100(int nIndex)
{
	int nMaxSeg, nSeg, nInSeg, nOutSeg;
	TCHAR sep[] = {_T(",;\r\n\t")};
	TCHAR szData[200];
	TCHAR *token1, *token2, *token3, *token4, *token5;
	//char sep[] = {",;\r\n\t"};
	//char szData[200];
	//char *token1, *token2, *token3, *token4, *token5;
	CString strInSeg, strOutSeg, strAddr, strMReg, strSymbol, strName, strComboData, strPath, strIdx, strMBoth;

	strPath.Format(_T("%s"), PATH_MP2100_IO);

	if (0 < ::GetPrivateProfileString(_T("TOTAL"), _T("MAX_SEGMENT"), NULL, szData, sizeof(szData), strPath))
		nMaxSeg = (long)_tstoi(szData);
	else
		nMaxSeg = (long)0L;

	if (0 < ::GetPrivateProfileString(_T("TOTAL"), _T("IN_SEGMENT"), NULL, szData, sizeof(szData), strPath))
		nInSeg = (long)_tstoi(szData);
	else
		nInSeg = (long)0L;

	if (0 < ::GetPrivateProfileString(_T("TOTAL"), _T("OUT_SEGMENT"), NULL, szData, sizeof(szData), strPath))
		nOutSeg = (long)_tstoi(szData);
	else
		nOutSeg = (long)0L;

	if(nInSeg > nOutSeg)
		nSeg = nInSeg;
	else
		nSeg = nOutSeg;

	if((nSeg <= nIndex) || (nSeg*2 <= nInSeg+nIndex))
		return;

	strInSeg.Format(_T("%d"), nIndex);
	strOutSeg.Format(_T("%d"), nInSeg+nIndex);

	for(int nR=0; nR<16; nR++)
	{
		strIdx.Format(_T("%d"), nR);
		if (0 < ::GetPrivateProfileString(strInSeg, strIdx, NULL, szData, sizeof(szData), strPath))
		{
			//token1 = strtok(szData,sep);
			token1 = _tcstok(szData, sep); // strtok
			token2 = _tcstok(NULL,sep);
			token3 = _tcstok(NULL,sep);

			strAddr = CString(token1);
			strSymbol = CString(token2);
			strName = CString(token3);
		}
		else
		{
			strAddr = _T("");
			strSymbol = _T("");
			strName = _T("");
		}	

		SetTextIn(nR+1, 0, strSymbol);
		SetTextIn(nR+1, 1, strName);
		m_sAddrIn[nR] = strAddr;

		strIdx.Format(_T("%d"), nR);
		if (0 < ::GetPrivateProfileString(strOutSeg, strIdx, NULL, szData, sizeof(szData), strPath))
		{
			token1 = _tcstok(szData,sep);
			token2 = _tcstok(NULL,sep);
			token3 = _tcstok(NULL,sep);
			token4 = _tcstok(NULL,sep);
// 			token5 = strtok(NULL,sep);

			strAddr = CString(token1);
// 			strMReg = CString(token2);
			strSymbol = CString(token2);
			strName = CString(token3);
			strMBoth = CString(token4);
		}
		else
		{
			strAddr = _T("");
// 			strMReg = _T("");
			strSymbol = _T("");
			strName = _T("");
			strMBoth = _T("");
		}	

		SetTextOut(nR+1, 0, strSymbol);
		SetTextOut(nR+1, 1, strName);

		m_sAddrOut[nR] = strAddr;
// 		m_sMRegOut[nR] = strMReg;
		m_sMDblOut[nR] = strMBoth;
	}

// 		m_nStatus = 0;
// 		InitDispStatus();

		
// 		m_nStatus = 0;
// 		InitDispStatus();

		// must be needed
// 		InitManualOutBit();
}

// void CDlgUtil04::DispListSliceIo(int nIndex)
// {
// 	int nMaxSeg, nSeg, nInSeg, nOutSeg;
// 	char sep[] = {",;\r\n\t"};
// 	char szData[200];
// 	char *token0,  *token1, *token2, *token3;
// 	CString strInSeg, strOutSeg, strAddr, strSymbol, strName, strComboData, strPath, strIdx, strMBoth;
// 
// 	strPath.Format(_T("%s"), PATH_SLICE_IO);
// 
// 	if (0 < ::GetPrivateProfileString(_T("TOTAL"), _T("MAX_SEGMENT"), NULL, szData, sizeof(szData), strPath))
// 		nMaxSeg = (long)_tstoi(szData);
// 	else
// 		nMaxSeg = (long)0L;
// 
// 	if (0 < ::GetPrivateProfileString(_T("TOTAL"), _T("IN_SEGMENT"), NULL, szData, sizeof(szData), strPath))
// 		nInSeg = (long)_tstoi(szData);
// 	else
// 		nInSeg = (long)0L;
// 
// 	if (0 < ::GetPrivateProfileString(_T("TOTAL"), _T("OUT_SEGMENT"), NULL, szData, sizeof(szData), strPath))
// 		nOutSeg = (long)_tstoi(szData);
// 	else
// 		nOutSeg = (long)0L;
// 
// 	if(nInSeg > nOutSeg)
// 		nSeg = nInSeg;
// 	else
// 		nSeg = nOutSeg;
// 
// 	if((nSeg <= nIndex) || (nMaxSeg < nInSeg+nIndex))
// 		return;
// 
// 	strInSeg.Format(_T("%d"), nIndex);
// 	strOutSeg.Format(_T("%d"), nInSeg+nIndex);
// 
// 	for(int nR=0; nR<16; nR++)
// 	{
// 		strIdx.Format(_T("%d"), nR);
// 		if (0 < ::GetPrivateProfileString(strInSeg, strIdx, NULL, szData, sizeof(szData), strPath))
// 		{
// 			token0 = strtok(szData,sep);
// 			token1 = strtok(NULL,sep);
// 			token2 = strtok(NULL,sep);
// 
// 			strAddr = CString(token0);
// 			strSymbol = CString(token1);
// 			strName = CString(token2);
// 		}
// 		else
// 		{
// 			strAddr = _T("");
// 			strSymbol = _T("");
// 			strName = _T("");
// 		}	
// 
// 		SetTextIn(nR+1, 0, strSymbol);
// 		SetTextIn(nR+1, 1, strName);
// 
// 		strIdx.Format(_T("%d"), nR);
// 		if (0 < ::GetPrivateProfileString(strOutSeg, strIdx, NULL, szData, sizeof(szData), strPath))
// 		{
// 			token0 = strtok(szData,sep);
// 			token1 = strtok(NULL,sep);
// 			token2 = strtok(NULL,sep);
// 			token3 = strtok(NULL,sep);
// 
// 			strAddr = CString(token0);
// 			strSymbol = CString(token1);
// 			strName = CString(token2);
// 			strMBoth = CString(token3);
// 		}
// 		else
// 		{
// 			strAddr = _T("");
// 			strSymbol = _T("");
// 			strName = _T("");
// 			strMBoth = _T("");
// 		}	
// 
// 		SetTextOut(nR+1, 0, strSymbol);
// 		SetTextOut(nR+1, 1, strName);
// 
// 		m_sSDblOut[nR] = strMBoth;
// 	}
// 
// }

void CDlgUtil04::OnBtnUtil040() 
{
	// TODO: Add your control notification handler code here
	if(m_nComboIndex<=0)
	{
		myBtn[0].SetWindowText(_T("없음"));
		GetDlgItem(IDC_STATIC_PREV)->SetWindowText(_T("없음"));
		return;
	}

	CComboBox* pCombo1 = ((CComboBox*)GetDlgItem(IDC_COMBO1));	

	m_nComboIndex--;
	pCombo1->SetCurSel(m_nComboIndex);
	Disp((int)m_nComboIndex);
}

void CDlgUtil04::OnBtnUtil041() 
{
	// TODO: Add your control notification handler code here
	int nMaxR = (m_nInSeg>m_nOutSeg)?m_nInSeg:m_nOutSeg;
	if(m_nComboIndex >= (nMaxR-1))
	{
		myBtn[1].SetWindowText(_T("없음"));
		GetDlgItem(IDC_STATIC_NEXT)->SetWindowText(_T("없음"));
		return;
	}

	CComboBox* pCombo1 = ((CComboBox*)GetDlgItem(IDC_COMBO1));

	m_nComboIndex++;
	pCombo1->SetCurSel(m_nComboIndex);
	Disp((int)m_nComboIndex);
}

void CDlgUtil04::OnChk1() 
{
	// TODO: Add your control notification handler code here
	m_nSel = 0;
	if(myBtn[2].GetCheck())
	{
		SetCombo(m_nSel);
		m_nComboIndex = 0;
	}
	SetBtn(m_nSel);

	CComboBox* pCombo1 = ((CComboBox*)GetDlgItem(IDC_COMBO1));
	pCombo1->SetCurSel(m_nComboIndex);
	Disp(m_nComboIndex);
}

void CDlgUtil04::OnChk2() 
{
	// TODO: Add your control notification handler code here
	m_nSel = 1;
	if(myBtn[2].GetCheck())
	{
		SetCombo(m_nSel);
		m_nComboIndex = 0;
	}
	SetBtn(m_nSel);	

	CComboBox* pCombo1 = ((CComboBox*)GetDlgItem(IDC_COMBO1));
	pCombo1->SetCurSel(m_nComboIndex);
	Disp(m_nComboIndex);
}

void CDlgUtil04::OnSelchangeCombo1() 
{
	// TODO: Add your control notification handler code here
	ApplyCombo1();	
}

BOOL CDlgUtil04::ApplyCombo1() 
{
	// TODO: Add your control notification handler code here

	CString strVal;
	CComboBox* pCombo1 = ((CComboBox*)GetDlgItem(IDC_COMBO1));

	pCombo1->GetWindowText(strVal);
	if(strVal == "")
		return FALSE;

	int nIdx = pCombo1->GetCurSel();

	m_nComboIndex = nIdx;
	Disp((int)m_nComboIndex);

	return TRUE;
}

void CDlgUtil04::OnTimer(UINT_PTR nIDEvent)//(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent==TIM_DISP_STS)
	{
		KillTimer(TIM_DISP_STS);
		DispStatus();
		if(m_bTIM_DISP_STS)
			SetTimer(TIM_DISP_STS, 100, NULL);
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CDlgUtil04::DispStatus()
{
	unsigned short usIn=0, usOut=0;
	if(m_nSel==0) // MpeIO
	{
#ifdef USE_MPE
		if(m_nComboIndex<m_nInSeg)
			usIn = pDoc->m_pMpeIb[m_nComboIndex];
		else
#endif
			usIn = 0;

#ifdef USE_MPE
		if(m_nInSeg+m_nComboIndex<m_nMaxSeg)
			usOut = pDoc->m_pMpeIo[m_nInSeg+m_nComboIndex];
		else
#endif
			usOut = 0;
	}
// 	else if(m_nSel==1)	// SliceIO
// 	{
// 		if(m_nComboIndex<m_nInSeg)
// 			usIn = pDoc->m_pSliceIo[m_nComboIndex];
// 		else
// 			usIn = 0;
// 
// 		if(m_nInSeg+m_nComboIndex<m_nMaxSeg)
// 			usOut = pDoc->m_pSliceIo[m_nInSeg+m_nComboIndex];
// 		else
// 			usOut = 0;
// 	}
	else
		return;

	for(int nR=0; nR<16; nR++)
	{
		if(usIn & (0x01<<nR))
			myLabel[nR].SetImageBk(LBL_IMG_DN);
		else
			myLabel[nR].SetImageBk(LBL_IMG_UP);

		if(usOut & (0x01<<nR))
			myLabel[16+nR].SetImageBk(LBL_IMG_DN);
		else
			myLabel[16+nR].SetImageBk(LBL_IMG_UP);
	}
}

void CDlgUtil04::ShowDlgMyIo(CString sSym, CString sText, int nIdx) 
{
	// TODO: Add your control notification handler code here
// 	if(m_nSel==0) // MpeIO
// 	{
		CString sParam[2];
		sParam[0] = m_sAddrOut[nIdx];
// 		sParam[1] = m_sMRegOut[nIdx];
		sParam[1] = m_sMDblOut[nIdx];
		CDlgUtil05 dlg(this);
		dlg.SetDisp(sSym, sText);
		dlg.SetParamMp2100(sParam);
		dlg.SetParamMpeIo(m_nInSeg+m_nComboIndex, nIdx);
		dlg.DoModal();
// 	}
// 	else if(m_nSel==1)	// SliceIO
// 	{
// 		CDlgUtil05 dlg(this);
// 		dlg.SetDisp(sSym, sText);
// 		dlg.SetParamSliceIo(m_nInSeg+m_nComboIndex, nIdx);
// 		dlg.DoModal();
// 	}
}

void CDlgUtil04::OnStcUtil0456() 
{
	// TODO: Add your control notification handler code here
	myStc[55].SetBkColor(RGB_LT_PINK);
	CString sSym = myStc[39].GetText();
	CString sText = myStc[55].GetText();
	ShowDlgMyIo(sSym, sText, 0);
	myStc[55].SetBkColor(RGB_WHITE);
}

void CDlgUtil04::OnStcUtil0459() 
{
	// TODO: Add your control notification handler code here
	myStc[56].SetBkColor(RGB_LT_PINK);
	CString sSym = myStc[40].GetText();
	CString sText = myStc[56].GetText();
	ShowDlgMyIo(sSym, sText, 1);
	myStc[56].SetBkColor(RGB_WHITE);	
}

void CDlgUtil04::OnStcUtil0462() 
{
	// TODO: Add your control notification handler code here
	myStc[57].SetBkColor(RGB_LT_PINK);
	CString sSym = myStc[41].GetText();
	CString sText = myStc[57].GetText();
	ShowDlgMyIo(sSym, sText, 2);
	myStc[57].SetBkColor(RGB_WHITE);	
}

void CDlgUtil04::OnStcUtil0465() 
{
	// TODO: Add your control notification handler code here
	myStc[58].SetBkColor(RGB_LT_PINK);
	CString sSym = myStc[42].GetText();
	CString sText = myStc[58].GetText();
	ShowDlgMyIo(sSym, sText, 3);
	myStc[58].SetBkColor(RGB_WHITE);	
}

void CDlgUtil04::OnStcUtil0468() 
{
	// TODO: Add your control notification handler code here
	myStc[59].SetBkColor(RGB_LT_PINK);
	CString sSym = myStc[43].GetText();
	CString sText = myStc[59].GetText();
	ShowDlgMyIo(sSym, sText, 4);
	myStc[59].SetBkColor(RGB_WHITE);	
}

void CDlgUtil04::OnStcUtil0471() 
{
	// TODO: Add your control notification handler code here
	myStc[60].SetBkColor(RGB_LT_PINK);
	CString sSym = myStc[44].GetText();
	CString sText = myStc[60].GetText();
	ShowDlgMyIo(sSym, sText, 5);
	myStc[60].SetBkColor(RGB_WHITE);	
}

void CDlgUtil04::OnStcUtil0474() 
{
	// TODO: Add your control notification handler code here
	myStc[61].SetBkColor(RGB_LT_PINK);
	CString sSym = myStc[45].GetText();
	CString sText = myStc[61].GetText();
	ShowDlgMyIo(sSym, sText, 6);
	myStc[61].SetBkColor(RGB_WHITE);	
}

void CDlgUtil04::OnStcUtil0477() 
{
	// TODO: Add your control notification handler code here
	myStc[62].SetBkColor(RGB_LT_PINK);
	CString sSym = myStc[46].GetText();
	CString sText = myStc[62].GetText();
	ShowDlgMyIo(sSym, sText, 7);
	myStc[62].SetBkColor(RGB_WHITE);	
}

void CDlgUtil04::OnStcUtil0480() 
{
	// TODO: Add your control notification handler code here
	myStc[63].SetBkColor(RGB_LT_PINK);
	CString sSym = myStc[47].GetText();
	CString sText = myStc[63].GetText();
	ShowDlgMyIo(sSym, sText, 8);
	myStc[63].SetBkColor(RGB_WHITE);	
}

void CDlgUtil04::OnStcUtil0483() 
{
	// TODO: Add your control notification handler code here
	myStc[64].SetBkColor(RGB_LT_PINK);
	CString sSym = myStc[48].GetText();
	CString sText = myStc[64].GetText();
	ShowDlgMyIo(sSym, sText, 9);
	myStc[64].SetBkColor(RGB_WHITE);	
}

void CDlgUtil04::OnStcUtil0486() 
{
	// TODO: Add your control notification handler code here
	myStc[65].SetBkColor(RGB_LT_PINK);
	CString sSym = myStc[49].GetText();
	CString sText = myStc[65].GetText();
	ShowDlgMyIo(sSym, sText, 10);
	myStc[65].SetBkColor(RGB_WHITE);	
}

void CDlgUtil04::OnStcUtil0489() 
{
	// TODO: Add your control notification handler code here
	myStc[66].SetBkColor(RGB_LT_PINK);
	CString sSym = myStc[50].GetText();
	CString sText = myStc[66].GetText();
	ShowDlgMyIo(sSym, sText, 11);
	myStc[66].SetBkColor(RGB_WHITE);	
}

void CDlgUtil04::OnStcUtil0492() 
{
	// TODO: Add your control notification handler code here
	myStc[67].SetBkColor(RGB_LT_PINK);
	CString sSym = myStc[51].GetText();
	CString sText = myStc[67].GetText();
	ShowDlgMyIo(sSym, sText, 12);
	myStc[67].SetBkColor(RGB_WHITE);	
}

void CDlgUtil04::OnStcUtil0495() 
{
	// TODO: Add your control notification handler code here
	myStc[68].SetBkColor(RGB_LT_PINK);
	CString sSym = myStc[52].GetText();
	CString sText = myStc[68].GetText();
	ShowDlgMyIo(sSym, sText, 13);
	myStc[68].SetBkColor(RGB_WHITE);	
}

void CDlgUtil04::OnStcUtil0498() 
{
	// TODO: Add your control notification handler code here
	myStc[69].SetBkColor(RGB_LT_PINK);
	CString sSym = myStc[53].GetText();
	CString sText = myStc[69].GetText();
	ShowDlgMyIo(sSym, sText, 14);
	myStc[69].SetBkColor(RGB_WHITE);	
}

void CDlgUtil04::OnStcUtil04101() 
{
	// TODO: Add your control notification handler code here
	myStc[70].SetBkColor(RGB_LT_PINK);
	CString sSym = myStc[54].GetText();
	CString sText = myStc[70].GetText();
	ShowDlgMyIo(sSym, sText, 15);
	myStc[70].SetBkColor(RGB_WHITE);	
}

BOOL CDlgUtil04::PreTranslateMessage(MSG* pMsg) 
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

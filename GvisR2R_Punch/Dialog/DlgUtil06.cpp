// DlgUtil06.cpp : implementation file
//
#ifdef USE_FLUCK

#include "stdafx.h"
#include "../gvisr2r_punch.h"
#include "DlgUtil06.h"
#include "DlgKeyNum.h"

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
// CDlgUtil06 dialog


CDlgUtil06::CDlgUtil06(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUtil06::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgUtil06)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pRect = NULL;
}

CDlgUtil06::~CDlgUtil06()
{
	if(m_pRect)
	{
		delete m_pRect;
		m_pRect = NULL;
	}
}

BOOL CDlgUtil06::Create()
{
	return CDialog::Create(CDlgUtil06::IDD);
}


void CDlgUtil06::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgUtil06)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgUtil06, CDialog)
	//{{AFX_MSG_MAP(CDlgUtil06)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_STC_132, OnStc132)
	ON_BN_CLICKED(IDC_STC_136, OnStc136)
	ON_BN_CLICKED(IDC_STC_139, OnStc139)
	ON_BN_CLICKED(IDC_STC_143, OnStc143)
	ON_BN_CLICKED(IDC_STC_147, OnStc147)
	ON_BN_CLICKED(IDC_STC_150, OnStc150)
	ON_BN_CLICKED(IDC_STC_153, OnStc153)
	ON_BN_CLICKED(IDC_STC_93, OnStc93)
	ON_BN_CLICKED(IDC_STC_97, OnStc97)
	ON_BN_CLICKED(IDC_STC_110, OnStc110)
	ON_BN_CLICKED(IDC_STC_113, OnStc113)
	ON_BN_CLICKED(IDC_STC_117, OnStc117)
	ON_BN_CLICKED(IDC_STC_120, OnStc120)
	ON_BN_CLICKED(IDC_STC_123, OnStc123)
	ON_BN_CLICKED(IDC_STC_161, OnStc161)
	ON_BN_CLICKED(IDC_STC_165, OnStc165)
	ON_BN_CLICKED(IDC_BTN_MEAS_START, OnBtnMeasStart)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_MEAS_MOVE, OnBtnMeasMove)
	ON_BN_CLICKED(IDC_BTN_MEAS_DOWN, OnBtnMeasDown)
	ON_BN_CLICKED(IDC_BTN_MEAS_SAVE, OnBtnMeasSave)
	ON_BN_CLICKED(IDC_BTN_MEAS_MOVE2, OnBtnMeasMove2)
	ON_BN_CLICKED(IDC_BTN_MEAS_DOWN2, OnBtnMeasDown2)
	ON_BN_CLICKED(IDC_BTN_MEAS_SAVE2, OnBtnMeasSave2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgUtil06 message handlers

void CDlgUtil06::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if(!m_pRect)
	{
		m_pRect = new CRect;
		
		this->GetClientRect(m_pRect);
		m_pRect->top = 75;
		m_pRect->bottom += 75;
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

void CDlgUtil06::AtDlgShow()
{
	LoadImg();
	Disp();
}

void CDlgUtil06::AtDlgHide()
{
	DelImg();
}

void CDlgUtil06::LoadImg()
{
	if(m_bLoadImg)
		return;
	m_bLoadImg = TRUE;
	
	int i;
	for(i=0; i<MAX_UTIL06_BTN; i++)
	{
		myBtn[i].LoadBkImage(IMG_BTN_UP_DlgUtil03, BTN_IMG_UP);
		myBtn[i].LoadBkImage(IMG_BTN_DN_DlgUtil03, BTN_IMG_DN);
	}
}

void CDlgUtil06::DelImg()
{
	if(!m_bLoadImg)
		return;
	m_bLoadImg = FALSE;
}

BOOL CDlgUtil06::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitBtn();
	InitStatic();

	GetDlgItem(IDC_STC_159)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_161)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_160)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STC_145)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_142)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_143)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_144)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_146)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_147)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_148)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_149)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_150)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_151)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_152)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_153)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_154)->ShowWindow(SW_HIDE);
	
	GetDlgItem(IDC_STC_115)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_112)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_113)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_114)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_116)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_117)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_118)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_119)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_120)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_121)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_122)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_123)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_124)->ShowWindow(SW_HIDE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgUtil06::InitBtn()
{
	myBtn[0].SubclassDlgItem(IDC_BTN_MEAS_MOVE, this);
	myBtn[0].SetHwnd(this->GetSafeHwnd(), IDC_BTN_MEAS_MOVE);
	myBtn[0].SetFont(_T("±¼¸²Ã¼"),16,TRUE);
	myBtn[0].SetTextColor(RGB_BLACK);

	myBtn[1].SubclassDlgItem(IDC_BTN_MEAS_SAVE, this);
	myBtn[1].SetHwnd(this->GetSafeHwnd(), IDC_BTN_MEAS_SAVE);
	myBtn[1].SetFont(_T("±¼¸²Ã¼"),16,TRUE);
	myBtn[1].SetTextColor(RGB_BLACK);

	myBtn[2].SubclassDlgItem(IDC_BTN_MEAS_DOWN, this); 
	myBtn[2].SetHwnd(this->GetSafeHwnd(), IDC_BTN_MEAS_DOWN);
	myBtn[2].SetText(_T("Prob\rÇÏ°­"), BTN_UP);
	myBtn[2].SetText(_T("Prob\r»ó½Â"), BTN_DN);
	myBtn[2].SetBtnType(BTN_TYPE_CHECK);
	myBtn[2].SetFont(_T("±¼¸²Ã¼"),16,TRUE);
	myBtn[2].SetTextColor(RGB_BLACK);
	
	myBtn[3].SubclassDlgItem(IDC_BTN_MEAS_START, this);
	myBtn[3].SetHwnd(this->GetSafeHwnd(), IDC_BTN_MEAS_START);
	myBtn[3].SetFont(_T("±¼¸²Ã¼"),16,TRUE);
	myBtn[3].SetTextColor(RGB_BLACK);

	myBtn[4].SubclassDlgItem(IDC_BTN_MEAS_MOVE2, this);
	myBtn[4].SetHwnd(this->GetSafeHwnd(), IDC_BTN_MEAS_MOVE2);
	myBtn[4].SetFont(_T("±¼¸²Ã¼"),16,TRUE);
	myBtn[4].SetTextColor(RGB_BLACK);
	
	myBtn[5].SubclassDlgItem(IDC_BTN_MEAS_SAVE2, this);
	myBtn[5].SetHwnd(this->GetSafeHwnd(), IDC_BTN_MEAS_SAVE2);
	myBtn[5].SetFont(_T("±¼¸²Ã¼"),16,TRUE);
	myBtn[5].SetTextColor(RGB_BLACK);

	myBtn[6].SubclassDlgItem(IDC_BTN_MEAS_DOWN2, this);
	myBtn[6].SetHwnd(this->GetSafeHwnd(), IDC_BTN_MEAS_DOWN2);
	myBtn[6].SetText(_T("Prob\rÇÏ°­"), BTN_UP);
	myBtn[6].SetText(_T("Prob\r»ó½Â"), BTN_DN);
	myBtn[6].SetBtnType(BTN_TYPE_CHECK);
	myBtn[6].SetFont(_T("±¼¸²Ã¼"),16,TRUE);
	myBtn[6].SetTextColor(RGB_BLACK);
}

void CDlgUtil06::InitStatic()
{
	InitStcTitle();
	InitStcData();
}

void CDlgUtil06::InitStcTitle()
{
	int i;

	myStcTitle[0].SubclassDlgItem(IDC_STC_129, this);
	myStcTitle[0].SetFontName(_T("Arial"));
	myStcTitle[0].SetFontSize(14);
	myStcTitle[0].SetTextColor(RGB_NAVY);
	myStcTitle[0].SetBkColor(RGB_LTDKORANGE);
	myStcTitle[0].SetFontBold(TRUE);

	myStcTitle[1].SubclassDlgItem(IDC_STC_130, this);
	myStcTitle[1].SetFontName(_T("Arial"));
	myStcTitle[1].SetFontSize(14);
	myStcTitle[1].SetTextColor(RGB_WHITE);
	myStcTitle[1].SetBkColor(RGB_DARKGREEN);
	myStcTitle[1].SetFontBold(TRUE);

	GetDlgItem(IDC_STC_134)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_131)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_135)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_138)->ShowWindow(SW_HIDE);
// 	myStcTitle[2].SubclassDlgItem(IDC_STC_134, this);
// 	myStcTitle[3].SubclassDlgItem(IDC_STC_131, this);
// 	myStcTitle[4].SubclassDlgItem(IDC_STC_135, this);
// 	myStcTitle[5].SubclassDlgItem(IDC_STC_138, this);

	myStcTitle[6].SubclassDlgItem(IDC_STC_145, this);
	myStcTitle[7].SubclassDlgItem(IDC_STC_142, this);
	myStcTitle[8].SubclassDlgItem(IDC_STC_146, this);
	myStcTitle[9].SubclassDlgItem(IDC_STC_149, this);
	myStcTitle[10].SubclassDlgItem(IDC_STC_152, this);

	myStcTitle[11].SubclassDlgItem(IDC_STC_158, this);
	myStcTitle[12].SubclassDlgItem(IDC_STC_180, this);

// 	for(i=2; i<=12; i++)
	for(i=6; i<=12; i++)
	{
		myStcTitle[i].SetFontName(_T("Arial"));
		myStcTitle[i].SetFontSize(14);
		myStcTitle[i].SetTextColor(RGB_NAVY);
		myStcTitle[i].SetBkColor(RGB_LTGRAY);
		myStcTitle[i].SetFontBold(TRUE);
	}

	GetDlgItem(IDC_STC_133)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_137)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_140)->ShowWindow(SW_HIDE);
// 	myStcTitle[13].SubclassDlgItem(IDC_STC_133, this);
// 	myStcTitle[14].SubclassDlgItem(IDC_STC_137, this);
// 	myStcTitle[15].SubclassDlgItem(IDC_STC_140, this);
	myStcTitle[16].SubclassDlgItem(IDC_STC_144, this);
	myStcTitle[17].SubclassDlgItem(IDC_STC_148, this);
	myStcTitle[18].SubclassDlgItem(IDC_STC_151, this);
	myStcTitle[19].SubclassDlgItem(IDC_STC_154, this);
	myStcTitle[20].SubclassDlgItem(IDC_STC_156, this);
	
	for(i=16; i<=20; i++)
	{
		myStcTitle[i].SetFontName(_T("Arial"));
		myStcTitle[i].SetFontSize(12);
		myStcTitle[i].SetTextColor(RGB_NAVY);
		myStcTitle[i].SetBkColor(RGB_WHITE);
		myStcTitle[i].SetFontBold(FALSE);
	}


	myStcTitle[21].SubclassDlgItem(IDC_STC_90, this);
	myStcTitle[21].SetFontName(_T("Arial"));
	myStcTitle[21].SetFontSize(14);
	myStcTitle[21].SetTextColor(RGB_NAVY);
	myStcTitle[21].SetBkColor(RGB_LTDKORANGE);
	myStcTitle[21].SetFontBold(TRUE);

	myStcTitle[22].SubclassDlgItem(IDC_STC_91, this);
	myStcTitle[22].SetFontName(_T("Arial"));
	myStcTitle[22].SetFontSize(14);
	myStcTitle[22].SetTextColor(RGB_WHITE);
	myStcTitle[22].SetBkColor(RGB_DARKGREEN);
	myStcTitle[22].SetFontBold(TRUE);

	GetDlgItem(IDC_STC_95)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_92)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_96)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_99)->ShowWindow(SW_HIDE);
// 	myStcTitle[23].SubclassDlgItem(IDC_STC_95, this);
// 	myStcTitle[24].SubclassDlgItem(IDC_STC_92, this);
// 	myStcTitle[25].SubclassDlgItem(IDC_STC_96, this);
// 	myStcTitle[26].SubclassDlgItem(IDC_STC_99, this);

	myStcTitle[27].SubclassDlgItem(IDC_STC_115, this);
	myStcTitle[28].SubclassDlgItem(IDC_STC_112, this);
	myStcTitle[29].SubclassDlgItem(IDC_STC_116, this);
	myStcTitle[30].SubclassDlgItem(IDC_STC_119, this);
	myStcTitle[31].SubclassDlgItem(IDC_STC_122, this);

	myStcTitle[32].SubclassDlgItem(IDC_STC_128, this);
	myStcTitle[33].SubclassDlgItem(IDC_STC_125, this);
	
// 	for(i=23; i<=33; i++)
	for(i=27; i<=33; i++)
	{
		myStcTitle[i].SetFontName(_T("Arial"));
		myStcTitle[i].SetFontSize(14);
		myStcTitle[i].SetTextColor(RGB_NAVY);
		myStcTitle[i].SetBkColor(RGB_LTGRAY);
		myStcTitle[i].SetFontBold(TRUE);
	}

	GetDlgItem(IDC_STC_94)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_98)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_111)->ShowWindow(SW_HIDE);
// 	myStcTitle[34].SubclassDlgItem(IDC_STC_94, this);
// 	myStcTitle[35].SubclassDlgItem(IDC_STC_98, this);
// 	myStcTitle[36].SubclassDlgItem(IDC_STC_111, this);
	myStcTitle[37].SubclassDlgItem(IDC_STC_114, this);
	myStcTitle[38].SubclassDlgItem(IDC_STC_118, this);
	myStcTitle[39].SubclassDlgItem(IDC_STC_121, this);
	myStcTitle[40].SubclassDlgItem(IDC_STC_124, this);
	myStcTitle[41].SubclassDlgItem(IDC_STC_127, this);
	
	for(i=37; i<=41; i++)
	{
		myStcTitle[i].SetFontName(_T("Arial"));
		myStcTitle[i].SetFontSize(12);
		myStcTitle[i].SetTextColor(RGB_NAVY);
		myStcTitle[i].SetBkColor(RGB_WHITE);
		myStcTitle[i].SetFontBold(FALSE);
	}
	
	myStcTitle[42].SubclassDlgItem(IDC_STC_159, this);
	myStcTitle[42].SetFontName(_T("Arial"));
	myStcTitle[42].SetFontSize(14);
	myStcTitle[42].SetTextColor(RGB_NAVY);
	myStcTitle[42].SetBkColor(RGB_LTGRAY);
	myStcTitle[42].SetFontBold(TRUE);

	myStcTitle[43].SubclassDlgItem(IDC_STC_160, this);
	myStcTitle[43].SetFontName(_T("Arial"));
	myStcTitle[43].SetFontSize(12);
	myStcTitle[43].SetTextColor(RGB_NAVY);
	myStcTitle[43].SetBkColor(RGB_WHITE);
	myStcTitle[43].SetFontBold(FALSE);

	
	myStcTitle[44].SubclassDlgItem(IDC_STC_163, this);
	myStcTitle[44].SetFontName(_T("Arial"));
	myStcTitle[44].SetFontSize(14);
	myStcTitle[44].SetTextColor(RGB_NAVY);
	myStcTitle[44].SetBkColor(RGB_LTGRAY);
	myStcTitle[44].SetFontBold(TRUE);

	myStcTitle[45].SubclassDlgItem(IDC_STC_164, this);
	myStcTitle[45].SetFontName(_T("Arial"));
	myStcTitle[45].SetFontSize(12);
	myStcTitle[45].SetTextColor(RGB_NAVY);
	myStcTitle[45].SetBkColor(RGB_WHITE);
	myStcTitle[45].SetFontBold(FALSE);
}


void CDlgUtil06::InitStcData()
{
	int i;

	GetDlgItem(IDC_STC_132)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_136)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_139)->ShowWindow(SW_HIDE);
// 	myStcData[0].SubclassDlgItem(IDC_STC_132, this);
// 	myStcData[1].SubclassDlgItem(IDC_STC_136, this);
// 	myStcData[2].SubclassDlgItem(IDC_STC_139, this);
	myStcData[3].SubclassDlgItem(IDC_STC_143, this);
	myStcData[4].SubclassDlgItem(IDC_STC_147, this);
	myStcData[5].SubclassDlgItem(IDC_STC_150, this);
	myStcData[6].SubclassDlgItem(IDC_STC_153, this);
	myStcData[7].SubclassDlgItem(IDC_STC_155, this);
	myStcData[8].SubclassDlgItem(IDC_STC_157, this);
	for(i=3; i<=8; i++)
	{
		myStcData[i].SetFontName(_T("Arial"));
		myStcData[i].SetFontSize(14);
		myStcData[i].SetFontBold(TRUE);
		myStcData[i].SetTextColor(RGB_BLACK);
		myStcData[i].SetBkColor(RGB_WHITE);
	}

	GetDlgItem(IDC_STC_93)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_97)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_110)->ShowWindow(SW_HIDE);
// 	myStcData[9].SubclassDlgItem(IDC_STC_93, this);
// 	myStcData[10].SubclassDlgItem(IDC_STC_97, this);
// 	myStcData[11].SubclassDlgItem(IDC_STC_110, this);
	myStcData[12].SubclassDlgItem(IDC_STC_113, this);
	myStcData[13].SubclassDlgItem(IDC_STC_117, this);
	myStcData[14].SubclassDlgItem(IDC_STC_120, this);
	myStcData[15].SubclassDlgItem(IDC_STC_123, this);
	myStcData[16].SubclassDlgItem(IDC_STC_126, this);
	myStcData[17].SubclassDlgItem(IDC_STC_141, this);
//	for(int i=0; i<MAX_UTIL06_STC_DATA; i++)
	for(i=12; i<=17; i++)
	{
		myStcData[i].SetFontName(_T("Arial"));
		myStcData[i].SetFontSize(14);
		myStcData[i].SetFontBold(TRUE);
		myStcData[i].SetTextColor(RGB_BLACK);
		myStcData[i].SetBkColor(RGB_WHITE);
	}

	myStcData[18].SubclassDlgItem(IDC_STC_161, this);
	myStcData[18].SetFontName(_T("Arial"));
	myStcData[18].SetFontSize(16);
	myStcData[18].SetFontBold(TRUE);
	myStcData[18].SetTextColor(RGB_BLACK);
	myStcData[18].SetBkColor(RGB_WHITE);
	
	myStcData[19].SubclassDlgItem(IDC_STC_165, this);
	myStcData[19].SetFontName(_T("Arial"));
	myStcData[19].SetFontSize(16);
	myStcData[19].SetFontBold(TRUE);
	myStcData[19].SetTextColor(RGB_BLACK);
	myStcData[19].SetBkColor(RGB_WHITE);
	
	myStcData[20].SubclassDlgItem(IDC_STC_166, this);
	myStcData[20].SetFontName(_T("Arial"));
	myStcData[20].SetFontSize(26);
	myStcData[20].SetFontBold(TRUE);
	myStcData[20].SetTextColor(RGB_GREEN);
	myStcData[20].SetBkColor(RGB_BLACK);

}

void CDlgUtil06::Disp()
{
	CString str;
	
// 	str.Format(_T("%.2f"), _tstof(pDoc->WorkingInfo.Probing[0].sWaitPos));
// 	myStcData[0].SetText(str);
// 	str.Format(_T("%.0f"), _tstof(pDoc->WorkingInfo.Probing[0].sWaitVel));
// 	myStcData[1].SetText(str);
// 	str.Format(_T("%.0f"), _tstof(pDoc->WorkingInfo.Probing[0].sWaitAcc));
// 	myStcData[2].SetText(str);
	
	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Probing[0].sProbingPos));
	myStcData[3].SetText(str);
	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Probing[0].sProbingVel));
	myStcData[4].SetText(str);
	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Probing[0].sProbingAcc));
	myStcData[5].SetText(str);
	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Probing[0].sProbingToq));
	myStcData[6].SetText(str);
	
	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Probing[0].sMeasurePosX));
	myStcData[7].SetText(str);
	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Probing[0].sMeasurePosY));
	myStcData[8].SetText(str);


// 	str.Format(_T("%.2f"), _tstof(pDoc->WorkingInfo.Probing[1].sWaitPos));
// 	myStcData[9].SetText(str);
// 	str.Format(_T("%.0f"), _tstof(pDoc->WorkingInfo.Probing[1].sWaitVel));
// 	myStcData[10].SetText(str);
// 	str.Format(_T("%.0f"), _tstof(pDoc->WorkingInfo.Probing[1].sWaitAcc));
// 	myStcData[11].SetText(str);
	
	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Probing[1].sProbingPos));
	myStcData[12].SetText(str);
	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Probing[1].sProbingVel));
	myStcData[13].SetText(str);
	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Probing[1].sProbingAcc));
	myStcData[14].SetText(str);
	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Probing[1].sProbingToq));
	myStcData[15].SetText(str);
	
	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Probing[1].sMeasurePosX));
	myStcData[16].SetText(str);
	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Probing[1].sMeasurePosY));
	myStcData[17].SetText(str);
	
	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Fluck.sThreshold));
	myStcData[18].SetText(str);
	str.Format(_T("%d"), _tstoi(pDoc->WorkingInfo.Fluck.sRejectMkNum));
	myStcData[19].SetText(str);
}

BOOL CDlgUtil06::ShowKeypad(int nCtlID, CPoint ptSt, int nDir)
{
	BOOL bAdj = TRUE;
	CString strData, strPrev;
	GetDlgItemText(nCtlID, strData);
	strPrev = strData;
	
	CString strMin, strMax;
	CRect rect(0,0,0,0);
	CDlgKeyNum *pDlg = new CDlgKeyNum(&strData, &rect, ptSt, nDir);
	pDlg->m_strCurr = strData;
	pDlg->m_strMin = strMin = pDoc->GetMin(IDD_DLG_UTIL_06, nCtlID);
	pDlg->m_strMax = strMax = pDoc->GetMax(IDD_DLG_UTIL_06, nCtlID);
	pDlg->DoModal();
	delete pDlg;
	
	if(!strMin.IsEmpty() && !strMax.IsEmpty())
	{
		if(_tstof(strData) < _tstof(strMin) || 
			_tstof(strData) > _tstof(strMax))
		{
			SetDlgItemText(nCtlID, strPrev);
			pView->DispMsg("ÀÔ·Â ¹üÀ§¸¦ ¹þ¾î³µ½À´Ï´Ù."), _T("ÁÖÀÇ", RGB_YELLOW);
		}
		else
			SetDlgItemText(nCtlID, strData);
	}
	else
		SetDlgItemText(nCtlID, strData);
	
	return bAdj;
}


void CDlgUtil06::OnStc132() 
{
	// TODO: Add your control notification handler code here
	myStcData[0].SetBkColor(RGB_RED);
	myStcData[0].RedrawWindow();
	
	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_132)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_132, pt, TO_BOTTOM|TO_RIGHT);
	
	myStcData[0].SetBkColor(RGB_WHITE);
	myStcData[0].RedrawWindow();
	
	CString sData;
	GetDlgItem(IDC_STC_132)->GetWindowText(sData);
	pDoc->SetProbWaitPos(0, _tstof(sData));
	
	pView->m_pVoiceCoil[0]->SetProbShiftData(0);
}

void CDlgUtil06::OnStc136() 
{
	// TODO: Add your control notification handler code here
	myStcData[1].SetBkColor(RGB_RED);
	myStcData[1].RedrawWindow();
	
	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_136)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_136, pt, TO_BOTTOM|TO_RIGHT);
	
	myStcData[1].SetBkColor(RGB_WHITE);
	myStcData[1].RedrawWindow();
	
	CString sData;
	GetDlgItem(IDC_STC_136)->GetWindowText(sData);
	pDoc->SetProbingVel(0, _tstof(sData));
	
	pView->m_pVoiceCoil[0]->SetProbFinalData(0);
}

void CDlgUtil06::OnStc139() 
{
	// TODO: Add your control notification handler code here
	myStcData[2].SetBkColor(RGB_RED);
	myStcData[2].RedrawWindow();
	
	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_139)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_139, pt, TO_BOTTOM|TO_RIGHT);
	
	myStcData[2].SetBkColor(RGB_WHITE);
	myStcData[2].RedrawWindow();
	
	CString sData;
	GetDlgItem(IDC_STC_139)->GetWindowText(sData);
	pDoc->SetProbWaitAcc(0, _tstof(sData));	
	
	pView->m_pVoiceCoil[0]->SetProbShiftData(0);
}


void CDlgUtil06::OnStc143() 
{
	// TODO: Add your control notification handler code here
	myStcData[3].SetBkColor(RGB_RED);
	myStcData[3].RedrawWindow();
	
	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_143)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_143, pt, TO_BOTTOM|TO_RIGHT);
	
	myStcData[3].SetBkColor(RGB_WHITE);
	myStcData[3].RedrawWindow();
	
	CString sData;
	GetDlgItem(IDC_STC_143)->GetWindowText(sData);
	pDoc->SetProbingPos(0, _tstof(sData));	
	
	pView->m_pVoiceCoil[0]->SetProbFinalData(0);
}

void CDlgUtil06::OnStc147() 
{
	// TODO: Add your control notification handler code here
	myStcData[4].SetBkColor(RGB_RED);
	myStcData[4].RedrawWindow();
	
	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_147)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_147, pt, TO_BOTTOM|TO_RIGHT);
	
	myStcData[4].SetBkColor(RGB_WHITE);
	myStcData[4].RedrawWindow();
	
	CString sData;
	GetDlgItem(IDC_STC_147)->GetWindowText(sData);
	pDoc->SetProbingVel(0, _tstof(sData));
	
	pView->m_pVoiceCoil[0]->SetProbFinalData(0);
}

void CDlgUtil06::OnStc150() 
{
	// TODO: Add your control notification handler code here
	myStcData[5].SetBkColor(RGB_RED);
	myStcData[5].RedrawWindow();
	
	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_150)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_150, pt, TO_BOTTOM|TO_RIGHT);
	
	myStcData[5].SetBkColor(RGB_WHITE);
	myStcData[5].RedrawWindow();
	
	CString sData;
	GetDlgItem(IDC_STC_150)->GetWindowText(sData);
	pDoc->SetProbingAcc(0, _tstof(sData));
	
	pView->m_pVoiceCoil[0]->SetProbFinalData(0);
}

void CDlgUtil06::OnStc153() 
{
	// TODO: Add your control notification handler code here
	myStcData[6].SetBkColor(RGB_RED);
	myStcData[6].RedrawWindow();
	
	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_153)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_153, pt, TO_BOTTOM|TO_RIGHT);
	
	myStcData[6].SetBkColor(RGB_WHITE);
	myStcData[6].RedrawWindow();
	
	CString sData;
	GetDlgItem(IDC_STC_153)->GetWindowText(sData);
	pDoc->SetProbingToq(0, _tstof(sData));
	
	pView->m_pVoiceCoil[0]->SetProbFinalData(0);
}




void CDlgUtil06::OnStc93() 
{
	// TODO: Add your control notification handler code here
	myStcData[9].SetBkColor(RGB_RED);
	myStcData[9].RedrawWindow();
	
	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_93)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_93, pt, TO_BOTTOM|TO_RIGHT);
	
	myStcData[9].SetBkColor(RGB_WHITE);
	myStcData[9].RedrawWindow();
	
	CString sData;
	GetDlgItem(IDC_STC_93)->GetWindowText(sData);
	pDoc->SetProbWaitPos(1, _tstof(sData));	
	
	pView->m_pVoiceCoil[1]->SetProbShiftData(1);
}

void CDlgUtil06::OnStc97() 
{
	// TODO: Add your control notification handler code here
	myStcData[10].SetBkColor(RGB_RED);
	myStcData[10].RedrawWindow();
	
	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_97)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_97, pt, TO_BOTTOM|TO_RIGHT);
	
	myStcData[10].SetBkColor(RGB_WHITE);
	myStcData[10].RedrawWindow();
	
	CString sData;
	GetDlgItem(IDC_STC_97)->GetWindowText(sData);
	pDoc->SetProbWaitVel(1, _tstof(sData));	
	
	pView->m_pVoiceCoil[1]->SetProbShiftData(1);
}

void CDlgUtil06::OnStc110() 
{
	// TODO: Add your control notification handler code here
	myStcData[11].SetBkColor(RGB_RED);
	myStcData[11].RedrawWindow();
	
	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_110)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_110, pt, TO_BOTTOM|TO_RIGHT);
	
	myStcData[11].SetBkColor(RGB_WHITE);
	myStcData[11].RedrawWindow();
	
	CString sData;
	GetDlgItem(IDC_STC_110)->GetWindowText(sData);
	pDoc->SetProbWaitAcc(1, _tstof(sData));	
	
	pView->m_pVoiceCoil[1]->SetProbShiftData(1);
}


void CDlgUtil06::OnStc113() 
{
	// TODO: Add your control notification handler code here
	myStcData[12].SetBkColor(RGB_RED);
	myStcData[12].RedrawWindow();
	
	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_113)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_113, pt, TO_BOTTOM|TO_RIGHT);
	
	myStcData[12].SetBkColor(RGB_WHITE);
	myStcData[12].RedrawWindow();
	
	CString sData;
	GetDlgItem(IDC_STC_113)->GetWindowText(sData);
	pDoc->SetProbingPos(1, _tstof(sData));
	
	pView->m_pVoiceCoil[1]->SetProbFinalData(1);
}

void CDlgUtil06::OnStc117() 
{
	// TODO: Add your control notification handler code here
	myStcData[13].SetBkColor(RGB_RED);
	myStcData[13].RedrawWindow();
	
	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_117)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_117, pt, TO_BOTTOM|TO_RIGHT);
	
	myStcData[13].SetBkColor(RGB_WHITE);
	myStcData[13].RedrawWindow();
	
	CString sData;
	GetDlgItem(IDC_STC_117)->GetWindowText(sData);
	pDoc->SetProbingVel(1, _tstof(sData));
	
	pView->m_pVoiceCoil[1]->SetProbFinalData(1);
}

void CDlgUtil06::OnStc120() 
{
	// TODO: Add your control notification handler code here
	myStcData[14].SetBkColor(RGB_RED);
	myStcData[14].RedrawWindow();
	
	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_120)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_120, pt, TO_BOTTOM|TO_RIGHT);
	
	myStcData[14].SetBkColor(RGB_WHITE);
	myStcData[14].RedrawWindow();
	
	CString sData;
	GetDlgItem(IDC_STC_120)->GetWindowText(sData);
	pDoc->SetProbingAcc(1, _tstof(sData));
	
	pView->m_pVoiceCoil[1]->SetProbFinalData(1);
}

void CDlgUtil06::OnStc123() 
{
	// TODO: Add your control notification handler code here
	myStcData[15].SetBkColor(RGB_RED);
	myStcData[15].RedrawWindow();
	
	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_123)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_123, pt, TO_BOTTOM|TO_RIGHT);
	
	myStcData[15].SetBkColor(RGB_WHITE);
	myStcData[15].RedrawWindow();
	
	CString sData;
	GetDlgItem(IDC_STC_123)->GetWindowText(sData);
	pDoc->SetProbingToq(1, _tstof(sData));
	
	pView->m_pVoiceCoil[1]->SetProbFinalData(1);
}

void CDlgUtil06::OnStc161() 
{
	// TODO: Add your control notification handler code here
	myStcData[18].SetBkColor(RGB_RED);
	myStcData[18].RedrawWindow();
	
	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_161)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_161, pt, TO_BOTTOM|TO_RIGHT);
	
	myStcData[18].SetBkColor(RGB_WHITE);
	myStcData[18].RedrawWindow();
	
	CString sData;
	GetDlgItem(IDC_STC_161)->GetWindowText(sData);
	pDoc->SetProbingThreshold(_tstof(sData));	
}

void CDlgUtil06::OnStc165() 
{
	// TODO: Add your control notification handler code here
	myStcData[19].SetBkColor(RGB_RED);
	myStcData[19].RedrawWindow();
	
	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_165)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_165, pt, TO_BOTTOM|TO_RIGHT);
	
	myStcData[19].SetBkColor(RGB_WHITE);
	myStcData[19].RedrawWindow();
	
	CString sData;
	GetDlgItem(IDC_STC_165)->GetWindowText(sData);
	pDoc->SetStripRejectMkNum(_tstoi(sData));		
}

void CDlgUtil06::OnBtnMeasStart() 
{
	// TODO: Add your control notification handler code here
	if(!pDoc->WorkingInfo.Fluck.bUse)
		return;

	//pView->m_pFluck->Clear();
	pView->m_pFluck->Trig();

	SetTimer(TIM_GET, 100, NULL);
}


void CDlgUtil06::OnTimer(UINT_PTR nIDEvent)//(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	double dVal;
	CString str;
	
	if(nIDEvent == TIM_GET)
	{
		KillTimer(TIM_GET);
		if(pDoc->WorkingInfo.Fluck.bUse)
		{
			if(pView->m_pFluck->Get(dVal))
			{	
				if(dVal >= FLUCK_OVER)
				{
					myStcData[20].SetTextColor(RGB_RED);
					str.Format(_T("Open"), dVal);
				}
				else if(dVal == FLUCK_ERROR)
				{
					myStcData[20].SetTextColor(RGB_RED);
					str.Format(_T("Error"), dVal);
				}
				else
				{
					myStcData[20].SetTextColor(RGB_GREEN);
					str.Format(_T("%8f"), dVal);
				}

				myStcData[20].SetText(str);
			}
			else
				SetTimer(TIM_GET, 100, NULL);
		}
	}
	
	CDialog::OnTimer(nIDEvent);
}


BOOL CDlgUtil06::MoveMeasPos(int nId) 
{
	if(!pView->m_pMotion)
		return FALSE;

	if(!pView->m_pLight)
		return FALSE;
	
	if(nId==0)
	{

		if(pView->m_bProbDn[0])
		{
			if(pView->m_pVoiceCoil[0])
			{
				pView->m_pVoiceCoil[0]->SearchHomeSmac(0);
				pView->m_pVoiceCoil[0]->MoveSmacShiftPos(0);
				pView->m_bProbDn[0] = FALSE;
				myBtn[2].SetCheck(FALSE);
			}
		}

		if(pView->m_pDlgMenu02)
			pView->m_pDlgMenu02->SetLight();

		double pPos[2];
		pPos[0] = _tstof(pDoc->WorkingInfo.Probing[nId].sMeasurePosX);
		pPos[1] = _tstof(pDoc->WorkingInfo.Probing[nId].sMeasurePosY);

		if(pPos[1] > 0.0 && pPos[0] > 0.0)
		{
			double dCurrX = pView->m_dEnc[AXIS_X0];
			double dCurrY = pView->m_dEnc[AXIS_Y0];			
			
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
	}
	else if(nId==1)
	{
		if(pView->m_bProbDn[1])
		{
			if(pView->m_pVoiceCoil[1])
			{
				pView->m_pVoiceCoil[1]->SearchHomeSmac(1);
				pView->m_pVoiceCoil[1]->MoveSmacShiftPos(1);
				pView->m_bProbDn[1] = FALSE;
				myBtn[6].SetCheck(FALSE);
			}
		}

		if(pView->m_pDlgMenu02)
			pView->m_pDlgMenu02->SetLight2();
		
		double pPos[2];
		pPos[0] = _tstof(pDoc->WorkingInfo.Probing[nId].sMeasurePosX);
		pPos[1] = _tstof(pDoc->WorkingInfo.Probing[nId].sMeasurePosY);
		
		if(pPos[1] > 0.0 && pPos[0] > 0.0)
		{
			double dCurrX = pView->m_dEnc[AXIS_X1];
			double dCurrY = pView->m_dEnc[AXIS_Y1];			
			
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
				pView->m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X0, fLen, fVel, fAcc, fJerk);
				if(!pView->m_pMotion->Move(MS_X1Y1, pPos, fVel, fAcc, fAcc))
				{
					if(!pView->m_pMotion->Move(MS_X1Y1, pPos, fVel, fAcc, fAcc))
						AfxMessageBox(_T("Move XY Error..."));
				}
			}
			
			return TRUE;
		}
	}
	
	return FALSE;
}

void CDlgUtil06::SaveMeasPos(int nId)
{
	if(IDNO == pView->MsgBox(_T("¼îÆ®°Ë»ç À§Ä¡¸¦ º¯°æÇÏ½Ã°Ú½À´Ï±î?"), 0, MB_YESNO))
	{
		return;
	}
	
	double dX, dY;
	
	CString sData, sPath=PATH_WORKING_INFO;
	
	if(nId==0)
	{
		dX = pView->m_dEnc[AXIS_X0];
		dY = pView->m_dEnc[AXIS_Y0];
		
		sData.Format(_T("%.2f"), dX);
		pDoc->WorkingInfo.Probing[0].sMeasurePosX = sData;
		pDoc->WorkingInfo.Fluck.dMeasPosX[0] = _tstof(sData);
		::WritePrivateProfileString(_T("Probing0"), _T("PROBING_MEASURE_POSX"), sData, sPath);
		myStcData[7].SetText(sData);
		
		sData.Format(_T("%.2f"), dY);
		pDoc->WorkingInfo.Probing[0].sMeasurePosY = sData;
		pDoc->WorkingInfo.Fluck.dMeasPosY[0] = _tstof(sData);
		::WritePrivateProfileString(_T("Probing0"), _T("PROBING_MEASURE_POSY"), sData, sPath);
		myStcData[8].SetText(sData);
	}
	else if(nId==1)
	{
		dX = pView->m_dEnc[AXIS_X1];
		dY = pView->m_dEnc[AXIS_Y1];
		
		sData.Format(_T("%.2f"), dX);
		pDoc->WorkingInfo.Probing[1].sMeasurePosX = sData;
		pDoc->WorkingInfo.Fluck.dMeasPosX[1] = _tstof(sData);
		::WritePrivateProfileString(_T("Probing1"), _T("PROBING_MEASURE_POSX"), sData, sPath);
		myStcData[16].SetText(sData);
		
		sData.Format(_T("%.2f"), dY);
		pDoc->WorkingInfo.Probing[1].sMeasurePosY = sData;
		pDoc->WorkingInfo.Fluck.dMeasPosY[1] = _tstof(sData);
		::WritePrivateProfileString(_T("Probing1"), _T("PROBING_MEASURE_POSY"), sData, sPath);
		myStcData[17].SetText(sData);
	}

}

void CDlgUtil06::OnBtnMeasMove()
{
	// TODO: Add your control notification handler code here
	MoveMeasPos(0);
}

void CDlgUtil06::OnBtnMeasDown() 
{
	// TODO: Add your control notification handler code here
// 	BOOL bOn = ((CButton*)GetDlgItem(IDC_BTN_MEAS_DOWN))->GetCheck();

	BOOL bOn = myBtn[2].GetCheck();
	if(bOn)
	{
		pView->m_bProbDn[0] = TRUE;
		double dMkOffsetX, dMkOffsetY;
		if(pDoc->WorkingInfo.Vision[0].sMkOffsetX.IsEmpty())
			dMkOffsetX = 0.0;
		else
			dMkOffsetX = _tstof(pDoc->WorkingInfo.Vision[0].sMkOffsetX);

		if(pDoc->WorkingInfo.Vision[0].sMkOffsetY.IsEmpty())
			dMkOffsetY = 0.0;
		else
			dMkOffsetY = _tstof(pDoc->WorkingInfo.Vision[0].sMkOffsetY);

		m_dCurrPos[0][0] = pView->m_dEnc[AXIS_X0];
		m_dCurrPos[0][1] = pView->m_dEnc[AXIS_Y0];

		double pPos[2];
		pPos[0] = m_dCurrPos[0][0] + dMkOffsetX;
		pPos[1] = m_dCurrPos[0][1] + dMkOffsetY;

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
			pView->m_pVoiceCoil[0]->SetProbing(0);
	}
	else
	{
		if(pView->m_pVoiceCoil[0])
		{
			pView->m_pVoiceCoil[0]->SearchHomeSmac(0);
			pView->m_pVoiceCoil[0]->MoveSmacShiftPos(0);

			double dMkOffsetX, dMkOffsetY;
			dMkOffsetX = pView->m_dEnc[AXIS_X0] - m_dCurrPos[0][0];
			dMkOffsetY = pView->m_dEnc[AXIS_Y0] - m_dCurrPos[0][1];

			double pPos[2];
			pPos[0] = m_dCurrPos[0][0];
			pPos[1] = m_dCurrPos[0][1];

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

			pView->m_bProbDn[0] = FALSE;

		}
	}
}

void CDlgUtil06::OnBtnMeasSave() 
{
	// TODO: Add your control notification handler code here
	SaveMeasPos(0);
}

void CDlgUtil06::OnBtnMeasMove2() 
{
	// TODO: Add your control notification handler code here
	MoveMeasPos(1);
}

void CDlgUtil06::OnBtnMeasDown2() 
{
	// TODO: Add your control notification handler code here
//	BOOL bOn = ((CButton*)GetDlgItem(IDC_BTN_MEAS_DOWN2))->GetCheck();
	BOOL bOn = myBtn[6].GetCheck();
	if(bOn)
	{
		pView->m_bProbDn[1] = TRUE;

		double dMkOffsetX, dMkOffsetY;
		if(pDoc->WorkingInfo.Vision[1].sMkOffsetX.IsEmpty())
			dMkOffsetX = 0.0;
		else
			dMkOffsetX = _tstof(pDoc->WorkingInfo.Vision[1].sMkOffsetX);

		if(pDoc->WorkingInfo.Vision[1].sMkOffsetY.IsEmpty())
			dMkOffsetY = 0.0;
		else
			dMkOffsetY = _tstof(pDoc->WorkingInfo.Vision[1].sMkOffsetY);
		
		m_dCurrPos[1][0] = pView->m_dEnc[AXIS_X1];
		m_dCurrPos[1][1] = pView->m_dEnc[AXIS_Y1];
		
		double pPos[2];
		pPos[0] = m_dCurrPos[1][0] + dMkOffsetX;
		pPos[1] = m_dCurrPos[1][1] + dMkOffsetY;

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
			pView->m_pVoiceCoil[1]->SetProbing(1);
	}
	else
	{
		if(pView->m_pVoiceCoil[1])
		{
			pView->m_pVoiceCoil[1]->SearchHomeSmac(1);
			pView->m_pVoiceCoil[1]->MoveSmacShiftPos(1);

			double dMkOffsetX, dMkOffsetY;
			dMkOffsetX = pView->m_dEnc[AXIS_X1] - m_dCurrPos[1][0];
			dMkOffsetY = pView->m_dEnc[AXIS_Y1] - m_dCurrPos[1][1];
			
			double pPos[2];
			pPos[0] = m_dCurrPos[1][0];
			pPos[1] = m_dCurrPos[1][1];

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

			pView->m_bProbDn[1] = FALSE;
		}
	}	
}

void CDlgUtil06::OnBtnMeasSave2() 
{
	// TODO: Add your control notification handler code here
	SaveMeasPos(1);	
}

#endif // #ifdef USE_FLUCK
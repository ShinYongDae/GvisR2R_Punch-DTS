// DlgMenu04.cpp : implementation file
//

#include "stdafx.h"
#include "../gvisr2r_punch.h"
#include "DlgMenu04.h"
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
// CDlgMenu04 dialog


CDlgMenu04::CDlgMenu04(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMenu04::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMenu04)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pRect = NULL;
	m_bLoadImg = FALSE;
	m_bTIM_BUF_ENC = FALSE;
}

CDlgMenu04::~CDlgMenu04()
{
	m_bTIM_BUF_ENC = FALSE;
	if(m_pRect)
	{
		delete m_pRect;
		m_pRect = NULL;
	}
}

BOOL CDlgMenu04::Create()
{
	return CDialog::Create(CDlgMenu04::IDD);
}

void CDlgMenu04::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMenu04)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMenu04, CDialog)
	//{{AFX_MSG_MAP(CDlgMenu04)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_STC_004, OnStc004)
	ON_BN_CLICKED(IDC_STC_104, OnStc104)
	ON_BN_CLICKED(IDC_STC_108, OnStc108)
	ON_BN_CLICKED(IDC_STC_007, OnStc007)
	ON_BN_CLICKED(IDC_STC_010, OnStc010)
	ON_BN_CLICKED(IDC_STC_014, OnStc014)
	ON_BN_CLICKED(IDC_STC_22, OnStc22)
	ON_BN_CLICKED(IDC_STC_018, OnStc018)
	ON_BN_CLICKED(IDC_STC_021, OnStc021)
	ON_BN_CLICKED(IDC_STC_024, OnStc024)
	ON_BN_CLICKED(IDC_STC_204, OnStc204)
	ON_BN_CLICKED(IDC_STC_209, OnStc209)
	ON_BN_CLICKED(IDC_STC_25, OnStc25)
	ON_BN_CLICKED(IDC_STC_28, OnStc28)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_BUFF_HOME2, OnBtnBuffHome2)
	ON_BN_CLICKED(IDC_BTN_BUFF_INIT_MOVE2, OnBtnBuffInitMove2)
	ON_BN_CLICKED(IDC_BTN_BUFF_INIT_SAVE2, OnBtnBuffInitSave2)
	ON_BN_CLICKED(IDC_STC_48, OnStc48)
	ON_BN_CLICKED(IDC_STC_52, OnStc52)
	ON_BN_CLICKED(IDC_STC_55, OnStc55)
	ON_BN_CLICKED(IDC_STC_58, OnStc58)
	ON_BN_CLICKED(IDC_STC_62, OnStc62)
	ON_BN_CLICKED(IDC_STC_65, OnStc65)
	ON_BN_CLICKED(IDC_STC_68, OnStc68)
	ON_BN_CLICKED(IDC_STC_71, OnStc71)
	ON_BN_CLICKED(IDC_STC_86, OnStc86)
	ON_BN_CLICKED(IDC_STC_75, OnStc75)
	ON_BN_CLICKED(IDC_STC_87, OnStc87)
	ON_BN_CLICKED(IDC_STC_78, OnStc78)
	ON_BN_CLICKED(IDC_STC_88, OnStc88)
	ON_BN_CLICKED(IDC_STC_81, OnStc81)
	ON_BN_CLICKED(IDC_STC_89, OnStc89)
	ON_BN_CLICKED(IDC_STC_84, OnStc84)
	ON_BN_CLICKED(IDC_STC_93, OnStc93)
	ON_BN_CLICKED(IDC_STC_97, OnStc97)
	ON_BN_CLICKED(IDC_STC_110, OnStc110)
	ON_BN_CLICKED(IDC_STC_113, OnStc113)
	ON_BN_CLICKED(IDC_STC_117, OnStc117)
	ON_BN_CLICKED(IDC_STC_120, OnStc120)
	ON_BN_CLICKED(IDC_STC_123, OnStc123)
	ON_BN_CLICKED(IDC_STC_126, OnStc126)
	ON_BN_CLICKED(IDC_STC_141, OnStc141)
	ON_BN_CLICKED(IDC_STC_130, OnStc130)
	ON_BN_CLICKED(IDC_STC_142, OnStc142)
	ON_BN_CLICKED(IDC_STC_133, OnStc133)
	ON_BN_CLICKED(IDC_STC_143, OnStc143)
	ON_BN_CLICKED(IDC_STC_136, OnStc136)
	ON_BN_CLICKED(IDC_STC_144, OnStc144)
	ON_BN_CLICKED(IDC_STC_139, OnStc139)
	ON_BN_CLICKED(IDC_STC_148, OnStc148)
	ON_BN_CLICKED(IDC_STC_152, OnStc152)
	ON_BN_CLICKED(IDC_STC_158, OnStc158)
	ON_BN_CLICKED(IDC_STC_162, OnStc162)
	ON_BN_CLICKED(IDC_STC_165, OnStc165)
	ON_BN_CLICKED(IDC_STC_170, OnStc170)
	ON_BN_CLICKED(IDC_STC_212, OnStc212)
	ON_BN_CLICKED(IDC_STC_221, OnStc221)
	ON_BN_CLICKED(IDC_STC_222, OnStc222)
	ON_BN_CLICKED(IDC_BTN_RESET_MK_L, OnBtnResetMkL)
	ON_BN_CLICKED(IDC_BTN_RESET_MK_R, OnBtnResetMkR)
	ON_BN_CLICKED(IDC_STC_69, OnStc69)
	ON_BN_CLICKED(IDC_STC_124, OnStc124)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MYBTN_DOWN, OnMyBtnDown)
	ON_MESSAGE(WM_MYBTN_UP, OnMyBtnUp)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMenu04 message handlers

void CDlgMenu04::OnShowWindow(BOOL bShow, UINT nStatus) 
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

void CDlgMenu04::AtDlgShow()
{
	LoadImg();
	Disp();
}

void CDlgMenu04::AtDlgHide()
{
	DelImg();
}

void CDlgMenu04::LoadImg()
{
	if(m_bLoadImg)
		return;
	m_bLoadImg = TRUE;

	int i;
	for(i=0; i<MAX_MENU04_BTN; i++)
	{
		switch(i)
		{
		case 0:
		case 1:
		case 2:
			myBtn[i].LoadBkImage(IMG_CIR_GRN_OFF_DlgMenu04, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_GRN_ON_DlgMenu04, BTN_IMG_DN);
			break;
		case 5:
			myBtn[i].LoadBkImage(IMG_JOG_UP_DlgMenu04, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_JOG_UP_DlgMenu04, BTN_IMG_DN);
			break;
		case 6:
			myBtn[i].LoadBkImage(IMG_JOG_DN_DlgMenu04, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_JOG_DN_DlgMenu04, BTN_IMG_DN);
			break;
		default:
			myBtn[i].LoadBkImage(IMG_BTN_UP_DlgMenu04, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_BTN_DN_DlgMenu04, BTN_IMG_DN);
			break;
		}
	}
}

void CDlgMenu04::DelImg()
{
	if(!m_bLoadImg)
		return;
	m_bLoadImg = FALSE;

	int i;
	for(i=0; i<MAX_MENU04_BTN; i++)
		myBtn[i].DelImgList();
}

BOOL CDlgMenu04::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitStatic();
	InitBtn();
	
	GetDlgItem(IDC_CHK_10)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHK_11)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_00_3)->ShowWindow(SW_HIDE);	
	GetDlgItem(IDC_CHK_0)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_00_4)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHK_1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_00_5)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHK_2)->ShowWindow(SW_HIDE);
	
	m_bTIM_BUF_ENC = TRUE;
	SetTimer(TIM_BUF_ENC_, 100, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMenu04::InitBtn()
{
	myBtn[0].SubclassDlgItem(IDC_CHK_0, this);
	myBtn[0].SetHwnd(this->GetSafeHwnd(), IDC_CHK_0);
	myBtn[0].SetBoarder(FALSE);
	myBtn[0].SetBtnType(BTN_TYPE_CHECK);

	myBtn[1].SubclassDlgItem(IDC_CHK_1, this);
	myBtn[1].SetHwnd(this->GetSafeHwnd(), IDC_CHK_1);
	myBtn[1].SetBoarder(FALSE);
	myBtn[1].SetBtnType(BTN_TYPE_CHECK);

	myBtn[2].SubclassDlgItem(IDC_CHK_2, this);
	myBtn[2].SetHwnd(this->GetSafeHwnd(), IDC_CHK_2);
	myBtn[2].SetBoarder(FALSE);
	myBtn[2].SetBtnType(BTN_TYPE_CHECK);
	
	myBtn[3].SubclassDlgItem(IDC_CHK_10, this);
	myBtn[3].SetHwnd(this->GetSafeHwnd(), IDC_CHK_10);
	myBtn[3].SetBtnType(BTN_TYPE_CHECK);

	myBtn[4].SubclassDlgItem(IDC_CHK_11, this);
	myBtn[4].SetHwnd(this->GetSafeHwnd(), IDC_CHK_11);
	myBtn[4].SetBtnType(BTN_TYPE_CHECK);

	myBtn[5].SubclassDlgItem(IDC_BTN_BUFF_UP2, this);
	myBtn[5].SetHwnd(this->GetSafeHwnd(), IDC_BTN_BUFF_UP2);
	myBtn[5].SetBoarder(FALSE);

	myBtn[6].SubclassDlgItem(IDC_BTN_BUFF_DN2, this);
	myBtn[6].SetHwnd(this->GetSafeHwnd(), IDC_BTN_BUFF_DN2);
	myBtn[6].SetBoarder(FALSE);

	myBtn[7].SubclassDlgItem(IDC_BTN_BUFF_HOME2, this);
	myBtn[7].SetHwnd(this->GetSafeHwnd(), IDC_BTN_BUFF_HOME2);

	myBtn[8].SubclassDlgItem(IDC_BTN_BUFF_INIT_MOVE2, this);
	myBtn[8].SetHwnd(this->GetSafeHwnd(), IDC_BTN_BUFF_INIT_MOVE2);

	myBtn[9].SubclassDlgItem(IDC_BTN_BUFF_INIT_SAVE2, this);
	myBtn[9].SetHwnd(this->GetSafeHwnd(), IDC_BTN_BUFF_INIT_SAVE2);

	myBtn[10].SubclassDlgItem(IDC_BTN_RESET_MK_L, this);
	myBtn[10].SetHwnd(this->GetSafeHwnd(), IDC_BTN_RESET_MK_L);

	myBtn[11].SubclassDlgItem(IDC_BTN_RESET_MK_R, this);
	myBtn[11].SetHwnd(this->GetSafeHwnd(), IDC_BTN_RESET_MK_R);

	int i;
	for(i=0; i<MAX_MENU04_BTN; i++)
	{
		myBtn[i].SetFont(_T("굴림체"),16,TRUE);
		myBtn[i].SetTextColor(RGB_BLACK);
//		myBtn[i].SetBtnType(BTN_TYPE_CHECK);
	}
}

void CDlgMenu04::InitStatic()
{
	InitStcTitle();
	InitStcData();
}

void CDlgMenu04::InitStcTitle()
{
	myStcTitle[0].SubclassDlgItem(IDC_STC_000, this);
	myStcTitle[1].SubclassDlgItem(IDC_STC_001, this);
	myStcTitle[2].SubclassDlgItem(IDC_STC_002, this);
	myStcTitle[3].SubclassDlgItem(IDC_STC_003, this);
	myStcTitle[4].SubclassDlgItem(IDC_STC_005, this);
	myStcTitle[5].SubclassDlgItem(IDC_STC_006, this);
	myStcTitle[6].SubclassDlgItem(IDC_STC_008, this);
	myStcTitle[7].SubclassDlgItem(IDC_STC_009, this);
	myStcTitle[8].SubclassDlgItem(IDC_STC_011, this);
	myStcTitle[9].SubclassDlgItem(IDC_STC_012, this);
	myStcTitle[10].SubclassDlgItem(IDC_STC_013, this);
	myStcTitle[11].SubclassDlgItem(IDC_STC_015, this);
	myStcTitle[12].SubclassDlgItem(IDC_STC_016, this);
	myStcTitle[13].SubclassDlgItem(IDC_STC_017, this);
	myStcTitle[14].SubclassDlgItem(IDC_STC_019, this);
	myStcTitle[15].SubclassDlgItem(IDC_STC_020, this);
	myStcTitle[16].SubclassDlgItem(IDC_STC_022, this);
	myStcTitle[17].SubclassDlgItem(IDC_STC_023, this);
	myStcTitle[18].SubclassDlgItem(IDC_STC_025, this);
	myStcTitle[19].SubclassDlgItem(IDC_STC_100, this);
	myStcTitle[20].SubclassDlgItem(IDC_STC_101, this);
	myStcTitle[21].SubclassDlgItem(IDC_STC_102, this);
	myStcTitle[22].SubclassDlgItem(IDC_STC_103, this);
	myStcTitle[23].SubclassDlgItem(IDC_STC_105, this);
	myStcTitle[24].SubclassDlgItem(IDC_STC_200, this);
	myStcTitle[25].SubclassDlgItem(IDC_STC_201, this);
	myStcTitle[26].SubclassDlgItem(IDC_STC_202, this);
	myStcTitle[27].SubclassDlgItem(IDC_STC_203, this);
	myStcTitle[28].SubclassDlgItem(IDC_STC_205, this);
	myStcTitle[29].SubclassDlgItem(IDC_STC_00_3, this);
	myStcTitle[30].SubclassDlgItem(IDC_STC_00_4, this);
	myStcTitle[31].SubclassDlgItem(IDC_STC_00_5, this);
	myStcTitle[32].SubclassDlgItem(IDC_STC_24, this);
	myStcTitle[33].SubclassDlgItem(IDC_STC_21, this);
	myStcTitle[34].SubclassDlgItem(IDC_STC_23, this);

	myStcTitle[35].SubclassDlgItem(IDC_STC_106, this);
	myStcTitle[36].SubclassDlgItem(IDC_STC_107, this);
	myStcTitle[37].SubclassDlgItem(IDC_STC_109, this);

	myStcTitle[38].SubclassDlgItem(IDC_STC_206, this);
	myStcTitle[39].SubclassDlgItem(IDC_STC_207, this);
	myStcTitle[40].SubclassDlgItem(IDC_STC_208, this);
	myStcTitle[41].SubclassDlgItem(IDC_STC_210, this);

	myStcTitle[42].SubclassDlgItem(IDC_STC_6, this);
	myStcTitle[43].SubclassDlgItem(IDC_STC_26, this);
	myStcTitle[44].SubclassDlgItem(IDC_STC_27, this);
	myStcTitle[45].SubclassDlgItem(IDC_STC_29, this);
	myStcTitle[46].SubclassDlgItem(IDC_STC_38, this);

	myStcTitle[47].SubclassDlgItem(IDC_STC_BUF_ROL2, this);
	myStcTitle[48].SubclassDlgItem(IDC_STC_BUF_HI2, this);

	myStcTitle[49].SubclassDlgItem(IDC_STC_8, this);
	myStcTitle[50].SubclassDlgItem(IDC_STC_9, this);
	myStcTitle[51].SubclassDlgItem(IDC_STC_50, this);
	myStcTitle[52].SubclassDlgItem(IDC_STC_39, this);
	myStcTitle[53].SubclassDlgItem(IDC_STC_49, this);

	myStcTitle[54].SubclassDlgItem(IDC_STC_54, this);
	myStcTitle[55].SubclassDlgItem(IDC_STC_51, this);
	myStcTitle[56].SubclassDlgItem(IDC_STC_53, this);

	myStcTitle[57].SubclassDlgItem(IDC_STC_145, this);
	myStcTitle[58].SubclassDlgItem(IDC_STC_155, this);
	myStcTitle[59].SubclassDlgItem(IDC_STC_90, this);

	myStcTitle[60].SubclassDlgItem(IDC_STC_146, this);
	myStcTitle[61].SubclassDlgItem(IDC_STC_156, this);
	myStcTitle[62].SubclassDlgItem(IDC_STC_91, this);

	myStcTitle[63].SubclassDlgItem(IDC_STC_150, this);
	myStcTitle[64].SubclassDlgItem(IDC_STC_147, this);
	myStcTitle[65].SubclassDlgItem(IDC_STC_154, this);
	myStcTitle[66].SubclassDlgItem(IDC_STC_151, this);
	myStcTitle[67].SubclassDlgItem(IDC_STC_160, this);
	myStcTitle[68].SubclassDlgItem(IDC_STC_157, this);
	myStcTitle[69].SubclassDlgItem(IDC_STC_167, this);
	myStcTitle[70].SubclassDlgItem(IDC_STC_161, this);
	myStcTitle[71].SubclassDlgItem(IDC_STC_168, this);
	myStcTitle[72].SubclassDlgItem(IDC_STC_164, this);
	myStcTitle[73].SubclassDlgItem(IDC_STC_172, this);
	myStcTitle[74].SubclassDlgItem(IDC_STC_169, this);
	myStcTitle[75].SubclassDlgItem(IDC_STC_60, this);
	myStcTitle[76].SubclassDlgItem(IDC_STC_57, this);
	myStcTitle[77].SubclassDlgItem(IDC_STC_61, this);
	myStcTitle[78].SubclassDlgItem(IDC_STC_64, this);
	myStcTitle[79].SubclassDlgItem(IDC_STC_67, this);
	myStcTitle[80].SubclassDlgItem(IDC_STC_73, this);
	myStcTitle[81].SubclassDlgItem(IDC_STC_70, this);
	myStcTitle[82].SubclassDlgItem(IDC_STC_74, this);
	myStcTitle[83].SubclassDlgItem(IDC_STC_77, this);
	myStcTitle[84].SubclassDlgItem(IDC_STC_80, this);
	myStcTitle[85].SubclassDlgItem(IDC_STC_83, this);
	myStcTitle[86].SubclassDlgItem(IDC_STC_95, this);
	myStcTitle[87].SubclassDlgItem(IDC_STC_92, this);
	myStcTitle[88].SubclassDlgItem(IDC_STC_96, this);
	myStcTitle[89].SubclassDlgItem(IDC_STC_99, this);
	myStcTitle[90].SubclassDlgItem(IDC_STC_115, this);
	myStcTitle[91].SubclassDlgItem(IDC_STC_112, this);
	myStcTitle[92].SubclassDlgItem(IDC_STC_116, this);
	myStcTitle[93].SubclassDlgItem(IDC_STC_119, this);
	myStcTitle[94].SubclassDlgItem(IDC_STC_122, this);
	myStcTitle[95].SubclassDlgItem(IDC_STC_128, this);
	myStcTitle[96].SubclassDlgItem(IDC_STC_125, this);
	myStcTitle[97].SubclassDlgItem(IDC_STC_129, this);
	myStcTitle[98].SubclassDlgItem(IDC_STC_132, this);
	myStcTitle[99].SubclassDlgItem(IDC_STC_135, this);
	myStcTitle[100].SubclassDlgItem(IDC_STC_138, this);

	myStcTitle[101].SubclassDlgItem(IDC_STC_149, this);
	myStcTitle[102].SubclassDlgItem(IDC_STC_153, this);
	myStcTitle[103].SubclassDlgItem(IDC_STC_159, this);
	myStcTitle[104].SubclassDlgItem(IDC_STC_163, this);
	myStcTitle[105].SubclassDlgItem(IDC_STC_166, this);
	myStcTitle[106].SubclassDlgItem(IDC_STC_171, this);
	myStcTitle[107].SubclassDlgItem(IDC_STC_56, this);
	myStcTitle[108].SubclassDlgItem(IDC_STC_59, this);
	myStcTitle[109].SubclassDlgItem(IDC_STC_63, this);
	myStcTitle[110].SubclassDlgItem(IDC_STC_66, this);
	myStcTitle[111].SubclassDlgItem(IDC_STC_69, this);
	myStcTitle[112].SubclassDlgItem(IDC_STC_72, this);
	myStcTitle[113].SubclassDlgItem(IDC_STC_76, this);
	myStcTitle[114].SubclassDlgItem(IDC_STC_79, this);
	myStcTitle[115].SubclassDlgItem(IDC_STC_82, this);
	myStcTitle[116].SubclassDlgItem(IDC_STC_85, this);
	myStcTitle[117].SubclassDlgItem(IDC_STC_94, this);
	myStcTitle[118].SubclassDlgItem(IDC_STC_98, this);
	myStcTitle[119].SubclassDlgItem(IDC_STC_111, this);
	myStcTitle[120].SubclassDlgItem(IDC_STC_114, this);
	myStcTitle[121].SubclassDlgItem(IDC_STC_118, this);
	myStcTitle[122].SubclassDlgItem(IDC_STC_121, this);
	myStcTitle[123].SubclassDlgItem(IDC_STC_124, this);
	myStcTitle[124].SubclassDlgItem(IDC_STC_127, this);
	myStcTitle[125].SubclassDlgItem(IDC_STC_131, this);
	myStcTitle[126].SubclassDlgItem(IDC_STC_134, this);
	myStcTitle[127].SubclassDlgItem(IDC_STC_137, this);
	myStcTitle[128].SubclassDlgItem(IDC_STC_140, this);

	myStcTitle[129].SubclassDlgItem(IDC_STC_211, this);
	myStcTitle[130].SubclassDlgItem(IDC_STC_213, this);

	myStcTitle[131].SubclassDlgItem(IDC_STC_179, this);
	myStcTitle[132].SubclassDlgItem(IDC_STC_214, this);
	myStcTitle[133].SubclassDlgItem(IDC_STC_216, this);
	myStcTitle[134].SubclassDlgItem(IDC_STC_219, this);

	for(int i=0; i<MAX_MENU04_STC; i++)
	{
		myStcTitle[i].SetFontName(_T("Arial"));
		myStcTitle[i].SetFontSize(14);
		switch(i)
		{
		case 0:
		case 19:
		case 24: 
		case 38:
		case 49:
		case 57:
		case 58:
		case 59:
		case 131:
			myStcTitle[i].SetTextColor(RGB_NAVY);
			myStcTitle[i].SetBkColor(RGB_LTDKORANGE);
			myStcTitle[i].SetFontBold(TRUE);
			break;
		case 1:
		case 20:
		case 25:
		case 39:
		case 50:
		case 60:
		case 61:
		case 62:
			myStcTitle[i].SetTextColor(RGB_WHITE);
			myStcTitle[i].SetBkColor(RGB_DARKGREEN);
			myStcTitle[i].SetFontBold(TRUE);
			break;
		case 4:
		case 6:
		case 8:
		case 11:
		case 14:
		case 16:
		case 18:
		case 23:
		case 28:
		case 34:
		case 37:
		case 41:
		case 44:
		case 46:
		case 53:
		case 56:
		case 101:
		case 102:
		case 103:
		case 104:
		case 105:
		case 106:
		case 107:
		case 108:
		case 109:
		case 110:
		case 111:
		case 112:
		case 113:
		case 114:
		case 115:
		case 116:
		case 117:
		case 118:
		case 119:
		case 120:
		case 121:
		case 122:
		case 123:
		case 124:
		case 125:
		case 126:
		case 127:
		case 128:
		case 130:
			myStcTitle[i].SetTextColor(RGB_NAVY);
			myStcTitle[i].SetBkColor(RGB_WHITE);
			myStcTitle[i].SetFontBold(FALSE);
			break;
		case 29:
		case 30:
		case 31:
		case 47:
		case 48:
			myStcTitle[i].SetTextColor(RGB_NAVY);
			myStcTitle[i].SetBkColor(RGB_DLG_FRM);
			myStcTitle[i].SetFontBold(TRUE);
			break;
		default:
			myStcTitle[i].SetTextColor(RGB_NAVY);
			myStcTitle[i].SetBkColor(RGB_LTGRAY);
			myStcTitle[i].SetFontBold(TRUE);
			break;
		}
	}
}

void CDlgMenu04::InitStcData()
{
	int i;
	myStcData[0].SubclassDlgItem(IDC_STC_004, this);
	myStcData[1].SubclassDlgItem(IDC_STC_007, this);
	myStcData[2].SubclassDlgItem(IDC_STC_010, this);
	myStcData[3].SubclassDlgItem(IDC_STC_014, this);
	myStcData[4].SubclassDlgItem(IDC_STC_018, this);
	myStcData[5].SubclassDlgItem(IDC_STC_021, this);
	myStcData[6].SubclassDlgItem(IDC_STC_024, this);
	myStcData[7].SubclassDlgItem(IDC_STC_104, this);
	myStcData[8].SubclassDlgItem(IDC_STC_204, this);
	myStcData[9].SubclassDlgItem(IDC_STC_22, this);
	myStcData[10].SubclassDlgItem(IDC_STC_108, this);
	myStcData[11].SubclassDlgItem(IDC_STC_209, this);
	myStcData[12].SubclassDlgItem(IDC_STC_25, this);
	myStcData[13].SubclassDlgItem(IDC_STC_28, this);
	myStcData[14].SubclassDlgItem(IDC_STC_BUF_POS2, this);
	myStcData[15].SubclassDlgItem(IDC_STC_48, this);
	myStcData[16].SubclassDlgItem(IDC_STC_52, this);

	myStcData[17].SubclassDlgItem(IDC_STC_148, this);
	myStcData[18].SubclassDlgItem(IDC_STC_152, this);
	myStcData[19].SubclassDlgItem(IDC_STC_158, this);
	myStcData[20].SubclassDlgItem(IDC_STC_162, this);
	myStcData[21].SubclassDlgItem(IDC_STC_165, this);
	myStcData[22].SubclassDlgItem(IDC_STC_170, this);
	myStcData[23].SubclassDlgItem(IDC_STC_55, this);
	myStcData[24].SubclassDlgItem(IDC_STC_58, this);
	myStcData[25].SubclassDlgItem(IDC_STC_62, this);
	myStcData[26].SubclassDlgItem(IDC_STC_65, this);
	myStcData[27].SubclassDlgItem(IDC_STC_68, this);
	myStcData[28].SubclassDlgItem(IDC_STC_71, this);
	myStcData[29].SubclassDlgItem(IDC_STC_86, this);
	myStcData[30].SubclassDlgItem(IDC_STC_75, this);
	myStcData[31].SubclassDlgItem(IDC_STC_87, this);
	myStcData[32].SubclassDlgItem(IDC_STC_78, this);
	myStcData[33].SubclassDlgItem(IDC_STC_88, this);
	myStcData[34].SubclassDlgItem(IDC_STC_81, this);
	myStcData[35].SubclassDlgItem(IDC_STC_89, this);
	myStcData[36].SubclassDlgItem(IDC_STC_84, this);
	myStcData[37].SubclassDlgItem(IDC_STC_93, this);
	myStcData[38].SubclassDlgItem(IDC_STC_97, this);
	myStcData[39].SubclassDlgItem(IDC_STC_110, this);
	myStcData[40].SubclassDlgItem(IDC_STC_113, this);
	myStcData[41].SubclassDlgItem(IDC_STC_117, this);
	myStcData[42].SubclassDlgItem(IDC_STC_120, this);
	myStcData[43].SubclassDlgItem(IDC_STC_123, this);
	myStcData[44].SubclassDlgItem(IDC_STC_126, this);
	myStcData[45].SubclassDlgItem(IDC_STC_141, this);
	myStcData[46].SubclassDlgItem(IDC_STC_130, this);
	myStcData[47].SubclassDlgItem(IDC_STC_142, this);
	myStcData[48].SubclassDlgItem(IDC_STC_133, this);
	myStcData[49].SubclassDlgItem(IDC_STC_143, this);
	myStcData[50].SubclassDlgItem(IDC_STC_136, this);
	myStcData[51].SubclassDlgItem(IDC_STC_144, this);
	myStcData[52].SubclassDlgItem(IDC_STC_139, this);

	myStcData[53].SubclassDlgItem(IDC_STC_212, this);
	for(i=0; i<54; i++)
	{
		myStcData[i].SetFontName(_T("Arial"));
		myStcData[i].SetFontSize(20);
		myStcData[i].SetFontBold(TRUE);
		myStcData[i].SetTextColor(RGB_BLACK);
		myStcData[i].SetBkColor(RGB_WHITE);
	}

	myStcData[54].SubclassDlgItem(IDC_STC_215, this);
	myStcData[55].SubclassDlgItem(IDC_STC_221, this);
	
	myStcData[56].SubclassDlgItem(IDC_STC_220, this);
	myStcData[57].SubclassDlgItem(IDC_STC_222, this);

	for(i=54; i<MAX_MENU04_STC_DATA; i++)
	{
		myStcData[i].SetFontName(_T("Arial"));
		myStcData[i].SetFontSize(15);
		myStcData[i].SetFontBold(TRUE);
		myStcData[i].SetTextColor(RGB_BLACK);
		myStcData[i].SetBkColor(RGB_WHITE);
	}
}

BOOL CDlgMenu04::ShowKeypad(int nCtlID, CPoint ptSt, int nDir)
{
	BOOL bAdj = TRUE;
	CString strData, strPrev;
	GetDlgItemText(nCtlID, strData);
	strPrev = strData;

	CString strMin, strMax;
	CRect rect(0,0,0,0);
	CDlgKeyNum *pDlg = new CDlgKeyNum(&strData, &rect, ptSt, nDir);
	pDlg->m_strCurr = strData;
	pDlg->m_strMin = strMin = pDoc->GetMin(IDD_DLG_MENU_04, nCtlID);
	pDlg->m_strMax = strMax = pDoc->GetMax(IDD_DLG_MENU_04, nCtlID);
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

void CDlgMenu04::Disp()
{
	CString str;

	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Motion.sMkFdDist));
	myStcData[0].SetText(str);
	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Motion.sMkFdVel));
	myStcData[1].SetText(str);
	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Motion.sMkFdAcc));
	myStcData[2].SetText(str);
	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Motion.sAoiFdLead));
	myStcData[3].SetText(str);
	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Motion.sMkFdLead));
	myStcData[9].SetText(str);
	
	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Motion.sLmtFdErr));
	myStcData[4].SetText(str);
	
	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Motion.sLmtFdAdjOffSet));
	myStcData[5].SetText(str);
	
	str.Format(_T("%d"), _tstoi(pDoc->WorkingInfo.Motion.sLmtFdOvrNum));
	myStcData[6].SetText(str);

	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Motion.sAoiFdVacOff));
	myStcData[7].SetText(str);
	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Motion.sMkFdVacOff));
 	myStcData[10].SetText(str);

// 	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Motion.sMkFdVacOff));
	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Motion.sStBufPos));
	myStcData[8].SetText(str);
// 	myStcData[8].SetText(pDoc->WorkingInfo.Motion.sBufStdPosDist);
	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Motion.sFdMkAoiInitDist));
	myStcData[11].SetText(str);
	str.Format(_T("%d"), _tstoi(pDoc->WorkingInfo.Motion.sFdAoiAoiDistShot));
	myStcData[53].SetText(str);
// 	myStcData[11].SetText(pDoc->WorkingInfo.LastJob.sDistAoiMk);
	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Motion.sMkJogVel));
	myStcData[12].SetText(str);
	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Motion.sMkJogAcc));
	myStcData[13].SetText(str);
	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.LastJob.sFurgeNum));
	myStcData[14].SetText(str);

	// KSH 141011
	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Motion.sAoiTq));
	myStcData[17].SetText(str);
	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Motion.sMkTq));
	myStcData[18].SetText(str);

	// Lot
	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Lot.sTotalReelDist));
	myStcData[19].SetText(str);
	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Lot.sSeparateDist));
	myStcData[20].SetText(str);
	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Lot.sCuttingDist));
	myStcData[21].SetText(str);
	str.Format(_T("%.1f"), _tstof(pDoc->WorkingInfo.Lot.sStopDist));
	myStcData[22].SetText(str);

	// Marking1 Data
	str.Format(_T("%.2f"), _tstof(pDoc->WorkingInfo.Marking[0].sWaitPos));
	myStcData[15].SetText(str);
	str.Format(_T("%.0f"), _tstof(pDoc->WorkingInfo.Marking[0].sWaitVel));
	myStcData[16].SetText(str);
	str.Format(_T("%.0f"), _tstof(pDoc->WorkingInfo.Marking[0].sWaitAcc));
	myStcData[23].SetText(str);

	str.Format(_T("%.2f"), _tstof(pDoc->WorkingInfo.Marking[0].sMarkingPos));
	myStcData[24].SetText(str);
	str.Format(_T("%.0f"), _tstof(pDoc->WorkingInfo.Marking[0].sMarkingVel));
	myStcData[25].SetText(str);
	str.Format(_T("%.0f"), _tstof(pDoc->WorkingInfo.Marking[0].sMarkingAcc));
	myStcData[26].SetText(str);
	str.Format(_T("%.0f"), _tstof(pDoc->WorkingInfo.Marking[0].sMarkingToq));
	myStcData[27].SetText(str);
	str.Format(_T("\r(0~1900)\rg(%.0f)"), _tstof(pDoc->WorkingInfo.Marking[0].sMarkingToqOffset));
	myStcTitle[111].SetText(str);

	str.Format(_T("%.0f"), _tstof(pDoc->WorkingInfo.Marking[0].sMeasurePosX[0]));
	myStcData[28].SetText(str);
	str.Format(_T("%.0f"), _tstof(pDoc->WorkingInfo.Marking[0].sMeasurePosY[0]));
	myStcData[29].SetText(str);
	str.Format(_T("%.0f"), _tstof(pDoc->WorkingInfo.Marking[0].sMeasurePosX[1]));
	myStcData[30].SetText(str);
	str.Format(_T("%.0f"), _tstof(pDoc->WorkingInfo.Marking[0].sMeasurePosY[1]));
	myStcData[31].SetText(str);
	str.Format(_T("%.0f"), _tstof(pDoc->WorkingInfo.Marking[0].sMeasurePosX[2]));
	myStcData[32].SetText(str);
	str.Format(_T("%.0f"), _tstof(pDoc->WorkingInfo.Marking[0].sMeasurePosY[2]));
	myStcData[33].SetText(str);
	str.Format(_T("%.0f"), _tstof(pDoc->WorkingInfo.Marking[0].sMeasurePosX[3]));
	myStcData[34].SetText(str);
	str.Format(_T("%.0f"), _tstof(pDoc->WorkingInfo.Marking[0].sMeasurePosY[3]));
	myStcData[35].SetText(str);

	str.Format(_T("%.2f"), _tstof(pDoc->WorkingInfo.Marking[0].sAverDist));
	myStcData[36].SetText(str);
	
	// Marking2 Data
	str.Format(_T("%.2f"), _tstof(pDoc->WorkingInfo.Marking[1].sWaitPos));
	myStcData[37].SetText(str);
	str.Format(_T("%.0f"), _tstof(pDoc->WorkingInfo.Marking[1].sWaitVel));
	myStcData[38].SetText(str);
	str.Format(_T("%.0f"), _tstof(pDoc->WorkingInfo.Marking[1].sWaitAcc));
	myStcData[39].SetText(str);

	str.Format(_T("%.2f"), _tstof(pDoc->WorkingInfo.Marking[1].sMarkingPos));
	myStcData[40].SetText(str);
	str.Format(_T("%.0f"), _tstof(pDoc->WorkingInfo.Marking[1].sMarkingVel));
	myStcData[41].SetText(str);
	str.Format(_T("%.0f"), _tstof(pDoc->WorkingInfo.Marking[1].sMarkingAcc));
	myStcData[42].SetText(str);
	str.Format(_T("%.0f"), _tstof(pDoc->WorkingInfo.Marking[1].sMarkingToq));
	myStcData[43].SetText(str);
	str.Format(_T("\r(0~1900)\rg(%.0f)"), _tstof(pDoc->WorkingInfo.Marking[1].sMarkingToqOffset));
	myStcTitle[123].SetText(str);

	str.Format(_T("%.0f"), _tstof(pDoc->WorkingInfo.Marking[1].sMeasurePosX[0]));
	myStcData[44].SetText(str);
	str.Format(_T("%.0f"), _tstof(pDoc->WorkingInfo.Marking[1].sMeasurePosY[0]));
	myStcData[45].SetText(str);
	str.Format(_T("%.0f"), _tstof(pDoc->WorkingInfo.Marking[1].sMeasurePosX[1]));
	myStcData[46].SetText(str);
	str.Format(_T("%.0f"), _tstof(pDoc->WorkingInfo.Marking[1].sMeasurePosY[1]));
	myStcData[47].SetText(str);
	str.Format(_T("%.0f"), _tstof(pDoc->WorkingInfo.Marking[1].sMeasurePosX[2]));
	myStcData[48].SetText(str);
	str.Format(_T("%.0f"), _tstof(pDoc->WorkingInfo.Marking[1].sMeasurePosY[2]));
	myStcData[49].SetText(str);
	str.Format(_T("%.0f"), _tstof(pDoc->WorkingInfo.Marking[1].sMeasurePosX[3]));
	myStcData[50].SetText(str);
	str.Format(_T("%.0f"), _tstof(pDoc->WorkingInfo.Marking[1].sMeasurePosY[3]));
	myStcData[51].SetText(str);

	str.Format(_T("%.2f"), _tstof(pDoc->WorkingInfo.Marking[1].sAverDist));
	myStcData[52].SetText(str);

	str.Format(_T("%d"), pDoc->GetMkCntL());
	myStcData[54].SetText(str);
	str.Format(_T("%d"), pDoc->GetMkLimitL());
	myStcData[55].SetText(str);

	str.Format(_T("%d"), pDoc->GetMkCntR());
	myStcData[56].SetText(str);
	str.Format(_T("%d"), pDoc->GetMkLimitR());
	myStcData[57].SetText(str);

}

void CDlgMenu04::OnStc004() 
{
	// TODO: Add your control notification handler code here
	myStcData[0].SetBkColor(RGB_RED);
	myStcData[0].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_004)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_004, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[0].SetBkColor(RGB_WHITE);
	myStcData[0].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_004)->GetWindowText(sData);
	pDoc->SetOnePnlLen(_tstof(sData));
	pView->SetLotLastShot();
}

void CDlgMenu04::OnStc007() 
{
	// TODO: Add your control notification handler code here
	myStcData[1].SetBkColor(RGB_RED);
	myStcData[1].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_007)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_007, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[1].SetBkColor(RGB_WHITE);
	myStcData[1].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_007)->GetWindowText(sData);
	pDoc->SetOnePnlVel(_tstof(sData));
}

void CDlgMenu04::OnStc010() 
{
	// TODO: Add your control notification handler code here
	myStcData[2].SetBkColor(RGB_RED);
	myStcData[2].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_010)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_010, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[2].SetBkColor(RGB_WHITE);
	myStcData[2].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_010)->GetWindowText(sData);
	pDoc->SetOnePnlAcc(_tstof(sData));
}

void CDlgMenu04::OnStc014() 
{
	// TODO: Add your control notification handler code here
	myStcData[3].SetBkColor(RGB_RED);
	myStcData[3].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_014)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_014, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[3].SetBkColor(RGB_WHITE);
	myStcData[3].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_014)->GetWindowText(sData);
	pView->SetAoiFdPitch(_tstof(sData));	
}

void CDlgMenu04::OnStc22() 
{
	// TODO: Add your control notification handler code here
	myStcData[9].SetBkColor(RGB_RED);
	myStcData[9].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_22)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_22, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[9].SetBkColor(RGB_WHITE);
	myStcData[9].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_22)->GetWindowText(sData);
	pView->SetMkFdPitch(_tstof(sData));	
}

void CDlgMenu04::OnStc018() 
{
	// TODO: Add your control notification handler code here
	myStcData[4].SetBkColor(RGB_RED);
	myStcData[4].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_018)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_018, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[4].SetBkColor(RGB_WHITE);
	myStcData[4].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_018)->GetWindowText(sData);
	pDoc->SetFdErrLmt(_tstof(sData));
}

void CDlgMenu04::OnStc021() 
{
	// TODO: Add your control notification handler code here
	myStcData[5].SetBkColor(RGB_RED);
	myStcData[5].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_021)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_021, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[5].SetBkColor(RGB_WHITE);
	myStcData[5].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_021)->GetWindowText(sData);
	pDoc->SetFdErrRng(_tstof(sData));
}

void CDlgMenu04::OnStc024() 
{
	// TODO: Add your control notification handler code here
	myStcData[6].SetBkColor(RGB_RED);
	myStcData[6].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_024)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_024, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[6].SetBkColor(RGB_WHITE);
	myStcData[6].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_024)->GetWindowText(sData);
	pDoc->SetFdErrNum(_tstoi(sData));
}

void CDlgMenu04::OnStc104() 
{
	// TODO: Add your control notification handler code here
	myStcData[7].SetBkColor(RGB_RED);
	myStcData[7].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_104)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_104, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[7].SetBkColor(RGB_WHITE);
	myStcData[7].RedrawWindow();

	CString sPath=PATH_WORKING_INFO;
	CString sData;
	GetDlgItem(IDC_STC_104)->GetWindowText(sData);
	pDoc->WorkingInfo.Motion.sAoiFdVacOff = sData;
	::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_VACUUM_OFF"), sData, sPath);

// 	pDoc->WorkingInfo.Motion.sAoiTq = sData;
// 	::WritePrivateProfileString(_T("Motion"), _T("AOI_TENSION_SERVO_TORQUE"), sData, sPath);
// 	
// 	// 정격토크 : 1.27 N*m = 127 N*cm = 12.95 Kgf*cm = 10922 (32767/3) pulse
// 	// 843.0 [pulse/Kgf]
// 	if(pView->m_pMotion)
// 		pView->m_pMotion->SetTorque(AXIS_AOITQ, _tstof(pDoc->WorkingInfo.Motion.sAoiTq));
}

void CDlgMenu04::OnStc108() 
{
	// TODO: Add your control notification handler code here
	myStcData[10].SetBkColor(RGB_RED);
	myStcData[10].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_108)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_108, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[10].SetBkColor(RGB_WHITE);
	myStcData[10].RedrawWindow();

	CString sPath=PATH_WORKING_INFO;
	CString sData;
	GetDlgItem(IDC_STC_108)->GetWindowText(sData);
	pDoc->WorkingInfo.Motion.sMkFdVacOff = sData;
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_VACUUM_OFF"), sData, sPath);

// 	pDoc->WorkingInfo.Motion.sMkTq = sData;
// 	::WritePrivateProfileString(_T("Motion"), _T("MARKING_TENSION_SERVO_TORQUE"), sData, sPath);
// 	
// 	// 정격토크 : 1.27 N*m = 127 N*cm = 12.95 Kgf*cm = 10922 (32767/3) pulse
// 	// 843.0 [pulse/Kgf]
// 	if(pView->m_pMotion)
// 		pView->m_pMotion->SetTorque(AXIS_MKTQ, _tstof(pDoc->WorkingInfo.Motion.sMkTq));
}

void CDlgMenu04::OnStc204() 
{
	// TODO: Add your control notification handler code here
	myStcData[8].SetBkColor(RGB_RED);
	myStcData[8].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_204)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_204, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[8].SetBkColor(RGB_WHITE);
	myStcData[8].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_204)->GetWindowText(sData);
	pDoc->SetBufInitPos(_tstoi(sData));
}

void CDlgMenu04::OnStc209() 
{
	// TODO: Add your control notification handler code here
	myStcData[11].SetBkColor(RGB_RED);
	myStcData[11].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_209)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_209, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[11].SetBkColor(RGB_WHITE);
	myStcData[11].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_209)->GetWindowText(sData);
	pDoc->SetAoiMkDist(_tstof(sData));
}

void CDlgMenu04::OnStc25() 
{
	// TODO: Add your control notification handler code here
	myStcData[12].SetBkColor(RGB_RED);
	myStcData[12].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_25)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_25, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[12].SetBkColor(RGB_WHITE);
	myStcData[12].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_25)->GetWindowText(sData);
	pDoc->SetFdJogVel(_tstof(sData));
}

void CDlgMenu04::OnStc28() 
{
	// TODO: Add your control notification handler code here
	myStcData[13].SetBkColor(RGB_RED);
	myStcData[13].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_28)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_28, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[13].SetBkColor(RGB_WHITE);
	myStcData[13].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_28)->GetWindowText(sData);
	pDoc->SetFdJogAcc(_tstof(sData));
}

BOOL CDlgMenu04::PreTranslateMessage(MSG* pMsg) 
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

void CDlgMenu04::DispBufEnc()
{
	if (!pDoc->m_pMpeData)
		return;

	CString str;
	double dBufEnc = (double)pDoc->m_pMpeData[0][1]	/ 1000.0;	// 마킹부 버퍼 엔코더 값(단위 mm * 1000)
	str.Format(_T("%.1f"), dBufEnc);
	myStcData[14].SetText(str);
}

void CDlgMenu04::OnTimer(UINT_PTR nIDEvent)//(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == TIM_BUF_ENC_)
	{
		KillTimer(TIM_BUF_ENC_);
//		DispBufEnc();
// 		if(m_bTIM_BUF_ENC)
// 			SetTimer(TIM_BUF_ENC_, 100, NULL);
	}
	
	CDialog::OnTimer(nIDEvent);
}

LRESULT CDlgMenu04::OnMyBtnDown(WPARAM wPara, LPARAM lPara)
{
	int nCtrlID = (int)lPara;
	SwMyBtnDown(nCtrlID);

	return 0L;
}

void CDlgMenu04::SwMyBtnDown(int nCtrlID)
{

	if(pView->m_bProbDn[0])
	{
		if(pView->m_pVoiceCoil[0])
		{
			pView->m_pVoiceCoil[0]->SearchHomeSmac(0);
			pView->m_pVoiceCoil[0]->MoveSmacShiftPos(0);
			pView->m_bProbDn[0] = FALSE;
			//if(pView->m_pDlgMenu02->m_pDlgUtil06)
			//	pView->m_pDlgMenu02->m_pDlgUtil06->myBtn[2].SetCheck(FALSE);
		}
	}

	if(pView->m_bProbDn[1])
	{
		if(pView->m_pVoiceCoil[1])
		{
			pView->m_pVoiceCoil[1]->SearchHomeSmac(1);
			pView->m_pVoiceCoil[1]->MoveSmacShiftPos(1);
			pView->m_bProbDn[1] = FALSE;
			//if(pView->m_pDlgMenu02->m_pDlgUtil06)
			//	pView->m_pDlgMenu02->m_pDlgUtil06->myBtn[6].SetCheck(FALSE);
		}
	}

	switch(nCtrlID)
	{
	case IDC_BTN_BUFF_UP2:
#ifdef USE_MPE
		if(pView->m_pMpe)
			pView->m_pMpe->Write(_T("MB005513"), 1);		// 마킹부 피딩 정회전 스위치
#endif
// 		if(pView->m_pMotion)
// 			pView->m_pMotion->VMove(MS_MKFD, M_CW);
		break;
	case IDC_BTN_BUFF_DN2:
#ifdef USE_MPE
		if(pView->m_pMpe)
			pView->m_pMpe->Write(_T("MB005514"), 1);		// 마킹부 피딩 역회전 스위치
#endif
// 		if(pView->m_pMotion)
// 			pView->m_pMotion->VMove(MS_MKFD, M_CCW);
		break;
	}
}

LRESULT CDlgMenu04::OnMyBtnUp(WPARAM wPara, LPARAM lPara)
{
	int nCtrlID = (int)lPara;
	SwMyBtnUp(nCtrlID);

	return 0L;
}

void CDlgMenu04::SwMyBtnUp(int nCtrlID)
{
	switch(nCtrlID)
	{
	case IDC_BTN_BUFF_UP2:
#ifdef USE_MPE
		if(pView->m_pMpe)
			pView->m_pMpe->Write(_T("MB005513"), 0);		// 마킹부 피딩 정회전 스위치
#endif
		break;
	case IDC_BTN_BUFF_DN2:
#ifdef USE_MPE
		if(pView->m_pMpe)
			pView->m_pMpe->Write(_T("MB005514"), 0);		// 마킹부 피딩 역회전 스위치
#endif
		break;
	}
}
	
void CDlgMenu04::ResetMotion(int nMsId)
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


void CDlgMenu04::OnBtnBuffHome2() 
{
	// TODO: Add your control notification handler code here
	if(pDoc->WorkingInfo.Motion.bBufHomming)
	{
		pView->DispMsg(_T("Homming"),_T("Searching Buffer Home Position..."),RGB_GREEN,2000,TRUE);
#ifdef USE_MPE
		if(pView->m_pMpe)
			pView->m_pMpe->Write(_T("MB440152"), 1);	// 마킹부 버퍼롤러 홈동작 ON (PLC가 홈동작 완료 후 OFF)
#endif
		if(pView->m_pDlgMenu03)
			pView->m_pDlgMenu03->ChkBufHomeDone();
	}	
}

void CDlgMenu04::OnBtnBuffInitMove2() 
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
// 	double dCurPosMkFd = (double)pDoc->m_pMpeData[0][0];		// 마킹부 Feeding 엔코더 값(단위 mm )
// 	double dCurPosBuf = (double)pDoc->m_pMpeData[0][1] / 1000.0;		// 마킹부 버퍼 엔코더 값(단위 mm * 1000)
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
// // 		double dPos = dCurPosMkFd + dDir*fLen;
// // 		pView->m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_MKFD, fLen, fVel, fAcc, fJerk);
// // 		pView->m_pMotion->Move(MS_MKFD, dPos, fVel/10.0, fAcc/10.0, fAcc/10.0);
// 		fVel = pView->m_pMotion->m_pParamMotion[MS_MKFD].Home.f1stSpd;
// 		fAcc = pView->m_pMotion->m_pParamMotion[MS_MKFD].Home.fAcc;
// 		if(!pView->m_pMotion->Move(MS_MKFD, dDir*fLen, fVel, fAcc, fAcc, INC, WAIT))
// 			if(!pView->m_pMotion->Move(MS_MKFD, dDir*fLen, fVel, fAcc, fAcc, INC, WAIT))
// 				AfxMessageBox("Move XY Error...");
// 	}	
}

void CDlgMenu04::OnBtnBuffInitSave2() 
{
	// TODO: Add your control notification handler code here
	if(!pView->m_pMotion)
		return;

//	if(IDNO == pView->DoMyMsgBox(_T("Do you want to save Buffer Position?"), MB_YESNO))
	if(IDNO == pView->MsgBox(_T("Do you want to save Buffer Position?"), 0, MB_YESNO))
		return;

#ifdef USE_MPE
	double dBufEnc = (double)pDoc->m_pMpeData[0][1]	/ 1000.0;	// 마킹부 버퍼 엔코더 값(단위 mm * 1000)
	pView->SetBufInitPos(dBufEnc);		// ML45016	,	버퍼 관련 설정 롤러 초기위치(단위 mm * 1000)
#endif
}

void CDlgMenu04::OnStc48() 
{
	// TODO: Add your control notification handler code here
	myStcData[15].SetBkColor(RGB_RED);
	myStcData[15].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_48)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_48, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[15].SetBkColor(RGB_WHITE);
	myStcData[15].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_48)->GetWindowText(sData);
	pDoc->SetWaitPos1(_tstof(sData));

	pView->m_pVoiceCoil[0]->SetMarkShiftData(0);
}

void CDlgMenu04::OnStc52() 
{
	// TODO: Add your control notification handler code here
	myStcData[16].SetBkColor(RGB_RED);
	myStcData[16].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_52)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_52, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[16].SetBkColor(RGB_WHITE);
	myStcData[16].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_52)->GetWindowText(sData);
	pDoc->SetWaitVel1(_tstof(sData));

	pView->m_pVoiceCoil[0]->SetMarkShiftData(0);
}

void CDlgMenu04::OnStc55() 
{
	// TODO: Add your control notification handler code here
	myStcData[23].SetBkColor(RGB_RED);
	myStcData[23].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_55)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_55, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[23].SetBkColor(RGB_WHITE);
	myStcData[23].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_55)->GetWindowText(sData);
	pDoc->SetWaitAcc1(_tstof(sData));	

	pView->m_pVoiceCoil[0]->SetMarkShiftData(0);
}

void CDlgMenu04::OnStc58() 
{
	// TODO: Add your control notification handler code here
	myStcData[24].SetBkColor(RGB_RED);
	myStcData[24].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_58)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_58, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[24].SetBkColor(RGB_WHITE);
	myStcData[24].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_58)->GetWindowText(sData);
	pDoc->SetMarkingPos1(_tstof(sData));	
	
	pView->m_pVoiceCoil[0]->SetMarkFinalData(0);
}

void CDlgMenu04::OnStc62() 
{
	// TODO: Add your control notification handler code here
	myStcData[25].SetBkColor(RGB_RED);
	myStcData[25].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_62)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_62, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[25].SetBkColor(RGB_WHITE);
	myStcData[25].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_62)->GetWindowText(sData);
	pDoc->SetMarkingVel1(_tstof(sData));

	pView->m_pVoiceCoil[0]->SetMarkFinalData(0);
}

void CDlgMenu04::OnStc65() 
{
	// TODO: Add your control notification handler code here
	myStcData[26].SetBkColor(RGB_RED);
	myStcData[26].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_65)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_65, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[26].SetBkColor(RGB_WHITE);
	myStcData[26].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_65)->GetWindowText(sData);
	pDoc->SetMarkingAcc1(_tstof(sData));
	
	pView->m_pVoiceCoil[0]->SetMarkFinalData(0);
}

void CDlgMenu04::OnStc68() 
{
	// TODO: Add your control notification handler code here
	myStcData[27].SetBkColor(RGB_RED);
	myStcData[27].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_68)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_68, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[27].SetBkColor(RGB_WHITE);
	myStcData[27].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_68)->GetWindowText(sData);
	pDoc->SetMarkingToq1(_tstof(sData));

	pView->m_pVoiceCoil[0]->SetMarkFinalData(0);
}

void CDlgMenu04::OnStc71() 
{
	// TODO: Add your control notification handler code here
	myStcData[28].SetBkColor(RGB_RED);
	myStcData[28].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_71)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_71, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[28].SetBkColor(RGB_WHITE);
	myStcData[28].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_71)->GetWindowText(sData);
	pDoc->SetPosX1_1(_tstof(sData));	
}

void CDlgMenu04::OnStc86() 
{
	// TODO: Add your control notification handler code here
	myStcData[29].SetBkColor(RGB_RED);
	myStcData[29].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_86)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_86, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[29].SetBkColor(RGB_WHITE);
	myStcData[29].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_86)->GetWindowText(sData);
	pDoc->SetPosY1_1(_tstof(sData));	
}

void CDlgMenu04::OnStc75() 
{
	// TODO: Add your control notification handler code here
	myStcData[30].SetBkColor(RGB_RED);
	myStcData[30].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_75)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_75, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[30].SetBkColor(RGB_WHITE);
	myStcData[30].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_75)->GetWindowText(sData);
	pDoc->SetPosX1_2(_tstof(sData));		
}

void CDlgMenu04::OnStc87() 
{
	// TODO: Add your control notification handler code here
	myStcData[31].SetBkColor(RGB_RED);
	myStcData[31].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_87)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_87, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[31].SetBkColor(RGB_WHITE);
	myStcData[31].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_87)->GetWindowText(sData);
	pDoc->SetPosY1_2(_tstof(sData));		
}

void CDlgMenu04::OnStc78() 
{
	// TODO: Add your control notification handler code here
	myStcData[32].SetBkColor(RGB_RED);
	myStcData[32].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_78)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_78, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[32].SetBkColor(RGB_WHITE);
	myStcData[32].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_78)->GetWindowText(sData);
	pDoc->SetPosX1_3(_tstof(sData));		
}

void CDlgMenu04::OnStc88() 
{
	// TODO: Add your control notification handler code here
	myStcData[33].SetBkColor(RGB_RED);
	myStcData[33].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_88)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_88, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[33].SetBkColor(RGB_WHITE);
	myStcData[33].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_88)->GetWindowText(sData);
	pDoc->SetPosY1_3(_tstof(sData));		
}

void CDlgMenu04::OnStc81() 
{
	// TODO: Add your control notification handler code here
	myStcData[34].SetBkColor(RGB_RED);
	myStcData[34].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_81)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_81, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[34].SetBkColor(RGB_WHITE);
	myStcData[34].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_81)->GetWindowText(sData);
	pDoc->SetPosX1_4(_tstof(sData));		
}

void CDlgMenu04::OnStc89() 
{
	// TODO: Add your control notification handler code here
	myStcData[35].SetBkColor(RGB_RED);
	myStcData[35].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_89)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_89, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[35].SetBkColor(RGB_WHITE);
	myStcData[35].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_89)->GetWindowText(sData);
	pDoc->SetPosY1_4(_tstof(sData));			
}

void CDlgMenu04::OnStc84() 
{
	// TODO: Add your control notification handler code here
	myStcData[36].SetBkColor(RGB_RED);
	myStcData[36].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_84)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_84, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[36].SetBkColor(RGB_WHITE);
	myStcData[36].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_84)->GetWindowText(sData);
 	pDoc->SetAverDist1(_tstoi(sData));		
}

void CDlgMenu04::OnStc93() 
{
	// TODO: Add your control notification handler code here
	myStcData[37].SetBkColor(RGB_RED);
	myStcData[37].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_93)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_93, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[37].SetBkColor(RGB_WHITE);
	myStcData[37].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_93)->GetWindowText(sData);
	pDoc->SetWaitPos2(_tstof(sData));	
	
	pView->m_pVoiceCoil[1]->SetMarkShiftData(1);
}

void CDlgMenu04::OnStc97() 
{
	// TODO: Add your control notification handler code here
	myStcData[38].SetBkColor(RGB_RED);
	myStcData[38].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_97)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_97, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[38].SetBkColor(RGB_WHITE);
	myStcData[38].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_97)->GetWindowText(sData);
	pDoc->SetWaitVel2(_tstof(sData));	

	pView->m_pVoiceCoil[1]->SetMarkShiftData(1);
}

void CDlgMenu04::OnStc110() 
{
	// TODO: Add your control notification handler code here
	myStcData[39].SetBkColor(RGB_RED);
	myStcData[39].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_110)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_110, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[39].SetBkColor(RGB_WHITE);
	myStcData[39].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_110)->GetWindowText(sData);
	pDoc->SetWaitAcc2(_tstof(sData));	
	
	pView->m_pVoiceCoil[1]->SetMarkShiftData(1);
}

void CDlgMenu04::OnStc113() 
{
	// TODO: Add your control notification handler code here
	myStcData[40].SetBkColor(RGB_RED);
	myStcData[40].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_113)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_113, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[40].SetBkColor(RGB_WHITE);
	myStcData[40].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_113)->GetWindowText(sData);
	pDoc->SetMarkingPos2(_tstof(sData));
	
	pView->m_pVoiceCoil[1]->SetMarkFinalData(1);
}

void CDlgMenu04::OnStc117() 
{
	// TODO: Add your control notification handler code here
	myStcData[41].SetBkColor(RGB_RED);
	myStcData[41].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_117)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_117, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[41].SetBkColor(RGB_WHITE);
	myStcData[41].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_117)->GetWindowText(sData);
	pDoc->SetMarkingVel2(_tstof(sData));
	
	pView->m_pVoiceCoil[1]->SetMarkFinalData(1);
}

void CDlgMenu04::OnStc120() 
{
	// TODO: Add your control notification handler code here
	myStcData[42].SetBkColor(RGB_RED);
	myStcData[42].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_120)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_120, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[42].SetBkColor(RGB_WHITE);
	myStcData[42].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_120)->GetWindowText(sData);
	pDoc->SetMarkingAcc2(_tstof(sData));
	
	pView->m_pVoiceCoil[1]->SetMarkFinalData(1);
}

void CDlgMenu04::OnStc123() 
{
	// TODO: Add your control notification handler code here
	myStcData[43].SetBkColor(RGB_RED);
	myStcData[43].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_123)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_123, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[43].SetBkColor(RGB_WHITE);
	myStcData[43].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_123)->GetWindowText(sData);
	pDoc->SetMarkingToq2(_tstof(sData));
	
	pView->m_pVoiceCoil[1]->SetMarkFinalData(1);
}

void CDlgMenu04::OnStc126() 
{
	// TODO: Add your control notification handler code here
	myStcData[44].SetBkColor(RGB_RED);
	myStcData[44].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_126)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_126, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[44].SetBkColor(RGB_WHITE);
	myStcData[44].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_126)->GetWindowText(sData);
	pDoc->SetPosX2_1(_tstof(sData));	
}

void CDlgMenu04::OnStc141() 
{
	// TODO: Add your control notification handler code here
	myStcData[45].SetBkColor(RGB_RED);
	myStcData[45].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_141)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_141, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[45].SetBkColor(RGB_WHITE);
	myStcData[45].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_141)->GetWindowText(sData);
	pDoc->SetPosY2_1(_tstof(sData));		
}

void CDlgMenu04::OnStc130() 
{
	// TODO: Add your control notification handler code here
	myStcData[46].SetBkColor(RGB_RED);
	myStcData[46].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_130)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_130, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[46].SetBkColor(RGB_WHITE);
	myStcData[46].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_130)->GetWindowText(sData);
	pDoc->SetPosX2_2(_tstof(sData));	
}

void CDlgMenu04::OnStc142() 
{
	// TODO: Add your control notification handler code here
	myStcData[47].SetBkColor(RGB_RED);
	myStcData[47].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_142)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_142, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[47].SetBkColor(RGB_WHITE);
	myStcData[47].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_142)->GetWindowText(sData);
	pDoc->SetPosY2_2(_tstof(sData));	
}

void CDlgMenu04::OnStc133() 
{
	// TODO: Add your control notification handler code here
	myStcData[48].SetBkColor(RGB_RED);
	myStcData[48].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_133)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_133, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[48].SetBkColor(RGB_WHITE);
	myStcData[48].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_133)->GetWindowText(sData);
	pDoc->SetPosX2_3(_tstof(sData));		
}

void CDlgMenu04::OnStc143() 
{
	// TODO: Add your control notification handler code here
	myStcData[49].SetBkColor(RGB_RED);
	myStcData[49].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_143)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_143, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[49].SetBkColor(RGB_WHITE);
	myStcData[49].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_143)->GetWindowText(sData);
	pDoc->SetPosY2_3(_tstof(sData));	
}

void CDlgMenu04::OnStc136() 
{
	// TODO: Add your control notification handler code here
	myStcData[50].SetBkColor(RGB_RED);
	myStcData[50].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_136)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_136, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[50].SetBkColor(RGB_WHITE);
	myStcData[50].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_136)->GetWindowText(sData);
	pDoc->SetPosX2_4(_tstof(sData));		
}

void CDlgMenu04::OnStc144() 
{
	// TODO: Add your control notification handler code here
	myStcData[51].SetBkColor(RGB_RED);
	myStcData[51].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_144)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_144, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[51].SetBkColor(RGB_WHITE);
	myStcData[51].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_144)->GetWindowText(sData);
	pDoc->SetPosY2_4(_tstof(sData));	
}

void CDlgMenu04::OnStc139() 
{
	// TODO: Add your control notification handler code here
	myStcData[52].SetBkColor(RGB_RED);
	myStcData[52].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_139)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_139, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[52].SetBkColor(RGB_WHITE);
	myStcData[52].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_139)->GetWindowText(sData);
 	pDoc->SetAverDist2(_tstoi(sData));		
}

void CDlgMenu04::OnStc148() 
{
	// TODO: Add your control notification handler code here
	myStcData[17].SetBkColor(RGB_RED);
	myStcData[17].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_148)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_148, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[17].SetBkColor(RGB_WHITE);
	myStcData[17].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_148)->GetWindowText(sData);
 	pDoc->SetAOIToq(_tstoi(sData));		
}

void CDlgMenu04::OnStc152() 
{
	// TODO: Add your control notification handler code here
	myStcData[18].SetBkColor(RGB_RED);
	myStcData[18].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_152)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_152, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[18].SetBkColor(RGB_WHITE);
	myStcData[18].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_152)->GetWindowText(sData);
 	pDoc->SetMarkingToq(_tstoi(sData));	
}

void CDlgMenu04::OnStc158() 
{
	// TODO: Add your control notification handler code here
	myStcData[19].SetBkColor(RGB_RED);
	myStcData[19].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_158)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_158, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[19].SetBkColor(RGB_WHITE);
	myStcData[19].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_158)->GetWindowText(sData);
 	pDoc->SetTotalReelDist(_tstoi(sData));		
}

void CDlgMenu04::OnStc162() 
{
	// TODO: Add your control notification handler code here
	myStcData[20].SetBkColor(RGB_RED);
	myStcData[20].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_162)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_162, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[20].SetBkColor(RGB_WHITE);
	myStcData[20].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_162)->GetWindowText(sData);
 	pDoc->SetSeparateDist(_tstoi(sData));	
	pView->SetLotLastShot();
}

void CDlgMenu04::OnStc165() 
{
	// TODO: Add your control notification handler code here
	myStcData[21].SetBkColor(RGB_RED);
	myStcData[21].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_165)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_165, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[21].SetBkColor(RGB_WHITE);
	myStcData[21].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_165)->GetWindowText(sData);
 	pDoc->SetCuttingDist(_tstoi(sData));		
}

void CDlgMenu04::OnStc170() 
{
	// TODO: Add your control notification handler code here
	myStcData[22].SetBkColor(RGB_RED);
	myStcData[22].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_170)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_170, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[22].SetBkColor(RGB_WHITE);
	myStcData[22].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_170)->GetWindowText(sData);
 	pDoc->SetStopDist(_tstoi(sData));	
}

void CDlgMenu04::OnStc212() 
{
	// TODO: Add your control notification handler code here
	myStcData[53].SetBkColor(RGB_RED);
	myStcData[53].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_212)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_212, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[53].SetBkColor(RGB_WHITE);
	myStcData[53].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_212)->GetWindowText(sData);
	pDoc->SetAoiAoiDist(_tstoi(sData));
}

void CDlgMenu04::OnStc221() 
{
	// TODO: Add your control notification handler code here
	myStcData[55].SetBkColor(RGB_RED);
	myStcData[55].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_221)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_221, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[55].SetBkColor(RGB_WHITE);
	myStcData[55].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_221)->GetWindowText(sData);
	pDoc->SetMkLimitL(_tstoi(sData));	
}

void CDlgMenu04::OnStc222() 
{
	// TODO: Add your control notification handler code here
	myStcData[57].SetBkColor(RGB_RED);
	myStcData[57].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_222)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_222, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[57].SetBkColor(RGB_WHITE);
	myStcData[57].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_222)->GetWindowText(sData);
	pDoc->SetMkLimitR(_tstoi(sData));		
}

void CDlgMenu04::OnBtnResetMkL() 
{
	// TODO: Add your control notification handler code here
//	if(IDYES == pView->DoMyMsgBox(_T("마킹(좌)의 카운트를 리세하시겠습니까?"), MB_YESNO))
	if(IDYES == pView->MsgBox(_T("마킹(좌)의 카운트를 리세하시겠습니까?"), 0, MB_YESNO))
	{
		pDoc->SetMkCntL(0);
		GetDlgItem(IDC_STC_215)->SetWindowText(_T("0"));
	}
}

void CDlgMenu04::OnBtnResetMkR() 
{
	// TODO: Add your control notification handler code here
//	if(IDYES == pView->DoMyMsgBox(_T("마킹(우)의 카운트를 리세하시겠습니까?"), MB_YESNO))
	if(IDYES == pView->MsgBox(_T("마킹(우)의 카운트를 리세하시겠습니까?"), 0, MB_YESNO))
	{
		pDoc->SetMkCntR(0);
		GetDlgItem(IDC_STC_220)->SetWindowText(_T("0"));
	}	
}


void CDlgMenu04::OnStc69() 
{
	// TODO: Add your control notification handler code here
	myStcTitle[111].SetBkColor(RGB_RED);
	myStcTitle[111].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_69)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_69, pt, TO_BOTTOM|TO_RIGHT);

	myStcTitle[111].SetBkColor(RGB_WHITE);
	myStcTitle[111].RedrawWindow();

	CString sData, str;
	GetDlgItem(IDC_STC_69)->GetWindowText(sData);
	pDoc->SetMarkingToq1Offset(_tstof(sData));
	str.Format(_T("\r(0~1900)\rg(%.0f)"), _tstof(pDoc->WorkingInfo.Marking[0].sMarkingToqOffset));
	GetDlgItem(IDC_STC_69)->SetWindowText(str);

	pView->m_pVoiceCoil[0]->SetMarkFinalData(0);
}

void CDlgMenu04::OnStc124() 
{
	// TODO: Add your control notification handler code here
	myStcData[43].SetBkColor(RGB_RED);
	myStcData[43].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_124)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_124, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[43].SetBkColor(RGB_WHITE);
	myStcData[43].RedrawWindow();

	CString sData, str;
	GetDlgItem(IDC_STC_124)->GetWindowText(sData);
	pDoc->SetMarkingToq2Offset(_tstof(sData));
	str.Format(_T("\r(0~1900)\rg(%.0f)"), _tstof(pDoc->WorkingInfo.Marking[1].sMarkingToqOffset));
	GetDlgItem(IDC_STC_124)->SetWindowText(str);
	
	pView->m_pVoiceCoil[1]->SetMarkFinalData(1);
}

BOOL CDlgMenu04::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB_DLG_FRM);
	return TRUE;

	//return CDialog::OnEraseBkgnd(pDC);
}

void CDlgMenu04::UpdateData()
{
	Disp();
}


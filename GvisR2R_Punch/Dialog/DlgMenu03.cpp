// DlgMenu03.cpp : implementation file
//

#include "stdafx.h"
#include "../gvisr2r_punch.h"
#include "DlgMenu03.h"

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
// CDlgMenu03 dialog


CDlgMenu03::CDlgMenu03(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMenu03::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMenu03)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pRect = NULL;
	m_bLoadImg = FALSE;
	m_bTIM_MENU03_DISP = FALSE;
	m_bTIM_CHK_DONE_MK = FALSE;
	m_bTIM_CHK_DONE_AOI = FALSE;
// 	m_bTIM_CHK_SOL_MK = FALSE;
// 	m_bTIM_CHK_SOL_AOI = FALSE;
	m_bTIM_CHK_DONE_MKAOI = FALSE;
	m_bTIM_CHK_DONE_BUF_HOME = FALSE;
	m_bTIM_CHK_DONE_BUF_INIT = FALSE;
	m_bTIM_CHK_DONE_READY = FALSE;
	m_bTIM_SET_RC_RELATION = FALSE;
	m_bTIM_SW_RC_RELATION = FALSE;
	m_bTIM_SW_UC_RELATION = FALSE;

	m_nFdSpd = 2; // Normal
	for(int i=0; i<MAX_AXIS; i++)
	{
		m_dPrevEnc[i] = 0.0;
		m_nDir[i] = 0; // -1 : CCW , 0 : NO Dir , 1 : CW
	}

	m_bBufInitPosMove = FALSE;
	m_bVMoveMkFdVac = FALSE;
	m_bVMoveAoiFdVac = FALSE;

	m_bTIM_CHK_DONE_ENGRAVE = FALSE;
}

CDlgMenu03::~CDlgMenu03()
{
	m_bTIM_MENU03_DISP = FALSE;
	m_bTIM_CHK_DONE_MK = FALSE;
	m_bTIM_CHK_DONE_AOI = FALSE;
// 	m_bTIM_CHK_SOL_MK = FALSE;
// 	m_bTIM_CHK_SOL_AOI = FALSE;
	m_bTIM_CHK_DONE_MKAOI = FALSE;
	m_bTIM_CHK_DONE_BUF_HOME = FALSE;
	m_bTIM_CHK_DONE_BUF_INIT = FALSE;
	m_bTIM_CHK_DONE_READY = FALSE;
	m_bTIM_SET_RC_RELATION = FALSE;
	m_bTIM_SW_RC_RELATION = FALSE;
	m_bTIM_SW_UC_RELATION = FALSE;

	m_bTIM_CHK_DONE_ENGRAVE = FALSE;

	DelImg();
	if(m_pRect)
	{
		delete m_pRect;
		m_pRect = NULL;
	}
}

BOOL CDlgMenu03::Create()
{
	return CDialog::Create(CDlgMenu03::IDD);
}

void CDlgMenu03::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMenu03)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMenu03, CDialog)
	//{{AFX_MSG_MAP(CDlgMenu03)
	ON_BN_CLICKED(IDC_CHK_24, OnChk24)
	ON_BN_CLICKED(IDC_CHK_62, OnChk62)
	ON_BN_CLICKED(IDC_CHK_16, OnChk16)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MYBTN_DOWN, OnMyBtnDown)
	ON_MESSAGE(WM_MYBTN_UP, OnMyBtnUp)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_CHK_79, &CDlgMenu03::OnBnClickedChk79)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMenu03 message handlers

void CDlgMenu03::OnShowWindow(BOOL bShow, UINT nStatus) 
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

void CDlgMenu03::AtDlgShow()
{
	LoadImg();
	m_bTIM_MENU03_DISP = TRUE;
	//SetTimer(TIM_MENU03_DISP, 100, NULL);	// Disp();
	SetTimer(TIM_CHK_MREG, 300, NULL);
}

void CDlgMenu03::AtDlgHide()
{
	m_bTIM_MENU03_DISP = FALSE;
	DelImg();
}

void CDlgMenu03::LoadImg()
{
	if (m_bLoadImg)
		return;
	m_bLoadImg = TRUE;

	int i;
	for (i = 0; i < MAX_MENU03_BTN; i++)
	{
		switch (i)
		{
		case 0:		// Run-Main
			myBtn[i].LoadBkImage(IMG_CIR_GRN_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_GRN_ON_DlgMenu03, BTN_IMG_DN);
			break;
		case 33:	// Stop-Main
		case 49:	// LaserPoint-Punching
		case 50:	// LaserPoint-AOI상
		case 65:	// LaserPoint-AOI하
		case 82:	// LaserPoint-Engraving
			myBtn[i].LoadBkImage(IMG_CIR_RED_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_RED_ON_DlgMenu03, BTN_IMG_DN);
			break;
		case 34:	// 운전준비-Main
			myBtn[i].LoadBkImage(IMG_CIR_BLUE_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_BLUE_ON_DlgMenu03, BTN_IMG_DN);
			break;
		case 1:		// Reset-Main
		case 40:	// 정지-Uncoiler
		case 47:	// 정지-Recoiler
			myBtn[i].LoadBkImage(IMG_CIR_YLO_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_YLO_ON_DlgMenu03, BTN_IMG_DN);
			break;
		case 4:		// 연동선택-Recoiler
		case 9:		// 연동선택-Punching
		case 17:	// 연동선택-AOI상
		case 25:	// 연동선택-Uncoiler
		case 55:	// 연동선택-AOI하
		case 72:	// 연동선택-Engraving
			myBtn[i].LoadBkImage(IMG_CIR_GRY_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_GRY_ON_DlgMenu03, BTN_IMG_DN);
			break;
		case 2:		// TorqueMotor-Punching
		case 3:		// TorqueMotor-AOI
		case 5:		// 정회전-Recoiler
		case 6:		// 역회전-Recoiler
		case 10:	// 정회전-Punching
		case 11:	// 역회전-Punching
		case 18:	// 정회전-AOI상
		case 19:	// 역회전-AOI상
		case 26:	// 정회전-Uncoiler
		case 27:	// 역회전-Uncoiler
		case 32:	// 간지 정회전-Uncoiler
		case 39:	// 간지 역회전-Uncoiler
		case 45:	// 간지 정회전-Uncoiler
		case 46:	// 간지 역회전-Uncoiler
		case 56:	// 정회전-AOI하
		case 57:	// 역회전-AOI하
		case 70:	// Core150mm-Recoiler
		case 71:	// Core150mm-Uncoiler
		case 73:	// 정회전-Engraving
		case 74:	// 역회전-Engraving
		case 83:	// TorqueMotor-Engraving
		case 85:	// 초음파세정기-Engrave
		case 86:	// 초음파세정기-AOI_DN
			myBtn[i].LoadBkImage(IMG_CIR_GRN_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_GRN_ON_DlgMenu03, BTN_IMG_DN);
			break;
		case 12:	// 피딩진공-Punching
		case 13:	// 제품푸쉬-Punching
		case 20:	// 피딩진공-AOI상
		case 21:	// 제품푸쉬-AOI상
		case 28:	// 제품댄서롤상승-Uncoiler
		case 29:	// 클린롤러상승-Uncoiler
		case 31:	// 간지척-Uncoiler
		case 35:	// 제품척-Uncoiler
		case 36:	// 클린롤러누름상승-Uncoiler
		case 41:	// 제품척-Recoiler
		case 42:	// 제품댄서롤상승-Recoiler
		case 44:	// 간지척-Recoiler
		case 48:	// 댄서롤상승-Punching
		case 51:	// 피딩클램프-Punching
		case 52:	// 텐션클램프-Punching
		case 53:	// 피딩클램프-AOI상
		case 54:	// 텐션클램프-AOI상
		case 58:	// 피딩진공-AOI하
		case 59:	// 제품푸쉬-AOI하
		case 62:	// 피딩클램프-AOI하
		case 63:	// 텐션클램프-AOI하
		case 66:	// Rewinder동작-Recoiler
		case 67:	// Rewinder제품간지-Recoiler
		case 75:	// 피딩진공-Engraving
		case 76:	// 제품푸쉬-Engraving
		case 79:	// 피딩클램프-Engraving
		case 80:	// 텐션클램프-Engraving
		case 84:	// 댄서롤상승-Engraving
			myBtn[i].LoadBkImage(IMG_CIR_DGN_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_DGN_ON_DlgMenu03, BTN_IMG_DN);
			break;
		case 7:		// 제품이음매상승우-Recoiler
		case 8:		// 제품이음매진공-Recoiler
		case 14:	// 테이블블로워-Punching
		case 15:	// 테이블진공-Punching
		case 22:	// 테이블블로워-AOI상
		case 23:	// 테이블진공-AOI상
		case 30:	// 제품이음매상승좌-Uncoiler
		case 37:	// 제품이음매상승우-Uncoiler
		case 38:	// 제품이음매진공-Uncoiler
		case 43:	// 제품이음매상승좌-Recoiler
		case 60:	// 테이블블로워-AOI하
		case 61:	// 테이블진공-AOI하
		case 77:	// 테이블블로워-Engraving
		case 78:	// 테이블진공-Engraving
			myBtn[i].LoadBkImage(IMG_CIR_PNK_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_PNK_ON_DlgMenu03, BTN_IMG_DN);
			break;
		case 16:	// 한판넬이송-Punching
		case 24:	// 한판넬이송-AOI상	
		case 64:	// 한판넬이송-AOI하	
		case 81:	// 한판넬이송-Engraving	
			myBtn[i].LoadBkImage(IMG_CIR_ORG_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_RED_ON_DlgMenu03, BTN_IMG_DN);
			break;
		case 68:	// InductionMotor-Recoiler
		case 69:	// InductionMotor-Uncoiler	
			myBtn[i].LoadBkImage(IMG_CIR_BLUE_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_RED_ON_DlgMenu03, BTN_IMG_DN);
			break;
		}
	}

	myStcTitle[33].LoadBkImage(IMG_CIR_BLU_OFF_DlgMenu03, LBL_IMG_UP);
	myStcTitle[33].LoadBkImage(IMG_CIR_BLU_ON_DlgMenu03, LBL_IMG_DN);
	myStcTitle[34].LoadBkImage(IMG_CIR_BLU_OFF_DlgMenu03, LBL_IMG_UP);
	myStcTitle[34].LoadBkImage(IMG_CIR_BLU_ON_DlgMenu03, LBL_IMG_DN);
	myStcTitle[35].LoadBkImage(IMG_CIR_BLU_OFF_DlgMenu03, LBL_IMG_UP);
	myStcTitle[35].LoadBkImage(IMG_CIR_BLU_ON_DlgMenu03, LBL_IMG_DN);
}

void CDlgMenu03::DelImg()
{
	if(!m_bLoadImg)
		return;
	m_bLoadImg = FALSE;

	int i;
	for(i=0; i<MAX_MENU03_BTN; i++)
		myBtn[i].DelImgList();

	myStcTitle[33].DelImgList();
	myStcTitle[34].DelImgList();
	myStcTitle[35].DelImgList();
}

BOOL CDlgMenu03::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitStatic();
	InitBtn();
	InitGroup();

	GetDlgItem(IDC_STC_12_38)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHK_47)->ShowWindow(SW_HIDE);	// Recoiler-Stop

	GetDlgItem(IDC_STC_12_31)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHK_40)->ShowWindow(SW_HIDE);	// Uncoiler-Stop

	//GetDlgItem(IDC_STC_12_39)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_CHK_49)->ShowWindow(SW_HIDE);	// LaserPoint-Punching

	//GetDlgItem(IDC_STC_12_52)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_CHK_63)->ShowWindow(SW_HIDE);	// LaserPoint-AOI하

	//GetDlgItem(IDC_STC_12_40)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_CHK_50)->ShowWindow(SW_HIDE);	// LaserPoint-AOI상

	//GetDlgItem(IDC_STC_12_64)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_CHK_81)->ShowWindow(SW_HIDE);	// LaserPoint-Engraving

	//pView->SetSynqIO();
	//SetJogSpd(m_nFdSpd);


	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMenu03::InitGroup()
{
	myGrp[0].SubclassDlgItem(IDC_GRP_00, this);
	myGrp[0].SetHwnd(this->GetSafeHwnd(), IDC_GRP_00);	// Main

	myGrp[1].SubclassDlgItem(IDC_GRP_1, this);
	myGrp[1].SetHwnd(this->GetSafeHwnd(), IDC_GRP_1);	// Recoiler

	myGrp[2].SubclassDlgItem(IDC_GRP_02, this);
	myGrp[2].SetHwnd(this->GetSafeHwnd(), IDC_GRP_02);	// Punching

	myGrp[3].SubclassDlgItem(IDC_GRP_4, this);
	myGrp[3].SetHwnd(this->GetSafeHwnd(), IDC_GRP_4);	// AOI하

	myGrp[4].SubclassDlgItem(IDC_GRP_03, this);
	myGrp[4].SetHwnd(this->GetSafeHwnd(), IDC_GRP_03);	// AOI상

	myGrp[5].SubclassDlgItem(IDC_GRP_04, this);
	myGrp[5].SetHwnd(this->GetSafeHwnd(), IDC_GRP_04);	// Uncoiler

	myGrp[6].SubclassDlgItem(IDC_GRP_05, this);
	myGrp[6].SetHwnd(this->GetSafeHwnd(), IDC_GRP_05);	// TorqueMotor

	myGrp[7].SubclassDlgItem(IDC_GRP_16, this);
	myGrp[7].SetHwnd(this->GetSafeHwnd(), IDC_GRP_16);	// InductionMotor

	myGrp[8].SubclassDlgItem(IDC_GRP_17, this);
	myGrp[8].SetHwnd(this->GetSafeHwnd(), IDC_GRP_17);	// Core150mm

	myGrp[9].SubclassDlgItem(IDC_GRP_3, this);
	myGrp[9].SetHwnd(this->GetSafeHwnd(), IDC_GRP_3);	// Engraving

	for (int i = 0; i < MAX_MENU03_GRP; i++)
	{
		myGrp[i].SetFontName(_T("Arial"));
		myGrp[i].SetFontSize(14);
		myGrp[i].SetFontBold(TRUE);
		myGrp[i].SetTextColor(RGB_NAVY);
		myGrp[i].SetBkColor(RGB_DLG_FRM);
	}
}

void CDlgMenu03::InitBtn()
{
	// Main
	myBtn[0].SubclassDlgItem(IDC_CHK_0, this);			// Run
	myBtn[0].SetHwnd(this->GetSafeHwnd(), IDC_CHK_0);
	myBtn[0].SetBoarder(FALSE);

	myBtn[34].SubclassDlgItem(IDC_CHK_34, this);		// 운전준비
	myBtn[34].SetHwnd(this->GetSafeHwnd(), IDC_CHK_34);
	myBtn[34].SetBoarder(FALSE);

	myBtn[33].SubclassDlgItem(IDC_CHK_33, this);		// Stop
	myBtn[33].SetHwnd(this->GetSafeHwnd(), IDC_CHK_33);
	myBtn[33].SetBoarder(FALSE);
	myBtn[1].SubclassDlgItem(IDC_CHK_1, this);			// Reset
	myBtn[1].SetHwnd(this->GetSafeHwnd(), IDC_CHK_1);
	myBtn[1].SetBoarder(FALSE);

	// Torque Motor
	myBtn[2].SubclassDlgItem(IDC_CHK_2, this);			// 펀칭부
	myBtn[2].SetHwnd(this->GetSafeHwnd(), IDC_CHK_2);
	myBtn[2].SetBoarder(FALSE);

	myBtn[3].SubclassDlgItem(IDC_CHK_3, this);			// 검사부
	myBtn[3].SetHwnd(this->GetSafeHwnd(), IDC_CHK_3);
	myBtn[3].SetBoarder(FALSE);

	myBtn[83].SubclassDlgItem(IDC_CHK_84, this);		// 각인부
	myBtn[83].SetHwnd(this->GetSafeHwnd(), IDC_CHK_84);
	myBtn[83].SetBoarder(FALSE);

	// 세정기
	myBtn[85].SubclassDlgItem(IDC_CHK_87, this);		// 세정기 - 각인부
	myBtn[85].SetHwnd(this->GetSafeHwnd(), IDC_CHK_87);
	myBtn[85].SetBoarder(FALSE);

	myBtn[86].SubclassDlgItem(IDC_CHK_88, this);		// 세정기 - AOI 하면
	myBtn[86].SetHwnd(this->GetSafeHwnd(), IDC_CHK_88);
	myBtn[86].SetBoarder(FALSE);

	// Induction Motor
	myBtn[68].SubclassDlgItem(IDC_CHK_68, this);		// Induction Motor - Recoiler\r역방향 ON
	myBtn[68].SetHwnd(this->GetSafeHwnd(), IDC_CHK_68);
	myBtn[68].SetBoarder(FALSE);

	myBtn[69].SubclassDlgItem(IDC_CHK_69, this);		// Induction Motor - Uncoiler\r역방향 ON
	myBtn[69].SetHwnd(this->GetSafeHwnd(), IDC_CHK_69);
	myBtn[69].SetBoarder(FALSE);

	// Core 150mm
	myBtn[70].SubclassDlgItem(IDC_CHK_70, this);		// Core 150mm - Recoiler
	myBtn[70].SetHwnd(this->GetSafeHwnd(), IDC_CHK_70);
	myBtn[70].SetBoarder(FALSE);

	myBtn[71].SubclassDlgItem(IDC_CHK_71, this);		// Core 150mm - Uncoiler
	myBtn[71].SetHwnd(this->GetSafeHwnd(), IDC_CHK_71);
	myBtn[71].SetBoarder(FALSE);

	// Recoiler
	myBtn[4].SubclassDlgItem(IDC_CHK_4, this);			// 연동선택
	myBtn[4].SetHwnd(this->GetSafeHwnd(), IDC_CHK_4);
	myBtn[4].SetBoarder(FALSE);

	myBtn[5].SubclassDlgItem(IDC_CHK_5, this);			// ◀정회전
	myBtn[5].SetHwnd(this->GetSafeHwnd(), IDC_CHK_5);
	myBtn[5].SetBoarder(FALSE);

	myBtn[6].SubclassDlgItem(IDC_CHK_6, this);			// 역회전▶
	myBtn[6].SetHwnd(this->GetSafeHwnd(), IDC_CHK_6);
	myBtn[6].SetBoarder(FALSE);

	myBtn[41].SubclassDlgItem(IDC_CHK_41, this);		// 제품척\r클램프
	myBtn[41].SetHwnd(this->GetSafeHwnd(), IDC_CHK_41);
	myBtn[41].SetBoarder(FALSE);

	myBtn[42].SubclassDlgItem(IDC_CHK_42, this);		// 제품댄서롤\r상승/하강
	myBtn[42].SetHwnd(this->GetSafeHwnd(), IDC_CHK_42);
	myBtn[42].SetBoarder(FALSE);

	myBtn[43].SubclassDlgItem(IDC_CHK_43, this);		// 제품이음매(좌)\r상승/하강
	myBtn[43].SetHwnd(this->GetSafeHwnd(), IDC_CHK_43);
	myBtn[43].SetBoarder(FALSE);

	myBtn[7].SubclassDlgItem(IDC_CHK_7, this);			// 제품이음매(우)\r상승/하강
	myBtn[7].SetHwnd(this->GetSafeHwnd(), IDC_CHK_7);
	myBtn[7].SetBoarder(FALSE);

	myBtn[8].SubclassDlgItem(IDC_CHK_8, this);			// 제품이음매\r진공
	myBtn[8].SetHwnd(this->GetSafeHwnd(), IDC_CHK_8);
	myBtn[8].SetBoarder(FALSE);

	myBtn[44].SubclassDlgItem(IDC_CHK_44, this);		// 간지 척\r클램프
	myBtn[44].SetHwnd(this->GetSafeHwnd(), IDC_CHK_44);
	myBtn[44].SetBoarder(FALSE);

	myBtn[45].SubclassDlgItem(IDC_CHK_45, this);		// 간지 휠\r◀정회전
	myBtn[45].SetHwnd(this->GetSafeHwnd(), IDC_CHK_45);
	myBtn[45].SetBoarder(FALSE);

	myBtn[46].SubclassDlgItem(IDC_CHK_46, this);		// 간지 휠\r역회전▶
	myBtn[46].SetHwnd(this->GetSafeHwnd(), IDC_CHK_46);
	myBtn[46].SetBoarder(FALSE);
	myBtn[46].SetBtnType(BTN_TYPE_DEFAULT);

	myBtn[47].SubclassDlgItem(IDC_CHK_47, this);		// Recoiler-정지
	myBtn[47].SetHwnd(this->GetSafeHwnd(), IDC_CHK_47);
	myBtn[47].SetBoarder(FALSE);

	myBtn[66].SubclassDlgItem(IDC_CHK_66, this);		// Rewinder\r동작
	myBtn[66].SetHwnd(this->GetSafeHwnd(), IDC_CHK_66);
	myBtn[66].SetBoarder(FALSE);

	myBtn[67].SubclassDlgItem(IDC_CHK_67, this);		// Rewinder\r제품&&간지
	myBtn[67].SetHwnd(this->GetSafeHwnd(), IDC_CHK_67);
	myBtn[67].SetBoarder(FALSE);

	// Punching
	myBtn[9].SubclassDlgItem(IDC_CHK_9, this);			// 연동선택
	myBtn[9].SetHwnd(this->GetSafeHwnd(), IDC_CHK_9);
	myBtn[9].SetBoarder(FALSE);

	myBtn[10].SubclassDlgItem(IDC_CHK_10, this);		// ◀정회전
	myBtn[10].SetHwnd(this->GetSafeHwnd(), IDC_CHK_10);
	myBtn[10].SetBoarder(FALSE);

	myBtn[11].SubclassDlgItem(IDC_CHK_11, this);		// 역회전▶
	myBtn[11].SetHwnd(this->GetSafeHwnd(), IDC_CHK_11);
	myBtn[11].SetBoarder(FALSE);

	myBtn[12].SubclassDlgItem(IDC_CHK_12, this);		// 피딩진공
	myBtn[12].SetHwnd(this->GetSafeHwnd(), IDC_CHK_12);
	myBtn[12].SetBoarder(FALSE);

	myBtn[13].SubclassDlgItem(IDC_CHK_13, this);		// 제품푸쉬
	myBtn[13].SetHwnd(this->GetSafeHwnd(), IDC_CHK_13);
	myBtn[13].SetBoarder(FALSE);

	myBtn[14].SubclassDlgItem(IDC_CHK_14, this);		// 테이블\r블로워
	myBtn[14].SetHwnd(this->GetSafeHwnd(), IDC_CHK_14);
	myBtn[14].SetBoarder(FALSE);

	myBtn[15].SubclassDlgItem(IDC_CHK_15, this);		// 테이블\r진공
	myBtn[15].SetHwnd(this->GetSafeHwnd(), IDC_CHK_15);
	myBtn[15].SetBoarder(FALSE);

	myBtn[51].SubclassDlgItem(IDC_CHK_51, this);		// 피딩\r클램프
	myBtn[51].SetHwnd(this->GetSafeHwnd(), IDC_CHK_51);
	myBtn[51].SetBoarder(FALSE);

	myBtn[52].SubclassDlgItem(IDC_CHK_52, this);		// 텐션\r클램프
	myBtn[52].SetHwnd(this->GetSafeHwnd(), IDC_CHK_52);
	myBtn[52].SetBoarder(FALSE);

	myBtn[16].SubclassDlgItem(IDC_CHK_16, this);		// 한판넬\r이송
	myBtn[16].SetHwnd(this->GetSafeHwnd(), IDC_CHK_16);
	myBtn[16].SetBoarder(FALSE);

	myBtn[49].SubclassDlgItem(IDC_CHK_49, this);		// 레이져\r마크
	myBtn[49].SetHwnd(this->GetSafeHwnd(), IDC_CHK_49);
	myBtn[49].SetBoarder(FALSE);

	myBtn[48].SubclassDlgItem(IDC_CHK_48, this);		// 댄서롤\r상승 / 하강
	myBtn[48].SetHwnd(this->GetSafeHwnd(), IDC_CHK_48);
	myBtn[48].SetBoarder(FALSE);

	// AOI 상면.	
	myBtn[17].SubclassDlgItem(IDC_CHK_17, this);		// 연동선택
	myBtn[17].SetHwnd(this->GetSafeHwnd(), IDC_CHK_17);
	myBtn[17].SetBoarder(FALSE);

	myBtn[18].SubclassDlgItem(IDC_CHK_18, this);		// ◀정회전
	myBtn[18].SetHwnd(this->GetSafeHwnd(), IDC_CHK_18);
	myBtn[18].SetBoarder(FALSE);

	myBtn[19].SubclassDlgItem(IDC_CHK_19, this);		// 역회전▶
	myBtn[19].SetHwnd(this->GetSafeHwnd(), IDC_CHK_19);
	myBtn[19].SetBoarder(FALSE);

	myBtn[20].SubclassDlgItem(IDC_CHK_20, this);		// 피딩진공
	myBtn[20].SetHwnd(this->GetSafeHwnd(), IDC_CHK_20);
	myBtn[20].SetBoarder(FALSE);

	myBtn[21].SubclassDlgItem(IDC_CHK_21, this);		// 제품푸쉬
	myBtn[21].SetHwnd(this->GetSafeHwnd(), IDC_CHK_21);
	myBtn[21].SetBoarder(FALSE);

	myBtn[22].SubclassDlgItem(IDC_CHK_22, this);		// 테이블\r블로워
	myBtn[22].SetHwnd(this->GetSafeHwnd(), IDC_CHK_22);
	myBtn[22].SetBoarder(FALSE);

	myBtn[23].SubclassDlgItem(IDC_CHK_23, this);		// 테이블\r진공
	myBtn[23].SetHwnd(this->GetSafeHwnd(), IDC_CHK_23);
	myBtn[23].SetBoarder(FALSE);

	myBtn[53].SubclassDlgItem(IDC_CHK_53, this);		// 피딩\r클램프
	myBtn[53].SetHwnd(this->GetSafeHwnd(), IDC_CHK_53);
	myBtn[53].SetBoarder(FALSE);

	myBtn[54].SubclassDlgItem(IDC_CHK_54, this);		// 텐션\r클램프
	myBtn[54].SetHwnd(this->GetSafeHwnd(), IDC_CHK_54);
	myBtn[54].SetBoarder(FALSE);

	myBtn[24].SubclassDlgItem(IDC_CHK_24, this);		// 한판넬\r이송
	myBtn[24].SetHwnd(this->GetSafeHwnd(), IDC_CHK_24);
	myBtn[24].SetBoarder(FALSE);

	myBtn[50].SubclassDlgItem(IDC_CHK_50, this);		// 레이져\r마크
	myBtn[50].SetHwnd(this->GetSafeHwnd(), IDC_CHK_50);
	myBtn[50].SetBoarder(FALSE);

	// Uncoiler
	myBtn[25].SubclassDlgItem(IDC_CHK_25, this);		// 연동선택
	myBtn[25].SetHwnd(this->GetSafeHwnd(), IDC_CHK_25);
	myBtn[25].SetBoarder(FALSE);

	myBtn[26].SubclassDlgItem(IDC_CHK_26, this);		// ◀정회전
	myBtn[26].SetHwnd(this->GetSafeHwnd(), IDC_CHK_26);
	myBtn[26].SetBoarder(FALSE);

	myBtn[27].SubclassDlgItem(IDC_CHK_27, this);		// 역회전▶
	myBtn[27].SetHwnd(this->GetSafeHwnd(), IDC_CHK_27);
	myBtn[27].SetBoarder(FALSE);

	myBtn[28].SubclassDlgItem(IDC_CHK_28, this);		// 제품댄서롤\r상승/하강
	myBtn[28].SetHwnd(this->GetSafeHwnd(), IDC_CHK_28);
	myBtn[28].SetBoarder(FALSE);

	myBtn[29].SubclassDlgItem(IDC_CHK_29, this);		// 클린롤러\r상승/하강
	myBtn[29].SetHwnd(this->GetSafeHwnd(), IDC_CHK_29);
	myBtn[29].SetBoarder(FALSE);

	myBtn[30].SubclassDlgItem(IDC_CHK_30, this);		// 제품이음매(좌)\r상승/하강
	myBtn[30].SetHwnd(this->GetSafeHwnd(), IDC_CHK_30);
	myBtn[30].SetBoarder(FALSE);

	myBtn[31].SubclassDlgItem(IDC_CHK_31, this);		// 간지 척\r클램프
	myBtn[31].SetHwnd(this->GetSafeHwnd(), IDC_CHK_31);
	myBtn[31].SetBoarder(FALSE);

	myBtn[32].SubclassDlgItem(IDC_CHK_32, this);		// 간지 휠\r◀정회전
	myBtn[32].SetHwnd(this->GetSafeHwnd(), IDC_CHK_32);
	myBtn[32].SetBoarder(FALSE);

	myBtn[35].SubclassDlgItem(IDC_CHK_35, this);		// 제품척\r클램프
	myBtn[35].SetHwnd(this->GetSafeHwnd(), IDC_CHK_35);
	myBtn[35].SetBoarder(FALSE);

	myBtn[36].SubclassDlgItem(IDC_CHK_36, this);		// 클린롤러누름\r상승/하강
	myBtn[36].SetHwnd(this->GetSafeHwnd(), IDC_CHK_36);
	myBtn[36].SetBoarder(FALSE);

	myBtn[37].SubclassDlgItem(IDC_CHK_37, this);		// 제품이음매(우)\r상승/하강
	myBtn[37].SetHwnd(this->GetSafeHwnd(), IDC_CHK_37);
	myBtn[37].SetBoarder(FALSE);		

	myBtn[38].SubclassDlgItem(IDC_CHK_38, this);		// 제품이음매\r진공
	myBtn[38].SetHwnd(this->GetSafeHwnd(), IDC_CHK_38);
	myBtn[38].SetBoarder(FALSE);

	myBtn[39].SubclassDlgItem(IDC_CHK_39, this);		// 간지 휠\r역회전▶
	myBtn[39].SetHwnd(this->GetSafeHwnd(), IDC_CHK_39);
	myBtn[39].SetBoarder(FALSE);

	myBtn[40].SubclassDlgItem(IDC_CHK_40, this);		// Uncoiler-정지
	myBtn[40].SetHwnd(this->GetSafeHwnd(), IDC_CHK_40);
	myBtn[40].SetBoarder(FALSE);

	// AOI 하면.
	myBtn[55].SubclassDlgItem(IDC_CHK_55, this);		// 연동선택
	myBtn[55].SetHwnd(this->GetSafeHwnd(), IDC_CHK_55);
	myBtn[55].SetBoarder(FALSE);

	myBtn[56].SubclassDlgItem(IDC_CHK_56, this);		// ◀정회전
	myBtn[56].SetHwnd(this->GetSafeHwnd(), IDC_CHK_56);
	myBtn[56].SetBoarder(FALSE);

	myBtn[57].SubclassDlgItem(IDC_CHK_57, this);		// 역회전▶
	myBtn[57].SetHwnd(this->GetSafeHwnd(), IDC_CHK_57);
	myBtn[57].SetBoarder(FALSE);

	myBtn[58].SubclassDlgItem(IDC_CHK_58, this);		// 피딩진공
	myBtn[58].SetHwnd(this->GetSafeHwnd(), IDC_CHK_58);
	myBtn[58].SetBoarder(FALSE);

	myBtn[59].SubclassDlgItem(IDC_CHK_59, this);		// 제품푸쉬
	myBtn[59].SetHwnd(this->GetSafeHwnd(), IDC_CHK_59);
	myBtn[59].SetBoarder(FALSE);

	myBtn[60].SubclassDlgItem(IDC_CHK_60, this);		// 테이블\r블로워
	myBtn[60].SetHwnd(this->GetSafeHwnd(), IDC_CHK_60);
	myBtn[60].SetBoarder(FALSE);

	myBtn[61].SubclassDlgItem(IDC_CHK_61, this);		// 테이블\r진공
	myBtn[61].SetHwnd(this->GetSafeHwnd(), IDC_CHK_61);
	myBtn[61].SetBoarder(FALSE);

	myBtn[62].SubclassDlgItem(IDC_CHK_64, this);		// 피딩\r클램프
	myBtn[62].SetHwnd(this->GetSafeHwnd(), IDC_CHK_64);
	myBtn[62].SetBoarder(FALSE);

	myBtn[63].SubclassDlgItem(IDC_CHK_65, this);		// 텐션\r클램프
	myBtn[63].SetHwnd(this->GetSafeHwnd(), IDC_CHK_65);
	myBtn[63].SetBoarder(FALSE);

	myBtn[64].SubclassDlgItem(IDC_CHK_62, this);		// 한판넬\r이송
	myBtn[64].SetHwnd(this->GetSafeHwnd(), IDC_CHK_62);
	myBtn[64].SetBoarder(FALSE);

	myBtn[65].SubclassDlgItem(IDC_CHK_63, this);		// 레이져\r마크
	myBtn[65].SetHwnd(this->GetSafeHwnd(), IDC_CHK_63);
	myBtn[65].SetBoarder(FALSE);

	// Engraving
	myBtn[72].SubclassDlgItem(IDC_CHK_72, this);		// 연동선택
	myBtn[72].SetHwnd(this->GetSafeHwnd(), IDC_CHK_72);
	myBtn[72].SetBoarder(FALSE);

	myBtn[73].SubclassDlgItem(IDC_CHK_73, this);		// ◀정회전
	myBtn[73].SetHwnd(this->GetSafeHwnd(), IDC_CHK_73);
	myBtn[73].SetBoarder(FALSE);

	myBtn[74].SubclassDlgItem(IDC_CHK_74, this);		// 역회전▶
	myBtn[74].SetHwnd(this->GetSafeHwnd(), IDC_CHK_74);
	myBtn[74].SetBoarder(FALSE);

	myBtn[75].SubclassDlgItem(IDC_CHK_75, this);		// 피딩진공
	myBtn[75].SetHwnd(this->GetSafeHwnd(), IDC_CHK_75);
	myBtn[75].SetBoarder(FALSE);

	myBtn[76].SubclassDlgItem(IDC_CHK_76, this);		// 제품푸쉬
	myBtn[76].SetHwnd(this->GetSafeHwnd(), IDC_CHK_76);
	myBtn[76].SetBoarder(FALSE);

	myBtn[77].SubclassDlgItem(IDC_CHK_77, this);		// 테이블\r블로워
	myBtn[77].SetHwnd(this->GetSafeHwnd(), IDC_CHK_77);
	myBtn[77].SetBoarder(FALSE);

	myBtn[78].SubclassDlgItem(IDC_CHK_78, this);		// 테이블\r진공
	myBtn[78].SetHwnd(this->GetSafeHwnd(), IDC_CHK_78);
	myBtn[78].SetBoarder(FALSE);

	myBtn[79].SubclassDlgItem(IDC_CHK_82, this);		// 피딩\r클램프
	myBtn[79].SetHwnd(this->GetSafeHwnd(), IDC_CHK_82);
	myBtn[79].SetBoarder(FALSE);

	myBtn[80].SubclassDlgItem(IDC_CHK_83, this);		// 텐션\r클램프
	myBtn[80].SetHwnd(this->GetSafeHwnd(), IDC_CHK_83);
	myBtn[80].SetBoarder(FALSE);

	myBtn[81].SubclassDlgItem(IDC_CHK_79, this);		// 한판넬\r이송
	myBtn[81].SetHwnd(this->GetSafeHwnd(), IDC_CHK_79);
	myBtn[81].SetBoarder(FALSE);

	myBtn[82].SubclassDlgItem(IDC_CHK_81, this);		// 레이져\r마크
	myBtn[82].SetHwnd(this->GetSafeHwnd(), IDC_CHK_81);
	myBtn[82].SetBoarder(FALSE);

	myBtn[84].SubclassDlgItem(IDC_CHK_80, this);		// 댄서롤\r상승/하강
	myBtn[84].SetHwnd(this->GetSafeHwnd(), IDC_CHK_80);
	myBtn[84].SetBoarder(FALSE);

#ifdef USE_MPE
	myBtn[0].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[1].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[2].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[3].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[4].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[5].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[6].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[7].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[8].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[9].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[10].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[11].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[12].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[13].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[14].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[15].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[16].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[17].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[18].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[19].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[20].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[21].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[22].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[23].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[24].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[25].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[26].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[27].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[28].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[29].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[30].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[31].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[32].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[33].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[34].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[35].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[36].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[37].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[38].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[39].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[40].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[41].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[42].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[43].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[44].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[45].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[47].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[48].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[49].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[50].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[51].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[52].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[53].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[54].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[55].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[56].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[57].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[58].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[59].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[60].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[61].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[62].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[63].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[64].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[65].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[66].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[67].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[68].SetBtnType(BTN_TYPE_CHECK);
	myBtn[69].SetBtnType(BTN_TYPE_CHECK);
	myBtn[70].SetBtnType(BTN_TYPE_CHECK);
	myBtn[71].SetBtnType(BTN_TYPE_CHECK);
	myBtn[72].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[73].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[74].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[75].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[76].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[77].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[78].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[79].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[80].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[81].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[82].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[83].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[84].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[85].SetBtnType(BTN_TYPE_DEFAULT);
	myBtn[86].SetBtnType(BTN_TYPE_DEFAULT);
#else
	myBtn[0].SetBtnType(BTN_TYPE_CHECK);
	myBtn[1].SetBtnType(BTN_TYPE_CHECK);
	myBtn[2].SetBtnType(BTN_TYPE_CHECK);
	myBtn[3].SetBtnType(BTN_TYPE_CHECK);
	myBtn[4].SetBtnType(BTN_TYPE_CHECK);
	myBtn[5].SetBtnType(BTN_TYPE_CHECK);
	myBtn[6].SetBtnType(BTN_TYPE_CHECK);
	myBtn[7].SetBtnType(BTN_TYPE_CHECK);
	myBtn[8].SetBtnType(BTN_TYPE_CHECK);
	myBtn[9].SetBtnType(BTN_TYPE_CHECK);
	myBtn[10].SetBtnType(BTN_TYPE_CHECK);
	myBtn[11].SetBtnType(BTN_TYPE_CHECK);
	myBtn[12].SetBtnType(BTN_TYPE_CHECK);
	myBtn[13].SetBtnType(BTN_TYPE_CHECK);
	myBtn[14].SetBtnType(BTN_TYPE_CHECK);
	myBtn[15].SetBtnType(BTN_TYPE_CHECK);
	myBtn[16].SetBtnType(BTN_TYPE_CHECK);
	myBtn[17].SetBtnType(BTN_TYPE_CHECK);
	myBtn[18].SetBtnType(BTN_TYPE_CHECK);
	myBtn[19].SetBtnType(BTN_TYPE_CHECK);
	myBtn[20].SetBtnType(BTN_TYPE_CHECK);
	myBtn[21].SetBtnType(BTN_TYPE_CHECK);
	myBtn[22].SetBtnType(BTN_TYPE_CHECK);
	myBtn[23].SetBtnType(BTN_TYPE_CHECK);
	myBtn[24].SetBtnType(BTN_TYPE_CHECK);
	myBtn[25].SetBtnType(BTN_TYPE_CHECK);
	myBtn[26].SetBtnType(BTN_TYPE_CHECK);
	myBtn[27].SetBtnType(BTN_TYPE_CHECK);
	myBtn[28].SetBtnType(BTN_TYPE_CHECK);
	myBtn[29].SetBtnType(BTN_TYPE_CHECK);
	myBtn[30].SetBtnType(BTN_TYPE_CHECK);
	myBtn[31].SetBtnType(BTN_TYPE_CHECK);
	myBtn[32].SetBtnType(BTN_TYPE_CHECK);
	myBtn[33].SetBtnType(BTN_TYPE_CHECK);
	myBtn[34].SetBtnType(BTN_TYPE_CHECK);
	myBtn[35].SetBtnType(BTN_TYPE_CHECK);
	myBtn[36].SetBtnType(BTN_TYPE_CHECK);
	myBtn[37].SetBtnType(BTN_TYPE_CHECK);
	myBtn[38].SetBtnType(BTN_TYPE_CHECK);
	myBtn[39].SetBtnType(BTN_TYPE_CHECK);
	myBtn[40].SetBtnType(BTN_TYPE_CHECK);
	myBtn[41].SetBtnType(BTN_TYPE_CHECK);
	myBtn[42].SetBtnType(BTN_TYPE_CHECK);
	myBtn[43].SetBtnType(BTN_TYPE_CHECK);
	myBtn[44].SetBtnType(BTN_TYPE_CHECK);
	myBtn[45].SetBtnType(BTN_TYPE_CHECK);
	myBtn[47].SetBtnType(BTN_TYPE_CHECK);
	myBtn[48].SetBtnType(BTN_TYPE_CHECK);
	myBtn[49].SetBtnType(BTN_TYPE_CHECK);
	myBtn[50].SetBtnType(BTN_TYPE_CHECK);
	myBtn[51].SetBtnType(BTN_TYPE_CHECK);
	myBtn[52].SetBtnType(BTN_TYPE_CHECK);
	myBtn[53].SetBtnType(BTN_TYPE_CHECK);
	myBtn[54].SetBtnType(BTN_TYPE_CHECK);
	myBtn[55].SetBtnType(BTN_TYPE_CHECK);
	myBtn[56].SetBtnType(BTN_TYPE_CHECK);
	myBtn[57].SetBtnType(BTN_TYPE_CHECK);
	myBtn[58].SetBtnType(BTN_TYPE_CHECK);
	myBtn[59].SetBtnType(BTN_TYPE_CHECK);
	myBtn[60].SetBtnType(BTN_TYPE_CHECK);
	myBtn[61].SetBtnType(BTN_TYPE_CHECK);
	myBtn[62].SetBtnType(BTN_TYPE_CHECK);
	myBtn[63].SetBtnType(BTN_TYPE_CHECK);
	myBtn[64].SetBtnType(BTN_TYPE_CHECK);
	myBtn[65].SetBtnType(BTN_TYPE_CHECK);
	myBtn[66].SetBtnType(BTN_TYPE_CHECK);
	myBtn[67].SetBtnType(BTN_TYPE_CHECK);
	myBtn[68].SetBtnType(BTN_TYPE_CHECK);
	myBtn[69].SetBtnType(BTN_TYPE_CHECK);
	myBtn[70].SetBtnType(BTN_TYPE_CHECK);
	myBtn[71].SetBtnType(BTN_TYPE_CHECK);
	myBtn[72].SetBtnType(BTN_TYPE_CHECK);
	myBtn[73].SetBtnType(BTN_TYPE_CHECK);
	myBtn[74].SetBtnType(BTN_TYPE_CHECK);
	myBtn[75].SetBtnType(BTN_TYPE_CHECK);
	myBtn[76].SetBtnType(BTN_TYPE_CHECK);
	myBtn[77].SetBtnType(BTN_TYPE_CHECK);
	myBtn[78].SetBtnType(BTN_TYPE_CHECK);
	myBtn[79].SetBtnType(BTN_TYPE_CHECK);
	myBtn[80].SetBtnType(BTN_TYPE_CHECK);
	myBtn[81].SetBtnType(BTN_TYPE_CHECK);
	myBtn[82].SetBtnType(BTN_TYPE_CHECK);
	myBtn[83].SetBtnType(BTN_TYPE_CHECK);
	myBtn[84].SetBtnType(BTN_TYPE_CHECK);
	myBtn[85].SetBtnType(BTN_TYPE_CHECK);
	myBtn[86].SetBtnType(BTN_TYPE_CHECK);
#endif

	int i;
	for (i = 0; i < MAX_MENU03_BTN; i++)
	{
		myBtn[i].SetFont(_T("굴림체"), 12, TRUE);
		myBtn[i].SetTextColor(RGB_BLACK);
	}
}

void CDlgMenu03::InitStatic()
{
	// Main
	myStcTitle[0].SubclassDlgItem(IDC_STC_000_, this);		// Run
	myStcTitle[1].SubclassDlgItem(IDC_STC_01_, this);		// Reset
	myStcTitle[39].SubclassDlgItem(IDC_STC_000_3, this);	// Stop
	myStcTitle[40].SubclassDlgItem(IDC_STC_000_4, this);	// 운전준비

	myStcTitle[33].SubclassDlgItem(IDC_STC_02, this);		// LED-자동 
	myStcTitle[34].SubclassDlgItem(IDC_STC_03, this);		// LED-1회운전
	myStcTitle[35].SubclassDlgItem(IDC_STC_04, this);		// LED-수동 

	myStcTitle[36].SubclassDlgItem(IDC_STC_02_, this);		// 자동
	myStcTitle[37].SubclassDlgItem(IDC_STC_03_, this);		// 1회운전
	myStcTitle[38].SubclassDlgItem(IDC_STC_04_, this);		// 수동

	// TorqueMotor
	myStcTitle[2].SubclassDlgItem(IDC_STC_000_2, this);		// 펀칭부
	myStcTitle[3].SubclassDlgItem(IDC_STC_01_2, this);		// 검사부
	myStcTitle[78].SubclassDlgItem(IDC_STC_000_9, this);	// 각인부

	// InductionMotor
	myStcTitle[74].SubclassDlgItem(IDC_STC_000_6, this);	// Recoiler역방향
	myStcTitle[75].SubclassDlgItem(IDC_STC_01_3, this);		// Uncoiler역방향

	// Core150mm
	myStcTitle[76].SubclassDlgItem(IDC_STC_000_7, this);	// Recoiler
	myStcTitle[77].SubclassDlgItem(IDC_STC_01_4, this);		// Uncoiler

	// Recoiler
	myStcTitle[4].SubclassDlgItem(IDC_STC_10_, this);		// 연동선택
	myStcTitle[5].SubclassDlgItem(IDC_STC_12_2, this);		// 정회전
	myStcTitle[6].SubclassDlgItem(IDC_STC_12_, this);		// 역회전
	myStcTitle[7].SubclassDlgItem(IDC_STC_12_4, this);		// 제품이음매(우)상승
	myStcTitle[8].SubclassDlgItem(IDC_STC_12_3, this);		// 제품이음매진공
	myStcTitle[47].SubclassDlgItem(IDC_STC_12_32, this);	// 제품척클램프
	myStcTitle[48].SubclassDlgItem(IDC_STC_12_33, this);	// 제품댄서롤상승
	myStcTitle[49].SubclassDlgItem(IDC_STC_12_34, this);	// 제품이음매(좌)상승
	myStcTitle[50].SubclassDlgItem(IDC_STC_12_36, this);	// 간지척클램프
	myStcTitle[51].SubclassDlgItem(IDC_STC_12_35, this);	// 간지휠정회전
	myStcTitle[52].SubclassDlgItem(IDC_STC_12_37, this);	// 간지휠역회전
	myStcTitle[53].SubclassDlgItem(IDC_STC_12_38, this);	// 정지
	myStcTitle[72].SubclassDlgItem(IDC_STC_12_55, this);	// Rewinder동작
	myStcTitle[73].SubclassDlgItem(IDC_STC_12_56, this);	// Rewinder제품간지

	// Punching
	myStcTitle[9].SubclassDlgItem(IDC_STC_20_, this);		// 연동선택
	myStcTitle[10].SubclassDlgItem(IDC_STC_12_6, this);		// 정회전
	myStcTitle[11].SubclassDlgItem(IDC_STC_12_5, this);		// 역회전
	myStcTitle[12].SubclassDlgItem(IDC_STC_12_8, this);		// 피딩진공
	myStcTitle[13].SubclassDlgItem(IDC_STC_12_7, this);		// 제품푸쉬
	myStcTitle[14].SubclassDlgItem(IDC_STC_12_10, this);	// 테이블블로워
	myStcTitle[15].SubclassDlgItem(IDC_STC_12_9, this);		// 테이블진공
	myStcTitle[16].SubclassDlgItem(IDC_STC_12_12, this);	// 한판넬이송
	myStcTitle[54].SubclassDlgItem(IDC_STC_000_5, this);	// 댄서롤상승
	myStcTitle[55].SubclassDlgItem(IDC_STC_12_39, this);	// 레이져마크
	myStcTitle[57].SubclassDlgItem(IDC_STC_12_42, this);	// 피딩클램프
	myStcTitle[58].SubclassDlgItem(IDC_STC_12_41, this);	// 텐션클램프

	// AOI하
	myStcTitle[61].SubclassDlgItem(IDC_STC_30_2, this);		// 연동선택
	myStcTitle[62].SubclassDlgItem(IDC_STC_12_46, this);	// 정회전
	myStcTitle[63].SubclassDlgItem(IDC_STC_12_45, this);	// 역회전
	myStcTitle[64].SubclassDlgItem(IDC_STC_12_48, this);	// 피딩진공
	myStcTitle[65].SubclassDlgItem(IDC_STC_12_47, this);	// 제품푸쉬
	myStcTitle[66].SubclassDlgItem(IDC_STC_12_50, this);	// 테이블블로워
	myStcTitle[67].SubclassDlgItem(IDC_STC_12_49, this);	// 테이블진공
	myStcTitle[68].SubclassDlgItem(IDC_STC_12_54, this);	// 피딩클램프
	myStcTitle[69].SubclassDlgItem(IDC_STC_12_53, this);	// 텐션클램프
	myStcTitle[70].SubclassDlgItem(IDC_STC_12_51, this);	// 한판넬이송
	myStcTitle[71].SubclassDlgItem(IDC_STC_12_52, this);	// 레이져마크

	// AOI상
	myStcTitle[17].SubclassDlgItem(IDC_STC_30_, this);		// 연동선택
	myStcTitle[18].SubclassDlgItem(IDC_STC_12_13, this);	// 정회전
	myStcTitle[19].SubclassDlgItem(IDC_STC_12_11, this);	// 역회전
	myStcTitle[20].SubclassDlgItem(IDC_STC_12_15, this);	// 피딩진공
	myStcTitle[21].SubclassDlgItem(IDC_STC_12_14, this);	// 제품푸쉬
	myStcTitle[22].SubclassDlgItem(IDC_STC_12_17, this);	// 테이블블로워
	myStcTitle[23].SubclassDlgItem(IDC_STC_12_16, this);	// 테이블진공
	myStcTitle[24].SubclassDlgItem(IDC_STC_12_19, this);	// 한판넬이송
	myStcTitle[56].SubclassDlgItem(IDC_STC_12_40, this);	// 레이져마크
	myStcTitle[59].SubclassDlgItem(IDC_STC_12_44, this);	// 피딩클램프
	myStcTitle[60].SubclassDlgItem(IDC_STC_12_43, this);	// 텐션클램프

	// Uncoiler
	myStcTitle[25].SubclassDlgItem(IDC_STC_40_, this);		// 연동선택
	myStcTitle[26].SubclassDlgItem(IDC_STC_12_20, this);	// 정회전
	myStcTitle[27].SubclassDlgItem(IDC_STC_12_18, this);	// 역회전
	myStcTitle[28].SubclassDlgItem(IDC_STC_12_22, this);	// 제품댄서롤상승
	myStcTitle[29].SubclassDlgItem(IDC_STC_12_21, this);	// 클린롤러상승
	myStcTitle[30].SubclassDlgItem(IDC_STC_12_24, this);	// 제품이음매(좌)상승
	myStcTitle[31].SubclassDlgItem(IDC_STC_12_25, this);	// 간지척클램프
	myStcTitle[32].SubclassDlgItem(IDC_STC_12_23, this);	// 간지휠정회전
	myStcTitle[41].SubclassDlgItem(IDC_STC_12_26, this);	// 제품척클램프
	myStcTitle[42].SubclassDlgItem(IDC_STC_12_27, this);	// 클린롤러누름상승
	myStcTitle[43].SubclassDlgItem(IDC_STC_12_28, this);	// 제품이음매(우)상승
	myStcTitle[44].SubclassDlgItem(IDC_STC_12_29, this);	// 제품이음매진공
	myStcTitle[45].SubclassDlgItem(IDC_STC_12_30, this);	// 간지휠역회전
	myStcTitle[46].SubclassDlgItem(IDC_STC_12_31, this);	// 정지

	// Engraving
	myStcTitle[79].SubclassDlgItem(IDC_STC_20_2, this);		// 연동선택
	myStcTitle[80].SubclassDlgItem(IDC_STC_12_58, this);	// 정회전
	myStcTitle[81].SubclassDlgItem(IDC_STC_12_57, this);	// 역회전
	myStcTitle[82].SubclassDlgItem(IDC_STC_12_60, this);	// 피딩진공
	myStcTitle[83].SubclassDlgItem(IDC_STC_12_59, this);	// 제품푸쉬
	myStcTitle[84].SubclassDlgItem(IDC_STC_12_62, this);	// 테이블블로워
	myStcTitle[85].SubclassDlgItem(IDC_STC_12_61, this);	// 테이블진공
	myStcTitle[86].SubclassDlgItem(IDC_STC_12_63, this);	// 한판넬이송
	myStcTitle[87].SubclassDlgItem(IDC_STC_000_8, this);	// 댄서롤상승
	myStcTitle[88].SubclassDlgItem(IDC_STC_12_64, this);	// 레이져마크
	myStcTitle[89].SubclassDlgItem(IDC_STC_12_66, this);	// 피딩클램프
	myStcTitle[90].SubclassDlgItem(IDC_STC_12_65, this);	// 텐션클램프

	// 세정기
	myStcTitle[91].SubclassDlgItem(IDC_STC_000_10, this);	// 각인부 세정기
	myStcTitle[92].SubclassDlgItem(IDC_STC_000_11, this);	// AOI(하) 세정기

	for (int i = 0; i < MAX_MENU03_STC; i++)
	{
		myStcTitle[i].SetFontName(_T("Arial"));
		myStcTitle[i].SetFontSize(12);
		myStcTitle[i].SetFontBold(TRUE);
		myStcTitle[i].SetTextColor(RGB_NAVY);
		myStcTitle[i].SetBkColor(RGB_DLG_FRM);
	}
}

void CDlgMenu03::Disp()
{
	DispMain();
	DispRecoiler();
	DispPunch();
	DispAoiDn();
	DispAoiUp();
	DispEngrave();
	DispUncoiler();
}

void CDlgMenu03::DispMain()
{
	BOOL bOn;

	// [Main]
	bOn = pDoc->BtnStatus.Main.Ready;	// 마킹부 운전준비 스위치 램프
	if (myBtn[34].GetCheck() != bOn)
		myBtn[34].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Main.Run;		// 마킹부 운전 스위치 램프
	if (myBtn[0].GetCheck() != bOn)
		myBtn[0].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Main.Reset;	// 마킹부 리셋 스위치 램프
	if (myBtn[1].GetCheck() != bOn)
		myBtn[1].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Main.Stop;	// 마킹부 정지 스위치 램프
	if (myBtn[33].GetCheck() != bOn)
		myBtn[33].SetCheck(bOn);

	pDoc->Status.bAuto = pDoc->BtnStatus.Main.Auto;
	pDoc->Status.bManual = pDoc->BtnStatus.Main.Manual;
	if (pDoc->Status.bAuto)	// Auto
	{
		if (myStcTitle[33].GetImageBk() != LBL_IMG_DN)
			myStcTitle[33].SetImageBk(LBL_IMG_DN);	// 자 동
		if (myStcTitle[34].GetImageBk() != LBL_IMG_UP)
			myStcTitle[34].SetImageBk(LBL_IMG_UP);	// 1회 운전
		if (myStcTitle[35].GetImageBk() != LBL_IMG_UP)
			myStcTitle[35].SetImageBk(LBL_IMG_UP);	// 수 동 
	}
	else if (pDoc->Status.bManual)	// Manual
	{
		if (myStcTitle[33].GetImageBk() != LBL_IMG_UP)
			myStcTitle[33].SetImageBk(LBL_IMG_UP);	// 자 동
		if (myStcTitle[34].GetImageBk() != LBL_IMG_UP)
			myStcTitle[34].SetImageBk(LBL_IMG_UP);	// 1회 운전
		if (myStcTitle[35].GetImageBk() != LBL_IMG_DN)
			myStcTitle[35].SetImageBk(LBL_IMG_DN);	// 수 동 
	}
	else if (pDoc->Status.bOneCycle)	// OneCycl
	{
		if (myStcTitle[33].GetImageBk() != LBL_IMG_UP)
			myStcTitle[33].SetImageBk(LBL_IMG_UP);	// 자 동
		if (myStcTitle[34].GetImageBk() != LBL_IMG_DN)
			myStcTitle[34].SetImageBk(LBL_IMG_DN);	// 1회 운전
		if (myStcTitle[35].GetImageBk() != LBL_IMG_UP)
			myStcTitle[35].SetImageBk(LBL_IMG_UP);	// 수 동 
	}
	else
	{
		if (myStcTitle[33].GetImageBk() != LBL_IMG_UP)
			myStcTitle[33].SetImageBk(LBL_IMG_UP);	// 자 동
		if (myStcTitle[34].GetImageBk() != LBL_IMG_UP)
			myStcTitle[34].SetImageBk(LBL_IMG_UP);	// 1회 운전
		if (myStcTitle[35].GetImageBk() != LBL_IMG_UP)
			myStcTitle[35].SetImageBk(LBL_IMG_UP);	// 수 동 
	}

	// [Torque Motor]
	bOn = pDoc->WorkingInfo.Motion.bMkTq = pDoc->BtnStatus.Tq.Mk;
	if (myBtn[2].GetImageBk() != bOn)
		myBtn[2].SetCheck(bOn);

	bOn = pDoc->WorkingInfo.Motion.bAoiTq = pDoc->BtnStatus.Tq.Aoi;
	if (myBtn[3].GetImageBk() != bOn)
		myBtn[3].SetCheck(bOn);

	bOn = pDoc->WorkingInfo.Motion.bEngraveTq = pDoc->BtnStatus.Tq.Eng;
	if (myBtn[83].GetImageBk() != bOn)
		myBtn[83].SetCheck(bOn);

	// [InductionMotor]
	CString sCap;
	bOn = pDoc->WorkingInfo.LastJob.bOneMetal = pDoc->BtnStatus.Induct.Rc;	// [One Metal]
	if (bOn)
	{
		sCap = myStcTitle[74].GetText();
		if (sCap != _T("Recoiler\r정방향 CCW"))
		{
			myStcTitle[74].ShowWindow(SW_HIDE);
			myStcTitle[74].SetTextColor(RGB_RED);
			myStcTitle[74].SetText(_T("Recoiler\r정방향 CCW"));
			myStcTitle[74].ShowWindow(SW_SHOW);
		}
	}
	else
	{
		sCap = myStcTitle[74].GetText();
		if (sCap != _T("Recoiler\r정방향 CW"))
		{
			myStcTitle[74].ShowWindow(SW_HIDE);
			myStcTitle[74].SetTextColor(RGB_NAVY);
			myStcTitle[74].SetText(_T("Recoiler\r정방향 CW"));
			myStcTitle[74].ShowWindow(SW_SHOW);
		}
	}

	if (myBtn[68].GetImageBk() != bOn) // Induction Motor Recoiler\r역방향 ON
	{
		myBtn[68].SetCheck(bOn);
	}

	bOn = pDoc->WorkingInfo.LastJob.bTwoMetal = pDoc->BtnStatus.Induct.Uc;	// [Two Metal]
	if (bOn)
	{
		sCap = myStcTitle[75].GetText();
		if (sCap != _T("Uncoiler\r역방향"))
		{
			myStcTitle[75].ShowWindow(SW_HIDE);
			myStcTitle[75].SetTextColor(RGB_RED);
			myStcTitle[75].SetText(_T("Uncoiler\r역방향"));
			myStcTitle[75].ShowWindow(SW_SHOW);
		}
	}
	else
	{
		sCap = myStcTitle[75].GetText();
		if (sCap != _T("Uncoiler\r정방향"))
		{
			myStcTitle[75].ShowWindow(SW_HIDE);
			myStcTitle[75].SetTextColor(RGB_NAVY);
			myStcTitle[75].SetText(_T("Uncoiler\r정방향"));
			myStcTitle[75].ShowWindow(SW_SHOW);
		}
	}

	if (myBtn[69].GetImageBk() != bOn) // Induction Motor Uncoiler\r역방향 ON
	{
		myBtn[69].SetCheck(bOn);
	}

	// [Core 150mm]
	bOn = pDoc->WorkingInfo.LastJob.bCore150Recoiler = pDoc->BtnStatus.Core150.Rc;
	if (myBtn[70].GetImageBk() != bOn)
		myBtn[70].SetCheck(bOn);

	bOn = pDoc->WorkingInfo.LastJob.bCore150Uncoiler = pDoc->BtnStatus.Core150.Uc;
	if (myBtn[71].GetImageBk() != bOn)
		myBtn[71].SetCheck(bOn);
}

void CDlgMenu03::DispRecoiler()
{
	BOOL bOn;

	// [Recoiler]
	bOn = pDoc->BtnStatus.Rc.Relation;	// 리코일러 연동 온/오프 스위치 램프
	if (myBtn[4].GetImageBk() != bOn)
		myBtn[4].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Rc.FdCw;	// 리코일러 제품휠 정회전 스위치 램프
	if (myBtn[5].GetImageBk() != bOn)
		myBtn[5].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Rc.FdCcw;	// 리코일러 제품휠 역회전 스위치 램프
	if (myBtn[6].GetImageBk() != bOn)
		myBtn[6].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Rc.ReelChuck;	// 리코일러 제품척 클램프 스위치 램프
	if (myBtn[41].GetImageBk() != bOn)
		myBtn[41].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Rc.DcRlUpDn;	// 리코일러 댄서롤 상승/하강 스위치 램프
	if (myBtn[42].GetImageBk() != bOn)
		myBtn[42].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Rc.ReelJoinL;	// 리코일러 제품 이음매(좌) 스위치 램프
	if (myBtn[43].GetImageBk() != bOn)
		myBtn[43].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Rc.ReelJoinR;	// 리코일러 제품 이음매(우) 스위치 램프
	if (myBtn[7].GetImageBk() != bOn)
		myBtn[7].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Rc.ReelJoinVac;	// 리코일러 제품 이음매 진공 스위치 램프
	if (myBtn[8].GetImageBk() != bOn)
		myBtn[8].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Rc.PprChuck;	// 리코일러 간지척 클램프 스위치 램프
	if (myBtn[44].GetImageBk() != bOn)
		myBtn[44].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Rc.PprCw;	// 리코일러 간지휠 정회전 스위치 램프
	if (myBtn[45].GetImageBk() != bOn)
		myBtn[45].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Rc.PprCcw;	// 리코일러 간지휠 역회전 스위치 램프
	if (myBtn[46].GetImageBk() != bOn)
		myBtn[46].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Rc.Rewine;	// 리코일러 Rewinder 동작 스위치 램프
	if (myBtn[66].GetImageBk() != bOn)
		myBtn[66].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Rc.RewineReelPpr;	// 리코일러 Rewinder 제품 & 간지 스위치 램프
	if (myBtn[67].GetImageBk() != bOn)
		myBtn[67].SetCheck(bOn);

	// 	bOn = pDoc->m_pMpeIo[nInSeg + 16] & (0x01<<7) ? TRUE : FALSE;	// 리코일러 제품휠 지지 스위치 램프
	// 	if(myBtn[8].GetImageBk() != bOn)
	// 		myBtn[8].SetCheck(bOn);

	//bOn = pDoc->m_pMpeIo[nInSeg + 16] & (0x01 << 14) ? TRUE : FALSE;	// 리코일러 제품 EPC원점 스위치 램프
	//if (myBtn[47].GetImageBk() != bOn)
	//	myBtn[47].SetCheck(bOn);
}

void CDlgMenu03::DispPunch()
{
	BOOL bOn;

	// [Marking]
	bOn = pDoc->BtnStatus.Mk.Relation;	// 마킹부 연동 온/오프 스위치 램프
	if (myBtn[9].GetImageBk() != bOn)
		myBtn[9].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Mk.FdCw;	// 마킹부 피딩 정회전 스위치 램프
	if (myBtn[10].GetImageBk() != bOn)
		myBtn[10].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Mk.FdCcw;	// 마킹부 피딩 역회전 스위치 램프
	if (myBtn[11].GetImageBk() != bOn)
		myBtn[11].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Mk.FdVac;	// 마킹부 피딩 진공 스위치 램프
	if (myBtn[12].GetImageBk() != bOn)
		myBtn[12].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Mk.PushUp;	// 마킹부 제품푸쉬 스위치 램프 // 마킹부 토크 진공 스위치 램프 - X
	if (myBtn[13].GetImageBk() != bOn)
		myBtn[13].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Mk.TblBlw;	// 마킹부 테이블 브로워 스위치 램프
	if (myBtn[14].GetImageBk() != bOn)
		myBtn[14].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Mk.TblVac;	// 마킹부 테이블 진공 스위치 램프
	if (myBtn[15].GetImageBk() != bOn)
		myBtn[15].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Mk.FdClp;	// 마킹부 피딩 클램프 스위치 램프
	if (myBtn[51].GetImageBk() != bOn)
		myBtn[51].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Mk.TqClp;	// 마킹부 토크 클램프 스위치 램프
	if (myBtn[52].GetImageBk() != bOn)
		myBtn[52].SetCheck(bOn);

	bOn = pDoc->WorkingInfo.LastJob.bMkOnePnl = pDoc->BtnStatus.Mk.MvOne; // 마킹부 한판넬 이송
	if (myBtn[16].GetImageBk() != bOn)
		myBtn[16].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Mk.LsrPt;	// 마킹부 레이져 포인터 스위치 램프
	if (myBtn[49].GetImageBk() != bOn)
		myBtn[49].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Mk.DcRSol;	// 마킹부 댄서롤 상승/하강 스위치 램프
	if (myBtn[48].GetCheck() != bOn)
		myBtn[48].SetCheck(bOn);
}

void CDlgMenu03::DispAoiDn()
{
	BOOL bOn;

	// [하면 AOI]
	bOn = pDoc->BtnStatus.AoiDn.Relation;	// 검사부 하 연동 온/오프 스위치 램프
	if (myBtn[55].GetImageBk() != bOn)
		myBtn[55].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiDn.FdCw;	// 검사부 하 피딩 정회전 스위치 램프
	if (myBtn[56].GetImageBk() != bOn)
		myBtn[56].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiDn.FdCcw;	// 검사부 하 피딩 역회전 스위치 램프
	if (myBtn[57].GetImageBk() != bOn)
		myBtn[57].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiDn.FdVac;	// 검사부 하 피딩 진공 스위치 램프
	if (myBtn[58].GetImageBk() != bOn)
		myBtn[58].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiDn.PushUp;	// 검사부 하 제품푸쉬 스위치 램프 // (토크 진공 스위치 램프) - X
	if (myBtn[59].GetImageBk() != bOn)
		myBtn[59].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiDn.TblBlw;	// 검사부 하 테이블 브로워 스위치 램프
	if (myBtn[60].GetImageBk() != bOn)
		myBtn[60].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiDn.TblVac;	// 검사부 하 테이블 진공 스위치 램프
	if (myBtn[61].GetImageBk() != bOn)
		myBtn[61].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiDn.FdClp;	// 검사부 하 피딩 클램프 스위치 램프
	if (myBtn[62].GetImageBk() != bOn)
		myBtn[62].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiDn.TqClp;	// 검사부 하 텐션 클램프 스위치 램프 
	if (myBtn[63].GetImageBk() != bOn)
		myBtn[63].SetCheck(bOn);

	bOn = pDoc->WorkingInfo.LastJob.bAoiOnePnl = pDoc->BtnStatus.AoiDn.MvOne; // 검사부 하 한판넬 이송
	if (myBtn[64].GetImageBk() != bOn)
		myBtn[64].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiDn.LsrPt;	// 검사부 하 레이져 포인터 스위치 램프
	if (myBtn[65].GetImageBk() != bOn)
		myBtn[65].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiDn.SonicBlw;	// 검사부 하 초음파 세정기 스위치 램프
	if (myBtn[86].GetImageBk() != bOn)
		myBtn[86].SetCheck(bOn);
}

void CDlgMenu03::DispAoiUp()
{
	BOOL bOn;

	// [상면 AOI]
	bOn = pDoc->BtnStatus.AoiUp.Relation;	// 검사부 상 연동 온/오프 스위치 램프
	if (myBtn[17].GetImageBk() != bOn)
		myBtn[17].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiUp.FdCw;	// 검사부 상 피딩 정회전 스위치 램프
	if (myBtn[18].GetImageBk() != bOn)
		myBtn[18].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiUp.FdCcw;	// 검사부 상 피딩 역회전 스위치 램프
	if (myBtn[19].GetImageBk() != bOn)
		myBtn[19].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiUp.FdVac;	// 검사부 상 피딩 진공 스위치 램프
	if (myBtn[20].GetImageBk() != bOn)
		myBtn[20].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiUp.PushUp;	// 검사부 상 제품푸쉬 스위치 램프 // (토크 진공 스위치 램프) - X
	if (myBtn[21].GetImageBk() != bOn)
		myBtn[21].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiUp.TblBlw;	// 검사부 상 테이블 브로워 스위치 램프
	if (myBtn[22].GetImageBk() != bOn)
		myBtn[22].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiUp.TblVac;	// 검사부 상 테이블 진공 스위치 램프
	if (myBtn[23].GetImageBk() != bOn)
		myBtn[23].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiUp.FdClp;	// 검사부 상 피딩 클램프 스위치 램프
	if (myBtn[53].GetImageBk() != bOn)
		myBtn[53].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiUp.TqClp;	// 검사부 상 텐션 클램프 스위치 램프 
	if (myBtn[54].GetImageBk() != bOn)
		myBtn[54].SetCheck(bOn);

	bOn = pDoc->WorkingInfo.LastJob.bAoiOnePnl = pDoc->BtnStatus.AoiUp.MvOne; // 검사부 상 한판넬 이송
	if (myBtn[24].GetImageBk() != bOn)
		myBtn[24].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiUp.LsrPt;	// 검사부 상 레이져 포인터 스위치 램프
	if (myBtn[50].GetImageBk() != bOn)
		myBtn[50].SetCheck(bOn);
}

void CDlgMenu03::DispEngrave()
{
	BOOL bOn;

	bOn = pDoc->BtnStatus.Eng.Relation;	// 각인부 연동 온/오프 스위치 램프 IDC_CHK_72
	if (myBtn[72].GetImageBk() != bOn)
		myBtn[72].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Eng.FdCw;	// 각인부 피딩 정회전 스위치 램프 IDC_CHK_73
	if (myBtn[73].GetImageBk() != bOn)
		myBtn[73].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Eng.FdCcw;	// 각인부 피딩 역회전 스위치 램프 IDC_CHK_74
	if (myBtn[74].GetImageBk() != bOn)
		myBtn[74].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Eng.FdVac;	// 각인부 피딩 진공 스위치 램프 IDC_CHK_75
	if (myBtn[75].GetImageBk() != bOn)
		myBtn[75].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Eng.PushUp;	// 각인부 제품푸쉬 스위치 램프 IDC_CHK_76 // 마킹부 토크 진공 스위치 램프 - X
	if (myBtn[76].GetImageBk() != bOn)
		myBtn[76].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Eng.TblBlw;	// 각인부 테이블 브로워 스위치 램프 IDC_CHK_77
	if (myBtn[77].GetImageBk() != bOn)
		myBtn[77].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Eng.TblVac;	// 각인부 테이블 진공 스위치 램프 IDC_CHK_78
	if (myBtn[78].GetImageBk() != bOn)
		myBtn[78].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Eng.FdClp;	// 각인부 피딩 클램프 스위치 램프 IDC_CHK_82
	if (myBtn[79].GetImageBk() != bOn)
		myBtn[79].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Eng.TqClp;	// 각인부 토크 클램프 스위치 램프 IDC_CHK_83
	if (myBtn[80].GetImageBk() != bOn)
		myBtn[80].SetCheck(bOn);

	bOn = pDoc->WorkingInfo.LastJob.bEngraveOnePnl = pDoc->BtnStatus.Eng.MvOne; // 각인부 한판넬 이송 IDC_CHK_79
	if (myBtn[81].GetImageBk() != bOn)
		myBtn[81].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Eng.LsrPt;	// 각인부 레이져 포인터 스위치 램프 IDC_CHK_81
	if (myBtn[82].GetImageBk() != bOn)
		myBtn[82].SetCheck(bOn);


	bOn = pDoc->BtnStatus.Eng.SonicBlw;	// 각인부 초음파 세정기 스위치 램프 IDC_CHK_87
	if (myBtn[85].GetImageBk() != bOn)
		myBtn[85].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Eng.DcRSol;	// 각인부 댄서롤 상승/하강 스위치 램프 IDC_CHK_80
	if (myBtn[84].GetImageBk() != bOn)
		myBtn[84].SetCheck(bOn);
}

void CDlgMenu03::DispUncoiler()
{
	BOOL bOn;

	// [Uncoiler]
	bOn = pDoc->BtnStatus.Uc.Relation;		//[24] 언코일러 연동 온/오프 스위치 램프
	if (myBtn[25].GetImageBk() != bOn)
		myBtn[25].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Uc.FdCw;		// 언코일러 제품휠 정회전 스위치 램프
	if (myBtn[26].GetImageBk() != bOn)
		myBtn[26].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Uc.FdCcw;		// 언코일러 제품휠 역회전 스위치 램프
	if (myBtn[27].GetImageBk() != bOn)
		myBtn[27].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Uc.ReelChuck;		// 언코일러 제품척 클램프 스위치 램프
	if (myBtn[35].GetImageBk() != bOn)
		myBtn[35].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Uc.DcRlUpDn;		// 언코일러 댄서롤 상승/하강 스위치 램프
	if (myBtn[28].GetImageBk() != bOn)
		myBtn[28].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Uc.ReelJoinL;		// 언코일러 제품 이음매(좌) 스위치 램프
	if (myBtn[30].GetImageBk() != bOn)
		myBtn[30].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Uc.ReelJoinR;		// 언코일러 제품 이음매(우) 스위치 램프
	if (myBtn[37].GetImageBk() != bOn)
		myBtn[37].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Uc.ReelJoinVac;		// 언코일러 제품 이음매 진공 스위치 램프
	if (myBtn[38].GetImageBk() != bOn)
		myBtn[38].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Uc.PprChuck;		// 언코일러 간지척 클램프 스위치 램프
	if (myBtn[31].GetImageBk() != bOn)
		myBtn[31].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Uc.PprCw;		// 언코일러 간지휠 정회전 스위치 램프
	if (myBtn[32].GetImageBk() != bOn)
		myBtn[32].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Uc.PprCcw;		// 언코일러 간지휠 역회전 스위치 램프
	if (myBtn[39].GetImageBk() != bOn)
		myBtn[39].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Uc.ClRlUpDn;		// 언코일러 클린롤러 상승/하강 스위치 램프
	if (myBtn[29].GetImageBk() != bOn)
		myBtn[29].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Uc.ClRlPshUpDn;		// 언코일러 클린롤러누름 상승/하강 스위치 램프
	if (myBtn[36].GetImageBk() != bOn)
		myBtn[36].SetCheck(bOn);


	//bOn = pDoc->m_pMpeIo[nInSeg] & (0x01<<7) ? TRUE : FALSE;		// 언코일러 제품휠 지지 스위치 램프
	//if(myBtn[38].GetImageBk() != bOn)
	//	myBtn[38].SetCheck(bOn);

	//bOn = pDoc->m_pMpeIo[nInSeg] & (0x01<<14) ? TRUE : FALSE;		// 언코일러 제품 EPC원점 스위치 램프
	//if(myBtn[40].GetImageBk() != bOn)
	//	myBtn[40].SetCheck(bOn);
}

void CDlgMenu03::OnTimer(UINT_PTR nIDEvent)//(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	CString strMsg, strTitle;
// 	double dVal;

	//if(nIDEvent == TIM_MENU03_DISP)
	//{
	//	KillTimer(TIM_MENU03_DISP);
	//	Disp();
	//	if(m_bTIM_MENU03_DISP)
	//		SetTimer(TIM_MENU03_DISP, 100, NULL);
	//}
	if(nIDEvent == TIM_CHK_DONE_MKAOI)
	{
		KillTimer(TIM_CHK_DONE_MKAOI);

#ifdef USE_MPE
		if(!(pDoc->m_pMpeSignal[5] & (0x01<<1)) && !(pDoc->m_pMpeSignal[5] & (0x01<<0)))	// 마킹부 피딩 ON (PLC가 피딩완료 후 OFF) && 검사부 피딩 ON (PLC가 피딩완료 후 OFF)
		{
			m_bTIM_CHK_DONE_MKAOI = FALSE;
		}
#endif
		if(m_bTIM_CHK_DONE_MKAOI)
			SetTimer(TIM_CHK_DONE_MKAOI, 100, NULL);
	}
	if(nIDEvent == TIM_CHK_DONE_MK)
	{
		KillTimer(TIM_CHK_DONE_MK);

#ifdef USE_MPE
		if(!(pDoc->m_pMpeSignal[5] & (0x01<<1)))	// 마킹부 피딩 ON (PLC가 피딩완료 후 OFF)
			m_bTIM_CHK_DONE_MK = FALSE;
#endif
		if(m_bTIM_CHK_DONE_MK)
			SetTimer(TIM_CHK_DONE_MK, 100, NULL);
	}
	if(nIDEvent == TIM_CHK_DONE_AOI)
	{
		KillTimer(TIM_CHK_DONE_AOI);

#ifdef USE_MPE
		if(!(pDoc->m_pMpeSignal[5] & (0x01<<0)))	// 검사부 피딩 ON (PLC가 피딩완료 후 OFF)
			m_bTIM_CHK_DONE_AOI = FALSE;
#endif
		if(m_bTIM_CHK_DONE_AOI)
			SetTimer(TIM_CHK_DONE_AOI, 100, NULL);
	}

	if(nIDEvent == TIM_CHK_MREG)
	{
		KillTimer(TIM_CHK_MREG);
		pView->ChkMRegOut();
	}
	
	if(nIDEvent == TIM_CHK_DONE_BUF_HOME)
	{
		KillTimer(TIM_CHK_DONE_BUF_HOME);

#ifdef USE_MPE
		if(pDoc->m_pMpeSignal[4] & (0x01<<2))	// 마킹부 버퍼롤러 홈동작 ON (PLC가 홈동작 완료 후 OFF)
		{
			pView->GetDispMsg(strMsg, strTitle);
			if(strMsg != _T("Searching Buffer Home Position...") || strTitle != _T("Homming"))
				pView->DispMsg(_T("Searching Buffer Home Position..."), _T("Homming"), RGB_GREEN, DELAY_TIME_MSG);
		}
		else
		{
			m_bTIM_CHK_DONE_BUF_HOME = FALSE;
			pView->m_bBufHomeDone = TRUE;
			pView->ClrDispMsg();
		}
#endif
		if(m_bTIM_CHK_DONE_BUF_HOME)
			SetTimer(TIM_CHK_DONE_BUF_HOME, 100, NULL);
	}
	
	if(nIDEvent == TIM_CHK_DONE_BUF_INIT)
	{
		KillTimer(TIM_CHK_DONE_BUF_INIT);

#ifdef USE_MPE
		if(pDoc->m_pMpeSignal[4] & (0x01<<10))	// 마킹부 버퍼 초기위치 이동(PC가 ON, PLC가 OFF)
		{
			pView->GetDispMsg(strMsg, strTitle);
			if(strMsg != _T("Searching Buffer Initial Position...") || strTitle != _T("Moving"))
				pView->DispMsg(_T("Searching Buffer Initial Position..."), _T("Moving"), RGB_GREEN, DELAY_TIME_MSG);
		}
		else
		{
			m_bTIM_CHK_DONE_BUF_INIT = FALSE;
			pView->ClrDispMsg();
		}
#endif
		if(m_bTIM_CHK_DONE_BUF_INIT)
			SetTimer(TIM_CHK_DONE_BUF_INIT, 100, NULL);
	}
	
	if(nIDEvent == TIM_CHK_DONE_READY)
	{
		KillTimer(TIM_CHK_DONE_READY);

#ifdef USE_MPE
		if(pDoc->m_pMpeSignal[0] & (0x01<<0))	// PLC 운전준비 완료(PC가 확인하고 Reset시킴.)
		{
			m_bTIM_CHK_DONE_READY = FALSE;
			//pView->ClrDispMsg();
			pView->m_bReadyDone = TRUE;
			if(pView->m_pMpe)
				pView->m_pMpe->Write(_T("MB440100"), 0);	// PLC 운전준비 완료(PC가 확인하고 Reset시킴.)
		}
		else
		{
			pView->GetDispMsg(strMsg, strTitle);
			if(strMsg != _T("Searching Buffer Home Position...") || strTitle != _T("Homming"))
			{
				pView->m_bReadyDone = FALSE;
				//pView->MsgBox("Searching Buffer Home Position...");
				//pView->DispMsg(_T("Searching Buffer Home Position..."), _T("Homming"), RGB_GREEN, DELAY_TIME_MSG);
			}
		}
#endif
		if(m_bTIM_CHK_DONE_READY)
			SetTimer(TIM_CHK_DONE_READY, 100, NULL);
	}
	
// 	if(nIDEvent == TIM_SET_RC_RELATION)
// 	{
// 		KillTimer(TIM_SET_RC_RELATION);
// 		if(pView->m_pMpe)
// 		{
// 			pView->m_pMpe->Write(_T("MB004001", 1);
// 			Sleep(60);
// 			pView->m_pMpe->Write(_T("MB004001", 0);
// 		}
// 	}
	
// 	if(nIDEvent == TIM_SW_RC_RELATION)
// 	{
// 		KillTimer(TIM_SW_RC_RELATION);
// 		SwRcRelationTim();
// 	}
// 	
// 	if(nIDEvent == TIM_SW_UC_RELATION)
// 	{
// 		KillTimer(TIM_SW_UC_RELATION);
// 		SwUcRelationTim();
// 	}

	if(nIDEvent == TIM_AOI_RESET_OFF)
	{
		KillTimer(TIM_AOI_RESET_OFF);
		SwAoiReset(FALSE);
	}

	if(nIDEvent == TIM_AOI_LOTEND_OFF)
	{
		KillTimer(TIM_AOI_LOTEND_OFF);
		SwAoiLotEnd(FALSE);
	}

// 	if(nIDEvent == TIM_AOI_EMG_SW)
// 	{
// 		KillTimer(TIM_AOI_EMG_SW);
// 		SwAoiEmg(FALSE);
// 	}

	if (nIDEvent == TIM_CHK_DONE_ENGRAVE)
	{
		KillTimer(TIM_CHK_DONE_ENGRAVE);

#ifdef USE_MPE
		if (!(pDoc->m_pMpeSignal[6] & (0x01 << 12)))	// 각인부 피딩 CW ON (PLC가 피딩완료 후 OFF) - MB44017C
			m_bTIM_CHK_DONE_ENGRAVE = FALSE;
#endif
		if (m_bTIM_CHK_DONE_ENGRAVE)
			SetTimer(TIM_CHK_DONE_ENGRAVE, 100, NULL);
	}

	CDialog::OnTimer(nIDEvent);
}

LRESULT CDlgMenu03::OnMyBtnDown(WPARAM wPara, LPARAM lPara)
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
	
	int nCtrlID = (int)lPara;

 	SwMyBtnDown(nCtrlID);
	return 0L;
}

LRESULT CDlgMenu03::OnMyBtnUp(WPARAM wPara, LPARAM lPara)
{
	int nCtrlID = (int)lPara;

	SwMyBtnUp(nCtrlID);
	return 0L;
}

void CDlgMenu03::SwMyBtnDown(int nCtrlID)
{
	if (!pView)
		return;

	long lData = 1; BOOL bData = TRUE;

	if (nCtrlID != IDC_CHK_33 && nCtrlID != IDC_CHK_0)		// 마킹부 정지 스위치 , 마킹부 운전 스위치
	{
		if (!pDoc->Status.bManual)
		{
			pView->DispMsg(_T("수동 모드가 아닙니다."), _T("주의"), RGB_YELLOW);
			return;
		}
	}

#ifdef USE_ENGRAVE
	if (!pView->m_pEngrave)
		return;
#endif

#ifdef USE_MPE
	if (!pView->m_pMpe)
		return;
#endif

	SwMpeBtn(nCtrlID, lData);
	SwEngraveBtn(nCtrlID, bData);
}

void CDlgMenu03::SwMyBtnUp(int nCtrlID)
{
	if (!pView)
		return;

	long lData = 0; BOOL bData = FALSE;

	if (nCtrlID != IDC_CHK_33 && nCtrlID != IDC_CHK_0)		// 마킹부 정지 스위치 , 마킹부 운전 스위치
	{
		if (!pDoc->Status.bManual)
		{
			pView->DispMsg(_T("수동 모드가 아닙니다."), _T("주의"), RGB_YELLOW);
			return;
		}
	}

#ifdef USE_ENGRAVE
	if (!pView->m_pEngrave)
		return;
#endif

#ifdef USE_MPE
	if (!pView->m_pMpe)
		return;
#endif

	SwMpeBtn(nCtrlID, lData);
	SwEngraveBtn(nCtrlID, bData);
}

void CDlgMenu03::SwMpeBtn(int nCtrlID, long lData)
{
#ifdef USE_MPE	
	switch (nCtrlID)
	{
	// Main
	case IDC_CHK_34:		// 마킹부 운전준비 스위치
		pView->m_pMpe->Write(_T("MB005503"), lData);
		break;
	case IDC_CHK_0:			// 마킹부 운전 스위치
		pView->m_pMpe->Write(_T("MB005501"), lData);
		break;
	case IDC_CHK_1:			// 마킹부 리셋 스위치	
		pView->m_pMpe->Write(_T("MB005504"), lData);
		break;
	case IDC_CHK_33:		// 마킹부 정지 스위치
		pView->m_pMpe->Write(_T("MB005502"), lData);
		break;

		// Recoiler
	case IDC_CHK_4:			// 리코일러 연동 온/오프 스위치
		pView->m_pMpe->Write(_T("MB005801"), lData);
		break;
	case IDC_CHK_5:			// 리코일러 제품휠 정회전 스위치
		pView->m_pMpe->Write(_T("MB00580C"), lData);
		break;
	case IDC_CHK_6:			// 리코일러 제품휠 역회전 스위치
		pView->m_pMpe->Write(_T("MB00580D"), lData);
		break;
	case IDC_CHK_41:		// 리코일러 제품척 클램프 스위치
		pView->m_pMpe->Write(_T("MB00580B"), lData);
		break;
	case IDC_CHK_42:		// 리코일러 댄서롤 상승/하강 스위치
		pView->m_pMpe->Write(_T("MB005802"), lData);
		break;
	case IDC_CHK_43:		// 리코일러 제품 이음매(상/좌) 스위치
		pView->m_pMpe->Write(_T("MB005805"), lData);
		break;
	case IDC_CHK_7:			// 리코일러 제품 이음매(하/우) 스위치
		pView->m_pMpe->Write(_T("MB005806"), lData);
		break;
	case IDC_CHK_8:			// 리코일러 제품 이음매 진공 스위치
		pView->m_pMpe->Write(_T("MB00580F"), lData);
		break;
	case IDC_CHK_44:		// 리코일러 간지척 클램프 스위치
		pView->m_pMpe->Write(_T("MB005808"), lData);
		break;
	case IDC_CHK_45:		// 리코일러 간지휠 정회전 스위치
		pView->m_pMpe->Write(_T("MB005809"), lData);
		break;
	case IDC_CHK_46:		// 리코일러 간지휠 역회전 스위치
		pView->m_pMpe->Write(_T("MB00580A"), lData);
		break;
	case IDC_CHK_66:		// 리코일러 Rewinder 동작 스위치
		pView->m_pMpe->Write(_T("MB005803"), lData);
		break;
	case IDC_CHK_67:		// 리코일러 Rewinder 제품 & 간지 스위치
		pView->m_pMpe->Write(_T("MB005804"), lData);
		break;

		// Marking
	case IDC_CHK_9:			// 마킹부 연동 온/오프 스위치
		pView->m_pMpe->Write(_T("MB005511"), lData);
		break;
	case IDC_CHK_10:		// 마킹부 피딩 정회전 스위치
		pView->m_pMpe->Write(_T("MB005513"), lData);
		break;
	case IDC_CHK_11:		// 마킹부 피딩 역회전 스위치
		pView->m_pMpe->Write(_T("MB005514"), lData);
		break;
	case IDC_CHK_12:		// 마킹부 피딩 진공 스위치
		pView->m_pMpe->Write(_T("MB005515"), lData);
		break;
	case IDC_CHK_13:		// 마킹부 제품푸쉬 스위치 // (토크 진공 스위치) - X
		pView->m_pMpe->Write(_T("MB005516"), lData);
		break;
	case IDC_CHK_14:		// 마킹부 테이블 브로워 스위치
		pView->m_pMpe->Write(_T("MB005512"), lData);
		break;
	case IDC_CHK_15:		// 마킹부 테이블 진공 스위치
		pView->m_pMpe->Write(_T("MB005517"), lData);
		break;
	case IDC_CHK_51:		// 마킹부 피딩 클램프 스위치
		pView->m_pMpe->Write(_T("MB005519"), lData);
		break;
	case IDC_CHK_52:		// 마킹부 텐션 클램프 스위치
		pView->m_pMpe->Write(_T("MB00551A"), lData);
		break;

		// AOI 하면 검사	
	case IDC_CHK_55:		// 검사부 하 연동 온/오프 스위치
		pView->m_pMpe->Write(_T("MB005701"), lData);
		break;
	case IDC_CHK_56:		// 검사부 하 피딩 정회전 스위치
		pView->m_pMpe->Write(_T("MB005703"), lData);
		break;
	case IDC_CHK_57:		// 검사부 하 피딩 역회전 스위치
		pView->m_pMpe->Write(_T("MB005704"), lData);
		break;
	case IDC_CHK_58:		// 검사부 하 피딩 진공 스위치
		pView->m_pMpe->Write(_T("MB005705"), lData);
		break;
	case IDC_CHK_59:		// 검사부 하 제품푸쉬 스위치 // (토크 진공 스위치) - X
		pView->m_pMpe->Write(_T("MB005706"), lData);
		break;
	case IDC_CHK_60:		// 검사부 하 테이블 브로워 스위치
		pView->m_pMpe->Write(_T("MB005702"), lData);
		break;
	case IDC_CHK_61:		// 검사부 하 테이블 진공 스위치
		pView->m_pMpe->Write(_T("MB005707"), lData);
		break;
	case IDC_CHK_64:		// 검사부 하 피딩 클램프 스위치
		pView->m_pMpe->Write(_T("MB005709"), lData);
		break;
	case IDC_CHK_65:		// 검사부 하 텐션 클램프 스위치
		pView->m_pMpe->Write(_T("MB00570A"), lData);
		break;

		// AOI 상면 검사
	case IDC_CHK_17:		// 검사부 상 연동 온/오프 스위치
		pView->m_pMpe->Write(_T("MB005601"), lData);
		break;
	case IDC_CHK_18:		// 검사부 상 피딩 정회전 스위치
		pView->m_pMpe->Write(_T("MB005603"), lData);
		break;
	case IDC_CHK_19:		// 검사부 상 피딩 역회전 스위치
		pView->m_pMpe->Write(_T("MB005604"), lData);
		break;
	case IDC_CHK_20:		// 검사부 상 피딩 진공 스위치
		pView->m_pMpe->Write(_T("MB005605"), lData);
		break;
	case IDC_CHK_21:		// 검사부 상 제품푸쉬 스위치 // (토크 진공 스위치) - X
		pView->m_pMpe->Write(_T("MB005606"), lData);
		break;
	case IDC_CHK_22:		// 검사부 상 테이블 브로워 스위치
		pView->m_pMpe->Write(_T("MB005602"), lData);
		break;
	case IDC_CHK_23:		// 검사부 상 테이블 진공 스위치
		pView->m_pMpe->Write(_T("MB005607"), lData);
		break;
	case IDC_CHK_53:		// 검사부 상 피딩 클램프 스위치
		pView->m_pMpe->Write(_T("MB005609"), lData);
		break;
	case IDC_CHK_54:		// 검사부 상 텐션 클램프 스위치
		pView->m_pMpe->Write(_T("MB00560A"), lData);
		break;

		// Uncoiler
	case IDC_CHK_25:		// 언코일러 연동 온/오프 스위치
		pView->m_pMpe->Write(_T("MB005401"), lData);
		break;
	case IDC_CHK_26:		// 언코일러 제품휠 정회전 스위치
		pView->m_pMpe->Write(_T("MB00540C"), lData);
		break;
	case IDC_CHK_27:		// 언코일러 제품휠 역회전 스위치
		pView->m_pMpe->Write(_T("MB00540D"), lData);
		break;
	case IDC_CHK_35:		// 언코일러 제품척 클램프 스위치
		pView->m_pMpe->Write(_T("MB00540B"), lData);
		break;
	case IDC_CHK_28:		// 언코일러 댄서롤 상승/하강 스위치
		pView->m_pMpe->Write(_T("MB005402"), lData);
		break;
	case IDC_CHK_30:		// 언코일러 제품 이음매(상/좌) 스위치
		pView->m_pMpe->Write(_T("MB005405"), lData);
		break;
	case IDC_CHK_37:		// 언코일러 제품 이음매(하/우) 스위치
		pView->m_pMpe->Write(_T("MB005406"), lData);
		break;
	case IDC_CHK_38:		// 언코일러 제품 이음매 진공 스위치
		pView->m_pMpe->Write(_T("MB00540F"), lData);
		break;
	case IDC_CHK_31:		// 언코일러 간지척 클램프 스위치
		pView->m_pMpe->Write(_T("MB005408"), lData);
		break;
	case IDC_CHK_32:		// 언코일러 간지휠 정회전 스위치
		pView->m_pMpe->Write(_T("MB005409"), lData);
		break;
	case IDC_CHK_39:		// 언코일러 간지휠 역회전 스위치
		pView->m_pMpe->Write(_T("MB00540A"), lData);
		break;
	case IDC_CHK_29:		// 언코일러 클린롤러 상승/하강 스위치
		pView->m_pMpe->Write(_T("MB005403"), lData);
		break;
	case IDC_CHK_36:		// 언코일러 클린롤러누름 상승/하강 스위치
		pView->m_pMpe->Write(_T("MB005404"), lData);
		break;

	// Torque Motor
	case IDC_CHK_2:		// 마킹부 텐션 ON (PC가 ON/OFF시킴)
		if (!(pDoc->m_pMpeSignal[4] & (0x01 << 5)))
		{
			pDoc->WorkingInfo.Motion.bMkTq = TRUE;
			pView->m_pMpe->Write(_T("MB440155"), 1);
		}
		else
		{
			pDoc->WorkingInfo.Motion.bMkTq = FALSE;
			pView->m_pMpe->Write(_T("MB440155"), 0);
		}
		break;
	case IDC_CHK_3:		// 검사부 텐션 ON (PC가 ON/OFF시킴)
		if (!(pDoc->m_pMpeSignal[4] & (0x01 << 6)))
		{
			pDoc->WorkingInfo.Motion.bAoiTq = TRUE;
			pView->m_pMpe->Write(_T("MB440156"), 1);
		}
		else
		{
			pDoc->WorkingInfo.Motion.bAoiTq = FALSE;
			pView->m_pMpe->Write(_T("MB440156"), 0);
		}
		break;
	case IDC_CHK_84:	// 각인부 텐션 ON (PC가 ON/OFF시킴)
		if (!(pDoc->m_pMpeSignal[4] & (0x01 << 4)))
		{
			pDoc->WorkingInfo.Motion.bEngraveTq = TRUE;
			pView->m_pMpe->Write(_T("MB440154"), 1);
		}
		else
		{
			pDoc->WorkingInfo.Motion.bEngraveTq = FALSE;
			pView->m_pMpe->Write(_T("MB440154"), 0);
		}
		break;

		// [각인부 세정기]
	case IDC_CHK_87:	// 각인부 세정기 ON (PC가 ON/OFF시킴)
		if (!(pDoc->WorkingInfo.LastJob.bEngraveCleanner))
		{
			pDoc->WorkingInfo.LastJob.bEngraveCleanner = TRUE;
			pView->m_pMpe->Write(_T("MB44014E"), 1);
		}
		else
		{
			pDoc->WorkingInfo.LastJob.bEngraveCleanner = FALSE;
			pView->m_pMpe->Write(_T("MB44014E"), 0);
		}
		break;
		//if (!(pDoc->m_pMpeSignal[5] & (0x01 << 14)))
		//{
		//	pDoc->WorkingInfo.LastJob.bEngraveCleanner = TRUE;
		//	pView->m_pMpe->Write(_T("MB44014E"), 1);
		//}
		//else
		//{
		//	pDoc->WorkingInfo.LastJob.bEngraveCleanner = FALSE;
		//	pView->m_pMpe->Write(_T("MB44014E"), 0);
		//}
		//break;

		// [AOI(하) 세정기]
	case IDC_CHK_88:	// AOI(하) 세정기 ON (PC가 ON/OFF시킴)
		if (!(pDoc->WorkingInfo.LastJob.bAoiDnCleanner))
		{
			pDoc->WorkingInfo.LastJob.bAoiDnCleanner = TRUE;
			pView->m_pMpe->Write(_T("MB44014F"), 1);
		}
		else
		{
			pDoc->WorkingInfo.LastJob.bAoiDnCleanner = FALSE;
			pView->m_pMpe->Write(_T("MB44014F"), 0);
		}
		break;
		//if (!(pDoc->m_pMpeSignal[5] & (0x01 << 15)))
		//{
		//	pDoc->WorkingInfo.LastJob.bAoiDnCleanner = TRUE;
		//	pView->m_pMpe->Write(_T("MB44014F"), 1);
		//}
		//else
		//{
		//	pDoc->WorkingInfo.LastJob.bAoiDnCleanner = FALSE;
		//	pView->m_pMpe->Write(_T("MB44014F"), 0);
		//}
		//break;


		// [One Metal]
	case IDC_CHK_68:
		pView->SetTwoMetal(FALSE, TRUE);
		break;

		// [Two Metal]
	case IDC_CHK_69:
		pView->SetTwoMetal(TRUE, TRUE);
		break;

		// [Core 150mm] - Recoiler
	case IDC_CHK_70:
		SetCore150mmRecoiler(TRUE);
		break;

		// [Core 150mm] - Uncoiler
	case IDC_CHK_71:
		SetCore150mmUncoiler(TRUE);
		break;
	}
#else
	switch (nCtrlID)
	{
		// Torque Motor
	case IDC_CHK_2:		// 마킹부 텐션 ON (PC가 ON/OFF시킴)
		if (!pDoc->WorkingInfo.Motion.bMkTq)
			pDoc->WorkingInfo.Motion.bMkTq = TRUE;
		else
			pDoc->WorkingInfo.Motion.bMkTq = FALSE;
		break;
	case IDC_CHK_3:		// 검사부 텐션 ON (PC가 ON/OFF시킴)
		if (!pDoc->WorkingInfo.Motion.bAoiTq)
			pDoc->WorkingInfo.Motion.bAoiTq = TRUE;
		else
			pDoc->WorkingInfo.Motion.bAoiTq = FALSE;
		break;
	case IDC_CHK_84:	// 각인부 텐션 ON (PC가 ON/OFF시킴)
		if (!pDoc->WorkingInfo.Motion.bEngraveTq)
			pDoc->WorkingInfo.Motion.bEngraveTq = TRUE;
		else
			pDoc->WorkingInfo.Motion.bEngraveTq = FALSE;
		break;

		// [각인부 세정기]
	case IDC_CHK_87:	// 각인부 세정기 ON (PC가 ON/OFF시킴)
		if (!pDoc->WorkingInfo.LastJob.bEngraveCleanner)
			pDoc->WorkingInfo.LastJob.bEngraveCleanner = TRUE;
		else
			pDoc->WorkingInfo.LastJob.bEngraveCleanner = FALSE;
		break;

		// [AOI(하) 세정기]
	case IDC_CHK_88:	// AOI(하) 세정기 ON (PC가 ON/OFF시킴)
		if (!pDoc->WorkingInfo.LastJob.bAoiDnCleanner)
			pDoc->WorkingInfo.LastJob.bAoiDnCleanner = TRUE;
		else
			pDoc->WorkingInfo.LastJob.bAoiDnCleanner = FALSE;
		break;
	}
#endif
}

void CDlgMenu03::SwEngraveBtn(int nCtrlID, BOOL bOn)
{
#ifdef USE_ENGRAVE
	switch (nCtrlID)
	{
	// [Main]
	case IDC_CHK_34:		// 마킹부 운전준비 스위치 "MB005503"
		pView->m_pEngrave->SwReady(bOn);
		pDoc->BtnStatus.Main.Ready = bOn;
		break;
	case IDC_CHK_0:			// 마킹부 운전 스위치 "MB005501"
		pView->m_pEngrave->SwRun(bOn);
		pDoc->BtnStatus.Main.Run = bOn;
		break;
	case IDC_CHK_1:			// 마킹부 리셋 스위치 "MB005504"
		pView->m_pEngrave->SwReset(bOn);
		pDoc->BtnStatus.Main.Reset = bOn;
		break;
	case IDC_CHK_33:		// 마킹부 정지 스위치 "MB005502"
		pView->m_pEngrave->SwStop(bOn);
		pDoc->BtnStatus.Main.Stop = bOn;
		break;
	//case IDC_CHK_:		// 마킹부 자동(ON)/수동(OFF) 스위치 "MB005505"
	//	pView->m_pEngrave->SwAuto(bOn);
	//	pView->m_pEngrave->SwManual(!bOn);
	//	pDoc->BtnStatus.Main.Auto = bOn;
	//	pDoc->BtnStatus.Main.Manual = !bOn;
	//	break;

	// [Torque Motor]
	case IDC_CHK_2:		// 마킹부 텐션 ON (PC가 ON/OFF시킴) "MB440155"
		pView->m_pEngrave->SwMkTq(bOn); // pDoc->WorkingInfo.Motion.bMkTq
		pDoc->WorkingInfo.Motion.bMkTq = pDoc->BtnStatus.Tq.Mk = bOn;
		break;
	case IDC_CHK_3:		// 검사부 텐션 ON (PC가 ON/OFF시킴) "MB440156"
		pView->m_pEngrave->SwAoiTq(bOn); // pDoc->WorkingInfo.Motion.bAoiTq
		pDoc->WorkingInfo.Motion.bAoiTq = pDoc->BtnStatus.Tq.Aoi = bOn;
		break;
	case IDC_CHK_84:	// 각인부 텐션 ON (PC가 ON/OFF시킴) "MB440154"
		pView->m_pEngrave->SwEngTq(bOn); // pDoc->WorkingInfo.Motion.bEngraveTq
		pDoc->WorkingInfo.Motion.bEngraveTq = pDoc->BtnStatus.Tq.Eng = bOn;
		break;

	// [Induction Motor}
	case IDC_CHK_68:	// One Metal		//pView->SetTwoMetal(FALSE, TRUE);
		pView->m_pEngrave->SwRcInductionCcw(bOn);
		pDoc->WorkingInfo.LastJob.bOneMetal = pDoc->BtnStatus.Induct.Rc = bOn;
		break;

	case IDC_CHK_69:	// Two Metal		//pView->SetTwoMetal(TRUE, TRUE);
		pView->m_pEngrave->SwUcInductionCcw(bOn);
		pDoc->WorkingInfo.LastJob.bTwoMetal = pDoc->BtnStatus.Induct.Uc = bOn;
		break;

	// [Core 150mm]
	case IDC_CHK_70:	// Recoiler		//SetCore150mmRecoiler(TRUE);
		pView->m_pEngrave->SwRcCore150mm(bOn);
		pDoc->BtnStatus.Core150.Rc = bOn;
		break;

	case IDC_CHK_71:	// Uncoiler		//SetCore150mmUncoiler(TRUE);
		pView->m_pEngrave->SwUcCore150mm(bOn);
		pDoc->BtnStatus.Core150.Uc = bOn;
		break;

	// Recoiler
	case IDC_CHK_4:			// 리코일러 연동 온/오프 스위치 "MB005801"
		pView->m_pEngrave->SwRcRelation(bOn);
		pDoc->BtnStatus.Rc.Relation = bOn;
		break;
	case IDC_CHK_5:			// 리코일러 제품휠 정회전 스위치 "MB00580C"
		pView->m_pEngrave->SwRcFdCw(bOn);
		pDoc->BtnStatus.Rc.FdCw = bOn;
		break;
	case IDC_CHK_6:			// 리코일러 제품휠 역회전 스위치 "MB00580D"
		pView->m_pEngrave->SwRcFdCcw(bOn);
		pDoc->BtnStatus.Rc.FdCcw = bOn;
		break;
	case IDC_CHK_41:		// 리코일러 제품척 클램프 스위치 "MB00580B"
		pView->m_pEngrave->SwRcReelChuck(bOn);
		pDoc->BtnStatus.Rc.ReelChuck = bOn;
		break;
	case IDC_CHK_42:		// 리코일러 댄서롤 상승/하강 스위치 "MB005802"
		pView->m_pEngrave->SwRcDcRlUpDn(bOn);
		pDoc->BtnStatus.Rc.DcRlUpDn = bOn;
		break;
	case IDC_CHK_43:		// 리코일러 제품 이음매(상/좌) 스위치 "MB005805"
		pView->m_pEngrave->SwRcReelJoinL(bOn);
		pDoc->BtnStatus.Rc.ReelJoinL = bOn;
		break;
	case IDC_CHK_7:			// 리코일러 제품 이음매(하/우) 스위치 "MB005806"
		pView->m_pEngrave->SwRcReelJoinR(bOn);
		pDoc->BtnStatus.Rc.ReelJoinR = bOn;
		break;
	case IDC_CHK_8:			// 리코일러 제품 이음매 진공 스위치 "MB00580F"
		pView->m_pEngrave->SwRcReelJoinVac(bOn);
		pDoc->BtnStatus.Rc.ReelJoinVac = bOn;
		break;
	case IDC_CHK_44:		// 리코일러 간지척 클램프 스위치 "MB005808"
		pView->m_pEngrave->SwRcPaperChuck(bOn);
		pDoc->BtnStatus.Rc.PprChuck = bOn;
		break;
	case IDC_CHK_45:		// 리코일러 간지휠 정회전 스위치 "MB005809"
		pView->m_pEngrave->SwRcPaperCw(bOn);
		pDoc->BtnStatus.Rc.PprCw = bOn;
		break;
	case IDC_CHK_46:		// 리코일러 간지휠 역회전 스위치 "MB00580A"
		pView->m_pEngrave->SwRcPaperCcw(bOn);
		pDoc->BtnStatus.Rc.PprCcw = bOn;
		break;
	case IDC_CHK_66:		// 리코일러 Rewinder 동작 스위치 "MB005803"
		pView->m_pEngrave->SwRcRewinder(bOn);
		pDoc->BtnStatus.Rc.Rewine = bOn;
		break;
	case IDC_CHK_67:		// 리코일러 Rewinder 제품 & 간지 스위치 "MB005804"
		pView->m_pEngrave->SwRcRewinderReelPaper(bOn);
		pDoc->BtnStatus.Rc.RewineReelPpr = bOn;
		break;

	// Punching
	case IDC_CHK_9:			// 마킹부 연동 온/오프 스위치 "MB005511"
		pView->m_pEngrave->SwMkRelation(bOn);
		pDoc->BtnStatus.Mk.Relation = bOn;
		break;
	case IDC_CHK_10:		// 마킹부 피딩 정회전 스위치 "MB005513"
		pView->m_pEngrave->SwMkFdCw(bOn);
		pDoc->BtnStatus.Mk.FdCw = bOn;
		break;
	case IDC_CHK_11:		// 마킹부 피딩 역회전 스위치 "MB005514"
		pView->m_pEngrave->SwMkFdCcw(bOn);
		pDoc->BtnStatus.Mk.FdCcw = bOn;
		break;
	case IDC_CHK_12:		// 마킹부 피딩 진공 스위치 "MB005515"
		pView->m_pEngrave->SwMkFdVac(bOn);
		pDoc->BtnStatus.Mk.FdVac = bOn;
		break;
	case IDC_CHK_13:		// 마킹부 제품푸쉬 스위치 "MB005516" // (토크 진공 스위치) - X
		pView->m_pEngrave->SwMkPushUp(bOn);
		pDoc->BtnStatus.Mk.PushUp = bOn;
		break;
	case IDC_CHK_14:		// 마킹부 테이블 브로워 스위치 "MB005512"
		pView->m_pEngrave->SwMkTblBlw(bOn);
		pDoc->BtnStatus.Mk.TblBlw = bOn;
		break;
	case IDC_CHK_15:		// 마킹부 테이블 진공 스위치 "MB005517"
		pView->m_pEngrave->SwMkTblVac(bOn);
		pDoc->BtnStatus.Mk.TblVac = bOn;
		break;
	case IDC_CHK_51:		// 마킹부 피딩 클램프 스위치 "MB005519"
		pView->m_pEngrave->SwMkFdClp(bOn);
		pDoc->BtnStatus.Mk.FdClp = bOn;
		break;
	case IDC_CHK_52:		// 마킹부 텐션 클램프 스위치 "MB00551A"
		pView->m_pEngrave->SwMkTqClp(bOn);
		pDoc->BtnStatus.Mk.TqClp = bOn;
		break;
	case IDC_CHK_16:		// 마킹부 한판넬 이송 스위치 "MB440151"
		pView->m_pEngrave->SwMkMvOne(bOn);
		pDoc->BtnStatus.Mk.MvOne = bOn;
		break;
	case IDC_CHK_49:		// 마킹부 레이져마크 스위치 "MB005518"
		pView->m_pEngrave->SwMkLsrPt(bOn);
		pDoc->BtnStatus.Mk.LsrPt = bOn;
		break;
	case IDC_CHK_48:		// 마킹부 댄서롤 상승/하강 스위치 "MB00551B", "X421B"
		pView->m_pEngrave->SwMkDcRSol(bOn);
		pDoc->BtnStatus.Mk.DcRSol = bOn;
		break;

	// AOI 하면 검사	
	case IDC_CHK_55:		// 검사부 하 연동 온/오프 스위치 "MB005701"
		pView->m_pEngrave->SwAoiDnRelation(bOn);
		pDoc->BtnStatus.AoiDn.Relation = bOn;
		break;
	case IDC_CHK_56:		// 검사부 하 피딩 정회전 스위치 "MB005703"
		pView->m_pEngrave->SwAoiDnFdCw(bOn);
		pDoc->BtnStatus.AoiDn.FdCw = bOn;
		break;
	case IDC_CHK_57:		// 검사부 하 피딩 역회전 스위치 "MB005704"
		pView->m_pEngrave->SwAoiDnFdCcw(bOn);
		pDoc->BtnStatus.AoiDn.FdCcw = bOn;
		break;
	case IDC_CHK_58:		// 검사부 하 피딩 진공 스위치 "MB005705"
		pView->m_pEngrave->SwAoiDnFdVac(bOn);
		pDoc->BtnStatus.AoiDn.FdVac = bOn;
		break;
	case IDC_CHK_59:		// 검사부 하 제품푸쉬 스위치 "MB005706" // (토크 진공 스위치) - X
		pView->m_pEngrave->SwAoiDnPushUp(bOn);
		pDoc->BtnStatus.AoiDn.PushUp = bOn;
		break;
	case IDC_CHK_60:		// 검사부 하 테이블 브로워 스위치 "MB005702"
		pView->m_pEngrave->SwAoiDnTblBlw(bOn);
		pDoc->BtnStatus.AoiDn.TblBlw = bOn;
		break;
	case IDC_CHK_61:		// 검사부 하 테이블 진공 스위치 "MB005707"
		pView->m_pEngrave->SwAoiDnTblVac(bOn);
		pDoc->BtnStatus.AoiDn.TblVac = bOn;
		break;
	case IDC_CHK_64:		// 검사부 하 피딩 클램프 스위치 "MB005709"
		pView->m_pEngrave->SwAoiDnFdClp(bOn);
		pDoc->BtnStatus.AoiDn.FdClp = bOn;
		break;
	case IDC_CHK_65:		// 검사부 하 텐션 클램프 스위치 "MB00570A"
		pView->m_pEngrave->SwAoiDnTqClp(bOn);
		pDoc->BtnStatus.AoiDn.TqClp = bOn;
		break;
	case IDC_CHK_62:		// 검사부 하 한판넬 이송 스위치 "MB440151"
		pView->m_pEngrave->SwAoiDnMvOne(bOn);
		pDoc->BtnStatus.AoiDn.MvOne = bOn;
		break;
	case IDC_CHK_63:		// 검사부 하 레이져마크 스위치 "MB005708"
		pView->m_pEngrave->SwAoiDnLsrPt(bOn);
		pDoc->BtnStatus.AoiDn.LsrPt = bOn;
		break;
	case IDC_CHK_88:		// 검사부 하 초음파 세정기 스위치 "MB44014F" //pDoc->WorkingInfo.LastJob.bAoiDnCleanner
		pView->m_pEngrave->SwAoiDnSonicBlw(bOn);
		pDoc->BtnStatus.AoiDn.SonicBlw = bOn;
		break;
		

	// AOI 상면 검사
	case IDC_CHK_17:		// 검사부 상 연동 온/오프 스위치 "MB005601"
		pView->m_pEngrave->SwAoiUpRelation(bOn);
		pDoc->BtnStatus.AoiUp.Relation = bOn;
		break;
	case IDC_CHK_18:		// 검사부 상 피딩 정회전 스위치 "MB005603"
		pView->m_pEngrave->SwAoiUpFdCw(bOn);
		pDoc->BtnStatus.AoiUp.FdCw = bOn;
		break;
	case IDC_CHK_19:		// 검사부 상 피딩 역회전 스위치 "MB005604"
		pView->m_pEngrave->SwAoiUpFdCcw(bOn);
		pDoc->BtnStatus.AoiUp.FdCcw = bOn;
		break;
	case IDC_CHK_20:		// 검사부 상 피딩 진공 스위치 "MB005605"
		pView->m_pEngrave->SwAoiUpFdVac(bOn);
		pDoc->BtnStatus.AoiUp.FdVac = bOn;
		break;
	case IDC_CHK_21:		// 검사부 상 제품푸쉬 스위치 "MB005606" // (토크 진공 스위치) - X
		pView->m_pEngrave->SwAoiUpPushUp(bOn);
		pDoc->BtnStatus.AoiUp.PushUp = bOn;
		break;
	case IDC_CHK_22:		// 검사부 상 테이블 브로워 스위치 "MB005602"
		pView->m_pEngrave->SwAoiUpTblBlw(bOn);
		pDoc->BtnStatus.AoiUp.TblBlw = bOn;
		break;
	case IDC_CHK_23:		// 검사부 상 테이블 진공 스위치 "MB005607"
		pView->m_pEngrave->SwAoiUpTblVac(bOn);
		pDoc->BtnStatus.AoiUp.TblVac = bOn;
		break;
	case IDC_CHK_53:		// 검사부 상 피딩 클램프 스위치 "MB005609"
		pView->m_pEngrave->SwAoiUpFdClp(bOn);
		pDoc->BtnStatus.AoiUp.FdClp = bOn;
		break;
	case IDC_CHK_54:		// 검사부 상 텐션 클램프 스위치 "MB00560A"
		pView->m_pEngrave->SwAoiUpTqClp(bOn);
		pDoc->BtnStatus.AoiUp.TqClp = bOn;
		break;
	case IDC_CHK_24:		// 검사부 상 한판넬 이송 스위치  "MB440151"
		pView->m_pEngrave->SwAoiUpMvOne(bOn);
		pDoc->BtnStatus.AoiUp.MvOne = bOn;
		break;
	case IDC_CHK_50:		// 검사부 상 레이져마크 스위치 "MB005608"
		pView->m_pEngrave->SwAoiUpLsrPt(bOn);
		pDoc->BtnStatus.AoiUp.LsrPt = bOn;
		break;

	// 각인부
	case IDC_CHK_72:			// 각인부 연동 온/오프 스위치
		pView->m_pEngrave->SwEngRelation(bOn);
		pDoc->BtnStatus.Eng.Relation = bOn;
		break;
	case IDC_CHK_73:		// 각인부 피딩 정회전 스위치
		pView->m_pEngrave->SwEngFdCw(bOn);
		pDoc->BtnStatus.Eng.FdCw = bOn;
		break;
	case IDC_CHK_74:		// 각인부 피딩 역회전 스위치
		pView->m_pEngrave->SwEngFdCcw(bOn);
		pDoc->BtnStatus.Eng.FdCcw = bOn;
		break;
	case IDC_CHK_75:		// 각인부 피딩 진공 스위치
		pView->m_pEngrave->SwEngFdVac(bOn);
		pDoc->BtnStatus.Eng.FdVac = bOn;
		break;
	case IDC_CHK_76:		// 각인부 제품푸쉬 스위치 // (토크 진공 스위치) - X
		pView->m_pEngrave->SwEngPushUp(bOn);
		pDoc->BtnStatus.Eng.PushUp = bOn;
		break;
	case IDC_CHK_77:		// 각인부 테이블 브로워 스위치
		pView->m_pEngrave->SwEngTblBlw(bOn);
		pDoc->BtnStatus.Eng.TblBlw = bOn;
		break;
	case IDC_CHK_78:		// 각인부 테이블 진공 스위치
		pView->m_pEngrave->SwEngTblVac(bOn);
		pDoc->BtnStatus.Eng.TblVac = bOn;
		break;
	case IDC_CHK_82:		// 각인부 피딩 클램프 스위치
		pView->m_pEngrave->SwEngFdClp(bOn);
		pDoc->BtnStatus.Eng.FdClp = bOn;
		break;
	case IDC_CHK_83:		// 각인부 텐션 클램프 스위치
		pView->m_pEngrave->SwEngTqClp(bOn);
		pDoc->BtnStatus.Eng.TqClp = bOn;
		break;
	case IDC_CHK_79:		// 각인부 한판넬 이송 스위치  "MB440151"
		pView->m_pEngrave->SwEngMvOne(bOn);
		pDoc->BtnStatus.Eng.MvOne = bOn;
		break;
	case IDC_CHK_81:		// 각인부 레이져마크 스위치 "" IDC_CHK_81
		pView->m_pEngrave->SwEngLsrPt(bOn);
		pDoc->BtnStatus.Eng.LsrPt = bOn;
		break;
	case IDC_CHK_80:		// 각인부 댄서롤 상승/하강 스위치
		pView->m_pEngrave->SwEngDcRSol(bOn);
		pDoc->BtnStatus.Eng.DcRSol = bOn;
		break;
	case IDC_CHK_87:		// 각인부 초음파 세정기 스위치 "MB44014E" pDoc->WorkingInfo.LastJob.bEngraveCleanner
		pView->m_pEngrave->SwEngSonicBlw(bOn);
		pDoc->BtnStatus.Eng.SonicBlw = bOn;
		break;

	// Uncoiler
	case IDC_CHK_25:		// 언코일러 연동 온/오프 스위치 "MB005401"
		pView->m_pEngrave->SwUcRelation(bOn);
		pDoc->BtnStatus.Uc.Relation = bOn;
		break;
	case IDC_CHK_26:		// 언코일러 제품휠 정회전 스위치 "MB00540C"
		pView->m_pEngrave->SwUcFdCw(bOn);
		pDoc->BtnStatus.Uc.FdCw = bOn;
		break;
	case IDC_CHK_27:		// 언코일러 제품휠 역회전 스위치 "MB00540D"
		pView->m_pEngrave->SwUcFdCcw(bOn);
		pDoc->BtnStatus.Uc.FdCcw = bOn;
		break;
	case IDC_CHK_35:		// 언코일러 제품척 클램프 스위치 "MB00540B"
		pView->m_pEngrave->SwUcReelChuck(bOn);
		pDoc->BtnStatus.Uc.ReelChuck = bOn;
		break;
	case IDC_CHK_28:		// 언코일러 댄서롤 상승/하강 스위치 "MB005402"
		pView->m_pEngrave->SwUcDcRlUpDn(bOn);
		pDoc->BtnStatus.Uc.DcRlUpDn = bOn;
		break;
	case IDC_CHK_30:		// 언코일러 제품 이음매(상/좌) 스위치 "MB005405"
		pView->m_pEngrave->SwUcReelJoinL(bOn);
		pDoc->BtnStatus.Uc.ReelJoinL = bOn;
		break;
	case IDC_CHK_37:		// 언코일러 제품 이음매(하/우) 스위치 "MB005406"
		pView->m_pEngrave->SwUcReelJoinR(bOn);
		pDoc->BtnStatus.Uc.ReelJoinR = bOn;
		break;
	case IDC_CHK_38:		// 언코일러 제품 이음매 진공 스위치 "MB00540F"
		pView->m_pEngrave->SwUcReelJoinVac(bOn);
		pDoc->BtnStatus.Uc.ReelJoinVac = bOn;
		break;
	case IDC_CHK_31:		// 언코일러 간지척 클램프 스위치 "MB005408"
		pView->m_pEngrave->SwUcPaperChuck(bOn);
		pDoc->BtnStatus.Uc.PprChuck = bOn;
		break;
	case IDC_CHK_32:		// 언코일러 간지휠 정회전 스위치 "MB005409"
		pView->m_pEngrave->SwUcPaperCw(bOn);
		pDoc->BtnStatus.Uc.PprCw = bOn;
		break;
	case IDC_CHK_39:		// 언코일러 간지휠 역회전 스위치 "MB00540A"
		pView->m_pEngrave->SwUcPaperCcw(bOn);
		pDoc->BtnStatus.Uc.PprCcw = bOn;
		break;
	case IDC_CHK_29:		// 언코일러 클린롤러 상승/하강 스위치 "MB005403"
		pView->m_pEngrave->SwUcClRlUpDn(bOn);
		pDoc->BtnStatus.Uc.ClRlUpDn = bOn;
		break;
	case IDC_CHK_36:		// 언코일러 클린롤러누름 상승/하강 스위치 "MB005404"
		pView->m_pEngrave->SwUcClRlPshUpDn(bOn);
		pDoc->BtnStatus.Uc.ClRlPshUpDn = bOn;
		break;
	}
#endif
}

BOOL CDlgMenu03::GetCw() 
{
	if(!pDoc->m_pMpeIo)
		return FALSE;

	// MpeIO
	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;

#ifdef USE_MPE
	if(pDoc->m_pMpeIo[nInSeg + 5] & (0x01<<3))		//[29] 마킹부 피딩 정회전 스위치 램프
		return TRUE;
	if(pDoc->m_pMpeIo[nInSeg + 8] & (0x01<<3))		//[32] 검사부 피딩 정회전 스위치 램프
		return TRUE;
#endif
	return FALSE;
}

void CDlgMenu03::SetCw(BOOL bOn) 
{
	long lData = 1;
#ifdef USE_MPE
	pView->m_pMpe->Write(_T("MB440161"), lData);	// 마킹부 피딩 ON (PLC가 피딩완료 후 OFF)
	pView->m_pMpe->Write(_T("MB440160"), lData);	// 검사부 피딩 ON (PLC가 피딩완료 후 OFF)
#endif
}

BOOL CDlgMenu03::GetCcw() 
{
	if(!pDoc->m_pMpeIo)
		return FALSE;

	// MpeIO
	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;

#ifdef USE_MPE
	if(pDoc->m_pMpeIo[nInSeg + 5] & (0x01<<4))		//[29] 마킹부 피딩 역회전 스위치 램프 
		return TRUE;
	if(pDoc->m_pMpeIo[nInSeg + 8] & (0x01<<4))		//[32] 검사부 피딩 역회전 스위치 램프
		return TRUE;
#endif
	return FALSE;
}

void CDlgMenu03::SetCcw(BOOL bOn) 
{
	long lData = 1;
#ifdef USE_MPE
	if(pView->m_pMpe)
	{
		pView->m_pMpe->Write(_T("MB440161"), lData);	// 마킹부 피딩 ON (PLC가 피딩완료 후 OFF)
		pView->m_pMpe->Write(_T("MB440160"), lData);	// 검사부 피딩 ON (PLC가 피딩완료 후 OFF)
	}
#endif
}

void CDlgMenu03::MoveMk(double dOffset)
{
	double fLen, fVel, fAcc, fTarget;
// 	if(pView->m_pMotion)
// 		pView->m_pMotion->SetOriginPos(AXIS_MKFD);

	fLen = pDoc->GetOnePnlLen();
	fVel = pDoc->GetOnePnlVel();
	fAcc = pDoc->GetOnePnlAcc();
	fTarget = fLen + dOffset;

#ifdef USE_MPE
	if(pView->m_pMpe)
	{
		long lData = (long)(fTarget * 1000.0);
		pView->m_pMpe->Write(_T("ML45066"), lData);	// 마킹부 Feeding 롤러 Offset(*1000, +:더 보냄, -덜 보냄)

		MoveMk();
	}
#endif
// 	if(pView->m_pMotion)
// 	{
// 		if(!pView->m_pMotion->Move(MS_MKFD, fTarget, fVel, fAcc, fAcc, INC, NO_WAIT))
// 		{
// 			if(!pView->m_pMotion->Move(MS_MKFD, fTarget, fVel, fAcc, fAcc, INC, NO_WAIT))
// 				AfxMessageBox(_T("Move XY Error..."));
// 		}
// 
// 	}
} 

void CDlgMenu03::TimMoveMk(int nDir) 
{
}

void CDlgMenu03::MoveMk(int nDir) 
{
#ifdef USE_MPE
	if(pDoc->WorkingInfo.LastJob.bMkOnePnl)
	{
		if(pView->m_pMpe)
			pView->m_pMpe->Write(_T("MB440161"), 1);	// 마킹부 피딩 ON (PLC가 피딩완료 후 OFF)
	}
#endif
}

void CDlgMenu03::StopMk() 
{
// 	if(!pDoc->WorkingInfo.LastJob.bMkOnePnl)
// 	{
// 		if(pView->m_pMotion)
// 		{
// 			pView->m_pMotion->EStop(MS_MKFD);
// 			Sleep(30);
// 			pView->m_pMotion->SetOriginPos(AXIS_MKFD);
// 			pView->m_pMotion->Clear(MS_MKFD);
// 		}
// 	}
}

void CDlgMenu03::MoveAoi(double dOffset)
{
 	double fLen, fVel, fAcc, fTarget;

	fLen = pDoc->GetOnePnlLen();
	fVel = pDoc->GetOnePnlVel();
	fAcc = pDoc->GetOnePnlAcc();
	fTarget = fLen - dOffset;

#ifdef USE_MPE
	if(pView->m_pMpe)
	{
 		long lData = (long)(fTarget * 1000.0);
//		long lData = (long)0;
		pView->m_pMpe->Write(_T("ML45064"), lData);	// 검사부 Feeding 롤러 Offset(*1000, +:더 보냄, -덜 보냄)

		MoveAoi();
	}
#endif
} 

void CDlgMenu03::TimMoveAoi(int nDir)
{

}

void CDlgMenu03::MoveAoi(int nDir) 
{
#ifdef USE_MPE
	if(pDoc->WorkingInfo.LastJob.bAoiOnePnl)
	{
		if(pView->m_pMpe)
			pView->m_pMpe->Write(_T("MB440160"), 1);	// 검사부 피딩 ON (PLC가 피딩완료 후 OFF)
	}
#endif
}

void CDlgMenu03::StopAoi() 
{
// 	if(!pDoc->WorkingInfo.LastJob.bAoiOnePnl)
// 	{
// 		if(pView->m_pMotion)
// 		{
// 			pView->m_pMotion->EStop(MS_AOIFD);
// 			Sleep(30);
// 			pView->m_pMotion->SetOriginPos(AXIS_AOIFD);
// 			pView->m_pMotion->Clear(MS_AOIFD);
// 		}
// 	}
}

void CDlgMenu03::ChkDoneMk() 
{
	if(pDoc->WorkingInfo.LastJob.bMkOnePnl)
	{
		if(!m_bTIM_CHK_DONE_MK)
		{
			m_bTIM_CHK_DONE_MK = TRUE;
			SetTimer(TIM_CHK_DONE_MK, 100, NULL);
		}
	}
// 	else
// 	{
// 		if(!m_bTIM_CHK_SOL_MK)
// 		{
// 			m_bTIM_CHK_SOL_MK = TRUE;
// 			SetTimer(TIM_CHK_SOL_MK, 100, NULL);
// 		}
// 	}
}

void CDlgMenu03::ChkDoneAoi() 
{
	if(pDoc->WorkingInfo.LastJob.bAoiOnePnl)
	{
		if(!m_bTIM_CHK_DONE_AOI)
		{
			m_bTIM_CHK_DONE_AOI = TRUE;
			SetTimer(TIM_CHK_DONE_AOI, 100, NULL);
		}
	}
// 	else
// 	{
// 		if(!m_bTIM_CHK_SOL_AOI)
// 		{
// 			m_bTIM_CHK_SOL_AOI = TRUE;
// 			SetTimer(TIM_CHK_SOL_AOI, 100, NULL);
// 		}
// 	}
}

void CDlgMenu03::ChkDoneMkAoi() 
{
	if(pDoc->WorkingInfo.LastJob.bMkOnePnl && pDoc->WorkingInfo.LastJob.bAoiOnePnl)
	{
		if(!m_bTIM_CHK_DONE_MKAOI)
		{
			m_bTIM_CHK_DONE_MKAOI = TRUE;
			SetTimer(TIM_CHK_DONE_MKAOI, 100, NULL);
		}
	}
}

BOOL CDlgMenu03::GetRelation() 
{
	if(!pDoc->m_pMpeIo)
		return FALSE;

	// MpeIO
	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;

#ifdef USE_MPE
	if(!(pDoc->m_pMpeIo[nInSeg + 16] & (0x01<<1)))	//[40] 리코일러 연동 온/오프 스위치 램프
		return FALSE;
	if(!(pDoc->m_pMpeIo[nInSeg] & (0x01<<1)))		//[24] 언코일러 연동 온/오프 스위치 램프
		return FALSE;

	if(!(pDoc->m_pMpeIo[nInSeg + 5] & (0x01<<1)))	//[29] 마킹부 연동 온/오프 스위치 램프
		return FALSE;
	if(!(pDoc->m_pMpeIo[nInSeg + 8] & (0x01<<1)))	//[32] 검사부 상 연동 온/오프 스위치 램프
		return FALSE;
	if(!(pDoc->m_pMpeIo[nInSeg + 12] & (0x01<<1)))	//[36] 검사부 하 연동 온/오프 스위치 램프
		return FALSE;
#endif
	return TRUE;
}

BOOL CDlgMenu03::GetMkOnePnl()
{
	return pDoc->WorkingInfo.LastJob.bMkOnePnl;
}

BOOL CDlgMenu03::GetAoiOnePnl()
{
	return pDoc->WorkingInfo.LastJob.bAoiOnePnl;
}

BOOL CDlgMenu03::GetRun() 
{
	if(!pDoc->m_pMpeIo)
		return FALSE;

	//if(pDoc->m_pMpeIo[28] & (0x01<<1))	// 마킹부 운전 스위치 램프
	if(pView->m_bSwRun) // 초기운전시 램프 On/Off
		return TRUE;

	return FALSE;
}

BOOL CDlgMenu03::GetReady()
{
	if(!pDoc->m_pMpeIo)
		return FALSE;

	//if(pDoc->m_pMpeIo[28] & (0x01<<3))	// 마킹부 운전준비 스위치 램프
	//if(pDoc->m_pMpeSignal[0] & (0x01<<0))	// PLC 운전준비 완료(PC가 확인하고 Reset시킴.)
	//if(pView->m_bReadyDone)
	//{
	//	pView->m_bReadyDone = TRUE;
		return TRUE;
	//}

	return FALSE;
}

BOOL CDlgMenu03::GetReset()
{
	if(!pDoc->m_pMpeIo)
		return FALSE;

 	//if(pDoc->m_pMpeIo[28] & (0x01<<4))	//  마킹부 리셋 스위치 램프
	if(pView->m_bSwReset)
		return TRUE;

	return FALSE;
}

BOOL CDlgMenu03::GetMkTq()
{
	return (pDoc->WorkingInfo.Motion.bMkTq);
}

BOOL CDlgMenu03::GetAoiTq()
{
	return (pDoc->WorkingInfo.Motion.bAoiTq);
}

BOOL CDlgMenu03::GetEngraveTq()
{
	return (pDoc->WorkingInfo.Motion.bEngraveTq);
}

void CDlgMenu03::SetMkTq(BOOL bOn)
{
// 	pDoc->WorkingInfo.Motion.bMkTq = bOn;
// 
// 	if(bOn)
// 	{
// 		if(pView->m_pMotion)
// 			pView->m_pMotion->ServoOnOff(AXIS_MKTQ, ON);		// Amp ON
// 	}
// 	else
// 	{
// 		if(pView->m_pMotion)
// 			pView->m_pMotion->ServoOnOff(AXIS_MKTQ, OFF);		// Amp OFF
// 	}
// 
#ifdef USE_MPE
	if(pView->m_pMpe)
		pView->m_pMpe->Write(_T("MB440155"), bOn?1:0);	// 마킹부 텐션 ON (PC가 ON/OFF시킴)
#endif
	CString sData, sPath=PATH_WORKING_INFO;
	sData.Format(_T("%d"), pDoc->WorkingInfo.Motion.bMkTq?1:0);
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_TENSION_SERVO_TORQUE_ON"), sData, sPath);
}

void CDlgMenu03::SetAoiTq(BOOL bOn)
{
// 	pDoc->WorkingInfo.Motion.bAoiTq = bOn;
// 
// 	if(bOn)
// 	{
// 		if(pView->m_pMotion)
// 			pView->m_pMotion->ServoOnOff(AXIS_AOITQ, ON);		// Amp ON
// 	}
// 	else
// 	{
// 		if(pView->m_pMotion)
// 			pView->m_pMotion->ServoOnOff(AXIS_AOITQ, OFF);		// Amp OFF
// 	}
// 
#ifdef USE_MPE
	if(pView->m_pMpe)
		pView->m_pMpe->Write(_T("MB440156"), bOn?1:0);	// 검사부 텐션 ON (PC가 ON/OFF시킴)
#endif
	CString sData, sPath=PATH_WORKING_INFO;
	sData.Format(_T("%d"), pDoc->WorkingInfo.Motion.bAoiTq?1:0);
	::WritePrivateProfileString(_T("Motion"), _T("AOI_TENSION_SERVO_TORQUE_ON"), sData, sPath);	
}

void CDlgMenu03::SetEngraveTq(BOOL bOn)
{
#ifdef USE_MPE
	if(pView->m_pMpe)
		pView->m_pMpe->Write(_T("MB440154"), bOn?1:0);	// 검사부 텐션 ON (PC가 ON/OFF시킴)
#endif
	CString sData, sPath=PATH_WORKING_INFO;
	sData.Format(_T("%d"), pDoc->WorkingInfo.Motion.bEngraveTq?1:0);
	::WritePrivateProfileString(_T("Motion"), _T("ENGRAVE_TENSION_SERVO_TORQUE_ON"), sData, sPath);	
}

void CDlgMenu03::SetMkOnePnl(BOOL bOn)
{
#ifdef USE_MPE
	//pView->IoWrite("MB440151", bOn?1:0);	// 한판넬 이송상태 ON (PC가 ON, OFF)
	pView->m_pMpe->Write(_T("MB440151"), bOn?1:0);
#endif
	CString sData, sPath=PATH_WORKING_INFO;
	pDoc->WorkingInfo.LastJob.bMkOnePnl = bOn;
	sData.Format(_T("%d"), pDoc->WorkingInfo.LastJob.bMkOnePnl?1:0);
	::WritePrivateProfileString(_T("Last Job"), _T("Marking One Pannel Move On"), sData, sPath);

}

void CDlgMenu03::SetEngraveCleanner(BOOL bOn)
{
#ifdef USE_MPE
	pView->m_pMpe->Write(_T("MB44014E"), bOn ? 1 : 0); // [각인부 세정기]
#endif
	CString sData, sPath = PATH_WORKING_INFO;
	pDoc->WorkingInfo.LastJob.bEngraveCleanner = bOn;
	sData.Format(_T("%d"), pDoc->WorkingInfo.LastJob.bEngraveCleanner ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Engrave Cleanner"), sData, sPath);
}

void CDlgMenu03::SetAoiDnCleanner(BOOL bOn)
{
#ifdef USE_MPE
	pView->m_pMpe->Write(_T("MB44014F"), bOn ? 1 : 0); // [AOI(하) 세정기]
#endif
	CString sData, sPath = PATH_WORKING_INFO;
	pDoc->WorkingInfo.LastJob.bAoiDnCleanner = bOn;
	sData.Format(_T("%d"), pDoc->WorkingInfo.LastJob.bAoiDnCleanner ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("AoiDn Cleanner"), sData, sPath);
}

void CDlgMenu03::SetAoiOnePnl(BOOL bOn)
{
#ifdef USE_MPE
	//pView->IoWrite("MB440151", bOn?1:0);	// 한판넬 이송상태 ON (PC가 ON, OFF)
	pView->m_pMpe->Write(_T("MB440151"), bOn?1:0);
#endif
	CString sData, sPath=PATH_WORKING_INFO;
	pDoc->WorkingInfo.LastJob.bAoiOnePnl = bOn;
	sData.Format(_T("%d"), pDoc->WorkingInfo.LastJob.bAoiOnePnl?1:0);
	::WritePrivateProfileString(_T("Last Job"), _T("AOI One Pannel Move On"), sData, sPath);
}

BOOL CDlgMenu03::DoReset()
{
	pView->DispThreadTick();

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if(pDoc->Status.bManual)
	{
		BOOL bInit=TRUE;

		if(m_bTIM_CHK_DONE_READY)
		{
			m_bTIM_CHK_DONE_READY = FALSE;
			//pView->ClrDispMsg();
			pView->m_bReadyDone = FALSE;
#ifdef USE_MPE
			if(pView->m_pMpe)
				pView->m_pMpe->Write(_T("MB440100"), 0);	// PLC 운전준비 완료(PC가 확인하고 Reset시킴.)
#endif
		}
		pView->ClrDispMsg();
		
//		if(IDNO == pView->DoMyMsgBox(_T("초기화 하시겠습니까?"), MB_YESNO))
		if(IDNO == pView->MsgBox(_T("초기화 하시겠습니까?"), 0, MB_YESNO))
			bInit = FALSE;
		else
		{
			pDoc->m_bDoneChgLot = FALSE;
			pView->m_nNewLot = 0;
		}

		if(!bInit)
		{
//			if(IDNO == pView->DoMyMsgBox(_T("이어가기를 하시겠습니까?"), MB_YESNO))
			if(IDNO == pView->MsgBox(_T("이어가기를 하시겠습니까?"), 0, MB_YESNO))
			{
				pView->m_bCont = FALSE;
				return FALSE;
			}
			pView->m_bCont = TRUE;
		}
		pView->m_nDebugStep = 1; pView->DispThreadTick();
		pView->InitAuto(bInit);

		pView->m_nDebugStep = 2; pView->DispThreadTick();
		pView->SetPathAtBuf();

		pView->m_nDebugStep = 3; pView->DispThreadTick();
		pView->SetAoiDummyShot(0, pView->GetAoiUpDummyShot());

		pView->m_nDebugStep = 4; pView->DispThreadTick();
		if(bDualTest)
			pView->SetAoiDummyShot(1, pView->GetAoiDnDummyShot());
			
		//pView->IoWrite("MB440162", 0); // 마킹부 정지 스위치 램프 ON(PC가 On/Off시킴)  - 20141021	
		//pView->m_pMpe->Write(_T("MB440162", 0);

		pView->m_nDebugStep = 5; pView->DispThreadTick();
		pView->m_bAoiFdWrite[0] = FALSE;
		pView->m_bAoiFdWrite[1] = FALSE;
		pView->m_bAoiFdWriteF[0] = FALSE;
		pView->m_bAoiFdWriteF[1] = FALSE;
		pView->m_bCycleStop = FALSE;
		pView->m_bContDiffLot = FALSE;

		pView->m_bInit = bInit;

		CFileFind cFile;
		BOOL bExistSup, bExistBup, bExistSdn, bExistBdn;

		pView->m_nDebugStep = 6; pView->DispThreadTick();
		bExistSup = cFile.FindFile(pDoc->WorkingInfo.System.sPathVrsShareUp + _T("*.pcr"));
		pView->m_nDebugStep = 7; pView->DispThreadTick();
		bExistBup = cFile.FindFile(pDoc->WorkingInfo.System.sPathVrsBufUp + _T("*.pcr"));
		if(bDualTest)
		{
			pView->m_nDebugStep = 8; pView->DispThreadTick();
			bExistSdn = cFile.FindFile(pDoc->WorkingInfo.System.sPathVrsShareDn + _T("*.pcr"));
			pView->m_nDebugStep = 9; pView->DispThreadTick();
			bExistBdn = cFile.FindFile(pDoc->WorkingInfo.System.sPathVrsBufDn + _T("*.pcr"));
			if(bExistSup || bExistSdn ||
				bExistBup || bExistBdn)
			{
				pView->m_nDebugStep = 10; pView->DispThreadTick();
				pDoc->DelPcrAll();
			}
		}
		else
		{
			if(bExistSup || bExistBup)
			{
				pView->m_nDebugStep = 11; pView->DispThreadTick();
				pDoc->DelPcrAll();
			}

		}

		pView->m_nDebugStep = 12; pView->DispThreadTick();
		pView->TowerLamp(RGB_RED, TRUE, FALSE);
		pView->m_nDebugStep = 13; pView->DispThreadTick();
		pView->DispStsBar(_T("정지-2"), 0);
		pView->m_nDebugStep = 14; pView->DispThreadTick();
		pView->DispMain(_T("정 지"), RGB_RED);	
//		SwStop();
		pView->m_nDebugStep = 15; pView->DispThreadTick();
		SwAoiReset(TRUE);

		pView->m_nDebugStep = 16; pView->DispThreadTick();
		if(bInit)
		{
			pDoc->m_ListBuf[0].Clear();
			pDoc->m_ListBuf[1].Clear();
		}
		else
		{
			pView->SetListBuf();
		}
		pView->m_nDebugStep = 17; pView->DispThreadTick();
		
		return TRUE;
	}

	return FALSE;		
}

void CDlgMenu03::DoReady()
{
//	if(pDoc->Status.bAuto)
//	{
// 		// 한판넬 이송 On
// 		SetMkOnePnl(TRUE);
// 		SetAoiOnePnl(TRUE);

		
		//pView->IoWrite("MB440162", 0); // 마킹부 정지 스위치 램프 ON(PC가 On/Off시킴)  - 20141021	
	//pView->m_pMpe->Write(_T("MB440162", 0);
	if(m_bTIM_CHK_DONE_READY)
	{
		m_bTIM_CHK_DONE_READY = FALSE;
		KillTimer(TIM_CHK_DONE_READY);
	}
	ChkReadyDone();
//	}
}

void CDlgMenu03::ChkBufHomeDone()
{
	if(!m_bTIM_CHK_DONE_BUF_HOME)
	{
// 		m_bBufInitPosMove = bInitPos;
		m_bTIM_CHK_DONE_BUF_HOME = TRUE;
		SetTimer(TIM_CHK_DONE_BUF_HOME, 100, NULL);
// 		m_dPrevEnc[AXIS_MKFD] = pView->m_dEnc[AXIS_MKFD];
	}
}

void CDlgMenu03::ChkBufInitDone()
{
	if(!m_bTIM_CHK_DONE_BUF_INIT)
	{
// 		m_bBufInitPosMove = bInitPos;
		m_bTIM_CHK_DONE_BUF_INIT = TRUE;
		SetTimer(TIM_CHK_DONE_BUF_INIT, 100, NULL);
// 		m_dPrevEnc[AXIS_MKFD] = pView->m_dEnc[AXIS_MKFD];
	}
}

void CDlgMenu03::ChkReadyDone()
{
	if(!m_bTIM_CHK_DONE_READY)
	{
		m_bTIM_CHK_DONE_READY = TRUE;
		SetTimer(TIM_CHK_DONE_READY, 100, NULL);
	}
}

// [Main]
void CDlgMenu03::SwRun(BOOL bOn) 
{
// 	if(bOn)
// 		pDoc->m_pSliceIo[6] |= (0x01<<1);		// 마킹부 운전 스위치 램프
// 	else
// 		pDoc->m_pSliceIo[6] &= ~(0x01<<1);		// 마킹부 운전 스위치 램프
}

void CDlgMenu03::SwRun() 
{
	if(pDoc->Status.bAuto)
	{
// 		if(!(pDoc->m_pMpeSignal[0] & (0x01<<0)))	// PLC 운전준비 완료
		if(pView->m_nStepAuto < AT_LP)
		{
			//if(!pView->m_bSwReady)	// PLC 운전준비 완료
			//{
				//pView->DispMsg(_T("운전 준비를 눌러주세요."),_T("주의"),RGB_YELLOW,2000,TRUE);
				//return;
			//}
		}
	
		//pView->IoWrite("MB440162", 0); // 마킹부 정지 스위치 램프 ON(PC가 On/Off시킴)  - 20141021	
		//pView->m_pMpe->Write(_T("MB440162", 0);
		
		pView->m_bSwRun = TRUE;
		pView->m_nStop = 0;
		pView->m_bSwStop = FALSE;
		pView->m_bSwReady = FALSE;
		pView->m_bSwReset = FALSE;
		pView->m_bCycleStop = FALSE;

		// 한판넬 이송 On
		SetMkOnePnl(TRUE);
		SetAoiOnePnl(TRUE);
		SetEngraveOnePnl(TRUE);

		pDoc->m_sAlmMsg = _T("");
		pDoc->m_sPrevAlmMsg = _T("");
	}

	pView->ClrDispMsg();
}

BOOL CDlgMenu03::IsStop() 
{
// 	BOOL bOn = pDoc->m_pMpeIo[28] & (0x01<<2);	// 마킹부 정지 스위치 램프
// 	return bOn;
	return pView->m_bSwStop;
}

void CDlgMenu03::SwStop(BOOL bOn) 
{
	if(bOn)
	{
// 		pDoc->m_pSliceIo[6] |= (0x01<<2);		// 마킹부 정지 스위치 램프

// 		pDoc->m_pMpeIo[8] |= (0x01<<14);		// 언코일러 제품 EPC원점 스위치 램프
// 		pDoc->m_pMpeIo[12] |= (0x01<<14);		// 언코일러 제품 EPC원점 스위치 램프
	}
	else
	{
// 		pDoc->m_pSliceIo[6] &= ~(0x01<<2);		// 마킹부 정지 스위치 램프

// 		pDoc->m_pMpeIo[8] &= ~(0x01<<14);		// 언코일러 제품 EPC원점 스위치 램프
// 		pDoc->m_pMpeIo[12] &= ~(0x01<<14);		// 언코일러 제품 EPC원점 스위치 램프
	}
}

void CDlgMenu03::SwStop() 
{
	pView->m_bSwRun = FALSE;
	pView->m_bSwStop = TRUE;
	pView->m_bSwReady = FALSE;
	pView->m_bSwReset = FALSE;

	//if(!pView->m_bAuto)
		pView->DispStsBar(_T("정지-3"), 0);
		pView->DispMain(_T("정 지"), RGB_RED);
	pView->TowerLamp(RGB_RED, TRUE, FALSE);
#ifdef USE_MPE
	//pView->IoWrite("MB440162", 1); // 마킹부 정지 스위치 램프 ON(PC가 On/Off시킴)  - 20141021
	pView->m_pMpe->Write(_T("MB440162"), 1);
#endif
	pView->ClrDispMsg();
}

void CDlgMenu03::SwReady(BOOL bOn)
{
// 	if(bOn)
// 	{
// 		pDoc->m_pSliceIo[6] |= (0x01<<3);	// 마킹부 운전준비 스위치 램프
// 		DoReady();
// 	}
// 	else
// 		pDoc->m_pSliceIo[6] &= ~(0x01<<3);	// 마킹부 운전준비 스위치 램프

}

void CDlgMenu03::SwReady()
{
 	if(!pView->m_bSwRun)
	{
		pView->m_bSwReady = TRUE;
 		DoReady();
	}
}

void CDlgMenu03::SwReset(BOOL bOn)
{
// 	if(bOn)
// 		pDoc->m_pSliceIo[6] |= (0x01<<4);	// 마킹부 리셋 스위치 램프	
// 	else
// 		pDoc->m_pSliceIo[6] &= ~(0x01<<4);	// 마킹부 리셋 스위치 램프	
}

void CDlgMenu03::SwReset()
{
// 	if(!pDoc->m_pSliceIo)
// 		return;
	pView->ClrDispMsg();

	if(!DoReset())
		return;

// 	if(pDoc->m_pSliceIo[6] & (0x01<<1))	// 마킹부 운전 스위치 램프
// 		return;
// 
// 	pDoc->m_pSliceIo[6] &= ~(0x01<<3);	// 마킹부 운전준비 스위치 램프
// 	pDoc->m_pSliceIo[6] |= (0x01<<4);	// 마킹부 리셋 스위치 램프	


	pView->m_bSwRun = FALSE;
	pView->m_bSwStop = FALSE;
	pView->m_bSwReady = FALSE;
	pView->m_bSwReset = TRUE;
}

// [Torque Motor]
void CDlgMenu03::SwMkTq(BOOL bOn)
{
	SetMkTq(bOn);
}

void CDlgMenu03::SwMkTq()
{
// 	if(!pDoc->Status.bManual)
// 	{
// 		pView->DispMsg(_T("수동 모드가 아닙니다."), _T("주의", RGB_YELLOW);
// 		return;
// 	}

	BOOL bOn = GetMkTq();
	SetMkTq(!bOn);
}

void CDlgMenu03::SwAoiTq(BOOL bOn)
{
	SetAoiTq(bOn);
}

void CDlgMenu03::SwAoiTq()
{
// 	if(!pDoc->Status.bManual)
// 	{
// 		pView->DispMsg(_T("수동 모드가 아닙니다."), _T("주의", RGB_YELLOW);
// 		return;
// 	}

	BOOL bOn = GetAoiTq();
	SetAoiTq(!bOn);
}

void CDlgMenu03::SwEngraveTq(BOOL bOn)
{
	SetEngraveTq(bOn);
}

// [Marking]
void CDlgMenu03::SwMkRelation(BOOL bOn) 
{
//	SetRelation(bOn);
}

void CDlgMenu03::SwMkRelation() 
{
	// TODO: Add your control notification handler code here
// 	if(!pDoc->Status.bManual)
// 	{
// 		pView->DispMsg(_T("수동 모드가 아닙니다."), _T("주의", RGB_YELLOW);
// 		return;
// 	}

// 	BOOL bOn = pDoc->m_pSliceIo[7] & (0x01<<1) ? TRUE : FALSE;	// 마킹부 연동 온/오프 스위치 램프
// 	SetRelation(!bOn);

// 	bOn = pDoc->m_pSliceIo[7] & (0x01<<5) ? TRUE : FALSE;	// 마킹부 피딩 진공 스위치 램프
// 	SwAoiFdVac(bOn);
// 	bOn = pDoc->m_pSliceIo[7] & (0x01<<6) ? TRUE : FALSE;	// 마킹부 토크 진공 스위치 램프
// 	SwAoiTqVac(bOn);

	if(pDoc->WorkingInfo.LastJob.bMkOnePnl != pDoc->WorkingInfo.LastJob.bAoiOnePnl) 
	{
		SetMkOnePnl(FALSE);
		SetAoiOnePnl(FALSE);
		SetEngraveOnePnl(FALSE);
	}
}

BOOL CDlgMenu03::IsMkTblBlw()
{
	if (!pDoc->m_pMpeIo)
		return FALSE;

	// MpeIO
	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;

	BOOL bOn = FALSE;
#ifdef USE_MPE
	bOn = pDoc->m_pMpeIo[nInSeg + 5] & (0x01<<2);		//[29] 마킹부 테이블 브로워 스위치 램프
#endif
	return bOn;
}

void CDlgMenu03::SwMkTblBlw(BOOL bOn)
{
// 	if(bOn)
// 		pDoc->m_pSliceIo[7] |= (0x01<<2);						
// 	else
// 		pDoc->m_pSliceIo[7] &= ~(0x01<<2);						
}

void CDlgMenu03::SwMkTblBlw()
{
	// TODO: Add your control notification handler code here
// 	if(!pDoc->Status.bManual)
// 	{
// 		pView->DispMsg(_T("수동 모드가 아닙니다."), _T("주의", RGB_YELLOW);
// 		return;
// 	}

// 	BOOL bOn = pDoc->m_pSliceIo[7] & (0x01<<2) ? TRUE : FALSE;	// 마킹부 테이블 브로워 스위치 램프
// 	if(bOn)
// 		pDoc->m_pSliceIo[7] &= ~(0x01<<2);						
// 	else
// 		pDoc->m_pSliceIo[7] |= (0x01<<2);						
}

BOOL CDlgMenu03::IsMkFdVac()
{
	if (!pDoc->m_pMpeIo)
		return FALSE;

	// MpeIO
	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;

	BOOL bOn = FALSE;
#ifdef USE_MPE
	bOn = pDoc->m_pMpeIo[nInSeg + 5] & (0x01<<5);	//[29] MB003715,	Y4255	,	마킹부 피딩 진공 스위치 램프
#endif
	return bOn;
}

void CDlgMenu03::SwMkFdVac(BOOL bOn)
{
// 	if(bOn)
// 		pDoc->m_pSliceIo[7] |= (0x01<<5);
// 	else
// 		pDoc->m_pSliceIo[7] &= ~(0x01<<5);						
}

void CDlgMenu03::SwMkFdVac()
{
// 	if(!pDoc->Status.bManual)
// 	{
// 		pView->DispMsg(_T("수동 모드가 아닙니다."), _T("주의", RGB_YELLOW);
// 		return;
// 	}

// 	BOOL bOn = pDoc->m_pSliceIo[7] & (0x01<<5) ? TRUE : FALSE;	// 마킹부 피딩 진공 스위치 램프
// 	if(bOn)
// 		pDoc->m_pSliceIo[7] &= ~(0x01<<5);						
// 	else
// 		pDoc->m_pSliceIo[7] |= (0x01<<5);

	BOOL bRelation = GetRelation();
	if(bRelation)
	{
// 		SwAoiFdVac(!bOn);
	}
}

BOOL CDlgMenu03::IsMkTqVac()
{
	if (!pDoc->m_pMpeIo)
		return FALSE;

	// MpeIO
	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;

	BOOL bOn = FALSE;
#ifdef USE_MPE
	bOn = pDoc->m_pMpeIo[nInSeg + 5] & (0x01<<6);		//[29] MB003716,	Y4256	,	마킹부 토크 진공 스위치 램프
#endif
	return bOn;
}

void CDlgMenu03::SwMkTqVac(BOOL bOn)
{
// 	if(bOn)
// 		pDoc->m_pSliceIo[7] |= (0x01<<6);						
// 	else
// 		pDoc->m_pSliceIo[7] &= ~(0x01<<6);						
}

void CDlgMenu03::SwMkTqVac()
{
// 	if(!pDoc->Status.bManual)
// 	{
// 		pView->DispMsg(_T("수동 모드가 아닙니다."), _T("주의", RGB_YELLOW);
// 		return;
// 	}

// 	BOOL bOn = pDoc->m_pSliceIo[7] & (0x01<<6) ? TRUE : FALSE;	// 마킹부 토크 진공 스위치 램프
// 	if(bOn)
// 		pDoc->m_pSliceIo[7] &= ~(0x01<<6);						
// 	else
// 		pDoc->m_pSliceIo[7] |= (0x01<<6);						

	BOOL bRelation = GetRelation();
	if(bRelation)
	{
// 		SwAoiTqVac(!bOn);
	}
}

BOOL CDlgMenu03::IsMkTblVac() 
{
	if (!pDoc->m_pMpeIo)
		return FALSE;

	// MpeIO
	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;

	BOOL bOn = FALSE;
#ifdef USE_MPE
	bOn = pDoc->m_pMpeIo[nInSeg + 5] & (0x01<<7);		//[29] MB003717,	Y4257	,	마킹부 테이블 진공 스위치 램프
#endif
	return bOn;
}

void CDlgMenu03::SwMkTblVac(BOOL bOn) 
{
// 	if(bOn)
// 		pDoc->m_pSliceIo[7] |= (0x01<<7);						
// 	else
// 		pDoc->m_pSliceIo[7] &= ~(0x01<<7);						
}

void CDlgMenu03::SwMkTblVac() 
{
// 	if(!pDoc->Status.bManual)
// 	{
// 		pView->DispMsg(_T("수동 모드가 아닙니다."), _T("주의", RGB_YELLOW);
// 		return;
// 	}

// 	BOOL bOn = pDoc->m_pSliceIo[7] & (0x01<<7) ? TRUE : FALSE;	// 마킹부 테이블 진공 스위치 램프
// 	if(bOn)
// 		pDoc->m_pSliceIo[7] &= ~(0x01<<7);						
// 	else
// 		pDoc->m_pSliceIo[7] |= (0x01<<7);						
}

void CDlgMenu03::SwMkLsrPt(BOOL bOn) 
{
// 	if(bOn)
// 		pDoc->m_pSliceIo[7] |= (0x01<<8);						
// 	else
// 		pDoc->m_pSliceIo[7] &= ~(0x01<<8);						
}

void CDlgMenu03::SwMkLsrPt() 
{
// 	if(!pDoc->Status.bManual)
// 	{
// 		pView->DispMsg(_T("수동 모드가 아닙니다."), _T("주의", RGB_YELLOW);
// 		return;
// 	}

// 	BOOL bOn = pDoc->m_pSliceIo[7] & (0x01<<8) ? TRUE : FALSE;	// 마킹부 레이져 포인터 스위치 램프
// 	if(bOn)
// 		pDoc->m_pSliceIo[7] &= ~(0x01<<8);						
// 	else
// 		pDoc->m_pSliceIo[7] |= (0x01<<8);						
}

void CDlgMenu03::SwMkFdClp(BOOL bOn) 
{
// 	if(bOn)
// 		pDoc->m_pSliceIo[7] |= (0x01<<9);						
// 	else
// 		pDoc->m_pSliceIo[7] &= ~(0x01<<9);						
}

void CDlgMenu03::SwMkFdClp() 
{
// 	if(!pDoc->Status.bManual)
// 	{
// 		pView->DispMsg(_T("수동 모드가 아닙니다."), _T("주의", RGB_YELLOW);
// 		return;
// 	}

// 	BOOL bOn = pDoc->m_pSliceIo[7] & (0x01<<9) ? TRUE : FALSE;	// 마킹부 피딩 클램프 스위치 램프
// 	if(bOn)
// 		pDoc->m_pSliceIo[7] &= ~(0x01<<9);						
// 	else
// 		pDoc->m_pSliceIo[7] |= (0x01<<9);						
}

// void CDlgMenu03::SwMkDnSol(BOOL bOn) 
// {
// 	if(bOn)
// 		pDoc->m_pSliceIo[7] |= (0x01<<10);						
// 	else
// 		pDoc->m_pSliceIo[7] &= ~(0x01<<10);						
// }

// void CDlgMenu03::SwMkDnSol() 
// {
// 	if(!pDoc->Status.bManual)
// 	{
// 		pView->DispMsg(_T("수동 모드가 아닙니다."), _T("주의", RGB_YELLOW);
// 		return;
// 	}

// 	BOOL bOn = pDoc->m_pSliceIo[7] & (0x01<<10) ? TRUE : FALSE;	// 마킹부 토크 클램프 스위치 램프 -> 마킹부 마킹 실린더 SOL
// 	if(bOn)
// 		pDoc->m_pSliceIo[7] &= ~(0x01<<10);						
// 	else
// 		pDoc->m_pSliceIo[7] |= (0x01<<10);						
// }

BOOL CDlgMenu03::IsBufRolSol() 
{
	if (!pDoc->m_pMpeIo)
		return FALSE;

	// MpeIO
	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;

	BOOL bOn = FALSE;
#ifdef USE_MPE
	bOn = pDoc->m_pMpeIo[nInSeg + 5] & (0x01<<11);		//[29] MB00371B,	Y425B	,	마킹부 댄서롤 상승/하강 스위치 램프
#endif
	return bOn;
}

void CDlgMenu03::SwBufRolSol(BOOL bOn) 
{
// 	if(bOn)
// 		pDoc->m_pSliceIo[6] |= (0x01<<6);		// 마킹부 버퍼롤러 스위치 램프					
// 	else
// 		pDoc->m_pSliceIo[6] &= ~(0x01<<6);		// 마킹부 버퍼롤러 스위치 램프					
}

void CDlgMenu03::SwBufRolSol() 
{
// 	if(!pDoc->Status.bManual)
// 	{
// 		pView->DispMsg(_T("수동 모드가 아닙니다."), _T("주의", RGB_YELLOW);
// 		return;
// 	}

// 	BOOL bOn = pDoc->m_pSliceIo[6] & (0x01<<6) ? TRUE : FALSE;	// 마킹부 버퍼롤러 스위치 램프 -> 마킹부 버퍼롤러 실린더 SOL
// 	if(bOn)
// 		pDoc->m_pSliceIo[6] &= ~(0x01<<6);						
// 	else
// 		pDoc->m_pSliceIo[6] |= (0x01<<6);						
}

// [AOI]
void CDlgMenu03::SwAoiRelation(BOOL bOn) 
{
//	SetRelation(bOn);
}

void CDlgMenu03::SwAoiRelation() 
{
	// TODO: Add your control notification handler code here
// 	if(!pDoc->Status.bManual)
// 	{
// 		pView->DispMsg(_T("수동 모드가 아닙니다."), _T("주의", RGB_YELLOW);
// 		return;
// 	}

// 	BOOL bOn = pDoc->m_pSliceIo[9] & (0x01<<0) ? TRUE : FALSE;	// 검사부 연동 온/오프 스위치 램프
// 	SetRelation(!bOn);

// 	bOn = pDoc->m_pSliceIo[9] & (0x01<<4) ? TRUE : FALSE;	// 검사부 피딩 진공 스위치 램프
// 	SwMkFdVac(bOn);
// 	bOn = pDoc->m_pSliceIo[9] & (0x01<<5) ? TRUE : FALSE;	// 검사부 토크 진공 스위치 램프
// 	SwMkTqVac(bOn);

	if(pDoc->WorkingInfo.LastJob.bMkOnePnl != pDoc->WorkingInfo.LastJob.bAoiOnePnl) 
	{
		SetMkOnePnl(FALSE);
		SetAoiOnePnl(FALSE);
		SetEngraveOnePnl(FALSE);
	}
}

BOOL CDlgMenu03::IsAoiDustBlw()
{
// 	BOOL bOn = pDoc->m_pMpeIo[8] & (0x01<<12);
// 	return bOn;
	return FALSE;
}

void CDlgMenu03::SwAoiDustBlw(BOOL bOn) 
{
// 	if(bOn)
// 		pDoc->m_pSliceIo[8] |= (0x01<<12);						
// 	else
// 		pDoc->m_pSliceIo[8] &= ~(0x01<<12);						
}

void CDlgMenu03::SwAoiDustBlw() 
{
// 	if(!pDoc->Status.bManual)
// 	{
// 		pView->DispMsg(_T("수동 모드가 아닙니다."), _T("주의", RGB_YELLOW);
// 		return;
// 	}

// 	BOOL bOn = pDoc->m_pSliceIo[8] & (0x01<<12) ? TRUE : FALSE;	// 검사부 이물제거 스위치 램프
// 	if(bOn)
// 		pDoc->m_pSliceIo[8] &= ~(0x01<<12);						
// 	else
// 		pDoc->m_pSliceIo[8] |= (0x01<<12);						
}

void CDlgMenu03::SwAoiTblBlw(BOOL bOn) 
{
// 	if(bOn)
// 		pDoc->m_pSliceIo[9] |= (0x01<<1);						
// 	else
// 		pDoc->m_pSliceIo[9] &= ~(0x01<<1);						
}

void CDlgMenu03::SwAoiTblBlw() 
{
// 	if(!pDoc->Status.bManual)
// 	{
// 		pView->DispMsg(_T("수동 모드가 아닙니다."), _T("주의", RGB_YELLOW);
// 		return;
// 	}

// 	BOOL bOn = pDoc->m_pSliceIo[9] & (0x01<<1) ? TRUE : FALSE;	// 검사부 테이블 브로워 스위치 램프
// 	if(bOn)
// 		pDoc->m_pSliceIo[9] &= ~(0x01<<1);						
// 	else
// 		pDoc->m_pSliceIo[9] |= (0x01<<1);						
}

BOOL CDlgMenu03::IsAoiFdVac()
{
	BOOL bOn = FALSE;
#ifdef USE_MPE
	bOn = pDoc->m_pMpeIb[9] & (0x01<<4);
#endif
	return bOn;
}

void CDlgMenu03::SwAoiFdVac(BOOL bOn) 
{
// 	if(bOn)
// 		pDoc->m_pSliceIo[9] |= (0x01<<4);						
// 	else
// 		pDoc->m_pSliceIo[9] &= ~(0x01<<4);						
}

void CDlgMenu03::SwAoiFdVac() 
{
// 	if(!pDoc->Status.bManual)
// 	{
// 		pView->DispMsg(_T("수동 모드가 아닙니다."), _T("주의", RGB_YELLOW);
// 		return;
// 	}

// 	BOOL bOn = pDoc->m_pSliceIo[9] & (0x01<<4) ? TRUE : FALSE;	// 검사부 피딩 진공 스위치 램프
// 	if(bOn)
// 		pDoc->m_pSliceIo[9] &= ~(0x01<<4);						
// 	else
// 		pDoc->m_pSliceIo[9] |= (0x01<<4);						

	BOOL bRelation = GetRelation();
	if(bRelation)
	{
// 		SwMkFdVac(!bOn);
	}
}

void CDlgMenu03::SwAoiTqVac(BOOL bOn) 
{
// 	if(bOn)
// 		pDoc->m_pSliceIo[9] |= (0x01<<5);						
// 	else
// 		pDoc->m_pSliceIo[9] &= ~(0x01<<5);						
}

void CDlgMenu03::SwAoiTqVac() 
{
// 	if(!pDoc->Status.bManual)
// 	{
// 		pView->DispMsg(_T("수동 모드가 아닙니다."), _T("주의", RGB_YELLOW);
// 		return;
// 	}

// 	BOOL bOn = pDoc->m_pMpeIo[32] & (0x01<<6) ? TRUE : FALSE;	// 검사부 상 토크 진공 스위치 램프
// 	if(bOn)
// 		pDoc->m_pSliceIo[9] &= ~(0x01<<5);						
// 	else
// 		pDoc->m_pSliceIo[9] |= (0x01<<5);						
// 
// 	BOOL bRelation = GetRelation();
// 	if(bRelation)
// 	{
// // 		SwMkTqVac(!bOn);
// 	}
}

BOOL CDlgMenu03::IsAoiLdRun()
{
	BOOL bRtn = TRUE;

#ifdef USE_MPE
	BOOL bOn0 = (pDoc->m_pMpeIb[10] & (0x01 << 11)) ? TRUE : FALSE;		// 검사부 상 자동 운전 <-> X432B I/F
 	BOOL bOn1 = (pDoc->m_pMpeIb[14] & (0x01<<11)) ? TRUE : FALSE;		// 검사부 하 자동 운전 <-> X442B I/F

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if(bDualTest)
	{
		if((pView->m_AoiLdRun & 0x03) == 0x03)
		{
			if(!bOn0 || !bOn1)
				bRtn = FALSE;
		}

		if(bOn0)
			pView->m_AoiLdRun |= (0x01<<0);
		else 
			pView->m_AoiLdRun &= ~(0x01<<0);

		if(bOn1)
			pView->m_AoiLdRun |= (0x01<<1);
		else 
			pView->m_AoiLdRun &= ~(0x01<<1);


		if(bOn0 && bOn1)
			return TRUE;
		else if(bOn0 && !bOn1)
			return FALSE;
		else if(!bOn0 && !bOn1)
			return FALSE;
	}
	else
	{
		if((pView->m_AoiLdRun & 0x03) == 0x03)
		{
			if(!bOn0)
				bRtn = FALSE;
		}

		if(bOn0)
			pView->m_AoiLdRun |= (0x01<<0);
		else 
			pView->m_AoiLdRun &= ~(0x01<<0);


		if(bOn0)
			return TRUE;

		return FALSE;
	}
#endif
	return bRtn;
}

BOOL CDlgMenu03::IsAoiTest()
{
// 	BOOL bOn = (pDoc->m_pMpeIo[] & (0x01<<0)) ? TRUE : FALSE;		// 검사부In 검사 시작
// 	return bOn;
	return FALSE;
}

BOOL CDlgMenu03::IsAoiTestDone()
{
// 	BOOL bOn = (pDoc->m_pMpeIo[] & (0x01<<0)) ? TRUE : FALSE;		// 검사부Out 검사 완료
// 	return bOn;
	return FALSE;
}

void CDlgMenu03::SwAoiTest(BOOL bOn)
{
// 	if(bOn)
// 		pDoc->m_pMpeIo[] |= (0x01<<0);		// 검사부 검사 시작 ON
// 	else
// 		pDoc->m_pMpeIo[] &= ~(0x01<<0);		// 검사부 검사 시작 OFF
}

BOOL CDlgMenu03::IsAoiReset()
{
// 	BOOL bOn = (pDoc->m_pMpeIo[] & (0x01<<2)) ? TRUE : FALSE;		// 검사부In Reset
// 	return bOn;
	return FALSE;
}

void CDlgMenu03::SwAoiReset(BOOL bOn)
{
	if(bOn)
	{
#ifdef USE_MPE
		//pView->IoWrite("MB00382A", 1); // 검사부 상 Reset <-> Y436A I/F
		pView->m_pMpe->Write(_T("MB00382A"), 1);
		//pView->IoWrite("MB00392A", 1); // 검사부 하 Reset <-> Y446A I/F
		pView->m_pMpe->Write(_T("MB00392A"), 1);
#endif
		SetTimer(TIM_AOI_RESET_OFF, 500, NULL);
	}
	else
	{
#ifdef USE_MPE
		//pView->IoWrite("MB00382A", 0); // 검사부 상 Reset <-> Y436A I/F
		pView->m_pMpe->Write(_T("MB00382A"), 0);
		//pView->IoWrite("MB00392A", 0); // 검사부 하 Reset <-> Y446A I/F
		pView->m_pMpe->Write(_T("MB00392A"), 0);
#endif
	}

// 	if(bOn)
// 	{
// 		pDoc->m_pMpeIo[] |= (0x01<<2);		// 검사부 Reset ON
// 		SetTimer(TIM_AOI_RESET_OFF, 500, NULL);
// 	}
// 	else
// 		pDoc->m_pMpeIo[] &= ~(0x01<<2);		// 검사부 Reset OFF
}

void CDlgMenu03::SwAoiLotEnd(BOOL bOn)
{
// 	if(bOn)
// 	{
// 		pDoc->m_pSliceIo[10] |= (0x01<<3);		// 검사부 Lot End ON
// 		SetTimer(TIM_AOI_LOTEND_OFF, 500, NULL);
// 	}
// 	else
// 		pDoc->m_pSliceIo[10] &= ~(0x01<<3);		// 검사부 Lot End OFF
}

void CDlgMenu03::SwAoiEmg(BOOL bOn)
{
// 	if(bOn)
// 		pDoc->m_pSliceIo[10] |= (0x01<<4);		// 검사부 비상정지 ON
// 	else
// 		pDoc->m_pSliceIo[10] &= ~(0x01<<4);		// 검사부 비상정지 OFF
}

BOOL CDlgMenu03::IsAoiTblVac() 
{
// 	BOOL bOn = (pDoc->m_pSliceIo[10] & (0x01<<1)) ? TRUE : FALSE;		// Out - 검사부 검사 테이블 진공 SOL
// 	return bOn;
	return FALSE;
}

BOOL CDlgMenu03::IsAoiTblVacDone() 
{
// 	BOOL bOn = (pDoc->m_pSliceIo[5] & (0x01<<1)) ? TRUE : FALSE;		// In - 검사부 테이블 진공 완료
// 	return bOn;
	return FALSE;
}

void CDlgMenu03::SwAoiTblVac(BOOL bOn) 
{
// 	if(bOn)
// 		pDoc->m_pSliceIo[9] |= (0x01<<6);						
// 	else
// 		pDoc->m_pSliceIo[9] &= ~(0x01<<6);						
}

void CDlgMenu03::SwAoiTblVac() 
{
// 	if(!pDoc->Status.bManual)
// 	{
// 		pView->DispMsg(_T("수동 모드가 아닙니다."), _T("주의", RGB_YELLOW);
// 		return;
// 	}

// 	BOOL bOn = pDoc->m_pSliceIo[9] & (0x01<<6) ? TRUE : FALSE;	// 검사부 테이블 진공 스위치 램프
// 	if(bOn)
// 		pDoc->m_pSliceIo[9] &= ~(0x01<<6);						
// 	else
// 		pDoc->m_pSliceIo[9] |= (0x01<<6);						
}

void CDlgMenu03::SwAoiLsrPt(BOOL bOn) 
{
// 	if(bOn)
// 		pDoc->m_pSliceIo[9] |= (0x01<<7);						
// 	else
// 		pDoc->m_pSliceIo[9] &= ~(0x01<<7);						
}

void CDlgMenu03::SwAoiLsrPt() 
{
// 	if(!pDoc->Status.bManual)
// 	{
// 		pView->DispMsg(_T("수동 모드가 아닙니다."), _T("주의", RGB_YELLOW);
// 		return;
// 	}

// 	BOOL bOn = pDoc->m_pSliceIo[9] & (0x01<<7) ? TRUE : FALSE;	// 검사부 레이져 포인터 스위치 램프
// 	if(bOn)
// 		pDoc->m_pSliceIo[9] &= ~(0x01<<7);						
// 	else
// 		pDoc->m_pSliceIo[9] |= (0x01<<7);						
}

void CDlgMenu03::SwAoiFdClp(BOOL bOn) 
{
// 	if(bOn)
// 		pDoc->m_pSliceIo[9] |= (0x01<<8);						
// 	else
// 		pDoc->m_pSliceIo[9] &= ~(0x01<<8);						
}

void CDlgMenu03::SwAoiFdClp() 
{
// 	if(!pDoc->Status.bManual)
// 	{
// 		pView->DispMsg(_T("수동 모드가 아닙니다."), _T("주의", RGB_YELLOW);
// 		return;
// 	}

// 	BOOL bOn = pDoc->m_pSliceIo[9] & (0x01<<8) ? TRUE : FALSE;	// 검사부 피딩 클램프 스위치 램프
// 	if(bOn)
// 		pDoc->m_pSliceIo[9] &= ~(0x01<<8);						
// 	else
// 		pDoc->m_pSliceIo[9] |= (0x01<<8);						
}

void CDlgMenu03::SwAoiTqClp(BOOL bOn) 
{
// 	if(bOn)
// 		pDoc->m_pSliceIo[9] |= (0x01<<9);						
// 	else
// 		pDoc->m_pSliceIo[9] &= ~(0x01<<9);						
}

void CDlgMenu03::SwAoiTqClp() 
{
// 	if(!pDoc->Status.bManual)
// 	{
// 		pView->DispMsg(_T("수동 모드가 아닙니다."), _T("주의", RGB_YELLOW);
// 		return;
// 	}

// 	BOOL bOn = pDoc->m_pSliceIo[9] & (0x01<<9) ? TRUE : FALSE;	// 검사부 토크 클램프 스위치 램프
// 	if(bOn)
// 		pDoc->m_pSliceIo[9] &= ~(0x01<<9);						
// 	else
// 		pDoc->m_pSliceIo[9] |= (0x01<<9);						
}

// [Uncoiler]
void CDlgMenu03::SwUcRelation(BOOL bOn) 
{
//	SetRelation(bOn);
}

void CDlgMenu03::SwUcRelation() 
{
//	SetTimer(TIM_SW_UC_RELATION, 200, NULL);
}

void CDlgMenu03::SwUcRelationTim() 
{
	// TODO: Add your control notification handler code here
// 	if(!pDoc->Status.bManual)
// 	{
// 		pView->DispMsg(_T("수동 모드가 아닙니다."), _T("주의", RGB_YELLOW);
// 		return;
// 	}

// 	BOOL bOn = pDoc->m_pMpeIo[8] & (0x01<<1) ? TRUE : FALSE;	// 언코일러 연동 온/오프 스위치 램프 MB003641
// 	SetRelation(bOn);

// 	bOn = pDoc->m_pSliceIo[7] & (0x01<<5) ? TRUE : FALSE;	// 마킹부 피딩 진공 스위치 램프
// 	if(bOn)
// 		SwAoiFdVac(bOn);
// 	bOn = pDoc->m_pSliceIo[7] & (0x01<<6) ? TRUE : FALSE;	// 마킹부 토크 진공 스위치 램프
// 	if(bOn)
// 		SwAoiTqVac(bOn);
// 	bOn = pDoc->m_pSliceIo[9] & (0x01<<4) ? TRUE : FALSE;	// 검사부 피딩 진공 스위치 램프
// 	if(bOn)
// 		SwMkFdVac(bOn);
// 	bOn = pDoc->m_pSliceIo[9] & (0x01<<5) ? TRUE : FALSE;	// 검사부 토크 진공 스위치 램프
// 	if(bOn)
// 		SwMkTqVac(bOn);

// 	if(pDoc->WorkingInfo.LastJob.bMkOnePnl != pDoc->WorkingInfo.LastJob.bAoiOnePnl) 
// 	{
// 		SetMkOnePnl(FALSE);
// 		SetAoiOnePnl(FALSE);
// 	}
}

void CDlgMenu03::SwUcDcRlUpDn(BOOL bOn) 
{
// 	if(bOn)
// 		pDoc->m_pMpeIo[8] |= (0x01<<2);						
// 	else
// 		pDoc->m_pMpeIo[8] &= ~(0x01<<2);						
}

void CDlgMenu03::SwUcDcRlUpDn() 
{
// 	if(!pDoc->Status.bManual)
// 	{
// 		pView->DispMsg(_T("수동 모드가 아닙니다."), _T("주의", RGB_YELLOW);
// 		return;
// 	}

// 	BOOL bOn = pDoc->m_pMpeIo[8] & (0x01<<2) ? TRUE : FALSE;	// 언코일러 댄서롤 상승/하강 스위치 램프 MB003642
// 	if(bOn)
// 		pDoc->m_pMpeIo[8] &= ~(0x01<<2);						
// 	else
// 		pDoc->m_pMpeIo[8] |= (0x01<<2);						
}

void CDlgMenu03::SwUcClRlUpDn(BOOL bOn) 
{
// 	if(bOn)
// 		pDoc->m_pMpeIo[8] |= (0x01<<3);						
// 	else
// 		pDoc->m_pMpeIo[8] &= ~(0x01<<3);						
}

void CDlgMenu03::SwUcClRlUpDn() 
{
// 	if(!pDoc->Status.bManual)
// 	{
// 		pView->DispMsg(_T("수동 모드가 아닙니다."), _T("주의", RGB_YELLOW);
// 		return;
// 	}

// 	BOOL bOn = pDoc->m_pMpeIo[8] & (0x01<<3) ? TRUE : FALSE;		// 언코일러 클린롤러 상승/하강 스위치 램프 MB003643
// 	if(bOn)
// 		pDoc->m_pMpeIo[8] &= ~(0x01<<3);						
// 	else
// 		pDoc->m_pMpeIo[8] |= (0x01<<3);						
}

void CDlgMenu03::SwUcClRlPshUpDn(BOOL bOn) 
{
// 	if(bOn)
// 		pDoc->m_pMpeIo[8] |= (0x01<<4);						
// 	else
// 		pDoc->m_pMpeIo[8] &= ~(0x01<<4);						
}

void CDlgMenu03::SwUcClRlPshUpDn() 
{
// 	if(!pDoc->Status.bManual)
// 	{
// 		pView->DispMsg(_T("수동 모드가 아닙니다."), _T("주의", RGB_YELLOW);
// 		return;
// 	}

// 	BOOL bOn = pDoc->m_pMpeIo[8] & (0x01<<4) ? TRUE : FALSE;		// 언코일러 클린롤러누름 상승/하강 스위치 램프 MB003644
// 	if(bOn)
// 		pDoc->m_pMpeIo[8] &= ~(0x01<<4);						
// 	else
// 		pDoc->m_pMpeIo[8] |= (0x01<<4);						
}

void CDlgMenu03::SwUcReelJoinL(BOOL bOn) 
{
// 	if(bOn)
// 		pDoc->m_pMpeIo[8] |= (0x01<<5);					
// 	else
// 		pDoc->m_pMpeIo[8] &= ~(0x01<<5);						
}

void CDlgMenu03::SwUcReelJoinL() 
{
// 	if(!pDoc->Status.bManual)
// 	{
// 		pView->DispMsg(_T("수동 모드가 아닙니다."), _T("주의", RGB_YELLOW);
// 		return;
// 	}

// 	BOOL bOn = pDoc->m_pMpeIo[8] & (0x01<<5) ? TRUE : FALSE;		// 언코일러 제품 이음매(좌) 스위치 램프 MB003645
// 	if(bOn)
// 		pDoc->m_pMpeIo[8] &= ~(0x01<<5);						
// 	else
// 		pDoc->m_pMpeIo[8] |= (0x01<<5);					
}

void CDlgMenu03::SwUcReelJoinR(BOOL bOn) 
{
// 	if(bOn)
// 		pDoc->m_pMpeIo[8] |= (0x01<<6);					
// 	else
// 		pDoc->m_pMpeIo[8] &= ~(0x01<<6);						
}

void CDlgMenu03::SwUcReelJoinR() 
{
// 	if(!pDoc->Status.bManual)
// 	{
// 		pView->DispMsg(_T("수동 모드가 아닙니다."), _T("주의", RGB_YELLOW);
// 		return;
// 	}

// 	BOOL bOn = pDoc->m_pMpeIo[8] & (0x01<<6) ? TRUE : FALSE;		// 언코일러 제품 이음매(우) 스위치 램프 MB003646
// 	if(bOn)
// 		pDoc->m_pMpeIo[8] &= ~(0x01<<6);						
// 	else
// 		pDoc->m_pMpeIo[8] |= (0x01<<6);					
}

void CDlgMenu03::SwUcReelWheel(BOOL bOn) 
{
// 	if(bOn)
// 		pDoc->m_pMpeIo[8] |= (0x01<<7);					
// 	else
// 		pDoc->m_pMpeIo[8] &= ~(0x01<<7);						
}

void CDlgMenu03::SwUcReelWheel() 
{
// 	if(!pDoc->Status.bManual)
// 	{
// 		pView->DispMsg(_T("수동 모드가 아닙니다."), _T("주의", RGB_YELLOW);
// 		return;
// 	}

// 	BOOL bOn = pDoc->m_pMpeIo[8] & (0x01<<7) ? TRUE : FALSE;		// 언코일러 제품휠 지지 스위치 램프 MB003647
// 	if(bOn)
// 		pDoc->m_pMpeIo[8] &= ~(0x01<<7);						
// 	else
// 		pDoc->m_pMpeIo[8] |= (0x01<<7);					
}

void CDlgMenu03::SwUcPprChuck(BOOL bOn) 
{
// 	if(bOn)
// 		pDoc->m_pMpeIo[8] |= (0x01<<8);					
// 	else
// 		pDoc->m_pMpeIo[8] &= ~(0x01<<8);						
}

void CDlgMenu03::SwUcPprChuck() 
{
// 	if(!pDoc->Status.bManual)
// 	{
// 		pView->DispMsg(_T("수동 모드가 아닙니다."), _T("주의", RGB_YELLOW);
// 		return;
// 	}

// 	BOOL bOn = pDoc->m_pMpeIo[8] & (0x01<<8) ? TRUE : FALSE;		// 언코일러 간지척 클램프 스위치 램프 MB003648
// 	if(bOn)
// 		pDoc->m_pMpeIo[8] &= ~(0x01<<8);						
// 	else
// 		pDoc->m_pMpeIo[8] |= (0x01<<8);					
}

void CDlgMenu03::SwUcReelChuck(BOOL bOn) 
{
// 	if(bOn)
// 		pDoc->m_pMpeIo[8] |= (0x01<<11);						
// 	else
// 		pDoc->m_pMpeIo[8] &= ~(0x01<<11);						
}

void CDlgMenu03::SwUcReelChuck() 
{
// 	if(!pDoc->Status.bManual)
// 	{
// 		pView->DispMsg(_T("수동 모드가 아닙니다."), _T("주의", RGB_YELLOW);
// 		return;
// 	}

// 	BOOL bOn = pDoc->m_pMpeIo[8] & (0x01<<11) ? TRUE : FALSE;	// 언코일러 제품척 클램프 스위치 램프 MB00364B
// 	if(bOn)
// 		pDoc->m_pMpeIo[8] &= ~(0x01<<11);						
// 	else
// 		pDoc->m_pMpeIo[8] |= (0x01<<11);						
}

// [Recoiler]
void CDlgMenu03::SwRcRelation(BOOL bOn) 
{
//	SetRelation(bOn);
}

void CDlgMenu03::SwRcRelation() 
{
// 	SetTimer(TIM_SW_RC_RELATION, 200, NULL);
}

void CDlgMenu03::SwRcRelationTim() 
{
// 	if(!pDoc->Status.bManual)
// 	{
// 		pView->DispMsg(_T("수동 모드가 아닙니다."), _T("주의", RGB_YELLOW);
// 		return;
// 	}

// 	BOOL bOn = pDoc->m_pMpeIo[12] & (0x01<<1) ? TRUE : FALSE;	// 리코일러 연동 온/오프 스위치 램프 MB004041
// 	SetRelation(bOn);

// 	bOn = pDoc->m_pSliceIo[7] & (0x01<<5) ? TRUE : FALSE;	// 마킹부 피딩 진공 스위치 램프
// 	if(bOn)
// 		SwAoiFdVac(bOn);
// 	bOn = pDoc->m_pSliceIo[7] & (0x01<<6) ? TRUE : FALSE;	// 마킹부 토크 진공 스위치 램프
// 	if(bOn)
// 		SwAoiTqVac(bOn);
// 	bOn = pDoc->m_pSliceIo[9] & (0x01<<4) ? TRUE : FALSE;	// 검사부 피딩 진공 스위치 램프
// 	if(bOn)
// 		SwMkFdVac(bOn);
// 	bOn = pDoc->m_pSliceIo[9] & (0x01<<5) ? TRUE : FALSE;	// 검사부 토크 진공 스위치 램프
// 	if(bOn)
// 		SwMkTqVac(bOn);

// 	if(pDoc->WorkingInfo.LastJob.bMkOnePnl != pDoc->WorkingInfo.LastJob.bAoiOnePnl) 
// 	{
// 		SetMkOnePnl(FALSE);
// 		SetAoiOnePnl(FALSE);
// 	}
}

void CDlgMenu03::SwRcDcRlUpDn(BOOL bOn) 
{
// 	if(bOn)
// 		pDoc->m_pMpeIo[12] |= (0x01<<2);						
// 	else
// 		pDoc->m_pMpeIo[12] &= ~(0x01<<2);						
}

void CDlgMenu03::SwRcDcRlUpDn() 
{
// 	if(!pDoc->Status.bManual)
// 	{
// 		pView->DispMsg(_T("수동 모드가 아닙니다."), _T("주의", RGB_YELLOW);
// 		return;
// 	}

// 	BOOL bOn = pDoc->m_pMpeIo[12] & (0x01<<2) ? TRUE : FALSE;	// 리코일러 댄서롤 상승/하강 스위치 램프 MB004042
// 	if(bOn)
// 		pDoc->m_pMpeIo[12] &= ~(0x01<<2);						
// 	else
// 		pDoc->m_pMpeIo[12] |= (0x01<<2);						
}

void CDlgMenu03::SwRcReelJoinL(BOOL bOn) 
{
// 	if(bOn)
// 		pDoc->m_pMpeIo[12] |= (0x01<<5);						
// 	else
// 		pDoc->m_pMpeIo[12] &= ~(0x01<<5);						
}

void CDlgMenu03::SwRcReelJoinL() 
{
// 	if(!pDoc->Status.bManual)
// 	{
// 		pView->DispMsg(_T("수동 모드가 아닙니다."), _T("주의", RGB_YELLOW);
// 		return;
// 	}

// 	BOOL bOn = pDoc->m_pMpeIo[12] & (0x01<<5) ? TRUE : FALSE;	// 리코일러 제품 이음매(좌) 스위치 램프 MB004045
// 	if(bOn)
// 		pDoc->m_pMpeIo[12] &= ~(0x01<<5);						
// 	else
// 		pDoc->m_pMpeIo[12] |= (0x01<<5);						
}

void CDlgMenu03::SwRcReelJoinR(BOOL bOn) 
{
// 	if(bOn)
// 		pDoc->m_pMpeIo[12] |= (0x01<<6);						
// 	else
// 		pDoc->m_pMpeIo[12] &= ~(0x01<<6);						
}

void CDlgMenu03::SwRcReelJoinR() 
{
// 	if(!pDoc->Status.bManual)
// 	{
// 		pView->DispMsg(_T("수동 모드가 아닙니다."), _T("주의", RGB_YELLOW);
// 		return;
// 	}

// 	BOOL bOn = pDoc->m_pMpeIo[12] & (0x01<<6) ? TRUE : FALSE;	// 리코일러 제품 이음매(우) 스위치 램프 MB004046
// 	if(bOn)
// 		pDoc->m_pMpeIo[12] &= ~(0x01<<6);						
// 	else
// 		pDoc->m_pMpeIo[12] |= (0x01<<6);						
}

void CDlgMenu03::SwRcReelWheel(BOOL bOn) 
{
// 	if(bOn)
// 		pDoc->m_pMpeIo[12] |= (0x01<<7);						
// 	else
// 		pDoc->m_pMpeIo[12] &= ~(0x01<<7);						
}

void CDlgMenu03::SwRcReelWheel() 
{
// 	if(!pDoc->Status.bManual)
// 	{
// 		pView->DispMsg(_T("수동 모드가 아닙니다."), _T("주의", RGB_YELLOW);
// 		return;
// 	}

// 	BOOL bOn = pDoc->m_pMpeIo[12] & (0x01<<7) ? TRUE : FALSE;	// 리코일러 제품휠 지지 스위치 램프 MB004047
// 	if(bOn)
// 		pDoc->m_pMpeIo[12] &= ~(0x01<<7);						
// 	else
// 		pDoc->m_pMpeIo[12] |= (0x01<<7);						
}

void CDlgMenu03::SwRcPprChuck(BOOL bOn) 
{
// 	if(bOn)
// 		pDoc->m_pMpeIo[12] |= (0x01<<8);						
// 	else
// 		pDoc->m_pMpeIo[12] &= ~(0x01<<8);						
}

void CDlgMenu03::SwRcPprChuck() 
{
// 	if(!pDoc->Status.bManual)
// 	{
// 		pView->DispMsg(_T("수동 모드가 아닙니다."), _T("주의", RGB_YELLOW);
// 		return;
// 	}

// 	BOOL bOn = pDoc->m_pMpeIo[12] & (0x01<<8) ? TRUE : FALSE;	// 리코일러 간지척 클램프 스위치 램프 MB004048
// 	if(bOn)
// 		pDoc->m_pMpeIo[12] &= ~(0x01<<8);						
// 	else
// 		pDoc->m_pMpeIo[12] |= (0x01<<8);						
}

void CDlgMenu03::SwRcReelChuck(BOOL bOn) 
{
// 	if(bOn)
// 		pDoc->m_pMpeIo[12] |= (0x01<<11);						
// 	else
// 		pDoc->m_pMpeIo[12] &= ~(0x01<<11);						
}

void CDlgMenu03::SwRcReelChuck() 
{
// 	if(!pDoc->Status.bManual)
// 	{
// 		pView->DispMsg(_T("수동 모드가 아닙니다."), _T("주의", RGB_YELLOW);
// 		return;
// 	}

// 	BOOL bOn = pDoc->m_pMpeIo[12] & (0x01<<11) ? TRUE : FALSE;	// 리코일러 제품척 클램프 스위치 램프 MB00404B
// 	if(bOn)
// 		pDoc->m_pMpeIo[12] &= ~(0x01<<11);						
// 	else
// 		pDoc->m_pMpeIo[12] |= (0x01<<11);						
}

void CDlgMenu03::OnChk16() 
{
	// TODO: Add your control notification handler code here
	BOOL bOn = pDoc->WorkingInfo.LastJob.bMkOnePnl;				// 마킹부 한판넬 이송
	SetMkOnePnl(!bOn);

// 	BOOL bRelation = GetRelation();
// 	if(bRelation)
		SetAoiOnePnl(!bOn);
		SetEngraveOnePnl(!bOn); 
}

void CDlgMenu03::OnChk24() 
{
	// TODO: Add your control notification handler code here
	BOOL bOn = pDoc->WorkingInfo.LastJob.bAoiOnePnl;			// 검사부 한판넬 이송
	SetAoiOnePnl(!bOn);

// 	BOOL bRelation = GetRelation();
// 	if(bRelation)
		SetMkOnePnl(!bOn);
		SetEngraveOnePnl(!bOn);
}

BOOL CDlgMenu03::PreTranslateMessage(MSG* pMsg) 
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

void CDlgMenu03::DoManual()
{
	SetMkOnePnl(FALSE);
	SetAoiOnePnl(FALSE);
	SetEngraveOnePnl(FALSE);

	// 검사부 - FD/TQ 진공 OFF, TBL진공 OFF, TBL파기 ON, 
// 	SwAoiFdVac(FALSE);
// 	SwAoiTqVac(FALSE);
// 	SwAoiTblVac(FALSE);
// 	SwAoiTblBlw(FALSE);
// 	SwAoiDustBlw(FALSE);

	// 마킹부 - FD/TQ 진공 OFF, TBL진공 OFF, TBL파기 ON, 
// 	SwMkFdVac(FALSE);
// 	SwMkTqVac(FALSE);
// 	SwMkTblVac(FALSE);
// 	SwMkTblBlw(FALSE);
// 
// 	SwMkFdClp(FALSE);

	SwStop();

	pView->ClrDispMsg();

// 	SwMkDnSol(FALSE);
}

void CDlgMenu03::DoAuto()
{
	SetMkOnePnl(TRUE);
	SetAoiOnePnl(TRUE);
	SetEngraveOnePnl(TRUE);

// 	BOOL bOn0 = pDoc->m_pMpeIo[12] & (0x01<<1) ? TRUE : FALSE;		// 리코일러 연동 온/오프 스위치 램프 MB004041
// 	BOOL bOn1 = pDoc->m_pMpeIo[8] & (0x01<<1) ? TRUE : FALSE;		// 언코일러 연동 온/오프 스위치 램프 MB003641

// 	BOOL bOn2 = pDoc->m_pSliceIo[7] & (0x01<<1) ? TRUE : FALSE;		// 마킹부 연동 온/오프 스위치 램프
// 	BOOL bOn3 = pDoc->m_pSliceIo[9] & (0x01<<0) ? TRUE : FALSE;		// 검사부 연동 온/오프 스위치 램프

// 	SwMkFdClp(TRUE);

// 	if(!bOn0 || !bOn1)
// 	{
// 		SetRcRelation(TRUE);
// 		SwRcRelation(TRUE);
// 	}

//	SetRelation(TRUE);

// 	SwMkDnSol(TRUE);
}

void CDlgMenu03::InitRelation()
{
// 	BOOL bOn0 = pDoc->m_pMpeIo[12] & (0x01<<1) ? TRUE : FALSE;		// 리코일러 연동 온/오프 스위치 램프 MB004041
// 	BOOL bOn1 = pDoc->m_pMpeIo[8] & (0x01<<1) ? TRUE : FALSE;		// 언코일러 연동 온/오프 스위치 램프 MB003641

// 	BOOL bOn2 = pDoc->m_pSliceIo[7] & (0x01<<1) ? TRUE : FALSE;		// 마킹부 연동 온/오프 스위치 램프
// 	BOOL bOn3 = pDoc->m_pSliceIo[9] & (0x01<<0) ? TRUE : FALSE;		// 검사부 연동 온/오프 스위치 램프

// 	if(bOn0 || bOn1)
// 	{
// 		if(!bOn2 || !bOn3)
// 			SetRelation(TRUE);
// 	}
// 
// 	if(!bOn0 && !bOn1)
// 	{
// 		if(bOn2 || bOn3)
// 			SetRelation(FALSE);
// 	}
}

void CDlgMenu03::OnChk62() 
{
	// TODO: Add your control notification handler code here
	BOOL bOn = pDoc->WorkingInfo.LastJob.bAoiOnePnl;			// 검사부 한판넬 이송
	SetAoiOnePnl(!bOn);

	BOOL bRelation = GetRelation();
	if (bRelation)
	{
		SetMkOnePnl(!bOn);
		SetEngraveOnePnl(!bOn);
	}
}

void CDlgMenu03::SetCore150mmRecoiler(BOOL bOn)
{
#ifdef USE_MPE
	pView->m_pMpe->Write(_T("MB44017E"), bOn?1:0);
#endif
	CString sData, sPath=PATH_WORKING_INFO;
	pDoc->WorkingInfo.LastJob.bCore150Recoiler = bOn;
	sData.Format(_T("%d"), pDoc->WorkingInfo.LastJob.bCore150Recoiler?1:0);
	::WritePrivateProfileString(_T("Last Job"), _T("Recoiler Core 150mm"), sData, sPath);
}

void CDlgMenu03::SetCore150mmUncoiler(BOOL bOn)
{
#ifdef USE_MPE
	pView->m_pMpe->Write(_T("MB44017F"), bOn?1:0);
#endif
	CString sData, sPath=PATH_WORKING_INFO;
	pDoc->WorkingInfo.LastJob.bCore150Uncoiler = bOn;
	sData.Format(_T("%d"), pDoc->WorkingInfo.LastJob.bCore150Uncoiler?1:0);
	::WritePrivateProfileString(_T("Last Job"), _T("Uncoiler Core 150mm"), sData, sPath);
}

void CDlgMenu03::SetDualTest(BOOL bOn)
{
	if(bOn)
	{
		if(!myStcTitle[61].IsWindowVisible())
			myStcTitle[61].ShowWindow(SW_SHOW);
		if(!myBtn[55].IsWindowVisible())
			myBtn[55].ShowWindow(SW_SHOW);
		
		if(!myStcTitle[62].IsWindowVisible())
			myStcTitle[62].ShowWindow(SW_SHOW);
		if(!myBtn[56].IsWindowVisible())
			myBtn[56].ShowWindow(SW_SHOW);
		
		if(!myStcTitle[63].IsWindowVisible())
			myStcTitle[63].ShowWindow(SW_SHOW);
		if(!myBtn[57].IsWindowVisible())
			myBtn[57].ShowWindow(SW_SHOW);
		
		if(!myStcTitle[64].IsWindowVisible())
			myStcTitle[64].ShowWindow(SW_SHOW);
		if(!myBtn[58].IsWindowVisible())
			myBtn[58].ShowWindow(SW_SHOW);
		
		if(!myStcTitle[65].IsWindowVisible())
			myStcTitle[65].ShowWindow(SW_SHOW);
		if(!myBtn[59].IsWindowVisible())
			myBtn[59].ShowWindow(SW_SHOW);
		
		if(!myStcTitle[66].IsWindowVisible())
			myStcTitle[66].ShowWindow(SW_SHOW);
		if(!myBtn[60].IsWindowVisible())
			myBtn[60].ShowWindow(SW_SHOW);
		
		if(!myStcTitle[67].IsWindowVisible())
			myStcTitle[67].ShowWindow(SW_SHOW);
		if(!myBtn[61].IsWindowVisible())
			myBtn[61].ShowWindow(SW_SHOW);
		
		if(!myStcTitle[68].IsWindowVisible())
			myStcTitle[68].ShowWindow(SW_SHOW);
		if(!myBtn[62].IsWindowVisible())
			myBtn[62].ShowWindow(SW_SHOW);
		
		if(!myStcTitle[69].IsWindowVisible())
			myStcTitle[69].ShowWindow(SW_SHOW);
		if(!myBtn[63].IsWindowVisible())
			myBtn[63].ShowWindow(SW_SHOW);
		
		if(!myStcTitle[70].IsWindowVisible())
			myStcTitle[70].ShowWindow(SW_SHOW);
		if(!myBtn[64].IsWindowVisible())
			myBtn[64].ShowWindow(SW_SHOW);
		
		if(!myStcTitle[71].IsWindowVisible())
			myStcTitle[71].ShowWindow(SW_SHOW);
		if(!myBtn[65].IsWindowVisible())
			myBtn[65].ShowWindow(SW_SHOW);
	}
	else
	{
		if(myStcTitle[61].IsWindowVisible())
			myStcTitle[61].ShowWindow(SW_HIDE);
		if(myBtn[55].IsWindowVisible())
			myBtn[55].ShowWindow(SW_HIDE);
		
		if(myStcTitle[62].IsWindowVisible())
			myStcTitle[62].ShowWindow(SW_HIDE);
		if(myBtn[56].IsWindowVisible())
			myBtn[56].ShowWindow(SW_HIDE);
		
		if(myStcTitle[63].IsWindowVisible())
			myStcTitle[63].ShowWindow(SW_HIDE);
		if(myBtn[57].IsWindowVisible())
			myBtn[57].ShowWindow(SW_HIDE);
		
		if(myStcTitle[64].IsWindowVisible())
			myStcTitle[64].ShowWindow(SW_HIDE);
		if(myBtn[58].IsWindowVisible())
			myBtn[58].ShowWindow(SW_HIDE);
		
		if(myStcTitle[65].IsWindowVisible())
			myStcTitle[65].ShowWindow(SW_HIDE);
		if(myBtn[59].IsWindowVisible())
			myBtn[59].ShowWindow(SW_HIDE);
		
		if(myStcTitle[66].IsWindowVisible())
			myStcTitle[66].ShowWindow(SW_HIDE);
		if(myBtn[60].IsWindowVisible())
			myBtn[60].ShowWindow(SW_HIDE);
		
		if(myStcTitle[67].IsWindowVisible())
			myStcTitle[67].ShowWindow(SW_HIDE);
		if(myBtn[61].IsWindowVisible())
			myBtn[61].ShowWindow(SW_HIDE);
		
		if(myStcTitle[68].IsWindowVisible())
			myStcTitle[68].ShowWindow(SW_HIDE);
		if(myBtn[62].IsWindowVisible())
			myBtn[62].ShowWindow(SW_HIDE);
		
		if(myStcTitle[69].IsWindowVisible())
			myStcTitle[69].ShowWindow(SW_HIDE);
		if(myBtn[63].IsWindowVisible())
			myBtn[63].ShowWindow(SW_HIDE);
		
		if(myStcTitle[70].IsWindowVisible())
			myStcTitle[70].ShowWindow(SW_HIDE);
		if(myBtn[64].IsWindowVisible())
			myBtn[64].ShowWindow(SW_HIDE);
		
		if(myStcTitle[71].IsWindowVisible())
			myStcTitle[71].ShowWindow(SW_HIDE);
		if(myBtn[65].IsWindowVisible())
			myBtn[65].ShowWindow(SW_HIDE);
	}
}

BOOL CDlgMenu03::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB_DLG_FRM);
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}

void CDlgMenu03::SetEngraveOnePnl(BOOL bOn)
{
#ifdef USE_MPE
	//pView->IoWrite("MB440151", bOn?1:0);	// 한판넬 이송상태 ON (PC가 ON, OFF)
	pView->m_pMpe->Write(_T("MB440151"), bOn ? 1 : 0);
#endif
	CString sData, sPath = PATH_WORKING_INFO;
	pDoc->WorkingInfo.LastJob.bEngraveOnePnl = bOn;
	sData.Format(_T("%d"), pDoc->WorkingInfo.LastJob.bEngraveOnePnl ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Engrave One Pannel Move On"), sData, sPath);

}

void CDlgMenu03::OnBnClickedChk79()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BOOL bOn = pDoc->WorkingInfo.LastJob.bEngraveOnePnl;				// 각인부 한판넬 이송

	SetMkOnePnl(!bOn);
	SetAoiOnePnl(!bOn);
	SetEngraveOnePnl(!bOn);
}

void CDlgMenu03::MoveEngrave(double dOffset)
{
	double fLen, fVel, fAcc, fTarget;

	fLen = pDoc->GetOnePnlLen();
	fVel = pDoc->GetOnePnlVel();
	fAcc = pDoc->GetOnePnlAcc();
	fTarget = fLen + dOffset;

	if (pView->m_pMpe)
	{
#ifdef USE_MPE
		long lData = (long)(fTarget * 1000.0);
		pView->m_pMpe->Write(_T("ML45078"), lData);	// 각인부 Feeding 롤러 Offset(*1000, +:더 보냄, -덜 보냄, PC가 쓰고 PLC에서 지움)
#endif
		MoveEngrave();
	}
}

void CDlgMenu03::MoveEngrave(int nDir)
{
	if (pDoc->WorkingInfo.LastJob.bEngraveOnePnl)
	{
#ifdef USE_MPE
		if (pView->m_pMpe)
			pView->m_pMpe->Write(_T("MB44017C"), 1);	// 각인부 피딩 CW ON (PLC가 피딩완료 후 OFF)
#endif
	}
}

void CDlgMenu03::ChkDoneEngrave()
{
	if (pDoc->WorkingInfo.LastJob.bEngraveOnePnl)
	{
		if (!m_bTIM_CHK_DONE_ENGRAVE)
		{
			m_bTIM_CHK_DONE_ENGRAVE = TRUE;
			SetTimer(TIM_CHK_DONE_ENGRAVE, 100, NULL);
		}
	}
}

BOOL CDlgMenu03::IsEngraveFdVac()
{
	if (!pDoc->m_pMpeIo)
		return FALSE;

	// MpeIO
	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;

#ifdef USE_MPE
	BOOL bOn = pDoc->m_pMpeIo[nInSeg + 21] & (0x01 << 5);		// 각인부 피딩 진공 스위치 램프
	return bOn;
#endif

	return FALSE;
}

BOOL CDlgMenu03::IsEngraveTqVac()
{
	if (!pDoc->m_pMpeIo)
		return FALSE;

	// MpeIO
	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;

#ifdef USE_MPE
	BOOL bOn = pDoc->m_pMpeIo[nInSeg + 21] & (0x01 << 6);		// 각인부 토크 진공 스위치 램프
	return bOn;
#endif

	return FALSE;
}

BOOL CDlgMenu03::IsEngraveTblVac()
{
	if (!pDoc->m_pMpeIo)
		return FALSE;

	// MpeIO
	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;

#ifdef USE_MPE
	BOOL bOn = pDoc->m_pMpeIo[nInSeg + 21] & (0x01 << 7);		// 각인부 테이블 진공 스위치 램프
	return bOn;
#endif

	return FALSE;
}

BOOL CDlgMenu03::IsEngraveTblBlw()
{
	if (!pDoc->m_pMpeIo)
		return FALSE;

	// MpeIO
	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;

#ifdef USE_MPE
	BOOL bOn = pDoc->m_pMpeIo[nInSeg + 21] & (0x01 << 2);		// 각인부 테이블 브로워 스위치 램프
	return bOn;
#endif

	return FALSE;
}

void CDlgMenu03::UpdateData()
{
	;
}

void CDlgMenu03::UpdateSignal()
{
	Disp();
}

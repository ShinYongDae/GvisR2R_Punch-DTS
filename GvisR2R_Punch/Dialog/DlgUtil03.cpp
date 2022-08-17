// DlgUtil03.cpp : implementation file
//

#include "stdafx.h"
#include "../gvisr2r_punch.h"
#include "DlgUtil03.h"
#include "DlgKeyNum.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgUtil03 dialog


#include "../GvisR2R_PunchDoc.h"
#include "../GvisR2R_PunchView.h"

extern CGvisR2R_PunchDoc* pDoc;
extern CGvisR2R_PunchView* pView;

CDlgUtil03::CDlgUtil03(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUtil03::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgUtil03)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pRect = NULL;
	nScrlV = 0;
	nScrlH = 0;
	m_bLoadImg = FALSE;
}

CDlgUtil03::~CDlgUtil03()
{
	if(m_pRect)
	{
		delete m_pRect;
		m_pRect = NULL;
	}
}

BOOL CDlgUtil03::Create()
{
	return CDialog::Create(CDlgUtil03::IDD);
}


void CDlgUtil03::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgUtil03)
	DDX_Control(pDX, IDC_SCROLLBAR_V, m_scrollV);
	DDX_Control(pDX, IDC_SCROLLBAR_H, m_scrollH);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgUtil03, CDialog)
	//{{AFX_MSG_MAP(CDlgUtil03)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_MK, OnBtnMk)
	ON_BN_CLICKED(IDC_STC_0_0, OnStc00)
	ON_BN_CLICKED(IDC_STC_0_1, OnStc01)
	ON_BN_CLICKED(IDC_STC_0_2, OnStc02)
	ON_BN_CLICKED(IDC_STC_0_3, OnStc03)
	ON_BN_CLICKED(IDC_STC_0_4, OnStc04)
	ON_BN_CLICKED(IDC_STC_0_5, OnStc05)
	ON_BN_CLICKED(IDC_STC_0_6, OnStc06)
	ON_BN_CLICKED(IDC_STC_0_7, OnStc07)
	ON_BN_CLICKED(IDC_STC_0_8, OnStc08)
	ON_BN_CLICKED(IDC_STC_0_9, OnStc09)
	ON_BN_CLICKED(IDC_STC_1_0, OnStc10)
	ON_BN_CLICKED(IDC_STC_1_1, OnStc11)
	ON_BN_CLICKED(IDC_STC_1_2, OnStc12)
	ON_BN_CLICKED(IDC_STC_1_3, OnStc13)
	ON_BN_CLICKED(IDC_STC_1_4, OnStc14)
	ON_BN_CLICKED(IDC_STC_1_5, OnStc15)
	ON_BN_CLICKED(IDC_STC_1_6, OnStc16)
	ON_BN_CLICKED(IDC_STC_1_7, OnStc17)
	ON_BN_CLICKED(IDC_STC_1_8, OnStc18)
	ON_BN_CLICKED(IDC_STC_1_9, OnStc19)
	ON_BN_CLICKED(IDC_STC_2_0, OnStc20)
	ON_BN_CLICKED(IDC_STC_2_1, OnStc21)
	ON_BN_CLICKED(IDC_STC_2_2, OnStc22)
	ON_BN_CLICKED(IDC_STC_2_3, OnStc23)
	ON_BN_CLICKED(IDC_STC_2_4, OnStc24)
	ON_BN_CLICKED(IDC_STC_2_5, OnStc25)
	ON_BN_CLICKED(IDC_STC_2_6, OnStc26)
	ON_BN_CLICKED(IDC_STC_2_7, OnStc27)
	ON_BN_CLICKED(IDC_STC_2_8, OnStc28)
	ON_BN_CLICKED(IDC_STC_2_9, OnStc29)
	ON_BN_CLICKED(IDC_STC_3_0, OnStc30)
	ON_BN_CLICKED(IDC_STC_3_1, OnStc31)
	ON_BN_CLICKED(IDC_STC_3_2, OnStc32)
	ON_BN_CLICKED(IDC_STC_3_3, OnStc33)
	ON_BN_CLICKED(IDC_STC_3_4, OnStc34)
	ON_BN_CLICKED(IDC_STC_3_5, OnStc35)
	ON_BN_CLICKED(IDC_STC_3_6, OnStc36)
	ON_BN_CLICKED(IDC_STC_3_7, OnStc37)
	ON_BN_CLICKED(IDC_STC_3_8, OnStc38)
	ON_BN_CLICKED(IDC_STC_3_9, OnStc39)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
 	ON_BN_CLICKED(IDC_BTN_ALL_MK_TEST, OnBtnAllMkTest)
	ON_BN_CLICKED(IDC_CHK_NO_MK, OnChkNoMk)
	ON_BN_CLICKED(IDC_STC_0_10, OnStc010)
	ON_BN_CLICKED(IDC_STC_1_10, OnStc110)
	ON_BN_CLICKED(IDC_STC_2_10, OnStc210)
	ON_BN_CLICKED(IDC_STC_4_0, OnStc40)
	ON_BN_CLICKED(IDC_STC_4_1, OnStc41)
	ON_BN_CLICKED(IDC_STC_4_2, OnStc42)
	ON_BN_CLICKED(IDC_STC_4_3, OnStc43)
	ON_BN_CLICKED(IDC_STC_4_4, OnStc44)
	ON_BN_CLICKED(IDC_STC_4_5, OnStc45)
	ON_BN_CLICKED(IDC_STC_4_6, OnStc46)
	ON_BN_CLICKED(IDC_STC_4_7, OnStc47)
	ON_BN_CLICKED(IDC_STC_4_8, OnStc48)
	ON_BN_CLICKED(IDC_STC_4_9, OnStc49)
	ON_BN_CLICKED(IDC_STC_4_10, OnStc410)
	ON_BN_CLICKED(IDC_STC_5_0, OnStc50)
	ON_BN_CLICKED(IDC_STC_5_1, OnStc51)
	ON_BN_CLICKED(IDC_STC_5_2, OnStc52)
	ON_BN_CLICKED(IDC_STC_5_3, OnStc53)
	ON_BN_CLICKED(IDC_STC_5_4, OnStc54)
	ON_BN_CLICKED(IDC_STC_5_5, OnStc55)
	ON_BN_CLICKED(IDC_STC_5_6, OnStc56)
	ON_BN_CLICKED(IDC_STC_5_7, OnStc57)
	ON_BN_CLICKED(IDC_STC_5_8, OnStc58)
	ON_BN_CLICKED(IDC_STC_5_9, OnStc59)
	ON_BN_CLICKED(IDC_STC_5_10, OnStc510)
	ON_BN_CLICKED(IDC_STC_3_10, OnStc310)
	ON_BN_CLICKED(IDC_BTN_MK_MOVE_INIT, OnBtnMkMoveInit)
	ON_BN_CLICKED(IDC_BTN_MK_HOME, OnBtnMkHome)
	ON_WM_KILLFOCUS()
	ON_BN_CLICKED(IDC_EDIT_YSHIFT_RATIO, OnEditYshiftRatio)
	ON_BN_CLICKED(IDC_CHECK_YSHIFT, OnCheckYshift)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MYBTN_DOWN, OnMyBtnDown)
	ON_MESSAGE(WM_MYBTN_UP, OnMyBtnUp)
	ON_BN_CLICKED(IDC_CHECK_PCS, &CDlgUtil03::OnBnClickedCheckPcs)
	ON_BN_CLICKED(IDC_CHECK_PCS_DN, &CDlgUtil03::OnBnClickedCheckPcsDn)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgUtil03 message handlers

LRESULT CDlgUtil03::OnMyBtnDown(WPARAM wPara, LPARAM lPara)
{
	int nCtrlID = (int)lPara;
	switch(nCtrlID)
	{
	case IDC_CHK_NO_MK:
		pDoc->WorkingInfo.System.bNoMk = TRUE;
		break;
	}
	return 0L;
}

LRESULT CDlgUtil03::OnMyBtnUp(WPARAM wPara, LPARAM lPara)
{
	int nCtrlID = (int)lPara;
	switch(nCtrlID)
	{
	case IDC_CHK_NO_MK:
		pDoc->WorkingInfo.System.bNoMk = FALSE;
		break;
	}
	return 0L;
}

void CDlgUtil03::OnShowWindow(BOOL bShow, UINT nStatus) 
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

void CDlgUtil03::AtDlgShow()
{
	if(pDoc->m_Master[0].m_pPcsRgn)
		SetScrlBarMax(pDoc->m_Master[0].m_pPcsRgn->nCol, pDoc->m_Master[0].m_pPcsRgn->nRow); // ROT_NONE
// 	SetScrlBarMax(pDoc->m_pPcsRgn->nRow, pDoc->m_pPcsRgn->nCol); // ROT_CCW_90
	SetScrlBar(0,0);
	
	LoadImg();
	Disp(ROT_NONE);
// 	Disp(ROT_CCW_90);
}

void CDlgUtil03::AtDlgHide()
{
	DelImg();
}

void CDlgUtil03::LoadImg()
{
	if(m_bLoadImg)
		return;
	m_bLoadImg = TRUE;

	int i;
	for(i=0; i<MAX_UTIL03_BTN; i++)
	{
		myBtn[i].LoadBkImage(IMG_BTN_UP_DlgUtil03, BTN_IMG_UP);
		myBtn[i].LoadBkImage(IMG_BTN_DN_DlgUtil03, BTN_IMG_DN);
	}
}

void CDlgUtil03::DelImg()
{
	if(!m_bLoadImg)
		return;
	m_bLoadImg = FALSE;

	int i;
	for(i=0; i<MAX_UTIL03_BTN; i++)
	{
		myBtn[i].DelImgList();
	}
}

BOOL CDlgUtil03::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitStc();
	InitBtn();

	if(pDoc->m_Master[0].m_pPcsRgn)
		SetScrlBarMax(pDoc->m_Master[0].m_pPcsRgn->nCol, pDoc->m_Master[0].m_pPcsRgn->nRow); // ROT_NONE
// 	SetScrlBarMax(pDoc->m_pPcsRgn->nRow, pDoc->m_pPcsRgn->nCol); // ROT_CCW_90
	SetScrlBar(0,0);

	if(pDoc->WorkingInfo.System.bNoMk)
	{
		myBtn[2].SetCheck(TRUE);
	}
	else
	{
		myBtn[2].EnableWindow(FALSE);
	}

	// 20160926-syd
	((CButton*)GetDlgItem(IDC_CHECK_YSHIFT))->SetCheck(pDoc->m_bUseRTRYShiftAdjust);
	CString strRatio;

	strRatio.Format(_T("%3.2f"), pDoc->m_dShiftAdjustRatio);
	GetDlgItem(IDC_EDIT_YSHIFT_RATIO)->SetWindowText(strRatio);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgUtil03::InitBtn()
{
	myBtn[0].SubclassDlgItem(IDC_BTN_MK, this);
	myBtn[0].SetHwnd(this->GetSafeHwnd(), IDC_BTN_MK);
	myBtn[1].SubclassDlgItem(IDC_BTN_ALL_MK_TEST, this);
	myBtn[1].SetHwnd(this->GetSafeHwnd(), IDC_BTN_ALL_MK_TEST);
	myBtn[1].SetBtnType(BTN_TYPE_CHECK);
	myBtn[2].SubclassDlgItem(IDC_CHK_NO_MK, this);
	myBtn[2].SetHwnd(this->GetSafeHwnd(), IDC_CHK_NO_MK);
	myBtn[2].SetBtnType(BTN_TYPE_CHECK);
	myBtn[2].SetText(_T("마킹모드"), BTN_UP);
	myBtn[2].SetText(_T("비젼모드"), BTN_DN);

	myBtn[3].SubclassDlgItem(IDC_CHK_LEFT, this);
	myBtn[3].SetHwnd(this->GetSafeHwnd(), IDC_CHK_LEFT);
	myBtn[3].SetBtnType(BTN_TYPE_CHECK);
	myBtn[4].SubclassDlgItem(IDC_CHK_RIGHT, this);
	myBtn[4].SetHwnd(this->GetSafeHwnd(), IDC_CHK_RIGHT);
	myBtn[4].SetBtnType(BTN_TYPE_CHECK);

	myBtn[5].SubclassDlgItem(IDC_BTN_MK_MOVE_INIT, this);
	myBtn[5].SetHwnd(this->GetSafeHwnd(), IDC_BTN_MK_MOVE_INIT);
	myBtn[6].SubclassDlgItem(IDC_BTN_MK_HOME, this);
	myBtn[6].SetHwnd(this->GetSafeHwnd(), IDC_BTN_MK_HOME);
	
	int i;
	for(i=0; i<MAX_UTIL03_BTN; i++)
	{
		myBtn[i].SetFont(_T("굴림체"),16,TRUE);
		myBtn[i].SetTextColor(RGB_BLACK);
// 		myBtn[i].SetBtnType(BTN_TYPE_CHECK);
	}
}

void CDlgUtil03::InitStc()
{
	myStcData[0].SubclassDlgItem(IDC_STC_0_0, this);
	myStcData[1].SubclassDlgItem(IDC_STC_0_1, this);
	myStcData[2].SubclassDlgItem(IDC_STC_0_2, this);
	myStcData[3].SubclassDlgItem(IDC_STC_0_3, this);
	myStcData[4].SubclassDlgItem(IDC_STC_0_4, this);
	myStcData[5].SubclassDlgItem(IDC_STC_0_5, this);
	myStcData[6].SubclassDlgItem(IDC_STC_0_6, this);
	myStcData[7].SubclassDlgItem(IDC_STC_0_7, this);
	myStcData[8].SubclassDlgItem(IDC_STC_0_8, this);
	myStcData[9].SubclassDlgItem(IDC_STC_0_9, this);
	myStcData[10].SubclassDlgItem(IDC_STC_0_10, this);

	myStcData[11].SubclassDlgItem(IDC_STC_1_0, this);
	myStcData[12].SubclassDlgItem(IDC_STC_1_1, this);
	myStcData[13].SubclassDlgItem(IDC_STC_1_2, this);
	myStcData[14].SubclassDlgItem(IDC_STC_1_3, this);
	myStcData[15].SubclassDlgItem(IDC_STC_1_4, this);
	myStcData[16].SubclassDlgItem(IDC_STC_1_5, this);
	myStcData[17].SubclassDlgItem(IDC_STC_1_6, this);
	myStcData[18].SubclassDlgItem(IDC_STC_1_7, this);
	myStcData[19].SubclassDlgItem(IDC_STC_1_8, this);
	myStcData[20].SubclassDlgItem(IDC_STC_1_9, this);
	myStcData[21].SubclassDlgItem(IDC_STC_1_10, this);

	myStcData[22].SubclassDlgItem(IDC_STC_2_0, this);
	myStcData[23].SubclassDlgItem(IDC_STC_2_1, this);
	myStcData[24].SubclassDlgItem(IDC_STC_2_2, this);
	myStcData[25].SubclassDlgItem(IDC_STC_2_3, this);
	myStcData[26].SubclassDlgItem(IDC_STC_2_4, this);
	myStcData[27].SubclassDlgItem(IDC_STC_2_5, this);
	myStcData[28].SubclassDlgItem(IDC_STC_2_6, this);
	myStcData[29].SubclassDlgItem(IDC_STC_2_7, this);
	myStcData[30].SubclassDlgItem(IDC_STC_2_8, this);
	myStcData[31].SubclassDlgItem(IDC_STC_2_9, this);
	myStcData[32].SubclassDlgItem(IDC_STC_2_10, this);

	myStcData[33].SubclassDlgItem(IDC_STC_3_0, this);
	myStcData[34].SubclassDlgItem(IDC_STC_3_1, this);
	myStcData[35].SubclassDlgItem(IDC_STC_3_2, this);
	myStcData[36].SubclassDlgItem(IDC_STC_3_3, this);
	myStcData[37].SubclassDlgItem(IDC_STC_3_4, this);
	myStcData[38].SubclassDlgItem(IDC_STC_3_5, this);
	myStcData[39].SubclassDlgItem(IDC_STC_3_6, this);
	myStcData[40].SubclassDlgItem(IDC_STC_3_7, this);
	myStcData[41].SubclassDlgItem(IDC_STC_3_8, this);
	myStcData[42].SubclassDlgItem(IDC_STC_3_9, this);
	myStcData[43].SubclassDlgItem(IDC_STC_3_10, this);

	myStcData[44].SubclassDlgItem(IDC_STC_4_0, this);
	myStcData[45].SubclassDlgItem(IDC_STC_4_1, this);
	myStcData[46].SubclassDlgItem(IDC_STC_4_2, this);
	myStcData[47].SubclassDlgItem(IDC_STC_4_3, this);
	myStcData[48].SubclassDlgItem(IDC_STC_4_4, this);
	myStcData[49].SubclassDlgItem(IDC_STC_4_5, this);
	myStcData[50].SubclassDlgItem(IDC_STC_4_6, this);
	myStcData[51].SubclassDlgItem(IDC_STC_4_7, this);
	myStcData[52].SubclassDlgItem(IDC_STC_4_8, this);
	myStcData[53].SubclassDlgItem(IDC_STC_4_9, this);
	myStcData[54].SubclassDlgItem(IDC_STC_4_10, this);

	myStcData[55].SubclassDlgItem(IDC_STC_5_0, this);
	myStcData[56].SubclassDlgItem(IDC_STC_5_1, this);
	myStcData[57].SubclassDlgItem(IDC_STC_5_2, this);
	myStcData[58].SubclassDlgItem(IDC_STC_5_3, this);
	myStcData[59].SubclassDlgItem(IDC_STC_5_4, this);
	myStcData[60].SubclassDlgItem(IDC_STC_5_5, this);
	myStcData[61].SubclassDlgItem(IDC_STC_5_6, this);
	myStcData[62].SubclassDlgItem(IDC_STC_5_7, this);
	myStcData[63].SubclassDlgItem(IDC_STC_5_8, this);
	myStcData[64].SubclassDlgItem(IDC_STC_5_9, this);
	myStcData[65].SubclassDlgItem(IDC_STC_5_10, this);

	myStcData[66].SubclassDlgItem(IDC_EDIT_YSHIFT_RATIO, this);

	for(int i=0; i<MAX_UTIL03_STC_DATA; i++)
	{
		myStcData[i].SetFontName(_T("Arial"));
		myStcData[i].SetFontSize(12);
		myStcData[i].SetFontBold(TRUE);
		myStcData[i].SetTextColor(RGB_BLACK);
		myStcData[i].SetBkColor(RGB_WHITE);
	}
}

void CDlgUtil03::Disp(int nDir)
{
	if(!pDoc->m_Master[0].m_pPcsRgn)
		return;

	int nScrlH = m_scrollH.GetScrollPos();
	int nScrlV = m_scrollV.GetScrollPos();

	CString str;
	int nC=0, nR=0, nP=0, nStc=0;
	CfPoint ptMk;
	CRect ptRect;

	BOOL bChk = ((CButton*)GetDlgItem(IDC_CHECK_PCS))->GetCheck();
	BOOL bChk2 = ((CButton*)GetDlgItem(IDC_CHECK_PCS_DN))->GetCheck();

	switch(nDir)
	{
	case ROT_NONE:
		for(nC=0; nC<MAX_SPREAD_COL; nC++)			// nC, nR : Cam(or Reelmap) 기준
		{
			for(nR=0; nR<MAX_SPREAD_ROW; nR++)
			{
				if(!bChk)
				{
					if (pDoc->m_Master[0].m_pPcsRgn)
						pDoc->m_Master[0].m_pPcsRgn->GetMkPnt(nC + nScrlH, nR + nScrlV, nP, ptMk);
					str = _T("");
					if (nP >= 0)
					{
						str.Format(_T("%03d(%03d,%03d)"), nP, (int)ptMk.y, (int)ptMk.x);
					}
				}
				else
				{
					if (!bChk2) // 상면 PCS
					{
						if (pDoc->m_Master[0].m_pPcsRgn)
							pDoc->m_Master[0].m_pPcsRgn->GetPcsRgn(nC + nScrlH, nR + nScrlV, nP, ptRect);
					}
					else		// 하면 PCS
					{
						if (pDoc->m_Master[1].m_pPcsRgn)
							pDoc->m_Master[1].m_pPcsRgn->GetPcsRgn(nC + nScrlH, nR + nScrlV, nP, ptRect);
					}
					str = _T("");
					if (nP >= 0)
					{
						str.Format(_T("%03d(%03d,%03d)"), nP, (int)((ptRect.left + ptRect.right) / 2), (int)((ptRect.top + ptRect.bottom) / 2));
					}
				}
				myStcData[nStc++].SetText(str);
			}
		}
		break;
	case ROT_CCW_90:
// 		for(int nC=0; nC<MAX_SPREAD_COL; nC++)			// nC, nR : Cam(or Reelmap) 기준
// 		{
// 			for(int nR=0; nR<MAX_SPREAD_ROW; nR++)
// 			{
// 				pDoc->m_pPcsRgn->GetMkPnt(nC+nScrlV, nR+nScrlH, nP, ptMk);
// 				str = _T("");
// 				if(nP >= 0)
// 				{
//  					str.Format(_T("%03d(%03d,%03d)", nP, (int)ptMk.y, (int)ptMk.x);
// 				}
// 				myStcData[nStc++].SetText(str);
// 			}
// 		}
		break;
	}

}

void CDlgUtil03::OnBtnMk() 
{
	// TODO: Add your control notification handler code here
	BOOL bOn0 = myBtn[3].GetCheck();
	BOOL bOn1 = myBtn[4].GetCheck();

	if(pView->m_pDlgMenu02)
	{
		if(bOn0 && !bOn1)
		{
			pView->m_pDlgMenu02->SwMarking();
		}
		else if(!bOn0 && bOn1)
		{
			pView->m_pDlgMenu02->SwMarking2();
		}
		else if(bOn0 && bOn1)
		{
			pView->m_pDlgMenu02->SwMarking();
			pView->m_pDlgMenu02->SwMarking2();
		}
	}
}

void CDlgUtil03::MoveMkPos(int nStcId)
{
	CString str;
	int nPos, nPcsId;
	CfPoint ptPnt;
	double dCurrX, dCurrY;
	double pPos[2];
	double fLen, fVel, fAcc, fJerk;

	BOOL bOn0 = myBtn[3].GetCheck();
	BOOL bOn1 = myBtn[4].GetCheck();

	myStcData[nStcId].SetBkColor(RGB_PINK);

	if(bOn0 && !bOn1)
	{
		str = myStcData[nStcId].GetText();
		nPos = str.Find('(', 0);
		nPcsId = _tstoi(str.Left(nPos));
		if(pDoc->m_Master[0].m_pPcsRgn)
			ptPnt = pDoc->m_Master[0].m_pPcsRgn->GetMkPnt0(nPcsId);

		dCurrX = pView->m_dEnc[AXIS_X0]; // pView->m_pMotion->GetActualPosition(AXIS_X);
		dCurrY = pView->m_dEnc[AXIS_Y0]; // pView->m_pMotion->GetActualPosition(AXIS_Y);
		
		pPos[0] = ptPnt.x;
		pPos[1] = ptPnt.y;

		fLen = sqrt( ((pPos[0] - dCurrX) * (pPos[0] - dCurrX)) + ((pPos[1] - dCurrY) * (pPos[1] - dCurrY)) );
		if(fLen > 0.001)
		{
			pView->m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X0, fLen, fVel, fAcc, fJerk);
			if(!pView->m_pMotion->Move(MS_X0Y0, pPos, fVel, fAcc, fAcc))
				AfxMessageBox(_T("Move X0Y0 Error..."));
		}
	}
	else if(!bOn0 && bOn1)
	{
		str = myStcData[nStcId].GetText();
		nPos = str.Find('(', 0);
		nPcsId = _tstoi(str.Left(nPos));
		if(pDoc->m_Master[0].m_pPcsRgn)
			ptPnt = pDoc->m_Master[0].m_pPcsRgn->GetMkPnt1(nPcsId);

		dCurrX = pView->m_dEnc[AXIS_X1]; // pView->m_pMotion->GetActualPosition(AXIS_X);
		dCurrY = pView->m_dEnc[AXIS_Y1]; // pView->m_pMotion->GetActualPosition(AXIS_Y);
		
		pPos[0] = ptPnt.x;
		pPos[1] = ptPnt.y;

		fLen = sqrt( ((pPos[0] - dCurrX) * (pPos[0] - dCurrX)) + ((pPos[1] - dCurrY) * (pPos[1] - dCurrY)) );
		if(fLen > 0.001)
		{
			pView->m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X1, fLen, fVel, fAcc, fJerk);
			if(!pView->m_pMotion->Move(MS_X1Y1, pPos, fVel, fAcc, fAcc))
				AfxMessageBox(_T("Move X1Y1 Error..."));
		}
	}
	else if(bOn0 && bOn1)
	{
		str = myStcData[nStcId].GetText();
		nPos = str.Find('(', 0);
		nPcsId = _tstoi(str.Left(nPos));
		if(pDoc->m_Master[0].m_pPcsRgn)
			ptPnt = pDoc->m_Master[0].m_pPcsRgn->GetMkPnt0(nPcsId);

		dCurrX = pView->m_dEnc[AXIS_X0]; // pView->m_pMotion->GetActualPosition(AXIS_X);
		dCurrY = pView->m_dEnc[AXIS_Y0]; // pView->m_pMotion->GetActualPosition(AXIS_Y);
		
		pPos[0] = ptPnt.x;
		pPos[1] = ptPnt.y;

		fLen = sqrt( ((pPos[0] - dCurrX) * (pPos[0] - dCurrX)) + ((pPos[1] - dCurrY) * (pPos[1] - dCurrY)) );
		if(fLen > 0.001)
		{
			pView->m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X0, fLen, fVel, fAcc, fJerk);
			if(!pView->m_pMotion->Move(MS_X0Y0, pPos, fVel, fAcc, fAcc))
				AfxMessageBox(_T("Move X0Y0 Error..."));
		}

		nPos = str.Find('(', 0);
		nPcsId = _tstoi(str.Left(nPos));
		if(pDoc->m_Master[0].m_pPcsRgn)
			ptPnt = pDoc->m_Master[0].m_pPcsRgn->GetMkPnt1(nPcsId);

		dCurrX = pView->m_dEnc[AXIS_X1]; // pView->m_pMotion->GetActualPosition(AXIS_X);
		dCurrY = pView->m_dEnc[AXIS_Y1]; // pView->m_pMotion->GetActualPosition(AXIS_Y);
		
		pPos[0] = ptPnt.x;
		pPos[1] = ptPnt.y;

		fLen = sqrt( ((pPos[0] - dCurrX) * (pPos[0] - dCurrX)) + ((pPos[1] - dCurrY) * (pPos[1] - dCurrY)) );
		if(fLen > 0.001)
		{
			pView->m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X1, fLen, fVel, fAcc, fJerk);
			if(!pView->m_pMotion->Move(MS_X1Y1, pPos, fVel, fAcc, fAcc))
				AfxMessageBox(_T("Move X1Y1 Error..."));
		}
	}


	myStcData[nStcId].SetBkColor(RGB_WHITE);
}

void CDlgUtil03::SetScrlBarMax(int nMaxH, int nMaxV)
{
	int nSet=0;
	SCROLLINFO si;
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;

    m_scrollH.GetScrollInfo(&si);
    si.nMin = 0;
    si.nPage = 1;	
	si.nPos = 0;

	nSet = (nMaxH-MAX_SPREAD_COL)>0 ? (nMaxH-MAX_SPREAD_COL) : 0;
    si.nMax = nSet;
	m_scrollH.SetScrollInfo(&si);
    m_scrollH.SetScrollPos(si.nPos, TRUE);

    m_scrollV.GetScrollInfo(&si);
    si.nMin = 0;
    si.nPage = 1;	
	si.nPos = 0;

	nSet = (nMaxV-MAX_SPREAD_ROW)>0 ? (nMaxV-MAX_SPREAD_ROW) : 0;
    si.nMax = nSet;
	m_scrollV.SetScrollInfo(&si);
    m_scrollV.SetScrollPos(si.nPos, TRUE);
}

void CDlgUtil03::SetScrlBar(int nCurH, int nCurV)
{
	SCROLLINFO si;
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;

    m_scrollH.GetScrollInfo(&si);

	si.nPos = nCurH;
	m_scrollH.SetScrollInfo(&si);
    m_scrollH.SetScrollPos(si.nPos, TRUE);

    m_scrollV.GetScrollInfo(&si);

	si.nPos = nCurV;
	m_scrollV.SetScrollInfo(&si);
    m_scrollV.SetScrollPos(si.nPos, TRUE);
}

void CDlgUtil03::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default

	int nStep=1, nCurPos;

	if(m_scrollH.m_hWnd == pScrollBar->m_hWnd)
	{
		SCROLLINFO si =
		{
			sizeof(SCROLLINFO),
				SIF_ALL | SIF_DISABLENOSCROLL,
		};

		m_scrollH.GetScrollInfo(&si);

		switch (nSBCode)
		{
		case SB_BOTTOM:         // scroll to bottom
			nCurPos = 0;
			break;
		case SB_TOP:            // scroll to top
			nCurPos = si.nMax;
			break;
		case SB_PAGEDOWN:       // scroll one page down
			nCurPos = si.nPos + nStep;
			if(nCurPos < 0)
				nCurPos = 0;
			break;
		case SB_PAGEUP:         // scroll one page up
			nCurPos = si.nPos - nStep;
			if(nCurPos > si.nMax)
				nCurPos = si.nMax;
			break;
		case SB_LINEDOWN:       // scroll one line up
			nCurPos = si.nPos + nStep;
			if(nCurPos < 0)
				nCurPos = 0;
			break;
		case SB_LINEUP:         // scroll one line up
			nCurPos = si.nPos - nStep;
			if(nCurPos > si.nMax)
				nCurPos = si.nMax;
			break;
		case SB_THUMBTRACK:     // drag scroll box 
			nCurPos = nPos;
			break;
		case SB_THUMBPOSITION:  // scroll to the absolute 
			nCurPos = nPos;
			break;
		case SB_ENDSCROLL:      // end scroll
			//nCurPos = nPos;
			return;
		default:
			return;
		}

		si.nPos = nCurPos;

		if (si.nPos < 0)
			si.nPos = 0;
		if (si.nPos > si.nMax)
			si.nPos = si.nMax;
		m_scrollH.SetScrollInfo(&si);
	}

	Disp(ROT_NONE);
// 	Disp(ROT_CCW_90);
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDlgUtil03::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default

	int nStep=1, nCurPos;

	if(m_scrollV.m_hWnd == pScrollBar->m_hWnd)
	{
		SCROLLINFO si =
		{
			sizeof(SCROLLINFO),
				SIF_ALL | SIF_DISABLENOSCROLL,
		};

		m_scrollV.GetScrollInfo(&si);

		switch (nSBCode)
		{
		case SB_BOTTOM:         // scroll to bottom
			nCurPos = 0;
			break;
		case SB_TOP:            // scroll to top
			nCurPos = si.nMax;
			break;
		case SB_PAGEDOWN:       // scroll one page down
			nCurPos = si.nPos + nStep;
			if(nCurPos < 0)
				nCurPos = 0;
			break;
		case SB_PAGEUP:         // scroll one page up
			nCurPos = si.nPos - nStep;
			if(nCurPos > si.nMax)
				nCurPos = si.nMax;
			break;
		case SB_LINEDOWN:       // scroll one line up
			nCurPos = si.nPos + nStep;
			if(nCurPos < 0)
				nCurPos = 0;
			break;
		case SB_LINEUP:         // scroll one line up
			nCurPos = si.nPos - nStep;
			if(nCurPos > si.nMax)
				nCurPos = si.nMax;
			break;
		case SB_THUMBTRACK:     // drag scroll box 
			nCurPos = nPos;
			break;
		case SB_THUMBPOSITION:  // scroll to the absolute 
			nCurPos = nPos;
			break;
		case SB_ENDSCROLL:      // end scroll
			//nCurPos = nPos;
			return;
		default:
			return;
		}

		si.nPos = nCurPos;

		if (si.nPos < 0)
			si.nPos = 0;
		if (si.nPos > si.nMax)
			si.nPos = si.nMax;
		m_scrollV.SetScrollInfo(&si);
	}
	
	Disp(ROT_NONE);

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CDlgUtil03::OnBtnAllMkTest() 
{
	// TODO: Add your control notification handler code here
	BOOL bOn0 = myBtn[3].GetCheck();
	BOOL bOn1 = myBtn[4].GetCheck();

	BOOL bOn = myBtn[1].GetCheck();

	if(bOn)
	{
		if(bOn0 && !bOn1)
			pView->DoAllMk(CAM_LF);
		else if(!bOn0 && bOn1)
			pView->DoAllMk(CAM_RT);
		else
			pView->DoAllMk(CAM_BOTH);
	}
	else
	{
		pView->StopAllMk();	
	}
}

// void CDlgUtil03::DoDispenseTest()
// {
// 	BOOL bOn;
// 	double pTgtPos[2];
// 	pTgtPos[AXIS_Y] = _tstof(pDoc->WorkingInfo.Motion.sStPosY);
// 	pTgtPos[AXIS_X] = _tstof(pDoc->WorkingInfo.Motion.sStPosX);
// 	double dCurrX = pView->m_dEnc[AXIS_X];
// 	double dCurrY = pView->m_dEnc[AXIS_Y];
// 
// 	double fLen, fVel, fAcc, fJerk;
// 	fLen = sqrt( ((pTgtPos[AXIS_X] - dCurrX) * (pTgtPos[AXIS_X] - dCurrX)) + ((pTgtPos[AXIS_Y] - dCurrY) * (pTgtPos[AXIS_Y] - dCurrY)) );
// 	if(fLen > 0.001)
// 	{
// // 		bOn = pDoc->m_pSliceIo[7] & (0x01<<10) ? TRUE : FALSE;	// 마킹부 토크 클램프 스위치 램프 -> 마킹부 마킹 실린더 SOL
// // 		if(bOn)
// // 		{
// // 			if(pView->m_pDlgMenu03)
// // 			{
// // 				if(!pView->IsAuto())
// // 					pView->m_pDlgMenu03->SwMkDnSol(FALSE);
// // 			}
// // 			Sleep(300);
// // 		}
// 
// 		pView->m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X, fLen, fVel, fAcc, fJerk);
// 		if(!pView->m_pMotion->Move(MS_XY, pTgtPos, fVel, fAcc, fAcc))
// 		if(!pView->m_pMotion->Move(MS_XY, pTgtPos, fVel, fAcc, fAcc))
// 			AfxMessageBox(_T("Move XY Error..."));
// 	}
// 
// 	if(!pView->IsNoMk())
// 	{
// // 		bOn = pDoc->m_pSliceIo[7] & (0x01<<10) ? TRUE : FALSE;	// 마킹부 토크 클램프 스위치 램프 -> 마킹부 마킹 실린더 SOL
// // 		if(!bOn)
// // 		{
// // 			if(pView->m_pDlgMenu03)
// // 			{
// // 				if(!pView->IsAuto())
// // 					pView->m_pDlgMenu03->SwMkDnSol(TRUE);
// // 			}
// // 			Sleep(300);
// // 		}
// 
// 		SetFurge();
// 	}
// }

// void CDlgUtil03::SetFurge()
// {
// 	// Ink Marking ON
// // 	pDoc->m_pSliceIo[7] |= (0x01<<12);	// 마킹부 마킹 콘트롤러 출력 1(전원공급) -> Purge
// 	if(pView->m_pDlgMenu02)
// 	{
// 		pView->m_pDlgMenu02->SetMkCurPos();
// 
// // 		pView->m_pDlgMenu02->m_bMkDnSolOff = TRUE;
// 		pView->m_pDlgMenu02->SetMkDlyOff();
// 	}
// }

void CDlgUtil03::OnChkNoMk() 
{
	// TODO: Add your control notification handler code here
	BOOL bOn = ((CButton*)GetDlgItem(IDC_CHK_NO_MK))->GetCheck() ? TRUE : FALSE;
	pDoc->WorkingInfo.System.bNoMk = bOn;
}

BOOL CDlgUtil03::PreTranslateMessage(MSG* pMsg) 
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

void CDlgUtil03::OnStc00() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(0);
}

void CDlgUtil03::OnStc01() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(1);
}

void CDlgUtil03::OnStc02() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(2);
}

void CDlgUtil03::OnStc03() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(3);
}

void CDlgUtil03::OnStc04() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(4);
}

void CDlgUtil03::OnStc05() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(5);
}

void CDlgUtil03::OnStc06() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(6);
}

void CDlgUtil03::OnStc07() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(7);
}

void CDlgUtil03::OnStc08() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(8);
}

void CDlgUtil03::OnStc09() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(9);
}

void CDlgUtil03::OnStc010() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(10);	
}

void CDlgUtil03::OnStc10() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(11);
}

void CDlgUtil03::OnStc11() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(12);
}

void CDlgUtil03::OnStc12() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(13);
}

void CDlgUtil03::OnStc13() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(14);
}

void CDlgUtil03::OnStc14() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(15);
}

void CDlgUtil03::OnStc15() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(16);
}

void CDlgUtil03::OnStc16() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(17);
}

void CDlgUtil03::OnStc17() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(18);
}

void CDlgUtil03::OnStc18() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(19);
}

void CDlgUtil03::OnStc19() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(20);
}

void CDlgUtil03::OnStc110() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(21);	
}

void CDlgUtil03::OnStc20() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(22);
}

void CDlgUtil03::OnStc21() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(23);
}

void CDlgUtil03::OnStc22() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(24);
}

void CDlgUtil03::OnStc23() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(25);
}

void CDlgUtil03::OnStc24() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(26);
}

void CDlgUtil03::OnStc25() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(27);
}

void CDlgUtil03::OnStc26() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(28);
}

void CDlgUtil03::OnStc27() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(29);
}

void CDlgUtil03::OnStc28() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(30);
}

void CDlgUtil03::OnStc29() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(31);
}

void CDlgUtil03::OnStc210() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(32);	
}

void CDlgUtil03::OnStc30() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(33);
}

void CDlgUtil03::OnStc31() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(34);
}

void CDlgUtil03::OnStc32() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(35);
}

void CDlgUtil03::OnStc33() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(36);
}

void CDlgUtil03::OnStc34() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(37);
}

void CDlgUtil03::OnStc35() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(38);
}

void CDlgUtil03::OnStc36() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(39);
}

void CDlgUtil03::OnStc37() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(40);
}

void CDlgUtil03::OnStc38() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(41);
}

void CDlgUtil03::OnStc39() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(42);
}

void CDlgUtil03::OnStc310() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(43);	
}

void CDlgUtil03::OnStc40() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(44);
}

void CDlgUtil03::OnStc41() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(45);
}

void CDlgUtil03::OnStc42() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(46);
}

void CDlgUtil03::OnStc43() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(47);
}

void CDlgUtil03::OnStc44() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(48);
}

void CDlgUtil03::OnStc45() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(49);
}

void CDlgUtil03::OnStc46() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(50);
}

void CDlgUtil03::OnStc47() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(51);
}

void CDlgUtil03::OnStc48() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(52);
}

void CDlgUtil03::OnStc49() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(53);
}

void CDlgUtil03::OnStc410() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(54);
}

void CDlgUtil03::OnStc50() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(55);
}

void CDlgUtil03::OnStc51() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(56);
}

void CDlgUtil03::OnStc52() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(57);
}

void CDlgUtil03::OnStc53() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(58);
}

void CDlgUtil03::OnStc54() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(59);
}

void CDlgUtil03::OnStc55() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(60);
}

void CDlgUtil03::OnStc56() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(61);
}

void CDlgUtil03::OnStc57() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(62);
}

void CDlgUtil03::OnStc58() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(63);
}

void CDlgUtil03::OnStc59() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(64);
}

void CDlgUtil03::OnStc510() 
{
	// TODO: Add your control notification handler code here
	MoveMkPos(65);
}


void CDlgUtil03::OnBtnMkMoveInit() 
{
	// TODO: Add your control notification handler code here
	BOOL bOn0 = myBtn[3].GetCheck();
	BOOL bOn1 = myBtn[4].GetCheck();

	if(bOn0 && !bOn1)
		pView->m_pVoiceCoil[0]->MoveSmacShiftPos(0);
	else if(!bOn0 && bOn1)
		pView->m_pVoiceCoil[1]->MoveSmacShiftPos(1);
	else if(bOn0 && bOn1)
	{
		pView->m_pVoiceCoil[0]->MoveSmacShiftPos(0);
		pView->m_pVoiceCoil[1]->MoveSmacShiftPos(1);
	}
}

void CDlgUtil03::OnBtnMkHome() 
{
	// TODO: Add your control notification handler code here
	BOOL bOn0 = myBtn[3].GetCheck();
	BOOL bOn1 = myBtn[4].GetCheck();

	if(bOn0 && !bOn1)
		pView->m_pVoiceCoil[0]->SearchHomeSmac(0);
	else if(!bOn0 && bOn1)
		pView->m_pVoiceCoil[1]->SearchHomeSmac(1);
	else if(bOn0 && bOn1)
	{
		pView->m_pVoiceCoil[0]->SearchHomeSmac(0);
		pView->m_pVoiceCoil[1]->SearchHomeSmac(1);
	}
}

void CDlgUtil03::OnKillFocus(CWnd* pNewWnd) 
{
	CDialog::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	
}

BOOL CDlgUtil03::ShowKeypad(int nCtlID, CPoint ptSt, int nDir)
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

void CDlgUtil03::OnEditYshiftRatio() 
{
	// TODO: Add your control notification handler code here
	myStcData[66].SetBkColor(RGB_RED);
	myStcData[66].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_EDIT_YSHIFT_RATIO)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_EDIT_YSHIFT_RATIO, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[66].SetBkColor(RGB_WHITE);
	myStcData[66].RedrawWindow();

	CString sPath=PATH_WORKING_INFO;
	CString sData;
	GetDlgItem(IDC_EDIT_YSHIFT_RATIO)->GetWindowText(sData);
	pDoc->m_dShiftAdjustRatio = _tstof(sData);
	::WritePrivateProfileString(_T("System"), _T("RTR_SHIFT_ADJUST_RATIO"), sData, sPath);
}

void CDlgUtil03::OnCheckYshift() 
{
	// TODO: Add your control notification handler code here
	CString strData;

	if(((CButton*)GetDlgItem(IDC_CHECK_YSHIFT))->GetCheck())
		pDoc->m_bUseRTRYShiftAdjust = TRUE;
	else
		pDoc->m_bUseRTRYShiftAdjust = FALSE;

	CString sPath=PATH_WORKING_INFO;
	strData.Format(_T("%d"), pDoc->m_bUseRTRYShiftAdjust);
	::WritePrivateProfileString(_T("System"), _T("USE_RTR_SHIFT_ADJUST"), strData, sPath);
}


void CDlgUtil03::OnBnClickedCheckPcs()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Disp(ROT_NONE);

	BOOL bChk = ((CButton*)GetDlgItem(IDC_CHECK_PCS))->GetCheck();

	if (bChk)
		((CButton*)GetDlgItem(IDC_CHECK_PCS_DN))->ShowWindow(SW_SHOW);
	else
		((CButton*)GetDlgItem(IDC_CHECK_PCS_DN))->ShowWindow(SW_HIDE);	
}


void CDlgUtil03::OnBnClickedCheckPcsDn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Disp(ROT_NONE);
}
